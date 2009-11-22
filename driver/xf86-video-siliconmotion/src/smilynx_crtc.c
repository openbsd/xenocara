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

static void
SMILynx_CrtcVideoInit_crt(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    int pitch;

    ENTER();

    switch (pScrn->bitsPerPixel) {
    case 8:
	WRITE_VPR(pSmi, 0x00, 0x00000000);
	break;
    case 16:
	WRITE_VPR(pSmi, 0x00, 0x00020000);
	break;
    case 24:
	WRITE_VPR(pSmi, 0x00, 0x00040000);
	break;
    case 32:
	WRITE_VPR(pSmi, 0x00, 0x00030000);
	break;
    }

    pitch = (crtc->rotatedData? crtc->mode.HDisplay : pScrn->displayWidth) * pSmi->Bpp;
    pitch = (pitch + 15) & ~15;

    WRITE_VPR(pSmi, 0x10, (crtc->mode.HDisplay * pSmi->Bpp) >> 3 << 16 | pitch >> 3);

    LEAVE();
}

static void
SMILynx_CrtcVideoInit_lcd(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    SMIRegPtr mode = pSmi->mode;
    CARD16 fifo_readoffset,fifo_writeoffset;

    ENTER();

    /* Set display depth */
    if (pScrn->bitsPerPixel > 8)
	mode->SR31 |= 0x40; /* 16 bpp */
    else
	mode->SR31 &= ~0x40; /* 8 bpp */

    /* FIFO1/2 Read Offset*/
    fifo_readoffset = (crtc->rotatedData? crtc->mode.HDisplay : pScrn->displayWidth) * pSmi->Bpp;
    fifo_readoffset = ((fifo_readoffset + 15) & ~15) >> 3;

    /* FIFO1 Read Offset */
    mode->SR44 = fifo_readoffset & 0x000000FF;
    /* FIFO2 Read Offset */
    mode->SR4B = fifo_readoffset & 0x000000FF;

    if(pSmi->Chipset == SMI_LYNX3DM){
	/* FIFO1/2 Read Offset overflow */
	mode->SR4C = (((fifo_readoffset & 0x00000300) >> 8) << 2) |
	    (((fifo_readoffset & 0x00000300) >> 8) << 6);
    }else{
	/* FIFO1 Read Offset overflow */
	mode->SR45 = (mode->SR45 & 0x3F) | ((fifo_readoffset & 0x00000300) >> 8) << 6;
	/* FIFO2 Read Offset overflow */
	mode->SR4C = (((fifo_readoffset & 0x00000300) >> 8) << 6);
    }

    /* FIFO Write Offset */
    fifo_writeoffset = crtc->mode.HDisplay * pSmi->Bpp >> 3;
    mode->SR48 = fifo_writeoffset & 0x000000FF;
    mode->SR49 = (fifo_writeoffset & 0x00000300) >> 8;

    /* set FIFO levels */
    mode->SR4A = 0x41;

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x31, mode->SR31);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x44, mode->SR44);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x45, mode->SR45);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x48, mode->SR48);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x49, mode->SR49);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x4A, mode->SR4A);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x4B, mode->SR4B);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x4C, mode->SR4C);

    LEAVE();
}

static void
SMI730_CrtcVideoInit(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    int pitch;

    ENTER();

    switch (pScrn->bitsPerPixel) {
    case 8:
	WRITE_VPR(pSmi, 0x00, 0x00000000);
	WRITE_FPR(pSmi, FPR00, 0x00080000);
	break;
    case 16:
	WRITE_VPR(pSmi, 0x00, 0x00020000);
	WRITE_FPR(pSmi, FPR00, 0x000A0000);
	break;
    case 24:
	WRITE_VPR(pSmi, 0x00, 0x00040000);
	WRITE_FPR(pSmi, FPR00, 0x000C0000);
	break;
    case 32:
	WRITE_VPR(pSmi, 0x00, 0x00030000);
	WRITE_FPR(pSmi, FPR00, 0x000B0000);
	break;
    }

    pitch = (crtc->rotatedData? crtc->mode.HDisplay : pScrn->displayWidth) * pSmi->Bpp;
    pitch = (pitch + 15) & ~15;

    WRITE_VPR(pSmi, 0x10, (crtc->mode.HDisplay * pSmi->Bpp) >> 3 << 16 | pitch >> 3);
    WRITE_FPR(pSmi, FPR10, (crtc->mode.HDisplay * pSmi->Bpp) >> 3 << 16 | pitch >> 3);

    LEAVE();
}

