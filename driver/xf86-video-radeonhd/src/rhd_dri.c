/*
 * Copyright 2000  ATI Technologies Inc., Markham, Ontario,
 * Copyright 2000  VA Linux Systems Inc., Fremont, California.
 * Copyright 2007  Luc Verhaegen <libv@exsuse.de>
 * Copyright 2007  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007  Egbert Eich   <eich@novell.com>
 * Copyright 2007  Advanced Micro Devices, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Based on radeon_dri.c
 * Original authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *   Rickard E. Faith <faith@valinux.com>
 *   Gareth Hughes <gareth@valinux.com>
 * Additional main authors:
 *   Dave Airlie <airlied@linux.ie>
 *   Michel Dänzer <michel@tungstengraphics.com>
 *   Benjamin Herrenschmidt <benh@kernel.crashing.org>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* X and server generic header files */
#include "xf86.h"
#include "xaa.h"
#ifdef USE_EXA
# include "exa.h"
#endif
#include "xf86PciInfo.h"
#include "windowstr.h"

/* GLX/DRI/DRM definitions */
#define _XF86DRI_SERVER_
#include "dri.h"
/* Workaround for header mismatches */
#ifndef DEPRECATED
#  define DEPRECATED __attribute__ ((deprecated))
#  define __user
#endif
#include "radeon_drm.h"
#include "GL/glxint.h"
#include "GL/glxtokens.h"
#include "sarea.h"

#if HAVE_XF86_ANSIC_H
# include "xf86_ansic.h"
#else
# include <string.h>
# include <stdio.h>
# include <unistd.h>
#endif

/* Driver data structures */
#include "rhd.h"
#include "rhd_regs.h"
#include "rhd_dri.h"
#include "r5xx_accel.h"
#include "r5xx_regs.h"
#include "r6xx_accel.h"
#include "rhd_cs.h"

#define IS_RADEONHD_DRIVER 1
#include "radeon_dri.h"

#ifdef RANDR_12_SUPPORT		// FIXME check / move to rhd_randr.c
# include "xf86i2c.h" /* this is complete BS, stop using unnamed structs! */
# include "xf86Crtc.h"
#endif


/* DRI Driver defaults */
#define RHD_DEFAULT_GART_SIZE       16	/* MB (must be 2^n and > 4MB) */
#define RHD_DEFAULT_RING_SIZE       2	/* MB (must be page aligned) */
#define RHD_DEFAULT_BUFFER_SIZE     2	/* MB (must be page aligned) */
#define RHD_DEFAULT_CP_TIMEOUT      100000  /* usecs */
#define RHD_DEFAULT_PCI_APER_SIZE   32	/* in MB */

#define RADEON_MAX_DRAWABLES        256

#define RADEON_DRIAPI_VERSION_MAJOR 4
#define RADEON_DRIAPI_VERSION_MAJOR_TILED 5
#define RADEON_DRIAPI_VERSION_MINOR 3
#define RADEON_DRIAPI_VERSION_PATCH 0

#define RHD_IDLE_RETRY              16	/* Fall out of idle loops after this count */

/* awkward... */
#define PIXEL_CODE(x) (x->bitsPerPixel != 16 ? x->bitsPerPixel : x->depth)

typedef struct {
    /* Nothing here yet */
    int dummy;
} RADEONConfigPrivRec, *RADEONConfigPrivPtr;

typedef struct {
    /* Nothing here yet */
    int dummy;
} RADEONDRIContextRec, *RADEONDRIContextPtr;

/* driver data only needed by dri */
struct rhdDri {
    int               scrnIndex;

    /* FIXME: Some Save&Restore is still a TODO
     * Need to save/restore/update GEN_INT_CNTL (interrupts) on drm init.
     * AGP_BASE, MC_FB_LOCATION, MC_AGP_LOCATION are (partially) handled
     * in _mc.c */

    int               pixel_code;

    DRIInfoPtr        pDRIInfo;
    int               drmFD;
    int               numVisualConfigs;
    __GLXvisualConfig *pVisualConfigs;
    RADEONConfigPrivPtr pVisualConfigsPriv;

    drm_handle_t      registerHandle;
    drm_handle_t      pciMemHandle;
    int               irq;

    int               have3Dwindows;

    drmSize           gartSize;
    drm_handle_t      agpMemHandle;     /* Handle from drmAgpAlloc */
    unsigned long     gartOffset;
    int               agpMode;

    /* CP ring buffer data */
    unsigned long     ringStart;        /* Offset into GART space */
    drm_handle_t      ringHandle;       /* Handle from drmAddMap */
    drmSize           ringMapSize;      /* Size of map */
    int               ringSize;         /* Size of ring (in MB) */
    drmAddress        ring;             /* Map */
    int               ringSizeLog2QW;

    // TODO: what is r/o ring space for (1 page)
    unsigned long     ringReadOffset;   /* Offset into GART space */
    drm_handle_t      ringReadPtrHandle; /* Handle from drmAddMap */
    drmSize           ringReadMapSize;  /* Size of map */
    drmAddress        ringReadPtr;      /* Map */

    /* CP vertex/indirect buffer data */
    unsigned long     bufStart;         /* Offset into GART space */
    drm_handle_t      bufHandle;        /* Handle from drmAddMap */
    drmSize           bufMapSize;       /* Size of map */
    int               bufSize;          /* Size of buffers (in MB) */
    drmAddress        buf;              /* Map */
    int               bufNumBufs;       /* Number of buffers */
    drmBufMapPtr      buffers;          /* Buffer map */

    /* CP GART Texture data */
    unsigned long     gartTexStart;      /* Offset into GART space */
    drm_handle_t      gartTexHandle;     /* Handle from drmAddMap */
    drmSize           gartTexMapSize;    /* Size of map */
    int               gartTexSize;       /* Size of GART tex space (in MB) */
    drmAddress        gartTex;           /* Map */
    int               log2GARTTexGran;

    /* DRI screen private data */
    int               frontOffset;
    int               frontPitch;
    int               backOffset;
    int               backPitch;
    int               depthOffset;
    int               depthPitch;
    int               depthBits;
    int               textureOffset;
    int               textureSize;
    int               log2TexGran;

    int               pciGartSize;
    CARD32            pciGartOffset;
    void             *pciGartBackup;

    CARD32            gartLocation;
};

static size_t radeon_drm_page_size;
static char  *dri_driver_name  = "radeon";
static char  *r300_driver_name = "r300";
static char  *r600_driver_name = "r600";


extern void GlxSetVisualConfigs(int nconfigs, __GLXvisualConfig *configs,
				void **configprivs);


static void RHDDRIAllocatePCIGARTTable(ScrnInfoPtr pScrn);
static int  RHDDRIGetPciAperTableSize(ScrnInfoPtr pScrn);
static int  RHDDRISetParam(ScrnInfoPtr pScrn,
			   unsigned int param, int64_t value);
static void RHDDRITransitionTo2d(ScreenPtr pScreen);
static void RHDDRITransitionTo3d(ScreenPtr pScreen);
static void RHDDRITransitionMultiToSingle3d(ScreenPtr pScreen);
static void RHDDRITransitionSingleToMulti3d(ScreenPtr pScreen);


/* Compute log base 2 of val */
static __inline__ int RHDMinBits(int val)
{
    int  bits;

    if (!val) return 1;
    for (bits = 0; val; val >>= 1, ++bits);
    return bits;
}

/* Initialize the visual configs that are supported by the hardware.
 * These are combined with the visual configs that the indirect
 * rendering core supports, and the intersection is exported to the
 * client. */
