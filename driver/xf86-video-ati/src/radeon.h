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

				/* PCI support */
#include "xf86Pci.h"

#include "exa.h"

				/* Exa and Cursor Support */
#include "xf86Cursor.h"

				/* DDC support */
#include "xf86DDC.h"

				/* Xv support */
#include "xf86xv.h"

#include "radeon_probe.h"

				/* DRI support */
#include "xf86drm.h"
#include "radeon_drm.h"

#ifdef DAMAGE
#include "damage.h"
#include "globals.h"
#endif

#include "xf86Crtc.h"
#include "X11/Xatom.h"

#include "radeon_bo.h"
#include "radeon_cs.h"
#include "radeon_dri2.h"
#include "drmmode_display.h"
#include "radeon_surface.h"

				/* Render support */
#ifdef RENDER
#include "picturestr.h"
#endif

#include "compat-api.h"

#include "simple_list.h"
#include "atipcirename.h"

struct _SyncFence;

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

typedef enum {
    OPTION_ACCEL,
    OPTION_SW_CURSOR,
    OPTION_PAGE_FLIP,
    OPTION_EXA_PIXMAPS,
    OPTION_COLOR_TILING,
    OPTION_COLOR_TILING_2D,
#ifdef RENDER
    OPTION_RENDER_ACCEL,
    OPTION_SUBPIXEL_ORDER,
#endif
    OPTION_ACCELMETHOD,
    OPTION_EXA_VSYNC,
    OPTION_ZAPHOD_HEADS,
    OPTION_SWAPBUFFERS_WAIT,
    OPTION_DELETE_DP12,
    OPTION_DRI3,
    OPTION_DRI,
    OPTION_SHADOW_PRIMARY,
    OPTION_TEAR_FREE,
} RADEONOpts;


#define RADEON_VSYNC_TIMEOUT	20000 /* Maximum wait for VSYNC (in usecs) */

/* Buffer are aligned on 4096 byte boundaries */
#define RADEON_GPU_PAGE_SIZE 4096
#define RADEON_BUFFER_ALIGN (RADEON_GPU_PAGE_SIZE - 1)


#define xFixedToFloat(f) (((float) (f)) / 65536)

#define RADEON_LOGLEVEL_DEBUG 4

/* for Xv, outputs */
#define MAKE_ATOM(a) MakeAtom(a, sizeof(a) - 1, TRUE)

/* Other macros */
#define RADEON_ARRAY_SIZE(x)  (sizeof(x)/sizeof(x[0]))
#define RADEON_ALIGN(x,bytes) (((x) + ((bytes) - 1)) & ~((bytes) - 1))
#define RADEONPTR(pScrn)      ((RADEONInfoPtr)(pScrn)->driverPrivate)

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

/* RS6xx, RS740 are technically R4xx as well, but the
 * clipping hardware seems to follow the r3xx restrictions
 */
#define IS_R400_3D ((info->ChipFamily == CHIP_FAMILY_R420)  ||  \
	(info->ChipFamily == CHIP_FAMILY_RV410))

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

#define CURSOR_WIDTH	64
#define CURSOR_HEIGHT	64

#define CURSOR_WIDTH_CIK	128
#define CURSOR_HEIGHT_CIK	128


#ifdef USE_GLAMOR

struct radeon_pixmap {
	struct radeon_surface surface;

	uint_fast32_t gpu_read;
	uint_fast32_t gpu_write;

	struct radeon_bo *bo;

	uint32_t tiling_flags;

	/* GEM handle for glamor-only pixmaps shared via DRI3 */
	Bool handle_valid;
	uint32_t handle;
};

#if HAS_DEVPRIVATEKEYREC
extern DevPrivateKeyRec glamor_pixmap_index;
#else
extern int glamor_pixmap_index;
#endif

static inline struct radeon_pixmap *radeon_get_pixmap_private(PixmapPtr pixmap)
{
#if HAS_DEVPRIVATEKEYREC
	return dixGetPrivate(&pixmap->devPrivates, &glamor_pixmap_index);
#else
	return dixLookupPrivate(&pixmap->devPrivates, &glamor_pixmap_index);
#endif
}

static inline void radeon_set_pixmap_private(PixmapPtr pixmap, struct radeon_pixmap *priv)
{
	dixSetPrivate(&pixmap->devPrivates, &glamor_pixmap_index, priv);
}

#endif /* USE_GLAMOR */


