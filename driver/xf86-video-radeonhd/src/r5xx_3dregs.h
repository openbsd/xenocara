/* BROKEN DISCLAIMER */

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

#ifndef HAVE_R5XX_3DREGS_H
#define HAVE_R5XX_3DREGS_H 1

/* PLL register i cannot find any documentation on */
#define R500_DYN_SCLK_PWMEM_PIPE                        0x000d /* PLL */

#define R300_DST_PIPE_CONFIG		                0x170c
#       define R300_PIPE_AUTO_CONFIG                    (1 << 31)

#define RADEON_WAIT_UNTIL                   0x1720
#       define RADEON_WAIT_CRTC_PFLIP       (1 << 0)
#       define RADEON_WAIT_RE_CRTC_VLINE    (1 << 1)
#       define RADEON_WAIT_FE_CRTC_VLINE    (1 << 2)
#       define RADEON_WAIT_CRTC_VLINE       (1 << 3)
#       define RADEON_WAIT_DMA_VID_IDLE     (1 << 8)
#       define RADEON_WAIT_DMA_GUI_IDLE     (1 << 9)
#       define RADEON_WAIT_CMDFIFO          (1 << 10) /* wait for CMDFIFO_ENTRIES */
#       define RADEON_WAIT_OV0_FLIP         (1 << 11)
#       define RADEON_WAIT_AGP_FLUSH        (1 << 13)
#       define RADEON_WAIT_2D_IDLE          (1 << 14)
#       define RADEON_WAIT_3D_IDLE          (1 << 15)
#       define RADEON_WAIT_2D_IDLECLEAN     (1 << 16)
#       define RADEON_WAIT_3D_IDLECLEAN     (1 << 17)
#       define RADEON_WAIT_HOST_IDLECLEAN   (1 << 18)
#       define RADEON_CMDFIFO_ENTRIES_SHIFT 10
#       define RADEON_CMDFIFO_ENTRIES_MASK  0x7f
#       define RADEON_WAIT_VAP_IDLE         (1 << 28)
#       define RADEON_WAIT_BOTH_CRTC_PFLIP  (1 << 30)
#       define RADEON_ENG_DISPLAY_SELECT_CRTC0    (0 << 31)
#       define RADEON_ENG_DISPLAY_SELECT_CRTC1    (1 << 31)

#define RADEON_RB3D_BLENDCNTL               0x1c20
#       define RADEON_COMB_FCN_MASK                    (3  << 12)
#       define RADEON_COMB_FCN_ADD_CLAMP               (0  << 12)
#       define RADEON_COMB_FCN_ADD_NOCLAMP             (1  << 12)
#       define RADEON_COMB_FCN_SUB_CLAMP               (2  << 12)
#       define RADEON_COMB_FCN_SUB_NOCLAMP             (3  << 12)
#       define RADEON_SRC_BLEND_GL_ZERO                (32 << 16)
#       define RADEON_SRC_BLEND_GL_ONE                 (33 << 16)
#       define RADEON_SRC_BLEND_GL_SRC_COLOR           (34 << 16)
#       define RADEON_SRC_BLEND_GL_ONE_MINUS_SRC_COLOR (35 << 16)
#       define RADEON_SRC_BLEND_GL_DST_COLOR           (36 << 16)
#       define RADEON_SRC_BLEND_GL_ONE_MINUS_DST_COLOR (37 << 16)
#       define RADEON_SRC_BLEND_GL_SRC_ALPHA           (38 << 16)
#       define RADEON_SRC_BLEND_GL_ONE_MINUS_SRC_ALPHA (39 << 16)
#       define RADEON_SRC_BLEND_GL_DST_ALPHA           (40 << 16)
#       define RADEON_SRC_BLEND_GL_ONE_MINUS_DST_ALPHA (41 << 16)
#       define RADEON_SRC_BLEND_GL_SRC_ALPHA_SATURATE  (42 << 16)
#       define RADEON_SRC_BLEND_MASK                   (63 << 16)
#       define RADEON_DST_BLEND_GL_ZERO                (32 << 24)
#       define RADEON_DST_BLEND_GL_ONE                 (33 << 24)
#       define RADEON_DST_BLEND_GL_SRC_COLOR           (34 << 24)
#       define RADEON_DST_BLEND_GL_ONE_MINUS_SRC_COLOR (35 << 24)
#       define RADEON_DST_BLEND_GL_DST_COLOR           (36 << 24)
#       define RADEON_DST_BLEND_GL_ONE_MINUS_DST_COLOR (37 << 24)
#       define RADEON_DST_BLEND_GL_SRC_ALPHA           (38 << 24)
#       define RADEON_DST_BLEND_GL_ONE_MINUS_SRC_ALPHA (39 << 24)
#       define RADEON_DST_BLEND_GL_DST_ALPHA           (40 << 24)
#       define RADEON_DST_BLEND_GL_ONE_MINUS_DST_ALPHA (41 << 24)
#       define RADEON_DST_BLEND_MASK                   (63 << 24)

#define R300_VAP_CNTL				        0x2080
#       define R300_PVS_NUM_SLOTS_SHIFT                 0
#       define R300_PVS_NUM_CNTLRS_SHIFT                4
#       define R300_PVS_NUM_FPUS_SHIFT                  8
#       define R300_VF_MAX_VTX_NUM_SHIFT                18
#       define R300_GL_CLIP_SPACE_DEF                   (0 << 22)
#       define R300_DX_CLIP_SPACE_DEF                   (1 << 22)
#       define R500_TCL_STATE_OPTIMIZATION              (1 << 23)

#define R500_VAP_INDEX_OFFSET			        0x208c

#define R300_VAP_OUT_VTX_FMT_0			        0x2090
#       define R300_VTX_POS_PRESENT                     (1 << 0)
#       define R300_VTX_COLOR_0_PRESENT                 (1 << 1)
#       define R300_VTX_COLOR_1_PRESENT                 (1 << 2)
#       define R300_VTX_COLOR_2_PRESENT                 (1 << 3)
#       define R300_VTX_COLOR_3_PRESENT                 (1 << 4)
#       define R300_VTX_PT_SIZE_PRESENT                 (1 << 16)
#define R300_VAP_OUT_VTX_FMT_1			        0x2094
#       define R300_TEX_0_COMP_CNT_SHIFT                0
#       define R300_TEX_1_COMP_CNT_SHIFT                3
#       define R300_TEX_2_COMP_CNT_SHIFT                6
#       define R300_TEX_3_COMP_CNT_SHIFT                9
#       define R300_TEX_4_COMP_CNT_SHIFT                12
#       define R300_TEX_5_COMP_CNT_SHIFT                15
#       define R300_TEX_6_COMP_CNT_SHIFT                18
#       define R300_TEX_7_COMP_CNT_SHIFT                21

#define R300_VAP_VTE_CNTL				0x20B0
#       define R300_VPORT_X_SCALE_ENA                   (1 << 0)
#       define R300_VPORT_X_OFFSET_ENA                  (1 << 1)
#       define R300_VPORT_Y_SCALE_ENA                   (1 << 2)
#       define R300_VPORT_Y_OFFSET_ENA                  (1 << 3)
#       define R300_VPORT_Z_SCALE_ENA                   (1 << 4)
#       define R300_VPORT_Z_OFFSET_ENA                  (1 << 5)
#       define R300_VTX_XY_FMT                          (1 << 8)
#       define R300_VTX_Z_FMT                           (1 << 9)
#       define R300_VTX_W0_FMT                          (1 << 10)
#define R300_VAP_VTX_SIZE				0x20b4

#define R300_VAP_CNTL_STATUS				0x2140
#       define R300_PVS_BYPASS                          (1 << 8)

#define R300_VAP_PROG_STREAM_CNTL_0		        0x2150
#       define R300_DATA_TYPE_0_SHIFT                   0
#       define R300_DATA_TYPE_FLOAT_1                   0
#       define R300_DATA_TYPE_FLOAT_2                   1
#       define R300_DATA_TYPE_FLOAT_3                   2
#       define R300_DATA_TYPE_FLOAT_4                   3
#       define R300_DATA_TYPE_BYTE                      4
#       define R300_DATA_TYPE_D3DCOLOR                  5
#       define R300_DATA_TYPE_SHORT_2                   6
#       define R300_DATA_TYPE_SHORT_4                   7
#       define R300_DATA_TYPE_VECTOR_3_TTT              8
#       define R300_DATA_TYPE_VECTOR_3_EET              9
#       define R300_SKIP_DWORDS_0_SHIFT                 4
#       define R300_DST_VEC_LOC_0_SHIFT                 8
#       define R300_LAST_VEC_0                          (1 << 13)
#       define R300_SIGNED_0                            (1 << 14)
#       define R300_NORMALIZE_0                         (1 << 15)
#       define R300_DATA_TYPE_1_SHIFT                   16
#       define R300_SKIP_DWORDS_1_SHIFT                 20
#       define R300_DST_VEC_LOC_1_SHIFT                 24
#       define R300_LAST_VEC_1                          (1 << 29)
#       define R300_SIGNED_1                            (1 << 30)
#       define R300_NORMALIZE_1                         (1 << 31)

#define R300_VAP_PROG_STREAM_CNTL_1		        0x2154
#       define R300_DATA_TYPE_2_SHIFT                   0
#       define R300_SKIP_DWORDS_2_SHIFT                 4
#       define R300_DST_VEC_LOC_2_SHIFT                 8
#       define R300_LAST_VEC_2                          (1 << 13)
#       define R300_SIGNED_2                            (1 << 14)
#       define R300_NORMALIZE_2                         (1 << 15)
#       define R300_DATA_TYPE_3_SHIFT                   16
#       define R300_SKIP_DWORDS_3_SHIFT                 20
#       define R300_DST_VEC_LOC_3_SHIFT                 24
#       define R300_LAST_VEC_3                          (1 << 29)
#       define R300_SIGNED_3                            (1 << 30)
#       define R300_NORMALIZE_3                         (1 << 31)

#define R300_VAP_VTX_STATE_CNTL		                0x2180

#define R300_VAP_PVS_CODE_CNTL_0			0x22D0
#       define R300_PVS_FIRST_INST_SHIFT                0
#       define R300_PVS_XYZW_VALID_INST_SHIFT           10
#       define R300_PVS_LAST_INST_SHIFT                 20
#define R300_VAP_PVS_CODE_CNTL_1			0x22D8
#       define R300_PVS_LAST_VTX_SRC_INST_SHIFT         0

#define R300_VAP_PSC_SGN_NORM_CNTL		        0x21DC

