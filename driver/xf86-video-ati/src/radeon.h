/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/ati/radeon.h,v 1.43 2003/11/06 18:38:00 tsi Exp $ */
/*
 * Copyright 2000 ATI Technologies Inc., Markham, Ontario, and
 *                VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *   Rickard E. Faith <faith@valinux.com>
 *   Alan Hourihane <alanh@fairlite.demon.co.uk>
 *
 */

#ifndef _RADEON_H_
#define _RADEON_H_

#include <stdlib.h>		/* For abs() */
#include <unistd.h>		/* For usleep() */

#include "xf86str.h"
#include "compiler.h"
#include "xf86fbman.h"

				/* PCI support */
#include "xf86Pci.h"

#ifdef USE_EXA
#include "exa.h"
#endif
#ifdef USE_XAA
#include "xaa.h"
#endif

				/* Exa and Cursor Support */
#include "vbe.h"
#include "xf86Cursor.h"

				/* DDC support */
#include "xf86DDC.h"

				/* Xv support */
#include "xf86xv.h"

#include "radeon_probe.h"
				/* DRI support */
#ifdef XF86DRI
#define _XF86DRI_SERVER_
#include "radeon_dripriv.h"
#include "dri.h"
#include "GL/glxint.h"
#endif

				/* Render support */
#ifdef RENDER
#include "picturestr.h"
#endif

typedef enum {
    OPTION_NOACCEL,
    OPTION_SW_CURSOR,
    OPTION_DAC_6BIT,
    OPTION_DAC_8BIT,
#ifdef XF86DRI
    OPTION_BUS_TYPE,
    OPTION_CP_PIO,
    OPTION_USEC_TIMEOUT,
    OPTION_AGP_MODE,
    OPTION_AGP_FW,
    OPTION_GART_SIZE,
    OPTION_GART_SIZE_OLD,
    OPTION_RING_SIZE,
    OPTION_BUFFER_SIZE,
    OPTION_DEPTH_MOVE,
    OPTION_PAGE_FLIP,
    OPTION_NO_BACKBUFFER,
    OPTION_XV_DMA,
    OPTION_FBTEX_PERCENT,
    OPTION_DEPTH_BITS,
#ifdef USE_EXA
    OPTION_ACCEL_DFS,
#endif
#endif
    OPTION_PANEL_OFF,
    OPTION_DDC_MODE,
    OPTION_MONITOR_LAYOUT,
    OPTION_IGNORE_EDID,
    OPTION_FBDEV,
    OPTION_MERGEDFB,
    OPTION_CRT2HSYNC,
    OPTION_CRT2VREFRESH,
    OPTION_CRT2POS,
    OPTION_METAMODES,
    OPTION_MERGEDDPI,
    OPTION_RADEONXINERAMA,
    OPTION_CRT2ISSCRN0,
    OPTION_MERGEDFBNONRECT,
    OPTION_MERGEDFBMOUSER,
    OPTION_DISP_PRIORITY,
    OPTION_PANEL_SIZE,
    OPTION_MIN_DOTCLOCK,
    OPTION_COLOR_TILING,
#ifdef XvExtension
    OPTION_VIDEO_KEY,
    OPTION_RAGE_THEATRE_CRYSTAL,
    OPTION_RAGE_THEATRE_TUNER_PORT,
    OPTION_RAGE_THEATRE_COMPOSITE_PORT,
    OPTION_RAGE_THEATRE_SVIDEO_PORT,
    OPTION_TUNER_TYPE,
    OPTION_RAGE_THEATRE_MICROC_PATH,
    OPTION_RAGE_THEATRE_MICROC_TYPE,
#endif
#ifdef RENDER
    OPTION_RENDER_ACCEL,
    OPTION_SUBPIXEL_ORDER,
#endif
    OPTION_SHOWCACHE,
    OPTION_DYNAMIC_CLOCKS,
    OPTION_BIOS_HOTKEYS,
    OPTION_VGA_ACCESS,
    OPTION_REVERSE_DDC,
    OPTION_LVDS_PROBE_PLL,
    OPTION_ACCELMETHOD,
    OPTION_CONSTANTDPI,
#ifdef __powerpc__
    OPTION_IBOOKHACKS
#endif
} RADEONOpts;

/* ------- mergedfb support ------------- */
		/* Psuedo Xinerama support */
#define NEED_REPLIES  		/* ? */
#define EXTENSION_PROC_ARGS void *
#include "extnsionst.h"  	/* required */
#include <X11/extensions/panoramiXproto.h>  	/* required */
#define RADEON_XINERAMA_MAJOR_VERSION  1
#define RADEON_XINERAMA_MINOR_VERSION  1


/* Relative merge position */
typedef enum {
   radeonLeftOf,
   radeonRightOf,
   radeonAbove,
   radeonBelow,
   radeonClone
} RADEONScrn2Rel;

typedef struct _region {
    int x0,x1,y0,y1;
} region;

/* ------------------------------------- */

#define RADEON_DEBUG            1 /* Turn off debugging output               */
#define RADEON_IDLE_RETRY      16 /* Fall out of idle loops after this count */
#define RADEON_TIMEOUT    2000000 /* Fall out of wait loops after this count */

