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

#include "intel_options.h"

#include <xf86drm.h>
#include <xf86xv.h>
#include <X11/extensions/Xv.h>
#include <fourcc.h>
#include <i915_drm.h>
#include <errno.h>

#ifdef  DRM_IOCTL_MODE_GETPLANERESOURCES
#include <drm_fourcc.h>

#define IMAGE_MAX_WIDTH		2048
#define IMAGE_MAX_HEIGHT	2048

#define MAKE_ATOM(a) MakeAtom(a, sizeof(a) - 1, true)

static Atom xvColorKey;

static XF86VideoFormatRec xv_formats[] = {
	{15, TrueColor}, {16, TrueColor}, {24, TrueColor}
};
static XF86ImageRec xv_images[] = { XVIMAGE_YUY2, XVIMAGE_UYVY, };
static const XF86VideoEncodingRec xv_dummy_encoding[] = {
	{ 0, "XV_IMAGE", IMAGE_MAX_WIDTH, IMAGE_MAX_HEIGHT, {1, 1} }
};
static XF86AttributeRec attribs[] = {
	{XvSettable | XvGettable, 0, 0xffffff, "XV_COLORKEY"},
};

static void sna_video_sprite_off(struct sna *sna, struct sna_video *video)
{
	struct drm_mode_set_plane s;

	if (video->plane == 0)
		return;

	memset(&s, 0, sizeof(s));
	s.plane_id = video->plane;
	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_SETPLANE, &s))
		xf86DrvMsg(sna->scrn->scrnIndex, X_ERROR,
			   "failed to disable plane\n");

	video->plane = 0;
}

static void sna_video_sprite_stop(ScrnInfoPtr scrn, pointer data, Bool shutdown)
{
	return sna_video_sprite_off(to_sna(scrn), data);
}

static int sna_video_sprite_set_attr(ScrnInfoPtr scrn,
				     Atom attribute, INT32 value,
				     pointer data)
{
	struct sna_video *video = data;

	if (attribute == xvColorKey) {
		video->color_key_changed = true;
		video->color_key = value;
		DBG(("COLORKEY = %d\n", value));
	} else
		return BadMatch;

	return Success;
}

static int sna_video_sprite_get_attr(ScrnInfoPtr scrn,
				     Atom attribute, INT32 *value,
				     pointer data)
{
	struct sna_video *video = data;

	if (attribute == xvColorKey)
		*value = video->color_key;
	else
		return BadMatch;

	return Success;
}

static void sna_video_sprite_best_size(ScrnInfoPtr scrn, Bool motion,
				       short vid_w, short vid_h,
				       short drw_w, short drw_h,
				       unsigned int *p_w, unsigned int *p_h,
				       pointer data)
{
	struct sna *sna = to_sna(scrn);

	if (sna->kgem.gen == 075) {
		*p_w = vid_w;
		*p_h = vid_h;
	} else {
		*p_w = drw_w;
		*p_h = drw_h;
	}
}

static void
update_dst_box_to_crtc_coords(struct sna *sna, xf86CrtcPtr crtc, BoxPtr dstBox)
{
	ScrnInfoPtr scrn = sna->scrn;
	int tmp;

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
}