#define R300_VAP_PROG_STREAM_CNTL_EXT_0	                0x21e0
#       define R300_SWIZZLE_SELECT_X_0_SHIFT            0
#       define R300_SWIZZLE_SELECT_Y_0_SHIFT            3
#       define R300_SWIZZLE_SELECT_Z_0_SHIFT            6
#       define R300_SWIZZLE_SELECT_W_0_SHIFT            9
#       define R300_SWIZZLE_SELECT_X                    0
#       define R300_SWIZZLE_SELECT_Y                    1
#       define R300_SWIZZLE_SELECT_Z                    2
#       define R300_SWIZZLE_SELECT_W                    3
#       define R300_SWIZZLE_SELECT_FP_ZERO              4
#       define R300_SWIZZLE_SELECT_FP_ONE               5
#       define R300_WRITE_ENA_0_SHIFT                   12
#       define R300_WRITE_ENA_X                         1
#       define R300_WRITE_ENA_Y                         2
#       define R300_WRITE_ENA_Z                         4
#       define R300_WRITE_ENA_W                         8
#       define R300_SWIZZLE_SELECT_X_1_SHIFT            16
#       define R300_SWIZZLE_SELECT_Y_1_SHIFT            19
#       define R300_SWIZZLE_SELECT_Z_1_SHIFT            22
#       define R300_SWIZZLE_SELECT_W_1_SHIFT            25
#       define R300_WRITE_ENA_1_SHIFT                   28
#define R300_VAP_PROG_STREAM_CNTL_EXT_1	                0x21e4
#       define R300_SWIZZLE_SELECT_X_2_SHIFT            0
#       define R300_SWIZZLE_SELECT_Y_2_SHIFT            3
#       define R300_SWIZZLE_SELECT_Z_2_SHIFT            6
#       define R300_SWIZZLE_SELECT_W_2_SHIFT            9
#       define R300_WRITE_ENA_2_SHIFT                   12
#       define R300_SWIZZLE_SELECT_X_3_SHIFT            16
#       define R300_SWIZZLE_SELECT_Y_3_SHIFT            19
#       define R300_SWIZZLE_SELECT_Z_3_SHIFT            22
#       define R300_SWIZZLE_SELECT_W_3_SHIFT            25
#       define R300_WRITE_ENA_3_SHIFT                   28

#define R300_VAP_PVS_VECTOR_INDX_REG		        0x2200
#define R300_VAP_PVS_VECTOR_DATA_REG		        0x2204

/* PVS instructions */
/* Opcode and dst instruction */
#define R300_PVS_DST_OPCODE(x)                          (x << 0)
/* Vector ops */
#       define R300_VECTOR_NO_OP                        0
#       define R300_VE_DOT_PRODUCT                      1
#       define R300_VE_MULTIPLY                         2
#       define R300_VE_ADD                              3
#       define R300_VE_MULTIPLY_ADD                     4
#       define R300_VE_DISTANCE_VECTOR                  5
#       define R300_VE_FRACTION                         6
#       define R300_VE_MAXIMUM                          7
#       define R300_VE_MINIMUM                          8
#       define R300_VE_SET_GREATER_THAN_EQUAL           9
#       define R300_VE_SET_LESS_THAN                    10
#       define R300_VE_MULTIPLYX2_ADD                   11
#       define R300_VE_MULTIPLY_CLAMP                   12
#       define R300_VE_FLT2FIX_DX                       13
#       define R300_VE_FLT2FIX_DX_RND                   14
/* R500 additions */
#       define R500_VE_PRED_SET_EQ_PUSH                 15
#       define R500_VE_PRED_SET_GT_PUSH                 16
#       define R500_VE_PRED_SET_GTE_PUSH                17
#       define R500_VE_PRED_SET_NEQ_PUSH                18
#       define R500_VE_COND_WRITE_EQ                    19
#       define R500_VE_COND_WRITE_GT                    20
#       define R500_VE_COND_WRITE_GTE                   21
#       define R500_VE_COND_WRITE_NEQ                   22
#       define R500_VE_COND_MUX_EQ                      23
#       define R500_VE_COND_MUX_GT                      24
#       define R500_VE_COND_MUX_GTE                     25
#       define R500_VE_SET_GREATER_THAN                 26
#       define R500_VE_SET_EQUAL                        27
#       define R500_VE_SET_NOT_EQUAL                    28
/* Math ops */
#       define R300_MATH_NO_OP                          0
#       define R300_ME_EXP_BASE2_DX                     1
#       define R300_ME_LOG_BASE2_DX                     2
#       define R300_ME_EXP_BASEE_FF                     3
#       define R300_ME_LIGHT_COEFF_DX                   4
#       define R300_ME_POWER_FUNC_FF                    5
#       define R300_ME_RECIP_DX                         6
#       define R300_ME_RECIP_FF                         7
#       define R300_ME_RECIP_SQRT_DX                    8
#       define R300_ME_RECIP_SQRT_FF                    9
#       define R300_ME_MULTIPLY                         10
#       define R300_ME_EXP_BASE2_FULL_DX                11
#       define R300_ME_LOG_BASE2_FULL_DX                12
#       define R300_ME_POWER_FUNC_FF_CLAMP_B            13
#       define R300_ME_POWER_FUNC_FF_CLAMP_B1           14
#       define R300_ME_POWER_FUNC_FF_CLAMP_01           15
#       define R300_ME_SIN                              16
#       define R300_ME_COS                              17
/* R500 additions */
#       define R500_ME_LOG_BASE2_IEEE                   18
#       define R500_ME_RECIP_IEEE                       19
#       define R500_ME_RECIP_SQRT_IEEE                  20
#       define R500_ME_PRED_SET_EQ                      21
#       define R500_ME_PRED_SET_GT                      22
#       define R500_ME_PRED_SET_GTE                     23
#       define R500_ME_PRED_SET_NEQ                     24
#       define R500_ME_PRED_SET_CLR                     25
#       define R500_ME_PRED_SET_INV                     26
#       define R500_ME_PRED_SET_POP                     27
#       define R500_ME_PRED_SET_RESTORE                 28
/* macro */
#       define R300_PVS_MACRO_OP_2CLK_MADD              0
#       define R300_PVS_MACRO_OP_2CLK_M2X_ADD           1
#define R300_PVS_DST_MATH_INST                          (1 << 6)
#define R300_PVS_DST_MACRO_INST                         (1 << 7)
#define R300_PVS_DST_REG_TYPE(x)                        (x << 8)
#       define R300_PVS_DST_REG_TEMPORARY               0
#       define R300_PVS_DST_REG_A0                      1
#       define R300_PVS_DST_REG_OUT                     2
#       define R500_PVS_DST_REG_OUT_REPL_X              3
#       define R300_PVS_DST_REG_ALT_TEMPORARY           4
#       define R300_PVS_DST_REG_INPUT                   5
#define R300_PVS_DST_ADDR_MODE_1                        (1 << 12)
#define R300_PVS_DST_OFFSET(x)                          (x << 13)
#define R300_PVS_DST_WE_X                               (1 << 20)
#define R300_PVS_DST_WE_Y                               (1 << 21)
#define R300_PVS_DST_WE_Z                               (1 << 22)
#define R300_PVS_DST_WE_W                               (1 << 23)
#define R300_PVS_DST_VE_SAT                             (1 << 24)
#define R300_PVS_DST_ME_SAT                             (1 << 25)
#define R300_PVS_DST_PRED_ENABLE                        (1 << 26)
#define R300_PVS_DST_PRED_SENSE                         (1 << 27)
#define R300_PVS_DST_DUAL_MATH_OP                       (1 << 28)
#define R300_PVS_DST_ADDR_SEL(x)                        (x << 29)
#define R300_PVS_DST_ADDR_MODE_0                        (1 << 31)
/* src operand instruction */
#define R300_PVS_SRC_REG_TYPE(x)                        (x << 0)
#       define R300_PVS_SRC_REG_TEMPORARY               0
#       define R300_PVS_SRC_REG_INPUT                   1
#       define R300_PVS_SRC_REG_CONSTANT                2
#       define R300_PVS_SRC_REG_ALT_TEMPORARY           3
#define R300_SPARE_0                                    (1 << 2)
#define R300_PVS_SRC_ABS_XYZW                           (1 << 3)
#define R300_PVS_SRC_ADDR_MODE_0                        (1 << 4)
#define R300_PVS_SRC_OFFSET(x)                          (x << 5)
#define R300_PVS_SRC_SWIZZLE_X(x)                       (x << 13)
#define R300_PVS_SRC_SWIZZLE_Y(x)                       (x << 16)
#define R300_PVS_SRC_SWIZZLE_Z(x)                       (x << 19)
#define R300_PVS_SRC_SWIZZLE_W(x)                       (x << 22)
#       define R300_PVS_SRC_SELECT_X                    0
#       define R300_PVS_SRC_SELECT_Y                    1
#       define R300_PVS_SRC_SELECT_Z                    2
#       define R300_PVS_SRC_SELECT_W                    3
#       define R300_PVS_SRC_SELECT_FORCE_0              4
#       define R300_PVS_SRC_SELECT_FORCE_1              5
#define R300_PVS_SRC_NEG_X                              (1 << 25)
#define R300_PVS_SRC_NEG_Y                              (1 << 26)
#define R300_PVS_SRC_NEG_Z                              (1 << 27)
#define R300_PVS_SRC_NEG_W                              (1 << 28)
#define R300_PVS_SRC_ADDR_SEL(x)                        (x << 29)
#define R300_PVS_SRC_ADDR_MODE_1                        (1 << 31)

#define R300_VAP_CLIP_CNTL				0x221c
#       define R300_UCP_ENA_0                           (1 << 0)
#       define R300_UCP_ENA_1                           (1 << 1)
#       define R300_UCP_ENA_2                           (1 << 2)
#       define R300_UCP_ENA_3                           (1 << 3)
#       define R300_UCP_ENA_4                           (1 << 4)
#       define R300_UCP_ENA_5                           (1 << 5)
#       define R300_PS_UCP_MODE_SHIFT                   14
#       define R300_CLIP_DISABLE                        (1 << 16)
#       define R300_UCP_CULL_ONLY_ENA                   (1 << 17)
#       define R300_BOUNDARY_EDGE_FLAG_ENA              (1 << 18)

#define R300_VAP_GB_VERT_CLIP_ADJ		        0x2220
#define R300_VAP_GB_VERT_DISC_ADJ		        0x2224
#define R300_VAP_GB_HORZ_CLIP_ADJ		        0x2228
#define R300_VAP_GB_HORZ_DISC_ADJ		        0x222c

#define R300_VAP_PVS_STATE_FLUSH_REG			0x2284

#define R300_VAP_PVS_FLOW_CNTL_OPC		        0x22DC

#define R300_GB_ENABLE				        0x4008

#define R300_GB_MSPOS0				        0x4010
#       define R300_MS_X0_SHIFT                         0
#       define R300_MS_Y0_SHIFT                         4
#       define R300_MS_X1_SHIFT                         8
#       define R300_MS_Y1_SHIFT                         12
#       define R300_MS_X2_SHIFT                         16
#       define R300_MS_Y2_SHIFT                         20
#       define R300_MSBD0_Y_SHIFT                       24
#       define R300_MSBD0_X_SHIFT                       28
#define R300_GB_MSPOS1				        0x4014
#       define R300_MS_X3_SHIFT                         0
#       define R300_MS_Y3_SHIFT                         4
#       define R300_MS_X4_SHIFT                         8
#       define R300_MS_Y4_SHIFT                         12
#       define R300_MS_X5_SHIFT                         16
#       define R300_MS_Y5_SHIFT                         20
#       define R300_MSBD1_SHIFT                         24

