/*
 * Copyright 2016 Kevin Brace
 * Copyright 2005-2016 The OpenChrome Project
 *                     [https://www.freedesktop.org/wiki/Openchrome]
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

#ifndef _VIA_UMS_H_
#define _VIA_UMS_H_ 1

#include "via_vgahw.h"

#define     VIA_PANEL6X4                    0
#define     VIA_PANEL8X6                    1
#define     VIA_PANEL10X7                   2
#define     VIA_PANEL12X7                   3
#define     VIA_PANEL12X10                  4
#define     VIA_PANEL14X10                  5
#define     VIA_PANEL16X12                  6
#define     VIA_PANEL12X8                   7
#define     VIA_PANEL8X4                    8
#define     VIA_PANEL1366X7                 9
#define     VIA_PANEL1360X7                 10
#define     VIA_PANEL1920x1080              11
#define     VIA_PANEL1920x1200              12
#define     VIA_PANEL10X6                   13
#define     VIA_PANEL14X9                   14
#define     VIA_PANEL1280X720               15
#define     VIA_PANEL12X9                   16
#define     VIA_PANEL_INVALID               255

#define     TVTYPE_NONE                     0x00
#define     TVTYPE_NTSC                     0x01
#define     TVTYPE_PAL                      0x02
#define     TVTYPE_480P                     0X03
#define     TVTYPE_576P                     0X04
#define     TVTYPE_720P                     0X05
#define     TVTYPE_1080I                    0X06

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
#define  VIA_CH7011   5
#define  VIA_CH7019A  6
#define  VIA_CH7019B  7
#define  VIA_CH7017   8
#define  VIA_CH7304   9
#define  VIA_CH7305   10


#define     VIA_TVNORMAL                    0
#define     VIA_TVOVER                      1

#define     VIA_DEVICE_NONE                 0x00
#define	    VIA_DEVICE_CRT		    0x01
#define	    VIA_DEVICE_LCD		    0x02
#define	    VIA_DEVICE_TV		    0x04
#define	    VIA_DEVICE_DFP		    0x08

#define     VIA_I2C_NONE                    0x00
#define     VIA_I2C_BUS1                    0x01
#define     VIA_I2C_BUS2                    0x02
#define     VIA_I2C_BUS3                    0x04

/* System Memory CLK */
#define VIA_MEM_SDR66   0x00
#define VIA_MEM_SDR100  0x01
#define VIA_MEM_SDR133  0x02
#define VIA_MEM_DDR200  0x03
#define VIA_MEM_DDR266  0x04
#define VIA_MEM_DDR333  0x05
#define VIA_MEM_DDR400  0x06
#define VIA_MEM_DDR533  0x07
#define VIA_MEM_DDR667  0x08
#define VIA_MEM_DDR800  0x09
#define VIA_MEM_DDR1066 0x0A
#define VIA_MEM_END     0x0B
#define VIA_MEM_NONE    0xFF

#define VIA_BW_MIN       74000000 /* > 640x480@60Hz@32bpp */
#define VIA_BW_DDR200   394000000
#define VIA_BW_DDR400   553000000 /* > 1920x1200@60Hz@32bpp */
#define VIA_BW_DDR667   922000000
#define VIA_BW_DDR1066  922000000

/* Digital Output Bus Width */
#define	    VIA_DI_12BIT		    0x00
#define	    VIA_DI_24BIT		    0x01

/* Digital Port */
#define     VIA_DI_PORT_NONE        0x0
#define     VIA_DI_PORT_DIP0        0x1
#define     VIA_DI_PORT_DIP1        0x2
#define     VIA_DI_PORT_DVP0        0x4
#define     VIA_DI_PORT_DVP1        0x8
#define     VIA_DI_PORT_FPDPLOW     0x10
#define     VIA_DI_PORT_FPDPHIGH    0x20
#define     VIA_DI_PORT_LVDS1       0x40
#define     VIA_DI_PORT_TMDS        0x40
#define     VIA_DI_PORT_LVDS2       0x80

/* External TMDS (DVI) Transmitter Type */
#define     VIA_TMDS_NONE           0x0
#define     VIA_TMDS_VT1632         0x1
#define     VIA_TMDS_SII164         0x2

/* To be used with via_analog_set_dpms_control inline function. */
#define VIA_ANALOG_DPMS_ON          0x00
#define VIA_ANALOG_DPMS_STANDBY     0x01
#define VIA_ANALOG_DPMS_SUSPEND     0x02
#define VIA_ANALOG_DPMS_OFF         0x03

#define VIA_DPA_CLK_30M       30000000
#define VIA_DPA_CLK_50M       50000000
#define VIA_DPA_CLK_70M       70000000
#define VIA_DPA_CLK_100M      100000000
#define VIA_DPA_CLK_150M      150000000


#define BIT(x) (1 << x)


enum {
    VIA_DPA_CLK_RANGE_30M,
    VIA_DPA_CLK_RANGE_30M_50M,
    VIA_DPA_CLK_RANGE_50M_70M,
    VIA_DPA_CLK_RANGE_70M_100M,
    VIA_DPA_CLK_RANGE_100M_150M,
    VIA_DPA_CLK_RANGE_150M,
};

typedef struct {
    CARD8   SR[256];
    CARD8   CR[256];
} VIARegRec, *VIARegPtr;

typedef struct ViaPanelMode {
    int Width;
    int Height;
    Bool useDualEdge;
    Bool useDithering;
} ViaPanelModeRec, *ViaPanelModePtr ;

typedef struct _VIADISPLAY {
    Bool        analogPresence;
    CARD8       analogI2CBus;

    Bool        intTMDSPresence;
    CARD8       intTMDSDIPort;
    CARD8       intTMDSI2CBus;

    Bool        extTMDSPresence;
    CARD8       extTMDSDIPort;
    CARD8       extTMDSI2CBus;
    CARD8       extTMDSTransmitter;

    Bool        intFP1Presence;
    CARD8       intFP1DIPort;
    CARD8       intFP1I2CBus;

    Bool        intFP2Presence;
    CARD8       intFP2DIPort;
    CARD8       intFP2I2CBus;

    /* Keeping track of the number of analog VGA connectors. */
    unsigned int        numberVGA;

    /* Keeping track of the number of DVI connectors. */
    unsigned int        numberDVI;

    /* Keeping track of the number of FP (Flat Panel) connectors. */
    unsigned int        numberFP;

    CARD8       I2CDevices;
    CARD8       mappedI2CBus;

    I2CBusPtr       pI2CBus1;
    I2CBusPtr       pI2CBus2;
    I2CBusPtr       pI2CBus3;

    /* VIA Technologies NanoBook reference design.
     * Examples include Everex CloudBook and Sylvania g netbook.
     * It is also called FIC CE260 and CE261 by its ODM (Original
     * Design Manufacturer) name. */
    Bool                isVIANanoBook;

    /* Quanta IL1 netbook has its FP connected to DVP1
     * rather than LVDS, hence, a special flag register
     * is needed for properly controlling its FP. */
    Bool                isQuantaIL1;

    /* Samsung NC20 netbook has its FP connected to LVDS2
     * rather than the more logical LVDS1, hence, a special
     * flag register is needed for properly controlling its
     * FP. */
    Bool                isSamsungNC20;

    /* OLPC XO-1.5 */
    Bool                isOLPCXO15;

    VIARegRec           SavedReg;

    xf86OutputPtr tv;

    CARD32      Clock; /* register value for the dotclock */
    Bool        ClockExternal;
    CARD32      Bandwidth; /* available memory bandwidth */

    /* TV entries */
    int         TVEncoder;
    int         TVOutput;
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
    void (*TVModeCrtc) (xf86CrtcPtr crtc, DisplayModePtr mode);
    void (*TVPower) (ScrnInfoPtr pScrn, Bool On);
    void (*LCDPower) (ScrnInfoPtr pScrn, Bool On);
    DisplayModePtr TVModes;
    int            TVNumModes;
    void (*TVPrintRegs) (ScrnInfoPtr pScrn);

} VIADisplayRec, *VIADisplayPtr;

