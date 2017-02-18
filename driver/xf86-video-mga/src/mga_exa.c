/*
 * Copyright 2006 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software")
 * to deal in the software without restriction, including without limitation
 * on the rights to use, copy, modify, merge, publish, distribute, sub
 * license, and/or sell copies of the Software, and to permit persons to whom
 * them Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTIBILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Adam Jackson <ajax@nwnk.net>
 *    Tilman Sauerbeck <tilman@code-monkey.de>
 */

/*
 * Sources:
 * - mga kdrive accel by Anders Carlsson
 * - mga g400 Render accel by Damien Ciabrini
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

#include "mga.h"
#include "mga_reg.h"
#include "mga_macros.h"

#include "exa.h"
#ifdef MGADRI
#include "mga_dri.h"
#endif


#if 0
#define DEBUG_MSG(x)  ErrorF x
#else
#define DEBUG_MSG(x)
#endif

#define PMGA(x) \
    MGAPtr pMga = xf86ScreenToScrn(x->drawable.pScreen)->driverPrivate;

#define QUIESCE_DMA(x) \
    CHECK_DMA_QUIESCENT(pMga, xf86ScreenToScrn(x->drawable.pScreen));

/* stuff stolen from mga_storm.c */
#define BLIT_LEFT   1
#define BLIT_UP        4

/* FIXME
 * We could use MGADWG_BLK for GXcopy if HaveSDRAM is FALSE.
 * That will only work in some situations though (see mga_storm.c
 * for details).
 */
static const CARD32 mgaRop[16] = {
    /* GXclear        */  MGADWG_RPL  | 0x00000000,    /* 0 */
    /* GXand          */  MGADWG_RSTR | 0x00080000,    /* src AND dst */
    /* GXandReverse   */  MGADWG_RSTR | 0x00040000,    /* src AND NOT dst */
    /* GXcopy         */  MGADWG_RSTR | 0x000c0000,    /* src */
    /* GXandInverted  */  MGADWG_RSTR | 0x00020000,    /* NOT src AND dst */
    /* GXnoop         */  MGADWG_RSTR | 0x000a0000,    /* dst */
    /* GXxor          */  MGADWG_RSTR | 0x00060000,    /* src XOR dst */
    /* GXor           */  MGADWG_RSTR | 0x000e0000,    /* src OR dst */
    /* GXnor          */  MGADWG_RSTR | 0x00010000,    /* NOT src AND NOT dst */
    /* GXequiv        */  MGADWG_RSTR | 0x00090000,    /* NOT src XOR dst */
    /* GXinvert       */  MGADWG_RSTR | 0x00050000,    /* NOT dst */
    /* GXorReverse    */  MGADWG_RSTR | 0x000d0000,    /* src OR NOT dst */
    /* GXcopyInverted */  MGADWG_RPL  | 0x00030000,    /* NOT src */
    /* GXorInverted   */  MGADWG_RSTR | 0x000b0000,    /* NOT src OR dst */
    /* GXnand         */  MGADWG_RSTR | 0x00070000,    /* NOT src OR NOT dst */
    /* GXset          */  MGADWG_RPL  | 0x000f0000    /* 1 */
};

static const struct {
    Bool dst_alpha;
    Bool src_alpha;
    CARD32 blend_cntl;
} mgaBlendOp[] = {
    /* Clear */
    {0, 0, MGA_SRC_ZERO                | MGA_DST_ZERO},
    /* Src */
    {0, 0, MGA_SRC_ONE                 | MGA_DST_ZERO},
    /* Dst */
    {0, 0, MGA_SRC_ZERO                | MGA_DST_ONE},
    /* Over */
    {0, 1, MGA_SRC_ONE                 | MGA_DST_ONE_MINUS_SRC_ALPHA},
    /* OverReverse */
    {1, 0, MGA_SRC_ONE_MINUS_DST_ALPHA | MGA_DST_ONE},
    /* In */
    {1, 0, MGA_SRC_DST_ALPHA           | MGA_DST_ZERO},
    /* InReverse */
    {0, 1, MGA_SRC_ZERO                | MGA_DST_SRC_ALPHA},
    /* Out */
    {1, 0, MGA_SRC_ONE_MINUS_DST_ALPHA | MGA_DST_ZERO},
    /* OutReverse */
    {0, 1, MGA_SRC_ZERO                | MGA_DST_ONE_MINUS_SRC_ALPHA},
    /* Atop */
    {1, 1, MGA_SRC_DST_ALPHA           | MGA_DST_ONE_MINUS_SRC_ALPHA},
    /* AtopReverse */
    {1, 1, MGA_SRC_ONE_MINUS_DST_ALPHA | MGA_DST_SRC_ALPHA},
    /* Xor */
    {1, 1, MGA_SRC_ONE_MINUS_DST_ALPHA | MGA_DST_ONE_MINUS_SRC_ALPHA},
    /* Add */
    {0, 0, MGA_SRC_ONE                 | MGA_DST_ONE},
};

