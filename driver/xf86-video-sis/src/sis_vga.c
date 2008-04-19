/*
 * Mode setup and basic video bridge detection
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria.
 *
 * The SISInit() function for old series (except TV and FIFO calculation)
 * was previously based on code which was Copyright (C) 1998,1999 by Alan
 * Hourihane, Wigan, England. However, the code has been rewritten entirely
 * and is - it its current representation - not covered by this old copyright.
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

#include "sis.h"
#define SIS_NEED_inSISREG
#define SIS_NEED_outSISREG
#define SIS_NEED_inSISIDXREG
#define SIS_NEED_outSISIDXREG
#define SIS_NEED_orSISIDXREG
#define SIS_NEED_andSISIDXREG
#include "sis_regs.h"
#include "sis_dac.h"

static Bool  SISInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
static Bool  SIS300Init(ScrnInfoPtr pScrn, DisplayModePtr mode);
static int   SIS6326DoSense(ScrnInfoPtr pScrn, int tempbh, int tempbl, int tempch, int tempcl);
static void  SISSense6326(ScrnInfoPtr pScrn);
static void  SiS6326TVDelay(ScrnInfoPtr pScrn, int delay);
extern void  SISSense30x(ScrnInfoPtr pScrn, Bool quiet);
extern void  SISSenseChrontel(ScrnInfoPtr pScrn, Bool quiet);

/* Our very own vgaHW functions */
void SiSVGASave(ScrnInfoPtr pScrn, SISRegPtr save, int flags);
void SiSVGARestore(ScrnInfoPtr pScrn, SISRegPtr restore, int flags);
void SiSVGASaveFonts(ScrnInfoPtr pScrn);
void SiSVGARestoreFonts(ScrnInfoPtr pScrn);
void SISVGALock(SISPtr pSiS);
void SiSVGAUnlock(SISPtr pSiS);
void SiSVGAProtect(ScrnInfoPtr pScrn, Bool on);
#ifdef SIS_PC_PLATFORM
Bool SiSVGAMapMem(ScrnInfoPtr pScrn);
void SiSVGAUnmapMem(ScrnInfoPtr pScrn);
#endif
Bool SiSVGASaveScreen(ScreenPtr pScreen, int mode);
static Bool SiSVGAInit(ScrnInfoPtr pScrn, DisplayModePtr mode, int fixsync);

const CARD8 SiS6326TVRegs1[14] = {
     0x00,0x01,0x02,0x03,0x04,0x11,0x12,0x13,0x21,0x26,0x27,0x3a,0x3c,0x43
};

const CARD8 SiS6326TVRegs1_NTSC[6][14] = {
    {0x81,0x3f,0x49,0x1b,0xa9,0x03,0x00,0x09,0x08,0x7d,0x00,0x88,0x30,0x60},
    {0x81,0x3f,0x49,0x1d,0xa0,0x03,0x00,0x09,0x08,0x7d,0x00,0x88,0x30,0x60},
    {0x81,0x45,0x24,0x8e,0x26,0x0b,0x00,0x09,0x02,0xfe,0x00,0x09,0x51,0x60},
    {0x81,0x45,0x24,0x8e,0x26,0x07,0x00,0x29,0x04,0x30,0x10,0x3b,0x61,0x60},
    {0x81,0x3f,0x24,0x8e,0x26,0x09,0x00,0x09,0x02,0x30,0x10,0x3b,0x51,0x60},  /* 640x400, 640x480 */
    {0x83,0x5d,0x21,0xbe,0x75,0x03,0x00,0x09,0x08,0x42,0x10,0x4d,0x61,0x79}   /* 640x480u */
};

const CARD8 SiS6326TVRegs2_NTSC[6][54] = {
    {0x11, 0x17, 0x03, 0x09, 0x94, 0x02, 0x05, 0x06, 0x09, 0x50, 0x0C,
     0x0C, 0x06, 0x0D, 0x04, 0x0A, 0x94, 0x06, 0x0D, 0x04, 0x0A, 0x94,
     0xFC, 0xDF, 0x94, 0x1F, 0x4A, 0x03, 0x71, 0x07, 0x97, 0x10, 0x40,
     0x48, 0x00, 0x26, 0xB6, 0x10, 0x5C, 0xEC, 0x21, 0x2E, 0xBE, 0x10,
     0x64, 0xF4, 0x21, 0x13, 0x75, 0x08, 0x31, 0x6A, 0x01, 0xA0},
    {0x11, 0x17, 0x03, 0x0A, 0x94, 0x02, 0x05, 0x06, 0x09, 0x50, 0x0C,
     0x0D, 0x06, 0x0D, 0x04, 0x0A, 0x94, 0x06, 0x0D, 0x04, 0x0A, 0x94,
     0xFF, 0xDF, 0x94, 0x1F, 0x4A, 0x03, 0x71, 0x07, 0x97, 0x10, 0x40,
     0x48, 0x00, 0x26, 0xB6, 0x10, 0x5C, 0xEC, 0x21, 0x2E, 0xBE, 0x10,
     0x64, 0xF4, 0x21, 0x13, 0x75, 0x08, 0x31, 0x6A, 0x01, 0xA0},
    {0x11, 0x17, 0x03, 0x0A, 0x94, 0x02, 0x05, 0x06, 0x09, 0x50, 0x0C,
     0x0D, 0x06, 0x0D, 0x04, 0x0A, 0x94, 0x06, 0x0D, 0x04, 0x0A, 0x94,
     0xFF, 0xDF, 0x94, 0x3F, 0x8C, 0x06, 0xCE, 0x07, 0x27, 0x30, 0x73,
     0x7B, 0x00, 0x48, 0x68, 0x30, 0xB2, 0xD2, 0x52, 0x50, 0x70, 0x30,
     0xBA, 0xDA, 0x52, 0xDC, 0x02, 0xD1, 0x53, 0xF7, 0x02, 0xA0},
    {0x11, 0x17, 0x03, 0x09, 0x94, 0x02, 0x05, 0x06, 0x09, 0x50, 0x0C,
     0x0C, 0x06, 0x0D, 0x04, 0x0A, 0x94, 0x06, 0x0D, 0x04, 0x0A, 0x94,
     0xDC, 0xDF, 0x94, 0x3F, 0x8C, 0x06, 0xCE, 0x07, 0x27, 0x30, 0x73,
     0x7B, 0x00, 0x48, 0x68, 0x30, 0xB2, 0xD2, 0x52, 0x50, 0x70, 0x30,
     0xBA, 0xDA, 0x52, 0x00, 0x02, 0xF5, 0x53, 0xF7, 0x02, 0xA0},
    {0x11, 0x17, 0x03, 0x09, 0x94, 0x02, 0x05, 0x06, 0x09, 0x50, 0x0C,
     0x0C, 0x06, 0x0D, 0x04, 0x0A, 0x94, 0x06, 0x0D, 0x04, 0x0A, 0x94,
     0xDC, 0xDF, 0x94, 0x3F, 0x8C, 0x06, 0xCE, 0x07, 0x27, 0x30, 0x73,
     0x7B, 0x00, 0x48, 0x68, 0x30, 0xB2, 0xD2, 0x52, 0x50, 0x70, 0x30,
     0xBA, 0xDA, 0x52, 0xDC, 0x02, 0xD1, 0x53, 0xF7, 0x02, 0xA0},
    {0x11, 0x17, 0x03, 0x09, 0x94, 0x02, 0x05, 0x06, 0x09, 0x50, 0x0C,  /* 640x480u */
     0x0C, 0x06, 0x0D, 0x04, 0x0A, 0x94, 0x06, 0x0D, 0x04, 0x0A, 0x94,
     0xDC, 0xDF, 0x94, 0xAF, 0x95, 0x06, 0xDD, 0x07, 0x5F, 0x30, 0x7E,
     0x86, 0x00, 0x4C, 0xA4, 0x30, 0xE3, 0x3B, 0x62, 0x54, 0xAC, 0x30,
     0xEB, 0x43, 0x62, 0x48, 0x34, 0x3D, 0x63, 0x29, 0x03, 0xA0}
};

const CARD8 SiS6326TVRegs1_PAL[6][14] = {
    {0x81,0x2d,0xc8,0x07,0xb2,0x0b,0x00,0x09,0x02,0xed,0x00,0xf8,0x30,0x40},
    {0x80,0x2d,0xa4,0x03,0xd9,0x0b,0x00,0x09,0x02,0xed,0x10,0xf8,0x71,0x40},
    {0x81,0x2d,0xa4,0x03,0xd9,0x0b,0x00,0x09,0x02,0xed,0x10,0xf8,0x71,0x40},  /* 640x480 */
    {0x81,0x2d,0xa4,0x03,0xd9,0x0b,0x00,0x09,0x02,0x8f,0x10,0x9a,0x71,0x40},  /* 800x600 */
    {0x83,0x63,0xa1,0x7a,0xa3,0x0a,0x00,0x09,0x02,0xb5,0x11,0xc0,0x81,0x59},  /* 800x600u */
    {0x81,0x63,0xa4,0x03,0xd9,0x01,0x00,0x09,0x10,0x9f,0x10,0xaa,0x71,0x59}   /* 720x540  */
};

const CARD8 SiS6326TVRegs2_PAL[6][54] = {
    {0x15, 0x4E, 0x35, 0x6E, 0x94, 0x02, 0x04, 0x38, 0x3A, 0x50, 0x3D,
     0x70, 0x06, 0x3E, 0x35, 0x6D, 0x94, 0x05, 0x3F, 0x36, 0x6E, 0x94,
     0xE5, 0xDF, 0x94, 0xEF, 0x5A, 0x03, 0x7F, 0x07, 0xFF, 0x10, 0x4E,
     0x56, 0x00, 0x2B, 0x23, 0x20, 0xB4, 0xAC, 0x31, 0x33, 0x2B, 0x20,
     0xBC, 0xB4, 0x31, 0x83, 0xE1, 0x78, 0x31, 0xD6, 0x01, 0xA0},
    {0x15, 0x4E, 0x35, 0x6E, 0x94, 0x02, 0x04, 0x38, 0x3A, 0x50, 0x3D,
     0x70, 0x06, 0x3E, 0x35, 0x6D, 0x94, 0x05, 0x3F, 0x36, 0x6E, 0x94,
     0xE5, 0xDF, 0x94, 0xDF, 0xB2, 0x07, 0xFB, 0x07, 0xF7, 0x30, 0x90,
     0x98, 0x00, 0x4F, 0x3F, 0x40, 0x62, 0x52, 0x73, 0x57, 0x47, 0x40,
     0x6A, 0x5A, 0x73, 0x03, 0xC1, 0xF8, 0x63, 0xB6, 0x03, 0xA0},
    {0x15, 0x4E, 0x35, 0x6E, 0x94, 0x02, 0x04, 0x38, 0x3A, 0x50, 0x3D,  /* 640x480 */
     0x70, 0x06, 0x3E, 0x35, 0x6D, 0x94, 0x05, 0x3F, 0x36, 0x6E, 0x94,
     0xE5, 0xDF, 0x94, 0xDF, 0xB2, 0x07, 0xFB, 0x07, 0xF7, 0x30, 0x90,
     0x98, 0x00, 0x4F, 0x3F, 0x40, 0x62, 0x52, 0x73, 0x57, 0x47, 0x40,
     0x6A, 0x5A, 0x73, 0x03, 0xC1, 0xF8, 0x63, 0xB6, 0x03, 0xA0},
    {0x15, 0x4E, 0x35, 0x6E, 0x94, 0x02, 0x04, 0x38, 0x3A, 0x50, 0x3D,  /* 800x600 */
     0x70, 0x06, 0x3E, 0x35, 0x6D, 0x94, 0x05, 0x3F, 0x36, 0x6E, 0x94,
     0xE5, 0xDF, 0x94, 0xDF, 0xB2, 0x07, 0xFB, 0x07, 0xF7, 0x30, 0x90,
     0x98, 0x00, 0x4F, 0x3F, 0x40, 0x62, 0x52, 0x73, 0x57, 0x47, 0x40,
     0x6A, 0x5A, 0x73, 0xA0, 0xC1, 0x95, 0x73, 0xB6, 0x03, 0xA0},
    {0x15, 0x4E, 0x35, 0x6E, 0x94, 0x02, 0x04, 0x38, 0x3A, 0x50, 0x3D,  /* 800x600u */
     0x70, 0x06, 0x3E, 0x35, 0x6D, 0x94, 0x05, 0x3F, 0x36, 0x6E, 0x94,
     0xE5, 0xDF, 0x94, 0x7F, 0xBD, 0x08, 0x0E, 0x07, 0x47, 0x40, 0x9D,
     0xA5, 0x00, 0x54, 0x94, 0x40, 0xA4, 0xE4, 0x73, 0x5C, 0x9C, 0x40,
     0xAC, 0xEC, 0x73, 0x0B, 0x0E, 0x00, 0x84, 0x03, 0x04, 0xA0},
    {0x15, 0x4E, 0x35, 0x6E, 0x94, 0x02, 0x04, 0x38, 0x3A, 0x50, 0x3D,  /* 720x540  */
     0x70, 0x06, 0x3E, 0x35, 0x6D, 0x94, 0x05, 0x3F, 0x36, 0x6E, 0x94,
     0xE5, 0xDF, 0x94, 0xDF, 0xB0, 0x07, 0xFB, 0x07, 0xF7, 0x30, 0x9D,
     0xA5, 0x00, 0x4F, 0x3F, 0x40, 0x62, 0x52, 0x73, 0x57, 0x47, 0x40,
     0x6A, 0x5A, 0x73, 0xA0, 0xC1, 0x95, 0x73, 0xB6, 0x03, 0xA0}
};

