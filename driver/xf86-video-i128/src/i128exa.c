/*
 * Copyright 1997-2000 by Robin Cutshaw <robin@XFree86.Org>
 * Copyright 2005 Adam Jackson <ajax@nwnk.net>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of the authors not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The authors make no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/* Solid and Copy support derived from the i128 XAA code */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "exa.h"
#include "miline.h"
#include "servermd.h"
#include "picture.h"

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Pci.h"
#include "xf86PciInfo.h"

#include "i128.h"
#include "i128reg.h"

#define PI128_FROM_PIXMAP(x) \
    I128Ptr pI128 = I128PTR(xf86ScreenToScrn(x->drawable.pScreen))
#define PI128_FROM_SCREEN(x) \
    I128Ptr pI128 = I128PTR(xf86ScreenToScrn(x))
#define PI128_FROM_PICTURE(x) \
    I128Ptr pI128 = I128PTR(xf86ScreenToScrn(x->pDrawable->pScreen))

/* we might be able to do something smarter than this */
#define ENG_PIPELINE_READY() \
    while (pI128->mem.rbase_a[BUSY] & BUSY_BUSY)
#define ENG_DONE() \
    while (pI128->mem.rbase_a[FLOW] & (FLOW_DEB | FLOW_MCB | FLOW_PRV))

#if 1
#define I128_EXA_DEBUG(x)
#else
#define I128_EXA_DEBUG(x) ErrorF x
#endif

/* technically we should set the caches to bogus things during init... */
#define CACHE_DEBUG 0

