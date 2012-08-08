/*
 * Copyright 2000 ATI Technologies Inc., Markham, Ontario,
 *                VA Linux Systems Inc., Fremont, California.
 *
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *   Rickard E. Faith <faith@valinux.com>
 *   Gareth Hughes <gareth@valinux.com>
 *
 */

#include <string.h>
#include <stdio.h>

				/* Driver data structures */
#include "radeon.h"
#include "radeon_video.h"
#include "radeon_reg.h"
#include "r600_reg.h"
#include "radeon_macros.h"
#include "radeon_drm.h"
#include "radeon_dri.h"
#include "radeon_version.h"


				/* X and server generic header files */
#include "xf86.h"
#include "windowstr.h"

				/* GLX/DRI/DRM definitions */
#define _XF86DRI_SERVER_
#include "GL/glxtokens.h"
#include "sarea.h"

#include "atipciids.h"

static size_t radeon_drm_page_size;

#define RADEON_MAX_DRAWABLES 256

extern void GlxSetVisualConfigs(int nconfigs, __GLXvisualConfig *configs,
				void **configprivs);

static void RADEONDRITransitionTo2d(ScreenPtr pScreen);
static void RADEONDRITransitionTo3d(ScreenPtr pScreen);
static void RADEONDRITransitionMultiToSingle3d(ScreenPtr pScreen);
static void RADEONDRITransitionSingleToMulti3d(ScreenPtr pScreen);

#ifdef DAMAGE
static void RADEONDRIRefreshArea(ScrnInfoPtr pScrn, RegionPtr pReg);

#if (DRIINFO_MAJOR_VERSION > 5 ||		\
     (DRIINFO_MAJOR_VERSION == 5 && DRIINFO_MINOR_VERSION >= 1))
static void RADEONDRIClipNotify(ScreenPtr pScreen, WindowPtr *ppWin, int num);
#endif
#endif

/* Initialize the visual configs that are supported by the hardware.
 * These are combined with the visual configs that the indirect
 * rendering core supports, and the intersection is exported to the
 * client.
 */
static Bool RADEONInitVisualConfigs(ScreenPtr pScreen)
{
    ScrnInfoPtr          pScrn             = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr        info              = RADEONPTR(pScrn);
    int                  numConfigs        = 0;
    __GLXvisualConfig   *pConfigs          = 0;
    RADEONConfigPrivPtr  pRADEONConfigs    = 0;
    RADEONConfigPrivPtr *pRADEONConfigPtrs = 0;
    int                  i, accum, stencil, db, use_db;

    use_db = !info->dri->noBackBuffer ? 1 : 0;

    switch (info->CurrentLayout.pixel_code) {
    case 8:  /* 8bpp mode is not support */
    case 15: /* FIXME */
    case 24: /* FIXME */
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[dri] RADEONInitVisualConfigs failed "
		   "(depth %d not supported).  "
		   "Disabling DRI.\n", info->CurrentLayout.pixel_code);
	return FALSE;

#define RADEON_USE_ACCUM   1
#define RADEON_USE_STENCIL 1

    case 16:
	numConfigs = 1;
	if (RADEON_USE_ACCUM)   numConfigs *= 2;
	if (RADEON_USE_STENCIL) numConfigs *= 2;
	if (use_db)             numConfigs *= 2;

	if (!(pConfigs
	      = (__GLXvisualConfig *)calloc(sizeof(__GLXvisualConfig),
					    numConfigs))) {
	    return FALSE;
	}
	if (!(pRADEONConfigs
	      = (RADEONConfigPrivPtr)calloc(sizeof(RADEONConfigPrivRec),
					    numConfigs))) {
	    free(pConfigs);
	    return FALSE;
	}
	if (!(pRADEONConfigPtrs
	      = (RADEONConfigPrivPtr *)calloc(sizeof(RADEONConfigPrivPtr),
					      numConfigs))) {
	    free(pConfigs);
	    free(pRADEONConfigs);
	    return FALSE;
	}

	i = 0;
	for (db = use_db; db >= 0; db--) {
	  for (accum = 0; accum <= RADEON_USE_ACCUM; accum++) {
	    for (stencil = 0; stencil <= RADEON_USE_STENCIL; stencil++) {
		pRADEONConfigPtrs[i] = &pRADEONConfigs[i];

		pConfigs[i].vid                = (VisualID)(-1);
		pConfigs[i].class              = -1;
		pConfigs[i].rgba               = TRUE;
		pConfigs[i].redSize            = 5;
		pConfigs[i].greenSize          = 6;
		pConfigs[i].blueSize           = 5;
		pConfigs[i].alphaSize          = 0;
		pConfigs[i].redMask            = 0x0000F800;
		pConfigs[i].greenMask          = 0x000007E0;
		pConfigs[i].blueMask           = 0x0000001F;
		pConfigs[i].alphaMask          = 0x00000000;
		if (accum) { /* Simulated in software */
		    pConfigs[i].accumRedSize   = 16;
		    pConfigs[i].accumGreenSize = 16;
		    pConfigs[i].accumBlueSize  = 16;
		    pConfigs[i].accumAlphaSize = 0;
		} else {
		    pConfigs[i].accumRedSize   = 0;
		    pConfigs[i].accumGreenSize = 0;
		    pConfigs[i].accumBlueSize  = 0;
		    pConfigs[i].accumAlphaSize = 0;
		}
		if (db)
		    pConfigs[i].doubleBuffer   = TRUE;
		else
		    pConfigs[i].doubleBuffer   = FALSE;
		pConfigs[i].stereo             = FALSE;
		pConfigs[i].bufferSize         = 16;
		pConfigs[i].depthSize          = info->dri->depthBits;
		if (pConfigs[i].depthSize == 24 ? (RADEON_USE_STENCIL - stencil)
						: stencil) {
		    pConfigs[i].stencilSize    = 8;
		} else {
		    pConfigs[i].stencilSize    = 0;
		}
		pConfigs[i].auxBuffers         = 0;
		pConfigs[i].level              = 0;
		if (accum ||
		    (pConfigs[i].stencilSize && pConfigs[i].depthSize == 16)) {
		   pConfigs[i].visualRating    = GLX_SLOW_CONFIG;
		} else {
		   pConfigs[i].visualRating    = GLX_NONE;
		}
		pConfigs[i].transparentPixel   = GLX_NONE;
		pConfigs[i].transparentRed     = 0;
		pConfigs[i].transparentGreen   = 0;
		pConfigs[i].transparentBlue    = 0;
		pConfigs[i].transparentAlpha   = 0;
		pConfigs[i].transparentIndex   = 0;
		i++;
	    }
	  }
	}
	break;

    case 32:
	numConfigs = 1;
	if (RADEON_USE_ACCUM)   numConfigs *= 2;
	if (RADEON_USE_STENCIL) numConfigs *= 2;
	if (use_db)             numConfigs *= 2;

	if (!(pConfigs
	      = (__GLXvisualConfig *)calloc(sizeof(__GLXvisualConfig),
					    numConfigs))) {
	    return FALSE;
	}
	if (!(pRADEONConfigs
	      = (RADEONConfigPrivPtr)calloc(sizeof(RADEONConfigPrivRec),
					    numConfigs))) {
	    free(pConfigs);
	    return FALSE;
	}
	if (!(pRADEONConfigPtrs
	      = (RADEONConfigPrivPtr *)calloc(sizeof(RADEONConfigPrivPtr),
					      numConfigs))) {
	    free(pConfigs);
	    free(pRADEONConfigs);
	    return FALSE;
	}

	i = 0;
	for (db = use_db; db >= 0; db--) {
	  for (accum = 0; accum <= RADEON_USE_ACCUM; accum++) {
	    for (stencil = 0; stencil <= RADEON_USE_STENCIL; stencil++) {
		pRADEONConfigPtrs[i] = &pRADEONConfigs[i];

		pConfigs[i].vid                = (VisualID)(-1);
		pConfigs[i].class              = -1;
		pConfigs[i].rgba               = TRUE;
		pConfigs[i].redSize            = 8;
		pConfigs[i].greenSize          = 8;
		pConfigs[i].blueSize           = 8;
		pConfigs[i].alphaSize          = 8;
		pConfigs[i].redMask            = 0x00FF0000;
		pConfigs[i].greenMask          = 0x0000FF00;
		pConfigs[i].blueMask           = 0x000000FF;
		pConfigs[i].alphaMask          = 0xFF000000;
		if (accum) { /* Simulated in software */
		    pConfigs[i].accumRedSize   = 16;
		    pConfigs[i].accumGreenSize = 16;
		    pConfigs[i].accumBlueSize  = 16;
		    pConfigs[i].accumAlphaSize = 16;
		} else {
		    pConfigs[i].accumRedSize   = 0;
		    pConfigs[i].accumGreenSize = 0;
		    pConfigs[i].accumBlueSize  = 0;
		    pConfigs[i].accumAlphaSize = 0;
		}
		if (db)
		    pConfigs[i].doubleBuffer   = TRUE;
		else
		    pConfigs[i].doubleBuffer   = FALSE;
		pConfigs[i].stereo             = FALSE;
		pConfigs[i].bufferSize         = 32;
		pConfigs[i].depthSize          = info->dri->depthBits;
		if (pConfigs[i].depthSize == 24 ? (RADEON_USE_STENCIL - stencil)
						: stencil) {
		    pConfigs[i].stencilSize    = 8;
		} else {
		    pConfigs[i].stencilSize    = 0;
		}
		pConfigs[i].auxBuffers         = 0;
		pConfigs[i].level              = 0;
		if (accum ||
		    (pConfigs[i].stencilSize && pConfigs[i].depthSize == 16)) {
		   pConfigs[i].visualRating    = GLX_SLOW_CONFIG;
		} else {
		   pConfigs[i].visualRating    = GLX_NONE;
		}
		pConfigs[i].transparentPixel   = GLX_NONE;
		pConfigs[i].transparentRed     = 0;
		pConfigs[i].transparentGreen   = 0;
		pConfigs[i].transparentBlue    = 0;
		pConfigs[i].transparentAlpha   = 0;
		pConfigs[i].transparentIndex   = 0;
		i++;
	    }
	  }
	}
	break;
    }

    info->dri->numVisualConfigs   = numConfigs;
    info->dri->pVisualConfigs     = pConfigs;
    info->dri->pVisualConfigsPriv = pRADEONConfigs;
    GlxSetVisualConfigs(numConfigs, pConfigs, (void**)pRADEONConfigPtrs);
    return TRUE;
}

/* Create the Radeon-specific context information */
static Bool RADEONCreateContext(ScreenPtr pScreen, VisualPtr visual,
				drm_context_t hwContext, void *pVisualConfigPriv,
				DRIContextType contextStore)
{
    return TRUE;
}

/* Destroy the Radeon-specific context information */
static void RADEONDestroyContext(ScreenPtr pScreen, drm_context_t hwContext,
				 DRIContextType contextStore)
{
}

/* Called when the X server is woken up to allow the last client's
 * context to be saved and the X server's context to be loaded.  This is
 * not necessary for the Radeon since the client detects when it's
 * context is not currently loaded and then load's it itself.  Since the
 * registers to start and stop the CP are privileged, only the X server
 * can start/stop the engine.
 */
