/* $XFree86$ */
/* $XdotOrg$ */
/*
 * SiS DGA handling
 *
 * Copyright (C) 2000 by Alan Hourihane, Sychdyn, North Wales, UK.
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria
 *
 * Portions from radeon_dga.c which is
 *          Copyright 2000 ATI Technologies Inc., Markham, Ontario, and
 *                         VA Linux Systems Inc., Fremont, California.
 *
 * Licensed under the following terms:
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the providers not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The providers make no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THE PROVIDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE PROVIDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors:  Alan Hourihane, <alanh@fairlite.demon.co.uk>
 *           Thomas Winischhofer <thomas@winischhofer.net>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sis.h"
#include "dgaproc.h"

#include "sis_regs.h"

#ifndef NEW_DGAOPENFRAMEBUFFER
static Bool SIS_OpenFramebuffer(ScrnInfoPtr, char **, UChar **,
			int *, int *, int *);
#else
static Bool SIS_OpenFramebuffer(ScrnInfoPtr, char **, unsigned int *,
			unsigned int *, unsigned int *, unsigned int *);
#endif
static Bool SIS_SetMode(ScrnInfoPtr, DGAModePtr);
static void SIS_Sync(ScrnInfoPtr);
static int  SIS_GetViewport(ScrnInfoPtr);
static void SIS_SetViewport(ScrnInfoPtr, int, int, int);
static void SIS_FillRect(ScrnInfoPtr, int, int, int, int, unsigned long);
static void SIS_BlitRect(ScrnInfoPtr, int, int, int, int, int, int);
static void SIS_BlitTransRect(ScrnInfoPtr, int, int, int, int, int, int, unsigned long);

static
DGAFunctionRec SISDGAFuncs = {
   SIS_OpenFramebuffer,
   NULL,
   SIS_SetMode,
   SIS_SetViewport,
   SIS_GetViewport,
   SIS_Sync,
   SIS_FillRect,
   SIS_BlitRect,
   NULL
};

static
DGAFunctionRec SISDGAFuncs3xx = {
   SIS_OpenFramebuffer,
   NULL,
   SIS_SetMode,
   SIS_SetViewport,
   SIS_GetViewport,
   SIS_Sync,
   SIS_FillRect,
   SIS_BlitRect,
   SIS_BlitTransRect
};

static DGAModePtr
SISSetupDGAMode(
   ScrnInfoPtr pScrn,
   DGAModePtr modes,
   int *num,
   int bitsPerPixel,
   int depth,
   Bool pixmap,
   int secondPitch,
   ULong red,
   ULong green,
   ULong blue,
   short visualClass
){
   SISPtr pSiS = SISPTR(pScrn);
   DGAModePtr newmodes = NULL, currentMode;
   DisplayModePtr pMode, firstMode;
   int otherPitch, Bpp = bitsPerPixel >> 3;
   Bool oneMore;

   pMode = firstMode = pScrn->modes;

   while(pMode) {

#ifdef SISMERGED
	if(pSiS->MergedFB) {
	   Bool nogood = FALSE;
	   /* Filter out all meta modes that would require driver-side panning */
	   switch(((SiSMergedDisplayModePtr)pMode->Private)->CRT2Position) {
	   case sisClone:
	      if( (((SiSMergedDisplayModePtr)pMode->Private)->CRT1->HDisplay !=
		   ((SiSMergedDisplayModePtr)pMode->Private)->CRT2->HDisplay)	||
		  (((SiSMergedDisplayModePtr)pMode->Private)->CRT1->VDisplay !=
		   ((SiSMergedDisplayModePtr)pMode->Private)->CRT2->VDisplay)	||
		  (((SiSMergedDisplayModePtr)pMode->Private)->CRT1->HDisplay !=
		   pMode->HDisplay)						||
		  (((SiSMergedDisplayModePtr)pMode->Private)->CRT1->VDisplay !=
		   pMode->VDisplay) )
		 nogood = TRUE;
	      break;
	   case sisRightOf:
	   case sisLeftOf:
	      if( (((SiSMergedDisplayModePtr)pMode->Private)->CRT1->VDisplay !=
		   ((SiSMergedDisplayModePtr)pMode->Private)->CRT2->VDisplay)	||
		  (((SiSMergedDisplayModePtr)pMode->Private)->CRT1->VDisplay != pMode->VDisplay) )
		 nogood = TRUE;
	      break;
	   default:
	      if( (((SiSMergedDisplayModePtr)pMode->Private)->CRT1->HDisplay !=
		   ((SiSMergedDisplayModePtr)pMode->Private)->CRT2->HDisplay)	||
		  (((SiSMergedDisplayModePtr)pMode->Private)->CRT1->HDisplay != pMode->HDisplay) )
		 nogood = TRUE;
	   }
	   if(nogood) {
	      if(depth == 16) { /* Print this only the first time */
		 xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"DGA: MetaMode %dx%d not suitable for DGA, skipping\n",
			pMode->HDisplay, pMode->VDisplay);
	      }
	      goto mode_nogood;
	   }
	}