/* Buffer are aligned on 4096 byte boundaries */
#define RADEON_BUFFER_ALIGN 0x00000fff
#define RADEON_VBIOS_SIZE 0x00010000
#define RADEON_USE_RMX 0x80000000 /* mode flag for using RMX
				   * Need to comfirm this is not used
				   * for something else.
				   */

#if RADEON_DEBUG
#define RADEONTRACE(x)						\
do {									\
    ErrorF("(**) %s(%d): ", RADEON_NAME, pScrn->scrnIndex);		\
    ErrorF x;								\
} while(0)
#else
#define RADEONTRACE(x) do { } while(0)
#endif


/* Other macros */
#define RADEON_ARRAY_SIZE(x)  (sizeof(x)/sizeof(x[0]))
#define RADEON_ALIGN(x,bytes) (((x) + ((bytes) - 1)) & ~((bytes) - 1))
#define RADEONPTR(pScrn)      ((RADEONInfoPtr)(pScrn)->driverPrivate)


typedef struct {
				/* Common registers */
    CARD32            ovr_clr;
    CARD32            ovr_wid_left_right;
    CARD32            ovr_wid_top_bottom;
    CARD32            ov0_scale_cntl;
    CARD32            mpp_tb_config;
    CARD32            mpp_gp_config;
    CARD32            subpic_cntl;
    CARD32            viph_control;
    CARD32            i2c_cntl_1;
    CARD32            gen_int_cntl;
    CARD32            cap0_trig_cntl;
    CARD32            cap1_trig_cntl;
    CARD32            bus_cntl;
    CARD32            bios_4_scratch;
    CARD32            bios_5_scratch;
    CARD32            bios_6_scratch;
    CARD32            surface_cntl;
    CARD32            surfaces[8][3];
    CARD32            mc_agp_location;
    CARD32            mc_fb_location;
    CARD32            display_base_addr;
    CARD32            display2_base_addr;
    CARD32            ov0_base_addr;

				/* Other registers to save for VT switches */
    CARD32            dp_datatype;
    CARD32            rbbm_soft_reset;
    CARD32            clock_cntl_index;
    CARD32            amcgpio_en_reg;
    CARD32            amcgpio_mask;

				/* CRTC registers */
    CARD32            crtc_gen_cntl;
    CARD32            crtc_ext_cntl;
    CARD32            dac_cntl;
    CARD32            crtc_h_total_disp;
    CARD32            crtc_h_sync_strt_wid;
    CARD32            crtc_v_total_disp;
    CARD32            crtc_v_sync_strt_wid;
    CARD32            crtc_offset;
    CARD32            crtc_offset_cntl;
    CARD32            crtc_pitch;
    CARD32            disp_merge_cntl;
    CARD32            grph_buffer_cntl;
    CARD32            crtc_more_cntl;

				/* CRTC2 registers */
    CARD32            crtc2_gen_cntl;

    CARD32            dac2_cntl;
    CARD32            disp_output_cntl;
    CARD32            disp_hw_debug;
    CARD32            disp2_merge_cntl;
    CARD32            grph2_buffer_cntl;
    CARD32            crtc2_h_total_disp;
    CARD32            crtc2_h_sync_strt_wid;
    CARD32            crtc2_v_total_disp;
    CARD32            crtc2_v_sync_strt_wid;
    CARD32            crtc2_offset;
    CARD32            crtc2_offset_cntl;
    CARD32            crtc2_pitch;
				/* Flat panel registers */
    CARD32            fp_crtc_h_total_disp;
    CARD32            fp_crtc_v_total_disp;
    CARD32            fp_gen_cntl;
    CARD32            fp2_gen_cntl;
    CARD32            fp_h_sync_strt_wid;
    CARD32            fp2_h_sync_strt_wid;
    CARD32            fp_horz_stretch;
    CARD32            fp_panel_cntl;
    CARD32            fp_v_sync_strt_wid;
    CARD32            fp2_v_sync_strt_wid;
    CARD32            fp_vert_stretch;
    CARD32            lvds_gen_cntl;
    CARD32            lvds_pll_cntl;
    CARD32            tmds_pll_cntl;
    CARD32            tmds_transmitter_cntl;

				/* Computed values for PLL */
    CARD32            dot_clock_freq;
    CARD32            pll_output_freq;
    int               feedback_div;
    int               post_div;

				/* PLL registers */
    unsigned          ppll_ref_div;
    unsigned          ppll_div_3;
    CARD32            htotal_cntl;

				/* Computed values for PLL2 */
    CARD32            dot_clock_freq_2;
    CARD32            pll_output_freq_2;
    int               feedback_div_2;
    int               post_div_2;

				/* PLL2 registers */
    CARD32            p2pll_ref_div;
    CARD32            p2pll_div_0;
    CARD32            htotal_cntl2;

				/* Pallet */
    Bool              palette_valid;
    CARD32            palette[256];
    CARD32            palette2[256];

    CARD32            tv_dac_cntl;

} RADEONSaveRec, *RADEONSavePtr;

typedef struct {
    CARD16            reference_freq;
    CARD16            reference_div;
    CARD32            min_pll_freq;
    CARD32            max_pll_freq;
    CARD16            xclk;
} RADEONPLLRec, *RADEONPLLPtr;

typedef struct {
    int               bitsPerPixel;
    int               depth;
    int               displayWidth;
    int               displayHeight;
    int               pixel_code;
    int               pixel_bytes;
    DisplayModePtr    mode;
} RADEONFBLayout;