static CARD32
mgaGetPixmapPitch(PixmapPtr pPix)
{
    return exaGetPixmapPitch(pPix) / (pPix->drawable.bitsPerPixel >> 3);
}

static CARD32
mgaGetMACCESS(PixmapPtr pixmap, PicturePtr pict)
{
    switch (pixmap->drawable.bitsPerPixel) {
    case 8:
        return MGAMAC_PW8 | MGAMAC_BYPASS332 | MGAMAC_NODITHER;
    case 16:
        if (pict &&
            (pict->format == PICT_x1r5g5b5 || pict->format == PICT_a1r5g5b5))
            return MGAMAC_PW16 | MGAMAC_DIT555;
        else
            return MGAMAC_PW16;
    case 24:
        return MGAMAC_PW24;
    default:
        return MGAMAC_PW32;
    }
}

static Bool
mgaSetup(MGAPtr pMga, PixmapPtr pixmap, PicturePtr pict, int wait)
{
    WAITFIFO(wait + 4);

    /* Set the format of the destination pixmap */
    OUTREG(MGAREG_MACCESS, mgaGetMACCESS(pixmap, pict));

    OUTREG(MGAREG_CXBNDRY, 0xffff0000);
    OUTREG(MGAREG_YTOP, 0x00000000);
    OUTREG(MGAREG_YBOT, 0x007fffff);

    return TRUE;
}

static void
mgaNoopDone(PixmapPtr pPixmap)
{
}

static Bool
mgaPrepareSolid(PixmapPtr pPixmap, int alu, Pixel planemask, Pixel fg)
{
    PMGA(pPixmap);
    int dwgctl;

    /* FIXME
     * Is this needed here? We don't use DMA stuff here...
     */
    QUIESCE_DMA(pPixmap);

    /* We must pad planemask and fg depending on the format of the
     * destination pixmap
     */
    switch (pPixmap->drawable.bitsPerPixel) {
    case 8:
        fg |= fg << 8;
        planemask |= planemask << 8;
        /* fall through */
    case 16:
        fg |= fg << 16;
        planemask |= planemask << 16;
        break;
    }

    dwgctl = MGADWG_TRAP | MGADWG_SOLID | MGADWG_ARZERO |
             MGADWG_SGNZERO | MGADWG_SHIFTZERO | mgaRop[alu];

    mgaSetup(pMga, pPixmap, NULL, 5);

    OUTREG(MGAREG_PITCH, mgaGetPixmapPitch(pPixmap));
    OUTREG(MGAREG_DSTORG, exaGetPixmapOffset(pPixmap));
    OUTREG(MGAREG_FCOL, fg);
    OUTREG(MGAREG_PLNWT, planemask);
    OUTREG(MGAREG_DWGCTL, dwgctl);

    return TRUE;
}

static void
mgaSolid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2)
{
    PMGA(pPixmap);

    WAITFIFO(2);
    OUTREG(MGAREG_FXBNDRY, (x2 << 16) | (x1 & 0xffff));
    OUTREG(MGAREG_YDSTLEN | MGAREG_EXEC, (y1 << 16) | (y2 - y1));
}