static void RADEONEnterServer(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    drm_radeon_sarea_t *pSAREAPriv;


    RADEON_MARK_SYNC(info, pScrn);

    pSAREAPriv = DRIGetSAREAPrivate(pScrn->pScreen);
    if (pSAREAPriv->ctx_owner != DRIGetContext(pScrn->pScreen)) {
	info->accel_state->XInited3D = FALSE;
	info->cp->needCacheFlush = (info->ChipFamily >= CHIP_FAMILY_R300);
    }

#ifdef DAMAGE
    if (!info->dri->pDamage && info->dri->allowPageFlip) {
	PixmapPtr pPix  = pScreen->GetScreenPixmap(pScreen);
	info->dri->pDamage = DamageCreate(NULL, NULL, DamageReportNone, TRUE,
					  pScreen, pPix);

	if (info->dri->pDamage == NULL) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "No screen damage record, page flipping disabled\n");
	    info->dri->allowPageFlip = 0;
	} else {
	    DamageRegister(&pPix->drawable, info->dri->pDamage);

	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Damage tracking initialized for page flipping\n");
	}
    }
#endif
}

/* Called when the X server goes to sleep to allow the X server's
 * context to be saved and the last client's context to be loaded.  This
 * is not necessary for the Radeon since the client detects when it's
 * context is not currently loaded and then load's it itself.  Since the
 * registers to start and stop the CP are privileged, only the X server
 * can start/stop the engine.
 */
static void RADEONLeaveServer(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    RING_LOCALS;

#ifdef DAMAGE
    if (info->dri->pDamage) {
	RegionPtr pDamageReg = DamageRegion(info->dri->pDamage);
	int nrects = pDamageReg ? REGION_NUM_RECTS(pDamageReg) : 0;

	if (nrects) {
	    RADEONDRIRefreshArea(pScrn, pDamageReg);
	}
    }
#endif

    /* The CP is always running, but if we've generated any CP commands
     * we must flush them to the kernel module now.
     */
    RADEONCP_RELEASE(pScrn, info);

#ifdef USE_EXA
    info->accel_state->engineMode = EXA_ENGINEMODE_UNKNOWN;
#endif
}

/* Contexts can be swapped by the X server if necessary.  This callback
 * is currently only used to perform any functions necessary when
 * entering or leaving the X server, and in the future might not be
 * necessary.
 */
static void RADEONDRISwapContext(ScreenPtr pScreen, DRISyncType syncType,
				 DRIContextType oldContextType,
				 void *oldContext,
				 DRIContextType newContextType,
				 void *newContext)
{
    if ((syncType==DRI_3D_SYNC) && (oldContextType==DRI_2D_CONTEXT) &&
	(newContextType==DRI_2D_CONTEXT)) { /* Entering from Wakeup */
	RADEONEnterServer(pScreen);
    }

    if ((syncType==DRI_2D_SYNC) && (oldContextType==DRI_NO_CONTEXT) &&
	(newContextType==DRI_2D_CONTEXT)) { /* Exiting from Block Handler */
	RADEONLeaveServer(pScreen);
    }
}

#ifdef USE_XAA

/* The Radeon has depth tiling on all the time. Rely on surface regs to
 * translate the addresses (only works if allowColorTiling is true).
 */

/* 16-bit depth buffer functions */
#define WRITE_DEPTH16(_x, _y, d)					\
    *(uint16_t *)(pointer)(buf + 2*(_x + _y*info->dri->frontPitch)) = (d)

#define READ_DEPTH16(d, _x, _y)						\
    (d) = *(uint16_t *)(pointer)(buf + 2*(_x + _y*info->dri->frontPitch))

/* 32-bit depth buffer (stencil and depth simultaneously) functions */
#define WRITE_DEPTHSTENCIL32(_x, _y, d)					\
    *(uint32_t *)(pointer)(buf + 4*(_x + _y*info->dri->frontPitch)) = (d)

#define READ_DEPTHSTENCIL32(d, _x, _y)					\
    (d) = *(uint32_t *)(pointer)(buf + 4*(_x + _y*info->dri->frontPitch))

/* Screen to screen copy of data in the depth buffer */
static void RADEONScreenToScreenCopyDepth(ScrnInfoPtr pScrn,
					  int xa, int ya,
					  int xb, int yb,
					  int w, int h)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    unsigned char *buf  = info->FB + info->dri->depthOffset;
    int            xstart, xend, xdir;
    int            ystart, yend, ydir;
    int            x, y, d;

    if (xa < xb) xdir = -1, xstart = w-1, xend = 0;
    else         xdir =  1, xstart = 0,   xend = w-1;

    if (ya < yb) ydir = -1, ystart = h-1, yend = 0;
    else         ydir =  1, ystart = 0,   yend = h-1;

    switch (pScrn->bitsPerPixel) {
    case 16:
	for (x = xstart; x != xend; x += xdir) {
	    for (y = ystart; y != yend; y += ydir) {
		READ_DEPTH16(d, xa+x, ya+y);
		WRITE_DEPTH16(xb+x, yb+y, d);
	    }
	}
	break;

    case 32:
	for (x = xstart; x != xend; x += xdir) {
	    for (y = ystart; y != yend; y += ydir) {
		READ_DEPTHSTENCIL32(d, xa+x, ya+y);
		WRITE_DEPTHSTENCIL32(xb+x, yb+y, d);
	    }
	}
	break;

    default:
	break;
    }
}

#endif /* USE_XAA */

/* Initialize the state of the back and depth buffers */
static void RADEONDRIInitBuffers(WindowPtr pWin, RegionPtr prgn, CARD32 indx)
{
   /* NOOP.  There's no need for the 2d driver to be clearing buffers
    * for the 3d client.  It knows how to do that on its own.
    */
}

/* Copy the back and depth buffers when the X server moves a window.
 *
 * This routine is a modified form of XAADoBitBlt with the calls to
 * ScreenToScreenBitBlt built in. My routine has the prgnSrc as source
 * instead of destination. My origin is upside down so the ydir cases
 * are reversed.
 */
static void RADEONDRIMoveBuffers(WindowPtr pParent, DDXPointRec ptOldOrg,
				 RegionPtr prgnSrc, CARD32 indx)
{
#ifdef USE_XAA
    ScreenPtr      pScreen  = pParent->drawable.pScreen;
    ScrnInfoPtr    pScrn    = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info     = RADEONPTR(pScrn);

    BoxPtr         pboxTmp, pboxNext, pboxBase;
    DDXPointPtr    pptTmp;
    int            xdir, ydir;

    int            screenwidth = pScrn->virtualX;
    int            screenheight = pScrn->virtualY;

    BoxPtr         pbox     = REGION_RECTS(prgnSrc);
    int            nbox     = REGION_NUM_RECTS(prgnSrc);

    BoxPtr         pboxNew1 = NULL;
    BoxPtr         pboxNew2 = NULL;
    DDXPointPtr    pptNew1  = NULL;
    DDXPointPtr    pptNew2  = NULL;
    DDXPointPtr    pptSrc   = &ptOldOrg;

    int            dx       = pParent->drawable.x - ptOldOrg.x;
    int            dy       = pParent->drawable.y - ptOldOrg.y;

    /* XXX: Fix in EXA case. */
    if (info->useEXA)
	return;

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

    /* pretty much a hack. */
    info->accel_state->dst_pitch_offset = info->dri->backPitchOffset;
    if (info->tilingEnabled)
       info->accel_state->dst_pitch_offset |= RADEON_DST_TILE_MACRO;

    (*info->accel_state->accel->SetupForScreenToScreenCopy)(pScrn, xdir, ydir, GXcopy,
							    (uint32_t)(-1), -1);

    for (; nbox-- ; pbox++) {
	int  xa    = pbox->x1;
	int  ya    = pbox->y1;
	int  destx = xa + dx;
	int  desty = ya + dy;
	int  w     = pbox->x2 - xa + 1;
	int  h     = pbox->y2 - ya + 1;

	if (destx < 0)                xa -= destx, w += destx, destx = 0;
	if (desty < 0)                ya -= desty, h += desty, desty = 0;
	if (destx + w > screenwidth)  w = screenwidth  - destx;
	if (desty + h > screenheight) h = screenheight - desty;

	if (w <= 0) continue;
	if (h <= 0) continue;

	(*info->accel_state->accel->SubsequentScreenToScreenCopy)(pScrn,
								  xa, ya,
								  destx, desty,
								  w, h);

	if (info->dri->depthMoves) {
	    RADEONScreenToScreenCopyDepth(pScrn,
					  xa, ya,
					  destx, desty,
					  w, h);
	}
    }

    info->accel_state->dst_pitch_offset = info->dri->frontPitchOffset;;

    free(pptNew2);
    free(pboxNew2);
    free(pptNew1);
    free(pboxNew1);

    info->accel_state->accel->NeedToSync = TRUE;
#endif /* USE_XAA */
}

static void RADEONDRIInitGARTValues(RADEONInfoPtr info)
{
    int            s, l;

    info->dri->gartOffset = 0;

				/* Initialize the CP ring buffer data */
    info->dri->ringStart       = info->dri->gartOffset;
    info->dri->ringMapSize     = info->dri->ringSize*1024*1024 + radeon_drm_page_size;
    info->dri->ringSizeLog2QW  = RADEONMinBits(info->dri->ringSize*1024*1024/8)-1;

    info->dri->ringReadOffset  = info->dri->ringStart + info->dri->ringMapSize;
    info->dri->ringReadMapSize = radeon_drm_page_size;

				/* Reserve space for vertex/indirect buffers */
    info->dri->bufStart        = info->dri->ringReadOffset + info->dri->ringReadMapSize;
    info->dri->bufMapSize      = info->dri->bufSize*1024*1024;

				/* Reserve the rest for GART textures */
    info->dri->gartTexStart     = info->dri->bufStart + info->dri->bufMapSize;
    s = (info->dri->gartSize*1024*1024 - info->dri->gartTexStart);
    l = RADEONMinBits((s-1) / RADEON_NR_TEX_REGIONS);
    if (l < RADEON_LOG_TEX_GRANULARITY) l = RADEON_LOG_TEX_GRANULARITY;
    info->dri->gartTexMapSize   = (s >> l) << l;
    info->dri->log2GARTTexGran  = l;
}

/* AGP Mode Quirk List - Certain hostbridge/gfx-card combos don't work with
 * the standard AGPMode settings, so we detect and handle these
 * on a case-by-base basis with quirks.  To see if an AGPMode is valid, test
 * it by setting Option "AGPMode" "1" (or "2", or "4", or "8"). */
typedef struct {
    unsigned int hostbridgeVendor;
    unsigned int hostbridgeDevice;
    unsigned int chipVendor;
    unsigned int chipDevice;
    unsigned int subsysVendor;
    unsigned int subsysDevice;
    unsigned int defaultMode;
} radeon_agpmode_quirk, *radeon_agpmode_quirk_ptr;