const CARD8 SiS6326CR[9][15] = {
     {0x79,0x63,0x64,0x1d,0x6a,0x93,0x00,0x6f,0xf0,0x58,0x8a,0x57,0x57,0x70,0x20},  /* PAL 800x600   */
     {0x79,0x4f,0x50,0x95,0x60,0x93,0x00,0x6f,0xba,0x14,0x86,0xdf,0xe0,0x30,0x00},  /* PAL 640x480   */
     {0x5f,0x4f,0x50,0x82,0x53,0x9f,0x00,0x0b,0x3e,0xe9,0x8b,0xdf,0xe7,0x04,0x00},  /* NTSC 640x480  */
     {0x5f,0x4f,0x50,0x82,0x53,0x9f,0x00,0x0b,0x3e,0xcb,0x8d,0x8f,0x96,0xe9,0x00},  /* NTSC 640x400  */
     {0x83,0x63,0x64,0x1f,0x6d,0x9b,0x00,0x6f,0xf0,0x48,0x0a,0x23,0x57,0x70,0x20},  /* PAL 800x600u  */
     {0x79,0x59,0x5b,0x1d,0x66,0x93,0x00,0x6f,0xf0,0x42,0x04,0x1b,0x40,0x70,0x20},  /* PAL 720x540   */
     {0x66,0x4f,0x51,0x0a,0x57,0x89,0x00,0x0b,0x3e,0xd9,0x0b,0xb6,0xe7,0x04,0x00},  /* NTSC 640x480u */
     {0xce,0x9f,0x9f,0x92,0xa4,0x16,0x00,0x28,0x5a,0x00,0x04,0xff,0xff,0x29,0x39},  /* 1280x1024-75  */
     {0x09,0xc7,0xc7,0x0d,0xd2,0x0a,0x01,0xe0,0x10,0xb0,0x04,0xaf,0xaf,0xe1,0x1f}   /* 1600x1200-60  */
};

/* Initialize a display mode on 5597/5598, 6326 and 530/620 */
static Bool
SISInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    SISPtr       pSiS = SISPTR(pScrn);
    SISRegPtr    pReg = &pSiS->ModeReg;
    UChar        temp;
    int          mclk = pSiS->MemClock;
    int          clock = mode->Clock;
    int          width = mode->HDisplay;
    int          height = mode->VDisplay;
    int          rate = (int)SiSCalcVRate(mode);
    int          buswidth = pSiS->BusWidth;
    unsigned int vclk[5];
    UShort       CRT_CPUthresholdLow, CRT_CPUthresholdHigh, CRT_ENGthreshold;
    double       a, b, c;
    int          d, factor, offset, fixsync = 1;
    int          num, denum, div, sbit, scale;
    Bool	 sis6326tvmode, sis6326himode;

    /* Save the registers for further processing */
    (*pSiS->SiSSave)(pScrn, pReg);

    /* Initialise the standard VGA registers */
    if(!pSiS->UseVESA) {
       if(!SiSVGAInit(pScrn, mode, fixsync)) {
          xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "SISInit: SiSVGAInit() failed\n");
          return FALSE;
       }
    }

    /* Determine if chosen mode is suitable for TV on the 6326
     * and if the mode is one of our special hi-res modes.
     */
    sis6326tvmode = FALSE;
    sis6326himode = FALSE;
    if(pSiS->Chipset == PCI_CHIP_SIS6326) {
       if(pSiS->SiS6326Flags & SIS6326_HASTV) {
	  if((pSiS->SiS6326Flags & SIS6326_TVDETECTED) &&
	     ((strcmp(mode->name, "PAL800x600") == 0)   ||	/* Special TV modes */
	      (strcmp(mode->name, "PAL800x600U") == 0)  ||
	      (strcmp(mode->name, "PAL720x540") == 0)   ||
	      (strcmp(mode->name, "PAL640x480") == 0)   ||
	      (strcmp(mode->name, "NTSC640x480") == 0)  ||
	      (strcmp(mode->name, "NTSC640x480U") == 0) ||
	      (strcmp(mode->name, "NTSC640x400") == 0))) {
	     sis6326tvmode = TRUE;
	  } else {
	     pReg->sis6326tv[0x00] &= 0xfb;
	  }
       }
       if((strcmp(mode->name, "SIS1280x1024-75") == 0) ||	/* Special high-res modes */
	  (strcmp(mode->name, "SIS1600x1200-60") == 0)) {
	  sis6326himode = TRUE;
       }
    }

#ifdef UNLOCK_ALWAYS
    outSISIDXREG(SISSR, 0x05, 0x86);
