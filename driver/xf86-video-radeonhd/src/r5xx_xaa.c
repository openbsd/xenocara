/*
 * Copyright 2008  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2008  Matthias Hopf <mhopf@novell.com>
 * Copyright 2008  Egbert Eich   <eich@novell.com>
 * Copyright 2008  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The below code is a clean-up from code in  xf86-video-ati/src/radeon_accel.c
 * The original license is included below, it has the messed up disclaimer and
 * an all rights reserved statement.
 */

/*
 * Copyright 2000 ATI Technologies Inc., Markham, Ontario, and
 *                VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *   Rickard E. Faith <faith@valinux.com>
 *   Alan Hourihane <alanh@fairlite.demon.co.uk>
 *   Michel Dänzer <michel@daenzer.net>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xaa.h"
#include "miline.h" /* line support */
#include "compiler.h" /* write_mem_barrier */

#include "rhd.h"
#include "r5xx_accel.h"
#include "r5xx_2dregs.h"

extern struct R5xxRop R5xxRops[];

/* Set up for transparency
 *
 * Mmmm, Seems as though the transparency compare is opposite to r128.
 * It should only draw when source != trans_color, this is the opposite
 * of that.
 */
static void
R5xxXAASetTransparency(ScrnInfoPtr pScrn, int trans_color)
{
    R5xxFIFOWait(pScrn->scrnIndex, 3);

    RHDRegWrite(pScrn, R5XX_CLR_CMP_CLR_SRC, trans_color);
    RHDRegWrite(pScrn, R5XX_CLR_CMP_MASK, R5XX_CLR_CMP_MSK);
    RHDRegWrite(pScrn, R5XX_CLR_CMP_CNTL,
		R5XX_SRC_CMP_EQ_COLOR | R5XX_CLR_CMP_SRC_SOURCE);
}

/*
 * Set up the clipping rectangle
 */
static void
R5xxXAASetClippingRectangle(ScrnInfoPtr pScrn, int xa, int ya, int xb, int yb)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;
    unsigned long tmp1, tmp2;

    if (xa < 0) {
	tmp1 = (-xa) & 0x3fff;
	tmp1 |= R5XX_SC_SIGN_MASK_LO;
    } else
	tmp1 = xa;

    if (ya < 0) {
	tmp1 |= (((-ya) & 0x3fff) << 16);
	tmp1 |= R5XX_SC_SIGN_MASK_HI;
    } else
	tmp1 |= (ya << 16);

    xb++;
    yb++;

    if (xb < 0) {
	tmp2 = (-xb) & 0x3fff;
	tmp2 |= R5XX_SC_SIGN_MASK_LO;
    } else
	tmp2 = xb;

    if (yb < 0) {
	tmp2 |= (((-yb) & 0x3fff) << 16);
	tmp2 |= R5XX_SC_SIGN_MASK_HI;
    } else
	tmp2 |= (yb << 16);

    R5xxFIFOWait(pScrn->scrnIndex, 3);

    RHDRegWrite(pScrn, R5XX_DP_GUI_MASTER_CNTL, TwoDInfo->control_saved |
		  R5XX_GMC_DST_CLIPPING);
    RHDRegWrite(pScrn, R5XX_SC_TOP_LEFT, tmp1);
    RHDRegWrite(pScrn, R5XX_SC_BOTTOM_RIGHT, tmp2);

    if (TwoDInfo->trans_color != -1)
	R5xxXAASetTransparency(pScrn, TwoDInfo->trans_color);
}

/*
 * Disable the clipping rectangle
 */
static void
R5xxXAADisableClipping(ScrnInfoPtr pScrn)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;

    R5xxFIFOWait(pScrn->scrnIndex, 3);

    RHDRegWrite(pScrn, R5XX_DP_GUI_MASTER_CNTL, TwoDInfo->control_saved);
    RHDRegWrite(pScrn, R5XX_SC_TOP_LEFT, 0);
    RHDRegWrite(pScrn, R5XX_SC_BOTTOM_RIGHT,
		  R5XX_DEFAULT_SC_RIGHT_MAX | R5XX_DEFAULT_SC_BOTTOM_MAX);
    if (TwoDInfo->trans_color != -1)
	R5xxXAASetTransparency(pScrn, TwoDInfo->trans_color);
}