static void
SMILynx_CrtcAdjustFrame(xf86CrtcPtr crtc, int x, int y)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    SMIRegPtr mode = pSmi->mode;
    xf86CrtcConfigPtr crtcConf = XF86_CRTC_CONFIG_PTR(pScrn);
    CARD32 Base;

    ENTER();

    if(crtc->rotatedData)
	Base = (char*)crtc->rotatedData - (char*)pSmi->FBBase;
    else
	Base = pSmi->FBOffset + (x + y * pScrn->displayWidth) * pSmi->Bpp;


    if (SMI_LYNX3D_SERIES(pSmi->Chipset) ||
	     SMI_COUGAR_SERIES(pSmi->Chipset)) {
	Base = (Base + 15) & ~15;
	while ((Base % pSmi->Bpp) > 0) {
	    Base -= 16;
	}
    } else {
	Base = (Base + 7) & ~7;
	while ((Base % pSmi->Bpp) > 0)
	    Base -= 8;
    }

    Base >>= 3;

    if(SMI_COUGAR_SERIES(pSmi->Chipset)){
	WRITE_VPR(pSmi, 0x0C, Base);
	WRITE_FPR(pSmi, FPR0C, Base);
    }else{
	if(pSmi->Dualhead && crtc == crtcConf->crtc[1]){
	    /* LCD */

	    /* FIFO1 read start address */
	    mode->SR40 = Base & 0x000000FF;
	    mode->SR41 = (Base & 0x0000FF00) >> 8;

	    /* FIFO2 read start address */
	    mode->SR42 = Base & 0x000000FF;
	    mode->SR43 = (Base & 0x0000FF00) >> 8;

	    /* FIFO1/2 read start address overflow */
	    if(pSmi->Chipset == SMI_LYNX3DM)
		mode->SR45 = (Base & 0x000F0000) >> 16 | (Base & 0x000F0000) >> 16 << 4;
	    else
		mode->SR45 = (mode->SR45 & 0xC0) |
		    (Base & 0x00070000) >> 16 | (Base & 0x00070000) >> 16 << 3;

	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x40, mode->SR40);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x41, mode->SR41);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x42, mode->SR42);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x43, mode->SR43);
	    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x45, mode->SR45);

	}else{
	    /* CRT or single head */
	    WRITE_VPR(pSmi, 0x0C, Base);
	}
    }

    LEAVE();
}

static Bool
SMILynx_CrtcModeFixup(xf86CrtcPtr crtc,
		      DisplayModePtr mode,
		      DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    if (pSmi->Chipset == SMI_LYNXEMplus) {
	/* Adjust the pixel clock in case it is near one of the known
	   stable frequencies (KHz) */
	int stable_clocks[] = {46534,};
	int epsilon = 3000;
	int i;

	for (i=0; i < sizeof(stable_clocks)/sizeof(int); i++) {
	    if ( abs(mode->Clock - stable_clocks[i]) < epsilon) {
		adjusted_mode->Clock = stable_clocks[i];
		break;
	    }
	}
    }

    LEAVE(TRUE);
}

