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
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Priv.h"

#include "xf86PciInfo.h"
#include "xf86Pci.h"

#define _XF86DRI_SERVER_
#include "GL/glxtokens.h"
#include "sarea.h"

#include "via.h"
#include "via_driver.h"
#include "via_drm.h"
#include "via_dri.h"
#include "via_id.h"
#include "xf86drm.h"

#define VIDEO	0 
#define AGP		1
#define AGP_PAGE_SIZE 4096
#define AGP_PAGES 8192
#define AGP_SIZE (AGP_PAGE_SIZE * AGP_PAGES)
#define AGP_CMDBUF_PAGES 512
#define AGP_CMDBUF_SIZE (AGP_PAGE_SIZE * AGP_CMDBUF_PAGES)
#define VIA_AGP_MODE_MASK 0x17
#define VIA_AGPv3_MODE    0x08
#define VIA_AGPv3_8X_MODE 0x02
#define VIA_AGPv3_4X_MODE 0x01
#define VIA_AGP_4X_MODE 0x04
#define VIA_AGP_2X_MODE 0x02
#define VIA_AGP_1X_MODE 0x01
#define VIA_AGP_FW_MODE 0x10

extern void GlxSetVisualConfigs(
    int nconfigs,
    __GLXvisualConfig *configs,
    void **configprivs
);

typedef struct {
    int major;
    int minor;
    int patchlevel;
} ViaDRMVersion;

static char VIAKernelDriverName[] = "via";
static char VIAClientDriverName[] = "unichrome";
static const ViaDRMVersion drmExpected = {1, 3, 0};
static const ViaDRMVersion drmCompat = {2, 0, 0};

int test_alloc_FB(ScreenPtr pScreen, VIAPtr pVia, int Size);
int test_alloc_AGP(ScreenPtr pScreen, VIAPtr pVia, int Size);
static Bool VIAInitVisualConfigs(ScreenPtr pScreen);
static Bool VIADRIAgpInit(ScreenPtr pScreen, VIAPtr pVia);
static Bool VIADRIPciInit(ScreenPtr pScreen, VIAPtr pVia);
static Bool VIADRIFBInit(ScreenPtr pScreen, VIAPtr pVia);
static Bool VIADRIKernelInit(ScreenPtr pScreen, VIAPtr pVia);
static Bool VIADRIMapInit(ScreenPtr pScreen, VIAPtr pVia);

static Bool VIACreateContext(ScreenPtr pScreen, VisualPtr visual, 
                   drm_context_t hwContext, void *pVisualConfigPriv,
                   DRIContextType contextStore);
static void VIADestroyContext(ScreenPtr pScreen, drm_context_t hwContext,
                   DRIContextType contextStore);
static void VIADRISwapContext(ScreenPtr pScreen, DRISyncType syncType, 
                   DRIContextType readContextType, 
                   void *readContextStore,
                   DRIContextType writeContextType, 
                   void *writeContextStore);
static void VIADRIInitBuffers(WindowPtr pWin, RegionPtr prgn, CARD32 index);
static void VIADRIMoveBuffers(WindowPtr pParent, DDXPointRec ptOldOrg, 
                   RegionPtr prgnSrc, CARD32 index);


static void VIADRIIrqInit( ScrnInfoPtr pScrn , VIADRIPtr pVIADRI)
{
    VIAPtr pVia = VIAPTR(pScrn);   
 
    pVIADRI->irqEnabled = drmGetInterruptFromBusID
	(pVia->drmFD,
	 ((pciConfigPtr)pVia->PciInfo->thisCard)->busnum,
	 ((pciConfigPtr)pVia->PciInfo->thisCard)->devnum,
	 ((pciConfigPtr)pVia->PciInfo->thisCard)->funcnum);
    if ((drmCtlInstHandler(pVia->drmFD, pVIADRI->irqEnabled))) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "[drm] Failure adding irq handler. "
		   "Falling back to irq-free operation.\n");
	pVIADRI->irqEnabled = 0;
    }

    if (pVIADRI->irqEnabled)
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "[drm] Irq handler installed, using IRQ %d.\n",
		   pVIADRI->irqEnabled);
}

static void VIADRIIrqExit( ScrnInfoPtr pScrn , VIADRIPtr pVIADRI) {

    VIAPtr pVia = VIAPTR(pScrn);   

    if (pVIADRI->irqEnabled) {
	if (drmCtlUninstHandler(pVia->drmFD)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,"[drm] Irq handler uninstalled.\n");
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "[drm] Could not uninstall irq handler.\n");
	}
    }
}
	    
