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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Xiang Haihao <haihao.xiang@intel.com>
 *
 */

#include <sys/ioctl.h>

#include "i915_xvmc.h"
#include "i915_structs.h"
#include "i915_program.h"

#define ALIGN(i,m)		(((i) + (m) - 1) & ~((m) - 1))

#define STRIDE(w)               (ALIGN((w), 1024))
#define SIZE_Y420(w, h)         (h * STRIDE(w))
#define SIZE_UV420(w, h)        ((h >> 1) * STRIDE(w >> 1))
#define SIZE_YUV420(w, h)       (SIZE_Y420(w,h) + SIZE_UV420(w,h) * 2)
#define UOFFSET(context)        (SIZE_Y420(context->width, context->height))
#define VOFFSET(context)        (SIZE_Y420(context->width, context->height) + \
                                 SIZE_UV420(context->width, context->height))

typedef union {
	int16_t component[2];
	int32_t v;
} vector_t;

static void i915_inst_arith(unsigned int *inst,
			    unsigned int op,
			    unsigned int dest,
			    unsigned int mask,
			    unsigned int saturate,
			    unsigned int src0, unsigned int src1,
			    unsigned int src2)
{
	dest = UREG(GET_UREG_TYPE(dest), GET_UREG_NR(dest));
	*inst = (op | A0_DEST(dest) | mask | saturate | A0_SRC0(src0));
	inst++;
	*inst = (A1_SRC0(src0) | A1_SRC1(src1));
	inst++;
	*inst = (A2_SRC1(src1) | A2_SRC2(src2));
}

static void i915_inst_decl(unsigned int *inst,
			   unsigned int type,
			   unsigned int nr, unsigned int d0_flags)
{
	unsigned int reg = UREG(type, nr);

	*inst = (D0_DCL | D0_DEST(reg) | d0_flags);
	inst++;
	*inst = D1_MBZ;
	inst++;
	*inst = D2_MBZ;
}

static void i915_inst_texld(unsigned int *inst,
			    unsigned int op,
			    unsigned int dest,
			    unsigned int coord, unsigned int sampler)
{
	dest = UREG(GET_UREG_TYPE(dest), GET_UREG_NR(dest));
	*inst = (op | T0_DEST(dest) | T0_SAMPLER(sampler));
	inst++;
	*inst = T1_ADDRESS_REG(coord);
	inst++;
	*inst = T2_MBZ;
}

