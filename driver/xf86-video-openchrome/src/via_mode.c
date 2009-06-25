/*
 * Copyright 2005-2007 The Openchrome Project [openchrome.org]
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

static void
ViaPrintMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Name: %s\n", mode->name);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Clock: %d\n", mode->Clock);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VRefresh: %f\n", mode->VRefresh);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "HSync: %f\n", mode->HSync);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "HDisplay: %d\n", mode->HDisplay);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "HSyncStart: %d\n", mode->HSyncStart);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "HSyncEnd: %d\n", mode->HSyncEnd);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "HTotal: %d\n", mode->HTotal);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "HSkew: %d\n", mode->HSkew);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VDisplay: %d\n", mode->VDisplay);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VSyncStart: %d\n", mode->VSyncStart);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VSyncEnd: %d\n", mode->VSyncEnd);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VTotal: %d\n", mode->VTotal);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VScan: %d\n", mode->VScan);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Flags: %d\n", mode->Flags);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHDisplay: 0x%x\n",
               mode->CrtcHDisplay);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHBlankStart: 0x%x\n",
               mode->CrtcHBlankStart);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHSyncStart: 0x%x\n",
               mode->CrtcHSyncStart);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHSyncEnd: 0x%x\n",
               mode->CrtcHSyncEnd);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHBlankEnd: 0x%x\n",
               mode->CrtcHBlankEnd);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHTotal: 0x%x\n",
               mode->CrtcHTotal);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcHSkew: 0x%x\n",
               mode->CrtcHSkew);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVDisplay: 0x%x\n",
               mode->CrtcVDisplay);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVBlankStart: 0x%x\n",
               mode->CrtcVBlankStart);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVSyncStart: 0x%x\n",
               mode->CrtcVSyncStart);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVSyncEnd: 0x%x\n",
               mode->CrtcVSyncEnd);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVBlankEnd: 0x%x\n",
               mode->CrtcVBlankEnd);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CrtcVTotal: 0x%x\n",
               mode->CrtcVTotal);

}

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


static Bool
ViaDFPDetect(ScrnInfoPtr pScrn)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaDFPDetect\n"));

    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;
    xf86MonPtr          monPtr = NULL;

    if (pVia->pI2CBus2)
        monPtr = xf86DoEDID_DDC2(pScrn->scrnIndex, pVia->pI2CBus2);
    
    if (monPtr) {
        xf86PrintEDID(monPtr);
        xf86SetDDCproperties(pScrn, monPtr);
        return TRUE;
    } else {
        return FALSE;
    }
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
    pBIOSInfo->DfpPresent = FALSE;
    
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
    
    if ((pVia->Chipset == VIA_CX700) || (pVia->Chipset == VIA_VX800)) {
        
        if (ViaDFPDetect(pScrn)) {
            pBIOSInfo->DfpPresent = TRUE;
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "DFP is connected.\n");
        } else {
            pBIOSInfo->DfpPresent = FALSE;
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "DFP is disconnected.\n");
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

    pBIOSInfo->Panel->IsActive = FALSE;
    pBIOSInfo->CrtActive = FALSE;
    pBIOSInfo->TVActive = FALSE;
    pBIOSInfo->DfpActive = FALSE;
    
    if (!pVia->ActiveDevice) {
        /* always enable the panel when present */
        if (pBIOSInfo->PanelPresent)
            pBIOSInfo->Panel->IsActive = TRUE;
        else if (pBIOSInfo->TVOutput != TVOUTPUT_NONE)  /* cable is attached! */
            pBIOSInfo->TVActive = TRUE;

        /* CRT can be used with everything when present */
        if (pBIOSInfo->CrtPresent)
            pBIOSInfo->CrtActive = TRUE;

#if 0
        # FIXME : DFP must be activated with the ActiveDevice option 
        /* DFP */
        if (pBIOSInfo->DfpPresent)
            pBIOSInfo->DfpActive = TRUE;