static Bool RHDInitVisualConfigs(ScreenPtr pScreen)
{
    ScrnInfoPtr          pScrn             = xf86Screens[pScreen->myNum];
    struct rhdDri       *rhdDRI              = RHDPTR(pScrn)->dri;
    int                  numConfigs        = 0;
    __GLXvisualConfig   *pConfigs          = 0;
    RADEONConfigPrivPtr  pRADEONConfigs    = 0;
    RADEONConfigPrivPtr *pRADEONConfigPtrs = 0;
    int                  i, accum, stencil, db;
    int pixel_code = PIXEL_CODE(pScrn);
    RHDFUNC(pScrn);

#define RHD_USE_ACCUM   1
#define RHD_USE_STENCIL 1
#define RHD_USE_DB      1

    switch (pixel_code) {

    case 16:
    case 32:
	numConfigs = 1;
	if (RHD_USE_ACCUM)   numConfigs *= 2;
	if (RHD_USE_STENCIL) numConfigs *= 2;
	if (RHD_USE_DB)      numConfigs *= 2;

	if (!(pConfigs
	      = (__GLXvisualConfig *)xcalloc(sizeof(__GLXvisualConfig),
					     numConfigs))) {
	    return FALSE;
	}
	if (!(pRADEONConfigs
	      = (RADEONConfigPrivPtr)xcalloc(sizeof(RADEONConfigPrivRec),
					     numConfigs))) {
	    xfree(pConfigs);
	    return FALSE;
	}
	if (!(pRADEONConfigPtrs
	      = (RADEONConfigPrivPtr *)xcalloc(sizeof(RADEONConfigPrivPtr),
					       numConfigs))) {
	    xfree(pConfigs);
	    xfree(pRADEONConfigs);
	    return FALSE;
	}

	i = 0;
	for (db = RHD_USE_DB; db >= 0; db--) {
	  for (accum = 0; accum <= RHD_USE_ACCUM; accum++) {
	    for (stencil = 0; stencil <= RHD_USE_STENCIL; stencil++) {
		pRADEONConfigPtrs[i] = &pRADEONConfigs[i];

		pConfigs[i].vid                = (VisualID)(-1);
		pConfigs[i].class              = -1;
		pConfigs[i].rgba               = TRUE;
		if (pixel_code == 32) {
		    pConfigs[i].redSize            = 8;
		    pConfigs[i].greenSize          = 8;
		    pConfigs[i].blueSize           = 8;
		    pConfigs[i].alphaSize          = 8;
		    pConfigs[i].redMask            = 0x00FF0000;
		    pConfigs[i].greenMask          = 0x0000FF00;
		    pConfigs[i].blueMask           = 0x000000FF;
		    pConfigs[i].alphaMask          = 0xFF000000;
		} else {
		    pConfigs[i].redSize            = 5;
		    pConfigs[i].greenSize          = 6;
		    pConfigs[i].blueSize           = 5;
		    pConfigs[i].redMask            = 0x0000F800;
		    pConfigs[i].greenMask          = 0x000007E0;
		    pConfigs[i].blueMask           = 0x0000001F;
		}
		if (accum) { /* Simulated in software */
		    pConfigs[i].accumRedSize   = 16;
		    pConfigs[i].accumGreenSize = 16;
		    pConfigs[i].accumBlueSize  = 16;
		    if (pixel_code == 32)
			pConfigs[i].accumAlphaSize = 16;
		}
		pConfigs[i].doubleBuffer       = db;
		pConfigs[i].bufferSize         = pixel_code;
		pConfigs[i].depthSize          = rhdDRI->depthBits;
		if (stencil)
		    pConfigs[i].stencilSize    = 8;
		if (accum ||
		    (pConfigs[i].stencilSize && pConfigs[i].depthSize != 24))
		    pConfigs[i].visualRating   = GLX_SLOW_CONFIG;
		else
		    pConfigs[i].visualRating   = GLX_NONE;
		pConfigs[i].transparentPixel   = GLX_NONE;
		i++;
	    }
	  }
	}
	break;

    default:
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[dri] RHDInitVisualConfigs failed "
		   "(depth %d not supported).  "
		   "Disabling DRI.\n", pixel_code);
	return FALSE;
    }

    rhdDRI->numVisualConfigs   = numConfigs;
    rhdDRI->pVisualConfigs     = pConfigs;
    rhdDRI->pVisualConfigsPriv = pRADEONConfigs;
    GlxSetVisualConfigs(numConfigs, pConfigs, (void**)pRADEONConfigPtrs);
    return TRUE;
}

/* Create the Radeon-specific context information */
static Bool RHDCreateContext(ScreenPtr pScreen, VisualPtr visual,
			     drm_context_t hwContext, void *pVisualConfigPriv,
			     DRIContextType contextStore)
{
    return TRUE;
}

/* Destroy the Radeon-specific context information */
static void RHDDestroyContext(ScreenPtr pScreen, drm_context_t hwContext,
			      DRIContextType contextStore)
{
}

/*
 *
 */
void
RHDDRIContextClaim(ScrnInfoPtr pScrn)
{
    drm_radeon_sarea_t *pSAREAPriv =
	(drm_radeon_sarea_t *) DRIGetSAREAPrivate(pScrn->pScreen);

    pSAREAPriv->ctx_owner = DRIGetContext(pScrn->pScreen);
}

/* Called when the X server is woken up to allow the last client's
 * context to be saved and the X server's context to be loaded.  This is
 * not necessary for the Radeon since the client detects when it's
 * context is not currently loaded and then load's it itself.  Since the
 * registers to start and stop the CP are privileged, only the X server
 * can start/stop the engine. */
static void RHDEnterServer(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct RhdCS *CS = rhdPtr->CS;
    drm_radeon_sarea_t *pSAREAPriv;

#ifdef USE_EXA
    if (rhdPtr->EXAInfo)
	exaMarkSync(pScrn->pScreen);
#endif

    pSAREAPriv = (drm_radeon_sarea_t *)DRIGetSAREAPrivate(pScreen);
    if (pSAREAPriv->ctx_owner != (signed) DRIGetContext(pScreen)) {
	if (rhdPtr->ChipSet < RHD_R600) {
	    struct R5xx3D *R5xx3D = rhdPtr->ThreeDPrivate;

	    if (CS->Clean != RHD_CS_CLEAN_QUEUED) {
		R5xxDstCacheFlush(CS);
		R5xxZCacheFlush(CS);
		R5xxEngineWaitIdleFull(CS);

		CS->Clean = RHD_CS_CLEAN_QUEUED;
	    }

	    if (R5xx3D)
		R5xx3D->XHas3DEngineState = FALSE;
	} else {
	    struct r6xx_accel_state *accel_state = rhdPtr->TwoDPrivate;

	    if (CS->Clean != RHD_CS_CLEAN_QUEUED) {
		R6xxCacheFlush(CS);
		R6xxEngineWaitIdleFull(CS);

		CS->Clean = RHD_CS_CLEAN_QUEUED;
	    }

	    if (accel_state)
		accel_state->XHas3DEngineState = FALSE;
	}
    } else {
	/* if the engine has been untouched, we need to track this too. */
	if (CS->Clean != RHD_CS_CLEAN_QUEUED)
	    CS->Clean = RHD_CS_CLEAN_UNTOUCHED;
    }
}

/* Called when the X server goes to sleep to allow the X server's
 * context to be saved and the last client's context to be loaded.  This
 * is not necessary for the Radeon since the client detects when it's
 * context is not currently loaded and then load's it itself.  Since the
 * registers to start and stop the CP are privileged, only the X server
 * can start/stop the engine. */
static void RHDLeaveServer(ScreenPtr pScreen)
{
    struct RhdCS *CS = RHDPTR(xf86Screens[pScreen->myNum])->CS;
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RHDPtr rhdPtr = RHDPTR(pScrn);

    /* The CP is always running, but if we've generated any CP commands
     * we must flush them to the kernel module now. */
    if (CS->Clean == RHD_CS_CLEAN_DONE) {
	    if (rhdPtr->ChipSet >= RHD_R600) {
		R6xxCacheFlush(CS);
	    } else {
		R5xxDstCacheFlush(CS);
		R5xxZCacheFlush(CS);
	    }

	RHDCSFlush(CS); /* was a Release... */

	CS->Clean = RHD_CS_CLEAN_DIRTY;
    }
}

/* Contexts can be swapped by the X server if necessary.  This callback
 * is currently only used to perform any functions necessary when
 * entering or leaving the X server, and in the future might not be
 * necessary. */
static void RHDDRISwapContext(ScreenPtr pScreen, DRISyncType syncType,
			      DRIContextType oldContextType,
			      void *oldContext,
			      DRIContextType newContextType,
			      void *newContext)
{
    /* is this the right place to check this? Because SwapContext is still being
       called when we already switched away. --libv */
    if (!xf86Screens[pScreen->myNum]->vtSema)
	return;

    if ((syncType==DRI_3D_SYNC) && (oldContextType==DRI_2D_CONTEXT) &&
	(newContextType==DRI_2D_CONTEXT)) { /* Entering from Wakeup */
	RHDEnterServer(pScreen);
    }

    if ((syncType==DRI_2D_SYNC) && (oldContextType==DRI_NO_CONTEXT) &&
	(newContextType==DRI_2D_CONTEXT)) { /* Exiting from Block Handler */
	RHDLeaveServer(pScreen);
    }
}

/* Initialize the state of the back and depth buffers */
static void RHDDRIInitBuffers(WindowPtr pWin, RegionPtr prgn, CARD32 indx)
{
   /* NOOP.  There's no need for the 2d driver to be clearing buffers
    * for the 3d client.  It knows how to do that on its own.
    */
}

/* Copy the back and depth buffers when the X server moves a window */
static void RHDDRIMoveBuffers(WindowPtr pParent, DDXPointRec ptOldOrg,
			      RegionPtr prgnSrc, CARD32 indx)
{
    /* TODO: ATM we do not move window contents at all - back+z buffers should
     * be moved, but we need full cp support & varying-pitch + varying-tiled
     * screen-to-screen copy for that.
     * Nobody will probably notice that anyways (not with games or video
     * players, which constantly update their window contents anyway). Doesn't
     * work with EXA with radeon either.
     * Alternative: create Expose events for this region */
}

static void RHDDRIInitGARTValues(struct rhdDri * rhdDRI)
{
    int            s, l;

    RHDFUNC(rhdDRI);

    rhdDRI->gartOffset = 0;

    /* Initialize the CP ring buffer data */
    rhdDRI->ringStart       = rhdDRI->gartOffset;
    rhdDRI->ringMapSize     = rhdDRI->ringSize*1024*1024 + radeon_drm_page_size;
    rhdDRI->ringSizeLog2QW  = RHDMinBits(rhdDRI->ringSize*1024*1024/8)-1;

    rhdDRI->ringReadOffset  = rhdDRI->ringStart + rhdDRI->ringMapSize;
    rhdDRI->ringReadMapSize = radeon_drm_page_size;

    /* Reserve space for vertex/indirect buffers */
    rhdDRI->bufStart        = rhdDRI->ringReadOffset + rhdDRI->ringReadMapSize;
    rhdDRI->bufMapSize      = rhdDRI->bufSize*1024*1024;

    /* Reserve the rest for GART textures */
    rhdDRI->gartTexStart     = rhdDRI->bufStart + rhdDRI->bufMapSize;
    s = (rhdDRI->gartSize*1024*1024 - rhdDRI->gartTexStart);
    l = RHDMinBits((s-1) / RADEON_NR_TEX_REGIONS);
    if (l < RADEON_LOG_TEX_GRANULARITY) l = RADEON_LOG_TEX_GRANULARITY;
    rhdDRI->gartTexMapSize   = (s >> l) << l;
    rhdDRI->log2GARTTexGran  = l;
}

