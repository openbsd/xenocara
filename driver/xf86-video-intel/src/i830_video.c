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

#include "xf86.h"
#include "xf86_OSproc.h"
#include "compiler.h"
#include "xf86PciInfo.h"
#include "xf86Pci.h"
#include "xf86fbman.h"
#include "regionstr.h"
#include "randrstr.h"
#include "windowstr.h"
#include "damage.h"
#include "i830.h"
#include "i830_video.h"
#include "xf86xv.h"
#include <X11/extensions/Xv.h>
#include "dixstruct.h"
#include "fourcc.h"

#ifdef INTEL_XVMC
#define _INTEL_XVMC_SERVER_
#include "i830_hwmc.h"
#include "i915_hwmc.h"
#endif

#define OFF_DELAY 	250	/* milliseconds */
#define FREE_DELAY 	15000

#define OFF_TIMER 	0x01
#define FREE_TIMER	0x02
#define CLIENT_VIDEO_ON	0x04

#define TIMER_MASK      (OFF_TIMER | FREE_TIMER)

static XF86VideoAdaptorPtr I830SetupImageVideoOverlay(ScreenPtr);
static XF86VideoAdaptorPtr I830SetupImageVideoTextured(ScreenPtr);
static void I830StopVideo(ScrnInfoPtr, pointer, Bool);
static int I830SetPortAttributeOverlay(ScrnInfoPtr, Atom, INT32, pointer);
static int I830SetPortAttributeTextured(ScrnInfoPtr, Atom, INT32, pointer);
static int I830GetPortAttribute(ScrnInfoPtr, Atom, INT32 *, pointer);
static void I830QueryBestSize(ScrnInfoPtr, Bool,
			      short, short, short, short, unsigned int *,
			      unsigned int *, pointer);
static int I830PutImage(ScrnInfoPtr, short, short, short, short, short, short,
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

/*
 * OCMD - Overlay Command Register
 */
#define OCMD_REGISTER		0x30168
#define MIRROR_MODE		(0x3<<17)
#define MIRROR_HORIZONTAL	(0x1<<17)
#define MIRROR_VERTICAL		(0x2<<17)
#define MIRROR_BOTH		(0x3<<17)
#define OV_BYTE_ORDER		(0x3<<14)
#define UV_SWAP			(0x1<<14)
#define Y_SWAP			(0x2<<14)
#define Y_AND_UV_SWAP		(0x3<<14)
#define SOURCE_FORMAT		(0xf<<10)
#define RGB_888			(0x1<<10)
#define	RGB_555			(0x2<<10)
#define	RGB_565			(0x3<<10)
#define	YUV_422			(0x8<<10)
#define	YUV_411			(0x9<<10)
#define	YUV_420			(0xc<<10)
#define	YUV_422_PLANAR		(0xd<<10)
#define	YUV_410			(0xe<<10)
#define TVSYNC_FLIP_PARITY	(0x1<<9)
#define TVSYNC_FLIP_ENABLE	(0x1<<7)
#define BUF_TYPE		(0x1<<5)
#define BUF_TYPE_FRAME		(0x0<<5)
#define BUF_TYPE_FIELD		(0x1<<5)
#define TEST_MODE		(0x1<<4)
#define BUFFER_SELECT		(0x3<<2)
#define BUFFER0			(0x0<<2)
#define BUFFER1			(0x1<<2)
#define FIELD_SELECT		(0x1<<1)
#define FIELD0			(0x0<<1)
#define FIELD1			(0x1<<1)
#define OVERLAY_ENABLE		0x1

#define OFC_UPDATE		0x1

/* OCONFIG register */
#define CC_OUT_8BIT		(0x1<<3)
#define OVERLAY_PIPE_MASK	(0x1<<18)		
#define OVERLAY_PIPE_A		(0x0<<18)		
#define OVERLAY_PIPE_B		(0x1<<18)		
#define GAMMA2_ENBL		(0x1<<16)
#define CSC_MODE_BT709		(0x1<<5)
#define CSC_MODE_BT601		(0x0<<5)
#define THREE_LINE_BUFFERS	(0x1<<0)
#define TWO_LINE_BUFFERS	(0x0<<0)

/* DCLRKM register */
#define DEST_KEY_ENABLE		(0x1<<31)

/* Polyphase filter coefficients */
#define N_HORIZ_Y_TAPS		5
#define N_VERT_Y_TAPS		3
#define N_HORIZ_UV_TAPS		3
#define N_VERT_UV_TAPS		3
#define N_PHASES		17
#define MAX_TAPS		5

/* Filter cutoff frequency limits. */
#define MIN_CUTOFF_FREQ		1.0
#define MAX_CUTOFF_FREQ		3.0

#define RGB16ToColorKey(c) \
(((c & 0xF800) << 8) | ((c & 0x07E0) << 5) | ((c & 0x001F) << 3))

#define RGB15ToColorKey(c) \
(((c & 0x7c00) << 9) | ((c & 0x03E0) << 6) | ((c & 0x001F) << 3))

/* client libraries expect an encoding */
static XF86VideoEncodingRec DummyEncoding[1] = {
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

#define CLONE_ATTRIBUTES 1
static XF86AttributeRec CloneAttributes[CLONE_ATTRIBUTES] = {
	{XvSettable | XvGettable, -1, 1, "XV_PIPE"}
};

#define NUM_ATTRIBUTES 4
static XF86AttributeRec Attributes[NUM_ATTRIBUTES] = {
	{XvSettable | XvGettable, 0, (1 << 24) - 1, "XV_COLORKEY"},
	{XvSettable | XvGettable, -128, 127, "XV_BRIGHTNESS"},
	{XvSettable | XvGettable, 0, 255, "XV_CONTRAST"},
	{XvSettable | XvGettable, 0, 1023, "XV_SATURATION"}
};

#define NUM_TEXTURED_ATTRIBUTES 3
static XF86AttributeRec TexturedAttributes[NUM_TEXTURED_ATTRIBUTES] = {
	{XvSettable | XvGettable, -128, 127, "XV_BRIGHTNESS"},
	{XvSettable | XvGettable, 0, 255, "XV_CONTRAST"},
	{XvSettable | XvGettable, -1, 1, "XV_SYNC_TO_VBLANK"},
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

#define NUM_IMAGES 5

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

typedef struct {
    uint32_t OBUF_0Y;
    uint32_t OBUF_1Y;
    uint32_t OBUF_0U;
    uint32_t OBUF_0V;
    uint32_t OBUF_1U;
    uint32_t OBUF_1V;
    uint32_t OSTRIDE;
    uint32_t YRGB_VPH;
    uint32_t UV_VPH;
    uint32_t HORZ_PH;
    uint32_t INIT_PHS;
    uint32_t DWINPOS;
    uint32_t DWINSZ;
    uint32_t SWIDTH;
    uint32_t SWIDTHSW;
    uint32_t SHEIGHT;
    uint32_t YRGBSCALE;
    uint32_t UVSCALE;
    uint32_t OCLRC0;
    uint32_t OCLRC1;
    uint32_t DCLRKV;
    uint32_t DCLRKM;
    uint32_t SCLRKVH;
    uint32_t SCLRKVL;
    uint32_t SCLRKEN;
    uint32_t OCONFIG;
    uint32_t OCMD;
    uint32_t RESERVED1;			/* 0x6C */
    uint32_t OSTART_0Y; 		/* for i965 */
    uint32_t OSTART_1Y;		/* for i965 */
    uint32_t OSTART_0U;
    uint32_t OSTART_0V;
    uint32_t OSTART_1U;
    uint32_t OSTART_1V;
    uint32_t OTILEOFF_0Y;
    uint32_t OTILEOFF_1Y;
    uint32_t OTILEOFF_0U;
    uint32_t OTILEOFF_0V;
    uint32_t OTILEOFF_1U;
    uint32_t OTILEOFF_1V;
    uint32_t FASTHSCALE;			/* 0xA0 */
    uint32_t UVSCALEV;			/* 0xA4 */

    uint32_t RESERVEDC[(0x200 - 0xA8) / 4];		   /* 0xA8 - 0x1FC */
    uint16_t Y_VCOEFS[N_VERT_Y_TAPS * N_PHASES];		   /* 0x200 */
    uint16_t RESERVEDD[0x100 / 2 - N_VERT_Y_TAPS * N_PHASES];
    uint16_t Y_HCOEFS[N_HORIZ_Y_TAPS * N_PHASES];		   /* 0x300 */
    uint16_t RESERVEDE[0x200 / 2 - N_HORIZ_Y_TAPS * N_PHASES];
    uint16_t UV_VCOEFS[N_VERT_UV_TAPS * N_PHASES];		   /* 0x500 */
    uint16_t RESERVEDF[0x100 / 2 - N_VERT_UV_TAPS * N_PHASES];
    uint16_t UV_HCOEFS[N_HORIZ_UV_TAPS * N_PHASES];	   /* 0x600 */
    uint16_t RESERVEDG[0x100 / 2 - N_HORIZ_UV_TAPS * N_PHASES];
} I830OverlayRegRec, *I830OverlayRegPtr;

#define I830OVERLAYREG(intel) ((I830OverlayRegPtr)\
			       ((intel)->FbBase + \
				(intel)->overlay_regs->offset))
#if VIDEO_DEBUG
static void CompareOverlay(intel_screen_private *intel, uint32_t * overlay, int size)
{
	int i;
	uint32_t val;
	int bad = 0;

	for (i = 0; i < size; i += 4) {
		val = INREG(0x30100 + i);
		if (val != overlay[i / 4]) {
			OVERLAY_DEBUG
			    ("0x%05x value doesn't match (0x%lx != 0x%lx)\n",
			     0x30100 + i, val, overlay[i / 4]);
			bad++;
		}
	}
	if (!bad)
		OVERLAY_DEBUG("CompareOverlay: no differences\n");
}
#endif

static void I830SetOneLineModeRatio(ScrnInfoPtr scrn);

static void
i830_overlay_switch_to_crtc (ScrnInfoPtr scrn, xf86CrtcPtr crtc)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	intel_adaptor_private *adaptor_priv = intel->adaptor->pPortPrivates[0].ptr;
	I830CrtcPrivatePtr  intel_crtc = crtc->driver_private;
	int			pipeconf_reg = intel_crtc->pipe == 0 ? PIPEACONF : PIPEBCONF;

    /* overlay can't be used on pipe with double wide, and pipe must be enabled. */
    if ((!IS_I965G(intel) && (INREG(pipeconf_reg) & PIPEACONF_DOUBLE_WIDE))
	    || (intel_crtc->dpms_mode == DPMSModeOff))
	adaptor_priv->overlayOK = FALSE;
    else
	adaptor_priv->overlayOK = TRUE;

    if (!adaptor_priv->overlayOK)
	return;

    /* Check we have an LFP connected */
    if (i830PipeHasType(crtc, I830_OUTPUT_LVDS)) 
    {

	int	vtotal_reg = intel_crtc->pipe == 0 ? VTOTAL_A : VTOTAL_B;
	uint32_t size = intel_crtc->pipe ? INREG(PIPEBSRC) : INREG(PIPEASRC);
	uint32_t active;
	uint32_t hsize, vsize;

	hsize = (size >> 16) & 0x7FF;
	vsize = size & 0x7FF;
	active = INREG(vtotal_reg) & 0x7FF;

	if (vsize < active && hsize > 1024)
	    I830SetOneLineModeRatio(scrn);

	if (adaptor_priv->scaleRatio & 0xFFFE0000) 
	{
	    /* Possible bogus ratio, using in-accurate fallback */
	    xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		       "Bogus panel fit register, Xvideo positioning may not "
		       "be accurate.\n");
	    xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		       "Using fallback ratio - was 0x%x, now 0x%x\n",
		       adaptor_priv->scaleRatio,
		       (int)(((float)active * 65536)/(float)vsize));

	    adaptor_priv->scaleRatio = (int)(((float)active * 65536) / (float)vsize);
	}
    }
}

