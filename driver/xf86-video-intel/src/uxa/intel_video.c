/***************************************************************************

 Copyright 2000 Intel Corporation.  All Rights Reserved. 

 Permission is hereby granted, free of charge, to any person obtaining a 
 copy of this software and associated documentation files (the 
 "Software"), to deal in the Software without restriction, including 
 without limitation the rights to use, copy, modify, merge, publish, 
 distribute, sub license, and/or sell copies of the Software, and to 
 permit persons to whom the Software is furnished to do so, subject to 
 the following conditions: 

 The above copyright notice and this permission notice (including the 
 next paragraph) shall be included in all copies or substantial portions 
 of the Software. 

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. 
 IN NO EVENT SHALL INTEL, AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, 
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
 THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 **************************************************************************/

/*
 * i830_video.c: i830/i845 Xv driver. 
 *
 * Copyright © 2002 by Alan Hourihane and David Dawes
 *
 * Authors: 
 *	Alan Hourihane <alanh@tungstengraphics.com>
 *	David Dawes <dawes@xfree86.org>
 *
 * Derived from i810 Xv driver:
 *
 * Authors of i810 code:
 * 	Jonathan Bian <jonathan.bian@intel.com>
 *      Offscreen Images:
 *        Matt Sottek <matthew.j.sottek@intel.com>
 */

/*
 * XXX Could support more formats.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "xorg-server.h"
#include "xf86.h"
#include "xf86_OSproc.h"
#include "compiler.h"
#include "xf86Pci.h"
#include "xf86fbman.h"
#include "xf86drm.h"
#include "regionstr.h"
#include "randrstr.h"
#include "windowstr.h"
#include "damage.h"
#include "intel.h"
#include "intel_video.h"
#include "i830_reg.h"
#include "xf86xv.h"
#include <X11/extensions/Xv.h>
#include "dixstruct.h"
#include "fourcc.h"

#ifdef INTEL_XVMC
#define _INTEL_XVMC_SERVER_
#include "intel_xvmc.h"
#endif
#include "intel_glamor.h"

#define OFF_DELAY	250	/* milliseconds */

#define OFF_TIMER	0x01
#define CLIENT_VIDEO_ON	0x02

static XF86VideoAdaptorPtr I830SetupImageVideoOverlay(ScreenPtr);
static XF86VideoAdaptorPtr I830SetupImageVideoTextured(ScreenPtr);
static void I830StopVideo(ScrnInfoPtr, pointer, Bool);
static int I830SetPortAttributeOverlay(ScrnInfoPtr, Atom, INT32, pointer);
static int I830SetPortAttributeTextured(ScrnInfoPtr, Atom, INT32, pointer);
static int I830GetPortAttribute(ScrnInfoPtr, Atom, INT32 *, pointer);
static void I830QueryBestSize(ScrnInfoPtr, Bool,
			      short, short, short, short, unsigned int *,
			      unsigned int *, pointer);
static int I830PutImageTextured(ScrnInfoPtr, short, short, short, short, short, short,
			short, short, int, unsigned char *, short, short,
			Bool, RegionPtr, pointer, DrawablePtr);
static int I830PutImageOverlay(ScrnInfoPtr, short, short, short, short, short, short,
			short, short, int, unsigned char *, short, short,
			Bool, RegionPtr, pointer, DrawablePtr);
static int I830QueryImageAttributes(ScrnInfoPtr, int, unsigned short *,
				    unsigned short *, int *, int *);

#define MAKE_ATOM(a) MakeAtom(a, sizeof(a) - 1, TRUE)

static Atom xvBrightness, xvContrast, xvSaturation, xvColorKey, xvPipe;
static Atom xvGamma0, xvGamma1, xvGamma2, xvGamma3, xvGamma4, xvGamma5;
static Atom xvSyncToVblank;

/* Limits for the overlay/textured video source sizes.  The documented hardware
 * limits are 2048x2048 or better for overlay and both of our textured video
 * implementations.  Additionally, on the 830 and 845, larger sizes resulted in
 * the card hanging, so we keep the limits lower there.
 */
#define IMAGE_MAX_WIDTH		2048
#define IMAGE_MAX_HEIGHT	2048
#define IMAGE_MAX_WIDTH_LEGACY	1024
#define IMAGE_MAX_HEIGHT_LEGACY	1088

/* overlay debugging printf function */
#if 0
#define OVERLAY_DEBUG ErrorF
#else
#define OVERLAY_DEBUG if (0) ErrorF
#endif

/* client libraries expect an encoding */
static const XF86VideoEncodingRec DummyEncoding[1] = {
	{
	 0,
	 "XV_IMAGE",
	 IMAGE_MAX_WIDTH, IMAGE_MAX_HEIGHT,
	 {1, 1}
	 }
};

#define NUM_FORMATS 3

static XF86VideoFormatRec Formats[NUM_FORMATS] = {
	{15, TrueColor}, {16, TrueColor}, {24, TrueColor}
};

#define NUM_ATTRIBUTES 5
static XF86AttributeRec Attributes[NUM_ATTRIBUTES] = {
	{XvSettable | XvGettable, 0, (1 << 24) - 1, "XV_COLORKEY"},
	{XvSettable | XvGettable, -128, 127, "XV_BRIGHTNESS"},
	{XvSettable | XvGettable, 0, 255, "XV_CONTRAST"},
	{XvSettable | XvGettable, 0, 1023, "XV_SATURATION"},
	{XvSettable | XvGettable, -1, 1, "XV_PIPE"}
};

