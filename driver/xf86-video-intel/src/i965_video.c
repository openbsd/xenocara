/*
 * Copyright © 2006 Intel Corporation
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *    Keith Packard <keithp@keithp.com>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86xv.h"
#include "fourcc.h"

#include "intel.h"
#include "intel_hwmc.h"
#include "intel_video.h"
#include "i830_reg.h"
#include "i965_reg.h"
#include "brw_defines.h"
#include "brw_structs.h"
#include <string.h>


/* Make assert() work. */
#undef NDEBUG
#include <assert.h>

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
 * with the base texture coordinate. It was extracted from the Mesa driver.
 * It uses about 10 GRF registers.
 */

#define SF_KERNEL_NUM_GRF  16
#define SF_MAX_THREADS	   1

static const uint32_t sf_kernel_static[][4] = {
#include "exa_sf.g4b"
};

/*
 * Ok, this kernel picks up the required data flow values in g0 and g1
 * and passes those along in m0 and m1. In m2-m9, it sticks constant
 * values (bright pink).
 */

/* Our PS kernel uses less than 32 GRF registers (about 20) */
#define PS_KERNEL_NUM_GRF   32
#define PS_MAX_THREADS	   32

#define BRW_GRF_BLOCKS(nreg)	((nreg + 15) / 16 - 1)

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

/* new program for Ironlake */
static const uint32_t sf_kernel_static_gen5[][4] = {
#include "exa_sf.g4b.gen5"
};

static const uint32_t ps_kernel_packed_static_gen5[][4] = {
#include "exa_wm_xy.g4b.gen5"
#include "exa_wm_src_affine.g4b.gen5"
#include "exa_wm_src_sample_argb.g4b.gen5"
#include "exa_wm_yuv_rgb.g4b.gen5"
#include "exa_wm_write.g4b.gen5"
};

static const uint32_t ps_kernel_planar_static_gen5[][4] = {
#include "exa_wm_xy.g4b.gen5"
#include "exa_wm_src_affine.g4b.gen5"
#include "exa_wm_src_sample_planar.g4b.gen5"
#include "exa_wm_yuv_rgb.g4b.gen5"
#include "exa_wm_write.g4b.gen5"
};

/* programs for Sandybridge */
static const uint32_t ps_kernel_packed_static_gen6[][4] = {
#include "exa_wm_src_affine.g6b"
#include "exa_wm_src_sample_argb.g6b"
#include "exa_wm_yuv_rgb.g6b"
#include "exa_wm_write.g6b"
};

static const uint32_t ps_kernel_planar_static_gen6[][4] = {
#include "exa_wm_src_affine.g6b"
#include "exa_wm_src_sample_planar.g6b"
#include "exa_wm_yuv_rgb.g6b"
#include "exa_wm_write.g6b"
};

/* programs for Ivybridge */
static const uint32_t ps_kernel_packed_static_gen7[][4] = {
#include "exa_wm_src_affine.g7b"
#include "exa_wm_src_sample_argb.g7b"
#include "exa_wm_yuv_rgb.g7b"
#include "exa_wm_write.g7b"
};

static const uint32_t ps_kernel_planar_static_gen7[][4] = {
#include "exa_wm_src_affine.g7b"
#include "exa_wm_src_sample_planar.g7b"
#include "exa_wm_yuv_rgb.g7b"
#include "exa_wm_write.g7b"
};

#ifndef MAX2
#define MAX2(a,b) ((a) > (b) ? (a) : (b))
#endif

#define SURFACE_STATE_PADDED_SIZE_I965  ALIGN(sizeof(struct brw_surface_state), 32)
#define SURFACE_STATE_PADDED_SIZE_GEN7  ALIGN(sizeof(struct gen7_surface_state), 32)
#define SURFACE_STATE_PADDED_SIZE       MAX2(SURFACE_STATE_PADDED_SIZE_I965, SURFACE_STATE_PADDED_SIZE_GEN7)
#define SURFACE_STATE_OFFSET(index)     (SURFACE_STATE_PADDED_SIZE * index)

static uint32_t float_to_uint(float f)
{
	union {
		uint32_t i;
		float f;
	} x;
	x.f = f;
	return x.i;
}

#if 0
static struct {
	uint32_t svg_ctl;
	char *name;
} svg_ctl_bits[] = {
	{
	BRW_SVG_CTL_GS_BA, "General State Base Address"}, {
	BRW_SVG_CTL_SS_BA, "Surface State Base Address"}, {
	BRW_SVG_CTL_IO_BA, "Indirect Object Base Address"}, {
	BRW_SVG_CTL_GS_AUB, "Generate State Access Upper Bound"}, {
	BRW_SVG_CTL_IO_AUB, "Indirect Object Access Upper Bound"}, {
	BRW_SVG_CTL_SIP, "System Instruction Pointer"}, {
0, 0},};

static void brw_debug(ScrnInfoPtr scrn, char *when)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int i;
	uint32_t v;

	ErrorF("brw_debug: %s\n", when);
	for (i = 0; svg_ctl_bits[i].name; i++) {
		OUTREG(BRW_SVG_CTL, svg_ctl_bits[i].svg_ctl);
		v = INREG(BRW_SVG_RDATA);
		ErrorF("\t%34.34s: 0x%08x\n", svg_ctl_bits[i].name, v);
	}
}
#endif

#define WATCH_SF 0
#define WATCH_WIZ 0
#define WATCH_STATS 0

static void i965_pre_draw_debug(ScrnInfoPtr scrn)
{
#if 0
	intel_screen_private *intel = intel_get_screen_private(scrn);
#endif

#if 0
	ErrorF("before EU_ATT 0x%08x%08x EU_ATT_DATA 0x%08x%08x\n",
	       INREG(BRW_EU_ATT_1), INREG(BRW_EU_ATT_0),
	       INREG(BRW_EU_ATT_DATA_1), INREG(BRW_EU_ATT_DATA_0));

	OUTREG(BRW_VF_CTL,
	       BRW_VF_CTL_SNAPSHOT_MUX_SELECT_THREADID |
	       BRW_VF_CTL_SNAPSHOT_TYPE_VERTEX_INDEX |
	       BRW_VF_CTL_SNAPSHOT_ENABLE);
	OUTREG(BRW_VF_STRG_VAL, 0);
#endif

#if 0
	OUTREG(BRW_VS_CTL,
	       BRW_VS_CTL_SNAPSHOT_ALL_THREADS |
	       BRW_VS_CTL_SNAPSHOT_MUX_VALID_COUNT |
	       BRW_VS_CTL_THREAD_SNAPSHOT_ENABLE);

	OUTREG(BRW_VS_STRG_VAL, 0);
#endif

#if WATCH_SF
	OUTREG(BRW_SF_CTL,
	       BRW_SF_CTL_SNAPSHOT_MUX_VERTEX_COUNT |
	       BRW_SF_CTL_SNAPSHOT_ALL_THREADS |
	       BRW_SF_CTL_THREAD_SNAPSHOT_ENABLE);
	OUTREG(BRW_SF_STRG_VAL, 0);
#endif

#if WATCH_WIZ
	OUTREG(BRW_WIZ_CTL,
	       BRW_WIZ_CTL_SNAPSHOT_MUX_SUBSPAN_INSTANCE |
	       BRW_WIZ_CTL_SNAPSHOT_ALL_THREADS | BRW_WIZ_CTL_SNAPSHOT_ENABLE);
	OUTREG(BRW_WIZ_STRG_VAL, (box_x1) | (box_y1 << 16));
#endif

#if 0
	OUTREG(BRW_TS_CTL,
	       BRW_TS_CTL_SNAPSHOT_MESSAGE_ERROR |
	       BRW_TS_CTL_SNAPSHOT_ALL_CHILD_THREADS |
	       BRW_TS_CTL_SNAPSHOT_ALL_ROOT_THREADS |
	       BRW_TS_CTL_SNAPSHOT_ENABLE);
#endif
}

static void i965_post_draw_debug(ScrnInfoPtr scrn)
{
#if 0
	intel_screen_private *intel = intel_get_screen_private(scrn);
#endif

#if 0
	for (j = 0; j < 100000; j++) {
		ctl = INREG(BRW_VF_CTL);
		if (ctl & BRW_VF_CTL_SNAPSHOT_COMPLETE)
			break;
	}

	rdata = INREG(BRW_VF_RDATA);
	OUTREG(BRW_VF_CTL, 0);
	ErrorF("VF_CTL: 0x%08x VF_RDATA: 0x%08x\n", ctl, rdata);
#endif

#if 0
	for (j = 0; j < 1000000; j++) {
		ctl = INREG(BRW_VS_CTL);
		if (ctl & BRW_VS_CTL_SNAPSHOT_COMPLETE)
			break;
	}

	rdata = INREG(BRW_VS_RDATA);
	for (k = 0; k <= 3; k++) {
		OUTREG(BRW_VS_CTL, BRW_VS_CTL_SNAPSHOT_COMPLETE | (k << 8));
		rdata = INREG(BRW_VS_RDATA);
		ErrorF("VS_CTL: 0x%08x VS_RDATA(%d): 0x%08x\n", ctl, k, rdata);
	}

	OUTREG(BRW_VS_CTL, 0);
#endif

#if WATCH_SF
	for (j = 0; j < 1000000; j++) {
		ctl = INREG(BRW_SF_CTL);
		if (ctl & BRW_SF_CTL_SNAPSHOT_COMPLETE)
			break;
	}

	for (k = 0; k <= 7; k++) {
		OUTREG(BRW_SF_CTL, BRW_SF_CTL_SNAPSHOT_COMPLETE | (k << 8));
		rdata = INREG(BRW_SF_RDATA);
		ErrorF("SF_CTL: 0x%08x SF_RDATA(%d): 0x%08x\n", ctl, k, rdata);
	}

	OUTREG(BRW_SF_CTL, 0);
#endif

#if WATCH_WIZ
	for (j = 0; j < 100000; j++) {
		ctl = INREG(BRW_WIZ_CTL);
		if (ctl & BRW_WIZ_CTL_SNAPSHOT_COMPLETE)
			break;
	}

	rdata = INREG(BRW_WIZ_RDATA);
	OUTREG(BRW_WIZ_CTL, 0);
	ErrorF("WIZ_CTL: 0x%08x WIZ_RDATA: 0x%08x\n", ctl, rdata);
#endif

#if 0
	for (j = 0; j < 100000; j++) {
		ctl = INREG(BRW_TS_CTL);
		if (ctl & BRW_TS_CTL_SNAPSHOT_COMPLETE)
			break;
	}

	rdata = INREG(BRW_TS_RDATA);
	OUTREG(BRW_TS_CTL, 0);
	ErrorF("TS_CTL: 0x%08x TS_RDATA: 0x%08x\n", ctl, rdata);

	ErrorF("after EU_ATT 0x%08x%08x EU_ATT_DATA 0x%08x%08x\n",
	       INREG(BRW_EU_ATT_1), INREG(BRW_EU_ATT_0),
	       INREG(BRW_EU_ATT_DATA_1), INREG(BRW_EU_ATT_DATA_0));
#endif

#if 0
	for (j = 0; j < 256; j++) {
		OUTREG(BRW_TD_CTL, j << BRW_TD_CTL_MUX_SHIFT);
		rdata = INREG(BRW_TD_RDATA);
		ErrorF("TD_RDATA(%d): 0x%08x\n", j, rdata);
	}
#endif
}

