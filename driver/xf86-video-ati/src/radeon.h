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
#include <sys/time.h>		/* For gettimeofday() */

#include "config.h"
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
#include "radeon_tv.h"

				/* DRI support */
#ifdef XF86DRI
#define _XF86DRI_SERVER_
#include "radeon_dripriv.h"
#include "dri.h"
#include "GL/glxint.h"
#ifdef DAMAGE
#include "damage.h"
#include "globals.h"
#endif
#endif

#include "xf86Crtc.h"
#include "X11/Xatom.h"

				/* Render support */
#ifdef RENDER
#include "picturestr.h"
#endif

#include "atipcirename.h"

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)>(b)?(b):(a))
#endif

/* Provide substitutes for gcc's __FUNCTION__ on other compilers */
#if !defined(__GNUC__) && !defined(__FUNCTION__)
# define __FUNCTION__ __func__		/* C99 */
#endif

#ifndef HAVE_XF86MODEBANDWIDTH
extern unsigned int xf86ModeBandwidth(DisplayModePtr mode, int depth);
#define MODE_BANDWIDTH MODE_BAD
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
    OPTION_PCIAPER_SIZE,
#ifdef USE_EXA
    OPTION_ACCEL_DFS,
#endif
#endif
    OPTION_DDC_MODE,
    OPTION_IGNORE_EDID,
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
    OPTION_SCALER_WIDTH,
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
    OPTION_CONNECTORTABLE,
    OPTION_DRI,
    OPTION_DEFAULT_CONNECTOR_TABLE,
#if defined(__powerpc__)
    OPTION_MAC_MODEL,
#endif
    OPTION_DEFAULT_TMDS_PLL,
    OPTION_TVDAC_LOAD_DETECT,
    OPTION_FORCE_TVOUT,
    OPTION_TVSTD,
    OPTION_IGNORE_LID_STATUS,
    OPTION_DEFAULT_TVDAC_ADJ,
    OPTION_INT10
} RADEONOpts;


#define RADEON_IDLE_RETRY      16 /* Fall out of idle loops after this count */
#define RADEON_TIMEOUT    2000000 /* Fall out of wait loops after this count */

#define RADEON_VSYNC_TIMEOUT	20000 /* Maximum wait for VSYNC (in usecs) */

/* Buffer are aligned on 4096 byte boundaries */
#define RADEON_BUFFER_ALIGN 0x00000fff
#define RADEON_VBIOS_SIZE 0x00010000
#define RADEON_USE_RMX 0x80000000 /* mode flag for using RMX
				   * Need to comfirm this is not used
				   * for something else.
				   */

#define xFixedToFloat(f) (((float) (f)) / 65536)

#define RADEON_LOGLEVEL_DEBUG 4

/* for Xv, outputs */
#define MAKE_ATOM(a) MakeAtom(a, sizeof(a) - 1, TRUE)

/* Other macros */
#define RADEON_ARRAY_SIZE(x)  (sizeof(x)/sizeof(x[0]))
#define RADEON_ALIGN(x,bytes) (((x) + ((bytes) - 1)) & ~((bytes) - 1))
#define RADEONPTR(pScrn)      ((RADEONInfoPtr)(pScrn)->driverPrivate)

typedef struct {
    int    revision;
    uint16_t rr1_offset;
    uint16_t rr2_offset;
    uint16_t dyn_clk_offset;
    uint16_t pll_offset;
    uint16_t mem_config_offset;
    uint16_t mem_reset_offset;
    uint16_t short_mem_offset;
    uint16_t rr3_offset;
    uint16_t rr4_offset;
} RADEONBIOSInitTable;

#define RADEON_PLL_USE_BIOS_DIVS   (1 << 0)
#define RADEON_PLL_NO_ODD_POST_DIV (1 << 1)
#define RADEON_PLL_USE_REF_DIV     (1 << 2)
#define RADEON_PLL_LEGACY          (1 << 3)
#define RADEON_PLL_PREFER_LOW_REF_DIV (1 << 4)