/* Keep sorted by hostbridge vendor and device */
static radeon_agpmode_quirk radeon_agpmode_quirk_list[] = {

    /* Intel E7505 Memory Controller Hub / RV350 AR [Radeon 9600XT] Needs AGPMode 4 (deb #515326) */
    { PCI_VENDOR_INTEL,0x2550,  PCI_VENDOR_ATI,0x4152,  0x1458,0x4038,           4 },
    /* Intel 82865G/PE/P DRAM Controller/Host-Hub / Mobility 9800 Needs AGPMode 4 (deb #462590) */
    { PCI_VENDOR_INTEL,0x2570,  PCI_VENDOR_ATI,0x4a4e,  PCI_VENDOR_DELL,0x5106,  4 },
    /* Intel 82865G/PE/P DRAM Controller/Host-Hub / RV280 [Radeon 9200 SE] Needs AGPMode 4 (lp #300304) */
    { PCI_VENDOR_INTEL,0x2570,  PCI_VENDOR_ATI,0x5964,  0x148c,0x2073,           4 },
    /* Intel 82855PM host bridge / Mobility M7 LW Needs AGPMode 4 (lp: #353996) */
    { PCI_VENDOR_INTEL,0x3340,  PCI_VENDOR_ATI,0x4c57, PCI_VENDOR_IBM,0x0530,    4 },
    /* Intel 82855PM Processor to I/O Controller / Mobility M6 LY Needs AGPMode 1 (deb #467235) */
    { PCI_VENDOR_INTEL,0x3340,  PCI_VENDOR_ATI,0x4c59,  PCI_VENDOR_IBM,0x052f,   1 },
    /* Intel 82855PM host bridge / Mobility 9600 M10 RV350 Needs AGPMode 1 (lp #195051) */
    { PCI_VENDOR_INTEL,0x3340,  PCI_VENDOR_ATI,0x4e50,  PCI_VENDOR_IBM,0x0550,   1 },
    /* Intel 82855PM host bridge / FireGL Mobility T2 RV350 Needs AGPMode 2 (fdo #20647) */
    { PCI_VENDOR_INTEL,0x3340,  PCI_VENDOR_ATI,0x4e54,  PCI_VENDOR_IBM,0x054f,   2 },
    /* Intel 82855PM host bridge / Mobility M9+ / VaioPCG-V505DX Needs AGPMode 2 (fdo #17928) */
    { PCI_VENDOR_INTEL,0x3340,  PCI_VENDOR_ATI,0x5c61,  PCI_VENDOR_SONY,0x816b,  2 },
    /* Intel 82855PM Processor to I/O Controller / Mobility M9+ Needs AGPMode 8 (phoronix forum) */
    { PCI_VENDOR_INTEL,0x3340,  PCI_VENDOR_ATI,0x5c61,  PCI_VENDOR_SONY,0x8195,  8 },
    /* Intel 82830 830 Chipset Host Bridge / Mobility M6 LY Needs AGPMode 2 (fdo #17360)*/
    { PCI_VENDOR_INTEL,0x3575,  PCI_VENDOR_ATI,0x4c59,  PCI_VENDOR_DELL,0x00e3,  2 },
    /* Intel 82852/82855 host bridge / Mobility FireGL 9000 R250 Needs AGPMode 1 (lp #296617) */
    { PCI_VENDOR_INTEL,0x3580,  PCI_VENDOR_ATI,0x4c66,  PCI_VENDOR_DELL,0x0149,  1 },
    /* Intel 82852/82855 host bridge / Mobility 9600 M10 RV350 Needs AGPMode 1 (deb #467460) */
    { PCI_VENDOR_INTEL,0x3580,  PCI_VENDOR_ATI,0x4e50,  0x1025,0x0061,           1 },
    /* Intel 82852/82855 host bridge / Mobility 9600 M10 RV350 Needs AGPMode 1 (lp #203007) */
    { PCI_VENDOR_INTEL,0x3580,  PCI_VENDOR_ATI,0x4e50,  0x1025,0x0064,           1 },
    /* Intel 82852/82855 host bridge / Mobility 9600 M10 RV350 Needs AGPMode 1 (lp #141551) */
    { PCI_VENDOR_INTEL,0x3580,  PCI_VENDOR_ATI,0x4e50,  PCI_VENDOR_ASUS,0x1942,  1 },
    /* Intel 82852/82855 host bridge / Mobility 9600/9700 Needs AGPMode 1 (deb #510208) */
    { PCI_VENDOR_INTEL,0x3580,  PCI_VENDOR_ATI,0x4e50,  0x10cf,0x127f,           1 },
    /* Intel 82443BX/ZX/DX Host bridge / RV280 [Radeon 9200] Needs AGPMode 1 (lp #370205) */
    { PCI_VENDOR_INTEL,0x7190,  PCI_VENDOR_ATI,0x5961,  0x174b,0x7c13,           1 },

    /* Ali Corp M1671 Super P4 Northbridge / Mobility M6 LY Needs AGPMode 1 (lp #146303)*/
    { 0x10b9,0x1671,           PCI_VENDOR_ATI,0x4c59,   0x103c,0x0027,           1 },

    /* SiS Host Bridge 655 / R420 [Radeon X800] Needs AGPMode 4 (lp #371296) */
    { 0x1039,0x0655,            PCI_VENDOR_ATI,0x4a4b,  PCI_VENDOR_ATI,0x4422,   4 },
    /* SiS Host Bridge / RV280 Needs AGPMode 4 */
    { 0x1039,0x0741,            PCI_VENDOR_ATI,0x5964,  0x148c,0x2073,           4 },

    /* ASRock K7VT4A+ AGP 8x / ATI Radeon 9250 AGP Needs AGPMode 4 (lp #133192) */
    { 0x1849,0x3189,            PCI_VENDOR_ATI,0x5960,  0x1787,0x5960,           4 },

    /* VIA K8M800 Host Bridge / RV280 [Radeon 9200 PRO] Needs AGPMode 4 (fdo #12544) */
    { PCI_VENDOR_VIA,0x0204,    PCI_VENDOR_ATI,0x5960,  0x17af,0x2020,           4 },
    /* VIA KT880 Host Bridge / RV350 [Radeon 9550] Needs AGPMode 4 (fdo #19981) */
    { PCI_VENDOR_VIA,0x0269,    PCI_VENDOR_ATI,0x4153,  PCI_VENDOR_ASUS,0x003c,  4 },
    /* VIA VT8363 Host Bridge / R200 QL [Radeon 8500] Needs AGPMode 2 (lp #141551) */
    { PCI_VENDOR_VIA,0x0305,    PCI_VENDOR_ATI,0x514c,  PCI_VENDOR_ATI,0x013a,   2 },
    /* VIA VT82C693A Host Bridge / RV280 [Radeon 9200 PRO] Needs AGPMode 2 (deb #515512) */
    { PCI_VENDOR_VIA,0x0691,    PCI_VENDOR_ATI,0x5960,  PCI_VENDOR_ASUS,0x004c,  2 },
    /* VIA VT82C693A Host Bridge / RV280 [Radeon 9200 PRO] Needs AGPMode 2 */
    { PCI_VENDOR_VIA,0x0691,    PCI_VENDOR_ATI,0x5960,  PCI_VENDOR_ASUS,0x0054,  2 },
    /* VIA VT8377 Host Bridge / R200 QM [Radeon 9100] Needs AGPMode 4 (deb #461144) */
    { PCI_VENDOR_VIA,0x3189,    PCI_VENDOR_ATI,0x514d,  0x174b,0x7149,           4 },
    /* VIA VT8377 Host Bridge / RV280 [Radeon 9200 PRO] Needs AGPMode 4 (lp #312693) */
    { PCI_VENDOR_VIA,0x3189,    PCI_VENDOR_ATI,0x5960,  0x1462,0x0380,           4 },
    /* VIA VT8377 Host Bridge / RV280 Needs AGPMode 4 (ati ML) */
    { PCI_VENDOR_VIA,0x3189,    PCI_VENDOR_ATI,0x5964,  0x148c,0x2073,           4 },
    /* VIA VT8377 Host Bridge / RV280 Needs AGPMode 4 (fdo #12544) */
    { PCI_VENDOR_VIA,0x3189,    PCI_VENDOR_ATI,0x5964,  PCI_VENDOR_ASUS,0xc008,  4 },
    /* VIA VT8377 Host Bridge / RV280 Needs AGPMode 4 (deb #545040) */
    { PCI_VENDOR_VIA,0x3189,    PCI_VENDOR_ATI,0x5960,  PCI_VENDOR_ASUS,0x004c,  4 },

    /* ATI Host Bridge / RV280 [M9+] Needs AGPMode 1 (phoronix forum) */
    { PCI_VENDOR_ATI,0xcbb2,    PCI_VENDOR_ATI,0x5c61,  PCI_VENDOR_SONY,0x8175,  1 },

    /* HP Host Bridge / R300 [FireGL X1] Needs AGPMode 2 (fdo #7770) */
    { PCI_VENDOR_HP,0x122e,    PCI_VENDOR_ATI,0x4e47,  PCI_VENDOR_ATI,0x0152,    2 },

    /* nVidia Host Bridge / R420 [X800 Pro] Needs AGPMode 4 (fdo #22726) */
    { 0x10de,0x00e1,           PCI_VENDOR_ATI,0x4a49,  PCI_VENDOR_ATI,0x0002,    4 },

    { 0, 0, 0, 0, 0, 0, 0 },
};