static void
SMILynx_CrtcModeSet_vga(xf86CrtcPtr crtc,
	    DisplayModePtr mode,
	    DisplayModePtr adjusted_mode,
	    int x, int y)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    SMIRegPtr reg = pSmi->mode;
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    int vgaIOBase  = hwp->IOBase;
    int vgaCRIndex = vgaIOBase + VGA_CRTC_INDEX_OFFSET;
    int vgaCRData  = vgaIOBase + VGA_CRTC_DATA_OFFSET;
    vgaRegPtr vganew = &hwp->ModeReg;

    ENTER();

    /* Initialize Video Processor Registers */

    SMICRTC(crtc)->video_init(crtc);
    SMILynx_CrtcAdjustFrame(crtc, x,y);


    /* Program the PLL */

    /* calculate vclk1 */
    if (SMI_LYNX_SERIES(pSmi->Chipset)) {
        SMI_CommonCalcClock(pScrn->scrnIndex, adjusted_mode->Clock,
			1, 1, 63, 0, 3,
                        pSmi->clockRange.minClock,
                        pSmi->clockRange.maxClock,
                        &reg->SR6C, &reg->SR6D);
    } else {
        SMI_CommonCalcClock(pScrn->scrnIndex, adjusted_mode->Clock,
			1, 1, 63, 0, 1,
                        pSmi->clockRange.minClock,
                        pSmi->clockRange.maxClock,
                        &reg->SR6C, &reg->SR6D);
    }

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6C, reg->SR6C);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6D, reg->SR6D);


    /* Adjust mode timings */

    if (!vgaHWInit(pScrn, mode)) {
	LEAVE();
    }

    if ((mode->HDisplay == 640) && SMI_LYNXM_SERIES(pSmi->Chipset)) {
	vganew->MiscOutReg &= ~0x0C;
    } else {
	vganew->MiscOutReg |= 0x0C;
    }
    vganew->MiscOutReg |= 0x20;

    {
	unsigned long HTotal=(mode->CrtcHTotal>>3)-5;
	unsigned long HBlankEnd=(mode->CrtcHBlankEnd>>3)-1;
	unsigned long VTotal=mode->CrtcVTotal-2;
	unsigned long VDisplay=mode->CrtcVDisplay-1;
	unsigned long VBlankStart=mode->CrtcVBlankStart-1;
	unsigned long VBlankEnd=mode->CrtcVBlankEnd-1;
	unsigned long VSyncStart=mode->CrtcVSyncStart;

	/* Fix HBlankEnd/VBlankEnd */
	if((mode->CrtcHBlankEnd >> 3) == (mode->CrtcHTotal >> 3)) HBlankEnd=0;
	if(mode->CrtcVBlankEnd == mode->CrtcVTotal) VBlankEnd=0;

	vganew->CRTC[3] = (vganew->CRTC[3] & ~0x1F) | (HBlankEnd & 0x1F);
	vganew->CRTC[5] = (vganew->CRTC[5] & ~0x80) | (HBlankEnd & 0x20) >> 5 << 7;
	vganew->CRTC[22] = VBlankEnd & 0xFF;

	/* Write the overflow from several VGA registers */
	reg->CR30 = (VTotal & 0x400) >> 10 << 3 |
	    (VDisplay & 0x400) >> 10 << 2 |
	    (VBlankStart & 0x400) >> 10 << 1 |
	    (VSyncStart & 0x400) >> 10 << 0;

	if(pSmi->Chipset == SMI_LYNX3DM)
	    reg->CR30 |= (HTotal & 0x100) >> 8 << 6;

	reg->CR33 = (HBlankEnd & 0xC0) >> 6 << 5 | (VBlankEnd & 0x300) >> 8 << 3;
    }

    vgaHWRestore(pScrn, vganew, VGA_SR_MODE);

    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x30, reg->CR30);
    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x33, reg->CR33);

    LEAVE();
}

