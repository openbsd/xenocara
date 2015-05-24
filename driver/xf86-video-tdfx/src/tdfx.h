/*
   Voodoo Banshee driver version 1.0.2

   Author: Daryll Strauss

   Copyright: 1998,1999
*/

#ifndef _TDFX_H_
#define _TDFX_H_

#include <string.h>
#include <stdio.h>

#include "xf86Pci.h"
#include "xf86DDC.h"
#include "xf86xv.h"
#include "tdfxdefs.h"

#ifndef XF86DRI
#undef TDFXDRI
#endif

#ifdef TDFXDRI
#include "xf86drm.h"
#include "sarea.h"
#define _XF86DRI_SERVER_
#include "dri.h"
#include "GL/glxint.h"
#include "tdfx_dri.h"
#include "tdfx_dripriv.h"
#endif

#define TDFX_VERSION 4000
#define TDFX_NAME "TDFX"
#define TDFX_DRIVER_NAME "tdfx"
#define TDFX_MAJOR_VERSION PACKAGE_VERSION_MAJOR
#define TDFX_MINOR_VERSION PACKAGE_VERSION_MINOR
#define TDFX_PATCHLEVEL PACKAGE_VERSION_PATCHLEVEL

/* Macros to aid source compatibilty between pci-rework and "classic" builds.
 */
#ifdef XSERVER_LIBPCIACCESS
#include <pciaccess.h>

#define DEVICE_ID(p) (p)->device_id

#define PCI_READ_LONG(value, offset, card_index) \
    pci_device_cfg_read_u32(pTDFX->PciInfo[(card_index)], & (value), (offset))

#define PCI_WRITE_LONG(value, offset, card_index) \
    pci_device_cfg_write_u32(pTDFX->PciInfo[(card_index)], (value), (offset))

#define PCI_IO_BASE(p, region) \
    (p)->regions[region].base_addr

#define PCI_MEM_BASE(p, region) \
    (p)->regions[region].base_addr
#else
#define DEVICE_ID(p) (p)->chipType

#define PCI_READ_LONG(value, offset, card_index) \
    (value) = pciReadLong(pTDFX->PciTag[(card_index)], (offset))

#define PCI_WRITE_LONG(value, offset, card_index) \
    pciWriteLong(pTDFX->PciTag[(card_index)], (offset), (value))

#define PCI_IO_BASE(p, region) \
    (p)->ioBase[region]

#define PCI_MEM_BASE(p, region) \
    (p)->memBase[region]
#endif

#define PCI_VENDOR_3DFX			0x121A

#define PCI_CHIP_VOODOO_GRAPHICS	0x0001
#define PCI_CHIP_VOODOO2		0x0002
#define PCI_CHIP_BANSHEE		0x0003
#define PCI_CHIP_VELOCITY		0x0004
#define PCI_CHIP_VOODOO3		0x0005
#define PCI_CHIP_VOODOO4		0x0007
#define PCI_CHIP_VOODOO5		0x0009

#define PCI_CARD_VOODOO3_2000		0x0036
#define PCI_CARD_VOODOO3_3000		0x003A

struct _TDFXRec;
typedef struct _TDFXRec *TDFXPtr;

#include "tdfx_priv.h"
extern void TDFXSwapContextFifo(ScreenPtr pScreen);
extern void TDFXLostContext(ScreenPtr pScreen);
extern Bool TDFXSetupSLI(ScrnInfoPtr pScrn, Bool sliEnable, int aaSamples);
extern Bool TDFXDisableSLI(TDFXPtr pTDFX);

#ifdef TDFXDRI
extern void FillPrivateDRI(TDFXPtr pTDFX, TDFXDRIPtr pTDFXDRI);
#endif

#if 0
/* These are not normally turned on. They are only included for debugging. */
#define TRACE 1
#define TRACEACCEL 1
#define TDFX_DEBUG_CMDS
#define TRACECURS 1
#define REGDEBUG 1
#define TRACEREG
#endif

#ifdef TRACE
#define TDFXTRACE ErrorF
#else
#define TDFXTRACE if(0) ErrorF
#endif

#ifdef TRACEACCEL
#define TDFXTRACEACCEL ErrorF
#else
#define TDFXTRACEACCEL if(0) ErrorF
#endif

#ifdef TRACECURS
#define TDFXTRACECURS ErrorF
#else
#define TDFXTRACECURS if(0) ErrorF
#endif

#ifdef TRACEREG
#define TDFXTRACEREG ErrorF
#else
#define TDFXTRACEREG if(0) ErrorF
#endif

#ifdef HAVE_XAA_H
#include "xaa.h"
#endif
#include "xf86fbman.h"
#include "xf86Cursor.h"
#include "compat-api.h"

typedef void (*TDFXWriteIndexedByteFunc)(TDFXPtr pTDFX, int addr, 
					 char index, char value);