/* For 3D, the VS must have 8, 12, 16, 24, or 32 VUEs allocated to it.
 * A VUE consists of a 256-bit vertex header followed by the vertex data,
 * which in our case is 4 floats (128 bits), thus a single 512-bit URB
 * entry.
 */
#define URB_VS_ENTRIES	      8
#define URB_VS_ENTRY_SIZE     1

#define URB_GS_ENTRIES	      0
#define URB_GS_ENTRY_SIZE     0

#define URB_CLIP_ENTRIES      0
#define URB_CLIP_ENTRY_SIZE   0

/* The SF kernel we use outputs only 4 256-bit registers, leading to an
 * entry size of 2 512-bit URBs.  We don't need to have many entries to
 * output as we're generally working on large rectangles and don't care
 * about having WM threads running on different rectangles simultaneously.
 */
#define URB_SF_ENTRIES	      1
#define URB_SF_ENTRY_SIZE     2

#define URB_CS_ENTRIES	      0
#define URB_CS_ENTRY_SIZE     0

static void i965_create_dst_surface_state(ScrnInfoPtr scrn,
					PixmapPtr pixmap,
					drm_intel_bo *surf_bo,
					uint32_t offset)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct brw_surface_state dest_surf_state;
	drm_intel_bo *pixmap_bo = intel_get_pixmap_bo(pixmap);
	assert(pixmap_bo != NULL);

	memset(&dest_surf_state, 0, sizeof(dest_surf_state));

	dest_surf_state.ss0.surface_type = BRW_SURFACE_2D;
	dest_surf_state.ss0.data_return_format =
	    BRW_SURFACERETURNFORMAT_FLOAT32;
	if (intel->cpp == 2) {
		dest_surf_state.ss0.surface_format =
		    BRW_SURFACEFORMAT_B5G6R5_UNORM;
	} else {
		dest_surf_state.ss0.surface_format =
		    BRW_SURFACEFORMAT_B8G8R8A8_UNORM;
	}
	dest_surf_state.ss0.writedisable_alpha = 0;
	dest_surf_state.ss0.writedisable_red = 0;
	dest_surf_state.ss0.writedisable_green = 0;
	dest_surf_state.ss0.writedisable_blue = 0;
	dest_surf_state.ss0.color_blend = 1;
	dest_surf_state.ss0.vert_line_stride = 0;
	dest_surf_state.ss0.vert_line_stride_ofs = 0;
	dest_surf_state.ss0.mipmap_layout_mode = 0;
	dest_surf_state.ss0.render_cache_read_mode = 0;

	dest_surf_state.ss1.base_addr =
	    intel_emit_reloc(surf_bo, offset + offsetof(struct brw_surface_state, ss1),
			     pixmap_bo, 0, I915_GEM_DOMAIN_RENDER, I915_GEM_DOMAIN_RENDER);

	dest_surf_state.ss2.height = pixmap->drawable.height - 1;
	dest_surf_state.ss2.width = pixmap->drawable.width - 1;
	dest_surf_state.ss2.mip_count = 0;
	dest_surf_state.ss2.render_target_rotation = 0;
	dest_surf_state.ss3.pitch = intel_pixmap_pitch(pixmap) - 1;
	dest_surf_state.ss3.tiled_surface = intel_pixmap_tiled(pixmap);
	dest_surf_state.ss3.tile_walk = 0;	/* TileX */

	dri_bo_subdata(surf_bo,
		       offset, sizeof(dest_surf_state),
		       &dest_surf_state);
}

static void i965_create_src_surface_state(ScrnInfoPtr scrn,
					drm_intel_bo * src_bo,
					uint32_t src_offset,
					int src_width,
					int src_height,
					int src_pitch,
					uint32_t src_surf_format,
					drm_intel_bo *surface_bo,
					uint32_t offset)
{
	struct brw_surface_state src_surf_state;

	memset(&src_surf_state, 0, sizeof(src_surf_state));

	/* Set up the source surface state buffer */
	src_surf_state.ss0.surface_type = BRW_SURFACE_2D;
	src_surf_state.ss0.surface_format = src_surf_format;
	src_surf_state.ss0.writedisable_alpha = 0;
	src_surf_state.ss0.writedisable_red = 0;
	src_surf_state.ss0.writedisable_green = 0;
	src_surf_state.ss0.writedisable_blue = 0;
	src_surf_state.ss0.color_blend = 1;
	src_surf_state.ss0.vert_line_stride = 0;
	src_surf_state.ss0.vert_line_stride_ofs = 0;
	src_surf_state.ss0.mipmap_layout_mode = 0;
	src_surf_state.ss0.render_cache_read_mode = 0;

	src_surf_state.ss2.width = src_width - 1;
	src_surf_state.ss2.height = src_height - 1;
	src_surf_state.ss2.mip_count = 0;
	src_surf_state.ss2.render_target_rotation = 0;
	src_surf_state.ss3.pitch = src_pitch - 1;

	if (src_bo) {
		src_surf_state.ss1.base_addr =
		    intel_emit_reloc(surface_bo,
				     offset + offsetof(struct brw_surface_state, ss1),
				     src_bo, src_offset,
				     I915_GEM_DOMAIN_SAMPLER, 0);
	} else {
		src_surf_state.ss1.base_addr = src_offset;
	}

	dri_bo_subdata(surface_bo,
		       offset, sizeof(src_surf_state),
		       &src_surf_state);
}

static void gen7_create_dst_surface_state(ScrnInfoPtr scrn,
					PixmapPtr pixmap,
					drm_intel_bo *surf_bo,
					uint32_t offset)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen7_surface_state dest_surf_state;
	drm_intel_bo *pixmap_bo = intel_get_pixmap_bo(pixmap);
	assert(pixmap_bo != NULL);

	memset(&dest_surf_state, 0, sizeof(dest_surf_state));

	dest_surf_state.ss0.surface_type = BRW_SURFACE_2D;
	dest_surf_state.ss0.tiled_surface = intel_pixmap_tiled(pixmap);
	dest_surf_state.ss0.tile_walk = 0;	/* TileX */

	if (intel->cpp == 2) {
		dest_surf_state.ss0.surface_format = BRW_SURFACEFORMAT_B5G6R5_UNORM;
	} else {
		dest_surf_state.ss0.surface_format = BRW_SURFACEFORMAT_B8G8R8A8_UNORM;
	}

	dest_surf_state.ss1.base_addr =
		intel_emit_reloc(surf_bo,
				offset + offsetof(struct gen7_surface_state, ss1),
				pixmap_bo, 0,
				I915_GEM_DOMAIN_SAMPLER, 0);

	dest_surf_state.ss2.height = pixmap->drawable.height - 1;
	dest_surf_state.ss2.width = pixmap->drawable.width - 1;

	dest_surf_state.ss3.pitch = intel_pixmap_pitch(pixmap) - 1;

	if (IS_HSW(intel)) {
		dest_surf_state.ss7.shader_chanel_select_r = HSW_SCS_RED;
		dest_surf_state.ss7.shader_chanel_select_g = HSW_SCS_GREEN;
		dest_surf_state.ss7.shader_chanel_select_b = HSW_SCS_BLUE;
		dest_surf_state.ss7.shader_chanel_select_a = HSW_SCS_ALPHA;
	}

	dri_bo_subdata(surf_bo,
		       offset, sizeof(dest_surf_state),
		       &dest_surf_state);
}

static void gen7_create_src_surface_state(ScrnInfoPtr scrn,
					drm_intel_bo * src_bo,
					uint32_t src_offset,
					int src_width,
					int src_height,
					int src_pitch,
					uint32_t src_surf_format,
					drm_intel_bo *surface_bo,
					uint32_t offset)
{
	intel_screen_private * const intel = intel_get_screen_private(scrn);
	struct gen7_surface_state src_surf_state;

	memset(&src_surf_state, 0, sizeof(src_surf_state));

	src_surf_state.ss0.surface_type = BRW_SURFACE_2D;
	src_surf_state.ss0.surface_format = src_surf_format;

	if (src_bo) {
		src_surf_state.ss1.base_addr =
			intel_emit_reloc(surface_bo,
					offset + offsetof(struct gen7_surface_state, ss1),
					src_bo, src_offset,
					I915_GEM_DOMAIN_SAMPLER, 0);
	} else {
		src_surf_state.ss1.base_addr = src_offset;
	}

	src_surf_state.ss2.width = src_width - 1;
	src_surf_state.ss2.height = src_height - 1;

	src_surf_state.ss3.pitch = src_pitch - 1;

	if (IS_HSW(intel)) {
		src_surf_state.ss7.shader_chanel_select_r = HSW_SCS_RED;
		src_surf_state.ss7.shader_chanel_select_g = HSW_SCS_GREEN;
		src_surf_state.ss7.shader_chanel_select_b = HSW_SCS_BLUE;
		src_surf_state.ss7.shader_chanel_select_a = HSW_SCS_ALPHA;
	}

	dri_bo_subdata(surface_bo,
		       offset, sizeof(src_surf_state),
		       &src_surf_state);
}