#endif
        
    } else {
        if (pVia->ActiveDevice & VIA_DEVICE_LCD) {
            if (pBIOSInfo->PanelPresent)
                pBIOSInfo->Panel->IsActive = TRUE;
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
            else if (pBIOSInfo->Panel->IsActive)
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Unable to activate"
                           " TV encoder and panel simultaneously. Not using"
                           " TV encoder.\n");
            else
                pBIOSInfo->TVActive = TRUE;
        }

        if (pVia->ActiveDevice & VIA_DEVICE_DFP) {
            pBIOSInfo->DfpPresent = TRUE;
            pBIOSInfo->DfpActive = TRUE;
        }

        if ((pVia->ActiveDevice & VIA_DEVICE_CRT)
            || (!pBIOSInfo->Panel->IsActive && !pBIOSInfo->TVActive
                && !pBIOSInfo->DfpActive)) {
            pBIOSInfo->CrtPresent = TRUE;
            pBIOSInfo->CrtActive = TRUE;
        }
    }
    if (!pVia->UseLegacyModeSwitch) {
        if (pBIOSInfo->CrtActive)
            pBIOSInfo->FirstCRTC->IsActive = TRUE ;
        if (pBIOSInfo->DfpActive)
            pBIOSInfo->FirstCRTC->IsActive = TRUE ;
        if (pBIOSInfo->Panel->IsActive) {
            pVia->pBIOSInfo->SecondCRTC->IsActive = TRUE ;
            if (pVia->Chipset == VIA_P4M900 || pVia->Chipset == VIA_CX700 || pVia->Chipset == VIA_VX800 )
                pVia->pBIOSInfo->Lvds->IsActive = TRUE ;
        }
    }

#ifdef HAVE_DEBUG
    if (pBIOSInfo->CrtActive)
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                         "ViaOutputsSelect: CRT.\n"));
    if (pBIOSInfo->Panel->IsActive)
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                         "ViaOutputsSelect: Panel.\n"));
    if (pBIOSInfo->TVActive)
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                         "ViaOutputsSelect: TV.\n"));
    if (pBIOSInfo->DfpActive)
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                         "ViaOutputsSelect: DFP.\n"));
#endif
    return TRUE; /* !Secondary always has at least CRT */
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
    char *PanelSizeString[7] = { "640x480", "800x480", "800x600", "1024x768", "1280x768"
                                 "1280x1024", "1400x1050", "1600x1200" };
    int width = 0;
    int height = 0;
    Bool ret;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAGetPanelSize\n"));

    ret = ViaPanelGetSizeFromDDCv1(pScrn, &width, &height);
    if (!ret)
        ret = ViaPanelGetSizeFromDDCv2(pScrn, &width);

    if (ret) {
        switch (width) {
            case 640:
                pBIOSInfo->Panel->NativeModeIndex = VIA_PANEL6X4;
                break;
            case 800:
		if (height == 480)
               	    pBIOSInfo->Panel->NativeModeIndex = VIA_PANEL8X4;
		else
               	    pBIOSInfo->Panel->NativeModeIndex = VIA_PANEL8X6;
                break;
            case 1024:
                pBIOSInfo->Panel->NativeModeIndex = VIA_PANEL10X7;
                break;
            case 1280:
                pBIOSInfo->Panel->NativeModeIndex = VIA_PANEL12X10;
                break;
            case 1400:
                pBIOSInfo->Panel->NativeModeIndex = VIA_PANEL14X10;
                break;
            case 1600:
                pBIOSInfo->Panel->NativeModeIndex = VIA_PANEL16X12;
                break;
            default:
                pBIOSInfo->Panel->NativeModeIndex = VIA_PANEL_INVALID;
                break;
        }
    } else {
        pBIOSInfo->Panel->NativeModeIndex = hwp->readCrtc(hwp, 0x3F) >> 4;
        if (pBIOSInfo->Panel->NativeModeIndex == 0) {
            /* VIA_PANEL6X4 == 0, but that value equals unset */
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Unable to "
                       "retrieve PanelSize: using default (1024x768)\n");
            pBIOSInfo->Panel->NativeModeIndex = VIA_PANEL10X7;
            return;
        }
    }

    if (pBIOSInfo->Panel->NativeModeIndex < 7)
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using panel at %s.\n",
                   PanelSizeString[pBIOSInfo->Panel->NativeModeIndex]);
    else
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Unknown panel size "
                   "detected: %d.\n", pBIOSInfo->Panel->NativeModeIndex);
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

    if (pBIOSInfo->Panel->NativeModeIndex == VIA_PANEL_INVALID) {
        VIAGetPanelSize(pScrn);
        if (pBIOSInfo->Panel->NativeModeIndex == VIA_PANEL_INVALID) {
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

    for (i = 0; ViaResolutionTable[i].Index != VIA_RES_INVALID; i++) {
        if (ViaResolutionTable[i].PanelIndex
            == pBIOSInfo->Panel->NativeModeIndex) {
            pBIOSInfo->panelX = ViaResolutionTable[i].X;
            pBIOSInfo->panelY = ViaResolutionTable[i].Y;
            break;
        }
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

    for (i = 0; i < VIA_BIOS_NUM_PANEL; i++) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaPanelGetIndex:"
                         "Match Debug: %d == %d)\n", pBIOSInfo->Panel->NativeModeIndex,
                         lcdTable[i].fpSize));
        if (lcdTable[i].fpSize == pBIOSInfo->Panel->NativeModeIndex) {
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
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaPanelGetIndex: Unable"
               " to match PanelSize with an lcdTable entry.\n");
    return FALSE;
}