#define GAMMA_ATTRIBUTES 6
static XF86AttributeRec GammaAttributes[GAMMA_ATTRIBUTES] = {
	{XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA0"},
	{XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA1"},
	{XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA2"},
	{XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA3"},
	{XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA4"},
	{XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA5"}
};

#ifdef INTEL_XVMC
#define NUM_IMAGES 5
#define XVMC_IMAGE 1
#else
#define NUM_IMAGES 4
#define XVMC_IMAGE 0
#endif

static XF86ImageRec Images[NUM_IMAGES] = {
	XVIMAGE_YUY2,
	XVIMAGE_YV12,
	XVIMAGE_I420,
	XVIMAGE_UYVY,
#ifdef INTEL_XVMC
	{
	 /*
	  * Below, a dummy picture type that is used in XvPutImage only to do
	  * an overlay update. Introduced for the XvMC client lib.
	  * Defined to have a zero data size.
	  */
	 FOURCC_XVMC,
	 XvYUV,
	 LSBFirst,
	 {'X', 'V', 'M', 'C',
	  0x00, 0x00, 0x00, 0x10, 0x80, 0x00, 0x00, 0xAA, 0x00,
	  0x38, 0x9B, 0x71},
	 12,
	 XvPlanar,
	 3,
	 0, 0, 0, 0,
	 8, 8, 8,
	 1, 2, 2,
	 1, 2, 2,
	 {'Y', 'V', 'U',
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	 XvTopToBottom},
#endif
};

/* kernel modesetting overlay functions */
static Bool intel_has_overlay(intel_screen_private *intel)
{
	struct drm_i915_getparam gp;
	int has_overlay = 0;
	int ret;

	gp.param = I915_PARAM_HAS_OVERLAY;
	gp.value = &has_overlay;
	ret = drmCommandWriteRead(intel->drmSubFD, DRM_I915_GETPARAM, &gp, sizeof(gp));

	return ret == 0 && !! has_overlay;
}

static Bool intel_overlay_update_attrs(intel_screen_private *intel)
{
	intel_adaptor_private *adaptor_priv = intel_get_adaptor_private(intel);
	struct drm_intel_overlay_attrs attrs;

	attrs.flags = I915_OVERLAY_UPDATE_ATTRS;
	attrs.brightness = adaptor_priv->brightness;
	attrs.contrast = adaptor_priv->contrast;
	attrs.saturation = adaptor_priv->saturation;
	attrs.color_key = adaptor_priv->colorKey;
	attrs.gamma0 = adaptor_priv->gamma0;
	attrs.gamma1 = adaptor_priv->gamma1;
	attrs.gamma2 = adaptor_priv->gamma2;
	attrs.gamma3 = adaptor_priv->gamma3;
	attrs.gamma4 = adaptor_priv->gamma4;
	attrs.gamma5 = adaptor_priv->gamma5;

	return drmCommandWriteRead(intel->drmSubFD, DRM_I915_OVERLAY_ATTRS,
				   &attrs, sizeof(attrs)) == 0;
}

static void intel_overlay_off(intel_screen_private *intel)
{
	struct drm_intel_overlay_put_image request;
	int ret;

	request.flags = 0;

	ret = drmCommandWrite(intel->drmSubFD, DRM_I915_OVERLAY_PUT_IMAGE,
			      &request, sizeof(request));
	(void) ret;
}

static Bool
intel_overlay_put_image(intel_screen_private *intel,
			  xf86CrtcPtr crtc,
			  int id, short width, short height,
			  int dstPitch, int dstPitch2,
			  BoxPtr dstBox, short src_w, short src_h, short drw_w,
			  short drw_h)
{
	intel_adaptor_private *adaptor_priv = intel_get_adaptor_private(intel);
	struct drm_intel_overlay_put_image request;
	int ret;
	int planar = is_planar_fourcc(id);
	float scale;
	dri_bo *tmp;

	request.flags = I915_OVERLAY_ENABLE;

	request.bo_handle = adaptor_priv->buf->handle;
	if (planar) {
		request.stride_Y = dstPitch2;
		request.stride_UV = dstPitch;
	} else {
		request.stride_Y = dstPitch;
		request.stride_UV = 0;
	}
	request.offset_Y = adaptor_priv->YBufOffset;
	request.offset_U = adaptor_priv->UBufOffset;
	request.offset_V = adaptor_priv->VBufOffset;
	OVERLAY_DEBUG("off_Y: %i, off_U: %i, off_V: %i\n", request.offset_Y,
		      request.offset_U, request.offset_V);

	request.crtc_id = intel_crtc_id(crtc);
	request.dst_x = dstBox->x1;
	request.dst_y = dstBox->y1;
	request.dst_width = dstBox->x2 - dstBox->x1;
	request.dst_height = dstBox->y2 - dstBox->y1;

	request.src_width = width;
	request.src_height = height;
	/* adjust src dimensions */
	if (request.dst_height > 1) {
		scale = ((float)request.dst_height - 1) / ((float)drw_h - 1);
		request.src_scan_height = src_h * scale;
	} else
		request.src_scan_height = 1;

	if (request.dst_width > 1) {
		scale = ((float)request.dst_width - 1) / ((float)drw_w - 1);
		request.src_scan_width = src_w * scale;
	} else
		request.src_scan_width = 1;

	if (planar) {
		request.flags |= I915_OVERLAY_YUV_PLANAR | I915_OVERLAY_YUV420;
	} else {
		request.flags |= I915_OVERLAY_YUV_PACKED | I915_OVERLAY_YUV422;
		if (id == FOURCC_UYVY)
			request.flags |= I915_OVERLAY_Y_SWAP;
	}

	ret = drmCommandWrite(intel->drmSubFD, DRM_I915_OVERLAY_PUT_IMAGE,
			      &request, sizeof(request));
	if (ret)
		return FALSE;

	if (!adaptor_priv->reusable) {
		drm_intel_bo_unreference(adaptor_priv->buf);
		adaptor_priv->buf = NULL;
		adaptor_priv->reusable = TRUE;
	}

	tmp = adaptor_priv->old_buf[1];
	adaptor_priv->old_buf[1] = adaptor_priv->old_buf[0];
	adaptor_priv->old_buf[0] = adaptor_priv->buf;
	adaptor_priv->buf = tmp;

	return TRUE;
}

void I830InitVideo(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	XF86VideoAdaptorPtr *adaptors = NULL, *newAdaptors = NULL;
	XF86VideoAdaptorPtr overlayAdaptor = NULL, texturedAdaptor = NULL, glamorAdaptor = NULL;
	int num_adaptors = xf86XVListGenericAdaptors(scrn, &adaptors);
	/* Give our adaptor list enough space for the overlay and/or texture video
	 * adaptors.
	 */
	newAdaptors = realloc(adaptors,
			      (num_adaptors + 3) * sizeof(XF86VideoAdaptorPtr));
	if (newAdaptors == NULL) {
		free(adaptors);
		return;
	}
	adaptors = newAdaptors;

	/* Add the adaptors supported by our hardware.  First, set up the atoms
	 * that will be used by both output adaptors.
	 */
	xvBrightness = MAKE_ATOM("XV_BRIGHTNESS");
	xvContrast = MAKE_ATOM("XV_CONTRAST");

	/* Set up textured video if we can do it at this depth and we are on
	 * supported hardware.
	 */
	if (!intel->force_fallback &&
	    scrn->bitsPerPixel >= 16 &&
	    INTEL_INFO(intel)->gen >= 030 &&
	    INTEL_INFO(intel)->gen < 0100) {
		texturedAdaptor = I830SetupImageVideoTextured(screen);
		if (texturedAdaptor != NULL) {
			xf86DrvMsg(scrn->scrnIndex, X_INFO,
				   "Set up textured video\n");
		} else {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
				   "Failed to set up textured video\n");
		}
	}

	/* Set up overlay video if it is available */
	intel->use_overlay = intel_has_overlay(intel);
	if (intel->use_overlay) {
		overlayAdaptor = I830SetupImageVideoOverlay(screen);
		if (overlayAdaptor != NULL) {
			xf86DrvMsg(scrn->scrnIndex, X_INFO,
				   "Set up overlay video\n");
		} else {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
				   "Failed to set up overlay video\n");
		}
	}

	glamorAdaptor = intel_glamor_xv_init(screen, 16);
	if (glamorAdaptor != NULL)
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "Set up textured video using glamor\n");

	if (overlayAdaptor && intel->XvPreferOverlay)
		adaptors[num_adaptors++] = overlayAdaptor;

	if (texturedAdaptor)
		adaptors[num_adaptors++] = texturedAdaptor;

	if (glamorAdaptor)
		adaptors[num_adaptors++] = glamorAdaptor;

	if (overlayAdaptor && !intel->XvPreferOverlay)
		adaptors[num_adaptors++] = overlayAdaptor;

	if (num_adaptors) {
		xf86XVScreenInit(screen, adaptors, num_adaptors);
	} else {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "Disabling Xv because no adaptors could be initialized.\n");
		intel->XvEnabled = FALSE;
	}

#ifdef INTEL_XVMC
	if (texturedAdaptor)
		intel_xvmc_adaptor_init(screen);
#endif
	free(adaptors);
}

static XF86VideoAdaptorPtr I830SetupImageVideoOverlay(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	XF86VideoAdaptorPtr adapt;
	intel_adaptor_private *adaptor_priv;
	XF86AttributePtr att;

	OVERLAY_DEBUG("I830SetupImageVideoOverlay\n");

	if (!(adapt = calloc(1,
			     sizeof(XF86VideoAdaptorRec) +
			     sizeof(intel_adaptor_private) +
			     sizeof(DevUnion))))
		return NULL;

	adapt->type = XvWindowMask | XvInputMask | XvImageMask;
	adapt->flags = VIDEO_OVERLAID_IMAGES /*| VIDEO_CLIP_TO_VIEWPORT */ ;
	adapt->name = "Intel(R) Video Overlay";
	adapt->nEncodings = 1;
	adapt->pEncodings = xnfalloc(sizeof(DummyEncoding));
	memcpy(adapt->pEncodings, DummyEncoding, sizeof(DummyEncoding));
	if (IS_845G(intel) || IS_I830(intel)) {
		adapt->pEncodings->width = IMAGE_MAX_WIDTH_LEGACY;
		adapt->pEncodings->height = IMAGE_MAX_HEIGHT_LEGACY;
	}
	adapt->nFormats = NUM_FORMATS;
	adapt->pFormats = Formats;
	adapt->nPorts = 1;
	adapt->pPortPrivates = (DevUnion *) (&adapt[1]);

	adaptor_priv = (intel_adaptor_private *)&adapt->pPortPrivates[1];

	adapt->pPortPrivates[0].ptr = (pointer) (adaptor_priv);
	adapt->nAttributes = NUM_ATTRIBUTES;
	if (INTEL_INFO(intel)->gen >= 030)
		adapt->nAttributes += GAMMA_ATTRIBUTES;	/* has gamma */
	adapt->pAttributes =
	    xnfalloc(sizeof(XF86AttributeRec) * adapt->nAttributes);
	/* Now copy the attributes */
	att = adapt->pAttributes;
	memcpy((char *)att, (char *)Attributes,
	       sizeof(XF86AttributeRec) * NUM_ATTRIBUTES);
	att += NUM_ATTRIBUTES;
	if (INTEL_INFO(intel)->gen >= 030) {
		memcpy((char *)att, (char *)GammaAttributes,
		       sizeof(XF86AttributeRec) * GAMMA_ATTRIBUTES);
	}
	adapt->nImages = NUM_IMAGES - XVMC_IMAGE;

	adapt->pImages = Images;
	adapt->PutVideo = NULL;
	adapt->PutStill = NULL;
	adapt->GetVideo = NULL;
	adapt->GetStill = NULL;
	adapt->StopVideo = I830StopVideo;
	adapt->SetPortAttribute = I830SetPortAttributeOverlay;
	adapt->GetPortAttribute = I830GetPortAttribute;
	adapt->QueryBestSize = I830QueryBestSize;
	adapt->PutImage = I830PutImageOverlay;
	adapt->QueryImageAttributes = I830QueryImageAttributes;

	adaptor_priv->textured = FALSE;
	adaptor_priv->colorKey = intel->colorKey & ((1 << scrn->depth) - 1);
	adaptor_priv->videoStatus = 0;
	adaptor_priv->brightness = -19;	/* (255/219) * -16 */
	adaptor_priv->contrast = 75;	/* 255/219 * 64 */
	adaptor_priv->saturation = 146;	/* 128/112 * 128 */
	adaptor_priv->desired_crtc = NULL;
	adaptor_priv->buf = NULL;
	adaptor_priv->old_buf[0] = NULL;
	adaptor_priv->old_buf[1] = NULL;
	adaptor_priv->gamma5 = 0xc0c0c0;
	adaptor_priv->gamma4 = 0x808080;
	adaptor_priv->gamma3 = 0x404040;
	adaptor_priv->gamma2 = 0x202020;
	adaptor_priv->gamma1 = 0x101010;
	adaptor_priv->gamma0 = 0x080808;

	adaptor_priv->rotation = RR_Rotate_0;

	/* gotta uninit this someplace */
	REGION_NULL(screen, &adaptor_priv->clip);

	intel->adaptor = adapt;

	xvColorKey = MAKE_ATOM("XV_COLORKEY");
	xvBrightness = MAKE_ATOM("XV_BRIGHTNESS");
	xvContrast = MAKE_ATOM("XV_CONTRAST");
	xvSaturation = MAKE_ATOM("XV_SATURATION");

	/* Allow the pipe to be switched from pipe A to B when in clone mode */
	xvPipe = MAKE_ATOM("XV_PIPE");

	if (INTEL_INFO(intel)->gen >= 030) {
		xvGamma0 = MAKE_ATOM("XV_GAMMA0");
		xvGamma1 = MAKE_ATOM("XV_GAMMA1");
		xvGamma2 = MAKE_ATOM("XV_GAMMA2");
		xvGamma3 = MAKE_ATOM("XV_GAMMA3");
		xvGamma4 = MAKE_ATOM("XV_GAMMA4");
		xvGamma5 = MAKE_ATOM("XV_GAMMA5");
	}

	intel_overlay_update_attrs(intel);

	return adapt;
}

static XF86VideoAdaptorPtr I830SetupImageVideoTextured(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	XF86VideoAdaptorPtr adapt;
	intel_adaptor_private *adaptor_privs;
	DevUnion *devUnions;
	int nports = 16, i;

	OVERLAY_DEBUG("I830SetupImageVideoOverlay\n");

	adapt = calloc(1, sizeof(XF86VideoAdaptorRec));
	adaptor_privs = calloc(nports, sizeof(intel_adaptor_private));
	devUnions = calloc(nports, sizeof(DevUnion));
	if (adapt == NULL || adaptor_privs == NULL || devUnions == NULL) {
		free(adapt);
		free(adaptor_privs);
		free(devUnions);
		return NULL;
	}

	adapt->type = XvWindowMask | XvInputMask | XvImageMask;
	adapt->flags = 0;
	adapt->name = "Intel(R) Textured Video";
	adapt->nEncodings = 1;
	adapt->pEncodings = xnfalloc(sizeof(DummyEncoding));
	memcpy(adapt->pEncodings, DummyEncoding, sizeof(DummyEncoding));
	adapt->nFormats = NUM_FORMATS;
	adapt->pFormats = Formats;
	adapt->nPorts = nports;
	adapt->pPortPrivates = devUnions;
	adapt->nAttributes = 0;
	adapt->pAttributes = NULL;
	if (IS_I915G(intel) || IS_I915GM(intel))
		adapt->nImages = NUM_IMAGES - XVMC_IMAGE;
	else
		adapt->nImages = NUM_IMAGES;

	adapt->pImages = Images;
	adapt->PutVideo = NULL;
	adapt->PutStill = NULL;
	adapt->GetVideo = NULL;
	adapt->GetStill = NULL;
	adapt->StopVideo = I830StopVideo;
	adapt->SetPortAttribute = I830SetPortAttributeTextured;
	adapt->GetPortAttribute = I830GetPortAttribute;
	adapt->QueryBestSize = I830QueryBestSize;
	adapt->PutImage = I830PutImageTextured;
	adapt->QueryImageAttributes = I830QueryImageAttributes;

	for (i = 0; i < nports; i++) {
		intel_adaptor_private *adaptor_priv = &adaptor_privs[i];

		adaptor_priv->textured = TRUE;
		adaptor_priv->videoStatus = 0;
		adaptor_priv->buf = NULL;
		adaptor_priv->old_buf[0] = NULL;
		adaptor_priv->old_buf[1] = NULL;

		adaptor_priv->rotation = RR_Rotate_0;
		adaptor_priv->SyncToVblank = 1;

		/* gotta uninit this someplace, XXX: shouldn't be necessary for textured */
		REGION_NULL(screen, &adaptor_priv->clip);

		adapt->pPortPrivates[i].ptr = (pointer) (adaptor_priv);
	}

	xvSyncToVblank = MAKE_ATOM("XV_SYNC_TO_VBLANK");

	return adapt;
}

static void intel_free_video_buffers(intel_adaptor_private *adaptor_priv)
{
	int i;

	for (i = 0; i < 2; i++) {
		if (adaptor_priv->old_buf[i]) {
			drm_intel_bo_disable_reuse(adaptor_priv->old_buf[i]);
			drm_intel_bo_unreference(adaptor_priv->old_buf[i]);
			adaptor_priv->old_buf[i] = NULL;
		}
	}

	if (adaptor_priv->buf) {
		drm_intel_bo_unreference(adaptor_priv->buf);
		adaptor_priv->buf = NULL;
	}
}

static void I830StopVideo(ScrnInfoPtr scrn, pointer data, Bool shutdown)
{
	intel_adaptor_private *adaptor_priv = (intel_adaptor_private *) data;

	if (adaptor_priv->textured)
		return;

	OVERLAY_DEBUG("I830StopVideo\n");

	REGION_EMPTY(scrn->pScreen, &adaptor_priv->clip);

	if (shutdown) {
		if (adaptor_priv->videoStatus & CLIENT_VIDEO_ON)
			intel_overlay_off(intel_get_screen_private(scrn));

		intel_free_video_buffers(adaptor_priv);
		adaptor_priv->videoStatus = 0;
	} else {
		if (adaptor_priv->videoStatus & CLIENT_VIDEO_ON) {
			adaptor_priv->videoStatus |= OFF_TIMER;
			adaptor_priv->offTime = currentTime.milliseconds + OFF_DELAY;
		}
	}

}

static int
I830SetPortAttributeTextured(ScrnInfoPtr scrn,
			     Atom attribute, INT32 value, pointer data)
{
	intel_adaptor_private *adaptor_priv = (intel_adaptor_private *) data;

	if (attribute == xvBrightness) {
		if ((value < -128) || (value > 127))
			return BadValue;
		adaptor_priv->brightness = value;
		return Success;
	} else if (attribute == xvContrast) {
		if ((value < 0) || (value > 255))
			return BadValue;
		adaptor_priv->contrast = value;
		return Success;
	} else if (attribute == xvSyncToVblank) {
		if ((value < -1) || (value > 1))
			return BadValue;
		adaptor_priv->SyncToVblank = value;
		return Success;
	} else {
		return BadMatch;
	}
}

static int
I830SetPortAttributeOverlay(ScrnInfoPtr scrn,
			    Atom attribute, INT32 value, pointer data)
{
	intel_adaptor_private *adaptor_priv = (intel_adaptor_private *) data;
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (attribute == xvBrightness) {
		if ((value < -128) || (value > 127))
			return BadValue;
		adaptor_priv->brightness = value;
		OVERLAY_DEBUG("BRIGHTNESS\n");
	} else if (attribute == xvContrast) {
		if ((value < 0) || (value > 255))
			return BadValue;
		adaptor_priv->contrast = value;
		OVERLAY_DEBUG("CONTRAST\n");
	} else if (attribute == xvSaturation) {
		if ((value < 0) || (value > 1023))
			return BadValue;
		adaptor_priv->saturation = value;
	} else if (attribute == xvPipe) {
		xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
		if ((value < -1) || (value >= xf86_config->num_crtc))
			return BadValue;
		if (value < 0)
			adaptor_priv->desired_crtc = NULL;
		else
			adaptor_priv->desired_crtc = xf86_config->crtc[value];
	} else if (attribute == xvGamma0 && (INTEL_INFO(intel)->gen >= 030)) {
		adaptor_priv->gamma0 = value;
	} else if (attribute == xvGamma1 && (INTEL_INFO(intel)->gen >= 030)) {
		adaptor_priv->gamma1 = value;
	} else if (attribute == xvGamma2 && (INTEL_INFO(intel)->gen >= 030)) {
		adaptor_priv->gamma2 = value;
	} else if (attribute == xvGamma3 && (INTEL_INFO(intel)->gen >= 030)) {
		adaptor_priv->gamma3 = value;
	} else if (attribute == xvGamma4 && (INTEL_INFO(intel)->gen >= 030)) {
		adaptor_priv->gamma4 = value;
	} else if (attribute == xvGamma5 && (INTEL_INFO(intel)->gen >= 030)) {
		adaptor_priv->gamma5 = value;
	} else if (attribute == xvColorKey) {
		adaptor_priv->colorKey = value;
		OVERLAY_DEBUG("COLORKEY\n");
	} else
		return BadMatch;

	if ((attribute == xvGamma0 ||
	     attribute == xvGamma1 ||
	     attribute == xvGamma2 ||
	     attribute == xvGamma3 ||
	     attribute == xvGamma4 ||
	     attribute == xvGamma5) && (INTEL_INFO(intel)->gen >= 030)) {
		OVERLAY_DEBUG("GAMMA\n");
	}

	if (!intel_overlay_update_attrs(intel))
		return BadValue;

	if (attribute == xvColorKey)
		REGION_EMPTY(scrn->pScreen, &adaptor_priv->clip);

	return Success;
}

static int
I830GetPortAttribute(ScrnInfoPtr scrn,
		     Atom attribute, INT32 * value, pointer data)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	intel_adaptor_private *adaptor_priv = (intel_adaptor_private *) data;

	if (attribute == xvBrightness) {
		*value = adaptor_priv->brightness;
	} else if (attribute == xvContrast) {
		*value = adaptor_priv->contrast;
	} else if (attribute == xvSaturation) {
		*value = adaptor_priv->saturation;
	} else if (attribute == xvPipe) {
		int c;
		xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
		for (c = 0; c < xf86_config->num_crtc; c++)
			if (xf86_config->crtc[c] == adaptor_priv->desired_crtc)
				break;
		if (c == xf86_config->num_crtc)
			c = -1;
		*value = c;
	} else if (attribute == xvGamma0 && (INTEL_INFO(intel)->gen >= 030)) {
		*value = adaptor_priv->gamma0;
	} else if (attribute == xvGamma1 && (INTEL_INFO(intel)->gen >= 030)) {
		*value = adaptor_priv->gamma1;
	} else if (attribute == xvGamma2 && (INTEL_INFO(intel)->gen >= 030)) {
		*value = adaptor_priv->gamma2;
	} else if (attribute == xvGamma3 && (INTEL_INFO(intel)->gen >= 030)) {
		*value = adaptor_priv->gamma3;
	} else if (attribute == xvGamma4 && (INTEL_INFO(intel)->gen >= 030)) {
		*value = adaptor_priv->gamma4;
	} else if (attribute == xvGamma5 && (INTEL_INFO(intel)->gen >= 030)) {
		*value = adaptor_priv->gamma5;
	} else if (attribute == xvColorKey) {
		*value = adaptor_priv->colorKey;
	} else if (attribute == xvSyncToVblank) {
		*value = adaptor_priv->SyncToVblank;
	} else
		return BadMatch;

	return Success;
}

static void
I830QueryBestSize(ScrnInfoPtr scrn,
		  Bool motion,
		  short vid_w, short vid_h,
		  short drw_w, short drw_h,
		  unsigned int *p_w, unsigned int *p_h, pointer data)
{
	if (vid_w > (drw_w << 1))
		drw_w = vid_w >> 1;
	if (vid_h > (drw_h << 1))
		drw_h = vid_h >> 1;

	*p_w = drw_w;
	*p_h = drw_h;
}

static Bool
I830CopyPackedData(intel_adaptor_private *adaptor_priv,
		   unsigned char *buf,
		   int srcPitch, int dstPitch, int top, int left, int h, int w)
{
	unsigned char *src, *dst, *dst_base;
	int i, j;
	unsigned char *s;

#if 0
	ErrorF("I830CopyPackedData: (%d,%d) (%d,%d)\n"
	       "srcPitch: %d, dstPitch: %d\n", top, left, h, w,
	       srcPitch, dstPitch);
#endif

	src = buf + (top * srcPitch) + (left << 1);

	if (drm_intel_gem_bo_map_gtt(adaptor_priv->buf))
		return FALSE;

	dst_base = adaptor_priv->buf->virtual;

	dst = dst_base + adaptor_priv->YBufOffset;

	switch (adaptor_priv->rotation) {
	case RR_Rotate_0:
		w <<= 1;
		for (i = 0; i < h; i++) {
			memcpy(dst, src, w);
			src += srcPitch;
			dst += dstPitch;
		}
		break;
	case RR_Rotate_90:
		h <<= 1;
		for (i = 0; i < h; i += 2) {
			s = src;
			for (j = 0; j < w; j++) {
				/* Copy Y */
				dst[(i + 0) + ((w - j - 1) * dstPitch)] = *s++;
				(void)*s++;
			}
			src += srcPitch;
		}
		h >>= 1;
		src = buf + (top * srcPitch) + (left << 1);
		for (i = 0; i < h; i += 2) {
			for (j = 0; j < w; j += 2) {
				/* Copy U */
				dst[((i * 2) + 1) + ((w - j - 1) * dstPitch)] =
				    src[(j * 2) + 1 + (i * srcPitch)];
				dst[((i * 2) + 1) + ((w - j - 2) * dstPitch)] =
				    src[(j * 2) + 1 + ((i + 1) * srcPitch)];
				/* Copy V */
				dst[((i * 2) + 3) + ((w - j - 1) * dstPitch)] =
				    src[(j * 2) + 3 + (i * srcPitch)];
				dst[((i * 2) + 3) + ((w - j - 2) * dstPitch)] =
				    src[(j * 2) + 3 + ((i + 1) * srcPitch)];
			}
		}
		break;
	case RR_Rotate_180:
		w <<= 1;
		for (i = 0; i < h; i++) {
			s = src;
			for (j = 0; j < w; j += 4) {
				dst[(w - j - 4) + ((h - i - 1) * dstPitch)] =
				    *s++;
				dst[(w - j - 3) + ((h - i - 1) * dstPitch)] =
				    *s++;
				dst[(w - j - 2) + ((h - i - 1) * dstPitch)] =
				    *s++;
				dst[(w - j - 1) + ((h - i - 1) * dstPitch)] =
				    *s++;
			}
			src += srcPitch;
		}
		break;
	case RR_Rotate_270:
		h <<= 1;
		for (i = 0; i < h; i += 2) {
			s = src;
			for (j = 0; j < w; j++) {
				/* Copy Y */
				dst[(h - i - 2) + (j * dstPitch)] = *s++;
				(void)*s++;
			}
			src += srcPitch;
		}
		h >>= 1;
		src = buf + (top * srcPitch) + (left << 1);
		for (i = 0; i < h; i += 2) {
			for (j = 0; j < w; j += 2) {
				/* Copy U */
				dst[(((h - i) * 2) - 3) + (j * dstPitch)] =
				    src[(j * 2) + 1 + (i * srcPitch)];
				dst[(((h - i) * 2) - 3) +
				    ((j + 1) * dstPitch)] =
				    src[(j * 2) + 1 + ((i + 1) * srcPitch)];
				/* Copy V */
				dst[(((h - i) * 2) - 1) + (j * dstPitch)] =
				    src[(j * 2) + 3 + (i * srcPitch)];
				dst[(((h - i) * 2) - 1) +
				    ((j + 1) * dstPitch)] =
				    src[(j * 2) + 3 + ((i + 1) * srcPitch)];
			}
		}
		break;
	}

	drm_intel_gem_bo_unmap_gtt(adaptor_priv->buf);
	return TRUE;
}

static void intel_memcpy_plane(unsigned char *dst, unsigned char *src,
			       int height, int width,
			       int dstPitch, int srcPitch, Rotation rotation)
{
	int i, j = 0;
	unsigned char *s;

	switch (rotation) {
	case RR_Rotate_0:
		/* optimise for the case of no clipping */
		if (srcPitch == dstPitch && srcPitch == width)
			memcpy(dst, src, srcPitch * height);
		else
			for (i = 0; i < height; i++) {
				memcpy(dst, src, width);
				src += srcPitch;
				dst += dstPitch;
			}
		break;
	case RR_Rotate_90:
		for (i = 0; i < height; i++) {
			s = src;
			for (j = 0; j < width; j++) {
				dst[(i) + ((width - j - 1) * dstPitch)] = *s++;
			}
			src += srcPitch;
		}
		break;
	case RR_Rotate_180:
		for (i = 0; i < height; i++) {
			s = src;
			for (j = 0; j < width; j++) {
				dst[(width - j - 1) +
				    ((height - i - 1) * dstPitch)] = *s++;
			}
			src += srcPitch;
		}
		break;
	case RR_Rotate_270:
		for (i = 0; i < height; i++) {
			s = src;
			for (j = 0; j < width; j++) {
				dst[(height - i - 1) + (j * dstPitch)] = *s++;
			}
			src += srcPitch;
		}
		break;
	}
}

static Bool
I830CopyPlanarData(intel_adaptor_private *adaptor_priv,
		   unsigned char *buf, int srcPitch, int srcPitch2,
		   int dstPitch, int dstPitch2,
		   int srcH, int top, int left,
		   int h, int w, int id)
{
	unsigned char *src1, *src2, *src3, *dst_base, *dst1, *dst2, *dst3;

#if 0
	ErrorF("I830CopyPlanarData: srcPitch %d, srcPitch %d, dstPitch %d\n"
	       "nlines %d, npixels %d, top %d, left %d\n",
	       srcPitch, srcPitch2, dstPitch, h, w, top, left);
#endif

	/* Copy Y data */
	src1 = buf + (top * srcPitch) + left;
#if 0
	ErrorF("src1 is %p, offset is %ld\n", src1,
	       (unsigned long)src1 - (unsigned long)buf);
#endif

	if (drm_intel_gem_bo_map_gtt(adaptor_priv->buf))
		return FALSE;

	dst_base = adaptor_priv->buf->virtual;

	dst1 = dst_base + adaptor_priv->YBufOffset;

	intel_memcpy_plane(dst1, src1, h, w, dstPitch2, srcPitch,
			  adaptor_priv->rotation);

	/* Copy V data for YV12, or U data for I420 */
	src2 = buf +		/* start of YUV data */
	    (srcH * srcPitch) +	/* move over Luma plane */
	    ((top >> 1) * srcPitch2) +	/* move down from by top lines */
	    (left >> 1);	/* move left by left pixels */

#if 0
	ErrorF("src2 is %p, offset is %ld\n", src2,
	       (unsigned long)src2 - (unsigned long)buf);
#endif
	if (id == FOURCC_I420)
		dst2 = dst_base + adaptor_priv->UBufOffset;
	else
		dst2 = dst_base + adaptor_priv->VBufOffset;

	intel_memcpy_plane(dst2, src2, h / 2, w / 2,
			  dstPitch, srcPitch2, adaptor_priv->rotation);

	/* Copy U data for YV12, or V data for I420 */
	src3 = buf +		/* start of YUV data */
	    (srcH * srcPitch) +	/* move over Luma plane */
	    ((srcH >> 1) * srcPitch2) +	/* move over Chroma plane */
	    ((top >> 1) * srcPitch2) +	/* move down from by top lines */
	    (left >> 1);	/* move left by left pixels */
#if 0
	ErrorF("src3 is %p, offset is %ld\n", src3,
	       (unsigned long)src3 - (unsigned long)buf);
#endif
	if (id == FOURCC_I420)
		dst3 = dst_base + adaptor_priv->VBufOffset;
	else
		dst3 = dst_base + adaptor_priv->UBufOffset;

	intel_memcpy_plane(dst3, src3, h / 2, w / 2,
			  dstPitch, srcPitch2, adaptor_priv->rotation);

	drm_intel_gem_bo_unmap_gtt(adaptor_priv->buf);
	return TRUE;
}

static void intel_box_intersect(BoxPtr dest, BoxPtr a, BoxPtr b)
{
	dest->x1 = a->x1 > b->x1 ? a->x1 : b->x1;
	dest->x2 = a->x2 < b->x2 ? a->x2 : b->x2;
	if (dest->x1 >= dest->x2) {
		dest->x1 = dest->x2 = dest->y1 = dest->y2 = 0;
		return;
	}

	dest->y1 = a->y1 > b->y1 ? a->y1 : b->y1;
	dest->y2 = a->y2 < b->y2 ? a->y2 : b->y2;
	if (dest->y1 >= dest->y2)
		dest->x1 = dest->x2 = dest->y1 = dest->y2 = 0;
}

static void intel_crtc_box(xf86CrtcPtr crtc, BoxPtr crtc_box)
{
	if (crtc->enabled) {
		crtc_box->x1 = crtc->x;
		crtc_box->x2 =
		    crtc->x + xf86ModeWidth(&crtc->mode, crtc->rotation);
		crtc_box->y1 = crtc->y;
		crtc_box->y2 =
		    crtc->y + xf86ModeHeight(&crtc->mode, crtc->rotation);
	} else
		crtc_box->x1 = crtc_box->x2 = crtc_box->y1 = crtc_box->y2 = 0;
}

static int intel_box_area(BoxPtr box)
{
	return (int)(box->x2 - box->x1) * (int)(box->y2 - box->y1);
}

/*
 * Return the crtc covering 'box'. If two crtcs cover a portion of
 * 'box', then prefer 'desired'. If 'desired' is NULL, then prefer the crtc
 * with greater coverage
 */

xf86CrtcPtr
intel_covering_crtc(ScrnInfoPtr scrn,
		    BoxPtr box, xf86CrtcPtr desired, BoxPtr crtc_box_ret)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	xf86CrtcPtr crtc, best_crtc;
	int coverage, best_coverage;
	int c;
	BoxRec crtc_box, cover_box;

	best_crtc = NULL;
	best_coverage = 0;
	crtc_box_ret->x1 = 0;
	crtc_box_ret->x2 = 0;
	crtc_box_ret->y1 = 0;
	crtc_box_ret->y2 = 0;
	for (c = 0; c < xf86_config->num_crtc; c++) {
		crtc = xf86_config->crtc[c];

		/* If the CRTC is off, treat it as not covering */
		if (!intel_crtc_on(crtc))
			continue;

		intel_crtc_box(crtc, &crtc_box);
		intel_box_intersect(&cover_box, &crtc_box, box);
		coverage = intel_box_area(&cover_box);
		if (coverage && crtc == desired) {
			*crtc_box_ret = crtc_box;
			return crtc;
		}
		if (coverage > best_coverage) {
			*crtc_box_ret = crtc_box;
			best_crtc = crtc;
			best_coverage = coverage;
		}
	}
	return best_crtc;
}

static void
intel_update_dst_box_to_crtc_coords(ScrnInfoPtr scrn, xf86CrtcPtr crtc,
				    BoxPtr dstBox)
{
	int tmp;

	/* for overlay, we should take it from crtc's screen
	 * coordinate to current crtc's display mode.
	 * yeah, a bit confusing.
	 */
	switch (crtc->rotation & 0xf) {
	case RR_Rotate_0:
		dstBox->x1 -= crtc->x;
		dstBox->x2 -= crtc->x;
		dstBox->y1 -= crtc->y;
		dstBox->y2 -= crtc->y;
		break;
	case RR_Rotate_90:
		tmp = dstBox->x1;
		dstBox->x1 = dstBox->y1 - crtc->x;
		dstBox->y1 = scrn->virtualX - tmp - crtc->y;
		tmp = dstBox->x2;
		dstBox->x2 = dstBox->y2 - crtc->x;
		dstBox->y2 = scrn->virtualX - tmp - crtc->y;
		tmp = dstBox->y1;
		dstBox->y1 = dstBox->y2;
		dstBox->y2 = tmp;
		break;
	case RR_Rotate_180:
		tmp = dstBox->x1;
		dstBox->x1 = scrn->virtualX - dstBox->x2 - crtc->x;
		dstBox->x2 = scrn->virtualX - tmp - crtc->x;
		tmp = dstBox->y1;
		dstBox->y1 = scrn->virtualY - dstBox->y2 - crtc->y;
		dstBox->y2 = scrn->virtualY - tmp - crtc->y;
		break;
	case RR_Rotate_270:
		tmp = dstBox->x1;
		dstBox->x1 = scrn->virtualY - dstBox->y1 - crtc->x;
		dstBox->y1 = tmp - crtc->y;
		tmp = dstBox->x2;
		dstBox->x2 = scrn->virtualY - dstBox->y2 - crtc->x;
		dstBox->y2 = tmp - crtc->y;
		tmp = dstBox->x1;
		dstBox->x1 = dstBox->x2;
		dstBox->x2 = tmp;
		break;
	}

	return;
}

int is_planar_fourcc(int id)
{
	switch (id) {
	case FOURCC_YV12:
	case FOURCC_I420:
#ifdef INTEL_XVMC
	case FOURCC_XVMC:
#endif
		return 1;
	case FOURCC_UYVY:
	case FOURCC_YUY2:
		return 0;
	default:
		ErrorF("Unknown format 0x%x\n", id);
		return 0;
	}
}

static int xvmc_passthrough(int id)
{
#ifdef INTEL_XVMC
	return id == FOURCC_XVMC;
#else
	return 0;
#endif
}

static Bool
intel_display_overlay(ScrnInfoPtr scrn, xf86CrtcPtr crtc,
		      int id, short width, short height,
		      int dstPitch, int dstPitch2,
		      BoxPtr dstBox, short src_w, short src_h, short drw_w,
		      short drw_h)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int tmp;

	OVERLAY_DEBUG("I830DisplayVideo: %dx%d (pitch %d)\n", width, height,
		      dstPitch);

	/*
	 * If the video isn't visible on any CRTC, turn it off
	 */
	if (!crtc) {
		intel_overlay_off(intel);
		return TRUE;
	}

	intel_update_dst_box_to_crtc_coords(scrn, crtc, dstBox);

	if (crtc->rotation & (RR_Rotate_90 | RR_Rotate_270)) {
		tmp = width;
		width = height;
		height = tmp;
		tmp = drw_w;
		drw_w = drw_h;
		drw_h = tmp;
		tmp = src_w;
		src_w = src_h;
		src_h = tmp;
	}

	return intel_overlay_put_image(intel, crtc, id,
					 width, height,
					 dstPitch, dstPitch2, dstBox,
					 src_w, src_h, drw_w, drw_h);
}