typedef enum {
    CHIP_FAMILY_UNKNOW,
    CHIP_FAMILY_LEGACY,
    CHIP_FAMILY_RADEON,
    CHIP_FAMILY_RV100,
    CHIP_FAMILY_RS100,    /* U1 (IGP320M) or A3 (IGP320)*/
    CHIP_FAMILY_RV200,
    CHIP_FAMILY_RS200,    /* U2 (IGP330M/340M/350M) or A4 (IGP330/340/345/350), RS250 (IGP 7000) */
    CHIP_FAMILY_R200,
    CHIP_FAMILY_RV250,
    CHIP_FAMILY_RS300,    /* RS300/RS350 */
    CHIP_FAMILY_RV280,
    CHIP_FAMILY_R300,
    CHIP_FAMILY_R350,
    CHIP_FAMILY_RV350,
    CHIP_FAMILY_RV380,    /* RV370/RV380/M22/M24 */
    CHIP_FAMILY_R420,     /* R420/R423/M18 */
    CHIP_FAMILY_RV410,    /* RV410, M26 */
    CHIP_FAMILY_RS400,    /* xpress 200, 200m (RS400/410/480) */
    CHIP_FAMILY_LAST
} RADEONChipFamily;

#define IS_RV100_VARIANT ((info->ChipFamily == CHIP_FAMILY_RV100)  ||  \
        (info->ChipFamily == CHIP_FAMILY_RV200)  ||  \
        (info->ChipFamily == CHIP_FAMILY_RS100)  ||  \
        (info->ChipFamily == CHIP_FAMILY_RS200)  ||  \
        (info->ChipFamily == CHIP_FAMILY_RV250)  ||  \
        (info->ChipFamily == CHIP_FAMILY_RV280)  ||  \
        (info->ChipFamily == CHIP_FAMILY_RS300))


#define IS_R300_VARIANT ((info->ChipFamily == CHIP_FAMILY_R300)  ||  \
        (info->ChipFamily == CHIP_FAMILY_RV350) ||  \
        (info->ChipFamily == CHIP_FAMILY_R350)  ||  \
        (info->ChipFamily == CHIP_FAMILY_RV380) ||  \
        (info->ChipFamily == CHIP_FAMILY_R420)  ||  \
        (info->ChipFamily == CHIP_FAMILY_RV410) ||  \
        (info->ChipFamily == CHIP_FAMILY_RS400))

/*
 * Errata workarounds
 */
typedef enum {
       CHIP_ERRATA_R300_CG             = 0x00000001,
       CHIP_ERRATA_PLL_DUMMYREADS      = 0x00000002,
       CHIP_ERRATA_PLL_DELAY           = 0x00000004
} RADEONErrata;

typedef enum {
	CARD_PCI,
	CARD_AGP,
	CARD_PCIE
} RADEONCardType;

typedef struct {
    CARD32 freq;
    CARD32 value;
}RADEONTMDSPll;

