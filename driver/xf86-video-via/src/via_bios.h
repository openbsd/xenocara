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

#ifndef _VIA_BIOS_H_
#define _VIA_BIOS_H_ 1

#define     VIA_PANEL6X4                    0
#define     VIA_PANEL8X6                    1
#define     VIA_PANEL10X7                   2
#define     VIA_PANEL12X7                   3
#define     VIA_PANEL12X10                  4
#define     VIA_PANEL14X10                  5
#define     VIA_PANEL16X12                  6
#define     VIA_PANEL_INVALID               255

#define     TVTYPE_NONE                     0x00
#define     TVTYPE_NTSC                     0x01
#define     TVTYPE_PAL                      0x02

#define     TVOUTPUT_NONE                   0x00
#define     TVOUTPUT_COMPOSITE              0x01
#define     TVOUTPUT_SVIDEO                 0x02
#define     TVOUTPUT_RGB                    0x04
#define     TVOUTPUT_YCBCR                  0x08
#define     TVOUTPUT_SC                     0x16

#define  VIA_NONETV   0
#define  VIA_VT1621   1 /* TV2PLUS */
#define  VIA_VT1622   2 /* TV3 */
#define  VIA_VT1623   3 /* also VT1622A */
#define  VIA_VT1625   4 

#define     VIA_TVNORMAL                    0
#define     VIA_TVOVER                      1

#define     VIA_DEVICE_NONE                 0x00
#define	    VIA_DEVICE_CRT		    0x01
#define	    VIA_DEVICE_LCD		    0x02
#define	    VIA_DEVICE_TV		    0x04
#define	    VIA_DEVICE_DFP		    0x08

/* System Memory CLK */
#define VIA_MEM_SDR66   0x00
#define VIA_MEM_SDR100  0x01
#define VIA_MEM_SDR133  0x02
#define VIA_MEM_DDR200  0x03
#define VIA_MEM_DDR266  0x04
#define VIA_MEM_DDR333  0x05
#define VIA_MEM_DDR400  0x06
#define VIA_MEM_END     0x07
#define VIA_MEM_NONE    0xFF

/* Digital Output Bus Width */
#define	    VIA_DI_12BIT		    0x00
#define	    VIA_DI_24BIT		    0x01

typedef struct _VIABIOSINFO {
    int         scrnIndex;

    Bool        CrtPresent;
    Bool        CrtActive;

    CARD16      ResolutionIndex;
    CARD32      Clock; /* register value for the dotclock */
    Bool        ClockExternal;
    CARD32      Bandwidth; /* available memory bandwidth */

    /* Panel/LCD entries */
    Bool        PanelPresent;
    Bool        PanelActive;
    Bool        ForcePanel;
    int         PanelIndex;
    int         PanelSize;
    Bool	LCDDualEdge;	/* mean add-on card is 2CH or Dual or DDR */
    Bool        Center;
    CARD8	BusWidth;		/* Digital Output Bus Width */
    Bool        SetDVI;
    /* LCD Simultaneous Expand Mode HWCursor Y Scale */
    Bool        scaleY;
    int         panelX;
    int         panelY;
    int         resY;

    /* TV entries */
    int         TVEncoder;
    int         TVOutput;
    Bool        TVActive;
    I2CDevPtr   TVI2CDev;
    int         TVType;
    Bool        TVDotCrawl;
    int         TVDeflicker;
    CARD8       TVRegs[0xFF];
    int         TVNumRegs;

    /* TV Callbacks */
    void (*TVSave) (ScrnInfoPtr pScrn);
    void (*TVRestore) (ScrnInfoPtr pScrn);
    Bool (*TVDACSense) (ScrnInfoPtr pScrn);
    ModeStatus (*TVModeValid) (ScrnInfoPtr pScrn, DisplayModePtr mode);
    void (*TVModeI2C) (ScrnInfoPtr pScrn, DisplayModePtr mode);
    void (*TVModeCrtc) (ScrnInfoPtr pScrn, DisplayModePtr mode);
    void (*TVPower) (ScrnInfoPtr pScrn, Bool On);
    DisplayModePtr TVModes;
    void (*TVPrintRegs) (ScrnInfoPtr pScrn);

} VIABIOSInfoRec, *VIABIOSInfoPtr;

/* Function prototypes */
/* via_vbe.c */
void ViaVbeAdjustFrame(int scrnIndex, int x, int y, int flags);
Bool ViaVbeSetMode(ScrnInfoPtr pScrn, DisplayModePtr pMode);
Bool ViaVbeSaveRestore(ScrnInfoPtr pScrn, vbeSaveRestoreFunction function);
Bool ViaVbeModePreInit(ScrnInfoPtr pScrn);
void ViaVbeDPMS(ScrnInfoPtr pScrn, int mode, int flags);
void ViaVbeDoDPMS(ScrnInfoPtr pScrn, int mode);

/* via_mode.c */
void ViaOutputsDetect(ScrnInfoPtr pScrn);
Bool ViaOutputsSelect(ScrnInfoPtr pScrn);
void ViaModesAttach(ScrnInfoPtr pScrn, MonPtr monitorp);
CARD32 ViaGetMemoryBandwidth(ScrnInfoPtr pScrn);
ModeStatus ViaValidMode(int scrnIndex, DisplayModePtr mode, Bool verbose, int flags);
void ViaModePrimary(ScrnInfoPtr pScrn, DisplayModePtr mode);
void ViaModeSecondary(ScrnInfoPtr pScrn, DisplayModePtr mode);
void ViaLCDPower(ScrnInfoPtr pScrn, Bool On);
void ViaTVPower(ScrnInfoPtr pScrn, Bool On);
void ViaTVSave(ScrnInfoPtr pScrn);
void ViaTVRestore(ScrnInfoPtr pScrn);
#ifdef HAVE_DEBUG
void ViaTVPrintRegs(ScrnInfoPtr pScrn);
#endif

/* in via_bandwidth.c */
void ViaSetPrimaryFIFO(ScrnInfoPtr pScrn, DisplayModePtr mode);
void ViaSetSecondaryFIFO(ScrnInfoPtr pScrn, DisplayModePtr mode);
void ViaDisablePrimaryFIFO(ScrnInfoPtr pScrn);

/* via_vt162x.c */
I2CDevPtr ViaVT162xDetect(ScrnInfoPtr pScrn, I2CBusPtr pBus, CARD8 Address);
void ViaVT162xInit(ScrnInfoPtr pScrn);

#endif /* _VIA_BIOS_H_ */