/*
 * This is more or less the correct way to initalise, update, and shut down
 * the overlay.
 *
 * XXX Need to make sure that the overlay engine is cleanly shutdown in
 * all modes of server exit.
 */

static void
i830_overlay_on(ScrnInfoPtr scrn)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830OverlayRegPtr	overlay = I830OVERLAYREG(intel);
    intel_adaptor_private *adaptor_priv = intel->adaptor->pPortPrivates[0].ptr;
    Bool		deactivate = FALSE;
    
    if (intel->overlayOn)
	return;

    /*
     * On I830, if pipe A is off when the overlayis enabled, it will fail to
     * turn on and blank the entire screen or lock up the ring. Light up pipe
     * A in this case to provide a clock for the overlay hardware
     */
    if (adaptor_priv->current_crtc && i830_crtc_pipe (adaptor_priv->current_crtc) != 0)
	deactivate = i830_pipe_a_require_activate (scrn);

    overlay->OCMD &= ~OVERLAY_ENABLE;
    BEGIN_BATCH(6);
    OUT_BATCH(MI_FLUSH | MI_WRITE_DIRTY_STATE);
    OUT_BATCH(MI_NOOP);
    OUT_BATCH(MI_OVERLAY_FLIP | MI_OVERLAY_FLIP_ON);
    if (OVERLAY_NOPHYSICAL(intel))
	OUT_BATCH(intel->overlay_regs->offset | OFC_UPDATE);
    else
	OUT_BATCH(intel->overlay_regs->bus_addr | OFC_UPDATE);
    /* Wait for the overlay to light up before attempting to use it */
    OUT_BATCH(MI_WAIT_FOR_EVENT | MI_WAIT_FOR_OVERLAY_FLIP);
    OUT_BATCH(MI_NOOP);
    ADVANCE_BATCH();
    intel_sync(scrn);
    
    /*
     * If we turned pipe A on up above, turn it
     * back off
     */
    if (deactivate)
	i830_pipe_a_require_deactivate (scrn);

    OVERLAY_DEBUG("overlay_on\n");
    intel->overlayOn = TRUE;

    overlay->OCMD |= OVERLAY_ENABLE;
}

static void
i830_overlay_continue(ScrnInfoPtr scrn, Bool update_filter)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t		flip_addr;
    I830OverlayRegPtr	overlay = I830OVERLAYREG(intel);

    if (!intel->overlayOn)
	return;

    if (OVERLAY_NOPHYSICAL(intel))
	flip_addr = intel->overlay_regs->offset;
    else
	flip_addr = intel->overlay_regs->bus_addr;
    if (update_filter)
	flip_addr |= OFC_UPDATE;
    OVERLAY_DEBUG ("overlay_continue cmd 0x%08x  -> 0x%08x sta 0x%08x\n",
		   overlay->OCMD, INREG(OCMD_REGISTER), INREG(DOVSTA));
    BEGIN_BATCH(4);
    OUT_BATCH(MI_FLUSH | MI_WRITE_DIRTY_STATE);
    OUT_BATCH(MI_NOOP);
    OUT_BATCH(MI_OVERLAY_FLIP | MI_OVERLAY_FLIP_CONTINUE);
    OUT_BATCH(flip_addr);
    ADVANCE_BATCH();
    OVERLAY_DEBUG("overlay_continue\n");

    intel_sync(scrn);
}

static void
i830_overlay_off(ScrnInfoPtr scrn)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830OverlayRegPtr	overlay = I830OVERLAYREG(intel);

    if (!intel->overlayOn)
	return;

    /*
     * Wait for overlay to go idle. This has to be
     * separated from the turning off state by a Sync
     * to ensure the overlay will not read OCMD early and
     * disable the overlay before the commands here are
     * executed
     */
    {
	BEGIN_BATCH(2);
	OUT_BATCH(MI_WAIT_FOR_EVENT | MI_WAIT_FOR_OVERLAY_FLIP);
	OUT_BATCH(MI_NOOP);
	ADVANCE_BATCH();
	intel_sync(scrn);
    }
    
    /*
     * Turn overlay off
     */
    {
	overlay->OCMD &= ~OVERLAY_ENABLE;
	OVERLAY_DEBUG ("overlay_off cmd 0x%08x -> 0x%08x sta 0x%08x\n",
		       overlay->OCMD, INREG(OCMD_REGISTER), INREG(DOVSTA));
	BEGIN_BATCH(6);
	OUT_BATCH(MI_FLUSH | MI_WRITE_DIRTY_STATE);
	OUT_BATCH(MI_NOOP);
	OUT_BATCH(MI_OVERLAY_FLIP | MI_OVERLAY_FLIP_CONTINUE);
	if (OVERLAY_NOPHYSICAL(intel))
	    OUT_BATCH(intel->overlay_regs->offset);
	else
	    OUT_BATCH(intel->overlay_regs->bus_addr);
	OUT_BATCH(MI_WAIT_FOR_EVENT | MI_WAIT_FOR_OVERLAY_FLIP);
	OUT_BATCH(MI_NOOP);
	ADVANCE_BATCH();
	intel_sync(scrn);
    }
    intel->overlayOn = FALSE;
    OVERLAY_DEBUG("overlay_off\n");
}

void I830InitVideo(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	XF86VideoAdaptorPtr *adaptors, *newAdaptors = NULL;
	XF86VideoAdaptorPtr overlayAdaptor = NULL, texturedAdaptor = NULL;
	int num_adaptors;
#ifdef INTEL_XVMC
	Bool xvmc_status = FALSE;
#endif

	num_adaptors = xf86XVListGenericAdaptors(scrn, &adaptors);
	/* Give our adaptor list enough space for the overlay and/or texture video
	 * adaptors.
	 */
	newAdaptors =
	    xalloc((num_adaptors + 2) * sizeof(XF86VideoAdaptorPtr *));
	if (newAdaptors == NULL)
		return;

	memcpy(newAdaptors, adaptors,
	       num_adaptors * sizeof(XF86VideoAdaptorPtr));
	adaptors = newAdaptors;

	/* Add the adaptors supported by our hardware.  First, set up the atoms
	 * that will be used by both output adaptors.
	 */
	xvBrightness = MAKE_ATOM("XV_BRIGHTNESS");
	xvContrast = MAKE_ATOM("XV_CONTRAST");

	/* Set up textured video if we can do it at this depth and we are on
	 * supported hardware.
	 */
	if (scrn->bitsPerPixel >= 16 && (IS_I9XX(intel) || IS_I965G(intel)) &&
	    !(!IS_I965G(intel) && scrn->displayWidth > 2048)) {
		texturedAdaptor = I830SetupImageVideoTextured(screen);
		if (texturedAdaptor != NULL) {
			xf86DrvMsg(scrn->scrnIndex, X_INFO,
				   "Set up textured video\n");
		} else {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
				   "Failed to set up textured video\n");
		}
	}

	/* Set up overlay video if we can do it at this depth. */
	if (!OVERLAY_NOEXIST(intel) && scrn->bitsPerPixel != 8 &&
	    !intel->use_drm_mode && intel->overlay_regs != NULL) {
		overlayAdaptor = I830SetupImageVideoOverlay(screen);
		if (overlayAdaptor != NULL) {
			xf86DrvMsg(scrn->scrnIndex, X_INFO, "Set up overlay video\n");
		} else {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			    "Failed to set up overlay video\n");
		}
	}

	if (overlayAdaptor && intel->XvPreferOverlay)
	adaptors[num_adaptors++] = overlayAdaptor;

	if (texturedAdaptor)
		adaptors[num_adaptors++] = texturedAdaptor;

	if (overlayAdaptor && !intel->XvPreferOverlay)
		adaptors[num_adaptors++] = overlayAdaptor;