#endif

    if(!pSiS->UseVESA) {
       pReg->sisRegs3C4[0x06] &= 0x01;
    }

    /* set interlace */
    if(!(mode->Flags & V_INTERLACE)) {
       offset = pSiS->CurrentLayout.displayWidth >> 3;
    } else {
       offset = pSiS->CurrentLayout.displayWidth >> 2;
       if(!pSiS->UseVESA) {
	  pReg->sisRegs3C4[0x06] |= 0x20;
       }
    }

    /* Enable Linear and Enhanced Gfx Mode */
    if(!pSiS->UseVESA) {
       pReg->sisRegs3C4[0x06] |= 0x82;
    }

    /* Enable MMIO at PCI Register 14H (D[6:5]: 11) */
    if(pSiS->oldChipset >= OC_SIS5597) {
       pReg->sisRegs3C4[0x0B] |= 0x60;
    } else {
       pReg->sisRegs3C4[0x0B] |= 0x20;
       pReg->sisRegs3C4[0x0B] &= ~0x40;
    }

    if(!pSiS->UseVESA) {

       /* Enable 32bit mem access (D7), read-ahead cache (D5) */
       pReg->sisRegs3C4[0x0C] |= 0x80;
       if(pSiS->oldChipset > OC_SIS6225) {
	  pReg->sisRegs3C4[0x0C] |= 0x20;
       }

       /* Some speed-up stuff */
       switch(pSiS->Chipset) {
       case PCI_CHIP_SIS5597:
	  /* enable host bus */
	  if(!pSiS->HostBus) {
	     pReg->sisRegs3C4[0x34] &= ~0x08;
	  } else {
	    pReg->sisRegs3C4[0x34] |= 0x08;
	  }
	  /* fall through */
       case PCI_CHIP_SIS6326:
       case PCI_CHIP_SIS530:
	  /* Enable "dual segment register mode" (D2) and "i/o gating while
	   * write buffer is not empty" (D3)
	   */
	  pReg->sisRegs3C4[0x0B] |= 0x0C;
       }

       /* set colordepth */
       if(pSiS->Chipset == PCI_CHIP_SIS530) {
	  pReg->sisRegs3C4[0x09] &= 0x7F;
       }
       switch(pSiS->CurrentLayout.bitsPerPixel) {
	  case 8:
	     break;
	  case 16:
	     offset <<= 1;
	     if(pSiS->CurrentLayout.depth == 15)
		pReg->sisRegs3C4[0x06] |= 0x04;
	     else
		pReg->sisRegs3C4[0x06] |= 0x08;
	     break;
	  case 24:
	     offset += (offset << 1);
	     pReg->sisRegs3C4[0x06] |= 0x10;
	     pReg->sisRegs3C4[0x0B] |= 0x90;
	     break;
	  case 32:
	     if(pSiS->Chipset == PCI_CHIP_SIS530) {
		offset <<= 2;
		if(pSiS->oldChipset != OC_SIS620) {
		   pReg->sisRegs3C4[0x06] |= 0x10;
		}
		pReg->sisRegs3C4[0x0B] |= 0x90;
		pReg->sisRegs3C4[0x09] |= 0x80;
	     } else return FALSE;
	     break;
       }
    }

    /* save screen pitch for acceleration functions */
    pSiS->scrnOffset = pSiS->CurrentLayout.displayWidth *
			((pSiS->CurrentLayout.bitsPerPixel + 7) / 8);

    /* Set accelerator dest color depth to 0 - not supported on 530/620 */
    pSiS->DstColor = 0;

    if(!pSiS->UseVESA) {

       /* set linear framebuffer addresses */
       switch(pScrn->videoRam) {
	  case 512:  temp = 0x00;  break;
	  case 1024: temp = 0x20;  break;
	  case 2048: temp = 0x40;  break;
	  case 4096: temp = 0x60;  break;
	  case 8192: temp = 0x80;  break;
	  default:   temp = 0x20;
       }
       pReg->sisRegs3C4[0x20] = (pSiS->FbAddress & 0x07F80000) >> 19;
       pReg->sisRegs3C4[0x21] = ((pSiS->FbAddress & 0xF8000000) >> 27) | temp;

       /* Set screen offset */
       pReg->sisRegs3D4[0x13] = offset & 0xFF;

       /* Set CR registers for our built-in TV and hi-res modes */
       if((sis6326tvmode) || (sis6326himode)) {

	  int index,i;

	  /* We need our very private data for hi-res and TV modes */
	  if(sis6326himode) {
	     if(strcmp(mode->name, "SIS1280x1024-75") == 0)  index = 7;
	     else index = 8;
	  } else {
	     if(pSiS->SiS6326Flags & SIS6326_TVPAL) {
		switch(width) {
		case 800:
		   if((strcmp(mode->name, "PAL800x600U") == 0))
		      index = 4;
		   else
		      index = 0;
		   break;
		case 720:
		   index = 5;
		   break;
		case 640:
		default:
		   index = 1;
		}
	     } else {
		switch(height) {
		case 400:
		   index = 3;
		   break;
		case 480:
		default:
		   if((strcmp(mode->name, "NTSC640x480U") == 0))
		      index = 6;
		   else
		      index = 2;
		}
	     }
	  }
	  for(i=0; i<=5; i++) {
	     pReg->sisRegs3D4[i] = SiS6326CR[index][i];
	  }
	  pReg->sisRegs3C4[0x12] = SiS6326CR[index][6];
	  pReg->sisRegs3D4[6] = SiS6326CR[index][7];
	  pReg->sisRegs3D4[7] = SiS6326CR[index][8];
	  pReg->sisRegs3D4[0x10] = SiS6326CR[index][9];
	  pReg->sisRegs3D4[0x11] = SiS6326CR[index][10];
	  pReg->sisRegs3D4[0x12] = SiS6326CR[index][11];
	  pReg->sisRegs3D4[0x15] = SiS6326CR[index][12];
	  pReg->sisRegs3D4[0x16] = SiS6326CR[index][13];
	  pReg->sisRegs3D4[9] &= ~0x20;
	  pReg->sisRegs3D4[9] |= (SiS6326CR[index][14] & 0x20);
	  pReg->sisRegs3C4[0x0A] = ((offset & 0xF00) >> 4) | (SiS6326CR[index][14] & 0x0f);

       } else {

	  /* Set extended vertical overflow register */
	  pReg->sisRegs3C4[0x0A] = (
	      ((offset                                & 0xF00) >>  4) |
	      (((mode->CrtcVTotal - 2)                & 0x400) >> 10) |
	      (((mode->CrtcVDisplay - 1)              & 0x400) >>  9) |
	      (((mode->CrtcVBlankStart - 1)           & 0x400) >>  8) |
	      (((mode->CrtcVSyncStart - fixsync)      & 0x400) >>  7));

	  /* Set extended horizontal overflow register */
	  pReg->sisRegs3C4[0x12] &= 0xE0;
	  pReg->sisRegs3C4[0x12] |= (
	      ((((mode->CrtcHTotal >> 3) - 5)          & 0x100) >> 8) |
	      ((((mode->CrtcHDisplay >> 3) - 1)        & 0x100) >> 7) |
	      ((((mode->CrtcHBlankStart >> 3) - 1)     & 0x100) >> 6) |
	      ((((mode->CrtcHSyncStart >> 3) - fixsync)& 0x100) >> 5) |
	      ((((mode->CrtcHBlankEnd >> 3) - 1)       & 0x40)  >> 2));
       }

       /* enable (or disable) line compare */
       if(mode->CrtcVDisplay >= 1024)
	  pReg->sisRegs3C4[0x38] |= 0x04;
       else
	  pReg->sisRegs3C4[0x38] &= 0xFB;

       /* Enable (or disable) high speed DCLK (some 6326 and 530/620 only) */
       if( ( (pSiS->Chipset == PCI_CHIP_SIS6326) &&
	     ( (pSiS->ChipRev == 0xd0) || (pSiS->ChipRev == 0xd1) ||
	       (pSiS->ChipRev == 0xd2) || (pSiS->ChipRev == 0x92) ||
	       (pSiS->Flags & A6326REVAB) ) ) ||
	   (pSiS->oldChipset > OC_SIS6326) ) {
	 if( (pSiS->CurrentLayout.bitsPerPixel == 24) ||
	     (pSiS->CurrentLayout.bitsPerPixel == 32) ||
	     (mode->CrtcHDisplay >= 1280) )
	    pReg->sisRegs3C4[0x3E] |= 0x01;
	 else
	    pReg->sisRegs3C4[0x3E] &= 0xFE;
       }

       /* We use the internal VCLK */
       pReg->sisRegs3C4[0x38] &= 0xFC;

       /* Programmable Clock */
       pReg->sisRegs3C2 = inSISREG(SISMISCR) | 0x0C;

#if 0
       if(pSiS->oldChipset <= OC_SIS86202) {
	  /* TODO: Handle SR07 for clock selection */
	  /* 86C201 does not even have a programmable clock... */
	  /* pReg->sisRegs3C4[0x07] &= 0x??; */
       }
#endif

       /* Set VCLK */
       if((sis6326tvmode) || (sis6326himode)) {

	  /* For our built-in modes, the calculation is not suitable */
	  if(sis6326himode) {
	     if((strcmp(mode->name, "SIS1280x1024-75") == 0)) {
		pReg->sisRegs3C4[0x2A] = 0x5d;	/* 1280x1024-75 */
		pReg->sisRegs3C4[0x2B] = 0xa4;
             } else {
		pReg->sisRegs3C4[0x2A] = 0x59;	/* 1600x1200-60 */
		pReg->sisRegs3C4[0x2B] = 0xa3;
	     }
	     pReg->sisRegs3C4[0x13] &= ~0x40;
	  } else {
             if(pSiS->SiS6326Flags & SIS6326_TVPAL) {
		/* PAL: 31.500 Mhz */
		if((strcmp(mode->name, "PAL800x600U") == 0)) {
		   pReg->sisRegs3C4[0x2A] = 0x46;
		   pReg->sisRegs3C4[0x2B] = 0x49;
		} else {
		   pReg->sisRegs3C4[0x2A] = 0xab;
		   pReg->sisRegs3C4[0x2B] = 0xe9;
		}
		pReg->sisRegs3C4[0x13] &= ~0x40;
	     } else {
		/* NTSC: 27.000 Mhz */
		if((strcmp(mode->name, "NTSC640x480U") == 0)) {
		   pReg->sisRegs3C4[0x2A] = 0x5a;
		   pReg->sisRegs3C4[0x2B] = 0x65;
		} else {
		   pReg->sisRegs3C4[0x2A] = 0x29;
		   pReg->sisRegs3C4[0x2B] = 0xe2;
		}
		pReg->sisRegs3C4[0x13] |= 0x40;
	     }
	  }

       } else if(SiS_compute_vclk(clock, &num, &denum, &div, &sbit, &scale)) {

	  pReg->sisRegs3C4[0x2A] = (num - 1) & 0x7f ;
	  pReg->sisRegs3C4[0x2A] |= (div == 2) ? 0x80 : 0;
	  pReg->sisRegs3C4[0x2B] = ((denum - 1) & 0x1f);
	  pReg->sisRegs3C4[0x2B] |= (((scale -1) & 3) << 5);

	  /* When setting VCLK, we should set SR13 first */
	  if(sbit)
	     pReg->sisRegs3C4[0x13] |= 0x40;
	  else
	     pReg->sisRegs3C4[0x13] &= 0xBF;

#ifdef TWDEBUG
	  xf86DrvMsg(0, X_INFO, "2a: %x 2b: %x 13: %x clock %d\n",
		pReg->sisRegs3C4[0x2A], pReg->sisRegs3C4[0x2B], pReg->sisRegs3C4[0x13], clock);
#endif

       } else {

	  /* if SiS_compute_vclk cannot handle the requested clock, try sisCalcClock */
	  SiSCalcClock(pScrn, clock, 2, vclk);

#define Midx    0
#define Nidx    1
#define VLDidx  2
#define Pidx    3
#define PSNidx  4

	  pReg->sisRegs3C4[0x2A] = (vclk[Midx] - 1) & 0x7f;
	  pReg->sisRegs3C4[0x2A] |= ((vclk[VLDidx] == 2) ? 1 : 0) << 7;

	  /* D[4:0]: denumerator */
	  pReg->sisRegs3C4[0x2B] = (vclk[Nidx] - 1) & 0x1f;

	  if(vclk[Pidx] <= 4){
	     /* postscale 1,2,3,4 */
	     pReg->sisRegs3C4[0x2B] |= (vclk[Pidx] - 1) << 5;
	     pReg->sisRegs3C4[0x13] &= 0xBF;
	  } else {
	     /* postscale 6,8 */
	     pReg->sisRegs3C4[0x2B] |= ((vclk[Pidx] / 2) - 1) << 5;
	     pReg->sisRegs3C4[0x13] |= 0x40;
	  }
	  pReg->sisRegs3C4[0x2B] |= 0x80 ;   /* gain for high frequency */

       }

       /* High speed DAC */
       if(clock > 135000)
	  pReg->sisRegs3C4[0x07] |= 0x02;

       if(pSiS->oldChipset > OC_SIS6225) {
	  /* 1 or 2 cycle DRAM (set by option FastVram) */
	  if(pSiS->newFastVram == -1) {
	     if(pSiS->oldChipset == OC_SIS620) {
		/* Use different default on the 620 */
		pReg->sisRegs3C4[0x34] |= 0x40;
		pReg->sisRegs3C4[0x34] &= ~0x80;
	     } else {
		pReg->sisRegs3C4[0x34] |= 0x80;
		pReg->sisRegs3C4[0x34] &= ~0x40;
	     }
	  } else if(pSiS->newFastVram == 1)
	     pReg->sisRegs3C4[0x34] |= 0xC0;
	  else
	     pReg->sisRegs3C4[0x34] &= ~0xC0;

	  if(pSiS->oldChipset == OC_SIS620) {
	     /* Enable SGRAM burst timing (= bit clear) on the 620 */
	     if(pSiS->Flags & SYNCDRAM) {
		pReg->sisRegs3C4[0x35] &= ~0x20;
	     } else {
		pReg->sisRegs3C4[0x35] |= 0x20;
	     }
	  }
       }

    } /* VESA */

    /* Logical line length */
    pSiS->ValidWidth = TRUE;
    pReg->sisRegs3C4[0x27] &= 0xCF;
    if(pSiS->CurrentLayout.bitsPerPixel == 24) {
       /* "Invalid logical width" */
       pReg->sisRegs3C4[0x27] |= 0x30;
       pSiS->ValidWidth = FALSE;
    } else {
       switch(pScrn->virtualX * (pSiS->CurrentLayout.bitsPerPixel >> 3)) {
	 case 1024:
		pReg->sisRegs3C4[0x27] |= 0x00;
		break;
	 case 2048:
		pReg->sisRegs3C4[0x27] |= 0x10;
		break;
	 case 4096:
		pReg->sisRegs3C4[0x27] |= 0x20;
		break;
	 default:
		/* Invalid logical width */
		pReg->sisRegs3C4[0x27] |= 0x30;
		pSiS->ValidWidth = FALSE;
		break;
       }
    }

    /* Acceleration stuff */
    if(!pSiS->NoAccel) {
       pReg->sisRegs3C4[0x27] |= 0x40;   /* Enable engine programming registers */
       if( (pSiS->TurboQueue) &&	 /* Handle TurboQueue */
	   (pSiS->oldChipset > OC_SIS6225) &&
	   ( (pSiS->Chipset != PCI_CHIP_SIS530) ||
	     (pSiS->CurrentLayout.bitsPerPixel != 24) ) ) {
	  pReg->sisRegs3C4[0x27] |= 0x80;        /* Enable TQ */
	  if((pSiS->Chipset == PCI_CHIP_SIS530) ||
	     ((pSiS->Chipset == PCI_CHIP_SIS6326 &&
	      (pSiS->ChipRev == 0xd0 || pSiS->ChipRev == 0xd1 ||
	       pSiS->ChipRev == 0xd2 || pSiS->ChipRev == 0x92 ||
	       pSiS->ChipRev == 0x0a || pSiS->ChipRev == 0x1a ||
	       pSiS->ChipRev == 0x2a || pSiS->ChipRev == 0x0b ||
	       pSiS->ChipRev == 0x1b || pSiS->ChipRev == 0x2b) ) ) ) {
	     /* pReg->sisRegs3C4[0x3D] |= 0x80;  */     /* Queue is 62K (530/620 specs) */
	     pReg->sisRegs3C4[0x3D] &= 0x7F;         /* Queue is 30K (530/620 specs) */
	  }
	  /* Locate the TQ at the beginning of the last 64K block of
	   * video RAM. The address is to be specified in 32K steps.
	   */
	  pReg->sisRegs3C4[0x2C] = (pScrn->videoRam - 64) / 32;
	  if(pSiS->Chipset != PCI_CHIP_SIS530) {	/* 530/620: Reserved (don't touch) */
	     pReg->sisRegs3C4[0x3C] &= 0xFC; 		/* 6326: Queue is all for 2D */
	  }						/* 5597: Must be 0           */
       } else {
	  pReg->sisRegs3C4[0x27] &= 0x7F;
       }
    }


    if(!pSiS->UseVESA) {

       /* No idea what this does. The Windows driver does it, so we do it as well */
       if(pSiS->Chipset == PCI_CHIP_SIS6326) {
	  if((pSiS->ChipRev == 0xd0) || (pSiS->ChipRev == 0xd1) ||
	     (pSiS->ChipRev == 0xd2) || (pSiS->ChipRev == 0x92) ||
	     (pSiS->Flags & A6326REVAB)) {
	     if((pSiS->Flags & (SYNCDRAM | RAMFLAG)) == (SYNCDRAM | RAMFLAG)) {
	        if(!(pReg->sisRegs3C4[0x0E] & 0x03)) {
	           pReg->sisRegs3C4[0x3E] |= 0x02;
	        }
	     }
	  }
       }

       /* Set memclock */
#if 0
       /* We don't need to do this; the SetMClk option was not used since 4.0. */
       if((pSiS->Chipset == PCI_CHIP_SIS5597) || (pSiS->Chipset == PCI_CHIP_SIS6326)) {
          if(pSiS->MemClock > 66000) {
             SiSCalcClock(pScrn, pSiS->MemClock, 1, vclk);

             pReg->sisRegs3C4[0x28] = (vclk[Midx] - 1) & 0x7f ;
             pReg->sisRegs3C4[0x28] |= ((vclk[VLDidx] == 2 ) ? 1 : 0 ) << 7 ;
             pReg->sisRegs3C4[0x29] = (vclk[Nidx] -1) & 0x1f ;   /* bits [4:0] contain denumerator -MC */
             if(vclk[Pidx] <= 4) {
                pReg->sisRegs3C4[0x29] |= (vclk[Pidx] - 1) << 5 ; /* postscale 1,2,3,4 */
                pReg->sisRegs3C4[0x13] &= 0x7F;
             } else {
                pReg->sisRegs3C4[0x29] |= ((vclk[Pidx] / 2) - 1) << 5 ;  /* postscale 6,8 */
                pReg->sisRegs3C4[0x13] |= 0x80;
             }
             /* Check programmed memory clock. Enable only to check the above code */
/*
             mclk = 14318 * ((pReg->sisRegs3C4[0x28] & 0x7f) + 1);
             mclk /= ((pReg->sisRegs3C4[0x29] & 0x0f) + 1);
             if(!(pReg->sisRegs3C4[0x13] & 0x80)) {
                mclk /= (((pReg->sisRegs3C4[0x29] & 0x60) >> 5) + 1);
             } else {
                if((pReg->sisRegs3C4[0x29] & 0x60) == 0x40) mclk /= 6;
                if((pReg->sisRegs3C4[0x29] & 0x60) == 0x60) mclk /= 8;
             }
             xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO,2,
                 "Setting memory clock to %.3f MHz\n",
                 mclk/1000.0);
*/
          }
       }
#endif

       /* Set threshold values */
       /*
        * CPU/CRT Threshold:                     FIFO
        *                           MCLK     ___________      VCLK
        * cpu/engine <---o       o--------->|___________| -----------> CRT
        *                ^       ^            ^       ^
        *                 \     /             |       |
        *                  \   /              |< gap >|
        *                   \ /               |       |
        *           selector switch   Thrsh. low     high
        *
        * CRT consumes the data in the FIFO during scanline display. When the
        * amount of data in the FIFO reaches the Threshold low value, the selector
        * switch will switch to the right, and the FIFO will be refilled with data.
        * When the amount of data in the FIFO reaches the Threshold high value, the
        * selector switch will switch to the left and allows the CPU and the chip
        * engines to access the video RAM.
        *
        * The Threshold low values should be increased at higher bpps, simply because
        * there is more data needed for the CRT. When Threshold low and high are very
        * close to each other, the selector switch will be activated more often, which
        * decreases performance.
        *
        */
       switch(pSiS->Chipset) {
       case PCI_CHIP_SIS5597:  factor = 65; break;
       case PCI_CHIP_SIS6326:  factor = 30; break;
       case PCI_CHIP_SIS530:   factor = (pSiS->Flags & UMA) ? 60 : 30; break;
       default:                factor = (pScrn->videoRam > 1024) ? 24 : 12;
       }
       a = width * height * rate * 1.40 * factor * ((pSiS->CurrentLayout.bitsPerPixel + 1) / 8);
       b = (mclk / 1000) * 999488.0 * (buswidth / 8);
       c = ((a / b) + 1.0) / 2;
       d = (int)c + 2;

       CRT_CPUthresholdLow = d;
       if((pSiS->Flags & (RAMFLAG | SYNCDRAM)) == (RAMFLAG | SYNCDRAM)) {
          CRT_CPUthresholdLow += 2;
       }
       CRT_CPUthresholdHigh = CRT_CPUthresholdLow + 3;

       CRT_ENGthreshold = 0x0F;

#if 0  /* See comment in sis_dac.c on why this is commented */
       if(pSiS->Chipset == PCI_CHIP_SIS530) {
	  if((pSiS->oldChipset == OC_SIS530A) &&
	     (pSiS->Flags & UMA) &&
	     (mclk == 100000) &&
	     (pSiS->Flags & ESS137xPRESENT)) {
	       if(!(pSiS->Flags & SECRETFLAG)) index = 0;
	       if((temp = SiS_CalcSpecial530Threshold(pSiS, mode, index)) {
		   CRT_CPUthresholdLow = temp;
		   break;
	       }
	  }
       }
#endif

       switch(pSiS->Chipset) {
       case PCI_CHIP_SIS530:
	  if(CRT_CPUthresholdLow > 0x1f)  CRT_CPUthresholdLow = 0x1f;
	  CRT_CPUthresholdHigh = 0x1f;
	  break;
       case PCI_CHIP_SIS5597:
       case PCI_CHIP_SIS6326:
       default:
	  if(CRT_CPUthresholdLow > 0x0f)  CRT_CPUthresholdLow  = 0x0f;
	  if(CRT_CPUthresholdHigh > 0x0f) CRT_CPUthresholdHigh = 0x0f;
       }

       pReg->sisRegs3C4[0x08] = ((CRT_CPUthresholdLow & 0x0F) << 4) |
				(CRT_ENGthreshold & 0x0F);

       pReg->sisRegs3C4[0x09] &= 0xF0;
       pReg->sisRegs3C4[0x09] |= (CRT_CPUthresholdHigh & 0x0F);

       pReg->sisRegs3C4[0x3F] &= 0xEB;
       pReg->sisRegs3C4[0x3F] |= ((CRT_CPUthresholdHigh & 0x10) |
				  ((CRT_CPUthresholdLow & 0x10) >> 2));

       if(pSiS->oldChipset >= OC_SIS530A) {
	  pReg->sisRegs3C4[0x3F] &= 0xDF;
	  pReg->sisRegs3C4[0x3F] |= 0x58;
       }

       /* Set SiS6326 TV registers */
       if((pSiS->Chipset == PCI_CHIP_SIS6326) && (sis6326tvmode)) {
	  UChar tmp;
	  int index=0, i, j, k;
	  int fsc;

	  if(pSiS->SiS6326Flags & SIS6326_TVPAL) {
	     pReg->sisRegs3C4[0x0D] |= 0x04;
	     switch(width) {
	     case 800:
	        if((strcmp(mode->name, "PAL800x600U") == 0))  index = 4;
	        else	        			      index = 3;
	        break;
	     case 720: index = 5;  break;
	     case 640:
	     default:  index = 2;
	     }
	     for(i=0; i<14; i++) {
	        pReg->sis6326tv[SiS6326TVRegs1[i]] = SiS6326TVRegs1_PAL[index][i];
	     }
	     fsc = (SiS6326TVRegs1_PAL[index][2] << 16) |
		   (SiS6326TVRegs1_PAL[index][3] << 8)  |
		   (SiS6326TVRegs1_PAL[index][4]);
	  } else {
	     pReg->sisRegs3C4[0x0D] &= ~0x04;
	     if((strcmp(mode->name, "NTSC640x480U") == 0))  index = 5;
	     else 					    index = 4;
	     for(i=0; i<14; i++) {
	        pReg->sis6326tv[SiS6326TVRegs1[i]] = SiS6326TVRegs1_NTSC[index][i];
	     }
	     fsc = (SiS6326TVRegs1_NTSC[index][2] << 16) |
	           (SiS6326TVRegs1_NTSC[index][3] << 8)  |
	           (SiS6326TVRegs1_NTSC[index][4]);
	  }
	  if(pSiS->sis6326fscadjust) {
	     fsc += pSiS->sis6326fscadjust;
	     pReg->sis6326tv[2] = (fsc >> 16) & 0xff;
	     pReg->sis6326tv[3] = (fsc >> 8) & 0xff;
	     pReg->sis6326tv[4] = fsc & 0xff;
	  }
	  tmp = pReg->sis6326tv[0x43];
	  if(pSiS->SiS6326Flags & SIS6326_TVCVBS) tmp |= 0x10;
	  tmp |= 0x08;
	  pReg->sis6326tv[0x43] = tmp;
	  j = 0; k = 0;
	  for(i=0; i<=0x44; i++) {
	     if(SiS6326TVRegs1[j] == i) {
		j++;
		continue;
	     }
	     if(pSiS->SiS6326Flags & SIS6326_TVPAL) {
		tmp = SiS6326TVRegs2_PAL[index][k];
	     } else {
		tmp = SiS6326TVRegs2_NTSC[index][k];
	     }
	     pReg->sis6326tv[i] = tmp;
	     k++;
	  }
	  pReg->sis6326tv[0x43] |= 0x08;
	  if((pSiS->ChipRev == 0xc1) || (pSiS->ChipRev == 0xc2)) {
	     pReg->sis6326tv[0x43] &= ~0x08;
	  }

	  tmp = pReg->sis6326tv[0];
	  tmp |= 0x18;
	  if(pSiS->SiS6326Flags & SIS6326_TVCVBS)   tmp &= ~0x10;
	  if(pSiS->SiS6326Flags & SIS6326_TVSVIDEO) tmp &= ~0x08;
	  tmp |= 0x04;
	  pReg->sis6326tv[0] = tmp;
       }

    } /* VESA */

    return TRUE;
}

