/*
 * Copyright 1999 through 2004 by Marc Aurele La France (TSI @ UQV), tsi@xfree86.org
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of Marc Aurele La France not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Marc Aurele La France makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as-is" without express or implied warranty.
 *
 * MARC AURELE LA FRANCE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO
 * EVENT SHALL MARC AURELE LA FRANCE BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * DRI support by:
 *    Gareth Hughes <gareth@valinux.com>
 *    Leif Delgass <ldelgass@retinalburn.net>
 */

#ifndef ___ATISTRUCT_H___
#define ___ATISTRUCT_H___ 1

#include "atibank.h"
#include "aticlock.h"
#include "atiregs.h"

#ifdef XF86DRI_DEVEL

/*
 * DRI support
 */
#define _XF86DRI_SERVER_
#include "atidripriv.h"
#include "mach64_dri.h"
#include "sarea.h"
#include "xf86drm.h"
#include "dri.h"
 
#endif /* XF86DRI_DEVEL */

#ifdef TV_OUT

#include "vbe.h"

#endif /* TV_OUT */

#include "picturestr.h"
#ifdef USE_EXA
#include "exa.h"
#endif
#ifdef USE_XAA
#include "xaa.h"
#endif
#include "xf86Cursor.h"
#include "xf86Pci.h"
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86Resources.h"
#endif

#include "atipcirename.h"

#include "compat-api.h"

#define CacheSlotOf(____Register) ((____Register) / UnitOf(DWORD_SELECT))

/*
 * This is probably as good a place as any to put this note, as it applies to
 * the entire driver, but especially here.  CARD8's are used rather than the
 * appropriate enum types because the latter would nearly quadruple storage
 * requirements (they are stored as int's).  This reduces the usefulness of
 * enum types to their ability to declare index values.  I've also elected to
 * forgo the strong typing capabilities of enum types.  C is not terribly adept
 * at strong typing anyway.
 */

/* A structure for local data related to video modes */
typedef struct _ATIHWRec
{
    /* Clock number for mode */
    CARD8 clock;

    /* The CRTC used to drive the screen (VGA, 8514, Mach64) */
    CARD8 crtc;

    /* Colour lookup table */
    CARD8 lut[256 * 3];

#ifndef AVOID_CPIO

    /* VGA registers */
    CARD8 genmo, crt[25], seq[5], gra[9], attr[21];

    /* VGA Wonder registers */
    CARD8             a3,         a6, a7,             ab, ac, ad, ae,
          b0, b1, b2, b3,     b5, b6,     b8, b9, ba,         bd, be, bf;

    /* Shadow VGA CRTC registers */
    CARD8 shadow_vga[25];

#endif /* AVOID_CPIO */

    /* Generic DAC registers */
    CARD8 dac_read, dac_write, dac_mask;

    /* IBM RGB 514 registers */
    CARD8 ibmrgb514[0x0092U];   /* All that's needed for now */

    /* Mach64 PLL registers */
    CARD8 pll_vclk_cntl, pll_vclk_post_div,
          pll_vclk0_fb_div, pll_vclk1_fb_div,
          pll_vclk2_fb_div, pll_vclk3_fb_div,
          pll_xclk_cntl, pll_ext_vpll_cntl;

    /* Mach64 CPIO registers */
    CARD32 crtc_h_total_disp, crtc_h_sync_strt_wid,
           crtc_v_total_disp, crtc_v_sync_strt_wid,
           crtc_off_pitch, crtc_gen_cntl, dsp_config, dsp_on_off, mem_buf_cntl,
           ovr_clr, ovr_wid_left_right, ovr_wid_top_bottom,
           cur_clr0, cur_clr1, cur_offset,
           cur_horz_vert_posn, cur_horz_vert_off,
           clock_cntl, bus_cntl, mem_cntl, mem_vga_wp_sel, mem_vga_rp_sel,
           dac_cntl, gen_test_cntl, config_cntl, mpp_config, mpp_strobe_seq,
           tvo_cntl;

    /* LCD registers */
    CARD32 lcd_index, config_panel, lcd_gen_ctrl,
           horz_stretching, vert_stretching, ext_vert_stretch;

    /* Shadow Mach64 CRTC registers */
    CARD32 shadow_h_total_disp, shadow_h_sync_strt_wid,
           shadow_v_total_disp, shadow_v_sync_strt_wid;

    /* Mach64 MMIO Block 0 registers and related subfields */
    CARD32 dst_off_pitch;
    CARD16 dst_x, dst_y, dst_height;
    CARD32 dst_bres_err, dst_bres_inc, dst_bres_dec, dst_cntl;
    CARD32 src_off_pitch;
    CARD16 src_x, src_y, src_width1, src_height1,
           src_x_start, src_y_start, src_width2, src_height2;
    CARD32 src_cntl;
    CARD32 host_cntl;
    CARD32 pat_reg0, pat_reg1, pat_cntl;
    CARD16 sc_left, sc_right, sc_top, sc_bottom;
    CARD32 dp_bkgd_clr, dp_frgd_clr, dp_write_mask, dp_chain_mask,
           dp_pix_width, dp_mix, dp_src;
    CARD32 clr_cmp_clr, clr_cmp_msk, clr_cmp_cntl;
    CARD32 context_mask, context_load_cntl;

    CARD32 scale_3d_cntl, tex_size_pitch, tex_cntl, tex_offset;

    /* Mach64 MMIO Block 1 registers */
    CARD32 overlay_y_x_start, overlay_y_x_end, overlay_graphics_key_clr,
           overlay_graphics_key_msk, overlay_key_cntl, overlay_scale_inc,
           overlay_scale_cntl, scaler_height_width, scaler_test,
           scaler_buf0_offset, scaler_buf1_offset, scaler_buf_pitch,
           video_format, overlay_exclusive_horz, overlay_exclusive_vert,
           buf0_offset, buf0_pitch, buf1_offset, buf1_pitch,
           scaler_colour_cntl, scaler_h_coeff0, scaler_h_coeff1,
           scaler_h_coeff2, scaler_h_coeff3, scaler_h_coeff4, gui_cntl,
           scaler_buf0_offset_u, scaler_buf0_offset_v, scaler_buf1_offset_u,
           scaler_buf1_offset_v;

    /* Clock programming data */
    int FeedbackDivider, ReferenceDivider, PostDivider;

#ifndef AVOID_CPIO

    /* This is used by ATISwap() */
    pointer frame_buffer;
    ATIBankProcPtr SetBank;
    unsigned int nBank, nPlane;

#endif /* AVOID_CPIO */

} ATIHWRec;

