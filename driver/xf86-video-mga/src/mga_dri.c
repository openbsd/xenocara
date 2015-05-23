/*
 * Copyright 2000 VA Linux Systems Inc., Fremont, California.
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
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Keith Whitwell <keith@tungstengraphics.com>
 *    Gareth Hughes <gareth@valinux.com>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"

#include "xf86Pci.h"

#include "miline.h"

#include <errno.h>

#include <inttypes.h>
#include "mga_reg.h"
#include "mga.h"
#include "mga_macros.h"
#include "mga_dri.h"
#include "mga_sarea.h"
#include "mga_drm.h"

#define _XF86DRI_SERVER_
#include "GL/glxtokens.h"
#include "sarea.h"





#include "GL/glxtokens.h"

#include "mga_reg.h"
#include "mga.h"
#include "mga_macros.h"
#include "mga_dri.h"

#define  DRM_MGA_IDLE_RETRY          2048

static char MGAKernelDriverName[] = "mga";
static char MGAClientDriverName[] = "mga";

/* Initialize the visual configs that are supported by the hardware.
 * These are combined with the visual configs that the indirect
 * rendering core supports, and the intersection is exported to the
 * client.
 */
static Bool MGAInitVisualConfigs( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   MGAPtr pMga = MGAPTR(pScrn);
   int numConfigs = 0;
   __GLXvisualConfig *pConfigs = 0;
   MGAConfigPrivPtr pMGAConfigs = 0;
   MGAConfigPrivPtr *pMGAConfigPtrs = 0;
   int i, db, depth, stencil, accum;

   switch ( pScrn->bitsPerPixel ) {
   case 8:
   case 24:
      break;

   case 16:
      numConfigs = 8;

      pConfigs = (__GLXvisualConfig*)calloc( sizeof(__GLXvisualConfig),
						numConfigs );
      if ( !pConfigs ) {
	 return FALSE;
      }

      pMGAConfigs = (MGAConfigPrivPtr)calloc( sizeof(MGAConfigPrivRec),
						 numConfigs );
      if ( !pMGAConfigs ) {
	 free(pConfigs);
	 return FALSE;
      }

      pMGAConfigPtrs = (MGAConfigPrivPtr*)calloc( sizeof(MGAConfigPrivPtr),
						     numConfigs );
      if ( !pMGAConfigPtrs ) {
	 free(pConfigs);
	 free(pMGAConfigs);
	 return FALSE;
      }

      for ( i = 0 ; i < numConfigs ; i++ ) {
	 pMGAConfigPtrs[i] = &pMGAConfigs[i];
      }

      i = 0;
      for ( accum = 0 ; accum <= 1 ; accum++ ) {
         for ( stencil = 0 ; stencil <= 1 ; stencil++ ) {
            for ( db = 1 ; db >= 0 ; db-- ) {
               pConfigs[i].vid			= -1;
               pConfigs[i].class		= -1;
               pConfigs[i].rgba			= TRUE;
               pConfigs[i].redSize		= 5;
               pConfigs[i].greenSize		= 6;
               pConfigs[i].blueSize		= 5;
               pConfigs[i].alphaSize		= 0;
               pConfigs[i].redMask		= 0x0000F800;
               pConfigs[i].greenMask		= 0x000007E0;
               pConfigs[i].blueMask		= 0x0000001F;
               pConfigs[i].alphaMask		= 0;
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
               pConfigs[i].bufferSize		= 16;
               pConfigs[i].depthSize		= 16;
               if ( stencil ) {
                  pConfigs[i].stencilSize	= 8;
               } else {
                  pConfigs[i].stencilSize	= 0;
	       }
               pConfigs[i].auxBuffers		= 0;
               pConfigs[i].level		= 0;
               if ( accum || stencil ) {
                  pConfigs[i].visualRating	= GLX_SLOW_CONFIG;
               } else {
                  pConfigs[i].visualRating	= GLX_NONE;
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

   case 32:
      numConfigs = 8;

      pConfigs = (__GLXvisualConfig*)calloc( sizeof(__GLXvisualConfig),
						numConfigs );
      if ( !pConfigs ) {
	 return FALSE;
      }

      pMGAConfigs = (MGAConfigPrivPtr)calloc( sizeof(MGAConfigPrivRec),
						 numConfigs );
      if ( !pMGAConfigs ) {
	 free(pConfigs);
	 return FALSE;
      }

      pMGAConfigPtrs = (MGAConfigPrivPtr*)calloc( sizeof(MGAConfigPrivPtr),
						     numConfigs );
      if ( !pMGAConfigPtrs ) {
	 free(pConfigs);
	 free(pMGAConfigs);
	 return FALSE;
      }

      for ( i = 0 ; i < numConfigs ; i++ ) {
	 pMGAConfigPtrs[i] = &pMGAConfigs[i];
      }

      i = 0;
      for ( accum = 0 ; accum <= 1 ; accum++ ) {
         for ( depth = 0 ; depth <= 1 ; depth++ ) { /* and stencil */
            for ( db = 1 ; db >= 0 ; db-- ) {
               pConfigs[i].vid			= -1;
               pConfigs[i].class		= -1;
               pConfigs[i].rgba			= TRUE;
               pConfigs[i].redSize		= 8;
               pConfigs[i].greenSize		= 8;
               pConfigs[i].blueSize		= 8;
               pConfigs[i].alphaSize		= 0;
               pConfigs[i].redMask		= 0x00FF0000;
               pConfigs[i].greenMask		= 0x0000FF00;
               pConfigs[i].blueMask		= 0x000000FF;
               pConfigs[i].alphaMask		= 0x0;
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
               pConfigs[i].bufferSize		= 24;
               if ( depth ) {
		     pConfigs[i].depthSize	= 24;
                     pConfigs[i].stencilSize	= 8;
               }
               else {
                     pConfigs[i].depthSize	= 0;
                     pConfigs[i].stencilSize	= 0;
               }
               pConfigs[i].auxBuffers		= 0;
               pConfigs[i].level		= 0;
               if ( accum ) {
                  pConfigs[i].visualRating	= GLX_SLOW_CONFIG;
               } else {
                  pConfigs[i].visualRating	= GLX_NONE;
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

   pMga->numVisualConfigs = numConfigs;
   pMga->pVisualConfigs = pConfigs;
   pMga->pVisualConfigsPriv = pMGAConfigs;

   GlxSetVisualConfigs( numConfigs, pConfigs, (void **)pMGAConfigPtrs );

   return TRUE;
}

static Bool MGACreateContext( ScreenPtr pScreen, VisualPtr visual,
			      drm_context_t hwContext, void *pVisualConfigPriv,
			      DRIContextType contextStore )
{
   /* Nothing yet */
   return TRUE;
}

static void MGADestroyContext( ScreenPtr pScreen, drm_context_t hwContext,
			       DRIContextType contextStore )
{
   /* Nothing yet */
}


/* Quiescence, locking
 */
#define MGA_TIMEOUT		2048

static void MGAWaitForIdleDMA( ScrnInfoPtr pScrn )
{
   MGAPtr pMga = MGAPTR(pScrn);
   drm_lock_t lock;
   int ret;
   int i = 0;

   memset( &lock, 0, sizeof(drm_lock_t) );

   for (;;) {
      do {
         /* first ask for quiescent and flush */
         lock.flags = DRM_LOCK_QUIESCENT | DRM_LOCK_FLUSH;
         do {
	    ret = drmCommandWrite( pMga->drmFD, DRM_MGA_FLUSH,
                                   &lock, sizeof( drm_lock_t ) );
         } while ( ret == -EBUSY && i++ < DRM_MGA_IDLE_RETRY );

         /* if it's still busy just try quiescent */
         if ( ret == -EBUSY ) { 
            lock.flags = DRM_LOCK_QUIESCENT;
            do {
	       ret = drmCommandWrite( pMga->drmFD, DRM_MGA_FLUSH,
                                      &lock, sizeof( drm_lock_t ) );
            } while ( ret == -EBUSY && i++ < DRM_MGA_IDLE_RETRY );
         }
      } while ( ( ret == -EBUSY ) && ( i++ < MGA_TIMEOUT ) );

      if ( ret == 0 )
	 return;

      xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		  "[dri] Idle timed out, resetting engine...\n" );

      drmCommandNone( pMga->drmFD, DRM_MGA_RESET );
   }
}


void MGAGetQuiescence( ScrnInfoPtr pScrn )
{
   MGAPtr pMga = MGAPTR(pScrn);

   pMga->haveQuiescense = 1;

   if ( pMga->directRenderingEnabled ) {
      MGAFBLayout *pLayout = &pMga->CurrentLayout;

      MGAWaitForIdleDMA( pScrn );

        /* FIXME what about EXA? */
#ifdef USE_XAA
        if (!pMga->Exa && pMga->AccelInfoRec) {
      WAITFIFO( 11 );
      OUTREG( MGAREG_MACCESS, pMga->MAccess );
      OUTREG( MGAREG_PITCH, pLayout->displayWidth );

      pMga->PlaneMask = ~0;
      OUTREG( MGAREG_PLNWT, pMga->PlaneMask );

      pMga->BgColor = 0;
      pMga->FgColor = 0;
      OUTREG( MGAREG_BCOL, pMga->BgColor );
      OUTREG( MGAREG_FCOL, pMga->FgColor );
      OUTREG( MGAREG_SRCORG, pMga->realSrcOrg );

      pMga->SrcOrg = 0;
      OUTREG( MGAREG_DSTORG, pMga->DstOrg );
      OUTREG( MGAREG_OPMODE, MGAOPM_DMA_BLIT );
      OUTREG( MGAREG_CXBNDRY, 0xFFFF0000 ); /* (maxX << 16) | minX */
      OUTREG( MGAREG_YTOP, 0x00000000 );    /* minPixelPointer */
      OUTREG( MGAREG_YBOT, 0x007FFFFF );    /* maxPixelPointer */

            pMga->AccelFlags &= ~CLIPPER_ON;
        }
#endif
   }
}

void MGAGetQuiescenceShared( ScrnInfoPtr pScrn )
{
   MGAPtr pMga = MGAPTR(pScrn);
   MGAEntPtr pMGAEnt = pMga->entityPrivate;
   MGAPtr pMGA2 = MGAPTR(pMGAEnt->pScrn_2);

   pMga = MGAPTR(pMGAEnt->pScrn_1);
   pMga->haveQuiescense = 1;
   pMGA2->haveQuiescense = 1;

   if ( pMGAEnt->directRenderingEnabled ) {
      MGAWaitForIdleDMA( pMGAEnt->pScrn_1 );

        /* FIXME what about EXA? */
#ifdef USE_XAA
        if (!pMga->Exa && pMga->AccelInfoRec)
            pMga->RestoreAccelState( pScrn );
#endif
      xf86SetLastScrnFlag( pScrn->entityList[0], pScrn->scrnIndex );
   }
}

static void MGASwapContext( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   MGAPtr pMga = MGAPTR(pScrn);

   /* Arrange for dma_quiescence and xaa sync to be called as
    * appropriate.
    */
   pMga->haveQuiescense = 0;

   MGA_MARK_SYNC(pMga, pScrn);
}

static void MGASwapContextShared( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   MGAPtr pMga = MGAPTR(pScrn);
   MGAEntPtr pMGAEnt = pMga->entityPrivate;
   MGAPtr pMGA2 = MGAPTR(pMGAEnt->pScrn_2);

   pMga = MGAPTR(pMGAEnt->pScrn_1);

   pMga->haveQuiescense = pMGA2->haveQuiescense = 0;

   MGA_MARK_SYNC(pMga, pScrn);
   MGA_MARK_SYNC(pMGA2, pMGAEnt->pScrn_2);
}

/* FIXME: This comment is out of date, since we aren't overriding
 * Block/Wakeuphandler anymore.
 *
 *
 * This is really only called from validate/postvalidate as we
 * override the dri lock/unlock.  Want to remove validate/postvalidate
 * processing, but need to remove all client-side use of drawable lock
 * first (otherwise there is noone recover when a client dies holding
 * the drawable lock).
 *
 * What does this mean?
 *
 *   - The above code gets executed every time a
 *     window changes shape or the focus changes, which isn't really
 *     optimal.
 *   - The X server therefore believes it needs to do an XAA sync
 *     *and* a dma quiescense ioctl each time that happens.
 *
 * We don't wrap wakeuphandler any longer, so at least we can say that
 * this doesn't happen *every time the mouse moves*...
 */
static void
MGADRISwapContext( ScreenPtr pScreen, DRISyncType syncType,
		   DRIContextType oldContextType, void *oldContext,
		   DRIContextType newContextType, void *newContext )
{
   if ( syncType == DRI_3D_SYNC &&
	oldContextType == DRI_2D_CONTEXT &&
	newContextType == DRI_2D_CONTEXT )
   {
      MGASwapContext( pScreen );
   }
}

static void
MGADRISwapContextShared( ScreenPtr pScreen, DRISyncType syncType,
			  DRIContextType oldContextType, void *oldContext,
			  DRIContextType newContextType, void *newContext )
{
   if ( syncType == DRI_3D_SYNC &&
	oldContextType == DRI_2D_CONTEXT &&
	newContextType == DRI_2D_CONTEXT )
   {
      MGASwapContextShared( pScreen );
   }
}

void MGASelectBuffer( ScrnInfoPtr pScrn, int which )
{
   MGAPtr pMga = MGAPTR(pScrn);
   MGADRIPtr pMGADRI = (MGADRIPtr)pMga->pDRIInfo->devPrivate;

   switch ( which ) {
   case MGA_BACK:
      OUTREG( MGAREG_DSTORG, pMGADRI->backOffset );
      OUTREG( MGAREG_SRCORG, pMGADRI->backOffset );
      break;
   case MGA_DEPTH:
      OUTREG( MGAREG_DSTORG, pMGADRI->depthOffset );
      OUTREG( MGAREG_SRCORG, pMGADRI->depthOffset );
      break;
   default:
   case MGA_FRONT:
      OUTREG( MGAREG_DSTORG, pMGADRI->frontOffset );
      OUTREG( MGAREG_SRCORG, pMGADRI->frontOffset );
      break;
   }
}

static unsigned int mylog2( unsigned int n )
{
   unsigned int log2 = 1;
   while ( n > 1 ) n >>= 1, log2++;
   return log2;
}

/**
 * Initialize DMA and secondary texture memory
 *
 * \todo
 * The sizes used for the primary DMA buffer and the bin size and count for
 * the secondary DMA buffers should be configurable from the xorg.conf.
 * 
 * \todo
 * This routine should use \c mga_bios_values::host_interface to limit the
 * AGP mode.  It the card is PCI, \c MGARec::agpSize should be forced to 0.
 */
static Bool MGADRIBootstrapDMA(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    MGAPtr pMga = MGAPTR(pScrn);
    MGADRIServerPrivatePtr pMGADRIServer = pMga->DRIServerInfo;
    int ret;
    int requested_agp_mode;
    int count;


    if(pMga->agpSize < 12)pMga->agpSize = 12;
    if(pMga->agpSize > 64)pMga->agpSize = 64; /* cap */


    requested_agp_mode = 0;
    switch ( pMga->agpMode ) {
    case 4:
	requested_agp_mode |= MGA_AGP_4X_MODE;
    case 2:
	requested_agp_mode |= MGA_AGP_2X_MODE;
    case 1:
    default:
	requested_agp_mode |= MGA_AGP_1X_MODE;
    }


    if ( (pMGADRIServer->drm_version_minor >= 2) && !pMga->useOldDmaInit ) {
	drm_mga_dma_bootstrap_t dma_bs;


	(void) memset( & dma_bs, 0, sizeof( dma_bs ) );
	dma_bs.primary_size = 1024 * 1024;
	dma_bs.secondary_bin_count = MGA_NUM_BUFFERS;
	dma_bs.secondary_bin_size = MGA_BUFFER_SIZE;
	dma_bs.agp_size = pMga->agpSize;
	dma_bs.agp_mode = (pMga->forcePciDma) ? 0 : requested_agp_mode;

	ret = drmCommandWriteRead( pMga->drmFD, DRM_MGA_DMA_BOOTSTRAP,
				   & dma_bs, sizeof( dma_bs ) );
	if ( ret ) {
	    xf86DrvMsg( pScreen->myNum, X_ERROR, "[drm] Could not boot-strap DMA (%d)\n", ret );
	    return FALSE;
	}

	pMga->agpMode = dma_bs.agp_mode;
	pMGADRIServer->agp.size = dma_bs.agp_size;

	pMGADRIServer->agpTextures.handle = dma_bs.texture_handle;
	pMGADRIServer->agpTextures.size   = dma_bs.texture_size;
    }
    else {
	unsigned long mode;
	unsigned int vendor, device;
	int i;


	if ( pMga->forcePciDma ) {
	    const char * const msg = (pMGADRIServer->drm_version_minor < 2)
	      ? "DRM version is too old (3.2 or later required)"
	      : "old DMA init path was requested";

	    xf86DrvMsg( pScreen->myNum, X_WARNING,
			"[agp] Cannot force PCI DMA because %s\n", msg );
	}

	if ( drmAgpAcquire( pMga->drmFD ) < 0 ) {
	    xf86DrvMsg( pScreen->myNum, X_ERROR, "[agp] AGP not available\n" );
	    return FALSE;
	}
       
	mode   = drmAgpGetMode( pMga->drmFD );        /* Default mode */
	vendor = drmAgpVendorId( pMga->drmFD );
	device = drmAgpDeviceId( pMga->drmFD );

	mode = (mode & ~MGA_AGP_MODE_MASK) | requested_agp_mode;

	xf86DrvMsg( pScreen->myNum, X_INFO,
		    "[agp] Mode 0x%08lx [AGP 0x%04x/0x%04x; Card 0x%04x/0x%04x]\n",
		    mode, vendor, device,
		    VENDOR_ID(pMga->PciInfo),
		    DEVICE_ID(pMga->PciInfo));

	if ( drmAgpEnable( pMga->drmFD, mode ) < 0 ) {
	    xf86DrvMsg( pScreen->myNum, X_ERROR, "[agp] AGP not enabled\n" );
	    drmAgpRelease( pMga->drmFD );
	    return FALSE;
	}

	if ( pMga->Chipset == PCI_CHIP_MGAG200 ) {
	    switch ( pMga->agpMode ) {
	    case 2:
		xf86DrvMsg( pScreen->myNum, X_INFO,
			    "[drm] Enabling AGP 2x PLL encoding\n" );
		OUTREG( MGAREG_AGP_PLL, MGA_AGP2XPLL_ENABLE );
		break;
		
	    case 1:
	    default:
		xf86DrvMsg( pScreen->myNum, X_INFO,
			    "[drm] Disabling AGP 2x PLL encoding\n" );
		OUTREG( MGAREG_AGP_PLL, MGA_AGP2XPLL_DISABLE );
		pMga->agpMode = 1;
		break;
	    }
	}

	pMGADRIServer->agp.size = pMga->agpSize * 1024 * 1024;

	pMGADRIServer->warp.offset = 0;
	pMGADRIServer->warp.size = MGA_WARP_UCODE_SIZE;

	pMGADRIServer->primary.offset = (pMGADRIServer->warp.offset +
					 pMGADRIServer->warp.size);
	pMGADRIServer->primary.size = 1024 * 1024;

	pMGADRIServer->buffers.offset = (pMGADRIServer->primary.offset +
					 pMGADRIServer->primary.size);
	pMGADRIServer->buffers.size = MGA_NUM_BUFFERS * MGA_BUFFER_SIZE;


	pMGADRIServer->agpTextures.offset = (pMGADRIServer->buffers.offset +
					     pMGADRIServer->buffers.size);

	pMGADRIServer->agpTextures.size = (pMGADRIServer->agp.size -
					   pMGADRIServer->agpTextures.offset);

	ret = drmAgpAlloc( pMga->drmFD, pMGADRIServer->agp.size,
			   0, NULL, &pMGADRIServer->agp.handle );
	if ( ret < 0 ) {
	    xf86DrvMsg( pScreen->myNum, X_ERROR, "[agp] Out of memory (%d)\n", ret );
	    drmAgpRelease( pMga->drmFD );
	    return FALSE;
	}
	xf86DrvMsg( pScreen->myNum, X_INFO,
 		    "[agp] %d kB allocated with handle 0x%08x\n",
		    pMGADRIServer->agp.size/1024, 
		    (unsigned int) pMGADRIServer->agp.handle );

	if ( drmAgpBind( pMga->drmFD, pMGADRIServer->agp.handle, 0 ) < 0 ) {
	    xf86DrvMsg( pScreen->myNum, X_ERROR, "[agp] Could not bind memory\n" );
	    drmAgpFree( pMga->drmFD, pMGADRIServer->agp.handle );
	    drmAgpRelease( pMga->drmFD );
	    return FALSE;
	}

	/* WARP microcode space
	 */
	if ( drmAddMap( pMga->drmFD,
			pMGADRIServer->warp.offset,
			pMGADRIServer->warp.size,
			DRM_AGP, DRM_READ_ONLY,
			&pMGADRIServer->warp.handle ) < 0 ) {
	    xf86DrvMsg( pScreen->myNum, X_ERROR,
			"[agp] Could not add WARP microcode mapping\n" );
	    return FALSE;
	}
	xf86DrvMsg( pScreen->myNum, X_INFO,
 		    "[agp] WARP microcode handle = 0x%08x\n",
		    (unsigned int) pMGADRIServer->warp.handle );

	/* Primary DMA space
	 */
	if ( drmAddMap( pMga->drmFD,
			pMGADRIServer->primary.offset,
			pMGADRIServer->primary.size,
			DRM_AGP, DRM_READ_ONLY,
			&pMGADRIServer->primary.handle ) < 0 ) {
	    xf86DrvMsg( pScreen->myNum, X_ERROR,
			"[agp] Could not add primary DMA mapping\n" );
	    return FALSE;
	}
	xf86DrvMsg( pScreen->myNum, X_INFO,
 		    "[agp] Primary DMA handle = 0x%08x\n",
		    (unsigned int) pMGADRIServer->primary.handle );

	/* DMA buffers
	 */
	if ( drmAddMap( pMga->drmFD,
			pMGADRIServer->buffers.offset,
			pMGADRIServer->buffers.size,
			DRM_AGP, 0,
			&pMGADRIServer->buffers.handle ) < 0 ) {
	    xf86DrvMsg( pScreen->myNum, X_ERROR,
			"[agp] Could not add DMA buffers mapping\n" );
	    return FALSE;
	}
	xf86DrvMsg( pScreen->myNum, X_INFO,
 		    "[agp] DMA buffers handle = 0x%08x\n",
		    (unsigned int) pMGADRIServer->buffers.handle );

	count = drmAddBufs( pMga->drmFD,
			    MGA_NUM_BUFFERS, MGA_BUFFER_SIZE,
			    DRM_AGP_BUFFER, pMGADRIServer->buffers.offset );
	if ( count <= 0 ) {
	    xf86DrvMsg( pScrn->scrnIndex, X_INFO,
			"[drm] failure adding %d %d byte DMA buffers\n",
			MGA_NUM_BUFFERS, MGA_BUFFER_SIZE );
	    return FALSE;
	}
	xf86DrvMsg( pScreen->myNum, X_INFO,
		    "[drm] Added %d %d byte DMA buffers\n",
		    count, MGA_BUFFER_SIZE );

	i = mylog2(pMGADRIServer->agpTextures.size / MGA_NR_TEX_REGIONS);
	if(i < MGA_LOG_MIN_TEX_REGION_SIZE)
	  i = MGA_LOG_MIN_TEX_REGION_SIZE;
	pMGADRIServer->agpTextures.size = (pMGADRIServer->agpTextures.size >> i) << i;

	if ( drmAddMap( pMga->drmFD,
			pMGADRIServer->agpTextures.offset,
			pMGADRIServer->agpTextures.size,
			DRM_AGP, 0,
			&pMGADRIServer->agpTextures.handle ) < 0 ) {
	    xf86DrvMsg( pScreen->myNum, X_ERROR,
			"[agp] Could not add agpTexture mapping\n" );
	    return FALSE;
	}

	xf86DrvMsg( pScreen->myNum, X_INFO,
 		    "[agp] agpTexture handle = 0x%08x\n",
		    (unsigned int) pMGADRIServer->agpTextures.handle );
	xf86DrvMsg( pScreen->myNum, X_INFO,
		    "[agp] agpTexture size: %d kb\n", pMGADRIServer->agpTextures.size/1024 );

	pMGADRIServer->registers.size = MGAIOMAPSIZE;

	if ( drmAddMap( pMga->drmFD,
			(drm_handle_t) MGA_IO_ADDRESS(pMga),
			pMGADRIServer->registers.size,
			DRM_REGISTERS, DRM_READ_ONLY,
			&pMGADRIServer->registers.handle ) < 0 ) {
	    xf86DrvMsg( pScreen->myNum, X_ERROR,
			"[drm] Could not add MMIO registers mapping\n" );
	    return FALSE;
	}
	xf86DrvMsg( pScreen->myNum, X_INFO,
 		    "[drm] Registers handle = 0x%08x\n",
		    (unsigned int) pMGADRIServer->registers.handle );

	pMGADRIServer->status.size = SAREA_MAX;

	if ( drmAddMap( pMga->drmFD, 0, pMGADRIServer->status.size,
			DRM_SHM, DRM_READ_ONLY | DRM_LOCKED | DRM_KERNEL,
			&pMGADRIServer->status.handle ) < 0 ) {
	    xf86DrvMsg( pScreen->myNum, X_ERROR,
			"[drm] Could not add status page mapping\n" );
	    return FALSE;
	}
	xf86DrvMsg( pScreen->myNum, X_INFO,
 		    "[drm] Status handle = 0x%08x\n",
		    (unsigned int) pMGADRIServer->status.handle );
    }

    return TRUE;
}

static Bool MGADRIKernelInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   MGAPtr pMga = MGAPTR(pScrn);
   MGADRIServerPrivatePtr pMGADRIServer = pMga->DRIServerInfo;
   drm_mga_init_t init;
   int ret;


   if (!pMga->chip_attribs->dri_capable) {
       return FALSE;
   }

   memset( &init, 0, sizeof(drm_mga_init_t) );

   init.func = MGA_INIT_DMA;
   init.sarea_priv_offset = sizeof(XF86DRISAREARec);
   init.chipset = pMga->chip_attribs->dri_chipset;
   init.sgram = !pMga->HasSDRAM;

   init.maccess = pMga->MAccess;

   init.fb_cpp		= pScrn->bitsPerPixel / 8;
   init.front_offset	= pMGADRIServer->frontOffset;
   init.front_pitch	= pMGADRIServer->frontPitch / init.fb_cpp;
   init.back_offset	= pMGADRIServer->backOffset;
   init.back_pitch	= pMGADRIServer->backPitch / init.fb_cpp;

   init.depth_cpp	= pScrn->bitsPerPixel / 8;
   init.depth_offset	= pMGADRIServer->depthOffset;
   init.depth_pitch	= pMGADRIServer->depthPitch / init.depth_cpp;

   init.texture_offset[0] = pMGADRIServer->textureOffset;
   init.texture_size[0] = pMGADRIServer->textureSize;

   init.fb_offset = pMGADRIServer->fb.handle;
   init.mmio_offset = pMGADRIServer->registers.handle;
   init.status_offset = pMGADRIServer->status.handle;

   init.warp_offset = pMGADRIServer->warp.handle;
   init.primary_offset = pMGADRIServer->primary.handle;
   init.buffers_offset = pMGADRIServer->buffers.handle;

   init.texture_offset[1] = pMGADRIServer->agpTextures.handle;
   init.texture_size[1] = pMGADRIServer->agpTextures.size;

   ret = drmCommandWrite( pMga->drmFD, DRM_MGA_INIT, &init, sizeof(drm_mga_init_t));
   if ( ret < 0 ) {
      xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		  "[drm] Failed to initialize DMA! (%d)\n", ret );
      return FALSE;
   }


   return TRUE;
}

/* FIXME: This function uses the DRM to get the IRQ, but the pci_device
 * FIXME: structure (PciInfo) already has that information.
 */
static void MGADRIIrqInit(MGAPtr pMga, ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);

   if (!pMga->irq) {
      pMga->irq = drmGetInterruptFromBusID(pMga->drmFD,
#ifdef XSERVER_LIBPCIACCESS
					   ((pMga->PciInfo->domain << 8) |
					    pMga->PciInfo->bus),
					   pMga->PciInfo->dev,
					   pMga->PciInfo->func
#else
	 ((pciConfigPtr)pMga->PciInfo->thisCard)->busnum,
	 ((pciConfigPtr)pMga->PciInfo->thisCard)->devnum,
	 ((pciConfigPtr)pMga->PciInfo->thisCard)->funcnum
#endif
					   );

      if((drmCtlInstHandler(pMga->drmFD, pMga->irq)) != 0) {
	 xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		    "[drm] failure adding irq handler, "
		    "there is a device already using that irq\n"
		    "[drm] falling back to irq-free operation\n");
	 pMga->irq = 0;
      } else {
          pMga->reg_ien = INREG( MGAREG_IEN );
      }
   }

   if (pMga->irq)
      xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		 "[drm] dma control initialized, using IRQ %d\n",
		 pMga->irq);
}

static Bool MGADRIBuffersInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   MGAPtr pMga = MGAPTR(pScrn);
   MGADRIServerPrivatePtr pMGADRIServer = pMga->DRIServerInfo;


   pMGADRIServer->drmBuffers = drmMapBufs( pMga->drmFD );
   if ( !pMGADRIServer->drmBuffers ) {
	xf86DrvMsg( pScreen->myNum, X_ERROR,
		    "[drm] Failed to map DMA buffers list\n" );
	return FALSE;
    }
    xf86DrvMsg( pScreen->myNum, X_INFO,
		"[drm] Mapped %d DMA buffers\n",
		pMGADRIServer->drmBuffers->count );

    return TRUE;
}

#ifdef USE_XAA
static void MGADRIInitBuffersXAA(WindowPtr pWin, RegionPtr prgn,
                                 CARD32 index)
{
    ScreenPtr pScreen = pWin->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    MGAPtr pMga = MGAPTR(pScrn);
    BoxPtr pbox = REGION_RECTS(prgn);
    int nbox  = REGION_NUM_RECTS(prgn);
    XAAInfoRecPtr xaa = pMga->AccelInfoRec;

    CHECK_DMA_QUIESCENT(MGAPTR(pScrn), pScrn);

    xaa->SetupForSolidFill(pScrn, 0, GXcopy, -1);

    while (nbox--) {
        MGASelectBuffer(pScrn, MGA_BACK);
        xaa->SubsequentSolidFillRect(pScrn, pbox->x1, pbox->y1,
                                     pbox->x2-pbox->x1, pbox->y2-pbox->y1);
        MGASelectBuffer(pScrn, MGA_DEPTH);
        xaa->SubsequentSolidFillRect(pScrn, pbox->x1, pbox->y1,
                                     pbox->x2-pbox->x1, pbox->y2-pbox->y1);
        pbox++;
    }

    MGASelectBuffer(pScrn, MGA_FRONT);

    pMga->AccelInfoRec->NeedToSync = TRUE;
}
#endif

static void MGADRIInitBuffersEXA(WindowPtr pWin, RegionPtr prgn,
                                 CARD32 index)
{
    /* FIXME */
}

#ifdef USE_XAA
/*
  This routine is a modified form of XAADoBitBlt with the calls to
  ScreenToScreenBitBlt built in. My routine has the prgnSrc as source
  instead of destination. My origin is upside down so the ydir cases
  are reversed.
*/
static void MGADRIMoveBuffersXAA(WindowPtr pParent, DDXPointRec ptOldOrg,
                                 RegionPtr prgnSrc, CARD32 index)
{
    ScreenPtr pScreen = pParent->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    MGAPtr pMga = MGAPTR(pScrn);
    int nbox;
    BoxPtr pbox, pboxTmp, pboxNext, pboxBase, pboxNew1, pboxNew2;
    DDXPointPtr pptTmp, pptNew1, pptNew2;
    int xdir, ydir;
    int dx, dy;
    DDXPointPtr pptSrc;
    int screenwidth = pScrn->virtualX;
    int screenheight = pScrn->virtualY;
    XAAInfoRecPtr xaa = pMga->AccelInfoRec;

    CHECK_DMA_QUIESCENT(pMga, pScrn);

    pbox = REGION_RECTS(prgnSrc);
    nbox = REGION_NUM_RECTS(prgnSrc);
    pboxNew1 = 0;
    pptNew1 = 0;
    pboxNew2 = 0;
    pboxNew2 = 0;
    pptSrc = &ptOldOrg;

    dx = pParent->drawable.x - ptOldOrg.x;
    dy = pParent->drawable.y - ptOldOrg.y;

    /* If the copy will overlap in Y, reverse the order */
    if (dy>0) {
        ydir = -1;

        if (nbox>1) {
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
	    pboxNew2 = (BoxPtr)malloc(sizeof(BoxRec)*nbox);
	    pptNew2 = (DDXPointPtr)malloc(sizeof(DDXPointRec)*nbox);
	    if (!pboxNew2 || !pptNew2) {
	        free(pptNew2);
	        free(pboxNew2);
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

    xaa->SetupForScreenToScreenCopy(pScrn, xdir, ydir, GXcopy, -1, -1);
    for ( ; nbox-- ; pbox++) {
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

	 MGASelectBuffer(pScrn, MGA_BACK);
	 xaa->SubsequentScreenToScreenCopy(pScrn, x1, y1, destx, desty, w, h);
	 MGASelectBuffer(pScrn, MGA_DEPTH);
	 xaa->SubsequentScreenToScreenCopy(pScrn, x1, y1, destx, desty, w, h);
    }
    MGASelectBuffer(pScrn, MGA_FRONT);

    if (pboxNew2) {
        free(pptNew2);
        free(pboxNew2);
    }
    if (pboxNew1) {
        free(pptNew1);
        free(pboxNew1);
    }

    pMga->AccelInfoRec->NeedToSync = TRUE;

}
#endif

static void MGADRIMoveBuffersEXA(WindowPtr pParent, DDXPointRec ptOldOrg,
                                 RegionPtr prgnSrc, CARD32 index)
{
    /* FIXME */
}

Bool MGADRIScreenInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   MGAPtr pMga = MGAPTR(pScrn);
   DRIInfoPtr pDRIInfo;
   MGADRIPtr pMGADRI;
   MGADRIServerPrivatePtr pMGADRIServer;

   if (!pMga->chip_attribs->dri_capable) {
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "[drm] Direct rendering only supported with G200/G400/G450/G550.\n");
       return FALSE;
   }

   /* Check that the GLX, DRI, and DRM modules have been loaded by testing
    * for canonical symbols in each module.
    */
   if ( !xf86LoaderCheckSymbol( "GlxSetVisualConfigs" ) )	return FALSE;
   if ( !xf86LoaderCheckSymbol( "drmAvailable" ) )		return FALSE;
   if ( !xf86LoaderCheckSymbol( "DRIQueryVersion" ) ) {
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[dri] MGADRIScreenInit failed (libdri.a too old)\n" );
      return FALSE;
   }

   /* Check the DRI version */
   {
      int major, minor, patch;
      DRIQueryVersion( &major, &minor, &patch );
      if ( major != DRIINFO_MAJOR_VERSION || minor < DRIINFO_MINOR_VERSION ) {
         xf86DrvMsg( pScreen->myNum, X_ERROR,
		     "[dri] MGADRIScreenInit failed because of a version mismatch.\n"
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
   pMga->pDRIInfo = pDRIInfo;

   pDRIInfo->drmDriverName = MGAKernelDriverName;
   pDRIInfo->clientDriverName = MGAClientDriverName;
   if (xf86LoaderCheckSymbol("DRICreatePCIBusID")) {
      pDRIInfo->busIdString = DRICreatePCIBusID(pMga->PciInfo);
   } else {
      pDRIInfo->busIdString = malloc(64);
      sprintf( pDRIInfo->busIdString, "PCI:%d:%d:%d",
#ifdef XSERVER_LIBPCIACCESS
	       ((pMga->PciInfo->domain << 8) | pMga->PciInfo->bus),
	       pMga->PciInfo->dev, pMga->PciInfo->func
#else
	       ((pciConfigPtr)pMga->PciInfo->thisCard)->busnum,
	       ((pciConfigPtr)pMga->PciInfo->thisCard)->devnum,
	       ((pciConfigPtr)pMga->PciInfo->thisCard)->funcnum
#endif
	       );
   }
   pDRIInfo->ddxDriverMajorVersion = PACKAGE_VERSION_MAJOR;
   pDRIInfo->ddxDriverMinorVersion = PACKAGE_VERSION_MINOR;
   pDRIInfo->ddxDriverPatchVersion = PACKAGE_VERSION_PATCHLEVEL;
   pDRIInfo->frameBufferPhysicalAddress = (void *) pMga->FbAddress;
   pDRIInfo->frameBufferSize = pMga->FbMapSize;
   pDRIInfo->frameBufferStride = pScrn->displayWidth*(pScrn->bitsPerPixel/8);
   pDRIInfo->ddxDrawableTableEntry = MGA_MAX_DRAWABLES;

   pDRIInfo->createDummyCtx = TRUE;
   pDRIInfo->createDummyCtxPriv = FALSE;

   if ( SAREA_MAX_DRAWABLES < MGA_MAX_DRAWABLES ) {
      pDRIInfo->maxDrawableTableEntry = SAREA_MAX_DRAWABLES;
   } else {
      pDRIInfo->maxDrawableTableEntry = MGA_MAX_DRAWABLES;
   }

   /* For now the mapping works by using a fixed size defined
    * in the SAREA header.
    */
   if ( sizeof(XF86DRISAREARec) + sizeof(MGASAREAPrivRec) > SAREA_MAX ) {
      xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		  "[drm] Data does not fit in SAREA\n" );
      return FALSE;
   }

   xf86DrvMsg( pScrn->scrnIndex, X_INFO,
	       "[drm] Sarea %d+%d: %d\n",
	       (int)sizeof(XF86DRISAREARec), (int)sizeof(MGASAREAPrivRec),
	       (int)sizeof(XF86DRISAREARec) + (int)sizeof(MGASAREAPrivRec) );

   pDRIInfo->SAREASize = SAREA_MAX;

   pMGADRI = (MGADRIPtr)calloc( sizeof(MGADRIRec), 1 );
   if ( !pMGADRI ) {
      DRIDestroyInfoRec( pMga->pDRIInfo );
      pMga->pDRIInfo = 0;
      xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		  "[drm] Failed to allocate memory for private record\n" );
      return FALSE;
   }

   pMGADRIServer = (MGADRIServerPrivatePtr)
      calloc( sizeof(MGADRIServerPrivateRec), 1 );
   if ( !pMGADRIServer ) {
      free( pMGADRI );
      DRIDestroyInfoRec( pMga->pDRIInfo );
      pMga->pDRIInfo = 0;
      xf86DrvMsg( pScrn->scrnIndex, X_ERROR,
		  "[drm] Failed to allocate memory for private record\n" );
      return FALSE;
   }
   pMga->DRIServerInfo = pMGADRIServer;

   pDRIInfo->devPrivate = pMGADRI;
   pDRIInfo->devPrivateSize = sizeof(MGADRIRec);
   pDRIInfo->contextSize = sizeof(MGADRIContextRec);

   pDRIInfo->CreateContext = MGACreateContext;
   pDRIInfo->DestroyContext = MGADestroyContext;
   if ( xf86IsEntityShared( pScrn->entityList[0] ) 
		&& pMga->DualHeadEnabled) {
      pDRIInfo->SwapContext = MGADRISwapContextShared;
   } else {
      pDRIInfo->SwapContext = MGADRISwapContext;
   }

   pDRIInfo->InitBuffers = MGADRIInitBuffersEXA;
   pDRIInfo->MoveBuffers = MGADRIMoveBuffersEXA;
#ifdef USE_EXA
    if (pMga->Exa) {
        pDRIInfo->InitBuffers = MGADRIInitBuffersEXA;
        pDRIInfo->MoveBuffers = MGADRIMoveBuffersEXA;
    } else {
#endif
#ifdef USE_XAA
        pDRIInfo->InitBuffers = MGADRIInitBuffersXAA;
        pDRIInfo->MoveBuffers = MGADRIMoveBuffersXAA;
#endif
#ifdef USE_EXA
    }
#endif

   pDRIInfo->bufferRequests = DRI_ALL_WINDOWS;

   if ( !DRIScreenInit( pScreen, pDRIInfo, &pMga->drmFD ) ) {
      free( pMGADRIServer );
      pMga->DRIServerInfo = 0;
      free( pDRIInfo->devPrivate );
      pDRIInfo->devPrivate = 0;
      DRIDestroyInfoRec( pMga->pDRIInfo );
      pMga->pDRIInfo = 0;
      xf86DrvMsg( pScreen->myNum, X_ERROR,
		  "[drm] DRIScreenInit failed.  Disabling DRI.\n" );
      return FALSE;
   }

   /* Check the DRM versioning */
   {
      drmVersionPtr version;

      /* Check the DRM lib version.
	 drmGetLibVersion was not supported in version 1.0, so check for
	 symbol first to avoid possible crash or hang.
       */
      if (xf86LoaderCheckSymbol("drmGetLibVersion")) {
         version = drmGetLibVersion(pMga->drmFD);
      }
      else {
	 /* drmlib version 1.0.0 didn't have the drmGetLibVersion
	    entry point.  Fake it by allocating a version record
	    via drmGetVersion and changing it to version 1.0.0
	  */
	 version = drmGetVersion(pMga->drmFD);
	 version->version_major      = 1;
	 version->version_minor      = 0;
	 version->version_patchlevel = 0;
      }

      if (version) {
	 if (version->version_major != 1 ||
	     version->version_minor < 1) {
	     /* incompatible drm library version */
	    xf86DrvMsg(pScreen->myNum, X_ERROR,
		       "[dri] MGADRIScreenInit failed because of a version mismatch.\n"
		       "[dri] libdrm.a module version is %d.%d.%d but version 1.1.x is needed.\n"
		       "[dri] Disabling DRI.\n",
		       version->version_major,
		       version->version_minor,
		       version->version_patchlevel);
	    drmFreeVersion(version);
	    MGADRICloseScreen( pScreen );		/* FIXME: ??? */
	    return FALSE;
	 }
	 drmFreeVersion(version);
      }

      /* Check the MGA DRM version */
      version = drmGetVersion(pMga->drmFD);
      if ( version ) {
         if ( version->version_major != 3 ||
	      version->version_minor < 0 ) {
            /* incompatible drm version */
            xf86DrvMsg( pScreen->myNum, X_ERROR,
			"[dri] MGADRIScreenInit failed because of a version mismatch.\n"
			"[dri] mga.o kernel module version is %d.%d.%d but version 3.0.x is needed.\n"
			"[dri] Disabling DRI.\n",
			version->version_major,
			version->version_minor,
			version->version_patchlevel );
            drmFreeVersion( version );
	    MGADRICloseScreen( pScreen );		/* FIXME: ??? */
            return FALSE;
         }
	 pMGADRIServer->drm_version_major = version->version_major;
	 pMGADRIServer->drm_version_minor = version->version_minor;

         drmFreeVersion( version );
      }
   }

   if ( (pMga->bios.host_interface == MGA_HOST_PCI) &&
	((pMGADRIServer->drm_version_minor < 2) || pMga->useOldDmaInit) ) {
       /* PCI cards are supported if the DRM version is at least 3.2 and the
	* user has not explicitly disabled the new DMA init path (i.e., to
	* support old version of the client-side driver that don't use the
	* new features of the 3.2 DRM).
	*/
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR, 
		  "[drm] Direct rendering on PCI cards requires DRM version 3.2 or higher\n");
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR, 
		  "[drm] and a recent client-side driver.  Also make sure that 'OldDmaInit'\n");
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR, 
		  "[drm] is not selected in xorg.conf.'\n");
       return FALSE;
   }


   if ( !MGADRIBootstrapDMA( pScreen ) ) {
      DRICloseScreen( pScreen );
      return FALSE;
   }

   {
       void *scratch_ptr;
       int scratch_int;

       DRIGetDeviceInfo(pScreen, &pMGADRIServer->fb.handle,
			&scratch_int, &scratch_int, 
			&scratch_int, &scratch_int,
			&scratch_ptr);
   }

   if ( !MGAInitVisualConfigs( pScreen ) ) {
      DRICloseScreen( pScreen );
      return FALSE;
   }
   xf86DrvMsg( pScrn->scrnIndex, X_INFO, "[dri] visual configs initialized\n" );

   return TRUE;
}


Bool MGADRIFinishScreenInit( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   MGAPtr pMga = MGAPTR(pScrn);
   MGADRIServerPrivatePtr pMGADRIServer = pMga->DRIServerInfo;
   MGADRIPtr pMGADRI;
   int i;

   if ( !pMga->pDRIInfo )
      return FALSE;

   pMGADRI = (MGADRIPtr)pMga->pDRIInfo->devPrivate;

   pMga->pDRIInfo->driverSwapMethod = DRI_HIDE_X_CONTEXT;

   /* NOTE: DRIFinishScreenInit must be called before *DRIKernelInit
    * because *DRIKernelInit requires that the hardware lock is held by
    * the X server, and the first time the hardware lock is grabbed is
    * in DRIFinishScreenInit.
    */
   if ( !DRIFinishScreenInit( pScreen ) ) {
      MGADRICloseScreen( pScreen );
      return FALSE;
   }

   if ( !MGADRIKernelInit( pScreen ) ) {
      MGADRICloseScreen( pScreen );
      return FALSE;
   }

   if ( !MGADRIBuffersInit( pScreen ) ) {
      MGADRICloseScreen( pScreen );
      return FALSE;
   }

   MGADRIIrqInit(pMga, pScreen);

   pMGADRI->chipset		= pMga->chip_attribs->dri_chipset;
   pMGADRI->width		= pScrn->virtualX;
   pMGADRI->height		= pScrn->virtualY;
   pMGADRI->cpp			= pScrn->bitsPerPixel / 8;

   pMGADRI->agpMode		= pMga->agpMode;

   pMGADRI->frontOffset		= pMGADRIServer->frontOffset;
   pMGADRI->frontPitch		= pMGADRIServer->frontPitch;
   pMGADRI->backOffset		= pMGADRIServer->backOffset;
   pMGADRI->backPitch		= pMGADRIServer->backPitch;
   pMGADRI->depthOffset		= pMGADRIServer->depthOffset;
   pMGADRI->depthPitch		= pMGADRIServer->depthPitch;
   pMGADRI->textureOffset	= pMGADRIServer->textureOffset;
   pMGADRI->textureSize		= pMGADRIServer->textureSize;

   pMGADRI->agpTextureOffset = (unsigned int)pMGADRIServer->agpTextures.handle;
   pMGADRI->agpTextureSize = (unsigned int)pMGADRIServer->agpTextures.size;

   pMGADRI->sarea_priv_offset = sizeof(XF86DRISAREARec);


   /* Newer versions of the client-side driver do not need these if the
    * kernel version is high enough to support interrupt based waiting.
    */

   pMGADRI->registers.handle	= pMGADRIServer->registers.handle;
   pMGADRI->registers.size	= pMGADRIServer->registers.size;
   pMGADRI->primary.handle	= pMGADRIServer->primary.handle;
   pMGADRI->primary.size	= pMGADRIServer->primary.size;


   /* These are no longer used by the client-side DRI driver.  They should
    * be removed in the next release (i.e., 6.9 / 7.0).
    */

   pMGADRI->status.handle	= pMGADRIServer->status.handle;
   pMGADRI->status.size		= pMGADRIServer->status.size;
   pMGADRI->buffers.handle	= pMGADRIServer->buffers.handle;
   pMGADRI->buffers.size	= pMGADRIServer->buffers.size;

   i = mylog2( pMGADRI->textureSize / MGA_NR_TEX_REGIONS );
   if ( i < MGA_LOG_MIN_TEX_REGION_SIZE )
      i = MGA_LOG_MIN_TEX_REGION_SIZE;

   pMGADRI->logTextureGranularity = i;
   pMGADRI->textureSize = (pMGADRI->textureSize >> i) << i; /* truncate */

   i = mylog2( pMGADRIServer->agpTextures.size / MGA_NR_TEX_REGIONS );
   if ( i < MGA_LOG_MIN_TEX_REGION_SIZE )
      i = MGA_LOG_MIN_TEX_REGION_SIZE;

   pMGADRI->logAgpTextureGranularity = i;

   return TRUE;
}


void MGADRICloseScreen( ScreenPtr pScreen )
{
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   MGAPtr pMga = MGAPTR(pScrn);
   MGADRIServerPrivatePtr pMGADRIServer = pMga->DRIServerInfo;
   drm_mga_init_t init;

   if ( pMGADRIServer->drmBuffers ) {
      drmUnmapBufs( pMGADRIServer->drmBuffers );
      pMGADRIServer->drmBuffers = NULL;
   }

   if (pMga->irq) {
      drmCtlUninstHandler(pMga->drmFD);
      pMga->irq = 0;
      pMga->reg_ien = 0;
   }

   /* Cleanup DMA */
   memset( &init, 0, sizeof(drm_mga_init_t) );
   init.func = MGA_CLEANUP_DMA;
   drmCommandWrite( pMga->drmFD, DRM_MGA_INIT, &init, sizeof(drm_mga_init_t) );

   if ( pMGADRIServer->agp.handle != DRM_AGP_NO_HANDLE ) {
      drmAgpUnbind( pMga->drmFD, pMGADRIServer->agp.handle );
      drmAgpFree( pMga->drmFD, pMGADRIServer->agp.handle );
      pMGADRIServer->agp.handle = DRM_AGP_NO_HANDLE;
      drmAgpRelease( pMga->drmFD );
   }

   DRICloseScreen( pScreen );

   if ( pMga->pDRIInfo ) {
      free(pMga->pDRIInfo->devPrivate);
      pMga->pDRIInfo->devPrivate = 0;
      DRIDestroyInfoRec( pMga->pDRIInfo );
      pMga->pDRIInfo = 0;
   }
   free(pMga->DRIServerInfo);
   pMga->DRIServerInfo = 0;
   free(pMga->pVisualConfigs);
   free(pMga->pVisualConfigsPriv);
}