/* Init a mode for SiS 300, 315, 330, 340 series
 * This function is now only used for setting up some
 * variables (eg. scrnOffset).
 */
Bool
SIS300Init(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    SISPtr    pSiS = SISPTR(pScrn);
    SISRegPtr pReg = &pSiS->ModeReg;
    UShort    temp;
    DisplayModePtr realmode = mode;

    PDEBUG(xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 4, "SIS300Init()\n"));

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 4,
	"virtualX = %d depth = %d Logical width = %d\n",
	pScrn->virtualX, pSiS->CurrentLayout.bitsPerPixel,
	pScrn->virtualX * pSiS->CurrentLayout.bitsPerPixel/8);

#ifdef SISMERGED
    if(pSiS->MergedFB) {
       realmode = ((SiSMergedDisplayModePtr)mode->Private)->CRT1;
    }
#endif

    /* Copy current register settings to structure */
    (*pSiS->SiSSave)(pScrn, pReg);

    /* Calculate Offset/Display Pitch */
    pSiS->scrnOffset = pSiS->CurrentLayout.displayWidth *
                          ((pSiS->CurrentLayout.bitsPerPixel + 7) / 8);

    pSiS->scrnPitch = pSiS->scrnPitch2 = pSiS->scrnOffset;
    if(!(pSiS->VBFlags & CRT1_LCDA)) {
       if(realmode->Flags & V_INTERLACE) pSiS->scrnPitch <<= 1;
    }
    /* CRT2 mode can never be interlaced */

#ifdef UNLOCK_ALWAYS
    outSISIDXREG(SISSR, 0x05, 0x86);