static Bool
mgaPrepareCopy(PixmapPtr pSrc, PixmapPtr pDst, int xdir, int ydir, int alu,
           Pixel planemask)
{
    PMGA(pSrc);
    int blit_direction = 0;
    int dwgctl = 0;

    /* FIXME
     * Is this needed here? We don't use DMA stuff here...
     */
    QUIESCE_DMA(pSrc);

    if (xdir < 0)
        blit_direction |= BLIT_LEFT;

    if (ydir < 0)
        blit_direction |= BLIT_UP;

    pMga->BltScanDirection = blit_direction;

    dwgctl = mgaRop[alu] | MGADWG_SHIFTZERO | MGADWG_BITBLT | MGADWG_BFCOL;
    pMga->src_pitch = mgaGetPixmapPitch(pSrc);

    mgaSetup(pMga, pDst, NULL, 7);
    OUTREG(MGAREG_PITCH, mgaGetPixmapPitch(pDst));
    OUTREG(MGAREG_SRCORG, exaGetPixmapOffset(pSrc));
    OUTREG(MGAREG_DSTORG, exaGetPixmapOffset(pDst));
    OUTREG(MGAREG_DWGCTL, dwgctl);
    OUTREG(MGAREG_SGN, blit_direction);
    OUTREG(MGAREG_PLNWT, planemask);
    OUTREG(MGAREG_AR5, (ydir < 0 ? -1 : 1) * pMga->src_pitch);

    return TRUE;
}

static void
mgaCopy(PixmapPtr pDst, int srcx, int srcy, int dstx, int dsty, int w, int h)
{
    PMGA(pDst);
    int start, end;

    w--;

    if (pMga->BltScanDirection & BLIT_UP) {
        srcy += h - 1;
        dsty += h - 1;
    }

    start = end = srcy * pMga->src_pitch + srcx;

    if (pMga->BltScanDirection & BLIT_LEFT)
        start += w;
    else
        end += w;

    WAITFIFO(4);
    OUTREG(MGAREG_AR0, end);
    OUTREG(MGAREG_AR3, start);
    OUTREG(MGAREG_FXBNDRY, ((dstx + w) << 16) | (dstx & 0xffff));
    OUTREG(MGAREG_YDSTLEN | MGAREG_EXEC, (dsty << 16) | h);
}

static int
MGA_LOG2(int val)
{
    int ret = 0;

    if (val == 1)
        return 0;

    while (val >> ret)
        ret++;

    return ((1 << (ret - 1)) == val) ? (ret - 1) : ret;
}

static CARD32
mgaGetTexFormat(PicturePtr pPict)
{
    static const struct {
        int fmt;
        CARD32 card_fmt;
    } *ptr, texformats[] = {
        { PICT_a8r8g8b8, MGA_TW32 },
        { PICT_x8r8g8b8, MGA_TW32 },
        { PICT_r5g6b5, MGA_TW16 },
        { PICT_a1r5g5b5, MGA_TW15 },
        { PICT_x1r5g5b5, MGA_TW15 },
        { PICT_a4r4g4b4, MGA_TW12 },
        { PICT_x4r4g4b4, MGA_TW12 },
        { PICT_a8, MGA_TW8A },
        { 0, 0}
    };

    for (ptr = texformats; ptr->fmt; ptr++)
        if (ptr->fmt == pPict->format)
            return ptr->card_fmt;

    return 0;
}

static Bool
mgaCheckSourceTexture(int tmu, PicturePtr pPict)
{
    if (!pPict->pDrawable) {
        DEBUG_MSG(("Solid / gradient pictures not supported\n"));
        return FALSE;
    }

    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;

    if ((w > 2047) || (h > 2047)){
        DEBUG_MSG(("Picture w/h too large (%dx%d)\n", w, h));
        return FALSE;
    }

    if (!mgaGetTexFormat(pPict)) {
        DEBUG_MSG(("Unsupported picture format 0x%x\n", pPict->format));
        return FALSE;
    }

    if (pPict->repeatType != RepeatNormal) {
        DEBUG_MSG(("Unsupported repeat type %d\n", pPict->repeatType));
        return FALSE;
    }

    if (pPict->repeat && ((w & (w - 1)) != 0 || (h & (h - 1)) != 0)) {
        DEBUG_MSG(("NPOT repeat unsupported (%dx%d)\n", w, h));
        return FALSE;
    }

    if (pPict->filter != PictFilterNearest &&
        pPict->filter != PictFilterBilinear) {
        DEBUG_MSG(("Unsupported filter 0x%x\n", pPict->filter));
        return FALSE;
    }

    return TRUE;
}

