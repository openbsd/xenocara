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
#include "fbclip.h"
#include <mi.h>
#include <miline.h>
#include <scrnintstr.h>

#define FbDashDeclare	\
    unsigned char	*__dash, *__firstDash, *__lastDash

#define FbDashInit(gc,pgc,dashOffset,dashlen,even) {	    \
    (even) = TRUE;					    \
    __firstDash = (gc)->dash;				    \
    __lastDash = __firstDash + (gc)->numInDashList;	    \
    (dashOffset) %= (pgc)->dashLength;		    \
							    \
    __dash = __firstDash;				    \
    while ((dashOffset) >= ((dashlen) = *__dash)) {	    \
	(dashOffset) -= (dashlen);			    \
	(even) = 1-(even);				    \
	if (++__dash == __lastDash)			    \
	    __dash = __firstDash;			    \
    }							    \
    (dashlen) -= (dashOffset);				    \
}

#define FbDashNext(dashlen) {				    \
    if (++__dash == __lastDash)				    \
	__dash = __firstDash;				    \
    (dashlen) = *__dash;				    \
}

/* as numInDashList is always even, this case can skip a test */

#define FbDashNextEven(dashlen) {			    \
    (dashlen) = *++__dash;				    \
}

#define FbDashNextOdd(dashlen)	FbDashNext(dashlen)

#define FbDashStep(dashlen,even) {			    \
    if (!--(dashlen)) {					    \
	FbDashNext(dashlen);				    \
	(even) = 1-(even);				    \
    }							    \
}

#define fbBresShiftMask(mask,dir,bpp) ((bpp == FB_STIP_UNIT) ? 0 : \
					((dir < 0) ? FbStipLeft(mask,bpp) : \
					 FbStipRight(mask,bpp)))

typedef void FbBres(DrawablePtr drawable,
                    GCPtr gc,
                    int dashOffset,
                    int sdx,
                    int sdy,
                    int axis, int x, int y, int e, int e1, int e3, int len);

#define BRESSOLID   fbBresSolid8
#define BRESSOLIDR  fbBresSolidR8
#define BRESDASH    fbBresDash8
#define BITS	    BYTE
#define BITS2	    CARD16
#define BITS4	    CARD32
#include "fbsegbits.h"
#undef BRESSOLID
#undef BRESSOLIDR
#undef BRESDASH
#undef BITS
#undef BITS2
#undef BITS4

#define BRESSOLID   fbBresSolid16
#define BRESSOLIDR  fbBresSolidR16
#define BRESDASH    fbBresDash16
#define BITS	    CARD16
#define BITS2	    CARD32
#include "fbsegbits.h"
#undef BRESSOLID
#undef BRESSOLIDR
#undef BRESDASH
#undef BITS
#undef BITS2

#define BRESSOLID   fbBresSolid32
#define BRESSOLIDR  fbBresSolidR32
#define BRESDASH    fbBresDash32
#define BITS	    CARD32
#include "fbsegbits.h"
#undef BRESSOLID
#undef BRESSOLIDR
#undef BRESDASH
#undef BITS

