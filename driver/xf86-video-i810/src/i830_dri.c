/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/i810/i830_dri.c,v 1.15 2003/06/18 13:14:17 dawes Exp $ */
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

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86_ansic.h"
#include "xf86Priv.h"

#include "xf86PciInfo.h"
#include "xf86Pci.h"

#include "windowstr.h"
#include "shadow.h"

#include "GL/glxtokens.h"

#include "i830.h"
#include "i830_dri.h"

static char I830KernelDriverName[] = "i915";
static char I830ClientDriverName[] = "i915";

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
static void I830DRITransitionMultiToSingle3d(ScreenPtr pScreen);
static void I830DRITransitionSingleToMulti3d(ScreenPtr pScreen);

static void I830DRIShadowUpdate (ScreenPtr pScreen, shadowBufPtr pBuf);

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

   info.ring_start = ring->mem.Start + pI830->LinearAddr;
   info.ring_end = ring->mem.End + pI830->LinearAddr;
   info.ring_size = ring->mem.Size;

   info.mmio_offset = (unsigned int)pI830DRI->regs;

   info.sarea_priv_offset = sizeof(XF86DRISAREARec);

   info.front_offset = pI830->FrontBuffer.Start;
   info.back_offset = pI830->BackBuffer.Start;
   info.depth_offset = pI830->DepthBuffer.Start;
   info.w = pScrn->virtualX;
   info.h = pScrn->virtualY;
   info.pitch = pI830->backPitch;
   info.back_pitch = pI830->backPitch;
   info.depth_pitch = pI830->backPitch;
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


