/*
 * Copyright 1998-2008 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2003 S3 Graphics, Inc. All Rights Reserved.
 * Copyright 2006 Thomas Hellström. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * 2D acceleration functions for the VIA/S3G UniChrome IGPs.
 *
 * Mostly rewritten, and modified for EXA support, by Thomas Hellström.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <X11/Xarch.h>
#include "miline.h"

#include "via_driver.h"
#include "via_regs.h"
#include "via_dmabuffer.h"
#include "via_rop.h"

/*
 * Check if we can use a planeMask and update the 2D context accordingly.
 */
static Bool
viaAccelPlaneMaskHelper_H2(ViaTwodContext * tdc, CARD32 planeMask)
{
    CARD32 modeMask = (1 << ((1 << tdc->bytesPPShift) << 3)) - 1;
    CARD32 curMask = 0x00000000;
    CARD32 curByteMask;
    int i;

    if ((planeMask & modeMask) != modeMask) {

        /* Masking doesn't work in 8bpp. */
        if (modeMask == 0xFF) {
            tdc->keyControl &= 0x0FFFFFFF;
            return FALSE;
        }

        /* Translate the bit planemask to a byte planemask. */
        for (i = 0; i < (1 << tdc->bytesPPShift); ++i) {
            curByteMask = (0xFF << (i << 3));

            if ((planeMask & curByteMask) == 0) {
                curMask |= (1 << i);
            } else if ((planeMask & curByteMask) != curByteMask) {
                tdc->keyControl &= 0x0FFFFFFF;
                return FALSE;
            }
        }
        ErrorF("DEBUG: planeMask 0x%08x, curMask 0%02x\n",
               (unsigned)planeMask, (unsigned)curMask);

        tdc->keyControl = (tdc->keyControl & 0x0FFFFFFF) | (curMask << 28);
    }

    return TRUE;
}

/*
 * Emit transparency state and color to the command buffer.
 */
static void
viaAccelTransparentHelper_H2(VIAPtr pVia, CARD32 keyControl,
                          CARD32 transColor, Bool usePlaneMask)
{
    ViaTwodContext *tdc = &pVia->td;

    RING_VARS;

    tdc->keyControl &= ((usePlaneMask) ? 0xF0000000 : 0x00000000);
    tdc->keyControl |= (keyControl & 0x0FFFFFFF);
    BEGIN_RING(4);
    OUT_RING_H1(VIA_REG_KEYCONTROL, tdc->keyControl);
    if (keyControl) {
        OUT_RING_H1(VIA_REG_SRCCOLORKEY, transColor);
    }
}

/*
 * Mark Sync using the 2D blitter for AGP. NoOp for PCI.
 * In the future one could even launch a NULL PCI DMA command
 * to have an interrupt generated, provided it is possible to
 * write to the PCI DMA engines from the AGP command stream.
 */
int
viaAccelMarkSync_H2(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);

    RING_VARS;

    ++pVia->curMarker;

    /* Wrap around without affecting the sign bit. */
    pVia->curMarker &= 0x7FFFFFFF;

    if (pVia->agpDMA) {
        BEGIN_RING(16);
        OUT_RING_H1(VIA_REG_KEYCONTROL, 0x00);
        OUT_RING_H1(VIA_REG_GEMODE, VIA_GEM_32bpp);
        OUT_RING_H1(VIA_REG_DSTBASE, pVia->markerOffset >> 3);
        OUT_RING_H1(VIA_REG_PITCH, VIA_PITCH_ENABLE);
        OUT_RING_H1(VIA_REG_DSTPOS, 0);
        OUT_RING_H1(VIA_REG_DIMENSION, 0);
        OUT_RING_H1(VIA_REG_FGCOLOR, pVia->curMarker);
        OUT_RING_H1(VIA_REG_GECMD, (0xF0 << 24) | VIA_GEC_BLT | VIA_GEC_FIXCOLOR_PAT);

        ADVANCE_RING;
    }
    return pVia->curMarker;
}

/*
 * Exa functions. It is assumed that EXA does not exceed the blitter limits.
 */
Bool
viaExaPrepareSolid_H2(PixmapPtr pPixmap, int alu, Pixel planeMask, Pixel fg)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPixmap->drawable.pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    ViaTwodContext *tdc = &pVia->td;

    if (exaGetPixmapPitch(pPixmap) & 7)
        return FALSE;

    if (!viaAccelSetMode(pPixmap->drawable.depth, tdc))
        return FALSE;

    if (!viaAccelPlaneMaskHelper_H2(tdc, planeMask))
        return FALSE;

    viaAccelTransparentHelper_H2(pVia, 0x0, 0x0, TRUE);

    tdc->cmd = VIA_GEC_BLT | VIA_GEC_FIXCOLOR_PAT | VIAACCELPATTERNROP(alu);

    tdc->fgColor = fg;

    return TRUE;
}

