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

static Atom xvBrightness, xvContrast, xvSaturation, xvColorKey, xvPipe, xvAlwaysOnTop;
static Atom xvGamma0, xvGamma1, xvGamma2, xvGamma3, xvGamma4, xvGamma5;

/* Limits for the overlay/textured video source sizes.  The documented hardware
 * limits are 2048x2048 or better for overlay and both of our textured video
 * implementations.  Additionally, on the 830 and 845, larger sizes resulted in
 * the card hanging, so we keep the limits lower there.
 */
#define IMAGE_MAX_WIDTH		2048
#define IMAGE_MAX_HEIGHT	2048
#define IMAGE_MAX_WIDTH_LEGACY	1024
#define IMAGE_MAX_HEIGHT_LEGACY	1088

static XvFormatRec Formats[] = { {15}, {16}, {24} };

static const XvAttributeRec Attributes[] = {
	{XvSettable | XvGettable, 0, (1 << 24) - 1, (char *)"XV_COLORKEY"},
	{XvSettable | XvGettable, 0, 1, (char *)"XV_ALWAYS_ON_TOP"},
	{XvSettable | XvGettable, -128, 127, (char *)"XV_BRIGHTNESS"},
	{XvSettable | XvGettable, 0, 255, (char *)"XV_CONTRAST"},
	{XvSettable | XvGettable, 0, 1023, (char *)"XV_SATURATION"},
	{XvSettable | XvGettable, -1, 1, (char *)"XV_PIPE"},
#define NUM_ATTRIBUTES 6

	{XvSettable | XvGettable, 0, 0xffffff, (char *)"XV_GAMMA0"},
	{XvSettable | XvGettable, 0, 0xffffff, (char *)"XV_GAMMA1"},
	{XvSettable | XvGettable, 0, 0xffffff, (char *)"XV_GAMMA2"},
	{XvSettable | XvGettable, 0, 0xffffff, (char *)"XV_GAMMA3"},
	{XvSettable | XvGettable, 0, 0xffffff, (char *)"XV_GAMMA4"},
	{XvSettable | XvGettable, 0, 0xffffff, (char *)"XV_GAMMA5"}
#define GAMMA_ATTRIBUTES 6
};

static const XvImageRec Images[] = {
	XVIMAGE_YUY2,
	XVIMAGE_YV12,
	XVIMAGE_I420,
	XVIMAGE_UYVY,
	XVMC_YUV
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

static bool sna_video_overlay_update_attrs(struct sna_video *video)
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

	if (video->AlwaysOnTop)
		attrs.flags |= 1<<2;

	return drmIoctl(video->sna->kgem.fd, DRM_IOCTL_I915_OVERLAY_ATTRS, &attrs) == 0;
}

static int sna_video_overlay_stop(ddStopVideo_ARGS)
{
	struct sna_video *video = port->devPriv.ptr;
	struct sna *sna = video->sna;
	struct drm_intel_overlay_put_image request;

	DBG(("%s()\n", __FUNCTION__));

	REGION_EMPTY(scrn->pScreen, &video->clip);

	request.flags = 0;
	(void)drmIoctl(sna->kgem.fd,
		       DRM_IOCTL_I915_OVERLAY_PUT_IMAGE,
		       &request);

	if (video->bo[0])
		kgem_bo_destroy(&sna->kgem, video->bo[0]);
	video->bo[0] = NULL;

	sna_video_free_buffers(video);
	sna_window_set_port((WindowPtr)draw, NULL);
	return Success;
}

static int
sna_video_overlay_set_attribute(ddSetPortAttribute_ARGS)
{
	struct sna_video *video = port->devPriv.ptr;
	struct sna *sna = video->sna;

	DBG(("%s: set(%lx) to %d\n", __FUNCTION__, (long)attribute, (int)value));
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
		xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(sna->scrn);
		if ((value < -1) || (value >= xf86_config->num_crtc))
			return BadValue;
		if (value < 0)
			video->desired_crtc = NULL;
		else
			video->desired_crtc = xf86_config->crtc[value];
	} else if (attribute == xvAlwaysOnTop) {
		DBG(("%s: ALWAYS_ON_TOP: %d -> %d\n", __FUNCTION__,
		     video->AlwaysOnTop, !!value));
		video->AlwaysOnTop = !!value;
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
		RegionEmpty(&video->clip);
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

	if (!sna_video_overlay_update_attrs(video))
		return BadValue;

	return Success;
}

