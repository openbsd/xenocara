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
#include "dri.h"
#include "GL/glxint.h"
#include "xf86drm.h"
#include "radeon_drm.h"

#ifdef DAMAGE
#include "damage.h"
#include "globals.h"
#endif
#endif

#include "xf86Crtc.h"
#include "X11/Xatom.h"

#ifdef XF86DRM_MODE
#include "radeon_bo.h"
#include "radeon_cs.h"
#include "radeon_dri2.h"
#include "drmmode_display.h"
#include "radeon_surface.h"
#else
#include "radeon_dummy_bufmgr.h"
#endif

				/* Render support */
#ifdef RENDER
#include "picturestr.h"
#endif

#include "simple_list.h"
#include "atipcirename.h"

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)>(b)?(b):(a))
#endif

#if HAVE_BYTESWAP_H
#include <byteswap.h>
#elif defined(USE_SYS_ENDIAN_H)
#include <sys/endian.h>
#else
#define bswap_16(value)  \
        ((((value) & 0xff) << 8) | ((value) >> 8))

#define bswap_32(value) \
        (((uint32_t)bswap_16((uint16_t)((value) & 0xffff)) << 16) | \
        (uint32_t)bswap_16((uint16_t)((value) >> 16)))
 
#define bswap_64(value) \
        (((uint64_t)bswap_32((uint32_t)((value) & 0xffffffff)) \
            << 32) | \
        (uint64_t)bswap_32((uint32_t)((value) >> 32)))
#endif

#if X_BYTE_ORDER == X_BIG_ENDIAN
#define le32_to_cpu(x) bswap_32(x)
#define le16_to_cpu(x) bswap_16(x)
#define cpu_to_le32(x) bswap_32(x)
#define cpu_to_le16(x) bswap_16(x)
#else
#define le32_to_cpu(x) (x)
#define le16_to_cpu(x) (x)
#define cpu_to_le32(x) (x)
#define cpu_to_le16(x) (x)
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
    OPTION_EXA_PIXMAPS,
#endif
#endif
    OPTION_IGNORE_EDID,
    OPTION_CUSTOM_EDID,
    OPTION_DISP_PRIORITY,
    OPTION_PANEL_SIZE,
    OPTION_MIN_DOTCLOCK,
    OPTION_COLOR_TILING,
    OPTION_COLOR_TILING_2D,
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
    OPTION_CLOCK_GATING,
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
    OPTION_INT10,
    OPTION_EXA_VSYNC,
    OPTION_ATOM_TVOUT,
    OPTION_R4XX_ATOM,
    OPTION_FORCE_LOW_POWER,
    OPTION_DYNAMIC_PM,
    OPTION_NEW_PLL,
    OPTION_ZAPHOD_HEADS,
    OPTION_SWAPBUFFERS_WAIT
} RADEONOpts;


#define RADEON_IDLE_RETRY      16 /* Fall out of idle loops after this count */
#define RADEON_TIMEOUT    2000000 /* Fall out of wait loops after this count */

#define RADEON_VSYNC_TIMEOUT	20000 /* Maximum wait for VSYNC (in usecs) */

/* Buffer are aligned on 4096 byte boundaries */
#define RADEON_GPU_PAGE_SIZE 4096
#define RADEON_BUFFER_ALIGN (RADEON_GPU_PAGE_SIZE - 1)
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
#define RADEON_PLL_PREFER_LOW_REF_DIV   (1 << 4)
#define RADEON_PLL_PREFER_HIGH_REF_DIV  (1 << 5)
#define RADEON_PLL_PREFER_LOW_FB_DIV    (1 << 6)
#define RADEON_PLL_PREFER_HIGH_FB_DIV   (1 << 7)
#define RADEON_PLL_PREFER_LOW_POST_DIV  (1 << 8)
#define RADEON_PLL_PREFER_HIGH_POST_DIV (1 << 9)
#define RADEON_PLL_USE_FRAC_FB_DIV      (1 << 10)
#define RADEON_PLL_PREFER_CLOSEST_LOWER (1 << 11)
#define RADEON_PLL_USE_POST_DIV    (1 << 12)

typedef struct {
    uint32_t          reference_freq;
    uint32_t          reference_div;
    uint32_t          post_div;
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
    uint32_t          min_frac_feedback_div;
    uint32_t          max_frac_feedback_div;
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

#define IS_DCE32_VARIANT ((info->ChipFamily >= CHIP_FAMILY_RV730))

#define IS_DCE4_VARIANT ((info->ChipFamily >= CHIP_FAMILY_CEDAR))

#define IS_DCE41_VARIANT ((info->ChipFamily >= CHIP_FAMILY_PALM))

#define IS_DCE5_VARIANT ((info->ChipFamily >= CHIP_FAMILY_BARTS))

#define IS_EVERGREEN_3D (info->ChipFamily >= CHIP_FAMILY_CEDAR)

#define IS_R600_3D (info->ChipFamily >= CHIP_FAMILY_R600)

#define IS_R500_3D ((info->ChipFamily == CHIP_FAMILY_RV515)  ||  \
	(info->ChipFamily == CHIP_FAMILY_R520)   ||  \
	(info->ChipFamily == CHIP_FAMILY_RV530)  ||  \
	(info->ChipFamily == CHIP_FAMILY_R580)   ||  \
	(info->ChipFamily == CHIP_FAMILY_RV560)  ||  \
	(info->ChipFamily == CHIP_FAMILY_RV570))