static Bool
mgaCheckComposite(int op, PicturePtr pSrcPict, PicturePtr pMaskPict,
                  PicturePtr pDstPict)
{
    MGAPtr pMga = xf86ScreenToScrn(pDstPict->pDrawable->pScreen)->driverPrivate;

    if (op >= sizeof(mgaBlendOp) / sizeof(mgaBlendOp[0])) {
        DEBUG_MSG(("unsupported op %x\n", op));
        return FALSE;
    }

    if (!mgaCheckSourceTexture(0, pSrcPict))
        return FALSE;

    if (pMaskPict) {
        if (!mgaCheckSourceTexture(1, pMaskPict))
            return FALSE;

        if (pMaskPict->componentAlpha) {
            DEBUG_MSG(("Component alpha unsupported\n"));
            return FALSE;
        }
    }

    if (PICT_FORMAT_TYPE(pDstPict->format) == PICT_TYPE_ABGR) {
        DEBUG_MSG(("render to (A)BGR unsupported\n"));
        return FALSE;
    }

    /* Only the G550 can perform Add on A8 textures, it seems. */
    if (pMga->Chipset != PCI_CHIP_MGAG550 &&
        op == PictOpAdd && pSrcPict->format == PICT_a8 &&
        pDstPict->format == PICT_a8)
        return FALSE;

    return TRUE;
}

static void
PrepareSourceTexture(int tmu, PicturePtr pSrcPicture, PixmapPtr pSrc)
{
    PMGA(pSrc);
    int pitch = mgaGetPixmapPitch(pSrc);
    int w = pSrc->drawable.width;
    int h = pSrc->drawable.height;
    int w_log2 = MGA_LOG2(w);
    int h_log2 = MGA_LOG2(h);

    int texctl = MGA_PITCHLIN | ((pitch & (2048 - 1)) << 9) |
                 MGA_NOPERSPECTIVE | MGA_TAKEY;
    int texctl2 = MGA_G400_TC2_MAGIC | MGA_TC2_CKSTRANSDIS;
    int texfilter = MGA_FILTERALPHA | (0x10 << 21);

    texctl |= mgaGetTexFormat(pSrcPicture);

    if (pSrcPicture->filter == PictFilterBilinear)
        texfilter |= MGA_MAG_BILIN | MGA_MIN_BILIN;
    else
        texfilter |= MGA_MAG_NRST | MGA_MIN_NRST;

    if (!pSrcPicture->repeat)
        texctl |= MGA_CLAMPUV;

    if (tmu == 1)
        texctl2 |= MGA_TC2_DUALTEX | MGA_TC2_SELECT_TMU1;

    WAITFIFO(6);
    OUTREG(MGAREG_TEXCTL2, texctl2);
    OUTREG(MGAREG_TEXCTL, texctl);

    /* Source (texture) address + pitch */
    OUTREG(MGAREG_TEXORG, exaGetPixmapOffset(pSrc));
    OUTREG(MGAREG_TEXWIDTH, (w - 1) << 18 | ((8 - w_log2) & 63) << 9 | w_log2);
    OUTREG(MGAREG_TEXHEIGHT, (h - 1) << 18 | ((8 - h_log2) & 63) << 9 | h_log2);

    /* Set blit filtering flags */
    OUTREG(MGAREG_TEXFILTER, texfilter);

    if (tmu == 1) {
        WAITFIFO(1);
        OUTREG(MGAREG_TEXCTL2, texctl2 & ~MGA_TC2_SELECT_TMU1);
    }
}

/*
 *  The formals params are the elements of the following matrix:
 *
 *     Dest            Transform             Src
 *    coords                                coords
 *   / Xdst \   / X_incx X_incy X_init \   / Xsrc \
 *   | Ydst | = | Y_incx Y_incy Y_init | x | Ysrc |
 *   \  1   /   \ H_incx H_incy H_init /   \  1   /
 *
 * matrix elements are 32bits fixed points (16.16)
 * mga_fx_* is the size of the fixed point for the TMU
 */
static void
setTMIncrementsRegs(PixmapPtr pPix, int X_incx, int X_incy, int X_init,
                    int Y_incx, int Y_incy, int Y_init,
                    int H_incx, int H_incy, int H_init,
                    int mga_fx_width_size, int mga_fx_height_size)
{
    PMGA(pPix);

    int decalw = mga_fx_width_size - 16;
    int decalh = mga_fx_height_size - 16;

    /* Convert 16 bits fixpoint -> MGA variable size fixpoint */
    if (decalw >= 0) {
        X_incx <<= decalw;
        X_incy <<= decalw;
        X_init <<= decalw;
    } else {
        decalw = -decalw;
        X_incx >>= decalw;
        X_incy >>= decalw;
        X_init >>= decalw;
    }

    /* Convert 16 bits fixpoint -> MGA variable size fixpoint */
    if (decalh >= 0) {
        Y_incx <<= decalh;
        Y_incy <<= decalh;
        Y_init <<= decalh;
    } else {
        decalh = -decalh;
        Y_incx >>= decalh;
        Y_incy >>= decalh;
        Y_init >>= decalh;
    }

    /* Set TM registers */
    WAITFIFO(9);
    OUTREG(MGAREG_TMR0, X_incx);
    OUTREG(MGAREG_TMR1, Y_incx);
    OUTREG(MGAREG_TMR2, X_incy);
    OUTREG(MGAREG_TMR3, Y_incy);
    OUTREG(MGAREG_TMR4, H_incx);
    OUTREG(MGAREG_TMR5, H_incy);
    OUTREG(MGAREG_TMR6, X_init);
    OUTREG(MGAREG_TMR7, Y_init);
    OUTREG(MGAREG_TMR8, H_init);
}