static Bool
intel_clip_video_helper(ScrnInfoPtr scrn,
			intel_adaptor_private *adaptor_priv,
			xf86CrtcPtr * crtc_ret,
			BoxPtr dst,
			short src_x, short src_y,
			short drw_x, short drw_y,
			short src_w, short src_h,
			short drw_w, short drw_h,
			int id,
			int *top, int* left, int* npixels, int *nlines,
			RegionPtr reg, INT32 width, INT32 height)
{
	Bool ret;
	RegionRec crtc_region_local;
	RegionPtr crtc_region = reg;
	BoxRec crtc_box;
	INT32 x1, x2, y1, y2;
	xf86CrtcPtr crtc;

	x1 = src_x;
	x2 = src_x + src_w;
	y1 = src_y;
	y2 = src_y + src_h;

	dst->x1 = drw_x;
	dst->x2 = drw_x + drw_w;
	dst->y1 = drw_y;
	dst->y2 = drw_y + drw_h;

	/*
	 * For overlay video, compute the relevant CRTC and
	 * clip video to that
	 */
	crtc = intel_covering_crtc(scrn, dst, adaptor_priv->desired_crtc,
				   &crtc_box);

	/* For textured video, we don't actually want to clip at all. */
	if (crtc && !adaptor_priv->textured) {
		REGION_INIT(screen, &crtc_region_local, &crtc_box, 1);
		crtc_region = &crtc_region_local;
		REGION_INTERSECT(screen, crtc_region, crtc_region,
				 reg);
	}
	*crtc_ret = crtc;

	ret = xf86XVClipVideoHelper(dst, &x1, &x2, &y1, &y2,
				    crtc_region, width, height);
	if (crtc_region != reg)
		REGION_UNINIT(screen, &crtc_region_local);

	*top = y1 >> 16;
	*left = (x1 >> 16) & ~1;
	*npixels = ALIGN(((x2 + 0xffff) >> 16), 2) - *left;
	if (is_planar_fourcc(id)) {
		*top &= ~1;
		*nlines = ALIGN(((y2 + 0xffff) >> 16), 2) - *top;
	} else
		*nlines = ((y2 + 0xffff) >> 16) - *top;

	return ret;
}

