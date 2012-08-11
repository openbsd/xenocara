#ifndef _TSENG_H
#define _TSENG_H

#include "config.h"

#include "tseng_pcirename.h"
#include <string.h>

/* All drivers should typically include these */
#include "xf86.h"
#include "xf86_OSproc.h"

/* All drivers need this */

/* Everything using inb/outb, etc needs "compiler.h" */
#include "compiler.h"

/* Drivers that need to access the PCI config space directly need this */
#include "xf86Pci.h"

/* All drivers using the vgahw module need this */
/* All Tseng chips _need_ VGA register access, so multihead operation is out of the question */
#include "vgaHW.h"

/* All drivers using the mi colormap manipulation need this */
#include "micmap.h"

/* Needed for the 1 and 4 bpp framebuffers */
#ifdef HAVE_XF1BPP
#include "xf1bpp.h"
#endif
#ifdef HAVE_XF4BPP
#include "xf4bpp.h"
#endif
#include "fb.h"

/* Drivers using the XAA interface ... */
#ifdef HAVE_XAA_H
#include "xaa.h"
#include "xaalocal.h"
#endif
#include "xf86Cursor.h"
#include "xf86fbman.h"

#include "compat-api.h"
/*
 * Contrary to the old driver, we use the "Chip Revision" here intead of
 * multiple chipsets like "TYPE_ET4000W32Pa", "TYPE_ET4000W32Pb", etc.
 */

typedef enum {
    ET4000, /* We only have the PCI ones so all are W32p */
    ET6000  /* Both ET6000 and ET6100 */
} tseng_chiptype;

/* Artificial: W32p revisions are different pci ids.
 * ET6000 and ET6100 have same pci id but differ by revision.
 */
typedef enum {
    TSENGNOREV = 0,
    REV_A,
    REV_B,
    REV_C,
    REV_D,
    REV_ET6000,
    REV_ET6100
} tseng_chiprev;

typedef enum {
    UNKNOWN_DAC = -1,
    STG1703,
    CH8398
} tseng_ramdac;

typedef struct {
    CARD8 CR30, CR31, CR32, CR33, CR34, CR35, CR36, CR37, CR3F;

    CARD8 SR06, SR07;

    /* ATC 0x16 */
    CARD8 ExtATC;

    /* 0x3CD , 0x3CB */
    CARD8 ExtSegSel[2];

    /* ET6000 PCI config space registers */
    CARD8 ET6K_13, ET6K_40, ET6K_41;
    CARD8 ET6K_44, ET6K_46, ET6K_58;
    CARD16 ET6K_PLL, ET6K_MClk;

    CARD8 CursorCtrl;
    void *RAMDAC; /* new RAMDAC register pointer */
} TsengRegRec, *TsengRegPtr;