static void
SMILynx_CrtcModeSet_crt(xf86CrtcPtr crtc,
	    DisplayModePtr mode,
	    DisplayModePtr adjusted_mode,
	    int x, int y)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    SMIRegPtr reg = pSmi->mode;
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    int vgaIOBase  = hwp->IOBase;
    int	vgaCRIndex = vgaIOBase + VGA_CRTC_INDEX_OFFSET;
    int	vgaCRData  = vgaIOBase + VGA_CRTC_DATA_OFFSET;
    int i;

    ENTER();

    /* Initialize Video Processor Registers */

    SMILynx_CrtcVideoInit_crt(crtc);
    SMILynx_CrtcAdjustFrame(crtc, x,y);


    /* Program the PLL */

    /* calculate vclk1 */
    if (SMI_LYNX_SERIES(pSmi->Chipset)) {
        SMI_CommonCalcClock(pScrn->scrnIndex, adjusted_mode->Clock,
			1, 1, 63, 0, 3,
                        pSmi->clockRange.minClock,
                        pSmi->clockRange.maxClock,
                        &reg->SR6C, &reg->SR6D);
    } else {
        SMI_CommonCalcClock(pScrn->scrnIndex, adjusted_mode->Clock,
			1, 1, 63, 0, 1,
                        pSmi->clockRange.minClock,
                        pSmi->clockRange.maxClock,
                        &reg->SR6C, &reg->SR6D);
    }

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6C, reg->SR6C);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6D, reg->SR6D);


    /* Adjust mode timings */
    /* In virtual refresh mode, the CRT timings are controlled through
       the shadow VGA registers */

    {
	unsigned long HTotal=(mode->CrtcHTotal>>3)-5;
	unsigned long HDisplay=(mode->CrtcHDisplay>>3)-1;
	unsigned long HBlankStart=(mode->CrtcHBlankStart>>3)-1;
	unsigned long HBlankEnd=(mode->CrtcHBlankEnd>>3)-1;
	unsigned long HSyncStart=mode->CrtcHSyncStart>>3;
	unsigned long HSyncEnd=mode->CrtcHSyncEnd>>3;
	unsigned long VTotal=mode->CrtcVTotal-2;
	unsigned long VDisplay=mode->CrtcVDisplay-1;
	unsigned long VBlankStart=mode->CrtcVBlankStart-1;
	unsigned long VBlankEnd=mode->CrtcVBlankEnd-1;
	unsigned long VSyncStart=mode->CrtcVSyncStart;
	unsigned long VSyncEnd=mode->CrtcVSyncEnd;

	/* Fix HBlankEnd/VBlankEnd */
	if((mode->CrtcHBlankEnd >> 3) == (mode->CrtcHTotal >> 3)) HBlankEnd=0;
	if(mode->CrtcVBlankEnd == mode->CrtcVTotal) VBlankEnd=0;

	reg->CR40 [0x0] = HTotal & 0xFF;
	reg->CR40 [0x1] = HBlankStart & 0xFF;
	reg->CR40 [0x2] = HBlankEnd & 0x1F;
	reg->CR40 [0x3] = HSyncStart & 0xFF;
	reg->CR40 [0x4] = (HBlankEnd & 0x20) >> 5 << 7 |
	    (HSyncEnd & 0x1F);
	reg->CR40 [0x5] = VTotal & 0xFF;
	reg->CR40 [0x6] = VBlankStart & 0xFF;
	reg->CR40 [0x7] = VBlankEnd & 0xFF;
	reg->CR40 [0x8] = VSyncStart & 0xFF;
	reg->CR40 [0x9] = VSyncEnd & 0x0F;
	reg->CR40 [0xA] = (VSyncStart & 0x200) >> 9 << 7 |
	    (VDisplay & 0x200) >> 9 << 6 |
	    (VTotal & 0x200) >> 9 << 5 |
	    (VBlankStart & 0x100) >> 8 << 3 |
	    (VSyncStart & 0x100) >> 8 << 2 |
	    (VDisplay & 0x100) >> 8 << 1 |
	    (VTotal & 0x100) >> 8 << 0;
	reg->CR40 [0xB] = ((mode->Flags & V_NVSYNC)?1:0) << 7 |
	    ((mode->Flags & V_NHSYNC)?1:0) << 6 |
	    (VBlankStart & 0x200) >> 9 << 5;
	reg->CR40 [0xC] = HDisplay & 0xFF;
	reg->CR40 [0xD] = VDisplay & 0xFF;

	reg->CR30 = (VTotal & 0x400) >> 10 << 3 |
	    (VDisplay & 0x400) >> 10 << 2 |
	    (VBlankStart & 0x400) >> 10 << 1 |
	    (VSyncStart & 0x400) >> 10 << 0;

	if(pSmi->Chipset == SMI_LYNX3DM)
	    reg->CR30 |= (HTotal & 0x100) >> 8 << 6;

	reg->CR33 = (HBlankEnd & 0xC0) >> 6 << 5 | (VBlankEnd & 0x300) >> 8 << 3;

    }

    /* Select primary set of shadow registers */
    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x9E, reg->CR90[0xE] & ~0x20);

    for(i=0; i <= 0xD; i++)
	VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x40 + i, reg->CR40[i]);

    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x30, reg->CR30);
    VGAOUT8_INDEX(pSmi, vgaCRIndex, vgaCRData, 0x33, reg->CR33);

    LEAVE();
}