/*
 *
 */
static void
R5xxXAASetupForSolidFill(ScrnInfoPtr pScrn,
			 int color, int rop, unsigned int planemask)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;
    CARD32 control;

    control = TwoDInfo->control | R5xxRops[rop].pattern;
    control |= R5XX_GMC_BRUSH_SOLID_COLOR | R5XX_GMC_SRC_DATATYPE_COLOR;

    /* Save for later clipping */
    TwoDInfo->control_saved = control;

    R5xxFIFOWait(pScrn->scrnIndex, 4);

    RHDRegWrite(pScrn, R5XX_DP_GUI_MASTER_CNTL, control);
    RHDRegWrite(pScrn, R5XX_DP_BRUSH_FRGD_CLR, color);
    RHDRegWrite(pScrn, R5XX_DP_WRITE_MASK, planemask);
    RHDRegWrite(pScrn, R5XX_DP_CNTL,
		R5XX_DST_X_LEFT_TO_RIGHT | R5XX_DST_Y_TOP_TO_BOTTOM);
}

/*
 * Tests: xtest CH06/fllrctngl, xterm
 */
static void
R5xxXAASubsequentSolidFillRect(ScrnInfoPtr pScrn, int x, int y, int w, int h)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;

    R5xxFIFOWait(pScrn->scrnIndex, 3);

    RHDRegWrite(pScrn, R5XX_DST_PITCH_OFFSET, TwoDInfo->dst_pitch_offset);
    RHDRegWrite(pScrn, R5XX_DST_Y_X, (y << 16) | x);
    RHDRegWrite(pScrn, R5XX_DST_WIDTH_HEIGHT, (w << 16) | h);
}

/*
 * Setup for XAA solid lines
 */
static void
R5xxXAASetupForSolidLine(ScrnInfoPtr pScrn,
			 int color, int rop, unsigned int planemask)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;
    CARD32 control = TwoDInfo->control;

    control |= R5XX_GMC_BRUSH_SOLID_COLOR |
	R5XX_GMC_SRC_DATATYPE_COLOR | R5xxRops[rop].pattern;

    /* Save for later clipping */
    TwoDInfo->control_saved = control;

    R5xxFIFOWait(pScrn->scrnIndex, 4);

    RHDRegWrite(pScrn, R5XX_DST_LINE_PATCOUNT, 0x55 << R5XX_BRES_CNTL_SHIFT);
    RHDRegWrite(pScrn, R5XX_DP_GUI_MASTER_CNTL, control);
    RHDRegWrite(pScrn, R5XX_DP_BRUSH_FRGD_CLR, color);
    RHDRegWrite(pScrn, R5XX_DP_WRITE_MASK, planemask);
}

/*
 * Subsequent XAA solid horizontal and vertical lines
 */
static void
R5xxXAASubsequentSolidHorVertLine(ScrnInfoPtr pScrn,
				  int x, int y, int len, int dir)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;
    int w = 1, h = 1;

    if (dir == DEGREES_0)
	w = len;
    else
	h = len;

    R5xxFIFOWait(pScrn->scrnIndex, 4);

    RHDRegWrite(pScrn, R5XX_DP_CNTL,
		R5XX_DST_X_LEFT_TO_RIGHT | R5XX_DST_Y_TOP_TO_BOTTOM);
    RHDRegWrite(pScrn, R5XX_DST_PITCH_OFFSET, TwoDInfo->dst_pitch_offset);
    RHDRegWrite(pScrn, R5XX_DST_Y_X, (y << 16) | x);
    RHDRegWrite(pScrn, R5XX_DST_WIDTH_HEIGHT, (w << 16) | h);
}

/* Subsequent XAA solid TwoPointLine line
 *
 * Tests: xtest CH06/drwln, ico, Mark Vojkovich's linetest program
 *
 * [See http://www.xfree86.org/devel/archives/devel/1999-Jun/0102.shtml for
 * Mark Vojkovich's linetest program, posted 2Jun99 to devel@xfree86.org.]
 */
