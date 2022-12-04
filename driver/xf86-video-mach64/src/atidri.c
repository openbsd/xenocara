/* -*- mode: c; c-basic-offset: 3 -*- */
/*
 * Copyright 2000 Gareth Hughes
 * All Rights Reserved.
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * GARETH HUGHES BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
/*
 * Authors:
 *   Gareth Hughes <gareth@valinux.com>
 *   Leif Delgass <ldelgass@retinalburn.net>
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>

/* Driver data structures */
#include "ati.h"
#include "atibus.h"
#include "atidri.h"
#include "atiregs.h"
#include "atistruct.h"

#include "atimach64io.h"
#include "atimach64version.h"
#include "mach64_dri.h"
#include "mach64_common.h"
#include "mach64_sarea.h"

/* X and server generic header files */
#include "xf86.h"
#include "windowstr.h"

/* DRI/DRM definitions */
#define _XF86DRI_SERVER_
#include "sarea.h"

static char ATIKernelDriverName[] = "mach64";
static char ATIClientDriverName[] = "mach64";

/* Create the ATI-specific context information */
static Bool ATICreateContext( ScreenPtr pScreen, VisualPtr visual,
			      drm_context_t hwContext, void *pVisualConfigPriv,
			      DRIContextType contextStore )
{
   /* Nothing yet */
   return TRUE;
}

/* Destroy the ATI-specific context information */
static void ATIDestroyContext( ScreenPtr pScreen, drm_context_t hwContext,
			       DRIContextType contextStore )
{
   /* Nothing yet */
}

/* Called when the X server is woken up to allow the last client's
 * context to be saved and the X server's context to be loaded.
 * The client detects when it's context is not currently loaded and 
 * then loads it itself.  The X server's context is loaded in the
 * XAA Sync callback if NeedDRISync is set.
 */
static void ATIEnterServer( ScreenPtr pScreen )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);

   if ( pATI->directRenderingEnabled ) { 
      ATIDRIMarkSyncInt(pScreenInfo);
      ATIDRIMarkSyncExt(pScreenInfo);
   }
}

/* Called when the X server goes to sleep to allow the X server's
 * context to be saved and the last client's context to be loaded.
 * The client detects when it's context is not currently loaded and 
 * then loads it itself.  The X server keeps track of it's own state.
 */
static void ATILeaveServer( ScreenPtr pScreen )
{
   /* Nothing yet */
}

/* Contexts can be swapped by the X server if necessary.  This callback
 * is currently only used to perform any functions necessary when
 * entering or leaving the X server, and in the future might not be
 * necessary.
 */
static void ATIDRISwapContext( ScreenPtr pScreen,
			       DRISyncType syncType,
			       DRIContextType oldContextType,
			       void *oldContext,
			       DRIContextType newContextType,
			       void *newContext )
{
   if ( ( syncType == DRI_3D_SYNC ) && ( oldContextType == DRI_2D_CONTEXT ) &&
	( newContextType == DRI_2D_CONTEXT ) ) {
      /* Entering from Wakeup */
      ATIEnterServer( pScreen );
   }
   if ( ( syncType == DRI_2D_SYNC ) && ( oldContextType == DRI_NO_CONTEXT ) &&
	( newContextType == DRI_2D_CONTEXT ) ) {
      /* Exiting from Block Handler */
      ATILeaveServer( pScreen );
   }
}

#ifdef USE_XAA
static void ATIDRITransitionTo2d(ScreenPtr pScreen)
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);

   if (pATI->backArea) {
      xf86FreeOffscreenArea(pATI->backArea);
      pATI->backArea = NULL;
   }
   if (pATI->depthTexArea) {
      xf86FreeOffscreenArea(pATI->depthTexArea);
      pATI->depthTexArea = NULL;
   }
   pATI->have3DWindows = FALSE;
}

static void ATIDRITransitionTo3d(ScreenPtr pScreen)
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   FBAreaPtr fbArea;
   int width, height;

   xf86PurgeUnlockedOffscreenAreas(pScreen);

   xf86QueryLargestOffscreenArea(pScreen, &width, &height, 0, 0, 0);

   xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
	      "Largest offscreen area available: %d x %d\n",
	      width, height);

   fbArea = xf86AllocateOffscreenArea(pScreen, pScreenInfo->displayWidth,
				      height - pATI->depthTexLines - 
				      pATI->backLines,
				      pScreenInfo->displayWidth, NULL, NULL, NULL);

   if (!fbArea)
      xf86DrvMsg(pScreen->myNum, X_ERROR, "Unable to reserve placeholder "
		 "offscreen area, you might experience screen corruption\n");

   if (!pATI->backArea) {
      pATI->backArea = 
	 xf86AllocateOffscreenArea(pScreen, pScreenInfo->displayWidth,
				   pATI->backLines,
				   pScreenInfo->displayWidth,
				   NULL, NULL, NULL);
   }
   if (!pATI->backArea)
      xf86DrvMsg(pScreen->myNum, X_ERROR, "Unable to reserve offscreen area "
		 "for back buffer, you might experience screen corruption\n");

   if (!pATI->depthTexArea) {
      pATI->depthTexArea = 
	 xf86AllocateOffscreenArea(pScreen, pScreenInfo->displayWidth,
				   pATI->depthTexLines,
				   pScreenInfo->displayWidth,
				   NULL, NULL, NULL);
   }
   if (!pATI->depthTexArea)
      xf86DrvMsg(pScreen->myNum, X_ERROR, "Unable to reserve offscreen area "
		 "for depth buffer and textures, you might experience screen corruption\n");

   if (fbArea)
      xf86FreeOffscreenArea(fbArea);

   pATI->have3DWindows = TRUE;
}
#endif /* USE_XAA */

#ifdef USE_EXA
static void ATIDRITransitionTo2d_EXA(ScreenPtr pScreen)
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;

   exaEnableDisableFBAccess(SCREEN_ARG(pScreen), FALSE);

   pATI->pExa->offScreenBase = pATIDRIServer->backOffset;

   exaEnableDisableFBAccess(SCREEN_ARG(pScreen), TRUE);

   pATI->have3DWindows = FALSE;
}

static void ATIDRITransitionTo3d_EXA(ScreenPtr pScreen)
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;

   exaEnableDisableFBAccess(SCREEN_ARG(pScreen), FALSE);

   pATI->pExa->offScreenBase = pATIDRIServer->textureOffset +
			       pATIDRIServer->textureSize;

   exaEnableDisableFBAccess(SCREEN_ARG(pScreen), TRUE);

   pATI->have3DWindows = TRUE;
}
#endif /* USE_EXA */