void
viaExaSolid_H2(PixmapPtr pPixmap, int x1, int y1, int x2, int y2)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPixmap->drawable.pScreen);
    CARD32 dstOffset = exaGetPixmapOffset(pPixmap);
    CARD32 dstPitch = exaGetPixmapPitch(pPixmap);
    int w = x2 - x1, h = y2 - y1;
    VIAPtr pVia = VIAPTR(pScrn);
    ViaTwodContext *tdc = &pVia->td;

    RING_VARS;

    BEGIN_RING(14);
    OUT_RING_H1(VIA_REG_GEMODE, tdc->mode);
    OUT_RING_H1(VIA_REG_DSTBASE, dstOffset >> 3);
    OUT_RING_H1(VIA_REG_PITCH, VIA_PITCH_ENABLE | (dstPitch >> 3) << 16);
    OUT_RING_H1(VIA_REG_DSTPOS, (y1 << 16) | (x1 & 0xFFFF));
    OUT_RING_H1(VIA_REG_DIMENSION, ((h - 1) << 16) | (w - 1));
    OUT_RING_H1(VIA_REG_FGCOLOR, tdc->fgColor);
    OUT_RING_H1(VIA_REG_GECMD, tdc->cmd);

    ADVANCE_RING;
}

void
viaExaDoneSolidCopy_H2(PixmapPtr pPixmap)
{
}

Bool
viaExaPrepareCopy_H2(PixmapPtr pSrcPixmap, PixmapPtr pDstPixmap, int xdir,
                        int ydir, int alu, Pixel planeMask)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDstPixmap->drawable.pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    ViaTwodContext *tdc = &pVia->td;

    if (pSrcPixmap->drawable.bitsPerPixel != pDstPixmap->drawable.bitsPerPixel)
        return FALSE;

    if ((tdc->srcPitch = exaGetPixmapPitch(pSrcPixmap)) & 3)
        return FALSE;

    if (exaGetPixmapPitch(pDstPixmap) & 7)
        return FALSE;

    tdc->srcOffset = exaGetPixmapOffset(pSrcPixmap);

    tdc->cmd = VIA_GEC_BLT | VIAACCELCOPYROP(alu);
    if (xdir < 0)
        tdc->cmd |= VIA_GEC_DECX;
    if (ydir < 0)
        tdc->cmd |= VIA_GEC_DECY;

    if (!viaAccelSetMode(pDstPixmap->drawable.bitsPerPixel, tdc))
        return FALSE;

    if (!viaAccelPlaneMaskHelper_H2(tdc, planeMask))
        return FALSE;
    viaAccelTransparentHelper_H2(pVia, 0x0, 0x0, TRUE);

    return TRUE;
}

void
viaExaCopy_H2(PixmapPtr pDstPixmap, int srcX, int srcY, int dstX, int dstY,
                int width, int height)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDstPixmap->drawable.pScreen);
    CARD32 dstOffset = exaGetPixmapOffset(pDstPixmap), val;
    CARD32 dstPitch = exaGetPixmapPitch(pDstPixmap);
    VIAPtr pVia = VIAPTR(pScrn);
    ViaTwodContext *tdc = &pVia->td;

    RING_VARS;

    if (!width || !height)
        return;

    if (tdc->cmd & VIA_GEC_DECY) {
        srcY += height - 1;
        dstY += height - 1;
    }

    if (tdc->cmd & VIA_GEC_DECX) {
        srcX += width - 1;
        dstX += width - 1;
    }
    val = VIA_PITCH_ENABLE | (dstPitch >> 3) << 16 | (tdc->srcPitch >> 3);

    BEGIN_RING(16);
    OUT_RING_H1(VIA_REG_GEMODE, tdc->mode);
    OUT_RING_H1(VIA_REG_SRCBASE, tdc->srcOffset >> 3);
    OUT_RING_H1(VIA_REG_DSTBASE, dstOffset >> 3);
    OUT_RING_H1(VIA_REG_PITCH, val);
    OUT_RING_H1(VIA_REG_SRCPOS, (srcY << 16) | (srcX & 0xFFFF));
    OUT_RING_H1(VIA_REG_DSTPOS, (dstY << 16) | (dstX & 0xFFFF));
    OUT_RING_H1(VIA_REG_DIMENSION, ((height - 1) << 16) | (width - 1));
    OUT_RING_H1(VIA_REG_GECMD, tdc->cmd);

    ADVANCE_RING;
}