#define R300_GB_TILE_CONFIG				0x4018
#       define R300_ENABLE_TILING                       (1 << 0)
#       define R300_PIPE_COUNT_RV350                    (0 << 1)
#       define R300_PIPE_COUNT_R300                     (3 << 1)
#       define R300_PIPE_COUNT_R420_3P                  (6 << 1)
#       define R300_PIPE_COUNT_R420                     (7 << 1)
#       define R300_TILE_SIZE_8                         (0 << 4)
#       define R300_TILE_SIZE_16                        (1 << 4)
#       define R300_TILE_SIZE_32                        (2 << 4)
#       define R300_SUBPIXEL_1_12                       (0 << 16)
#       define R300_SUBPIXEL_1_16                       (1 << 16)

#define R300_GB_SELECT				        0x401c
#define R300_GB_AA_CONFIG				0x4020

#define R400_GB_PIPE_SELECT                             0x402c

#define R500_RS_IP_0					0x4074
#define R500_RS_IP_1					0x4078
#   define R500_RS_IP_PTR_K0				62
#   define R500_RS_IP_PTR_K1 				63
#   define R500_RS_IP_TEX_PTR_S_SHIFT 			0
#   define R500_RS_IP_TEX_PTR_T_SHIFT 			6
#   define R500_RS_IP_TEX_PTR_R_SHIFT 			12
#   define R500_RS_IP_TEX_PTR_Q_SHIFT 			18
#   define R500_RS_IP_COL_PTR_SHIFT 			24
#   define R500_RS_IP_COL_FMT_SHIFT 			27
#   define R500_RS_IP_COL_FMT_RGBA			(0 << 27)
#   define R500_RS_IP_OFFSET_EN 			(1 << 31)

#define R300_TX_INVALTAGS				0x4100
#define R300_TX_ENABLE				        0x4104
#       define R300_TEX_0_ENABLE                        (1 << 0)
#       define R300_TEX_1_ENABLE                        (1 << 1)

/* R500 US has to be loaded through an index/data pair */
#define R500_GA_US_VECTOR_INDEX				0x4250
#   define R500_US_VECTOR_INDEX(x)			(x << 0)
#   define R500_US_VECTOR_TYPE_INST			(0 << 16)
#   define R500_US_VECTOR_TYPE_CONST			(1 << 16)
#   define R500_US_VECTOR_CLAMP				(1 << 17)
#define R500_GA_US_VECTOR_DATA				0x4254

#define R300_GA_ENHANCE				        0x4274
#       define R300_GA_DEADLOCK_CNTL                    (1 << 0)
#       define R300_GA_FASTSYNC_CNTL                    (1 << 1)

#define R300_GA_COLOR_CONTROL			        0x4278
#       define R300_RGB0_SHADING_SOLID                  (0 << 0)
#       define R300_RGB0_SHADING_FLAT                   (1 << 0)
#       define R300_RGB0_SHADING_GOURAUD                (2 << 0)
#       define R300_ALPHA0_SHADING_SOLID                (0 << 2)
#       define R300_ALPHA0_SHADING_FLAT                 (1 << 2)
#       define R300_ALPHA0_SHADING_GOURAUD              (2 << 2)
#       define R300_RGB1_SHADING_SOLID                  (0 << 4)
#       define R300_RGB1_SHADING_FLAT                   (1 << 4)
#       define R300_RGB1_SHADING_GOURAUD                (2 << 4)
#       define R300_ALPHA1_SHADING_SOLID                (0 << 6)
#       define R300_ALPHA1_SHADING_FLAT                 (1 << 6)
#       define R300_ALPHA1_SHADING_GOURAUD              (2 << 6)
#       define R300_RGB2_SHADING_SOLID                  (0 << 8)
#       define R300_RGB2_SHADING_FLAT                   (1 << 8)
#       define R300_RGB2_SHADING_GOURAUD                (2 << 8)
#       define R300_ALPHA2_SHADING_SOLID                (0 << 10)
#       define R300_ALPHA2_SHADING_FLAT                 (1 << 10)
#       define R300_ALPHA2_SHADING_GOURAUD              (2 << 10)
#       define R300_RGB3_SHADING_SOLID                  (0 << 12)
#       define R300_RGB3_SHADING_FLAT                   (1 << 12)
#       define R300_RGB3_SHADING_GOURAUD                (2 << 12)
#       define R300_ALPHA3_SHADING_SOLID                (0 << 14)
#       define R300_ALPHA3_SHADING_FLAT                 (1 << 14)
#       define R300_ALPHA3_SHADING_GOURAUD              (2 << 14)

#define R300_GA_POLY_MODE				0x4288
#       define R300_FRONT_PTYPE_POINT                   (0 << 4)
#       define R300_FRONT_PTYPE_LINE                    (1 << 4)
#       define R300_FRONT_PTYPE_TRIANGE                 (2 << 4)
#       define R300_BACK_PTYPE_POINT                    (0 << 7)
#       define R300_BACK_PTYPE_LINE                     (1 << 7)
#       define R300_BACK_PTYPE_TRIANGE                  (2 << 7)
#define R300_GA_ROUND_MODE				0x428c
#       define R300_GEOMETRY_ROUND_TRUNC                (0 << 0)
#       define R300_GEOMETRY_ROUND_NEAREST              (1 << 0)
#       define R300_COLOR_ROUND_TRUNC                   (0 << 2)
#       define R300_COLOR_ROUND_NEAREST                 (1 << 2)

#define R300_GA_OFFSET				        0x4290

#define R300_SU_TEX_WRAP				0x42a0
#define R300_SU_POLY_OFFSET_ENABLE		        0x42b4
#define R300_SU_CULL_MODE				0x42b8
#       define R300_CULL_FRONT                          (1 << 0)
#       define R300_CULL_BACK                           (1 << 1)
#       define R300_FACE_POS                            (0 << 2)
#       define R300_FACE_NEG                            (1 << 2)
#define R300_SU_DEPTH_SCALE				0x42c0
#define R300_SU_DEPTH_OFFSET			        0x42c4

#define R500_SU_REG_DEST                                0x42c8

#define R300_RS_COUNT				        0x4300
#	define R300_RS_COUNT_IT_COUNT_SHIFT		0
#	define R300_RS_COUNT_IC_COUNT_SHIFT		7
#	define R300_RS_COUNT_HIRES_EN			(1 << 18)

#define R300_RS_INST_COUNT				0x4304
#	define R300_INST_COUNT_RS(x)		        (x << 0)
#	define R300_RS_W_EN			        (1 << 4)
#	define R300_TX_OFFSET_RS(x)		        (x << 5)

#define R300_RS_IP_0				        0x4310
#define R300_RS_IP_1				        0x4314
#	define R300_RS_TEX_PTR(x)		        (x << 0)
#	define R300_RS_COL_PTR(x)		        (x << 6)
#	define R300_RS_COL_FMT(x)		        (x << 9)
#	define R300_RS_COL_FMT_RGBA		        0
#	define R300_RS_COL_FMT_RGB0		        2
#	define R300_RS_COL_FMT_RGB1		        3
#	define R300_RS_COL_FMT_000A		        4
#	define R300_RS_COL_FMT_0000		        5
#	define R300_RS_COL_FMT_0001		        6
#	define R300_RS_COL_FMT_111A		        8
#	define R300_RS_COL_FMT_1110		        9
#	define R300_RS_COL_FMT_1111		        10
#	define R300_RS_SEL_S(x)		                (x << 13)
#	define R300_RS_SEL_T(x)		                (x << 16)
#	define R300_RS_SEL_R(x)		                (x << 19)
#	define R300_RS_SEL_Q(x)		                (x << 22)
#	define R300_RS_SEL_C0		                0
#	define R300_RS_SEL_C1		                1
#	define R300_RS_SEL_C2		                2
#	define R300_RS_SEL_C3		                3
#	define R300_RS_SEL_K0		                4
#	define R300_RS_SEL_K1		                5

#define R300_RS_INST_0				        0x4330
#define R300_RS_INST_1				        0x4334
#	define R300_INST_TEX_ID(x)		        (x << 0)
#       define R300_RS_INST_TEX_CN_WRITE		(1 << 3)
#	define R300_INST_TEX_ADDR(x)		        (x << 6)

#define R500_RS_INST_0					0x4320
#define R500_RS_INST_1					0x4324
#   define R500_RS_INST_TEX_ID_SHIFT			0
#   define R500_RS_INST_TEX_CN_WRITE			(1 << 4)
#   define R500_RS_INST_TEX_ADDR_SHIFT			5
#   define R500_RS_INST_COL_ID_SHIFT			12
#   define R500_RS_INST_COL_CN_NO_WRITE			(0 << 16)
#   define R500_RS_INST_COL_CN_WRITE			(1 << 16)
#   define R500_RS_INST_COL_CN_WRITE_FBUFFER		(2 << 16)
#   define R500_RS_INST_COL_CN_WRITE_BACKFACE		(3 << 16)
#   define R500_RS_INST_COL_COL_ADDR_SHIFT		18
#   define R500_RS_INST_TEX_ADJ				(1 << 25)
#   define R500_RS_INST_W_CN				(1 << 26)

#define R300_SC_CLIP_0_A				0x43b0
#define R300_SC_CLIP_0_B				0x43b4
#       define R300_CLIP_X_SHIFT                        0
#       define R300_CLIP_Y_SHIFT                        13
#define R300_SC_CLIP_RULE				0x43d0

#define R300_SC_EDGERULE				0x43a8
#define R300_SC_SCISSOR0				0x43e0
#define R300_SC_SCISSOR1				0x43e4
#       define R300_SCISSOR_X_SHIFT                     0
#       define R300_SCISSOR_Y_SHIFT                     13

#define R300_SC_SCREENDOOR				0x43e8

#define R300_TX_FILTER0_0				0x4400
#       define R300_TX_CLAMP_S(x)                       (x << 0)
#       define R300_TX_CLAMP_T(x)                       (x << 3)
#       define R300_TX_CLAMP_R(x)                       (x << 6)
#       define R300_TX_CLAMP_WRAP                       0
#       define R300_TX_CLAMP_MIRROR                     1
#       define R300_TX_CLAMP_CLAMP_LAST                 2
#       define R300_TX_CLAMP_MIRROR_CLAMP_LAST          3
#       define R300_TX_CLAMP_CLAMP_BORDER               4
#       define R300_TX_CLAMP_MIRROR_CLAMP_BORDER        5
#       define R300_TX_CLAMP_CLAMP_GL                   6
#       define R300_TX_CLAMP_MIRROR_CLAMP_GL            7
#       define R300_TX_MAG_FILTER_NEAREST               (1 << 9)
#       define R300_TX_MIN_FILTER_NEAREST               (1 << 11)
#       define R300_TX_MAG_FILTER_LINEAR                (2 << 9)
#       define R300_TX_MIN_FILTER_LINEAR                (2 << 11)
#       define R300_TX_ID_SHIFT                         28