static void
R5xxXAASubsequentSolidTwoPointLine(ScrnInfoPtr pScrn, int xa, int ya,
				   int xb, int yb, int flags)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;

    if (!(flags & OMIT_LAST))
	R5xxXAASubsequentSolidHorVertLine(pScrn, xb, yb, 1, DEGREES_0);

    R5xxFIFOWait(pScrn->scrnIndex, 3);

    RHDRegWrite(pScrn, R5XX_DST_PITCH_OFFSET, TwoDInfo->dst_pitch_offset);
    RHDRegWrite(pScrn, R5XX_DST_LINE_START, (ya << 16) | xa);
    RHDRegWrite(pScrn, R5XX_DST_LINE_END, (yb << 16) | xb);
}

/*
 * Tests: xtest CH06/fllrctngl (also tests transparency)
 */
static void
R5xxXAASetupForScreenToScreenCopy(ScrnInfoPtr pScrn, int xdir, int ydir, int rop,
				  unsigned int planemask, int trans_color)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;
    CARD32 control;

    TwoDInfo->xdir = xdir;
    TwoDInfo->ydir = ydir;

    control = TwoDInfo->control;
    control |= R5XX_GMC_BRUSH_NONE | R5XX_GMC_SRC_DATATYPE_COLOR |
	R5xxRops[rop].rop | R5XX_DP_SRC_SOURCE_MEMORY |
	R5XX_GMC_SRC_PITCH_OFFSET_CNTL;

    /* Save for later clipping */
    TwoDInfo->control_saved = control;

    R5xxFIFOWait(pScrn->scrnIndex, 3);

    RHDRegWrite(pScrn, R5XX_DP_GUI_MASTER_CNTL, control);
    RHDRegWrite(pScrn, R5XX_DP_WRITE_MASK, planemask);
    RHDRegWrite(pScrn, R5XX_DP_CNTL,
		((xdir >= 0 ? R5XX_DST_X_LEFT_TO_RIGHT : 0) |
		 (ydir >= 0 ? R5XX_DST_Y_TOP_TO_BOTTOM : 0)));

    TwoDInfo->trans_color = trans_color;
    if (trans_color != -1)
	R5xxXAASetTransparency(pScrn, trans_color);
}

/*
 *
 */
static void
R5xxXAASubsequentScreenToScreenCopy(ScrnInfoPtr pScrn, int xa, int ya,
				    int xb, int yb, int w, int h)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;

    if (TwoDInfo->xdir < 0) {
	xa += w - 1;
	xb += w - 1;
    }

    if (TwoDInfo->ydir < 0) {
	ya += h - 1;
	yb += h - 1;
    }

    R5xxFIFOWait(pScrn->scrnIndex, 5);

    RHDRegWrite(pScrn, R5XX_SRC_PITCH_OFFSET, TwoDInfo->dst_pitch_offset);
    RHDRegWrite(pScrn, R5XX_DST_PITCH_OFFSET, TwoDInfo->dst_pitch_offset);
    RHDRegWrite(pScrn, R5XX_SRC_Y_X, (ya << 16) | xa);
    RHDRegWrite(pScrn, R5XX_DST_Y_X, (yb << 16) | xb);
    RHDRegWrite(pScrn, R5XX_DST_HEIGHT_WIDTH, (h << 16) | w);
}

/* Setup for XAA mono 8x8 pattern color expansion.  Patterns with
 * transparency use `bg == -1'.  This routine is only used if the XAA
 * pixmap cache is turned on.
 *
 * Tests: xtest XFree86/fllrctngl (no other test will test this routine with
 *                                 both transparency and non-transparency)
 */
static void
R5xxXAASetupForMono8x8PatternFill(ScrnInfoPtr pScrn, int patternx, int patterny,
				  int fg, int bg, int rop, unsigned int planemask)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;
    CARD32 control;

#if X_BYTE_ORDER == X_BIG_ENDIAN

