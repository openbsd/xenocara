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
/*
 * Reformatted with GNU indent (2.2.8), using the following options:
 *
 *    -bad -bap -c41 -cd0 -ncdb -ci6 -cli0 -cp0 -ncs -d0 -di3 -i3 -ip3 -l78
 *    -lp -npcs -psl -sob -ss -br -ce -sc -hnl
 *
 * This provides a good match with the original i810 code and preferred
 * XFree86 formatting conventions.
 *
 * When editing this driver, please follow the existing formatting, and edit
 * with <TAB> characters expanded at 8-column intervals.
 */

/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/i810/i830_dga.c,v 1.2 2002/11/05 02:01:18 dawes Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Pci.h"
#include "xf86PciInfo.h"
#include "xaa.h"
#include "xaalocal.h"
#include "i830.h"
#include "i810_reg.h"
#include "dgaproc.h"
#include "vgaHW.h"

static Bool I830_OpenFramebuffer(ScrnInfoPtr, char **, unsigned char **,
				 int *, int *, int *);
static void I830_CloseFramebuffer(ScrnInfoPtr pScrn);
static Bool I830_SetMode(ScrnInfoPtr, DGAModePtr);
static void I830_Sync(ScrnInfoPtr);
static int I830_GetViewport(ScrnInfoPtr);
static void I830_SetViewport(ScrnInfoPtr, int, int, int);
static void I830_FillRect(ScrnInfoPtr, int, int, int, int, unsigned long);
static void I830_BlitRect(ScrnInfoPtr, int, int, int, int, int, int);

#if 0
static void I830_BlitTransRect(ScrnInfoPtr, int, int, int, int, int, int,
			       unsigned long);
#endif

static
DGAFunctionRec I830DGAFuncs = {
   I830_OpenFramebuffer,
   I830_CloseFramebuffer,
   I830_SetMode,
   I830_SetViewport,
   I830_GetViewport,
   I830_Sync,
   I830_FillRect,
   I830_BlitRect,
#if 0
   I830_BlitTransRect
#else
   NULL
#endif
};

Bool
I830DGAInit(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   DGAModePtr modes = NULL, newmodes = NULL, currentMode;
   DisplayModePtr pMode, firstMode;
   int Bpp = pScrn->bitsPerPixel >> 3;
   int num = 0;

   MARKER();

   pMode = firstMode = pScrn->modes;

   while (pMode) {

	if(pI830->MergedFB) {
	   Bool nogood = FALSE;
	   /* Filter out all meta modes that would require driver-side panning */
	   switch(((I830ModePrivatePtr)pMode->Private)->merged.SecondPosition) {
	   case PosRightOf:
	   case PosLeftOf:
	      if( (((I830ModePrivatePtr)pMode->Private)->merged.First->VDisplay !=
		   ((I830ModePrivatePtr)pMode->Private)->merged.Second->VDisplay)	||
		  (((I830ModePrivatePtr)pMode->Private)->merged.First->VDisplay != pMode->VDisplay) )
		 nogood = TRUE;
	      break;
	   default:
	      if( (((I830ModePrivatePtr)pMode->Private)->merged.First->HDisplay !=
		   ((I830ModePrivatePtr)pMode->Private)->merged.Second->HDisplay)	||
		  (((I830ModePrivatePtr)pMode->Private)->merged.First->HDisplay != pMode->HDisplay) )
		 nogood = TRUE;
	   }
	   if(nogood) {
	      xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"DGA: MetaMode %dx%d not suitable for DGA, skipping\n",
			pMode->HDisplay, pMode->VDisplay);
	      goto mode_nogood;
	   }
	}

      newmodes = xrealloc(modes, (num + 1) * sizeof(DGAModeRec));

      if (!newmodes) {
	 xfree(modes);
	 return FALSE;
      }
      modes = newmodes;

      currentMode = modes + num;
      num++;

      currentMode->mode = pMode;
      currentMode->flags = DGA_CONCURRENT_ACCESS | DGA_PIXMAP_AVAILABLE;
      if (!pI830->noAccel)
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
      if (I830IsPrimary(pScrn)) {
         currentMode->address = pI830->FbBase + pI830->FrontBuffer.Start;
      } else {
         I830Ptr pI8301 = I830PTR(pI830->entityPrivate->pScrn_1);
         currentMode->address = pI830->FbBase + pI8301->FrontBuffer2.Start;
      }

      currentMode->bytesPerScanline = ((pI830->displayWidth * Bpp) + 3) & ~3L;
      if (I830IsPrimary(pScrn)) {
         currentMode->imageWidth = pI830->FbMemBox.x2;
         currentMode->imageHeight = pI830->FbMemBox.y2;
      } else {
         I830Ptr pI8301 = I830PTR(pI830->entityPrivate->pScrn_1);
         currentMode->imageWidth = pI8301->FbMemBox2.x2;
         currentMode->imageHeight = pI8301->FbMemBox2.y2;
      }
      currentMode->pixmapWidth = currentMode->imageWidth;
      currentMode->pixmapHeight = currentMode->imageHeight;
      currentMode->maxViewportX = currentMode->imageWidth -
	    currentMode->viewportWidth;
      /* this might need to get clamped to some maximum */
      currentMode->maxViewportY = currentMode->imageHeight -
	    currentMode->viewportHeight;

mode_nogood:
      pMode = pMode->next;
      if (pMode == firstMode)
	 break;
   }

   pI830->numDGAModes = num;
   pI830->DGAModes = modes;

   return DGAInit(pScreen, &I830DGAFuncs, modes, num);
}

