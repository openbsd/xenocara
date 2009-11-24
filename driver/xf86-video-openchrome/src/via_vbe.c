/*
 * Copyright 2004 The Unichrome Project  [unichrome.sf.net]
 * Copyright (c) 2000 by Conectiva S.A. (http://www.conectiva.com)
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * VBE Mode setting.
 *
 * The functions were more or less taken from the vesa driver.
 * They were added to support BIOS modes directly.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via.h"
#include "via_driver.h"
#include "vbe.h"
#include "vbeModes.h"

#define R16(v)                ((v) & 0xffff)


void
ViaVbeAdjustFrame(int scrnIndex, int x, int y, int flags)
{
    VIAPtr pVia = VIAPTR(xf86Screens[scrnIndex]);

    VBESetDisplayStart(pVia->pVbe, x, y, TRUE);
}

/*
 * Default values for pInt10.
 */
static void
ViaVbeInitInt10(vbeInfoPtr pVbe)
{
    pVbe->pInt10->ax = 0x4F14;
    pVbe->pInt10->cx = 0;
    pVbe->pInt10->dx = 0;
    pVbe->pInt10->di = 0;
    pVbe->pInt10->num = 0x10;
}

static int
ViaVbeGetRefreshRateIndex(int maxRefresh)
{
    int rri = 0;

    if (maxRefresh >= 120) {
        rri = 10;
    } else if (maxRefresh >= 100) {
        rri = 9;
    } else if (maxRefresh >= 85) {
        rri = 7;
    } else if (maxRefresh >= 75) {
        rri = 5;
    } else {
        rri = 0;
    }

    return rri;
}

static int
ViaVbeGetActiveDevices(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    int activeDevices;

    activeDevices = 0;

    /* Set Active Device and translate BIOS byte definition. */
    if (pBIOSInfo->CrtActive)
        activeDevices = 0x01;
    if (pBIOSInfo->Panel->IsActive)
        activeDevices |= 0x02;
    if (pBIOSInfo->TVActive)
        activeDevices |= 0x04;

    /* TODO: Add others devices. */

    return activeDevices;
}

/*
 * Sets the requested mode, refresh rate and active devices.
 */
static Bool
ViaVbeSetActiveDevices(ScrnInfoPtr pScrn, int mode, int refresh)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vbeInfoPtr pVbe = pVia->pVbe;

    ViaVbeInitInt10(pVbe);
    pVbe->pInt10->bx = 0x8003;
    pVbe->pInt10->cx = ViaVbeGetActiveDevices(pScrn);
    pVbe->pInt10->dx = mode & 0x1FF;
    pVbe->pInt10->di = ViaVbeGetRefreshRateIndex(refresh);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaVbeSetActiveDevices "
                     "mode: %x, refresh: %d active devices: 0x%2x\n",
                     mode, refresh, pVbe->pInt10->cx));

    xf86ExecX86int10(pVbe->pInt10);

    if (pVbe->pInt10->ax != 0x4F)
        return FALSE;

    return TRUE;
}

/*
 * Sets the panel mode (expand or center).
 */
static Bool
ViaVbeSetPanelMode(ScrnInfoPtr pScrn, Bool expand)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vbeInfoPtr pVbe = pVia->pVbe;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaVbeSetPanelMode\n"));

    ViaVbeInitInt10(pVbe);
    pVbe->pInt10->ax = 0x4F14;
    pVbe->pInt10->bx = 0x0306;
    pVbe->pInt10->cx = 0x80 | expand;

    xf86ExecX86int10(pVbe->pInt10);

    if (pVbe->pInt10->ax != 0x4F)
        return FALSE;

    return TRUE;
}

static Bool
ViaVbeSetRefresh(ScrnInfoPtr pScrn, int maxRefresh)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaVbeSetRefresh\n"));
    vbeInfoPtr pVbe = pVia->pVbe;

    ViaVbeInitInt10(pVbe);
    pVbe->pInt10->bx = 0x0001;
    pVbe->pInt10->cx = ViaVbeGetActiveDevices(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Active Device: %d\n",
                     pVbe->pInt10->cx));

    pVbe->pInt10->di = ViaVbeGetRefreshRateIndex(maxRefresh);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Refresh Rate Index: %d\n",
                     pVbe->pInt10->di));

    /* Real execution */
    xf86ExecX86int10(pVbe->pInt10);

    if (pVbe->pInt10->ax != 0x4F)
        return FALSE;

    return TRUE;
}