/* Set AGP transfer mode according to requests and constraints */
static Bool RHDSetAgpMode(struct rhdDri * rhdDRI, ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn  = xf86Screens[pScreen->myNum];
    RHDPtr rhdPtr = RHDPTR(pScrn);
    unsigned long mode   = drmAgpGetMode(rhdDRI->drmFD);	/* Default mode */
    unsigned int  vendor = drmAgpVendorId(rhdDRI->drmFD);
    unsigned int  device = drmAgpDeviceId(rhdDRI->drmFD);

    if (rhdPtr->ChipSet < RHD_R600) {
	/* ignore agp 3.0 mode bit from the chip as it's buggy on some cards with
	   pcie-agp rialto bridge chip - use the one from bridge which must match */
	CARD32 agp_status = (RHDRegRead (rhdDRI, AGP_STATUS) | AGPv3_MODE) & mode;
	Bool is_v3 = (agp_status & AGPv3_MODE);

	RHDFUNC(rhdDRI);

	if (is_v3) {
	    rhdDRI->agpMode = (agp_status & AGPv3_8X_MODE) ? 8 : 4;
	} else {
	    if (agp_status & AGP_4X_MODE)
		rhdDRI->agpMode = 4;
	    else if (agp_status & AGP_2X_MODE)
		rhdDRI->agpMode = 2;
	    else
		rhdDRI->agpMode = 1;
	}
	xf86DrvMsg(pScreen->myNum, X_DEFAULT, "Using AGP %dx\n", rhdDRI->agpMode);

	mode &= ~AGP_MODE_MASK;
	if (is_v3) {
	    /* only set one mode bit for AGPv3 */
	    switch (rhdDRI->agpMode) {
	    case 8:          mode |= AGPv3_8X_MODE; break;
	    case 4: default: mode |= AGPv3_4X_MODE;
	    }
	} else {
	    switch (rhdDRI->agpMode) {
	    case 4:          mode |= AGP_4X_MODE;
	    case 2:          mode |= AGP_2X_MODE;
	    case 1: default: mode |= AGP_1X_MODE;
	    }
	}
    } else
	rhdDRI->agpMode = 8; /* doesn't matter at this point */

    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] Mode 0x%08lx [AGP 0x%04x/0x%04x]\n",
	       mode, vendor, device);

    if (drmAgpEnable(rhdDRI->drmFD, mode) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[agp] AGP not enabled\n");
	drmAgpRelease(rhdDRI->drmFD);
	return FALSE;
    }

    return TRUE;
}

/* Initialize Radeon's AGP registers */
static void RHDSetAgpBase(struct rhdDri * rhdDRI, ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn  = xf86Screens[pScreen->myNum];
    RHDPtr rhdPtr = RHDPTR(pScrn);

    if (rhdPtr->ChipSet < RHD_R600)
	RHDRegWrite (rhdDRI, AGP_BASE, drmAgpBase(rhdDRI->drmFD));
}

/* Initialize the AGP state.  Request memory for use in AGP space, and
 * initialize the Radeon registers to point to that memory. */
static Bool RHDDRIAgpInit(struct rhdDri * rhdDRI, ScreenPtr pScreen)
{
    int            ret;

    RHDFUNC(rhdDRI);

    if (drmAgpAcquire(rhdDRI->drmFD) < 0) {
	xf86DrvMsg(pScreen->myNum, X_WARNING, "[agp] AGP not available\n");
	return FALSE;
    }

    if (!RHDSetAgpMode(rhdDRI, pScreen))
	return FALSE;

    RHDDRIInitGARTValues(rhdDRI);

    if ((ret = drmAgpAlloc(rhdDRI->drmFD, rhdDRI->gartSize*1024*1024, 0, NULL,
			   &rhdDRI->agpMemHandle)) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[agp] Out of memory (%d)\n", ret);
	drmAgpRelease(rhdDRI->drmFD);
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] %d kB allocated with handle 0x%08x\n",
	       rhdDRI->gartSize*1024, rhdDRI->agpMemHandle);

    if (drmAgpBind(rhdDRI->drmFD,
		   rhdDRI->agpMemHandle, rhdDRI->gartOffset) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[agp] Could not bind\n");
	drmAgpFree(rhdDRI->drmFD, rhdDRI->agpMemHandle);
	drmAgpRelease(rhdDRI->drmFD);
	return FALSE;
    }

    if (drmAddMap(rhdDRI->drmFD, rhdDRI->ringStart, rhdDRI->ringMapSize,
		  DRM_AGP, DRM_READ_ONLY, &rhdDRI->ringHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not add ring mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] ring handle = 0x%08x\n", rhdDRI->ringHandle);

    if (drmMap(rhdDRI->drmFD, rhdDRI->ringHandle, rhdDRI->ringMapSize,
	       &rhdDRI->ring) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[agp] Could not map ring\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] Ring mapped at 0x%08lx\n",
	       (unsigned long)rhdDRI->ring);

    if (drmAddMap(rhdDRI->drmFD, rhdDRI->ringReadOffset, rhdDRI->ringReadMapSize,
		  DRM_AGP, DRM_READ_ONLY, &rhdDRI->ringReadPtrHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not add ring read ptr mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
 	       "[agp] ring read ptr handle = 0x%08x\n",
	       rhdDRI->ringReadPtrHandle);

    if (drmMap(rhdDRI->drmFD, rhdDRI->ringReadPtrHandle, rhdDRI->ringReadMapSize,
	       &rhdDRI->ringReadPtr) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not map ring read ptr\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] Ring read ptr mapped at 0x%08lx\n",
	       (unsigned long)rhdDRI->ringReadPtr);

    if (drmAddMap(rhdDRI->drmFD, rhdDRI->bufStart, rhdDRI->bufMapSize,
		  DRM_AGP, 0, &rhdDRI->bufHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not add vertex/indirect buffers mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
 	       "[agp] vertex/indirect buffers handle = 0x%08x\n",
	       rhdDRI->bufHandle);

    if (drmMap(rhdDRI->drmFD, rhdDRI->bufHandle, rhdDRI->bufMapSize,
	       &rhdDRI->buf) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not map vertex/indirect buffers\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] Vertex/indirect buffers mapped at 0x%08lx\n",
	       (unsigned long)rhdDRI->buf);

    if (drmAddMap(rhdDRI->drmFD, rhdDRI->gartTexStart, rhdDRI->gartTexMapSize,
		  DRM_AGP, 0, &rhdDRI->gartTexHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not add GART texture map mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
 	       "[agp] GART texture map handle = 0x%08x\n",
	       rhdDRI->gartTexHandle);

    if (drmMap(rhdDRI->drmFD, rhdDRI->gartTexHandle, rhdDRI->gartTexMapSize,
	       &rhdDRI->gartTex) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not map GART texture map\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] GART Texture map mapped at 0x%08lx\n",
	       (unsigned long)rhdDRI->gartTex);

    RHDSetAgpBase(rhdDRI, pScreen);

    return TRUE;
}

/* Initialize the PCI GART state.  Request memory for use in PCI space,
 * and initialize the Radeon registers to point to that memory. */
static Bool RHDDRIPciInit(struct rhdDri * rhdDRI, ScreenPtr pScreen)
{
    int  ret;
    int  flags = DRM_READ_ONLY | DRM_LOCKED | DRM_KERNEL;

    RHDFUNC(rhdDRI);

    ret = drmScatterGatherAlloc(rhdDRI->drmFD, rhdDRI->gartSize*1024*1024,
				&rhdDRI->pciMemHandle);
    if (ret < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[pci] Out of memory (%d)\n", ret);
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] %d kB allocated with handle 0x%08x\n",
	       rhdDRI->gartSize*1024, rhdDRI->pciMemHandle);

    RHDDRIInitGARTValues(rhdDRI);

    if (drmAddMap(rhdDRI->drmFD, rhdDRI->ringStart, rhdDRI->ringMapSize,
		  DRM_SCATTER_GATHER, flags, &rhdDRI->ringHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not add ring mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] ring handle = 0x%08x\n", rhdDRI->ringHandle);

    if (drmMap(rhdDRI->drmFD, rhdDRI->ringHandle, rhdDRI->ringMapSize,
	       &rhdDRI->ring) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[pci] Could not map ring\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] Ring mapped at 0x%08lx\n",
	       (unsigned long)rhdDRI->ring);
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] Ring contents 0x%08lx\n",
	       *(unsigned long *)(pointer)rhdDRI->ring);

    if (drmAddMap(rhdDRI->drmFD, rhdDRI->ringReadOffset, rhdDRI->ringReadMapSize,
		  DRM_SCATTER_GATHER, flags, &rhdDRI->ringReadPtrHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not add ring read ptr mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
 	       "[pci] ring read ptr handle = 0x%08x\n",
	       rhdDRI->ringReadPtrHandle);

    if (drmMap(rhdDRI->drmFD, rhdDRI->ringReadPtrHandle, rhdDRI->ringReadMapSize,
	       &rhdDRI->ringReadPtr) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not map ring read ptr\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] Ring read ptr mapped at 0x%08lx\n",
	       (unsigned long)rhdDRI->ringReadPtr);
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] Ring read ptr contents 0x%08lx\n",
	       *(unsigned long *)(pointer)rhdDRI->ringReadPtr);

    if (drmAddMap(rhdDRI->drmFD, rhdDRI->bufStart, rhdDRI->bufMapSize,
		  DRM_SCATTER_GATHER, 0, &rhdDRI->bufHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not add vertex/indirect buffers mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
 	       "[pci] vertex/indirect buffers handle = 0x%08x\n",
	       rhdDRI->bufHandle);

    if (drmMap(rhdDRI->drmFD, rhdDRI->bufHandle, rhdDRI->bufMapSize,
	       &rhdDRI->buf) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not map vertex/indirect buffers\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] Vertex/indirect buffers mapped at 0x%08lx\n",
	       (unsigned long)rhdDRI->buf);
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] Vertex/indirect buffers contents 0x%08lx\n",
	       *(unsigned long *)(pointer)rhdDRI->buf);

    if (drmAddMap(rhdDRI->drmFD, rhdDRI->gartTexStart, rhdDRI->gartTexMapSize,
		  DRM_SCATTER_GATHER, 0, &rhdDRI->gartTexHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not add GART texture map mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
 	       "[pci] GART texture map handle = 0x%08x\n",
	       rhdDRI->gartTexHandle);

    if (drmMap(rhdDRI->drmFD, rhdDRI->gartTexHandle, rhdDRI->gartTexMapSize,
	       &rhdDRI->gartTex) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not map GART texture map\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] GART Texture map mapped at 0x%08lx\n",
	       (unsigned long)rhdDRI->gartTex);

    return TRUE;
}

