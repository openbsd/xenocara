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
#include <gcstruct.h>
#include <scrnintstr.h>

/*
 * Pad pixmap to FB_UNIT bits wide
 */
void
fbPadPixmap(PixmapPtr pPixmap)
{
	int width;
	FbBits *bits;
	FbBits b;
	FbBits mask;
	int height;
	int w;
	int stride;
	int bpp;
	_X_UNUSED int xOff, yOff;

	fbGetDrawable(&pPixmap->drawable, bits, stride, bpp, xOff, yOff);

	width = pPixmap->drawable.width * pPixmap->drawable.bitsPerPixel;
	height = pPixmap->drawable.height;
	mask = FbBitsMask(0, width);
	while (height--) {
		b = READ(bits) & mask;
		w = width;
		while (w < FB_UNIT) {
			b = b | FbScrRight(b, w);
			w <<= 1;
		}
		WRITE(bits, b);
		bits += stride;
	}
}

/*
 * Verify that 'bits' repeats every 'len' bits
 */
static Bool
fbBitsRepeat(FbBits bits, int len, int width)
{
	FbBits mask = FbBitsMask(0, len);
	FbBits orig = bits & mask;
	int i;

	if (width > FB_UNIT)
		width = FB_UNIT;
	for (i = 0; i < width / len; i++) {
		if ((bits & mask) != orig)
			return FALSE;
		bits = FbScrLeft(bits, len);
	}
	return TRUE;
}

/*
 * Check whether an entire bitmap line is a repetition of
 * the first 'len' bits
 */
static Bool
fbLineRepeat(FbBits * bits, int len, int width)
{
	FbBits first = bits[0];

	if (!fbBitsRepeat(first, len, width))
		return FALSE;
	width = (width + FB_UNIT - 1) >> FB_SHIFT;
	bits++;
	while (--width)
		if (READ(bits) != first)
			return FALSE;
	return TRUE;
}

/*
 * The even stipple code wants the first FB_UNIT/bpp bits on
 * each scanline to represent the entire stipple
 */
static Bool
fbCanEvenStipple(PixmapPtr pStipple, int bpp)
{
	int len = FB_UNIT / bpp;
	FbBits *bits;
	int stride;
	int stip_bpp;
	_X_UNUSED int stipXoff, stipYoff;
	int h;

	/* make sure the stipple width is a multiple of the even stipple width */
	if (pStipple->drawable.width % len != 0)
		return FALSE;

	fbGetDrawable(&pStipple->drawable, bits, stride, stip_bpp, stipXoff,
		      stipYoff);
	h = pStipple->drawable.height;
	/* check to see that the stipple repeats horizontally */
	while (h--) {
		if (!fbLineRepeat(bits, len, pStipple->drawable.width))
			return FALSE;

		bits += stride;
	}
	return TRUE;
}

void
fbValidateGC(GCPtr gc, unsigned long changes, DrawablePtr drawable)
{
	FbGCPrivPtr pgc = fb_gc(gc);
	FbBits mask;

	DBG(("%s changes=%lx\n", __FUNCTION__, changes));

	if (changes & GCStipple) {
		pgc->evenStipple = FALSE;

		if (gc->stipple) {
			/* can we do an even stipple ?? */
			if (FbEvenStip(gc->stipple->drawable.width,
				       drawable->bitsPerPixel) &&
			    (fbCanEvenStipple(gc->stipple, drawable->bitsPerPixel)))
				pgc->evenStipple = TRUE;
		}
	}

	/*
	 * Recompute reduced rop values
	 */
	if (changes & (GCForeground | GCBackground | GCPlaneMask | GCFunction)) {
		int s;
		FbBits depthMask;

		mask = FbFullMask(drawable->bitsPerPixel);
		depthMask = FbFullMask(drawable->depth);

		DBG(("%s: computing rrop mask=%08x, depthMask=%08x, fg=%08x, bg=%08x, planemask=%08x\n",
		     __FUNCTION__, mask, depthMask, (int)gc->fgPixel, (int)gc->bgPixel, (int)gc->planemask));

		pgc->fg = gc->fgPixel & mask;
		pgc->bg = gc->bgPixel & mask;

		if ((gc->planemask & depthMask) == depthMask)
			pgc->pm = mask;
		else
			pgc->pm = gc->planemask & mask;

		s = drawable->bitsPerPixel;
		while (s < FB_UNIT) {
			pgc->fg |= pgc->fg << s;
			pgc->bg |= pgc->bg << s;
			pgc->pm |= pgc->pm << s;
			s <<= 1;
		}
		pgc->and = fbAnd(gc->alu, pgc->fg, pgc->pm);
		pgc->xor = fbXor(gc->alu, pgc->fg, pgc->pm);
		pgc->bgand = fbAnd(gc->alu, pgc->bg, pgc->pm);
		pgc->bgxor = fbXor(gc->alu, pgc->bg, pgc->pm);

		DBG(("%s: rrop fg=%08x, bg=%08x, pm=%08x, and=%08x, xor=%08x, bgand=%08x, bgxor=%08x\n",
		     __FUNCTION__, pgc->fg, pgc->bg, pgc->pm, pgc->and, pgc->xor, pgc->bgand, pgc->bgxor));
	}

	if (changes & GCDashList) {
		unsigned short n = gc->numInDashList;
		unsigned char *dash = gc->dash;
		unsigned int dashLength = 0;

		while (n--)
			dashLength += (unsigned int) *dash++;
		pgc->dashLength = dashLength;
	}
}