static void
SMILynx_CrtcModeSet_lcd(xf86CrtcPtr crtc,
	    DisplayModePtr mode,
	    DisplayModePtr adjusted_mode,
	    int x, int y)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    SMIRegPtr reg = pSmi->mode;

    ENTER();

    /* Initialize the flat panel video processor */

    SMILynx_CrtcVideoInit_lcd(crtc);
    SMILynx_CrtcAdjustFrame(crtc,x,y);


    /* Program the PLL */

    /* calculate vclk2 */
    if (SMI_LYNX_SERIES(pSmi->Chipset)) {
        SMI_CommonCalcClock(pScrn->scrnIndex, adjusted_mode->Clock,
			1, 1, 63, 0, 0,
                        pSmi->clockRange.minClock,
                        pSmi->clockRange.maxClock,
                        &reg->SR6E, &reg->SR6F);
    } else {
        SMI_CommonCalcClock(pScrn->scrnIndex, adjusted_mode->Clock,
			1, 1, 63, 0, 1,
                        pSmi->clockRange.minClock,
                        pSmi->clockRange.maxClock,
                        &reg->SR6E, &reg->SR6F);
    }

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6E, reg->SR6E);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6F, reg->SR6F);


    /* Adjust mode timings */
    {
	unsigned long HTotal=(mode->CrtcHTotal>>3)-1;
	unsigned long HDisplay=(mode->CrtcHDisplay>>3)-1;
	unsigned long HSyncStart=(mode->CrtcHSyncStart>>3);
	unsigned long HSyncWidth=((mode->CrtcHSyncEnd - mode->CrtcHSyncStart) >> 3) - 1;
	unsigned long VTotal=mode->CrtcVTotal-1;
	unsigned long VDisplay=mode->CrtcVDisplay-1;
	unsigned long VSyncStart=mode->CrtcVSyncStart-1;
	unsigned long VSyncWidth=mode->CrtcVSyncEnd - mode->CrtcVSyncStart - 1;

	reg->SR50 = (VTotal & 0x700) >> 8 << 1 |
	    (HSyncStart & 0x100) >> 8 << 0;
	reg->SR51 = (VSyncStart & 0x700) >> 8 << 5 |
	    (VDisplay & 0x700) >> 8 << 2 |
	    (HDisplay & 0x100) >> 8 << 1 |
	    (HTotal & 0x100) >> 8 << 0;
	reg->SR52 = HTotal & 0xFF;
	reg->SR53 = HDisplay & 0xFF;
	reg->SR54 = HSyncStart & 0xFF;
	reg->SR55 = VTotal & 0xFF;
	reg->SR56 = VDisplay & 0xFF;
	reg->SR57 = VSyncStart & 0xFF;
	reg->SR5A = (HSyncWidth & 0x1F) << 3 |
	    (VSyncWidth & 0x07) << 0;

	/* XXX - Why is the polarity hardcoded here? */
	reg->SR32 &= ~0x18;
	if (mode->HDisplay == 800) {
	    reg->SR32 |= 0x18;
	}
	if ((mode->HDisplay == 1024) && SMI_LYNXM_SERIES(pSmi->Chipset)) {
	    reg->SR32 |= 0x18;
	}
    }

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x32, reg->SR32);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x50, reg->SR50);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x51, reg->SR51);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x52, reg->SR52);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x53, reg->SR53);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x54, reg->SR54);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x55, reg->SR55);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x56, reg->SR56);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x57, reg->SR57);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x5A, reg->SR5A);

    LEAVE();
}

static void
SMILynx_CrtcModeSet_bios(xf86CrtcPtr crtc,
	    DisplayModePtr mode,
	    DisplayModePtr adjusted_mode,
	    int x, int y)
{
    ScrnInfoPtr pScrn=crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    SMIRegPtr reg = pSmi->mode;
    int i;
    CARD8 tmp;

    ENTER();

    /* Find the INT 10 mode number */
    {
	static struct {
	    int x, y, bpp;
	    CARD16 mode;
	} modeTable[] =
	    {
		{  640,  480,  8, 0x50 },
		{  640,  480, 16, 0x52 },
		{  640,  480, 24, 0x53 },
		{  640,  480, 32, 0x54 },
		{  800,  480,  8, 0x4A },
		{  800,  480, 16, 0x4C },
		{  800,  480, 24, 0x4D },
		{  800,  600,  8, 0x55 },
		{  800,  600, 16, 0x57 },
		{  800,  600, 24, 0x58 },
		{  800,  600, 32, 0x59 },
		{ 1024,  768,  8, 0x60 },
		{ 1024,  768, 16, 0x62 },
		{ 1024,  768, 24, 0x63 },
		{ 1024,  768, 32, 0x64 },
		{ 1280, 1024,  8, 0x65 },
		{ 1280, 1024, 16, 0x67 },
		{ 1280, 1024, 24, 0x68 },
		{ 1280, 1024, 32, 0x69 },
	    };

	reg->mode = 0;
	for (i = 0; i < sizeof(modeTable) / sizeof(modeTable[0]); i++) {
	    if ((modeTable[i].x == mode->HDisplay) &&
		(modeTable[i].y == mode->VDisplay) &&
		(modeTable[i].bpp == pScrn->bitsPerPixel)) {
		reg->mode = modeTable[i].mode;
		break;
	    }
	}
    }

    if(!reg->mode){
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "SMILynx_CrtcModeSet_bios: Not a known BIOS mode: "
		   "falling back to direct modesetting.\n");
	SMILynx_CrtcModeSet_vga(crtc,mode,adjusted_mode,x,y);
	LEAVE();
    }

    pSmi->pInt10->num = 0x10;
    pSmi->pInt10->ax = reg->mode | 0x80;
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Setting mode 0x%02X\n",
	       reg->mode);
    xf86ExecX86int10(pSmi->pInt10);

    /* Enable linear mode. */
    outb(pSmi->PIOBase + VGA_SEQ_INDEX, 0x18);
    tmp = inb(pSmi->PIOBase + VGA_SEQ_DATA);
    outb(pSmi->PIOBase + VGA_SEQ_DATA, tmp | 0x01);

    /* Enable DPR/VPR registers. */
    tmp = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x21);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x21, tmp & ~0x03);


    /* Initialize Video Processor Registers */

    SMICRTC(crtc)->video_init(crtc);
    SMILynx_CrtcAdjustFrame(crtc, x,y);

    LEAVE();
}

