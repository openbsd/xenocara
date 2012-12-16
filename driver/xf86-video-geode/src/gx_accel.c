/* Copyright (c) 2003-2005 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Neither the name of the Advanced Micro Devices, Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 * */

/*
 * File Contents:   This file is consists of main Xfree acceleration supported
 *                  routines like solid fill used here.
 *
 * Project:         Geode Xfree Frame buffer device driver.
 * */

/* #undef OPT_ACCEL */

/* Xfree86 header files */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "vgaHW.h"
#include "xf86.h"
#ifdef HAVE_XAA_H
#include "xaalocal.h"
#endif
#include "xf86fbman.h"
#include "miline.h"
#include "xaarop.h"
#include "servermd.h"
#include "picture.h"
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Pci.h"
#include "xf86PciInfo.h"
#include "geode.h"
#include "gfx_defs.h"
#include "gfx_regs.h"

/* Common macros for blend operations are here */

#include "geode_blend.h"

#undef ulong
typedef unsigned long ulong;

#undef uint
typedef unsigned int uint;

#undef ushort
typedef unsigned short ushort;

#undef uchar
typedef unsigned char uchar;

#define CALC_FBOFFSET(x, y) \
	        (((ulong)(y) * gu2_pitch + ((ulong)(x) << gu2_xshift)))

#define FBADDR(x,y)				\
		((unsigned char *)pGeode->FBBase + CALC_FBOFFSET(x, y))

#define OS_UDELAY 0
#if OS_UDELAY > 0
#define OS_USLEEP(usec) usleep(usec);
#else
#define OS_USLEEP(usec)
#endif

#ifdef OPT_ACCEL
static unsigned int BPP;
static unsigned int BLT_MODE, VEC_MODE;
static unsigned int ACCEL_STRIDE;

#define GU2_WAIT_PENDING while(READ_GP32(MGP_BLT_STATUS) & MGP_BS_BLT_PENDING)
#define GU2_WAIT_BUSY    while(READ_GP32(MGP_BLT_STATUS) & MGP_BS_BLT_BUSY)
#endif

#define HOOK(fn) localRecPtr->fn = GX##fn

#define DLOG(l, fmt, args...) ErrorF(fmt, ##args)

/* static storage declarations */

typedef struct sGBltBox {
    ulong x, y;
    ulong w, h;
    ulong color;
    int bpp, transparent;
} GBltBox;

#if GX_SCANLINE_SUPPORT
static GBltBox giwr;
#endif
#if GX_CPU2SCREXP_SUPPORT
static GBltBox gc2s;
#endif
#if GX_CLREXP_8X8_PAT_SUPPORT
static ulong *gc8x8p;
#endif

#if GX_DASH_LINE_SUPPORT
typedef struct sGDashLine {
    ulong pat[2];
    int len;
    int fg;
    int bg;
} GDashLine;

static GDashLine gdln;
#endif

static unsigned int gu2_xshift, gu2_yshift;
static unsigned int gu2_pitch;

#if XF86XAA
static XAAInfoRecPtr localRecPtr;
#endif

/* pat  0xF0 */
/* src  0xCC */
/* dst  0xAA */

/* (src FUNC dst) */

static const int SDfn[16] = {
    0x00, 0x88, 0x44, 0xCC, 0x22, 0xAA, 0x66, 0xEE,
    0x11, 0x99, 0x55, 0xDD, 0x33, 0xBB, 0x77, 0xFF
};

/* ((src FUNC dst) AND pat-mask) OR (dst AND (NOT pat-mask)) */

static const int SDfn_PM[16] = {
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA,
    0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA
};

/* (pat FUNC dst) */

static const int PDfn[16] = {
    0x00, 0xA0, 0x50, 0xF0, 0x0A, 0xAA, 0x5A, 0xFA,
    0x05, 0xA5, 0x55, 0xF5, 0x0F, 0xAF, 0x5F, 0xFF
};

/* ((pat FUNC dst) AND src-mask) OR (dst AND (NOT src-mask)) */

static const int PDfn_SM[16] = {
    0x22, 0xA2, 0x62, 0xE2, 0x2A, 0xAA, 0x6A, 0xEA,
    0x26, 0xA6, 0x66, 0xE6, 0x2E, 0xAE, 0x6E, 0xEE
};

#ifdef OPT_ACCEL
static inline CARD32
amd_gx_BppToRasterMode(int bpp)
{
    switch (bpp) {
    case 16:
        return MGP_RM_BPPFMT_565;
    case 32:
        return MGP_RM_BPPFMT_8888;
    case 8:
        return MGP_RM_BPPFMT_332;
    default:
        return 0;
    }
}
#endif                          /* OPT_ACCEL */

/*----------------------------------------------------------------------------
 * GXAccelSync.
 *
 * Description  :This function is called to synchronize with the graphics
 *               engine and it waits the graphic engine is idle.  This is
 *               required before allowing direct access to the framebuffer.
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *
 * Returns              :none
 *---------------------------------------------------------------------------*/
void
GXAccelSync(ScrnInfoPtr pScrni)
{
    //ErrorF("GXAccelSync()\n");
#ifndef OPT_ACCEL
    gfx_wait_until_idle();
#else
    GU2_WAIT_BUSY;
#endif
}

#if GX_FILL_RECT_SUPPORT
/*----------------------------------------------------------------------------
 * GXSetupForSolidFill.
 *
 * Description  :The SetupFor and Subsequent SolidFill(Rect) provide
 *               filling rectangular areas of the screen with a
 *               foreground color.
 *
 * Parameters.
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *   color        int     foreground fill color
 *    rop         int     unmapped raster op
 * planemask     uint     -1 (fill) or pattern data
 *
 * Returns              :none
 *--------------------------------------------------------------------------*/
static void
GXSetupForSolidFill(ScrnInfoPtr pScrni,
                    int color, int rop, unsigned int planemask)
{
    //ErrorF("GXSetupForSolidFill(%#x,%#x,%#x)\n", color, rop, planemask);
    rop &= 0x0F;
#ifndef OPT_ACCEL
    gfx_set_solid_pattern(planemask);
    gfx_set_solid_source(color);
    gfx_set_raster_operation(planemask == ~0U ? SDfn[rop] : SDfn_PM[rop]);
#else
    {
        unsigned int ROP = BPP | (planemask == ~0U ? SDfn[rop] : SDfn_PM[rop]);

        BLT_MODE = ((ROP ^ (ROP >> 2)) & 0x33) == 0 ? MGP_BM_SRC_MONO : 0;
        if (((ROP ^ (ROP >> 1)) & 0x55) != 0)
            BLT_MODE |= MGP_BM_DST_REQ;
        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_RASTER_MODE, ROP);
        WRITE_GP32(MGP_PAT_COLOR_0, planemask);
        WRITE_GP32(MGP_SRC_COLOR_FG, color);
        WRITE_GP32(MGP_STRIDE, ACCEL_STRIDE);
    }
#endif
}

/*----------------------------------------------------------------------------
 * GXSubsequentSolidFillRect.
 *
 * Description  :see GXSetupForSolidFill.
 *
 * Parameters.
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *     x          int     destination x offset
 *     y          int     destination y offset
 *     w          int     fill area width (pixels)
 *     h          int     fill area height (pixels)
 *
 * Returns      :none
 *
 * Sample application uses:
 *   - Window backgrounds.
 *   - pull down highlighting.
 *   - x11perf: rectangle tests (-rect500).
 *   - x11perf: fill trapezoid tests (-trap100).
 *   - x11perf: horizontal line segments (-hseg500).
 *----------------------------------------------------------------------------*/
static void
GXSubsequentSolidFillRect(ScrnInfoPtr pScrni, int x, int y, int w, int h)
{
    //ErrorF("GXSubsequentSolidFillRect() at %d,%d %dx%d\n", x, y, w, h);
#ifndef OPT_ACCEL
    gfx_pattern_fill(x, y, w, h);
#else
    {
        unsigned int offset = CALC_FBOFFSET(x, y);
        unsigned int size = (w << 16) | h;

        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_DST_OFFSET, offset);
        WRITE_GP32(MGP_WID_HEIGHT, size);
        WRITE_GP32(MGP_BLT_MODE, BLT_MODE);
    }
#endif
}

#endif                          /* if GX_FILL_RECT_SUPPORT */

#if GX_CLREXP_8X8_PAT_SUPPORT
/*----------------------------------------------------------------------------
 * GXSetupForColor8x8PatternFill
 *
 * Description  :8x8 color pattern data is 64 pixels of full color data
 *               stored linearly in offscreen video memory.  These patterns
 *               are useful as a substitute for 8x8 mono patterns when tiling,
 *               doing opaque stipples, or regular stipples.
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *    patx        int     x offset to pattern data
 *    paty        int     y offset to pattern data
 *    rop         int     unmapped raster operation
 * planemask     uint     -1 (copy) or pattern data
 * trans_color    int     -1 (copy) or transparent color (not enabled)
 *                         trans color only supported on source channel
 *                         or in monochrome pattern channel
 *
 * Returns      :none.
 *
 *---------------------------------------------------------------------------*/

static void
GXSetupForColor8x8PatternFill(ScrnInfoPtr pScrni, int patx, int paty, int rop,
                              uint planemask, int trans_color)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);

    //ErrorF("GXSetupForColor8x8PatternFill() pat %#x,%#x rop %#x %#x %#x\n",
    //    patx, paty, rop, planemask, trans_color);
    rop &= 0x0F;
    gc8x8p = (unsigned long *) FBADDR(patx, paty);
    /* gfx_set_solid_pattern is needed to clear src/pat transparency */
    gfx_set_solid_pattern(0);
    gfx_set_raster_operation(planemask == ~0U ? PDfn[rop] :
                             (gfx_set_solid_source(planemask), PDfn_SM[rop]));
    gfx2_set_source_stride(pGeode->Pitch);
    gfx2_set_destination_stride(pGeode->Pitch);
    if (trans_color == -1)
        gfx2_set_source_transparency(0, 0);
    else
        gfx2_set_source_transparency(trans_color, ~0);
}