static void
intel_wait_for_scanline(ScrnInfoPtr scrn, PixmapPtr pixmap,
			xf86CrtcPtr crtc, RegionPtr clipBoxes)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	pixman_box16_t box, crtc_box;
	int pipe, event;
	Bool full_height;
	int y1, y2;

	pipe = -1;
	if (scrn->vtSema && pixmap_is_scanout(pixmap))
		pipe = intel_crtc_to_pipe(crtc);
	if (pipe < 0)
		return;

	box = *REGION_EXTENTS(unused, clipBoxes);

	if (crtc->transform_in_use)
		pixman_f_transform_bounds(&crtc->f_framebuffer_to_crtc, &box);

	/* We could presume the clip was correctly computed... */
	intel_crtc_box(crtc, &crtc_box);
	intel_box_intersect(&box, &crtc_box, &box);

	/*
	 * Make sure we don't wait for a scanline that will
	 * never occur
	 */
	y1 = (crtc_box.y1 <= box.y1) ? box.y1 - crtc_box.y1 : 0;
	y2 = (box.y2 <= crtc_box.y2) ?
		box.y2 - crtc_box.y1 : crtc_box.y2 - crtc_box.y1;
	if (y2 <= y1)
		return;

	full_height = FALSE;
	if (y1 == 0 && y2 == (crtc_box.y2 - crtc_box.y1))
		full_height = TRUE;

	/*
	 * Pre-965 doesn't have SVBLANK, so we need a bit
	 * of extra time for the blitter to start up and
	 * do its job for a full height blit
	 */
	if (full_height && INTEL_INFO(intel)->gen < 040)
		y2 -= 2;

	if (pipe == 0) {
		pipe = MI_LOAD_SCAN_LINES_DISPLAY_PIPEA;
		event = MI_WAIT_FOR_PIPEA_SCAN_LINE_WINDOW;
		if (full_height && INTEL_INFO(intel)->gen >= 040)
			event = MI_WAIT_FOR_PIPEA_SVBLANK;
	} else {
		pipe = MI_LOAD_SCAN_LINES_DISPLAY_PIPEB;
		event = MI_WAIT_FOR_PIPEB_SCAN_LINE_WINDOW;
		if (full_height && INTEL_INFO(intel)->gen >= 040)
			event = MI_WAIT_FOR_PIPEB_SVBLANK;
	}

	if (crtc->mode.Flags & V_INTERLACE) {
		/* DSL count field lines */
		y1 /= 2;
		y2 /= 2;
	}

	BEGIN_BATCH(5);
	/* The documentation says that the LOAD_SCAN_LINES command
	 * always comes in pairs. Don't ask me why. */
	OUT_BATCH(MI_LOAD_SCAN_LINES_INCL | pipe);
	OUT_BATCH((y1 << 16) | (y2-1));
	OUT_BATCH(MI_LOAD_SCAN_LINES_INCL | pipe);
	OUT_BATCH((y1 << 16) | (y2-1));
	OUT_BATCH(MI_WAIT_FOR_EVENT | event);
	ADVANCE_BATCH();
}