#ifdef INTEL_XVMC
	if (intel_xvmc_probe(scrn)) {
		if (texturedAdaptor)
			xvmc_status =
			    intel_xvmc_driver_init(screen, texturedAdaptor);
	}
#endif

	if (num_adaptors) {
		xf86XVScreenInit(screen, adaptors, num_adaptors);
	} else {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "Disabling Xv because no adaptors could be initialized.\n");
		intel->XvEnabled = FALSE;
	}

#ifdef INTEL_XVMC
	if (xvmc_status)
		intel_xvmc_screen_init(screen);
#endif
	xfree(adaptors);
}

static void
I830ResetVideo(ScrnInfoPtr scrn)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);
    intel_adaptor_private *adaptor_priv = intel->adaptor->pPortPrivates[0].ptr;
    I830OverlayRegPtr	overlay = I830OVERLAYREG(intel);

    OVERLAY_DEBUG("I830ResetVideo: base: %p, offset: 0x%lx, obase: %p\n",
		  intel->FbBase, intel->overlay_regs->offset, overlay);
    /*
     * Default to maximum image size in YV12
     */

    memset(overlay, 0, sizeof(*overlay));
    overlay->YRGB_VPH = 0;
    overlay->UV_VPH = 0;
    overlay->HORZ_PH = 0;
    overlay->INIT_PHS = 0;
    overlay->DWINPOS = 0;
    overlay->DWINSZ = 0;
    overlay->SWIDTH = 0;
    overlay->SWIDTHSW = 0;
    overlay->SHEIGHT = 0;
    overlay->OCLRC0 = (adaptor_priv->contrast << 18) | (adaptor_priv->brightness & 0xff);
    overlay->OCLRC1 = adaptor_priv->saturation;
#if 0
    overlay->AWINPOS = 0;
    overlay->AWINSZ = 0;
#endif
    overlay->FASTHSCALE = 0;

    /*
     * Enable destination color keying
     */
    switch (scrn->depth) {
    case 8:
	overlay->DCLRKV = 0;
	overlay->DCLRKM = 0xffffff | DEST_KEY_ENABLE;
	break;
    case 15:
	overlay->DCLRKV = RGB15ToColorKey(adaptor_priv->colorKey);
	overlay->DCLRKM = 0x070707 | DEST_KEY_ENABLE;
	break;
    case 16:
	overlay->DCLRKV = RGB16ToColorKey(adaptor_priv->colorKey);
	overlay->DCLRKM = 0x070307 | DEST_KEY_ENABLE;
	break;
    default:
	overlay->DCLRKV = adaptor_priv->colorKey;
	overlay->DCLRKM = DEST_KEY_ENABLE;
	break;
    }

    overlay->SCLRKVH = 0;
    overlay->SCLRKVL = 0;
    overlay->SCLRKEN = 0;		/* source color key disable */
    overlay->OCONFIG = CC_OUT_8BIT;
    if (IS_I965GM(intel))
	overlay->OCONFIG |= CSC_MODE_BT709;

    /*
     * Select which pipe the overlay is enabled on.
     */
    overlay->OCONFIG &= ~OVERLAY_PIPE_MASK;
    if (i830_crtc_pipe (adaptor_priv->current_crtc) == 0)
	overlay->OCONFIG |= OVERLAY_PIPE_A;
    else 
	overlay->OCONFIG |= OVERLAY_PIPE_B;

#if 0
    /* 
     * XXX DUMP REGISTER CODE !!!
     * This allows us to dump the complete i845 registers and compare
     * with warm boot situations before we upload our first copy.
     */
    {
	int i;
	for (i = 0x30000; i < 0x31000; i += 4)
	    ErrorF("0x%x 0x%" PRIx32 "\n", i, INREG(i));
    }
#endif
}

#define PFIT_CONTROLS 0x61230
#define PFIT_AUTOVSCALE_MASK 0x200
#define PFIT_ON_MASK 0x80000000
#define PFIT_AUTOSCALE_RATIO 0x61238
#define PFIT_PROGRAMMED_SCALE_RATIO 0x61234

static void
I830SetOneLineModeRatio(ScrnInfoPtr scrn)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);
    intel_adaptor_private *adaptor_priv = intel->adaptor->pPortPrivates[0].ptr;
    uint32_t panelFitControl = INREG(PFIT_CONTROLS);
    int vertScale;

    adaptor_priv->scaleRatio = 0x10000;

    if (panelFitControl & PFIT_ON_MASK) {
	if (panelFitControl & PFIT_AUTOVSCALE_MASK) {
	    vertScale = INREG(PFIT_AUTOSCALE_RATIO) >> 16;
	} else {
	    vertScale = INREG(PFIT_PROGRAMMED_SCALE_RATIO) >> 16;
	}

	if (vertScale != 0)
	    adaptor_priv->scaleRatio = ((double) 0x10000 / (double)vertScale) * 0x10000;

	adaptor_priv->oneLineMode = TRUE;

	xf86DrvMsg(scrn->scrnIndex, X_INFO, "Enabling Xvideo one-line mode\n");
    }

    if (adaptor_priv->scaleRatio == 0x10000)
	adaptor_priv->oneLineMode = FALSE;
}

static uint32_t I830BoundGammaElt (uint32_t elt, uint32_t eltPrev)
{
    elt &= 0xff;
    eltPrev &= 0xff;
    if (elt < eltPrev)
	elt = eltPrev;
    else if ((elt - eltPrev) > 0x7e)
	elt = eltPrev + 0x7e;
    return elt;
}

static uint32_t I830BoundGamma (uint32_t gamma, uint32_t gammaPrev)
{
    return (I830BoundGammaElt (gamma >> 16, gammaPrev >> 16) << 16 |
	    I830BoundGammaElt (gamma >>  8, gammaPrev >>  8) <<  8 |
	    I830BoundGammaElt (gamma      , gammaPrev      ));
}

static uint32_t I830Gamma5Errata(uint32_t gamma)
{
    int i;

    for (i = 0; i < 3; i++) {
	if ((gamma >> i*8 & 0xff) == 0x80) {
	    /* According to Intel docs, overlay fails if GAMMA5 is 0x80.
	     * In this case, change the value to 0x81 */
	    gamma += 1 << i*8;
	}
    }

    return gamma;
}

static void
I830UpdateGamma(ScrnInfoPtr scrn)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);
    intel_adaptor_private *adaptor_priv = intel->adaptor->pPortPrivates[0].ptr;
    uint32_t gamma0 = adaptor_priv->gamma0;
    uint32_t gamma1 = adaptor_priv->gamma1;
    uint32_t gamma2 = adaptor_priv->gamma2;
    uint32_t gamma3 = adaptor_priv->gamma3;
    uint32_t gamma4 = adaptor_priv->gamma4;
    uint32_t gamma5 = adaptor_priv->gamma5;

#if 0
    ErrorF ("Original gamma: 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx\n",
	    gamma0, gamma1, gamma2, gamma3, gamma4, gamma5);
#endif
    gamma1 = I830BoundGamma (gamma1, gamma0);
    gamma2 = I830BoundGamma (gamma2, gamma1);
    gamma3 = I830BoundGamma (gamma3, gamma2);
    gamma4 = I830BoundGamma (gamma4, gamma3);
    gamma5 = I830BoundGamma (gamma5, gamma4);
    gamma5 = I830Gamma5Errata(gamma5);
#if 0
    ErrorF ("Bounded  gamma: 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx\n",
	    gamma0, gamma1, gamma2, gamma3, gamma4, gamma5);
#endif

    OUTREG(OGAMC5, gamma5);
    OUTREG(OGAMC4, gamma4);
    OUTREG(OGAMC3, gamma3);
    OUTREG(OGAMC2, gamma2);
    OUTREG(OGAMC1, gamma1);
    OUTREG(OGAMC0, gamma0);
}

