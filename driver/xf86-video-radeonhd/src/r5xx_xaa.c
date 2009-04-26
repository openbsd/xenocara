/*
 * Copyright 2008  Luc Verhaegen <libv@exsuse.de>
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
#include "rhd_cs.h"
#include "r5xx_accel.h"
#include "r5xx_regs.h"

extern struct R5xxRop R5xxRops[];

struct R5xxXaaPrivate {
    CARD32 dst_pitch_offset; /* Base value for R5XX_DST_PITCH_OFFSET */
    CARD32 control; /* Base value for R5XX_DP_GUI_MASTER_CNTL */
    CARD32 control_saved;

    int xdir;
    int ydir;

    int trans_color;

    int scanline_x;
    int scanline_y;
    int scanline_w;
    CARD32 scanline_h;
    CARD32 scanline_words;
    int scanline_bpp; /* Only used for ImageWrite */
    int scanline_fg;
    int scanline_bg;
    int scanline_hpass;
    int scanline_x1clip;
    int scanline_x2clip;

    CARD8 *Buffer;
    CARD8 *BufferHook[1];
};

/*
 *
 */
static void
R5xxXAAIdle(ScrnInfoPtr pScrn)
{
    struct RhdCS *CS = RHDPTR(pScrn)->CS;

    RHDCSFlush(CS);
    RHDCSIdle(CS);
    R5xx2DIdle(pScrn);
}

/* Set up for transparency
 *
 * Mmmm, Seems as though the transparency compare is opposite to r128.
 * It should only draw when source != trans_color, this is the opposite
 * of that.
 */
static void
R5xxXAASetTransparency(struct RhdCS *CS, int trans_color)
{
    RHDCSGrab(CS, 2 * 3);

    RHDCSRegWrite(CS, R5XX_CLR_CMP_CLR_SRC, trans_color);
    RHDCSRegWrite(CS, R5XX_CLR_CMP_MASK, R5XX_CLR_CMP_MSK);
    RHDCSRegWrite(CS, R5XX_CLR_CMP_CNTL,
		  R5XX_SRC_CMP_EQ_COLOR | R5XX_CLR_CMP_SRC_SOURCE);
}

/*
 * Set up the clipping rectangle
 */
static void
R5xxXAASetClippingRectangle(ScrnInfoPtr pScrn, int xa, int ya, int xb, int yb)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;
    unsigned long tmp1, tmp2;

    if (xa < 0) {
	tmp1 = (-xa) & 0x3fff;
	tmp1 |= R5XX_SC_SIGN_MASK_LO;
    } else
	tmp1 = xa;

    if (ya < 0) {
	tmp1 |= ((-ya) & 0x3fff) << 16;
	tmp1 |= R5XX_SC_SIGN_MASK_HI;
    } else
	tmp1 |= ya << 16;

    xb++;
    yb++;

    if (xb < 0) {
	tmp2 = (-xb) & 0x3fff;
	tmp2 |= R5XX_SC_SIGN_MASK_LO;
    } else
	tmp2 = xb;

    if (yb < 0) {
	tmp2 |= ((-yb) & 0x3fff) << 16;
	tmp2 |= R5XX_SC_SIGN_MASK_HI;
    } else
	tmp2 |= yb << 16;

    RHDCSGrab(CS, 2 * 3);

    RHDCSRegWrite(CS, R5XX_DP_GUI_MASTER_CNTL,
		  XaaPrivate->control_saved | R5XX_GMC_DST_CLIPPING);
    RHDCSRegWrite(CS, R5XX_SC_TOP_LEFT, tmp1);
    RHDCSRegWrite(CS, R5XX_SC_BOTTOM_RIGHT, tmp2);

    if (XaaPrivate->trans_color != -1)
	R5xxXAASetTransparency(CS, XaaPrivate->trans_color);

    RHDCSAdvance(CS);
}

/*
 * Disable the clipping rectangle
 */
static void
R5xxXAADisableClipping(ScrnInfoPtr pScrn)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;

    RHDCSGrab(CS, 2 * 3);

    RHDCSRegWrite(CS, R5XX_DP_GUI_MASTER_CNTL, XaaPrivate->control_saved);
    RHDCSRegWrite(CS, R5XX_SC_TOP_LEFT, 0);
    RHDCSRegWrite(CS, R5XX_SC_BOTTOM_RIGHT,
		  R5XX_DEFAULT_SC_RIGHT_MAX | R5XX_DEFAULT_SC_BOTTOM_MAX);
    if (XaaPrivate->trans_color != -1)
	R5xxXAASetTransparency(CS, XaaPrivate->trans_color);

    RHDCSAdvance(CS);
}

