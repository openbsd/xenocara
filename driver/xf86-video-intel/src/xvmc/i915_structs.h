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

#ifndef _I915_STRUCTS_H
#define _I915_STRUCTS_H

#include <stdint.h>

/* BLT */
#define CMD_2D          0x02
#define OPC_COLOR_BLT                           (0x40)

struct i915_color_blt {
	struct {
		unsigned length:5;
		unsigned pad0:15;
		unsigned bpp_mask:2;
		unsigned opcode:7;
		unsigned type:3;
	} dw0;

	struct {
		unsigned pitch:16;
		unsigned rop:8;
		unsigned color_depth:2;
		unsigned pad0:6;
	} dw1;

	struct {
		unsigned width:16;
		unsigned height:16;
	} dw2;

	struct {
		unsigned address;
	} dw3;

	struct {
		unsigned pattern;
	} dw4;
};

/* 3D_INSTRUCTION */
#define CMD_3D          0x03
#define OPCODE_3D(x)	(CMD_3D << 29 | (x) << 16)

#define OPC_3DMPEG_MACROBLOCK_IPICTURE          (0x01 + (0x1e << 5))
#define OPC_3DMPEG_SET_ORIGIN                   (0x10 + (0x1e << 5))
#define OPC_3DMPEG_MACROBLOCK                   (0x11 + (0x1e << 5))
#define OPC_3DMPEG_SLICE                        (0x12 + (0x1e << 5))
#define OPC_3DMPEG_QM_PALETTE_LOAD              (0x13 + (0x1e << 5))

#define OPC_3DSTATE_SCISSOR_ENABLE              (0x10 + (0x1c << 5))

#define OPC_3DSTATE_MAP_STATE                   (0x00 + (0x1d << 8))
#define OPC_3DSTATE_SAMPLER_STATE               (0x01 + (0x1d << 8))
#define OPC_3DSTATE_LOAD_STATE_IMMEDIATE_1      (0x04 + (0x1d << 8))
#define OP_3D_LOAD_STATE_IMMEDIATE_1      	OPCODE_3D(OPC_3DSTATE_LOAD_STATE_IMMEDIATE_1)
#define OPC_3DSTATE_PIXEL_SHADER_PROGRAM        (0x05 + (0x1d << 8))
#define OPC_3DSTATE_PIXEL_SHADER_CONSTANTS      (0x06 + (0x1d << 8))
#define OPC_3DSTATE_LOAD_INDIRECT               (0x07 + (0x1d << 8))
#define OP_3D_LOAD_INDIRECT               	OPCODE_3D(OPC_3DSTATE_LOAD_INDIRECT)

#define OPC_3DSTATE_MODES_5                     (0x0c)
#define OPC_3DSTATE_COORD_SET_BINDINGS          (0x16)
#define OPC_3DPRIMITIVE                         (0x1f)

#define OPC_3DSTATE_DRAWING_RECTANGLE           (0x80 + (0x1d << 8))
#define OPC_3DSTATE_SCISSOR_RECTANGLE           (0x81 + (0x1d << 8))
#define OPC_3DSTATE_DEST_BUFFER_VARIABLES       (0x85 + (0x1d << 8))
#define OPC_3DSTATE_DEST_BUFFER_VARIABLES_MPEG  (0x87 + (0x1d << 8))
#define OPC_3DSTATE_BUFFER_INFO                 (0x8e + (0x1d << 8))

/*
 * 3DMPEG instructions
 */
struct i915_3dmpeg_macroblock_header {
	struct {
		unsigned length:19;
		unsigned opcode:10;
		unsigned type:3;
	} dw0;

	struct {
		unsigned mb_intra:1;
		unsigned forward:1;
		unsigned backward:1;
		unsigned h263_4mv:1;
		unsigned pad0:1;
		unsigned dct_type:1;
		unsigned pad1:2;
		unsigned motion_type:2;
		unsigned pad2:2;
		unsigned vertical_field_select:4;
		unsigned coded_block_pattern:6;
		unsigned pad3:2;
		unsigned skipped_macroblocks:7;
		unsigned pad4:1;
	} dw1;
};