#define IS_R400_3D ((info->ChipFamily == CHIP_FAMILY_R420)  ||  \
	(info->ChipFamily == CHIP_FAMILY_RV410) ||  \
	(info->ChipFamily == CHIP_FAMILY_RS690) ||  \
	(info->ChipFamily == CHIP_FAMILY_RS600) ||  \
	(info->ChipFamily == CHIP_FAMILY_RS740))

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

#define IS_R200_3D ((info->ChipFamily == CHIP_FAMILY_RV250) || \
	(info->ChipFamily == CHIP_FAMILY_RV280) || \
	(info->ChipFamily == CHIP_FAMILY_RS300) || \
	(info->ChipFamily == CHIP_FAMILY_R200))

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
    RADEON_MAC_IMAC_G5_ISIGHT,
    RADEON_MAC_EMAC
} RADEONMacModel;
#endif

typedef enum {
	CARD_PCI,
	CARD_AGP,
	CARD_PCIE
} RADEONCardType;

typedef enum {
	POWER_DEFAULT,
	POWER_LOW,
	POWER_HIGH
} RADEONPMType;

typedef struct {
    RADEONPMType type;
    uint32_t sclk;
    uint32_t mclk;
    uint32_t pcie_lanes;
    uint32_t flags;
} RADEONPowerMode;

typedef struct {
    /* power modes */
    int num_modes;
    int current_mode;
    RADEONPowerMode mode[3];

    Bool     clock_gating_enabled;
    Bool     dynamic_mode_enabled;
    Bool     force_low_power_enabled;
} RADEONPowerManagement;

typedef struct _atomBiosHandle *atomBiosHandlePtr;

struct radeon_exa_pixmap_priv {
    struct radeon_bo *bo;
    uint32_t tiling_flags;
#ifdef XF86DRM_MODE
    struct radeon_surface surface;
#endif
    Bool bo_mapped;
};

#define RADEON_2D_EXA_COPY 1
#define RADEON_2D_EXA_SOLID 2

struct radeon_2d_state {
    int op; //
    uint32_t dst_pitch_offset;
    uint32_t src_pitch_offset;
    uint32_t dp_gui_master_cntl;
    uint32_t dp_cntl;
    uint32_t dp_write_mask;
    uint32_t dp_brush_frgd_clr;
    uint32_t dp_brush_bkgd_clr;
    uint32_t dp_src_frgd_clr;
    uint32_t dp_src_bkgd_clr;
    uint32_t default_sc_bottom_right;
    struct radeon_bo *dst_bo;
    struct radeon_bo *src_bo;
};
    
#ifdef XF86DRI
struct radeon_cp {
    Bool              CPRuns;           /* CP is running */
    Bool              CPInUse;          /* CP has been used by X server */
    Bool              CPStarted;        /* CP has started */
    int               CPMode;           /* CP mode that server/clients use */
    int               CPFifoSize;       /* Size of the CP command FIFO */
    int               CPusecTimeout;    /* CP timeout in usecs */
    Bool              needCacheFlush;

    /* CP accleration */
    drmBufPtr         indirectBuffer;
    int               indirectStart;

    /* Debugging info for BEGIN_RING/ADVANCE_RING pairs. */
    int               dma_begin_count;
    char              *dma_debug_func;
    int               dma_debug_lineno;

    };

typedef struct {
    /* Nothing here yet */
    int dummy;
} RADEONConfigPrivRec, *RADEONConfigPrivPtr;

typedef struct {
    /* Nothing here yet */
    int dummy;
} RADEONDRIContextRec, *RADEONDRIContextPtr;

struct radeon_dri {
    Bool              noBackBuffer;

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

    int               irq;

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

};
#endif

#define DMA_BO_FREE_TIME 1000

struct radeon_dma_bo {
    struct radeon_dma_bo *next, *prev;
    struct radeon_bo  *bo;
    int expire_counter;
};

struct r600_accel_object {
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint32_t offset;
    int bpp;
    uint32_t domain;
    struct radeon_bo *bo;
    uint32_t tiling_flags;
#if defined(XF86DRM_MODE)
    struct radeon_surface *surface;
#endif
};

struct radeon_vbo_object {
    int               vb_offset;
    uint64_t          vb_mc_addr;
    int               vb_total;
    void              *vb_ptr;
    uint32_t          vb_size;
    uint32_t          vb_op_vert_size;
    int32_t           vb_start_op;
    struct radeon_bo *vb_bo;
    unsigned          verts_per_op;
};

struct radeon_accel_state {
    /* common accel data */
    int               fifo_slots;       /* Free slots in the FIFO (64 max)   */
				/* Computed values for Radeon */
    uint32_t          dp_gui_master_cntl;
    uint32_t          dp_gui_master_cntl_clip;
    uint32_t          trans_color;
				/* Saved values for ScreenToScreenCopy */
    int               xdir;
    int               ydir;
    uint32_t          dst_pitch_offset;