/* Set AGP transfer mode according to requests and constraints */
static Bool RADEONSetAgpMode(RADEONInfoPtr info, ScreenPtr pScreen)
{
    unsigned char *RADEONMMIO = info->MMIO;
    unsigned long mode   = drmAgpGetMode(info->dri->drmFD);	/* Default mode */
    unsigned int  vendor = drmAgpVendorId(info->dri->drmFD);
    unsigned int  device = drmAgpDeviceId(info->dri->drmFD);

    if (info->ChipFamily < CHIP_FAMILY_R600) {
	/* ignore agp 3.0 mode bit from the chip as it's buggy on some cards with
	   pcie-agp rialto bridge chip - use the one from bridge which must match */
	uint32_t agp_status = (INREG(RADEON_AGP_STATUS) | RADEON_AGPv3_MODE) & mode;
	Bool is_v3 = (agp_status & RADEON_AGPv3_MODE);
	unsigned int defaultMode;
	MessageType from;

	if (is_v3) {
	    defaultMode = (agp_status & RADEON_AGPv3_8X_MODE) ? 8 : 4;
	} else {
	    if (agp_status & RADEON_AGP_4X_MODE) defaultMode = 4;
	    else if (agp_status & RADEON_AGP_2X_MODE) defaultMode = 2;
	    else defaultMode = 1;
	}

	/* Apply AGPMode Quirks */
	radeon_agpmode_quirk_ptr p = radeon_agpmode_quirk_list;
	while (p && p->chipDevice != 0) {
	    if (vendor == p->hostbridgeVendor &&
		device == p->hostbridgeDevice &&
		PCI_DEV_VENDOR_ID(info->PciInfo) == p->chipVendor &&
		PCI_DEV_DEVICE_ID(info->PciInfo) == p->chipDevice &&
		PCI_SUB_VENDOR_ID(info->PciInfo) == p->subsysVendor &&
		PCI_SUB_DEVICE_ID(info->PciInfo) == p->subsysDevice)
	    {
		defaultMode = p->defaultMode;
	    }
	    ++p;
	}

	from = X_DEFAULT;

	if (xf86GetOptValInteger(info->Options, OPTION_AGP_MODE, &info->dri->agpMode)) {
	    if ((info->dri->agpMode < (is_v3 ? 4 : 1)) ||
		(info->dri->agpMode > (is_v3 ? 8 : 4)) ||
		(info->dri->agpMode & (info->dri->agpMode - 1))) {
		xf86DrvMsg(pScreen->myNum, X_ERROR,
			   "Illegal AGP Mode: %d (valid values: %s), leaving at "
			   "%dx\n", info->dri->agpMode, is_v3 ? "4, 8" : "1, 2, 4",
			   defaultMode);
		info->dri->agpMode = defaultMode;
	    } else
		from = X_CONFIG;
	} else
	    info->dri->agpMode = defaultMode;

	xf86DrvMsg(pScreen->myNum, from, "Using AGP %dx\n", info->dri->agpMode);

	mode &= ~RADEON_AGP_MODE_MASK;
	if (is_v3) {
	    /* only set one mode bit for AGPv3 */
	    switch (info->dri->agpMode) {
	    case 8:          mode |= RADEON_AGPv3_8X_MODE; break;
	    case 4: default: mode |= RADEON_AGPv3_4X_MODE;
	    }
	    /*TODO: need to take care of other bits valid for v3 mode
	     *      currently these bits are not used in all tested cards.
	     */
	} else {
	    switch (info->dri->agpMode) {
	    case 4:          mode |= RADEON_AGP_4X_MODE;
	    case 2:          mode |= RADEON_AGP_2X_MODE;
	    case 1: default: mode |= RADEON_AGP_1X_MODE;
	    }
	}

	/* AGP Fast Writes.
	 * TODO: take into account that certain agp modes don't support fast
	 * writes at all */
	mode &= ~RADEON_AGP_FW_MODE; /* Disable per default */
	if (xf86ReturnOptValBool(info->Options, OPTION_AGP_FW, FALSE)) {
	    xf86DrvMsg(pScreen->myNum, X_WARNING,
		       "WARNING: Using the AGPFastWrite option is not recommended.\n");
	    xf86Msg(X_NONE, "\tThis option does not provide much of a noticable speed"
		    " boost, while it\n\twill probably hard lock your machine."
		    " All bets are off!\n");

	    /* Black list some host/AGP bridges. */
	    if ((vendor == PCI_VENDOR_AMD) && (device == PCI_CHIP_AMD761))
		xf86DrvMsg(pScreen->myNum, X_PROBED, "Ignoring AGPFastWrite option "
			   "for the AMD 761 northbridge.\n");
	    else {
		xf86DrvMsg(pScreen->myNum, X_CONFIG, "Enabling AGP Fast Writes.\n");
		mode |= RADEON_AGP_FW_MODE;
	    }
	} /* Don't mention this otherwise, so that people don't get funny ideas */
    } else
	info->dri->agpMode = 8; /* doesn't matter at this point */

    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] Mode 0x%08lx [AGP 0x%04x/0x%04x; Card 0x%04x/0x%04x 0x%04x/0x%04x]\n",
	       mode, vendor, device,
	       PCI_DEV_VENDOR_ID(info->PciInfo),
	       PCI_DEV_DEVICE_ID(info->PciInfo),
	       PCI_SUB_VENDOR_ID(info->PciInfo),
	       PCI_SUB_DEVICE_ID(info->PciInfo));

    if (drmAgpEnable(info->dri->drmFD, mode) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[agp] AGP not enabled\n");
	drmAgpRelease(info->dri->drmFD);
	return FALSE;
    }

    /* Workaround for some hardware bugs */
    if (info->ChipFamily < CHIP_FAMILY_R200)
	OUTREG(RADEON_AGP_CNTL, INREG(RADEON_AGP_CNTL) | 0x000e0000);

				/* Modify the mode if the default mode
				 * is not appropriate for this
				 * particular combination of graphics
				 * card and AGP chipset.
				 */

    return TRUE;
}

/* Initialize Radeon's AGP registers */
static void RADEONSetAgpBase(RADEONInfoPtr info, ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    unsigned char *RADEONMMIO = info->MMIO;

    if (info->ChipFamily >= CHIP_FAMILY_R600)
	return;

    /* drm already does this, so we can probably remove this.
     * agp_base_2 ?
     */
    if (info->ChipFamily == CHIP_FAMILY_RV515)
	OUTMC(pScrn, RV515_MC_AGP_BASE, drmAgpBase(info->dri->drmFD));
    else if ((info->ChipFamily >= CHIP_FAMILY_R520) &&
	     (info->ChipFamily <= CHIP_FAMILY_RV570))
	OUTMC(pScrn, R520_MC_AGP_BASE, drmAgpBase(info->dri->drmFD));
    else if ((info->ChipFamily == CHIP_FAMILY_RS690) ||
	     (info->ChipFamily == CHIP_FAMILY_RS740))
	OUTMC(pScrn, RS690_MC_AGP_BASE, drmAgpBase(info->dri->drmFD));
    else if (info->ChipFamily < CHIP_FAMILY_RV515)
	OUTREG(RADEON_AGP_BASE, drmAgpBase(info->dri->drmFD));
}

/* Initialize the AGP state.  Request memory for use in AGP space, and
 * initialize the Radeon registers to point to that memory.
 */
static Bool RADEONDRIAgpInit(RADEONInfoPtr info, ScreenPtr pScreen)
{
    int            ret;

    if (drmAgpAcquire(info->dri->drmFD) < 0) {
	xf86DrvMsg(pScreen->myNum, X_WARNING, "[agp] AGP not available\n");
	return FALSE;
    }

    if (!RADEONSetAgpMode(info, pScreen))
	return FALSE;

    RADEONDRIInitGARTValues(info);

    if ((ret = drmAgpAlloc(info->dri->drmFD, info->dri->gartSize*1024*1024, 0, NULL,
			   &info->dri->agpMemHandle)) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[agp] Out of memory (%d)\n", ret);
	drmAgpRelease(info->dri->drmFD);
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] %d kB allocated with handle 0x%08x\n",
	       info->dri->gartSize*1024,
	       (unsigned int)info->dri->agpMemHandle);

    if (drmAgpBind(info->dri->drmFD,
		   info->dri->agpMemHandle, info->dri->gartOffset) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[agp] Could not bind\n");
	drmAgpFree(info->dri->drmFD, info->dri->agpMemHandle);
	drmAgpRelease(info->dri->drmFD);
	return FALSE;
    }

    if (drmAddMap(info->dri->drmFD, info->dri->ringStart, info->dri->ringMapSize,
		  DRM_AGP, DRM_READ_ONLY, &info->dri->ringHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not add ring mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] ring handle = 0x%08x\n",
	       (unsigned int)info->dri->ringHandle);

    if (drmMap(info->dri->drmFD, info->dri->ringHandle, info->dri->ringMapSize,
	       &info->dri->ring) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[agp] Could not map ring\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] Ring mapped at 0x%08lx\n",
	       (unsigned long)info->dri->ring);

    if (drmAddMap(info->dri->drmFD, info->dri->ringReadOffset, info->dri->ringReadMapSize,
		  DRM_AGP, DRM_READ_ONLY, &info->dri->ringReadPtrHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not add ring read ptr mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
 	       "[agp] ring read ptr handle = 0x%08x\n",
	       (unsigned int)info->dri->ringReadPtrHandle);

    if (drmMap(info->dri->drmFD, info->dri->ringReadPtrHandle, info->dri->ringReadMapSize,
	       &info->dri->ringReadPtr) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not map ring read ptr\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] Ring read ptr mapped at 0x%08lx\n",
	       (unsigned long)info->dri->ringReadPtr);

    if (drmAddMap(info->dri->drmFD, info->dri->bufStart, info->dri->bufMapSize,
		  DRM_AGP, 0, &info->dri->bufHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not add vertex/indirect buffers mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
 	       "[agp] vertex/indirect buffers handle = 0x%08x\n",
	       (unsigned int)info->dri->bufHandle);

    if (drmMap(info->dri->drmFD, info->dri->bufHandle, info->dri->bufMapSize,
	       &info->dri->buf) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not map vertex/indirect buffers\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] Vertex/indirect buffers mapped at 0x%08lx\n",
	       (unsigned long)info->dri->buf);

    if (drmAddMap(info->dri->drmFD, info->dri->gartTexStart, info->dri->gartTexMapSize,
		  DRM_AGP, 0, &info->dri->gartTexHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not add GART texture map mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
 	       "[agp] GART texture map handle = 0x%08x\n",
	       (unsigned int)info->dri->gartTexHandle);

    if (drmMap(info->dri->drmFD, info->dri->gartTexHandle, info->dri->gartTexMapSize,
	       &info->dri->gartTex) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] Could not map GART texture map\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[agp] GART Texture map mapped at 0x%08lx\n",
	       (unsigned long)info->dri->gartTex);

    RADEONSetAgpBase(info, pScreen);

    return TRUE;
}

/* Initialize the PCI GART state.  Request memory for use in PCI space,
 * and initialize the Radeon registers to point to that memory.
 */
static Bool RADEONDRIPciInit(RADEONInfoPtr info, ScreenPtr pScreen)
{
    int  ret;
    int  flags = DRM_READ_ONLY | DRM_LOCKED | DRM_KERNEL;

    ret = drmScatterGatherAlloc(info->dri->drmFD, info->dri->gartSize*1024*1024,
				&info->dri->pciMemHandle);
    if (ret < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[pci] Out of memory (%d)\n", ret);
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] %d kB allocated with handle 0x%08x\n",
	       info->dri->gartSize*1024,
	       (unsigned int)info->dri->pciMemHandle);

    RADEONDRIInitGARTValues(info);

    if (drmAddMap(info->dri->drmFD, info->dri->ringStart, info->dri->ringMapSize,
		  DRM_SCATTER_GATHER, flags, &info->dri->ringHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not add ring mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] ring handle = 0x%08x\n",
	       (unsigned int)info->dri->ringHandle);

    if (drmMap(info->dri->drmFD, info->dri->ringHandle, info->dri->ringMapSize,
	       &info->dri->ring) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[pci] Could not map ring\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] Ring mapped at 0x%08lx\n",
	       (unsigned long)info->dri->ring);
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] Ring contents 0x%08lx\n",
	       *(unsigned long *)(pointer)info->dri->ring);

    if (drmAddMap(info->dri->drmFD, info->dri->ringReadOffset, info->dri->ringReadMapSize,
		  DRM_SCATTER_GATHER, flags, &info->dri->ringReadPtrHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not add ring read ptr mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
 	       "[pci] ring read ptr handle = 0x%08x\n",
	       (unsigned int)info->dri->ringReadPtrHandle);

    if (drmMap(info->dri->drmFD, info->dri->ringReadPtrHandle, info->dri->ringReadMapSize,
	       &info->dri->ringReadPtr) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not map ring read ptr\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] Ring read ptr mapped at 0x%08lx\n",
	       (unsigned long)info->dri->ringReadPtr);
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] Ring read ptr contents 0x%08lx\n",
	       *(unsigned long *)(pointer)info->dri->ringReadPtr);

    if (drmAddMap(info->dri->drmFD, info->dri->bufStart, info->dri->bufMapSize,
		  DRM_SCATTER_GATHER, 0, &info->dri->bufHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not add vertex/indirect buffers mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
 	       "[pci] vertex/indirect buffers handle = 0x%08x\n",
	       (unsigned int)info->dri->bufHandle);

    if (drmMap(info->dri->drmFD, info->dri->bufHandle, info->dri->bufMapSize,
	       &info->dri->buf) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not map vertex/indirect buffers\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] Vertex/indirect buffers mapped at 0x%08lx\n",
	       (unsigned long)info->dri->buf);
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] Vertex/indirect buffers contents 0x%08lx\n",
	       *(unsigned long *)(pointer)info->dri->buf);

    if (drmAddMap(info->dri->drmFD, info->dri->gartTexStart, info->dri->gartTexMapSize,
		  DRM_SCATTER_GATHER, 0, &info->dri->gartTexHandle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not add GART texture map mapping\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
 	       "[pci] GART texture map handle = 0x%08x\n",
	       (unsigned int)info->dri->gartTexHandle);

    if (drmMap(info->dri->drmFD, info->dri->gartTexHandle, info->dri->gartTexMapSize,
	       &info->dri->gartTex) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] Could not map GART texture map\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[pci] GART Texture map mapped at 0x%08lx\n",
	       (unsigned long)info->dri->gartTex);

    return TRUE;
}

