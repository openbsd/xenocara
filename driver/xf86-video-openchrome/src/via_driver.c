/*
 * Copyright 2005-2007 The Openchrome Project  [openchrome.org]
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

#include "shadowfb.h"

#include "globals.h"
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#include "version.h"

#include "via_driver.h"
#include "via_video.h"
#include "via.h"

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6 
#include "xf86RAC.h"
#endif

#ifdef OPENCHROMEDRI
#include "dri.h"
#endif
#include "via_vgahw.h"
#include "via_id.h"

/* RandR support */
#include "xf86RandR12.h"

/* Prototypes. */
static void VIAIdentify(int flags);

#ifdef XSERVER_LIBPCIACCESS
struct pci_device *
via_pci_device(const struct pci_slot_match *bridge_match)
{
    struct pci_device_iterator *slot_iterator;
    struct pci_device *bridge;

    slot_iterator = pci_slot_match_iterator_create(bridge_match);
    bridge = pci_device_next(slot_iterator);
    pci_iterator_destroy(slot_iterator);
    return bridge;
}

struct pci_device *
via_host_bridge(void)
{
    static const struct pci_slot_match bridge_match = {
        0, 0, 0, 0, 0
    };
    return via_pci_device(&bridge_match);
}

struct pci_device *
viaPciDeviceVga(void)
{
    static const struct pci_slot_match bridge_match = {
        0, 0, 0, 3, 0
    };
    return via_pci_device(&bridge_match);
}

static Bool via_pci_probe(DriverPtr drv, int entity_num,
                          struct pci_device *dev, intptr_t match_data);
#else /* !XSERVER_LIBPCIACCESS */
static Bool VIAProbe(DriverPtr drv, int flags);
#endif

static Bool VIASetupDefaultOptions(ScrnInfoPtr pScrn);
static Bool VIAPreInit(ScrnInfoPtr pScrn, int flags);
static Bool VIAEnterVT(VT_FUNC_ARGS_DECL);
static void VIALeaveVT(VT_FUNC_ARGS_DECL);
static void VIASave(ScrnInfoPtr pScrn);
static void VIARestore(ScrnInfoPtr pScrn);
static Bool VIAWriteMode(ScrnInfoPtr pScrn, DisplayModePtr mode);
static Bool VIACloseScreen(CLOSE_SCREEN_ARGS_DECL);
static Bool VIASaveScreen(ScreenPtr pScreen, int mode);
static Bool VIAScreenInit(SCREEN_INIT_ARGS_DECL);
static int VIAInternalScreenInit(ScreenPtr pScreen);
static void VIAFreeScreen(FREE_SCREEN_ARGS_DECL);
static Bool VIASwitchMode(SWITCH_MODE_ARGS_DECL);
static void VIAAdjustFrame(ADJUST_FRAME_ARGS_DECL);
static void VIADPMS(ScrnInfoPtr pScrn, int mode, int flags);
static const OptionInfoRec *VIAAvailableOptions(int chipid, int busid);

static Bool VIAMapMMIO(ScrnInfoPtr pScrn);
static Bool VIAMapFB(ScrnInfoPtr pScrn);
static void VIAUnmapMem(ScrnInfoPtr pScrn);

static void VIALoadRgbLut(ScrnInfoPtr pScrn, int numColors, int *indices,
                          LOCO *colors, VisualPtr pVisual);
/* RandR */
static Bool VIADriverFunc(ScrnInfoPtr pScrnInfo, xorgDriverFuncOp op, pointer data);


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
    VIA_VERSION,
    DRIVER_NAME,
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
    {VIA_CLE266,   "CLE266"},
    {VIA_KM400,    "KM400/KN400"},
    {VIA_K8M800,   "K8M800/K8N800"},
    {VIA_PM800,    "PM800/PM880/CN400"},
    {VIA_VM800,    "VM800/P4M800Pro/VN800/CN700"},
    {VIA_CX700,    "CX700/VX700"},
    {VIA_K8M890,   "K8M890/K8N890"},
    {VIA_P4M890,   "P4M890"},
    {VIA_P4M900,   "P4M900/VN896/CN896"},
    {VIA_VX800,    "VX800/VX820"},
    {VIA_VX855,    "VX855/VX875"},
    {VIA_VX900,    "VX900"},
    {-1,            NULL }
};

/* Mapping a PCI device ID to a chipset family identifier. */
static PciChipsets VIAPciChipsets[] = {
    {VIA_CLE266,   PCI_CHIP_CLE3122,   VIA_RES_SHARED},
    {VIA_KM400,    PCI_CHIP_VT3205,    VIA_RES_SHARED},
    {VIA_K8M800,   PCI_CHIP_VT3204,    VIA_RES_SHARED},
    {VIA_PM800,    PCI_CHIP_VT3259,    VIA_RES_SHARED},
    {VIA_VM800,    PCI_CHIP_VT3314,    VIA_RES_SHARED},
    {VIA_CX700,    PCI_CHIP_VT3324,    VIA_RES_SHARED},
    {VIA_K8M890,   PCI_CHIP_VT3336,    VIA_RES_SHARED},
    {VIA_P4M890,   PCI_CHIP_VT3327,    VIA_RES_SHARED},
    {VIA_P4M900,   PCI_CHIP_VT3364,    VIA_RES_SHARED},
    {VIA_VX800,    PCI_CHIP_VT3353,    VIA_RES_SHARED},
    {VIA_VX855,    PCI_CHIP_VT3409,    VIA_RES_SHARED},
    {VIA_VX900,    PCI_CHIP_VT3410,    VIA_RES_SHARED},
    {-1,           -1,                 VIA_RES_UNDEF}
};

int gVIAEntityIndex = -1;

typedef enum
{
#ifdef HAVE_DEBUG
    OPTION_PRINTVGAREGS,
    OPTION_PRINTTVREGS,
    OPTION_I2CSCAN,
#endif
    OPTION_VBEMODES,
    OPTION_NOACCEL,
    OPTION_ACCELMETHOD,
    OPTION_EXA_NOCOMPOSITE,
    OPTION_EXA_SCRATCH_SIZE,
    OPTION_SWCURSOR,
    OPTION_SHADOW_FB,
    OPTION_ROTATION_TYPE,
    OPTION_ROTATE,
    OPTION_VIDEORAM,
    OPTION_ACTIVEDEVICE,
    OPTION_I2CDEVICES,
    OPTION_BUSWIDTH,
    OPTION_CENTER,
    OPTION_PANELSIZE,
    OPTION_FORCEPANEL,
    OPTION_TVDOTCRAWL,
    OPTION_TVTYPE,
    OPTION_TVOUTPUT,
    OPTION_TVDIPORT,
    OPTION_DISABLEVQ,
    OPTION_DISABLEIRQ,
    OPTION_TVDEFLICKER,
    OPTION_AGP_DMA,
    OPTION_2D_DMA,
    OPTION_XV_DMA,
    OPTION_VBE_SAVERESTORE,
    OPTION_MAX_DRIMEM,
    OPTION_AGPMEM,
    OPTION_DISABLE_XV_BW_CHECK,
    OPTION_MODE_SWITCH_METHOD
} VIAOpts;


static OptionInfoRec VIAOptions[] = {
#ifdef HAVE_DEBUG /* Don't document these three. */
    {OPTION_PRINTVGAREGS,        "PrintVGARegs",     OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_PRINTTVREGS,         "PrintTVRegs",      OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_I2CSCAN,             "I2CScan",          OPTV_BOOLEAN, {0}, FALSE},
#endif
    {OPTION_VBEMODES,            "VBEModes",         OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_NOACCEL,             "NoAccel",          OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_ACCELMETHOD,         "AccelMethod",      OPTV_STRING,  {0}, FALSE},
    {OPTION_EXA_NOCOMPOSITE,     "ExaNoComposite",   OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_EXA_SCRATCH_SIZE,    "ExaScratchSize",   OPTV_INTEGER, {0}, FALSE},
    {OPTION_SWCURSOR,            "SWCursor",         OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_SHADOW_FB,           "ShadowFB",         OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_ROTATION_TYPE,       "RotationType",     OPTV_ANYSTR,  {0}, FALSE},
    {OPTION_ROTATE,              "Rotate",           OPTV_ANYSTR,  {0}, FALSE},
    {OPTION_VIDEORAM,            "VideoRAM",         OPTV_INTEGER, {0}, FALSE},
    {OPTION_ACTIVEDEVICE,        "ActiveDevice",     OPTV_ANYSTR,  {0}, FALSE},
    {OPTION_BUSWIDTH,            "BusWidth",         OPTV_ANYSTR,  {0}, FALSE},
    {OPTION_CENTER,              "Center",           OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_PANELSIZE,           "PanelSize",        OPTV_ANYSTR,  {0}, FALSE},
    /* Forcing use of panel is a last resort - don't document this one. */
    {OPTION_FORCEPANEL,          "ForcePanel",       OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_TVDOTCRAWL,          "TVDotCrawl",       OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_TVDEFLICKER,         "TVDeflicker",      OPTV_INTEGER, {0}, FALSE},
    {OPTION_TVTYPE,              "TVType",           OPTV_ANYSTR,  {0}, FALSE},
    {OPTION_TVOUTPUT,            "TVOutput",         OPTV_ANYSTR,  {0}, FALSE},
    {OPTION_TVDIPORT,            "TVPort",           OPTV_ANYSTR,  {0}, FALSE},    
    {OPTION_DISABLEVQ,           "DisableVQ",        OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_DISABLEIRQ,          "DisableIRQ",       OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_AGP_DMA,             "EnableAGPDMA",     OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_2D_DMA,              "NoAGPFor2D",       OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_XV_DMA,              "NoXVDMA",          OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_VBE_SAVERESTORE,     "VbeSaveRestore",   OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_DISABLE_XV_BW_CHECK, "DisableXvBWCheck", OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_MODE_SWITCH_METHOD,  "ModeSwitchMethod", OPTV_ANYSTR,  {0}, FALSE},
    {OPTION_MAX_DRIMEM,          "MaxDRIMem",        OPTV_INTEGER, {0}, FALSE},
    {OPTION_AGPMEM,              "AGPMem",           OPTV_INTEGER, {0}, FALSE},
    {OPTION_I2CDEVICES,          "I2CDevices",       OPTV_ANYSTR,  {0}, FALSE},
    {-1,                         NULL,               OPTV_NONE,    {0}, FALSE}
};


#ifdef XFree86LOADER
static MODULESETUPPROTO(VIASetup);