/*----------------------------------------------------------------------------
 * GXSubsequentColor8x8PatternFillRect
 *
 * Description  :see GXSetupForColor8x8PatternFill.
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *   patx         int     pattern phase x offset
 *   paty         int     pattern phase y offset
 *      x         int     destination x offset
 *      y         int     destination y offset
 *      w         int     fill area width (pixels)
 *      h         int     fill area height (pixels)
 *
 * Returns      :none
 *
 * Sample application uses:
 *   - Patterned desktops
 *   - x11perf: stippled rectangle tests (-srect500).
 *   - x11perf: opaque stippled rectangle tests (-osrect500).
 *--------------------------------------------------------------------------*/
static void
GXSubsequentColor8x8PatternFillRect(ScrnInfoPtr pScrni, int patx, int paty,
                                    int x, int y, int w, int h)
{
    //ErrorF(
    //    "GXSubsequentColor8x8PatternFillRect() patxy %d,%d at %d,%d %dsx%d\n",
    //    patx, paty, x, y, w, h);
    gfx2_set_pattern_origin(patx, paty);
    gfx2_color_pattern_fill(CALC_FBOFFSET(x, y), w, h, gc8x8p);
}

/* GX_CLREXP_8X8_PAT_SUPPORT */
#endif

#if GX_MONO_8X8_PAT_SUPPORT
/*----------------------------------------------------------------------------
 * GXSetupForMono8x8PatternFill
 *
 * Description  :8x8 mono pattern data is 64 bits of color expansion data
 *               with ones indicating the foreground color and zeros
 *               indicating the background color.  These patterns are
 *               useful when tiling, doing opaque stipples, or regular
 *               stipples.
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *    patx        int     x offset to pattern data
 *    paty        int     y offset to pattern data
 *     fg         int     foreground color
 *     bg         int     -1 (transparent) or background color
 *    rop         int     unmapped raster operation
 * planemask     uint     -1 (copy) or pattern data
 *
 * Returns      :none.
 *
 * Comments     :none.
 *
 *--------------------------------------------------------------------------*/
static void
GXSetupForMono8x8PatternFill(ScrnInfoPtr pScrni, int patx, int paty,
                             int fg, int bg, int rop, uint planemask)
{
    //ErrorF(
    //"GXSetupForMono8x8PatternFill() pat %#x,%#x fg %#x bg %#x %#x %#x\n",
    //patx, paty, fg, bg, rop, planemask);
    rop &= 0x0F;
#ifndef OPT_ACCEL
    gfx_set_mono_pattern(bg, fg, patx, paty, bg == -1 ? 1 : 0);
    gfx_set_raster_operation(planemask == ~0U ? PDfn[rop] :
                             (gfx_set_solid_source(planemask), PDfn_SM[rop]));
#else
    {
        unsigned int ROP = BPP |
            (bg ==
             -1 ? MGP_RM_PAT_MONO | MGP_RM_PAT_TRANS : MGP_RM_PAT_MONO) |
            (planemask == ~0U ? PDfn[rop] : PDfn_SM[rop]);
        BLT_MODE = ((ROP ^ (ROP >> 2)) & 0x33) == 0 ? MGP_BM_SRC_MONO : 0;
        if (((ROP ^ (ROP >> 1)) & 0x55) != 0)
            BLT_MODE |= MGP_BM_DST_REQ;
        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_RASTER_MODE, ROP);
        WRITE_GP32(MGP_SRC_COLOR_FG, planemask);
        WRITE_GP32(MGP_PAT_COLOR_0, bg);
        WRITE_GP32(MGP_PAT_COLOR_1, fg);
        WRITE_GP32(MGP_PAT_DATA_0, patx);
        WRITE_GP32(MGP_PAT_DATA_1, paty);
        WRITE_GP32(MGP_STRIDE, ACCEL_STRIDE);
    }
#endif
}

/*----------------------------------------------------------------------------
 * GXSubsequentMono8x8PatternFillRect
 *
 * Description  :see GXSetupForMono8x8PatternFill
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *   patx         int     pattern phase x offset
 *   paty         int     pattern phase y offset
 *      x         int     destination x offset
 *      y         int     destination y offset
 *      w         int     fill area width (pixels)
 *      h         int     fill area height (pixels)

 * Returns      :none
 *
 * Sample application uses:
 *   - Patterned desktops
 *   - x11perf: stippled rectangle tests (-srect500).
 *   - x11perf: opaque stippled rectangle tests (-osrect500).
 *--------------------------------------------------------------------------*/
static void
GXSubsequentMono8x8PatternFillRect(ScrnInfoPtr pScrni, int patx, int paty,
                                   int x, int y, int w, int h)
{
    DEBUGMSG(1, (0, X_INFO, "%s() pat %#x,%#x at %d,%d %dx%d\n",
                 __func__, patx, paty, x, y, w, h));
#ifndef OPT_ACCEL
    gfx_pattern_fill(x, y, w, h);
#else
    {
        unsigned int offset =
            CALC_FBOFFSET(x, y) | ((x & 7) << 26) | ((y & 7) << 29);
        unsigned int size = (w << 16) | h;

        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_DST_OFFSET, offset);
        WRITE_GP32(MGP_WID_HEIGHT, size);
        WRITE_GP32(MGP_BLT_MODE, BLT_MODE);
    }
#endif
}

#endif                          /* GX_MONO_8X8_PAT_SUPPORT */

#if GX_SCR2SCRCPY_SUPPORT
/*----------------------------------------------------------------------------
 * GXSetupForScreenToScreenCopy
 *
 * Description  :SetupFor and Subsequent ScreenToScreenCopy functions
 *               provide an interface for copying rectangular areas from
 *               video memory to video memory.
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *   xdir         int     x copy direction (up/dn)
 *   ydir         int     y copy direction (up/dn)
 *    rop         int     unmapped raster operation
 * planemask     uint     -1 (copy) or pattern data
 * trans_color    int     -1 (copy) or transparent color
 *
 * Returns      :none
 *---------------------------------------------------------------------------*/
static void
GXSetupForScreenToScreenCopy(ScrnInfoPtr pScrni, int xdir, int ydir, int rop,
                             uint planemask, int trans_color)
{
    DEBUGMSG(1, (0, X_INFO, "%s() xd%d yd%d rop %#x %#x %#x\n",
                 __func__, xdir, ydir, rop, planemask, trans_color));
    rop &= 0x0F;
#ifndef OPT_ACCEL
    {
        GeodeRec *pGeode = GEODEPTR(pScrni);

        gfx_set_solid_pattern(planemask);
        /* transparency is a parameter to set_rop, but set...pattern clears
         * transparency */
        if (trans_color == -1)
            gfx2_set_source_transparency(0, 0);
        else
            gfx2_set_source_transparency(trans_color, ~0);
        gfx_set_raster_operation(planemask == ~0U ? SDfn[rop] : SDfn_PM[rop]);
        gfx2_set_source_stride(pGeode->Pitch);
        gfx2_set_destination_stride(pGeode->Pitch);
    }
#else
    {
        unsigned int ROP = BPP | (planemask == ~0U ? SDfn[rop] : SDfn_PM[rop]);

        if (trans_color != -1)
            ROP |= MGP_RM_SRC_TRANS;
        BLT_MODE = ((ROP ^ (ROP >> 1)) & 0x55) != 0 ?
            MGP_BM_SRC_FB | MGP_BM_DST_REQ : MGP_BM_SRC_FB;
        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_RASTER_MODE, ROP);
        WRITE_GP32(MGP_PAT_COLOR_0, planemask);
        WRITE_GP32(MGP_SRC_COLOR_FG, trans_color);
        WRITE_GP32(MGP_SRC_COLOR_BG, ~0);
        WRITE_GP32(MGP_STRIDE, ACCEL_STRIDE);
    }
#endif
}

/*----------------------------------------------------------------------------
 * GXSubsquentScreenToScreenCopy
 *
 * Description  :see GXSetupForScreenToScreenCopy.
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *     x1         int     source x offset
 *     y1         int     source y offset
 *     x2         int     destination x offset
 *     y2         int     destination y offset
 *      w         int     copy area width (pixels)
 *      h         int     copy area height (pixels)
 *
 * Returns      :none
 *
 * Sample application uses (non-transparent):
 *   - Moving windows.
 *   - x11perf: scroll tests (-scroll500).
 *   - x11perf: copy from window to window (-copywinwin500).
 *---------------------------------------------------------------------------*/
static void
GXSubsequentScreenToScreenCopy(ScrnInfoPtr pScrni,
                               int x1, int y1, int x2, int y2, int w, int h)
{
    DEBUGMSG(1, (0, X_INFO, "%s() from %d,%d to %d,%d %dx%d\n",
                 __func__, x1, y1, x2, y2, w, h));
#ifndef OPT_ACCEL
    {
        int flags = 0;

        if (x2 > x1)
            flags |= 1;
        if (y2 > y1)
            flags |= 2;
        gfx2_screen_to_screen_blt(CALC_FBOFFSET(x1, y1), CALC_FBOFFSET(x2,
                                                                       y2), w,
                                  h, flags);
    }
#else
    {
        GeodeRec *pGeode = GEODEPTR(pScrni);
        unsigned int src = CALC_FBOFFSET(x1, y1);
        unsigned int dst = CALC_FBOFFSET(x2, y2);
        unsigned int size = (w << 16) | h;
        unsigned int blt_mode = BLT_MODE;

        if (x2 > x1) {
            int n = (w << gu2_xshift) - 1;

            src += n;
            dst += n;
            blt_mode |= MGP_BM_NEG_XDIR;
        }
        if (y2 > y1) {
            int n = (h - 1) * pGeode->Pitch;

            src += n;
            dst += n;
            blt_mode |= MGP_BM_NEG_YDIR;
        }
        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_SRC_OFFSET, src);
        WRITE_GP32(MGP_DST_OFFSET, dst);
        WRITE_GP32(MGP_WID_HEIGHT, size);
        WRITE_GP16(MGP_BLT_MODE, blt_mode);
    }
#endif
}

#endif                          /* if GX_SCR2SCRCPY_SUPPORT */