void
VIADRIRingBufferCleanup(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIADRIPtr pVIADRI = pVia->pDRIInfo->devPrivate;

    if (pVIADRI->ringBufActive) {
	drm_via_dma_init_t ringBufInit;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
		   "[drm] Cleaning up DMA ring-buffer.\n");
	ringBufInit.func = VIA_CLEANUP_DMA;
	if (drmCommandWrite(pVia->drmFD, DRM_VIA_DMA_INIT, &ringBufInit,
			    sizeof(ringBufInit))) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, 
		       "[drm] Failed to clean up DMA ring-buffer: %d\n", errno);
	}
	pVIADRI->ringBufActive = 0;
    }
}

Bool
VIADRIRingBufferInit(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIADRIPtr pVIADRI = pVia->pDRIInfo->devPrivate;

    if (pVIADRI->ringBufActive)
	return TRUE;

    if (pVia->agpEnable) {
	drm_via_dma_init_t ringBufInit;

	if (((pVia->drmVerMajor == 1) && (pVia->drmVerMinor <= 3))) {
	    return FALSE;
	} 

	/*
	 * Info frome code-snippet on DRI-DEVEL list; Erdi Chen.
	 */

	switch (pVia->ChipId) {
	case PCI_CHIP_VT3259:
	    pVIADRI->reg_pause_addr = 0x40c;
	    break;
	default:
	    pVIADRI->reg_pause_addr = 0x418;
	    break;
	}
   
	ringBufInit.offset = pVia->agpSize;
	ringBufInit.size = AGP_CMDBUF_SIZE;
	ringBufInit.reg_pause_addr = pVIADRI->reg_pause_addr;
	ringBufInit.func = VIA_INIT_DMA;
	if (drmCommandWrite(pVia->drmFD, DRM_VIA_DMA_INIT, &ringBufInit,
			    sizeof(ringBufInit))) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, 
		       "[drm] Failed to initialize DMA ring-buffer: %d\n", errno);
	    return FALSE;
	}
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
		   "[drm] Initialized AGP ring-buffer, size 0x%lx at AGP offset 0x%lx.\n",
		   ringBufInit.size, ringBufInit.offset);
   
	pVIADRI->ringBufActive = 1;
    }
    return TRUE;
}	    

static Bool VIASetAgpMode(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    CARD32       mode   = drmAgpGetMode(pVia->drmFD);
    unsigned int vendor = drmAgpVendorId(pVia->drmFD);
    unsigned int device = drmAgpDeviceId(pVia->drmFD);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[drm] Detected AGP vendor 0x%x, device 0x%x\n",
	       vendor, device);

    mode &= ~VIA_AGP_MODE_MASK;
    if ((mode & VIA_AGPv3_MODE)) {
	mode |= 
	    VIA_AGPv3_8X_MODE | 
	    VIA_AGPv3_4X_MODE;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[drm] Found AGP v3 compatible device. "
		   "Trying AGP 8X mode.\n");
    } else {
	mode |= 
	    VIA_AGP_4X_MODE | 
	    VIA_AGP_2X_MODE |
	    VIA_AGP_1X_MODE;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[drm] Didn't find any AGP v3 compatible device. "
		   "Trying AGP 4X mode.\n");
    }
	
    mode |= VIA_AGP_FW_MODE;
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[drm] Trying to enable AGP fast writes.\n");

    if (drmAgpEnable(pVia->drmFD, mode) < 0) {
        return FALSE;
    }
    return TRUE;
}
    
    
	