static XF86ModuleVersionInfo VIAVersRec = {
    "openchrome",
    "http://openchrome.org/",
    MODINFOSTRING1,
    MODINFOSTRING2,
#ifdef XORG_VERSION_CURRENT
    XORG_VERSION_CURRENT,
#else
    XF86_VERSION_CURRENT,
#endif
    VERSION_MAJOR, VERSION_MINOR, PATCHLEVEL,
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
VIAGetRec(ScrnInfoPtr pScrn)
{
    Bool ret;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAGetRec\n"));

    ret = FALSE;
    if (pScrn->driverPrivate)
        return TRUE;

    /* allocate VIARec */
    pScrn->driverPrivate = xnfcalloc(sizeof(VIARec), 1);
    VIAPtr pVia = ((VIARec *) (pScrn->driverPrivate));

    if (pVia) {

        pVia->pBIOSInfo = xnfcalloc(sizeof(VIABIOSInfoRec), 1);
        VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

        if (pBIOSInfo) {
            pBIOSInfo->scrnIndex = pScrn->scrnIndex;
            pBIOSInfo->TVI2CDev = NULL;

            pBIOSInfo->Panel =
                    (ViaPanelInfoPtr) xnfcalloc(sizeof(ViaPanelInfoRec), 1);
            if (pBIOSInfo->Panel) {
                pBIOSInfo->Panel->NativeModeIndex = VIA_PANEL_INVALID;
                pBIOSInfo->Panel->NativeMode =
                        (ViaPanelModePtr) xnfcalloc(sizeof(ViaPanelModeRec), 1);
                pBIOSInfo->Panel->CenteredMode =
                        (DisplayModePtr) xnfcalloc(sizeof(DisplayModeRec), 1);
                pBIOSInfo->Lvds =
                        (ViaLVDSInfoPtr) xnfcalloc(sizeof(ViaLVDSInfoRec), 1);
                pBIOSInfo->FirstCRTC =
                        (ViaCRTCInfoPtr) xnfcalloc(sizeof(ViaCRTCInfoRec), 1);
                pBIOSInfo->SecondCRTC =
                        (ViaCRTCInfoPtr) xnfcalloc(sizeof(ViaCRTCInfoRec), 1);
                pBIOSInfo->Simultaneous =
                        (ViaSimultaneousInfoPtr)
                        xnfcalloc(sizeof(ViaSimultaneousInfoRec), 1);
                ret = pBIOSInfo->Panel->NativeMode
                        && pBIOSInfo->Panel->CenteredMode && pBIOSInfo->Lvds
                        && pBIOSInfo->FirstCRTC && pBIOSInfo->SecondCRTC
                        && pBIOSInfo->Simultaneous;
            }
            pVia->VideoRegs =
                    (video_via_regs *) xnfcalloc(sizeof(video_via_regs), 1);
            if (!pVia->VideoRegs)
                ret = FALSE;
        }
    }

    return ret;

} /* VIAGetRec */


static void
VIAFreeRec(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAFreeRec\n"));
    if (!pScrn->driverPrivate)
        return;

    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    if (pBIOSInfo) {

        if (pBIOSInfo->Panel) {
            if (pBIOSInfo->Panel->NativeMode)
                free(pBIOSInfo->Panel->NativeMode);
            if (pBIOSInfo->Panel->CenteredMode)
                free(pBIOSInfo->Panel->CenteredMode);
            free(pBIOSInfo->Panel);
        }

        if (pBIOSInfo->FirstCRTC)
            free(pBIOSInfo->FirstCRTC);
        if (pBIOSInfo->SecondCRTC)
            free(pBIOSInfo->SecondCRTC);
        if (pBIOSInfo->Simultaneous)
            free(pBIOSInfo->Simultaneous);
        if (pBIOSInfo->Lvds)
            free(pBIOSInfo->Lvds);
    }

    if (VIAPTR(pScrn)->pVbe)
        vbeFree(VIAPTR(pScrn)->pVbe);

    if (pVia->VideoRegs)
        free(pVia->VideoRegs);

    if (((VIARec *) (pScrn->driverPrivate))->pBIOSInfo->TVI2CDev)
        xf86DestroyI2CDevRec((((VIARec *) (pScrn->driverPrivate))->pBIOSInfo->
                              TVI2CDev), TRUE);
    free(((VIARec *) (pScrn->driverPrivate))->pBIOSInfo);

    VIAUnmapMem(pScrn);

    free(pScrn->driverPrivate);
    pScrn->driverPrivate = NULL;
} /* VIAFreeRec */


static const OptionInfoRec *
VIAAvailableOptions(int chipid, int busid)
{
    return VIAOptions;
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
    EntityInfoPtr entity;

    scrn = xf86ConfigPciEntity(scrn, 0, entity_num, VIAPciChipsets,
                               NULL, NULL, NULL, NULL, NULL);

    if (scrn != NULL) {
        scrn->driverVersion = VIA_VERSION;
        scrn->driverName = DRIVER_NAME;
        scrn->name = "CHROME";
        scrn->Probe = NULL;

        entity = xf86GetEntityInfo(entity_num);

        scrn->PreInit = VIAPreInit;
        scrn->ScreenInit = VIAScreenInit;
        scrn->SwitchMode = VIASwitchMode;
        scrn->AdjustFrame = VIAAdjustFrame;
        scrn->EnterVT = VIAEnterVT;
        scrn->LeaveVT = VIALeaveVT;
        scrn->FreeScreen = VIAFreeScreen;
        scrn->ValidMode = ViaValidMode;

        xf86Msg(X_NOTICE,
                "VIA Technologies does not support this driver in any way.\n");
        xf86Msg(X_NOTICE,
                "For support, please refer to http://www.openchrome.org/.\n");
#ifdef BUILDCOMMENT
        xf86Msg(X_NOTICE, BUILDCOMMENT"\n");
#endif
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
    xf86Msg(X_NOTICE, "For support, please refer to http://openchrome.org/.\n");

#ifdef BUILDCOMMENT
    xf86Msg(X_NOTICE, BUILDCOMMENT"\n");
#endif

    if (flags & PROBE_DETECT) {
        foundScreen = TRUE;
    } else {
        for (i = 0; i < numUsed; i++) {
            ScrnInfoPtr pScrn = xf86AllocateScreen(drv, 0);
            EntityInfoPtr pEnt;

            if ((pScrn = xf86ConfigPciEntity(pScrn, 0, usedChips[i],
                                             VIAPciChipsets, 0, 0, 0, 0, 0))) {
                pScrn->driverVersion = VIA_VERSION;
                pScrn->driverName = DRIVER_NAME;
                pScrn->name = "CHROME";
                pScrn->Probe = VIAProbe;
                pScrn->PreInit = VIAPreInit;
                pScrn->ScreenInit = VIAScreenInit;
                pScrn->SwitchMode = VIASwitchMode;
                pScrn->AdjustFrame = VIAAdjustFrame;
                pScrn->EnterVT = VIAEnterVT;
                pScrn->LeaveVT = VIALeaveVT;
                pScrn->FreeScreen = VIAFreeScreen;
                pScrn->ValidMode = ViaValidMode;
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

#ifdef OPENCHROMEDRI
static void
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

    if (pVIADRI->irqEnabled) {
        hwp->writeCrtc(hwp, 0x11, hwp->readCrtc(hwp, 0x11) | 0x30);
    }
}
#endif

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

static void
VIAProbeDDC(ScrnInfoPtr pScrn, int index)
{
    vbeInfoPtr pVbe;

    if (xf86LoadSubModule(pScrn, "vbe")) {
        /* FIXME This line should be replaced with:

           pVbe = VBEExtendedInit(NULL, index, 0);

           for XF86 version > 4.2.99
        */
        pVbe = VBEInit(NULL, index);
        ConfiguredMonitor = vbeDoEDID(pVbe, NULL);
        vbeFree(pVbe);
    }
}

static Bool
VIASetupDefaultOptions(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIASetupDefaultOptions - Setting up default chipset options.\n"));

    pVia->shadowFB = FALSE;
    pVia->NoAccel = FALSE;
    pVia->noComposite = TRUE;
    pVia->useEXA = TRUE;
    pVia->exaScratchSize = VIA_SCRATCH_SIZE / 1024;
    pVia->hwcursor = TRUE;
    pVia->VQEnable = TRUE;
    pVia->DRIIrqEnable = TRUE;
    pVia->agpEnable = TRUE;
    pVia->dma2d = TRUE;
    pVia->dmaXV = TRUE;
    pVia->useVBEModes = FALSE;
    pVia->vbeSR = FALSE;
#ifdef HAVE_DEBUG
    pVia->disableXvBWCheck = FALSE;
#endif
    pVia->maxDriSize = 0;
    pVia->agpMem = AGP_SIZE / 1024;
    pVia->ActiveDevice = 0x00;
    pVia->I2CDevices = 0x00;
    pVia->VideoEngine = VIDEO_ENGINE_CLE;
#ifdef HAVE_DEBUG
    pVia->PrintVGARegs = FALSE;
#endif

    /* Disable vertical interpolation because the size of */
    /* line buffer (limited to 800) is too small to do interpolation. */
    pVia->swov.maxWInterp = 800;
    pVia->swov.maxHInterp = 600;
    pVia->useLegacyVBE = TRUE;

    pVia->UseLegacyModeSwitch = FALSE;
    pBIOSInfo->TVDIPort = VIA_DI_PORT_DVP1; 

    switch (pVia->Chipset) {
        case VIA_CLE266:    
            pVia->UseLegacyModeSwitch = TRUE;
            pBIOSInfo->TVDIPort = VIA_DI_PORT_DVP0; 
        case VIA_KM400:
            /* IRQ is not broken on KM400A, but testing (pVia->ChipRev < 0x80)
             * is not enough to make sure we have an older, broken KM400. */
            pVia->DRIIrqEnable = FALSE;
            
            /* The KM400 not working properly with new mode switch (See Ticket #301) */
            pVia->UseLegacyModeSwitch = TRUE;
            pBIOSInfo->TVDIPort = VIA_DI_PORT_DVP0; 
            break;
        case VIA_K8M800:
            pVia->DRIIrqEnable = FALSE;
            break;
        case VIA_PM800:
            /* Use new mode switch to resolve many resolution and display bugs (switch to console) */
            /* FIXME The video playing (XV) is not working correctly after turn on new mode switch */
            pVia->VideoEngine = VIDEO_ENGINE_CME;
            break;
        case VIA_VM800:
            /* New mode switch resolve bug with gamma set #282 */
            /* and with Xv after hibernate #240                */
            break;
        case VIA_CX700:
            pVia->VideoEngine = VIDEO_ENGINE_CME;
            pVia->swov.maxWInterp = 1920;
            pVia->swov.maxHInterp = 1080;
            break;
        case VIA_K8M890:
            pVia->VideoEngine = VIDEO_ENGINE_CME;
            pVia->agpEnable = FALSE;
            pVia->dmaXV = FALSE;
            break;
        case VIA_P4M890:
            pVia->VideoEngine = VIDEO_ENGINE_CME;
            pVia->dmaXV = FALSE;
            break;
        case VIA_P4M900:
            pVia->VideoEngine = VIDEO_ENGINE_CME;
            pVia->agpEnable = FALSE;
            pVia->useLegacyVBE = FALSE;
            /* FIXME: this needs to be tested */
            pVia->dmaXV = FALSE;
            pBIOSInfo->TVDIPort = VIA_DI_PORT_DVP0; 
            break;

        case VIA_VX800:
        case VIA_VX855:
        case VIA_VX900:
            pVia->VideoEngine = VIDEO_ENGINE_CME;
            pVia->agpEnable = FALSE;
            pVia->dmaXV = FALSE;
            break;
    }

    return TRUE;
}


static Bool
VIAPreInit(ScrnInfoPtr pScrn, int flags)
{
    EntityInfoPtr pEnt;
    VIAPtr pVia;
    VIABIOSInfoPtr pBIOSInfo;
    MessageType from = X_DEFAULT;
    ClockRangePtr clockRanges;
    char *s = NULL;

#ifndef USE_FB
    char *mod = NULL;
#endif
    vgaHWPtr hwp;
    int i, bMemSize = 0;

#ifdef XSERVER_LIBPCIACCESS
    struct pci_device *bridge = via_host_bridge();
    uint8_t rev = 0 ;
#endif

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAPreInit\n"));

    if (pScrn->numEntities > 1)
        return FALSE;

    if (flags & PROBE_DETECT)
        return FALSE;

    if (!xf86LoadSubModule(pScrn, "vgahw"))
        return FALSE;

    if (!vgaHWGetHWRec(pScrn))
        return FALSE;

#if 0
    /* Here we can alter the number of registers saved and restored by the
     * standard vgaHWSave and Restore routines.
     */
    vgaHWSetRegCounts(pScrn, VGA_NUM_CRTC, VGA_NUM_SEQ, VGA_NUM_GFX,
                      VGA_NUM_ATTR);
#endif

    if (!VIAGetRec(pScrn)) {
        return FALSE;
    }

    pVia = VIAPTR(pScrn);
    pBIOSInfo = pVia->pBIOSInfo;

    pVia->IsSecondary = FALSE;
    pEnt = xf86GetEntityInfo(pScrn->entityList[0]);
#ifndef XSERVER_LIBPCIACCESS
    if (pEnt->resources) {
        free(pEnt);
        VIAFreeRec(pScrn);
        return FALSE;
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
                VIAFreeRec(pScrn);
                return FALSE;
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

    if (flags & PROBE_DETECT) {
        VIAProbeDDC(pScrn, pVia->EntityIndex);
        return TRUE;
    }

    pScrn->monitor = pScrn->confScreen->monitor;

    /*
     * We support depths of 8, 16 and 24.
     * We support bpp of 8, 16, and 32.
     */

    if (!xf86SetDepthBpp(pScrn, 0, 0, 0, Support32bppFb)) {
        free(pEnt);
        VIAFreeRec(pScrn);
        return FALSE;
    } else {
        switch (pScrn->depth) {
            case 8:
            case 16:
            case 24:
            case 32:
                /* OK */
                break;
            default:
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                           "Given depth (%d) is not supported by this driver\n",
                           pScrn->depth);
                free(pEnt);
                VIAFreeRec(pScrn);
                return FALSE;
        }
    }

    xf86PrintDepthBpp(pScrn);

    if (pScrn->depth == 32) {
        pScrn->depth = 24;
    }

    if (pScrn->depth > 8) {
        rgb zeros = { 0, 0, 0 };

        if (!xf86SetWeight(pScrn, zeros, zeros)) {
            free(pEnt);
            VIAFreeRec(pScrn);
            return FALSE;
        } else {
            /* TODO check weight returned is supported */
            ;
        }
    }

    if (!xf86SetDefaultVisual(pScrn, -1)) {
        return FALSE;
    } else {
        /* We don't currently support DirectColor at > 8bpp */
        if (pScrn->depth > 8 && pScrn->defaultVisual != TrueColor) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Given default visual"
                       " (%s) is not supported at depth %d.\n",
                       xf86GetVisualName(pScrn->defaultVisual), pScrn->depth);
            free(pEnt);
            VIAFreeRec(pScrn);
            return FALSE;
        }
    }

    /* We use a programmable clock */
    pScrn->progClock = TRUE;

    xf86CollectOptions(pScrn, NULL);

    /* Set the bits per RGB for 8bpp mode */
    if (pScrn->depth == 8)
        pScrn->rgbBits = 6;

    pVia->PciInfo = xf86GetPciInfoForEntity(pEnt->index);
#ifndef XSERVER_LIBPCIACCESS
    xf86RegisterResources(pEnt->index, NULL, ResNone);
#endif

#if 0
    xf86SetOperatingState(RES_SHARED_VGA, pEnt->index, ResUnusedOpr);
    xf86SetOperatingState(resVgaMemShared, pEnt->index, ResDisableOpr);
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
	pci_device_cfg_read_u8(bridge, &rev, 0xF6);
	pVia->ChipRev = rev ;
#else
        pVia->ChipRev = pciReadByte(pciTag(0, 0, 0), 0xF6);
#endif
    }

    if (pVia->Chipset == VIA_CLE266)
        ViaDoubleCheckCLE266Revision(pScrn);

    xf86DrvMsg(pScrn->scrnIndex, from, "Chipset revision: %d\n", pVia->ChipRev);

    free(pEnt);

    /* Detect the amount of installed RAM */
    from = X_PROBED;
    CARD8 videoRam;

#ifdef XSERVER_LIBPCIACCESS
    struct pci_device *vgaDevice = viaPciDeviceVga();
#endif

    hwp = VGAHWPTR(pScrn);

    switch (pVia->Chipset) {
        case VIA_CLE266:
#ifdef XSERVER_LIBPCIACCESS
            pci_device_cfg_read_u8(bridge, &videoRam, 0xE1);
#else
            videoRam = pciReadByte(pciTag(0, 0, 0), 0xE1) & 0x70;
#endif
            pScrn->videoRam = (1 << ((videoRam & 0x70) >> 4)) << 10;
        case VIA_KM400:
#ifdef XSERVER_LIBPCIACCESS
            pci_device_cfg_read_u8(bridge, &videoRam, 0xE1);
#else
            videoRam = pciReadByte(pciTag(0, 0, 0), 0xE1) & 0x70;
#endif
            pScrn->videoRam = (1 << ((videoRam & 0x70) >> 4)) << 10;
            /* Workaround for #177 (VRAM probing fail on P4M800) */
            if (pScrn->videoRam < 16384) {
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                           "Memory size detection failed: using 16 MB.\n");
                pScrn->videoRam = 16 << 10;
            }
            break;
        case VIA_PM800:
        case VIA_VM800:
        case VIA_K8M800:
#ifdef XSERVER_LIBPCIACCESS
            pci_device_cfg_read_u8(vgaDevice, &videoRam, 0xA1);
#else
            videoRam = pciReadByte(pciTag(0, 0, 3), 0xA1) & 0x70;
#endif
            pScrn->videoRam = (1 << ((videoRam & 0x70) >> 4)) << 10;
            break;
        case VIA_K8M890:
        case VIA_P4M890:
        case VIA_P4M900:
        case VIA_CX700:
        case VIA_VX800:
        case VIA_VX855:
        case VIA_VX900:
#ifdef XSERVER_LIBPCIACCESS
            pci_device_cfg_read_u8(vgaDevice, &videoRam, 0xA1);
#else
            videoRam = pciReadByte(pciTag(0, 0, 3), 0xA1) & 0x70;
#endif
            pScrn->videoRam = (1 << ((videoRam & 0x70) >> 4)) << 12;
            break;
        default:
            if (pScrn->videoRam < 16384 || pScrn->videoRam > 65536) {
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                           "Using old memory-detection method.\n");
                bMemSize = hwp->readSeq(hwp, 0x39);
                if (bMemSize > 16 && bMemSize <= 128)
                    pScrn->videoRam = (bMemSize + 1) << 9;
                else if (bMemSize > 0 && bMemSize < 31)
                    pScrn->videoRam = bMemSize << 12;
                else {
                    from = X_DEFAULT;
                    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                               "Memory size detection failed: using 16 MB.\n");
                    pScrn->videoRam = 16 << 10;
                }
            } else {
                from = X_DEFAULT;
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                           "No memory-detection done. Use VideoRAM option.\n");
            }
    }

    /*
     * PCI BAR are limited to 256 MB.
     * This limitation will go away with TTM.
     */
    if (pScrn->videoRam > (256 << 10)) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "Cannot use more than 256 MB of VRAM.\n");
        pScrn->videoRam = (256 << 10);
    }

    if (from == X_PROBED) {
        xf86DrvMsg(pScrn->scrnIndex, from,
                   "Probed amount of VideoRAM = %d kB\n", pScrn->videoRam);
    }

    if (!VIASetupDefaultOptions(pScrn)) {
        VIAFreeRec(pScrn);
        return FALSE;
    }

    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, VIAOptions);

    if (xf86GetOptValInteger(VIAOptions, OPTION_VIDEORAM, &pScrn->videoRam))
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                   "Setting amount of VideoRAM to %d kB\n", pScrn->videoRam);

    if ((s = xf86GetOptValString(VIAOptions, OPTION_MODE_SWITCH_METHOD))) {
        if (!xf86NameCmp(s, "legacy")) {
            if (pVia->UseLegacyModeSwitch) {
                xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                           "Already using \"legacy\" as ModeSwitchMethod, "
                           "did not force anything.\n");
            }
            else {
                pVia->UseLegacyModeSwitch = TRUE;
                xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                           "Forced ModeSwitchMethod to \"legacy\".\n");
            }
        }
        else if (!xf86NameCmp(s, "new")) {
            if (pVia->UseLegacyModeSwitch) {
                pVia->UseLegacyModeSwitch = FALSE;
                xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                           "Forced ModeSwitchMethod to \"new\".\n");
            }
            else {
                xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                           "Already using \"new\" as ModeSwitchMethod, "
                           "did not force anything.\n");
            }
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "\"%s\" is not a valid"
                       "value for Option \"ModeSwitchMethod\".\n", s);
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "Valid options are \"legacy\" or \"new\".\n");
        }
    }

    /* When rotating, switch shadow framebuffer on and acceleration off. */
    if ((s = xf86GetOptValString(VIAOptions, OPTION_ROTATION_TYPE))) {
        if (!xf86NameCmp(s, "SWRandR")) {
            pVia->shadowFB = TRUE;
            pVia->NoAccel = TRUE;
            pVia->RandRRotation = TRUE;
            pVia->rotate = RR_Rotate_0;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Rotating screen "
                       "RandR enabled, acceleration disabled\n");
        } else if (!xf86NameCmp(s, "HWRandR")) {
            pVia->shadowFB = TRUE;
            pVia->NoAccel = TRUE;
            pVia->RandRRotation = TRUE;
            pVia->rotate = RR_Rotate_0;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Hardware accelerated "
                       "rotating screen is not implemented. Using SW RandR.\n");
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "\"%s\" is not a valid"
                       "value for Option \"RotationType\".\n", s);
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "Valid options are \"SWRandR\" and \"HWRandR\".\n");
        }
    }


    /* When rotating, switch shadow framebuffer on and acceleration off. */
    if ((s = xf86GetOptValString(VIAOptions, OPTION_ROTATE))) {
        if (!xf86NameCmp(s, "CW")) {
            pVia->shadowFB = TRUE;
            pVia->NoAccel = TRUE;
            pVia->RandRRotation = TRUE;
            pVia->rotate = RR_Rotate_270;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Rotating screen "
                       "clockwise -- acceleration is disabled.\n");
        } else if (!xf86NameCmp(s, "CCW")) {
            pVia->shadowFB = TRUE;
            pVia->NoAccel = TRUE;
            pVia->RandRRotation = TRUE;
            pVia->rotate = RR_Rotate_90;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Rotating screen "
                       "counterclockwise -- acceleration is disabled.\n");
        } else if (!xf86NameCmp(s, "UD")) {
            pVia->shadowFB = TRUE;
            pVia->NoAccel = TRUE;
            pVia->RandRRotation = TRUE;
            pVia->rotate = RR_Rotate_180;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Rotating screen "
                       "upside-down -- acceleration is disabled.\n");
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "\"%s\" is not a valid"
                       "value for Option \"Rotate\".\n", s);
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "Valid options are \"CW\", \"CCW\" or  \"UD\".\n");
        }
    }

    from = (xf86GetOptValBool(VIAOptions, OPTION_SHADOW_FB, &pVia->shadowFB)
            ? X_CONFIG : X_DEFAULT);
    xf86DrvMsg(pScrn->scrnIndex, from, "Shadow framebuffer is %s.\n",
               pVia->shadowFB ? "enabled" : "disabled");

    /* Use hardware acceleration, unless on shadow framebuffer. */
    from = (xf86GetOptValBool(VIAOptions, OPTION_NOACCEL, &pVia->NoAccel)
            ? X_CONFIG : X_DEFAULT);
    if (!pVia->NoAccel && pVia->shadowFB) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Acceleration is "
                   "not supported when using shadow framebuffer.\n");
        pVia->NoAccel = TRUE;
        from = X_DEFAULT;
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Hardware acceleration is %s.\n",
               !pVia->NoAccel ? "enabled" : "disabled");

    if (!pVia->NoAccel) {
        from = X_DEFAULT;
        if ((s = (char *)xf86GetOptValString(VIAOptions, OPTION_ACCELMETHOD))) {
            if (!xf86NameCmp(s, "EXA")) {
                from = X_CONFIG;
                pVia->useEXA = TRUE;
#ifdef USE_XAA
            } else if (!xf86NameCmp(s, "XAA")) {
                from = X_CONFIG;
                pVia->useEXA = FALSE;
#endif
            }
        }
        xf86DrvMsg(pScrn->scrnIndex, from,
                   "Using %s acceleration architecture.\n",
                   pVia->useEXA ? "EXA" : "XAA");

        //pVia->noComposite = FALSE;
        if (pVia->useEXA) {
            from = xf86GetOptValBool(VIAOptions, OPTION_EXA_NOCOMPOSITE,
                                     &pVia->noComposite) ? X_CONFIG : X_DEFAULT;
            xf86DrvMsg(pScrn->scrnIndex, from,
                       "EXA composite acceleration %s.\n",
                       !pVia->noComposite ? "enabled" : "disabled");

            //pVia->exaScratchSize = VIA_SCRATCH_SIZE / 1024;
            from = xf86GetOptValInteger(VIAOptions, OPTION_EXA_SCRATCH_SIZE,
                                        &pVia->exaScratchSize)
                    ? X_CONFIG : X_DEFAULT;
            xf86DrvMsg(pScrn->scrnIndex, from,
                       "EXA scratch area size is %d kB.\n",
                       pVia->exaScratchSize);
        }
    }

    /* Use a hardware cursor, unless on secondary or on shadow framebuffer. */
    from = X_DEFAULT;
    if (pVia->IsSecondary || pVia->shadowFB)
        pVia->hwcursor = FALSE;
    else if (xf86GetOptValBool(VIAOptions, OPTION_SWCURSOR,
                               &pVia->hwcursor)) {
        pVia->hwcursor = !pVia->hwcursor;
        from = X_CONFIG;
    }
    if (pVia->hwcursor)
        xf86DrvMsg(pScrn->scrnIndex, from, "Using hardware two-color "
                   "cursors and software full-color cursors.\n");
    else
        xf86DrvMsg(pScrn->scrnIndex, from, "Using software cursors.\n");

    //pVia->VQEnable = TRUE;
    from = xf86GetOptValBool(VIAOptions, OPTION_DISABLEVQ, &pVia->VQEnable)
            ? X_CONFIG : X_DEFAULT;
    if (from == X_CONFIG)
        pVia->VQEnable = !pVia->VQEnable;
    xf86DrvMsg(pScrn->scrnIndex, from,
               "GPU virtual command queue will be %s.\n",
               (pVia->VQEnable) ? "enabled" : "disabled");

    //pVia->DRIIrqEnable = TRUE;
    from = xf86GetOptValBool(VIAOptions, OPTION_DISABLEIRQ, &pVia->DRIIrqEnable)
            ? X_CONFIG : X_DEFAULT;
    if (from == X_CONFIG)
        pVia->DRIIrqEnable = !pVia->DRIIrqEnable;
    xf86DrvMsg(pScrn->scrnIndex, from,
               "DRI IRQ will be %s if DRI is enabled.\n",
               (pVia->DRIIrqEnable) ? "enabled" : "disabled");

    //pVia->agpEnable = FALSE;
    from = xf86GetOptValBool(VIAOptions, OPTION_AGP_DMA, &pVia->agpEnable)
            ? X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from,
               "AGP DMA will be %s if DRI is enabled.\n",
               (pVia->agpEnable) ? "enabled" : "disabled");

    //pVia->dma2d = TRUE;
    if (pVia->agpEnable) {
        from = xf86GetOptValBool(VIAOptions, OPTION_2D_DMA, &pVia->dma2d)
                ? X_CONFIG : X_DEFAULT;
        if (from == X_CONFIG)
            pVia->dma2d = !pVia->dma2d;
        xf86DrvMsg(pScrn->scrnIndex, from, "AGP DMA will %sbe used for "
                   "2D acceleration.\n", (pVia->dma2d) ? "" : "not ");
    }
    //pVia->dmaXV = TRUE;
    from = xf86GetOptValBool(VIAOptions, OPTION_XV_DMA, &pVia->dmaXV)
            ? X_CONFIG : X_DEFAULT;
    if (from == X_CONFIG)
        pVia->dmaXV = !pVia->dmaXV;
    xf86DrvMsg(pScrn->scrnIndex, from, "PCI DMA will %sbe used for XV "
               "image transfer if DRI is enabled.\n",
               (pVia->dmaXV) ? "" : "not ");

    //pVia->useVBEModes = FALSE;
    from = xf86GetOptValBool(VIAOptions, OPTION_VBEMODES, &pVia->useVBEModes)
            ? X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from, "Will %senable VBE modes.\n",
               (pVia->useVBEModes) ? "" : "not ");

    //pVia->vbeSR = FALSE;
    from = xf86GetOptValBool(VIAOptions, OPTION_VBE_SAVERESTORE, &pVia->vbeSR)
            ? X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from, "VBE VGA register save & restore "
               "will %sbe used\n\tif VBE modes are enabled.\n",
               (pVia->vbeSR) ? "" : "not ");

