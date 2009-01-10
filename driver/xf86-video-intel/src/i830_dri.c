/* $xfree86: xc/programs/Xserver/hw/xfree86/drivers/i810/i830_dri.c,v 1.15 2003/06/18 13:14:17 dawes Exp $ */
/**************************************************************************

Copyright 2001 VA Linux Systems Inc., Fremont, California.
Copyright © 2002 by David Dawes

All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
on the rights to use, copy, modify, merge, publish, distribute, sub
license, and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice (including the next
paragraph) shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
ATI, VA LINUX SYSTEMS AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

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

/*
 * Authors: Jeff Hartmann <jhartmann@valinux.com>
 *          David Dawes <dawes@xfree86.org>
 *          Keith Whitwell <keith@tungstengraphics.com>
 */

/*
 * This driver does AGP memory allocation a little differently from most
 * others.  The 2D and 3D allocations have been unified (see i830_memory.c).
 * The driver does the AGP allocations and binding directly, then passes
 * on the mappings to the DRM module.  The DRM's AGP interfaces are not used.
 * The main difference with this is that the offsets need to include
 * the AGP aperture base address because that won't be known or added on
 * by the DRM module.
 *
 * DHD 07/2002
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Priv.h"

#include "xf86PciInfo.h"
#include "xf86Pci.h"

#include "windowstr.h"
#include "shadow.h"

#include "GL/glxtokens.h"

#include "i830.h"
#include "i830_dri.h"

#include "i915_drm.h"

/* This block and the corresponding configure test can be removed when
 * libdrm >= 2.3.1 is required.
 */
#ifndef HAVE_I915_FLIP

#define DRM_VBLANK_FLIP 0x8000000

typedef struct drm_i915_flip {
   int pipes;
} drm_i915_flip_t;

#undef DRM_IOCTL_I915_FLIP
#define DRM_IOCTL_I915_FLIP DRM_IOW(DRM_COMMAND_BASE + DRM_I915_FLIP, \
				    drm_i915_flip_t)

#endif

#include "dristruct.h"

static Bool I830InitVisualConfigs(ScreenPtr pScreen);
static Bool I830CreateContext(ScreenPtr pScreen, VisualPtr visual,
			      drm_context_t hwContext, void *pVisualConfigPriv,
			      DRIContextType contextStore);
static void I830DestroyContext(ScreenPtr pScreen, drm_context_t hwContext,
			       DRIContextType contextStore);
static void I830DRISwapContext(ScreenPtr pScreen, DRISyncType syncType,
			       DRIContextType readContextType,
			       void *readContextStore,
			       DRIContextType writeContextType,
			       void *writeContextStore);
static void I830DRIInitBuffers(WindowPtr pWin, RegionPtr prgn, CARD32 index);
static void I830DRIMoveBuffers(WindowPtr pParent, DDXPointRec ptOldOrg,
			       RegionPtr prgnSrc, CARD32 index);

static void I830DRITransitionTo2d(ScreenPtr pScreen);
static void I830DRITransitionTo3d(ScreenPtr pScreen);
#if defined(DAMAGE) && (DRIINFO_MAJOR_VERSION > 5 ||		\
			(DRIINFO_MAJOR_VERSION == 5 && DRIINFO_MINOR_VERSION >= 1))
#define DRI_SUPPORTS_CLIP_NOTIFY 1
#else
#define DRI_SUPPORTS_CLIP_NOTIFY 0
static void I830DRITransitionMultiToSingle3d(ScreenPtr pScreen);
static void I830DRITransitionSingleToMulti3d(ScreenPtr pScreen);
#endif

#if (DRIINFO_MAJOR_VERSION > 5 || \
     (DRIINFO_MAJOR_VERSION == 5 && DRIINFO_MINOR_VERSION >= 4))
#define DRI_DRIVER_FRAMEBUFFER_MAP 1
#else
#define DRI_DRIVER_FRAMEBUFFER_MAP 0
#endif

#if DRI_SUPPORTS_CLIP_NOTIFY
static void I830DRIClipNotify(ScreenPtr pScreen, WindowPtr *ppWin, int num);
#endif

extern void GlxSetVisualConfigs(int nconfigs,
				__GLXvisualConfig * configs,
				void **configprivs);

static Bool
I830CleanupDma(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   drmI830Init info;

   memset(&info, 0, sizeof(drmI830Init));
   info.func = I830_CLEANUP_DMA;

   if (drmCommandWrite(pI830->drmSubFD, DRM_I830_INIT,
		       &info, sizeof(drmI830Init))) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "I830 Dma Cleanup Failed\n");
      return FALSE;
   }

   return TRUE;
}

static Bool
I830InitDma(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   I830RingBuffer *ring = pI830->LpRing;
   I830DRIPtr pI830DRI = (I830DRIPtr) pI830->pDRIInfo->devPrivate;
   drmI830Init info;

   memset(&info, 0, sizeof(drmI830Init));
   info.func = I830_INIT_DMA;

   info.ring_start = ring->mem->offset + pI830->LinearAddr;
   info.ring_end = ring->mem->end + pI830->LinearAddr;
   info.ring_size = ring->mem->size;

   info.mmio_offset = -1;

   info.sarea_priv_offset = sizeof(XF86DRISAREARec);

   info.front_offset = pI830->front_buffer->offset;
   info.back_offset = pI830->back_buffer->offset;
   info.depth_offset = pI830->depth_buffer->offset;
   info.w = pScrn->virtualX;
   info.h = pScrn->virtualY;
   info.pitch = pScrn->displayWidth;
   info.back_pitch = pScrn->displayWidth;
   info.depth_pitch = pScrn->displayWidth;
   info.cpp = pI830->cpp;

   if (drmCommandWrite(pI830->drmSubFD, DRM_I830_INIT,
		       &info, sizeof(drmI830Init))) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		 "I830 Dma Initialization Failed\n");
      return FALSE;
   }

   return TRUE;
}

static Bool
I830ResumeDma(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   drmI830Init info;

   memset(&info, 0, sizeof(drmI830Init));
   info.func = I830_RESUME_DMA;

   if (drmCommandWrite(pI830->drmSubFD, DRM_I830_INIT,
		       &info, sizeof(drmI830Init))) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "I830 Dma Resume Failed\n");
      return FALSE;
   }

   return TRUE;
}

static Bool
I830SetParam(ScrnInfoPtr pScrn, int param, int value)
{
   I830Ptr pI830 = I830PTR(pScrn);
   drmI830SetParam sp;

   memset(&sp, 0, sizeof(sp));
   sp.param = param;
   sp.value = value;

   if (drmCommandWrite(pI830->drmSubFD, DRM_I830_SETPARAM, &sp, sizeof(sp))) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "I830 SetParam Failed\n");
      return FALSE;
   }

   return TRUE;
}

Bool
I830DRISetHWS(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    drmI830HWS hws;

    hws.addr = pI830->hw_status->offset;

    if (drmCommandWrite(pI830->drmSubFD, DRM_I830_HWS_PAGE_ADDR,
		&hws, sizeof(drmI830HWS))) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"hw status page initialization Failed\n");
	return FALSE;
    }
    return TRUE;
}

