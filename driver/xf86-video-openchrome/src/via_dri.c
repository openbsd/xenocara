/*
 * Copyright 2005-2015 The Openchrome Project
 *                     [https://www.freedesktop.org/wiki/Openchrome]
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

#include "xf86Pci.h"

#define _XF86DRI_SERVER_
#include "GL/glxtokens.h"
#include "sarea.h"

#include "via_driver.h"
#include "via_drm.h"
#include "via_dri.h"
#include "xf86drm.h"

#ifndef DRIINFO_MAJOR_VERSION
#define DRIINFO_MAJOR_VERSION 4
#endif
#ifndef DRIINFO_MINOR_VERSION
#define DRIINFO_MINOR_VERSION 0
#endif

#define VIDEO  0
#define AGP    1
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

extern void GlxSetVisualConfigs(int nconfigs,
                                __GLXvisualConfig * configs,
                                void **configprivs);

static char VIAKernelDriverName[] = "via";
static char VIAClientDriverName[] = "unichrome";

static char SWRastClientDriverName[] = "swrast";

static Bool VIAInitVisualConfigs(ScreenPtr pScreen);
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

void
kickVblank(ScrnInfoPtr pScrn)
{
	/*
	 * Switching mode will clear registers that make vblank
	 * interrupts happen. If the driver thinks interrupts
	 * are enabled, make sure vblank interrupts go through.
	 * registers are not documented in VIA docs.
	 */
	VIAPtr pVia = VIAPTR(pScrn);
	vgaHWPtr hwp = VGAHWPTR(pScrn);
	VIADRIPtr pVIADRI = pVia->pDRIInfo->devPrivate;

	if (pVIADRI->irqEnabled)
        hwp->writeCrtc(hwp, 0x11, hwp->readCrtc(hwp, 0x11) | 0x30);
}

static void
VIADRIIrqInit(ScrnInfoPtr pScrn, VIADRIPtr pVIADRI)
{
    VIAPtr pVia = VIAPTR(pScrn);

    pVIADRI->irqEnabled = drmGetInterruptFromBusID
            (pVia->drmmode.fd,
#ifdef XSERVER_LIBPCIACCESS
             ((pVia->PciInfo->domain << 8) | pVia->PciInfo->bus),
             pVia->PciInfo->dev, pVia->PciInfo->func
#else
             ((pciConfigPtr)pVia->PciInfo->thisCard)->busnum,
             ((pciConfigPtr)pVia->PciInfo->thisCard)->devnum,
             ((pciConfigPtr)pVia->PciInfo->thisCard)->funcnum
#endif
            );
    if ((drmCtlInstHandler(pVia->drmmode.fd, pVIADRI->irqEnabled))) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "[drm] Failure adding IRQ handler. "
                   "Falling back to IRQ-free operation.\n");
        pVIADRI->irqEnabled = 0;
    }

    if (pVIADRI->irqEnabled)
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "[drm] IRQ handler installed, using IRQ %d.\n",
                   pVIADRI->irqEnabled);
}

static void
VIADRIIrqExit(ScrnInfoPtr pScrn, VIADRIPtr pVIADRI)
{
    VIAPtr pVia = VIAPTR(pScrn);

    if (pVIADRI->irqEnabled) {
        if (drmCtlUninstHandler(pVia->drmmode.fd)) {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "[drm] IRQ handler uninstalled.\n");
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "[drm] Could not uninstall IRQ handler.\n");
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
        if (drmCommandWrite(pVia->drmmode.fd, DRM_VIA_DMA_INIT, &ringBufInit,
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

        if ((pVia->drmVerMajor == 1) && (pVia->drmVerMinor <= 3))
            return FALSE;

        /*
         * Info from code-snippet on DRI-DEVEL list; Erdi Chen.
         */

        switch (pVia->ChipId) {
            case PCI_CHIP_VT3314:
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

        if (drmCommandWrite(pVia->drmmode.fd, DRM_VIA_DMA_INIT, &ringBufInit,
                            sizeof(ringBufInit))) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "[drm] Failed to initialize DMA ring-buffer: %d\n",
                       errno);
            return FALSE;
        }

        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "[drm] Initialized AGP ring-buffer, size 0x%lx at AGP "
                   "offset 0x%lx.\n", ringBufInit.size, ringBufInit.offset);
        pVIADRI->ringBufActive = 1;
    }
    return TRUE;
}