#define CACHED_UPDATE(val, reg) \
    do if (pI128->val != val) { \
        if (CACHE_DEBUG) I128_EXA_DEBUG(("Updated cache for " #reg "\n")); \
        pI128->mem.rbase_a[reg] = pI128->val = val; \
    } while (0)

static void
i128SetBufCtrl(I128Ptr pI128, int dest_bpp)
{
    unsigned int buf_ctrl;

    switch (dest_bpp) {
        case 8:  buf_ctrl = BC_PSIZ_8B;  break;
        case 16: buf_ctrl = BC_PSIZ_16B; break;
        case 24:
        case 32: buf_ctrl = BC_PSIZ_32B; break;
        default: buf_ctrl = 0;           break; /* error */
    }
    if (pI128->Chipset == PCI_CHIP_I128_T2R) {
        if (pI128->MemoryType == I128_MEMORY_SGRAM)
            buf_ctrl |= BC_MDM_PLN;
        else
            buf_ctrl |= BC_BLK_ENA;
    }

    CACHED_UPDATE(buf_ctrl, BUF_CTRL);
}

static const CARD32 i128alu[16] =
{
    CR_CLEAR << 8,
    CR_AND << 8,
    CR_AND_REV << 8,
    CR_COPY << 8,
    CR_AND_INV << 8,
    CR_NOOP << 8,
    CR_XOR << 8,
    CR_OR << 8,
    CR_NOR << 8,
    CR_EQUIV << 8,
    CR_INVERT << 8,
    CR_OR_REV << 8,
    CR_COPY_INV << 8,
    CR_OR_INV << 8,
    CR_NAND << 8,
    CR_SET << 8
};

/*                                8bpp   16bpp  32bpp unused */
static const int min_size[]   = { 0x62,  0x32,  0x1A, 0x00 };
static const int max_size[]   = { 0x80,  0x40,  0x20, 0x00 };
static const int split_size[] = { 0x20,  0x10,  0x08, 0x00 };

/*
 * this is the workhorse for our solid and copy routines.  this works because
 * when CS_SOLID is set, the color comes from the FORE register regardless of
 * the source pixmap coords.
 */

static void
i128ExaBlit(PixmapPtr dst, int x1, int y1, int x2, int y2, int w, int h)
{
    int wh;
    PI128_FROM_PIXMAP(dst);

    I128_EXA_DEBUG(("Blit: %d %d %d %d %d %d\n", x1, y1, x2, y2, w, h));
    ENG_PIPELINE_READY();

    /*
     * this deserves explanation.  XY3_DIR == 0 means left to right, top to
     * bottom.  setting bit zero (DIR_LR_BT) switches to bottom to top, and
     * setting bit one (DIR_RL_TB) switches to right to left. XXX rewrite me.
     */
    if (pI128->blitdir & DIR_RL_TB) { /* right-to-left */
        x1 += w; x1--;
        x2 += w; x2--;
    }
    if (pI128->blitdir & DIR_LR_BT) { /* bottom-to-top */
        y1 += h; y1--;
        y2 += h; y2--;
    }

    if (pI128->Chipset == PCI_CHIP_I128) {
        int bppi;

        /* The I128-1 has a nasty bitblit bug
         * that occurs when dest is exactly 8 pages wide
         */

        bppi = (pI128->mem.rbase_a[BUF_CTRL] & BC_PSIZ_MSK) >> 24;

        if ((w >= min_size[bppi]) && (w <= max_size[bppi])) {
            bppi = split_size[bppi];
#if 1
            /* split method */

            wh = (bppi << 16) | h;
            CACHED_UPDATE(wh, XY2_WH);
            pI128->mem.rbase_a[XY0_SRC] = (x1 << 16) | y1;    MB;
            pI128->mem.rbase_a[XY1_DST] = (x2 << 16) | y2;    MB;

            ENG_PIPELINE_READY();

            w -= bppi;

            if (pI128->blitdir & DIR_RL_TB) {
                /* right to left blit */
                x1 -= bppi;
                x2 -= bppi;
            } else {
                /* left to right blit */
                x1 += bppi;
                x2 += bppi;
            }
#else
            /* clip method */
            pI128->mem.rbase_a[CLPTL] = (x2 << 16) | y2;
            pI128->mem.rbase_a[CLPBR] = ((x2 + w) << 16) | (y2 + h);
            w += bppi;
#endif
        }
    }

    /* this is overkill, but you can never have too much overkill */
    wh = (w << 16) | h;
    CACHED_UPDATE(wh, XY2_WH);

    pI128->mem.rbase_a[XY0_SRC] = (x1 << 16) | y1;    MB;
    pI128->mem.rbase_a[XY1_DST] = (x2 << 16) | y2;    MB;
}

static void
i128WaitMarker(ScreenPtr pScreen, int Marker)
{
    PI128_FROM_SCREEN(pScreen);
    ENG_DONE();
}

static void
i128SetPlanemask(I128Ptr pI128, Pixel p)
{
    Pixel planemask;
    I128_EXA_DEBUG(("SetPlanemask: %d\n", (int)p));
    if (p == -1)
        planemask = -1;
    else switch (pI128->bitsPerPixel) {
        case 8:
            planemask = p * 0x01010101; break;
        case 16:
            planemask = p * 0x00010001; break;
        default:
            planemask = p; break;
    }

    CACHED_UPDATE(planemask, MASK);
}

/* this should be superfluous... */
static void
i128SetClip(I128Ptr pI128)
{
#if 0
    pI128->clptl = pI128->mem.rbase_a[CLPTL] = 0x00000000;
    pI128->clpbr = pI128->mem.rbase_a[CLPBR] = (4095 << 16) | 2047;
#endif
}

static void
i128SetBlitDirection(I128Ptr pI128, int dx, int dy)
{
    int blitdir;

    I128_EXA_DEBUG(("SetBlitDirection: %d %d\n", dx, dy));
    
    if (dx < 0) {
        if (dy < 0) blitdir = DIR_RL_BT;
        else blitdir = DIR_RL_TB;
    } else {
        if (dy < 0) blitdir = DIR_LR_BT;
        else blitdir = DIR_LR_TB;
    }

    CACHED_UPDATE(blitdir, XY3_DIR);
}

static void
i128SetRop(I128Ptr pI128, int alu, int solid)
{
    int cmd;

    I128_EXA_DEBUG(("SetRop: %d %d\n", alu, solid));

    cmd = i128alu[alu] | CO_BITBLT | (solid ? (CS_SOLID << 16) : 0);

    CACHED_UPDATE(cmd, CMD);
}

static void
i128SetSourcePixmap(I128Ptr pI128, PixmapPtr src)
{
    unsigned int sorg = exaGetPixmapOffset(src);
    unsigned int sptch = exaGetPixmapPitch(src);

    I128_EXA_DEBUG(("SetSourcePixmap: %x, %d\n", sorg, sptch));

    CACHED_UPDATE(sorg, DE_SORG);
    CACHED_UPDATE(sptch, DE_SPTCH);
}

static void
i128SetDestPixmap(I128Ptr pI128, PixmapPtr dst)
{
    unsigned int dorg = exaGetPixmapOffset(dst);
    unsigned int dptch = exaGetPixmapPitch(dst);
    
    I128_EXA_DEBUG(("SetDestPixmap: %x, %d\n", dorg, dptch));
    
    CACHED_UPDATE(dorg, DE_DORG);
    CACHED_UPDATE(dptch, DE_DPTCH);
}

static void
i128SetTexture(I128Ptr pI128, PixmapPtr tex)
{
    unsigned int torg = exaGetPixmapOffset(tex);
    unsigned int tptch = exaGetPixmapPitch(tex);

    I128_EXA_DEBUG(("SetTexture: %x, %d\n", torg, tptch));

    CACHED_UPDATE(torg, LOD0_ORG);
    CACHED_UPDATE(tptch, DE_TPTCH);
}

static const int func_tab[13][2] = {
        /* source function,         destination function */
        { ABLEND_SRC_ZERO,          ABLEND_DST_ZERO        }, /* clear */
        { ABLEND_SRC_ONE,           ABLEND_DST_ZERO        }, /* src */
        { ABLEND_SRC_ZERO,          ABLEND_DST_ONE         }, /* dst */
        { ABLEND_SRC_ONE,           ABLEND_DST_OMSRC_ALPHA }, /* over */
        { ABLEND_SRC_OMDST_ALPHA,   ABLEND_DST_ONE         }, /* overreverse */
        { ABLEND_SRC_DST_ALPHA,     ABLEND_DST_ZERO        }, /* in */
        { ABLEND_SRC_ZERO,          ABLEND_DST_SRC_ALPHA   }, /* inreverse */
        { ABLEND_SRC_OMDST_ALPHA,   ABLEND_DST_ZERO        }, /* out */
        { ABLEND_SRC_ZERO,          ABLEND_DST_OMSRC_ALPHA }, /* outreverse */
        { ABLEND_SRC_DST_ALPHA,     ABLEND_DST_OMSRC_ALPHA }, /* atop */
        { ABLEND_SRC_OMDST_ALPHA,   ABLEND_DST_SRC_ALPHA   }, /* atopreverse */
        { ABLEND_SRC_OMDST_ALPHA,   ABLEND_DST_OMSRC_ALPHA }, /* xor */
        { ABLEND_SRC_ONE,           ABLEND_DST_ONE         }  /* add */
};

static void
i128SetAlphaForOp(I128Ptr pI128, int op, int enable)
{
    int acntrl = 0;

    if (enable) {
        acntrl |= ACTL_BE;  /* blend enable */
        acntrl |= func_tab[op][0];  /* source factor */
        acntrl |= func_tab[op][1];  /* dest_factor */
        acntrl |= 0; /* ACTL_AMD; / * modulate alpha */
    } else {
        acntrl = 0;
    }

    I128_EXA_DEBUG(("SetAlphaForOp: %d, %d\n", op, enable));

    CACHED_UPDATE(acntrl, ACNTRL);
}

/* we don't need a finalizer, yet */
static void
i128Done(PixmapPtr p) {
    I128_EXA_DEBUG(("Done\n\n"));
    return;
}

/* Solid */

static Bool
i128PrepareSolid(PixmapPtr pPixmap, int alu, Pixel planemask, Pixel fg)
{
    PI128_FROM_PIXMAP(pPixmap);

    ENG_PIPELINE_READY();

    i128SetPlanemask(pI128, planemask);

    if (alu != GXclear && alu != GXset)
        pI128->mem.rbase_a[FORE] = fg;

    i128SetClip(pI128);
    i128SetBlitDirection(pI128, 1, 1); /* probably unnecessary/ignored */

    i128SetAlphaForOp(pI128, 0, 0);
    i128SetRop(pI128, alu, 1);

    /* no need to set the source, the chip ignores it */
    i128SetDestPixmap(pI128, pPixmap);
    
    return TRUE;
}

static void
i128Solid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2)
{
    i128ExaBlit(pPixmap, 0, 0, x1, y1, x2 - x1, y2 - y1);
}

/* Copy */

static Bool
i128PrepareCopy(PixmapPtr pSrcPixmap, PixmapPtr pDstPixmap, int dx, int dy,
                int alu, Pixel planemask)
{
    PI128_FROM_PIXMAP(pSrcPixmap);

    ENG_PIPELINE_READY();

    i128SetPlanemask(pI128, planemask);
    i128SetClip(pI128);
    i128SetBlitDirection(pI128, dx, dy);

    i128SetAlphaForOp(pI128, 0, 0);
    i128SetRop(pI128, alu, 0);

    i128SetSourcePixmap(pI128, pSrcPixmap);
    i128SetDestPixmap(pI128, pDstPixmap);
    
    return TRUE;
}

static void
i128Copy(PixmapPtr pDstPixmap, int x1, int y1, int x2, int y2, int w, int h)
{
    i128ExaBlit(pDstPixmap, x1, y1, x2, y2, w, h);
}

/* Composite */

static const struct source_format source_formats[] = {
    /* 32bpp */
    { PICT_a8r8g8b8,    0x14,   0,  0 },
    { PICT_x8r8g8b8,    0x14,   0,  1 },
#if 0
    { PICT_a8b8g8r8,    0x14,   0,  0 },
    { PICT_x8b8g8r8,    0x14,   0,  1 },
    /* no direct 24bpp formats */
    /* 16bpp */
    { PICT_r5g6b5,      0x12,   0,  0 },
    { PICT_b5g6r5,      0x12,   0,  0 },
    { PICT_a1r5g5b5,    0x11,   0,  0 },
    { PICT_x1r5g5b5,    0x11,   0,  1 },
    { PICT_a1b5g5r5,    0x11,   0,  0 },
    { PICT_x1b5g5r5,    0x11,   0,  1 },
    { PICT_a4r4g4b4,    0x10,   0,  0 },
    { PICT_x4r4g4b4,    0x10,   0,  1 },
    { PICT_a4b4g4r4,    0x10,   0,  0 },
    { PICT_x4b4g4r4,    0x10,   0,  1 },
    /* 8bpp */
    { PICT_a8,          0x21,   0,  0 },
    { PICT_r3g3b2,      0x0D,   0,  0 },
    { PICT_b2g3r3,      0x0D,   0,  0 },
    { PICT_a2r2g2b2,    0x30,   0,  0 },
    { PICT_a2b2g2r2,    0x30,   0,  0 },
    /* 4bpp */
    { PICT_a4,          0x20,   0,  0 },
#endif
    /* terminator */
    { 0,                0,      0,  0 }
};

static struct source_format *
i128MapSourceFormat(int fmt)
{
    struct source_format *f;
    for (f = (struct source_format *)source_formats; f->render_format; f++)
        if (f->render_format == fmt)
            return f;
    return NULL;
}

struct dest_format {
    int render_format;
    int i128_format;
};

static const struct dest_format dest_formats[] = {
    { 0, 0 }
};

#if 0
static struct dest_format *
i128MapDestFormat(int fmt)
{
    return NULL;
}
#endif

/* Composite is probably t2r and t2r4 only */
static Bool
i128CheckComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
                   PicturePtr pDstPicture)
{
    PI128_FROM_PICTURE(pDstPicture);

    if (op >= PictOpSaturate) return FALSE;

    /*
     * no direct alpha mask support.  we only have one TMU, so while we
     * can emulate it, we should emulate it in the generic EXA layer.
     */
    if (pMaskPicture) return FALSE;

    /* when transforms added, be sure to check for linear/nearest */
    if (pSrcPicture->transform && pSrcPicture->filter != PictFilterNearest)
        return FALSE;

    /* no support for external alpha */
    if (pSrcPicture->alphaMap || pDstPicture->alphaMap) return FALSE;

    /* driver currently doesn't support repeating */
    if (pSrcPicture->repeat) return FALSE;

    pI128->source = i128MapSourceFormat(pSrcPicture->format);
    if (!pI128->source)
        return FALSE;
#if 0
    if (!i128MapDestFormat(pDstPicture->format)) return FALSE;
#endif

    return TRUE;
}