struct i915_3dmpeg_macroblock_0mv {
	struct i915_3dmpeg_macroblock_header header;
};

struct i915_3dmpeg_macroblock_1fbmv {
	struct i915_3dmpeg_macroblock_header header;
	unsigned dw2;
	unsigned dw3;
};
struct i915_3dmpeg_macroblock_2fbmv {
	struct i915_3dmpeg_macroblock_header header;
	unsigned dw2;
	unsigned dw3;
	unsigned dw4;
	unsigned dw5;
};

struct i915_3dmpeg_macroblock_5fmv {
	struct i915_3dmpeg_macroblock_header header;
	unsigned dw2;
	unsigned dw3;
	unsigned dw4;
	unsigned dw5;
	unsigned dw6;
};

struct i915_3dmpeg_macroblock_ipicture {
	struct {
		unsigned pad0:5;
		unsigned dct_type:1;
		unsigned pad1:13;
		unsigned opcode:10;
		unsigned type:3;
	} dw0;
};

struct i915_3dmpeg_set_origin {
	struct {
		unsigned length:19;
		unsigned opcode:10;
		unsigned type:3;
	} dw0;

	struct {
		unsigned v_origin:7;
		unsigned pad0:1;
		unsigned h_origin:7;
		unsigned pad1:17;
	} dw1;
};

struct i915_3dmpeg_slice {
	struct {
		unsigned length:19;
		unsigned opcode:10;
		unsigned type:3;
	} dw0;

	struct {
		unsigned fst_mb_bit_off:3;
		unsigned pad0:5;
		unsigned mb_count:7;
		unsigned pad1:1;
		unsigned v_position:7;
		unsigned pad2:1;
		unsigned h_position:7;
		unsigned pad3:1;
	} dw1;

	struct {
		unsigned length_minus_one:17;
		unsigned pad0:7;
		unsigned qt_scale_code:5;
		unsigned pad1:3;
	} dw2;
};

struct i915_3dmpeg_qm_palette_load {
	struct {
		unsigned length:4;
		unsigned pad0:15;
		unsigned opcode:10;
		unsigned type:3;
	} dw0;

	unsigned quantmatrix[16];
};

/*
 * 3DSTATE instruction
 */
#define BUFFERID_COLOR_BACK     3
#define BUFFERID_COLOR_AUX      4
#define BUFFERID_MC_INTRA_CORR  5
#define BUFFERID_DEPTH          7

#define TILEWALK_XMAJOR         0
#define TILEWALK_YMAJOR         1

struct i915_3dstate_buffer_info {
	struct {
		unsigned length:16;
		unsigned opcode:13;
		unsigned type:3;
	} dw0;

	struct {
		unsigned pad0:2;
		unsigned pitch:12;
		unsigned pad1:7;
		unsigned walk:1;
		unsigned tiled_surface:1;
		unsigned fence_regs:1;
		unsigned buffer_id:4;
		unsigned aux_id:1;
		unsigned pad2:3;
	} dw1;

	struct {
		unsigned pad0:2;
		unsigned base_address:27;
		unsigned pad1:3;
	} dw2;
};

#define COLORBUFFER_8BIT         0x00
#define COLORBUFFER_X1R5G5B5     0x01
#define COLORBUFFER_R5G6B5       0x02
#define COLORBUFFER_A8R8G8B8     0x03
#define COLORBUFFER_YCRCB_SWAP   0x04
#define COLORBUFFER_YCRCB_NORMAL 0x05
#define COLORBUFFER_YCRCB_SWAPUV 0x06
#define COLORBUFFER_YCRCB_SWAPUVY   0x07
#define COLORBUFFER_A4R4G4B4     0x08
#define COLORBUFFER_A1R5G5B5     0x09
#define COLORBUFFER_A2R10G10B10  0x0a

struct i915_3dstate_dest_buffer_variables {
	struct {
		unsigned length:16;
		unsigned opcode:13;
		unsigned type:3;
	} dw0;