/* Add a map for the MMIO registers that will be accessed by any
 * DRI-based clients. */
static Bool RHDDRIMapInit(RHDPtr rhdPtr, ScreenPtr pScreen)
{
    struct rhdDri *rhdDRI = rhdPtr->dri;

    RHDFUNC(rhdPtr);

    /* Map registers */
    if (drmAddMap(rhdDRI->drmFD,
		  rhdPtr->MMIOPCIAddress, rhdPtr->MMIOMapSize,
		  DRM_REGISTERS, DRM_READ_ONLY, &rhdDRI->registerHandle) < 0) {
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[drm] register handle = 0x%08x\n", rhdDRI->registerHandle);

    return TRUE;
}

/* Initialize the kernel data structures */
static int RHDDRIKernelInit(RHDPtr rhdPtr, ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn  = xf86Screens[pScreen->myNum];
    struct rhdDri *rhdDRI   = rhdPtr->dri;
    int            bytesPerPixel = pScrn->bitsPerPixel / 8;
    drm_radeon_init_t drmInfo;

    RHDFUNC(rhdDRI);

    memset(&drmInfo, 0, sizeof(drm_radeon_init_t));
    if (rhdPtr->ChipSet >= RHD_R600)
	drmInfo.func             = RADEON_INIT_R600_CP;
    else
	drmInfo.func             = RADEON_INIT_R300_CP;

    drmInfo.sarea_priv_offset   = sizeof(XF86DRISAREARec);
    drmInfo.is_pci              = (rhdPtr->cardType != RHD_CARD_AGP);
    drmInfo.cp_mode             = R5XX_CSQ_PRIBM_INDBM;
    drmInfo.gart_size           = rhdDRI->gartSize*1024*1024;
    drmInfo.ring_size           = rhdDRI->ringSize*1024*1024;
    drmInfo.usec_timeout        = RHD_DEFAULT_CP_TIMEOUT;

    drmInfo.fb_bpp              = PIXEL_CODE(pScrn);
    drmInfo.depth_bpp           = (rhdDRI->depthBits - 8) * 2;

    drmInfo.front_offset        = rhdDRI->frontOffset;
    drmInfo.front_pitch         = rhdDRI->frontPitch * bytesPerPixel;
    drmInfo.back_offset         = rhdDRI->backOffset;
    drmInfo.back_pitch          = rhdDRI->backPitch * bytesPerPixel;
    drmInfo.depth_offset        = rhdDRI->depthOffset;
    drmInfo.depth_pitch         = rhdDRI->depthPitch * drmInfo.depth_bpp / 8;

    drmInfo.ring_offset         = rhdDRI->ringHandle;
    drmInfo.ring_rptr_offset    = rhdDRI->ringReadPtrHandle;
    drmInfo.buffers_offset      = rhdDRI->bufHandle;
    drmInfo.gart_textures_offset= rhdDRI->gartTexHandle;

    /* Make sure the MC has been set up before DRM_RADEON_CP_INIT is called */
    ASSERT((RHD_CHECKDEBUGFLAG(rhdPtr, MC_SETUP)));

    if (drmCommandWrite(rhdDRI->drmFD, DRM_RADEON_CP_INIT,
			&drmInfo, sizeof(drm_radeon_init_t)) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[dri] CP_INIT failed\n");
	return FALSE;
    }

    // FIXME: this is to be moved to rhd_cp
    /* DRM_RADEON_CP_INIT does an engine reset, which resets some engine
     * registers back to their default values, so we need to restore
     * those engine register here. */
//    R5xx2DSetup(pScrn);

    return TRUE;
}

static void RHDDRIGartHeapInit(struct rhdDri * rhdDRI, ScreenPtr pScreen)
{
    drm_radeon_mem_init_heap_t drmHeap;

    RHDFUNC(rhdDRI);

    /* Start up the simple memory manager for GART space */
    drmHeap.region = RADEON_MEM_REGION_GART;
    drmHeap.start  = 0;
    drmHeap.size   = rhdDRI->gartTexMapSize;

    if (drmCommandWrite(rhdDRI->drmFD, DRM_RADEON_INIT_HEAP,
			&drmHeap, sizeof(drmHeap))) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[drm] Failed to initialize GART heap manager\n");
    } else {
	xf86DrvMsg(pScreen->myNum, X_INFO,
		   "[drm] Initialized kernel GART heap manager, %d\n",
		   rhdDRI->gartTexMapSize);
    }
}

/* Add a map for the vertex buffers that will be accessed by any
 * DRI-based clients. */
static Bool RHDDRIBufInit(RHDPtr rhdPtr, ScreenPtr pScreen)
{
    struct rhdDri *rhdDRI = rhdPtr->dri;

    RHDFUNC(rhdPtr);

    /* Initialize vertex buffers */
    rhdDRI->bufNumBufs = drmAddBufs(rhdDRI->drmFD,
				  rhdDRI->bufMapSize / RADEON_BUFFER_SIZE,
				  RADEON_BUFFER_SIZE,
				  (rhdPtr->cardType != RHD_CARD_AGP) ? DRM_SG_BUFFER : DRM_AGP_BUFFER,
				  rhdDRI->bufStart);

    if (rhdDRI->bufNumBufs <= 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[drm] Could not create vertex/indirect buffers list\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[drm] Added %d %d byte vertex/indirect buffers\n",
	       rhdDRI->bufNumBufs, RADEON_BUFFER_SIZE);

    if (!(rhdDRI->buffers = drmMapBufs(rhdDRI->drmFD))) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[drm] Failed to map vertex/indirect buffers list\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[drm] Mapped %d vertex/indirect buffers\n",
	       rhdDRI->buffers->count);

    return TRUE;
}

static void RHDDRIIrqInit(RHDPtr rhdPtr, ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86Screens[pScreen->myNum];
    struct rhdDri *rhdDRI  = rhdPtr->dri;

    RHDFUNC(rhdPtr);

    if (!rhdDRI->irq) {
	rhdDRI->irq = drmGetInterruptFromBusID(
	    rhdDRI->drmFD,
	    PCI_BUS(rhdPtr->PciInfo),
	    PCI_DEV(rhdPtr->PciInfo),
	    PCI_FUNC(rhdPtr->PciInfo));

	if ((drmCtlInstHandler(rhdDRI->drmFD, rhdDRI->irq)) != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "[drm] failure adding irq handler, "
		       "there is a device already using that irq\n"
		       "[drm] falling back to irq-free operation\n");
	    rhdDRI->irq = 0;
	} else {
// FIXME
//	    rhdDRI->ModeReg->gen_int_cntl = RHDRegRead (rhdDRI,  RADEON_GEN_INT_CNTL );
	}
    }

    if (rhdDRI->irq)
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "[drm] dma control initialized, using IRQ %d\n",
		   rhdDRI->irq);
}

/*
 * Get the DRM version and do some basic useability checks of DRI
 */
