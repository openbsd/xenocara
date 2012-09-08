/*
 * Copyright 2006 by Alan Hourihane, North Wales, UK.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the authors not be used in
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
 *
 * Authors:  Alan Hourihane, <alanh@fairlite.demon.co.uk>
 *
 * Trident XP4/XP5 accelerated options.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "exa.h"
#include "picture.h"

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Pci.h"
#include "xaarop.h"

#include "trident.h"
#include "trident_regs.h"

static int ropcode;

static int CopyROP[16] =
{
   ROP_0,               /* GXclear */
   ROP_DSa,             /* GXand */
   ROP_SDna,            /* GXandReverse */
   ROP_S,               /* GXcopy */
   ROP_DSna,            /* GXandInverted */
   ROP_D,               /* GXnoop */
   ROP_DSx,             /* GXxor */
   ROP_DSo,             /* GXor */
   ROP_DSon,            /* GXnor */
   ROP_DSxn,            /* GXequiv */
   ROP_Dn,              /* GXinvert*/
   ROP_SDno,            /* GXorReverse */
   ROP_Sn,              /* GXcopyInverted */
   ROP_DSno,            /* GXorInverted */
   ROP_DSan,            /* GXnand */
   ROP_1                /* GXset */
};

static int PatternROP[16]=
{
   ROP_0,
   ROP_DPa,
   ROP_PDna,
   ROP_P,
   ROP_DPna,
   ROP_D,
   ROP_DPx,
   ROP_DPo,
   ROP_DPon,
   ROP_PDxn,
   ROP_Dn,
   ROP_PDno,
   ROP_Pn,
   ROP_DPno,
   ROP_DPan,
   ROP_1
};

static int GetCopyROP(int i)
{
    return CopyROP[i];
}

static int GetPatternROP(int i)
{
    return PatternROP[i];
}

static void
XP4WaitMarker(ScreenPtr pScreen, int Marker)
{
	/* Don't need a wait marker as we need to sync on all operations */
}

static void
XP4Done(PixmapPtr p) {
    ScrnInfoPtr pScrn = xf86ScreenToScrn(p->drawable.pScreen);
    TRIDENTPtr pTrident = TRIDENTPTR(pScrn);
    int count = 0, timeout = 0;
    int busy;

    for (;;) {
	BLTBUSY(busy);
	if (busy != GE_BUSY) {
	    return;
	}
	count++;
	if (count == 10000000) {
	    ErrorF("XP: BitBLT engine time-out.\n");
	    count = 9990000;
	    timeout++;
	    if (timeout == 4) {
		/* Reset BitBLT Engine */
		TGUI_STATUS(0x00);
		return;
	    }
	}
    }
}

static Bool
XP4PrepareSolid(PixmapPtr pPixmap, int alu, Pixel planemask, Pixel fg)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPixmap->drawable.pScreen);
    TRIDENTPtr pTrident = TRIDENTPTR(pScrn);
    unsigned int dorg = exaGetPixmapOffset(pPixmap);
    unsigned int dptch = exaGetPixmapPitch(pPixmap);

    if (planemask != -1)
	    return FALSE;

    ropcode = alu;

    MMIO_OUT32(pTrident->IOBase, 0x2150, (dptch << 18) | (dorg >> 4));

    REPLICATE(fg);
    MMIO_OUT32(pTrident->IOBase, 0x2158, fg);
    MMIO_OUT32(pTrident->IOBase, 0x2128, 1<<14);

    return TRUE;
}

static void
XP4Solid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPixmap->drawable.pScreen);
    TRIDENTPtr pTrident = TRIDENTPTR(pScrn);
    int bpp;
    
    switch (pPixmap->drawable.bitsPerPixel) {
	case 8:
	    bpp = 0x40;
	    break;
	case 16:
	    bpp = 0x41;
	    break;
	case 32:
	    bpp = 0x42;
	    break;
    }

    MMIO_OUT32(pTrident->IOBase, 0x2138, x1<<16 | y1);
    MMIO_OUT32(pTrident->IOBase, 0x2140, (x2-x1)<<16 | (y2-y1));
    MMIO_OUT32(pTrident->IOBase, 0x2124, GetPatternROP(ropcode) << 24 | bpp << 8 | 2);
}

