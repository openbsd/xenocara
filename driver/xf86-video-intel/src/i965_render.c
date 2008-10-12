/*
 * Copyright © 2006,2008 Intel Corporation
 * Copyright © 2007 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Wang Zhenyu <zhenyu.z.wang@intel.com>
 *    Eric Anholt <eric@anholt.net>
 *    Carl Worth <cworth@redhat.com>
 *    Keith Packard <keithp@keithp.com>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <assert.h>
#include "xf86.h"
#include "i830.h"
#include "i915_reg.h"

/* bring in brw structs */
#include "brw_defines.h"
#include "brw_structs.h"

#ifdef I830DEBUG
#define DEBUG_I830FALLBACK 1
#endif

#ifdef DEBUG_I830FALLBACK
#define I830FALLBACK(s, arg...)				\
do {							\
	DPRINTF(PFX, "EXA fallback: " s "\n", ##arg); 	\
	return FALSE;					\
} while(0)
#else
#define I830FALLBACK(s, arg...) 			\
do { 							\
	return FALSE;					\
} while(0)
#endif

#define MAX_VERTEX_PER_COMPOSITE    24
#define MAX_VERTEX_BUFFERS	    256

struct blendinfo {
    Bool dst_alpha;
    Bool src_alpha;
    uint32_t src_blend;
    uint32_t dst_blend;
};

struct formatinfo {
    int fmt;
    uint32_t card_fmt;
};

// refer vol2, 3d rasterization 3.8.1

/* defined in brw_defines.h */
static struct blendinfo i965_blend_op[] = {
    /* Clear */
    {0, 0, BRW_BLENDFACTOR_ZERO,          BRW_BLENDFACTOR_ZERO},
    /* Src */
    {0, 0, BRW_BLENDFACTOR_ONE,           BRW_BLENDFACTOR_ZERO},
    /* Dst */
    {0, 0, BRW_BLENDFACTOR_ZERO,          BRW_BLENDFACTOR_ONE},
    /* Over */
    {0, 1, BRW_BLENDFACTOR_ONE,           BRW_BLENDFACTOR_INV_SRC_ALPHA},
    /* OverReverse */
    {1, 0, BRW_BLENDFACTOR_INV_DST_ALPHA, BRW_BLENDFACTOR_ONE},
    /* In */
    {1, 0, BRW_BLENDFACTOR_DST_ALPHA,     BRW_BLENDFACTOR_ZERO},
    /* InReverse */
    {0, 1, BRW_BLENDFACTOR_ZERO,          BRW_BLENDFACTOR_SRC_ALPHA},
    /* Out */
    {1, 0, BRW_BLENDFACTOR_INV_DST_ALPHA, BRW_BLENDFACTOR_ZERO},
    /* OutReverse */
    {0, 1, BRW_BLENDFACTOR_ZERO,          BRW_BLENDFACTOR_INV_SRC_ALPHA},
    /* Atop */
    {1, 1, BRW_BLENDFACTOR_DST_ALPHA,     BRW_BLENDFACTOR_INV_SRC_ALPHA},
    /* AtopReverse */
    {1, 1, BRW_BLENDFACTOR_INV_DST_ALPHA, BRW_BLENDFACTOR_SRC_ALPHA},
    /* Xor */
    {1, 1, BRW_BLENDFACTOR_INV_DST_ALPHA, BRW_BLENDFACTOR_INV_SRC_ALPHA},
    /* Add */
    {0, 0, BRW_BLENDFACTOR_ONE,           BRW_BLENDFACTOR_ONE},
};
/**
 * Highest-valued BLENDFACTOR used in i965_blend_op.
 *
 * This leaves out BRW_BLENDFACTOR_INV_DST_COLOR,
 * BRW_BLENDFACTOR_INV_CONST_{COLOR,ALPHA},
 * BRW_BLENDFACTOR_INV_SRC1_{COLOR,ALPHA}
 */
#define BRW_BLENDFACTOR_COUNT (BRW_BLENDFACTOR_INV_DST_ALPHA + 1)

/* FIXME: surface format defined in brw_defines.h, shared Sampling engine
 * 1.7.2
 */
static struct formatinfo i965_tex_formats[] = {
    {PICT_a8r8g8b8, BRW_SURFACEFORMAT_B8G8R8A8_UNORM },
    {PICT_x8r8g8b8, BRW_SURFACEFORMAT_B8G8R8X8_UNORM },
    {PICT_a8b8g8r8, BRW_SURFACEFORMAT_R8G8B8A8_UNORM },
    {PICT_x8b8g8r8, BRW_SURFACEFORMAT_R8G8B8X8_UNORM },
    {PICT_r5g6b5,   BRW_SURFACEFORMAT_B5G6R5_UNORM   },
    {PICT_a1r5g5b5, BRW_SURFACEFORMAT_B5G5R5A1_UNORM },
    {PICT_a8,       BRW_SURFACEFORMAT_A8_UNORM	 },
};

static void i965_get_blend_cntl(int op, PicturePtr pMask, uint32_t dst_format,
				uint32_t *sblend, uint32_t *dblend)
{

    *sblend = i965_blend_op[op].src_blend;
    *dblend = i965_blend_op[op].dst_blend;

    /* If there's no dst alpha channel, adjust the blend op so that we'll treat
     * it as always 1.
     */
    if (PICT_FORMAT_A(dst_format) == 0 && i965_blend_op[op].dst_alpha) {
        if (*sblend == BRW_BLENDFACTOR_DST_ALPHA)
            *sblend = BRW_BLENDFACTOR_ONE;
        else if (*sblend == BRW_BLENDFACTOR_INV_DST_ALPHA)
            *sblend = BRW_BLENDFACTOR_ZERO;
    }

    /* If the source alpha is being used, then we should only be in a case where
     * the source blend factor is 0, and the source blend value is the mask
     * channels multiplied by the source picture's alpha.
     */
    if (pMask && pMask->componentAlpha && PICT_FORMAT_RGB(pMask->format)
            && i965_blend_op[op].src_alpha) {
        if (*dblend == BRW_BLENDFACTOR_SRC_ALPHA) {
	    *dblend = BRW_BLENDFACTOR_SRC_COLOR;
        } else if (*dblend == BRW_BLENDFACTOR_INV_SRC_ALPHA) {
	    *dblend = BRW_BLENDFACTOR_INV_SRC_COLOR;
        }
    }

}

static Bool i965_get_dest_format(PicturePtr pDstPicture, uint32_t *dst_format)
{
    switch (pDstPicture->format) {
    case PICT_a8r8g8b8:
    case PICT_x8r8g8b8:
        *dst_format = BRW_SURFACEFORMAT_B8G8R8A8_UNORM;
        break;
    case PICT_r5g6b5:
        *dst_format = BRW_SURFACEFORMAT_B5G6R5_UNORM;
        break;
    case PICT_a1r5g5b5:
    	*dst_format = BRW_SURFACEFORMAT_B5G5R5A1_UNORM;
	break;
    case PICT_x1r5g5b5:
        *dst_format = BRW_SURFACEFORMAT_B5G5R5X1_UNORM;
        break;
    case PICT_a8:
        *dst_format = BRW_SURFACEFORMAT_A8_UNORM;
        break;
    case PICT_a4r4g4b4:
    case PICT_x4r4g4b4:
	*dst_format = BRW_SURFACEFORMAT_B4G4R4A4_UNORM;
	break;
    default:
        I830FALLBACK("Unsupported dest format 0x%x\n",
		     (int)pDstPicture->format);
    }

    return TRUE;
}

static Bool i965_check_composite_texture(PicturePtr pPict, int unit)
{
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    int i;

    if ((w > 8192) || (h > 8192))
        I830FALLBACK("Picture w/h too large (%dx%d)\n", w, h);

    for (i = 0; i < sizeof(i965_tex_formats) / sizeof(i965_tex_formats[0]);
	 i++)
    {
        if (i965_tex_formats[i].fmt == pPict->format)
            break;
    }
    if (i == sizeof(i965_tex_formats) / sizeof(i965_tex_formats[0]))
        I830FALLBACK("Unsupported picture format 0x%x\n",
		     (int)pPict->format);

    if (pPict->repeat && pPict->repeatType != RepeatNormal)
	I830FALLBACK("extended repeat (%d) not supported\n",
		     pPict->repeatType);

    if (pPict->filter != PictFilterNearest &&
        pPict->filter != PictFilterBilinear)
    {
        I830FALLBACK("Unsupported filter 0x%x\n", pPict->filter);
    }

    return TRUE;
}

Bool
i965_check_composite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
		     PicturePtr pDstPicture)
{
    uint32_t tmp1;

    /* Check for unsupported compositing operations. */
    if (op >= sizeof(i965_blend_op) / sizeof(i965_blend_op[0]))
        I830FALLBACK("Unsupported Composite op 0x%x\n", op);

    if (pMaskPicture && pMaskPicture->componentAlpha &&
            PICT_FORMAT_RGB(pMaskPicture->format)) {
        /* Check if it's component alpha that relies on a source alpha and on
         * the source value.  We can only get one of those into the single
         * source value that we get to blend with.
         */
        if (i965_blend_op[op].src_alpha &&
            (i965_blend_op[op].src_blend != BRW_BLENDFACTOR_ZERO))
	{
	    I830FALLBACK("Component alpha not supported with source "
			 "alpha and source value blending.\n");
	}
    } 

    if (!i965_check_composite_texture(pSrcPicture, 0))
        I830FALLBACK("Check Src picture texture\n");
    if (pMaskPicture != NULL && !i965_check_composite_texture(pMaskPicture, 1))
        I830FALLBACK("Check Mask picture texture\n");

    if (!i965_get_dest_format(pDstPicture, &tmp1))
	I830FALLBACK("Get Color buffer format\n");

    return TRUE;

}

