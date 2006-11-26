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
 *
 * VBE Mode setting
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via.h"
#include "via_driver.h"
#include "vbe.h"
#include "vbeModes.h"

#define R16(v)		((v) & 0xffff)

/*
 * Functions more or less stolen from the vesa driver. Added to support BIOS modes directly.
 */

void
ViaVbeAdjustFrame(int scrnIndex, int x, int y, int flags)
{
    VIAPtr pVia = VIAPTR(xf86Screens[scrnIndex]);

    VBESetDisplayStart(pVia->pVbe, x, y, TRUE);
}


static Bool
ViaVbeSetRefresh(ScrnInfoPtr pScrn, int maxRefresh)
{
    VIAPtr  pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr  pBIOSInfo = pVia->pBIOSInfo;
    int RealOff;
    pointer page = NULL;
    vbeInfoPtr pVbe = pVia->pVbe;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaVbeSetRefresh\n"));
    page = xf86Int10AllocPages(pVbe->pInt10, 1, &RealOff);
    if (!page)
        return FALSE;
    pVbe->pInt10->ax = 0x4F14;
    pVbe->pInt10->bx = 0x0001;
    pVbe->pInt10->cx = 0;
    pVbe->pInt10->dx = 0;
    pVbe->pInt10->di = 0;
    pVbe->pInt10->num = 0x10;

    /* Set Active Device and Translate BIOS byte definition */
    if (pBIOSInfo->CrtActive)
        pVbe->pInt10->cx = 0x01;
    if (pBIOSInfo->PanelActive)
        pVbe->pInt10->cx |= 0x02;
    if (pBIOSInfo->TVActive)
        pVbe->pInt10->cx |= 0x04;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Active Device: %d\n",
                     pVbe->pInt10->cx));

    if (maxRefresh >= 120) {
	pVbe->pInt10->di = 10;
    } else if (maxRefresh >= 100) {
	pVbe->pInt10->di = 9;
    } else if (maxRefresh >= 85) {
	pVbe->pInt10->di = 7;
    } else if (maxRefresh >= 75) {
	pVbe->pInt10->di = 5;
    } else {
	pVbe->pInt10->di = 0;
    }
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Refresh Rate Index: %d\n",
                     pVbe->pInt10->di));

    /* Real execution */
    xf86ExecX86int10(pVbe->pInt10);

    if (pVbe->pInt10->ax != 0x4F)
    {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                         "Via BIOS Set Device Refresh Rate fail!\n"));
        if (page)
            xf86Int10FreePages(pVbe->pInt10, page, 1);
        return FALSE;
    }

    if (page)
        xf86Int10FreePages(pVbe->pInt10, page, 1);

    return TRUE;
}

Bool
ViaVbeSetMode(ScrnInfoPtr pScrn, DisplayModePtr pMode)
{
    VIAPtr pVia;
    VbeModeInfoData *data;
    int mode;

    pVia = VIAPTR(pScrn);

    pVia->OverlaySupported = FALSE;

    data = (VbeModeInfoData*)pMode->Private;

    mode = data->mode | (1 << 15);

    /* enable linear addressing */
    mode |= 1 << 14;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Trying VBE Mode %dx%d (0x%x)\n", 
	       (int) data->data->XResolution,
	       (int) data->data->YResolution,
	       mode & ~(1 << 11));
    ViaVbeSetRefresh(pScrn, data->block->RefreshRate/100);
    if (VBESetVBEMode(pVia->pVbe, mode, data->block) == FALSE) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VBESetVBEMode failed");
	if ((data->block || (data->mode & (1 << 11))) &&
	    VBESetVBEMode(pVia->pVbe, (mode & ~(1 << 11)), NULL) == TRUE) {
	    /* Some cards do not like setting the clock.
	     */
	    xf86ErrorF("...but worked OK without customized refresh and dotclock.\n");
	    data->mode &= ~(1 << 11);
	}
	else {
	    ErrorF("\n");
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Set VBE Mode failed!\n");
	    return (FALSE);
	}
    }

    if (data->data->XResolution != pScrn->displayWidth)
	VBESetLogicalScanline(pVia->pVbe, pScrn->displayWidth);

    pScrn->vtSema = TRUE;

    if (!pVia->NoAccel)
	viaInitialize2DEngine(pScrn);
    
#ifdef XF86DRI
    VIAInitialize3DEngine(pScrn);
#endif 

    ViaVbeAdjustFrame(pScrn->scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);

    return (TRUE);
}