typedef struct _VIAANALOG {
    CARD8       i2cBus;
} VIAAnalogRec, *VIAAnalogPtr;

/*
 * Record for storing FP (Flat Panel) specific information.
 */
typedef struct _VIAFP {
    Bool IsActive ;
    /* Native physical resolution */
    int NativeHeight;
    int NativeWidth;
    Bool useDualEdge;
    Bool useDithering;

    /* Native resolution index, see via_panel.c */
    CARD8 NativeModeIndex;
    /* Determine if we must use the hardware scaler
     * It might be false even if the "Center" option
     * was specified
     */
    Bool            Scale;

    /* Panel/LCD entries */
    CARD16      ResolutionIndex;
    int         PanelIndex;
    Bool        Center;
    Bool        SetDVI;
    /* LCD Simultaneous Expand Mode HWCursor Y Scale */
    Bool        scaleY;
    int         resY;

    uint32_t    diPort;
    CARD8       i2cBus;
} VIAFPRec, *VIAFPPtr;

typedef struct _VIATMDS {
    uint32_t    diPort;
    CARD8       i2cBus;
} VIATMDSRec, *VIATMDSPtr;

typedef struct
{
    CARD16 X;
    CARD16 Y;
    CARD16 Bpp;
    CARD8 bRamClock;
    CARD8 bTuningValue;
} ViaExpireNumberTable;

union pllparams {
    struct {
        CARD32 dtz : 2;
        CARD32 dr  : 3;
        CARD32 dn  : 7;
        CARD32 dm  :10;
    } params;
    CARD32 packed;
};

/*
 * DPA Setting Structure.
 */
typedef struct _VIADPA {
    CARD8   dvp0Adjustment;
    CARD8   dvp0ClockDriveStrength;
    CARD8   dvp0DataDriveStrength;
    CARD8   dvp1Adjustment;
    CARD8   dvp1ClockDriveStrength;
    CARD8   dvp1DataDriveStrength;
    CARD8   fpdpLowAdjustment;
    CARD8   fpdpHighAdjustment;
} VIADPARec, *VIADPAPtr;

typedef struct _VIADPAINFOTABLE {
    CARD32          ClockRangeIndex;
    VIADPAPtr       pDPASetting;
} VIADPAInfoTableRec, *VIADPAInfoTablePtr;

typedef struct _VIADPAINDEXTABLE {
    int      Chipset;

    VIADPAInfoTablePtr      pExtTMDSDPATable;
    VIADPAInfoTablePtr      pFPDPATable;
} VIA_DPA_INDEX_TABLE;


#define NUMBER_VIA_DPA_TABLE    (sizeof(viaDPAIndexTable) / sizeof(*(viaDPAIndexTable)))

/*
 * Sets IGA1 or IGA2 for palette LUT access.
 * This function should be called before changing the
 * contents of the palette.
 */
static inline void
viaSetPaletteLUTAccess(ScrnInfoPtr pScrn, CARD8 displaySource)
{
    ViaSeqMask(VGAHWPTR(pScrn), 0x1A,
                displaySource, BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Palette LUT Access: IGA%d\n",
                        (displaySource & BIT(0)) + 1));
}

/*
 * Resets IGA1 hardware.
 */
static inline void
viaIGA1HWReset(ScrnInfoPtr pScrn, Bool resetState)
{
    /* 3X5.17[7] - IGA1 HW Reset
     *             0: Reset
     *             1: Normal Operation */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x17, resetState ? 0x00 : BIT(7),
                BIT(7));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "IGA1 HW Reset: %s\n",
                        resetState ? "On" : "Off"));
}

/*
 * Resets IGA2 hardware.
 */
static inline void
viaIGA2HWReset(ScrnInfoPtr pScrn, Bool resetState)
{
    /* 3X5.6A[6] - Second Display Channel Reset
     *             0: Reset
     *             1: Normal Operation */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x6A, resetState ? 0x00 : BIT(6), BIT(6));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "IGA2 HW Reset: %s\n",
                        resetState ? "On" : "Off"));
}

/*
 * Sets IGA1 palette LUT resolution. (6-bit or 8-bit)
 */
static inline void
viaIGA1SetPaletteLUTResolution(ScrnInfoPtr pScrn, Bool paletteLUT)
{
    /* 3C5.15[7] - IGA1 6 / 8 Bit LUT
     *             0: 6-bit
     *             1: 8-bit */
    ViaSeqMask(VGAHWPTR(pScrn), 0x15,
                paletteLUT ? BIT(7) : 0x00, BIT(7));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "IGA1 Palette LUT Resolution: %s bit\n",
                        paletteLUT ? "8" : "6"));
}

/*
 * Sets IGA2 palette LUT resolution. (6-bit or 8-bit)
 */
static inline void
viaIGA2SetPaletteLUTResolution(ScrnInfoPtr pScrn, Bool paletteLUT)
{
    /* Set the palette LUT resolution for IGA2. */
    /* 3X5.6A[5] - IGA2 6 / 8 Bit LUT
     *             0: 6-bit
     *             1: 8-bit */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x6A,
                paletteLUT ? BIT(5) : 0x00, BIT(5));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "IGA2 Palette LUT Resolution: %s bit\n",
                        paletteLUT ? "8" : "6"));
}

/*
 * Sets IGA1 display output state.
 */
static inline void
viaIGA1SetDisplayOutput(ScrnInfoPtr pScrn, Bool outputState)
{
    /* 3C5.01[5] - IGA1 Screen Off
     *             0: Screen on
     *             1: Screen off */
    ViaSeqMask(VGAHWPTR(pScrn), 0x01, outputState ? 0x00 : BIT(5), BIT(5));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "IGA1 Display Output: %s\n",
                        outputState ? "On" : "Off"));
}

/*
 * Sets IGA2 display output state.
 */
static inline void
viaIGA2SetDisplayOutput(ScrnInfoPtr pScrn, Bool outputState)
{
    /* 3X5.6B[2] - IGA2 Screen Off
     *             0: Screen on
     *             1: Screen off */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x6B,
                outputState ? 0x00 : BIT(2), BIT(2));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "IGA2 Display Output: %s\n",
                        outputState ? "On" : "Off"));
}

/*
 * Controls IGA2 display channel state.
 */
static inline void
viaIGA2DisplayChannel(ScrnInfoPtr pScrn, Bool channelState)
{
    /* 3X5.6A[7] - Second Display Channel Enable */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x6A,
                channelState ? BIT(7) : 0x00, BIT(7));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "IGA2 Display Channel: %s\n",
                        channelState ? "On" : "Off"));
}

