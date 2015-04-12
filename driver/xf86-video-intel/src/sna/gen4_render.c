/*
 * Copyright © 2006,2008,2011 Intel Corporation
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
 *    Wang Zhenyu <zhenyu.z.wang@sna.com>
 *    Eric Anholt <eric@anholt.net>
 *    Carl Worth <cworth@redhat.com>
 *    Keith Packard <keithp@keithp.com>
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

#include "brw/brw.h"
#include "gen4_common.h"
#include "gen4_render.h"
#include "gen4_source.h"
#include "gen4_vertex.h"

/* gen4 has a serious issue with its shaders that we need to flush
 * after every rectangle... So until that is resolved, prefer
 * the BLT engine.
 */
#define FORCE_SPANS 0
#define FORCE_NONRECTILINEAR_SPANS -1
#define FORCE_FLUSH 1 /* https://bugs.freedesktop.org/show_bug.cgi?id=55500 */

#define ALWAYS_FLUSH 1

#define NO_COMPOSITE 0
#define NO_COMPOSITE_SPANS 0
#define NO_COPY 0
#define NO_COPY_BOXES 0
#define NO_FILL 0
#define NO_FILL_ONE 0
#define NO_FILL_BOXES 0
#define NO_VIDEO 0

#define MAX_FLUSH_VERTICES 1 /* was 6, https://bugs.freedesktop.org/show_bug.cgi?id=55500 */

#define GEN4_GRF_BLOCKS(nreg)    ((nreg + 15) / 16 - 1)

/* Set up a default static partitioning of the URB, which is supposed to
 * allow anything we would want to do, at potentially lower performance.
 */
#define URB_CS_ENTRY_SIZE     1
#define URB_CS_ENTRIES	      0

#define URB_VS_ENTRY_SIZE     1
#define URB_VS_ENTRIES	      32

#define URB_GS_ENTRY_SIZE     0
#define URB_GS_ENTRIES	      0

#define URB_CL_ENTRY_SIZE   0
#define URB_CL_ENTRIES      0

#define URB_SF_ENTRY_SIZE     2
#define URB_SF_ENTRIES	      64

/*
 * this program computes dA/dx and dA/dy for the texture coordinates along
 * with the base texture coordinate. It was extracted from the Mesa driver
 */

#define SF_KERNEL_NUM_GRF 16
#define PS_KERNEL_NUM_GRF 32

#define GEN4_MAX_SF_THREADS 24
#define GEN4_MAX_WM_THREADS 32
#define G4X_MAX_WM_THREADS 50

static const uint32_t ps_kernel_packed_static[][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_affine.g4b"
#include "exa_wm_src_sample_argb.g4b"
#include "exa_wm_yuv_rgb.g4b"
#include "exa_wm_write.g4b"
};

static const uint32_t ps_kernel_planar_static[][4] = {
#include "exa_wm_xy.g4b"
#include "exa_wm_src_affine.g4b"
#include "exa_wm_src_sample_planar.g4b"
#include "exa_wm_yuv_rgb.g4b"
#include "exa_wm_write.g4b"
};

#define NOKERNEL(kernel_enum, func, masked) \
    [kernel_enum] = {func, 0, masked}
#define KERNEL(kernel_enum, kernel, masked) \
    [kernel_enum] = {&kernel, sizeof(kernel), masked}
static const struct wm_kernel_info {
	const void *data;
	unsigned int size;
	bool has_mask;
} wm_kernels[] = {
	NOKERNEL(WM_KERNEL, brw_wm_kernel__affine, false),
	NOKERNEL(WM_KERNEL_P, brw_wm_kernel__projective, false),

	NOKERNEL(WM_KERNEL_MASK, brw_wm_kernel__affine_mask, true),
	NOKERNEL(WM_KERNEL_MASK_P, brw_wm_kernel__projective_mask, true),

	NOKERNEL(WM_KERNEL_MASKCA, brw_wm_kernel__affine_mask_ca, true),
	NOKERNEL(WM_KERNEL_MASKCA_P, brw_wm_kernel__projective_mask_ca, true),

	NOKERNEL(WM_KERNEL_MASKSA, brw_wm_kernel__affine_mask_sa, true),
	NOKERNEL(WM_KERNEL_MASKSA_P, brw_wm_kernel__projective_mask_sa, true),

	NOKERNEL(WM_KERNEL_OPACITY, brw_wm_kernel__affine_opacity, true),
	NOKERNEL(WM_KERNEL_OPACITY_P, brw_wm_kernel__projective_opacity, true),

	KERNEL(WM_KERNEL_VIDEO_PLANAR, ps_kernel_planar_static, false),
	KERNEL(WM_KERNEL_VIDEO_PACKED, ps_kernel_packed_static, false),
};
#undef KERNEL

static const struct blendinfo {
	bool src_alpha;
	uint32_t src_blend;
	uint32_t dst_blend;
} gen4_blend_op[] = {
	/* Clear */	{0, GEN4_BLENDFACTOR_ZERO, GEN4_BLENDFACTOR_ZERO},
	/* Src */	{0, GEN4_BLENDFACTOR_ONE, GEN4_BLENDFACTOR_ZERO},
	/* Dst */	{0, GEN4_BLENDFACTOR_ZERO, GEN4_BLENDFACTOR_ONE},
	/* Over */	{1, GEN4_BLENDFACTOR_ONE, GEN4_BLENDFACTOR_INV_SRC_ALPHA},
	/* OverReverse */ {0, GEN4_BLENDFACTOR_INV_DST_ALPHA, GEN4_BLENDFACTOR_ONE},
	/* In */	{0, GEN4_BLENDFACTOR_DST_ALPHA, GEN4_BLENDFACTOR_ZERO},
	/* InReverse */	{1, GEN4_BLENDFACTOR_ZERO, GEN4_BLENDFACTOR_SRC_ALPHA},
	/* Out */	{0, GEN4_BLENDFACTOR_INV_DST_ALPHA, GEN4_BLENDFACTOR_ZERO},
	/* OutReverse */ {1, GEN4_BLENDFACTOR_ZERO, GEN4_BLENDFACTOR_INV_SRC_ALPHA},
	/* Atop */	{1, GEN4_BLENDFACTOR_DST_ALPHA, GEN4_BLENDFACTOR_INV_SRC_ALPHA},
	/* AtopReverse */ {1, GEN4_BLENDFACTOR_INV_DST_ALPHA, GEN4_BLENDFACTOR_SRC_ALPHA},
	/* Xor */	{1, GEN4_BLENDFACTOR_INV_DST_ALPHA, GEN4_BLENDFACTOR_INV_SRC_ALPHA},
	/* Add */	{0, GEN4_BLENDFACTOR_ONE, GEN4_BLENDFACTOR_ONE},
};

/**
 * Highest-valued BLENDFACTOR used in gen4_blend_op.
 *
 * This leaves out GEN4_BLENDFACTOR_INV_DST_COLOR,
 * GEN4_BLENDFACTOR_INV_CONST_{COLOR,ALPHA},
 * GEN4_BLENDFACTOR_INV_SRC1_{COLOR,ALPHA}
 */
#define GEN4_BLENDFACTOR_COUNT (GEN4_BLENDFACTOR_INV_DST_ALPHA + 1)

#define BLEND_OFFSET(s, d) \
	(((s) * GEN4_BLENDFACTOR_COUNT + (d)) * 64)

#define SAMPLER_OFFSET(sf, se, mf, me, k) \
	((((((sf) * EXTEND_COUNT + (se)) * FILTER_COUNT + (mf)) * EXTEND_COUNT + (me)) * KERNEL_COUNT + (k)) * 64)

static void
gen4_emit_pipelined_pointers(struct sna *sna,
			     const struct sna_composite_op *op,
			     int blend, int kernel);

#define OUT_BATCH(v) batch_emit(sna, v)
#define OUT_VERTEX(x,y) vertex_emit_2s(sna, x,y)
#define OUT_VERTEX_F(v) vertex_emit(sna, v)

#define GEN4_MAX_3D_SIZE 8192

static inline bool too_large(int width, int height)
{
	return width > GEN4_MAX_3D_SIZE || height > GEN4_MAX_3D_SIZE;
}

static int
gen4_choose_composite_kernel(int op, bool has_mask, bool is_ca, bool is_affine)
{
	int base;

	if (has_mask) {
		if (is_ca) {
			if (gen4_blend_op[op].src_alpha)
				base = WM_KERNEL_MASKSA;
			else
				base = WM_KERNEL_MASKCA;
		} else
			base = WM_KERNEL_MASK;
	} else
		base = WM_KERNEL;

	return base + !is_affine;
}

static bool gen4_magic_ca_pass(struct sna *sna,
			       const struct sna_composite_op *op)
{
	struct gen4_render_state *state = &sna->render_state.gen4;

	if (!op->need_magic_ca_pass)
		return false;

	assert(sna->render.vertex_index > sna->render.vertex_start);

	DBG(("%s: CA fixup\n", __FUNCTION__));
	assert(op->mask.bo != NULL);
	assert(op->has_component_alpha);

	gen4_emit_pipelined_pointers(sna, op, PictOpAdd,
				     gen4_choose_composite_kernel(PictOpAdd,
								  true, true, op->is_affine));

	OUT_BATCH(GEN4_3DPRIMITIVE |
		  GEN4_3DPRIMITIVE_VERTEX_SEQUENTIAL |
		  (_3DPRIM_RECTLIST << GEN4_3DPRIMITIVE_TOPOLOGY_SHIFT) |
		  (0 << 9) |
		  4);
	OUT_BATCH(sna->render.vertex_index - sna->render.vertex_start);
	OUT_BATCH(sna->render.vertex_start);
	OUT_BATCH(1);	/* single instance */
	OUT_BATCH(0);	/* start instance location */
	OUT_BATCH(0);	/* index buffer offset, ignored */

	state->last_primitive = sna->kgem.nbatch;
	return true;
}

static uint32_t gen4_get_blend(int op,
			       bool has_component_alpha,
			       uint32_t dst_format)
{
	uint32_t src, dst;

	src = gen4_blend_op[op].src_blend;
	dst = gen4_blend_op[op].dst_blend;

	/* If there's no dst alpha channel, adjust the blend op so that we'll treat
	 * it as always 1.
	 */
	if (PICT_FORMAT_A(dst_format) == 0) {
		if (src == GEN4_BLENDFACTOR_DST_ALPHA)
			src = GEN4_BLENDFACTOR_ONE;
		else if (src == GEN4_BLENDFACTOR_INV_DST_ALPHA)
			src = GEN4_BLENDFACTOR_ZERO;
	}

	/* If the source alpha is being used, then we should only be in a
	 * case where the source blend factor is 0, and the source blend
	 * value is the mask channels multiplied by the source picture's alpha.
	 */
	if (has_component_alpha && gen4_blend_op[op].src_alpha) {
		if (dst == GEN4_BLENDFACTOR_SRC_ALPHA)
			dst = GEN4_BLENDFACTOR_SRC_COLOR;
		else if (dst == GEN4_BLENDFACTOR_INV_SRC_ALPHA)
			dst = GEN4_BLENDFACTOR_INV_SRC_COLOR;
	}

	DBG(("blend op=%d, dst=%x [A=%d] => src=%d, dst=%d => offset=%x\n",
	     op, dst_format, PICT_FORMAT_A(dst_format),
	     src, dst, BLEND_OFFSET(src, dst)));
	return BLEND_OFFSET(src, dst);
}

static uint32_t gen4_get_card_format(PictFormat format)
{
	switch (format) {
	default:
		return -1;
	case PICT_a8r8g8b8:
		return GEN4_SURFACEFORMAT_B8G8R8A8_UNORM;
	case PICT_x8r8g8b8:
		return GEN4_SURFACEFORMAT_B8G8R8X8_UNORM;
	case PICT_a8b8g8r8:
		return GEN4_SURFACEFORMAT_R8G8B8A8_UNORM;
	case PICT_x8b8g8r8:
		return GEN4_SURFACEFORMAT_R8G8B8X8_UNORM;
#ifdef PICT_a2r10g10b10
	case PICT_a2r10g10b10:
		return GEN4_SURFACEFORMAT_B10G10R10A2_UNORM;
	case PICT_x2r10g10b10:
		return GEN4_SURFACEFORMAT_B10G10R10X2_UNORM;
#endif
	case PICT_r8g8b8:
		return GEN4_SURFACEFORMAT_R8G8B8_UNORM;
	case PICT_r5g6b5:
		return GEN4_SURFACEFORMAT_B5G6R5_UNORM;
	case PICT_a1r5g5b5:
		return GEN4_SURFACEFORMAT_B5G5R5A1_UNORM;
	case PICT_a8:
		return GEN4_SURFACEFORMAT_A8_UNORM;
	case PICT_a4r4g4b4:
		return GEN4_SURFACEFORMAT_B4G4R4A4_UNORM;
	}
}