#endif

    switch(pSiS->CurrentLayout.bitsPerPixel) {
	case 8:
	    pSiS->DstColor = 0x0000;
	    pSiS->SiS310_AccelDepth = 0x00000000;
	    break;
	case 16:
	    if(pSiS->CurrentLayout.depth == 15)
	        pSiS->DstColor = (short) 0x4000;
	    else
	        pSiS->DstColor = (short) 0x8000;
	    pSiS->SiS310_AccelDepth = 0x00010000;
	    break;
	case 32:
	    pSiS->DstColor = (short) 0xC000;
	    pSiS->SiS310_AccelDepth = 0x00020000;
	    break;
    }

    /* Enable PCI LINEAR ADDRESSING (0x80), MMIO (0x01), PCI_IO (0x20) */
    pReg->sisRegs3C4[0x20] = 0xA1;

    /* Now initialize TurboQueue. TB is always located at the very top of
     * the videoRAM (notably NOT the x framebuffer memory, which can/should
     * be limited by MaxXFbMem when using DRI). Also, enable the accelerators.
     */
    if(!pSiS->NoAccel) {
	pReg->sisRegs3C4[0x1E] |= 0x42;  /* Enable 2D accelerator */
	pReg->sisRegs3C4[0x1E] |= 0x18;  /* Enable 3D accelerator */
	switch(pSiS->VGAEngine) {
	case SIS_300_VGA:
	  if(pSiS->TurboQueue) {    		/* set Turbo Queue as 512k */
	    temp = ((pScrn->videoRam/64)-8);    /* 8=512k, 4=256k, 2=128k, 1=64k */
	    pReg->sisRegs3C4[0x26] = temp & 0xFF;
	    pReg->sisRegs3C4[0x27] =
		(pReg->sisRegs3C4[0x27] & 0xfc) | (((temp >> 8) & 3) | 0xF0);
	  }	/* line above new for saving D2&3 of status register */
	  break;
	case SIS_315_VGA:
#ifndef SISVRAMQ
	  /* See comments in sis_driver.c */
	  pReg->sisRegs3C4[0x27] = 0x1F;
	  pReg->sisRegs3C4[0x26] = 0x22;
	  pReg->sisMMIO85C0 = (pScrn->videoRam - 512) * 1024;
#endif
	  break;
	}
    }

    return TRUE;
}

static void
SiS6326TVDelay(ScrnInfoPtr pScrn, int delay)
{
    SISPtr  pSiS = SISPTR(pScrn);
    int i;
    UChar temp;

    for(i=0; i<delay; i++) {
       inSISIDXREG(SISSR, 0x05, temp);
    }
    (void)temp;
}

static int
SIS6326DoSense(ScrnInfoPtr pScrn, int tempbh, int tempbl, int tempch, int tempcl)
{
    UChar temp;

    SiS6326SetTVReg(pScrn, 0x42, tempbl);
    temp = SiS6326GetTVReg(pScrn, 0x43);
    temp &= 0xfc;
    temp |= tempbh;
    SiS6326SetTVReg(pScrn, 0x43, temp);
    SiS6326TVDelay(pScrn, 0x1000);
    temp = SiS6326GetTVReg(pScrn, 0x43);
    temp |= 0x04;
    SiS6326SetTVReg(pScrn, 0x43, temp);
    SiS6326TVDelay(pScrn, 0x8000);
    temp = SiS6326GetTVReg(pScrn, 0x44);
    if(!(tempch & temp)) tempcl = 0;
    return tempcl;
}

static void
SISSense6326(ScrnInfoPtr pScrn)
{
    SISPtr pSiS = SISPTR(pScrn);
    UChar  temp;
    int    result;

    pSiS->SiS6326Flags &= (SIS6326_HASTV | SIS6326_TVPAL);
    temp = SiS6326GetTVReg(pScrn, 0x43);
    temp &= 0xfb;
    SiS6326SetTVReg(pScrn, 0x43, temp);
    result = SIS6326DoSense(pScrn, 0x01, 0xb0, 0x06, SIS6326_TVSVIDEO);  /* 0x02 */
    pSiS->SiS6326Flags |= result;
    result = SIS6326DoSense(pScrn, 0x01, 0xa0, 0x01, SIS6326_TVCVBS);    /* 0x04 */
    pSiS->SiS6326Flags |= result;
    temp = SiS6326GetTVReg(pScrn, 0x43);
    temp &= 0xfb;
    SiS6326SetTVReg(pScrn, 0x43, temp);
    if(pSiS->SiS6326Flags & (SIS6326_TVSVIDEO | SIS6326_TVCVBS)) {
       pSiS->SiS6326Flags |= SIS6326_TVDETECTED;
       xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	    "SiS6326: Detected TV connected to %s output\n",
		(((pSiS->SiS6326Flags & (SIS6326_TVSVIDEO | SIS6326_TVCVBS)) ==
		   (SIS6326_TVSVIDEO | SIS6326_TVCVBS)) ?
			"both SVIDEO and COMPOSITE" :
			   ((pSiS->SiS6326Flags & SIS6326_TVSVIDEO) ?
				"SVIDEO" : "COMPOSITE")));
    } else {
       xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	    "SiS6326: No TV detected\n");
    }
}

static Bool
SISIsUMC(SISPtr pSiS)
{
    UShort p4_0f, p4_25, p4_27, temp;

    inSISIDXREG(SISPART4, 0x0f, p4_0f);
    inSISIDXREG(SISPART4, 0x25, p4_25);
    inSISIDXREG(SISPART4, 0x27, p4_27);
    andSISIDXREG(SISPART4, 0x0f, 0x7f);
    orSISIDXREG(SISPART4, 0x25, 0x08);
    andSISIDXREG(SISPART4, 0x27, 0xfd);
    inSISIDXREG(SISPART4, 0x26, temp);
    outSISIDXREG(SISPART4, 0x27, p4_27);
    outSISIDXREG(SISPART4, 0x25, p4_25);
    outSISIDXREG(SISPART4, 0x0f, p4_0f);
    return((temp & 0x08) ? TRUE : FALSE);
}

/* Detect video bridge and set VBFlags accordingly */
void SISVGAPreInit(ScrnInfoPtr pScrn)
{
    SISPtr pSiS = SISPTR(pScrn);
    int    temp,temp1,temp2,sistypeidx;
    int    upperlimitlvds, lowerlimitlvds;
    int    upperlimitch, lowerlimitch;
    int    chronteltype, chrontelidreg, upperlimitvb;
    static const char *detectvb = "Detected SiS%s video bridge (%s, ID %d; Rev 0x%x)\n";
#if 0
    UChar sr17=0;
#endif
    static const char *ChrontelTypeStr[] = {
	"7004",
	"7005",
	"7007",
	"7006",
	"7008",
	"7013",
	"7019",
	"7020",
	"(unknown)"
    };
    static const char *SiSVBTypeStr[] = {
	"301",		/* 0 */
	"301B",		/* 1 */
	"301B-DH",	/* 2 */
	"301LV",	/* 3 */
	"302LV",	/* 4 */
	"301C",		/* 5 */
	"302ELV",	/* 6 */
	"302B"		/* 7 */
    };

    switch(pSiS->Chipset) {
       case PCI_CHIP_SIS300:
       case PCI_CHIP_SIS540:
       case PCI_CHIP_SIS630:
       case PCI_CHIP_SIS550:
       case PCI_CHIP_SIS315:
       case PCI_CHIP_SIS315H:
       case PCI_CHIP_SIS315PRO:
       case PCI_CHIP_SIS650:
       case PCI_CHIP_SIS330:
       case PCI_CHIP_SIS660:
       case PCI_CHIP_SIS340:
       case PCI_CHIP_XGIXG20:
       case PCI_CHIP_XGIXG40:
          pSiS->ModeInit = SIS300Init;
          break;
       default:
          pSiS->ModeInit = SISInit;
    }

    if((pSiS->Chipset == PCI_CHIP_SIS6326) && (pSiS->SiS6326Flags & SIS6326_HASTV)) {
       UChar sr0d;
       inSISIDXREG(SISSR, 0x0d, sr0d);
       if(sr0d & 0x04) {
	  pSiS->SiS6326Flags |= SIS6326_TVPAL;
       }
       SISSense6326(pScrn);
    }

    pSiS->VBFlags = pSiS->VBFlags2 = 0; /* reset VBFlags */
    pSiS->SiS_Pr->SiS_UseLCDA = FALSE;
    pSiS->SiS_Pr->Backup = FALSE;

    /* Videobridges only available for 300/315/330/340 series */
    if((pSiS->VGAEngine != SIS_300_VGA) && (pSiS->VGAEngine != SIS_315_VGA))
       return;

    /* No video bridge ever on XGI Z7 (XG20) */
    if(pSiS->ChipType == XGI_20)
       return;

    inSISIDXREG(SISPART4, 0x00, temp);
    temp &= 0x0F;
    if(temp == 1) {

        inSISIDXREG(SISPART4, 0x01, temp1);
	temp1 &= 0xff;

	if(temp1 >= 0xC0) {
	   if(SISIsUMC(pSiS)) pSiS->VBFlags2 |= VB2_SISUMC;
	}

        if(temp1 >= 0xE0) {
		inSISIDXREG(SISPART4, 0x39, temp2);
		if(temp2 == 0xff) {
		   pSiS->VBFlags2 |= VB2_302LV;
		   sistypeidx = 4;
		} else {
		   pSiS->VBFlags2 |= VB2_301C;	/* VB_302ELV; */
		   sistypeidx = 5; 		/* 6; */
		}
	} else if(temp1 >= 0xD0) {
		pSiS->VBFlags2 |= VB2_301LV;
		sistypeidx = 3;
	} else if(temp1 >= 0xC0) {
		pSiS->VBFlags2 |= VB2_301C;
		sistypeidx = 5;
	} else if(temp1 >= 0xB0) {
		pSiS->VBFlags2 |= VB2_301B;
		sistypeidx = 1;
		inSISIDXREG(SISPART4, 0x23, temp2);
		if(!(temp2 & 0x02)) {
		   pSiS->VBFlags2 |= VB2_30xBDH;
		   sistypeidx = 2;
		}
	} else {
		pSiS->VBFlags2 |= VB2_301;
		sistypeidx = 0;
	}

	xf86DrvMsg(pScrn->scrnIndex, X_PROBED, detectvb, SiSVBTypeStr[sistypeidx],
				(pSiS->VBFlags2 & VB2_SISUMC) ? "UMC-0" : "Charter/UMC-1", 1, temp1);

	SISSense30x(pScrn, FALSE);

    } else if(temp == 2) {

	inSISIDXREG(SISPART4, 0x01, temp1);
	temp1 &= 0xff;

	if(temp1 >= 0xC0) {
	   if(SISIsUMC(pSiS)) pSiS->VBFlags2 |= VB2_SISUMC;
	}

	if(temp1 >= 0xE0) {
		pSiS->VBFlags2 |= VB2_302LV;
		sistypeidx = 4;
	} else if(temp1 >= 0xD0) {
		pSiS->VBFlags2 |= VB2_301LV;
		sistypeidx = 3;
	} else {
		pSiS->VBFlags2 |= VB2_302B;
		sistypeidx = 7;
	}

	xf86DrvMsg(pScrn->scrnIndex, X_PROBED, detectvb, SiSVBTypeStr[sistypeidx],
				(pSiS->VBFlags2 & VB2_SISUMC) ? "UMC-0" : "Charter/UMC-1", 2, temp1);

	SISSense30x(pScrn, FALSE);

    } else if (temp == 3) {

	xf86DrvMsg(pScrn->scrnIndex, X_PROBED, detectvb, "303", "unsupported, unknown", temp, 0);

    } else {

	if(pSiS->NewCRLayout) {
	   inSISIDXREG(SISCR, 0x38, temp);
	   temp = (temp >> 5) & 0x07;
	} else {
	   inSISIDXREG(SISCR, 0x37, temp);
	   temp = (temp >> 1) & 0x07;
	}
	if(pSiS->VGAEngine == SIS_300_VGA) {
	   lowerlimitlvds = 2; upperlimitlvds = 4;
	   lowerlimitch   = 4; upperlimitch   = 5;
	   chronteltype = 1;   chrontelidreg  = 0x25;
	   upperlimitvb = upperlimitlvds;
	} else {
	   lowerlimitlvds = 2; upperlimitlvds = 3;
	   lowerlimitch   = 3; upperlimitch   = 3;
	   chronteltype = 2;   chrontelidreg  = 0x4b;
	   upperlimitvb = upperlimitlvds;
	   if(pSiS->NewCRLayout) {
	      upperlimitvb = 4;
	   }
	}

	if((temp >= lowerlimitlvds) && (temp <= upperlimitlvds)) {
	   pSiS->VBFlags2 |= VB2_LVDS;
	   xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	          "Detected LVDS transmitter (External chip ID %d)\n", temp);
	}
	if((temp >= lowerlimitch) && (temp <= upperlimitch))  {
	   /* Set global for init301.c */
	   pSiS->SiS_Pr->SiS_IF_DEF_CH70xx = chronteltype;

	   if(chronteltype == 1) {
	      /* Set general purpose IO for Chrontel communication */
	      SiS_SetChrontelGPIO(pSiS->SiS_Pr, 0x9c);
	   }

	   /* Read Chrontel version number */
	   temp1 = SiS_GetCH70xx(pSiS->SiS_Pr, chrontelidreg);
	   if(chronteltype == 1) {
	      /* See Chrontel TB31 for explanation */
	      temp2 = SiS_GetCH700x(pSiS->SiS_Pr, 0x0e);
	      if(((temp2 & 0x07) == 0x01) || (temp2 & 0x04)) {
		 SiS_SetCH700x(pSiS->SiS_Pr, 0x0e, 0x0b);
		 SiS_DDC2Delay(pSiS->SiS_Pr, 300);
	      }
	      temp2 = SiS_GetCH70xx(pSiS->SiS_Pr, chrontelidreg);
	      if(temp2 != temp1) temp1 = temp2;
	   }
	   if(temp1 == 0xFFFF) {	/* 0xFFFF = error reading DDC port */
	      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			"Detected Chrontel 70xx, but encountered error reading I2C port\n");
	      andSISIDXREG(SISCR, 0x32, ~0x07);
	      pSiS->postVBCR32 &= ~0x07;
	   } else if((temp1 >= 0x19) && (temp1 <= 200)) {
	      /* We only support device ids 0x19-200; other values may indicate DDC problems */
	      pSiS->VBFlags2 |= VB2_CHRONTEL;
	      switch (temp1) {
		case 0x32: temp2 = 0; pSiS->ChrontelType = CHRONTEL_700x; break;
		case 0x3A: temp2 = 1; pSiS->ChrontelType = CHRONTEL_700x; break;
		case 0x50: temp2 = 2; pSiS->ChrontelType = CHRONTEL_700x; break;
		case 0x2A: temp2 = 3; pSiS->ChrontelType = CHRONTEL_700x; break;
		case 0x40: temp2 = 4; pSiS->ChrontelType = CHRONTEL_700x; break;
		case 0x22: temp2 = 5; pSiS->ChrontelType = CHRONTEL_700x; break;
	        case 0x19: temp2 = 6; pSiS->ChrontelType = CHRONTEL_701x; break;
	        case 0x20: temp2 = 7; pSiS->ChrontelType = CHRONTEL_701x; break;  /* ID for 7020? */
		default:   temp2 = 8; pSiS->ChrontelType = CHRONTEL_701x; break;
	      }
	      xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			"Detected Chrontel %s TV encoder (ID 0x%02x; chip ID %d)\n",
					ChrontelTypeStr[temp2], temp1, temp);

	      /* Sense connected TV's */
	      SISSenseChrontel(pScrn, FALSE);

	   } else if(temp1 == 0) {
	      /* This indicates a communication problem, but it only occures if there
	       * is no TV attached. So we don't use TV in this case.
	       */
	      xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"Detected Chrontel TV encoder in promiscuous state (DDC/I2C mix-up)\n");
	      andSISIDXREG(SISCR, 0x32, ~0x07);
	      pSiS->postVBCR32 &= ~0x07;
	   } else {
	      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			"Chrontel: Unsupported device id (%d) detected\n",temp1);
	      andSISIDXREG(SISCR, 0x32, ~0x07);
	      pSiS->postVBCR32 &= ~0x07;
	   }
	   if(chronteltype == 1) {
	      /* Set general purpose IO for Chrontel communication */
	      SiS_SetChrontelGPIO(pSiS->SiS_Pr, 0x00);
	   }
	}
	if((pSiS->NewCRLayout) && (temp == 4)) {
	   pSiS->VBFlags2 |= VB2_CONEXANT;
	   xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	               "Detected Conexant video bridge - UNSUPPORTED\n");
	}
	if((pSiS->VGAEngine == SIS_300_VGA) && (temp == 3)) {
	    pSiS->VBFlags2 |= VB2_TRUMPION;
	    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	               "Detected Trumpion Zurac (I/II/III) LVDS scaler\n");
	}
	if(temp > upperlimitvb) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	               "Detected unknown bridge type (%d)\n", temp);
	}
    }

   /* Old BIOSes store the detected CRT2 type in SR17, 16 and 13
    * instead of CR32. However, since our detection routines
    * store their results to CR32, we now copy the
    * remaining bits (for LCD and VGA) to CR32 for unified usage.
    * SR17[0] CRT1     [1] LCD       [2] TV    [3] VGA2
    *     [4] AVIDEO   [5] SVIDEO
    * SR13[0] SCART    [1] HiVision
    * SR16[5] PAL/NTSC [6] LCD-SCALE [7] OVERSCAN
    */