static Bool
I830InitVisualConfigs(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   int numConfigs = 0;
   __GLXvisualConfig *pConfigs = NULL;
   I830ConfigPrivPtr pI830Configs = NULL;
   I830ConfigPrivPtr *pI830ConfigPtrs = NULL;
   int accum, stencil, db, depth;
   int i;

   switch (pScrn->bitsPerPixel) {
   case 8:
   case 24:
      break;

   case 16:
      numConfigs = 8;

      pConfigs =
	    (__GLXvisualConfig *) xcalloc(sizeof(__GLXvisualConfig),
					  numConfigs);
      if (!pConfigs)
	 return FALSE;

      pI830Configs =
	    (I830ConfigPrivPtr) xcalloc(sizeof(I830ConfigPrivRec),
					numConfigs);
      if (!pI830Configs) {
	 xfree(pConfigs);
	 return FALSE;
      }

      pI830ConfigPtrs =
	    (I830ConfigPrivPtr *) xcalloc(sizeof(I830ConfigPrivPtr),
					  numConfigs);
      if (!pI830ConfigPtrs) {
	 xfree(pConfigs);
	 xfree(pI830Configs);
	 return FALSE;
      }

      for (i = 0; i < numConfigs; i++)
	 pI830ConfigPtrs[i] = &pI830Configs[i];

      i = 0;
      depth = 1;
      for (accum = 0; accum <= 1; accum++) {
	 for (stencil = 0; stencil <= 1; stencil++) {
	    for (db = 1; db >= 0; db--) {
	       pConfigs[i].vid = -1;
	       pConfigs[i].class = -1;
	       pConfigs[i].rgba = TRUE;
	       pConfigs[i].redSize = 5;
	       pConfigs[i].greenSize = 6;
	       pConfigs[i].blueSize = 5;
	       pConfigs[i].alphaSize = 0;
	       pConfigs[i].redMask = 0x0000F800;
	       pConfigs[i].greenMask = 0x000007E0;
	       pConfigs[i].blueMask = 0x0000001F;
	       pConfigs[i].alphaMask = 0;
	       if (accum) {
		  pConfigs[i].accumRedSize = 16;
		  pConfigs[i].accumGreenSize = 16;
		  pConfigs[i].accumBlueSize = 16;
		  pConfigs[i].accumAlphaSize = 0;
	       } else {
		  pConfigs[i].accumRedSize = 0;
		  pConfigs[i].accumGreenSize = 0;
		  pConfigs[i].accumBlueSize = 0;
		  pConfigs[i].accumAlphaSize = 0;
	       }
	       pConfigs[i].doubleBuffer = db ? TRUE : FALSE;
	       pConfigs[i].stereo = FALSE;
	       pConfigs[i].bufferSize = 16;
	       if (depth)
		  pConfigs[i].depthSize = 16;
	       else
		  pConfigs[i].depthSize = 0;
	       if (stencil)
		  pConfigs[i].stencilSize = 8;
	       else
		  pConfigs[i].stencilSize = 0;
	       pConfigs[i].auxBuffers = 0;
	       pConfigs[i].level = 0;
	       if (stencil || accum)
		  pConfigs[i].visualRating = GLX_SLOW_VISUAL_EXT;
	       else
		  pConfigs[i].visualRating = GLX_NONE_EXT;
	       pConfigs[i].transparentPixel = GLX_NONE_EXT;
	       pConfigs[i].transparentRed = 0;
	       pConfigs[i].transparentGreen = 0;
	       pConfigs[i].transparentBlue = 0;
	       pConfigs[i].transparentAlpha = 0;
	       pConfigs[i].transparentIndex = 0;
	       i++;
	    }
	 }
      }
      assert(i == numConfigs);
      break;

   case 32:
      numConfigs = 8;

      pConfigs = (__GLXvisualConfig *) xcalloc(sizeof(__GLXvisualConfig),
					       numConfigs);
      if (!pConfigs) {
	 return FALSE;
      }

      pI830Configs = (I830ConfigPrivPtr) xcalloc(sizeof(I830ConfigPrivRec),
						 numConfigs);
      if (!pI830Configs) {
	 xfree(pConfigs);
	 return FALSE;
      }

      pI830ConfigPtrs = (I830ConfigPrivPtr *)
	    xcalloc(sizeof(I830ConfigPrivPtr), numConfigs);
      if (!pI830ConfigPtrs) {
	 xfree(pConfigs);
	 xfree(pI830Configs);
	 return FALSE;
      }

      for (i = 0; i < numConfigs; i++) {
	 pI830ConfigPtrs[i] = &pI830Configs[i];
      }

      i = 0;
      for (accum = 0; accum <= 1; accum++) {
	 for (depth = 1; depth >= 0; depth--) {	/* and stencil */
	    for (db = 1; db >= 0; db--) {
	       pConfigs[i].vid = -1;
	       pConfigs[i].class = -1;
	       pConfigs[i].rgba = TRUE;
	       pConfigs[i].redSize = 8;
	       pConfigs[i].greenSize = 8;
	       pConfigs[i].blueSize = 8;
	       pConfigs[i].alphaSize = 8;
	       pConfigs[i].redMask = 0x00FF0000;
	       pConfigs[i].greenMask = 0x0000FF00;
	       pConfigs[i].blueMask = 0x000000FF;
	       pConfigs[i].alphaMask = 0xFF000000;
	       if (accum) {
		  pConfigs[i].accumRedSize = 16;
		  pConfigs[i].accumGreenSize = 16;
		  pConfigs[i].accumBlueSize = 16;
		  pConfigs[i].accumAlphaSize = 16;
	       } else {
		  pConfigs[i].accumRedSize = 0;
		  pConfigs[i].accumGreenSize = 0;
		  pConfigs[i].accumBlueSize = 0;
		  pConfigs[i].accumAlphaSize = 0;
	       }
	       if (db) {
		  pConfigs[i].doubleBuffer = TRUE;
	       } else {
		  pConfigs[i].doubleBuffer = FALSE;
	       }
	       pConfigs[i].stereo = FALSE;
	       pConfigs[i].bufferSize = 32;
	       if (depth) {
		  pConfigs[i].depthSize = 24;
		  pConfigs[i].stencilSize = 8;
	       } else {
		  pConfigs[i].depthSize = 0;
		  pConfigs[i].stencilSize = 0;
	       }
	       pConfigs[i].auxBuffers = 0;
	       pConfigs[i].level = 0;
	       if (accum) {
		  pConfigs[i].visualRating = GLX_SLOW_VISUAL_EXT;
	       } else {
		  pConfigs[i].visualRating = GLX_NONE_EXT;
	       }
	       pConfigs[i].transparentPixel = GLX_NONE_EXT;
	       pConfigs[i].transparentRed = 0;
	       pConfigs[i].transparentGreen = 0;
	       pConfigs[i].transparentBlue = 0;
	       pConfigs[i].transparentAlpha = 0;
	       pConfigs[i].transparentIndex = 0;
	       i++;
	    }
	 }
      }
      if (i != numConfigs) {
	 xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "[drm] Incorrect initialization of visuals\n");
	 return FALSE;
      }
      break;

   }
   pI830->numVisualConfigs = numConfigs;
   pI830->pVisualConfigs = pConfigs;
   pI830->pVisualConfigsPriv = pI830Configs;
   GlxSetVisualConfigs(numConfigs, pConfigs, (void **)pI830ConfigPtrs);
   return TRUE;
}