static Bool VIADRIAgpInit(ScreenPtr pScreen, VIAPtr pVia)
{
    unsigned long  agp_phys;
    drmAddress agpaddr;
    VIADRIPtr pVIADRI;
    DRIInfoPtr pDRIInfo;
    pDRIInfo = pVia->pDRIInfo;
    pVIADRI = pDRIInfo->devPrivate;
    pVia->agpSize = 0;

    if (drmAgpAcquire(pVia->drmFD) < 0) {
        xf86DrvMsg(pScreen->myNum, X_ERROR, "[drm] drmAgpAcquire failed %d\n", errno);
        return FALSE;
    }

    if (!VIASetAgpMode(xf86Screens[pScreen->myNum])) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "[drm] VIASetAgpMode failed\n");
	drmAgpRelease(pVia->drmFD);
        return FALSE;
    }
    
    xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] drmAgpEnabled succeeded\n");

    if (drmAgpAlloc(pVia->drmFD, AGP_SIZE, 0, &agp_phys, &pVia->agpHandle) < 0) {
        xf86DrvMsg(pScreen->myNum, X_ERROR,
                 "[drm] drmAgpAlloc failed\n");
        drmAgpRelease(pVia->drmFD);
        return FALSE;
    }
   
    if (drmAgpBind(pVia->drmFD, pVia->agpHandle, 0) < 0) {
        xf86DrvMsg(pScreen->myNum, X_ERROR,
                 "[drm] drmAgpBind failed\n");
        drmAgpFree(pVia->drmFD, pVia->agpHandle);
        drmAgpRelease(pVia->drmFD);

        return FALSE;
    }

    /*
     * Place the ring-buffer last in the AGP region, and restrict the
     * public map not to include the buffer for security reasons.
     */

    pVia->agpSize = AGP_SIZE - AGP_CMDBUF_SIZE;
    pVia->agpAddr = drmAgpBase(pVia->drmFD);
    xf86DrvMsg(pScreen->myNum, X_INFO,
                 "[drm] agpAddr = 0x%08lx\n",pVia->agpAddr);
		 
    pVIADRI->agp.size = pVia->agpSize;
    if (drmAddMap(pVia->drmFD, (drm_handle_t)0,
                 pVIADRI->agp.size, DRM_AGP, 0, 
                 &pVIADRI->agp.handle) < 0) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
	    "[drm] Failed to map public agp area\n");
        pVIADRI->agp.size = 0;
	drmAgpUnbind(pVia->drmFD, pVia->agpHandle);
	drmAgpFree(pVia->drmFD, pVia->agpHandle);
	drmAgpRelease(pVia->drmFD);
	return FALSE;
    }  
    /* Map AGP from kernel to Xserver - Not really needed */
    drmMap(pVia->drmFD, pVIADRI->agp.handle, pVIADRI->agp.size, &agpaddr);
    pVia->agpMappedAddr = agpaddr;

    xf86DrvMsg(pScreen->myNum, X_INFO, 
                "[drm] agpBase = %p\n", pVia->agpBase);
    xf86DrvMsg(pScreen->myNum, X_INFO, 
                "[drm] agpAddr = 0x%08lx\n", pVia->agpAddr);
    xf86DrvMsg(pScreen->myNum, X_INFO, 
                "[drm] agpSize = 0x%08x\n", pVia->agpSize);
    xf86DrvMsg(pScreen->myNum, X_INFO, 
                "[drm] agp physical addr = 0x%08lx\n", agp_phys);
    
    {
	drm_via_agp_t agp;
	agp.offset = 0;
	agp.size = AGP_SIZE-AGP_CMDBUF_SIZE;
	if (drmCommandWrite(pVia->drmFD, DRM_VIA_AGP_INIT, &agp,
			    sizeof(drm_via_agp_t)) < 0) {
	    drmUnmap(agpaddr,pVia->agpSize);
	    drmRmMap(pVia->drmFD,pVIADRI->agp.handle);
	    drmAgpUnbind(pVia->drmFD, pVia->agpHandle);
	    drmAgpFree(pVia->drmFD, pVia->agpHandle);
	    drmAgpRelease(pVia->drmFD);
	    return FALSE;
	}
    }
    
    return TRUE;
  
}
static Bool VIADRIFBInit(ScreenPtr pScreen, VIAPtr pVia)
{   
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    int FBSize = pVia->driSize;
    int FBOffset;
    VIADRIPtr pVIADRI = pVia->pDRIInfo->devPrivate;

    if (FBSize < pVia->Bpl) {
        xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[drm] No DRM framebuffer heap available.\n");
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[drm] Please increase the frame buffer\n");
	xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[drm] memory area in BIOS. Disabling DRI.\n");
	return FALSE;
    }
    if (FBSize < 3*(pScrn->virtualY * pVia->Bpl)) {
	xf86DrvMsg(pScreen->myNum, X_WARNING,
		   "[drm] The DRM Heap and Pixmap cache memory could be too small\n");
	xf86DrvMsg(pScreen->myNum, X_WARNING,
		   "[drm] for optimal performance. Please increase the frame buffer\n");
	xf86DrvMsg(pScreen->myNum, X_WARNING,
		   "[drm] memory area in BIOS.\n");
    }

    pVia->driOffScreenMem.pool = 0;
    if (Success != viaOffScreenLinear(&pVia->driOffScreenMem, pScrn, FBSize)) {
        xf86DrvMsg(pScreen->myNum, X_ERROR,
		   "[drm] failed to allocate offscreen frame buffer area\n");
	return FALSE;
    }

    FBOffset = pVia->driOffScreenMem.base;

    pVIADRI->fbOffset = FBOffset;
    pVIADRI->fbSize = FBSize;
    
    {
	drm_via_fb_t fb;
	fb.offset = FBOffset;
	fb.size = FBSize;
	
	if (drmCommandWrite(pVia->drmFD, DRM_VIA_FB_INIT, &fb,
			    sizeof(drm_via_fb_t)) < 0) {
	    xf86DrvMsg(pScreen->myNum, X_ERROR,
		       "[drm] failed to init frame buffer area\n");
	    return FALSE;
	} else {
	    xf86DrvMsg(pScreen->myNum, X_INFO,
		       "[drm] Using %d bytes for DRM memory heap.\n", FBSize);
	    return TRUE;	
	}   
    }
}

