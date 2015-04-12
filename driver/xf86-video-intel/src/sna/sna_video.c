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
 *	Jonathan Bian <jonathan.bian@intel.com>
 *      Offscreen Images:
 *        Matt Sottek <matthew.j.sottek@intel.com>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include <sys/mman.h>

#include "sna.h"
#include "sna_reg.h"
#include "sna_video.h"

#include "intel_options.h"

#include <xf86xv.h>

#ifdef SNA_XVMC
#define _SNA_XVMC_SERVER_
#include "sna_video_hwmc.h"
#else
static inline void sna_video_xvmc_setup(struct sna *sna, ScreenPtr ptr)
{
	DBG(("%s: XvMC not compiled in\n", __FUNCTION__));
}
#endif

void sna_video_free_buffers(struct sna_video *video)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(video->old_buf); i++) {
		if (video->old_buf[i]) {
			kgem_bo_destroy(&video->sna->kgem, video->old_buf[i]);
			video->old_buf[i] = NULL;
		}
	}

	if (video->buf) {
		kgem_bo_destroy(&video->sna->kgem, video->buf);
		video->buf = NULL;
	}
}

struct kgem_bo *
sna_video_buffer(struct sna_video *video,
		 struct sna_video_frame *frame)
{
	/* Free the current buffer if we're going to have to reallocate */
	if (video->buf && __kgem_bo_size(video->buf) < frame->size)
		sna_video_free_buffers(video);

	if (video->buf && video->buf->scanout) {
		if (frame->width != video->width ||
		    frame->height != video->height ||
		    frame->id != video->format)
			sna_video_free_buffers(video);
	}

	if (video->buf == NULL) {
		if (video->tiled) {
			video->buf = kgem_create_2d(&video->sna->kgem,
						    frame->width, frame->height, 32,
						    I915_TILING_X, CREATE_EXACT);
		} else {
			video->buf = kgem_create_linear(&video->sna->kgem, frame->size,
							CREATE_GTT_MAP);
		}
	}

	video->width  = frame->width;
	video->height = frame->height;
	video->format = frame->id;

	return video->buf;
}

void sna_video_buffer_fini(struct sna_video *video)
{
	struct kgem_bo *bo;

	bo = video->old_buf[1];
	video->old_buf[1] = video->old_buf[0];
	video->old_buf[0] = video->buf;
	video->buf = bo;
}

bool
sna_video_clip_helper(struct sna_video *video,
		      struct sna_video_frame *frame,
		      xf86CrtcPtr *crtc_ret,
		      BoxPtr dst,
		      short src_x, short src_y,
		      short drw_x, short drw_y,
		      short src_w, short src_h,
		      short drw_w, short drw_h,
		      RegionPtr reg)
{
	bool ret;
	RegionRec crtc_region_local;
	RegionPtr crtc_region = reg;
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
	crtc = sna_covering_crtc(video->sna, dst, video->desired_crtc);

	/* For textured video, we don't actually want to clip at all. */
	if (crtc && !video->textured) {
		crtc_region_local.extents = crtc->bounds;
		crtc_region_local.data = NULL;
		crtc_region = &crtc_region_local;
		RegionIntersect(crtc_region, crtc_region, reg);
	}
	*crtc_ret = crtc;

	ret = xf86XVClipVideoHelper(dst, &x1, &x2, &y1, &y2,
				    crtc_region, frame->width, frame->height);
	if (crtc_region != reg)
		RegionUninit(crtc_region);

	frame->src.x1 = x1 >> 16;
	frame->src.y1 = y1 >> 16;
	frame->src.x2 = (x2 + 0xffff) >> 16;
	frame->src.y2 = (y2 + 0xffff) >> 16;

	frame->image.x1 = frame->src.x1 & ~1;
	frame->image.x2 = ALIGN(frame->src.x2, 2);
	if (is_planar_fourcc(frame->id)) {
		frame->image.y1 = frame->src.y1 & ~1;
		frame->image.y2 = ALIGN(frame->src.y2, 2);
	} else {
		frame->image.y1 = frame->src.y1;
		frame->image.y2 = frame->src.y2;
	}

	return ret;
}

void
sna_video_frame_init(struct sna_video *video,
		     int id, short width, short height,
		     struct sna_video_frame *frame)
{
	DBG(("%s: id=%d [planar? %d], width=%d, height=%d, align=%d\n",
	     __FUNCTION__, id, is_planar_fourcc(id), width, height, video->alignment));
	assert(width && height);

	frame->bo = NULL;
	frame->id = id;
	frame->width = width;
	frame->height = height;
	frame->rotation = 0;
}

