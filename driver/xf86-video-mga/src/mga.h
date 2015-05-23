/*
 * MGA Millennium (MGA2064W) functions
 *
 * Copyright 1996 The XFree86 Project, Inc.
 *
 * Authors
 *		Dirk Hohndel
 *			hohndel@XFree86.Org
 *		David Dawes
 *			dawes@XFree86.Org
 */

#ifndef MGA_H
#define MGA_H

#ifdef XSERVER_LIBPCIACCESS
#include <pciaccess.h>
#endif
#include <string.h>
#include <stdio.h>

#include "compiler.h"
#ifdef HAVE_XAA_H
#include "xaa.h"
#endif
#include "xf86fbman.h"
#include "exa.h"
#include "xf86Cursor.h"
#include "vgaHW.h"
#include "colormapst.h"
#include "xf86DDC.h"
#include "xf86xv.h"

#ifndef XF86DRI
#undef MGADRI
#endif

#ifdef MGADRI
#include "xf86drm.h"

#define _XF86DRI_SERVER_
#include "mga_dripriv.h"
#include "dri.h"
#include "GL/glxint.h"

#include "dri.h"

#include "GL/glxint.h"
#include "mga_dri.h"
#endif

#include "compat-api.h"

typedef enum {
    OPTION_SW_CURSOR,
    OPTION_HW_CURSOR,
    OPTION_PCI_RETRY,
    OPTION_SYNC_ON_GREEN,
    OPTION_NOACCEL,
    OPTION_SHOWCACHE,
    OPTION_OVERLAY,
    OPTION_MGA_SDRAM,
    OPTION_SHADOW_FB,
    OPTION_FBDEV,
    OPTION_COLOR_KEY,
    OPTION_SET_MCLK,
    OPTION_OVERCLOCK_MEM,
    OPTION_VIDEO_KEY,
    OPTION_ROTATE,
    OPTION_TEXTURED_VIDEO,
    OPTION_CRTC2HALF,
    OPTION_CRTC2RAM,
    OPTION_INT10,
    OPTION_AGP_MODE,
    OPTION_AGP_SIZE,
    OPTION_DIGITAL1,
    OPTION_DIGITAL2,
    OPTION_TV,
    OPTION_TVSTANDARD,
    OPTION_CABLETYPE,
    OPTION_USEIRQZERO,
    OPTION_NOHAL,
    OPTION_SWAPPED_HEAD,
    OPTION_DRI,
    OPTION_MERGEDFB,
    OPTION_HSYNC2,
    OPTION_VREFRESH2,
    OPTION_MONITOR2POS,
    OPTION_METAMODES,
    OPTION_OLDDMA,
    OPTION_PCIDMA,
    OPTION_ACCELMETHOD,
    OPTION_KVM
} MGAOpts;


#if !defined(EXTRADEBUG)
#define INREG8(addr) MMIO_IN8(pMga->IOBase, addr)
#define INREG16(addr) MMIO_IN16(pMga->IOBase, addr)
#define INREG(addr) MMIO_IN32(pMga->IOBase, addr)
#define OUTREG8(addr, val) MMIO_OUT8(pMga->IOBase, addr, val)
#define OUTREG16(addr, val) MMIO_OUT16(pMga->IOBase, addr, val)
#define OUTREG(addr, val) MMIO_OUT32(pMga->IOBase, addr, val)
#else /* !EXTRADEBUG */
CARD8 MGAdbg_inreg8(ScrnInfoPtr, int, int, char*);
CARD16 MGAdbg_inreg16(ScrnInfoPtr, int, int, char*);
CARD32 MGAdbg_inreg32(ScrnInfoPtr, int, int, char*);
void MGAdbg_outreg8(ScrnInfoPtr, int, int, char*);
void MGAdbg_outreg16(ScrnInfoPtr, int,int, char*);
void MGAdbg_outreg32(ScrnInfoPtr, int,int, char*);
#ifndef __GNUC__
# define MGA_STRINGIZE(x) #x
# define MGA_STRINGIFY(x) MGA_STRINGIZE(x)
# define __FUNCTION__ MGA_STRINGIFY(__FILE__) ", line " MGA_STRINGIFY(__LINE__)
#endif
#define INREG8(addr) MGAdbg_inreg8(pScrn, addr, 1, __FUNCTION__)
#define INREG16(addr) MGAdbg_inreg16(pScrn, addr, 1, __FUNCTION__)
#define INREG(addr) MGAdbg_inreg32(pScrn, addr, 1, __FUNCTION__)
#define OUTREG8(addr,val) MGAdbg_outreg8(pScrn, addr, val, __FUNCTION__)
#define OUTREG16(addr,val) MGAdbg_outreg16(pScrn, addr, val, __FUNCTION__)
#define OUTREG(addr,val) MGAdbg_outreg32(pScrn, addr, val, __FUNCTION__)
#endif /* EXTRADEBUG */