static Bool
intel_setup_video_buffer(ScrnInfoPtr scrn, intel_adaptor_private *adaptor_priv,
			 int alloc_size, int id, unsigned char *buf)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* Free the current buffer if we're going to have to reallocate */
	if (adaptor_priv->buf && adaptor_priv->buf->size < alloc_size)
		intel_free_video_buffers(adaptor_priv);

	if (adaptor_priv->buf == NULL) {
		adaptor_priv->buf = drm_intel_bo_alloc(intel->bufmgr, "xv buffer",
						       alloc_size, 4096);
		if (adaptor_priv->buf == NULL)
			return FALSE;

		adaptor_priv->reusable = TRUE;
	}

	return TRUE;
}

static void
intel_setup_dst_params(ScrnInfoPtr scrn, intel_adaptor_private *adaptor_priv, short width,
		       short height, int *dstPitch, int *dstPitch2, int *size,
		       int id)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int pitchAlign;

	/* Only needs to be DWORD-aligned for textured on i915, but overlay has
	 * stricter requirements.
	 */
	if (adaptor_priv->textured) {
		pitchAlign = 4;
	} else {
		if (INTEL_INFO(intel)->gen >= 040)
			/* Actually the alignment is 64 bytes, too. But the
			 * stride must be at least 512 bytes. Take the easy fix
			 * and align on 512 bytes unconditionally. */
			pitchAlign = 512;
		else if (IS_I830(intel) || IS_845G(intel))
			/* Harsh, errata on these chipsets limit the stride to be
			 * a multiple of 256 bytes.
			 */
			pitchAlign = 256;
		else
			pitchAlign = 64;
	}