static Bool
VIASetAgpMode(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    CARD32 mode = drmAgpGetMode(pVia->drmmode.fd);
    unsigned int vendor = drmAgpVendorId(pVia->drmmode.fd);
    unsigned int device = drmAgpDeviceId(pVia->drmmode.fd);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[drm] Detected AGP "
               "vendor 0x%04x, device 0x%04x\n", vendor, device);

    mode &= ~VIA_AGP_MODE_MASK;
    if (mode & VIA_AGPv3_MODE) {
        mode |= VIA_AGPv3_8X_MODE;
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[drm] Found AGP v3 "
                   "compatible device. Trying AGP 8X mode.\n");
    } else {
        mode |= VIA_AGP_4X_MODE;
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[drm] Didn't find any AGP v3 "
                   "compatible device. Trying AGP 4X mode.\n");
    }

    mode |= VIA_AGP_FW_MODE;
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "[drm] Trying to enable AGP fast writes.\n");

    if (drmAgpEnable(pVia->drmmode.fd, mode) < 0)
        return FALSE;
    return TRUE;
}

static Bool
VIADRIAgpInit(ScrnInfoPtr pScrn)
{
    unsigned long agpCmdSize, agp_phys;
    VIAPtr pVia = VIAPTR(pScrn);
    drm_handle_t handle;
    drmAddress agpaddr;
    drm_via_agp_t agp;
    int agpPages;

    pVia->agpSize = 0;
    if (drmAgpAcquire(pVia->drmmode.fd) < 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "[drm] drmAgpAcquire failed %d\n",
                   errno);
        return FALSE;
    }

    if (!VIASetAgpMode(pScrn)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "[drm] VIASetAgpMode failed\n");
        drmAgpRelease(pVia->drmmode.fd);
        return FALSE;
    }

    agpCmdSize = (pVia->agpEnable) ? AGP_CMDBUF_SIZE : 0;
    if (pVia->agpMem * 1024 < agpCmdSize + AGP_PAGE_SIZE) {
        pVia->agpMem = (agpCmdSize + AGP_PAGE_SIZE) / 1024;
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "[drm] Forcing AGP size to %d kB\n", pVia->agpMem);
    }

    agpPages = (pVia->agpMem * 1024 + AGP_PAGE_SIZE - 1) / AGP_PAGE_SIZE;
    if (drmAgpAlloc(pVia->drmmode.fd, agpPages * AGP_PAGE_SIZE,
                    0, &agp_phys, &pVia->agpHandle) < 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "[drm] drmAgpAlloc failed\n");
        drmAgpRelease(pVia->drmmode.fd);
        return FALSE;
    }

    if (drmAgpBind(pVia->drmmode.fd, pVia->agpHandle, 0) < 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "[drm] drmAgpBind failed\n");
        drmAgpFree(pVia->drmmode.fd, pVia->agpHandle);
        drmAgpRelease(pVia->drmmode.fd);
        return FALSE;
    }

    /*
     * Place the ring-buffer last in the AGP region, and restrict the
     * public map not to include the buffer for security reasons.
     */
    pVia->agpSize = agpPages * AGP_PAGE_SIZE - agpCmdSize;
    pVia->agpAddr = drmAgpBase(pVia->drmmode.fd);
    agp.offset = 0;
    agp.size = pVia->agpSize;
    if (drmCommandWrite(pVia->drmmode.fd, DRM_VIA_AGP_INIT, &agp,
                        sizeof(drm_via_agp_t)) < 0) {
        drmAgpUnbind(pVia->drmmode.fd, pVia->agpHandle);
        drmAgpFree(pVia->drmmode.fd, pVia->agpHandle);
        drmAgpRelease(pVia->drmmode.fd);
        return FALSE;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[drm] drmAgpEnabled succeeded\n");

    /* Allocate all of AGP memory */
    if (drmAddMap(pVia->drmmode.fd, 0, pVia->agpSize,
                  DRM_AGP, 0, &handle) < 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[drm] Failed to map public agp area.\n");
        pVia->agpSize = 0;
        drmAgpUnbind(pVia->drmmode.fd, pVia->agpHandle);
        drmAgpFree(pVia->drmmode.fd, pVia->agpHandle);
        drmAgpRelease(pVia->drmmode.fd);
        return FALSE;
    }
    drmMap(pVia->drmmode.fd, handle, pVia->agpSize, &agpaddr);
    pVia->agpMappedAddr = agpaddr;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "[drm] agpAddr = 0x%08lx\n", pVia->agpAddr);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "[drm] agpSize = 0x%08x\n", pVia->agpSize);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "[drm] agp physical addr = %p\n", pVia->agpMappedAddr);
    return TRUE;
}