    /* render accel */
    unsigned short    texW[2];
    unsigned short    texH[2];
    Bool              XInited3D; /* X itself has the 3D context */
    int               num_gb_pipes;
    Bool              has_tcl;
    Bool              allowHWDFS;

#ifdef USE_EXA
    /* EXA */
    ExaDriverPtr      exa;
    int               exaSyncMarker;
    int               exaMarkerSynced;
    int               engineMode;
#define EXA_ENGINEMODE_UNKNOWN 0
#define EXA_ENGINEMODE_2D      1
#define EXA_ENGINEMODE_3D      2

    int               composite_op;
    PicturePtr        dst_pic;
    PicturePtr        msk_pic;
    PicturePtr        src_pic;
    PixmapPtr         dst_pix;
    PixmapPtr         msk_pix;
    PixmapPtr         src_pix;
    Bool              is_transform[2];
    PictTransform     *transform[2];
    /* Whether we are tiling horizontally and vertically */
    Bool              need_src_tile_x;
    Bool              need_src_tile_y;
    /* Size of tiles ... set to 65536x65536 if not tiling in that direction */
    Bool              src_tile_width;
    Bool              src_tile_height;
    uint32_t          *draw_header;
    unsigned          vtx_count;
    unsigned          num_vtx;
    Bool              vsync;

    drmBufPtr         ib;

    struct radeon_vbo_object vbo;
    struct radeon_vbo_object cbuf;

    /* where to discard IB from if we cancel operation */
    uint32_t          ib_reset_op;
#ifdef XF86DRM_MODE
    struct radeon_dma_bo bo_free;
    struct radeon_dma_bo bo_wait;
    struct radeon_dma_bo bo_reserved;
    Bool use_vbos;
#endif
    void (*finish_op)(ScrnInfoPtr, int);
    // shader storage
    ExaOffscreenArea  *shaders;
    struct radeon_bo  *shaders_bo;
    uint32_t          solid_vs_offset;
    uint32_t          solid_ps_offset;
    uint32_t          copy_vs_offset;
    uint32_t          copy_ps_offset;
    uint32_t          comp_vs_offset;
    uint32_t          comp_ps_offset;
    uint32_t          xv_vs_offset;
    uint32_t          xv_ps_offset;
    // shader consts
    uint32_t          solid_vs_const_offset;
    uint32_t          solid_ps_const_offset;
    uint32_t          copy_vs_const_offset;
    uint32_t          copy_ps_const_offset;
    uint32_t          comp_vs_const_offset;
    uint32_t          comp_ps_const_offset;
    uint32_t          comp_mask_ps_const_offset;
    uint32_t          xv_vs_const_offset;
    uint32_t          xv_ps_const_offset;

    //size/addr stuff
    struct r600_accel_object src_obj[2];
    struct r600_accel_object dst_obj;
    uint32_t          src_size[2];
    uint32_t          dst_size;

    uint32_t          vs_size;
    uint64_t          vs_mc_addr;
    uint32_t          ps_size;
    uint64_t          ps_mc_addr;

    // UTS/DFS
    drmBufPtr         scratch;

    // solid/copy
    ExaOffscreenArea  *copy_area;
    struct radeon_bo  *copy_area_bo;
    Bool              same_surface;
    int               rop;
    uint32_t          planemask;
    uint32_t          fg;

    // composite
    Bool              component_alpha;
    Bool              src_alpha;
    // vline
    xf86CrtcPtr       vline_crtc;
    int               vline_y1;
    int               vline_y2;
#endif

#ifdef USE_XAA
    /* XAA */
    XAAInfoRecPtr     accel;
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
				/* Saved values for DashedTwoPointLine */
    int               dashLen;
    uint32_t          dashPattern;
    int               dash_fg;
    int               dash_bg;

    FBLinearPtr       RenderTex;
    void              (*RenderCallback)(ScrnInfoPtr);
    Time              RenderTimeout;
    /*
     * XAAForceTransBlit is used to change the behavior of the XAA
     * SetupForScreenToScreenCopy function, to make it DGA-friendly.
     */
    Bool              XAAForceTransBlit;
#endif

};