static void i915_mc_one_time_context_init(XvMCContext * context)
{
	unsigned int dest, src0, src1, src2;
	i915XvMCContext *pI915XvMC = (i915XvMCContext *) context->privData;
	int i;
	struct i915_3dstate_sampler_state *sampler_state;
	struct i915_3dstate_pixel_shader_program *pixel_shader_program;
	struct i915_3dstate_pixel_shader_constants *pixel_shader_constants;

	/* sampler static state */
	drm_intel_gem_bo_map_gtt(pI915XvMC->ssb_bo);
	sampler_state = pI915XvMC->ssb_bo->virtual;

	memset(sampler_state, 0, sizeof(*sampler_state));
	sampler_state->dw0.type = CMD_3D;
	sampler_state->dw0.opcode = OPC_3DSTATE_SAMPLER_STATE;
	sampler_state->dw0.length = 6;
	sampler_state->dw1.sampler_masker = SAMPLER_SAMPLER0 | SAMPLER_SAMPLER1;

	sampler_state->sampler0.ts0.reverse_gamma = 0;
	sampler_state->sampler0.ts0.planar2packet = 0;
	sampler_state->sampler0.ts0.color_conversion = 0;
	sampler_state->sampler0.ts0.chromakey_index = 0;
	sampler_state->sampler0.ts0.base_level = 0;
	sampler_state->sampler0.ts0.mip_filter = MIPFILTER_NONE;	/* NONE */
	sampler_state->sampler0.ts0.mag_filter = MAPFILTER_LINEAR;	/* LINEAR */
	sampler_state->sampler0.ts0.min_filter = MAPFILTER_LINEAR;	/* LINEAR */
	sampler_state->sampler0.ts0.lod_bias = 0;	/* 0.0 */
	sampler_state->sampler0.ts0.shadow_enable = 0;
	sampler_state->sampler0.ts0.max_anisotropy = ANISORATIO_2;
	sampler_state->sampler0.ts0.shadow_function = PREFILTEROP_ALWAYS;
	sampler_state->sampler0.ts1.min_lod = 0;	/* 0.0 Maximum Mip Level */
	sampler_state->sampler0.ts1.kill_pixel = 0;
	sampler_state->sampler0.ts1.keyed_texture_filter = 0;
	sampler_state->sampler0.ts1.chromakey_enable = 0;
	sampler_state->sampler0.ts1.tcx_control = TEXCOORDMODE_CLAMP;
	sampler_state->sampler0.ts1.tcy_control = TEXCOORDMODE_CLAMP;
	sampler_state->sampler0.ts1.tcz_control = TEXCOORDMODE_CLAMP;
	sampler_state->sampler0.ts1.normalized_coor = 0;
	sampler_state->sampler0.ts1.map_index = 0;
	sampler_state->sampler0.ts1.east_deinterlacer = 0;
	sampler_state->sampler0.ts2.default_color = 0;

	sampler_state->sampler1.ts0.reverse_gamma = 0;
	sampler_state->sampler1.ts0.planar2packet = 0;
	sampler_state->sampler1.ts0.color_conversion = 0;
	sampler_state->sampler1.ts0.chromakey_index = 0;
	sampler_state->sampler1.ts0.base_level = 0;
	sampler_state->sampler1.ts0.mip_filter = MIPFILTER_NONE;	/* NONE */
	sampler_state->sampler1.ts0.mag_filter = MAPFILTER_LINEAR;	/* LINEAR */
	sampler_state->sampler1.ts0.min_filter = MAPFILTER_LINEAR;	/* LINEAR */
	sampler_state->sampler1.ts0.lod_bias = 0;	/* 0.0 */
	sampler_state->sampler1.ts0.shadow_enable = 0;
	sampler_state->sampler1.ts0.max_anisotropy = ANISORATIO_2;
	sampler_state->sampler1.ts0.shadow_function = PREFILTEROP_ALWAYS;
	sampler_state->sampler1.ts1.min_lod = 0;	/* 0.0 Maximum Mip Level */
	sampler_state->sampler1.ts1.kill_pixel = 0;
	sampler_state->sampler1.ts1.keyed_texture_filter = 0;
	sampler_state->sampler1.ts1.chromakey_enable = 0;
	sampler_state->sampler1.ts1.tcx_control = TEXCOORDMODE_CLAMP;
	sampler_state->sampler1.ts1.tcy_control = TEXCOORDMODE_CLAMP;
	sampler_state->sampler1.ts1.tcz_control = TEXCOORDMODE_CLAMP;
	sampler_state->sampler1.ts1.normalized_coor = 0;
	sampler_state->sampler1.ts1.map_index = 1;
	sampler_state->sampler1.ts1.east_deinterlacer = 0;
	sampler_state->sampler1.ts2.default_color = 0;

	drm_intel_gem_bo_unmap_gtt(pI915XvMC->ssb_bo);

	/* pixel shader static state */
	drm_intel_gem_bo_map_gtt(pI915XvMC->psp_bo);
	pixel_shader_program = pI915XvMC->psp_bo->virtual;

	memset(pixel_shader_program, 0, sizeof(*pixel_shader_program));
	pixel_shader_program->shader0.type = CMD_3D;
	pixel_shader_program->shader0.opcode = OPC_3DSTATE_PIXEL_SHADER_PROGRAM;
	pixel_shader_program->shader0.retain = 1;
	pixel_shader_program->shader0.length = 2;	/* 1 inst */
	i = 0;

	dest = UREG(REG_TYPE_OC, 0);
	src0 = UREG(REG_TYPE_CONST, 0);
	src1 = 0;
	src2 = 0;
	i915_inst_arith(&pixel_shader_program->inst0[i], A0_MOV,
			dest, A0_DEST_CHANNEL_ALL, A0_DEST_SATURATE, src0, src1,
			src2);

	pixel_shader_program->shader1.type = CMD_3D;
	pixel_shader_program->shader1.opcode = OPC_3DSTATE_PIXEL_SHADER_PROGRAM;
	pixel_shader_program->shader1.retain = 1;
	pixel_shader_program->shader1.length = 14;	/* 5 inst */
	i = 0;
	/* dcl t0.xy */
	i915_inst_decl(&pixel_shader_program->inst1[i], REG_TYPE_T, T_TEX0,
		       D0_CHANNEL_XY);
	i += 3;
	/* dcl t1.xy */
	i915_inst_decl(&pixel_shader_program->inst1[i], REG_TYPE_T, T_TEX1,
		       D0_CHANNEL_XY);
	/* dcl_2D s0 */
	i += 3;
	i915_inst_decl(&pixel_shader_program->inst1[i], REG_TYPE_S, 0,
		       D0_SAMPLE_TYPE_2D);
	/* texld r0, t0, s0 */
	i += 3;
	dest = UREG(REG_TYPE_R, 0);
	src0 = UREG(REG_TYPE_T, 0);	/* COORD */
	src1 = UREG(REG_TYPE_S, 0);	/* SAMPLER */
	i915_inst_texld(&pixel_shader_program->inst1[i], T0_TEXLD, dest, src0,
			src1);
	/* mov oC, r0 */
	i += 3;
	dest = UREG(REG_TYPE_OC, 0);
	src0 = UREG(REG_TYPE_R, 0);
	src1 = src2 = 0;
	i915_inst_arith(&pixel_shader_program->inst1[i], A0_MOV, dest,
			A0_DEST_CHANNEL_ALL, A0_DEST_SATURATE, src0, src1,
			src2);

	pixel_shader_program->shader2.type = CMD_3D;
	pixel_shader_program->shader2.opcode = OPC_3DSTATE_PIXEL_SHADER_PROGRAM;
	pixel_shader_program->shader2.retain = 1;
	pixel_shader_program->shader2.length = 14;	/* 5 inst */
	i = 0;
	/* dcl t2.xy */
	i915_inst_decl(&pixel_shader_program->inst2[i], REG_TYPE_T, T_TEX2,
		       D0_CHANNEL_XY);
	/* dcl t3.xy */
	i += 3;
	i915_inst_decl(&pixel_shader_program->inst2[i], REG_TYPE_T, T_TEX3,
		       D0_CHANNEL_XY);
	/* dcl_2D s1 */
	i += 3;
	i915_inst_decl(&pixel_shader_program->inst2[i], REG_TYPE_S, 1,
		       D0_SAMPLE_TYPE_2D);
	/* texld r0, t2, s1 */
	i += 3;
	dest = UREG(REG_TYPE_R, 0);
	src0 = UREG(REG_TYPE_T, 2);	/* COORD */
	src1 = UREG(REG_TYPE_S, 1);	/* SAMPLER */
	i915_inst_texld(&pixel_shader_program->inst2[i], T0_TEXLD, dest, src0,
			src1);
	/* mov oC, r0 */
	i += 3;
	dest = UREG(REG_TYPE_OC, 0);
	src0 = UREG(REG_TYPE_R, 0);
	src1 = src2 = 0;
	i915_inst_arith(&pixel_shader_program->inst2[i], A0_MOV, dest,
			A0_DEST_CHANNEL_ALL, A0_DEST_SATURATE, src0, src1,
			src2);

	/* Shader 3 */
	pixel_shader_program->shader3.type = CMD_3D;
	pixel_shader_program->shader3.opcode = OPC_3DSTATE_PIXEL_SHADER_PROGRAM;
	pixel_shader_program->shader3.retain = 1;
	pixel_shader_program->shader3.length = 29;	/* 10 inst */
	i = 0;
	/* dcl t0.xy */
	i915_inst_decl(&pixel_shader_program->inst3[i], REG_TYPE_T, T_TEX0,
		       D0_CHANNEL_XY);
	/* dcl t1.xy */
	i += 3;
	i915_inst_decl(&pixel_shader_program->inst3[i], REG_TYPE_T, T_TEX1,
		       D0_CHANNEL_XY);
	/* dcl t2.xy */
	i += 3;
	i915_inst_decl(&pixel_shader_program->inst3[i], REG_TYPE_T, T_TEX2,
		       D0_CHANNEL_XY);
	/* dcl t3.xy */
	i += 3;
	i915_inst_decl(&pixel_shader_program->inst3[i], REG_TYPE_T, T_TEX3,
		       D0_CHANNEL_XY);
	/* dcl_2D s0 */
	i += 3;
	i915_inst_decl(&pixel_shader_program->inst3[i], REG_TYPE_S, 0,
		       D0_SAMPLE_TYPE_2D);
	/* dcl_2D s1 */
	i += 3;
	i915_inst_decl(&pixel_shader_program->inst3[i], REG_TYPE_S, 1,
		       D0_SAMPLE_TYPE_2D);
	/* texld r0, t0, s0 */
	i += 3;
	dest = UREG(REG_TYPE_R, 0);
	src0 = UREG(REG_TYPE_T, 0);	/* COORD */
	src1 = UREG(REG_TYPE_S, 0);	/* SAMPLER */
	i915_inst_texld(&pixel_shader_program->inst3[i], T0_TEXLD, dest, src0,
			src1);
	/* texld r1, t2, s1 */
	i += 3;
	dest = UREG(REG_TYPE_R, 1);
	src0 = UREG(REG_TYPE_T, 2);	/* COORD */
	src1 = UREG(REG_TYPE_S, 1);	/* SAMPLER */
	i915_inst_texld(&pixel_shader_program->inst3[i], T0_TEXLD, dest, src0,
			src1);
	/* add r0, r0, r1 */
	i += 3;
	dest = UREG(REG_TYPE_R, 0);
	src0 = UREG(REG_TYPE_R, 0);
	src1 = UREG(REG_TYPE_R, 1);
	src2 = 0;
	i915_inst_arith(&pixel_shader_program->inst3[i], A0_ADD, dest,
			A0_DEST_CHANNEL_ALL, 0 /* A0_DEST_SATURATE */ , src0,
			src1, src2);
	/* mul oC, r0, c0 */
	i += 3;
	dest = UREG(REG_TYPE_OC, 0);
	src0 = UREG(REG_TYPE_R, 0);
	src1 = UREG(REG_TYPE_CONST, 0);
	src2 = 0;
	i915_inst_arith(&pixel_shader_program->inst3[i], A0_MUL, dest,
			A0_DEST_CHANNEL_ALL, A0_DEST_SATURATE, src0, src1,
			src2);

	drm_intel_gem_bo_unmap_gtt(pI915XvMC->psp_bo);

	/* pixel shader contant static state */
	drm_intel_gem_bo_map_gtt(pI915XvMC->psc_bo);
	pixel_shader_constants = pI915XvMC->psc_bo->virtual;

	memset(pixel_shader_constants, 0, sizeof(*pixel_shader_constants));
	pixel_shader_constants->dw0.type = CMD_3D;
	pixel_shader_constants->dw0.opcode = OPC_3DSTATE_PIXEL_SHADER_CONSTANTS;
	pixel_shader_constants->dw0.length = 4;
	pixel_shader_constants->dw1.reg_mask = REG_CR0;
	pixel_shader_constants->value.x = 0.5;
	pixel_shader_constants->value.y = 0.5;
	pixel_shader_constants->value.z = 0.5;
	pixel_shader_constants->value.w = 0.5;

	drm_intel_gem_bo_unmap_gtt(pI915XvMC->psc_bo);
}

