/***************************************************************************

 Copyright 2000-2011 Intel Corporation.  All Rights Reserved.

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sna.h"
#include "sna_video.h"

#include <xf86drm.h>
#include <xf86xv.h>
#include <X11/extensions/Xv.h>
#include <fourcc.h>
#include <i915_drm.h>

#include "intel_options.h"

#define MAKE_ATOM(a) MakeAtom(a, sizeof(a) - 1, TRUE)

#define HAS_GAMMA(sna) ((sna)->kgem.gen >= 030)

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
static const XF86VideoFormatRec Formats[NUM_FORMATS] = {
	{15, TrueColor}, {16, TrueColor}, {24, TrueColor}
};

#define NUM_ATTRIBUTES 5
static const XF86AttributeRec Attributes[NUM_ATTRIBUTES] = {
	{XvSettable | XvGettable, 0, (1 << 24) - 1, "XV_COLORKEY"},
	{XvSettable | XvGettable, -128, 127, "XV_BRIGHTNESS"},
	{XvSettable | XvGettable, 0, 255, "XV_CONTRAST"},
	{XvSettable | XvGettable, 0, 1023, "XV_SATURATION"},
	{XvSettable | XvGettable, -1, 1, "XV_PIPE"}
};

#define GAMMA_ATTRIBUTES 6
static const XF86AttributeRec GammaAttributes[GAMMA_ATTRIBUTES] = {
	{XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA0"},
	{XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA1"},
	{XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA2"},
	{XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA3"},
	{XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA4"},
	{XvSettable | XvGettable, 0, 0xffffff, "XV_GAMMA5"}
};

#define NUM_IMAGES 4
static const XF86ImageRec Images[NUM_IMAGES] = {
	XVIMAGE_YUY2,
	XVIMAGE_YV12,
	XVIMAGE_I420,
	XVIMAGE_UYVY,
};

/* kernel modesetting overlay functions */
static bool sna_has_overlay(struct sna *sna)
{
	struct drm_i915_getparam gp;
	int has_overlay = 0;
	int ret;

	VG_CLEAR(gp);
	gp.param = I915_PARAM_HAS_OVERLAY;
	gp.value = &has_overlay;
	ret = drmIoctl(sna->kgem.fd, DRM_IOCTL_I915_GETPARAM, &gp);
	return ret == 0 && has_overlay;
}

static bool sna_video_overlay_update_attrs(struct sna *sna,
					   struct sna_video *video)
{
	struct drm_intel_overlay_attrs attrs;

	DBG(("%s()\n", __FUNCTION__));

	attrs.flags = I915_OVERLAY_UPDATE_ATTRS;
	attrs.brightness = video->brightness;
	attrs.contrast = video->contrast;
	attrs.saturation = video->saturation;
	attrs.color_key = video->color_key;
	attrs.gamma0 = video->gamma0;
	attrs.gamma1 = video->gamma1;
	attrs.gamma2 = video->gamma2;
	attrs.gamma3 = video->gamma3;
	attrs.gamma4 = video->gamma4;
	attrs.gamma5 = video->gamma5;

	return drmIoctl(sna->kgem.fd, DRM_IOCTL_I915_OVERLAY_ATTRS, &attrs) == 0;
}

static void sna_video_overlay_off(struct sna *sna)
{
	struct drm_intel_overlay_put_image request;

	DBG(("%s()\n", __FUNCTION__));

	request.flags = 0;

	/* Not much we can do if the hardware dies before we turn it off! */
	(void)drmIoctl(sna->kgem.fd,
		       DRM_IOCTL_I915_OVERLAY_PUT_IMAGE,
		       &request);
}

static void sna_video_overlay_stop(ScrnInfoPtr scrn,
				   pointer data,
				   Bool shutdown)
{
	struct sna *sna = to_sna(scrn);
	struct sna_video *video = data;

	DBG(("%s()\n", __FUNCTION__));

	REGION_EMPTY(scrn->pScreen, &video->clip);

	if (!shutdown)
		return;

	sna_video_overlay_off(sna);
	sna_video_free_buffers(sna, video);
}