static Bool
i128PrepareComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
                     PicturePtr pDstPicture, PixmapPtr pSrc, PixmapPtr pMask,
                     PixmapPtr pDst)
{
    unsigned int cmd;
    unsigned int tex_ctl = 0;
    unsigned int threedctl = 0;
    PI128_FROM_PIXMAP(pDst);

    /* 2D setup */
    i128SetBufCtrl(pI128, pDst->drawable.bitsPerPixel);
    i128SetPlanemask(pI128, -1);
    i128SetSourcePixmap(pI128, pSrc);
    i128SetDestPixmap(pI128, pDst);

    /* TEX_INV command here? */

    cmd = CO_TRIAN3D;
    CACHED_UPDATE(cmd, CMD);

    /* 3D setup */
    i128SetTexture(pI128, pSrc);
    
    i128SetAlphaForOp(pI128, op, 1);

    /* it looks pointless to cache these, but we'll need it for DRI */
    
    tex_ctl |= TEX_TM;  /* enable texture mapping */
    tex_ctl |= TEX_NMG | TEX_NMN;   /* nearest interpolation */
    tex_ctl |= 0; /* TEX_RM;  / * modulate RGB */
    CACHED_UPDATE(tex_ctl, TEX_CTL);

    threedctl |= 0; /* COMP_TRUE << TCTL_ZOP_SHIFT;   / * always pass Z check */
    threedctl |= TCTL_ABS;  /* enable alpha blend */
    threedctl |= TCTL_TBS;  /* enable texture blend */
    threedctl |= TCTL_RT;   /* draw textured rectangle */
    CACHED_UPDATE(threedctl, THREEDCTL);
     
    return TRUE;
}

