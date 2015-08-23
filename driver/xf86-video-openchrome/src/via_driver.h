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

#ifndef _VIA_DRIVER_H_
#define _VIA_DRIVER_H_ 1

#define HAVE_DEBUG 1

#ifdef HAVE_DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

#include "vgaHW.h"
#include "xf86.h"

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6 
#include "xf86Resources.h"
#endif

#include "xf86Pci.h"
#include "xf86PciInfo.h"
#include "xf86_OSproc.h"
#include "compiler.h"
#include "xf86Cursor.h"
#include "mipointer.h"
#include "micmap.h"
#include "fourcc.h"

#define USE_FB
#ifdef USE_FB
#include "fb.h"
#else
#include "cfb.h"
#include "cfb16.h"
#include "cfb32.h"
#endif

#include "xf86cmap.h"
#include "vbe.h"
#ifdef USE_XAA
#include "xaa.h"
#endif

#include "compat-api.h"
#include "via_regs.h"
#include "via_bios.h"
#include "via_priv.h"
#include "via_swov.h"
#include "via_dmabuffer.h"
#include "via_3d.h"
#include "via_video.h"

#ifdef XSERVER_LIBPCIACCESS
#include <pciaccess.h>
#endif

#ifdef OPENCHROMEDRI
#define _OPENCHROMEDRI_SERVER_
#include "sarea.h"
#include "dri.h"
#include "GL/glxint.h"
#include "via_dri.h"
#endif

#include "exa.h"
#define VIA_AGP_UPL_SIZE    (1024*128)
#define VIA_DMA_DL_SIZE     (1024*128)
#define VIA_SCRATCH_SIZE    (4*1024*1024)

/*
 * Pixmap sizes below which we don't try to do hw accel.
 */

#define VIA_MIN_COMPOSITE   400
#define VIA_MIN_UPLOAD 4000
#define VIA_MIN_TEX_UPLOAD 200
#define VIA_MIN_DOWNLOAD 200

#define AGP_PAGE_SIZE 4096
#define AGP_PAGES 8192
#define AGP_SIZE (AGP_PAGE_SIZE * AGP_PAGES)

#define DRIVER_NAME     "openchrome"
#define VERSION_MAJOR   0
#define VERSION_MINOR   2
#define PATCHLEVEL      906
#define VIA_VERSION     ((VERSION_MAJOR<<24) | (VERSION_MINOR<<16) | PATCHLEVEL)

#define VIA_VQ_SIZE             (256 * 1024)

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6 
#define VIA_RES_SHARED RES_SHARED_VGA
#define VIA_RES_UNDEF RES_UNDEFINED
#else
#define VIA_RES_SHARED NULL
#define VIA_RES_UNDEF NULL
#endif

typedef struct {
    CARD8   SR08, SR0A, SR0F;

    /*   extended Sequencer registers */
    CARD8   SR10, SR11, SR12, SR13,SR14,SR15,SR16;
    CARD8   SR17, SR18, SR19, SR1A,SR1B,SR1C,SR1D,SR1E;
    CARD8   SR1F, SR20, SR21, SR22,SR23,SR24,SR25,SR26;
    CARD8   SR27, SR28, SR29, SR2A,SR2B,SR2C,SR2D,SR2E;
    CARD8   SR2F, SR30, SR31, SR32,SR33,SR34,SR40,SR41;
    CARD8   SR42, SR43, SR44, SR45,SR46,SR47,SR48,SR49;
    CARD8   SR4A, SR4B, SR4C, SR4D;

    /*   extended CRTC registers */
    CARD8   CR0C, CR0D;
    CARD8   CR13, CR30, CR31, CR32, CR33, CR34, CR35, CR36;
    CARD8   CR37, CR38, CR39, CR3A, CR40, CR41, CR42, CR43;
    CARD8   CR44, CR45, CR46, CR47, CR48, CR49, CR4A;
    CARD8   CR97, CR99, CR9B, CR9F, CRA0, CRA1, CRA2;
    CARD8   CRTCRegs[68];
/*    CARD8   LCDRegs[0x40];*/

    /* TMDS/LVDS Control */
    CARD8   CRD2;
} VIARegRec, *VIARegPtr;

/*
 * variables that need to be shared among different screens.
 */
typedef struct {
    Bool b3DRegsInitialized;
} ViaSharedRec, *ViaSharedPtr;

#ifdef OPENCHROMEDRI