static DisplayModePtr I830SavedDGAModes[MAXSCREENS];

static Bool
I830_SetMode(ScrnInfoPtr pScrn, DGAModePtr pMode)
{
   int index = pScrn->pScreen->myNum;
   I830Ptr pI830 = I830PTR(pScrn);

   MARKER();

   if (!pMode) {			/* restore the original mode */
      DPRINTF(PFX, "Restoring original mode (from DGA mode)\n");
      if (pI830->DGAactive) {
         I830_CloseFramebuffer(pScrn);
	 pScrn->currentMode = I830SavedDGAModes[index];
	 pScrn->SwitchMode(index, pScrn->currentMode, 0);
	 pScrn->AdjustFrame(index, 0, 0, 0);
	 pI830->DGAactive = FALSE;
      }
   } else {
      if (!pI830->DGAactive) {
	 DPRINTF(PFX, "Setting DGA mode\n");
	 I830SavedDGAModes[index] = pScrn->currentMode;
	 pI830->DGAactive = TRUE;
         if (I830IsPrimary(pScrn)) {
            pScrn->fbOffset = pI830->FrontBuffer.Start;
         }
         else {
            I830Ptr pI8301 = I830PTR(pI830->entityPrivate->pScrn_1);
            pScrn->fbOffset = pI8301->FrontBuffer2.Start;
         }
         pScrn->displayWidth = pI830->displayWidth;
         I830SelectBuffer(pScrn, I830_SELECT_FRONT);
      }

      pScrn->SwitchMode(index, pMode->mode, 0);
   }

   return TRUE;
}

static int
I830_GetViewport(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

   MARKER();

   return pI830->DGAViewportStatus;
}

static void
I830_SetViewport(ScrnInfoPtr pScrn, int x, int y, int flags)
{
   I830Ptr pI830 = I830PTR(pScrn);
   vgaHWPtr hwp = VGAHWPTR(pScrn);

   MARKER();

   pScrn->AdjustFrame(pScrn->pScreen->myNum, x, y, flags);

   /* wait for retrace */
   while ((hwp->readST01(hwp) & 0x08)) ;
   while (!(hwp->readST01(hwp) & 0x08)) ;

   pI830->DGAViewportStatus = 0;
}

static void
I830_FillRect(ScrnInfoPtr pScrn,
	      int x, int y, int w, int h, unsigned long color)
{
   I830Ptr pI830 = I830PTR(pScrn);

   MARKER();

   if (pI830->AccelInfoRec) {
      (*pI830->AccelInfoRec->SetupForSolidFill) (pScrn, color, GXcopy, ~0);
      (*pI830->AccelInfoRec->SubsequentSolidFillRect) (pScrn, x, y, w, h);
      SET_SYNC_FLAG(pI830->AccelInfoRec);
   }
}

static void
I830_Sync(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   int flags = MI_WRITE_DIRTY_STATE | MI_INVALIDATE_MAP_CACHE;

   MARKER();

   if (pI830->noAccel) 
      return;

   if (IS_I965G(pI830))
      flags = 0;

   BEGIN_LP_RING(2);
   OUT_RING(MI_FLUSH | flags);
   OUT_RING(MI_NOOP);		/* pad to quadword */
   ADVANCE_LP_RING();

   I830WaitLpRing(pScrn, pI830->LpRing->mem.Size - 8, 0);

   pI830->LpRing->space = pI830->LpRing->mem.Size - 8;
   pI830->nextColorExpandBuf = 0;
}