static Bool
XP4PrepareCopy(PixmapPtr pSrcPixmap, PixmapPtr pDstPixmap, int dx, int dy,
                int alu, Pixel planemask)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDstPixmap->drawable.pScreen);
    TRIDENTPtr pTrident = TRIDENTPTR(pScrn);
    unsigned int sorg = exaGetPixmapOffset(pSrcPixmap);
    unsigned int dorg = exaGetPixmapOffset(pDstPixmap);
    unsigned int sptch = exaGetPixmapPitch(pSrcPixmap);
    unsigned int dptch = exaGetPixmapPitch(pDstPixmap);

    if (planemask != -1)
	    return FALSE;

    pTrident->BltScanDirection = 0;
    if (dx < 0) pTrident->BltScanDirection |= XNEG;
    if (dy < 0) pTrident->BltScanDirection |= YNEG;

    ropcode = alu;

    MMIO_OUT32(pTrident->IOBase, 0x2154, (sptch << 18) | (sorg >> 4));
    MMIO_OUT32(pTrident->IOBase, 0x2150, (dptch << 18) | (dorg >> 4));

    return TRUE;
}

static void
XP4Copy(PixmapPtr pDstPixmap, int x1, int y1, int x2, int y2, int w, int h)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDstPixmap->drawable.pScreen);
    TRIDENTPtr pTrident = TRIDENTPTR(pScrn);
    int bpp;
    
    switch (pDstPixmap->drawable.bitsPerPixel) {
	case 8:
	    bpp = 0x40;
	    break;
	case 16:
	    bpp = 0x41;
	    break;
	case 32:
	    bpp = 0x42;
	    break;
    }

    if (pTrident->BltScanDirection & YNEG) {
        y1 = y1 + h - 1;
	y2 = y2 + h - 1;
    }
    if (pTrident->BltScanDirection & XNEG) {
	x1 = x1 + w - 1;
	x2 = x2 + w - 1;
    }
    MMIO_OUT32(pTrident->IOBase, 0x2128, pTrident->BltScanDirection | SCR2SCR);
    MMIO_OUT32(pTrident->IOBase, 0x2138, x2<<16 | y2);
    MMIO_OUT32(pTrident->IOBase, 0x213C, x1<<16 | y1);
    MMIO_OUT32(pTrident->IOBase, 0x2140, w<<16 | h);
    MMIO_OUT32(pTrident->IOBase, 0x2124, GetCopyROP(ropcode) << 24 | bpp << 8 | 1);
}

Bool
XP4ExaInit(ScreenPtr pScreen)
{
    ExaDriverPtr pExa;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    TRIDENTPtr pTrident = TRIDENTPTR(pScrn);

    if (pTrident->NoAccel)
	return FALSE;

    if (!(pExa = pTrident->EXADriverPtr = exaDriverAlloc())) {
        pTrident->NoAccel = TRUE;
        return FALSE;
    }

    pExa->exa_major = 2;
    pExa->exa_minor = 0;

    pExa->flags = EXA_OFFSCREEN_PIXMAPS;
    pExa->memoryBase = pTrident->FbBase;
    pExa->memorySize = pTrident->FbMapSize;
    pExa->offScreenBase = pScrn->displayWidth * pScrn->virtualY *
                               ((pScrn->bitsPerPixel + 7) / 8);

    pExa->pixmapOffsetAlign = 16;
    pExa->pixmapPitchAlign = 16;

    pExa->maxX = 4095;
    pExa->maxY = 4095;

    pExa->WaitMarker = XP4WaitMarker;

    pExa->PrepareSolid = XP4PrepareSolid;
    pExa->Solid = XP4Solid;
    pExa->DoneSolid = XP4Done;

    pExa->PrepareCopy = XP4PrepareCopy;
    pExa->Copy = XP4Copy;
    pExa->DoneCopy = XP4Done;

    return(exaDriverInit(pScreen, pExa));
}