#define CARD32SWAP(x) ((x) >> 24) | (((x) & 0x00FF0000) >> 8) | \
    (((x) & 0x0000FF00) << 8) | (((x) & 0x000000FF) << 24)

    patternx = CARD32SWAP(patternx);
    patterny = CARD32SWAP(patterny);
#endif

    control = TwoDInfo->control | R5xxRops[rop].pattern;
    if (bg != -1)
	control |= R5XX_GMC_BRUSH_8X8_MONO_FG_BG;
    else
	control |= R5XX_GMC_BRUSH_8X8_MONO_FG_LA;
#if X_BYTE_ORDER == X_LITTLE_ENDIAN
    control |= R5XX_GMC_BYTE_MSB_TO_LSB;
#endif

    /* Save for later clipping */
    TwoDInfo->control_saved = control;

    if (bg != -1)
	R5xxFIFOWait(pScrn->scrnIndex, 5);
    else
	R5xxFIFOWait(pScrn->scrnIndex, 6);

    RHDRegWrite(pScrn, R5XX_DP_GUI_MASTER_CNTL, control);
    RHDRegWrite(pScrn, R5XX_DP_WRITE_MASK, planemask);
    RHDRegWrite(pScrn, R5XX_DP_BRUSH_FRGD_CLR, fg);
    if (bg != -1)
	RHDRegWrite(pScrn, R5XX_DP_BRUSH_BKGD_CLR, bg);

    RHDRegWrite(pScrn, R5XX_BRUSH_DATA0, patternx);
    RHDRegWrite(pScrn, R5XX_BRUSH_DATA1, patterny);
}

/* Subsequent XAA 8x8 pattern color expansion.  Because they are used in
 * the setup function, `patternx' and `patterny' are not used here.
 */
static void
R5xxXAASubsequentMono8x8PatternFillRect(ScrnInfoPtr pScrn,
					int patternx, int patterny,
					int x, int y, int w, int h)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;

    R5xxFIFOWait(pScrn->scrnIndex, 4);

    RHDRegWrite(pScrn, R5XX_DST_PITCH_OFFSET, TwoDInfo->dst_pitch_offset);
    RHDRegWrite(pScrn, R5XX_BRUSH_Y_X, (patterny << 8) | patternx);
    RHDRegWrite(pScrn, R5XX_DST_Y_X, (y << 16) | x);
    RHDRegWrite(pScrn, R5XX_DST_HEIGHT_WIDTH, (h << 16) | w);
}

/* Setup for XAA indirect CPU-to-screen color expansion (indirect).
 * Because of how the scratch buffer is initialized, this is really a
 * mainstore-to-screen color expansion.  Transparency is supported when
 * `bg == -1'.
 */
static void
R5xxXAASetupForScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrn, int fg, int bg,
						  int rop, unsigned int planemask)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;
    CARD32 control = TwoDInfo->control;

    TwoDInfo->scanline_bpp = 0;

    control |= R5XX_GMC_DST_CLIPPING | R5XX_GMC_BRUSH_NONE |
	R5xxRops[rop].rop | R5XX_DP_SRC_SOURCE_HOST_DATA;

    if (bg == -1)
	control |= R5XX_GMC_SRC_DATATYPE_MONO_FG_LA;
    else
	control |= R5XX_GMC_SRC_DATATYPE_MONO_FG_BG;

#if X_BYTE_ORDER == X_LITTLE_ENDIAN
    control |= R5XX_GMC_BYTE_LSB_TO_MSB;
#else
    control |= R5XX_GMC_BYTE_MSB_TO_LSB;
#endif

    /* Save for later clipping */
    TwoDInfo->control_saved = control;

#if X_BYTE_ORDER == X_LITTLE_ENDIAN
    if (bg != -1)
	R5xxFIFOWait(pScrn->scrnIndex, 4);
    else
	R5xxFIFOWait(pScrn->scrnIndex, 3);