typedef char (*TDFXReadIndexedByteFunc)(TDFXPtr pTDFX, int addr, 
					char index);
typedef void (*TDFXWriteWordFunc)(TDFXPtr pTDFX, int addr, int value);
typedef int (*TDFXReadWordFunc)(TDFXPtr pTDFX, int addr);
typedef void (*TDFXWriteChipIndexedByteFunc)(TDFXPtr pTDFX, int chip, 
					     int addr, char index, char value);
typedef char (*TDFXReadChipIndexedByteFunc)(TDFXPtr pTDFX, int chip, 
					    int addr, char index);
typedef void (*TDFXWriteChipWordFunc)(TDFXPtr pTDFX, int chip, 
				      int addr, int value);
typedef int (*TDFXReadChipWordFunc)(TDFXPtr pTDFX, int chip,
				    int addr);
typedef void (*TDFXSyncFunc)(ScrnInfoPtr pScrn);
typedef void (*TDFXBufferFunc)(TDFXPtr pTDFX, int which);
#if X_BYTE_ORDER == X_BIG_ENDIAN
typedef void (*TDFXWriteFifoFunc)(TDFXPtr pTDFX, int val);
#endif


typedef struct {
  unsigned int vidcfg;
  unsigned int vidpll;
  unsigned int dacmode;
  unsigned int vgainit0;
  unsigned int vgainit1;
  unsigned int miscinit0;
  unsigned int miscinit1;
  unsigned int screensize;
  unsigned int stride;
  unsigned int cursloc;
  unsigned int startaddr;
  unsigned int clip0min;
  unsigned int clip0max;
  unsigned int clip1min;
  unsigned int clip1max;
  unsigned int srcbaseaddr;
  unsigned int dstbaseaddr;
  unsigned char ExtVga[2];
  unsigned int dactable[512];
} TDFXRegRec, *TDFXRegPtr;

typedef struct TextureData_t {
  int contextid;
  void *data;
  struct TextureData_t *next;
} TextureData;

#define MAXCHIPS 4

#ifdef XSERVER_LIBPCIACCESS
enum tdfx_chips {
    Banshee = 0,
    Voodoo3_2000,
    Voodoo3_3000,
    Voodoo3_Unknown,
    Voodoo5,
    MAX_VOODOO_CARDS
};
#endif

typedef struct _TDFXRec {
#ifdef XSERVER_LIBPCIACCESS
    enum tdfx_chips match_id;
    void *MMIOBase[MAXCHIPS];
    void *FbBase;
#else
  unsigned char *MMIOBase[MAXCHIPS];
  unsigned char *FbBase;
  unsigned char *myFbBase;
#endif
  unsigned long PIOBase[MAXCHIPS];
  long FbMapSize;
  int pixelFormat;
  int stride;
  int cpp;
  int maxClip;
  int MaxClock;
  int ChipType;
#ifdef XSERVER_LIBPCIACCESS
    struct pci_device *PciInfo[MAXCHIPS];
#else
  pciVideoPtr PciInfo;
#endif
  unsigned long LinearAddr[MAXCHIPS];
  unsigned long MMIOAddr[MAXCHIPS];
  EntityInfoPtr pEnt;
  int numChips;
#ifndef XSERVER_LIBPCIACCESS
  PCITAG PciTag[MAXCHIPS];
#endif
  Bool Primary;
  int HasSGRAM;
  int PciCnt;
  int PrevDrawState;
  int DrawState;
  int Cmd;
  int DashedLineSize;
  BoxRec prevBlitDest;
  TDFXRegRec SavedReg;
  TDFXRegRec ModeReg;
#ifdef HAVE_XAA_H
  XAAInfoRecPtr AccelInfoRec;
#endif
  xf86CursorInfoPtr CursorInfoRec;
  CloseScreenProcPtr CloseScreen;
  Bool usePIO;
  Bool NoAccel;
  DGAModePtr DGAModes;
  Bool DGAactive;
  Bool initDone;
  int DGAViewportStatus;
  int cursorOffset;
  int fbOffset;
  int backOffset;
  int depthOffset;
  int texOffset;
  int texSize;
  TDFXWriteIndexedByteFunc writeControl;
  TDFXReadIndexedByteFunc readControl;
  TDFXWriteWordFunc writeLong;
  TDFXReadWordFunc readLong;
  TDFXWriteChipWordFunc writeChipLong;
  TDFXReadChipWordFunc readChipLong;
  TDFXSyncFunc sync;
#if X_BYTE_ORDER == X_BIG_ENDIAN
  TDFXWriteFifoFunc writeFifo;
#endif
  int syncDone;
  int scanlineWidth;
  unsigned char *scanlineColorExpandBuffers[2];
  PROPDATA
#ifdef TDFXDRI
  Bool directRenderingEnabled;
  DRIInfoPtr pDRIInfo;
  int drmSubFD;
  int numVisualConfigs;
  __GLXvisualConfig* pVisualConfigs;
  TDFXConfigPrivPtr pVisualConfigsPriv;
  TDFXRegRec DRContextRegs;
#endif
  /* State for video */
  FBAreaPtr offscreenYUVBuf;
  int offscreenYUVBufWidth;
  int offscreenYUVBufHeight;
  
  /* This is a small register shadow.  I'm only shadowing 
   *   sst2dDstFmt
   *   sst2dSrcFmt
   * If a real register shadow is ever needed we should probably
   * shadow everything and make it happen automatically for every write. */
  INT32 sst2DSrcFmtShadow;
  INT32 sst2DDstFmtShadow;
  int pixmapCacheLinesMin;
  int pixmapCacheLinesMax;
  FBAreaPtr reservedArea;
  Bool ShowCache;
  int videoKey;
  void (*VideoTimerCallback)(ScrnInfoPtr, Time);
  FBLinearPtr overlayBuffer;
  FBLinearPtr overlayBuffer2;	/* for double-buffering */
  int whichOverlayBuffer;	/* flip-flop */
  FBAreaPtr textureBuffer;
  Bool TextureXvideo;
  XF86VideoAdaptorPtr overlayAdaptor;
  XF86VideoAdaptorPtr textureAdaptor;
  ScreenBlockHandlerProcPtr BlockHandler;
  OptionInfoPtr Options;

  ScreenWakeupHandlerProcPtr coreWakeupHandler;
  ScreenBlockHandlerProcPtr coreBlockHandler;

  I2CBusPtr pI2CBus;

} TDFXRec;