/*
 *
 */
static void
R5xxXAASetupForSolidFill(ScrnInfoPtr pScrn,
			 int color, int rop, unsigned int planemask)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;
    CARD32 control;

    control = XaaPrivate->control | R5xxRops[rop].pattern;
    control |= R5XX_GMC_BRUSH_SOLID_COLOR | R5XX_GMC_SRC_DATATYPE_COLOR;

    /* Save for later clipping */
    XaaPrivate->control_saved = control;

    RHDCSGrab(CS, 2 * 4);

    RHDCSRegWrite(CS, R5XX_DP_GUI_MASTER_CNTL, control);
    RHDCSRegWrite(CS, R5XX_DP_BRUSH_FRGD_CLR, color);
    RHDCSRegWrite(CS, R5XX_DP_WRITE_MASK, planemask);
    RHDCSRegWrite(CS, R5XX_DP_CNTL,
		  R5XX_DST_X_LEFT_TO_RIGHT | R5XX_DST_Y_TOP_TO_BOTTOM);
}

/*
 * Tests: xtest CH06/fllrctngl, xterm
 */
static void
R5xxXAASubsequentSolidFillRect(ScrnInfoPtr pScrn, int x, int y, int w, int h)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;

    RHDCSGrab(CS, 2 * 3);

    RHDCSRegWrite(CS, R5XX_DST_PITCH_OFFSET, XaaPrivate->dst_pitch_offset);
    RHDCSRegWrite(CS, R5XX_DST_Y_X, (y << 16) | x);
    RHDCSRegWrite(CS, R5XX_DST_WIDTH_HEIGHT, (w << 16) | h);

    RHDCSAdvance(CS);
}

/*
 *
 */
static void
R5xxXAASetupForSolidLine(ScrnInfoPtr pScrn,
			 int color, int rop, unsigned int planemask)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;
    CARD32 control;

    control = XaaPrivate->control | R5xxRops[rop].pattern;
    control |= R5XX_GMC_BRUSH_SOLID_COLOR | R5XX_GMC_SRC_DATATYPE_COLOR;

    /* Save for later clipping */
    XaaPrivate->control_saved = control;

    RHDCSGrab(CS, 2 * 4);

    RHDCSRegWrite(CS, R5XX_DST_LINE_PATCOUNT, 0x55 << R5XX_BRES_CNTL_SHIFT);
    RHDCSRegWrite(CS, R5XX_DP_GUI_MASTER_CNTL, control);
    RHDCSRegWrite(CS, R5XX_DP_BRUSH_FRGD_CLR, color);
    RHDCSRegWrite(CS, R5XX_DP_WRITE_MASK, planemask);
}

/*
 *
 */
static void
R5xxXAASubsequentSolidHorVertLine(ScrnInfoPtr pScrn,
				  int x, int y, int len, int dir)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;
    int w = 1, h = 1;

    if (dir == DEGREES_0)
	w = len;
    else
	h = len;

    RHDCSGrab(CS, 2 * 4);

    RHDCSRegWrite(CS, R5XX_DP_CNTL,
		  R5XX_DST_X_LEFT_TO_RIGHT | R5XX_DST_Y_TOP_TO_BOTTOM);
    RHDCSRegWrite(CS, R5XX_DST_PITCH_OFFSET, XaaPrivate->dst_pitch_offset);
    RHDCSRegWrite(CS, R5XX_DST_Y_X, (y << 16) | x);
    RHDCSRegWrite(CS, R5XX_DST_WIDTH_HEIGHT, (w << 16) | h);

    RHDCSAdvance(CS);
}

/*
 * Tests: xtest CH06/drwln, ico, Mark Vojkovich's linetest program
 *
 * [See http://www.xfree86.org/devel/archives/devel/1999-Jun/0102.shtml for
 * Mark Vojkovich's linetest program, posted 2Jun99 to devel@xfree86.org.]
 */
static void
R5xxXAASubsequentSolidTwoPointLine(ScrnInfoPtr pScrn, int xa, int ya,
				   int xb, int yb, int flags)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;

    if (!(flags & OMIT_LAST))
	R5xxXAASubsequentSolidHorVertLine(pScrn, xb, yb, 1, DEGREES_0);

    RHDCSGrab(CS, 2 * 3);

    RHDCSRegWrite(CS, R5XX_DST_PITCH_OFFSET, XaaPrivate->dst_pitch_offset);
    RHDCSRegWrite(CS, R5XX_DST_LINE_START, (ya << 16) | xa);
    RHDCSRegWrite(CS, R5XX_DST_LINE_END, (yb << 16) | xb);

    RHDCSAdvance(CS);
}