typedef struct {
    EntityInfoPtr     pEnt;
    pciVideoPtr       PciInfo;
#ifndef XSERVER_LIBPCIACCESS
    PCITAG            PciTag;
#endif
    int               Chipset;
    RADEONChipFamily  ChipFamily;
    RADEONErrata      ChipErrata;

    unsigned long long     LinearAddr;       /* Frame buffer physical address     */
    unsigned long long     MMIOAddr;         /* MMIO region physical address      */
    unsigned long long     BIOSAddr;         /* BIOS physical address             */
    uint64_t          fbLocation;
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
    int               default_dispclk;
    int               dp_extclk;

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

    xf86CursorInfoPtr cursor;
#ifdef ARGB_CURSOR
    Bool	      cursor_argb;
#endif
    int               cursor_fg;
    int               cursor_bg;

    int               pix24bpp;         /* Depth of pixmap for 24bpp fb      */
    Bool              dac6bits;         /* Use 6 bit DAC?                    */

    RADEONFBLayout    CurrentLayout;

#ifdef XF86DRI
    Bool              directRenderingEnabled;
    Bool              directRenderingInited;
    RADEONCardType    cardType;            /* Current card is a PCI card */
    struct radeon_cp  *cp;
    struct radeon_dri  *dri;
#ifdef XF86DRM_MODE
    struct radeon_dri2  dri2;
#endif
#ifdef USE_EXA
    Bool              accelDFS;
#endif
    Bool              DMAForXv;
#endif /* XF86DRI */

    /* accel */
    Bool              RenderAccel; /* Render */
    Bool              allowColorTiling;
    Bool              allowColorTiling2D;
    Bool              tilingEnabled; /* mirror of sarea->tiling_enabled */
    struct radeon_accel_state *accel_state;
    Bool              accelOn;
    Bool              useEXA;
#ifdef USE_EXA
    Bool	      exa_pixmaps;
    Bool              exa_force_create;
    XF86ModReqInfo    exaReq;
#endif
#ifdef USE_XAA
    XF86ModReqInfo    xaaReq;
#endif

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
    unsigned int xv_max_width;
    unsigned int xv_max_height;

    /* general */
    OptionInfoPtr     Options;

    DisplayModePtr currentMode, savedCurrentMode;

    /* special handlings for DELL triple-head server */
    Bool              IsDellServer;

    Bool              VGAAccess;

    int               MaxSurfaceWidth;
    int               MaxLines;

    Bool want_vblank_interrupts;
    RADEONBIOSConnector BiosConnector[RADEON_MAX_BIOS_CONNECTOR];
    radeon_encoder_ptr encoders[RADEON_MAX_BIOS_CONNECTOR];
    RADEONBIOSInitTable BiosTable;

    /* save crtc state for console restore */
    Bool              crtc_on;
    Bool              crtc2_on;

    Bool              InternalTVOut;

#if defined(__powerpc__)
    RADEONMacModel    MacModel;
#endif
    RADEONExtTMDSChip ext_tmds_chip;

    atomBiosHandlePtr atomBIOS;
    unsigned long FbFreeStart, FbFreeSize;
    unsigned char*      BIOSCopy;

    CreateScreenResourcesProcPtr CreateScreenResources;

    /* if no devices are connected at server startup */
    Bool              first_load_no_devices;

    Bool              IsSecondary;
    Bool              IsPrimary;

    Bool              r600_shadow_fb;
    void *fb_shadow;

    /* some server chips have a hardcoded edid in the bios so that they work with KVMs */
    Bool get_hardcoded_edid_from_bios;

    int               virtualX;
    int               virtualY;

    Bool              r4xx_atom;

    /* pm */
    RADEONPowerManagement pm;

    /* igp info */
    float igp_sideport_mclk;
    float igp_system_mclk;
    float igp_ht_link_clk;
    float igp_ht_link_width;

    int can_resize;
    void (*reemit_current2d)(ScrnInfoPtr pScrn, int op); // emit the current 2D state into the IB 
    struct radeon_2d_state state_2d;
    Bool kms_enabled;
    struct radeon_bo *front_bo;
#ifdef XF86DRM_MODE
    struct radeon_bo_manager *bufmgr;
    struct radeon_cs_manager *csm;
    struct radeon_cs *cs;

    struct radeon_bo *cursor_bo[32];
    uint64_t vram_size;
    uint64_t gart_size;
    drmmode_rec drmmode;
    /* r6xx+ tile config */
    Bool have_tiling_info;
    uint32_t tile_config;
    int group_bytes;
    int num_channels;
    int num_banks;
    int r7xx_bank_op;
    struct radeon_surface_manager *surf_man;
    struct radeon_surface front_surface;
#else
    /* fake bool */
    Bool cs;
#endif

    /* Xv bicubic filtering */
    struct radeon_bo *bicubic_bo;
    void             *bicubic_memory;
    int               bicubic_offset;
    /* kms pageflipping */
    Bool allowPageFlip;

    /* Perform vsync'ed SwapBuffers? */
    Bool swapBuffersWait;
} RADEONInfoRec, *RADEONInfoPtr;

#define RADEONWaitForFifo(pScrn, entries)				\
do {									\
    if (info->accel_state->fifo_slots < entries)			\
	RADEONWaitForFifoFunction(pScrn, entries);			\
    info->accel_state->fifo_slots -= entries;				\
} while (0)

/* legacy_crtc.c */
extern void legacy_crtc_dpms(xf86CrtcPtr crtc, int mode);
extern void legacy_crtc_mode_set(xf86CrtcPtr crtc, DisplayModePtr mode,
				 DisplayModePtr adjusted_mode, int x, int y);
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
extern RADEONMonitorType legacy_dac_detect(xf86OutputPtr output);
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
extern void radeon_save_palette_on_demand(ScrnInfoPtr pScrn, int palID);

extern void RADEONGetTVDacAdjInfo(ScrnInfoPtr pScrn, radeon_tvdac_ptr tvdac);
extern void RADEONGetTMDSInfoFromTable(ScrnInfoPtr pScrn, radeon_tmds_ptr tmds);
extern void RADEONGetTMDSInfo(ScrnInfoPtr pScrn, radeon_tmds_ptr tmds);
extern void RADEONGetExtTMDSInfo(ScrnInfoPtr pScrn, radeon_dvo_ptr dvo);
extern void RADEONGetLVDSInfo(ScrnInfoPtr pScrn, radeon_lvds_ptr lvds);

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
uint32_t radeonGetPixmapOffset(PixmapPtr pPix);
#endif
extern int radeon_cs_space_remaining(ScrnInfoPtr pScrn);

#ifdef USE_XAA
/* radeon_accelfuncs.c */
extern void RADEONAccelInitMMIO(ScreenPtr pScreen, XAAInfoRecPtr a);
extern Bool RADEONSetupMemXAA(int scrnIndex, ScreenPtr pScreen);
#endif

