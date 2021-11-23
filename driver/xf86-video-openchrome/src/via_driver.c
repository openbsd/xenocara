/*
 * Copyright 2005-2015 The Openchrome Project
 *                     [https://www.freedesktop.org/wiki/Openchrome]
 * Copyright 2004-2006 Luc Verhaegen.
 * Copyright 2004-2005 The Unichrome Project  [unichrome.sf.net]
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

#include "shadow.h"

#include "globals.h"

#include "xf86.h"

#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#include "drm_fourcc.h"

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86RAC.h"
#endif
#include "xf86Crtc.h"

#ifdef OPENCHROMEDRI
#include "dri.h"
#else
#include "drm_fourcc.h"
#endif

/* RandR support */
#include "xf86RandR12.h"

#include "via_driver.h"


#ifdef OPENCHROMEDRI
static const ViaDRMVersion drmVIADRMExpected = { 1, 3, 0 };
static const ViaDRMVersion drmVIADRMCompat = { 3, 0, 0 };
static const ViaDRMVersion drmOpenChromeDRMVersion = { 3, 4, 0 };
#endif /* OPENCHROMEDRI */

/* Prototypes. */
static void VIAIdentify(int flags);

#ifdef XSERVER_LIBPCIACCESS
static Bool via_pci_probe(DriverPtr drv, int entity_num,
                          struct pci_device *dev, intptr_t match_data);
#else /* !XSERVER_LIBPCIACCESS */
static Bool VIAProbe(DriverPtr drv, int flags);
#endif

static Bool VIAPreInit(ScrnInfoPtr pScrn, int flags);
static Bool VIAScreenInit(SCREEN_INIT_ARGS_DECL);


int gVIAEntityIndex = -1;

#ifdef XSERVER_LIBPCIACCESS

#define VIA_DEVICE_MATCH(d,i) \
    { 0x1106, (d), PCI_MATCH_ANY, PCI_MATCH_ANY, 0, 0, (i) }

static const struct pci_id_match via_device_match[] = {
   VIA_DEVICE_MATCH (PCI_CHIP_VT3204, 0 ),
   VIA_DEVICE_MATCH (PCI_CHIP_VT3259, 0 ),
   VIA_DEVICE_MATCH (PCI_CHIP_CLE3122, 0 ),
   VIA_DEVICE_MATCH (PCI_CHIP_VT3205, 0 ),
   VIA_DEVICE_MATCH (PCI_CHIP_VT3314, 0 ),
   VIA_DEVICE_MATCH (PCI_CHIP_VT3336, 0 ),
   VIA_DEVICE_MATCH (PCI_CHIP_VT3364, 0 ),
   VIA_DEVICE_MATCH (PCI_CHIP_VT3324, 0 ),
   VIA_DEVICE_MATCH (PCI_CHIP_VT3327, 0 ),
   VIA_DEVICE_MATCH (PCI_CHIP_VT3353, 0 ),
   VIA_DEVICE_MATCH (PCI_CHIP_VT3409, 0 ),
   VIA_DEVICE_MATCH (PCI_CHIP_VT3410, 0 ),
    { 0, 0, 0 },
};

#endif /* XSERVER_LIBPCIACCESS */

_X_EXPORT DriverRec VIA = {
    VIA_DRIVER_VERSION,
    VIA_DRIVER_NAME,
    VIAIdentify,
#ifdef XSERVER_LIBPCIACCESS
    NULL,
#else
    VIAProbe,
#endif
    VIAAvailableOptions,
    NULL,
    0,
    NULL,
#ifdef XSERVER_LIBPCIACCESS
    via_device_match,
    via_pci_probe
#endif
};

/* Supported chipsets */
static SymTabRec VIAChipsets[] = {
    {VIA_CLE266,        "CLE266"},
    {VIA_KM400,         "KM400(A) / KN400(A) / P4M800"},
    {VIA_K8M800,        "K8M800 / K8N800"},
    {VIA_PM800,         "PM800 / PN800 / PM880 / CN333 / CN400"},
    {VIA_P4M800PRO,     "P4M800 Pro / VN800 / CN700"},
    {VIA_CX700,         "CX700 / VX700"},
    {VIA_P4M890,        "P4M890 / VN890 / CN800"},
    {VIA_K8M890,        "K8M890 / K8N890"},
    {VIA_P4M900,        "P4M900 / VN896 / CN896"},
    {VIA_VX800,         "VX800 / VX820"},
    {VIA_VX855,         "VX855 / VX875"},
    {VIA_VX900,         "VX900"},
    {-1,                NULL}
};

/* Mapping a PCI device ID to a chipset family identifier. */
static PciChipsets VIAPciChipsets[] = {
    {VIA_CLE266,        PCI_CHIP_CLE3122,   VIA_RES_SHARED},
    {VIA_KM400,         PCI_CHIP_VT3205,    VIA_RES_SHARED},
    {VIA_K8M800,        PCI_CHIP_VT3204,    VIA_RES_SHARED},
    {VIA_PM800,         PCI_CHIP_VT3259,    VIA_RES_SHARED},
    {VIA_P4M800PRO,     PCI_CHIP_VT3314,    VIA_RES_SHARED},
    {VIA_CX700,         PCI_CHIP_VT3324,    VIA_RES_SHARED},
    {VIA_P4M890,        PCI_CHIP_VT3327,    VIA_RES_SHARED},
    {VIA_K8M890,        PCI_CHIP_VT3336,    VIA_RES_SHARED},
    {VIA_P4M900,        PCI_CHIP_VT3364,    VIA_RES_SHARED},
    {VIA_VX800,         PCI_CHIP_VT3353,    VIA_RES_SHARED},
    {VIA_VX855,         PCI_CHIP_VT3409,    VIA_RES_SHARED},
    {VIA_VX900,         PCI_CHIP_VT3410,    VIA_RES_SHARED},
    {-1,                -1,                 VIA_RES_UNDEF}
};

static char strEXAOptionName[] = "MigrationHeuristic";
static char strEXAValue[] = "greedy";

#ifdef XFree86LOADER
static MODULESETUPPROTO(VIASetup);

