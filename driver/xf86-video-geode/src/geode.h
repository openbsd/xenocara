 /*
  * (c) 2006 Advanced Micro Devices, Inc.
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
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  * DEALINGS IN THE SOFTWARE.
  *
  * Neither the name of the Advanced Micro Devices, Inc. nor the names of its
  * contributors may be used to endorse or promote products derived from this
  * software without specific prior written permission.
  */

#ifndef _GEODE_H_
#define _GEODE_H_

#include "geode_pcirename.h"

#include "xaa.h"
#include "exa.h"
#include "xf86Cursor.h"

#include "vgaHW.h"
#include "xf86int10.h"
#include <X11/extensions/randr.h>
#include "randrstr.h"

#include "xf86xv.h"

/* We only support EXA version >=2 */

#if (EXA_VERSION_MAJOR >= 2)
#ifndef XF86EXA
#define XF86EXA 1
#endif
#else
#undef XF86EXA
#endif

#define CFB 0
#define INT10_SUPPORT 1

/* Existing Processor Models */
#define GX1     0x01
#define GX      0x10
#define LX      0x20

#define PCI_VENDOR_ID_CYRIX  0x1078
#define PCI_VENDOR_ID_NS     0x100B
#define PCI_VENDOR_ID_AMD    0x1022

#define PCI_CHIP_5530     0x0104
#define PCI_CHIP_SC1200   0x0504
#define PCI_CHIP_SC1400   0x0104
#define PCI_CHIP_GEODEGX  0x0030
#define PCI_CHIP_GEODELX  0x2081

#define GFX_CPU_GEODELX   4

#ifdef HAVE_GX
#define GX_FILL_RECT_SUPPORT 1
#define GX_BRES_LINE_SUPPORT 1
#define GX_DASH_LINE_SUPPORT 0	       /* does not do dashed lines */
#define GX_MONO_8X8_PAT_SUPPORT 1
#define GX_CLREXP_8X8_PAT_SUPPORT 1
#define GX_SCR2SCREXP_SUPPORT 1
#define GX_SCR2SCRCPY_SUPPORT 1
#define GX_CPU2SCREXP_SUPPORT 1
#define GX_SCANLINE_SUPPORT 1
#define GX_USE_OFFSCRN_MEM 0
#define GX_ONE_LINE_AT_A_TIME 1
#define GX_WRITE_PIXMAP_SUPPORT 1

#define GFX(func) gfx_##func
#define GFX2(func) gfx2_##func
#endif

#ifdef HAVE_LX
#define LX_FILL_RECT_SUPPORT 1
#define LX_BRES_LINE_SUPPORT 1
#define LX_DASH_LINE_SUPPORT 1
#define LX_MONO_8X8_PAT_SUPPORT 1
#define LX_CLREXP_8X8_PAT_SUPPORT 1
#define LX_SCR2SCREXP_SUPPORT 1
#define LX_SCR2SCRCPY_SUPPORT 1
#define LX_CPU2SCREXP_SUPPORT 1
#define LX_SCANLINE_SUPPORT 1
#define LX_USE_OFFSCRN_MEM 1
#define LX_WRITE_PIXMAP_SUPPORT 1
#endif

#define DEBUGTIM 0
#define DEBUGLVL 0

#ifndef DEBUGTIM
#define DEBUGTIM 0
#endif
#ifndef DEBUGLVL
#define DEBUGLVL 0
#endif

#define GEODEPTR(p) ((GeodeRec *)((p)->driverPrivate))

#define DEFAULT_IMG_LINE_BUFS 20
#define DEFAULT_CLR_LINE_BUFS 20
#define DEFAULT_EXA_SCRATCH_BFRSZ 0x40000

/* 64x64 cursor used as 32x32 */
#define HW_CURSOR_W 64
#define HW_CURSOR_H 64

#define MYDBG 1
#if defined(MYDBG)
#define DEBUGMSG(cond, drv_msg) if((cond)) xf86DrvMsg drv_msg
#else
#define DEBUGMSG(cond, drv_msg)
#endif

#ifdef HAVE_LX
extern unsigned char cim_inb(unsigned short port);
extern void cim_outb(unsigned short port, unsigned char data);
extern unsigned short cim_inw(unsigned short port);
extern void cim_outw(unsigned short port, unsigned short data);
extern unsigned long cim_ind(unsigned short port);
extern void cim_outd(unsigned short port, unsigned long data);

#include "cim_rtns.h"

#define CIM_CMD_BFR_SZ 0x200000L
#define CIM_CMD_BFR_MIN 0x20000L