static void i965_create_binding_table(ScrnInfoPtr scrn,
				      drm_intel_bo *bind_bo,
				      int n_surf)
{
	uint32_t binding_table[n_surf];
	int i;

	/* Set up a binding table for our surfaces.  Only the PS will use it */
	for (i = 0; i < n_surf; i++)
		binding_table[i] = i * SURFACE_STATE_PADDED_SIZE;

	dri_bo_subdata(bind_bo,
		       n_surf * SURFACE_STATE_PADDED_SIZE,
		       sizeof(binding_table), binding_table);
}

static drm_intel_bo *i965_create_sampler_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct brw_sampler_state sampler_state;

	memset(&sampler_state, 0, sizeof(sampler_state));
	sampler_state.ss0.min_filter = BRW_MAPFILTER_LINEAR;
	sampler_state.ss0.mag_filter = BRW_MAPFILTER_LINEAR;
	sampler_state.ss1.r_wrap_mode = BRW_TEXCOORDMODE_CLAMP;
	sampler_state.ss1.s_wrap_mode = BRW_TEXCOORDMODE_CLAMP;
	sampler_state.ss1.t_wrap_mode = BRW_TEXCOORDMODE_CLAMP;

	return intel_bo_alloc_for_data(intel,
				       &sampler_state, sizeof(sampler_state),
				       "textured video sampler state");
}

static drm_intel_bo *gen7_create_sampler_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen7_sampler_state sampler_state;

	memset(&sampler_state, 0, sizeof(sampler_state));
	sampler_state.ss0.min_filter = BRW_MAPFILTER_LINEAR;
	sampler_state.ss0.mag_filter = BRW_MAPFILTER_LINEAR;
	sampler_state.ss3.r_wrap_mode = BRW_TEXCOORDMODE_CLAMP;
	sampler_state.ss3.s_wrap_mode = BRW_TEXCOORDMODE_CLAMP;
	sampler_state.ss3.t_wrap_mode = BRW_TEXCOORDMODE_CLAMP;

	return intel_bo_alloc_for_data(intel,
				       &sampler_state, sizeof(sampler_state),
				       "textured video sampler state");
}

static drm_intel_bo *i965_create_vs_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct brw_vs_unit_state vs_state;

	/* Set up the vertex shader to be disabled (passthrough) */
	memset(&vs_state, 0, sizeof(vs_state));
	if (IS_GEN5(intel))
		vs_state.thread4.nr_urb_entries = URB_VS_ENTRIES >> 2;
	else
		vs_state.thread4.nr_urb_entries = URB_VS_ENTRIES;
	vs_state.thread4.urb_entry_allocation_size = URB_VS_ENTRY_SIZE - 1;
	vs_state.vs6.vs_enable = 0;
	vs_state.vs6.vert_cache_disable = 1;

	return intel_bo_alloc_for_data(intel,
				       &vs_state, sizeof(vs_state),
				       "textured video vs state");
}

static drm_intel_bo *i965_create_program(ScrnInfoPtr scrn,
					 const uint32_t * program,
					 unsigned int program_size)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	return intel_bo_alloc_for_data(intel,
				       program, program_size,
				       "textured video program");
}

static drm_intel_bo *i965_create_sf_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *sf_bo, *kernel_bo;
	struct brw_sf_unit_state sf_state;

	if (IS_GEN5(intel))
		kernel_bo = i965_create_program(scrn,
						&sf_kernel_static_gen5[0][0],
						sizeof(sf_kernel_static_gen5));
	else
		kernel_bo = i965_create_program(scrn,
						&sf_kernel_static[0][0],
						sizeof(sf_kernel_static));
	if (!kernel_bo)
		return NULL;

	sf_bo = drm_intel_bo_alloc(intel->bufmgr,
				   "textured video sf state", 4096,
				   sizeof(sf_state));
	if (sf_bo == NULL) {
		drm_intel_bo_unreference(kernel_bo);
		return NULL;
	}

	/* Set up the SF kernel to do coord interp: for each attribute,
	 * calculate dA/dx and dA/dy.  Hand these interpolation coefficients
	 * back to SF which then hands pixels off to WM.
	 */
	memset(&sf_state, 0, sizeof(sf_state));
	sf_state.thread0.grf_reg_count = BRW_GRF_BLOCKS(SF_KERNEL_NUM_GRF);
	sf_state.thread0.kernel_start_pointer =
	    intel_emit_reloc(sf_bo, offsetof(struct brw_sf_unit_state, thread0),
			     kernel_bo, sf_state.thread0.grf_reg_count << 1,
			     I915_GEM_DOMAIN_INSTRUCTION, 0) >> 6;
	sf_state.sf1.single_program_flow = 1;	/* XXX */
	sf_state.sf1.binding_table_entry_count = 0;
	sf_state.sf1.thread_priority = 0;
	sf_state.sf1.floating_point_mode = 0;	/* Mesa does this */
	sf_state.sf1.illegal_op_exception_enable = 1;
	sf_state.sf1.mask_stack_exception_enable = 1;
	sf_state.sf1.sw_exception_enable = 1;
	sf_state.thread2.per_thread_scratch_space = 0;
	/* scratch space is not used in our kernel */
	sf_state.thread2.scratch_space_base_pointer = 0;
	sf_state.thread3.const_urb_entry_read_length = 0;	/* no const URBs */
	sf_state.thread3.const_urb_entry_read_offset = 0;	/* no const URBs */
	sf_state.thread3.urb_entry_read_length = 1;	/* 1 URB per vertex */
	sf_state.thread3.urb_entry_read_offset = 0;
	sf_state.thread3.dispatch_grf_start_reg = 3;
	sf_state.thread4.max_threads = SF_MAX_THREADS - 1;
	sf_state.thread4.urb_entry_allocation_size = URB_SF_ENTRY_SIZE - 1;
	sf_state.thread4.nr_urb_entries = URB_SF_ENTRIES;
	sf_state.thread4.stats_enable = 1;
	sf_state.sf5.viewport_transform = FALSE;	/* skip viewport */
	sf_state.sf6.cull_mode = BRW_CULLMODE_NONE;
	sf_state.sf6.scissor = 0;
	sf_state.sf7.trifan_pv = 2;
	sf_state.sf6.dest_org_vbias = 0x8;
	sf_state.sf6.dest_org_hbias = 0x8;

	dri_bo_subdata(sf_bo, 0, sizeof(sf_state), &sf_state);
	return sf_bo;
}

static drm_intel_bo *i965_create_wm_state(ScrnInfoPtr scrn,
					  drm_intel_bo * sampler_bo,
					  Bool is_packed)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *wm_bo, *kernel_bo;
	struct brw_wm_unit_state wm_state;

	if (is_packed) {
		if (IS_GEN5(intel))
			kernel_bo =
			    i965_create_program(scrn,
						&ps_kernel_packed_static_gen5[0]
						[0],
						sizeof
						(ps_kernel_packed_static_gen5));
		else
			kernel_bo =
			    i965_create_program(scrn,
						&ps_kernel_packed_static[0][0],
						sizeof
						(ps_kernel_packed_static));
	} else {
		if (IS_GEN5(intel))
			kernel_bo =
			    i965_create_program(scrn,
						&ps_kernel_planar_static_gen5[0]
						[0],
						sizeof
						(ps_kernel_planar_static_gen5));
		else
			kernel_bo =
			    i965_create_program(scrn,
						&ps_kernel_planar_static[0][0],
						sizeof
						(ps_kernel_planar_static));
	}
	if (!kernel_bo)
		return NULL;

	wm_bo = drm_intel_bo_alloc(intel->bufmgr,
				   "textured video wm state",
				   sizeof(wm_state), 0);
	if (wm_bo == NULL) {
		drm_intel_bo_unreference(kernel_bo);
		return NULL;
	}

	memset(&wm_state, 0, sizeof(wm_state));
	wm_state.thread0.grf_reg_count = BRW_GRF_BLOCKS(PS_KERNEL_NUM_GRF);
	wm_state.thread0.kernel_start_pointer =
	    intel_emit_reloc(wm_bo, offsetof(struct brw_wm_unit_state, thread0),
			     kernel_bo, wm_state.thread0.grf_reg_count << 1,
			     I915_GEM_DOMAIN_INSTRUCTION, 0) >> 6;
	wm_state.thread1.single_program_flow = 1;	/* XXX */
	if (is_packed)
		wm_state.thread1.binding_table_entry_count = 2;
	else
		wm_state.thread1.binding_table_entry_count = 7;

	/* binding table entry count is only used for prefetching, and it has to
	 * be set 0 for Ironlake
	 */
	if (IS_GEN5(intel))
		wm_state.thread1.binding_table_entry_count = 0;

	/* Though we never use the scratch space in our WM kernel, it has to be
	 * set, and the minimum allocation is 1024 bytes.
	 */
	wm_state.thread2.scratch_space_base_pointer = 0;
	wm_state.thread2.per_thread_scratch_space = 0;	/* 1024 bytes */
	wm_state.thread3.dispatch_grf_start_reg = 3;	/* XXX */
	wm_state.thread3.const_urb_entry_read_length = 0;
	wm_state.thread3.const_urb_entry_read_offset = 0;
	wm_state.thread3.urb_entry_read_length = 1;	/* XXX */
	wm_state.thread3.urb_entry_read_offset = 0;	/* XXX */
	wm_state.wm4.stats_enable = 1;
	wm_state.wm4.sampler_state_pointer =
	    intel_emit_reloc(wm_bo, offsetof(struct brw_wm_unit_state, wm4),
			     sampler_bo, 0,
			     I915_GEM_DOMAIN_INSTRUCTION, 0) >> 5;
	if (IS_GEN5(intel))
		wm_state.wm4.sampler_count = 0;
	else
		wm_state.wm4.sampler_count = 1;	/* 1-4 samplers used */
	wm_state.wm5.max_threads = PS_MAX_THREADS - 1;
	wm_state.wm5.thread_dispatch_enable = 1;
	wm_state.wm5.enable_16_pix = 1;
	wm_state.wm5.enable_8_pix = 0;
	wm_state.wm5.early_depth_test = 1;

	dri_bo_subdata(wm_bo, 0, sizeof(wm_state), &wm_state);
	drm_intel_bo_unreference(kernel_bo);
	return wm_bo;
}