/*
 * Tests: xtest CH06/fllrctngl (also tests transparency)
 */
static void
R5xxXAASetupForScreenToScreenCopy(ScrnInfoPtr pScrn, int xdir, int ydir, int rop,
				  unsigned int planemask, int trans_color)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;
    CARD32 control;

    XaaPrivate->xdir = xdir;
    XaaPrivate->ydir = ydir;

    control = XaaPrivate->control;
    control |= R5XX_GMC_BRUSH_NONE | R5XX_GMC_SRC_DATATYPE_COLOR |
	R5xxRops[rop].rop | R5XX_DP_SRC_SOURCE_MEMORY |
	R5XX_GMC_SRC_PITCH_OFFSET_CNTL;

    /* Save for later clipping */
    XaaPrivate->control_saved = control;

    RHDCSGrab(CS, 2 * 3);

    RHDCSRegWrite(CS, R5XX_DP_GUI_MASTER_CNTL, control);
    RHDCSRegWrite(CS, R5XX_DP_WRITE_MASK, planemask);
    RHDCSRegWrite(CS, R5XX_DP_CNTL,
		  (xdir >= 0 ? R5XX_DST_X_LEFT_TO_RIGHT : 0) |
		  (ydir >= 0 ? R5XX_DST_Y_TOP_TO_BOTTOM : 0));

    XaaPrivate->trans_color = trans_color;
    if (trans_color != -1)
	R5xxXAASetTransparency(CS, trans_color);
}

/*
 *
 */
static void
R5xxXAASubsequentScreenToScreenCopy(ScrnInfoPtr pScrn, int xa, int ya,
				    int xb, int yb, int w, int h)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;

    if (XaaPrivate->xdir < 0) {
	xa += w - 1;
	xb += w - 1;
    }

    if (XaaPrivate->ydir < 0) {
	ya += h - 1;
	yb += h - 1;
    }

    RHDCSGrab(CS, 2 * 5);

    RHDCSRegWrite(CS, R5XX_SRC_PITCH_OFFSET, XaaPrivate->dst_pitch_offset);
    RHDCSRegWrite(CS, R5XX_DST_PITCH_OFFSET, XaaPrivate->dst_pitch_offset);
    RHDCSRegWrite(CS, R5XX_SRC_Y_X, (ya << 16) | xa);
    RHDCSRegWrite(CS, R5XX_DST_Y_X, (yb << 16) | xb);
    RHDCSRegWrite(CS, R5XX_DST_HEIGHT_WIDTH, (h << 16) | w);

    RHDCSAdvance(CS);
}

/*
 * Setup for XAA mono 8x8 pattern color expansion.  Patterns with
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
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;
    CARD32 control;

#if X_BYTE_ORDER == X_BIG_ENDIAN

#define CARD32SWAP(x) ((x) >> 24) | (((x) & 0x00FF0000) >> 8) | \
    (((x) & 0x0000FF00) << 8) | (((x) & 0x000000FF) << 24)

    patternx = CARD32SWAP(patternx);
    patterny = CARD32SWAP(patterny);
#endif

    control = XaaPrivate->control | R5xxRops[rop].pattern;
    if (bg != -1)
	control |= R5XX_GMC_BRUSH_8X8_MONO_FG_BG;
    else
	control |= R5XX_GMC_BRUSH_8X8_MONO_FG_LA;
#if X_BYTE_ORDER == X_LITTLE_ENDIAN
    control |= R5XX_GMC_BYTE_MSB_TO_LSB;
#endif

    /* Save for later clipping */
    XaaPrivate->control_saved = control;

    if (bg != -1)
	RHDCSGrab(CS, 2 * 6);
    else
	RHDCSGrab(CS, 2 * 5);

    RHDCSRegWrite(CS, R5XX_DP_GUI_MASTER_CNTL, control);
    RHDCSRegWrite(CS, R5XX_DP_WRITE_MASK, planemask);
    RHDCSRegWrite(CS, R5XX_DP_BRUSH_FRGD_CLR, fg);
    if (bg != -1)
	RHDCSRegWrite(CS, R5XX_DP_BRUSH_BKGD_CLR, bg);

    RHDCSRegWrite(CS, R5XX_BRUSH_DATA0, patternx);
    RHDCSRegWrite(CS, R5XX_BRUSH_DATA1, patterny);
}

/*
 * Subsequent XAA 8x8 pattern color expansion.  Because they are used in
 * the setup function, `patternx' and `patterny' are not used here.
 */