static XF86VideoAdaptorPtr I830SetupImageVideoOverlay(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	XF86VideoAdaptorPtr adapt;
	intel_adaptor_private *adaptor_priv;
	XF86AttributePtr att;

	OVERLAY_DEBUG("I830SetupImageVideoOverlay\n");

	if (!(adapt = xcalloc(1, sizeof(XF86VideoAdaptorRec) +
			      sizeof(intel_adaptor_private) + sizeof(DevUnion))))
		return NULL;

	adapt->type = XvWindowMask | XvInputMask | XvImageMask;
	adapt->flags = VIDEO_OVERLAID_IMAGES /*| VIDEO_CLIP_TO_VIEWPORT */ ;
	adapt->name = "Intel(R) Video Overlay";
	adapt->nEncodings = 1;
	adapt->pEncodings = DummyEncoding;
	/* update the DummyEncoding for these two chipsets */
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
	adapt->nAttributes += CLONE_ATTRIBUTES;
	if (IS_I9XX(intel))
		adapt->nAttributes += GAMMA_ATTRIBUTES;	/* has gamma */
	adapt->pAttributes =
	    xnfalloc(sizeof(XF86AttributeRec) * adapt->nAttributes);
	/* Now copy the attributes */
	att = adapt->pAttributes;
	memcpy((char *)att, (char *)Attributes,
	       sizeof(XF86AttributeRec) * NUM_ATTRIBUTES);
	att += NUM_ATTRIBUTES;
	memcpy((char *)att, (char *)CloneAttributes,
	       sizeof(XF86AttributeRec) * CLONE_ATTRIBUTES);
	att += CLONE_ATTRIBUTES;
	if (IS_I9XX(intel)) {
		memcpy((char *)att, (char *)GammaAttributes,
		       sizeof(XF86AttributeRec) * GAMMA_ATTRIBUTES);
		att += GAMMA_ATTRIBUTES;
	}
	adapt->nImages = NUM_IMAGES;
	adapt->pImages = Images;
	adapt->PutVideo = NULL;
	adapt->PutStill = NULL;
	adapt->GetVideo = NULL;
	adapt->GetStill = NULL;
	adapt->StopVideo = I830StopVideo;
	adapt->SetPortAttribute = I830SetPortAttributeOverlay;
	adapt->GetPortAttribute = I830GetPortAttribute;
	adapt->QueryBestSize = I830QueryBestSize;
	adapt->PutImage = I830PutImage;
	adapt->QueryImageAttributes = I830QueryImageAttributes;

	adaptor_priv->textured = FALSE;
	adaptor_priv->colorKey = intel->colorKey & ((1 << scrn->depth) - 1);
	adaptor_priv->videoStatus = 0;
	adaptor_priv->brightness = -19;	/* (255/219) * -16 */
	adaptor_priv->contrast = 75;	/* 255/219 * 64 */
	adaptor_priv->saturation = 146;	/* 128/112 * 128 */
	adaptor_priv->current_crtc = NULL;
	adaptor_priv->desired_crtc = NULL;
	adaptor_priv->buf = NULL;
	adaptor_priv->oldBuf = NULL;
	adaptor_priv->oldBuf_pinned = FALSE;
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

	/* With LFP's we need to detect whether we're in One Line Mode, which
	 * essentially means a resolution greater than 1024x768, and fix up
	 * the scaler accordingly. */
	adaptor_priv->scaleRatio = 0x10000;
	adaptor_priv->oneLineMode = FALSE;

	/*
	 * Initialise adaptor_priv->overlayOK.  Set it to TRUE here so that a
	 * warning will be generated if i830_crtc_dpms_video() sets it to
	 * FALSE during mode setup.
	 */
	adaptor_priv->overlayOK = TRUE;

	xvColorKey = MAKE_ATOM("XV_COLORKEY");
	xvBrightness = MAKE_ATOM("XV_BRIGHTNESS");
	xvContrast = MAKE_ATOM("XV_CONTRAST");
	xvSaturation = MAKE_ATOM("XV_SATURATION");

	/* Allow the pipe to be switched from pipe A to B when in clone mode */
	xvPipe = MAKE_ATOM("XV_PIPE");

	if (IS_I9XX(intel)) {
		xvGamma0 = MAKE_ATOM("XV_GAMMA0");
		xvGamma1 = MAKE_ATOM("XV_GAMMA1");
		xvGamma2 = MAKE_ATOM("XV_GAMMA2");
		xvGamma3 = MAKE_ATOM("XV_GAMMA3");
		xvGamma4 = MAKE_ATOM("XV_GAMMA4");
		xvGamma5 = MAKE_ATOM("XV_GAMMA5");
	}

	/* XXX These two acccess the overlay regs, don't call with drmmode */
	I830ResetVideo(scrn);
	I830UpdateGamma(scrn);

	return adapt;
}

static XF86VideoAdaptorPtr I830SetupImageVideoTextured(ScreenPtr screen)
{
	XF86VideoAdaptorPtr adapt;
	XF86AttributePtr attrs;
	intel_adaptor_private *adaptor_privs;
	DevUnion *devUnions;
	int nports = 16, i;
	int nAttributes;

	OVERLAY_DEBUG("I830SetupImageVideoOverlay\n");

	nAttributes = NUM_TEXTURED_ATTRIBUTES;

	adapt = xcalloc(1, sizeof(XF86VideoAdaptorRec));
	adaptor_privs = xcalloc(nports, sizeof(intel_adaptor_private));
	devUnions = xcalloc(nports, sizeof(DevUnion));
	attrs = xcalloc(nAttributes, sizeof(XF86AttributeRec));
	if (adapt == NULL || adaptor_privs == NULL || devUnions == NULL ||
	    attrs == NULL) {
		xfree(adapt);
		xfree(adaptor_privs);
		xfree(devUnions);
		xfree(attrs);
		return NULL;
	}

	adapt->type = XvWindowMask | XvInputMask | XvImageMask;
	adapt->flags = 0;
	adapt->name = "Intel(R) Textured Video";
	adapt->nEncodings = 1;
	adapt->pEncodings = DummyEncoding;
	adapt->nFormats = NUM_FORMATS;
	adapt->pFormats = Formats;
	adapt->nPorts = nports;
	adapt->pPortPrivates = devUnions;
	adapt->nAttributes = nAttributes;
	adapt->pAttributes = attrs;
	memcpy(attrs, TexturedAttributes,
	       nAttributes * sizeof(XF86AttributeRec));
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
	adapt->PutImage = I830PutImage;
	adapt->QueryImageAttributes = I830QueryImageAttributes;

	for (i = 0; i < nports; i++) {
		intel_adaptor_private *adaptor_priv = &adaptor_privs[i];

		adaptor_priv->textured = TRUE;
		adaptor_priv->videoStatus = 0;
		adaptor_priv->buf = NULL;
		adaptor_priv->oldBuf = NULL;
		adaptor_priv->oldBuf_pinned = FALSE;

		adaptor_priv->rotation = RR_Rotate_0;
		adaptor_priv->SyncToVblank = 1;

		/* gotta uninit this someplace, XXX: shouldn't be necessary for textured */
		REGION_NULL(screen, &adaptor_priv->clip);

		adapt->pPortPrivates[i].ptr = (pointer) (adaptor_priv);
	}

	xvSyncToVblank = MAKE_ATOM("XV_SYNC_TO_VBLANK");

	return adapt;
}

static void i830_free_video_buffers(intel_adaptor_private *adaptor_priv)
{
	if (adaptor_priv->buf) {
		drm_intel_bo_unreference(adaptor_priv->buf);
		adaptor_priv->buf = NULL;
	}

	if (adaptor_priv->oldBuf) {
		if (adaptor_priv->oldBuf_pinned)
			drm_intel_bo_unpin(adaptor_priv->oldBuf);
		drm_intel_bo_unreference(adaptor_priv->oldBuf);
		adaptor_priv->oldBuf = NULL;
		adaptor_priv->oldBuf_pinned = FALSE;
	}
}

static void
ums_overlay_off(ScrnInfoPtr scrn)
{
    i830_overlay_off (scrn);
}

static void I830StopVideo(ScrnInfoPtr scrn, pointer data, Bool shutdown)
{
	intel_adaptor_private *adaptor_priv = (intel_adaptor_private *) data;

	if (adaptor_priv->textured)
		return;

	OVERLAY_DEBUG("I830StopVideo\n");

	REGION_EMPTY(scrn->screen, &adaptor_priv->clip);

	if (shutdown) {
		if (adaptor_priv->videoStatus & CLIENT_VIDEO_ON)
			ums_overlay_off(scrn);

		i830_free_video_buffers(adaptor_priv);
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

static void
ums_overlay_update_attrs(ScrnInfoPtr scrn, intel_adaptor_private * adaptor_priv)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830OverlayRegPtr overlay;

    overlay = I830OVERLAYREG(intel);

    overlay->OCLRC0 = (adaptor_priv->contrast << 18) | (adaptor_priv->brightness & 0xff);
    overlay->OCLRC0 = (adaptor_priv->contrast << 18) | (adaptor_priv->brightness & 0xff);
    overlay->OCLRC1 = adaptor_priv->saturation;

    switch (scrn->depth) {
    case 16:
	overlay->DCLRKV = RGB16ToColorKey(adaptor_priv->colorKey);
	break;
    case 15:
	overlay->DCLRKV = RGB15ToColorKey(adaptor_priv->colorKey);
	break;
    default:
	overlay->DCLRKV = adaptor_priv->colorKey;
	break;
    }

    I830UpdateGamma(scrn);

    i830_overlay_continue(scrn, FALSE);
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
		xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
		if ((value < -1) || (value > xf86_config->num_crtc))
			return BadValue;
		if (value < 0)
			adaptor_priv->desired_crtc = NULL;
		else
			adaptor_priv->desired_crtc = xf86_config->crtc[value];
		/*
		 * Leave this to be updated at the next frame
		 */
	} else if (attribute == xvGamma0 && (IS_I9XX(intel))) {
		adaptor_priv->gamma0 = value;
	} else if (attribute == xvGamma1 && (IS_I9XX(intel))) {
		adaptor_priv->gamma1 = value;
	} else if (attribute == xvGamma2 && (IS_I9XX(intel))) {
		adaptor_priv->gamma2 = value;
	} else if (attribute == xvGamma3 && (IS_I9XX(intel))) {
		adaptor_priv->gamma3 = value;
	} else if (attribute == xvGamma4 && (IS_I9XX(intel))) {
		adaptor_priv->gamma4 = value;
	} else if (attribute == xvGamma5 && (IS_I9XX(intel))) {
		adaptor_priv->gamma5 = value;
	} else if (attribute == xvColorKey) {
		adaptor_priv->colorKey = value;
		OVERLAY_DEBUG("COLORKEY\n");
	} else
		return BadMatch;

	/* Ensure that the overlay is off, ready for updating */
	if ((attribute == xvGamma0 ||
	    attribute == xvGamma1 ||
	    attribute == xvGamma2 ||
	    attribute == xvGamma3 ||
	    attribute == xvGamma4 ||
	    attribute == xvGamma5) && (IS_I9XX(intel))) {
		OVERLAY_DEBUG("GAMMA\n");
	}

	ums_overlay_update_attrs(scrn, adaptor_priv);

	if (attribute == xvColorKey)
		REGION_EMPTY(scrn->screen, &adaptor_priv->clip);

	if (attribute == xvColorKey)
		REGION_EMPTY(scrn->screen, &adaptor_priv->clip);

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
	} else if (attribute == xvGamma0 && (IS_I9XX(intel))) {
		*value = adaptor_priv->gamma0;
	} else if (attribute == xvGamma1 && (IS_I9XX(intel))) {
		*value = adaptor_priv->gamma1;
	} else if (attribute == xvGamma2 && (IS_I9XX(intel))) {
		*value = adaptor_priv->gamma2;
	} else if (attribute == xvGamma3 && (IS_I9XX(intel))) {
		*value = adaptor_priv->gamma3;
	} else if (attribute == xvGamma4 && (IS_I9XX(intel))) {
		*value = adaptor_priv->gamma4;
	} else if (attribute == xvGamma5 && (IS_I9XX(intel))) {
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

static void
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

	drm_intel_bo_map(adaptor_priv->buf, TRUE);
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

	drm_intel_bo_unmap(adaptor_priv->buf);
}