/* Initialize the state of the back and depth buffers. */
static void ATIDRIInitBuffers( WindowPtr pWin, RegionPtr prgn, CARD32 indx )
{
#ifdef USE_XAA
   ScreenPtr   pScreen = pWin->drawable.pScreen;
   ScrnInfoPtr pScreenInfo   = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;
   XAAInfoRecPtr pXAAInfo = pATI->pXAAInfo;
   BoxPtr      pbox, pboxSave;
   int         nbox, nboxSave;
   int         depth;

   depth = 0x0000ffff;

   if (!pXAAInfo)
      return;

   if (!pXAAInfo->SetupForSolidFill)
      return;
   
   /* FIXME: Only initialize the back and depth buffers for contexts
      that request them */

   /* FIXME: Use drm clear? (see Radeon driver) */

   pboxSave = pbox = REGION_RECTS(prgn);
   nboxSave = nbox = REGION_NUM_RECTS(prgn);

   (*pXAAInfo->SetupForSolidFill)(pScreenInfo, 0, GXcopy, (CARD32)(-1));
   for (; nbox; nbox--, pbox++) {
      (*pXAAInfo->SubsequentSolidFillRect)(pScreenInfo,
					      pbox->x1 + pATIDRIServer->fbX,
					      pbox->y1 + pATIDRIServer->fbY,
					      pbox->x2 - pbox->x1,
					      pbox->y2 - pbox->y1);
      (*pXAAInfo->SubsequentSolidFillRect)(pScreenInfo,
					      pbox->x1 + pATIDRIServer->backX,
					      pbox->y1 + pATIDRIServer->backY,
					      pbox->x2 - pbox->x1,
					      pbox->y2 - pbox->y1);
   }

   pbox = pboxSave;
   nbox = nboxSave;

   (*pXAAInfo->SetupForSolidFill)(pScreenInfo, depth, GXcopy, (CARD32)(-1));
   for (; nbox; nbox--, pbox++)
      (*pXAAInfo->SubsequentSolidFillRect)(pScreenInfo,
					      pbox->x1 + pATIDRIServer->depthX,
					      pbox->y1 + pATIDRIServer->depthY,
					      pbox->x2 - pbox->x1,
					      pbox->y2 - pbox->y1);

   ATIDRIMarkSyncInt(pScreenInfo);
#endif
}

/* Copy the back and depth buffers when the X server moves a window.
 * 
 * Note: this function was copied from the Radeon driver...
 * 
 * This routine is a modified form of XAADoBitBlt with the calls to
 * ScreenToScreenBitBlt built in. My routine has the prgnSrc as source
 * instead of destination. My origin is upside down so the ydir cases
 * are reversed.
 */
static void ATIDRIMoveBuffers( WindowPtr pWin, DDXPointRec ptOldOrg,
			       RegionPtr prgnSrc, CARD32 indx )
{
#ifdef USE_XAA
    ScreenPtr pScreen = pWin->drawable.pScreen;
    ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
    ATIPtr pATI = ATIPTR(pScreenInfo);
    XAAInfoRecPtr pXAAInfo = pATI->pXAAInfo;

    int backOffsetPitch =  (((pATI->pDRIServerInfo->backPitch/8) << 22) |
					   (pATI->pDRIServerInfo->backOffset >> 3));
#if 0
    int depthOffsetPitch = (((pATI->pDRIServerInfo->depthPitch/8) << 22) |
					   (pATI->pDRIServerInfo->depthOffset >> 3));
#endif
    BoxPtr        pboxTmp, pboxNext, pboxBase;
    DDXPointPtr   pptTmp;
    int           xdir, ydir;

    int           screenwidth = pScreenInfo->virtualX;
    int           screenheight = pScreenInfo->virtualY;

    BoxPtr        pbox     = REGION_RECTS(prgnSrc);
    int           nbox     = REGION_NUM_RECTS(prgnSrc);

    BoxPtr        pboxNew1 = NULL;
    BoxPtr        pboxNew2 = NULL;
    DDXPointPtr   pptNew1  = NULL;
    DDXPointPtr   pptNew2  = NULL;
    DDXPointPtr   pptSrc   = &ptOldOrg;

    int           dx       = pWin->drawable.x - ptOldOrg.x;
    int           dy       = pWin->drawable.y - ptOldOrg.y;

   if (!pXAAInfo)
      return;

   if (!pXAAInfo->SetupForScreenToScreenCopy)
      return;

    /* FIXME: Only move the back and depth buffers for contexts
     * that request them.
     */

    /* If the copy will overlap in Y, reverse the order */
    if (dy > 0) {
	ydir = -1;

	if (nbox > 1) {
	    /* Keep ordering in each band, reverse order of bands */
	    pboxNew1 = (BoxPtr)malloc(sizeof(BoxRec)*nbox);
	    if (!pboxNew1) return;
	    pptNew1 = (DDXPointPtr)malloc(sizeof(DDXPointRec)*nbox);
	    if (!pptNew1) {
		free(pboxNew1);
		return;
	    }
	    pboxBase = pboxNext = pbox+nbox-1;
	    while (pboxBase >= pbox) {
		while ((pboxNext >= pbox) && (pboxBase->y1 == pboxNext->y1))
		    pboxNext--;
		pboxTmp = pboxNext+1;
		pptTmp  = pptSrc + (pboxTmp - pbox);
		while (pboxTmp <= pboxBase) {
		    *pboxNew1++ = *pboxTmp++;
		    *pptNew1++  = *pptTmp++;
		}
		pboxBase = pboxNext;
	    }
	    pboxNew1 -= nbox;
	    pbox      = pboxNew1;
	    pptNew1  -= nbox;
	    pptSrc    = pptNew1;
	}
    } else {
	/* No changes required */
	ydir = 1;
    }

    /* If the regions will overlap in X, reverse the order */
    if (dx > 0) {
	xdir = -1;

	if (nbox > 1) {
	    /* reverse order of rects in each band */
	    pboxNew2 = (BoxPtr)malloc(sizeof(BoxRec)*nbox);
	    pptNew2  = (DDXPointPtr)malloc(sizeof(DDXPointRec)*nbox);
	    if (!pboxNew2 || !pptNew2) {
		free(pptNew2);
		free(pboxNew2);
		free(pptNew1);
		free(pboxNew1);
		return;
	    }
	    pboxBase = pboxNext = pbox;
	    while (pboxBase < pbox+nbox) {
		while ((pboxNext < pbox+nbox)
		       && (pboxNext->y1 == pboxBase->y1))
		    pboxNext++;
		pboxTmp = pboxNext;
		pptTmp  = pptSrc + (pboxTmp - pbox);
		while (pboxTmp != pboxBase) {
		    *pboxNew2++ = *--pboxTmp;
		    *pptNew2++  = *--pptTmp;
		}
		pboxBase = pboxNext;
	    }
	    pboxNew2 -= nbox;
	    pbox      = pboxNew2;
	    pptNew2  -= nbox;
	    pptSrc    = pptNew2;
	}
    } else {
	/* No changes are needed */
	xdir = 1;
    }

    (*pXAAInfo->SetupForScreenToScreenCopy)(pScreenInfo, xdir, ydir, GXcopy,
					       (CARD32)(-1), -1);

    for (; nbox-- ; pbox++) {
	int xa    = pbox->x1;
	int ya    = pbox->y1;
	int destx = xa + dx;
	int desty = ya + dy;
	int w     = pbox->x2 - xa + 1;
	int h     = pbox->y2 - ya + 1;

	if (destx < 0)                xa -= destx, w += destx, destx = 0;
	if (desty < 0)                ya -= desty, h += desty, desty = 0;
	if (destx + w > screenwidth)  w = screenwidth  - destx;
	if (desty + h > screenheight) h = screenheight - desty;

	if (w <= 0) continue;
	if (h <= 0) continue;

	ATIMach64WaitForFIFO(pATI, 2);
	outf(SRC_OFF_PITCH, backOffsetPitch);
	outf(DST_OFF_PITCH, backOffsetPitch);

	(*pXAAInfo->SubsequentScreenToScreenCopy)(pScreenInfo,
						     xa, ya,
						     destx, desty,
						     w, h);
#if 0
	/* FIXME: Move depth buffers? */
	ATIMach64WaitForFIFO(pATI, 2);
	outf(SRC_OFF_PITCH, depthOffsetPitch);
	outf(DST_OFF_PITCH, depthOffsetPitch);

	if (pATI->depthMoves)
	    ATIScreenToScreenCopyDepth(pScreenInfo,
					  xa, ya,
					  destx, desty,
					  w, h);
#endif
    }

    ATIMach64WaitForFIFO(pATI, 2);
    outf(SRC_OFF_PITCH, pATI->NewHW.dst_off_pitch);
    outf(DST_OFF_PITCH, pATI->NewHW.src_off_pitch);

    free(pptNew2);
    free(pboxNew2);
    free(pptNew1);
    free(pboxNew1);

    ATIDRIMarkSyncInt(pScreenInfo);
#endif
}