static Bool VIADRIPciInit(ScreenPtr pScreen, VIAPtr pVia)
{
    return TRUE;	
}

static Bool
VIAInitVisualConfigs(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    VIAPtr pVia = VIAPTR(pScrn);
    int numConfigs = 0;
    __GLXvisualConfig *pConfigs = 0;
    VIAConfigPrivPtr pVIAConfigs = 0;
    VIAConfigPrivPtr *pVIAConfigPtrs = 0;
    int i, db, stencil, accum;

    switch (pScrn->bitsPerPixel) {
	case 8:
	case 24:
	    break;
	case 16:
	    numConfigs = 12;
	    if (!(pConfigs = (__GLXvisualConfig*)xcalloc(sizeof(__GLXvisualConfig),
						   numConfigs)))
		return FALSE;
	    if (!(pVIAConfigs = (VIAConfigPrivPtr)xcalloc(sizeof(VIAConfigPrivRec),
						    numConfigs))) {
    		xfree(pConfigs);
    		return FALSE;
	    }
	    if (!(pVIAConfigPtrs = (VIAConfigPrivPtr*)xcalloc(sizeof(VIAConfigPrivPtr),
							  numConfigs))) {
    		xfree(pConfigs);
    		xfree(pVIAConfigs);
    		return FALSE;
	    }
	    for (i=0; i<numConfigs; i++) 
    		pVIAConfigPtrs[i] = &pVIAConfigs[i];

	    i = 0;
	    for (accum = 0; accum <= 1; accum++) {
		/* 32bpp depth buffer disabled, as Mesa has limitations */
    		for (stencil=0; stencil<=2; stencil++) {
    		    for (db = 0; db <= 1; db++) {
        		pConfigs[i].vid = -1;
        		pConfigs[i].class = -1;
        		pConfigs[i].rgba = TRUE;
        		pConfigs[i].redSize = -1;
        		pConfigs[i].greenSize = -1;
        		pConfigs[i].blueSize = -1;
        		pConfigs[i].redMask = -1;
        		pConfigs[i].greenMask = -1;
        		pConfigs[i].blueMask = -1;
			pConfigs[i].alphaSize = 0;
        		pConfigs[i].alphaMask = 0;
        		
			if (accum) {
        		    pConfigs[i].accumRedSize = 16;
        		    pConfigs[i].accumGreenSize = 16;
        		    pConfigs[i].accumBlueSize = 16;
        		    pConfigs[i].accumAlphaSize = 0;
        		}
			else {
        		    pConfigs[i].accumRedSize = 0;
        		    pConfigs[i].accumGreenSize = 0;
        		    pConfigs[i].accumBlueSize = 0;
        		    pConfigs[i].accumAlphaSize = 0;
        		}
        		if (!db)
        		    pConfigs[i].doubleBuffer = TRUE;
        		else
        		    pConfigs[i].doubleBuffer = FALSE;
        		
			pConfigs[i].stereo = FALSE;
        		pConfigs[i].bufferSize = -1;
        		
			switch (stencil) {
        		    case 0:
            			pConfigs[i].depthSize = 24;
            			pConfigs[i].stencilSize = 8;
            			break;
        		    case 1:
            			pConfigs[i].depthSize = 16;
            			pConfigs[i].stencilSize = 0;
            			break;
        		    case 2:
            			pConfigs[i].depthSize = 0;
            			pConfigs[i].stencilSize = 0;
            			break;
        		    case 3:
            			pConfigs[i].depthSize = 32;
            			pConfigs[i].stencilSize = 0;
            			break;
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
		"[dri] Incorrect initialization of visuals.  Disabling DRI.\n");
    		return FALSE;
	}
	break;
	    
	case 32:
	    numConfigs = 12;
	    if (!(pConfigs = (__GLXvisualConfig*)xcalloc(sizeof(__GLXvisualConfig),
						   numConfigs)))
		return FALSE;
	    if (!(pVIAConfigs = (VIAConfigPrivPtr)xcalloc(sizeof(VIAConfigPrivRec),
						    numConfigs))) {
    		xfree(pConfigs);
    		return FALSE;
	    }
	    if (!(pVIAConfigPtrs = (VIAConfigPrivPtr*)xcalloc(sizeof(VIAConfigPrivPtr),
							  numConfigs))) {
    		xfree(pConfigs);
    		xfree(pVIAConfigs);
    		return FALSE;
	    }
	    for (i=0; i<numConfigs; i++) 
    		pVIAConfigPtrs[i] = &pVIAConfigs[i];

	    i = 0;
	    for (accum = 0; accum <= 1; accum++) {
		/* 32bpp depth buffer disabled, as Mesa has limitations */
    		for (stencil=0; stencil<=2; stencil++) {
    		    for (db = 0; db <= 1; db++) {
        		pConfigs[i].vid = -1;
        		pConfigs[i].class = -1;
        		pConfigs[i].rgba = TRUE;
        		pConfigs[i].redSize = -1;
        		pConfigs[i].greenSize = -1;
        		pConfigs[i].blueSize = -1;
        		pConfigs[i].redMask = -1;
        		pConfigs[i].greenMask = -1;
        		pConfigs[i].blueMask = -1;
			pConfigs[i].alphaSize = 8;
        		pConfigs[i].alphaMask = 0xFF000000;
        		
			if (accum) {
        		    pConfigs[i].accumRedSize = 16;
        		    pConfigs[i].accumGreenSize = 16;
        		    pConfigs[i].accumBlueSize = 16;
        		    pConfigs[i].accumAlphaSize = 16;
        		}
			else {
        		    pConfigs[i].accumRedSize = 0;
        		    pConfigs[i].accumGreenSize = 0;
        		    pConfigs[i].accumBlueSize = 0;
        		    pConfigs[i].accumAlphaSize = 0;
        		}
        		if (!db)
        		    pConfigs[i].doubleBuffer = TRUE;
        		else
        		    pConfigs[i].doubleBuffer = FALSE;
        		
			pConfigs[i].stereo = FALSE;
        		pConfigs[i].bufferSize = -1;
        		
			switch (stencil) {
        		    case 0:
            			pConfigs[i].depthSize = 24;
            			pConfigs[i].stencilSize = 8;
            			break;
        		    case 1:
            			pConfigs[i].depthSize = 16;
            			pConfigs[i].stencilSize = 0;
            			break;
        		    case 2:
            			pConfigs[i].depthSize = 0;
            			pConfigs[i].stencilSize = 0;
            			break;
        		    case 3:
            			pConfigs[i].depthSize = 32;
            			pConfigs[i].stencilSize = 0;
            			break;
        		}
        		
			pConfigs[i].auxBuffers = 0;
        		pConfigs[i].level = 0;
			if (accum)
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
	
	if (i != numConfigs) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"[dri] Incorrect initialization of visuals.  Disabling DRI.\n");
    		return FALSE;
	}
	    
	break;
    }
 
    pVia->numVisualConfigs = numConfigs;
    pVia->pVisualConfigs = pConfigs;
    pVia->pVisualConfigsPriv = pVIAConfigs;
    GlxSetVisualConfigs(numConfigs, pConfigs, (void**)pVIAConfigPtrs);

    return TRUE;
}

Bool VIADRIScreenInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    VIAPtr pVia = VIAPTR(pScrn);
    DRIInfoPtr pDRIInfo;
    VIADRIPtr pVIADRI;
    drmVersionPtr drmVer;

    /* if symbols or version check fails, we still want this to be NULL */
    pVia->pDRIInfo = NULL;

    /* Check that the GLX, DRI, and DRM modules have been loaded by testing
    * for canonical symbols in each module. */
    if (!xf86LoaderCheckSymbol("GlxSetVisualConfigs")) return FALSE;
    if (!xf86LoaderCheckSymbol("drmAvailable"))        return FALSE;
    if (!xf86LoaderCheckSymbol("DRIQueryVersion")) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
                 "[dri] VIADRIScreenInit failed (libdri.a too old)\n");
        return FALSE;
    }
    
    /* Check the DRI version */
    {
        int major, minor, patch;
        DRIQueryVersion(&major, &minor, &patch);
        if (major != DRIINFO_MAJOR_VERSION || minor < DRIINFO_MINOR_VERSION) {
            xf86DrvMsg(pScreen->myNum, X_ERROR,
                    "[dri] VIADRIScreenInit failed because of a version mismatch.\n"
                    "[dri] libdri version is %d.%d.%d but version %d.%d.x is needed.\n"
                    "[dri] Disabling DRI.\n",
                    major, minor, patch,
                    DRIINFO_MAJOR_VERSION, DRIINFO_MINOR_VERSION);
            return FALSE;
        }
    }

    pVia->pDRIInfo = DRICreateInfoRec();
    if (!pVia->pDRIInfo)
	return FALSE;
    
    pDRIInfo = pVia->pDRIInfo;
    pDRIInfo->drmDriverName = VIAKernelDriverName;
    pDRIInfo->clientDriverName = VIAClientDriverName;
    pDRIInfo->busIdString = xalloc(64);
    sprintf(pDRIInfo->busIdString, "PCI:%d:%d:%d",
        ((pciConfigPtr)pVia->PciInfo->thisCard)->busnum,
        ((pciConfigPtr)pVia->PciInfo->thisCard)->devnum,
        ((pciConfigPtr)pVia->PciInfo->thisCard)->funcnum);
    pDRIInfo->ddxDriverMajorVersion = VIA_DRIDDX_VERSION_MAJOR;
    pDRIInfo->ddxDriverMinorVersion = VIA_DRIDDX_VERSION_MINOR;
    pDRIInfo->ddxDriverPatchVersion = VIA_DRIDDX_VERSION_PATCH;
    pDRIInfo->frameBufferPhysicalAddress = (pointer) pVia->FrameBufferBase;
    pDRIInfo->frameBufferSize = pVia->videoRambytes;  
  
    pDRIInfo->frameBufferStride = (pScrn->displayWidth *
					    pScrn->bitsPerPixel / 8);
    pDRIInfo->ddxDrawableTableEntry = VIA_MAX_DRAWABLES;

    if (SAREA_MAX_DRAWABLES < VIA_MAX_DRAWABLES)
	pDRIInfo->maxDrawableTableEntry = SAREA_MAX_DRAWABLES;
    else
	pDRIInfo->maxDrawableTableEntry = VIA_MAX_DRAWABLES;

#ifdef NOT_DONE
    /* FIXME need to extend DRI protocol to pass this size back to client 
    * for SAREA mapping that includes a device private record
    */
    pDRIInfo->SAREASize = 
	((sizeof(XF86DRISAREARec) + 0xfff) & 0x1000); /* round to page */
    /* + shared memory device private rec */
#else
    /* For now the mapping works by using a fixed size defined
    * in the SAREA header
    */
    if (sizeof(XF86DRISAREARec)+sizeof(drm_via_sarea_t) > SAREA_MAX) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Data does not fit in SAREA\n");
	DRIDestroyInfoRec(pVia->pDRIInfo);
	pVia->pDRIInfo = NULL;
	return FALSE;
    }
    pDRIInfo->SAREASize = SAREA_MAX;