#define VIA_XVMC_MAX_BUFFERS 2
#define VIA_XVMC_MAX_CONTEXTS 4
#define VIA_XVMC_MAX_SURFACES 20

    
typedef struct {
    VIAMem memory_ref;
    unsigned long offsets[VIA_XVMC_MAX_BUFFERS];
} ViaXvMCSurfacePriv;

typedef struct {
    drm_context_t drmCtx;
} ViaXvMCContextPriv;

typedef struct {
    XID contexts[VIA_XVMC_MAX_CONTEXTS];
    XID surfaces[VIA_XVMC_MAX_SURFACES];
    ViaXvMCSurfacePriv *sPrivs[VIA_XVMC_MAX_SURFACES];
    ViaXvMCContextPriv *cPrivs[VIA_XVMC_MAX_CONTEXTS];
    int nContexts,nSurfaces;
    drm_handle_t mmioBase,fbBase,sAreaBase;
    unsigned sAreaSize;
    drmAddress sAreaAddr;
    unsigned activePorts;
}ViaXvMC, *ViaXvMCPtr;

#endif

typedef struct _twodContext {
    CARD32 mode;
    CARD32 cmd;
    CARD32 fgColor;
    CARD32 bgColor;
    CARD32 pattern0;
    CARD32 pattern1;
    CARD32 patternAddr;
    CARD32 keyControl;
    unsigned srcOffset;
    unsigned srcPitch;
    unsigned Bpp;
    unsigned bytesPPShift;
    Bool clipping;
    Bool dashed;
    int clipX1;
    int clipX2;
    int clipY1;
    int clipY2;
} ViaTwodContext;

typedef struct{
    /* textMode */
    CARD8 *state, *pstate; /* SVGA state */
    int statePage, stateSize, stateMode;

    /* vbe version */
    int major, minor;
} ViaVbeModeInfo;

