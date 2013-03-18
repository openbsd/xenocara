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
#include <mi.h>
#include <mizerarc.h>
#include <limits.h>

#define ARC	    fbArc8
#define BITS	    BYTE
#define BITS2	    CARD16
#define BITS4	    CARD32
#include "fbarcbits.h"
#undef BITS
#undef BITS2
#undef BITS4
#undef ARC

#define ARC	    fbArc16
#define BITS	    CARD16
#define BITS2	    CARD32
#include "fbarcbits.h"
#undef BITS
#undef BITS2
#undef ARC

#define ARC	    fbArc32
#define BITS	    CARD32
#include "fbarcbits.h"
#undef BITS
#undef ARC

void
fbPolyArc(DrawablePtr drawable, GCPtr gc, int n, xArc *arc)
{
	DBG(("%s x %d, width=%d, fill=%d, line=%d\n",
	     __FUNCTION__, n, gc->lineWidth, gc->lineStyle, gc->fillStyle));

	if (gc->lineWidth == 0) {
		void (*raster)(FbBits *dst, FbStride dstStride, int dstBpp,
			       xArc *arc, int dx, int dy,
			       FbBits and, FbBits xor);

		raster = 0;
		if (gc->lineStyle == LineSolid && gc->fillStyle == FillSolid) {
			switch (drawable->bitsPerPixel) {
			case 8:
				raster = fbArc8;
				break;
			case 16:
				raster = fbArc16;
				break;
			case 32:
				raster = fbArc32;
				break;
			}
		}
		if (raster) {
			FbGCPrivPtr pgc = fb_gc(gc);
			FbBits *dst;
			FbStride dstStride;
			int dstBpp;
			int dstXoff, dstYoff;
			BoxRec box;
			int x2, y2;

			fbGetDrawable(drawable, dst, dstStride, dstBpp, dstXoff, dstYoff);
			while (n--) {
				if (miCanZeroArc(arc)) {
					box.x1 = arc->x + drawable->x;
					box.y1 = arc->y + drawable->y;
					/*
					 * Because box.x2 and box.y2 get truncated to 16 bits, and the
					 * RECT_IN_REGION test treats the resulting number as a signed
					 * integer, the RECT_IN_REGION test alone can go the wrong way.
					 * This can result in a server crash because the rendering
					 * routines in this file deal directly with cpu addresses
					 * of pixels to be stored, and do not clip or otherwise check
					 * that all such addresses are within their respective pixmaps.
					 * So we only allow the RECT_IN_REGION test to be used for
					 * values that can be expressed correctly in a signed short.
					 */
					x2 = box.x1 + (int) arc->width + 1;
					box.x2 = x2;
					y2 = box.y1 + (int) arc->height + 1;
					box.y2 = y2;
					if ((x2 <= SHRT_MAX) && (y2 <= SHRT_MAX) &&
					    (RegionContainsRect(gc->pCompositeClip, &box) == rgnIN)) {
						raster(dst, dstStride, dstBpp,
						       arc, drawable->x + dstXoff,
							drawable->y + dstYoff, pgc->and, pgc->xor);
					} else
						miZeroPolyArc(drawable, gc, 1, arc);
				} else
					miPolyArc(drawable, gc, 1, arc);
				arc++;
			}
		} else
			miZeroPolyArc(drawable, gc, n, arc);
	} else
		miPolyArc(drawable, gc, n, arc);
}
