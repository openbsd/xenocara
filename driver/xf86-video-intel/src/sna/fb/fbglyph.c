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
#include <X11/fonts/fontstruct.h>
#include <dixfontstr.h>

#define GLYPH	    fbGlyph8
#define BITS	    BYTE
#define BITS2	    CARD16
#define BITS4	    CARD32
#include "fbglyphbits.h"
#undef BITS
#undef BITS2
#undef BITS4
#undef GLYPH

#define GLYPH	    fbGlyph16
#define BITS	    CARD16
#define BITS2	    CARD32
#include "fbglyphbits.h"
#undef BITS
#undef BITS2
#undef GLYPH

#define GLYPH	    fbGlyph32
#define BITS	    CARD32
#include "fbglyphbits.h"
#undef BITS
#undef GLYPH

static bool
fbGlyphIn(GCPtr gc, int x, int y, int width, int height)
{
	BoxRec box;
	BoxPtr extents = RegionExtents(gc->pCompositeClip);

	/*
	 * Check extents by hand to avoid 16 bit overflows
	 */
	if (x < (int) extents->x1 || (int) extents->x2 < x + width)
		return FALSE;
	if (y < (int) extents->y1 || (int) extents->y2 < y + height)
		return FALSE;

	box.x1 = x;
	box.x2 = x + width;
	box.y1 = y;
	box.y2 = y + height;
	return RegionContainsRect(gc->pCompositeClip, &box) == rgnIN;
}

#define WRITE1(d,n,fg)	WRITE((d) + (n), (CARD8) fg)
#define WRITE2(d,n,fg)	WRITE((CARD16 *) &(d[n]), (CARD16) fg)
#define WRITE4(d,n,fg)	WRITE((CARD32 *) &(d[n]), (CARD32) fg)

/*
 * This is a bit tricky, but it's brief.  Write 12 bytes worth
 * of dest, which is four pixels, at a time.  This gives constant
 * code for each pattern as they're always aligned the same
 *
 *  a b c d  a b c d  a b c d	bytes
 *  A B C A  B C A B  C A B C	pixels
 * 
 *    f0        f1       f2
 *  A B C A  B C A B  C A B C	pixels LSB
 *  C A B C  A B C A  B C A B	pixels MSB
 *
 *		LSB	MSB
 *  A		f0	f1
 *  B		f1	f2
 *  C		f2	f0
 *  A B		f0	f2
 *  B C		f1	f0
 *  C A		f2	f1
 *  A B C A	f0	f1
 *  B C A B	f1    	f2
 *  C A B C	f2	f0
 */

#undef _A
#undef _B
#undef _C
#undef _AB
#undef _BC
#undef _CA
#undef _ABCA
#undef _BCAB
#undef _CABC

#define _A	f0
#define _B	f1
#define _C	f2
#define _AB	f0
#define _BC	f1
#define _CA	f2
#define _ABCA	f0
#define _BCAB	f1
#define _CABC	f2
#define CASE(a,b,c,d)	(a | (b << 1) | (c << 2) | (d << 3))

void
fbPolyGlyphBlt(DrawablePtr drawable, GCPtr gc,
               int x, int y,
               unsigned int nglyph, CharInfoPtr * ppci, pointer glyphs)
{
	FbGCPrivPtr pgc = fb_gc(gc);
	CharInfoPtr pci;
	unsigned char *pglyph;      /* pointer bits in glyph */
	int gx, gy;
	int gWidth, gHeight;        /* width and height of glyph */
	FbStride gStride;           /* stride of glyph */
	void (*raster) (FbBits *, FbStride, int, FbStip *, FbBits, int, int);
	FbBits *dst = 0;
	FbStride dstStride = 0;
	int dstBpp = 0;
	int dstXoff = 0, dstYoff = 0;

	DBG(("%s x %d\n", __FUNCTION__, nglyph));

	raster = 0;
	if (gc->fillStyle == FillSolid && pgc->and == 0) {
		dstBpp = drawable->bitsPerPixel;
		switch (dstBpp) {
		case 8:
			raster = fbGlyph8;
			break;
		case 16:
			raster = fbGlyph16;
			break;
		case 32:
			raster = fbGlyph32;
			break;
		}
	}
	x += drawable->x;
	y += drawable->y;

	while (nglyph--) {
		pci = *ppci++;
		pglyph = FONTGLYPHBITS(glyphs, pci);
		gWidth = GLYPHWIDTHPIXELS(pci);
		gHeight = GLYPHHEIGHTPIXELS(pci);
		if (gWidth && gHeight) {
			gx = x + pci->metrics.leftSideBearing;
			gy = y - pci->metrics.ascent;
			if (raster && gWidth <= sizeof(FbStip) * 8 &&
			    fbGlyphIn(gc, gx, gy, gWidth, gHeight)) {
				fbGetDrawable(drawable, dst, dstStride, dstBpp, dstXoff,
					      dstYoff);
				raster(dst + (gy + dstYoff) * dstStride, dstStride, dstBpp,
					  (FbStip *) pglyph, pgc->xor, gx + dstXoff, gHeight);
			} else {
				gStride = GLYPHWIDTHBYTESPADDED(pci) / sizeof(FbStip);
				fbPushImage(drawable, gc,
					    (FbStip *)pglyph,
					    gStride, 0, gx, gy, gWidth, gHeight);
			}
		}
		x += pci->metrics.characterWidth;
	}
}