static XF86ModuleVersionInfo VIAVersRec = {
    "openchrome",
    "https://www.freedesktop.org/wiki/Openchrome/",
    MODINFOSTRING1,
    MODINFOSTRING2,
#ifdef XORG_VERSION_CURRENT
    XORG_VERSION_CURRENT,
#else
    XF86_VERSION_CURRENT,
#endif
    PACKAGE_VERSION_MAJOR,
    PACKAGE_VERSION_MINOR,
    PACKAGE_VERSION_PATCHLEVEL,
    ABI_CLASS_VIDEODRV,
    ABI_VIDEODRV_VERSION,
    MOD_CLASS_VIDEODRV,
    {0, 0, 0, 0}
};

_X_EXPORT XF86ModuleData openchromeModuleData = { &VIAVersRec, VIASetup, NULL };

static pointer
VIASetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
    static Bool setupDone = FALSE;

    /* Only be loaded once */
    if (!setupDone) {
        setupDone = TRUE;
        xf86AddDriver(&VIA, module,
#ifdef XSERVER_LIBPCIACCESS
                     HaveDriverFuncs
#else
                     0
#endif
                     );

        return (pointer) 1;
    } else {
        if (errmaj)
            *errmaj = LDR_ONCEONLY;

        return NULL;
    }
} /* VIASetup */

#endif /* XFree86LOADER */

static Bool
VIASwitchMode(SWITCH_MODE_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);

    return xf86SetSingleMode(pScrn, mode, RR_Rotate_0);
}

static void
VIAAdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAAdjustFrame %dx%d\n", x, y));

    for (i = 0; i < xf86_config->num_crtc; i++) {
        xf86CrtcPtr crtc = xf86_config->crtc[i];

        xf86CrtcSetOrigin(crtc, x, y);
    }
}

static Bool
VIAEnterVT_internal(ScrnInfoPtr pScrn, int flags)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    int i;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    for (i = 0; i < xf86_config->num_crtc; i++) {
        xf86CrtcPtr crtc = xf86_config->crtc[i];

        if (crtc->funcs->save) {
            crtc->funcs->save(crtc);
        }
    }

    for (i = 0; i < xf86_config->num_output; i++) {
        xf86OutputPtr output = xf86_config->output[i];

        if (output->funcs->save) {
            output->funcs->save(output);
        }
    }

    if (!xf86SetDesiredModes(pScrn)) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Exiting %s.\n", __func__));
        return FALSE;
    }

    if (!flags) {
        /* Restore video status. */
        if ((!pVia->IsSecondary) && (!pVia->KMS)) {
            viaRestoreVideo(pScrn);
        }

#ifdef OPENCHROMEDRI
        if (pVia->directRenderingType == DRI_1) {
            kickVblank(pScrn);
            VIADRIRingBufferInit(pScrn);
            viaDRIOffscreenRestore(pScrn);
            DRIUnlock(xf86ScrnToScreen(pScrn));
        }
#endif
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
    return TRUE;
}

static Bool
VIAEnterVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    return VIAEnterVT_internal(pScrn, 0);
}

static void
VIALeaveVT_internal(ScrnInfoPtr pScrn, int flags)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    int i;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    if (!flags) {
#ifdef OPENCHROMEDRI
        if (pVia->directRenderingType == DRI_1) {
            volatile drm_via_sarea_t *saPriv = (drm_via_sarea_t *) DRIGetSAREAPrivate(pScrn->pScreen);

            DRILock(xf86ScrnToScreen(pScrn), 0);
            saPriv->ctxOwner = ~0;

            viaAccelSync(pScrn);

            VIADRIRingBufferCleanup(pScrn);
            viaDRIOffscreenSave(pScrn);

            if ((pVia->VQEnable) && (!pVia->KMS)) {
                viaDisableVQ(pScrn);
            }
        }
#endif

        /* Save video status and turn off all video activities. */
        if ((!pVia->IsSecondary) && (!pVia->KMS)){
            viaSaveVideo(pScrn);
        }
    }

    for (i = 0; i < xf86_config->num_output; i++) {
        xf86OutputPtr output = xf86_config->output[i];

        if (output->funcs->restore) {
            output->funcs->restore(output);
        }
    }

    for (i = 0; i < xf86_config->num_crtc; i++) {
        xf86CrtcPtr crtc = xf86_config->crtc[i];

        if (crtc->funcs->restore) {
            crtc->funcs->restore(crtc);
        }
    }

    pScrn->vtSema = FALSE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

static void
VIALeaveVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    VIALeaveVT_internal(pScrn, 0);
}

static void
VIAFreeRec(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIADisplayPtr pVIADisplay;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAFreeRec\n"));
    if (!pScrn->driverPrivate)
        return;

    pVIADisplay = pVia->pVIADisplay;

    if (pVIADisplay) {
        if (pVIADisplay->TVI2CDev)
            xf86DestroyI2CDevRec(pVIADisplay->TVI2CDev, TRUE);

        pVia->pVIADisplay = NULL;
        free(pVIADisplay);
    }

    if (pVia->VideoRegs)
        free(pVia->VideoRegs);

    free(pScrn->driverPrivate);
    pScrn->driverPrivate = NULL;
} /* VIAFreeRec */

/*
 * This only gets called when a screen is being deleted.  It does not
 * get called routinely at the end of a server generation.
 */
static void
VIAFreeScreen(FREE_SCREEN_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAFreeScreen\n"));

    /*
     * There is no guarantee that pVia
     * (same as pScrn->driverPrivate) is valid.
     * Hence, check to see if it is a null pointer.
     * If it is null, do nothing and exit.
     */
    if (!pVia) {
        goto exit;
    }

    if (xf86LoaderCheckSymbol("vgaHWFreeHWRec")) {
        vgaHWFreeHWRec(pScrn);
    }

    if (!pVia->KMS) {
        viaUMSDestroy(pScrn);
    }

    VIAFreeRec(pScrn);
exit:
    return;
}

static void
VIAIdentify(int flags)
{
    xf86PrintChipsets("OPENCHROME", "Driver for VIA Chrome chipsets",
                      VIAChipsets);
}

#ifdef XSERVER_LIBPCIACCESS
static Bool
via_pci_probe(DriverPtr driver, int entity_num,
              struct pci_device *device, intptr_t match_data)
{
    ScrnInfoPtr scrn = NULL;

    scrn = xf86ConfigPciEntity(scrn, 0, entity_num, VIAPciChipsets,
                               NULL, NULL, NULL, NULL, NULL);

