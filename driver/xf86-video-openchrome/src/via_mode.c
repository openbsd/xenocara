/*
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

/*
 * via_mode.c
 *
 * Everything to do with setting and changing modes.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via.h"
#include "via_driver.h"
#include "via_vgahw.h"
#include "via_id.h"

/*
 * Modetable nonsense.
 *
 */
#include "via_mode.h"

/*
 *
 * TV specific code.
 *
 */

/*
 *
 */
static Bool
ViaTVDetect(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIATVDetect\n"));

    /* preset some pBIOSInfo TV related values -- move up */
    pBIOSInfo->TVEncoder = VIA_NONETV;
    pBIOSInfo->TVI2CDev = NULL;
    pBIOSInfo->TVSave = NULL;
    pBIOSInfo->TVRestore = NULL;
    pBIOSInfo->TVDACSense = NULL;
    pBIOSInfo->TVModeValid = NULL;
    pBIOSInfo->TVModeI2C = NULL;
    pBIOSInfo->TVModeCrtc = NULL;
    pBIOSInfo->TVPower = NULL;
    pBIOSInfo->TVModes = NULL;
    pBIOSInfo->TVPrintRegs = NULL;
    pBIOSInfo->LCDPower = NULL;
    pBIOSInfo->TVNumRegs = 0;

    /*
     * On an SK43G (KM400/Ch7011), false positive detections at a VT162x
     * chip were observed, so try to detect the Ch7011 first.
     */
    if (pVia->pI2CBus2 && xf86I2CProbeAddress(pVia->pI2CBus2, 0xEC))
        pBIOSInfo->TVI2CDev = ViaCH7xxxDetect(pScrn, pVia->pI2CBus2, 0xEC);
    else if (pVia->pI2CBus2 && xf86I2CProbeAddress(pVia->pI2CBus2, 0x40))
        pBIOSInfo->TVI2CDev = ViaVT162xDetect(pScrn, pVia->pI2CBus2, 0x40);
    else if (pVia->pI2CBus3 && xf86I2CProbeAddress(pVia->pI2CBus3, 0x40))
        pBIOSInfo->TVI2CDev = ViaVT162xDetect(pScrn, pVia->pI2CBus3, 0x40);
    else if (pVia->pI2CBus2 && xf86I2CProbeAddress(pVia->pI2CBus2, 0xEA))
        pBIOSInfo->TVI2CDev = ViaCH7xxxDetect(pScrn, pVia->pI2CBus2, 0xEA);
    else if (pVia->pI2CBus3 && xf86I2CProbeAddress(pVia->pI2CBus3, 0xEA))
        pBIOSInfo->TVI2CDev = ViaCH7xxxDetect(pScrn, pVia->pI2CBus3, 0xEA);

    if (pBIOSInfo->TVI2CDev)
        return TRUE;
    return FALSE;
}

/*
 *
 */
static Bool
ViaTVInit(ScrnInfoPtr pScrn)
{
    VIABIOSInfoPtr pBIOSInfo = VIAPTR(pScrn)->pBIOSInfo;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaTVInit\n"));

    switch (pBIOSInfo->TVEncoder) {
        case VIA_VT1621:
        case VIA_VT1622:
        case VIA_VT1623:
        case VIA_VT1625:
            ViaVT162xInit(pScrn);
            break;
        case VIA_CH7011:
        case VIA_CH7019A:
        case VIA_CH7019B:
            ViaCH7xxxInit(pScrn);
            break;
        default:
            return FALSE;
            break;
    }

    if (!pBIOSInfo->TVSave || !pBIOSInfo->TVRestore
        || !pBIOSInfo->TVDACSense || !pBIOSInfo->TVModeValid
        || !pBIOSInfo->TVModeI2C || !pBIOSInfo->TVModeCrtc
        || !pBIOSInfo->TVPower || !pBIOSInfo->TVModes
        || !pBIOSInfo->TVPrintRegs) {

        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "ViaTVInit: TVEncoder was not properly initialised.\n");

        xf86DestroyI2CDevRec(pBIOSInfo->TVI2CDev, TRUE);
        pBIOSInfo->TVI2CDev = NULL;
        pBIOSInfo->TVOutput = TVOUTPUT_NONE;
        pBIOSInfo->TVEncoder = VIA_NONETV;
        pBIOSInfo->TVI2CDev = NULL;
        pBIOSInfo->TVSave = NULL;
        pBIOSInfo->TVRestore = NULL;
        pBIOSInfo->TVDACSense = NULL;
        pBIOSInfo->TVModeValid = NULL;
        pBIOSInfo->TVModeI2C = NULL;
        pBIOSInfo->TVModeCrtc = NULL;
        pBIOSInfo->TVPower = NULL;
        pBIOSInfo->TVModes = NULL;
        pBIOSInfo->TVPrintRegs = NULL;
        pBIOSInfo->TVNumRegs = 0;

        return FALSE;
    }

    /* Save now */
    pBIOSInfo->TVSave(pScrn);

#ifdef HAVE_DEBUG
    if (VIAPTR(pScrn)->PrintTVRegs)
        pBIOSInfo->TVPrintRegs(pScrn);
#endif

    return TRUE;
}

void
ViaTVSave(ScrnInfoPtr pScrn)
{
    VIABIOSInfoPtr pBIOSInfo = VIAPTR(pScrn)->pBIOSInfo;

    if (pBIOSInfo->TVSave)
        pBIOSInfo->TVSave(pScrn);
}

void
ViaTVRestore(ScrnInfoPtr pScrn)
{
    VIABIOSInfoPtr pBIOSInfo = VIAPTR(pScrn)->pBIOSInfo;

    if (pBIOSInfo->TVRestore)
        pBIOSInfo->TVRestore(pScrn);
}

static Bool
ViaTVDACSense(ScrnInfoPtr pScrn)
{
    VIABIOSInfoPtr pBIOSInfo = VIAPTR(pScrn)->pBIOSInfo;

    if (pBIOSInfo->TVDACSense)
        return pBIOSInfo->TVDACSense(pScrn);
    return FALSE;
}

static void
ViaTVSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIABIOSInfoPtr pBIOSInfo = VIAPTR(pScrn)->pBIOSInfo;

    if (pBIOSInfo->TVModeI2C)
        pBIOSInfo->TVModeI2C(pScrn, mode);

    if (pBIOSInfo->TVModeCrtc)
        pBIOSInfo->TVModeCrtc(pScrn, mode);
}

void
ViaTVPower(ScrnInfoPtr pScrn, Bool On)
{
    VIABIOSInfoPtr pBIOSInfo = VIAPTR(pScrn)->pBIOSInfo;

#ifdef HAVE_DEBUG
    if (On)
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaTVPower: On.\n");
    else
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaTVPower: Off.\n");
#endif

    if (pBIOSInfo->TVPower)
        pBIOSInfo->TVPower(pScrn, On);
}

#ifdef HAVE_DEBUG
void
ViaTVPrintRegs(ScrnInfoPtr pScrn)
{
    VIABIOSInfoPtr pBIOSInfo = VIAPTR(pScrn)->pBIOSInfo;

    if (pBIOSInfo->TVPrintRegs)
        pBIOSInfo->TVPrintRegs(pScrn);
}
#endif /* HAVE_DEBUG */

/*
 *
 */
static ModeStatus
ViaTVModeValid(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIABIOSInfoPtr pBIOSInfo = VIAPTR(pScrn)->pBIOSInfo;

    if (pBIOSInfo->TVModeValid)
        return pBIOSInfo->TVModeValid(pScrn, mode);
    return MODE_OK;
}

/*
 *
 */
void
ViaOutputsDetect(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaOutputsDetect\n"));

    pBIOSInfo->CrtPresent = FALSE;
    pBIOSInfo->PanelPresent = FALSE;

    /* Panel */
    if (pBIOSInfo->ForcePanel) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Enabling panel from config.\n");
        pBIOSInfo->PanelPresent = TRUE;
    } else if (pVia->Id && (pVia->Id->Outputs & VIA_DEVICE_LCD)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "Enabling panel from PCI-subsystem ID information.\n");
        pBIOSInfo->PanelPresent = TRUE;
    }

    /* Crt */
    if (pVia->DDC1)
        pBIOSInfo->CrtPresent = TRUE;
    /* If any of the unichromes support this, add CRT detection here */
    else if (!pBIOSInfo->PanelPresent) {
        /* Make sure that at least CRT is enabled. */
        if (!pVia->Id || (pVia->Id->Outputs & VIA_DEVICE_CRT))
            pBIOSInfo->CrtPresent = TRUE;
    }

    /* 
     * FIXME: xf86I2CProbeAddress(pVia->pI2CBus3, 0x40)
     * disables the panel on P4M900
     * See ViaTVDetect.
     */
    if (pVia->Chipset == VIA_P4M900 && pBIOSInfo->PanelPresent) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "Will not try to detect TV encoder.\n");
    } else {
        /* TV encoder */
        if (ViaTVDetect(pScrn) && ViaTVInit(pScrn)) {
            if (!pBIOSInfo->TVOutput)  /* Config might've set this already */
                ViaTVDACSense(pScrn);
        } else if (pVia->Id && (pVia->Id->Outputs & VIA_DEVICE_TV)) {
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                       "This device is supposed to have a TV encoder, but "
                       "we are unable to detect it (support missing?).\n");
            pBIOSInfo->TVOutput = 0;
        }
    }
}

