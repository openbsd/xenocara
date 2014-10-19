/*
 * Copyright (C) 1994-2000 The XFree86 Project, Inc.  All Rights Reserved.
 * Copyright (c) 2003-2006, X.Org Foundation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */

#ifndef SAVAGE_DRIVER_H
#define SAVAGE_DRIVER_H

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef XSERVER_LIBPCIACCESS
#include <pciaccess.h>
#define VENDOR_ID(p)      (p)->vendor_id
#define DEVICE_ID(p)      (p)->device_id
#define SUBSYS_ID(p)      (p)->subdevice_id
#define CHIP_REVISION(p)  (p)->revision
#else
#define VENDOR_ID(p)      (p)->vendor
#define DEVICE_ID(p)      (p)->chipType
#define SUBSYS_ID(p)      (p)->subsysCard
#define CHIP_REVISION(p)  (p)->chipRev
#endif

#define MODE_24 24

#include "compiler.h"
#include "vgaHW.h"
#include "xf86.h"
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86Resources.h"
#endif
#include "xf86Modes.h"
#include "xf86Pci.h"
#include "xf86_OSproc.h"
#include "xf86Cursor.h"
#include "mipointer.h"
#include "micmap.h"
#include "fb.h"
#include "fboverlay.h"
#include "xf86cmap.h"
#include "vbe.h"
#ifdef HAVE_XAA_H
#include "xaa.h"
#endif
#include "xf86fbman.h"
#include "exa.h"
#include "xf86xv.h"

#include "savage_regs.h"
#include "savage_vbe.h"

#ifndef XF86DRI
#undef SAVAGEDRI
#endif

#ifdef SAVAGEDRI
#define _XF86DRI_SERVER_
#include "savage_dripriv.h"
#include "savage_dri.h"
#include "dri.h"
#include "GL/glxint.h"
#include "xf86drm.h"

/* Totals 2 Mbytes which equals 2^16 32-byte vertices divided among up
 * to 32 clients. */
#define SAVAGE_NUM_BUFFERS 32
#define SAVAGE_BUFFER_SIZE (1 << 16) /* 64k */

#define SAVAGE_CMDDMA_SIZE 0x100000 /* 1MB */

#define SAVAGE_DEFAULT_AGP_MODE     1
#define SAVAGE_MAX_AGP_MODE         4

/* Buffer are aligned on 4096 byte boundaries.
 */
/*  this is used for backbuffer, depthbuffer, etc..*/
/*          alignment                                      */

#define SAVAGE_BUFFER_ALIGN	0x00000fff

typedef struct _server{
   int reserved_map_agpstart;
   int reserved_map_idx;

   int sarea_priv_offset;

   int chipset;
   int sgram;     /* seems no use */

   unsigned int frontOffset;
   unsigned int frontPitch;
   unsigned int frontbufferSize;
   unsigned int frontBitmapDesc;
   
   unsigned int backOffset;
   unsigned int backPitch;
   unsigned int backbufferSize;
   unsigned int backBitmapDesc;

   unsigned int depthOffset;
   unsigned int depthPitch;
   unsigned int depthbufferSize;
   unsigned int depthBitmapDesc;

   unsigned int textureOffset;
   int textureSize;
   int logTextureGranularity;

   drmRegion agp;

   /* PCI mappings */
   drmRegion aperture;
   drmRegion registers;
   drmRegion status;

   /* AGP mappings */
   drmRegion buffers;
   drmRegion agpTextures;
   int logAgpTextureGranularity;

   /* command DMA */
   drmRegion cmdDma;

   /* XVideo through AGP */
   drmRegion agpXVideo;
} SAVAGEDRIServerPrivateRec, *SAVAGEDRIServerPrivatePtr;

#endif

#include "compat-api.h"

typedef enum {
    MT_NONE,
    MT_CRT,
    MT_LCD,
    MT_DFP,
    MT_TV
} SavageMonitorType;