/* Compute log base 2 of val. */
static int Mach64MinBits(int val)
{
    int bits;

    if (!val) return 1;
    for (bits = 0; val; val >>= 1, ++bits);
    return bits;
}

static Bool ATIDRISetBufSize( ScreenPtr pScreen, unsigned int maxSize )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;

   if (pATI->OptionBufferSize) {
      if (pATI->OptionBufferSize < 1 || pATI->OptionBufferSize > maxSize  ) {
	 xf86DrvMsg( pScreen->myNum, X_ERROR, "[drm] Illegal DMA buffers size: %d MB\n",
		     pATI->OptionBufferSize );
	 return FALSE;
      }
      if (pATI->OptionBufferSize > 2) {
	 xf86DrvMsg( pScreen->myNum, X_WARNING, "[drm] Illegal DMA buffers size: %d MB\n",
		     pATI->OptionBufferSize );
	 xf86DrvMsg( pScreen->myNum, X_WARNING, "[drm] Clamping DMA buffers size to 2 MB\n");
	 pATIDRIServer->bufferSize = 2;
      } else {
	 pATIDRIServer->bufferSize = pATI->OptionBufferSize;
	 xf86DrvMsg( pScreen->myNum, X_CONFIG, "[drm] Using %d MB for DMA buffers\n",
		     pATIDRIServer->bufferSize );
      }
   } else {
      xf86DrvMsg( pScreen->myNum, X_DEFAULT, "[drm] Using %d MB for DMA buffers\n",
		  pATIDRIServer->bufferSize );
   }

   return TRUE;
}

static Bool ATIDRISetAgpMode( ScreenPtr pScreen )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;

   unsigned long mode   = drmAgpGetMode( pATI->drmFD );        /* Default mode */
   unsigned int vendor = drmAgpVendorId( pATI->drmFD );
   unsigned int device = drmAgpDeviceId( pATI->drmFD );

   if (pATI->OptionAGPMode > 0 && pATI->OptionAGPMode <= ATI_AGP_MAX_MODE) {
      pATIDRIServer->agpMode = pATI->OptionAGPMode;
      xf86DrvMsg( pScreen->myNum, X_CONFIG, "[agp] Using AGP %dx Mode\n", 
		  pATIDRIServer->agpMode );
   } else if (pATI->OptionAGPMode > 0) {
      xf86DrvMsg( pScreen->myNum, X_ERROR, "[agp] Illegal AGP Mode: %d\n", 
		  pATI->OptionAGPMode );
      return FALSE;
   } else {
      /* If no mode configured, use the default mode obtained from agpgart */
      if ( mode & AGP_MODE_2X ) {
	 pATIDRIServer->agpMode = 2;
      } else if ( mode & AGP_MODE_1X ) {
	 pATIDRIServer->agpMode = 1;
      }
      xf86DrvMsg( pScreen->myNum, X_DEFAULT, "[agp] Using AGP %dx Mode\n", 
		  pATIDRIServer->agpMode );
   }

   mode &= ~AGP_MODE_MASK;
   switch ( pATIDRIServer->agpMode ) {
   case 2:          mode |= AGP_MODE_2X;
   case 1: default: mode |= AGP_MODE_1X;
   }

   if (pATI->OptionAGPSize) {
      switch (pATI->OptionAGPSize) {
      case 128:
      case  64:
      case  32:
      case  16:
      case   8:
      case   4:
	 pATIDRIServer->agpSize = pATI->OptionAGPSize;
	 xf86DrvMsg( pScreen->myNum, X_CONFIG, "[agp] Using %d MB AGP aperture\n", 
		     pATIDRIServer->agpSize );
	 break;
      default:
	 xf86DrvMsg( pScreen->myNum, X_ERROR,
		     "[agp] Illegal aperture size %d MB\n", pATI->OptionAGPSize );
	 return FALSE;
      }
   } else {
      xf86DrvMsg( pScreen->myNum, X_DEFAULT, "[agp] Using %d MB AGP aperture\n", 
		  pATIDRIServer->agpSize );
   }

   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[agp] Mode 0x%08lx [AGP 0x%04x/0x%04x; Card 0x%04x/0x%04x]\n",
	       mode, vendor, device,
	       PCI_DEV_VENDOR_ID(pATI->PCIInfo),
	       PCI_DEV_DEVICE_ID(pATI->PCIInfo) );

   if ( drmAgpEnable( pATI->drmFD, mode ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR, "[agp] AGP not enabled\n" );
      drmAgpRelease( pATI->drmFD );
      return FALSE;
   }

   return TRUE;
}

