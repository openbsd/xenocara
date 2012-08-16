/*
 * Copyright (c) 2007 NVIDIA, Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "g80_type.h"
#include "g80_dma.h"
#include "g80_xaa.h"

static void
waitMarker(ScreenPtr pScreen, int marker)
{
    G80Sync(xf86ScreenToScrn(pScreen));
}

static Bool
setSrc(G80Ptr pNv, PixmapPtr pSrc)
{
    CARD32 depth;

    switch(pSrc->drawable.depth) {
        case  8: depth = 0x000000f3; break;
        case 15: depth = 0x000000f8; break;
        case 16: depth = 0x000000e8; break;
        case 24: depth = 0x000000e6; break;
        case 32: depth = 0x000000cf; break;
        default: return FALSE;
    }

    G80DmaStart(pNv, 0x230, 2);
    G80DmaNext (pNv, depth);
    G80DmaNext (pNv, 0x00000001);
    G80DmaStart(pNv, 0x244, 5);
    G80DmaNext (pNv, exaGetPixmapPitch(pSrc));
    G80DmaNext (pNv, pSrc->drawable.width);
    G80DmaNext (pNv, pSrc->drawable.height);
    G80DmaNext (pNv, 0x00000000);
    G80DmaNext (pNv, exaGetPixmapOffset(pSrc));

    return TRUE;
}

static Bool
setDst(G80Ptr pNv, PixmapPtr pDst)
{
    CARD32 depth, depth2;

    switch(pDst->drawable.depth) {
        case  8: depth = 0x000000f3; depth2 = 3; break;
        case 15: depth = 0x000000f8; depth2 = 1; break;
        case 16: depth = 0x000000e8; depth2 = 0; break;
        case 24: depth = 0x000000e6; depth2 = 2; break;
        case 32: depth = 0x000000cf; depth2 = 2; break;
        default: return FALSE;
    }

    G80DmaStart(pNv, 0x200, 2);
    G80DmaNext (pNv, depth);
    G80DmaNext (pNv, 0x00000001);
    G80DmaStart(pNv, 0x214, 5);
    G80DmaNext (pNv, exaGetPixmapPitch(pDst));
    G80DmaNext (pNv, pDst->drawable.width);
    G80DmaNext (pNv, pDst->drawable.height);
    G80DmaNext (pNv, 0x00000000);
    G80DmaNext (pNv, exaGetPixmapOffset(pDst));
    G80DmaStart(pNv, 0x2e8, 1);
    G80DmaNext (pNv, depth2);
    G80DmaStart(pNv, 0x584, 1);
    G80DmaNext (pNv, depth);
    G80SetClip(pNv, 0, 0, pDst->drawable.width, pDst->drawable.height);

    return TRUE;
}

/* solid fills */

static Bool
prepareSolid(PixmapPtr      pPixmap,
             int            alu,
             Pixel          planemask,
             Pixel          fg)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPixmap->drawable.pScreen);
    G80Ptr pNv = G80PTR(pScrn);

    if(pPixmap->drawable.depth > 24) return FALSE;
    if(!setDst(pNv, pPixmap)) return FALSE;
    G80DmaStart(pNv, 0x2ac, 1);
    G80DmaNext (pNv, 4);
    G80SetRopSolid(pNv, alu, planemask);
    G80DmaStart(pNv, 0x580, 1);
    G80DmaNext (pNv, 4);
    G80DmaStart(pNv, 0x588, 1);
    G80DmaNext (pNv, fg);

    pNv->DMAKickoffCallback = G80DMAKickoffCallback;
    return TRUE;
}

static void
solid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPixmap->drawable.pScreen);
    G80Ptr pNv = G80PTR(pScrn);

    G80DmaStart(pNv, 0x600, 4);
    G80DmaNext (pNv, x1);
    G80DmaNext (pNv, y1);
    G80DmaNext (pNv, x2);
    G80DmaNext (pNv, y2);

    if((x2 - x1) * (y2 - y1) >= 512)
        G80DmaKickoff(pNv);
}

static void
doneSolid(PixmapPtr pPixmap)
{
}

/* screen to screen copies */

static Bool
prepareCopy(PixmapPtr       pSrcPixmap,
            PixmapPtr       pDstPixmap,
            int             dx,
            int             dy,
            int             alu,
            Pixel           planemask)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDstPixmap->drawable.pScreen);
    G80Ptr pNv = G80PTR(pScrn);

    if(!setSrc(pNv, pSrcPixmap)) return FALSE;
    if(!setDst(pNv, pDstPixmap)) return FALSE;
    G80DmaStart(pNv, 0x2ac, 1);
    if(alu == GXcopy && planemask == ~0) {
        G80DmaNext (pNv, 3);
    } else {
        G80DmaNext (pNv, 4);
        G80SetRopSolid(pNv, alu, planemask);
    }
    pNv->DMAKickoffCallback = G80DMAKickoffCallback;
    return TRUE;
}