struct radeon_exa_pixmap_priv {
    struct radeon_bo *bo;
    uint32_t tiling_flags;
    struct radeon_surface surface;
    Bool bo_mapped;
    Bool shared;
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
    uint32_t dst_domain;
    struct radeon_bo *dst_bo;
    struct radeon_bo *src_bo;
};
    
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
    int bpp;
    uint32_t domain;
    struct radeon_bo *bo;
    uint32_t tiling_flags;
    struct radeon_surface *surface;
};

struct radeon_vbo_object {
    int               vb_offset;
    int               vb_total;
    uint32_t          vb_size;
    uint32_t          vb_op_vert_size;
    int32_t           vb_start_op;
    struct radeon_bo *vb_bo;
    unsigned          verts_per_op;
};

struct radeon_accel_state {

				/* Saved values for ScreenToScreenCopy */
    int               xdir;
    int               ydir;

    /* render accel */
    unsigned short    texW[2];
    unsigned short    texH[2];
    Bool              XInited3D; /* X itself has the 3D context */
    int               num_gb_pipes;
    Bool              has_tcl;
    Bool              allowHWDFS;

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

    struct radeon_vbo_object vbo;
    struct radeon_vbo_object cbuf;

    /* where to discard IB from if we cancel operation */
    uint32_t          ib_reset_op;
    struct radeon_dma_bo bo_free;
    struct radeon_dma_bo bo_wait;
    struct radeon_dma_bo bo_reserved;
    Bool use_vbos;
    void (*finish_op)(ScrnInfoPtr, int);
    // shader storage
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

    // solid/copy
    void *copy_area;
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

    Bool              force;
};

typedef struct {
    EntityInfoPtr     pEnt;
    pciVideoPtr       PciInfo;
    int               Chipset;
    RADEONChipFamily  ChipFamily;

    Bool              (*CloseScreen)(CLOSE_SCREEN_ARGS_DECL);

    void              (*BlockHandler)(BLOCKHANDLER_ARGS_DECL);

    void              (*CreateFence) (ScreenPtr pScreen, struct _SyncFence *pFence,
				      Bool initially_triggered);

    int               pix24bpp;         /* Depth of pixmap for 24bpp fb      */
    Bool              dac6bits;         /* Use 6 bit DAC?                    */

    int               pixel_bytes;

    Bool              directRenderingEnabled;
    struct radeon_dri2  dri2;

    /* accel */
    Bool              RenderAccel; /* Render */
    Bool              allowColorTiling;
    Bool              allowColorTiling2D;
    uint_fast32_t     gpu_flushed;
    uint_fast32_t     gpu_synced;
    struct radeon_accel_state *accel_state;
    PixmapPtr         fbcon_pixmap;
    Bool              accelOn;
    Bool              use_glamor;
    Bool              shadow_primary;
    Bool              tear_free;
    Bool	      exa_pixmaps;
    Bool              exa_force_create;
    XF86ModReqInfo    exaReq;
    Bool              is_fast_fb; /* use direct mapping for fast fb access */

    unsigned int xv_max_width;
    unsigned int xv_max_height;

    /* general */
    OptionInfoPtr     Options;

    DisplayModePtr currentMode;

    CreateScreenResourcesProcPtr CreateScreenResources;
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 10
    CreateWindowProcPtr CreateWindow;
#endif

    Bool              IsSecondary;

    Bool              r600_shadow_fb;
    void *fb_shadow;

    void (*reemit_current2d)(ScrnInfoPtr pScrn, int op); // emit the current 2D state into the IB 
    struct radeon_2d_state state_2d;
    struct radeon_bo *front_bo;
    struct radeon_bo_manager *bufmgr;
    struct radeon_cs_manager *csm;
    struct radeon_cs *cs;

    struct radeon_bo *cursor_bo[32];
    uint64_t vram_size;
    uint64_t gart_size;
    drmmode_rec drmmode;
    Bool drmmode_inited;
    /* r6xx+ tile config */
    Bool have_tiling_info;
    uint32_t tile_config;
    int group_bytes;
    int num_channels;
    int num_banks;
    int r7xx_bank_op;
    struct radeon_surface_manager *surf_man;
    struct radeon_surface front_surface;

    /* Xv bicubic filtering */
    struct radeon_bo *bicubic_bo;

    /* kms pageflipping */
    Bool allowPageFlip;

    /* Perform vsync'ed SwapBuffers? */
    Bool swapBuffersWait;

    /* cursor size */
    int cursor_w;
    int cursor_h;

#ifdef USE_GLAMOR
    struct {
	CreateGCProcPtr SavedCreateGC;
	RegionPtr (*SavedCopyArea)(DrawablePtr, DrawablePtr, GCPtr, int, int,
				   int, int, int, int);
	void (*SavedPolyFillRect)(DrawablePtr, GCPtr, int, xRectangle*);
	CloseScreenProcPtr SavedCloseScreen;
	GetImageProcPtr SavedGetImage;
	GetSpansProcPtr SavedGetSpans;
	CreatePixmapProcPtr SavedCreatePixmap;
	DestroyPixmapProcPtr SavedDestroyPixmap;
	CopyWindowProcPtr SavedCopyWindow;
	ChangeWindowAttributesProcPtr SavedChangeWindowAttributes;
	BitmapToRegionProcPtr SavedBitmapToRegion;
#ifdef RENDER
	CompositeProcPtr SavedComposite;
	TrianglesProcPtr SavedTriangles;
	GlyphsProcPtr SavedGlyphs;
	TrapezoidsProcPtr SavedTrapezoids;
	AddTrapsProcPtr SavedAddTraps;
	UnrealizeGlyphProcPtr SavedUnrealizeGlyph;
#endif
    } glamor;
#endif /* USE_GLAMOR */
} RADEONInfoRec, *RADEONInfoPtr;