	struct {
		unsigned v_ls_offset:1;
		unsigned v_ls:1;
		unsigned depth_fmt:2;
		unsigned pad0:4;
		unsigned color_fmt:4;
		unsigned yuv422_select:3;
		unsigned pad1:1;
		unsigned dest_v_bias:4;
		unsigned dest_h_bias:4;
		unsigned dither_enhancement:1;
		unsigned linear_gamma:1;
		unsigned dither_pattern:2;
		unsigned lod_preclamp:1;
		unsigned edt_zone:1;	/* early depth test in zone rendering */
		unsigned texture_default_color:1;
		unsigned edt_classic:1;	/* early depth test in classic mode */
	} dw1;
};

#define MPEG_DECODE_MC          0
#define MPEG_DECODE_VLD_IDCT_MC 1

#define MPEG_I_PICTURE          1
#define MPEG_P_PICTURE          2
#define MPEG_B_PICTURE          3

#define MC_SUB_1H               0
#define MC_SUB_2H               1
#define MC_SUB_4H               2

#define MC_SUB_1V               0
#define MC_SUB_2V               1

struct i915_3dstate_dest_buffer_variables_mpeg {
	struct {
		unsigned length:16;
		unsigned opcode:13;
		unsigned type:3;
	} dw0;

	struct {
		unsigned picture_width:7;
		unsigned pad0:1;
		unsigned v_subsample_factor:2;
		unsigned h_subsample_factor:2;
		unsigned tff:1;
		unsigned mismatch:1;
		unsigned pad1:1;
		unsigned intra8:1;
		unsigned abort_on_error:8;
		unsigned pad2:4;
		unsigned bidir_avrg_control:1;
		unsigned rcontrol:1;
		unsigned decode_mode:2;
	} dw1;

	struct {
		unsigned pad0:1;
		unsigned picture_coding_type:2;
		unsigned pad1:2;
		unsigned scan_order:1;
		unsigned pad2:2;
		unsigned q_scale_type:1;
		unsigned concealment:1;
		unsigned fpf_dct:1;
		unsigned pad3:2;
		unsigned intra_dc:2;
		unsigned intra_vlc:1;
		unsigned f_code00:4;
		unsigned f_code01:4;
		unsigned f_code10:4;
		unsigned f_code11:4;
	} dw2;
};

struct i915_mc_static_indirect_state_buffer {
	struct i915_3dstate_buffer_info dest_y;
	struct i915_3dstate_buffer_info dest_u;
	struct i915_3dstate_buffer_info dest_v;
	struct i915_3dstate_dest_buffer_variables dest_buf;
	struct i915_3dstate_dest_buffer_variables_mpeg dest_buf_mpeg;
	struct i915_3dstate_buffer_info corr;
};

#define MAP_MAP0        0x0001
#define MAP_MAP1        0x0002
#define MAP_MAP2        0x0004
#define MAP_MAP3        0x0008
#define MAP_MAP4        0x0010
#define MAP_MAP5        0x0020
#define MAP_MAP6        0x0040
#define MAP_MAP7        0x0080
#define MAP_MAP8        0x0100
#define MAP_MAP9        0x0200
#define MAP_MAP10       0x0400
#define MAP_MAP11       0x0800
#define MAP_MAP12       0x1000
#define MAP_MAP13       0x2000
#define MAP_MAP14       0x4000
#define MAP_MAP15       0x8000

struct texture_map {
	struct {
		unsigned v_ls_offset:1;
		unsigned v_ls:1;
		unsigned base_address:27;
		unsigned pad0:2;
		unsigned untrusted:1;
	} tm0;

	struct {
		unsigned tile_walk:1;
		unsigned tiled_surface:1;
		unsigned utilize_fence_regs:1;
		unsigned texel_fmt:4;
		unsigned surface_fmt:3;
		unsigned width:11;
		unsigned height:11;
	} tm1;

	struct {
		unsigned depth:8;
		unsigned mipmap_layout:1;
		unsigned max_lod:6;
		unsigned cube_face:6;
		unsigned pitch:11;
	} tm2;
};

struct i915_3dstate_map_state {
	struct {
		unsigned length:6;
		unsigned pad0:9;
		unsigned retain:1;
		unsigned opcode:13;
		unsigned type:3;
	} dw0;

	struct {
		unsigned map_mask:16;
		unsigned pad0:16;
	} dw1;
};