Bool
ViaVbeSaveRestore(ScrnInfoPtr pScrn, vbeSaveRestoreFunction function)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr  hwp = VGAHWPTR(pScrn);

    if ((MODE_QUERY < 0) || (function > MODE_RESTORE))
	return (FALSE);

    if (function == MODE_SAVE) {
        pVia->SavedReg.SR1A = hwp->readSeq(hwp, 0x1A);
    }

    /* Query amount of memory to save state */
    if ((function == MODE_QUERY) ||
	((function == MODE_SAVE) && (pVia->vbeMode.state == NULL))) {

	/* Make sure we save at least this information in case of failure */
	(void)VBEGetVBEMode(pVia->pVbe, &(pVia->vbeMode.stateMode));

	if (pVia->vbeMode.major > 1) { 

	    if (!VBESaveRestore(pVia->pVbe, function,
				(pointer) &(pVia->vbeMode.state),
				&(pVia->vbeMode.stateSize),
				&(pVia->vbeMode.statePage)))
		return FALSE;
	  
	} 
    }

    /* Save/Restore Super VGA state */
    if (function != MODE_QUERY) {
        Bool retval = TRUE;

	if (pVia->vbeMode.major > 1) { 
	    if (function == MODE_RESTORE)
		memcpy(pVia->vbeMode.state, pVia->vbeMode.pstate, pVia->vbeMode.stateSize);

	    if ((retval = VBESaveRestore(pVia->pVbe, function,
					 (pointer) &(pVia->vbeMode.state),
					 &(pVia->vbeMode.stateSize),
					 &(pVia->vbeMode.statePage)))
		&& (function == MODE_SAVE)) {
	        /* don't rely on the memory not being touched */
	        if (pVia->vbeMode.pstate == NULL)
		    pVia->vbeMode.pstate = xalloc(pVia->vbeMode.stateSize);
		memcpy(pVia->vbeMode.pstate, pVia->vbeMode.state, pVia->vbeMode.stateSize);
	    }
	} 

	if (function == MODE_RESTORE) {
	    VBESetVBEMode(pVia->pVbe, pVia->vbeMode.stateMode, NULL);
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
    VbeInfoBlock    *vbe;
    VbeModeInfoBlock *vbeMode;
    DisplayModePtr  pMode;
    int i;

    memset(&(pVia->vbeMode), 0, sizeof(ViaVbeModeInfo));

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
	       "Searching for matching VESA mode(s):\n");

    if ((vbe = VBEGetVBEInfo(pVia->pVbe)) == NULL) {
	return FALSE;
    }

    pVia->vbeMode.major = (unsigned) (vbe->VESAVersion >> 8);
    pVia->vbeMode.minor = vbe->VESAVersion & 0xff;

    pScrn->modePool = VBEGetModePool (pScrn, pVia->pVbe, vbe, V_MODETYPE_VBE);
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

    pMode = pScrn->modes;
    do {
	vbeMode = ((VbeModeInfoData*)pMode->Private)->data;
	pMode = pMode->next;
    } while (pMode != pScrn->modes);

    return TRUE;
}

static int ViaVbePanelPower(vbeInfoPtr pVbe, int mode)
{
    pVbe->pInt10->num = 0x10;
    pVbe->pInt10->ax  = 0x5F54;
    pVbe->pInt10->bx = (mode) ? 0x00 : 0x01;

    xf86ExecX86int10(pVbe->pInt10);

    return (R16(pVbe->pInt10->ax) == 0x015f);
}

#if 0  
/* 
 * FIXME: This might be useful in the future. Otherwise feel free to remove. 
 * if mode=1 sets the panel in a low power, low performance state.
 * if mode=0 high performance.
 */

static int ViaVbePanelLowPower(vbeInfoPtr pVbe, int mode)
{
    pVbe->pInt10->num = 0x10;
    pVbe->pInt10->ax  = 0x5F60;
    pVbe->pInt10->bx = (mode) ? 0x01 : 0x00;
    
    xf86ExecX86int10(pVbe->pInt10);

    return (R16(pVbe->pInt10->ax) == 0x015f);
}
#endif

void
ViaVbeDoDPMS(ScrnInfoPtr pScrn, int mode)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIABIOSInfoPtr  pBIOSInfo = pVia->pBIOSInfo;


    if (pBIOSInfo->PanelActive) 
        ViaVbePanelPower(pVia->pVbe, (mode == DPMSModeOn));

    VBEDPMSSet(pVia->pVbe,mode);
}

void
ViaVbeDPMS(ScrnInfoPtr pScrn, int mode, int flags)
{
    if (!pScrn->vtSema) 
	return; 
    
    ViaVbeDoDPMS(pScrn, mode);
}
