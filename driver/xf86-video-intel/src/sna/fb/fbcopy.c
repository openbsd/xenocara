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

#include <stdlib.h>

#include "fb.h"
#include <mi.h>

void
fbCopyNtoN(DrawablePtr src_drawable, DrawablePtr dst_drawable, GCPtr gc,
           BoxPtr box, int nbox,
           int dx, int dy,
	   Bool reverse, Bool upsidedown, Pixel bitplane,
	   void *closure)
{
	CARD8 alu = gc ? gc->alu : GXcopy;
	FbBits pm = gc ? fb_gc(gc)->pm : FB_ALLONES;
	FbBits *src, *dst;
	FbStride srcStride, dstStride;
	int dstBpp, srcBpp;
	int srcXoff, srcYoff;
	int dstXoff, dstYoff;

	fbGetDrawable(src_drawable, src, srcStride, srcBpp, srcXoff, srcYoff);
	fbGetDrawable(dst_drawable, dst, dstStride, dstBpp, dstXoff, dstYoff);

	src += (dy + srcYoff) * srcStride;
	srcXoff += dx;
	dst += dstYoff * dstStride;
	do {
		fbBlt(src + box->y1 * srcStride, srcStride,
		      (box->x1 + srcXoff) * srcBpp,
		      dst + box->y1 * dstStride, dstStride,
		      (box->x1 + dstXoff) * dstBpp,
		      (box->x2 - box->x1) * dstBpp,
		      (box->y2 - box->y1),
		      alu, pm, dstBpp, reverse, upsidedown);
	} while (box++, --nbox);
}

void
fbCopy1toN(DrawablePtr src_drawable, DrawablePtr dst_drawable, GCPtr gc,
           BoxPtr box, int nbox,
           int dx, int dy,
	   Bool reverse, Bool upsidedown, Pixel bitplane,
	   void *closure)
{
	FbGCPrivPtr pgc = fb_gc(gc);
	FbBits *src;
	FbStride srcStride;
	int srcBpp;
	int srcXoff, srcYoff;
	FbBits *dst;
	FbStride dstStride;
	int dstBpp;
	int dstXoff, dstYoff;

	fbGetDrawable(src_drawable, src, srcStride, srcBpp, srcXoff, srcYoff);
	fbGetDrawable(dst_drawable, dst, dstStride, dstBpp, dstXoff, dstYoff);

	while (nbox--) {
		if (dstBpp == 1) {
			fbBlt(src + (box->y1 + dy + srcYoff) * srcStride,
			      srcStride,
			      (box->x1 + dx + srcXoff) * srcBpp,
			      dst + (box->y1 + dstYoff) * dstStride,
			      dstStride,
			      (box->x1 + dstXoff) * dstBpp,
			      (box->x2 - box->x1) * dstBpp,
			      (box->y2 - box->y1),
			      FbOpaqueStipple1Rop(gc->alu,
						  gc->fgPixel, gc->bgPixel),
			      pgc->pm, dstBpp, reverse, upsidedown);
		} else {
			fbBltOne((FbStip *) (src + (box->y1 + dy + srcYoff) * srcStride),
				 srcStride * (FB_UNIT / FB_STIP_UNIT),
				 (box->x1 + dx + srcXoff),
				 dst + (box->y1 + dstYoff) * dstStride,
				 dstStride,
				 (box->x1 + dstXoff) * dstBpp,
				 dstBpp,
				 (box->x2 - box->x1) * dstBpp,
				 (box->y2 - box->y1),
				 pgc->and, pgc->xor, pgc->bgand, pgc->bgxor);
		}
		box++;
	}
}