#ifdef HAVE_DEBUG
/*
 * Returns:
 *   Bit[7] 2nd Path
 *   Bit[6] 1/0 MHS Enable/Disable
 *   Bit[5] 0 = Bypass Callback, 1 = Enable Callback
 *   Bit[4] 0 = Hot-Key Sequence Control (OEM Specific)
 *   Bit[3] LCD
 *   Bit[2] TV
 *   Bit[1] CRT
 *   Bit[0] DVI
 */
static CARD8
VIAGetActiveDisplay(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD8 tmp;

    tmp = (hwp->readCrtc(hwp, 0x3E) >> 4);
    tmp |= ((hwp->readCrtc(hwp, 0x3B) & 0x18) << 3);

    return tmp;
}
#endif /* HAVE_DEBUG */

/*
 *
 */
Bool
ViaOutputsSelect(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    if (pVia->IsSecondary) {  /* we just abort for now */
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "ViaOutputsSelect:"
                   " Not handling secondary.\n");
        return FALSE;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaOutputsSelect\n"));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaOutputsSelect: X"
                     " Configuration: 0x%02x\n", pVia->ActiveDevice));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaOutputsSelect: BIOS"
                     " Initialised register: 0x%02x\n",
                     VIAGetActiveDisplay(pScrn)));

    pBIOSInfo->PanelActive = FALSE;
    pBIOSInfo->CrtActive = FALSE;
    pBIOSInfo->TVActive = FALSE;

    if (!pVia->ActiveDevice) {
        /* always enable the panel when present */
        if (pBIOSInfo->PanelPresent)
            pBIOSInfo->PanelActive = TRUE;
        else if (pBIOSInfo->TVOutput != TVOUTPUT_NONE)  /* cable is attached! */
            pBIOSInfo->TVActive = TRUE;

        /* CRT can be used with everything when present */
        if (pBIOSInfo->CrtPresent)
            pBIOSInfo->CrtActive = TRUE;
    } else {
        if (pVia->ActiveDevice & VIA_DEVICE_LCD) {
            if (pBIOSInfo->PanelPresent)
                pBIOSInfo->PanelActive = TRUE;
            else
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Unable to activate"
                           " panel: no panel is present.\n");
        }

        if (pVia->ActiveDevice & VIA_DEVICE_TV) {
            if (!pBIOSInfo->TVI2CDev)
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Unable to activate"
                           " TV encoder: no TV encoder present.\n");
            else if (pBIOSInfo->TVOutput == TVOUTPUT_NONE)
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Unable to activate"
                           " TV encoder: no cable attached.\n");
            else if (pBIOSInfo->PanelActive)
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Unable to activate"
                           " TV encoder and panel simultaneously. Not using"
                           " TV encoder.\n");
            else
                pBIOSInfo->TVActive = TRUE;
        }

        if ((pVia->ActiveDevice & VIA_DEVICE_CRT)
            || (!pBIOSInfo->PanelActive && !pBIOSInfo->TVActive)) {
            pBIOSInfo->CrtPresent = TRUE;
            pBIOSInfo->CrtActive = TRUE;
        }
    }

#ifdef HAVE_DEBUG
    if (pBIOSInfo->CrtActive)
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                         "ViaOutputsSelect: CRT.\n"));
    if (pBIOSInfo->PanelActive)
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                         "ViaOutputsSelect: Panel.\n"));
    if (pBIOSInfo->TVActive)
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                         "ViaOutputsSelect: TV.\n"));
#endif
    return TRUE; /* !Secondary always has at least CRT */
}

/*
 * Try to interprete EDID ourselves.
 */
static Bool
ViaGetPanelSizeFromEDID(ScrnInfoPtr pScrn, xf86MonPtr pMon, int *size)
{
    int i, max = 0;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAGetPanelSizeFromEDID\n"));

    /* !!! Why are we not checking VESA modes? */

    /* checking standard timings */
    for (i = 0; i < 8; i++)
        if ((pMon->timings2[i].hsize > 256) && (pMon->timings2[i].hsize > max))
            max = pMon->timings2[i].hsize;

    if (max != 0) {
        *size = max;
        return TRUE;
    }

    /* checking detailed monitor section */

    /* !!! skip Ranges and standard timings */

    /* check detailed timings */
    for (i = 0; i < DET_TIMINGS; i++)
        if (pMon->det_mon[i].type == DT) {
            struct detailed_timings timing = pMon->det_mon[i].section.d_timings;

            /* ignore v_active for now */
            if ((timing.clock > 15000000) && (timing.h_active > max))
                max = timing.h_active;
        }

    if (max != 0) {
        *size = max;
        return TRUE;
    }

    return FALSE;
}

/*
 *
 */
static Bool
VIAGetPanelSizeFromDDCv1(ScrnInfoPtr pScrn, int *size)
{
    VIAPtr pVia = VIAPTR(pScrn);
    xf86MonPtr pMon;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAGetPanelSizeFromDDCv1\n"));

    if (!xf86I2CProbeAddress(pVia->pI2CBus2, 0xA0))
        return FALSE;

    pMon = xf86DoEDID_DDC2(pScrn->scrnIndex, pVia->pI2CBus2);
    if (!pMon)
        return FALSE;

    pVia->DDC2 = pMon;

    if (!pVia->DDC1) {
        xf86PrintEDID(pMon);
        xf86SetDDCproperties(pScrn, pMon);
    }

    if (!ViaGetPanelSizeFromEDID(pScrn, pMon, size)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Unable to read PanelSize from EDID information\n");
        return FALSE;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "VIAGetPanelSizeFromDDCv1: %d\n", *size));
    return TRUE;
}

/*
 *
 */
static Bool
VIAGetPanelSizeFromDDCv2(ScrnInfoPtr pScrn, int *size)
{
    VIAPtr pVia = VIAPTR(pScrn);
    CARD8 W_Buffer[1];
    CARD8 R_Buffer[2];
    I2CDevPtr dev;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAGetPanelSizeFromDDCv2\n"));

    if (!xf86I2CProbeAddress(pVia->pI2CBus2, 0xA2))
        return FALSE;

    dev = xf86CreateI2CDevRec();
    if (!dev)
        return FALSE;

    dev->DevName = "EDID2";
    dev->SlaveAddr = 0xA2;
    dev->ByteTimeout = 2200;  /* VESA DDC spec 3 p. 43 (+10 %) */
    dev->StartTimeout = 550;
    dev->BitTimeout = 40;
    dev->ByteTimeout = 40;
    dev->AcknTimeout = 40;
    dev->pI2CBus = pVia->pI2CBus2;

    if (!xf86I2CDevInit(dev)) {
        xf86DestroyI2CDevRec(dev, TRUE);
        return FALSE;
    }

    xf86I2CReadByte(dev, 0x00, R_Buffer);
    if (R_Buffer[0] != 0x20) {
        xf86DestroyI2CDevRec(dev, TRUE);
        return FALSE;
    }

    /* Found EDID2 Table */

    W_Buffer[0] = 0x76;
    xf86I2CWriteRead(dev, W_Buffer, 1, R_Buffer, 2);
    xf86DestroyI2CDevRec(dev, TRUE);

    *size = R_Buffer[0] | (R_Buffer[1] << 8);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "VIAGetPanelSizeFromDDCv2: %d\n", *size));
    return TRUE;
}

/*
 *
 */
static void
VIAGetPanelSize(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;
    char *PanelSizeString[7] = { "640x480", "800x600", "1024x768", "1280x768"
                                 "1280x1024", "1400x1050", "1600x1200" };
    int size = 0;
    Bool ret;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAGetPanelSize\n"));

    ret = VIAGetPanelSizeFromDDCv1(pScrn, &size);
    if (!ret)
        ret = VIAGetPanelSizeFromDDCv2(pScrn, &size);

    if (ret) {
        switch (size) {
            case 640:
                pBIOSInfo->PanelSize = VIA_PANEL6X4;
                break;
            case 800:
                pBIOSInfo->PanelSize = VIA_PANEL8X6;
                break;
            case 1024:
                pBIOSInfo->PanelSize = VIA_PANEL10X7;
                break;
            case 1280:
                pBIOSInfo->PanelSize = VIA_PANEL12X10;
                break;
            case 1400:
                pBIOSInfo->PanelSize = VIA_PANEL14X10;
                break;
            case 1600:
                pBIOSInfo->PanelSize = VIA_PANEL16X12;
                break;
            default:
                pBIOSInfo->PanelSize = VIA_PANEL_INVALID;
                break;
        }
    } else {
        pBIOSInfo->PanelSize = hwp->readCrtc(hwp, 0x3F) >> 4;
        if (pBIOSInfo->PanelSize == 0) {
            /* VIA_PANEL6X4 == 0, but that value equals unset */
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Unable to "
                       "retrieve PanelSize: using default (1024x768)\n");
            pBIOSInfo->PanelSize = VIA_PANEL10X7;
            return;
        }
    }

    if (pBIOSInfo->PanelSize < 7)
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using panel at %s.\n",
                   PanelSizeString[pBIOSInfo->PanelSize]);
    else
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Unknown panel size "
                   "detected: %d.\n", pBIOSInfo->PanelSize);
}