#if GX_SCANLINE_SUPPORT
/*----------------------------------------------------------------------------
 * GXSetupForScanlineImageWrite
 *
 * Description  :SetupFor/Subsequent ScanlineImageWrite and ImageWriteScanline
 *               transfer full color pixel data from system memory to video
 *               memory.  This is useful for dealing with alignment issues and
 *               performing raster ops on the data.
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *    rop         int     unmapped raster operation
 * planemask     uint     -1 (copy) or pattern data
 *    bpp         int     bits per pixel (unused)
 *  depth         int     color depth (unused)
 *
 * Returns      :none
 *
 *  x11perf -putimage10
 *  x11perf -putimage100
 *  x11perf -putimage500
 *----------------------------------------------------------------------------
 */
static void
GXSetupForScanlineImageWrite(ScrnInfoPtr pScrni, int rop, uint planemask,
                             int trans_color, int bpp, int depth)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);

    DEBUGMSG(1, (0, X_INFO, "%s() rop %#x %#x %#x %d %d\n",
                 __func__, rop, planemask, trans_color, bpp, depth));
    rop &= 0x0F;
    /* transparency is a parameter to set_rop, but set...pattern clears
     * transparency */
    gfx_set_solid_pattern(planemask);
    if (trans_color == -1)
        gfx2_set_source_transparency(0, 0);
    else
        gfx2_set_source_transparency(trans_color, ~0);
    gfx_set_raster_operation(planemask == ~0U ? SDfn[rop] : SDfn_PM[rop]);
    gfx2_set_source_stride(pGeode->Pitch);
    gfx2_set_destination_stride(pGeode->Pitch);
}

/*----------------------------------------------------------------------------
 * GXSubsequentScanlineImageWriteRect
 *
 * Description  : see GXSetupForScanlineImageWrite.
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *      x         int     destination x offset
 *      y         int     destination y offset
 *      w         int     copy area width (pixels)
 *      h         int     copy area height (pixels)
 * skipleft       int     x margin (pixels) to skip (not enabled)
 *
 * Returns      :none
 *---------------------------------------------------------------------------*/
static void
GXSubsequentScanlineImageWriteRect(ScrnInfoPtr pScrni,
                                   int x, int y, int w, int h, int skipleft)
{
    DEBUGMSG(1, (0, X_INFO, "%s() rop %d,%d %dx%d %d\n",
                 __func__, x, y, w, h, skipleft));
    giwr.x = x;
    giwr.y = y;
    giwr.w = w;
    giwr.h = h;
#if !GX_USE_OFFSCRN_MEM
#if !GX_ONE_LINE_AT_A_TIME
    GXAccelSync(pScrni);
#endif
#endif
}

/*----------------------------------------------------------------------------
 * GXSubsquentImageWriteScanline
 *
 * Description  : see GXSetupForScanlineImageWrite.
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *  bufno         int     scanline number in write group
 *
 * Returns      :none
 *
 * Sample application uses (non-transparent):
 *   - Moving windows.
 *   - x11perf: scroll tests (-scroll500).
 *   - x11perf: copy from window to window (-copywinwin500).
 *
 *---------------------------------------------------------------------------*/
static void
GXSubsequentImageWriteScanline(ScrnInfoPtr pScrni, int bufno)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);

#if !GX_USE_OFFSCRN_MEM
    unsigned long offset;
#endif

#if GX_ONE_LINE_AT_A_TIME
    DEBUGMSG(1, (0, X_INFO, "%s() %d\n", __func__, bufno));
#if !GX_USE_OFFSCRN_MEM
    offset = pGeode->AccelImageWriteBuffers[bufno] - pGeode->FBBase;
    gfx2_screen_to_screen_blt(offset, CALC_FBOFFSET(giwr.x, giwr.y), giwr.w,
                              1, 0);
#else                           /* if !GX_USE_OFFSCRN_MEM */
    gfx2_color_bitmap_to_screen_blt(0, 0, CALC_FBOFFSET(giwr.x, giwr.y),
                                    giwr.w, 1,
                                    pGeode->AccelImageWriteBuffers[bufno],
                                    pGeode->Pitch);
#endif                          /* if !GX_USE_OFFSCRN_MEM */
    ++giwr.y;
#else                           /* if GX_ONE_LINE_AT_A_TIME */
    int blt_height;

    DEBUGMSG(1, (0, X_INFO, "%s() %d\n", __func__, bufno));

    if ((blt_height = pGeode->NoOfImgBuffers) > giwr.h)
        blt_height = giwr.h;
    if (++bufno < blt_height)
        return;
#if !GX_USE_OFFSCRN_MEM
    offset = pGeode->AccelImageWriteBuffers[0] - pGeode->FBBase;
    gfx2_screen_to_screen_blt(offset, CALC_FBOFFSET(giwr.x, giwr.y), giwr.w,
                              blt_height, 0);
    GXAccelSync(pScrni);
#else                           /* if !GX_USE_OFFSCRN_MEM */
    gfx2_color_bitmap_to_screen_blt(0, 0, CALC_FBOFFSET(giwr.x, giwr.y),
                                    giwr.w, blt_height,
                                    pGeode->AccelImageWriteBuffers[0],
                                    pGeode->Pitch);
#endif                          /* if !GX_USE_OFFSCRN_MEM */
    giwr.h -= blt_height;
    giwr.y += blt_height;
#endif                          /* if GX_ONE_LINE_AT_A_TIME */
}
#endif                          /* GX_SCANLINE_SUPPORT */

#if GX_CPU2SCREXP_SUPPORT
/*----------------------------------------------------------------------------
 * GXSetupForScanlineCPUToScreenColorExpandFill
 *
 * Description  :SetupFor/Subsequent CPUToScreenColorExpandFill and
 *               ColorExpandScanline routines provide an interface for
 *               doing expansion blits from source patterns stored in
 *               system memory.
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *     fg         int     foreground color
 *     bg         int     -1 (transparent) or background color
 *    rop         int     unmapped raster operation
 * planemask     uint     -1 (copy) or pattern data
 *
 * Returns      :none.
 *---------------------------------------------------------------------------*/

static void
GXSetupForScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrni,
                                             int fg, int bg, int rop,
                                             uint planemask)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);
    ulong srcpitch;

    DEBUGMSG(1, (0, X_INFO, "%s() fg %#x bg %#x rop %#x %#x\n",
                 __func__, fg, bg, rop, planemask));
    rop &= 0x0F;
    srcpitch = ((pGeode->Pitch + 31) >> 5) << 2;
#ifndef OPT_ACCEL
    gfx_set_solid_pattern(planemask);
    gfx_set_mono_source(bg, fg, bg == -1 ? 1 : 0);
    gfx_set_raster_operation(planemask == ~0U ? SDfn[rop] : SDfn_PM[rop]);
    gfx2_set_source_stride(srcpitch);
    gfx2_set_destination_stride(pGeode->Pitch);
#else
    {
        unsigned int stride = (srcpitch << 16) | pGeode->Pitch;
        unsigned int ROP = BPP | (planemask == ~0U ? SDfn[rop] : SDfn_PM[rop]);

        if (bg == -1)
            ROP |= MGP_RM_SRC_TRANS;
        BLT_MODE = ((ROP ^ (ROP >> 1)) & 0x55) != 0 ?
            MGP_BM_SRC_MONO | MGP_BM_SRC_FB | MGP_BM_DST_REQ :
            MGP_BM_SRC_MONO | MGP_BM_SRC_FB;
        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_RASTER_MODE, ROP);
        WRITE_GP32(MGP_PAT_COLOR_0, planemask);
        WRITE_GP32(MGP_SRC_COLOR_BG, bg);
        WRITE_GP32(MGP_SRC_COLOR_FG, fg);
        WRITE_GP32(MGP_STRIDE, stride);
    }
#endif
}

/*----------------------------------------------------------------------------
 * GXSubsequentScanlineCPUToScreenColorExpandFill
 *
  Description  :see GXSetupForScanlineCPUToScreenColorExpandFill
 *
 * Parameters:
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *     x          int     destination x offset
 *     y          int     destination y offset
 *     w          int     fill area width (pixels)
 *     h          int     fill area height (pixels)
 *
 * Returns      :none
 *
 *---------------------------------------------------------------------------*/
static void
GXSubsequentScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrni,
                                               int x, int y, int w, int h,
                                               int skipleft)
{
    DEBUGMSG(1, (0, X_INFO, "%s() %d,%d %dx%d %d\n",
                 __func__, x, y, w, h, skipleft));
    gc2s.x = x;
    gc2s.y = y;
    gc2s.w = w;
    gc2s.h = h;
#ifdef OPT_ACCEL
    {
#if GX_ONE_LINE_AT_A_TIME
        unsigned int size = (gc2s.w << 16) | 1;

        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_WID_HEIGHT, size);
#else
        GeodeRec *pGeode = GEODEPTR(pScrni);
        unsigned int src = pGeode->AccelColorExpandBuffers[0] - pGeode->FBBase;

        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_SRC_OFFSET, src);
#endif
    }
#endif
}

/*----------------------------------------------------------------------------
 * GXSubsequentColorExpandScanline
 *
 * Description  :see GXSetupForScanlineCPUToScreenColorExpandFill
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *  bufno         int     scanline number in write group
 *
 * Returns      :none
 *----------------------------------------------------------------------------
 */
static void
GXSubsequentColorExpandScanline(ScrnInfoPtr pScrni, int bufno)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);

    DEBUGMSG(1, (0, X_INFO, "%s() %d\n", __func__, bufno));
#ifndef OPT_ACCEL
    {
#if GX_ONE_LINE_AT_A_TIME
        ulong offset = pGeode->AccelColorExpandBuffers[bufno] - pGeode->FBBase;

        gfx2_mono_expand_blt(offset, 0, 0, CALC_FBOFFSET(gc2s.x, gc2s.y),
                             gc2s.w, 1, 0);
        ++gc2s.y;
#else                           /* if GX_ONE_LINE_AT_A_TIME */
        ulong srcpitch;
        int blt_height;

        if ((blt_height = pGeode->NoOfImgBuffers) > gc2s.h)
            blt_height = gc2s.h;
        if (++bufno < blt_height)
            return;

        /* convert from bits to dwords */
        srcpitch = ((pGeode->Pitch + 31) >> 5) << 2;
        gfx2_mono_bitmap_to_screen_blt(0, 0, CALC_FBOFFSET(gc2s.x, gc2s.y),
                                       gc2s.w, blt_height,
                                       pGeode->AccelColorExpandBuffers[0],
                                       srcpitch);
        gc2s.h -= blt_height;
        gc2s.y += blt_height;
#endif                          /* if GX_ONE_LINE_AT_A_TIME */
    }