static void
SMILynx_CrtcLoadLUT_crt(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    SMIRegPtr mode = pSmi->mode;
    SMICrtcPrivatePtr crtcPriv = SMICRTC(crtc);
    int i;

    ENTER();

    /* Write CRT RAM only */
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX,VGA_SEQ_DATA,0x66,(mode->SR66 & ~0x30) | 0x20);

    for(i=0;i<256;i++){
	VGAOUT8(pSmi, VGA_DAC_WRITE_ADDR, i);
	VGAOUT8(pSmi, VGA_DAC_DATA, crtcPriv->lut_r[i] >> 8);
	VGAOUT8(pSmi, VGA_DAC_DATA, crtcPriv->lut_g[i] >> 8);
	VGAOUT8(pSmi, VGA_DAC_DATA, crtcPriv->lut_b[i] >> 8);
    }

    LEAVE();
}

static void
SMILynx_CrtcLoadLUT_lcd(xf86CrtcPtr crtc)
{
    ENTER();

    /* XXX - Is it possible to load LCD LUT in Virtual Refresh mode? */

    LEAVE();
}

static void
SMILynx_CrtcSetCursorColors_crt (xf86CrtcPtr crtc, int bg, int fg)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    CARD8 packedFG,packedBG;

    ENTER();

    /* Pack the true color into 8 bit */
    packedFG = (fg & 0xE00000) >> 16 |
	(fg & 0x00E000) >> 11 |
	(fg & 0x0000C0) >> 6;
    packedBG = (bg & 0xE00000) >> 16 |
	(bg & 0x00E000) >> 11 |
	(bg & 0x0000C0) >> 6;

    /* Program the colors */
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x8C, packedFG);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x8D, packedBG);

    /* Program FPR copy when on the 730 */
    if (pSmi->Chipset == SMI_COUGAR3DR) {
	CARD32 fpr15c;

	fpr15c  = READ_FPR(pSmi, FPR15C) & FPR15C_MASK_HWCADDREN;
	fpr15c |= packedFG;
	fpr15c |= packedBG << 8;
	WRITE_FPR(pSmi, FPR15C, fpr15c);
    }

    LEAVE();
}

static void
SMILynx_CrtcSetCursorPosition_crt (xf86CrtcPtr crtc, int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    if (x >= 0) {
	VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x88,
		      x & 0xFF);
	VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x89,
		      (x >> 8) & 0x07);
    }
    else {
	VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x88,
		      (-x) & (SMILYNX_MAX_CURSOR - 1));
	VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x89,
		      0x08);
    }

    if (y >= 0) {
	VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x8A,
		      y & 0xFF);
	VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x8B,
		      (y >> 8) & 0x07);
    }
    else {
	VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x8A,
			  (-y) & (SMILYNX_MAX_CURSOR - 1));
	VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA,
		      0x8B, 0x08);
    }

    /* Program FPR copy when on the 730 */
    if (pSmi->Chipset == SMI_COUGAR3DR) {
	CARD32 fpr158;

	if (x >= 0)
	    fpr158 = (x & FPR158_MASK_MAXBITS) << 16;
	else
	    fpr158 = ((-x & FPR158_MASK_MAXBITS) |
		      FPR158_MASK_BOUNDARY) << 16;

	if (y >= 0)
	    fpr158 |= y & FPR158_MASK_MAXBITS;
	else
	    fpr158 |= (-y & FPR158_MASK_MAXBITS) | FPR158_MASK_BOUNDARY;

	/* Program combined coordinates */
	WRITE_FPR(pSmi, FPR158, fpr158);
    }

    LEAVE();
}