typedef struct tag_CIM_DISPLAYTIMING
{
    unsigned short wPitch;
    unsigned short wBpp;

    VG_DISPLAY_MODE vgDisplayMode;
}
CIM_DISPLAYTIMING;
#endif

#ifdef HAVE_GX
extern void gfx_write_reg32(int offset, int data);
extern void gfx_write_reg16(int offset, short data);
extern void gfx_write_reg8(int offset, char data);
extern int gfx_read_reg32(int offset);
extern short gfx_read_reg16(int offset);
extern void gfx_write_vid32(int offset, int data);
extern int gfx_read_vid32(int offset);
extern unsigned char gfx_inb(unsigned short port);
extern void gfx_outb(unsigned short port, unsigned char data);
extern unsigned short gfx_inw(unsigned short port);
extern void gfx_outw(unsigned short port, unsigned short data);
extern unsigned long gfx_ind(unsigned short port);
extern void gfx_outd(unsigned short port, unsigned long data);

#define Q_WORD QQ_WORD
#include "gfx_rtns.h"
#undef Q_WORD
#include "panel.h"

typedef struct __TVPARAMS
{
    unsigned int dwFlags;
    unsigned short wWidth;
    unsigned short wHeight;
    unsigned short wStandard;
    unsigned short wType;
    unsigned short wOutput;
    unsigned short wResolution;
    Bool bState;
} TVPARAMS;

typedef struct tag_GFX_DISPLAYTIMING
{
    unsigned int dwDotClock;
    unsigned short wPitch;
    unsigned short wBpp;
    unsigned short wHTotal;
    unsigned short wHActive;
    unsigned short wHSyncStart;
    unsigned short wHSyncEnd;
    unsigned short wHBlankStart;
    unsigned short wHBlankEnd;
    unsigned short wVTotal;
    unsigned short wVActive;
    unsigned short wVSyncStart;
    unsigned short wVSyncEnd;
    unsigned short wVBlankStart;
    unsigned short wVBlankEnd;
    unsigned short wPolarity;
} GFX_DISPLAYTIMING;

/* TV Timings */
typedef struct __TVTIMING
{
    unsigned long HorzTim;
    unsigned long HorzSync;
    unsigned long VertSync;
    unsigned long LineEnd;
    unsigned long VertDownscale;
    unsigned long HorzScaling;
    unsigned long TimCtrl1;
    unsigned long TimCtrl2;
    unsigned long Subfreq;
    unsigned long DispPos;
    unsigned long DispSize;
    unsigned long Debug;
    unsigned long DacCtrl;
    unsigned int DotClock;
} TVTIMING;
#endif

typedef struct _VESARec
{
    xf86Int10InfoPtr pInt;
}
VESARec;

#define OUTPUT_PANEL 0x01
#define OUTPUT_CRT   0x02
#define OUTPUT_TV    0x04
#define OUTPUT_VOP   0x08
#define OUTPUT_DCON  0x10