#else                           /* ifndef OPT_ACCEL */
    {
#if GX_ONE_LINE_AT_A_TIME
        unsigned int src =
            pGeode->AccelColorExpandBuffers[bufno] - pGeode->FBBase;
        unsigned int dst = CALC_FBOFFSET(gc2s.x, gc2s.y);

        ++gc2s.y;
        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_SRC_OFFSET, src);
        WRITE_GP32(MGP_DST_OFFSET, dst);
        WRITE_GP16(MGP_BLT_MODE, BLT_MODE);
#else                           /* if GX_ONE_LINE_AT_A_TIME */
        unsigned int dst, size;
        int blt_height;

        GU2_WAIT_BUSY;
        if ((blt_height = pGeode->NoOfImgBuffers) > gc2s.h)
            blt_height = gc2s.h;
        if (++bufno < blt_height)
            return;
        dst = CALC_FBOFFSET(gc2s.x, gc2s.y);
        size = (gc2s.w << 16) | blt_height;
        gc2s.h -= blt_height;
        gc2s.y += blt_height;
        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_DST_OFFSET, dst);
        WRITE_GP32(MGP_WID_HEIGHT, size);
        WRITE_GP16(MGP_BLT_MODE, BLT_MODE);
#endif                          /* if GX_ONE_LINE_AT_A_TIME */
    }
#endif                          /* ifndef OPT_ACCEL */
}
#endif                          /* GX_CPU2SCREXP_SUPPORT */

#if GX_SCR2SCREXP_SUPPORT
/*----------------------------------------------------------------------------
 * GXSetupForScreenToScreenColorExpandFill
 *
 * Description  :SetupFor/Subsequent ScreenToScreenColorExpandFill and
 *               ColorExpandScanline routines provide an interface for
 *               doing expansion blits from source patterns stored in
 *               video memory.
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *     fg         int     foreground color
 *     bg         int     -1 (transparent) or background color
 *    rop         int     unmapped raster operation
 * planemask     uint     -1 (copy) or pattern data
 *
 * Returns      :none.
 *---------------------------------------------------------------------------*/

static void
GXSetupForScreenToScreenColorExpandFill(ScrnInfoPtr pScrni, int fg, int bg,
                                        int rop, uint planemask)
{
    DEBUGMSG(1, (0, X_INFO, "%s() fg %#x bg %#x rop %#x %#x\n",
                 __func__, fg, bg, rop, planemask));
    rop &= 0x0F;
#ifndef OPT_ACCEL
    {
        GeodeRec *pGeode = GEODEPTR(pScrni);

        gfx_set_solid_pattern(planemask);
        gfx_set_mono_source(bg, fg, bg == -1 ? 1 : 0);
        gfx_set_raster_operation(planemask == ~0U ? SDfn[rop] : SDfn_PM[rop]);
        gfx2_set_source_stride(pGeode->Pitch);
        gfx2_set_destination_stride(pGeode->Pitch);
    }
#else
    {
        unsigned int ROP = BPP | (planemask == ~0U ? SDfn[rop] : SDfn_PM[rop]);

        if (bg == -1)
            ROP |= MGP_RM_SRC_TRANS;
        BLT_MODE = ((ROP ^ (ROP >> 1)) & 0x55) != 0 ?
            MGP_BM_SRC_MONO | MGP_BM_SRC_FB | MGP_BM_DST_REQ :
            MGP_BM_SRC_MONO | MGP_BM_SRC_FB;
        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_RASTER_MODE, ROP);
        WRITE_GP32(MGP_PAT_COLOR_0, planemask);
        WRITE_GP32(MGP_SRC_COLOR_BG, bg);
        WRITE_GP32(MGP_SRC_COLOR_FG, fg);
        WRITE_GP32(MGP_STRIDE, ACCEL_STRIDE);
    }
#endif
}

/*----------------------------------------------------------------------------
 * GXSubsequentScreenToScreenColorExpandFill
 *
 * Description  :see GXSetupForScreenToScreenColorExpandFill
 *
 * Parameters:
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *     x          int     destination x offset
 *     y          int     destination y offset
 *     w          int     fill area width (pixels)
 *     h          int     fill area height (pixels)
 * offset         int     initial x offset
 *
 * Returns      :none
 *
 *---------------------------------------------------------------------------*/
static void
GXSubsequentScreenToScreenColorExpandFill(ScrnInfoPtr pScrni,
                                          int x, int y, int w, int h, int srcx,
                                          int srcy, int offset)
{
    DEBUGMSG(1, (0, X_INFO, "%s() %d,%d %dx%d %d,%d %d\n",
                 __func__, x, y, w, h, srcx, srcy, offset));
#ifndef OPT_ACCEL
    gfx2_mono_expand_blt(CALC_FBOFFSET(srcx, srcy), offset, 0,
                         CALC_FBOFFSET(x, y), w, h, 0);
#else
    {
        unsigned int src = (CALC_FBOFFSET(srcx,
                                          srcy) +
                            (offset >> 3)) | ((offset & 7) << 26);
        unsigned int dst = CALC_FBOFFSET(x, y);
        unsigned int size = (w << 16) | h;

        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_SRC_OFFSET, src);
        WRITE_GP32(MGP_DST_OFFSET, dst);
        WRITE_GP32(MGP_WID_HEIGHT, size);
        WRITE_GP16(MGP_BLT_MODE, BLT_MODE);
    }
#endif
}
#endif                          /* GX_SCR2SCREXP_SUPPORT */

#define VM_MAJOR_DEC 0
#define VM_MINOR_DEC 0

static unsigned short vmode[] = {
    VM_X_MAJOR | VM_MAJOR_INC | VM_MINOR_INC,
    /* !XDECREASING !YDECREASING !YMAJOR */
    VM_Y_MAJOR | VM_MAJOR_INC | VM_MINOR_INC,
    /* !XDECREASING !YDECREASING  YMAJOR */
    VM_X_MAJOR | VM_MAJOR_INC | VM_MINOR_DEC,
    /* !XDECREASING  YDECREASING !YMAJOR */
    VM_Y_MAJOR | VM_MAJOR_DEC | VM_MINOR_INC,
    /* !XDECREASING  YDECREASING  YMAJOR */
    VM_X_MAJOR | VM_MAJOR_DEC | VM_MINOR_INC,
    /*  XDECREASING !YDECREASING !YMAJOR */
    VM_Y_MAJOR | VM_MAJOR_INC | VM_MINOR_DEC,
    /*  XDECREASING !YDECREASING  YMAJOR */
    VM_X_MAJOR | VM_MAJOR_DEC | VM_MINOR_DEC,
    /*  XDECREASING  YDECREASING !YMAJOR */
    VM_Y_MAJOR | VM_MAJOR_DEC | VM_MINOR_DEC,
    /*  XDECREASING  YDECREASING  YMAJOR */
};

#if GX_BRES_LINE_SUPPORT
/*----------------------------------------------------------------------------
 * GXSetupForSolidLine
 *
 * Description  :SetupForSolidLine and Subsequent HorVertLine TwoPointLine
 *               BresenhamLine provides an interface for drawing thin
 *               solid lines.
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *   color        int     foreground fill color
 *    rop         int     unmapped raster op
 * planemask     uint     -1 (fill) or pattern data (not enabled)
 *
 * Returns		:none
 *---------------------------------------------------------------------------*/
static void
GXSetupForSolidLine(ScrnInfoPtr pScrni, int color, int rop, uint planemask)
{
    DEBUGMSG(1, (0, X_INFO, "%s() %#x %#x %#x\n",
                 __func__, color, rop, planemask));
    rop &= 0x0F;
#ifndef OPT_ACCEL
    gfx_set_solid_pattern(color);
    gfx_set_raster_operation(planemask == ~0U ? PDfn[rop] :
                             (gfx_set_solid_source(planemask), PDfn_SM[rop]));
#else
    {
        unsigned int ROP = BPP | (planemask == ~0U ? PDfn[rop] : PDfn_SM[rop]);

        BLT_MODE = ((ROP ^ (ROP >> 2)) & 0x33) == 0 ? MGP_BM_SRC_MONO : 0;
        VEC_MODE = ((ROP ^ (ROP >> 1)) & 0x55) != 0 ? ((BLT_MODE |=
                                                        MGP_BM_DST_REQ),
                                                       MGP_VM_DST_REQ) : 0;
        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_RASTER_MODE, ROP);
        WRITE_GP32(MGP_PAT_COLOR_0, color);
        WRITE_GP32(MGP_SRC_COLOR_FG, planemask);
        WRITE_GP32(MGP_STRIDE, ACCEL_STRIDE);
    }
#endif
}

/*---------------------------------------------------------------------------
 * GXSubsequentSolidBresenhamLine
 *
 * Description  :see GXSetupForSolidLine
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *     x1         int     destination x offset
 *     y1         int     destination y offset
 * absmaj         int     Bresenman absolute major
 * absmin         int     Bresenman absolute minor
 *    err         int     Bresenman initial error term
 *    len         int     length of the vector (pixels)
 * octant         int     specifies sign and magnitude relationships
 *                         used to determine axis of magor rendering
 *                         and direction of vector progress.
 *
 * Returns      :none
 *
 *   - Window outlines on window move.
 *   - x11perf: line segments (-line500).
 *   - x11perf: line segments (-seg500).
 *---------------------------------------------------------------------------*/
static void
GXSubsequentSolidBresenhamLine(ScrnInfoPtr pScrni, int x1, int y1,
                               int absmaj, int absmin, int err, int len,
                               int octant)
{
    long axial, diagn;

    DEBUGMSG(1, (0, X_INFO, "%s() %d,%d %d %d, %d %d, %d\n",
                 __func__, x1, y1, absmaj, absmin, err, len, octant));
    if (len <= 0)
        return;
    axial = absmin;
    err += axial;
    diagn = absmin - absmaj;
#ifndef OPT_ACCEL
    gfx_bresenham_line(x1, y1, len, err, axial, diagn, vmode[octant]);
#else
    {
        unsigned int offset = CALC_FBOFFSET(x1, y1);
        unsigned int vec_err = (axial << 16) | (unsigned short) diagn;
        unsigned int vec_len = (len << 16) | (unsigned short) err;
        unsigned int vec_mode = VEC_MODE | vmode[octant];

        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_DST_OFFSET, offset);
        WRITE_GP32(MGP_VEC_ERR, vec_err);
        WRITE_GP32(MGP_VEC_LEN, vec_len);
        WRITE_GP32(MGP_VECTOR_MODE, vec_mode);
    }