static drm_intel_bo *i965_create_cc_vp_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct brw_cc_viewport cc_viewport;

	memset(&cc_viewport, 0, sizeof(cc_viewport));
	cc_viewport.min_depth = -1.e35;
	cc_viewport.max_depth = 1.e35;

	return intel_bo_alloc_for_data(intel,
				       &cc_viewport, sizeof(cc_viewport),
				       "textured video cc viewport");
}

static drm_intel_bo *i965_create_cc_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *cc_bo, *cc_vp_bo;
	struct brw_cc_unit_state cc_state;

	cc_vp_bo = i965_create_cc_vp_state(scrn);
	if (!cc_vp_bo)
		return NULL;

	cc_bo = drm_intel_bo_alloc(intel->bufmgr,
				   "textured video cc state",
				   sizeof(cc_state), 0);
	if (cc_bo == NULL){
		drm_intel_bo_unreference(cc_vp_bo);
		return NULL;
	}

	/* Color calculator state */
	memset(&cc_state, 0, sizeof(cc_state));
	cc_state.cc0.stencil_enable = 0;	/* disable stencil */
	cc_state.cc2.depth_test = 0;	/* disable depth test */
	cc_state.cc2.logicop_enable = 1;	/* enable logic op */
	cc_state.cc3.ia_blend_enable = 1;	/* blend alpha just like colors */
	cc_state.cc3.blend_enable = 0;	/* disable color blend */
	cc_state.cc3.alpha_test = 0;	/* disable alpha test */
	cc_state.cc4.cc_viewport_state_offset =
	    intel_emit_reloc(cc_bo, offsetof(struct brw_cc_unit_state, cc4),
			     cc_vp_bo, 0, I915_GEM_DOMAIN_INSTRUCTION, 0) >> 5;
	cc_state.cc5.dither_enable = 0;	/* disable dither */
	cc_state.cc5.logicop_func = 0xc;	/* WHITE */
	cc_state.cc5.statistics_enable = 1;
	cc_state.cc5.ia_blend_function = BRW_BLENDFUNCTION_ADD;
	cc_state.cc5.ia_src_blend_factor = BRW_BLENDFACTOR_ONE;
	cc_state.cc5.ia_dest_blend_factor = BRW_BLENDFACTOR_ONE;

	dri_bo_subdata(cc_bo, 0, sizeof(cc_state), &cc_state);
	drm_intel_bo_unreference(cc_vp_bo);

	return cc_bo;
}

static void
i965_emit_video_setup(ScrnInfoPtr scrn, drm_intel_bo * surface_state_binding_table_bo, int n_src_surf, PixmapPtr pixmap)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int urb_vs_start, urb_vs_size;
	int urb_gs_start, urb_gs_size;
	int urb_clip_start, urb_clip_size;
	int urb_sf_start, urb_sf_size;
	int urb_cs_start, urb_cs_size;
	int pipe_ctl;

	IntelEmitInvarientState(scrn);
	intel->last_3d = LAST_3D_VIDEO;
	intel->needs_3d_invariant = TRUE;

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

	OUT_BATCH(MI_FLUSH |
		  MI_STATE_INSTRUCTION_CACHE_FLUSH |
		  BRW_MI_GLOBAL_SNAPSHOT_RESET);
	OUT_BATCH(MI_NOOP);

	/* brw_debug (scrn, "before base address modify"); */
	/* Match Mesa driver setup */
	if (INTEL_INFO(intel)->gen >= 045)
		OUT_BATCH(NEW_PIPELINE_SELECT | PIPELINE_SELECT_3D);
	else
		OUT_BATCH(BRW_PIPELINE_SELECT | PIPELINE_SELECT_3D);

	/* Mesa does this. Who knows... */
	OUT_BATCH(BRW_CS_URB_STATE | 0);
	OUT_BATCH((0 << 4) |	/* URB Entry Allocation Size */
		  (0 << 0));	/* Number of URB Entries */

	/* Zero out the two base address registers so all offsets are
	 * absolute
	 */
	if (IS_GEN5(intel)) {
		OUT_BATCH(BRW_STATE_BASE_ADDRESS | 6);
		OUT_BATCH(0 | BASE_ADDRESS_MODIFY);	/* Generate state base address */
		OUT_RELOC(surface_state_binding_table_bo, I915_GEM_DOMAIN_INSTRUCTION, 0, BASE_ADDRESS_MODIFY); /* Surface state base address */
		OUT_BATCH(0 | BASE_ADDRESS_MODIFY);	/* media base addr, don't care */
		OUT_BATCH(0 | BASE_ADDRESS_MODIFY);	/* Instruction base address */
		/* general state max addr, disabled */
		OUT_BATCH(0 | BASE_ADDRESS_MODIFY);
		/* media object state max addr, disabled */
		OUT_BATCH(0 | BASE_ADDRESS_MODIFY);
		/* Instruction max addr, disabled */
		OUT_BATCH(0 | BASE_ADDRESS_MODIFY);
	} else {
		OUT_BATCH(BRW_STATE_BASE_ADDRESS | 4);
		OUT_BATCH(0 | BASE_ADDRESS_MODIFY);	/* Generate state base address */
		OUT_RELOC(surface_state_binding_table_bo, I915_GEM_DOMAIN_INSTRUCTION, 0, BASE_ADDRESS_MODIFY); /* Surface state base address */
		OUT_BATCH(0 | BASE_ADDRESS_MODIFY);	/* media base addr, don't care */
		/* general state max addr, disabled */
		OUT_BATCH(0 | BASE_ADDRESS_MODIFY);
		/* media object state max addr, disabled */
		OUT_BATCH(0 | BASE_ADDRESS_MODIFY);
	}

	/* Set system instruction pointer */
	OUT_BATCH(BRW_STATE_SIP | 0);
	/* system instruction pointer */
	OUT_RELOC(intel->video.gen4_sip_kernel_bo,
		  I915_GEM_DOMAIN_INSTRUCTION, 0, 0);

	/* brw_debug (scrn, "after base address modify"); */

	if (IS_GEN5(intel))
		pipe_ctl = BRW_PIPE_CONTROL_NOWRITE;
	else
		pipe_ctl = BRW_PIPE_CONTROL_NOWRITE | BRW_PIPE_CONTROL_IS_FLUSH;

	/* Pipe control */
	OUT_BATCH(BRW_PIPE_CONTROL | pipe_ctl | 2);
	OUT_BATCH(0);		/* Destination address */
	OUT_BATCH(0);		/* Immediate data low DW */
	OUT_BATCH(0);		/* Immediate data high DW */

	/* Binding table pointers */
	OUT_BATCH(BRW_3DSTATE_BINDING_TABLE_POINTERS | 4);
	OUT_BATCH(0);		/* vs */
	OUT_BATCH(0);		/* gs */
	OUT_BATCH(0);		/* clip */
	OUT_BATCH(0);		/* sf */
	/* Only the PS uses the binding table */
	OUT_BATCH((n_src_surf + 1) * SURFACE_STATE_PADDED_SIZE);

	/* Blend constant color (magenta is fun) */
	OUT_BATCH(BRW_3DSTATE_CONSTANT_COLOR | 3);
	OUT_BATCH(float_to_uint(1.0));
	OUT_BATCH(float_to_uint(0.0));
	OUT_BATCH(float_to_uint(1.0));
	OUT_BATCH(float_to_uint(1.0));

	/* The drawing rectangle clipping is always on.  Set it to values that
	 * shouldn't do any clipping.
	 */
	OUT_BATCH(BRW_3DSTATE_DRAWING_RECTANGLE | 2);	/* XXX 3 for BLC or CTG */
	OUT_BATCH(0x00000000);	/* ymin, xmin */
	OUT_BATCH((pixmap->drawable.width - 1) | (pixmap->drawable.height - 1) << 16);	/* ymax, xmax */
	OUT_BATCH(0x00000000);	/* yorigin, xorigin */

	/* skip the depth buffer */
	/* skip the polygon stipple */
	/* skip the polygon stipple offset */
	/* skip the line stipple */

	/* Set the pointers to the 3d pipeline state */
	OUT_BATCH(BRW_3DSTATE_PIPELINED_POINTERS | 5);
	OUT_RELOC(intel->video.gen4_vs_bo, I915_GEM_DOMAIN_INSTRUCTION, 0, 0);
	/* disable GS, resulting in passthrough */
	OUT_BATCH(BRW_GS_DISABLE);
	/* disable CLIP, resulting in passthrough */
	OUT_BATCH(BRW_CLIP_DISABLE);
	OUT_RELOC(intel->video.gen4_sf_bo, I915_GEM_DOMAIN_INSTRUCTION, 0, 0);
	if (n_src_surf == 1)
		OUT_RELOC(intel->video.gen4_wm_packed_bo,
			  I915_GEM_DOMAIN_INSTRUCTION, 0, 0);
	else
		OUT_RELOC(intel->video.gen4_wm_planar_bo,
			  I915_GEM_DOMAIN_INSTRUCTION, 0, 0);
	OUT_RELOC(intel->video.gen4_cc_bo, I915_GEM_DOMAIN_INSTRUCTION, 0, 0);

	/* URB fence */
	OUT_BATCH(BRW_URB_FENCE |
		  UF0_CS_REALLOC |
		  UF0_SF_REALLOC |
		  UF0_CLIP_REALLOC | UF0_GS_REALLOC | UF0_VS_REALLOC | 1);
	OUT_BATCH(((urb_clip_start + urb_clip_size) << UF1_CLIP_FENCE_SHIFT) |
		  ((urb_gs_start + urb_gs_size) << UF1_GS_FENCE_SHIFT) |
		  ((urb_vs_start + urb_vs_size) << UF1_VS_FENCE_SHIFT));
	OUT_BATCH(((urb_cs_start + urb_cs_size) << UF2_CS_FENCE_SHIFT) |
		  ((urb_sf_start + urb_sf_size) << UF2_SF_FENCE_SHIFT));

	/* Constant buffer state */
	OUT_BATCH(BRW_CS_URB_STATE | 0);
	OUT_BATCH(((URB_CS_ENTRY_SIZE - 1) << 4) | (URB_CS_ENTRIES << 0));

	/* Set up our vertex elements, sourced from the single vertex buffer. */

	if (IS_GEN5(intel)) {
		OUT_BATCH(BRW_3DSTATE_VERTEX_ELEMENTS | 3);
		/* offset 0: X,Y -> {X, Y, 1.0, 1.0} */
		OUT_BATCH((0 << VE0_VERTEX_BUFFER_INDEX_SHIFT) |
			  VE0_VALID |
			  (BRW_SURFACEFORMAT_R32G32_FLOAT << VE0_FORMAT_SHIFT) |
			  (0 << VE0_OFFSET_SHIFT));
		OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT)
			  | (BRW_VFCOMPONENT_STORE_SRC <<
			     VE1_VFCOMPONENT_1_SHIFT) |
			  (BRW_VFCOMPONENT_STORE_1_FLT <<
			   VE1_VFCOMPONENT_2_SHIFT) |
			  (BRW_VFCOMPONENT_STORE_1_FLT <<
			   VE1_VFCOMPONENT_3_SHIFT));
		/* offset 8: S0, T0 -> {S0, T0, 1.0, 1.0} */
		OUT_BATCH((0 << VE0_VERTEX_BUFFER_INDEX_SHIFT) |
			  VE0_VALID |
			  (BRW_SURFACEFORMAT_R32G32_FLOAT << VE0_FORMAT_SHIFT) |
			  (8 << VE0_OFFSET_SHIFT));
		OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT)
			  | (BRW_VFCOMPONENT_STORE_SRC <<
			     VE1_VFCOMPONENT_1_SHIFT) |
			  (BRW_VFCOMPONENT_STORE_1_FLT <<
			   VE1_VFCOMPONENT_2_SHIFT) |
			  (BRW_VFCOMPONENT_STORE_1_FLT <<
			   VE1_VFCOMPONENT_3_SHIFT));
	} else {
		OUT_BATCH(BRW_3DSTATE_VERTEX_ELEMENTS | 3);
		/* offset 0: X,Y -> {X, Y, 1.0, 1.0} */
		OUT_BATCH((0 << VE0_VERTEX_BUFFER_INDEX_SHIFT) |
			  VE0_VALID |
			  (BRW_SURFACEFORMAT_R32G32_FLOAT << VE0_FORMAT_SHIFT) |
			  (0 << VE0_OFFSET_SHIFT));
		OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT)
			  | (BRW_VFCOMPONENT_STORE_SRC <<
			     VE1_VFCOMPONENT_1_SHIFT) |
			  (BRW_VFCOMPONENT_STORE_1_FLT <<
			   VE1_VFCOMPONENT_2_SHIFT) |
			  (BRW_VFCOMPONENT_STORE_1_FLT <<
			   VE1_VFCOMPONENT_3_SHIFT) | (0 <<
						       VE1_DESTINATION_ELEMENT_OFFSET_SHIFT));
		/* offset 8: S0, T0 -> {S0, T0, 1.0, 1.0} */
		OUT_BATCH((0 << VE0_VERTEX_BUFFER_INDEX_SHIFT) |
			  VE0_VALID |
			  (BRW_SURFACEFORMAT_R32G32_FLOAT << VE0_FORMAT_SHIFT) |
			  (8 << VE0_OFFSET_SHIFT));
		OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT)
			  | (BRW_VFCOMPONENT_STORE_SRC <<
			     VE1_VFCOMPONENT_1_SHIFT) |
			  (BRW_VFCOMPONENT_STORE_1_FLT <<
			   VE1_VFCOMPONENT_2_SHIFT) |
			  (BRW_VFCOMPONENT_STORE_1_FLT <<
			   VE1_VFCOMPONENT_3_SHIFT) | (4 <<
						       VE1_DESTINATION_ELEMENT_OFFSET_SHIFT));
	}
}

