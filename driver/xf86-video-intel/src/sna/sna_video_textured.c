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

#include <xf86xv.h>
#include <X11/extensions/Xv.h>

#ifdef SNA_XVMC
#define _SNA_XVMC_SERVER_
#include "sna_video_hwmc.h"
#endif

#define MAKE_ATOM(a) MakeAtom(a, sizeof(a) - 1, true)

static Atom xvBrightness, xvContrast, xvSyncToVblank;

#define NUM_FORMATS 3
static const XF86VideoFormatRec Formats[NUM_FORMATS] = {
	{15, TrueColor}, {16, TrueColor}, {24, TrueColor}
};

//#define NUM_TEXTURED_ATTRIBUTES 3
#define NUM_TEXTURED_ATTRIBUTES 1
static const XF86AttributeRec TexturedAttributes[] = {
	{XvSettable | XvGettable, -1, 1, "XV_SYNC_TO_VBLANK"},
	{XvSettable | XvGettable, -128, 127, "XV_BRIGHTNESS"},
	{XvSettable | XvGettable, 0, 255, "XV_CONTRAST"},
};

#ifdef SNA_XVMC
#define NUM_IMAGES 5
#define XVMC_IMAGE 1
#else
#define NUM_IMAGES 4
#define XVMC_IMAGE 0
#endif