static int
sna_video_overlay_set_port_attribute(ScrnInfoPtr scrn,
				     Atom attribute, INT32 value, pointer data)
{
	struct sna *sna = to_sna(scrn);
	struct sna_video *video = data;

	if (attribute == xvBrightness) {
		if ((value < -128) || (value > 127))
			return BadValue;
		DBG(("%s: BRIGHTNESS %d -> %d\n", __FUNCTION__,
		     video->contrast, (int)value));
		video->brightness = value;
	} else if (attribute == xvContrast) {
		if ((value < 0) || (value > 255))
			return BadValue;
		DBG(("%s: CONTRAST %d -> %d\n", __FUNCTION__,
		     video->contrast, (int)value));
		video->contrast = value;
	} else if (attribute == xvSaturation) {
		if ((value < 0) || (value > 1023))
			return BadValue;
		DBG(("%s: SATURATION %d -> %d\n", __FUNCTION__,
		     video->saturation, (int)value));
		video->saturation = value;
	} else if (attribute == xvPipe) {
		xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
		if ((value < -1) || (value >= xf86_config->num_crtc))
			return BadValue;
		if (value < 0)
			video->desired_crtc = NULL;
		else
			video->desired_crtc = xf86_config->crtc[value];
	} else if (attribute == xvGamma0 && HAS_GAMMA(sna)) {
		video->gamma0 = value;
	} else if (attribute == xvGamma1 && HAS_GAMMA(sna)) {
		video->gamma1 = value;
	} else if (attribute == xvGamma2 && HAS_GAMMA(sna)) {
		video->gamma2 = value;
	} else if (attribute == xvGamma3 && HAS_GAMMA(sna)) {
		video->gamma3 = value;
	} else if (attribute == xvGamma4 && HAS_GAMMA(sna)) {
		video->gamma4 = value;
	} else if (attribute == xvGamma5 && HAS_GAMMA(sna)) {
		video->gamma5 = value;
	} else if (attribute == xvColorKey) {
		video->color_key = value;
		DBG(("COLORKEY\n"));
	} else
		return BadMatch;

	if ((attribute == xvGamma0 ||
	     attribute == xvGamma1 ||
	     attribute == xvGamma2 ||
	     attribute == xvGamma3 ||
	     attribute == xvGamma4 ||
	     attribute == xvGamma5) && HAS_GAMMA(sna)) {
		DBG(("%s: GAMMA\n", __FUNCTION__));
	}

	if (!sna_video_overlay_update_attrs(sna, data))
		return BadValue;

	if (attribute == xvColorKey)
		REGION_EMPTY(scrn->pScreen, &video->clip);

	return Success;
}

static int
sna_video_overlay_get_port_attribute(ScrnInfoPtr scrn,
				     Atom attribute, INT32 * value, pointer data)
{
	struct sna *sna = to_sna(scrn);
	struct sna_video *video = (struct sna_video *) data;

	if (attribute == xvBrightness) {
		*value = video->brightness;
	} else if (attribute == xvContrast) {
		*value = video->contrast;
	} else if (attribute == xvSaturation) {
		*value = video->saturation;
	} else if (attribute == xvPipe) {
		int c;
		xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
		for (c = 0; c < xf86_config->num_crtc; c++)
			if (xf86_config->crtc[c] == video->desired_crtc)
				break;
		if (c == xf86_config->num_crtc)
			c = -1;
		*value = c;
	} else if (attribute == xvGamma0 && HAS_GAMMA(sna)) {
		*value = video->gamma0;
	} else if (attribute == xvGamma1 && HAS_GAMMA(sna)) {
		*value = video->gamma1;
	} else if (attribute == xvGamma2 && HAS_GAMMA(sna)) {
		*value = video->gamma2;
	} else if (attribute == xvGamma3 && HAS_GAMMA(sna)) {
		*value = video->gamma3;
	} else if (attribute == xvGamma4 && HAS_GAMMA(sna)) {
		*value = video->gamma4;
	} else if (attribute == xvGamma5 && HAS_GAMMA(sna)) {
		*value = video->gamma5;
	} else if (attribute == xvColorKey) {
		*value = video->color_key;
	} else if (attribute == xvSyncToVblank) {
		*value = video->SyncToVblank;
	} else
		return BadMatch;

	return Success;
}

