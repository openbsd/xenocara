/*
 * Copyright 2007 The Openchrome Project [openchrome.org]
 * Copyright (c) 1997-2003 by The XFree86 Project, Inc.
 * Copyright 1998-2007 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2007 S3 Graphics, Inc. All Rights Reserved.
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
 * Core panel functions.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via.h"
#include "via_driver.h"
#include "via_vgahw.h"
#include "via_id.h"
#include "via_timing.h"

static ViaPanelModeRec ViaPanelNativeModes[] = {
    {640, 480},
    {800, 600},
    {1024, 768},
    {1280, 768},
    {1280, 1024},
    {1400, 1050},
    {1600, 1200},   /* 0x6 Resolution 1440x900 */
    {1280, 800},    /* 0x7 Resolution 1280x800 (Samsung NC20) */
    {800, 480},     /* 0x8 For Quanta 800x480 */
    {1024, 600},    /* 0x9 Resolution 1024x600 (for HP 2133) */
    {1366, 768},    /* 0xA Resolution 1366x768 */
    {1920, 1080},
    {1920, 1200},
    {1280, 1024},   /* 0xD Need to be fixed to 1920x1200 */
    {1440, 900},    /* 0xE Need to be fixed to 640x240 */
    {1280, 720},    /* 0xF 480x640 */
    {1200, 900},   /* 0x10 For Panasonic 1280x768 18bit Dual-Channel Panel */
    {1360, 768},   /* 0x11 Resolution 1360X768 */
    {1024, 768},   /* 0x12 Resolution 1024x768 */
    {800, 480}     /* 0x13 General 8x4 panel use this setting */
};

static int
ViaPanelLookUpModeIndex(int width, int height)
{
    int i, index = VIA_PANEL_INVALID;
    int length = sizeof(ViaPanelNativeModes) / sizeof(ViaPanelModeRec);


    for (i = 0; i < length; i++) {
        if (ViaPanelNativeModes[i].Width == width
            && ViaPanelNativeModes[i].Height == height) {
            index = i;
            break;
        }
    }
    return index;
}

/*
 * Sets the panel dimensions from the configuration
 * using name with format "9999x9999".
 */
void
ViaPanelGetNativeModeFromOption(ScrnInfoPtr pScrn, char *name)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;
    ViaPanelInfoPtr panel = pBIOSInfo->Panel;
    CARD8 index;
    CARD8 length;
    char aux[10];

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "ViaPanelGetNativeModeFromOption\n"));

    panel->NativeModeIndex = VIA_PANEL_INVALID;
    if (strlen(name) < 10) {
        length = sizeof(ViaPanelNativeModes) / sizeof(ViaPanelModeRec);

        for (index = 0; index < length; index++) {
            sprintf(aux, "%dx%d", ViaPanelNativeModes[index].Width,
                    ViaPanelNativeModes[index].Height);
            if (!xf86NameCmp(name, aux)) {
                panel->NativeModeIndex = index;
                panel->NativeMode->Width = ViaPanelNativeModes[index].Width;
                panel->NativeMode->Height = ViaPanelNativeModes[index].Height;
                break;
            }
        }
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "%s is not a valid panel size.\n", name);
    }
}

/*
 * Gets the native panel resolution from scratch pad registers.
 */
void
ViaPanelGetNativeModeFromScratchPad(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD8 index;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "ViaPanelGetNativeModeFromScratchPad\n"));

    index = hwp->readCrtc(hwp, 0x3F) & 0x0F;

    ViaPanelInfoPtr panel = pVia->pBIOSInfo->Panel;

    panel->NativeModeIndex = index;
    panel->NativeMode->Width = ViaPanelNativeModes[index].Width;
    panel->NativeMode->Height = ViaPanelNativeModes[index].Height;
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "Native Panel Resolution is %dx%d\n",
               panel->NativeMode->Width, panel->NativeMode->Height);
}

void
ViaPanelScaleDisable(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    ViaCrtcMask(hwp, 0x79, 0x00, 0x01);
    if (pVia->Chipset != VIA_CLE266 && pVia->Chipset != VIA_KM400)
        ViaCrtcMask(hwp, 0xA2, 0x00, 0xC8);
}

