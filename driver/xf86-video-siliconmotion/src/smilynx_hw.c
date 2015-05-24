/*
Copyright (C) 1994-1999 The XFree86 Project, Inc.  All Rights Reserved.
Copyright (C) 2000 Silicon Motion, Inc.  All Rights Reserved.
Copyright (C) 2008 Francisco Jerez. All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FIT-
NESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the names of The XFree86 Project and
Silicon Motion shall not be used in advertising or otherwise to promote the
sale, use or other dealings in this Software without prior written
authorization from The XFree86 Project or Silicon Motion.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "smi.h"
#include "smi_crtc.h"
#include "smilynx.h"

static unsigned int SMILynx_ddc1Read(ScrnInfoPtr pScrn);

Bool
SMILynx_HWInit(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    SMIRegPtr mode = pSmi->mode;
    vgaHWPtr	hwp = VGAHWPTR(pScrn);
    int		vgaIOBase  = hwp->IOBase;
    int		vgaCRIndex = vgaIOBase + VGA_CRTC_INDEX_OFFSET;
    int		vgaCRData  = vgaIOBase + VGA_CRTC_DATA_OFFSET;

    ENTER();

    if (pSmi->PCIBurst) {
	mode->SR17 |= 0x20;
    } else {
	mode->SR17 &= ~0x20;
    }

    /* Gamma correction */
    if (pSmi->Chipset == SMI_LYNX3DM || pSmi->Chipset == SMI_COUGAR3DR) {
	if(pScrn->bitsPerPixel == 8)
	    mode->SR66 = (mode->SR66 & 0x33) | 0x00; /* Both RAMLUT on, 6 bits-RAM */
	else
	    mode->SR66 = (mode->SR66 & 0x33) | 0x04; /* Both RAMLUT on, Gamma correct ON */
    }

    /* Program MCLK */
    if (pSmi->MCLK > 0)
	SMI_CommonCalcClock(pScrn->scrnIndex, pSmi->MCLK,
			    1, 1, 63, 0, 0,
			    pSmi->clockRange.minClock,
			    pSmi->clockRange.maxClock,
			    &mode->SR6A, &mode->SR6B);

    if(!pSmi->useBIOS) {
	/* Disable DAC and LCD framebuffer r/w operation */
	mode->SR21 |= 0xB0;

	/* Power down mode is standby mode, VCLK and MCLK divided by 4 in standby mode */
	mode->SR20  = (mode->SR20 & ~0xB0) | 0x10;

	/* Set DPMS state to Off */
	mode->SR22 |= 0x30;

	if (pSmi->Chipset != SMI_COUGAR3DR) {
	    /* Select no displays */
	    mode->SR31 &= ~0x07;

	    /* Disable virtual refresh */
	    mode->SR31 &= ~0x80;

	    /* Disable expansion */
	    mode->SR32 &= ~0x03;
	    /* Enable autocentering */
	    if (SMI_LYNXM_SERIES(pSmi->Chipset))
		mode->SR32 |= 0x04;
	    else
		mode->SR32 &= ~0x04;

	    if (pSmi->lcd == 2) /* Panel is DSTN */
		mode->SR21 = 0x00;

	    /* Enable HW LCD power sequencing */
	    mode->SR34 |= 0x80;
	}

	/* Disable Vertical Expansion/Vertical Centering/Horizontal Centering */
	mode->CR90[0xE] &= ~0x7;

	/* use vclk1 */
	mode->SR68 = 0x54;

	if(pSmi->Dualhead){
	    /* set LCD to vclk2 */
	    mode->SR69 = 0x04;
	}

	/* Disable panel video */
	mode->SRA0 = 0;

	mode->CR33 = 0;
	mode->CR3A = 0;
    }

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x17, mode->SR17);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x20, mode->SR20);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x21, mode->SR21);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x22, mode->SR22);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x31, mode->SR31);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x32, mode->SR32);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x34, mode->SR34);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x66, mode->SR66);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x68, mode->SR68);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x69, mode->SR69);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6A, mode->SR6A);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6B, mode->SR6B);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0xA0, mode->SRA0);

    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x33, mode->CR33);
    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x3A, mode->CR3A);
    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x9E, mode->CR90[0xE]);

    LEAVE(TRUE);
}