#if 0
    inSISIDXREG(SISSR, 0x17, sr17);
    if( (pSiS->VGAEngine == SIS_300_VGA) &&
        (pSiS->Chipset != PCI_CHIP_SIS300) &&
        (sr17 & 0x0F) ) {

	UChar cr32;
	inSISIDXREG(SISCR, 0x32, cr32);

	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"Converting SR17 (%02x) to CR32 (%02x)\n", sr17, cr32);

	 if(sr17 & 0x01) {  	/* CRT1 */
		orSISIDXREG(SISCR, 0x32, 0x20);
		pSiS->postVBCR32 |= 0x20;
	 } else {
		andSISIDXREG(SISCR, 0x32, ~0x20);
		pSiS->postVBCR32 &= ~0x20;
	 }

	 if(sr17 & 0x02) {	/* LCD */
		orSISIDXREG(SISCR, 0x32, 0x08);
		pSiS->postVBCR32 |= 0x08;
	 } else	{
		andSISIDXREG(SISCR, 0x32, ~0x08);
		pSiS->postVBCR32 &= ~0x08;
	 }

	 /* No Hivision, no DVI here */
	 andSISIDXREG(SISCR,0x32,~0xc0);
	 pSiS->postVBCR32 &= ~0xc0;
    }
#endif

    /* Try to find out if the bridge uses LCDA for low resolution and
     * text modes. If sisfb saved this for us, use it. Otherwise,
     * check if we are running on a low mode on LCD and read the
     * relevant registers ourselves.
     */
    if(pSiS->VGAEngine == SIS_315_VGA) {

       if(pSiS->VBFlags2 & VB2_SISLCDABRIDGE) {
          if(pSiS->sisfblcda != 0xff) {
	     if((pSiS->sisfblcda & 0x03) == 0x03) {
		pSiS->SiS_Pr->SiS_UseLCDA = TRUE;
		pSiS->ChipFlags |= SiSCF_UseLCDA;
	     }
	  } else {
             inSISIDXREG(SISCR,0x34,temp);
	     if(temp <= 0x13) {
		inSISIDXREG(SISCR,0x38,temp);
		if((temp & 0x03) == 0x03) {
		   pSiS->SiS_Pr->SiS_UseLCDA = TRUE;
		   pSiS->ChipFlags |= SiSCF_UseLCDA;
		   pSiS->SiS_Pr->Backup = TRUE;
		} else {
		   orSISIDXREG(SISPART1,0x2f,0x01);  /* Unlock CRT2 */
		   inSISIDXREG(SISPART1,0x13,temp);
		   if(temp & 0x04) {
		      pSiS->SiS_Pr->SiS_UseLCDA = TRUE;
		      pSiS->ChipFlags |= SiSCF_UseLCDA;
		      pSiS->SiS_Pr->Backup = TRUE;
		   }
		}
	     }
	  }
	  if(pSiS->ChipFlags & SiSCF_UseLCDA) {
	     xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 3,
		"BIOS uses LCDA for low resolution and text modes\n");
	     if(pSiS->SiS_Pr->Backup == TRUE) {
		inSISIDXREG(SISCR,0x34,pSiS->SiS_Pr->Backup_Mode);
		inSISIDXREG(SISPART1,0x14,pSiS->SiS_Pr->Backup_14);
		inSISIDXREG(SISPART1,0x15,pSiS->SiS_Pr->Backup_15);
		inSISIDXREG(SISPART1,0x16,pSiS->SiS_Pr->Backup_16);
		inSISIDXREG(SISPART1,0x17,pSiS->SiS_Pr->Backup_17);
		inSISIDXREG(SISPART1,0x18,pSiS->SiS_Pr->Backup_18);
		inSISIDXREG(SISPART1,0x19,pSiS->SiS_Pr->Backup_19);
		inSISIDXREG(SISPART1,0x1a,pSiS->SiS_Pr->Backup_1a);
		inSISIDXREG(SISPART1,0x1b,pSiS->SiS_Pr->Backup_1b);
		inSISIDXREG(SISPART1,0x1c,pSiS->SiS_Pr->Backup_1c);
		inSISIDXREG(SISPART1,0x1d,pSiS->SiS_Pr->Backup_1d);
	     }
	  }
       }
    }
}

static void
SiS_WriteAttr(SISPtr pSiS, int index, int value)
{
    (void)inSISREG(SISINPSTAT);
    index |= 0x20;
    outSISREG(SISAR, index);
    outSISREG(SISAR, value);
}

static int
SiS_ReadAttr(SISPtr pSiS, int index)
{
    (void)inSISREG(SISINPSTAT);
    index |= 0x20;
    outSISREG(SISAR, index);
    return(inSISREG(SISARR));
}

static void
SiS_EnablePalette(SISPtr pSiS)
{
    (void)inSISREG(SISINPSTAT);
    outSISREG(SISAR, 0x00);
    pSiS->VGAPaletteEnabled = TRUE;
}

static void
SiS_DisablePalette(SISPtr pSiS)
{
    (void)inSISREG(SISINPSTAT);
    outSISREG(SISAR, 0x20);
    pSiS->VGAPaletteEnabled = FALSE;
}

void
SISVGALock(SISPtr pSiS)
{
    orSISIDXREG(SISCR, 0x11, 0x80);  	/* Protect CRTC[0-7] */
}

void
SiSVGAUnlock(SISPtr pSiS)
{
    andSISIDXREG(SISCR, 0x11, 0x7f);	/* Unprotect CRTC[0-7] */
}

#define SIS_FONTS_SIZE (8 * 8192)