static Bool
VIADRIFBInit(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    drm_via_fb_t fb;

    fb.offset = pVia->FBFreeStart;
    fb.size = pVia->FBFreeEnd;
    if (drmCommandWrite(pVia->drmmode.fd, DRM_VIA_FB_INIT, &fb,
                        sizeof(drm_via_fb_t)) < 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "[drm] Failed to initialize frame buffer area.\n");
        return FALSE;
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "[drm] Using %d KB for DRM memory heap.\n", fb.size >> 10);
        return TRUE;
    }
}

static Bool
VIAInitVisualConfigs(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    int numConfigs = 0;
    __GLXvisualConfig *pConfigs = 0;
    VIAConfigPrivPtr pVIAConfigs = 0;
    VIAConfigPrivPtr *pVIAConfigPtrs = 0;
    int i, db, stencil, accum;

    if (pScrn->bitsPerPixel == 16 || pScrn->bitsPerPixel == 32) {
        numConfigs = 12;
        if (!(pConfigs = (__GLXvisualConfig *)
                         calloc(sizeof(__GLXvisualConfig), numConfigs)))
            return FALSE;
        if (!(pVIAConfigs = (VIAConfigPrivPtr)
                            calloc(sizeof(VIAConfigPrivRec), numConfigs))) {
            free(pConfigs);
            return FALSE;
        }
        if (!(pVIAConfigPtrs = (VIAConfigPrivPtr *)
                               calloc(sizeof(VIAConfigPrivPtr), numConfigs))) {
            free(pConfigs);
            free(pVIAConfigs);
            return FALSE;
        }
        for (i = 0; i < numConfigs; i++)
            pVIAConfigPtrs[i] = &pVIAConfigs[i];

        i = 0;
        for (accum = 0; accum <= 1; accum++) {
            /* 32bpp depth buffer disabled, as Mesa has limitations */
            for (stencil = 0; stencil <= 2; stencil++) {
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
                    if (pScrn->bitsPerPixel == 32) {
                        pConfigs[i].alphaSize = 8;
                        pConfigs[i].alphaMask = 0xFF000000;
                    } else {
                        pConfigs[i].alphaSize = 0;
                        pConfigs[i].alphaMask = 0;
                    }

                    if (accum) {
                        pConfigs[i].accumRedSize = 16;
                        pConfigs[i].accumGreenSize = 16;
                        pConfigs[i].accumBlueSize = 16;
                        if (pScrn->bitsPerPixel == 32)
                            pConfigs[i].accumAlphaSize = 16;
                        else
                            pConfigs[i].accumAlphaSize = 0;
                    } else {
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
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "[dri] Incorrect "
                       "initialization of visuals.  Disabling DRI.\n");

            if (pConfigs)
                free(pConfigs);
            if (pVIAConfigs)
                free(pVIAConfigs);
            if (pVIAConfigPtrs)
                free(pVIAConfigPtrs);

            return FALSE;
        }
    }

    pVia->numVisualConfigs = numConfigs;
    pVia->pVisualConfigs = pConfigs;
    pVia->pVisualConfigsPriv = pVIAConfigs;
    GlxSetVisualConfigs(numConfigs, pConfigs, (void **)pVIAConfigPtrs);

    return TRUE;
}

Bool
VIADRI1ScreenInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    int major, minor, patch;
    DRIInfoPtr pDRIInfo;
    VIADRIPtr pVIADRI;

    /* If symbols or version check fails, we still want this to be NULL. */
    pVia->pDRIInfo = NULL;
    drmClose(pVia->drmmode.fd);

    /* Check that the GLX, DRI, and DRM modules have been loaded by testing
     * for canonical symbols in each module. */
    if (!xf86LoaderCheckSymbol("GlxSetVisualConfigs"))
        return FALSE;
    if (!xf86LoaderCheckSymbol("drmAvailable"))
        return FALSE;
    if (!xf86LoaderCheckSymbol("DRIQueryVersion")) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[dri] VIADRI1ScreenInit failed (libdri.a is too old).\n");
        return FALSE;
    }

    /* Check the DRI version. */
    DRIQueryVersion(&major, &minor, &patch);
    if (major != DRIINFO_MAJOR_VERSION || minor < DRIINFO_MINOR_VERSION) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "[dri] VIADRI1ScreenInit failed -- FALSE mismatch.\n"
                    "[dri] libdri is %d.%d.%d, but %d.%d.x is needed.\n"
                    "[dri] Disabling DRI.\n",
                    major, minor, patch,
                    DRIINFO_MAJOR_VERSION, DRIINFO_MINOR_VERSION);
        return FALSE;
    }

    pVia->pDRIInfo = DRICreateInfoRec();
    if (!pVia->pDRIInfo)
        return FALSE;

    pDRIInfo = pVia->pDRIInfo;
    pDRIInfo->drmDriverName = VIAKernelDriverName;
    switch (pVia->Chipset) {
        case VIA_K8M890:
        case VIA_P4M900:
        case VIA_VX800:
        case VIA_VX855:
        case VIA_VX900:
            pDRIInfo->clientDriverName = SWRastClientDriverName;
            break;
        default:
            pDRIInfo->clientDriverName = VIAClientDriverName;
            break;
    }
    pDRIInfo->busIdString = DRICreatePCIBusID(pVia->PciInfo);
    pDRIInfo->ddxDriverMajorVersion = VIA_DRIDDX_VERSION_MAJOR;
    pDRIInfo->ddxDriverMinorVersion = VIA_DRIDDX_VERSION_MINOR;
    pDRIInfo->ddxDriverPatchVersion = VIA_DRIDDX_VERSION_PATCH;