#else
    if (bg != -1)
	R5xxFIFOWait(pScrn->scrnIndex, 5);
    else
	R5xxFIFOWait(pScrn->scrnIndex, 4);

    RHDRegWrite(pScrn, R5XX_RBBM_GUICNTL, R5XX_HOST_DATA_SWAP_NONE);
#endif
    RHDRegWrite(pScrn, R5XX_DP_GUI_MASTER_CNTL, control);
    RHDRegWrite(pScrn, R5XX_DP_WRITE_MASK, planemask);
    RHDRegWrite(pScrn, R5XX_DP_SRC_FRGD_CLR, fg);

    if (bg != -1)
	RHDRegWrite(pScrn, R5XX_DP_SRC_BKGD_CLR, bg);
}

/* Subsequent XAA indirect CPU-to-screen color expansion.  This is only
 * called once for each rectangle.
 */
static void
R5xxXAASubsequentScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrn, int x, int y,
						    int w, int h, int skipleft)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;

    TwoDInfo->scanline_h = h;
    TwoDInfo->scanline_words = (w + 31) >> 5;

    R5xxFIFOWait(pScrn->scrnIndex, 5);

    RHDRegWrite(pScrn, R5XX_DST_PITCH_OFFSET, TwoDInfo->dst_pitch_offset);
    RHDRegWrite(pScrn, R5XX_SC_TOP_LEFT, (y << 16) | ((x + skipleft) & 0xffff));
    RHDRegWrite(pScrn, R5XX_SC_BOTTOM_RIGHT, ((y + h) << 16) | ((x + w) & 0xffff));
    RHDRegWrite(pScrn, R5XX_DST_Y_X, (y << 16) | (x & 0xffff));
    /* Have to pad the width here and use clipping engine */
    RHDRegWrite(pScrn, R5XX_DST_HEIGHT_WIDTH, (h << 16) | ((w + 31) & ~31));
}

/* Subsequent XAA indirect CPU-to-screen color expansion and indirect
 * image write.  This is called once for each scanline.
 */
static void
R5xxXAASubsequentScanline(ScrnInfoPtr pScrn, int bufno)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;
    CARD32 *p = (CARD32 *) TwoDInfo->Buffer;
    int i, left = TwoDInfo->scanline_words;

    TwoDInfo->scanline_h--;

    while (left) {
	write_mem_barrier();
	if (TwoDInfo->scanline_h == 0) {
	    /* Last scanline - finish write to DATA_LAST */
	    if (left <= 9) {
		R5xxFIFOWait(pScrn->scrnIndex, left);
		for (i = 4 * (9 - left); i < 36; i += 4)
		    RHDRegWrite(pScrn, R5XX_HOST_DATA0 + i, *p++);
		left = 0;
	    } else {
		R5xxFIFOWait(pScrn->scrnIndex, 8);
		for (i = 0; i < 32; i += 4)
		    RHDRegWrite(pScrn, R5XX_HOST_DATA0 + i, *p++);
		left -= 8;
	    }
	} else {
	    if (left <= 8) {
		R5xxFIFOWait(pScrn->scrnIndex, left);
		for (i = 8 - left; i < 8; i++)
		    RHDRegWrite(pScrn, R5XX_HOST_DATA0 + 4 * i, *p++);
		left = 0;
	    } else {
		R5xxFIFOWait(pScrn->scrnIndex, 8);
		for (i = 0; i < 8; i++)
		    RHDRegWrite(pScrn, R5XX_HOST_DATA0 + 4 * i, *p++);
		left -= 8;
	    }
	}
    }
}

/*
 * Setup for XAA indirect image write
 */
static void
R5xxXAASetupForScanlineImageWrite(ScrnInfoPtr pScrn, int rop, unsigned int planemask,
				  int trans_color, int bpp, int depth)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;
    CARD32 control = TwoDInfo->control;

    ErrorF("%s(rop = %d, planemask = 0x%08X, trans_color = %d, bpp = %d, depth = %d)\n",
	   __func__, rop, planemask, trans_color, bpp, depth);

    TwoDInfo->scanline_bpp = bpp;

    control |= R5XX_GMC_DST_CLIPPING | R5XX_GMC_BRUSH_NONE |
	R5XX_GMC_SRC_DATATYPE_COLOR | R5xxRops[rop].rop |
	R5XX_GMC_BYTE_MSB_TO_LSB | R5XX_DP_SRC_SOURCE_HOST_DATA;

    /* Save for later clipping */
    TwoDInfo->control_saved = control;