#if INTEL_XVMC
	/* for i915 xvmc, hw requires 1kb aligned surfaces */
	if ((id == FOURCC_XVMC) && IS_GEN3(intel))
		pitchAlign = 1024;
#endif

	/* Determine the desired destination pitch (representing the chroma's pitch,
	 * in the planar case.
	 */
	if (is_planar_fourcc(id)) {
		if (adaptor_priv->rotation & (RR_Rotate_90 | RR_Rotate_270)) {
			*dstPitch = ALIGN((height / 2), pitchAlign);
			*dstPitch2 = ALIGN(height, pitchAlign);
			*size = *dstPitch * width * 3;
		} else {
			*dstPitch = ALIGN((width / 2), pitchAlign);
			*dstPitch2 = ALIGN(width, pitchAlign);
			*size = *dstPitch * height * 3;
		}
	} else {
		if (adaptor_priv->rotation & (RR_Rotate_90 | RR_Rotate_270)) {
			*dstPitch = ALIGN((height << 1), pitchAlign);
			*size = *dstPitch * width;
		} else {
			*dstPitch = ALIGN((width << 1), pitchAlign);
			*size = *dstPitch * height;
		}
		*dstPitch2 = 0;
	}
#if 0
	ErrorF("srcPitch: %d, dstPitch: %d, size: %d\n", srcPitch, *dstPitch,
	       size);