    if (scrn != NULL) {
        scrn->driverVersion = VIA_DRIVER_VERSION;
        scrn->driverName = VIA_DRIVER_NAME;
        scrn->name = VIA_NAME;
        scrn->Probe = NULL;

        scrn->PreInit = VIAPreInit;
        scrn->ScreenInit = VIAScreenInit;
        scrn->SwitchMode = VIASwitchMode;
        scrn->AdjustFrame = VIAAdjustFrame;
        scrn->EnterVT = VIAEnterVT;
        scrn->LeaveVT = VIALeaveVT;
        scrn->FreeScreen = VIAFreeScreen;

        xf86Msg(X_NOTICE,
                "VIA Technologies does not support this driver in any way.\n");
        xf86Msg(X_NOTICE,
                "For support, please refer to"
                " https://www.freedesktop.org/wiki/Openchrome/.\n");
    }
    return scrn != NULL;
}
#else /* !XSERVER_LIBPCIACCESS */
static Bool
VIAProbe(DriverPtr drv, int flags)
{
    GDevPtr *devSections;
    int *usedChips;
    int numDevSections;
    int numUsed;
    Bool foundScreen = FALSE;
    int i;

    /* sanity checks */
    if ((numDevSections = xf86MatchDevice(DRIVER_NAME, &devSections)) <= 0)
        return FALSE;

    if (xf86GetPciVideoInfo() == NULL)
        return FALSE;

    numUsed = xf86MatchPciInstances(DRIVER_NAME,
                                    PCI_VIA_VENDOR_ID,
                                    VIAChipsets,
                                    VIAPciChipsets,
                                    devSections,
                                    numDevSections,
                                    drv,
                                    &usedChips);
    free(devSections);

    if (numUsed <= 0)
        return FALSE;

    xf86Msg(X_NOTICE,
            "VIA Technologies does not support this driver in any way.\n");
    xf86Msg(X_NOTICE, "For support, please refer to"
                      " https://www.freedesktop.org/wiki/Openchrome/.\n");

    if (flags & PROBE_DETECT) {
        foundScreen = TRUE;
    } else {
        for (i = 0; i < numUsed; i++) {
            ScrnInfoPtr pScrn = xf86AllocateScreen(drv, 0);
            EntityInfoPtr pEnt;

            if ((pScrn = xf86ConfigPciEntity(pScrn, 0, usedChips[i],
                                             VIAPciChipsets, 0, 0, 0, 0, 0))) {
                pScrn->driverVersion = VIA_DRIVER_VERSION;
                pScrn->driverName = VIA_DRIVER_NAME;
                pScrn->name = VIA_NAME;
                pScrn->Probe = VIAProbe;
                pScrn->PreInit = VIAPreInit;
                pScrn->ScreenInit = VIAScreenInit;
                pScrn->SwitchMode = VIASwitchMode;
                pScrn->AdjustFrame = VIAAdjustFrame;
                pScrn->EnterVT = VIAEnterVT;
                pScrn->LeaveVT = VIALeaveVT;
                pScrn->FreeScreen = VIAFreeScreen;
                foundScreen = TRUE;
            }
#if 0
            xf86ConfigActivePciEntity(pScrn,
                                      usedChips[i],
                                      VIAPciChipsets,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL);
#endif
            pEnt = xf86GetEntityInfo(usedChips[i]);

            /* CLE266 supports dual-head; mark the entity as sharable. */
            if (pEnt->chipset == VIA_CLE266 || pEnt->chipset == VIA_KM400) {
                static int instance = 0;
                DevUnion *pPriv;

                xf86SetEntitySharable(usedChips[i]);
                xf86SetEntityInstanceForScreen(pScrn,
                                               pScrn->entityList[0], instance);

                if (gVIAEntityIndex < 0) {
                    gVIAEntityIndex = xf86AllocateEntityPrivateIndex();
                    pPriv = xf86GetEntityPrivate(pScrn->entityList[0],
                                                 gVIAEntityIndex);

                    if (!pPriv->ptr) {
                        VIAEntPtr pVIAEnt;

                        pPriv->ptr = xnfcalloc(sizeof(VIAEntRec), 1);
                        pVIAEnt = pPriv->ptr;
                        pVIAEnt->IsDRIEnabled = FALSE;
                        pVIAEnt->BypassSecondary = FALSE;
                        pVIAEnt->HasSecondary = FALSE;
                        pVIAEnt->IsSecondaryRestored = FALSE;
                    }
                }
                instance++;
            }
            free(pEnt);
        }
    }

    free(usedChips);

    return foundScreen;

} /* VIAProbe */
#endif /* !XSERVER_LIBPCIACCESS */

static int
LookupChipSet(PciChipsets *pset, int chipSet)
{
    while (pset->numChipset >= 0) {
        if (pset->numChipset == chipSet)
            return pset->PCIid;
        pset++;
    }
    return -1;
}


static int
LookupChipID(PciChipsets *pset, int ChipID)
{
    /* Is there a function to do this for me? */
    while (pset->numChipset >= 0) {
        if (pset->PCIid == ChipID)
            return pset->numChipset;

        pset++;
    }

    return -1;
}

static Bool
VIAGetRec(ScrnInfoPtr pScrn)
{
    Bool ret = FALSE;
    VIAPtr pVia;
    VIADisplayPtr pVIADisplay;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAGetRec\n"));

    if (pScrn->driverPrivate)
        return TRUE;

    /* allocate VIARec */
    pVia = (VIARec *) xnfcalloc(sizeof(VIARec), 1);
    if (pVia) {
        pVia->pVIADisplay = xnfcalloc(sizeof(VIADisplayRec), 1);
        pVIADisplay = pVia->pVIADisplay;

        if (pVIADisplay) {
            pVIADisplay->TVI2CDev = NULL;

            pVia->VideoRegs = (video_via_regs *) xnfcalloc(sizeof(video_via_regs), 1);
            if (!pVia->VideoRegs) {
                free(pVIADisplay);
                free(pVia);
            } else {
                pScrn->driverPrivate = pVia;
                ret = TRUE;
            }
        }
    }
    return ret;
} /* VIAGetRec */