/* Initialize the AGP state.  Request memory for use in AGP space, and
 * initialize the Rage Pro registers to point to that memory.
 */
static Bool ATIDRIAgpInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;

   int ret;
   unsigned long cntl;
   int s, l;

   pATIDRIServer->agpSize = ATI_DEFAULT_AGP_SIZE;
   pATIDRIServer->agpMode = ATI_DEFAULT_AGP_MODE;
   pATIDRIServer->bufferSize = ATI_DEFAULT_BUFFER_SIZE;
   pATIDRIServer->ringSize = 16; /* 16 kB ring */

   if ( drmAgpAcquire( pATI->drmFD ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_WARNING, "[agp] AGP not available\n" );
      return FALSE;
   }

   if (!ATIDRISetAgpMode( pScreen ))
      return FALSE;

   pATIDRIServer->agpOffset = 0;

   ret = drmAgpAlloc( pATI->drmFD, pATIDRIServer->agpSize*1024*1024,
		      0, NULL, &pATIDRIServer->agpHandle );
   if ( ret < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR, "[agp] Out of memory (%d)\n", ret );
      drmAgpRelease( pATI->drmFD );
      return FALSE;
   }
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[agp] %d kB allocated with handle 0x%08x\n",
	       pATIDRIServer->agpSize*1024, pATIDRIServer->agpHandle );

   if ( drmAgpBind( pATI->drmFD, pATIDRIServer->agpHandle, pATIDRIServer->agpOffset) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR, "[agp] Could not bind\n" );
      drmAgpFree( pATI->drmFD, pATIDRIServer->agpHandle );
      drmAgpRelease( pATI->drmFD );
      return FALSE;
   }

   xf86DrvMsg(pScreen->myNum, X_INFO,
	      "[agp] Using %d kB for DMA descriptor ring\n", pATIDRIServer->ringSize);

   if ( !ATIDRISetBufSize( pScreen, pATIDRIServer->agpSize ) )
      return FALSE;

   pATIDRIServer->agpTexSize    = pATIDRIServer->agpSize - pATIDRIServer->bufferSize;

   /* Reserve space for the DMA descriptor ring */
   pATIDRIServer->ringStart   = pATIDRIServer->agpOffset;
   pATIDRIServer->ringMapSize = pATIDRIServer->ringSize*1024; /* ringSize is in kB */

   /* Reserve space for the vertex buffer */
   pATIDRIServer->bufferStart   = pATIDRIServer->ringStart + pATIDRIServer->ringMapSize;
   pATIDRIServer->bufferMapSize = pATIDRIServer->bufferSize*1024*1024;
    
   /* Reserve the rest for AGP textures */
   pATIDRIServer->agpTexStart = pATIDRIServer->bufferStart + pATIDRIServer->bufferMapSize;
   s = (pATIDRIServer->agpSize*1024*1024 - pATIDRIServer->agpTexStart);
   l = Mach64MinBits((s-1) / MACH64_NR_TEX_REGIONS);
   if (l < MACH64_LOG_TEX_GRANULARITY) l = MACH64_LOG_TEX_GRANULARITY;
   pATIDRIServer->agpTexMapSize   = (s >> l) << l;
   pATIDRIServer->log2AGPTexGran  = l;

   xf86DrvMsg(pScreen->myNum, X_INFO,
	      "[agp] Using %d kB for AGP textures\n", pATIDRIServer->agpTexMapSize/1024);

   /* Map DMA descriptor ring */
   if ( drmAddMap( pATI->drmFD, pATIDRIServer->ringStart, pATIDRIServer->ringMapSize,
		   DRM_AGP, DRM_RESTRICTED, &pATIDRIServer->ringHandle ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[agp] Could not add ring mapping\n" );
      return FALSE;
   }
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[agp] ring handle = 0x%08x\n",
	       pATIDRIServer->ringHandle );

   if ( drmMap( pATI->drmFD, pATIDRIServer->ringHandle,
		pATIDRIServer->ringMapSize, &pATIDRIServer->ringMap ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[agp] Could not map ring\n" );
      return FALSE;
   }
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[agp] Ring mapped at 0x%08lx\n",
	       (unsigned long)pATIDRIServer->ringMap );

   /* Map vertex buffers */
   if ( drmAddMap( pATI->drmFD, pATIDRIServer->bufferStart, pATIDRIServer->bufferMapSize,
		   DRM_AGP, DRM_READ_ONLY, &pATIDRIServer->bufferHandle ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[agp] Could not add vertex buffers mapping\n" );
      return FALSE;
   }
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[agp] vertex buffers handle = 0x%08x\n",
	       pATIDRIServer->bufferHandle );

   if ( drmMap( pATI->drmFD, pATIDRIServer->bufferHandle,
		pATIDRIServer->bufferMapSize, &pATIDRIServer->bufferMap ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[agp] Could not map vertex buffers\n" );
      return FALSE;
   }
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[agp] Vertex buffers mapped at 0x%08lx\n",
	       (unsigned long)pATIDRIServer->bufferMap );

   /* Map AGP Textures */
   if (drmAddMap(pATI->drmFD, pATIDRIServer->agpTexStart, pATIDRIServer->agpTexMapSize,
		 DRM_AGP, 0, &pATIDRIServer->agpTexHandle) < 0) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[agp] Could not add AGP texture region mapping\n");
      return FALSE;
   }
   xf86DrvMsg(pScreen->myNum, X_INFO,
	      "[agp] AGP texture region handle = 0x%08x\n",
	      pATIDRIServer->agpTexHandle);

   if (drmMap(pATI->drmFD, pATIDRIServer->agpTexHandle, pATIDRIServer->agpTexMapSize,
	      &pATIDRIServer->agpTexMap) < 0) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[agp] Could not map AGP texture region\n");
      return FALSE;
   }
   xf86DrvMsg(pScreen->myNum, X_INFO,
	      "[agp] AGP Texture region mapped at 0x%08lx\n",
	      (unsigned long)pATIDRIServer->agpTexMap);

   /* Initialize Mach64's AGP registers */
   cntl  = inm( AGP_CNTL );
   cntl &= ~AGP_APER_SIZE_MASK;
   switch ( pATIDRIServer->agpSize ) {
   case 256: cntl |= AGP_APER_SIZE_256MB; break;
   case 128: cntl |= AGP_APER_SIZE_128MB; break;
   case  64: cntl |= AGP_APER_SIZE_64MB;  break;
   case  32: cntl |= AGP_APER_SIZE_32MB;  break;
   case  16: cntl |= AGP_APER_SIZE_16MB;  break;
   case   8: cntl |= AGP_APER_SIZE_8MB;   break;
   case   4: cntl |= AGP_APER_SIZE_4MB;   break;
   default:
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[agp] Illegal aperture size %d kB\n",
		  pATIDRIServer->agpSize*1024 );
      return FALSE;
   }

   /* 1 = DATA comes in clock in which TRDY sampled (default) */
   /* 0 = DATA comes in clock after TRDY sampled */
   cntl |= AGP_TRDY_MODE;

   /* 1 = generate all reads as high priority */
   /* 0 = generate all reads as their default priority (default) */
   /* Setting this only works for me at AGP 1x mode (LLD) */
   if (pATIDRIServer->agpMode == 1) {
      cntl |= HIGH_PRIORITY_READ_EN;
   } else {
      cntl &= ~HIGH_PRIORITY_READ_EN;
   }

   outm( AGP_BASE, drmAgpBase(pATI->drmFD) );
   outm( AGP_CNTL, cntl );

   return TRUE;
}