typedef struct
{
    Bool HasSecondary;
    Bool TvOn;
    ScrnInfoPtr pSecondaryScrn;
    ScrnInfoPtr pPrimaryScrn;
  
} SavageEntRec, *SavageEntPtr;

#define VGAIN8(addr) MMIO_IN8(psav->MapBase+0x8000, addr)
#define VGAIN16(addr) MMIO_IN16(psav->MapBase+0x8000, addr)
#define VGAIN(addr) MMIO_IN32(psav->MapBase+0x8000, addr)
 
#define VGAOUT8(addr,val) MMIO_OUT8(psav->MapBase+0x8000, addr, val)
#define VGAOUT16(addr,val) MMIO_OUT16(psav->MapBase+0x8000, addr, val)
#define VGAOUT(addr,val) MMIO_OUT32(psav->MapBase+0x8000, addr, val)

#define INREG8(addr) MMIO_IN8(psav->MapBase, addr)
#define INREG16(addr) MMIO_IN16(psav->MapBase, addr)
#define INREG32(addr) MMIO_IN32(psav->MapBase, addr)
#define OUTREG8(addr,val) MMIO_OUT8(psav->MapBase, addr, val)
#define OUTREG16(addr,val) MMIO_OUT16(psav->MapBase, addr, val)
#define OUTREG32(addr,val) MMIO_OUT32(psav->MapBase, addr, val)
#define INREG(addr) INREG32(addr) 
#define OUTREG(addr,val) OUTREG32(addr,val) 

#if X_BYTE_ORDER == X_LITTLE_ENDIAN
#define B_O16(x)  (x)
#define B_O32(x)  (x)
#else
#define B_O16(x)  ((((x) & 0xff) << 8) | (((x) & 0xff) >> 8))
#define B_O32(x)  ((((x) & 0xff) << 24) | (((x) & 0xff00) << 8) \
                  | (((x) & 0xff0000) >> 8) | (((x) & 0xff000000) >> 24))
#endif
#define L_ADD(x)  (B_O32(x) & 0xffff) + ((B_O32(x) >> 12) & 0xffff00)

#define SAVAGEIOMAPSIZE	0x80000

#define SAVAGE_CRT_ON	1
#define SAVAGE_LCD_ON	2
#define SAVAGE_TV_ON	4

#define SAVAGE_DRIVER_NAME	"savage"
#define SAVAGE_DRIVER_VERSION	PACKAGE_VERSION
#define SAVAGE_VERSION_MAJOR	PACKAGE_VERSION_MAJOR
#define SAVAGE_VERSION_MINOR	PACKAGE_VERSION_MINOR
#define SAVAGE_PATCHLEVEL	PACKAGE_VERSION_PATCHLEVEL
#define SAVAGE_VERSION	((SAVAGE_VERSION_MAJOR << 24) | \
			 (SAVAGE_VERSION_MINOR << 16) | \
			 SAVAGE_PATCHLEVEL)

typedef struct _S3VMODEENTRY {
   unsigned short Width;
   unsigned short Height;
   unsigned short VesaMode;
   unsigned char RefreshCount;
   unsigned char * RefreshRate;
} SavageModeEntry, *SavageModeEntryPtr;


typedef struct _S3VMODETABLE {
   unsigned short NumModes;
   SavageModeEntry Modes[1];
} SavageModeTableRec, *SavageModeTablePtr;


