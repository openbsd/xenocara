/* $XFree86$ */
/* $XdotOrg: driver/xf86-video-sisusb/src/sisusb_vga.c,v 1.5 2005/07/11 02:30:00 ajax Exp $ */
/*
 * Mode setup and basic video bridge detection
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1) Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2) Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3) The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: 	Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sisusb.h"
#include "sisusb_regs.h"
#include "sisusb_dac.h"

/* Our very own vgaHW functions */
void SiSUSBVGASave(ScrnInfoPtr pScrn, SISUSBRegPtr save, int flags);
void SiSUSBVGARestore(ScrnInfoPtr pScrn, SISUSBRegPtr restore, int flags);
void SISUSBVGALock(SISUSBPtr pSiSUSB);
void SiSUSBVGAUnlock(SISUSBPtr pSiSUSB);
void SiSUSBVGAProtect(ScrnInfoPtr pScrn, Bool on);
Bool SiSUSBVGASaveScreen(ScreenPtr pScreen, int mode);

/* Init a mode.
 * This function is now only used for setting up some
 * variables (eg. scrnOffset).
 */
static Bool
SISUSB300Init(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    SISUSBPtr    pSiSUSB = SISUSBPTR(pScrn);
    SISUSBRegPtr pReg = &pSiSUSB->ModeReg;
    DisplayModePtr realmode = mode;

    /* Copy current register settings to structure */
    (*pSiSUSB->SiSSave)(pScrn, pReg);

    /* Calculate Offset/Display Pitch */
    pSiSUSB->scrnOffset = pSiSUSB->CurrentLayout.displayWidth *
                          ((pSiSUSB->CurrentLayout.bitsPerPixel + 7) / 8);

    pSiSUSB->scrnPitch = pSiSUSB->scrnPitch2 = pSiSUSB->scrnOffset;
    if(realmode->Flags & V_INTERLACE) pSiSUSB->scrnPitch <<= 1;

#ifdef UNLOCK_ALWAYS
    outSISIDXREG(pSiSUSB, SISSR, 0x05, 0x86);
#endif

    switch(pSiSUSB->CurrentLayout.bitsPerPixel) {
        case 8:
            pSiSUSB->DstColor = 0x0000;
	    pSiSUSB->SiS310_AccelDepth = 0x00000000;
            break;
        case 16:
	    pSiSUSB->DstColor = 0x8000;
	    pSiSUSB->SiS310_AccelDepth = 0x00010000;
            break;
        case 24:
            break;
        case 32:
            pSiSUSB->DstColor = 0xC000;
	    pSiSUSB->SiS310_AccelDepth = 0x00020000;
            break;
    }

    /* Enable PCI LINEAR ADDRESSING (0x80), MMIO (0x01), PCI_IO (0x20) */
    pReg->sisRegs3C4[0x20] = 0xA1;

    if(!pSiSUSB->NoAccel) {
        pReg->sisRegs3C4[0x1E] |= 0x42;  /* Enable 2D accelerator */
	pReg->sisRegs3C4[0x1E] |= 0x18;  /* Enable 3D accelerator */
#ifndef SISVRAMQ
	/* See comments in sis_driver.c */
	pReg->sisRegs3C4[0x27] = 0x1F;
	pReg->sisRegs3C4[0x26] = 0x22;
	pReg->sisMMIO85C0 = (pScrn->videoRam - 512) * 1024;
#endif
    }

    return TRUE;
}

/* Detect video bridge and set VBFlags accordingly */
void SISUSBVGAPreInit(ScrnInfoPtr pScrn)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    pSiSUSB->ModeInit = SISUSB300Init;

    pSiSUSB->VBFlags = pSiSUSB->VBFlags2 = 0; /* reset VBFlags */
}

static void
SiSUSB_WriteAttr(SISUSBPtr pSiSUSB, int index, int value)
{
    (void)inSISREG(pSiSUSB, SISINPSTAT);
    index |= 0x20;
    outSISREG(pSiSUSB, SISAR, index);
    outSISREG(pSiSUSB, SISAR, value);
}