void
fbCopyNto1(DrawablePtr src_drawable, DrawablePtr dst_drawable, GCPtr gc,
           BoxPtr box, int nbox,
           int dx, int dy,
	   Bool reverse, Bool upsidedown, Pixel bitplane, void *closure)
{
	FbGCPrivPtr pgc = fb_gc(gc);

	while (nbox--) {
		if (dst_drawable->bitsPerPixel == 1) {
			FbBits *src;
			FbStride srcStride;
			int srcBpp;
			int srcXoff, srcYoff;

			FbStip *dst;
			FbStride dstStride;
			int dstBpp;
			int dstXoff, dstYoff;

			fbGetDrawable(src_drawable, src,
				      srcStride, srcBpp, srcXoff, srcYoff);
			fbGetStipDrawable(dst_drawable,
					  dst, dstStride, dstBpp, dstXoff, dstYoff);
			fbBltPlane(src + (box->y1 + dy + srcYoff) * srcStride, srcStride,
				   (box->x1 + dx + srcXoff) * srcBpp, srcBpp,
				   dst + (box->y1 + dstYoff) * dstStride, dstStride,
				   (box->x1 + dstXoff) * dstBpp,
				   (box->x2 - box->x1) * srcBpp, (box->y2 - box->y1),
				   (FbStip) pgc->and, (FbStip) pgc->xor,
				   (FbStip) pgc->bgand, (FbStip) pgc->bgxor, bitplane);
		} else {
			FbBits *src;
			FbStride srcStride;
			int srcBpp;
			int srcXoff, srcYoff;

			FbBits *dst;
			FbStride dstStride;
			int dstBpp;
			int dstXoff, dstYoff;

			FbStip *tmp;
			FbStride tmpStride;
			int width, height;

			width = box->x2 - box->x1;
			height = box->y2 - box->y1;

			tmpStride = ((width + FB_STIP_MASK) >> FB_STIP_SHIFT);
			tmp = malloc(tmpStride * height * sizeof(FbStip));
			if (!tmp)
				return;

			fbGetDrawable(src_drawable, src,
				      srcStride, srcBpp, srcXoff, srcYoff);
			fbGetDrawable(dst_drawable, dst,
				      dstStride, dstBpp, dstXoff, dstYoff);

			fbBltPlane(src + (box->y1 + dy + srcYoff) * srcStride,
				   srcStride,
				   (box->x1 + dx + srcXoff) * srcBpp,
				   srcBpp,
				   tmp,
				   tmpStride,
				   0,
				   width * srcBpp,
				   height,
				   fbAndStip(GXcopy, FB_ALLONES, FB_ALLONES),
				   fbXorStip(GXcopy, FB_ALLONES, FB_ALLONES),
				   fbAndStip(GXcopy, 0, FB_ALLONES),
				   fbXorStip(GXcopy, 0, FB_ALLONES), bitplane);
			fbBltOne(tmp,
				 tmpStride,
				 0,
				 dst + (box->y1 + dstYoff) * dstStride,
				 dstStride,
				 (box->x1 + dstXoff) * dstBpp,
				 dstBpp,
				 width * dstBpp,
				 height,
				 pgc->and, pgc->xor, pgc->bgand, pgc->bgxor);
			free(tmp);
		}
		box++;
	}
}

RegionPtr
fbCopyArea(DrawablePtr src, DrawablePtr dst, GCPtr gc,
	   int sx, int sy,
	   int width, int height,
	   int dx, int dy)
{
	return miDoCopy(src, dst, gc, sx, sy, width, height, dx, dy,
			fbCopyNtoN, 0, 0);
}

RegionPtr
fbCopyPlane(DrawablePtr src, DrawablePtr dst, GCPtr gc,
	    int sx, int sy,
	    int width, int height,
	    int dx, int dy,
	    unsigned long bitplane)
{
	if (src->bitsPerPixel > 1)
		return miDoCopy(src, dst, gc, sx, sy, width, height, dx, dy,
				fbCopyNto1, (Pixel) bitplane, 0);
	else if (bitplane & 1)
		return miDoCopy(src, dst, gc, sx, sy, width, height, dx, dy,
				fbCopy1toN, (Pixel) bitplane, 0);
	else
		return miHandleExposures(src, dst, gc,
					 sx, sy, width, height, dx, dy,
					 bitplane);
}