#endif

	adaptor_priv->YBufOffset = 0;

	if (adaptor_priv->rotation & (RR_Rotate_90 | RR_Rotate_270)) {
		adaptor_priv->UBufOffset =
		    adaptor_priv->YBufOffset + (*dstPitch2 * width);
		adaptor_priv->VBufOffset =
		    adaptor_priv->UBufOffset + (*dstPitch * width / 2);
	} else {
		adaptor_priv->UBufOffset =
		    adaptor_priv->YBufOffset + (*dstPitch2 * height);
		adaptor_priv->VBufOffset =
		    adaptor_priv->UBufOffset + (*dstPitch * height / 2);
	}
}

static Bool
intel_copy_video_data(ScrnInfoPtr scrn, intel_adaptor_private *adaptor_priv,
		     short width, short height, int *dstPitch, int *dstPitch2,
		     int top, int left, int npixels, int nlines,
		     int id, unsigned char *buf)
{
	int srcPitch = 0, srcPitch2 = 0;
	int size;

	if (is_planar_fourcc(id)) {
		srcPitch = ALIGN(width, 0x4);
		srcPitch2 = ALIGN((width >> 1), 0x4);
	} else {
		srcPitch = width << 1;
	}

	intel_setup_dst_params(scrn, adaptor_priv, width, height, dstPitch,
				dstPitch2, &size, id);

	if (!intel_setup_video_buffer(scrn, adaptor_priv, size, id, buf))
		return FALSE;

	/* copy data */
	if (is_planar_fourcc(id)) {
		return I830CopyPlanarData(adaptor_priv, buf, srcPitch, srcPitch2,
					  *dstPitch, *dstPitch2,
					  height, top, left, nlines,
					  npixels, id);
	} else {
		return I830CopyPackedData(adaptor_priv, buf, srcPitch, *dstPitch, top, left,
					  nlines, npixels);
	}
}

/*
 * The source rectangle of the video is defined by (src_x, src_y, src_w, src_h).
 * The dest rectangle of the video is defined by (drw_x, drw_y, drw_w, drw_h).
 * id is a fourcc code for the format of the video.
 * buf is the pointer to the source data in system memory.
 * width and height are the w/h of the source data.
 * If "sync" is TRUE, then we must be finished with *buf at the point of return
 * (which we always are).
 * clipBoxes is the clipping region in screen space.
 * data is a pointer to our port private.
 * drawable is some Drawable, which might not be the screen in the case of
 * compositing.  It's a new argument to the function in the 1.1 server.
 */
static int
I830PutImageTextured(ScrnInfoPtr scrn,
		     short src_x, short src_y,
		     short drw_x, short drw_y,
		     short src_w, short src_h,
		     short drw_w, short drw_h,
		     int id, unsigned char *buf,
		     short width, short height,
		     Bool sync, RegionPtr clipBoxes, pointer data,
		     DrawablePtr drawable)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	intel_adaptor_private *adaptor_priv = (intel_adaptor_private *) data;
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	int dstPitch, dstPitch2;
	BoxRec dstBox;
	xf86CrtcPtr crtc;
	int top, left, npixels, nlines;

	if (!intel_pixmap_is_offscreen(pixmap))
		return BadAlloc;

#if 0
	ErrorF("I830PutImage: src: (%d,%d)(%d,%d), dst: (%d,%d)(%d,%d)\n"
	       "width %d, height %d\n", src_x, src_y, src_w, src_h, drw_x,
	       drw_y, drw_w, drw_h, width, height);