void
ViaPanelScale(ScrnInfoPtr pScrn, int resWidth, int resHeight,
              int panelWidth, int panelHeight)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    int horScalingFactor = 0;
    int verScalingFactor = 0;
    CARD8 cra2 = 0;
    CARD8 cr77 = 0;
    CARD8 cr78 = 0;
    CARD8 cr79 = 0;
    CARD8 cr9f = 0;
    Bool scaling = FALSE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "ViaPanelScale: %d,%d -> %d,%d\n",
                     resWidth, resHeight, panelWidth, panelHeight));

    if (resWidth < panelWidth) {
        /* FIXME: It is different for chipset < K8M800 */
        horScalingFactor = ((resWidth - 1) * 4096) / (panelWidth - 1);

        /* Horizontal scaling enabled */
        cra2 = 0xC0;
        cr9f = horScalingFactor & 0x0003;          /* HSCaleFactor[1:0] at CR9F[1:0] */
        cr77 = (horScalingFactor & 0x03FC) >> 2;   /* HSCaleFactor[9:2] at CR77[7:0] */
        cr79 = (horScalingFactor & 0x0C00) >> 10;  /* HSCaleFactor[11:10] at CR79[5:4] */
        cr79 <<= 4;
        scaling = TRUE;
    }

    if (resHeight < panelHeight) {
        verScalingFactor = ((resHeight - 1) * 2048) / (panelHeight - 1);

        /* Vertical scaling enabled */
        cra2 |= 0x08;
        cr79 |= ((verScalingFactor & 0x0001) << 3);       /* VSCaleFactor[0] at CR79[3] */
        cr78 |= (verScalingFactor & 0x01FE) >> 1;         /* VSCaleFactor[8:1] at CR78[7:0] */
        cr79 |= ((verScalingFactor & 0x0600) >> 9) << 6;  /* VSCaleFactor[10:9] at CR79[7:6] */
        scaling = TRUE;
    }

    if (scaling) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                         "Scaling factor: horizontal %d (0x%x), vertical %d (0x%x)\n",
                         horScalingFactor, horScalingFactor,
                         verScalingFactor, verScalingFactor));

        ViaCrtcMask(hwp, 0x77, cr77, 0xFF);
        ViaCrtcMask(hwp, 0x78, cr78, 0xFF);
        ViaCrtcMask(hwp, 0x79, cr79, 0xF8);
        ViaCrtcMask(hwp, 0x9F, cr9f, 0x03);
        ViaCrtcMask(hwp, 0x79, 0x03, 0x03);
    } else
        ViaCrtcMask(hwp, 0x79, 0x00, 0x01);

    ViaCrtcMask(hwp, 0xA2, cra2, 0xC8);

    /* Horizontal scaling selection: interpolation */
    // ViaCrtcMask(hwp, 0x79, 0x02, 0x02);
    // else
    // ViaCrtcMask(hwp, 0x79, 0x00, 0x02);
    /* Horizontal scaling factor selection original / linear */
    //ViaCrtcMask(hwp, 0xA2, 0x40, 0x40);
}


/*
 * Generates a display mode for the native panel resolution,  using CVT.
 */
static void
ViaPanelGetNativeDisplayMode(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    ViaPanelModePtr panelMode = pVia->pBIOSInfo->Panel->NativeMode;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "ViaPanelGetNativeDisplayMode\n"));

    if (panelMode->Width && panelMode->Height) {

        /* TODO: fix refresh rate and check malloc */
        DisplayModePtr p = malloc( sizeof(DisplayModeRec) ) ;
        memset(p, 0, sizeof(DisplayModeRec));

        float refresh = 60.0f ;

        /* The following code is borrowed from xf86SetModeCrtc. */
        if (p) {
            viaTimingCvt(p, panelMode->Width, panelMode->Height, refresh, FALSE, TRUE);
            p->CrtcHDisplay = p->HDisplay;
            p->CrtcHSyncStart = p->HSyncStart;
            p->CrtcHSyncEnd = p->HSyncEnd;
            p->CrtcHTotal = p->HTotal;
            p->CrtcHSkew = p->HSkew;
            p->CrtcVDisplay = p->VDisplay;
            p->CrtcVSyncStart = p->VSyncStart;
            p->CrtcVSyncEnd = p->VSyncEnd;
            p->CrtcVTotal = p->VTotal;

            p->CrtcVBlankStart = min(p->CrtcVSyncStart, p->CrtcVDisplay);
            p->CrtcVBlankEnd = max(p->CrtcVSyncEnd, p->CrtcVTotal);
            p->CrtcHBlankStart = min(p->CrtcHSyncStart, p->CrtcHDisplay);
            p->CrtcHBlankEnd = max(p->CrtcHSyncEnd, p->CrtcHTotal);

        }
        pVia->pBIOSInfo->Panel->NativeDisplayMode = p;
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "Invalid panel dimension (%dx%d)\n", panelMode->Width,
                   panelMode->Height);
    }
}

