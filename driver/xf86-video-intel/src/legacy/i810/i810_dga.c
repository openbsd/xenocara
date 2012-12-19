/*
 * Copyright 2000 by Alan Hourihane, Sychdyn, North Wales, UK.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Alan Hourihane not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Alan Hourihane makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * ALAN HOURIHANE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL ALAN HOURIHANE BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors:  Alan Hourihane, <alanh@fairlite.demon.co.uk>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Pci.h"
#include "i810.h"
#include "i810_reg.h"
#include "dgaproc.h"
#include "vgaHW.h"

static Bool I810_OpenFramebuffer(ScrnInfoPtr, char **, unsigned char **,
				 int *, int *, int *);
static Bool I810_SetMode(ScrnInfoPtr, DGAModePtr);
static int I810_GetViewport(ScrnInfoPtr);
static void I810_SetViewport(ScrnInfoPtr, int, int, int);

#ifdef HAVE_XAA_H
static void I810_Sync(ScrnInfoPtr);
static void I810_FillRect(ScrnInfoPtr, int, int, int, int, unsigned long);
static void I810_BlitRect(ScrnInfoPtr, int, int, int, int, int, int);
#endif

#if 0
static void I810_BlitTransRect(ScrnInfoPtr, int, int, int, int, int, int,
			       unsigned long);
#endif

static
DGAFunctionRec I810DGAFuncs = {
   I810_OpenFramebuffer,
   NULL,
   I810_SetMode,
   I810_SetViewport,
   I810_GetViewport,
#ifdef HAVE_XAA_H
   I810_Sync,
   I810_FillRect,
   I810_BlitRect,
#else
   NULL,
   NULL,
   NULL,
#endif
#if 0
   I810_BlitTransRect
#else
   NULL
#endif
};

Bool
I810DGAInit(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   I810Ptr pI810 = I810PTR(pScrn);
   DGAModePtr modes = NULL, newmodes = NULL, currentMode;
   DisplayModePtr pMode, firstMode;
   int Bpp = pScrn->bitsPerPixel >> 3;
   int num = 0;

   pMode = firstMode = pScrn->modes;

   while (pMode) {

      newmodes = realloc(modes, (num + 1) * sizeof(DGAModeRec));

      if (!newmodes) {
	 free(modes);
	 return FALSE;
      }
      modes = newmodes;

      currentMode = modes + num;
      num++;

      currentMode->mode = pMode;
      currentMode->flags = DGA_CONCURRENT_ACCESS | DGA_PIXMAP_AVAILABLE;
      if (!pI810->noAccel)
	 currentMode->flags |= DGA_FILL_RECT | DGA_BLIT_RECT;
      if (pMode->Flags & V_DBLSCAN)
	 currentMode->flags |= DGA_DOUBLESCAN;
      if (pMode->Flags & V_INTERLACE)
	 currentMode->flags |= DGA_INTERLACED;
      currentMode->byteOrder = pScrn->imageByteOrder;
      currentMode->depth = pScrn->depth;
      currentMode->bitsPerPixel = pScrn->bitsPerPixel;
      currentMode->red_mask = pScrn->mask.red;
      currentMode->green_mask = pScrn->mask.green;
      currentMode->blue_mask = pScrn->mask.blue;
      currentMode->visualClass = (Bpp == 1) ? PseudoColor : TrueColor;
      currentMode->viewportWidth = pMode->HDisplay;
      currentMode->viewportHeight = pMode->VDisplay;
      currentMode->xViewportStep = (Bpp == 3) ? 2 : 1;
      currentMode->yViewportStep = 1;
      currentMode->viewportFlags = DGA_FLIP_RETRACE;
      currentMode->offset = 0;
      currentMode->address = pI810->FbBase;

      currentMode->bytesPerScanline = ((pScrn->displayWidth * Bpp) + 3) & ~3L;
      currentMode->imageWidth = pI810->FbMemBox.x2;
      currentMode->imageHeight = pI810->FbMemBox.y2;
      currentMode->pixmapWidth = currentMode->imageWidth;
      currentMode->pixmapHeight = currentMode->imageHeight;
      currentMode->maxViewportX = currentMode->imageWidth -
	    currentMode->viewportWidth;
      /* this might need to get clamped to some maximum */
      currentMode->maxViewportY = currentMode->imageHeight -
	    currentMode->viewportHeight;

      pMode = pMode->next;
      if (pMode == firstMode)
	 break;
   }

   pI810->numDGAModes = num;
   pI810->DGAModes = modes;

   return DGAInit(pScreen, &I810DGAFuncs, modes, num);
}