/*
 *
 */
static Bool
ViaGetResolutionIndex(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIABIOSInfoPtr pBIOSInfo = VIAPTR(pScrn)->pBIOSInfo;
    int i;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "ViaGetResolutionIndex: Looking for %dx%d\n",
                     mode->CrtcHDisplay, mode->CrtcVDisplay));
    for (i = 0; ViaResolutionTable[i].Index != VIA_RES_INVALID; i++) {
        if ((ViaResolutionTable[i].X == mode->CrtcHDisplay)
            && (ViaResolutionTable[i].Y == mode->CrtcVDisplay)) {
            pBIOSInfo->ResolutionIndex = ViaResolutionTable[i].Index;
            DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaGetResolutionIndex:"
                             " %d\n", pBIOSInfo->ResolutionIndex));
            return TRUE;
        }
    }

    pBIOSInfo->ResolutionIndex = VIA_RES_INVALID;
    return FALSE;
}

/*
 *
 */
static int
ViaGetVesaMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    int i;

    for (i = 0; ViaVesaModes[i].Width; i++)
        if ((ViaVesaModes[i].Width == mode->CrtcHDisplay)
            && (ViaVesaModes[i].Height == mode->CrtcVDisplay)) {
            switch (pScrn->bitsPerPixel) {
                case 8:
                    return ViaVesaModes[i].mode_8b;
                case 16:
                    return ViaVesaModes[i].mode_16b;
                case 24:
                case 32:
                    return ViaVesaModes[i].mode_32b;
                default:
                    return 0xFFFF;
            }
        }
    return 0xFFFF;
}

/*
 *
 * ViaResolutionTable[i].PanelIndex is pBIOSInfo->PanelSize 
 * pBIOSInfo->PanelIndex is the index to lcdTable.
 *
 */
static Bool
ViaPanelGetIndex(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIABIOSInfoPtr pBIOSInfo = VIAPTR(pScrn)->pBIOSInfo;
    int i;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaPanelGetIndex\n"));

    pBIOSInfo->PanelIndex = VIA_BIOS_NUM_PANEL;

    if (pBIOSInfo->PanelSize == VIA_PANEL_INVALID) {
        VIAGetPanelSize(pScrn);
        if (pBIOSInfo->PanelSize == VIA_PANEL_INVALID) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "ViaPanelGetIndex: PanelSize not set.\n");
            return FALSE;
        }
    }

    if ((mode->PrivSize != sizeof(struct ViaModePriv))
        || (mode->Private != (void *)&ViaPanelPrivate)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "ViaPanelGetIndex: Mode not supported by Panel.\n");
        return FALSE;
    }

    if (!ViaGetResolutionIndex(pScrn, mode)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Panel does not support this"
                   " resolution: %s\n", mode->name);
        return FALSE;
    }

    for (i = 0; ViaResolutionTable[i].Index != VIA_RES_INVALID; i++)
        if (ViaResolutionTable[i].PanelIndex == pBIOSInfo->PanelSize) {
            pBIOSInfo->panelX = ViaResolutionTable[i].X;
            pBIOSInfo->panelY = ViaResolutionTable[i].Y;
            break;
        }

    if (ViaResolutionTable[i].Index == VIA_RES_INVALID) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaPanelGetIndex: Unable"
                   " to find matching PanelSize in ViaResolutionTable.\n");
        return FALSE;
    }

    if ((pBIOSInfo->panelX != mode->CrtcHDisplay)
        || (pBIOSInfo->panelY != mode->CrtcVDisplay)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaPanelGetIndex: Non-native"
                   " resolutions are broken.\n");
        return FALSE;
    }

    for (i = 0; i < VIA_BIOS_NUM_PANEL; i++)
        if (lcdTable[i].fpSize == pBIOSInfo->PanelSize) {
            int modeNum, tmp;

            modeNum = ViaGetVesaMode(pScrn, mode);
            if (modeNum == 0xFFFF) {
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "ViaPanelGetIndex: "
                           "Unable to determine matching VESA modenumber.\n");
                return FALSE;
            }

            tmp = 0x01 << (modeNum & 0xF);
            if ((CARD16) tmp & lcdTable[i].SuptMode[(modeNum >> 4)]) {
                pBIOSInfo->PanelIndex = i;
                DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaPanelGetIndex:"
                                 "index: %d (%dx%d)\n", pBIOSInfo->PanelIndex,
                                 pBIOSInfo->panelX, pBIOSInfo->panelY));
                return TRUE;
            }

            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaPanelGetIndex: Unable"
                       " to match given mode with this PanelSize.\n");
            return FALSE;
        }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaPanelGetIndex: Unable"
               " to match PanelSize with an lcdTable entry.\n");
    return FALSE;
}

/*
 * Stolen from xf86Config.c's addDefaultModes
 */
static void
ViaModesAttachHelper(ScrnInfoPtr pScrn, MonPtr monitorp, DisplayModePtr Modes)
{
    DisplayModePtr mode;
    DisplayModePtr last = monitorp->Last;
    int i;

    for (i = 0; Modes[i].name; i++) {
        mode = xnfalloc(sizeof(DisplayModeRec));
        memcpy(mode, &Modes[i], sizeof(DisplayModeRec));
        mode->name = xnfstrdup(Modes[i].name);
        if (last) {
            mode->prev = last;
            last->next = mode;
        } else {  /* this is the first mode */
            monitorp->Modes = mode;
            mode->prev = NULL;
        }
        last = mode;
    }
    monitorp->Last = last;
}

/*
 *
 */
void
ViaModesAttach(ScrnInfoPtr pScrn, MonPtr monitorp)
{
    VIABIOSInfoPtr pBIOSInfo = VIAPTR(pScrn)->pBIOSInfo;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModesAttach\n"));

    if (pBIOSInfo->PanelActive)
        ViaModesAttachHelper(pScrn, monitorp, ViaPanelModes);
    if (pBIOSInfo->TVActive && pBIOSInfo->TVModes)
        ViaModesAttachHelper(pScrn, monitorp, pBIOSInfo->TVModes);
}

/*
 *
 */
CARD32
ViaGetMemoryBandwidth(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaGetMemoryBandwidth\n"));

    switch (pVia->Chipset) {
        case VIA_CLE266:
            if (CLE266_REV_IS_AX(pVia->ChipRev))
                return ViaBandwidthTable[VIA_BW_CLE266A].Bandwidth[pVia->
                                                                   MemClk];
            else
                return ViaBandwidthTable[VIA_BW_CLE266C].Bandwidth[pVia->
                                                                   MemClk];
        case VIA_KM400:
            /* 0x84 is earliest public device, 0x80 is more likely though */
            if (pVia->ChipRev < 0x84)
                return ViaBandwidthTable[VIA_BW_KM400].Bandwidth[pVia->MemClk];
            else
                return ViaBandwidthTable[VIA_BW_KM400A].Bandwidth[pVia->MemClk];
        case VIA_K8M800:
            return ViaBandwidthTable[VIA_BW_K8M800].Bandwidth[pVia->MemClk];
        case VIA_PM800:
            return ViaBandwidthTable[VIA_BW_PM800].Bandwidth[pVia->MemClk];
        case VIA_VM800:
            return ViaBandwidthTable[VIA_BW_VM800].Bandwidth[pVia->MemClk];
        case VIA_K8M890:
            return ViaBandwidthTable[VIA_BW_K8M890].Bandwidth[pVia->MemClk];
        case VIA_P4M900:
            return ViaBandwidthTable[VIA_BW_P4M900].Bandwidth[pVia->MemClk];
        case VIA_CX700:
            return ViaBandwidthTable[VIA_BW_CX700].Bandwidth[pVia->MemClk];
        case VIA_P4M890:
            return ViaBandwidthTable[VIA_BW_P4M890].Bandwidth[pVia->MemClk];
        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "ViaBandwidthAllowed: Unknown Chipset.\n");
            return VIA_BW_MIN;
    }
}

/*
 * Checks for limitations imposed by the available VGA timing registers.
 *
 */
