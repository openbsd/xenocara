/*
 * Copyright (c) 2013 Matthieu Herrb 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE * SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "compiler.h"

#include "xf86Pci.h"
#include "vgaHW.h"

#include "cir.h"
#define _ALP_PRIVATE_
#include "alp.h"

#include "exa.h"

#define WAIT	outb(pCir->PIOReg, 0x31); \
		while(inb(pCir->PIOReg + 1) & pCir->chip.alp->waitMsk){};
#define WAIT_1	outb(pCir->PIOReg, 0x31); \
		while(inb(pCir->PIOReg + 1) & 0x1){};
#define SetupForRop(rop) outw(pCir->PIOReg, translated_rop[rop])

static Bool AlpPrepareSolid(PixmapPtr, int, Pixel, Pixel);
static void AlpSolid(PixmapPtr, int, int, int, int);
static Bool AlpPrepareCopy(PixmapPtr, PixmapPtr, int, int, int, Pixel);
static void AlpCopy(PixmapPtr, int, int, int, int, int, int);
static void AlpDone(PixmapPtr);
static void AlpSync(ScreenPtr, int);

static const CARD16 translated_rop[] =
{
  /* GXclear */        0x0032U,
  /* GXand   */        0x0532U,
  /* GXandreverse */   0x0932U,
  /* GXcopy */         0x0D32U,
  /* GXandinversted */ 0x5032U,
  /* GXnoop */         0x0632U,
  /* GXxor */          0x5932U,
  /* GXor */           0x6D32U,
  /* GXnor */          0x9032U,
  /* GXequiv */        0x9532U,
  /* GXinvert */       0x0B32U,
  /* GXorReverse */    0xAD32U,
  /* GXcopyInverted */ 0xD032U,
  /* GXorInverted */   0xD632U,
  /* GXnand */         0xDA32U,
  /* GXset */          0x0E32U
};


static Bool
AlpPrepareSolid(PixmapPtr pPixmap, int alu, Pixel planemask, Pixel fg)
{
    ScreenPtr pScreen = pPixmap->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    CirPtr pCir = CIRPTR(pScrn);
    AlpPtr pAlp = ALPPTR(pCir);
    int pitch = pCir->pitch;
    
#ifdef ALP_DEBUG
    ErrorF("AlpSetupForSolidFill color=%x alu=%x planemask=%x\n",
	   fg, alu, planemask);
#endif
    WAIT;
    
    SetupForRop(alu);
    
    switch (pCir -> Chipset) {
      case PCI_CHIP_GD7548:
	/* The GD7548 does not (apparently) support solid filling
	   directly, it always need an actual source.
	   We therefore use it as a pattern fill with a solid
	   pattern */
      {
	  int source = pAlp->monoPattern8x8;
	  /* source = 8x8 solid mono pattern */
	  outw(pCir->PIOReg, ((source << 8) & 0xff00) | 0x2C);
	  outw(pCir->PIOReg, ((source) & 0xff00) | 0x2D);
	  outw(pCir->PIOReg, ((source >> 8) & 0x3f00) | 0x2E);
	  /* memset() may not be the fastest */
	  memset((char *)pCir->FbBase + pAlp->monoPattern8x8, 0xFF, 8);
	  write_mem_barrier();
	  break;
      }
      default:
	/* GR33 = 0x04 => does not exist on GD7548 */
	outw(pCir->PIOReg, 0x0433);
    }
    
    /* GR30 = color expansion, pattern copy */
    /* Choses 8bpp / 16bpp color expansion */
    outw(pCir->PIOReg, 0xC030 |((pScrn->bitsPerPixel - 8) << 9));
    
    outw(pCir->PIOReg, ((fg << 8) & 0xff00) | 0x01);
    outw(pCir->PIOReg, ((fg) & 0xff00) | 0x11);
    outw(pCir->PIOReg, ((fg >> 8) & 0xff00) | 0x13);
    outw(pCir->PIOReg, 0x15);
    
    /* Set dest pitch */
    outw(pCir->PIOReg, ((pitch << 8) & 0xff00) | 0x24);
    outw(pCir->PIOReg, ((pitch) & 0x1f00) | 0x25);
    
    return TRUE;
}