static uint32_t gen4_get_dest_format(PictFormat format)
{
	switch (format) {
	default:
		return -1;
	case PICT_a8r8g8b8:
	case PICT_x8r8g8b8:
		return GEN4_SURFACEFORMAT_B8G8R8A8_UNORM;
	case PICT_a8b8g8r8:
	case PICT_x8b8g8r8:
		return GEN4_SURFACEFORMAT_R8G8B8A8_UNORM;
#ifdef PICT_a2r10g10b10
	case PICT_a2r10g10b10:
	case PICT_x2r10g10b10:
		return GEN4_SURFACEFORMAT_B10G10R10A2_UNORM;
#endif
	case PICT_r5g6b5:
		return GEN4_SURFACEFORMAT_B5G6R5_UNORM;
	case PICT_x1r5g5b5:
	case PICT_a1r5g5b5:
		return GEN4_SURFACEFORMAT_B5G5R5A1_UNORM;
	case PICT_a8:
		return GEN4_SURFACEFORMAT_A8_UNORM;
	case PICT_a4r4g4b4:
	case PICT_x4r4g4b4:
		return GEN4_SURFACEFORMAT_B4G4R4A4_UNORM;
	}
}

static bool gen4_check_dst_format(PictFormat format)
{
	if (gen4_get_dest_format(format) != -1)
		return true;

	DBG(("%s: unhandled format: %x\n", __FUNCTION__, (int)format));
	return false;
}

static bool gen4_check_format(uint32_t format)
{
	if (gen4_get_card_format(format) != -1)
		return true;

	DBG(("%s: unhandled format: %x\n", __FUNCTION__, (int)format));
	return false;
}

typedef struct gen4_surface_state_padded {
	struct gen4_surface_state state;
	char pad[32 - sizeof(struct gen4_surface_state)];
} gen4_surface_state_padded;

static void null_create(struct sna_static_stream *stream)
{
	/* A bunch of zeros useful for legacy border color and depth-stencil */
	sna_static_stream_map(stream, 64, 64);
}

static void
sampler_state_init(struct gen4_sampler_state *sampler_state,
		   sampler_filter_t filter,
		   sampler_extend_t extend)
{
	sampler_state->ss0.lod_preclamp = 1;	/* GL mode */

	/* We use the legacy mode to get the semantics specified by
	 * the Render extension. */
	sampler_state->ss0.border_color_mode = GEN4_BORDER_COLOR_MODE_LEGACY;

	switch (filter) {
	default:
	case SAMPLER_FILTER_NEAREST:
		sampler_state->ss0.min_filter = GEN4_MAPFILTER_NEAREST;
		sampler_state->ss0.mag_filter = GEN4_MAPFILTER_NEAREST;
		break;
	case SAMPLER_FILTER_BILINEAR:
		sampler_state->ss0.min_filter = GEN4_MAPFILTER_LINEAR;
		sampler_state->ss0.mag_filter = GEN4_MAPFILTER_LINEAR;
		break;
	}

	switch (extend) {
	default:
	case SAMPLER_EXTEND_NONE:
		sampler_state->ss1.r_wrap_mode = GEN4_TEXCOORDMODE_CLAMP_BORDER;
		sampler_state->ss1.s_wrap_mode = GEN4_TEXCOORDMODE_CLAMP_BORDER;
		sampler_state->ss1.t_wrap_mode = GEN4_TEXCOORDMODE_CLAMP_BORDER;
		break;
	case SAMPLER_EXTEND_REPEAT:
		sampler_state->ss1.r_wrap_mode = GEN4_TEXCOORDMODE_WRAP;
		sampler_state->ss1.s_wrap_mode = GEN4_TEXCOORDMODE_WRAP;
		sampler_state->ss1.t_wrap_mode = GEN4_TEXCOORDMODE_WRAP;
		break;
	case SAMPLER_EXTEND_PAD:
		sampler_state->ss1.r_wrap_mode = GEN4_TEXCOORDMODE_CLAMP;
		sampler_state->ss1.s_wrap_mode = GEN4_TEXCOORDMODE_CLAMP;
		sampler_state->ss1.t_wrap_mode = GEN4_TEXCOORDMODE_CLAMP;
		break;
	case SAMPLER_EXTEND_REFLECT:
		sampler_state->ss1.r_wrap_mode = GEN4_TEXCOORDMODE_MIRROR;
		sampler_state->ss1.s_wrap_mode = GEN4_TEXCOORDMODE_MIRROR;
		sampler_state->ss1.t_wrap_mode = GEN4_TEXCOORDMODE_MIRROR;
		break;
	}
}

static uint32_t gen4_filter(uint32_t filter)
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

static uint32_t gen4_check_filter(PicturePtr picture)
{
	switch (picture->filter) {
	case PictFilterNearest:
	case PictFilterBilinear:
		return true;
	default:
		DBG(("%s: unknown filter: %s [%d]\n",
		     __FUNCTION__,
		     PictureGetFilterName(picture->filter),
		     picture->filter));
		return false;
	}
}

static uint32_t gen4_repeat(uint32_t repeat)
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

static bool gen4_check_repeat(PicturePtr picture)
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
		DBG(("%s: unknown repeat: %d\n",
		     __FUNCTION__, picture->repeatType));
		return false;
	}
}

static uint32_t
gen4_tiling_bits(uint32_t tiling)
{
	switch (tiling) {
	default: assert(0);
	case I915_TILING_NONE: return 0;
	case I915_TILING_X: return GEN4_SURFACE_TILED;
	case I915_TILING_Y: return GEN4_SURFACE_TILED | GEN4_SURFACE_TILED_Y;
	}
}

/**
 * Sets up the common fields for a surface state buffer for the given
 * picture in the given surface state buffer.
 */
static uint32_t
gen4_bind_bo(struct sna *sna,
	     struct kgem_bo *bo,
	     uint32_t width,
	     uint32_t height,
	     uint32_t format,
	     bool is_dst)
{
	uint32_t domains;
	uint16_t offset;
	uint32_t *ss;

	assert(sna->kgem.gen != 040 || !kgem_bo_is_snoop(bo));

	/* After the first bind, we manage the cache domains within the batch */
	offset = kgem_bo_get_binding(bo, format | is_dst << 31);
	if (offset) {
		assert(offset >= sna->kgem.surface);
		if (is_dst)
			kgem_bo_mark_dirty(bo);
		return offset * sizeof(uint32_t);
	}

	offset = sna->kgem.surface -=
		sizeof(struct gen4_surface_state_padded) / sizeof(uint32_t);
	ss = sna->kgem.batch + offset;

	ss[0] = (GEN4_SURFACE_2D << GEN4_SURFACE_TYPE_SHIFT |
		 GEN4_SURFACE_BLEND_ENABLED |
		 format << GEN4_SURFACE_FORMAT_SHIFT);

	if (is_dst) {
		ss[0] |= GEN4_SURFACE_RC_READ_WRITE;
		domains = I915_GEM_DOMAIN_RENDER << 16 | I915_GEM_DOMAIN_RENDER;
	} else
		domains = I915_GEM_DOMAIN_SAMPLER << 16;
	ss[1] = kgem_add_reloc(&sna->kgem, offset + 1, bo, domains, 0);

	ss[2] = ((width - 1)  << GEN4_SURFACE_WIDTH_SHIFT |
		 (height - 1) << GEN4_SURFACE_HEIGHT_SHIFT);
	ss[3] = (gen4_tiling_bits(bo->tiling) |
		 (bo->pitch - 1) << GEN4_SURFACE_PITCH_SHIFT);
	ss[4] = 0;
	ss[5] = 0;

	kgem_bo_set_binding(bo, format | is_dst << 31, offset);

	DBG(("[%x] bind bo(handle=%d, addr=%d), format=%d, width=%d, height=%d, pitch=%d, tiling=%d -> %s\n",
	     offset, bo->handle, ss[1],
	     format, width, height, bo->pitch, bo->tiling,
	     domains & 0xffff ? "render" : "sampler"));

	return offset * sizeof(uint32_t);
}

static void gen4_emit_vertex_buffer(struct sna *sna,
				    const struct sna_composite_op *op)
{
	int id = op->u.gen4.ve_id;

	assert((sna->render.vb_id & (1 << id)) == 0);

	OUT_BATCH(GEN4_3DSTATE_VERTEX_BUFFERS | 3);
	OUT_BATCH((id << VB0_BUFFER_INDEX_SHIFT) | VB0_VERTEXDATA |
		  (4*op->floats_per_vertex << VB0_BUFFER_PITCH_SHIFT));
	assert(sna->render.nvertex_reloc < ARRAY_SIZE(sna->render.vertex_reloc));
	sna->render.vertex_reloc[sna->render.nvertex_reloc++] = sna->kgem.nbatch;
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);

	sna->render.vb_id |= 1 << id;
}