#endif

    if (!(pVIADRI = (VIADRIPtr)xcalloc(sizeof(VIADRIRec),1))) {
	DRIDestroyInfoRec(pVia->pDRIInfo);
	pVia->pDRIInfo = NULL;
	return FALSE;
    }
    pDRIInfo->devPrivate = pVIADRI;
    pDRIInfo->devPrivateSize = sizeof(VIADRIRec);
    pDRIInfo->contextSize = sizeof(VIADRIContextRec);

    pDRIInfo->CreateContext = VIACreateContext;
    pDRIInfo->DestroyContext = VIADestroyContext;
    pDRIInfo->SwapContext = VIADRISwapContext;
    pDRIInfo->InitBuffers = VIADRIInitBuffers;
    pDRIInfo->MoveBuffers = VIADRIMoveBuffers;
    pDRIInfo->bufferRequests = DRI_ALL_WINDOWS;

    if (!DRIScreenInit(pScreen, pDRIInfo, &pVia->drmFD)) {
	xf86DrvMsg(pScreen->myNum, X_ERROR,
	    "[dri] DRIScreenInit failed.  Disabling DRI.\n");
	xfree(pDRIInfo->devPrivate);
	pDRIInfo->devPrivate = NULL;
	DRIDestroyInfoRec(pVia->pDRIInfo);
	pVia->pDRIInfo = NULL;
	pVia->drmFD = -1;
	return FALSE;
    }

    if (NULL == (drmVer = drmGetVersion(pVia->drmFD))) {
	VIADRICloseScreen(pScreen);
	return FALSE;
    }
    pVia->drmVerMajor = drmVer->version_major;
    pVia->drmVerMinor = drmVer->version_minor;
    pVia->drmVerPL = drmVer->version_patchlevel;

    if ((drmVer->version_major < drmExpected.major) || 
	(drmVer->version_major > drmCompat.major) ||
	((drmVer->version_major == drmExpected.major ) && 
	(drmVer->version_minor < drmExpected.minor))) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, 
		   "[dri] Kernel drm is not compatible with this driver.\n"); 
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, 
		   "[dri] Kernel drm version: %d.%d.%d "
		   "and I can work with versions %d.%d.x - %d.x.x\n",
		   drmVer->version_major,drmVer->version_minor,
		   drmVer->version_patchlevel, drmExpected.major, 
		   drmExpected.minor, drmCompat.major); 	
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, 
		   "[dri] Please update either this 2D driver or your kernel DRM. "
		   "Disabling DRI.\n");
	drmFreeVersion(drmVer);
	VIADRICloseScreen(pScreen);
	return FALSE;
    } 
    drmFreeVersion(drmVer);

	   
    if (!(VIAInitVisualConfigs(pScreen))) {
	VIADRICloseScreen(pScreen);
	return FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] visual configs initialized.\n" );
  
    /* DRIScreenInit doesn't add all the common mappings.  Add additional mappings here. */
    if (!VIADRIMapInit(pScreen, pVia)) {
	VIADRICloseScreen(pScreen);
	return FALSE;
    }
    pVIADRI->regs.size = VIA_MMIO_REGSIZE;
    pVIADRI->regs.handle = pVia->registerHandle;
    xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] mmio Registers = 0x%08lx\n",
               (unsigned long) pVIADRI->regs.handle);
    
    pVIADRI->drixinerama = pVia->drixinerama;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] mmio mapped.\n" );

    return TRUE;
}