static void i830_memcpy_plane(unsigned char *dst, unsigned char *src,
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

static void
I830CopyPlanarData(intel_adaptor_private *adaptor_priv,
		   unsigned char *buf, int srcPitch,
		   int srcPitch2, int dstPitch, int srcH, int top, int left,
		   int h, int w, int id)
{
	unsigned char *src1, *src2, *src3, *dst_base, *dst1, *dst2, *dst3;
	int dstPitch2 = dstPitch << 1;

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

	drm_intel_bo_map(adaptor_priv->buf, TRUE);
	dst_base = adaptor_priv->buf->virtual;

	dst1 = dst_base + adaptor_priv->YBufOffset;

	i830_memcpy_plane(dst1, src1, h, w, dstPitch2, srcPitch,
			  adaptor_priv->rotation);

	/* Copy V data for YV12, or U data for I420 */
	src2 = buf +		/* start of YUV data */
	    (srcH * srcPitch) +	/* move over Luma plane */
	    ((top * srcPitch) >> 2) +	/* move down from by top lines */
	    (left >> 1);	/* move left by left pixels */

#if 0
	ErrorF("src2 is %p, offset is %ld\n", src2,
	       (unsigned long)src2 - (unsigned long)buf);
#endif
	if (id == FOURCC_I420)
		dst2 = dst_base + adaptor_priv->UBufOffset;
	else
		dst2 = dst_base + adaptor_priv->VBufOffset;

	i830_memcpy_plane(dst2, src2, h / 2, w / 2,
			  dstPitch, srcPitch2, adaptor_priv->rotation);

	/* Copy U data for YV12, or V data for I420 */
	src3 = buf +		/* start of YUV data */
	    (srcH * srcPitch) +	/* move over Luma plane */
	    ((srcH >> 1) * srcPitch2) +	/* move over Chroma plane */
	    ((top * srcPitch) >> 2) +	/* move down from by top lines */
	    (left >> 1);	/* move left by left pixels */
#if 0
	ErrorF("src3 is %p, offset is %ld\n", src3,
	       (unsigned long)src3 - (unsigned long)buf);
#endif
	if (id == FOURCC_I420)
		dst3 = dst_base + adaptor_priv->VBufOffset;
	else
		dst3 = dst_base + adaptor_priv->UBufOffset;

	i830_memcpy_plane(dst3, src3, h / 2, w / 2,
			  dstPitch, srcPitch2, adaptor_priv->rotation);

	drm_intel_bo_unmap(adaptor_priv->buf);
}

typedef struct {
	uint8_t sign;
	uint16_t mantissa;
	uint8_t exponent;
} coeffRec, *coeffPtr;

static Bool
SetCoeffRegs(double *coeff, int mantSize, coeffPtr pCoeff, int pos)
{
    int maxVal, icoeff, res;
    int sign;
    double c;

    sign = 0;
    maxVal = 1 << mantSize;
    c = *coeff;
    if (c < 0.0) {
	sign = 1;
	c = -c;
    }

    res = 12 - mantSize;
    if ((icoeff = (int)(c * 4 * maxVal + 0.5)) < maxVal) {
	pCoeff[pos].exponent = 3;
	pCoeff[pos].mantissa = icoeff << res;
	*coeff = (double)icoeff / (double)(4 * maxVal);
    } else if ((icoeff = (int)(c * 2 * maxVal + 0.5)) < maxVal) {
	pCoeff[pos].exponent = 2;
	pCoeff[pos].mantissa = icoeff << res;
	*coeff = (double)icoeff / (double)(2 * maxVal);
    } else if ((icoeff = (int)(c * maxVal + 0.5)) < maxVal) {
	pCoeff[pos].exponent = 1;
	pCoeff[pos].mantissa = icoeff << res;
	*coeff = (double)icoeff / (double)(maxVal);
    } else if ((icoeff = (int)(c * maxVal * 0.5 + 0.5)) < maxVal) {
	pCoeff[pos].exponent = 0;
	pCoeff[pos].mantissa = icoeff << res;
	*coeff = (double)icoeff / (double)(maxVal / 2);
    } else {
	/* Coeff out of range */
	return FALSE;
    }

    pCoeff[pos].sign = sign;
    if (sign)
	*coeff = -(*coeff);
    return TRUE;
}

static void
UpdateCoeff(int taps, double fCutoff, Bool isHoriz, Bool isY, coeffPtr pCoeff)
{
    int i, j, j1, num, pos, mantSize;
    double pi = 3.1415926535, val, sinc, window, sum;
    double rawCoeff[MAX_TAPS * 32], coeffs[N_PHASES][MAX_TAPS];
    double diff;
    int tapAdjust[MAX_TAPS], tap2Fix;
    Bool isVertAndUV;

    if (isHoriz)
	mantSize = 7;
    else
	mantSize = 6;

    isVertAndUV = !isHoriz && !isY;
    num = taps * 16;
    for (i = 0; i < num  * 2; i++) {
	val = (1.0 / fCutoff) * taps * pi * (i - num) / (2 * num);
	if (val == 0.0)
	    sinc = 1.0;
	else
	    sinc = sin(val) / val;

	/* Hamming window */
	window = (0.5 - 0.5 * cos(i * pi / num));
	rawCoeff[i] = sinc * window;
    }

    for (i = 0; i < N_PHASES; i++) {
	/* Normalise the coefficients. */
	sum = 0.0;
	for (j = 0; j < taps; j++) {
	    pos = i + j * 32;
	    sum += rawCoeff[pos];
	}
	for (j = 0; j < taps; j++) {
	    pos = i + j * 32;
	    coeffs[i][j] = rawCoeff[pos] / sum;
	}

	/* Set the register values. */
	for (j = 0; j < taps; j++) {
	    pos = j + i * taps;
	    if ((j == (taps - 1) / 2) && !isVertAndUV)
		SetCoeffRegs(&coeffs[i][j], mantSize + 2, pCoeff, pos);
	    else
		SetCoeffRegs(&coeffs[i][j], mantSize, pCoeff, pos);
	}

	tapAdjust[0] = (taps - 1) / 2;
	for (j = 1, j1 = 1; j <= tapAdjust[0]; j++, j1++) {
	    tapAdjust[j1] = tapAdjust[0] - j;
	    tapAdjust[++j1] = tapAdjust[0] + j;
	}

	/* Adjust the coefficients. */
	sum = 0.0;
	for (j = 0; j < taps; j++)
	    sum += coeffs[i][j];
	if (sum != 1.0) {
	    for (j1 = 0; j1 < taps; j1++) {
		tap2Fix = tapAdjust[j1];
		diff = 1.0 - sum;
		coeffs[i][tap2Fix] += diff;
		pos = tap2Fix + i * taps;
		if ((tap2Fix == (taps - 1) / 2) && !isVertAndUV)
		    SetCoeffRegs(&coeffs[i][tap2Fix], mantSize + 2, pCoeff, pos);
		else
		    SetCoeffRegs(&coeffs[i][tap2Fix], mantSize, pCoeff, pos);

		sum = 0.0;
		for (j = 0; j < taps; j++)
		    sum += coeffs[i][j];
		if (sum == 1.0)
		    break;
	    }
	}
    }
}

static void i830_box_intersect(BoxPtr dest, BoxPtr a, BoxPtr b)
{
	dest->x1 = a->x1 > b->x1 ? a->x1 : b->x1;
	dest->x2 = a->x2 < b->x2 ? a->x2 : b->x2;
	dest->y1 = a->y1 > b->y1 ? a->y1 : b->y1;
	dest->y2 = a->y2 < b->y2 ? a->y2 : b->y2;
	if (dest->x1 >= dest->x2 || dest->y1 >= dest->y2)
		dest->x1 = dest->x2 = dest->y1 = dest->y2 = 0;
}

static void i830_crtc_box(xf86CrtcPtr crtc, BoxPtr crtc_box)
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

static int i830_box_area(BoxPtr box)
{
	return (int)(box->x2 - box->x1) * (int)(box->y2 - box->y1);
}

/*
 * Return the crtc covering 'box'. If two crtcs cover a portion of
 * 'box', then prefer 'desired'. If 'desired' is NULL, then prefer the crtc
 * with greater coverage
 */

xf86CrtcPtr
i830_covering_crtc(ScrnInfoPtr scrn,
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
		if (!i830_crtc_on(crtc))
			continue;

		i830_crtc_box(crtc, &crtc_box);
		i830_box_intersect(&cover_box, &crtc_box, box);
		coverage = i830_box_area(&cover_box);
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

static int
i830_swidth (intel_screen_private *intel, unsigned int offset,
	     unsigned int width, unsigned int mask, int shift)
{
    int	swidth = ((offset + width + mask) >> shift) - (offset >> shift);
    if (IS_I9XX(intel))
	swidth <<= 1;
    swidth -= 1;
    return swidth << 2;
}

static void
i830_calc_src_regs(intel_screen_private *intel, int planar, short width,
    short height, uint32_t *swidth_out, uint32_t *swidthsw_out,
    uint32_t *sheigth_out)
{
    unsigned int	mask, shift, offsety, offsetu;
    unsigned int	swidth, swidthy, swidthuv;
    intel_adaptor_private *adaptor_priv = intel->adaptor->pPortPrivates[0].ptr;

    if (IS_I9XX(intel)) {
	shift = 6;
	mask = 0x3f;
    } else {
	shift = 5;
	mask = 0x1f;
    }

    offsety = adaptor_priv->YBufOffset;
    offsetu = adaptor_priv->UBufOffset;

    if (planar) {
	*swidth_out = width | ((width/2 & 0x7ff) << 16);
	swidthy  = i830_swidth (intel, offsety, width, mask, shift);
	swidthuv = i830_swidth (intel, offsetu, width/2, mask, shift);
	*swidthsw_out = (swidthy) | (swidthuv << 16);
	*sheigth_out = height | ((height / 2) << 16);
    } else {
	*swidth_out = width;
	swidth = i830_swidth (intel, offsety, width << 1, mask, shift);
	*swidthsw_out = swidth;
	*sheigth_out = height;
    }

    return;
}

static void
i830_update_dst_box_to_crtc_coords(ScrnInfoPtr scrn, xf86CrtcPtr crtc,
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

static void
i830_store_coeffs_in_overlay_regs(uint16_t *reg_coeffs, coeffPtr new_coeffs,
	int max_taps)
{
    int i, j, pos;

    for (i = 0; i < N_PHASES; i++) {
	for (j = 0; j < max_taps; j++) {
	    pos = i * max_taps + j;
	    reg_coeffs[pos] = (new_coeffs[pos].sign << 15 |
				      new_coeffs[pos].exponent << 12 |
				      new_coeffs[pos].mantissa);
	}
    }
}

static uint32_t
i830_overlay_cmd(int id, int planar)
{
    uint32_t OCMD = OVERLAY_ENABLE;

    if (planar) {
	OVERLAY_DEBUG("YUV420\n");
	OCMD &= ~SOURCE_FORMAT;
	OCMD &= ~OV_BYTE_ORDER;
	OCMD |= YUV_420;
    } else {
	OVERLAY_DEBUG("YUV422\n");
	OCMD &= ~SOURCE_FORMAT;
	OCMD |= YUV_422;
	OCMD &= ~OV_BYTE_ORDER;
	if (id == FOURCC_UYVY)
	    OCMD |= Y_SWAP;
    }

    OCMD &= ~(BUFFER_SELECT | FIELD_SELECT);
    OCMD |= BUFFER0;

    OVERLAY_DEBUG("OCMD is 0x%x\n", OCMD);

    return OCMD;
}

static double
i830_limit_coeff(double coeff)
{
    /* Limit to between 1.0 and 3.0. */
    if (coeff < MIN_CUTOFF_FREQ)
	coeff = MIN_CUTOFF_FREQ;
    if (coeff > MAX_CUTOFF_FREQ)
	coeff = MAX_CUTOFF_FREQ;

    return coeff;
}

static void
i830_update_polyphase_coeffs(I830OverlayRegPtr	overlay,
	int xscaleFract, int xscaleFractUV)
{
    /*
     * Only Horizontal coefficients so far.
     */
    double fCutoffY;
    double fCutoffUV;
    coeffRec xcoeffY[N_HORIZ_Y_TAPS * N_PHASES];
    coeffRec xcoeffUV[N_HORIZ_UV_TAPS * N_PHASES];

    fCutoffY = xscaleFract / 4096.0;
    fCutoffUV = xscaleFractUV / 4096.0;

    fCutoffUV = i830_limit_coeff(fCutoffUV);
    fCutoffY = i830_limit_coeff(fCutoffY);

    UpdateCoeff(N_HORIZ_Y_TAPS, fCutoffY, TRUE, TRUE, xcoeffY);
    UpdateCoeff(N_HORIZ_UV_TAPS, fCutoffUV, TRUE, FALSE, xcoeffUV);

    i830_store_coeffs_in_overlay_regs(overlay->Y_HCOEFS, xcoeffY,
		    N_HORIZ_Y_TAPS);
    i830_store_coeffs_in_overlay_regs(overlay->UV_HCOEFS, xcoeffUV,
		    N_HORIZ_UV_TAPS);
}

/*
 * Calculate horizontal and vertical scaling factors and polyphase
 * coefficients.
 */

static Bool
i830_update_scaling_factors(I830OverlayRegPtr overlay,
	short src_w, short src_h, short drw_w, short drw_h)
{
    int xscaleInt, xscaleFract, yscaleInt, yscaleFract;
    int xscaleIntUV, xscaleFractUV;
    int yscaleIntUV, yscaleFractUV;
    uint32_t newval;
    Bool scaleChanged = FALSE;

    /*
     * Y down-scale factor as a multiple of 4096.
     */
    xscaleFract = ((src_w - 1) << 12) / drw_w;
    yscaleFract = ((src_h - 1) << 12) / drw_h;

    /* Calculate the UV scaling factor.
     * UV is half the size of Y -- YUV420 */
    xscaleFractUV = xscaleFract / 2;
    yscaleFractUV = yscaleFract / 2;

    /*
     * To keep the relative Y and UV ratios exact, round the Y scales
     * to a multiple of the Y/UV ratio.
     */
    xscaleFract = xscaleFractUV * 2;
    yscaleFract = yscaleFractUV * 2;

    /* Integer (un-multiplied) values. */
    xscaleInt = xscaleFract >> 12;
    yscaleInt = yscaleFract >> 12;

    xscaleIntUV = xscaleFractUV >> 12;
    yscaleIntUV = yscaleFractUV >> 12;

    OVERLAY_DEBUG("xscale: %x.%03x, yscale: %x.%03x\n", xscaleInt,
		  xscaleFract & 0xFFF, yscaleInt, yscaleFract & 0xFFF);
    OVERLAY_DEBUG("UV xscale: %x.%03x, UV yscale: %x.%03x\n", xscaleIntUV,
		  xscaleFractUV & 0xFFF, yscaleIntUV, yscaleFractUV & 0xFFF);

    /* shouldn't get here */
    if (xscaleInt > 7) {
	OVERLAY_DEBUG("xscale: bad scale\n");
	return FALSE;
    }

    /* shouldn't get here */
    if (xscaleIntUV > 7) {
	OVERLAY_DEBUG("xscaleUV: bad scale\n");
	return FALSE;
    }

    newval = (xscaleInt << 16) |
    ((xscaleFract & 0xFFF) << 3) | ((yscaleFract & 0xFFF) << 20);
    if (newval != overlay->YRGBSCALE) {
	scaleChanged = TRUE;
	overlay->YRGBSCALE = newval;
    }

    newval = (xscaleIntUV << 16) | ((xscaleFractUV & 0xFFF) << 3) |
    ((yscaleFractUV & 0xFFF) << 20);
    if (newval != overlay->UVSCALE) {
	scaleChanged = TRUE;
	overlay->UVSCALE = newval;
    }

    newval = yscaleInt << 16 | yscaleIntUV;
    if (newval != overlay->UVSCALEV) {
	scaleChanged = TRUE;
	overlay->UVSCALEV = newval;
    }

    if (scaleChanged) {
	i830_update_polyphase_coeffs(overlay, xscaleFract, xscaleFractUV);
    }

    return scaleChanged;
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
ums_overlay_put_image(ScrnInfoPtr scrn, xf86CrtcPtr crtc,
		   int id, short width, short height,
		   int dstPitch, int x1, int y1, int x2, int y2, BoxPtr dstBox,
		   short src_w, short src_h, short drw_w, short drw_h)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);
    intel_adaptor_private *adaptor_priv = intel->adaptor->pPortPrivates[0].ptr;
    I830OverlayRegPtr	overlay = I830OVERLAYREG(intel);
    int			planar;
    uint32_t		swidth, swidthsw, sheigth;
    Bool		scaleChanged;
    drm_intel_bo	*tmp_buf;

    if (crtc != adaptor_priv->current_crtc)
    {
	/* this may adjust adaptor_priv->oneLineMode */
	i830_overlay_switch_to_crtc (scrn, crtc);
	if (adaptor_priv->overlayOK) {
	    adaptor_priv->current_crtc = crtc;
	    I830ResetVideo (scrn);
	}
    }

    if (!adaptor_priv->overlayOK)
	return TRUE;

    if (adaptor_priv->oneLineMode) {
	/* change the coordinates with panel fitting active */
	dstBox->y1 = (((dstBox->y1 - 1) * adaptor_priv->scaleRatio) >> 16) + 1;
	dstBox->y2 = ((dstBox->y2 * adaptor_priv->scaleRatio) >> 16) + 1;

	/* Now, alter the height, so we scale to the correct size */
	drw_h = ((drw_h * adaptor_priv->scaleRatio) >> 16) + 1;
    }

    planar = is_planar_fourcc(id);

    i830_calc_src_regs(intel, planar, width, height,
	    &swidth, &swidthsw, &sheigth);

    overlay->SWIDTH = swidth;
    overlay->SWIDTHSW = swidthsw;
    overlay->SHEIGHT = sheigth;

    overlay->DWINPOS = (dstBox->y1 << 16) | dstBox->x1;

    overlay->DWINSZ = (((dstBox->y2 - dstBox->y1) << 16) |
		       (dstBox->x2 - dstBox->x1));

    OVERLAY_DEBUG("dstBox: x1: %d, y1: %d, x2: %d, y2: %d\n",
		  dstBox->x1, dstBox->y1, dstBox->x2, dstBox->y2);

    /* buffer locations */
    if (drm_intel_bo_pin(adaptor_priv->buf, GTT_PAGE_SIZE) != 0)
        return FALSE;
    overlay->OBUF_0Y = adaptor_priv->YBufOffset + adaptor_priv->buf->offset;
    overlay->OBUF_0U = adaptor_priv->UBufOffset + adaptor_priv->buf->offset;
    overlay->OBUF_0V = adaptor_priv->VBufOffset + adaptor_priv->buf->offset;

    OVERLAY_DEBUG("pos: 0x%x, size: 0x%x\n",
		  overlay->DWINPOS, overlay->DWINSZ);
    OVERLAY_DEBUG("dst: %d x %d, src: %d x %d\n", drw_w, drw_h, src_w, src_h);

    scaleChanged = i830_update_scaling_factors(overlay,
	    src_w, src_h, drw_w, drw_h);

    if (planar) {
#if 0
	/* set UV vertical phase to -0.25 */
	overlay->UV_VPH = 0x30003000;
#endif
	overlay->OSTRIDE = (dstPitch * 2) | (dstPitch << 16);
	OVERLAY_DEBUG("UV stride is %d, Y stride is %d\n",
		      dstPitch, dstPitch * 2);
    } else
	overlay->OSTRIDE = dstPitch;

    overlay->OCMD = i830_overlay_cmd(id, planar);

    /* make sure the overlay is on */
    i830_overlay_on (scrn);
    /* and show this frame */
    i830_overlay_continue (scrn, scaleChanged);

    /* unpin the old buffer and exchange buffers */
    if (adaptor_priv->oldBuf_pinned) {
	drm_intel_bo_unpin(adaptor_priv->oldBuf);
    }
    tmp_buf = adaptor_priv->buf;
    adaptor_priv->buf = adaptor_priv->oldBuf;
    adaptor_priv->oldBuf = tmp_buf;
    adaptor_priv->oldBuf_pinned = TRUE;

    return TRUE;
}

static Bool
i830_display_overlay(ScrnInfoPtr scrn, xf86CrtcPtr crtc,
		     int id, short width, short height,
		     int dstPitch, int x1, int y1, int x2, int y2,
		     BoxPtr dstBox, short src_w, short src_h, short drw_w,
		     short drw_h)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	intel_adaptor_private *adaptor_priv = intel->adaptor->pPortPrivates[0].ptr;
	int tmp;

	OVERLAY_DEBUG("I830DisplayVideo: %dx%d (pitch %d)\n", width, height,
		      dstPitch);

#if VIDEO_DEBUG
	CompareOverlay(intel, (uint32_t *) overlay, 0x100);
#endif

	/*
	 * If the video isn't visible on any CRTC, turn it off
	 */
	if (!crtc) {
		adaptor_priv->current_crtc = NULL;
		ums_overlay_off(scrn);

		return TRUE;
	}

	i830_update_dst_box_to_crtc_coords(scrn, crtc, dstBox);

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

	return ums_overlay_put_image(scrn, crtc, id, width, height, dstPitch,
	    x1, y1, x2, y2, dstBox, src_w, src_h, drw_w, drw_h);
}

static Bool
i830_clip_video_helper(ScrnInfoPtr scrn,
		       intel_adaptor_private *adaptor_priv,
		       xf86CrtcPtr * crtc_ret,
		       BoxPtr dst,
		       INT32 * xa,
		       INT32 * xb,
		       INT32 * ya,
		       INT32 * yb, RegionPtr reg, INT32 width, INT32 height)
{
	Bool ret;
	RegionRec crtc_region_local;
	RegionPtr crtc_region = reg;

	/*
	 * For overlay video, compute the relevant CRTC and
	 * clip video to that
	 */
	if (crtc_ret) {
		BoxRec crtc_box;
		xf86CrtcPtr crtc = i830_covering_crtc(scrn, dst,
						      adaptor_priv->desired_crtc,
						      &crtc_box);

		/* For textured video, we don't actually want to clip at all. */
		if (crtc && !adaptor_priv->textured) {
			REGION_INIT(screen, &crtc_region_local, &crtc_box, 1);
			crtc_region = &crtc_region_local;
			REGION_INTERSECT(screen, crtc_region, crtc_region,
					 reg);
		}
		*crtc_ret = crtc;
	}
	ret = xf86XVClipVideoHelper(dst, xa, xb, ya, yb,
				    crtc_region, width, height);
	if (crtc_region != reg)
		REGION_UNINIT(screen, &crtc_region_local);
	return ret;
}

static void
i830_fill_colorkey(ScreenPtr screen, uint32_t key, RegionPtr clipboxes)
{
	DrawablePtr root = &WindowTable[screen->myNum]->drawable;
	XID pval[2];
	BoxPtr pbox = REGION_RECTS(clipboxes);
	int i, nbox = REGION_NUM_RECTS(clipboxes);
	xRectangle *rects;
	GCPtr gc;

	if (!xf86Screens[screen->myNum]->vtSema)
		return;

	gc = GetScratchGC(root->depth, screen);
	pval[0] = key;
	pval[1] = IncludeInferiors;
	(void)ChangeGC(gc, GCForeground | GCSubwindowMode, pval);
	ValidateGC(root, gc);

	rects = xalloc(nbox * sizeof(xRectangle));

	for (i = 0; i < nbox; i++, pbox++) {
		rects[i].x = pbox->x1;
		rects[i].y = pbox->y1;
		rects[i].width = pbox->x2 - pbox->x1;
		rects[i].height = pbox->y2 - pbox->y1;
	}

	(*gc->ops->PolyFillRect) (root, gc, nbox, rects);

	xfree(rects);
	FreeScratchGC(gc);
}

static void
i830_wait_for_scanline(ScrnInfoPtr scrn, PixmapPtr pixmap,
		       xf86CrtcPtr crtc, RegionPtr clipBoxes)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	BoxPtr box;
	pixman_box16_t box_in_crtc_coordinates;
	int pipe = -1, event, load_scan_lines_pipe;

	if (pixmap_is_scanout(pixmap))
		pipe = i830_crtc_to_pipe(crtc);

	if (pipe >= 0) {
		if (pipe == 0) {
			event = MI_WAIT_FOR_PIPEA_SCAN_LINE_WINDOW;
			load_scan_lines_pipe = MI_LOAD_SCAN_LINES_DISPLAY_PIPEA;
		} else {
			event = MI_WAIT_FOR_PIPEB_SCAN_LINE_WINDOW;
			load_scan_lines_pipe = MI_LOAD_SCAN_LINES_DISPLAY_PIPEB;
		}

		box = REGION_EXTENTS(unused, clipBoxes);
		box_in_crtc_coordinates = *box;
		if (crtc->transform_in_use)
			pixman_f_transform_bounds(&crtc->f_framebuffer_to_crtc,
						  &box_in_crtc_coordinates);

		BEGIN_BATCH(5);
		/* The documentation says that the LOAD_SCAN_LINES command
		 * always comes in pairs. Don't ask me why. */
		OUT_BATCH(MI_LOAD_SCAN_LINES_INCL | load_scan_lines_pipe);
		OUT_BATCH((box_in_crtc_coordinates.
			   y1 << 16) | box_in_crtc_coordinates.y2);
		OUT_BATCH(MI_LOAD_SCAN_LINES_INCL | load_scan_lines_pipe);
		OUT_BATCH((box_in_crtc_coordinates.
			   y1 << 16) | box_in_crtc_coordinates.y2);
		OUT_BATCH(MI_WAIT_FOR_EVENT | event);
		ADVANCE_BATCH();
	}
}