static void
fbBresSolid(DrawablePtr drawable, GCPtr gc, int dashOffset,
            int sdx, int sdy, int axis,
	    int x1, int y1,
	    int e, int e1, int e3, int len)
{
	FbStip *dst;
	FbStride stride;
	int bpp;
	int dx, dy;
	FbGCPrivPtr pgc = fb_gc(gc);
	FbStip and = (FbStip) pgc->and;
	FbStip xor = (FbStip) pgc->xor;
	FbStip mask, mask0;
	FbStip bits;

	fbGetStipDrawable(drawable, dst, stride, bpp, dx, dy);
	dst += ((y1 + dy) * stride);
	x1 = (x1 + dx) * bpp;
	dst += x1 >> FB_STIP_SHIFT;
	x1 &= FB_STIP_MASK;
	mask0 = FbStipMask(0, bpp);
	mask = FbStipRight(mask0, x1);
	if (sdx < 0)
		mask0 = FbStipRight(mask0, FB_STIP_UNIT - bpp);
	if (sdy < 0)
		stride = -stride;
	if (axis == X_AXIS) {
		bits = 0;
		while (len--) {
			bits |= mask;
			mask = fbBresShiftMask(mask, sdx, bpp);
			if (!mask) {
				WRITE(dst, FbDoMaskRRop(READ(dst), and, xor, bits));
				bits = 0;
				dst += sdx;
				mask = mask0;
			}
			e += e1;
			if (e >= 0) {
				WRITE(dst, FbDoMaskRRop(READ(dst), and, xor, bits));
				bits = 0;
				dst += stride;
				e += e3;
			}
		}
		if (bits)
			WRITE(dst, FbDoMaskRRop(READ(dst), and, xor, bits));
	} else {
		while (len--) {
			WRITE(dst, FbDoMaskRRop(READ(dst), and, xor, mask));
			dst += stride;
			e += e1;
			if (e >= 0) {
				e += e3;
				mask = fbBresShiftMask(mask, sdx, bpp);
				if (!mask) {
					dst += sdx;
					mask = mask0;
				}
			}
		}
	}
}

static void
fbBresDash(DrawablePtr drawable, GCPtr gc, int dashOffset,
           int sdx, int sdy, int axis,
	   int x1, int y1,
	   int e, int e1, int e3, int len)
{
	FbStip *dst;
	FbStride stride;
	int bpp;
	int dx, dy;
	FbGCPrivPtr pgc = fb_gc(gc);
	FbStip and = (FbStip) pgc->and;
	FbStip xor = (FbStip) pgc->xor;
	FbStip bgand = (FbStip) pgc->bgand;
	FbStip bgxor = (FbStip) pgc->bgxor;
	FbStip mask, mask0;

	FbDashDeclare;
	int dashlen;
	bool even;
	bool doOdd;

	fbGetStipDrawable(drawable, dst, stride, bpp, dx, dy);
	doOdd = gc->lineStyle == LineDoubleDash;

	FbDashInit(gc, pgc, dashOffset, dashlen, even);

	dst += ((y1 + dy) * stride);
	x1 = (x1 + dx) * bpp;
	dst += x1 >> FB_STIP_SHIFT;
	x1 &= FB_STIP_MASK;
	mask0 = FbStipMask(0, bpp);
	mask = FbStipRight(mask0, x1);
	if (sdx < 0)
		mask0 = FbStipRight(mask0, FB_STIP_UNIT - bpp);
	if (sdy < 0)
		stride = -stride;
	while (len--) {
		if (even)
			WRITE(dst, FbDoMaskRRop(READ(dst), and, xor, mask));
		else if (doOdd)
			WRITE(dst, FbDoMaskRRop(READ(dst), bgand, bgxor, mask));
		if (axis == X_AXIS) {
			mask = fbBresShiftMask(mask, sdx, bpp);
			if (!mask) {
				dst += sdx;
				mask = mask0;
			}
			e += e1;
			if (e >= 0) {
				dst += stride;
				e += e3;
			}
		} else {
			dst += stride;
			e += e1;
			if (e >= 0) {
				e += e3;
				mask = fbBresShiftMask(mask, sdx, bpp);
				if (!mask) {
					dst += sdx;
					mask = mask0;
				}
			}
		}
		FbDashStep(dashlen, even);
	}
}

static void
fbBresFill(DrawablePtr drawable, GCPtr gc, int dashOffset,
           int sdx, int sdy, int axis,
	   int x1, int y1,
	   int e, int e1, int e3, int len)
{
	while (len--) {
		fbFill(drawable, gc, x1, y1, 1, 1);
		if (axis == X_AXIS) {
			x1 += sdx;
			e += e1;
			if (e >= 0) {
				e += e3;
				y1 += sdy;
			}
		} else {
			y1 += sdy;
			e += e1;
			if (e >= 0) {
				e += e3;
				x1 += sdx;
			}
		}
	}
}

static void
fbSetFg(DrawablePtr drawable, GCPtr gc, Pixel fg)
{
	if (fg != gc->fgPixel) {
		gc->fgPixel = fg;
		fbValidateGC(gc, GCForeground, drawable);
	}
}