typedef struct {
    unsigned int mode, refresh;
    unsigned char SR08, SR0E, SR0F;
    unsigned char SR10, SR11, SR12, SR13, SR15, SR18, SR1B, SR29, SR30;
    unsigned char SR54[8];
    unsigned char Clock;
    unsigned char CR31, CR32, CR33, CR34, CR36, CR3A, CR3B, CR3C;
    unsigned char CR40, CR41, CR42, CR43, CR45;
    unsigned char CR50, CR51, CR53, CR55, CR58, CR5B, CR5D, CR5E;
    unsigned char CR60, CR63, CR65, CR66, CR67, CR68, CR69, CR6D, CR6F;
    unsigned char CR86, CR88;
    unsigned char CR90, CR91, CRB0;
    unsigned int  STREAMS[22];	/* yuck, streams regs */
    unsigned int  MMPR0, MMPR1, MMPR2, MMPR3;
} SavageRegRec, *SavageRegPtr;

typedef  struct {
    CARD32 redMask, greenMask, blueMask;
    int redShift, greenShift, blueShift;
} savageOverlayRec;

/*  Tiling defines */
#define TILE_SIZE_BYTE          2048   /* 0x800, 2K */
#define TILE_SIZE_BYTE_2000     4096

#define TILEHEIGHT_16BPP        16
#define TILEHEIGHT_32BPP        16
#define TILEHEIGHT              16      /* all 16 and 32bpp tiles are 16 lines high */
#define TILEHEIGHT_2000         32      /* 32 lines on savage 2000 */

#define TILEWIDTH_BYTES         128     /* 2048/TILEHEIGHT (** not for use w/8bpp tiling) */
#define TILEWIDTH8BPP_BYTES     64      /* 2048/TILEHEIGHT_8BPP */
#define TILEWIDTH_16BPP         64      /* TILEWIDTH_BYTES/2-BYTES-PER-PIXEL */
#define TILEWIDTH_32BPP         32      /* TILEWIDTH_BYTES/4-BYTES-PER-PIXEL */

/* Bitmap descriptor structures for BCI */
typedef struct _HIGH {
    unsigned short Stride;
    unsigned char Bpp;
    unsigned char ResBWTile;
} HIGH;

typedef struct _BMPDESC1 {
    unsigned long Offset;
    HIGH  HighPart;
} BMPDESC1;

typedef struct _BMPDESC2 {
    unsigned long LoPart;
    unsigned long HiPart;
} BMPDESC2;

typedef union _BMPDESC {
    BMPDESC1 bd1;
    BMPDESC2 bd2;
} BMPDESC;

typedef struct _StatInfo {
    int     origMode;
    int     pageCnt;    
    pointer statBuf;
    int     realSeg;    
    int     realOff;
} StatInfoRec,*StatInfoPtr;

struct savage_region {
#ifdef XSERVER_LIBPCIACCESS
    pciaddr_t       base;
    pciaddr_t       size;
#else
    unsigned long   base;
    unsigned long   size;
#endif
    void          * memory;
};

