/*
 * Copyright 1998-2003 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2003 S3 Graphics, Inc. All Rights Reserved.
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
 * VIA, S3 GRAPHICS, AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef SAVAGEDRI

#include "xf86.h"
#include "xf86_OSproc.h"

#ifdef HAVE_XAA_H
#include "xaalocal.h"
#include "xaarop.h"
#endif

#include "xf86Pci.h"
#include "xf86fbman.h"

#include "miline.h"


/*#include "savage_vbe.h"*/
#include "savage_regs.h"
#include "savage_driver.h"
#include "savage_bci.h"
#include "savage_streams.h"
#include "savage_common.h"

#define _XF86DRI_SERVER_
#include "GL/glxtokens.h"
#include "sarea.h"
#include "savage_dri.h"
#include "savage_sarea.h"

static struct {
   int bpp;
   int redSize;
   int greenSize;
   int blueSize;
   int alphaSize;
   int redMask;
   int greenMask;
   int blueMask;
   int alphaMask;
   int depthSize;
} SAVAGEVisuals[] = {
   { 16, 5, 6, 5, 0, 0x0000F800, 0x000007E0, 0x0000001F, 0, 16 },
   { 32, 8, 8, 8, 0, 0x00FF0000, 0x0000FF00, 0x000000FF, 0, 24 },
   {  0, 0, 0, 0, 0,          0,          0,          0, 0,  0 }
};

static char SAVAGEKernelDriverName[] = "savage";
static char SAVAGEClientDriverName[] = "savage";

static Bool SAVAGEDRIOpenFullScreen(ScreenPtr pScreen);
static Bool SAVAGEDRICloseFullScreen(ScreenPtr pScreen);
/* DRI buffer management
 */
void SAVAGEDRIInitBuffers( WindowPtr pWin, RegionPtr prgn,
				CARD32 index );
void SAVAGEDRIMoveBuffers( WindowPtr pParent, DDXPointRec ptOldOrg,
				RegionPtr prgnSrc, CARD32 index );

/*        almost the same besides set src/desc to */
/*        Primary Bitmap Description              */

static void 
SAVAGEDRISetupForScreenToScreenCopy(
    ScrnInfoPtr pScrn, int xdir, int ydir,
    int rop, unsigned planemask, int transparency_color);


static void 
SAVAGEDRISubsequentScreenToScreenCopy(
    ScrnInfoPtr pScrn, int x1, int y1, int x2, int y2,
    int w, int h);


/* Initialize the visual configs that are supported by the hardware.
 * These are combined with the visual configs that the indirect
 * rendering core supports, and the intersection is exported to the
 * client.
 */
static Bool SAVAGEInitVisualConfigs( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   SavagePtr psav = SAVPTR(pScrn);
   int numConfigs = 0;
   __GLXvisualConfig *pConfigs = 0;
   SAVAGEConfigPrivPtr pSAVAGEConfigs = 0;
   SAVAGEConfigPrivPtr *pSAVAGEConfigPtrs = 0;
   int i, db, stencil, accum, visNum;

   switch ( pScrn->bitsPerPixel ) {
   case 8:
   case 24:
      break;

   case 16:
   case 32:
      numConfigs = 8;

      pConfigs = (__GLXvisualConfig*)calloc( sizeof(__GLXvisualConfig),
						numConfigs );
      if ( !pConfigs ) {
	 return FALSE;
      }

      pSAVAGEConfigs = (SAVAGEConfigPrivPtr)calloc( sizeof(SAVAGEConfigPrivRec),
						 numConfigs );
      if ( !pSAVAGEConfigs ) {
	 free( pConfigs );
	 return FALSE;
      }

      pSAVAGEConfigPtrs = (SAVAGEConfigPrivPtr*)calloc( sizeof(SAVAGEConfigPrivPtr),
						     numConfigs );
      if ( !pSAVAGEConfigPtrs ) {
	 free( pConfigs );
	 free( pSAVAGEConfigs );
	 return FALSE;
      }

      for ( i = 0 ; i < numConfigs ; i++ ) {
	 pSAVAGEConfigPtrs[i] = &pSAVAGEConfigs[i];
      }

      for (visNum = 0; SAVAGEVisuals[visNum].bpp != 0; visNum++) {
         if ( SAVAGEVisuals[visNum].bpp == pScrn->bitsPerPixel )
            break;
      }
      if ( SAVAGEVisuals[visNum].bpp == 0 ) {
	 free( pConfigs );
	 free( pSAVAGEConfigs );
         return FALSE;
      }

      i = 0;
      for ( accum = 0 ; accum <= 1 ; accum++ ) {
         for ( stencil = 0 ; stencil <= 1 ; stencil++ ) {
            for ( db = 1 ; db >= 0 ; db-- ) {
               pConfigs[i].vid		= -1;
               pConfigs[i].class	= -1;
               pConfigs[i].rgba		= TRUE;
               pConfigs[i].redSize	= SAVAGEVisuals[visNum].redSize;
               pConfigs[i].greenSize	= SAVAGEVisuals[visNum].greenSize;
               pConfigs[i].blueSize	= SAVAGEVisuals[visNum].blueSize;
               pConfigs[i].alphaSize	= SAVAGEVisuals[visNum].alphaSize;
               pConfigs[i].redMask	= SAVAGEVisuals[visNum].redMask;
               pConfigs[i].greenMask	= SAVAGEVisuals[visNum].greenMask;
               pConfigs[i].blueMask	= SAVAGEVisuals[visNum].blueMask;
               pConfigs[i].alphaMask	= SAVAGEVisuals[visNum].alphaMask;

               if ( accum ) {
                  pConfigs[i].accumRedSize	= 16;
                  pConfigs[i].accumGreenSize	= 16;
                  pConfigs[i].accumBlueSize	= 16;
                  pConfigs[i].accumAlphaSize	= 0;
               } else {
                  pConfigs[i].accumRedSize	= 0;
                  pConfigs[i].accumGreenSize	= 0;
                  pConfigs[i].accumBlueSize	= 0;
                  pConfigs[i].accumAlphaSize	= 0;
               }
               if ( db ) {
                  pConfigs[i].doubleBuffer	= TRUE;
               } else {
                  pConfigs[i].doubleBuffer	= FALSE;
               }
               pConfigs[i].stereo		= FALSE;
               pConfigs[i].bufferSize		= pScrn->bitsPerPixel;
               pConfigs[i].depthSize	= SAVAGEVisuals[visNum].depthSize;
               if ( stencil ) {
                  pConfigs[i].stencilSize	= 8;
               } else {
                  pConfigs[i].stencilSize	= 0;
               }

               pConfigs[i].auxBuffers		= 0;
               pConfigs[i].level		= 0;

               pConfigs[i].visualRating	= GLX_NONE;
               if ( pScrn->bitsPerPixel == 16 ) {
                  if ( accum || stencil ) {
                     pConfigs[i].visualRating	= GLX_SLOW_CONFIG;
                  }
               } else if ( accum ) {
                  pConfigs[i].visualRating	= GLX_SLOW_VISUAL_EXT;
               }
               pConfigs[i].transparentPixel	= GLX_NONE;
               pConfigs[i].transparentRed	= 0;
               pConfigs[i].transparentGreen	= 0;
               pConfigs[i].transparentBlue	= 0;
               pConfigs[i].transparentAlpha	= 0;
               pConfigs[i].transparentIndex	= 0;
               i++;
            }
         }
      }
      if ( i != numConfigs ) {
         xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		     "[drm] Incorrect initialization of visuals\n" );
         return FALSE;
      }
      break;

   default:
      /* Unexpected bits/pixels */
      break;
   }

   psav->numVisualConfigs = numConfigs;
   psav->pVisualConfigs = pConfigs;
   psav->pVisualConfigsPriv = pSAVAGEConfigs;

   GlxSetVisualConfigs( numConfigs, pConfigs, (void **)pSAVAGEConfigPtrs );

   return TRUE;
}

static Bool SAVAGECreateContext( ScreenPtr pScreen, VisualPtr visual,
			      drm_context_t hwContext, void *pVisualConfigPriv,
			      DRIContextType contextStore )
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    SavagePtr psav = SAVPTR(pScrn);

    if(psav->xvmcContext)
        return FALSE;
    else
    {
        psav->DRIrunning++;
    }

    return TRUE;
}

static void SAVAGEDestroyContext( ScreenPtr pScreen, drm_context_t hwContext,
			       DRIContextType contextStore )
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    SavagePtr psav = SAVPTR(pScrn);

    psav->DRIrunning--;
}