typedef struct {
    uint16_t          reference_freq;
    uint16_t          reference_div;
    uint32_t          pll_in_min;
    uint32_t          pll_in_max;
    uint32_t          pll_out_min;
    uint32_t          pll_out_max;
    uint16_t          xclk;

    uint32_t          min_ref_div;
    uint32_t          max_ref_div;
    uint32_t          min_post_div;
    uint32_t          max_post_div;
    uint32_t          min_feedback_div;
    uint32_t          max_feedback_div;
    uint32_t          best_vco;
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
    CHIP_FAMILY_RS400,    /* xpress 200, 200m (RS400) Intel */
    CHIP_FAMILY_RS480,    /* xpress 200, 200m (RS410/480/482/485) AMD */
    CHIP_FAMILY_RV515,    /* rv515 */
    CHIP_FAMILY_R520,    /* r520 */
    CHIP_FAMILY_RV530,    /* rv530 */
    CHIP_FAMILY_R580,    /* r580 */
    CHIP_FAMILY_RV560,   /* rv560 */
    CHIP_FAMILY_RV570,   /* rv570 */
    CHIP_FAMILY_RS600,
    CHIP_FAMILY_RS690,
    CHIP_FAMILY_RS740,
    CHIP_FAMILY_R600,    /* r600 */
    CHIP_FAMILY_R630,
    CHIP_FAMILY_RV610,
    CHIP_FAMILY_RV630,
    CHIP_FAMILY_RV670,
    CHIP_FAMILY_RV620,
    CHIP_FAMILY_RV635,
    CHIP_FAMILY_RS780,
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
        (info->ChipFamily == CHIP_FAMILY_RS400) ||  \
        (info->ChipFamily == CHIP_FAMILY_RS480))

#define IS_AVIVO_VARIANT ((info->ChipFamily >= CHIP_FAMILY_RV515))

#define IS_DCE3_VARIANT ((info->ChipFamily >= CHIP_FAMILY_RV620))

#define IS_R500_3D ((info->ChipFamily == CHIP_FAMILY_RV515)  ||  \
	(info->ChipFamily == CHIP_FAMILY_R520)   ||  \
	(info->ChipFamily == CHIP_FAMILY_RV530)  ||  \
	(info->ChipFamily == CHIP_FAMILY_R580)   ||  \
	(info->ChipFamily == CHIP_FAMILY_RV560)  ||  \
	(info->ChipFamily == CHIP_FAMILY_RV570))

#define IS_R300_3D ((info->ChipFamily == CHIP_FAMILY_R300)  ||  \
	(info->ChipFamily == CHIP_FAMILY_RV350) ||  \
	(info->ChipFamily == CHIP_FAMILY_R350)  ||  \
	(info->ChipFamily == CHIP_FAMILY_RV380) ||  \
	(info->ChipFamily == CHIP_FAMILY_R420)  ||  \
	(info->ChipFamily == CHIP_FAMILY_RV410) ||  \
	(info->ChipFamily == CHIP_FAMILY_RS690) ||  \
	(info->ChipFamily == CHIP_FAMILY_RS600) ||  \
	(info->ChipFamily == CHIP_FAMILY_RS740) ||  \
	(info->ChipFamily == CHIP_FAMILY_RS400) ||  \
	(info->ChipFamily == CHIP_FAMILY_RS480))

/*
 * Errata workarounds
 */
typedef enum {
       CHIP_ERRATA_R300_CG             = 0x00000001,
       CHIP_ERRATA_PLL_DUMMYREADS      = 0x00000002,
       CHIP_ERRATA_PLL_DELAY           = 0x00000004
} RADEONErrata;

typedef enum {
    RADEON_DVOCHIP_NONE,
    RADEON_SIL_164,
    RADEON_SIL_1178
} RADEONExtTMDSChip;

#if defined(__powerpc__)
typedef enum {
    RADEON_MAC_NONE,
    RADEON_MAC_IBOOK,
    RADEON_MAC_POWERBOOK_EXTERNAL,
    RADEON_MAC_POWERBOOK_INTERNAL,
    RADEON_MAC_POWERBOOK_VGA,
    RADEON_MAC_MINI_EXTERNAL,
    RADEON_MAC_MINI_INTERNAL,
    RADEON_MAC_IMAC_G5_ISIGHT
} RADEONMacModel;
#endif

typedef enum {
	CARD_PCI,
	CARD_AGP,
	CARD_PCIE
} RADEONCardType;

typedef struct _atomBiosHandle *atomBiosHandlePtr;

typedef struct {
    uint32_t pci_device_id;
    RADEONChipFamily chip_family;
    int mobility;
    int igp;
    int nocrtc2;
    int nointtvout;
    int singledac;
} RADEONCardInfo;