void
I965DisplayVideoTextured(ScrnInfoPtr scrn,
			 intel_adaptor_private *adaptor_priv, int id,
			 RegionPtr dstRegion,
			 short width, short height,
			 int video_pitch, int video_pitch2,
			 short src_w, short src_h,
			 short drw_w, short drw_h, PixmapPtr pixmap)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	BoxPtr pbox;
	int nbox, dxo, dyo, pix_xoff, pix_yoff;
	float src_scale_x, src_scale_y;
	int src_surf;
	int n_src_surf;
	uint32_t src_surf_format;
	uint32_t src_surf_base[6];
	int src_width[6];
	int src_height[6];
	int src_pitch[6];
	drm_intel_bo *surface_state_binding_table_bo;

#if 0
	ErrorF("BroadwaterDisplayVideoTextured: %dx%d (pitch %d)\n", width,
	       height, video_pitch);
#endif

#if 0
	/* enable debug */
	OUTREG(INST_PM, (1 << (16 + 4)) | (1 << 4));
	ErrorF("INST_PM 0x%08x\n", INREG(INST_PM));
#endif

	src_surf_base[0] = adaptor_priv->YBufOffset;
	src_surf_base[1] = adaptor_priv->YBufOffset;
	src_surf_base[2] = adaptor_priv->VBufOffset;
	src_surf_base[3] = adaptor_priv->VBufOffset;
	src_surf_base[4] = adaptor_priv->UBufOffset;
	src_surf_base[5] = adaptor_priv->UBufOffset;
#if 0
	ErrorF("base 0 0x%x base 1 0x%x base 2 0x%x\n",
	       src_surf_base[0], src_surf_base[1], src_surf_base[2]);
#endif

	if (is_planar_fourcc(id)) {
		src_surf_format = BRW_SURFACEFORMAT_R8_UNORM;
		src_width[1] = src_width[0] = width;
		src_height[1] = src_height[0] = height;
		src_pitch[1] = src_pitch[0] = video_pitch2;
		src_width[4] = src_width[5] = src_width[2] = src_width[3] =
		    width / 2;
		src_height[4] = src_height[5] = src_height[2] = src_height[3] =
		    height / 2;
		src_pitch[4] = src_pitch[5] = src_pitch[2] = src_pitch[3] =
		    video_pitch;
		n_src_surf = 6;
	} else {
		if (id == FOURCC_UYVY)
			src_surf_format = BRW_SURFACEFORMAT_YCRCB_SWAPY;
		else
			src_surf_format = BRW_SURFACEFORMAT_YCRCB_NORMAL;

		src_width[0] = width;
		src_height[0] = height;
		src_pitch[0] = video_pitch;
		n_src_surf = 1;
	}

#if 0
	ErrorF("dst surf:      0x%08x\n", state_base_offset + dest_surf_offset);
	ErrorF("src surf:      0x%08x\n", state_base_offset + src_surf_offset);