static void
R5xxXAASubsequentMono8x8PatternFillRect(ScrnInfoPtr pScrn,
					int patternx, int patterny,
					int x, int y, int w, int h)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;

    RHDCSGrab(CS, 2 * 4);

    RHDCSRegWrite(CS, R5XX_DST_PITCH_OFFSET, XaaPrivate->dst_pitch_offset);
    RHDCSRegWrite(CS, R5XX_BRUSH_Y_X, (patterny << 8) | patternx);
    RHDCSRegWrite(CS, R5XX_DST_Y_X, (y << 16) | x);
    RHDCSRegWrite(CS, R5XX_DST_HEIGHT_WIDTH, (h << 16) | w);

    RHDCSAdvance(CS);
}

/*
 * Setup for XAA indirect CPU-to-screen color expansion (indirect).
 * Because of how the scratch buffer is initialized, this is really a
 * mainstore-to-screen color expansion.  Transparency is supported when
 * `bg == -1'.
 * We always need to provide the bg here, otherwise the engine locks.
 */
static void
R5xxXAASetupForScanlineCPUToScreenColorExpandFillMMIO(ScrnInfoPtr pScrn, int fg, int bg,
						      int rop, unsigned int planemask)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;
    CARD32 control = XaaPrivate->control;

    XaaPrivate->scanline_bpp = 0;

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
    XaaPrivate->control_saved = control;

#if X_BYTE_ORDER == X_LITTLE_ENDIAN
    RHDCSGrab(CS, 2 * 4);
#else
    RHDCSGrab(CS, 2 * 5);

    RHDCSRegWrite(CS, R5XX_RBBM_GUICNTL, R5XX_HOST_DATA_SWAP_NONE);
#endif
    RHDCSRegWrite(CS, R5XX_DP_GUI_MASTER_CNTL, control);
    RHDCSRegWrite(CS, R5XX_DP_WRITE_MASK, planemask);
    RHDCSRegWrite(CS, R5XX_DP_SRC_FRGD_CLR, fg);
    RHDCSRegWrite(CS, R5XX_DP_SRC_BKGD_CLR, bg);
}

/*
 * Subsequent XAA indirect CPU-to-screen color expansion.  This is only
 * called once for each rectangle.
 */
static void
R5xxXAASubsequentScanlineCPUToScreenColorExpandFillMMIO(ScrnInfoPtr pScrn, int x, int y,
							int w, int h, int skipleft)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;

    XaaPrivate->scanline_h = h;
    XaaPrivate->scanline_words = (w + 31) >> 5;

    RHDCSGrab(CS, 2 * 5);

    RHDCSRegWrite(CS, R5XX_DST_PITCH_OFFSET, XaaPrivate->dst_pitch_offset);
    RHDCSRegWrite(CS, R5XX_SC_TOP_LEFT, (y << 16) | ((x + skipleft) & 0xffff));
    RHDCSRegWrite(CS, R5XX_SC_BOTTOM_RIGHT, ((y + h) << 16) | ((x + w) & 0xffff));
    RHDCSRegWrite(CS, R5XX_DST_Y_X, (y << 16) | (x & 0xffff));
    /* Have to pad the width here and use clipping engine */
    RHDCSRegWrite(CS, R5XX_DST_HEIGHT_WIDTH, (h << 16) | ((w + 31) & ~31));

    RHDCSAdvance(CS);
}

/*
 * Subsequent XAA indirect CPU-to-screen color expansion and indirect
 * image write.  This is called once for each scanline.
 */
static void
R5xxXAASubsequentScanlineMMIO(ScrnInfoPtr pScrn, int bufno)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;
    CARD32 *p = (CARD32 *) XaaPrivate->BufferHook[bufno];
    int i, remainder, left = XaaPrivate->scanline_words;
    CARD16 Reg;

    XaaPrivate->scanline_h--;

    if (XaaPrivate->scanline_h)
	remainder = 8;
    else
	remainder = 9;

    while (left > remainder) {
	Reg = R5XX_HOST_DATA0;

	RHDCSGrab(CS, 2 * 8);
	for (i = 0; i < 8; i++, Reg += 4)
	    RHDCSRegWrite(CS, Reg, *p++);

	left -= 8;
    }

    if (XaaPrivate->scanline_h)
	Reg = R5XX_HOST_DATA7;
    else
	Reg = R5XX_HOST_DATA_LAST; /* Last scanline - finish write to DATA_LAST */

    Reg -= 4 * (left - 1);

    RHDCSGrab(CS, 2 * left);
    for (i = 0; i < left; i++, Reg += 4)
	RHDCSRegWrite(CS, Reg, *p++);

    if (!(XaaPrivate->scanline_h % 32)) /* don't push too big an amount of data */
	RHDCSAdvance(CS);
}