static Bool
I830InitVisualConfigs(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   int numConfigs = 0;
   __GLXvisualConfig *pConfigs = 0;
   I830ConfigPrivPtr pI830Configs = 0;
   I830ConfigPrivPtr *pI830ConfigPtrs = 0;
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
	 for (depth = 0; depth <= 1; depth++) {	/* and stencil */
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
I830DRIScreenInit(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   DRIInfoPtr pDRIInfo;
   I830DRIPtr pI830DRI;

   DPRINTF(PFX, "I830DRIScreenInit\n");
   /* Hardware 3D rendering only implemented for 16bpp and 32 bpp */
   if (((pScrn->bitsPerPixel / 8) != 2 && pScrn->depth != 16) &&
       (pScrn->bitsPerPixel / 8) != 4) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[drm] Direct rendering only supported in 16 and 32 bpp modes\n");
      return FALSE;
   }

   /* Check that the GLX, DRI, and DRM modules have been loaded by testing
    * for known symbols in each module. */
   if (!xf86LoaderCheckSymbol("GlxSetVisualConfigs"))
      return FALSE;
   if (!xf86LoaderCheckSymbol("drmAvailable"))
      return FALSE;
   if (!xf86LoaderCheckSymbol("DRIQueryVersion")) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[dri] %s failed (libdri.a too old)\n", "I830DRIScreenInit");
      return FALSE;
   }

   /* Check the DRI version */
   {
      int major, minor, patch;

      DRIQueryVersion(&major, &minor, &patch);
      if (major != DRIINFO_MAJOR_VERSION || minor < DRIINFO_MINOR_VERSION) {
	 xf86DrvMsg(pScreen->myNum, X_ERROR,
		    "[dri] %s failed because of a version mismatch.\n"
		    "[dri] libdri version is %d.%d.%d bug version %d.%d.x is needed.\n"
		    "[dri] Disabling DRI.\n",
		    "I830DRIScreenInit", major, minor, patch,
                    DRIINFO_MAJOR_VERSION, DRIINFO_MINOR_VERSION);
	 return FALSE;
      }
   }

   pDRIInfo = DRICreateInfoRec();
   if (!pDRIInfo) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[dri] DRICreateInfoRec failed. Disabling DRI.\n");
      return FALSE;
   }

   pI830->pDRIInfo = pDRIInfo;
   pI830->LockHeld = 0;

   pDRIInfo->drmDriverName = I830KernelDriverName;
   pDRIInfo->clientDriverName = I830ClientDriverName;
   if (xf86LoaderCheckSymbol("DRICreatePCIBusID")) {
      pDRIInfo->busIdString = DRICreatePCIBusID(pI830->PciInfo);
   } else {
      pDRIInfo->busIdString = xalloc(64);
      sprintf(pDRIInfo->busIdString, "PCI:%d:%d:%d",
	      ((pciConfigPtr) pI830->PciInfo->thisCard)->busnum,
	      ((pciConfigPtr) pI830->PciInfo->thisCard)->devnum,
	      ((pciConfigPtr) pI830->PciInfo->thisCard)->funcnum);
   }
   pDRIInfo->ddxDriverMajorVersion = I830_MAJOR_VERSION;
   pDRIInfo->ddxDriverMinorVersion = I830_MINOR_VERSION;
   pDRIInfo->ddxDriverPatchVersion = I830_PATCHLEVEL;
   pDRIInfo->frameBufferPhysicalAddress = pI830->LinearAddr +
					  pI830->FrontBuffer.Start;
   pDRIInfo->frameBufferSize = ROUND_TO_PAGE(pScrn->displayWidth *
					     pScrn->virtualY * pI830->cpp);
   pDRIInfo->frameBufferStride = pScrn->displayWidth * pI830->cpp;
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
      pI830->pDRIInfo = 0;
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
   pDRIInfo->TransitionTo2d = I830DRITransitionTo2d;
   pDRIInfo->TransitionTo3d = I830DRITransitionTo3d;
   pDRIInfo->TransitionSingleToMulti3D = I830DRITransitionSingleToMulti3d;
   pDRIInfo->TransitionMultiToSingle3D = I830DRITransitionMultiToSingle3d;

   if (!DRIScreenInit(pScreen, pDRIInfo, &pI830->drmSubFD)) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[dri] DRIScreenInit failed. Disabling DRI.\n");
      xfree(pDRIInfo->devPrivate);
      pDRIInfo->devPrivate = 0;
      DRIDestroyInfoRec(pI830->pDRIInfo);
      pI830->pDRIInfo = 0;
      return FALSE;
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
	 if (version->version_major != 1 || version->version_minor < 1) {
	    /* incompatible drm version */
	    xf86DrvMsg(pScreen->myNum, X_ERROR,
		       "[dri] %s failed because of a version mismatch.\n"
		       "[dri] i915 kernel module version is %d.%d.%d but version 1.1 or greater is needed.\n"
		       "[dri] Disabling DRI.\n",
		       "I830DRIScreenInit",
		       version->version_major,
		       version->version_minor, version->version_patchlevel);
	    I830DRICloseScreen(pScreen);
	    drmFreeVersion(version);
	    return FALSE;
	 }
	 if (strncmp(version->name, I830KernelDriverName, strlen(I830KernelDriverName))) {
	    xf86DrvMsg(pScreen->myNum, X_WARNING, 
			"i830 Kernel module detected, Use the i915 Kernel module instead, aborting DRI init.\n");
	    I830DRICloseScreen(pScreen);
	    drmFreeVersion(version);
	    return FALSE;
	 }
	 pI830->drmMinor = version->version_minor;
	 drmFreeVersion(version);
      }
   }

   return TRUE;
}