#endif

	otherPitch = secondPitch ? secondPitch : pMode->HDisplay;

	if(pMode->HDisplay != otherPitch) {

	    newmodes = xrealloc(modes, (*num + 2) * sizeof(DGAModeRec));
	    oneMore  = TRUE;

	} else {

	    newmodes = xrealloc(modes, (*num + 1) * sizeof(DGAModeRec));
	    oneMore  = FALSE;

	}

	if(!newmodes) {
	    xfree(modes);
	    return NULL;
	}
	modes = newmodes;

SECOND_PASS:

	currentMode = modes + *num;
	(*num)++;

	currentMode->mode           = pMode;
	currentMode->flags          = DGA_CONCURRENT_ACCESS;
	if(pixmap)
	    currentMode->flags     |= DGA_PIXMAP_AVAILABLE;
	if(!pSiS->NoAccel) {
	    currentMode->flags     |= DGA_FILL_RECT | DGA_BLIT_RECT;
	    if((pSiS->VGAEngine == SIS_300_VGA) ||
	       (pSiS->VGAEngine == SIS_315_VGA) ||
	       (pSiS->VGAEngine == SIS_530_VGA)) {
               currentMode->flags  |= DGA_BLIT_RECT_TRANS;
            }
	}
	if(pMode->Flags & V_DBLSCAN)
	    currentMode->flags     |= DGA_DOUBLESCAN;
	if(pMode->Flags & V_INTERLACE)
	    currentMode->flags     |= DGA_INTERLACED;
	currentMode->byteOrder      = pScrn->imageByteOrder;
	currentMode->depth          = depth;
	currentMode->bitsPerPixel   = bitsPerPixel;
	currentMode->red_mask       = red;
	currentMode->green_mask     = green;
	currentMode->blue_mask      = blue;
	currentMode->visualClass    = visualClass;
	currentMode->viewportWidth  = pMode->HDisplay;
	currentMode->viewportHeight = pMode->VDisplay;
	currentMode->xViewportStep  = 1;
	currentMode->yViewportStep  = 1;
	currentMode->viewportFlags  = DGA_FLIP_RETRACE;
	currentMode->offset         = 0;
	currentMode->address        = pSiS->FbBase;

	if(oneMore) {

	    /* first one is narrow width */
	    currentMode->bytesPerScanline = (((pMode->HDisplay * Bpp) + 3) & ~3L);
	    currentMode->imageWidth   = pMode->HDisplay;
	    currentMode->imageHeight  = pMode->VDisplay;
	    currentMode->pixmapWidth  = currentMode->imageWidth;
	    currentMode->pixmapHeight = currentMode->imageHeight;
	    currentMode->maxViewportX = currentMode->imageWidth -
					currentMode->viewportWidth;
	    /* this might need to get clamped to some maximum */
	    currentMode->maxViewportY = (currentMode->imageHeight -
					 currentMode->viewportHeight);
	    oneMore = FALSE;
	    goto SECOND_PASS;

	} else {

	    currentMode->bytesPerScanline = ((otherPitch * Bpp) + 3) & ~3L;
	    currentMode->imageWidth       = otherPitch;
	    currentMode->imageHeight      = pMode->VDisplay;
	    currentMode->pixmapWidth      = currentMode->imageWidth;
	    currentMode->pixmapHeight     = currentMode->imageHeight;
	    currentMode->maxViewportX     = (currentMode->imageWidth -
					     currentMode->viewportWidth);
            /* this might need to get clamped to some maximum */
	    currentMode->maxViewportY     = (currentMode->imageHeight -
					     currentMode->viewportHeight);
	}