Bool
ViaVbeSetMode(ScrnInfoPtr pScrn, DisplayModePtr pMode)
{
    VIAPtr pVia;
    VbeModeInfoData *data;

    pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;
    int mode;
    int refreshRate;

    pVia->OverlaySupported = FALSE;

    data = (VbeModeInfoData *) pMode->Private;

    mode = data->mode | (1 << 15);

    /* Enable linear addressing. */
    mode |= 1 << 14;

    if (data->block) {
        refreshRate = data->block->RefreshRate;
    } else {
        refreshRate = VBE_DEFAULT_REFRESH;
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "Unable to determine the refresh rate, using %.2f. "
                   "Please check your configuration.\n", refreshRate / 100.);
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "Trying VBE Mode %dx%d (0x%x) Refresh %.2f:\n",
               (int)data->data->XResolution, (int)data->data->YResolution,
               mode & ~(1 << 11), (float)refreshRate / 100.);

    if (pVia->useLegacyVBE) {

        ViaVbeSetRefresh(pScrn, refreshRate / 100);

        if (VBESetVBEMode(pVia->pVbe, mode, data->block) == FALSE) {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VBESetVBEMode failed");
            if ((data->block || (data->mode & (1 << 11))) &&
                VBESetVBEMode(pVia->pVbe, (mode & ~(1 << 11)), NULL) == TRUE) {
                /* Some cards do not like setting the clock. */
                xf86ErrorF("...but worked OK without customized "
                           "refresh and dotclock.\n");
                xfree(data->block);
                data->block = NULL;
                data->mode &= ~(1 << 11);
            } else {
                ErrorF("\n");
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Set VBE Mode failed!\n");
                return (FALSE);
            }
        }
    } else {

        if (pBIOSInfo->Panel->IsActive && !pVia->useLegacyVBE) {
            /* 
             * FIXME: Should we always set the panel expansion?
             * Does it depend on the resolution?
             */
            if (!ViaVbeSetPanelMode(pScrn, !pBIOSInfo->Center)) {
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                           "Unable to set the panel mode.\n");
            }
        }

        data->mode &= ~(1 << 11);
        if (VBESetVBEMode(pVia->pVbe, data->mode, NULL) == FALSE) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Set VBE Mode failed.\n");
            return (FALSE);
        }

        if (!ViaVbeSetActiveDevices(pScrn, data->mode, refreshRate / 100)) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Unable to set the active devices.\n");
            return (FALSE);
        }
    }

    if (data->data->XResolution != pScrn->displayWidth)
        VBESetLogicalScanline(pVia->pVbe, pScrn->displayWidth);

    pScrn->vtSema = TRUE;

    return (TRUE);
}

Bool
ViaVbeSaveRestore(ScrnInfoPtr pScrn, vbeSaveRestoreFunction function)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaVbeSaveRestore\n"));

    if ((MODE_QUERY < 0) || (function > MODE_RESTORE))
        return (FALSE);

    if (function == MODE_SAVE)
        pVia->SavedReg.SR1A = hwp->readSeq(hwp, 0x1A);

    /* Query amount of memory to save state. */
    if ((function == MODE_QUERY) ||
        ((function == MODE_SAVE) && (pVia->vbeMode.state == NULL))) {

        /* Make sure we save at least this information in case of failure. */
        (void)VBEGetVBEMode(pVia->pVbe, &(pVia->vbeMode.stateMode));

        if (pVia->vbeMode.major > 1) {
            if (!VBESaveRestore(pVia->pVbe, function,
                                (pointer) & (pVia->vbeMode.state),
                                &(pVia->vbeMode.stateSize),
                                &(pVia->vbeMode.statePage))) {
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                           "VBESaveRestore failed\n");
                return FALSE;
            }
        }
    }

    /* Save/Restore Super VGA state. */
    if (function != MODE_QUERY) {
        Bool retval = TRUE;

        if (pVia->vbeMode.major > 1) {
            if (function == MODE_RESTORE)
                memcpy(pVia->vbeMode.state, pVia->vbeMode.pstate,
                       pVia->vbeMode.stateSize);

            if ((retval = VBESaveRestore(pVia->pVbe, function,
                                         (pointer) & (pVia->vbeMode.state),
                                         &(pVia->vbeMode.stateSize),
                                         &(pVia->vbeMode.statePage)))
                && (function == MODE_SAVE)) {
                /* Do not rely on the memory not being touched. */
                if (pVia->vbeMode.pstate == NULL)
                    pVia->vbeMode.pstate = xalloc(pVia->vbeMode.stateSize);
                memcpy(pVia->vbeMode.pstate, pVia->vbeMode.state,
                       pVia->vbeMode.stateSize);
            }
        }

        if (function == MODE_RESTORE) {
            if (!VBESetVBEMode(pVia->pVbe, pVia->vbeMode.stateMode, NULL)) {
                xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                           "VBESetVBEMode failed\n");
            }
        }

        if (!retval)
            return (FALSE);
    }

    return (TRUE);
}