#endif
}

/*---------------------------------------------------------------------------
 * GXSubsequentSolidTwoPointLine
 *
 * Description  :see GXSetupForSolidLine
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *     x0         int     destination x start offset
 *     y0         int     destination y start offset
 *     x1         int     destination x end offset
 *     y1         int     destination y end offset
 *  flags         int     OMIT_LAST, dont draw last pixel (not used)
 *
 * Returns      :none
 *---------------------------------------------------------------------------*/
static void
GXSubsequentSolidTwoPointLine(ScrnInfoPtr pScrni, int x0, int y0,
                              int x1, int y1, int flags)
{
    long dx, dy, dmaj, dmin, octant, bias;
    long axial, diagn, err, len;

    DEBUGMSG(1, (0, X_INFO, "%s() %d,%d %d,%d, %#x\n",
                 __func__, x0, y0, x1, y1, flags));

    if ((dx = x1 - x0) < 0)
        dx = -dx;
    if ((dy = y1 - y0) < 0)
        dy = -dy;
    if (dy >= dx) {
        dmaj = dy;
        dmin = dx;
        octant = YMAJOR;
    }
    else {
        dmaj = dx;
        dmin = dy;
        octant = 0;
    }
    len = dmaj;
    if ((flags & OMIT_LAST) == 0)
        ++len;
    if (len <= 0)
        return;
    if (x1 < x0)
        octant |= XDECREASING;
    if (y1 < y0)
        octant |= YDECREASING;

    axial = dmin << 1;
    bias = miGetZeroLineBias(pScrni->pScreen);
    err = axial - dmaj - ((bias >> octant) & 1);
    diagn = (dmin - dmaj) << 1;

#ifndef OPT_ACCEL
    gfx_bresenham_line(x0, y0, len, err, axial, diagn, vmode[octant]);
#else
    {
        unsigned int offset = CALC_FBOFFSET(x0, y0);
        unsigned int vec_err = (axial << 16) | (unsigned short) diagn;
        unsigned int vec_len = (len << 16) | (unsigned short) err;
        unsigned int vec_mode = VEC_MODE | vmode[octant];

        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_DST_OFFSET, offset);
        WRITE_GP32(MGP_VEC_ERR, vec_err);
        WRITE_GP32(MGP_VEC_LEN, vec_len);
        WRITE_GP32(MGP_VECTOR_MODE, vec_mode);
    }
#endif
}

/*---------------------------------------------------------------------------
 * GXSubsequentSolidHorVertLine
 *
 * Description  :see GXSetupForSolidLine
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *     x          int     destination x offset
 *     y          int     destination y offset
 *    len         int     length of the vector (pixels)
 *    dir         int     DEGREES_270 or DEGREES_0 line direction
 *
 * Sample application uses:
 *   - Window outlines on window move.
 *   - x11perf: line segments (-hseg500).
 *   - x11perf: line segments (-vseg500).
 *---------------------------------------------------------------------------
 */
static void
GXSubsequentSolidHorVertLine(ScrnInfoPtr pScrni, int x, int y, int len, int dir)
{
    DEBUGMSG(1, (0, X_INFO, "%s() %d,%d %d %d\n", __func__, x, y, len, dir));
#ifndef OPT_ACCEL
    if (dir == DEGREES_0)
        gfx_pattern_fill(x, y, len, 1);
    else
        gfx_pattern_fill(x, y, 1, len);
#else
    {
        unsigned int offset = CALC_FBOFFSET(x, y);
        unsigned int size =
            dir == DEGREES_0 ? (len << 16) | 1 : (1 << 16) | len;
        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_DST_OFFSET, offset);
        WRITE_GP32(MGP_WID_HEIGHT, size);
        WRITE_GP32(MGP_BLT_MODE, BLT_MODE);
    }
#endif
}
#endif                          /* GX_BRES_LINE_SUPPORT */

#if GX_DASH_LINE_SUPPORT
/*----------------------------------------------------------------------------
 * GXSetupForDashedLine
 *
 * Description  :SetupForDashedLine and Subsequent TwoPointLine
 *               BresenhamLine provides an interface for drawing thin
 *               dashed lines.
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *     fg         int     foreground fill color
 *     bg         int     -1 (transp) or background fill color
 *    rop         int     unmapped raster op
 * planemask     uint     -1 (fill) or pattern data (not enabled)
 *  length        int     pattern length (bits)
 * pattern     uchar*     dash pattern mask
 *
 * Returns              :none
 *---------------------------------------------------------------------------*/
static void
GXSetupForDashedLine(ScrnInfoPtr pScrn, int fg, int bg, int rop,
                     unsigned int planemask, int length, unsigned char *pattern)
{
    int i, l, n, m;
    CARD32 pat = *pattern;
    CARD32 pat8x8[2];

    if (length <= 0)
        return;
    i = l = m = 0;
    while (i < 2) {
        m |= pat >> l;
        l += length;
        if ((n = l - 32) >= 0) {
            pat8x8[i++] = m;
            m = pat << (length - n);
            l = n;
        }
    }
    gdln.pat[0] = pat8x8[0];
    gdln.pat[1] = pat8x8[1];
    gdln.len = length;
    gdln.fg = fg;
    gdln.bg = bg;
    rop &= 0x0F;
    gfx_set_solid_pattern(0);
    gfx_set_raster_operation(planemask == ~0U ? PDfn[rop] :
                             (gfx_set_solid_source(planemask), PDfn_SM[rop]));
}

/*---------------------------------------------------------------------------
 * GXSubsequentDashedBresenhamLine
 *
 * Description:		This function is used to render a vector using the
 *                 	specified bresenham parameters.
 *
 * Parameters:
 *		pScrni:		Screen handler pointer having screen information.
 *      x1:  		Specifies the starting x position
 *      y1:      	Specifies starting y possition
 *      absmaj:		Specfies the Bresenman absolute major.
 *		absmin:		Specfies the Bresenman absolute minor.
 *		err:     	Specifies the bresenham err term.
 *		len:     	Specifies the length of the vector interms of pixels.
 *		octant:  	not used in this function,may be added for standard
 *               	interface.
 *
 * Returns:			none
 *
 * Comments:		none
 *
 * Sample application uses:
 *   - Window outlines on window move.
 *   - x11perf: line segments (-line500).
 *   - x11perf: line segments (-seg500).
 *----------------------------------------------------------------------------
 */
static void
GXSubsequentDashedBresenhamLine(ScrnInfoPtr pScrni,
                                int x1, int y1, int absmaj, int absmin,
                                int err, int len, int octant, int phase)
{
    int i, n;
    int axial, diagn;
    int trans = (gdln.bg == -1);
    unsigned long pat8x8[2];

    //ErrorF("BLine %d, %d, %d, %d, %d, %d, %d\n" x1, y1, absmaj, absmin,
    //err, len, octant);

    i = phase >= 32 ? (phase -= 32, 1) : 0;
    n = 32 - phase;
    pat8x8[0] =
        ((gdln.pat[i] >> phase) & ((1UL << n) - 1)) | (gdln.pat[1 - i] << n);
    pat8x8[1] =
        ((gdln.pat[1 - i] >> phase) & ((1UL << n) - 1)) | (gdln.pat[i] << n);
    axial = absmin;
    err += axial;
    diagn = absmin - absmaj;
    gfx_set_mono_pattern(gdln.bg, gdln.fg, pat8x8[0], pat8x8[1], trans);
    gfx2_set_pattern_origin(x1, y1);
    gfx2_bresenham_line(CALC_FBOFFSET(x1, y1), len, err, axial, diagn,
                        vmode[octant]);
}

/*---------------------------------------------------------------------------
 * GXSubsequentDashedTwoPointLine
 *
 * Description  :see GXSetupForDashedLine
 *
 *    Arg        Type     Comment
 *  pScrni   ScrnInfoPtr  pointer to Screeen info
 *     x0         int     destination x start offset
 *     y0         int     destination y start offset
 *     x1         int     destination x end offset
 *     y1         int     destination y end offset
 *  flags         int     OMIT_LAST, dont draw last pixel (not used)
 *  phase         int     initial pattern offset at x1,y1
 *
 * Returns      :none
 *---------------------------------------------------------------------------*/
static void
GXSubsequentDashedTwoPointLine(ScrnInfoPtr pScrni, int x0, int y0,
                               int x1, int y1, int flags, int phase)
{
    int i, n;
    long dx, dy, dmaj, dmin, octant, bias;
    long axial, diagn, err, len, pat8x8[2];

    //ErrorF("GXSubsequentDashedTwoPointLine() %d,%d %d,%d, %#x %d\n",
    //   x0, y0, x1, y1, flags, phase);

    i = phase >= 32 ? (phase -= 32, 1) : 0;
    n = 32 - phase;
    pat8x8[0] =
        ((gdln.pat[i] >> phase) & ((1UL << n) - 1)) | (gdln.pat[1 - i] << n);
    pat8x8[1] =
        ((gdln.pat[1 - i] >> phase) & ((1UL << n) - 1)) | (gdln.pat[i] << n);

    if ((dx = x1 - x0) < 0)
        dx = -dx;
    if ((dy = y1 - y0) < 0)
        dy = -dy;
    if (dy >= dx) {
        dmaj = dy;
        dmin = dx;
        octant = YMAJOR;
    }
    else {
        dmaj = dx;
        dmin = dy;
        octant = 0;
    }
    len = dmaj;
    if ((flags & OMIT_LAST) == 0)
        ++len;
    if (len <= 0)
        return;
    if (x1 < x0)
        octant |= XDECREASING;
    if (y1 < y0)
        octant |= YDECREASING;

    axial = dmin << 1;
    bias = miGetZeroLineBias(pScrni->pScreen);
    err = axial - dmaj - ((bias >> octant) & 1);
    diagn = (dmin - dmaj) << 1;

    gfx2_set_pattern_origin(x0, y0);
    gfx2_bresenham_line(CALC_FBOFFSET(x0, y0), len, err, axial, diagn,
                        vmode[octant]);

}
#endif                          /* GX_DASH_LINE_SUPPORT */