typedef struct {
  RegionRec	clip;
  CARD32	colorKey;
  int		filterQuality;
  CARD32	videoStatus;
  Time		offTime;
  Time		freeTime;
} TDFXPortPrivRec, *TDFXPortPrivPtr;

typedef struct {
  PROPSAREADATA
  int fifoOwner;
  int CtxOwner;
  int TexOwner;
} TDFXSAREAPriv;

#define TDFXPTR(p) ((TDFXPtr)((p)->driverPrivate))

#define DRAW_STATE_CLIPPING 0x1
#define DRAW_STATE_TRANSPARENT 0x2
#define DRAW_STATE_CLIP1CHANGED 0x4

#define TDFX_FRONT 0
#define TDFX_BACK 1
#define TDFX_DEPTH 2

#define TDFX2XCUTOFF 135000

extern Bool TDFXAccelInit(ScreenPtr pScreen);
extern Bool TDFXCursorInit(ScreenPtr pScreen);
extern void TDFXSync(ScrnInfoPtr pScrn);
extern Bool TDFXDRIScreenInit(ScreenPtr pScreen);
extern void TDFXDRICloseScreen(ScreenPtr pScreen);
extern Bool TDFXDRIFinishScreenInit(ScreenPtr pScreen);
extern Bool TDFXDGAInit(ScreenPtr pScreen);
extern void TDFXCursorGrabMemory(ScreenPtr pScreen);
extern void TDFXSetLFBConfig(TDFXPtr pTDFX);
extern void TDFXSendNOPFifo(ScrnInfoPtr pScrn);

extern Bool TDFXSwitchMode(SWITCH_MODE_ARGS_DECL);
extern void TDFXAdjustFrame(ADJUST_FRAME_ARGS_DECL);

extern void TDFXSetPIOAccess(TDFXPtr pTDFX);
extern void TDFXSetMMIOAccess(TDFXPtr pTDFX);
extern void TDFXWriteLongMMIO(TDFXPtr pTDFX, int addr, int val);
extern int TDFXReadLongMMIO(TDFXPtr pTDFX, int addr);
extern void TDFXWriteChipLongMMIO(TDFXPtr pTDFX, int chip, int addr, int val);

extern void TDFXNeedSync(ScrnInfoPtr pScrn);
extern void TDFXCheckSync(ScrnInfoPtr pScrn);
extern void TDFXFirstSync(ScrnInfoPtr pScrn);

extern void TDFXSetupForScreenToScreenCopy(ScrnInfoPtr pScrn, int xdir, 
					   int ydir, int rop,
					   unsigned int planemask, 
					   int trans_color);
extern void TDFXSubsequentScreenToScreenCopy(ScrnInfoPtr pScrn, int srcX, 
					     int srcY, int dstX, int dstY, 
					     int w, int h);
extern void TDFXSetupForSolidFill(ScrnInfoPtr pScrn, int color, int rop, 
				  unsigned int planemask);
extern void TDFXSubsequentSolidFillRect(ScrnInfoPtr pScrn, int x, int y, 
					int w, int h);

extern void TDFXSelectBuffer(TDFXPtr pTDFX, int which);

extern void TDFXInitVideo(ScreenPtr pScreen);
extern void TDFXCloseVideo(ScreenPtr pScreen);

#endif