/*
 * adapted from nv and savage 
 */
static void
ViaModesMonitorFixup(ScrnInfoPtr pScrn, MonPtr monitorp, DisplayModePtr mode)
{
	/* These are all modes that the driver sets up
	 * so we can comfortably update the monitor
	 * configuration to work with them.
	 */

	if (monitorp->hsync[0].lo == 0)
		monitorp->hsync[0].lo = 31.50;
	if (monitorp->hsync[0].hi == 0)
		monitorp->hsync[0].hi = 37.90;
	if (monitorp->vrefresh[0].lo == 0)
		monitorp->vrefresh[0].lo = 50.00;
	if (monitorp->vrefresh[0].hi == 0)
		monitorp->vrefresh[0].hi = 70.00;

	if (!mode->HSync)
		mode->HSync = ((float) mode->Clock ) / ((float) mode->HTotal);
	if (!mode->VRefresh)
		mode->VRefresh = (1000.0 * ((float) mode->Clock)) /
		   ((float) (mode->HTotal * mode->VTotal));

	if (mode->HSync < monitorp->hsync[0].lo) {
		monitorp->hsync[0].lo = mode->HSync;
		DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModesMonitorFixup - Adjusted HSync.lo to %f\n", monitorp->hsync[0].lo));
	}
	if (mode->HSync > monitorp->hsync[0].hi) {
		monitorp->hsync[0].hi = mode->HSync;
		DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModesMonitorFixup - Adjusted HSync.hi to %f\n", monitorp->hsync[0].hi));
	}
	if (mode->VRefresh < monitorp->vrefresh[0].lo) {
		monitorp->vrefresh[0].lo = mode->VRefresh;
		DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModesMonitorFixup - Adjusted VRefresh.lo to %f\n", monitorp->vrefresh[0].lo));
	}
	if (mode->VRefresh > monitorp->vrefresh[0].hi) {
		monitorp->vrefresh[0].hi = mode->VRefresh;
		DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModesMonitorFixup - Adjusted VRefresh.hi to %f\n", monitorp->vrefresh[0].hi));
	}

	monitorp->nHsync = 1;
	monitorp->nVrefresh = 1;
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
        ViaModesMonitorFixup(pScrn, monitorp, mode);
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

    if (pBIOSInfo->Panel->IsActive)
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

    switch (pVia->MemClk) {
        case VIA_MEM_SDR66:
        case VIA_MEM_SDR100:
        case VIA_MEM_SDR133:
            return VIA_BW_MIN;
        case VIA_MEM_DDR200:
            return VIA_BW_DDR200;
        case VIA_MEM_DDR266:
        case VIA_MEM_DDR333:
        case VIA_MEM_DDR400:
            return VIA_BW_DDR400;
        case VIA_MEM_DDR533:
        case VIA_MEM_DDR667:
            return VIA_BW_DDR667;
        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "ViaBandwidthAllowed: Unknown memory type: %d\n", pVia->MemClk);
            return VIA_BW_MIN;
    }
}