/* radeon_accel.c */
extern Bool RADEONAccelInit(ScreenPtr pScreen);
extern void RADEONEngineInit(ScrnInfoPtr pScrn);
extern void  RADEONCopySwap(uint8_t *dst, uint8_t *src, unsigned int size, int swap);
extern void RADEONInit3DEngine(ScrnInfoPtr pScrn);
extern int radeon_cs_space_remaining(ScrnInfoPtr pScrn);

/* radeon_commonfuncs.c */
extern void RADEONWaitForVLine(ScrnInfoPtr pScrn, PixmapPtr pPix,
			       xf86CrtcPtr crtc, int start, int stop);


/* radeon_exa.c */
extern unsigned eg_tile_split(unsigned tile_split);
extern Bool radeon_transform_is_affine_or_scaled(PictTransformPtr t);

/* radeon_exa_funcs.c */
extern Bool RADEONDrawInit(ScreenPtr pScreen);
extern Bool R600DrawInit(ScreenPtr pScreen);
extern Bool R600LoadShaders(ScrnInfoPtr pScrn);
extern Bool EVERGREENDrawInit(ScreenPtr pScreen);

/* radeon_exa.c */
extern Bool RADEONGetDatatypeBpp(int bpp, uint32_t *type);
extern Bool RADEONGetPixmapOffsetPitch(PixmapPtr pPix,
				       uint32_t *pitch_offset);

/* radeon_dri3.c */
Bool radeon_dri3_screen_init(ScreenPtr screen);

/* radeon_kms.c */
void radeon_scanout_update_handler(ScrnInfoPtr scrn, uint32_t frame,
				   uint64_t usec, void *event_data);

/* radeon_present.c */
Bool radeon_present_screen_init(ScreenPtr screen);

/* radeon_sync.c */
extern Bool radeon_sync_init(ScreenPtr screen);
extern void radeon_sync_close(ScreenPtr screen);

/* radeon_video.c */
extern void RADEONInitVideo(ScreenPtr pScreen);
extern void RADEONResetVideo(ScrnInfoPtr pScrn);
extern Bool radeon_load_bicubic_texture(ScrnInfoPtr pScrn);
extern xf86CrtcPtr radeon_pick_best_crtc(ScrnInfoPtr pScrn,
					 Bool consider_disabled,
					 int x1, int x2, int y1, int y2);

extern void radeon_cs_flush_indirect(ScrnInfoPtr pScrn);
extern void radeon_ddx_cs_start(ScrnInfoPtr pScrn,
				int num, const char *file,
				const char *func, int line);
void radeon_kms_update_vram_limit(ScrnInfoPtr pScrn, uint32_t new_fb_size);
extern RADEONEntPtr RADEONEntPriv(ScrnInfoPtr pScrn);

drmVBlankSeqType radeon_populate_vbl_request_type(xf86CrtcPtr crtc);

#if XF86_CRTC_VERSION >= 5
#define RADEON_PIXMAP_SHARING 1
#endif

static inline struct radeon_surface *radeon_get_pixmap_surface(PixmapPtr pPix)
{
#ifdef USE_GLAMOR
    RADEONInfoPtr info = RADEONPTR(xf86ScreenToScrn(pPix->drawable.pScreen));

    if (info->use_glamor) {
	struct radeon_pixmap *priv;
	priv = radeon_get_pixmap_private(pPix);
	return priv ? &priv->surface : NULL;
    } else
#endif
    {
	struct radeon_exa_pixmap_priv *driver_priv;
	driver_priv = exaGetPixmapDriverPrivate(pPix);
	return &driver_priv->surface;
    }

    return NULL;
}