typedef struct _geodeRec
{
    /* Common */

    int Output;			       /* Bitmask indicating the valid output options */

    Bool HWCursor;
    Bool NoAccel;
    Bool useVGA;
    Bool VGAActive;		       /* Flag indicating if LX VGA is active */
    Bool Compression;
    Bool useEXA;

    int rotation;
    int displayWidth;
    Bool starting;
    Bool tryCompression;
    Bool tryHWCursor;
    unsigned int shadowSize;
    unsigned int shadowOffset;

    ExaOffscreenArea *shadowArea;

    DisplayModePtr curMode;
    VG_COMPRESSION_DATA CBData;

    unsigned long CursorStartOffset;
    unsigned int CursorSize;
    xf86CursorInfoPtr CursorInfo;
    int CursorXHot;
    int CursorYHot;

    /* Geometry information */
    unsigned int maxWidth;	       /* Maximum possible width of the screen */
    unsigned int maxHeight;	       /* Maximum possible height of the screen */

    int Pitch;			       /* display FB pitch */

    int displayPitch;		       /* The pitch ofthe visible area */
    int displayOffset;		       /* The offset of the visible area */
    int displaySize;		       /* The size of the visibile area */

    int PanelX;
    int PanelY;

    /* Framebuffer memory */

    unsigned long FBLinearAddr;
    unsigned char *FBBase;
    unsigned int FBAvail;
    unsigned int FBOffset;
    unsigned int FBSize;

    /* Video information */
    int video_x;
    int video_y;
    short video_w;
    short video_h;
    short video_srcw;
    short video_srch;
    short video_dstw;
    short video_dsth;
    int video_id;
    int video_offset;
    ScrnInfoPtr video_scrnptr;
    BOOL OverlayON;
    int videoKey;

    /* EXA structures */

    ExaDriverPtr pExa;
    unsigned int exaBfrOffset;
    unsigned int exaBfrSz;

    /* XAA structures */
    unsigned char **AccelImageWriteBuffers;
    int NoOfImgBuffers;
    unsigned char **AccelColorExpandBuffers;
    int NoOfColorExpandLines;
    XAAInfoRecPtr AccelInfoRec;

    /* Other structures */

    EntityInfoPtr pEnt;
    ScreenBlockHandlerProcPtr BlockHandler;	/* needed for video */
    XF86VideoAdaptorPtr adaptor;

    /* State save structures */

    gfx_vga_struct FBgfxVgaRegs;
    TVTIMING FBtvtiming;
    GFX_DISPLAYTIMING FBgfxdisplaytiming;
    CIM_DISPLAYTIMING FBcimdisplaytiming;

    unsigned int FBTVActive;
    unsigned int FBSupport;
    unsigned long FBDisplayOffset;
    unsigned long PrevDisplayOffset;

    VESARec *vesa;

    int FBCompressionEnable;
    VG_COMPRESSION_DATA FBCBData;
    VG_CURSOR_DATA FBCursor;
    unsigned long FBCompressionOffset;
    unsigned short FBCompressionPitch;
    unsigned short FBCompressionSize;

    /* Save the Cursor offset of the FB */
    unsigned long FBCursorOffset;
    unsigned char FBBIOSMode;

    /* Hooks */

    void (*WritePixmap) (ScrnInfoPtr pScrni, int x, int y, int w, int h,
	unsigned char *src, int srcwidth, int rop,
	unsigned int planemask, int trans, int bpp, int depth);

    void (*PointerMoved) (int index, int x, int y);
    CloseScreenProcPtr CloseScreen;
        Bool(*CreateScreenResources) (ScreenPtr);

    /* LX only */

    unsigned long CmdBfrOffset;
    unsigned long CmdBfrSize;

#ifdef HAVE_TVSUPPORT
    Bool TVSupport;
    int tv_encoder;
    int tv_bus_fmt;
    int tv_flags;
    int tv_601_fmt;
    int tv_601_flags;
    int tv_conversion;
    int tv_vsync_shift;
    int tv_vsync_shift_count;
    int tv_vsync_select;
    int tvox, tvoy;

    TVPARAMS TvParam;

    int TVOx, TVOy, TVOw, TVOh;
    Bool TV_Overscan_On;
#endif

    /* To be killed! */

    int FBVGAActive;
    unsigned int cpySrcOffset;
    int cpySrcPitch, cpySrcBpp;
    int cpyDx, cpyDy;
    unsigned int cmpSrcOffset;
    int cmpSrcPitch, cmpSrcBpp;
    unsigned int cmpSrcFmt, cmpDstFmt;
    int cmpOp;

    Bool Panel;

}
GeodeRec, *GeodePtr;

/* option flags are self-explanatory */
#ifdef HAVE_LX
enum
{
    LX_OPTION_SW_CURSOR,
    LX_OPTION_HW_CURSOR,
    LX_OPTION_NOCOMPRESSION,
    LX_OPTION_NOACCEL,
    LX_OPTION_ACCEL_METHOD,
    LX_OPTION_EXA_SCRATCH_BFRSZ,
    LX_OPTION_TV_SUPPORT,
    LX_OPTION_TV_OUTPUT,
    LX_OPTION_TV_OVERSCAN,
    LX_OPTION_ROTATE,
    LX_OPTION_NOPANEL,
    LX_OPTION_COLOR_KEY,
    LX_OPTION_FBSIZE,
    LX_OPTION_PANEL_GEOMETRY,
    LX_OPTION_DONT_PROGRAM
}
LX_GeodeOpts;
#endif