typedef struct {
    EntityInfoPtr     pEnt;
    pciVideoPtr       PciInfo;
    PCITAG            PciTag;
    int               Chipset;
    RADEONChipFamily  ChipFamily;
    RADEONErrata      ChipErrata;

    Bool              FBDev;

    unsigned long     LinearAddr;       /* Frame buffer physical address     */
    unsigned long     MMIOAddr;         /* MMIO region physical address      */
    unsigned long     BIOSAddr;         /* BIOS physical address             */
    CARD32            fbLocation;
    CARD32            gartLocation;
    CARD32            mc_fb_location;
    CARD32            mc_agp_location;

    unsigned char     *MMIO;            /* Map of MMIO region                */
    unsigned char     *FB;              /* Map of frame buffer               */
    CARD8             *VBIOS;           /* Video BIOS pointer                */

    Bool              IsAtomBios;       /* New BIOS used in R420 etc.        */
    int               ROMHeaderStart;   /* Start of the ROM Info Table       */
    int               MasterDataStart;  /* Offset for Master Data Table for ATOM BIOS */

    CARD32            MemCntl;
    CARD32            BusCntl;
    unsigned long     MMIOSize;         /* MMIO region physical address      */
    unsigned long     FbMapSize;        /* Size of frame buffer, in bytes    */
    unsigned long     FbSecureSize;     /* Size of secured fb area at end of
                                           framebuffer */
    int               Flags;            /* Saved copy of mode flags          */

				/* VE/M6 support */
    RADEONMonitorType DisplayType;      /* Monitor connected on              */
    RADEONDDCType     DDCType;
    RADEONConnectorType ConnectorType;
    Bool              HasCRTC2;         /* All cards except original Radeon  */
    Bool              IsMobility;       /* Mobile chips for laptops */
    Bool              IsIGP;            /* IGP chips */
    Bool              HasSingleDAC;     /* only TVDAC on chip */
    Bool              IsSecondary;      /* Second Screen                     */
    Bool	      IsPrimary;        /* Primary Screen */
    Bool              IsSwitching;      /* Flag for switching mode           */
    Bool              OverlayOnCRTC2;
    Bool              PanelOff;         /* Force panel (LCD/DFP) off         */
    Bool              ddc_mode;         /* Validate mode by matching exactly
					 * the modes supported in DDC data
					 */
    Bool              R300CGWorkaround;

				/* EDID or BIOS values for FPs */
    int               PanelXRes;
    int               PanelYRes;
    int               HOverPlus;
    int               HSyncWidth;
    int               HBlank;
    int               VOverPlus;
    int               VSyncWidth;
    int               VBlank;
    int               PanelPwrDly;
    int               DotClock;
    int               RefDivider;
    int               FeedbackDivider;
    int               PostDivider;
    Bool              UseBiosDividers;
				/* EDID data using DDC interface */
    Bool              ddc_bios;
    Bool              ddc1;
    Bool              ddc2;
    I2CBusPtr         pI2CBus;
    CARD32            DDCReg;

    RADEONPLLRec      pll;
    RADEONTMDSPll     tmds_pll[4];
    int               RamWidth;
    float	      sclk;		/* in MHz */
    float	      mclk;		/* in MHz */
    Bool	      IsDDR;
    int               DispPriority;

    RADEONSaveRec     SavedReg;         /* Original (text) mode              */
    RADEONSaveRec     ModeReg;          /* Current mode                      */
    Bool              (*CloseScreen)(int, ScreenPtr);

    void              (*BlockHandler)(int, pointer, pointer, pointer);

    Bool              PaletteSavedOnVT; /* Palette saved on last VT switch   */

#ifdef USE_EXA
    ExaDriverPtr      exa;
    int               engineMode;
#define EXA_ENGINEMODE_UNKNOWN 0
#define EXA_ENGINEMODE_2D      1
#define EXA_ENGINEMODE_3D      2
#ifdef XF86DRI
    Bool              accelDFS;
#endif
#endif
#ifdef USE_XAA
    XAAInfoRecPtr     accel;
#endif
    Bool              accelOn;
    xf86CursorInfoPtr cursor;
    CARD32            cursor_offset;
#ifdef USE_XAA
    unsigned long     cursor_end;
#endif
    Bool              allowColorTiling;
    Bool              tilingEnabled; /* mirror of sarea->tiling_enabled */
#ifdef ARGB_CURSOR
    Bool	      cursor_argb;
#endif
    int               cursor_fg;
    int               cursor_bg;

#ifdef USE_XAA
    /*
     * XAAForceTransBlit is used to change the behavior of the XAA
     * SetupForScreenToScreenCopy function, to make it DGA-friendly.
     */
    Bool              XAAForceTransBlit;
#endif

    int               fifo_slots;       /* Free slots in the FIFO (64 max)   */
    int               pix24bpp;         /* Depth of pixmap for 24bpp fb      */
    Bool              dac6bits;         /* Use 6 bit DAC?                    */

				/* Computed values for Radeon */
    int               pitch;
    int               datatype;
    CARD32            dp_gui_master_cntl;
    CARD32            dp_gui_master_cntl_clip;
    CARD32            trans_color;

				/* Saved values for ScreenToScreenCopy */
    int               xdir;
    int               ydir;

#ifdef USE_XAA
				/* ScanlineScreenToScreenColorExpand support */
    unsigned char     *scratch_buffer[1];
    unsigned char     *scratch_save;
    int               scanline_x;
    int               scanline_y;
    int               scanline_w;
    int               scanline_h;
    int               scanline_h_w;
    int               scanline_words;
    int               scanline_direct;
    int               scanline_bpp;     /* Only used for ImageWrite */
    int               scanline_fg;
    int               scanline_bg;
    int               scanline_hpass;
    int               scanline_x1clip;
    int               scanline_x2clip;
#endif
				/* Saved values for DashedTwoPointLine */
    int               dashLen;
    CARD32            dashPattern;
    int               dash_fg;
    int               dash_bg;

    DGAModePtr        DGAModes;
    int               numDGAModes;
    Bool              DGAactive;
    int               DGAViewportStatus;
    DGAFunctionRec    DGAFuncs;

    RADEONFBLayout    CurrentLayout;
    CARD32            dst_pitch_offset;
#ifdef XF86DRI
    Bool              noBackBuffer;	
    Bool              directRenderingEnabled;
    Bool              directRenderingInited;
    Bool              newMemoryMap;
    drmVersionPtr     pLibDRMVersion;
    drmVersionPtr     pKernelDRMVersion;
    DRIInfoPtr        pDRIInfo;
    int               drmFD;
    int               numVisualConfigs;
    __GLXvisualConfig *pVisualConfigs;
    RADEONConfigPrivPtr pVisualConfigsPriv;
    Bool             (*DRICloseScreen)(int, ScreenPtr);

    drm_handle_t         fbHandle;

    drmSize           registerSize;
    drm_handle_t         registerHandle;

    RADEONCardType    cardType;            /* Current card is a PCI card */
    drmSize           pciSize;
    drm_handle_t         pciMemHandle;
    unsigned char     *PCI;             /* Map */

    Bool              depthMoves;       /* Enable depth moves -- slow! */
    Bool              allowPageFlip;    /* Enable 3d page flipping */
    Bool              have3DWindows;    /* Are there any 3d clients? */

    drmSize           gartSize;
    drm_handle_t         agpMemHandle;     /* Handle from drmAgpAlloc */
    unsigned long     gartOffset;
    unsigned char     *AGP;             /* Map */
    int               agpMode;
    int               agpFastWrite;

    CARD32            pciCommand;

    Bool              CPRuns;           /* CP is running */
    Bool              CPInUse;          /* CP has been used by X server */
    Bool              CPStarted;        /* CP has started */
    int               CPMode;           /* CP mode that server/clients use */
    int               CPFifoSize;       /* Size of the CP command FIFO */
    int               CPusecTimeout;    /* CP timeout in usecs */

				/* CP ring buffer data */
    unsigned long     ringStart;        /* Offset into GART space */
    drm_handle_t         ringHandle;       /* Handle from drmAddMap */
    drmSize           ringMapSize;      /* Size of map */
    int               ringSize;         /* Size of ring (in MB) */
    drmAddress        ring;             /* Map */
    int               ringSizeLog2QW;

    unsigned long     ringReadOffset;   /* Offset into GART space */
    drm_handle_t         ringReadPtrHandle; /* Handle from drmAddMap */
    drmSize           ringReadMapSize;  /* Size of map */
    drmAddress        ringReadPtr;      /* Map */

				/* CP vertex/indirect buffer data */
    unsigned long     bufStart;         /* Offset into GART space */
    drm_handle_t         bufHandle;        /* Handle from drmAddMap */
    drmSize           bufMapSize;       /* Size of map */
    int               bufSize;          /* Size of buffers (in MB) */
    drmAddress        buf;              /* Map */
    int               bufNumBufs;       /* Number of buffers */
    drmBufMapPtr      buffers;          /* Buffer map */

				/* CP GART Texture data */
    unsigned long     gartTexStart;      /* Offset into GART space */
    drm_handle_t         gartTexHandle;     /* Handle from drmAddMap */
    drmSize           gartTexMapSize;    /* Size of map */
    int               gartTexSize;       /* Size of GART tex space (in MB) */
    drmAddress        gartTex;           /* Map */
    int               log2GARTTexGran;

				/* CP accleration */
    drmBufPtr         indirectBuffer;
    int               indirectStart;

				/* DRI screen private data */
    int               fbX;
    int               fbY;
    int               backX;
    int               backY;
    int               depthX;
    int               depthY;

    int               frontOffset;
    int               frontPitch;
    int               backOffset;
    int               backPitch;
    int               depthOffset;
    int               depthPitch;
    int               depthBits;
    int               textureOffset;
    int               textureSize;
    int               log2TexGran;

    int               pciGartSize;
    CARD32            pciGartOffset;
    void              *pciGartBackup;
#ifdef USE_XAA
    CARD32            frontPitchOffset;
    CARD32            backPitchOffset;
    CARD32            depthPitchOffset;

				/* offscreen memory management */
    int               backLines;
    FBAreaPtr         backArea;
    int               depthTexLines;
    FBAreaPtr         depthTexArea;
#endif

				/* Saved scissor values */
    CARD32            sc_left;
    CARD32            sc_right;
    CARD32            sc_top;
    CARD32            sc_bottom;

    CARD32            re_top_left;
    CARD32            re_width_height;

    CARD32            aux_sc_cntl;

    int               irq;

    Bool              DMAForXv;

#ifdef PER_CONTEXT_SAREA
    int               perctx_sarea_size;
#endif

    /* Debugging info for BEGIN_RING/ADVANCE_RING pairs. */
    int               dma_begin_count;
    char              *dma_debug_func;
    int               dma_debug_lineno;
#endif /* XF86DRI */

				/* XVideo */
    XF86VideoAdaptorPtr adaptor;
    void              (*VideoTimerCallback)(ScrnInfoPtr, Time);
    int               videoKey;
    int		      RageTheatreCrystal;
    int               RageTheatreTunerPort;
    int               RageTheatreCompositePort;
    int               RageTheatreSVideoPort;
    int               tunerType;
	char*			RageTheatreMicrocPath;
	char*			RageTheatreMicrocType;
    Bool               MM_TABLE_valid;
    struct {
    	CARD8 table_revision;
	CARD8 table_size;
        CARD8 tuner_type;
        CARD8 audio_chip;
        CARD8 product_id;
        CARD8 tuner_voltage_teletext_fm;
        CARD8 i2s_config; /* configuration of the sound chip */
        CARD8 video_decoder_type;
        CARD8 video_decoder_host_config;
        CARD8 input[5];
    	} MM_TABLE;
    CARD16 video_decoder_type;

    /* Render */
    Bool              RenderAccel;
#ifdef USE_XAA
    FBLinearPtr       RenderTex;
    void              (*RenderCallback)(ScrnInfoPtr);
    Time              RenderTimeout;
#endif

    /* general */
    Bool              showCache;
    OptionInfoPtr     Options;

    Bool              useEXA;
#ifdef XFree86LOADER
#ifdef USE_EXA
    XF86ModReqInfo    exaReq;
#endif
#ifdef USE_XAA
    XF86ModReqInfo    xaaReq;
#endif
#endif

    /* X itself has the 3D context */
    Bool              XInited3D;

    /* merged fb stuff, also covers clone modes */
    Bool		MergedFB;
    RADEONScrn2Rel	CRT2Position;
    char *		CRT2HSync;
    char *		CRT2VRefresh;
    char *		MetaModes;
    ScrnInfoPtr		CRT2pScrn;
    DisplayModePtr	CRT1Modes;
    DisplayModePtr	CRT1CurrentMode;
    int			CRT1frameX0;
    int			CRT1frameY0;
    int			CRT1frameX1;
    int			CRT1frameY1;
    RADEONMonitorType   MergeType;
    RADEONDDCType       MergeDDCType;
    void        	(*PointerMoved)(int index, int x, int y);
    /* pseudo xinerama support for mergedfb */
    int			maxCRT1_X1, maxCRT1_X2, maxCRT1_Y1, maxCRT1_Y2;
    int			maxCRT2_X1, maxCRT2_X2, maxCRT2_Y1, maxCRT2_Y2;
    int			maxClone_X1, maxClone_X2, maxClone_Y1, maxClone_Y2;
    Bool		UseRADEONXinerama;
    Bool		CRT2IsScrn0;
    ExtensionEntry 	*XineramaExtEntry;
    int			RADEONXineramaVX, RADEONXineramaVY;
    Bool		AtLeastOneNonClone;
    int			MergedFBXDPI, MergedFBYDPI;
    Bool		NoVirtual;
    int                 CRT1XOffs, CRT1YOffs, CRT2XOffs, CRT2YOffs;
    int                 MBXNR1XMAX, MBXNR1YMAX, MBXNR2XMAX, MBXNR2YMAX;
    Bool                NonRect, HaveNonRect, HaveOffsRegions, MouseRestrictions;
    region              NonRectDead, OffDead1, OffDead2;

    int			constantDPI; /* -1 = auto, 0 = off, 1 = on */
    int			RADEONDPIVX, RADEONDPIVY;
    RADEONScrn2Rel	MergedDPISRel;
    int			RADEONMergedDPIVX, RADEONMergedDPIVY, RADEONMergedDPIRot;

    /* special handlings for DELL triple-head server */
    Bool		IsDellServer; 

    /* enable bios hotkey output switching */
    Bool		BiosHotkeys;

    Bool               VGAAccess;

    int                MaxSurfaceWidth;
    int                MaxLines;

} RADEONInfoRec, *RADEONInfoPtr;