static int
sna_video_overlay_get_attribute(ddGetPortAttribute_ARGS)
{
	struct sna_video *video = port->devPriv.ptr;
	struct sna *sna = video->sna;

	if (attribute == xvBrightness) {
		*value = video->brightness;
	} else if (attribute == xvContrast) {
		*value = video->contrast;
	} else if (attribute == xvSaturation) {
		*value = video->saturation;
	} else if (attribute == xvPipe) {
		int c;
		xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(sna->scrn);
		for (c = 0; c < xf86_config->num_crtc; c++)
			if (xf86_config->crtc[c] == video->desired_crtc)
				break;
		if (c == xf86_config->num_crtc)
			c = -1;
		*value = c;
	} else if (attribute == xvAlwaysOnTop) {
		*value = video->AlwaysOnTop;
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
	} else
		return BadMatch;

	return Success;
}

static int
sna_video_overlay_best_size(ddQueryBestSize_ARGS)
{
	struct sna_video *video = port->devPriv.ptr;
	struct sna *sna = video->sna;
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
	return Success;
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

	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_I915_OVERLAY_PUT_IMAGE, &request)) {
		DBG(("%s: Putimage failed\n", __FUNCTION__));
		return false;
	}

	if (video->bo[0] != frame->bo) {
		if (video->bo[0])
			kgem_bo_destroy(&sna->kgem, video->bo[0]);
		video->bo[0] = kgem_bo_reference(frame->bo);
	}

	return true;
}