Bool
I830DRIDoMappings(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   DRIInfoPtr pDRIInfo = pI830->pDRIInfo;
   I830DRIPtr pI830DRI = pDRIInfo->devPrivate;

   DPRINTF(PFX, "I830DRIDoMappings\n");
   pI830DRI->regsSize = I830_REG_SIZE;
   if (drmAddMap(pI830->drmSubFD, (drm_handle_t)pI830->MMIOAddr,
		 pI830DRI->regsSize, DRM_REGISTERS, 0, &pI830DRI->regs) < 0) {
      xf86DrvMsg(pScreen->myNum, X_ERROR, "[drm] drmAddMap(regs) failed\n");
      DRICloseScreen(pScreen);
      return FALSE;
   }
   xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] Registers = 0x%08x\n",
	      pI830DRI->regs);

   /*
    * The tile setup is now initiated from I830BIOSScreenInit().
    */

   pI830->backPitch = pScrn->displayWidth;

   pI830DRI->backbufferSize = pI830->BackBuffer.Size;

   if (drmAddMap(pI830->drmSubFD,
		 (drm_handle_t)pI830->BackBuffer.Start + pI830->LinearAddr,
		 pI830->BackBuffer.Size, DRM_AGP, 0,
		 &pI830DRI->backbuffer) < 0) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[drm] drmAddMap(backbuffer) failed. Disabling DRI\n");
      DRICloseScreen(pScreen);
      return FALSE;
   }
   xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] Back Buffer = 0x%08x\n",
	      pI830DRI->backbuffer);

   pI830DRI->depthbufferSize = pI830->DepthBuffer.Size;
   if (drmAddMap(pI830->drmSubFD,
		 (drm_handle_t)pI830->DepthBuffer.Start + pI830->LinearAddr,
		 pI830->DepthBuffer.Size, DRM_AGP, 0,
		 &pI830DRI->depthbuffer) < 0) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[drm] drmAddMap(depthbuffer) failed. Disabling DRI\n");
      DRICloseScreen(pScreen);
      return FALSE;
   }
   xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] Depth Buffer = 0x%08x\n",
	      pI830DRI->depthbuffer);


   if (drmAddMap(pI830->drmSubFD,
		 (drm_handle_t)pI830->LpRing->mem.Start + pI830->LinearAddr,
		 pI830->LpRing->mem.Size, DRM_AGP, 0,
		 &pI830->ring_map) < 0) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[drm] drmAddMap(ring_map) failed. Disabling DRI\n");
      DRICloseScreen(pScreen);
      return FALSE;
   }
   xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] ring buffer = 0x%08x\n",
	      pI830->ring_map);

   pI830DRI->textureSize = pI830->TexMem.Size;
   pI830DRI->logTextureGranularity = pI830->TexGranularity;

   if (drmAddMap(pI830->drmSubFD,
		 (drm_handle_t)pI830->TexMem.Start + pI830->LinearAddr,
		 pI830->TexMem.Size, DRM_AGP, 0,
		 &pI830DRI->textures) < 0) {
      xf86DrvMsg(pScreen->myNum, X_ERROR,
		 "[drm] drmAddMap(textures) failed. Disabling DRI\n");
      DRICloseScreen(pScreen);
      return FALSE;
   }
   xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] textures = 0x%08x\n",
	      pI830DRI->textures);

   if (!I830InitDma(pScrn)) {
      DRICloseScreen(pScreen);
      return FALSE;
   }

   if (pI830->PciInfo->chipType != PCI_CHIP_845_G &&
       pI830->PciInfo->chipType != PCI_CHIP_I830_M) {
      I830SetParam(pScrn, I830_SETPARAM_USE_MI_BATCHBUFFER_START, 1 );
   }

   /* Okay now initialize the dma engine */
   {
      pI830DRI->irq = drmGetInterruptFromBusID(pI830->drmSubFD,
					       ((pciConfigPtr) pI830->
						PciInfo->thisCard)->busnum,
					       ((pciConfigPtr) pI830->
						PciInfo->thisCard)->devnum,
					       ((pciConfigPtr) pI830->
						PciInfo->thisCard)->funcnum);

      if (drmCtlInstHandler(pI830->drmSubFD, pI830DRI->irq)) {
	 xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "[drm] failure adding irq handler\n");
	 pI830DRI->irq = 0;
	 DRICloseScreen(pScreen);
	 return FALSE;
      }
      else
	 xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		    "[drm] dma control initialized, using IRQ %d\n",
		    pI830DRI->irq);
   }

   /* Start up the simple memory manager for agp space */
   {
      drmI830MemInitHeap drmHeap;
      drmHeap.region = I830_MEM_REGION_AGP;
      drmHeap.start  = 0;
      drmHeap.size   = pI830DRI->textureSize;
      
      if (drmCommandWrite(pI830->drmSubFD, DRM_I830_INIT_HEAP,
			  &drmHeap, sizeof(drmHeap))) {
	 xf86DrvMsg(pScreen->myNum, X_ERROR,
		    "[drm] Failed to initialized agp heap manager\n");
      } else {
	 xf86DrvMsg(pScreen->myNum, X_INFO,
		    "[drm] Initialized kernel agp heap manager, %d\n",
		    pI830DRI->textureSize);

	 I830SetParam(pScrn, I830_SETPARAM_TEX_LRU_LOG_GRANULARITY, 
		      pI830->TexGranularity);
      }
   }



   pI830DRI = (I830DRIPtr) pI830->pDRIInfo->devPrivate;
   pI830DRI->deviceID = pI830->PciInfo->chipType;
   pI830DRI->width = pScrn->virtualX;
   pI830DRI->height = pScrn->virtualY;
   pI830DRI->mem = pScrn->videoRam * 1024;
   pI830DRI->cpp = pI830->cpp;

   pI830DRI->fbOffset = pI830->FrontBuffer.Start;
   pI830DRI->fbStride = pI830->backPitch;

   pI830DRI->bitsPerPixel = pScrn->bitsPerPixel;

   pI830DRI->textureOffset = pI830->TexMem.Start;

   pI830DRI->backOffset = pI830->BackBuffer.Start;
   pI830DRI->depthOffset = pI830->DepthBuffer.Start;

   pI830DRI->backPitch = pI830->backPitch;
   pI830DRI->depthPitch = pI830->backPitch;
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
I830DRIResume(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   I830DRIPtr pI830DRI = (I830DRIPtr) pI830->pDRIInfo->devPrivate;

   DPRINTF(PFX, "I830DRIResume\n");

   I830ResumeDma(pScrn);

   {
      pI830DRI->irq = drmGetInterruptFromBusID(pI830->drmSubFD,
					       ((pciConfigPtr) pI830->
						PciInfo->thisCard)->busnum,
					       ((pciConfigPtr) pI830->
						PciInfo->thisCard)->devnum,
					       ((pciConfigPtr) pI830->
						PciInfo->thisCard)->funcnum);

      if (drmCtlInstHandler(pI830->drmSubFD, pI830DRI->irq)) {
	 xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "[drm] failure adding irq handler\n");
	 pI830DRI->irq = 0;
	 return FALSE;
      }
      else
	 xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		    "[drm] dma control initialized, using IRQ %d\n",
		    pI830DRI->irq);
   }

   return FALSE;
}

