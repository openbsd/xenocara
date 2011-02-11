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

/* 24 = 4 vertices/composite * 3 texcoords/vertex * 2 floats/texcoord
 *
 * This is an upper-bound based on the case of a non-affine
 * transformation and with a mask, but useful for sizing all cases for
 * simplicity.
 */
#define VERTEX_FLOATS_PER_COMPOSITE	24
#define VERTEX_BUFFER_SIZE		(256 * VERTEX_FLOATS_PER_COMPOSITE)

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
	{0, 0, BRW_BLENDFACTOR_ZERO, BRW_BLENDFACTOR_ZERO},
	/* Src */
	{0, 0, BRW_BLENDFACTOR_ONE, BRW_BLENDFACTOR_ZERO},
	/* Dst */
	{0, 0, BRW_BLENDFACTOR_ZERO, BRW_BLENDFACTOR_ONE},
	/* Over */
	{0, 1, BRW_BLENDFACTOR_ONE, BRW_BLENDFACTOR_INV_SRC_ALPHA},
	/* OverReverse */
	{1, 0, BRW_BLENDFACTOR_INV_DST_ALPHA, BRW_BLENDFACTOR_ONE},
	/* In */
	{1, 0, BRW_BLENDFACTOR_DST_ALPHA, BRW_BLENDFACTOR_ZERO},
	/* InReverse */
	{0, 1, BRW_BLENDFACTOR_ZERO, BRW_BLENDFACTOR_SRC_ALPHA},
	/* Out */
	{1, 0, BRW_BLENDFACTOR_INV_DST_ALPHA, BRW_BLENDFACTOR_ZERO},
	/* OutReverse */
	{0, 1, BRW_BLENDFACTOR_ZERO, BRW_BLENDFACTOR_INV_SRC_ALPHA},
	/* Atop */
	{1, 1, BRW_BLENDFACTOR_DST_ALPHA, BRW_BLENDFACTOR_INV_SRC_ALPHA},
	/* AtopReverse */
	{1, 1, BRW_BLENDFACTOR_INV_DST_ALPHA, BRW_BLENDFACTOR_SRC_ALPHA},
	/* Xor */
	{1, 1, BRW_BLENDFACTOR_INV_DST_ALPHA, BRW_BLENDFACTOR_INV_SRC_ALPHA},
	/* Add */
	{0, 0, BRW_BLENDFACTOR_ONE, BRW_BLENDFACTOR_ONE},
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
	{PICT_a8r8g8b8, BRW_SURFACEFORMAT_B8G8R8A8_UNORM},
	{PICT_x8r8g8b8, BRW_SURFACEFORMAT_B8G8R8X8_UNORM},
	{PICT_a8b8g8r8, BRW_SURFACEFORMAT_R8G8B8A8_UNORM},
	{PICT_x8b8g8r8, BRW_SURFACEFORMAT_R8G8B8X8_UNORM},
	{PICT_r5g6b5, BRW_SURFACEFORMAT_B5G6R5_UNORM},
	{PICT_a1r5g5b5, BRW_SURFACEFORMAT_B5G5R5A1_UNORM},
	{PICT_a8, BRW_SURFACEFORMAT_A8_UNORM},
};

static void i965_get_blend_cntl(int op, PicturePtr mask, uint32_t dst_format,
				uint32_t * sblend, uint32_t * dblend)
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
	if (mask && mask->componentAlpha && PICT_FORMAT_RGB(mask->format)
	    && i965_blend_op[op].src_alpha) {
		if (*dblend == BRW_BLENDFACTOR_SRC_ALPHA) {
			*dblend = BRW_BLENDFACTOR_SRC_COLOR;
		} else if (*dblend == BRW_BLENDFACTOR_INV_SRC_ALPHA) {
			*dblend = BRW_BLENDFACTOR_INV_SRC_COLOR;
		}
	}

}

static Bool i965_get_dest_format(PicturePtr dest_picture, uint32_t * dst_format)
{
	ScrnInfoPtr scrn = xf86Screens[dest_picture->pDrawable->pScreen->myNum];

	switch (dest_picture->format) {
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
		intel_debug_fallback(scrn, "Unsupported dest format 0x%x\n",
				     (int)dest_picture->format);
		return FALSE;
	}

	return TRUE;
}

Bool
i965_check_composite(int op,
		     PicturePtr source_picture,
		     PicturePtr mask_picture,
		     PicturePtr dest_picture,
		     int width, int height)
{
	ScrnInfoPtr scrn = xf86Screens[dest_picture->pDrawable->pScreen->myNum];
	uint32_t tmp1;

	/* Check for unsupported compositing operations. */
	if (op >= sizeof(i965_blend_op) / sizeof(i965_blend_op[0])) {
		intel_debug_fallback(scrn,
				     "Unsupported Composite op 0x%x\n", op);
		return FALSE;
	}

	if (mask_picture && mask_picture->componentAlpha &&
	    PICT_FORMAT_RGB(mask_picture->format)) {
		/* Check if it's component alpha that relies on a source alpha and on
		 * the source value.  We can only get one of those into the single
		 * source value that we get to blend with.
		 */
		if (i965_blend_op[op].src_alpha &&
		    (i965_blend_op[op].src_blend != BRW_BLENDFACTOR_ZERO)) {
			intel_debug_fallback(scrn,
					     "Component alpha not supported "
					     "with source alpha and source "
					     "value blending.\n");
			return FALSE;
		}
	}

	if (!i965_get_dest_format(dest_picture, &tmp1)) {
		intel_debug_fallback(scrn, "Get Color buffer format\n");
		return FALSE;
	}

	return TRUE;
}