#ifdef SISMERGED
mode_nogood:
#endif

	pMode = pMode->next;
	if(pMode == firstMode)
	   break;
    }

    return modes;
}

Bool
SISDGAInit(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   SISPtr pSiS = SISPTR(pScrn);
   DGAModePtr modes = NULL;
   int num = 0;

   /* 8 */
   /* We don't support 8bpp modes in dual head or MergedFB mode,
    * so don't offer them to DGA either.
    */
#ifdef SISDUALHEAD
   if(!pSiS->DualHeadMode) {
#endif
#ifdef SISMERGED
      if(!(pSiS->MergedFB)) {
#endif
         modes = SISSetupDGAMode(pScrn, modes, &num, 8, 8,
				 (pScrn->bitsPerPixel == 8),
				 ((pScrn->bitsPerPixel != 8)
				     ? 0 : pScrn->displayWidth),
				 0, 0, 0, PseudoColor);
#ifdef SISMERGED
      }
#endif
#ifdef SISDUALHEAD
   }
#endif

   /* 16 */
   modes = SISSetupDGAMode(pScrn, modes, &num, 16, 16,
			   (pScrn->bitsPerPixel == 16),
			   ((pScrn->depth != 16)
				? 0 : pScrn->displayWidth),
			   0xf800, 0x07e0, 0x001f, TrueColor);

   if((pSiS->VGAEngine == SIS_530_VGA) || (pSiS->VGAEngine == SIS_OLD_VGA)) {
      /* 24 */
      modes = SISSetupDGAMode(pScrn, modes, &num, 24, 24,
			      (pScrn->bitsPerPixel == 24),
			      ((pScrn->bitsPerPixel != 24)
				 ? 0 : pScrn->displayWidth),
			      0xff0000, 0x00ff00, 0x0000ff, TrueColor);
   }

   if(pSiS->VGAEngine != SIS_OLD_VGA) {
      /* 32 */
      modes = SISSetupDGAMode(pScrn, modes, &num, 32, 24,
			      (pScrn->bitsPerPixel == 32),
			      ((pScrn->bitsPerPixel != 32)
				  ? 0 : pScrn->displayWidth),
			      0xff0000, 0x00ff00, 0x0000ff, TrueColor);
   }

   pSiS->numDGAModes = num;
   pSiS->DGAModes = modes;

   if(num) {
      if((pSiS->VGAEngine == SIS_300_VGA) ||
         (pSiS->VGAEngine == SIS_315_VGA) ||
         (pSiS->VGAEngine == SIS_530_VGA)) {
         return DGAInit(pScreen, &SISDGAFuncs3xx, modes, num);
      } else {
         return DGAInit(pScreen, &SISDGAFuncs, modes, num);
      }
   } else {
      xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		"No DGA-suitable modes found, disabling DGA\n");
      return TRUE;
   }
}

static Bool
SIS_OpenFramebuffer(
   ScrnInfoPtr pScrn,
   char **name,
#ifndef NEW_DGAOPENFRAMEBUFFER
   UChar **mem,
   int *size,
   int *offset,
   int *flags
#else
   unsigned int *mem,
   unsigned int *size,
   unsigned int *offset,
   unsigned int *flags
#endif
){
    SISPtr pSiS = SISPTR(pScrn);

    *name = NULL;       /* no special device */
#ifndef NEW_DGAOPENFRAMEBUFFER
    *mem = (UChar *)pSiS->FbAddress;
#else
    *mem = pSiS->FbAddress;
#endif
    *size = pSiS->maxxfbmem;
    *offset = 0;
#ifndef NEW_DGAOPENFRAMEBUFFER
    *flags = DGA_NEED_ROOT;
#else
    *flags = 0;
#endif

    return TRUE;
}

