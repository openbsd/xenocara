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
fbSolid(FbBits * dst,
        FbStride dstStride,
        int dstX, int bpp, int width, int height, FbBits and, FbBits xor)
{
	FbBits startmask, endmask;
	int n, nmiddle;
	int startbyte, endbyte;

	dst += dstX >> FB_SHIFT;
	dstX &= FB_MASK;
	FbMaskBitsBytes(dstX, width, and == 0, startmask, startbyte,
			nmiddle, endmask, endbyte);
	if (startmask)
		dstStride--;
	dstStride -= nmiddle;
	while (height--) {
		if (startmask) {
			FbDoLeftMaskByteRRop(dst, startbyte, startmask, and, xor);
			dst++;
		}
		n = nmiddle;
		if (!and)
			while (n--)
				WRITE(dst++, xor);
		else
			while (n--) {
				WRITE(dst, FbDoRRop(READ(dst), and, xor));
				dst++;
			}
		if (endmask)
			FbDoRightMaskByteRRop(dst, endbyte, endmask, and, xor);
		dst += dstStride;
	}
}

void
fbFill(DrawablePtr drawable, GCPtr gc, int x, int y, int width, int height)
{
	FbBits *dst;
	FbStride dstStride;
	int dstBpp;
	int dstXoff, dstYoff;
	FbGCPrivPtr pgc = fb_gc(gc);

	DBG(("%s (%d, %d)x(%d, %d), style=%d\n",
	     __FUNCTION__, x, y, width, height, gc->fillStyle));

	fbGetDrawable(drawable, dst, dstStride, dstBpp, dstXoff, dstYoff);

	switch (gc->fillStyle) {
	case FillSolid:
		if (pgc->and ||
		    !pixman_fill((uint32_t *) dst, dstStride, dstBpp,
				 x + dstXoff, y + dstYoff,
				 width, height, pgc->xor))
			fbSolid(dst + (y + dstYoff) * dstStride,
				dstStride,
				(x + dstXoff) * dstBpp,
				dstBpp, width * dstBpp, height, pgc->and, pgc->xor);
		break;

	case FillStippled:
	case FillOpaqueStippled:
		{
			PixmapPtr pStip = gc->stipple;
			int stipWidth = pStip->drawable.width;
			int stipHeight = pStip->drawable.height;

			if (dstBpp == 1) {
				int alu;
				FbBits *stip;
				FbStride stipStride;
				int stipBpp;
				_X_UNUSED int stipXoff, stipYoff;

				if (gc->fillStyle == FillStippled)
					alu = FbStipple1Rop(gc->alu, gc->fgPixel);
				else
					alu = FbOpaqueStipple1Rop(gc->alu, gc->fgPixel, gc->bgPixel);
				fbGetDrawable(&pStip->drawable, stip, stipStride, stipBpp, stipXoff,
					      stipYoff);
				fbTile(dst + (y + dstYoff) * dstStride, dstStride, x + dstXoff,
				       width, height, stip, stipStride, stipWidth, stipHeight, alu,
				       pgc->pm, dstBpp, (gc->patOrg.x + drawable->x + dstXoff),
				       gc->patOrg.y + drawable->y - y);
			} else {
				FbStip *stip;
				FbStride stipStride;
				int stipBpp;
				_X_UNUSED int stipXoff, stipYoff;
				FbBits fgand, fgxor, bgand, bgxor;

				fgand = pgc->and;
				fgxor = pgc->xor;
				if (gc->fillStyle == FillStippled) {
					bgand = fbAnd(GXnoop, (FbBits) 0, FB_ALLONES);
					bgxor = fbXor(GXnoop, (FbBits) 0, FB_ALLONES);
				} else {
					bgand = pgc->bgand;
					bgxor = pgc->bgxor;
				}

				fbGetStipDrawable(&pStip->drawable, stip, stipStride, stipBpp,
						  stipXoff, stipYoff);
				fbStipple(dst + (y + dstYoff) * dstStride, dstStride,
					  (x + dstXoff) * dstBpp, dstBpp, width * dstBpp, height,
					  stip, stipStride, stipWidth, stipHeight,
					  pgc->evenStipple, fgand, fgxor, bgand, bgxor,
					  gc->patOrg.x + drawable->x + dstXoff,
					  gc->patOrg.y + drawable->y - y);
			}
			break;
		}

	case FillTiled:
		{
			PixmapPtr tile = gc->tile.pixmap;

			fbTile(dst + (y + dstYoff) * dstStride, dstStride,
			       (x + dstXoff) * dstBpp, width * dstBpp, height,
			       tile->devPrivate.ptr, tile->devKind / sizeof(FbBits),
			       tile->drawable.width * tile->drawable.bitsPerPixel,
			       tile->drawable.height,
			       gc->alu, pgc->pm, dstBpp,
			       (gc->patOrg.x + drawable->x + dstXoff) * dstBpp,
			       gc->patOrg.y + drawable->y - y);
			break;
		}
	}
}

static void
_fbSolidBox(DrawablePtr drawable, GCPtr gc, const BoxRec *b, void *_data)
{
	FbBits *dst;
	FbStride stride;
	int dx, dy, bpp;
	FbBits and = fbAnd(GXcopy, fb_gc(gc)->bg, fb_gc(gc)->pm);
	FbBits xor = fbXor(GXcopy, fb_gc(gc)->bg, fb_gc(gc)->pm);

	fbGetDrawable(drawable, dst, stride, bpp, dx, dy);

	if (and ||
	    !pixman_fill((uint32_t *) dst, stride, bpp,
			 b->x1 + dx, b->y1 + dy,
			 (b->x2 - b->x1), (b->y2 - b->y1), xor))
		fbSolid(dst + (b->y1 + dy) * stride, stride,
			(b->x1 + dx) * bpp, bpp,
			(b->x2 - b->x1) * bpp, (b->y2 - b->y1),
			and, xor);
}

void
fbSolidBoxClipped(DrawablePtr drawable, GCPtr gc,
                  int x1, int y1, int x2, int y2)
{
	BoxRec box;

	box.x1 = x1;
	box.y1 = y1;
	box.x2 = x2;
	box.y2 = y2;

	fbDrawableRun(drawable, gc, &box, _fbSolidBox, NULL);
}

inline static void
fbFillBox(DrawablePtr drawable, GCPtr gc, const BoxRec *box, void *data)
{
	DBG(("%s box=(%d, %d), (%d, %d)\n", __FUNCTION__,
	     box->x1, box->y1, box->x2, box->y2));
	fbFill(drawable, gc,
	       box->x1, box->y1,
	       box->x2 - box->x1, box->y2 - box->y1);
}

void
fbPolyFillRect(DrawablePtr drawable, GCPtr gc, int n, xRectangle *r)
{
	DBG(("%s x %d\n", __FUNCTION__, n));
	while (n--) {
		BoxRec b;

		b.x1 = r->x + drawable->x;
		b.y1 = r->y + drawable->y;
		b.x2 = fbBound(b.x1, r->width);
		b.y2 = fbBound(b.y1, r->height);
		r++;

		DBG(("%s: rectangle (%d, %d), (%d, %d)\n",
		     __FUNCTION__, b.x1, b.y1, b.x2, b.y2));
		fbDrawableRun(drawable, gc, &b, fbFillBox, NULL);
	}
}