static void
AlpSolid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2)
{
    ScreenPtr pScreen = pPixmap->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    CirPtr pCir = CIRPTR(pScrn);
    int dest;
    int hh, ww;
    int pitch = pCir->pitch;
    
    ww = ((x2 - x1) * pScrn->bitsPerPixel / 8) - 1;
    hh = (y2 - y1) - 1;
    dest = y1 * pitch + x1 * pScrn->bitsPerPixel / 8;
    
    WAIT;
    
    /* Width */
    outw(pCir->PIOReg, ((ww << 8) & 0xff00) | 0x20);
    outw(pCir->PIOReg, ((ww) & 0x1f00) | 0x21);
    /* Height */
    outw(pCir->PIOReg, ((hh << 8) & 0xff00) | 0x22);
    outw(pCir->PIOReg, ((hh) & 0x0700) | 0x23);
    
    /* dest */
    outw(pCir->PIOReg, ((dest << 8) & 0xff00) | 0x28);
    outw(pCir->PIOReg, ((dest) & 0xff00) | 0x29);
    outw(pCir->PIOReg, ((dest >> 8) & 0x3f00) | 0x2A);
    if (!pCir->chip.alp->autoStart)
	outw(pCir->PIOReg, 0x0231);
    
#ifdef ALP_DEBUG
    ErrorF("AlpSubsequentSolidFillRect x=%d y=%d w=%d h=%d\n",
	   x1, y1, x2 - x1, y2 - y1);
#endif
}

static Bool 
AlpPrepareCopy(PixmapPtr pSrcPixmap, PixmapPtr pDstPixmap,
	       int xdir, int ydir, int alu, Pixel planemask)
{
    ScreenPtr pScreen = pSrcPixmap->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    CirPtr pCir = CIRPTR(pScrn);
    int pitch = pCir->pitch;

#ifdef ALP_DEBUG
    ErrorF("AlpPrepareCopy xdir=%d ydir=%d alu=%x planemask=%x\n",
	   xdir, ydir, alu, planemask);
#endif
    WAIT;
    SetupForRop(alu);
    /* Set dest pitch */
    outw(pCir->PIOReg, ((pitch << 8) & 0xff00) | 0x24);
    outw(pCir->PIOReg, ((pitch) & 0x1f00) | 0x25);
    /* Set source pitch */
    outw(pCir->PIOReg, ((pitch << 8) & 0xff00) | 0x26);
    outw(pCir->PIOReg, ((pitch) & 0x1f00) | 0x27);
    return TRUE;
}

static void
AlpCopy(PixmapPtr pDstPixmap, int srcX, int srcY, int dstX, int dstY, 
	int width, int height)
{
    ScreenPtr pScreen = pDstPixmap->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    CirPtr pCir = CIRPTR(pScrn);
    int source, dest;
    int  hh, ww;
    int decrement = 0;
    int pitch = pCir->pitch;
    
    ww = (width * pScrn->bitsPerPixel / 8) - 1;
    hh = height - 1;
    dest = dstY * pitch + dstX * pScrn->bitsPerPixel / 8;
    source = srcY * pitch + srcX * pScrn->bitsPerPixel / 8;
    if (dest > source) {
	decrement = 1 << 8;
	dest += hh * pitch + ww;
	source += hh * pitch + ww;
    }
    
    WAIT;
    
    outw(pCir->PIOReg, decrement | 0x30);
    
    /* Width */
    outw(pCir->PIOReg, ((ww << 8) & 0xff00) | 0x20);
    outw(pCir->PIOReg, ((ww) & 0x1f00) | 0x21);
    /* Height */
    outw(pCir->PIOReg, ((hh << 8) & 0xff00) | 0x22);
    outw(pCir->PIOReg, ((hh) & 0x0700) | 0x23);
    
    
    /* source */
    outw(pCir->PIOReg, ((source << 8) & 0xff00) | 0x2C);
    outw(pCir->PIOReg, ((source) & 0xff00) | 0x2D);
    outw(pCir->PIOReg, ((source >> 8) & 0x3f00)| 0x2E);
    
    /* dest */
    outw(pCir->PIOReg, ((dest  << 8) & 0xff00) | 0x28);
    outw(pCir->PIOReg, ((dest) & 0xff00) | 0x29);
    outw(pCir->PIOReg, ((dest >> 8) & 0x3f00) | 0x2A);
    if (!pCir->chip.alp->autoStart)
	outw(pCir->PIOReg, 0x0231);
    
#ifdef ALP_DEBUG
    ErrorF("AlpCopy x1=%d y1=%d x2=%d y2=%d w=%d h=%d\n",
	   srcX, srcY, dstX, dstY, width, height);
    ErrorF("AlpSCopy s=%d d=%d ww=%d hh=%d\n",
	   source, dest, ww, hh);
#endif
}