/*
 * Sets DIP0 (Digital Interface Port 0) I/O pad state.
 * CLE266 chipset only.
 */
static inline void
viaDIP0SetIOPadState(ScrnInfoPtr pScrn, CARD8 ioPadState)
{
    /* 3C5.1E[7:6] - DIP0 Power Control
     *               0x: Pad always off
     *               10: Depend on the other control signal
     *               11: Pad on/off according to the
     *                   Power Management Status (PMS) */
    ViaSeqMask(VGAHWPTR(pScrn), 0x1E,
                ioPadState << 6, BIT(7) | BIT(6));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DIP0 I/O Pad State: %s\n",
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x03) ?
                            "On" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x02) ?
                            "Conditional" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x01) ?
                            "Off" :
                            "Off"));
}

/*
 * Output enable of DIP0 (Digital Interface Port 0) interface.
 * CLE266 chipset only.
 */
static inline void
viaDIP0SetOutputEnable(ScrnInfoPtr pScrn, Bool outputEnable)
{
    /*
     * 3X5.6C[0] - DIP0 Output Enable
     *             0: Output Disable
     *             1: Output Enable
     */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x6C,
                outputEnable ? 0x01 : 0x00, BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DIP0 Output: %s\n",
                        outputEnable ? "Enable" : "Disable"));
}

/*
 * Sets the clock source of DIP0 (Digital Interface Port 0)
 * interface. CLE266 chipset only.
 */
static inline void
viaDIP0SetClockSource(ScrnInfoPtr pScrn, Bool clockSource)
{
    /*
     * 3X5.6C[5] - DIP0 Clock Source
     *             0: External
     *             1: Internal
     */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x6C,
                clockSource ? BIT(5) : 0x00, BIT(5));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DIP0 Clock Source: %s\n",
                        clockSource ? "Internal" : "External"));
}

/*
 * Sets the display source of DIP0 (Digital Interface Port 0)
 * interface. CLE266 chipset only.
 */
static inline void
viaDIP0SetDisplaySource(ScrnInfoPtr pScrn, CARD8 displaySource)
{
    /* 3X5.6C[7] - DIP0 Data Source Selection
     *             0: Primary Display
     *             1: Secondary Display */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x6C,
                displaySource << 7, BIT(7));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DIP0 Display Source: IGA%d\n",
                        (displaySource & 0x01) + 1));
}

/*
 * Sets DIP1 (Digital Interface Port 1) I/O pad state.
 * CLE266 chipset only.
 */
static inline void
viaDIP1SetIOPadState(ScrnInfoPtr pScrn, CARD8 ioPadState)
{
    /*
     * 3C5.1E[5:4] - DIP1 I/O Pad Control
     *               00: I/O pad off
     *               11: I/O pad on
     */
    ViaSeqMask(VGAHWPTR(pScrn), 0x1E,
                ioPadState << 4, BIT(5) | BIT(4));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DIP1 I/O Pad State: %s\n",
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x03) ?
                            "On" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x02) ?
                            "Unknown" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x01) ?
                            "Unknown" :
                            "Off"));
}

/*
 * Output enable of DIP1 (Digital Interface Port 1) interface.
 * CLE266 chipset only.
 */
static inline void
viaDIP1SetOutputEnable(ScrnInfoPtr pScrn, Bool outputEnable)
{
    /*
     * 3X5.93[0] - DIP1 Output Enable
     *             0: Output Disable
     *             1: Output Enable
     */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x93,
                outputEnable ? 0x01 : 0x00, BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DIP1 Output: %s\n",
                        outputEnable ? "Enable" : "Disable"));
}

/*
 * Sets the clock source of DIP1 (Digital Interface Port 1)
 * interface. CLE266 chipset only.
 */
static inline void
viaDIP1SetClockSource(ScrnInfoPtr pScrn, Bool clockSource)
{
    /*
     * 3X5.93[5] - DIP1 Clock Source
     *             0: External
     *             1: Internal
     */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x93,
                clockSource ? BIT(5) : 0x00, BIT(5));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DIP1 Clock Source: %s\n",
                        clockSource ? "Internal" : "External"));
}

/*
 * Sets the display source of DIP1 (Digital Interface Port 1)
 * interface. CLE266 chipset only.
 */
static inline void
viaDIP1SetDisplaySource(ScrnInfoPtr pScrn, uint8_t displaySource)
{
    /*
     * 3X5.93[7] - DIP1 Data Source Selection
     *             0: IGA1
     *             1: IGA2
     */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x93,
                displaySource << 7, BIT(7));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DIP1 Display Source: IGA%d\n",
                        (displaySource & 0x01) + 1));
}


/*
 * Sets DVP0 (Digital Video Port 0) I/O pad state.
 */
static inline void
viaDVP0SetIOPadState(ScrnInfoPtr pScrn, CARD8 ioPadState)
{
    /* 3C5.1E[7:6] - DVP0 Power Control
     *               0x: Pad always off
     *               10: Depend on the other control signal
     *               11: Pad on/off according to the
     *                   Power Management Status (PMS) */
    ViaSeqMask(VGAHWPTR(pScrn), 0x1E, ioPadState << 6, BIT(7) | BIT(6));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP0 I/O Pad State: %s\n",
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x03) ? "On" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x02) ? "Conditional" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x01) ? "Off" :
                                                                     "Off"));
}

/*
 * Sets DVP0 (Digital Video Port 0) clock I/O pad drive strength.
 */
static inline void
viaDVP0SetClockDriveStrength(ScrnInfoPtr pScrn, CARD8 clockDriveStrength)
{
    /* 3C5.1E[2] - DVP0 Clock Drive Strength Bit [0] */
    ViaSeqMask(VGAHWPTR(pScrn), 0x1E, clockDriveStrength << 2,
                BIT(2));

    /* 3C5.2A[4] - DVP0 Clock Drive Strength Bit [1] */
    ViaSeqMask(VGAHWPTR(pScrn), 0x2A, clockDriveStrength << 3,
                BIT(4));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP0 Clock I/O Pad Drive Strength: %u\n",
                        clockDriveStrength & (BIT(1) | BIT(0))));
}

/*
 * Sets DVP0 (Digital Video Port 0) data I/O pads drive strength.
 */
static inline void
viaDVP0SetDataDriveStrength(ScrnInfoPtr pScrn, CARD8 dataDriveStrength)
{
    /* 3C5.1B[1] - DVP0 Data Drive Strength Bit [0] */
    ViaSeqMask(VGAHWPTR(pScrn), 0x1B, dataDriveStrength << 1, BIT(1));

    /* 3C5.2A[5] - DVP0 Data Drive Strength Bit [1] */
    ViaSeqMask(VGAHWPTR(pScrn), 0x2A, dataDriveStrength << 4, BIT(5));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP0 Data I/O Pads Drive Strength: %u\n",
                        dataDriveStrength & (BIT(1) | BIT(0))));
}

/*
 * Sets DVP0 (Digital Video Port 0) adjustment register.
 */
static inline void
viaDVP0SetAdjustment(ScrnInfoPtr pScrn, CARD8 adjustment)
{
    /* 3X5.96[3:0] - DVP0 Adjustment */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x96,
                adjustment, BIT(3) | BIT(2) | BIT(1) | BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP0 Adjustment: %d\n",
                        (adjustment & (BIT(3) | BIT(2) |
                                     BIT(1) | BIT(0)))));
}

