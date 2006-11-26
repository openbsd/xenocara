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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via_driver.h"
#include "via_vgahw.h"
#include "via_id.h"

/*
 * Now that via_bios is no longer such a behemoth and the relevant
 * code is moved via_mode.c, this code should be moved to via_mode.c too
 * especially as output abstraction will trim via_mode.c down further
 */

/*
 *
 */
static void
ViaSetCLE266APrimaryFIFO(ScrnInfoPtr pScrn, Bool Enable)
{
    VIAPtr pVia = VIAPTR(pScrn);
    CARD32  dwGE230, dwGE298;
    
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaSetCLE266APrimaryFIFO: %d\n", Enable));
    
    dwGE298 = VIAGETREG(0x298);
    VIASETREG(0x298, dwGE298 | 0x20000000);
    
    dwGE230 = VIAGETREG(0x230);
    if (Enable)
	dwGE230 |= 0x00200000;
    else
	dwGE230 &= ~0x00200000;
    VIASETREG(0x230, dwGE230);
    
    dwGE298 = VIAGETREG(0x298);
    VIASETREG(0x298, dwGE298 & ~0x20000000);
}

/*
 * 
 */
typedef struct {
    CARD16   X;
    CARD16   Y;
    CARD16   Bpp;
    CARD8    bRamClock;
    CARD8    bTuningValue;
} ViaExpireNumberTable;

static ViaExpireNumberTable CLE266AExpireNumber[] = {
    {1280, 768,32,0x03,0x3}, {1280,1024,32,0x03,0x4}, {1280,1024,32,0x04,0x3},
    {1600,1200,16,0x03,0x4}, {1600,1200,32,0x04,0x4}, {1024, 768,32,0x03,0xA},
    {1400,1050,16,0x03,0x3}, {1400,1050,32,0x03,0x4}, {1400,1050,32,0x04,0x4},
    { 800, 600,32,0x03,0xA}, {   0,   0, 0,   0,  0}
};

static ViaExpireNumberTable CLE266CExpireNumber[] = {
    {1280, 768,32,0x03,0x3}, {1280,1024,32,0x03,0x4}, {1280,1024,32,0x04,0x4},
    {1600,1200,32,0x03,0x3}, {1600,1200,32,0x04,0x4}, {1024, 768,32,0x03,0xA},
    {1400,1050,32,0x03,0x4}, {1400,1050,32,0x04,0x4},
    { 800, 600,32,0x03,0xA}, {   0,   0, 0,   0,  0}
};

static ViaExpireNumberTable KM400ExpireNumber[]={
    {1280,1024,32,0x03,0x3}, {1280,1024,32,0x04,0x9}, {1280, 768,32,0x03,0x3},
    {1280, 768,32,0x04,0x9}, {1400,1050,32,0x03,0x3}, {1400,1050,32,0x04,0x9},
    {1600,1200,32,0x03,0x4}, {1600,1200,32,0x04,0xA}, {   0,   0, 0,   0,  0}
};

/*
 *
 */
static void 
ViaSetPrimaryExpireNumber(ScrnInfoPtr pScrn, DisplayModePtr mode, ViaExpireNumberTable *Expire)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaSetPrimaryExpireNumber\n"));

    for (; Expire->X; Expire++)
        if ((Expire->X == mode->CrtcHDisplay) &&
            (Expire->Y == mode->CrtcVDisplay) &&
            (Expire->Bpp == pScrn->bitsPerPixel) &&
            (Expire->bRamClock == pVia->MemClk)) {
	    ViaSeqMask(hwp, 0x22, Expire->bTuningValue, 0x1F);
	    return;
	}
}

/*
 *
 */