static void
copy(PixmapPtr pDstPixmap,
     int       srcX,
     int       srcY,
     int       dstX,
     int       dstY,
     int       width,
     int       height)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDstPixmap->drawable.pScreen);
    G80Ptr pNv = G80PTR(pScrn);

    G80DmaStart(pNv, 0x110, 1);
    G80DmaNext (pNv, 0);
    G80DmaStart(pNv, 0x8b0, 12);
    G80DmaNext (pNv, dstX);
    G80DmaNext (pNv, dstY);
    G80DmaNext (pNv, width);
    G80DmaNext (pNv, height);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, 1);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, 1);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, srcX);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, srcY);

    if(width * height >= 512)
        G80DmaKickoff(pNv);
}

static void
doneCopy(PixmapPtr pDstPixmap)
{
}

/* composite */

static Bool
checkComposite(int          op,
               PicturePtr   pSrc,
               PicturePtr   pMask,
               PicturePtr   pDst)
{
    return FALSE;
}

/* upload to screen */

static Bool
upload(PixmapPtr pDst,
       int       x,
       int       y,
       int       w,
       int       h,
       char      *src,
       int       src_pitch)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
    G80Ptr pNv = G80PTR(pScrn);
    const int Bpp = pDst->drawable.bitsPerPixel >> 3;
    int line_dwords = (w * Bpp + 3) / 4;
    const Bool kickoff = w * h >= 512;
    CARD32 depth;

    if(!setDst(pNv, pDst)) return FALSE;
    switch(pDst->drawable.depth) {
        case  8: depth = 0x000000f3; break;
        case 15: depth = 0x000000f8; break;
        case 16: depth = 0x000000e8; break;
        case 24: depth = 0x000000e6; break;
        case 32: depth = 0x000000cf; break;
        default: return FALSE;
    }

    G80SetClip(pNv, x, y, w, h);
    G80DmaStart(pNv, 0x2ac, 1);
    G80DmaNext (pNv, 3);
    G80DmaStart(pNv, 0x800, 2);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, depth);
    G80DmaStart(pNv, 0x838, 10);
    G80DmaNext (pNv, (line_dwords * 4) / Bpp);
    G80DmaNext (pNv, h);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, 1);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, 1);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, x);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, y);

    while(h-- > 0) {
        int count = line_dwords;
        char *p = src;

        while(count) {
            int size = count > 1792 ? 1792 : count;

            G80DmaStart(pNv, 0x40000860, size);
            memcpy(&pNv->dmaBase[pNv->dmaCurrent], p, size * 4);

            p += size * Bpp;
            pNv->dmaCurrent += size;

            count -= size;
        }

        src += src_pitch;
    }

    if(kickoff)
        G80DmaKickoff(pNv);
    else
        pNv->DMAKickoffCallback = G80DMAKickoffCallback;

    return TRUE;
}

/******************************************************************************/

Bool G80ExaInit(ScreenPtr pScreen, ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);
    ExaDriverPtr exa;
    const int pitch = pScrn->displayWidth * (pScrn->bitsPerPixel / 8);

    exa = pNv->exa = exaDriverAlloc();
    if(!exa) return FALSE;

    exa->exa_major         = EXA_VERSION_MAJOR;
    exa->exa_minor         = EXA_VERSION_MINOR;
    exa->memoryBase        = pNv->mem;
    exa->offScreenBase     = 0;
    exa->memorySize        = pitch * pNv->offscreenHeight;
    exa->pixmapOffsetAlign = 256;
    exa->pixmapPitchAlign  = 256;
    exa->flags             = EXA_OFFSCREEN_PIXMAPS;
    exa->maxX              = 8192;
    exa->maxY              = 8192;

    /**** Rendering ops ****/
    exa->PrepareSolid     = prepareSolid;
    exa->Solid            = solid;
    exa->DoneSolid        = doneSolid;
    exa->PrepareCopy      = prepareCopy;
    exa->Copy             = copy;
    exa->DoneCopy         = doneCopy;
    exa->CheckComposite   = checkComposite;
    //exa->PrepareComposite = prepareComposite;
    //exa->Composite        = composite;
    //exa->DoneComposite    = doneComposite;
    exa->UploadToScreen   = upload;

    exa->WaitMarker       = waitMarker;

    return exaDriverInit(pScreen, exa);
}