void
fbImageGlyphBlt(DrawablePtr drawable, GCPtr gc,
                int x, int y,
                unsigned int nglyph, CharInfoPtr * ppciInit, pointer glyphs)
{
	FbGCPrivPtr pgc = fb_gc(gc);
	CharInfoPtr *ppci;
	CharInfoPtr pci;
	unsigned char *pglyph;      /* pointer bits in glyph */
	int gWidth, gHeight;        /* width and height of glyph */
	FbStride gStride;           /* stride of glyph */
	bool opaque;
	int n;
	int gx, gy;
	void (*raster)(FbBits *, FbStride, int, FbStip *, FbBits, int, int);
	FbBits *dst = 0;
	FbStride dstStride = 0;
	int dstBpp = 0;
	int dstXoff = 0, dstYoff = 0;

	DBG(("%s x %d\n", __FUNCTION__, nglyph));

	raster = 0;
	if (pgc->and == 0) {
		dstBpp = drawable->bitsPerPixel;
		switch (dstBpp) {
		case 8:
			raster = fbGlyph8;
			break;
		case 16:
			raster = fbGlyph16;
			break;
		case 32:
			raster = fbGlyph32;
			break;
		}
	}

	x += drawable->x;
	y += drawable->y;

	if (TERMINALFONT(gc->font) && !raster) {
		opaque = TRUE;
	} else {
		int xBack, widthBack;
		int yBack, heightBack;

		ppci = ppciInit;
		n = nglyph;
		widthBack = 0;
		while (n--)
			widthBack += (*ppci++)->metrics.characterWidth;

		xBack = x;
		if (widthBack < 0) {
			xBack += widthBack;
			widthBack = -widthBack;
		}
		yBack = y - FONTASCENT(gc->font);
		heightBack = FONTASCENT(gc->font) + FONTDESCENT(gc->font);
		fbSolidBoxClipped(drawable, gc,
				  xBack, yBack,
				  xBack + widthBack,
				  yBack + heightBack);
		opaque = FALSE;
	}

	ppci = ppciInit;
	while (nglyph--) {
		pci = *ppci++;
		pglyph = FONTGLYPHBITS(glyphs, pci);
		gWidth = GLYPHWIDTHPIXELS(pci);
		gHeight = GLYPHHEIGHTPIXELS(pci);
		if (gWidth && gHeight) {
			gx = x + pci->metrics.leftSideBearing;
			gy = y - pci->metrics.ascent;
			if (raster && gWidth <= sizeof(FbStip) * 8 &&
			    fbGlyphIn(gc, gx, gy, gWidth, gHeight)) {
				fbGetDrawable(drawable, dst, dstStride, dstBpp, dstXoff,
					      dstYoff);
				raster(dst + (gy + dstYoff) * dstStride, dstStride, dstBpp,
				       (FbStip *) pglyph, pgc->fg, gx + dstXoff, gHeight);
			} else {
				gStride = GLYPHWIDTHBYTESPADDED(pci) / sizeof(FbStip);
				fbPutXYImage(drawable, gc,
					     pgc->fg, pgc->bg, pgc->pm,
					     GXcopy, opaque,
					     gx, gy, gWidth, gHeight,
					     (FbStip *) pglyph, gStride, 0);
			}
		}
		x += pci->metrics.characterWidth;
	}
}