static Bool
SIS_SetMode(
   ScrnInfoPtr pScrn,
   DGAModePtr pMode
){
   static SISFBLayout BackupLayouts[MAXSCREENS];
   int index = pScrn->pScreen->myNum;
   SISPtr pSiS = SISPTR(pScrn);

    if(!pMode) { /* restore the original mode */

	if(pSiS->DGAactive) {
	   /* put the ScreenParameters back */
	   memcpy(&pSiS->CurrentLayout, &BackupLayouts[index], sizeof(SISFBLayout));
	}

	pScrn->currentMode = pSiS->CurrentLayout.mode;
	pSiS->DGAactive = FALSE;

	(*pScrn->SwitchMode)(index, pScrn->currentMode, 0);
	(*pScrn->AdjustFrame)(index, pScrn->frameX0, pScrn->frameY0, 0);

    } else {	/* set new mode */

        if(!pSiS->DGAactive) {
	    /* save the old parameters */
	    memcpy(&BackupLayouts[index], &pSiS->CurrentLayout, sizeof(SISFBLayout));
	    pSiS->DGAactive = TRUE;
	}

	pSiS->CurrentLayout.bitsPerPixel  = pMode->bitsPerPixel;
	pSiS->CurrentLayout.depth         = pMode->depth;
	pSiS->CurrentLayout.displayWidth  = pMode->bytesPerScanline / (pMode->bitsPerPixel >> 3);
	pSiS->CurrentLayout.displayHeight = pMode->imageHeight;

	(*pScrn->SwitchMode)(index, pMode->mode, 0);
	/* Adjust viewport to 0/0 after mode switch */
	/* This fixes the vmware-in-dualhead problems */
	(*pScrn->AdjustFrame)(index, 0, 0, 0);
	pSiS->CurrentLayout.DGAViewportX = pSiS->CurrentLayout.DGAViewportY = 0;
    }

    return TRUE;
}

static int
SIS_GetViewport(
  ScrnInfoPtr pScrn
){
    SISPtr pSiS = SISPTR(pScrn);

    return pSiS->DGAViewportStatus;
}

static void
SIS_SetViewport(
   ScrnInfoPtr pScrn,
   int x, int y,
   int flags
){
   SISPtr pSiS = SISPTR(pScrn);

   (*pScrn->AdjustFrame)(pScrn->pScreen->myNum, x, y, flags);
   pSiS->DGAViewportStatus = 0;  /* There are never pending Adjusts */
   pSiS->CurrentLayout.DGAViewportX = x;
   pSiS->CurrentLayout.DGAViewportY = y;
}

static void
SIS_Sync(
   ScrnInfoPtr pScrn
){
    SISPtr pSiS = SISPTR(pScrn);

    (*pSiS->SyncAccel)(pScrn);
}

static void
SIS_FillRect(
   ScrnInfoPtr pScrn,
   int x, int y, int w, int h,
   unsigned long color
){
    SISPtr pSiS = SISPTR(pScrn);

    if(pSiS->FillRect) {
       (*pSiS->FillRect)(pScrn, x, y, w, h, (int)color);
#ifdef SIS_USE_XAA
       if(!pSiS->useEXA && pSiS->AccelInfoPtr) {
          SET_SYNC_FLAG(pSiS->AccelInfoPtr);
       }
#endif
    }
}

static void
SIS_BlitRect(
   ScrnInfoPtr pScrn,
   int srcx, int srcy,
   int w, int h,
   int dstx, int dsty
){
    SISPtr pSiS = SISPTR(pScrn);

    if(pSiS->BlitRect) {
       (*pSiS->BlitRect)(pScrn, srcx, srcy, dstx, dsty, w, h, -1);
#ifdef SIS_USE_XAA
       if(!pSiS->useEXA && pSiS->AccelInfoPtr) {
          SET_SYNC_FLAG(pSiS->AccelInfoPtr);
       }
#endif
    }
}

static void
SIS_BlitTransRect(
   ScrnInfoPtr pScrn,
   int srcx, int srcy,
   int w, int h,
   int dstx, int dsty,
   ULong color
){
    SISPtr pSiS = SISPTR(pScrn);

    if(pSiS->BlitRect) {
       (*pSiS->BlitRect)(pScrn, srcx, srcy, dstx, dsty, w, h, (int)color);
#ifdef SIS_USE_XAA
       if(!pSiS->useEXA && pSiS->AccelInfoPtr) {
          SET_SYNC_FLAG(pSiS->AccelInfoPtr);
       }
#endif
    }
}