#ifdef HAVE_DEBUG
    //pVia->disableXvBWCheck = FALSE;
    from = xf86GetOptValBool(VIAOptions, OPTION_DISABLE_XV_BW_CHECK,
                             &pVia->disableXvBWCheck)
            ? X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from, "Xv Bandwidth check is %s.\n",
               pVia->disableXvBWCheck ? "disabled" : "enabled");
    if (pVia->disableXvBWCheck) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "You may get a \"snowy\" screen"
                   " when using the Xv overlay.\n");
    }
#endif

    //pVia->maxDriSize = 0;
    from = xf86GetOptValInteger(VIAOptions, OPTION_MAX_DRIMEM,
                                &pVia->maxDriSize)
            ? X_CONFIG : X_DEFAULT;
    if (pVia->maxDriSize > 0)
        xf86DrvMsg(pScrn->scrnIndex, from,
                   "Will impose a %d kB limit on video RAM reserved for DRI.\n",
                   pVia->maxDriSize);
    else
        xf86DrvMsg(pScrn->scrnIndex, from,
                   "Will not impose a limit on video RAM reserved for DRI.\n");

    //pVia->agpMem = AGP_SIZE / 1024;
    from = xf86GetOptValInteger(VIAOptions, OPTION_AGPMEM, &pVia->agpMem)
            ? X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from,
               "Will try to allocate %d kB of AGP memory.\n", pVia->agpMem);

    /* ActiveDevice Option for device selection */
    //pVia->ActiveDevice = 0x00;
    if ((s = xf86GetOptValString(VIAOptions, OPTION_ACTIVEDEVICE))) {
        if (strstr(s, "CRT"))
            pVia->ActiveDevice |= VIA_DEVICE_CRT;
        if (strstr(s, "LCD"))
            pVia->ActiveDevice |= VIA_DEVICE_LCD;
        if (strstr(s, "DFP"))  
            pVia->ActiveDevice |= VIA_DEVICE_DFP;
        if (strstr(s, "TV"))
            pVia->ActiveDevice |= VIA_DEVICE_TV;
    }

    /* Digital Output Bus Width Option */
    pBIOSInfo->BusWidth = VIA_DI_12BIT;
    from = X_DEFAULT;
    if ((s = xf86GetOptValString(VIAOptions, OPTION_BUSWIDTH))) {
        from = X_CONFIG;
        if (!xf86NameCmp(s, "12BIT")) {
            pBIOSInfo->BusWidth = VIA_DI_12BIT;
        } else if (!xf86NameCmp(s, "24BIT")) {
            pBIOSInfo->BusWidth = VIA_DI_24BIT;
        }
    }
    xf86DrvMsg(pScrn->scrnIndex, from,
               "Digital output bus width is %d bits.\n",
               (pBIOSInfo->BusWidth == VIA_DI_12BIT) ? 12 : 24);


    /* LCD Center/Expend Option */
    pBIOSInfo->Center = FALSE;
    from = xf86GetOptValBool(VIAOptions, OPTION_CENTER, &pBIOSInfo->Center)
            ? X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from, "DVI Center is %s.\n",
               pBIOSInfo->Center ? "enabled" : "disabled");

    /* Panel Size Option */
    if ((s = xf86GetOptValString(VIAOptions, OPTION_PANELSIZE))) {
        ViaPanelGetNativeModeFromOption(pScrn, s);
        if (pBIOSInfo->Panel->NativeModeIndex != VIA_PANEL_INVALID) {
            ViaPanelModePtr mode = pBIOSInfo->Panel->NativeMode;

            DEBUG(xf86DrvMsg
                  (pScrn->scrnIndex, X_CONFIG, "Panel mode index is %d\n",
                   pBIOSInfo->Panel->NativeModeIndex));
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                       "Selected Panel Size is %dx%d\n", mode->Width,
                       mode->Height);
        }
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT,
                   "Panel size is not selected from config file.\n");
    }

    /* Force the use of the Panel? */
    pBIOSInfo->ForcePanel = FALSE;
    from = xf86GetOptValBool(VIAOptions, OPTION_FORCEPANEL,
                             &pBIOSInfo->ForcePanel)
            ? X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from,
               "Panel will %sbe forced.\n",
               pBIOSInfo->ForcePanel ? "" : "not ");

    pBIOSInfo->TVDotCrawl = FALSE;
    from = xf86GetOptValBool(VIAOptions, OPTION_TVDOTCRAWL,
                             &pBIOSInfo->TVDotCrawl)
            ? X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from, "TV dotCrawl is %s.\n",
               pBIOSInfo->TVDotCrawl ? "enabled" : "disabled");

    /* TV Deflicker */
    pBIOSInfo->TVDeflicker = 0;
    from = xf86GetOptValInteger(VIAOptions, OPTION_TVDEFLICKER,
                                &pBIOSInfo->TVDeflicker)
            ? X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from, "TV deflicker is set to %d.\n",
               pBIOSInfo->TVDeflicker);

    pBIOSInfo->TVType = TVTYPE_NONE;
    if ((s = xf86GetOptValString(VIAOptions, OPTION_TVTYPE))) {
        if (!xf86NameCmp(s, "NTSC")) {
            pBIOSInfo->TVType = TVTYPE_NTSC;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "TV Type is NTSC.\n");
        } else if (!xf86NameCmp(s, "PAL")) {
            pBIOSInfo->TVType = TVTYPE_PAL;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "TV Type is PAL.\n");
        } else if (!xf86NameCmp(s, "480P")) {
            pBIOSInfo->TVType = TVTYPE_480P;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "TV Type is SDTV 480P.\n");
        } else if (!xf86NameCmp(s, "576P")) {
            pBIOSInfo->TVType = TVTYPE_576P;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "TV Type is SDTV 576P.\n");
        } else if (!xf86NameCmp(s, "720P")) {
            pBIOSInfo->TVType = TVTYPE_720P;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "TV Type is HDTV 720P.\n");
        } else if (!xf86NameCmp(s, "1080I")) {
            pBIOSInfo->TVType = TVTYPE_1080I;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "TV Type is HDTV 1080i.\n");
        }
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT, "No default TV type is set.\n");
    }

    /* TV output signal Option */
    pBIOSInfo->TVOutput = TVOUTPUT_NONE;
    if ((s = xf86GetOptValString(VIAOptions, OPTION_TVOUTPUT))) {
        if (!xf86NameCmp(s, "S-Video")) {
            pBIOSInfo->TVOutput = TVOUTPUT_SVIDEO;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                       "TV Output Signal is S-Video.\n");
        } else if (!xf86NameCmp(s, "Composite")) {
            pBIOSInfo->TVOutput = TVOUTPUT_COMPOSITE;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                       "TV Output Signal is Composite.\n");
        } else if (!xf86NameCmp(s, "SC")) {
            pBIOSInfo->TVOutput = TVOUTPUT_SC;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "TV Output Signal is SC.\n");
        } else if (!xf86NameCmp(s, "RGB")) {
            pBIOSInfo->TVOutput = TVOUTPUT_RGB;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                       "TV Output Signal is RGB.\n");
        } else if (!xf86NameCmp(s, "YCbCr")) {
            pBIOSInfo->TVOutput = TVOUTPUT_YCBCR;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                       "TV Output Signal is YCbCr.\n");
        }
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT,
                   "No default TV output signal type is set.\n");
    }

    /* TV DI Port */
    if ((s = xf86GetOptValString(VIAOptions, OPTION_TVDIPORT))) {
        if (!xf86NameCmp(s, "DVP0")) {
            pBIOSInfo->TVDIPort = VIA_DI_PORT_DVP0;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                       "TV Output Port is DVP0.\n");
        } else if (!xf86NameCmp(s, "DVP1")) {
            pBIOSInfo->TVDIPort = VIA_DI_PORT_DVP1;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                       "TV Output Port is DVP1.\n");
        } else if (!xf86NameCmp(s, "DFPHigh")) {
            pBIOSInfo->TVDIPort = VIA_DI_PORT_DFPHIGH;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                       "TV Output Port is DFPHigh.\n");
        } else if (!xf86NameCmp(s, "DFPLow")) {
            pBIOSInfo->TVDIPort = VIA_DI_PORT_DFPLOW;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                       "TV Output Port is DFPLow.\n");
        } 
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT,
                   "No default TV output port is set.\n");
    }

    VIAVidHWDiffInit(pScrn);

    /* maybe throw in some more sanity checks here */