static unsigned int
viaConvertDepthToBpp(int bpp, int depth)
{
    unsigned int format = DRM_FORMAT_XRGB8888;
    unsigned int bppSize;

    switch (bpp) {
    case 8:
        format = DRM_FORMAT_C8;
        break;
    case 16:
        if (depth == 15)
            format = DRM_FORMAT_XRGB1555;
        else
            format = DRM_FORMAT_RGB565;
        break;
    case 24:
        format = DRM_FORMAT_RGB888;
        break;
    case 32:
        if (depth == 24)
            format = DRM_FORMAT_XRGB8888;
        else if (depth == 30)
            format = DRM_FORMAT_XRGB2101010;
        break;
    default:
        break;
    }

    switch (format) {
    case DRM_FORMAT_C8:
        bppSize = 1;
        break;
    case DRM_FORMAT_XRGB1555:
    case DRM_FORMAT_RGB565:
        bppSize = 2;
        break;
    case DRM_FORMAT_RGB888:
        bppSize = 3;
        break;
    case DRM_FORMAT_XRGB2101010:
    case DRM_FORMAT_XRGB8888:
        bppSize = 4;
        break;
    default:
        bppSize = 0;
        break;
    }

    return bppSize;
}

Bool
via_xf86crtc_resize(ScrnInfoPtr scrn, int width, int height)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    drmmode_crtc_private_ptr drmmode_crtc
                                    = xf86_config->crtc[0]->driver_private;
    drmmode_ptr drmmode = drmmode_crtc->drmmode;
    struct buffer_object *old_front = NULL;
    Bool ret;
    ScreenPtr screen;
    uint32_t old_fb_id;
    int i;
    int old_width, old_height, old_displayWidth;
    int cpp = (scrn->bitsPerPixel + 7) / 8;
    PixmapPtr ppix;
    void *new_pixels;
    VIAPtr pVia = VIAPTR(scrn);
    xf86CrtcPtr crtc = NULL;
    unsigned int bppSize, alignedPitch;

    DEBUG(xf86DrvMsg(scrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    screen = xf86ScrnToScreen(scrn);
    ppix = screen->GetScreenPixmap(screen);

    if ((scrn->virtualX == width) && (scrn->virtualY == height)) {
        DEBUG(xf86DrvMsg(scrn->scrnIndex, X_INFO,
                            "Exiting %s.\n", __func__));
        return TRUE;
    }

    /* Preserve the old screen information just in case it needs to
     * be restored. */
    old_width = scrn->virtualX;
    old_height = scrn->virtualY;
    old_displayWidth = scrn->displayWidth;
    old_fb_id = drmmode->fb_id;
    old_front = drmmode->front_bo;

    bppSize = viaConvertDepthToBpp(scrn->bitsPerPixel,
                                        scrn->depth);
    alignedPitch = width * bppSize;
    alignedPitch = ALIGN_TO(alignedPitch, 16);
    drmmode->front_bo = drm_bo_alloc(scrn,
                                        alignedPitch * height,
                                        16, TTM_PL_VRAM);
    if (!drmmode->front_bo) {
        goto fail;
    }
    scrn->virtualX = width;
    scrn->virtualY = height;
    scrn->displayWidth = width;

#ifdef OPENCHROMEDRI
    if (pVia->KMS) {
        ret = drmModeAddFB(drmmode->fd, width, height, scrn->depth,
                            scrn->bitsPerPixel, width * cpp,
                            drmmode->front_bo->handle,
                            &drmmode->fb_id);
        if (ret) {
            goto fail;
        }
    }
#endif

    new_pixels = drm_bo_map(scrn, drmmode->front_bo);
    if (!new_pixels) {
        goto fail;
    }

    if (pVia->shadowFB) {
        new_pixels = malloc(scrn->displayWidth * scrn->virtualY *
                            ((scrn->bitsPerPixel + 7) >> 3));
        if (!new_pixels) {
            goto fail;
        }

        free(pVia->ShadowPtr);
        pVia->ShadowPtr = new_pixels;
    }

    screen->ModifyPixmapHeader(ppix, width, height, -1, -1,
                                width * cpp, new_pixels);

    xf86DrvMsg(scrn->scrnIndex, X_INFO,
                "Allocated a new frame buffer: %dx%d\n",
                width, height);


#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,9,99,1,0)
    scrn->pixmapPrivate.ptr = ppix->devPrivate.ptr;
#endif


    for (i = 0; i < xf86_config->num_crtc; i++) {
        crtc = xf86_config->crtc[i];

        if (!xf86CrtcInUse(crtc)) {
            continue;
        }

        ret = xf86CrtcSetMode(crtc,
                                &crtc->desiredMode,
                                crtc->desiredRotation,
                                crtc->desiredX, crtc->desiredY);
        if (!ret) {
            xf86DrvMsg(scrn->scrnIndex, X_INFO,
                        "Mode setting failed.\n");
            goto fail;
        }
    }

#ifdef OPENCHROMEDRI
    if (pVia->KMS && old_fb_id) {
        drmModeRmFB(drmmode->fd, old_fb_id);
    }
#endif

    if (old_fb_id) {
        drm_bo_unmap(scrn, old_front);
        drm_bo_free(scrn, old_front);
    }

    xf86DrvMsg(scrn->scrnIndex, X_INFO,
                "Screen resize successful.\n");
    DEBUG(xf86DrvMsg(scrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
    return TRUE;

fail:
    if (drmmode->front_bo) {
        drm_bo_unmap(scrn, drmmode->front_bo);
        drm_bo_free(scrn, drmmode->front_bo);
    }

    scrn->virtualX = old_width;
    scrn->virtualY = old_height;
    scrn->displayWidth = old_displayWidth;
    drmmode->fb_id = old_fb_id;
    drmmode->front_bo = old_front;

    xf86DrvMsg(scrn->scrnIndex, X_ERROR,
                "An error occurred during screen resize.\n");
    DEBUG(xf86DrvMsg(scrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
    return FALSE;
}

/*
 * viaDrmOpen() probably should not be located here, but for now,
 * it will stay here.  Someday, it will move into a different file.
 */
#ifdef OPENCHROMEDRI
static Bool
viaDrmOpen(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    char *busId = NULL;
    drmVersionPtr drmVer;
    Bool status = FALSE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    if (xf86LoaderCheckSymbol("DRICreatePCIBusID")) {
        busId = DRICreatePCIBusID(pVia->PciInfo);
        if (!busId) {
            goto exit;
        }
    } else {
        busId = malloc(64);
        if (!busId) {
            goto exit;
        }

        sprintf(busId,
                "PCI:%d:%d:%d",
                pVia->PciInfo->bus,
#ifdef XSERVER_LIBPCIACCESS
                pVia->PciInfo->dev,
#else
                pVia->PciInfo->device,
#endif
                pVia->PciInfo->func);
    }

    /*
     * Look for OpenChrome DRM first.
     * KMS supports needs to be present for OpenChrome DRM to
     * function properly.
     */
    pVia->drmmode.fd = drmOpen(OPENCHROME_DRM_DRIVER_NAME, busId);
    if (pVia->drmmode.fd != -1) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "OpenChrome DRM detected.\n");
        drmVer = drmGetVersion(pVia->drmmode.fd);
        if (drmVer) {
            pVia->drmVerMajor = drmVer->version_major;
            pVia->drmVerMinor = drmVer->version_minor;
            pVia->drmVerPatchLevel = drmVer->version_patchlevel;
            drmFreeVersion(drmVer);
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "OpenChrome DRM Version: %d.%d.%d\n",
                        pVia->drmVerMajor, pVia->drmVerMinor,
                        pVia->drmVerPatchLevel);

            if ((pVia->drmVerMajor > drmOpenChromeDRMVersion.major) ||
                ((pVia->drmVerMajor == drmOpenChromeDRMVersion.major) &&
                (pVia->drmVerMinor >= drmOpenChromeDRMVersion.minor))) {
                if (!drmCheckModesettingSupported(busId)) {
                    pVia->KMS = TRUE;
                    pVia->directRenderingType = DRI_2;
                    pVia->NoAccel = TRUE;
                    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                "KMS is supported by "
                                "OpenChrome DRM.\n");
                } else {
                    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                                "KMS is not available.\n");
                    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                                "Disabling OpenChrome DRM support.\n");
                }
            } else {
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                            "Unsupported version of OpenChrome DRM "
                            "detected.\n");
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                            "Only OpenChrome DRM Version %d.%d or "
                            "later is supported.\n",
                            drmOpenChromeDRMVersion.major,
                            drmOpenChromeDRMVersion.minor);
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                            "Disabling OpenChrome DRM support.\n");
            }
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                        "Not able to obtain OpenChrome DRM version.\n");
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                        "Disabling OpenChrome DRM support.\n");
        }
    }

    free(busId);

    /*
     * Now, check for "legacy" DRI1 VIA DRM.
     */
    if (!pVia->KMS) {
        busId = DRICreatePCIBusID(pVia->PciInfo);
        pVia->drmmode.fd = drmOpen(VIA_DRM_DRIVER_NAME, busId);
        if (pVia->drmmode.fd != -1) {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "VIA DRM detected.\n");
            drmVer = drmGetVersion(pVia->drmmode.fd);
            if (drmVer) {
                pVia->drmVerMajor = drmVer->version_major;
                pVia->drmVerMinor = drmVer->version_minor;
                pVia->drmVerPatchLevel = drmVer->version_patchlevel;
                drmFreeVersion(drmVer);

                xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "VIA DRM Version: %d.%d.%d\n",
                            pVia->drmVerMajor, pVia->drmVerMinor,
                            pVia->drmVerPatchLevel);

                if (((pVia->drmVerMajor > drmVIADRMExpected.major) &&
                    (pVia->drmVerMajor < drmVIADRMCompat.major)) ||
                    ((pVia->drmVerMajor == drmVIADRMExpected.major) &&
                    (pVia->drmVerMinor >= drmVIADRMExpected.minor) &&
                    (pVia->drmVerMajor < drmVIADRMCompat.major))) {
                    pVia->directRenderingType = DRI_1;
                    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                "This version of VIA DRM is "
                                "compatible with OpenChrome DDX.\n");
                } else {
                    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                                "This version of VIA DRM is not "
                                "compatible with OpenChrome DDX.\n"
                                "OpenChrome DDX can work with "
                                "VIA DRM Version %d.%d to %d.%d.\n",
                                drmVIADRMExpected.major,
                                drmVIADRMExpected.minor,
                                drmVIADRMCompat.major,
                                drmVIADRMCompat.minor);
                    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                                "Disabling VIA DRM support.\n");
                }
            } else {
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                            "Not able to obtain VIA DRM version.\n");
            }
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "OpenChrome DDX will now operate "
                        "without DRM.\n");
        }

        free(busId);
    }

    status = TRUE;