static int
sna_video_overlay_put_image(ddPutImage_ARGS)
{
	struct sna_video *video = port->devPriv.ptr;
	struct sna *sna = video->sna;
	struct sna_video_frame frame;
	xf86CrtcPtr crtc;
	BoxRec dstBox;
	RegionRec clip;
	int ret;

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

	clip.extents.x1 = draw->x + drw_x;
	clip.extents.y1 = draw->y + drw_y;
	clip.extents.x2 = clip.extents.x1 + drw_w;
	clip.extents.y2 = clip.extents.y1 + drw_h;
	clip.data = NULL;

	DBG(("%s: always_on_top=%d\n", __FUNCTION__, video->AlwaysOnTop));
	if (!video->AlwaysOnTop)
		RegionIntersect(&clip, &clip, gc->pCompositeClip);
	if (box_empty(&clip.extents))
		goto invisible;

	DBG(("%s: src=(%d, %d),(%d, %d), dst=(%d, %d),(%d, %d), id=%d, sizep=%dx%d, sync?=%d\n",
	     __FUNCTION__,
	     src_x, src_y, src_w, src_h,
	     drw_x, drw_y, drw_w, drw_h,
	     format->id, width, height, sync));

	DBG(("%s: region %d:(%d, %d), (%d, %d)\n", __FUNCTION__,
	     region_num_rects(&clip),
	     clip.extents.x1, clip.extents.y1,
	     clip.extents.x2, clip.extents.y2));

	sna_video_frame_init(video, format->id, width, height, &frame);

	if (!sna_video_clip_helper(video, &frame, &crtc, &dstBox,
				   src_x, src_y, draw->x + drw_x, draw->y + drw_y,
				   src_w, src_h, drw_w, drw_h,
				   &clip))
		goto invisible;

	if (!crtc)
		goto invisible;

	/* overlay can't handle rotation natively, store it for the copy func */
	sna_video_frame_set_rotation(video, &frame, crtc->rotation);

	if (xvmc_passthrough(format->id)) {
		DBG(("%s: using passthough, name=%d\n",
		     __FUNCTION__, *(uint32_t *)buf));

		if (*(uint32_t*)buf == 0)
			goto invisible;

		frame.bo = kgem_create_for_name(&sna->kgem, *(uint32_t*)buf);
		if (frame.bo == NULL) {
			DBG(("%s: failed to open bo\n", __FUNCTION__));
			return BadAlloc;
		}

		if (kgem_bo_size(frame.bo) < frame.size) {
			DBG(("%s: bo size=%d, expected=%d\n",
			     __FUNCTION__, kgem_bo_size(frame.bo), frame.size));
			kgem_bo_destroy(&sna->kgem, frame.bo);
			return BadAlloc;
		}

		frame.image.x1 = 0;
		frame.image.y1 = 0;
		frame.image.x2 = frame.width;
		frame.image.y2 = frame.height;
	} else {
		frame.bo = sna_video_buffer(video, &frame);
		if (frame.bo == NULL) {
			DBG(("%s: failed to allocate video bo\n", __FUNCTION__));
			return BadAlloc;
		}

		if (!sna_video_copy_data(video, &frame, buf)) {
			DBG(("%s: failed to copy video data\n", __FUNCTION__));
			return BadAlloc;
		}
	}

	ret = Success;
	if (sna_video_overlay_show
	    (sna, video, &frame, crtc, &dstBox, src_w, src_h, drw_w, drw_h)) {
		//xf86XVFillKeyHelperDrawable(draw, video->color_key, &clip);
		if (!video->AlwaysOnTop && !RegionEqual(&video->clip, &clip) &&
		    sna_blt_fill_boxes(sna, GXcopy,
				       __sna_pixmap_get_bo(sna->front),
				       sna->front->drawable.bitsPerPixel,
				       video->color_key,
				       region_rects(&clip),
				       region_num_rects(&clip)))
			RegionCopy(&video->clip, &clip);
		sna_window_set_port((WindowPtr)draw, port);
	} else {
		DBG(("%s: failed to show video frame\n", __FUNCTION__));
		ret = BadAlloc;
	}

	frame.bo->domain = DOMAIN_NONE;
	if (xvmc_passthrough(format->id))
		kgem_bo_destroy(&sna->kgem, frame.bo);
	else
		sna_video_buffer_fini(video);

	return ret;

invisible:
	/*
	 * If the video isn't visible on any CRTC, turn it off
	 */
#if XORG_XV_VERSION < 2
	sna_video_overlay_stop(client, port, draw);
#else
	sna_video_overlay_stop(port, draw);
#endif
	return Success;
}