void
sna_video_frame_set_rotation(struct sna_video *video,
			     struct sna_video_frame *frame,
			     Rotation rotation)
{
	unsigned width = frame->width;
	unsigned height = frame->height;
	unsigned align;

	DBG(("%s: rotation=%d\n", __FUNCTION__, rotation));
	frame->rotation = rotation;

	align = video->alignment;
#if SNA_XVMC
	/* for i915 xvmc, hw requires 1kb aligned surfaces */
	if (frame->id == FOURCC_XVMC && video->sna->kgem.gen < 040 && align < 1024)
		align = 1024;
#endif

	/* Determine the desired destination pitch (representing the
	 * chroma's pitch in the planar case).
	 */
	if (is_planar_fourcc(frame->id)) {
		assert((width & 1) == 0);
		assert((height & 1) == 0);
		if (rotation & (RR_Rotate_90 | RR_Rotate_270)) {
			frame->pitch[0] = ALIGN((height / 2), align);
			frame->pitch[1] = ALIGN(height, align);
			frame->size = width;
		} else {
			frame->pitch[0] = ALIGN((width / 2), align);
			frame->pitch[1] = ALIGN(width, align);
			frame->size = height;
		}
		frame->size *= frame->pitch[0] + frame->pitch[1];

		if (rotation & (RR_Rotate_90 | RR_Rotate_270)) {
			frame->UBufOffset = (int)frame->pitch[1] * width;
			frame->VBufOffset =
				frame->UBufOffset + (int)frame->pitch[0] * width / 2;
		} else {
			frame->UBufOffset = (int)frame->pitch[1] * height;
			frame->VBufOffset =
				frame->UBufOffset + (int)frame->pitch[0] * height / 2;
		}
	} else {
		switch (frame->id) {
		case FOURCC_RGB888:
			if (rotation & (RR_Rotate_90 | RR_Rotate_270)) {
				frame->pitch[0] = ALIGN((height << 2), align);
				frame->size = (int)frame->pitch[0] * width;
			} else {
				frame->pitch[0] = ALIGN((width << 2), align);
				frame->size = (int)frame->pitch[0] * height;
			}
			frame->UBufOffset = frame->VBufOffset = 0;
			break;
		case FOURCC_RGB565:
			if (rotation & (RR_Rotate_90 | RR_Rotate_270)) {
				frame->pitch[0] = ALIGN((height << 1), align);
				frame->size = (int)frame->pitch[0] * width;
			} else {
				frame->pitch[0] = ALIGN((width << 1), align);
				frame->size = (int)frame->pitch[0] * height;
			}
			frame->UBufOffset = frame->VBufOffset = 0;
			break;

		default:
			if (rotation & (RR_Rotate_90 | RR_Rotate_270)) {
				frame->pitch[0] = ALIGN((height << 1), align);
				frame->size = (int)frame->pitch[0] * width;
			} else {
				frame->pitch[0] = ALIGN((width << 1), align);
				frame->size = (int)frame->pitch[0] * height;
			}
			break;
		}
		frame->pitch[1] = 0;
		frame->UBufOffset = 0;
		frame->VBufOffset = 0;
	}

	assert(frame->size);
}

static void sna_memcpy_plane(struct sna_video *video,
			     uint8_t *dst, const uint8_t *src,
			     const struct sna_video_frame *frame, int sub)
{
	int dstPitch = frame->pitch[!sub], srcPitch;
	const uint8_t *s;
	int i, j = 0;
	int x, y, w, h;

	x = frame->image.x1;
	y = frame->image.y1;
	w = frame->image.x2 - frame->image.x1;
	h = frame->image.y2 - frame->image.y1;
	if (sub) {
		x >>= 1; w >>= 1;
		y >>= 1; h >>= 1;
		srcPitch = ALIGN((frame->width >> 1), 4);
	} else
		srcPitch = ALIGN(frame->width, 4);

	src += y * srcPitch + x;
	if (!video->textured)
		x = y = 0;

	switch (frame->rotation) {
	case RR_Rotate_0:
		dst += y * dstPitch + x;
		if (srcPitch == dstPitch && srcPitch == w)
			memcpy(dst, src, srcPitch * h);
		else while (h--) {
			memcpy(dst, src, w);
			src += srcPitch;
			dst += dstPitch;
		}
		break;
	case RR_Rotate_90:
		for (i = 0; i < h; i++) {
			s = src;
			for (j = 0; j < w; j++)
				dst[i + ((x + w - j - 1) * dstPitch)] = *s++;
			src += srcPitch;
		}
		break;
	case RR_Rotate_180:
		for (i = 0; i < h; i++) {
			s = src;
			for (j = 0; j < w; j++) {
				dst[(x + w - j - 1) +
				    ((h - i - 1) * dstPitch)] = *s++;
			}
			src += srcPitch;
		}
		break;
	case RR_Rotate_270:
		for (i = 0; i < h; i++) {
			s = src;
			for (j = 0; j < w; j++) {
				dst[(h - i - 1) + (x + j * dstPitch)] = *s++;
			}
			src += srcPitch;
		}
		break;
	}
}