Bool
viaExaCheckComposite_H2(int op, PicturePtr pSrcPicture,
                        PicturePtr pMaskPicture, PicturePtr pDstPicture)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDstPicture->pDrawable->pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    Via3DState *v3d = &pVia->v3d;

    if (!pSrcPicture->pDrawable)
        return FALSE;

    /* Reject small composites early. They are done much faster in software. */
    if (!pSrcPicture->repeat &&
        pSrcPicture->pDrawable->width *
        pSrcPicture->pDrawable->height < VIA_MIN_COMPOSITE)
        return FALSE;

    if (pMaskPicture && pMaskPicture->pDrawable &&
        !pMaskPicture->repeat &&
        pMaskPicture->pDrawable->width *
        pMaskPicture->pDrawable->height < VIA_MIN_COMPOSITE)
        return FALSE;

    if (pMaskPicture && pMaskPicture->repeat &&
        pMaskPicture->repeatType != RepeatNormal)
        return FALSE;

    if (pMaskPicture && pMaskPicture->componentAlpha) {
#ifdef VIA_DEBUG_COMPOSITE
        viaExaPrintCompositeInfo("Component Alpha operation", op,  pSrcPicture, pMaskPicture, pDstPicture);
#endif
        return FALSE;
    }

    if (!v3d->opSupported(op)) {
#ifdef VIA_DEBUG_COMPOSITE
        viaExaPrintCompositeInfo("Operator not supported", op, pSrcPicture, pMaskPicture, pDstPicture);
#endif
        return FALSE;
    }

    /*
     * FIXME: A8 destination formats are currently not supported and do not
     * seem supported by the hardware, although there are some leftover
     * register settings apparent in the via_3d_reg.h file. We need to fix this
     * (if important), by using component ARGB8888 operations with bitmask.
     */

    if (!v3d->dstSupported(pDstPicture->format)) {
#ifdef VIA_DEBUG_COMPOSITE
        viaExaPrintCompositeInfo(" Destination format not supported", op, pSrcPicture, pMaskPicture, pDstPicture);
#endif
        return FALSE;
    }

    if (v3d->texSupported(pSrcPicture->format)) {
        if (pMaskPicture && (PICT_FORMAT_A(pMaskPicture->format) == 0 ||
                             !v3d->texSupported(pMaskPicture->format))) {
#ifdef VIA_DEBUG_COMPOSITE
            viaExaPrintCompositeInfo("Mask format not supported", op, pSrcPicture, pMaskPicture, pDstPicture);
#endif
            return FALSE;
        }
        return TRUE;
    }
#ifdef VIA_DEBUG_COMPOSITE
    viaExaPrintCompositeInfo("Src format not supported", op, pSrcPicture, pMaskPicture, pDstPicture);
#endif
    return FALSE;
}