struct i915_mc_map_state {
	struct i915_3dstate_map_state y_map;
	struct texture_map y_forward;
	struct texture_map y_backward;
	struct i915_3dstate_map_state u_map;
	struct texture_map u_forward;
	struct texture_map u_backward;
	struct i915_3dstate_map_state v_map;
	struct texture_map v_forward;
	struct texture_map v_backward;
};

#define SAMPLER_SAMPLER0        0x0001
#define SAMPLER_SAMPLER1        0x0002
#define SAMPLER_SAMPLER2        0x0004
#define SAMPLER_SAMPLER3        0x0008
#define SAMPLER_SAMPLER4        0x0010
#define SAMPLER_SAMPLER5        0x0020
#define SAMPLER_SAMPLER6        0x0040
#define SAMPLER_SAMPLER7        0x0080
#define SAMPLER_SAMPLER8        0x0100
#define SAMPLER_SAMPLER9        0x0200
#define SAMPLER_SAMPLER10       0x0400
#define SAMPLER_SAMPLER11       0x0800
#define SAMPLER_SAMPLER12       0x1000
#define SAMPLER_SAMPLER13       0x2000
#define SAMPLER_SAMPLER14       0x4000
#define SAMPLER_SAMPLER15       0x8000

#define MIPFILTER_NONE          0
#define MIPFILTER_NEAREST       1
#define MIPFILTER_LINEAR        3

#define MAPFILTER_NEAREST       0
#define MAPFILTER_LINEAR        1
#define MAPFILTER_ANISOTROPIC   2
#define MAPFILTER_4X4_1         3
#define MAPFILTER_4X4_2         4
#define MAPFILTER_4X4_FLAT      5
#define MAPFILTER_MONO          6

#define ANISORATIO_2            0
#define ANISORATIO_4            1

#define PREFILTEROP_ALWAYS      0
#define PREFILTEROP_NEVER       1
#define PREFILTEROP_LESS        2
#define PREFILTEROP_EQUAL       3
#define PREFILTEROP_LEQUAL      4
#define PREFILTEROP_GREATER     5
#define PREFILTEROP_NOTEQUAL    6
#define PREFILTEROP_GEQUAL      7

#define TEXCOORDMODE_WRAP       0
#define TEXCOORDMODE_MIRROR     1
#define TEXCOORDMODE_CLAMP      2
#define TEXCOORDMODE_CUBE       3
#define TEXCOORDMODE_CLAMP_BORDER       4
#define TEXCOORDMODE_MIRROR_ONCE        5

struct texture_sampler {
	struct {
		unsigned shadow_function:3;
		unsigned max_anisotropy:1;
		unsigned shadow_enable:1;
		unsigned lod_bias:9;
		unsigned min_filter:3;
		unsigned mag_filter:3;
		unsigned mip_filter:2;
		unsigned base_level:5;
		unsigned chromakey_index:2;
		unsigned color_conversion:1;
		unsigned planar2packet:1;
		unsigned reverse_gamma:1;
	} ts0;

	struct {
		unsigned east_deinterlacer:1;
		unsigned map_index:4;
		unsigned normalized_coor:1;
		unsigned tcz_control:3;
		unsigned tcy_control:3;
		unsigned tcx_control:3;
		unsigned chromakey_enable:1;
		unsigned keyed_texture_filter:1;
		unsigned kill_pixel:1;
		unsigned pad0:6;
		unsigned min_lod:8;
	} ts1;

	struct {
		unsigned default_color;
	} ts2;
};

struct i915_3dstate_sampler_state {
	struct {
		unsigned length:6;
		unsigned pad0:10;
		unsigned opcode:13;
		unsigned type:3;
	} dw0;

	struct {
		unsigned sampler_masker:16;
		unsigned pad0:16;
	} dw1;
	/* we always use two samplers for mc */
	struct texture_sampler sampler0;
	struct texture_sampler sampler1;
};

struct arithmetic_inst {
	struct {
		unsigned pad0:2;
		unsigned src0_reg:5;
		unsigned src0_reg_t:3;
		unsigned dest_channel_mask:4;
		unsigned dest_reg:4;
		unsigned pad1:1;
		unsigned dest_reg_t:3;
		unsigned dest_saturate:1;
		unsigned pad2:1;
		unsigned opcode:5;
		unsigned pad3:3;
	} dw0;