static void
i128Composite(PixmapPtr pDst, int srcX, int srcY, int maskX, int maskY,
              int dstX, int dstY, int width, int height)
{
    PI128_FROM_PIXMAP(pDst);

    /*
     * vertex setup.  vertex layout must be:
     * V0    V1
     * V2   (V3 is implicit)
     */

    pI128->mem.rbase_af[V0_X] = 0.0;
    pI128->mem.rbase_af[V0_Y] = 0.0;
    pI128->mem.rbase_af[V0_W] = 1.0;
    pI128->mem.rbase_af[V0_U] = 0.0;
    pI128->mem.rbase_af[V0_V] = 0.0;
    pI128->mem.rbase_af[V1_X] = 300.0;
    pI128->mem.rbase_af[V1_Y] = 0.0;
    pI128->mem.rbase_af[V1_W] = 1.0;
    pI128->mem.rbase_af[V1_U] = 1.0;
    pI128->mem.rbase_af[V1_V] = 0.0;
    pI128->mem.rbase_af[V2_X] = 0.0;
    pI128->mem.rbase_af[V2_Y] = 300.0;
    pI128->mem.rbase_af[V2_W] = 1.0;
    pI128->mem.rbase_af[V2_U] = 0.0;
    pI128->mem.rbase_af[V2_V] = 1.0;

    /* and fire */
    pI128->mem.rbase_a[TRIGGER3D] = 1; MB;

#if 0
    static int i = 0;
    /* test for raster */
    if (!(i = (i + 1) % 32)) {
        ErrorF("Composite test: %d %d %d %d %d %d\n", srcX, srcY, dstX, dstY,
                width, height);
    }
    i128SetRop(pI128, GXxor, 0);
    i128ExaBlit(pDst, srcX, srcY, dstX, dstY, width, height);
#endif
}