static void
AlpDone(PixmapPtr pPixmap)
{
    ScreenPtr pScreen = pPixmap->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    CirPtr pCir = CIRPTR(pScrn);
#ifdef ALP_DEBUG
    ErrorF("AlpDone\n");
#endif
}

static void
AlpSync(ScreenPtr pScreen, int marker)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    CirPtr pCir = CIRPTR(pScrn);

#ifdef ALP_DEBUG
    ErrorF("AlpSync\n");
#endif
    WAIT_1;
    return;
}


static void
AlpAccelEngineInit(ScrnInfoPtr pScrn)
{
    CirPtr pCir = CIRPTR(pScrn);

    outw(pCir->PIOReg, 0x200E); /* enable writes to gr33 */
    /* Setup things for autostart */
    if (pCir->properties & ACCEL_AUTOSTART) {
        outw(pCir->PIOReg, 0x8031); /* enable autostart */
	pCir->chip.alp->waitMsk = 0x10;
	pCir->chip.alp->autoStart = TRUE;
    } else {
        pCir->chip.alp->waitMsk = 0x1;
	pCir->chip.alp->autoStart = FALSE;
    }
}

Bool
AlpEXAInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    CirPtr pCir = CIRPTR(pScrn);

    AlpAccelEngineInit(pScrn);

    pCir->InitAccel =  AlpAccelEngineInit;

    pCir->ExaDriver->exa_major = EXA_VERSION_MAJOR;
    pCir->ExaDriver->exa_minor = EXA_VERSION_MINOR;

    pCir->ExaDriver->memoryBase = (char *)pCir->FbBase + pScrn->fbOffset;
    pCir->ExaDriver->flags = EXA_OFFSCREEN_PIXMAPS | EXA_OFFSCREEN_ALIGN_POT;

#if EXA_VERSION_MAJOR > 2 || (EXA_VERSION_MAJOR == 2 && EXA_VERSION_MINOR >= 3)
    pCir->ExaDriver->maxPitchBytes = 16320;
#endif
    /* Pitch alignment is in sets of 8 pixels, and we need to cover 32bpp, so it's 32 bytes */
    pCir->ExaDriver->pixmapPitchAlign = 32;
    pCir->ExaDriver->pixmapOffsetAlign = 32;
    pCir->ExaDriver->maxX = 2048;
    pCir->ExaDriver->maxY = 2048;
    
    pCir->ExaDriver->PrepareSolid = AlpPrepareSolid;
    pCir->ExaDriver->Solid = AlpSolid;
    pCir->ExaDriver->DoneSolid =AlpDone;
    
    pCir->ExaDriver->PrepareCopy = AlpPrepareCopy;
    pCir->ExaDriver->Copy = AlpCopy;
    pCir->ExaDriver->DoneCopy = AlpDone;

    pCir->ExaDriver->WaitMarker = AlpSync;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Initializing EXA driver...\n");
    if (!exaDriverInit(pScreen, pCir->ExaDriver)) {
	free(pCir->ExaDriver);
	return FALSE;
    }
    return TRUE;
}

    