static void
fbBresFillDash(DrawablePtr drawable,
               GCPtr gc,
               int dashOffset,
               int sdx,
               int sdy,
               int axis, int x1, int y1, int e, int e1, int e3, int len)
{
	FbGCPrivPtr pgc = fb_gc(gc);

	FbDashDeclare;
	int dashlen;
	bool even;
	bool doOdd;
	bool doBg;
	Pixel fg, bg;

	fg = gc->fgPixel;
	bg = gc->bgPixel;

	/* whether to fill the odd dashes */
	doOdd = gc->lineStyle == LineDoubleDash;
	/* whether to switch fg to bg when filling odd dashes */
	doBg = doOdd && (gc->fillStyle == FillSolid ||
			 gc->fillStyle == FillStippled);

	/* compute current dash position */
	FbDashInit(gc, pgc, dashOffset, dashlen, even);

	while (len--) {
		if (even || doOdd) {
			if (doBg) {
				if (even)
					fbSetFg(drawable, gc, fg);
				else
					fbSetFg(drawable, gc, bg);
			}
			fbFill(drawable, gc, x1, y1, 1, 1);
		}
		if (axis == X_AXIS) {
			x1 += sdx;
			e += e1;
			if (e >= 0) {
				e += e3;
				y1 += sdy;
			}
		} else {
			y1 += sdy;
			e += e1;
			if (e >= 0) {
				e += e3;
				x1 += sdx;
			}
		}
		FbDashStep(dashlen, even);
	}
	if (doBg)
		fbSetFg(drawable, gc, fg);
}

static FbBres *
fbSelectBres(DrawablePtr drawable, GCPtr gc)
{
	FbGCPrivPtr pgc = fb_gc(gc);
	int bpp = drawable->bitsPerPixel;
	FbBres *bres;

	DBG(("%s: line=%d, fill=%d, and=%lx, bgand=%lx\n",
	     __FUNCTION__, gc->lineStyle, gc->fillStyle, pgc->and, pgc->bgand));
	assert(gc->lineWidth == 0);

	if (gc->lineStyle == LineSolid) {
		bres = fbBresFill;
		if (gc->fillStyle == FillSolid) {
			bres = fbBresSolid;
			if (pgc->and == 0) {
				switch (bpp) {
				case 8:
					bres = fbBresSolid8;
					break;
				case 16:
					bres = fbBresSolid16;
					break;
				case 32:
					bres = fbBresSolid32;
					break;
				}
			} else {
				switch (bpp) {
				case 8:
					bres = fbBresSolidR8;
					break;
				case 16:
					bres = fbBresSolidR16;
					break;
				case 32:
					bres = fbBresSolidR32;
					break;
				}
			}
		}
	} else {
		bres = fbBresFillDash;
		if (gc->fillStyle == FillSolid) {
			bres = fbBresDash;
			if (pgc->and == 0 &&
			    (gc->lineStyle == LineOnOffDash || pgc->bgand == 0)) {
				switch (bpp) {
				case 8:
					bres = fbBresDash8;
					break;
				case 16:
					bres = fbBresDash16;
					break;
				case 32:
					bres = fbBresDash32;
					break;
				}
			}
		}
	}
	return bres;
}

struct fbSegment {
	FbBres *bres;
	bool drawLast;
	int *dashOffset;
	int x1, y1, x2, y2;
};