uint32_t radeon_get_pixmap_tiling(PixmapPtr pPix);

static inline void radeon_set_pixmap_bo(PixmapPtr pPix, struct radeon_bo *bo)
{
#ifdef USE_GLAMOR
    RADEONInfoPtr info = RADEONPTR(xf86ScreenToScrn(pPix->drawable.pScreen));

    if (info->use_glamor) {
	struct radeon_pixmap *priv;

	priv = radeon_get_pixmap_private(pPix);
	if (priv == NULL && bo == NULL)
	    return;

	if (priv) {
	    if (priv->bo == bo)
		return;

	    if (priv->bo)
		radeon_bo_unref(priv->bo);

	    if (!bo) {
		free(priv);
		priv = NULL;
	    }
	}

	if (bo) {
	    uint32_t pitch;

	    if (!priv) {
		priv = calloc(1, sizeof (struct radeon_pixmap));
		if (!priv)
		    goto out;
	    }

	    radeon_bo_ref(bo);
	    priv->bo = bo;

	    radeon_bo_get_tiling(bo, &priv->tiling_flags, &pitch);
	}
out:
	radeon_set_pixmap_private(pPix, priv);
    } else
#endif /* USE_GLAMOR */
    {
	struct radeon_exa_pixmap_priv *driver_priv;

	driver_priv = exaGetPixmapDriverPrivate(pPix);
	if (driver_priv) {
	    uint32_t pitch;

	    if (driver_priv->bo)
		radeon_bo_unref(driver_priv->bo);

	    radeon_bo_ref(bo);
	    driver_priv->bo = bo;

	    radeon_bo_get_tiling(bo, &driver_priv->tiling_flags, &pitch);
	}
    }
}

static inline struct radeon_bo *radeon_get_pixmap_bo(PixmapPtr pPix)
{
#ifdef USE_GLAMOR
    RADEONInfoPtr info = RADEONPTR(xf86ScreenToScrn(pPix->drawable.pScreen));

    if (info->use_glamor) {
	struct radeon_pixmap *priv;
	priv = radeon_get_pixmap_private(pPix);
	return priv ? priv->bo : NULL;
    } else
#endif
    {
	struct radeon_exa_pixmap_priv *driver_priv;
	driver_priv = exaGetPixmapDriverPrivate(pPix);
	return driver_priv ? driver_priv->bo : NULL;
    }

    return NULL;
}

static inline Bool radeon_get_pixmap_shared(PixmapPtr pPix)
{
#ifdef USE_GLAMOR
    RADEONInfoPtr info = RADEONPTR(xf86ScreenToScrn(pPix->drawable.pScreen));

    if (info->use_glamor) {
        ErrorF("glamor sharing todo\n");
	return FALSE;
    } else
#endif
    {
	struct radeon_exa_pixmap_priv *driver_priv;
	driver_priv = exaGetPixmapDriverPrivate(pPix);
	return driver_priv->shared;
    }
    return FALSE;
}

#define CP_PACKET0(reg, n)						\
	(RADEON_CP_PACKET0 | ((n) << 16) | ((reg) >> 2))
#define CP_PACKET1(reg0, reg1)						\
	(RADEON_CP_PACKET1 | (((reg1) >> 2) << 11) | ((reg0) >> 2))
#define CP_PACKET2()							\
	(RADEON_CP_PACKET2)
#define CP_PACKET3(pkt, n)						\
	(RADEON_CP_PACKET3 | (pkt) | ((n) << 16))


#define RADEON_VERBOSE	0

#define BEGIN_RING(n) do {						\
    if (RADEON_VERBOSE) {						\
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,				\
		   "BEGIN_RING(%d) in %s\n", (unsigned int)n, __FUNCTION__);\
    }									\
    radeon_ddx_cs_start(pScrn, n, __FILE__, __func__, __LINE__);   \
} while (0)

#define ADVANCE_RING() do {						\
    radeon_cs_end(info->cs, __FILE__, __func__, __LINE__); \
  } while (0)

#define OUT_RING(x) do {						\
    if (RADEON_VERBOSE) {						\
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,				\
		   "   OUT_RING(0x%08x)\n", (unsigned int)(x));		\
    }									\
    radeon_cs_write_dword(info->cs, (x));		\
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
    radeon_cs_flush_indirect(pScrn); 				\
} while (0)

#define CS_FULL(cs) ((cs)->cdw > 15 * 1024)