void
VIADRICloseScreen(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    VIAPtr pVia = VIAPTR(pScrn);
    VIADRIPtr pVIADRI;

    VIADRIRingBufferCleanup(pScrn); 
    if (pVia->agpSize) {
	drmUnmap(pVia->agpMappedAddr,pVia->agpSize);
	drmRmMap(pVia->drmFD,pVia->agpHandle);
	drmAgpUnbind(pVia->drmFD, pVia->agpHandle);
	xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] Freeing agp memory\n");
	drmAgpFree(pVia->drmFD, pVia->agpHandle);
	xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] Releasing agp module\n");
	drmAgpRelease(pVia->drmFD);
    }

    DRICloseScreen(pScreen);
    VIAFreeLinear(&pVia->driOffScreenMem);
    
    if (pVia->pDRIInfo) {
	if ((pVIADRI = (VIADRIPtr) pVia->pDRIInfo->devPrivate)) {
	    VIADRIIrqExit(pScrn, pVIADRI);
    	    xfree(pVIADRI);
    	    pVia->pDRIInfo->devPrivate = NULL;
	}
	DRIDestroyInfoRec(pVia->pDRIInfo);
	pVia->pDRIInfo = NULL;
    }
    
    if (pVia->pVisualConfigs) {
	xfree(pVia->pVisualConfigs);
	pVia->pVisualConfigs = NULL;
    }
    if (pVia->pVisualConfigsPriv) {
	xfree(pVia->pVisualConfigsPriv);
	pVia->pVisualConfigsPriv = NULL;
    }
}