/*
 * PCI vendor/device ids, formerly in xf86PciInfo.h
 */

#define PCI_VENDOR_MATROX               0x102B

#define PCI_CHIP_MGA2085                0x0518
#define PCI_CHIP_MGA2064                0x0519
#define PCI_CHIP_MGA1064                0x051A
#define PCI_CHIP_MGA2164                0x051B
#define PCI_CHIP_MGA2164_AGP            0x051F

#define PCI_CHIP_MGAG100_PCI            0x1000
#define PCI_CHIP_MGAG100                0x1001
#define PCI_CHIP_MGAG200_PCI            0x0520
#define PCI_CHIP_MGAG200                0x0521
#define PCI_CHIP_MGAG200_SE_A_PCI       0x0522
#define PCI_CHIP_MGAG200_SE_B_PCI       0x0524
#define PCI_CHIP_MGAG200_WINBOND_PCI    0x0532
#define PCI_CHIP_MGAG200_EV_PCI         0x0530
#define PCI_CHIP_MGAG200_EH_PCI         0x0533
#define PCI_CHIP_MGAG200_ER_PCI         0x0534
#define PCI_CHIP_MGAG400                0x0525
#define PCI_CHIP_MGAG550                0x2527

/*
 * Read/write to the DAC via MMIO 
 */

/*
 * These were functions.  Use macros instead to avoid the need to
 * pass pMga to them.
 */

#define inMGAdreg(reg) INREG8(RAMDAC_OFFSET + (reg))

#define outMGAdreg(reg, val) OUTREG8(RAMDAC_OFFSET + (reg), val)

#define inMGAdac(reg) \
	(outMGAdreg(MGA1064_INDEX, reg), inMGAdreg(MGA1064_DATA))

#define outMGAdac(reg, val) \
	(outMGAdreg(MGA1064_INDEX, reg), outMGAdreg(MGA1064_DATA, val))

#define outMGAdacmsk(reg, mask, val) \
	do { /* note: mask and reg may get evaluated twice */ \
	    unsigned char tmp = (mask) ? (inMGAdac(reg) & (mask)) : 0; \
	    outMGAdreg(MGA1064_INDEX, reg); \
	    outMGAdreg(MGA1064_DATA, tmp | (val)); \
	} while (0)

#define MGAWAITVSYNC() \
    do { \
	unsigned int count = 0; \
    	unsigned int status = 0; \
	do { \
	    status = INREG( MGAREG_Status ); \
	    count++; \
    	} while( ( status & 0x08 ) && (count < 250000) );\
	count = 0; \
    	status = 0; \
	do { \
	    status = INREG( MGAREG_Status ); \
	    count++; \
    	} while( !( status & 0x08 ) && (count < 250000) );\
    } while (0)
    
#define MGAWAITBUSY() \
    do { \
    	unsigned int count = 0; \
	unsigned int status = 0; \
    	do { \
    	    status = INREG8( MGAREG_Status + 2 ); \
	    count++; \
    	} while( ( status & 0x01 ) && (count < 500000) ); \
    } while (0)

#define PORT_OFFSET 	(0x1F00 - 0x300)