static void SAVAGEWakeupHandler(WAKEUPHANDLER_ARGS_DECL)
{
   SCREEN_PTR(arg);
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   SavagePtr psav = SAVPTR(pScrn);

   psav->pDRIInfo->wrap.WakeupHandler = psav->coreWakeupHandler;
   (*psav->pDRIInfo->wrap.WakeupHandler) (WAKEUPHANDLER_ARGS);
   psav->pDRIInfo->wrap.WakeupHandler = SAVAGEWakeupHandler;
   psav->LockHeld = 1;
   if (psav->ShadowStatus) {
      /* fetch the global shadow counter */
#if 0
      if (psav->ShadowCounter != (psav->ShadowVirtual[1023] & 0xffff))
	 xf86DrvMsg( pScrn->scrnIndex, X_INFO,
		     "[dri] WakeupHandler: shadowCounter adjusted from %04x to %04lx\n",
		     psav->ShadowCounter, psav->ShadowVirtual[1023] & 0xffff);
#endif
      psav->ShadowCounter = psav->ShadowVirtual[1023] & 0xffff;
   }
   if (psav->useEXA)
	exaMarkSync(pScreen);
#ifdef HAVE_XAA_H
   else
	psav->AccelInfoRec->NeedToSync = TRUE;
#endif
   /* FK: this flag doesn't seem to be used. */
}

static void SAVAGEBlockHandler(BLOCKHANDLER_ARGS_DECL)
{
   SCREEN_PTR(arg);
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   SavagePtr psav = SAVPTR(pScrn);

   if (psav->ShadowStatus) {
      /* update the global shadow counter */
      CARD32 globalShadowCounter = psav->ShadowVirtual[1023];
      globalShadowCounter = (globalShadowCounter & 0xffff0000) |
	  ((CARD32)psav->ShadowCounter & 0x0000ffff);

#if 0
      if (globalShadowCounter != psav->ShadowVirtual[1023])
	 xf86DrvMsg( pScrn->scrnIndex, X_INFO,
		     "[dri] BlockHandler: shadowCounter adjusted from %08lx to %08x\n",
		     psav->ShadowVirtual[1023], globalShadowCounter);
#endif
      psav->ShadowVirtual[1023] = globalShadowCounter;
   }
   psav->LockHeld = 0;
   psav->pDRIInfo->wrap.BlockHandler = psav->coreBlockHandler;
   (*psav->pDRIInfo->wrap.BlockHandler) (BLOCKHANDLER_ARGS);
   psav->pDRIInfo->wrap.BlockHandler = SAVAGEBlockHandler;
}

static void SAVAGESelectBuffer( ScrnInfoPtr pScrn, int which )
{
   SavagePtr psav = SAVPTR(pScrn);
   SAVAGEDRIServerPrivatePtr pSAVAGEDRIServer = psav->DRIServerInfo;

   psav->WaitIdleEmpty(psav);

   OUTREG(0x48C18,INREG(0x48C18)&(~0x00000008));
   
   switch ( which ) {
   case SAVAGE_BACK:
      OUTREG( 0x8170, pSAVAGEDRIServer->backOffset );
      OUTREG( 0x8174, pSAVAGEDRIServer->backBitmapDesc );
      break;
   case SAVAGE_DEPTH:
      OUTREG( 0x8170, pSAVAGEDRIServer->depthOffset );
      OUTREG( 0x8174, pSAVAGEDRIServer->depthBitmapDesc );
      break;
   default:
   case SAVAGE_FRONT:
      OUTREG( 0x8170, pSAVAGEDRIServer->frontOffset );
      OUTREG( 0x8174, pSAVAGEDRIServer->frontBitmapDesc );
      break;
   }
   OUTREG(0x48C18,INREG(0x48C18)|(0x00000008));
   psav->WaitIdleEmpty(psav);

}


static unsigned int mylog2( unsigned int n )
{
   unsigned int log2 = 1;

   n--;
   while ( n > 1 ) n >>= 1, log2++;

   return log2;
}

static Bool SAVAGESetAgpMode(SavagePtr psav, ScreenPtr pScreen)
{
   unsigned long mode = drmAgpGetMode( psav->drmFD );    /* Default mode */
   unsigned int vendor = drmAgpVendorId( psav->drmFD );
   unsigned int device = drmAgpDeviceId( psav->drmFD );

   mode &= ~SAVAGE_AGP_MODE_MASK;

   switch ( psav->agpMode ) {
   case 4:
      mode |= SAVAGE_AGP_4X_MODE;
   case 2:
      mode |= SAVAGE_AGP_2X_MODE;
   case 1:
   default:
      mode |= SAVAGE_AGP_1X_MODE;
   }

   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[agp] Mode 0x%08lx [AGP 0x%04x/0x%04x; Card 0x%04x/0x%04x]\n",
	       mode, vendor, device,
	       VENDOR_ID(psav->PciInfo),
	       DEVICE_ID(psav->PciInfo));

   if ( drmAgpEnable( psav->drmFD, mode ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR, "[agp] AGP not enabled\n" );
      drmAgpRelease( psav->drmFD );
      return FALSE;
   }

   return TRUE;
}

static Bool SAVAGEDRIAgpInit(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   SavagePtr psav = SAVPTR(pScrn);
   SAVAGEDRIServerPrivatePtr pSAVAGEDRIServer = psav->DRIServerInfo;
   unsigned int offset;
   int ret;

   if (psav->agpSize < 2)
      psav->agpSize = 2; /* at least 2MB for DMA buffers */

   pSAVAGEDRIServer->agp.size = psav->agpSize * 1024 * 1024;
   pSAVAGEDRIServer->agp.offset = pSAVAGEDRIServer->agp.size; /* ? */

   if ( drmAgpAcquire( psav->drmFD ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR, "[agp] AGP not available\n" );
      return FALSE;
   }

   if (!SAVAGESetAgpMode(psav, pScreen)) {
      pSAVAGEDRIServer->agp.handle = 0; /* indicate that AGP init failed */
      return FALSE;
   }

   ret = drmAgpAlloc( psav->drmFD, pSAVAGEDRIServer->agp.size,
		      0, NULL, &pSAVAGEDRIServer->agp.handle );
   if ( ret < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR, "[agp] Out of memory (%d)\n", ret );
      drmAgpRelease( psav->drmFD );
      pSAVAGEDRIServer->agp.handle = 0; /* indicate that AGP init failed */
      return FALSE;
   }
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[agp] %d kB allocated with handle 0x%08lx\n",
	       pSAVAGEDRIServer->agp.size/1024,
	       (unsigned long)pSAVAGEDRIServer->agp.handle );

   if ( drmAgpBind( psav->drmFD, pSAVAGEDRIServer->agp.handle, 0 ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR, "[agp] Could not bind memory\n" );
      drmAgpFree( psav->drmFD, pSAVAGEDRIServer->agp.handle );
      drmAgpRelease( psav->drmFD );
      pSAVAGEDRIServer->agp.handle = 0; /* indicate that AGP init failed */
      return FALSE;
   }

   /* AGP initialization failures above are not fatal, we can fall
    * back to PCI mode. Failures while adding AGP mappings below are
    * fatal though. DRI must be disabled in that case.
    * pSAVAGEDRIServer->agp.handle can be used to distinguish these
    * two cases.
    */

   /* AGP memory layout
    */
   offset = 0;

   if ( psav->AgpDMA ) {
       if ( psav->CommandDMA ) {
	   pSAVAGEDRIServer->cmdDma.offset = offset;
	   pSAVAGEDRIServer->cmdDma.size = SAVAGE_CMDDMA_SIZE;
	   offset += pSAVAGEDRIServer->cmdDma.size;
       } else if ( psav->VertexDMA ) {
	   pSAVAGEDRIServer->buffers.offset = 0;
	   pSAVAGEDRIServer->buffers.size = SAVAGE_NUM_BUFFERS * SAVAGE_BUFFER_SIZE;
	   offset += pSAVAGEDRIServer->buffers.size;
       }
   }

   if (psav->AGPforXv) {
       pSAVAGEDRIServer->agpXVideo.offset = offset;
       pSAVAGEDRIServer->agpXVideo.size = 2 * 1024 * 1024; /* Max XV image is 1024x1024x16bpp */
       offset += pSAVAGEDRIServer->agpXVideo.size;
   } else {
       pSAVAGEDRIServer->agpXVideo.offset = 0;
       pSAVAGEDRIServer->agpXVideo.size = 0;
   }

   pSAVAGEDRIServer->agpTextures.offset = offset;
   pSAVAGEDRIServer->agpTextures.size = (pSAVAGEDRIServer->agp.size - offset);

   /* DMA buffers
    */
   if ( psav->AgpDMA ) {
       if ( psav->CommandDMA ) {
	   if ( drmAddMap( psav->drmFD,
			   pSAVAGEDRIServer->cmdDma.offset,
			   pSAVAGEDRIServer->cmdDma.size,
			   DRM_AGP, DRM_RESTRICTED | DRM_KERNEL,
			   &pSAVAGEDRIServer->cmdDma.handle ) < 0 ) {
	       xf86DrvMsg( pScreen->myNum, X_ERROR,
			   "[agp] Could not add command DMA mapping\n" );
	       return FALSE;
	   }
	   xf86DrvMsg( pScreen->myNum, X_INFO,
		       "[agp] command DMA handle = 0x%08lx\n",
		       (unsigned long)pSAVAGEDRIServer->cmdDma.handle );
	   /* not needed in the server
	   if ( drmMap( psav->drmFD,
			pSAVAGEDRIServer->cmdDma.handle,
			pSAVAGEDRIServer->cmdDma.size,
			&pSAVAGEDRIServer->cmdDma.map ) < 0 ) {
	       xf86DrvMsg( pScreen->myNum, X_ERROR,
			   "[agp] Could not map command DMA\n" );
	       return FALSE;
	   }
	   xf86DrvMsg( pScreen->myNum, X_INFO,
		       "[agp] command DMA mapped at 0x%08lx\n",
		       (unsigned long)pSAVAGEDRIServer->cmdDma.map );
	   */
       } else if ( psav->VertexDMA ) {
	   if ( drmAddMap( psav->drmFD,
			   pSAVAGEDRIServer->buffers.offset,
			   pSAVAGEDRIServer->buffers.size,
			   DRM_AGP, 0,
			   &pSAVAGEDRIServer->buffers.handle ) < 0 ) {
	       xf86DrvMsg( pScreen->myNum, X_ERROR,
			   "[agp] Could not add DMA buffers mapping\n" );
	       return FALSE;
	   }
	   xf86DrvMsg( pScreen->myNum, X_INFO,
		       "[agp] DMA buffers handle = 0x%08lx\n",
		       (unsigned long)pSAVAGEDRIServer->buffers.handle );
	   /* not needed in the server
	   if ( drmMap( psav->drmFD,
			pSAVAGEDRIServer->buffers.handle,
			pSAVAGEDRIServer->buffers.size,
			&pSAVAGEDRIServer->buffers.map ) < 0 ) {
	       xf86DrvMsg( pScreen->myNum, X_ERROR,
			   "[agp] Could not map DMA buffers\n" );
	       return FALSE;
	   }
	   xf86DrvMsg( pScreen->myNum, X_INFO,
		       "[agp] DMA buffers mapped at 0x%08lx\n",
		       (unsigned long)pSAVAGEDRIServer->buffers.map );
	   */
       }
   }

   /* XVideo buffer
    */
   if (pSAVAGEDRIServer->agpXVideo.size != 0) {
       if ( drmAddMap( psav->drmFD,
		   pSAVAGEDRIServer->agpXVideo.offset,
		   pSAVAGEDRIServer->agpXVideo.size,
		   DRM_AGP, 0,
		   &pSAVAGEDRIServer->agpXVideo.handle ) < 0 ) {
	    xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[agp] Could not add agpXVideo, will use framebuffer upload (slower) \n" );
	    pSAVAGEDRIServer->agpXVideo.size = 0;
	    pSAVAGEDRIServer->agpXVideo.handle = 0;
       } else {
	    xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[agp] agpXVideo handle = 0x%08lx\n",
	       (unsigned long)pSAVAGEDRIServer->agpXVideo.handle );
       }
   }

   /* AGP textures
    */
   if ( drmAddMap( psav->drmFD,
		   pSAVAGEDRIServer->agpTextures.offset,
		   pSAVAGEDRIServer->agpTextures.size,
		   DRM_AGP, 0,
		   &pSAVAGEDRIServer->agpTextures.handle ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[agp] Could not add agpTextures \n" );
      return FALSE;
   }
   /*   pSAVAGEDRIServer->agp_offset=pSAVAGEDRIServer->agpTexture.size;*/
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[agp] agpTextures handle = 0x%08lx\n",
	       (unsigned long)pSAVAGEDRIServer->agpTextures.handle );

   /* not needed in the server
   if ( drmMap( psav->drmFD,
		pSAVAGEDRIServer->agpTextures.handle,
		pSAVAGEDRIServer->agpTextures.size,
		&pSAVAGEDRIServer->agpTextures.map ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[agp] Could not map agpTextures \n" );
      return FALSE;
   }
   
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[agp] agpTextures mapped at 0x%08lx\n",
	       (unsigned long)pSAVAGEDRIServer->agpTextures.map );
   */
   
   return TRUE;
}