/*
 * Sets DVP0 (Digital Video Port 0) sync polarity.
 */
static inline void
viaDVP0SetSyncPolarity(ScrnInfoPtr pScrn, CARD8 syncPolarity)
{
    /* 3X5.96[6] - DVP0 VSYNC Polarity
     *             0: Positive
     *             1: Negative
     * 3X5.96[5] - DVP0 HSYNC Polarity
     *             0: Positive
     *             1: Negative */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x96,
                syncPolarity << 5, BIT(6) | BIT(5));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP0 Horizontal Sync Polarity: %s\n",
                        (syncPolarity & BIT(0)) ? "-" : "+"));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP0 Vertical Sync Polarity: %s\n",
                        (syncPolarity & BIT(1)) ? "-" : "+"));
}

/*
 * Sets the display source of DVP0 (Digital Video Port 0) interface.
 */
static inline void
viaDVP0SetDisplaySource(ScrnInfoPtr pScrn, CARD8 displaySource)
{
    /* 3X5.96[4] - DVP0 Data Source Selection
     *             0: Primary Display
     *             1: Secondary Display */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x96, displaySource << 4, BIT(4));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP0 Display Source: IGA%d\n",
                        (displaySource & 0x01) + 1));
}

/*
 * Sets DVP1 (Digital Video Port 1) I/O pad state.
 */
static inline void
viaDVP1SetIOPadState(ScrnInfoPtr pScrn, CARD8 ioPadState)
{
    /* 3C5.1E[5:4] - DVP1 Power Control
     *               0x: Pad always off
     *               10: Depend on the other control signal
     *               11: Pad on/off according to the
     *                   Power Management Status (PMS) */
    ViaSeqMask(VGAHWPTR(pScrn), 0x1E, ioPadState << 4, BIT(5) | BIT(4));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP1 I/O Pad State: %s\n",
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x03) ? "On" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x02) ? "Conditional" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x01) ? "Off" :
                                                                     "Off"));
}

/*
 * Sets DVP1 (Digital Video Port 1) clock I/O pad drive strength.
 */
static inline void
viaDVP1SetClockDriveStrength(ScrnInfoPtr pScrn, CARD8 clockDriveStrength)
{
    /* 3C5.65[3:2] - DVP1 Clock Pads Driving Select [1:0]
     *               00: lowest
     *               01: low
     *               10: high
     *               11: highest */
    ViaSeqMask(VGAHWPTR(pScrn), 0x65, clockDriveStrength << 2, BIT(3) | BIT(2));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP1 Clock I/O Pad Drive Strength: %u\n",
                        clockDriveStrength & (BIT(1) | BIT(0))));
}

/*
 * Sets DVP1 (Digital Video Port 1) data I/O pads drive strength.
 */
static inline void
viaDVP1SetDataDriveStrength(ScrnInfoPtr pScrn, CARD8 dataDriveStrength)
{
    /* 3C5.65[1:0] - DVP1 Data Pads Driving Select [1:0}
     *               00: lowest
     *               01: low
     *               10: high
     *               11: highest */
    ViaSeqMask(VGAHWPTR(pScrn), 0x65, dataDriveStrength, BIT(1) | BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP1 Data I/O Pads Drive Strength: %u\n",
                        dataDriveStrength & (BIT(1) | BIT(0))));
}

/*
 * Sets DVP1 (Digital Video Port 1) adjustment register.
 */
static inline void
viaDVP1SetAdjustment(ScrnInfoPtr pScrn, CARD8 adjustment)
{
    /* 3X5.9B[3:0] - DVP1 Adjustment */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x9B,
                adjustment, BIT(3) | BIT(2) | BIT(1) | BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP1 Adjustment: %d\n",
                        (adjustment & (BIT(3) | BIT(2) |
                                     BIT(1) | BIT(0)))));
}

/*
 * Sets DVP1 (Digital Video Port 1) sync polarity.
 */
static inline void
viaDVP1SetSyncPolarity(ScrnInfoPtr pScrn, CARD8 syncPolarity)
{
    /* 3X5.9B[6] - DVP1 VSYNC Polarity
     *             0: Positive
     *             1: Negative
     * 3X5.9B[5] - DVP1 HSYNC Polarity
     *             0: Positive
     *             1: Negative */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x9B,
                syncPolarity << 5, BIT(6) | BIT(5));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP1 Horizontal Sync Polarity: %s\n",
                        (syncPolarity & BIT(0)) ? "-" : "+"));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP1 Vertical Sync Polarity: %s\n",
                        (syncPolarity & BIT(1)) ? "-" : "+"));
}

/*
 * Sets the display source of DVP1 (Digital Video Port 1) interface.
 */
static inline void
viaDVP1SetDisplaySource(ScrnInfoPtr pScrn, CARD8 displaySource)
{
    /* 3X5.9B[4] - DVP1 Data Source Selection
     *             0: Primary Display
     *             1: Secondary Display */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x9B, displaySource << 4, BIT(4));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "DVP1 Display Source: IGA%d\n",
                        (displaySource & 0x01) + 1));
}

/*
 * Sets analog (VGA) DAC power.
 */
static inline void
viaAnalogSetPower(ScrnInfoPtr pScrn, Bool outputState)
{
    /* 3X5.47[2] - DACOFF Backdoor Register
     *             0: DAC on
     *             1: DAC off */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x47,
                outputState ? 0x00 : BIT(2), BIT(2));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Analog (VGA) Power: %s\n",
                        outputState ? "On" : "Off"));
}

/*
 * Sets analog (VGA) DAC off setting.
 * Only available in CX700 / VX700, VX800, VX855, and VX900 chipsets.
 */
static inline void
viaAnalogSetDACOff(ScrnInfoPtr pScrn, Bool dacOff)
{
    /* 3C5.5E[0] - CRT DACOFF Setting
     *             0: Disabled
     *             1: DAC is controlled by 3C5.01[5] */
    ViaSeqMask(VGAHWPTR(pScrn), 0x5E,
                dacOff ? BIT(0) : 0x00, BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Analog (VGA) DAC Off Setting: %s\n",
                        dacOff ? "On" : "Off"));
}

/*
 * Sets analog (VGA) DPMS State.
 */
static inline void
viaAnalogSetDPMSControl(ScrnInfoPtr pScrn, CARD8 dpmsControl)
{
    /* 3X5.36[5:4] - DPMS Control
     *               00: On
     *               01: Stand-by
     *               10: Suspend
     *               11: Off */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x36,
                    dpmsControl << 4, BIT(5) | BIT(4));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Analog (VGA) DPMS: %s\n",
                        ((dpmsControl & (BIT(1) | BIT(0))) == 0x03) ?
                            "Off" :
                        ((dpmsControl & (BIT(1) | BIT(0))) == 0x02) ?
                            "Suspend" :
                        ((dpmsControl & (BIT(1) | BIT(0))) == 0x01) ?
                            "Standby" :
                            "On"));
}

/*
 * Sets analog (VGA) sync polarity.
 */