static DisplayModePtr I810SavedDGAModes[MAXSCREENS];

static Bool
I810_SetMode(ScrnInfoPtr pScrn, DGAModePtr pMode)
{
   int index = pScrn->pScreen->myNum;
   I810Ptr pI810 = I810PTR(pScrn);

   if (!pMode) {			/* restore the original mode */
      if (pI810->DGAactive) {
	 pScrn->currentMode = I810SavedDGAModes[index];
	 pScrn->SwitchMode(SWITCH_MODE_ARGS(pScrn, pScrn->currentMode));
	 pScrn->AdjustFrame(ADJUST_FRAME_ARGS(pScrn, 0, 0));
	 pI810->DGAactive = FALSE;
      }
   } else {
      if (!pI810->DGAactive) {
	 I810SavedDGAModes[index] = pScrn->currentMode;
	 pI810->DGAactive = TRUE;
      }
      pScrn->SwitchMode(SWITCH_MODE_ARGS(pScrn, pMode->mode));
   }

   return TRUE;
}

static int
I810_GetViewport(ScrnInfoPtr pScrn)
{
   I810Ptr pI810 = I810PTR(pScrn);

   return pI810->DGAViewportStatus;
}

static void
I810_SetViewport(ScrnInfoPtr pScrn, int x, int y, int flags)
{
   I810Ptr pI810 = I810PTR(pScrn);
   vgaHWPtr hwp = VGAHWPTR(pScrn);

   pScrn->AdjustFrame(ADJUST_FRAME_ARGS(pScrn, x, y));

   /* wait for retrace */
   while ((hwp->readST01(hwp) & 0x08)) ;
   while (!(hwp->readST01(hwp) & 0x08)) ;

   pI810->DGAViewportStatus = 0;
}

#ifdef HAVE_XAA_H
static void
I810_FillRect(ScrnInfoPtr pScrn,
	      int x, int y, int w, int h, unsigned long color)
{
   I810Ptr pI810 = I810PTR(pScrn);

   if (pI810->AccelInfoRec) {
      (*pI810->AccelInfoRec->SetupForSolidFill) (pScrn, color, GXcopy, ~0);
      (*pI810->AccelInfoRec->SubsequentSolidFillRect) (pScrn, x, y, w, h);
      SET_SYNC_FLAG(pI810->AccelInfoRec);
   }
}

static void
I810_Sync(ScrnInfoPtr pScrn)
{
   I810Ptr pI810 = I810PTR(pScrn);

   if (pI810->AccelInfoRec) {
      (*pI810->AccelInfoRec->Sync) (pScrn);
   }
}

static void
I810_BlitRect(ScrnInfoPtr pScrn,
	      int srcx, int srcy, int w, int h, int dstx, int dsty)
{
   I810Ptr pI810 = I810PTR(pScrn);

   if (pI810->AccelInfoRec) {
      int xdir = ((srcx < dstx) && (srcy == dsty)) ? -1 : 1;
      int ydir = (srcy < dsty) ? -1 : 1;

      (*pI810->AccelInfoRec->SetupForScreenToScreenCopy) (pScrn, xdir, ydir,
							  GXcopy, ~0, -1);
      (*pI810->AccelInfoRec->SubsequentScreenToScreenCopy) (pScrn, srcx, srcy,
							    dstx, dsty, w, h);
      SET_SYNC_FLAG(pI810->AccelInfoRec);
   }
}
#endif

#if 0
static void
I810_BlitTransRect(ScrnInfoPtr pScrn,
		   int srcx, int srcy,
		   int w, int h, int dstx, int dsty, unsigned long color)
{

   /* this one should be separate since the XAA function would
    * prohibit usage of ~0 as the key */
}
#endif

static Bool
I810_OpenFramebuffer(ScrnInfoPtr pScrn,
		     char **name,
		     unsigned char **mem, int *size, int *offset, int *flags)
{
   I810Ptr pI810 = I810PTR(pScrn);

   *name = NULL;			/* no special device */
   *mem = (unsigned char *)pI810->LinearAddr;
   *size = pI810->FbMapSize;
   *offset = 0;
   *flags = DGA_NEED_ROOT;

   return TRUE;
}