/* XXX these look like magic */
#define C_ARG1_CUR              0x0
#define C_ARG1_ALPHA            MGA_TDS_COLOR_ARG1_REPLICATEALPHA
#define C_ARG2_DIFFUSE          MGA_TDS_COLOR_ARG2_DIFFUSE
#define C_ARG2_FCOL             MGA_TDS_COLOR_ARG2_FCOL
#define C_ARG2_PREV             MGA_TDS_COLOR_ARG2_PREVSTAGE
#define C_ARG1_INV              MGA_TDS_COLOR_ARG1_INV
#define C_ARG2_INV              MGA_TDS_COLOR_ARG2_INV
#define COLOR_MUL               MGA_TDS_COLOR_SEL_MUL
#define COLOR_ARG1              MGA_TDS_COLOR_SEL_ARG1
#define COLOR_ARG2              MGA_TDS_COLOR_SEL_ARG2
#define A_ARG1_CUR              0x0
#define A_ARG2_IGN              A_ARG2_DIFFUSE
#define A_ARG2_FCOL             MGA_TDS_ALPHA_ARG2_FCOL
#define A_ARG2_DIFFUSE          MGA_TDS_ALPHA_ARG2_DIFFUSE
#define A_ARG2_PREV             MGA_TDS_ALPHA_ARG2_PREVSTAGE
#define ALPHA_MUL               MGA_TDS_ALPHA_SEL_MUL
#define ALPHA_ARG1              MGA_TDS_ALPHA_SEL_ARG1
#define ALPHA_ARG2              MGA_TDS_ALPHA_SEL_ARG2