/* Initialize the kernel data structures */
static int RADEONDRIKernelInit(RADEONInfoPtr info, ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    int            cpp   = info->CurrentLayout.pixel_bytes;
    drm_radeon_init_t  drmInfo;

    memset(&drmInfo, 0, sizeof(drm_radeon_init_t));
    if ( info->ChipFamily >= CHIP_FAMILY_R600 )
	drmInfo.func             = RADEON_INIT_R600_CP;
    else if ( info->ChipFamily >= CHIP_FAMILY_R300 )
	drmInfo.func             = RADEON_INIT_R300_CP;
    else if ( info->ChipFamily >= CHIP_FAMILY_R200 )
	drmInfo.func		= RADEON_INIT_R200_CP;
    else
	drmInfo.func		= RADEON_INIT_CP;

    drmInfo.sarea_priv_offset   = sizeof(XF86DRISAREARec);
    drmInfo.is_pci              = (info->cardType!=CARD_AGP);
    drmInfo.cp_mode             = RADEON_CSQ_PRIBM_INDBM;
    drmInfo.gart_size           = info->dri->gartSize*1024*1024;
    drmInfo.ring_size           = info->dri->ringSize*1024*1024;
    drmInfo.usec_timeout        = info->cp->CPusecTimeout;

    drmInfo.fb_bpp              = info->CurrentLayout.pixel_code;
    drmInfo.depth_bpp           = (info->dri->depthBits - 8) * 2;

    drmInfo.front_offset        = info->dri->frontOffset;
    drmInfo.front_pitch         = info->dri->frontPitch * cpp;
    drmInfo.back_offset         = info->dri->backOffset;
    drmInfo.back_pitch          = info->dri->backPitch * cpp;
    drmInfo.depth_offset        = info->dri->depthOffset;
    drmInfo.depth_pitch         = info->dri->depthPitch * drmInfo.depth_bpp / 8;

    drmInfo.fb_offset           = info->dri->fbHandle;
    drmInfo.mmio_offset         = -1;
    drmInfo.ring_offset         = info->dri->ringHandle;
    drmInfo.ring_rptr_offset    = info->dri->ringReadPtrHandle;
    drmInfo.buffers_offset      = info->dri->bufHandle;
    drmInfo.gart_textures_offset= info->dri->gartTexHandle;

    if (drmCommandWrite(info->dri->drmFD, DRM_RADEON_CP_INIT,
			&drmInfo, sizeof(drm_radeon_init_t)) < 0)
	return FALSE;

    /* DRM_RADEON_CP_INIT does an engine reset, which resets some engine
     * registers back to their default values, so we need to restore
     * those engine register here.
     */
    if (info->ChipFamily < CHIP_FAMILY_R600)
	RADEONEngineRestore(pScrn);

    return TRUE;
}

static void RADEONDRIGartHeapInit(RADEONInfoPtr info, ScreenPtr pScreen)
{
    drm_radeon_mem_init_heap_t drmHeap;

    /* Start up the simple memory manager for GART space */
    drmHeap.region = RADEON_MEM_REGION_GART;
    drmHeap.start  = 0;
    drmHeap.size   = info->dri->gartTexMapSize;

    if (drmCommandWrite(info->dri->drmFD, DRM_RADEON_INIT_HEAP,
			&drmHeap, sizeof(drmHeap))) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[drm] Failed to initialize GART heap manager\n");
    } else {
	xf86DrvMsg(pScreen->myNum, X_INFO,
		   "[drm] Initialized kernel GART heap manager, %d\n",
		   info->dri->gartTexMapSize);
    }
}

/* Add a map for the vertex buffers that will be accessed by any
 * DRI-based clients.
 */
static Bool RADEONDRIBufInit(RADEONInfoPtr info, ScreenPtr pScreen)
{
				/* Initialize vertex buffers */
    info->dri->bufNumBufs = drmAddBufs(info->dri->drmFD,
				       info->dri->bufMapSize / RADEON_BUFFER_SIZE,
				       RADEON_BUFFER_SIZE,
				       (info->cardType!=CARD_AGP) ? DRM_SG_BUFFER : DRM_AGP_BUFFER,
				       info->dri->bufStart);

    if (info->dri->bufNumBufs <= 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[drm] Could not create vertex/indirect buffers list\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[drm] Added %d %d byte vertex/indirect buffers\n",
	       info->dri->bufNumBufs, RADEON_BUFFER_SIZE);

    if (!(info->dri->buffers = drmMapBufs(info->dri->drmFD))) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[drm] Failed to map vertex/indirect buffers list\n");
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO,
	       "[drm] Mapped %d vertex/indirect buffers\n",
	       info->dri->buffers->count);

    return TRUE;
}

static void RADEONDRIIrqInit(RADEONInfoPtr info, ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);

    if (!info->dri->irq) {
	info->dri->irq = drmGetInterruptFromBusID(
	    info->dri->drmFD,
	    PCI_CFG_BUS(info->PciInfo),
	    PCI_CFG_DEV(info->PciInfo),
	    PCI_CFG_FUNC(info->PciInfo));

	if ((drmCtlInstHandler(info->dri->drmFD, info->dri->irq)) != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "[drm] failure adding irq handler, "
		       "there is a device already using that irq\n"
		       "[drm] falling back to irq-free operation\n");
	    info->dri->irq = 0;
	} else {
	    if (info->ChipFamily < CHIP_FAMILY_R600) {
		unsigned char *RADEONMMIO = info->MMIO;
		info->ModeReg->gen_int_cntl = INREG( RADEON_GEN_INT_CNTL );

		/* Let the DRM know it can safely disable the vblank interrupts */
		radeon_crtc_modeset_ioctl(XF86_CRTC_CONFIG_PTR(pScrn)->crtc[0],
					  FALSE);
		radeon_crtc_modeset_ioctl(XF86_CRTC_CONFIG_PTR(pScrn)->crtc[0],
					  TRUE);
	    }
	}
    }

    if (info->dri->irq)
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "[drm] dma control initialized, using IRQ %d\n",
		   info->dri->irq);
}


/* Initialize the CP state, and start the CP (if used by the X server) */
static void RADEONDRICPInit(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

				/* Turn on bus mastering */
    info->BusCntl &= ~RADEON_BUS_MASTER_DIS;

				/* Make sure the CP is on for the X server */
    RADEONCP_START(pScrn, info);
#ifdef USE_XAA
    if (!info->useEXA)
	info->accel_state->dst_pitch_offset = info->dri->frontPitchOffset;
#endif
}


/* Get the DRM version and do some basic useability checks of DRI */
Bool RADEONDRIGetVersion(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info    = RADEONPTR(pScrn);
    int            major, minor, patch, fd;
    int            req_major, req_minor, req_patch;
    char           *busId;

    /* Check that the GLX, DRI, and DRM modules have been loaded by testing
     * for known symbols in each module.
     */
    if (!xf86LoaderCheckSymbol("GlxSetVisualConfigs")) return FALSE;
    if (!xf86LoaderCheckSymbol("drmAvailable"))        return FALSE;
    if (!xf86LoaderCheckSymbol("DRIQueryVersion")) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		 "[dri] RADEONDRIGetVersion failed (libdri too old)\n"
		 "[dri] Disabling DRI.\n");
      return FALSE;
    }

    /* Check the DRI version */
    DRIQueryVersion(&major, &minor, &patch);
    if (major < DRIINFO_MAJOR_VERSION) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
            "[dri] RADEONDRIGetVersion failed because of a version mismatch.\n"
            "[dri] This driver was built with %d.%d.x, which is too new;\n"
            "[dri] libdri reports a version of %d.%d.%d."
            "[dri] A server upgrade may be needed.\n"
            "[dri] Disabling DRI.\n",
            DRIINFO_MAJOR_VERSION, 0,
            major, minor, patch);
        return FALSE;
    } else if (major > DRIINFO_MAJOR_VERSION) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
            "[dri] RADEONDRIGetVersion failed because of a version mismatch.\n"
            "[dri] This driver was built with %d.%d.x, which is too old;\n"
            "[dri] libdri reports a version of %d.%d.%d."
            "[dri] This driver needs to be upgraded/rebuilt.\n"
            "[dri] Disabling DRI.\n",
            DRIINFO_MAJOR_VERSION, 0,
            major, minor, patch);
        return FALSE;
    }

    /* Check the lib version */
    if (xf86LoaderCheckSymbol("drmGetLibVersion"))
	info->dri->pLibDRMVersion = drmGetLibVersion(info->dri->drmFD);
    if (info->dri->pLibDRMVersion == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "[dri] RADEONDRIGetVersion failed because libdrm is really "
		   "way to old to even get a version number out of it.\n"
		   "[dri] Disabling DRI.\n");
	return FALSE;
    }
    if (info->dri->pLibDRMVersion->version_major != 1 ||
	info->dri->pLibDRMVersion->version_minor < 2) {
	    /* incompatible drm library version */
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "[dri] RADEONDRIGetVersion failed because of a "
		   "version mismatch.\n"
		   "[dri] libdrm module version is %d.%d.%d but "
		   "version 1.2.x is needed.\n"
		   "[dri] Disabling DRI.\n",
		   info->dri->pLibDRMVersion->version_major,
		   info->dri->pLibDRMVersion->version_minor,
		   info->dri->pLibDRMVersion->version_patchlevel);
	drmFreeVersion(info->dri->pLibDRMVersion);
	info->dri->pLibDRMVersion = NULL;
	return FALSE;
    }

    /* Create a bus Id */
    if (xf86LoaderCheckSymbol("DRICreatePCIBusID")) {
	busId = DRICreatePCIBusID(info->PciInfo);
    } else {
	busId = malloc(64);
	sprintf(busId,
		"PCI:%d:%d:%d",
		PCI_DEV_BUS(info->PciInfo),
		PCI_DEV_DEV(info->PciInfo),
		PCI_DEV_FUNC(info->PciInfo));
    }

    /* Low level DRM open */
    fd = drmOpen(RADEON_DRIVER_NAME, busId);
    free(busId);
    if (fd < 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "[dri] RADEONDRIGetVersion failed to open the DRM\n"
		   "[dri] Disabling DRI.\n");
	return FALSE;
    }

    /* Get DRM version & close DRM */
    info->dri->pKernelDRMVersion = drmGetVersion(fd);
    drmClose(fd);
    if (info->dri->pKernelDRMVersion == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "[dri] RADEONDRIGetVersion failed to get the DRM version\n"
		   "[dri] Disabling DRI.\n");
	return FALSE;
    }

    /* Now check if we qualify */
    req_major = 1;
    if (info->ChipFamily >= CHIP_FAMILY_R300) {
        req_minor = 17;
        req_patch = 0;
    } else if (info->IsIGP) {
        req_minor = 10;
        req_patch = 0;
    } else { /* Many problems have been reported with 1.7 in the 2.4 kernel */
        req_minor = 8;
        req_patch = 0;
    }

    /* We don't, bummer ! */
    if (info->dri->pKernelDRMVersion->version_major != req_major) {
        /* Looks like we're trying to start in UMS mode on a KMS kernel.
	 * This can happen if the radeon kernel module wasn't loaded before
	 * X starts.
	 */
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
            "[dri] RADEONDRIGetVersion failed because of a version mismatch.\n"
            "[dri] This chipset requires a kernel module version of %d.%d.%d,\n"
            "[dri] but the kernel reports a version of %d.%d.%d."
            "[dri] Make sure your module is loaded prior to starting X, and\n"
            "[dri] that this driver was built with support for KMS.\n"
            "[dri] Aborting.\n",
            req_major, req_minor, req_patch,
            info->dri->pKernelDRMVersion->version_major,
            info->dri->pKernelDRMVersion->version_minor,
            info->dri->pKernelDRMVersion->version_patchlevel);
        drmFreeVersion(info->dri->pKernelDRMVersion);
        info->dri->pKernelDRMVersion = NULL;
        return -1;
    } else if (info->dri->pKernelDRMVersion->version_minor < req_minor ||
        (info->dri->pKernelDRMVersion->version_minor == req_minor &&
        info->dri->pKernelDRMVersion->version_patchlevel < req_patch)) {
        /* Incompatible drm version */
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
            "[dri] RADEONDRIGetVersion failed because of a version mismatch.\n"
            "[dri] This chipset requires a kernel module version of %d.%d.%d,\n"
            "[dri] but the kernel reports a version of %d.%d.%d."
            "[dri] Try upgrading your kernel.\n"
            "[dri] Disabling DRI.\n",
            req_major, req_minor, req_patch,
            info->dri->pKernelDRMVersion->version_major,
            info->dri->pKernelDRMVersion->version_minor,
            info->dri->pKernelDRMVersion->version_patchlevel);
        drmFreeVersion(info->dri->pKernelDRMVersion);
        info->dri->pKernelDRMVersion = NULL;
        return FALSE;
    }

    return TRUE;
}