#ifndef XSERVER_LIBPCIACCESS
    pVia->PciTag = pciTag(pVia->PciInfo->bus, pVia->PciInfo->device,
                          pVia->PciInfo->func);
#endif

    if (!VIAMapMMIO(pScrn)) {
        VIAFreeRec(pScrn);
        return FALSE;
    }

#ifdef HAVE_DEBUG
    //pVia->PrintVGARegs = FALSE;
    from = xf86GetOptValBool(VIAOptions, OPTION_PRINTVGAREGS,
                             &pVia->PrintVGARegs)
            ? X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from, "Will %sprint VGA registers.\n",
               pVia->PrintVGARegs ? "" : "not ");
    if (pVia->PrintVGARegs)
        ViaVgahwPrint(VGAHWPTR(pScrn)); /* Do this as early as possible */

    pVia->I2CScan = FALSE;
    from = xf86GetOptValBool(VIAOptions, OPTION_I2CSCAN, &pVia->I2CScan)
            ? X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from, "Will %sscan I2C buses.\n",
               pVia->I2CScan ? "" : "not ");
#endif /* HAVE_DEBUG */

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "...Finished parsing config file options.\n");

    ViaCheckCardId(pScrn);

    /* Read memory bandwidth from registers. */
    pVia->MemClk = hwp->readCrtc(hwp, 0x3D) >> 4;
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "Detected MemClk %d\n", pVia->MemClk));
    if (pVia->MemClk >= VIA_MEM_END) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "Unknown Memory clock: %d\n", pVia->MemClk);
        pVia->MemClk = VIA_MEM_END - 1;
    }
    pBIOSInfo->Bandwidth = ViaGetMemoryBandwidth(pScrn);

    if (pBIOSInfo->TVType == TVTYPE_NONE) {
        /* Use jumper to determine TV type. */
        if (hwp->readCrtc(hwp, 0x3B) & 0x02) {
            pBIOSInfo->TVType = TVTYPE_PAL;
            DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                             "Detected TV standard: PAL.\n"));
        } else {
            pBIOSInfo->TVType = TVTYPE_NTSC;
            DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                             "Detected TV standard: NTSC.\n"));
        }
    }

    /* Initialize the colormap */
    Gamma zeros = { 0.0, 0.0, 0.0 };
    if (!xf86SetGamma(pScrn, zeros)) {
        VIAFreeRec(pScrn);
        return FALSE;
    }

    /* Split the FB for SAMM. */
    /* FIXME: For now, split the FB into two equal sections.
     * This should be user-adjustable via a config option. */
    if (pVia->IsSecondary) {
        DevUnion *pPriv;
        VIAEntPtr pVIAEnt;
        VIAPtr pVia1;

        pPriv = xf86GetEntityPrivate(pScrn->entityList[0], gVIAEntityIndex);
        pVIAEnt = pPriv->ptr;
        pScrn->videoRam = pScrn->videoRam >> 1;
        pVIAEnt->pPrimaryScrn->videoRam = pScrn->videoRam;
        pVia1 = VIAPTR(pVIAEnt->pPrimaryScrn);
        pVia1->videoRambytes = pScrn->videoRam << 10;
        pVia->FrameBufferBase += (pScrn->videoRam << 10);
    }

    pVia->videoRambytes = pScrn->videoRam << 10;

    /* I2CDevices Option for I2C Initialization */
    //pVia->I2CDevices = 0x00;
    if ((s = xf86GetOptValString(VIAOptions, OPTION_I2CDEVICES))) {
        if (strstr(s, "Bus1"))
            pVia->I2CDevices |= VIA_I2C_BUS1;
        if (strstr(s, "Bus2"))
            pVia->I2CDevices |= VIA_I2C_BUS2;
        if (strstr(s, "Bus3"))  
            pVia->I2CDevices |= VIA_I2C_BUS3;
    }

    if (!xf86LoadSubModule(pScrn, "i2c")) {
        VIAFreeRec(pScrn);
        return FALSE;
    } else {
        ViaI2CInit(pScrn);
    }

    if (!xf86LoadSubModule(pScrn, "ddc")) {
        VIAFreeRec(pScrn);
        return FALSE;
    } else {

        if (pVia->pI2CBus1) {
            pVia->DDC1 = xf86DoEEDID(XF86_SCRN_ARG(pScrn), pVia->pI2CBus1, TRUE);
            if (pVia->DDC1) {
                xf86PrintEDID(pVia->DDC1);
                xf86SetDDCproperties(pScrn, pVia->DDC1);
                DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                    "DDC pI2CBus1 detected a %s\n", DIGITAL(pVia->DDC1->features.input_type) ?
                    "DFP" : "CRT"));
            }
        }
    }

    ViaOutputsDetect(pScrn);
    if (!ViaOutputsSelect(pScrn)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No outputs possible.\n");
        VIAFreeRec(pScrn);
        return FALSE;
    }

	/* Might not belong here temporary fix for bug fix */
    ViaPreInitCRTCConfig(pScrn);

    if (!pVia->UseLegacyModeSwitch) {
        if (pBIOSInfo->Panel->IsActive)
            ViaPanelPreInit(pScrn);
    }

    pVia->pVbe = NULL;
    if (pVia->useVBEModes) {
        /* VBE doesn't properly initialise int10 itself. */
        if (xf86LoadSubModule(pScrn, "int10")
            && xf86LoadSubModule(pScrn, "vbe")) {
            pVia->pVbe = VBEExtendedInit(NULL, pVia->EntityIndex,
                                         SET_BIOS_SCRATCH |
                                         RESTORE_BIOS_SCRATCH);
        }

        if (!pVia->pVbe)
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "VBE initialisation failed."
                       " Using builtin code to set modes.\n");
    }

    if (pVia->pVbe) {

        if (!ViaVbeModePreInit(pScrn)) {
            VIAFreeRec(pScrn);
            return FALSE;
        }

    } else {
        int max_pitch, max_height;
        /* Add own modes. */
        ViaModesAttach(pScrn, pScrn->monitor);

        /*
         * Set up ClockRanges, which describe what clock ranges are
         * available, and what sort of modes they can be used for.
         */
        clockRanges = xnfalloc(sizeof(ClockRange));
        clockRanges->next = NULL;
        clockRanges->minClock = 20000;
        clockRanges->maxClock = 230000;

        clockRanges->clockIndex = -1;
        clockRanges->interlaceAllowed = TRUE;
        clockRanges->doubleScanAllowed = FALSE;

        switch (pVia->Chipset) {
            case VIA_CLE266:
            case VIA_KM400:
            case VIA_K8M800:
            case VIA_PM800:
            case VIA_VM800:
                max_pitch = 3344;
                max_height = 2508;
            case VIA_CX700:
            case VIA_K8M890:
            case VIA_P4M890:
            case VIA_P4M900:
                max_pitch = 8192/(pScrn->bitsPerPixel >> 3)-1;
                max_height = max_pitch;
                break;
            default:
                max_pitch = 16384/(pScrn->bitsPerPixel >> 3)-1;
                max_height = max_pitch;        
        }

        /*
         * xf86ValidateModes will check that the mode HTotal and VTotal values
         * don't exceed the chipset's limit if pScrn->maxHValue and
         * pScrn->maxVValue are set.  Since our VIAValidMode() already takes
         * care of this, we don't worry about setting them here.
         *
         * CLE266A:
         *   Max Line Pitch: 4080, (FB corruption when higher, driver problem?)
         *   Max Height: 4096 (and beyond)
         *
         * CLE266A: primary AdjustFrame can use only 24 bits, so we are limited
         * to 12x11 bits; 4080x2048 (~2:1), 3344x2508 (4:3), or 2896x2896 (1:1).
         * TODO Test CLE266Cx, KM400, KM400A, K8M800, CN400 please.
         *
         * We should be able to limit the memory available for a mode to 32 MB,
         * but xf86ValidateModes (or miScanLineWidth) fails to catch this
         * properly (apertureSize).
         */

        /* Select valid modes from those available. */
        i = xf86ValidateModes(pScrn, 
			      pScrn->monitor->Modes,     /* List of modes available for the monitor */
                              pScrn->display->modes,     /* List of mode names that the screen is requesting */
                              clockRanges,               /* list of clock ranges */
                              NULL,     /* list of line pitches */
                              256,      /* minimum line pitch */
                              max_pitch,     /* maximum line pitch */
                              16 * 8,   /* pitch increment (in bits), we just want 16 bytes alignment */
                              128,      /* min virtual height */
                              max_height,     /* maximum virtual height */
                              pScrn->display->virtualX, /* virtual width */
                              pScrn->display->virtualY, /* virtual height */
                              pVia->videoRambytes,      /* apertureSize */
                              LOOKUP_BEST_REFRESH);     /* lookup mode flags */

        if (i == -1) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "xf86ValidateModes failure\n");
            VIAFreeRec(pScrn);
            return FALSE;
        }

        /* This function deletes modes in the modes field of the ScrnInfoRec that have been marked as invalid. */
        xf86PruneDriverModes(pScrn);

        if (i == 0 || pScrn->modes == NULL) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
            VIAFreeRec(pScrn);
            return FALSE;
        }
    }

    /* Set up screen parameters. */
    pVia->Bpp = pScrn->bitsPerPixel >> 3;
    pVia->Bpl = pScrn->displayWidth * pVia->Bpp;

    /* This function fills in the Crtc fields for all the modes in the modes field of the ScrnInfoRec. */
    xf86SetCrtcForModes(pScrn, INTERLACE_HALVE_V);

    /* Set the current mode to the first in the list */
    pScrn->currentMode = pScrn->modes;

    /* Print the list of modes being used */    
    xf86PrintModes(pScrn);

    /* Set display resolution */
    xf86SetDpi(pScrn, 0, 0);

