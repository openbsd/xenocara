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
#include <micoord.h>
#include <miline.h>
#include <scrnintstr.h>

#define POLYLINE    fbPolyline8
#define POLYSEGMENT fbPolySegment8
#define BITS	    BYTE
#define BITS2	    CARD16
#define BITS4	    CARD32
#include "fblinebits.h"
#undef BITS
#undef BITS2
#undef BITS4
#undef POLYSEGMENT
#undef POLYLINE

#define POLYLINE    fbPolyline16
#define POLYSEGMENT fbPolySegment16
#define BITS	    CARD16
#define BITS2	    CARD32
#include "fblinebits.h"
#undef BITS
#undef BITS2
#undef POLYSEGMENT
#undef POLYLINE

#define POLYLINE    fbPolyline32
#define POLYSEGMENT fbPolySegment32
#define BITS	    CARD32
#include "fblinebits.h"
#undef BITS
#undef POLYSEGMENT
#undef POLYLINE

static void
fbZeroLine(DrawablePtr drawable, GCPtr gc, int mode, int n, DDXPointPtr pt)
{
	int x1, y1, x2, y2;
	int x, y;
	int dashOffset;

	x = drawable->x;
	y = drawable->y;
	x1 = pt->x;
	y1 = pt->y;
	dashOffset = gc->dashOffset;
	while (--n) {
		++pt;
		x2 = pt->x;
		y2 = pt->y;
		if (mode == CoordModePrevious) {
			x2 += x1;
			y2 += y1;
		}
		fbSegment(drawable, gc,
			  x1 + x, y1 + y,
			  x2 + x, y2 + y,
			  n == 1 && gc->capStyle != CapNotLast, &dashOffset);
		x1 = x2;
		y1 = y2;
	}
}

static void
fbZeroSegment(DrawablePtr drawable, GCPtr gc, int n, xSegment *seg)
{
	int dashOffset;
	int16_t x, y;
	Bool drawLast = gc->capStyle != CapNotLast;

	x = drawable->x;
	y = drawable->y;
	while (n--) {
		dashOffset = gc->dashOffset;
		fbSegment(drawable, gc,
			  seg->x1 + x, seg->y1 + y,
			  seg->x2 + x, seg->y2 + y,
			  drawLast, &dashOffset);
		seg++;
	}
}

void
fbFixCoordModePrevious(int n, DDXPointPtr pt)
{
	int16_t x = pt->x;
	int16_t y = pt->y;
	while (--n) {
		pt++;
		x = (pt->x += x);
		y = (pt->y += y);
	}
}

void
fbPolyLine(DrawablePtr drawable, GCPtr gc, int mode, int n, DDXPointPtr pt)
{
	void (*raster)(DrawablePtr, GCPtr, int mode, int n, DDXPointPtr pt);

	DBG(("%s x %d, width=%d, fill=%d [solid? %d], line=%d [solid? %d], bpp=%d\n",
	     __FUNCTION__, n, gc->lineWidth,
	     gc->fillStyle, gc->fillStyle == FillSolid,
	     gc->lineStyle, gc->lineStyle == LineSolid,
	     drawable->bitsPerPixel));

	if (gc->lineWidth == 0) {
		raster = fbZeroLine;
		if (gc->fillStyle == FillSolid && gc->lineStyle == LineSolid) {
			switch (drawable->bitsPerPixel) {
			case 8:
				raster = fbPolyline8;
				break;
			case 16:
				raster = fbPolyline16;
				break;
			case 32:
				raster = fbPolyline32;
				break;
			}
		}
	} else {
		if (gc->lineStyle != LineSolid)
			raster = miWideDash;
		else
			raster = miWideLine;
	}
	raster(drawable, gc, mode, n, pt);
}

void
fbPolySegment(DrawablePtr drawable, GCPtr gc, int n, xSegment *seg)
{
	void (*raster)(DrawablePtr drawable, GCPtr gc, int n, xSegment * seg);

	DBG(("%s x %d, width=%d, fill=%d [solid? %d], line=%d [solid? %d], bpp=%d\n",
	     __FUNCTION__, n, gc->lineWidth,
	     gc->fillStyle, gc->fillStyle == FillSolid,
	     gc->lineStyle, gc->lineStyle == LineSolid,
	     drawable->bitsPerPixel));

	if (gc->lineWidth == 0) {
		raster = fbZeroSegment;
		if (gc->fillStyle == FillSolid && gc->lineStyle == LineSolid) {
			switch (drawable->bitsPerPixel) {
			case 8:
				raster = fbPolySegment8;
				break;
			case 16:
				raster = fbPolySegment16;
				break;
			case 32:
				raster = fbPolySegment32;
				break;
			}
		}
	} else
		raster = miPolySegment;

	raster(drawable, gc, n, seg);
}