static Bool
mgaPrepareComposite(int op, PicturePtr pSrcPict, PicturePtr pMaskPict,
                    PicturePtr pDstPict, PixmapPtr pSrc, PixmapPtr pMask,
                    PixmapPtr pDst)
{
    PMGA(pDst);
    CARD32 fcol = 0xff000000, ds0 = 0, ds1 = 0, cmd, blendcntl;
    if (!pSrc || !pSrcPict->pDrawable) return FALSE;

    mgaSetup(pMga, pDst, pDstPict, 2);
    OUTREG(MGAREG_DSTORG, exaGetPixmapOffset(pDst));
    OUTREG(MGAREG_PITCH, mgaGetPixmapPitch(pDst));

    PrepareSourceTexture(0, pSrcPict, pSrc);

    if (pMask)
        PrepareSourceTexture(1, pMaskPict, pMask);
    else
        PrepareSourceTexture(1, pSrcPict, pSrc);

    /* For A8 writes, the desired alpha value needs to be replicated
     * to the color channels - if the source texture doesn't have an
     * alpha channel, set it to 0xff instead.
     * Otherwise, use the color channels if available, or set them
     * to black.
     */
    if (pDstPict->format == PICT_a8 && !PICT_FORMAT_A(pSrcPict->format)) {
        fcol = 0xffffffff;
        ds0 |= C_ARG2_FCOL | COLOR_ARG2; /* C = 0xff */
    } else if (pDstPict->format == PICT_a8)
        ds0 |= C_ARG1_ALPHA | COLOR_ARG1; /* C = As */
    else if (pSrcPict->format == PICT_a8)
        ds0 |= C_ARG2_FCOL | COLOR_ARG2; /* C = 0 */
    else
        ds0 |= C_ARG1_CUR | COLOR_ARG1; /* C = Cs */

    /* If the source texture has an alpha channel, use it.
     * Otherwise, set the alpha channel to 0xff (see FCOL setting above).
     */
    if (PICT_FORMAT_A(pSrcPict->format))
        ds0 |= A_ARG1_CUR | ALPHA_ARG1; /* A = As */
    else
        ds0 |= A_ARG2_FCOL | ALPHA_ARG2; /* A = 0xff */

    if (!pMask)
        ds1 = ds0;
    else {
        /* As or Am might be NULL. in this case we don't multiply because,
         * the alpha component holds garbage.
         */
        if (!PICT_FORMAT_A(pMaskPict->format))
            ds1 |= C_ARG2_PREV | COLOR_ARG2; /* C = Cs */
        else
            ds1 |= C_ARG1_ALPHA | C_ARG2_PREV | COLOR_MUL; /* C = Am * Cs */

        if (!PICT_FORMAT_A(pMaskPict->format))
            ds1 |= A_ARG2_PREV | ALPHA_ARG2; /* A = As */
        else if (!PICT_FORMAT_A(pSrcPict->format))
            ds1 |= A_ARG1_CUR | ALPHA_ARG1; /* A = Am */
        else
            ds1 |= A_ARG1_CUR | A_ARG2_PREV | ALPHA_MUL; /* A = Am * As */
    }

    cmd = MGADWG_TEXTURE_TRAP | MGADWG_I | 0x000c0000 |
          MGADWG_SHIFTZERO | MGADWG_SGNZERO | MGADWG_ARZERO;
    blendcntl = mgaBlendOp[op].blend_cntl;

    if (!PICT_FORMAT_A(pDstPict->format) && mgaBlendOp[op].dst_alpha) {
        int sblend = blendcntl & MGA_SRC_BLEND_MASK;

        if (sblend == MGA_SRC_DST_ALPHA)
            blendcntl = (blendcntl & ~MGA_SRC_BLEND_MASK) | MGA_SRC_ONE;
        else if (sblend == MGA_SRC_ONE_MINUS_DST_ALPHA)
            blendcntl = (blendcntl & ~MGA_SRC_BLEND_MASK) | MGA_SRC_ZERO;
    }

    WAITFIFO(5);
    OUTREG(MGAREG_FCOL, fcol);
    OUTREG(MGAREG_TDUALSTAGE0, ds0);
    OUTREG(MGAREG_TDUALSTAGE1, ds1);
    OUTREG(MGAREG_DWGCTL, cmd);
    OUTREG(MGAREG_ALPHACTRL, MGA_ALPHACHANNEL | blendcntl);

    pMga->currentSrcPicture = pSrcPict;
    pMga->currentMaskPicture = pMaskPict;
    pMga->currentSrc = pSrc;
    pMga->currentMask = pMask;

    pMga->src_w2 = MGA_LOG2 (pSrc->drawable.width);
    pMga->src_h2 = MGA_LOG2 (pSrc->drawable.height);

    if (pMask) {
        pMga->mask_w2 = MGA_LOG2 (pMask->drawable.width);
        pMga->mask_h2 = MGA_LOG2 (pMask->drawable.height);
    }

    return TRUE;
}