/*
 * Setup for XAA indirect image write
 */
static void
R5xxXAASetupForScanlineImageWriteMMIO(ScrnInfoPtr pScrn, int rop, unsigned int planemask,
				      int trans_color, int bpp, int depth)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;
    CARD32 control = XaaPrivate->control;

    XaaPrivate->scanline_bpp = bpp;

    control |= R5XX_GMC_DST_CLIPPING | R5XX_GMC_BRUSH_NONE |
	R5XX_GMC_SRC_DATATYPE_COLOR | R5xxRops[rop].rop |
	R5XX_GMC_BYTE_MSB_TO_LSB | R5XX_DP_SRC_SOURCE_HOST_DATA;

    /* Save for later clipping */
    XaaPrivate->control_saved = control;

#if X_BYTE_ORDER == X_LITTLE_ENDIAN
    RHDCSGrab(CS, 2 * 2);
#else
    RHDCSGrab(CS, 2 * 3);

    if (bpp == 16)
	RHDCSRegWrite(CS, R5XX_RBBM_GUICNTL, R5XX_HOST_DATA_SWAP_16BIT);
    else if (bpp == 32)
	RHDCSRegWrite(CS, R5XX_RBBM_GUICNTL, R5XX_HOST_DATA_SWAP_32BIT);
    else
	RHDCSRegWrite(CS, R5XX_RBBM_GUICNTL, R5XX_HOST_DATA_SWAP_NONE);
#endif
    RHDCSRegWrite(CS, R5XX_DP_GUI_MASTER_CNTL, control);
    RHDCSRegWrite(CS, R5XX_DP_WRITE_MASK, planemask);

    XaaPrivate->trans_color = trans_color;
    if (trans_color != -1)
	R5xxXAASetTransparency(CS, trans_color);
}

/*
 *
 */
static void
R5xxXAASubsequentScanlineImageWriteRectMMIO(ScrnInfoPtr pScrn, int x, int y,
					    int w, int h, int skipleft)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;
    int shift = 0; /* 32bpp */

    if (pScrn->bitsPerPixel == 8)
	shift = 3;
    else if (pScrn->bitsPerPixel == 16)
	shift = 1;

    XaaPrivate->scanline_h = h;
    XaaPrivate->scanline_words  = (w * XaaPrivate->scanline_bpp + 31) >> 5;

    RHDCSGrab(CS, 2 * 5);
    RHDCSRegWrite(CS, R5XX_DST_PITCH_OFFSET, XaaPrivate->dst_pitch_offset);
    RHDCSRegWrite(CS, R5XX_SC_TOP_LEFT, (y << 16) | ((x + skipleft) & 0xffff));
    RHDCSRegWrite(CS, R5XX_SC_BOTTOM_RIGHT, ((y + h) << 16) | ((x + w) & 0xffff));
    RHDCSRegWrite(CS, R5XX_DST_Y_X, (y << 16) | (x & 0xffff));
    /* Have to pad the width here and use clipping engine */
    RHDCSRegWrite(CS, R5XX_DST_HEIGHT_WIDTH, (h << 16) | ((w + shift) & ~shift));

    RHDCSAdvance(CS);
}

/*
 * Helper function to write out a HOSTDATA_BLT packet into the indirect
 * buffer and set the XAA scratch buffer address appropriately.
 */
static void
R5xxXAACPScanlinePacket(struct RhdCS *CS, struct R5xxXaaPrivate *XaaPrivate)
{
    int chunk_words = XaaPrivate->scanline_hpass * XaaPrivate->scanline_words;

    RHDCSGrab(CS, chunk_words + 10);

    RHDCSWrite(CS, CP_PACKET3(R5XX_CP_PACKET3_CNTL_HOSTDATA_BLT,chunk_words+10-2));
    RHDCSWrite(CS, XaaPrivate->control_saved);
    RHDCSWrite(CS, XaaPrivate->dst_pitch_offset);
    RHDCSWrite(CS, (XaaPrivate->scanline_y << 16) |
	       (XaaPrivate->scanline_x1clip & 0xffff));
    RHDCSWrite(CS, ((XaaPrivate->scanline_y + XaaPrivate->scanline_hpass) << 16) |
	       (XaaPrivate->scanline_x2clip & 0xffff));
    RHDCSWrite(CS, XaaPrivate->scanline_fg);
    RHDCSWrite(CS, XaaPrivate->scanline_bg);
    RHDCSWrite(CS, (XaaPrivate->scanline_y << 16) |
	       (XaaPrivate->scanline_x & 0xffff));
    RHDCSWrite(CS, (XaaPrivate->scanline_hpass << 16) |
	       (XaaPrivate->scanline_w & 0xffff));
    RHDCSWrite(CS, chunk_words);

    XaaPrivate->BufferHook[0] = (CARD8 *) &(CS->Buffer[CS->Wptr]);
    CS->Wptr += chunk_words;

    XaaPrivate->scanline_y += XaaPrivate->scanline_hpass;
    XaaPrivate->scanline_h -= XaaPrivate->scanline_hpass;
}