	struct {
		unsigned src1_y_select:3;
		unsigned src1_y_negate:1;
		unsigned src1_x_select:3;
		unsigned src1_x_negate:1;
		unsigned src1_reg:5;
		unsigned src1_reg_t:3;
		unsigned src0_w_select:3;
		unsigned src0_w_negate:1;
		unsigned src0_z_select:3;
		unsigned src0_z_negate:1;
		unsigned src0_y_select:3;
		unsigned src0_y_negate:1;
		unsigned src0_x_select:3;
		unsigned src0_x_negate:1;
	} dw1;

	struct {
		unsigned src2_w_select:3;
		unsigned src2_w_negate:1;
		unsigned src2_z_select:3;
		unsigned src2_z_negate:1;
		unsigned src2_y_select:3;
		unsigned src2_y_negate:1;
		unsigned src2_x_select:3;
		unsigned src2_x_negate:1;
		unsigned src2_reg:5;
		unsigned src2_reg_t:3;
		unsigned src1_w_select:3;
		unsigned src1_w_negate:1;
		unsigned src1_z_select:3;
		unsigned src1_z_negate:1;
	} dw2;
};

struct texture_inst {
	struct {
		unsigned sampler_reg:4;
		unsigned pad0:10;
		unsigned dest_reg:4;
		unsigned pad1:1;
		unsigned dest_reg_t:3;
		unsigned pad2:2;
		unsigned opcode:5;
		unsigned pad3:3;
	} dw0;

	struct {
		unsigned pad0:16;
		unsigned address_reg:5;
		unsigned pad1:3;
		unsigned address_reg_t:3;
		unsigned pad2:5;
	} dw1;

	struct {
		unsigned pad0;
	} dw2;
};

struct declaration_inst {
	struct {
		unsigned pad0:10;
		unsigned decl_channel_mask:4;
		unsigned decl_reg:4;
		unsigned pad1:1;
		unsigned decl_reg_t:2;
		unsigned pad2:1;
		unsigned sampler_type:2;
		unsigned opcode:5;
		unsigned pad3:3;
	} dw0;

	struct {
		unsigned pad0;
	} dw1;

	struct {
		unsigned pad0;
	} dw2;
};

union shader_inst {
	struct arithmetic_inst a;
	struct texture_inst t;
	struct declaration_inst d;
};

struct i915_3dstate_pixel_shader_header {
	unsigned length:9;
	unsigned pad0:6;
	unsigned retain:1;
	unsigned opcode:13;
	unsigned type:3;
};

struct i915_3dstate_pixel_shader_program {
	struct i915_3dstate_pixel_shader_header shader0;
	/* mov oC, c0.0000 */
	uint32_t inst0[3];

	struct i915_3dstate_pixel_shader_header shader1;
	/* dcl t0.xy */
	/* dcl t1.xy */
	/* dcl_2D s0 */
	/* texld r0, t0, s0 */
	/* mov oC, r0 */
	uint32_t inst1[3 * 5];

	struct i915_3dstate_pixel_shader_header shader2;
	/* dcl t2.xy */
	/* dcl t3.xy */
	/* dcl_2D s1 */
	/* texld r0, t2, s1 */
	/* mov oC, r0 */
	uint32_t inst2[3 * 5];

	struct i915_3dstate_pixel_shader_header shader3;
	/* dcl t0.xy */
	/* dcl t1.xy */
	/* dcl t2.xy */
	/* dcl t3.xy */
	/* dcl_2D s0 */
	/* dcl_2D s1 */
	/* texld r0, t0, s0 */
	/* texld r0, t2, s1 */
	/* add r0, r0, r1 */
	/* mov oC, r0 */
	uint32_t inst3[3 * 10];
};