void 
ViaSetPrimaryFIFO(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaSetPrimaryFIFO\n"));

    /* standard values */
    ViaSeqMask(hwp, 0x17, 0x1F, 0xFF);

    if (mode->CrtcHDisplay >= 1600) {
	ViaSeqMask(hwp, 0x16, 0x0F, 0xBF);
	ViaSeqMask(hwp, 0x18, 0x4F, 0xFF);
    } else if (mode->CrtcHDisplay >= 1024) {
	ViaSeqMask(hwp, 0x16, 0x0C, 0xBF);
	ViaSeqMask(hwp, 0x18, 0x4C, 0xFF);
    } else {
	ViaSeqMask(hwp, 0x16, 0x08, 0xBF);
	ViaSeqMask(hwp, 0x18, 0x4E, 0xFF);
    }

    switch(pVia->Chipset) {
    case VIA_CLE266:
	if (CLE266_REV_IS_CX(pVia->ChipRev)) {
	    if (pVia->HasSecondary) {  /* SAMM or DuoView case */
		if (mode->HDisplay >= 1024) {
		    ViaSeqMask(hwp, 0x16, 0x1C, 0x3F); /* 28 */
		    hwp->writeSeq(hwp, 0x17, 0x3F); /* 63 */
		}
	    } else {   /* Single view or Simultaneous case */
		if (mode->HDisplay > 1024) {
		    ViaSeqMask(hwp, 0x16, 0x17, 0x3F); /* 23 */
		    hwp->writeSeq(hwp, 0x17, 0x2F); /* 47 */
		}
	    }
	    hwp->writeSeq(hwp, 0x18, 0x57); /* 23 */

	    /* originally when setting secondary */
	    ViaSetPrimaryExpireNumber(pScrn, mode, CLE266CExpireNumber);
	} else {
	    if ((mode->HDisplay > 1024) && pVia->HasSecondary) {
		ViaSetCLE266APrimaryFIFO(pScrn, TRUE);

		ViaSeqMask(hwp, 0x16, 0x17, 0x3F); /* 23 */
		hwp->writeSeq(hwp, 0x17, 0x2F); /* 47 */
		hwp->writeSeq(hwp, 0x18, 0x57); /* 23 */
	    }

	    /* originally when setting secondary */
	    ViaSetPrimaryExpireNumber(pScrn, mode, CLE266AExpireNumber);
	}
	break;
    case VIA_KM400:
        if (pVia->HasSecondary) {  /* SAMM or DuoView case */
            if ((mode->HDisplay >= 1600) &&
                (pVia->MemClk <= VIA_MEM_DDR200)) {
		ViaSeqMask(hwp, 0x16, 0x09, 0x3F); /* 9 */
		hwp->writeSeq(hwp, 0x17, 0x1C); /* 28 */
            } else {
		ViaSeqMask(hwp, 0x16, 0x1C, 0x3F); /* 28 */
		hwp->writeSeq(hwp, 0x17, 0x3F); /* 63 */
            }
        } else {
	    if ((mode->HDisplay > 1280))
		ViaSeqMask(hwp, 0x16, 0x1C, 0x3F); /* 28 */
            else if (mode->HDisplay > 1024)
		ViaSeqMask(hwp, 0x16, 0x17, 0x3F); /* 23 */
            else
		ViaSeqMask(hwp, 0x16, 0x10, 0x3F); /* 16 */
	    hwp->writeSeq(hwp, 0x17, 0x3F); /* 63 */
	}
	hwp->writeSeq(hwp, 0x18, 0x57); /* 23 */

	/* originally when setting secondary */
	ViaSetPrimaryExpireNumber(pScrn, mode, KM400ExpireNumber);
	break;
#ifdef HAVE_K8M800
    case VIA_K8M800:
	hwp->writeSeq(hwp, 0x17, 0xBF); /* 384/2 - 1 = 191 (orig via comment: 384/8) */
	ViaSeqMask(hwp, 0x16, 0x92, 0xBF); /* 328/4 = 82 = 0x52*/
	ViaSeqMask(hwp, 0x18, 0x8a, 0xBF); /* 74 */

	if ((mode->HDisplay >= 1400) && (pScrn->bitsPerPixel == 32))
	    ViaSeqMask(hwp, 0x22, 0x10, 0x1F); /* 64/4 = 16 */
	else
	    ViaSeqMask(hwp, 0x22, 0x00, 0x1F); /* 128/4 = overflow = 0 */
	break;
#endif /* HAVE_K8M800 */
#ifdef HAVE_PM800
    case VIA_PM800:
	hwp->writeSeq(hwp, 0x17, 0x5F); /* 95 */
	ViaSeqMask(hwp, 0x16, 0x20, 0xBF); /* 32 */
	ViaSeqMask(hwp, 0x18, 0x10, 0xBF); /* 16 */

	if ((mode->HDisplay >= 1400) && (pScrn->bitsPerPixel == 32))
	    ViaSeqMask(hwp, 0x22, 0x10, 0x1F); /* 64/4 = 16 */
	else
	    ViaSeqMask(hwp, 0x22, 0x1F, 0x1F); /* 31 */
	break;
#endif /* HAVE_PM800 */
    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "ViaSetPrimaryFIFO:"
		   " Chipset %d not implemented\n", pVia->Chipset);
	break;

    }
}

/*
 * I've thrown out the LCD requirement. Size > 1024 is not supported
 * by any currently known TV encoder anyway. -- Luc.
 *
 */