#define MGA_VERSION 4000
#define MGA_NAME "MGA"
#define MGA_C_NAME MGA
#define MGA_MODULE_DATA mgaModuleData
#define MGA_DRIVER_NAME "mga"

typedef struct {
    unsigned char	ExtVga[6];
    unsigned char 	DacClk[6];
    unsigned char	ExtVga_Index24;
    unsigned char	Dac_Index90;
    unsigned char * DacRegs;
    unsigned long	crtc2[0x58];
    unsigned char	dac2[0x21];
    CARD32		Option;
    CARD32		Option2;
    CARD32		Option3;
    long                Clock;
    unsigned char	Pan_Ctl;
    Bool                PIXPLLCSaved;
    unsigned char       PllM;
    unsigned char       PllN;
    unsigned char       PllP;
} MGARegRec, *MGARegPtr;

/* For programming the second CRTC */
typedef struct {
   CARD32   ulDispWidth;        /* Display Width in pixels*/
   CARD32   ulDispHeight;       /* Display Height in pixels*/
   CARD32   ulBpp;              /* Bits Per Pixels / input format*/
   CARD32   ulPixClock;         /* Pixel Clock in kHz*/
   CARD32   ulHFPorch;          /* Horizontal front porch in pixels*/
   CARD32   ulHSync;            /* Horizontal Sync in pixels*/
   CARD32   ulHBPorch;          /* Horizontal back porch in pixels*/
   CARD32   ulVFPorch;          /* Vertical front porch in lines*/
   CARD32   ulVSync;            /* Vertical Sync in lines*/
   CARD32   ulVBPorch;          /* Vertical back Porch in lines*/
   CARD32   ulFBPitch;          /* Pitch*/
   CARD32   flSignalMode;       /* Signal Mode*/
} xMODEINFO;


typedef struct {
   int          brightness;
   int          contrast;
   Bool         doubleBuffer;
   unsigned char currentBuffer;
   RegionRec	clip;
   CARD32	colorKey;
   CARD32	videoStatus;
   Time		offTime;
   Time		freeTime;
   int		lastPort;

#ifdef USE_EXA
   int              size;
   ExaOffscreenArea *off_screen;
#endif

   void         *video_memory;
   int           video_offset;
} MGAPortPrivRec, *MGAPortPrivPtr;

typedef struct {
    Bool	isHwCursor;
    int		CursorMaxWidth;
    int 	CursorMaxHeight;
    int		CursorFlags;
    int		CursorOffscreenMemSize;
    Bool	(*UseHWCursor)(ScreenPtr, CursorPtr);
    void	(*LoadCursorImage)(ScrnInfoPtr, unsigned char*);
    void	(*ShowCursor)(ScrnInfoPtr);
    void	(*HideCursor)(ScrnInfoPtr);
    void	(*SetCursorPosition)(ScrnInfoPtr, int, int);
    void	(*SetCursorColors)(ScrnInfoPtr, int, int);
    long	maxPixelClock;
    long	MemoryClock;
    MessageType ClockFrom;
    MessageType MemClkFrom;
    Bool	SetMemClk;
    void	(*LoadPalette)(ScrnInfoPtr, int, int*, LOCO*, VisualPtr);
    void	(*RestorePalette)(ScrnInfoPtr, unsigned char *);
    void	(*PreInit)(ScrnInfoPtr);
    void	(*Save)(ScrnInfoPtr, vgaRegPtr, MGARegPtr, Bool);
    void	(*Restore)(ScrnInfoPtr, vgaRegPtr, MGARegPtr, Bool);
    Bool	(*ModeInit)(ScrnInfoPtr, DisplayModePtr);
} MGARamdacRec, *MGARamdacPtr;


typedef struct {
    int bitsPerPixel;
    int depth;
    int displayWidth;
    rgb weight;
    DisplayModePtr mode;
} MGAFBLayout;

/* Card-specific driver information */

typedef struct {
    Bool update;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} MGAPaletteInfo;

#define MGAPTR(p) ((MGAPtr)((p)->driverPrivate))