static Bool
RHDDRIVersionCheck(RHDPtr rhdPtr)
{
    drmVersionPtr  DrmVersion = NULL;
    int            major, minor, patch, fd;
    char           *busId;

    RHDFUNC(rhdPtr);

    /* Check that the GLX, DRI, and DRM modules have been loaded by testing
     * for known symbols in each module. */
    if (!xf86LoaderCheckSymbol("GlxSetVisualConfigs")) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
		   "%s: symbol GlxSetVisualConfigs not available.\n", __func__);
	return FALSE;
    }

    if (!xf86LoaderCheckSymbol("drmAvailable")) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
		   "%s: symbol drmAvailable not available.\n", __func__);
	return FALSE;
    }

    if (!xf86LoaderCheckSymbol("DRIQueryVersion")) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
		   "%s: symbol DRIQueryVersion not available."
		   "(libdri.a is too old)\n", __func__);
	return FALSE;
    }

    /* Check the DRI version */
    DRIQueryVersion(&major, &minor, &patch);
    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO,
	       "Found libdri %d.%d.%d.\n", major, minor, patch);

    if (major != DRIINFO_MAJOR_VERSION) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
		   "%s: Version Mismatch: libdri >= %d.0.0 is needed.\n",
		   __func__, DRIINFO_MAJOR_VERSION);
	return FALSE;
    }

    /* Create a bus Id */
    if (xf86LoaderCheckSymbol("DRICreatePCIBusID")) {
	busId = DRICreatePCIBusID(rhdPtr->PciInfo);
    } else {
	busId = xalloc(64);
	sprintf(busId, "PCI:%d:%d:%d", PCI_BUS(rhdPtr->PciInfo),
		PCI_DEV(rhdPtr->PciInfo), PCI_FUNC(rhdPtr->PciInfo));
    }

    /* Low level DRM open */
    fd = drmOpen(dri_driver_name, busId);
    if (fd < 0) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
		   "%s: drmOpen(\"%s\", \"%s\") failed.\n",
		   __func__, dri_driver_name, busId);
	xfree(busId);
	return FALSE;
    }
    xfree(busId);

    /* Check the lib version */
    if (xf86LoaderCheckSymbol("drmGetLibVersion"))
	DrmVersion = drmGetLibVersion(fd);
    if (DrmVersion == NULL) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
		   "%s: drmGetLibVersion failed.\n", __func__);
	drmClose(fd);
	return FALSE;
    }

    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "Found libdrm %d.%d.%d.\n",
	       DrmVersion->version_major, DrmVersion->version_minor,
	       DrmVersion->version_patchlevel);

    if ((DrmVersion->version_major != 1) ||
	((DrmVersion->version_major == 1) && (DrmVersion->version_minor < 2))) {
	/* incompatible drm library version */
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Version Mismatch: "
		   "libdrm >= 1.2.0 is needed.\n", __func__);
	drmFreeVersion(DrmVersion);
	drmClose(fd);
	return FALSE;
    }
    drmFreeVersion(DrmVersion);
    DrmVersion = NULL;

    /* Get DRM version & close DRM */
    DrmVersion = drmGetVersion(fd);
    drmClose(fd);
    if (DrmVersion == NULL) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: drmGetVersion failed.\n",
		   __func__);
	return FALSE;
    }

    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "Found radeon drm %d.%d.%d.\n",
	       DrmVersion->version_major, DrmVersion->version_minor,
	       DrmVersion->version_patchlevel);

    /* Incompatible drm version */
    if ((DrmVersion->version_major < 1) ||
	((DrmVersion->version_major == 1) && (DrmVersion->version_minor < 28))) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Version Mismatch: "
		   "radeon drm >= 1.28.0 is needed.\n", __func__);
	drmFreeVersion(DrmVersion);
	return FALSE;
    }

    drmFreeVersion(DrmVersion);
    return TRUE;
}

static Bool RHDDRISetVBlankInterrupt(ScrnInfoPtr pScrn, Bool on)
{
#ifdef RADEON_SETPARAM_VBLANK_CRTC
    struct rhdDri *  rhdDRI    = RHDPTR(pScrn)->dri;
    int value = 0;

    RHDFUNC(rhdDRI);

    if (rhdDRI->irq) {
        if (on) {
#ifdef RANDR_12_SUPPORT		// FIXME check / move to rhd_randr.c
	    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
  	    if (xf86_config->num_crtc > 1 && xf86_config->crtc[1]->enabled)
	        value = DRM_RADEON_VBLANK_CRTC1 | DRM_RADEON_VBLANK_CRTC2;
	    else
#endif
	        value = DRM_RADEON_VBLANK_CRTC1;
	}

	if (RHDDRISetParam(pScrn, RADEON_SETPARAM_VBLANK_CRTC, value)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "RHD Vblank Crtc Setup Failed %d\n", value);
	    return FALSE;
	}
    }
#endif
    return TRUE;
}

/* PreInit */
Bool RHDDRIPreInit(ScrnInfoPtr pScrn)
{
    RHDPtr         rhdPtr = RHDPTR(pScrn);
    struct rhdDri *rhdDRI;
    int pixel_code = PIXEL_CODE(pScrn);

    RHDFUNC(rhdPtr);

    rhdPtr->directRenderingEnabled = FALSE;

    if (!rhdPtr->useDRI.val.bool) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Direct rendering explicitly turned off.\n");
	return FALSE;
    }

    if (xf86IsEntityShared(rhdPtr->pEnt->index)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Direct Rendering Disabled -- "
		   "Dual-head configuration is not working with "
		   "DRI at present.\n"
		   "Please use a RandR merged framebuffer setup if you "
		   "want Dual-head with DRI.\n");
	return FALSE;
    }

    if (rhdPtr->ChipSet >= RHD_R600) {
	if (rhdPtr->useDRI.set && rhdPtr->useDRI.val.bool) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Direct rendering for R600 and up forced on - "
		       "This is NOT officially supported yet "
		       "and may cause instability or lockups\n");
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Direct rendering not officially supported on R600 and up\n");
	    return FALSE;
	}
    }

    if (!RHDDRIVersionCheck(rhdPtr)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "%s: Version check failed. Disabling DRI.\n", __func__);
	return FALSE;
    }

    rhdDRI = xnfcalloc(1, sizeof(struct rhdDri));
    rhdDRI->scrnIndex = rhdPtr->scrnIndex;
    rhdPtr->dri = rhdDRI;

    rhdDRI->gartSize      = RHD_DEFAULT_GART_SIZE;
    rhdDRI->ringSize      = RHD_DEFAULT_RING_SIZE;
    rhdDRI->bufSize       = RHD_DEFAULT_BUFFER_SIZE;

    rhdDRI->drmFD         = -1;

    rhdDRI->gartLocation  = 0;

#if 0
    if ((xf86GetOptValInteger(rhdPtr->Options,
			      OPTION_GART_SIZE, (int *)&(rhdDRI->gartSize))) ||
	(xf86GetOptValInteger(rhdPtr->Options,
			      OPTION_GART_SIZE_OLD, (int *)&(rhdDRI->gartSize)))) {
	switch (rhdDRI->gartSize) {
	case 4:
	case 8:
	case 16:
	case 32:
	case 64:
	case 128:
	case 256:
	    break;

	default:
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Illegal GART size: %d MB\n", rhdDRI->gartSize);
	    xfree(rhdDRI);
	    return NULL;
	}
    }
#endif

    rhdDRI->gartTexSize = rhdDRI->gartSize - (rhdDRI->ringSize + rhdDRI->bufSize);
    radeon_drm_page_size = getpagesize();

    /* Only 16 and 32 color depths are supports currently. */
    if (pixel_code != 16 && pixel_code != 32) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "[dri] RHDInitVisualConfigs failed "
		   "(depth %d not supported).  "
		   "Disabling DRI.\n", pixel_code);
	xfree(rhdDRI);
	rhdPtr->dri = NULL;
	return FALSE;
    }

    /* Currently 32bpp pixel buffer implies 32bpp depth(+stencil).
     * Same for 16bpp. */
    rhdDRI->depthBits = pScrn->depth;

    if (rhdPtr->AccelMethod == RHD_ACCEL_SHADOWFB) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "DRI conflicts with SHADOWFB\n");
	rhdPtr->AccelMethod = RHD_ACCEL_NONE;
    }

    return TRUE;
}


/* Allocate all framebuffer chunks.
 * ATM this is still static. When this gets dynamic, it doesn't have to occure
 * in PreInit at all any more */
Bool RHDDRIAllocateBuffers(ScrnInfoPtr pScrn)
{
    RHDPtr         rhdPtr  = RHDPTR(pScrn);
    struct rhdDri *rhdDRI    = rhdPtr->dri;
    int            bytesPerPixel      = pScrn->bitsPerPixel / 8;
    int            depthBytesPerPixel = (rhdDRI->depthBits == 24 ? 32 : rhdDRI->depthBits) / 8;
    int            size, depth_size;
    unsigned int   old_freeoffset, old_freesize;

    RHDFUNC(rhdPtr);

    size = pScrn->displayWidth * bytesPerPixel * pScrn->virtualY;

    old_freeoffset = rhdPtr->FbFreeStart;
    old_freesize   = rhdPtr->FbFreeSize;

    rhdDRI->frontPitch  = pScrn->displayWidth;
    rhdDRI->frontOffset = rhdPtr->FbScanoutStart;

    rhdDRI->backPitch   = pScrn->displayWidth;
    rhdDRI->backOffset  = RHDAllocFb(rhdPtr, size, "DRI Back Buffer");

    /* Due to tiling, the Z buffer pitch must be a multiple of 32 pixels,
     * which is always the case if color tiling is used due to color pitch
     * but not necessarily otherwise, and its height a multiple of 16 lines. */
    rhdDRI->depthPitch  = ALIGN(pScrn->displayWidth,32);
    depth_size = ALIGN(pScrn->virtualY, 16) * rhdDRI->depthPitch
                 * depthBytesPerPixel;
    rhdDRI->depthOffset = RHDAllocFb(rhdPtr, depth_size, "DRI Depth Buffer");
    if (rhdDRI->backOffset == -1 || rhdDRI->depthOffset == -1) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
		   "DRI: Failed allocating buffers, disabling\n");
	rhdPtr->FbFreeStart = old_freeoffset;
	rhdPtr->FbFreeSize  = old_freesize;

	/* so far we are called from PreInit(): if we fail we free the DRI struct */
	xfree(rhdPtr->dri);
	rhdPtr->dri = NULL;
	return FALSE;
    }

    RHDDRIAllocatePCIGARTTable(pScrn);

    rhdDRI->textureSize = rhdPtr->FbFreeSize;
    if (rhdDRI->textureSize > 0) {
	int l = RHDMinBits((rhdDRI->textureSize-1) / RADEON_NR_TEX_REGIONS);
	if (l < RADEON_LOG_TEX_GRANULARITY)
	    l = RADEON_LOG_TEX_GRANULARITY;
	/* Round the texture size up to the nearest whole number of
	 * texture regions.  Again, be greedy about this, don't
	 * round down. */
	rhdDRI->log2TexGran = l;
	rhdDRI->textureSize = (rhdDRI->textureSize >> l) << l;
    } else {
	rhdDRI->textureSize = 0;
    }
    if (rhdDRI->textureSize < 512 * 1024)
	/* Minimum texture size is for 2 256x256x32bpp textures */
	rhdDRI->textureSize = 0;

    if (rhdDRI->textureSize > 0) {
	rhdDRI->textureOffset = RHDAllocFb(rhdPtr, rhdDRI->textureSize,
					 "DRI Textures");
	ASSERT(rhdDRI->textureOffset != -1);
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d MB GART aperture\n", rhdDRI->gartSize);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d MB for the ring buffer\n", rhdDRI->ringSize);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d MB for vertex/indirect buffers\n", rhdDRI->bufSize);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d MB for GART textures\n", rhdDRI->gartTexSize);

    return TRUE;
}