void
ViaPanelPreInit(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaPanelPreInit\n"));

    ViaPanelInfoPtr panel = pBIOSInfo->Panel;

    /* First try to get the mode from EDID. */
    if (panel->NativeModeIndex == VIA_PANEL_INVALID) {
        int width, height;
        Bool ret;

        ret = ViaPanelGetSizeFromDDCv1(pScrn, &width, &height);
/*
        if (!ret)
            ret = ViaPanelGetSizeFromDDCv2(pScrn, &width);
*/
        if (ret) {
            panel->NativeModeIndex = ViaPanelLookUpModeIndex(width, height);
            DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaPanelLookUpModeIndex, Width %d, Height %d, NativeModeIndex%d\n", width, height, panel->NativeModeIndex));
            if (panel->NativeModeIndex != VIA_PANEL_INVALID) {
                panel->NativeMode->Width = width;
                panel->NativeMode->Height = height;
            }
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Unable to get panel size from EDID. Return code: %d\n", ret);
        }
    }

    if (panel->NativeModeIndex == VIA_PANEL_INVALID)
        ViaPanelGetNativeModeFromScratchPad(pScrn);

    if (panel->NativeModeIndex != VIA_PANEL_INVALID)
        ViaPanelGetNativeDisplayMode(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "NativeModeIndex: %d\n", panel->NativeModeIndex )) ;

}

void
ViaPanelCenterMode(DisplayModePtr centerMode, DisplayModePtr panelMode,
                   DisplayModePtr mode)
{
    memcpy(centerMode, mode, sizeof(DisplayModeRec));

    CARD32 HDiff = (panelMode->CrtcHDisplay - mode->CrtcHDisplay) / 2;
    CARD32 VDiff = (panelMode->CrtcVDisplay - mode->CrtcVDisplay) / 2;

    centerMode->CrtcHTotal += HDiff * 2;
    centerMode->CrtcVTotal += VDiff * 2;

    centerMode->CrtcHSyncStart += HDiff;
    centerMode->CrtcHSyncEnd += HDiff;
    centerMode->CrtcHBlankStart += HDiff;
    centerMode->CrtcHBlankEnd += HDiff;

    centerMode->CrtcVSyncStart += VDiff;
    centerMode->CrtcVSyncEnd += VDiff;
    centerMode->CrtcVBlankStart += VDiff;
    centerMode->CrtcVBlankEnd += VDiff;
}


/*
 * Try to interprete EDID ourselves.
 */
Bool
ViaPanelGetSizeFromEDID(ScrnInfoPtr pScrn, xf86MonPtr pMon,
                        int *width, int *height)
{
    int i, max = 0, vsize;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAGetPanelSizeFromEDID\n"));

    /* !!! Why are we not checking VESA modes? */

    /* checking standard timings */
    for (i = 0; i < 8; i++)
        if ((pMon->timings2[i].hsize > 256)
            && (pMon->timings2[i].hsize > max)) {
            max = pMon->timings2[i].hsize;
            vsize = pMon->timings2[i].vsize;
        }

    if (max != 0) {
        *width = max;
        *height = vsize;
        return TRUE;
    }

    /* checking detailed monitor section */

    /* !!! skip Ranges and standard timings */

    /* check detailed timings */
    for (i = 0; i < DET_TIMINGS; i++)
        if (pMon->det_mon[i].type == DT) {
            struct detailed_timings timing = pMon->det_mon[i].section.d_timings;

            /* ignore v_active for now */
            if ((timing.clock > 15000000) && (timing.h_active > max)) {
                max = timing.h_active;
                vsize = timing.v_active;
            }
        }

    if (max != 0) {
        *width = max;
        *height = vsize;
        return TRUE;
    }

    return FALSE;
}

Bool
ViaPanelGetSizeFromDDCv1(ScrnInfoPtr pScrn, int *width, int *height)

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

    if (!ViaPanelGetSizeFromEDID(pScrn, pMon, width, height)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Unable to read PanelSize from EDID information\n");
        return FALSE;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "VIAGetPanelSizeFromDDCv1: (%dx%d)\n", *width, *height));
    return TRUE;
}

Bool
ViaPanelGetSizeFromDDCv2(ScrnInfoPtr pScrn, int *width)
{
    VIAPtr pVia = VIAPTR(pScrn);
    CARD8 W_Buffer[1];
    CARD8 R_Buffer[4];
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

    *width = R_Buffer[0] | (R_Buffer[1] << 8);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "VIAGetPanelSizeFromDDCv2: %d\n", *width));

    return TRUE;
}