#define R300_TX_FILTER1_0				0x4440

#define R300_TX_FORMAT0_0				0x4480
#       define R300_TXWIDTH_SHIFT                       0
#       define R300_TXHEIGHT_SHIFT                      11
#       define R300_NUM_LEVELS_SHIFT                    26
#       define R300_NUM_LEVELS_MASK                     0x
#       define R300_TXPROJECTED                         (1 << 30)
#       define R300_TXPITCH_EN                          (1 << 31)

#define R300_TX_FORMAT1_0				0x44c0
#	define R300_TX_FORMAT_X8		    0x0
#	define R300_TX_FORMAT_X16		    0x1
#	define R300_TX_FORMAT_Y4X4		    0x2
#	define R300_TX_FORMAT_Y8X8		    0x3
#	define R300_TX_FORMAT_Y16X16		    0x4
#	define R300_TX_FORMAT_Z3Y3X2		    0x5
#	define R300_TX_FORMAT_Z5Y6X5		    0x6
#	define R300_TX_FORMAT_Z6Y5X5		    0x7
#	define R300_TX_FORMAT_Z11Y11X10		    0x8
#	define R300_TX_FORMAT_Z10Y11X11		    0x9
#	define R300_TX_FORMAT_W4Z4Y4X4		    0xA
#	define R300_TX_FORMAT_W1Z5Y5X5		    0xB
#	define R300_TX_FORMAT_W8Z8Y8X8		    0xC
#	define R300_TX_FORMAT_W2Z10Y10X10	    0xD
#	define R300_TX_FORMAT_W16Z16Y16X16	    0xE
#	define R300_TX_FORMAT_DXT1	    	    0xF
#	define R300_TX_FORMAT_DXT3	    	    0x10
#	define R300_TX_FORMAT_DXT5	    	    0x11
#	define R300_TX_FORMAT_D3DMFT_CxV8U8	    0x12     /* no swizzle */
#	define R300_TX_FORMAT_A8R8G8B8	    	    0x13     /* no swizzle */
#	define R300_TX_FORMAT_B8G8_B8G8	    	    0x14     /* no swizzle */
#	define R300_TX_FORMAT_G8R8_G8B8	    	    0x15     /* no swizzle */
#	define R300_TX_FORMAT_VYUY422	    	    0x14     /* no swizzle */
#	define R300_TX_FORMAT_YVYU422	    	    0x15     /* no swizzle */
#	define R300_TX_FORMAT_X24_Y8	    	    0x1e
#	define R300_TX_FORMAT_X32	    	    0x1e
	/* Floating point formats */
	/* Note - hardware supports both 16 and 32 bit floating point */
#	define R300_TX_FORMAT_FL_I16	    	    0x18
#	define R300_TX_FORMAT_FL_I16A16	    	    0x19
#	define R300_TX_FORMAT_FL_R16G16B16A16	    0x1A
#	define R300_TX_FORMAT_FL_I32	    	    0x1B
#	define R300_TX_FORMAT_FL_I32A32	    	    0x1C
#	define R300_TX_FORMAT_FL_R32G32B32A32	    0x1D
	/* alpha modes, convenience mostly */
	/* if you have alpha, pick constant appropriate to the
	   number of channels (1 for I8, 2 for I8A8, 4 for R8G8B8A8, etc */
# 	define R300_TX_FORMAT_ALPHA_1CH		    0x000
# 	define R300_TX_FORMAT_ALPHA_2CH		    0x200
# 	define R300_TX_FORMAT_ALPHA_4CH		    0x600
# 	define R300_TX_FORMAT_ALPHA_NONE	    0xA00
	/* Swizzling */
	/* constants */
#	define R300_TX_FORMAT_X		0
#	define R300_TX_FORMAT_Y		1
#	define R300_TX_FORMAT_Z		2
#	define R300_TX_FORMAT_W		3
#	define R300_TX_FORMAT_ZERO	4
#	define R300_TX_FORMAT_ONE	5
	/* 2.0*Z, everything above 1.0 is set to 0.0 */
#	define R300_TX_FORMAT_CUT_Z	6
	/* 2.0*W, everything above 1.0 is set to 0.0 */
#	define R300_TX_FORMAT_CUT_W	7

#	define R300_TX_FORMAT_B_SHIFT	18
#	define R300_TX_FORMAT_G_SHIFT	15
#	define R300_TX_FORMAT_R_SHIFT	12
#	define R300_TX_FORMAT_A_SHIFT	9

	/* Convenience macro to take care of layout and swizzling */