static int
SiSUSB_ReadAttr(SISUSBPtr pSiSUSB, int index)
{
    (void)inSISREG(pSiSUSB, SISINPSTAT);
    index |= 0x20;
    outSISREG(pSiSUSB, SISAR, index);
    return(inSISREG(pSiSUSB, SISARR));
}

static void
SiSUSB_EnablePalette(SISUSBPtr pSiSUSB)
{
    (void)inSISREG(pSiSUSB, SISINPSTAT);
    outSISREG(pSiSUSB, SISAR, 0x00);
    pSiSUSB->VGAPaletteEnabled = TRUE;
}

static void
SiSUSB_DisablePalette(SISUSBPtr pSiSUSB)
{
    (void)inSISREG(pSiSUSB, SISINPSTAT);
    outSISREG(pSiSUSB, SISAR, 0x20);
    pSiSUSB->VGAPaletteEnabled = FALSE;
}

void
SISUSBVGALock(SISUSBPtr pSiSUSB)
{
    orSISIDXREG(pSiSUSB, SISCR, 0x11, 0x80);  	/* Protect CRTC[0-7] */
}

void
SiSUSBVGAUnlock(SISUSBPtr pSiSUSB)
{
    andSISIDXREG(pSiSUSB, SISCR, 0x11, 0x7f);	/* Unprotect CRTC[0-7] */
}

static void
SiSUSBVGASaveMode(ScrnInfoPtr pScrn, SISUSBRegPtr save)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    int i;

    save->sisRegMiscOut = inSISREG(pSiSUSB, SISMISCR);

    for(i = 0; i < 25; i++) {
       inSISIDXREG(pSiSUSB, SISCR, i, save->sisRegs3D4[i]);
    }

    SiSUSB_EnablePalette(pSiSUSB);
    for(i = 0; i < 21; i++) {
       save->sisRegsATTR[i] = SiSUSB_ReadAttr(pSiSUSB, i);
    }
    SiSUSB_DisablePalette(pSiSUSB);

    for(i = 0; i < 9; i++) {
       inSISIDXREG(pSiSUSB, SISGR, i, save->sisRegsGR[i]);
    }

    for(i = 1; i < 5; i++) {
       inSISIDXREG(pSiSUSB, SISSR, i, save->sisRegs3C4[i]);
    }
}

static void
SiSUSBVGASaveColormap(ScrnInfoPtr pScrn, SISUSBRegPtr save)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    int i;

    if(pSiSUSB->VGACMapSaved) return;

    outSISREG(pSiSUSB, SISPEL, 0xff);

    outSISREG(pSiSUSB, SISCOLIDXR, 0x00);
    for(i = 0; i < 768; i++) {
       save->sisDAC[i] = inSISREG(pSiSUSB, SISCOLDATA);
       (void)inSISREG(pSiSUSB, SISINPSTAT);
       (void)inSISREG(pSiSUSB, SISINPSTAT);
    }

    SiSUSB_DisablePalette(pSiSUSB);
    pSiSUSB->VGACMapSaved = TRUE;
}

void
SiSUSBVGASave(ScrnInfoPtr pScrn, SISUSBRegPtr save, int flags)
{
    if(save == NULL) return;

    if(flags & SISVGA_SR_CMAP)  SiSUSBVGASaveColormap(pScrn, save);
    if(flags & SISVGA_SR_MODE)  SiSUSBVGASaveMode(pScrn, save);
}