Bool
i965_check_composite_texture(ScreenPtr screen, PicturePtr picture)
{
	if (picture->repeatType > RepeatReflect) {
		ScrnInfoPtr scrn = xf86Screens[screen->myNum];
		intel_debug_fallback(scrn,
				     "extended repeat (%d) not supported\n",
				     picture->repeatType);
		return FALSE;
	}

	if (picture->filter != PictFilterNearest &&
	    picture->filter != PictFilterBilinear) {
		ScrnInfoPtr scrn = xf86Screens[screen->myNum];
		intel_debug_fallback(scrn, "Unsupported filter 0x%x\n",
				     picture->filter);
		return FALSE;
	}

	if (picture->pDrawable) {
		int w, h, i;

		w = picture->pDrawable->width;
		h = picture->pDrawable->height;
		if ((w > 8192) || (h > 8192)) {
			ScrnInfoPtr scrn = xf86Screens[screen->myNum];
			intel_debug_fallback(scrn,
					     "Picture w/h too large (%dx%d)\n",
					     w, h);
			return FALSE;
		}

		for (i = 0;
		     i < sizeof(i965_tex_formats) / sizeof(i965_tex_formats[0]);
		     i++) {
			if (i965_tex_formats[i].fmt == picture->format)
				break;
		}
		if (i == sizeof(i965_tex_formats) / sizeof(i965_tex_formats[0]))
		{
			ScrnInfoPtr scrn = xf86Screens[screen->myNum];
			intel_debug_fallback(scrn,
					     "Unsupported picture format "
					     "0x%x\n",
					     (int)picture->format);
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}


#define BRW_GRF_BLOCKS(nreg)    ((nreg + 15) / 16 - 1)

/* Set up a default static partitioning of the URB, which is supposed to
 * allow anything we would want to do, at potentially lower performance.
 */
#define URB_CS_ENTRY_SIZE     0
#define URB_CS_ENTRIES	      0

#define URB_VS_ENTRY_SIZE     1	// each 512-bit row
#define URB_VS_ENTRIES	      8	// we needs at least 8 entries

#define URB_GS_ENTRY_SIZE     0
#define URB_GS_ENTRIES	      0

#define URB_CLIP_ENTRY_SIZE   0
#define URB_CLIP_ENTRIES      0

#define URB_SF_ENTRY_SIZE     2
#define URB_SF_ENTRIES	      1

static const uint32_t sip_kernel_static[][4] = {
/*    wait (1) a0<1>UW a145<0,1,0>UW { align1 +  } */
	{0x00000030, 0x20000108, 0x00001220, 0x00000000},
/*    nop (4) g0<1>UD { align1 +  } */
	{0x0040007e, 0x20000c21, 0x00690000, 0x00000000},
/*    nop (4) g0<1>UD { align1 +  } */
	{0x0040007e, 0x20000c21, 0x00690000, 0x00000000},
/*    nop (4) g0<1>UD { align1 +  } */
	{0x0040007e, 0x20000c21, 0x00690000, 0x00000000},
/*    nop (4) g0<1>UD { align1 +  } */
	{0x0040007e, 0x20000c21, 0x00690000, 0x00000000},
/*    nop (4) g0<1>UD { align1 +  } */
	{0x0040007e, 0x20000c21, 0x00690000, 0x00000000},
/*    nop (4) g0<1>UD { align1 +  } */
	{0x0040007e, 0x20000c21, 0x00690000, 0x00000000},
/*    nop (4) g0<1>UD { align1 +  } */
	{0x0040007e, 0x20000c21, 0x00690000, 0x00000000},
/*    nop (4) g0<1>UD { align1 +  } */
	{0x0040007e, 0x20000c21, 0x00690000, 0x00000000},
/*    nop (4) g0<1>UD { align1 +  } */
	{0x0040007e, 0x20000c21, 0x00690000, 0x00000000},
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

static const uint32_t ps_kernel_nomask_affine_static[][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_affine.g4b"
#include "exa_wm_src_sample_argb.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_nomask_projective_static[][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_projective.g4b"
#include "exa_wm_src_sample_argb.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_maskca_affine_static[][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_affine.g4b"
#include "exa_wm_src_sample_argb.g4b"
#include "exa_wm_mask_affine.g4b"
#include "exa_wm_mask_sample_argb.g4b"
#include "exa_wm_ca.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_maskca_projective_static[][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_projective.g4b"
#include "exa_wm_src_sample_argb.g4b"
#include "exa_wm_mask_projective.g4b"
#include "exa_wm_mask_sample_argb.g4b"
#include "exa_wm_ca.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_maskca_srcalpha_affine_static[][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_affine.g4b"
#include "exa_wm_src_sample_a.g4b"
#include "exa_wm_mask_affine.g4b"
#include "exa_wm_mask_sample_argb.g4b"
#include "exa_wm_ca_srcalpha.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_maskca_srcalpha_projective_static[][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_projective.g4b"
#include "exa_wm_src_sample_a.g4b"
#include "exa_wm_mask_projective.g4b"
#include "exa_wm_mask_sample_argb.g4b"
#include "exa_wm_ca_srcalpha.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_masknoca_affine_static[][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_affine.g4b"
#include "exa_wm_src_sample_argb.g4b"
#include "exa_wm_mask_affine.g4b"
#include "exa_wm_mask_sample_a.g4b"
#include "exa_wm_noca.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_masknoca_projective_static[][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_projective.g4b"
#include "exa_wm_src_sample_argb.g4b"
#include "exa_wm_mask_projective.g4b"
#include "exa_wm_mask_sample_a.g4b"
#include "exa_wm_noca.g4b"
#include "exa_wm_write.g4b"
};

/* new programs for IGDNG */
static const uint32_t sf_kernel_static_gen5[][4] = {
#include "exa_sf.g4b.gen5"
};

static const uint32_t sf_kernel_mask_static_gen5[][4] = {
#include "exa_sf_mask.g4b.gen5"
};

static const uint32_t ps_kernel_nomask_affine_static_gen5[][4] = {
#include "exa_wm_xy.g4b.gen5"
#include "exa_wm_src_affine.g4b.gen5"
#include "exa_wm_src_sample_argb.g4b.gen5"
#include "exa_wm_write.g4b.gen5"
};

static const uint32_t ps_kernel_nomask_projective_static_gen5[][4] = {
#include "exa_wm_xy.g4b.gen5"
#include "exa_wm_src_projective.g4b.gen5"
#include "exa_wm_src_sample_argb.g4b.gen5"
#include "exa_wm_write.g4b.gen5"
};

static const uint32_t ps_kernel_maskca_affine_static_gen5[][4] = {
#include "exa_wm_xy.g4b.gen5"
#include "exa_wm_src_affine.g4b.gen5"
#include "exa_wm_src_sample_argb.g4b.gen5"
#include "exa_wm_mask_affine.g4b.gen5"
#include "exa_wm_mask_sample_argb.g4b.gen5"
#include "exa_wm_ca.g4b.gen5"
#include "exa_wm_write.g4b.gen5"
};

static const uint32_t ps_kernel_maskca_projective_static_gen5[][4] = {
#include "exa_wm_xy.g4b.gen5"
#include "exa_wm_src_projective.g4b.gen5"
#include "exa_wm_src_sample_argb.g4b.gen5"
#include "exa_wm_mask_projective.g4b.gen5"
#include "exa_wm_mask_sample_argb.g4b.gen5"
#include "exa_wm_ca.g4b.gen5"
#include "exa_wm_write.g4b.gen5"
};

static const uint32_t ps_kernel_maskca_srcalpha_affine_static_gen5[][4] = {
#include "exa_wm_xy.g4b.gen5"
#include "exa_wm_src_affine.g4b.gen5"
#include "exa_wm_src_sample_a.g4b.gen5"
#include "exa_wm_mask_affine.g4b.gen5"
#include "exa_wm_mask_sample_argb.g4b.gen5"
#include "exa_wm_ca_srcalpha.g4b.gen5"
#include "exa_wm_write.g4b.gen5"
};

static const uint32_t ps_kernel_maskca_srcalpha_projective_static_gen5[][4] = {
#include "exa_wm_xy.g4b.gen5"
#include "exa_wm_src_projective.g4b.gen5"
#include "exa_wm_src_sample_a.g4b.gen5"
#include "exa_wm_mask_projective.g4b.gen5"
#include "exa_wm_mask_sample_argb.g4b.gen5"
#include "exa_wm_ca_srcalpha.g4b.gen5"
#include "exa_wm_write.g4b.gen5"
};

static const uint32_t ps_kernel_masknoca_affine_static_gen5[][4] = {
#include "exa_wm_xy.g4b.gen5"
#include "exa_wm_src_affine.g4b.gen5"
#include "exa_wm_src_sample_argb.g4b.gen5"
#include "exa_wm_mask_affine.g4b.gen5"
#include "exa_wm_mask_sample_a.g4b.gen5"
#include "exa_wm_noca.g4b.gen5"
#include "exa_wm_write.g4b.gen5"
};

static const uint32_t ps_kernel_masknoca_projective_static_gen5[][4] = {
#include "exa_wm_xy.g4b.gen5"
#include "exa_wm_src_projective.g4b.gen5"
#include "exa_wm_src_sample_argb.g4b.gen5"
#include "exa_wm_mask_projective.g4b.gen5"
#include "exa_wm_mask_sample_a.g4b.gen5"
#include "exa_wm_noca.g4b.gen5"
#include "exa_wm_write.g4b.gen5"
};

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
	SAMPLER_STATE_EXTEND_PAD,
	SAMPLER_STATE_EXTEND_REFLECT,
	SAMPLER_STATE_EXTEND_COUNT
} sampler_state_extend_t;

typedef enum {
	WM_KERNEL_NOMASK_AFFINE,
	WM_KERNEL_NOMASK_PROJECTIVE,
	WM_KERNEL_MASKCA_AFFINE,
	WM_KERNEL_MASKCA_PROJECTIVE,
	WM_KERNEL_MASKCA_SRCALPHA_AFFINE,
	WM_KERNEL_MASKCA_SRCALPHA_PROJECTIVE,
	WM_KERNEL_MASKNOCA_AFFINE,
	WM_KERNEL_MASKNOCA_PROJECTIVE,
	WM_KERNEL_COUNT
} wm_kernel_t;

#define KERNEL(kernel_enum, kernel, masked) \
    [kernel_enum] = {&kernel, sizeof(kernel), masked}
struct wm_kernel_info {
	void *data;
	unsigned int size;
	Bool has_mask;
};

static struct wm_kernel_info wm_kernels[] = {
	KERNEL(WM_KERNEL_NOMASK_AFFINE,
	       ps_kernel_nomask_affine_static, FALSE),
	KERNEL(WM_KERNEL_NOMASK_PROJECTIVE,
	       ps_kernel_nomask_projective_static, FALSE),
	KERNEL(WM_KERNEL_MASKCA_AFFINE,
	       ps_kernel_maskca_affine_static, TRUE),
	KERNEL(WM_KERNEL_MASKCA_PROJECTIVE,
	       ps_kernel_maskca_projective_static, TRUE),
	KERNEL(WM_KERNEL_MASKCA_SRCALPHA_AFFINE,
	       ps_kernel_maskca_srcalpha_affine_static, TRUE),
	KERNEL(WM_KERNEL_MASKCA_SRCALPHA_PROJECTIVE,
	       ps_kernel_maskca_srcalpha_projective_static, TRUE),
	KERNEL(WM_KERNEL_MASKNOCA_AFFINE,
	       ps_kernel_masknoca_affine_static, TRUE),
	KERNEL(WM_KERNEL_MASKNOCA_PROJECTIVE,
	       ps_kernel_masknoca_projective_static, TRUE),
};

static struct wm_kernel_info wm_kernels_gen5[] = {
	KERNEL(WM_KERNEL_NOMASK_AFFINE,
	       ps_kernel_nomask_affine_static_gen5, FALSE),
	KERNEL(WM_KERNEL_NOMASK_PROJECTIVE,
	       ps_kernel_nomask_projective_static_gen5, FALSE),
	KERNEL(WM_KERNEL_MASKCA_AFFINE,
	       ps_kernel_maskca_affine_static_gen5, TRUE),
	KERNEL(WM_KERNEL_MASKCA_PROJECTIVE,
	       ps_kernel_maskca_projective_static_gen5, TRUE),
	KERNEL(WM_KERNEL_MASKCA_SRCALPHA_AFFINE,
	       ps_kernel_maskca_srcalpha_affine_static_gen5, TRUE),
	KERNEL(WM_KERNEL_MASKCA_SRCALPHA_PROJECTIVE,
	       ps_kernel_maskca_srcalpha_projective_static_gen5, TRUE),
	KERNEL(WM_KERNEL_MASKNOCA_AFFINE,
	       ps_kernel_masknoca_affine_static_gen5, TRUE),
	KERNEL(WM_KERNEL_MASKNOCA_PROJECTIVE,
	       ps_kernel_masknoca_projective_static_gen5, TRUE),
};

#undef KERNEL

typedef struct _brw_cc_unit_state_padded {
	struct brw_cc_unit_state state;
	char pad[64 - sizeof(struct brw_cc_unit_state)];
} brw_cc_unit_state_padded;

typedef struct brw_surface_state_padded {
	struct brw_surface_state state;
	char pad[32 - sizeof(struct brw_surface_state)];
} brw_surface_state_padded;

struct gen4_cc_unit_state {
	/* Index by [src_blend][dst_blend] */
	brw_cc_unit_state_padded cc_state[BRW_BLENDFACTOR_COUNT]
	    [BRW_BLENDFACTOR_COUNT];
};

typedef float gen4_vertex_buffer[VERTEX_BUFFER_SIZE];

typedef struct gen4_composite_op {
	int op;
	drm_intel_bo *binding_table_bo;
	sampler_state_filter_t src_filter;
	sampler_state_filter_t mask_filter;
	sampler_state_extend_t src_extend;
	sampler_state_extend_t mask_extend;
	Bool is_affine;
	wm_kernel_t wm_kernel;
} gen4_composite_op;

/** Private data for gen4 render accel implementation. */
struct gen4_render_state {
	drm_intel_bo *vs_state_bo;
	drm_intel_bo *sf_state_bo;
	drm_intel_bo *sf_mask_state_bo;
	drm_intel_bo *cc_state_bo;
	drm_intel_bo *wm_state_bo[WM_KERNEL_COUNT]
	    [SAMPLER_STATE_FILTER_COUNT]
	    [SAMPLER_STATE_EXTEND_COUNT]
	    [SAMPLER_STATE_FILTER_COUNT]
	    [SAMPLER_STATE_EXTEND_COUNT];
	drm_intel_bo *wm_kernel_bo[WM_KERNEL_COUNT];

	drm_intel_bo *sip_kernel_bo;
	dri_bo *vertex_buffer_bo;

	gen4_composite_op composite_op;

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
static drm_intel_bo *gen4_create_sf_state(ScrnInfoPtr scrn,
					  drm_intel_bo * kernel_bo)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct brw_sf_unit_state *sf_state;
	drm_intel_bo *sf_state_bo;

	sf_state_bo = drm_intel_bo_alloc(intel->bufmgr, "gen4 SF state",
					 sizeof(*sf_state), 4096);
	drm_intel_bo_map(sf_state_bo, TRUE);
	sf_state = sf_state_bo->virtual;

	memset(sf_state, 0, sizeof(*sf_state));
	sf_state->thread0.grf_reg_count = BRW_GRF_BLOCKS(SF_KERNEL_NUM_GRF);
	sf_state->thread0.kernel_start_pointer =
	    intel_emit_reloc(sf_state_bo,
			     offsetof(struct brw_sf_unit_state, thread0),
			     kernel_bo, sf_state->thread0.grf_reg_count << 1,
			     I915_GEM_DOMAIN_INSTRUCTION, 0) >> 6;
	sf_state->sf1.single_program_flow = 1;
	sf_state->sf1.binding_table_entry_count = 0;
	sf_state->sf1.thread_priority = 0;
	sf_state->sf1.floating_point_mode = 0;	/* Mesa does this */
	sf_state->sf1.illegal_op_exception_enable = 1;
	sf_state->sf1.mask_stack_exception_enable = 1;
	sf_state->sf1.sw_exception_enable = 1;
	sf_state->thread2.per_thread_scratch_space = 0;
	/* scratch space is not used in our kernel */
	sf_state->thread2.scratch_space_base_pointer = 0;
	sf_state->thread3.const_urb_entry_read_length = 0;	/* no const URBs */
	sf_state->thread3.const_urb_entry_read_offset = 0;	/* no const URBs */
	sf_state->thread3.urb_entry_read_length = 1;	/* 1 URB per vertex */
	/* don't smash vertex header, read start from dw8 */
	sf_state->thread3.urb_entry_read_offset = 1;
	sf_state->thread3.dispatch_grf_start_reg = 3;
	sf_state->thread4.max_threads = SF_MAX_THREADS - 1;
	sf_state->thread4.urb_entry_allocation_size = URB_SF_ENTRY_SIZE - 1;
	sf_state->thread4.nr_urb_entries = URB_SF_ENTRIES;
	sf_state->thread4.stats_enable = 1;
	sf_state->sf5.viewport_transform = FALSE;	/* skip viewport */
	sf_state->sf6.cull_mode = BRW_CULLMODE_NONE;
	sf_state->sf6.scissor = 0;
	sf_state->sf7.trifan_pv = 2;
	sf_state->sf6.dest_org_vbias = 0x8;
	sf_state->sf6.dest_org_hbias = 0x8;

	drm_intel_bo_unmap(sf_state_bo);

	return sf_state_bo;
}

static drm_intel_bo *sampler_border_color_create(ScrnInfoPtr scrn)
{
	struct brw_sampler_legacy_border_color sampler_border_color;

	/* Set up the sampler border color (always transparent black) */
	memset(&sampler_border_color, 0, sizeof(sampler_border_color));
	sampler_border_color.color[0] = 0;	/* R */
	sampler_border_color.color[1] = 0;	/* G */
	sampler_border_color.color[2] = 0;	/* B */
	sampler_border_color.color[3] = 0;	/* A */

	return intel_bo_alloc_for_data(scrn,
				       &sampler_border_color,
				       sizeof(sampler_border_color),
				       "gen4 render sampler border color");
}

static void
sampler_state_init(drm_intel_bo * sampler_state_bo,
		   struct brw_sampler_state *sampler_state,
		   sampler_state_filter_t filter,
		   sampler_state_extend_t extend,
		   drm_intel_bo * border_color_bo)
{
	uint32_t sampler_state_offset;

	sampler_state_offset = (char *)sampler_state -
	    (char *)sampler_state_bo->virtual;

	/* PS kernel use this sampler */
	memset(sampler_state, 0, sizeof(*sampler_state));

	sampler_state->ss0.lod_preclamp = 1;	/* GL mode */

	/* We use the legacy mode to get the semantics specified by
	 * the Render extension. */
	sampler_state->ss0.border_color_mode = BRW_BORDER_COLOR_MODE_LEGACY;

	switch (filter) {
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
	case SAMPLER_STATE_EXTEND_PAD:
		sampler_state->ss1.r_wrap_mode = BRW_TEXCOORDMODE_CLAMP;
		sampler_state->ss1.s_wrap_mode = BRW_TEXCOORDMODE_CLAMP;
		sampler_state->ss1.t_wrap_mode = BRW_TEXCOORDMODE_CLAMP;
		break;
	case SAMPLER_STATE_EXTEND_REFLECT:
		sampler_state->ss1.r_wrap_mode = BRW_TEXCOORDMODE_MIRROR;
		sampler_state->ss1.s_wrap_mode = BRW_TEXCOORDMODE_MIRROR;
		sampler_state->ss1.t_wrap_mode = BRW_TEXCOORDMODE_MIRROR;
		break;
	}

	sampler_state->ss2.border_color_pointer =
	    intel_emit_reloc(sampler_state_bo, sampler_state_offset +
			     offsetof(struct brw_sampler_state, ss2),
			     border_color_bo, 0,
			     I915_GEM_DOMAIN_SAMPLER, 0) >> 5;

	sampler_state->ss3.chroma_key_enable = 0;	/* disable chromakey */
}

static drm_intel_bo *gen4_create_sampler_state(ScrnInfoPtr scrn,
					       sampler_state_filter_t
					       src_filter,
					       sampler_state_extend_t
					       src_extend,
					       sampler_state_filter_t
					       mask_filter,
					       sampler_state_extend_t
					       mask_extend,
					       drm_intel_bo * border_color_bo)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *sampler_state_bo;
	struct brw_sampler_state *sampler_state;

	sampler_state_bo =
	    drm_intel_bo_alloc(intel->bufmgr, "gen4 sampler state",
			       sizeof(struct brw_sampler_state) * 2, 4096);
	drm_intel_bo_map(sampler_state_bo, TRUE);
	sampler_state = sampler_state_bo->virtual;

	sampler_state_init(sampler_state_bo,
			   &sampler_state[0],
			   src_filter, src_extend, border_color_bo);
	sampler_state_init(sampler_state_bo,
			   &sampler_state[1],
			   mask_filter, mask_extend, border_color_bo);

	drm_intel_bo_unmap(sampler_state_bo);

	return sampler_state_bo;
}

static void
cc_state_init(drm_intel_bo * cc_state_bo,
	      uint32_t cc_state_offset,
	      int src_blend, int dst_blend, drm_intel_bo * cc_vp_bo)
{
	struct brw_cc_unit_state *cc_state;

	cc_state = (struct brw_cc_unit_state *)((char *)cc_state_bo->virtual +
						cc_state_offset);

	memset(cc_state, 0, sizeof(*cc_state));
	cc_state->cc0.stencil_enable = 0;	/* disable stencil */
	cc_state->cc2.depth_test = 0;	/* disable depth test */
	cc_state->cc2.logicop_enable = 0;	/* disable logic op */
	cc_state->cc3.ia_blend_enable = 0;	/* blend alpha same as colors */
	cc_state->cc3.blend_enable = 1;	/* enable color blend */
	cc_state->cc3.alpha_test = 0;	/* disable alpha test */

	cc_state->cc4.cc_viewport_state_offset =
	    intel_emit_reloc(cc_state_bo, cc_state_offset +
			     offsetof(struct brw_cc_unit_state, cc4),
			     cc_vp_bo, 0, I915_GEM_DOMAIN_INSTRUCTION, 0) >> 5;

	cc_state->cc5.dither_enable = 0;	/* disable dither */
	cc_state->cc5.logicop_func = 0xc;	/* COPY */
	cc_state->cc5.statistics_enable = 1;
	cc_state->cc5.ia_blend_function = BRW_BLENDFUNCTION_ADD;

	/* Fill in alpha blend factors same as color, for the future. */
	cc_state->cc5.ia_src_blend_factor = src_blend;
	cc_state->cc5.ia_dest_blend_factor = dst_blend;

	cc_state->cc6.blend_function = BRW_BLENDFUNCTION_ADD;
	cc_state->cc6.clamp_post_alpha_blend = 1;
	cc_state->cc6.clamp_pre_alpha_blend = 1;
	cc_state->cc6.clamp_range = 0;	/* clamp range [0,1] */

	cc_state->cc6.src_blend_factor = src_blend;
	cc_state->cc6.dest_blend_factor = dst_blend;
}

static drm_intel_bo *gen4_create_wm_state(ScrnInfoPtr scrn,
					  Bool has_mask,
					  drm_intel_bo * kernel_bo,
					  drm_intel_bo * sampler_bo)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct brw_wm_unit_state *wm_state;
	drm_intel_bo *wm_state_bo;

	wm_state_bo = drm_intel_bo_alloc(intel->bufmgr, "gen4 WM state",
					 sizeof(*wm_state), 4096);
	drm_intel_bo_map(wm_state_bo, TRUE);
	wm_state = wm_state_bo->virtual;

	memset(wm_state, 0, sizeof(*wm_state));
	wm_state->thread0.grf_reg_count = BRW_GRF_BLOCKS(PS_KERNEL_NUM_GRF);
	wm_state->thread0.kernel_start_pointer =
	    intel_emit_reloc(wm_state_bo,
			     offsetof(struct brw_wm_unit_state, thread0),
			     kernel_bo, wm_state->thread0.grf_reg_count << 1,
			     I915_GEM_DOMAIN_INSTRUCTION, 0) >> 6;

	wm_state->thread1.single_program_flow = 0;

	/* scratch space is not used in our kernel */
	wm_state->thread2.scratch_space_base_pointer = 0;
	wm_state->thread2.per_thread_scratch_space = 0;

	wm_state->thread3.const_urb_entry_read_length = 0;
	wm_state->thread3.const_urb_entry_read_offset = 0;

	wm_state->thread3.urb_entry_read_offset = 0;
	/* wm kernel use urb from 3, see wm_program in compiler module */
	wm_state->thread3.dispatch_grf_start_reg = 3;	/* must match kernel */

	wm_state->wm4.stats_enable = 1;	/* statistic */

	if (IS_IGDNG(intel))
		wm_state->wm4.sampler_count = 0;	/* hardware requirement */
	else
		wm_state->wm4.sampler_count = 1;	/* 1-4 samplers used */

	wm_state->wm4.sampler_state_pointer =
	    intel_emit_reloc(wm_state_bo,
			     offsetof(struct brw_wm_unit_state, wm4),
			     sampler_bo,
			     wm_state->wm4.stats_enable +
			     (wm_state->wm4.sampler_count << 2),
			     I915_GEM_DOMAIN_INSTRUCTION, 0) >> 5;
	wm_state->wm5.max_threads = PS_MAX_THREADS - 1;
	wm_state->wm5.transposed_urb_read = 0;
	wm_state->wm5.thread_dispatch_enable = 1;
	/* just use 16-pixel dispatch (4 subspans), don't need to change kernel
	 * start point
	 */
	wm_state->wm5.enable_16_pix = 1;
	wm_state->wm5.enable_8_pix = 0;
	wm_state->wm5.early_depth_test = 1;

	/* Each pair of attributes (src/mask coords) is two URB entries */
	if (has_mask) {
		wm_state->thread1.binding_table_entry_count = 3;	/* 2 tex and fb */
		wm_state->thread3.urb_entry_read_length = 4;
	} else {
		wm_state->thread1.binding_table_entry_count = 2;	/* 1 tex and fb */
		wm_state->thread3.urb_entry_read_length = 2;
	}

	/* binding table entry count is only used for prefetching, and it has to
	 * be set 0 for IGDNG
	 */
	if (IS_IGDNG(intel))
		wm_state->thread1.binding_table_entry_count = 0;

	drm_intel_bo_unmap(wm_state_bo);

	return wm_state_bo;
}