static inline void
viaAnalogSetSyncPolarity(ScrnInfoPtr pScrn, CARD8 syncPolarity)
{
    /* 3C2[7] - Analog Vertical Sync Polarity
     *          0: Positive
     *          1: Negative
     * 3C2[6] - Analog Horizontal Sync Polarity
     *          0: Positive
     *          1: Negative */
    VGAHWPTR(pScrn)->writeMiscOut(VGAHWPTR(pScrn),
            ((VGAHWPTR(pScrn)->readMiscOut(VGAHWPTR(pScrn)))
                & ~((BIT(1) | BIT(0)) << 6))
            | ((syncPolarity & (BIT(1) | BIT(0))) << 6));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Analog (VGA) Horizontal Sync Polarity: %s\n",
                        (syncPolarity & BIT(0)) ? "-" : "+"));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Analog (VGA) Vertical Sync Polarity: %s\n",
                        (syncPolarity & BIT(1)) ? "-" : "+"));
}

/*
 * Sets analog (VGA) display source.
 */
static inline void
viaAnalogSetDisplaySource(ScrnInfoPtr pScrn, CARD8 displaySource)
{
    /* 3C5.16[6] - CRT Display Source
     *             0: Primary Display Stream (IGA1)
     *             1: Secondary Display Stream (IGA2) */
    ViaSeqMask(VGAHWPTR(pScrn), 0x16,
                displaySource << 6, BIT(6));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Analog (VGA) Display Source: IGA%d\n",
                        (displaySource & 0x01) + 1));
}

/*
 * Sets KM400 or later chipset's FP primary power sequence control
 * type.
 */
static inline void
viaFPSetPrimaryPowerSeqType(ScrnInfoPtr pScrn, Bool ctrlType)
{
    /* Set FP primary power sequence control type. */
    /* 3X5.91[0] - FP Primary Power Sequence Control Type
     *             0: Hardware Control
     *             1: Software Control */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x91, ctrlType ? 0x00 : BIT(0),
                BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Primary Power Sequence Control Type: "
                        "%s Control\n",
                        ctrlType ? "Hardware" : "Software"));
}

/*
 * Sets KM400 or later chipset's FP primary software controlled
 * back light.
 */
static inline void
viaFPSetPrimarySoftBackLight(ScrnInfoPtr pScrn, Bool softOn)
{
    /* 3X5.91[1] - FP Primary Software Back Light On
     *             0: Off
     *             1: On */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x91,
                softOn ? BIT(1) : 0x00, BIT(1));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Primary Software Controlled Back Light: "
                        "%s\n",
                        softOn ? "On" : "Off"));
}

/*
 * Sets KM400 or later chipset's FP primary software controlled
 * VEE.
 */
static inline void
viaFPSetPrimarySoftVEE(ScrnInfoPtr pScrn, Bool softOn)
{
    /* 3X5.91[2] - FP Primary Software VEE On
     *             0: Off
     *             1: On */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x91,
                softOn ? BIT(2) : 0x00, BIT(2));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Primary Software Controlled VEE: "
                        "%s\n",
                        softOn ? "On" : "Off"));
}

/*
 * Sets KM400 or later chipset's FP primary software controlled
 * data.
 */
static inline void
viaFPSetPrimarySoftData(ScrnInfoPtr pScrn, Bool softOn)
{
    /* 3X5.91[3] - FP Primary Software Data On
     *             0: Off
     *             1: On */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x91,
                softOn ? BIT(3) : 0x00, BIT(3));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Primary Software Controlled Data: "
                        "%s\n",
                        softOn ? "On" : "Off"));
}

/*
 * Sets KM400 or later chipset's FP primary software controlled
 * VDD.
 */
static inline void
viaFPSetPrimarySoftVDD(ScrnInfoPtr pScrn, Bool softOn)
{
    /* 3X5.91[4] - FP Primary Software VDD On
     *             0: Off
     *             1: On */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x91,
                softOn ? BIT(4) : 0x00, BIT(4));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Primary Software Controlled VDD: "
                        "%s\n",
                        softOn ? "On" : "Off"));
}

/*
 * Sets KM400 or later chipset's FP primary direct back light control
 * bit.
 */
static inline void
viaFPSetPrimaryDirectBackLightCtrl(ScrnInfoPtr pScrn, Bool directOn)
{
    /* Set FP primary direct back light control bit. */
    /* 3X5.91[6] - FP Primary Direct Back Light Control
     *             0: On
     *             1: Off */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x91, directOn ? 0x00 : BIT(6),
                BIT(6));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Primary Direct Back Light Control: %s\n",
                        directOn ? "On" : "Off"));
}

/*
 * Sets KM400 or later chipset's FP primary direct display period
 * control bit.
 */
static inline void
viaFPSetPrimaryDirectDisplayPeriod(ScrnInfoPtr pScrn, Bool directOn)
{
    /* Set FP primary direct display period bit. */
    /* 3X5.91[7] - FP Primary Direct Display Period Control
     *             0: On
     *             1: Off */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x91, directOn ? 0x00 : BIT(7),
                BIT(7));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Primary Direct Display Period Control: %s\n",
                        directOn ? "On" : "Off"));
}

/*
 * Sets KM400 or later chipset's FP primary hardware controlled
 * power sequence bit.
 */
static inline void
viaFPSetPrimaryHardPower(ScrnInfoPtr pScrn, Bool powerState)
{
    /* Set FP primary hardware controlled power sequence bit. */
    /* 3X5.6A[3] - FP Primary Hardware Controlled Power Sequence
     *             0: Hardware Controlled Power Off
     *             1: Hardware Controlled Power On */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x6A, powerState ? BIT(3) : 0x00,
                BIT(3));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Primary Hardware Controlled Power "
                        "Sequence: %s\n",
                        powerState ? "On" : "Off"));
}

/*
 * Sets FP secondary power sequence control type.
 */
static inline void
viaFPSetSecondaryPowerSeqType(ScrnInfoPtr pScrn, Bool ctrlType)
{
    /* 3X5.D3[0] - FP Secondary Power Sequence Control Type
     *             0: Hardware Control
     *             1: Software Control */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD3, ctrlType ? 0x00 : BIT(0),
                BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Secondary Power Sequence Control Type: "
                        "%s Control\n",
                        ctrlType ? "Hardware" : "Software"));
}

/*
 * Sets CX700 / VX700 or later chipset's FP secondary
 * software controlled back light.
 */
static inline void
viaFPSetSecondarySoftBackLight(ScrnInfoPtr pScrn, Bool softOn)
{
    /* 3X5.D3[1] - FP Secondary Software Back Light On
     *             0: Off
     *             1: On */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD3,
                softOn ? BIT(1) : 0x00, BIT(1));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Secondary Software Controlled "
                        "Back Light: %s\n", softOn ? "On" : "Off"));
}

/*
 * Sets CX700 / VX700 or later chipset's FP secondary software
 * controlled VEE.
 */
static inline void
viaFPSetSecondarySoftVEE(ScrnInfoPtr pScrn, Bool softOn)
{
    /* 3X5.D3[2] - FP Secondary Software VEE On
     *             0: Off
     *             1: On */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD3,
                softOn ? BIT(2) : 0x00, BIT(2));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Secondary Software Controlled VEE: %s\n",
                        softOn ? "On" : "Off"));
}

/*
 * Sets CX700 / VX700 or later chipset's FP secondary software
 * controlled data.
 */