static Bool SAVAGEDRIMapInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   SavagePtr psav = SAVPTR(pScrn);
   SAVAGEDRIServerPrivatePtr pSAVAGEDRIServer = psav->DRIServerInfo;

   pSAVAGEDRIServer->registers.size = SAVAGEIOMAPSIZE;

   if ( drmAddMap( psav->drmFD,
		   (drm_handle_t)psav->MmioRegion.base,
		   pSAVAGEDRIServer->registers.size,
		   DRM_REGISTERS,0,
		   &pSAVAGEDRIServer->registers.handle ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[drm] Could not add MMIO registers mapping\n" );
      return FALSE;
   }
   
   pSAVAGEDRIServer->aperture.size = 5 * 0x01000000;
   
   if ( drmAddMap( psav->drmFD,
		   (drm_handle_t)(psav->ApertureRegion.base),
		   pSAVAGEDRIServer->aperture.size,
		   DRM_FRAME_BUFFER,0,
		   &pSAVAGEDRIServer->aperture.handle ) < 0 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[drm] Could not add aperture mapping\n" );
      return FALSE;
   }
   
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[drm] aperture handle = 0x%08lx\n",
	       (unsigned long)pSAVAGEDRIServer->aperture.handle );

   /*if(drmMap(psav->drmFD,
          pSAVAGEDRIServer->registers.handle,
          pSAVAGEDRIServer->registers.size,
          &pSAVAGEDRIServer->registers.map)<0)
   {
         xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[drm] Could not map MMIO registers region to virtual\n" );
      return FALSE;
   
   }*/

   if ( !psav->AgpDMA && psav->CommandDMA ) {
       pSAVAGEDRIServer->cmdDma.size = SAVAGE_CMDDMA_SIZE;
       if ( drmAddMap( psav->drmFD, 0, pSAVAGEDRIServer->cmdDma.size,
		       DRM_CONSISTENT, DRM_RESTRICTED | DRM_LOCKED |
		       DRM_KERNEL | DRM_WRITE_COMBINING,
		       &pSAVAGEDRIServer->cmdDma.handle ) < 0 ) {
	   psav->CommandDMA = FALSE;
	   xf86DrvMsg( pScreen->myNum, X_WARNING,
		       "[drm] Could not add PCI command DMA mapping\n" );
       } else
	   xf86DrvMsg( pScreen->myNum, X_INFO,
		       "[drm] PCI command DMA handle = 0x%08lx\n",
		       (unsigned long)pSAVAGEDRIServer->cmdDma.handle );
   }

   /* Enable ShadowStatus by default for direct rendering. */
   if ( !psav->ShadowStatus && !psav->ForceShadowStatus ) {
       psav->ShadowStatus = TRUE;
       xf86DrvMsg( pScreen->myNum, X_INFO,
		   "[drm] Enabling ShadowStatus for DRI.\n" );
   }

   /* If shadow status is manually or automatically enabled, use a
    * page in system memory. */
   if ( psav->ShadowStatus ) {
       pSAVAGEDRIServer->status.size = 4096; /* 1 page */

       if ( drmAddMap( psav->drmFD, 0, pSAVAGEDRIServer->status.size,
		       DRM_CONSISTENT, DRM_READ_ONLY | DRM_LOCKED | DRM_KERNEL,
		       &pSAVAGEDRIServer->status.handle ) < 0 ) {
	   xf86DrvMsg( pScreen->myNum, X_ERROR,
		       "[drm] Could not add status page mapping\n" );
	   return FALSE;
       }
       xf86DrvMsg( pScreen->myNum, X_INFO,
		   "[drm] Status handle = 0x%08lx\n",
		   (unsigned long)pSAVAGEDRIServer->status.handle );

       if ( drmMap( psav->drmFD,
		    pSAVAGEDRIServer->status.handle,
		    pSAVAGEDRIServer->status.size,
		    &pSAVAGEDRIServer->status.map ) < 0 ) {
	   xf86DrvMsg( pScreen->myNum, X_ERROR,
		       "[drm] Could not map status page\n" );
	   return FALSE;
       }
       xf86DrvMsg( pScreen->myNum, X_INFO,
		   "[drm] Status page mapped at 0x%08lx\n",
		   (unsigned long)pSAVAGEDRIServer->status.map );

       psav->ShadowPhysical = pSAVAGEDRIServer->status.handle;
       psav->ShadowVirtual = pSAVAGEDRIServer->status.map;
   }

   return TRUE;
}

static Bool SAVAGEDRIBuffersInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   SavagePtr psav = SAVPTR(pScrn);
   SAVAGEDRIServerPrivatePtr pSAVAGEDRIServer = psav->DRIServerInfo;
   int count;

   if ( !psav->VertexDMA || psav->CommandDMA ) {
       /* At this point psav->CommandDMA == TRUE means that CommandDMA
	* allocation was actually successful. */
       psav->VertexDMA = FALSE;
       return TRUE;
   }

   if ( psav->AgpDMA ) {
       count = drmAddBufs( psav->drmFD,
			   SAVAGE_NUM_BUFFERS, SAVAGE_BUFFER_SIZE,
			   DRM_AGP_BUFFER, pSAVAGEDRIServer->buffers.offset );
   } else {
       count = drmAddBufs( psav->drmFD,
			   SAVAGE_NUM_BUFFERS, SAVAGE_BUFFER_SIZE,
			   0, 0 );
   }
   if ( count <= 0 ) {
       xf86DrvMsg( pScrn->scrnIndex, X_INFO,
		   "[drm] failure adding %d %d byte DMA buffers (%d)\n",
		   SAVAGE_NUM_BUFFERS, SAVAGE_BUFFER_SIZE, count );
       return FALSE;
   }
   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[drm] Added %d %d byte DMA buffers\n",
	       count, SAVAGE_BUFFER_SIZE );

   /* not needed in the server
   pSAVAGEDRIServer->drmBuffers = drmMapBufs( psav->drmFD );
   if ( !pSAVAGEDRIServer->drmBuffers ) {
	xf86DrvMsg( pScreen->myNum, X_ERROR,
		    "[drm] Failed to map DMA buffers list\n" );
	return FALSE;
    }
    xf86DrvMsg( pScreen->myNum, X_INFO,
		"[drm] Mapped %d DMA buffers\n",
		pSAVAGEDRIServer->drmBuffers->count );
   */

    return TRUE;
}

static Bool SAVAGEDRIKernelInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   SavagePtr psav = SAVPTR(pScrn);
   SAVAGEDRIServerPrivatePtr pSAVAGEDRIServer = psav->DRIServerInfo;
   drmSAVAGEInit init;
   int ret;

   memset( &init, 0, sizeof(drmSAVAGEInit) );

   init.func = SAVAGE_INIT_BCI;
   init.sarea_priv_offset = sizeof(XF86DRISAREARec);

   init.cob_size = psav->cobSize/4; /* size in 32-bit entries */
   init.bci_threshold_lo = psav->bciThresholdLo;
   init.bci_threshold_hi = psav->bciThresholdHi;
   init.dma_type = psav->AgpDMA ? SAVAGE_DMA_AGP : SAVAGE_DMA_PCI;

   init.fb_bpp		= pScrn->bitsPerPixel;
   init.front_offset	= pSAVAGEDRIServer->frontOffset;
   init.front_pitch	= pSAVAGEDRIServer->frontPitch;
   init.back_offset	= pSAVAGEDRIServer->backOffset;
   init.back_pitch	= pSAVAGEDRIServer->backPitch;

   init.depth_bpp	= pScrn->bitsPerPixel;
   init.depth_offset	= pSAVAGEDRIServer->depthOffset;
   init.depth_pitch	= pSAVAGEDRIServer->depthPitch;

   init.texture_offset  = pSAVAGEDRIServer->textureOffset;
   init.texture_size    = pSAVAGEDRIServer->textureSize;

   init.status_offset   = pSAVAGEDRIServer->status.handle;
   init.agp_textures_offset = pSAVAGEDRIServer->agpTextures.handle;

   /* Savage4-based chips with DRM version >= 2.4 support command DMA,
    * which is preferred because it works with all vertex
    * formats. Command DMA and vertex DMA don't work at the same
    * time. */
   init.buffers_offset = 0;
   init.cmd_dma_offset = 0;
   if ( psav->CommandDMA )
       init.cmd_dma_offset = pSAVAGEDRIServer->cmdDma.handle;
   else if ( psav->VertexDMA )
       init.buffers_offset = pSAVAGEDRIServer->buffers.handle;

   ret = drmCommandWrite( psav->drmFD, DRM_SAVAGE_BCI_INIT, &init, sizeof(init) );
   if ( ret < 0 ) {
      xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		  "[drm] Failed to initialize BCI! (%d)\n", ret );
      return FALSE;
   }

   return TRUE;
}

Bool SAVAGEDRIScreenInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   SavagePtr psav = SAVPTR(pScrn);
   DRIInfoPtr pDRIInfo;
   SAVAGEDRIPtr pSAVAGEDRI;
   SAVAGEDRIServerPrivatePtr pSAVAGEDRIServer;

   /* Check that the GLX, DRI, and DRM modules have been loaded by testing
    * for canonical symbols in each module.
    */
   if ( !xf86LoaderCheckSymbol( "GlxSetVisualConfigs" ) )	return FALSE;
   if ( !xf86LoaderCheckSymbol( "drmAvailable" ) )		return FALSE;
   if ( !xf86LoaderCheckSymbol( "DRIQueryVersion" ) ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[dri] SAVAGEDRIScreenInit failed (libdri.a too old)\n" );
      return FALSE;
   }

   /* Check the DRI version */
   {
      int major, minor, patch;
      DRIQueryVersion( &major, &minor, &patch );
      if ( major != DRIINFO_MAJOR_VERSION || minor < DRIINFO_MINOR_VERSION ) {
         xf86DrvMsg( pScreen->myNum, X_ERROR,
		     "[dri] SAVAGEDRIScreenInit failed because of a version mismatch.\n"
		     "[dri] libdri version = %d.%d.%d but version %d.%d.x is needed.\n"
		     "[dri] Disabling the DRI.\n",
		     major, minor, patch,
                     DRIINFO_MAJOR_VERSION, DRIINFO_MINOR_VERSION );
         return FALSE;
      }
   }

   xf86DrvMsg( pScreen->myNum, X_INFO,
	       "[drm] bpp: %d depth: %d\n",
	       pScrn->bitsPerPixel, pScrn->depth );

   if ( (pScrn->bitsPerPixel / 8) != 2 &&
	(pScrn->bitsPerPixel / 8) != 4 ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[dri] Direct rendering only supported in 16 and 32 bpp modes\n" );
      return FALSE;
   }

   pDRIInfo = DRICreateInfoRec();
   if ( !pDRIInfo ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[dri] DRICreateInfoRec() failed\n" );
      return FALSE;
   }
   psav->pDRIInfo = pDRIInfo;

   pDRIInfo->drmDriverName = SAVAGEKernelDriverName;
   pDRIInfo->clientDriverName = SAVAGEClientDriverName;
   if (xf86LoaderCheckSymbol("DRICreatePCIBusID")) {
      pDRIInfo->busIdString = DRICreatePCIBusID(psav->PciInfo);
   } else {
      pDRIInfo->busIdString            = malloc(64);
      sprintf(pDRIInfo->busIdString,
              "PCI:%d:%d:%d",
              psav->PciInfo->bus,
#ifdef XSERVER_LIBPCIACCESS
              psav->PciInfo->dev,
#else
              psav->PciInfo->device,
#endif
              psav->PciInfo->func);
   }
   pDRIInfo->ddxDriverMajorVersion = SAVAGE_VERSION_MAJOR;
   pDRIInfo->ddxDriverMinorVersion = SAVAGE_VERSION_MINOR;
   pDRIInfo->ddxDriverPatchVersion = SAVAGE_PATCHLEVEL;

   pDRIInfo->frameBufferPhysicalAddress = (pointer)(uintptr_t) psav->FbRegion.base;
   pDRIInfo->frameBufferSize = psav->videoRambytes;
   pDRIInfo->frameBufferStride = pScrn->displayWidth*(pScrn->bitsPerPixel/8);
   pDRIInfo->ddxDrawableTableEntry = SAVAGE_MAX_DRAWABLES;

   /* override default DRI block and wakeup handler */
   psav->coreBlockHandler = pDRIInfo->wrap.BlockHandler;
   pDRIInfo->wrap.BlockHandler = SAVAGEBlockHandler;
   psav->coreWakeupHandler = pDRIInfo->wrap.WakeupHandler;
   pDRIInfo->wrap.WakeupHandler = SAVAGEWakeupHandler;

   pDRIInfo->createDummyCtx = TRUE;
   pDRIInfo->createDummyCtxPriv = FALSE;

   if ( SAREA_MAX_DRAWABLES < SAVAGE_MAX_DRAWABLES ) {
      pDRIInfo->maxDrawableTableEntry = SAREA_MAX_DRAWABLES;
   } else {
      pDRIInfo->maxDrawableTableEntry = SAVAGE_MAX_DRAWABLES;
   }

   /* For now the mapping works by using a fixed size defined
    * in the SAREA header.
    */
   if ( sizeof(XF86DRISAREARec) + sizeof(SAVAGESAREAPrivRec) > SAREA_MAX ) {
      xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		  "[drm] Data does not fit in SAREA\n" );
      return FALSE;
   }

   xf86DrvMsg( pScrn->scrnIndex, X_INFO,
	       "[drm] Sarea %d+%d: %d\n",
	       sizeof(XF86DRISAREARec), sizeof(SAVAGESAREAPrivRec),
	       sizeof(XF86DRISAREARec) + sizeof(SAVAGESAREAPrivRec) );

   pDRIInfo->SAREASize = SAREA_MAX;

   pSAVAGEDRI = (SAVAGEDRIPtr)calloc( sizeof(SAVAGEDRIRec), 1 );
   if ( !pSAVAGEDRI ) {
      DRIDestroyInfoRec( psav->pDRIInfo );
      psav->pDRIInfo = 0;
      xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		  "[drm] Failed to allocate memory for private record\n" );
      return FALSE;
   }

   pSAVAGEDRIServer = (SAVAGEDRIServerPrivatePtr)
      calloc( sizeof(SAVAGEDRIServerPrivateRec), 1 );
   if ( !pSAVAGEDRIServer ) {
      free( pSAVAGEDRI );
      DRIDestroyInfoRec( psav->pDRIInfo );
      psav->pDRIInfo = 0;
      xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		  "[drm] Failed to allocate memory for private record\n" );
      return FALSE;
   }
   psav->DRIServerInfo = pSAVAGEDRIServer;

   pDRIInfo->devPrivate = pSAVAGEDRI;
   pDRIInfo->devPrivateSize = sizeof(SAVAGEDRIRec);
   pDRIInfo->contextSize = sizeof(SAVAGEDRIContextRec);

   pDRIInfo->CreateContext = SAVAGECreateContext;
   pDRIInfo->DestroyContext = SAVAGEDestroyContext;

   /* FK: SwapContext is not used with KERNEL_SWAP. */
   pDRIInfo->SwapContext = NULL;

   pDRIInfo->InitBuffers = SAVAGEDRIInitBuffers;
   pDRIInfo->MoveBuffers = SAVAGEDRIMoveBuffers;
   pDRIInfo->OpenFullScreen = SAVAGEDRIOpenFullScreen;
   pDRIInfo->CloseFullScreen = SAVAGEDRICloseFullScreen;
   pDRIInfo->bufferRequests = DRI_ALL_WINDOWS;

   if ( !DRIScreenInit( pScreen, pDRIInfo, &psav->drmFD ) ) {
      free( pSAVAGEDRIServer );
      psav->DRIServerInfo = 0;
      free( pDRIInfo->devPrivate );
      pDRIInfo->devPrivate = 0;
      DRIDestroyInfoRec( psav->pDRIInfo );
      psav->pDRIInfo = 0;
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[drm] DRIScreenInit failed.  Disabling DRI.\n" );
      return FALSE;
   }

   /* Check the SAVAGE DRM version */
   {
      drmVersionPtr version = drmGetVersion(psav->drmFD);
      if ( version ) {
         if ( version->version_major != 2 ||
	      version->version_minor < 0 ) {
            /* incompatible drm version */
            xf86DrvMsg( pScreen->myNum, X_ERROR,
			"[dri] SAVAGEDRIScreenInit failed because of a version mismatch.\n"
			"[dri] savage.ko kernel module version is %d.%d.%d but version 2.0.x is needed.\n"
			"[dri] Disabling DRI.\n",
			version->version_major,
			version->version_minor,
			version->version_patchlevel );
            drmFreeVersion( version );
	    SAVAGEDRICloseScreen( pScreen );		/* FIXME: ??? */
            return FALSE;
         }
	 if ( psav->CommandDMA && version->version_minor < 4 ) {
	    xf86DrvMsg( pScreen->myNum, X_WARNING,
			"[drm] DRM version < 2.4.0 does not support command DMA.\n");
	    psav->CommandDMA = FALSE;
	 }
	 if ( !psav->VertexDMA && version->version_minor < 4 ) {
	    xf86DrvMsg( pScreen->myNum, X_ERROR,
			"[drm] DRM version < 2.4.0 requires vertex DMA.\n");
	    drmFreeVersion( version );
	    SAVAGEDRICloseScreen( pScreen );
	    return FALSE;
	 }
         drmFreeVersion( version );
      }
   }

   if ( !psav->IsPCI && !SAVAGEDRIAgpInit( pScreen ) ) {
       if (pSAVAGEDRIServer->agp.handle != 0) {
	   /* AGP initialization succeeded, but adding AGP mappings failed. */
	   SAVAGEDRICloseScreen( pScreen );
	   return FALSE;
       }
       /* AGP initialization failed, fall back to PCI mode. */
       psav->IsPCI = TRUE;
       psav->AgpDMA = FALSE;
       xf86DrvMsg( pScrn->scrnIndex, X_WARNING,
		   "[agp] AGP failed to initialize -- falling back to PCI mode.\n");
       xf86DrvMsg( pScrn->scrnIndex, X_WARNING,
		   "[agp] Make sure you have the agpgart kernel module loaded.\n");
   }

   if ( !SAVAGEDRIMapInit( pScreen ) ) {
      SAVAGEDRICloseScreen( pScreen );
      return FALSE;
   }

   /* Linux kernel DRM broken in 2.6.30 through 2.6.39 */
   if (pDRIInfo->hFrameBuffer == pSAVAGEDRIServer->aperture.handle)
       xf86DrvMsg( pScrn->scrnIndex, X_WARNING,
		   "[drm] Detected broken drm maps. Please upgrade to linux kernel 3.x\n");

   if ( !SAVAGEDRIBuffersInit( pScreen ) ) {
      SAVAGEDRICloseScreen( pScreen );
      return FALSE;
   }

   if ( !SAVAGEInitVisualConfigs( pScreen ) ) {
      SAVAGEDRICloseScreen( pScreen );
      return FALSE;
   }
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[dri] visual configs initialized\n" );

   return TRUE;
}