static CARD32
ViaModeDotClockTranslate(ScrnInfoPtr pScrn, DisplayModePtr mode);

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

    if (pVia->UseLegacyModeSwitch) {

        if (pVia->IsSecondary)
            ret = ViaSecondCRTCModeValid(pScrn, mode);
        else
            ret = ViaFirstCRTCModeValid(pScrn, mode);

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
        } else {
            if (pBIOSInfo->Panel->IsActive && !ViaPanelGetIndex(pScrn, mode))
                return MODE_BAD;
            else if (!ViaModeDotClockTranslate(pScrn, mode))
                return MODE_NOCLOCK;
        }

    } else {

        if (pBIOSInfo->FirstCRTC->IsActive) {
            ret = ViaFirstCRTCModeValid(pScrn, mode);
            if (ret != MODE_OK)
                return ret;
        }

        if (pBIOSInfo->SecondCRTC->IsActive) {
            ret = ViaSecondCRTCModeValid(pScrn, mode);
            if (ret != MODE_OK)
                return ret;
        }

        if (pBIOSInfo->Panel->IsActive) {
            ViaPanelModePtr nativeMode = pBIOSInfo->Panel->NativeMode;

            if (nativeMode->Width < mode->HDisplay
                || nativeMode->Height < mode->VDisplay)
                return MODE_PANEL;
        }

        if (!ViaModeDotClockTranslate(pScrn, mode))
            return MODE_NOCLOCK;
    }

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

    if (pBIOSInfo->Panel->NativeModeIndex == VIA_PANEL12X10)
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
                if (pBIOSInfo->Panel->NativeModeIndex == VIA_PANEL12X10)
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
ViaModePrimaryLegacy(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModePrimary\n"));
    DEBUG(ViaPrintMode(pScrn, mode));

    /* Turn off Screen */
    ViaCrtcMask(hwp, 0x17, 0x00, 0x80);

    /* Clean Second Path Status */
    hwp->writeCrtc(hwp, 0x6A, 0x00);
    hwp->writeCrtc(hwp, 0x6B, 0x00);
    hwp->writeCrtc(hwp, 0x6C, 0x00);
    hwp->writeCrtc(hwp, 0x93, 0x00);

    ViaCRTCInit(pScrn);
    ViaFirstCRTCSetMode(pScrn, mode);
    pBIOSInfo->Clock = ViaModeDotClockTranslate(pScrn, mode);
    pBIOSInfo->ClockExternal = FALSE;

    /* Enable MMIO & PCI burst (1 wait state) */
    ViaSeqMask(hwp, 0x1A, 0x06, 0x06);

    if (!pBIOSInfo->CrtActive)
        ViaCrtcMask(hwp, 0x36, 0x30, 0x30);
    else
        ViaSeqMask(hwp, 0x16, 0x00, 0x40);

    if (pBIOSInfo->Panel->IsActive && ViaPanelGetIndex(pScrn, mode)) {
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

/*
 *
 */
void
ViaModeSecondaryLegacy(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModeSecondary\n"));
    DEBUG(ViaPrintMode(pScrn, mode));

    /* Turn off Screen */
    ViaCrtcMask(hwp, 0x17, 0x00, 0x80);

    ViaSecondCRTCSetMode(pScrn, mode);

    if (pBIOSInfo->TVActive)
        ViaTVSetMode(pScrn, mode);

    /* CLE266A2 apparently doesn't like this */
    if (!(pVia->Chipset == VIA_CLE266 && pVia->ChipRev == 0x02))
        ViaCrtcMask(hwp, 0x6C, 0x00, 0x1E);

    if (pBIOSInfo->Panel->IsActive
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

    if (pBIOSInfo->LCDPower) 
        pBIOSInfo->LCDPower(pScrn, On);

    /* Find Panel Size Index for PowerSeq Table */
    if (pVia->Chipset == VIA_CLE266) {
        if (pBIOSInfo->Panel->NativeModeIndex != VIA_PANEL_INVALID) {
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

void
ViaDFPPower(ScrnInfoPtr pScrn, Bool On)
{
#ifdef HAVE_DEBUG
    if (On)
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaDFPPower: On.\n");
    else
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaDFPPower: Off.\n");
#endif
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    /* Display Channel Select */
    ViaCrtcMask(hwp, 0xD2, 0x30, 0x30);

    /* Power on TMDS */
    ViaCrtcMask(hwp, 0xD2, 0x00, 0x08);
    
}


void
ViaModeFirstCRTC(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModeFirstCRTC\n");
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    /* Turn off Screen */
    ViaCrtcMask(hwp, 0x17, 0x00, 0x80);

    ViaFirstCRTCSetMode(pScrn, mode);
    pBIOSInfo->Clock = ViaModeDotClockTranslate(pScrn, mode);
    pBIOSInfo->ClockExternal = FALSE;

    ViaSetPrimaryFIFO(pScrn, mode);

    ViaSetPrimaryDotclock(pScrn, pBIOSInfo->Clock);
    ViaSetUseExternalClock(hwp);
    ViaCrtcMask(hwp, 0x6B, 0x00, 0x01);

    hwp->disablePalette(hwp);

    /* Turn on Screen */
    ViaCrtcMask(hwp, 0x17, 0x80, 0x80);
}

void
ViaModeSecondCRTC(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    DisplayModePtr nativeDisplayMode = pBIOSInfo->Panel->NativeDisplayMode;
    DisplayModePtr centeredMode = pBIOSInfo->Panel->CenteredMode;
    DisplayModePtr realMode = mode;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModeSecondCRTC\n"));

    if (pBIOSInfo->Panel->IsActive) {
        if (nativeDisplayMode) {
            ViaPanelScale(pScrn, mode->HDisplay, mode->VDisplay,
                          nativeDisplayMode->HDisplay,
                          nativeDisplayMode->VDisplay);
            if (!pBIOSInfo->Center
                && (mode->HDisplay < nativeDisplayMode->HDisplay
                    || mode->VDisplay < nativeDisplayMode->VDisplay)) {
                pBIOSInfo->Panel->Scale = TRUE;
                realMode = nativeDisplayMode;
            } else {
                pBIOSInfo->Panel->Scale = FALSE;
                ViaPanelCenterMode(centeredMode, nativeDisplayMode, mode);
                realMode = centeredMode;
                ViaPanelScaleDisable(pScrn);
            }
        }
    }

    ViaSecondCRTCSetMode(pScrn, realMode);
    ViaSetSecondaryFIFO(pScrn, realMode);
    pBIOSInfo->Clock = ViaModeDotClockTranslate(pScrn, realMode);

    /* Fix LCD scaling */
    ViaSecondCRTCHorizontalQWCount(pScrn, mode->CrtcHDisplay);

    pBIOSInfo->ClockExternal = FALSE;
    ViaSetSecondaryDotclock(pScrn, pBIOSInfo->Clock);
    ViaSetUseExternalClock(hwp);

    hwp->disablePalette(hwp);
}

void
ViaModeSet(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaModeSet\n"));

    ViaPrintMode(pScrn, mode);

    if (pBIOSInfo->SecondCRTC->IsActive) {
        ViaModeSecondCRTC(pScrn, mode);
        ViaSecondDisplayChannelEnable(pScrn);
    }

    if (pBIOSInfo->FirstCRTC->IsActive) {
        if (pBIOSInfo->CrtActive) {
            /* CRT on FirstCRTC */
            ViaDisplaySetStreamOnCRT(pScrn, TRUE);
            ViaDisplayEnableCRT(pScrn);
        }

        if (pBIOSInfo->DfpActive) {
            /* DFP on FirstCrtc */
            ViaDisplaySetStreamOnDFP(pScrn, TRUE);
            ViaDFPPower(pScrn, TRUE);
        }
        
        ViaModeFirstCRTC(pScrn, mode);
    } else {
        ViaDisplayDisableCRT(pScrn);
    }

    if (pBIOSInfo->Simultaneous->IsActive) {
        ViaDisplayEnableSimultaneous(pScrn);
    } else {
        ViaDisplayDisableSimultaneous(pScrn);
    }
}