exit:
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
    return status;
}
#endif /* OPENCHROMEDRI */

static Bool
VIAPreInit(ScrnInfoPtr pScrn, int flags)
{
    EntityInfoPtr pEnt;
    VIAPtr pVia;
    MessageType from = X_DEFAULT;
    rgb defaultWeight = {0, 0, 0};
    rgb defaultMask = {0, 0, 0};
    Gamma defaultGamma = {0.0, 0.0, 0.0};
    XF86OptionPtr option;
    Bool status = FALSE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    pScrn->monitor = pScrn->confScreen->monitor;

    /*
     * We support depths of 8, 16 and 24.
     * We support bpp of 8, 16, and 32.
     */
    if (!xf86SetDepthBpp(pScrn, 0, 0, 0, Support32bppFb)) {
        goto exit;
    } else {
        switch (pScrn->depth) {
        case 8:
        case 16:
        case 24:
            break;
        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                        "Given depth (%d) is not supported by this driver!\n",
                        pScrn->depth);
            goto exit;
            break;
        }
    }

    pScrn->rgbBits = 8;

    /* Print out the depth / bpp that was set. */
    xf86PrintDepthBpp(pScrn);

    if (pScrn->depth > 8) {
        if (!xf86SetWeight(pScrn, defaultWeight, defaultMask)) {
            goto exit;
        } else {
            /* TODO check weight returned is supported. */
        }
    }

    if (!xf86SetDefaultVisual(pScrn, -1)) {
        goto exit;
    } else {
        /* We don't currently support DirectColor at > 8bpp. */
        if (pScrn->depth > 8 && pScrn->defaultVisual != TrueColor) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                        "Given default visual (%s) is not supported "
                        "at depth %d.\n",
                        xf86GetVisualName(pScrn->defaultVisual),
                                            pScrn->depth);
            goto exit;
        }
    }

    /* If the driver supports gamma correction, set the gamma. */
    if (!xf86SetGamma(pScrn, defaultGamma)) {
        goto exit;
    }

    /* This driver uses a programmable clock. */
    pScrn->progClock = TRUE;

    if (pScrn->numEntities > 1) {
        goto exit;
    }

    if (!VIAGetRec(pScrn)) {
        goto exit;
    }

    pVia = VIAPTR(pScrn);
    pVia->IsSecondary = FALSE;
    pEnt = xf86GetEntityInfo(pScrn->entityList[0]);
#ifndef XSERVER_LIBPCIACCESS
    if (pEnt->resources) {
        free(pEnt);
        goto free_rec;
    }