#endif

	/* We'll be poking the state buffers that could be in use by the 3d
	 * hardware here, but we should have synced the 3D engine already in
	 * I830PutImage.
	 */

	surface_state_binding_table_bo = 
		drm_intel_bo_alloc(intel->bufmgr,
				"surface state & binding table",
				(n_src_surf + 1) * (SURFACE_STATE_PADDED_SIZE + sizeof(uint32_t)),
				4096);

	if (!surface_state_binding_table_bo)
		return;
				
	i965_create_dst_surface_state(scrn, pixmap, surface_state_binding_table_bo, 0);

	for (src_surf = 0; src_surf < n_src_surf; src_surf++) {
		i965_create_src_surface_state(scrn,
					adaptor_priv->buf,
					src_surf_base[src_surf],
					src_width[src_surf],
					src_height[src_surf],
					src_pitch[src_surf],
					src_surf_format,
					surface_state_binding_table_bo,
					(src_surf + 1) * SURFACE_STATE_PADDED_SIZE);
	}

	i965_create_binding_table(scrn, surface_state_binding_table_bo, n_src_surf + 1);

	if (intel->video.gen4_sampler_bo == NULL)
		intel->video.gen4_sampler_bo = i965_create_sampler_state(scrn);
	if (intel->video.gen4_sip_kernel_bo == NULL) {
		intel->video.gen4_sip_kernel_bo =
		    i965_create_program(scrn, &sip_kernel_static[0][0],
					sizeof(sip_kernel_static));
		if (!intel->video.gen4_sip_kernel_bo) {
			drm_intel_bo_unreference(surface_state_binding_table_bo);
			return;
		}
	}

	if (intel->video.gen4_vs_bo == NULL) {
		intel->video.gen4_vs_bo = i965_create_vs_state(scrn);
		if (!intel->video.gen4_vs_bo) {
			drm_intel_bo_unreference(surface_state_binding_table_bo);
			return;
		}
	}
	if (intel->video.gen4_sf_bo == NULL) {
		intel->video.gen4_sf_bo = i965_create_sf_state(scrn);
		if (!intel->video.gen4_sf_bo) {
			drm_intel_bo_unreference(surface_state_binding_table_bo);
			return;
		}
	}
	if (intel->video.gen4_wm_packed_bo == NULL) {
		intel->video.gen4_wm_packed_bo =
		    i965_create_wm_state(scrn, intel->video.gen4_sampler_bo,
					 TRUE);
		if (!intel->video.gen4_wm_packed_bo) {
			drm_intel_bo_unreference(surface_state_binding_table_bo);
			return;
		}
	}

	if (intel->video.gen4_wm_planar_bo == NULL) {
		intel->video.gen4_wm_planar_bo =
		    i965_create_wm_state(scrn, intel->video.gen4_sampler_bo,
					 FALSE);
		if (!intel->video.gen4_wm_planar_bo) {
			drm_intel_bo_unreference(surface_state_binding_table_bo);
			return;
		}
	}

	if (intel->video.gen4_cc_bo == NULL) {
		intel->video.gen4_cc_bo = i965_create_cc_state(scrn);
		if (!intel->video.gen4_cc_bo) {
			drm_intel_bo_unreference(surface_state_binding_table_bo);
			return;
		}
	}

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

	dxo = dstRegion->extents.x1;
	dyo = dstRegion->extents.y1;

	/* Use normalized texture coordinates */
	src_scale_x = ((float)src_w / width) / (float)drw_w;
	src_scale_y = ((float)src_h / height) / (float)drw_h;

	pbox = REGION_RECTS(dstRegion);
	nbox = REGION_NUM_RECTS(dstRegion);
	while (nbox--) {
		int box_x1 = pbox->x1;
		int box_y1 = pbox->y1;
		int box_x2 = pbox->x2;
		int box_y2 = pbox->y2;
		int i;
		float vb[12];
		drm_intel_bo *bo_table[] = {
			NULL,	/* vb_bo */
			intel->batch_bo,
			surface_state_binding_table_bo,
			intel->video.gen4_sampler_bo,
			intel->video.gen4_sip_kernel_bo,
			intel->video.gen4_vs_bo,
			intel->video.gen4_sf_bo,
			intel->video.gen4_wm_packed_bo,
			intel->video.gen4_wm_planar_bo,
			intel->video.gen4_cc_bo,
		};

		pbox++;

		i = 0;
		vb[i++] = (box_x2 - dxo) * src_scale_x;
		vb[i++] = (box_y2 - dyo) * src_scale_y;
		vb[i++] = (float)box_x2 + pix_xoff;
		vb[i++] = (float)box_y2 + pix_yoff;

		vb[i++] = (box_x1 - dxo) * src_scale_x;
		vb[i++] = (box_y2 - dyo) * src_scale_y;
		vb[i++] = (float)box_x1 + pix_xoff;
		vb[i++] = (float)box_y2 + pix_yoff;

		vb[i++] = (box_x1 - dxo) * src_scale_x;
		vb[i++] = (box_y1 - dyo) * src_scale_y;
		vb[i++] = (float)box_x1 + pix_xoff;
		vb[i++] = (float)box_y1 + pix_yoff;

		bo_table[0] = intel_bo_alloc_for_data(intel,
						      vb, sizeof(vb),
						      "textured video vbo");

		if (IS_GEN4(intel))
			i965_pre_draw_debug(scrn);

		/* If this command won't fit in the current batch, flush.
		 * Assume that it does after being flushed.
		 */
		if (drm_intel_bufmgr_check_aperture_space(bo_table,
							  ARRAY_SIZE(bo_table))
		    < 0) {
			intel_batch_submit(scrn);
		}

		intel_batch_start_atomic(scrn, 150);

		i965_emit_video_setup(scrn, surface_state_binding_table_bo, n_src_surf, pixmap);

		/* Set up the pointer to our vertex buffer */
		OUT_BATCH(BRW_3DSTATE_VERTEX_BUFFERS | 3);
		/* four 32-bit floats per vertex */
		OUT_BATCH((0 << VB0_BUFFER_INDEX_SHIFT) |
			  VB0_VERTEXDATA | ((4 * 4) << VB0_BUFFER_PITCH_SHIFT));
		OUT_RELOC(bo_table[0], I915_GEM_DOMAIN_VERTEX, 0, 0);
		if (IS_GEN5(intel))
			OUT_RELOC(bo_table[0], I915_GEM_DOMAIN_VERTEX, 0,
				  i * 4);
		else
			OUT_BATCH(3);	/* four corners to our rectangle */
		OUT_BATCH(0);	/* reserved */

		OUT_BATCH(BRW_3DPRIMITIVE | BRW_3DPRIMITIVE_VERTEX_SEQUENTIAL | (_3DPRIM_RECTLIST << BRW_3DPRIMITIVE_TOPOLOGY_SHIFT) | (0 << 9) |	/* CTG - indirect vertex count */
			  4);
		OUT_BATCH(3);	/* vertex count per instance */
		OUT_BATCH(0);	/* start vertex offset */
		OUT_BATCH(1);	/* single instance */
		OUT_BATCH(0);	/* start instance location */
		OUT_BATCH(0);	/* index buffer offset, ignored */
		OUT_BATCH(MI_NOOP);

		intel_batch_end_atomic(scrn);

		drm_intel_bo_unreference(bo_table[0]);

		if (IS_GEN4(intel))
			i965_post_draw_debug(scrn);

	}

	/* release reference once we're finished */
	drm_intel_bo_unreference(surface_state_binding_table_bo);

	intel_debug_flush(scrn);
}

void i965_free_video(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	drm_intel_bo_unreference(intel->video.gen4_vs_bo);
	intel->video.gen4_vs_bo = NULL;
	drm_intel_bo_unreference(intel->video.gen4_sf_bo);
	intel->video.gen4_sf_bo = NULL;
	drm_intel_bo_unreference(intel->video.gen4_cc_bo);
	intel->video.gen4_cc_bo = NULL;
	drm_intel_bo_unreference(intel->video.gen4_wm_packed_bo);
	intel->video.gen4_wm_packed_bo = NULL;
	drm_intel_bo_unreference(intel->video.gen4_wm_planar_bo);
	intel->video.gen4_wm_planar_bo = NULL;
	drm_intel_bo_unreference(intel->video.gen4_cc_vp_bo);
	intel->video.gen4_cc_vp_bo = NULL;
	drm_intel_bo_unreference(intel->video.gen4_sampler_bo);
	intel->video.gen4_sampler_bo = NULL;
	drm_intel_bo_unreference(intel->video.gen4_sip_kernel_bo);
	intel->video.gen4_sip_kernel_bo = NULL;
	drm_intel_bo_unreference(intel->video.wm_prog_packed_bo);
	intel->video.wm_prog_packed_bo = NULL;
	drm_intel_bo_unreference(intel->video.wm_prog_planar_bo);
	intel->video.wm_prog_planar_bo = NULL;
	drm_intel_bo_unreference(intel->video.gen6_blend_bo);
	intel->video.gen6_blend_bo = NULL;
	drm_intel_bo_unreference(intel->video.gen6_depth_stencil_bo);
	intel->video.gen6_depth_stencil_bo = NULL;
}

/* for GEN6+ */
static drm_intel_bo *
gen6_create_cc_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen6_color_calc_state cc_state;

	memset(&cc_state, 0, sizeof(cc_state));
	cc_state.constant_r = 1.0;
	cc_state.constant_g = 0.0;
	cc_state.constant_b = 1.0;
	cc_state.constant_a = 1.0;

	return intel_bo_alloc_for_data(intel,
				       &cc_state, sizeof(cc_state),
				       "textured video cc state");
}

static drm_intel_bo *
gen6_create_blend_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen6_blend_state blend_state;

	memset(&blend_state, 0, sizeof(blend_state));
	blend_state.blend1.logic_op_enable = 1;
	blend_state.blend1.logic_op_func = 0xc;
	blend_state.blend1.pre_blend_clamp_enable = 1;

	return intel_bo_alloc_for_data(intel,
				       &blend_state, sizeof(blend_state),
				       "textured video blend state");
}

static drm_intel_bo *
gen6_create_depth_stencil_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct gen6_depth_stencil_state depth_stencil_state;

	memset(&depth_stencil_state, 0, sizeof(depth_stencil_state));
	return intel_bo_alloc_for_data(intel,
				       &depth_stencil_state,
				       sizeof(depth_stencil_state),
				       "textured video blend state");
}

static Bool
gen6_create_vidoe_objects(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *(*create_sampler_state)(ScrnInfoPtr);
	const uint32_t *packed_ps_kernel, *planar_ps_kernel;
	unsigned int packed_ps_size, planar_ps_size;
	
	if (INTEL_INFO(intel)->gen >= 070) {
		create_sampler_state = gen7_create_sampler_state;
		packed_ps_kernel = &ps_kernel_packed_static_gen7[0][0];
		packed_ps_size = sizeof(ps_kernel_packed_static_gen7);
		planar_ps_kernel = &ps_kernel_planar_static_gen7[0][0];
		planar_ps_size = sizeof(ps_kernel_planar_static_gen7);
	} else {
		create_sampler_state = i965_create_sampler_state;
		packed_ps_kernel = &ps_kernel_packed_static_gen6[0][0];
		packed_ps_size = sizeof(ps_kernel_packed_static_gen6);
		planar_ps_kernel = &ps_kernel_planar_static_gen6[0][0];
		planar_ps_size = sizeof(ps_kernel_planar_static_gen6);
	}

	if (intel->video.gen4_sampler_bo == NULL)
		intel->video.gen4_sampler_bo = create_sampler_state(scrn);
		
	if (intel->video.wm_prog_packed_bo == NULL)
		intel->video.wm_prog_packed_bo =
			i965_create_program(scrn,
					packed_ps_kernel,
					packed_ps_size);
		
	if (intel->video.wm_prog_planar_bo == NULL)
		intel->video.wm_prog_planar_bo =
			i965_create_program(scrn,
					planar_ps_kernel,
					planar_ps_size);

	if (intel->video.gen4_cc_vp_bo == NULL)
		intel->video.gen4_cc_vp_bo = i965_create_cc_vp_state(scrn);

	if (intel->video.gen4_cc_bo == NULL)
		intel->video.gen4_cc_bo = gen6_create_cc_state(scrn);

	if (intel->video.gen6_blend_bo == NULL)
		intel->video.gen6_blend_bo = gen6_create_blend_state(scrn);

	if (intel->video.gen6_depth_stencil_bo == NULL)
		intel->video.gen6_depth_stencil_bo = gen6_create_depth_stencil_state(scrn);


	return (intel->video.gen4_sampler_bo != NULL &&
		intel->video.wm_prog_packed_bo != NULL &&
		intel->video.wm_prog_planar_bo != NULL &&
		intel->video.gen4_cc_vp_bo != NULL &&
		intel->video.gen4_cc_bo != NULL &&
		intel->video.gen6_blend_bo != NULL &&
		intel->video.gen6_depth_stencil_bo != NULL);
}