static void SAVAGEDRISetupTiledSurfaceRegs( SavagePtr psav )
{
      SAVAGEDRIPtr pSAVAGEDRI = (SAVAGEDRIPtr)psav->pDRIInfo->devPrivate;
      unsigned int value = 0;
      
      OUTREG(0x850C,(INREG(0x850C) | 0x00008000)); /* AGD: I don't think this does anything on 3D/MX/IX */
						   /* maybe savage4 too... */
      /* we don't use Y range flag,so comment it */
      /*
        if(pSAVAGEDRI->width <= 1024)
            value |= (1<<29);
      */
      if ((psav->Chipset == S3_SAVAGE_MX) /* 3D/MX/IX seem to set up the tile stride differently */
	|| (psav->Chipset == S3_SAVAGE3D)) {
      	    if(pSAVAGEDRI->cpp == 2)
      	    {
         	value |=  ((psav->lDelta / 4) >> 5) << 24;
         	value |= 2<<30;
      	    } else {
         	value |=  ((psav->lDelta / 4) >> 5) << 24;
         	value |= 3<<30;
      	    }

	    OUTREG(TILED_SURFACE_REGISTER_0, value|(pSAVAGEDRI->frontOffset) ); /* front */ 
	    OUTREG(TILED_SURFACE_REGISTER_1, value|(pSAVAGEDRI->backOffset) ); /* back  */
	    OUTREG(TILED_SURFACE_REGISTER_2, value|(pSAVAGEDRI->depthOffset) ); /* depth */
      } else {
	    int offset_shift = 5;
      	    if(pSAVAGEDRI->cpp == 2)
      	    {
         	value |=  (((pSAVAGEDRI->width + 0x3F) & 0xFFC0) >> 6) << 20;
         	value |= 2<<30;
      	    } else {
         	value |=  (((pSAVAGEDRI->width + 0x1F) & 0xFFE0) >> 5) << 20;
         	value |= 3<<30;
      	    }
	    if (psav->Chipset == S3_SUPERSAVAGE) /* supersavages have a different shift */
		offset_shift = 6;
	    OUTREG(TILED_SURFACE_REGISTER_0, value|(pSAVAGEDRI->frontOffset >> offset_shift) ); /* front */
	    OUTREG(TILED_SURFACE_REGISTER_1, value|(pSAVAGEDRI->backOffset >> offset_shift) ); /* back  */
	    OUTREG(TILED_SURFACE_REGISTER_2, value|(pSAVAGEDRI->depthOffset >> offset_shift) ); /* depth */
      }
}

Bool SAVAGEDRIFinishScreenInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   SavagePtr psav = SAVPTR(pScrn);
   SAVAGEDRIServerPrivatePtr pSAVAGEDRIServer = psav->DRIServerInfo;
   SAVAGEDRIPtr pSAVAGEDRI = (SAVAGEDRIPtr)psav->pDRIInfo->devPrivate;
   int i;
   
   if ( !psav->pDRIInfo )
      return FALSE;

   psav->pDRIInfo->driverSwapMethod = DRI_KERNEL_SWAP;

   /* NOTE: DRIFinishScreenInit must be called before *DRIKernelInit
    * because *DRIKernelInit requires that the hardware lock is held by
    * the X server, and the first time the hardware lock is grabbed is
    * in DRIFinishScreenInit.
    */
   if ( !DRIFinishScreenInit( pScreen ) ) {
      SAVAGEDRICloseScreen( pScreen );
      return FALSE;
   }
   psav->LockHeld = 1;

   if ( !SAVAGEDRIKernelInit( pScreen ) ) {
      SAVAGEDRICloseScreen( pScreen );
      return FALSE;
   }

   pSAVAGEDRI->chipset          = psav->Chipset;
   pSAVAGEDRI->width		= pScrn->virtualX;
   pSAVAGEDRI->height		= pScrn->virtualY;
   pSAVAGEDRI->mem		= pScrn->videoRam * 1024;
   pSAVAGEDRI->cpp		= pScrn->bitsPerPixel / 8;
   pSAVAGEDRI->zpp		= pSAVAGEDRI->cpp;

   pSAVAGEDRI->agpMode		= psav->IsPCI ? 0 : psav->agpMode;

   pSAVAGEDRI->bufferSize       = SAVAGE_BUFFER_SIZE;

   pSAVAGEDRI->frontOffset		= pSAVAGEDRIServer->frontOffset;
   pSAVAGEDRI->frontbufferSize		= pSAVAGEDRIServer->frontbufferSize;

   pSAVAGEDRI->backOffset		= pSAVAGEDRIServer->backOffset;
   pSAVAGEDRI->backbufferSize		= pSAVAGEDRIServer->backbufferSize;

   pSAVAGEDRI->depthOffset		= pSAVAGEDRIServer->depthOffset;
   pSAVAGEDRI->depthbufferSize		= pSAVAGEDRIServer->depthbufferSize;

   pSAVAGEDRI->textureOffset	= pSAVAGEDRIServer->textureOffset;

   i = mylog2( pSAVAGEDRIServer->textureSize / SAVAGE_NR_TEX_REGIONS );
   if ( i < SAVAGE_LOG_MIN_TEX_REGION_SIZE )
      i = SAVAGE_LOG_MIN_TEX_REGION_SIZE;

   pSAVAGEDRI->logTextureGranularity = i;
   pSAVAGEDRI->textureSize = (pSAVAGEDRIServer->textureSize >> i) << i; /* truncate */

   pSAVAGEDRI->agpTextureHandle = pSAVAGEDRIServer->agpTextures.handle;

   i = mylog2( pSAVAGEDRIServer->agpTextures.size / SAVAGE_NR_TEX_REGIONS );
   if ( i < SAVAGE_LOG_MIN_TEX_REGION_SIZE )
      i = SAVAGE_LOG_MIN_TEX_REGION_SIZE;

   pSAVAGEDRI->logAgpTextureGranularity = i;
   pSAVAGEDRI->agpTextureSize = (pSAVAGEDRIServer->agpTextures.size >> i) << i; /* truncate */

   pSAVAGEDRI->apertureHandle	= pSAVAGEDRIServer->aperture.handle;
   pSAVAGEDRI->apertureSize	= pSAVAGEDRIServer->aperture.size;
   pSAVAGEDRI->aperturePitch    = psav->ulAperturePitch;
   
   pSAVAGEDRI->statusHandle	= pSAVAGEDRIServer->status.handle;
   pSAVAGEDRI->statusSize	= pSAVAGEDRIServer->status.size;

   pSAVAGEDRI->sarea_priv_offset = sizeof(XF86DRISAREARec);
   
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]pSAVAGEDRIServer:\n" );
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	reserved_map_agpstart:0x%08x\n",pSAVAGEDRIServer->reserved_map_agpstart); 
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	reserved_map_idx:0x%08x\n",pSAVAGEDRIServer->reserved_map_idx);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	sarea_priv_offset:0x%08x\n",pSAVAGEDRIServer->sarea_priv_offset);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	chipset:0x%08x\n",pSAVAGEDRIServer->chipset);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	sgram:0x%08x\n",pSAVAGEDRIServer->sgram);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	frontbufferSize:0x%08x\n",pSAVAGEDRIServer->frontbufferSize);  
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	frontOffset:0x%08x\n",pSAVAGEDRIServer->frontOffset);  
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	frontPitch:0x%08x\n",pSAVAGEDRIServer->frontPitch);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	backbufferSize:0x%08x\n",pSAVAGEDRIServer->backbufferSize);  
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	backOffset:0x%08x\n",pSAVAGEDRIServer->backOffset);  
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	backPitch:0x%08x\n",pSAVAGEDRIServer->backPitch);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	depthbufferSize:0x%08x\n",pSAVAGEDRIServer->depthbufferSize);  
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	depthOffset:0x%08x\n",pSAVAGEDRIServer->depthOffset);  
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	depthPitch:0x%08x\n",pSAVAGEDRIServer->depthPitch);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	textureOffset:0x%08x\n",pSAVAGEDRIServer->textureOffset);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	textureSize:0x%08x\n",pSAVAGEDRIServer->textureSize);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	textureSize:0x%08x\n",pSAVAGEDRIServer->textureSize);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	logTextureGranularity:0x%08x\n",pSAVAGEDRIServer->logTextureGranularity);

   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	agp:handle:0x%08lx\n",(unsigned long)pSAVAGEDRIServer->agp.handle);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	agp:offset:0x%08x\n",pSAVAGEDRIServer->agp.offset);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	agp:size:0x%08x\n",pSAVAGEDRIServer->agp.size);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	agp:map:0x%08lx\n",(unsigned long)pSAVAGEDRIServer->agp.map);
   
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	registers:handle:0x%08lx\n",(unsigned long)pSAVAGEDRIServer->registers.handle);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	registers:offset:0x%08x\n",pSAVAGEDRIServer->registers.offset);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	registers:size:0x%08x\n",pSAVAGEDRIServer->registers.size);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	registers:map:0x%08lx\n",(unsigned long)pSAVAGEDRIServer->registers.map);
   
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	status:handle:0x%08lx\n",(unsigned long)pSAVAGEDRIServer->status.handle);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	status:offset:0x%08x\n",pSAVAGEDRIServer->status.offset);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	status:size:0x%08x\n",pSAVAGEDRIServer->status.size);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	status:map:0x%08lx\n",(unsigned long)pSAVAGEDRIServer->status.map);
   
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	agpTextures:handle:0x%08lx\n",(unsigned long)pSAVAGEDRIServer->agpTextures.handle);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	agpTextures:offset:0x%08x\n",pSAVAGEDRIServer->agpTextures.offset);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	agpTextures:size:0x%08x\n",pSAVAGEDRIServer->agpTextures.size);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	apgTextures:map:0x%08lx\n",(unsigned long)pSAVAGEDRIServer->agpTextures.map);

   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	logAgpTextureGranularity:0x%08x\n",pSAVAGEDRIServer->logAgpTextureGranularity); 

   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	cmdDma:handle:0x%08lx\n",(unsigned long)pSAVAGEDRIServer->cmdDma.handle);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	cmdDma:offset:0x%08x\n",pSAVAGEDRIServer->cmdDma.offset);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	cmdDma:size:0x%08x\n",pSAVAGEDRIServer->cmdDma.size);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	cmdDma:map:0x%08lx\n",(unsigned long)pSAVAGEDRIServer->cmdDma.map);
   
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]pSAVAGEDRI:\n" );
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	chipset:0x%08x\n",pSAVAGEDRI->chipset );
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	width:0x%08x\n",pSAVAGEDRI->width );
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	height:0x%08x\n",pSAVAGEDRI->height );
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	mem:0x%08x\n",pSAVAGEDRI->mem );
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	cpp:%d\n",pSAVAGEDRI->cpp );
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	zpp:%d\n",pSAVAGEDRI->zpp );

   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	agpMode:%d\n",pSAVAGEDRI->agpMode );

   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	bufferSize:%u\n",pSAVAGEDRI->bufferSize );
   
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	frontbufferSize:0x%08x\n",pSAVAGEDRI->frontbufferSize);  
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	frontOffset:0x%08x\n",pSAVAGEDRI->frontOffset );

   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	backbufferSize:0x%08x\n",pSAVAGEDRI->backbufferSize);     
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	backOffset:0x%08x\n",pSAVAGEDRI->backOffset );
   
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	depthbufferSize:0x%08x\n",pSAVAGEDRI->depthbufferSize);  
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	depthOffset:0x%08x\n",pSAVAGEDRI->depthOffset );

   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	textureOffset:0x%08x\n",pSAVAGEDRI->textureOffset );
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	textureSize:0x%08x\n",pSAVAGEDRI->textureSize );
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	logTextureGranularity:0x%08x\n",pSAVAGEDRI->logTextureGranularity );
   
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	agpTextureHandle:0x%08lx\n",(unsigned long)pSAVAGEDRI->agpTextureHandle );
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	agpTextureSize:0x%08x\n",pSAVAGEDRI->agpTextureSize );
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	logAgpTextureGranularity:0x%08x\n",pSAVAGEDRI->logAgpTextureGranularity );

   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	apertureHandle:0x%08lx\n",(unsigned long)pSAVAGEDRI->apertureHandle);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	apertureSize:0x%08x\n",pSAVAGEDRI->apertureSize);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	aperturePitch:0x%08x\n",pSAVAGEDRI->aperturePitch);

   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	statusHandle:0x%08lx\n",(unsigned long)pSAVAGEDRI->statusHandle);
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	statusSize:0x%08x\n",pSAVAGEDRI->statusSize);

   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[junkers]	sarea_priv_offset:0x%08x\n",pSAVAGEDRI->sarea_priv_offset);
    
   SAVAGEDRISetupTiledSurfaceRegs( psav );
   return TRUE;
}

void SAVAGEDRIResume(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   SavagePtr psav = SAVPTR(pScrn);
   SAVAGESAREAPrivPtr pSAREAPriv =
			(SAVAGESAREAPrivPtr)DRIGetSAREAPrivate(pScreen);

   if (!psav->IsPCI) {
	SAVAGESetAgpMode(psav, pScreen);
   }
   SAVAGEDRISetupTiledSurfaceRegs(psav);
   /* Assume that 3D state was clobbered, invalidate it by
    * changing ctxOwner in the sarea. */
   pSAREAPriv->ctxOwner = DRIGetContext(pScreen);
}