typedef struct {
    /* we'll put variables that we want to access _fast_ at the beginning (just a hunch) */
    unsigned char cache_SegSelL, cache_SegSelH;  /* for tseng_bank.c */
    int Bytesperpixel;		       /* a shorthand for the XAA code */
    Bool need_wait_acl;		       /* always need a full "WAIT" for ACL finish */
    int line_width;		       /* framebuffer width in bytes per scanline */
    int planemask_mask;		       /* mask for active bits in planemask */
    int neg_x_pixel_offset;
    int powerPerPixel;		       /* power-of-2 version of bytesperpixel */
    unsigned char *BresenhamTable;
    /* normal stuff starts here */
    pciVideoPtr PciInfo;
#ifndef XSERVER_LIBPCIACCESS
    PCITAG PciTag;
#endif

    Bool UsePCIRetry;		       /* Do we use PCI-retry or busy-waiting */
    Bool UseAccel;		       /* Do we use the XAA acceleration architecture */
    Bool HWCursor;		       /* Do we use the hardware cursor (if supported) */

    Bool SlowDram;
    Bool FastDram;
    Bool MedDram;
    Bool SetPCIBurst;
    Bool PCIBurst;
    Bool SetW32Interleave;
    Bool W32Interleave;
    Bool ShowCache;

    TsengRegRec SavedReg; /* saved Tseng registers at server start */

    tseng_chiptype  ChipType;  /* "Chipset" causes confusion with pScrn->chipset */
    tseng_chiprev  ChipRev;

    memType FbAddress;
    unsigned char *FbBase;
    long FbMapSize;
    CARD32 ET6000IOAddress; /* PCI config space base address for ET6000 */
    char * MMioBase;

    int MemClk;  /* ET6000 only */
    ClockRange clockRange;
    tseng_ramdac RAMDAC; /* ET4000W32p only */

    int max_vco_freq;  /* ET6000: max internal VCO frequency */
    CloseScreenProcPtr CloseScreen;
    int save_divide;
#ifdef HAVE_XAA_H
    XAAInfoRecPtr AccelInfoRec;
#endif
    xf86CursorInfoPtr CursorInfoRec;
    CARD32 AccelColorBufferOffset;     /* offset in video memory where FG and BG colors will be stored */
    CARD32 AccelColorExpandBufferOffsets[3];   /* offset in video memory for ColorExpand buffers */
    unsigned char * XAAColorExpandBuffers[3];  /* pointers to colorexpand buffers */
    CARD32 AccelImageWriteBufferOffsets[2];    /* offset in video memory for ImageWrite Buffers */
    unsigned char * XAAScanlineImageWriteBuffers[2];   /* pointers to ImageWrite Buffers */
    CARD32 HWCursorBufferOffset;
    unsigned char *HWCursorBuffer;
    unsigned char * XAAScanlineColorExpandBuffers[1];
    int acl_blitxdir;
    int acl_blitydir;
    CARD32 acl_iw_dest;
    CARD32 acl_skipleft;
    CARD32 acl_ColorExpandDst;
    int acl_colexp_width_dwords;
    int acl_colexp_width_bytes;

    CARD32* ColExpLUT;

    EntityInfoPtr       pEnt;

    pointer scratchMemBase;
    pointer tsengCPU2ACLBase;
    /* These will hold the ping-pong registers. */
    int tsengFg;
    int tsengBg;
    int tsengPat;
    int tseng_old_dir;
    int old_x;
    int old_y;
    int DGAnumModes;
    Bool DGAactive;
    DGAModePtr DGAModes;
    int	DGAViewportStatus;
    OptionInfoPtr Options;
} TsengRec, *TsengPtr;

#define TsengPTR(p) ((TsengPtr)((p)->driverPrivate))

/* tseng_accel.c */
void tseng_init_acl(ScrnInfoPtr pScrn);
Bool TsengXAAInit(ScreenPtr pScreen);

/* tseng_cursor.c */
Bool TsengHWCursorInit(ScreenPtr pScreen);
void TsengCursorStore(ScrnInfoPtr pScrn, TsengRegPtr Reg);
void TsengCursorRestore(ScrnInfoPtr pScrn, TsengRegPtr Reg);

/* tseng_dga.c */
Bool TsengDGAInit(ScreenPtr pScreen);

/* some IO abstractions
 * May seem daft when you're worked in on this driver but it makes all the
 * difference when coming back after a while or when new to it.
 */
void vgaHWWriteBank(vgaHWPtr hwp, CARD8 value);
CARD8 vgaHWReadBank(vgaHWPtr hwp);
void vgaHWWriteSegment(vgaHWPtr hwp, CARD8 value);
CARD8 vgaHWReadSegment(vgaHWPtr hwp);
void vgaHWWriteModeControl(vgaHWPtr hwp, CARD8 value);
void vgaHWHerculesSecondPage(vgaHWPtr hwp, Bool Enable);
CARD8 ET6000IORead(TsengPtr pTseng, CARD8 Offset);
void ET6000IOWrite(TsengPtr pTseng, CARD8 Offset, CARD8 Value);

/* tseng_mode.c */
Bool TsengRAMDACProbe(ScrnInfoPtr pScrn);
void TsengSetupClockRange(ScrnInfoPtr pScrn);
Bool TsengModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
void TsengAdjustFrame(ADJUST_FRAME_ARGS_DECL);
ModeStatus TsengValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode, Bool verbose, int flags);
void TsengSave(ScrnInfoPtr pScrn);
void TsengRestore(ScrnInfoPtr pScrn, vgaRegPtr vgaReg, TsengRegPtr tsengReg, int flags);
void TsengHVSyncDPMSSet(ScrnInfoPtr pScrn, int PowerManagementMode, int flags);
void TsengCrtcDPMSSet(ScrnInfoPtr pScrn, int PowerManagementMode, int flags);

/*
 * For debugging
 */

#undef TSENG_DEBUG

#ifdef TSENG_DEBUG
#define PDEBUG(arg) do { ErrorF(arg); } while (0)
#else
#define PDEBUG(arg) do {} while (0)
#endif

#endif