#if (DRIINFO_MAJOR_VERSION == 5)
    pDRIInfo->frameBufferPhysicalAddress = (pointer) pVia->FrameBufferBase;
#else
    pDRIInfo->frameBufferPhysicalAddress = pVia->FrameBufferBase;
#endif
    pDRIInfo->frameBufferSize = pVia->videoRambytes;

    pDRIInfo->frameBufferStride = (pScrn->displayWidth *
                                   pScrn->bitsPerPixel / 8);
    pDRIInfo->ddxDrawableTableEntry = VIA_MAX_DRAWABLES;

    if (SAREA_MAX_DRAWABLES < VIA_MAX_DRAWABLES)
        pDRIInfo->maxDrawableTableEntry = SAREA_MAX_DRAWABLES;
    else
        pDRIInfo->maxDrawableTableEntry = VIA_MAX_DRAWABLES;

#ifdef NOT_DONE
    /* FIXME: need to extend DRI protocol to pass this size back to client
     * for SAREA mapping that includes a device private record. */
    pDRIInfo->SAREASize = ((sizeof(XF86DRISAREARec) + 0xfff) & 0x1000); /* round to page */
    /* + shared memory device private rec */
#else
    /* For now the mapping works by using a fixed size defined
     * in the SAREA header. */
    if (sizeof(XF86DRISAREARec) + sizeof(drm_via_sarea_t) > SAREA_MAX) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Data does not fit in SAREA\n");
        DRIDestroyInfoRec(pVia->pDRIInfo);
        pVia->pDRIInfo = NULL;
        return FALSE;
    }
    pDRIInfo->SAREASize = SAREA_MAX;