Bool
I830CheckDRIAvailable(ScrnInfoPtr pScrn)
{
   /* Hardware 3D rendering only implemented for 16bpp and 32 bpp */
   if (((pScrn->bitsPerPixel / 8) != 2 && pScrn->depth != 16) &&
       (pScrn->bitsPerPixel / 8) != 4) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		 "[drm] Direct rendering only supported in 16 and 32 bpp modes\n");
      return FALSE;
   }

   /* Check that the GLX, DRI, and DRM modules have been loaded by testing
    * for known symbols in each module. */
   if (!xf86LoaderCheckSymbol("GlxSetVisualConfigs")) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		 "[dri] %s failed: glx not loaded\n", __FUNCTION__);
      return FALSE;
   }
   if (!xf86LoaderCheckSymbol("DRIScreenInit")) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		 "[dri] %s failed: dri not loaded\n", __FUNCTION__);
      return FALSE;
   }
   if (!xf86LoaderCheckSymbol("drmAvailable")) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		 "[dri] %s failed: libdrm not loaded\n", __FUNCTION__);
      return FALSE;
   }
   if (!xf86LoaderCheckSymbol("DRIQueryVersion")) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		 "[dri] %s failed (libdri.a too old)\n", __FUNCTION__);
      return FALSE;
   }

   /* Check the DRI version */
   {
      int major, minor, patch;

      DRIQueryVersion(&major, &minor, &patch);
      if (major != DRIINFO_MAJOR_VERSION || minor < DRIINFO_MINOR_VERSION ||
	  major < 5) {
	 xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "[dri] %s failed because of a version mismatch.\n"
		    "[dri] libDRI version is %d.%d.%d but version %d.%d.x is needed.\n"
		    "[dri] Disabling DRI.\n",
		    "I830CheckDRIAvailable", major, minor, patch,
		     DRIINFO_MAJOR_VERSION, DRIINFO_MINOR_VERSION);
	 return FALSE;
      }
   }

   return TRUE;
}

Bool
I830DRIScreenInit(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   DRIInfoPtr pDRIInfo;
   I830DRIPtr pI830DRI;

   DPRINTF(PFX, "I830DRIScreenInit\n");

   if (!I830CheckDRIAvailable(pScrn))
      return FALSE;

   pDRIInfo = DRICreateInfoRec();
   if (!pDRIInfo) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[dri] DRICreateInfoRec failed. Disabling DRI.\n");
      return FALSE;
   }

   pI830->pDRIInfo = pDRIInfo;
   pI830->LockHeld = 0;

   pDRIInfo->drmDriverName = "i915";
   if (IS_I965G(pI830))
      pDRIInfo->clientDriverName = "i965";
   else
      pDRIInfo->clientDriverName = "i915";

   if (xf86LoaderCheckSymbol("DRICreatePCIBusID")) {
      pDRIInfo->busIdString = DRICreatePCIBusID(pI830->PciInfo);
   } else {
      pDRIInfo->busIdString = xalloc(64);
      sprintf(pDRIInfo->busIdString, "PCI:%d:%d:%d",
#if XSERVER_LIBPCIACCESS
	      ((pI830->PciInfo->domain << 8) | pI830->PciInfo->bus),
	      pI830->PciInfo->dev, pI830->PciInfo->func
#else
	      ((pciConfigPtr) pI830->PciInfo->thisCard)->busnum,
	      ((pciConfigPtr) pI830->PciInfo->thisCard)->devnum,
	      ((pciConfigPtr) pI830->PciInfo->thisCard)->funcnum
#endif
	      );
   }
   pDRIInfo->ddxDriverMajorVersion = I830_MAJOR_VERSION;
   pDRIInfo->ddxDriverMinorVersion = I830_MINOR_VERSION;
   pDRIInfo->ddxDriverPatchVersion = I830_PATCHLEVEL;
   pDRIInfo->ddxDrawableTableEntry = I830_MAX_DRAWABLES;

   if (SAREA_MAX_DRAWABLES < I830_MAX_DRAWABLES)
      pDRIInfo->maxDrawableTableEntry = SAREA_MAX_DRAWABLES;
   else
      pDRIInfo->maxDrawableTableEntry = I830_MAX_DRAWABLES;

   if (sizeof(XF86DRISAREARec) + sizeof(drmI830Sarea) > SAREA_MAX) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[dri] Data does not fit in SAREA\n");
      return FALSE;
   }
   /* This is a hack for now.  We have to have more than a 4k page here
    * because of the size of the state.  However, the state should be
    * in a per-context mapping.  This will be added in the Mesa 3.5 port
    * of the I830 driver.
    */
   pDRIInfo->SAREASize = SAREA_MAX;

   if (!(pI830DRI = (I830DRIPtr) xcalloc(sizeof(I830DRIRec), 1))) {
      DRIDestroyInfoRec(pI830->pDRIInfo);
      pI830->pDRIInfo = NULL;
      return FALSE;
   }
   pDRIInfo->devPrivate = pI830DRI;
   pDRIInfo->devPrivateSize = sizeof(I830DRIRec);
   pDRIInfo->contextSize = sizeof(I830DRIContextRec);

   pDRIInfo->CreateContext = I830CreateContext;
   pDRIInfo->DestroyContext = I830DestroyContext;
   pDRIInfo->SwapContext = I830DRISwapContext;
   pDRIInfo->InitBuffers = I830DRIInitBuffers;
   pDRIInfo->MoveBuffers = I830DRIMoveBuffers;
   pDRIInfo->bufferRequests = DRI_ALL_WINDOWS;

#if DRIINFO_MAJOR_VERSION > 5 || \
    (DRIINFO_MAJOR_VERSION == 5 && DRIINFO_MINOR_VERSION >= 3)
      if (pI830->useEXA)
	 pDRIInfo->texOffsetStart = I830TexOffsetStart;
#endif

#if DRI_SUPPORTS_CLIP_NOTIFY
      pDRIInfo->ClipNotify = I830DRIClipNotify;
#endif

#if DRI_DRIVER_FRAMEBUFFER_MAP
   /* DRI version is high enough that we can get the DRI code to not
    * try to manage the framebuffer.
    */
   pDRIInfo->frameBufferPhysicalAddress = 0;
   pDRIInfo->frameBufferSize = 0;
   pDRIInfo->frameBufferStride = 0;
   pDRIInfo->dontMapFrameBuffer = TRUE;
#else
   /* Supply a dummy mapping info required by DRI setup.
    */
   pDRIInfo->frameBufferPhysicalAddress = (char *) pI830->LinearAddr;
   pDRIInfo->frameBufferSize = GTT_PAGE_SIZE;
   pDRIInfo->frameBufferStride = 1;
#endif

   pDRIInfo->TransitionTo2d = I830DRITransitionTo2d;
   pDRIInfo->TransitionTo3d = I830DRITransitionTo3d;

#if !DRI_SUPPORTS_CLIP_NOTIFY
   pDRIInfo->TransitionSingleToMulti3D = I830DRITransitionSingleToMulti3d;
   pDRIInfo->TransitionMultiToSingle3D = I830DRITransitionMultiToSingle3d;