#define ALIGN(i,m)    (((i) + (m) - 1) & ~((m) - 1))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define BRW_GRF_BLOCKS(nreg)    ((nreg + 15) / 16 - 1)

/* Set up a default static partitioning of the URB, which is supposed to
 * allow anything we would want to do, at potentially lower performance.
 */
#define URB_CS_ENTRY_SIZE     0
#define URB_CS_ENTRIES	      0

#define URB_VS_ENTRY_SIZE     1	  // each 512-bit row
#define URB_VS_ENTRIES	      8	  // we needs at least 8 entries

#define URB_GS_ENTRY_SIZE     0
#define URB_GS_ENTRIES	      0

#define URB_CLIP_ENTRY_SIZE   0
#define URB_CLIP_ENTRIES      0

#define URB_SF_ENTRY_SIZE     2
#define URB_SF_ENTRIES	      1

static const uint32_t sip_kernel_static[][4] = {
/*    wait (1) a0<1>UW a145<0,1,0>UW { align1 +  } */
    { 0x00000030, 0x20000108, 0x00001220, 0x00000000 },
/*    nop (4) g0<1>UD { align1 +  } */
    { 0x0040007e, 0x20000c21, 0x00690000, 0x00000000 },
/*    nop (4) g0<1>UD { align1 +  } */
    { 0x0040007e, 0x20000c21, 0x00690000, 0x00000000 },
/*    nop (4) g0<1>UD { align1 +  } */
    { 0x0040007e, 0x20000c21, 0x00690000, 0x00000000 },
/*    nop (4) g0<1>UD { align1 +  } */
    { 0x0040007e, 0x20000c21, 0x00690000, 0x00000000 },
/*    nop (4) g0<1>UD { align1 +  } */
    { 0x0040007e, 0x20000c21, 0x00690000, 0x00000000 },
/*    nop (4) g0<1>UD { align1 +  } */
    { 0x0040007e, 0x20000c21, 0x00690000, 0x00000000 },
/*    nop (4) g0<1>UD { align1 +  } */
    { 0x0040007e, 0x20000c21, 0x00690000, 0x00000000 },
/*    nop (4) g0<1>UD { align1 +  } */
    { 0x0040007e, 0x20000c21, 0x00690000, 0x00000000 },
/*    nop (4) g0<1>UD { align1 +  } */
    { 0x0040007e, 0x20000c21, 0x00690000, 0x00000000 },
};

/*
 * this program computes dA/dx and dA/dy for the texture coordinates along
 * with the base texture coordinate. It was extracted from the Mesa driver
 */

#define SF_KERNEL_NUM_GRF  16
#define SF_MAX_THREADS	   2

static const uint32_t sf_kernel_static[][4] = {
#include "exa_sf.g4b"
};

static const uint32_t sf_kernel_mask_static[][4] = {
#include "exa_sf_mask.g4b"
};

/* ps kernels */
#define PS_KERNEL_NUM_GRF   32
#define PS_MAX_THREADS	    48
#define PS_SCRATCH_SPACE    1024
#define PS_SCRATCH_SPACE_LOG	0   /* log2 (PS_SCRATCH_SPACE) - 10  (1024 is 0, 2048 is 1) */

static const uint32_t ps_kernel_nomask_affine_static [][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_affine.g4b"
#include "exa_wm_src_sample_argb.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_nomask_projective_static [][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_projective.g4b"
#include "exa_wm_src_sample_argb.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_maskca_affine_static [][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_affine.g4b"
#include "exa_wm_src_sample_argb.g4b"
#include "exa_wm_mask_affine.g4b"
#include "exa_wm_mask_sample_argb.g4b"
#include "exa_wm_ca.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_maskca_projective_static [][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_projective.g4b"
#include "exa_wm_src_sample_argb.g4b"
#include "exa_wm_mask_projective.g4b"
#include "exa_wm_mask_sample_argb.g4b"
#include "exa_wm_ca.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_maskca_srcalpha_affine_static [][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_affine.g4b"
#include "exa_wm_src_sample_a.g4b"
#include "exa_wm_mask_affine.g4b"
#include "exa_wm_mask_sample_argb.g4b"
#include "exa_wm_ca_srcalpha.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_maskca_srcalpha_projective_static [][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_projective.g4b"
#include "exa_wm_src_sample_a.g4b"
#include "exa_wm_mask_projective.g4b"
#include "exa_wm_mask_sample_argb.g4b"
#include "exa_wm_ca_srcalpha.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_masknoca_affine_static [][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_affine.g4b"
#include "exa_wm_src_sample_argb.g4b"
#include "exa_wm_mask_affine.g4b"
#include "exa_wm_mask_sample_a.g4b"
#include "exa_wm_noca.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_masknoca_projective_static [][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_projective.g4b"
#include "exa_wm_src_sample_argb.g4b"
#include "exa_wm_mask_projective.g4b"
#include "exa_wm_mask_sample_a.g4b"
#include "exa_wm_noca.g4b"
#include "exa_wm_write.g4b"
};

/**
 * Storage for the static kernel data with template name, rounded to 64 bytes.
 */