static void i915_mc_one_time_state_emit(XvMCContext * context)
{
	i915XvMCContext *pI915XvMC = (i915XvMCContext *) context->privData;
	uint32_t load_state_immediate_1, load_indirect, s3_dword, s6_dword;
	int mem_select;
	BATCH_LOCALS;

	/* 3DSTATE_LOAD_STATE_IMMEDIATE_1 */
	BEGIN_BATCH(3 + 8);
	load_state_immediate_1 = OP_3D_LOAD_STATE_IMMEDIATE_1;
	load_state_immediate_1 |= OP_3D_LOAD_STATE_IMM_LOAD_S3;
	load_state_immediate_1 |= OP_3D_LOAD_STATE_IMM_LOAD_S6;
	load_state_immediate_1 |= 3 - 2; /* length */
	OUT_BATCH(load_state_immediate_1);

	s3_dword = S3_SET0_PCD | S3_SET1_PCD |
		   S3_SET2_PCD | S3_SET3_PCD |
		   S3_SET4_PCD | S3_SET5_PCD |
		   S3_SET6_PCD | S3_SET7_PCD;
	OUT_BATCH(s3_dword);

	s6_dword = S6_COLOR_BUFFER_WRITE | S6_DEPTH_TEST_ENABLE;
	s6_dword |= 1 << S6_SRC_BLEND_FACTOR_SHIFT;
	s6_dword |= 1 << S6_DST_BLEND_FACTOR_SHIFT;
	OUT_BATCH(s6_dword);

	/* 3DSTATE_LOAD_INDIRECT */
	load_indirect = OP_3D_LOAD_INDIRECT;
	load_indirect |= (BLOCK_DIS | BLOCK_SSB | BLOCK_PSP | BLOCK_PSC)
				<< BLOCK_MASK_SHIFT;
	load_indirect |= 8 - 2; /* length */

	if (pI915XvMC->use_phys_addr)
		mem_select = 0;	/* use physical address */
	else {
		load_indirect |= OP_3D_LOAD_INDIRECT_GFX_ADDR;
		mem_select = 1;	/* use gfx address */
	}

	OUT_BATCH(load_indirect);

	/* Dynamic indirect state buffer */
	OUT_BATCH(0); /* no dynamic indirect state */

	/* Sample state buffer */
	OUT_RELOC(pI915XvMC->ssb_bo, I915_GEM_DOMAIN_INSTRUCTION, 0,
			STATE_VALID | STATE_FORCE);
	OUT_BATCH(7);	/* 8 - 1 */

	/* Pixel shader program buffer */
	OUT_RELOC(pI915XvMC->psp_bo, I915_GEM_DOMAIN_INSTRUCTION, 0,
			STATE_VALID | STATE_FORCE);
	OUT_BATCH(66);	/* 4 + 16 + 16 + 31 - 1 */

	/* Pixel shader constant buffer */
	OUT_RELOC(pI915XvMC->psc_bo, I915_GEM_DOMAIN_INSTRUCTION, 0,
			STATE_VALID | STATE_FORCE);
	OUT_BATCH(5);	/* 6 - 1 */
	ADVANCE_BATCH();
}