void
SiSVGASaveFonts(ScrnInfoPtr pScrn)
{
#ifdef SIS_PC_PLATFORM
    SISPtr pSiS = SISPTR(pScrn);
    pointer vgaMemBase = pSiS->VGAMemBase;
    UChar miscOut, attr10, gr4, gr5, gr6, seq2, seq4, scrn;

    if((pSiS->fonts) || (vgaMemBase == NULL)) return;

    /* If in graphics mode, don't save anything */
    attr10 = SiS_ReadAttr(pSiS, 0x10);
    if(attr10 & 0x01) return;

    if(!(pSiS->fonts = xalloc(SIS_FONTS_SIZE * 2))) {
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"Could not save console fonts, mem allocation failed\n");
       return;
    }

    /* save the registers that are needed here */
    miscOut = inSISREG(SISMISCR);
    inSISIDXREG(SISGR, 0x04, gr4);
    inSISIDXREG(SISGR, 0x05, gr5);
    inSISIDXREG(SISGR, 0x06, gr6);
    inSISIDXREG(SISSR, 0x02, seq2);
    inSISIDXREG(SISSR, 0x04, seq4);

    /* Force into color mode */
    outSISREG(SISMISCW, miscOut | 0x01);

    inSISIDXREG(SISSR, 0x01, scrn);
    outSISIDXREG(SISSR, 0x00, 0x01);
    outSISIDXREG(SISSR, 0x01, scrn | 0x20);
    outSISIDXREG(SISSR, 0x00, 0x03);

    SiS_WriteAttr(pSiS, 0x10, 0x01);  /* graphics mode */

    /*font1 */
    outSISIDXREG(SISSR, 0x02, 0x04);  /* write to plane 2 */
    outSISIDXREG(SISSR, 0x04, 0x06);  /* enable plane graphics */
    outSISIDXREG(SISGR, 0x04, 0x02);  /* read plane 2 */
    outSISIDXREG(SISGR, 0x05, 0x00);  /* write mode 0, read mode 0 */
    outSISIDXREG(SISGR, 0x06, 0x05);  /* set graphics */
    slowbcopy_frombus(vgaMemBase, pSiS->fonts, SIS_FONTS_SIZE);

    /* font2 */
    outSISIDXREG(SISSR, 0x02, 0x08);  /* write to plane 3 */
    outSISIDXREG(SISSR, 0x04, 0x06);  /* enable plane graphics */
    outSISIDXREG(SISGR, 0x04, 0x03);  /* read plane 3 */
    outSISIDXREG(SISGR, 0x05, 0x00);  /* write mode 0, read mode 0 */
    outSISIDXREG(SISGR, 0x06, 0x05);  /* set graphics */
    slowbcopy_frombus(vgaMemBase, pSiS->fonts + SIS_FONTS_SIZE, SIS_FONTS_SIZE);

    inSISIDXREG(SISSR, 0x01, scrn);
    outSISIDXREG(SISSR, 0x00, 0x01);
    outSISIDXREG(SISSR, 0x01, scrn & ~0x20);
    outSISIDXREG(SISSR, 0x00, 0x03);

    /* Restore clobbered registers */
    SiS_WriteAttr(pSiS, 0x10, attr10);
    outSISIDXREG(SISSR, 0x02, seq2);
    outSISIDXREG(SISSR, 0x04, seq4);
    outSISIDXREG(SISGR, 0x04, gr4);
    outSISIDXREG(SISGR, 0x05, gr5);
    outSISIDXREG(SISGR, 0x06, gr6);
    outSISREG(SISMISCW, miscOut);
#endif
}

static void
SiSVGASaveMode(ScrnInfoPtr pScrn, SISRegPtr save)
{
    SISPtr pSiS = SISPTR(pScrn);
    int i;

    save->sisRegMiscOut = inSISREG(SISMISCR);

    for(i = 0; i < 25; i++) {
       inSISIDXREG(SISCR, i, save->sisRegs3D4[i]);
    }

    SiS_EnablePalette(pSiS);
    for(i = 0; i < 21; i++) {
       save->sisRegsATTR[i] = SiS_ReadAttr(pSiS, i);
    }
    SiS_DisablePalette(pSiS);

    for(i = 0; i < 9; i++) {
       inSISIDXREG(SISGR, i, save->sisRegsGR[i]);
    }

    for(i = 1; i < 5; i++) {
       inSISIDXREG(SISSR, i, save->sisRegs3C4[i]);
    }
}

static void
SiSVGASaveColormap(ScrnInfoPtr pScrn, SISRegPtr save)
{
    SISPtr pSiS = SISPTR(pScrn);
    int i;

    if(pSiS->VGACMapSaved) return;

    outSISREG(SISPEL, 0xff);

    outSISREG(SISCOLIDXR, 0x00);
    for(i = 0; i < 768; i++) {
       save->sisDAC[i] = inSISREG(SISCOLDATA);
       (void)inSISREG(SISINPSTAT);
       (void)inSISREG(SISINPSTAT);
    }

    SiS_DisablePalette(pSiS);
    pSiS->VGACMapSaved = TRUE;
}

void
SiSVGASave(ScrnInfoPtr pScrn, SISRegPtr save, int flags)
{
    if(save == NULL) return;

    if(flags & SISVGA_SR_CMAP)  SiSVGASaveColormap(pScrn, save);
    if(flags & SISVGA_SR_MODE)  SiSVGASaveMode(pScrn, save);
    if(flags & SISVGA_SR_FONTS) SiSVGASaveFonts(pScrn);
}

void
SiSVGARestoreFonts(ScrnInfoPtr pScrn)
{
#ifdef SIS_PC_PLATFORM
    SISPtr pSiS = SISPTR(pScrn);
    pointer vgaMemBase = pSiS->VGAMemBase;
    UChar miscOut, attr10, gr1, gr3, gr4, gr5, gr6, gr8, seq2, seq4, scrn;

    if((!pSiS->fonts) || (vgaMemBase == NULL)) return;

    /* save the registers that are needed here */
    miscOut = inSISREG(SISMISCR);
    attr10 = SiS_ReadAttr(pSiS, 0x10);
    inSISIDXREG(SISGR, 0x01, gr1);
    inSISIDXREG(SISGR, 0x03, gr3);
    inSISIDXREG(SISGR, 0x04, gr4);
    inSISIDXREG(SISGR, 0x05, gr5);
    inSISIDXREG(SISGR, 0x06, gr6);
    inSISIDXREG(SISGR, 0x08, gr8);
    inSISIDXREG(SISSR, 0x02, seq2);
    inSISIDXREG(SISSR, 0x04, seq4);

    /* Force into color mode */
    outSISREG(SISMISCW, miscOut | 0x01);
    inSISIDXREG(SISSR, 0x01, scrn);
    outSISIDXREG(SISSR, 0x00, 0x01);
    outSISIDXREG(SISSR, 0x01, scrn | 0x20);
    outSISIDXREG(SISSR, 0x00, 0x03);

    SiS_WriteAttr(pSiS, 0x10, 0x01);	  /* graphics mode */
    if(pScrn->depth == 4) {
       outSISIDXREG(SISGR, 0x03, 0x00);  /* don't rotate, write unmodified */
       outSISIDXREG(SISGR, 0x08, 0xFF);  /* write all bits in a byte */
       outSISIDXREG(SISGR, 0x01, 0x00);  /* all planes come from CPU */
    }

    outSISIDXREG(SISSR, 0x02, 0x04); /* write to plane 2 */
    outSISIDXREG(SISSR, 0x04, 0x06); /* enable plane graphics */
    outSISIDXREG(SISGR, 0x04, 0x02); /* read plane 2 */
    outSISIDXREG(SISGR, 0x05, 0x00); /* write mode 0, read mode 0 */
    outSISIDXREG(SISGR, 0x06, 0x05); /* set graphics */
    slowbcopy_tobus(pSiS->fonts, vgaMemBase, SIS_FONTS_SIZE);

    outSISIDXREG(SISSR, 0x02, 0x08); /* write to plane 3 */
    outSISIDXREG(SISSR, 0x04, 0x06); /* enable plane graphics */
    outSISIDXREG(SISGR, 0x04, 0x03); /* read plane 3 */
    outSISIDXREG(SISGR, 0x05, 0x00); /* write mode 0, read mode 0 */
    outSISIDXREG(SISGR, 0x06, 0x05); /* set graphics */
    slowbcopy_tobus(pSiS->fonts + SIS_FONTS_SIZE, vgaMemBase, SIS_FONTS_SIZE);

    inSISIDXREG(SISSR, 0x01, scrn);
    outSISIDXREG(SISSR, 0x00, 0x01);
    outSISIDXREG(SISSR, 0x01, scrn & ~0x20);
    outSISIDXREG(SISSR, 0x00, 0x03);

    /* restore the registers that were changed */
    outSISREG(SISMISCW, miscOut);
    SiS_WriteAttr(pSiS, 0x10, attr10);
    outSISIDXREG(SISGR, 0x01, gr1);
    outSISIDXREG(SISGR, 0x03, gr3);
    outSISIDXREG(SISGR, 0x04, gr4);
    outSISIDXREG(SISGR, 0x05, gr5);
    outSISIDXREG(SISGR, 0x06, gr6);
    outSISIDXREG(SISGR, 0x08, gr8);
    outSISIDXREG(SISSR, 0x02, seq2);
    outSISIDXREG(SISSR, 0x04, seq4);
#endif
}

static void
SiSVGARestoreMode(ScrnInfoPtr pScrn, SISRegPtr restore)
{
    SISPtr pSiS = SISPTR(pScrn);
    int i;

    outSISREG(SISMISCW, restore->sisRegMiscOut);

    for(i = 1; i < 5; i++) {
       outSISIDXREG(SISSR, i, restore->sisRegs3C4[i]);
    }

    outSISIDXREG(SISCR, 17, restore->sisRegs3D4[17] & ~0x80);

    for(i = 0; i < 25; i++) {
       outSISIDXREG(SISCR, i, restore->sisRegs3D4[i]);
    }

    for(i = 0; i < 9; i++) {
       outSISIDXREG(SISGR, i, restore->sisRegsGR[i]);
    }

    SiS_EnablePalette(pSiS);
    for(i = 0; i < 21; i++) {
       SiS_WriteAttr(pSiS, i, restore->sisRegsATTR[i]);
    }
    SiS_DisablePalette(pSiS);
}


static void
SiSVGARestoreColormap(ScrnInfoPtr pScrn, SISRegPtr restore)
{
    SISPtr pSiS = SISPTR(pScrn);
    int i;

    if(!pSiS->VGACMapSaved) return;

    outSISREG(SISPEL, 0xff);

    outSISREG(SISCOLIDX, 0x00);
    for(i = 0; i < 768; i++) {
       outSISREG(SISCOLDATA, restore->sisDAC[i]);
       (void)inSISREG(SISINPSTAT);
       (void)inSISREG(SISINPSTAT);
    }

    SiS_DisablePalette(pSiS);
}

void
SiSVGARestore(ScrnInfoPtr pScrn, SISRegPtr restore, int flags)
{
    if(restore == NULL) return;

    if(flags & SISVGA_SR_MODE)  SiSVGARestoreMode(pScrn, restore);
    if(flags & SISVGA_SR_FONTS) SiSVGARestoreFonts(pScrn);
    if(flags & SISVGA_SR_CMAP)  SiSVGARestoreColormap(pScrn, restore);
}

static void
SiS_SeqReset(SISPtr pSiS, Bool start)
{
    if(start) {
       outSISIDXREG(SISSR, 0x00, 0x01);	/* Synchronous Reset */
    } else {
       outSISIDXREG(SISSR, 0x00, 0x03);	/* End Reset */
    }
}

void
SiSVGAProtect(ScrnInfoPtr pScrn, Bool on)
{
    SISPtr pSiS = SISPTR(pScrn);
    UChar  tmp;

    if(!pScrn->vtSema) return;

    if(on) {
       inSISIDXREG(SISSR, 0x01, tmp);
       SiS_SeqReset(pSiS, TRUE);		/* start synchronous reset */
       outSISIDXREG(SISSR, 0x01, tmp | 0x20);	/* disable display */
       SiS_EnablePalette(pSiS);
    } else {
       andSISIDXREG(SISSR, 0x01, ~0x20);	/* enable display */
       SiS_SeqReset(pSiS, FALSE);		/* clear synchronous reset */
       SiS_DisablePalette(pSiS);
    }
}

#ifdef SIS_PC_PLATFORM
Bool
SiSVGAMapMem(ScrnInfoPtr pScrn)
{
    SISPtr pSiS = SISPTR(pScrn);

    /* Map only once */
    if(pSiS->VGAMemBase) return TRUE;

    if(pSiS->VGAMapSize == 0) pSiS->VGAMapSize = (64 * 1024);
    if(pSiS->VGAMapPhys == 0) pSiS->VGAMapPhys = 0xA0000;

#if XF86_VERSION_CURRENT >= XF86_VERSION_NUMERIC(4,3,0,0,0)
#if XSERVER_LIBPCIACCESS
    pSiS->VGAMemBase = xf86MapDomainMemory(pScrn->scrnIndex, VIDMEM_MMIO_32BIT,
			pSiS->PciInfo, pSiS->VGAMapPhys, pSiS->VGAMapSize);
#else
    pSiS->VGAMemBase = xf86MapDomainMemory(pScrn->scrnIndex, VIDMEM_MMIO_32BIT,
			pSiS->PciTag, pSiS->VGAMapPhys, pSiS->VGAMapSize);
#endif
#else
    pSiS->VGAMemBase = xf86MapVidMem(pScrn->scrnIndex, VIDMEM_MMIO_32BIT,
			pSiS->VGAMapPhys, pSiS->VGAMapSize);
#endif

    return(pSiS->VGAMemBase != NULL);
}