static Bool ATIDRIPciInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;

   pATIDRIServer->bufferSize = ATI_DEFAULT_BUFFER_SIZE;
   pATIDRIServer->ringSize = 16; /* 16 kB ring */

   if ( !ATIDRISetBufSize( pScreen, (unsigned)(-1) ) )
      return FALSE;

   /* Set size of the DMA descriptor ring */
   pATIDRIServer->ringStart   = 0;
   pATIDRIServer->ringMapSize = pATIDRIServer->ringSize*1024; /* ringSize is in kB */

   /* Set size of the vertex buffer */
   pATIDRIServer->bufferStart   = 0;
   pATIDRIServer->bufferMapSize = pATIDRIServer->bufferSize*1024*1024;

   /* Map DMA descriptor ring */
   if ( drmAddMap( pATI->drmFD, 0, pATIDRIServer->ringMapSize,
		   DRM_CONSISTENT, DRM_RESTRICTED, &pATIDRIServer->ringHandle ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[pci] Could not add ring mapping\n" );
      return FALSE;
   }
   xf86DrvMsg( pScreen->myNum, X_INFO, "[pci] ring handle = 0x%08x\n",
	       pATIDRIServer->ringHandle );

   if ( drmMap( pATI->drmFD, pATIDRIServer->ringHandle,
		pATIDRIServer->ringMapSize, &pATIDRIServer->ringMap ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[pci] Could not map ring\n" );
      return FALSE;
   }
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[pci] Ring mapped at 0x%08lx\n",
	       (unsigned long)pATIDRIServer->ringMap );

   /* Disable AGP for ForcePCIMode */
   if ( pATI->BusType != ATI_BUS_PCI ) {
       outm( AGP_BASE, 0 );
       outm( AGP_CNTL, 0 );
   }

   return TRUE;
}

/* Add a map for the MMIO registers that will be accessed by any
 * DRI-based clients.
 */
static Bool ATIDRIMapInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;

   pATIDRIServer->regsSize = getpagesize();
   if ( drmAddMap( pATI->drmFD, pATI->Block1Base,
		   pATIDRIServer->regsSize,
		   DRM_REGISTERS, DRM_READ_ONLY,
		   &pATIDRIServer->regsHandle ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[drm] failed to map registers\n" );
      return FALSE;
   }
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[drm] register handle = 0x%08x\n",
	       pATIDRIServer->regsHandle );

   return TRUE;
}

/* Initialize the kernel data structures. */
static Bool ATIDRIKernelInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;
   drmMach64Init info;

   memset( &info, 0, sizeof(drmMach64Init) );

   info.func 			= DRM_MACH64_INIT_DMA;
   info.sarea_priv_offset	= sizeof(XF86DRISAREARec);
   info.is_pci			= pATIDRIServer->IsPCI;
   info.dma_mode                = pATI->OptionDMAMode;

   info.fb_bpp			= pATI->bitsPerPixel;
   info.front_offset		= pATIDRIServer->frontOffset;
   info.front_pitch		= pATIDRIServer->frontPitch;
   info.back_offset		= pATIDRIServer->backOffset;
   info.back_pitch		= pATIDRIServer->backPitch;

   info.depth_bpp		= 16;
   info.depth_offset		= pATIDRIServer->depthOffset;
   info.depth_pitch		= pATIDRIServer->depthPitch;

   info.fb_offset		= pATI->LinearBase;
   info.mmio_offset		= pATIDRIServer->regsHandle;
   info.ring_offset		= pATIDRIServer->ringHandle;
   info.buffers_offset		= pATIDRIServer->bufferHandle;
   info.agp_textures_offset	= pATIDRIServer->agpTexHandle;

   if ( drmCommandWrite( pATI->drmFD, DRM_MACH64_INIT, 
			 &info, sizeof(drmMach64Init) ) < 0 ) {
      return FALSE;
   } else {
      return TRUE;
   }
}

/* Add a map for the DMA buffers that will be accessed by any
 * DRI-based clients.
 */
static Bool ATIDRIAddBuffers( ScreenPtr pScreen )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;

   /* Initialize vertex buffers */
   if ( pATIDRIServer->IsPCI ) {
      pATIDRIServer->numBuffers = drmAddBufs( pATI->drmFD,
					      pATIDRIServer->bufferMapSize/MACH64_BUFFER_SIZE,
					      MACH64_BUFFER_SIZE,
					      DRM_PCI_BUFFER_RO,
					      0 );
   } else {
      pATIDRIServer->numBuffers = drmAddBufs( pATI->drmFD,
					      pATIDRIServer->bufferMapSize/MACH64_BUFFER_SIZE,
					      MACH64_BUFFER_SIZE,
					      DRM_AGP_BUFFER,
					      pATIDRIServer->bufferStart );
   }
   if ( pATIDRIServer->numBuffers <= 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[drm] Could not create DMA buffers list\n" );
      return FALSE;
   }
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[drm] Added %d %d byte DMA buffers\n",
	       pATIDRIServer->numBuffers, MACH64_BUFFER_SIZE );

    return TRUE;
}

static Bool ATIDRIMapBuffers( ScreenPtr pScreen )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;

   pATIDRIServer->drmBuffers = drmMapBufs( pATI->drmFD );
   if ( !pATIDRIServer->drmBuffers ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[drm] Failed to map DMA buffers list\n" );
      return FALSE;
   }
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[drm] Mapped %d DMA buffers at 0x%08lx\n",
	       pATIDRIServer->drmBuffers->count,
	       (unsigned long)pATIDRIServer->drmBuffers->list->address );

   return TRUE;
}

