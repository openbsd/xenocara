/* **********************************************************
 * Copyright (C) 1998-2001 VMware, Inc.
 * All Rights Reserved
 * **********************************************************/

#ifndef VMWARE_H
#define VMWARE_H


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_XORG_SERVER_1_1_0
#include <string.h>
#endif

#include "xf86.h"
#include "xf86_OSproc.h"

#include <X11/extensions/panoramiXproto.h>

#ifdef XSERVER_LIBPCIACCESS
#include <pciaccess.h>
#else
#include "xf86Resources.h"
#endif

#include "compiler.h"	        /* inb/outb */

#include "xf86Pci.h"		/* pci */
#include "xf86Cursor.h"		/* hw cursor */
#include "cursorstr.h"          /* xhot/yhot */

#include "vgaHW.h"		/* VGA hardware */
#include "fb.h"

#include "xf86cmap.h"		/* xf86HandleColormaps */

#include "vm_basic_types.h"
#include "svga_reg.h"
#include "svga_struct.h"
#include "vmware_bootstrap.h"
#include <xf86Module.h>

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 12
#define _swapl(x, n) swapl(x,n)
#define _swaps(x, n) swaps(x,n)
#else
#define _swapl(x, n) (void) n; swapl(x)
#define _swaps(x, n) (void) n; swaps(x)
#endif

/*
 * The virtual hardware's cursor limits are pretty big. Some VMware
 * product versions limit to 1024x1024 pixels, others limit to 128
 * kilobytes of cursor data. We just choose an arbitrary maximum
 * cursor size. 64x64 is a common value for real hardware, so we'll go
 * with that.
 */
#define MAX_CURS        64

#define NUM_DYN_MODES   2


typedef struct {
    CARD32 svga_reg_enable;
    CARD32 svga_reg_width;
    CARD32 svga_reg_height;
    CARD32 svga_reg_bits_per_pixel;

    CARD32 svga_reg_cursor_on;
    CARD32 svga_reg_cursor_x;
    CARD32 svga_reg_cursor_y;
    CARD32 svga_reg_cursor_id;

    Bool svga_fifo_enabled;

    CARD32 svga_reg_id;
} VMWARERegRec, *VMWARERegPtr;

typedef xXineramaScreenInfo VMWAREXineramaRec, *VMWAREXineramaPtr;

typedef struct {
    EntityInfoPtr pEnt;
#if XSERVER_LIBPCIACCESS
    struct pci_device *PciInfo;
#else
    pciVideoPtr PciInfo;
    PCITAG PciTag;
#endif
    Bool Primary;
    int depth;
    int bitsPerPixel;
    rgb weight;
    rgb offset;
    int defaultVisual;
    int videoRam;
    unsigned long memPhysBase;
    unsigned long fbOffset;
    unsigned long fbPitch;
    unsigned long ioBase;
    unsigned long portIOBase;
    int maxWidth;
    int maxHeight;
    unsigned int vmwareCapability;

    unsigned char* FbBase;
    unsigned long FbSize;

    VMWARERegRec SavedReg;
    VMWARERegRec ModeReg;
    CARD32 suspensionSavedRegId;

    DisplayModePtr dynModes[NUM_DYN_MODES];

    Bool* pvtSema;

    Bool noAccel;
    Bool hwCursor;
    Bool cursorDefined;
    int cursorSema;
    Bool cursorExcludedForUpdate;
    Bool cursorShouldBeHidden;

    unsigned int cursorRemoveFromFB;
    unsigned int cursorRestoreToFB;

#ifdef RENDER
    CompositeProcPtr Composite;
    void (*EnableDisableFBAccess)(int, Bool);
#endif /* RENDER */

    unsigned long mmioPhysBase;
    unsigned long mmioSize;

    unsigned char* mmioVirtBase;
    CARD32* vmwareFIFO;

    xf86CursorInfoPtr CursorInfoRec;
    CursorPtr oldCurs;
    struct {
        int bg, fg, x, y;
        int hotX, hotY;
        BoxRec box;

        uint32 mask[SVGA_BITMAP_SIZE(MAX_CURS, MAX_CURS)];
        uint32 maskPixmap[SVGA_PIXMAP_SIZE(MAX_CURS, MAX_CURS, 32)];
        uint32 source[SVGA_BITMAP_SIZE(MAX_CURS, MAX_CURS)];
        uint32 sourcePixmap[SVGA_PIXMAP_SIZE(MAX_CURS, MAX_CURS, 32)];
    } hwcur;

    unsigned long indexReg, valueReg;

    ScreenRec ScrnFuncs;

    /*
     * Xinerama state
     */
    Bool xinerama;
    Bool xineramaStatic;

    VMWAREXineramaPtr xineramaState;
    unsigned int xineramaNumOutputs;

    VMWAREXineramaPtr xineramaNextState;
    unsigned int xineramaNextNumOutputs;

    /*
     * Xv
     */
    DevUnion *videoStreams;

} VMWARERec, *VMWAREPtr;