/* Initialize the screen-specific data structures for the DRI and the
 * Radeon.  This is the main entry point to the device-specific
 * initialization code.  It calls device-independent DRI functions to
 * create the DRI data structures and initialize the DRI state. */
Bool RHDDRIScreenInit(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn   = xf86Screens[pScreen->myNum];
    RHDPtr         rhdPtr  = RHDPTR(pScrn);
    struct rhdDri *rhdDRI    = rhdPtr->dri;
    DRIInfoPtr     pDRIInfo;
    RADEONDRIPtr   pRADEONDRI;

    RHDFUNC(rhdPtr);

    /* Create the DRI data structure, and fill it in before calling the
     * DRIScreenInit(). */
    if (!(pDRIInfo = DRICreateInfoRec())) return FALSE;

    rhdDRI->pDRIInfo                       = pDRIInfo;
    pDRIInfo->drmDriverName              = dri_driver_name;

    if (rhdPtr->ChipSet >= RHD_R600)
       pDRIInfo->clientDriverName        = r600_driver_name;
    else
       pDRIInfo->clientDriverName        = r300_driver_name;

    if (xf86LoaderCheckSymbol("DRICreatePCIBusID")) {
	pDRIInfo->busIdString = DRICreatePCIBusID(rhdPtr->PciInfo);
    } else {
	pDRIInfo->busIdString            = xalloc(64);
	sprintf(pDRIInfo->busIdString,
		"PCI:%d:%d:%d",
		PCI_BUS(rhdPtr->PciInfo),
		PCI_DEV(rhdPtr->PciInfo),
		PCI_FUNC(rhdPtr->PciInfo));
    }
    pDRIInfo->ddxDriverMajorVersion      = /* TODO rhdDRI->allowColorTiling ?
    				RADEON_VERSION_MAJOR_TILED : */ RADEON_DRIAPI_VERSION_MAJOR;
    pDRIInfo->ddxDriverMinorVersion      = RADEON_DRIAPI_VERSION_MINOR;
    pDRIInfo->ddxDriverPatchVersion      = RADEON_DRIAPI_VERSION_PATCH;
    pDRIInfo->frameBufferPhysicalAddress = (void *) (size_t) rhdPtr->FbPCIAddress;

    pDRIInfo->frameBufferSize            = rhdPtr->FbFreeStart + rhdPtr->FbFreeSize;
    pDRIInfo->frameBufferStride          = (pScrn->displayWidth *
					    pScrn->bitsPerPixel / 8);
    pDRIInfo->ddxDrawableTableEntry      = RADEON_MAX_DRAWABLES;
    pDRIInfo->maxDrawableTableEntry      = (SAREA_MAX_DRAWABLES
					    < RADEON_MAX_DRAWABLES
					    ? SAREA_MAX_DRAWABLES
					    : RADEON_MAX_DRAWABLES);
    /* kill DRIAdjustFrame. We adjust sarea frame info ourselves to work
       correctly with pageflip + mergedfb/color tiling */
    pDRIInfo->wrap.AdjustFrame = NULL;

    /* For now the mapping works by using a fixed size defined
     * in the SAREA header */
    if (sizeof(XF86DRISAREARec)+sizeof(drm_radeon_sarea_t) > SAREA_MAX) {
	ErrorF("Data does not fit in SAREA\n");
	RHDDRICloseScreen(pScreen);
	return FALSE;
    }
    pDRIInfo->SAREASize = SAREA_MAX;

    if (!(pRADEONDRI = (RADEONDRIPtr)xcalloc(sizeof(RADEONDRIRec),1))) {
	RHDDRICloseScreen(pScreen);
	return FALSE;
    }

    pDRIInfo->devPrivate     = pRADEONDRI;
    pDRIInfo->devPrivateSize = sizeof(RADEONDRIRec);
    pDRIInfo->contextSize    = sizeof(RADEONDRIContextRec);

    pDRIInfo->CreateContext  = RHDCreateContext;
    pDRIInfo->DestroyContext = RHDDestroyContext;
    pDRIInfo->SwapContext    = RHDDRISwapContext;
    pDRIInfo->InitBuffers    = RHDDRIInitBuffers;
    pDRIInfo->MoveBuffers    = RHDDRIMoveBuffers;
    pDRIInfo->bufferRequests = DRI_ALL_WINDOWS;
    pDRIInfo->TransitionTo2d = RHDDRITransitionTo2d;
    pDRIInfo->TransitionTo3d = RHDDRITransitionTo3d;
    pDRIInfo->TransitionSingleToMulti3D = RHDDRITransitionSingleToMulti3d;
    pDRIInfo->TransitionMultiToSingle3D = RHDDRITransitionMultiToSingle3d;

    pDRIInfo->createDummyCtx     = TRUE;
    pDRIInfo->createDummyCtxPriv = FALSE;

    if (!DRIScreenInit(pScreen, pDRIInfo, &rhdDRI->drmFD)) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[dri] DRIScreenInit failed.  Disabling DRI.\n");
	RHDDRICloseScreen(pScreen);
	return FALSE;
    }

    /* Initialize AGP */
    if (rhdPtr->cardType == RHD_CARD_AGP &&
	!RHDDRIAgpInit(rhdDRI, pScreen)) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] AGP failed to initialize. Disabling the DRI.\n" );
	xf86DrvMsg(pScreen->myNum, X_INFO,
		   "[agp] You may want to make sure the agpgart kernel "
		   "module\nis loaded before the radeon kernel module.\n");
	RHDDRICloseScreen(pScreen);
	return FALSE;
    }

    /* Initialize PCI */
    if (rhdPtr->cardType != RHD_CARD_AGP &&
	!RHDDRIPciInit(rhdDRI, pScreen)) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] PCI failed to initialize. Disabling the DRI.\n" );
	RHDDRICloseScreen(pScreen);
	return FALSE;
    }

    /* DRIScreenInit doesn't add all the common mappings.  Add additional
     * mappings here. */
    if (!RHDDRIMapInit(rhdPtr, pScreen)) {
	RHDDRICloseScreen(pScreen);
	return FALSE;
    }

    /* FIXME: When are these mappings unmapped? */
    if (!RHDInitVisualConfigs(pScreen)) {
	RHDDRICloseScreen(pScreen);
	return FALSE;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] Visual configs initialized\n");

    /* Tell DRI about new memory map */
    if (RHDDRISetParam(pScrn, RADEON_SETPARAM_NEW_MEMMAP, 1) < 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "[drm] failed to enable new memory map\n");
	RHDDRICloseScreen(pScreen);
	return FALSE;
    }

    return TRUE;
}

/*
 *
 */
static int
RHDDRIGARTBaseGet(RHDPtr rhdPtr)
{
    struct rhdDri *Dri = rhdPtr->dri;
    drm_radeon_getparam_t gp;
    int gart_base;

    memset(&gp, 0, sizeof(gp));
    gp.param = RADEON_PARAM_GART_BASE;
    gp.value = &gart_base;

    if (drmCommandWriteRead(Dri->drmFD, DRM_RADEON_GETPARAM, &gp,
			    sizeof(gp)) < 0) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
		   "%s: Failed to determine GART area MC location.\n", __func__);
	return 0;
    } else {
	RHDDebug(rhdPtr->scrnIndex, "GART location: 0x%08X\n", gart_base);
	return gart_base;
    }
}

/* Finish initializing the device-dependent DRI state, and call
 * DRIFinishScreenInit() to complete the device-independent DRI
 * initialization.
 */
Bool RHDDRIFinishScreenInit(ScreenPtr pScreen)
{
    ScrnInfoPtr         pScrn  = xf86Screens[pScreen->myNum];
    RHDPtr              rhdPtr = RHDPTR(pScrn);
    struct rhdDri      *rhdDRI   = rhdPtr->dri;
    drm_radeon_sarea_t *pSAREAPriv;
    RADEONDRIPtr        pRADEONDRI;

    RHDFUNC(rhdPtr);

    if (! rhdDRI)
	return FALSE;
    if (rhdPtr->cardType == RHD_CARD_PCIE)
    {
      if (RHDDRISetParam(pScrn, RADEON_SETPARAM_PCIGART_LOCATION, rhdDRI->pciGartOffset) < 0)
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "[drm] failed set pci gart location\n");

#ifdef RADEON_SETPARAM_PCIGART_TABLE_SIZE
	if (RHDDRISetParam(pScrn, RADEON_SETPARAM_PCIGART_TABLE_SIZE, rhdDRI->pciGartSize) < 0)
	  xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		     "[drm] failed set pci gart table size\n");