static void
gen6_upload_state_base_address(ScrnInfoPtr scrn, drm_intel_bo *surface_state_binding_table_bo)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	OUT_BATCH(BRW_STATE_BASE_ADDRESS | (10 - 2));
	OUT_BATCH(BASE_ADDRESS_MODIFY); /* General state base address */
	OUT_RELOC(surface_state_binding_table_bo, I915_GEM_DOMAIN_INSTRUCTION, 0, BASE_ADDRESS_MODIFY); /* Surface state base address */
	OUT_BATCH(BASE_ADDRESS_MODIFY); /* Dynamic state base address */
	OUT_BATCH(BASE_ADDRESS_MODIFY); /* Indirect object base address */
	OUT_BATCH(BASE_ADDRESS_MODIFY); /* Instruction base address */
	OUT_BATCH(BASE_ADDRESS_MODIFY); /* General state upper bound */
	OUT_BATCH(BASE_ADDRESS_MODIFY); /* Dynamic state upper bound */
	OUT_BATCH(BASE_ADDRESS_MODIFY); /* Indirect object upper bound */
	OUT_BATCH(BASE_ADDRESS_MODIFY); /* Instruction access upper bound */
}

static void
gen6_upload_drawing_rectangle(ScrnInfoPtr scrn, PixmapPtr pixmap)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	OUT_BATCH(BRW_3DSTATE_DRAWING_RECTANGLE | 2);
	OUT_BATCH(0x00000000);	/* ymin, xmin */
	OUT_BATCH((pixmap->drawable.width - 1) | (pixmap->drawable.height - 1) << 16);	/* ymax, xmax */
	OUT_BATCH(0x00000000);	/* yorigin, xorigin */
}

static void 
gen6_upload_wm_state(ScrnInfoPtr scrn, Bool is_packed)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* disable WM constant buffer */
	OUT_BATCH(GEN6_3DSTATE_CONSTANT_PS | (5 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);

	OUT_BATCH(GEN6_3DSTATE_WM | (9 - 2));
	if (is_packed) {
		OUT_RELOC(intel->video.wm_prog_packed_bo, 
			I915_GEM_DOMAIN_INSTRUCTION, 0,
			0);
		OUT_BATCH((1 << GEN6_3DSTATE_WM_SAMPLER_COUNT_SHITF) |
			(2 << GEN6_3DSTATE_WM_BINDING_TABLE_ENTRY_COUNT_SHIFT));
	} else {
		OUT_RELOC(intel->video.wm_prog_planar_bo,
			I915_GEM_DOMAIN_INSTRUCTION, 0,
			0);
		OUT_BATCH((1 << GEN6_3DSTATE_WM_SAMPLER_COUNT_SHITF) |
			(7 << GEN6_3DSTATE_WM_BINDING_TABLE_ENTRY_COUNT_SHIFT));
	}
	OUT_BATCH(0);
	OUT_BATCH((6 << GEN6_3DSTATE_WM_DISPATCH_START_GRF_0_SHIFT)); /* DW4 */
	OUT_BATCH(((40 - 1) << GEN6_3DSTATE_WM_MAX_THREADS_SHIFT) |
		GEN6_3DSTATE_WM_DISPATCH_ENABLE |
		GEN6_3DSTATE_WM_16_DISPATCH_ENABLE);
	OUT_BATCH((1 << GEN6_3DSTATE_WM_NUM_SF_OUTPUTS_SHIFT) |
		GEN6_3DSTATE_WM_PERSPECTIVE_PIXEL_BARYCENTRIC);
	OUT_BATCH(0);
	OUT_BATCH(0);
}

static void
gen6_upload_vertex_element_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* Set up our vertex elements, sourced from the single vertex buffer. */
	OUT_BATCH(BRW_3DSTATE_VERTEX_ELEMENTS | (5 - 2));
	/* offset 0: X,Y -> {X, Y, 1.0, 1.0} */
	OUT_BATCH((0 << GEN6_VE0_VERTEX_BUFFER_INDEX_SHIFT) |
		GEN6_VE0_VALID |
		(BRW_SURFACEFORMAT_R32G32_FLOAT << VE0_FORMAT_SHIFT) |
		(0 << VE0_OFFSET_SHIFT));
	OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT) |
		(BRW_VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_1_SHIFT) |
		(BRW_VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_2_SHIFT) |
		(BRW_VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_3_SHIFT));
	/* offset 8: S0, T0 -> {S0, T0, 1.0, 1.0} */
	OUT_BATCH((0 << GEN6_VE0_VERTEX_BUFFER_INDEX_SHIFT) |
		GEN6_VE0_VALID |
		(BRW_SURFACEFORMAT_R32G32_FLOAT << VE0_FORMAT_SHIFT) |
		(8 << VE0_OFFSET_SHIFT));
	OUT_BATCH((BRW_VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_0_SHIFT) | 
		(BRW_VFCOMPONENT_STORE_SRC << VE1_VFCOMPONENT_1_SHIFT) |
		(BRW_VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_2_SHIFT) |
		(BRW_VFCOMPONENT_STORE_1_FLT << VE1_VFCOMPONENT_3_SHIFT));
}

static void 
gen6_upload_vertex_buffer(ScrnInfoPtr scrn, drm_intel_bo *vertex_bo, uint32_t end_address_offset)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* Set up the pointer to our vertex buffer */
	OUT_BATCH(BRW_3DSTATE_VERTEX_BUFFERS | (5 - 2));
	/* four 32-bit floats per vertex */
	OUT_BATCH((0 << GEN6_VB0_BUFFER_INDEX_SHIFT) |
		GEN6_VB0_VERTEXDATA | 
		((4 * 4) << VB0_BUFFER_PITCH_SHIFT));
	OUT_RELOC(vertex_bo, I915_GEM_DOMAIN_VERTEX, 0, 0);
	OUT_RELOC(vertex_bo, I915_GEM_DOMAIN_VERTEX, 0, end_address_offset);
	OUT_BATCH(0);	/* reserved */
}

static void
gen6_upload_primitive(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	OUT_BATCH(BRW_3DPRIMITIVE | 
		BRW_3DPRIMITIVE_VERTEX_SEQUENTIAL | 
		(_3DPRIM_RECTLIST << BRW_3DPRIMITIVE_TOPOLOGY_SHIFT) | 
		(0 << 9) | /* Internal Vertex Count */
		(6 - 2));
	OUT_BATCH(3);	/* vertex count per instance */
	OUT_BATCH(0);	/* start vertex offset */
	OUT_BATCH(1);	/* single instance */
	OUT_BATCH(0);	/* start instance location */
	OUT_BATCH(0);	/* index buffer offset, ignored */
}

static void
gen6_emit_video_setup(ScrnInfoPtr scrn,
		drm_intel_bo *surface_state_binding_table_bo, int n_src_surf,
		PixmapPtr pixmap,
		drm_intel_bo *vertex_bo, uint32_t end_address_offset)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	assert(n_src_surf == 1 || n_src_surf == 6);
	IntelEmitInvarientState(scrn);
	intel->last_3d = LAST_3D_VIDEO;
	intel->needs_3d_invariant = TRUE;

	gen6_upload_invariant_states(intel);
	gen6_upload_state_base_address(scrn, surface_state_binding_table_bo);
	gen6_upload_viewport_state_pointers(intel, intel->video.gen4_cc_vp_bo);
	gen6_upload_urb(intel);
	gen6_upload_cc_state_pointers(intel, intel->video.gen6_blend_bo, intel->video.gen4_cc_bo, intel->video.gen6_depth_stencil_bo, 0);
	gen6_upload_sampler_state_pointers(intel, intel->video.gen4_sampler_bo);
	gen6_upload_vs_state(intel);
	gen6_upload_gs_state(intel);
	gen6_upload_clip_state(intel);
	gen6_upload_sf_state(intel, 1, 0);
	gen6_upload_wm_state(scrn, n_src_surf == 1 ? TRUE : FALSE);
	gen6_upload_binding_table(intel, (n_src_surf + 1) * SURFACE_STATE_PADDED_SIZE);
	gen6_upload_depth_buffer_state(intel);
	gen6_upload_drawing_rectangle(scrn, pixmap);
	gen6_upload_vertex_element_state(scrn);
	gen6_upload_vertex_buffer(scrn, vertex_bo, end_address_offset);
	gen6_upload_primitive(scrn);
}

static void 
gen7_upload_wm_state(ScrnInfoPtr scrn, Bool is_packed)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	unsigned int max_threads_shift = GEN7_PS_MAX_THREADS_SHIFT_IVB;
	unsigned int num_samples = 0;

	if (IS_HSW(intel)) {
		max_threads_shift = GEN7_PS_MAX_THREADS_SHIFT_HSW;
		num_samples = 1 << GEN7_PS_SAMPLE_MASK_SHIFT_HSW;
	}

	/* disable WM constant buffer */
	OUT_BATCH(GEN6_3DSTATE_CONSTANT_PS | (7 - 2));
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);
	OUT_BATCH(0);

	OUT_BATCH(GEN6_3DSTATE_WM | (3 - 2));
	OUT_BATCH(GEN7_WM_DISPATCH_ENABLE |
		GEN7_WM_PERSPECTIVE_PIXEL_BARYCENTRIC);
	OUT_BATCH(0);

	OUT_BATCH(GEN7_3DSTATE_PS | (8 - 2));

	if (is_packed) {
		OUT_RELOC(intel->video.wm_prog_packed_bo,
			I915_GEM_DOMAIN_INSTRUCTION, 0,
			0);
		OUT_BATCH((1 << GEN7_PS_SAMPLER_COUNT_SHIFT) |
			(2 << GEN7_PS_BINDING_TABLE_ENTRY_COUNT_SHIFT));
	} else {
		OUT_RELOC(intel->video.wm_prog_planar_bo,
			I915_GEM_DOMAIN_INSTRUCTION, 0,
			0);
		OUT_BATCH((1 << GEN7_PS_SAMPLER_COUNT_SHIFT) |
			(7 << GEN7_PS_BINDING_TABLE_ENTRY_COUNT_SHIFT));
	}

	OUT_BATCH(0); /* scratch space base offset */
	OUT_BATCH(
		((48 - 1) << max_threads_shift) | num_samples |
		GEN7_PS_ATTRIBUTE_ENABLE |
		GEN7_PS_16_DISPATCH_ENABLE);
	OUT_BATCH(
		(6 << GEN7_PS_DISPATCH_START_GRF_SHIFT_0));
	OUT_BATCH(0); /* kernel 1 pointer */
	OUT_BATCH(0); /* kernel 2 pointer */
}