static inline void
viaFPSetSecondarySoftData(ScrnInfoPtr pScrn, Bool softOn)
{
    /* 3X5.D3[3] - FP Secondary Software Data On
     *             0: Off
     *             1: On */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD3,
                softOn ? BIT(3) : 0x00, BIT(3));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Secondary Software Controlled Data: %s\n",
                        softOn ? "On" : "Off"));
}

/*
 * Sets CX700 / VX700 or later chipset's FP secondary software
 * controlled VDD.
 */
static inline void
viaFPSetSecondarySoftVDD(ScrnInfoPtr pScrn, Bool softOn)
{
    /* 3X5.D3[4] - FP Secondary Software VDD On
     *             0: Off
     *             1: On */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD3,
                softOn ? BIT(4) : 0x00, BIT(4));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Secondary Software Controlled VDD: %s\n",
                        softOn ? "On" : "Off"));
}

/*
 * Sets CX700 / VX700 or later chipset's FP secondary direct back
 * light control.
 */
static inline void
viaFPSetSecondaryDirectBackLightCtrl(ScrnInfoPtr pScrn, Bool directOn)
{
    /* 3X5.D3[6] - FP Secondary Direct Back Light Control
     *             0: On
     *             1: Off */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD3,
                directOn ? 0x00 : BIT(6), BIT(6));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Secondary Direct Back Light "
                        "Control: %s\n", directOn ? "On" : "Off"));
}

/*
 * Sets CX700 / VX700 or later chipset's FP secondary direct
 * display period control.
 */
static inline void
viaFPSetSecondaryDirectDisplayPeriod(ScrnInfoPtr pScrn, Bool directOn)
{
    /* 3X5.D3[7] - FP Secondary Direct Display Period Control
     *             0: On
     *             1: Off */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD3,
                directOn ? 0x00 : BIT(7), BIT(7));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Secondary Direct Display Period "
                        "Control: %s\n", directOn ? "On" : "Off"));
}

/*
 * Sets FP secondary hardware controlled power sequence enable bit.
 */
static inline void
viaFPSetSecondaryHardPower(ScrnInfoPtr pScrn, Bool powerState)
{
    /* 3X5.D4[1] - Secondary Power Hardware Power Sequence Enable
     *             0: Off
     *             1: On */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD4,
                powerState ? BIT(1) : 0x00, BIT(1));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FP Secondary Hardware Controlled Power "
                        "Sequence: %s\n",
                        powerState ? "On" : "Off"));
}

/*
 * Sets FPDP (Flat Panel Display Port) Low I/O pad state.
 */
static inline void
viaFPDPLowSetIOPadState(ScrnInfoPtr pScrn, CARD8 ioPadState)
{
    /* 3C5.2A[1:0] - FPDP Low Power Control
     *               0x: Pad always off
     *               10: Depend on the other control signal
     *               11: Pad on/off according to the
     *                   Power Management Status (PMS) */
    ViaSeqMask(VGAHWPTR(pScrn), 0x2A,
                ioPadState, BIT(1) | BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FPDP Low I/O Pad State: %s\n",
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x03) ?
                            "On" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x02) ?
                            "Conditional" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x01) ?
                            "Off" :
                            "Off"));
}

/*
 * Sets FPDP (Flat Panel Display Port) Low adjustment register.
 */
static inline void
viaFPDPLowSetAdjustment(ScrnInfoPtr pScrn, CARD8 adjustment)
{
    /* 3X5.99[3:0] - FPDP Low Adjustment */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x99,
            adjustment, BIT(3) | BIT(2) | BIT(1) | BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FPDP Low Adjustment: %d\n",
                        (adjustment & (BIT(3) | BIT(2) |
                                     BIT(1) | BIT(0)))));
}

/*
 * Sets FPDP (Flat Panel Display Port) Low interface display source.
 */
static inline void
viaFPDPLowSetDisplaySource(ScrnInfoPtr pScrn, CARD8 displaySource)
{
    /* 3X5.99[4] - FPDP Low Data Source Selection
     *             0: Primary Display
     *             1: Secondary Display */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x99,
                displaySource << 4, BIT(4));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FPDP Low Display Source: IGA%d\n",
                        (displaySource & 0x01) + 1));
}

/*
 * Sets FPDP (Flat Panel Display Port) High I/O pad state.
 */
static inline void
viaFPDPHighSetIOPadState(ScrnInfoPtr pScrn, CARD8 ioPadState)
{
    /* 3C5.2A[3:2] - FPDP High Power Control
     *               0x: Pad always off
     *               10: Depend on the other control signal
     *               11: Pad on/off according to the
     *                   Power Management Status (PMS) */
    ViaSeqMask(VGAHWPTR(pScrn), 0x2A,
                ioPadState << 2, BIT(3) | BIT(2));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FPDP High I/O Pad State: %s\n",
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x03) ?
                            "On" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x02) ?
                            "Conditional" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x01) ?
                            "Off" :
                            "Off"));
}

/*
 * Sets FPDP (Flat Panel Display Port) High adjustment register.
 */
static inline void
viaFPDPHighSetAdjustment(ScrnInfoPtr pScrn, CARD8 adjustment)
{
    /* 3X5.97[3:0] - FPDP High Adjustment */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x97, adjustment,
                BIT(3) | BIT(2) | BIT(1) | BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FPDP High Adjustment: %d\n",
                        (adjustment & (BIT(3) | BIT(2) |
                                     BIT(1) | BIT(0)))));
}

/*
 * Sets FPDP (Flat Panel Display Port) High interface display source.
 */
static inline void
viaFPDPHighSetDisplaySource(ScrnInfoPtr pScrn, CARD8 displaySource)
{
    /* 3X5.97[4] - FPDP High Data Source Selection
     *             0: Primary Display
     *             1: Secondary Display */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x97,
                displaySource << 4, BIT(4));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "FPDP High Display Source: IGA%d\n",
                        (displaySource & 0x01) + 1));
}

/*
 * Sets LVDS1 power state.
 */
static inline void
viaLVDS1SetPower(ScrnInfoPtr pScrn, Bool powerState)
{
    /* 3X5.D2[7] - Power Down (Active High) for Channel 1 LVDS
     *             0: LVDS1 power on
     *             1: LVDS1 power down */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD2,
                powerState ? 0x00 : BIT(7), BIT(7));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS1 Power State: %s\n",
                        powerState ? "On" : "Off"));
}

/*
 * Sets CX700 or later single chipset's LVDS1 power sequence type.
 */
static inline void
viaLVDS1SetPowerSeq(ScrnInfoPtr pScrn, Bool ctrlType)
{
    /* Set LVDS1 power sequence type. */
    /* 3X5.91[0] - LVDS1 Hardware or Software Control Power Sequence
     *             0: Hardware Control
     *             1: Software Control */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x91, ctrlType ? BIT(0) : 0x00,
                BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS1 Power Sequence: %s Control\n",
                        ctrlType ? "Software" : "Hardware"));
}

/*
 * Sets CX700 or later single chipset's LVDS1 software controlled Vdd.
 */
static inline void
viaLVDS1SetSoftVdd(ScrnInfoPtr pScrn, Bool softOn)
{
    /* Set LVDS1 software controlled Vdd. */
    /* 3X5.91[4] - Software VDD On
     *             0: Off
     *             1: On */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x91, softOn ? BIT(4) : 0,
                BIT(4));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS1 Software Controlled Vdd: %s\n",
                        softOn ? "On" : "Off"));
}