#endif
    }
    RHDDebug(pScrn->scrnIndex, "DRI Finishing init !\n");

    rhdDRI->pDRIInfo->driverSwapMethod = DRI_HIDE_X_CONTEXT;

    /* NOTE: DRIFinishScreenInit must be called before *DRIKernelInit
     * because *DRIKernelInit requires that the hardware lock is held by
     * the X server, and the first time the hardware lock is grabbed is
     * in DRIFinishScreenInit. */
    if (!DRIFinishScreenInit(pScreen)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "%s: DRIFinishScreenInit Failed.\n", __func__);
	return RHDDRICloseScreen(pScreen);
    }

    /* Initialize the kernel data structures */
    if (!RHDDRIKernelInit(rhdPtr, pScreen)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "%s: RHDDRIKernelInit Failed.\n", __func__);
	return RHDDRICloseScreen(pScreen);
    }

    /* Initialize the vertex buffers list */
    if (!RHDDRIBufInit(rhdPtr, pScreen))
	return RHDDRICloseScreen(pScreen);

    /* Initialize IRQ */
    RHDDRIIrqInit(rhdPtr, pScreen);

    /* Initialize kernel GART memory manager */
    RHDDRIGartHeapInit(rhdDRI, pScreen);

    /* Initialize the SAREA private data structure */
    pSAREAPriv = (drm_radeon_sarea_t *)DRIGetSAREAPrivate(pScreen);
    memset(pSAREAPriv, 0, sizeof(*pSAREAPriv));

    pRADEONDRI                    = (RADEONDRIPtr)rhdDRI->pDRIInfo->devPrivate;

    pRADEONDRI->deviceID          = rhdPtr->PciDeviceID;
    pRADEONDRI->width             = pScrn->virtualX;
    pRADEONDRI->height            = pScrn->virtualY;
    pRADEONDRI->depth             = pScrn->depth;
    pRADEONDRI->bpp               = pScrn->bitsPerPixel;

    pRADEONDRI->IsPCI             = (rhdPtr->cardType != RHD_CARD_AGP);
    pRADEONDRI->AGPMode           = rhdDRI->agpMode;

    pRADEONDRI->frontOffset       = rhdDRI->frontOffset;
    pRADEONDRI->frontPitch        = rhdDRI->frontPitch;
    pRADEONDRI->backOffset        = rhdDRI->backOffset;
    pRADEONDRI->backPitch         = rhdDRI->backPitch;
    pRADEONDRI->depthOffset       = rhdDRI->depthOffset;
    pRADEONDRI->depthPitch        = rhdDRI->depthPitch;
    pRADEONDRI->textureOffset     = rhdDRI->textureOffset;
    pRADEONDRI->textureSize       = rhdDRI->textureSize;
    pRADEONDRI->log2TexGran       = rhdDRI->log2TexGran;

    pRADEONDRI->registerHandle    = rhdDRI->registerHandle;
    pRADEONDRI->registerSize      = rhdPtr->MMIOMapSize;

    pRADEONDRI->statusHandle      = rhdDRI->ringReadPtrHandle;
    pRADEONDRI->statusSize        = rhdDRI->ringReadMapSize;

    pRADEONDRI->gartTexHandle     = rhdDRI->gartTexHandle;
    pRADEONDRI->gartTexMapSize    = rhdDRI->gartTexMapSize;
    pRADEONDRI->log2GARTTexGran   = rhdDRI->log2GARTTexGran;
    pRADEONDRI->gartTexOffset     = rhdDRI->gartTexStart;

    pRADEONDRI->sarea_priv_offset = sizeof(XF86DRISAREARec);

    /* disable vblank at startup */
    RHDDRISetVBlankInterrupt (pScrn, FALSE);

    /* TODO: If RADEON_PARAM_GART_BASE is ever to be saved/restored, it has
     * to be updated here. Same on EnterVT. */

    rhdDRI->gartLocation = RHDDRIGARTBaseGet(rhdPtr);

    /* TODO: call drm's DRM_RADEON_GETPARAM in the EXA case, for accelerated
     * DownloadFromScreen hook. Same on EnterVT. */

    /* TODO: display buffer watermark calculation
     * (radeon: legacy_crtc.c/InitDispBandwidth)
     * Don't know the registers for r5xx & r6xx yet.
     * Probably should be in _driver.c anyway. */
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Direct rendering enabled\n");

    return TRUE;
}

/* Reinit after vt switch / resume */
void RHDDRIEnterVT(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn   = xf86Screens[pScreen->myNum];
    RHDPtr         rhdPtr  = RHDPTR(pScrn);
    struct rhdDri *rhdDRI    = rhdPtr->dri;
    int            ret;

    RHDFUNC(rhdPtr);

    if (rhdDRI->drmFD == -1)
	return;

    if (rhdPtr->cardType == RHD_CARD_AGP) {
	if (!RHDSetAgpMode(rhdDRI, pScreen))
	    return;
	RHDSetAgpBase(rhdDRI, pScreen);
    }

    /* TODO: maybe using CP_INIT instead of CP_RESUME is enough, so we wouldn't
     * need an additional copy of the GART table in main memory. OTOH the table
     * must be initialized but not allocated anew. */
    /* Restore the PCIE GART TABLE */
    if (rhdDRI->pciGartBackup)
	memcpy((char *)rhdPtr->FbBase + rhdDRI->pciGartOffset,
	       rhdDRI->pciGartBackup, rhdDRI->pciGartSize);

    RHDDRISetVBlankInterrupt(pScrn, rhdDRI->have3Dwindows);

    if ( (ret = drmCommandNone(rhdDRI->drmFD, DRM_RADEON_CP_RESUME)) )
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "%s: CP resume %d\n", __func__, ret);
}

/* Stop all before vt switch / suspend */
void RHDDRILeaveVT(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn  = xf86Screens[pScreen->myNum];
    RHDPtr         rhdPtr = RHDPTR(pScrn);
    struct rhdDri *rhdDRI   = rhdPtr->dri;

    RHDFUNC(rhdPtr);

    if (rhdDRI->drmFD == -1)
	return;

    RHDDRISetVBlankInterrupt (pScrn, FALSE);
    DRILock(pScrn->pScreen, 0);

    /* Backup the PCIE GART TABLE from fb memory */
    if (rhdDRI->pciGartBackup)
	memcpy(rhdDRI->pciGartBackup,
	       (char*)rhdPtr->FbBase + rhdDRI->pciGartOffset, rhdDRI->pciGartSize);

    /* Make sure 3D clients will re-upload textures to video RAM */
    if (rhdDRI->textureSize) {
	drm_radeon_sarea_t *pSAREAPriv = DRIGetSAREAPrivate(pScreen);
	struct drm_tex_region *list = pSAREAPriv->tex_list[0];
	int age = ++pSAREAPriv->tex_age[0], i = 0;
	do {
	    list[i].age = age;
	    i = list[i].next;
	} while (i != 0);
    }
}

/* The screen is being closed, so clean up any state and free any
 * resources used by the DRI. */
/* This is Bool and returns FALSE always to ease cleanup */
Bool RHDDRICloseScreen(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn  = xf86Screens[pScreen->myNum];
    RHDPtr         rhdPtr = RHDPTR(pScrn);
    struct rhdDri *rhdDRI   = rhdPtr->dri;
    //drm_radeon_init_t drmInfo;
    drm_radeon_init_t drmInfo;

    RHDFUNC(pScrn);

    if (rhdDRI->irq) {
	RHDDRISetVBlankInterrupt (pScrn, FALSE);
	drmCtlUninstHandler(rhdDRI->drmFD);
	rhdDRI->irq = 0;
//	rhdDRI->ModeReg->gen_int_cntl = 0;
    }

    /* invalidate GART location for EXA */
    rhdDRI->gartLocation  = 0;

    /* De-allocate vertex buffers */
    if (rhdDRI->buffers) {
	drmUnmapBufs(rhdDRI->buffers);
	rhdDRI->buffers = NULL;
    }

    /* De-allocate all kernel resources */
    memset(&drmInfo, 0, sizeof(drm_radeon_init_t));
    drmInfo.func = RADEON_CLEANUP_CP;
    drmCommandWrite(rhdDRI->drmFD, DRM_RADEON_CP_INIT,
		    &drmInfo, sizeof(drm_radeon_init_t));

    /* De-allocate all GART resources */
    if (rhdDRI->gartTex) {
	drmUnmap(rhdDRI->gartTex, rhdDRI->gartTexMapSize);
	rhdDRI->gartTex = NULL;
    }
    if (rhdDRI->buf) {
	drmUnmap(rhdDRI->buf, rhdDRI->bufMapSize);
	rhdDRI->buf = NULL;
    }
    if (rhdDRI->ringReadPtr) {
	drmUnmap(rhdDRI->ringReadPtr, rhdDRI->ringReadMapSize);
	rhdDRI->ringReadPtr = NULL;
    }
    if (rhdDRI->ring) {
	drmUnmap(rhdDRI->ring, rhdDRI->ringMapSize);
	rhdDRI->ring = NULL;
    }
    if (rhdDRI->agpMemHandle != DRM_AGP_NO_HANDLE) {
	drmAgpUnbind(rhdDRI->drmFD, rhdDRI->agpMemHandle);
	drmAgpFree(rhdDRI->drmFD, rhdDRI->agpMemHandle);
	rhdDRI->agpMemHandle = DRM_AGP_NO_HANDLE;
	drmAgpRelease(rhdDRI->drmFD);
    }
    if (rhdDRI->pciMemHandle) {
	drmScatterGatherFree(rhdDRI->drmFD, rhdDRI->pciMemHandle);
	rhdDRI->pciMemHandle = 0;
    }
    if (rhdDRI->pciGartBackup) {
	xfree(rhdDRI->pciGartBackup);
	rhdDRI->pciGartBackup = NULL;
    }

    /* De-allocate all DRI resources */
    DRICloseScreen(pScreen);
    rhdDRI->drmFD = -1;

    /* De-allocate all DRI data structures */
    if (rhdDRI->pDRIInfo) {
	if (rhdDRI->pDRIInfo->devPrivate) {
	    xfree(rhdDRI->pDRIInfo->devPrivate);
	    rhdDRI->pDRIInfo->devPrivate = NULL;
	}
	DRIDestroyInfoRec(rhdDRI->pDRIInfo);
	rhdDRI->pDRIInfo = NULL;
    }
    if (rhdDRI->pVisualConfigs) {
	xfree(rhdDRI->pVisualConfigs);
	rhdDRI->pVisualConfigs = NULL;
    }
    if (rhdDRI->pVisualConfigsPriv) {
	xfree(rhdDRI->pVisualConfigsPriv);
	rhdDRI->pVisualConfigsPriv = NULL;
    }

    rhdPtr->directRenderingEnabled = FALSE;

    return TRUE;
}