/*
 * Setup for XAA indirect CPU-to-screen color expansion (indirect).
 * Because of how the scratch buffer is initialized, this is really a
 * mainstore-to-screen color expansion.  Transparency is supported when
 * `bg == -1'.
 */
static void
R5xxXAASetupForScanlineCPUToScreenColorExpandFillCP(ScrnInfoPtr pScrn, int fg,
						    int bg, int rop, unsigned int planemask)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;
    CARD32 control = XaaPrivate->control;

    XaaPrivate->scanline_bpp = 0;

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

    XaaPrivate->control_saved = control;

    XaaPrivate->scanline_fg = fg;
    XaaPrivate->scanline_bg = bg;

    RHDCSGrab(CS, 2 * 1);
    RHDCSRegWrite(CS, R5XX_DP_WRITE_MASK, planemask);
}

/*
 * Subsequent XAA indirect CPU-to-screen color expansion.  This is only
 * called once for each rectangle.
 */
static void
R5xxXAASubsequentScanlineCPUToScreenColorExpandFillCP(ScrnInfoPtr pScrn, int x, int y,
						      int w, int h, int skipleft)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;

    XaaPrivate->scanline_x = x;
    XaaPrivate->scanline_y = y;
    /* Have to pad the width here and use clipping engine */
    XaaPrivate->scanline_w = (w + 31) & ~31;
    XaaPrivate->scanline_h = h;

    XaaPrivate->scanline_x1clip = x + skipleft;
    XaaPrivate->scanline_x2clip = x + w;

    XaaPrivate->scanline_words = XaaPrivate->scanline_w / 32;
    XaaPrivate->scanline_hpass =
	min(XaaPrivate->scanline_h, ((CS->Size - 10) / XaaPrivate->scanline_words));

    R5xxXAACPScanlinePacket(CS, XaaPrivate);
}

#if X_BYTE_ORDER == X_BIG_ENDIAN

static inline void
R5xxXAAScanlineSwap16(unsigned int *Buf, unsigned int nwords)
{
    unsigned int *d = Buf;
    unsigned int *s = Buf;

    for (; nwords > 0; --nwords, ++d, ++s)
	*d = ((*s & 0xffff) << 16) | ((*s >> 16) & 0xffff);
}

static inline void
R5xxXAAScanlineSwap8(unsigned int *Buf, unsigned int nwords)
{
    unsigned int *d = Buf;
    unsigned int *s = Buf;

    for (; nwords > 0; --nwords, ++d, ++s)
#ifdef __powerpc__
	asm volatile("stwbrx %0,0,%1" : : "r" (*s), "r" (d));
#else
    *d = ((*s >> 24) & 0xff) | ((*s >> 8) & 0xff00)
	| ((*s & 0xff00) << 8) | ((*s & 0xff) << 24);
#endif
}

#endif /* X_BYTE_ORDER */

/*
 * Subsequent XAA indirect CPU-to-screen color expansion and indirect
 * image write.  This is called once for each scanline.
 */
static void
R5xxXAASubsequentScanlineCP(ScrnInfoPtr pScrn, int bufno)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;

    if (bufno) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "%s: buffer number provided (%d)\n", __func__, bufno);
	return;
    }

#if X_BYTE_ORDER == X_BIG_ENDIAN
    if (XaaPrivate->scanline_bpp == 16)
	R5xxXAAScanlineSwap16((unsigned int *)XaaPrivate->BufferHook[0],
			     XaaPrivate->scanline_words);
    else if (XaaPrivate->scanline_bpp < 15)
	R5xxXAAScanlineSwap8((unsigned int *)XaaPrivate->BufferHook[0],
			    XaaPrivate->scanline_words);