#endif

   /* do driver-independent DRI screen initialization here */
   if (!DRIScreenInit(pScreen, pDRIInfo, &pI830->drmSubFD)) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[dri] DRIScreenInit failed. Disabling DRI.\n");
      xfree(pDRIInfo->devPrivate);
      pDRIInfo->devPrivate = NULL;
      DRIDestroyInfoRec(pI830->pDRIInfo);
      pI830->pDRIInfo = NULL;
      return FALSE;
   }

   /* Now, nuke dri.c's dummy frontbuffer map setup if we did that. */
   if (pDRIInfo->frameBufferSize != 0) {
       int tmp;
       drm_handle_t fb_handle;
       void *ptmp;

       /* With the compat method, it will continue to report
	* the wrong map out of GetDeviceInfo, which will break AIGLX.
	*/
       DRIGetDeviceInfo(pScreen, &fb_handle, &tmp, &tmp, &tmp, &tmp, &ptmp);
       drmRmMap(pI830->drmSubFD, fb_handle);

       xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		  "Removed DRI frontbuffer mapping in compatibility mode.\n");
       xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		  "DRIGetDeviceInfo will report incorrect frontbuffer "
		  "handle.\n");
   }

   /* Check the i915 DRM versioning */
   {
      drmVersionPtr version;

      /* Check the DRM lib version.
       * drmGetLibVersion was not supported in version 1.0, so check for
       * symbol first to avoid possible crash or hang.
       */
      if (xf86LoaderCheckSymbol("drmGetLibVersion")) {
	 version = drmGetLibVersion(pI830->drmSubFD);
      } else
      {
	 /* drmlib version 1.0.0 didn't have the drmGetLibVersion
	  * entry point.  Fake it by allocating a version record
	  * via drmGetVersion and changing it to version 1.0.0
	  */
	 version = drmGetVersion(pI830->drmSubFD);
	 version->version_major = 1;
	 version->version_minor = 0;
	 version->version_patchlevel = 0;
      }

#define REQ_MAJ 1
#define REQ_MIN 1
      if (version) {
	 if (version->version_major != REQ_MAJ ||
	     version->version_minor < REQ_MIN) {
	    /* incompatible drm library version */
	    xf86DrvMsg(pScreen->myNum, X_ERROR,
		       "[dri] I830DRIScreenInit failed because of a version mismatch.\n"
		       "[dri] libdrm.a module version is %d.%d.%d but version %d.%d.x is needed.\n"
		       "[dri] Disabling DRI.\n",
		       version->version_major,
		       version->version_minor, version->version_patchlevel,
		       REQ_MAJ, REQ_MIN);
	    drmFreeVersion(version);
	    I830DRICloseScreen(pScreen);
	    return FALSE;
	 }
	 drmFreeVersion(version);
      }

      /* Check the i915 DRM version */
      version = drmGetVersion(pI830->drmSubFD);
      if (version) {
	 if (version->version_major != 1 || version->version_minor < 3) {
	    /* incompatible drm version */
	    xf86DrvMsg(pScreen->myNum, X_ERROR,
		       "[dri] %s failed because of a version mismatch.\n"
		       "[dri] i915 kernel module version is %d.%d.%d but version 1.3 or greater is needed.\n"
		       "[dri] Disabling DRI.\n",
		       "I830DRIScreenInit",
		       version->version_major,
		       version->version_minor, version->version_patchlevel);
	    I830DRICloseScreen(pScreen);
	    drmFreeVersion(version);
	    return FALSE;
	 }
	 /* Check whether the kernel module attached to the device isn't the
	  * one we expected (meaning it's the old i830 module).
	  */
	 if (strncmp(version->name, pDRIInfo->drmDriverName,
		     strlen(pDRIInfo->drmDriverName)))
	 {
	    xf86DrvMsg(pScreen->myNum, X_WARNING,
		       "Detected i830 kernel module.  The i915 kernel module "
		       "is required for DRI.  Aborting.\n");
	    I830DRICloseScreen(pScreen);
	    drmFreeVersion(version);
	    return FALSE;
	 }
	 pI830->drmMinor = version->version_minor;
#ifdef DAMAGE
	 if (pI830->allowPageFlip && pI830->drmMinor < 9) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "DRM version 1.9 or newer required for Page flipping. "
		       "Disabling.\n");
	    pI830->allowPageFlip = FALSE;
	 }
#endif	 
	 drmFreeVersion(version);
      }
   }

   return TRUE;
}

static void
I830InitTextureHeap(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   drmI830MemInitHeap drmHeap;

   if (pI830->textures == NULL)
       return;

   /* Start up the simple memory manager for agp space */
   drmHeap.region = I830_MEM_REGION_AGP;
   drmHeap.start  = 0;
   drmHeap.size   = pI830->textures->size;

   if (drmCommandWrite(pI830->drmSubFD, DRM_I830_INIT_HEAP,
			  &drmHeap, sizeof(drmHeap))) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "[drm] Failed to initialized agp heap manager\n");
   } else {
      xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		    "[drm] Initialized kernel agp heap manager, %ld\n",
		    pI830->textures->size);

      I830SetParam(pScrn, I830_SETPARAM_TEX_LRU_LOG_GRANULARITY, 
		   pI830->TexGranularity);
   }
}

/**
 * Sets up mappings for static, lifetime-fixed allocations, and inital SAREA
 * setup.
 */
Bool
I830DRIDoMappings(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   DRIInfoPtr pDRIInfo = pI830->pDRIInfo;
   I830DRIPtr pI830DRI = pDRIInfo->devPrivate;
   drmI830Sarea *sarea = (drmI830Sarea *) DRIGetSAREAPrivate(pScreen);

   DPRINTF(PFX, "I830DRIDoMappings\n");

   if (drmAddMap(pI830->drmSubFD,
		 (drm_handle_t)pI830->LpRing->mem->offset + pI830->LinearAddr,
		 pI830->LpRing->mem->size, DRM_AGP, 0,
		 (drmAddress) &pI830->ring_map) < 0) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[drm] drmAddMap(ring_map) failed. Disabling DRI\n");
      DRICloseScreen(pScreen);
      return FALSE;
   }
   xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] ring buffer = 0x%08x\n",
	      (int)pI830->ring_map);

   if (!I830InitDma(pScrn)) {
      DRICloseScreen(pScreen);
      return FALSE;
   }

   /* init to zero to be safe */
   sarea->front_handle = 0;
   sarea->back_handle = 0;
   sarea->third_handle = 0;
   sarea->depth_handle = 0;
   sarea->tex_handle = 0;

   /* Assign pScreen */
   pScrn->pScreen = pScreen;

   /* Need to initialize pScreen now to let RandR know. */
   pScrn->pScreen->width = pScrn->virtualX;
   pScrn->pScreen->height = pScrn->virtualY;

   /* If we are using the kernel memory manager, we have to delay SAREA and
    * mapping setup until our buffers are pinned at EnterVT, losing the
    * opportunity to fail cleanly early on.
    */
   if (pI830->memory_manager == NULL) {
      if (!i830_update_dri_buffers(pScrn)) {
	 /* screen mappings probably failed */
	 xf86DrvMsg(pScreen->myNum, X_ERROR,
		    "[drm] drmAddMap(screen mappings) failed. "
		    "Disabling DRI\n");
	 DRICloseScreen(pScreen);
	 return FALSE;
      }
   }

   if (pI830->allocate_classic_textures)
      I830InitTextureHeap(pScrn);

   if (DEVICE_ID(pI830->PciInfo) != PCI_CHIP_845_G &&
       DEVICE_ID(pI830->PciInfo) != PCI_CHIP_I830_M) {
      I830SetParam(pScrn, I830_SETPARAM_USE_MI_BATCHBUFFER_START, 1 );
   }

   pI830DRI = (I830DRIPtr) pI830->pDRIInfo->devPrivate;
   pI830DRI->deviceID = DEVICE_ID(pI830->PciInfo);
   pI830DRI->width = pScrn->virtualX;
   pI830DRI->height = pScrn->virtualY;
   pI830DRI->mem = pScrn->videoRam * 1024;
   pI830DRI->cpp = pI830->cpp;

   pI830DRI->bitsPerPixel = pScrn->bitsPerPixel;

   pI830DRI->sarea_priv_offset = sizeof(XF86DRISAREARec);

   if (!(I830InitVisualConfigs(pScreen))) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[dri] I830InitVisualConfigs failed. Disabling DRI\n");
      DRICloseScreen(pScreen);
      return FALSE;
   }

   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] visual configs initialized\n");
   pI830->pDRIInfo->driverSwapMethod = DRI_HIDE_X_CONTEXT;

   return TRUE;
}