static Bool ATIDRIIrqInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);

   if ( pATI->irq <= 0 ) {
      pATI->irq = drmGetInterruptFromBusID(pATI->drmFD,
					   PCI_CFG_BUS(pATI->PCIInfo),
					   PCI_CFG_DEV(pATI->PCIInfo),
					   PCI_CFG_FUNC(pATI->PCIInfo));
      if ( pATI->irq <= 0 ) {
	 xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
		    "[drm] Couldn't find IRQ for bus id %d:%d:%d\n",
		    PCI_CFG_BUS(pATI->PCIInfo),
		    PCI_CFG_DEV(pATI->PCIInfo),
		    PCI_CFG_FUNC(pATI->PCIInfo));
	 pATI->irq = 0;
      } else if ((drmCtlInstHandler(pATI->drmFD, pATI->irq)) != 0) {
 	 xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
		    "[drm] Failed to initialize interrupt handler with IRQ %d\n",
		    pATI->irq);
	 pATI->irq = 0;
      }

      if (pATI->irq)
	 xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
		    "[drm] Installed interrupt handler, using IRQ %d\n",
		    pATI->irq);
      else {
	 xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
		    "[drm] Falling back to irq-free operation\n");
	 return FALSE;
      }
   }

   return TRUE;

}

/* Initialize the screen-specific data structures for the DRI and the
 * Rage Pro.  This is the main entry point to the device-specific
 * initialization code.  It calls device-independent DRI functions to
 * create the DRI data structures and initialize the DRI state.
 */
Bool ATIDRIScreenInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   DRIInfoPtr pDRIInfo;
   ATIDRIPtr pATIDRI;
   ATIDRIServerInfoPtr pATIDRIServer;
   drmVersionPtr version;
   int major, minor, patch;

   /* Check that the DRI, and DRM modules have been loaded by testing
    * for known symbols in each module.
    */
   if ( !xf86LoaderCheckSymbol("drmAvailable") ) return FALSE;
   if ( !xf86LoaderCheckSymbol("DRIQueryVersion") ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[dri] ATIDRIScreenInit failed (libdri.a too old)\n" );
      return FALSE;
   }

   /* Check the DRI version */
   DRIQueryVersion( &major, &minor, &patch );
   if ( major != DRIINFO_MAJOR_VERSION || minor < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[dri] ATIDRIScreenInit failed because of a version mismatch.\n"
		  "[dri] libdri version is %d.%d.%d but version %d.%d.x is needed.\n"
		  "[dri] Disabling the DRI.\n",
		  major, minor, patch,
                  DRIINFO_MAJOR_VERSION, 0 );
      return FALSE;
   }

   switch ( pATI->bitsPerPixel ) {
   case 8:
      /* These modes are not supported (yet). */
   case 15:
   case 24:
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[dri] Direct rendering only supported in 16 and 32 bpp modes\n");
      return FALSE;

      /* Only 16 and 32 color depths are supported currently. */
   case 16:
      if ( pATI->depth != 16) {
	 xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[dri] Direct rendering not supported for depth %d at fbbpp 16.\n", pATI->depth );
	 return FALSE;
      }
      break;
   case 32:
      break;
   }

   /* Create the DRI data structure, and fill it in before calling the
    * DRIScreenInit().
    */
   pDRIInfo = DRICreateInfoRec();
   if ( !pDRIInfo ) return FALSE;

   pATI->pDRIInfo = pDRIInfo;
   pDRIInfo->drmDriverName = ATIKernelDriverName;
   pDRIInfo->clientDriverName = ATIClientDriverName;
   if (xf86LoaderCheckSymbol("DRICreatePCIBusID")) {
      pDRIInfo->busIdString = DRICreatePCIBusID(pATI->PCIInfo);
   } else {
      pDRIInfo->busIdString = malloc( 64 );
      sprintf( pDRIInfo->busIdString,
	       "PCI:%d:%d:%d",
	       PCI_DEV_BUS(pATI->PCIInfo),
	       PCI_DEV_DEV(pATI->PCIInfo),
	       PCI_DEV_FUNC(pATI->PCIInfo) );
   }
   pDRIInfo->ddxDriverMajorVersion = MACH64_VERSION_MAJOR;
   pDRIInfo->ddxDriverMinorVersion = MACH64_VERSION_MINOR;
   pDRIInfo->ddxDriverPatchVersion = MACH64_VERSION_PATCH;
   pDRIInfo->frameBufferPhysicalAddress = (void *)pATI->LinearBase;
   pDRIInfo->frameBufferSize = pATI->LinearSize;
   pDRIInfo->frameBufferStride = (pScreenInfo->displayWidth *
				  pATI->FBBytesPerPixel);
   pDRIInfo->ddxDrawableTableEntry = ATI_MAX_DRAWABLES;

   if ( SAREA_MAX_DRAWABLES < ATI_MAX_DRAWABLES ) {
      pDRIInfo->maxDrawableTableEntry = SAREA_MAX_DRAWABLES;
   } else {
      pDRIInfo->maxDrawableTableEntry = ATI_MAX_DRAWABLES;
   }

   /* For now the mapping works by using a fixed size defined
    * in the SAREA header
    */
   if ( sizeof(XF86DRISAREARec) + sizeof(ATISAREAPrivRec) > SAREA_MAX ) {
      ErrorF( "[dri] Data does not fit in SAREA\n" );
      return FALSE;
   }
   xf86DrvMsg( pScreenInfo->scrnIndex, X_INFO, "[drm] SAREA %u+%u: %u\n",
	       (unsigned)sizeof(XF86DRISAREARec),
	       (unsigned)sizeof(ATISAREAPrivRec),
	       (unsigned)(sizeof(XF86DRISAREARec) + sizeof(ATISAREAPrivRec)) );
   pDRIInfo->SAREASize = SAREA_MAX;

   pATIDRI = (ATIDRIPtr) xnfcalloc( sizeof(ATIDRIRec), 1 );
   if ( !pATIDRI ) {
      DRIDestroyInfoRec( pATI->pDRIInfo );
      pATI->pDRIInfo = NULL;
      xf86DrvMsg( pScreenInfo->scrnIndex, X_ERROR,
		  "[dri] Failed to allocate memory for private record\n" );
      return FALSE;
   }
   pATIDRIServer = (ATIDRIServerInfoPtr)
      xnfcalloc( sizeof(ATIDRIServerInfoRec), 1 );
   if ( !pATIDRIServer ) {
      free( pATIDRI );
      DRIDestroyInfoRec( pATI->pDRIInfo );
      pATI->pDRIInfo = NULL;
      xf86DrvMsg( pScreenInfo->scrnIndex, X_ERROR,
		  "[dri] Failed to allocate memory for private record\n" );
      return FALSE;
   }

   pATI->pDRIServerInfo = pATIDRIServer;

   pDRIInfo->devPrivate		= pATIDRI;
   pDRIInfo->devPrivateSize	= sizeof(ATIDRIRec);
   pDRIInfo->contextSize	= sizeof(ATIDRIContextRec);

   pDRIInfo->CreateContext	= ATICreateContext;
   pDRIInfo->DestroyContext	= ATIDestroyContext;
   pDRIInfo->SwapContext	= ATIDRISwapContext;
   pDRIInfo->InitBuffers	= ATIDRIInitBuffers;
   pDRIInfo->MoveBuffers	= ATIDRIMoveBuffers;