static void
sna_copy_planar_data(struct sna_video *video,
		     const struct sna_video_frame *frame,
		     const uint8_t *src, uint8_t *dst)
{
	uint8_t *d;

	sna_memcpy_plane(video, dst, src, frame, 0);
	src += frame->height * ALIGN(frame->width, 4);

	if (frame->id == FOURCC_I420)
		d = dst + frame->UBufOffset;
	else
		d = dst + frame->VBufOffset;
	sna_memcpy_plane(video, d, src, frame, 1);
	src += (frame->height >> 1) * ALIGN(frame->width >> 1, 4);

	if (frame->id == FOURCC_I420)
		d = dst + frame->VBufOffset;
	else
		d = dst + frame->UBufOffset;
	sna_memcpy_plane(video, d, src, frame, 1);
}

static void
sna_copy_packed_data(struct sna_video *video,
		     const struct sna_video_frame *frame,
		     const uint8_t *buf,
		     uint8_t *dst)
{
	int pitch = frame->width << 1;
	const uint8_t *src, *s;
	int x, y, w, h;
	int i, j;

	if (video->textured) {
		/* XXX support copying cropped extents */
		x = y = 0;
		w = frame->width;
		h = frame->height;
	} else {
		x = frame->image.x1;
		y = frame->image.y1;
		w = frame->image.x2 - frame->image.x1;
		h = frame->image.y2 - frame->image.y1;
	}

	src = buf + (y * pitch) + (x << 1);

	switch (frame->rotation) {
	case RR_Rotate_0:
		w <<= 1;
		for (i = 0; i < h; i++) {
			memcpy(dst, src, w);
			src += pitch;
			dst += frame->pitch[0];
		}
		break;
	case RR_Rotate_90:
		h <<= 1;
		for (i = 0; i < h; i += 2) {
			s = src;
			for (j = 0; j < w; j++) {
				/* Copy Y */
				dst[(i + 0) + ((w - j - 1) * frame->pitch[0])] = *s;
				s += 2;
			}
			src += pitch;
		}
		h >>= 1;
		src = buf + (y * pitch) + (x << 1);
		for (i = 0; i < h; i += 2) {
			for (j = 0; j < w; j += 2) {
				/* Copy U */
				dst[((i * 2) + 1) + ((w - j - 1) * frame->pitch[0])] = src[(j * 2) + 1 + (i * pitch)];
				dst[((i * 2) + 1) + ((w - j - 2) * frame->pitch[0])] = src[(j * 2) + 1 + ((i + 1) * pitch)];
				/* Copy V */ dst[((i * 2) + 3) + ((w - j - 1) * frame->pitch[0])] = src[(j * 2) + 3 + (i * pitch)];
				dst[((i * 2) + 3) + ((w - j - 2) * frame->pitch[0])] = src[(j * 2) + 3 + ((i + 1) * pitch)];
			}
		}
		break;
	case RR_Rotate_180:
		w <<= 1;
		for (i = 0; i < h; i++) {
			s = src;
			for (j = 0; j < w; j += 4) {
				dst[(w - j - 4) + ((h - i - 1) * frame->pitch[0])] = *s++;
				dst[(w - j - 3) + ((h - i - 1) * frame->pitch[0])] = *s++;
				dst[(w - j - 2) + ((h - i - 1) * frame->pitch[0])] = *s++;
				dst[(w - j - 1) + ((h - i - 1) * frame->pitch[0])] = *s++;
			}
			src += pitch;
		}
		break;
	case RR_Rotate_270:
		h <<= 1;
		for (i = 0; i < h; i += 2) {
			s = src;
			for (j = 0; j < w; j++) {
				/* Copy Y */
				dst[(h - i - 2) + (j * frame->pitch[0])] = *s;
				s += 2;
			}
			src += pitch;
		}
		h >>= 1;
		src = buf + (y * pitch) + (x << 1);
		for (i = 0; i < h; i += 2) {
			for (j = 0; j < w; j += 2) {
				/* Copy U */
				dst[(((h - i) * 2) - 3) + (j * frame->pitch[0])] = src[(j * 2) + 1 + (i * pitch)];
				dst[(((h - i) * 2) - 3) + ((j + 1) * frame->pitch[0])] = src[(j * 2) + 1 + ((i + 1) * pitch)];
				/* Copy V */
				dst[(((h - i) * 2) - 1) + (j * frame->pitch[0])] = src[(j * 2) + 3 + (i * pitch)];
				dst[(((h - i) * 2) - 1) + ((j + 1) * frame->pitch[0])] = src[(j * 2) + 3 + ((i + 1) * pitch)];
			}
		}
		break;
	}
}