/* radeon_bios.c */
extern Bool RADEONGetBIOSInfo(ScrnInfoPtr pScrn, xf86Int10InfoPtr pInt10);
extern Bool RADEONGetClockInfoFromBIOS(ScrnInfoPtr pScrn);
extern Bool RADEONGetConnectorInfoFromBIOS(ScrnInfoPtr pScrn);
extern Bool RADEONGetDAC2InfoFromBIOS(ScrnInfoPtr pScrn, radeon_tvdac_ptr tvdac);
extern Bool RADEONGetExtTMDSInfoFromBIOS (ScrnInfoPtr pScrn, radeon_dvo_ptr dvo);
extern xf86MonPtr RADEONGetHardCodedEDIDFromBIOS(xf86OutputPtr output);
extern Bool RADEONGetBIOSInitTableOffsets(ScrnInfoPtr pScrn);
extern Bool RADEONGetLVDSInfoFromBIOS(ScrnInfoPtr pScrn, radeon_lvds_ptr lvds);
extern Bool RADEONGetTMDSInfoFromBIOS(ScrnInfoPtr pScrn, radeon_tmds_ptr tmds);
extern Bool RADEONGetTVInfoFromBIOS(xf86OutputPtr output);
extern Bool RADEONInitExtTMDSInfoFromBIOS (xf86OutputPtr output);
extern Bool RADEONPostCardFromBIOSTables(ScrnInfoPtr pScrn);
extern Bool radeon_card_posted(ScrnInfoPtr pScrn);

/* radeon_commonfuncs.c */
#ifdef XF86DRI
extern void RADEONWaitForIdleCP(ScrnInfoPtr pScrn);
extern void RADEONWaitForVLineCP(ScrnInfoPtr pScrn, PixmapPtr pPix,
				 xf86CrtcPtr crtc, int start, int stop);
#endif
extern void RADEONWaitForIdleMMIO(ScrnInfoPtr pScrn);
extern void RADEONWaitForVLineMMIO(ScrnInfoPtr pScrn, PixmapPtr pPix,
				   xf86CrtcPtr crtc, int start, int stop);

/* radeon_crtc.c */
extern void radeon_crtc_dpms(xf86CrtcPtr crtc, int mode);
extern void radeon_do_crtc_dpms(xf86CrtcPtr crtc, int mode);
extern void radeon_crtc_load_lut(xf86CrtcPtr crtc);
extern void radeon_crtc_modeset_ioctl(xf86CrtcPtr crtc, Bool post);
extern Bool RADEONAllocateControllers(ScrnInfoPtr pScrn, int mask);
extern void RADEONBlank(ScrnInfoPtr pScrn);
extern void RADEONComputePLL(xf86CrtcPtr crtc,
			     RADEONPLLPtr pll, unsigned long freq,
			     uint32_t *chosen_dot_clock_freq,
			     uint32_t *chosen_feedback_div,
			     uint32_t *chosen_frac_feedback_div,
			     uint32_t *chosen_reference_div,
			     uint32_t *chosen_post_div, int flags);
extern DisplayModePtr RADEONCrtcFindClosestMode(xf86CrtcPtr crtc,
						DisplayModePtr pMode);
extern void RADEONUnblank(ScrnInfoPtr pScrn);
extern Bool RADEONSetTiling(ScrnInfoPtr pScrn);
extern void RADEONInitDispBandwidth(ScrnInfoPtr pScrn);

/* radeon_cursor.c */
extern Bool RADEONCursorInit(ScreenPtr pScreen);
extern void radeon_crtc_hide_cursor(xf86CrtcPtr crtc);
extern void radeon_crtc_load_cursor_argb(xf86CrtcPtr crtc, CARD32 *image);
extern void radeon_crtc_set_cursor_colors(xf86CrtcPtr crtc, int bg, int fg);
extern void radeon_crtc_set_cursor_position(xf86CrtcPtr crtc, int x, int y);
extern void radeon_crtc_show_cursor(xf86CrtcPtr crtc);

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
extern unsigned RADEONINPCIE(ScrnInfoPtr pScrn, int addr);
extern unsigned R600INPCIE_PORT(ScrnInfoPtr pScrn, int addr);
extern void RADEONOUTMC(ScrnInfoPtr pScrn, int addr, uint32_t data);
extern void RADEONOUTPLL(ScrnInfoPtr pScrn, int addr, uint32_t data);
extern void RADEONOUTPCIE(ScrnInfoPtr pScrn, int addr, uint32_t data);
extern void R600OUTPCIE_PORT(ScrnInfoPtr pScrn, int addr, uint32_t data);
extern void RADEONPllErrataAfterData(RADEONInfoPtr info);
extern void RADEONPllErrataAfterIndex(RADEONInfoPtr info);
extern void RADEONWaitForVerticalSync(ScrnInfoPtr pScrn);
extern void RADEONWaitForVerticalSync2(ScrnInfoPtr pScrn);
extern void RADEONInitMemMapRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save,
				      RADEONInfoPtr info);
extern void RADEONRestoreMemMapRegisters(ScrnInfoPtr pScrn,
					 RADEONSavePtr restore);