static ModeStatus
ViaModePrimaryVGAValid(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModePrimaryVGAValid\n"));

    if (mode->CrtcHTotal > 4100) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHTotal out of range.\n");
        return MODE_BAD_HVALUE;
    }

    if (mode->CrtcHDisplay > 2048) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHDisplay out of range.\n");
        return MODE_BAD_HVALUE;
    }

    if (mode->CrtcHBlankStart > 2048) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHBlankStart out of range.\n");
        return MODE_BAD_HVALUE;
    }

    if ((mode->CrtcHBlankEnd - mode->CrtcHBlankStart) > 1025) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHBlankEnd out of range.\n");
        return MODE_HBLANK_WIDE;
    }

    if (mode->CrtcHSyncStart > 4095) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHSyncStart out of range.\n");
        return MODE_BAD_HVALUE;
    }

    if ((mode->CrtcHSyncEnd - mode->CrtcHSyncStart) > 256) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHSyncEnd out of range.\n");
        return MODE_HSYNC_WIDE;
    }

    if (mode->CrtcVTotal > 2049) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVTotal out of range.\n");
        return MODE_BAD_VVALUE;
    }

    if (mode->CrtcVDisplay > 2048) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVDisplay out of range.\n");
        return MODE_BAD_VVALUE;
    }

    if (mode->CrtcVSyncStart > 2047) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVSyncStart out of range.\n");
        return MODE_BAD_VVALUE;
    }

    if ((mode->CrtcVSyncEnd - mode->CrtcVSyncStart) > 16) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVSyncEnd out of range.\n");
        return MODE_VSYNC_WIDE;
    }

    if (mode->CrtcVBlankStart > 2048) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVBlankStart out of range.\n");
        return MODE_BAD_VVALUE;
    }

    if ((mode->CrtcVBlankEnd - mode->CrtcVBlankStart) > 257) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVBlankEnd out of range.\n");
        return MODE_VBLANK_WIDE;
    }

    return MODE_OK;
}

/*
 *
 */
static ModeStatus
ViaModeSecondaryVGAValid(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModeSecondaryVGAValid\n"));

    if (mode->CrtcHTotal > 4096) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHTotal out of range.\n");
        return MODE_BAD_HVALUE;
    }

    if (mode->CrtcHDisplay > 2048) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHDisplay out of range.\n");
        return MODE_BAD_HVALUE;
    }

    if (mode->CrtcHBlankStart > 2048) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHBlankStart out of range.\n");
        return MODE_BAD_HVALUE;
    }

    if (mode->CrtcHBlankEnd > 4096) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHBlankEnd out of range.\n");
        return MODE_HBLANK_WIDE;
    }

    if (mode->CrtcHSyncStart > 2047) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHSyncStart out of range.\n");
        return MODE_BAD_HVALUE;
    }

    if ((mode->CrtcHSyncEnd - mode->CrtcHSyncStart) > 512) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHSyncEnd out of range.\n");
        return MODE_HSYNC_WIDE;
    }

    if (mode->CrtcVTotal > 2048) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVTotal out of range.\n");
        return MODE_BAD_VVALUE;
    }

    if (mode->CrtcVDisplay > 2048) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVDisplay out of range.\n");
        return MODE_BAD_VVALUE;
    }

    if (mode->CrtcVBlankStart > 2048) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVBlankStart out of range.\n");
        return MODE_BAD_VVALUE;
    }

    if (mode->CrtcVBlankEnd > 2048) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVBlankEnd out of range.\n");
        return MODE_VBLANK_WIDE;
    }

    if (mode->CrtcVSyncStart > 2047) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVSyncStart out of range.\n");
        return MODE_BAD_VVALUE;
    }

    if ((mode->CrtcVSyncEnd - mode->CrtcVSyncStart) > 32) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVSyncEnd out of range.\n");
        return MODE_VSYNC_WIDE;
    }

    return MODE_OK;
}


static CARD32 ViaModeDotClockTranslate(ScrnInfoPtr pScrn, DisplayModePtr mode);

/*
 *
 */
ModeStatus
ViaValidMode(int scrnIndex, DisplayModePtr mode, Bool verbose, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;
    ModeStatus ret;
    CARD32 temp;

    if (pVia->pVbe)
        return MODE_OK;

    DEBUG(xf86DrvMsg(scrnIndex, X_INFO, "ViaValidMode: Validating %s (%d)\n",
                     mode->name, mode->Clock));

    if (mode->Flags & V_INTERLACE)
        return MODE_NO_INTERLACE;

    if (pVia->IsSecondary)
        ret = ViaModeSecondaryVGAValid(pScrn, mode);
    else
        ret = ViaModePrimaryVGAValid(pScrn, mode);

    if (ret != MODE_OK)
        return ret;

    if (pBIOSInfo->TVActive) {
        ret = ViaTVModeValid(pScrn, mode);
        if (ret != MODE_OK) {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "Mode \"%s\" is not supported by TV encoder.\n",
                       mode->name);
            return ret;
        }
    } else if (pBIOSInfo->PanelActive && !ViaPanelGetIndex(pScrn, mode))
        return MODE_BAD;
    else if (!ViaModeDotClockTranslate(pScrn, mode))
        return MODE_NOCLOCK;

    temp = mode->CrtcHDisplay * mode->CrtcVDisplay * mode->VRefresh
            * (pScrn->bitsPerPixel >> 3);
    if (pBIOSInfo->Bandwidth < temp) {
        xf86DrvMsg(scrnIndex, X_INFO,
                   "Required bandwidth is not available. (%u > %u)\n",
                   (unsigned)temp, (unsigned)pBIOSInfo->Bandwidth);
        return MODE_CLOCK_HIGH; /* since there is no MODE_BANDWIDTH */
    }

    return MODE_OK;
}

/*
 *
 * Some very common abstractions.
 *
 */

/*
 * Standard vga call really.
 * Needs to be called to reset the dotclock (after SR40:2/1 reset)
 */
static void
ViaSetUseExternalClock(vgaHWPtr hwp)
{
    CARD8 data;

    DEBUG(xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO,
                     "ViaSetUseExternalClock\n"));

    data = hwp->readMiscOut(hwp);
    hwp->writeMiscOut(hwp, data | 0x0C);
}

/* 
 *
 */
static void
ViaSetPrimaryDotclock(ScrnInfoPtr pScrn, CARD32 clock)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO,
                     "ViaSetPrimaryDotclock to 0x%06x\n", (unsigned)clock));

    if ((pVia->Chipset == VIA_CLE266) || (pVia->Chipset == VIA_KM400)) {
        hwp->writeSeq(hwp, 0x46, clock >> 8);
        hwp->writeSeq(hwp, 0x47, clock & 0xFF);
    } else {  /* unichrome pro */
        hwp->writeSeq(hwp, 0x44, clock >> 16);
        hwp->writeSeq(hwp, 0x45, (clock >> 8) & 0xFF);
        hwp->writeSeq(hwp, 0x46, clock & 0xFF);
    }

    ViaSeqMask(hwp, 0x40, 0x02, 0x02);
    ViaSeqMask(hwp, 0x40, 0x00, 0x02);
}

/* 
 *
 */
static void
ViaSetSecondaryDotclock(ScrnInfoPtr pScrn, CARD32 clock)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO,
                     "ViaSetSecondaryDotclock to 0x%06x\n", (unsigned)clock));

    if ((pVia->Chipset == VIA_CLE266) || (pVia->Chipset == VIA_KM400)) {
        hwp->writeSeq(hwp, 0x44, clock >> 8);
        hwp->writeSeq(hwp, 0x45, clock & 0xFF);
    } else {  /* unichrome pro */
        hwp->writeSeq(hwp, 0x4A, clock >> 16);
        hwp->writeSeq(hwp, 0x4B, (clock >> 8) & 0xFF);
        hwp->writeSeq(hwp, 0x4C, clock & 0xFF);
    }

    ViaSeqMask(hwp, 0x40, 0x04, 0x04);
    ViaSeqMask(hwp, 0x40, 0x00, 0x04);
}

/*
 * Broken, only does native mode decently. I (Luc) personally broke this.
 */