static void i915_mc_static_indirect_state_set(XvMCContext * context,
					      XvMCSurface * dest,
					      unsigned int picture_structure,
					      unsigned int flags,
					      unsigned int picture_coding_type)
{
	i915XvMCContext *pI915XvMC = (i915XvMCContext *) context->privData;
	struct intel_xvmc_surface *intel_surf = dest->privData;
	struct i915_mc_static_indirect_state_buffer *buffer_info;

	drm_intel_gem_bo_map_gtt(pI915XvMC->sis_bo);
	buffer_info = pI915XvMC->sis_bo->virtual;

	memset(buffer_info, 0, sizeof(*buffer_info));

	/* dest Y */
	buffer_info->dest_y.dw0.type = CMD_3D;
	buffer_info->dest_y.dw0.opcode = OPC_3DSTATE_BUFFER_INFO;
	buffer_info->dest_y.dw0.length = 1;
	buffer_info->dest_y.dw1.aux_id = 0;
	buffer_info->dest_y.dw1.buffer_id = BUFFERID_COLOR_BACK;
	buffer_info->dest_y.dw1.fence_regs = 0;	/* disabled *//* FIXME: tiled y for performance */
	buffer_info->dest_y.dw1.tiled_surface = 0;	/* linear */
	buffer_info->dest_y.dw1.walk = TILEWALK_XMAJOR;
	buffer_info->dest_y.dw1.pitch = (pI915XvMC->yStride >> 2);	/* in DWords */
	buffer_info->dest_y.dw2.base_address = intel_surf->bo->offset >> 2;	/* starting DWORD address */
	drm_intel_bo_emit_reloc(pI915XvMC->sis_bo,
				offsetof(typeof(*buffer_info),dest_y.dw2),
				intel_surf->bo, 0,
				I915_GEM_DOMAIN_RENDER,
				I915_GEM_DOMAIN_RENDER);

	/* dest U */
	buffer_info->dest_u.dw0.type = CMD_3D;
	buffer_info->dest_u.dw0.opcode = OPC_3DSTATE_BUFFER_INFO;
	buffer_info->dest_u.dw0.length = 1;
	buffer_info->dest_u.dw1.aux_id = 0;
	buffer_info->dest_u.dw1.buffer_id = BUFFERID_COLOR_AUX;
	buffer_info->dest_u.dw1.fence_regs = 0;
	buffer_info->dest_u.dw1.tiled_surface = 0;
	buffer_info->dest_u.dw1.walk = TILEWALK_XMAJOR;
	buffer_info->dest_u.dw1.pitch = (pI915XvMC->uvStride >> 2);	/* in DWords */
	buffer_info->dest_u.dw2.base_address =
		(intel_surf->bo->offset + UOFFSET(context)) >> 2;
	drm_intel_bo_emit_reloc(pI915XvMC->sis_bo,
				offsetof(typeof(*buffer_info),dest_u.dw2),
				intel_surf->bo, UOFFSET(context),
				I915_GEM_DOMAIN_RENDER,
				I915_GEM_DOMAIN_RENDER);

	/* dest V */
	buffer_info->dest_v.dw0.type = CMD_3D;
	buffer_info->dest_v.dw0.opcode = OPC_3DSTATE_BUFFER_INFO;
	buffer_info->dest_v.dw0.length = 1;
	buffer_info->dest_v.dw1.aux_id = 1;
	buffer_info->dest_v.dw1.buffer_id = BUFFERID_COLOR_AUX;
	buffer_info->dest_v.dw1.fence_regs = 0;
	buffer_info->dest_v.dw1.tiled_surface = 0;
	buffer_info->dest_v.dw1.walk = TILEWALK_XMAJOR;
	buffer_info->dest_v.dw1.pitch = (pI915XvMC->uvStride >> 2);	/* in Dwords */
	buffer_info->dest_v.dw2.base_address =
		(intel_surf->bo->offset + VOFFSET(context)) >> 2;
	drm_intel_bo_emit_reloc(pI915XvMC->sis_bo,
				offsetof(typeof(*buffer_info),dest_v.dw2),
				intel_surf->bo, VOFFSET(context),
				I915_GEM_DOMAIN_RENDER,
				I915_GEM_DOMAIN_RENDER);

	/* Dest buffer parameters */
	buffer_info->dest_buf.dw0.type = CMD_3D;
	buffer_info->dest_buf.dw0.opcode = OPC_3DSTATE_DEST_BUFFER_VARIABLES;
	buffer_info->dest_buf.dw0.length = 0;
	buffer_info->dest_buf.dw1.dest_v_bias = 8;	/* 0.5 */
	buffer_info->dest_buf.dw1.dest_h_bias = 8;	/* 0.5 */
	buffer_info->dest_buf.dw1.color_fmt = COLORBUFFER_8BIT;
	buffer_info->dest_buf.dw1.v_ls = 0;	/* fill later */
	buffer_info->dest_buf.dw1.v_ls_offset = 0;	/* fill later */
	if ((picture_structure & XVMC_FRAME_PICTURE) == XVMC_FRAME_PICTURE) {
		;
	} else if ((picture_structure & XVMC_FRAME_PICTURE) == XVMC_TOP_FIELD) {
		buffer_info->dest_buf.dw1.v_ls = 1;
	} else if ((picture_structure & XVMC_FRAME_PICTURE) ==
		   XVMC_BOTTOM_FIELD) {
		buffer_info->dest_buf.dw1.v_ls = 1;
		buffer_info->dest_buf.dw1.v_ls_offset = 1;
	}

	/* MPEG buffer parameters */
	buffer_info->dest_buf_mpeg.dw0.type = CMD_3D;
	buffer_info->dest_buf_mpeg.dw0.opcode =
	    OPC_3DSTATE_DEST_BUFFER_VARIABLES_MPEG;
	buffer_info->dest_buf_mpeg.dw0.length = 1;
	buffer_info->dest_buf_mpeg.dw1.decode_mode = MPEG_DECODE_MC;
	buffer_info->dest_buf_mpeg.dw1.rcontrol = 0;	/* for MPEG-1/MPEG-2 */
	buffer_info->dest_buf_mpeg.dw1.bidir_avrg_control = 0;	/* for MPEG-1/MPEG-2/MPEG-4 */
	buffer_info->dest_buf_mpeg.dw1.abort_on_error = 1;
	buffer_info->dest_buf_mpeg.dw1.intra8 = 0;	/* 16-bit formatted correction data */
	buffer_info->dest_buf_mpeg.dw1.tff = 1;	/* fill later */

	buffer_info->dest_buf_mpeg.dw1.v_subsample_factor = MC_SUB_1V;
	buffer_info->dest_buf_mpeg.dw1.h_subsample_factor = MC_SUB_1H;

	if (picture_structure & XVMC_FRAME_PICTURE) {
		;
	} else if (picture_structure & XVMC_TOP_FIELD) {
		if (flags & XVMC_SECOND_FIELD)
			buffer_info->dest_buf_mpeg.dw1.tff = 0;
		else
			buffer_info->dest_buf_mpeg.dw1.tff = 1;
	} else if (picture_structure & XVMC_BOTTOM_FIELD) {
		if (flags & XVMC_SECOND_FIELD)
			buffer_info->dest_buf_mpeg.dw1.tff = 1;
		else
			buffer_info->dest_buf_mpeg.dw1.tff = 0;
	}

	buffer_info->dest_buf_mpeg.dw1.picture_width = (dest->width >> 4);	/* in macroblocks */
	buffer_info->dest_buf_mpeg.dw2.picture_coding_type =
	    picture_coding_type;

	buffer_info->corr.dw0.type = CMD_3D;
	buffer_info->corr.dw0.opcode = OPC_3DSTATE_BUFFER_INFO;
	buffer_info->corr.dw0.length = 1;
	buffer_info->corr.dw1.aux_id = 0;
	buffer_info->corr.dw1.buffer_id = BUFFERID_MC_INTRA_CORR;
	buffer_info->corr.dw1.aux_id = 0;
	buffer_info->corr.dw1.fence_regs = 0;
	buffer_info->corr.dw1.tiled_surface = 0;
	buffer_info->corr.dw1.walk = 0;
	buffer_info->corr.dw1.pitch = 0;
	buffer_info->corr.dw2.base_address = pI915XvMC->corrdata_bo->offset >> 2;	/* starting DWORD address */
	drm_intel_bo_emit_reloc(pI915XvMC->sis_bo,
				offsetof(typeof(*buffer_info),corr.dw2),
				pI915XvMC->corrdata_bo, 0,
				I915_GEM_DOMAIN_RENDER, 0);

	drm_intel_gem_bo_unmap_gtt(pI915XvMC->sis_bo);
}

static void i915_mc_map_state_set(XvMCContext * context,
				  struct intel_xvmc_surface * privPast,
				  struct intel_xvmc_surface * privFuture)
{
	i915XvMCContext *pI915XvMC = (i915XvMCContext *) context->privData;
	struct i915_mc_map_state *map_state;
	unsigned int w = context->width;
	unsigned int h = context->height;

	drm_intel_gem_bo_map_gtt(pI915XvMC->msb_bo);
	map_state = pI915XvMC->msb_bo->virtual;

	memset(map_state, 0, sizeof(*map_state));

	/* 3DSATE_MAP_STATE: Y */
	map_state->y_map.dw0.type = CMD_3D;
	map_state->y_map.dw0.opcode = OPC_3DSTATE_MAP_STATE;
	map_state->y_map.dw0.retain = 1;
	map_state->y_map.dw0.length = 6;
	map_state->y_map.dw1.map_mask = MAP_MAP0 | MAP_MAP1;

	/* Y Forward (Past) */
	map_state->y_forward.tm0.v_ls_offset = 0;
	map_state->y_forward.tm0.v_ls = 0;
	map_state->y_forward.tm1.tile_walk = TILEWALK_XMAJOR;
	map_state->y_forward.tm1.tiled_surface = 0;
	map_state->y_forward.tm1.utilize_fence_regs = 0;
	map_state->y_forward.tm1.texel_fmt = 0;	/* 8bit */
	map_state->y_forward.tm1.surface_fmt = 1;	/* 8bit */
	map_state->y_forward.tm1.width = w - 1;
	map_state->y_forward.tm1.height = h - 1;
	map_state->y_forward.tm2.depth = 0;
	map_state->y_forward.tm2.max_lod = 0;
	map_state->y_forward.tm2.cube_face = 0;
	map_state->y_forward.tm0.base_address = privPast->bo->offset >> 2;
	drm_intel_bo_emit_reloc(pI915XvMC->msb_bo,
				offsetof(typeof(*map_state),y_forward.tm0),
				privPast->bo, 0,
				I915_GEM_DOMAIN_SAMPLER, 0);
	map_state->y_forward.tm2.pitch = (pI915XvMC->yStride >> 2) - 1;	/* in DWords - 1 */

