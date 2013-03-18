/*
 * Copyright © 1998 Keith Packard
 * Copyright © 2012 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Keith Packard not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Keith Packard makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <string.h>

#include "fb.h"
#include "fbclip.h"

struct fbPutZImage {
	FbStip *src, *dst;
	FbStride src_stride, dst_stride;

	int dst_x, dst_y;
	int x0, y0;
};

inline static void
_fbPutZImage(DrawablePtr drawable, GCPtr gc, const BoxRec *b, void *_data)
{
	struct fbPutZImage *data = _data;
	int bpp = drawable->bitsPerPixel;

	fbBltStip(data->src + (b->y1 - data->y0) * data->src_stride, data->src_stride,
		  (b->x1 - data->x0) * bpp,
		  data->dst + (b->y1 + data->dst_y) * data->dst_stride,
		  data->dst_stride,
		  (b->x1 + data->dst_x) * bpp,
		  (b->x2 - b->x1) * bpp, (b->y2 - b->y1),
		  gc->alu, fb_gc(gc)->pm, bpp);
}

static void
fbPutZImage(DrawablePtr drawable, GCPtr gc,
            int x, int y, int width, int height,
	    FbStip *src, FbStride srcStride)
{
	PixmapPtr pixmap;
	struct fbPutZImage data;
	BoxRec box;

	box.x1 = data.x0 = x;
	box.y1 = data.y0 = y;
	box.x2 = x + width;
	box.y2 = y + height;
	data.src = src;
	data.src_stride = srcStride;

	fbGetDrawablePixmap(drawable, pixmap, data.dst_x, data.dst_y);
	data.dst = pixmap->devPrivate.ptr;
	data.dst_stride = pixmap->devKind / sizeof(FbStip);

	fbDrawableRun(drawable, gc, &box, _fbPutZImage, &data);
}

struct fbPutXYImage {
	FbStip *src, *dst;
	FbStride src_stride, dst_stride;

	int dst_x, dst_y, src_x;
	int x0, y0;

	int alu, pm;
	FbBits fgand, fgxor, bgand, bgxor;
};

inline static void
_fbPutXYImage1(DrawablePtr drawable, GCPtr gc, const BoxRec *b, void *_data)
{
	struct fbPutXYImage *data = _data;
	int bpp = drawable->bitsPerPixel;

	fbBltStip(data->src + (b->y1 - data->y0) * data->src_stride, data->src_stride,
		  (b->x1 - data->x0) + data->src_x,
		  (FbStip *) (data->dst + (b->y1 + data->dst_y) * data->dst_stride),
		  data->dst_stride,
		  (b->x1 + data->dst_x) * bpp,
		  (b->x2 - b->x1) * bpp, (b->y2 - b->y1),
		  data->alu, data->pm, bpp);
}

inline static void
_fbPutXYImageN(DrawablePtr drawable, GCPtr gc, const BoxRec *b, void *_data)
{
	struct fbPutXYImage *data = _data;
	int bpp = drawable->bitsPerPixel;

	fbBltOne(data->src + (b->y1 - data->y0) * data->src_stride,
		 data->src_stride,
		 (b->x1 - data->x0) + data->src_x,
		 data->dst + (b->y1 + data->dst_y) * data->dst_stride,
		 data->dst_stride,
		 (b->x1 + data->dst_x) * bpp, bpp,
		 (b->x2 - b->x1) * bpp, (b->y2 - b->y1),
		 data->fgand, data->fgxor,
		 data->bgand, data->bgxor);
}

void
fbPutXYImage(DrawablePtr drawable, GCPtr gc,
             FbBits fg, FbBits bg, FbBits pm, int alu, Bool opaque,
             int x, int y, int width, int height,
	     FbStip *src, FbStride srcStride, int srcX)
{
	PixmapPtr pixmap;
	struct fbPutXYImage data;
	BoxRec box;

	box.x1 = data.x0 = x;
	box.y1 = data.y0 = y;
	box.x2 = x + width;
	box.y2 = y + height;
	data.src = src;
	data.src_stride = srcStride;
	data.src_x = srcX;

	fbGetDrawablePixmap(drawable, pixmap, data.dst_x, data.dst_y);
	data.dst = pixmap->devPrivate.ptr;
	data.dst_stride = pixmap->devKind / sizeof(FbStip);

	if (drawable->bitsPerPixel == 1) {
		if (opaque)
			data.alu = FbOpaqueStipple1Rop(alu, fg, bg);
		else
			data.alu = FbStipple1Rop(alu, fg);
		data.pm = pm;

		fbDrawableRun(drawable, gc, &box, _fbPutXYImage1, &data);
	} else {
		data.fgand = fbAnd(alu, fg, pm);
		data.fgxor = fbXor(alu, fg, pm);
		if (opaque) {
			data.bgand = fbAnd(alu, bg, pm);
			data.bgxor = fbXor(alu, bg, pm);
		} else {
			data.bgand = fbAnd(GXnoop, (FbBits) 0, FB_ALLONES);
			data.bgxor = fbXor(GXnoop, (FbBits) 0, FB_ALLONES);
		}

		fbDrawableRun(drawable, gc, &box, _fbPutXYImageN, &data);
	}
}

void
fbPutImage(DrawablePtr drawable, GCPtr gc, int depth,
           int x, int y, int w, int h,
	   int leftPad, int format, char *image)
{
	FbGCPrivPtr pgc = fb_gc(gc);
	unsigned long i;
	FbStride srcStride;
	FbStip *src = (FbStip *)image;

	DBG(("%s (%d, %d)x(%d, %d)\n", __FUNCTION__, x, y, w, h));

	x += drawable->x;
	y += drawable->y;

	switch (format) {
	case XYBitmap:
		srcStride = BitmapBytePad(w + leftPad) / sizeof(FbStip);
		fbPutXYImage(drawable, gc,
			     pgc->fg, pgc->bg, pgc->pm,
			     gc->alu, TRUE,
			     x, y, w, h,
			     src, srcStride, leftPad);
		break;
	case XYPixmap:
		srcStride = BitmapBytePad(w + leftPad) / sizeof(FbStip);
		for (i = (unsigned long) 1 << (drawable->depth - 1); i; i >>= 1) {
			if (i & gc->planemask) {
				fbPutXYImage(drawable, gc,
					     FB_ALLONES,
					     0,
					     fbReplicatePixel(i, drawable->bitsPerPixel),
					     gc->alu,
					     TRUE, x, y, w, h, src, srcStride, leftPad);
				src += srcStride * h;
			}
		}
		break;
	case ZPixmap:
		srcStride = PixmapBytePad(w, drawable->depth) / sizeof(FbStip);
		fbPutZImage(drawable, gc,
			    x, y, w, h, src, srcStride);
	}
}

void
fbGetImage(DrawablePtr drawable,
           int x, int y, int w, int h,
	   unsigned int format, unsigned long planeMask, char *d)
{
	FbBits *src;
	FbStride srcStride;
	int srcBpp;
	int srcXoff, srcYoff;
	FbStip *dst;
	FbStride dstStride;

	DBG(("%s (%d, %d)x(%d, %d)\n", __FUNCTION__, x, y, w, h));

	fbGetDrawable(drawable, src, srcStride, srcBpp, srcXoff, srcYoff);

	x += drawable->x;
	y += drawable->y;

	dst = (FbStip *) d;
	if (format == ZPixmap || srcBpp == 1) {
		FbBits pm;

		pm = fbReplicatePixel(planeMask, srcBpp);
		dstStride = PixmapBytePad(w, drawable->depth);
		if (pm != FB_ALLONES)
			memset(d, 0, dstStride * h);
		dstStride /= sizeof(FbStip);
		fbBltStip((FbStip *)(src + (y + srcYoff) * srcStride), srcStride,
			  (x + srcXoff) * srcBpp,
			  dst, dstStride, 0, w * srcBpp, h, GXcopy, pm, srcBpp);
	} else {
		dstStride = BitmapBytePad(w) / sizeof(FbStip);
		fbBltPlane(src + (y + srcYoff) * srcStride,
			   srcStride,
			   (x + srcXoff) * srcBpp,
			   srcBpp,
			   dst,
			   dstStride,
			   0,
			   w * srcBpp, h,
			   fbAndStip(GXcopy, FB_STIP_ALLONES, FB_STIP_ALLONES),
			   fbXorStip(GXcopy, FB_STIP_ALLONES, FB_STIP_ALLONES),
			   fbAndStip(GXcopy, 0, FB_STIP_ALLONES),
			   fbXorStip(GXcopy, 0, FB_STIP_ALLONES), planeMask);
	}
}