typedef struct {
    EntityInfoPtr     pEnt;
    pciVideoPtr       PciInfo;
    PCITAG            PciTag;
    int               Chipset;
    RADEONChipFamily  ChipFamily;
    RADEONErrata      ChipErrata;

    unsigned long     LinearAddr;       /* Frame buffer physical address     */
    unsigned long     MMIOAddr;         /* MMIO region physical address      */
    unsigned long     BIOSAddr;         /* BIOS physical address             */
    uint32_t          fbLocation;
    uint32_t          gartLocation;
    uint32_t          mc_fb_location;
    uint32_t          mc_agp_location;
    uint32_t          mc_agp_location_hi;

    void              *MMIO;            /* Map of MMIO region                */
    void              *FB;              /* Map of frame buffer               */
    uint8_t           *VBIOS;           /* Video BIOS pointer                */

    Bool              IsAtomBios;       /* New BIOS used in R420 etc.        */
    int               ROMHeaderStart;   /* Start of the ROM Info Table       */
    int               MasterDataStart;  /* Offset for Master Data Table for ATOM BIOS */

    uint32_t          MemCntl;
    uint32_t          BusCntl;
    unsigned long     MMIOSize;         /* MMIO region physical address      */
    unsigned long     FbMapSize;        /* Size of frame buffer, in bytes    */
    unsigned long     FbSecureSize;     /* Size of secured fb area at end of
                                           framebuffer */

    Bool              IsMobility;       /* Mobile chips for laptops */
    Bool              IsIGP;            /* IGP chips */
    Bool              HasSingleDAC;     /* only TVDAC on chip */
    Bool              ddc_mode;         /* Validate mode by matching exactly
					 * the modes supported in DDC data
					 */
    Bool              R300CGWorkaround;

				/* EDID or BIOS values for FPs */
    int               RefDivider;
    int               FeedbackDivider;
    int               PostDivider;
    Bool              UseBiosDividers;
				/* EDID data using DDC interface */
    Bool              ddc_bios;
    Bool              ddc1;
    Bool              ddc2;

    RADEONPLLRec      pll;

    int               RamWidth;
    float	      sclk;		/* in MHz */
    float	      mclk;		/* in MHz */
    Bool	      IsDDR;
    int               DispPriority;

    RADEONSavePtr     SavedReg;         /* Original (text) mode              */
    RADEONSavePtr     ModeReg;          /* Current mode                      */
    Bool              (*CloseScreen)(int, ScreenPtr);

    void              (*BlockHandler)(int, pointer, pointer, pointer);

    Bool              PaletteSavedOnVT; /* Palette saved on last VT switch   */

#ifdef USE_EXA
    ExaDriverPtr      exa;
    int               exaSyncMarker;
    int               exaMarkerSynced;
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
    uint32_t          dp_gui_master_cntl;
    uint32_t          dp_gui_master_cntl_clip;
    uint32_t          trans_color;

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
    uint32_t          dashPattern;
    int               dash_fg;
    int               dash_bg;

    DGAModePtr        DGAModes;
    int               numDGAModes;
    Bool              DGAactive;
    int               DGAViewportStatus;
    DGAFunctionRec    DGAFuncs;

    RADEONFBLayout    CurrentLayout;
    uint32_t          dst_pitch_offset;
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

    drm_handle_t      fbHandle;

    drmSize           registerSize;
    drm_handle_t      registerHandle;

    RADEONCardType    cardType;            /* Current card is a PCI card */
    drmSize           pciSize;
    drm_handle_t      pciMemHandle;
    unsigned char     *PCI;             /* Map */

    Bool              depthMoves;       /* Enable depth moves -- slow! */
    Bool              allowPageFlip;    /* Enable 3d page flipping */
#ifdef DAMAGE
    DamagePtr         pDamage;
    RegionRec         driRegion;
#endif
    Bool              have3DWindows;    /* Are there any 3d clients? */

    int               pciAperSize;
    drmSize           gartSize;
    drm_handle_t      agpMemHandle;     /* Handle from drmAgpAlloc */
    unsigned long     gartOffset;
    unsigned char     *AGP;             /* Map */
    int               agpMode;

    uint32_t          pciCommand;

    Bool              CPRuns;           /* CP is running */
    Bool              CPInUse;          /* CP has been used by X server */
    Bool              CPStarted;        /* CP has started */
    int               CPMode;           /* CP mode that server/clients use */
    int               CPFifoSize;       /* Size of the CP command FIFO */
    int               CPusecTimeout;    /* CP timeout in usecs */
    Bool              needCacheFlush;

				/* CP ring buffer data */
    unsigned long     ringStart;        /* Offset into GART space */
    drm_handle_t      ringHandle;       /* Handle from drmAddMap */
    drmSize           ringMapSize;      /* Size of map */
    int               ringSize;         /* Size of ring (in MB) */
    drmAddress        ring;             /* Map */
    int               ringSizeLog2QW;

    unsigned long     ringReadOffset;   /* Offset into GART space */
    drm_handle_t      ringReadPtrHandle; /* Handle from drmAddMap */
    drmSize           ringReadMapSize;  /* Size of map */
    drmAddress        ringReadPtr;      /* Map */

				/* CP vertex/indirect buffer data */
    unsigned long     bufStart;         /* Offset into GART space */
    drm_handle_t      bufHandle;        /* Handle from drmAddMap */
    drmSize           bufMapSize;       /* Size of map */
    int               bufSize;          /* Size of buffers (in MB) */
    drmAddress        buf;              /* Map */
    int               bufNumBufs;       /* Number of buffers */
    drmBufMapPtr      buffers;          /* Buffer map */

				/* CP GART Texture data */
    unsigned long     gartTexStart;      /* Offset into GART space */
    drm_handle_t      gartTexHandle;     /* Handle from drmAddMap */
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
    uint32_t          pciGartOffset;
    void              *pciGartBackup;
#ifdef USE_XAA
    uint32_t          frontPitchOffset;
    uint32_t          backPitchOffset;
    uint32_t          depthPitchOffset;

				/* offscreen memory management */
    int               backLines;
    FBAreaPtr         backArea;
    int               depthTexLines;
    FBAreaPtr         depthTexArea;
#endif

				/* Saved scissor values */
    uint32_t          sc_left;
    uint32_t          sc_right;
    uint32_t          sc_top;
    uint32_t          sc_bottom;

    uint32_t          re_top_left;
    uint32_t          re_width_height;

    uint32_t          aux_sc_cntl;

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
    char*             RageTheatreMicrocPath;
    char*             RageTheatreMicrocType;
    Bool              MM_TABLE_valid;
    struct {
    	uint8_t table_revision;
	uint8_t table_size;
        uint8_t tuner_type;
        uint8_t audio_chip;
        uint8_t product_id;
        uint8_t tuner_voltage_teletext_fm;
        uint8_t i2s_config; /* configuration of the sound chip */
        uint8_t video_decoder_type;
        uint8_t video_decoder_host_config;
        uint8_t input[5];
    } MM_TABLE;
    uint16_t video_decoder_type;
    int overlay_scaler_buffer_width;
    int ecp_div;

    /* Render */
    Bool              RenderAccel;
    unsigned short    texW[2];
    unsigned short    texH[2];
#ifdef USE_XAA
    FBLinearPtr       RenderTex;
    void              (*RenderCallback)(ScrnInfoPtr);
    Time              RenderTimeout;
#endif

    /* general */
    Bool              showCache;
    OptionInfoPtr     Options;

    Bool              useEXA;
#ifdef USE_EXA
    XF86ModReqInfo    exaReq;
#endif
#ifdef USE_XAA
    XF86ModReqInfo    xaaReq;
#endif

    /* X itself has the 3D context */
    Bool              XInited3D;

    DisplayModePtr currentMode, savedCurrentMode;

    /* special handlings for DELL triple-head server */
    Bool              IsDellServer; 

    Bool              VGAAccess;

    int               MaxSurfaceWidth;
    int               MaxLines;

    uint32_t          tv_dac_adj;
    uint32_t          tv_dac_enable_mask;

    Bool want_vblank_interrupts;
    RADEONBIOSConnector BiosConnector[RADEON_MAX_BIOS_CONNECTOR];
    RADEONBIOSInitTable BiosTable;

    /* save crtc state for console restore */
    Bool              crtc_on;
    Bool              crtc2_on;

    Bool              InternalTVOut;
    int               tvdac_use_count;

#if defined(__powerpc__)
    RADEONMacModel    MacModel;
#endif
    RADEONExtTMDSChip ext_tmds_chip;

    atomBiosHandlePtr atomBIOS;
    unsigned long FbFreeStart, FbFreeSize;
    unsigned char*      BIOSCopy;

    /* output enable masks for outputs shared across connectors */
    int output_crt1;
    int output_crt2;
    int output_dfp1;
    int output_dfp2;
    int output_lcd1;
    int output_tv1;

    Rotation rotation;
    void (*PointerMoved)(int, int, int);
    CreateScreenResourcesProcPtr CreateScreenResources;

    /* if no devices are connected at server startup */
    Bool              first_load_no_devices;

    Bool              IsSecondary;
    Bool              IsPrimary;

    Bool              r600_shadow_fb;
    void *fb_shadow;

    int num_gb_pipes;
    Bool has_tcl;
} RADEONInfoRec, *RADEONInfoPtr;