bool
sna_video_copy_data(struct sna_video *video,
		    struct sna_video_frame *frame,
		    const uint8_t *buf)
{
	uint8_t *dst;

	DBG(("%s: handle=%d, size=%dx%d [%d], pitch=[%d,%d] rotation=%d, is-texture=%d\n",
	     __FUNCTION__, frame->bo ? frame->bo->handle : 0,
	     frame->width, frame->height, frame->size, frame->pitch[0], frame->pitch[1],
	     frame->rotation, video->textured));
	DBG(("%s: image=(%d, %d), (%d, %d), source=(%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     frame->image.x1, frame->image.y1, frame->image.x2, frame->image.y2,
	     frame->src.x1, frame->src.y1, frame->src.x2, frame->src.y2));
	assert(frame->width && frame->height);
	assert(frame->rotation);
	assert(frame->size);

	/* In the common case, we can simply the upload in a single pwrite */
	if (frame->rotation == RR_Rotate_0 && !video->tiled) {
		DBG(("%s: unrotated, untiled fast paths: is-planar?=%d\n",
		     __FUNCTION__, is_planar_fourcc(frame->id)));
		if (is_planar_fourcc(frame->id)) {
			int w = frame->image.x2 - frame->image.x1;
			int h = frame->image.y2 - frame->image.y1;
			if (ALIGN(h, 2) == frame->height &&
			    ALIGN(w >> 1, 4) == frame->pitch[0] &&
			    ALIGN(w, 4) == frame->pitch[1]) {
				if (frame->bo) {
					if (!kgem_bo_write(&video->sna->kgem, frame->bo,
							   buf, frame->size))
						goto use_gtt;
				} else {
					frame->bo = kgem_create_buffer(&video->sna->kgem, frame->size,
								       KGEM_BUFFER_WRITE | KGEM_BUFFER_WRITE_INPLACE,
								       (void **)&dst);
					if (frame->bo == NULL)
						return false;

					memcpy(dst, buf, frame->size);
				}
				if (frame->id != FOURCC_I420) {
					uint32_t tmp;
					tmp = frame->VBufOffset;
					frame->VBufOffset = frame->UBufOffset;
					frame->UBufOffset = tmp;
				}
				return true;
			}
		} else {
			int x, y, w, h;

			if (video->textured) {
				/* XXX support copying cropped extents */
				x = y = 0;
				w = frame->width;
				h = frame->height;
			} else {
				x = frame->image.x1;
				y = frame->image.y1;
				w = frame->image.x2 - frame->image.x1;
				h = frame->image.y2 - frame->image.y1;
			}

			if (w*2 == frame->pitch[0]) {
				buf += (2U*y * frame->width) + (x << 1);
				if (frame->bo) {
					if (!kgem_bo_write(&video->sna->kgem, frame->bo,
							   buf, 2U*h*frame->width))
						goto use_gtt;
				} else {
					frame->bo = kgem_create_buffer(&video->sna->kgem, frame->size,
								       KGEM_BUFFER_WRITE | KGEM_BUFFER_WRITE_INPLACE,
								       (void **)&dst);
					if (frame->bo == NULL)
						return false;

					memcpy(dst, buf, 2U*h*frame->width);
				}
				return true;
			}
		}

		DBG(("%s: source cropped, fallback\n", __FUNCTION__));
	}

use_gtt: /* copy data, must use GTT so that we keep the overlay uncached */
	if (frame->bo) {
		dst = kgem_bo_map__gtt(&video->sna->kgem, frame->bo);
		if (dst == NULL)
			return false;
	} else {
		frame->bo = kgem_create_buffer(&video->sna->kgem, frame->size,
					       KGEM_BUFFER_WRITE | KGEM_BUFFER_WRITE_INPLACE,
					       (void **)&dst);
		if (frame->bo == NULL)
			return false;
	}

	if (is_planar_fourcc(frame->id))
		sna_copy_planar_data(video, frame, buf, dst);
	else
		sna_copy_packed_data(video, frame, buf, dst);

	return true;
}