static bool
sna_video_sprite_show(struct sna *sna,
		      struct sna_video *video,
		      struct sna_video_frame *frame,
		      xf86CrtcPtr crtc,
		      BoxPtr dstBox)
{
	struct drm_mode_set_plane s;

	VG_CLEAR(s);
	s.plane_id = sna_crtc_to_plane(crtc);

	update_dst_box_to_crtc_coords(sna, crtc, dstBox);
	if (crtc->rotation & (RR_Rotate_90 | RR_Rotate_270)) {
		int tmp = frame->width;
		frame->width = frame->height;
		frame->height = tmp;
	}

#if defined(DRM_I915_SET_SPRITE_DESTKEY)
	if (video->color_key_changed || video->plane != s.plane_id) {
		struct drm_intel_set_sprite_destkey set;

		DBG(("%s: updating color key: %x\n",
		     __FUNCTION__, video->color_key));

		set.plane_id = s.plane_id;
		set.value = video->color_key;

		if (drmIoctl(sna->kgem.fd,
			     DRM_IOCTL_I915_SET_SPRITE_DESTKEY,
			     &set))
			xf86DrvMsg(sna->scrn->scrnIndex, X_ERROR,
				   "failed to update color key\n");

		video->color_key_changed = false;
	}
#endif

	if (frame->bo->delta == 0) {
		uint32_t offsets[4], pitches[4], handles[4];
		uint32_t pixel_format;

		switch (frame->id) {
		case FOURCC_UYVY:
			pixel_format = DRM_FORMAT_UYVY;
			break;
		case FOURCC_YUY2:
		default:
			pixel_format = DRM_FORMAT_YUYV;
			break;
		}

		handles[0] = frame->bo->handle;
		pitches[0] = frame->pitch[0];
		offsets[0] = 0;

		DBG(("%s: creating new fb for handle=%d, width=%d, height=%d, stride=%d\n",
		     __FUNCTION__, frame->bo->handle,
		     frame->width, frame->height, frame->pitch[0]));

		if (drmModeAddFB2(sna->kgem.fd,
				  frame->width, frame->height, pixel_format,
				  handles, pitches, offsets,
				  &frame->bo->delta, 0)) {
			xf86DrvMsg(sna->scrn->scrnIndex,
				   X_ERROR, "failed to add fb\n");
			return false;
		}

		frame->bo->scanout = true;
	}

	assert(frame->bo->scanout);
	assert(frame->bo->delta);

	s.crtc_id = sna_crtc_id(crtc);
	s.fb_id = frame->bo->delta;
	s.flags = 0;
	s.crtc_x = dstBox->x1;
	s.crtc_y = dstBox->y1;
	s.crtc_w = dstBox->x2 - dstBox->x1;
	s.crtc_h = dstBox->y2 - dstBox->y1;
	s.src_x = 0;
	s.src_y = 0;
	s.src_w = (frame->image.x2 - frame->image.x1) << 16;
	s.src_h = (frame->image.y2 - frame->image.y1) << 16;

	DBG(("%s: updating crtc=%d, plane=%d, handle=%d [fb %d], dst=(%d,%d)x(%d,%d), src=(%d,%d)x(%d,%d)\n",
	     __FUNCTION__, s.crtc_id, s.plane_id, frame->bo->handle, s.fb_id,
	     s.crtc_x, s.crtc_y, s.crtc_w, s.crtc_h,
	     s.src_x >> 16, s.src_y >> 16, s.src_w >> 16, s.src_h >> 16));

	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_SETPLANE, &s)) {
		DBG(("SET_PLANE failed: ret=%d\n", errno));
		return false;
	}

	frame->bo->domain = DOMAIN_NONE;
	video->plane = s.plane_id;
	return true;
}

static int sna_video_sprite_put_image(ScrnInfoPtr scrn,
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
	xf86CrtcPtr crtc;
	BoxRec dst_box;

	sna_video_frame_init(sna, video, id, width, height, &frame);

	if (!sna_video_clip_helper(scrn, video, &frame, &crtc, &dst_box,
				   src_x, src_y, drw_x, drw_y,
				   src_w, src_h, drw_w, drw_h,
				   clip))
		return Success;

	if (!crtc || sna_crtc_to_plane(crtc) == 0) {
		/* If the video isn't visible on any CRTC, turn it off */
		sna_video_sprite_off(sna, video);
		return Success;
	}

	/* sprites can't handle rotation natively, store it for the copy func */
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

	if (!sna_video_sprite_show(sna, video, &frame, crtc, &dst_box)) {
		DBG(("%s: failed to show video frame\n", __FUNCTION__));
		return BadAlloc;
	}

	sna_video_buffer_fini(sna, video);

	if (!REGION_EQUAL(scrn->pScreen, &video->clip, clip)) {
		REGION_COPY(scrn->pScreen, &video->clip, clip);
		xf86XVFillKeyHelperDrawable(drawable, video->color_key, clip);
	}

	return Success;
}