static void RHDDisablePageFlip(ScreenPtr pScreen)
{
    drm_radeon_sarea_t *  pSAREAPriv = DRIGetSAREAPrivate(pScreen);
    pSAREAPriv->pfState = 0;
}

static void RHDDRITransitionSingleToMulti3d(ScreenPtr pScreen)
{
    RHDDisablePageFlip(pScreen);
}

static void RHDDRITransitionMultiToSingle3d(ScreenPtr pScreen)
{
    /* Let the remaining 3d app start page flipping again */
//    RHDEnablePageFlip(pScreen);
}

static void RHDDRITransitionTo3d(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn  = xf86Screens[pScreen->myNum];
    struct rhdDri *rhdDRI   = RHDPTR(pScrn)->dri;

    rhdDRI->have3Dwindows = TRUE;
//    RHDChangeSurfaces(pScrn);	// FIXME needed for tiling
//    RHDEnablePageFlip(pScreen);

    RHDDRISetVBlankInterrupt(pScrn, TRUE);
}

static void RHDDRITransitionTo2d(ScreenPtr pScreen)
{
    ScrnInfoPtr         pScrn      = xf86Screens[pScreen->myNum];
    struct rhdDri *       rhdDRI       = RHDPTR(pScrn)->dri;
    drm_radeon_sarea_t *  pSAREAPriv = DRIGetSAREAPrivate(pScreen);

    rhdDRI->have3Dwindows = FALSE;

    /* Try flipping back to the front page if necessary */
    if (pSAREAPriv->pfCurrentPage == 1)
	drmCommandNone(rhdDRI->drmFD, DRM_RADEON_FLIP);

    /* Shut down shadowing if we've made it back to the front page */
    if (pSAREAPriv->pfCurrentPage == 0) {
	RHDDisablePageFlip(pScreen);
    } else {
	xf86DrvMsg(pScreen->myNum, X_WARNING,
		   "[dri] RHDDRITransitionTo2d: "
		   "kernel failed to unflip buffers.\n");
    }
//    RHDChangeSurfaces(pScrn);

    RHDDRISetVBlankInterrupt(pScrn, FALSE);
}

static int RHDDRIGetPciAperTableSize(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    int page_size  = getpagesize();
    int ret_size;
    int num_pages;

    num_pages = (RHD_DEFAULT_PCI_APER_SIZE * 1024 * 1024) / page_size;

    if ((rhdPtr->ChipSet >= RHD_R600) ||
	(rhdPtr->ChipSet == RHD_RS600))
	ret_size = num_pages * sizeof(uint64_t);
    else
	ret_size = num_pages * sizeof(unsigned int);

    return ret_size;
}

static void RHDDRIAllocatePCIGARTTable(ScrnInfoPtr pScrn)
{
    RHDPtr         rhdPtr = RHDPTR(pScrn);
    struct rhdDri *rhdDRI   = RHDPTR(pScrn)->dri;

    RHDFUNC(rhdPtr);

    if (rhdPtr->cardType != RHD_CARD_PCIE)
      return;

    rhdDRI->pciGartSize = RHDDRIGetPciAperTableSize(pScrn);

    if (rhdPtr->FbFreeSize < (unsigned) rhdDRI->pciGartSize) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Was not able to reserve %d kb for PCI GART\n",
		   rhdDRI->pciGartSize/1024);
	return;
    }
    /* Allocate at end of FB, so it's not part of the general memory map */
    rhdDRI->pciGartOffset = rhdPtr->FbFreeStart + rhdPtr->FbFreeSize - rhdDRI->pciGartSize;
    rhdPtr->FbFreeSize -= rhdDRI->pciGartSize;
    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO,
	       "FB: Allocated GART table at offset 0x%08X (size = 0x%08X, end of FB)\n",
	       (unsigned int) rhdDRI->pciGartOffset, rhdDRI->pciGartSize);
    rhdDRI->pciGartBackup = xalloc(rhdDRI->pciGartSize);
}

static int RHDDRISetParam(ScrnInfoPtr pScrn, unsigned int param, int64_t value)
{
    drm_radeon_setparam_t  radeonsetparam;
    struct rhdDri *  rhdDRI   = RHDPTR(pScrn)->dri;
    int ret;

    memset(&radeonsetparam, 0, sizeof(drm_radeon_setparam_t));
    radeonsetparam.param = param;
    radeonsetparam.value = value;
    ret = drmCommandWrite(rhdDRI->drmFD, DRM_RADEON_SETPARAM,
			  &radeonsetparam, sizeof(drm_radeon_setparam_t));
    return ret;
}

/*
 *
 */
int
RHDDRMFDGet(int scrnIndex)
{
    struct rhdDri *Dri = RHDPTR(xf86Screens[scrnIndex])->dri;

    if (!Dri)
	return -1;
    else
	return Dri->drmFD;
}

/*
 * Get an indirect buffer for the CP 2D acceleration commands
 */
struct _drmBuf *
RHDDRMCPBuffer(int scrnIndex)
{
    struct rhdDri *Dri = RHDPTR(xf86Screens[scrnIndex])->dri;
    drmDMAReq dma;
    drmBufPtr buf = NULL;
    int indx = 0;
    int size = 0;
    int i;

    /* This is the X server's context */
    dma.context = 0x00000001;
    dma.send_count    = 0;
    dma.send_list     = NULL;
    dma.send_sizes    = NULL;
    dma.flags         = 0;
    dma.request_count = 1;
    dma.request_size  = 64 << 10;
    dma.request_list  = &indx;
    dma.request_sizes = &size;
    dma.granted_count = 0;

    for (i = 0; i < R5XX_LOOP_COUNT; i++) {
	int ret = drmDMA(Dri->drmFD, &dma);
	if (!ret) {
	    buf = &Dri->buffers->list[indx];
	    //xf86DrvMsg(scrnIndex, X_INFO, "%s: index %d, addr %p\n",  __func__, buf->idx, buf->address);
	    buf->used = 0;
	    return buf;
	} else if (ret != -16)
	    xf86DrvMsg(scrnIndex, X_ERROR, "%s: drmDMA returned %d\n",
		       __func__, ret);
    }

    /* There is nothing we can do here... If we fail, we either failed to set
     * up the buffers correctly, or we passed the wrong info here, or we didn't
     * free the buffers correctly. No engine reset will save us, ever. We
     * should just return, and let the X server segfault itself. */
    xf86DrvMsg(scrnIndex, X_ERROR,
	       "%s: throwing in the towel: SIGSEGV ahead!\n", __func__);
    return NULL;
}

/*
 *
 */
CARD8 *
RHDDRMIndirectBufferGet(int scrnIndex, unsigned int *IntAddress, CARD32 *Size)
{
    struct rhdDri *Dri = RHDPTR(xf86Screens[scrnIndex])->dri;
    struct _drmBuf *DrmBuffer;
    CARD8 *Buffer;

    if (!Dri->gartLocation)
	return NULL;

    DrmBuffer = RHDDRMCPBuffer(scrnIndex);
    Buffer = DrmBuffer->address;
    *Size = DrmBuffer->total;
    *IntAddress = Dri->gartLocation + Dri->bufStart + DrmBuffer->idx * DrmBuffer->total;

    return Buffer;
}

/*
 *
 */
void
RHDDRMIndirectBufferDiscard(int scrnIndex, CARD8 *Buffer)
{
    struct rhdDri *Dri = RHDPTR(xf86Screens[scrnIndex])->dri;
    struct drm_radeon_indirect indirect;
    int i;

    for (i = 0; i < Dri->bufNumBufs; i++)
	if (Buffer == Dri->buffers->list[i].address) {
	    /* stick a NOP in our Buffer */
	    ((CARD32 *) Buffer)[0] = CP_PACKET2();

	    indirect.idx = Dri->buffers->list[i].idx;
	    indirect.start = 0;
	    indirect.end = 1;
	    indirect.discard = 1;

	    drmCommandWriteRead(Dri->drmFD, DRM_RADEON_INDIRECT,
				&indirect, sizeof(struct drm_radeon_indirect));
	    return;
	}

    xf86DrvMsg(scrnIndex, X_ERROR,
	       "%s: Unable to retrieve the indirect Buffer at address %p!\n",
	       __func__, Buffer);
}

unsigned int
RHDDRIGetIntGARTLocation(ScrnInfoPtr pScrn)
{
    RHDPtr         rhdPtr  = RHDPTR(pScrn);
    struct rhdDri *rhdDRI    = rhdPtr->dri;

    if (!rhdDRI->gartLocation)
        return 0;

    return rhdDRI->gartLocation + rhdDRI->bufStart;
}