typedef struct _VIA {
    VIARegRec           SavedReg;
    xf86CursorInfoPtr   CursorInfoRec;
    int                 Bpp, Bpl;

    Bool                FirstInit;
    unsigned long       videoRambytes;
    int                 videoRamKbytes;
    int                 FBFreeStart;
    int                 FBFreeEnd;
    int                 driSize;
    int                 maxDriSize;
    int                 VQStart;
    int                 VQEnd;

    /* These are physical addresses. */
    unsigned long       FrameBufferBase;
    unsigned long       MmioBase;

    /* These are linear addresses. */
    unsigned char*      MapBase;
    unsigned char*      VidMapBase;
    unsigned char*      MpegMapBase;
    unsigned char*      BltBase;
    unsigned char*      MapBaseDense;
    unsigned char*      FBBase;
    CARD8               MemClk;

    /* Here are all the Options */
    Bool                VQEnable;
    Bool		hwcursor;
    Bool                NoAccel;
    Bool                shadowFB;
    Rotation            rotate;
    Bool                vbeSR;
    int                 agpMem;

    CloseScreenProcPtr  CloseScreen;
#ifdef XSERVER_LIBPCIACCESS
    struct pci_device *PciInfo;
    int mmio_bar;
    int fb_bar;
#else
    pciVideoPtr PciInfo;
    PCITAG PciTag;
#endif
    int                 Chipset;
    int                 ChipId;
    int                 ChipRev;
    int                 EntityIndex;

    /* vbe */
    vbeInfoPtr          pVbe;
    ViaVbeModeInfo      vbeMode;
    Bool                useVBEModes;
    Bool                useLegacyVBE;

    /* Support for shadowFB and rotation */
    unsigned char*      ShadowPtr;
    int                 ShadowPitch;
    void                (*PointerMoved)(int index, int x, int y);

    /* Support for XAA acceleration */
#ifdef USE_XAA
    XAAInfoRecPtr       AccelInfoRec;
#endif
    ViaTwodContext      td;
    Via3DState          v3d;
    Via3DState          *lastToUpload;
    ViaCommandBuffer    cb;
    int                 accelMarker;
    CARD32              markerOffset;
    CARD32             *markerBuf;
    CARD32              curMarker;
    CARD32              lastMarkerRead;
    Bool                agpDMA;
    Bool                nPOT[VIA_NUM_TEXUNITS];
    const unsigned     *TwodRegs;
    const unsigned     *HqvCmeRegs;
    ExaDriverPtr        exaDriverPtr;
    ExaOffscreenArea   *exa_scratch;
    unsigned int        exa_scratch_next;
    Bool                useEXA;
    void               *maskP;
    CARD32              maskFormat;
    Bool                componentAlpha;
    void               *srcP;
    CARD32              srcFormat;
    ExaOffscreenArea   *scratchFBBuffer;
    unsigned            scratchOffset;
    int                 exaScratchSize;
    char *              scratchAddr;
    Bool                noComposite;
#ifdef OPENCHROMEDRI
    drm_via_mem_t       scratchAGPBuffer;
    drm_via_mem_t       texAGPBuffer;
    unsigned            texOffset;
    char *              texAddr;
    char *              dBounce;
#endif

    /* Rotation */
    Bool    RandRRotation;

    /* BIOS Info Ptr */
    VIABIOSInfoPtr      pBIOSInfo;
    struct ViaCardIdStruct* Id;

    /* Support for DGA */
    int                 numDGAModes;
    DGAModePtr          DGAModes;
    Bool                DGAactive;
    int                 DGAViewportStatus;
    int			DGAOldDisplayWidth;
    int			DGAOldBitsPerPixel;
    int			DGAOldDepth;

    /* I2C & DDC */
    I2CBusPtr           pI2CBus1;    
    I2CBusPtr           pI2CBus2;    
    I2CBusPtr           pI2CBus3;
    xf86MonPtr          DDC1;
    xf86MonPtr          DDC2;

    /* MHS */
    Bool                IsSecondary;
    Bool                HasSecondary;
    Bool                SAMM;

#ifdef OPENCHROMEDRI
    Bool		directRenderingEnabled;
    Bool                XvMCEnabled;
    DRIInfoPtr		pDRIInfo;
    int 		drmFD;
    int 		numVisualConfigs;
    __GLXvisualConfig* 	pVisualConfigs;
    VIAConfigPrivPtr 	pVisualConfigsPriv;
    drm_handle_t 	agpHandle;
    drm_handle_t 	registerHandle;
    drm_handle_t        frameBufferHandle;
    unsigned long 	agpAddr;
    drmAddress          agpMappedAddr;
    unsigned char 	*agpBase;
    unsigned int 	agpSize;
    Bool 		IsPCI;
    ViaXvMC             xvmc;
    int                 drmVerMajor;
    int                 drmVerMinor;
    int                 drmVerPL;
    VIAMem              driOffScreenMem;
    void *              driOffScreenSave;
#endif
    Bool		DRIIrqEnable;
    Bool                agpEnable;
    Bool                dma2d;
    Bool                dmaXV;

    CARD8               ActiveDevice;	/* Option */

    unsigned char       *CursorImage;
    CARD32              CursorFG;
    CARD32              CursorBG;
    Bool                CursorARGB;
    Bool                CursorARGBSupported;
    CARD8               CursorPipe;
    int                 CursorStart;
	int					CursorMaxWidth;
	int					CursorMaxHeight;
	int					CursorSize;

    CARD32              CursorRegControl;
    CARD32              CursorRegBase;
    CARD32              CursorRegPos;
    CARD32              CursorRegOffset;
    CARD32              CursorRegFifo;
    CARD32              CursorRegTransKey;

    CARD32              CursorControl0;
    CARD32              CursorControl1;
    CARD32              CursorFifo;
    CARD32              CursorTransparentKey;
    CARD32              CursorPrimHiInvtColor;
    CARD32              CursorV327HiInvtColor; 
    
    /* Video */
    int                 VideoEngine;
    swovRec		swov;
    CARD32              VideoStatus;
    VIAHWDiff		HWDiff;
    unsigned long	dwV1, dwV3;
    unsigned long	OverlaySupported;
    unsigned long	dwFrameNum;

    CARD32*		VidRegBuffer; /* Temporary buffer for video overlay registers. */
    unsigned long	VidRegCursor; /* Write cursor for VidRegBuffer. */

    unsigned long	old_dwUseExtendedFIFO;
    
    ViaSharedPtr	sharedData;
    Bool                useDmaBlit;

    void                *displayMap;
    CARD32              displayOffset;
    void                *cursorMap;
    CARD32              cursorOffset;

    CARD8               I2CDevices;     /* Option */

#ifdef HAVE_DEBUG
    Bool                disableXvBWCheck;
    Bool                DumpVGAROM;
    Bool                PrintVGARegs;
    Bool                PrintTVRegs;
    Bool                I2CScan;
#endif /* HAVE_DEBUG */
    
    Bool                UseLegacyModeSwitch ;
    video_via_regs*     VideoRegs ;
} VIARec, *VIAPtr;

#define VIAPTR(p) ((VIAPtr)((p)->driverPrivate))