static drm_intel_bo *gen4_create_cc_viewport(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *bo;
	struct brw_cc_viewport cc_viewport;

	cc_viewport.min_depth = -1.e35;
	cc_viewport.max_depth = 1.e35;

	bo = drm_intel_bo_alloc(intel->bufmgr, "gen4 render unit state",
				sizeof(cc_viewport), 4096);
	drm_intel_bo_subdata(bo, 0, sizeof(cc_viewport), &cc_viewport);

	return bo;
}

static drm_intel_bo *gen4_create_vs_unit_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct brw_vs_unit_state vs_state;
	memset(&vs_state, 0, sizeof(vs_state));

	/* Set up the vertex shader to be disabled (passthrough) */
	if (IS_IGDNG(intel))
		vs_state.thread4.nr_urb_entries = URB_VS_ENTRIES >> 2;	/* hardware requirement */
	else
		vs_state.thread4.nr_urb_entries = URB_VS_ENTRIES;
	vs_state.thread4.urb_entry_allocation_size = URB_VS_ENTRY_SIZE - 1;
	vs_state.vs6.vs_enable = 0;
	vs_state.vs6.vert_cache_disable = 1;

	return intel_bo_alloc_for_data(scrn, &vs_state, sizeof(vs_state),
				       "gen4 render VS state");
}