inline static void
gen4_emit_pipe_flush(struct sna *sna)
{
#if 1
	OUT_BATCH(GEN4_PIPE_CONTROL |
		  GEN4_PIPE_CONTROL_WC_FLUSH |
		  (4 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
#else
	OUT_BATCH(MI_FLUSH | MI_INHIBIT_RENDER_CACHE_FLUSH);
#endif
}

inline static void
gen4_emit_pipe_break(struct sna *sna)
{
#if !ALWAYS_FLUSH
	OUT_BATCH(GEN4_PIPE_CONTROL | (4 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
#else
	OUT_BATCH(MI_FLUSH | MI_INHIBIT_RENDER_CACHE_FLUSH);
#endif
}

inline static void
gen4_emit_pipe_invalidate(struct sna *sna)
{
#if 0
	OUT_BATCH(GEN4_PIPE_CONTROL |
		  GEN4_PIPE_CONTROL_WC_FLUSH |
		  (sna->kgem.gen >= 045 ? GEN4_PIPE_CONTROL_TC_FLUSH : 0) |
		  (4 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
#else
	OUT_BATCH(MI_FLUSH);
#endif
}

static void gen4_emit_primitive(struct sna *sna)
{
	if (sna->kgem.nbatch == sna->render_state.gen4.last_primitive) {
		sna->render.vertex_offset = sna->kgem.nbatch - 5;
		return;
	}

	OUT_BATCH(GEN4_3DPRIMITIVE |
		  GEN4_3DPRIMITIVE_VERTEX_SEQUENTIAL |
		  (_3DPRIM_RECTLIST << GEN4_3DPRIMITIVE_TOPOLOGY_SHIFT) |
		  (0 << 9) |
		  4);
	sna->render.vertex_offset = sna->kgem.nbatch;
	OUT_BATCH(0);	/* vertex count, to be filled in later */
	OUT_BATCH(sna->render.vertex_index);
	OUT_BATCH(1);	/* single instance */
	OUT_BATCH(0);	/* start instance location */
	OUT_BATCH(0);	/* index buffer offset, ignored */
	sna->render.vertex_start = sna->render.vertex_index;

	sna->render_state.gen4.last_primitive = sna->kgem.nbatch;
}

static bool gen4_rectangle_begin(struct sna *sna,
				 const struct sna_composite_op *op)
{
	unsigned int id = 1 << op->u.gen4.ve_id;
	int ndwords;

	if (sna_vertex_wait__locked(&sna->render) && sna->render.vertex_offset)
		return true;

	/* 7xpipelined pointers + 6xprimitive + 1xflush */
	ndwords = op->need_magic_ca_pass? 19 : 6;
	if ((sna->render.vb_id & id) == 0)
		ndwords += 5;
	ndwords += 8*FORCE_FLUSH;

	if (!kgem_check_batch(&sna->kgem, ndwords))
		return false;

	if ((sna->render.vb_id & id) == 0)
		gen4_emit_vertex_buffer(sna, op);
	if (sna->render.vertex_offset == 0)
		gen4_emit_primitive(sna);

	return true;
}

static int gen4_get_rectangles__flush(struct sna *sna,
				      const struct sna_composite_op *op)
{
	/* Preventing discarding new vbo after lock contention */
	if (sna_vertex_wait__locked(&sna->render)) {
		int rem = vertex_space(sna);
		if (rem > op->floats_per_rect)
			return rem;
	}

	if (!kgem_check_batch(&sna->kgem,
			      8*FORCE_FLUSH + (op->need_magic_ca_pass ? 2*19+6 : 6)))
		return 0;
	if (!kgem_check_reloc_and_exec(&sna->kgem, 2))
		return 0;

	if (sna->render.vertex_offset) {
		gen4_vertex_flush(sna);
		if (gen4_magic_ca_pass(sna, op))
			gen4_emit_pipelined_pointers(sna, op, op->op,
						     op->u.gen4.wm_kernel);
	}

	return gen4_vertex_finish(sna);
}

inline static int gen4_get_rectangles(struct sna *sna,
				      const struct sna_composite_op *op,
				      int want,
				      void (*emit_state)(struct sna *sna, const struct sna_composite_op *op))
{
	int rem;

	assert(want);
#if FORCE_FLUSH
	rem = sna->render.vertex_offset;
	if (sna->kgem.nbatch == sna->render_state.gen4.last_primitive)
		rem = sna->kgem.nbatch - 5;
	if (rem) {
		rem = MAX_FLUSH_VERTICES - (sna->render.vertex_index - sna->render.vertex_start) / 3;
		if (rem <= 0) {
			if (sna->render.vertex_offset) {
				gen4_vertex_flush(sna);
				if (gen4_magic_ca_pass(sna, op)) {
					if (kgem_check_batch(&sna->kgem, 19+6))
						gen4_emit_pipelined_pointers(sna, op, op->op,
									     op->u.gen4.wm_kernel);
				}
			}
			gen4_emit_pipe_break(sna);
			rem = MAX_FLUSH_VERTICES;
		}
	} else
		rem = MAX_FLUSH_VERTICES;
	if (want > rem)
		want = rem;
#endif

start:
	rem = vertex_space(sna);
	if (unlikely(rem < op->floats_per_rect)) {
		DBG(("flushing vbo for %s: %d < %d\n",
		     __FUNCTION__, rem, op->floats_per_rect));
		rem = gen4_get_rectangles__flush(sna, op);
		if (unlikely(rem == 0))
			goto flush;
	}

	if (unlikely(sna->render.vertex_offset == 0)) {
		if (!gen4_rectangle_begin(sna, op))
			goto flush;
		else
			goto start;
	}

	assert(rem <= vertex_space(sna));
	assert(op->floats_per_rect <= rem);
	if (want > 1 && want * op->floats_per_rect > rem)
		want = rem / op->floats_per_rect;

	sna->render.vertex_index += 3*want;
	return want;

flush:
	if (sna->render.vertex_offset) {
		gen4_vertex_flush(sna);
		gen4_magic_ca_pass(sna, op);
	}
	sna_vertex_wait__locked(&sna->render);
	_kgem_submit(&sna->kgem);
	emit_state(sna, op);
	goto start;
}

static uint32_t *
gen4_composite_get_binding_table(struct sna *sna, uint16_t *offset)
{
	sna->kgem.surface -=
		sizeof(struct gen4_surface_state_padded) / sizeof(uint32_t);

	DBG(("%s(%x)\n", __FUNCTION__, 4*sna->kgem.surface));

	/* Clear all surplus entries to zero in case of prefetch */
	*offset = sna->kgem.surface;
	return memset(sna->kgem.batch + sna->kgem.surface,
		      0, sizeof(struct gen4_surface_state_padded));
}

static void
gen4_emit_urb(struct sna *sna)
{
	int urb_vs_end;
	int urb_gs_end;
	int urb_cl_end;
	int urb_sf_end;
	int urb_cs_end;

	if (!sna->render_state.gen4.needs_urb)
		return;

	urb_vs_end =              URB_VS_ENTRIES * URB_VS_ENTRY_SIZE;
	urb_gs_end = urb_vs_end + URB_GS_ENTRIES * URB_GS_ENTRY_SIZE;
	urb_cl_end = urb_gs_end + URB_CL_ENTRIES * URB_CL_ENTRY_SIZE;
	urb_sf_end = urb_cl_end + URB_SF_ENTRIES * URB_SF_ENTRY_SIZE;
	urb_cs_end = urb_sf_end + URB_CS_ENTRIES * URB_CS_ENTRY_SIZE;
	assert(urb_cs_end <= 256);

	while ((sna->kgem.nbatch & 15) > 12)
		OUT_BATCH(MI_NOOP);

	OUT_BATCH(GEN4_URB_FENCE |
		  UF0_CS_REALLOC |
		  UF0_SF_REALLOC |
		  UF0_CLIP_REALLOC |
		  UF0_GS_REALLOC |
		  UF0_VS_REALLOC |
		  1);
	OUT_BATCH(urb_cl_end << UF1_CLIP_FENCE_SHIFT |
		  urb_gs_end << UF1_GS_FENCE_SHIFT |
		  urb_vs_end << UF1_VS_FENCE_SHIFT);
	OUT_BATCH(urb_cs_end << UF2_CS_FENCE_SHIFT |
		  urb_sf_end << UF2_SF_FENCE_SHIFT);

	/* Constant buffer state */
	OUT_BATCH(GEN4_CS_URB_STATE | 0);
	OUT_BATCH((URB_CS_ENTRY_SIZE - 1) << 4 | URB_CS_ENTRIES << 0);

	sna->render_state.gen4.needs_urb = false;
}

static void
gen4_emit_state_base_address(struct sna *sna)
{
	assert(sna->render_state.gen4.general_bo->proxy == NULL);
	OUT_BATCH(GEN4_STATE_BASE_ADDRESS | 4);
	OUT_BATCH(kgem_add_reloc(&sna->kgem, /* general */
				 sna->kgem.nbatch,
				 sna->render_state.gen4.general_bo,
				 I915_GEM_DOMAIN_INSTRUCTION << 16,
				 BASE_ADDRESS_MODIFY));
	OUT_BATCH(kgem_add_reloc(&sna->kgem, /* surface */
				 sna->kgem.nbatch,
				 NULL,
				 I915_GEM_DOMAIN_INSTRUCTION << 16,
				 BASE_ADDRESS_MODIFY));
	OUT_BATCH(0); /* media */

	/* upper bounds, all disabled */
	OUT_BATCH(BASE_ADDRESS_MODIFY);
	OUT_BATCH(0);
}

static void
gen4_emit_invariant(struct sna *sna)
{
	assert(sna->kgem.surface == sna->kgem.batch_size);

	if (sna->kgem.gen >= 045)
		OUT_BATCH(NEW_PIPELINE_SELECT | PIPELINE_SELECT_3D);
	else
		OUT_BATCH(GEN4_PIPELINE_SELECT | PIPELINE_SELECT_3D);

	gen4_emit_state_base_address(sna);

	sna->render_state.gen4.needs_invariant = false;
}

static void
gen4_get_batch(struct sna *sna, const struct sna_composite_op *op)
{
	kgem_set_mode(&sna->kgem, KGEM_RENDER, op->dst.bo);

	if (!kgem_check_batch_with_surfaces(&sna->kgem, 150 + 50*FORCE_FLUSH, 4)) {
		DBG(("%s: flushing batch: %d < %d+%d\n",
		     __FUNCTION__, sna->kgem.surface - sna->kgem.nbatch,
		     150, 4*8));
		kgem_submit(&sna->kgem);
		_kgem_set_mode(&sna->kgem, KGEM_RENDER);
	}

	if (sna->render_state.gen4.needs_invariant)
		gen4_emit_invariant(sna);
}

static void
gen4_align_vertex(struct sna *sna, const struct sna_composite_op *op)
{
	assert(op->floats_per_rect == 3*op->floats_per_vertex);
	if (op->floats_per_vertex != sna->render_state.gen4.floats_per_vertex) {
		DBG(("aligning vertex: was %d, now %d floats per vertex\n",
		     sna->render_state.gen4.floats_per_vertex,
		     op->floats_per_vertex));
		gen4_vertex_align(sna, op);
		sna->render_state.gen4.floats_per_vertex = op->floats_per_vertex;
	}
}

static void
gen4_emit_binding_table(struct sna *sna, uint16_t offset)
{
	if (sna->render_state.gen4.surface_table == offset)
		return;

	sna->render_state.gen4.surface_table = offset;

	/* Binding table pointers */
	OUT_BATCH(GEN4_3DSTATE_BINDING_TABLE_POINTERS | 4);
	OUT_BATCH(0);		/* vs */
	OUT_BATCH(0);		/* gs */
	OUT_BATCH(0);		/* clip */
	OUT_BATCH(0);		/* sf */
	/* Only the PS uses the binding table */
	OUT_BATCH(offset*4);
}

static void
gen4_emit_pipelined_pointers(struct sna *sna,
			     const struct sna_composite_op *op,
			     int blend, int kernel)
{
	uint16_t sp, bp;
	uint32_t key;

	DBG(("%s: has_mask=%d, src=(%d, %d), mask=(%d, %d),kernel=%d, blend=%d, ca=%d, format=%x\n",
	     __FUNCTION__, op->u.gen4.ve_id & 2,
	     op->src.filter, op->src.repeat,
	     op->mask.filter, op->mask.repeat,
	     kernel, blend, op->has_component_alpha, (int)op->dst.format));

	sp = SAMPLER_OFFSET(op->src.filter, op->src.repeat,
			    op->mask.filter, op->mask.repeat,
			    kernel);
	bp = gen4_get_blend(blend, op->has_component_alpha, op->dst.format);

	DBG(("%s: sp=%d, bp=%d\n", __FUNCTION__, sp, bp));
	key = sp | (uint32_t)bp << 16;
	if (key == sna->render_state.gen4.last_pipelined_pointers)
		return;

	OUT_BATCH(GEN4_3DSTATE_PIPELINED_POINTERS | 5);
	OUT_BATCH(sna->render_state.gen4.vs);
	OUT_BATCH(GEN4_GS_DISABLE); /* passthrough */
	OUT_BATCH(GEN4_CLIP_DISABLE); /* passthrough */
	OUT_BATCH(sna->render_state.gen4.sf);
	OUT_BATCH(sna->render_state.gen4.wm + sp);
	OUT_BATCH(sna->render_state.gen4.cc + bp);

	sna->render_state.gen4.last_pipelined_pointers = key;
	gen4_emit_urb(sna);
}

static bool
gen4_emit_drawing_rectangle(struct sna *sna, const struct sna_composite_op *op)
{
	uint32_t limit = (op->dst.height - 1) << 16 | (op->dst.width - 1);
	uint32_t offset = (uint16_t)op->dst.y << 16 | (uint16_t)op->dst.x;

	assert(!too_large(op->dst.x, op->dst.y));
	assert(!too_large(op->dst.width, op->dst.height));

	if (sna->render_state.gen4.drawrect_limit == limit &&
	    sna->render_state.gen4.drawrect_offset == offset)
		return true;

	sna->render_state.gen4.drawrect_offset = offset;
	sna->render_state.gen4.drawrect_limit = limit;

	OUT_BATCH(GEN4_3DSTATE_DRAWING_RECTANGLE | (4 - 2));
	OUT_BATCH(0);
	OUT_BATCH(limit);
	OUT_BATCH(offset);
	return false;
}

static void
gen4_emit_vertex_elements(struct sna *sna,
			  const struct sna_composite_op *op)
{
	/*
	 * vertex data in vertex buffer
	 *    position: (x, y)
	 *    texture coordinate 0: (u0, v0) if (is_affine is true) else (u0, v0, w0)
	 *    texture coordinate 1 if (has_mask is true): same as above
	 */
	struct gen4_render_state *render = &sna->render_state.gen4;
	uint32_t src_format, dw;
	int id = op->u.gen4.ve_id;

	if (render->ve_id == id)
		return;
	render->ve_id = id;

	/* The VUE layout
	 *    dword 0-3: position (x, y, 1.0, 1.0),
	 *    dword 4-7: texture coordinate 0 (u0, v0, w0, 1.0)
	 *    [optional] dword 8-11: texture coordinate 1 (u1, v1, w1, 1.0)
	 */
	OUT_BATCH(GEN4_3DSTATE_VERTEX_ELEMENTS | (2 * (1 + 2) - 1));

	/* x,y */
	OUT_BATCH(id << VE0_VERTEX_BUFFER_INDEX_SHIFT | VE0_VALID |
		  GEN4_SURFACEFORMAT_R16G16_SSCALED << VE0_FORMAT_SHIFT |
		  0 << VE0_OFFSET_SHIFT);
	OUT_BATCH(VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT |
		  VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_1_SHIFT |
		  VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_2_SHIFT |
		  VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_3_SHIFT |
		  (1*4) << VE1_DESTINATION_ELEMENT_OFFSET_SHIFT);

	/* u0, v0, w0 */
	/* u0, v0, w0 */
	DBG(("%s: first channel %d floats, offset=4b\n", __FUNCTION__, id & 3));
	dw = VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_3_SHIFT;
	switch (id & 3) {
	default:
		assert(0);
	case 0:
		src_format = GEN4_SURFACEFORMAT_R16G16_SSCALED;
		dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT;
		dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_1_SHIFT;
		dw |= VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_2_SHIFT;
		break;
	case 1:
		src_format = GEN4_SURFACEFORMAT_R32_FLOAT;
		dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT;
		dw |= VFCOMPONENT_STORE_0 << VE1_VFCOMPONENT_1_SHIFT;
		dw |= VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_2_SHIFT;
		break;
	case 2:
		src_format = GEN4_SURFACEFORMAT_R32G32_FLOAT;
		dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT;
		dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_1_SHIFT;
		dw |= VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_2_SHIFT;
		break;
	case 3:
		src_format = GEN4_SURFACEFORMAT_R32G32B32_FLOAT;
		dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT;
		dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_1_SHIFT;
		dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_2_SHIFT;
		break;
	}
	OUT_BATCH(id << VE0_VERTEX_BUFFER_INDEX_SHIFT | VE0_VALID |
		  src_format << VE0_FORMAT_SHIFT |
		  4 << VE0_OFFSET_SHIFT);
	OUT_BATCH(dw | 8 << VE1_DESTINATION_ELEMENT_OFFSET_SHIFT);

	/* u1, v1, w1 */
	if (id >> 2) {
		unsigned src_offset = 4 + ((id & 3) ?: 1) * sizeof(float);
		DBG(("%s: second channel %d floats, offset=%db\n", __FUNCTION__,
		     id >> 2, src_offset));
		dw = VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_3_SHIFT;
		switch (id >> 2) {
		case 1:
			src_format = GEN4_SURFACEFORMAT_R32_FLOAT;
			dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT;
			dw |= VFCOMPONENT_STORE_0 << VE1_VFCOMPONENT_1_SHIFT;
			dw |= VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_2_SHIFT;
			break;
		default:
			assert(0);
		case 2:
			src_format = GEN4_SURFACEFORMAT_R32G32_FLOAT;
			dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT;
			dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_1_SHIFT;
			dw |= VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_2_SHIFT;
			break;
		case 3:
			src_format = GEN4_SURFACEFORMAT_R32G32B32_FLOAT;
			dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT;
			dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_1_SHIFT;
			dw |= VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_2_SHIFT;
			break;
		}
		OUT_BATCH(id << VE0_VERTEX_BUFFER_INDEX_SHIFT | VE0_VALID |
			  src_format << VE0_FORMAT_SHIFT |
			  src_offset << VE0_OFFSET_SHIFT);
		OUT_BATCH(dw | 12 << VE1_DESTINATION_ELEMENT_OFFSET_SHIFT);
	} else {
		OUT_BATCH(id << VE0_VERTEX_BUFFER_INDEX_SHIFT | VE0_VALID |
			  GEN4_SURFACEFORMAT_R16G16_SSCALED << VE0_FORMAT_SHIFT |
			  0 << VE0_OFFSET_SHIFT);
		OUT_BATCH(VFCOMPONENT_STORE_0 << VE1_VFCOMPONENT_0_SHIFT |
			  VFCOMPONENT_STORE_0 << VE1_VFCOMPONENT_1_SHIFT |
			  VFCOMPONENT_STORE_0 << VE1_VFCOMPONENT_2_SHIFT |
			  VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_3_SHIFT |
			  12 << VE1_DESTINATION_ELEMENT_OFFSET_SHIFT);
	}
}

static void
gen4_emit_state(struct sna *sna,
		const struct sna_composite_op *op,
		uint16_t wm_binding_table)
{
	bool flush;

	assert(op->dst.bo->exec);

	flush = wm_binding_table & 1;
	wm_binding_table &= ~1;

	if (ALWAYS_FLUSH || kgem_bo_is_dirty(op->src.bo) || kgem_bo_is_dirty(op->mask.bo)) {
		DBG(("%s: flushing dirty (%d, %d), forced? %d\n", __FUNCTION__,
		     kgem_bo_is_dirty(op->src.bo),
		     kgem_bo_is_dirty(op->mask.bo),
		     flush));
		gen4_emit_pipe_invalidate(sna);
		kgem_clear_dirty(&sna->kgem);
		kgem_bo_mark_dirty(op->dst.bo);
		flush = false;
	}
	flush &= gen4_emit_drawing_rectangle(sna, op);
	if (flush && op->op > PictOpSrc)
		gen4_emit_pipe_flush(sna);

	gen4_emit_binding_table(sna, wm_binding_table);
	gen4_emit_pipelined_pointers(sna, op, op->op, op->u.gen4.wm_kernel);
	gen4_emit_vertex_elements(sna, op);
}

static void
gen4_bind_surfaces(struct sna *sna,
		   const struct sna_composite_op *op)
{
	uint32_t *binding_table;
	uint16_t offset, dirty;

	gen4_get_batch(sna, op);
	dirty = kgem_bo_is_dirty(op->dst.bo);

	binding_table = gen4_composite_get_binding_table(sna, &offset);

	binding_table[0] =
		gen4_bind_bo(sna,
			    op->dst.bo, op->dst.width, op->dst.height,
			    gen4_get_dest_format(op->dst.format),
			    true);
	binding_table[1] =
		gen4_bind_bo(sna,
			     op->src.bo, op->src.width, op->src.height,
			     op->src.card_format,
			     false);
	if (op->mask.bo) {
		assert(op->u.gen4.ve_id >> 2);
		binding_table[2] =
			gen4_bind_bo(sna,
				     op->mask.bo,
				     op->mask.width,
				     op->mask.height,
				     op->mask.card_format,
				     false);
	}

	if (sna->kgem.surface == offset &&
	    *(uint64_t *)(sna->kgem.batch + sna->render_state.gen4.surface_table) == *(uint64_t*)binding_table &&
	    (op->mask.bo == NULL ||
	     sna->kgem.batch[sna->render_state.gen4.surface_table+2] == binding_table[2])) {
		sna->kgem.surface += sizeof(struct gen4_surface_state_padded) / sizeof(uint32_t);
		offset = sna->render_state.gen4.surface_table;
	}

	if (!ALWAYS_FLUSH && sna->kgem.batch[sna->render_state.gen4.surface_table] == binding_table[0])
		dirty = 0;

	gen4_emit_state(sna, op, offset | dirty);
}

fastcall static void
gen4_render_composite_blt(struct sna *sna,
			  const struct sna_composite_op *op,
			  const struct sna_composite_rectangles *r)
{
	DBG(("%s: src=(%d, %d)+(%d, %d), mask=(%d, %d)+(%d, %d), dst=(%d, %d)+(%d, %d), size=(%d, %d)\n",
	     __FUNCTION__,
	     r->src.x, r->src.y, op->src.offset[0], op->src.offset[1],
	     r->mask.x, r->mask.y, op->mask.offset[0], op->mask.offset[1],
	     r->dst.x, r->dst.y, op->dst.x, op->dst.y,
	     r->width, r->height));

	gen4_get_rectangles(sna, op, 1, gen4_bind_surfaces);
	op->prim_emit(sna, op, r);
}

fastcall static void
gen4_render_composite_box(struct sna *sna,
			  const struct sna_composite_op *op,
			  const BoxRec *box)
{
	struct sna_composite_rectangles r;

	DBG(("  %s: (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     box->x1, box->y1, box->x2, box->y2));

	gen4_get_rectangles(sna, op, 1, gen4_bind_surfaces);

	r.dst.x = box->x1;
	r.dst.y = box->y1;
	r.width  = box->x2 - box->x1;
	r.height = box->y2 - box->y1;
	r.mask = r.src = r.dst;

	op->prim_emit(sna, op, &r);
}

static void
gen4_render_composite_boxes__blt(struct sna *sna,
				 const struct sna_composite_op *op,
				 const BoxRec *box, int nbox)
{
	DBG(("%s(%d) delta=(%d, %d), src=(%d, %d)/(%d, %d), mask=(%d, %d)/(%d, %d)\n",
	     __FUNCTION__, nbox, op->dst.x, op->dst.y,
	     op->src.offset[0], op->src.offset[1],
	     op->src.width, op->src.height,
	     op->mask.offset[0], op->mask.offset[1],
	     op->mask.width, op->mask.height));

	do {
		int nbox_this_time;

		nbox_this_time = gen4_get_rectangles(sna, op, nbox,
						     gen4_bind_surfaces);
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
			r.mask = r.src = r.dst;
			op->prim_emit(sna, op, &r);
			box++;
		} while (--nbox_this_time);
	} while (nbox);
}

static void
gen4_render_composite_boxes(struct sna *sna,
			    const struct sna_composite_op *op,
			    const BoxRec *box, int nbox)
{
	DBG(("%s: nbox=%d\n", __FUNCTION__, nbox));

	do {
		int nbox_this_time;
		float *v;

		nbox_this_time = gen4_get_rectangles(sna, op, nbox,
						     gen4_bind_surfaces);
		assert(nbox_this_time);
		nbox -= nbox_this_time;

		v = sna->render.vertices + sna->render.vertex_used;
		sna->render.vertex_used += nbox_this_time * op->floats_per_rect;

		op->emit_boxes(op, box, nbox_this_time, v);
		box += nbox_this_time;
	} while (nbox);
}

#if !FORCE_FLUSH
static void
gen4_render_composite_boxes__thread(struct sna *sna,
				    const struct sna_composite_op *op,
				    const BoxRec *box, int nbox)
{
	DBG(("%s: nbox=%d\n", __FUNCTION__, nbox));

	sna_vertex_lock(&sna->render);
	do {
		int nbox_this_time;
		float *v;

		nbox_this_time = gen4_get_rectangles(sna, op, nbox,
						     gen4_bind_surfaces);
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
#endif

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

static uint32_t gen4_bind_video_source(struct sna *sna,
				       struct kgem_bo *src_bo,
				       uint32_t src_offset,
				       int src_width,
				       int src_height,
				       int src_pitch,
				       uint32_t src_surf_format)
{
	struct gen4_surface_state *ss;

	sna->kgem.surface -= sizeof(struct gen4_surface_state_padded) / sizeof(uint32_t);

	ss = memset(sna->kgem.batch + sna->kgem.surface, 0, sizeof(*ss));
	ss->ss0.surface_type = GEN4_SURFACE_2D;
	ss->ss0.surface_format = src_surf_format;
	ss->ss0.color_blend = 1;

	ss->ss1.base_addr =
		kgem_add_reloc(&sna->kgem,
			       sna->kgem.surface + 1,
			       src_bo,
			       I915_GEM_DOMAIN_SAMPLER << 16,
			       src_offset);

	ss->ss2.width  = src_width - 1;
	ss->ss2.height = src_height - 1;
	ss->ss3.pitch  = src_pitch - 1;

	return sna->kgem.surface * sizeof(uint32_t);
}

static void gen4_video_bind_surfaces(struct sna *sna,
				     const struct sna_composite_op *op)
{
	struct sna_video_frame *frame = op->priv;
	uint32_t src_surf_format;
	uint32_t src_surf_base[6];
	int src_width[6];
	int src_height[6];
	int src_pitch[6];
	uint32_t *binding_table;
	uint16_t offset, dirty;
	int n_src, n;

	src_surf_base[0] = 0;
	src_surf_base[1] = 0;
	src_surf_base[2] = frame->VBufOffset;
	src_surf_base[3] = frame->VBufOffset;
	src_surf_base[4] = frame->UBufOffset;
	src_surf_base[5] = frame->UBufOffset;

	if (is_planar_fourcc(frame->id)) {
		src_surf_format = GEN4_SURFACEFORMAT_R8_UNORM;
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
			src_surf_format = GEN4_SURFACEFORMAT_YCRCB_SWAPY;
		else
			src_surf_format = GEN4_SURFACEFORMAT_YCRCB_NORMAL;

		src_width[0]  = frame->width;
		src_height[0] = frame->height;
		src_pitch[0]  = frame->pitch[0];
		n_src = 1;
	}

	gen4_get_batch(sna, op);
	dirty = kgem_bo_is_dirty(op->dst.bo);

	binding_table = gen4_composite_get_binding_table(sna, &offset);
	binding_table[0] =
		gen4_bind_bo(sna,
			     op->dst.bo, op->dst.width, op->dst.height,
			     gen4_get_dest_format(op->dst.format),
			     true);
	for (n = 0; n < n_src; n++) {
		binding_table[1+n] =
			gen4_bind_video_source(sna,
					       frame->bo,
					       src_surf_base[n],
					       src_width[n],
					       src_height[n],
					       src_pitch[n],
					       src_surf_format);
	}

	if (!ALWAYS_FLUSH && sna->kgem.batch[sna->render_state.gen4.surface_table] == binding_table[0])
		dirty = 0;

	gen4_emit_state(sna, op, offset | dirty);
}

static bool
gen4_render_video(struct sna *sna,
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
	const BoxRec *box;

	DBG(("%s: %dx%d -> %dx%d\n", __FUNCTION__,
	     src_width, src_height, dst_width, dst_height));

	assert(priv->gpu_bo);
	memset(&tmp, 0, sizeof(tmp));

	tmp.op = PictOpSrc;
	tmp.dst.pixmap = pixmap;
	tmp.dst.width  = pixmap->drawable.width;
	tmp.dst.height = pixmap->drawable.height;
	tmp.dst.format = sna_format_for_depth(pixmap->drawable.depth);
	tmp.dst.bo = priv->gpu_bo;

	if (src_width == dst_width && src_height == dst_height)
		tmp.src.filter = SAMPLER_FILTER_NEAREST;
	else
		tmp.src.filter = SAMPLER_FILTER_BILINEAR;
	tmp.src.repeat = SAMPLER_EXTEND_PAD;
	tmp.src.bo = frame->bo;
	tmp.mask.bo = NULL;
	tmp.u.gen4.wm_kernel =
		is_planar_fourcc(frame->id) ? WM_KERNEL_VIDEO_PLANAR : WM_KERNEL_VIDEO_PACKED;
	tmp.u.gen4.ve_id = 2;
	tmp.is_affine = true;
	tmp.floats_per_vertex = 3;
	tmp.floats_per_rect = 9;
	tmp.priv = frame;

	if (!kgem_check_bo(&sna->kgem, tmp.dst.bo, frame->bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem, tmp.dst.bo, frame->bo, NULL))
			return false;
	}

	gen4_align_vertex(sna, &tmp);
	gen4_video_bind_surfaces(sna, &tmp);

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

	src_scale_x = (float)src_width / dst_width / frame->width;
	src_offset_x = (float)frame->src.x1 / frame->width - dstRegion->extents.x1 * src_scale_x;

	src_scale_y = (float)src_height / dst_height / frame->height;
	src_offset_y = (float)frame->src.y1 / frame->height - dstRegion->extents.y1 * src_scale_y;

	box = region_rects(dstRegion);
	nbox = region_num_rects(dstRegion);
	do {
		int n;

		n = gen4_get_rectangles(sna, &tmp, nbox,
					gen4_video_bind_surfaces);
		assert(n);
		nbox -= n;

		do {
			BoxRec r;

			r.x1 = box->x1 + pix_xoff;
			r.x2 = box->x2 + pix_xoff;
			r.y1 = box->y1 + pix_yoff;
			r.y2 = box->y2 + pix_yoff;

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
		} while (--n);
	} while (nbox);
	gen4_vertex_flush(sna);

	return true;
}

static int
gen4_composite_picture(struct sna *sna,
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

	if (!gen4_check_repeat(picture)) {
		DBG(("%s: unknown repeat mode fixup\n", __FUNCTION__));
		return sna_render_picture_fixup(sna, picture, channel,
						x, y, w, h, dst_x, dst_y);
	}

	if (!gen4_check_filter(picture)) {
		DBG(("%s: unhandled filter fixup\n", __FUNCTION__));
		return sna_render_picture_fixup(sna, picture, channel,
						x, y, w, h, dst_x, dst_y);
	}

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

		if (channel->repeat &&
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
	channel->card_format = gen4_get_card_format(picture->format);
	if (channel->card_format == -1)
		return sna_render_picture_convert(sna, picture, channel, pixmap,
						  x, y, w, h, dst_x, dst_y,
						  false);

	if (too_large(pixmap->drawable.width, pixmap->drawable.height))
		return sna_render_picture_extract(sna, picture, channel,
						  x, y, w, h, dst_x, dst_y);

	return sna_render_pixmap_bo(sna, channel, pixmap,
				    x, y, w, h, dst_x, dst_y);
}

static void gen4_composite_channel_convert(struct sna_composite_channel *channel)
{
	DBG(("%s: repeat %d -> %d, filter %d -> %d\n",
	     __FUNCTION__,
	     channel->repeat, gen4_repeat(channel->repeat),
	     channel->filter, gen4_repeat(channel->filter)));
	channel->repeat = gen4_repeat(channel->repeat);
	channel->filter = gen4_filter(channel->filter);
	if (channel->card_format == (unsigned)-1)
		channel->card_format = gen4_get_card_format(channel->pict_format);
}

static void
gen4_render_composite_done(struct sna *sna,
			   const struct sna_composite_op *op)
{
	DBG(("%s()\n", __FUNCTION__));

	if (sna->render.vertex_offset) {
		gen4_vertex_flush(sna);
		gen4_magic_ca_pass(sna, op);
	}

	if (op->mask.bo)
		kgem_bo_destroy(&sna->kgem, op->mask.bo);
	if (op->src.bo)
		kgem_bo_destroy(&sna->kgem, op->src.bo);

	sna_render_composite_redirect_done(sna, op);
}

static bool
gen4_composite_set_target(struct sna *sna,
			  struct sna_composite_op *op,
			  PicturePtr dst,
			  int x, int y, int w, int h,
			  bool partial)
{
	BoxRec box;
	unsigned hint;

	op->dst.pixmap = get_drawable_pixmap(dst->pDrawable);
	op->dst.width  = op->dst.pixmap->drawable.width;
	op->dst.height = op->dst.pixmap->drawable.height;
	op->dst.format = dst->format;
	if (w && h) {
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
check_gradient(PicturePtr picture, bool precise)
{
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
need_upload(struct sna *sna, PicturePtr p)
{
	return p->pDrawable && untransformed(p) &&
		!is_gpu(sna, p->pDrawable, PREFER_GPU_RENDER);
}

static bool
source_is_busy(PixmapPtr pixmap)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	if (priv == NULL)
		return false;

	if (priv->clear)
		return false;

	if (priv->gpu_bo && kgem_bo_is_busy(priv->gpu_bo))
		return true;

	if (priv->cpu_bo && kgem_bo_is_busy(priv->cpu_bo))
		return true;

	return priv->gpu_damage && !priv->cpu_damage;
}

static bool
source_fallback(struct sna *sna, PicturePtr p, PixmapPtr pixmap, bool precise)
{
	if (sna_picture_is_solid(p, NULL))
		return false;

	if (p->pSourcePict)
		return check_gradient(p, precise);

	if (!gen4_check_repeat(p) || !gen4_check_format(p->format))
		return true;

	/* soft errors: perfer to upload/compute rather than readback */
	if (pixmap && source_is_busy(pixmap))
		return false;

	return has_alphamap(p) || !gen4_check_filter(p) || need_upload(sna, p);
}

static bool
gen4_composite_fallback(struct sna *sna,
			PicturePtr src,
			PicturePtr mask,
			PicturePtr dst)
{
	PixmapPtr src_pixmap;
	PixmapPtr mask_pixmap;
	PixmapPtr dst_pixmap;
	bool src_fallback, mask_fallback;

	if (!gen4_check_dst_format(dst->format)) {
		DBG(("%s: unknown destination format: %d\n",
		     __FUNCTION__, dst->format));
		return true;
	}

	dst_pixmap = get_drawable_pixmap(dst->pDrawable);

	src_pixmap = src->pDrawable ? get_drawable_pixmap(src->pDrawable) : NULL;
	src_fallback = source_fallback(sna, src, src_pixmap,
				       dst->polyMode == PolyModePrecise);

	if (mask) {
		mask_pixmap = mask->pDrawable ? get_drawable_pixmap(mask->pDrawable) : NULL;
		mask_fallback = source_fallback(sna, mask, mask_pixmap,
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

	if (mask_fallback) {
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

	if (!gen4_check_repeat(mask))
		return false;

	if (!gen4_check_filter(mask))
		return false;

	if (!gen4_check_format(mask->format))
		return false;

	DBG(("%s: reusing source channel for mask with a twist\n",
	     __FUNCTION__));

	*mc = *sc;
	mc->repeat = gen4_repeat(mask->repeat ? mask->repeatType : RepeatNone);
	mc->filter = gen4_filter(mask->filter);
	mc->pict_format = mask->format;
	mc->card_format = gen4_get_card_format(mask->format);
	mc->bo = kgem_bo_reference(mc->bo);
	return true;
}

static bool
gen4_render_composite(struct sna *sna,
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
	DBG(("%s: %dx%d, current mode=%d\n", __FUNCTION__,
	     width, height, sna->kgem.mode));

	if (op >= ARRAY_SIZE(gen4_blend_op))
		return false;

	if (mask == NULL &&
	    sna_blt_composite(sna, op,
			      src, dst,
			      src_x, src_y,
			      dst_x, dst_y,
			      width, height,
			      flags, tmp))
		return true;

	if (gen4_composite_fallback(sna, src, mask, dst))
		goto fallback;

	if (need_tiling(sna, width, height))
		return sna_tiling_composite(op, src, mask, dst,
					    src_x, src_y,
					    msk_x, msk_y,
					    dst_x, dst_y,
					    width, height,
					    tmp);

	if (!gen4_composite_set_target(sna, tmp, dst,
				       dst_x, dst_y, width, height,
				       flags & COMPOSITE_PARTIAL || op > PictOpSrc)) {
		DBG(("%s: failed to set composite target\n", __FUNCTION__));
		goto fallback;
	}

	tmp->op = op;
	switch (gen4_composite_picture(sna, src, &tmp->src,
				       src_x, src_y,
				       width, height,
				       dst_x, dst_y,
				       dst->polyMode == PolyModePrecise)) {
	case -1:
		DBG(("%s: failed to prepare source\n", __FUNCTION__));
		goto cleanup_dst;
	case 0:
		if (!gen4_channel_init_solid(sna, &tmp->src, 0))
			goto cleanup_dst;
		/* fall through to fixup */
	case 1:
		if (mask == NULL &&
		    sna_blt_composite__convert(sna,
					       dst_x, dst_y, width, height,
					       tmp))
			return true;

		gen4_composite_channel_convert(&tmp->src);
		break;
	}

	tmp->is_affine = tmp->src.is_affine;
	tmp->has_component_alpha = false;
	tmp->need_magic_ca_pass = false;

	if (mask) {
		if (mask->componentAlpha && PICT_FORMAT_RGB(mask->format)) {
			tmp->has_component_alpha = true;

			/* Check if it's component alpha that relies on a source alpha and on
			 * the source value.  We can only get one of those into the single
			 * source value that we get to blend with.
			 */
			if (gen4_blend_op[op].src_alpha &&
			    (gen4_blend_op[op].src_blend != GEN4_BLENDFACTOR_ZERO)) {
				if (op != PictOpOver) {
					DBG(("%s -- fallback: unhandled component alpha blend\n",
					     __FUNCTION__));

					goto cleanup_src;
				}

				tmp->need_magic_ca_pass = true;
				tmp->op = PictOpOutReverse;
			}
		}

		if (!reuse_source(sna,
				  src, &tmp->src, src_x, src_y,
				  mask, &tmp->mask, msk_x, msk_y)) {
			switch (gen4_composite_picture(sna, mask, &tmp->mask,
						       msk_x, msk_y,
						       width, height,
						       dst_x, dst_y,
						       dst->polyMode == PolyModePrecise)) {
			case -1:
				DBG(("%s: failed to prepare mask\n", __FUNCTION__));
				goto cleanup_src;
			case 0:
				if (!gen4_channel_init_solid(sna, &tmp->mask, 0))
					goto cleanup_src;
				/* fall through to fixup */
			case 1:
				gen4_composite_channel_convert(&tmp->mask);
				break;
			}
		}

		tmp->is_affine &= tmp->mask.is_affine;
	}

	tmp->u.gen4.wm_kernel =
		gen4_choose_composite_kernel(tmp->op,
					     tmp->mask.bo != NULL,
					     tmp->has_component_alpha,
					     tmp->is_affine);
	tmp->u.gen4.ve_id = gen4_choose_composite_emitter(sna, tmp);

	tmp->blt   = gen4_render_composite_blt;
	tmp->box   = gen4_render_composite_box;
	tmp->boxes = gen4_render_composite_boxes__blt;
	if (tmp->emit_boxes) {
		tmp->boxes = gen4_render_composite_boxes;
#if !FORCE_FLUSH
		tmp->thread_boxes = gen4_render_composite_boxes__thread;
#endif
	}
	tmp->done  = gen4_render_composite_done;

	if (!kgem_check_bo(&sna->kgem,
			   tmp->dst.bo, tmp->src.bo, tmp->mask.bo,
			   NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem,
				     tmp->dst.bo, tmp->src.bo, tmp->mask.bo,
				     NULL))
			goto cleanup_mask;
	}

	gen4_align_vertex(sna, tmp);
	gen4_bind_surfaces(sna, tmp);
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
gen4_render_composite_spans_box(struct sna *sna,
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

	gen4_get_rectangles(sna, &op->base, 1, gen4_bind_surfaces);
	op->prim_emit(sna, op, box, opacity);
}

static void
gen4_render_composite_spans_boxes(struct sna *sna,
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

		nbox_this_time = gen4_get_rectangles(sna, &op->base, nbox,
						     gen4_bind_surfaces);
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
gen4_render_composite_spans_boxes__thread(struct sna *sna,
					  const struct sna_composite_spans_op *op,
					  const struct sna_opacity_box *box,
					  int nbox)
{
	DBG(("%s: nbox=%d, src=+(%d, %d), dst=+(%d, %d)\n",
	     __FUNCTION__, nbox,
	     op->base.src.offset[0], op->base.src.offset[1],
	     op->base.dst.x, op->base.dst.y));
	assert(nbox);

	sna_vertex_lock(&sna->render);
	do {
		int nbox_this_time;
		float *v;

		nbox_this_time = gen4_get_rectangles(sna, &op->base, nbox,
						     gen4_bind_surfaces);
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
gen4_render_composite_spans_done(struct sna *sna,
				 const struct sna_composite_spans_op *op)
{
	if (sna->render.vertex_offset)
		gen4_vertex_flush(sna);

	DBG(("%s()\n", __FUNCTION__));

	kgem_bo_destroy(&sna->kgem, op->base.src.bo);
	sna_render_composite_redirect_done(sna, &op->base);
}

static bool
gen4_check_composite_spans(struct sna *sna,
			   uint8_t op, PicturePtr src, PicturePtr dst,
			   int16_t width, int16_t height,
			   unsigned flags)
{
	DBG(("%s: op=%d, width=%d, height=%d, flags=%x\n",
	     __FUNCTION__, op, width, height, flags));

	if (op >= ARRAY_SIZE(gen4_blend_op))
		return false;

	if (gen4_composite_fallback(sna, src, NULL, dst)) {
		DBG(("%s: operation would fallback\n", __FUNCTION__));
		return false;
	}

	if (need_tiling(sna, width, height) &&
	    !is_gpu(sna, dst->pDrawable, PREFER_GPU_SPANS)) {
		DBG(("%s: fallback, tiled operation not on GPU\n",
		     __FUNCTION__));
		return false;
	}

	if (FORCE_SPANS)
		return FORCE_SPANS > 0;

	if ((flags & COMPOSITE_SPANS_RECTILINEAR) == 0) {
		struct sna_pixmap *priv;

		if (FORCE_NONRECTILINEAR_SPANS)
			return FORCE_NONRECTILINEAR_SPANS > 0;

		if ((sna->render.prefer_gpu & PREFER_GPU_SPANS) == 0)
			return false;

		priv = sna_pixmap_from_drawable(dst->pDrawable);
		assert(priv);

		if (priv->cpu_bo &&
		    __kgem_bo_is_busy(&sna->kgem, priv->cpu_bo))
			return true;

		if (flags & COMPOSITE_SPANS_INPLACE_HINT)
			return false;

		return priv->gpu_bo && kgem_bo_is_busy(priv->gpu_bo);
	}

	return true;
}

static bool
gen4_render_composite_spans(struct sna *sna,
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

	assert(gen4_check_composite_spans(sna, op, src, dst, width, height, flags));

	if (need_tiling(sna, width, height)) {
		DBG(("%s: tiling, operation (%dx%d) too wide for pipeline\n",
		     __FUNCTION__, width, height));
		return sna_tiling_composite_spans(op, src, dst,
						  src_x, src_y, dst_x, dst_y,
						  width, height, flags, tmp);
	}

	tmp->base.op = op;
	if (!gen4_composite_set_target(sna, &tmp->base, dst,
				       dst_x, dst_y, width, height, true))
		return false;

	switch (gen4_composite_picture(sna, src, &tmp->base.src,
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
		gen4_composite_channel_convert(&tmp->base.src);
		break;
	}

	tmp->base.mask.bo = NULL;
	tmp->base.mask.filter = SAMPLER_FILTER_NEAREST;
	tmp->base.mask.repeat = SAMPLER_EXTEND_NONE;

	tmp->base.is_affine = tmp->base.src.is_affine;
	tmp->base.has_component_alpha = false;
	tmp->base.need_magic_ca_pass = false;

	tmp->base.u.gen4.ve_id = gen4_choose_spans_emitter(sna, tmp);
	tmp->base.u.gen4.wm_kernel = WM_KERNEL_OPACITY | !tmp->base.is_affine;

	tmp->box   = gen4_render_composite_spans_box;
	tmp->boxes = gen4_render_composite_spans_boxes;
	if (tmp->emit_boxes)
		tmp->thread_boxes = gen4_render_composite_spans_boxes__thread;
	tmp->done  = gen4_render_composite_spans_done;

	if (!kgem_check_bo(&sna->kgem,
			   tmp->base.dst.bo, tmp->base.src.bo,
			   NULL))  {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem,
				   tmp->base.dst.bo, tmp->base.src.bo,
				   NULL))
			goto cleanup_src;
	}

	gen4_align_vertex(sna, &tmp->base);
	gen4_bind_surfaces(sna, &tmp->base);
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
gen4_copy_bind_surfaces(struct sna *sna, const struct sna_composite_op *op)
{
	uint32_t *binding_table;
	uint16_t offset, dirty;

	gen4_get_batch(sna, op);
	dirty = kgem_bo_is_dirty(op->dst.bo);

	binding_table = gen4_composite_get_binding_table(sna, &offset);

	binding_table[0] =
		gen4_bind_bo(sna,
			     op->dst.bo, op->dst.width, op->dst.height,
			     gen4_get_dest_format(op->dst.format),
			     true);
	binding_table[1] =
		gen4_bind_bo(sna,
			     op->src.bo, op->src.width, op->src.height,
			     op->src.card_format,
			     false);

	if (sna->kgem.surface == offset &&
	    *(uint64_t *)(sna->kgem.batch + sna->render_state.gen4.surface_table) == *(uint64_t*)binding_table) {
		sna->kgem.surface += sizeof(struct gen4_surface_state_padded) / sizeof(uint32_t);
		offset = sna->render_state.gen4.surface_table;
	}

	if (!ALWAYS_FLUSH && sna->kgem.batch[sna->render_state.gen4.surface_table] == binding_table[0])
		dirty = 0;

	gen4_emit_state(sna, op, offset | dirty);
}

static void
gen4_render_copy_one(struct sna *sna,
		     const struct sna_composite_op *op,
		     int sx, int sy,
		     int w, int h,
		     int dx, int dy)
{
	gen4_get_rectangles(sna, op, 1, gen4_copy_bind_surfaces);

	OUT_VERTEX(dx+w, dy+h);
	OUT_VERTEX_F((sx+w)*op->src.scale[0]);
	OUT_VERTEX_F((sy+h)*op->src.scale[1]);

	OUT_VERTEX(dx, dy+h);
	OUT_VERTEX_F(sx*op->src.scale[0]);
	OUT_VERTEX_F((sy+h)*op->src.scale[1]);

	OUT_VERTEX(dx, dy);
	OUT_VERTEX_F(sx*op->src.scale[0]);
	OUT_VERTEX_F(sy*op->src.scale[1]);
}

static bool
gen4_render_copy_boxes(struct sna *sna, uint8_t alu,
		       const DrawableRec *src, struct kgem_bo *src_bo, int16_t src_dx, int16_t src_dy,
		       const DrawableRec *dst, struct kgem_bo *dst_bo, int16_t dst_dx, int16_t dst_dy,
		       const BoxRec *box, int n, unsigned flags)
{
	struct sna_composite_op tmp;

	DBG(("%s x %d\n", __FUNCTION__, n));

	if (sna_blt_compare_depth(src, dst) &&
	    sna_blt_copy_boxes(sna, alu,
			       src_bo, src_dx, src_dy,
			       dst_bo, dst_dx, dst_dy,
			       dst->bitsPerPixel,
			       box, n))
		return true;

	if (!(alu == GXcopy || alu == GXclear) || src_bo == dst_bo) {
fallback_blt:
		if (!sna_blt_compare_depth(src, dst))
			return false;

		return sna_blt_copy_boxes_fallback(sna, alu,
						   src, src_bo, src_dx, src_dy,
						   dst, dst_bo, dst_dx, dst_dy,
						   box, n);
	}

	memset(&tmp, 0, sizeof(tmp));

	DBG(("%s (%d, %d)->(%d, %d) x %d\n",
	     __FUNCTION__, src_dx, src_dy, dst_dx, dst_dy, n));

	if (dst->depth == src->depth) {
		tmp.dst.format = sna_render_format_for_depth(dst->depth);
		tmp.src.pict_format = tmp.dst.format;
	} else {
		tmp.dst.format = sna_format_for_depth(dst->depth);
		tmp.src.pict_format = sna_format_for_depth(src->depth);
	}
	if (!gen4_check_format(tmp.src.pict_format))
		goto fallback_blt;

	tmp.op = alu == GXcopy ? PictOpSrc : PictOpClear;

	tmp.dst.pixmap = (PixmapPtr)dst;
	tmp.dst.width  = dst->width;
	tmp.dst.height = dst->height;
	tmp.dst.x = tmp.dst.y = 0;
	tmp.dst.bo = dst_bo;
	tmp.damage = NULL;

	sna_render_composite_redirect_init(&tmp);
	if (too_large(tmp.dst.width, tmp.dst.height)) {
		BoxRec extents = box[0];
		int i;

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

	tmp.src.filter = SAMPLER_FILTER_NEAREST;
	tmp.src.repeat = SAMPLER_EXTEND_NONE;
	tmp.src.card_format = gen4_get_card_format(tmp.src.pict_format);
	if (too_large(src->width, src->height)) {
		BoxRec extents = box[0];
		int i;

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
		tmp.src.bo = kgem_bo_reference(src_bo);
		tmp.src.width  = src->width;
		tmp.src.height = src->height;
		tmp.src.offset[0] = tmp.src.offset[1] = 0;
		tmp.src.scale[0] = 1.f/src->width;
		tmp.src.scale[1] = 1.f/src->height;
	}

	tmp.is_affine = true;
	tmp.floats_per_vertex = 3;
	tmp.floats_per_rect = 9;
	tmp.u.gen4.wm_kernel = WM_KERNEL;
	tmp.u.gen4.ve_id = 2;

	if (!kgem_check_bo(&sna->kgem, dst_bo, src_bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem, dst_bo, src_bo, NULL)) {
			kgem_bo_destroy(&sna->kgem, tmp.src.bo);
			if (tmp.redirect.real_bo)
				kgem_bo_destroy(&sna->kgem, tmp.dst.bo);

			goto fallback_blt;
		}
	}

	dst_dx += tmp.dst.x;
	dst_dy += tmp.dst.y;
	tmp.dst.x = tmp.dst.y = 0;

	src_dx += tmp.src.offset[0];
	src_dy += tmp.src.offset[1];

	gen4_align_vertex(sna, &tmp);
	gen4_copy_bind_surfaces(sna, &tmp);

	do {
		gen4_render_copy_one(sna, &tmp,
				     box->x1 + src_dx, box->y1 + src_dy,
				     box->x2 - box->x1, box->y2 - box->y1,
				     box->x1 + dst_dx, box->y1 + dst_dy);
		box++;
	} while (--n);

	gen4_vertex_flush(sna);
	sna_render_composite_redirect_done(sna, &tmp);
	kgem_bo_destroy(&sna->kgem, tmp.src.bo);
	return true;

fallback_tiled_dst:
	if (tmp.redirect.real_bo)
		kgem_bo_destroy(&sna->kgem, tmp.dst.bo);
fallback_tiled:
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
gen4_render_copy_blt(struct sna *sna,
		     const struct sna_copy_op *op,
		     int16_t sx, int16_t sy,
		     int16_t w,  int16_t h,
		     int16_t dx, int16_t dy)
{
	gen4_render_copy_one(sna, &op->base, sx, sy, w, h, dx, dy);
}

static void
gen4_render_copy_done(struct sna *sna, const struct sna_copy_op *op)
{
	if (sna->render.vertex_offset)
		gen4_vertex_flush(sna);
}

static bool
gen4_render_copy(struct sna *sna, uint8_t alu,
		 PixmapPtr src, struct kgem_bo *src_bo,
		 PixmapPtr dst, struct kgem_bo *dst_bo,
		 struct sna_copy_op *op)
{
	DBG(("%s: src=%ld, dst=%ld, alu=%d\n",
	     __FUNCTION__,
	     src->drawable.serialNumber,
	     dst->drawable.serialNumber,
	     alu));

	if (sna_blt_compare_depth(&src->drawable, &dst->drawable) &&
	    sna_blt_copy(sna, alu,
			 src_bo, dst_bo,
			 dst->drawable.bitsPerPixel,
			 op))
		return true;

	if (!(alu == GXcopy || alu == GXclear) || src_bo == dst_bo ||
	    too_large(src->drawable.width, src->drawable.height) ||
	    too_large(dst->drawable.width, dst->drawable.height)) {
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
	if (!gen4_check_format(op->base.src.pict_format))
		goto fallback;

	op->base.op = alu == GXcopy ? PictOpSrc : PictOpClear;

	op->base.dst.pixmap = dst;
	op->base.dst.width  = dst->drawable.width;
	op->base.dst.height = dst->drawable.height;
	op->base.dst.bo = dst_bo;

	op->base.src.bo = src_bo;
	op->base.src.card_format =
		gen4_get_card_format(op->base.src.pict_format);
	op->base.src.width  = src->drawable.width;
	op->base.src.height = src->drawable.height;
	op->base.src.scale[0] = 1.f/src->drawable.width;
	op->base.src.scale[1] = 1.f/src->drawable.height;
	op->base.src.filter = SAMPLER_FILTER_NEAREST;
	op->base.src.repeat = SAMPLER_EXTEND_NONE;

	op->base.is_affine = true;
	op->base.floats_per_vertex = 3;
	op->base.floats_per_rect = 9;
	op->base.u.gen4.wm_kernel = WM_KERNEL;
	op->base.u.gen4.ve_id = 2;

	if (!kgem_check_bo(&sna->kgem, dst_bo, src_bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem, dst_bo, src_bo, NULL))
			goto fallback;
	}

	if (kgem_bo_is_dirty(src_bo)) {
		if (sna_blt_compare_depth(&src->drawable, &dst->drawable) &&
		    sna_blt_copy(sna, alu,
				 src_bo, dst_bo,
				 dst->drawable.bitsPerPixel,
				 op))
			return true;
	}

	gen4_align_vertex(sna, &op->base);
	gen4_copy_bind_surfaces(sna, &op->base);

	op->blt  = gen4_render_copy_blt;
	op->done = gen4_render_copy_done;
	return true;
}

static void
gen4_render_fill_rectangle(struct sna *sna,
			   const struct sna_composite_op *op,
			   int x, int y, int w, int h)
{
	gen4_get_rectangles(sna, op, 1, gen4_bind_surfaces);

	OUT_VERTEX(x+w, y+h);
	OUT_VERTEX_F(.5);

	OUT_VERTEX(x, y+h);
	OUT_VERTEX_F(.5);

	OUT_VERTEX(x, y);
	OUT_VERTEX_F(.5);
}

static bool
gen4_render_fill_boxes(struct sna *sna,
		       CARD8 op,
		       PictFormat format,
		       const xRenderColor *color,
		       const DrawableRec *dst, struct kgem_bo *dst_bo,
		       const BoxRec *box, int n)
{
	struct sna_composite_op tmp;
	uint32_t pixel;

	if (op >= ARRAY_SIZE(gen4_blend_op)) {
		DBG(("%s: fallback due to unhandled blend op: %d\n",
		     __FUNCTION__, op));
		return false;
	}

	if (op <= PictOpSrc) {
		uint8_t alu = GXinvalid;

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

		if (alu != GXinvalid &&
		    sna_blt_fill_boxes(sna, alu,
				       dst_bo, dst->bitsPerPixel,
				       pixel, box, n))
			return true;

		if (!gen4_check_dst_format(format))
			return false;

		if (too_large(dst->width, dst->height))
			return sna_tiling_fill_boxes(sna, op, format, color,
						     dst, dst_bo, box, n);
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

	DBG(("%s(%08x x %d)\n", __FUNCTION__, pixel, n));

	memset(&tmp, 0, sizeof(tmp));

	tmp.op = op;

	tmp.dst.pixmap = (PixmapPtr)dst;
	tmp.dst.width  = dst->width;
	tmp.dst.height = dst->height;
	tmp.dst.format = format;
	tmp.dst.bo = dst_bo;

	gen4_channel_init_solid(sna, &tmp.src, pixel);

	tmp.is_affine = true;
	tmp.floats_per_vertex = 2;
	tmp.floats_per_rect = 6;
	tmp.u.gen4.wm_kernel = WM_KERNEL;
	tmp.u.gen4.ve_id = 1;

	if (!kgem_check_bo(&sna->kgem, dst_bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem, dst_bo, NULL))
			return false;
	}

	gen4_align_vertex(sna, &tmp);
	gen4_bind_surfaces(sna, &tmp);

	do {
		gen4_render_fill_rectangle(sna, &tmp,
					   box->x1, box->y1,
					   box->x2 - box->x1,
					   box->y2 - box->y1);
		box++;
	} while (--n);

	gen4_vertex_flush(sna);
	kgem_bo_destroy(&sna->kgem, tmp.src.bo);
	return true;
}

static void
gen4_render_fill_op_blt(struct sna *sna, const struct sna_fill_op *op,
			int16_t x, int16_t y, int16_t w, int16_t h)
{
	gen4_render_fill_rectangle(sna, &op->base, x, y, w, h);
}

fastcall static void
gen4_render_fill_op_box(struct sna *sna,
			const struct sna_fill_op *op,
			const BoxRec *box)
{
	gen4_render_fill_rectangle(sna, &op->base,
				   box->x1, box->y1,
				   box->x2-box->x1, box->y2-box->y1);
}

fastcall static void
gen4_render_fill_op_boxes(struct sna *sna,
			  const struct sna_fill_op *op,
			  const BoxRec *box,
			  int nbox)
{
	do {
		gen4_render_fill_rectangle(sna, &op->base,
					   box->x1, box->y1,
					   box->x2-box->x1, box->y2-box->y1);
		box++;
	} while (--nbox);
}

static void
gen4_render_fill_op_done(struct sna *sna, const struct sna_fill_op *op)
{
	if (sna->render.vertex_offset)
		gen4_vertex_flush(sna);
	kgem_bo_destroy(&sna->kgem, op->base.src.bo);
}

static bool
gen4_render_fill(struct sna *sna, uint8_t alu,
		 PixmapPtr dst, struct kgem_bo *dst_bo,
		 uint32_t color, unsigned flags,
		 struct sna_fill_op *op)
{
	if (sna_blt_fill(sna, alu,
			 dst_bo, dst->drawable.bitsPerPixel,
			 color,
			 op))
		return true;

	if (!(alu == GXcopy || alu == GXclear) ||
	    too_large(dst->drawable.width, dst->drawable.height))
		return sna_blt_fill(sna, alu,
				    dst_bo, dst->drawable.bitsPerPixel,
				    color,
				    op);

	if (alu == GXclear)
		color = 0;

	op->base.op = color == 0 ? PictOpClear : PictOpSrc;

	op->base.dst.pixmap = dst;
	op->base.dst.width  = dst->drawable.width;
	op->base.dst.height = dst->drawable.height;
	op->base.dst.format = sna_format_for_depth(dst->drawable.depth);
	op->base.dst.bo = dst_bo;
	op->base.dst.x = op->base.dst.y = 0;

	op->base.need_magic_ca_pass = 0;
	op->base.has_component_alpha = 0;

	gen4_channel_init_solid(sna, &op->base.src,
				sna_rgba_for_color(color,
						   dst->drawable.depth));
	op->base.mask.bo = NULL;

	op->base.is_affine = true;
	op->base.floats_per_vertex = 2;
	op->base.floats_per_rect = 6;
	op->base.u.gen4.wm_kernel = WM_KERNEL;
	op->base.u.gen4.ve_id = 1;

	if (!kgem_check_bo(&sna->kgem, dst_bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem, dst_bo, NULL)) {
			kgem_bo_destroy(&sna->kgem, op->base.src.bo);
			return false;
		}
	}

	gen4_align_vertex(sna, &op->base);
	gen4_bind_surfaces(sna, &op->base);

	op->blt   = gen4_render_fill_op_blt;
	op->box   = gen4_render_fill_op_box;
	op->boxes = gen4_render_fill_op_boxes;
	op->points = NULL;
	op->done  = gen4_render_fill_op_done;
	return true;
}

static bool
gen4_render_fill_one_try_blt(struct sna *sna, PixmapPtr dst, struct kgem_bo *bo,
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
gen4_render_fill_one(struct sna *sna, PixmapPtr dst, struct kgem_bo *bo,
		     uint32_t color,
		     int16_t x1, int16_t y1,
		     int16_t x2, int16_t y2,
		     uint8_t alu)
{
	struct sna_composite_op tmp;

	DBG(("%s: color=%08x\n", __FUNCTION__, color));

	if (gen4_render_fill_one_try_blt(sna, dst, bo, color,
					 x1, y1, x2, y2, alu))
		return true;

	/* Must use the BLT if we can't RENDER... */
	if (!(alu == GXcopy || alu == GXclear) ||
	    too_large(dst->drawable.width, dst->drawable.height))
		return false;

	if (alu == GXclear)
		color = 0;

	tmp.op = color == 0 ? PictOpClear : PictOpSrc;

	tmp.dst.pixmap = dst;
	tmp.dst.width  = dst->drawable.width;
	tmp.dst.height = dst->drawable.height;
	tmp.dst.format = sna_format_for_depth(dst->drawable.depth);
	tmp.dst.bo = bo;
	tmp.dst.x = tmp.dst.y = 0;

	gen4_channel_init_solid(sna, &tmp.src,
				sna_rgba_for_color(color,
						   dst->drawable.depth));
	tmp.mask.bo = NULL;
	tmp.mask.filter = SAMPLER_FILTER_NEAREST;
	tmp.mask.repeat = SAMPLER_EXTEND_NONE;

	tmp.is_affine = true;
	tmp.floats_per_vertex = 2;
	tmp.floats_per_rect = 6;
	tmp.has_component_alpha = false;
	tmp.need_magic_ca_pass = false;

	tmp.u.gen4.wm_kernel = WM_KERNEL;
	tmp.u.gen4.ve_id = 1;

	if (!kgem_check_bo(&sna->kgem, bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo(&sna->kgem, bo, NULL)) {
			kgem_bo_destroy(&sna->kgem, tmp.src.bo);
			return false;
		}
	}

	gen4_align_vertex(sna, &tmp);
	gen4_bind_surfaces(sna, &tmp);

	gen4_render_fill_rectangle(sna, &tmp, x1, y1, x2 - x1, y2 - y1);

	gen4_vertex_flush(sna);
	kgem_bo_destroy(&sna->kgem, tmp.src.bo);

	return true;
}

static void gen4_render_reset(struct sna *sna)
{
	sna->render_state.gen4.needs_invariant = true;
	sna->render_state.gen4.needs_urb = true;
	sna->render_state.gen4.ve_id = -1;
	sna->render_state.gen4.last_primitive = -1;
	sna->render_state.gen4.last_pipelined_pointers = -1;

	sna->render_state.gen4.drawrect_offset = -1;
	sna->render_state.gen4.drawrect_limit = -1;
	sna->render_state.gen4.surface_table = 0;

	if (sna->render.vbo && !kgem_bo_can_map(&sna->kgem, sna->render.vbo)) {
		DBG(("%s: discarding unmappable vbo\n", __FUNCTION__));
		discard_vbo(sna);
	}

	sna->render.vertex_offset = 0;
	sna->render.nvertex_reloc = 0;
	sna->render.vb_id = 0;
}

static void gen4_render_fini(struct sna *sna)
{
	kgem_bo_destroy(&sna->kgem, sna->render_state.gen4.general_bo);
}

static uint32_t gen4_create_vs_unit_state(struct sna_static_stream *stream)
{
	struct gen4_vs_unit_state *vs = sna_static_stream_map(stream, sizeof(*vs), 32);

	/* Set up the vertex shader to be disabled (passthrough) */
	vs->thread4.nr_urb_entries = URB_VS_ENTRIES;
	vs->thread4.urb_entry_allocation_size = URB_VS_ENTRY_SIZE - 1;
	vs->vs6.vs_enable = 0;
	vs->vs6.vert_cache_disable = 1;

	return sna_static_stream_offsetof(stream, vs);
}

static uint32_t gen4_create_sf_state(struct sna_static_stream *stream,
				     uint32_t kernel)
{
	struct gen4_sf_unit_state *sf;

	sf = sna_static_stream_map(stream, sizeof(*sf), 32);

	sf->thread0.grf_reg_count = GEN4_GRF_BLOCKS(SF_KERNEL_NUM_GRF);
	sf->thread0.kernel_start_pointer = kernel >> 6;
	sf->thread3.const_urb_entry_read_length = 0;	/* no const URBs */
	sf->thread3.const_urb_entry_read_offset = 0;	/* no const URBs */
	sf->thread3.urb_entry_read_length = 1;	/* 1 URB per vertex */
	/* don't smash vertex header, read start from dw8 */
	sf->thread3.urb_entry_read_offset = 1;
	sf->thread3.dispatch_grf_start_reg = 3;
	sf->thread4.max_threads = GEN4_MAX_SF_THREADS - 1;
	sf->thread4.urb_entry_allocation_size = URB_SF_ENTRY_SIZE - 1;
	sf->thread4.nr_urb_entries = URB_SF_ENTRIES;
	sf->sf5.viewport_transform = false;	/* skip viewport */
	sf->sf6.cull_mode = GEN4_CULLMODE_NONE;
	sf->sf6.scissor = 0;
	sf->sf7.trifan_pv = 2;
	sf->sf6.dest_org_vbias = 0x8;
	sf->sf6.dest_org_hbias = 0x8;

	return sna_static_stream_offsetof(stream, sf);
}

static uint32_t gen4_create_sampler_state(struct sna_static_stream *stream,
					  sampler_filter_t src_filter,
					  sampler_extend_t src_extend,
					  sampler_filter_t mask_filter,
					  sampler_extend_t mask_extend)
{
	struct gen4_sampler_state *sampler_state;

	sampler_state = sna_static_stream_map(stream,
					      sizeof(struct gen4_sampler_state) * 2,
					      32);
	sampler_state_init(&sampler_state[0], src_filter, src_extend);
	sampler_state_init(&sampler_state[1], mask_filter, mask_extend);

	return sna_static_stream_offsetof(stream, sampler_state);
}

static void gen4_init_wm_state(struct gen4_wm_unit_state *wm,
			       int gen,
			       bool has_mask,
			       uint32_t kernel,
			       uint32_t sampler)
{
	assert((kernel & 63) == 0);
	wm->thread0.kernel_start_pointer = kernel >> 6;
	wm->thread0.grf_reg_count = GEN4_GRF_BLOCKS(PS_KERNEL_NUM_GRF);

	wm->thread1.single_program_flow = 0;

	wm->thread3.const_urb_entry_read_length = 0;
	wm->thread3.const_urb_entry_read_offset = 0;

	wm->thread3.urb_entry_read_offset = 0;
	wm->thread3.dispatch_grf_start_reg = 3;

	assert((sampler & 31) == 0);
	wm->wm4.sampler_state_pointer = sampler >> 5;
	wm->wm4.sampler_count = 1;

	wm->wm5.max_threads = gen >= 045 ? G4X_MAX_WM_THREADS - 1 : GEN4_MAX_WM_THREADS - 1;
	wm->wm5.transposed_urb_read = 0;
	wm->wm5.thread_dispatch_enable = 1;
	/* just use 16-pixel dispatch (4 subspans), don't need to change kernel
	 * start point
	 */
	wm->wm5.enable_16_pix = 1;
	wm->wm5.enable_8_pix = 0;
	wm->wm5.early_depth_test = 1;

	/* Each pair of attributes (src/mask coords) is two URB entries */
	if (has_mask) {
		wm->thread1.binding_table_entry_count = 3;
		wm->thread3.urb_entry_read_length = 4;
	} else {
		wm->thread1.binding_table_entry_count = 2;
		wm->thread3.urb_entry_read_length = 2;
	}
}

static uint32_t gen4_create_cc_unit_state(struct sna_static_stream *stream)
{
	uint8_t *ptr, *base;
	int i, j;

	base = ptr =
		sna_static_stream_map(stream,
				      GEN4_BLENDFACTOR_COUNT*GEN4_BLENDFACTOR_COUNT*64,
				      64);

	for (i = 0; i < GEN4_BLENDFACTOR_COUNT; i++) {
		for (j = 0; j < GEN4_BLENDFACTOR_COUNT; j++) {
			struct gen4_cc_unit_state *state =
				(struct gen4_cc_unit_state *)ptr;

			state->cc3.blend_enable =
				!(j == GEN4_BLENDFACTOR_ZERO && i == GEN4_BLENDFACTOR_ONE);

			state->cc5.logicop_func = 0xc;	/* COPY */
			state->cc5.ia_blend_function = GEN4_BLENDFUNCTION_ADD;

			/* Fill in alpha blend factors same as color, for the future. */
			state->cc5.ia_src_blend_factor = i;
			state->cc5.ia_dest_blend_factor = j;

			state->cc6.blend_function = GEN4_BLENDFUNCTION_ADD;
			state->cc6.clamp_post_alpha_blend = 1;
			state->cc6.clamp_pre_alpha_blend = 1;
			state->cc6.src_blend_factor = i;
			state->cc6.dest_blend_factor = j;

			ptr += 64;
		}
	}

	return sna_static_stream_offsetof(stream, base);
}

static bool gen4_render_setup(struct sna *sna)
{
	struct gen4_render_state *state = &sna->render_state.gen4;
	struct sna_static_stream general;
	struct gen4_wm_unit_state_padded *wm_state;
	uint32_t sf, wm[KERNEL_COUNT];
	int i, j, k, l, m;

	sna_static_stream_init(&general);

	/* Zero pad the start. If you see an offset of 0x0 in the batchbuffer
	 * dumps, you know it points to zero.
	 */
	null_create(&general);

	sf = sna_static_stream_compile_sf(sna, &general, brw_sf_kernel__mask);
	for (m = 0; m < KERNEL_COUNT; m++) {
		if (wm_kernels[m].size) {
			wm[m] = sna_static_stream_add(&general,
						      wm_kernels[m].data,
						      wm_kernels[m].size,
						      64);
		} else {
			wm[m] = sna_static_stream_compile_wm(sna, &general,
							     wm_kernels[m].data,
							     16);
		}
	}

	state->vs = gen4_create_vs_unit_state(&general);
	state->sf = gen4_create_sf_state(&general, sf);

	wm_state = sna_static_stream_map(&general,
					  sizeof(*wm_state) * KERNEL_COUNT *
					  FILTER_COUNT * EXTEND_COUNT *
					  FILTER_COUNT * EXTEND_COUNT,
					  64);
	state->wm = sna_static_stream_offsetof(&general, wm_state);
	for (i = 0; i < FILTER_COUNT; i++) {
		for (j = 0; j < EXTEND_COUNT; j++) {
			for (k = 0; k < FILTER_COUNT; k++) {
				for (l = 0; l < EXTEND_COUNT; l++) {
					uint32_t sampler_state;

					sampler_state =
						gen4_create_sampler_state(&general,
									  i, j,
									  k, l);

					for (m = 0; m < KERNEL_COUNT; m++) {
						gen4_init_wm_state(&wm_state->state,
								   sna->kgem.gen,
								   wm_kernels[m].has_mask,
								   wm[m], sampler_state);
						wm_state++;
					}
				}
			}
		}
	}

	state->cc = gen4_create_cc_unit_state(&general);

	state->general_bo = sna_static_stream_fini(sna, &general);
	return state->general_bo != NULL;
}

const char *gen4_render_init(struct sna *sna, const char *backend)
{
	if (!gen4_render_setup(sna))
		return backend;

	sna->kgem.retire = gen4_render_retire;
	sna->kgem.expire = gen4_render_expire;

#if !NO_COMPOSITE
	sna->render.composite = gen4_render_composite;
	sna->render.prefer_gpu |= PREFER_GPU_RENDER;
#endif
#if !NO_COMPOSITE_SPANS
	sna->render.check_composite_spans = gen4_check_composite_spans;
	sna->render.composite_spans = gen4_render_composite_spans;
	if (0)
		sna->render.prefer_gpu |= PREFER_GPU_SPANS;
#endif

#if !NO_VIDEO
	sna->render.video = gen4_render_video;
#endif

#if !NO_COPY_BOXES
	sna->render.copy_boxes = gen4_render_copy_boxes;
#endif
#if !NO_COPY
	sna->render.copy = gen4_render_copy;
#endif

#if !NO_FILL_BOXES
	sna->render.fill_boxes = gen4_render_fill_boxes;
#endif
#if !NO_FILL
	sna->render.fill = gen4_render_fill;
#endif
#if !NO_FILL_ONE
	sna->render.fill_one = gen4_render_fill_one;
#endif

	sna->render.flush = gen4_render_flush;
	sna->render.reset = gen4_render_reset;
	sna->render.fini = gen4_render_fini;

	sna->render.max_3d_size = GEN4_MAX_3D_SIZE;
	sna->render.max_3d_pitch = 1 << 18;
	return sna->kgem.gen >= 045 ? "Eaglelake (gen4.5)" : "Broadwater (gen4)";
}