#if 0
/*
 * upload and download will require a DRM.  AGP DMA only works on T2R4, and
 * then only for upload.  we could probably use memory windows on other chips,
 * but those have goofy alignment restrictions and need to be disabled when
 * not in use.
 */
static Bool
i128DownloadFromScreen(PixmapPtr pSrc, int x, int y, int w, int h, char *dst,
                       int dst_pitch)
{
}

static Bool
i128UploadToScreen(PixmapPtr pDst, int x, int y, int w, int h, char *src,
                   int src_pitch)
{
}
#endif

Bool
I128ExaInit(ScreenPtr pScreen)
{
    ExaDriverPtr pExa;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    I128Ptr pI128 = I128PTR(pScrn);

    if (!(pExa = exaDriverAlloc())) {
        pI128->NoAccel = TRUE;
        return FALSE;
    }
    pI128->ExaDriver = pExa;

    pExa->flags = EXA_OFFSCREEN_PIXMAPS | EXA_OFFSCREEN_ALIGN_POT;
    pExa->memoryBase = pI128->MemoryPtr;
    pExa->memorySize = pI128->MemorySize * 1024;
    pExa->offScreenBase = (pScrn->virtualX * pScrn->virtualY) *
                               (pScrn->bitsPerPixel / 8) + 4096;
    /* these two are probably right */
    pExa->pixmapOffsetAlign = 16;
    pExa->pixmapPitchAlign = 16;
    /* these two are guesses */
    pExa->maxX = 2048;
    pExa->maxY = 2048;

    pExa->WaitMarker = i128WaitMarker;

    pExa->PrepareSolid = i128PrepareSolid;
    pExa->Solid = i128Solid;
    pExa->DoneSolid = i128Done;

    pExa->PrepareCopy = i128PrepareCopy;
    pExa->Copy = i128Copy;
    pExa->DoneCopy = i128Done;

    if (0 && (pI128->Chipset == PCI_CHIP_I128_T2R ||
        pI128->Chipset == PCI_CHIP_I128_T2R4))
    {
#if 0
        pExa->DownloadFromScreen = i128DownloadFromScreen;
        pExa->UploadToScreen = i128UploadToScreen;
#endif
        pExa->CheckComposite = i128CheckComposite;
        pExa->PrepareComposite = i128PrepareComposite;
        pExa->Composite = i128Composite;
        pExa->DoneComposite = i128Done;
    }

    /*
     * XXX much of this is duplicated from the XAA code, but I expect the XAA
     * support to disappear eventually.
     */
    pI128->buf_ctrl = 0; /* force write */
    i128SetBufCtrl(pI128, pI128->bitsPerPixel);

    /* all of this needs to be properly documented */
    {
        pI128->mem.rbase_a[DE_PGE] = 0x00;
        pI128->mem.rbase_a[DE_SORG] = pI128->displayOffset;
        pI128->mem.rbase_a[DE_DORG] = pI128->displayOffset;
        pI128->mem.rbase_a[DE_MSRC] = 0x00;
        pI128->mem.rbase_a[DE_WKEY] = 0x00;
        pI128->mem.rbase_a[DE_SPTCH] = pI128->mem.rbase_g[DB_PTCH];
        pI128->mem.rbase_a[DE_DPTCH] = pI128->mem.rbase_g[DB_PTCH];
        if (pI128->Chipset == PCI_CHIP_I128_T2R4)
            pI128->mem.rbase_a[DE_ZPTCH] = pI128->mem.rbase_g[DB_PTCH];
        pI128->mem.rbase_a[RMSK] = 0x00000000;
        pI128->mem.rbase_a[XY4_ZM] = ZOOM_NONE;
        pI128->mem.rbase_a[LPAT] = 0xffffffff;  /* for lines */
        pI128->mem.rbase_a[PCTRL] = 0x00000000; /* for lines */
        pI128->mem.rbase_a[CLPTL] = 0x00000000;
        pI128->mem.rbase_a[CLPBR] = (4095 << 16) | 2047 ;
        pI128->mem.rbase_a[ACNTRL] = 0x00000000;
        pI128->mem.rbase_a[INTM] = 0x03;
    }

    /* need this as a float * for vertex setup */
    pI128->mem.rbase_af = (float *)pI128->mem.rbase_a;
    
    if (pI128->Debug) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "I128ExaInit done\n");
        I128DumpActiveRegisters(pScrn);
    }

    return(exaDriverInit(pScreen, pExa));
}