#ifdef USE_FB
    if (xf86LoadSubModule(pScrn, "fb") == NULL) {
        VIAFreeRec(pScrn);
        return FALSE;
    }


#else
    /* Load bpp-specific modules. */
    switch (pScrn->bitsPerPixel) {
        case 8:
            mod = "cfb";
            break;
        case 16:
            mod = "cfb16";
            break;
        case 32:
            mod = "cfb32";
            break;
    }

    if (mod && xf86LoadSubModule(pScrn, mod) == NULL) {
        VIAFreeRec(pScrn);
        return FALSE;
    }

#endif

    if (!pVia->NoAccel) {
        if (pVia->useEXA) {
            XF86ModReqInfo req;
            int errmaj, errmin;

            memset(&req, 0, sizeof(req));
            req.majorversion = 2;
            req.minorversion = 0;
            if (!LoadSubModule(pScrn->module, "exa", NULL, NULL, NULL, &req,
                               &errmaj, &errmin)) {
                LoaderErrorMsg(NULL, "exa", errmaj, errmin);
                VIAFreeRec(pScrn);
                return FALSE;
            }
#ifdef USE_XAA
        } else {
            if (!xf86LoadSubModule(pScrn, "xaa")) {
                VIAFreeRec(pScrn);
                return FALSE;
            }
#endif
        }
    }

    if (pVia->hwcursor) {
        if (!xf86LoadSubModule(pScrn, "ramdac")) {
            VIAFreeRec(pScrn);
            return FALSE;
        }
    }

    if (pVia->shadowFB) {
        if (!xf86LoadSubModule(pScrn, "shadowfb")) {
            VIAFreeRec(pScrn);
            return FALSE;
        }
    }

    VIAUnmapMem(pScrn);

    return TRUE;
}


static Bool
VIAEnterVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    Bool ret;

    /* FIXME: Rebind AGP memory here. */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAEnterVT\n"));

    if (pVia->pVbe) {
        if (pVia->vbeSR)
            ViaVbeSaveRestore(pScrn, MODE_SAVE);
        else
            VIASave(pScrn);
        ret = ViaVbeSetMode(pScrn, pScrn->currentMode);
    } else {
        VIASave(pScrn);
        ret = VIAWriteMode(pScrn, pScrn->currentMode);
    }
    vgaHWUnlock(hwp);

    VIASaveScreen(pScrn->pScreen, SCREEN_SAVER_ON);

    /* A patch for APM suspend/resume, when HWCursor has garbage. */
    if (pVia->hwcursor)
        viaCursorRestore(pScrn);

    /* Restore video status. */
    if (!pVia->IsSecondary)
        viaRestoreVideo(pScrn);

#ifdef OPENCHROMEDRI
    if (pVia->directRenderingEnabled) {
        kickVblank(pScrn);
        VIADRIRingBufferInit(pScrn);
        viaDRIOffscreenRestore(pScrn);
    }
#endif

    if (pVia->NoAccel) {
        memset(pVia->FBBase, 0x00, pVia->Bpl * pScrn->virtualY);
    } else {
        viaAccelFillRect(pScrn, 0, 0, pScrn->displayWidth, pScrn->virtualY,
                         0x00000000);
        viaAccelSyncMarker(pScrn);
    }

#ifdef OPENCHROMEDRI
    if (pVia->directRenderingEnabled) {
        DRIUnlock(xf86ScrnToScreen(pScrn));
    }
#endif

    return ret;
}


static void
VIALeaveVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIALeaveVT\n"));

#ifdef OPENCHROMEDRI
    if (pVia->directRenderingEnabled) {
        volatile drm_via_sarea_t *saPriv = (drm_via_sarea_t *)
                DRIGetSAREAPrivate(pScrn->pScreen);

        DRILock(xf86ScrnToScreen(pScrn), 0);
        saPriv->ctxOwner = ~0;
    }
#endif

    viaAccelSync(pScrn);

    /* A soft reset helps to avoid a 3D hang on VT switch. */
    switch (pVia->Chipset) {
        case VIA_K8M890:
        case VIA_P4M900:
        case VIA_VX800:
        case VIA_VX855:
        case VIA_VX900:
            break;
        default:
            hwp->writeSeq(hwp, 0x1A, pVia->SavedReg.SR1A | 0x40);
            break;
    }

#ifdef OPENCHROMEDRI
    if (pVia->directRenderingEnabled) {
        VIADRIRingBufferCleanup(pScrn);
        viaDRIOffscreenSave(pScrn);
    }
#endif

    if (pVia->VQEnable)
        viaDisableVQ(pScrn);

    /* Save video status and turn off all video activities. */
    if (!pVia->IsSecondary)
        viaSaveVideo(pScrn);

    if (pVia->hwcursor)
        viaCursorStore(pScrn);

    if (pVia->pVbe && pVia->vbeSR)
        ViaVbeSaveRestore(pScrn, MODE_RESTORE);
    else
        VIARestore(pScrn);

    vgaHWLock(hwp);
}

static void
ViaGammaDisable(ScrnInfoPtr pScrn)
{

    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    switch (pVia->Chipset) {
        case VIA_CLE266:
        case VIA_KM400:
            ViaSeqMask(hwp, 0x16, 0x00, 0x80);
            break;
        default:
            ViaCrtcMask(hwp, 0x33, 0x00, 0x80);
            break;
    }

    /* Disable gamma on secondary */
    /* This is needed or the hardware will lockup */
    ViaSeqMask(hwp, 0x1A, 0x00, 0x01);
    ViaCrtcMask(hwp, 0x6A, 0x00, 0x02);
    switch (pVia->Chipset) {
        case VIA_CLE266:
        case VIA_KM400:
        case VIA_K8M800:
        case VIA_PM800:
            break;
        default:
            ViaCrtcMask(hwp, 0x6A, 0x00, 0x20);
            break;
    }

}

static void
VIASave(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;
    VIARegPtr Regs = &pVia->SavedReg;
    int i;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIASave\n"));

    if (pVia->IsSecondary) {
        DevUnion *pPriv;
        VIAEntPtr pVIAEnt;
        VIAPtr pVia1;
        vgaHWPtr hwp1;

        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Secondary\n"));

        pPriv = xf86GetEntityPrivate(pScrn->entityList[0], gVIAEntityIndex);
        pVIAEnt = pPriv->ptr;
        hwp1 = VGAHWPTR(pVIAEnt->pPrimaryScrn);
        pVia1 = VIAPTR(pVIAEnt->pPrimaryScrn);
        hwp->SavedReg = hwp1->SavedReg;
        pVia->SavedReg = pVia1->SavedReg;
    } else {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Primary\n"));

        vgaHWProtect(pScrn, TRUE);

        if (xf86IsPrimaryPci(pVia->PciInfo)) {
            vgaHWSave(pScrn, &hwp->SavedReg, VGA_SR_ALL);
            DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                  "Primary Adapter! saving VGA_SR_ALL !!\n"));
        } else {
            vgaHWSave(pScrn, &hwp->SavedReg, VGA_SR_MODE);
            DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                  "Non-Primary Adapter! saving VGA_SR_MODE only !!\n"));
        }
        /* Unlock and save extended registers. */
        hwp->writeSeq(hwp, 0x10, 0x01);

        Regs->SR14 = hwp->readSeq(hwp, 0x14);
        Regs->SR15 = hwp->readSeq(hwp, 0x15);
        Regs->SR16 = hwp->readSeq(hwp, 0x16);
        Regs->SR17 = hwp->readSeq(hwp, 0x17);
        Regs->SR18 = hwp->readSeq(hwp, 0x18);
        Regs->SR19 = hwp->readSeq(hwp, 0x19);
        /* PCI Bus Control */
        Regs->SR1A = hwp->readSeq(hwp, 0x1A);

        Regs->SR1B = hwp->readSeq(hwp, 0x1B);
        Regs->SR1C = hwp->readSeq(hwp, 0x1C);
        Regs->SR1D = hwp->readSeq(hwp, 0x1D);
        Regs->SR1E = hwp->readSeq(hwp, 0x1E);
        Regs->SR1F = hwp->readSeq(hwp, 0x1F);

        Regs->SR22 = hwp->readSeq(hwp, 0x22);
        Regs->SR23 = hwp->readSeq(hwp, 0x23);
        Regs->SR24 = hwp->readSeq(hwp, 0x24);
        Regs->SR25 = hwp->readSeq(hwp, 0x25);
        Regs->SR26 = hwp->readSeq(hwp, 0x26);
        Regs->SR27 = hwp->readSeq(hwp, 0x27);
        Regs->SR28 = hwp->readSeq(hwp, 0x28);
        Regs->SR29 = hwp->readSeq(hwp, 0x29);
        Regs->SR2A = hwp->readSeq(hwp, 0x2A);
        Regs->SR2B = hwp->readSeq(hwp, 0x2B);

        Regs->SR2E = hwp->readSeq(hwp, 0x2E);

        /*=* Save VCK, LCDCK and ECK  *=*/
        /* Primary Display (VCK) (description for Chipset >= K8M800): */
        Regs->SR44 = hwp->readSeq(hwp, 0x44);
        Regs->SR45 = hwp->readSeq(hwp, 0x45);
        Regs->SR46 = hwp->readSeq(hwp, 0x46);

        /* ECK Clock Synthesizer (description for Chipset >= K8M800): */
        Regs->SR47 = hwp->readSeq(hwp, 0x47);
        Regs->SR48 = hwp->readSeq(hwp, 0x48);
        Regs->SR49 = hwp->readSeq(hwp, 0x49);

        switch (pVia->Chipset) {
            case VIA_CLE266:
            case VIA_KM400:
                break;
            default:
 	        /* Secondary Display (LCDCK): */
                Regs->SR4A = hwp->readSeq(hwp, 0x4A);
                Regs->SR4B = hwp->readSeq(hwp, 0x4B);
                Regs->SR4C = hwp->readSeq(hwp, 0x4C);
                break;
        }

        /* Save Preemptive Arbiter Control Register */
        Regs->SR4D = hwp->readSeq(hwp, 0x4D);
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Crtc...\n"));

        Regs->CR13 = hwp->readCrtc(hwp, 0x13);

        Regs->CR32 = hwp->readCrtc(hwp, 0x32);
        Regs->CR33 = hwp->readCrtc(hwp, 0x33);

        Regs->CR35 = hwp->readCrtc(hwp, 0x35);
        Regs->CR36 = hwp->readCrtc(hwp, 0x36);



        /* Starting Address */
        /* Start Address High */
        Regs->CR0C = hwp->readCrtc(hwp, 0x0C);
        /* Start Address Low */
        Regs->CR0D = hwp->readCrtc(hwp, 0x0D);
        /* Starting Address Overflow Bits[28:24] */
        Regs->CR48 = hwp->readCrtc(hwp, 0x48);
        /* CR34 are fire bits. Must be written after CR0C CR0D CR48.  */
        /* Starting Address Overflow Bits[23:16] */
        Regs->CR34 = hwp->readCrtc(hwp, 0x34);


        Regs->CR49 = hwp->readCrtc(hwp, 0x49);

        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "TVSave...\n"));
        if (pBIOSInfo->TVI2CDev)
            ViaTVSave(pScrn);

        /* Save LCD control registers (from CR 0x50 to 0x93). */
        for (i = 0; i < 68; i++)
            Regs->CRTCRegs[i] = hwp->readCrtc(hwp, i + 0x50);

        if (pVia->Chipset != VIA_CLE266 && pVia->Chipset != VIA_KM400) {
            /* LVDS Channel 2 Function Select 0 / DVI Function Select */ 
            Regs->CR97 = hwp->readCrtc(hwp, 0x97);
            /* LVDS Channel 1 Function Select 0 */
            Regs->CR99 = hwp->readCrtc(hwp, 0x99);
            /* Digital Video Port 1 Function Select 0 */
            Regs->CR9B = hwp->readCrtc(hwp, 0x9B);
            /* Power Now Control 4 */
            Regs->CR9F = hwp->readCrtc(hwp, 0x9F);

            /* Horizontal Scaling Initial Value */
            Regs->CRA0 = hwp->readCrtc(hwp, 0xA0);
            /* Vertical Scaling Initial Value */
            Regs->CRA1 = hwp->readCrtc(hwp, 0xA1);
            /* Scaling Enable Bit */
            Regs->CRA2 = hwp->readCrtc(hwp, 0xA2);
        }

        /* Save TMDS status */
        switch (pVia->Chipset) {
            case VIA_CX700:
            case VIA_VX800:
            case VIA_VX855:
            case VIA_VX900:
                Regs->CRD2 = hwp->readCrtc(hwp, 0xD2);
                break;
        }
        
        vgaHWProtect(pScrn, FALSE);
    }
}