Bool
I830DRIInstIrqHandler(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   I830DRIPtr pI830DRI = (I830DRIPtr) pI830->pDRIInfo->devPrivate;

   pI830DRI->irq = drmGetInterruptFromBusID(pI830->drmSubFD,
#if XSERVER_LIBPCIACCESS
					       ((pI830->PciInfo->domain << 8) |
						pI830->PciInfo->bus),
					       pI830->PciInfo->dev,
					       pI830->PciInfo->func
#else
					       ((pciConfigPtr) pI830->
						PciInfo->thisCard)->busnum,
					       ((pciConfigPtr) pI830->
						PciInfo->thisCard)->devnum,
					       ((pciConfigPtr) pI830->
						PciInfo->thisCard)->funcnum
#endif
					       );

   if (drmCtlInstHandler(pI830->drmSubFD, pI830DRI->irq)) {
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	       "[drm] failure adding irq handler\n");
       pI830DRI->irq = 0;
       return FALSE;
   } else
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "[drm] dma control initialized, using IRQ %d\n",
	       pI830DRI->irq);

   return TRUE;
}

Bool
I830DRIResume(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];

   DPRINTF(PFX, "I830DRIResume\n");

   I830ResumeDma(pScrn);

   I830DRIInstIrqHandler(pScrn);

   return TRUE;
}

void
I830DRICloseScreen(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   I830DRIPtr pI830DRI = (I830DRIPtr) pI830->pDRIInfo->devPrivate;

   DPRINTF(PFX, "I830DRICloseScreen\n");

#ifdef DAMAGE
   REGION_UNINIT(pScreen, &pI830->driRegion);
#endif

   if (pI830DRI->irq) {
       drmCtlUninstHandler(pI830->drmSubFD);
       pI830DRI->irq = 0;
   }

   I830CleanupDma(pScrn);

   DRICloseScreen(pScreen);

   if (pI830->pDRIInfo) {
      if (pI830->pDRIInfo->devPrivate) {
	 xfree(pI830->pDRIInfo->devPrivate);
	 pI830->pDRIInfo->devPrivate = NULL;
      }
      DRIDestroyInfoRec(pI830->pDRIInfo);
      pI830->pDRIInfo = NULL;
   }
   if (pI830->pVisualConfigs)
      xfree(pI830->pVisualConfigs);
   if (pI830->pVisualConfigsPriv)
      xfree(pI830->pVisualConfigsPriv);
}

static Bool
I830CreateContext(ScreenPtr pScreen, VisualPtr visual,
		  drm_context_t hwContext, void *pVisualConfigPriv,
		  DRIContextType contextStore)
{
   return TRUE;
}

static void
I830DestroyContext(ScreenPtr pScreen, drm_context_t hwContext,
		   DRIContextType contextStore)
{
}

Bool
I830DRIFinishScreenInit(ScreenPtr pScreen)
{
   DPRINTF(PFX, "I830DRIFinishScreenInit\n");

   if (!DRIFinishScreenInit(pScreen))
      return FALSE;

   /* move irq initialize later in EnterVT, as then we
    * would finish binding possible hw status page, which
    * requires irq ctrl ioctl not be called that early.
    */
   return TRUE;
}

#ifdef DAMAGE
/* This should be done *before* XAA syncs,
 * Otherwise will have to sync again???
 */
static void
I830DRIDoRefreshArea (ScrnInfoPtr pScrn, int num, BoxPtr pbox, uint32_t dst)
{
   I830Ptr pI830 = I830PTR(pScrn);
   int i, cmd, br13 = (pScrn->displayWidth * pI830->cpp) | (0xcc << 16);

   if (pScrn->bitsPerPixel == 32) {
      cmd = (XY_SRC_COPY_BLT_CMD | XY_SRC_COPY_BLT_WRITE_ALPHA |
	     XY_SRC_COPY_BLT_WRITE_RGB);
      br13 |= 3 << 24;
   } else {
      cmd = (XY_SRC_COPY_BLT_CMD);
      br13 |= 1 << 24;
   }

   for (i = 0 ; i < num ; i++, pbox++) {
      BEGIN_BATCH(8);
      OUT_BATCH(cmd);
      OUT_BATCH(br13);
      OUT_BATCH((pbox->y1 << 16) | pbox->x1);
      OUT_BATCH((pbox->y2 << 16) | pbox->x2);
      OUT_BATCH(dst);
      OUT_BATCH((pbox->y1 << 16) | pbox->x1);
      OUT_BATCH(br13 & 0xffff);
      OUT_BATCH(pI830->front_buffer->offset);
      ADVANCE_BATCH();
   }
}

static void
I830DRIRefreshArea (ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
   I830Ptr pI830 = I830PTR(pScrn);
   drmI830Sarea *pSAREAPriv = DRIGetSAREAPrivate(pScrn->pScreen);

   /* Don't want to do this when no 3d is active and pages are
    * right-way-round :
    */
   if (!pSAREAPriv->pf_active && pSAREAPriv->pf_current_page == 0)
      return;

   I830DRIDoRefreshArea(pScrn, num, pbox, pI830->back_buffer->offset);

   if (pI830->third_buffer) {
      I830DRIDoRefreshArea(pScrn, num, pbox, pI830->third_buffer->offset);
   }

   DamageEmpty(pI830->pDamage);
}
#endif

static void
I830DRISwapContext(ScreenPtr pScreen, DRISyncType syncType,
		   DRIContextType oldContextType, void *oldContext,
		   DRIContextType newContextType, void *newContext)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);

   if (syncType == DRI_3D_SYNC &&
       oldContextType == DRI_2D_CONTEXT && newContextType == DRI_2D_CONTEXT) {

      if (I810_DEBUG & DEBUG_VERBOSE_DRI)
	 ErrorF("i830DRISwapContext (in)\n");

      *pI830->last_3d = LAST_3D_OTHER;

      if (!pScrn->vtSema)
     	 return;
      pI830->LockHeld = 1;
      i830_refresh_ring(pScrn);

      I830EmitFlush(pScrn);

#ifdef DAMAGE
      if (!pI830->pDamage && pI830->allowPageFlip) {
	 PixmapPtr pPix  = pScreen->GetScreenPixmap(pScreen);
	 pI830->pDamage = DamageCreate(NULL, NULL, DamageReportNone, TRUE,
				       pScreen, pPix);

	 if (pI830->pDamage == NULL) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "No screen damage record, page flipping disabled\n");
            pI830->allowPageFlip = FALSE;
	 } else {
	    DamageRegister(&pPix->drawable, pI830->pDamage);

	    DamageDamageRegion(&pPix->drawable,
			       &WindowTable[pScreen->myNum]->winSize);

            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "Damage tracking initialized for page flipping\n");
	 }
    }