#if GX_WRITE_PIXMAP_SUPPORT
static void
GXWritePixmap(ScrnInfoPtr pScrni, int x, int y, int w, int h,
              unsigned char *src, int srcwidth, int rop, unsigned int planemask,
              int trans, int bpp, int depth)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);

    //ErrorF("GXWritePixmap() %d,%d %dx%d, s%#x sp%d %#x %#x %#x %d %d\n",
    //    x, y, w, h, src, srcwidth, rop, planemask, trans, bpp, depth);

    if (bpp == pScrni->bitsPerPixel) {
        rop &= 0x0F;
        if (rop == GXcopy && trans == -1) {
            gfx_wait_until_idle();
            geode_memory_to_screen_blt((unsigned long) src,
                                       (unsigned long) FBADDR(x, y), srcwidth,
                                       pGeode->Pitch, w, h, bpp);
        }
        else {
            gfx_set_solid_pattern(planemask);
            gfx_set_raster_operation(planemask ==
                                     ~0U ? SDfn[rop] : SDfn_PM[rop]);
            if (trans != -1)
                gfx_color_bitmap_to_screen_xblt(0, 0, x, y, w, h, src,
                                                srcwidth, trans);
            else
                gfx_color_bitmap_to_screen_blt(0, 0, x, y, w, h, src, srcwidth);
            SET_SYNC_FLAG(pGeode->AccelInfoRec);
        }
    }
    else
        pGeode->WritePixmap(pScrni, x, y, w, h, src, srcwidth, rop, planemask,
                            trans, bpp, depth);
}
#endif                          /* if GX_WRITE_PIXMAP_SUPPORT */

#if XF86EXA

static void
amd_gx_exa_WaitMarker(ScreenPtr pScreen, int Marker)
{
    GU2_WAIT_BUSY;
}

static void
amd_gx_exa_Done(PixmapPtr p)
{
}

static Bool
amd_gx_exa_UploadToScreen(PixmapPtr pDst, int x, int y, int w, int h,
                          char *src, int src_pitch)
{
    char *dst = pDst->devPrivate.ptr;
    int dst_pitch = exaGetPixmapPitch(pDst);
    int bpp = pDst->drawable.bitsPerPixel;

    dst += y * dst_pitch + x * (bpp >> 3);
    GU2_WAIT_BUSY;
    geode_memory_to_screen_blt((unsigned long) src, (unsigned long) dst,
                               src_pitch, dst_pitch, w, h, bpp);
    return TRUE;
}

static Bool
amd_gx_exa_DownloadFromScreen(PixmapPtr pSrc, int x, int y, int w, int h,
                              char *dst, int dst_pitch)
{
    char *src = pSrc->devPrivate.ptr;
    int src_pitch = exaGetPixmapPitch(pSrc);
    int bpp = pSrc->drawable.bitsPerPixel;

    src += (y * src_pitch) + (x * (bpp >> 3));
    GU2_WAIT_BUSY;
    geode_memory_to_screen_blt((unsigned long) src, (unsigned long) dst,
                               src_pitch, dst_pitch, w, h, bpp);
    return TRUE;
}

/* Solid */

static Bool
amd_gx_exa_PrepareSolid(PixmapPtr pxMap, int alu, Pixel planemask, Pixel fg)
{
    int dstPitch = exaGetPixmapPitch(pxMap);
    unsigned int ROP = amd_gx_BppToRasterMode(pxMap->drawable.bitsPerPixel)
        | (planemask == ~0U ? SDfn[alu] : SDfn_PM[alu]);

    //  FIXME: this should go away -- workaround for the blockparty icon corruption
    //if (pxMap->drawable.bitsPerPixel == 32)
    //  return FALSE;

    BLT_MODE = ((ROP ^ (ROP >> 2)) & 0x33) == 0 ? MGP_BM_SRC_MONO : 0;
    if (((ROP ^ (ROP >> 1)) & 0x55) != 0)
        BLT_MODE |= MGP_BM_DST_REQ;
    //ErrorF("amd_gx_exa_PrepareSolid(%#x,%#x,%#x - ROP=%x,BLT_MODE=%x)\n", alu, planemask, fg, ROP, BLT_MODE);
    GU2_WAIT_PENDING;
    WRITE_GP32(MGP_RASTER_MODE, ROP);
    WRITE_GP32(MGP_PAT_COLOR_0, planemask);
    WRITE_GP32(MGP_SRC_COLOR_FG, fg);
    WRITE_GP32(MGP_STRIDE, dstPitch);
    return TRUE;
}

static void
amd_gx_exa_Solid(PixmapPtr pxMap, int x1, int y1, int x2, int y2)
{
    int bpp = (pxMap->drawable.bitsPerPixel + 7) / 8;
    int pitch = exaGetPixmapPitch(pxMap);
    unsigned int offset = exaGetPixmapOffset(pxMap) + pitch * y1 + bpp * x1;
    unsigned int size = ((x2 - x1) << 16) | (y2 - y1);

    //ErrorF("amd_gx_exa_Solid() at %d,%d %d,%d - offset=%d, bpp=%d\n", x1, y1, x2, y2, offset, bpp);

    GU2_WAIT_PENDING;
    WRITE_GP32(MGP_DST_OFFSET, offset);
    WRITE_GP32(MGP_WID_HEIGHT, size);
    WRITE_GP32(MGP_BLT_MODE, BLT_MODE);
}

/* Copy */

static Bool
amd_gx_exa_PrepareCopy(PixmapPtr pxSrc, PixmapPtr pxDst, int dx, int dy,
                       int alu, Pixel planemask)
{
    GeodeRec *pGeode = GEODEPTR_FROM_PIXMAP(pxDst);
    int dstPitch = exaGetPixmapPitch(pxDst);
    unsigned int ROP;

    /* Punt if the color formats aren't the same */

    if (pxSrc->drawable.bitsPerPixel != pxDst->drawable.bitsPerPixel)
        return FALSE;

    //ErrorF("amd_gx_exa_PrepareCopy() dx%d dy%d alu %#x %#x\n",
    //  dx, dy, alu, planemask);

    pGeode->cpySrcOffset = exaGetPixmapOffset(pxSrc);
    pGeode->cpySrcPitch = exaGetPixmapPitch(pxSrc);
    pGeode->cpySrcBpp = (pxSrc->drawable.bitsPerPixel + 7) / 8;
    pGeode->cpyDx = dx;
    pGeode->cpyDy = dy;
    ROP = amd_gx_BppToRasterMode(pxSrc->drawable.bitsPerPixel) |
        (planemask == ~0U ? SDfn[alu] : SDfn_PM[alu]);

    BLT_MODE = ((ROP ^ (ROP >> 1)) & 0x55) != 0 ?
        MGP_BM_SRC_FB | MGP_BM_DST_REQ : MGP_BM_SRC_FB;
    GU2_WAIT_PENDING;
    WRITE_GP32(MGP_RASTER_MODE, ROP);
    WRITE_GP32(MGP_PAT_COLOR_0, planemask);
    WRITE_GP32(MGP_SRC_COLOR_FG, ~0);
    WRITE_GP32(MGP_SRC_COLOR_BG, ~0);
    WRITE_GP32(MGP_STRIDE, (pGeode->cpySrcPitch << 16) | dstPitch);
    return TRUE;
}

static void
amd_gx_exa_Copy(PixmapPtr pxDst, int srcX, int srcY, int dstX, int dstY,
                int w, int h)
{
    GeodeRec *pGeode = GEODEPTR_FROM_PIXMAP(pxDst);
    int dstBpp = (pxDst->drawable.bitsPerPixel + 7) / 8;
    int dstPitch = exaGetPixmapPitch(pxDst);
    unsigned int srcOffset =
        pGeode->cpySrcOffset + (pGeode->cpySrcPitch * srcY) +
        (pGeode->cpySrcBpp * srcX);
    unsigned int dstOffset =
        exaGetPixmapOffset(pxDst) + (dstPitch * dstY) + (dstBpp * dstX);
    unsigned int size = (w << 16) | h;
    unsigned int blt_mode = BLT_MODE;

    //ErrorF("amd_gx_exa_Copy() from %d,%d to %d,%d %dx%d\n", srcX, srcY,
    //   dstX, dstY, w, h);

    if (pGeode->cpyDx < 0) {
        srcOffset += w * pGeode->cpySrcBpp - 1;
        dstOffset += w * dstBpp - 1;
        blt_mode |= MGP_BM_NEG_XDIR;
    }
    if (pGeode->cpyDy < 0) {
        srcOffset += (h - 1) * pGeode->cpySrcPitch;
        dstOffset += (h - 1) * dstPitch;
        blt_mode |= MGP_BM_NEG_YDIR;
    }
    GU2_WAIT_PENDING;
    WRITE_GP32(MGP_SRC_OFFSET, srcOffset);
    WRITE_GP32(MGP_DST_OFFSET, dstOffset);
    WRITE_GP32(MGP_WID_HEIGHT, size);
    WRITE_GP16(MGP_BLT_MODE, blt_mode);
}

/* A=SRC, B=DST */
#define SRC_DST 0
/* B=SRC, A=DST */
#define DST_SRC MGP_RM_DEST_FROM_CHAN_A
/* A*alpha + B*0         */
#define Aa_B0   MGP_RM_ALPHA_TIMES_A
/* A*0     + B*(1-alpha) */
#define A0_B1a  MGP_RM_BETA_TIMES_B
/* A*1     + B*(1-alpha) */
#define A1_B1a  MGP_RM_A_PLUS_BETA_B
/* A*alpha + B*(1-alpha) */
#define Aa_B1a  MGP_RM_ALPHA_A_PLUS_BETA_B
/* alpha from A */
#define a_A MGP_RM_SELECT_ALPHA_A
/* alpha from B */
#define a_B MGP_RM_SELECT_ALPHA_B
/* alpha from const */
#define a_C MGP_RM_SELECT_ALPHA_R
/* alpha = 1 */
#define a_1 MGP_RM_SELECT_ALPHA_1