/*
 * This function performs the inverse of the restore function: It saves all the
 * standard and extended registers that we are going to modify to set up a video
 * mode.
 */

void
SMILynx_Save(ScrnInfoPtr pScrn)
{
    SMIPtr	pSmi = SMIPTR(pScrn);
    int		i;
    CARD32	offset;
    SMIRegPtr	save = pSmi->save;
    vgaHWPtr	hwp = VGAHWPTR(pScrn);
    vgaRegPtr	vgaSavePtr = &hwp->SavedReg;
    int		vgaIOBase  = hwp->IOBase;
    int		vgaCRIndex = vgaIOBase + VGA_CRTC_INDEX_OFFSET;
    int		vgaCRData  = vgaIOBase + VGA_CRTC_DATA_OFFSET;

    ENTER();

    /* Save the standard VGA registers */
    vgaHWSave(pScrn, vgaSavePtr, VGA_SR_ALL);
    save->smiDACMask = VGAIN8(pSmi, VGA_DAC_MASK);
    VGAOUT8(pSmi, VGA_DAC_READ_ADDR, 0);
    for (i = 0; i < 256; i++) {
	save->smiDacRegs[i][0] = VGAIN8(pSmi, VGA_DAC_DATA);
	save->smiDacRegs[i][1] = VGAIN8(pSmi, VGA_DAC_DATA);
	save->smiDacRegs[i][2] = VGAIN8(pSmi, VGA_DAC_DATA);
    }
    for (i = 0, offset = 2; i < 8192; i++, offset += 8)
	save->smiFont[i] = *(pSmi->FBBase + offset);

    /* Now we save all the extended registers we need. */
    save->SR17 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x17);
    save->SR18 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x18);

    save->SR20 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x20);
    save->SR21 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x21);
    save->SR22 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x22);
    save->SR23 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x23);
    save->SR24 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x24);

    save->SR31 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x31);
    save->SR32 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x32);

    save->SR66 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x66);
    save->SR68 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x68);
    save->SR69 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x69);
    save->SR6A = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6A);
    save->SR6B = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6B);
    save->SR6C = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6C);
    save->SR6D = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6D);

    save->SR81 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x81);
    save->SRA0 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0xA0);

    if (pSmi->Dualhead) {
	/* dualhead stuff */
	save->SR40 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x40);
	save->SR41 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x41);
	save->SR42 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x42);
	save->SR43 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x43);
	save->SR44 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x44);
	save->SR45 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x45);
	save->SR48 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x48);
	save->SR49 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x49);
	save->SR4A = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x4A);
	save->SR4B = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x4B);
	save->SR4C = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x4C);

	save->SR50 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x50);
	save->SR51 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x51);
	save->SR52 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x52);
	save->SR53 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x53);
	save->SR54 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x54);
	save->SR55 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x55);
	save->SR56 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x56);
	save->SR57 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x57);
	save->SR5A = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x5A);

	/* PLL2 stuff */
	save->SR6E = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6E);
	save->SR6F = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6F);
    }

    if (SMI_LYNXM_SERIES(pSmi->Chipset)) {
	/* Save common registers */
	save->CR30 = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x30);
	save->CR3A = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x3A);
	for (i = 0; i < 15; i++) {
	    save->CR90[i] = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x90 + i);
	}
	for (i = 0; i < 14; i++) {
	    save->CRA0[i] = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0xA0 + i);
	}

	/* Save primary registers */
	VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x9E, save->CR90[14] & ~0x20);

	save->CR33 = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x33);
	for (i = 0; i < 14; i++) {
	    save->CR40[i] = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x40 + i);
	}
	save->CR9F = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x9F);

	/* Save secondary registers */
	VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x9E, save->CR90[14] | 0x20);
	save->CR33_2 = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x33);
	for (i = 0; i < 14; i++) {
	    save->CR40_2[i] = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x40 + i);
	}
	save->CR9F_2 = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x9F);

	/* PDR#1069 */
	VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x9E, save->CR90[14]);

    }
    else {
	save->CR30 = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x30);
	save->CR33 = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x33);
	save->CR3A = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x3A);
	for (i = 0; i < 14; i++) {
	    save->CR40[i] = VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x40 + i);
	}
    }

    save->DPR10 = READ_DPR(pSmi, 0x10);
    save->DPR1C = READ_DPR(pSmi, 0x1C);
    save->DPR20 = READ_DPR(pSmi, 0x20);
    save->DPR24 = READ_DPR(pSmi, 0x24);
    save->DPR28 = READ_DPR(pSmi, 0x28);
    save->DPR2C = READ_DPR(pSmi, 0x2C);
    save->DPR30 = READ_DPR(pSmi, 0x30);
    save->DPR3C = READ_DPR(pSmi, 0x3C);
    save->DPR40 = READ_DPR(pSmi, 0x40);
    save->DPR44 = READ_DPR(pSmi, 0x44);

    save->VPR00 = READ_VPR(pSmi, 0x00);
    save->VPR0C = READ_VPR(pSmi, 0x0C);
    save->VPR10 = READ_VPR(pSmi, 0x10);

    if (pSmi->Chipset == SMI_COUGAR3DR) {
	save->FPR00_ = READ_FPR(pSmi, FPR00);
	save->FPR0C_ = READ_FPR(pSmi, FPR0C);
	save->FPR10_ = READ_FPR(pSmi, FPR10);
    }

    save->CPR00 = READ_CPR(pSmi, 0x00);

    if (!pSmi->ModeStructInit) {
	vgaHWCopyReg(&hwp->ModeReg, vgaSavePtr);
	memcpy(pSmi->mode, save, sizeof(SMIRegRec));
	pSmi->ModeStructInit = TRUE;
    }

    if (pSmi->useBIOS && pSmi->pInt10 != NULL) {
	pSmi->pInt10->num = 0x10;
	pSmi->pInt10->ax = 0x0F00;
	xf86ExecX86int10(pSmi->pInt10);
	save->mode = pSmi->pInt10->ax & 0x007F;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Current mode 0x%02X.\n",
		   save->mode);
    }

    if (xf86GetVerbosity() > 1) {
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		       "Saved current video mode.  Register dump:\n");
	SMI_PrintRegs(pScrn);
    }

    LEAVE();
}