#endif
   } else if (syncType == DRI_2D_SYNC &&
	      oldContextType == DRI_NO_CONTEXT &&
	      newContextType == DRI_2D_CONTEXT) {
#ifdef DAMAGE
      drmI830Sarea *sPriv = (drmI830Sarea *) DRIGetSAREAPrivate(pScreen);
#endif

      if (I810_DEBUG & DEBUG_VERBOSE_DRI)
	 ErrorF("i830DRISwapContext (out)\n");

      if (!pScrn->vtSema)
     	 return;

#ifdef DAMAGE
      if (pI830->pDamage) {
	 RegionPtr pDamageReg = DamageRegion(pI830->pDamage);

	 if (pDamageReg) {
	    RegionRec region;
	    int nrects;

	    REGION_NULL(pScreen, &region);
	    REGION_SUBTRACT(pScreen, &region, pDamageReg, &pI830->driRegion);

	    if ((nrects = REGION_NUM_RECTS(&region)))
	       I830DRIRefreshArea(pScrn, nrects, REGION_RECTS(&region));

	    REGION_UNINIT(pScreen, &region);
	 }
      }
#endif

      I830EmitFlush(pScrn);

#ifdef DAMAGE
      /* Try flipping back to the front page if necessary */
      if (sPriv && !sPriv->pf_enabled && sPriv->pf_current_page != 0) {
	 drm_i915_flip_t flip = { .pipes = 0 };

	 if (sPriv->pf_current_page & (0x3 << 2)) {
	    sPriv->pf_current_page = sPriv->pf_current_page & 0x3;
	    sPriv->pf_current_page |= (sPriv->third_handle ? 2 : 1) << 2;

	    flip.pipes |= 0x2;
	 }

	 if (sPriv->pf_current_page & 0x3) {
	    sPriv->pf_current_page = sPriv->pf_current_page & (0x3 << 2);
	    sPriv->pf_current_page |= sPriv->third_handle ? 2 : 1;

	    flip.pipes |= 0x1;
	 }

	 drmCommandWrite(pI830->drmSubFD, DRM_I915_FLIP, &flip, sizeof(flip));

	 if (sPriv->pf_current_page != 0)
	    xf86DrvMsg(pScreen->myNum, X_WARNING,
		       "[dri] %s: kernel failed to unflip buffers.\n", __func__);
      }
#endif

      pI830->LockHeld = 0;
   } else if (I810_DEBUG & DEBUG_VERBOSE_DRI)
      ErrorF("i830DRISwapContext (other)\n");
}

static void
I830DRIInitBuffers(WindowPtr pWin, RegionPtr prgn, CARD32 index)
{
   ScreenPtr pScreen = pWin->drawable.pScreen;
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   BoxPtr pbox = REGION_RECTS(prgn);
   int nbox = REGION_NUM_RECTS(prgn);

   if (I810_DEBUG & DEBUG_VERBOSE_DRI)
      ErrorF("I830DRIInitBuffers\n");

   I830SetupForSolidFill(pScrn, 0, GXcopy, -1);
   while (nbox--) {
      I830SelectBuffer(pScrn, I830_SELECT_BACK);
      I830SubsequentSolidFillRect(pScrn, pbox->x1, pbox->y1,
				  pbox->x2 - pbox->x1, pbox->y2 - pbox->y1);

      if (I830PTR(pScrn)->third_buffer) {
	 I830SelectBuffer(pScrn, I830_SELECT_THIRD);
	 I830SubsequentSolidFillRect(pScrn, pbox->x1, pbox->y1,
				     pbox->x2 - pbox->x1, pbox->y2 - pbox->y1);
      }

      pbox++;
   }

   /* Clear the depth buffer - uses 0xffff rather than 0.
    */
   pbox = REGION_RECTS(prgn);
   nbox = REGION_NUM_RECTS(prgn);

   I830SelectBuffer(pScrn, I830_SELECT_DEPTH);

   switch (pScrn->bitsPerPixel) {
   case 16:
      I830SetupForSolidFill(pScrn, 0xffff, GXcopy, -1);
      break;
   case 32:
      I830SetupForSolidFill(pScrn, 0xffffff, GXcopy, -1);
      break;
   }

   while (nbox--) {
      I830SubsequentSolidFillRect(pScrn, pbox->x1, pbox->y1,
				  pbox->x2 - pbox->x1, pbox->y2 - pbox->y1);
      pbox++;
   }

   I830SelectBuffer(pScrn, I830_SELECT_FRONT);
   i830MarkSync(pScrn);
}

/* This routine is a modified form of XAADoBitBlt with the calls to
 * ScreenToScreenBitBlt built in. My routine has the prgnSrc as source
 * instead of destination. My origin is upside down so the ydir cases
 * are reversed.
 */