static int sna_video_sprite_query_attrs(ScrnInfoPtr scrn, int id,
					unsigned short *w, unsigned short *h,
					int *pitches, int *offsets)
{
	int size;

	if (*w > IMAGE_MAX_WIDTH)
		*w = IMAGE_MAX_WIDTH;
	if (*h > IMAGE_MAX_HEIGHT)
		*h = IMAGE_MAX_HEIGHT;

	*w = (*w + 1) & ~1;
	if (offsets)
		offsets[0] = 0;

	switch (id) {
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

static int sna_video_sprite_color_key(struct sna *sna)
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

XF86VideoAdaptorPtr sna_video_sprite_setup(struct sna *sna,
					   ScreenPtr screen)
{
	XF86VideoAdaptorPtr adaptor;
	struct drm_mode_get_plane_res r;
	struct sna_video *video;

	memset(&r, 0, sizeof(struct drm_mode_get_plane_res));
	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_MODE_GETPLANERESOURCES, &r))
		return NULL;
	DBG(("%s: %d sprite planes\n", __FUNCTION__, r.count_planes));
	if (r.count_planes == 0)
		return NULL;

	adaptor = calloc(1,
			 sizeof(XF86VideoAdaptorRec) +
			 sizeof(struct sna_video) +
			 sizeof(DevUnion));
	if (!adaptor)
		return NULL;

	adaptor->type = XvWindowMask | XvInputMask | XvImageMask;
	adaptor->flags = VIDEO_OVERLAID_IMAGES /*| VIDEO_CLIP_TO_VIEWPORT */ ;
	adaptor->name = "Intel(R) Video Sprite";
	adaptor->nEncodings = ARRAY_SIZE(xv_dummy_encoding);
	adaptor->pEncodings = xnfalloc(sizeof(xv_dummy_encoding));
	memcpy(adaptor->pEncodings, xv_dummy_encoding, sizeof(xv_dummy_encoding));
	adaptor->nFormats = ARRAY_SIZE(xv_formats);
	adaptor->pFormats = xv_formats;
	adaptor->nPorts = 1;
	adaptor->pPortPrivates = (DevUnion *)&adaptor[1];

	video = (struct sna_video *)&adaptor->pPortPrivates[1];
	adaptor->pPortPrivates[0].ptr = video;

	adaptor->nAttributes = ARRAY_SIZE(attribs);
	adaptor->pAttributes = attribs;

	adaptor->nImages = ARRAY_SIZE(xv_images);
	adaptor->pImages = xv_images;

	adaptor->PutVideo = NULL;
	adaptor->PutStill = NULL;
	adaptor->GetVideo = NULL;
	adaptor->GetStill = NULL;
	adaptor->StopVideo = sna_video_sprite_stop;
	adaptor->SetPortAttribute = sna_video_sprite_set_attr;
	adaptor->GetPortAttribute = sna_video_sprite_get_attr;
	adaptor->QueryBestSize = sna_video_sprite_best_size;
	adaptor->PutImage = sna_video_sprite_put_image;
	adaptor->QueryImageAttributes = sna_video_sprite_query_attrs;

	video->alignment = 64;
	video->color_key = sna_video_sprite_color_key(sna);
	video->color_key_changed = true;
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

	REGION_NULL(screen, &video->clip);

	xvColorKey = MAKE_ATOM("XV_COLORKEY");

	return adaptor;
}
#else
XF86VideoAdaptorPtr sna_video_sprite_setup(struct sna *sna, ScreenPtr screen)
{
	return NULL;
}
#endif