static void
mgaComposite(PixmapPtr pDst, int srcx, int srcy, int maskx, int masky,
             int dstx, int dsty, int w, int h)
{
    PMGA(pDst);
    PictTransformPtr t;
    int texctl2;

    srcx %= pMga->currentSrc->drawable.width;
    srcy %= pMga->currentSrc->drawable.height;

    if (pMga->currentMask) {
        maskx %= pMga->currentMask->drawable.width;
        masky %= pMga->currentMask->drawable.height;
    }

    t = pMga->currentSrcPicture->transform;

    if (t)
        setTMIncrementsRegs(pMga->currentSrc,
                            t->matrix[0][0],
                            t->matrix[0][1],
                            t->matrix[0][2] + (srcx << 16),
                            t->matrix[1][0],
                            t->matrix[1][1],
                            t->matrix[1][2] + (srcy << 16),
                            t->matrix[2][0],
                            t->matrix[2][1],
                            t->matrix[2][2],
                            20 - pMga->src_w2,
                            20 - pMga->src_h2);
    else
        setTMIncrementsRegs(pMga->currentSrc,
                            1 << 16, 0, srcx << 16,
                            0, 1 << 16, srcy << 16,
                            0, 0, 1 << 16,
                            20 - pMga->src_w2,
                            20 - pMga->src_h2);

    if (pMga->currentMask) {
        texctl2 = MGA_G400_TC2_MAGIC | MGA_TC2_CKSTRANSDIS | MGA_TC2_DUALTEX;

        WAITFIFO(1);
        OUTREG(MGAREG_TEXCTL2, texctl2 | MGA_TC2_SELECT_TMU1);

        t = pMga->currentMaskPicture->transform;

        if (t)
            setTMIncrementsRegs(pMga->currentMask,
                                t->matrix[0][0],
                                t->matrix[0][1],
                                t->matrix[0][2] + (maskx << 16),
                                t->matrix[1][0],
                                t->matrix[1][1],
                                t->matrix[1][2] + (masky << 16),
                                t->matrix[2][0],
                                t->matrix[2][1],
                                t->matrix[2][2],
                                20 - pMga->mask_w2,
                                20 - pMga->mask_h2);
        else
            setTMIncrementsRegs(pMga->currentMask,
                                1 << 16, 0, maskx << 16,
                                0, 1 << 16, masky << 16,
                                0, 0, 1 << 16,
                                20 - pMga->mask_w2,
                                20 - pMga->mask_h2);

        WAITFIFO(1);
        OUTREG(MGAREG_TEXCTL2, texctl2 & ~MGA_TC2_SELECT_TMU1);
    }

    WAITFIFO(2);
    OUTREG(MGAREG_FXBNDRY, ((dstx + w) << 16) | (dstx & 0xffff));
    OUTREG(MGAREG_YDSTLEN | MGAREG_EXEC, (dsty << 16) | (h & 0xffff));
}

static Bool
mgaUploadToScreen(PixmapPtr pDst, int x, int y, int w, int h,
                  char *src, int src_pitch)
{
    PMGA(pDst);
    int bytes_padded = ((pDst->drawable.bitsPerPixel * w + 31) / 32) * 4;

    QUIESCE_DMA(pDst);

    mgaSetup(pMga, pDst, NULL, 10);

    OUTREG(MGAREG_OPMODE, MGAOPM_DMA_BLIT);
    OUTREG(MGAREG_DSTORG, exaGetPixmapOffset(pDst));
    OUTREG(MGAREG_PITCH, mgaGetPixmapPitch(pDst));
    OUTREG(MGAREG_PLNWT, 0xffffffff);
    OUTREG(MGAREG_DWGCTL, MGADWG_ILOAD | MGADWG_BFCOL |
           MGADWG_SGNZERO | MGADWG_SHIFTZERO |
           MGADWG_RSTR | 0x000c0000);
    OUTREG(MGAREG_AR0, w - 1);
    OUTREG(MGAREG_AR3, 0);
    OUTREG(MGAREG_AR5, 0);
    OUTREG(MGAREG_FXBNDRY, ((x + w - 1) << 16) | (x & 0xffff));
    OUTREG(MGAREG_YDSTLEN | MGAREG_EXEC, (y << 16) | (h & 0xffff));

    while (h--) {
        memcpy (pMga->ILOADBase, src, bytes_padded);
        src += src_pitch;
    }

    exaMarkSync(pDst->drawable.pScreen);

    return TRUE;
}

static void
mgaWaitMarker(ScreenPtr pScreen, int marker)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    MGAPtr pMga = pScrn->driverPrivate;

    WAITFIFO(1);

    OUTREG(MGAREG_CACHEFLUSH, 0);

    /* wait until the "drawing engine busy" bit is unset */
    while (INREG (MGAREG_Status) & 0x10000);
}