typedef struct _Savage {
    SavageRegRec	SavedReg;
    SavageRegRec	ModeReg;
    xf86CursorInfoPtr	CursorInfoRec;
    Bool		ModeStructInit;
    Bool		NeedSTREAMS;
    Bool		STREAMSRunning;
    int			Bpp, Bpl, ScissB;
    unsigned		PlaneMask;
    I2CBusPtr		I2C;
    I2CBusPtr		DVI;
    unsigned char       DDCPort;
    unsigned char       I2CPort;

    int			videoRambytes;
    int			videoRamKbytes;
    int			MemOffScreen;
    int			CursorKByte;
    int			endfb;

    /* These are physical addresses. */
    unsigned long	ShadowPhysical;

    /* These are linear addresses. */
    struct savage_region   MmioRegion;
    struct savage_region   FbRegion;
    struct savage_region   ApertureRegion;

    unsigned char*	MapBase;
    unsigned char*	BciMem;
    unsigned char*	FBBase;
    unsigned char*	ApertureMap;
    unsigned char*	FBStart;
    CARD32 volatile *	ShadowVirtual;

    Bool		PrimaryVidMapped;
    int			maxClock;
    int			HorizScaleFactor;
    int			MCLK, REFCLK, LCDclk;
    double		refclk_fact;
    int			GEResetCnt;

    /* Here are all the Options */

    OptionInfoPtr	Options;
    Bool		IgnoreEDID;
    Bool		ShowCache;
    Bool		pci_burst;
    Bool		NoPCIRetry;
    Bool		fifo_conservative;
    Bool		fifo_moderate;
    Bool		fifo_aggressive;
    Bool		hwcursor;
    Bool		hwc_on;
    Bool		NoAccel;
    Bool		shadowFB;
    Bool		UseBIOS;
    int			rotate;
    double		LCDClock;
    Bool		ConfigShadowStatus; /* from the config */
    Bool		ShadowStatus;       /* automatically enabled with DRI */
    Bool		ForceShadowStatus;  /* true if explicitly set in conf */
    Bool		CrtOnly;
    Bool		TvOn;
    Bool		PAL;
    Bool		ForceInit;
    int			iDevInfo;
    int			iDevInfoPrim;

    Bool		FPExpansion;
    int			PanelX;		/* panel width */
    int			PanelY;		/* panel height */
    int			iResX;		/* crtc X display */
    int			iResY;		/* crtc Y display */
    int			XFactor;	/* overlay X factor */
    int			YFactor;	/* overlay Y factor */
    int			displayXoffset;	/* overlay X offset */
    int			displayYoffset;	/* overlay Y offset */
    int			XExp1;		/* expansion ratio in x */
    int			XExp2;
    int			YExp1;		/* expansion ratio in x */
    int			YExp2;
    int			cxScreen;
    int			TVSizeX;
    int			TVSizeY;

    CloseScreenProcPtr	CloseScreen;
#ifdef XSERVER_LIBPCIACCESS
    struct pci_device * PciInfo;
#else
    pciVideoPtr		PciInfo;
    PCITAG		PciTag;
#endif
    int			Chipset;
    int			ChipId;
    int			ChipRev;
    vbeInfoPtr		pVbe;
    int			EntityIndex;
    int			ShadowCounter;
    int			vgaIOBase;	/* 3b0 or 3d0 */

    /* The various Savage wait handlers. */
    int			(*WaitQueue)(struct _Savage *, int);
    int			(*WaitIdle)(struct _Savage *);
    int			(*WaitIdleEmpty)(struct _Savage *);

    /* Support for shadowFB and rotation */
    unsigned char *	ShadowPtr;
    int			ShadowPitch;
    void		(*PointerMoved)(SCRN_ARG_TYPE arg, int x, int y);

    /* support for EXA */
    ExaDriverPtr        EXADriverPtr;
    Bool		useEXA;
    unsigned long	EXAendfb;
    unsigned long 	pbd_offset;
    unsigned long	sbd_offset;
    unsigned long	pbd_high;
    unsigned long	sbd_high;

    /* Support for XAA acceleration */
#ifdef HAVE_XAA_H
    XAAInfoRecPtr	AccelInfoRec;
#endif
    xRectangle		Rect;
    unsigned int	SavedBciCmd;
    unsigned int	SavedFgColor;
    unsigned int	SavedBgColor;
    unsigned int	SavedSbdOffset;
    unsigned int	SavedSbd;

    SavageModeTablePtr	ModeTable;

    /* Support for the Savage command overflow buffer. */
    unsigned long	cobIndex;	/* size index */
    unsigned long	cobSize;	/* size in bytes */
    unsigned long	cobOffset;	/* offset in frame buffer */
    unsigned long       bciThresholdLo; /* low and high thresholds for */
    unsigned long       bciThresholdHi; /* shadow status update (32bit words) */
    unsigned long	bciUsedMask;	/* BCI entries used mask */
    unsigned int	eventStatusReg; /* Status register index that holds
					 * event counter 0. */

    /* Support for DGA */
    int			numDGAModes;
    DGAModePtr		DGAModes;
    Bool		DGAactive;
    int			DGAViewportStatus;

    /* Support for XVideo */

    unsigned int	videoFlags;
    unsigned int	blendBase;
    int			videoFourCC;
    XF86VideoAdaptorPtr	adaptor;
    int			VideoZoomMax;
    int			dwBCIWait2DIdle;
    XF86OffscreenImagePtr offscreenImages;

    /* Support for Overlays */
     unsigned char *	FBStart2nd;
     savageOverlayRec	overlay;
     int                 overlayDepth;
     int			primStreamBpp;

#ifdef SAVAGEDRI
    int 		LockHeld;
    Bool 		directRenderingEnabled;
    DRIInfoPtr 		pDRIInfo;
    int 		drmFD;
    int 		numVisualConfigs;
    __GLXvisualConfig*	pVisualConfigs;
    SAVAGEConfigPrivPtr 	pVisualConfigsPriv;
    SAVAGEDRIServerPrivatePtr DRIServerInfo;
    ScreenWakeupHandlerProcPtr coreWakeupHandler;
    ScreenBlockHandlerProcPtr  coreBlockHandler;

#if 0
    Bool		haveQuiescense;
    void		(*GetQuiescence)(ScrnInfoPtr pScrn);
#endif

    Bool		IsPCI;
    Bool		AgpDMA;
    Bool		VertexDMA;
    Bool		CommandDMA;
    int 		agpMode;
    drmSize		agpSize;
    FBLinearPtr		reserved;
    
    unsigned int surfaceAllocation[7];
    unsigned int xvmcContext;
    unsigned int DRIrunning;
    unsigned int hwmcOffset;
    unsigned int hwmcSize;

    Bool bDisableXvMC;

    Bool AGPforXv;
#endif

    Bool disableCOB;
    Bool BCIforXv;

    /* Bitmap Descriptors for BCI */
    BMPDESC GlobalBD;
    BMPDESC PrimaryBD;
    BMPDESC SecondBD;
    /* do we disable tile mode by option? */
    Bool bDisableTile;
    /* if we enable tile,we only support tile under 16/32bpp */
    Bool bTiled;
    int  lDelta;
    int  ulAperturePitch; /* aperture pitch */

    /*
     * cxMemory is number of pixels across screen width
     * cyMemory is number of scanlines in available adapter memory.
     *
     * cxMemory * cyMemory is used to determine how much memory to
     * allocate to our heap manager.  So make sure that any space at the
     * end of video memory set aside at bInitializeHardware time is kept
     * out of the cyMemory calculation.
     */
    int cxMemory,cyMemory;
    
    StatInfoRec     StatInfo; /* save the SVGA state */

    /* for dvi option */
    Bool  dvi;

    SavageMonitorType   DisplayType;
    /* DuoView stuff */
    Bool		HasCRTC2;     /* MX, IX, Supersavage */
    Bool		IsSecondary;  /* second Screen */	
    Bool		IsPrimary;  /* first Screen */
    EntityInfoPtr       pEnt;

} SavageRec, *SavagePtr;