/*
 * Sets LVDS1 I/O pad state.
 */
static inline void
viaLVDS1SetIOPadSetting(ScrnInfoPtr pScrn, CARD8 ioPadState)
{
    /* 3C5.2A[1:0] - LVDS1 I/O Pad Control
     *               0x: Pad always off
     *               10: Depend on the other control signal
     *               11: Pad on/off according to the
     *                   Power Management Status (PMS) */
    ViaSeqMask(VGAHWPTR(pScrn), 0x2A,
                ioPadState, BIT(1) | BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS1 I/O Pad State: %s\n",
            ((ioPadState & (BIT(1) | BIT(0))) == 0x03) ?
                "On" :
            ((ioPadState & (BIT(1) | BIT(0))) == 0x02) ?
                "Conditional" :
            ((ioPadState & (BIT(1) | BIT(0))) == 0x01) ?
                "Off" :
                "Off"));
}

/*
 * Sets LVDS1 format.
 */
static inline void
viaLVDS1SetFormat(ScrnInfoPtr pScrn, CARD8 format)
{
    /* 3X5.D2[1] - LVDS Channel 1 Format Selection
     *             0: SPWG Mode
     *             1: OPENLDI Mode */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD2,
                format << 1, BIT(1));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS1 Format: %s\n",
                        (format & BIT(0)) ? "OPENLDI" : "SPWG"));
}

/*
 * Sets LVDS1 output format (rotation or sequential mode).
 */
static inline void
viaLVDS1SetOutputFormat(ScrnInfoPtr pScrn, CARD8 outputFormat)
{
    /* 3X5.88[6] - LVDS Channel 1 Output Format
     *             0: Rotation
     *             1: Sequential */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x88,
                outputFormat << 6, BIT(6));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS1 Output Format: %s\n",
                        (outputFormat & BIT(0)) ?
                        "Sequential" : "Rotation"));
}

/*
 * Sets LVDS1 output color dithering (18-bit color display vs.
 * 24-bit color display).
 */
static inline void
viaLVDS1SetDithering(ScrnInfoPtr pScrn, Bool dithering)
{
    /* 3X5.88[0] - LVDS Channel 1 Output Bits
     *             0: 24 bits (dithering off)
     *             1: 18 bits (dithering on) */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x88,
                dithering ? BIT(0) : 0x00, BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS1 Color Dithering: %s\n",
                        dithering ?
                        "On (18 bit color)" : "Off (24 bit color)"));
}

/*
 * Sets LVDS1 sync polarity.
 */
static inline void
viaLVDS1SetSyncPolarity(ScrnInfoPtr pScrn, CARD8 syncPolarity)
{
    /* 3X5.99[6] - LVDS1 VSYNC Polarity
     *             0: Positive
     *             1: Negative
     * 3X5.99[5] - LVDS1 HSYNC Polarity
     *             0: Positive
     *             1: Negative */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x99,
                syncPolarity << 5, BIT(6) | BIT(5));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS1 Horizontal Sync Polarity: %s\n",
                        (syncPolarity & BIT(0)) ? "-" : "+"));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS1 Vertical Sync Polarity: %s\n",
                        (syncPolarity & BIT(1)) ? "-" : "+"));
}

/*
 * Sets LVDS1 display source.
 */
static inline void
viaLVDS1SetDisplaySource(ScrnInfoPtr pScrn, CARD8 displaySource)
{
    /* 3X5.99[4] - LVDS Channel 1 Data Source Selection
     *             0: Primary Display
     *             1: Secondary Display */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x99,
                displaySource << 4, BIT(4));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS1 Display Source: IGA%d\n",
                        (displaySource & 0x01) + 1));
}

/*
 * Sets LVDS2 power state.
 */
static inline void
viaLVDS2SetPower(ScrnInfoPtr pScrn, Bool powerState)
{
    /* 3X5.D2[6] - Power Down (Active High) for Channel 2 LVDS
     *             0: LVDS2 power on
     *             1: LVDS2 power down */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD2,
                powerState ? 0x00 : BIT(6), BIT(6));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS2 Power State: %s\n",
                        powerState ? "On" : "Off"));
}

/*
 * Sets LVDS2 I/O pad state.
 */
static inline void
viaLVDS2SetIOPadSetting(ScrnInfoPtr pScrn, CARD8 ioPadState)
{
    /* 3C5.2A[3:2] - LVDS2 I/O Pad Control
     *               0x: Pad always off
     *               10: Depend on the other control signal
     *               11: Pad on/off according to the
     *                   Power Management Status (PMS) */
    ViaSeqMask(VGAHWPTR(pScrn), 0x2A,
                ioPadState << 2, BIT(3) | BIT(2));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS2 I/O Pad State: %s\n",
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x03) ?
                            "On" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x02) ?
                            "Conditional" :
                        ((ioPadState & (BIT(1) | BIT(0))) == 0x01) ?
                            "Off" :
                            "Off"));
}

/*
 * Sets LVDS2 format.
 */
static inline void
viaLVDS2SetFormat(ScrnInfoPtr pScrn, CARD8 format)
{
    /* 3X5.D2[0] - LVDS Channel 2 Format Selection
     *             0: SPWG Mode
     *             1: OPENLDI Mode */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD2, format, BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS2 Format: %s\n",
                        (format & BIT(0)) ? "OPENLDI" : "SPWG"));
}

/*
 * Sets LVDS2 output format (rotation or sequential mode).
 */
static inline void
viaLVDS2SetOutputFormat(ScrnInfoPtr pScrn, CARD8 outputFormat)
{
    /* 3X5.D4[7] - LVDS Channel 2 Output Format
     *             0: Rotation
     *             1: Sequential */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD4, outputFormat << 7, BIT(7));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS2 Output Format: %s\n",
                        (outputFormat & BIT(0)) ? "Sequential" : "Rotation"));
}

/*
 * Sets LVDS2 output color dithering (18-bit color display vs.
 * 24-bit color display).
 */
static inline void
viaLVDS2SetDithering(ScrnInfoPtr pScrn, Bool dithering)
{
    /* 3X5.D4[6] - LVDS Channel 2 Output Bits
     *             0: 24 bits (dithering off)
     *             1: 18 bits (dithering on) */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD4, dithering ? BIT(6) : 0x00, BIT(6));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS2 Color Dithering: %s\n",
                        dithering ? "On (18 bit color)" : "Off (24 bit color)"));
}

/*
 * Sets LVDS2 sync polarity.
 */
static inline void
viaLVDS2SetSyncPolarity(ScrnInfoPtr pScrn, CARD8 syncPolarity)
{
    /* 3X5.97[6] - LVDS2 VSYNC Polarity
     *             0: Positive
     *             1: Negative
     * 3X5.97[5] - LVDS2 HSYNC Polarity
     *             0: Positive
     *             1: Negative */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x97,
                syncPolarity << 5, BIT(6) | BIT(5));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS2 Horizontal Sync Polarity: %s\n",
                        (syncPolarity & BIT(0)) ? "-" : "+"));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS2 Vertical Sync Polarity: %s\n",
                        (syncPolarity & BIT(1)) ? "-" : "+"));
}

/*
 * Sets LVDS2 display source.
 */