extern Bool
RADEONZaphodStringMatches(ScrnInfoPtr pScrn, const char *s, char *output_name);

Bool RADEONGetRec(ScrnInfoPtr pScrn);
void RADEONFreeRec(ScrnInfoPtr pScrn);
Bool RADEONPreInitVisual(ScrnInfoPtr pScrn);
Bool RADEONPreInitWeight(ScrnInfoPtr pScrn);

extern Bool RADEON_DP_I2CInit(ScrnInfoPtr pScrn, I2CBusPtr *bus_ptr,
			      char *name, xf86OutputPtr output);
extern void RADEON_DP_GetDPCD(xf86OutputPtr output);
extern int RADEON_DP_GetSinkType(xf86OutputPtr output);

/* radeon_pm.c */
extern void RADEONPMInit(ScrnInfoPtr pScrn);
extern void RADEONPMBlockHandler(ScrnInfoPtr pScrn);
extern void RADEONPMEnterVT(ScrnInfoPtr pScrn);
extern void RADEONPMLeaveVT(ScrnInfoPtr pScrn);
extern void RADEONPMFini(ScrnInfoPtr pScrn);

#ifdef USE_EXA
/* radeon_exa.c */
extern unsigned eg_tile_split(unsigned tile_split);
extern Bool RADEONSetupMemEXA(ScreenPtr pScreen);
extern Bool radeon_transform_is_affine_or_scaled(PictTransformPtr t);

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
extern Bool R600DrawInit(ScreenPtr pScreen);
extern Bool R600LoadShaders(ScrnInfoPtr pScrn);
#ifdef XF86DRM_MODE
extern Bool EVERGREENDrawInit(ScreenPtr pScreen);
#endif
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
extern Bool RADEONI2CDoLock(xf86OutputPtr output, I2CBusPtr b, Bool lock_state);

extern Bool radeon_dp_mode_fixup(xf86OutputPtr output, DisplayModePtr mode, DisplayModePtr adjusted_mode);

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
extern Bool radeon_load_bicubic_texture(ScrnInfoPtr pScrn);
extern xf86CrtcPtr radeon_pick_best_crtc(ScrnInfoPtr pScrn,
					 int x1, int x2, int y1, int y2);

/* radeon_legacy_memory.c */
extern uint32_t
radeon_legacy_allocate_memory(ScrnInfoPtr pScrn,
			      void **mem_struct,
			      int size,
			      int align,
			      int domain);
extern void
radeon_legacy_free_memory(ScrnInfoPtr pScrn,
		          void *mem_struct);

#ifdef XF86DRM_MODE
extern void radeon_cs_flush_indirect(ScrnInfoPtr pScrn);
extern void radeon_ddx_cs_start(ScrnInfoPtr pScrn,
				int num, const char *file,
				const char *func, int line);
void radeon_kms_update_vram_limit(ScrnInfoPtr pScrn, int new_fb_size);
struct radeon_surface *radeon_get_pixmap_surface(PixmapPtr pPix);
#endif
struct radeon_bo *radeon_get_pixmap_bo(PixmapPtr pPix);
void radeon_set_pixmap_bo(PixmapPtr pPix, struct radeon_bo *bo);
uint32_t radeon_get_pixmap_tiling(PixmapPtr pPix);

#ifdef XF86DRI
#  ifdef USE_XAA
/* radeon_accelfuncs.c */
extern void RADEONAccelInitCP(ScreenPtr pScreen, XAAInfoRecPtr a);
#  endif

#define RADEONCP_START(pScrn, info)					\
do {									\
    int _ret = drmCommandNone(info->dri->drmFD, DRM_RADEON_CP_START);	\
    if (_ret) {								\
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,				\
		   "%s: CP start %d\n", __FUNCTION__, _ret);		\
    }									\
    info->cp->CPStarted = TRUE;                                         \
} while (0)

#define RADEONCP_RELEASE(pScrn, info)					\
do {									\
    if (info->cs) {							\
	radeon_cs_flush_indirect(pScrn);				\
    } else if (info->cp->CPInUse) {					\
	RADEON_PURGE_CACHE();						\
	RADEON_WAIT_UNTIL_IDLE();					\
	RADEONCPReleaseIndirect(pScrn);					\
	info->cp->CPInUse = FALSE;				        \
    }									\
} while (0)

#define RADEONCP_STOP(pScrn, info)					\
do {									\
    int _ret;								\
    if (info->cp->CPStarted) {						\
        _ret = RADEONCPStop(pScrn, info);				\
        if (_ret) {							\
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,			\
		   "%s: CP stop %d\n", __FUNCTION__, _ret);		\
        }								\
        info->cp->CPStarted = FALSE;                                    \
    }									\
    if (info->ChipFamily < CHIP_FAMILY_R600)                            \
        RADEONEngineRestore(pScrn);					\
    info->cp->CPRuns = FALSE;						\
} while (0)

#define RADEONCP_RESET(pScrn, info)					\
do {									\
	int _ret = drmCommandNone(info->dri->drmFD, DRM_RADEON_CP_RESET);	\
	if (_ret) {							\
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,			\
		       "%s: CP reset %d\n", __FUNCTION__, _ret);	\
	}								\
} while (0)