#define REG_CR0         0x00000001
#define REG_CR1         0x00000002
#define REG_CR2         0x00000004
#define REG_CR3         0x00000008
#define REG_CR4         0x00000010
#define REG_CR5         0x00000020
#define REG_CR6         0x00000040
#define REG_CR7         0x00000080
#define REG_CR8         0x00000100
#define REG_CR9         0x00000200
#define REG_CR10        0x00000400
#define REG_CR11        0x00000800
#define REG_CR12        0x00001000
#define REG_CR13        0x00002000
#define REG_CR14        0x00004000
#define REG_CR15        0x00008000
#define REG_CR16        0x00010000
#define REG_CR17        0x00020000
#define REG_CR18        0x00040000
#define REG_CR19        0x00080000
#define REG_CR20        0x00100000
#define REG_CR21        0x00200000
#define REG_CR22        0x00400000
#define REG_CR23        0x00800000
#define REG_CR24        0x01000000
#define REG_CR25        0x02000000
#define REG_CR26        0x04000000
#define REG_CR27        0x08000000
#define REG_CR28        0x10000000
#define REG_CR29        0x20000000
#define REG_CR30        0x40000000
#define REG_CR31        0x80000000

struct shader_constant {
	float x;
	float y;
	float z;
	float w;
};

struct i915_3dstate_pixel_shader_constants {
	struct {
		unsigned length:8;
		unsigned pad0:8;
		unsigned opcode:13;
		unsigned type:3;
	} dw0;

	struct {
		unsigned reg_mask;
	} dw1;
	/* we only need one constant */
	struct shader_constant value;
};

#define BLOCK_SIS       0x01
#define BLOCK_DIS       0x02
#define BLOCK_SSB       0x04
#define BLOCK_MSB       0x08
#define BLOCK_PSP       0x10
#define BLOCK_PSC       0x20
#define BLOCK_MASK_SHIFT 8

typedef struct _state_ddword {
	struct {
		unsigned valid:1;
		unsigned force:1;
		unsigned buffer_address:30;
	} dw0;

	struct {
		unsigned length:9;
		unsigned pad0:23;
	} dw1;
} sis_state, msb_state;
#define STATE_VALID	0x1
#define STATE_FORCE	0x2

struct i915_3dstate_load_indirect {
	struct {
		unsigned length:8;
		unsigned block_mask:6;
		unsigned mem_select:1;
		unsigned pad0:1;
		unsigned opcode:13;
		unsigned type:3;
	} dw0;
};

#define OP_3D_LOAD_INDIRECT_GFX_ADDR (1 << 14)

#define TEXCOORDFMT_2FP       0x00
#define TEXCOORDFMT_3FP       0x01
#define TEXCOORDFMT_4FP       0x02
#define TEXCOORDFMT_1FP       0x03
#define TEXCOORDFMT_2FP_16    0x04
#define TEXCOORDFMT_4FP_16    0x05
#define TEXCOORDFMT_NOT_PRESENT  0x0f
struct s2_dword {
	unsigned set0_texcoord_fmt:4;
	unsigned set1_texcoord_fmt:4;
	unsigned set2_texcoord_fmt:4;
	unsigned set3_texcoord_fmt:4;
	unsigned set4_texcoord_fmt:4;
	unsigned set5_texcoord_fmt:4;
	unsigned set6_texcoord_fmt:4;
	unsigned set7_texcoord_fmt:4;
};

#define S3_SET0_PCD (1 << 0*4)
#define S3_SET1_PCD (1 << 1*4)
#define S3_SET2_PCD (1 << 2*4)
#define S3_SET3_PCD (1 << 3*4)
#define S3_SET4_PCD (1 << 4*4)
#define S3_SET5_PCD (1 << 5*4)
#define S3_SET6_PCD (1 << 6*4)
#define S3_SET7_PCD (1 << 7*4)

#define VERTEXHAS_XYZ      1
#define VERTEXHAS_XYZW     2
#define VERTEXHAS_XY       3
#define VERTEXHAS_XYW      4

#define CULLMODE_BOTH      0
#define CULLMODE_NONE      1
#define CULLMODE_CW        2
#define CULLMODE_CCW       3