#ifdef USE_XAA
   if (!pATI->useEXA) {
      pDRIInfo->TransitionTo2d  = ATIDRITransitionTo2d;
      pDRIInfo->TransitionTo3d  = ATIDRITransitionTo3d;
   }
#endif /* USE_XAA */
#ifdef USE_EXA
   if (pATI->useEXA) {
      pDRIInfo->TransitionTo2d  = ATIDRITransitionTo2d_EXA;
      pDRIInfo->TransitionTo3d  = ATIDRITransitionTo3d_EXA;
   }
#endif /* USE_EXA */
   pDRIInfo->bufferRequests	= DRI_ALL_WINDOWS;

   pDRIInfo->createDummyCtx     = TRUE;
   pDRIInfo->createDummyCtxPriv = FALSE;

   pATI->have3DWindows = FALSE;

   if ( !DRIScreenInit( pScreen, pDRIInfo, &pATI->drmFD ) ) {
      free( pATIDRIServer );
      pATI->pDRIServerInfo = NULL;
      free( pDRIInfo->devPrivate );
      pDRIInfo->devPrivate = NULL;
      DRIDestroyInfoRec( pDRIInfo );
      pDRIInfo = NULL;
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[dri] DRIScreenInit Failed\n" );
      return FALSE;
   }
   
   /* Check the DRM lib version.
      drmGetLibVersion was not supported in version 1.0, so check for
      symbol first to avoid possible crash or hang.
   */
   if (xf86LoaderCheckSymbol("drmGetLibVersion")) {
      version = drmGetLibVersion(pATI->drmFD);
   } else {
      /* drmlib version 1.0.0 didn't have the drmGetLibVersion
	 entry point.  Fake it by allocating a version record
	 via drmGetVersion and changing it to version 1.0.0
      */
      version = drmGetVersion(pATI->drmFD);
      version->version_major      = 1;
      version->version_minor      = 0;
      version->version_patchlevel = 0;
   }

   if (version) {
      if (version->version_major != 1 ||
	  version->version_minor < 1) {
	 /* incompatible drm library version */
	 xf86DrvMsg(pScreen->myNum, X_ERROR,
		    "[dri] ATIDRIScreenInit failed because of a version mismatch.\n"
		    "[dri] libdrm.a module version is %d.%d.%d but version 1.1.x is needed.\n"
		    "[dri] Disabling DRI.\n",
		    version->version_major,
		    version->version_minor,
		    version->version_patchlevel);
	 drmFreeVersion(version);
	 ATIDRICloseScreen(pScreen);
	 return FALSE;
      }
      drmFreeVersion(version);
   }

   /* Check the mach64 DRM version */
   version = drmGetVersion( pATI->drmFD );
   if ( version ) {
      if ( version->version_major != 2 ||
	   version->version_minor < 0 ) {
	 /* Incompatible DRM version */
	 xf86DrvMsg( pScreen->myNum, X_ERROR,
		     "[dri] ATIDRIScreenInit failed because of a version mismatch.\n"
		     "[dri] mach64.o kernel module version is %d.%d.%d, but version 2.x is needed (with 2.x >= 2.0).\n"
		     "[dri] Disabling DRI.\n",
		     version->version_major,
		     version->version_minor,
		     version->version_patchlevel );
	 drmFreeVersion( version );
	 ATIDRICloseScreen( pScreen );
	 return FALSE;
      }
      drmFreeVersion( version );
   }

   switch ( pATI->OptionDMAMode ) {
   case MACH64_MODE_DMA_ASYNC:
      xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] Will request asynchronous DMA mode\n");
      break;
   case MACH64_MODE_DMA_SYNC:
      xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] Will request synchronous DMA mode\n");
      break;
   case MACH64_MODE_MMIO:
      xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] Will request pseudo-DMA (MMIO) mode\n");
      break;
   default:
      xf86DrvMsg(pScreen->myNum, X_WARNING, "[drm] Unknown DMA mode\n");
   }

   pATIDRIServer->IsPCI = (pATI->BusType == ATI_BUS_PCI || pATI->OptionIsPCI) ? TRUE : FALSE;

   if ( pATI->BusType != ATI_BUS_PCI && pATI->OptionIsPCI ) {
       xf86DrvMsg(pScreen->myNum, X_CONFIG, "[dri] Forcing PCI mode\n");
   }

   /* Initialize AGP */
   if ( !pATIDRIServer->IsPCI && !ATIDRIAgpInit( pScreen ) ) {
      pATIDRIServer->IsPCI = TRUE;
      xf86DrvMsg( pScreen->myNum, X_WARNING, "[agp] AGP failed to initialize -- falling back to PCI mode.\n" );
      xf86DrvMsg( pScreen->myNum, X_WARNING, "[agp] Make sure you have the agpgart kernel module loaded.\n" );
   }

   /* Initialize PCI */
   if ( pATIDRIServer->IsPCI && !ATIDRIPciInit( pScreen ) ) {
      ATIDRICloseScreen( pScreen );
      return FALSE;
   }

   if ( !ATIDRIMapInit( pScreen ) ) {
      ATIDRICloseScreen( pScreen );
      return FALSE;
   }

   xf86DrvMsg( pScreenInfo->scrnIndex, X_INFO,
	       "[dri] Visual configs initialized\n" );

   xf86DrvMsg( pScreenInfo->scrnIndex, X_INFO,
	       "[dri] Block 0 base at 0x%08lx\n", pATI->Block0Base );

   return TRUE;
}

/* Finish initializing the device-dependent DRI state, and call
 * DRIFinishScreenInit() to complete the device-independent DRI
 * initialization.
 */
Bool ATIDRIFinishScreenInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATISAREAPrivPtr pSAREAPriv;
   ATIDRIPtr pATIDRI;
   ATIDRIServerInfoPtr pATIDRIServer;

   pATI->pDRIInfo->driverSwapMethod = DRI_HIDE_X_CONTEXT;

   /* NOTE: DRIFinishScreenInit must be called before *DRIKernelInit
    * because *DRIKernelInit requires that the hardware lock is held by
    * the X server, and the first time the hardware lock is grabbed is
    * in DRIFinishScreenInit.
    */
   if ( !DRIFinishScreenInit( pScreen ) ) {
      ATIDRICloseScreen( pScreen );
      return FALSE;
   }

   /* Initialize the DMA buffer list */
   /* Need to do this before ATIDRIKernelInit so we can init the freelist */
   if ( !ATIDRIAddBuffers( pScreen ) ) {
      ATIDRICloseScreen( pScreen );
      return FALSE;
   }

   /* Initialize the kernel data structures */
   if ( !ATIDRIKernelInit( pScreen ) ) {
      xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR, 
		 "[drm] Failed to initialize the mach64.o kernel module\n");
      xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR, 
		 "[drm] Check the system log for more information.\n");
      ATIDRICloseScreen( pScreen );
      return FALSE;
   }

   if ( !ATIDRIMapBuffers( pScreen ) ) {
      ATIDRICloseScreen( pScreen );
      return FALSE;
   }

   /* Initialize IRQ */
   ATIDRIIrqInit( pScreen );

   pSAREAPriv = (ATISAREAPrivPtr) DRIGetSAREAPrivate( pScreen );
   memset( pSAREAPriv, 0, sizeof(*pSAREAPriv) );

   pATIDRI = (ATIDRIPtr)pATI->pDRIInfo->devPrivate;
   pATIDRIServer = pATI->pDRIServerInfo;

   pATIDRI->width = pScreenInfo->virtualX;
   pATIDRI->height = pScreenInfo->virtualY;
   pATIDRI->mem = pScreenInfo->videoRam * 1024;
   pATIDRI->cpp = pScreenInfo->bitsPerPixel / 8;

   pATIDRI->IsPCI = pATIDRIServer->IsPCI;
   pATIDRI->AGPMode = pATIDRIServer->agpMode;

   pATIDRI->frontOffset = pATIDRIServer->frontOffset;
   pATIDRI->frontPitch = pATIDRIServer->frontPitch;

   pATIDRI->backOffset = pATIDRIServer->backOffset;
   pATIDRI->backPitch = pATIDRIServer->backPitch;

   pATIDRI->depthOffset = pATIDRIServer->depthOffset;
   pATIDRI->depthPitch = pATIDRIServer->depthPitch;

   pATIDRI->textureOffset = pATIDRIServer->textureOffset;
   pATIDRI->textureSize	= pATIDRIServer->textureSize;
   pATIDRI->logTextureGranularity = pATIDRIServer->logTextureGranularity;

   pATIDRI->regs = pATIDRIServer->regsHandle;
   pATIDRI->regsSize = pATIDRIServer->regsSize;

   pATIDRI->agp = pATIDRIServer->agpTexHandle;
   pATIDRI->agpSize = pATIDRIServer->agpTexMapSize;
   pATIDRI->logAgpTextureGranularity = pATIDRIServer->log2AGPTexGran;
   pATIDRI->agpTextureOffset = pATIDRIServer->agpTexStart;

   return TRUE;
}

/*
 * This function will attempt to get the Mach64 hardware back into shape
 * after a resume from disc. Its an extract from ATIDRIAgpInit and ATIDRIFinishScreenInit
 * This also calls a new ioctl in the mach64 DRM that in its turn is
 * an extraction of the hardware-affecting bits from mach64_do_init_drm()
 * (see atidrm.c)
 * I am assuming here that pATI->pDRIServerInfo doesn't change
 * elsewhere in incompatible ways.
 * How will this code react to resuming after a failed resumeor pci based dri ?
 */
void ATIDRIResume( ScreenPtr pScreen )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;

   xf86DrvMsg( pScreen->myNum, X_INFO,
		 "[RESUME] Attempting to re-init Mach64 hardware.\n");

   if (!pATIDRIServer->IsPCI) {
      if (!ATIDRISetAgpMode(pScreen))
      return;

      outm( AGP_BASE, drmAgpBase(pATI->drmFD) );
   }
}

/* The screen is being closed, so clean up any state and free any
 * resources used by the DRI.
 */
void ATIDRICloseScreen( ScreenPtr pScreen )
{
   ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pScreen);
   ATIPtr pATI = ATIPTR(pScreenInfo);
   ATIDRIServerInfoPtr pATIDRIServer = pATI->pDRIServerInfo;
   drmMach64Init info;

   /* Stop interrupt generation and handling if used */
   if ( pATI->irq > 0 ) {
      if ( drmCtlUninstHandler(pATI->drmFD) != 0 ) {
	 xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
		    "[drm] Error uninstalling interrupt handler for IRQ %d\n", pATI->irq);
      } else {
	 xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
		    "[drm] Uninstalled interrupt handler for IRQ %d\n", pATI->irq);
      }
      pATI->irq = 0;
   }

   /* De-allocate DMA buffers */
   if ( pATIDRIServer->drmBuffers ) {
      drmUnmapBufs( pATIDRIServer->drmBuffers );
      pATIDRIServer->drmBuffers = NULL;
   }

   /* De-allocate all kernel resources */
   memset(&info, 0, sizeof(drmMach64Init));
   info.func = DRM_MACH64_CLEANUP_DMA;
   drmCommandWrite( pATI->drmFD, DRM_MACH64_INIT,
		    &info, sizeof(drmMach64Init) );

   /* De-allocate all AGP resources */
   if ( pATIDRIServer->agpTexMap ) {
      drmUnmap( pATIDRIServer->agpTexMap, pATIDRIServer->agpTexMapSize );
      pATIDRIServer->agpTexMap = NULL;
   }
   if ( pATIDRIServer->bufferMap ) {
      drmUnmap( pATIDRIServer->bufferMap, pATIDRIServer->bufferMapSize );
      pATIDRIServer->bufferMap = NULL;
   }
   if ( pATIDRIServer->ringMap ) {
      drmUnmap( pATIDRIServer->ringMap, pATIDRIServer->ringMapSize );
      pATIDRIServer->ringMap = NULL;
   }
   if ( pATIDRIServer->agpHandle ) {
      drmAgpUnbind( pATI->drmFD, pATIDRIServer->agpHandle );
      drmAgpFree( pATI->drmFD, pATIDRIServer->agpHandle );
      pATIDRIServer->agpHandle = 0;
      drmAgpRelease( pATI->drmFD );
   }

   /* De-allocate all PCI resources */
   if ( pATIDRIServer->IsPCI && pATIDRIServer->ringHandle ) {
      drmRmMap( pATI->drmFD, pATIDRIServer->ringHandle );
      pATIDRIServer->ringHandle = 0;
   }

   /* De-allocate all DRI resources */
   DRICloseScreen( pScreen );

   /* De-allocate all DRI data structures */
   if ( pATI->pDRIInfo ) {
      if ( pATI->pDRIInfo->devPrivate ) {
	 free( pATI->pDRIInfo->devPrivate );
	 pATI->pDRIInfo->devPrivate = NULL;
      }
      DRIDestroyInfoRec( pATI->pDRIInfo );
      pATI->pDRIInfo = NULL;
   }
   if ( pATI->pDRIServerInfo ) {
      free( pATI->pDRIServerInfo );
      pATI->pDRIServerInfo = NULL;
   }
}