#ifdef USE_EXA
/*
 * Card engine state for communication across RENDER acceleration hooks.
 */
typedef struct _Mach64ContextRegs3D
{
    CARD32	dp_mix;
    CARD32	dp_src;
    CARD32	dp_write_mask;
    CARD32	dp_pix_width;
    CARD32	dst_pitch_offset;

    CARD32	scale_3d_cntl;

    CARD32	tex_cntl;
    CARD32	tex_size_pitch;
    CARD32	tex_offset;

    int		tex_width;	/* src/mask texture width (pixels) */
    int		tex_height;	/* src/mask texture height (pixels) */

    Bool	frag_src;	/* solid src uses fragment color */
    Bool	frag_mask;	/* solid mask uses fragment color */
    CARD32	frag_color;	/* solid src/mask color */

    Bool	color_alpha;	/* the alpha value is contained in the color
				   channels instead of the alpha channel */

    PictTransform *transform;
} Mach64ContextRegs3D;
#endif /* USE_EXA */

/*
 * This structure defines the driver's private area.
 */
typedef struct _ATIRec
{

#ifndef AVOID_CPIO

    CARD8 VGAAdapter;

#endif /* AVOID_CPIO */

    /*
     * Chip-related definitions.
     */
    CARD32 config_chip_id;
    CARD16 ChipType;
    CARD8 Chip;
    CARD8 ChipClass, ChipRevision, ChipRev, ChipVersion, ChipFoundry;

    /*
     * Processor I/O decoding definitions.
     */
    CARD8 CPIODecoding;
    unsigned long CPIOBase;

#ifndef AVOID_CPIO

    /*
     * Processor I/O port definition for VGA.
     */
    unsigned long CPIO_VGABase;

    /*
     * Processor I/O port definitions for VGA Wonder.
     */
    unsigned long CPIO_VGAWonder;

#endif /* AVOID_CPIO */

    /*
     * DAC-related definitions.
     */

#ifndef AVOID_CPIO

    unsigned long CPIO_DAC_MASK, CPIO_DAC_DATA, CPIO_DAC_READ, CPIO_DAC_WRITE,
              CPIO_DAC_WAIT;

#endif /* AVOID_CPIO */

    CARD16 DAC;
    CARD8 rgbBits;

    /*
     * Definitions related to system bus interface.
     */
    pciVideoPtr PCIInfo;
    CARD8 BusType;

#ifndef AVOID_CPIO
#ifndef XSERVER_LIBPCIACCESS
     resRange VGAWonderResources[2];
#endif
#endif /* AVOID_CPIO */

    /*
     * Definitions related to video memory.
     */
    CARD8 MemoryType;
    int VideoRAM;

    /*
     * BIOS-related definitions.
     */
    CARD8 I2CType, Tuner, Decoder, Audio;

    /*
     * Definitions related to video memory apertures.
     */
    pointer pMemory, pShadow;
    pointer pMemoryLE;          /* Always little-endian */
    unsigned long LinearBase;
    int LinearSize, FBPitch, FBBytesPerPixel;

#ifndef AVOID_CPIO

    /*
     * Banking interface.
     */
    pointer pBank;

#endif /* AVOID_CPIO */

    /*
     * Definitions related to MMIO register apertures.
     */
    pointer pMMIO, pBlock[2];
    unsigned long Block0Base, Block1Base;

    /*
     * XAA interface.
     */
    Bool useEXA;
#ifdef USE_EXA
    ExaDriverPtr pExa;
#endif
#ifdef USE_XAA
    XAAInfoRecPtr pXAAInfo;
#endif
    int nAvailableFIFOEntries, nFIFOEntries, nHostFIFOEntries;
    CARD8 EngineIsBusy, EngineIsLocked, XModifier;
    CARD32 dst_cntl;    /* For SetupFor/Subsequent communication */
    CARD32 sc_left_right, sc_top_bottom;
    CARD16 sc_left, sc_right, sc_top, sc_bottom;        /* Current scissors */
    pointer pHOST_DATA; /* Current HOST_DATA_* transfer window address */
#ifdef USE_XAA
    CARD32 *ExpansionBitmapScanlinePtr[2];
    int ExpansionBitmapWidth;
#endif
#ifdef USE_EXA
    Bool RenderAccelEnabled;
    Mach64ContextRegs3D m3d;
#endif

    /*
     * Cursor-related definitions.
     */
    xf86CursorInfoPtr pCursorInfo;
    pointer pCursorPage, pCursorImage;
    unsigned long CursorBase;
    CARD32 CursorOffset;
    CARD16 CursorXOffset, CursorYOffset;
    CARD8 Cursor;

    /*
     * MMIO cache.
     */
    CARD32 MMIOCache[CacheSlotOf(DWORD_SELECT) + 1];
    CARD8  MMIOCached[(CacheSlotOf(DWORD_SELECT) + 8) >> 3];

    /*
     * Clock-related definitions.
     */
    int refclk;
    int ClockNumberToProgramme, ReferenceNumerator, ReferenceDenominator;
    int ProgrammableClock, maxClock;
    ClockRec ClockDescriptor;

    /*
     * DSP register data.
     */
    int XCLKFeedbackDivider, XCLKReferenceDivider, XCLKPostDivider;
    CARD16 XCLKMaxRASDelay, XCLKPageFaultDelay,
           DisplayLoopLatency, DisplayFIFODepth;

    /*
     * LCD panel data.
     */
    int LCDPanelID, LCDClock, LCDHorizontal, LCDVertical;
    unsigned LCDHSyncStart, LCDHSyncWidth, LCDHBlankWidth;
    unsigned LCDVSyncStart, LCDVSyncWidth, LCDVBlankWidth;
    int LCDVBlendFIFOSize;

    /*
     * Data used by ATIAdjustFrame().
     */
    int AdjustDepth, AdjustMaxX, AdjustMaxY;
    unsigned long AdjustMask, AdjustMaxBase;

    /*
     * DGA and non-DGA common data.
     */
    DisplayModePtr currentMode;
    CARD8 depth, bitsPerPixel;
    short int displayWidth;
    rgb weight;

#ifndef AVOID_DGA

    /*
     * DGA-related data.
     */
    DGAModePtr pDGAMode;
    DGAFunctionRec ATIDGAFunctions;
    int nDGAMode;

    /*
     * XAAForceTransBlit alters the behavior of 'SetupForScreenToScreenCopy',
     * such that ~0 is interpreted as a legitimate transparency key.
     */
    CARD8 XAAForceTransBlit;

#endif /* AVOID_DGA */

    /*
     * XVideo-related data.
     */
    DevUnion XVPortPrivate[1];
    pointer pXVBuffer;		/* USE_EXA: ExaOffscreenArea*
				   USE_XAA: FBLinearPtr */
    RegionRec VideoClip;
    int SurfacePitch, SurfaceOffset;
    CARD8 AutoPaint, DoubleBuffer, CurrentBuffer, ActiveSurface;

    /*
     * Data saved by ATIUnlock() and restored by ATILock().
     */
    struct
    {
        /* Mach64 registers */
        CARD32 crtc_int_cntl, crtc_gen_cntl, i2c_cntl_0, hw_debug,
               scratch_reg3, bus_cntl, lcd_index, mem_cntl, i2c_cntl_1,
               dac_cntl, gen_test_cntl, mpp_config, mpp_strobe_seq, tvo_cntl;

#ifndef AVOID_CPIO

        CARD32 config_cntl;

        /* VGA Wonder registers */
        CARD8 a6, ab, b1, b4, b5, b6, b8, b9, be;

        /* VGA registers */
        CARD8 crt03, crt11;

        /* VGA shadow registers */
        CARD8 shadow_crt03, shadow_crt11;

#endif /* AVOID_CPIO */

    } LockData;

    /* Mode data */
    ATIHWRec OldHW, NewHW;

    /*
     * Resource Access Control entity index.
     */
    int iEntity;

    /*
     * Driver options.
     */
    unsigned int OptionProbeSparse:1;  /* Force probe for fixed (sparse) I/O */
    unsigned int OptionAccel:1;        /* Use hardware draw engine */
    unsigned int OptionBIOSDisplay:1;  /* Allow BIOS interference */
    unsigned int OptionBlend:1;        /* Force horizontal blending */
    unsigned int OptionCRTDisplay:1;   /* Display on both CRT & DFP */
    unsigned int OptionCSync:1;        /* Use composite sync */
    unsigned int OptionDevel:1;        /* Intentionally undocumented */

#ifdef TV_OUT

    CARD8 OptionTvOut;          /* Enable TV out if TV is connected */
    CARD8 OptionTvStd;          /* Requested TV standard - see ATITVStandard enum in atioption.h */

#endif /* TV_OUT */

    unsigned int OptionMMIOCache:1;    /* Cache MMIO writes */
    unsigned int OptionTestMMIOCache:1;/* Test MMIO cache integrity */
    unsigned int OptionPanelDisplay:1; /* Prefer digital panel over CRT */
    unsigned int OptionShadowFB:1;     /* Use shadow frame buffer */
    unsigned int OptionLCDSync:1;      /* Temporary */

    /*
     * State flags.
     */
    CARD8 Unlocked, Mapped, Closeable;
    CARD8 MMIOInLinear;

    /*
     * Wrapped functions.
     */
    CloseScreenProcPtr CloseScreen;

#ifdef XF86DRI_DEVEL

    /*
     * DRI data.
     */
    int directRenderingEnabled;
    DRIInfoPtr pDRIInfo;
    int drmFD;
    int irq;
    ATIDRIServerInfoPtr pDRIServerInfo;
    Bool NeedDRISync;
    Bool have3DWindows;
                                                                                
    /* offscreen memory management */
#ifdef USE_XAA
    int               backLines;
    FBAreaPtr         backArea;
    int               depthTexLines;
    FBAreaPtr         depthTexArea;
#endif
    CARD8 OptionIsPCI;           /* Force PCI mode */
    CARD8 OptionDMAMode;         /* async, sync, mmio */
    CARD8 OptionAGPMode;         /* AGP mode */
    CARD8 OptionAGPSize;         /* AGP size in MB */
    CARD8 OptionLocalTextures;   /* Use local textures + AGP textures (only valid for AGP) */
    CARD8 OptionBufferSize;      /* Command/dma buffer size in MB */

#endif /* XF86DRI_DEVEL */

#ifdef TV_OUT
    /* TV out */
    vbeInfoPtr pVBE;
    xf86Int10InfoPtr pInt10;
    int vbemode; /* saved text mode */
    Bool tvActive;
#endif /* TV_OUT */
} ATIRec;

#define ATIPTR(_p) ((ATIPtr)((_p)->driverPrivate))

#endif /* ___ATISTRUCT_H___ */