static void
_fbSegment(DrawablePtr drawable, GCPtr gc, const BoxRec *b, void *_data)
{
	struct fbSegment *data = _data;
	const unsigned int bias = miGetZeroLineBias(drawable->pScreen);
	int adx, ady;               /* abs values of dx and dy */
	int sdx, sdy;               /* sign of dx and dy */
	int e, e1, e2, e3;          /* bresenham error and increments */
	int len, axis, octant;
	int dashoff, doff;
	unsigned int oc1, oc2;

	DBG(("%s box=(%d, %d),(%d, %d)\n",
	     __FUNCTION__, b->x1, b->y1, b->x2, b->y2));

	CalcLineDeltas(data->x1, data->y1, data->x2, data->y2,
		       adx, ady, sdx, sdy, 1, 1, octant);

	if (adx > ady) {
		axis = X_AXIS;
		e1 = ady << 1;
		e2 = e1 - (adx << 1);
		e = e1 - adx;
		len = adx;
	} else {
		axis = Y_AXIS;
		e1 = adx << 1;
		e2 = e1 - (ady << 1);
		e = e1 - ady;
		SetYMajorOctant(octant);
		len = ady;
	}

	FIXUP_ERROR(e, octant, bias);

	/*
	 * Adjust error terms to compare against zero
	 */
	e3 = e2 - e1;
	e = e - e1;

	if (data->drawLast)
		len++;
	dashoff = *data->dashOffset;
	*data->dashOffset = dashoff + len;

	oc1 = 0;
	oc2 = 0;
	OUTCODES(oc1, data->x1, data->y1, b);
	OUTCODES(oc2, data->x2, data->y2, b);
	if ((oc1 | oc2) == 0) {
		data->bres(drawable, gc, dashoff,
			   sdx, sdy, axis, data->x1, data->y1, e, e1, e3, len);
	} else if (oc1 & oc2) {
	} else {
		int new_x1 = data->x1, new_y1 = data->y1;
		int new_x2 = data->x2, new_y2 = data->y2;
		int clip1 = 0, clip2 = 0;
		int clipdx, clipdy;
		int err;

		if (miZeroClipLine(b->x1, b->y1, b->x2-1, b->y2-1,
				   &new_x1, &new_y1, &new_x2, &new_y2,
				   adx, ady, &clip1, &clip2,
				   octant, bias, oc1, oc2) == -1)
			return;

		if (axis == X_AXIS)
			len = abs(new_x2 - new_x1);
		else
			len = abs(new_y2 - new_y1);
		if (clip2 != 0 || data->drawLast)
			len++;
		if (len) {
			/* unwind bresenham error term to first point */
			doff = dashoff;
			err = e;
			if (clip1) {
				clipdx = abs(new_x1 - data->x1);
				clipdy = abs(new_y1 - data->y1);
				if (axis == X_AXIS) {
					doff += clipdx;
					err += e3 * clipdy + e1 * clipdx;
				} else {
					doff += clipdy;
					err += e3 * clipdx + e1 * clipdy;
				}
			}
			data->bres(drawable, gc, doff,
				   sdx, sdy, axis, new_x1, new_y1,
				   err, e1, e3, len);
		}
	}
}

void
fbSegment(DrawablePtr drawable, GCPtr gc,
          int x1, int y1, int x2, int y2,
	  bool drawLast, int *dashOffset)
{
	struct fbSegment data;
	BoxRec box;

	DBG(("%s (%d, %d), (%d, %d), drawLast?=%d\n",
	     __FUNCTION__, x1, y1, x2, y2, drawLast));

	/* simple overestimate of line extents for clipping */
	box.x1 = x1 - 1;
	box.y1 = y1 - 1;
	box.x2 = x2 + 1;
	box.y2 = y2 + 1;

	data.x1 = x1;
	data.y1 = y1;
	data.x2 = x2;
	data.y2 = y2;

	data.dashOffset = dashOffset;
	data.drawLast = drawLast;
	data.bres = fbSelectBres(drawable, gc);

	fbDrawableRunUnclipped(drawable, gc, &box, _fbSegment, &data);
}

void
fbSegment1(DrawablePtr drawable, GCPtr gc, const BoxRec *b,
	   int x1, int y1, int x2, int y2,
	   bool drawLast, int *dashOffset)
{
	struct fbSegment data;

	DBG(("%s (%d, %d), (%d, %d), drawLast?=%d\n",
	     __FUNCTION__, x1, y1, x2, y2, drawLast));

	data.x1 = x1;
	data.y1 = y1;
	data.x2 = x2;
	data.y2 = y2;

	data.dashOffset = dashOffset;
	data.drawLast = drawLast;
	data.bres = fbSelectBres(drawable, gc);

	_fbSegment(drawable, gc, b, &data);
}