static Bool
i830_setup_video_buffer(ScrnInfoPtr scrn, intel_adaptor_private *adaptor_priv,
			int alloc_size, int id, unsigned char *buf)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* Free the current buffer if we're going to have to reallocate */
	if (adaptor_priv->buf && adaptor_priv->buf->size < alloc_size) {
		drm_intel_bo_unreference(adaptor_priv->buf);
		adaptor_priv->buf = NULL;
	}

	if (xvmc_passthrough(id)) {
		i830_free_video_buffers(adaptor_priv);
		if (IS_I965G(intel)) {
			adaptor_priv->buf =
				drm_intel_bo_gem_create_from_name(intel->bufmgr,
								  "xvmc surface",
								  (uintptr_t)buf);
		}
	} else {
		if (adaptor_priv->buf == NULL) {
			adaptor_priv->buf = drm_intel_bo_alloc(intel->bufmgr,
							"xv buffer", alloc_size,
							4096);
			if (adaptor_priv->buf == NULL)
				return FALSE;
		}
	}

	return TRUE;
}

static void
i830_dst_pitch_and_size(ScrnInfoPtr scrn, intel_adaptor_private *adaptor_priv, short width,
			short height, int *dstPitch, int *dstPitch2, int *size,
			int id)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int pitchAlignMask;

	/* Only needs to be DWORD-aligned for textured on i915, but overlay has
	 * stricter requirements.
	 */
	if (adaptor_priv->textured) {
		pitchAlignMask = 3;
#ifdef INTEL_XVMC
		/* for i915 xvmc, hw requires at least 1kb aligned surface */
		if ((id == FOURCC_XVMC) && IS_I915(intel))
			pitchAlignMask = 0x3ff;
#endif
	} else {
		if (IS_I965G(intel))
			pitchAlignMask = 255;
		else
			pitchAlignMask = 63;
	}

	/* Determine the desired destination pitch (representing the chroma's pitch,
	 * in the planar case.
	 */
	switch (id) {
	case FOURCC_YV12:
	case FOURCC_I420:
		if (adaptor_priv->rotation & (RR_Rotate_90 | RR_Rotate_270)) {
			*dstPitch =
			    ((height / 2) + pitchAlignMask) & ~pitchAlignMask;
			*size = *dstPitch * width * 3;
		} else {
			*dstPitch =
			    ((width / 2) + pitchAlignMask) & ~pitchAlignMask;
			*size = *dstPitch * height * 3;
		}
		break;
	case FOURCC_UYVY:
	case FOURCC_YUY2:

		if (adaptor_priv->rotation & (RR_Rotate_90 | RR_Rotate_270)) {
			*dstPitch =
			    ((height << 1) + pitchAlignMask) & ~pitchAlignMask;
			*size = *dstPitch * width;
		} else {
			*dstPitch =
			    ((width << 1) + pitchAlignMask) & ~pitchAlignMask;
			*size = *dstPitch * height;
		}
		break;
#ifdef INTEL_XVMC
	case FOURCC_XVMC:
		*dstPitch = ((width / 2) + pitchAlignMask) & ~pitchAlignMask;
		*dstPitch2 = (width + pitchAlignMask) & ~pitchAlignMask;
		*size = 0;
		break;
#endif
	default:
		*dstPitch = 0;
		*size = 0;
		break;
	}