Bool RADEONDRISetVBlankInterrupt(ScrnInfoPtr pScrn, Bool on)
{
    RADEONInfoPtr  info    = RADEONPTR(pScrn);
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int value = 0;

    if (!info->want_vblank_interrupts)
        on = FALSE;

    if (info->directRenderingEnabled && info->dri->pKernelDRMVersion->version_minor >= 28) {
        if (on) {
  	    if (xf86_config->num_crtc > 1 && xf86_config->crtc[1]->enabled)
	        value = DRM_RADEON_VBLANK_CRTC1 | DRM_RADEON_VBLANK_CRTC2;
	    else
	        value = DRM_RADEON_VBLANK_CRTC1;
	}

	if (RADEONDRISetParam(pScrn, RADEON_SETPARAM_VBLANK_CRTC, value)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "RADEON Vblank Crtc Setup Failed %d\n", value);
	    return FALSE;
	}
    }
    return TRUE;
}


/* Initialize the screen-specific data structures for the DRI and the
 * Radeon.  This is the main entry point to the device-specific
 * initialization code.  It calls device-independent DRI functions to
 * create the DRI data structures and initialize the DRI state.
 */
Bool RADEONDRIScreenInit(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn   = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info    = RADEONPTR(pScrn);
    DRIInfoPtr     pDRIInfo;
    RADEONDRIPtr   pRADEONDRI;

    info->dri->DRICloseScreen = NULL;

    switch (info->CurrentLayout.pixel_code) {
    case 8:
    case 15:
    case 24:
	/* These modes are not supported (yet). */
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[dri] RADEONInitVisualConfigs failed "
		   "(depth %d not supported).  "
		   "Disabling DRI.\n", info->CurrentLayout.pixel_code);
	return FALSE;

	/* Only 16 and 32 color depths are supports currently. */
    case 16:
    case 32:
	break;
    }

    radeon_drm_page_size = getpagesize();

    /* Create the DRI data structure, and fill it in before calling the
     * DRIScreenInit().
     */
    if (!(pDRIInfo = DRICreateInfoRec())) return FALSE;

    info->dri->pDRIInfo                       = pDRIInfo;
    pDRIInfo->drmDriverName              = RADEON_DRIVER_NAME;

    if ( (info->ChipFamily >= CHIP_FAMILY_R600) )
       pDRIInfo->clientDriverName        = R600_DRIVER_NAME;
    else if ( (info->ChipFamily >= CHIP_FAMILY_R300) )
       pDRIInfo->clientDriverName        = R300_DRIVER_NAME;
    else if ( info->ChipFamily >= CHIP_FAMILY_R200 )
       pDRIInfo->clientDriverName	 = R200_DRIVER_NAME;
    else
       pDRIInfo->clientDriverName	 = RADEON_DRIVER_NAME;

    if (xf86LoaderCheckSymbol("DRICreatePCIBusID")) {
	pDRIInfo->busIdString = DRICreatePCIBusID(info->PciInfo);
    } else {
	pDRIInfo->busIdString            = malloc(64);
	sprintf(pDRIInfo->busIdString,
		"PCI:%d:%d:%d",
		PCI_DEV_BUS(info->PciInfo),
		PCI_DEV_DEV(info->PciInfo),
		PCI_DEV_FUNC(info->PciInfo));
    }
    pDRIInfo->ddxDriverMajorVersion      = info->allowColorTiling ? 5 : 4;
    pDRIInfo->ddxDriverMinorVersion      = 3;
    pDRIInfo->ddxDriverPatchVersion      = 0;
    pDRIInfo->frameBufferPhysicalAddress = (void *)(uintptr_t)info->LinearAddr + info->dri->frontOffset;
    pDRIInfo->frameBufferSize            = info->FbMapSize - info->FbSecureSize;
    pDRIInfo->frameBufferStride          = (pScrn->displayWidth *
					    info->CurrentLayout.pixel_bytes);
    pDRIInfo->ddxDrawableTableEntry      = RADEON_MAX_DRAWABLES;
    pDRIInfo->maxDrawableTableEntry      = (SAREA_MAX_DRAWABLES
					    < RADEON_MAX_DRAWABLES
					    ? SAREA_MAX_DRAWABLES
					    : RADEON_MAX_DRAWABLES);
    /* kill DRIAdjustFrame. We adjust sarea frame info ourselves to work
       correctly with pageflip + mergedfb/color tiling */
    pDRIInfo->wrap.AdjustFrame = NULL;

#ifdef NOT_DONE
    /* FIXME: Need to extend DRI protocol to pass this size back to
     * client for SAREA mapping that includes a device private record
     */
    pDRIInfo->SAREASize = ((sizeof(XF86DRISAREARec) + 0xfff)
			   & 0x1000); /* round to page */
    /* + shared memory device private rec */
#else
    /* For now the mapping works by using a fixed size defined
     * in the SAREA header
     */
    if (sizeof(XF86DRISAREARec)+sizeof(drm_radeon_sarea_t) > SAREA_MAX) {
	ErrorF("Data does not fit in SAREA\n");
	return FALSE;
    }
    pDRIInfo->SAREASize = SAREA_MAX;
#endif

    if (!(pRADEONDRI = (RADEONDRIPtr)calloc(sizeof(RADEONDRIRec),1))) {
	DRIDestroyInfoRec(info->dri->pDRIInfo);
	info->dri->pDRIInfo = NULL;
	return FALSE;
    }
    pDRIInfo->devPrivate     = pRADEONDRI;
    pDRIInfo->devPrivateSize = sizeof(RADEONDRIRec);
    pDRIInfo->contextSize    = sizeof(RADEONDRIContextRec);

    pDRIInfo->CreateContext  = RADEONCreateContext;
    pDRIInfo->DestroyContext = RADEONDestroyContext;
    pDRIInfo->SwapContext    = RADEONDRISwapContext;
    pDRIInfo->InitBuffers    = RADEONDRIInitBuffers;
    pDRIInfo->MoveBuffers    = RADEONDRIMoveBuffers;
    pDRIInfo->bufferRequests = DRI_ALL_WINDOWS;
    pDRIInfo->TransitionTo2d = RADEONDRITransitionTo2d;
    pDRIInfo->TransitionTo3d = RADEONDRITransitionTo3d;
    pDRIInfo->TransitionSingleToMulti3D = RADEONDRITransitionSingleToMulti3d;
    pDRIInfo->TransitionMultiToSingle3D = RADEONDRITransitionMultiToSingle3d;
#if defined(DAMAGE) && (DRIINFO_MAJOR_VERSION > 5 ||	\
			(DRIINFO_MAJOR_VERSION == 5 &&	\
			 DRIINFO_MINOR_VERSION >= 1))
    pDRIInfo->ClipNotify     = RADEONDRIClipNotify;
#endif

    pDRIInfo->createDummyCtx     = TRUE;
    pDRIInfo->createDummyCtxPriv = FALSE;

#ifdef USE_EXA
    if (info->useEXA) {
#if DRIINFO_MAJOR_VERSION == 5 && DRIINFO_MINOR_VERSION >= 3
       int major, minor, patch;

       DRIQueryVersion(&major, &minor, &patch);

       if (minor >= 3)
#endif
#if DRIINFO_MAJOR_VERSION > 5 || \
    (DRIINFO_MAJOR_VERSION == 5 && DRIINFO_MINOR_VERSION >= 3)
	  pDRIInfo->texOffsetStart = RADEONTexOffsetStart;
#endif
    }
#endif

    if (!DRIScreenInit(pScreen, pDRIInfo, &info->dri->drmFD)) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[dri] DRIScreenInit failed.  Disabling DRI.\n");
	free(pDRIInfo->devPrivate);
	pDRIInfo->devPrivate = NULL;
	DRIDestroyInfoRec(pDRIInfo);
	pDRIInfo = NULL;
	return FALSE;
    }
				/* Initialize AGP */
    if (info->cardType==CARD_AGP && !RADEONDRIAgpInit(info, pScreen)) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[agp] AGP failed to initialize. Disabling the DRI.\n" );
	xf86DrvMsg(pScreen->myNum, X_INFO,
		   "[agp] You may want to make sure the agpgart kernel "
		   "module\nis loaded before the radeon kernel module.\n");
	RADEONDRICloseScreen(pScreen);
	return FALSE;
    }

				/* Initialize PCI */
    if ((info->cardType!=CARD_AGP) && !RADEONDRIPciInit(info, pScreen)) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[pci] PCI failed to initialize. Disabling the DRI.\n" );
	RADEONDRICloseScreen(pScreen);
	return FALSE;
    }

				/* DRIScreenInit adds the frame buffer
				   map, but we need it as well */
    {
	void *scratch_ptr;
        int scratch_int;

	DRIGetDeviceInfo(pScreen, &info->dri->fbHandle,
                         &scratch_int, &scratch_int,
                         &scratch_int, &scratch_int,
                         &scratch_ptr);
    }

				/* FIXME: When are these mappings unmapped? */

    if (!RADEONInitVisualConfigs(pScreen)) {
	RADEONDRICloseScreen(pScreen);
	return FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] Visual configs initialized\n");

    return TRUE;
}