#ifdef HAVE_GX
enum
{
    GX_OPTION_SW_CURSOR,
    GX_OPTION_HW_CURSOR,
    GX_OPTION_NOCOMPRESSION,
    GX_OPTION_NOACCEL,
    GX_OPTION_ACCEL_METHOD,
    GX_OPTION_EXA_SCRATCH_BFRSZ,
    GX_OPTION_TV_SUPPORT,
    GX_OPTION_TV_OUTPUT,
    GX_OPTION_TV_OVERSCAN,
    GX_OPTION_ROTATE,
    GX_OPTION_NOPANEL,
    GX_OPTION_FLATPANEL_INFO,
    GX_OPTION_FLATPANEL_IN_BIOS,
    GX_OPTION_COLOR_KEY,
    GX_OPTION_OSM,
    GX_OPTION_OSM_IMG_BUFS,
    GX_OPTION_OSM_CLR_BUFS,
    GX_OPTION_FBSIZE,
    GX_OPTION_PANEL_GEOMETRY,
    GX_OPTION_DONT_PROGRAM
}
GX_GeodeOpts;
#endif

/* OLPC defines */

#define DCON_DEFAULT_XRES  1200
#define DCON_DEFAULT_YRES   900
#define DCON_DEFAULT_BPP     16
#define DCON_DEFAULT_REFRESH 50

/* geode_dcon.c */
extern Bool dcon_init(ScrnInfoPtr pScrni);
extern int DCONDPMSSet(ScrnInfoPtr pScrni, int mode, int flags);

/* geode_common.c */

void geode_memory_to_screen_blt(unsigned long, unsigned long,
    unsigned long, unsigned long, long, long, int);
int GeodeGetRefreshRate(DisplayModePtr);
void GeodeCopyGreyscale(unsigned char *, unsigned char *, int, int, int, int);
int GeodeGetSizeFromFB(unsigned int *);

/* gx_video.c */

int
GeodeQueryImageAttributes(ScrnInfoPtr, int id, unsigned short *w,
    unsigned short *h, int *pitches, int *offsets);

Bool RegionsEqual(RegionPtr A, RegionPtr B);

/* gx_driver.c */

void GeodeProbeDDC(ScrnInfoPtr pScrni, int index);
xf86MonPtr GeodeDoDDC(ScrnInfoPtr pScrni, int index);
int GeodeGetFPGeometry(const char *str, int *width, int *height);
void GeodePointerMoved(int index, int x, int y);
void GeodeFreeScreen(int scrnIndex, int flags);
int GeodeCalculatePitchBytes(unsigned int width, unsigned int bpp);
void GXSetupChipsetFPtr(ScrnInfoPtr pScrn);

/* geode_msr.c */
int GeodeReadMSR(unsigned long addr, unsigned long *lo, unsigned long *hi);
int GeodeWriteMSR(unsigned long addr, unsigned long lo, unsigned long hi);

/* gx_cursor.c */
Bool GXHWCursorInit(ScreenPtr pScrn);
void GXLoadCursorImage(ScrnInfoPtr pScrni, unsigned char *src);
void GXHideCursor(ScrnInfoPtr pScrni);
void GXShowCursor(ScrnInfoPtr pScrni);

/* gx_randr.c */
Rotation GXGetRotation(ScreenPtr pScreen);
Bool GXRandRInit(ScreenPtr pScreen, int rotation);
Bool GXRandRSetConfig(ScreenPtr pScreen, Rotation rotation, int rate, RRScreenSizePtr pSize);

/* gx_rotate.c */
Bool GXRotate(ScrnInfoPtr pScrni, DisplayModePtr mode);

/* gx_accel.c */
Bool GXAccelInit(ScreenPtr pScrn);
void GXAccelSync(ScrnInfoPtr pScrni);

/* gx_video.c */
void GXInitVideo(ScreenPtr pScrn);

/* lx_driver.c */
void LXSetupChipsetFPtr(ScrnInfoPtr pScrn);

/* lx_cursor.c */
Bool LXHWCursorInit(ScreenPtr pScrn);
void LXLoadCursorImage(ScrnInfoPtr pScrni, unsigned char *src);
void LXHideCursor(ScrnInfoPtr pScrni);
void LXShowCursor(ScrnInfoPtr pScrni);

/* lx_randr.c */
Rotation LXGetRotation(ScreenPtr pScreen);
Bool LXRandRInit(ScreenPtr pScreen, int rotation);
Bool LXRandRSetConfig(ScreenPtr pScreen, Rotation rotation, int rate, RRScreenSizePtr pSize);

/* lx_rotate.c */
Bool LXSetRotatePitch(ScrnInfoPtr pScrni);
Bool LXRotate(ScrnInfoPtr pScrni, DisplayModePtr mode);
Bool LXAllocShadow(ScrnInfoPtr pScrni);

/* lx_exa.c */
Bool LXExaInit(ScreenPtr pScreen);

/* lx_video.c */
void LXInitVideo(ScreenPtr pScrn);

#endif