static void
VIARestore(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;
    VIARegPtr Regs = &pVia->SavedReg;
    int i;
    CARD8 tmp;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIARestore\n"));

    /* Secondary? */

    vgaHWProtect(pScrn, TRUE);

    /* Unlock extended registers. */
    hwp->writeSeq(hwp, 0x10, 0x01);

    /*=* CR6A, CR6B, CR6C must be reset before restoring
         standard vga regs, or system will hang. *=*/
    /*=* TODO Check is reset IGA2 channel before disable IGA2 channel
         is necessary or it may cause some line garbage. *=*/
    hwp->writeCrtc(hwp, 0x6A, 0x00);
    hwp->writeCrtc(hwp, 0x6B, 0x00);
    hwp->writeCrtc(hwp, 0x6C, 0x00);
    
    /* Gamma must be disabled before restoring palette */
    ViaGammaDisable(pScrn);

    if (pBIOSInfo->TVI2CDev)
        ViaTVRestore(pScrn);

    /* Restore the standard VGA registers. */
    if (xf86IsPrimaryPci(pVia->PciInfo))
        vgaHWRestore(pScrn, &hwp->SavedReg, VGA_SR_ALL);
    else
        vgaHWRestore(pScrn, &hwp->SavedReg, VGA_SR_MODE);

    /* Restore extended registers. */
    hwp->writeSeq(hwp, 0x14, Regs->SR14);
    hwp->writeSeq(hwp, 0x15, Regs->SR15);
    hwp->writeSeq(hwp, 0x16, Regs->SR16);
    hwp->writeSeq(hwp, 0x17, Regs->SR17);
    hwp->writeSeq(hwp, 0x18, Regs->SR18);
    hwp->writeSeq(hwp, 0x19, Regs->SR19);
    hwp->writeSeq(hwp, 0x1A, Regs->SR1A);
    hwp->writeSeq(hwp, 0x1B, Regs->SR1B);
    hwp->writeSeq(hwp, 0x1C, Regs->SR1C);
    hwp->writeSeq(hwp, 0x1D, Regs->SR1D);
    hwp->writeSeq(hwp, 0x1E, Regs->SR1E);
    hwp->writeSeq(hwp, 0x1F, Regs->SR1F);

    hwp->writeSeq(hwp, 0x22, Regs->SR22);
    hwp->writeSeq(hwp, 0x23, Regs->SR23);
    hwp->writeSeq(hwp, 0x24, Regs->SR24);
    hwp->writeSeq(hwp, 0x25, Regs->SR25);
    hwp->writeSeq(hwp, 0x26, Regs->SR26);
    hwp->writeSeq(hwp, 0x27, Regs->SR27);
    hwp->writeSeq(hwp, 0x28, Regs->SR28);
    hwp->writeSeq(hwp, 0x29, Regs->SR29);
    hwp->writeSeq(hwp, 0x2A, Regs->SR2A);
    hwp->writeSeq(hwp, 0x2B, Regs->SR2B);

    hwp->writeSeq(hwp, 0x2E, Regs->SR2E);
    
    /*=* restore VCK, LCDCK and ECK *=*/
    /* Primary Display (VCK): */
    hwp->writeSeq(hwp, 0x44, Regs->SR44);
    hwp->writeSeq(hwp, 0x45, Regs->SR45);
    hwp->writeSeq(hwp, 0x46, Regs->SR46);

    /* Reset VCK PLL */
    hwp->writeSeq(hwp, 0x40, hwp->readSeq(hwp, 0x40) | 0x02); /* Set SR40[1] to 1 */
    hwp->writeSeq(hwp, 0x40, hwp->readSeq(hwp, 0x40) & 0xFD); /* Set SR40[1] to 0 */

    /* ECK Clock Synthesizer: */
    hwp->writeSeq(hwp, 0x47, Regs->SR47);
    hwp->writeSeq(hwp, 0x48, Regs->SR48);
    hwp->writeSeq(hwp, 0x49, Regs->SR49);

    /* Reset ECK PLL */
    hwp->writeSeq(hwp, 0x40, hwp->readSeq(hwp, 0x40) | 0x01); /* Set SR40[0] to 1 */
    hwp->writeSeq(hwp, 0x40, hwp->readSeq(hwp, 0x40) & 0xFE); /* Set SR40[0] to 0 */

    switch (pVia->Chipset) {
        case VIA_CLE266:
        case VIA_KM400:
            break;
        default:
 	    /* Secondary Display (LCDCK): */
            hwp->writeSeq(hwp, 0x4A, Regs->SR4A);
            hwp->writeSeq(hwp, 0x4B, Regs->SR4B);
            hwp->writeSeq(hwp, 0x4C, Regs->SR4C);

            /* Reset LCK PLL */
            hwp->writeSeq(hwp, 0x40, hwp->readSeq(hwp, 0x40) | 0x04); /* Set SR40[2] to 1 */
            hwp->writeSeq(hwp, 0x40, hwp->readSeq(hwp, 0x40) & 0xFB); /* Set SR40[2] to 0 */
            break;
    }

    /* Restore Preemptive Arbiter Control Register
     * VX800 and VX855 should restore this register too,
     * but I don't do that for I don't want to affect any
     * chips now.
     */
    if (pVia->Chipset == VIA_VX900) {
        hwp->writeSeq(hwp, 0x4D, Regs->SR4D);
    }

    /* Reset dotclocks. */
    ViaSeqMask(hwp, 0x40, 0x06, 0x06);
    ViaSeqMask(hwp, 0x40, 0x00, 0x06);

    /* Integrated LVDS Mode Select */ 
    hwp->writeCrtc(hwp, 0x13, Regs->CR13);

    /*=* Restore CRTC controller extended regs: *=*/
    /* Mode Control */
    hwp->writeCrtc(hwp, 0x32, Regs->CR32);
    /* HSYNCH Adjuster */
    hwp->writeCrtc(hwp, 0x33, Regs->CR33);
    /* Extended Overflow */
    hwp->writeCrtc(hwp, 0x35, Regs->CR35);
    /*Power Management 3 (Monitor Control) */
    hwp->writeCrtc(hwp, 0x36, Regs->CR36);

    /* Starting Address */
    /* Start Address High */
    hwp->writeCrtc(hwp, 0x0C, Regs->CR0C);
    /* Start Address Low */
    hwp->writeCrtc(hwp, 0x0D, Regs->CR0D);
    /* Starting Address Overflow Bits[28:24] */
    hwp->writeCrtc(hwp, 0x48, Regs->CR48);
    /* CR34 are fire bits. Must be written after CR0C CR0D CR48.  */
    /* Starting Address Overflow Bits[23:16] */
    hwp->writeCrtc(hwp, 0x34, Regs->CR34);
    

    hwp->writeCrtc(hwp, 0x49, Regs->CR49);

    /* Restore LCD control registers. */
    for (i = 0; i < 68; i++)
        hwp->writeCrtc(hwp, i + 0x50, Regs->CRTCRegs[i]);

    if (pVia->Chipset != VIA_CLE266 && pVia->Chipset != VIA_KM400) {
        /* Scaling Initial values */
        hwp->writeCrtc(hwp, 0xA0, Regs->CRA0);
        hwp->writeCrtc(hwp, 0xA1, Regs->CRA1);
        hwp->writeCrtc(hwp, 0xA2, Regs->CRA2);

        /* LVDS Channels Functions Selection */
        hwp->writeCrtc(hwp, 0x97, Regs->CR97);
        hwp->writeCrtc(hwp, 0x99, Regs->CR99);
        hwp->writeCrtc(hwp, 0x9B, Regs->CR9B);
        hwp->writeCrtc(hwp, 0x9F, Regs->CR9F);

    }

    /* Restore TMDS status */
    switch (pVia->Chipset) {
        case VIA_CX700:
        case VIA_VX800:
        case VIA_VX855:
        case VIA_VX900:
            /* LVDS Control Register */
            hwp->writeCrtc(hwp, 0xD2, Regs->CRD2);
            break;
    }
    
    if (pBIOSInfo->Panel->IsActive)
        ViaLCDPower(pScrn, TRUE);

    ViaDisablePrimaryFIFO(pScrn);

    /* Reset clock. */
    tmp = hwp->readMiscOut(hwp);
    hwp->writeMiscOut(hwp, tmp);

    vgaHWProtect(pScrn, FALSE);
}

static void
ViaMMIOEnable(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    switch (pVia->Chipset) {
        case VIA_K8M890:
        case VIA_CX700:
        case VIA_P4M900:
        case VIA_VX800:
        case VIA_VX855:
        case VIA_VX900:
            ViaSeqMask(hwp, 0x1A, 0x08, 0x08);
            break;
        default:
            if (pVia->IsSecondary)
                ViaSeqMask(hwp, 0x1A, 0x38, 0x38);
            else
                ViaSeqMask(hwp, 0x1A, 0x68, 0x68);
            break;
    }
}

static void
ViaMMIODisable(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    switch (pVia->Chipset) {
        case VIA_K8M890:
        case VIA_CX700:
        case VIA_P4M900:
        case VIA_VX800:
        case VIA_VX855:
        case VIA_VX900:
            ViaSeqMask(hwp, 0x1A, 0x00, 0x08);
            break;
        default:
            ViaSeqMask(hwp, 0x1A, 0x00, 0x60);
            break;
    }
}

static Bool
VIAMapMMIO(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

#ifdef XSERVER_LIBPCIACCESS
    pVia->MmioBase = pVia->PciInfo->regions[1].base_addr;
    int err;
#else
    pVia->MmioBase = pVia->PciInfo->memBase[1];
#endif

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAMapMMIO\n"));

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
               "mapping MMIO @ 0x%lx with size 0x%x\n",
               pVia->MmioBase, VIA_MMIO_REGSIZE);

#ifdef XSERVER_LIBPCIACCESS
    err = pci_device_map_range(pVia->PciInfo,
                               pVia->MmioBase,
                               VIA_MMIO_REGSIZE,
                               PCI_DEV_MAP_FLAG_WRITABLE,
                               (void **)&pVia->MapBase);

    if (err) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Unable to map mmio BAR. %s (%d)\n", strerror(err), err);
        return FALSE;
    }
#else
    pVia->MapBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO, pVia->PciTag,
                                  pVia->MmioBase, VIA_MMIO_REGSIZE);
    if (!pVia->MapBase)
        return FALSE;
#endif

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
               "mapping BitBlt MMIO @ 0x%lx with size 0x%x\n",
               pVia->MmioBase + VIA_MMIO_BLTBASE, VIA_MMIO_BLTSIZE);

#ifdef XSERVER_LIBPCIACCESS
    err = pci_device_map_range(pVia->PciInfo,
                               pVia->MmioBase + VIA_MMIO_BLTBASE,
                               VIA_MMIO_BLTSIZE,
                               PCI_DEV_MAP_FLAG_WRITABLE,
                               (void **)&pVia->BltBase);

    if (err) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Unable to map blt BAR. %s (%d)\n", strerror(err), err);
        return FALSE;
    }
#else
    pVia->BltBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO, pVia->PciTag,
                                  pVia->MmioBase + VIA_MMIO_BLTBASE,
                                  VIA_MMIO_BLTSIZE);
    if (!pVia->BltBase)
        return FALSE;
#endif

    if (!pVia->MapBase || !pVia->BltBase) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "BitBlit could not be mapped.\n");
        return FALSE;
    }

    /* Memory mapped IO for video engine. */
    pVia->VidMapBase = pVia->MapBase + 0x200;
    /* Memory mapped IO for mpeg engine. */
    pVia->MpegMapBase = pVia->MapBase + 0xc00;

    /* Set up MMIO vgaHW. */
    {
        vgaHWPtr hwp = VGAHWPTR(pScrn);
        CARD8 val;

        vgaHWSetMmioFuncs(hwp, pVia->MapBase, 0x8000);

        val = hwp->readEnable(hwp);
        hwp->writeEnable(hwp, val | 0x01);

        val = hwp->readMiscOut(hwp);
        hwp->writeMiscOut(hwp, val | 0x01);

        /* Unlock extended IO space. */
        ViaSeqMask(hwp, 0x10, 0x01, 0x01);

        ViaMMIOEnable(pScrn);

        vgaHWSetMmioFuncs(hwp, pVia->MapBase, 0x8000);

        /* Unlock CRTC. */
        ViaCrtcMask(hwp, 0x47, 0x00, 0x01);

        vgaHWGetIOBase(hwp);
    }

    return TRUE;
}


static Bool
VIAMapFB(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

#ifdef XSERVER_LIBPCIACCESS
    if (pVia->Chipset == VIA_VX900) {
        pVia->FrameBufferBase = pVia->PciInfo->regions[2].base_addr;
    } else {
        pVia->FrameBufferBase = pVia->PciInfo->regions[0].base_addr;
    }
    int err;
#else
    if (pVia->Chipset == VIA_VX900) {
        pVia->FrameBufferBase = pVia->PciInfo->memBase[2];
    } else {
        pVia->FrameBufferBase = pVia->PciInfo->memBase[0];
    }
#endif

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAMapFB\n"));
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
               "mapping framebuffer @ 0x%lx with size 0x%lx\n",
               pVia->FrameBufferBase, pVia->videoRambytes);

    if (pVia->videoRambytes) {

#ifndef XSERVER_LIBPCIACCESS
        /*
         * FIXME: This is a hack to get rid of offending wrongly sized
         * MTRR regions set up by the VIA BIOS. Should be taken care of
         * in the OS support layer.
         */

        unsigned char *tmp;

        tmp = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO, pVia->PciTag,
                            pVia->FrameBufferBase, pVia->videoRambytes);
        xf86UnMapVidMem(pScrn->scrnIndex, (pointer) tmp, pVia->videoRambytes);

        /*
         * And, as if this wasn't enough, 2.6 series kernels don't
         * remove MTRR regions on the first attempt. So try again.
         */

        tmp = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO, pVia->PciTag,
                            pVia->FrameBufferBase, pVia->videoRambytes);
        xf86UnMapVidMem(pScrn->scrnIndex, (pointer) tmp, pVia->videoRambytes);

        /*
         * End of hack.
         */
#endif