static Bool RADEONDRIDoCloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);

    RADEONDRICloseScreen(pScreen);

    pScreen->CloseScreen = info->dri->DRICloseScreen;
    return (*pScreen->CloseScreen)(CLOSE_SCREEN_ARGS);
}

/* Finish initializing the device-dependent DRI state, and call
 * DRIFinishScreenInit() to complete the device-independent DRI
 * initialization.
 */
Bool RADEONDRIFinishScreenInit(ScreenPtr pScreen)
{
    ScrnInfoPtr         pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr       info  = RADEONPTR(pScrn);
    drm_radeon_sarea_t  *pSAREAPriv;
    RADEONDRIPtr        pRADEONDRI;

    info->dri->pDRIInfo->driverSwapMethod = DRI_HIDE_X_CONTEXT;
    /* info->dri->pDRIInfo->driverSwapMethod = DRI_SERVER_SWAP; */

    /* NOTE: DRIFinishScreenInit must be called before *DRIKernelInit
     * because *DRIKernelInit requires that the hardware lock is held by
     * the X server, and the first time the hardware lock is grabbed is
     * in DRIFinishScreenInit.
     */
    if (!DRIFinishScreenInit(pScreen)) {
	RADEONDRICloseScreen(pScreen);
	return FALSE;
    }

    /* Initialize the kernel data structures */
    if (!RADEONDRIKernelInit(info, pScreen)) {
	RADEONDRICloseScreen(pScreen);
	return FALSE;
    }

    /* Initialize the vertex buffers list */
    if (!RADEONDRIBufInit(info, pScreen)) {
	RADEONDRICloseScreen(pScreen);
	return FALSE;
    }

    /* Initialize IRQ */
    RADEONDRIIrqInit(info, pScreen);

    /* Initialize kernel GART memory manager */
    RADEONDRIGartHeapInit(info, pScreen);

    /* Initialize and start the CP if required */
    RADEONDRICPInit(pScrn);

    /* Initialize the SAREA private data structure */
    pSAREAPriv = (drm_radeon_sarea_t*)DRIGetSAREAPrivate(pScreen);
    memset(pSAREAPriv, 0, sizeof(*pSAREAPriv));

    pRADEONDRI                    = (RADEONDRIPtr)info->dri->pDRIInfo->devPrivate;

    pRADEONDRI->deviceID          = info->Chipset;
    pRADEONDRI->width             = pScrn->virtualX;
    pRADEONDRI->height            = pScrn->virtualY;
    pRADEONDRI->depth             = pScrn->depth;
    pRADEONDRI->bpp               = pScrn->bitsPerPixel;

    pRADEONDRI->IsPCI             = (info->cardType!=CARD_AGP);
    pRADEONDRI->AGPMode           = info->dri->agpMode;

    pRADEONDRI->frontOffset       = info->dri->frontOffset;
    pRADEONDRI->frontPitch        = info->dri->frontPitch;
    pRADEONDRI->backOffset        = info->dri->backOffset;
    pRADEONDRI->backPitch         = info->dri->backPitch;
    pRADEONDRI->depthOffset       = info->dri->depthOffset;
    pRADEONDRI->depthPitch        = info->dri->depthPitch;
    pRADEONDRI->textureOffset     = info->dri->textureOffset;
    pRADEONDRI->textureSize       = info->dri->textureSize;
    pRADEONDRI->log2TexGran       = info->dri->log2TexGran;

    pRADEONDRI->registerHandle    = -1;
    pRADEONDRI->registerSize      = -1;

    pRADEONDRI->statusHandle      = info->dri->ringReadPtrHandle;
    pRADEONDRI->statusSize        = info->dri->ringReadMapSize;

    pRADEONDRI->gartTexHandle     = info->dri->gartTexHandle;
    pRADEONDRI->gartTexMapSize    = info->dri->gartTexMapSize;
    pRADEONDRI->log2GARTTexGran   = info->dri->log2GARTTexGran;
    pRADEONDRI->gartTexOffset     = info->dri->gartTexStart;

    pRADEONDRI->sarea_priv_offset = sizeof(XF86DRISAREARec);

    info->directRenderingInited = TRUE;

    /* Wrap CloseScreen */
    info->dri->DRICloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = RADEONDRIDoCloseScreen;

    /* disable vblank at startup */
    RADEONDRISetVBlankInterrupt (pScrn, FALSE);

    return TRUE;
}

/**
 * This function will attempt to get the Radeon hardware back into shape
 * after a resume from disc.
 *
 * Charl P. Botha <http://cpbotha.net>
 */
void RADEONDRIResume(ScreenPtr pScreen)
{
    int _ret;
    ScrnInfoPtr   pScrn   = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info    = RADEONPTR(pScrn);

    if (info->dri->pKernelDRMVersion->version_minor >= 9) {
	xf86DrvMsg(pScreen->myNum, X_INFO,
		   "[RESUME] Attempting to re-init Radeon hardware.\n");
    } else {
	xf86DrvMsg(pScreen->myNum, X_WARNING,
		   "[RESUME] Cannot re-init Radeon hardware, DRM too old\n"
		   "(need 1.9.0  or newer)\n");
	return;
    }

    if (info->cardType==CARD_AGP) {
	if (!RADEONSetAgpMode(info, pScreen))
	    return;

	RADEONSetAgpBase(info, pScreen);
    }

    _ret = drmCommandNone(info->dri->drmFD, DRM_RADEON_CP_RESUME);
    if (_ret) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "%s: CP resume %d\n", __FUNCTION__, _ret);
	/* FIXME: return? */
    }

    if (info->ChipFamily < CHIP_FAMILY_R600)
	RADEONEngineRestore(pScrn);

    RADEONDRICPInit(pScrn);
}

void RADEONDRIStop(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    RING_LOCALS;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONDRIStop\n");

    /* Stop the CP */
    if (info->directRenderingInited) {
	/* If we've generated any CP commands, we must flush them to the
	 * kernel module now.
	 */
	RADEONCP_RELEASE(pScrn, info);
	RADEONCP_STOP(pScrn, info);
    }
    info->directRenderingInited = FALSE;
}

/* The screen is being closed, so clean up any state and free any
 * resources used by the DRI.
 */
void RADEONDRICloseScreen(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    drm_radeon_init_t  drmInfo;

     xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		    "RADEONDRICloseScreen\n");

#ifdef DAMAGE
     REGION_UNINIT(pScreen, &info->dri->driRegion);
#endif

     if (info->dri->irq) {
	RADEONDRISetVBlankInterrupt (pScrn, FALSE);
	drmCtlUninstHandler(info->dri->drmFD);
	info->dri->irq = 0;
	info->ModeReg->gen_int_cntl = 0;
    }

    /* De-allocate vertex buffers */
    if (info->dri->buffers) {
	drmUnmapBufs(info->dri->buffers);
	info->dri->buffers = NULL;
    }

    /* De-allocate all kernel resources */
    memset(&drmInfo, 0, sizeof(drm_radeon_init_t));
    drmInfo.func = RADEON_CLEANUP_CP;
    drmCommandWrite(info->dri->drmFD, DRM_RADEON_CP_INIT,
		    &drmInfo, sizeof(drm_radeon_init_t));

    /* De-allocate all GART resources */
    if (info->dri->gartTex) {
	drmUnmap(info->dri->gartTex, info->dri->gartTexMapSize);
	info->dri->gartTex = NULL;
    }
    if (info->dri->buf) {
	drmUnmap(info->dri->buf, info->dri->bufMapSize);
	info->dri->buf = NULL;
    }
    if (info->dri->ringReadPtr) {
	drmUnmap(info->dri->ringReadPtr, info->dri->ringReadMapSize);
	info->dri->ringReadPtr = NULL;
    }
    if (info->dri->ring) {
	drmUnmap(info->dri->ring, info->dri->ringMapSize);
	info->dri->ring = NULL;
    }
    if (info->dri->agpMemHandle != DRM_AGP_NO_HANDLE) {
	drmAgpUnbind(info->dri->drmFD, info->dri->agpMemHandle);
	drmAgpFree(info->dri->drmFD, info->dri->agpMemHandle);
	info->dri->agpMemHandle = DRM_AGP_NO_HANDLE;
	drmAgpRelease(info->dri->drmFD);
    }
    if (info->dri->pciMemHandle) {
	drmScatterGatherFree(info->dri->drmFD, info->dri->pciMemHandle);
	info->dri->pciMemHandle = 0;
    }

    if (info->dri->pciGartBackup) {
	free(info->dri->pciGartBackup);
	info->dri->pciGartBackup = NULL;
    }

    /* De-allocate all DRI resources */
    DRICloseScreen(pScreen);

    /* De-allocate all DRI data structures */
    if (info->dri->pDRIInfo) {
	if (info->dri->pDRIInfo->devPrivate) {
	    free(info->dri->pDRIInfo->devPrivate);
	    info->dri->pDRIInfo->devPrivate = NULL;
	}
	DRIDestroyInfoRec(info->dri->pDRIInfo);
	info->dri->pDRIInfo = NULL;
    }
    if (info->dri->pVisualConfigs) {
	free(info->dri->pVisualConfigs);
	info->dri->pVisualConfigs = NULL;
    }
    if (info->dri->pVisualConfigsPriv) {
	free(info->dri->pVisualConfigsPriv);
	info->dri->pVisualConfigsPriv = NULL;
    }
}

/* Use callbacks from dri.c to support pageflipping mode for a single
 * 3d context without need for any specific full-screen extension.
 *
 * Also use these callbacks to allocate and free 3d-specific memory on
 * demand.
 */


#ifdef DAMAGE

/* Use the damage layer to maintain a list of dirty rectangles.
 * These are blitted to the back buffer to keep both buffers clean
 * during page-flipping when the 3d application isn't fullscreen.
 *
 * An alternative to this would be to organize for all on-screen drawing
 * operations to be duplicated for the two buffers.  That might be
 * faster, but seems like a lot more work...
 */