static void
SMILynx_CrtcShowCursor_crt (xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    char tmp;

    ENTER();

    /* Show cursor */
    tmp = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x81);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x81, tmp | 0x80);

    /* Program FPR copy when on the 730 */
    if (pSmi->Chipset == SMI_COUGAR3DR) {
	CARD32 fpr15c;

	/* turn on the top bit */
	fpr15c  = READ_FPR(pSmi, FPR15C);
	fpr15c |= FPR15C_MASK_HWCENABLE;
	WRITE_FPR(pSmi, FPR15C, fpr15c);
    }

    LEAVE();
}

static void
SMILynx_CrtcHideCursor_crt (xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    char tmp;

    ENTER();

    /* Hide cursor */
    tmp = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x81);
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x81, tmp & ~0x80);

    /* Program FPR copy when on the 730 */
    if (pSmi->Chipset == SMI_COUGAR3DR) {
	CARD32 fpr15c;

	/* turn off the top bit */
	fpr15c  = READ_FPR(pSmi, FPR15C);
	fpr15c &= ~FPR15C_MASK_HWCENABLE;
	WRITE_FPR(pSmi, FPR15C, fpr15c);
    }


    LEAVE();
}

static void
SMILynx_CrtcLoadCursorImage_crt (xf86CrtcPtr crtc, CARD8 *image)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    CARD8 tmp;
    int i;
    CARD8* dst;

    ENTER();

    /* Load storage location. */
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x80,
		  pSmi->FBCursorOffset / 2048);
    tmp = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x81) & 0x80;
    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x81,
		  tmp | ((pSmi->FBCursorOffset / 2048) >> 8));

    /* Program FPR copy when on the 730 */
    if (pSmi->Chipset == SMI_COUGAR3DR) {
	CARD32 fpr15c;

	/* put address in upper word, and disable the cursor */
	fpr15c  = READ_FPR(pSmi, FPR15C) & FPR15C_MASK_HWCCOLORS;
	fpr15c |= (pSmi->FBCursorOffset / 2048) << 16;
	WRITE_FPR(pSmi, FPR15C, fpr15c);
    }

    /* Copy cursor image to framebuffer storage */
    dst = pSmi->FBBase + pSmi->FBCursorOffset;
    for(i=0; i < (SMILYNX_MAX_CURSOR * SMILYNX_MAX_CURSOR >> 2); i++){
	*(dst++) = image[i];
	if((i & 0x3) == 0x3) dst+=4;
    }

    LEAVE();
}

static void
SMILynx_CrtcDPMS_crt(xf86CrtcPtr crtc, int mode)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    SMIRegPtr reg = pSmi->mode;
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    ENTER();

    if(mode == DPMSModeOff)
	reg->SR21 |= 0x88; /* Disable DAC and color palette RAM */
    else
	reg->SR21 &= ~0x88; /* Enable DAC and color palette RAM */

    /* Wait for vertical retrace */
    while (hwp->readST01(hwp) & 0x8) ;
    while (!(hwp->readST01(hwp) & 0x8)) ;

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x21, reg->SR21);

    if(mode == DPMSModeOn){
	/* Reload the LUT */
	SMILynx_CrtcLoadLUT_crt(crtc);
    }

    LEAVE();
}

static void
SMILynx_CrtcDPMS_lcd(xf86CrtcPtr crtc, int mode)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    SMIRegPtr reg = pSmi->mode;
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    ENTER();

    if(mode == DPMSModeOff)
	reg->SR31 &= ~0x80; /* Disable Virtual Refresh */
    else
	reg->SR31 |= 0x80; /* Enable Virtual Refresh */

    /* Wait for vertical retrace */
    while (hwp->readST01(hwp) & 0x8) ;
    while (!(hwp->readST01(hwp) & 0x8)) ;

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x31, reg->SR31);

    LEAVE();
}