static void 
gen7_upload_vertex_buffer(ScrnInfoPtr scrn, drm_intel_bo *vertex_bo, uint32_t end_address_offset)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* Set up the pointer to our vertex buffer */
	OUT_BATCH(BRW_3DSTATE_VERTEX_BUFFERS | (5 - 2));
	/* four 32-bit floats per vertex */
	OUT_BATCH((0 << GEN6_VB0_BUFFER_INDEX_SHIFT) |
		GEN6_VB0_VERTEXDATA | 
		GEN7_VB0_ADDRESS_MODIFYENABLE |
		((4 * 4) << VB0_BUFFER_PITCH_SHIFT));
	OUT_RELOC(vertex_bo, I915_GEM_DOMAIN_VERTEX, 0, 0);
	OUT_RELOC(vertex_bo, I915_GEM_DOMAIN_VERTEX, 0, end_address_offset);
	OUT_BATCH(0);	/* reserved */
}

static void
gen7_upload_primitive(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	OUT_BATCH(BRW_3DPRIMITIVE | (7 - 2));
	OUT_BATCH(_3DPRIM_RECTLIST |
		GEN7_3DPRIM_VERTEXBUFFER_ACCESS_SEQUENTIAL);
	OUT_BATCH(3); /* vertex count per instance */
	OUT_BATCH(0); /* start vertex offset */
	OUT_BATCH(1); /* single instance */
	OUT_BATCH(0); /* start instance location */
	OUT_BATCH(0);
}

static void
gen7_emit_video_setup(ScrnInfoPtr scrn,
		drm_intel_bo *surface_state_binding_table_bo, int n_src_surf,
		PixmapPtr pixmap,
		drm_intel_bo *vertex_bo, uint32_t end_address_offset)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	assert(n_src_surf == 1 || n_src_surf == 6);
	IntelEmitInvarientState(scrn);
	intel->last_3d = LAST_3D_VIDEO;
	intel->needs_3d_invariant = TRUE;

	gen6_upload_invariant_states(intel);
	gen6_upload_state_base_address(scrn, surface_state_binding_table_bo);
	gen7_upload_viewport_state_pointers(intel, intel->video.gen4_cc_vp_bo);
	gen7_upload_urb(intel);
	gen7_upload_cc_state_pointers(intel, intel->video.gen6_blend_bo, intel->video.gen4_cc_bo, intel->video.gen6_depth_stencil_bo, 0);
	gen7_upload_sampler_state_pointers(intel, intel->video.gen4_sampler_bo);
	gen7_upload_bypass_states(intel);
	gen6_upload_vs_state(intel);
	gen6_upload_clip_state(intel);
	gen7_upload_sf_state(intel, 1, 0);
	gen7_upload_wm_state(scrn, n_src_surf == 1 ? TRUE : FALSE);
	gen7_upload_binding_table(intel, (n_src_surf + 1) * SURFACE_STATE_PADDED_SIZE);
	gen7_upload_depth_buffer_state(intel);
	gen6_upload_drawing_rectangle(scrn, pixmap);
	gen6_upload_vertex_element_state(scrn);
	gen7_upload_vertex_buffer(scrn, vertex_bo, end_address_offset);
	gen7_upload_primitive(scrn);
}

void Gen6DisplayVideoTextured(ScrnInfoPtr scrn,
			intel_adaptor_private *adaptor_priv, int id,
			RegionPtr dstRegion,
			short width, short height,
			int video_pitch, int video_pitch2,
			short src_w, short src_h,
			short drw_w, short drw_h, PixmapPtr pixmap)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	BoxPtr pbox;
	int nbox, dxo, dyo, pix_xoff, pix_yoff;
	float src_scale_x, src_scale_y;
	int src_surf;
	int n_src_surf;
	uint32_t src_surf_format;
	uint32_t src_surf_base[6];
	int src_width[6];
	int src_height[6];
	int src_pitch[6];
	drm_intel_bo *surface_state_binding_table_bo;
	void (*create_dst_surface_state)(ScrnInfoPtr,
					PixmapPtr, 
					drm_intel_bo *, 
					uint32_t);
	void (*create_src_surface_state)(ScrnInfoPtr,
					drm_intel_bo *,
					uint32_t, int, 
					int, int, uint32_t, 
					drm_intel_bo *, uint32_t);
	void (*emit_video_setup)(ScrnInfoPtr,
				drm_intel_bo *, int,
				PixmapPtr,
				drm_intel_bo *, uint32_t);

	if (INTEL_INFO(intel)->gen >= 070) {
		create_dst_surface_state = gen7_create_dst_surface_state;
		create_src_surface_state = gen7_create_src_surface_state;
		emit_video_setup = gen7_emit_video_setup;
	} else {
		create_dst_surface_state = i965_create_dst_surface_state;
		create_src_surface_state = i965_create_src_surface_state;
		emit_video_setup = gen6_emit_video_setup;
	}

	src_surf_base[0] = adaptor_priv->YBufOffset;
	src_surf_base[1] = adaptor_priv->YBufOffset;
	src_surf_base[2] = adaptor_priv->VBufOffset;
	src_surf_base[3] = adaptor_priv->VBufOffset;
	src_surf_base[4] = adaptor_priv->UBufOffset;
	src_surf_base[5] = adaptor_priv->UBufOffset;

	if (is_planar_fourcc(id)) {
		src_surf_format = BRW_SURFACEFORMAT_R8_UNORM;
		src_width[1] = src_width[0] = width;
		src_height[1] = src_height[0] = height;
		src_pitch[1] = src_pitch[0] = video_pitch2;
		src_width[4] = src_width[5] = src_width[2] = src_width[3] =
			width / 2;
		src_height[4] = src_height[5] = src_height[2] = src_height[3] =
			height / 2;
		src_pitch[4] = src_pitch[5] = src_pitch[2] = src_pitch[3] =
			video_pitch;
		n_src_surf = 6;
	} else {
		if (id == FOURCC_UYVY)
			src_surf_format = BRW_SURFACEFORMAT_YCRCB_SWAPY;
		else
			src_surf_format = BRW_SURFACEFORMAT_YCRCB_NORMAL;

		src_width[0] = width;
		src_height[0] = height;
		src_pitch[0] = video_pitch;
		n_src_surf = 1;
	}

	surface_state_binding_table_bo = 
		drm_intel_bo_alloc(intel->bufmgr,
				"surface state & binding table",
				(n_src_surf + 1) * (SURFACE_STATE_PADDED_SIZE + sizeof(uint32_t)),
				4096);

	if (!surface_state_binding_table_bo)
		return;
				
	create_dst_surface_state(scrn, pixmap, surface_state_binding_table_bo, 0);

	for (src_surf = 0; src_surf < n_src_surf; src_surf++) {
		create_src_surface_state(scrn,
					adaptor_priv->buf,
					src_surf_base[src_surf],
					src_width[src_surf],
					src_height[src_surf],
					src_pitch[src_surf],
					src_surf_format,
					surface_state_binding_table_bo,
					(src_surf + 1) * SURFACE_STATE_PADDED_SIZE);
	}

	i965_create_binding_table(scrn, surface_state_binding_table_bo, n_src_surf + 1);

	if (!gen6_create_vidoe_objects(scrn)) {
		drm_intel_bo_unreference(surface_state_binding_table_bo);
		return;
	}

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

	dxo = dstRegion->extents.x1;
	dyo = dstRegion->extents.y1;

	/* Use normalized texture coordinates */
	src_scale_x = ((float)src_w / width) / (float)drw_w;
	src_scale_y = ((float)src_h / height) / (float)drw_h;

	pbox = REGION_RECTS(dstRegion);
	nbox = REGION_NUM_RECTS(dstRegion);
	while (nbox--) {
		int box_x1 = pbox->x1;
		int box_y1 = pbox->y1;
		int box_x2 = pbox->x2;
		int box_y2 = pbox->y2;
		int i;
		float vb[12];
		drm_intel_bo *bo_table[] = {
			NULL,	/* vb_bo */
			intel->batch_bo,
			surface_state_binding_table_bo,
			intel->video.gen4_sampler_bo,
			intel->video.wm_prog_packed_bo,
			intel->video.wm_prog_planar_bo,
			intel->video.gen4_cc_vp_bo,
			intel->video.gen4_cc_bo,
			intel->video.gen6_blend_bo,
			intel->video.gen6_depth_stencil_bo,
		};

		pbox++;

		i = 0;
		vb[i++] = (box_x2 - dxo) * src_scale_x;
		vb[i++] = (box_y2 - dyo) * src_scale_y;
		vb[i++] = (float)box_x2 + pix_xoff;
		vb[i++] = (float)box_y2 + pix_yoff;

		vb[i++] = (box_x1 - dxo) * src_scale_x;
		vb[i++] = (box_y2 - dyo) * src_scale_y;
		vb[i++] = (float)box_x1 + pix_xoff;
		vb[i++] = (float)box_y2 + pix_yoff;

		vb[i++] = (box_x1 - dxo) * src_scale_x;
		vb[i++] = (box_y1 - dyo) * src_scale_y;
		vb[i++] = (float)box_x1 + pix_xoff;
		vb[i++] = (float)box_y1 + pix_yoff;

		bo_table[0] = intel_bo_alloc_for_data(intel,
						      vb, sizeof(vb),
						      "video vbo");

		/* If this command won't fit in the current batch, flush.
		 * Assume that it does after being flushed.
		 */
		if (drm_intel_bufmgr_check_aperture_space(bo_table, ARRAY_SIZE(bo_table)) < 0)
			intel_batch_submit(scrn);

		intel_batch_start_atomic(scrn, 200);
		emit_video_setup(scrn, surface_state_binding_table_bo, n_src_surf, pixmap, bo_table[0], i * 4);
		intel_batch_end_atomic(scrn);

		drm_intel_bo_unreference(bo_table[0]);
	}

	/* release reference once we're finished */
	drm_intel_bo_unreference(surface_state_binding_table_bo);
	intel_debug_flush(scrn);
}