/* TODO: xserver receives driver's swapping event and does something
 *       according the data initialized in this function. 
 */
static Bool
VIACreateContext(ScreenPtr pScreen, VisualPtr visual,
          drm_context_t hwContext, void *pVisualConfigPriv,
          DRIContextType contextStore)
{
    return TRUE;
}

static void
VIADestroyContext(ScreenPtr pScreen, drm_context_t hwContext, 
           DRIContextType contextStore)
{
}

Bool
VIADRIFinishScreenInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    VIAPtr pVia = VIAPTR(pScrn);
    VIADRIPtr pVIADRI;

    pVia->pDRIInfo->driverSwapMethod = DRI_HIDE_X_CONTEXT;
    
    pVia->IsPCI = !VIADRIAgpInit(pScreen, pVia);
  
    if (pVia->IsPCI) {
	VIADRIPciInit(pScreen, pVia);
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] use pci.\n" );
    }
    else
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] use agp.\n" );

    if (!(VIADRIFBInit(pScreen, pVia))) {
	VIADRICloseScreen(pScreen);
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "[dri] frame buffer initialization failed.\n" );
	return FALSE;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] frame buffer initialized.\n" );
  
    DRIFinishScreenInit(pScreen);
    
    if (!VIADRIKernelInit(pScreen, pVia)) {
	VIADRICloseScreen(pScreen);
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO, "[dri] kernel data initialized.\n");

    /* set SAREA value */
    {
	drm_via_sarea_t *saPriv;

	saPriv=(drm_via_sarea_t *)DRIGetSAREAPrivate(pScreen);
	memset(saPriv, 0, sizeof(*saPriv));
	saPriv->ctxOwner = -1;
    }
    pVIADRI=(VIADRIPtr)pVia->pDRIInfo->devPrivate;
    pVIADRI->deviceID=pVia->Chipset;  
    pVIADRI->width=pScrn->virtualX;
    pVIADRI->height=pScrn->virtualY;
    pVIADRI->mem=pScrn->videoRam*1024;
    pVIADRI->bytesPerPixel= (pScrn->bitsPerPixel+7) / 8; 
    pVIADRI->sarea_priv_offset = sizeof(XF86DRISAREARec);
    /* TODO */
    pVIADRI->scrnX=pVIADRI->width;
    pVIADRI->scrnY=pVIADRI->height;

    /* Initialize IRQ */
    if (pVia->DRIIrqEnable) 
	VIADRIIrqInit(pScrn, pVIADRI);
    
    pVIADRI->ringBufActive = 0;
    VIADRIRingBufferInit(pScrn);
    return TRUE;
}