	/* Y Backward (Future) */
	map_state->y_backward.tm0.v_ls_offset = 0;
	map_state->y_backward.tm0.v_ls = 0;
	map_state->y_backward.tm1.tile_walk = TILEWALK_XMAJOR;
	map_state->y_backward.tm1.tiled_surface = 0;
	map_state->y_backward.tm1.utilize_fence_regs = 0;
	map_state->y_backward.tm1.texel_fmt = 0;	/* 8bit */
	map_state->y_backward.tm1.surface_fmt = 1;	/* 8bit */
	map_state->y_backward.tm1.width = w - 1;
	map_state->y_backward.tm1.height = h - 1;
	map_state->y_backward.tm2.depth = 0;
	map_state->y_backward.tm2.max_lod = 0;
	map_state->y_backward.tm2.cube_face = 0;
	map_state->y_backward.tm0.base_address = privFuture->bo->offset >> 2;
	drm_intel_bo_emit_reloc(pI915XvMC->msb_bo,
				offsetof(typeof(*map_state),y_backward.tm0),
				privFuture->bo, 0,
				I915_GEM_DOMAIN_SAMPLER, 0);
	map_state->y_backward.tm2.pitch = (pI915XvMC->yStride >> 2) - 1;

	/* 3DSATE_MAP_STATE: U */
	map_state->u_map.dw0.type = CMD_3D;
	map_state->u_map.dw0.opcode = OPC_3DSTATE_MAP_STATE;
	map_state->u_map.dw0.retain = 1;
	map_state->u_map.dw0.length = 6;
	map_state->u_map.dw1.map_mask = MAP_MAP0 | MAP_MAP1;

	/* U Forward */
	map_state->u_forward.tm0.v_ls_offset = 0;
	map_state->u_forward.tm0.v_ls = 0;
	map_state->u_forward.tm1.tile_walk = TILEWALK_XMAJOR;
	map_state->u_forward.tm1.tiled_surface = 0;
	map_state->u_forward.tm1.utilize_fence_regs = 0;
	map_state->u_forward.tm1.texel_fmt = 0;	/* 8bit */
	map_state->u_forward.tm1.surface_fmt = 1;	/* 8bit */
	map_state->u_forward.tm1.width = (w >> 1) - 1;
	map_state->u_forward.tm1.height = (h >> 1) - 1;
	map_state->u_forward.tm2.depth = 0;
	map_state->u_forward.tm2.max_lod = 0;
	map_state->u_forward.tm2.cube_face = 0;
	map_state->u_forward.tm0.base_address =
		(privPast->bo->offset + UOFFSET(context)) >> 2;
	drm_intel_bo_emit_reloc(pI915XvMC->msb_bo,
				offsetof(typeof(*map_state),u_forward.tm0),
				privPast->bo, UOFFSET(context),
				I915_GEM_DOMAIN_SAMPLER, 0);
	map_state->u_forward.tm2.pitch = (pI915XvMC->uvStride >> 2) - 1;	/* in DWords - 1 */

	/* U Backward */
	map_state->u_backward.tm0.v_ls_offset = 0;
	map_state->u_backward.tm0.v_ls = 0;
	map_state->u_backward.tm1.tile_walk = TILEWALK_XMAJOR;
	map_state->u_backward.tm1.tiled_surface = 0;
	map_state->u_backward.tm1.utilize_fence_regs = 0;
	map_state->u_backward.tm1.texel_fmt = 0;
	map_state->u_backward.tm1.surface_fmt = 1;
	map_state->u_backward.tm1.width = (w >> 1) - 1;
	map_state->u_backward.tm1.height = (h >> 1) - 1;
	map_state->u_backward.tm2.depth = 0;
	map_state->u_backward.tm2.max_lod = 0;
	map_state->u_backward.tm2.cube_face = 0;
	map_state->u_backward.tm0.base_address =
		(privFuture->bo->offset + UOFFSET(context)) >> 2;
	drm_intel_bo_emit_reloc(pI915XvMC->msb_bo,
				offsetof(typeof(*map_state),u_backward.tm0),
				privFuture->bo, UOFFSET(context),
				I915_GEM_DOMAIN_SAMPLER, 0);
	map_state->u_backward.tm2.pitch = (pI915XvMC->uvStride >> 2) - 1;

	/* 3DSATE_MAP_STATE: V */
	map_state->v_map.dw0.type = CMD_3D;
	map_state->v_map.dw0.opcode = OPC_3DSTATE_MAP_STATE;
	map_state->v_map.dw0.retain = 1;
	map_state->v_map.dw0.length = 6;
	map_state->v_map.dw1.map_mask = MAP_MAP0 | MAP_MAP1;

	/* V Forward */
	map_state->v_forward.tm0.v_ls_offset = 0;
	map_state->v_forward.tm0.v_ls = 0;
	map_state->v_forward.tm1.tile_walk = TILEWALK_XMAJOR;
	map_state->v_forward.tm1.tiled_surface = 0;
	map_state->v_forward.tm1.utilize_fence_regs = 0;
	map_state->v_forward.tm1.texel_fmt = 0;
	map_state->v_forward.tm1.surface_fmt = 1;
	map_state->v_forward.tm1.width = (w >> 1) - 1;
	map_state->v_forward.tm1.height = (h >> 1) - 1;
	map_state->v_forward.tm2.depth = 0;
	map_state->v_forward.tm2.max_lod = 0;
	map_state->v_forward.tm2.cube_face = 0;
	map_state->v_forward.tm0.base_address =
		(privPast->bo->offset + VOFFSET(context)) >> 2;
	drm_intel_bo_emit_reloc(pI915XvMC->msb_bo,
				offsetof(typeof(*map_state),v_forward.tm0),
				privPast->bo, VOFFSET(context),
				I915_GEM_DOMAIN_SAMPLER, 0);
	map_state->v_forward.tm2.pitch = (pI915XvMC->uvStride >> 2) - 1;	/* in DWords - 1 */

	/* V Backward */
	map_state->v_backward.tm0.v_ls_offset = 0;
	map_state->v_backward.tm0.v_ls = 0;
	map_state->v_backward.tm1.tile_walk = TILEWALK_XMAJOR;
	map_state->v_backward.tm1.tiled_surface = 0;
	map_state->v_backward.tm1.utilize_fence_regs = 0;
	map_state->v_backward.tm1.texel_fmt = 0;
	map_state->v_backward.tm1.surface_fmt = 1;
	map_state->v_backward.tm1.width = (w >> 1) - 1;
	map_state->v_backward.tm1.height = (h >> 1) - 1;
	map_state->v_backward.tm2.depth = 0;
	map_state->v_backward.tm2.max_lod = 0;
	map_state->v_backward.tm2.cube_face = 0;
	map_state->v_backward.tm0.base_address =
		(privFuture->bo->offset + VOFFSET(context)) >> 2;
	drm_intel_bo_emit_reloc(pI915XvMC->msb_bo,
				offsetof(typeof(*map_state),v_backward.tm0),
				privFuture->bo, VOFFSET(context),
				I915_GEM_DOMAIN_SAMPLER, 0);
	map_state->v_backward.tm2.pitch = (pI915XvMC->uvStride >> 2) - 1;