#endif

    pVia->EntityIndex = pEnt->index;

    if (xf86IsEntityShared(pScrn->entityList[0])) {
        if (xf86IsPrimInitDone(pScrn->entityList[0])) {
            DevUnion *pPriv;
            VIAEntPtr pVIAEnt;
            VIAPtr pVia1;

            pVia->IsSecondary = TRUE;
            pPriv = xf86GetEntityPrivate(pScrn->entityList[0], gVIAEntityIndex);
            pVIAEnt = pPriv->ptr;
            if (pVIAEnt->BypassSecondary) {
                free(pEnt);
                goto free_rec;
            }

            pVIAEnt->pSecondaryScrn = pScrn;
            pVIAEnt->HasSecondary = TRUE;
            pVia1 = VIAPTR(pVIAEnt->pPrimaryScrn);
            pVia1->HasSecondary = TRUE;
            pVia->sharedData = pVia1->sharedData;
        } else {
            DevUnion *pPriv;
            VIAEntPtr pVIAEnt;

            xf86SetPrimInitDone(pScrn->entityList[0]);
            pPriv = xf86GetEntityPrivate(pScrn->entityList[0], gVIAEntityIndex);
            pVia->sharedData = xnfcalloc(sizeof(ViaSharedRec), 1);
            pVIAEnt = pPriv->ptr;
            pVIAEnt->pPrimaryScrn = pScrn;
            pVIAEnt->IsDRIEnabled = FALSE;
            pVIAEnt->BypassSecondary = FALSE;
            pVIAEnt->HasSecondary = FALSE;
            pVIAEnt->RestorePrimary = FALSE;
            pVIAEnt->IsSecondaryRestored = FALSE;
        }
    } else {
        pVia->sharedData = xnfcalloc(sizeof(ViaSharedRec), 1);
    }

    pVia->PciInfo = xf86GetPciInfoForEntity(pEnt->index);
#ifndef XSERVER_LIBPCIACCESS
    xf86RegisterResources(pEnt->index, NULL, ResNone);
#endif
    if (pEnt->device->chipset && *pEnt->device->chipset) {
        from = X_CONFIG;
        pScrn->chipset = pEnt->device->chipset;
        pVia->Chipset = xf86StringToToken(VIAChipsets, pScrn->chipset);
        pVia->ChipId = LookupChipSet(VIAPciChipsets, pVia->Chipset);
    } else if (pEnt->device->chipID >= 0) {
        from = X_CONFIG;
        pVia->ChipId = pEnt->device->chipID;
        pVia->Chipset = LookupChipID(VIAPciChipsets, pVia->ChipId);
        pScrn->chipset = (char *)xf86TokenToString(VIAChipsets, pVia->Chipset);
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipID override: 0x%04X\n",
                   pEnt->device->chipID);
    } else {
        from = X_PROBED;
        pVia->ChipId = DEVICE_ID(pVia->PciInfo);
        pVia->Chipset = LookupChipID(VIAPciChipsets, pVia->ChipId);
        pScrn->chipset = (char *)xf86TokenToString(VIAChipsets, pVia->Chipset);
    }

    xf86DrvMsg(pScrn->scrnIndex, from, "Chipset: %s\n", pScrn->chipset);

    if (pEnt->device->chipRev >= 0) {
        pVia->ChipRev = pEnt->device->chipRev;
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipRev override: %d\n",
                   pVia->ChipRev);
    } else {
        /* Read PCI bus 0, dev 0, function 0, index 0xF6 to get chip revision */
#ifdef XSERVER_LIBPCIACCESS
        struct pci_device *bridge = pci_device_get_parent_bridge(pVia->PciInfo);
        uint8_t rev = 0;

        pci_device_cfg_read_u8(bridge, &rev, 0xF6);
        pVia->ChipRev = rev;
#else
        pVia->ChipRev = pciReadByte(pciTag(0, 0, 0), 0xF6);
#endif
    }

    if (pEnt)
        free(pEnt);

    xf86DrvMsg(pScrn->scrnIndex, from, "Chipset revision: %d\n", pVia->ChipRev);

    pVia->directRenderingType = DRI_NONE;
    pVia->KMS = FALSE;
#ifdef OPENCHROMEDRI
    if (!viaDrmOpen(pScrn)) {
        goto free_rec;
    }
#endif /* OPENCHROMEDRI */

    option = xf86NewOption(strEXAOptionName, strEXAValue);
    xf86CollectOptions(pScrn, option);

    viaSetupDefaultOptions(pScrn);

    viaProcessOptions(pScrn);

    VIAVidHWDiffInit(pScrn);

    if (pVia->KMS) {
        if (!drmmode_pre_init(pScrn, &pVia->drmmode)) {
            goto free_rec;
        }
    } else {
        /*
         * After viaUMSPreInit() succeeds, MMIO PCI hardware resources
         * are memory mapped.  If there is an error from this point on,
         * they will need to be explicitly relinquished.
         */
        if (!viaUMSPreInit(pScrn)) {
            goto free_rec;
        }
    }

    if (!pScrn->modes) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
        goto fail;
    }

    /* Set the current mode to the first in the list */
    pScrn->currentMode = pScrn->modes;

    /* Set display resolution */
    xf86SetDpi(pScrn, 0, 0);

    if (xf86LoadSubModule(pScrn, "fb") == NULL) {
        goto fail;
    }

    if (!pVia->NoAccel) {
        XF86ModReqInfo req;
        int errmaj, errmin;

        memset(&req, 0, sizeof(req));
        req.majorversion = 2;
        req.minorversion = 0;
        if (!LoadSubModule(pScrn->module, "exa", NULL, NULL, NULL, &req,
                            &errmaj, &errmin)) {
            LoaderErrorMsg(NULL, "exa", errmaj, errmin);
            goto fail;
        }
    }

    if (pVia->shadowFB) {
        if (!xf86LoadSubModule(pScrn, "shadow")) {
            goto fail;
        }
    }

    /*
     * Set up screen parameters.
     */
    pVia->Bpl = pScrn->virtualX * (pScrn->bitsPerPixel / 8);

    if (!pVia->KMS) {
        viaUMSPreInitExit(pScrn);
    }

    status = TRUE;
    goto exit;
fail:
    if (!pVia->KMS) {
        viaUMSPreInitExit(pScrn);
    }

free_rec:
    VIAFreeRec(pScrn);
exit:
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
    return status;
}