Bool
viaExaPrepareComposite_H2(int op, PicturePtr pSrcPicture,
                            PicturePtr pMaskPicture, PicturePtr pDstPicture,
                            PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst)
{
    CARD32 height, width;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    Via3DState *v3d = &pVia->v3d;
    int curTex = 0;
    ViaTexBlendingModes srcMode;
    Bool isAGP;
    unsigned long offset;

    /* Workaround: EXA crash with new libcairo2 on a VIA VX800 (#298) */
    /* TODO Add real source only pictures */
    if (!pSrc) {
	    ErrorF("pSrc is NULL\n");
	    return FALSE;
	}

    v3d->setDestination(v3d, exaGetPixmapOffset(pDst),
                        exaGetPixmapPitch(pDst), pDstPicture->format);
    v3d->setCompositeOperator(v3d, op);
    v3d->setDrawing(v3d, 0x0c, 0xFFFFFFFF, 0x000000FF, 0xFF);

    viaOrder(pSrc->drawable.width, &width);
    viaOrder(pSrc->drawable.height, &height);

    /*
     * For one-pixel repeat mask pictures we avoid using multitexturing by
     * modifying the src's texture blending equation and feed the pixel
     * value as a constant alpha for the src's texture. Multitexturing on the
     * Unichromes seems somewhat slow, so this speeds up translucent windows.
     */

    srcMode = via_src;
    pVia->maskP = NULL;
    if (pMaskPicture &&
        (pMaskPicture->pDrawable->height == 1) &&
        (pMaskPicture->pDrawable->width == 1) &&
        pMaskPicture->repeat && viaExpandablePixel(pMaskPicture->format)) {
        pVia->maskP = pMask->devPrivate.ptr;
        pVia->maskFormat = pMaskPicture->format;
        pVia->componentAlpha = pMaskPicture->componentAlpha;
        srcMode = ((pMaskPicture->componentAlpha)
                   ? via_src_onepix_comp_mask : via_src_onepix_mask);
    }

    /*
     * One-Pixel repeat src pictures go as solid color instead of textures.
     * Speeds up window shadows.
     */

    pVia->srcP = NULL;
    if (pSrcPicture && pSrcPicture->repeat
        && (pSrcPicture->pDrawable->height == 1)
        && (pSrcPicture->pDrawable->width == 1)
        && viaExpandablePixel(pSrcPicture->format)) {
        pVia->srcP = pSrc->devPrivate.ptr;
        pVia->srcFormat = pSrcPicture->format;
    }

    /* Exa should be smart enough to eliminate this IN operation. */
    if (pVia->srcP && pVia->maskP) {
        ErrorF("Bad one-pixel IN composite operation. "
               "EXA needs to be smarter.\n");
        return FALSE;
    }

    if (!pVia->srcP) {
        offset = exaGetPixmapOffset(pSrc);
        isAGP = viaIsAGP(pVia, pSrc, &offset);
        if (!isAGP && !viaExaIsOffscreen(pSrc))
            return FALSE;
        if (!v3d->setTexture(v3d, curTex, offset,
                             exaGetPixmapPitch(pSrc), pVia->nPOT[curTex],
                             1 << width, 1 << height, pSrcPicture->format,
                             via_repeat, via_repeat, srcMode, isAGP)) {
            return FALSE;
        }
        curTex++;
    }

    if (pMaskPicture && !pVia->maskP) {
        offset = exaGetPixmapOffset(pMask);
        isAGP = viaIsAGP(pVia, pMask, &offset);
        if (!isAGP && !viaExaIsOffscreen(pMask))
            return FALSE;
        viaOrder(pMask->drawable.width, &width);
        viaOrder(pMask->drawable.height, &height);
        if (!v3d->setTexture(v3d, curTex, offset,
                             exaGetPixmapPitch(pMask), pVia->nPOT[curTex],
                             1 << width, 1 << height, pMaskPicture->format,
                             via_repeat, via_repeat,
                             ((pMaskPicture->componentAlpha)
                              ? via_comp_mask : via_mask), isAGP)) {
            return FALSE;
        }
        curTex++;
    }

    v3d->setFlags(v3d, curTex, FALSE, TRUE, TRUE);
    v3d->emitState(v3d, &pVia->cb, viaCheckUpload(pScrn, v3d));
    v3d->emitClipRect(v3d, &pVia->cb, 0, 0, pDst->drawable.width,
                      pDst->drawable.height);

    return TRUE;
}

void
viaExaComposite_H2(PixmapPtr pDst, int srcX, int srcY, int maskX, int maskY,
                    int dstX, int dstY, int width, int height)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    Via3DState *v3d = &pVia->v3d;
    CARD32 col;

    if (pVia->maskP) {
        viaPixelARGB8888(pVia->maskFormat, pVia->maskP, &col);
        v3d->setTexBlendCol(v3d, 0, pVia->componentAlpha, col);
    }
    if (pVia->srcP) {
        viaPixelARGB8888(pVia->srcFormat, pVia->srcP, &col);
        v3d->setDrawing(v3d, 0x0c, 0xFFFFFFFF, col & 0x00FFFFFF, col >> 24);
        srcX = maskX;
        srcY = maskY;
    }

    if (pVia->maskP || pVia->srcP)
        v3d->emitState(v3d, &pVia->cb, viaCheckUpload(pScrn, v3d));

    v3d->emitQuad(v3d, &pVia->cb, dstX, dstY, srcX, srcY, maskX, maskY,
                  width, height);
}

void
viaAccelTextureBlit(ScrnInfoPtr pScrn, unsigned long srcOffset,
                    unsigned srcPitch, unsigned w, unsigned h, unsigned srcX,
                    unsigned srcY, unsigned srcFormat, unsigned long dstOffset,
                    unsigned dstPitch, unsigned dstX, unsigned dstY,
                    unsigned dstFormat, int rotate)
{
    VIAPtr pVia = VIAPTR(pScrn);
    CARD32 wOrder, hOrder;
    Via3DState *v3d = &pVia->v3d;

    viaOrder(w, &wOrder);
    viaOrder(h, &hOrder);

    v3d->setDestination(v3d, dstOffset, dstPitch, dstFormat);
    v3d->setDrawing(v3d, 0x0c, 0xFFFFFFFF, 0x000000FF, 0x00);
    v3d->setFlags(v3d, 1, TRUE, TRUE, FALSE);
    v3d->setTexture(v3d, 0, srcOffset, srcPitch, TRUE,
                    1 << wOrder, 1 << hOrder, srcFormat,
                    via_single, via_single, via_src, FALSE);
    v3d->emitState(v3d, &pVia->cb, viaCheckUpload(pScrn, v3d));
    v3d->emitClipRect(v3d, &pVia->cb, dstX, dstY, w, h);
    v3d->emitQuad(v3d, &pVia->cb, dstX, dstY, srcX, srcY, 0, 0, w, h);
}