#endif

    if (!(pVIADRI = (VIADRIPtr) calloc(sizeof(VIADRIRec), 1))) {
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

    if (!DRIScreenInit(pScreen, pDRIInfo, &pVia->drmmode.fd)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[dri] DRIScreenInit failed.  Disabling DRI.\n");
        free(pDRIInfo->devPrivate);
        pDRIInfo->devPrivate = NULL;
        DRIDestroyInfoRec(pVia->pDRIInfo);
        pVia->pDRIInfo = NULL;
        pVia->drmmode.fd = -1;
        return FALSE;
    }

    if ((pVia->driSize > (pVia->maxDriSize * 1024)) &&
        (pVia->maxDriSize > 0)) {
        pVia->driSize = pVia->maxDriSize * 1024;
    } else {
        pVia->driSize = (pVia->FBFreeEnd - pVia->FBFreeStart) >> 2;
    }

    if (!(VIAInitVisualConfigs(pScreen))) {
        VIADRICloseScreen(pScreen);
        return FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] visual configs initialized.\n");

    /* DRIScreenInit doesn't add all the common mappings.
     * Add additional mappings here. */
    if (!VIADRIMapInit(pScreen, pVia)) {
        VIADRICloseScreen(pScreen);
        return FALSE;
    }
    pVIADRI->regs.size = VIA_MMIO_REGSIZE;
    pVIADRI->regs.handle = pVia->registerHandle;
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[drm] mmio Registers = 0x%08lx\n",
               (unsigned long)pVIADRI->regs.handle);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] mmio mapped.\n");
    return TRUE;
}