static void
VIASetLCDMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;
    VIALCDModeTableRec Table = lcdTable[pBIOSInfo->PanelIndex];
    CARD8 modeNum = 0;
    int resIdx;
    int port, offset, data;
    int i, j, misc;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIASetLCDMode\n"));

    if (pBIOSInfo->PanelSize == VIA_PANEL12X10)
        hwp->writeCrtc(hwp, 0x89, 0x07);

    /* LCD Expand Mode Y Scale Flag */
    pBIOSInfo->scaleY = FALSE;

    /* Set LCD InitTb Regs */
    if (pBIOSInfo->BusWidth == VIA_DI_12BIT) {
        if (pVia->IsSecondary)
            pBIOSInfo->Clock = Table.InitTb.LCDClk_12Bit;
        else {
            pBIOSInfo->Clock = Table.InitTb.VClk_12Bit;
            /* for some reason still to be defined this is neccessary */
            ViaSetSecondaryDotclock(pScrn, Table.InitTb.LCDClk_12Bit);
        }
    } else {
        if (pVia->IsSecondary)
            pBIOSInfo->Clock = Table.InitTb.LCDClk;
        else {
            pBIOSInfo->Clock = Table.InitTb.VClk;
            ViaSetSecondaryDotclock(pScrn, Table.InitTb.LCDClk);
        }

    }

    ViaSetUseExternalClock(hwp);

    for (i = 0; i < Table.InitTb.numEntry; i++) {
        port = Table.InitTb.port[i];
        offset = Table.InitTb.offset[i];
        data = Table.InitTb.data[i];
        ViaVgahwWrite(hwp, 0x300 + port, offset, 0x301 + port, data);
    }

    if ((mode->CrtcHDisplay != pBIOSInfo->panelX)
        || (mode->CrtcVDisplay != pBIOSInfo->panelY)) {
        VIALCDModeEntryPtr Main;
        VIALCDMPatchEntryPtr Patch1, Patch2;
        int numPatch1, numPatch2;

        resIdx = VIA_RES_INVALID;

        /* Find MxxxCtr & MxxxExp Index and
         * HWCursor Y Scale (PanelSize Y / Res. Y) */
        pBIOSInfo->resY = mode->CrtcVDisplay;
        switch (pBIOSInfo->ResolutionIndex) {
            case VIA_RES_640X480:
                resIdx = 0;
                break;
            case VIA_RES_800X600:
                resIdx = 1;
                break;
            case VIA_RES_1024X768:
                resIdx = 2;
                break;
            case VIA_RES_1152X864:
                resIdx = 3;
                break;
            case VIA_RES_1280X768:
            case VIA_RES_1280X960:
            case VIA_RES_1280X1024:
                if (pBIOSInfo->PanelSize == VIA_PANEL12X10)
                    resIdx = VIA_RES_INVALID;
                else
                    resIdx = 4;
                break;
            default:
                resIdx = VIA_RES_INVALID;
                break;
        }

        if ((mode->CrtcHDisplay == 640) && (mode->CrtcVDisplay == 400))
            resIdx = 0;

        if (resIdx == VIA_RES_INVALID) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "VIASetLCDMode: Failed "
                       "to find a suitable Panel Size index.\n");
            return;
        }

        if (pBIOSInfo->Center) {
            Main = &(Table.MCtr[resIdx]);
            Patch1 = Table.MPatchDP1Ctr;
            numPatch1 = Table.numMPatchDP1Ctr;
            Patch2 = Table.MPatchDP2Ctr;
            numPatch2 = Table.numMPatchDP2Ctr;
        } else {  /* expand! */
            /* LCD Expand Mode Y Scale Flag */
            pBIOSInfo->scaleY = TRUE;
            Main = &(Table.MExp[resIdx]);
            Patch1 = Table.MPatchDP1Exp;
            numPatch1 = Table.numMPatchDP1Exp;
            Patch2 = Table.MPatchDP2Exp;
            numPatch2 = Table.numMPatchDP2Exp;
        }

        /* Set Main LCD Registers */
        for (i = 0; i < Main->numEntry; i++) {
            ViaVgahwWrite(hwp, 0x300 + Main->port[i], Main->offset[i],
                          0x301 + Main->port[i], Main->data[i]);
        }

        if (pBIOSInfo->BusWidth == VIA_DI_12BIT) {
            if (pVia->IsSecondary)
                pBIOSInfo->Clock = Main->LCDClk_12Bit;
            else
                pBIOSInfo->Clock = Main->VClk_12Bit;
        } else {
            if (pVia->IsSecondary)
                pBIOSInfo->Clock = Main->LCDClk;
            else
                pBIOSInfo->Clock = Main->VClk;
        }

        j = ViaGetVesaMode(pScrn, mode);
        if (j == 0xFFFF) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "VIASetLCDMode: "
                       "Unable to determine matching VESA modenumber.\n");
            return;
        }
        for (i = 0; i < modeFix.numEntry; i++) {
            if (modeFix.reqMode[i] == j) {
                modeNum = modeFix.fixMode[i];
                break;
            }
        }

        /* Set LCD Mode patch registers. */
        for (i = 0; i < numPatch2; i++, Patch2++) {
            if (Patch2->Mode == modeNum) {
                if (!pBIOSInfo->Center
                    && (mode->CrtcHDisplay == pBIOSInfo->panelX))
                    pBIOSInfo->scaleY = FALSE;

                for (j = 0; j < Patch2->numEntry; j++) {
                    ViaVgahwWrite(hwp, 0x300 + Patch2->port[j],
                                  Patch2->offset[j], 0x301 + Patch2->port[j],
                                  Patch2->data[j]);
                }

                if (pBIOSInfo->BusWidth == VIA_DI_12BIT) {
                    if (pVia->IsSecondary)
                        pBIOSInfo->Clock = Patch2->LCDClk_12Bit;
                    else
                        pBIOSInfo->Clock = Patch2->VClk_12Bit;
                } else {
                    if (pVia->IsSecondary)
                        pBIOSInfo->Clock = Patch2->LCDClk;
                    else
                        pBIOSInfo->Clock = Patch2->VClk;
                }
                break;
            }
        }

        /* Set LCD Secondary Mode Patch registers. */
        if (pVia->IsSecondary) {
            for (i = 0; i < numPatch1; i++, Patch1++) {
                if (Patch1->Mode == modeNum) {
                    for (j = 0; j < Patch1->numEntry; j++) {
                        ViaVgahwWrite(hwp, 0x300 + Patch1->port[j],
                                      Patch1->offset[j],
                                      0x301 + Patch1->port[j], Patch1->data[j]);
                    }
                    break;
                }
            }
        }
    }

    /* LCD patch 3D5.02 */
    misc = hwp->readCrtc(hwp, 0x01);
    hwp->writeCrtc(hwp, 0x02, misc);

    /* Enable LCD */
    if (!pVia->IsSecondary) {
        /* CRT Display Source Bit 6 - 0: CRT, 1: LCD */
        ViaSeqMask(hwp, 0x16, 0x40, 0x40);

        /* Enable Simultaneous */
        if (pBIOSInfo->BusWidth == VIA_DI_12BIT) {
            hwp->writeCrtc(hwp, 0x6B, 0xA8);

            if ((pVia->Chipset == VIA_CLE266)
                && CLE266_REV_IS_AX(pVia->ChipRev))
                hwp->writeCrtc(hwp, 0x93, 0xB1);
            else
                hwp->writeCrtc(hwp, 0x93, 0xAF);
        } else {
            ViaCrtcMask(hwp, 0x6B, 0x08, 0x08);
            hwp->writeCrtc(hwp, 0x93, 0x00);
        }
        hwp->writeCrtc(hwp, 0x6A, 0x48);
    } else {
        /* CRT Display Source Bit 6 - 0: CRT, 1: LCD */
        ViaSeqMask(hwp, 0x16, 0x00, 0x40);

        /* Enable SAMM */
        if (pBIOSInfo->BusWidth == VIA_DI_12BIT) {
            ViaCrtcMask(hwp, 0x6B, 0x20, 0x20);
            if ((pVia->Chipset == VIA_CLE266)
                && CLE266_REV_IS_AX(pVia->ChipRev))
                hwp->writeCrtc(hwp, 0x93, 0xB1);
            else
                hwp->writeCrtc(hwp, 0x93, 0xAF);
        } else {
            hwp->writeCrtc(hwp, 0x6B, 0x00);
            hwp->writeCrtc(hwp, 0x93, 0x00);
        }
        hwp->writeCrtc(hwp, 0x6A, 0xC8);
    }
}

/*
 *
 */