#define SHADEMODE_LINEAR   0
#define SHADEMODE_FLAT     1
struct s4_dword {
	unsigned anti_aliasing_enable:1;
	unsigned sprite_point_enable:1;
	unsigned fog_parameter_present:1;
	unsigned local_depth_offset_enable:1;
	unsigned force_specular_diffuse_color:1;
	unsigned force_default_diffuse_color:1;
	unsigned position_mask:3;
	unsigned local_depth_offset_present:1;
	unsigned diffuse_color_presetn:1;
	unsigned specular_color_fog_factor_present:1;
	unsigned point_width_present:1;
	unsigned cull_mode:2;
	unsigned color_shade_mode:1;
	unsigned specular_shade_mode:1;
	unsigned fog_shade_mode:1;
	unsigned alpha_shade_mode:1;
	unsigned line_width:4;
	unsigned point_width:9;
};

struct s5_dword {
	unsigned logic_op_enable:1;
	unsigned color_dither_enable:1;
	unsigned stencil_test_enable:1;
	unsigned stencil_buffer_write_enable:1;
	unsigned stencil_pass_depth_pass_op:3;
	unsigned stencil_pass_depth_fail_op:3;
	unsigned stencil_fail_op:3;
	unsigned stencil_test_function:3;
	unsigned stencil_reference_value:8;
	unsigned fog_enable:1;
	unsigned global_depth_offset_enable:1;
	unsigned last_pixel_enable:1;
	unsigned force_default_point_width:1;
	unsigned color_buffer_component_write_disable:4;
};

#define S6_COLOR_BUFFER_WRITE		(1 << 2)
#define S6_DST_BLEND_FACTOR_SHIFT	4
#define S6_SRC_BLEND_FACTOR_SHIFT	8
#define S6_DEPTH_TEST_ENABLE		(1 << 19)

struct s7_dword {
	unsigned global_depth_offset_const;
};

#define OP_3D_LOAD_STATE_IMM_LOAD_S0 (1 << 4)
#define OP_3D_LOAD_STATE_IMM_LOAD_S1 (1 << 5)
#define OP_3D_LOAD_STATE_IMM_LOAD_S2 (1 << 6)
#define OP_3D_LOAD_STATE_IMM_LOAD_S3 (1 << 7)
#define OP_3D_LOAD_STATE_IMM_LOAD_S4 (1 << 8)
#define OP_3D_LOAD_STATE_IMM_LOAD_S5 (1 << 9)
#define OP_3D_LOAD_STATE_IMM_LOAD_S6 (1 << 10)
#define OP_3D_LOAD_STATE_IMM_LOAD_S7 (1 << 11)

struct i915_3dstate_scissor_rectangle {
	struct {
		unsigned length:16;
		unsigned opcode:13;
		unsigned type:3;
	} dw0;

	struct {
		unsigned min_x:16;
		unsigned min_y:16;
	} dw1;

	struct {
		unsigned max_x:16;
		unsigned max_y:16;
	} dw2;
};

#define VERTEX_INLINE         0x00
#define VERTEX_INDIRECT       0x01

#define PRIM_TRILIST          0x00
#define PRIM_TRISTRIP         0x01
#define PRIM_TRISTRIP_REVERSE 0x02
#define PRIM_TRIFAN           0x03
#define PRIM_POLYGON          0x04
#define PRIM_LINELIST         0x05
#define PRIM_LINESTRIP        0x06
#define PRIM_RECTLIST         0x07
#define PRIM_POINTLIST        0x08
#define PRIM_DIB              0x09
#define PRIM_CLEAR_RECT       0x0a
#define PRIM_ZONE_INIT        0x0d

struct texture_coordinate_set {
	unsigned tcx;
	unsigned tcy;
};

struct vertex_data {
	unsigned x;
	unsigned y;
	struct texture_coordinate_set tc0;
	struct texture_coordinate_set tc1;
};

struct i915_3dprimitive {
	union {
		struct {
			unsigned length:18;
			unsigned prim:5;
			unsigned vertex_location:1;
			unsigned opcode:5;
			unsigned type:3;
		} inline_prim;

		struct {
			unsigned vertex_count:16;
			unsigned pad0:1;
			unsigned vertex_access_mode:1;
			unsigned prim:5;
			unsigned vertex_location:1;
			unsigned opcode:5;
			unsigned type:3;
		} indirect_prim;
	} dw0;
};
#endif /*_I915_STRUCTS_H */