static void
I830DRIMoveBuffers(WindowPtr pParent, DDXPointRec ptOldOrg,
		   RegionPtr prgnSrc, CARD32 index)
{
   ScreenPtr pScreen = pParent->drawable.pScreen;
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   BoxPtr pboxTmp, pboxNext, pboxBase;
   DDXPointPtr pptTmp, pptNew2 = NULL;
   int xdir, ydir;

#if 0
   int screenwidth = pScrn->virtualX;
   int screenheight = pScrn->virtualY;
#else
   int screenwidth = pScreen->width;
   int screenheight = pScreen->height;
#endif

   BoxPtr pbox = REGION_RECTS(prgnSrc);
   int nbox = REGION_NUM_RECTS(prgnSrc);

   BoxPtr pboxNew1 = NULL;
   BoxPtr pboxNew2 = NULL;
   DDXPointPtr pptNew1 = NULL;
   DDXPointPtr pptSrc = &ptOldOrg;

   int dx = pParent->drawable.x - ptOldOrg.x;
   int dy = pParent->drawable.y - ptOldOrg.y;

   /* If the copy will overlap in Y, reverse the order */
   if (dy > 0) {
      ydir = -1;

      if (nbox > 1) {
	 /* Keep ordering in each band, reverse order of bands */
	 pboxNew1 = (BoxPtr) xalloc(sizeof(BoxRec) * nbox);
	 if (!pboxNew1)
	    return;
	 pptNew1 = (DDXPointPtr) xalloc(sizeof(DDXPointRec) * nbox);
	 if (!pptNew1) {
	    xfree(pboxNew1);
	    return;
	 }
	 pboxBase = pboxNext = pbox + nbox - 1;
	 while (pboxBase >= pbox) {
	    while ((pboxNext >= pbox) && (pboxBase->y1 == pboxNext->y1))
	       pboxNext--;
	    pboxTmp = pboxNext + 1;
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
   if (dx > 0) {
      xdir = -1;

      if (nbox > 1) {
	 /*reverse orderof rects in each band */
	 pboxNew2 = (BoxPtr) xalloc(sizeof(BoxRec) * nbox);
	 pptNew2 = (DDXPointPtr) xalloc(sizeof(DDXPointRec) * nbox);
	 if (!pboxNew2 || !pptNew2) {
	    if (pptNew2)
	       xfree(pptNew2);
	    if (pboxNew2)
	       xfree(pboxNew2);
	    if (pboxNew1) {
	       xfree(pptNew1);
	       xfree(pboxNew1);
	    }
	    return;
	 }
	 pboxBase = pboxNext = pbox;
	 while (pboxBase < pbox + nbox) {
	    while ((pboxNext < pbox + nbox) && (pboxNext->y1 == pboxBase->y1))
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

   /* SelectBuffer isn't really a good concept for the i810.
    */
   I830EmitFlush(pScrn);
   I830SetupForScreenToScreenCopy(pScrn, xdir, ydir, GXcopy, -1, -1);
   for (; nbox--; pbox++) {

      int x1 = pbox->x1;
      int y1 = pbox->y1;
      int destx = x1 + dx;
      int desty = y1 + dy;
      int w = pbox->x2 - x1 + 1;
      int h = pbox->y2 - y1 + 1;

      if (destx < 0)
	 x1 -= destx, w += destx, destx = 0;
      if (desty < 0)
	 y1 -= desty, h += desty, desty = 0;
      if (destx + w > screenwidth)
	 w = screenwidth - destx;
      if (desty + h > screenheight)
	 h = screenheight - desty;
      if (w <= 0)
	 continue;
      if (h <= 0)
	 continue;

      if (I810_DEBUG & DEBUG_VERBOSE_DRI)
	 ErrorF("MoveBuffers %d,%d %dx%d dx: %d dy: %d\n",
		x1, y1, w, h, dx, dy);

      I830SelectBuffer(pScrn, I830_SELECT_BACK);
      I830SubsequentScreenToScreenCopy(pScrn, x1, y1, destx, desty, w, h);
      if (pI830->third_buffer) {
	 I830SelectBuffer(pScrn, I830_SELECT_THIRD);
	 I830SubsequentScreenToScreenCopy(pScrn, x1, y1, destx, desty, w, h);
      }
      if (!IS_I965G(pI830)) {
         I830SelectBuffer(pScrn, I830_SELECT_DEPTH);
         I830SubsequentScreenToScreenCopy(pScrn, x1, y1, destx, desty, w, h);
      }
   }
   I830SelectBuffer(pScrn, I830_SELECT_FRONT);
   I830EmitFlush(pScrn);

   if (pboxNew2) {
      xfree(pptNew2);
      xfree(pboxNew2);
   }
   if (pboxNew1) {
      xfree(pptNew1);
      xfree(pboxNew1);
   }
   i830MarkSync(pScrn);
}

/* Use callbacks from dri.c to support pageflipping mode for a single
 * 3d context without need for any specific full-screen extension.
 *
 * Also see tdfx driver for example of using these callbacks to
 * allocate and free 3d-specific memory on demand.
 */

/* Use the miext/shadow module to maintain a list of dirty rectangles.
 * These are blitted to the back buffer to keep both buffers clean
 * during page-flipping when the 3d application isn't fullscreen.
 *
 * Unlike most use of the shadow code, both buffers are in video
 * memory.
 *
 * An alternative to this would be to organize for all on-screen
 * drawing operations to be duplicated for the two buffers.  That
 * might be faster, but seems like a lot more work...
 */

static void
I830DRISetPfMask(ScreenPtr pScreen, int pfMask)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   drmI830Sarea *pSAREAPriv = DRIGetSAREAPrivate(pScreen);

   if (pI830->allowPageFlip && pfMask) {
      pSAREAPriv->pf_enabled = pI830->allowPageFlip;
      pSAREAPriv->pf_active = pfMask;
   } else
      pSAREAPriv->pf_active = 0;
}

#if !DRI_SUPPORTS_CLIP_NOTIFY
static void
I830DRITransitionSingleToMulti3d(ScreenPtr pScreen)
{
   /* Tell the clients not to pageflip.  How?
    *   -- Field in sarea, plus bumping the window counters.
    *   -- DRM needs to cope with Front-to-Back swapbuffers.
    */
   I830DRISetPfMask(pScreen, 0);
}

static void
I830DRITransitionMultiToSingle3d(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);

   /* Let the remaining 3d app start page flipping again.
    */
   I830DRISetPfMask(pScreen, pI830->allowPageFlip ? 0x3 : 0);
}
#endif /* !DRI_SUPPORTS_CLIP_NOTIFY */

static void
I830DRITransitionTo3d(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);

   I830DRISetPfMask(pScreen, pI830->allowPageFlip ? 0x3 : 0);

   pI830->want_vblank_interrupts = TRUE;
   I830DRISetVBlankInterrupt(pScrn, TRUE);
}

static void
I830DRITransitionTo2d(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   drmI830Sarea *sPriv = (drmI830Sarea *) DRIGetSAREAPrivate(pScreen);

   I830DRISetPfMask(pScreen, 0);

   sPriv->pf_enabled = 0;

   pI830->want_vblank_interrupts = FALSE;
   I830DRISetVBlankInterrupt(pScrn, FALSE);
}

#if DRI_SUPPORTS_CLIP_NOTIFY
static void
I830DRIClipNotify(ScreenPtr pScreen, WindowPtr *ppWin, int num)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   unsigned pfMask = 0;

   REGION_UNINIT(pScreen, &pI830->driRegion);
   REGION_NULL(pScreen, &pI830->driRegion);

   if (num > 0) {
      drmI830Sarea *sPriv = (drmI830Sarea *) DRIGetSAREAPrivate(pScreen);
      BoxRec crtcBox[2];
      unsigned numvisible[2] = { 0, 0 };
      int i, j;

      crtcBox[0].x1 = sPriv->planeA_x;
      crtcBox[0].y1 = sPriv->planeA_y;
      crtcBox[0].x2 = crtcBox[0].x1 + sPriv->planeA_w;
      crtcBox[0].y2 = crtcBox[0].y1 + sPriv->planeA_h;
      crtcBox[1].x1 = sPriv->planeB_x;
      crtcBox[1].y1 = sPriv->planeB_y;
      crtcBox[1].x2 = crtcBox[1].x1 + sPriv->planeB_w;
      crtcBox[1].y2 = crtcBox[1].y1 + sPriv->planeB_h;

      for (i = 0; i < 2; i++) {
	 for (j = 0; j < num; j++) {
	    WindowPtr pWin = ppWin[j];

	    if (pWin) {
	       if (RECT_IN_REGION(pScreen, &pWin->clipList, &crtcBox[i]) !=
		   rgnOUT)
		  numvisible[i]++;

	       if (i == 0)
		  REGION_UNION(pScreen, &pI830->driRegion, &pWin->clipList,
			       &pI830->driRegion);
	    }
	 }

	 if (numvisible[i] == 1)
	    pfMask |= 1 << i;
      }
   } else
      REGION_NULL(pScreen, &pI830->driRegion);

   I830DRISetPfMask(pScreen, pfMask);
}
#endif /* DRI_SUPPORTS_CLIP_NOTIFY */

/**
 * Update the SAREA fields with current buffer information.
 *
 * Most of the SAREA fields are already updated by i830_do_addmap().
 *
 * This does include other SAREA initialization which will actually be constant
 * over the lifetime of the server.
 */
static void
i830_update_sarea(ScrnInfoPtr pScrn, drmI830Sarea *sarea)
{
   ScreenPtr pScreen = pScrn->pScreen;
   I830Ptr pI830 = I830PTR(pScrn);

   sarea->width = pScreen->width;
   sarea->height = pScreen->height;
   sarea->pitch = pScrn->displayWidth;
   sarea->virtualX = pScrn->virtualX;
   sarea->virtualY = pScrn->virtualY;

   sarea->front_tiled = (pI830->front_buffer->tiling != TILE_NONE);
   sarea->back_tiled = (pI830->back_buffer->tiling != TILE_NONE);
   if (pI830->third_buffer != NULL)
       sarea->third_tiled = (pI830->third_buffer->tiling != TILE_NONE);
   else
       sarea->third_tiled = FALSE;
   sarea->depth_tiled = (pI830->depth_buffer->tiling != TILE_NONE);
   sarea->rotated_tiled = FALSE;

   sarea->log_tex_granularity = pI830->TexGranularity;

   sarea->front_bo_handle = -1;
   sarea->back_bo_handle = -1;
   sarea->third_bo_handle = -1;
   sarea->depth_bo_handle = -1;
#ifdef XF86DRI_MM
   if (pI830->front_buffer->bo.size)
       sarea->front_bo_handle = pI830->front_buffer->bo.handle;
   if (pI830->back_buffer->bo.size)
       sarea->back_bo_handle = pI830->back_buffer->bo.handle;
   if (pI830->third_buffer != NULL && pI830->third_buffer->bo.size)
       sarea->third_bo_handle = pI830->third_buffer->bo.handle;
   if (pI830->depth_buffer->bo.size)
       sarea->depth_bo_handle = pI830->depth_buffer->bo.handle;
#endif

   /* The rotation is now handled entirely by the X Server, so just leave the
    * DRI unaware.
    */
   sarea->rotation = 0;
   sarea->rotated_offset = -1;
   sarea->rotated_size = 0;
   sarea->rotated_pitch = pScrn->displayWidth;
}

/**
 * Updates the DRI mapping for the given i830_memory struct, with the given
 * flags.
 */
static int
i830_do_addmap(ScrnInfoPtr pScrn, i830_memory *mem,
	       drm_handle_t *sarea_handle, int *sarea_size, int *sarea_offset)
{
    I830Ptr pI830 = I830PTR(pScrn);
    int size = mem->size;

    if (mem == pI830->front_buffer) {
	/* Workaround for XAA pixmap cache: Don't use front_buffer->size
	 * and instead, calculate the visible frontbuffer size and round to
	 * avoid irritating the assertions of older DRI drivers.
	 */
	size = ROUND_TO_PAGE(pScrn->displayWidth * pScrn->virtualY *
			     pI830->cpp);
    }

    if (*sarea_handle != 0 &&
	(*sarea_size != size || *sarea_offset != mem->offset))
    {
	drmRmMap(pI830->drmSubFD, *sarea_handle);
	*sarea_handle = 0;
	*sarea_size = 0;
	*sarea_offset = 0;
    }

    if (*sarea_handle == 0) {
	int ret;

	ret = drmAddMap(pI830->drmSubFD,
			(drm_handle_t)(mem->offset + pI830->LinearAddr),
			size, DRM_AGP, 0,
			(drmAddress) sarea_handle);
	if (ret == 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "[drm] mapped %s at 0x%08lx, handle = 0x%08x\n",
		       mem->name, mem->offset + pI830->LinearAddr,
		       (int)*sarea_handle);
	    *sarea_size = size;
	    *sarea_offset = mem->offset;
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "[drm] failed to map %s at 0x%08lx\n",
		       mem->name, mem->offset + pI830->LinearAddr);
	    return FALSE;
	}
    }

    return TRUE;
}