static void RADEONDRIRefreshArea(ScrnInfoPtr pScrn, RegionPtr pReg)
{
    RADEONInfoPtr       info       = RADEONPTR(pScrn);
    int                 i, num;
    ScreenPtr           pScreen    = pScrn->pScreen;
    drm_radeon_sarea_t  *pSAREAPriv = DRIGetSAREAPrivate(pScreen);
#ifdef USE_EXA
    PixmapPtr           pPix = pScreen->GetScreenPixmap(pScreen);
#endif
    RegionRec region;
    BoxPtr pbox;

    if (!info->directRenderingInited || !info->cp->CPStarted)
	return;

    /* Don't want to do this when no 3d is active and pages are
     * right-way-round
     */
    if (!pSAREAPriv->pfState && pSAREAPriv->pfCurrentPage == 0)
	return;

    REGION_NULL(pScreen, &region);
    REGION_SUBTRACT(pScreen, &region, pReg, &info->dri->driRegion);

    num = REGION_NUM_RECTS(&region);

    if (!num) {
	goto out;
    }

    pbox = REGION_RECTS(&region);

    /* pretty much a hack. */

#ifdef USE_EXA
    if (info->useEXA) {
	uint32_t src_pitch_offset, dst_pitch_offset, datatype;

	RADEONGetPixmapOffsetPitch(pPix, &src_pitch_offset);
	dst_pitch_offset = src_pitch_offset + (info->dri->backOffset >> 10);
	RADEONGetDatatypeBpp(pScrn->bitsPerPixel, &datatype);
	info->accel_state->xdir = info->accel_state->ydir = 1;

	RADEONDoPrepareCopyCP(pScrn, src_pitch_offset, dst_pitch_offset, datatype,
			      GXcopy, ~0);
    }
#endif

#ifdef USE_XAA
    if (!info->useEXA) {
	/* Make sure accel has been properly inited */
	if (info->accel_state->accel == NULL ||
	    info->accel_state->accel->SetupForScreenToScreenCopy == NULL)
	    goto out;
	if (info->tilingEnabled)
	    info->accel_state->dst_pitch_offset |= RADEON_DST_TILE_MACRO;
	(*info->accel_state->accel->SetupForScreenToScreenCopy)(pScrn,
								1, 1, GXcopy,
								(uint32_t)(-1), -1);
    }
#endif

    for (i = 0 ; i < num ; i++, pbox++) {
	int xa = max(pbox->x1, 0), xb = min(pbox->x2, pScrn->virtualX-1);
	int ya = max(pbox->y1, 0), yb = min(pbox->y2, pScrn->virtualY-1);

	if (xa <= xb && ya <= yb) {
#ifdef USE_EXA
	    if (info->useEXA) {
		RADEONCopyCP(pPix, xa, ya, xa, ya, xb - xa + 1, yb - ya + 1);
	    }
#endif

#ifdef USE_XAA
	    if (!info->useEXA) {
		(*info->accel_state->accel->SubsequentScreenToScreenCopy)(pScrn, xa, ya,
									  xa + info->dri->backX,
									  ya + info->dri->backY,
									  xb - xa + 1,
									  yb - ya + 1);
	    }
#endif
	}
    }

#ifdef USE_XAA
    info->accel_state->dst_pitch_offset &= ~RADEON_DST_TILE_MACRO;
#endif

out:
    REGION_NULL(pScreen, &region);
    DamageEmpty(info->dri->pDamage);
}

#endif /* DAMAGE */

static void RADEONEnablePageFlip(ScreenPtr pScreen)
{
#ifdef DAMAGE
    ScrnInfoPtr         pScrn      = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr       info       = RADEONPTR(pScrn);

    if (info->dri->allowPageFlip) {
	drm_radeon_sarea_t *pSAREAPriv = DRIGetSAREAPrivate(pScreen);
	BoxRec box = { .x1 = 0, .y1 = 0, .x2 = pScrn->virtualX - 1,
		       .y2 = pScrn->virtualY - 1 };
	RegionPtr pReg = REGION_CREATE(pScreen, &box, 1);

	pSAREAPriv->pfState = 1;
	RADEONDRIRefreshArea(pScrn, pReg);
	REGION_DESTROY(pScreen, pReg);
    }
#endif
}

static void RADEONDisablePageFlip(ScreenPtr pScreen)
{
    /* Tell the clients not to pageflip.  How?
     *   -- Field in sarea, plus bumping the window counters.
     *   -- DRM needs to cope with Front-to-Back swapbuffers.
     */
    drm_radeon_sarea_t  *pSAREAPriv = DRIGetSAREAPrivate(pScreen);

    pSAREAPriv->pfState = 0;
}

static void RADEONDRITransitionSingleToMulti3d(ScreenPtr pScreen)
{
    RADEONDisablePageFlip(pScreen);
}

static void RADEONDRITransitionMultiToSingle3d(ScreenPtr pScreen)
{
    /* Let the remaining 3d app start page flipping again */
    RADEONEnablePageFlip(pScreen);
}

static void RADEONDRITransitionTo3d(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
#ifdef USE_XAA
    FBAreaPtr      fbarea;
    int            width, height;

    /* EXA allocates these areas up front, so it doesn't do the following
     * stuff.
     */
    if (!info->useEXA) {
	/* reserve offscreen area for back and depth buffers and textures */

	/* If we still have an area for the back buffer reserved, free it
	 * first so we always start with all free offscreen memory, except
	 * maybe for Xv
	 */
	if (info->dri->backArea) {
	    xf86FreeOffscreenArea(info->dri->backArea);
	    info->dri->backArea = NULL;
        }

	xf86PurgeUnlockedOffscreenAreas(pScreen);

	xf86QueryLargestOffscreenArea(pScreen, &width, &height, 0, 0, 0);

	/* Free Xv linear offscreen memory if necessary
	 * FIXME: This is hideous.  What about telling xv "oh btw you have no memory
	 * any more?" -- anholt
	 */
	if (height < (info->dri->depthTexLines + info->dri->backLines)) {
	    RADEONPortPrivPtr portPriv = info->adaptor->pPortPrivates[0].ptr;
	    xf86FreeOffscreenLinear((FBLinearPtr)portPriv->video_memory);
	    portPriv->video_memory = NULL;
	    xf86QueryLargestOffscreenArea(pScreen, &width, &height, 0, 0, 0);
	}

	/* Reserve placeholder area so the other areas will match the
	 * pre-calculated offsets
	 * FIXME: We may have other locked allocations and thus this would allocate
	 * in the wrong place.  The XV surface allocations seem likely. -- anholt
	 */
	fbarea = xf86AllocateOffscreenArea(pScreen, pScrn->displayWidth,
					   height
					   - info->dri->depthTexLines
					   - info->dri->backLines,
					   pScrn->displayWidth,
					   NULL, NULL, NULL);
	if (!fbarea)
	    xf86DrvMsg(pScreen->myNum, X_ERROR, "Unable to reserve placeholder "
		       "offscreen area, you might experience screen corruption\n");

	info->dri->backArea = xf86AllocateOffscreenArea(pScreen, pScrn->displayWidth,
							info->dri->backLines,
							pScrn->displayWidth,
							NULL, NULL, NULL);
	if (!info->dri->backArea)
	    xf86DrvMsg(pScreen->myNum, X_ERROR, "Unable to reserve offscreen "
		       "area for back buffer, you might experience screen "
		       "corruption\n");

	info->dri->depthTexArea = xf86AllocateOffscreenArea(pScreen,
							    pScrn->displayWidth,
							    info->dri->depthTexLines,
							    pScrn->displayWidth,
							    NULL, NULL, NULL);
	if (!info->dri->depthTexArea)
	    xf86DrvMsg(pScreen->myNum, X_ERROR, "Unable to reserve offscreen "
		       "area for depth buffer and textures, you might "
		       "experience screen corruption\n");

	xf86FreeOffscreenArea(fbarea);
    }
#endif /* USE_XAA */

    info->dri->have3DWindows = 1;

    RADEONChangeSurfaces(pScrn);
    RADEONEnablePageFlip(pScreen);
    
    info->want_vblank_interrupts = TRUE;
    RADEONDRISetVBlankInterrupt(pScrn, TRUE);

    if (info->cursor)
	xf86ForceHWCursor (pScreen, TRUE);
}

static void RADEONDRITransitionTo2d(ScreenPtr pScreen)
{
    ScrnInfoPtr         pScrn      = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr       info       = RADEONPTR(pScrn);
    drm_radeon_sarea_t  *pSAREAPriv = DRIGetSAREAPrivate(pScreen);

    /* Try flipping back to the front page if necessary */
    if (pSAREAPriv->pfCurrentPage == 1)
	drmCommandNone(info->dri->drmFD, DRM_RADEON_FLIP);

    /* Shut down shadowing if we've made it back to the front page */
    if (pSAREAPriv->pfCurrentPage == 0) {
	RADEONDisablePageFlip(pScreen);
#ifdef USE_XAA
	if (!info->useEXA) {
	    xf86FreeOffscreenArea(info->dri->backArea);
	    info->dri->backArea = NULL;
	}
#endif
    } else {
	xf86DrvMsg(pScreen->myNum, X_WARNING,
		   "[dri] RADEONDRITransitionTo2d: "
		   "kernel failed to unflip buffers.\n");
    }

#ifdef USE_XAA
    if (!info->useEXA)
	xf86FreeOffscreenArea(info->dri->depthTexArea);
#endif

    info->dri->have3DWindows = 0;

    RADEONChangeSurfaces(pScrn);

    info->want_vblank_interrupts = FALSE;
    RADEONDRISetVBlankInterrupt(pScrn, FALSE);

    if (info->cursor)
	xf86ForceHWCursor (pScreen, FALSE);
}

#if defined(DAMAGE) && (DRIINFO_MAJOR_VERSION > 5 ||	\
			(DRIINFO_MAJOR_VERSION == 5 &&	\
			 DRIINFO_MINOR_VERSION >= 1))
static void
RADEONDRIClipNotify(ScreenPtr pScreen, WindowPtr *ppWin, int num)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);

    REGION_UNINIT(pScreen, &info->dri->driRegion);
    REGION_NULL(pScreen, &info->dri->driRegion);

    if (num > 0) {
	int i;

	for (i = 0; i < num; i++) {
	    WindowPtr pWin = ppWin[i];

	    if (pWin) {
		REGION_UNION(pScreen, &info->dri->driRegion, &pWin->clipList,
			     &info->dri->driRegion);
	    }
	}
    }
}
#endif

void RADEONDRIAllocatePCIGARTTable(ScreenPtr pScreen)
{
    ScrnInfoPtr        pScrn   = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr      info    = RADEONPTR(pScrn);

    if (info->cardType != CARD_PCIE ||
	info->dri->pKernelDRMVersion->version_minor < 19)
      return;

    if (info->FbSecureSize==0)
      return;

    /* set the old default size of pci gart table */
    if (info->dri->pKernelDRMVersion->version_minor < 26)
      info->dri->pciGartSize = 32768;

    info->dri->pciGartSize = RADEONDRIGetPciAperTableSize(pScrn);

    /* allocate space to back up PCIEGART table */
    info->dri->pciGartBackup = xnfcalloc(1, info->dri->pciGartSize);
    if (info->dri->pciGartBackup == NULL)
      return;

    info->dri->pciGartOffset = (info->FbMapSize - info->FbSecureSize);


}

int RADEONDRIGetPciAperTableSize(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    int ret_size;
    int num_pages;

    num_pages = (info->dri->pciAperSize * 1024 * 1024) / 4096;

    if ((info->ChipFamily >= CHIP_FAMILY_R600) ||
	(info->ChipFamily == CHIP_FAMILY_RS600))
	ret_size = num_pages * sizeof(uint64_t);
    else
	ret_size = num_pages * sizeof(unsigned int);

    return ret_size;
}

int RADEONDRISetParam(ScrnInfoPtr pScrn, unsigned int param, int64_t value)
{
    drm_radeon_setparam_t  radeonsetparam;
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    int ret;

    memset(&radeonsetparam, 0, sizeof(drm_radeon_setparam_t));
    radeonsetparam.param = param;
    radeonsetparam.value = value;
    ret = drmCommandWrite(info->dri->drmFD, DRM_RADEON_SETPARAM,
			  &radeonsetparam, sizeof(drm_radeon_setparam_t));
    return ret;
}