/* Video flags. */

#define VF_STREAMS_ON	0x0001

#define SAVPTR(p)	((SavagePtr)((p)->driverPrivate))

/* Make the names of these externals driver-unique */
#define gpScrn savagegpScrn
#define readdw savagereaddw
#define readfb savagereadfb
#define writedw savagewritedw
#define writefb savagewritefb
#define writescan savagewritescan

/* add for support DRI */
#ifdef SAVAGEDRI

#define SAVAGE_FRONT	0x1
#define SAVAGE_BACK	0x2
#define SAVAGE_DEPTH	0x4
#define SAVAGE_STENCIL	0x8

Bool SAVAGEDRIScreenInit( ScreenPtr pScreen );
Bool SAVAGEInitMC(ScreenPtr pScreen);
void SAVAGEDRICloseScreen( ScreenPtr pScreen );
Bool SAVAGEDRIFinishScreenInit( ScreenPtr pScreen );
void SAVAGEDRIResume( ScreenPtr pScreen );

#define SAVAGE_AGP_1X_MODE		0x01
#define SAVAGE_AGP_2X_MODE		0x02
#define SAVAGE_AGP_4X_MODE		0x04
#define SAVAGE_AGP_MODE_MASK	0x07

#endif


/* Prototypes. */

extern void SavageCommonCalcClock(long freq, int min_m, int min_n1,
			int max_n1, int min_n2, int max_n2,
			long freq_min, long freq_max,
			unsigned char *mdiv, unsigned char *ndiv);