#define RADEONWaitForFifo(pScrn, entries)				\
do {									\
    if (info->fifo_slots < entries)					\
	RADEONWaitForFifoFunction(pScrn, entries);			\
    info->fifo_slots -= entries;					\
} while (0)

extern RADEONEntPtr RADEONEntPriv(ScrnInfoPtr pScrn);
extern void        RADEONWaitForFifoFunction(ScrnInfoPtr pScrn, int entries);
extern void        RADEONWaitForIdleMMIO(ScrnInfoPtr pScrn);
#ifdef XF86DRI
extern void        RADEONWaitForIdleCP(ScrnInfoPtr pScrn);
#endif

extern void        RADEONDoAdjustFrame(ScrnInfoPtr pScrn, int x, int y,
				       int clone);

extern void        RADEONEngineReset(ScrnInfoPtr pScrn);
extern void        RADEONEngineFlush(ScrnInfoPtr pScrn);
extern void        RADEONEngineRestore(ScrnInfoPtr pScrn);

extern unsigned    RADEONINPLL(ScrnInfoPtr pScrn, int addr);
extern void        RADEONOUTPLL(ScrnInfoPtr pScrn, int addr, CARD32 data);

extern void        RADEONWaitForVerticalSync(ScrnInfoPtr pScrn);
extern void        RADEONWaitForVerticalSync2(ScrnInfoPtr pScrn);