void SAVAGEDRICloseScreen( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   SavagePtr psav = SAVPTR(pScrn);
   SAVAGEDRIServerPrivatePtr pSAVAGEDRIServer = psav->DRIServerInfo;

   if ( pSAVAGEDRIServer->status.map ) {
      drmUnmap( pSAVAGEDRIServer->status.map, pSAVAGEDRIServer->status.size );
      pSAVAGEDRIServer->status.map = NULL;
   }

   if ( pSAVAGEDRIServer->registers.map ) {
      drmUnmap( pSAVAGEDRIServer->registers.map, pSAVAGEDRIServer->registers.size );
      pSAVAGEDRIServer->registers.map = NULL;
   }

   if ( pSAVAGEDRIServer->aperture.map ) {
      drmUnmap( pSAVAGEDRIServer->aperture.map, pSAVAGEDRIServer->aperture.size );
      pSAVAGEDRIServer->aperture.map = NULL;
   }

   if ( pSAVAGEDRIServer->agpXVideo.map ) {
      drmUnmap( pSAVAGEDRIServer->agpXVideo.map, 
                pSAVAGEDRIServer->agpXVideo.size );
      pSAVAGEDRIServer->agpXVideo.map = NULL;
   }

   if ( pSAVAGEDRIServer->agpTextures.map ) {
      drmUnmap( pSAVAGEDRIServer->agpTextures.map, 
                pSAVAGEDRIServer->agpTextures.size );
      pSAVAGEDRIServer->agpTextures.map = NULL;
   }

   if (pSAVAGEDRIServer->status.handle)
       drmRmMap(psav->drmFD,pSAVAGEDRIServer->status.handle);

   if (pSAVAGEDRIServer->registers.handle)
       drmRmMap(psav->drmFD,pSAVAGEDRIServer->registers.handle);

   if (pSAVAGEDRIServer->aperture.handle)
       drmRmMap(psav->drmFD,pSAVAGEDRIServer->registers.handle);

   if (pSAVAGEDRIServer->agpXVideo.handle)
       drmRmMap(psav->drmFD,pSAVAGEDRIServer->agpXVideo.handle);

   if (pSAVAGEDRIServer->agpTextures.handle)
       drmRmMap(psav->drmFD,pSAVAGEDRIServer->agpTextures.handle);

   if (pSAVAGEDRIServer->cmdDma.handle)
       drmRmMap(psav->drmFD,pSAVAGEDRIServer->cmdDma.handle);

   if ( pSAVAGEDRIServer->buffers.map ) {
      drmUnmap( pSAVAGEDRIServer->buffers.map, pSAVAGEDRIServer->buffers.size );
      pSAVAGEDRIServer->buffers.map = NULL;
   }

   if ( pSAVAGEDRIServer->agp.handle ) {
      drmAgpUnbind( psav->drmFD, pSAVAGEDRIServer->agp.handle );
      drmAgpFree( psav->drmFD, pSAVAGEDRIServer->agp.handle );
      pSAVAGEDRIServer->agp.handle = 0;
      drmAgpRelease( psav->drmFD );
   }

   DRICloseScreen( pScreen );
   
   /* Don't use shadow status any more. If this happens due to failed
    * DRI initialization then SavageScreenInit will do the real
    * cleanup and restore ShadowStatus to sane settings. */
   psav->ShadowVirtual = NULL;
   psav->ShadowPhysical = 0;

   if(psav->reserved)
      xf86FreeOffscreenLinear(psav->reserved);

   if ( psav->pDRIInfo ) {
      if ( psav->pDRIInfo->devPrivate ) {
	 free( psav->pDRIInfo->devPrivate );
	 psav->pDRIInfo->devPrivate = 0;
      }
      DRIDestroyInfoRec( psav->pDRIInfo );
      psav->pDRIInfo = 0;
   }
   if ( psav->DRIServerInfo ) {
      free( psav->DRIServerInfo );
      psav->DRIServerInfo = 0;
   }
   if ( psav->pVisualConfigs ) {
      free( psav->pVisualConfigs );
   }
   if ( psav->pVisualConfigsPriv ) {
      free( psav->pVisualConfigsPriv );
   }
}

void
SAVAGEDRIInitBuffers(WindowPtr pWin, RegionPtr prgn, CARD32 index)
{
    ScreenPtr pScreen = pWin->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    SavagePtr psav = SAVPTR(pScrn);
    BoxPtr pbox = REGION_RECTS(prgn);
    int nbox  = REGION_NUM_RECTS(prgn);
    drmSAVAGECmdHeader cmd[2];
    drmSAVAGECmdbuf cmdBuf;
    int ret;

    if (!psav->LockHeld) {
	xf86DrvMsg( pScrn->scrnIndex, X_WARNING,
		    "Not holding the lock in InitBuffers.\n");
	return;
    }

    cmd[0].clear0.cmd = SAVAGE_CMD_CLEAR;
    cmd[0].clear0.flags = SAVAGE_BACK|SAVAGE_DEPTH;
    cmd[1].clear1.mask = 0xffffffff;
    cmd[1].clear1.value = 0;

    cmdBuf.cmd_addr = cmd;
    cmdBuf.size = 2;
    cmdBuf.dma_idx = 0;
    cmdBuf.discard = 0;
    cmdBuf.vb_addr = NULL;
    cmdBuf.vb_size = 0;
    cmdBuf.vb_stride = 0;
    cmdBuf.box_addr = (drm_clip_rect_t*)pbox;
    cmdBuf.nbox = nbox;

    ret = drmCommandWrite(psav->drmFD, DRM_SAVAGE_BCI_CMDBUF,
			  &cmdBuf, sizeof(cmdBuf));
    if (ret < 0) {
	xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		    "SAVAGEDRIInitBuffers: drmCommandWrite returned %d.\n",
		    ret);
    }
}

/*
  This routine is a modified form of XAADoBitBlt with the calls to
  ScreenToScreenBitBlt built in. My routine has the prgnSrc as source
  instead of destination. My origin is upside down so the ydir cases
  are reversed.
*/

void
SAVAGEDRIMoveBuffers(WindowPtr pParent, DDXPointRec ptOldOrg,
		   RegionPtr prgnSrc, CARD32 index)
{
    ScreenPtr pScreen = pParent->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    SavagePtr psav = SAVPTR(pScrn);
    int nbox;
    BoxPtr pbox, pboxTmp, pboxNext, pboxBase, pboxNew1, pboxNew2;
    DDXPointPtr pptTmp, pptNew1, pptNew2;
    int xdir, ydir;
    int dx, dy;
    DDXPointPtr pptSrc;
    int screenwidth = pScrn->virtualX;
    int screenheight = pScrn->virtualY;
    BCI_GET_PTR;

    if (!psav->LockHeld) {
	xf86DrvMsg( pScrn->scrnIndex, X_INFO, "Not holding lock in MoveBuffers\n");
    }

    pbox = REGION_RECTS(prgnSrc);
    nbox = REGION_NUM_RECTS(prgnSrc);
    pboxNew1 = 0;
    pptNew1 = 0;
    pboxNew2 = 0;
    pptNew2 = 0;
    pptSrc = &ptOldOrg;

    dx = pParent->drawable.x - ptOldOrg.x;
    dy = pParent->drawable.y - ptOldOrg.y;

    /* If the copy will overlap in Y, reverse the order */
    if (dy>0) {
        ydir = -1;

        if (nbox>1) {
	    /* Keep ordering in each band, reverse order of bands */
	    pboxNew1 = malloc(sizeof(BoxRec)*nbox);
	    if (!pboxNew1) return;
	    pptNew1 = malloc(sizeof(DDXPointRec)*nbox);
	    if (!pptNew1) {
	        free(pboxNew1);
	        return;
	    }
	    pboxBase = pboxNext = pbox+nbox-1;
	    while (pboxBase >= pbox) {
	        while ((pboxNext >= pbox) && (pboxBase->y1 == pboxNext->y1))
		  pboxNext--;
	        pboxTmp = pboxNext+1;
	        pptTmp = pptSrc + (pboxTmp - pbox);
	        while (pboxTmp <= pboxBase) {
		    *pboxNew1++ = *pboxTmp++;
		    *pptNew1++ = *pptTmp++;
		}
	        pboxBase = pboxNext;
	    }
	    pboxNew1 -= nbox;
	    pbox = pboxNew1;
	    pptNew1 -= nbox;
	    pptSrc = pptNew1;
	}
    } else {
        /* No changes required */
        ydir = 1;
    }

    /* If the regions will overlap in X, reverse the order */
    if (dx>0) {
        xdir = -1;

        if (nbox > 1) {
	    /*reverse orderof rects in each band */
	    pboxNew2 = malloc(sizeof(BoxRec)*nbox);
	    pptNew2 = malloc(sizeof(DDXPointRec)*nbox);
	    if (!pboxNew2 || !pptNew2) {
	        if (pptNew2) free(pptNew2);
	        if (pboxNew2) free(pboxNew2);
	        if (pboxNew1) {
		    free(pptNew1);
		    free(pboxNew1);
		}
	       return;
	    }
	    pboxBase = pboxNext = pbox;
	    while (pboxBase < pbox+nbox) {
	        while ((pboxNext < pbox+nbox) &&
		       (pboxNext->y1 == pboxBase->y1))
		  pboxNext++;
	        pboxTmp = pboxNext;
	        pptTmp = pptSrc + (pboxTmp - pbox);
	        while (pboxTmp != pboxBase) {
		    *pboxNew2++ = *--pboxTmp;
		    *pptNew2++ = *--pptTmp;
		}
	        pboxBase = pboxNext;
	    }
	    pboxNew2 -= nbox;
	    pbox = pboxNew2;
	    pptNew2 -= nbox;
	    pptSrc = pptNew2;
	}
    } else {
        /* No changes are needed */
        xdir = 1;
    }

    BCI_SEND(0xc0030000); /* wait for 2D+3D idle */
    SAVAGEDRISetupForScreenToScreenCopy(pScrn, xdir, ydir, GXcopy, -1, -1);
    for ( ; nbox-- ; pbox++)
     {
	 int x1 = pbox->x1;
	 int y1 = pbox->y1;
	 int destx = x1 + dx;
	 int desty = y1 + dy;
	 int w = pbox->x2 - x1 + 1;
	 int h = pbox->y2 - y1 + 1;

	 if ( destx < 0 ) x1 -= destx, w += destx, destx = 0;
	 if ( desty < 0 ) y1 -= desty, h += desty, desty = 0;
	 if ( destx + w > screenwidth ) w = screenwidth - destx;
	 if ( desty + h > screenheight ) h = screenheight - desty;
	 if ( w <= 0 ) continue;
	 if ( h <= 0 ) continue;

	 SAVAGESelectBuffer(pScrn, SAVAGE_BACK);
	 SAVAGEDRISubsequentScreenToScreenCopy(pScrn, x1, y1,
					       destx,desty, w, h);
	 SAVAGESelectBuffer(pScrn, SAVAGE_DEPTH);
	 SAVAGEDRISubsequentScreenToScreenCopy(pScrn, x1,y1,
					       destx,desty, w, h);
     }
    SAVAGESelectBuffer(pScrn, SAVAGE_FRONT);

    if (pboxNew2) {
        free(pptNew2);
        free(pboxNew2);
    }
    if (pboxNew1) {
        free(pptNew1);
        free(pboxNew1);
    }

    BCI_SEND(0xc0020000); /* wait for 2D idle */
    if (psav->useEXA)
	exaMarkSync(pScreen);
#ifdef HAVE_XAA_H
    else
	psav->AccelInfoRec->NeedToSync = TRUE;
#endif
}