static void
ViaModePrimaryVGA(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD16 temp;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModePrimaryVGA\n"));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModePrimaryVGA: "
                     "Setting up %s\n", mode->name));

    ViaCrtcMask(hwp, 0x11, 0x00, 0x80); /* modify starting address */
    ViaCrtcMask(hwp, 0x03, 0x80, 0x80); /* enable vertical retrace access */
    hwp->writeSeq(hwp, 0x10, 0x01);     /* unlock extended registers */
    ViaCrtcMask(hwp, 0x47, 0x00, 0x01); /* unlock CRT registers */

    /* Set Misc Register */
    temp = 0x23;
    if (mode->Flags & V_NHSYNC)
        temp |= 0x40;
    if (mode->Flags & V_NVSYNC)
        temp |= 0x80;
    temp |= 0x0C;  /* undefined/external clock */
    hwp->writeMiscOut(hwp, temp);

    /* Sequence registers */
    hwp->writeSeq(hwp, 0x00, 0x00);

    /* if (mode->Flags & V_CLKDIV2)
     * hwp->writeSeq(hwp, 0x01, 0x09);
     * else */
    hwp->writeSeq(hwp, 0x01, 0x01);

    hwp->writeSeq(hwp, 0x02, 0x0F);
    hwp->writeSeq(hwp, 0x03, 0x00);
    hwp->writeSeq(hwp, 0x04, 0x0E);

    ViaSeqMask(hwp, 0x15, 0x02, 0x02);

    /* bpp */
    switch (pScrn->bitsPerPixel) {
        case 8:
            ViaSeqMask(hwp, 0x15, 0x20, 0xFC);
            break;
        case 16:
            ViaSeqMask(hwp, 0x15, 0xB4, 0xFC);
            break;
        case 24:
        case 32:
            ViaSeqMask(hwp, 0x15, 0xAC, 0xFC);
            break;
        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unhandled bitdepth: %d\n",
                       pScrn->bitsPerPixel);
            break;
    }

    ViaSeqMask(hwp, 0x16, 0x08, 0xBF);
    ViaSeqMask(hwp, 0x17, 0x1F, 0xFF);
    ViaSeqMask(hwp, 0x18, 0x4E, 0xFF);
    ViaSeqMask(hwp, 0x1A, 0x08, 0xFD);

    /* graphics registers */
    hwp->writeGr(hwp, 0x00, 0x00);
    hwp->writeGr(hwp, 0x01, 0x00);
    hwp->writeGr(hwp, 0x02, 0x00);
    hwp->writeGr(hwp, 0x03, 0x00);
    hwp->writeGr(hwp, 0x04, 0x00);
    hwp->writeGr(hwp, 0x05, 0x40);
    hwp->writeGr(hwp, 0x06, 0x05);
    hwp->writeGr(hwp, 0x07, 0x0F);
    hwp->writeGr(hwp, 0x08, 0xFF);

    ViaGrMask(hwp, 0x20, 0, 0xFF);
    ViaGrMask(hwp, 0x21, 0, 0xFF);
    ViaGrMask(hwp, 0x22, 0, 0xFF);

    /* attribute registers */
    hwp->writeAttr(hwp, 0x00, 0x00);
    hwp->writeAttr(hwp, 0x01, 0x01);
    hwp->writeAttr(hwp, 0x02, 0x02);
    hwp->writeAttr(hwp, 0x03, 0x03);
    hwp->writeAttr(hwp, 0x04, 0x04);
    hwp->writeAttr(hwp, 0x05, 0x05);
    hwp->writeAttr(hwp, 0x06, 0x06);
    hwp->writeAttr(hwp, 0x07, 0x07);
    hwp->writeAttr(hwp, 0x08, 0x08);
    hwp->writeAttr(hwp, 0x09, 0x09);
    hwp->writeAttr(hwp, 0x0A, 0x0A);
    hwp->writeAttr(hwp, 0x0B, 0x0B);
    hwp->writeAttr(hwp, 0x0C, 0x0C);
    hwp->writeAttr(hwp, 0x0D, 0x0D);
    hwp->writeAttr(hwp, 0x0E, 0x0E);
    hwp->writeAttr(hwp, 0x0F, 0x0F);
    hwp->writeAttr(hwp, 0x10, 0x41);
    hwp->writeAttr(hwp, 0x11, 0xFF);
    hwp->writeAttr(hwp, 0x12, 0x0F);
    hwp->writeAttr(hwp, 0x13, 0x00);
    hwp->writeAttr(hwp, 0x14, 0x00);

    /* Crtc registers */
    /* horizontal total : 4100 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHTotal: 0x%03X\n",
                     mode->CrtcHTotal));
    temp = (mode->CrtcHTotal >> 3) - 5;
    hwp->writeCrtc(hwp, 0x00, temp & 0xFF);
    ViaCrtcMask(hwp, 0x36, temp >> 5, 0x08);

    /* horizontal address : 2048 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHDisplay: 0x%03X\n",
                     mode->CrtcHDisplay));
    temp = (mode->CrtcHDisplay >> 3) - 1;
    hwp->writeCrtc(hwp, 0x01, temp & 0xFF);

    /* horizontal blanking start : 2048 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHBlankStart: 0x%03X\n",
                     mode->CrtcHBlankStart));
    if (mode->CrtcHBlankStart != mode->CrtcHDisplay)  /* FIXME */
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Caught X working around "
                   "an old VGA limitation (HBlankStart).\n");
    temp = (mode->CrtcHDisplay >> 3) - 1;
    hwp->writeCrtc(hwp, 0x02, temp & 0xFF);
    /* If HblankStart has more bits anywhere, add them here */

    /* horizontal blanking end : start + 1025 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHBlankEnd: 0x%03X\n",
                     mode->CrtcHBlankEnd));
    if (mode->CrtcHBlankEnd != mode->CrtcHTotal)  /* FIXME */
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Caught X working around "
                   "an old VGA limitation (HBlankEnd).\n");
    temp = (mode->CrtcHTotal >> 3) - 1;
    ViaCrtcMask(hwp, 0x03, temp, 0x1F);
    ViaCrtcMask(hwp, 0x05, temp << 2, 0x80);
    ViaCrtcMask(hwp, 0x33, temp >> 1, 0x20);

    /* CrtcHSkew ??? */

    /* horizontal sync start : 4095 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHSyncStart: 0x%03X\n",
                     mode->CrtcHSyncStart));
    temp = mode->CrtcHSyncStart >> 3;
    hwp->writeCrtc(hwp, 0x04, temp & 0xFF);
    ViaCrtcMask(hwp, 0x33, temp >> 4, 0x10);

    /* horizontal sync end : start + 256 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHSyncEnd: 0x%03X\n",
                     mode->CrtcHSyncEnd));
    temp = mode->CrtcHSyncEnd >> 3;
    ViaCrtcMask(hwp, 0x05, temp, 0x1F);

    /* vertical total : 2049 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVTotal: 0x%03X\n",
                     mode->CrtcVTotal));
    temp = mode->CrtcVTotal - 2;
    hwp->writeCrtc(hwp, 0x06, temp & 0xFF);
    ViaCrtcMask(hwp, 0x07, temp >> 8, 0x01);
    ViaCrtcMask(hwp, 0x07, temp >> 4, 0x20);
    ViaCrtcMask(hwp, 0x35, temp >> 10, 0x01);

    /* vertical address : 2048 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVDisplay: 0x%03X\n",
                     mode->CrtcVDisplay));
    temp = mode->CrtcVDisplay - 1;
    hwp->writeCrtc(hwp, 0x12, temp & 0xFF);
    ViaCrtcMask(hwp, 0x07, temp >> 7, 0x02);
    ViaCrtcMask(hwp, 0x07, temp >> 3, 0x40);
    ViaCrtcMask(hwp, 0x35, temp >> 8, 0x04);

    /* Primary starting address -> 0x00, adjustframe does the rest */
    hwp->writeCrtc(hwp, 0x0C, 0x00);
    hwp->writeCrtc(hwp, 0x0D, 0x00);
    hwp->writeCrtc(hwp, 0x34, 0x00);
    ViaCrtcMask(hwp, 0x48, 0x00, 0x03); /* is this even possible on CLE266A ? */

    /* vertical sync start : 2047 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVSyncStart: 0x%03X\n",
                     mode->CrtcVSyncStart));
    temp = mode->CrtcVSyncStart;
    hwp->writeCrtc(hwp, 0x10, temp & 0xFF);
    ViaCrtcMask(hwp, 0x07, temp >> 6, 0x04);
    ViaCrtcMask(hwp, 0x07, temp >> 2, 0x80);
    ViaCrtcMask(hwp, 0x35, temp >> 9, 0x02);

    /* vertical sync end : start + 16 -- other bits someplace? */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVSyncEnd: 0x%03X\n",
                     mode->CrtcVSyncEnd));
    ViaCrtcMask(hwp, 0x11, mode->CrtcVSyncEnd, 0x0F);

    /* line compare: We are not doing splitscreen so 0x3FFF */
    hwp->writeCrtc(hwp, 0x18, 0xFF);
    ViaCrtcMask(hwp, 0x07, 0x10, 0x10);
    ViaCrtcMask(hwp, 0x09, 0x40, 0x40);
    ViaCrtcMask(hwp, 0x33, 0x07, 0x06);
    ViaCrtcMask(hwp, 0x35, 0x10, 0x10);

    /* zero Maximum scan line */
    ViaCrtcMask(hwp, 0x09, 0x00, 0x1F);
    hwp->writeCrtc(hwp, 0x14, 0x00);

    /* vertical blanking start : 2048 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVBlankStart: 0x%03X\n",
                     mode->CrtcVBlankStart));
    if (mode->CrtcVBlankStart != mode->CrtcVDisplay)  /* FIXME */
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Caught X working around "
                   "an old VGA limitation (VBlankStart).\n");
    temp = mode->CrtcVDisplay - 1;
    hwp->writeCrtc(hwp, 0x15, temp & 0xFF);
    ViaCrtcMask(hwp, 0x07, temp >> 5, 0x08);
    ViaCrtcMask(hwp, 0x09, temp >> 4, 0x20);
    ViaCrtcMask(hwp, 0x35, temp >> 7, 0x08);

    /* vertical blanking end : start + 257 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVBlankEnd: 0x%03X\n",
                     mode->CrtcVBlankEnd));
    if (mode->CrtcVBlankEnd != mode->CrtcVTotal)  /* FIXME */
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Caught X working around "
                   "an old VGA limitation (VBlankEnd).\n");
    temp = mode->CrtcVTotal - 1;
    hwp->writeCrtc(hwp, 0x16, temp);

    /* some leftovers */
    hwp->writeCrtc(hwp, 0x08, 0x00);
    ViaCrtcMask(hwp, 0x32, 0, 0xFF);  /* ? */
    ViaCrtcMask(hwp, 0x33, 0, 0xC8);

    /* offset */
    temp = (pScrn->displayWidth * (pScrn->bitsPerPixel >> 3)) >> 3;
    /* Make sure that this is 32byte aligned */
    if (temp & 0x03) {
        temp += 0x03;
        temp &= ~0x03;
    }
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Offset: 0x%03X\n", temp));
    hwp->writeCrtc(hwp, 0x13, temp & 0xFF);
    ViaCrtcMask(hwp, 0x35, temp >> 3, 0xE0);

    /* fetch count */
    temp = (mode->CrtcHDisplay * (pScrn->bitsPerPixel >> 3)) >> 3;
    /* Make sure that this is 32byte aligned */
    if (temp & 0x03) {
        temp += 0x03;
        temp &= ~0x03;
    }
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Fetch Count: 0x%03X\n", temp));
    hwp->writeSeq(hwp, 0x1C, (temp >> 1) & 0xFF);
    ViaSeqMask(hwp, 0x1D, temp >> 9, 0x03);

    /* some leftovers */
    ViaCrtcMask(hwp, 0x32, 0, 0xFF);
    ViaCrtcMask(hwp, 0x33, 0, 0xC8);
}