#if X_BYTE_ORDER == X_LITTLE_ENDIAN
    R5xxFIFOWait(pScrn->scrnIndex, 2);
#else
    R5xxFIFOWait(pScrn->scrnIndex, 3);

    if (bpp == 16)
	RHDRegWrite(pScrn, R5XX_RBBM_GUICNTL, R5XX_HOST_DATA_SWAP_16BIT);
    else if (bpp == 32)
	RHDRegWrite(pScrn, R5XX_RBBM_GUICNTL, R5XX_HOST_DATA_SWAP_32BIT);
    else
	RHDRegWrite(pScrn, R5XX_RBBM_GUICNTL, R5XX_HOST_DATA_SWAP_NONE);
#endif
    RHDRegWrite(pScrn, R5XX_DP_GUI_MASTER_CNTL, control);
    RHDRegWrite(pScrn, R5XX_DP_WRITE_MASK, planemask);

    TwoDInfo->trans_color = trans_color;
    if (trans_color != -1)
	R5xxXAASetTransparency(pScrn, trans_color);
}

/*
 *
 */
static void
R5xxXAASubsequentScanlineImageWriteRect(ScrnInfoPtr pScrn, int x, int y,
					int w, int h, int skipleft)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;
    int shift = 0; /* 32bpp */

    ErrorF("%s(x = %d, y = %d, w = %d, h = %d, skipleft = %d)\n",
	   __func__, x, y, w, h, skipleft);

    if (pScrn->bitsPerPixel == 8)
	shift = 3;
    else if (pScrn->bitsPerPixel == 16)
	shift = 1;

    TwoDInfo->scanline_h = h;
    TwoDInfo->scanline_words  = (w * TwoDInfo->scanline_bpp + 31) >> 5;

    R5xxFIFOWait(pScrn->scrnIndex, 5);
    RHDRegWrite(pScrn, R5XX_DST_PITCH_OFFSET, TwoDInfo->dst_pitch_offset);
    RHDRegWrite(pScrn, R5XX_SC_TOP_LEFT, (y << 16) | ((x + skipleft) & 0xffff));
    RHDRegWrite(pScrn, R5XX_SC_BOTTOM_RIGHT, ((y + h) << 16) | ((x + w) & 0xffff));
    RHDRegWrite(pScrn, R5XX_DST_Y_X, (y << 16) | (x & 0xffff));
    /* Have to pad the width here and use clipping engine */
    RHDRegWrite(pScrn, R5XX_DST_HEIGHT_WIDTH, (h << 16) | ((w + shift) & ~shift));
}

/*
 *
 */