/*avoids segfault by returning false if pMgaHwInfo not defined*/
#define ISDIGITAL1(p) (p->pMgaHwInfo && ((p->pMgaHwInfo->ulCapsFirstOutput) & MGAHWINFOCAPS_OUTPUT_DIGITAL))
#define ISDIGITAL2(p) (p->pMgaHwInfo && ((p->pMgaHwInfo->ulCapsSecondOutput) & MGAHWINFOCAPS_OUTPUT_DIGITAL))
#define ISTV1(p) (p->pMgaHwInfo && ((p->pMgaHwInfo->ulCapsFirstOutput) & MGAHWINFOCAPS_OUTPUT_TV))
#define ISTV2(p) (p->pMgaHwInfo && ((p->pMgaHwInfo->ulCapsSecondOutput) & MGAHWINFOCAPS_OUTPUT_TV))



typedef enum {
    mgaLeftOf,
    mgaRightOf,
    mgaAbove,
    mgaBelow,
    mgaClone
} MgaScrn2Rel;

typedef struct {
    int			lastInstance;
    int			refCount;
    CARD32		masterFbAddress;
    long		masterFbMapSize;
    CARD32		slaveFbAddress;
    long		slaveFbMapSize;
    int			mastervideoRam;
    int			slavevideoRam;
    Bool		directRenderingEnabled;

    void *		mappedIOBase;
    int			mappedIOUsage;

    void *		mappedILOADBase;
    int			mappedILOADUsage;

    ScrnInfoPtr 	pScrn_1;
    ScrnInfoPtr 	pScrn_2;
} MGAEntRec, *MGAEntPtr;

/**
 * Track the range of a voltage controlled osciliator (VCO).
 */
struct mga_VCO {
    /**
     * Minimum selectable frequency for this VCO, measured in kHz.
     */
    unsigned min_freq;
    
    /**
     * Maximum selectable frequency for this VCO, measured in kHz.
     * 
     * If this value is zero, then the VCO is not available.
     */
    unsigned max_freq;
};

/**
 * Host interface types that can be set by the card's BIOS.
 */
typedef enum {
    MGA_HOST_UNKNOWN0 = 0,  /**< Meaning unknown. */
    MGA_HOST_UNKNOWN1 = 1,  /**< Meaning unknown. */
    MGA_HOST_UNKNOWN2 = 2,  /**< Meaning unknown. */
    MGA_HOST_HYBRID = 3,    /**< AGP 4x for data xfers only. */

    /**
     * PCI interface.  Either native or via a universal PCI-to-PCI bridge
     * chip.  The PCI G450 and PCI G550 cards are examples.
     */
    MGA_HOST_PCI = 4,

    MGA_HOST_AGP_1x = 5,    /**< AGP 1x capable. */
    MGA_HOST_AGP_2x = 6,    /**< AGP 2x capable. */
    MGA_HOST_AGP_4x = 7     /**< AGP 4x capable. */
} mga_host_t;

/**
 * Card information derrived from BIOS PInS data.
 */
struct mga_bios_values {
    /**
     * \name Voltage Controlled Oscilators
     * \brief Track information about the various VCOs.
     *
     * MGA cards have between one and three VCOs that can be used to drive the
     * various clocks.  On older cards, only \c mga_bios_values::pixel VCO is
     * available.  On newer cards, such as the G450 and G550, all three are
     * available.  If \c mga_VCO::max_freq is zero, the VCO is not available.
     */
    /*@{*/
    struct mga_VCO   system;    /**< System VCO. */
    struct mga_VCO   pixel;     /**< Pixel VCO. */
    struct mga_VCO   video;     /**< Video VCO. */
    /*@}*/
    
    /**
     * Memory clock speed, measured in kHz.
     */
    unsigned mem_clock;

    /**
     * PLL reference frequency value.  On older cards this is ~14MHz, and on
     * newer cards it is ~27MHz.
     */
    unsigned pll_ref_freq;

    /**
     * Some older MGA cards have a "fast bitblt" mode.  This is determined
     * by a capability bit stored in the PInS data.
     */
    Bool fast_bitblt;