extern void        RADEONChangeSurfaces(ScrnInfoPtr pScrn);

extern Bool        RADEONAccelInit(ScreenPtr pScreen);
#ifdef USE_EXA
extern Bool        RADEONSetupMemEXA (ScreenPtr pScreen);
extern Bool        RADEONDrawInitMMIO(ScreenPtr pScreen);
#ifdef XF86DRI
extern Bool        RADEONDrawInitCP(ScreenPtr pScreen);
#endif
#endif
#ifdef USE_XAA
extern void        RADEONAccelInitMMIO(ScreenPtr pScreen, XAAInfoRecPtr a);
#endif
extern void        RADEONEngineInit(ScrnInfoPtr pScrn);
extern Bool        RADEONCursorInit(ScreenPtr pScreen);
extern Bool        RADEONDGAInit(ScreenPtr pScreen);

extern void        RADEONInit3DEngine(ScrnInfoPtr pScrn);

extern int         RADEONMinBits(int val);

extern void        RADEONInitVideo(ScreenPtr pScreen);
extern void        RADEONResetVideo(ScrnInfoPtr pScrn);
extern void        R300CGWorkaround(ScrnInfoPtr pScrn);

extern void        RADEONPllErrataAfterIndex(RADEONInfoPtr info);
extern void        RADEONPllErrataAfterData(RADEONInfoPtr info);

extern Bool        RADEONGetBIOSInfo(ScrnInfoPtr pScrn, xf86Int10InfoPtr pInt10);
extern Bool        RADEONGetConnectorInfoFromBIOS (ScrnInfoPtr pScrn);
extern Bool        RADEONGetClockInfoFromBIOS (ScrnInfoPtr pScrn);
extern Bool        RADEONGetLVDSInfoFromBIOS (ScrnInfoPtr pScrn);
extern Bool        RADEONGetTMDSInfoFromBIOS (ScrnInfoPtr pScrn);
extern Bool        RADEONGetHardCodedEDIDFromBIOS (ScrnInfoPtr pScrn);