#ifdef XSERVER_LIBPCIACCESS
        err = pci_device_map_range(pVia->PciInfo, pVia->FrameBufferBase,
                                   pVia->videoRambytes,
                                   (PCI_DEV_MAP_FLAG_WRITABLE |
                                    PCI_DEV_MAP_FLAG_WRITE_COMBINE),
                                   (void **)&pVia->FBBase);
        if (err) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Unable to map mmio BAR. %s (%d)\n", strerror(err), err);
            return FALSE;
        }
#else
        pVia->FBBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_FRAMEBUFFER,
                                     pVia->PciTag, pVia->FrameBufferBase,
                                     pVia->videoRambytes);

        if (!pVia->FBBase) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Internal error: could not map framebuffer\n");
            return FALSE;
        }
#endif

        pVia->FBFreeStart = (pScrn->displayWidth * pScrn->bitsPerPixel >> 3) *
                pScrn->virtualY;
        pVia->FBFreeEnd = pVia->videoRambytes;

        xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                   "Frame buffer start: %p, free start: 0x%x end: 0x%x\n",
                   pVia->FBBase, pVia->FBFreeStart, pVia->FBFreeEnd);
    }

#ifdef XSERVER_LIBPCIACCESS
    pScrn->memPhysBase = pVia->PciInfo->regions[0].base_addr;
#else
    pScrn->memPhysBase = pVia->PciInfo->memBase[0];
#endif
    pScrn->fbOffset = 0;
    if (pVia->IsSecondary)
        pScrn->fbOffset = pScrn->videoRam << 10;

    return TRUE;
}


static void
VIAUnmapMem(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAUnmapMem\n"));

    ViaMMIODisable(pScrn);

#ifdef XSERVER_LIBPCIACCESS
    if (pVia->MapBase)
        pci_device_unmap_range(pVia->PciInfo, (pointer) pVia->MapBase,
                               VIA_MMIO_REGSIZE);

    if (pVia->BltBase)
        pci_device_unmap_range(pVia->PciInfo, (pointer) pVia->BltBase,
                               VIA_MMIO_BLTSIZE);

    if (pVia->FBBase)
        pci_device_unmap_range(pVia->PciInfo, (pointer) pVia->FBBase,
                               pVia->videoRambytes);
#else
    if (pVia->MapBase)
        xf86UnMapVidMem(pScrn->scrnIndex, (pointer) pVia->MapBase,
                        VIA_MMIO_REGSIZE);

    if (pVia->BltBase)
        xf86UnMapVidMem(pScrn->scrnIndex, (pointer) pVia->BltBase,
                        VIA_MMIO_BLTSIZE);

    if (pVia->FBBase)
        xf86UnMapVidMem(pScrn->scrnIndex, (pointer) pVia->FBBase,
                        pVia->videoRambytes);
#endif
}

static void
VIALoadRgbLut(ScrnInfoPtr pScrn, int numColors, int *indices, LOCO *colors,
              VisualPtr pVisual)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    int i, j, index;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIALoadRgbLut\n"));

    hwp->enablePalette(hwp);
    hwp->writeDacMask(hwp, 0xFF);

    /* We need the same palette contents for both 16 and 24 bits, but X doesn't
     * play: X's colormap handling is hopelessly intertwined with almost every
     * X subsystem.  So we just space out RGB values over the 256*3. */

    switch (pScrn->bitsPerPixel) {
        case 16:
            for (i = 0; i < numColors; i++) {
                index = indices[i];
                hwp->writeDacWriteAddr(hwp, index * 4);
                for (j = 0; j < 4; j++) {
                    hwp->writeDacData(hwp, colors[index / 2].red);
                    hwp->writeDacData(hwp, colors[index].green);
                    hwp->writeDacData(hwp, colors[index / 2].blue);
                }
            }
            break;
        case 8:
        case 24:
        case 32:
            for (i = 0; i < numColors; i++) {
                index = indices[i];
                hwp->writeDacWriteAddr(hwp, index);
                hwp->writeDacData(hwp, colors[index].red);
                hwp->writeDacData(hwp, colors[index].green);
                hwp->writeDacData(hwp, colors[index].blue);
            }
            break;
        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Unsupported bitdepth: %d\n", pScrn->bitsPerPixel);
            break;
    }
    hwp->disablePalette(hwp);
}

static void
VIALoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices,
               LOCO *colors, VisualPtr pVisual)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    int i, index;
    int SR1A, SR1B, CR67, CR6A;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIALoadPalette: numColors: %d\n", numColors));

    if (pScrn->bitsPerPixel != 8) {

        if (pBIOSInfo->FirstCRTC->IsActive) {

            switch (pVia->Chipset) {
                case VIA_CLE266:
                case VIA_KM400:
                    ViaSeqMask(hwp, 0x16, 0x80, 0x80);
                    break;
                default:
                    ViaCrtcMask(hwp, 0x33, 0x80, 0x80);
                    break;
            }

            ViaSeqMask(hwp, 0x1A, 0x00, 0x01);
            VIALoadRgbLut(pScrn, numColors, indices, colors, pVisual);
        }

        /* If secondary is enabled, adjust its palette too. */
        if (pBIOSInfo->SecondCRTC->IsActive) {
            if (!(pVia->Chipset == VIA_CLE266
                  && CLE266_REV_IS_AX(pVia->ChipRev))) {
                ViaSeqMask(hwp, 0x1A, 0x01, 0x01);
                ViaCrtcMask(hwp, 0x6A, 0x02, 0x02);
                switch (pVia->Chipset) {
                    case VIA_CLE266:
                    case VIA_KM400:
                    case VIA_K8M800:
                    case VIA_PM800:
                        break;
                    default:
                        ViaCrtcMask(hwp, 0x6A, 0x20, 0x20);
                        break;
                }
                VIALoadRgbLut(pScrn, numColors, indices, colors, pVisual);
            }
        }

    } else {

        SR1A = hwp->readSeq(hwp, 0x1A);
        SR1B = hwp->readSeq(hwp, 0x1B);
        CR67 = hwp->readCrtc(hwp, 0x67);
        CR6A = hwp->readCrtc(hwp, 0x6A);

        if (pBIOSInfo->SecondCRTC->IsActive) {
            ViaSeqMask(hwp, 0x1A, 0x01, 0x01);
            ViaSeqMask(hwp, 0x1B, 0x80, 0x80);
            ViaCrtcMask(hwp, 0x67, 0x00, 0xC0);
            ViaCrtcMask(hwp, 0x6A, 0xC0, 0xC0);
        }

        for (i = 0; i < numColors; i++) {
            index = indices[i];
            hwp->writeDacWriteAddr(hwp, index);
            hwp->writeDacData(hwp, colors[index].red);
            hwp->writeDacData(hwp, colors[index].green);
            hwp->writeDacData(hwp, colors[index].blue);
        }

        if (pBIOSInfo->SecondCRTC->IsActive) {
            hwp->writeSeq(hwp, 0x1A, SR1A);
            hwp->writeSeq(hwp, 0x1B, SR1B);
            hwp->writeCrtc(hwp, 0x67, CR67);
            hwp->writeCrtc(hwp, 0x6A, CR6A);

            /* Screen 0 palette was changed by mode setting of Screen 1,
             * so load it again. */
            for (i = 0; i < numColors; i++) {
                index = indices[i];
                hwp->writeDacWriteAddr(hwp, index);
                hwp->writeDacData(hwp, colors[index].red);
                hwp->writeDacData(hwp, colors[index].green);
                hwp->writeDacData(hwp, colors[index].blue);
            }
        }
    }
}

static Bool
VIAScreenInit(SCREEN_INIT_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);

    pScrn->pScreen = pScreen;
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAScreenInit\n"));

    if (!VIAMapFB(pScrn))
        return FALSE;

    if (!VIAMapMMIO(pScrn))
        return FALSE;

    if (pVia->pVbe && pVia->vbeSR) {
        ViaVbeSaveRestore(pScrn, MODE_SAVE);
    } else {
        VIASave(pScrn);
    }

    vgaHWUnlock(hwp);

    pVia->FirstInit = TRUE;
    if (pVia->pVbe) {
        vgaHWBlankScreen(pScrn, FALSE);
        if (!ViaVbeSetMode(pScrn, pScrn->currentMode)) {
            vgaHWBlankScreen(pScrn, TRUE);
            return FALSE;
        }
    } else {
        vgaHWBlankScreen(pScrn, FALSE);

        if (!VIAWriteMode(pScrn, pScrn->currentMode)) {
            vgaHWBlankScreen(pScrn, TRUE);
            return FALSE;
        }
    }
    pVia->FirstInit = FALSE;

    /* Darken the screen for aesthetic reasons and set the viewport. */
    VIASaveScreen(pScreen, SCREEN_SAVER_ON);
    pScrn->AdjustFrame(ADJUST_FRAME_ARGS(pScrn, pScrn->frameX0, pScrn->frameY0));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "- Blanked\n"));

    miClearVisualTypes();

    if (pScrn->bitsPerPixel > 8 && !pVia->IsSecondary) {
        if (!miSetVisualTypes(pScrn->depth, TrueColorMask,
                              pScrn->rgbBits, pScrn->defaultVisual))
            return FALSE;
        if (!miSetPixmapDepths())
            return FALSE;
    } else {
        if (!miSetVisualTypes(pScrn->depth,
                              miGetDefaultVisualMask(pScrn->depth),
                              pScrn->rgbBits, pScrn->defaultVisual))
            return FALSE;
        if (!miSetPixmapDepths())
            return FALSE;
    }

#ifdef OPENCHROMEDRI
    pVia->directRenderingEnabled = VIADRIScreenInit(pScreen);
#endif

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "- Visuals set up\n"));

    if (!VIAInternalScreenInit(pScreen))
        return FALSE;

    xf86SetBlackWhitePixels(pScreen);
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "- B & W\n"));

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
#ifdef USE_FB
    /* Must be after RGB ordering is fixed. */
    fbPictureInit(pScreen, 0, 0);
#endif

    if (!pVia->NoAccel) {
        viaInitAccel(pScreen);
    }

    xf86SetBackingStore(pScreen);
#if 0
    xf86SetSilkenMouse(pScreen);
#endif
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "- Backing store set up\n"));

    if (!pVia->shadowFB)        /* hardware cursor needs to wrap this layer */
        VIADGAInit(pScreen);

    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "- SW cursor set up\n"));

    if (pVia->hwcursor) {
        if (!viaHWCursorInit(pScreen)) {
			pVia->hwcursor = FALSE;
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Hardware cursor initialization failed\n");
        }
    }

    if (pVia->shadowFB)
        ViaShadowFBInit(pScrn, pScreen);
    
    if (pVia->RandRRotation)
    {
        pScrn->DriverFunc = VIADriverFunc;
    }      

    if (!miCreateDefColormap(pScreen))
        return FALSE;
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "- Def Color map set up\n"));

    if (!xf86HandleColormaps(pScreen, 256, 8, VIALoadPalette, NULL,
                             CMAP_RELOAD_ON_MODE_SWITCH
                             | CMAP_PALETTED_TRUECOLOR))
        return FALSE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "- Palette loaded\n"));

    pVia->CloseScreen = pScreen->CloseScreen;
    pScreen->SaveScreen = VIASaveScreen;
    pScreen->CloseScreen = VIACloseScreen;

    xf86DPMSInit(pScreen, VIADPMS, 0);
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "- DPMS set up\n"));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "- Color maps etc. set up\n"));
    pVia->agpDMA = FALSE;

#ifdef OPENCHROMEDRI
    if (pVia->directRenderingEnabled)
        pVia->directRenderingEnabled = VIADRIFinishScreenInit(pScreen);

    if (pVia->directRenderingEnabled) {
        VIADRIPtr pVIADRI = pVia->pDRIInfo->devPrivate;

        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "direct rendering enabled\n");
        pVia->agpDMA = pVia->dma2d && pVIADRI->ringBufActive;
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "direct rendering disabled\n");
    }
#endif
    if (!pVia->NoAccel)
        viaFinishInitAccel(pScreen);

    if (pVia->NoAccel) {
        memset(pVia->FBBase, 0x00, pVia->videoRambytes);
    } else {
#ifdef OPENCHROMEDRI
        if (pVia->directRenderingEnabled)
            DRILock(xf86ScrnToScreen(pScrn), 0);
#endif
        viaAccelFillRect(pScrn, pScrn->frameX0, pScrn->frameY0,
                         pScrn->displayWidth, pScrn->virtualY, 0x00000000);
        viaAccelSyncMarker(pScrn);
#ifdef OPENCHROMEDRI
        if (pVia->directRenderingEnabled)
            DRIUnlock(xf86ScrnToScreen(pScrn));
#endif
    }
    vgaHWBlankScreen(pScrn, TRUE);

    if (pVia->NoAccel) {

        /*
         * This is only for Xv in Noaccel path, and since Xv is in some
         * sense accelerated, it might be a better idea to disable it
         * altogether.
         */

        BoxRec AvailFBArea;

        AvailFBArea.x1 = 0;
        AvailFBArea.y1 = 0;
        AvailFBArea.x2 = pScrn->displayWidth;
        AvailFBArea.y2 = pScrn->virtualY + 1;
        pVia->FBFreeStart = (AvailFBArea.y2 + 1) * pVia->Bpl;
        xf86InitFBManager(pScreen, &AvailFBArea);
        VIAInitLinear(pScreen);
        pVia->driSize = (pVia->FBFreeEnd - pVia->FBFreeStart - pVia->Bpl);
    }

    viaInitVideo(pScreen);

    if (serverGeneration == 1)
        xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);

#ifdef HAVE_DEBUG
    if (pVia->PrintVGARegs) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "VIAScreenInit: Printing VGA registers.\n");
        ViaVgahwPrint(VGAHWPTR(pScrn));
    }

    if (pVia->PrintTVRegs) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "VIAScreenInit: Printing TV registers.\n");
        ViaTVPrintRegs(pScrn);
    }
#endif

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "- Done\n"));
    return TRUE;
}