XvAdaptorPtr sna_xv_adaptor_alloc(struct sna *sna)
{
	XvAdaptorPtr new_adaptors;

	new_adaptors = realloc(sna->xv.adaptors,
			       (sna->xv.num_adaptors+1)*sizeof(XvAdaptorRec));
	if (new_adaptors == NULL)
		return NULL;

	if (sna->xv.num_adaptors && new_adaptors != sna->xv.adaptors) {
		XvAdaptorPtr adaptor = new_adaptors;
		int i = sna->xv.num_adaptors, j;
		while (i--) {
			for (j = 0; j < adaptor->nPorts; j++)
				adaptor->pPorts[j].pAdaptor = adaptor;
			adaptor++;
		}
	}

	sna->xv.adaptors = new_adaptors;
	return &sna->xv.adaptors[sna->xv.num_adaptors++];
}

int
sna_xv_alloc_port(unsigned long port, XvPortPtr in, XvPortPtr *out)
{
	*out = in;
	return Success;
}

int
sna_xv_free_port(XvPortPtr port)
{
	return Success;
}

int
sna_xv_fixup_formats(ScreenPtr screen, XvFormatPtr formats, int num_formats)
{
	XvFormatPtr out = formats;
	int count = 0;

	while (num_formats--) {
		int num_visuals = screen->numVisuals;
		VisualPtr v = screen->visuals;

		while (num_visuals--) {
			if (v->class == TrueColor &&
			    v->nplanes == formats->depth) {
				int tmp = out[count].depth;
				out[count].depth = formats->depth;
				out[count].visual = v->vid;
				formats->depth = tmp;
				count++;
				break;
			}
			v++;
		}

		formats++;
	}

	return count;
}

#if XORG_XV_VERSION < 2
static int
sna_xv_query_adaptors(ScreenPtr screen,
		      XvAdaptorPtr *adaptors,
		      int *num_adaptors)
{
	struct sna *sna = to_sna_from_screen(screen);

	*num_adaptors = sna->xv.num_adaptors;
	*adaptors = sna->xv.adaptors;
	return Success;
}

static Bool
sna_xv_close_screen(CLOSE_SCREEN_ARGS_DECL)
{
	struct sna *sna = to_sna_from_screen(screen);
	sna_video_close(sna);
	return TRUE;
}
#endif

void sna_video_init(struct sna *sna, ScreenPtr screen)
{
	XvScreenPtr xv;

	if (noXvExtension)
		return;

	if (xf86LoaderCheckSymbol("xf86XVListGenericAdaptors")) {
		XF86VideoAdaptorPtr *adaptors = NULL;
		int num_adaptors = xf86XVListGenericAdaptors(sna->scrn, &adaptors);
		if (num_adaptors)
			xf86DrvMsg(sna->scrn->scrnIndex, X_ERROR,
				   "Ignoring generic xf86XV adaptors");
		free(adaptors);
	}

	if (XvScreenInit(screen) != Success)
		return;

	xv = to_xv(screen);
#if XORG_XV_VERSION < 2
	xv->ddCloseScreen = sna_xv_close_screen;
	xv->ddQueryAdaptors = sna_xv_query_adaptors;
#endif

	sna_video_textured_setup(sna, screen);
	sna_video_sprite_setup(sna, screen);
	sna_video_overlay_setup(sna, screen);

	if (sna->xv.num_adaptors >= 2 &&
	    xf86ReturnOptValBool(sna->Options, OPTION_PREFER_OVERLAY, false)) {
		XvAdaptorRec tmp;

		tmp = sna->xv.adaptors[0];
		sna->xv.adaptors[0] = sna->xv.adaptors[1];
		sna->xv.adaptors[1] = tmp;
	}

	xv->nAdaptors = sna->xv.num_adaptors;
	xv->pAdaptors = sna->xv.adaptors;

	sna_video_xvmc_setup(sna, screen);
}

void sna_video_destroy_window(WindowPtr win)
{
	XvPortPtr port;

	port = sna_window_get_port(win);
	if (port) {
#if XORG_XV_VERSION < 2
		port->pAdaptor->ddStopVideo(NULL, port, &win->drawable);
#else
		port->pAdaptor->ddStopVideo(port, &win->drawable);
#endif
	}
	assert(sna_window_get_port(win) == NULL);
}

void sna_video_close(struct sna *sna)
{
	int i;

	for (i = 0; i < sna->xv.num_adaptors; i++) {
		free(sna->xv.adaptors[i].pPorts->devPriv.ptr);
		free(sna->xv.adaptors[i].pPorts);
		free(sna->xv.adaptors[i].pEncodings);
	}
	free(sna->xv.adaptors);

	sna->xv.adaptors = NULL;
	sna->xv.num_adaptors = 0;
}