void
I830DRICloseScreen(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   I830DRIPtr pI830DRI = (I830DRIPtr) pI830->pDRIInfo->devPrivate;

   DPRINTF(PFX, "I830DRICloseScreen\n");

   if (pI830DRI->irq) {
       drmCtlUninstHandler(pI830->drmSubFD);
       pI830DRI->irq = 0;
   }

   I830CleanupDma(pScrn);

   DRICloseScreen(pScreen);

   if (pI830->pDRIInfo) {
      if (pI830->pDRIInfo->devPrivate) {
	 xfree(pI830->pDRIInfo->devPrivate);
	 pI830->pDRIInfo->devPrivate = 0;
      }
      DRIDestroyInfoRec(pI830->pDRIInfo);
      pI830->pDRIInfo = 0;
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
   drmI830Sarea *sPriv = (drmI830Sarea *) DRIGetSAREAPrivate(pScreen);
   ScrnInfoPtr        pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);

   DPRINTF(PFX, "I830DRIFinishScreenInit\n");

   memset(sPriv, 0, sizeof(sPriv));

   /* Have shadow run only while there is 3d active.
    */
   if (pI830->allowPageFlip && pI830->drmMinor >= 1) {
      shadowSetup(pScreen);
      shadowAdd(pScreen, 0, I830DRIShadowUpdate, 0, 0, 0);
   }
   else
      pI830->allowPageFlip = 0;

   return DRIFinishScreenInit(pScreen);
}

void
I830DRISwapContext(ScreenPtr pScreen, DRISyncType syncType,
		   DRIContextType oldContextType, void *oldContext,
		   DRIContextType newContextType, void *newContext)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);

   if (syncType == DRI_3D_SYNC &&
       oldContextType == DRI_2D_CONTEXT && newContextType == DRI_2D_CONTEXT) {
      ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];

      if (I810_DEBUG & DEBUG_VERBOSE_DRI)
	 ErrorF("i830DRISwapContext (in)\n");

      if (!pScrn->vtSema)
     	 return;
      pI830->LockHeld = 1;
      I830RefreshRing(pScrn);
   } else if (syncType == DRI_2D_SYNC &&
	      oldContextType == DRI_NO_CONTEXT &&
	      newContextType == DRI_2D_CONTEXT) {
      pI830->LockHeld = 0;
      if (I810_DEBUG & DEBUG_VERBOSE_DRI)
	 ErrorF("i830DRISwapContext (out)\n");
   } else if (I810_DEBUG & DEBUG_VERBOSE_DRI)
      ErrorF("i830DRISwapContext (other)\n");
}