void
SiSVGAUnmapMem(ScrnInfoPtr pScrn)
{
    SISPtr pSiS = SISPTR(pScrn);

    if(pSiS->VGAMemBase == NULL) return;

    xf86UnMapVidMem(pScrn->scrnIndex, pSiS->VGAMemBase, pSiS->VGAMapSize);
    pSiS->VGAMemBase = NULL;
}
#endif

#if 0
static CARD32
SiS_HBlankKGA(DisplayModePtr mode, SISRegPtr regp, int nBits, unsigned int Flags)
{
    int    nExtBits = (nBits < 6) ? 0 : nBits - 6;
    CARD32 ExtBits;
    CARD32 ExtBitMask = ((1 << nExtBits) - 1) << 6;

    regp->sisRegs3D4[3] = (regp->sisRegs3D4[3] & ~0x1F) |
                          (((mode->CrtcHBlankEnd >> 3) - 1) & 0x1F);
    regp->sisRegs3D4[5] = (regp->sisRegs3D4[5] & ~0x80) |
                          ((((mode->CrtcHBlankEnd >> 3) - 1) & 0x20) << 2);
    ExtBits             = ((mode->CrtcHBlankEnd >> 3) - 1) & ExtBitMask;

    if( (Flags & SISKGA_FIX_OVERSCAN) &&
        ((mode->CrtcHBlankEnd >> 3) == (mode->CrtcHTotal >> 3))) {
       int i = (regp->sisRegs3D4[3] & 0x1F)        |
	       ((regp->sisRegs3D4[5] & 0x80) >> 2) |
	       ExtBits;
       if(Flags & SISKGA_ENABLE_ON_ZERO) {
	  if( (i-- > (((mode->CrtcHBlankStart >> 3) - 1) & (0x3F | ExtBitMask))) &&
	      (mode->CrtcHBlankEnd == mode->CrtcHTotal) ) {
	     i = 0;
	  }
       } else if (Flags & SISKGA_BE_TOT_DEC) i--;
       regp->sisRegs3D4[3] = (regp->sisRegs3D4[3] & ~0x1F) | (i & 0x1F);
       regp->sisRegs3D4[5] = (regp->sisRegs3D4[5] & ~0x80) | ((i << 2) & 0x80);
       ExtBits = i & ExtBitMask;
    }
    return ExtBits >> 6;
}
#endif

static CARD32
SiS_VBlankKGA(DisplayModePtr mode, SISRegPtr regp, int nBits, unsigned int Flags)
{
    CARD32 nExtBits   = (nBits < 8) ? 0 : (nBits - 8);
    CARD32 ExtBitMask = ((1 << nExtBits) - 1) << 8;
    CARD32 ExtBits    = (mode->CrtcVBlankEnd - 1) & ExtBitMask;
    CARD32 BitMask    = (nBits < 7) ? 0 : ((1 << nExtBits) - 1);
    int VBlankStart   = (mode->CrtcVBlankStart - 1) & 0xFF;
    regp->sisRegs3D4[22] = (mode->CrtcVBlankEnd - 1) & 0xFF;

    if((Flags & SISKGA_FIX_OVERSCAN) && (mode->CrtcVBlankEnd == mode->CrtcVTotal)) {
       int i = regp->sisRegs3D4[22] | ExtBits;
       if(Flags & SISKGA_ENABLE_ON_ZERO) {
	  if( ((BitMask && ((i & BitMask) > (VBlankStart & BitMask))) ||
	       ((i > VBlankStart)  &&  		            /* 8-bit case */
	        ((i & 0x7F) > (VBlankStart & 0x7F)))) &&    /* 7-bit case */
	      (!(regp->sisRegs3D4[9] & 0x9F)) ) {	    /* 1 scanline/row */
	     i = 0;
	  } else {
	     i--;
	  }
       } else if(Flags & SISKGA_BE_TOT_DEC) i--;

       regp->sisRegs3D4[22] = i & 0xFF;
       ExtBits = i & 0xFF00;
    }
    return (ExtBits >> 8);
}

Bool
SiSVGAInit(ScrnInfoPtr pScrn, DisplayModePtr mode, int fixsync)
{
    SISPtr pSiS = SISPTR(pScrn);
    SISRegPtr regp = &pSiS->ModeReg;
    int depth = pScrn->depth;
    unsigned int i;

    /* Sync */
    if((mode->Flags & (V_PHSYNC | V_NHSYNC)) && (mode->Flags & (V_PVSYNC | V_NVSYNC))) {
        regp->sisRegMiscOut = 0x23;
        if(mode->Flags & V_NHSYNC) regp->sisRegMiscOut |= 0x40;
        if(mode->Flags & V_NVSYNC) regp->sisRegMiscOut |= 0x80;
    } else {
        int VDisplay = mode->VDisplay;

        if(mode->Flags & V_DBLSCAN) VDisplay *= 2;
        if(mode->VScan > 1)         VDisplay *= mode->VScan;

        if(VDisplay < 400)        regp->sisRegMiscOut = 0xA3;	/* +hsync -vsync */
        else if (VDisplay < 480)  regp->sisRegMiscOut = 0x63;	/* -hsync +vsync */
        else if (VDisplay < 768)  regp->sisRegMiscOut = 0xE3;	/* -hsync -vsync */
        else                      regp->sisRegMiscOut = 0x23;	/* +hsync +vsync */
    }

    regp->sisRegMiscOut |= (mode->ClockIndex & 0x03) << 2;

    /* Seq */
    if(depth == 4) regp->sisRegs3C4[0] = 0x02;
    else	   regp->sisRegs3C4[0] = 0x00;

    if(mode->Flags & V_CLKDIV2) regp->sisRegs3C4[1] = 0x09;
    else                        regp->sisRegs3C4[1] = 0x01;

    regp->sisRegs3C4[2] = 0x0F;

    regp->sisRegs3C4[3] = 0x00;

    if(depth < 8)  regp->sisRegs3C4[4] = 0x06;
    else           regp->sisRegs3C4[4] = 0x0E;

    /* CRTC */
    regp->sisRegs3D4[0]  = (mode->CrtcHTotal >> 3) - 5;
    regp->sisRegs3D4[1]  = (mode->CrtcHDisplay >> 3) - 1;
    regp->sisRegs3D4[2]  = (mode->CrtcHBlankStart >> 3) - 1;
    regp->sisRegs3D4[3]  = (((mode->CrtcHBlankEnd >> 3) - 1) & 0x1F) | 0x80;
    i = (((mode->CrtcHSkew << 2) + 0x10) & ~0x1F);
    if(i < 0x80)  regp->sisRegs3D4[3] |= i;
    regp->sisRegs3D4[4]  = (mode->CrtcHSyncStart >> 3) - fixsync;
    regp->sisRegs3D4[5]  = ((((mode->CrtcHBlankEnd >> 3) - 1) & 0x20) << 2) |
			   (((mode->CrtcHSyncEnd >> 3) - fixsync) & 0x1F);
    regp->sisRegs3D4[6]  = (mode->CrtcVTotal - 2) & 0xFF;
    regp->sisRegs3D4[7]  = (((mode->CrtcVTotal - 2) & 0x100) >> 8)           |
			   (((mode->CrtcVDisplay - 1) & 0x100) >> 7)         |
			   (((mode->CrtcVSyncStart - fixsync) & 0x100) >> 6) |
			   (((mode->CrtcVBlankStart - 1) & 0x100) >> 5)      |
			   0x10                                              |
			   (((mode->CrtcVTotal - 2) & 0x200)   >> 4)         |
			   (((mode->CrtcVDisplay - 1) & 0x200) >> 3)         |
			   (((mode->CrtcVSyncStart - fixsync) & 0x200) >> 2);
    regp->sisRegs3D4[8]  = 0x00;
    regp->sisRegs3D4[9]  = (((mode->CrtcVBlankStart - 1) & 0x200) >> 4) | 0x40;
    if(mode->Flags & V_DBLSCAN) regp->sisRegs3D4[9] |= 0x80;
    if(mode->VScan >= 32)     regp->sisRegs3D4[9] |= 0x1F;
    else if (mode->VScan > 1) regp->sisRegs3D4[9] |= mode->VScan - 1;
    regp->sisRegs3D4[10] = 0x00;
    regp->sisRegs3D4[11] = 0x00;
    regp->sisRegs3D4[12] = 0x00;
    regp->sisRegs3D4[13] = 0x00;
    regp->sisRegs3D4[14] = 0x00;
    regp->sisRegs3D4[15] = 0x00;
    regp->sisRegs3D4[16] = (mode->CrtcVSyncStart - fixsync) & 0xFF;
    regp->sisRegs3D4[17] = ((mode->CrtcVSyncEnd - fixsync) & 0x0F) | 0x20;
    regp->sisRegs3D4[18] = (mode->CrtcVDisplay - 1) & 0xFF;
    regp->sisRegs3D4[19] = pScrn->displayWidth >> 4;
    regp->sisRegs3D4[20] = 0x00;
    regp->sisRegs3D4[21] = (mode->CrtcVBlankStart - 1) & 0xFF;
    regp->sisRegs3D4[22] = (mode->CrtcVBlankEnd - 1) & 0xFF;
    if(depth < 8) regp->sisRegs3D4[23] = 0xE3;
    else	  regp->sisRegs3D4[23] = 0xC3;
    regp->sisRegs3D4[24] = 0xFF;

#if 0
    SiS_HBlankKGA(mode, regp, 0, SISKGA_FIX_OVERSCAN | SISKGA_ENABLE_ON_ZERO);
#endif
    SiS_VBlankKGA(mode, regp, 0, SISKGA_FIX_OVERSCAN | SISKGA_ENABLE_ON_ZERO);

    /* GR */
    regp->sisRegsGR[0] = 0x00;
    regp->sisRegsGR[1] = 0x00;
    regp->sisRegsGR[2] = 0x00;
    regp->sisRegsGR[3] = 0x00;
    regp->sisRegsGR[4] = 0x00;
    if(depth == 4) regp->sisRegsGR[5] = 0x02;
    else           regp->sisRegsGR[5] = 0x40;
    regp->sisRegsGR[6] = 0x05;   /* only map 64k VGA memory !!!! */
    regp->sisRegsGR[7] = 0x0F;
    regp->sisRegsGR[8] = 0xFF;

    /* Attr */
    for(i = 0; i <= 15; i++) {	/* standard colormap translation */
       regp->sisRegsATTR[i] = i;
    }
    if(depth == 4) regp->sisRegsATTR[16] = 0x81;
    else           regp->sisRegsATTR[16] = 0x41;
    if(depth >= 4) regp->sisRegsATTR[17] = 0xFF;
    else           regp->sisRegsATTR[17] = 0x01;
    regp->sisRegsATTR[18] = 0x0F;
    regp->sisRegsATTR[19] = 0x00;
    regp->sisRegsATTR[20] = 0x00;

    return TRUE;
}

static void
SISVGABlankScreen(ScrnInfoPtr pScrn, Bool on)
{
    SISPtr pSiS = SISPTR(pScrn);
    UChar  tmp;

    inSISIDXREG(SISSR, 0x01, tmp);
    if(on) tmp &= ~0x20;
    else   tmp |= 0x20;
    SiS_SeqReset(pSiS, TRUE);
    outSISIDXREG(SISSR, 0x01, tmp);
    SiS_SeqReset(pSiS, FALSE);
}

Bool
SiSVGASaveScreen(ScreenPtr pScreen, int mode)
{
    ScrnInfoPtr pScrn = NULL;
    Bool on = xf86IsUnblank(mode);

    if(pScreen == NULL) return FALSE;

    pScrn = xf86Screens[pScreen->myNum];

    if(pScrn->vtSema) {
       SISVGABlankScreen(pScrn, on);
    }
    return TRUE;
}

#undef SIS_FONTS_SIZE