#define RADEON_SWITCH_TO_2D()						\
do {									\
	uint32_t flush = 0;                                             \
	switch (info->accel_state->engineMode) {			\
	case EXA_ENGINEMODE_UNKNOWN:					\
	    flush = 1;                                                  \
	    break;							\
	case EXA_ENGINEMODE_3D:						\
	    flush = CS_FULL(info->cs);			\
	    break;							\
	case EXA_ENGINEMODE_2D:						\
	    flush = CS_FULL(info->cs);			\
	    break;							\
	}								\
	if (flush) {							\
	    radeon_cs_flush_indirect(pScrn);			\
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
	    flush = CS_FULL(info->cs);	 		\
	    break;							\
	case EXA_ENGINEMODE_3D:						\
	    flush = CS_FULL(info->cs);			\
	    break;							\
	}								\
	if (flush) {							\
	    radeon_cs_flush_indirect(pScrn);			\
	}                                                               \
	if (!info->accel_state->XInited3D)				\
	    RADEONInit3DEngine(pScrn);                                  \
        info->accel_state->engineMode = EXA_ENGINEMODE_3D;              \
} while (0);

				/* Memory mapped register access macros */

#define BEGIN_ACCEL_RELOC(n, r) do {		\
	int _nqw = (n) + (r);	\
	BEGIN_RING(2*_nqw);			\
    } while (0)

#define EMIT_OFFSET(reg, value, pPix, rd, wd) do {		\
    driver_priv = exaGetPixmapDriverPrivate(pPix);		\
    OUT_RING_REG((reg), (value));				\
    OUT_RING_RELOC(driver_priv->bo, (rd), (wd));			\
    } while(0)

#define EMIT_READ_OFFSET(reg, value, pPix) EMIT_OFFSET(reg, value, pPix, (RADEON_GEM_DOMAIN_VRAM | RADEON_GEM_DOMAIN_GTT), 0)
#define EMIT_WRITE_OFFSET(reg, value, pPix) EMIT_OFFSET(reg, value, pPix, 0, RADEON_GEM_DOMAIN_VRAM)

#define OUT_TEXTURE_REG(reg, offset, bo) do {   \
    OUT_RING_REG((reg), (offset));                                   \
    OUT_RING_RELOC((bo), RADEON_GEM_DOMAIN_VRAM | RADEON_GEM_DOMAIN_GTT, 0); \
  } while(0)

#define EMIT_COLORPITCH(reg, value, pPix) do {			\
    driver_priv = exaGetPixmapDriverPrivate(pPix);			\
    OUT_RING_REG((reg), value);					\
    OUT_RING_RELOC(driver_priv->bo, 0, RADEON_GEM_DOMAIN_VRAM);		\
} while(0)

static __inline__ void RADEON_SYNC(RADEONInfoPtr info, ScrnInfoPtr pScrn)
{
    if (pScrn->pScreen)
	exaWaitSync(pScrn->pScreen);
}

enum {
    RADEON_CREATE_PIXMAP_SCANOUT		= 0x02000000,
    RADEON_CREATE_PIXMAP_DRI2			= 0x04000000,
    RADEON_CREATE_PIXMAP_TILING_MICRO_SQUARE	= 0x08000000,
    RADEON_CREATE_PIXMAP_TILING_MACRO		= 0x10000000,
    RADEON_CREATE_PIXMAP_TILING_MICRO		= 0x20000000,
    RADEON_CREATE_PIXMAP_DEPTH			= 0x40000000, /* for r200 */
    RADEON_CREATE_PIXMAP_SZBUFFER		= 0x80000000, /* for eg */
};

#define RADEON_CREATE_PIXMAP_TILING_FLAGS	\
    (RADEON_CREATE_PIXMAP_TILING_MICRO_SQUARE |	\
     RADEON_CREATE_PIXMAP_TILING_MACRO |	\
     RADEON_CREATE_PIXMAP_TILING_MICRO |	\
     RADEON_CREATE_PIXMAP_DEPTH |		\
     RADEON_CREATE_PIXMAP_SZBUFFER)


/* Compute log base 2 of val. */
static __inline__ int
RADEONLog2(int val)
{
	int bits;
#if (defined __i386__ || defined __x86_64__) && (defined __GNUC__)
	__asm volatile("bsrl	%1, %0"
		: "=r" (bits)
		: "c" (val)
	);
	return bits;
#else
	for (bits = 0; val != 0; val >>= 1, ++bits)
		;
	return bits - 1;
#endif
}

#define RADEON_TILING_MASK				0xff
#define RADEON_TILING_LINEAR				0x0

#endif /* _RADEON_H_ */