    /**
     * Type of physical interface used for the card.
     */
    mga_host_t host_interface;
};


/**
 * Attributes that of an MGA device that can be derrived purely from its
 * PCI ID.
 */
struct mga_device_attributes {
    unsigned has_sdram:1;
    unsigned probe_for_sdram:1;
    unsigned dual_head_possible:1;
    unsigned fb_4mb_quirk:1;
    unsigned hwcursor_1064:1;

    unsigned dri_capable:1;
    unsigned dri_chipset:3;
    
    unsigned HAL_chipset:1;

    enum {
	old_BARs = 0,
	probe_BARs,
	new_BARs
    } BARs:2;
    
    uint32_t accel_flags;
    
    /** Default BIOS values. */
    struct mga_bios_values default_bios_values;
    
    /** Default memory probe offset / size values. */
    unsigned probe_size;
    unsigned probe_offset;
};

typedef struct {
    EntityInfoPtr	pEnt;
    struct mga_bios_values bios;
    CARD8               BiosOutputMode;
#ifdef XSERVER_LIBPCIACCESS
    struct pci_device *	PciInfo;
#else
    pciVideoPtr		PciInfo;
    PCITAG		PciTag;
    xf86AccessRec	Access;
#endif
    const struct mga_device_attributes * chip_attribs;
    int			Chipset;
    int                 ChipRev;

    int is_Gx50:1;
    int is_G200SE:1;
    int is_G200WB:1;
    int is_G200EV:1;
    int is_G200EH:1;
    int is_G200ER:1;

    int KVM;

    CARD32		reg_1e24;   /* model revision on g200se */

    Bool		Primary;
    Bool		Interleave;
    int			HwBpp;
    int			Roundings[4];
    int			BppShifts[4];
    Bool		HasFBitBlt;
    Bool		OverclockMem;
    int			YDstOrg;
    int			DstOrg;
    int			SrcOrg;

    /**
     * Which BAR corresponds to the framebuffer on this chip?
     */
    unsigned            framebuffer_bar;

    /**
     * Which BAR corresponds to IO space on this chip?
     */
    unsigned            io_bar;

    /**
     * Which BAR corresponds to ILOAD space on this chip?  If the value is
     * -1, then this chip does not have an ILOAD region.
     */
    int                 iload_bar;

#ifndef XSERVER_LIBPCIACCESS
    unsigned long	IOAddress;
    unsigned long	ILOADAddress;
    unsigned long	BiosAddress;
    MessageType		BiosFrom;
#endif
    unsigned long	FbAddress;
    unsigned char *     IOBase;
    unsigned char *	FbBase;
    unsigned char *	ILOADBase;
    unsigned char *	FbStart;
    long		FbMapSize;
    long		FbUsableSize;
    long		FbCursorOffset;
    MGARamdacRec	Dac;
    Bool		HasSDRAM;
    Bool		NoAccel;
    Bool		Exa;
    ExaDriverPtr 	ExaDriver;
    Bool		SyncOnGreen;
    Bool		HWCursor;
    Bool		UsePCIRetry;
    Bool		ShowCache;
    Bool		ShadowFB;
    unsigned char *	ShadowPtr;
    int			ShadowPitch;
    int			MemClk;
    int			MinClock;
    int			MaxClock;
    MGARegRec		SavedReg;
    MGARegRec		ModeReg;
    int			MaxFastBlitY;
    CARD32		BltScanDirection;
    CARD32		FilledRectCMD;
    CARD32		SolidLineCMD;
    CARD32		PatternRectCMD;
    CARD32		DashCMD;
    CARD32		NiceDashCMD;
    CARD32		AccelFlags;
    CARD32		PlaneMask;
    CARD32		FgColor;
    CARD32		BgColor;
    CARD32		MAccess;
    int			FifoSize;
    int			StyleLen;
#ifdef HAVE_XAA_H
    XAAInfoRecPtr	AccelInfoRec;
#endif
    xf86CursorInfoPtr	CursorInfoRec;
    DGAModePtr		DGAModes;
    int			numDGAModes;
    Bool		DGAactive;
    int			DGAViewportStatus;
    CARD32		*Atype;
    CARD32		*AtypeNoBLK;
    void		(*PreInit)(ScrnInfoPtr pScrn);
    void		(*Save)(ScrnInfoPtr, vgaRegPtr, MGARegPtr, Bool);
    void		(*Restore)(ScrnInfoPtr, vgaRegPtr, MGARegPtr, Bool);
    Bool		(*ModeInit)(ScrnInfoPtr, DisplayModePtr);
    void		(*PointerMoved)(SCRN_ARG_TYPE arg, int x, int y);
    CloseScreenProcPtr	CloseScreen;
    ScreenBlockHandlerProcPtr BlockHandler;
    unsigned int	(*ddc1Read)(ScrnInfoPtr);
    void (*DDC1SetSpeed)(ScrnInfoPtr, xf86ddcSpeed);
    Bool		(*i2cInit)(ScrnInfoPtr);
    I2CBusPtr		DDC_Bus1;
    I2CBusPtr		DDC_Bus2;
    I2CBusPtr		Maven_Bus;
    I2CDevPtr		Maven;
    char		Maven_Version;
    Bool		UseMaven;
    Bool		UseMavenPM;
    Bool		FBDev;
    int			colorKey;
    int			videoKey;
    int			fifoCount;
    int			Rotate;
    MGAFBLayout		CurrentLayout;
    Bool		DrawTransparent;
    int			MaxBlitDWORDS;
    Bool		TexturedVideo;
    MGAPortPrivPtr	portPrivate;
    unsigned char	*ScratchBuffer;
    unsigned char	*ColorExpandBase;
    int			expandRows;
    int			expandDWORDs;
    int			expandRemaining;
    int			expandHeight;
    int			expandY;
#ifdef MGADRI
    Bool 		directRenderingEnabled;
    DRIInfoPtr 		pDRIInfo;
    int 		drmFD;
    int 		numVisualConfigs;
    __GLXvisualConfig*	pVisualConfigs;
    MGAConfigPrivPtr 	pVisualConfigsPriv;
    MGADRIServerPrivatePtr DRIServerInfo;

    MGARegRec		DRContextRegs;

    Bool		haveQuiescense;
    void		(*GetQuiescence)(ScrnInfoPtr pScrn);

    int 		agpMode;
    int                 agpSize;

    int                 irq;
    CARD32              reg_ien;
    
    Bool                useOldDmaInit;
    Bool                forcePciDma;
#endif
    XF86VideoAdaptorPtr adaptor;
    Bool		DualHeadEnabled;
    Bool		Crtc2IsTV;
    Bool		SecondCrtc;
    Bool                SecondOutput;

    GDevPtr		device;
    /* The hardware's real SrcOrg */
    int			realSrcOrg;
    MGAEntPtr		entityPrivate;
    void		(*SetupForSolidFill)(ScrnInfoPtr pScrn, int color,
					     int rop, unsigned int planemask);
    void		(*SubsequentSolidFillRect)(ScrnInfoPtr pScrn,
					     int x, int y, int w, int h);
    void		(*RestoreAccelState)(ScrnInfoPtr pScrn);
    int			allowedWidth;
    void		(*VideoTimerCallback)(ScrnInfoPtr, Time);
    void		(*PaletteLoadCallback)(ScrnInfoPtr);
    void		(*RenderCallback)(ScrnInfoPtr);
    Time		RenderTime;
    MGAPaletteInfo	palinfo[256];  /* G400 hardware bug workaround */
    FBLinearPtr		LinearScratch;
    Bool                softbooted;
    OptionInfoPtr	Options;

    /* Exa */
    PicturePtr currentSrcPicture;
    PicturePtr currentMaskPicture;
    PixmapPtr currentSrc;
    PixmapPtr currentMask;
    int src_w2;
    int src_h2;
    int mask_w2;
    int mask_h2;
    CARD32 src_pitch; /* FIXME kill me */

/* Merged Framebuffer data */
    Bool                MergedFB;

    /* Real values specific to monitor1, since the original ones are replaced */
    DisplayModePtr	M1modes;	 /* list of actual modes */
    DisplayModePtr	M1currentMode; /* current mode */
    int			M1frameX0;	/* viewport position */
    int			M1frameY0;
    int			M1frameX1;
    int			M1frameY1;
    
    ScrnInfoPtr       pScrn2; /*pointer to second CRTC screeninforec,
                                       if in merged mode */
/* End of Merged Framebuffer Data */
  int			HALGranularityOffX, HALGranularityOffY;
} MGARec, *MGAPtr;