void
VIADRICloseScreen(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    VIADRIPtr pVIADRI;

    VIADRIRingBufferCleanup(pScrn);
    if (pVia->agpSize) {
        drmUnmap(pVia->agpMappedAddr, pVia->agpSize);
        drmRmMap(pVia->drmmode.fd, pVia->agpHandle);
        drmAgpUnbind(pVia->drmmode.fd, pVia->agpHandle);
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[drm] Freeing agp memory\n");
        drmAgpFree(pVia->drmmode.fd, pVia->agpHandle);
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[drm] Releasing agp module\n");
        drmAgpRelease(pVia->drmmode.fd);
    }

    DRICloseScreen(pScreen);
    drm_bo_free(pScrn, pVia->driOffScreenMem);

    if (pVia->pDRIInfo) {
        if ((pVIADRI = (VIADRIPtr) pVia->pDRIInfo->devPrivate)) {
            VIADRIIrqExit(pScrn, pVIADRI);
            free(pVIADRI);
            pVia->pDRIInfo->devPrivate = NULL;
        }
        DRIDestroyInfoRec(pVia->pDRIInfo);
        pVia->pDRIInfo = NULL;
    }

    if (pVia->pVisualConfigs) {
        free(pVia->pVisualConfigs);
        pVia->pVisualConfigs = NULL;
    }
    if (pVia->pVisualConfigsPriv) {
        free(pVia->pVisualConfigsPriv);
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
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    VIADRIPtr pVIADRI;

    pVia->pDRIInfo->driverSwapMethod = DRI_HIDE_X_CONTEXT;
    pVia->agpDMA = FALSE;

    pVIADRI = (VIADRIPtr) pVia->pDRIInfo->devPrivate;
    pVIADRI->drixinerama = FALSE;

    if (pVia->driSize < pVia->Bpl) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[drm] No DRM framebuffer heap available.\n"
                   "[drm] Please increase the frame buffer\n"
                   "[drm] memory area in the BIOS. Disabling DRI.\n");
        return FALSE;
    }
    if (pVia->driSize < 3 * (pScrn->virtualY * pVia->Bpl)) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "[drm] The DRM heap and pixmap cache memory may be too\n"
                   "[drm] small for optimal performance. Please increase\n"
                   "[drm] the frame buffer memory area in the BIOS.\n");
    }

    pVia->driOffScreenMem = drm_bo_alloc(pScrn, pVia->driSize, 16, TTM_PL_VRAM);

    DRIFinishScreenInit(pScreen);

    /* Set SAREA value. */
    {
        drm_via_sarea_t *saPriv;

        saPriv = (drm_via_sarea_t *) DRIGetSAREAPrivate(pScreen);
        assert(saPriv);
        memset(saPriv, 0, sizeof(*saPriv));
        saPriv->ctxOwner = -1;
    }
    pVIADRI->deviceID = pVia->Chipset;
    pVIADRI->width = pScrn->virtualX;
    pVIADRI->height = pScrn->virtualY;
    pVIADRI->mem = pScrn->videoRam * 1024;
    pVIADRI->bytesPerPixel = (pScrn->bitsPerPixel + 7) / 8;
    pVIADRI->sarea_priv_offset = sizeof(XF86DRISAREARec);
    /* TODO */
    pVIADRI->scrnX = pVIADRI->width;
    pVIADRI->scrnY = pVIADRI->height;

    /* Initialize IRQ. */
    if (pVia->DRIIrqEnable)
        VIADRIIrqInit(pScrn, pVIADRI);

    pVIADRI->ringBufActive = 0;
    VIADRIRingBufferInit(pScrn);
    pVia->agpDMA = pVia->dma2d && pVIADRI->ringBufActive;
    return TRUE;
}

static void
VIADRISwapContext(ScreenPtr pScreen, DRISyncType syncType,
                  DRIContextType oldContextType, void *oldContext,
                  DRIContextType newContextType, void *newContext)
{
#if 0
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
#endif
    return;
}

static void
VIADRIInitBuffers(WindowPtr pWin, RegionPtr prgn, CARD32 index)
{
#if 0
    ScreenPtr pScreen = pWin->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
#endif
    return;
}

static void
VIADRIMoveBuffers(WindowPtr pParent, DDXPointRec ptOldOrg,
                  RegionPtr prgnSrc, CARD32 index)
{
#if 0
    ScreenPtr pScreen = pParent->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
#endif
    return;
}

/* Initialize the kernel data structures. */
Bool
VIADRIKernelInit(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    drm_via_init_t drmInfo;

    pVia->IsPCI = !VIADRIAgpInit(pScrn);
    if (pVia->IsPCI)
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] Using PCI.\n");
    else
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] Using AGP.\n");

    if (!(VIADRIFBInit(pScrn))) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "[dri] Frame buffer initialization failed.\n");
        return FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] Frame buffer initialized.\n");

    memset(&drmInfo, 0, sizeof(drm_via_init_t));
    drmInfo.func = VIA_INIT_MAP;
    drmInfo.sarea_priv_offset = sizeof(XF86DRISAREARec);
    drmInfo.fb_offset = pVia->frameBufferHandle;
    drmInfo.mmio_offset = pVia->registerHandle;

    if (pVia->IsPCI) {
        drmInfo.agpAddr = 0;
    } else {
        /*For AMD64*/
#ifndef __x86_64__
	    drmInfo.agpAddr = (CARD32)pVia->agpAddr;
#else
	    drmInfo.agpAddr = (CARD64)pVia->agpAddr;
#endif
    }

    if ((drmCommandWrite(pVia->drmmode.fd, DRM_VIA_MAP_INIT, &drmInfo,
                         sizeof(drm_via_init_t))) < 0)
        return FALSE;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[dri] Kernel data initialized.\n");
    return TRUE;
}