Bool
SMILynx_CrtcPreInit(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    xf86CrtcPtr crtc;
    xf86CrtcFuncsPtr crtcFuncs;
    SMICrtcPrivatePtr crtcPriv;

    ENTER();

    if(pSmi->Chipset == SMI_COUGAR3DR){
	/* XXX - Looking at the datasheet, it seems trivial to add
	   dualhead support for this chip... Little more than
	   splitting the WRITE_FPR/WRITE_VPR calls in separate
	   functions. Has someone access to this hardware? */

	SMI_CrtcFuncsInit_base(&crtcFuncs, &crtcPriv);

	if(pSmi->useBIOS){
	    crtcFuncs->mode_set = SMILynx_CrtcModeSet_bios;
	}else{
	    crtcFuncs->dpms = SMILynx_CrtcDPMS_crt;
	    crtcFuncs->mode_set = SMILynx_CrtcModeSet_vga;
	}

	crtcFuncs->mode_fixup = SMILynx_CrtcModeFixup;
	crtcPriv->adjust_frame = SMILynx_CrtcAdjustFrame;
	crtcPriv->video_init = SMI730_CrtcVideoInit;
	crtcPriv->load_lut = SMILynx_CrtcLoadLUT_crt;

	if(pSmi->HwCursor){
	    crtcFuncs->set_cursor_colors = SMILynx_CrtcSetCursorColors_crt;
	    crtcFuncs->set_cursor_position = SMILynx_CrtcSetCursorPosition_crt;
	    crtcFuncs->show_cursor = SMILynx_CrtcShowCursor_crt;
	    crtcFuncs->hide_cursor = SMILynx_CrtcHideCursor_crt;
	    crtcFuncs->load_cursor_image = SMILynx_CrtcLoadCursorImage_crt;
	}

	if(! (crtc = xf86CrtcCreate(pScrn,crtcFuncs)))
	    LEAVE(FALSE);
	crtc->driver_private = crtcPriv;
    }else{
	/* CRTC0 can drive both outputs when virtual refresh is
	   disabled, and only the VGA output with virtual refresh
	   enabled. */
	SMI_CrtcFuncsInit_base(&crtcFuncs, &crtcPriv);

	if(pSmi->useBIOS){
	    crtcFuncs->mode_set = SMILynx_CrtcModeSet_bios;
	}else{
	    crtcFuncs->dpms = SMILynx_CrtcDPMS_crt;

	    if(pSmi->Dualhead){
		/* The standard VGA CRTC registers get locked in
		   virtual refresh mode. */
		crtcFuncs->mode_set = SMILynx_CrtcModeSet_crt;

	    }else{
		crtcFuncs->mode_set = SMILynx_CrtcModeSet_vga;
	    }
	}

	crtcFuncs->mode_fixup = SMILynx_CrtcModeFixup;
	crtcPriv->adjust_frame = SMILynx_CrtcAdjustFrame;
	crtcPriv->video_init = SMILynx_CrtcVideoInit_crt;
	crtcPriv->load_lut = SMILynx_CrtcLoadLUT_crt;

	if(pSmi->HwCursor){
	    crtcFuncs->set_cursor_colors = SMILynx_CrtcSetCursorColors_crt;
	    crtcFuncs->set_cursor_position = SMILynx_CrtcSetCursorPosition_crt;
	    crtcFuncs->show_cursor = SMILynx_CrtcShowCursor_crt;
	    crtcFuncs->hide_cursor = SMILynx_CrtcHideCursor_crt;
	    crtcFuncs->load_cursor_image = SMILynx_CrtcLoadCursorImage_crt;
	}

	if(! (crtc = xf86CrtcCreate(pScrn,crtcFuncs)))
	    LEAVE(FALSE);
	crtc->driver_private = crtcPriv;

	if(pSmi->Dualhead){
	    /* CRTC1 drives LCD when enabled. */
	    SMI_CrtcFuncsInit_base(&crtcFuncs, &crtcPriv);
	    crtcFuncs->mode_set = SMILynx_CrtcModeSet_lcd;
	    crtcFuncs->mode_fixup = SMILynx_CrtcModeFixup;
	    crtcFuncs->dpms = SMILynx_CrtcDPMS_lcd;
	    crtcPriv->adjust_frame = SMILynx_CrtcAdjustFrame;
	    crtcPriv->video_init = SMILynx_CrtcVideoInit_lcd;
	    crtcPriv->load_lut = SMILynx_CrtcLoadLUT_lcd;

	    if(! (crtc = xf86CrtcCreate(pScrn,crtcFuncs)))
		LEAVE(FALSE);
	    crtc->driver_private = crtcPriv;
	}
    }

    LEAVE(TRUE);
}