#define RADEONWaitForFifo(pScrn, entries)				\
do {									\
    if (info->fifo_slots < entries)					\
	RADEONWaitForFifoFunction(pScrn, entries);			\
    info->fifo_slots -= entries;					\
} while (0)

/* legacy_crtc.c */
extern void legacy_crtc_dpms(xf86CrtcPtr crtc, int mode);
extern void legacy_crtc_mode_set(xf86CrtcPtr crtc, DisplayModePtr mode,
				 DisplayModePtr adjusted_mode, int x, int y);
extern void RADEONInitDispBandwidth(ScrnInfoPtr pScrn);
extern void RADEONRestoreCommonRegisters(ScrnInfoPtr pScrn,
					 RADEONSavePtr restore);
extern void RADEONRestoreCrtcRegisters(ScrnInfoPtr pScrn,
				       RADEONSavePtr restore);
extern void RADEONRestoreCrtc2Registers(ScrnInfoPtr pScrn,
					RADEONSavePtr restore);
extern void RADEONRestorePLLRegisters(ScrnInfoPtr pScrn,
				      RADEONSavePtr restore);
extern void RADEONRestorePLL2Registers(ScrnInfoPtr pScrn,
				       RADEONSavePtr restore);
extern void RADEONSaveCommonRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save);
extern void RADEONSaveCrtcRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save);
extern void RADEONSaveCrtc2Registers(ScrnInfoPtr pScrn, RADEONSavePtr save);
extern void RADEONSavePLLRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save);
extern void RADEONSavePLL2Registers(ScrnInfoPtr pScrn, RADEONSavePtr save);