typedef struct
{
    Bool IsDRIEnabled;

    Bool HasSecondary;
    Bool BypassSecondary;
    /*These two registers are used to make sure the CRTC2 is
      restored before CRTC_EXT, otherwise it could lead to blank screen.*/
    Bool IsSecondaryRestored;
    Bool RestorePrimary;

    ScrnInfoPtr pSecondaryScrn;
    ScrnInfoPtr pPrimaryScrn;
} VIAEntRec, *VIAEntPtr;

/* Prototypes. */
void VIAInitialize3DEngine(ScrnInfoPtr pScrn);

/* In via_cursor.c. */
Bool viaCursorHWInit(ScreenPtr pScreen);
void viaCursorShow(ScrnInfoPtr);
void viaCursorHide(ScrnInfoPtr);
void viaCursorStore(ScrnInfoPtr pScrn);
void viaCursorRestore(ScrnInfoPtr pScrn);
Bool viaCursorRecInit(ScrnInfoPtr pScrn);
void viaCursorRecDestroy(ScrnInfoPtr pScrn);
void viaCursorSetFB(ScrnInfoPtr pScrn);

/* In via_accel.c. */
Bool viaInitAccel(ScreenPtr);
void viaInitialize2DEngine(ScrnInfoPtr);
void viaAccelSync(ScrnInfoPtr);
void viaDisableVQ(ScrnInfoPtr);
void viaExitAccel(ScreenPtr);
void viaAccelBlitRect(ScrnInfoPtr, int, int, int, int, int, int);
void viaAccelFillRect(ScrnInfoPtr, int, int, int, int, unsigned long);
void viaAccelSyncMarker(ScrnInfoPtr);
void viaFinishInitAccel(ScreenPtr);
void viaAccelWaitMarker(ScreenPtr, int);
int viaAccelMarkSync(ScreenPtr);
void viaAccelFillPixmap(ScrnInfoPtr, unsigned long, unsigned long, 
			int, int, int, int, int, unsigned long);
void viaAccelTextureBlit(ScrnInfoPtr, unsigned long, unsigned, unsigned, 
			 unsigned, unsigned, unsigned, unsigned,
			 unsigned long, unsigned, unsigned,
			 unsigned, unsigned, int);



/* In via_shadow.c */
void ViaShadowFBInit(ScrnInfoPtr pScrn, ScreenPtr pScreen);

/* In via_dga.c */
Bool VIADGAInit(ScreenPtr);

/*In via_video.c*/
void viaInitVideo(ScreenPtr pScreen);
void viaExitVideo(ScrnInfoPtr pScrn);
void viaSaveVideo(ScrnInfoPtr pScrn);
void viaRestoreVideo(ScrnInfoPtr pScrn);
void viaSetColorSpace(VIAPtr pVia, int hue, int saturation, int brightness, int contrast,
		      Bool reset);
void VIAVidAdjustFrame(ScrnInfoPtr pScrn, int x, int y);

/* In via_memory.c */
void VIAFreeLinear(VIAMemPtr);
int VIAAllocLinear(VIAMemPtr, ScrnInfoPtr, unsigned long);
int viaOffscreenLinear(VIAMemPtr, ScrnInfoPtr, unsigned long);
void VIAInitLinear(ScreenPtr pScreen);

/* In via_xwmc.c */

#ifdef OPENCHROMEDRI
/* Basic init and exit functions */
void ViaInitXVMC(ScreenPtr pScreen);    
void ViaCleanupXVMC(ScrnInfoPtr pScrn, XF86VideoAdaptorPtr *XvAdaptors, int XvAdaptorCount);
int viaXvMCInitXv(ScrnInfoPtr pScrn, XF86VideoAdaptorPtr XvAdapt);

/* Returns the size of the fake Xv Image used as XvMC command buffer to the X server*/
unsigned long viaXvMCPutImageSize(ScrnInfoPtr pScrn);



#endif

/* via_i2c.c */
void ViaI2CInit(ScrnInfoPtr pScrn);

#ifdef OPENCHROMEDRI
Bool VIADRIScreenInit(ScreenPtr pScreen);
void VIADRICloseScreen(ScreenPtr pScreen);
Bool VIADRIFinishScreenInit(ScreenPtr pScreen);
void VIADRIRingBufferCleanup(ScrnInfoPtr pScrn);
Bool VIADRIRingBufferInit(ScrnInfoPtr pScrn);
void viaDRIOffscreenRestore(ScrnInfoPtr pScrn);
void viaDRIOffscreenSave(ScrnInfoPtr pScrn);

#endif /* OPENCHROMEDRI */

#endif /* _VIA_DRIVER_H_ */