/*
 * This function is used to restore a video mode. It writes out all of the
 * standard VGA and extended registers needed to setup a video mode.
 */

void
SMILynx_WriteMode(ScrnInfoPtr pScrn, vgaRegPtr vgaSavePtr, SMIRegPtr restore)
{
    SMIPtr	pSmi = SMIPTR(pScrn);
    int		i;
    CARD8	tmp;
    CARD32	offset;
    vgaHWPtr	hwp = VGAHWPTR(pScrn);
    int		vgaIOBase  = hwp->IOBase;
    int		vgaCRIndex = vgaIOBase + VGA_CRTC_INDEX_OFFSET;
    int		vgaCRData  = vgaIOBase + VGA_CRTC_DATA_OFFSET;

    ENTER();

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x17, restore->SR17);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x18, restore->SR18);

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x20, restore->SR20);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x21, restore->SR21);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x22, restore->SR22);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x23, restore->SR23);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x24, restore->SR24);

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x31, restore->SR31);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x32, restore->SR32);

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x66, restore->SR66);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x68, restore->SR68);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x69, restore->SR69);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6A, restore->SR6A);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6B, restore->SR6B);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6C, restore->SR6C);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6D, restore->SR6D);

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x81, restore->SR81);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0xA0, restore->SRA0);

    if (pSmi->useBIOS && restore->mode != 0){
	pSmi->pInt10->num = 0x10;
	pSmi->pInt10->ax = restore->mode | 0x80;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Setting mode 0x%02X\n",
		   restore->mode);
	xf86ExecX86int10(pSmi->pInt10);

	/* Enable linear mode. */
	outb(pSmi->PIOBase + VGA_SEQ_INDEX, 0x18);
	tmp = inb(pSmi->PIOBase + VGA_SEQ_DATA);
	outb(pSmi->PIOBase + VGA_SEQ_DATA, tmp | 0x01);

	/* Enable DPR/VPR registers. */
	tmp = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x21);
	VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x21, tmp & ~0x03);
    } else {
	/* Restore the standard VGA registers */
	vgaHWRestore(pScrn, vgaSavePtr, VGA_SR_ALL);
	if (restore->smiDACMask) {
	    VGAOUT8(pSmi, VGA_DAC_MASK, restore->smiDACMask);
	} else {
	    VGAOUT8(pSmi, VGA_DAC_MASK, 0xFF);
	}
	VGAOUT8(pSmi, VGA_DAC_WRITE_ADDR, 0);
	for (i = 0; i < 256; i++) {
	    VGAOUT8(pSmi, VGA_DAC_DATA, restore->smiDacRegs[i][0]);
	    VGAOUT8(pSmi, VGA_DAC_DATA, restore->smiDacRegs[i][1]);
	    VGAOUT8(pSmi, VGA_DAC_DATA, restore->smiDacRegs[i][2]);
	}
	for (i = 0, offset = 2; i < 8192; i++, offset += 8) {
	    *(pSmi->FBBase + offset) = restore->smiFont[i];
	}

	if (SMI_LYNXM_SERIES(pSmi->Chipset)) {
	    /* Restore secondary registers */
	    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x9E,
			  restore->CR90[14] | 0x20);

	    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x33, restore->CR33_2);
	    for (i = 0; i < 14; i++) {
		VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x40 + i,
			      restore->CR40_2[i]);
	    }
	    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x9F, restore->CR9F_2);

	    /* Restore primary registers */
	    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x9E,
			  restore->CR90[14] & ~0x20);

	    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x33, restore->CR33);
	    for (i = 0; i < 14; i++) {
		VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x40 + i,
			      restore->CR40[i]);
	    }
	    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x9F, restore->CR9F);

	    /* Restore common registers */
	    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x30, restore->CR30);
	    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x3A, restore->CR3A);

	    for (i = 0; i < 15; i++)
		VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x90 + i,
			      restore->CR90[i]);

	    for (i = 0; i < 14; i++)
		VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0xA0 + i,
			      restore->CRA0[i]);

	}else{
	    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x30, restore->CR30);
	    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x33, restore->CR33);
	    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x3A, restore->CR3A);
	    for (i = 0; i < 14; i++) {
		VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x40 + i,
			      restore->CR40[i]);
	    }
	}

	if (pSmi->Dualhead) {
	    /* dualhead stuff */
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x40, restore->SR40);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x41, restore->SR41);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x42, restore->SR42);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x43, restore->SR43);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x44, restore->SR44);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x45, restore->SR45);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x48, restore->SR48);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x49, restore->SR49);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x4A, restore->SR4A);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x4B, restore->SR4B);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x4C, restore->SR4C);

	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x50, restore->SR50);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x51, restore->SR51);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x52, restore->SR52);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x53, restore->SR53);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x54, restore->SR54);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x55, restore->SR55);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x56, restore->SR56);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x57, restore->SR57);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x5A, restore->SR5A);

	    /* PLL2 stuff */
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6E, restore->SR6E);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6F, restore->SR6F);
	}
    }

    /* Reset the graphics engine */
    WRITE_DPR(pSmi, 0x10, restore->DPR10);
    WRITE_DPR(pSmi, 0x1C, restore->DPR1C);
    WRITE_DPR(pSmi, 0x20, restore->DPR20);
    WRITE_DPR(pSmi, 0x24, restore->DPR24);
    WRITE_DPR(pSmi, 0x28, restore->DPR28);
    WRITE_DPR(pSmi, 0x2C, restore->DPR2C);
    WRITE_DPR(pSmi, 0x30, restore->DPR30);
    WRITE_DPR(pSmi, 0x3C, restore->DPR3C);
    WRITE_DPR(pSmi, 0x40, restore->DPR40);
    WRITE_DPR(pSmi, 0x44, restore->DPR44);

    /* write video controller regs */
    WRITE_VPR(pSmi, 0x00, restore->VPR00);
    WRITE_VPR(pSmi, 0x0C, restore->VPR0C);
    WRITE_VPR(pSmi, 0x10, restore->VPR10);

    if(pSmi->Chipset == SMI_COUGAR3DR) {
	WRITE_FPR(pSmi, FPR00, restore->FPR00_);
	WRITE_FPR(pSmi, FPR0C, restore->FPR0C_);
	WRITE_FPR(pSmi, FPR10, restore->FPR10_);
    }

    WRITE_CPR(pSmi, 0x00, restore->CPR00);

    if (xf86GetVerbosity() > 1) {
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		       "Done restoring mode.  Register dump:\n");
	SMI_PrintRegs(pScrn);
    }

    vgaHWProtect(pScrn, FALSE);

    LEAVE();
}