/* Add a map for the MMIO registers. */
static Bool
VIADRIMapInit(ScreenPtr pScreen, VIAPtr pVia)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    int flags = DRM_READ_ONLY;

    if (drmAddMap(pVia->drmmode.fd, pVia->MmioBase, VIA_MMIO_REGSIZE,
                  DRM_REGISTERS, flags, &pVia->registerHandle) < 0) {
        return FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[drm] register handle = 0x%08lx\n",
               (unsigned long)pVia->registerHandle);
    if (drmAddMap(pVia->drmmode.fd, pVia->FrameBufferBase, pVia->videoRambytes,
                  DRM_FRAME_BUFFER, 0, &pVia->frameBufferHandle) < 0) {
        return FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "[drm] framebuffer handle = 0x%08lx\n",
               (unsigned long)pVia->frameBufferHandle);

    return TRUE;
}

#define DRM_VIA_BLIT_MAX_SIZE (2048*2048*4)

static int
viaDRIFBMemcpy(int fd, struct buffer_object *vram, unsigned char *addr,
               Bool toFB)
{
    unsigned long fbOffset = vram->offset, size = vram->size, curSize;
    drm_via_dmablit_t blit;
    int err;

    do {
        curSize = (size > DRM_VIA_BLIT_MAX_SIZE) ? DRM_VIA_BLIT_MAX_SIZE : size;

        blit.num_lines = 1;
        blit.line_length = curSize;
        blit.fb_addr = fbOffset;
        blit.fb_stride = ALIGN_TO(curSize, 16);
        blit.mem_addr = addr;
        blit.mem_stride = blit.fb_stride;
        blit.to_fb = (toFB) ? 1 : 0;

        do {
            err = drmCommandWriteRead(fd, DRM_VIA_DMA_BLIT,
                                      &blit, sizeof(blit));
        } while (-EAGAIN == err);
        if (err)
            return err;

        do {
            err = drmCommandWriteRead(fd, DRM_VIA_BLIT_SYNC,
                                      &blit.sync, sizeof(blit.sync));
        } while (-EAGAIN == err);
        if (err)
            return err;

        fbOffset += curSize;
        addr += curSize;
        size -= curSize;

    } while (size > 0);
    return 0;
}

void
viaDRIOffscreenSave(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    unsigned long srcSize = pVia->driOffScreenMem->size;
    int err;

    if (pVia->driOffScreenSave)
        free(pVia->driOffScreenSave);

    pVia->driOffScreenSave = malloc(srcSize + 16);
    if (pVia->driOffScreenSave) {
        void *dst, *src = drm_bo_map(pScrn, pVia->driOffScreenMem);

        dst = (void *) ALIGN_TO((unsigned long) pVia->driOffScreenSave, 16);
        if ((pVia->drmVerMajor == 2) && (pVia->drmVerMinor >= 8)) {
            err = viaDRIFBMemcpy(pVia->drmmode.fd, pVia->driOffScreenMem, dst, FALSE);
            if (!err)
                return;

            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                       "Hardware backup of DRI offscreen memory failed: %s.\n"
                       "\tUsing slow software backup instead.\n",
                       strerror(-err));
        }
        memcpy(dst, src, srcSize);
        drm_bo_unmap(pScrn, pVia->driOffScreenMem);
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Out of memory trying to backup DRI offscreen memory.\n");
    }
    return;
}

void
viaDRIOffscreenRestore(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    if (pVia->driOffScreenSave) {
        void *src, *dst = drm_bo_map(pScrn, pVia->driOffScreenMem);

        src = pVia->driOffScreenSave;
        memcpy(dst, src, pVia->driOffScreenMem->size);
        free(pVia->driOffScreenSave);
        pVia->driOffScreenSave = NULL;

        drm_bo_unmap(pScrn, pVia->driOffScreenMem);
    } else {
    }
}