static int
sna_video_overlay_query(ddQueryImageAttributes_ARGS)
{
	struct sna_video *video = port->devPriv.ptr;
	struct sna_video_frame frame;
	struct sna *sna = video->sna;
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

	switch (format->id) {
	case FOURCC_XVMC:
		*h = (*h + 1) & ~1;
		sna_video_frame_init(video, format->id, *w, *h, &frame);
		sna_video_frame_set_rotation(video, &frame, RR_Rotate_0);
		size = sizeof(uint32_t);
		if (pitches) {
			pitches[0] = frame.pitch[1];
			pitches[1] = frame.pitch[0];
			pitches[2] = frame.pitch[0];
		}
		if (offsets) {
			offsets[1] = frame.UBufOffset;
			offsets[2] = frame.VBufOffset;
		}
		break;

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

void sna_video_overlay_setup(struct sna *sna, ScreenPtr screen)
{
	XvAdaptorPtr adaptor;
	struct sna_video *video;
	XvPortPtr port;

	if (sna->flags & SNA_IS_HOSTED)
		return;

	if (!sna_has_overlay(sna))
		return;

	DBG(("%s()\n", __FUNCTION__));

	adaptor = sna_xv_adaptor_alloc(sna);
	if (adaptor == NULL)
		return;

	video = calloc(1, sizeof(*video));
	port = calloc(1, sizeof(*port));
	if (video == NULL || port == NULL) {
		free(video);
		free(port);
		sna->xv.num_adaptors--;
		return;
	}

	adaptor->type = XvInputMask | XvImageMask;
	adaptor->pScreen = screen;
	adaptor->name = (char *)"Intel(R) Video Overlay";
	adaptor->nEncodings = 1;
	adaptor->pEncodings = xnfalloc(sizeof(XvEncodingRec));
	adaptor->pEncodings[0].id = 0;
	adaptor->pEncodings[0].pScreen = screen;
	adaptor->pEncodings[0].name = (char *)"XV_IMAGE";
	adaptor->pEncodings[0].width = sna->kgem.gen < 021 ? IMAGE_MAX_WIDTH_LEGACY : IMAGE_MAX_WIDTH;
	adaptor->pEncodings[0].height = sna->kgem.gen < 021 ? IMAGE_MAX_HEIGHT_LEGACY : IMAGE_MAX_HEIGHT;
	adaptor->pEncodings[0].rate.numerator = 1;
	adaptor->pEncodings[0].rate.denominator = 1;
	adaptor->pFormats = Formats;
	adaptor->nFormats = sna_xv_fixup_formats(screen, Formats,
						 ARRAY_SIZE(Formats));
	adaptor->nAttributes = NUM_ATTRIBUTES;
	if (HAS_GAMMA(sna))
		adaptor->nAttributes += GAMMA_ATTRIBUTES;
	adaptor->pAttributes = (XvAttributeRec *)Attributes;
	adaptor->nImages = ARRAY_SIZE(Images);
	adaptor->pImages = (XvImageRec *)Images;
#if XORG_XV_VERSION < 2
	adaptor->ddAllocatePort = sna_xv_alloc_port;
	adaptor->ddFreePort = sna_xv_free_port;
#endif
	adaptor->ddPutVideo = NULL;
	adaptor->ddPutStill = NULL;
	adaptor->ddGetVideo = NULL;
	adaptor->ddGetStill = NULL;
	adaptor->ddStopVideo = sna_video_overlay_stop;
	adaptor->ddSetPortAttribute = sna_video_overlay_set_attribute;
	adaptor->ddGetPortAttribute = sna_video_overlay_get_attribute;
	adaptor->ddQueryBestSize = sna_video_overlay_best_size;
	adaptor->ddPutImage = sna_video_overlay_put_image;
	adaptor->ddQueryImageAttributes = sna_video_overlay_query;

	adaptor->nPorts = 1;
	adaptor->pPorts = port;
	adaptor->base_id = port->id = FakeClientID(0);
	AddResource(port->id, XvGetRTPort(), port);

	port->pAdaptor = adaptor;
	port->pNotify =  NULL;
	port->pDraw =  NULL;
	port->client =  NULL;
	port->grab.client =  NULL;
	port->time = currentTime;
	port->devPriv.ptr = video;

	video->sna = sna;
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
	RegionNull(&video->clip);

	xvColorKey = MAKE_ATOM("XV_COLORKEY");
	xvBrightness = MAKE_ATOM("XV_BRIGHTNESS");
	xvContrast = MAKE_ATOM("XV_CONTRAST");
	xvSaturation = MAKE_ATOM("XV_SATURATION");

	/* Allow the pipe to be switched from pipe A to B when in clone mode */
	xvPipe = MAKE_ATOM("XV_PIPE");
	xvAlwaysOnTop = MAKE_ATOM("XV_ALWAYS_ON_TOP");

	if (HAS_GAMMA(sna)) {
		xvGamma0 = MAKE_ATOM("XV_GAMMA0");
		xvGamma1 = MAKE_ATOM("XV_GAMMA1");
		xvGamma2 = MAKE_ATOM("XV_GAMMA2");
		xvGamma3 = MAKE_ATOM("XV_GAMMA3");
		xvGamma4 = MAKE_ATOM("XV_GAMMA4");
		xvGamma5 = MAKE_ATOM("XV_GAMMA5");
	}

	sna_video_overlay_update_attrs(video);

	DBG(("%s: '%s' initialized %d ports\n", __FUNCTION__, adaptor->name, adaptor->nPorts));
}