static void
SiSUSBVGARestoreMode(ScrnInfoPtr pScrn, SISUSBRegPtr restore)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    int i;

    outSISREG(pSiSUSB, SISMISCW, restore->sisRegMiscOut);

    for(i = 1; i < 5; i++) {
       outSISIDXREG(pSiSUSB, SISSR, i, restore->sisRegs3C4[i]);
    }

    outSISIDXREG(pSiSUSB, SISCR, 17, restore->sisRegs3D4[17] & ~0x80);

    for(i = 0; i < 25; i++) {
       outSISIDXREG(pSiSUSB, SISCR, i, restore->sisRegs3D4[i]);
    }

    for(i = 0; i < 9; i++) {
       outSISIDXREG(pSiSUSB, SISGR, i, restore->sisRegsGR[i]);
    }

    SiSUSB_EnablePalette(pSiSUSB);
    for(i = 0; i < 21; i++) {
       SiSUSB_WriteAttr(pSiSUSB, i, restore->sisRegsATTR[i]);
    }
    SiSUSB_DisablePalette(pSiSUSB);
}

static void
SiSUSBVGARestoreColormap(ScrnInfoPtr pScrn, SISUSBRegPtr restore)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    int i;

    if(!pSiSUSB->VGACMapSaved) return;

    outSISREG(pSiSUSB, SISPEL, 0xff);

    outSISREG(pSiSUSB, SISCOLIDX, 0x00);
    for(i = 0; i < 768; i++) {
       outSISREG(pSiSUSB, SISCOLDATA, restore->sisDAC[i]);
       (void)inSISREG(pSiSUSB, SISINPSTAT);
       (void)inSISREG(pSiSUSB, SISINPSTAT);
    }

    SiSUSB_DisablePalette(pSiSUSB);
}

void
SiSUSBVGARestore(ScrnInfoPtr pScrn, SISUSBRegPtr restore, int flags)
{
    if(restore == NULL) return;

    if(flags & SISVGA_SR_MODE)  SiSUSBVGARestoreMode(pScrn, restore);
    if(flags & SISVGA_SR_CMAP)  SiSUSBVGARestoreColormap(pScrn, restore);
}

static void
SiSUSB_SeqReset(SISUSBPtr pSiSUSB, Bool start)
{
    if(start) {
       outSISIDXREG(pSiSUSB, SISSR, 0x00, 0x01);	/* Synchronous Reset */
    } else {
       outSISIDXREG(pSiSUSB, SISSR, 0x00, 0x03);	/* End Reset */
    }
}

void
SiSUSBVGAProtect(ScrnInfoPtr pScrn, Bool on)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    UChar  tmp;

    if(!pScrn->vtSema) return;

    if(on) {
       inSISIDXREG(pSiSUSB, SISSR, 0x01, tmp);
       SiSUSB_SeqReset(pSiSUSB, TRUE);		/* start synchronous reset */
       outSISIDXREG(pSiSUSB, SISSR, 0x01, tmp | 0x20);	/* disable display */
       SiSUSB_EnablePalette(pSiSUSB);
    } else {
       andSISIDXREG(pSiSUSB, SISSR, 0x01, ~0x20);	/* enable display */
       SiSUSB_SeqReset(pSiSUSB, FALSE);		/* clear synchronous reset */
       SiSUSB_DisablePalette(pSiSUSB);
    }
}

static void
SISUSBVGABlankScreen(ScrnInfoPtr pScrn, Bool on)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    UChar  tmp;

    inSISIDXREG(pSiSUSB, SISSR, 0x01, tmp);
    if(on) tmp &= ~0x20;
    else   tmp |= 0x20;
    SiSUSB_SeqReset(pSiSUSB, TRUE);
    outSISIDXREG(pSiSUSB, SISSR, 0x01, tmp);
    SiSUSB_SeqReset(pSiSUSB, FALSE);
}

Bool
SiSUSBVGASaveScreen(ScreenPtr pScreen, int mode)
{
    ScrnInfoPtr pScrn = NULL;
    Bool on = xf86IsUnblank(mode);

    if(pScreen == NULL) return FALSE;

    pScrn = xf86Screens[pScreen->myNum];

    if(pScrn->vtSema) {
       SISUSBVGABlankScreen(pScrn, on);
    }
    return TRUE;
}