static CARD32
ViaComputeDotClock(unsigned clock)
{
    double fvco, fout, fref, err, minErr;
    CARD32 dr, dn, dm, maxdm, maxdn;
    CARD32 factual, best;

    fref = 14.31818e6;
    fout = (double)clock * 1.e3;

    factual = ~0;
    maxdm = 127;
    maxdn = 7;
    minErr = 1e10;
    best = 0;

    for (dr = 0; dr < 4; ++dr) {
        for (dn = (dr == 0) ? 2 : 1; dn <= maxdn; ++dn) {
            for (dm = 1; dm <= maxdm; ++dm) {
                factual = fref * dm;
                factual /= (dn << dr);
                err = fabs((double)factual / fout - 1.);
                if (err < minErr) {
                    minErr = err;
                    best = (dm & 127) | ((dn & 31) << 8) | (dr << 14);
                }
            }
        }
    }
    return best;
}

static CARD32
ViaComputeProDotClock(unsigned clock)
{
    double fvco, fout, fref, err, minErr;
    CARD32 dr = 0, dn, dm, maxdm, maxdn;
    CARD32 factual, bestClock;

    fref = 14.318e6;
    fout = (double)clock * 1.e3;

    factual = ~0;
    maxdm = factual / 14318000U - 2;
    minErr = 1.e10;
    bestClock = 0U;

    do {
        fvco = fout * (1 << dr);
    } while (fvco < 300.e6 && dr++ < 8);

    if (dr == 8) {
        return 0;
    }

    if (clock < 30000)
        maxdn = 6;
    else if (clock < 45000)
        maxdn = 5;
    else if (clock < 170000)
        maxdn = 4;
    else
        maxdn = 3;

    for (dn = 0; dn < maxdn; ++dn) {
        for (dm = 0; dm < maxdm; ++dm) {
            factual = 14318000U * (dm + 2);
            factual /= (dn + 2) << dr;
            if ((err = fabs((double)factual / fout - 1.)) < 0.005) {
                if (err < minErr) {
                    minErr = err;
                    bestClock = ((dm & 0xff) << 16) |
                            (((1 << 7) | (dr << 2) | ((dm & 0x300) >> 8)) << 8)
                            | (dn & 0x7f);
                }
            }
        }
    }

    return bestClock;
}

/*
 *
 */
static CARD32
ViaModeDotClockTranslate(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIAPtr pVia = VIAPTR(pScrn);
    int i;

    if ((pVia->Chipset == VIA_CLE266) || (pVia->Chipset == VIA_KM400)) {
        CARD32 best1 = 0, best2;

        for (i = 0; ViaDotClocks[i].DotClock; i++)
            if (ViaDotClocks[i].DotClock == mode->Clock) {
                best1 = ViaDotClocks[i].UniChrome;
                break;
            }

        best2 = ViaComputeDotClock(mode->Clock);

        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                         "ViaComputeDotClock %d : %04x : %04x\n",
                         mode->Clock, best1, best2));
        return best2;
    } else {
        for (i = 0; ViaDotClocks[i].DotClock; i++)
            if (ViaDotClocks[i].DotClock == mode->Clock)
                return ViaDotClocks[i].UniChromePro;
        return ViaComputeProDotClock(mode->Clock);
    }

    return 0;
}

/*
 *
 */
void
ViaModePrimary(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModePrimary\n"));

    /* Turn off Screen */
    ViaCrtcMask(hwp, 0x17, 0x00, 0x80);

    /* Clean Second Path Status */
    hwp->writeCrtc(hwp, 0x6A, 0x00);
    hwp->writeCrtc(hwp, 0x6B, 0x00);
    hwp->writeCrtc(hwp, 0x6C, 0x00);
    hwp->writeCrtc(hwp, 0x93, 0x00);

    ViaModePrimaryVGA(pScrn, mode);
    pBIOSInfo->Clock = ViaModeDotClockTranslate(pScrn, mode);
    pBIOSInfo->ClockExternal = FALSE;

    /* Enable MMIO & PCI burst (1 wait state) */
    ViaSeqMask(hwp, 0x1A, 0x06, 0x06);

    if (!pBIOSInfo->CrtActive)
        ViaCrtcMask(hwp, 0x36, 0x30, 0x30);
    else
        ViaSeqMask(hwp, 0x16, 0x00, 0x40);

    if (pBIOSInfo->PanelActive && ViaPanelGetIndex(pScrn, mode)) {
        VIASetLCDMode(pScrn, mode);
        ViaLCDPower(pScrn, TRUE);
    } else if (pBIOSInfo->PanelPresent)
        ViaLCDPower(pScrn, FALSE);

    if (pBIOSInfo->TVActive) {
        /* Quick 'n dirty workaround for non-primary case until TVCrtcMode
         * is removed -- copy from clock handling code below */
        if ((pVia->Chipset == VIA_CLE266) && CLE266_REV_IS_AX(pVia->ChipRev))
            ViaSetPrimaryDotclock(pScrn, 0x471C);  /* CLE266Ax uses 2x XCLK */
        else if (pVia->Chipset != VIA_CLE266 && pVia->Chipset != VIA_KM400)
            ViaSetPrimaryDotclock(pScrn, 0x529001);
        else
            ViaSetPrimaryDotclock(pScrn, 0x871C);
        ViaSetUseExternalClock(hwp);

        ViaTVSetMode(pScrn, mode);
    } else
        ViaTVPower(pScrn, FALSE);

    ViaSetPrimaryFIFO(pScrn, mode);

    if (pBIOSInfo->ClockExternal) {
        if ((pVia->Chipset == VIA_CLE266) && CLE266_REV_IS_AX(pVia->ChipRev))
            ViaSetPrimaryDotclock(pScrn, 0x471C);  /* CLE266Ax uses 2x XCLK */
        else if (pVia->Chipset != VIA_CLE266 && pVia->Chipset != VIA_KM400)
            ViaSetPrimaryDotclock(pScrn, 0x529001);
        else
            ViaSetPrimaryDotclock(pScrn, 0x871C);
        if (pVia->Chipset == VIA_CLE266 || pVia->Chipset == VIA_KM400)
            ViaCrtcMask(hwp, 0x6B, 0x01, 0x01);
    } else {
        ViaSetPrimaryDotclock(pScrn, pBIOSInfo->Clock);
        ViaSetUseExternalClock(hwp);
        ViaCrtcMask(hwp, 0x6B, 0x00, 0x01);
    }

    /* Enable CRT Controller (3D5.17 Hardware Reset) */
    ViaCrtcMask(hwp, 0x17, 0x80, 0x80);

    hwp->disablePalette(hwp);
}

void
ViaModeSecondaryVGAFetchCount(ScrnInfoPtr pScrn, int width)
{

    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD16 temp;

    /* fetch count */
    temp = (width * (pScrn->bitsPerPixel >> 3)) >> 3;
    /* Make sure that this is 32byte aligned */
    if (temp & 0x03) {
        temp += 0x03;
        temp &= ~0x03;
    }
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Fetch Count: 0x%03X\n", temp));
    hwp->writeCrtc(hwp, 0x65, (temp >> 1) & 0xFF);
    ViaCrtcMask(hwp, 0x67, temp >> 7, 0x0C);
}