/*
 * SMI_DisplayPowerManagementSet -- Sets VESA Display Power Management
 * Signaling (DPMS) Mode.
 */
void
SMILynx_DisplayPowerManagementSet(ScrnInfoPtr pScrn, int PowerManagementMode,
							  int flags)
{
    SMIPtr	pSmi = SMIPTR(pScrn);
    SMIRegPtr	mode = pSmi->mode;
    vgaHWPtr	hwp = VGAHWPTR(pScrn);

    ENTER();

    /* If we already are in the requested DPMS mode, just return */
    if (pSmi->CurrentDPMS != PowerManagementMode) {
	/* Read the required SR registers for the DPMS handler */
	CARD8 SR01 = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x01);

	switch (PowerManagementMode) {
	    case DPMSModeOn:
		SR01 &= ~0x20; /* Screen on */
		mode->SR23 &= ~0xC0; /* Disable chip activity detection */
		break;
	    case DPMSModeStandby:
	    case DPMSModeSuspend:
	    case DPMSModeOff:
		SR01 |= 0x20; /* Screen off */
		mode->SR23 = (mode->SR23 & ~0x07) | 0xD8; /* Enable chip activity detection
							     Enable internal auto-standby mode
							     Enable both IO Write and Host Memory write detect
							     0 minutes timeout */
		break;
	}

	/* Wait for vertical retrace */
	while (hwp->readST01(hwp) & 0x8) ;
	while (!(hwp->readST01(hwp) & 0x8)) ;

	/* Write the registers */
	VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x01, SR01);
	VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x23, mode->SR23);

	/* Set the DPMS mode to every output and CRTC */
	xf86DPMSSet(pScrn, PowerManagementMode, flags);

	/* Save the current power state */
	pSmi->CurrentDPMS = PowerManagementMode;
    }

    LEAVE();
}

