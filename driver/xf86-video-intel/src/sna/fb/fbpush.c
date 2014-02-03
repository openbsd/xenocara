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

#include "fb.h"
#include "fbclip.h"

static void
fbPushPattern(DrawablePtr drawable, GCPtr gc,
              FbStip *src, FbStride srcStride, int srcX,
	      int x, int y, int width, int height)
{
	FbStip *s, bitsMask, bitsMask0, bits;
	int xspan;
	int w;
	int lenspan;

	src += srcX >> FB_STIP_SHIFT;
	srcX &= FB_STIP_MASK;

	bitsMask0 = FbStipMask(srcX, 1);

	while (height--) {
		bitsMask = bitsMask0;
		w = width;
		s = src;
		src += srcStride;
		bits = READ(s++);
		xspan = x;
		while (w) {
			if (bits & bitsMask) {
				lenspan = 0;
				do {
					if (++lenspan == w)
						break;

					bitsMask = FbStipRight(bitsMask, 1);
					if (!bitsMask) {
						bits = READ(s++);
						bitsMask = FbBitsMask(0, 1);
					}
				} while (bits & bitsMask);
				fbFill(drawable, gc, xspan, y, lenspan, 1);
				xspan += lenspan;
				w -= lenspan;
			} else {
				do {
					xspan++;
					if (!--w)
						break;

					bitsMask = FbStipRight(bitsMask, 1);
					if (!bitsMask) {
						bits = READ(s++);
						bitsMask = FbBitsMask(0, 1);
					}
				} while (!(bits & bitsMask));
			}
		}
		y++;
	}
}

static void
fbPushFill(DrawablePtr drawable, GCPtr gc,
           FbStip *src, FbStride srcStride, int srcX,
	   int x, int y, int width, int height)
{
	FbGCPrivPtr pgc = fb_gc(gc);

	if (gc->fillStyle == FillSolid) {
		FbBits *dst;
		FbStride dstStride;
		int dstBpp;
		int dstXoff, dstYoff;
		int dstX;
		int dstWidth;

		fbGetDrawable(drawable, dst,
			      dstStride, dstBpp, dstXoff, dstYoff);
		dst = dst + (y + dstYoff) * dstStride;
		dstX = (x + dstXoff) * dstBpp;
		dstWidth = width * dstBpp;
		if (dstBpp == 1) {
			fbBltStip(src, srcStride, srcX,
				  (FbStip *)dst, dstStride, dstX,
				  dstWidth, height,
				  FbStipple1Rop(gc->alu, gc->fgPixel), pgc->pm, dstBpp);
		} else {
			fbBltOne(src, srcStride, srcX,
				 dst, dstStride, dstX, dstBpp,
				 dstWidth, height,
				 pgc->and, pgc->xor,
				 fbAnd(GXnoop, (FbBits) 0, FB_ALLONES),
				 fbXor(GXnoop, (FbBits) 0, FB_ALLONES));
		}
	} else
		fbPushPattern(drawable, gc, src, srcStride, srcX,
			      x, y, width, height);
}

struct fbPushImage {
	FbStip *src;
	FbStride stride;
	int x0, y0;
};

inline static void
_fbPushImage(DrawablePtr drawable, GCPtr gc, const BoxRec *b, void *_data)
{
	struct fbPushImage *data = _data;

	fbPushFill(drawable, gc,
		   data->src + (b->y1 - data->y0) * data->stride, data->stride,
		   b->x1 - data->x0,
		   b->x1, b->y1,
		   b->x2 - b->x1, b->y2 - b->y1);
}

void
fbPushImage(DrawablePtr drawable, GCPtr gc,
            FbStip *src, FbStride stride, int dx,
	    int x, int y, int width, int height)
{
	struct fbPushImage data;
	BoxRec box;

	DBG(("%s (%d, %d)x(%d, %d)\n", __FUNCTION__, x, y, width, height));

	data.src = src;
	data.stride = stride;
	data.y0 = y;
	data.x0 = x - dx;

	box.x1 = x;
	box.y1 = y;
	box.x2 = x + width;
	box.y2 = y + height;
	fbDrawableRun(drawable, gc, &box, _fbPushImage, &data);
}

void
fbPushPixels(GCPtr gc, PixmapPtr bitmap, DrawablePtr drawable,
	     int dx, int dy, int xOrg, int yOrg)
{
	FbStip *stip;
	FbStride stipStride;
	int stipBpp;
	_X_UNUSED int stipXoff, stipYoff;

	DBG(("%s bitmap=%x%d\n", __FUNCTION__,
	     bitmap->drawable.width, bitmap->drawable.height));

	fbGetStipDrawable(&bitmap->drawable, stip,
			  stipStride, stipBpp, stipXoff, stipYoff);

	fbPushImage(drawable, gc, stip, stipStride, 0, xOrg, yOrg, dx, dy);
}