#define KERNEL_DECL(template) \
    uint32_t template [((sizeof (template ## _static) + 63) & ~63) / 16][4];

#define WM_STATE_DECL(kernel) \
    struct brw_wm_unit_state wm_state_ ## kernel[SAMPLER_STATE_FILTER_COUNT] \
						[SAMPLER_STATE_EXTEND_COUNT] \
						[SAMPLER_STATE_FILTER_COUNT] \
						[SAMPLER_STATE_EXTEND_COUNT]

/* Many of the fields in the state structure must be aligned to a
 * 64-byte boundary, (or a 32-byte boundary, but 64 is good enough for
 * those too).
 */
#define PAD64_MULTI(previous, idx, factor) char previous ## _pad ## idx [(64 - (sizeof(struct previous) * (factor)) % 64) % 64]
#define PAD64(previous, idx) PAD64_MULTI(previous, idx, 1)

typedef enum {
    SAMPLER_STATE_FILTER_NEAREST,
    SAMPLER_STATE_FILTER_BILINEAR,
    SAMPLER_STATE_FILTER_COUNT
} sampler_state_filter_t;

typedef enum {
    SAMPLER_STATE_EXTEND_NONE,
    SAMPLER_STATE_EXTEND_REPEAT,
    SAMPLER_STATE_EXTEND_COUNT
} sampler_state_extend_t;

typedef struct _brw_cc_unit_state_padded {
    struct brw_cc_unit_state state;
    char pad[64 - sizeof (struct brw_cc_unit_state)];
} brw_cc_unit_state_padded;

typedef struct brw_surface_state_padded {
    struct brw_surface_state state;
    char pad[32 - sizeof (struct brw_surface_state)];
} brw_surface_state_padded;

/**
 * Gen4 rendering state buffer structure.
 *
 * Ideally this structure would contain static data for all of the
 * combinations of state that we use for Render acceleration, and another
 * buffer would be the use-and-throw-away surface and vertex data.  See the
 * intel-batchbuffer branch for an implementation of that.  For now, it
 * has the static program data, and then a changing buffer containing all
 * the rest.
 */
typedef struct _gen4_state {
    uint8_t wm_scratch[128 * PS_MAX_THREADS];

    KERNEL_DECL (sip_kernel);
    KERNEL_DECL (sf_kernel);
    KERNEL_DECL (sf_kernel_mask);
    KERNEL_DECL (ps_kernel_nomask_affine);
    KERNEL_DECL (ps_kernel_nomask_projective);
    KERNEL_DECL (ps_kernel_maskca_affine);
    KERNEL_DECL (ps_kernel_maskca_projective);
    KERNEL_DECL (ps_kernel_maskca_srcalpha_affine);
    KERNEL_DECL (ps_kernel_maskca_srcalpha_projective);
    KERNEL_DECL (ps_kernel_masknoca_affine);
    KERNEL_DECL (ps_kernel_masknoca_projective);

    struct brw_vs_unit_state vs_state;
    PAD64 (brw_vs_unit_state, 0);

    struct brw_sf_unit_state sf_state;
    PAD64 (brw_sf_unit_state, 0);
    struct brw_sf_unit_state sf_state_mask;
    PAD64 (brw_sf_unit_state, 1);

    WM_STATE_DECL (nomask_affine);
    WM_STATE_DECL (nomask_projective);
    WM_STATE_DECL (maskca_affine);
    WM_STATE_DECL (maskca_projective);
    WM_STATE_DECL (maskca_srcalpha_affine);
    WM_STATE_DECL (maskca_srcalpha_projective);
    WM_STATE_DECL (masknoca_affine);
    WM_STATE_DECL (masknoca_projective);

    uint32_t binding_table[128];

    struct brw_surface_state_padded surface_state[32];

    /* Index by [src_filter][src_extend][mask_filter][mask_extend].  Two of
     * the structs happen to add to 32 bytes.
     */
    struct brw_sampler_state sampler_state[SAMPLER_STATE_FILTER_COUNT]
					  [SAMPLER_STATE_EXTEND_COUNT]
					  [SAMPLER_STATE_FILTER_COUNT]
					  [SAMPLER_STATE_EXTEND_COUNT][2];

    struct brw_sampler_default_color sampler_default_color;
    PAD64 (brw_sampler_default_color, 0);

    /* Index by [src_blend][dst_blend] */
    brw_cc_unit_state_padded cc_state[BRW_BLENDFACTOR_COUNT]
				     [BRW_BLENDFACTOR_COUNT];
    struct brw_cc_viewport cc_viewport;
    PAD64 (brw_cc_viewport, 0);

    float vb[MAX_VERTEX_PER_COMPOSITE * MAX_VERTEX_BUFFERS];
} gen4_state_t;

/** Private data for gen4 render accel implementation. */
struct gen4_render_state {
    gen4_state_t *card_state;
    uint32_t card_state_offset;

    int binding_table_index;
    int surface_state_index;
    int vb_offset;
    int vertex_size;
};

/**
 * Sets up the SF state pointing at an SF kernel.
 *
 * The SF kernel does coord interp: for each attribute,
 * calculate dA/dx and dA/dy.  Hand these interpolation coefficients
 * back to SF which then hands pixels off to WM.
 */
static void
sf_state_init (struct brw_sf_unit_state *sf_state, int kernel_offset)
{
    memset(sf_state, 0, sizeof(*sf_state));
    sf_state->thread0.grf_reg_count = BRW_GRF_BLOCKS(SF_KERNEL_NUM_GRF);
    sf_state->sf1.single_program_flow = 1;
    sf_state->sf1.binding_table_entry_count = 0;
    sf_state->sf1.thread_priority = 0;
    sf_state->sf1.floating_point_mode = 0; /* Mesa does this */
    sf_state->sf1.illegal_op_exception_enable = 1;
    sf_state->sf1.mask_stack_exception_enable = 1;
    sf_state->sf1.sw_exception_enable = 1;
    sf_state->thread2.per_thread_scratch_space = 0;
    /* scratch space is not used in our kernel */
    sf_state->thread2.scratch_space_base_pointer = 0;
    sf_state->thread3.const_urb_entry_read_length = 0; /* no const URBs */
    sf_state->thread3.const_urb_entry_read_offset = 0; /* no const URBs */
    sf_state->thread3.urb_entry_read_length = 1; /* 1 URB per vertex */
    /* don't smash vertex header, read start from dw8 */
    sf_state->thread3.urb_entry_read_offset = 1;
    sf_state->thread3.dispatch_grf_start_reg = 3;
    sf_state->thread4.max_threads = SF_MAX_THREADS - 1;
    sf_state->thread4.urb_entry_allocation_size = URB_SF_ENTRY_SIZE - 1;
    sf_state->thread4.nr_urb_entries = URB_SF_ENTRIES;
    sf_state->thread4.stats_enable = 1;
    sf_state->sf5.viewport_transform = FALSE; /* skip viewport */
    sf_state->sf6.cull_mode = BRW_CULLMODE_NONE;
    sf_state->sf6.scissor = 0;
    sf_state->sf7.trifan_pv = 2;
    sf_state->sf6.dest_org_vbias = 0x8;
    sf_state->sf6.dest_org_hbias = 0x8;

    assert((kernel_offset & 63) == 0);
    sf_state->thread0.kernel_start_pointer = kernel_offset >> 6;
}

static void
sampler_state_init (struct brw_sampler_state *sampler_state,
		    sampler_state_filter_t filter,
		    sampler_state_extend_t extend,
		    int default_color_offset)
{
    /* PS kernel use this sampler */
    memset(sampler_state, 0, sizeof(*sampler_state));

    sampler_state->ss0.lod_preclamp = 1; /* GL mode */
    sampler_state->ss0.default_color_mode = 0; /* GL mode */

    switch(filter) {
    default:
    case SAMPLER_STATE_FILTER_NEAREST:
	sampler_state->ss0.min_filter = BRW_MAPFILTER_NEAREST;
	sampler_state->ss0.mag_filter = BRW_MAPFILTER_NEAREST;
	break;
    case SAMPLER_STATE_FILTER_BILINEAR:
	sampler_state->ss0.min_filter = BRW_MAPFILTER_LINEAR;
	sampler_state->ss0.mag_filter = BRW_MAPFILTER_LINEAR;
	break;
    }

    switch (extend) {
    default:
    case SAMPLER_STATE_EXTEND_NONE:
	sampler_state->ss1.r_wrap_mode = BRW_TEXCOORDMODE_CLAMP_BORDER;
	sampler_state->ss1.s_wrap_mode = BRW_TEXCOORDMODE_CLAMP_BORDER;
	sampler_state->ss1.t_wrap_mode = BRW_TEXCOORDMODE_CLAMP_BORDER;
	break;
    case SAMPLER_STATE_EXTEND_REPEAT:
	sampler_state->ss1.r_wrap_mode = BRW_TEXCOORDMODE_WRAP;
	sampler_state->ss1.s_wrap_mode = BRW_TEXCOORDMODE_WRAP;
	sampler_state->ss1.t_wrap_mode = BRW_TEXCOORDMODE_WRAP;
	break;
    }

    assert((default_color_offset & 31) == 0);
    sampler_state->ss2.default_color_pointer = default_color_offset >> 5;

    sampler_state->ss3.chroma_key_enable = 0; /* disable chromakey */
}

static void
cc_state_init (struct brw_cc_unit_state *cc_state,
	       int src_blend,
	       int dst_blend,
	       int cc_viewport_offset)
{
    memset(cc_state, 0, sizeof(*cc_state));
    cc_state->cc0.stencil_enable = 0;   /* disable stencil */
    cc_state->cc2.depth_test = 0;       /* disable depth test */
    cc_state->cc2.logicop_enable = 0;   /* disable logic op */
    cc_state->cc3.ia_blend_enable = 0;  /* blend alpha same as colors */
    cc_state->cc3.blend_enable = 1;     /* enable color blend */
    cc_state->cc3.alpha_test = 0;       /* disable alpha test */

    assert((cc_viewport_offset & 31) == 0);
    cc_state->cc4.cc_viewport_state_offset = cc_viewport_offset >> 5;

    cc_state->cc5.dither_enable = 0;    /* disable dither */
    cc_state->cc5.logicop_func = 0xc;   /* COPY */
    cc_state->cc5.statistics_enable = 1;
    cc_state->cc5.ia_blend_function = BRW_BLENDFUNCTION_ADD;

    /* Fill in alpha blend factors same as color, for the future. */
    cc_state->cc5.ia_src_blend_factor = src_blend;
    cc_state->cc5.ia_dest_blend_factor = dst_blend;

    cc_state->cc6.blend_function = BRW_BLENDFUNCTION_ADD;
    cc_state->cc6.clamp_post_alpha_blend = 1;
    cc_state->cc6.clamp_pre_alpha_blend = 1;
    cc_state->cc6.clamp_range = 0;  /* clamp range [0,1] */

    cc_state->cc6.src_blend_factor = src_blend;
    cc_state->cc6.dest_blend_factor = dst_blend;
}

static void
wm_state_init (struct brw_wm_unit_state *wm_state,
	       Bool has_mask,
	       int scratch_offset,
	       int kernel_offset,
	       int sampler_state_offset)
{
    memset(wm_state, 0, sizeof (*wm_state));
    wm_state->thread0.grf_reg_count = BRW_GRF_BLOCKS(PS_KERNEL_NUM_GRF);
    wm_state->thread1.single_program_flow = 0;

    assert((scratch_offset & 1023) == 0);
    wm_state->thread2.scratch_space_base_pointer = scratch_offset >> 10;

    wm_state->thread2.per_thread_scratch_space = PS_SCRATCH_SPACE_LOG;
    wm_state->thread3.const_urb_entry_read_length = 0;
    wm_state->thread3.const_urb_entry_read_offset = 0;

    wm_state->thread3.urb_entry_read_offset = 0;
    /* wm kernel use urb from 3, see wm_program in compiler module */
    wm_state->thread3.dispatch_grf_start_reg = 3; /* must match kernel */

    wm_state->wm4.stats_enable = 1;  /* statistic */
    assert((sampler_state_offset & 31) == 0);
    wm_state->wm4.sampler_state_pointer = sampler_state_offset >> 5;
    wm_state->wm4.sampler_count = 1; /* 1-4 samplers used */
    wm_state->wm5.max_threads = PS_MAX_THREADS - 1;
    wm_state->wm5.transposed_urb_read = 0;
    wm_state->wm5.thread_dispatch_enable = 1;
    /* just use 16-pixel dispatch (4 subspans), don't need to change kernel
     * start point
     */
    wm_state->wm5.enable_16_pix = 1;
    wm_state->wm5.enable_8_pix = 0;
    wm_state->wm5.early_depth_test = 1;

    assert((kernel_offset & 63) == 0);
    wm_state->thread0.kernel_start_pointer = kernel_offset >> 6;

    /* Each pair of attributes (src/mask coords) is two URB entries */
    if (has_mask) {
	wm_state->thread1.binding_table_entry_count = 3; /* 2 tex and fb */
	wm_state->thread3.urb_entry_read_length = 4;
    } else {
	wm_state->thread1.binding_table_entry_count = 2; /* 1 tex and fb */
	wm_state->thread3.urb_entry_read_length = 2;
    }
}

/**
 * Called at EnterVT to fill in our state buffer with any static information.
 */
static void
gen4_state_init (struct gen4_render_state *render_state)
{
    int i, j, k, l;
    gen4_state_t *card_state = render_state->card_state;
    uint32_t state_base_offset = render_state->card_state_offset;

#define KERNEL_COPY(kernel) \
    memcpy(card_state->kernel, kernel ## _static, sizeof(kernel ## _static))

    KERNEL_COPY (sip_kernel);
    KERNEL_COPY (sf_kernel);
    KERNEL_COPY (sf_kernel_mask);
    KERNEL_COPY (ps_kernel_nomask_affine);
    KERNEL_COPY (ps_kernel_nomask_projective);
    KERNEL_COPY (ps_kernel_maskca_affine);
    KERNEL_COPY (ps_kernel_maskca_projective);
    KERNEL_COPY (ps_kernel_maskca_srcalpha_affine);
    KERNEL_COPY (ps_kernel_maskca_srcalpha_projective);
    KERNEL_COPY (ps_kernel_masknoca_affine);
    KERNEL_COPY (ps_kernel_masknoca_projective);
#undef KERNEL_COPY

    /* Set up the vertex shader to be disabled (passthrough) */
    memset(&card_state->vs_state, 0, sizeof(card_state->vs_state));
    card_state->vs_state.thread4.nr_urb_entries = URB_VS_ENTRIES;
    card_state->vs_state.thread4.urb_entry_allocation_size =
	URB_VS_ENTRY_SIZE - 1;
    card_state->vs_state.vs6.vs_enable = 0;
    card_state->vs_state.vs6.vert_cache_disable = 1;

    /* Set up the sampler default color (always transparent black) */
    memset(&card_state->sampler_default_color, 0,
	   sizeof(card_state->sampler_default_color));
    card_state->sampler_default_color.color[0] = 0.0; /* R */
    card_state->sampler_default_color.color[1] = 0.0; /* G */
    card_state->sampler_default_color.color[2] = 0.0; /* B */
    card_state->sampler_default_color.color[3] = 0.0; /* A */

    card_state->cc_viewport.min_depth = -1.e35;
    card_state->cc_viewport.max_depth = 1.e35;

    sf_state_init (&card_state->sf_state,
		   state_base_offset +
		   offsetof (gen4_state_t, sf_kernel));
    sf_state_init (&card_state->sf_state_mask,
		   state_base_offset +
		   offsetof (gen4_state_t, sf_kernel_mask));

    for (i = 0; i < SAMPLER_STATE_FILTER_COUNT; i++) {
	for (j = 0; j < SAMPLER_STATE_EXTEND_COUNT; j++) {
	    for (k = 0; k < SAMPLER_STATE_FILTER_COUNT; k++) {
		for (l = 0; l < SAMPLER_STATE_EXTEND_COUNT; l++) {
		    sampler_state_init (&card_state->sampler_state[i][j][k][l][0],
					i, j,
					state_base_offset +
					offsetof (gen4_state_t,
						  sampler_default_color));
		    sampler_state_init (&card_state->sampler_state[i][j][k][l][1],
					k, l,
					state_base_offset +
					offsetof (gen4_state_t,
						  sampler_default_color));
		}
	    }
	}
    }


    for (i = 0; i < BRW_BLENDFACTOR_COUNT; i++) {
	for (j = 0; j < BRW_BLENDFACTOR_COUNT; j++) {
	    cc_state_init (&card_state->cc_state[i][j].state, i, j,
			   state_base_offset +
			   offsetof (gen4_state_t, cc_viewport));
	}
    }

#define SETUP_WM_STATE(kernel, has_mask)				\
    wm_state_init(&card_state->wm_state_ ## kernel [i][j][k][l],	\
		  has_mask,						\
		  state_base_offset + offsetof(gen4_state_t,		\
					       wm_scratch),		\
		  state_base_offset + offsetof(gen4_state_t,		\
					       ps_kernel_ ## kernel),	\
		  state_base_offset + offsetof(gen4_state_t,		\
					       sampler_state[i][j][k][l]));


    for (i = 0; i < SAMPLER_STATE_FILTER_COUNT; i++) {
	for (j = 0; j < SAMPLER_STATE_EXTEND_COUNT; j++) {
	    for (k = 0; k < SAMPLER_STATE_FILTER_COUNT; k++) {
		for (l = 0; l < SAMPLER_STATE_EXTEND_COUNT; l++) {
		    SETUP_WM_STATE (nomask_affine, FALSE);
		    SETUP_WM_STATE (nomask_projective, FALSE);
		    SETUP_WM_STATE (maskca_affine, TRUE);
		    SETUP_WM_STATE (maskca_projective, TRUE);
		    SETUP_WM_STATE (maskca_srcalpha_affine, TRUE);
		    SETUP_WM_STATE (maskca_srcalpha_projective, TRUE);
		    SETUP_WM_STATE (masknoca_affine, TRUE);
		    SETUP_WM_STATE (masknoca_projective, TRUE);
		}
	    }
	}
    }
#undef SETUP_WM_STATE
}

static uint32_t 
i965_get_card_format(PicturePtr pPict)
{
    int i;

    for (i = 0; i < sizeof(i965_tex_formats) / sizeof(i965_tex_formats[0]);
	 i++)
    {
	if (i965_tex_formats[i].fmt == pPict->format)
	    break;
    }
    assert(i != sizeof(i965_tex_formats) / sizeof(i965_tex_formats[0]));

    return i965_tex_formats[i].card_fmt;
}

static sampler_state_filter_t
sampler_state_filter_from_picture (int filter)
{
    switch (filter) {
    case PictFilterNearest:
	return SAMPLER_STATE_FILTER_NEAREST;
    case PictFilterBilinear:
	return SAMPLER_STATE_FILTER_BILINEAR;
    default:
	return -1;
    }
}

static sampler_state_extend_t
sampler_state_extend_from_picture (int repeat)
{
    switch (repeat) {
    case RepeatNone:
	return SAMPLER_STATE_EXTEND_NONE;
    case RepeatNormal:
	return SAMPLER_STATE_EXTEND_REPEAT;
    default:
	return -1;
    }
}

/**
 * Sets up the common fields for a surface state buffer for the given picture
 * in the surface state buffer at index, and returns the offset within the
 * state buffer for this entry.
 */
static unsigned int
i965_set_picture_surface_state(ScrnInfoPtr pScrn, struct brw_surface_state *ss,
			       PicturePtr pPicture, PixmapPtr pPixmap,
			       Bool is_dst)
{
    I830Ptr pI830 = I830PTR(pScrn);
    struct gen4_render_state *render_state= pI830->gen4_render_state;
    gen4_state_t *card_state = render_state->card_state;
    struct brw_surface_state local_ss;
    uint32_t offset;

    /* Since ss is a pointer to WC memory, do all of our bit operations
     * into a local temporary first.
     */
    memset(&local_ss, 0, sizeof(local_ss));
    local_ss.ss0.surface_type = BRW_SURFACE_2D;
    if (is_dst) {
	uint32_t dst_format = 0;
	Bool ret = TRUE;

	ret = i965_get_dest_format(pPicture, &dst_format);
	assert(ret == TRUE);
	local_ss.ss0.surface_format = dst_format;
    } else {
	local_ss.ss0.surface_format = i965_get_card_format(pPicture);
    }

    local_ss.ss0.data_return_format = BRW_SURFACERETURNFORMAT_FLOAT32;
    local_ss.ss0.writedisable_alpha = 0;
    local_ss.ss0.writedisable_red = 0;
    local_ss.ss0.writedisable_green = 0;
    local_ss.ss0.writedisable_blue = 0;
    local_ss.ss0.color_blend = 1;
    local_ss.ss0.vert_line_stride = 0;
    local_ss.ss0.vert_line_stride_ofs = 0;
    local_ss.ss0.mipmap_layout_mode = 0;
    local_ss.ss0.render_cache_read_mode = 0;
    local_ss.ss1.base_addr = intel_get_pixmap_offset(pPixmap);

    local_ss.ss2.mip_count = 0;
    local_ss.ss2.render_target_rotation = 0;
    local_ss.ss2.height = pPixmap->drawable.height - 1;
    local_ss.ss2.width = pPixmap->drawable.width - 1;
    local_ss.ss3.pitch = intel_get_pixmap_pitch(pPixmap) - 1;
    local_ss.ss3.tile_walk = 0; /* Tiled X */
    local_ss.ss3.tiled_surface = i830_pixmap_tiled(pPixmap) ? 1 : 0;

    memcpy(ss, &local_ss, sizeof(local_ss));

    offset = (char *)ss - (char *)card_state;
    assert((offset & 31) == 0);

    return offset;
}

Bool
i965_prepare_composite(int op, PicturePtr pSrcPicture,
		       PicturePtr pMaskPicture, PicturePtr pDstPicture,
		       PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst)
{
    ScrnInfoPtr pScrn = xf86Screens[pSrcPicture->pDrawable->pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);
    struct gen4_render_state *render_state= pI830->gen4_render_state;
    gen4_state_t *card_state = render_state->card_state;
    struct brw_surface_state_padded *ss;
    uint32_t sf_state_offset;
    sampler_state_filter_t src_filter, mask_filter;
    sampler_state_extend_t src_extend, mask_extend;
    Bool is_affine_src, is_affine_mask, is_affine;
    int urb_vs_start, urb_vs_size;
    int urb_gs_start, urb_gs_size;
    int urb_clip_start, urb_clip_size;
    int urb_sf_start, urb_sf_size;
    int urb_cs_start, urb_cs_size;
    char *state_base;
    int state_base_offset;
    uint32_t src_blend, dst_blend;
    uint32_t *binding_table;

    IntelEmitInvarientState(pScrn);
    *pI830->last_3d = LAST_3D_RENDER;

    pI830->scale_units[0][0] = pSrc->drawable.width;
    pI830->scale_units[0][1] = pSrc->drawable.height;

    pI830->transform[0] = pSrcPicture->transform;
    is_affine_src = i830_transform_is_affine (pI830->transform[0]);

    if (!pMask) {
	pI830->transform[1] = NULL;
	pI830->scale_units[1][0] = -1;
	pI830->scale_units[1][1] = -1;
	is_affine_mask = TRUE;
    } else {
	pI830->transform[1] = pMaskPicture->transform;
	pI830->scale_units[1][0] = pMask->drawable.width;
	pI830->scale_units[1][1] = pMask->drawable.height;
	is_affine_mask = i830_transform_is_affine (pI830->transform[1]);
    }

    is_affine = is_affine_src && is_affine_mask;

    state_base_offset = pI830->gen4_render_state_mem->offset;
    assert((state_base_offset & 63) == 0);
    state_base = (char *)(pI830->FbBase + state_base_offset);

    urb_vs_start = 0;
    urb_vs_size = URB_VS_ENTRIES * URB_VS_ENTRY_SIZE;
    urb_gs_start = urb_vs_start + urb_vs_size;
    urb_gs_size = URB_GS_ENTRIES * URB_GS_ENTRY_SIZE;
    urb_clip_start = urb_gs_start + urb_gs_size;
    urb_clip_size = URB_CLIP_ENTRIES * URB_CLIP_ENTRY_SIZE;
    urb_sf_start = urb_clip_start + urb_clip_size;
    urb_sf_size = URB_SF_ENTRIES * URB_SF_ENTRY_SIZE;
    urb_cs_start = urb_sf_start + urb_sf_size;
    urb_cs_size = URB_CS_ENTRIES * URB_CS_ENTRY_SIZE;

    i965_get_blend_cntl(op, pMaskPicture, pDstPicture->format,
			&src_blend, &dst_blend);

    if ((render_state->binding_table_index + 3 >=
	 ARRAY_SIZE(card_state->binding_table)) ||
	(render_state->surface_state_index + 3 >=
	 ARRAY_SIZE(card_state->surface_state)))
    {
	i830WaitSync(pScrn);
	render_state->binding_table_index = 0;
	render_state->surface_state_index = 0;
	render_state->vb_offset = 0;
    }

    binding_table = card_state->binding_table +
	render_state->binding_table_index;
    ss = card_state->surface_state + render_state->surface_state_index;
    /* We only use 2 or 3 entries, but the table has to be 32-byte
     * aligned.
     */
    render_state->binding_table_index += 8;
    render_state->surface_state_index += (pMask != NULL) ? 3 : 2;

    /* Set up and bind the state buffer for the destination surface */
    binding_table[0] = state_base_offset +
	i965_set_picture_surface_state(pScrn,
				       &ss[0].state,
				       pDstPicture, pDst, TRUE);

    /* Set up and bind the source surface state buffer */
    binding_table[1] = state_base_offset +
	i965_set_picture_surface_state(pScrn,
				       &ss[1].state,
				       pSrcPicture, pSrc, FALSE);
    if (pMask) {
	/* Set up and bind the mask surface state buffer */
	binding_table[2] = state_base_offset +
	    i965_set_picture_surface_state(pScrn,
					   &ss[2].state,
					   pMaskPicture, pMask,
					   FALSE);
    } else {
	binding_table[2] = 0;
    }

    src_filter = sampler_state_filter_from_picture (pSrcPicture->filter);
    if (src_filter < 0)
	I830FALLBACK ("Bad src filter 0x%x\n", pSrcPicture->filter);
    src_extend = sampler_state_extend_from_picture (pSrcPicture->repeat);
    if (src_extend < 0)
	I830FALLBACK ("Bad src repeat 0x%x\n", pSrcPicture->repeat);

    if (pMaskPicture) {
	mask_filter = sampler_state_filter_from_picture (pMaskPicture->filter);
	if (mask_filter < 0)
	    I830FALLBACK ("Bad mask filter 0x%x\n", pMaskPicture->filter);
	mask_extend = sampler_state_extend_from_picture (pMaskPicture->repeat);
	if (mask_extend < 0)
	    I830FALLBACK ("Bad mask repeat 0x%x\n", pMaskPicture->repeat);
    } else {
	mask_filter = SAMPLER_STATE_FILTER_NEAREST;
	mask_extend = SAMPLER_STATE_EXTEND_NONE;
    }

    /* Begin the long sequence of commands needed to set up the 3D
     * rendering pipe
     */
    {
	BEGIN_BATCH(2);
	OUT_BATCH(MI_FLUSH |
		  MI_STATE_INSTRUCTION_CACHE_FLUSH |
		  BRW_MI_GLOBAL_SNAPSHOT_RESET);
	OUT_BATCH(MI_NOOP);
	ADVANCE_BATCH();
    }
    {
        BEGIN_BATCH(12);

        /* Match Mesa driver setup */
	if (IS_GM45(pI830) || IS_G4X(pI830))
	    OUT_BATCH(NEW_PIPELINE_SELECT | PIPELINE_SELECT_3D);
	else
	    OUT_BATCH(BRW_PIPELINE_SELECT | PIPELINE_SELECT_3D);

	OUT_BATCH(BRW_CS_URB_STATE | 0);
	OUT_BATCH((0 << 4) |  /* URB Entry Allocation Size */
		  (0 << 0));  /* Number of URB Entries */

	/* Zero out the two base address registers so all offsets are
	 * absolute.
	 */
	OUT_BATCH(BRW_STATE_BASE_ADDRESS | 4);
	OUT_BATCH(0 | BASE_ADDRESS_MODIFY);  /* Generate state base address */
	OUT_BATCH(0 | BASE_ADDRESS_MODIFY);  /* Surface state base address */
	OUT_BATCH(0 | BASE_ADDRESS_MODIFY);  /* media base addr, don't care */
	/* general state max addr, disabled */
	OUT_BATCH(0x10000000 | BASE_ADDRESS_MODIFY);
	/* media object state max addr, disabled */
	OUT_BATCH(0x10000000 | BASE_ADDRESS_MODIFY);

	/* Set system instruction pointer */
	OUT_BATCH(BRW_STATE_SIP | 0);
	OUT_BATCH(state_base_offset + offsetof(gen4_state_t, sip_kernel));
	OUT_BATCH(MI_NOOP);
	ADVANCE_BATCH();
    }
    {
	BEGIN_BATCH(26);
	/* Pipe control */
	OUT_BATCH(BRW_PIPE_CONTROL |
		  BRW_PIPE_CONTROL_NOWRITE |
		  BRW_PIPE_CONTROL_IS_FLUSH |
		  2);
	OUT_BATCH(0);			       /* Destination address */
	OUT_BATCH(0);			       /* Immediate data low DW */
	OUT_BATCH(0);			       /* Immediate data high DW */

	/* Binding table pointers */
	OUT_BATCH(BRW_3DSTATE_BINDING_TABLE_POINTERS | 4);
	OUT_BATCH(0); /* vs */
	OUT_BATCH(0); /* gs */
	OUT_BATCH(0); /* clip */
	OUT_BATCH(0); /* sf */
	/* Only the PS uses the binding table */
	assert((((unsigned char *)binding_table - pI830->FbBase) & 31) == 0);
	OUT_BATCH((unsigned char *)binding_table - pI830->FbBase);

	/* The drawing rectangle clipping is always on.  Set it to values that
	 * shouldn't do any clipping.
	 */
	OUT_BATCH(BRW_3DSTATE_DRAWING_RECTANGLE | 2); /* XXX 3 for BLC or CTG */
	OUT_BATCH(0x00000000);	/* ymin, xmin */
	OUT_BATCH(DRAW_YMAX(pDst->drawable.height - 1) |
		  DRAW_XMAX(pDst->drawable.width - 1)); /* ymax, xmax */
	OUT_BATCH(0x00000000);	/* yorigin, xorigin */

	/* skip the depth buffer */
	/* skip the polygon stipple */
	/* skip the polygon stipple offset */
	/* skip the line stipple */

	/* Set the pointers to the 3d pipeline state */
	OUT_BATCH(BRW_3DSTATE_PIPELINED_POINTERS | 5);
	assert((offsetof(gen4_state_t, vs_state) & 31) == 0);
	OUT_BATCH(state_base_offset + offsetof(gen4_state_t, vs_state));
	OUT_BATCH(BRW_GS_DISABLE);   /* disable GS, resulting in passthrough */
	OUT_BATCH(BRW_CLIP_DISABLE); /* disable CLIP, resulting in passthrough */

	if (pMask) {
	    sf_state_offset = state_base_offset +
		offsetof(gen4_state_t, sf_state_mask);
	} else {
	    sf_state_offset = state_base_offset +
		offsetof(gen4_state_t, sf_state);
	}
	assert((sf_state_offset & 31) == 0);
	OUT_BATCH(sf_state_offset);

	/* Shorthand for long array lookup */
#define OUT_WM_KERNEL(kernel) do {					\
    uint32_t offset = state_base_offset +				\
	offsetof(gen4_state_t,						\
		 wm_state_ ## kernel					\
		 [src_filter]						\
		 [src_extend]						\
		 [mask_filter]						\
		 [mask_extend]);					\
    assert((offset & 31) == 0);						\
    OUT_BATCH(offset);							\
} while (0)

	if (pMask) {
	    if (pMaskPicture->componentAlpha &&
		PICT_FORMAT_RGB(pMaskPicture->format))
	    {
		if (i965_blend_op[op].src_alpha) {
		    if (is_affine)
			OUT_WM_KERNEL(maskca_srcalpha_affine);
		    else
			OUT_WM_KERNEL(maskca_srcalpha_projective);
		} else {
		    if (is_affine)
			OUT_WM_KERNEL(maskca_affine);
		    else
			OUT_WM_KERNEL(maskca_projective);
		}
	    } else {
		if (is_affine)
		    OUT_WM_KERNEL(masknoca_affine);
		else
		    OUT_WM_KERNEL(masknoca_projective);
	    }
	} else {
	    if (is_affine)
		OUT_WM_KERNEL(nomask_affine);
	    else
		OUT_WM_KERNEL(nomask_projective);
	}
#undef OUT_WM_KERNEL

	/* 64 byte aligned */
	assert((offsetof(gen4_state_t,
			 cc_state[src_blend][dst_blend]) & 63) == 0);
	OUT_BATCH(state_base_offset +
		  offsetof(gen4_state_t, cc_state[src_blend][dst_blend]));

	/* URB fence */
	OUT_BATCH(BRW_URB_FENCE |
		  UF0_CS_REALLOC |
		  UF0_SF_REALLOC |
		  UF0_CLIP_REALLOC |
		  UF0_GS_REALLOC |
		  UF0_VS_REALLOC |
		  1);
	OUT_BATCH(((urb_clip_start + urb_clip_size) << UF1_CLIP_FENCE_SHIFT) |
		  ((urb_gs_start + urb_gs_size) << UF1_GS_FENCE_SHIFT) |
		  ((urb_vs_start + urb_vs_size) << UF1_VS_FENCE_SHIFT));
	OUT_BATCH(((urb_cs_start + urb_cs_size) << UF2_CS_FENCE_SHIFT) |
		  ((urb_sf_start + urb_sf_size) << UF2_SF_FENCE_SHIFT));

	/* Constant buffer state */
	OUT_BATCH(BRW_CS_URB_STATE | 0);
	OUT_BATCH(((URB_CS_ENTRY_SIZE - 1) << 4) |
		  (URB_CS_ENTRIES << 0));
	ADVANCE_BATCH();
    }
    {
	/* 
	 * number of extra parameters per vertex
	 */
        int nelem = pMask ? 2: 1;
	/* 
	 * size of extra parameters:
	 *  3 for homogenous (xyzw)
	 *  2 for cartesian (xy)
	 */
	int selem = is_affine ? 2 : 3;
	uint32_t    w_component;
	uint32_t    src_format;

	render_state->vertex_size = 4 * (2 + nelem * selem);
	
	if (is_affine)
	{
	    src_format = BRW_SURFACEFORMAT_R32G32_FLOAT;
	    w_component = BRW_VFCOMPONENT_STORE_1_FLT;
	}
	else
	{
	    src_format = BRW_SURFACEFORMAT_R32G32B32_FLOAT;
	    w_component = BRW_VFCOMPONENT_STORE_SRC;
	}
	BEGIN_BATCH(pMask?7:5);
	/* Set up our vertex elements, sourced from the single vertex buffer.
	 * that will be set up later.
	 */
	
	OUT_BATCH(BRW_3DSTATE_VERTEX_ELEMENTS | ((2 * (1 + nelem)) - 1));
	/* x,y */
	OUT_BATCH((0 << VE0_VERTEX_BUFFER_INDEX_SHIFT) |
		  VE0_VALID |
		  (BRW_SURFACEFORMAT_R32G32_FLOAT << VE0_FORMAT_SHIFT) |
		  (0				<< VE0_OFFSET_SHIFT));
	OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC	<< VE1_VFCOMPONENT_0_SHIFT) |
		  (BRW_VFCOMPONENT_STORE_SRC	<< VE1_VFCOMPONENT_1_SHIFT) |
		  (BRW_VFCOMPONENT_STORE_1_FLT	<< VE1_VFCOMPONENT_2_SHIFT) |
		  (BRW_VFCOMPONENT_STORE_1_FLT	<< VE1_VFCOMPONENT_3_SHIFT) |
		  (4				<< VE1_DESTINATION_ELEMENT_OFFSET_SHIFT));
	/* u0, v0, w0 */
	OUT_BATCH((0				<< VE0_VERTEX_BUFFER_INDEX_SHIFT) |
		  VE0_VALID					     |
		  (src_format			<< VE0_FORMAT_SHIFT) |
		  ((2 * 4)			<< VE0_OFFSET_SHIFT)); /* offset vb in bytes */
	OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC	<< VE1_VFCOMPONENT_0_SHIFT) |
		  (BRW_VFCOMPONENT_STORE_SRC	<< VE1_VFCOMPONENT_1_SHIFT) |
		  (w_component			<< VE1_VFCOMPONENT_2_SHIFT) |
		  (BRW_VFCOMPONENT_STORE_1_FLT	<< VE1_VFCOMPONENT_3_SHIFT) |
		  ((4 + 4)			<< VE1_DESTINATION_ELEMENT_OFFSET_SHIFT)); /* VUE offset in dwords */
	/* u1, v1, w1 */
   	if (pMask) {
	    OUT_BATCH((0			    << VE0_VERTEX_BUFFER_INDEX_SHIFT) |
		      VE0_VALID							    |
		      (src_format		    << VE0_FORMAT_SHIFT) |
		      (((2 + selem) * 4)    	    << VE0_OFFSET_SHIFT));  /* vb offset in bytes */
	    
	    OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC    << VE1_VFCOMPONENT_0_SHIFT) |
		      (BRW_VFCOMPONENT_STORE_SRC    << VE1_VFCOMPONENT_1_SHIFT) |
		      (w_component		    << VE1_VFCOMPONENT_2_SHIFT) |
		      (BRW_VFCOMPONENT_STORE_1_FLT  << VE1_VFCOMPONENT_3_SHIFT) |
		      ((4 + 4 + 4)		    << VE1_DESTINATION_ELEMENT_OFFSET_SHIFT)); /* VUE offset in dwords */
   	}

	ADVANCE_BATCH();
    }