/* legacy_output.c */
extern RADEONMonitorType legacy_dac_detect(ScrnInfoPtr pScrn,
					   xf86OutputPtr output);
extern void legacy_output_dpms(xf86OutputPtr output, int mode);
extern void legacy_output_mode_set(xf86OutputPtr output, DisplayModePtr mode,
				   DisplayModePtr adjusted_mode);
extern I2CDevPtr RADEONDVODeviceInit(I2CBusPtr b, I2CSlaveAddr addr);
extern Bool RADEONDVOReadByte(I2CDevPtr dvo, int addr, uint8_t *ch);
extern Bool RADEONDVOWriteByte(I2CDevPtr dvo, int addr, uint8_t ch);
extern void RADEONRestoreDACRegisters(ScrnInfoPtr pScrn, RADEONSavePtr restore);
extern void RADEONRestoreFPRegisters(ScrnInfoPtr pScrn, RADEONSavePtr restore);
extern void RADEONRestoreFP2Registers(ScrnInfoPtr pScrn, RADEONSavePtr restore);
extern void RADEONRestoreLVDSRegisters(ScrnInfoPtr pScrn, RADEONSavePtr restore);
extern void RADEONRestoreRMXRegisters(ScrnInfoPtr pScrn, RADEONSavePtr restore);
extern void RADEONSaveDACRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save);
extern void RADEONSaveFPRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save);

/* radeon_accel.c */
extern Bool RADEONAccelInit(ScreenPtr pScreen);
extern void RADEONEngineFlush(ScrnInfoPtr pScrn);
extern void RADEONEngineInit(ScrnInfoPtr pScrn);
extern void RADEONEngineReset(ScrnInfoPtr pScrn);
extern void RADEONEngineRestore(ScrnInfoPtr pScrn);
extern uint8_t *RADEONHostDataBlit(ScrnInfoPtr pScrn, unsigned int cpp,
				 unsigned int w, uint32_t dstPitchOff,
				 uint32_t *bufPitch, int x, int *y,
				 unsigned int *h, unsigned int *hpass);
extern void RADEONHostDataBlitCopyPass(ScrnInfoPtr pScrn,
				       unsigned int bpp,
				       uint8_t *dst, uint8_t *src,
				       unsigned int hpass,
				       unsigned int dstPitch,
				       unsigned int srcPitch);
extern void  RADEONCopySwap(uint8_t *dst, uint8_t *src, unsigned int size, int swap);
extern void RADEONHostDataParams(ScrnInfoPtr pScrn, uint8_t *dst,
				 uint32_t pitch, int cpp,
				 uint32_t *dstPitchOffset, int *x, int *y);
extern void RADEONInit3DEngine(ScrnInfoPtr pScrn);
extern void RADEONWaitForFifoFunction(ScrnInfoPtr pScrn, int entries);
#ifdef XF86DRI
extern drmBufPtr RADEONCPGetBuffer(ScrnInfoPtr pScrn);
extern void RADEONCPFlushIndirect(ScrnInfoPtr pScrn, int discard);
extern void RADEONCPReleaseIndirect(ScrnInfoPtr pScrn);
extern int RADEONCPStop(ScrnInfoPtr pScrn,  RADEONInfoPtr info);
#  ifdef USE_XAA
extern Bool RADEONSetupMemXAA_DRI(int scrnIndex, ScreenPtr pScreen);
#  endif
#endif

