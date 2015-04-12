/*
 * Copyright © 2012,2013 Intel Corporation
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
 *    Chris Wilson <chris@chris-wilson.co.uk>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sna.h"
#include "sna_reg.h"
#include "sna_render.h"
#include "sna_render_inline.h"
#include "sna_video.h"

#include "gen8_render.h"
#include "gen8_eu.h"
#include "gen4_common.h"
#include "gen4_source.h"
#include "gen4_vertex.h"
#include "gen6_common.h"
#include "gen8_vertex.h"

#define SIM 1

#define ALWAYS_INVALIDATE 0
#define ALWAYS_FLUSH 0
#define ALWAYS_STALL 0

#define NO_COMPOSITE 0
#define NO_COMPOSITE_SPANS 0
#define NO_COPY 0
#define NO_COPY_BOXES 0
#define NO_FILL 0
#define NO_FILL_BOXES 0
#define NO_FILL_ONE 0
#define NO_FILL_CLEAR 0
#define NO_VIDEO 0

#define USE_8_PIXEL_DISPATCH 1
#define USE_16_PIXEL_DISPATCH 1
#define USE_32_PIXEL_DISPATCH 0

#if !USE_8_PIXEL_DISPATCH && !USE_16_PIXEL_DISPATCH && !USE_32_PIXEL_DISPATCH
#error "Must select at least 8, 16 or 32 pixel dispatch"
#endif

#define GEN8_MAX_SIZE 16384

/* XXX Todo
 *
 * STR (software tiled rendering) mode. No, really.
 * 64x32 pixel blocks align with the rendering cache. Worth considering.
 */

#define is_aligned(x, y) (((x) & ((y) - 1)) == 0)

/* Pipeline stages:
 *  1. Command Streamer (CS)
 *  2. Vertex Fetch (VF)
 *  3. Vertex Shader (VS)
 *  4. Hull Shader (HS)
 *  5. Tesselation Engine (TE)
 *  6. Domain Shader (DS)
 *  7. Geometry Shader (GS)
 *  8. Stream Output Logic (SOL)
 *  9. Clipper (CLIP)
 * 10. Strip/Fan (SF)
 * 11. Windower/Masker (WM)
 * 12. Color Calculator (CC)
 */

#if !NO_VIDEO
static const uint32_t ps_kernel_packed[][4] = {
#include "exa_wm_src_affine.g8b"
#include "exa_wm_src_sample_argb.g8b"
#include "exa_wm_yuv_rgb.g8b"
#include "exa_wm_write.g8b"
};

static const uint32_t ps_kernel_planar[][4] = {
#include "exa_wm_src_affine.g8b"
#include "exa_wm_src_sample_planar.g8b"
#include "exa_wm_yuv_rgb.g8b"
#include "exa_wm_write.g8b"
};
#endif

#define SURFACE_DW (64 / sizeof(uint32_t));