static const XF86ImageRec Images[NUM_IMAGES] = {
	XVIMAGE_YUY2,
	XVIMAGE_YV12,
	XVIMAGE_I420,
	XVIMAGE_UYVY,
#ifdef SNA_XVMC
	{
		/*
		 * Below, a dummy picture type that is used in XvPutImage
		 * only to do an overlay update.
		 * Introduced for the XvMC client lib.
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

static int xvmc_passthrough(int id)
{
#ifdef SNA_XVMC
	return id == FOURCC_XVMC;
#else
	return 0;
	(void)id;
#endif
}

static void sna_video_textured_stop(ScrnInfoPtr scrn,
				    pointer data,
				    Bool shutdown)
{
	struct sna *sna = to_sna(scrn);
	struct sna_video *video = data;

	DBG(("%s()\n", __FUNCTION__));

	REGION_EMPTY(scrn->pScreen, &video->clip);

	if (!shutdown)
		return;

	sna_video_free_buffers(sna, video);
}

static int
sna_video_textured_set_attribute(ScrnInfoPtr scrn,
				 Atom attribute,
				 INT32 value,
				 pointer data)
{
	struct sna_video *video = data;

	if (attribute == xvBrightness) {
		if (value < -128 || value > 127)
			return BadValue;

		video->brightness = value;
	} else if (attribute == xvContrast) {
		if (value < 0 || value > 255)
			return BadValue;

		video->contrast = value;
	} else if (attribute == xvSyncToVblank) {
		if (value < -1 || value > 1)
			return BadValue;

		video->SyncToVblank = value;
	} else
		return BadMatch;

	return Success;
}

static int
sna_video_textured_get_attribute(ScrnInfoPtr scrn,
				 Atom attribute,
				 INT32 *value,
				 pointer data)
{
	struct sna_video *video = data;

	if (attribute == xvBrightness)
		*value = video->brightness;
	else if (attribute == xvContrast)
		*value = video->contrast;
	else if (attribute == xvSyncToVblank)
		*value = video->SyncToVblank;
	else
		return BadMatch;

	return Success;
}

static void
sna_video_textured_best_size(ScrnInfoPtr scrn,
			     Bool motion,
			     short vid_w, short vid_h,
			     short drw_w, short drw_h,
			     unsigned int *p_w,
			     unsigned int *p_h,
			     pointer data)
{
	if (vid_w > (drw_w << 1))
		drw_w = vid_w >> 1;
	if (vid_h > (drw_h << 1))
		drw_h = vid_h >> 1;

	*p_w = drw_w;
	*p_h = drw_h;
}

/*
 * The source rectangle of the video is defined by (src_x, src_y, src_w, src_h).
 * The dest rectangle of the video is defined by (drw_x, drw_y, drw_w, drw_h).
 * id is a fourcc code for the format of the video.
 * buf is the pointer to the source data in system memory.
 * width and height are the w/h of the source data.
 * If "sync" is true, then we must be finished with *buf at the point of return
 * (which we always are).
 * clip is the clipping region in screen space.
 * data is a pointer to our port private.
 * drawable is some Drawable, which might not be the screen in the case of
 * compositing.  It's a new argument to the function in the 1.1 server.
 */
static int
sna_video_textured_put_image(ScrnInfoPtr scrn,
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
	PixmapPtr pixmap = get_drawable_pixmap(drawable);
	BoxRec dstBox;
	xf86CrtcPtr crtc;
	bool flush = false;
	bool ret;

	DBG(("%s: src=(%d, %d),(%d, %d), dst=(%d, %d),(%d, %d), id=%d, sizep=%dx%d, sync?=%d\n",
	     __FUNCTION__,
	     src_x, src_y, src_w, src_h,
	     drw_x, drw_y, drw_w, drw_h,
	     id, width, height, sync));

	if (buf == 0) {
		DBG(("%s: garbage video buffer\n", __FUNCTION__));
		return BadAlloc;
	}

	if (!sna_pixmap_move_to_gpu(pixmap, MOVE_READ | MOVE_WRITE)) {
		DBG(("%s: attempting to render to a non-GPU pixmap\n",
		     __FUNCTION__));
		return BadAlloc;
	}

	sna_video_frame_init(sna, video, id, width, height, &frame);

	if (!sna_video_clip_helper(scrn, video, &frame,
				   &crtc, &dstBox,
				   src_x, src_y, drw_x, drw_y,
				   src_w, src_h, drw_w, drw_h,
				   clip))
		return Success;

	if (xvmc_passthrough(id)) {
		DBG(("%s: using passthough, name=%d\n",
		     __FUNCTION__, *(uint32_t *)buf));

		if (sna->kgem.gen < 031) {
			/* XXX: i915 is not support and needs some
			 * serious care.  grep for KMS in i915_hwmc.c */
			return BadAlloc;
		}

		frame.bo = kgem_create_for_name(&sna->kgem, *(uint32_t*)buf);
		if (frame.bo == NULL) {
			DBG(("%s: failed to open bo\n", __FUNCTION__));
			return BadAlloc;
		}

		assert(kgem_bo_size(frame.bo) >= frame.size);
		frame.image.x1 = 0;
		frame.image.y1 = 0;
		frame.image.x2 = frame.width;
		frame.image.y2 = frame.height;
	} else {
		if (!sna_video_copy_data(sna, video, &frame, buf)) {
			DBG(("%s: failed to copy frame\n", __FUNCTION__));
			kgem_bo_destroy(&sna->kgem, frame.bo);
			return BadAlloc;
		}
	}

	if (crtc && video->SyncToVblank != 0 &&
	    sna_pixmap_is_scanout(sna, pixmap)) {
		kgem_set_mode(&sna->kgem, KGEM_RENDER, sna_pixmap(pixmap)->gpu_bo);
		flush = sna_wait_for_scanline(sna, pixmap, crtc,
					      &clip->extents);
	}

	ret = Success;
	if (!sna->render.video(sna, video, &frame, clip,
			       src_w, src_h, drw_w, drw_h,
			       drw_x - src_x, drw_y - src_y,
			       pixmap)) {
		DBG(("%s: failed to render video\n", __FUNCTION__));
		ret = BadAlloc;
	} else
		DamageDamageRegion(drawable, clip);

	kgem_bo_destroy(&sna->kgem, frame.bo);

	/* Push the frame to the GPU as soon as possible so
	 * we can hit the next vsync.
	 */
	if (flush)
		kgem_submit(&sna->kgem);

	return ret;
}

static int
sna_video_textured_query(ScrnInfoPtr scrn,
			 int id,
			 unsigned short *w, unsigned short *h,
			 int *pitches, int *offsets)
{
	int size, tmp;

	if (*w > 8192)
		*w = 8192;
	if (*h > 8192)
		*h = 8192;

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
		break;
	case FOURCC_UYVY:
	case FOURCC_YUY2:
	default:
		size = *w << 1;
		if (pitches)
			pitches[0] = size;
		size *= *h;
		break;
#ifdef SNA_XVMC
	case FOURCC_XVMC:
		*h = (*h + 1) & ~1;
		size = sizeof(uint32_t);
		if (pitches)
			pitches[0] = size;
		break;
#endif
	}

	return size;
}

XF86VideoAdaptorPtr sna_video_textured_setup(struct sna *sna,
					     ScreenPtr screen)
{
	XF86VideoAdaptorPtr adaptor;
	XF86AttributePtr attrs;
	struct sna_video *video;
	DevUnion *devUnions;
	int nports = 16, i;

	if (!sna->render.video) {
		xf86DrvMsg(sna->scrn->scrnIndex, X_WARNING,
			   "Textured video not supported on this hardware\n");
		return NULL;
	}

	if (wedged(sna)) {
		xf86DrvMsg(sna->scrn->scrnIndex, X_WARNING,
			   "cannot enable XVideo whilst the GPU is wedged\n");
		return NULL;
	}

	adaptor = calloc(1, sizeof(XF86VideoAdaptorRec));
	video = calloc(nports, sizeof(struct sna_video));
	devUnions = calloc(nports, sizeof(DevUnion));
#if NUM_TEXTURED_ATTRIBUTES
	attrs = calloc(NUM_TEXTURED_ATTRIBUTES, sizeof(XF86AttributeRec));
	if (adaptor == NULL ||
	    video == NULL ||
	    devUnions == NULL ||
	    attrs == NULL) {
		free(adaptor);
		free(video);
		free(devUnions);
		free(attrs);
		return NULL;
	}
#else
	if (adaptor == NULL || video == NULL || devUnions == NULL) {
		free(adaptor);
		free(video);
		free(devUnions);
		return NULL;
	}
	attrs = NULL;
#endif

	adaptor->type = XvWindowMask | XvInputMask | XvImageMask;
	adaptor->flags = 0;
	adaptor->name = "Intel(R) Textured Video";
	adaptor->nEncodings = 1;
	adaptor->pEncodings = xnfalloc(sizeof(XF86VideoEncodingRec));
	adaptor->pEncodings[0].id = 0;
	adaptor->pEncodings[0].name = "XV_IMAGE";
	adaptor->pEncodings[0].width = sna->render.max_3d_size;
	adaptor->pEncodings[0].height = sna->render.max_3d_size;
	adaptor->pEncodings[0].rate.numerator = 1;
	adaptor->pEncodings[0].rate.denominator = 1;
	adaptor->nFormats = NUM_FORMATS;
	adaptor->pFormats = (XF86VideoFormatPtr)Formats;
	adaptor->nPorts = nports;
	adaptor->pPortPrivates = devUnions;
	adaptor->nAttributes = NUM_TEXTURED_ATTRIBUTES;
	adaptor->pAttributes = attrs;
	memcpy(attrs, TexturedAttributes,
	       NUM_TEXTURED_ATTRIBUTES * sizeof(XF86AttributeRec));
	adaptor->nImages = NUM_IMAGES;
	adaptor->pImages = (XF86ImagePtr)Images;
	adaptor->PutVideo = NULL;
	adaptor->PutStill = NULL;
	adaptor->GetVideo = NULL;
	adaptor->GetStill = NULL;
	adaptor->StopVideo = sna_video_textured_stop;
	adaptor->SetPortAttribute = sna_video_textured_set_attribute;
	adaptor->GetPortAttribute = sna_video_textured_get_attribute;
	adaptor->QueryBestSize = sna_video_textured_best_size;
	adaptor->PutImage = sna_video_textured_put_image;
	adaptor->QueryImageAttributes = sna_video_textured_query;

	for (i = 0; i < nports; i++) {
		struct sna_video *v = &video[i];

		v->textured = true;
		v->alignment = 4;
		v->rotation = RR_Rotate_0;
		v->SyncToVblank = 1;

		/* gotta uninit this someplace, XXX: shouldn't be necessary for textured */
		RegionNull(&v->clip);

		adaptor->pPortPrivates[i].ptr = v;
	}

	xvBrightness = MAKE_ATOM("XV_BRIGHTNESS");
	xvContrast = MAKE_ATOM("XV_CONTRAST");
	xvSyncToVblank = MAKE_ATOM("XV_SYNC_TO_VBLANK");

	return adaptor;
}