#ifdef USE_XAA
/* radeon_accelfuncs.c */
extern void RADEONAccelInitMMIO(ScreenPtr pScreen, XAAInfoRecPtr a);
extern Bool RADEONSetupMemXAA(int scrnIndex, ScreenPtr pScreen);
#endif

/* radeon_bios.c */
extern Bool RADEONGetBIOSInfo(ScrnInfoPtr pScrn, xf86Int10InfoPtr pInt10);
extern Bool RADEONGetClockInfoFromBIOS(ScrnInfoPtr pScrn);
extern Bool RADEONGetConnectorInfoFromBIOS(ScrnInfoPtr pScrn);
extern Bool RADEONGetDAC2InfoFromBIOS(xf86OutputPtr output);
extern Bool RADEONGetExtTMDSInfoFromBIOS(xf86OutputPtr output);
extern Bool RADEONGetHardCodedEDIDFromBIOS(xf86OutputPtr output);
extern Bool RADEONGetBIOSInitTableOffsets(ScrnInfoPtr pScrn);
extern Bool RADEONGetLVDSInfoFromBIOS(xf86OutputPtr output);
extern Bool RADEONGetTMDSInfoFromBIOS(xf86OutputPtr output);
extern Bool RADEONGetTVInfoFromBIOS(xf86OutputPtr output);
extern Bool RADEONInitExtTMDSInfoFromBIOS (xf86OutputPtr output);
extern Bool RADEONPostCardFromBIOSTables(ScrnInfoPtr pScrn);

/* radeon_commonfuncs.c */
#ifdef XF86DRI
extern void RADEONWaitForIdleCP(ScrnInfoPtr pScrn);
#endif
extern void RADEONWaitForIdleMMIO(ScrnInfoPtr pScrn);

/* radeon_crtc.c */
extern void radeon_crtc_dpms(xf86CrtcPtr crtc, int mode);
extern void radeon_crtc_load_lut(xf86CrtcPtr crtc);
extern void radeon_crtc_modeset_ioctl(xf86CrtcPtr crtc, Bool post);
extern Bool RADEONAllocateControllers(ScrnInfoPtr pScrn, int mask);
extern void RADEONBlank(ScrnInfoPtr pScrn);
extern void RADEONComputePLL(RADEONPLLPtr pll, unsigned long freq,
			     uint32_t *chosen_dot_clock_freq,
			     uint32_t *chosen_feedback_div,
			     uint32_t *chosen_reference_div,
			     uint32_t *chosen_post_div, int flags);
extern DisplayModePtr RADEONCrtcFindClosestMode(xf86CrtcPtr crtc,
						DisplayModePtr pMode);
extern void RADEONUnblank(ScrnInfoPtr pScrn);
extern Bool RADEONSetTiling(ScrnInfoPtr pScrn);

/* radeon_cursor.c */
extern Bool RADEONCursorInit(ScreenPtr pScreen);
extern void radeon_crtc_hide_cursor(xf86CrtcPtr crtc);
extern void radeon_crtc_load_cursor_argb(xf86CrtcPtr crtc, CARD32 *image);
extern void radeon_crtc_set_cursor_colors(xf86CrtcPtr crtc, int bg, int fg);
extern void radeon_crtc_set_cursor_position(xf86CrtcPtr crtc, int x, int y);
extern void radeon_crtc_show_cursor(xf86CrtcPtr crtc);

/* radeon_dga.c */
extern Bool RADEONDGAInit(ScreenPtr pScreen);

#ifdef XF86DRI
/* radeon_dri.c */
extern void RADEONDRIAllocatePCIGARTTable(ScreenPtr pScreen);
extern void RADEONDRICloseScreen(ScreenPtr pScreen);
extern Bool RADEONDRIFinishScreenInit(ScreenPtr pScreen);
extern int RADEONDRIGetPciAperTableSize(ScrnInfoPtr pScrn);
extern Bool RADEONDRIGetVersion(ScrnInfoPtr pScrn);
extern void RADEONDRIResume(ScreenPtr pScreen);
extern Bool RADEONDRIScreenInit(ScreenPtr pScreen);
extern int RADEONDRISetParam(ScrnInfoPtr pScrn,
			     unsigned int param, int64_t value);
extern Bool RADEONDRISetVBlankInterrupt(ScrnInfoPtr pScrn, Bool on);
extern void RADEONDRIStop(ScreenPtr pScreen);
#endif