Bool
ViaVbeModePreInit(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VbeInfoBlock *vbe;

    int i;

    memset(&(pVia->vbeMode), 0, sizeof(ViaVbeModeInfo));

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "Searching for matching VESA mode(s):\n");

    if ((vbe = VBEGetVBEInfo(pVia->pVbe)) == NULL) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VBEGetVBEInfo failed\n");
        return FALSE;
    }

    pVia->vbeMode.major = (unsigned)(vbe->VESAVersion >> 8);
    pVia->vbeMode.minor = vbe->VESAVersion & 0xff;

    pScrn->modePool = VBEGetModePool(pScrn, pVia->pVbe, vbe, V_MODETYPE_VBE);
    if (pScrn->modePool == NULL) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No matching modes\n");
        return FALSE;
    }

    VBESetModeNames(pScrn->modePool);

    i = VBEValidateModes(pScrn, NULL, pScrn->display->modes,
                         NULL, NULL, 0, 2048, 1, 0, 2048,
                         pScrn->display->virtualX,
                         pScrn->display->virtualY,
                         pScrn->videoRam, LOOKUP_BEST_REFRESH);

    if (i <= 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes.\n");
        return (FALSE);
    }

    VBESetModeParameters(pScrn, pVia->pVbe);

    xf86PruneDriverModes(pScrn);
#if 0
    pMode = pScrn->modes;
    do {
        vbeMode = ((VbeModeInfoData *) pMode->Private)->data;
        pMode = pMode->next;
    } while (pMode != NULL && pMode != pScrn->modes);
#endif
    return TRUE;
}

static int
ViaVbePanelPower(vbeInfoPtr pVbe, int mode)
{
    pVbe->pInt10->num = 0x10;
    pVbe->pInt10->ax = 0x5F54;
    pVbe->pInt10->bx = (mode) ? 0x00 : 0x01;

    xf86ExecX86int10(pVbe->pInt10);

    return (R16(pVbe->pInt10->ax) == 0x015f);
}

#if 0
/* 
 * FIXME: This might be useful in the future. Otherwise feel free to remove. 
 * If mode=1, it sets the panel in a low-power, low-performance state;
 * if mode=0, then high performance.
 */
static int
ViaVbePanelLowPower(vbeInfoPtr pVbe, int mode)
{
    pVbe->pInt10->num = 0x10;
    pVbe->pInt10->ax = 0x5F60;
    pVbe->pInt10->bx = (mode) ? 0x01 : 0x00;

    xf86ExecX86int10(pVbe->pInt10);

    return (R16(pVbe->pInt10->ax) == 0x015f);
}
#endif

void
ViaVbeDoDPMS(ScrnInfoPtr pScrn, int mode)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr pBIOSInfo = pVia->pBIOSInfo;

    if (pBIOSInfo->Panel->IsActive)
        ViaVbePanelPower(pVia->pVbe, (mode == DPMSModeOn));

    VBEDPMSSet(pVia->pVbe, mode);
}

void
ViaVbeDPMS(ScrnInfoPtr pScrn, int mode, int flags)
{
    if (!pScrn->vtSema)
        return;

    ViaVbeDoDPMS(pScrn, mode);
}