#	define R300_EASY_TX_FORMAT(B, G, R, A, FMT)	(		\
		((R300_TX_FORMAT_##B)<<R300_TX_FORMAT_B_SHIFT)		\
		| ((R300_TX_FORMAT_##G)<<R300_TX_FORMAT_G_SHIFT)	\
		| ((R300_TX_FORMAT_##R)<<R300_TX_FORMAT_R_SHIFT)	\
		| ((R300_TX_FORMAT_##A)<<R300_TX_FORMAT_A_SHIFT)	\
		| (R300_TX_FORMAT_##FMT)				\
		)

#       define R300_TX_FORMAT_YUV_TO_RGB_CLAMP         (1 << 22)
#       define R300_TX_FORMAT_YUV_TO_RGB_NO_CLAMP      (2 << 22)
#       define R300_TX_FORMAT_SWAP_YUV                 (1 << 24)

#define R300_TX_OFFSET_0				0x4540
#       define R300_ENDIAN_SWAP_16_BIT                  (1 << 0)
#       define R300_ENDIAN_SWAP_32_BIT                  (2 << 0)
#       define R300_ENDIAN_SWAP_HALF_DWORD              (3 << 0)
#       define R300_MACRO_TILE                          (1 << 2)

#define R300_US_CONFIG				        0x4600
#       define R300_NLEVEL_SHIFT                        0
#       define R300_FIRST_TEX                           (1 << 3)
#       define R500_ZERO_TIMES_ANYTHING_EQUALS_ZERO     (1 << 1)
#define R300_US_PIXSIZE				        0x4604
#define R300_US_CODE_OFFSET				0x4608
#       define R300_ALU_CODE_OFFSET(x)                  (x << 0)
#       define R300_ALU_CODE_SIZE(x)                    (x << 6)
#       define R300_TEX_CODE_OFFSET(x)                  (x << 13)
#       define R300_TEX_CODE_SIZE(x)                    (x << 18)
#define R300_US_CODE_ADDR_0				0x4610
#       define R300_ALU_START(x)                        (x << 0)
#       define R300_ALU_SIZE(x)                         (x << 6)
#       define R300_TEX_START(x)                        (x << 12)
#       define R300_TEX_SIZE(x)                         (x << 17)
#       define R300_RGBA_OUT                            (1 << 22)
#       define R300_W_OUT                               (1 << 23)
#define R300_US_CODE_ADDR_1				0x4614
#define R300_US_CODE_ADDR_2				0x4618
#define R300_US_CODE_ADDR_3				0x461c
#define R300_US_TEX_INST_0				0x4620
#define R300_US_TEX_INST_1				0x4624
#define R300_US_TEX_INST_2				0x4628
#       define R300_TEX_SRC_ADDR(x)                     (x << 0)
#       define R300_TEX_DST_ADDR(x)                     (x << 6)
#       define R300_TEX_ID(x)                           (x << 11)
#       define R300_TEX_INST(x)                         (x << 15)
#       define R300_TEX_INST_NOP                        0
#       define R300_TEX_INST_LD                         1
#       define R300_TEX_INST_TEXKILL                    2
#       define R300_TEX_INST_PROJ                       3
#       define R300_TEX_INST_LODBIAS                    4

#define R300_TX_FORMAT2_0				0x4500
#       define R500_TXWIDTH_11                          (1 << 15)
#       define R500_TXHEIGHT_11                         (1 << 16)

#define R300_TX_BORDER_COLOR_0			        0x45c0

#define R500_US_FC_CTRL					0x4624

/* note that these are 8 bit lengths, despite the offsets, at least for R500 */
#define R500_US_CODE_ADDR				0x4630
#   define R500_US_CODE_START_ADDR(x)			(x << 0)
#   define R500_US_CODE_END_ADDR(x)			(x << 16)
#define R500_US_CODE_RANGE				0x4634
#   define R500_US_CODE_RANGE_ADDR(x)			(x << 0)
#   define R500_US_CODE_RANGE_SIZE(x)			(x << 16)
#define R500_US_CODE_OFFSET				0x4638
#   define R500_US_CODE_OFFSET_ADDR(x)			(x << 0)

#define R300_US_OUT_FMT_0				0x46a4
#define R300_US_OUT_FMT_1				0x46a8
#define R300_US_OUT_FMT_2				0x46ac
#define R300_US_OUT_FMT_3				0x46b0
#       define R300_OUT_FMT_C4_8                        (0 << 0)
#       define R300_OUT_FMT_C4_10                       (1 << 0)
#       define R300_OUT_FMT_C4_10_GAMMA                 (2 << 0)
#       define R300_OUT_FMT_C_16                        (3 << 0)
#       define R300_OUT_FMT_C2_16                       (4 << 0)
#       define R300_OUT_FMT_C4_16                       (5 << 0)
#       define R300_OUT_FMT_C_16_MPEG                   (6 << 0)
#       define R300_OUT_FMT_C2_16_MPEG                  (7 << 0)
#       define R300_OUT_FMT_C2_4                        (8 << 0)
#       define R300_OUT_FMT_C_3_3_2                     (9 << 0)
#       define R300_OUT_FMT_C_5_6_5                     (10 << 0)
#       define R300_OUT_FMT_C_11_11_10                  (11 << 0)
#       define R300_OUT_FMT_C_10_11_11                  (12 << 0)
#       define R300_OUT_FMT_C_2_10_10_10                (13 << 0)
#       define R300_OUT_FMT_UNUSED                      (15 << 0)
#       define R300_OUT_FMT_C_16_FP                     (16 << 0)
#       define R300_OUT_FMT_C2_16_FP                    (17 << 0)
#       define R300_OUT_FMT_C4_16_FP                    (18 << 0)
#       define R300_OUT_FMT_C_32_FP                     (19 << 0)
#       define R300_OUT_FMT_C2_32_FP                    (20 << 0)
#       define R300_OUT_FMT_C4_32_FP                    (21 << 0)
#       define R300_OUT_FMT_C0_SEL_ALPHA                (0 << 8)
#       define R300_OUT_FMT_C0_SEL_RED                  (1 << 8)
#       define R300_OUT_FMT_C0_SEL_GREEN                (2 << 8)
#       define R300_OUT_FMT_C0_SEL_BLUE                 (3 << 8)
#       define R300_OUT_FMT_C1_SEL_ALPHA                (0 << 10)
#       define R300_OUT_FMT_C1_SEL_RED                  (1 << 10)
#       define R300_OUT_FMT_C1_SEL_GREEN                (2 << 10)
#       define R300_OUT_FMT_C1_SEL_BLUE                 (3 << 10)
#       define R300_OUT_FMT_C2_SEL_ALPHA                (0 << 12)
#       define R300_OUT_FMT_C2_SEL_RED                  (1 << 12)
#       define R300_OUT_FMT_C2_SEL_GREEN                (2 << 12)
#       define R300_OUT_FMT_C2_SEL_BLUE                 (3 << 12)
#       define R300_OUT_FMT_C3_SEL_ALPHA                (0 << 14)
#       define R300_OUT_FMT_C3_SEL_RED                  (1 << 14)
#       define R300_OUT_FMT_C3_SEL_GREEN                (2 << 14)
#       define R300_OUT_FMT_C3_SEL_BLUE                 (3 << 14)
#define R300_US_W_FMT				        0x46b4

#define R300_US_ALU_RGB_ADDR_0			        0x46c0
#define R300_US_ALU_RGB_ADDR_1			        0x46c4
#define R300_US_ALU_RGB_ADDR_2			        0x46c8
/* for ADDR0-2, values 0-31 specify a location in the pixel stack,
   values 32-63 specify a constant */
#       define R300_ALU_RGB_ADDR0(x)                    (x << 0)
#       define R300_ALU_RGB_ADDR1(x)                    (x << 6)
#       define R300_ALU_RGB_ADDR2(x)                    (x << 12)
/* ADDRD - where on the pixel stack the result of this instruction
   will be written */
#       define R300_ALU_RGB_ADDRD(x)                    (x << 18)
#       define R300_ALU_RGB_WMASK(x)                    (x << 23)
#       define R300_ALU_RGB_OMASK(x)                    (x << 26)
#       define R300_ALU_RGB_MASK_NONE                   0
#       define R300_ALU_RGB_MASK_R                      1
#       define R300_ALU_RGB_MASK_G                      2
#       define R300_ALU_RGB_MASK_B                      4
#       define R300_ALU_RGB_TARGET_A                    (0 << 29)
#       define R300_ALU_RGB_TARGET_B                    (1 << 29)
#       define R300_ALU_RGB_TARGET_C                    (2 << 29)
#       define R300_ALU_RGB_TARGET_D                    (3 << 29)

#define R300_US_ALU_ALPHA_ADDR_0		        0x47c0
#define R300_US_ALU_ALPHA_ADDR_1		        0x47c4
#define R300_US_ALU_ALPHA_ADDR_2		        0x47c8
/* for ADDR0-2, values 0-31 specify a location in the pixel stack,
   values 32-63 specify a constant */
#       define R300_ALU_ALPHA_ADDR0(x)                  (x << 0)
#       define R300_ALU_ALPHA_ADDR1(x)                  (x << 6)
#       define R300_ALU_ALPHA_ADDR2(x)                  (x << 12)
/* ADDRD - where on the pixel stack the result of this instruction
   will be written */
#       define R300_ALU_ALPHA_ADDRD(x)                  (x << 18)
#       define R300_ALU_ALPHA_WMASK(x)                  (x << 23)
#       define R300_ALU_ALPHA_OMASK(x)                  (x << 24)
#       define R300_ALU_ALPHA_OMASK_W(x)                (x << 27)
#       define R300_ALU_ALPHA_MASK_NONE                 0
#       define R300_ALU_ALPHA_MASK_A                    1
#       define R300_ALU_ALPHA_TARGET_A                  (0 << 25)
#       define R300_ALU_ALPHA_TARGET_B                  (1 << 25)
#       define R300_ALU_ALPHA_TARGET_C                  (2 << 25)
#       define R300_ALU_ALPHA_TARGET_D                  (3 << 25)

#define R300_US_ALU_RGB_INST_0			        0x48c0
#define R300_US_ALU_RGB_INST_1			        0x48c4
#define R300_US_ALU_RGB_INST_2			        0x48c8
#       define R300_ALU_RGB_SEL_A(x)                    (x << 0)
#       define R300_ALU_RGB_SRC0_RGB                    0
#       define R300_ALU_RGB_SRC0_RRR                    1
#       define R300_ALU_RGB_SRC0_GGG                    2
#       define R300_ALU_RGB_SRC0_BBB                    3
#       define R300_ALU_RGB_SRC1_RGB                    4
#       define R300_ALU_RGB_SRC1_RRR                    5
#       define R300_ALU_RGB_SRC1_GGG                    6
#       define R300_ALU_RGB_SRC1_BBB                    7
#       define R300_ALU_RGB_SRC2_RGB                    8
#       define R300_ALU_RGB_SRC2_RRR                    9
#       define R300_ALU_RGB_SRC2_GGG                    10
#       define R300_ALU_RGB_SRC2_BBB                    11
#       define R300_ALU_RGB_SRC0_AAA                    12
#       define R300_ALU_RGB_SRC1_AAA                    13
#       define R300_ALU_RGB_SRC2_AAA                    14
#       define R300_ALU_RGB_SRCP_RGB                    15
#       define R300_ALU_RGB_SRCP_RRR                    16
#       define R300_ALU_RGB_SRCP_GGG                    17
#       define R300_ALU_RGB_SRCP_BBB                    18
#       define R300_ALU_RGB_SRCP_AAA                    19
#       define R300_ALU_RGB_0_0                         20
#       define R300_ALU_RGB_1_0                         21
#       define R300_ALU_RGB_0_5                         22
#       define R300_ALU_RGB_SRC0_GBR                    23
#       define R300_ALU_RGB_SRC1_GBR                    24
#       define R300_ALU_RGB_SRC2_GBR                    25
#       define R300_ALU_RGB_SRC0_BRG                    26
#       define R300_ALU_RGB_SRC1_BRG                    27
#       define R300_ALU_RGB_SRC2_BRG                    28
#       define R300_ALU_RGB_SRC0_ABG                    29
#       define R300_ALU_RGB_SRC1_ABG                    30
#       define R300_ALU_RGB_SRC2_ABG                    31
#       define R300_ALU_RGB_MOD_A(x)                    (x << 5)
#       define R300_ALU_RGB_MOD_NOP                     0
#       define R300_ALU_RGB_MOD_NEG                     1
#       define R300_ALU_RGB_MOD_ABS                     2
#       define R300_ALU_RGB_MOD_NAB                     3
#       define R300_ALU_RGB_SEL_B(x)                    (x << 7)
#       define R300_ALU_RGB_MOD_B(x)                    (x << 12)
#       define R300_ALU_RGB_SEL_C(x)                    (x << 14)
#       define R300_ALU_RGB_MOD_C(x)                    (x << 19)
#       define R300_ALU_RGB_SRCP_OP(x)                  (x << 21)
#       define R300_ALU_RGB_SRCP_OP_1_MINUS_2RGB0	0
#       define R300_ALU_RGB_SRCP_OP_RGB1_MINUS_RGB0	1
#       define R300_ALU_RGB_SRCP_OP_RGB1_PLUS_RGB0	2
#       define R300_ALU_RGB_SRCP_OP_1_MINUS_RGB0	3
#       define R300_ALU_RGB_OP(x)                       (x << 23)
#       define R300_ALU_RGB_OP_MAD                      0
#       define R300_ALU_RGB_OP_DP3                      1
#       define R300_ALU_RGB_OP_DP4                      2
#       define R300_ALU_RGB_OP_D2A                      3
#       define R300_ALU_RGB_OP_MIN                      4
#       define R300_ALU_RGB_OP_MAX                      5
#       define R300_ALU_RGB_OP_CND                      7
#       define R300_ALU_RGB_OP_CMP                      8
#       define R300_ALU_RGB_OP_FRC                      9
#       define R300_ALU_RGB_OP_SOP                      10
#       define R300_ALU_RGB_OMOD(x)                     (x << 27)
#       define R300_ALU_RGB_OMOD_NONE                   0
#       define R300_ALU_RGB_OMOD_MUL_2                  1
#       define R300_ALU_RGB_OMOD_MUL_4                  2
#       define R300_ALU_RGB_OMOD_MUL_8                  3
#       define R300_ALU_RGB_OMOD_DIV_2                  4
#       define R300_ALU_RGB_OMOD_DIV_4                  5
#       define R300_ALU_RGB_OMOD_DIV_8                  6
#       define R300_ALU_RGB_CLAMP                       (1 << 30)
#       define R300_ALU_RGB_INSERT_NOP                  (1 << 31)

#define R300_US_ALU_ALPHA_INST_0		        0x49c0
#define R300_US_ALU_ALPHA_INST_1		        0x49c4
#define R300_US_ALU_ALPHA_INST_2		        0x49c8
#       define R300_ALU_ALPHA_SEL_A(x)                  (x << 0)
#       define R300_ALU_ALPHA_SRC0_R                    0
#       define R300_ALU_ALPHA_SRC0_G                    1
#       define R300_ALU_ALPHA_SRC0_B                    2
#       define R300_ALU_ALPHA_SRC1_R                    3
#       define R300_ALU_ALPHA_SRC1_G                    4
#       define R300_ALU_ALPHA_SRC1_B                    5
#       define R300_ALU_ALPHA_SRC2_R                    6
#       define R300_ALU_ALPHA_SRC2_G                    7
#       define R300_ALU_ALPHA_SRC2_B                    8
#       define R300_ALU_ALPHA_SRC0_A                    9
#       define R300_ALU_ALPHA_SRC1_A                    10
#       define R300_ALU_ALPHA_SRC2_A                    11
#       define R300_ALU_ALPHA_SRCP_R                    12
#       define R300_ALU_ALPHA_SRCP_G                    13
#       define R300_ALU_ALPHA_SRCP_B                    14
#       define R300_ALU_ALPHA_SRCP_A                    15
#       define R300_ALU_ALPHA_0_0                       16
#       define R300_ALU_ALPHA_1_0                       17
#       define R300_ALU_ALPHA_0_5                       18
#       define R300_ALU_ALPHA_MOD_A(x)                  (x << 5)
#       define R300_ALU_ALPHA_MOD_NOP                   0
#       define R300_ALU_ALPHA_MOD_NEG                   1
#       define R300_ALU_ALPHA_MOD_ABS                   2
#       define R300_ALU_ALPHA_MOD_NAB                   3
#       define R300_ALU_ALPHA_SEL_B(x)                  (x << 7)
#       define R300_ALU_ALPHA_MOD_B(x)                  (x << 12)
#       define R300_ALU_ALPHA_SEL_C(x)                  (x << 14)
#       define R300_ALU_ALPHA_MOD_C(x)                  (x << 19)
#       define R300_ALU_ALPHA_SRCP_OP(x)                (x << 21)
#       define R300_ALU_ALPHA_SRCP_OP_1_MINUS_2RGB0	0
#       define R300_ALU_ALPHA_SRCP_OP_RGB1_MINUS_RGB0	1
#       define R300_ALU_ALPHA_SRCP_OP_RGB1_PLUS_RGB0	2
#       define R300_ALU_ALPHA_SRCP_OP_1_MINUS_RGB0	3
#       define R300_ALU_ALPHA_OP(x)                     (x << 23)
#       define R300_ALU_ALPHA_OP_MAD                    0
#       define R300_ALU_ALPHA_OP_DP                     1
#       define R300_ALU_ALPHA_OP_MIN                    2
#       define R300_ALU_ALPHA_OP_MAX                    3
#       define R300_ALU_ALPHA_OP_CND                    5
#       define R300_ALU_ALPHA_OP_CMP                    6
#       define R300_ALU_ALPHA_OP_FRC                    7
#       define R300_ALU_ALPHA_OP_EX2                    8
#       define R300_ALU_ALPHA_OP_LN2                    9
#       define R300_ALU_ALPHA_OP_RCP                    10
#       define R300_ALU_ALPHA_OP_RSQ                    11
#       define R300_ALU_ALPHA_OMOD(x)                   (x << 27)
#       define R300_ALU_ALPHA_OMOD_NONE                 0
#       define R300_ALU_ALPHA_OMOD_MUL_2                1
#       define R300_ALU_ALPHA_OMOD_MUL_4                2
#       define R300_ALU_ALPHA_OMOD_MUL_8                3
#       define R300_ALU_ALPHA_OMOD_DIV_2                4
#       define R300_ALU_ALPHA_OMOD_DIV_4                5
#       define R300_ALU_ALPHA_OMOD_DIV_8                6
#       define R300_ALU_ALPHA_CLAMP                     (1 << 30)

#define R300_FG_FOG_BLEND				0x4bc0
#define R300_FG_ALPHA_FUNC				0x4bd4
#define R300_FG_DEPTH_SRC				0x4bd8

#define R300_RB3D_CCTL				        0x4e00
#define R300_RB3D_BLENDCNTL				0x4e04
#       define R300_ALPHA_BLEND_ENABLE                  (1 << 0)
#       define R300_SEPARATE_ALPHA_ENABLE               (1 << 1)
#       define R300_READ_ENABLE                         (1 << 2)
#define R300_RB3D_ABLENDCNTL			        0x4e08
#define R300_RB3D_COLOR_CHANNEL_MASK	                0x4e0c
#       define R300_BLUE_MASK_EN                        (1 << 0)
#       define R300_GREEN_MASK_EN                       (1 << 1)
#       define R300_RED_MASK_EN                         (1 << 2)
#       define R300_ALPHA_MASK_EN                       (1 << 3)

#define R300_RB3D_COLOR_CLEAR_VALUE                     0x4e14
#define R300_RB3D_ROPCNTL				0x4e18

#define R300_RB3D_COLOROFFSET0			        0x4e28
#define R300_RB3D_COLORPITCH0			        0x4e38
#       define R300_COLORTILE                           (1 << 16)
#       define R300_COLORENDIAN_WORD                    (1 << 19)
#       define R300_COLORENDIAN_DWORD                   (2 << 19)
#       define R300_COLORENDIAN_HALF_DWORD              (3 << 19)
#       define R300_COLORFORMAT_ARGB1555                (3 << 21)
#       define R300_COLORFORMAT_RGB565                  (4 << 21)
#       define R300_COLORFORMAT_ARGB8888                (6 << 21)
#       define R300_COLORFORMAT_ARGB32323232            (7 << 21)
#       define R300_COLORFORMAT_I8                      (9 << 21)
#       define R300_COLORFORMAT_ARGB16161616            (10 << 21)
#       define R300_COLORFORMAT_VYUY                    (11 << 21)
#       define R300_COLORFORMAT_YVYU                    (12 << 21)
#       define R300_COLORFORMAT_UV88                    (13 << 21)
#       define R300_COLORFORMAT_ARGB4444                (15 << 21)

#define R300_RB3D_DSTCACHE_CTLSTAT		        0x4e4c
#       define R300_DC_FLUSH_3D                         (2 << 0)
#       define R300_DC_FREE_3D                          (2 << 2)
#       define R300_RB3D_DC_FLUSH_ALL                   (R300_DC_FLUSH_3D | R300_DC_FREE_3D)
#       define R300_DC_FINISH_3D                        (1 << 4)

#define R300_RB3D_DITHER_CTL			        0x4e50

#define R300_RB3D_AARESOLVE_CTL			        0x4e88

#define R300_RB3D_ZCNTL				        0x4f00
#define R300_RB3D_ZSTENCILCNTL			        0x4f04

#define R300_RB3D_ZTOP				        0x4f14
#define R300_RB3D_ZCACHE_CTLSTAT			0x4f18
#       define R300_ZC_FLUSH                            (1 << 0)
#       define R300_ZC_FREE                             (1 << 1)
#       define R300_ZC_FLUSH_ALL                        0x3
#define R300_RB3D_BW_CNTL				0x4f1c

#define R500_US_ALU_RGB_ADDR_0				0x9000
#   define R500_RGB_ADDR0(x)				(x << 0)
#   define R500_RGB_ADDR0_CONST				(1 << 8)
#   define R500_RGB_ADDR0_REL				(1 << 9)
#   define R500_RGB_ADDR1(x)				(x << 10)
#   define R500_RGB_ADDR1_CONST				(1 << 18)
#   define R500_RGB_ADDR1_REL				(1 << 19)
#   define R500_RGB_ADDR2(x)				(x << 20)
#   define R500_RGB_ADDR2_CONST				(1 << 28)
#   define R500_RGB_ADDR2_REL				(1 << 29)
#   define R500_RGB_SRCP_OP_1_MINUS_2RGB0		(0 << 30)
#   define R500_RGB_SRCP_OP_RGB1_MINUS_RGB0		(1 << 30)
#   define R500_RGB_SRCP_OP_RGB1_PLUS_RGB0		(2 << 30)
#   define R500_RGB_SRCP_OP_1_MINUS_RGB0		(3 << 30)

#define R500_US_TEX_INST_0				0x9000
#   define R500_TEX_ID(x)				(x << 16)
#   define R500_TEX_INST_NOP				(0 << 22)
#   define R500_TEX_INST_LD				(1 << 22)
#   define R500_TEX_INST_TEXKILL			(2 << 22)
#   define R500_TEX_INST_PROJ				(3 << 22)
#   define R500_TEX_INST_LODBIAS			(4 << 22)
#   define R500_TEX_INST_LOD				(5 << 22)
#   define R500_TEX_INST_DXDY				(6 << 22)
#   define R500_TEX_SEM_ACQUIRE				(1 << 25)
#   define R500_TEX_IGNORE_UNCOVERED			(1 << 26)
#   define R500_TEX_UNSCALED				(1 << 27)

#define R500_US_ALU_ALPHA_ADDR_0			0x9800
#   define R500_ALPHA_ADDR0(x)				(x << 0)
#   define R500_ALPHA_ADDR0_CONST			(1 << 8)
#   define R500_ALPHA_ADDR0_REL				(1 << 9)
#   define R500_ALPHA_ADDR1(x)				(x << 10)
#   define R500_ALPHA_ADDR1_CONST			(1 << 18)
#   define R500_ALPHA_ADDR1_REL				(1 << 19)
#   define R500_ALPHA_ADDR2(x)				(x << 20)
#   define R500_ALPHA_ADDR2_CONST			(1 << 28)
#   define R500_ALPHA_ADDR2_REL				(1 << 29)
#   define R500_ALPHA_SRCP_OP_1_MINUS_2A0		(0 << 30)
#   define R500_ALPHA_SRCP_OP_A1_MINUS_A0		(1 << 30)
#   define R500_ALPHA_SRCP_OP_A1_PLUS_A0		(2 << 30)
#   define R500_ALPHA_SRCP_OP_1_MINUS_A0		(3 << 30)

#define R500_US_TEX_ADDR_0				0x9800
#   define R500_TEX_SRC_ADDR(x)				(x << 0)
#   define R500_TEX_SRC_ADDR_REL			(1 << 7)
#   define R500_TEX_SRC_S_SWIZ_R			(0 << 8)
#   define R500_TEX_SRC_S_SWIZ_G			(1 << 8)
#   define R500_TEX_SRC_S_SWIZ_B			(2 << 8)
#   define R500_TEX_SRC_S_SWIZ_A			(3 << 8)
#   define R500_TEX_SRC_T_SWIZ_R			(0 << 10)
#   define R500_TEX_SRC_T_SWIZ_G			(1 << 10)
#   define R500_TEX_SRC_T_SWIZ_B			(2 << 10)
#   define R500_TEX_SRC_T_SWIZ_A			(3 << 10)
#   define R500_TEX_SRC_R_SWIZ_R			(0 << 12)
#   define R500_TEX_SRC_R_SWIZ_G			(1 << 12)
#   define R500_TEX_SRC_R_SWIZ_B			(2 << 12)
#   define R500_TEX_SRC_R_SWIZ_A			(3 << 12)
#   define R500_TEX_SRC_Q_SWIZ_R			(0 << 14)
#   define R500_TEX_SRC_Q_SWIZ_G			(1 << 14)
#   define R500_TEX_SRC_Q_SWIZ_B			(2 << 14)
#   define R500_TEX_SRC_Q_SWIZ_A			(3 << 14)
#   define R500_TEX_DST_ADDR(x)				(x << 16)
#   define R500_TEX_DST_ADDR_REL			(1 << 23)
#   define R500_TEX_DST_R_SWIZ_R			(0 << 24)
#   define R500_TEX_DST_R_SWIZ_G			(1 << 24)
#   define R500_TEX_DST_R_SWIZ_B			(2 << 24)
#   define R500_TEX_DST_R_SWIZ_A			(3 << 24)
#   define R500_TEX_DST_G_SWIZ_R			(0 << 26)
#   define R500_TEX_DST_G_SWIZ_G			(1 << 26)
#   define R500_TEX_DST_G_SWIZ_B			(2 << 26)
#   define R500_TEX_DST_G_SWIZ_A			(3 << 26)
#   define R500_TEX_DST_B_SWIZ_R			(0 << 28)
#   define R500_TEX_DST_B_SWIZ_G			(1 << 28)
#   define R500_TEX_DST_B_SWIZ_B			(2 << 28)
#   define R500_TEX_DST_B_SWIZ_A			(3 << 28)
#   define R500_TEX_DST_A_SWIZ_R			(0 << 30)
#   define R500_TEX_DST_A_SWIZ_G			(1 << 30)
#   define R500_TEX_DST_A_SWIZ_B			(2 << 30)
#   define R500_TEX_DST_A_SWIZ_A			(3 << 30)

#define R500_US_ALU_RGB_INST_0				0xa000
#   define R500_ALU_RGB_SEL_A_SRC0			(0 << 0)
#   define R500_ALU_RGB_SEL_A_SRC1			(1 << 0)
#   define R500_ALU_RGB_SEL_A_SRC2			(2 << 0)
#   define R500_ALU_RGB_SEL_A_SRCP			(3 << 0)
#   define R500_ALU_RGB_R_SWIZ_A_R			(0 << 2)
#   define R500_ALU_RGB_R_SWIZ_A_G			(1 << 2)
#   define R500_ALU_RGB_R_SWIZ_A_B			(2 << 2)
#   define R500_ALU_RGB_R_SWIZ_A_A			(3 << 2)
#   define R500_ALU_RGB_R_SWIZ_A_0			(4 << 2)
#   define R500_ALU_RGB_R_SWIZ_A_HALF			(5 << 2)
#   define R500_ALU_RGB_R_SWIZ_A_1			(6 << 2)
/* #define R500_ALU_RGB_R_SWIZ_A_UNUSED			(7 << 2) */
#   define R500_ALU_RGB_G_SWIZ_A_R			(0 << 5)
#   define R500_ALU_RGB_G_SWIZ_A_G			(1 << 5)
#   define R500_ALU_RGB_G_SWIZ_A_B			(2 << 5)
#   define R500_ALU_RGB_G_SWIZ_A_A			(3 << 5)
#   define R500_ALU_RGB_G_SWIZ_A_0			(4 << 5)
#   define R500_ALU_RGB_G_SWIZ_A_HALF			(5 << 5)
#   define R500_ALU_RGB_G_SWIZ_A_1			(6 << 5)
/* #define R500_ALU_RGB_G_SWIZ_A_UNUSED			(7 << 5) */
#   define R500_ALU_RGB_B_SWIZ_A_R			(0 << 8)
#   define R500_ALU_RGB_B_SWIZ_A_G			(1 << 8)
#   define R500_ALU_RGB_B_SWIZ_A_B			(2 << 8)
#   define R500_ALU_RGB_B_SWIZ_A_A			(3 << 8)
#   define R500_ALU_RGB_B_SWIZ_A_0			(4 << 8)
#   define R500_ALU_RGB_B_SWIZ_A_HALF			(5 << 8)
#   define R500_ALU_RGB_B_SWIZ_A_1			(6 << 8)
/* #define R500_ALU_RGB_B_SWIZ_A_UNUSED			(7 << 8) */
#   define R500_ALU_RGB_MOD_A_NOP			(0 << 11)
#   define R500_ALU_RGB_MOD_A_NEG			(1 << 11)
#   define R500_ALU_RGB_MOD_A_ABS			(2 << 11)
#   define R500_ALU_RGB_MOD_A_NAB			(3 << 11)
#   define R500_ALU_RGB_SEL_B_SRC0			(0 << 13)
#   define R500_ALU_RGB_SEL_B_SRC1			(1 << 13)
#   define R500_ALU_RGB_SEL_B_SRC2			(2 << 13)
#   define R500_ALU_RGB_SEL_B_SRCP			(3 << 13)
#   define R500_ALU_RGB_R_SWIZ_B_R			(0 << 15)
#   define R500_ALU_RGB_R_SWIZ_B_G			(1 << 15)
#   define R500_ALU_RGB_R_SWIZ_B_B			(2 << 15)
#   define R500_ALU_RGB_R_SWIZ_B_A			(3 << 15)
#   define R500_ALU_RGB_R_SWIZ_B_0			(4 << 15)
#   define R500_ALU_RGB_R_SWIZ_B_HALF			(5 << 15)
#   define R500_ALU_RGB_R_SWIZ_B_1			(6 << 15)
/* #define R500_ALU_RGB_R_SWIZ_B_UNUSED			(7 << 15) */
#   define R500_ALU_RGB_G_SWIZ_B_R			(0 << 18)
#   define R500_ALU_RGB_G_SWIZ_B_G			(1 << 18)
#   define R500_ALU_RGB_G_SWIZ_B_B			(2 << 18)
#   define R500_ALU_RGB_G_SWIZ_B_A			(3 << 18)
#   define R500_ALU_RGB_G_SWIZ_B_0			(4 << 18)
#   define R500_ALU_RGB_G_SWIZ_B_HALF			(5 << 18)
#   define R500_ALU_RGB_G_SWIZ_B_1			(6 << 18)
/* #define R500_ALU_RGB_G_SWIZ_B_UNUSED			(7 << 18) */
#   define R500_ALU_RGB_B_SWIZ_B_R			(0 << 21)
#   define R500_ALU_RGB_B_SWIZ_B_G			(1 << 21)
#   define R500_ALU_RGB_B_SWIZ_B_B			(2 << 21)
#   define R500_ALU_RGB_B_SWIZ_B_A			(3 << 21)
#   define R500_ALU_RGB_B_SWIZ_B_0			(4 << 21)
#   define R500_ALU_RGB_B_SWIZ_B_HALF			(5 << 21)
#   define R500_ALU_RGB_B_SWIZ_B_1			(6 << 21)
/* #define R500_ALU_RGB_B_SWIZ_B_UNUSED			(7 << 21) */
#   define R500_ALU_RGB_MOD_B_NOP			(0 << 24)
#   define R500_ALU_RGB_MOD_B_NEG			(1 << 24)
#   define R500_ALU_RGB_MOD_B_ABS			(2 << 24)
#   define R500_ALU_RGB_MOD_B_NAB			(3 << 24)
#   define R500_ALU_RGB_OMOD_IDENTITY			(0 << 26)
#   define R500_ALU_RGB_OMOD_MUL_2			(1 << 26)
#   define R500_ALU_RGB_OMOD_MUL_4			(2 << 26)
#   define R500_ALU_RGB_OMOD_MUL_8			(3 << 26)
#   define R500_ALU_RGB_OMOD_DIV_2			(4 << 26)
#   define R500_ALU_RGB_OMOD_DIV_4			(5 << 26)
#   define R500_ALU_RGB_OMOD_DIV_8			(6 << 26)
#   define R500_ALU_RGB_OMOD_DISABLE			(7 << 26)
#   define R500_ALU_RGB_TARGET(x)			(x << 29)
#   define R500_ALU_RGB_WMASK				(1 << 31)

#define R500_US_TEX_ADDR_DXDY_0				0xa000
#   define R500_DX_ADDR(x)				(x << 0)
#   define R500_DX_ADDR_REL				(1 << 7)
#   define R500_DX_S_SWIZ_R				(0 << 8)
#   define R500_DX_S_SWIZ_G				(1 << 8)
#   define R500_DX_S_SWIZ_B				(2 << 8)
#   define R500_DX_S_SWIZ_A				(3 << 8)
#   define R500_DX_T_SWIZ_R				(0 << 10)
#   define R500_DX_T_SWIZ_G				(1 << 10)
#   define R500_DX_T_SWIZ_B				(2 << 10)
#   define R500_DX_T_SWIZ_A				(3 << 10)
#   define R500_DX_R_SWIZ_R				(0 << 12)
#   define R500_DX_R_SWIZ_G				(1 << 12)
#   define R500_DX_R_SWIZ_B				(2 << 12)
#   define R500_DX_R_SWIZ_A				(3 << 12)
#   define R500_DX_Q_SWIZ_R				(0 << 14)
#   define R500_DX_Q_SWIZ_G				(1 << 14)
#   define R500_DX_Q_SWIZ_B				(2 << 14)
#   define R500_DX_Q_SWIZ_A				(3 << 14)
#   define R500_DY_ADDR(x)				(x << 16)
#   define R500_DY_ADDR_REL				(1 << 17)
#   define R500_DY_S_SWIZ_R				(0 << 24)
#   define R500_DY_S_SWIZ_G				(1 << 24)
#   define R500_DY_S_SWIZ_B				(2 << 24)
#   define R500_DY_S_SWIZ_A				(3 << 24)
#   define R500_DY_T_SWIZ_R				(0 << 26)
#   define R500_DY_T_SWIZ_G				(1 << 26)
#   define R500_DY_T_SWIZ_B				(2 << 26)
#   define R500_DY_T_SWIZ_A				(3 << 26)
#   define R500_DY_R_SWIZ_R				(0 << 28)
#   define R500_DY_R_SWIZ_G				(1 << 28)
#   define R500_DY_R_SWIZ_B				(2 << 28)
#   define R500_DY_R_SWIZ_A				(3 << 28)
#   define R500_DY_Q_SWIZ_R				(0 << 30)
#   define R500_DY_Q_SWIZ_G				(1 << 30)
#   define R500_DY_Q_SWIZ_B				(2 << 30)
#   define R500_DY_Q_SWIZ_A				(3 << 30)

/*
 * The R500 unified shader (US) registers come in banks of 512 each, one
 * for each instruction slot in the shader.  You can't touch them directly.
 * R500_US_VECTOR_INDEX() sets the base instruction to modify; successive
 * writes to R500_GA_US_VECTOR_DATA autoincrement the index after the
 * instruction is fully specified.
 */
#define R500_US_ALU_ALPHA_INST_0			0xa800
#   define R500_ALPHA_OP_MAD				0
#   define R500_ALPHA_OP_DP				1
#   define R500_ALPHA_OP_MIN				2
#   define R500_ALPHA_OP_MAX				3
/* #define R500_ALPHA_OP_RESERVED			4 */
#   define R500_ALPHA_OP_CND				5
#   define R500_ALPHA_OP_CMP				6
#   define R500_ALPHA_OP_FRC				7
#   define R500_ALPHA_OP_EX2				8
#   define R500_ALPHA_OP_LN2				9
#   define R500_ALPHA_OP_RCP				10
#   define R500_ALPHA_OP_RSQ				11
#   define R500_ALPHA_OP_SIN				12
#   define R500_ALPHA_OP_COS				13
#   define R500_ALPHA_OP_MDH				14
#   define R500_ALPHA_OP_MDV				15
#   define R500_ALPHA_ADDRD(x)				(x << 4)
#   define R500_ALPHA_ADDRD_REL				(1 << 11)
#   define R500_ALPHA_SEL_A_SRC0			(0 << 12)
#   define R500_ALPHA_SEL_A_SRC1			(1 << 12)
#   define R500_ALPHA_SEL_A_SRC2			(2 << 12)
#   define R500_ALPHA_SEL_A_SRCP			(3 << 12)
#   define R500_ALPHA_SWIZ_A_R				(0 << 14)
#   define R500_ALPHA_SWIZ_A_G				(1 << 14)
#   define R500_ALPHA_SWIZ_A_B				(2 << 14)
#   define R500_ALPHA_SWIZ_A_A				(3 << 14)
#   define R500_ALPHA_SWIZ_A_0				(4 << 14)
#   define R500_ALPHA_SWIZ_A_HALF			(5 << 14)
#   define R500_ALPHA_SWIZ_A_1				(6 << 14)
/* #define R500_ALPHA_SWIZ_A_UNUSED			(7 << 14) */
#   define R500_ALPHA_MOD_A_NOP				(0 << 17)
#   define R500_ALPHA_MOD_A_NEG				(1 << 17)
#   define R500_ALPHA_MOD_A_ABS				(2 << 17)
#   define R500_ALPHA_MOD_A_NAB				(3 << 17)
#   define R500_ALPHA_SEL_B_SRC0			(0 << 19)
#   define R500_ALPHA_SEL_B_SRC1			(1 << 19)
#   define R500_ALPHA_SEL_B_SRC2			(2 << 19)
#   define R500_ALPHA_SEL_B_SRCP			(3 << 19)
#   define R500_ALPHA_SWIZ_B_R				(0 << 21)
#   define R500_ALPHA_SWIZ_B_G				(1 << 21)
#   define R500_ALPHA_SWIZ_B_B				(2 << 21)
#   define R500_ALPHA_SWIZ_B_A				(3 << 21)
#   define R500_ALPHA_SWIZ_B_0				(4 << 21)
#   define R500_ALPHA_SWIZ_B_HALF			(5 << 21)
#   define R500_ALPHA_SWIZ_B_1				(6 << 21)
/* #define R500_ALPHA_SWIZ_B_UNUSED			(7 << 21) */
#   define R500_ALPHA_MOD_B_NOP				(0 << 24)
#   define R500_ALPHA_MOD_B_NEG				(1 << 24)
#   define R500_ALPHA_MOD_B_ABS				(2 << 24)
#   define R500_ALPHA_MOD_B_NAB				(3 << 24)
#   define R500_ALPHA_OMOD_IDENTITY			(0 << 26)
#   define R500_ALPHA_OMOD_MUL_2			(1 << 26)
#   define R500_ALPHA_OMOD_MUL_4			(2 << 26)
#   define R500_ALPHA_OMOD_MUL_8			(3 << 26)
#   define R500_ALPHA_OMOD_DIV_2			(4 << 26)
#   define R500_ALPHA_OMOD_DIV_4			(5 << 26)
#   define R500_ALPHA_OMOD_DIV_8			(6 << 26)
#   define R500_ALPHA_OMOD_DISABLE			(7 << 26)
#   define R500_ALPHA_TARGET(x)				(x << 29)
#   define R500_ALPHA_W_OMASK				(1 << 31)

#define R500_US_ALU_RGBA_INST_0				0xb000
#   define R500_ALU_RGBA_OP_MAD				(0 << 0)
#   define R500_ALU_RGBA_OP_DP3				(1 << 0)
#   define R500_ALU_RGBA_OP_DP4				(2 << 0)
#   define R500_ALU_RGBA_OP_D2A				(3 << 0)
#   define R500_ALU_RGBA_OP_MIN				(4 << 0)
#   define R500_ALU_RGBA_OP_MAX				(5 << 0)
/* #define R500_ALU_RGBA_OP_RESERVED			(6 << 0) */
#   define R500_ALU_RGBA_OP_CND				(7 << 0)
#   define R500_ALU_RGBA_OP_CMP				(8 << 0)
#   define R500_ALU_RGBA_OP_FRC				(9 << 0)
#   define R500_ALU_RGBA_OP_SOP				(10 << 0)
#   define R500_ALU_RGBA_OP_MDH				(11 << 0)
#   define R500_ALU_RGBA_OP_MDV				(12 << 0)
#   define R500_ALU_RGBA_ADDRD(x)			(x << 4)
#   define R500_ALU_RGBA_ADDRD_REL			(1 << 11)
#   define R500_ALU_RGBA_SEL_C_SRC0			(0 << 12)
#   define R500_ALU_RGBA_SEL_C_SRC1			(1 << 12)
#   define R500_ALU_RGBA_SEL_C_SRC2			(2 << 12)
#   define R500_ALU_RGBA_SEL_C_SRCP			(3 << 12)
#   define R500_ALU_RGBA_R_SWIZ_R			(0 << 14)
#   define R500_ALU_RGBA_R_SWIZ_G			(1 << 14)
#   define R500_ALU_RGBA_R_SWIZ_B			(2 << 14)
#   define R500_ALU_RGBA_R_SWIZ_A			(3 << 14)
#   define R500_ALU_RGBA_R_SWIZ_0			(4 << 14)
#   define R500_ALU_RGBA_R_SWIZ_HALF			(5 << 14)
#   define R500_ALU_RGBA_R_SWIZ_1			(6 << 14)
/* #define R500_ALU_RGBA_R_SWIZ_UNUSED			(7 << 14) */
#   define R500_ALU_RGBA_G_SWIZ_R			(0 << 17)
#   define R500_ALU_RGBA_G_SWIZ_G			(1 << 17)
#   define R500_ALU_RGBA_G_SWIZ_B			(2 << 17)
#   define R500_ALU_RGBA_G_SWIZ_A			(3 << 17)
#   define R500_ALU_RGBA_G_SWIZ_0			(4 << 17)
#   define R500_ALU_RGBA_G_SWIZ_HALF			(5 << 17)
#   define R500_ALU_RGBA_G_SWIZ_1			(6 << 17)
/* #define R500_ALU_RGBA_G_SWIZ_UNUSED			(7 << 17) */
#   define R500_ALU_RGBA_B_SWIZ_R			(0 << 20)
#   define R500_ALU_RGBA_B_SWIZ_G			(1 << 20)
#   define R500_ALU_RGBA_B_SWIZ_B			(2 << 20)
#   define R500_ALU_RGBA_B_SWIZ_A			(3 << 20)
#   define R500_ALU_RGBA_B_SWIZ_0			(4 << 20)
#   define R500_ALU_RGBA_B_SWIZ_HALF			(5 << 20)
#   define R500_ALU_RGBA_B_SWIZ_1			(6 << 20)
/* #define R500_ALU_RGBA_B_SWIZ_UNUSED			(7 << 20) */
#   define R500_ALU_RGBA_MOD_C_NOP			(0 << 23)
#   define R500_ALU_RGBA_MOD_C_NEG			(1 << 23)
#   define R500_ALU_RGBA_MOD_C_ABS			(2 << 23)
#   define R500_ALU_RGBA_MOD_C_NAB			(3 << 23)
#   define R500_ALU_RGBA_ALPHA_SEL_C_SRC0		(0 << 25)
#   define R500_ALU_RGBA_ALPHA_SEL_C_SRC1		(1 << 25)
#   define R500_ALU_RGBA_ALPHA_SEL_C_SRC2		(2 << 25)
#   define R500_ALU_RGBA_ALPHA_SEL_C_SRCP		(3 << 25)
#   define R500_ALU_RGBA_A_SWIZ_R			(0 << 27)
#   define R500_ALU_RGBA_A_SWIZ_G			(1 << 27)
#   define R500_ALU_RGBA_A_SWIZ_B			(2 << 27)
#   define R500_ALU_RGBA_A_SWIZ_A			(3 << 27)
#   define R500_ALU_RGBA_A_SWIZ_0			(4 << 27)
#   define R500_ALU_RGBA_A_SWIZ_HALF			(5 << 27)
#   define R500_ALU_RGBA_A_SWIZ_1			(6 << 27)
/* #define R500_ALU_RGBA_A_SWIZ_UNUSED			(7 << 27) */
#   define R500_ALU_RGBA_ALPHA_MOD_C_NOP		(0 << 30)
#   define R500_ALU_RGBA_ALPHA_MOD_C_NEG		(1 << 30)
#   define R500_ALU_RGBA_ALPHA_MOD_C_ABS		(2 << 30)
#   define R500_ALU_RGBA_ALPHA_MOD_C_NAB		(3 << 30)

#define R500_US_CMN_INST_0				0xb800
#   define R500_INST_TYPE_ALU				(0 << 0)
#   define R500_INST_TYPE_OUT				(1 << 0)
#   define R500_INST_TYPE_FC				(2 << 0)
#   define R500_INST_TYPE_TEX				(3 << 0)
#   define R500_INST_TEX_SEM_WAIT			(1 << 2)
#   define R500_INST_RGB_PRED_SEL_NONE			(0 << 3)
#   define R500_INST_RGB_PRED_SEL_RGBA			(1 << 3)
#   define R500_INST_RGB_PRED_SEL_RRRR			(2 << 3)
#   define R500_INST_RGB_PRED_SEL_GGGG			(3 << 3)
#   define R500_INST_RGB_PRED_SEL_BBBB			(4 << 3)
#   define R500_INST_RGB_PRED_SEL_AAAA			(5 << 3)
#   define R500_INST_RGB_PRED_INV			(1 << 6)
#   define R500_INST_WRITE_INACTIVE			(1 << 7)
#   define R500_INST_LAST				(1 << 8)
#   define R500_INST_NOP				(1 << 9)
#   define R500_INST_ALU_WAIT				(1 << 10)
#   define R500_INST_RGB_WMASK_R			(1 << 11)
#   define R500_INST_RGB_WMASK_G			(1 << 12)
#   define R500_INST_RGB_WMASK_B			(1 << 13)
#   define R500_INST_ALPHA_WMASK			(1 << 14)
#   define R500_INST_RGB_OMASK_R			(1 << 15)
#   define R500_INST_RGB_OMASK_G			(1 << 16)
#   define R500_INST_RGB_OMASK_B			(1 << 17)
#   define R500_INST_ALPHA_OMASK			(1 << 18)
#   define R500_INST_RGB_CLAMP				(1 << 19)
#   define R500_INST_ALPHA_CLAMP			(1 << 20)
#   define R500_INST_ALU_RESULT_SEL			(1 << 21)
#   define R500_INST_ALPHA_PRED_INV			(1 << 22)
#   define R500_INST_ALU_RESULT_OP_EQ			(0 << 23)
#   define R500_INST_ALU_RESULT_OP_LT			(1 << 23)
#   define R500_INST_ALU_RESULT_OP_GE			(2 << 23)
#   define R500_INST_ALU_RESULT_OP_NE			(3 << 23)
#   define R500_INST_ALPHA_PRED_SEL_NONE		(0 << 25)
#   define R500_INST_ALPHA_PRED_SEL_RGBA		(1 << 25)
#   define R500_INST_ALPHA_PRED_SEL_RRRR		(2 << 25)
#   define R500_INST_ALPHA_PRED_SEL_GGGG		(3 << 25)
#   define R500_INST_ALPHA_PRED_SEL_BBBB		(4 << 25)
#   define R500_INST_ALPHA_PRED_SEL_AAAA		(5 << 25)
/* XXX next four are kind of guessed */
#   define R500_INST_STAT_WE_R				(1 << 28)
#   define R500_INST_STAT_WE_G				(1 << 29)
#   define R500_INST_STAT_WE_B				(1 << 30)
#   define R500_INST_STAT_WE_A				(1 << 31)

#endif /* HAVE_R5XX_3DREGS_H */