static void
VIADRISwapContext(ScreenPtr pScreen, DRISyncType syncType, 
           DRIContextType oldContextType, void *oldContext,
           DRIContextType newContextType, void *newContext)
{
  /*ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
  VIAPtr pVia = VIAPTR(pScrn);
  */   
  return;
}

static void
VIADRIInitBuffers(WindowPtr pWin, RegionPtr prgn, CARD32 index)
{
  /*ScreenPtr pScreen = pWin->drawable.pScreen;
  ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
  VIAPtr pVia = VIAPTR(pScrn);
  */
  return;
}

static void
VIADRIMoveBuffers(WindowPtr pParent, DDXPointRec ptOldOrg, 
           RegionPtr prgnSrc, CARD32 index)
{
  /*ScreenPtr pScreen = pParent->drawable.pScreen;
  ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
  VIAPtr pVia = VIAPTR(pScrn);
  */
  return;
}

/* Initialize the kernel data structures. */
static Bool VIADRIKernelInit(ScreenPtr pScreen, VIAPtr pVia)
{
    drm_via_init_t drmInfo;
    memset(&drmInfo, 0, sizeof(drm_via_init_t));
    drmInfo.func = VIA_INIT_MAP;
    drmInfo.sarea_priv_offset   = sizeof(XF86DRISAREARec);
    drmInfo.fb_offset           = pVia->frameBufferHandle;
    drmInfo.mmio_offset         = pVia->registerHandle;
    if (pVia->IsPCI)
	drmInfo.agpAddr = (CARD32)NULL;
    else
	drmInfo.agpAddr = (CARD32)pVia->agpAddr;

	if ((drmCommandWrite(pVia->drmFD, DRM_VIA_MAP_INIT,&drmInfo,
			     sizeof(drm_via_init_t))) < 0)
	    return FALSE;
	     

    return TRUE;
}
/* Add a map for the MMIO registers */
static Bool VIADRIMapInit(ScreenPtr pScreen, VIAPtr pVia)
{
    int flags = DRM_READ_ONLY;

    if (drmAddMap(pVia->drmFD, pVia->MmioBase, VIA_MMIO_REGSIZE,
		  DRM_REGISTERS, flags, &pVia->registerHandle) < 0) {
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] register handle = 0x%08lx\n",
               (unsigned long) pVia->registerHandle);
    if (drmAddMap(pVia->drmFD, pVia->FrameBufferBase, pVia->videoRambytes,
		  DRM_FRAME_BUFFER, 0, &pVia->frameBufferHandle) < 0) {
	return FALSE;
    }
    xf86DrvMsg(pScreen->myNum, X_INFO, "[drm] framebuffer handle = 0x%08lx\n",
               (unsigned long) pVia->frameBufferHandle);
    
    return TRUE;
}