static void
R5xxXAAFunctionsInit(ScrnInfoPtr pScrn, ScreenPtr pScreen, XAAInfoRecPtr XAAInfo)
{
    struct R5xx2DInfo *TwoDInfo = RHDPTR(pScrn)->TwoDInfo;

    XAAInfo->Flags = PIXMAP_CACHE | OFFSCREEN_PIXMAPS | LINEAR_FRAMEBUFFER;

    /* Sync */
    XAAInfo->Sync = R5xx2DIdle;

    /* Clipping */
    XAAInfo->SetClippingRectangle = R5xxXAASetClippingRectangle;
    XAAInfo->DisableClipping = R5xxXAADisableClipping;
    XAAInfo->ClippingFlags = HARDWARE_CLIP_SOLID_LINE | HARDWARE_CLIP_DASHED_LINE |
	/* HARDWARE_CLIP_SOLID_FILL | -- seems very slow with this on */
	HARDWARE_CLIP_MONO_8x8_FILL | HARDWARE_CLIP_SCREEN_TO_SCREEN_COPY;

    /* Solid Filled Rectangle */
    XAAInfo->PolyFillRectSolidFlags = 0;
    XAAInfo->SetupForSolidFill = R5xxXAASetupForSolidFill;
    XAAInfo->SubsequentSolidFillRect = R5xxXAASubsequentSolidFillRect;

    /* Solid Lines
     *
     * R5XX only supports 14 bits for lines and clipping and only draws lines
     * that are completely on-screen correctly.  This will cause display
     * corruption problem in the cases when out-of-range commands are issued,
     * like when dimming screen during GNOME logout in dual-head setup.  Solid
     * and dashed lines are therefore limited to the virtual screen.
     */
    XAAInfo->SolidLineFlags = LINE_LIMIT_COORDS;
    XAAInfo->SolidLineLimits.x1 = 0;
    XAAInfo->SolidLineLimits.y1 = 0;
    XAAInfo->SolidLineLimits.x2 = pScrn->virtualX - 1;
    XAAInfo->SolidLineLimits.y2 = pScrn->virtualY - 1;

    /* Call miSetZeroLineBias() to have mi/mfb/fb routines match
       hardware accel two point lines */
    miSetZeroLineBias(pScreen, OCTANT5 | OCTANT6 | OCTANT7 | OCTANT8);

    XAAInfo->SetupForSolidLine = R5xxXAASetupForSolidLine;
    XAAInfo->SubsequentSolidHorVertLine = R5xxXAASubsequentSolidHorVertLine;
    XAAInfo->SubsequentSolidTwoPointLine = R5xxXAASubsequentSolidTwoPointLine;

#if 0 /* Disabled on RV200 and newer because it does not pass XTest */
    XAAInfo->SetupForDashedLine = R5xxXAASetupForDashedLine;
    XAAInfo->SubsequentDashedTwoPointLine = R5xxXAASubsequentDashedTwoPointLine;
    XAAInfo->DashPatternMaxLength = 32;
    /* ROP3 doesn't seem to work properly for dashedline with GXinvert */
    XAAInfo->DashedLineFlags = LINE_PATTERN_LSBFIRST_LSBJUSTIFIED |
	LINE_PATTERN_POWER_OF_2_ONLY | LINE_LIMIT_COORDS | ROP_NEEDS_SOURCE;
    XAAInfo->DashedLineLimits.x1 = 0;
    XAAInfo->DashedLineLimits.y1 = 0;
    XAAInfo->DashedLineLimits.x2 = pScrn->virtualX - 1;
    XAAInfo->DashedLineLimits.y2 = pScrn->virtualY - 1;
#endif

    /* Screen-to-screen Copy */
    XAAInfo->ScreenToScreenCopyFlags = 0;
    XAAInfo->SetupForScreenToScreenCopy = R5xxXAASetupForScreenToScreenCopy;
    XAAInfo->SubsequentScreenToScreenCopy = R5xxXAASubsequentScreenToScreenCopy;

    /* Mono 8x8 Pattern Fill (Color Expand) */
    XAAInfo->Mono8x8PatternFillFlags = HARDWARE_PATTERN_PROGRAMMED_BITS |
	HARDWARE_PATTERN_PROGRAMMED_ORIGIN | HARDWARE_PATTERN_SCREEN_ORIGIN;
#if X_BYTE_ORDER == X_LITTLE_ENDIAN
    XAAInfo->Mono8x8PatternFillFlags |= BIT_ORDER_IN_BYTE_MSBFIRST;
#else
    XAAInfo->Mono8x8PatternFillFlags |= BIT_ORDER_IN_BYTE_LSBFIRST;
#endif
    XAAInfo->SetupForMono8x8PatternFill = R5xxXAASetupForMono8x8PatternFill;
    XAAInfo->SubsequentMono8x8PatternFillRect = R5xxXAASubsequentMono8x8PatternFillRect;

    /* Indirect CPU-To-Screen Color Expand
     *
     * R5XX gets upset, when using HOST provided data without a source rop.
     * To show run 'xtest's drwarc.
     */
    XAAInfo->ScanlineCPUToScreenColorExpandFillFlags =
	LEFT_EDGE_CLIPPING | ROP_NEEDS_SOURCE | LEFT_EDGE_CLIPPING_NEGATIVE_X;
    XAAInfo->NumScanlineColorExpandBuffers = 1;
    XAAInfo->ScanlineColorExpandBuffers = (CARD8 **) &TwoDInfo->Buffer;
    XAAInfo->SetupForScanlineCPUToScreenColorExpandFill = R5xxXAASetupForScanlineCPUToScreenColorExpandFill;
    XAAInfo->SubsequentScanlineCPUToScreenColorExpandFill = R5xxXAASubsequentScanlineCPUToScreenColorExpandFill;
    XAAInfo->SubsequentColorExpandScanline = R5xxXAASubsequentScanline;

    /* ImageWrite */
    XAAInfo->ScanlineImageWriteFlags = CPU_TRANSFER_PAD_DWORD |
	/* Performance tests show that we shouldn't use MMIOed GXcopy for uploads
	   as a memcpy is faster */
	NO_GXCOPY |
	/* R5XX gets upset, when using HOST provided data without a source rop.
	   To show run 'xtest's ptimg */
	ROP_NEEDS_SOURCE |
	SCANLINE_PAD_DWORD | LEFT_EDGE_CLIPPING | LEFT_EDGE_CLIPPING_NEGATIVE_X;
    XAAInfo->NumScanlineImageWriteBuffers = 1;
    XAAInfo->ScanlineImageWriteBuffers = (CARD8 **) &TwoDInfo->Buffer;
    XAAInfo->SetupForScanlineImageWrite = R5xxXAASetupForScanlineImageWrite;
    XAAInfo->SubsequentScanlineImageWriteRect = R5xxXAASubsequentScanlineImageWriteRect;
    XAAInfo->SubsequentImageWriteScanline = R5xxXAASubsequentScanline;
}