#define KERNEL(kernel_enum, kernel, num_surfaces) \
    [GEN8_WM_KERNEL_##kernel_enum] = {#kernel_enum, kernel, sizeof(kernel), num_surfaces}
#define NOKERNEL(kernel_enum, func, num_surfaces) \
    [GEN8_WM_KERNEL_##kernel_enum] = {#kernel_enum, (void *)func, 0, num_surfaces}
static const struct wm_kernel_info {
	const char *name;
	const void *data;
	unsigned int size;
	int num_surfaces;
} wm_kernels[] = {
	NOKERNEL(NOMASK, gen8_wm_kernel__affine, 2),
	NOKERNEL(NOMASK_P, gen8_wm_kernel__projective, 2),

	NOKERNEL(MASK, gen8_wm_kernel__affine_mask, 3),
	NOKERNEL(MASK_P, gen8_wm_kernel__projective_mask, 3),

	NOKERNEL(MASKCA, gen8_wm_kernel__affine_mask_ca, 3),
	NOKERNEL(MASKCA_P, gen8_wm_kernel__projective_mask_ca, 3),

	NOKERNEL(MASKSA, gen8_wm_kernel__affine_mask_sa, 3),
	NOKERNEL(MASKSA_P, gen8_wm_kernel__projective_mask_sa, 3),

	NOKERNEL(OPACITY, gen8_wm_kernel__affine_opacity, 2),
	NOKERNEL(OPACITY_P, gen8_wm_kernel__projective_opacity, 2),

#if !NO_VIDEO
	KERNEL(VIDEO_PLANAR, ps_kernel_planar, 7),
	KERNEL(VIDEO_PACKED, ps_kernel_packed, 2),
#endif
};
#undef KERNEL

static const struct blendinfo {
	uint8_t src_alpha;
	uint8_t src_blend;
	uint8_t dst_blend;
} gen8_blend_op[] = {
	/* Clear */	{0, BLENDFACTOR_ZERO, BLENDFACTOR_ZERO},
	/* Src */	{0, BLENDFACTOR_ONE, BLENDFACTOR_ZERO},
	/* Dst */	{0, BLENDFACTOR_ZERO, BLENDFACTOR_ONE},
	/* Over */	{1, BLENDFACTOR_ONE, BLENDFACTOR_INV_SRC_ALPHA},
	/* OverReverse */ {0, BLENDFACTOR_INV_DST_ALPHA, BLENDFACTOR_ONE},
	/* In */	{0, BLENDFACTOR_DST_ALPHA, BLENDFACTOR_ZERO},
	/* InReverse */	{1, BLENDFACTOR_ZERO, BLENDFACTOR_SRC_ALPHA},
	/* Out */	{0, BLENDFACTOR_INV_DST_ALPHA, BLENDFACTOR_ZERO},
	/* OutReverse */ {1, BLENDFACTOR_ZERO, BLENDFACTOR_INV_SRC_ALPHA},
	/* Atop */	{1, BLENDFACTOR_DST_ALPHA, BLENDFACTOR_INV_SRC_ALPHA},
	/* AtopReverse */ {1, BLENDFACTOR_INV_DST_ALPHA, BLENDFACTOR_SRC_ALPHA},
	/* Xor */	{1, BLENDFACTOR_INV_DST_ALPHA, BLENDFACTOR_INV_SRC_ALPHA},
	/* Add */	{0, BLENDFACTOR_ONE, BLENDFACTOR_ONE},
};

/**
 * Highest-valued BLENDFACTOR used in gen8_blend_op.
 *
 * This leaves out GEN8_BLENDFACTOR_INV_DST_COLOR,
 * GEN8_BLENDFACTOR_INV_CONST_{COLOR,ALPHA},
 * GEN8_BLENDFACTOR_INV_SRC1_{COLOR,ALPHA}
 */
#define GEN8_BLENDFACTOR_COUNT (BLENDFACTOR_INV_DST_ALPHA + 1)

#define GEN8_BLEND_STATE_PADDED_SIZE	ALIGN(sizeof(struct gen8_blend_state), 64)

#define BLEND_OFFSET(s, d) \
	((d != BLENDFACTOR_ZERO) << 15 | ((s) * GEN8_BLENDFACTOR_COUNT + (d)) << 4)

#define NO_BLEND BLEND_OFFSET(BLENDFACTOR_ONE, BLENDFACTOR_ZERO)
#define CLEAR BLEND_OFFSET(BLENDFACTOR_ZERO, BLENDFACTOR_ZERO)

#define SAMPLER_OFFSET(sf, se, mf, me) \
	(((((sf) * EXTEND_COUNT + (se)) * FILTER_COUNT + (mf)) * EXTEND_COUNT + (me)) + 2)

#define VERTEX_2s2s 0

#define COPY_SAMPLER 0
#define COPY_VERTEX VERTEX_2s2s
#define COPY_FLAGS(a) GEN8_SET_FLAGS(COPY_SAMPLER, (a) == GXcopy ? NO_BLEND : CLEAR, GEN8_WM_KERNEL_NOMASK, COPY_VERTEX)

#define FILL_SAMPLER 1
#define FILL_VERTEX VERTEX_2s2s
#define FILL_FLAGS(op, format) GEN8_SET_FLAGS(FILL_SAMPLER, gen8_get_blend((op), false, (format)), GEN8_WM_KERNEL_NOMASK, FILL_VERTEX)
#define FILL_FLAGS_NOBLEND GEN8_SET_FLAGS(FILL_SAMPLER, NO_BLEND, GEN8_WM_KERNEL_NOMASK, FILL_VERTEX)

#define GEN8_SAMPLER(f) (((f) >> 20) & 0xfff)
#define GEN8_BLEND(f) (((f) >> 4) & 0x7ff)
#define GEN8_READS_DST(f) (((f) >> 15) & 1)
#define GEN8_KERNEL(f) (((f) >> 16) & 0xf)
#define GEN8_VERTEX(f) (((f) >> 0) & 0xf)
#define GEN8_SET_FLAGS(S, B, K, V)  ((S) << 20 | (K) << 16 | (B) | (V))

#define OUT_BATCH(v) batch_emit(sna, v)
#define OUT_BATCH64(v) batch_emit64(sna, v)
#define OUT_VERTEX(x,y) vertex_emit_2s(sna, x,y)
#define OUT_VERTEX_F(v) vertex_emit(sna, v)

static inline bool too_large(int width, int height)
{
	return width > GEN8_MAX_SIZE || height > GEN8_MAX_SIZE;
}

static inline bool unaligned(struct kgem_bo *bo, int bpp)
{
	/* XXX What exactly do we need to meet H_ALIGN and V_ALIGN? */
#if 0
	int x, y;

	if (bo->proxy == NULL)
		return false;

	/* Assume that all tiled proxies are constructed correctly. */
	if (bo->tiling)
		return false;

	DBG(("%s: checking alignment of a linear proxy, offset=%d, pitch=%d, bpp=%d: => (%d, %d)\n",
	     __FUNCTION__, bo->delta, bo->pitch, bpp,
	     8 * (bo->delta % bo->pitch) / bpp, bo->delta / bo->pitch));

	/* This may be a random userptr map, check that it meets the
	 * render alignment of SURFACE_VALIGN_4 | SURFACE_HALIGN_4.
	 */
	y = bo->delta / bo->pitch;
	if (y & 3)
		return true;

	x = 8 * (bo->delta - y * bo->pitch);
	if (x & (4*bpp - 1))
	    return true;

	return false;
#else
	return false;
#endif
}

static uint32_t gen8_get_blend(int op,
			       bool has_component_alpha,
			       uint32_t dst_format)
{
	uint32_t src, dst;

	COMPILE_TIME_ASSERT(BLENDFACTOR_INV_DST_ALPHA*GEN8_BLENDFACTOR_COUNT + BLENDFACTOR_INV_DST_ALPHA <= 0x7ff);

	src = gen8_blend_op[op].src_blend;
	dst = gen8_blend_op[op].dst_blend;

	/* If there's no dst alpha channel, adjust the blend op so that
	 * we'll treat it always as 1.
	 */
	if (PICT_FORMAT_A(dst_format) == 0) {
		if (src == BLENDFACTOR_DST_ALPHA)
			src = BLENDFACTOR_ONE;
		else if (src == BLENDFACTOR_INV_DST_ALPHA)
			src = BLENDFACTOR_ZERO;
	}

	/* If the source alpha is being used, then we should only be in a
	 * case where the source blend factor is 0, and the source blend
	 * value is the mask channels multiplied by the source picture's alpha.
	 */
	if (has_component_alpha && gen8_blend_op[op].src_alpha) {
		if (dst == BLENDFACTOR_SRC_ALPHA)
			dst = BLENDFACTOR_SRC_COLOR;
		else if (dst == BLENDFACTOR_INV_SRC_ALPHA)
			dst = BLENDFACTOR_INV_SRC_COLOR;
	}

	DBG(("blend op=%d, dst=%x [A=%d] => src=%d, dst=%d => offset=%x\n",
	     op, dst_format, PICT_FORMAT_A(dst_format),
	     src, dst, (int)(BLEND_OFFSET(src, dst)>>4)));
	assert(BLEND_OFFSET(src, dst) >> 4 <= 0xfff);
	return BLEND_OFFSET(src, dst);
}

static uint32_t gen8_get_card_format(PictFormat format)
{
	switch (format) {
	default:
		return -1;
	case PICT_a8r8g8b8:
		return SURFACEFORMAT_B8G8R8A8_UNORM;
	case PICT_x8r8g8b8:
		return SURFACEFORMAT_B8G8R8X8_UNORM;
	case PICT_a8b8g8r8:
		return SURFACEFORMAT_R8G8B8A8_UNORM;
	case PICT_x8b8g8r8:
		return SURFACEFORMAT_R8G8B8X8_UNORM;
#ifdef PICT_a2r10g10b10
	case PICT_a2r10g10b10:
		return SURFACEFORMAT_B10G10R10A2_UNORM;
	case PICT_x2r10g10b10:
		return SURFACEFORMAT_B10G10R10X2_UNORM;
#endif
	case PICT_r8g8b8:
		return SURFACEFORMAT_R8G8B8_UNORM;
	case PICT_r5g6b5:
		return SURFACEFORMAT_B5G6R5_UNORM;
	case PICT_a1r5g5b5:
		return SURFACEFORMAT_B5G5R5A1_UNORM;
	case PICT_a8:
		return SURFACEFORMAT_A8_UNORM;
	case PICT_a4r4g4b4:
		return SURFACEFORMAT_B4G4R4A4_UNORM;
	}
}

static uint32_t gen8_get_dest_format(PictFormat format)
{
	switch (format) {
	default:
		return -1;
	case PICT_a8r8g8b8:
	case PICT_x8r8g8b8:
		return SURFACEFORMAT_B8G8R8A8_UNORM;
	case PICT_a8b8g8r8:
	case PICT_x8b8g8r8:
		return SURFACEFORMAT_R8G8B8A8_UNORM;
#ifdef PICT_a2r10g10b10
	case PICT_a2r10g10b10:
	case PICT_x2r10g10b10:
		return SURFACEFORMAT_B10G10R10A2_UNORM;
#endif
	case PICT_r5g6b5:
		return SURFACEFORMAT_B5G6R5_UNORM;
	case PICT_x1r5g5b5:
	case PICT_a1r5g5b5:
		return SURFACEFORMAT_B5G5R5A1_UNORM;
	case PICT_a8:
		return SURFACEFORMAT_A8_UNORM;
	case PICT_a4r4g4b4:
	case PICT_x4r4g4b4:
		return SURFACEFORMAT_B4G4R4A4_UNORM;
	}
}

static bool gen8_check_dst_format(PictFormat format)
{
	if (gen8_get_dest_format(format) != -1)
		return true;

	DBG(("%s: unhandled format: %x\n", __FUNCTION__, (int)format));
	return false;
}

static bool gen8_check_format(uint32_t format)
{
	if (gen8_get_card_format(format) != -1)
		return true;

	DBG(("%s: unhandled format: %x\n", __FUNCTION__, (int)format));
	return false;
}

static uint32_t gen8_filter(uint32_t filter)
{
	switch (filter) {
	default:
		assert(0);
	case PictFilterNearest:
		return SAMPLER_FILTER_NEAREST;
	case PictFilterBilinear:
		return SAMPLER_FILTER_BILINEAR;
	}
}

static uint32_t gen8_check_filter(PicturePtr picture)
{
	switch (picture->filter) {
	case PictFilterNearest:
	case PictFilterBilinear:
		return true;
	default:
		return false;
	}
}

static uint32_t gen8_repeat(uint32_t repeat)
{
	switch (repeat) {
	default:
		assert(0);
	case RepeatNone:
		return SAMPLER_EXTEND_NONE;
	case RepeatNormal:
		return SAMPLER_EXTEND_REPEAT;
	case RepeatPad:
		return SAMPLER_EXTEND_PAD;
	case RepeatReflect:
		return SAMPLER_EXTEND_REFLECT;
	}
}

static bool gen8_check_repeat(PicturePtr picture)
{
	if (!picture->repeat)
		return true;

	switch (picture->repeatType) {
	case RepeatNone:
	case RepeatNormal:
	case RepeatPad:
	case RepeatReflect:
		return true;
	default:
		return false;
	}
}

static int
gen8_choose_composite_kernel(int op, bool has_mask, bool is_ca, bool is_affine)
{
	int base;

	if (has_mask) {
		if (is_ca) {
			if (gen8_blend_op[op].src_alpha)
				base = GEN8_WM_KERNEL_MASKSA;
			else
				base = GEN8_WM_KERNEL_MASKCA;
		} else
			base = GEN8_WM_KERNEL_MASK;
	} else
		base = GEN8_WM_KERNEL_NOMASK;

	return base + !is_affine;
}

static void
gen8_emit_push_constants(struct sna *sna)
{
#if SIM
	OUT_BATCH(GEN8_3DSTATE_PUSH_CONSTANT_ALLOC_VS | (2 - 2));
	OUT_BATCH(0);

	OUT_BATCH(GEN8_3DSTATE_PUSH_CONSTANT_ALLOC_HS | (2 - 2));
	OUT_BATCH(0);

	OUT_BATCH(GEN8_3DSTATE_PUSH_CONSTANT_ALLOC_DS | (2 - 2));
	OUT_BATCH(0);

	OUT_BATCH(GEN8_3DSTATE_PUSH_CONSTANT_ALLOC_GS | (2 - 2));
	OUT_BATCH(0);

	OUT_BATCH(GEN8_3DSTATE_PUSH_CONSTANT_ALLOC_PS | (2 - 2));
	OUT_BATCH(0);
#endif
}

static void
gen8_emit_urb(struct sna *sna)
{
	/* num of VS entries must be divisible by 8 if size < 9 */
	OUT_BATCH(GEN8_3DSTATE_URB_VS | (2 - 2));
	OUT_BATCH(1024 << URB_ENTRY_NUMBER_SHIFT |
		  (2 - 1) << URB_ENTRY_SIZE_SHIFT |
		  0 << URB_STARTING_ADDRESS_SHIFT);

	OUT_BATCH(GEN8_3DSTATE_URB_HS | (2 - 2));
	OUT_BATCH(0 << URB_ENTRY_SIZE_SHIFT |
		  0 << URB_STARTING_ADDRESS_SHIFT);

	OUT_BATCH(GEN8_3DSTATE_URB_DS | (2 - 2));
	OUT_BATCH(0 << URB_ENTRY_SIZE_SHIFT |
		  0 << URB_STARTING_ADDRESS_SHIFT);

	OUT_BATCH(GEN8_3DSTATE_URB_GS | (2 - 2));
	OUT_BATCH(0 << URB_ENTRY_SIZE_SHIFT |
		  0 << URB_STARTING_ADDRESS_SHIFT);
}

static void
gen8_emit_state_base_address(struct sna *sna)
{
	uint32_t num_pages;

	assert(sna->kgem.surface - sna->kgem.nbatch <= 16384);

	OUT_BATCH(GEN8_STATE_BASE_ADDRESS | (16 - 2));
	OUT_BATCH64(0); /* general */
	OUT_BATCH(0); /* stateless dataport */
	OUT_BATCH64(kgem_add_reloc64(&sna->kgem, /* surface */
				     sna->kgem.nbatch,
				     NULL,
				     I915_GEM_DOMAIN_INSTRUCTION << 16,
				     BASE_ADDRESS_MODIFY));
	OUT_BATCH64(kgem_add_reloc64(&sna->kgem, /* dynamic */
				     sna->kgem.nbatch,
				     sna->render_state.gen8.general_bo,
				     I915_GEM_DOMAIN_INSTRUCTION << 16,
				     BASE_ADDRESS_MODIFY));
	OUT_BATCH64(0); /* indirect */
	OUT_BATCH64(kgem_add_reloc64(&sna->kgem, /* instruction */
				     sna->kgem.nbatch,
				     sna->render_state.gen8.general_bo,
				     I915_GEM_DOMAIN_INSTRUCTION << 16,
				     BASE_ADDRESS_MODIFY));
	/* upper bounds */
	num_pages = sna->render_state.gen8.general_bo->size.pages.count;
	OUT_BATCH(0); /* general */
	OUT_BATCH(num_pages << 12 | 1); /* dynamic */
	OUT_BATCH(0); /* indirect */
	OUT_BATCH(num_pages << 12 | 1); /* instruction */
}

static void
gen8_emit_vs_invariant(struct sna *sna)
{
	OUT_BATCH(GEN8_3DSTATE_VS | (9 - 2));
	OUT_BATCH64(0); /* no VS kernel */
	OUT_BATCH(0);
	OUT_BATCH64(0);
	OUT_BATCH(0);
	OUT_BATCH(1 << 1); /* pass-through */
	OUT_BATCH(1 << 16 | 1 << 21); /* urb write to SBE */

#if SIM
	OUT_BATCH(GEN8_3DSTATE_CONSTANT_VS | (11 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);

	OUT_BATCH(GEN8_3DSTATE_BINDING_TABLE_POINTERS_VS | (2 - 2));
	OUT_BATCH(0);

	OUT_BATCH(GEN8_3DSTATE_SAMPLER_STATE_POINTERS_VS | (2 - 2));
	OUT_BATCH(0);
#endif
}

static void
gen8_emit_hs_invariant(struct sna *sna)
{
	OUT_BATCH(GEN8_3DSTATE_HS | (9 - 2));
	OUT_BATCH(0); /* no HS kernel */
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0); /* pass-through */

#if SIM
	OUT_BATCH(GEN8_3DSTATE_CONSTANT_HS | (11 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);

#if 1
	OUT_BATCH(GEN8_3DSTATE_BINDING_TABLE_POINTERS_HS | (2 - 2));
	OUT_BATCH(0);

	OUT_BATCH(GEN8_3DSTATE_SAMPLER_STATE_POINTERS_HS | (2 - 2));
	OUT_BATCH(0);
#endif
#endif
}

static void
gen8_emit_te_invariant(struct sna *sna)
{
	OUT_BATCH(GEN8_3DSTATE_TE | (4 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
}

static void
gen8_emit_ds_invariant(struct sna *sna)
{
	OUT_BATCH(GEN8_3DSTATE_DS | (9 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);

#if SIM
	OUT_BATCH(GEN8_3DSTATE_CONSTANT_DS | (11 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);

#if 1
	OUT_BATCH(GEN8_3DSTATE_BINDING_TABLE_POINTERS_DS | (2 - 2));
	OUT_BATCH(0);

	OUT_BATCH(GEN8_3DSTATE_SAMPLER_STATE_POINTERS_DS | (2 - 2));
	OUT_BATCH(0);
#endif
#endif
}

static void
gen8_emit_gs_invariant(struct sna *sna)
{
	OUT_BATCH(GEN8_3DSTATE_GS | (10 - 2));
	OUT_BATCH(0); /* no GS kernel */
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0); /* pass-through */

#if SIM
	OUT_BATCH(GEN8_3DSTATE_CONSTANT_GS | (11 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);

#if 1
	OUT_BATCH(GEN8_3DSTATE_BINDING_TABLE_POINTERS_GS | (2 - 2));
	OUT_BATCH(0);

	OUT_BATCH(GEN8_3DSTATE_SAMPLER_STATE_POINTERS_GS | (2 - 2));
	OUT_BATCH(0);
#endif
#endif
}

static void
gen8_emit_sol_invariant(struct sna *sna)
{
	OUT_BATCH(GEN8_3DSTATE_STREAMOUT | (5 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
}

static void
gen8_emit_sf_invariant(struct sna *sna)
{
	OUT_BATCH(GEN8_3DSTATE_SF | (4 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
}

static void
gen8_emit_clip_invariant(struct sna *sna)
{
	OUT_BATCH(GEN8_3DSTATE_CLIP | (4 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0); /* pass-through */
	OUT_BATCH(0);

	OUT_BATCH(GEN8_3DSTATE_VIEWPORT_STATE_POINTERS_SF_CLIP | (2 - 2));
	OUT_BATCH(0);

	OUT_BATCH(GEN8_3DSTATE_VIEWPORT_STATE_POINTERS_CC | (2 - 2));
	OUT_BATCH(0);
}

static void
gen8_emit_null_depth_buffer(struct sna *sna)
{
	OUT_BATCH(GEN8_3DSTATE_DEPTH_BUFFER | (8 - 2));
#if 0
	OUT_BATCH(SURFACE_NULL << DEPTH_BUFFER_TYPE_SHIFT |
		  DEPTHFORMAT_D32_FLOAT << DEPTH_BUFFER_FORMAT_SHIFT);
#else
	OUT_BATCH(SURFACE_2D << DEPTH_BUFFER_TYPE_SHIFT |
		  DEPTHFORMAT_D16_UNORM << DEPTH_BUFFER_FORMAT_SHIFT);
#endif
	OUT_BATCH64(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);

#if SIM
	OUT_BATCH(GEN8_3DSTATE_HIER_DEPTH_BUFFER | (5 - 2));
	OUT_BATCH(0);
	OUT_BATCH64(0);
	OUT_BATCH(0);
#endif

#if SIM
	OUT_BATCH(GEN8_3DSTATE_STENCIL_BUFFER | (5 - 2));
	OUT_BATCH(0);
	OUT_BATCH64(0);
	OUT_BATCH(0);
#endif

#if SIM
	OUT_BATCH(GEN8_3DSTATE_WM_DEPTH_STENCIL | (3 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
#endif

#if SIM
	OUT_BATCH(GEN8_3DSTATE_CLEAR_PARAMS | (3 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
#endif
}

static void
gen8_emit_wm_invariant(struct sna *sna)
{
	gen8_emit_null_depth_buffer(sna);

#if SIM
	OUT_BATCH(GEN8_3DSTATE_SCISSOR_STATE_POINTERS | (2 - 2));
	OUT_BATCH(0);
#endif

	OUT_BATCH(GEN8_3DSTATE_WM | (2 - 2));
	//OUT_BATCH(WM_NONPERSPECTIVE_PIXEL_BARYCENTRIC); /* XXX */
	OUT_BATCH(WM_PERSPECTIVE_PIXEL_BARYCENTRIC);

#if SIM
	OUT_BATCH(GEN8_3DSTATE_WM_HZ_OP | (5 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);

	OUT_BATCH(GEN8_3DSTATE_WM_CHROMAKEY | (2 - 2));
	OUT_BATCH(0);
#endif

	OUT_BATCH(GEN8_3DSTATE_PS_EXTRA | (2 - 2));
	OUT_BATCH(PSX_PIXEL_SHADER_VALID |
		  PSX_ATTRIBUTE_ENABLE);

	OUT_BATCH(GEN8_3DSTATE_RASTER | (5 - 2));
	OUT_BATCH(RASTER_FRONT_WINDING_CCW |
		  RASTER_CULL_NONE);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);

	OUT_BATCH(GEN8_3DSTATE_SBE_SWIZ | (11 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);

#if SIM
	OUT_BATCH(GEN8_3DSTATE_CONSTANT_PS | (11 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
#endif
}

static void
gen8_emit_cc_invariant(struct sna *sna)
{
}

static void
gen8_emit_vf_invariant(struct sna *sna)
{
#if 1
	OUT_BATCH(GEN8_3DSTATE_VF | (2 - 2));
	OUT_BATCH(0);
#endif

	OUT_BATCH(GEN8_3DSTATE_VF_SGVS | (2 - 2));
	OUT_BATCH(0);

	OUT_BATCH(GEN8_3DSTATE_VF_TOPOLOGY | (2 - 2));
	OUT_BATCH(RECTLIST);

	OUT_BATCH(GEN8_3DSTATE_VF_STATISTICS | 0);
}

static void
gen8_emit_invariant(struct sna *sna)
{
	OUT_BATCH(GEN8_PIPELINE_SELECT | PIPELINE_SELECT_3D);

#if SIM
	OUT_BATCH(GEN8_STATE_SIP | (3 - 2));
	OUT_BATCH64(0);

#endif

	OUT_BATCH(GEN8_3DSTATE_MULTISAMPLE | (2 - 2));
	OUT_BATCH(MULTISAMPLE_PIXEL_LOCATION_CENTER |
		  MULTISAMPLE_NUMSAMPLES_1); /* 1 sample/pixel */

	OUT_BATCH(GEN8_3DSTATE_SAMPLE_MASK | (2 - 2));
	OUT_BATCH(1);

#if SIM
	OUT_BATCH(GEN8_3DSTATE_SAMPLE_PATTERN | (5 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	//OUT_BATCH(8<<20 | 8<<16);
	OUT_BATCH(0);
#endif

	gen8_emit_push_constants(sna);
	gen8_emit_urb(sna);

	gen8_emit_state_base_address(sna);

	gen8_emit_vf_invariant(sna);
	gen8_emit_vs_invariant(sna);
	gen8_emit_hs_invariant(sna);
	gen8_emit_te_invariant(sna);
	gen8_emit_ds_invariant(sna);
	gen8_emit_gs_invariant(sna);
	gen8_emit_sol_invariant(sna);
	gen8_emit_clip_invariant(sna);
	gen8_emit_sf_invariant(sna);
	gen8_emit_wm_invariant(sna);
	gen8_emit_cc_invariant(sna);

	sna->render_state.gen8.needs_invariant = false;
}

static void
gen8_emit_cc(struct sna *sna, uint32_t blend)
{
	struct gen8_render_state *render = &sna->render_state.gen8;

	if (render->blend == blend)
		return;

	DBG(("%s: blend=%x (current=%x), src=%d, dst=%d\n",
	     __FUNCTION__, blend, render->blend,
	     blend / GEN8_BLENDFACTOR_COUNT,
	     blend % GEN8_BLENDFACTOR_COUNT));

	assert(blend < GEN8_BLENDFACTOR_COUNT * GEN8_BLENDFACTOR_COUNT);
	assert(blend / GEN8_BLENDFACTOR_COUNT > 0);
	assert(blend % GEN8_BLENDFACTOR_COUNT > 0);

	/* XXX can have upto 8 blend states preload, selectable via
	 * Render Target Index. What other side-effects of Render Target Index?
	 */

	OUT_BATCH(GEN8_3DSTATE_PS_BLEND | (2 - 2));
	if (blend != GEN8_BLEND(NO_BLEND)) {
		uint32_t src = blend / GEN8_BLENDFACTOR_COUNT;
		uint32_t dst = blend % GEN8_BLENDFACTOR_COUNT;
		OUT_BATCH(PS_BLEND_HAS_WRITEABLE_RT |
			  PS_BLEND_COLOR_BLEND_ENABLE |
			  src << PS_BLEND_SRC_ALPHA_SHIFT |
			  dst << PS_BLEND_DST_ALPHA_SHIFT |
			  src << PS_BLEND_SRC_SHIFT |
			  dst << PS_BLEND_DST_SHIFT);
	} else
		OUT_BATCH(PS_BLEND_HAS_WRITEABLE_RT);

	OUT_BATCH(GEN8_3DSTATE_BLEND_STATE_POINTERS | (2 - 2));
	OUT_BATCH((render->cc_blend + blend * GEN8_BLEND_STATE_PADDED_SIZE) | 1);

	/* Force a CC_STATE pointer change to improve blend performance */
	OUT_BATCH(GEN8_3DSTATE_CC_STATE_POINTERS | (2 - 2));
	OUT_BATCH(0);

	render->blend = blend;
}

static void
gen8_emit_sampler(struct sna *sna, uint32_t state)
{
	if (sna->render_state.gen8.samplers == state)
		return;

	sna->render_state.gen8.samplers = state;

	DBG(("%s: sampler = %x\n", __FUNCTION__, state));

	assert(2 * sizeof(struct gen8_sampler_state) == 32);
	OUT_BATCH(GEN8_3DSTATE_SAMPLER_STATE_POINTERS_PS | (2 - 2));
	OUT_BATCH(sna->render_state.gen8.wm_state + state * 2 * sizeof(struct gen8_sampler_state));
}

static void
gen8_emit_sf(struct sna *sna, bool has_mask)
{
	int num_sf_outputs = has_mask ? 2 : 1;

	if (sna->render_state.gen8.num_sf_outputs == num_sf_outputs)
		return;

	DBG(("%s: num_sf_outputs=%d\n", __FUNCTION__, num_sf_outputs));

	sna->render_state.gen8.num_sf_outputs = num_sf_outputs;

	OUT_BATCH(GEN8_3DSTATE_SBE | (4 - 2));
	OUT_BATCH(num_sf_outputs << SBE_NUM_OUTPUTS_SHIFT |
		  1 << SBE_URB_ENTRY_READ_LENGTH_SHIFT |
		  1 << SBE_URB_ENTRY_READ_OFFSET_SHIFT);
	OUT_BATCH(0);
	OUT_BATCH(0);
}

static void
gen8_emit_wm(struct sna *sna, int kernel)
{
	const uint32_t *kernels;

	assert(kernel < ARRAY_SIZE(wm_kernels));
	if (sna->render_state.gen8.kernel == kernel)
		return;

	sna->render_state.gen8.kernel = kernel;
	kernels = sna->render_state.gen8.wm_kernel[kernel];

	DBG(("%s: switching to %s, num_surfaces=%d (8-wide? %d, 16-wide? %d, 32-wide? %d)\n",
	     __FUNCTION__,
	     wm_kernels[kernel].name,
	     wm_kernels[kernel].num_surfaces,
	     kernels[0], kernels[1], kernels[2]));

	OUT_BATCH(GEN8_3DSTATE_PS | (12 - 2));
	OUT_BATCH64(kernels[0] ?: kernels[1] ?: kernels[2]);
	OUT_BATCH(1 << PS_SAMPLER_COUNT_SHIFT |
		  //PS_VECTOR_MASK_ENABLE |
		  wm_kernels[kernel].num_surfaces << PS_BINDING_TABLE_ENTRY_COUNT_SHIFT);
	OUT_BATCH64(0); /* scratch address */
	OUT_BATCH(PS_MAX_THREADS |
		  (kernels[0] ? PS_8_DISPATCH_ENABLE : 0) |
		  (kernels[1] ? PS_16_DISPATCH_ENABLE : 0) |
		  (kernels[2] ? PS_32_DISPATCH_ENABLE : 0));
	OUT_BATCH((kernels[0] ? 4 : kernels[1] ? 6 : 8) << PS_DISPATCH_START_GRF_SHIFT_0 |
		  8 << PS_DISPATCH_START_GRF_SHIFT_1 |
		  6 << PS_DISPATCH_START_GRF_SHIFT_2);
	OUT_BATCH64(kernels[2]);
	OUT_BATCH64(kernels[1]);
}

static bool
gen8_emit_binding_table(struct sna *sna, uint16_t offset)
{
	if (sna->render_state.gen8.surface_table == offset)
		return false;

	/* Binding table pointers */
	assert(is_aligned(4*offset, 32));
	OUT_BATCH(GEN8_3DSTATE_BINDING_TABLE_POINTERS_PS | (2 - 2));
	OUT_BATCH(offset*4);

	sna->render_state.gen8.surface_table = offset;
	return true;
}

static bool
gen8_emit_drawing_rectangle(struct sna *sna,
			    const struct sna_composite_op *op)
{
	uint32_t limit = (op->dst.height - 1) << 16 | (op->dst.width - 1);
	uint32_t offset = (uint16_t)op->dst.y << 16 | (uint16_t)op->dst.x;

	assert(!too_large(op->dst.x, op->dst.y));
	assert(!too_large(op->dst.width, op->dst.height));

	if (sna->render_state.gen8.drawrect_limit == limit &&
	    sna->render_state.gen8.drawrect_offset == offset)
		return true;

	sna->render_state.gen8.drawrect_offset = offset;
	sna->render_state.gen8.drawrect_limit = limit;

	OUT_BATCH(GEN8_3DSTATE_DRAWING_RECTANGLE | (4 - 2));
	OUT_BATCH(0);
	OUT_BATCH(limit);
	OUT_BATCH(offset);
	return false;
}

static void
gen8_emit_vertex_elements(struct sna *sna,
			  const struct sna_composite_op *op)
{
	/*
	 * vertex data in vertex buffer
	 *    position: (x, y)
	 *    texture coordinate 0: (u0, v0) if (is_affine is true) else (u0, v0, w0)
	 *    texture coordinate 1 if (has_mask is true): same as above
	 */
	struct gen8_render_state *render = &sna->render_state.gen8;
	uint32_t src_format, dw;
	int id = GEN8_VERTEX(op->u.gen8.flags);
	bool has_mask;

	DBG(("%s: setup id=%d\n", __FUNCTION__, id));

	if (render->ve_id == id)
		return;
	render->ve_id = id;

	/* The VUE layout
	 *    dword 0-3: pad (0.0, 0.0, 0.0. 0.0)
	 *    dword 4-7: position (x, y, 1.0, 1.0),
	 *    dword 8-11: texture coordinate 0 (u0, v0, w0, 1.0)
	 *    dword 12-15: texture coordinate 1 (u1, v1, w1, 1.0)
	 *
	 * dword 4-15 are fetched from vertex buffer
	 */
	has_mask = (id >> 2) != 0;
	OUT_BATCH(GEN8_3DSTATE_VERTEX_ELEMENTS |
		((2 * (3 + has_mask)) + 1 - 2));

	OUT_BATCH(id << VE_INDEX_SHIFT | VE_VALID |
		  SURFACEFORMAT_R32G32B32A32_FLOAT << VE_FORMAT_SHIFT |
		  0 << VE_OFFSET_SHIFT);
	OUT_BATCH(COMPONENT_STORE_0 << VE_COMPONENT_0_SHIFT |
		  COMPONENT_STORE_0 << VE_COMPONENT_1_SHIFT |
		  COMPONENT_STORE_0 << VE_COMPONENT_2_SHIFT |
		  COMPONENT_STORE_0 << VE_COMPONENT_3_SHIFT);

	/* x,y */
	OUT_BATCH(id << VE_INDEX_SHIFT | VE_VALID |
		  SURFACEFORMAT_R16G16_SSCALED << VE_FORMAT_SHIFT |
		  0 << VE_OFFSET_SHIFT);
	OUT_BATCH(COMPONENT_STORE_SRC << VE_COMPONENT_0_SHIFT |
		  COMPONENT_STORE_SRC << VE_COMPONENT_1_SHIFT |
		  COMPONENT_STORE_0 << VE_COMPONENT_2_SHIFT |
		  COMPONENT_STORE_1_FLT << VE_COMPONENT_3_SHIFT);

	/* u0, v0, w0 */
	DBG(("%s: first channel %d floats, offset=4\n", __FUNCTION__, id & 3));
	dw = COMPONENT_STORE_1_FLT << VE_COMPONENT_3_SHIFT;
	switch (id & 3) {
	default:
		assert(0);
	case 0:
		src_format = SURFACEFORMAT_R16G16_SSCALED;
		dw |= COMPONENT_STORE_SRC << VE_COMPONENT_0_SHIFT;
		dw |= COMPONENT_STORE_SRC << VE_COMPONENT_1_SHIFT;
		dw |= COMPONENT_STORE_0 << VE_COMPONENT_2_SHIFT;
		break;
	case 1:
		src_format = SURFACEFORMAT_R32_FLOAT;
		dw |= COMPONENT_STORE_SRC << VE_COMPONENT_0_SHIFT;
		dw |= COMPONENT_STORE_0 << VE_COMPONENT_1_SHIFT;
		dw |= COMPONENT_STORE_0 << VE_COMPONENT_2_SHIFT;
		break;
	case 2:
		src_format = SURFACEFORMAT_R32G32_FLOAT;
		dw |= COMPONENT_STORE_SRC << VE_COMPONENT_0_SHIFT;
		dw |= COMPONENT_STORE_SRC << VE_COMPONENT_1_SHIFT;
		dw |= COMPONENT_STORE_0 << VE_COMPONENT_2_SHIFT;
		break;
	case 3:
		src_format = SURFACEFORMAT_R32G32B32_FLOAT;
		dw |= COMPONENT_STORE_SRC << VE_COMPONENT_0_SHIFT;
		dw |= COMPONENT_STORE_SRC << VE_COMPONENT_1_SHIFT;
		dw |= COMPONENT_STORE_SRC << VE_COMPONENT_2_SHIFT;
		break;
	}
	OUT_BATCH(id << VE_INDEX_SHIFT | VE_VALID |
		  src_format << VE_FORMAT_SHIFT |
		  4 << VE_OFFSET_SHIFT);
	OUT_BATCH(dw);

	/* u1, v1, w1 */
	if (has_mask) {
		unsigned offset = 4 + ((id & 3) ?: 1) * sizeof(float);
		DBG(("%s: second channel %d floats, offset=%d\n", __FUNCTION__, (id >> 2) & 3, offset));
		dw = COMPONENT_STORE_1_FLT << VE_COMPONENT_3_SHIFT;
		switch (id >> 2) {
		case 1:
			src_format = SURFACEFORMAT_R32_FLOAT;
			dw |= COMPONENT_STORE_SRC << VE_COMPONENT_0_SHIFT;
			dw |= COMPONENT_STORE_0 << VE_COMPONENT_1_SHIFT;
			dw |= COMPONENT_STORE_0 << VE_COMPONENT_2_SHIFT;
			break;
		default:
			assert(0);
		case 2:
			src_format = SURFACEFORMAT_R32G32_FLOAT;
			dw |= COMPONENT_STORE_SRC << VE_COMPONENT_0_SHIFT;
			dw |= COMPONENT_STORE_SRC << VE_COMPONENT_1_SHIFT;
			dw |= COMPONENT_STORE_0 << VE_COMPONENT_2_SHIFT;
			break;
		case 3:
			src_format = SURFACEFORMAT_R32G32B32_FLOAT;
			dw |= COMPONENT_STORE_SRC << VE_COMPONENT_0_SHIFT;
			dw |= COMPONENT_STORE_SRC << VE_COMPONENT_1_SHIFT;
			dw |= COMPONENT_STORE_SRC << VE_COMPONENT_2_SHIFT;
			break;
		}
		OUT_BATCH(id << VE_INDEX_SHIFT | VE_VALID |
			  src_format << VE_FORMAT_SHIFT |
			  offset << VE_OFFSET_SHIFT);
		OUT_BATCH(dw);
	}
}

inline static void
gen8_emit_pipe_invalidate(struct sna *sna)
{
	OUT_BATCH(GEN8_PIPE_CONTROL | (6 - 2));
	OUT_BATCH(PIPE_CONTROL_WC_FLUSH |
		  PIPE_CONTROL_TC_FLUSH |
		  PIPE_CONTROL_CS_STALL);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
}

inline static void
gen8_emit_pipe_flush(struct sna *sna, bool need_stall)
{
	unsigned stall;

	stall = 0;
	if (need_stall)
		stall = (PIPE_CONTROL_CS_STALL |
			 PIPE_CONTROL_STALL_AT_SCOREBOARD);

	OUT_BATCH(GEN8_PIPE_CONTROL | (6 - 2));
	OUT_BATCH(PIPE_CONTROL_WC_FLUSH | stall);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
}

inline static void
gen8_emit_pipe_stall(struct sna *sna)
{
	OUT_BATCH(GEN8_PIPE_CONTROL | (6 - 2));
	OUT_BATCH(PIPE_CONTROL_CS_STALL |
		  PIPE_CONTROL_STALL_AT_SCOREBOARD);
	OUT_BATCH64(0);
	OUT_BATCH64(0);
}

static void
gen8_emit_state(struct sna *sna,
		const struct sna_composite_op *op,
		uint16_t wm_binding_table)
{
	bool need_invalidate;
	bool need_flush;
	bool need_stall;

	assert(op->dst.bo->exec);

	need_flush = wm_binding_table & 1 ||
		(sna->render_state.gen8.emit_flush && GEN8_READS_DST(op->u.gen8.flags));
	if (ALWAYS_FLUSH)
		need_flush = true;

	wm_binding_table &= ~1;

	need_stall = sna->render_state.gen8.surface_table != wm_binding_table;

	need_invalidate = kgem_bo_is_dirty(op->src.bo) || kgem_bo_is_dirty(op->mask.bo);
	if (ALWAYS_INVALIDATE)
		need_invalidate = true;

	need_stall &= gen8_emit_drawing_rectangle(sna, op);
	if (ALWAYS_STALL)
		need_stall = true;

	if (need_invalidate) {
		gen8_emit_pipe_invalidate(sna);
		kgem_clear_dirty(&sna->kgem);
		assert(op->dst.bo->exec);
		kgem_bo_mark_dirty(op->dst.bo);

		need_flush = false;
		need_stall = false;
	}
	if (need_flush) {
		gen8_emit_pipe_flush(sna, need_stall);
		need_stall = false;
	}
	if (need_stall)
		gen8_emit_pipe_stall(sna);

	gen8_emit_cc(sna, GEN8_BLEND(op->u.gen8.flags));
	gen8_emit_sampler(sna, GEN8_SAMPLER(op->u.gen8.flags));
	gen8_emit_sf(sna, GEN8_VERTEX(op->u.gen8.flags) >> 2);
	gen8_emit_wm(sna, GEN8_KERNEL(op->u.gen8.flags));
	gen8_emit_vertex_elements(sna, op);
	gen8_emit_binding_table(sna, wm_binding_table);

	sna->render_state.gen8.emit_flush = GEN8_READS_DST(op->u.gen8.flags);
}

static bool gen8_magic_ca_pass(struct sna *sna,
			       const struct sna_composite_op *op)
{
	struct gen8_render_state *state = &sna->render_state.gen8;

	if (!op->need_magic_ca_pass)
		return false;

	DBG(("%s: CA fixup (%d -> %d)\n", __FUNCTION__,
	     sna->render.vertex_start, sna->render.vertex_index));

	gen8_emit_pipe_stall(sna);

	gen8_emit_cc(sna,
		     GEN8_BLEND(gen8_get_blend(PictOpAdd, true,
					       op->dst.format)));
	gen8_emit_wm(sna,
		     gen8_choose_composite_kernel(PictOpAdd,
						  true, true,
						  op->is_affine));

	OUT_BATCH(GEN8_3DPRIMITIVE | (7- 2));
	OUT_BATCH(RECTLIST); /* ignored, see VF_TOPOLOGY */
	OUT_BATCH(sna->render.vertex_index - sna->render.vertex_start);
	OUT_BATCH(sna->render.vertex_start);
	OUT_BATCH(1);	/* single instance */
	OUT_BATCH(0);	/* start instance location */
	OUT_BATCH(0);	/* index buffer offset, ignored */

	state->last_primitive = sna->kgem.nbatch;
	return true;
}

static void null_create(struct sna_static_stream *stream)
{
	/* A bunch of zeros useful for legacy border color and depth-stencil */
	sna_static_stream_map(stream, 64, 64);
}

static void
sampler_state_init(struct gen8_sampler_state *sampler_state,
		   sampler_filter_t filter,
		   sampler_extend_t extend)
{
	COMPILE_TIME_ASSERT(sizeof(*sampler_state) == 4*sizeof(uint32_t));

	sampler_state->ss0.lod_preclamp = 2;	/* GL mode */
	sampler_state->ss0.default_color_mode = 1;

	switch (filter) {
	default:
	case SAMPLER_FILTER_NEAREST:
		sampler_state->ss0.min_filter = MAPFILTER_NEAREST;
		sampler_state->ss0.mag_filter = MAPFILTER_NEAREST;
		break;
	case SAMPLER_FILTER_BILINEAR:
		sampler_state->ss0.min_filter = MAPFILTER_LINEAR;
		sampler_state->ss0.mag_filter = MAPFILTER_LINEAR;
		break;
	}

	/* XXX bicubic filter using MAPFILTER_FLEXIBLE */

	switch (extend) {
	default:
	case SAMPLER_EXTEND_NONE:
		sampler_state->ss3.r_wrap_mode = TEXCOORDMODE_CLAMP_BORDER;
		sampler_state->ss3.s_wrap_mode = TEXCOORDMODE_CLAMP_BORDER;
		sampler_state->ss3.t_wrap_mode = TEXCOORDMODE_CLAMP_BORDER;
		break;
	case SAMPLER_EXTEND_REPEAT:
		sampler_state->ss3.r_wrap_mode = TEXCOORDMODE_WRAP;
		sampler_state->ss3.s_wrap_mode = TEXCOORDMODE_WRAP;
		sampler_state->ss3.t_wrap_mode = TEXCOORDMODE_WRAP;
		break;
	case SAMPLER_EXTEND_PAD:
		sampler_state->ss3.r_wrap_mode = TEXCOORDMODE_CLAMP;
		sampler_state->ss3.s_wrap_mode = TEXCOORDMODE_CLAMP;
		sampler_state->ss3.t_wrap_mode = TEXCOORDMODE_CLAMP;
		break;
	case SAMPLER_EXTEND_REFLECT:
		sampler_state->ss3.r_wrap_mode = TEXCOORDMODE_MIRROR;
		sampler_state->ss3.s_wrap_mode = TEXCOORDMODE_MIRROR;
		sampler_state->ss3.t_wrap_mode = TEXCOORDMODE_MIRROR;
		break;
	}
}

static void
sampler_copy_init(struct gen8_sampler_state *ss)
{
	sampler_state_init(ss, SAMPLER_FILTER_NEAREST, SAMPLER_EXTEND_NONE);
	ss->ss3.non_normalized_coord = 1;

	sampler_state_init(ss+1, SAMPLER_FILTER_NEAREST, SAMPLER_EXTEND_NONE);
}

static void
sampler_fill_init(struct gen8_sampler_state *ss)
{
	sampler_state_init(ss, SAMPLER_FILTER_NEAREST, SAMPLER_EXTEND_REPEAT);
	ss->ss3.non_normalized_coord = 1;

	sampler_state_init(ss+1, SAMPLER_FILTER_NEAREST, SAMPLER_EXTEND_NONE);
}

static uint32_t
gen8_tiling_bits(uint32_t tiling)
{
	switch (tiling) {
	default: assert(0);
	case I915_TILING_NONE: return 0;
	case I915_TILING_X: return SURFACE_TILED;
	case I915_TILING_Y: return SURFACE_TILED | SURFACE_TILED_Y;
	}
}

#define MOCS_WT (2 << 5)
#define MOCS_WB (3 << 5)
#define MOCS_eLLC_ONLY (0 << 3)
#define MOCS_LLC_ONLY (1 << 3)
#define MOCS_eLLC_LLC (2 << 3)
#define MOCS_ALL_CACHES (3 << 3)

/**
 * Sets up the common fields for a surface state buffer for the given
 * picture in the given surface state buffer.
 */
static uint32_t
gen8_bind_bo(struct sna *sna,
	     struct kgem_bo *bo,
	     uint32_t width,
	     uint32_t height,
	     uint32_t format,
	     bool is_dst)
{
	uint32_t *ss;
	uint32_t domains;
	int offset;
	uint32_t is_scanout = is_dst && bo->scanout;

	/* After the first bind, we manage the cache domains within the batch */
	offset = kgem_bo_get_binding(bo, format | is_dst << 30 | is_scanout << 31);
	if (offset) {
		if (is_dst)
			kgem_bo_mark_dirty(bo);
		assert(offset >= sna->kgem.surface);
		return offset * sizeof(uint32_t);
	}

	offset = sna->kgem.surface -= SURFACE_DW;
	ss = sna->kgem.batch + offset;
	ss[0] = (SURFACE_2D << SURFACE_TYPE_SHIFT |
		 gen8_tiling_bits(bo->tiling) |
		 format << SURFACE_FORMAT_SHIFT |
		 SURFACE_VALIGN_4 | SURFACE_HALIGN_4);
	if (is_dst) {
		ss[0] |= SURFACE_RC_READ_WRITE;
		domains = I915_GEM_DOMAIN_RENDER << 16 |I915_GEM_DOMAIN_RENDER;
	} else
		domains = I915_GEM_DOMAIN_SAMPLER << 16;
	ss[1] = (is_dst && is_uncached(sna, bo)) ? 0 : is_scanout ? (MOCS_WT | MOCS_ALL_CACHES) << 24 : (MOCS_WB | MOCS_ALL_CACHES) << 24;
	ss[2] = ((width - 1)  << SURFACE_WIDTH_SHIFT |
		 (height - 1) << SURFACE_HEIGHT_SHIFT);
	ss[3] = (bo->pitch - 1) << SURFACE_PITCH_SHIFT;
	ss[4] = 0;
	ss[5] = 0;
	ss[6] = 0;
	ss[7] = SURFACE_SWIZZLE(RED, GREEN, BLUE, ALPHA);
	*(uint64_t *)(ss+8) = kgem_add_reloc64(&sna->kgem, offset + 8, bo, domains, 0);
	ss[10] = 0;
	ss[11] = 0;
	ss[12] = 0;
	ss[13] = 0;
	ss[14] = 0;
	ss[15] = 0;

	kgem_bo_set_binding(bo, format | is_dst << 30 | is_scanout << 31, offset);

	DBG(("[%x] bind bo(handle=%d, addr=%d), format=%d, width=%d, height=%d, pitch=%d, tiling=%d -> %s\n",
	     offset, bo->handle, ss[1],
	     format, width, height, bo->pitch, bo->tiling,
	     domains & 0xffff ? "render" : "sampler"));

	return offset * sizeof(uint32_t);
}

static void gen8_emit_vertex_buffer(struct sna *sna,
				    const struct sna_composite_op *op)
{
	int id = GEN8_VERTEX(op->u.gen8.flags);

	OUT_BATCH(GEN8_3DSTATE_VERTEX_BUFFERS | (5 - 2));
	OUT_BATCH(id << VB_INDEX_SHIFT | VB_MODIFY_ENABLE |
		  4*op->floats_per_vertex);
	sna->render.vertex_reloc[sna->render.nvertex_reloc++] = sna->kgem.nbatch;
	OUT_BATCH64(0);
	OUT_BATCH(~0); /* buffer size: disabled */

	OUT_BATCH(GEN8_3DSTATE_VF_INSTANCING | (3 - 2));
	OUT_BATCH(id);
	OUT_BATCH(0);

	sna->render.vb_id |= 1 << id;
}

static void gen8_emit_primitive(struct sna *sna)
{
	if (sna->kgem.nbatch == sna->render_state.gen8.last_primitive) {
		sna->render.vertex_offset = sna->kgem.nbatch - 5;
		return;
	}

	OUT_BATCH(GEN8_3DPRIMITIVE | (7 - 2));
	OUT_BATCH(RECTLIST); /* ignored, see VF_TOPOLOGY */
	sna->render.vertex_offset = sna->kgem.nbatch;
	OUT_BATCH(0);	/* vertex count, to be filled in later */
	OUT_BATCH(sna->render.vertex_index);
	OUT_BATCH(1);	/* single instance */
	OUT_BATCH(0);	/* start instance location */
	OUT_BATCH(0);	/* index buffer offset, ignored */
	sna->render.vertex_start = sna->render.vertex_index;

	sna->render_state.gen8.last_primitive = sna->kgem.nbatch;
}

static bool gen8_rectangle_begin(struct sna *sna,
				 const struct sna_composite_op *op)
{
	int id = 1 << GEN8_VERTEX(op->u.gen8.flags);
	int ndwords;

	if (sna_vertex_wait__locked(&sna->render) && sna->render.vertex_offset)
		return true;

	ndwords = op->need_magic_ca_pass ? 60 : 6;
	if ((sna->render.vb_id & id) == 0)
		ndwords += 5;
	if (!kgem_check_batch(&sna->kgem, ndwords))
		return false;

	if ((sna->render.vb_id & id) == 0)
		gen8_emit_vertex_buffer(sna, op);

	gen8_emit_primitive(sna);
	return true;
}

static int gen8_get_rectangles__flush(struct sna *sna,
				      const struct sna_composite_op *op)
{
	/* Preventing discarding new vbo after lock contention */
	if (sna_vertex_wait__locked(&sna->render)) {
		int rem = vertex_space(sna);
		if (rem > op->floats_per_rect)
			return rem;
	}

	if (!kgem_check_batch(&sna->kgem, op->need_magic_ca_pass ? 65 : 6))
		return 0;
	if (!kgem_check_reloc_and_exec(&sna->kgem, 2))
		return 0;

	if (sna->render.vertex_offset) {
		gen8_vertex_flush(sna);
		if (gen8_magic_ca_pass(sna, op)) {
			gen8_emit_pipe_invalidate(sna);
			gen8_emit_cc(sna, GEN8_BLEND(op->u.gen8.flags));
			gen8_emit_wm(sna, GEN8_KERNEL(op->u.gen8.flags));
		}
	}

	return gen8_vertex_finish(sna);
}

inline static int gen8_get_rectangles(struct sna *sna,
				      const struct sna_composite_op *op,
				      int want,
				      void (*emit_state)(struct sna *sna, const struct sna_composite_op *op))
{
	int rem;

	assert(want);

start:
	rem = vertex_space(sna);
	if (unlikely(rem < op->floats_per_rect)) {
		DBG(("flushing vbo for %s: %d < %d\n",
		     __FUNCTION__, rem, op->floats_per_rect));
		rem = gen8_get_rectangles__flush(sna, op);
		if (unlikely(rem == 0))
			goto flush;
	}

	if (unlikely(sna->render.vertex_offset == 0)) {
		if (!gen8_rectangle_begin(sna, op))
			goto flush;
		else
			goto start;
	}

	assert(rem <= vertex_space(sna));
	assert(op->floats_per_rect <= rem);
	if (want > 1 && want * op->floats_per_rect > rem)
		want = rem / op->floats_per_rect;

	assert(want > 0);
	sna->render.vertex_index += 3*want;
	return want;

flush:
	if (sna->render.vertex_offset) {
		gen8_vertex_flush(sna);
		gen8_magic_ca_pass(sna, op);
	}
	sna_vertex_wait__locked(&sna->render);
	_kgem_submit(&sna->kgem);
	emit_state(sna, op);
	goto start;
}

inline static uint32_t *gen8_composite_get_binding_table(struct sna *sna,
							 uint16_t *offset)
{
	uint32_t *table;

	assert(sna->kgem.surface <= 16384);
	sna->kgem.surface -= SURFACE_DW;
	/* Clear all surplus entries to zero in case of prefetch */
	table = memset(sna->kgem.batch + sna->kgem.surface, 0, 64);

	DBG(("%s(%x)\n", __FUNCTION__, 4*sna->kgem.surface));

	*offset = sna->kgem.surface;
	return table;
}

static void
gen8_get_batch(struct sna *sna, const struct sna_composite_op *op)
{
	kgem_set_mode(&sna->kgem, KGEM_RENDER, op->dst.bo);

	if (!kgem_check_batch_with_surfaces(&sna->kgem, 150, 2*(1+3))) {
		DBG(("%s: flushing batch: %d < %d+%d\n",
		     __FUNCTION__, sna->kgem.surface - sna->kgem.nbatch,
		     150, 4*8*2));
		_kgem_submit(&sna->kgem);
		_kgem_set_mode(&sna->kgem, KGEM_RENDER);
	}

	assert(sna->kgem.mode == KGEM_RENDER);
	assert(sna->kgem.ring == KGEM_RENDER);

	if (sna->render_state.gen8.needs_invariant)
		gen8_emit_invariant(sna);
}

static void gen8_emit_composite_state(struct sna *sna,
				      const struct sna_composite_op *op)
{
	uint32_t *binding_table;
	uint16_t offset, dirty;

	gen8_get_batch(sna, op);

	binding_table = gen8_composite_get_binding_table(sna, &offset);

	dirty = kgem_bo_is_dirty(op->dst.bo);

	binding_table[0] =
		gen8_bind_bo(sna,
			    op->dst.bo, op->dst.width, op->dst.height,
			    gen8_get_dest_format(op->dst.format),
			    true);
	binding_table[1] =
		gen8_bind_bo(sna,
			     op->src.bo, op->src.width, op->src.height,
			     op->src.card_format,
			     false);
	if (op->mask.bo) {
		binding_table[2] =
			gen8_bind_bo(sna,
				     op->mask.bo,
				     op->mask.width,
				     op->mask.height,
				     op->mask.card_format,
				     false);
	}

	if (sna->kgem.surface == offset &&
	    *(uint64_t *)(sna->kgem.batch + sna->render_state.gen8.surface_table) == *(uint64_t*)binding_table &&
	    (op->mask.bo == NULL ||
	     sna->kgem.batch[sna->render_state.gen8.surface_table+2] == binding_table[2])) {
		sna->kgem.surface += SURFACE_DW;
		offset = sna->render_state.gen8.surface_table;
	}

	if (sna->kgem.batch[sna->render_state.gen8.surface_table] == binding_table[0])
		dirty = 0;

	gen8_emit_state(sna, op, offset | dirty);
}

static void
gen8_align_vertex(struct sna *sna, const struct sna_composite_op *op)
{
	if (op->floats_per_vertex != sna->render_state.gen8.floats_per_vertex) {
		DBG(("aligning vertex: was %d, now %d floats per vertex\n",
		     sna->render_state.gen8.floats_per_vertex, op->floats_per_vertex));
		gen8_vertex_align(sna, op);
		sna->render_state.gen8.floats_per_vertex = op->floats_per_vertex;
	}
}

fastcall static void
gen8_render_composite_blt(struct sna *sna,
			  const struct sna_composite_op *op,
			  const struct sna_composite_rectangles *r)
{
	gen8_get_rectangles(sna, op, 1, gen8_emit_composite_state);
	op->prim_emit(sna, op, r);
}

fastcall static void
gen8_render_composite_box(struct sna *sna,
			  const struct sna_composite_op *op,
			  const BoxRec *box)
{
	struct sna_composite_rectangles r;

	gen8_get_rectangles(sna, op, 1, gen8_emit_composite_state);

	DBG(("  %s: (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     box->x1, box->y1, box->x2, box->y2));

	r.dst.x = box->x1;
	r.dst.y = box->y1;
	r.width  = box->x2 - box->x1;
	r.height = box->y2 - box->y1;
	r.src = r.mask = r.dst;

	op->prim_emit(sna, op, &r);
}

static void
gen8_render_composite_boxes__blt(struct sna *sna,
				 const struct sna_composite_op *op,
				 const BoxRec *box, int nbox)
{
	DBG(("composite_boxes(%d)\n", nbox));

	do {
		int nbox_this_time;

		nbox_this_time = gen8_get_rectangles(sna, op, nbox,
						     gen8_emit_composite_state);
		nbox -= nbox_this_time;

		do {
			struct sna_composite_rectangles r;

			DBG(("  %s: (%d, %d), (%d, %d)\n",
			     __FUNCTION__,
			     box->x1, box->y1, box->x2, box->y2));

			r.dst.x = box->x1;
			r.dst.y = box->y1;
			r.width  = box->x2 - box->x1;
			r.height = box->y2 - box->y1;
			r.src = r.mask = r.dst;

			op->prim_emit(sna, op, &r);
			box++;
		} while (--nbox_this_time);
	} while (nbox);
}

static void
gen8_render_composite_boxes(struct sna *sna,
			    const struct sna_composite_op *op,
			    const BoxRec *box, int nbox)
{
	DBG(("%s: nbox=%d\n", __FUNCTION__, nbox));

	do {
		int nbox_this_time;
		float *v;

		nbox_this_time = gen8_get_rectangles(sna, op, nbox,
						     gen8_emit_composite_state);
		assert(nbox_this_time);
		nbox -= nbox_this_time;

		v = sna->render.vertices + sna->render.vertex_used;
		sna->render.vertex_used += nbox_this_time * op->floats_per_rect;

		op->emit_boxes(op, box, nbox_this_time, v);
		box += nbox_this_time;
	} while (nbox);
}

static void
gen8_render_composite_boxes__thread(struct sna *sna,
				    const struct sna_composite_op *op,
				    const BoxRec *box, int nbox)
{
	DBG(("%s: nbox=%d\n", __FUNCTION__, nbox));

	sna_vertex_lock(&sna->render);
	do {
		int nbox_this_time;
		float *v;

		nbox_this_time = gen8_get_rectangles(sna, op, nbox,
						     gen8_emit_composite_state);
		assert(nbox_this_time);
		nbox -= nbox_this_time;

		v = sna->render.vertices + sna->render.vertex_used;
		sna->render.vertex_used += nbox_this_time * op->floats_per_rect;

		sna_vertex_acquire__locked(&sna->render);
		sna_vertex_unlock(&sna->render);

		op->emit_boxes(op, box, nbox_this_time, v);
		box += nbox_this_time;

		sna_vertex_lock(&sna->render);
		sna_vertex_release__locked(&sna->render);
	} while (nbox);
	sna_vertex_unlock(&sna->render);
}

static uint32_t
gen8_create_blend_state(struct sna_static_stream *stream)
{
	char *base, *ptr;
	int src, dst;

	COMPILE_TIME_ASSERT(((GEN8_BLENDFACTOR_COUNT * GEN8_BLENDFACTOR_COUNT << 4) & (1 << 15)) == 0);

	base = sna_static_stream_map(stream,
				     GEN8_BLENDFACTOR_COUNT * GEN8_BLENDFACTOR_COUNT * GEN8_BLEND_STATE_PADDED_SIZE,
				     64);

	ptr = base;
	for (src = 0; src < GEN8_BLENDFACTOR_COUNT; src++) {
		for (dst = 0; dst < GEN8_BLENDFACTOR_COUNT; dst++) {
			struct gen8_blend_state *blend =
				(struct gen8_blend_state *)ptr;

			assert(((ptr - base) & 63) == 0);
			COMPILE_TIME_ASSERT(sizeof(blend->common) == 4);
			COMPILE_TIME_ASSERT(sizeof(blend->rt) == 8);

			blend->rt.post_blend_clamp = 1;
			blend->rt.pre_blend_clamp = 1;

			blend->rt.color_blend =
				!(dst == BLENDFACTOR_ZERO && src == BLENDFACTOR_ONE);
			blend->rt.dest_blend_factor = dst;
			blend->rt.source_blend_factor = src;
			blend->rt.color_blend_function = BLENDFUNCTION_ADD;

			blend->rt.dest_alpha_blend_factor = dst;
			blend->rt.source_alpha_blend_factor = src;
			blend->rt.alpha_blend_function = BLENDFUNCTION_ADD;

			ptr += GEN8_BLEND_STATE_PADDED_SIZE;
		}
	}

	return sna_static_stream_offsetof(stream, base);
}

static int
gen8_composite_picture(struct sna *sna,
		       PicturePtr picture,
		       struct sna_composite_channel *channel,
		       int x, int y,
		       int w, int h,
		       int dst_x, int dst_y,
		       bool precise)
{
	PixmapPtr pixmap;
	uint32_t color;
	int16_t dx, dy;

	DBG(("%s: (%d, %d)x(%d, %d), dst=(%d, %d)\n",
	     __FUNCTION__, x, y, w, h, dst_x, dst_y));

	channel->is_solid = false;
	channel->card_format = -1;

	if (sna_picture_is_solid(picture, &color))
		return gen4_channel_init_solid(sna, channel, color);

	if (picture->pDrawable == NULL) {
		int ret;

		if (picture->pSourcePict->type == SourcePictTypeLinear)
			return gen4_channel_init_linear(sna, picture, channel,
							x, y,
							w, h,
							dst_x, dst_y);

		DBG(("%s -- fixup, gradient\n", __FUNCTION__));
		ret = -1;
		if (!precise)
			ret = sna_render_picture_approximate_gradient(sna, picture, channel,
								      x, y, w, h, dst_x, dst_y);
		if (ret == -1)
			ret = sna_render_picture_fixup(sna, picture, channel,
						       x, y, w, h, dst_x, dst_y);
		return ret;
	}

	if (picture->alphaMap) {
		DBG(("%s -- fallback, alphamap\n", __FUNCTION__));
		return sna_render_picture_fixup(sna, picture, channel,
						x, y, w, h, dst_x, dst_y);
	}

	if (!gen8_check_repeat(picture))
		return sna_render_picture_fixup(sna, picture, channel,
						x, y, w, h, dst_x, dst_y);

	if (!gen8_check_filter(picture))
		return sna_render_picture_fixup(sna, picture, channel,
						x, y, w, h, dst_x, dst_y);

	channel->repeat = picture->repeat ? picture->repeatType : RepeatNone;
	channel->filter = picture->filter;

	pixmap = get_drawable_pixmap(picture->pDrawable);
	get_drawable_deltas(picture->pDrawable, pixmap, &dx, &dy);

	x += dx + picture->pDrawable->x;
	y += dy + picture->pDrawable->y;

	channel->is_affine = sna_transform_is_affine(picture->transform);
	if (sna_transform_is_imprecise_integer_translation(picture->transform, picture->filter, precise, &dx, &dy)) {
		DBG(("%s: integer translation (%d, %d), removing\n",
		     __FUNCTION__, dx, dy));
		x += dx;
		y += dy;
		channel->transform = NULL;
		channel->filter = PictFilterNearest;

		if (channel->repeat ||
		    (x >= 0 &&
		     y >= 0 &&
		     x + w < pixmap->drawable.width &&
		     y + h < pixmap->drawable.height)) {
			struct sna_pixmap *priv = sna_pixmap(pixmap);
			if (priv && priv->clear) {
				DBG(("%s: converting large pixmap source into solid [%08x]\n", __FUNCTION__, priv->clear_color));
				return gen4_channel_init_solid(sna, channel, priv->clear_color);
			}
		}
	} else
		channel->transform = picture->transform;

	channel->pict_format = picture->format;
	channel->card_format = gen8_get_card_format(picture->format);
	if (channel->card_format == (unsigned)-1)
		return sna_render_picture_convert(sna, picture, channel, pixmap,
						  x, y, w, h, dst_x, dst_y,
						  false);

	if (too_large(pixmap->drawable.width, pixmap->drawable.height)) {
		DBG(("%s: extracting from pixmap %dx%d\n", __FUNCTION__,
		     pixmap->drawable.width, pixmap->drawable.height));
		return sna_render_picture_extract(sna, picture, channel,
						  x, y, w, h, dst_x, dst_y);
	}

	return sna_render_pixmap_bo(sna, channel, pixmap,
				    x, y, w, h, dst_x, dst_y);
}

inline static bool gen8_composite_channel_convert(struct sna_composite_channel *channel)
{
	if (unaligned(channel->bo, PICT_FORMAT_BPP(channel->pict_format)))
		return false;

	channel->repeat = gen8_repeat(channel->repeat);
	channel->filter = gen8_filter(channel->filter);
	if (channel->card_format == (unsigned)-1)
		channel->card_format = gen8_get_card_format(channel->pict_format);
	assert(channel->card_format != (unsigned)-1);

	return true;
}

static void gen8_render_composite_done(struct sna *sna,
				       const struct sna_composite_op *op)
{
	if (sna->render.vertex_offset) {
		gen8_vertex_flush(sna);
		gen8_magic_ca_pass(sna, op);
	}

	if (op->mask.bo)
		kgem_bo_destroy(&sna->kgem, op->mask.bo);
	if (op->src.bo)
		kgem_bo_destroy(&sna->kgem, op->src.bo);

	sna_render_composite_redirect_done(sna, op);
}

inline static bool
gen8_composite_set_target(struct sna *sna,
			  struct sna_composite_op *op,
			  PicturePtr dst,
			  int x, int y, int w, int h,
			  bool partial)
{
	BoxRec box;
	unsigned int hint;

	DBG(("%s: (%d, %d)x(%d, %d), partial?=%d\n", __FUNCTION__, x, y, w, h, partial));

	op->dst.pixmap = get_drawable_pixmap(dst->pDrawable);
	op->dst.format = dst->format;
	op->dst.width  = op->dst.pixmap->drawable.width;
	op->dst.height = op->dst.pixmap->drawable.height;

	if (w | h) {
		assert(w && h);
		box.x1 = x;
		box.y1 = y;
		box.x2 = x + w;
		box.y2 = y + h;
	} else
		sna_render_picture_extents(dst, &box);

	hint = PREFER_GPU | FORCE_GPU | RENDER_GPU;
	if (!partial) {
		hint |= IGNORE_DAMAGE;
		if (w == op->dst.width && h == op->dst.height)
			hint |= REPLACES;
	}

	op->dst.bo = sna_drawable_use_bo(dst->pDrawable, hint, &box, &op->damage);
	if (op->dst.bo == NULL)
		return false;

	if (unaligned(op->dst.bo, dst->pDrawable->bitsPerPixel))
		return false;

	if (hint & REPLACES) {
		struct sna_pixmap *priv = sna_pixmap(op->dst.pixmap);
		kgem_bo_pair_undo(&sna->kgem, priv->gpu_bo, priv->cpu_bo);
	}

	get_drawable_deltas(dst->pDrawable, op->dst.pixmap,
			    &op->dst.x, &op->dst.y);

	DBG(("%s: pixmap=%ld, format=%08x, size=%dx%d, pitch=%d, delta=(%d,%d),damage=%p\n",
	     __FUNCTION__,
	     op->dst.pixmap->drawable.serialNumber, (int)op->dst.format,
	     op->dst.width, op->dst.height,
	     op->dst.bo->pitch,
	     op->dst.x, op->dst.y,
	     op->damage ? *op->damage : (void *)-1));

	assert(op->dst.bo->proxy == NULL);

	if (too_large(op->dst.width, op->dst.height) &&
	    !sna_render_composite_redirect(sna, op, x, y, w, h, partial))
		return false;

	return true;
}

static bool
try_blt(struct sna *sna,
	PicturePtr dst, PicturePtr src,
	int width, int height)
{
	struct kgem_bo *bo;

	if (sna->kgem.mode == KGEM_BLT) {
		DBG(("%s: already performing BLT\n", __FUNCTION__));
		return true;
	}

	if (too_large(width, height)) {
		DBG(("%s: operation too large for 3D pipe (%d, %d)\n",
		     __FUNCTION__, width, height));
		return true;
	}

	bo = __sna_drawable_peek_bo(dst->pDrawable);
	if (bo == NULL)
		return true;
	if (bo->rq)
		return RQ_IS_BLT(bo->rq);

	if (sna_picture_is_solid(src, NULL) && can_switch_to_blt(sna, bo, 0))
		return true;

	if (src->pDrawable) {
		bo = __sna_drawable_peek_bo(src->pDrawable);
		if (bo == NULL)
			return true;

		if (prefer_blt_bo(sna, bo))
			return RQ_IS_BLT(bo->rq);
	}

	if (sna->kgem.ring == KGEM_BLT) {
		DBG(("%s: already performing BLT\n", __FUNCTION__));
		return true;
	}

	return false;
}

static bool
check_gradient(PicturePtr picture, bool precise)
{
	if (picture->pDrawable)
		return false;

	switch (picture->pSourcePict->type) {
	case SourcePictTypeSolidFill:
	case SourcePictTypeLinear:
		return false;
	default:
		return precise;
	}
}

static bool
has_alphamap(PicturePtr p)
{
	return p->alphaMap != NULL;
}

static bool
need_upload(PicturePtr p)
{
	return p->pDrawable && unattached(p->pDrawable) && untransformed(p);
}

static bool
source_is_busy(PixmapPtr pixmap)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	if (priv == NULL || priv->clear)
		return false;

	if (priv->gpu_bo && kgem_bo_is_busy(priv->gpu_bo))
		return true;

	if (priv->cpu_bo && kgem_bo_is_busy(priv->cpu_bo))
		return true;

	return priv->gpu_damage && !priv->cpu_damage;
}

static bool
source_fallback(PicturePtr p, PixmapPtr pixmap, bool precise)
{
	if (sna_picture_is_solid(p, NULL))
		return false;

	if (p->pSourcePict)
		return check_gradient(p, precise);

	if (!gen8_check_repeat(p) || !gen8_check_format(p->format))
		return true;

	if (pixmap && source_is_busy(pixmap))
		return false;

	return has_alphamap(p) || !gen8_check_filter(p) || need_upload(p);
}

static bool
gen8_composite_fallback(struct sna *sna,
			PicturePtr src,
			PicturePtr mask,
			PicturePtr dst)
{
	PixmapPtr src_pixmap;
	PixmapPtr mask_pixmap;
	PixmapPtr dst_pixmap;
	bool src_fallback, mask_fallback;

	if (!gen8_check_dst_format(dst->format)) {
		DBG(("%s: unknown destination format: %d\n",
		     __FUNCTION__, dst->format));
		return true;
	}

	dst_pixmap = get_drawable_pixmap(dst->pDrawable);

	src_pixmap = src->pDrawable ? get_drawable_pixmap(src->pDrawable) : NULL;
	src_fallback = source_fallback(src, src_pixmap,
				       dst->polyMode == PolyModePrecise);

	if (mask) {
		mask_pixmap = mask->pDrawable ? get_drawable_pixmap(mask->pDrawable) : NULL;
		mask_fallback = source_fallback(mask, mask_pixmap,
						dst->polyMode == PolyModePrecise);
	} else {
		mask_pixmap = NULL;
		mask_fallback = false;
	}

	/* If we are using the destination as a source and need to
	 * readback in order to upload the source, do it all
	 * on the cpu.
	 */
	if (src_pixmap == dst_pixmap && src_fallback) {
		DBG(("%s: src is dst and will fallback\n",__FUNCTION__));
		return true;
	}
	if (mask_pixmap == dst_pixmap && mask_fallback) {
		DBG(("%s: mask is dst and will fallback\n",__FUNCTION__));
		return true;
	}

	/* If anything is on the GPU, push everything out to the GPU */
	if (dst_use_gpu(dst_pixmap)) {
		DBG(("%s: dst is already on the GPU, try to use GPU\n",
		     __FUNCTION__));
		return false;
	}

	if (src_pixmap && !src_fallback) {
		DBG(("%s: src is already on the GPU, try to use GPU\n",
		     __FUNCTION__));
		return false;
	}
	if (mask_pixmap && !mask_fallback) {
		DBG(("%s: mask is already on the GPU, try to use GPU\n",
		     __FUNCTION__));
		return false;
	}

	/* However if the dst is not on the GPU and we need to
	 * render one of the sources using the CPU, we may
	 * as well do the entire operation in place onthe CPU.
	 */
	if (src_fallback) {
		DBG(("%s: dst is on the CPU and src will fallback\n",
		     __FUNCTION__));
		return true;
	}

	if (mask && mask_fallback) {
		DBG(("%s: dst is on the CPU and mask will fallback\n",
		     __FUNCTION__));
		return true;
	}

	if (too_large(dst_pixmap->drawable.width,
		      dst_pixmap->drawable.height) &&
	    dst_is_cpu(dst_pixmap)) {
		DBG(("%s: dst is on the CPU and too large\n", __FUNCTION__));
		return true;
	}

	DBG(("%s: dst is not on the GPU and the operation should not fallback\n",
	     __FUNCTION__));
	return dst_use_cpu(dst_pixmap);
}

static int
reuse_source(struct sna *sna,
	     PicturePtr src, struct sna_composite_channel *sc, int src_x, int src_y,
	     PicturePtr mask, struct sna_composite_channel *mc, int msk_x, int msk_y)
{
	uint32_t color;

	if (src_x != msk_x || src_y != msk_y)
		return false;

	if (src == mask) {
		DBG(("%s: mask is source\n", __FUNCTION__));
		*mc = *sc;
		mc->bo = kgem_bo_reference(mc->bo);
		return true;
	}

	if (sna_picture_is_solid(mask, &color))
		return gen4_channel_init_solid(sna, mc, color);

	if (sc->is_solid)
		return false;

	if (src->pDrawable == NULL || mask->pDrawable != src->pDrawable)
		return false;

	DBG(("%s: mask reuses source drawable\n", __FUNCTION__));

	if (!sna_transform_equal(src->transform, mask->transform))
		return false;

	if (!sna_picture_alphamap_equal(src, mask))
		return false;

	if (!gen8_check_repeat(mask))
		return false;

	if (!gen8_check_filter(mask))
		return false;

	if (!gen8_check_format(mask->format))
		return false;

	DBG(("%s: reusing source channel for mask with a twist\n",
	     __FUNCTION__));

	*mc = *sc;
	mc->repeat = gen8_repeat(mask->repeat ? mask->repeatType : RepeatNone);
	mc->filter = gen8_filter(mask->filter);
	mc->pict_format = mask->format;
	mc->card_format = gen8_get_card_format(mask->format);
	mc->bo = kgem_bo_reference(mc->bo);
	return true;
}

static bool
gen8_render_composite(struct sna *sna,
		      uint8_t op,
		      PicturePtr src,
		      PicturePtr mask,
		      PicturePtr dst,
		      int16_t src_x, int16_t src_y,
		      int16_t msk_x, int16_t msk_y,
		      int16_t dst_x, int16_t dst_y,
		      int16_t width, int16_t height,
		      unsigned flags,
		      struct sna_composite_op *tmp)
{
	if (op >= ARRAY_SIZE(gen8_blend_op))
		return false;

	DBG(("%s: %dx%d, current mode=%d/%d\n", __FUNCTION__,
	     width, height, sna->kgem.mode, sna->kgem.ring));

	if (mask == NULL &&
	    try_blt(sna, dst, src, width, height) &&
	    sna_blt_composite(sna, op,
			      src, dst,
			      src_x, src_y,
			      dst_x, dst_y,
			      width, height,
			      flags, tmp))
		return true;

	if (gen8_composite_fallback(sna, src, mask, dst))
		goto fallback;

	if (need_tiling(sna, width, height))
		return sna_tiling_composite(op, src, mask, dst,
					    src_x, src_y,
					    msk_x, msk_y,
					    dst_x, dst_y,
					    width, height,
					    tmp);

	if (op == PictOpClear && src == sna->clear)
		op = PictOpSrc;
	tmp->op = op;
	if (!gen8_composite_set_target(sna, tmp, dst,
				       dst_x, dst_y, width, height,
				       flags & COMPOSITE_PARTIAL || op > PictOpSrc))
		goto fallback;

	switch (gen8_composite_picture(sna, src, &tmp->src,
				       src_x, src_y,
				       width, height,
				       dst_x, dst_y,
				       dst->polyMode == PolyModePrecise)) {
	case -1:
		goto cleanup_dst;
	case 0:
		if (!gen4_channel_init_solid(sna, &tmp->src, 0))
			goto cleanup_dst;
		/* fall through to fixup */
	case 1:
		/* Did we just switch rings to prepare the source? */
		if (mask == NULL &&
		    (prefer_blt_composite(sna, tmp) ||
		     unaligned(tmp->src.bo, PICT_FORMAT_BPP(tmp->src.pict_format))) &&
		    sna_blt_composite__convert(sna,
					       dst_x, dst_y, width, height,
					       tmp))
			return true;

		if (!gen8_composite_channel_convert(&tmp->src))
			goto cleanup_src;

		break;
	}

	tmp->is_affine = tmp->src.is_affine;
	tmp->has_component_alpha = false;
	tmp->need_magic_ca_pass = false;

	tmp->mask.bo = NULL;
	tmp->mask.filter = SAMPLER_FILTER_NEAREST;
	tmp->mask.repeat = SAMPLER_EXTEND_NONE;

	if (mask) {
		if (mask->componentAlpha && PICT_FORMAT_RGB(mask->format)) {
			tmp->has_component_alpha = true;

			/* Check if it's component alpha that relies on a source alpha and on
			 * the source value.  We can only get one of those into the single
			 * source value that we get to blend with.
			 */
			if (gen8_blend_op[op].src_alpha &&
			    (gen8_blend_op[op].src_blend != BLENDFACTOR_ZERO)) {
				if (op != PictOpOver)
					goto cleanup_src;

				tmp->need_magic_ca_pass = true;
				tmp->op = PictOpOutReverse;
			}
		}

		if (!reuse_source(sna,
				  src, &tmp->src, src_x, src_y,
				  mask, &tmp->mask, msk_x, msk_y)) {
			switch (gen8_composite_picture(sna, mask, &tmp->mask,
						       msk_x, msk_y,
						       width, height,
						       dst_x, dst_y,
						       dst->polyMode == PolyModePrecise)) {
			case -1:
				goto cleanup_src;
			case 0:
				if (!gen4_channel_init_solid(sna, &tmp->mask, 0))
					goto cleanup_src;
				/* fall through to fixup */
			case 1:
				if (!gen8_composite_channel_convert(&tmp->mask))
					goto cleanup_mask;
				break;
			}
		}

		tmp->is_affine &= tmp->mask.is_affine;
	}

	tmp->u.gen8.flags =
		GEN8_SET_FLAGS(SAMPLER_OFFSET(tmp->src.filter,
					      tmp->src.repeat,
					      tmp->mask.filter,
					      tmp->mask.repeat),
			       gen8_get_blend(tmp->op,
					      tmp->has_component_alpha,
					      tmp->dst.format),
			       gen8_choose_composite_kernel(tmp->op,
							    tmp->mask.bo != NULL,
							    tmp->has_component_alpha,
							    tmp->is_affine),
			       gen4_choose_composite_emitter(sna, tmp));

	tmp->blt   = gen8_render_composite_blt;
	tmp->box   = gen8_render_composite_box;
	tmp->boxes = gen8_render_composite_boxes__blt;
	if (tmp->emit_boxes){
		tmp->boxes = gen8_render_composite_boxes;
		tmp->thread_boxes = gen8_render_composite_boxes__thread;
	}
	tmp->done  = gen8_render_composite_done;

	kgem_set_mode(&sna->kgem, KGEM_RENDER, tmp->dst.bo);
	if (!kgem_check_bo(&sna->kgem,
			   tmp->dst.bo, tmp->src.bo, tmp->mask.bo,
			   NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem,
				   tmp->dst.bo, tmp->src.bo, tmp->mask.bo,
				   NULL))
			goto cleanup_mask;
		_kgem_set_mode(&sna->kgem, KGEM_RENDER);
	}

	gen8_align_vertex(sna, tmp);
	gen8_emit_composite_state(sna, tmp);
	return true;

cleanup_mask:
	if (tmp->mask.bo) {
		kgem_bo_destroy(&sna->kgem, tmp->mask.bo);
		tmp->mask.bo = NULL;
	}
cleanup_src:
	if (tmp->src.bo) {
		kgem_bo_destroy(&sna->kgem, tmp->src.bo);
		tmp->src.bo = NULL;
	}
cleanup_dst:
	if (tmp->redirect.real_bo) {
		kgem_bo_destroy(&sna->kgem, tmp->dst.bo);
		tmp->redirect.real_bo = NULL;
	}
fallback:
	return (mask == NULL &&
		sna_blt_composite(sna, op,
				  src, dst,
				  src_x, src_y,
				  dst_x, dst_y,
				  width, height,
				  flags | COMPOSITE_FALLBACK, tmp));
}

#if !NO_COMPOSITE_SPANS
fastcall static void
gen8_render_composite_spans_box(struct sna *sna,
				const struct sna_composite_spans_op *op,
				const BoxRec *box, float opacity)
{
	DBG(("%s: src=+(%d, %d), opacity=%f, dst=+(%d, %d), box=(%d, %d) x (%d, %d)\n",
	     __FUNCTION__,
	     op->base.src.offset[0], op->base.src.offset[1],
	     opacity,
	     op->base.dst.x, op->base.dst.y,
	     box->x1, box->y1,
	     box->x2 - box->x1,
	     box->y2 - box->y1));

	gen8_get_rectangles(sna, &op->base, 1, gen8_emit_composite_state);
	op->prim_emit(sna, op, box, opacity);
}

static void
gen8_render_composite_spans_boxes(struct sna *sna,
				  const struct sna_composite_spans_op *op,
				  const BoxRec *box, int nbox,
				  float opacity)
{
	DBG(("%s: nbox=%d, src=+(%d, %d), opacity=%f, dst=+(%d, %d)\n",
	     __FUNCTION__, nbox,
	     op->base.src.offset[0], op->base.src.offset[1],
	     opacity,
	     op->base.dst.x, op->base.dst.y));

	do {
		int nbox_this_time;

		nbox_this_time = gen8_get_rectangles(sna, &op->base, nbox,
						     gen8_emit_composite_state);
		nbox -= nbox_this_time;

		do {
			DBG(("  %s: (%d, %d) x (%d, %d)\n", __FUNCTION__,
			     box->x1, box->y1,
			     box->x2 - box->x1,
			     box->y2 - box->y1));

			op->prim_emit(sna, op, box++, opacity);
		} while (--nbox_this_time);
	} while (nbox);
}

fastcall static void
gen8_render_composite_spans_boxes__thread(struct sna *sna,
					  const struct sna_composite_spans_op *op,
					  const struct sna_opacity_box *box,
					  int nbox)
{
	DBG(("%s: nbox=%d, src=+(%d, %d), dst=+(%d, %d)\n",
	     __FUNCTION__, nbox,
	     op->base.src.offset[0], op->base.src.offset[1],
	     op->base.dst.x, op->base.dst.y));

	sna_vertex_lock(&sna->render);
	do {
		int nbox_this_time;
		float *v;

		nbox_this_time = gen8_get_rectangles(sna, &op->base, nbox,
						     gen8_emit_composite_state);
		assert(nbox_this_time);
		nbox -= nbox_this_time;

		v = sna->render.vertices + sna->render.vertex_used;
		sna->render.vertex_used += nbox_this_time * op->base.floats_per_rect;

		sna_vertex_acquire__locked(&sna->render);
		sna_vertex_unlock(&sna->render);

		op->emit_boxes(op, box, nbox_this_time, v);
		box += nbox_this_time;

		sna_vertex_lock(&sna->render);
		sna_vertex_release__locked(&sna->render);
	} while (nbox);
	sna_vertex_unlock(&sna->render);
}

fastcall static void
gen8_render_composite_spans_done(struct sna *sna,
				 const struct sna_composite_spans_op *op)
{
	if (sna->render.vertex_offset)
		gen8_vertex_flush(sna);

	DBG(("%s()\n", __FUNCTION__));

	if (op->base.src.bo)
		kgem_bo_destroy(&sna->kgem, op->base.src.bo);

	sna_render_composite_redirect_done(sna, &op->base);
}

static bool
gen8_check_composite_spans(struct sna *sna,
			   uint8_t op, PicturePtr src, PicturePtr dst,
			   int16_t width, int16_t height, unsigned flags)
{
	if (op >= ARRAY_SIZE(gen8_blend_op))
		return false;

	if (gen8_composite_fallback(sna, src, NULL, dst))
		return false;

	if (need_tiling(sna, width, height) &&
	    !is_gpu(sna, dst->pDrawable, PREFER_GPU_SPANS)) {
		DBG(("%s: fallback, tiled operation not on GPU\n",
		     __FUNCTION__));
		return false;
	}

	return true;
}

static bool
gen8_render_composite_spans(struct sna *sna,
			    uint8_t op,
			    PicturePtr src,
			    PicturePtr dst,
			    int16_t src_x,  int16_t src_y,
			    int16_t dst_x,  int16_t dst_y,
			    int16_t width,  int16_t height,
			    unsigned flags,
			    struct sna_composite_spans_op *tmp)
{
	DBG(("%s: %dx%d with flags=%x, current mode=%d\n", __FUNCTION__,
	     width, height, flags, sna->kgem.ring));

	assert(gen8_check_composite_spans(sna, op, src, dst, width, height, flags));

	if (need_tiling(sna, width, height)) {
		DBG(("%s: tiling, operation (%dx%d) too wide for pipeline\n",
		     __FUNCTION__, width, height));
		return sna_tiling_composite_spans(op, src, dst,
						  src_x, src_y, dst_x, dst_y,
						  width, height, flags, tmp);
	}

	tmp->base.op = op;
	if (!gen8_composite_set_target(sna, &tmp->base, dst,
				       dst_x, dst_y, width, height, true))
		return false;

	switch (gen8_composite_picture(sna, src, &tmp->base.src,
				       src_x, src_y,
				       width, height,
				       dst_x, dst_y,
				       dst->polyMode == PolyModePrecise)) {
	case -1:
		goto cleanup_dst;
	case 0:
		if (!gen4_channel_init_solid(sna, &tmp->base.src, 0))
			goto cleanup_dst;
		/* fall through to fixup */
	case 1:
		if (!gen8_composite_channel_convert(&tmp->base.src))
			goto cleanup_src;
		break;
	}
	tmp->base.mask.bo = NULL;

	tmp->base.is_affine = tmp->base.src.is_affine;
	tmp->base.need_magic_ca_pass = false;

	tmp->base.u.gen8.flags =
		GEN8_SET_FLAGS(SAMPLER_OFFSET(tmp->base.src.filter,
					      tmp->base.src.repeat,
					      SAMPLER_FILTER_NEAREST,
					      SAMPLER_EXTEND_PAD),
			       gen8_get_blend(tmp->base.op, false, tmp->base.dst.format),
			       GEN8_WM_KERNEL_OPACITY | !tmp->base.is_affine,
			       gen4_choose_spans_emitter(sna, tmp));

	tmp->box   = gen8_render_composite_spans_box;
	tmp->boxes = gen8_render_composite_spans_boxes;
	if (tmp->emit_boxes)
		tmp->thread_boxes = gen8_render_composite_spans_boxes__thread;
	tmp->done  = gen8_render_composite_spans_done;

	kgem_set_mode(&sna->kgem, KGEM_RENDER, tmp->base.dst.bo);
	if (!kgem_check_bo(&sna->kgem,
			   tmp->base.dst.bo, tmp->base.src.bo,
			   NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem,
				   tmp->base.dst.bo, tmp->base.src.bo,
				   NULL))
			goto cleanup_src;
		_kgem_set_mode(&sna->kgem, KGEM_RENDER);
	}

	gen8_align_vertex(sna, &tmp->base);
	gen8_emit_composite_state(sna, &tmp->base);
	return true;

cleanup_src:
	if (tmp->base.src.bo)
		kgem_bo_destroy(&sna->kgem, tmp->base.src.bo);
cleanup_dst:
	if (tmp->base.redirect.real_bo)
		kgem_bo_destroy(&sna->kgem, tmp->base.dst.bo);
	return false;
}
#endif

static void
gen8_emit_copy_state(struct sna *sna,
		     const struct sna_composite_op *op)
{
	uint32_t *binding_table;
	uint16_t offset, dirty;

	gen8_get_batch(sna, op);

	binding_table = gen8_composite_get_binding_table(sna, &offset);

	dirty = kgem_bo_is_dirty(op->dst.bo);

	binding_table[0] =
		gen8_bind_bo(sna,
			     op->dst.bo, op->dst.width, op->dst.height,
			     gen8_get_dest_format(op->dst.format),
			     true);
	binding_table[1] =
		gen8_bind_bo(sna,
			     op->src.bo, op->src.width, op->src.height,
			     op->src.card_format,
			     false);

	if (sna->kgem.surface == offset &&
	    *(uint64_t *)(sna->kgem.batch + sna->render_state.gen8.surface_table) == *(uint64_t*)binding_table) {
		sna->kgem.surface += SURFACE_DW;
		offset = sna->render_state.gen8.surface_table;
	}

	if (sna->kgem.batch[sna->render_state.gen8.surface_table] == binding_table[0])
		dirty = 0;

	assert(!GEN8_READS_DST(op->u.gen8.flags));
	gen8_emit_state(sna, op, offset | dirty);
}

static inline bool
prefer_blt_copy(struct sna *sna,
		struct kgem_bo *src_bo,
		struct kgem_bo *dst_bo,
		unsigned flags)
{
	if (sna->kgem.mode == KGEM_BLT)
		return true;

	assert((flags & COPY_SYNC) == 0);

	if (src_bo == dst_bo && can_switch_to_blt(sna, dst_bo, flags))
		return true;

	if (untiled_tlb_miss(src_bo) ||
	    untiled_tlb_miss(dst_bo))
		return true;

	if (force_blt_ring(sna))
		return true;

	if (kgem_bo_is_render(dst_bo) ||
	    kgem_bo_is_render(src_bo))
		return false;

	if (prefer_render_ring(sna, dst_bo))
		return false;

	if (!prefer_blt_ring(sna, dst_bo, flags))
		return false;

	return prefer_blt_bo(sna, src_bo) || prefer_blt_bo(sna, dst_bo);
}

static bool
gen8_render_copy_boxes(struct sna *sna, uint8_t alu,
		       const DrawableRec *src, struct kgem_bo *src_bo, int16_t src_dx, int16_t src_dy,
		       const DrawableRec *dst, struct kgem_bo *dst_bo, int16_t dst_dx, int16_t dst_dy,
		       const BoxRec *box, int n, unsigned flags)
{
	struct sna_composite_op tmp;
	BoxRec extents;

	DBG(("%s (%d, %d)->(%d, %d) x %d, alu=%x, flags=%x, self-copy=%d, overlaps? %d\n",
	     __FUNCTION__, src_dx, src_dy, dst_dx, dst_dy, n, alu, flags,
	     src_bo == dst_bo,
	     overlaps(sna,
		      src_bo, src_dx, src_dy,
		      dst_bo, dst_dx, dst_dy,
		      box, n, flags, &extents)));

	if (prefer_blt_copy(sna, src_bo, dst_bo, flags) &&
	    sna_blt_compare_depth(src, dst) &&
	    sna_blt_copy_boxes(sna, alu,
			       src_bo, src_dx, src_dy,
			       dst_bo, dst_dx, dst_dy,
			       dst->bitsPerPixel,
			       box, n))
		return true;

	if (!(alu == GXcopy || alu == GXclear) ||
	    unaligned(src_bo, src->bitsPerPixel) ||
	    unaligned(dst_bo, dst->bitsPerPixel)) {
fallback_blt:
		DBG(("%s: fallback blt\n", __FUNCTION__));
		if (!sna_blt_compare_depth(src, dst))
			return false;

		return sna_blt_copy_boxes_fallback(sna, alu,
						   src, src_bo, src_dx, src_dy,
						   dst, dst_bo, dst_dx, dst_dy,
						   box, n);
	}

	if (overlaps(sna,
		     src_bo, src_dx, src_dy,
		     dst_bo, dst_dx, dst_dy,
		     box, n, flags,
		     &extents)) {
		bool big = too_large(extents.x2-extents.x1, extents.y2-extents.y1);

		if ((big || can_switch_to_blt(sna, dst_bo, flags)) &&
		    sna_blt_copy_boxes(sna, alu,
				       src_bo, src_dx, src_dy,
				       dst_bo, dst_dx, dst_dy,
				       dst->bitsPerPixel,
				       box, n))
			return true;

		if (big)
			goto fallback_blt;

		assert(src_bo == dst_bo);
		assert(src->depth == dst->depth);
		assert(src->width == dst->width);
		assert(src->height == dst->height);
		return sna_render_copy_boxes__overlap(sna, alu,
						      src, src_bo,
						      src_dx, src_dy,
						      dst_dx, dst_dy,
						      box, n, &extents);
	}

	if (dst->depth == src->depth) {
		tmp.dst.format = sna_render_format_for_depth(dst->depth);
		tmp.src.pict_format = tmp.dst.format;
	} else {
		tmp.dst.format = sna_format_for_depth(dst->depth);
		tmp.src.pict_format = sna_format_for_depth(src->depth);
	}
	if (!gen8_check_format(tmp.src.pict_format))
		goto fallback_blt;

	tmp.dst.pixmap = (PixmapPtr)dst;
	tmp.dst.width  = dst->width;
	tmp.dst.height = dst->height;
	tmp.dst.bo = dst_bo;
	tmp.dst.x = tmp.dst.y = 0;
	tmp.damage = NULL;

	sna_render_composite_redirect_init(&tmp);
	if (too_large(tmp.dst.width, tmp.dst.height)) {
		int i;

		extents = box[0];
		for (i = 1; i < n; i++) {
			if (box[i].x1 < extents.x1)
				extents.x1 = box[i].x1;
			if (box[i].y1 < extents.y1)
				extents.y1 = box[i].y1;

			if (box[i].x2 > extents.x2)
				extents.x2 = box[i].x2;
			if (box[i].y2 > extents.y2)
				extents.y2 = box[i].y2;
		}

		if (!sna_render_composite_redirect(sna, &tmp,
						   extents.x1 + dst_dx,
						   extents.y1 + dst_dy,
						   extents.x2 - extents.x1,
						   extents.y2 - extents.y1,
						   n > 1))
			goto fallback_tiled;
	}

	tmp.src.card_format = gen8_get_card_format(tmp.src.pict_format);
	if (too_large(src->width, src->height)) {
		int i;

		extents = box[0];
		for (i = 1; i < n; i++) {
			if (box[i].x1 < extents.x1)
				extents.x1 = box[i].x1;
			if (box[i].y1 < extents.y1)
				extents.y1 = box[i].y1;

			if (box[i].x2 > extents.x2)
				extents.x2 = box[i].x2;
			if (box[i].y2 > extents.y2)
				extents.y2 = box[i].y2;
		}

		if (!sna_render_pixmap_partial(sna, src, src_bo, &tmp.src,
					       extents.x1 + src_dx,
					       extents.y1 + src_dy,
					       extents.x2 - extents.x1,
					       extents.y2 - extents.y1))
			goto fallback_tiled_dst;
	} else {
		tmp.src.bo = src_bo;
		tmp.src.width  = src->width;
		tmp.src.height = src->height;
		tmp.src.offset[0] = tmp.src.offset[1] = 0;
	}

	tmp.mask.bo = NULL;

	tmp.floats_per_vertex = 2;
	tmp.floats_per_rect = 6;
	tmp.need_magic_ca_pass = 0;

	tmp.u.gen8.flags = COPY_FLAGS(alu);

	kgem_set_mode(&sna->kgem, KGEM_RENDER, tmp.dst.bo);
	if (!kgem_check_bo(&sna->kgem, tmp.dst.bo, tmp.src.bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem, tmp.dst.bo, tmp.src.bo, NULL)) {
			if (tmp.src.bo != src_bo)
				kgem_bo_destroy(&sna->kgem, tmp.src.bo);
			if (tmp.redirect.real_bo)
				kgem_bo_destroy(&sna->kgem, tmp.dst.bo);
			goto fallback_blt;
		}
		_kgem_set_mode(&sna->kgem, KGEM_RENDER);
	}

	src_dx += tmp.src.offset[0];
	src_dy += tmp.src.offset[1];

	dst_dx += tmp.dst.x;
	dst_dy += tmp.dst.y;

	tmp.dst.x = tmp.dst.y = 0;

	gen8_align_vertex(sna, &tmp);
	gen8_emit_copy_state(sna, &tmp);

	do {
		int16_t *v;
		int n_this_time;

		n_this_time = gen8_get_rectangles(sna, &tmp, n,
						  gen8_emit_copy_state);
		n -= n_this_time;

		v = (int16_t *)(sna->render.vertices + sna->render.vertex_used);
		sna->render.vertex_used += 6 * n_this_time;
		assert(sna->render.vertex_used <= sna->render.vertex_size);
		do {

			DBG(("	(%d, %d) -> (%d, %d) + (%d, %d)\n",
			     box->x1 + src_dx, box->y1 + src_dy,
			     box->x1 + dst_dx, box->y1 + dst_dy,
			     box->x2 - box->x1, box->y2 - box->y1));
			v[0] = box->x2 + dst_dx;
			v[2] = box->x2 + src_dx;
			v[1]  = v[5] = box->y2 + dst_dy;
			v[3]  = v[7] = box->y2 + src_dy;
			v[8]  = v[4] = box->x1 + dst_dx;
			v[10] = v[6] = box->x1 + src_dx;
			v[9]  = box->y1 + dst_dy;
			v[11] = box->y1 + src_dy;
			v += 12; box++;
		} while (--n_this_time);
	} while (n);

	gen8_vertex_flush(sna);
	sna_render_composite_redirect_done(sna, &tmp);
	if (tmp.src.bo != src_bo)
		kgem_bo_destroy(&sna->kgem, tmp.src.bo);
	return true;

fallback_tiled_dst:
	if (tmp.redirect.real_bo)
		kgem_bo_destroy(&sna->kgem, tmp.dst.bo);
fallback_tiled:
	DBG(("%s: fallback tiled\n", __FUNCTION__));
	if (sna_blt_compare_depth(src, dst) &&
	    sna_blt_copy_boxes(sna, alu,
			       src_bo, src_dx, src_dy,
			       dst_bo, dst_dx, dst_dy,
			       dst->bitsPerPixel,
			       box, n))
		return true;

	return sna_tiling_copy_boxes(sna, alu,
				     src, src_bo, src_dx, src_dy,
				     dst, dst_bo, dst_dx, dst_dy,
				     box, n);
}

static void
gen8_render_copy_blt(struct sna *sna,
		     const struct sna_copy_op *op,
		     int16_t sx, int16_t sy,
		     int16_t w,  int16_t h,
		     int16_t dx, int16_t dy)
{
	int16_t *v;

	gen8_get_rectangles(sna, &op->base, 1, gen8_emit_copy_state);

	v = (int16_t *)&sna->render.vertices[sna->render.vertex_used];
	sna->render.vertex_used += 6;
	assert(sna->render.vertex_used <= sna->render.vertex_size);

	v[0]  = dx+w; v[1]  = dy+h;
	v[2]  = sx+w; v[3]  = sy+h;
	v[4]  = dx;   v[5]  = dy+h;
	v[6]  = sx;   v[7]  = sy+h;
	v[8]  = dx;   v[9]  = dy;
	v[10] = sx;   v[11] = sy;
}

static void
gen8_render_copy_done(struct sna *sna, const struct sna_copy_op *op)
{
	if (sna->render.vertex_offset)
		gen8_vertex_flush(sna);
}

static bool
gen8_render_copy(struct sna *sna, uint8_t alu,
		 PixmapPtr src, struct kgem_bo *src_bo,
		 PixmapPtr dst, struct kgem_bo *dst_bo,
		 struct sna_copy_op *op)
{
	DBG(("%s (alu=%d, src=(%dx%d), dst=(%dx%d))\n",
	     __FUNCTION__, alu,
	     src->drawable.width, src->drawable.height,
	     dst->drawable.width, dst->drawable.height));

	if (prefer_blt_copy(sna, src_bo, dst_bo, 0) &&
	    sna_blt_compare_depth(&src->drawable, &dst->drawable) &&
	    sna_blt_copy(sna, alu,
			 src_bo, dst_bo,
			 dst->drawable.bitsPerPixel,
			 op))
		return true;

	if (!(alu == GXcopy || alu == GXclear) || src_bo == dst_bo ||
	    too_large(src->drawable.width, src->drawable.height) ||
	    too_large(dst->drawable.width, dst->drawable.height) ||
	    unaligned(src_bo, src->drawable.bitsPerPixel) ||
	    unaligned(dst_bo, dst->drawable.bitsPerPixel)) {
fallback:
		if (!sna_blt_compare_depth(&src->drawable, &dst->drawable))
			return false;

		return sna_blt_copy(sna, alu, src_bo, dst_bo,
				    dst->drawable.bitsPerPixel,
				    op);
	}

	if (dst->drawable.depth == src->drawable.depth) {
		op->base.dst.format = sna_render_format_for_depth(dst->drawable.depth);
		op->base.src.pict_format = op->base.dst.format;
	} else {
		op->base.dst.format = sna_format_for_depth(dst->drawable.depth);
		op->base.src.pict_format = sna_format_for_depth(src->drawable.depth);
	}
	if (!gen8_check_format(op->base.src.pict_format))
		goto fallback;

	op->base.dst.pixmap = dst;
	op->base.dst.width  = dst->drawable.width;
	op->base.dst.height = dst->drawable.height;
	op->base.dst.bo = dst_bo;

	op->base.src.bo = src_bo;
	op->base.src.card_format =
		gen8_get_card_format(op->base.src.pict_format);
	op->base.src.width  = src->drawable.width;
	op->base.src.height = src->drawable.height;

	op->base.mask.bo = NULL;

	op->base.floats_per_vertex = 2;
	op->base.floats_per_rect = 6;

	op->base.u.gen8.flags = COPY_FLAGS(alu);

	kgem_set_mode(&sna->kgem, KGEM_RENDER, dst_bo);
	if (!kgem_check_bo(&sna->kgem, dst_bo, src_bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem, dst_bo, src_bo, NULL))
			goto fallback;
		_kgem_set_mode(&sna->kgem, KGEM_RENDER);
	}

	gen8_align_vertex(sna, &op->base);
	gen8_emit_copy_state(sna, &op->base);

	op->blt  = gen8_render_copy_blt;
	op->done = gen8_render_copy_done;
	return true;
}

static void
gen8_emit_fill_state(struct sna *sna, const struct sna_composite_op *op)
{
	uint32_t *binding_table;
	uint16_t offset, dirty;

	/* XXX Render Target Fast Clear
	 * Set RTFC Enable in PS and render a rectangle.
	 * Limited to a clearing the full MSC surface only with a
	 * specific kernel.
	 */

	gen8_get_batch(sna, op);

	binding_table = gen8_composite_get_binding_table(sna, &offset);

	dirty = kgem_bo_is_dirty(op->dst.bo);

	binding_table[0] =
		gen8_bind_bo(sna,
			     op->dst.bo, op->dst.width, op->dst.height,
			     gen8_get_dest_format(op->dst.format),
			     true);
	binding_table[1] =
		gen8_bind_bo(sna,
			     op->src.bo, 1, 1,
			     SURFACEFORMAT_B8G8R8A8_UNORM,
			     false);

	if (sna->kgem.surface == offset &&
	    *(uint64_t *)(sna->kgem.batch + sna->render_state.gen8.surface_table) == *(uint64_t*)binding_table) {
		sna->kgem.surface += SURFACE_DW;
		offset = sna->render_state.gen8.surface_table;
	}

	if (sna->kgem.batch[sna->render_state.gen8.surface_table] == binding_table[0])
		dirty = 0;

	gen8_emit_state(sna, op, offset | dirty);
}

static bool
gen8_render_fill_boxes(struct sna *sna,
		       CARD8 op,
		       PictFormat format,
		       const xRenderColor *color,
		       const DrawableRec *dst, struct kgem_bo *dst_bo,
		       const BoxRec *box, int n)
{
	struct sna_composite_op tmp;
	uint32_t pixel;

	DBG(("%s (op=%d, color=(%04x, %04x, %04x, %04x) [%08x])\n",
	     __FUNCTION__, op,
	     color->red, color->green, color->blue, color->alpha, (int)format));

	if (op >= ARRAY_SIZE(gen8_blend_op)) {
		DBG(("%s: fallback due to unhandled blend op: %d\n",
		     __FUNCTION__, op));
		return false;
	}

	if (prefer_blt_fill(sna, dst_bo, FILL_BOXES) ||
	    !gen8_check_dst_format(format) ||
	    unaligned(dst_bo, PICT_FORMAT_BPP(format))) {
		uint8_t alu = GXinvalid;

		if (op <= PictOpSrc) {
			pixel = 0;
			if (op == PictOpClear)
				alu = GXclear;
			else if (sna_get_pixel_from_rgba(&pixel,
							 color->red,
							 color->green,
							 color->blue,
							 color->alpha,
							 format))
				alu = GXcopy;
		}

		if (alu != GXinvalid &&
		    sna_blt_fill_boxes(sna, alu,
				       dst_bo, dst->bitsPerPixel,
				       pixel, box, n))
			return true;

		if (!gen8_check_dst_format(format))
			return false;
	}

	if (op == PictOpClear) {
		pixel = 0;
		op = PictOpSrc;
	} else if (!sna_get_pixel_from_rgba(&pixel,
					    color->red,
					    color->green,
					    color->blue,
					    color->alpha,
					    PICT_a8r8g8b8))
		return false;

	DBG(("%s(%08x x %d [(%d, %d), (%d, %d) ...])\n",
	     __FUNCTION__, pixel, n,
	     box[0].x1, box[0].y1, box[0].x2, box[0].y2));

	tmp.dst.pixmap = (PixmapPtr)dst;
	tmp.dst.width  = dst->width;
	tmp.dst.height = dst->height;
	tmp.dst.format = format;
	tmp.dst.bo = dst_bo;
	tmp.dst.x = tmp.dst.y = 0;
	tmp.damage = NULL;

	sna_render_composite_redirect_init(&tmp);
	if (too_large(dst->width, dst->height)) {
		BoxRec extents;

		boxes_extents(box, n, &extents);
		if (!sna_render_composite_redirect(sna, &tmp,
						   extents.x1, extents.y1,
						   extents.x2 - extents.x1,
						   extents.y2 - extents.y1,
						   n > 1))
			return sna_tiling_fill_boxes(sna, op, format, color,
						     dst, dst_bo, box, n);
	}

	tmp.src.bo = sna_render_get_solid(sna, pixel);
	tmp.mask.bo = NULL;

	tmp.floats_per_vertex = 2;
	tmp.floats_per_rect = 6;
	tmp.need_magic_ca_pass = false;

	tmp.u.gen8.flags = FILL_FLAGS(op, format);

	kgem_set_mode(&sna->kgem, KGEM_RENDER, dst_bo);
	if (!kgem_check_bo(&sna->kgem, dst_bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem, dst_bo, NULL)) {
			kgem_bo_destroy(&sna->kgem, tmp.src.bo);
			tmp.src.bo = NULL;

			if (tmp.redirect.real_bo) {
				kgem_bo_destroy(&sna->kgem, tmp.dst.bo);
				tmp.redirect.real_bo = NULL;
			}

			return false;
		}
		_kgem_set_mode(&sna->kgem, KGEM_RENDER);
	}

	gen8_align_vertex(sna, &tmp);
	gen8_emit_fill_state(sna, &tmp);

	do {
		int n_this_time;
		int16_t *v;

		n_this_time = gen8_get_rectangles(sna, &tmp, n,
						  gen8_emit_fill_state);
		n -= n_this_time;

		v = (int16_t *)(sna->render.vertices + sna->render.vertex_used);
		sna->render.vertex_used += 6 * n_this_time;
		assert(sna->render.vertex_used <= sna->render.vertex_size);
		do {
			DBG(("	(%d, %d), (%d, %d)\n",
			     box->x1, box->y1, box->x2, box->y2));

			v[0] = box->x2;
			v[5] = v[1] = box->y2;
			v[8] = v[4] = box->x1;
			v[9] = box->y1;
			v[2] = v[3]  = v[7]  = 1;
			v[6] = v[10] = v[11] = 0;
			v += 12; box++;
		} while (--n_this_time);
	} while (n);

	gen8_vertex_flush(sna);
	kgem_bo_destroy(&sna->kgem, tmp.src.bo);
	sna_render_composite_redirect_done(sna, &tmp);
	return true;
}

static void
gen8_render_fill_op_blt(struct sna *sna,
			const struct sna_fill_op *op,
			int16_t x, int16_t y, int16_t w, int16_t h)
{
	int16_t *v;

	DBG(("%s: (%d, %d)x(%d, %d)\n", __FUNCTION__, x, y, w, h));

	gen8_get_rectangles(sna, &op->base, 1, gen8_emit_fill_state);

	v = (int16_t *)&sna->render.vertices[sna->render.vertex_used];
	sna->render.vertex_used += 6;
	assert(sna->render.vertex_used <= sna->render.vertex_size);

	v[0] = x+w;
	v[4] = v[8] = x;
	v[1] = v[5] = y+h;
	v[9] = y;

	v[2] = v[3]  = v[7]  = 1;
	v[6] = v[10] = v[11] = 0;
}

fastcall static void
gen8_render_fill_op_box(struct sna *sna,
			const struct sna_fill_op *op,
			const BoxRec *box)
{
	int16_t *v;

	DBG(("%s: (%d, %d),(%d, %d)\n", __FUNCTION__,
	     box->x1, box->y1, box->x2, box->y2));

	gen8_get_rectangles(sna, &op->base, 1, gen8_emit_fill_state);

	v = (int16_t *)&sna->render.vertices[sna->render.vertex_used];
	sna->render.vertex_used += 6;
	assert(sna->render.vertex_used <= sna->render.vertex_size);

	v[0] = box->x2;
	v[8] = v[4] = box->x1;
	v[5] = v[1] = box->y2;
	v[9] = box->y1;

	v[7] = v[2]  = v[3]  = 1;
	v[6] = v[10] = v[11] = 0;
}

fastcall static void
gen8_render_fill_op_boxes(struct sna *sna,
			  const struct sna_fill_op *op,
			  const BoxRec *box,
			  int nbox)
{
	DBG(("%s: (%d, %d),(%d, %d)... x %d\n", __FUNCTION__,
	     box->x1, box->y1, box->x2, box->y2, nbox));

	do {
		int nbox_this_time;
		int16_t *v;

		nbox_this_time = gen8_get_rectangles(sna, &op->base, nbox,
						     gen8_emit_fill_state);
		nbox -= nbox_this_time;

		v = (int16_t *)&sna->render.vertices[sna->render.vertex_used];
		sna->render.vertex_used += 6 * nbox_this_time;
		assert(sna->render.vertex_used <= sna->render.vertex_size);

		do {
			v[0] = box->x2;
			v[8] = v[4] = box->x1;
			v[5] = v[1] = box->y2;
			v[9] = box->y1;
			v[7] = v[2]  = v[3]  = 1;
			v[6] = v[10] = v[11] = 0;
			box++; v += 12;
		} while (--nbox_this_time);
	} while (nbox);
}

static void
gen8_render_fill_op_done(struct sna *sna, const struct sna_fill_op *op)
{
	if (sna->render.vertex_offset)
		gen8_vertex_flush(sna);
	kgem_bo_destroy(&sna->kgem, op->base.src.bo);
}

static bool
gen8_render_fill(struct sna *sna, uint8_t alu,
		 PixmapPtr dst, struct kgem_bo *dst_bo,
		 uint32_t color, unsigned flags,
		 struct sna_fill_op *op)
{
	DBG(("%s: (alu=%d, color=%x)\n", __FUNCTION__, alu, color));

	if (prefer_blt_fill(sna, dst_bo, flags) &&
	    sna_blt_fill(sna, alu,
			 dst_bo, dst->drawable.bitsPerPixel,
			 color,
			 op))
		return true;

	if (!(alu == GXcopy || alu == GXclear) ||
	    too_large(dst->drawable.width, dst->drawable.height) ||
	    unaligned(dst_bo, dst->drawable.bitsPerPixel))
		return sna_blt_fill(sna, alu,
				    dst_bo, dst->drawable.bitsPerPixel,
				    color,
				    op);

	if (alu == GXclear)
		color = 0;

	op->base.dst.pixmap = dst;
	op->base.dst.width  = dst->drawable.width;
	op->base.dst.height = dst->drawable.height;
	op->base.dst.format = sna_format_for_depth(dst->drawable.depth);
	op->base.dst.bo = dst_bo;
	op->base.dst.x = op->base.dst.y = 0;

	op->base.src.bo =
		sna_render_get_solid(sna,
				     sna_rgba_for_color(color,
							dst->drawable.depth));
	op->base.mask.bo = NULL;

	op->base.need_magic_ca_pass = false;
	op->base.floats_per_vertex = 2;
	op->base.floats_per_rect = 6;

	op->base.u.gen8.flags = FILL_FLAGS_NOBLEND;

	kgem_set_mode(&sna->kgem, KGEM_RENDER, dst_bo);
	if (!kgem_check_bo(&sna->kgem, dst_bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem, dst_bo, NULL)) {
			kgem_bo_destroy(&sna->kgem, op->base.src.bo);
			return false;
		}

		_kgem_set_mode(&sna->kgem, KGEM_RENDER);
	}

	gen8_align_vertex(sna, &op->base);
	gen8_emit_fill_state(sna, &op->base);

	op->blt   = gen8_render_fill_op_blt;
	op->box   = gen8_render_fill_op_box;
	op->boxes = gen8_render_fill_op_boxes;
	op->points = NULL;
	op->done  = gen8_render_fill_op_done;
	return true;
}

static bool
gen8_render_fill_one_try_blt(struct sna *sna, PixmapPtr dst, struct kgem_bo *bo,
			     uint32_t color,
			     int16_t x1, int16_t y1, int16_t x2, int16_t y2,
			     uint8_t alu)
{
	BoxRec box;

	box.x1 = x1;
	box.y1 = y1;
	box.x2 = x2;
	box.y2 = y2;

	return sna_blt_fill_boxes(sna, alu,
				  bo, dst->drawable.bitsPerPixel,
				  color, &box, 1);
}

static bool
gen8_render_fill_one(struct sna *sna, PixmapPtr dst, struct kgem_bo *bo,
		     uint32_t color,
		     int16_t x1, int16_t y1,
		     int16_t x2, int16_t y2,
		     uint8_t alu)
{
	struct sna_composite_op tmp;
	int16_t *v;

	/* Prefer to use the BLT if already engaged */
	if (prefer_blt_fill(sna, bo, FILL_BOXES) &&
	    gen8_render_fill_one_try_blt(sna, dst, bo, color,
					 x1, y1, x2, y2, alu))
		return true;

	/* Must use the BLT if we can't RENDER... */
	if (!(alu == GXcopy || alu == GXclear) ||
	    too_large(dst->drawable.width, dst->drawable.height) ||
	    unaligned(bo, dst->drawable.bitsPerPixel))
		return gen8_render_fill_one_try_blt(sna, dst, bo, color,
						    x1, y1, x2, y2, alu);

	if (alu == GXclear)
		color = 0;

	tmp.dst.pixmap = dst;
	tmp.dst.width  = dst->drawable.width;
	tmp.dst.height = dst->drawable.height;
	tmp.dst.format = sna_format_for_depth(dst->drawable.depth);
	tmp.dst.bo = bo;
	tmp.dst.x = tmp.dst.y = 0;

	tmp.src.bo =
		sna_render_get_solid(sna,
				     sna_rgba_for_color(color,
							dst->drawable.depth));
	tmp.mask.bo = NULL;

	tmp.floats_per_vertex = 2;
	tmp.floats_per_rect = 6;
	tmp.need_magic_ca_pass = false;

	tmp.u.gen8.flags = FILL_FLAGS_NOBLEND;

	kgem_set_mode(&sna->kgem, KGEM_RENDER, bo);
	if (!kgem_check_bo(&sna->kgem, bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (kgem_check_bo(&sna->kgem, bo, NULL)) {
			kgem_bo_destroy(&sna->kgem, tmp.src.bo);
			return false;
		}
		_kgem_set_mode(&sna->kgem, KGEM_RENDER);
	}

	gen8_align_vertex(sna, &tmp);
	gen8_emit_fill_state(sna, &tmp);

	gen8_get_rectangles(sna, &tmp, 1, gen8_emit_fill_state);

	DBG(("	(%d, %d), (%d, %d)\n", x1, y1, x2, y2));

	v = (int16_t *)&sna->render.vertices[sna->render.vertex_used];
	sna->render.vertex_used += 6;
	assert(sna->render.vertex_used <= sna->render.vertex_size);

	v[0] = x2;
	v[8] = v[4] = x1;
	v[5] = v[1] = y2;
	v[9] = y1;
	v[7] = v[2]  = v[3]  = 1;
	v[6] = v[10] = v[11] = 0;

	gen8_vertex_flush(sna);
	kgem_bo_destroy(&sna->kgem, tmp.src.bo);

	return true;
}

static bool
gen8_render_clear_try_blt(struct sna *sna, PixmapPtr dst, struct kgem_bo *bo)
{
	BoxRec box;

	box.x1 = 0;
	box.y1 = 0;
	box.x2 = dst->drawable.width;
	box.y2 = dst->drawable.height;

	return sna_blt_fill_boxes(sna, GXclear,
				  bo, dst->drawable.bitsPerPixel,
				  0, &box, 1);
}

static bool
gen8_render_clear(struct sna *sna, PixmapPtr dst, struct kgem_bo *bo)
{
	struct sna_composite_op tmp;
	int16_t *v;

	DBG(("%s: %dx%d\n",
	     __FUNCTION__,
	     dst->drawable.width,
	     dst->drawable.height));

	/* Prefer to use the BLT if already engaged */
	if (sna->kgem.mode == KGEM_BLT &&
	    gen8_render_clear_try_blt(sna, dst, bo))
		return true;

	/* Must use the BLT if we can't RENDER... */
	if (too_large(dst->drawable.width, dst->drawable.height) ||
	    unaligned(bo, dst->drawable.bitsPerPixel))
		return gen8_render_clear_try_blt(sna, dst, bo);

	tmp.dst.pixmap = dst;
	tmp.dst.width  = dst->drawable.width;
	tmp.dst.height = dst->drawable.height;
	tmp.dst.format = sna_format_for_depth(dst->drawable.depth);
	tmp.dst.bo = bo;
	tmp.dst.x = tmp.dst.y = 0;

	tmp.src.bo = sna_render_get_solid(sna, 0);
	tmp.mask.bo = NULL;

	tmp.floats_per_vertex = 2;
	tmp.floats_per_rect = 6;
	tmp.need_magic_ca_pass = false;

	tmp.u.gen8.flags = FILL_FLAGS_NOBLEND;

	kgem_set_mode(&sna->kgem, KGEM_RENDER, bo);
	if (!kgem_check_bo(&sna->kgem, bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem, bo, NULL)) {
			kgem_bo_destroy(&sna->kgem, tmp.src.bo);
			return false;
		}
		_kgem_set_mode(&sna->kgem, KGEM_RENDER);
	}

	gen8_align_vertex(sna, &tmp);
	gen8_emit_fill_state(sna, &tmp);

	gen8_get_rectangles(sna, &tmp, 1, gen8_emit_fill_state);

	v = (int16_t *)&sna->render.vertices[sna->render.vertex_used];
	sna->render.vertex_used += 6;
	assert(sna->render.vertex_used <= sna->render.vertex_size);

	v[0] = dst->drawable.width;
	v[5] = v[1] = dst->drawable.height;
	v[8] = v[4] = 0;
	v[9] = 0;

	v[7] = v[2]  = v[3]  = 1;
	v[6] = v[10] = v[11] = 0;

	gen8_vertex_flush(sna);
	kgem_bo_destroy(&sna->kgem, tmp.src.bo);

	return true;
}

#if !NO_VIDEO
static uint32_t gen8_bind_video_source(struct sna *sna,
				       struct kgem_bo *bo,
				       uint32_t delta,
				       int width,
				       int height,
				       int pitch,
				       uint32_t format)
{
	uint32_t *ss;
	int offset;

	offset = sna->kgem.surface -= SURFACE_DW;
	ss = sna->kgem.batch + offset;
	ss[0] = (SURFACE_2D << SURFACE_TYPE_SHIFT |
		 gen8_tiling_bits(bo->tiling) |
		 format << SURFACE_FORMAT_SHIFT |
		 SURFACE_VALIGN_4 | SURFACE_HALIGN_4);
	ss[1] = 0;
	ss[2] = ((width - 1)  << SURFACE_WIDTH_SHIFT |
		 (height - 1) << SURFACE_HEIGHT_SHIFT);
	ss[3] = (pitch - 1) << SURFACE_PITCH_SHIFT;
	ss[4] = 0;
	ss[5] = 0;
	ss[6] = 0;
	ss[7] = SURFACE_SWIZZLE(RED, GREEN, BLUE, ALPHA);
	*(uint64_t *)(ss+8) =
		kgem_add_reloc64(&sna->kgem, offset + 8, bo,
				 I915_GEM_DOMAIN_SAMPLER << 16,
				 delta);
	ss[10] = 0;
	ss[11] = 0;
	ss[12] = 0;
	ss[13] = 0;
	ss[14] = 0;
	ss[15] = 0;

	DBG(("[%x] bind bo(handle=%d, addr=%d), format=%d, width=%d, height=%d, pitch=%d, tiling=%d -> sampler\n",
	     offset, bo->handle, ss[1],
	     format, width, height, bo->pitch, bo->tiling));

	return offset * sizeof(uint32_t);
}

static void gen8_emit_video_state(struct sna *sna,
				  const struct sna_composite_op *op)
{
	struct sna_video_frame *frame = op->priv;
	uint32_t src_surf_format;
	uint32_t src_surf_base[6];
	int src_width[6];
	int src_height[6];
	int src_pitch[6];
	uint32_t *binding_table;
	uint16_t offset;
	int n_src, n;

	/* XXX VeBox, bicubic */

	gen8_get_batch(sna, op);

	src_surf_base[0] = 0;
	src_surf_base[1] = 0;
	src_surf_base[2] = frame->VBufOffset;
	src_surf_base[3] = frame->VBufOffset;
	src_surf_base[4] = frame->UBufOffset;
	src_surf_base[5] = frame->UBufOffset;

	if (is_planar_fourcc(frame->id)) {
		src_surf_format = SURFACEFORMAT_R8_UNORM;
		src_width[1]  = src_width[0]  = frame->width;
		src_height[1] = src_height[0] = frame->height;
		src_pitch[1]  = src_pitch[0]  = frame->pitch[1];
		src_width[4]  = src_width[5]  = src_width[2]  = src_width[3] =
			frame->width / 2;
		src_height[4] = src_height[5] = src_height[2] = src_height[3] =
			frame->height / 2;
		src_pitch[4]  = src_pitch[5]  = src_pitch[2]  = src_pitch[3] =
			frame->pitch[0];
		n_src = 6;
	} else {
		if (frame->id == FOURCC_UYVY)
			src_surf_format = SURFACEFORMAT_YCRCB_SWAPY;
		else
			src_surf_format = SURFACEFORMAT_YCRCB_NORMAL;

		src_width[0]  = frame->width;
		src_height[0] = frame->height;
		src_pitch[0]  = frame->pitch[0];
		n_src = 1;
	}

	binding_table = gen8_composite_get_binding_table(sna, &offset);

	binding_table[0] =
		gen8_bind_bo(sna,
			     op->dst.bo, op->dst.width, op->dst.height,
			     gen8_get_dest_format(op->dst.format),
			     true);
	for (n = 0; n < n_src; n++) {
		binding_table[1+n] =
			gen8_bind_video_source(sna,
					       frame->bo,
					       src_surf_base[n],
					       src_width[n],
					       src_height[n],
					       src_pitch[n],
					       src_surf_format);
	}

	gen8_emit_state(sna, op, offset);
}

static bool
gen8_render_video(struct sna *sna,
		  struct sna_video *video,
		  struct sna_video_frame *frame,
		  RegionPtr dstRegion,
		  PixmapPtr pixmap)
{
	struct sna_composite_op tmp;
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	int dst_width = dstRegion->extents.x2 - dstRegion->extents.x1;
	int dst_height = dstRegion->extents.y2 - dstRegion->extents.y1;
	int src_width = frame->src.x2 - frame->src.x1;
	int src_height = frame->src.y2 - frame->src.y1;
	float src_offset_x, src_offset_y;
	float src_scale_x, src_scale_y;
	int nbox, pix_xoff, pix_yoff;
	unsigned filter;
	const BoxRec *box;

	DBG(("%s: src=(%d, %d), dst=(%d, %d), %dx[(%d, %d), (%d, %d)...]\n",
	     __FUNCTION__,
	     src_width, src_height, dst_width, dst_height,
	     region_num_rects(dstRegion),
	     REGION_EXTENTS(NULL, dstRegion)->x1,
	     REGION_EXTENTS(NULL, dstRegion)->y1,
	     REGION_EXTENTS(NULL, dstRegion)->x2,
	     REGION_EXTENTS(NULL, dstRegion)->y2));

	assert(priv->gpu_bo);
	assert(!too_large(pixmap->drawable.width, pixmap->drawable.height));
	assert(!unaligned(priv->gpu_bo, pixmap->drawable.bitsPerPixel));

	memset(&tmp, 0, sizeof(tmp));

	tmp.dst.pixmap = pixmap;
	tmp.dst.width  = pixmap->drawable.width;
	tmp.dst.height = pixmap->drawable.height;
	tmp.dst.format = sna_render_format_for_depth(pixmap->drawable.depth);
	tmp.dst.bo = priv->gpu_bo;

	tmp.src.bo = frame->bo;
	tmp.mask.bo = NULL;

	tmp.floats_per_vertex = 3;
	tmp.floats_per_rect = 9;

	if (src_width == dst_width && src_height == dst_height)
		filter = SAMPLER_FILTER_NEAREST;
	else
		filter = SAMPLER_FILTER_BILINEAR;

	tmp.u.gen8.flags =
		GEN8_SET_FLAGS(SAMPLER_OFFSET(filter, SAMPLER_EXTEND_PAD,
					      SAMPLER_FILTER_NEAREST, SAMPLER_EXTEND_NONE),
			       NO_BLEND,
			       is_planar_fourcc(frame->id) ?
			       GEN8_WM_KERNEL_VIDEO_PLANAR :
			       GEN8_WM_KERNEL_VIDEO_PACKED,
			       2);
	tmp.priv = frame;

	kgem_set_mode(&sna->kgem, KGEM_RENDER, tmp.dst.bo);
	if (!kgem_check_bo(&sna->kgem, tmp.dst.bo, frame->bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem, tmp.dst.bo, frame->bo, NULL))
			return false;

		_kgem_set_mode(&sna->kgem, KGEM_RENDER);
	}

	gen8_align_vertex(sna, &tmp);
	gen8_emit_video_state(sna, &tmp);

	/* Set up the offset for translating from the given region (in screen
	 * coordinates) to the backing pixmap.
	 */
#ifdef COMPOSITE
	pix_xoff = -pixmap->screen_x + pixmap->drawable.x;
	pix_yoff = -pixmap->screen_y + pixmap->drawable.y;
#else
	pix_xoff = 0;
	pix_yoff = 0;
#endif

	DBG(("%s: src=(%d, %d)x(%d, %d); frame=(%dx%d), dst=(%dx%d)\n",
	     __FUNCTION__,
	     frame->src.x1, frame->src.y1,
	     src_width, src_height,
	     dst_width, dst_height,
	     frame->width, frame->height));

	src_scale_x = (float)src_width / dst_width / frame->width;
	src_offset_x = (float)frame->src.x1 / frame->width - dstRegion->extents.x1 * src_scale_x;

	src_scale_y = (float)src_height / dst_height / frame->height;
	src_offset_y = (float)frame->src.y1 / frame->height - dstRegion->extents.y1 * src_scale_y;

	DBG(("%s: scale=(%f, %f), offset=(%f, %f)\n",
	     __FUNCTION__,
	     src_scale_x, src_scale_y,
	     src_offset_x, src_offset_y));

	box = region_rects(dstRegion);
	nbox = region_num_rects(dstRegion);
	while (nbox--) {
		BoxRec r;

		DBG(("%s: dst=(%d, %d), (%d, %d) + (%d, %d); src=(%f, %f), (%f, %f)\n",
		     __FUNCTION__,
		     box->x1, box->y1,
		     box->x2, box->y2,
		     pix_xoff, pix_yoff,
		     box->x1 * src_scale_x + src_offset_x,
		     box->y1 * src_scale_y + src_offset_y,
		     box->x2 * src_scale_x + src_offset_x,
		     box->y2 * src_scale_y + src_offset_y));

		r.x1 = box->x1 + pix_xoff;
		r.x2 = box->x2 + pix_xoff;
		r.y1 = box->y1 + pix_yoff;
		r.y2 = box->y2 + pix_yoff;

		gen8_get_rectangles(sna, &tmp, 1, gen8_emit_video_state);

		OUT_VERTEX(r.x2, r.y2);
		OUT_VERTEX_F(box->x2 * src_scale_x + src_offset_x);
		OUT_VERTEX_F(box->y2 * src_scale_y + src_offset_y);

		OUT_VERTEX(r.x1, r.y2);
		OUT_VERTEX_F(box->x1 * src_scale_x + src_offset_x);
		OUT_VERTEX_F(box->y2 * src_scale_y + src_offset_y);

		OUT_VERTEX(r.x1, r.y1);
		OUT_VERTEX_F(box->x1 * src_scale_x + src_offset_x);
		OUT_VERTEX_F(box->y1 * src_scale_y + src_offset_y);

		if (!DAMAGE_IS_ALL(priv->gpu_damage)) {
			sna_damage_add_box(&priv->gpu_damage, &r);
			sna_damage_subtract_box(&priv->cpu_damage, &r);
		}
		box++;
	}

	gen8_vertex_flush(sna);
	return true;
}
#endif

static void gen8_render_flush(struct sna *sna)
{
	gen8_vertex_close(sna);

	assert(sna->render.vb_id == 0);
	assert(sna->render.vertex_offset == 0);
}

static void gen8_render_reset(struct sna *sna)
{
	sna->render_state.gen8.emit_flush = false;
	sna->render_state.gen8.needs_invariant = true;
	sna->render_state.gen8.ve_id = 3 << 2;
	sna->render_state.gen8.last_primitive = -1;

	sna->render_state.gen8.num_sf_outputs = 0;
	sna->render_state.gen8.samplers = -1;
	sna->render_state.gen8.blend = -1;
	sna->render_state.gen8.kernel = -1;
	sna->render_state.gen8.drawrect_offset = -1;
	sna->render_state.gen8.drawrect_limit = -1;
	sna->render_state.gen8.surface_table = 0;

	if (sna->render.vbo && !kgem_bo_can_map(&sna->kgem, sna->render.vbo)) {
		DBG(("%s: discarding unmappable vbo\n", __FUNCTION__));
		discard_vbo(sna);
	}

	sna->render.vertex_offset = 0;
	sna->render.nvertex_reloc = 0;
	sna->render.vb_id = 0;
}

static void gen8_render_fini(struct sna *sna)
{
	kgem_bo_destroy(&sna->kgem, sna->render_state.gen8.general_bo);
}

static bool gen8_render_setup(struct sna *sna)
{
	struct gen8_render_state *state = &sna->render_state.gen8;
	struct sna_static_stream general;
	struct gen8_sampler_state *ss;
	int i, j, k, l, m;
	uint32_t devid;

	devid = intel_get_device_id(sna->scrn);
	if (devid & 0xf)
		state->gt = ((devid >> 4) & 0xf) + 1;
	DBG(("%s: gt=%d\n", __FUNCTION__, state->gt));

	sna_static_stream_init(&general);

	/* Zero pad the start. If you see an offset of 0x0 in the batchbuffer
	 * dumps, you know it points to zero.
	 */
	null_create(&general);

	for (m = 0; m < ARRAY_SIZE(wm_kernels); m++) {
		if (wm_kernels[m].size) {
			state->wm_kernel[m][1] =
				sna_static_stream_add(&general,
						      wm_kernels[m].data,
						      wm_kernels[m].size,
						      64);
		} else {
			if (USE_8_PIXEL_DISPATCH) {
				state->wm_kernel[m][0] =
					sna_static_stream_compile_wm(sna, &general,
								     wm_kernels[m].data, 8);
			}

			if (USE_16_PIXEL_DISPATCH) {
				state->wm_kernel[m][1] =
					sna_static_stream_compile_wm(sna, &general,
								     wm_kernels[m].data, 16);
			}

			if (USE_32_PIXEL_DISPATCH) {
				state->wm_kernel[m][2] =
					sna_static_stream_compile_wm(sna, &general,
								     wm_kernels[m].data, 32);
			}
		}
		assert(state->wm_kernel[m][0]|state->wm_kernel[m][1]|state->wm_kernel[m][2]);
	}

	COMPILE_TIME_ASSERT(SAMPLER_OFFSET(FILTER_COUNT, EXTEND_COUNT, FILTER_COUNT, EXTEND_COUNT) <= 0x7ff);
	ss = sna_static_stream_map(&general,
				   2 * sizeof(*ss) *
				   (2 +
				    FILTER_COUNT * EXTEND_COUNT *
				    FILTER_COUNT * EXTEND_COUNT),
				   32);
	state->wm_state = sna_static_stream_offsetof(&general, ss);
	sampler_copy_init(ss); ss += 2;
	sampler_fill_init(ss); ss += 2;
	for (i = 0; i < FILTER_COUNT; i++) {
		for (j = 0; j < EXTEND_COUNT; j++) {
			for (k = 0; k < FILTER_COUNT; k++) {
				for (l = 0; l < EXTEND_COUNT; l++) {
					sampler_state_init(ss++, i, j);
					sampler_state_init(ss++, k, l);
				}
			}
		}
	}

	state->cc_blend = gen8_create_blend_state(&general);

	state->general_bo = sna_static_stream_fini(sna, &general);
	return state->general_bo != NULL;
}

const char *gen8_render_init(struct sna *sna, const char *backend)
{
	if (!gen8_render_setup(sna))
		return backend;

	sna->kgem.context_switch = gen6_render_context_switch;
	sna->kgem.retire = gen6_render_retire;
	sna->kgem.expire = gen4_render_expire;

#if !NO_COMPOSITE
	sna->render.composite = gen8_render_composite;
	sna->render.prefer_gpu |= PREFER_GPU_RENDER;
#endif
#if !NO_COMPOSITE_SPANS
	sna->render.check_composite_spans = gen8_check_composite_spans;
	sna->render.composite_spans = gen8_render_composite_spans;
	sna->render.prefer_gpu |= PREFER_GPU_SPANS;
#endif
#if !NO_VIDEO
	sna->render.video = gen8_render_video;
#endif

#if !NO_COPY_BOXES
	sna->render.copy_boxes = gen8_render_copy_boxes;
#endif
#if !NO_COPY
	sna->render.copy = gen8_render_copy;
#endif

#if !NO_FILL_BOXES
	sna->render.fill_boxes = gen8_render_fill_boxes;
#endif
#if !NO_FILL
	sna->render.fill = gen8_render_fill;
#endif
#if !NO_FILL_ONE
	sna->render.fill_one = gen8_render_fill_one;
#endif
#if !NO_FILL_CLEAR
	sna->render.clear = gen8_render_clear;
#endif

	sna->render.flush = gen8_render_flush;
	sna->render.reset = gen8_render_reset;
	sna->render.fini = gen8_render_fini;

	sna->render.max_3d_size = GEN8_MAX_SIZE;
	sna->render.max_3d_pitch = 1 << 18;
	return "Broadwell";
}