/* Definition in savage_accel.c */
int SavageGetCopyROP(int rop);

static void 
SAVAGEDRISetupForScreenToScreenCopy(
    ScrnInfoPtr pScrn,
    int xdir, 
    int ydir,
    int rop,
    unsigned planemask,
    int transparency_color)
{
    SavagePtr psav = SAVPTR(pScrn);
    int cmd =0;

    cmd = BCI_CMD_RECT | BCI_CMD_DEST_PBD | BCI_CMD_SRC_PBD_COLOR;
    BCI_CMD_SET_ROP( cmd, SavageGetCopyROP(rop) );
    if (transparency_color != -1)
        cmd |= BCI_CMD_SEND_COLOR | BCI_CMD_SRC_TRANSPARENT;

    if (xdir == 1 ) cmd |= BCI_CMD_RECT_XP;
    if (ydir == 1 ) cmd |= BCI_CMD_RECT_YP;

    psav->SavedBciCmd = cmd;
    psav->SavedBgColor = transparency_color;

}

static void 
SAVAGEDRISubsequentScreenToScreenCopy(
    ScrnInfoPtr pScrn,
    int x1,
    int y1,
    int x2,
    int y2,
    int w,
    int h)
{
    SavagePtr psav = SAVPTR(pScrn);
    BCI_GET_PTR;

    if (!w || !h) return;
    if (!(psav->SavedBciCmd & BCI_CMD_RECT_XP)) {
        w --;
        x1 += w;
        x2 += w;
        w ++;
    }
    if (!(psav->SavedBciCmd & BCI_CMD_RECT_YP)) {
        h --;
        y1 += h;
        y2 += h;
        h ++;
    }

    psav->WaitQueue(psav,6);
    BCI_SEND(psav->SavedBciCmd);
    if (psav->SavedBgColor != -1) 
	BCI_SEND(psav->SavedBgColor);
    BCI_SEND(BCI_X_Y(x1, y1));
    BCI_SEND(BCI_X_Y(x2, y2));
    BCI_SEND(BCI_W_H(w, h));

}

/*
 * the FullScreen DRI code is dead, this is just left in place to show how
 * to set up pageflipping.
 */
static Bool
SAVAGEDRIOpenFullScreen(ScreenPtr pScreen)
{
  ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
  vgaHWPtr hwp = VGAHWPTR(pScrn);
  SavagePtr psav = SAVPTR(pScrn);
  unsigned int vgaCRIndex = hwp->IOBase + 4;
  unsigned int vgaCRReg = hwp->IOBase + 5;
  SAVAGEDRIPtr pSAVAGEDRI = (SAVAGEDRIPtr)psav->pDRIInfo->devPrivate;
  unsigned int TileStride;
  unsigned int WidthinTiles;
  unsigned int depth;
  
  OUTREG(0x48C18, INREG(0x48C18) & 0xFFFFFFF7);
  /*VGAOUT8(vgaCRIndex,0x66);
  VGAOUT8(vgaCRReg, VGAIN8(vgaCRReg)|0x10);*/
  VGAOUT8(vgaCRIndex,0x69);
  VGAOUT8(vgaCRReg, 0x80);
  
  depth = pScrn->bitsPerPixel;
  
  if(depth == 16)
  {
      WidthinTiles = (pSAVAGEDRI->width+63)>>6;
      TileStride = (pSAVAGEDRI->width+63)&(~63);
  
  }
  else
  {
      WidthinTiles = (pSAVAGEDRI->width+31)>>5;
      TileStride = (pSAVAGEDRI->width+31)&(~31);
  
  }  


  /* set primary stream stride */
  {
      unsigned int value;
      
      /*value = 0x80000000|(WidthinTiles<<24)|(TileStride*depth/8);*/
      value = 0x80000000|(WidthinTiles<<24);
      if(depth == 32)
          value |= 0x40000000;
      
      OUTREG(PRI_STREAM_STRIDE, value);
  
  }
  
  /* set global bitmap descriptor */
  {
      unsigned int value;
      value = 0x10000000|
              0x00000009|
              0x01000000|
              (depth<<16)  | TileStride;
  
      OUTREG(0x816C,value);
  
  }
 
   OUTREG(0x48C18, INREG(0x48C18) | 0x8);
   
  return TRUE;
}

static Bool
SAVAGEDRICloseFullScreen(ScreenPtr pScreen)
{
  ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
  SavagePtr psav = SAVPTR(pScrn);
  BCI_GET_PTR;

  BCI_SEND(0xC0FF0000);
  psav->WaitIdleEmpty(psav);
  OUTREG(0x48C18, INREG(0x48C18) & 0xFFFFFFF7);
  /* set primary stream stride */
  {
      /* MM81C0 and 81C4 are used to control primary stream. */
      OUTREG32(PRI_STREAM_FBUF_ADDR0,0x00000000);
      OUTREG32(PRI_STREAM_FBUF_ADDR1,0x00000000);
      
      /* FIFO control */
      OUTREG32(0X81EC,0Xffffffff);
      
      if (!psav->bTiled) {
          OUTREG32(PRI_STREAM_STRIDE,
                   (((psav->lDelta * 2) << 16) & 0x3FFFE000) |
                   (psav->lDelta & 0x00001fff));
      }
      else if (pScrn->bitsPerPixel == 16) {
          /* Scanline-length-in-bytes/128-bytes-per-tile * 256 Qwords/tile */
          OUTREG32(PRI_STREAM_STRIDE,
                   (((psav->lDelta * 2) << 16) & 0x3FFFE000)
                   | 0x80000000 | (psav->lDelta & 0x00001fff));
      }
      else if (pScrn->bitsPerPixel == 32) {
          OUTREG32(PRI_STREAM_STRIDE,
                   (((psav->lDelta * 2) << 16) & 0x3FFFE000)
                   | 0xC0000000 | (psav->lDelta & 0x00001fff));
      }
      
      
  }
  
  /* set global bitmap descriptor */
      {
          OUTREG32(S3_GLB_BD_LOW,psav->GlobalBD.bd2.LoPart );
          OUTREG32(S3_GLB_BD_HIGH,psav->GlobalBD.bd2.HiPart | BCI_ENABLE | S3_LITTLE_ENDIAN | S3_BD64);
          
      }
  
  OUTREG(0x48C18, INREG(0x48C18) | 0x8);
  return TRUE;
}

#endif