	drm_intel_gem_bo_unmap_gtt(pI915XvMC->msb_bo);
}

static void i915_mc_load_indirect_render_emit(XvMCContext * context)
{
	i915XvMCContext *pI915XvMC = (i915XvMCContext *) context->privData;
	int mem_select;
	uint32_t load_indirect;
	BATCH_LOCALS;

	BEGIN_BATCH(5);
	load_indirect = OP_3D_LOAD_INDIRECT;
	load_indirect |= (BLOCK_SIS | BLOCK_MSB) << BLOCK_MASK_SHIFT;
	load_indirect |= 5 - 2; /* length */

	if (pI915XvMC->use_phys_addr)
		mem_select = 0;	/* use physical address */
	else {
		load_indirect |= OP_3D_LOAD_INDIRECT_GFX_ADDR;
		mem_select = 1;	/* use gfx address */
	}
	OUT_BATCH(load_indirect);

	/* Static Indirect state buffer (dest buffer info) */
	OUT_RELOC(pI915XvMC->sis_bo, I915_GEM_DOMAIN_INSTRUCTION, 0,
			STATE_VALID | STATE_FORCE);
	OUT_BATCH(16);	/* 4 * 3 + 2 + 3 - 1 */

	/* Map state buffer (reference buffer info) */
	OUT_RELOC(pI915XvMC->msb_bo, I915_GEM_DOMAIN_INSTRUCTION, 0,
			STATE_VALID | STATE_FORCE);
	OUT_BATCH(23);	/* 3 * 8 - 1 */
	ADVANCE_BATCH();
}

static void i915_mc_mpeg_set_origin(XvMCContext * context, XvMCMacroBlock * mb)
{
	struct i915_3dmpeg_set_origin set_origin;

	/* 3DMPEG_SET_ORIGIN */
	memset(&set_origin, 0, sizeof(set_origin));
	set_origin.dw0.type = CMD_3D;
	set_origin.dw0.opcode = OPC_3DMPEG_SET_ORIGIN;
	set_origin.dw0.length = 0;
	set_origin.dw1.h_origin = mb->x;
	set_origin.dw1.v_origin = mb->y;

	intelBatchbufferData(&set_origin, sizeof(set_origin), 0);
}

static void i915_mc_mpeg_macroblock_ipicture(XvMCContext * context,
					     XvMCMacroBlock * mb)
{
	struct i915_3dmpeg_macroblock_ipicture macroblock_ipicture;

	/* 3DMPEG_MACROBLOCK_IPICTURE */
	memset(&macroblock_ipicture, 0, sizeof(macroblock_ipicture));
	macroblock_ipicture.dw0.type = CMD_3D;
	macroblock_ipicture.dw0.opcode = OPC_3DMPEG_MACROBLOCK_IPICTURE;
	macroblock_ipicture.dw0.dct_type =
	    (mb->dct_type == XVMC_DCT_TYPE_FIELD);

	intelBatchbufferData(&macroblock_ipicture, sizeof(macroblock_ipicture),
			     0);
}

static void i915_mc_mpeg_macroblock_1fbmv(XvMCContext * context,
					  XvMCMacroBlock * mb)
{
	struct i915_3dmpeg_macroblock_1fbmv macroblock_1fbmv;
	vector_t mv0[2];

	/* 3DMPEG_MACROBLOCK(1fbmv) */
	memset(&macroblock_1fbmv, 0, sizeof(macroblock_1fbmv));
	macroblock_1fbmv.header.dw0.type = CMD_3D;
	macroblock_1fbmv.header.dw0.opcode = OPC_3DMPEG_MACROBLOCK;
	macroblock_1fbmv.header.dw0.length = 2;
	macroblock_1fbmv.header.dw1.mb_intra = 0;	/* should be 0 */
	macroblock_1fbmv.header.dw1.forward =
	    ((mb->macroblock_type & XVMC_MB_TYPE_MOTION_FORWARD) ? 1 : 0);
	macroblock_1fbmv.header.dw1.backward =
	    ((mb->macroblock_type & XVMC_MB_TYPE_MOTION_BACKWARD) ? 1 : 0);
	macroblock_1fbmv.header.dw1.h263_4mv = 0;	/* should be 0 */
	macroblock_1fbmv.header.dw1.dct_type =
	    (mb->dct_type == XVMC_DCT_TYPE_FIELD);

	if (!(mb->coded_block_pattern & 0x3f))
		macroblock_1fbmv.header.dw1.dct_type = XVMC_DCT_TYPE_FRAME;

	macroblock_1fbmv.header.dw1.motion_type = (mb->motion_type & 0x03);
	macroblock_1fbmv.header.dw1.vertical_field_select =
	    (mb->motion_vertical_field_select & 0x0f);
	macroblock_1fbmv.header.dw1.coded_block_pattern =
	    mb->coded_block_pattern;
	macroblock_1fbmv.header.dw1.skipped_macroblocks = 0;

	mv0[0].component[0] = mb->PMV[0][0][0];
	mv0[0].component[1] = mb->PMV[0][0][1];
	mv0[1].component[0] = mb->PMV[0][1][0];
	mv0[1].component[1] = mb->PMV[0][1][1];

	macroblock_1fbmv.dw2 = mv0[0].v;
	macroblock_1fbmv.dw3 = mv0[1].v;

	intelBatchbufferData(&macroblock_1fbmv, sizeof(macroblock_1fbmv), 0);
}