#define MGP_RM_ALPHA_TO_ARGB (MGP_RM_ALPHA_TO_ALPHA | MGP_RM_ALPHA_TO_RGB)
#define gxPictOpMAX PictOpAdd   /* highest accelerated op */

unsigned int amd_gx_exa_alpha_ops[] =
/*    A   B      OP     AS           const = 0 */
{
    (SRC_DST | Aa_B0 | a_C), 0, /* clear    (src*0) */
    (SRC_DST | Aa_B0 | a_1), 0, /* src      (src*1) */
    (DST_SRC | Aa_B0 | a_1), 0, /* dst      (dst*1) */
    (SRC_DST | A1_B1a | a_A), 0,        /* src-over (src*1 + dst(1-A)) */
    (DST_SRC | A1_B1a | a_A), 0,        /* dst-over (dst*1 + src(1-B)) */
    (SRC_DST | Aa_B0 | a_B), 0, /* src-in   (src*B) */
    (DST_SRC | Aa_B0 | a_B), 0, /* dst-in   (dst*A) */
    (DST_SRC | A0_B1a | a_A), 0,        /* src-out  (src*(1-B)) */
    (SRC_DST | A0_B1a | a_A), 0,        /* dst-out  (dst*(1-A)) */
/* pass1 (SRC=dst DST=scr=src), pass2 (SRC=src, DST=dst) */
    (DST_SRC | Aa_B0 | a_B),    /* srcatop  (src*B) */
    (SRC_DST | A0_B1a | a_A),   /*                  + (dst(1-A)) */
    (SRC_DST | Aa_B0 | a_B),    /* dstatop  (dst*A) */
    (DST_SRC | A0_B1a | a_A),   /*                  + (src(1-B) */
    (SRC_DST | A0_B1a | a_A),   /* xor      (src*(1-B) */
    (SRC_DST | A0_B1a | a_A),   /*                  + (dst(1-A) */
    (SRC_DST | A1_B1a | a_C), 0,        /* add      (src*1 + dst*1) */
};

typedef struct {
    int exa_fmt;
    int bpp;
    int gx_fmt;
    int alpha_bits;
} amd_gx_exa_fmt_t;

amd_gx_exa_fmt_t amd_gx_exa_fmts[] = {
    {PICT_a8r8g8b8, 32, MGP_RM_BPPFMT_8888, 8},
    {PICT_x8r8g8b8, 32, MGP_RM_BPPFMT_8888, 0},
    {PICT_a4r4g4b4, 16, MGP_RM_BPPFMT_4444, 4},
    {PICT_a1r5g5b5, 16, MGP_RM_BPPFMT_1555, 1},
    {PICT_r5g6b5, 16, MGP_RM_BPPFMT_565, 0},
    {PICT_r3g3b2, 8, MGP_RM_BPPFMT_332, 0},
};

static amd_gx_exa_fmt_t *
amd_gx_exa_check_format(PicturePtr p)
{
    int i;
    int bpp = p->pDrawable ? p->pDrawable->bitsPerPixel : 0;
    amd_gx_exa_fmt_t *fp = &amd_gx_exa_fmts[0];

    for (i = sizeof(amd_gx_exa_fmts) / sizeof(amd_gx_exa_fmts[0]); --i >= 0;
         ++fp) {
        if (fp->bpp < bpp)
            return NULL;
        if (fp->bpp != bpp)
            continue;
        if (fp->exa_fmt == p->format)
            break;
    }
    return i < 0 ? NULL : fp;
}

/* Composite */

static Bool
amd_gx_exa_CheckComposite(int op, PicturePtr pSrc, PicturePtr pMsk,
                          PicturePtr pDst)
{
    GeodeRec *pGeode = GEODEPTR_FROM_PICTURE(pDst);

    if (op > gxPictOpMAX)
        return FALSE;
    if (pMsk)
        return FALSE;
    if (usesPasses(op) && pGeode->exaBfrSz == 0)
        return FALSE;
    if (pSrc->filter != PictFilterNearest &&
        pSrc->filter != PictFilterFast &&
        pSrc->filter != PictFilterGood && pSrc->filter != PictFilterBest)
        return FALSE;
    if (pSrc->repeat)
        return FALSE;
    if (pSrc->transform)
        return FALSE;
    return TRUE;
}

static Bool
amd_gx_exa_PrepareComposite(int op, PicturePtr pSrc, PicturePtr pMsk,
                            PicturePtr pDst, PixmapPtr pxSrc, PixmapPtr pxMsk,
                            PixmapPtr pxDst)
{
    int srcPitch;

    GeodeRec *pGeode = GEODEPTR_FROM_PIXMAP(pxDst);
    amd_gx_exa_fmt_t *sfp, *dfp;

    //ErrorF("amd_gx_exa_PrepareComposite()\n");

    if ((sfp = amd_gx_exa_check_format(pSrc)) == NULL)
        return FALSE;
    if (sfp->alpha_bits == 0 && usesSrcAlpha(op))
        return FALSE;
    if ((dfp = amd_gx_exa_check_format(pDst)) == NULL)
        return FALSE;
    if (dfp->alpha_bits == 0 && usesDstAlpha(op))
        return FALSE;
    if (sfp->gx_fmt != dfp->gx_fmt)
        return FALSE;
    srcPitch = exaGetPixmapPitch(pxSrc);
    if (usesPasses(op) && srcPitch > pGeode->exaBfrSz)
        return FALSE;
    pGeode->cmpSrcPitch = srcPitch;
    pGeode->cmpOp = op;
    pGeode->cmpSrcOffset = exaGetPixmapOffset(pxSrc);
    pGeode->cmpSrcBpp = (pxSrc->drawable.bitsPerPixel + 7) / 8;
    pGeode->cmpSrcFmt = sfp->gx_fmt;
    pGeode->cmpDstFmt = dfp->gx_fmt | (dfp->alpha_bits == 0 ?
                                       MGP_RM_ALPHA_TO_RGB :
                                       MGP_RM_ALPHA_TO_ARGB);
    return TRUE;
}

static void
amd_gx_exa_Composite(PixmapPtr pxDst, int srcX, int srcY, int maskX,
                     int maskY, int dstX, int dstY, int width, int height)
{
    int op, current_line, max_lines, lines, pass, scratchPitch;
    unsigned int srcOffset, srcOfs = 0, srcPitch, srcPch = 0, srcBpp;
    unsigned int dstOffset, dstOfs = 0, dstPitch, dstPch = 0, dstBpp;
    unsigned int sizes, strides, blt_mode = 0, rop = 0;
    GeodeRec *pGeode = GEODEPTR_FROM_PIXMAP(pxDst);

    //ErrorF("amd_gx_exa_Composite() from %d,%d to %d,%d %dx%d\n",
    //    srcX, srcY, dstX, dstY, width, height);

    op = pGeode->cmpOp;
    if (usesPasses(op)) {
        int cacheLineSz = 32;
        int cachelines =
            (width * pGeode->cmpSrcBpp + cacheLineSz - 1) / cacheLineSz;
        scratchPitch = cachelines * cacheLineSz;
        if (scratchPitch > pGeode->cmpSrcPitch)
            scratchPitch = pGeode->cmpSrcPitch;
        max_lines = pGeode->exaBfrSz / scratchPitch;
    }
    else {
        scratchPitch = 0;
        max_lines = height;
    }

    dstBpp = (pxDst->drawable.bitsPerPixel + 7) / 8;
    dstPitch = exaGetPixmapPitch(pxDst);
    dstOffset = exaGetPixmapOffset(pxDst) + dstPitch * dstY + dstBpp * dstX;
    srcBpp = pGeode->cmpSrcBpp;
    srcPitch = pGeode->cmpSrcPitch;
    srcOffset = pGeode->cmpSrcOffset + srcPitch * srcY + srcBpp * srcX;

    current_line = pass = 0;
    while (current_line < height) {
        if (usesPasses(op)) {
            lines = height - current_line;
            if (lines > max_lines)
                lines = max_lines;
            switch (pass) {
            case 0:            /* copy src to scratch */
                srcPch = srcPitch;
                srcOfs = srcOffset + current_line * srcPch;
                dstPch = scratchPitch;
                dstOfs = pGeode->exaBfrOffset;
                rop = pGeode->cmpSrcFmt | MGP_RM_ALPHA_TO_ARGB;
                rop |= amd_gx_exa_alpha_ops[PictOpSrc * 2];
                blt_mode = usesChanB0(PictOpSrc) ?
                    MGP_BM_SRC_FB | MGP_BM_DST_REQ : MGP_BM_SRC_FB;
                ++pass;
                break;
            case 1:            /* pass1 */
                srcPch = dstPitch;
                srcOfs = dstOffset + current_line * srcPch;
                dstPch = scratchPitch;
                dstOfs = pGeode->exaBfrOffset;
                rop = pGeode->cmpSrcFmt | MGP_RM_ALPHA_TO_ARGB;
                rop |= amd_gx_exa_alpha_ops[op * 2];
                blt_mode = usesChanB1(op) ?
                    MGP_BM_SRC_FB | MGP_BM_DST_REQ : MGP_BM_SRC_FB;
                ++pass;
                break;
            case 2:            /* pass2 */
                srcPch = srcPitch;
                srcOfs = srcOffset + current_line * srcPch;
                dstPch = dstPitch;
                dstOfs = dstOffset + current_line * dstPch;
                rop = pGeode->cmpSrcFmt | MGP_RM_ALPHA_TO_ARGB;
                rop |= amd_gx_exa_alpha_ops[op * 2 + 1];
                blt_mode = usesChanB2(op) ?
                    MGP_BM_SRC_FB | MGP_BM_DST_REQ : MGP_BM_SRC_FB;
                ++pass;
                break;
            case 3:            /* add */
                srcPch = scratchPitch;
                srcOfs = pGeode->exaBfrOffset;
                dstPch = dstPitch;
                dstOfs = dstOffset + current_line * dstPch;
                rop = pGeode->cmpDstFmt;
                rop |= amd_gx_exa_alpha_ops[PictOpAdd * 2];
                blt_mode = usesChanB0(PictOpAdd) ?
                    MGP_BM_SRC_FB | MGP_BM_DST_REQ : MGP_BM_SRC_FB;
                current_line += lines;
                pass = 0;
                break;
            }
            strides = (srcPch << 16) | dstPch;
        }
        else {                  /* not multi pass */
            srcOfs = srcOffset;
            dstOfs = dstOffset;
            current_line = lines = height;
            strides = (srcPitch << 16) | dstPitch;
            rop = pGeode->cmpDstFmt | amd_gx_exa_alpha_ops[op * 2];
            blt_mode = usesChanB0(op) ?
                MGP_BM_SRC_FB | MGP_BM_DST_REQ : MGP_BM_SRC_FB;
        }
        sizes = (width << 16) | lines;
        if (srcOfs < dstOfs) {
            srcOfs += (lines - 1) * srcPitch + width * srcBpp - 1;
            dstOfs += (lines - 1) * dstPitch + width * dstBpp - 1;
            blt_mode |= MGP_BM_NEG_XDIR | MGP_BM_NEG_YDIR;
        }
        GU2_WAIT_PENDING;
        WRITE_GP32(MGP_RASTER_MODE, rop);
        WRITE_GP32(MGP_SRC_OFFSET, srcOfs);
        WRITE_GP32(MGP_DST_OFFSET, dstOfs);
        WRITE_GP32(MGP_WID_HEIGHT, sizes);
        WRITE_GP32(MGP_STRIDE, strides);
        WRITE_GP16(MGP_BLT_MODE, blt_mode);
    }
}
#endif                          /* #if XF86EXA */