/* radeon_driver.c */
extern void RADEONDoAdjustFrame(ScrnInfoPtr pScrn, int x, int y, Bool clone);
extern void RADEONChangeSurfaces(ScrnInfoPtr pScrn);
extern RADEONEntPtr RADEONEntPriv(ScrnInfoPtr pScrn);
extern int RADEONMinBits(int val);
extern unsigned RADEONINMC(ScrnInfoPtr pScrn, int addr);
extern unsigned RADEONINPLL(ScrnInfoPtr pScrn, int addr);
extern void RADEONOUTMC(ScrnInfoPtr pScrn, int addr, uint32_t data);
extern void RADEONOUTPLL(ScrnInfoPtr pScrn, int addr, uint32_t data);
extern void RADEONPllErrataAfterData(RADEONInfoPtr info);
extern void RADEONPllErrataAfterIndex(RADEONInfoPtr info);
extern void RADEONWaitForVerticalSync(ScrnInfoPtr pScrn);
extern void RADEONWaitForVerticalSync2(ScrnInfoPtr pScrn);
extern void RADEONInitMemMapRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save,
				      RADEONInfoPtr info);
extern void RADEONRestoreMemMapRegisters(ScrnInfoPtr pScrn,
					 RADEONSavePtr restore);

#ifdef USE_EXA
/* radeon_exa.c */
extern Bool RADEONSetupMemEXA(ScreenPtr pScreen);

/* radeon_exa_funcs.c */
extern void RADEONCopyCP(PixmapPtr pDst, int srcX, int srcY, int dstX,
			 int dstY, int w, int h);
extern void RADEONCopyMMIO(PixmapPtr pDst, int srcX, int srcY, int dstX,
			   int dstY, int w, int h);
extern Bool RADEONDrawInitCP(ScreenPtr pScreen);
extern Bool RADEONDrawInitMMIO(ScreenPtr pScreen);
extern void RADEONDoPrepareCopyCP(ScrnInfoPtr pScrn,
				  uint32_t src_pitch_offset,
				  uint32_t dst_pitch_offset,
				  uint32_t datatype, int rop,
				  Pixel planemask);
extern void RADEONDoPrepareCopyMMIO(ScrnInfoPtr pScrn,
				    uint32_t src_pitch_offset,
				    uint32_t dst_pitch_offset,
				    uint32_t datatype, int rop,
				    Pixel planemask);
#endif

#if defined(XF86DRI) && defined(USE_EXA)
/* radeon_exa.c */
extern Bool RADEONGetDatatypeBpp(int bpp, uint32_t *type);
extern Bool RADEONGetPixmapOffsetPitch(PixmapPtr pPix,
				       uint32_t *pitch_offset);
extern unsigned long long RADEONTexOffsetStart(PixmapPtr pPix);
#endif

/* radeon_modes.c */
extern void RADEONSetPitch(ScrnInfoPtr pScrn);
extern DisplayModePtr RADEONProbeOutputModes(xf86OutputPtr output);

/* radeon_output.c */
extern RADEONI2CBusRec atom_setup_i2c_bus(int ddc_line);
extern RADEONI2CBusRec legacy_setup_i2c_bus(int ddc_line);
extern void RADEONGetPanelInfo(ScrnInfoPtr pScrn);
extern void RADEONInitConnector(xf86OutputPtr output);
extern void RADEONPrintPortMap(ScrnInfoPtr pScrn);
extern void RADEONSetOutputType(ScrnInfoPtr pScrn,
				RADEONOutputPrivatePtr radeon_output);
extern Bool RADEONSetupConnectors(ScrnInfoPtr pScrn);

/* radeon_tv.c */
extern void RADEONSaveTVRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save);
extern void RADEONAdjustCrtcRegistersForTV(ScrnInfoPtr pScrn, RADEONSavePtr save,
					   DisplayModePtr mode, xf86OutputPtr output);
extern void RADEONAdjustPLLRegistersForTV(ScrnInfoPtr pScrn, RADEONSavePtr save,
					  DisplayModePtr mode, xf86OutputPtr output);
extern void RADEONAdjustCrtc2RegistersForTV(ScrnInfoPtr pScrn, RADEONSavePtr save,
					   DisplayModePtr mode, xf86OutputPtr output);
extern void RADEONAdjustPLL2RegistersForTV(ScrnInfoPtr pScrn, RADEONSavePtr save,
					  DisplayModePtr mode, xf86OutputPtr output);
extern void RADEONInitTVRegisters(xf86OutputPtr output, RADEONSavePtr save,
                                  DisplayModePtr mode, BOOL IsPrimary);