static void
sna_video_overlay_query_best_size(ScrnInfoPtr scrn,
				  Bool motion,
				  short vid_w, short vid_h,
				  short drw_w, short drw_h,
				  unsigned int *p_w, unsigned int *p_h,
				  pointer data)
{
	struct sna *sna = to_sna(scrn);
	short max_w, max_h;

	if (vid_w > (drw_w << 1) || vid_h > (drw_h << 1)){
		drw_w = vid_w >> 1;
		drw_h = vid_h >> 1;
	}

	if (sna->kgem.gen < 021) {
		max_w = IMAGE_MAX_WIDTH_LEGACY;
		max_h = IMAGE_MAX_HEIGHT_LEGACY;
	} else {
		max_w = IMAGE_MAX_WIDTH;
		max_h = IMAGE_MAX_HEIGHT;
	}

	while (drw_w > max_w || drw_h > max_h) {
		drw_w >>= 1;
		drw_h >>= 1;
	}

	*p_w = drw_w;
	*p_h = drw_h;
}

static void
update_dst_box_to_crtc_coords(struct sna *sna, xf86CrtcPtr crtc, BoxPtr dstBox)
{
	ScrnInfoPtr scrn = sna->scrn;
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

static bool
sna_video_overlay_show(struct sna *sna,
		       struct sna_video *video,
		       struct sna_video_frame *frame,
		       xf86CrtcPtr crtc,
		       BoxPtr dstBox,
		       short src_w, short src_h,
		       short drw_w, short drw_h)
{
	struct drm_intel_overlay_put_image request;
	bool planar = is_planar_fourcc(frame->id);
	float scale;

	DBG(("%s: src=(%dx%d), dst=(%dx%d)\n", __FUNCTION__,
	     src_w, src_h, drw_w, drw_h));

	update_dst_box_to_crtc_coords(sna, crtc, dstBox);
	if (crtc->rotation & (RR_Rotate_90 | RR_Rotate_270)) {
		int tmp;

		tmp = frame->width;
		frame->width = frame->height;
		frame->height = tmp;

		tmp = drw_w;
		drw_w = drw_h;
		drw_h = tmp;

		tmp = src_w;
		src_w = src_h;
		src_h = tmp;
	}

	memset(&request, 0, sizeof(request));
	request.flags = I915_OVERLAY_ENABLE;

	request.bo_handle = frame->bo->handle;
	if (planar) {
		request.stride_Y = frame->pitch[1];
		request.stride_UV = frame->pitch[0];
	} else {
		request.stride_Y = frame->pitch[0];
		request.stride_UV = 0;
	}
	request.offset_Y = 0;
	request.offset_U = frame->UBufOffset;
	request.offset_V = frame->VBufOffset;
	DBG(("%s: handle=%d, stride_Y=%d, stride_UV=%d, off_Y: %i, off_U: %i, off_V: %i\n",
	     __FUNCTION__,
	     request.bo_handle, request.stride_Y, request.stride_UV,
	     request.offset_Y, request.offset_U, request.offset_V));

	request.crtc_id = sna_crtc_id(crtc);
	request.dst_x = dstBox->x1;
	request.dst_y = dstBox->y1;
	request.dst_width = dstBox->x2 - dstBox->x1;
	request.dst_height = dstBox->y2 - dstBox->y1;

	DBG(("%s: crtc=%d, dst=(%d, %d)x(%d, %d)\n",
	     __FUNCTION__, request.crtc_id,
	     request.dst_x, request.dst_y,
	     request.dst_width, request.dst_height));

	request.src_width = frame->width;
	request.src_height = frame->height;
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

	DBG(("%s: src=(%d, %d) scan=(%d, %d)\n",
	     __FUNCTION__,
	     request.src_width, request.src_height,
	     request.src_scan_width, request.src_scan_height));

	if (planar) {
		request.flags |= I915_OVERLAY_YUV_PLANAR | I915_OVERLAY_YUV420;
	} else {
		request.flags |= I915_OVERLAY_YUV_PACKED | I915_OVERLAY_YUV422;
		if (frame->id == FOURCC_UYVY)
			request.flags |= I915_OVERLAY_Y_SWAP;
	}

	DBG(("%s: flags=%x\n", __FUNCTION__, request.flags));

	return drmIoctl(sna->kgem.fd, DRM_IOCTL_I915_OVERLAY_PUT_IMAGE, &request) == 0;
}

static int
sna_video_overlay_put_image(ScrnInfoPtr scrn,
			    short src_x, short src_y,
			    short drw_x, short drw_y,
			    short src_w, short src_h,
			    short drw_w, short drw_h,
			    int id, unsigned char *buf,
			    short width, short height,
			    Bool sync, RegionPtr clip, pointer data,
			    DrawablePtr drawable)
{
	struct sna *sna = to_sna(scrn);
	struct sna_video *video = data;
	struct sna_video_frame frame;
	BoxRec dstBox;
	xf86CrtcPtr crtc;

	DBG(("%s: src: (%d,%d)(%d,%d), dst: (%d,%d)(%d,%d), width %d, height %d\n",
	     __FUNCTION__,
	     src_x, src_y, src_w, src_h, drw_x,
	     drw_y, drw_w, drw_h, width, height));

	/* If dst width and height are less than 1/8th the src size, the
	 * src/dst scale factor becomes larger than 8 and doesn't fit in
	 * the scale register. */
	if (src_w >= (drw_w * 8))
		drw_w = src_w / 7;

	if (src_h >= (drw_h * 8))
		drw_h = src_h / 7;

	sna_video_frame_init(sna, video, id, width, height, &frame);

	if (!sna_video_clip_helper(scrn,
				   video,
				   &frame,
				   &crtc,
				   &dstBox,
				   src_x, src_y, drw_x, drw_y,
				   src_w, src_h, drw_w, drw_h,
				   clip))
		return Success;

	if (!crtc) {
		/*
		 * If the video isn't visible on any CRTC, turn it off
		 */
		sna_video_overlay_off(sna);
		return Success;
	}

	/* overlay can't handle rotation natively, store it for the copy func */
	video->rotation = crtc->rotation;

	frame.bo = sna_video_buffer(sna, video, &frame);
	if (frame.bo == NULL) {
		DBG(("%s: failed to allocate video bo\n", __FUNCTION__));
		return BadAlloc;
	}

	if (!sna_video_copy_data(sna, video, &frame, buf)) {
		DBG(("%s: failed to copy video data\n", __FUNCTION__));
		return BadAlloc;
	}

	if (!sna_video_overlay_show
	    (sna, video, &frame, crtc, &dstBox, src_w, src_h, drw_w, drw_h)) {
		DBG(("%s: failed to show video frame\n", __FUNCTION__));
		return BadAlloc;
	}

	frame.bo->domain = DOMAIN_NONE;
	sna_video_buffer_fini(sna, video);

	/* update cliplist */
	if (!REGION_EQUAL(scrn->pScreen, &video->clip, clip)) {
		REGION_COPY(scrn->pScreen, &video->clip, clip);
		xf86XVFillKeyHelperDrawable(drawable, video->color_key, clip);
	}

	return Success;
}

static int
sna_video_overlay_query_video_attributes(ScrnInfoPtr scrn,
					 int id,
					 unsigned short *w, unsigned short *h,
					 int *pitches, int *offsets)
{
	struct sna *sna = to_sna(scrn);
	int size, tmp;

	DBG(("%s: w is %d, h is %d\n", __FUNCTION__, *w, *h));

	if (sna->kgem.gen < 021) {
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

static int sna_video_overlay_color_key(struct sna *sna)
{
	ScrnInfoPtr scrn = sna->scrn;
	int color_key;

	if (xf86GetOptValInteger(sna->Options, OPTION_VIDEO_KEY,
				 &color_key)) {
	} else if (xf86GetOptValInteger(sna->Options, OPTION_COLOR_KEY,
					&color_key)) {
	} else {
		color_key =
		    (1 << scrn->offset.red) |
		    (1 << scrn->offset.green) |
		    (((scrn->mask.blue >> scrn->offset.blue) - 1) << scrn->offset.blue);
	}

	return color_key & ((1 << scrn->depth) - 1);
}

XF86VideoAdaptorPtr sna_video_overlay_setup(struct sna *sna,
					    ScreenPtr screen)
{
	XF86VideoAdaptorPtr adaptor;
	struct sna_video *video;

	if (!sna_has_overlay(sna)) {
		xf86DrvMsg(sna->scrn->scrnIndex, X_INFO,
			   "Overlay video not supported on this hardware\n");
		return NULL;
	}

	DBG(("%s()\n", __FUNCTION__));

	if (!(adaptor = calloc(1,
			     sizeof(XF86VideoAdaptorRec) +
			     sizeof(struct sna_video) +
			     sizeof(DevUnion))))
		return NULL;

	adaptor->type = XvWindowMask | XvInputMask | XvImageMask;
	adaptor->flags = VIDEO_OVERLAID_IMAGES /*| VIDEO_CLIP_TO_VIEWPORT */ ;
	adaptor->name = "Intel(R) Video Overlay";
	adaptor->nEncodings = 1;
	adaptor->pEncodings = xnfalloc(sizeof(DummyEncoding));
	memcpy(adaptor->pEncodings, DummyEncoding, sizeof(DummyEncoding));
	if (sna->kgem.gen < 021) {
		adaptor->pEncodings->width = IMAGE_MAX_WIDTH_LEGACY;
		adaptor->pEncodings->height = IMAGE_MAX_HEIGHT_LEGACY;
	}
	adaptor->nFormats = NUM_FORMATS;
	adaptor->pFormats = (XF86VideoFormatPtr)Formats;
	adaptor->nPorts = 1;
	adaptor->pPortPrivates = (DevUnion *)&adaptor[1];

	video = (struct sna_video *)&adaptor->pPortPrivates[1];

	adaptor->pPortPrivates[0].ptr = video;
	adaptor->nAttributes = NUM_ATTRIBUTES;
	if (HAS_GAMMA(sna))
		adaptor->nAttributes += GAMMA_ATTRIBUTES;

	 adaptor->pAttributes =
	    xnfalloc(sizeof(XF86AttributeRec) * adaptor->nAttributes);
	/* Now copy the attributes */
	memcpy(adaptor->pAttributes, Attributes, sizeof(XF86AttributeRec) * NUM_ATTRIBUTES);
	if (HAS_GAMMA(sna))
		memcpy(adaptor->pAttributes + NUM_ATTRIBUTES, GammaAttributes,
		       sizeof(XF86AttributeRec) * GAMMA_ATTRIBUTES);

	adaptor->nImages = NUM_IMAGES;
	adaptor->pImages = (XF86ImagePtr)Images;
	adaptor->PutVideo = NULL;
	adaptor->PutStill = NULL;
	adaptor->GetVideo = NULL;
	adaptor->GetStill = NULL;
	adaptor->StopVideo = sna_video_overlay_stop;
	adaptor->SetPortAttribute = sna_video_overlay_set_port_attribute;
	adaptor->GetPortAttribute = sna_video_overlay_get_port_attribute;
	adaptor->QueryBestSize = sna_video_overlay_query_best_size;
	adaptor->PutImage = sna_video_overlay_put_image;
	adaptor->QueryImageAttributes = sna_video_overlay_query_video_attributes;

	if (sna->kgem.gen >= 040)
		/* Actually the alignment is 64 bytes, too. But the
		 * stride must be at least 512 bytes. Take the easy fix
		 * and align on 512 bytes unconditionally. */
		video->alignment = 512;
	else if (sna->kgem.gen < 021)
		/* Harsh, errata on these chipsets limit the stride
		 * to be a multiple of 256 bytes.
		 */
		video->alignment = 256;
	else
		video->alignment = 64;
	video->textured = false;
	video->color_key = sna_video_overlay_color_key(sna);
	video->brightness = -19;	/* (255/219) * -16 */
	video->contrast = 75;	/* 255/219 * 64 */
	video->saturation = 146;	/* 128/112 * 128 */
	video->desired_crtc = NULL;
	video->gamma5 = 0xc0c0c0;
	video->gamma4 = 0x808080;
	video->gamma3 = 0x404040;
	video->gamma2 = 0x202020;
	video->gamma1 = 0x101010;
	video->gamma0 = 0x080808;

	video->rotation = RR_Rotate_0;

	/* gotta uninit this someplace */
	REGION_NULL(screen, &video->clip);

	xvColorKey = MAKE_ATOM("XV_COLORKEY");
	xvBrightness = MAKE_ATOM("XV_BRIGHTNESS");
	xvContrast = MAKE_ATOM("XV_CONTRAST");
	xvSaturation = MAKE_ATOM("XV_SATURATION");

	/* Allow the pipe to be switched from pipe A to B when in clone mode */
	xvPipe = MAKE_ATOM("XV_PIPE");

	if (HAS_GAMMA(sna)) {
		xvGamma0 = MAKE_ATOM("XV_GAMMA0");
		xvGamma1 = MAKE_ATOM("XV_GAMMA1");
		xvGamma2 = MAKE_ATOM("XV_GAMMA2");
		xvGamma3 = MAKE_ATOM("XV_GAMMA3");
		xvGamma4 = MAKE_ATOM("XV_GAMMA4");
		xvGamma5 = MAKE_ATOM("XV_GAMMA5");
	}

	sna_video_overlay_update_attrs(sna, video);

	return adaptor;
}