static void i915_mc_mpeg_macroblock_2fbmv(XvMCContext * context,
					  XvMCMacroBlock * mb, unsigned int ps)
{
	struct i915_3dmpeg_macroblock_2fbmv macroblock_2fbmv;
	vector_t mv0[2];
	vector_t mv1[2];

	/* 3DMPEG_MACROBLOCK(2fbmv) */
	memset(&macroblock_2fbmv, 0, sizeof(macroblock_2fbmv));
	macroblock_2fbmv.header.dw0.type = CMD_3D;
	macroblock_2fbmv.header.dw0.opcode = OPC_3DMPEG_MACROBLOCK;
	macroblock_2fbmv.header.dw0.length = 4;
	macroblock_2fbmv.header.dw1.mb_intra = 0;	/* should be 0 */
	macroblock_2fbmv.header.dw1.forward =
	    ((mb->macroblock_type & XVMC_MB_TYPE_MOTION_FORWARD) ? 1 : 0);
	macroblock_2fbmv.header.dw1.backward =
	    ((mb->macroblock_type & XVMC_MB_TYPE_MOTION_BACKWARD) ? 1 : 0);
	macroblock_2fbmv.header.dw1.h263_4mv = 0;	/* should be 0 */
	macroblock_2fbmv.header.dw1.dct_type =
	    (mb->dct_type == XVMC_DCT_TYPE_FIELD);

	if (!(mb->coded_block_pattern & 0x3f))
		macroblock_2fbmv.header.dw1.dct_type = XVMC_DCT_TYPE_FRAME;

	macroblock_2fbmv.header.dw1.motion_type = (mb->motion_type & 0x03);
	macroblock_2fbmv.header.dw1.vertical_field_select =
	    (mb->motion_vertical_field_select & 0x0f);
	macroblock_2fbmv.header.dw1.coded_block_pattern =
	    mb->coded_block_pattern;
	macroblock_2fbmv.header.dw1.skipped_macroblocks = 0;

	mv0[0].component[0] = mb->PMV[0][0][0];
	mv0[0].component[1] = mb->PMV[0][0][1];
	mv0[1].component[0] = mb->PMV[0][1][0];
	mv0[1].component[1] = mb->PMV[0][1][1];
	mv1[0].component[0] = mb->PMV[1][0][0];
	mv1[0].component[1] = mb->PMV[1][0][1];
	mv1[1].component[0] = mb->PMV[1][1][0];
	mv1[1].component[1] = mb->PMV[1][1][1];

	if ((ps & XVMC_FRAME_PICTURE) == XVMC_FRAME_PICTURE) {
		if ((mb->motion_type & 3) == XVMC_PREDICTION_FIELD) {
			mv0[0].component[1] = mb->PMV[0][0][1] >> 1;
			mv0[1].component[1] = mb->PMV[0][1][1] >> 1;
			mv1[0].component[1] = mb->PMV[1][0][1] >> 1;
			mv1[1].component[1] = mb->PMV[1][1][1] >> 1;
		} else if ((mb->motion_type & 3) == XVMC_PREDICTION_DUAL_PRIME) {
			mv0[0].component[1] = mb->PMV[0][0][1] >> 1;
			mv0[1].component[1] = mb->PMV[0][1][1] >> 1;	// MPEG2 MV[0][1] isn't used
			mv1[0].component[1] = mb->PMV[1][0][1] >> 1;
			mv1[1].component[1] = mb->PMV[1][1][1] >> 1;
		}
	}

	macroblock_2fbmv.dw2 = mv0[0].v;
	macroblock_2fbmv.dw3 = mv0[1].v;
	macroblock_2fbmv.dw4 = mv1[0].v;
	macroblock_2fbmv.dw5 = mv1[1].v;

	intelBatchbufferData(&macroblock_2fbmv, sizeof(macroblock_2fbmv), 0);
}

static int i915_xvmc_alloc_one_time_buffers(i915XvMCContext *pI915XvMC)
{
	pI915XvMC->ssb_bo = drm_intel_bo_alloc(xvmc_driver->bufmgr,
					       "ssb",
					       GTT_PAGE_SIZE,
					       GTT_PAGE_SIZE);
	if (!pI915XvMC->ssb_bo)
		return 0;

	pI915XvMC->psp_bo = drm_intel_bo_alloc(xvmc_driver->bufmgr,
					       "psp",
					       GTT_PAGE_SIZE,
					       GTT_PAGE_SIZE);
	if (!pI915XvMC->psp_bo)
		return 0;

	pI915XvMC->psc_bo = drm_intel_bo_alloc(xvmc_driver->bufmgr,
					       "psc",
					       GTT_PAGE_SIZE,
					       GTT_PAGE_SIZE);
	if (!pI915XvMC->psc_bo)
		return 0;

	return 1;
}

static void i915_xvmc_free_one_time_buffers(i915XvMCContext *pI915XvMC)
{
	drm_intel_bo_unreference(pI915XvMC->ssb_bo);
	drm_intel_bo_unreference(pI915XvMC->psp_bo);
	drm_intel_bo_unreference(pI915XvMC->psc_bo);
}

/*
 * Function: i915_release_resource
 */
static void i915_release_resource(Display * display, XvMCContext * context)
{
	i915XvMCContext *pI915XvMC;

	if (!(pI915XvMC = context->privData))
		return;

	i915_xvmc_free_one_time_buffers(pI915XvMC);

	free(pI915XvMC);
	context->privData = NULL;
}

static Status i915_xvmc_mc_create_context(Display * display,
					  XvMCContext * context, int priv_count,
					  CARD32 * priv_data)
{
	i915XvMCContext *pI915XvMC = NULL;
	struct intel_xvmc_hw_context *tmpComm = NULL;

	if (priv_count != (sizeof(struct intel_xvmc_hw_context) >> 2)) {
		XVMC_ERR
		    ("_xvmc_create_context() returned incorrect data size!");
		XVMC_INFO("\tExpected %d, got %d",
			  (int)(sizeof(struct intel_xvmc_hw_context) >> 2),
			  priv_count);
		_xvmc_destroy_context(display, context);
		XFree(priv_data);
		context->privData = NULL;
		return BadValue;
	}

	context->privData = (void *)calloc(1, sizeof(i915XvMCContext));
	if (!context->privData) {
		XVMC_ERR("Unable to allocate resources for XvMC context.");
		return BadAlloc;
	}
	pI915XvMC = (i915XvMCContext *) context->privData;

	tmpComm = (struct intel_xvmc_hw_context *) priv_data;
	pI915XvMC->use_phys_addr = tmpComm->i915.use_phys_addr;
	pI915XvMC->comm.surface_bo_size = SIZE_YUV420(context->width,
						      context->height);

	/* Must free the private data we were passed from X */
	XFree(priv_data);
	priv_data = NULL;

	if (!i915_xvmc_alloc_one_time_buffers(pI915XvMC))
		goto free_one_time_buffers;

	/* Initialize private context values */
	pI915XvMC->yStride = STRIDE(context->width);
	pI915XvMC->uvStride = STRIDE(context->width >> 1);

	/* pre-init state buffers */
	i915_mc_one_time_context_init(context);

	return Success;

free_one_time_buffers:
	i915_xvmc_free_one_time_buffers(pI915XvMC);
	free(pI915XvMC);
	context->privData = NULL;
	return BadAlloc;
}

static int i915_xvmc_mc_destroy_context(Display * display,
					XvMCContext * context)
{
	i915XvMCContext *pI915XvMC;

	if (!(pI915XvMC = context->privData))
		return XvMCBadContext;

	/* Pass Control to the X server to destroy the drm_context_t */
	i915_release_resource(display, context);

	return Success;
}

static int i915_xvmc_alloc_render_state_buffers(i915XvMCContext *pI915XvMC)
{
	pI915XvMC->sis_bo = drm_intel_bo_alloc(xvmc_driver->bufmgr,
					       "sis",
					       GTT_PAGE_SIZE,
					       GTT_PAGE_SIZE);
	if (!pI915XvMC->sis_bo)
		return 0;

	pI915XvMC->msb_bo = drm_intel_bo_alloc(xvmc_driver->bufmgr,
					       "msb",
					       GTT_PAGE_SIZE,
					       GTT_PAGE_SIZE);
	if (!pI915XvMC->msb_bo)
		return 0;

	pI915XvMC->corrdata_bo = drm_intel_bo_alloc(xvmc_driver->bufmgr,
					       "corrdata",
					       CORRDATA_SIZE,
					       GTT_PAGE_SIZE);
	if (!pI915XvMC->corrdata_bo)
		return 0;

	return 1;
}

static void i915_xvmc_free_render_state_buffers(i915XvMCContext *pI915XvMC)
{
	drm_intel_bo_unreference(pI915XvMC->sis_bo);
	drm_intel_bo_unreference(pI915XvMC->msb_bo);
	drm_intel_bo_unreference(pI915XvMC->corrdata_bo);
}