#define RADEONCP_REFRESH(pScrn, info)					\
do {									\
    if (!info->cp->CPInUse && !info->cs) {				\
	if (info->cp->needCacheFlush) {					\
	    RADEON_PURGE_CACHE();					\
	    RADEON_PURGE_ZCACHE();					\
	    info->cp->needCacheFlush = FALSE;				\
	}								\
	RADEON_WAIT_UNTIL_IDLE();					\
	info->cp->CPInUse = TRUE;					\
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
    if (info->cs) { radeon_ddx_cs_start(pScrn, n, __FILE__, __func__, __LINE__); } else { \
      if (++info->cp->dma_begin_count != 1) {				\
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,				\
		   "BEGIN_RING without end at %s:%d\n",			\
		   info->cp->dma_debug_func, info->cp->dma_debug_lineno); \
	info->cp->dma_begin_count = 1;					\
      }									\
      info->cp->dma_debug_func = __FILE__;				\
      info->cp->dma_debug_lineno = __LINE__;				\
      if (!info->cp->indirectBuffer) {					\
	info->cp->indirectBuffer = RADEONCPGetBuffer(pScrn);		\
	info->cp->indirectStart = 0;					\
      } else if (info->cp->indirectBuffer->used + (n) * (int)sizeof(uint32_t) >	\
		 info->cp->indirectBuffer->total) {		        \
	RADEONCPFlushIndirect(pScrn, 1);				\
      }									\
      __expected = n;							\
      __head = (pointer)((char *)info->cp->indirectBuffer->address +	\
			 info->cp->indirectBuffer->used);		\
      __count = 0;							\
    }									\
} while (0)

#define ADVANCE_RING() do {						\
    if (info->cs) radeon_cs_end(info->cs, __FILE__, __func__, __LINE__); else {		\
      if (info->cp->dma_begin_count-- != 1) {				\
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,				\
		   "ADVANCE_RING without begin at %s:%d\n",		\
		   __FILE__, __LINE__);					\
	info->cp->dma_begin_count = 0;					\
      }									\
      if (__count != __expected) {					\
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,				\
		   "ADVANCE_RING count != expected (%d vs %d) at %s:%d\n", \
		   __count, __expected, __FILE__, __LINE__);		\
      }									\
      if (RADEON_VERBOSE) {						\
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,				\
		   "ADVANCE_RING() start: %d used: %d count: %d\n",	\
		   info->cp->indirectStart,				\
		   info->cp->indirectBuffer->used,			\
		   __count * (int)sizeof(uint32_t));			\
      }									\
      info->cp->indirectBuffer->used += __count * (int)sizeof(uint32_t); \
    }									\
  } while (0)

#define OUT_RING(x) do {						\
    if (RADEON_VERBOSE) {						\
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,				\
		   "   OUT_RING(0x%08x)\n", (unsigned int)(x));		\
    }									\
    if (info->cs) radeon_cs_write_dword(info->cs, (x)); else		\
    __head[__count++] = (x);						\
} while (0)

#define OUT_RING_REG(reg, val)						\
do {									\
    OUT_RING(CP_PACKET0(reg, 0));					\
    OUT_RING(val);							\
} while (0)

#define OUT_RING_RELOC(x, read_domains, write_domain)			\
  do {									\
	int _ret; \
    _ret = radeon_cs_write_reloc(info->cs, x, read_domains, write_domain, 0); \
	if (_ret) ErrorF("reloc emit failure %d\n", _ret); \
  } while(0)


#define FLUSH_RING()							\
do {									\
    if (RADEON_VERBOSE)							\
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,				\
		   "FLUSH_RING in %s\n", __FUNCTION__);			\
    if (info->cs)							\
	radeon_cs_flush_indirect(pScrn); 				\
    else if (info->cp->indirectBuffer)					\
	RADEONCPFlushIndirect(pScrn, 0);				\
} while (0)


#define RADEON_WAIT_UNTIL_2D_IDLE()					\
do {									\
    if (info->ChipFamily < CHIP_FAMILY_R600) {                          \
	BEGIN_RING(2);                                                  \
	OUT_RING(CP_PACKET0(RADEON_WAIT_UNTIL, 0));                     \
	OUT_RING((RADEON_WAIT_2D_IDLECLEAN |                            \
		  RADEON_WAIT_HOST_IDLECLEAN));                         \
	ADVANCE_RING();                                                 \
    }                                                                   \
} while (0)

#define RADEON_WAIT_UNTIL_3D_IDLE()					\
do {									\
    if (info->ChipFamily < CHIP_FAMILY_R600) {				\
	BEGIN_RING(2);							\
	OUT_RING(CP_PACKET0(RADEON_WAIT_UNTIL, 0));                     \
	OUT_RING((RADEON_WAIT_3D_IDLECLEAN |                            \
		  RADEON_WAIT_HOST_IDLECLEAN));                         \
	ADVANCE_RING();							\
    }                                                                   \
} while (0)

#define RADEON_WAIT_UNTIL_IDLE()					\
do {									\
    if (RADEON_VERBOSE) {						\
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,				\
		   "WAIT_UNTIL_IDLE() in %s\n", __FUNCTION__);		\
    }									\
    if (info->ChipFamily < CHIP_FAMILY_R600) {                          \
	BEGIN_RING(2);							\
	OUT_RING(CP_PACKET0(RADEON_WAIT_UNTIL, 0));                     \
	OUT_RING((RADEON_WAIT_2D_IDLECLEAN |                            \
                  RADEON_WAIT_3D_IDLECLEAN |                            \
		  RADEON_WAIT_HOST_IDLECLEAN));                         \
	ADVANCE_RING();							\
    }                                                                   \
} while (0)