void
ViaModeSecondaryVGAOffset(ScrnInfoPtr pScrn)
{

    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD16 temp;

    /* offset */
    temp = (pScrn->displayWidth * (pScrn->bitsPerPixel >> 3)) >> 3;
    if (temp & 0x03) {  /* Make sure that this is 32byte aligned */
        temp += 0x03;
        temp &= ~0x03;
    }
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Offset: 0x%03X\n", temp));
    hwp->writeCrtc(hwp, 0x66, temp & 0xFF);
    ViaCrtcMask(hwp, 0x67, temp >> 8, 0x03);

}

/*
 *
 */
static void
ViaModeSecondaryVGA(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD16 temp;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModeSecondaryVGA\n"));

    /* bpp */
    switch (pScrn->bitsPerPixel) {
        case 8:
            ViaCrtcMask(hwp, 0x67, 0x00, 0xC0);
            break;
        case 16:
            ViaCrtcMask(hwp, 0x67, 0x40, 0xC0);
            break;
        case 24:
        case 32:
            ViaCrtcMask(hwp, 0x67, 0x80, 0xC0);
            break;
        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unhandled bitdepth: %d\n",
                       pScrn->bitsPerPixel);
            break;
    }

    /* Crtc registers */
    /* horizontal total : 4096 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHTotal: 0x%03X\n",
                     mode->CrtcHTotal));
    temp = mode->CrtcHTotal - 1;
    hwp->writeCrtc(hwp, 0x50, temp & 0xFF);
    ViaCrtcMask(hwp, 0x55, temp >> 8, 0x0F);

    /* horizontal address : 2048 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHDisplay: 0x%03X\n",
                     mode->CrtcHDisplay));
    temp = mode->CrtcHDisplay - 1;
    hwp->writeCrtc(hwp, 0x51, temp & 0xFF);
    ViaCrtcMask(hwp, 0x55, temp >> 4, 0x70);

    /* horizontal blanking start : 2048 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHBlankStart: 0x%03X\n",
                     mode->CrtcHBlankStart));
    if (mode->CrtcHBlankStart != mode->CrtcHDisplay)  /* FIXME */
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Caught X working around "
                   "an old VGA limitation (HBlankStart).\n");
    temp = mode->CrtcHDisplay - 1;
    hwp->writeCrtc(hwp, 0x52, temp & 0xFF);
    ViaCrtcMask(hwp, 0x54, temp >> 8, 0x07);

    /* horizontal blanking end : 4096 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHBlankEnd: 0x%03X\n",
                     mode->CrtcHBlankEnd));
    if (mode->CrtcHBlankEnd != mode->CrtcHTotal)  /* FIXME */
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Caught X working around "
                   "an old VGA limitation (HBlankEnd).\n");
    temp = mode->CrtcHTotal - 1;
    hwp->writeCrtc(hwp, 0x53, temp & 0xFF);
    ViaCrtcMask(hwp, 0x54, temp >> 5, 0x38);
    ViaCrtcMask(hwp, 0x5D, temp >> 5, 0x40);

    /* horizontal sync start : 2047 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHSyncStart: 0x%03X\n",
                     mode->CrtcHSyncStart));
    temp = mode->CrtcHSyncStart;
    hwp->writeCrtc(hwp, 0x56, temp & 0xFF);
    ViaCrtcMask(hwp, 0x54, temp >> 2, 0xC0);
    ViaCrtcMask(hwp, 0x5C, temp >> 3, 0x80);

    /* horizontal sync end : sync start + 512 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHSyncEnd: 0x%03X\n",
                     mode->CrtcHSyncEnd));
    temp = mode->CrtcHSyncEnd;
    hwp->writeCrtc(hwp, 0x57, temp & 0xFF);
    ViaCrtcMask(hwp, 0x5C, temp >> 2, 0x40);

    /* vertical total : 2048 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVTotal: 0x%03X\n",
                     mode->CrtcVTotal));
    temp = mode->CrtcVTotal - 1;
    hwp->writeCrtc(hwp, 0x58, temp & 0xFF);
    ViaCrtcMask(hwp, 0x5D, temp >> 8, 0x07);

    /* vertical address : 2048 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVDisplay: 0x%03X\n",
                     mode->CrtcVDisplay));
    temp = mode->CrtcVDisplay - 1;
    hwp->writeCrtc(hwp, 0x59, temp & 0xFF);
    ViaCrtcMask(hwp, 0x5D, temp >> 5, 0x38);

    /* vertical blanking start : 2048 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVBlankStart: 0x%03X\n",
                     mode->CrtcVBlankStart));
    if (mode->CrtcVBlankStart != mode->CrtcVDisplay)  /* FIXME */
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Caught X working around "
                   "an old VGA limitation (VBlankStart).\n");
    temp = mode->CrtcVDisplay - 1;
    hwp->writeCrtc(hwp, 0x5A, temp & 0xFF);
    ViaCrtcMask(hwp, 0x5C, temp >> 8, 0x07);

    /* vertical blanking end : 2048 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVBlankEnd: 0x%03X\n",
                     mode->CrtcVBlankEnd));
    if (mode->CrtcVBlankEnd != mode->CrtcVTotal)  /* FIXME */
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Caught X working around "
                   "an old VGA limitation (VBlankEnd).\n");
    temp = mode->CrtcVTotal - 1;
    hwp->writeCrtc(hwp, 0x5B, temp & 0xFF);
    ViaCrtcMask(hwp, 0x5C, temp >> 5, 0x38);

    /* vertical sync start : 2047 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVSyncStart: 0x%03X\n",
                     mode->CrtcVSyncStart));
    temp = mode->CrtcVSyncStart;
    hwp->writeCrtc(hwp, 0x5E, temp & 0xFF);
    ViaCrtcMask(hwp, 0x5F, temp >> 3, 0xE0);

    /* vertical sync end : start + 32 */
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVSyncEnd: 0x%03X\n",
                     mode->CrtcVSyncEnd));
    temp = mode->CrtcVSyncEnd;
    ViaCrtcMask(hwp, 0x5F, temp, 0x1F);

    ViaModeSecondaryVGAOffset(pScrn);
    ViaModeSecondaryVGAFetchCount(pScrn, mode->CrtcHDisplay);

}

/*
 *
 */
void
ViaModeSecondary(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModeSecondary\n"));

    /* Turn off Screen */
    ViaCrtcMask(hwp, 0x17, 0x00, 0x80);

    ViaModeSecondaryVGA(pScrn, mode);

    if (pBIOSInfo->TVActive)
        ViaTVSetMode(pScrn, mode);

    /* CLE266A2 apparently doesn't like this */
    if (!(pVia->Chipset == VIA_CLE266 && pVia->ChipRev == 0x02))
        ViaCrtcMask(hwp, 0x6C, 0x00, 0x1E);

    if (pBIOSInfo->PanelActive
        && (pBIOSInfo->PanelIndex != VIA_BIOS_NUM_PANEL)) {
        pBIOSInfo->SetDVI = TRUE;
        VIASetLCDMode(pScrn, mode);
        ViaLCDPower(pScrn, TRUE);
    } else if (pBIOSInfo->PanelPresent)
        ViaLCDPower(pScrn, FALSE);

    ViaSetSecondaryFIFO(pScrn, mode);

    ViaSetSecondaryDotclock(pScrn, pBIOSInfo->Clock);
    ViaSetUseExternalClock(hwp);

    ViaCrtcMask(hwp, 0x17, 0x80, 0x80);

    hwp->disablePalette(hwp);
}

/*
 *
 */
static void
ViaLCDPowerSequence(vgaHWPtr hwp, VIALCDPowerSeqRec Sequence)
{
    int i;

    for (i = 0; i < Sequence.numEntry; i++) {
        ViaVgahwMask(hwp, 0x300 + Sequence.port[i], Sequence.offset[i],
                     0x301 + Sequence.port[i], Sequence.data[i],
                     Sequence.mask[i]);
        usleep(Sequence.delay[i]);
    }
}

/*
 *
 */
void
ViaLCDPower(ScrnInfoPtr pScrn, Bool On)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;
    int i;

#ifdef HAVE_DEBUG
    if (On)
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaLCDPower: On.\n");
    else
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaLCDPower: Off.\n");
#endif

    /* Enable LCD */
    if (On)
        ViaCrtcMask(hwp, 0x6A, 0x08, 0x08);
    else
        ViaCrtcMask(hwp, 0x6A, 0x00, 0x08);

    /* Find Panel Size Index for PowerSeq Table */
    if (pVia->Chipset == VIA_CLE266) {
        if (pBIOSInfo->PanelSize != VIA_PANEL_INVALID) {
            for (i = 0; i < NumPowerOn; i++) {
                if (lcdTable[pBIOSInfo->PanelIndex].powerSeq
                    == powerOn[i].powerSeq)
                    break;
            }
        } else
            i = 0;
    } else
        /* KM and K8M use PowerSeq Table index 2. */
        i = 2;

    usleep(1);
    if (On)
        ViaLCDPowerSequence(hwp, powerOn[i]);
    else
        ViaLCDPowerSequence(hwp, powerOff[i]);
    usleep(1);
}