void 
ViaSetSecondaryFIFO(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaSetSecondaryFIFO\n"));

    switch (pVia->Chipset) {
    case VIA_CLE266:
        if (CLE266_REV_IS_CX(pVia->ChipRev)) {
            if (mode->HDisplay >= 1024) {
		ViaCrtcMask(hwp, 0x6A, 0x20, 0x20);
		hwp->writeCrtc(hwp, 0x68, 0xAB); /* depth: 10, threshold: 11 */
	    } else {
		ViaCrtcMask(hwp, 0x6A, 0x00, 0x20);
		hwp->writeCrtc(hwp, 0x68, 0x67); /* depth: 6, threshold: 7 */
	    }
        } else {
            if ((pScrn->bitsPerPixel >= 24) &&
		(((mode->VDisplay > 768) && (pVia->MemClk <= VIA_MEM_DDR200)) ||
                 ((mode->HDisplay > 1280) && (pVia->MemClk <= VIA_MEM_DDR266)))) {
		ViaCrtcMask(hwp, 0x6A, 0x20, 0x20);
		hwp->writeCrtc(hwp, 0x68, 0xAB); /* depth: 10, threshold: 11 */
	    } else {
		ViaCrtcMask(hwp, 0x6A, 0x00, 0x20);
		hwp->writeCrtc(hwp, 0x68, 0x67); /* depth: 6, threshold: 7 */
	    }
        }
	break;
    case VIA_KM400:
	if ((mode->HDisplay >= 1600) && (pVia->MemClk <= VIA_MEM_DDR200)) {
	    ViaCrtcMask(hwp, 0x6A, 0x20, 0x20);
	    hwp->writeCrtc(hwp, 0x68, 0xEB); /* depth: 14, threshold: 11 */
        } else if ((pScrn->bitsPerPixel == 32) &&
		   (((mode->HDisplay > 1024) && (pVia->MemClk <= VIA_MEM_DDR333)) ||
		    ((mode->HDisplay >= 1024) && (pVia->MemClk <= VIA_MEM_DDR200)))) {
	    ViaCrtcMask(hwp, 0x6A, 0x20, 0x20);
	    hwp->writeCrtc(hwp, 0x68, 0xCA); /* depth: 12, threshold: 10 */
        } else if ((pScrn->bitsPerPixel == 16) &&
		   (((mode->HDisplay > 1280) && (pVia->MemClk <= VIA_MEM_DDR333)) ||
		    ((mode->HDisplay >= 1280) && (pVia->MemClk <= VIA_MEM_DDR200)))) {
	    ViaCrtcMask(hwp, 0x6A, 0x20, 0x20);
	    hwp->writeCrtc(hwp, 0x68, 0xAB); /* depth: 10, threshold: 11 */
        } else {
	   ViaCrtcMask(hwp, 0x6A, 0x00, 0x20);
	   hwp->writeCrtc(hwp, 0x68, 0x67); /* depth: 6, threshold: 7 */
	}
	break;
#ifdef HAVE_K8M800
    case VIA_K8M800:
	/* depth: (384 /8 -1 -1) = 46 = 0x2E */
	ViaCrtcMask(hwp, 0x68, 0xE0, 0xF0);
	ViaCrtcMask(hwp, 0x94, 0x00, 0x80);
	ViaCrtcMask(hwp, 0x95, 0x80, 0x80);

	/* threshold: (328/4) = 82 = 0x52 */
	ViaCrtcMask(hwp, 0x68, 0x02, 0x0F);
	ViaCrtcMask(hwp, 0x95, 0x50, 0x70);
	
	/* preq: 74 = 0x4A */
	ViaCrtcMask(hwp, 0x92, 0x0A, 0x0F);
	ViaCrtcMask(hwp, 0x95, 0x04, 0x07);

	if ((mode->HDisplay >= 1400) && (pScrn->bitsPerPixel == 32))
	    ViaCrtcMask(hwp, 0x94, 0x10, 0x7F); /* 64/4 */
	else
	    ViaCrtcMask(hwp, 0x94, 0x20, 0x7F); /* 128/4 */
	break;
#endif /* HAVE_K8M800 */
#ifdef HAVE_PM800
    case VIA_PM800:
	/* depth: 12 - 1 = 0x0B */
	ViaCrtcMask(hwp, 0x68, 0xB0, 0xF0);
	ViaCrtcMask(hwp, 0x94, 0x00, 0x80);
	ViaCrtcMask(hwp, 0x95, 0x00, 0x80);

	/* threshold: 16 = 0x10 */
	ViaCrtcMask(hwp, 0x68, 0x00, 0x0F);
	ViaCrtcMask(hwp, 0x95, 0x10, 0x70);

	/* preq: 8 = 0x08 */
	ViaCrtcMask(hwp, 0x92, 0x08, 0x0F);
	ViaCrtcMask(hwp, 0x95, 0x00, 0x07);

	if ((mode->HDisplay >= 1400) && (pScrn->bitsPerPixel == 32))
	    ViaCrtcMask(hwp, 0x94, 0x10, 0x7F); /* 64/4 */
	else
	    ViaCrtcMask(hwp, 0x94, 0x20, 0x7F); /* 128/4 */
	break;
#endif /* HAVE_PM800 */
    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "ViaSetSecondaryFIFO:"
		   " Chipset %d not implemented\n", pVia->Chipset);
	break;
    }
}

/*
 * Wrap around ViaSetCLE266APrimaryFIFO
 */
void 
ViaDisablePrimaryFIFO(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaDisablePrimaryFIFO\n"));

    /* Cause of exit XWindow will dump back register value, others chipset no
     * need to set extended fifo value */
    if ((pVia->Chipset == VIA_CLE266) && CLE266_REV_IS_AX(pVia->ChipRev) &&
        ((pScrn->currentMode->HDisplay > 1024) || pVia->HasSecondary))
        ViaSetCLE266APrimaryFIFO(pScrn, FALSE);
}