void SavageAdjustFrame(ADJUST_FRAME_ARGS_DECL);
void SavageDoAdjustFrame(ScrnInfoPtr pScrn, int y, int x, int crtc2);
Bool SavageSwitchMode(SWITCH_MODE_ARGS_DECL);

/* In savage_cursor.c. */

Bool SavageHWCursorInit(ScreenPtr pScreen);
void SavageShowCursor(ScrnInfoPtr);
void SavageHideCursor(ScrnInfoPtr);

/* In savage_accel.c. */

Bool SavageInitAccel(ScreenPtr);
void SavageInitialize2DEngine(ScrnInfoPtr);
void SavageSetGBD(ScrnInfoPtr);
void SavageAccelSync(ScrnInfoPtr);
/*int SavageHelpSolidROP(ScrnInfoPtr pScrn, int *fg, int pm, int *rop);*/

/* XAA and EXA */
Bool SavageXAAInit(ScreenPtr);
Bool SavageEXAInit(ScreenPtr);

/* In savage_i2c.c. */

Bool SavageI2CInit(ScrnInfoPtr pScrn);

/* In savage_shadow.c */

void SavagePointerMoved(SCRN_ARG_TYPE arg, int x, int y);
void SavageRefreshArea(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void SavageRefreshArea8(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void SavageRefreshArea16(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void SavageRefreshArea24(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void SavageRefreshArea32(ScrnInfoPtr pScrn, int num, BoxPtr pbox);

/* In savage_vbe.c */

void SavageSetTextMode( SavagePtr psav );
void SavageSetVESAMode( SavagePtr psav, int n, int Refresh );
void SavageSetPanelEnabled( SavagePtr psav, Bool active );
void SavageFreeBIOSModeTable( SavagePtr psav, SavageModeTablePtr* ppTable );
SavageModeTablePtr SavageGetBIOSModeTable( SavagePtr psav, int iDepth );
ModeStatus SavageMatchBiosMode(ScrnInfoPtr pScrn,int width,int height,int refresh,
                              unsigned int *vesaMode,unsigned int *newRefresh);

unsigned short SavageGetBIOSModes( 
    SavagePtr psav,
    VbeInfoBlock *vbe,
    int iDepth,
    SavageModeEntryPtr s3vModeTable );

/* In savage_video.c */

void SavageInitVideo( ScreenPtr pScreen );

/* In savage_streams.c */

void SavageStreamsOn(ScrnInfoPtr pScrn);
void SavageStreamsOff(ScrnInfoPtr pScrn);
void SavageInitSecondaryStream(ScrnInfoPtr pScrn);
void SavageInitStreamsOld(ScrnInfoPtr pScrn);
void SavageInitStreamsNew(ScrnInfoPtr pScrn);
void SavageInitStreams2000(ScrnInfoPtr pScrn);


#if (MODE_24 == 32)
# define  BYTES_PP24 4
#else
# define BYTES_PP24 3
#endif


#define DEPTH_BPP(depth) (depth == 24 ? (BYTES_PP24 << 3) : (depth + 7) & ~0x7)
#define DEPTH_2ND(pScrn) (pScrn->depth > 8 ? pScrn->depth\
                              : SAVPTR(pScrn)->overlayDepth)

#endif /* SAVAGE_DRIVER_H */