static int
VIAInternalScreenInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VIAPtr pVia = VIAPTR(pScrn);
    int width, height, displayWidth, shadowHeight;
    unsigned char *FBStart;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAInternalScreenInit\n");

    displayWidth = pScrn->displayWidth;

    if ((pVia->rotate==RR_Rotate_90) || (pVia->rotate==RR_Rotate_270)) {
        height = pScrn->virtualX;
        width = pScrn->virtualY;
    } else {
        width = pScrn->virtualX;
        height = pScrn->virtualY;
    }

    if (pVia->RandRRotation)
        shadowHeight = max(width, height);
    else
        shadowHeight = height;

    if (pVia->shadowFB) {
        pVia->ShadowPitch = BitmapBytePad(pScrn->bitsPerPixel * width);
        pVia->ShadowPtr = malloc(pVia->ShadowPitch * shadowHeight);
        displayWidth = pVia->ShadowPitch / (pScrn->bitsPerPixel >> 3);
        FBStart = pVia->ShadowPtr;
    } else {
        pVia->ShadowPtr = NULL;
        FBStart = pVia->FBBase;
    }

#ifdef USE_FB
    return fbScreenInit(pScreen, FBStart, width, height,
                        pScrn->xDpi, pScrn->yDpi, displayWidth,
                        pScrn->bitsPerPixel);
#else
    switch (pScrn->bitsPerPixel) {
        case 8:
            return cfbScreenInit(pScreen, FBStart, width, height, pScrn->xDpi,
                                 pScrn->yDpi, displayWidth);
        case 16:
            return cfb16ScreenInit(pScreen, FBStart, width, height, pScrn->xDpi,
                                   pScrn->yDpi, displayWidth);
        case 32:
            return cfb32ScreenInit(pScreen, FBStart, width, height, pScrn->xDpi,
                                   pScrn->yDpi, displayWidth);
        default:
            xf86DrvMsg(scrnIndex, X_ERROR, "Internal error: invalid bpp (%d) "
                       "in VIAInternalScreenInit\n", pScrn->bitsPerPixel);
            return FALSE;
    }
#endif
    return TRUE;
}

static Bool
VIAWriteMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAWriteMode\n"));

    pVia->OverlaySupported = FALSE;

    pScrn->vtSema = TRUE;

    if (!pVia->pVbe) {

        if (!vgaHWInit(pScrn, mode))
            return FALSE;

        if (pVia->UseLegacyModeSwitch) {
            if (!pVia->IsSecondary)
                ViaModePrimaryLegacy(pScrn, mode);
            else
                ViaModeSecondaryLegacy(pScrn, mode);
        } else {
            ViaCRTCInit(pScrn);
            ViaModeSet(pScrn, mode);
        }

    } else {

        if (!ViaVbeSetMode(pScrn, mode))
            return FALSE;
        /*
         * FIXME: pVia->IsSecondary is not working here.  We should be able
         * to detect when the display is using the secondary head.
         * TODO: This should be enabled for other chipsets as well.
         */
        if (pVia->pBIOSInfo->Panel->IsActive) {
            switch (pVia->Chipset) {
                case VIA_P4M900:
                case VIA_VX800:
                case VIA_VX855:
                case VIA_VX900:
                    /*
                     * Since we are using virtual, we need to adjust
                     * the offset to match the framebuffer alignment.
                     */
                    if (pScrn->displayWidth != mode->CrtcHDisplay)
                        ViaSecondCRTCHorizontalOffset(pScrn);
                    break;
            }
        }
    }

    /* Enable the graphics engine. */
    if (!pVia->NoAccel) {
        VIAInitialize3DEngine(pScrn);
        viaInitialize2DEngine(pScrn);
    }

    VIAAdjustFrame(ADJUST_FRAME_ARGS(pScrn, pScrn->frameX0, pScrn->frameY0));
    return TRUE;
}


static Bool
VIACloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIACloseScreen\n"));

    /* Is the display currently visible? */
    if (pScrn->vtSema) {
#ifdef OPENCHROMEDRI
        if (pVia->directRenderingEnabled)
            DRILock(xf86ScrnToScreen(pScrn), 0);
#endif
        /* Wait for hardware engine to idle before exiting graphical mode. */
        viaAccelSync(pScrn);

        /* A soft reset avoids a 3D hang after X restart. */
        switch (pVia->Chipset) {
            case VIA_K8M890:
            case VIA_P4M900:
            case VIA_VX800:
            case VIA_VX855:
            case VIA_VX900:
                break;
            default :
                hwp->writeSeq(hwp, 0x1A, pVia->SavedReg.SR1A | 0x40);
                break;
        }

        if (!pVia->IsSecondary) {
            /* Turn off all video activities. */
            viaExitVideo(pScrn);
            if (pVia->hwcursor)
                viaHideCursor(pScrn);
        }

        if (pVia->VQEnable)
            viaDisableVQ(pScrn);
    }
#ifdef OPENCHROMEDRI
    if (pVia->directRenderingEnabled)
        VIADRICloseScreen(pScreen);
#endif

    viaExitAccel(pScreen);
    if (pVia->ShadowPtr) {
        free(pVia->ShadowPtr);
        pVia->ShadowPtr = NULL;
    }
    if (pVia->DGAModes) {
        free(pVia->DGAModes);
        pVia->DGAModes = NULL;
    }

    if (pScrn->vtSema) {
        if (pVia->pVbe && pVia->vbeSR)
            ViaVbeSaveRestore(pScrn, MODE_RESTORE);
        else
            VIARestore(pScrn);

        vgaHWLock(hwp);
        VIAUnmapMem(pScrn);
    }
    pScrn->vtSema = FALSE;
    pScreen->CloseScreen = pVia->CloseScreen;
    return (*pScreen->CloseScreen) (CLOSE_SCREEN_ARGS);
}

/*
 * This only gets called when a screen is being deleted.  It does not
 * get called routinely at the end of a server generation.
 */
static void
VIAFreeScreen(FREE_SCREEN_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAFreeScreen\n"));

    VIAFreeRec(pScrn);

    if (xf86LoaderCheckSymbol("vgaHWFreeHWRec"))
        vgaHWFreeHWRec(pScrn);
}

static Bool
VIASaveScreen(ScreenPtr pScreen, int mode)
{
    return vgaHWSaveScreen(pScreen, mode);
}

static void
VIAAdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAAdjustFrame %dx%d\n", x, y));

    if (pVia->pVbe) {
        ViaVbeAdjustFrame(pScrn->scrnIndex, x, y, 0);
    } else {
        if (pVia->UseLegacyModeSwitch) {
            if (!pVia->IsSecondary)
                ViaFirstCRTCSetStartingAddress(pScrn, x, y);
            else
                ViaSecondCRTCSetStartingAddress(pScrn, x, y);
        } else {
            if (pVia->pBIOSInfo->FirstCRTC->IsActive)
                ViaFirstCRTCSetStartingAddress(pScrn, x, y);

            if (pVia->pBIOSInfo->SecondCRTC->IsActive)
                ViaSecondCRTCSetStartingAddress(pScrn, x, y);
        }
    }

    VIAVidAdjustFrame(pScrn, x, y);
}


static Bool
VIARandRGetInfo(ScrnInfoPtr pScrn, Rotation *rotations)
{
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIARandRGetInfo\n");
    
    VIAPtr pVia = VIAPTR(pScrn);

    /* to report what ability we can support. */
    if(pVia->RandRRotation)
       *rotations = RR_Rotate_0 | RR_Rotate_90 |RR_Rotate_180 | RR_Rotate_270;
    else
       *rotations = RR_Rotate_0;

    return TRUE;
}

static Bool
VIARandRSetConfig(ScrnInfoPtr pScrn, xorgRRConfig *config)
{
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIARandRSetConfig\n");
    VIAPtr pVia = VIAPTR(pScrn);
    
    switch(config->rotation) {
        case RR_Rotate_0:            
            pVia->rotate = RR_Rotate_0;
            break;

        case RR_Rotate_90:            
            pVia->rotate = RR_Rotate_90;
            break;
            
        case RR_Rotate_180:            
            pVia->rotate = RR_Rotate_180;
            break;

        case RR_Rotate_270:            
            pVia->rotate = RR_Rotate_270;
            break;

        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Unexpected rotation in VIARandRSetConfig\n");
            pVia->rotate = RR_Rotate_0;
            return FALSE;
    }
    return TRUE;
}

/*
 * The driverFunc. xorgDriverFuncOp specifies the action driver should
 * perform. If requested option is not supported function should return
 * FALSE. pointer can be used to pass arguments to the function or
 * to return data to the caller.
 */

static Bool
VIADriverFunc(ScrnInfoPtr pScrn, xorgDriverFuncOp op, pointer data)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIADriverFunc Operation: %d\n", op));
    
    switch(op) {
        case RR_GET_INFO:          
            return VIARandRGetInfo(pScrn, (Rotation*)data);
        case RR_SET_CONFIG:          
            return VIARandRSetConfig(pScrn, (xorgRRConfig*)data);
        default:
            return FALSE;
    }
    return FALSE;
}



static Bool
VIASwitchMode(SWITCH_MODE_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    VIAPtr pVia = VIAPTR(pScrn);
    Bool ret;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIASwitchMode\n"));

#ifdef OPENCHROMEDRI
    if (pVia->directRenderingEnabled)
        DRILock(xf86ScrnToScreen(pScrn), 0);
#endif

    viaAccelSync(pScrn);

#ifdef OPENCHROMEDRI
    if (pVia->directRenderingEnabled)
        VIADRIRingBufferCleanup(pScrn);
#endif

    if (pVia->VQEnable)
        viaDisableVQ(pScrn);

    ret = VIAWriteMode(pScrn, mode);

#ifdef OPENCHROMEDRI
    if (pVia->directRenderingEnabled) {
        kickVblank(pScrn);
        VIADRIRingBufferInit(pScrn);
        DRIUnlock(xf86ScrnToScreen(pScrn));
    }
#endif
    return ret;
}


static void
VIADPMS(ScrnInfoPtr pScrn, int mode, int flags)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    if (pVia->pVbe) {
        ViaVbeDPMS(pScrn, mode, flags);
    } else {

        switch (mode) {
            case DPMSModeOn:

                if (pBIOSInfo->Lvds->IsActive)
                    ViaLVDSPower(pScrn, TRUE);

                if (pBIOSInfo->CrtActive)
                    ViaDisplayEnableCRT(pScrn);

                if (pBIOSInfo->Panel->IsActive)
                    ViaLCDPower(pScrn, TRUE);

                if (pBIOSInfo->TVActive)
                    ViaTVPower(pScrn, TRUE);

                if (pBIOSInfo->DfpActive)
                    ViaDFPPower(pScrn, TRUE);

                if (pBIOSInfo->Simultaneous->IsActive)
                    ViaDisplayEnableSimultaneous(pScrn);

                break;
            case DPMSModeStandby:
            case DPMSModeSuspend:
            case DPMSModeOff:

                if (pBIOSInfo->Lvds->IsActive)
                    ViaLVDSPower(pScrn, FALSE);

                if (pBIOSInfo->CrtActive)
                    ViaDisplayDisableCRT(pScrn);

                if (pBIOSInfo->Panel->IsActive)
                    ViaLCDPower(pScrn, FALSE);

                if (pBIOSInfo->TVActive)
                    ViaTVPower(pScrn, FALSE);

                if (pBIOSInfo->DfpActive)
                    ViaDFPPower(pScrn, FALSE);
                
                if (pBIOSInfo->Simultaneous->IsActive)
                    ViaDisplayDisableSimultaneous(pScrn);

                break;
            default:
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Invalid DPMS mode %d\n",
                           mode);
                break;
        }
    }

}

void
VIAInitialize3DEngine(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    int i;

    VIASETREG(VIA_REG_TRANSET, 0x00010000);
    for (i = 0; i <= 0x7D; i++)
        VIASETREG(VIA_REG_TRANSPACE, (CARD32) i << 24);

    VIASETREG(VIA_REG_TRANSET, 0x00020000);
    for (i = 0; i <= 0x94; i++)
        VIASETREG(VIA_REG_TRANSPACE, (CARD32) i << 24);
    VIASETREG(VIA_REG_TRANSPACE, 0x82400000);

    VIASETREG(VIA_REG_TRANSET, 0x01020000);
    for (i = 0; i <= 0x94; i++)
        VIASETREG(VIA_REG_TRANSPACE, (CARD32) i << 24);
    VIASETREG(VIA_REG_TRANSPACE, 0x82400000);

    VIASETREG(VIA_REG_TRANSET, 0xfe020000);
    for (i = 0; i <= 0x03; i++)
        VIASETREG(VIA_REG_TRANSPACE, (CARD32) i << 24);

    VIASETREG(VIA_REG_TRANSET, 0x00030000);
    for (i = 0; i <= 0xff; i++)
        VIASETREG(VIA_REG_TRANSPACE, 0);

    VIASETREG(VIA_REG_TRANSET, 0x00100000);
    VIASETREG(VIA_REG_TRANSPACE, 0x00333004);
    VIASETREG(VIA_REG_TRANSPACE, 0x10000002);
    VIASETREG(VIA_REG_TRANSPACE, 0x60000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x61000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x62000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x63000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x64000000);

    VIASETREG(VIA_REG_TRANSET, 0x00fe0000);
    if (pVia->Chipset == VIA_CLE266 && pVia->ChipRev >= 3)
        VIASETREG(VIA_REG_TRANSPACE, 0x40008c0f);
    else
        VIASETREG(VIA_REG_TRANSPACE, 0x4000800f);
    VIASETREG(VIA_REG_TRANSPACE, 0x44000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x45080C04);
    VIASETREG(VIA_REG_TRANSPACE, 0x46800408);
    VIASETREG(VIA_REG_TRANSPACE, 0x50000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x51000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x52000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x53000000);

    VIASETREG(VIA_REG_TRANSET, 0x00fe0000);
    VIASETREG(VIA_REG_TRANSPACE, 0x08000001);
    VIASETREG(VIA_REG_TRANSPACE, 0x0A000183);
    VIASETREG(VIA_REG_TRANSPACE, 0x0B00019F);
    VIASETREG(VIA_REG_TRANSPACE, 0x0C00018B);
    VIASETREG(VIA_REG_TRANSPACE, 0x0D00019B);
    VIASETREG(VIA_REG_TRANSPACE, 0x0E000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x0F000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x10000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x11000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x20000000);
}