#ifdef MGADRI
static void
init_dri(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);
    MGADRIServerPrivatePtr dri = pMga->DRIServerInfo;
    int cpp = pScrn->bitsPerPixel / 8;
    int widthBytes = pScrn->displayWidth * cpp;
    int bufferSize = ((pScrn->virtualY * widthBytes + MGA_BUFFER_ALIGN)
                      & ~MGA_BUFFER_ALIGN);
    int maxlines, mb;

    switch (pMga->Chipset) {
    case PCI_CHIP_MGAG200_SE_A_PCI:
    case PCI_CHIP_MGAG200_SE_B_PCI:
        mb = 1;
	break;
    default:
        mb = 16;
	break;
    }

    maxlines = (min(pMga->FbUsableSize, mb * 1024 * 1024)) /
               (pScrn->displayWidth * pMga->CurrentLayout.bitsPerPixel / 8);

    dri->frontOffset = 0;
    dri->frontPitch = widthBytes;

    /* Try for front, back, depth, and two framebuffers worth of
     * pixmap cache.  Should be enough for a fullscreen background
     * image plus some leftovers.
     */
    dri->textureSize = pMga->FbMapSize - 5 * bufferSize;

    /* If that gives us less than half the available memory, let's
     * be greedy and grab some more.  Sorry, I care more about 3D
     * performance than playing nicely, and you'll get around a full
     * framebuffer's worth of pixmap cache anyway.
     */
    if (dri->textureSize < (int)pMga->FbMapSize / 2) {
        dri->textureSize = pMga->FbMapSize - 4 * bufferSize;
    }

    /* Check to see if there is more room available after the maximum
     * scanline for textures.
     */
    if ((int) pMga->FbMapSize - maxlines * widthBytes - bufferSize * 2
        > dri->textureSize) {
        dri->textureSize = pMga->FbMapSize - maxlines * widthBytes -
                           bufferSize * 2;
    }

    /* Set a minimum usable local texture heap size.  This will fit
     * two 256x256x32bpp textures.
     */
    if (dri->textureSize < 512 * 1024) {
        dri->textureOffset = 0;
        dri->textureSize = 0;
    }

    /* Reserve space for textures */
    dri->textureOffset = (pMga->FbMapSize - dri->textureSize +
                          MGA_BUFFER_ALIGN) & ~MGA_BUFFER_ALIGN;

    /* Reserve space for the shared depth buffer */
    dri->depthOffset = (dri->textureOffset - bufferSize +
                        MGA_BUFFER_ALIGN) & ~MGA_BUFFER_ALIGN;
    dri->depthPitch = widthBytes;

    /* Reserve space for the shared back buffer */
    dri->backOffset = (dri->depthOffset - bufferSize +
                       MGA_BUFFER_ALIGN) & ~MGA_BUFFER_ALIGN;
    dri->backPitch = widthBytes;
}
#endif /* MGADRI */

Bool
mgaExaInit(ScreenPtr pScreen)
{
    ExaDriverPtr pExa;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    MGAPtr pMga = pScrn->driverPrivate;

    pExa = exaDriverAlloc();
    if (!pExa) {
        pMga->NoAccel = TRUE;
        return FALSE;
    }

    pMga->ExaDriver = pExa;

    pExa->exa_major = EXA_VERSION_MAJOR;
    pExa->exa_minor = EXA_VERSION_MINOR;

    pExa->flags = EXA_OFFSCREEN_PIXMAPS | EXA_OFFSCREEN_ALIGN_POT;
    pExa->memoryBase = pMga->FbStart;
    pExa->memorySize = pMga->FbUsableSize - 4096;
    pExa->offScreenBase = (pScrn->virtualX * pScrn->virtualY *
                           pScrn->bitsPerPixel / 8) + 4096;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "X %d Y %d bpp %d\n",
               pScrn->virtualX, pScrn->virtualY, pScrn->bitsPerPixel);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Start at %p, size %lx, osb %lx\n",
               pExa->memoryBase, pExa->memorySize, pExa->offScreenBase);

    /* In PW24 mode, we need to align to "3 64-bytes" */
    pExa->pixmapOffsetAlign = 192;

    /* Pitch alignment is in sets of 32 pixels, and we need to cover
     * 32bpp, so it's 128 bytes.
     */
    pExa->pixmapPitchAlign = 128;

    pExa->maxX = 2048;
    pExa->maxY = 2048;

    pExa->WaitMarker = mgaWaitMarker;
    pExa->PrepareSolid = mgaPrepareSolid;
    pExa->Solid = mgaSolid;
    pExa->DoneSolid = mgaNoopDone;
    pExa->PrepareCopy = mgaPrepareCopy;
    pExa->Copy = mgaCopy;
    pExa->DoneCopy = mgaNoopDone;

    if (pMga->Chipset == PCI_CHIP_MGAG400 ||
        pMga->Chipset == PCI_CHIP_MGAG550) {
        pExa->CheckComposite = mgaCheckComposite;
        pExa->PrepareComposite = mgaPrepareComposite;
        pExa->Composite = mgaComposite;
        pExa->DoneComposite = mgaNoopDone;
    }

    pExa->UploadToScreen = mgaUploadToScreen;

#ifdef MGADRI
    if (pMga->directRenderingEnabled)
        init_dri(pScrn);
#endif

    return exaDriverInit(pScreen, pExa);
}