static void
LoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices,
            LOCO * colors, VisualPtr pVisual)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    CARD16 lut_r[256], lut_g[256], lut_b[256];
    int i, j, k, index;

    for (k = 0; k < xf86_config->num_crtc; k++) {
        xf86CrtcPtr crtc = xf86_config->crtc[k];

        switch (pScrn->depth) {
        case 15:
            for (i = 0; i < numColors; i++) {
                index = indices[i];
                for (j = 0; j < 8; j++) {
                    lut_r[index * 8 + j] = colors[index].red << 8;
                    lut_g[index * 8 + j] = colors[index].green << 8;
                    lut_b[index * 8 + j] = colors[index].blue << 8;
                }
            }
            break;
        case 16:
            for (i = 0; i < numColors; i++) {
                index = indices[i];

                if (index <= 31) {
                    for (j = 0; j < 8; j++) {
                        lut_r[index * 8 + j] = colors[index].red << 8;
                        lut_b[index * 8 + j] = colors[index].blue << 8;
                    }
                }

                for (j = 0; j < 4; j++)
                    lut_g[index * 4 + j] = colors[index].green << 8;
            }
            break;
        default:
            for (i = 0; i < numColors; i++) {
                index = indices[i];
                lut_r[index] = colors[index].red << 8;
                lut_g[index] = colors[index].green << 8;
                lut_b[index] = colors[index].blue << 8;
            }
            break;
        }

        /* Make the change through RandR */
#ifdef RANDR_12_INTERFACE
        RRCrtcGammaSet(crtc->randr_crtc, lut_r, lut_g, lut_b);
#else /*RANDR_12_INTERFACE*/
        crtc->funcs->gamma_set(crtc, lut_r, lut_g, lut_b, 256);
#endif
    }
}

static void
viaUpdatePacked(ScreenPtr pScreen, shadowBufPtr pBuf)
{
    shadowUpdatePacked(pScreen, pBuf);
}

static void *
viaShadowWindow(ScreenPtr pScreen, CARD32 row, CARD32 offset, int mode,
                CARD32 *size, void *closure)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    int stride;

    stride = (pScrn->displayWidth * pScrn->bitsPerPixel) / 8;
    *size = stride;
    return ((uint8_t *) drm_bo_map(pScrn, pVia->drmmode.front_bo) + row * stride + offset);
}

static Bool
VIACreateScreenResources(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    PixmapPtr rootPixmap;
    void *surface;

    pScreen->CreateScreenResources = pVia->CreateScreenResources;
    if (!(*pScreen->CreateScreenResources)(pScreen))
        return FALSE;
    pScreen->CreateScreenResources = VIACreateScreenResources;

    rootPixmap = pScreen->GetScreenPixmap(pScreen);

#ifdef OPENCHROMEDRI
    if (pVia->KMS) {
        drmmode_uevent_init(pScrn, &pVia->drmmode);
    }
#endif

    surface = drm_bo_map(pScrn, pVia->drmmode.front_bo);
    if (!surface)
        return FALSE;

    if (pVia->shadowFB)
        surface = pVia->ShadowPtr;

    if (!pScreen->ModifyPixmapHeader(rootPixmap, -1, -1, -1, -1, -1,
                                        surface))
        return FALSE;

    if (pVia->shadowFB) {
        if (!shadowAdd(pScreen, rootPixmap, viaUpdatePacked,
                        viaShadowWindow, 0, NULL))
            return FALSE;
    }
    return TRUE;
}

static Bool
VIACloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);

    /*
     * Since there is now only one location for the allocated
     * hardware cursor storage, the code needs access to one of the
     * two CRTC structs for obtaining pointer address to the hardware
     * cursor storage.  In this case, IGA1's CRTC struct happens to
     * be selected, but IGA2's CRTC struct can very well supply the
     * same information.
     */
    xf86CrtcPtr crtc = xf86_config->crtc[0];
    drmmode_crtc_private_ptr iga = crtc->driver_private;

    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIACloseScreen\n"));

#ifdef OPENCHROMEDRI
    if (pVia->KMS) {
        drmmode_uevent_fini(pScrn, &pVia->drmmode);
    }
#endif /* OPENCHROMEDRI */

    if (pVia->directRenderingType != DRI_2)
        viaExitVideo(pScrn);

    if (!pVia->NoAccel) {
        viaExitAccel(pScreen);
    }

    if (pVia->ShadowPtr) {
        shadowRemove(pScreen, pScreen->GetScreenPixmap(pScreen));
        free(pVia->ShadowPtr);
        pVia->ShadowPtr = NULL;
    }

    /* Is the display currently visible? */
    if (pScrn->vtSema)
        VIALeaveVT(VT_FUNC_ARGS(0));

    xf86_cursors_fini(pScreen);

    if (pVia->drmmode.front_bo) {
#ifdef OPENCHROMEDRI
        if (pVia->KMS && pVia->drmmode.fb_id)
            drmModeRmFB(pVia->drmmode.fd, pVia->drmmode.fb_id);
#endif
        pVia->drmmode.fb_id = 0;

        drm_bo_unmap(pScrn, pVia->drmmode.front_bo);
        drm_bo_free(pScrn, pVia->drmmode.front_bo);
    }

    if (iga->cursor_bo)
        /*
         * Release hardware cursor storage.
         */
        drm_bo_free(pScrn, iga->cursor_bo);

#ifdef OPENCHROMEDRI
    if (pVia->directRenderingType == DRI_1)
        VIADRICloseScreen(pScreen);

    if (pVia->KMS) {
        if (drmDropMaster(pVia->drmmode.fd))
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                        "drmDropMaster failed: %s\n",
                        strerror(errno));
    }
#endif

    if (!pVia->KMS) {
        viaUMSDestroy(pScrn);
    }

    pScrn->vtSema = FALSE;
    pScreen->CloseScreen = pVia->CloseScreen;
    return (*pScreen->CloseScreen) (CLOSE_SCREEN_ARGS);
}