/*
 * Initialize memory manager.
 */
static void
R5xxXAAFBInit(ScrnInfoPtr pScrn, ScreenPtr pScreen)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    BoxRec AvailFBArea;
    int tmp = 0;

    tmp = rhdPtr->FbScanoutSize + rhdPtr->FbOffscreenSize;
    tmp /= pScrn->displayWidth * (pScrn->bitsPerPixel >> 3);

    /* BoxRec Members are limited to 16bit signed, but the acceleration engine
       can only use 8191 lines anyway. */
    if (tmp > 0x1FFF)
	tmp = 0x1FFF;

    AvailFBArea.x1 = 0;
    AvailFBArea.y1 = 0;
    AvailFBArea.x2 = pScrn->displayWidth;
    AvailFBArea.y2 = tmp;

    xf86InitFBManager(pScreen, &AvailFBArea);

    /* Calculate how much offscreen memory / pixmap cache we are using */
    tmp -= pScrn->virtualY;
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d scanlines of offscreen memory\n", tmp);
}

/*
 *
 */
Bool
R5xxXAAInit(ScrnInfoPtr pScrn, ScreenPtr pScreen)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    XAAInfoRecPtr XAAInfo;

    R5xx2DInit(pScrn);

    XAAInfo = XAACreateInfoRec();
    if (!XAAInfo) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "%s: XAACreateInfoRec failed.\n", __func__);
	R5xx2DDestroy(pScrn);
	return FALSE;
    }

    R5xxXAAFunctionsInit(pScrn, pScreen, XAAInfo);

    R5xxXAAFBInit(pScrn, pScreen);

    if (!XAAInit(pScreen, XAAInfo)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "%s: XAAInit failed.\n",
		   __func__);
	XAADestroyInfoRec(XAAInfo);
	R5xx2DDestroy(pScrn);
	return FALSE;
    }
    rhdPtr->XAAInfo = XAAInfo;

    return TRUE;
}

/*
 *
 */
void
R5xxXAADestroy(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);

    if (!rhdPtr->XAAInfo)
	return;

    XAADestroyInfoRec(rhdPtr->XAAInfo);
    rhdPtr->XAAInfo = NULL;

    R5xx2DDestroy(pScrn);
}