#define RADEON_PURGE_CACHE()						\
do {									\
    if (info->ChipFamily < CHIP_FAMILY_R600) {				\
	BEGIN_RING(2);							\
	if (info->ChipFamily <= CHIP_FAMILY_RV280) {			\
	    OUT_RING(CP_PACKET0(RADEON_RB3D_DSTCACHE_CTLSTAT, 0));	\
	    OUT_RING(RADEON_RB3D_DC_FLUSH_ALL);				\
	} else {							\
	    OUT_RING(CP_PACKET0(R300_RB3D_DSTCACHE_CTLSTAT, 0));	\
	    OUT_RING(R300_RB3D_DC_FLUSH_ALL);				\
	}								\
	ADVANCE_RING();							\
    }									\
} while (0)

#define RADEON_PURGE_ZCACHE()						\
do {									\
    if (info->ChipFamily < CHIP_FAMILY_R600) {                          \
	BEGIN_RING(2);                                                  \
	if (info->ChipFamily <= CHIP_FAMILY_RV280) {                    \
	    OUT_RING(CP_PACKET0(RADEON_RB3D_ZCACHE_CTLSTAT, 0));        \
	    OUT_RING(RADEON_RB3D_ZC_FLUSH_ALL);                         \
	} else {                                                        \
	    OUT_RING(CP_PACKET0(R300_RB3D_ZCACHE_CTLSTAT, 0));          \
	    OUT_RING(R300_ZC_FLUSH_ALL);                                \
	}                                                               \
	ADVANCE_RING();                                                 \
    }                                                                   \
} while (0)

#endif /* XF86DRI */

#if defined(XF86DRI) && defined(USE_EXA)

#ifdef XF86DRM_MODE
#define CS_FULL(cs) ((cs)->cdw > 15 * 1024)
#else
#define CS_FULL(cs) FALSE
#endif

#define RADEON_SWITCH_TO_2D()						\
do {									\
	uint32_t flush = 0;                                             \
	switch (info->accel_state->engineMode) {			\
	case EXA_ENGINEMODE_UNKNOWN:					\
	    flush = 1;                                                  \
	    break;							\
	case EXA_ENGINEMODE_3D:						\
	    flush = !info->cs || CS_FULL(info->cs);			\
	    break;							\
	case EXA_ENGINEMODE_2D:						\
	    flush = info->cs && CS_FULL(info->cs);			\
	    break;							\
	}								\
	if (flush) {							\
    	    if (info->cs)						\
	        radeon_cs_flush_indirect(pScrn);			\
            else if (info->directRenderingEnabled)                     	\
	        RADEONCPFlushIndirect(pScrn, 1);                        \
	}								\
        info->accel_state->engineMode = EXA_ENGINEMODE_2D;              \
} while (0);

#define RADEON_SWITCH_TO_3D()						\
do {									\
	uint32_t flush = 0;						\
	switch (info->accel_state->engineMode) {			\
	case EXA_ENGINEMODE_UNKNOWN:					\
	    flush = 1;                                                  \
	    break;							\
	case EXA_ENGINEMODE_2D:						\
	    flush = !info->cs || CS_FULL(info->cs);			\
	    break;							\
	case EXA_ENGINEMODE_3D:						\
	    flush = info->cs && CS_FULL(info->cs);			\
	    break;							\
	}								\
	if (flush) {							\
    	    if (info->cs)						\
	        radeon_cs_flush_indirect(pScrn);			\
	    else if (info->directRenderingEnabled)				\
	        RADEONCPFlushIndirect(pScrn, 1);                        \
	}                                                               \
	if (!info->accel_state->XInited3D)				\
	    RADEONInit3DEngine(pScrn);                                  \
        info->accel_state->engineMode = EXA_ENGINEMODE_3D;              \
} while (0);
#else
#define RADEON_SWITCH_TO_2D()
#define RADEON_SWITCH_TO_3D()
#endif

static __inline__ void RADEON_MARK_SYNC(RADEONInfoPtr info, ScrnInfoPtr pScrn)
{
#ifdef USE_EXA
    if (info->useEXA)
	exaMarkSync(pScrn->pScreen);
#endif
#ifdef USE_XAA
    if (!info->useEXA)
	SET_SYNC_FLAG(info->accel_state->accel);
#endif
}

static __inline__ void RADEON_SYNC(RADEONInfoPtr info, ScrnInfoPtr pScrn)
{
#ifdef USE_EXA
    if (info->useEXA && pScrn->pScreen)
	exaWaitSync(pScrn->pScreen);
#endif
#ifdef USE_XAA
    if (!info->useEXA && info->accel_state->accel)
	info->accel_state->accel->Sync(pScrn);
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

enum {
    RADEON_CREATE_PIXMAP_TILING_MACRO = 0x10000000,
    RADEON_CREATE_PIXMAP_TILING_MICRO = 0x20000000,
    RADEON_CREATE_PIXMAP_DEPTH = 0x40000000, /* for r200 */
    RADEON_CREATE_PIXMAP_SZBUFFER = 0x80000000, /* for eg */
};

#endif /* _RADEON_H_ */