#ifdef XF86DRI
#ifdef USE_XAA
extern void        RADEONAccelInitCP(ScreenPtr pScreen, XAAInfoRecPtr a);
#endif
extern Bool        RADEONDRIGetVersion(ScrnInfoPtr pScrn);
extern Bool        RADEONDRIScreenInit(ScreenPtr pScreen);
extern void        RADEONDRICloseScreen(ScreenPtr pScreen);
extern void        RADEONDRIResume(ScreenPtr pScreen);
extern Bool        RADEONDRIFinishScreenInit(ScreenPtr pScreen);
extern void        RADEONDRIAllocatePCIGARTTable(ScreenPtr pScreen);
extern void	   RADEONDRIInitPageFlip(ScreenPtr pScreen);
extern void        RADEONDRIStop(ScreenPtr pScreen);

extern drmBufPtr   RADEONCPGetBuffer(ScrnInfoPtr pScrn);
extern void        RADEONCPFlushIndirect(ScrnInfoPtr pScrn, int discard);
extern void        RADEONCPReleaseIndirect(ScrnInfoPtr pScrn);
extern int         RADEONCPStop(ScrnInfoPtr pScrn,  RADEONInfoPtr info);

extern void        RADEONHostDataParams(ScrnInfoPtr pScrn, CARD8 *dst,
					CARD32 pitch, int cpp,
					CARD32 *dstPitchOffset, int *x, int *y);
extern CARD8*      RADEONHostDataBlit(ScrnInfoPtr pScrn, unsigned int cpp,
				      unsigned int w, CARD32 dstPitchOff,
				      CARD32 *bufPitch, int x, int *y,
				      unsigned int *h, unsigned int *hpass);
extern void        RADEONHostDataBlitCopyPass(ScrnInfoPtr pScrn,
					      unsigned int bpp,
					      CARD8 *dst, CARD8 *src,
					      unsigned int hpass,
					      unsigned int dstPitch,
					      unsigned int srcPitch);
extern void        RADEONCopySwap(CARD8 *dst, CARD8 *src, unsigned int size,
				  int swap);

#define RADEONCP_START(pScrn, info)					\
do {									\
    int _ret = drmCommandNone(info->drmFD, DRM_RADEON_CP_START);	\
    if (_ret) {								\
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,				\
		   "%s: CP start %d\n", __FUNCTION__, _ret);		\
    }									\
    info->CPStarted = TRUE;                                             \
} while (0)

#define RADEONCP_STOP(pScrn, info)					\
do {									\
    int _ret;								\
     if (info->CPStarted) {						\
        _ret = RADEONCPStop(pScrn, info);				\
        if (_ret) {							\
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,			\
		   "%s: CP stop %d\n", __FUNCTION__, _ret);		\
        }								\
        info->CPStarted = FALSE;                                        \
   }									\
    RADEONEngineRestore(pScrn);						\
    info->CPRuns = FALSE;						\
} while (0)

#define RADEONCP_RESET(pScrn, info)					\
do {									\
    if (RADEONCP_USE_RING_BUFFER(info->CPMode)) {			\
	int _ret = drmCommandNone(info->drmFD, DRM_RADEON_CP_RESET);	\
	if (_ret) {							\
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,			\
		       "%s: CP reset %d\n", __FUNCTION__, _ret);	\
	}								\
    }									\
} while (0)

#define RADEONCP_REFRESH(pScrn, info)					\
do {									\
    if (!info->CPInUse) {						\
	RADEON_WAIT_UNTIL_IDLE();					\
	BEGIN_RING(6);							\
	OUT_RING_REG(RADEON_RE_TOP_LEFT,     info->re_top_left);	\
	OUT_RING_REG(RADEON_RE_WIDTH_HEIGHT, info->re_width_height);	\
	OUT_RING_REG(RADEON_AUX_SC_CNTL,     info->aux_sc_cntl);	\
	ADVANCE_RING();							\
	info->CPInUse = TRUE;						\
    }									\
} while (0)


#define CP_PACKET0(reg, n)						\
	(RADEON_CP_PACKET0 | ((n) << 16) | ((reg) >> 2))
#define CP_PACKET1(reg0, reg1)						\
	(RADEON_CP_PACKET1 | (((reg1) >> 2) << 11) | ((reg0) >> 2))
#define CP_PACKET2()							\
	(RADEON_CP_PACKET2)
#define CP_PACKET3(pkt, n)						\
	(RADEON_CP_PACKET3 | (pkt) | ((n) << 16))


#define RADEON_VERBOSE	0

#define RING_LOCALS	CARD32 *__head = NULL; int __expected; int __count = 0