static Bool
VIAScreenInit(SCREEN_INIT_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    unsigned int bppSize, alignedPitch;
    unsigned long alignment;
    xf86CrtcConfigPtr xf86_config;
    struct buffer_object *bo;
    int cursorWidth, cursorHeight, flags;
    int cursorSize;
    int i;

    pScrn->displayWidth = pScrn->virtualX;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    pScrn->pScreen = pScreen;

    miClearVisualTypes();

    if (!miSetVisualTypes(pScrn->depth,
            (pScrn->bitsPerPixel > 8) && (!pVia->IsSecondary) ?
                TrueColorMask : miGetDefaultVisualMask(pScrn->depth),
            pScrn->rgbBits, pScrn->defaultVisual)) {
        return FALSE;
    }

    if (!miSetPixmapDepths()) {
        return FALSE;
    }

    if (pVia->shadowFB) {
        int pitch = BitmapBytePad(pScrn->bitsPerPixel * pScrn->virtualX);

        pVia->shadowFB = FALSE;
        pVia->ShadowPtr = malloc(pitch * pScrn->virtualY);
        if (pVia->ShadowPtr) {
            if (shadowSetup(pScreen))
                pVia->shadowFB = TRUE;
        }
    }

    if (!fbScreenInit(pScreen, NULL, pScrn->virtualX, pScrn->virtualY,
                        pScrn->xDpi, pScrn->yDpi, pScrn->displayWidth,
                        pScrn->bitsPerPixel))
        return FALSE;

    if (pScrn->bitsPerPixel > 8) {
        VisualPtr visual;

        visual = pScreen->visuals + pScreen->numVisuals;
        while (--visual >= pScreen->visuals) {
            if ((visual->class | DynamicClass) == DirectColor) {
                visual->offsetRed = pScrn->offset.red;
                visual->offsetGreen = pScrn->offset.green;
                visual->offsetBlue = pScrn->offset.blue;
                visual->redMask = pScrn->mask.red;
                visual->greenMask = pScrn->mask.green;
                visual->blueMask = pScrn->mask.blue;
            }
        }
    }

    /* Must be after RGB ordering is fixed. */
    fbPictureInit(pScreen, NULL, 0);

    if (!pVia->KMS) {
        if (!viaUMSMapIOResources(pScrn)) {
            return FALSE;
        }
    }

#ifdef OPENCHROMEDRI
    if (pVia->KMS) {
        if (drmSetMaster(pVia->drmmode.fd)) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                        "drmSetMaster failed: %s\n",
                        strerror(errno));
            return FALSE;
        }
    }

    if (pVia->drmmode.fd != -1) {
        if (pVia->directRenderingType == DRI_1) {
            /* DRI2 or DRI1 support */
            if (VIADRI1ScreenInit(pScreen))
                DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                    "DRI1 ScreenInit complete.\n"));
            else
                pVia->directRenderingType = DRI_NONE;
        }
    }
#endif

#ifdef OPENCHROMEDRI
    if (pVia->directRenderingType != DRI_2)
#endif /* OPENCHROMEDRI */
    {
        if (!viaUMSScreenInit(pScrn)) {
            return FALSE;
        }
    }

    if ((!pVia->NoAccel) &&
        ((pVia->directRenderingType == DRI_NONE)
#ifdef OPENCHROMEDRI
        || (pVia->directRenderingType == DRI_1)
#endif /* OPENCHROMEDRI */
        )) {
        if (!viaUMSAccelInit(pScrn)) {
            return FALSE;
        }
    }

    xf86SetBackingStore(pScreen);

    xf86SetSilkenMouse(pScreen);

    xf86SetBlackWhitePixels(pScreen);

    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    if (pVia->drmmode.hwcursor) {
        xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);

        cursorWidth = cursorHeight = 64;
        switch (pVia->Chipset) {
        case VIA_CLE266:
        case VIA_KM400:
            flags = 0;
            break;
        default:
            flags = HARDWARE_CURSOR_AND_SOURCE_WITH_MASK |
                    HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_64 |
                    HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
                    HARDWARE_CURSOR_ARGB;
            break;
        }

        cursorSize = (cursorWidth * cursorHeight) * (32 / 8);
        alignment = 1024;

        /*
         * Set cursor location in frame buffer.
         */
        bo = drm_bo_alloc(pScrn, cursorSize, alignment,
                            TTM_PL_VRAM);
        if (!bo) {
            return FALSE;
        }

        for (i = 0; i < xf86_config->num_crtc; i++) {
            xf86CrtcPtr crtc = xf86_config->crtc[i];
            drmmode_crtc_private_ptr iga = crtc->driver_private;

            /*
             * Set cursor location in frame buffer.
             */
            iga->cursor_bo = bo;
        }

        if (!xf86_cursors_init(pScreen,
                                cursorWidth, cursorHeight, flags)) {
            pVia->drmmode.hwcursor = FALSE;
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                        "Hardware cursor initialization failed.\n");
        }
    }

    bppSize = viaConvertDepthToBpp(pScrn->bitsPerPixel,
                                        pScrn->depth);
    alignedPitch = pScrn->virtualX * bppSize;
    alignedPitch = ALIGN_TO(alignedPitch, 16);
    pVia->drmmode.front_bo = drm_bo_alloc(pScrn,
                                            alignedPitch *
                                            pScrn->virtualY,
                                            16, TTM_PL_VRAM);
    if (!pVia->drmmode.front_bo)
        return FALSE;

    if (!drm_bo_map(pScrn, pVia->drmmode.front_bo))
        return FALSE;

    pScrn->vtSema = TRUE;
    pScreen->SaveScreen = xf86SaveScreen;
    pVia->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = VIACloseScreen;
    pVia->CreateScreenResources = pScreen->CreateScreenResources;
    pScreen->CreateScreenResources = VIACreateScreenResources;

    if (!xf86CrtcScreenInit(pScreen))
        return FALSE;

    if (!miCreateDefColormap(pScreen))
        return FALSE;

    if (!xf86HandleColormaps(pScreen, 256, 8, LoadPalette, NULL,
                             CMAP_RELOAD_ON_MODE_SWITCH
                             | CMAP_PALETTED_TRUECOLOR))
        return FALSE;

    xf86DPMSInit(pScreen, xf86DPMSSet, 0);

    if (!VIAEnterVT_internal(pScrn, 1))
        return FALSE;

    if (pVia->directRenderingType != DRI_2) {
#ifdef OPENCHROMEDRI
        if (pVia->directRenderingType == DRI_1) {
            if (!VIADRIFinishScreenInit(pScreen)) {
                xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Direct rendering disabled.\n");
                pVia->directRenderingType = DRI_NONE;
            } else
                xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Direct rendering enabled.\n");
        }
#endif
        if (!pVia->NoAccel)
            viaFinishInitAccel(pScreen);

        viaInitVideo(pScrn->pScreen);
    }

    if (serverGeneration == 1)
        xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
    return TRUE;
}