#endif

    if (--XaaPrivate->scanline_hpass) {
	XaaPrivate->BufferHook[0] += 4 * XaaPrivate->scanline_words;
    } else if (XaaPrivate->scanline_h) {
	XaaPrivate->scanline_hpass =
	    min(XaaPrivate->scanline_h, ((CS->Size - 10) / XaaPrivate->scanline_words));
	R5xxXAACPScanlinePacket(CS, XaaPrivate);
    }
}

/*
 * Setup for XAA indirect image write
 */
static void
R5xxXAASetupForScanlineImageWriteCP(ScrnInfoPtr pScrn, int rop, unsigned int planemask,
				    int trans_color, int bpp, int depth)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;
    CARD32 control = XaaPrivate->control;

    XaaPrivate->scanline_bpp = bpp;

    control |= R5XX_GMC_DST_CLIPPING | R5XX_GMC_BRUSH_NONE |
	R5XX_GMC_SRC_DATATYPE_COLOR | R5xxRops[rop].rop |
	R5XX_GMC_BYTE_MSB_TO_LSB | R5XX_DP_SRC_SOURCE_HOST_DATA;

     /* Save for later clipping */
    XaaPrivate->control_saved = control;

    RHDCSGrab(CS, 2 * 2);

    RHDCSRegWrite(CS, R5XX_DP_GUI_MASTER_CNTL, control);
    RHDCSRegWrite(CS, R5XX_DP_WRITE_MASK, planemask);

    XaaPrivate->trans_color = trans_color;
    if (trans_color != -1)
	R5xxXAASetTransparency(CS, trans_color);
}

/*
 * Subsequent XAA indirect image write. This is only called once for
 * each rectangle.
 */
static void
R5xxXAASubsequentScanlineImageWriteRectCP(ScrnInfoPtr pScrn, int x, int y,
					  int w, int h, int skipleft)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;
    int pad;

    switch (pScrn->bitsPerPixel) {
    case 8:
	pad = 3;
	break;
    case 16:
	pad = 1;
	break;
    case 32:
	pad = 0;
	break;
    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "%s: unhandled bpp: %d\n",
		   __func__, pScrn->bitsPerPixel);
	pad = 0;
    }

    XaaPrivate->scanline_x = x;
    XaaPrivate->scanline_y = y;
    /* Have to pad the width here and use clipping engine */
    XaaPrivate->scanline_w = (w + pad) & ~pad;
    XaaPrivate->scanline_h = h;

    XaaPrivate->scanline_x1clip = x + skipleft;
    XaaPrivate->scanline_x2clip = x + w;

    XaaPrivate->scanline_words = (w * XaaPrivate->scanline_bpp + 31) / 32;
    XaaPrivate->scanline_hpass =
	min(XaaPrivate->scanline_h, ((CS->Size - 10) / XaaPrivate->scanline_words));

    R5xxXAACPScanlinePacket(CS, XaaPrivate);
}

/*
 *
 */