static inline void
viaLVDS2SetDisplaySource(ScrnInfoPtr pScrn, CARD8 displaySource)
{
    /* 3X5.97[4] - LVDS Channel 2 Data Source Selection
     *             0: Primary Display
     *             1: Secondary Display */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x97,
                displaySource << 4, BIT(4));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "LVDS2 Display Source: IGA%d\n",
                        (displaySource & 0x01) + 1));
}

/*
 * Sets CX700 / VX700 and VX800 chipsets' TMDS (DVI) power state.
 */
static inline void
viaTMDSSetPower(ScrnInfoPtr pScrn, Bool powerState)
{
    /* 3X5.D2[3] - Power Down (Active High) for DVI
     *             0: TMDS power on
     *             1: TMDS power down */
    ViaCrtcMask(VGAHWPTR(pScrn), 0xD2,
                powerState ? 0x00 : BIT(3), BIT(3));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "TMDS (DVI) Power State: %s\n",
                        powerState ? "On" : "Off"));
}

/*
 * Sets CX700 / VX700 and VX800 chipsets' TMDS (DVI) sync polarity.
 */
static inline void
viaTMDSSetSyncPolarity(ScrnInfoPtr pScrn, CARD8 syncPolarity)
{
    /* Set TMDS (DVI) sync polarity. */
    /* 3X5.97[6] - DVI (TMDS) VSYNC Polarity
     *             0: Positive
     *             1: Negative
     * 3X5.97[5] - DVI (TMDS) HSYNC Polarity
     *             0: Positive
     *             1: Negative */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x97,
                syncPolarity << 5, BIT(6) | BIT(5));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "TMDS (DVI) Horizontal Sync Polarity: %s\n",
                        (syncPolarity & BIT(0)) ? "-" : "+"));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "TMDS (DVI) Vertical Sync Polarity: %s\n",
                        (syncPolarity & BIT(1)) ? "-" : "+"));
}

/*
 * Sets TMDS (DVI) display source.
 */
static inline void
viaTMDSSetDisplaySource(ScrnInfoPtr pScrn, CARD8 displaySource)
{
    /* Set integrated TMDS transmitter display output source.
     * The integrated TMDS transmitter appears to utilize LVDS1's data
     * source selection bit (3X5.99[4]). */
    /* 3X5.99[4] - LVDS Channel1 Data Source Selection
     *             0: Primary Display
     *             1: Secondary Display */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x99,
                displaySource << 4, BIT(4));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "TMDS (DVI) Display Source: IGA%d\n",
                        (displaySource & 0x01) + 1));
}

/*
 * Turn on / off display scaling engine.
 */
static inline void
viaSetDisplayScaling(ScrnInfoPtr pScrn, Bool scalingState)
{
    /*
     * 3X5.79[0] - LCD Scaling Enable
     *             0: Disable
     *             1: Enable
     */
    ViaCrtcMask(VGAHWPTR(pScrn), 0x79,
                scalingState, BIT(0));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Display Scaling: %s\n",
                        scalingState ? "On" : "Off"));
}


/* via_ums.c */
void viaDisableVQ(ScrnInfoPtr pScrn);
Bool viaUMSAccelInit(ScrnInfoPtr pScrn);
Bool viaUMSMapIOResources(ScrnInfoPtr pScrn);
void viaUMSDestroy(ScrnInfoPtr pScrn);
Bool viaUMSScreenInit(ScrnInfoPtr pScrn);
Bool viaUMSPreInit(ScrnInfoPtr pScrn);
void viaUMSPreInitExit(ScrnInfoPtr pScrn);

/* via_i2c.c */
void ViaI2CInit(ScrnInfoPtr pScrn);
Bool xf86I2CMaskByte(I2CDevPtr d, I2CByte subaddr,
                        I2CByte value, I2CByte mask);

/* via_output.c */
void viaDisplaySource(ScrnInfoPtr pScrn, uint32_t diPort, int index);
void viaIOPadState(ScrnInfoPtr pScrn, uint32_t diPort, uint8_t ioPadState);
void viaOutputEnable(ScrnInfoPtr pScrn, uint32_t diPort, Bool outputEnable);
void viaClockSource(ScrnInfoPtr pScrn, uint32_t diPort, Bool clockSource);
void viaInitDisplay(ScrnInfoPtr pScrn);
CARD32 ViaGetMemoryBandwidth(ScrnInfoPtr pScrn);
void viaSetUseExternalClock(ScrnInfoPtr pScrn);
CARD32 ViaModeDotClockTranslate(ScrnInfoPtr pScrn, DisplayModePtr mode);
void ViaSetPrimaryDotclock(ScrnInfoPtr pScrn, CARD32 clock);
void ViaSetSecondaryDotclock(ScrnInfoPtr pScrn, CARD32 clock);

/* via_display.c */
void ViaGammaDisable(ScrnInfoPtr pScrn);
void viaIGAInitCommon(ScrnInfoPtr pScrn);
void viaIGA1Init(ScrnInfoPtr pScrn);
void viaIGA1SetFBStartingAddress(xf86CrtcPtr crtc, int x, int y);
void viaIGA1SetDisplayRegister(ScrnInfoPtr pScrn, DisplayModePtr mode);
void viaIGA1Save(ScrnInfoPtr pScrn);
void viaIGA1Restore(ScrnInfoPtr pScrn);
void viaIGA2Init(ScrnInfoPtr pScrn);
void viaIGA2SetFBStartingAddress(xf86CrtcPtr crtc, int x, int y);
void viaIGA2SetDisplayRegister(ScrnInfoPtr pScrn, DisplayModePtr mode);
void viaIGA2Save(ScrnInfoPtr pScrn);
void viaIGA2Restore(ScrnInfoPtr pScrn);
void ViaShadowCRTCSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode);
extern const xf86CrtcFuncsRec via_crtc_funcs;

/* via_analog.c */
void viaAnalogProbe(ScrnInfoPtr pScrn);
void viaAnalogInit(ScrnInfoPtr pScrn);

/* via_fp.c */
void viaFPProbe(ScrnInfoPtr pScrn);
void viaFPInit(ScrnInfoPtr pScrn);

/* via_tmds.c */
void viaExtTMDSSetClockDriveStrength(ScrnInfoPtr pScrn,
                                        CARD8 clockDriveStrength);
void viaExtTMDSSetDataDriveStrength(ScrnInfoPtr pScrn,
                                        CARD8 dataDriveStrength);
void viaTMDSProbe(ScrnInfoPtr pScrn);
void viaExtTMDSProbe(ScrnInfoPtr pScrn);
void viaTMDSInit(ScrnInfoPtr pScrn);
void viaExtTMDSInit(ScrnInfoPtr pScrn);

/*via_tv.c */
#ifdef HAVE_DEBUG
void ViaTVPrintRegs(ScrnInfoPtr pScrn);
#endif
Bool via_tv_init(ScrnInfoPtr pScrn);

/* via_vt162x.c */
I2CDevPtr ViaVT162xDetect(ScrnInfoPtr pScrn, I2CBusPtr pBus, CARD8 Address);
void ViaVT162xInit(ScrnInfoPtr pScrn);

/* via_ch7xxx.c */
I2CDevPtr ViaCH7xxxDetect(ScrnInfoPtr pScrn, I2CBusPtr pBus, CARD8 Address);
void ViaCH7xxxInit(ScrnInfoPtr pScrn);

#endif /* _VIA_UMS_H_ */