/**
 * Set up all combinations of cc state: each blendfactor for source and
 * dest.
 */
static drm_intel_bo *gen4_create_cc_unit_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen4_cc_unit_state *cc_state;
	drm_intel_bo *cc_state_bo, *cc_vp_bo;
	int i, j;

	cc_vp_bo = gen4_create_cc_viewport(scrn);

	cc_state_bo = drm_intel_bo_alloc(intel->bufmgr, "gen4 CC state",
					 sizeof(*cc_state), 4096);
	drm_intel_bo_map(cc_state_bo, TRUE);
	cc_state = cc_state_bo->virtual;
	for (i = 0; i < BRW_BLENDFACTOR_COUNT; i++) {
		for (j = 0; j < BRW_BLENDFACTOR_COUNT; j++) {
			cc_state_init(cc_state_bo,
				      offsetof(struct gen4_cc_unit_state,
					       cc_state[i][j].state),
				      i, j, cc_vp_bo);
		}
	}
	drm_intel_bo_unmap(cc_state_bo);

	drm_intel_bo_unreference(cc_vp_bo);

	return cc_state_bo;
}

static uint32_t i965_get_card_format(PicturePtr picture)
{
	int i;

	for (i = 0; i < sizeof(i965_tex_formats) / sizeof(i965_tex_formats[0]);
	     i++) {
		if (i965_tex_formats[i].fmt == picture->format)
			break;
	}
	assert(i != sizeof(i965_tex_formats) / sizeof(i965_tex_formats[0]));

	return i965_tex_formats[i].card_fmt;
}

static sampler_state_filter_t sampler_state_filter_from_picture(int filter)
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

static sampler_state_extend_t sampler_state_extend_from_picture(int repeat_type)
{
	switch (repeat_type) {
	case RepeatNone:
		return SAMPLER_STATE_EXTEND_NONE;
	case RepeatNormal:
		return SAMPLER_STATE_EXTEND_REPEAT;
	case RepeatPad:
		return SAMPLER_STATE_EXTEND_PAD;
	case RepeatReflect:
		return SAMPLER_STATE_EXTEND_REFLECT;
	default:
		return -1;
	}
}

/**
 * Sets up the common fields for a surface state buffer for the given
 * picture in the given surface state buffer.
 */