#endif

	if (!intel_clip_video_helper(scrn,
				    adaptor_priv,
				    &crtc,
				    &dstBox,
				    src_x, src_y, drw_x, drw_y,
				    src_w, src_h, drw_w, drw_h,
				    id,
				    &top, &left, &npixels, &nlines, clipBoxes,
				    width, height))
		return Success;

	if (xvmc_passthrough(id)) {
		uint32_t *gem_handle = (uint32_t *)buf;
		int size;

		intel_setup_dst_params(scrn, adaptor_priv, width, height,
				&dstPitch, &dstPitch2, &size, id);

		if (IS_I915G(intel) || IS_I915GM(intel)) {
			/* XXX: i915 is not support and needs some
			 * serious care.  grep for KMS in i915_hwmc.c */
			return BadAlloc;
		}

		if (adaptor_priv->buf)
			drm_intel_bo_unreference(adaptor_priv->buf);

		adaptor_priv->buf =
			drm_intel_bo_gem_create_from_name(intel->bufmgr,
							  "xvmc surface",
							  *gem_handle);
		if (adaptor_priv->buf == NULL)
			return BadAlloc;

		adaptor_priv->reusable = FALSE;
	} else {
		if (!intel_copy_video_data(scrn, adaptor_priv, width, height,
					  &dstPitch, &dstPitch2,
					  top, left, npixels, nlines, id, buf))
			return BadAlloc;
	}

	if (crtc && adaptor_priv->SyncToVblank != 0 && INTEL_INFO(intel)->gen < 060) {
		intel_wait_for_scanline(scrn, pixmap, crtc, clipBoxes);
	}

	if (INTEL_INFO(intel)->gen >= 060) {
		Gen6DisplayVideoTextured(scrn, adaptor_priv, id, clipBoxes,
					 width, height, dstPitch, dstPitch2,
					 src_w, src_h,
					 drw_w, drw_h, pixmap);
	} else if (INTEL_INFO(intel)->gen >= 040) {
		I965DisplayVideoTextured(scrn, adaptor_priv, id, clipBoxes,
					 width, height, dstPitch, dstPitch2,
					 src_w, src_h,
					 drw_w, drw_h, pixmap);
	} else {
		I915DisplayVideoTextured(scrn, adaptor_priv, id, clipBoxes,
					 width, height, dstPitch, dstPitch2,
					 src_w, src_h, drw_w, drw_h,
					 pixmap);
	}

	intel_get_screen_private(scrn)->needs_flush = TRUE;
	DamageDamageRegion(drawable, clipBoxes);

	/* And make sure the WAIT_FOR_EVENT is queued before any
	 * modesetting/dpms operations on the pipe.
	 */
	intel_batch_submit(scrn);

	return Success;
}

static int
I830PutImageOverlay(ScrnInfoPtr scrn,
	     short src_x, short src_y,
	     short drw_x, short drw_y,
	     short src_w, short src_h,
	     short drw_w, short drw_h,
	     int id, unsigned char *buf,
	     short width, short height,
	     Bool sync, RegionPtr clipBoxes, pointer data,
	     DrawablePtr drawable)
{
	intel_adaptor_private *adaptor_priv = (intel_adaptor_private *) data;
	int dstPitch, dstPitch2;
	BoxRec dstBox;
	xf86CrtcPtr crtc;
	int top, left, npixels, nlines;

#if 0
	ErrorF("I830PutImage: src: (%d,%d)(%d,%d), dst: (%d,%d)(%d,%d)\n"
	       "width %d, height %d\n", src_x, src_y, src_w, src_h, drw_x,
	       drw_y, drw_w, drw_h, width, height);
#endif

	/* If dst width and height are less than 1/8th the src size, the
	 * src/dst scale factor becomes larger than 8 and doesn't fit in
	 * the scale register. */
	if (src_w >= (drw_w * 8))
		drw_w = src_w / 7;

	if (src_h >= (drw_h * 8))
		drw_h = src_h / 7;

	if (!intel_clip_video_helper(scrn,
				    adaptor_priv,
				    &crtc,
				    &dstBox,
				    src_x, src_y, drw_x, drw_y,
				    src_w, src_h, drw_w, drw_h,
				    id,
				    &top, &left, &npixels, &nlines, clipBoxes,
				    width, height))
		return Success;

	/* overlay can't handle rotation natively, store it for the copy func */
	if (crtc)
		adaptor_priv->rotation = crtc->rotation;
	else {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "Fail to clip video to any crtc!\n");
		return Success;
	}

	if (!intel_copy_video_data(scrn, adaptor_priv, width, height,
				  &dstPitch, &dstPitch2,
				  top, left, npixels, nlines, id, buf))
		return BadAlloc;

	if (!intel_display_overlay
	    (scrn, crtc, id, width, height, dstPitch, dstPitch2,
	     &dstBox, src_w, src_h, drw_w, drw_h))
		return BadAlloc;

	/* update cliplist */
	if (!REGION_EQUAL(scrn->pScreen, &adaptor_priv->clip, clipBoxes)) {
		REGION_COPY(scrn->pScreen, &adaptor_priv->clip, clipBoxes);
		xf86XVFillKeyHelperDrawable(drawable,
					    adaptor_priv->colorKey,
					    clipBoxes);
	}

	adaptor_priv->videoStatus = CLIENT_VIDEO_ON;

	return Success;
}

static int
I830QueryImageAttributes(ScrnInfoPtr scrn,
			 int id,
			 unsigned short *w, unsigned short *h,
			 int *pitches, int *offsets)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int size, tmp;

#if 0
	ErrorF("I830QueryImageAttributes: w is %d, h is %d\n", *w, *h);
#endif

	if (IS_845G(intel) || IS_I830(intel)) {
		if (*w > IMAGE_MAX_WIDTH_LEGACY)
			*w = IMAGE_MAX_WIDTH_LEGACY;
		if (*h > IMAGE_MAX_HEIGHT_LEGACY)
			*h = IMAGE_MAX_HEIGHT_LEGACY;
	} else {
		if (*w > IMAGE_MAX_WIDTH)
			*w = IMAGE_MAX_WIDTH;
		if (*h > IMAGE_MAX_HEIGHT)
			*h = IMAGE_MAX_HEIGHT;
	}

	*w = (*w + 1) & ~1;
	if (offsets)
		offsets[0] = 0;

	switch (id) {
		/* IA44 is for XvMC only */
	case FOURCC_IA44:
	case FOURCC_AI44:
		if (pitches)
			pitches[0] = *w;
		size = *w * *h;
		break;
	case FOURCC_YV12:
	case FOURCC_I420:
		*h = (*h + 1) & ~1;
		size = (*w + 3) & ~3;
		if (pitches)
			pitches[0] = size;
		size *= *h;
		if (offsets)
			offsets[1] = size;
		tmp = ((*w >> 1) + 3) & ~3;
		if (pitches)
			pitches[1] = pitches[2] = tmp;
		tmp *= (*h >> 1);
		size += tmp;
		if (offsets)
			offsets[2] = size;
		size += tmp;
#if 0
		if (pitches)
			ErrorF("pitch 0 is %d, pitch 1 is %d, pitch 2 is %d\n",
			       pitches[0], pitches[1], pitches[2]);
		if (offsets)
			ErrorF("offset 1 is %d, offset 2 is %d\n", offsets[1],
			       offsets[2]);
		if (offsets)
			ErrorF("size is %d\n", size);
#endif
		break;
#ifdef INTEL_XVMC
	case FOURCC_XVMC:
		*h = (*h + 1) & ~1;
		size = sizeof(struct intel_xvmc_command);
		if (pitches)
			pitches[0] = size;
		break;
#endif
	case FOURCC_UYVY:
	case FOURCC_YUY2:
	default:
		size = *w << 1;
		if (pitches)
			pitches[0] = size;
		size *= *h;
		break;
	}

	return size;
}

void
intel_video_block_handler(intel_screen_private *intel)
{
	intel_adaptor_private *adaptor_priv;

	/* no overlay */
	if (intel->adaptor == NULL)
		return;

	adaptor_priv = intel_get_adaptor_private(intel);
	if (adaptor_priv->videoStatus & OFF_TIMER) {
		Time now = currentTime.milliseconds;
		if (adaptor_priv->offTime < now) {
			/* Turn off the overlay */
			intel_overlay_off(intel);
			intel_free_video_buffers(adaptor_priv);
			adaptor_priv->videoStatus = 0;
		}
	}
}
