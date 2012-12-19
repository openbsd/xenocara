
/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
Copyright © 2002 David Dawes

All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

/*
 * Authors:
 *   Keith Whitwell <keith@tungstengraphics.com>
 *   David Dawes <dawes@xfree86.org>
 *
 */

#ifndef _I810_H_
#define _I810_H_

#include <stdint.h>
#include "compiler.h"
#include "xf86Pci.h"
#include "i810_reg.h"
#ifdef HAVE_XAA_H
#include "xaa.h"
#endif
#include "xf86Cursor.h"
#include "xf86xv.h"
#include "vbe.h"
#include "vgaHW.h"

#include "xorg-server.h"
#include <pciaccess.h>

#include "compat-api.h"
#ifdef HAVE_DRI1
#include "xf86drm.h"
#include "sarea.h"
#define _XF86DRI_SERVER_
#include "dri.h"
#include "i810_dri.h"
#endif

#include "i810_common.h"
#include "i810_ring.h"

#include "intel_driver.h"

/* HWMC Surfaces */
#define I810_MAX_SURFACES 7
#define I810_MAX_SUBPICTURES 2
#define I810_TOTAL_SURFACES 9

/* Globals */

typedef struct _I810Rec *I810Ptr;

extern void I810SetTiledMemory(ScrnInfoPtr pScrn, int nr, unsigned start,
			       unsigned pitch, unsigned size);

typedef struct {
   unsigned long Start;
   unsigned long End;
   unsigned long Size;
} I810MemRange;

typedef struct {
   int tail_mask;
   I810MemRange mem;
   unsigned char *virtual_start;
   int head;
   int tail;
   int space;
} I810RingBuffer;

typedef struct {
   unsigned char DisplayControl;
   unsigned char PixelPipeCfg0;
   unsigned char PixelPipeCfg1;
   unsigned char PixelPipeCfg2;
   unsigned short VideoClk2_M;
   unsigned short VideoClk2_N;
   unsigned char VideoClk2_DivisorSel;
   unsigned char AddressMapping;
   unsigned char IOControl;
   unsigned char BitBLTControl;
   unsigned char ExtVertTotal;
   unsigned char ExtVertDispEnd;
   unsigned char ExtVertSyncStart;
   unsigned char ExtVertBlankStart;
   unsigned char ExtHorizTotal;
   unsigned char ExtHorizBlank;
   unsigned char ExtOffset;
   unsigned char InterlaceControl;
   unsigned int LMI_FIFO_Watermark;

   unsigned int LprbTail;
   unsigned int LprbHead;
   unsigned int LprbStart;
   unsigned int LprbLen;

   unsigned int Fence[8];

   unsigned short OverlayActiveStart;
   unsigned short OverlayActiveEnd;

} I810RegRec, *I810RegPtr;

typedef struct _I810Rec {
   unsigned char *MMIOBase;
   unsigned char *FbBase;
   long FbMapSize;
   long DepthOffset;
   long BackOffset;
   int cpp;
   int MaxClock;

   unsigned int bufferOffset;		/* for I810SelectBuffer */
   Bool DoneFrontAlloc;
   BoxRec FbMemBox;
   I810MemRange FrontBuffer;
   I810MemRange BackBuffer;
   I810MemRange DepthBuffer;
   I810MemRange TexMem;
   I810MemRange Scratch;
   I810MemRange BufferMem;
   I810MemRange ContextMem;
   I810MemRange MC;

   int auxPitch;
   int auxPitchBits;

   Bool CursorIsARGB;
   int CursorOffset;
   unsigned long CursorPhysical;
   unsigned long CursorStart;
   int CursorARGBOffset;
   unsigned long CursorARGBPhysical;
   unsigned long CursorARGBStart;
   unsigned long OverlayPhysical;
   unsigned long OverlayStart;
   int colorKey;
   unsigned int surfaceAllocation[I810_TOTAL_SURFACES];
   int numSurfaces;

   DGAModePtr DGAModes;
   int numDGAModes;
   Bool DGAactive;
   int DGAViewportStatus;

   int Chipset;
   unsigned long LinearAddr;
   unsigned long MMIOAddr;
   EntityInfoPtr pEnt;
   struct pci_device *PciInfo;

   I810RingBuffer *LpRing;
   unsigned int BR[20];

   int LmFreqSel;

   int VramKey;
   unsigned long VramOffset;
   int DcacheKey;
   unsigned long DcacheOffset;
   int HwcursKey;
   unsigned long HwcursOffset;
   int ARGBHwcursKey;
   unsigned long ARGBHwcursOffset;

   int GttBound;

   I810MemRange DcacheMem;
   I810MemRange SysMem;

   I810MemRange SavedDcacheMem;
   I810MemRange SavedSysMem;

   unsigned char **ScanlineColorExpandBuffers;
   int NumScanlineColorExpandBuffers;
   int nextColorExpandBuf;

   I810RegRec SavedReg;
   I810RegRec ModeReg;

#ifdef HAVE_XAA_H
   XAAInfoRecPtr AccelInfoRec;
#endif
   xf86CursorInfoPtr CursorInfoRec;
   CloseScreenProcPtr CloseScreen;
   ScreenBlockHandlerProcPtr BlockHandler;

   Bool directRenderingDisabled;        /* DRI disabled in PreInit */
   Bool directRenderingEnabled;		/* false if XF86DRI not defined. */

#ifdef HAVE_DRI1
   int LockHeld;
   DRIInfoPtr pDRIInfo;
   int drmSubFD;
   int numVisualConfigs;
   unsigned long dcacheHandle;
   unsigned long backHandle;
   unsigned long zHandle;
   unsigned long cursorHandle;
   unsigned long cursorARGBHandle;
   unsigned long xvmcHandle;
   unsigned long sysmemHandle;
   Bool agpAcquired;
   drm_handle_t buffer_map;
   drm_handle_t ring_map;
   drm_handle_t overlay_map;
   drm_handle_t mc_map;
   drm_handle_t xvmcContext;
#endif
   Bool agpAcquired2d;

   XF86VideoAdaptorPtr adaptor;
   OptionInfoPtr Options;

   int configured_device;

   Bool showCache;
   Bool noAccel;
   Bool allowPageFlip;
   Bool have3DWindows;
   int  drmMinor;
} I810Rec;