static unsigned int
SMILynx_ddc1Read(ScrnInfoPtr pScrn)
{
    register vgaHWPtr hwp = VGAHWPTR(pScrn);
    SMIPtr pSmi = SMIPTR(pScrn);
    unsigned int ret;

    ENTER();

    while (hwp->readST01(hwp) & 0x8) ;
    while (!(hwp->readST01(hwp) & 0x8)) ;

    ret = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x72) & 0x08;

    LEAVE(ret);
}

static void
SMILynx_ddc1SetSpeed(ScrnInfoPtr pScrn, xf86ddcSpeed speed)
{
    vgaHWddc1SetSpeed(pScrn, speed);
}

xf86MonPtr
SMILynx_ddc1(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    xf86MonPtr pMon;
    unsigned char tmp;

    ENTER();

    tmp = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x72);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x72, tmp | 0x20);

    pMon = xf86PrintEDID(xf86DoEDID_DDC1(XF86_SCRN_ARG(pScrn),
					 SMILynx_ddc1SetSpeed,
					 SMILynx_ddc1Read));
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x72, tmp);

    LEAVE(pMon);
}


/* This function is used to debug, it prints out the contents of Lynx regs */
void
SMILynx_PrintRegs(ScrnInfoPtr pScrn)
{
    unsigned char i;
    SMIPtr pSmi = SMIPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    int vgaCRIndex = hwp->IOBase + VGA_CRTC_INDEX_OFFSET;
    int vgaCRReg   = hwp->IOBase + VGA_CRTC_DATA_OFFSET;
    int vgaStatus  = hwp->IOBase + VGA_IN_STAT_1_OFFSET;

    xf86ErrorFVerb(VERBLEV, "MISCELLANEOUS OUTPUT\n    %02X\n",
		   VGAIN8(pSmi, VGA_MISC_OUT_R));

    xf86ErrorFVerb(VERBLEV, "\nSEQUENCER\n"
		   "    x0 x1 x2 x3  x4 x5 x6 x7  x8 x9 xA xB  xC xD xE xF");
    for (i = 0x00; i <= 0xAF; i++) {
	if ((i & 0xF) == 0x0) xf86ErrorFVerb(VERBLEV, "\n%02X|", i);
	if ((i & 0x3) == 0x0) xf86ErrorFVerb(VERBLEV, " ");
	xf86ErrorFVerb(VERBLEV, "%02X ",
		       VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, i));
    }

    xf86ErrorFVerb(VERBLEV, "\n\nCRT CONTROLLER\n"
		   "    x0 x1 x2 x3  x4 x5 x6 x7  x8 x9 xA xB  xC xD xE xF");
    for (i = 0x00; i <= 0xAD; i++) {
	if (i == 0x20) i = 0x30;
	if (i == 0x50) i = 0x90;
	if ((i & 0xF) == 0x0) xf86ErrorFVerb(VERBLEV, "\n%02X|", i);
	if ((i & 0x3) == 0x0) xf86ErrorFVerb(VERBLEV, " ");
	xf86ErrorFVerb(VERBLEV, "%02X ",
		       VGAIN8_INDEX(pSmi, vgaCRIndex, vgaCRReg, i));
    }

    xf86ErrorFVerb(VERBLEV, "\n\nGRAPHICS CONTROLLER\n"
		   "    x0 x1 x2 x3  x4 x5 x6 x7  x8 x9 xA xB  xC xD xE xF");
    for (i = 0x00; i <= 0x08; i++) {
	if ((i & 0xF) == 0x0) xf86ErrorFVerb(VERBLEV, "\n%02X|", i);
	if ((i & 0x3) == 0x0) xf86ErrorFVerb(VERBLEV, " ");
	xf86ErrorFVerb(VERBLEV, "%02X ",
		       VGAIN8_INDEX(pSmi, VGA_GRAPH_INDEX, VGA_GRAPH_DATA, i));
    }

    xf86ErrorFVerb(VERBLEV, "\n\nATTRIBUTE 0CONTROLLER\n"
		   "    x0 x1 x2 x3  x4 x5 x6 x7  x8 x9 xA xB  xC xD xE xF");
    for (i = 0x00; i <= 0x14; i++) {
	(void) VGAIN8(pSmi, vgaStatus);
	if ((i & 0xF) == 0x0) xf86ErrorFVerb(VERBLEV, "\n%02X|", i);
	if ((i & 0x3) == 0x0) xf86ErrorFVerb(VERBLEV, " ");
	xf86ErrorFVerb(VERBLEV, "%02X ",
		       VGAIN8_INDEX(pSmi, VGA_ATTR_INDEX, VGA_ATTR_DATA_R, i));
    }
    (void) VGAIN8(pSmi, vgaStatus);
    VGAOUT8(pSmi, VGA_ATTR_INDEX, 0x20);
}