#ifdef I830DEBUG
    ErrorF("try to sync to show any errors...\n");
    I830Sync(pScrn);
#endif
    return TRUE;
}

void
i965_composite(PixmapPtr pDst, int srcX, int srcY, int maskX, int maskY,
	       int dstX, int dstY, int w, int h)
{
    ScrnInfoPtr pScrn = xf86Screens[pDst->drawable.pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);
    gen4_state_t *card_state = pI830->gen4_render_state->card_state;
    struct gen4_render_state *render_state = pI830->gen4_render_state;
    Bool has_mask;
    Bool is_affine_src, is_affine_mask, is_affine;
    float src_x[3], src_y[3], src_w[3], mask_x[3], mask_y[3], mask_w[3];
    float *vb = card_state->vb;
    int i;

    is_affine_src = i830_transform_is_affine (pI830->transform[0]);
    is_affine_mask = i830_transform_is_affine (pI830->transform[1]);
    is_affine = is_affine_src && is_affine_mask;
    
    if (is_affine)
    {
	if (!i830_get_transformed_coordinates(srcX, srcY,
					      pI830->transform[0],
					      &src_x[0], &src_y[0]))
	    return;
	if (!i830_get_transformed_coordinates(srcX, srcY + h,
					      pI830->transform[0],
					      &src_x[1], &src_y[1]))
	    return;
	if (!i830_get_transformed_coordinates(srcX + w, srcY + h,
					      pI830->transform[0],
					      &src_x[2], &src_y[2]))
	    return;
    }
    else
    {
	if (!i830_get_transformed_coordinates_3d(srcX, srcY,
						 pI830->transform[0],
						 &src_x[0], &src_y[0],
						 &src_w[0]))
	    return;
	if (!i830_get_transformed_coordinates_3d(srcX, srcY + h,
						 pI830->transform[0],
						 &src_x[1], &src_y[1],
						 &src_w[1]))
	    return;
	if (!i830_get_transformed_coordinates_3d(srcX + w, srcY + h,
						 pI830->transform[0],
						 &src_x[2], &src_y[2],
						 &src_w[2]))
	    return;
    }

    if (pI830->scale_units[1][0] == -1 || pI830->scale_units[1][1] == -1) {
	has_mask = FALSE;
    } else {
	has_mask = TRUE;
	if (is_affine) {
	    if (!i830_get_transformed_coordinates(maskX, maskY,
						  pI830->transform[1],
						  &mask_x[0], &mask_y[0]))
		return;
	    if (!i830_get_transformed_coordinates(maskX, maskY + h,
						  pI830->transform[1],
						  &mask_x[1], &mask_y[1]))
		return;
	    if (!i830_get_transformed_coordinates(maskX + w, maskY + h,
						  pI830->transform[1],
						  &mask_x[2], &mask_y[2]))
		return;
	} else {
	    if (!i830_get_transformed_coordinates_3d(maskX, maskY,
						     pI830->transform[1],
						     &mask_x[0], &mask_y[0],
						     &mask_w[0]))
		return;
	    if (!i830_get_transformed_coordinates_3d(maskX, maskY + h,
						     pI830->transform[1],
						     &mask_x[1], &mask_y[1],
						     &mask_w[1]))
		return;
	    if (!i830_get_transformed_coordinates_3d(maskX + w, maskY + h,
						     pI830->transform[1],
						     &mask_x[2], &mask_y[2],
						     &mask_w[2]))
		return;
	}
    }

    if (render_state->vb_offset + MAX_VERTEX_PER_COMPOSITE >= ARRAY_SIZE(card_state->vb)) {
	i830WaitSync(pScrn);
	render_state->vb_offset = 0;
    }

    i = render_state->vb_offset;
    /* rect (x2,y2) */
    vb[i++] = (float)(dstX + w);
    vb[i++] = (float)(dstY + h);
    vb[i++] = src_x[2] / pI830->scale_units[0][0];
    vb[i++] = src_y[2] / pI830->scale_units[0][1];
    if (!is_affine)
	vb[i++] = src_w[2];
    if (has_mask) {
        vb[i++] = mask_x[2] / pI830->scale_units[1][0];
        vb[i++] = mask_y[2] / pI830->scale_units[1][1];
	if (!is_affine)
	    vb[i++] = mask_w[2];
    }

    /* rect (x1,y2) */
    vb[i++] = (float)dstX;
    vb[i++] = (float)(dstY + h);
    vb[i++] = src_x[1] / pI830->scale_units[0][0];
    vb[i++] = src_y[1] / pI830->scale_units[0][1];
    if (!is_affine)
	vb[i++] = src_w[1];
    if (has_mask) {
        vb[i++] = mask_x[1] / pI830->scale_units[1][0];
        vb[i++] = mask_y[1] / pI830->scale_units[1][1];
	if (!is_affine)
	    vb[i++] = mask_w[1];
    }

    /* rect (x1,y1) */
    vb[i++] = (float)dstX;
    vb[i++] = (float)dstY;
    vb[i++] = src_x[0] / pI830->scale_units[0][0];
    vb[i++] = src_y[0] / pI830->scale_units[0][1];
    if (!is_affine)
	vb[i++] = src_w[0];
    if (has_mask) {
        vb[i++] = mask_x[0] / pI830->scale_units[1][0];
        vb[i++] = mask_y[0] / pI830->scale_units[1][1];
	if (!is_affine)
	    vb[i++] = mask_w[0];
    }
    assert (i * 4 <= sizeof(card_state->vb));

    BEGIN_BATCH(12);
    OUT_BATCH(MI_FLUSH);
    /* Set up the pointer to our (single) vertex buffer */
    OUT_BATCH(BRW_3DSTATE_VERTEX_BUFFERS | 3);
    OUT_BATCH((0 << VB0_BUFFER_INDEX_SHIFT) |
	      VB0_VERTEXDATA |
	      (render_state->vertex_size << VB0_BUFFER_PITCH_SHIFT));
    OUT_BATCH(render_state->card_state_offset + offsetof(gen4_state_t, vb) +
	      render_state->vb_offset * 4);
    OUT_BATCH(3);
    OUT_BATCH(0); // ignore for VERTEXDATA, but still there

    OUT_BATCH(BRW_3DPRIMITIVE |
	      BRW_3DPRIMITIVE_VERTEX_SEQUENTIAL |
	      (_3DPRIM_RECTLIST << BRW_3DPRIMITIVE_TOPOLOGY_SHIFT) |
	      (0 << 9) |  /* CTG - indirect vertex count */
	      4);
    OUT_BATCH(3);  /* vertex count per instance */
    OUT_BATCH(0); /* start vertex offset */
    OUT_BATCH(1); /* single instance */
    OUT_BATCH(0); /* start instance location */
    OUT_BATCH(0); /* index buffer offset, ignored */
    ADVANCE_BATCH();

    render_state->vb_offset = i;

#ifdef I830DEBUG
    ErrorF("sync after 3dprimitive\n");
    I830Sync(pScrn);
#endif
    /* we must be sure that the pipeline is flushed before next exa draw,
       because that will be new state, binding state and instructions*/
    {
	BEGIN_BATCH(4);
	OUT_BATCH(BRW_PIPE_CONTROL |
		  BRW_PIPE_CONTROL_NOWRITE |
		  BRW_PIPE_CONTROL_WC_FLUSH |
		  BRW_PIPE_CONTROL_IS_FLUSH |
		  (1 << 10) |  /* XXX texture cache flush for BLC/CTG */
		  2);
	OUT_BATCH(0); /* Destination address */
	OUT_BATCH(0); /* Immediate data low DW */
	OUT_BATCH(0); /* Immediate data high DW */
	ADVANCE_BATCH();
    }

    /* Mark sync so we can wait for it before setting up the VB on the next
     * rectangle.
     */
    i830MarkSync(pScrn);
}

/**
 * Called at EnterVT so we can set up our offsets into the state buffer.
 */
void
gen4_render_state_init(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    struct gen4_render_state *render_state;

    if (pI830->gen4_render_state == NULL)
	pI830->gen4_render_state = calloc(sizeof(*render_state), 1);

    render_state = pI830->gen4_render_state;

    render_state->card_state_offset = pI830->gen4_render_state_mem->offset;
    render_state->card_state = (gen4_state_t *)
	(pI830->FbBase + render_state->card_state_offset);

    gen4_state_init(render_state);
}

/**
 * Called at LeaveVT.
 */
void
gen4_render_state_cleanup(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);

    pI830->gen4_render_state->card_state = NULL;
}

unsigned int
gen4_render_state_size(ScrnInfoPtr pScrn)
{
    return sizeof(gen4_state_t);
}