static void
i965_set_picture_surface_state(intel_screen_private *intel,
			       dri_bo * ss_bo, int ss_index,
			       PicturePtr picture, PixmapPtr pixmap,
			       Bool is_dst)
{
	struct brw_surface_state_padded *ss;
	struct brw_surface_state local_ss;
	struct intel_pixmap *priv = i830_get_pixmap_intel(pixmap);

	ss = (struct brw_surface_state_padded *)ss_bo->virtual + ss_index;

	/* Since ss is a pointer to WC memory, do all of our bit operations
	 * into a local temporary first.
	 */
	memset(&local_ss, 0, sizeof(local_ss));
	local_ss.ss0.surface_type = BRW_SURFACE_2D;
	if (is_dst) {
		uint32_t dst_format = 0;
		Bool ret = TRUE;

		ret = i965_get_dest_format(picture, &dst_format);
		assert(ret == TRUE);
		local_ss.ss0.surface_format = dst_format;
	} else {
		local_ss.ss0.surface_format = i965_get_card_format(picture);
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
	local_ss.ss1.base_addr = priv->bo->offset;

	local_ss.ss2.mip_count = 0;
	local_ss.ss2.render_target_rotation = 0;
	local_ss.ss2.height = pixmap->drawable.height - 1;
	local_ss.ss2.width = pixmap->drawable.width - 1;
	local_ss.ss3.pitch = intel_get_pixmap_pitch(pixmap) - 1;
	local_ss.ss3.tile_walk = 0;	/* Tiled X */
	local_ss.ss3.tiled_surface = i830_pixmap_tiled(pixmap) ? 1 : 0;

	memcpy(ss, &local_ss, sizeof(local_ss));

	if (priv->bo != NULL) {
		uint32_t write_domain, read_domains;

		if (is_dst) {
			write_domain = I915_GEM_DOMAIN_RENDER;
			read_domains = I915_GEM_DOMAIN_RENDER;
		} else {
			write_domain = 0;
			read_domains = I915_GEM_DOMAIN_SAMPLER;
		}

		intel_batch_mark_pixmap_domains(intel, priv, read_domains, write_domain);
		dri_bo_emit_reloc(ss_bo, read_domains, write_domain,
				  0,
				  ss_index * sizeof(*ss) +
				  offsetof(struct brw_surface_state, ss1),
				  priv->bo);
	}
}

static void i965_emit_composite_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen4_render_state *render_state = intel->gen4_render_state;
	gen4_composite_op *composite_op = &render_state->composite_op;
	int op = composite_op->op;
	PicturePtr mask_picture = intel->render_mask_picture;
	PicturePtr dest_picture = intel->render_dest_picture;
	PixmapPtr mask = intel->render_mask;
	PixmapPtr dest = intel->render_dest;
	sampler_state_filter_t src_filter = composite_op->src_filter;
	sampler_state_filter_t mask_filter = composite_op->mask_filter;
	sampler_state_extend_t src_extend = composite_op->src_extend;
	sampler_state_extend_t mask_extend = composite_op->mask_extend;
	Bool is_affine = composite_op->is_affine;
	int urb_vs_start, urb_vs_size;
	int urb_gs_start, urb_gs_size;
	int urb_clip_start, urb_clip_size;
	int urb_sf_start, urb_sf_size;
	int urb_cs_start, urb_cs_size;
	uint32_t src_blend, dst_blend;
	dri_bo *binding_table_bo = composite_op->binding_table_bo;

	intel->needs_render_state_emit = FALSE;

	IntelEmitInvarientState(scrn);
	intel->last_3d = LAST_3D_RENDER;

	/* Mark the destination dirty within this batch */
	intel_batch_mark_pixmap_domains(intel,
					i830_get_pixmap_intel(dest),
					I915_GEM_DOMAIN_RENDER,
					I915_GEM_DOMAIN_RENDER);

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

	i965_get_blend_cntl(op, mask_picture, dest_picture->format,
			    &src_blend, &dst_blend);

	/* Begin the long sequence of commands needed to set up the 3D
	 * rendering pipe
	 */

	/* URB fence. Erratum (Vol 1a, p32): URB_FENCE must not cross a
	 * cache-line (64 bytes). Start by aligning this sequence of ops to
	 * a cache-line...
	 */
	ALIGN_BATCH(64);

	assert(intel->in_batch_atomic);
	{
		/* Match Mesa driver setup */
		OUT_BATCH(MI_FLUSH |
			  MI_STATE_INSTRUCTION_CACHE_FLUSH |
			  BRW_MI_GLOBAL_SNAPSHOT_RESET);
		if (IS_G4X(intel) || IS_IGDNG(intel))
			OUT_BATCH(NEW_PIPELINE_SELECT | PIPELINE_SELECT_3D);
		else
			OUT_BATCH(BRW_PIPELINE_SELECT | PIPELINE_SELECT_3D);

		OUT_BATCH(BRW_CS_URB_STATE | 0);
		OUT_BATCH((0 << 4) |	/* URB Entry Allocation Size */
			  (0 << 0));	/* Number of URB Entries */

		/* Zero out the two base address registers so all offsets are
		 * absolute.
		 */
		if (IS_IGDNG(intel)) {
			OUT_BATCH(BRW_STATE_BASE_ADDRESS | 6);
			OUT_BATCH(0 | BASE_ADDRESS_MODIFY);	/* Generate state base address */
			OUT_BATCH(0 | BASE_ADDRESS_MODIFY);	/* Surface state base address */
			OUT_BATCH(0 | BASE_ADDRESS_MODIFY);	/* media base addr, don't care */
			OUT_BATCH(0 | BASE_ADDRESS_MODIFY);	/* Instruction base address */
			/* general state max addr, disabled */
			OUT_BATCH(0x10000000 | BASE_ADDRESS_MODIFY);
			/* media object state max addr, disabled */
			OUT_BATCH(0x10000000 | BASE_ADDRESS_MODIFY);
			/* Instruction max addr, disabled */
			OUT_BATCH(0x10000000 | BASE_ADDRESS_MODIFY);
		} else {
			OUT_BATCH(BRW_STATE_BASE_ADDRESS | 4);
			OUT_BATCH(0 | BASE_ADDRESS_MODIFY);	/* Generate state base address */
			OUT_BATCH(0 | BASE_ADDRESS_MODIFY);	/* Surface state base address */
			OUT_BATCH(0 | BASE_ADDRESS_MODIFY);	/* media base addr, don't care */
			/* general state max addr, disabled */
			OUT_BATCH(0x10000000 | BASE_ADDRESS_MODIFY);
			/* media object state max addr, disabled */
			OUT_BATCH(0x10000000 | BASE_ADDRESS_MODIFY);
		}
		/* Set system instruction pointer */
		OUT_BATCH(BRW_STATE_SIP | 0);
		OUT_RELOC(render_state->sip_kernel_bo,
			  I915_GEM_DOMAIN_INSTRUCTION, 0, 0);
	}

	if (IS_IGDNG(intel)) {
		/* Ironlake errata workaround: Before disabling the clipper,
		 * you have to MI_FLUSH to get the pipeline idle.
		 */
		OUT_BATCH(MI_FLUSH);
	}

	{
		int pipe_ctrl;
		/* Pipe control */

		if (IS_IGDNG(intel))
			pipe_ctrl = BRW_PIPE_CONTROL_NOWRITE;
		else
			pipe_ctrl =
			    BRW_PIPE_CONTROL_NOWRITE |
			    BRW_PIPE_CONTROL_IS_FLUSH;

		OUT_BATCH(BRW_PIPE_CONTROL | pipe_ctrl | 2);
		OUT_BATCH(0);	/* Destination address */
		OUT_BATCH(0);	/* Immediate data low DW */
		OUT_BATCH(0);	/* Immediate data high DW */

		/* Binding table pointers */
		OUT_BATCH(BRW_3DSTATE_BINDING_TABLE_POINTERS | 4);
		OUT_BATCH(0);	/* vs */
		OUT_BATCH(0);	/* gs */
		OUT_BATCH(0);	/* clip */
		OUT_BATCH(0);	/* sf */
		/* Only the PS uses the binding table */
		OUT_RELOC(binding_table_bo, I915_GEM_DOMAIN_SAMPLER, 0, 0);

		/* The drawing rectangle clipping is always on.  Set it to values that
		 * shouldn't do any clipping.
		 */
		OUT_BATCH(BRW_3DSTATE_DRAWING_RECTANGLE | 2);
		OUT_BATCH(0x00000000);	/* ymin, xmin */
		OUT_BATCH(DRAW_YMAX(dest->drawable.height - 1) | DRAW_XMAX(dest->drawable.width - 1));	/* ymax, xmax */
		OUT_BATCH(0x00000000);	/* yorigin, xorigin */

		/* skip the depth buffer */
		/* skip the polygon stipple */
		/* skip the polygon stipple offset */
		/* skip the line stipple */

		/* Set the pointers to the 3d pipeline state */
		OUT_BATCH(BRW_3DSTATE_PIPELINED_POINTERS | 5);
		OUT_RELOC(render_state->vs_state_bo,
			  I915_GEM_DOMAIN_INSTRUCTION, 0, 0);
		OUT_BATCH(BRW_GS_DISABLE);	/* disable GS, resulting in passthrough */
		OUT_BATCH(BRW_CLIP_DISABLE);	/* disable CLIP, resulting in passthrough */
		if (mask) {
			OUT_RELOC(render_state->sf_mask_state_bo,
				  I915_GEM_DOMAIN_INSTRUCTION, 0, 0);
		} else {
			OUT_RELOC(render_state->sf_state_bo,
				  I915_GEM_DOMAIN_INSTRUCTION, 0, 0);
		}

		OUT_RELOC(render_state->wm_state_bo[composite_op->wm_kernel]
			  [src_filter][src_extend]
			  [mask_filter][mask_extend],
			  I915_GEM_DOMAIN_INSTRUCTION, 0, 0);

		OUT_RELOC(render_state->cc_state_bo,
			  I915_GEM_DOMAIN_INSTRUCTION, 0,
			  offsetof(struct gen4_cc_unit_state,
				   cc_state[src_blend][dst_blend]));

		/* URB fence. Erratum (Vol 1a, p32): URB_FENCE must not cross a
		 * cache-line (64 bytes).
		 *
		 * 21 preceding dwords since start of section: 84 bytes.
		 * 12 bytes for URB_FENCE, implies that the end-of-instruction
		 * does not cross the cache-line boundary...
		 *
		 * A total of 33 or 35 dwords since alignment: 132, 140 bytes.
		 * Again, the URB_FENCE will not cross a cache-line.
		 */
		OUT_BATCH(BRW_URB_FENCE |
			  UF0_CS_REALLOC |
			  UF0_SF_REALLOC |
			  UF0_CLIP_REALLOC |
			  UF0_GS_REALLOC | UF0_VS_REALLOC | 1);
		OUT_BATCH(((urb_clip_start +
			    urb_clip_size) << UF1_CLIP_FENCE_SHIFT) |
			  ((urb_gs_start +
			    urb_gs_size) << UF1_GS_FENCE_SHIFT) | ((urb_vs_start
								    +
								    urb_vs_size)
								   <<
								   UF1_VS_FENCE_SHIFT));
		OUT_BATCH(((urb_cs_start +
			    urb_cs_size) << UF2_CS_FENCE_SHIFT) | ((urb_sf_start
								    +
								    urb_sf_size)
								   <<
								   UF2_SF_FENCE_SHIFT));

		/* Constant buffer state */
		OUT_BATCH(BRW_CS_URB_STATE | 0);
		OUT_BATCH(((URB_CS_ENTRY_SIZE - 1) << 4) |
			  (URB_CS_ENTRIES << 0));
	}
	{
		/*
		 * number of extra parameters per vertex
		 */
		int nelem = mask ? 2 : 1;
		/*
		 * size of extra parameters:
		 *  3 for homogenous (xyzw)
		 *  2 for cartesian (xy)
		 */
		int selem = is_affine ? 2 : 3;
		uint32_t w_component;
		uint32_t src_format;

		render_state->vertex_size = 4 * (2 + nelem * selem);

		if (is_affine) {
			src_format = BRW_SURFACEFORMAT_R32G32_FLOAT;
			w_component = BRW_VFCOMPONENT_STORE_1_FLT;
		} else {
			src_format = BRW_SURFACEFORMAT_R32G32B32_FLOAT;
			w_component = BRW_VFCOMPONENT_STORE_SRC;
		}

		if (IS_IGDNG(intel)) {
			/*
			 * The reason to add this extra vertex element in the header is that
			 * IGDNG has different vertex header definition and origin method to
			 * set destination element offset doesn't exist anymore, which means
			 * hardware requires a predefined vertex element layout.
			 *
			 * haihao proposed this approach to fill the first vertex element, so
			 * origin layout for Gen4 doesn't need to change, and origin shader
			 * programs behavior is also kept.
			 *
			 * I think this is not bad. - zhenyu
			 */

			OUT_BATCH(BRW_3DSTATE_VERTEX_ELEMENTS |
				  ((2 * (2 + nelem)) - 1));
			OUT_BATCH((0 << VE0_VERTEX_BUFFER_INDEX_SHIFT) |
				  VE0_VALID | (BRW_SURFACEFORMAT_R32G32_FLOAT <<
					       VE0_FORMAT_SHIFT) | (0 <<
								    VE0_OFFSET_SHIFT));

			OUT_BATCH((BRW_VFCOMPONENT_STORE_0 <<
				   VE1_VFCOMPONENT_0_SHIFT) |
				  (BRW_VFCOMPONENT_STORE_0 <<
				   VE1_VFCOMPONENT_1_SHIFT) |
				  (BRW_VFCOMPONENT_STORE_0 <<
				   VE1_VFCOMPONENT_2_SHIFT) |
				  (BRW_VFCOMPONENT_STORE_0 <<
				   VE1_VFCOMPONENT_3_SHIFT));
		} else {
			/* Set up our vertex elements, sourced from the single vertex buffer.
			 * that will be set up later.
			 */
			OUT_BATCH(BRW_3DSTATE_VERTEX_ELEMENTS |
				  ((2 * (1 + nelem)) - 1));
		}

		/* x,y */
		OUT_BATCH((0 << VE0_VERTEX_BUFFER_INDEX_SHIFT) |
			  VE0_VALID |
			  (BRW_SURFACEFORMAT_R32G32_FLOAT << VE0_FORMAT_SHIFT) |
			  (0 << VE0_OFFSET_SHIFT));

		if (IS_IGDNG(intel))
			OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC <<
				   VE1_VFCOMPONENT_0_SHIFT) |
				  (BRW_VFCOMPONENT_STORE_SRC <<
				   VE1_VFCOMPONENT_1_SHIFT) |
				  (BRW_VFCOMPONENT_STORE_1_FLT <<
				   VE1_VFCOMPONENT_2_SHIFT) |
				  (BRW_VFCOMPONENT_STORE_1_FLT <<
				   VE1_VFCOMPONENT_3_SHIFT));
		else
			OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC <<
				   VE1_VFCOMPONENT_0_SHIFT) |
				  (BRW_VFCOMPONENT_STORE_SRC <<
				   VE1_VFCOMPONENT_1_SHIFT) |
				  (BRW_VFCOMPONENT_STORE_1_FLT <<
				   VE1_VFCOMPONENT_2_SHIFT) |
				  (BRW_VFCOMPONENT_STORE_1_FLT <<
				   VE1_VFCOMPONENT_3_SHIFT) | (4 <<
							       VE1_DESTINATION_ELEMENT_OFFSET_SHIFT));
		/* u0, v0, w0 */
		OUT_BATCH((0 << VE0_VERTEX_BUFFER_INDEX_SHIFT) | VE0_VALID | (src_format << VE0_FORMAT_SHIFT) | ((2 * 4) << VE0_OFFSET_SHIFT));	/* offset vb in bytes */

		if (IS_IGDNG(intel))
			OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC <<
				   VE1_VFCOMPONENT_0_SHIFT) |
				  (BRW_VFCOMPONENT_STORE_SRC <<
				   VE1_VFCOMPONENT_1_SHIFT) | (w_component <<
							       VE1_VFCOMPONENT_2_SHIFT)
				  | (BRW_VFCOMPONENT_STORE_1_FLT <<
				     VE1_VFCOMPONENT_3_SHIFT));
		else
			OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT) | (BRW_VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_1_SHIFT) | (w_component << VE1_VFCOMPONENT_2_SHIFT) | (BRW_VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_3_SHIFT) | ((4 + 4) << VE1_DESTINATION_ELEMENT_OFFSET_SHIFT));	/* VUE offset in dwords */
		/* u1, v1, w1 */
		if (mask) {
			OUT_BATCH((0 << VE0_VERTEX_BUFFER_INDEX_SHIFT) | VE0_VALID | (src_format << VE0_FORMAT_SHIFT) | (((2 + selem) * 4) << VE0_OFFSET_SHIFT));	/* vb offset in bytes */

			if (IS_IGDNG(intel))
				OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC <<
					   VE1_VFCOMPONENT_0_SHIFT) |
					  (BRW_VFCOMPONENT_STORE_SRC <<
					   VE1_VFCOMPONENT_1_SHIFT) |
					  (w_component <<
					   VE1_VFCOMPONENT_2_SHIFT) |
					  (BRW_VFCOMPONENT_STORE_1_FLT <<
					   VE1_VFCOMPONENT_3_SHIFT));
			else
				OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT) | (BRW_VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_1_SHIFT) | (w_component << VE1_VFCOMPONENT_2_SHIFT) | (BRW_VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_3_SHIFT) | ((4 + 4 + 4) << VE1_DESTINATION_ELEMENT_OFFSET_SHIFT));	/* VUE offset in dwords */
		}
	}
}