static void
I830DRIInitBuffers(WindowPtr pWin, RegionPtr prgn, CARD32 index)
{
   ScreenPtr pScreen = pWin->drawable.pScreen;
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   BoxPtr pbox = REGION_RECTS(prgn);
   int nbox = REGION_NUM_RECTS(prgn);

   if (I810_DEBUG & DEBUG_VERBOSE_DRI)
      ErrorF("I830DRIInitBuffers\n");

   I830SetupForSolidFill(pScrn, 0, GXcopy, -1);
   while (nbox--) {
      I830SelectBuffer(pScrn, I830_SELECT_BACK);
      I830SubsequentSolidFillRect(pScrn, pbox->x1, pbox->y1,
				  pbox->x2 - pbox->x1, pbox->y2 - pbox->y1);
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
   pI830->AccelInfoRec->NeedToSync = TRUE;
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
   DDXPointPtr pptTmp, pptNew2;
   int xdir, ydir;

   int screenwidth = pScrn->virtualX;
   int screenheight = pScrn->virtualY;

   BoxPtr pbox = REGION_RECTS(prgnSrc);
   int nbox = REGION_NUM_RECTS(prgnSrc);

   BoxPtr pboxNew1 = 0;
   BoxPtr pboxNew2 = 0;
   DDXPointPtr pptNew1 = 0;
   DDXPointPtr pptSrc = &ptOldOrg;

   int dx = pParent->drawable.x - ptOldOrg.x;
   int dy = pParent->drawable.y - ptOldOrg.y;

   /* If the copy will overlap in Y, reverse the order */
   if (dy > 0) {
      ydir = -1;

      if (nbox > 1) {
	 /* Keep ordering in each band, reverse order of bands */
	 pboxNew1 = (BoxPtr) ALLOCATE_LOCAL(sizeof(BoxRec) * nbox);
	 if (!pboxNew1)
	    return;
	 pptNew1 = (DDXPointPtr) ALLOCATE_LOCAL(sizeof(DDXPointRec) * nbox);
	 if (!pptNew1) {
	    DEALLOCATE_LOCAL(pboxNew1);
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
	 pboxNew2 = (BoxPtr) ALLOCATE_LOCAL(sizeof(BoxRec) * nbox);
	 pptNew2 = (DDXPointPtr) ALLOCATE_LOCAL(sizeof(DDXPointRec) * nbox);
	 if (!pboxNew2 || !pptNew2) {
	    if (pptNew2)
	       DEALLOCATE_LOCAL(pptNew2);
	    if (pboxNew2)
	       DEALLOCATE_LOCAL(pboxNew2);
	    if (pboxNew1) {
	       DEALLOCATE_LOCAL(pptNew1);
	       DEALLOCATE_LOCAL(pboxNew1);
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
      I830SelectBuffer(pScrn, I830_SELECT_DEPTH);
      I830SubsequentScreenToScreenCopy(pScrn, x1, y1, destx, desty, w, h);
   }
   I830SelectBuffer(pScrn, I830_SELECT_FRONT);
   I830EmitFlush(pScrn);

   if (pboxNew2) {
      DEALLOCATE_LOCAL(pptNew2);
      DEALLOCATE_LOCAL(pboxNew2);
   }
   if (pboxNew1) {
      DEALLOCATE_LOCAL(pptNew1);
      DEALLOCATE_LOCAL(pboxNew1);
   }

   pI830->AccelInfoRec->NeedToSync = TRUE;
}

/* Initialize the first context */
void
I830EmitInvarientState(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   CARD32 ctx_addr;


   ctx_addr = pI830->ContextMem.Start;
   /* Align to a 2k boundry */
   ctx_addr = ((ctx_addr + 2048 - 1) / 2048) * 2048;

   {
      BEGIN_LP_RING(2);
      OUT_RING(MI_SET_CONTEXT);
      OUT_RING(ctx_addr |
	       CTXT_NO_RESTORE |
	       CTXT_PALETTE_SAVE_DISABLE | CTXT_PALETTE_RESTORE_DISABLE);
      ADVANCE_LP_RING();
   }
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


/* This should be done *before* XAA syncs,
 * Otherwise will have to sync again???
 */
static void
I830DRIShadowUpdate (ScreenPtr pScreen, shadowBufPtr pBuf)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   RegionPtr damage = &pBuf->damage;
   int i, num =  REGION_NUM_RECTS(damage);
   BoxPtr pbox = REGION_RECTS(damage);
   drmI830Sarea *pSAREAPriv = DRIGetSAREAPrivate(pScreen);
   int cmd, br13;

   /* Don't want to do this when no 3d is active and pages are
    * right-way-round :
    */
   if (!pSAREAPriv->pf_active && pSAREAPriv->pf_current_page == 0)
      return;

   br13 = (pScrn->displayWidth * pI830->cpp) | (0xcc << 16);

   if (pScrn->bitsPerPixel == 32) {
      cmd = (XY_SRC_COPY_BLT_CMD | XY_SRC_COPY_BLT_WRITE_ALPHA |
	     XY_SRC_COPY_BLT_WRITE_RGB);
      br13 |= 3 << 24;
   } else {
      cmd = (XY_SRC_COPY_BLT_CMD);
      br13 |= 1 << 24;
   }

   for (i = 0 ; i < num ; i++, pbox++) {
      BEGIN_LP_RING(8);
      OUT_RING(cmd);
      OUT_RING(br13);
      OUT_RING((pbox->y1 << 16) | pbox->x1);
      OUT_RING((pbox->y2 << 16) | pbox->x2);
      OUT_RING(pI830->BackBuffer.Start);
      OUT_RING((pbox->y1 << 16) | pbox->x1);
      OUT_RING(br13 & 0xffff);
      OUT_RING(pI830->FrontBuffer.Start);
      ADVANCE_LP_RING();
   }
}


static void
I830EnablePageFlip(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   drmI830Sarea *pSAREAPriv = DRIGetSAREAPrivate(pScreen);

   pSAREAPriv->pf_enabled = pI830->allowPageFlip;
   pSAREAPriv->pf_active = 0;

   if (pI830->allowPageFlip) {
      int br13 = (pScrn->displayWidth * pI830->cpp) | (0xcc << 16);

      BEGIN_LP_RING(8);
      if (pScrn->bitsPerPixel == 32) {
	 OUT_RING(XY_SRC_COPY_BLT_CMD | XY_SRC_COPY_BLT_WRITE_ALPHA |
		  XY_SRC_COPY_BLT_WRITE_RGB);
	 br13 |= 3 << 24;
      } else {
	 OUT_RING(XY_SRC_COPY_BLT_CMD);
	 br13 |= 1 << 24;
      }

      OUT_RING(br13);
      OUT_RING(0);
      OUT_RING((pScrn->virtualY << 16) | pScrn->virtualX);
      OUT_RING(pI830->BackBuffer.Start);
      OUT_RING(0);
      OUT_RING(br13 & 0xffff);
      OUT_RING(pI830->FrontBuffer.Start);
      ADVANCE_LP_RING();

      pSAREAPriv->pf_active = 1;
   }
}

static void
I830DisablePageFlip(ScreenPtr pScreen)
{
   drmI830Sarea *pSAREAPriv = DRIGetSAREAPrivate(pScreen);

   pSAREAPriv->pf_active = 0;
}


static void
I830DRITransitionSingleToMulti3d(ScreenPtr pScreen)
{
   /* Tell the clients not to pageflip.  How?
    *   -- Field in sarea, plus bumping the window counters.
    *   -- DRM needs to cope with Front-to-Back swapbuffers.
    */
   I830DisablePageFlip(pScreen);
}

static void
I830DRITransitionMultiToSingle3d(ScreenPtr pScreen)
{
   /* Let the remaining 3d app start page flipping again.
    */
   I830EnablePageFlip(pScreen);
}


static void
I830DRITransitionTo3d(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);

   I830EnablePageFlip(pScreen);
   pI830->have3DWindows = 1;
}


static void
I830DRITransitionTo2d(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);
   drmI830Sarea *sPriv = (drmI830Sarea *) DRIGetSAREAPrivate(pScreen);

   /* Try flipping back to the front page if necessary */
   if (sPriv->pf_current_page == 1)
      drmCommandNone(pI830->drmSubFD, DRM_I830_FLIP);

   /* Shut down shadowing if we've made it back to the front page:
    */
   if (sPriv->pf_current_page == 0) {
      I830DisablePageFlip(pScreen);
   }

   pI830->have3DWindows = 0;
}