#if 0
	ErrorF("srcPitch: %d, dstPitch: %d, size: %d\n", srcPitch, *dstPitch,
	       size);
#endif
}

static Bool
i830_copy_video_data(ScrnInfoPtr scrn, intel_adaptor_private *adaptor_priv,
		     short width, short height, int *dstPitch, int *dstPitch2,
		     INT32 x1, INT32 y1, INT32 x2, INT32 y2,
		     int id, unsigned char *buf)
{
#ifdef INTEL_XVMC
	intel_screen_private *intel = intel_get_screen_private(scrn);
#endif
	int srcPitch = 0, srcPitch2 = 0;
	int top, left, npixels, nlines, size;

	if (is_planar_fourcc(id)) {
		srcPitch = (width + 0x3) & ~0x3;
		srcPitch2 = ((width >> 1) + 0x3) & ~0x3;
	} else {
		srcPitch = width << 1;
	}

	i830_dst_pitch_and_size(scrn, adaptor_priv, width, height, dstPitch,
				dstPitch2, &size, id);

	if (!i830_setup_video_buffer(scrn, adaptor_priv, size, id, buf))
		return FALSE;

	/* fixup pointers */
#ifdef INTEL_XVMC
	if (id == FOURCC_XVMC && IS_I915(intel)) {
		adaptor_priv->YBufOffset = (uint32_t) ((uintptr_t) buf);
		adaptor_priv->VBufOffset = adaptor_priv->YBufOffset + (*dstPitch2 * height);
		adaptor_priv->UBufOffset =
		    adaptor_priv->VBufOffset + (*dstPitch * height / 2);
	} else {
#endif
		adaptor_priv->YBufOffset = 0;

		if (adaptor_priv->rotation & (RR_Rotate_90 | RR_Rotate_270)) {
			adaptor_priv->UBufOffset =
			    adaptor_priv->YBufOffset + (*dstPitch * 2 * width);
			adaptor_priv->VBufOffset =
			    adaptor_priv->UBufOffset + (*dstPitch * width / 2);
		} else {
			adaptor_priv->UBufOffset =
			    adaptor_priv->YBufOffset + (*dstPitch * 2 * height);
			adaptor_priv->VBufOffset =
			    adaptor_priv->UBufOffset + (*dstPitch * height / 2);
		}
#ifdef INTEL_XVMC
	}
#endif

	/* copy data */
	top = y1 >> 16;
	left = (x1 >> 16) & ~1;
	npixels = ((((x2 + 0xffff) >> 16) + 1) & ~1) - left;

	if (is_planar_fourcc(id)) {
		if (!xvmc_passthrough(id)) {
			top &= ~1;
			nlines = ((((y2 + 0xffff) >> 16) + 1) & ~1) - top;
			I830CopyPlanarData(adaptor_priv, buf, srcPitch, srcPitch2,
					   *dstPitch, height, top, left, nlines,
					   npixels, id);
		}
	} else {
		nlines = ((y2 + 0xffff) >> 16) - top;
		I830CopyPackedData(adaptor_priv, buf, srcPitch, *dstPitch, top, left,
				   nlines, npixels);
	}

	return TRUE;
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
I830PutImage(ScrnInfoPtr scrn,
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
	ScreenPtr screen = screenInfo.screens[scrn->scrnIndex];
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	INT32 x1, x2, y1, y2;
	int dstPitch;
	int dstPitch2 = 0;
	BoxRec dstBox;
	xf86CrtcPtr crtc;

#if 0
	ErrorF("I830PutImage: src: (%d,%d)(%d,%d), dst: (%d,%d)(%d,%d)\n"
	       "width %d, height %d\n", src_x, src_y, src_w, src_h, drw_x,
	       drw_y, drw_w, drw_h, width, height);
#endif

	if (!adaptor_priv->textured) {
		/* If dst width and height are less than 1/8th the src size, the
		 * src/dst scale factor becomes larger than 8 and doesn't fit in
		 * the scale register. */
		if (src_w >= (drw_w * 8))
			drw_w = src_w / 7;

		if (src_h >= (drw_h * 8))
			drw_h = src_h / 7;
	}

	/* Clip */
	x1 = src_x;
	x2 = src_x + src_w;
	y1 = src_y;
	y2 = src_y + src_h;

	dstBox.x1 = drw_x;
	dstBox.x2 = drw_x + drw_w;
	dstBox.y1 = drw_y;
	dstBox.y2 = drw_y + drw_h;

	if (!i830_clip_video_helper(scrn,
				    adaptor_priv,
				    &crtc,
				    &dstBox, &x1, &x2, &y1, &y2, clipBoxes,
				    width, height))
		return Success;

	if (!adaptor_priv->textured) {
		/* texture video handles rotation differently. */
		if (crtc)
			adaptor_priv->rotation = crtc->rotation;
		else {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "Fail to clip video to any crtc!\n");
			return Success;
		}
	}

	if (!i830_copy_video_data(scrn, adaptor_priv, width, height,
				  &dstPitch, &dstPitch2,
				  x1, y1, x2, y2, id, buf))
		return BadAlloc;

	if (!adaptor_priv->textured) {
		if (!i830_display_overlay
		    (scrn, crtc, id, width, height, dstPitch, x1, y1, x2, y2,
		     &dstBox, src_w, src_h, drw_w, drw_h))
			return BadAlloc;

		/* update cliplist */
		if (!REGION_EQUAL(scrn->screen, &adaptor_priv->clip, clipBoxes)) {
			REGION_COPY(scrn->screen, &adaptor_priv->clip, clipBoxes);
			i830_fill_colorkey(screen, adaptor_priv->colorKey, clipBoxes);
		}
	} else {
		if (crtc && adaptor_priv->SyncToVblank != 0) {
			i830_wait_for_scanline(scrn, pixmap, crtc, clipBoxes);
		}

		if (IS_I965G(intel)) {
			I965DisplayVideoTextured(scrn, adaptor_priv, id, clipBoxes,
						 width, height, dstPitch, x1,
						 y1, x2, y2, src_w, src_h,
						 drw_w, drw_h, pixmap);
		} else {
			I915DisplayVideoTextured(scrn, adaptor_priv, id, clipBoxes,
						 width, height, dstPitch,
						 dstPitch2, x1, y1, x2, y2,
						 src_w, src_h, drw_w, drw_h,
						 pixmap);
		}

		DamageDamageRegion(drawable, clipBoxes);
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
I830VideoBlockHandler(int i, pointer blockData, pointer pTimeout,
		      pointer pReadmask)
{
	ScrnInfoPtr scrn = xf86Screens[i];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	intel_adaptor_private *adaptor_priv;

	/* no overlay */
	if (intel->adaptor == NULL)
		return;

	adaptor_priv = intel_get_adaptor_private(scrn);

	if (adaptor_priv->videoStatus & TIMER_MASK) {
#if 1
		Time now = currentTime.milliseconds;
#else
		UpdateCurrentTime();
#endif
		if (adaptor_priv->videoStatus & OFF_TIMER) {
			if (adaptor_priv->offTime < now) {
				/* Turn off the overlay */
				OVERLAY_DEBUG("BLOCKHANDLER\n");

				ums_overlay_off (scrn);

				adaptor_priv->videoStatus = FREE_TIMER;
				adaptor_priv->freeTime = now + FREE_DELAY;
			    }
		} else {				/* FREE_TIMER */
			if (adaptor_priv->freeTime < now) {
				i830_free_video_buffers(adaptor_priv);
				adaptor_priv->videoStatus = 0;
			}
		}
	}
}

void
i830_crtc_dpms_video(xf86CrtcPtr crtc, Bool on)
{
	ScrnInfoPtr scrn = crtc->scrn;
	intel_screen_private *intel = intel_get_screen_private(scrn);
	intel_adaptor_private *adaptor_priv;

	/* no overlay */
	if (intel->adaptor == NULL)
		return;

	adaptor_priv = intel->adaptor->pPortPrivates[0].ptr;

	if (crtc != adaptor_priv->current_crtc)
		return;

	/* Check if it's the crtc the overlay is off */
	if (!on) {
		/* We stop the video when mode switching, so we don't lock up
		 * the engine. The overlayOK will determine whether we can
		 * re-enable with the current video on completion of the mode
		 * switch.
		 */
		I830StopVideo(scrn, adaptor_priv, TRUE);
		adaptor_priv->current_crtc = NULL;
		adaptor_priv->overlayOK = FALSE;
		adaptor_priv->oneLineMode = FALSE;
	}
}