extern void RADEONRestoreTVRegisters(ScrnInfoPtr pScrn, RADEONSavePtr restore);
extern void RADEONUpdateHVPosition(xf86OutputPtr output, DisplayModePtr mode);

/* radeon_video.c */
extern void RADEONInitVideo(ScreenPtr pScreen);
extern void RADEONResetVideo(ScrnInfoPtr pScrn);

#ifdef XF86DRI
#  ifdef USE_XAA
/* radeon_accelfuncs.c */
extern void RADEONAccelInitCP(ScreenPtr pScreen, XAAInfoRecPtr a);
#  endif

#define RADEONCP_START(pScrn, info)					\
do {									\
    int _ret = drmCommandNone(info->drmFD, DRM_RADEON_CP_START);	\
    if (_ret) {								\
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,				\
		   "%s: CP start %d\n", __FUNCTION__, _ret);		\
    }									\
    info->CPStarted = TRUE;                                             \
} while (0)

#define RADEONCP_RELEASE(pScrn, info)					\
do {									\
    if (info->CPInUse) {						\
	RADEON_PURGE_CACHE();						\
	RADEON_WAIT_UNTIL_IDLE();					\
	RADEONCPReleaseIndirect(pScrn);					\
	info->CPInUse = FALSE;						\
    }									\
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
	if (info->needCacheFlush) {					\
	    RADEON_PURGE_CACHE();					\
	    RADEON_PURGE_ZCACHE();					\
	    info->needCacheFlush = FALSE;				\
	}								\
	RADEON_WAIT_UNTIL_IDLE();					\
        if (info->ChipFamily <= CHIP_FAMILY_RV280) {                    \
	    BEGIN_RING(6);						\
	    OUT_RING_REG(RADEON_RE_TOP_LEFT,     info->re_top_left);	\
	    OUT_RING_REG(RADEON_RE_WIDTH_HEIGHT, info->re_width_height); \
	    OUT_RING_REG(RADEON_AUX_SC_CNTL,     info->aux_sc_cntl);	\
	    ADVANCE_RING();						\
        } else {                                                        \
            BEGIN_RING(4);                                              \
            OUT_RING_REG(R300_SC_SCISSOR0, info->re_top_left);          \
	    OUT_RING_REG(R300_SC_SCISSOR1, info->re_width_height);      \
            ADVANCE_RING();                                             \
	}                                                               \
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

#define RING_LOCALS	uint32_t *__head = NULL; int __expected; int __count = 0

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
    } else if (info->indirectBuffer->used + (n) * (int)sizeof(uint32_t) >	\
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
		   __count * (int)sizeof(uint32_t));			\
    }									\
    info->indirectBuffer->used += __count * (int)sizeof(uint32_t);	\
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

#define RADEON_PURGE_CACHE()						\
do {									\
    BEGIN_RING(2);							\
    if (info->ChipFamily <= CHIP_FAMILY_RV280) {                        \
        OUT_RING(CP_PACKET0(RADEON_RB3D_DSTCACHE_CTLSTAT, 0));		\
        OUT_RING(RADEON_RB3D_DC_FLUSH_ALL);				\
    } else {                                                            \
        OUT_RING(CP_PACKET0(R300_RB3D_DSTCACHE_CTLSTAT, 0));		\
        OUT_RING(R300_RB3D_DC_FLUSH_ALL);				\
    }                                                                   \
    ADVANCE_RING();							\
} while (0)

#define RADEON_PURGE_ZCACHE()						\
do {									\
    BEGIN_RING(2);							\
    if (info->ChipFamily <= CHIP_FAMILY_RV280) {                        \
        OUT_RING(CP_PACKET0(RADEON_RB3D_ZCACHE_CTLSTAT, 0));		\
        OUT_RING(RADEON_RB3D_ZC_FLUSH_ALL);				\
    } else {                                                            \
        OUT_RING(CP_PACKET0(R300_RB3D_ZCACHE_CTLSTAT, 0));		\
        OUT_RING(R300_ZC_FLUSH_ALL);					\
    }                                                                   \
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

static __inline__ void radeon_init_timeout(struct timeval *endtime,
    unsigned int timeout)
{
    gettimeofday(endtime, NULL);
    endtime->tv_usec += timeout;
    endtime->tv_sec += endtime->tv_usec / 1000000;
    endtime->tv_usec %= 1000000;
}

static __inline__ int radeon_timedout(const struct timeval *endtime)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec == endtime->tv_sec ?
        now.tv_usec > endtime->tv_usec : now.tv_sec > endtime->tv_sec;
}

#endif /* _RADEON_H_ */