#define VMWAREPTR(p) ((VMWAREPtr)((p)->driverPrivate))

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define ABS(x) (((x) >= 0) ? (x) : -(x))

#define BOX_INTERSECT(a, b) \
		(ABS(((a).x1 + (a).x2) - ((b).x1 + (b).x2)) <= \
		((a).x2 - (a).x1) + ((b).x2 - (b).x1) && \
		ABS(((a).y1 + (a).y2) - ((b).y1 + (b).y2)) <= \
		((a).y2 - (a).y1) + ((b).y2 - (b).y1))

#define SVGA_GLYPH_SCANLINE_SIZE_DWORDS(w) (((w) + 31) >> 5)

#define PRE_OP_HIDE_CURSOR() \
    if (pVMWARE->cursorDefined && *pVMWARE->pvtSema) { \
        pVMWARE->cursorSema++; \
        if (pVMWARE->cursorSema == 1) { \
            vmwareWriteCursorRegs(pVMWARE, FALSE, FALSE); \
        } \
    }
#define POST_OP_SHOW_CURSOR() \
    if (pVMWARE->cursorDefined && *pVMWARE->pvtSema) { \
        pVMWARE->cursorSema--; \
        if (!pVMWARE->cursorSema && !pVMWARE->cursorShouldBeHidden) { \
            vmwareWriteCursorRegs(pVMWARE, TRUE, FALSE); \
        } \
    }

#define MOUSE_ID 1

/* Undefine this to kill all acceleration */
#define ACCELERATE_OPS

#if XSERVER_LIBPCIACCESS
#define VENDOR_ID(p)      (p)->vendor_id
#define DEVICE_ID(p)      (p)->device_id
#define SUBVENDOR_ID(p)   (p)->subvendor_id
#define SUBSYS_ID(p)      (p)->subdevice_id
#define CHIP_REVISION(p)  (p)->revision
#else
#define VENDOR_ID(p)      (p)->vendor
#define DEVICE_ID(p)      (p)->chipType
#define SUBVENDOR_ID(p)   (p)->subsysVendor
#define SUBSYS_ID(p)      (p)->subsysCard
#define CHIP_REVISION(p)  (p)->chipRev
#endif

void vmwareWriteReg(
   VMWAREPtr pVMWARE, int index, CARD32 value
   );

CARD32 vmwareReadReg(
    VMWAREPtr pVMWARE, int index
    );

void vmwareWriteWordToFIFO(
   VMWAREPtr pVMWARE, CARD32 value
   );

void vmwareWaitForFB(
   VMWAREPtr pVMWARE
   );

void vmwareSendSVGACmdUpdate(
   VMWAREPtr pVMWARE, BoxPtr pBB
   );

void vmwareSendSVGACmdUpdateFullScreen(
   VMWAREPtr pVMWARE
   );

DisplayModeRec *VMWAREAddDisplayMode(
    ScrnInfoPtr pScrn,
    const char *name,
    int width,
    int height
   );

Bool vmwareIsRegionEqual(
    const RegionPtr reg1,
    const RegionPtr reg2
   );

void vmwareNextXineramaState(
   VMWAREPtr pVMWARE
   );

/* vmwarecurs.c */
Bool vmwareCursorInit(
   ScreenPtr pScr
   );

void vmwareCursorModeInit(
    ScrnInfoPtr pScrn,
    DisplayModePtr mode
   );

void vmwareCursorCloseScreen(
    ScreenPtr pScr
    );

void vmwareWriteCursorRegs(
   VMWAREPtr pVMWARE,
   Bool visible,
   Bool force
   );

void vmwareCursorHookWrappers(
   ScreenPtr pScreen
   );


/* vmwarectrl.c */
void VMwareCtrl_ExtInit(ScrnInfoPtr pScrn);

/* vmwarexinerama.c */
void VMwareXinerama_ExtInit(ScrnInfoPtr pScrn);

/* vmwarevideo.c */
Bool vmwareVideoInit(
   ScreenPtr pScreen
   );
void vmwareVideoEnd(
   ScreenPtr pScreen
   );
Bool vmwareVideoEnabled(
   VMWAREPtr pVMWARE
   );

void vmwareCheckVideoSanity(
   ScrnInfoPtr pScrn
   );

/* vmwaremode.c */
void vmwareAddDefaultMode(
   ScrnInfoPtr pScrn,
   uint32 dwidth,
   uint32 dheight
   );
#endif