extern CARD32 MGAAtype[16];
extern CARD32 MGAAtypeNoBLK[16];

#define USE_RECTS_FOR_LINES	0x00000001
#define FASTBLT_BUG		0x00000002
#define CLIPPER_ON		0x00000004
#define BLK_OPAQUE_EXPANSION	0x00000008
#define TRANSC_SOLID_FILL	0x00000010
#define	NICE_DASH_PATTERN	0x00000020
#define	TWO_PASS_COLOR_EXPAND	0x00000040
#define	MGA_NO_PLANEMASK	0x00000080
/* linear expansion doesn't work on BE due to wrong byte order */
#if X_BYTE_ORDER == X_BIG_ENDIAN
#define USE_LINEAR_EXPANSION	0x00000000
#else
#define USE_LINEAR_EXPANSION	0x00000100
#endif
#define LARGE_ADDRESSES		0x00000200

#define MGAIOMAPSIZE		0x00004000
#define MGAILOADMAPSIZE		0x00400000

#define TRANSPARENCY_KEY	255
#define KEY_COLOR		0


/* Prototypes */

void MGAAdjustFrame(ADJUST_FRAME_ARGS_DECL);
Bool MGASwitchMode(SWITCH_MODE_ARGS_DECL);
void MGAFillModeInfoStruct(ScrnInfoPtr pScrn, DisplayModePtr mode);
Bool MGAGetRec(ScrnInfoPtr pScrn);
void MGAProbeDDC(ScrnInfoPtr pScrn, int index);
void MGASoftReset(ScrnInfoPtr pScrn);
void MGAFreeRec(ScrnInfoPtr pScrn);
Bool mga_read_and_process_bios(ScrnInfoPtr pScrn);
void MGADisplayPowerManagementSet(ScrnInfoPtr pScrn, int PowerManagementMode,
				  int flags);