/*----------------------------------------------------------------------------
 * GXAccelInit.
 *
 * Description:	This function sets up the supported acceleration routines and
 *              appropriate flags.
 *
 * Parameters:
 *      pScrn:	Screeen pointer structure.
 *
 * Returns:		TRUE on success and FALSE on Failure
 *
 * Comments:	This function is called in GXScreenInit in
 *              geode_driver.c to set  * the acceleration.
 *----------------------------------------------------------------------------
 */
Bool
GXAccelInit(ScreenPtr pScrn)
{
    ScrnInfoPtr pScrni = xf86ScreenToScrn(pScrn);
    GeodeRec *pGeode = GEODEPTR(pScrni);

#if XF86EXA
    ExaDriverPtr pExa = pGeode->pExa;
#endif

    gu2_xshift = pScrni->bitsPerPixel >> 4;

    /* XXX - fixme - this will change - we'll need to update it */

    gu2_pitch = pGeode->Pitch;

    switch (pGeode->Pitch) {
    case 1024:
        gu2_yshift = 10;
        break;
    case 2048:
        gu2_yshift = 11;
        break;
    case 4096:
        gu2_yshift = 12;
        break;
    default:
        gu2_yshift = 13;
        break;
    }

#ifdef OPT_ACCEL
    ACCEL_STRIDE = (pGeode->Pitch << 16) | pGeode->Pitch;
    BPP = amd_gx_BppToRasterMode(pScrni->bitsPerPixel);
#endif

#if XF86EXA
    if (pExa && pGeode->useEXA) {
        pExa->exa_major = EXA_VERSION_MAJOR;
        pExa->exa_minor = EXA_VERSION_MINOR;

        /* Sync */
        pExa->WaitMarker = amd_gx_exa_WaitMarker;
        /* UploadToScreen */
        pExa->UploadToScreen = amd_gx_exa_UploadToScreen;
        pExa->DownloadFromScreen = amd_gx_exa_DownloadFromScreen;

        /* Solid fill */
        pExa->PrepareSolid = amd_gx_exa_PrepareSolid;
        pExa->Solid = amd_gx_exa_Solid;
        pExa->DoneSolid = amd_gx_exa_Done;

        /* Copy */
        pExa->PrepareCopy = amd_gx_exa_PrepareCopy;
        pExa->Copy = amd_gx_exa_Copy;
        pExa->DoneCopy = amd_gx_exa_Done;

        /* Composite */
        pExa->CheckComposite = amd_gx_exa_CheckComposite;
        pExa->PrepareComposite = amd_gx_exa_PrepareComposite;
        pExa->Composite = amd_gx_exa_Composite;
        pExa->DoneComposite = amd_gx_exa_Done;

        return exaDriverInit(pScrn, pGeode->pExa);
    }
#endif

#if XF86XAA

    /* Getting the pointer for acceleration Inforecord */
    pGeode->AccelInfoRec = localRecPtr = XAACreateInfoRec();
    if (!pGeode->AccelInfoRec)
        return FALSE;

    /* SET ACCELERATION FLAGS */
    localRecPtr->Flags = PIXMAP_CACHE | OFFSCREEN_PIXMAPS | LINEAR_FRAMEBUFFER;

    /* HOOK SYNCRONIZARION ROUTINE */
    localRecPtr->Sync = GXAccelSync;

#if GX_FILL_RECT_SUPPORT
    /* HOOK FILLED RECTANGLES */
    HOOK(SetupForSolidFill);
    HOOK(SubsequentSolidFillRect);
    localRecPtr->SolidFillFlags = 0;
#endif

#if GX_MONO_8X8_PAT_SUPPORT
    /* Color expansion */
    HOOK(SetupForMono8x8PatternFill);
    HOOK(SubsequentMono8x8PatternFillRect);
/*         BIT_ORDER_IN_BYTE_MSBFIRST | SCANLINE_PAD_DWORD | NO_TRANSPARENCY | */
    localRecPtr->Mono8x8PatternFillFlags = BIT_ORDER_IN_BYTE_MSBFIRST |
        HARDWARE_PATTERN_PROGRAMMED_BITS | HARDWARE_PATTERN_SCREEN_ORIGIN;
#endif

#if GX_CLREXP_8X8_PAT_SUPPORT
    /* Color expansion */
    HOOK(SetupForColor8x8PatternFill);
    HOOK(SubsequentColor8x8PatternFillRect);
/*         BIT_ORDER_IN_BYTE_MSBFIRST | SCANLINE_PAD_DWORD | NO_TRANSPARENCY | */
    localRecPtr->Color8x8PatternFillFlags =
        BIT_ORDER_IN_BYTE_MSBFIRST | SCANLINE_PAD_DWORD |
        HARDWARE_PATTERN_PROGRAMMED_BITS | HARDWARE_PATTERN_PROGRAMMED_ORIGIN;
#endif

#if GX_SCR2SCRCPY_SUPPORT
    /* HOOK SCREEN TO SCREEN COPIES
     * Set flag to only allow copy if transparency is enabled.
     */
    HOOK(SetupForScreenToScreenCopy);
    HOOK(SubsequentScreenToScreenCopy);
    localRecPtr->ScreenToScreenCopyFlags =
        BIT_ORDER_IN_BYTE_MSBFIRST | SCANLINE_PAD_DWORD;
#endif

#if GX_BRES_LINE_SUPPORT
    /* HOOK BRESENHAM SOLID LINES */
    localRecPtr->SolidLineFlags = NO_PLANEMASK;
    HOOK(SetupForSolidLine);
    HOOK(SubsequentSolidBresenhamLine);
    HOOK(SubsequentSolidHorVertLine);
    HOOK(SubsequentSolidTwoPointLine);
    localRecPtr->SolidBresenhamLineErrorTermBits = 15;
#endif

#if GX_DASH_LINE_SUPPORT
    /* HOOK BRESENHAM DASHED LINES */
    HOOK(SetupForDashedLine);
    HOOK(SubsequentDashedBresenhamLine);
    HOOK(SubsequentDashedTwoPointLine);
    localRecPtr->DashedBresenhamLineErrorTermBits = 15;
    localRecPtr->DashPatternMaxLength = 64;
    localRecPtr->DashedLineFlags = NO_PLANEMASK |       /* TRANSPARENCY_ONLY | */
        LINE_PATTERN_POWER_OF_2_ONLY | LINE_PATTERN_MSBFIRST_MSBJUSTIFIED;
#endif

#if GX_SCR2SCREXP_SUPPORT
    /* Color expansion */
    HOOK(SetupForScreenToScreenColorExpandFill);
    HOOK(SubsequentScreenToScreenColorExpandFill);
    localRecPtr->ScreenToScreenColorExpandFillFlags =
        BIT_ORDER_IN_BYTE_MSBFIRST | SCANLINE_PAD_DWORD | NO_TRANSPARENCY;
#endif

    if (pGeode->AccelImageWriteBuffers) {
#if GX_SCANLINE_SUPPORT
        localRecPtr->ScanlineImageWriteBuffers = pGeode->AccelImageWriteBuffers;
        localRecPtr->NumScanlineImageWriteBuffers = pGeode->NoOfImgBuffers;
        HOOK(SetupForScanlineImageWrite);
        HOOK(SubsequentScanlineImageWriteRect);
        HOOK(SubsequentImageWriteScanline);
        localRecPtr->ScanlineImageWriteFlags = NO_PLANEMASK | NO_GXCOPY |
            BIT_ORDER_IN_BYTE_MSBFIRST | SCANLINE_PAD_DWORD;
#endif

    }
    else {
        localRecPtr->PixmapCacheFlags = DO_NOT_BLIT_STIPPLES;
    }

    if (pGeode->AccelColorExpandBuffers) {
#if GX_CPU2SCREXP_SUPPORT
        /* Color expansion */
        localRecPtr->ScanlineColorExpandBuffers =
            pGeode->AccelColorExpandBuffers;
        localRecPtr->NumScanlineColorExpandBuffers =
            pGeode->NoOfColorExpandLines;
        HOOK(SetupForScanlineCPUToScreenColorExpandFill);
        HOOK(SubsequentScanlineCPUToScreenColorExpandFill);
        HOOK(SubsequentColorExpandScanline);
        localRecPtr->ScanlineCPUToScreenColorExpandFillFlags = NO_PLANEMASK |
            BIT_ORDER_IN_BYTE_MSBFIRST | SCANLINE_PAD_DWORD;
#endif
    }
#if GX_WRITE_PIXMAP_SUPPORT
    pGeode->WritePixmap = localRecPtr->WritePixmap;
    HOOK(WritePixmap);
#endif

    return (XAAInit(pScrn, localRecPtr));
#else                           /* XF86XAA */
    return FALSE;
#endif
}

/* END OF FILE */