/**
 * Updates the DRM mappings with the current buffer information.
 *
 * Some old DRI drivers may be unprepared for buffers actually moving at
 * runtime, which would likely result in bus errors on software fallbacks or
 * hangs or misrendering on hardware rendering.
 */
static Bool
i830_update_dri_mappings(ScrnInfoPtr pScrn, drmI830Sarea *sarea)
{
   I830Ptr pI830 = I830PTR(pScrn);

   if (!i830_do_addmap(pScrn, pI830->front_buffer, &sarea->front_handle,
		       &sarea->front_size, &sarea->front_offset)) {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Disabling DRI.\n");
       return FALSE;
   }

   if (!i830_do_addmap(pScrn, pI830->back_buffer, &sarea->back_handle,
		       &sarea->back_size, &sarea->back_offset)) {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Disabling DRI.\n");
       return FALSE;
   }

   if (pI830->third_buffer) {
       if (!i830_do_addmap(pScrn, pI830->third_buffer, &sarea->third_handle,
			   &sarea->third_size, &sarea->third_offset)) {
	   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Disabling DRI.\n");
	   return FALSE;
       }
   } else {
       sarea->third_handle = 0;
       sarea->third_offset = 0;
       sarea->third_size = 0;
   }

   if (!i830_do_addmap(pScrn, pI830->depth_buffer, &sarea->depth_handle,
		       &sarea->depth_size, &sarea->depth_offset)) {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Disabling DRI.\n");
       return FALSE;
   }

   if (pI830->allocate_classic_textures) {
       if (!i830_do_addmap(pScrn, pI830->textures, &sarea->tex_handle,
			   &sarea->tex_size, &sarea->tex_offset)) {
	   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Disabling DRI.\n");
	   return FALSE;
       }
   }

   return TRUE;
}

static void
i830_update_screen_private(ScrnInfoPtr pScrn, drmI830Sarea *sarea)
{
   I830Ptr pI830 = I830PTR(pScrn);

   pI830->pDRIInfo->frameBufferPhysicalAddress = (char *) pI830->LinearAddr;
   pI830->pDRIInfo->frameBufferStride = pScrn->displayWidth * pI830->cpp;
   pI830->pDRIInfo->frameBufferSize = sarea->front_size;
#if DRI_DRIVER_FRAMEBUFFER_MAP
   pI830->pDRIInfo->hFrameBuffer = sarea->front_handle;
#endif
}

/**
 * Update the SAREA fields, DRI mappings, and screen info passed through the
 * protocol.
 *
 * This gets called both at startup and after any of the buffers might have
 * been relocated.
 */
Bool
i830_update_dri_buffers(ScrnInfoPtr pScrn)
{
   ScreenPtr pScreen = pScrn->pScreen;
   drmI830Sarea *sarea = (drmI830Sarea *) DRIGetSAREAPrivate(pScreen);
   Bool success;

   success = i830_update_dri_mappings(pScrn, sarea);
   if (!success)
       return FALSE;
   i830_update_sarea(pScrn, sarea);
   i830_update_screen_private(pScrn, sarea);

   return TRUE;
}

Bool
I830DRISetVBlankInterrupt (ScrnInfoPtr pScrn, Bool on)
{
    I830Ptr pI830 = I830PTR(pScrn);
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    drmI830VBlankPipe pipe;

    /* If we have no 3d running, then don't bother enabling the vblank
     * interrupt.
     */
    if (!pI830->want_vblank_interrupts)
	on = FALSE;

    if (pI830->directRenderingEnabled && pI830->drmMinor >= 5) {
	if (on) {
	    if (xf86_config->num_crtc > 1 && xf86_config->crtc[1]->enabled)
		if (pI830->drmMinor >= 6)
		    pipe.pipe = DRM_I830_VBLANK_PIPE_A | DRM_I830_VBLANK_PIPE_B;
		else
		    pipe.pipe = DRM_I830_VBLANK_PIPE_B;
	    else
		pipe.pipe = DRM_I830_VBLANK_PIPE_A;
	} else {
	    pipe.pipe = 0;
	}
	if (drmCommandWrite(pI830->drmSubFD, DRM_I830_SET_VBLANK_PIPE,
			    &pipe, sizeof (pipe))) {
	    return FALSE;
	}
    }

    return TRUE;
}

Bool
I830DRILock(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

   if (pI830->directRenderingEnabled && !pI830->LockHeld) {
      DRILock(screenInfo.screens[pScrn->scrnIndex], 0);
      pI830->LockHeld = 1;
      i830_refresh_ring(pScrn);
      return TRUE;
   }
   else
      return FALSE;
}



void
I830DRIUnlock(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

   if (pI830->directRenderingEnabled && pI830->LockHeld) {
      DRIUnlock(screenInfo.screens[pScrn->scrnIndex]);
      pI830->LockHeld = 0;
   }
}