#define I810PTR(p) ((I810Ptr)((p)->driverPrivate))

#define I810_SELECT_FRONT	0
#define I810_SELECT_BACK	1
#define I810_SELECT_DEPTH	2

#ifdef HAVE_DRI1
extern Bool I810DRIScreenInit(ScreenPtr pScreen);
extern void I810DRICloseScreen(ScreenPtr pScreen);
extern Bool I810DRIFinishScreenInit(ScreenPtr pScreen);
extern Bool I810DRILeave(ScrnInfoPtr pScrn);
extern Bool I810DRIEnter(ScrnInfoPtr pScrn);
#endif
extern Bool I810InitDma(ScrnInfoPtr pScrn);
extern Bool I810CleanupDma(ScrnInfoPtr pScrn);

#define I810PTR(p) ((I810Ptr)((p)->driverPrivate))
#define I810REGPTR(p) (&(I810PTR(p)->ModeReg))

extern Bool I810CursorInit(ScreenPtr pScreen);
#ifdef HAVE_XAA_H
extern Bool I810AccelInit(ScreenPtr pScreen);
#else
static inline  Bool I810AccelInit(ScreenPtr pScreen) { return TRUE; }
#endif
extern void I810SetPIOAccess(I810Ptr pI810);
extern void I810SetMMIOAccess(I810Ptr pI810);
extern unsigned int I810CalcWatermark(ScrnInfoPtr pScrn, double freq,
				      Bool dcache);
extern void I810PrintErrorState(ScrnInfoPtr pScrn);
extern int I810WaitLpRing(ScrnInfoPtr pScrn, int n, int timeout_millis);
extern void I810Sync(ScrnInfoPtr pScrn);
extern unsigned long I810LocalToPhysical(ScrnInfoPtr pScrn,
					 unsigned long local);
extern int I810AllocLow(I810MemRange * result, I810MemRange * pool, 
			int size);
extern int I810AllocHigh(I810MemRange * result, I810MemRange * pool,
			int size);
extern Bool I810AllocateFront(ScrnInfoPtr pScrn);

extern int I810AllocateGARTMemory(ScrnInfoPtr pScrn);
extern void I810FreeGARTMemory(ScrnInfoPtr pScrn);

extern Bool I810BindGARTMemory(ScrnInfoPtr pScrn);
extern Bool I810UnbindGARTMemory(ScrnInfoPtr pScrn);

extern int I810CheckAvailableMemory(ScrnInfoPtr pScrn);

extern Bool I810SwitchMode(SWITCH_MODE_ARGS_DECL);
extern void I810AdjustFrame(ADJUST_FRAME_ARGS_DECL);

extern void I810SetupForScreenToScreenCopy(ScrnInfoPtr pScrn, int xdir,
					   int ydir, int rop,
					   unsigned int planemask,
					   int trans_color);
extern void I810SubsequentScreenToScreenCopy(ScrnInfoPtr pScrn, int srcX,
					     int srcY, int dstX, int dstY,
					     int w, int h);
extern void I810SetupForSolidFill(ScrnInfoPtr pScrn, int color, int rop,
				  unsigned int planemask);
extern void I810SubsequentSolidFillRect(ScrnInfoPtr pScrn, int x, int y,
					int w, int h);

extern void I810SelectBuffer(ScrnInfoPtr pScrn, int buffer);

extern void I810RefreshRing(ScrnInfoPtr pScrn);
extern void I810EmitFlush(ScrnInfoPtr pScrn);

extern Bool I810DGAInit(ScreenPtr pScreen);

extern void I810InitVideo(ScreenPtr pScreen);
extern void I810InitMC(ScreenPtr pScreen);

extern const OptionInfoRec *I810AvailableOptions(int chipid, int busid);

extern const int I810CopyROP[16];
const int I810PatternROP[16];

#endif /* _I810_H_ */