static int i915_xvmc_mc_render_surface(Display * display, XvMCContext * context,
				       unsigned int picture_structure,
				       XvMCSurface * target_surface,
				       XvMCSurface * past_surface,
				       XvMCSurface * future_surface,
				       unsigned int flags,
				       unsigned int num_macroblocks,
				       unsigned int first_macroblock,
				       XvMCMacroBlockArray * macroblock_array,
				       XvMCBlockArray * blocks)
{
	int i;
	int picture_coding_type = MPEG_I_PICTURE;
	/* correction data buffer */
	char *corrdata_ptr;
	int corrdata_size = 0;

	/* Block Pointer */
	short *block_ptr;
	/* Current Macroblock Pointer */
	XvMCMacroBlock *mb;

	intel_xvmc_context_ptr intel_ctx;

	struct intel_xvmc_surface *privTarget = NULL;
	struct intel_xvmc_surface *privFuture = NULL;
	struct intel_xvmc_surface *privPast = NULL;
	i915XvMCContext *pI915XvMC = NULL;

	/* Check Parameters for validity */
	if (!display || !context || !target_surface) {
		XVMC_ERR("Invalid Display, Context or Target!");
		return BadValue;
	}

	if (!num_macroblocks)
		return Success;

	if (!macroblock_array || !blocks) {
		XVMC_ERR("Invalid block data!");
		return BadValue;
	}

	if (macroblock_array->num_blocks < (num_macroblocks + first_macroblock)) {
		XVMC_ERR("Too many macroblocks requested for MB array size.");
		return BadValue;
	}

	if (!(pI915XvMC = context->privData))
		return XvMCBadContext;

	if (!(privTarget = target_surface->privData))
		return XvMCBadSurface;

	if (!i915_xvmc_alloc_render_state_buffers(pI915XvMC))
		return BadAlloc;

	intel_ctx = context->privData;
	if (!intel_ctx) {
		XVMC_ERR("Can't find intel xvmc context\n");
		return BadValue;
	}

	/* P Frame Test */
	if (!past_surface) {
		/* Just to avoid some ifs later. */
		privPast = privTarget;
	} else {
		if (!(privPast = past_surface->privData)) {
			return XvMCBadSurface;
		}
		picture_coding_type = MPEG_P_PICTURE;
	}

	/* B Frame Test */
	if (!future_surface) {
		privFuture = privPast;	// privTarget;
	} else {
		if (!past_surface) {
			XVMC_ERR("No Past Surface!");
			return BadValue;
		}

		if (!(privFuture = future_surface->privData)) {
			XVMC_ERR("Invalid Future Surface!");
			return XvMCBadSurface;
		}

		picture_coding_type = MPEG_B_PICTURE;
	}

	LOCK_HARDWARE(intel_ctx->hw_context);
	drm_intel_gem_bo_map_gtt(pI915XvMC->corrdata_bo);
	corrdata_ptr = pI915XvMC->corrdata_bo->virtual;
	corrdata_size = 0;

	for (i = first_macroblock; i < (num_macroblocks + first_macroblock);
	     i++) {
		int bspm = 0;
		mb = &macroblock_array->macro_blocks[i];
		block_ptr = &(blocks->blocks[mb->index << 6]);

		/* Lockup can happen if the coordinates are too far out of range */
		if (mb->x > (target_surface->width >> 4)) {
			mb->x = 0;
			XVMC_INFO("reset x");
		}

		if (mb->y > (target_surface->height >> 4)) {
			mb->y = 0;
			XVMC_INFO("reset y");
		}

		/* Catch no pattern case */
		if (!(mb->macroblock_type & XVMC_MB_TYPE_PATTERN) &&
		    !(mb->macroblock_type & XVMC_MB_TYPE_INTRA) &&
		    mb->coded_block_pattern) {
			mb->coded_block_pattern = 0;
			XVMC_INFO("no coded blocks present!");
		}

		bspm = mb_bytes_420[mb->coded_block_pattern];

		if (!bspm)
			continue;

		corrdata_size += bspm;

		if (corrdata_size > CORRDATA_SIZE) {
			XVMC_ERR("correction data buffer overflow.");
			break;
		}
		memcpy(corrdata_ptr, block_ptr, bspm);
		corrdata_ptr += bspm;
	}

	drm_intel_gem_bo_unmap_gtt(pI915XvMC->corrdata_bo);

	// i915_mc_invalidate_subcontext_buffers(context, BLOCK_SIS | BLOCK_DIS | BLOCK_SSB
	// | BLOCK_MSB | BLOCK_PSP | BLOCK_PSC);

	i915_mc_one_time_state_emit(context);

	i915_mc_static_indirect_state_set(context, target_surface,
					  picture_structure, flags,
					  picture_coding_type);
	/* setup reference surfaces */
	i915_mc_map_state_set(context, privPast, privFuture);

	i915_mc_load_indirect_render_emit(context);

	i915_mc_mpeg_set_origin(context,
				&macroblock_array->macro_blocks
				[first_macroblock]);

	for (i = first_macroblock; i < (num_macroblocks + first_macroblock);
	     i++) {
		mb = &macroblock_array->macro_blocks[i];

		/* Intra Blocks */
		if (mb->macroblock_type & XVMC_MB_TYPE_INTRA) {
			i915_mc_mpeg_macroblock_ipicture(context, mb);
		} else if ((picture_structure & XVMC_FRAME_PICTURE) ==
			   XVMC_FRAME_PICTURE) {
			/* Frame Picture */
			switch (mb->motion_type & 3) {
			case XVMC_PREDICTION_FIELD:	/* Field Based */
				i915_mc_mpeg_macroblock_2fbmv(context, mb,
							      picture_structure);
				break;

			case XVMC_PREDICTION_FRAME:	/* Frame Based */
				i915_mc_mpeg_macroblock_1fbmv(context, mb);
				break;

			case XVMC_PREDICTION_DUAL_PRIME:	/* Dual Prime */
				i915_mc_mpeg_macroblock_2fbmv(context, mb,
							      picture_structure);
				break;

			default:	/* No Motion Type */
				XVMC_ERR
				    ("Invalid Macroblock Parameters found.");
				break;
			}
		} else {	/* Field Picture */
			switch (mb->motion_type & 3) {
			case XVMC_PREDICTION_FIELD:	/* Field Based */
				i915_mc_mpeg_macroblock_1fbmv(context, mb);
				break;

			case XVMC_PREDICTION_16x8:	/* 16x8 MC */
				i915_mc_mpeg_macroblock_2fbmv(context, mb,
							      picture_structure);
				break;

			case XVMC_PREDICTION_DUAL_PRIME:	/* Dual Prime */
				i915_mc_mpeg_macroblock_1fbmv(context, mb);
				break;

			default:	/* No Motion Type */
				XVMC_ERR
				    ("Invalid Macroblock Parameters found.");
				break;
			}
		}
	}

	intelFlushBatch(TRUE);

	i915_xvmc_free_render_state_buffers(pI915XvMC);

	UNLOCK_HARDWARE(intel_ctx->hw_context);
	return 0;
}

struct _intel_xvmc_driver i915_xvmc_mc_driver = {
	.type = XVMC_I915_MPEG2_MC,
	.num_ctx = 0,
	.ctx_list = NULL,
	.create_context = i915_xvmc_mc_create_context,
	.destroy_context = i915_xvmc_mc_destroy_context,
	.render_surface = i915_xvmc_mc_render_surface,
};