/**
 * Returns whether the current set of composite state plus vertex buffer is
 * expected to fit in the aperture.
 */
static Bool i965_composite_check_aperture(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen4_render_state *render_state = intel->gen4_render_state;
	gen4_composite_op *composite_op = &render_state->composite_op;
	drm_intel_bo *bo_table[] = {
		intel->batch_bo,
		composite_op->binding_table_bo,
		render_state->vertex_buffer_bo,
		render_state->vs_state_bo,
		render_state->sf_state_bo,
		render_state->sf_mask_state_bo,
		render_state->wm_state_bo[composite_op->wm_kernel]
		    [composite_op->src_filter]
		    [composite_op->src_extend]
		    [composite_op->mask_filter]
		    [composite_op->mask_extend],
		render_state->cc_state_bo,
		render_state->sip_kernel_bo,
	};

	return drm_intel_bufmgr_check_aperture_space(bo_table,
						     ARRAY_SIZE(bo_table)) == 0;
}

Bool
i965_prepare_composite(int op, PicturePtr source_picture,
		       PicturePtr mask_picture, PicturePtr dest_picture,
		       PixmapPtr source, PixmapPtr mask, PixmapPtr dest)
{
	ScrnInfoPtr scrn = xf86Screens[dest_picture->pDrawable->pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen4_render_state *render_state = intel->gen4_render_state;
	gen4_composite_op *composite_op;
	uint32_t *binding_table;
	drm_intel_bo *binding_table_bo, *surface_state_bo;

	if (render_state == NULL) 
		return FALSE;

	composite_op = &render_state->composite_op;

	composite_op->src_filter =
	    sampler_state_filter_from_picture(source_picture->filter);
	if (composite_op->src_filter < 0) {
		intel_debug_fallback(scrn, "Bad src filter 0x%x\n",
				     source_picture->filter);
		return FALSE;
	}
	composite_op->src_extend =
	    sampler_state_extend_from_picture(source_picture->repeatType);
	if (composite_op->src_extend < 0) {
		intel_debug_fallback(scrn, "Bad src repeat 0x%x\n",
				     source_picture->repeatType);
		return FALSE;
	}

	if (mask_picture) {
		if (mask_picture->componentAlpha &&
		    PICT_FORMAT_RGB(mask_picture->format)) {
			/* Check if it's component alpha that relies on a source alpha and on
			 * the source value.  We can only get one of those into the single
			 * source value that we get to blend with.
			 */
			if (i965_blend_op[op].src_alpha &&
			    (i965_blend_op[op].src_blend != BRW_BLENDFACTOR_ZERO)) {
				intel_debug_fallback(scrn,
						     "Component alpha not supported "
						     "with source alpha and source "
						     "value blending.\n");
				return FALSE;
			}
		}

		composite_op->mask_filter =
		    sampler_state_filter_from_picture(mask_picture->filter);
		if (composite_op->mask_filter < 0) {
			intel_debug_fallback(scrn, "Bad mask filter 0x%x\n",
					     mask_picture->filter);
			return FALSE;
		}
		composite_op->mask_extend =
		    sampler_state_extend_from_picture(mask_picture->repeatType);
		if (composite_op->mask_extend < 0) {
			intel_debug_fallback(scrn, "Bad mask repeat 0x%x\n",
					     mask_picture->repeatType);
			return FALSE;
		}
	} else {
		composite_op->mask_filter = SAMPLER_STATE_FILTER_NEAREST;
		composite_op->mask_extend = SAMPLER_STATE_EXTEND_NONE;
	}

	/* Flush any pending writes prior to relocating the textures. */
	if(i830_uxa_pixmap_is_dirty(source) ||
	   (mask && i830_uxa_pixmap_is_dirty(mask)))
		intel_batch_emit_flush(scrn);


	/* Set up the surface states. */
	surface_state_bo = dri_bo_alloc(intel->bufmgr, "surface_state",
					3 * sizeof(brw_surface_state_padded),
					4096);
	if (dri_bo_map(surface_state_bo, 1) != 0) {
		dri_bo_unreference(surface_state_bo);
		return FALSE;
	}
	/* Set up the state buffer for the destination surface */
	i965_set_picture_surface_state(intel, surface_state_bo, 0,
				       dest_picture, dest, TRUE);
	/* Set up the source surface state buffer */
	i965_set_picture_surface_state(intel, surface_state_bo, 1,
				       source_picture, source, FALSE);
	if (mask) {
		/* Set up the mask surface state buffer */
		i965_set_picture_surface_state(intel, surface_state_bo, 2,
					       mask_picture, mask, FALSE);
	}
	dri_bo_unmap(surface_state_bo);

	/* Set up the binding table of surface indices to surface state. */
	binding_table_bo = dri_bo_alloc(intel->bufmgr, "binding_table",
					3 * sizeof(uint32_t), 4096);
	if (dri_bo_map(binding_table_bo, 1) != 0) {
		dri_bo_unreference(binding_table_bo);
		dri_bo_unreference(surface_state_bo);
		return FALSE;
	}

	binding_table = binding_table_bo->virtual;
	binding_table[0] = intel_emit_reloc(binding_table_bo,
					    0 * sizeof(uint32_t),
					    surface_state_bo,
					    0 *
					    sizeof(brw_surface_state_padded),
					    I915_GEM_DOMAIN_INSTRUCTION, 0);

	binding_table[1] = intel_emit_reloc(binding_table_bo,
					    1 * sizeof(uint32_t),
					    surface_state_bo,
					    1 *
					    sizeof(brw_surface_state_padded),
					    I915_GEM_DOMAIN_INSTRUCTION, 0);

	if (mask) {
		binding_table[2] = intel_emit_reloc(binding_table_bo,
						    2 * sizeof(uint32_t),
						    surface_state_bo,
						    2 *
						    sizeof
						    (brw_surface_state_padded),
						    I915_GEM_DOMAIN_INSTRUCTION,
						    0);
	} else {
		binding_table[2] = 0;
	}
	dri_bo_unmap(binding_table_bo);
	/* All refs to surface_state are now contained in binding_table_bo. */
	drm_intel_bo_unreference(surface_state_bo);

	composite_op->op = op;
	intel->render_source_picture = source_picture;
	intel->render_mask_picture = mask_picture;
	intel->render_dest_picture = dest_picture;
	intel->render_source = source;
	intel->render_mask = mask;
	intel->render_dest = dest;
	drm_intel_bo_unreference(composite_op->binding_table_bo);
	composite_op->binding_table_bo = binding_table_bo;

	intel->scale_units[0][0] = source->drawable.width;
	intel->scale_units[0][1] = source->drawable.height;

	intel->transform[0] = source_picture->transform;
	composite_op->is_affine = i830_transform_is_affine(intel->transform[0]);

	if (!mask) {
		intel->transform[1] = NULL;
		intel->scale_units[1][0] = -1;
		intel->scale_units[1][1] = -1;
	} else {
		intel->transform[1] = mask_picture->transform;
		intel->scale_units[1][0] = mask->drawable.width;
		intel->scale_units[1][1] = mask->drawable.height;
		composite_op->is_affine &=
		    i830_transform_is_affine(intel->transform[1]);
	}

	if (mask) {
		if (mask_picture->componentAlpha &&
		    PICT_FORMAT_RGB(mask_picture->format)) {
			if (i965_blend_op[op].src_alpha) {
				if (composite_op->is_affine)
					composite_op->wm_kernel =
					    WM_KERNEL_MASKCA_SRCALPHA_AFFINE;
				else
					composite_op->wm_kernel =
					    WM_KERNEL_MASKCA_SRCALPHA_PROJECTIVE;
			} else {
				if (composite_op->is_affine)
					composite_op->wm_kernel =
					    WM_KERNEL_MASKCA_AFFINE;
				else
					composite_op->wm_kernel =
					    WM_KERNEL_MASKCA_PROJECTIVE;
			}
		} else {
			if (composite_op->is_affine)
				composite_op->wm_kernel =
				    WM_KERNEL_MASKNOCA_AFFINE;
			else
				composite_op->wm_kernel =
				    WM_KERNEL_MASKNOCA_PROJECTIVE;
		}
	} else {
		if (composite_op->is_affine)
			composite_op->wm_kernel = WM_KERNEL_NOMASK_AFFINE;
		else
			composite_op->wm_kernel = WM_KERNEL_NOMASK_PROJECTIVE;
	}

	if (!i965_composite_check_aperture(scrn)) {
		intel_batch_submit(scrn, FALSE);
		if (!i965_composite_check_aperture(scrn)) {
			intel_debug_fallback(scrn,
					     "Couldn't fit render operation "
					     "in aperture\n");
			return FALSE;
		}
	}

	intel->needs_render_state_emit = TRUE;

	return TRUE;
}

static drm_intel_bo *i965_get_vb_space(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen4_render_state *render_state = intel->gen4_render_state;

	/* If the vertex buffer is too full, then we free the old and a new one
	 * gets made.
	 */
	if (render_state->vb_offset + VERTEX_FLOATS_PER_COMPOSITE >
	    VERTEX_BUFFER_SIZE) {
		drm_intel_bo_unreference(render_state->vertex_buffer_bo);
		render_state->vertex_buffer_bo = NULL;
	}

	/* Alloc a new vertex buffer if necessary. */
	if (render_state->vertex_buffer_bo == NULL) {
		render_state->vertex_buffer_bo =
		    drm_intel_bo_alloc(intel->bufmgr, "vb",
				       sizeof(gen4_vertex_buffer), 4096);
		render_state->vb_offset = 0;
	}

	drm_intel_bo_reference(render_state->vertex_buffer_bo);
	return render_state->vertex_buffer_bo;
}

void
i965_composite(PixmapPtr dest, int srcX, int srcY, int maskX, int maskY,
	       int dstX, int dstY, int w, int h)
{
	ScrnInfoPtr scrn = xf86Screens[dest->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen4_render_state *render_state = intel->gen4_render_state;
	Bool has_mask;
	float src_x[3], src_y[3], src_w[3], mask_x[3], mask_y[3], mask_w[3];
	int i;
	drm_intel_bo *vb_bo;
	float vb[24]; /* 3 * (2 dst + 3 src + 3 mask) */
	Bool is_affine = render_state->composite_op.is_affine;

	if (is_affine) {
		if (!i830_get_transformed_coordinates(srcX, srcY,
						      intel->transform[0],
						      &src_x[0], &src_y[0]))
			return;
		if (!i830_get_transformed_coordinates(srcX, srcY + h,
						      intel->transform[0],
						      &src_x[1], &src_y[1]))
			return;
		if (!i830_get_transformed_coordinates(srcX + w, srcY + h,
						      intel->transform[0],
						      &src_x[2], &src_y[2]))
			return;
	} else {
		if (!i830_get_transformed_coordinates_3d(srcX, srcY,
							 intel->transform[0],
							 &src_x[0], &src_y[0],
							 &src_w[0]))
			return;
		if (!i830_get_transformed_coordinates_3d(srcX, srcY + h,
							 intel->transform[0],
							 &src_x[1], &src_y[1],
							 &src_w[1]))
			return;
		if (!i830_get_transformed_coordinates_3d(srcX + w, srcY + h,
							 intel->transform[0],
							 &src_x[2], &src_y[2],
							 &src_w[2]))
			return;
	}

	if (intel->scale_units[1][0] == -1 || intel->scale_units[1][1] == -1) {
		has_mask = FALSE;
	} else {
		has_mask = TRUE;
		if (is_affine) {
			if (!i830_get_transformed_coordinates(maskX, maskY,
							      intel->
							      transform[1],
							      &mask_x[0],
							      &mask_y[0]))
				return;
			if (!i830_get_transformed_coordinates(maskX, maskY + h,
							      intel->
							      transform[1],
							      &mask_x[1],
							      &mask_y[1]))
				return;
			if (!i830_get_transformed_coordinates
			    (maskX + w, maskY + h, intel->transform[1],
			     &mask_x[2], &mask_y[2]))
				return;
		} else {
			if (!i830_get_transformed_coordinates_3d(maskX, maskY,
								 intel->
								 transform[1],
								 &mask_x[0],
								 &mask_y[0],
								 &mask_w[0]))
				return;
			if (!i830_get_transformed_coordinates_3d
			    (maskX, maskY + h, intel->transform[1], &mask_x[1],
			     &mask_y[1], &mask_w[1]))
				return;
			if (!i830_get_transformed_coordinates_3d
			    (maskX + w, maskY + h, intel->transform[1],
			     &mask_x[2], &mask_y[2], &mask_w[2]))
				return;
		}
	}

	vb_bo = i965_get_vb_space(scrn);
	if (vb_bo == NULL)
		return;
	i = 0;
	/* rect (x2,y2) */
	vb[i++] = (float)(dstX + w);
	vb[i++] = (float)(dstY + h);
	vb[i++] = src_x[2] / intel->scale_units[0][0];
	vb[i++] = src_y[2] / intel->scale_units[0][1];
	if (!is_affine)
		vb[i++] = src_w[2];
	if (has_mask) {
		vb[i++] = mask_x[2] / intel->scale_units[1][0];
		vb[i++] = mask_y[2] / intel->scale_units[1][1];
		if (!is_affine)
			vb[i++] = mask_w[2];
	}

	/* rect (x1,y2) */
	vb[i++] = (float)dstX;
	vb[i++] = (float)(dstY + h);
	vb[i++] = src_x[1] / intel->scale_units[0][0];
	vb[i++] = src_y[1] / intel->scale_units[0][1];
	if (!is_affine)
		vb[i++] = src_w[1];
	if (has_mask) {
		vb[i++] = mask_x[1] / intel->scale_units[1][0];
		vb[i++] = mask_y[1] / intel->scale_units[1][1];
		if (!is_affine)
			vb[i++] = mask_w[1];
	}

	/* rect (x1,y1) */
	vb[i++] = (float)dstX;
	vb[i++] = (float)dstY;
	vb[i++] = src_x[0] / intel->scale_units[0][0];
	vb[i++] = src_y[0] / intel->scale_units[0][1];
	if (!is_affine)
		vb[i++] = src_w[0];
	if (has_mask) {
		vb[i++] = mask_x[0] / intel->scale_units[1][0];
		vb[i++] = mask_y[0] / intel->scale_units[1][1];
		if (!is_affine)
			vb[i++] = mask_w[0];
	}
	drm_intel_bo_subdata(vb_bo, render_state->vb_offset * 4, i * 4, vb);

	if (!i965_composite_check_aperture(scrn))
		intel_batch_submit(scrn, FALSE);

	intel_batch_start_atomic(scrn, 200);
	if (intel->needs_render_state_emit)
		i965_emit_composite_state(scrn);

	OUT_BATCH(MI_FLUSH);
	/* Set up the pointer to our (single) vertex buffer */
	OUT_BATCH(BRW_3DSTATE_VERTEX_BUFFERS | 3);
	OUT_BATCH((0 << VB0_BUFFER_INDEX_SHIFT) |
		  VB0_VERTEXDATA |
		  (render_state->vertex_size << VB0_BUFFER_PITCH_SHIFT));
	OUT_RELOC(vb_bo, I915_GEM_DOMAIN_VERTEX, 0,
		  render_state->vb_offset * 4);

	if (IS_IGDNG(intel))
		OUT_RELOC(vb_bo, I915_GEM_DOMAIN_VERTEX, 0,
			  render_state->vb_offset * 4 + i * 4);
	else
		OUT_BATCH(3);

	OUT_BATCH(0);		// ignore for VERTEXDATA, but still there

	OUT_BATCH(BRW_3DPRIMITIVE | BRW_3DPRIMITIVE_VERTEX_SEQUENTIAL | (_3DPRIM_RECTLIST << BRW_3DPRIMITIVE_TOPOLOGY_SHIFT) | (0 << 9) |	/* CTG - indirect vertex count */
		  4);
	OUT_BATCH(3);		/* vertex count per instance */
	OUT_BATCH(0);		/* start vertex offset */
	OUT_BATCH(1);		/* single instance */
	OUT_BATCH(0);		/* start instance location */
	OUT_BATCH(0);		/* index buffer offset, ignored */

	render_state->vb_offset += i;
	drm_intel_bo_unreference(vb_bo);

	intel_batch_end_atomic(scrn);
}

void i965_batch_flush_notify(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen4_render_state *render_state = intel->gen4_render_state;

	/* Once a batch is emitted, we never want to map again any buffer
	 * object being referenced by that batch, (which would be very
	 * expensive). */
	if (render_state->vertex_buffer_bo) {
		dri_bo_unreference(render_state->vertex_buffer_bo);
		render_state->vertex_buffer_bo = NULL;
	}

	intel->needs_render_state_emit = TRUE;
}

/**
 * Called at EnterVT so we can set up our offsets into the state buffer.
 */
void gen4_render_state_init(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen4_render_state *render_state;
	int i, j, k, l, m;
	drm_intel_bo *sf_kernel_bo, *sf_kernel_mask_bo;
	drm_intel_bo *border_color_bo;

	if (intel->gen4_render_state == NULL)
		intel->gen4_render_state = calloc(sizeof(*render_state), 1);

	render_state = intel->gen4_render_state;
	render_state->vb_offset = 0;

	render_state->vs_state_bo = gen4_create_vs_unit_state(scrn);

	/* Set up the two SF states (one for blending with a mask, one without) */
	if (IS_IGDNG(intel)) {
		sf_kernel_bo = intel_bo_alloc_for_data(scrn,
						       sf_kernel_static_gen5,
						       sizeof
						       (sf_kernel_static_gen5),
						       "sf kernel gen5");
		sf_kernel_mask_bo =
		    intel_bo_alloc_for_data(scrn, sf_kernel_mask_static_gen5,
					    sizeof(sf_kernel_mask_static_gen5),
					    "sf mask kernel");
	} else {
		sf_kernel_bo = intel_bo_alloc_for_data(scrn,
						       sf_kernel_static,
						       sizeof(sf_kernel_static),
						       "sf kernel");
		sf_kernel_mask_bo = intel_bo_alloc_for_data(scrn,
							    sf_kernel_mask_static,
							    sizeof
							    (sf_kernel_mask_static),
							    "sf mask kernel");
	}
	render_state->sf_state_bo = gen4_create_sf_state(scrn, sf_kernel_bo);
	render_state->sf_mask_state_bo = gen4_create_sf_state(scrn,
							      sf_kernel_mask_bo);
	drm_intel_bo_unreference(sf_kernel_bo);
	drm_intel_bo_unreference(sf_kernel_mask_bo);

	for (m = 0; m < WM_KERNEL_COUNT; m++) {
		if (IS_IGDNG(intel))
			render_state->wm_kernel_bo[m] =
			    intel_bo_alloc_for_data(scrn,
						    wm_kernels_gen5[m].data,
						    wm_kernels_gen5[m].size,
						    "WM kernel gen5");
		else
			render_state->wm_kernel_bo[m] =
			    intel_bo_alloc_for_data(scrn,
						    wm_kernels[m].data,
						    wm_kernels[m].size,
						    "WM kernel");
	}

	/* Set up the WM states: each filter/extend type for source and mask, per
	 * kernel.
	 */
	border_color_bo = sampler_border_color_create(scrn);
	for (i = 0; i < SAMPLER_STATE_FILTER_COUNT; i++) {
		for (j = 0; j < SAMPLER_STATE_EXTEND_COUNT; j++) {
			for (k = 0; k < SAMPLER_STATE_FILTER_COUNT; k++) {
				for (l = 0; l < SAMPLER_STATE_EXTEND_COUNT; l++) {
					drm_intel_bo *sampler_state_bo;

					sampler_state_bo =
					    gen4_create_sampler_state(scrn,
								      i, j,
								      k, l,
								      border_color_bo);

					for (m = 0; m < WM_KERNEL_COUNT; m++) {
						if (IS_IGDNG(intel))
							render_state->
							    wm_state_bo[m][i][j]
							    [k][l] =
							    gen4_create_wm_state
							    (scrn,
							     wm_kernels_gen5[m].
							     has_mask,
							     render_state->
							     wm_kernel_bo[m],
							     sampler_state_bo);
						else
							render_state->
							    wm_state_bo[m][i][j]
							    [k][l] =
							    gen4_create_wm_state
							    (scrn,
							     wm_kernels[m].
							     has_mask,
							     render_state->
							     wm_kernel_bo[m],
							     sampler_state_bo);
					}
					drm_intel_bo_unreference
					    (sampler_state_bo);
				}
			}
		}
	}
	drm_intel_bo_unreference(border_color_bo);

	render_state->cc_state_bo = gen4_create_cc_unit_state(scrn);
	render_state->sip_kernel_bo = intel_bo_alloc_for_data(scrn,
							      sip_kernel_static,
							      sizeof
							      (sip_kernel_static),
							      "sip kernel");
}

/**
 * Called at LeaveVT.
 */
void gen4_render_state_cleanup(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen4_render_state *render_state = intel->gen4_render_state;
	int i, j, k, l, m;
	gen4_composite_op *composite_op = &render_state->composite_op;

	drm_intel_bo_unreference(composite_op->binding_table_bo);
	drm_intel_bo_unreference(render_state->vertex_buffer_bo);

	drm_intel_bo_unreference(render_state->vs_state_bo);
	drm_intel_bo_unreference(render_state->sf_state_bo);
	drm_intel_bo_unreference(render_state->sf_mask_state_bo);

	for (i = 0; i < WM_KERNEL_COUNT; i++)
		drm_intel_bo_unreference(render_state->wm_kernel_bo[i]);

	for (i = 0; i < SAMPLER_STATE_FILTER_COUNT; i++)
		for (j = 0; j < SAMPLER_STATE_EXTEND_COUNT; j++)
			for (k = 0; k < SAMPLER_STATE_FILTER_COUNT; k++)
				for (l = 0; l < SAMPLER_STATE_EXTEND_COUNT; l++)
					for (m = 0; m < WM_KERNEL_COUNT; m++)
						drm_intel_bo_unreference
						    (render_state->
						     wm_state_bo[m][i][j][k]
						     [l]);

	drm_intel_bo_unreference(render_state->cc_state_bo);
	drm_intel_bo_unreference(render_state->sip_kernel_bo);

	free(intel->gen4_render_state);
	intel->gen4_render_state = NULL;
}