#define BEGIN_RING(n) do {						\
    if (RADEON_VERBOSE) {						\
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,				\
		   "BEGIN_RING(%d) in %s\n", (unsigned int)n, __FUNCTION__);\
    }									\
    if (++info->dma_begin_count != 1) {					\
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,				\
		   "BEGIN_RING without end at %s:%d\n",			\
		   info->dma_debug_func, info->dma_debug_lineno);	\
	info->dma_begin_count = 1;					\
    }									\
    info->dma_debug_func = __FILE__;					\
    info->dma_debug_lineno = __LINE__;					\
    if (!info->indirectBuffer) {					\
	info->indirectBuffer = RADEONCPGetBuffer(pScrn);		\
	info->indirectStart = 0;					\
    } else if (info->indirectBuffer->used + (n) * (int)sizeof(CARD32) >	\
	       info->indirectBuffer->total) {				\
	RADEONCPFlushIndirect(pScrn, 1);				\
    }									\
    __expected = n;							\
    __head = (pointer)((char *)info->indirectBuffer->address +		\
		       info->indirectBuffer->used);			\
    __count = 0;							\
} while (0)

#define ADVANCE_RING() do {						\
    if (info->dma_begin_count-- != 1) {					\
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,				\
		   "ADVANCE_RING without begin at %s:%d\n",		\
		   __FILE__, __LINE__);					\
	info->dma_begin_count = 0;					\
    }									\
    if (__count != __expected) {					\
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,				\
		   "ADVANCE_RING count != expected (%d vs %d) at %s:%d\n", \
		   __count, __expected, __FILE__, __LINE__);		\
    }									\
    if (RADEON_VERBOSE) {						\
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,				\
		   "ADVANCE_RING() start: %d used: %d count: %d\n",	\
		   info->indirectStart,					\
		   info->indirectBuffer->used,				\
		   __count * (int)sizeof(CARD32));			\
    }									\
    info->indirectBuffer->used += __count * (int)sizeof(CARD32);	\
} while (0)

#define OUT_RING(x) do {						\
    if (RADEON_VERBOSE) {						\
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,				\
		   "   OUT_RING(0x%08x)\n", (unsigned int)(x));		\
    }									\
    __head[__count++] = (x);						\
} while (0)

#define OUT_RING_REG(reg, val)						\
do {									\
    OUT_RING(CP_PACKET0(reg, 0));					\
    OUT_RING(val);							\
} while (0)

#define FLUSH_RING()							\
do {									\
    if (RADEON_VERBOSE)							\
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,				\
		   "FLUSH_RING in %s\n", __FUNCTION__);			\
    if (info->indirectBuffer) {						\
	RADEONCPFlushIndirect(pScrn, 0);				\
    }									\
} while (0)


#define RADEON_WAIT_UNTIL_2D_IDLE()					\
do {									\
    BEGIN_RING(2);							\
    OUT_RING(CP_PACKET0(RADEON_WAIT_UNTIL, 0));				\
    OUT_RING((RADEON_WAIT_2D_IDLECLEAN |				\
	      RADEON_WAIT_HOST_IDLECLEAN));				\
    ADVANCE_RING();							\
} while (0)

#define RADEON_WAIT_UNTIL_3D_IDLE()					\
do {									\
    BEGIN_RING(2);							\
    OUT_RING(CP_PACKET0(RADEON_WAIT_UNTIL, 0));				\
    OUT_RING((RADEON_WAIT_3D_IDLECLEAN |				\
	      RADEON_WAIT_HOST_IDLECLEAN));				\
    ADVANCE_RING();							\
} while (0)

#define RADEON_WAIT_UNTIL_IDLE()					\
do {									\
    if (RADEON_VERBOSE) {						\
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,				\
		   "WAIT_UNTIL_IDLE() in %s\n", __FUNCTION__);		\
    }									\
    BEGIN_RING(2);							\
    OUT_RING(CP_PACKET0(RADEON_WAIT_UNTIL, 0));				\
    OUT_RING((RADEON_WAIT_2D_IDLECLEAN |				\
	      RADEON_WAIT_3D_IDLECLEAN |				\
	      RADEON_WAIT_HOST_IDLECLEAN));				\
    ADVANCE_RING();							\
} while (0)

#define RADEON_FLUSH_CACHE()						\
do {									\
    BEGIN_RING(2);							\
    OUT_RING(CP_PACKET0(RADEON_RB3D_DSTCACHE_CTLSTAT, 0));		\
    OUT_RING(RADEON_RB3D_DC_FLUSH);					\
    ADVANCE_RING();							\
} while (0)

#define RADEON_PURGE_CACHE()						\
do {									\
    BEGIN_RING(2);							\
    OUT_RING(CP_PACKET0(RADEON_RB3D_DSTCACHE_CTLSTAT, 0));		\
    OUT_RING(RADEON_RB3D_DC_FLUSH_ALL);					\
    ADVANCE_RING();							\
} while (0)

#endif /* XF86DRI */

static __inline__ void RADEON_MARK_SYNC(RADEONInfoPtr info, ScrnInfoPtr pScrn)
{
#ifdef USE_EXA
    if (info->useEXA)
	exaMarkSync(pScrn->pScreen);
#endif
#ifdef USE_XAA
    if (!info->useEXA)
	SET_SYNC_FLAG(info->accel);
#endif
}

static __inline__ void RADEON_SYNC(RADEONInfoPtr info, ScrnInfoPtr pScrn)
{
#ifdef USE_EXA
    if (info->useEXA)
	exaWaitSync(pScrn->pScreen);
#endif
#ifdef USE_XAA
    if (!info->useEXA && info->accel)
	info->accel->Sync(pScrn);
#endif
}

#endif /* _RADEON_H_ */