static void
I830_BlitRect(ScrnInfoPtr pScrn,
	      int srcx, int srcy, int w, int h, int dstx, int dsty)
{
   I830Ptr pI830 = I830PTR(pScrn);

   MARKER();

   if (pI830->AccelInfoRec) {
      int xdir = ((srcx < dstx) && (srcy == dsty)) ? -1 : 1;
      int ydir = (srcy < dsty) ? -1 : 1;

      (*pI830->AccelInfoRec->SetupForScreenToScreenCopy) (pScrn, xdir, ydir,
							  GXcopy, ~0, -1);
      (*pI830->AccelInfoRec->SubsequentScreenToScreenCopy) (pScrn, srcx, srcy,
							    dstx, dsty, w, h);
      SET_SYNC_FLAG(pI830->AccelInfoRec);
   }
}

#if 0
static void
I830_BlitTransRect(ScrnInfoPtr pScrn,
		   int srcx, int srcy,
		   int w, int h, int dstx, int dsty, unsigned long color)
{

   MARKER();

   /* this one should be separate since the XAA function would
    * prohibit usage of ~0 as the key */
}
#endif

static Bool
I830_OpenFramebuffer(ScrnInfoPtr pScrn,
		     char **name,
		     unsigned char **mem, int *size, int *offset, int *flags)
{
   I830Ptr pI830 = I830PTR(pScrn);

   MARKER();

   *name = NULL;			/* no special device */
   if (I830IsPrimary(pScrn)) {
      *size = pI830->FrontBuffer.Size;
      *mem = (unsigned char *)(pI830->LinearAddr + pI830->FrontBuffer.Start);
      pScrn->fbOffset = pI830->FrontBuffer.Start;
   }
   else {
      I830Ptr pI8301 = I830PTR(pI830->entityPrivate->pScrn_1);
      *size = pI8301->FrontBuffer2.Size;
      *mem = (unsigned char *)(pI8301->LinearAddr + pI8301->FrontBuffer2.Start);
      pScrn->fbOffset = pI8301->FrontBuffer2.Start;
   }
   pScrn->displayWidth = pI830->displayWidth;
   I830SelectBuffer(pScrn, I830_SELECT_FRONT);
   *offset = 0;
   *flags = DGA_NEED_ROOT;

   DPRINTF(PFX,
	   " mem == 0x%.8x (pI830->LinearAddr)\n"
	   "size == %lu (pI830->FbMapSize)\n", *mem, *size);

   return TRUE;
}

static void
I830_CloseFramebuffer(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   int i;
   /* Good pitches to allow tiling.  Don't care about pitches < 1024. */
   static const int pitches[] = {
/*
	 128 * 2,
	 128 * 4,
*/
	 128 * 8,
	 128 * 16,
	 128 * 32,
	 128 * 64,
	 0
   };

   if (I830IsPrimary(pScrn)) {
      if (pI830->rotation != RR_Rotate_0)
         pScrn->fbOffset = pI830->RotatedMem.Start;
      else
         pScrn->fbOffset = pI830->FrontBuffer.Start;
   } else {
      I830Ptr pI8301 = I830PTR(pI830->entityPrivate->pScrn_1);

      if (pI830->rotation != RR_Rotate_0)
         pScrn->fbOffset = pI8301->RotatedMem2.Start;
      else
         pScrn->fbOffset = pI8301->FrontBuffer2.Start;
   }
   I830SelectBuffer(pScrn, I830_SELECT_FRONT);

   switch (pI830->rotation) {
      case RR_Rotate_0:
         pScrn->displayWidth = pI830->displayWidth;
         break;
      case RR_Rotate_90:
         pScrn->displayWidth = pScrn->pScreen->width;
         break;
      case RR_Rotate_180:
         pScrn->displayWidth = pI830->displayWidth;
         break;
      case RR_Rotate_270:
         pScrn->displayWidth = pScrn->pScreen->width;
         break;
   }

   /* As DRI doesn't run on the secondary head, we know that disableTiling
    * is always TRUE.
    */
   if (I830IsPrimary(pScrn) && !pI830->disableTiling) {
#if 0
      int dWidth = pScrn->displayWidth; /* save current displayWidth */
#endif

      for (i = 0; pitches[i] != 0; i++) {
         if (pitches[i] >= pScrn->displayWidth) {
            pScrn->displayWidth = pitches[i];
            break;
         }
      }

      /*
       * If the displayWidth is a tilable pitch, test if there's enough
       * memory available to enable tiling.
       */
      if (pScrn->displayWidth == pitches[i]) {
  	/* TODO */
      }
   }

}