static void
R5xxXAAFunctionsInit(ScrnInfoPtr pScrn, ScreenPtr pScreen, XAAInfoRecPtr XAAInfo)
{
    struct R5xxXaaPrivate *XaaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    struct RhdCS *CS = RHDPTR(pScrn)->CS;

    RHDFUNC(pScrn);

    XAAInfo->Flags = PIXMAP_CACHE | OFFSCREEN_PIXMAPS | LINEAR_FRAMEBUFFER;

    /* Sync */
    XAAInfo->Sync = R5xxXAAIdle;

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

    /* In the MMIO Case, we buffer the scanline */
    if (CS->Type == RHD_CS_MMIO) {
	if (!XaaPrivate->Buffer)
	    XaaPrivate->Buffer = xnfcalloc(1, ((pScrn->virtualX + 31) / 32 * 4) +
					   (pScrn->virtualX * (pScrn->bitsPerPixel / 8)));
	XaaPrivate->BufferHook[0] = XaaPrivate->Buffer;
    }

    /* Indirect CPU-To-Screen Color Expand
     *
     * R5XX gets upset, when using HOST provided data without a source rop.
     * To show run 'xtest's drwarc.
     */
    XAAInfo->ScanlineCPUToScreenColorExpandFillFlags =
	LEFT_EDGE_CLIPPING | ROP_NEEDS_SOURCE | LEFT_EDGE_CLIPPING_NEGATIVE_X;
    XAAInfo->NumScanlineColorExpandBuffers = 1;
    XAAInfo->ScanlineColorExpandBuffers = XaaPrivate->BufferHook;

    if (CS->Type == RHD_CS_MMIO) {
	XAAInfo->SetupForScanlineCPUToScreenColorExpandFill
	    = R5xxXAASetupForScanlineCPUToScreenColorExpandFillMMIO;
	XAAInfo->SubsequentScanlineCPUToScreenColorExpandFill
	    = R5xxXAASubsequentScanlineCPUToScreenColorExpandFillMMIO;
	XAAInfo->SubsequentColorExpandScanline = R5xxXAASubsequentScanlineMMIO;
    } else {
	XAAInfo->SetupForScanlineCPUToScreenColorExpandFill
	    = R5xxXAASetupForScanlineCPUToScreenColorExpandFillCP;
	XAAInfo->SubsequentScanlineCPUToScreenColorExpandFill
	    = R5xxXAASubsequentScanlineCPUToScreenColorExpandFillCP;
	XAAInfo->SubsequentColorExpandScanline = R5xxXAASubsequentScanlineCP;
    }

    /* ImageWrite */
    XAAInfo->ScanlineImageWriteFlags = CPU_TRANSFER_PAD_DWORD |
	/* R5XX gets upset, when using HOST provided data without a source rop.
	   To show run 'xtest's ptimg */
	ROP_NEEDS_SOURCE |
	SCANLINE_PAD_DWORD | LEFT_EDGE_CLIPPING | LEFT_EDGE_CLIPPING_NEGATIVE_X;

    /* Performance tests show that we shouldn't use MMIOed GXcopy for uploads
       as a memcpy is faster */
    if (CS->Type == RHD_CS_MMIO)
	XAAInfo->ScanlineImageWriteFlags |= NO_GXCOPY;

    XAAInfo->NumScanlineImageWriteBuffers = 1;
    XAAInfo->ScanlineImageWriteBuffers = XaaPrivate->BufferHook;

    if (CS->Type == RHD_CS_MMIO) {
	XAAInfo->SetupForScanlineImageWrite = R5xxXAASetupForScanlineImageWriteMMIO;
	XAAInfo->SubsequentScanlineImageWriteRect = R5xxXAASubsequentScanlineImageWriteRectMMIO;
	XAAInfo->SubsequentImageWriteScanline = R5xxXAASubsequentScanlineMMIO;
    } else {
	XAAInfo->SetupForScanlineImageWrite = R5xxXAASetupForScanlineImageWriteCP;
	XAAInfo->SubsequentScanlineImageWriteRect = R5xxXAASubsequentScanlineImageWriteRectCP;
	XAAInfo->SubsequentImageWriteScanline = R5xxXAASubsequentScanlineCP;
    }
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
static void
R5xxXaaPrivateInit(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct R5xxXaaPrivate *XaaPrivate =
	xnfcalloc(1, sizeof(struct R5xxXaaPrivate));

    XaaPrivate->control =
	(R5xx2DDatatypeGet(pScrn) << R5XX_GMC_DST_DATATYPE_SHIFT) |
	R5XX_GMC_CLR_CMP_CNTL_DIS | R5XX_GMC_DST_PITCH_OFFSET_CNTL;

    XaaPrivate->dst_pitch_offset =
	(((pScrn->displayWidth * (pScrn->bitsPerPixel / 8)) / 64) << 22) |
	((rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart) >> 10);

    rhdPtr->TwoDPrivate = XaaPrivate;
}

/*
 *
 */
static void
R5xxXaaPrivateDestroy(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct R5xxXaaPrivate *XaaPrivate = rhdPtr->TwoDPrivate;

    if (!XaaPrivate)
	return;

    if (XaaPrivate->Buffer)
	xfree(XaaPrivate->Buffer);

    xfree(XaaPrivate);
    rhdPtr->TwoDPrivate = NULL;
}

/*
 *
 */
Bool
R5xxXAAInit(ScrnInfoPtr pScrn, ScreenPtr pScreen)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    XAAInfoRecPtr XAAInfo;

    XAAInfo = XAACreateInfoRec();
    if (!XAAInfo) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "%s: XAACreateInfoRec failed.\n", __func__);
	return FALSE;
    }

    /* need to do this before FunctionsInit */
    R5xxXaaPrivateInit(pScrn);

    R5xxXAAFunctionsInit(pScrn, pScreen, XAAInfo);

    R5xxXAAFBInit(pScrn, pScreen);

    if (!XAAInit(pScreen, XAAInfo)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "%s: XAAInit failed.\n",
		   __func__);
	XAADestroyInfoRec(XAAInfo);
	R5xxXaaPrivateDestroy(pScrn);
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

    if (rhdPtr->XAAInfo) {
	XAADestroyInfoRec(rhdPtr->XAAInfo);
	rhdPtr->XAAInfo = NULL;
    }

    R5xxXaaPrivateDestroy(pScrn);
}