void MGAAdjustFrameCrtc2(ADJUST_FRAME_ARGS_DECL);
void MGADisplayPowerManagementSetCrtc2(ScrnInfoPtr pScrn,
					     int PowerManagementMode,
					     int flags);
void MGAAdjustGranularity(ScrnInfoPtr pScrn, int* x, int* y); 


void MGA2064SetupFuncs(ScrnInfoPtr pScrn);
void MGAGSetupFuncs(ScrnInfoPtr pScrn);

/*#ifdef USE_XAA */
void MGAStormSync(ScrnInfoPtr pScrn);
void MGAStormEngineInit(ScrnInfoPtr pScrn);
Bool MGAStormAccelInit(ScreenPtr pScreen);
Bool mgaAccelInit(ScreenPtr pScreen);
/* #endif */

#ifdef USE_EXA
Bool mgaExaInit(ScreenPtr pScreen);
#endif

Bool MGAHWCursorInit(ScreenPtr pScreen);


void MGAPolyArcThinSolid(DrawablePtr, GCPtr, int, xArc*);

Bool MGADGAInit(ScreenPtr pScreen);

void MGARefreshArea(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void MGARefreshArea8(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void MGARefreshArea16(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void MGARefreshArea24(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void MGARefreshArea32(ScrnInfoPtr pScrn, int num, BoxPtr pbox);

void mgaDoSetupForScreenToScreenCopy( ScrnInfoPtr pScrn, int xdir,
    int ydir, int rop, unsigned int planemask, int trans, unsigned int bpp );

void mgaDoSetupForSolidFill( ScrnInfoPtr pScrn, int color, int rop,
    unsigned int planemask, unsigned int bpp );

void MGAPointerMoved(SCRN_ARG_TYPE arg, int x, int y);

void MGAInitVideo(ScreenPtr pScreen);
void MGAResetVideo(ScrnInfoPtr pScrn);

#ifdef MGADRI

#define MGA_FRONT	0x1
#define MGA_BACK	0x2
#define MGA_DEPTH	0x4

Bool MGADRIScreenInit( ScreenPtr pScreen );
void MGADRICloseScreen( ScreenPtr pScreen );
Bool MGADRIFinishScreenInit( ScreenPtr pScreen );

Bool MGALockUpdate( ScrnInfoPtr pScrn, drmLockFlags flags );

void MGAGetQuiescence( ScrnInfoPtr pScrn );
void MGAGetQuiescenceShared( ScrnInfoPtr pScrn );

void MGASelectBuffer(ScrnInfoPtr pScrn, int which);
Bool MgaCleanupDma(ScrnInfoPtr pScrn);
Bool MgaInitDma(ScrnInfoPtr pScrn, int prim_size);

#define MGA_AGP_1X_MODE		0x01
#define MGA_AGP_2X_MODE		0x02
#define MGA_AGP_4X_MODE		0x04
#define MGA_AGP_MODE_MASK	0x07

#endif

Bool MGAMavenRead(ScrnInfoPtr pScrn, I2CByte reg, I2CByte *val);

void MGACRTC2Set(ScrnInfoPtr pScrn, xMODEINFO *pModeInfo);
void MGAEnableSecondOutPut(ScrnInfoPtr pScrn, xMODEINFO *pModeInfo);
void MGACRTC2SetPitch(ScrnInfoPtr pSrcn, xMODEINFO *pModeInfo);
void MGACRTC2SetDisplayStart(ScrnInfoPtr pScrn, xMODEINFO *pModeInfo, CARD32 base, CARD32 ulX, CARD32 ulY);

void MGACRTC2Get(ScrnInfoPtr pScrn, xMODEINFO *pModeInfo);
void MGACRTC2GetPitch(ScrnInfoPtr pSrcn, xMODEINFO *pModeInfo);
void MGACRTC2GetDisplayStart(ScrnInfoPtr pScrn, xMODEINFO *pModeInfo, CARD32 base, CARD32 ulX, CARD32 ulY);

double MGAG450SetPLLFreq(ScrnInfoPtr pScrn, long f_out);
#ifdef DEBUG
void MGAG450PrintPLL(ScrnInfoPtr pScrn);
#endif
long MGAG450SavePLLFreq(ScrnInfoPtr pScrn);
void MGAprintDac(ScrnInfoPtr pScrn);
void MGAG200SESaveFonts(ScrnInfoPtr, vgaRegPtr);
void MGAG200SERestoreFonts(ScrnInfoPtr, vgaRegPtr);
void MGAG200SESaveMode(ScrnInfoPtr, vgaRegPtr);
void MGAG200SERestoreMode(ScrnInfoPtr, vgaRegPtr);
void MGAG200SEHWProtect(ScrnInfoPtr, Bool);

static __inline__ void
MGA_MARK_SYNC(MGAPtr pMga, ScrnInfoPtr pScrn)
{
#ifdef USE_EXA
    if (pMga->Exa)
        exaMarkSync(pScrn->pScreen);
#endif
#ifdef USE_XAA
    if (!pMga->Exa)
        SET_SYNC_FLAG(pMga->AccelInfoRec);
#endif
}

static __inline__ void
MGA_SYNC(MGAPtr pMga, ScrnInfoPtr pScrn)
{
#ifdef USE_EXA
    if (pMga->Exa)
        exaWaitSync(pScrn->pScreen);
#endif
#ifdef USE_XAA
    if (!pMga->Exa && pMga->AccelInfoRec && pMga->AccelInfoRec->NeedToSync)
        pMga->AccelInfoRec->Sync(pScrn);
#endif
}

#endif
