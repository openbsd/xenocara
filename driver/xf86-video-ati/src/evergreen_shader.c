/*
 * Copyright 2010 Advanced Micro Devices, Inc.
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
 * Author: Alex Deucher <alexander.deucher@amd.com>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef XF86DRM_MODE

#include "xf86.h"

#include "evergreen_shader.h"
#include "evergreen_reg.h"

/* solid vs --------------------------------------- */
int evergreen_solid_vs(RADEONChipFamily ChipSet, uint32_t* shader)
{
    int i = 0;

    /* 0 */
    shader[i++] = CF_DWORD0(ADDR(4),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
			    CF_CONST(0),
			    COND(SQ_CF_COND_ACTIVE),
			    I_COUNT(1),
			    VALID_PIXEL_MODE(0),
			    END_OF_PROGRAM(0),
			    CF_INST(SQ_CF_INST_VC),
			    WHOLE_QUAD_MODE(0),
			    BARRIER(1));
    /* 1 */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(CF_POS0),
					  TYPE(SQ_EXPORT_POS),
					  RW_GPR(1),
					  RW_REL(ABSOLUTE),
					  INDEX_GPR(0),
					  ELEM_SIZE(0));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
					       SRC_SEL_Y(SQ_SEL_Y),
					       SRC_SEL_Z(SQ_SEL_Z),
					       SRC_SEL_W(SQ_SEL_W),
					       BURST_COUNT(1),
					       VALID_PIXEL_MODE(0),
					       END_OF_PROGRAM(0),
					       CF_INST(SQ_CF_INST_EXPORT_DONE),
					       MARK(0),
					       BARRIER(1));
    /* 2 - always export a param whether it's used or not */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(0),
					  TYPE(SQ_EXPORT_PARAM),
					  RW_GPR(0),
					  RW_REL(ABSOLUTE),
					  INDEX_GPR(0),
					  ELEM_SIZE(0));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
					       SRC_SEL_Y(SQ_SEL_Y),
					       SRC_SEL_Z(SQ_SEL_Z),
					       SRC_SEL_W(SQ_SEL_W),
					       BURST_COUNT(0),
					       VALID_PIXEL_MODE(0),
					       END_OF_PROGRAM(1),
					       CF_INST(SQ_CF_INST_EXPORT_DONE),
					       MARK(0),
					       BARRIER(0));
    /* 3 - padding */
    shader[i++] = 0x00000000;
    shader[i++] = 0x00000000;
    /* 4/5 */
    shader[i++] = VTX_DWORD0(VTX_INST(SQ_VTX_INST_FETCH),
			     FETCH_TYPE(SQ_VTX_FETCH_VERTEX_DATA),
			     FETCH_WHOLE_QUAD(0),
			     BUFFER_ID(0),
			     SRC_GPR(0),
			     SRC_REL(ABSOLUTE),
			     SRC_SEL_X(SQ_SEL_X),
			     MEGA_FETCH_COUNT(8));
    shader[i++] = VTX_DWORD1_GPR(DST_GPR(1),
				 DST_REL(0),
				 DST_SEL_X(SQ_SEL_X),
				 DST_SEL_Y(SQ_SEL_Y),
				 DST_SEL_Z(SQ_SEL_0),
				 DST_SEL_W(SQ_SEL_1),
				 USE_CONST_FIELDS(0),
				 DATA_FORMAT(FMT_32_32_FLOAT),
				 NUM_FORMAT_ALL(SQ_NUM_FORMAT_SCALED),
				 FORMAT_COMP_ALL(SQ_FORMAT_COMP_SIGNED),
				 SRF_MODE_ALL(SRF_MODE_ZERO_CLAMP_MINUS_ONE));
    shader[i++] = VTX_DWORD2(OFFSET(0),
#if X_BYTE_ORDER == X_BIG_ENDIAN
			     ENDIAN_SWAP(SQ_ENDIAN_8IN32),
#else
			     ENDIAN_SWAP(SQ_ENDIAN_NONE),
#endif
			     CONST_BUF_NO_STRIDE(0),
			     MEGA_FETCH(1),
			     ALT_CONST(0),
			     BUFFER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = VTX_DWORD_PAD;

    return i;
}

/* solid ps --------------------------------------- */
int evergreen_solid_ps(RADEONChipFamily ChipSet, uint32_t* shader)
{
    int i = 0;

    /* 0 */
    shader[i++] = CF_ALU_DWORD0(ADDR(2),
				KCACHE_BANK0(0),
				KCACHE_BANK1(0),
				KCACHE_MODE0(SQ_CF_KCACHE_LOCK_1));
    shader[i++] = CF_ALU_DWORD1(KCACHE_MODE1(SQ_CF_KCACHE_NOP),
				KCACHE_ADDR0(0),
				KCACHE_ADDR1(0),
				I_COUNT(4),
				ALT_CONST(0),
				CF_INST(SQ_CF_INST_ALU),
				WHOLE_QUAD_MODE(0),
				BARRIER(1));
    /* 1 */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(CF_PIXEL_MRT0),
					  TYPE(SQ_EXPORT_PIXEL),
					  RW_GPR(0),
					  RW_REL(ABSOLUTE),
					  INDEX_GPR(0),
					  ELEM_SIZE(1));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
					       SRC_SEL_Y(SQ_SEL_Y),
					       SRC_SEL_Z(SQ_SEL_Z),
					       SRC_SEL_W(SQ_SEL_W),
					       BURST_COUNT(1),
					       VALID_PIXEL_MODE(0),
					       END_OF_PROGRAM(1),
					       CF_INST(SQ_CF_INST_EXPORT_DONE),
					       MARK(0),
					       BARRIER(1));

    /* 2 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_KCACHE0_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_X),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_MOV),
				 BANK_SWIZZLE(SQ_ALU_VEC_012),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_X),
				 CLAMP(1));
    /* 3 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_KCACHE0_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Y),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_Y),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_MOV),
				 BANK_SWIZZLE(SQ_ALU_VEC_012),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Y),
				 CLAMP(1));
    /* 4 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_KCACHE0_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Z),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_Z),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_MOV),
				 BANK_SWIZZLE(SQ_ALU_VEC_012),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Z),
				 CLAMP(1));
    /* 5 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_KCACHE0_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_W),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_W),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_MOV),
				 BANK_SWIZZLE(SQ_ALU_VEC_012),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_W),
				 CLAMP(1));

    return i;
}

/* copy vs --------------------------------------- */
int evergreen_copy_vs(RADEONChipFamily ChipSet, uint32_t* shader)
{
    int i = 0;

    /* 0 */
    shader[i++] = CF_DWORD0(ADDR(4),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
			    CF_CONST(0),
			    COND(SQ_CF_COND_ACTIVE),
			    I_COUNT(2),
			    VALID_PIXEL_MODE(0),
			    END_OF_PROGRAM(0),
			    CF_INST(SQ_CF_INST_VC),
			    WHOLE_QUAD_MODE(0),
			    BARRIER(1));
    /* 1 */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(CF_POS0),
					  TYPE(SQ_EXPORT_POS),
					  RW_GPR(1),
					  RW_REL(ABSOLUTE),
					  INDEX_GPR(0),
					  ELEM_SIZE(0));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
					       SRC_SEL_Y(SQ_SEL_Y),
					       SRC_SEL_Z(SQ_SEL_Z),
					       SRC_SEL_W(SQ_SEL_W),
					       BURST_COUNT(0),
					       VALID_PIXEL_MODE(0),
					       END_OF_PROGRAM(0),
					       CF_INST(SQ_CF_INST_EXPORT_DONE),
					       MARK(0),
					       BARRIER(1));
    /* 2 */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(0),
					  TYPE(SQ_EXPORT_PARAM),
					  RW_GPR(0),
					  RW_REL(ABSOLUTE),
					  INDEX_GPR(0),
					  ELEM_SIZE(0));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
					       SRC_SEL_Y(SQ_SEL_Y),
					       SRC_SEL_Z(SQ_SEL_Z),
					       SRC_SEL_W(SQ_SEL_W),
					       BURST_COUNT(0),
					       VALID_PIXEL_MODE(0),
					       END_OF_PROGRAM(1),
					       CF_INST(SQ_CF_INST_EXPORT_DONE),
					       MARK(0),
					       BARRIER(0));
    /* 3 */
    shader[i++] = 0x00000000;
    shader[i++] = 0x00000000;
    /* 4/5 */
    shader[i++] = VTX_DWORD0(VTX_INST(SQ_VTX_INST_FETCH),
			     FETCH_TYPE(SQ_VTX_FETCH_VERTEX_DATA),
			     FETCH_WHOLE_QUAD(0),
			     BUFFER_ID(0),
			     SRC_GPR(0),
			     SRC_REL(ABSOLUTE),
			     SRC_SEL_X(SQ_SEL_X),
			     MEGA_FETCH_COUNT(16));
    shader[i++] = VTX_DWORD1_GPR(DST_GPR(1),
				 DST_REL(0),
				 DST_SEL_X(SQ_SEL_X),
				 DST_SEL_Y(SQ_SEL_Y),
				 DST_SEL_Z(SQ_SEL_0),
				 DST_SEL_W(SQ_SEL_1),
				 USE_CONST_FIELDS(0),
				 DATA_FORMAT(FMT_32_32_FLOAT),
				 NUM_FORMAT_ALL(SQ_NUM_FORMAT_SCALED),
				 FORMAT_COMP_ALL(SQ_FORMAT_COMP_SIGNED),
				 SRF_MODE_ALL(SRF_MODE_ZERO_CLAMP_MINUS_ONE));
    shader[i++] = VTX_DWORD2(OFFSET(0),
#if X_BYTE_ORDER == X_BIG_ENDIAN
                             ENDIAN_SWAP(SQ_ENDIAN_8IN32),
#else
                             ENDIAN_SWAP(SQ_ENDIAN_NONE),
#endif
			     CONST_BUF_NO_STRIDE(0),
			     MEGA_FETCH(1),
			     ALT_CONST(0),
			     BUFFER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = VTX_DWORD_PAD;
    /* 6/7 */
    shader[i++] = VTX_DWORD0(VTX_INST(SQ_VTX_INST_FETCH),
			     FETCH_TYPE(SQ_VTX_FETCH_VERTEX_DATA),
			     FETCH_WHOLE_QUAD(0),
			     BUFFER_ID(0),
			     SRC_GPR(0),
			     SRC_REL(ABSOLUTE),
			     SRC_SEL_X(SQ_SEL_X),
			     MEGA_FETCH_COUNT(8));
    shader[i++] = VTX_DWORD1_GPR(DST_GPR(0),
				 DST_REL(0),
				 DST_SEL_X(SQ_SEL_X),
				 DST_SEL_Y(SQ_SEL_Y),
				 DST_SEL_Z(SQ_SEL_0),
				 DST_SEL_W(SQ_SEL_1),
				 USE_CONST_FIELDS(0),
				 DATA_FORMAT(FMT_32_32_FLOAT),
				 NUM_FORMAT_ALL(SQ_NUM_FORMAT_SCALED),
				 FORMAT_COMP_ALL(SQ_FORMAT_COMP_SIGNED),
				 SRF_MODE_ALL(SRF_MODE_ZERO_CLAMP_MINUS_ONE));
    shader[i++] = VTX_DWORD2(OFFSET(8),
#if X_BYTE_ORDER == X_BIG_ENDIAN
                             ENDIAN_SWAP(SQ_ENDIAN_8IN32),
#else
                             ENDIAN_SWAP(SQ_ENDIAN_NONE),
#endif
			     CONST_BUF_NO_STRIDE(0),
			     MEGA_FETCH(0),
			     ALT_CONST(0),
			     BUFFER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = VTX_DWORD_PAD;

    return i;
}

/* copy ps --------------------------------------- */
int evergreen_copy_ps(RADEONChipFamily ChipSet, uint32_t* shader)
{
    int i = 0;

    /* CF INST 0 */
    shader[i++] = CF_ALU_DWORD0(ADDR(3),
				KCACHE_BANK0(0),
				KCACHE_BANK1(0),
				KCACHE_MODE0(SQ_CF_KCACHE_NOP));
    shader[i++] = CF_ALU_DWORD1(KCACHE_MODE1(SQ_CF_KCACHE_NOP),
				KCACHE_ADDR0(0),
				KCACHE_ADDR1(0),
				I_COUNT(4),
				ALT_CONST(0),
				CF_INST(SQ_CF_INST_ALU),
				WHOLE_QUAD_MODE(0),
				BARRIER(1));
    /* CF INST 1 */
    shader[i++] = CF_DWORD0(ADDR(8),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
			    CF_CONST(0),
			    COND(SQ_CF_COND_ACTIVE),
			    I_COUNT(1),
			    VALID_PIXEL_MODE(0),
			    END_OF_PROGRAM(0),
			    CF_INST(SQ_CF_INST_TC),
			    WHOLE_QUAD_MODE(0),
			    BARRIER(1));
    /* CF INST 2 */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(CF_PIXEL_MRT0),
					  TYPE(SQ_EXPORT_PIXEL),
					  RW_GPR(0),
					  RW_REL(ABSOLUTE),
					  INDEX_GPR(0),
					  ELEM_SIZE(1));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
					       SRC_SEL_Y(SQ_SEL_Y),
					       SRC_SEL_Z(SQ_SEL_Z),
					       SRC_SEL_W(SQ_SEL_W),
					       BURST_COUNT(1),
					       VALID_PIXEL_MODE(0),
					       END_OF_PROGRAM(1),
					       CF_INST(SQ_CF_INST_EXPORT_DONE),
					       MARK(0),
					       BARRIER(1));

    /* 3 interpolate tex coords */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Y),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_X),
				 CLAMP(0));
    /* 4 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_X),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Y),
				 CLAMP(0));
    /* 5 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Y),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(0),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Z),
				 CLAMP(0));
    /* 6 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_X),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(0),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_W),
				 CLAMP(0));

    /* 7 */
    shader[i++] = 0x00000000;
    shader[i++] = 0x00000000;

    /* 8/9 TEX INST 0 */
    shader[i++] = TEX_DWORD0(TEX_INST(SQ_TEX_INST_SAMPLE),
			     INST_MOD(0),
			     FETCH_WHOLE_QUAD(0),
			     RESOURCE_ID(0),
			     SRC_GPR(0),
			     SRC_REL(ABSOLUTE),
			     ALT_CONST(0),
			     RESOURCE_INDEX_MODE(SQ_CF_INDEX_NONE),
			     SAMPLER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = TEX_DWORD1(DST_GPR(0),
			     DST_REL(ABSOLUTE),
			     DST_SEL_X(SQ_SEL_X), /* R */
			     DST_SEL_Y(SQ_SEL_Y), /* G */
			     DST_SEL_Z(SQ_SEL_Z), /* B */
			     DST_SEL_W(SQ_SEL_W), /* A */
			     LOD_BIAS(0),
			     COORD_TYPE_X(TEX_UNNORMALIZED),
			     COORD_TYPE_Y(TEX_UNNORMALIZED),
			     COORD_TYPE_Z(TEX_UNNORMALIZED),
			     COORD_TYPE_W(TEX_UNNORMALIZED));
    shader[i++] = TEX_DWORD2(OFFSET_X(0),
			     OFFSET_Y(0),
			     OFFSET_Z(0),
			     SAMPLER_ID(0),
			     SRC_SEL_X(SQ_SEL_X),
			     SRC_SEL_Y(SQ_SEL_Y),
			     SRC_SEL_Z(SQ_SEL_0),
			     SRC_SEL_W(SQ_SEL_1));
    shader[i++] = TEX_DWORD_PAD;

    return i;
}

int evergreen_xv_vs(RADEONChipFamily ChipSet, uint32_t* shader)
{
    int i = 0;

    /* 0 */
    shader[i++] = CF_DWORD0(ADDR(6),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
                            CF_CONST(0),
                            COND(SQ_CF_COND_ACTIVE),
                            I_COUNT(2),
                            VALID_PIXEL_MODE(0),
                            END_OF_PROGRAM(0),
                            CF_INST(SQ_CF_INST_VC),
                            WHOLE_QUAD_MODE(0),
                            BARRIER(1));

    /* 1 - ALU */
    shader[i++] = CF_ALU_DWORD0(ADDR(4),
				KCACHE_BANK0(0),
				KCACHE_BANK1(0),
				KCACHE_MODE0(SQ_CF_KCACHE_LOCK_1));
    shader[i++] = CF_ALU_DWORD1(KCACHE_MODE1(SQ_CF_KCACHE_NOP),
				KCACHE_ADDR0(0),
				KCACHE_ADDR1(0),
				I_COUNT(2),
				ALT_CONST(0),
				CF_INST(SQ_CF_INST_ALU),
				WHOLE_QUAD_MODE(0),
				BARRIER(1));

    /* 2 */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(CF_POS0),
                                          TYPE(SQ_EXPORT_POS),
                                          RW_GPR(1),
                                          RW_REL(ABSOLUTE),
                                          INDEX_GPR(0),
                                          ELEM_SIZE(3));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
                                               SRC_SEL_Y(SQ_SEL_Y),
                                               SRC_SEL_Z(SQ_SEL_Z),
                                               SRC_SEL_W(SQ_SEL_W),
                                               BURST_COUNT(1),
                                               VALID_PIXEL_MODE(0),
                                               END_OF_PROGRAM(0),
                                               CF_INST(SQ_CF_INST_EXPORT_DONE),
                                               MARK(0),
                                               BARRIER(1));
    /* 3 */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(0),
                                          TYPE(SQ_EXPORT_PARAM),
                                          RW_GPR(0),
                                          RW_REL(ABSOLUTE),
                                          INDEX_GPR(0),
                                          ELEM_SIZE(3));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
                                               SRC_SEL_Y(SQ_SEL_Y),
                                               SRC_SEL_Z(SQ_SEL_Z),
                                               SRC_SEL_W(SQ_SEL_W),
                                               BURST_COUNT(1),
                                               VALID_PIXEL_MODE(0),
                                               END_OF_PROGRAM(1),
                                               CF_INST(SQ_CF_INST_EXPORT_DONE),
                                               MARK(0),
                                               BARRIER(0));


    /* 4 texX / w */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_X),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_AR_X),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_MUL),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(0),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_X),
                                 CLAMP(0));

    /* 5 texY / h */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Y),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Y),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_AR_X),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_MUL),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(0),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Y),
                                 CLAMP(0));

    /* 6/7 */
    shader[i++] = VTX_DWORD0(VTX_INST(SQ_VTX_INST_FETCH),
                             FETCH_TYPE(SQ_VTX_FETCH_VERTEX_DATA),
                             FETCH_WHOLE_QUAD(0),
                             BUFFER_ID(0),
                             SRC_GPR(0),
                             SRC_REL(ABSOLUTE),
                             SRC_SEL_X(SQ_SEL_X),
                             MEGA_FETCH_COUNT(16));
    shader[i++] = VTX_DWORD1_GPR(DST_GPR(1),
                                 DST_REL(ABSOLUTE),
                                 DST_SEL_X(SQ_SEL_X),
                                 DST_SEL_Y(SQ_SEL_Y),
                                 DST_SEL_Z(SQ_SEL_0),
                                 DST_SEL_W(SQ_SEL_1),
                                 USE_CONST_FIELDS(0),
                                 DATA_FORMAT(FMT_32_32_FLOAT),
                                 NUM_FORMAT_ALL(SQ_NUM_FORMAT_SCALED),
                                 FORMAT_COMP_ALL(SQ_FORMAT_COMP_SIGNED),
                                 SRF_MODE_ALL(SRF_MODE_ZERO_CLAMP_MINUS_ONE));
    shader[i++] = VTX_DWORD2(OFFSET(0),
#if X_BYTE_ORDER == X_BIG_ENDIAN
                             ENDIAN_SWAP(SQ_ENDIAN_8IN32),
#else
                             ENDIAN_SWAP(SQ_ENDIAN_NONE),
#endif
                             CONST_BUF_NO_STRIDE(0),
                             MEGA_FETCH(1),
			     ALT_CONST(0),
			     BUFFER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = VTX_DWORD_PAD;
    /* 8/9 */
    shader[i++] = VTX_DWORD0(VTX_INST(SQ_VTX_INST_FETCH),
                             FETCH_TYPE(SQ_VTX_FETCH_VERTEX_DATA),
                             FETCH_WHOLE_QUAD(0),
                             BUFFER_ID(0),
                             SRC_GPR(0),
                             SRC_REL(ABSOLUTE),
                             SRC_SEL_X(SQ_SEL_X),
                             MEGA_FETCH_COUNT(8));
    shader[i++] = VTX_DWORD1_GPR(DST_GPR(0),
                                 DST_REL(ABSOLUTE),
                                 DST_SEL_X(SQ_SEL_X),
                                 DST_SEL_Y(SQ_SEL_Y),
                                 DST_SEL_Z(SQ_SEL_0),
                                 DST_SEL_W(SQ_SEL_1),
                                 USE_CONST_FIELDS(0),
                                 DATA_FORMAT(FMT_32_32_FLOAT),
                                 NUM_FORMAT_ALL(SQ_NUM_FORMAT_SCALED),
                                 FORMAT_COMP_ALL(SQ_FORMAT_COMP_SIGNED),
                                 SRF_MODE_ALL(SRF_MODE_ZERO_CLAMP_MINUS_ONE));
    shader[i++] = VTX_DWORD2(OFFSET(8),
#if X_BYTE_ORDER == X_BIG_ENDIAN
                             ENDIAN_SWAP(SQ_ENDIAN_8IN32),
#else
                             ENDIAN_SWAP(SQ_ENDIAN_NONE),
#endif
                             CONST_BUF_NO_STRIDE(0),
                             MEGA_FETCH(0),
			     ALT_CONST(0),
			     BUFFER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = VTX_DWORD_PAD;

    return i;
}

int evergreen_xv_ps(RADEONChipFamily ChipSet, uint32_t* shader)
{
    int i = 0;

    /* 0 */
    shader[i++] = CF_ALU_DWORD0(ADDR(5),
				KCACHE_BANK0(0),
				KCACHE_BANK1(0),
				KCACHE_MODE0(SQ_CF_KCACHE_NOP));
    shader[i++] = CF_ALU_DWORD1(KCACHE_MODE1(SQ_CF_KCACHE_NOP),
				KCACHE_ADDR0(0),
				KCACHE_ADDR1(0),
				I_COUNT(4),
				ALT_CONST(0),
				CF_INST(SQ_CF_INST_ALU),
				WHOLE_QUAD_MODE(0),
				BARRIER(1));
    /* 1 */
    shader[i++] = CF_DWORD0(ADDR(21),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
                            CF_CONST(0),
                            COND(SQ_CF_COND_BOOL),
                            I_COUNT(0),
                            VALID_PIXEL_MODE(0),
                            END_OF_PROGRAM(0),
                            CF_INST(SQ_CF_INST_CALL),
                            WHOLE_QUAD_MODE(0),
                            BARRIER(0));
    /* 2 */
    shader[i++] = CF_DWORD0(ADDR(30),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
                            CF_CONST(0),
                            COND(SQ_CF_COND_NOT_BOOL),
                            I_COUNT(0),
                            VALID_PIXEL_MODE(0),
                            END_OF_PROGRAM(0),
                            CF_INST(SQ_CF_INST_CALL),
                            WHOLE_QUAD_MODE(0),
                            BARRIER(0));
    /* 3 */
    shader[i++] = CF_ALU_DWORD0(ADDR(9),
                                KCACHE_BANK0(0),
                                KCACHE_BANK1(0),
                                KCACHE_MODE0(SQ_CF_KCACHE_LOCK_1));
    shader[i++] = CF_ALU_DWORD1(KCACHE_MODE1(SQ_CF_KCACHE_NOP),
                                KCACHE_ADDR0(0),
                                KCACHE_ADDR1(0),
                                I_COUNT(12),
                                ALT_CONST(0),
                                CF_INST(SQ_CF_INST_ALU),
                                WHOLE_QUAD_MODE(0),
                                BARRIER(1));
    /* 4 */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(CF_PIXEL_MRT0),
                                          TYPE(SQ_EXPORT_PIXEL),
                                          RW_GPR(2),
                                          RW_REL(ABSOLUTE),
                                          INDEX_GPR(0),
                                          ELEM_SIZE(3));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
                                               SRC_SEL_Y(SQ_SEL_Y),
                                               SRC_SEL_Z(SQ_SEL_Z),
                                               SRC_SEL_W(SQ_SEL_W),
                                               BURST_COUNT(1),
                                               VALID_PIXEL_MODE(0),
                                               END_OF_PROGRAM(1),
                                               CF_INST(SQ_CF_INST_EXPORT_DONE),
                                               MARK(0),
                                               BARRIER(1));
    /* 5 interpolate tex coords */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Y),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_X),
				 CLAMP(0));
    /* 6 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_X),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Y),
				 CLAMP(0));
    /* 7 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Y),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(0),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Z),
				 CLAMP(0));
    /* 8 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_X),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(0),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_W),
				 CLAMP(0));

    /* 9,10,11,12 */
    /* r2.x = MAD(c0.w, r1.x, c0.x) */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_W),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_X),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP3(SRC2_SEL(ALU_SRC_KCACHE0_BASE + 0),
                                 SRC2_REL(ABSOLUTE),
                                 SRC2_ELEM(ELEM_X),
                                 SRC2_NEG(0),
                                 ALU_INST(SQ_OP3_INST_MULADD),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_X),
                                 CLAMP(0));
    /* r2.y = MAD(c0.w, r1.x, c0.y) */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_W),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_X),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP3(SRC2_SEL(ALU_SRC_KCACHE0_BASE + 0),
                                 SRC2_REL(ABSOLUTE),
                                 SRC2_ELEM(ELEM_Y),
                                 SRC2_NEG(0),
                                 ALU_INST(SQ_OP3_INST_MULADD),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Y),
                                 CLAMP(0));
    /* r2.z = MAD(c0.w, r1.x, c0.z) */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_W),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_X),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP3(SRC2_SEL(ALU_SRC_KCACHE0_BASE + 0),
                                 SRC2_REL(ABSOLUTE),
                                 SRC2_ELEM(ELEM_Z),
                                 SRC2_NEG(0),
                                 ALU_INST(SQ_OP3_INST_MULADD),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Z),
                                 CLAMP(0));
    /* r2.w = MAD(0, 0, 1) */
    shader[i++] = ALU_DWORD0(SRC0_SEL(SQ_ALU_SRC_0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(SQ_ALU_SRC_0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_X),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP3(SRC2_SEL(SQ_ALU_SRC_1),
                                 SRC2_REL(ABSOLUTE),
                                 SRC2_ELEM(ELEM_X),
                                 SRC2_NEG(0),
                                 ALU_INST(SQ_OP3_INST_MULADD),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_W),
                                 CLAMP(0));

    /* 13,14,15,16 */
    /* r2.x = MAD(c1.x, r1.y, pv.x) */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_KCACHE0_BASE + 1),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Y),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP3(SRC2_SEL(SQ_ALU_SRC_PV),
                                 SRC2_REL(ABSOLUTE),
                                 SRC2_ELEM(ELEM_X),
                                 SRC2_NEG(0),
                                 ALU_INST(SQ_OP3_INST_MULADD),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_X),
                                 CLAMP(0));
    /* r2.y = MAD(c1.y, r1.y, pv.y) */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_KCACHE0_BASE + 1),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Y),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Y),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP3(SRC2_SEL(SQ_ALU_SRC_PV),
                                 SRC2_REL(ABSOLUTE),
                                 SRC2_ELEM(ELEM_Y),
                                 SRC2_NEG(0),
                                 ALU_INST(SQ_OP3_INST_MULADD),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Y),
                                 CLAMP(0));
    /* r2.z = MAD(c1.z, r1.y, pv.z) */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_KCACHE0_BASE + 1),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Z),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Y),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP3(SRC2_SEL(SQ_ALU_SRC_PV),
                                 SRC2_REL(ABSOLUTE),
                                 SRC2_ELEM(ELEM_Z),
                                 SRC2_NEG(0),
                                 ALU_INST(SQ_OP3_INST_MULADD),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Z),
                                 CLAMP(0));
    /* r2.w = MAD(0, 0, 1) */
    shader[i++] = ALU_DWORD0(SRC0_SEL(SQ_ALU_SRC_0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(SQ_ALU_SRC_0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_X),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP3(SRC2_SEL(SQ_ALU_SRC_1),
                                 SRC2_REL(ABSOLUTE),
                                 SRC2_ELEM(ELEM_W),
                                 SRC2_NEG(0),
                                 ALU_INST(SQ_OP3_INST_MULADD),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_W),
                                 CLAMP(0));
    /* 17,18,19,20 */
    /* r2.x = MAD(c2.x, r1.z, pv.x) */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_KCACHE0_BASE + 2),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Z),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP3(SRC2_SEL(SQ_ALU_SRC_PV),
                                 SRC2_REL(ABSOLUTE),
                                 SRC2_ELEM(ELEM_X),
                                 SRC2_NEG(0),
                                 ALU_INST(SQ_OP3_INST_MULADD),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_X),
                                 CLAMP(1));
    /* r2.y = MAD(c2.y, r1.z, pv.y) */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_KCACHE0_BASE + 2),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Y),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Z),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP3(SRC2_SEL(SQ_ALU_SRC_PV),
                                 SRC2_REL(ABSOLUTE),
                                 SRC2_ELEM(ELEM_Y),
                                 SRC2_NEG(0),
                                 ALU_INST(SQ_OP3_INST_MULADD),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Y),
                                 CLAMP(1));
    /* r2.z = MAD(c2.z, r1.z, pv.z) */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_KCACHE0_BASE + 2),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Z),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Z),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP3(SRC2_SEL(SQ_ALU_SRC_PV),
                                 SRC2_REL(ABSOLUTE),
                                 SRC2_ELEM(ELEM_Z),
                                 SRC2_NEG(0),
                                 ALU_INST(SQ_OP3_INST_MULADD),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Z),
                                 CLAMP(1));
    /* r2.w = MAD(0, 0, 1) */
    shader[i++] = ALU_DWORD0(SRC0_SEL(SQ_ALU_SRC_0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(SQ_ALU_SRC_0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_X),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP3(SRC2_SEL(SQ_ALU_SRC_1),
                                 SRC2_REL(ABSOLUTE),
                                 SRC2_ELEM(ELEM_X),
                                 SRC2_NEG(0),
                                 ALU_INST(SQ_OP3_INST_MULADD),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_W),
                                 CLAMP(1));

    /* 21 */
    shader[i++] = CF_DWORD0(ADDR(24),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
                            CF_CONST(0),
                            COND(SQ_CF_COND_ACTIVE),
                            I_COUNT(3),
                            VALID_PIXEL_MODE(0),
                            END_OF_PROGRAM(0),
                            CF_INST(SQ_CF_INST_TC),
                            WHOLE_QUAD_MODE(0),
                            BARRIER(1));
    /* 22 */
    shader[i++] = CF_DWORD0(ADDR(0),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
			    CF_CONST(0),
			    COND(SQ_CF_COND_ACTIVE),
			    I_COUNT(0),
			    VALID_PIXEL_MODE(0),
			    END_OF_PROGRAM(0),
			    CF_INST(SQ_CF_INST_RETURN),
			    WHOLE_QUAD_MODE(0),
			    BARRIER(1));
    /* 23 */
    shader[i++] = 0x00000000;
    shader[i++] = 0x00000000;
    /* 24/25 */
    shader[i++] = TEX_DWORD0(TEX_INST(SQ_TEX_INST_SAMPLE),
                             INST_MOD(0),
                             FETCH_WHOLE_QUAD(0),
                             RESOURCE_ID(0),
                             SRC_GPR(0),
                             SRC_REL(ABSOLUTE),
                             ALT_CONST(0),
			     RESOURCE_INDEX_MODE(SQ_CF_INDEX_NONE),
			     SAMPLER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = TEX_DWORD1(DST_GPR(1),
                             DST_REL(ABSOLUTE),
                             DST_SEL_X(SQ_SEL_X),
                             DST_SEL_Y(SQ_SEL_MASK),
                             DST_SEL_Z(SQ_SEL_MASK),
                             DST_SEL_W(SQ_SEL_1),
                             LOD_BIAS(0),
                             COORD_TYPE_X(TEX_NORMALIZED),
                             COORD_TYPE_Y(TEX_NORMALIZED),
                             COORD_TYPE_Z(TEX_NORMALIZED),
                             COORD_TYPE_W(TEX_NORMALIZED));
    shader[i++] = TEX_DWORD2(OFFSET_X(0),
                             OFFSET_Y(0),
                             OFFSET_Z(0),
                             SAMPLER_ID(0),
                             SRC_SEL_X(SQ_SEL_X),
                             SRC_SEL_Y(SQ_SEL_Y),
                             SRC_SEL_Z(SQ_SEL_0),
                             SRC_SEL_W(SQ_SEL_1));
    shader[i++] = TEX_DWORD_PAD;
    /* 26/27 */
    shader[i++] = TEX_DWORD0(TEX_INST(SQ_TEX_INST_SAMPLE),
                             INST_MOD(0),
                             FETCH_WHOLE_QUAD(0),
                             RESOURCE_ID(1),
                             SRC_GPR(0),
                             SRC_REL(ABSOLUTE),
                             ALT_CONST(0),
			     RESOURCE_INDEX_MODE(SQ_CF_INDEX_NONE),
			     SAMPLER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = TEX_DWORD1(DST_GPR(1),
                             DST_REL(ABSOLUTE),
                             DST_SEL_X(SQ_SEL_MASK),
                             DST_SEL_Y(SQ_SEL_MASK),
                             DST_SEL_Z(SQ_SEL_X),
                             DST_SEL_W(SQ_SEL_MASK),
                             LOD_BIAS(0),
                             COORD_TYPE_X(TEX_NORMALIZED),
                             COORD_TYPE_Y(TEX_NORMALIZED),
                             COORD_TYPE_Z(TEX_NORMALIZED),
                             COORD_TYPE_W(TEX_NORMALIZED));
    shader[i++] = TEX_DWORD2(OFFSET_X(0),
                             OFFSET_Y(0),
                             OFFSET_Z(0),
                             SAMPLER_ID(1),
                             SRC_SEL_X(SQ_SEL_X),
                             SRC_SEL_Y(SQ_SEL_Y),
                             SRC_SEL_Z(SQ_SEL_0),
                             SRC_SEL_W(SQ_SEL_1));
    shader[i++] = TEX_DWORD_PAD;
    /* 28/29 */
    shader[i++] = TEX_DWORD0(TEX_INST(SQ_TEX_INST_SAMPLE),
                             INST_MOD(0),
                             FETCH_WHOLE_QUAD(0),
                             RESOURCE_ID(2),
                             SRC_GPR(0),
                             SRC_REL(ABSOLUTE),
                             ALT_CONST(0),
			     RESOURCE_INDEX_MODE(SQ_CF_INDEX_NONE),
			     SAMPLER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = TEX_DWORD1(DST_GPR(1),
                             DST_REL(ABSOLUTE),
                             DST_SEL_X(SQ_SEL_MASK),
                             DST_SEL_Y(SQ_SEL_X),
                             DST_SEL_Z(SQ_SEL_MASK),
                             DST_SEL_W(SQ_SEL_MASK),
                             LOD_BIAS(0),
                             COORD_TYPE_X(TEX_NORMALIZED),
                             COORD_TYPE_Y(TEX_NORMALIZED),
                             COORD_TYPE_Z(TEX_NORMALIZED),
                             COORD_TYPE_W(TEX_NORMALIZED));
    shader[i++] = TEX_DWORD2(OFFSET_X(0),
                             OFFSET_Y(0),
                             OFFSET_Z(0),
                             SAMPLER_ID(2),
                             SRC_SEL_X(SQ_SEL_X),
                             SRC_SEL_Y(SQ_SEL_Y),
                             SRC_SEL_Z(SQ_SEL_0),
                             SRC_SEL_W(SQ_SEL_1));
    shader[i++] = TEX_DWORD_PAD;
    /* 30 */
    shader[i++] = CF_DWORD0(ADDR(32),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
                            CF_CONST(0),
                            COND(SQ_CF_COND_ACTIVE),
                            I_COUNT(1),
                            VALID_PIXEL_MODE(0),
                            END_OF_PROGRAM(0),
                            CF_INST(SQ_CF_INST_TC),
                            WHOLE_QUAD_MODE(0),
                            BARRIER(1));
    /* 31 */
    shader[i++] = CF_DWORD0(ADDR(0),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
			    CF_CONST(0),
			    COND(SQ_CF_COND_ACTIVE),
			    I_COUNT(0),
			    VALID_PIXEL_MODE(0),
			    END_OF_PROGRAM(0),
			    CF_INST(SQ_CF_INST_RETURN),
			    WHOLE_QUAD_MODE(0),
			    BARRIER(1));
    /* 32/33 */
    shader[i++] = TEX_DWORD0(TEX_INST(SQ_TEX_INST_SAMPLE),
                             INST_MOD(0),
                             FETCH_WHOLE_QUAD(0),
                             RESOURCE_ID(0),
                             SRC_GPR(0),
                             SRC_REL(ABSOLUTE),
                             ALT_CONST(0),
                             RESOURCE_INDEX_MODE(SQ_CF_INDEX_NONE),
                             SAMPLER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = TEX_DWORD1(DST_GPR(1),
                             DST_REL(ABSOLUTE),
                             DST_SEL_X(SQ_SEL_X),
                             DST_SEL_Y(SQ_SEL_Y),
                             DST_SEL_Z(SQ_SEL_Z),
                             DST_SEL_W(SQ_SEL_1),
                             LOD_BIAS(0),
                             COORD_TYPE_X(TEX_NORMALIZED),
                             COORD_TYPE_Y(TEX_NORMALIZED),
                             COORD_TYPE_Z(TEX_NORMALIZED),
                             COORD_TYPE_W(TEX_NORMALIZED));
    shader[i++] = TEX_DWORD2(OFFSET_X(0),
                             OFFSET_Y(0),
                             OFFSET_Z(0),
                             SAMPLER_ID(0),
                             SRC_SEL_X(SQ_SEL_X),
                             SRC_SEL_Y(SQ_SEL_Y),
                             SRC_SEL_Z(SQ_SEL_0),
                             SRC_SEL_W(SQ_SEL_1));
    shader[i++] = TEX_DWORD_PAD;

    return i;
}

/* comp vs --------------------------------------- */
int evergreen_comp_vs(RADEONChipFamily ChipSet, uint32_t* shader)
{
    int i = 0;

    /* 0 */
    shader[i++] = CF_DWORD0(ADDR(3),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
                            CF_CONST(0),
                            COND(SQ_CF_COND_BOOL),
                            I_COUNT(0),
                            VALID_PIXEL_MODE(0),
                            END_OF_PROGRAM(0),
                            CF_INST(SQ_CF_INST_CALL),
                            WHOLE_QUAD_MODE(0),
                            BARRIER(0));
    /* 1 */
    shader[i++] = CF_DWORD0(ADDR(9),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
                            CF_CONST(0),
                            COND(SQ_CF_COND_NOT_BOOL),
                            I_COUNT(0),
                            VALID_PIXEL_MODE(0),
                            END_OF_PROGRAM(0),
                            CF_INST(SQ_CF_INST_CALL),
                            WHOLE_QUAD_MODE(0),
                            BARRIER(0));
    /* 2 */
    shader[i++] = CF_DWORD0(ADDR(0),
                            JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
                            CF_CONST(0),
                            COND(SQ_CF_COND_ACTIVE),
                            I_COUNT(0),
                            VALID_PIXEL_MODE(0),
                            END_OF_PROGRAM(1),
                            CF_INST(SQ_CF_INST_NOP),
                            WHOLE_QUAD_MODE(0),
                            BARRIER(1));
    /* 3 - mask sub */
    shader[i++] = CF_DWORD0(ADDR(44),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
			    CF_CONST(0),
			    COND(SQ_CF_COND_ACTIVE),
			    I_COUNT(3),
			    VALID_PIXEL_MODE(0),
			    END_OF_PROGRAM(0),
			    CF_INST(SQ_CF_INST_VC),
			    WHOLE_QUAD_MODE(0),
			    BARRIER(1));

    /* 4 - ALU */
    shader[i++] = CF_ALU_DWORD0(ADDR(14),
				KCACHE_BANK0(0),
				KCACHE_BANK1(0),
				KCACHE_MODE0(SQ_CF_KCACHE_LOCK_1));
    shader[i++] = CF_ALU_DWORD1(KCACHE_MODE1(SQ_CF_KCACHE_NOP),
				KCACHE_ADDR0(0),
				KCACHE_ADDR1(0),
				I_COUNT(20),
				ALT_CONST(0),
				CF_INST(SQ_CF_INST_ALU),
				WHOLE_QUAD_MODE(0),
				BARRIER(1));

    /* 5 - dst */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(CF_POS0),
					  TYPE(SQ_EXPORT_POS),
					  RW_GPR(2),
					  RW_REL(ABSOLUTE),
					  INDEX_GPR(0),
					  ELEM_SIZE(0));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
					       SRC_SEL_Y(SQ_SEL_Y),
					       SRC_SEL_Z(SQ_SEL_0),
					       SRC_SEL_W(SQ_SEL_1),
					       BURST_COUNT(1),
					       VALID_PIXEL_MODE(0),
					       END_OF_PROGRAM(0),
					       CF_INST(SQ_CF_INST_EXPORT_DONE),
					       MARK(0),
					       BARRIER(1));
    /* 6 - src */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(0),
					  TYPE(SQ_EXPORT_PARAM),
					  RW_GPR(1),
					  RW_REL(ABSOLUTE),
					  INDEX_GPR(0),
					  ELEM_SIZE(0));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
					       SRC_SEL_Y(SQ_SEL_Y),
					       SRC_SEL_Z(SQ_SEL_0),
					       SRC_SEL_W(SQ_SEL_1),
					       BURST_COUNT(1),
					       VALID_PIXEL_MODE(0),
					       END_OF_PROGRAM(0),
					       CF_INST(SQ_CF_INST_EXPORT),
					       MARK(0),
					       BARRIER(0));
    /* 7 - mask */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(1),
					  TYPE(SQ_EXPORT_PARAM),
					  RW_GPR(0),
					  RW_REL(ABSOLUTE),
					  INDEX_GPR(0),
					  ELEM_SIZE(0));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
					       SRC_SEL_Y(SQ_SEL_Y),
					       SRC_SEL_Z(SQ_SEL_0),
					       SRC_SEL_W(SQ_SEL_1),
					       BURST_COUNT(1),
					       VALID_PIXEL_MODE(0),
					       END_OF_PROGRAM(0),
					       CF_INST(SQ_CF_INST_EXPORT_DONE),
					       WHOLE_QUAD_MODE(0),
					       BARRIER(0));
    /* 8 */
    shader[i++] = CF_DWORD0(ADDR(0),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
			    CF_CONST(0),
			    COND(SQ_CF_COND_ACTIVE),
			    I_COUNT(0),
			    VALID_PIXEL_MODE(0),
			    END_OF_PROGRAM(0),
			    CF_INST(SQ_CF_INST_RETURN),
			    WHOLE_QUAD_MODE(0),
			    BARRIER(1));
    /* 9 - non-mask sub */
    shader[i++] = CF_DWORD0(ADDR(50),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
			    CF_CONST(0),
			    COND(SQ_CF_COND_ACTIVE),
			    I_COUNT(2),
			    VALID_PIXEL_MODE(0),
			    END_OF_PROGRAM(0),
			    CF_INST(SQ_CF_INST_VC),
			    WHOLE_QUAD_MODE(0),
			    BARRIER(1));

    /* 10 - ALU */
    shader[i++] = CF_ALU_DWORD0(ADDR(34),
				KCACHE_BANK0(0),
				KCACHE_BANK1(0),
				KCACHE_MODE0(SQ_CF_KCACHE_LOCK_1));
    shader[i++] = CF_ALU_DWORD1(KCACHE_MODE1(SQ_CF_KCACHE_NOP),
				KCACHE_ADDR0(0),
				KCACHE_ADDR1(0),
				I_COUNT(10),
				ALT_CONST(0),
				CF_INST(SQ_CF_INST_ALU),
				WHOLE_QUAD_MODE(0),
				BARRIER(1));

    /* 11 - dst */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(CF_POS0),
					  TYPE(SQ_EXPORT_POS),
					  RW_GPR(1),
					  RW_REL(ABSOLUTE),
					  INDEX_GPR(0),
					  ELEM_SIZE(0));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
					       SRC_SEL_Y(SQ_SEL_Y),
					       SRC_SEL_Z(SQ_SEL_0),
					       SRC_SEL_W(SQ_SEL_1),
					       BURST_COUNT(0),
					       VALID_PIXEL_MODE(0),
					       END_OF_PROGRAM(0),
					       CF_INST(SQ_CF_INST_EXPORT_DONE),
					       MARK(0),
					       BARRIER(1));
    /* 12 - src */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(0),
					  TYPE(SQ_EXPORT_PARAM),
					  RW_GPR(0),
					  RW_REL(ABSOLUTE),
					  INDEX_GPR(0),
					  ELEM_SIZE(0));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
					       SRC_SEL_Y(SQ_SEL_Y),
					       SRC_SEL_Z(SQ_SEL_0),
					       SRC_SEL_W(SQ_SEL_1),
					       BURST_COUNT(0),
					       VALID_PIXEL_MODE(0),
					       END_OF_PROGRAM(0),
					       CF_INST(SQ_CF_INST_EXPORT_DONE),
					       MARK(0),
					       BARRIER(0));
    /* 13 */
    shader[i++] = CF_DWORD0(ADDR(0),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
			    CF_CONST(0),
			    COND(SQ_CF_COND_ACTIVE),
			    I_COUNT(0),
			    VALID_PIXEL_MODE(0),
			    END_OF_PROGRAM(0),
			    CF_INST(SQ_CF_INST_RETURN),
			    WHOLE_QUAD_MODE(0),
			    BARRIER(1));

    /* 14 srcX.x DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_X),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(3),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_X),
                                 CLAMP(0));

    /* 15 srcX.y DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Y),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Y),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(3),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Y),
                                 CLAMP(0));

    /* 16 srcX.z DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Z),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Z),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(3),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Z),
                                 CLAMP(0));

    /* 17 srcX.w DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_W),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_W),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(3),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_W),
                                 CLAMP(0));

    /* 18 srcY.x DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_X),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(3),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_X),
                                 CLAMP(0));

    /* 19 srcY.y DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Y),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Y),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(3),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Y),
                                 CLAMP(0));

    /* 20 srcY.z DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Z),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Z),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(3),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Z),
                                 CLAMP(0));

    /* 21 srcY.w DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 1),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_W),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_W),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(3),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_W),
                                 CLAMP(0));

    /* 22 maskX.x DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 2),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_X),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(4),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_X),
                                 CLAMP(0));

    /* 23 maskX.y DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Y),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 2),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Y),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(4),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Y),
                                 CLAMP(0));

    /* 24 maskX.z DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Z),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 2),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Z),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(4),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Z),
                                 CLAMP(0));

    /* 25 maskX.w DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_W),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 2),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_W),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(4),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_W),
                                 CLAMP(0));

    /* 26 maskY.x DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 3),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_X),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(4),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_X),
                                 CLAMP(0));

    /* 27 maskY.y DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Y),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 3),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Y),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(4),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Y),
                                 CLAMP(0));

    /* 28 maskY.z DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Z),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 3),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Z),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(4),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Z),
                                 CLAMP(0));

    /* 29 maskY.w DOT4 - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_W),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 3),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_W),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(4),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_W),
                                 CLAMP(0));

    /* 30 srcX / w */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 3),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_W),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_AR_X),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_MUL),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(1),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_X),
                                 CLAMP(0));

    /* 31 srcY / h */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 3),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Y),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_W),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_AR_X),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_MUL),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(1),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Y),
                                 CLAMP(0));

    /* 32 maskX / w */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 4),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 2),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_W),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_AR_X),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_MUL),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(0),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_X),
                                 CLAMP(0));

    /* 33 maskY / h */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 4),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Y),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 3),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_W),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_AR_X),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_MUL),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(0),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Y),
                                 CLAMP(0));

    /* 34 srcX.x DOT4 - non-mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_X),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_X),
                                 CLAMP(0));

    /* 35 srcX.y DOT4 - non-mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Y),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Y),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Y),
                                 CLAMP(0));

    /* 36 srcX.z DOT4 - non-mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Z),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Z),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Z),
                                 CLAMP(0));

    /* 37 srcX.w DOT4 - non-mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_W),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_W),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_W),
                                 CLAMP(0));

    /* 38 srcY.x DOT4 - non-mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_X),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_X),
                                 CLAMP(0));

    /* 39 srcY.y DOT4 - non-mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Y),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Y),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Y),
                                 CLAMP(0));

    /* 40 srcY.z DOT4 - non-mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Z),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_Z),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Z),
                                 CLAMP(0));

    /* 41 srcY.w DOT4 - non-mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_W),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_W),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_LOOP),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(0),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_DOT4),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(2),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_W),
                                 CLAMP(0));

    /* 42 srcX / w */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 2),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_X),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 0),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_W),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_AR_X),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_MUL),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(0),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_X),
                                 CLAMP(0));

    /* 43 srcY / h */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 2),
                             SRC0_REL(ABSOLUTE),
                             SRC0_ELEM(ELEM_Y),
                             SRC0_NEG(0),
                             SRC1_SEL(ALU_SRC_KCACHE0_BASE + 1),
                             SRC1_REL(ABSOLUTE),
                             SRC1_ELEM(ELEM_W),
                             SRC1_NEG(0),
                             INDEX_MODE(SQ_INDEX_AR_X),
                             PRED_SEL(SQ_PRED_SEL_OFF),
                             LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
                                 SRC1_ABS(0),
                                 UPDATE_EXECUTE_MASK(0),
                                 UPDATE_PRED(0),
                                 WRITE_MASK(1),
                                 OMOD(SQ_ALU_OMOD_OFF),
                                 ALU_INST(SQ_OP2_INST_MUL),
                                 BANK_SWIZZLE(SQ_ALU_VEC_012),
                                 DST_GPR(0),
                                 DST_REL(ABSOLUTE),
                                 DST_ELEM(ELEM_Y),
                                 CLAMP(0));

    /* mask vfetch - 44/45 - dst */
    shader[i++] = VTX_DWORD0(VTX_INST(SQ_VTX_INST_FETCH),
			     FETCH_TYPE(SQ_VTX_FETCH_VERTEX_DATA),
			     FETCH_WHOLE_QUAD(0),
			     BUFFER_ID(0),
			     SRC_GPR(0),
			     SRC_REL(ABSOLUTE),
			     SRC_SEL_X(SQ_SEL_X),
			     MEGA_FETCH_COUNT(24));
    shader[i++] = VTX_DWORD1_GPR(DST_GPR(2),
				 DST_REL(0),
				 DST_SEL_X(SQ_SEL_X),
				 DST_SEL_Y(SQ_SEL_Y),
				 DST_SEL_Z(SQ_SEL_0),
				 DST_SEL_W(SQ_SEL_1),
				 USE_CONST_FIELDS(0),
				 DATA_FORMAT(FMT_32_32_FLOAT),
				 NUM_FORMAT_ALL(SQ_NUM_FORMAT_SCALED),
				 FORMAT_COMP_ALL(SQ_FORMAT_COMP_SIGNED),
				 SRF_MODE_ALL(SRF_MODE_ZERO_CLAMP_MINUS_ONE));
    shader[i++] = VTX_DWORD2(OFFSET(0),
#if X_BYTE_ORDER == X_BIG_ENDIAN
                             ENDIAN_SWAP(SQ_ENDIAN_8IN32),
#else
                             ENDIAN_SWAP(SQ_ENDIAN_NONE),
#endif
			     CONST_BUF_NO_STRIDE(0),
			     MEGA_FETCH(1),
			     ALT_CONST(0),
			     BUFFER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = VTX_DWORD_PAD;
    /* 46/47 - src */
    shader[i++] = VTX_DWORD0(VTX_INST(SQ_VTX_INST_FETCH),
			     FETCH_TYPE(SQ_VTX_FETCH_VERTEX_DATA),
			     FETCH_WHOLE_QUAD(0),
			     BUFFER_ID(0),
			     SRC_GPR(0),
			     SRC_REL(ABSOLUTE),
			     SRC_SEL_X(SQ_SEL_X),
			     MEGA_FETCH_COUNT(8));
    shader[i++] = VTX_DWORD1_GPR(DST_GPR(1),
				 DST_REL(0),
				 DST_SEL_X(SQ_SEL_X),
				 DST_SEL_Y(SQ_SEL_Y),
				 DST_SEL_Z(SQ_SEL_1),
				 DST_SEL_W(SQ_SEL_0),
				 USE_CONST_FIELDS(0),
				 DATA_FORMAT(FMT_32_32_FLOAT),
				 NUM_FORMAT_ALL(SQ_NUM_FORMAT_SCALED),
				 FORMAT_COMP_ALL(SQ_FORMAT_COMP_SIGNED),
				 SRF_MODE_ALL(SRF_MODE_ZERO_CLAMP_MINUS_ONE));
    shader[i++] = VTX_DWORD2(OFFSET(8),
#if X_BYTE_ORDER == X_BIG_ENDIAN
                             ENDIAN_SWAP(SQ_ENDIAN_8IN32),
#else
                             ENDIAN_SWAP(SQ_ENDIAN_NONE),
#endif
			     CONST_BUF_NO_STRIDE(0),
			     MEGA_FETCH(0),
			     ALT_CONST(0),
			     BUFFER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = VTX_DWORD_PAD;
    /* 48/49 - mask */
    shader[i++] = VTX_DWORD0(VTX_INST(SQ_VTX_INST_FETCH),
			     FETCH_TYPE(SQ_VTX_FETCH_VERTEX_DATA),
			     FETCH_WHOLE_QUAD(0),
			     BUFFER_ID(0),
			     SRC_GPR(0),
			     SRC_REL(ABSOLUTE),
			     SRC_SEL_X(SQ_SEL_X),
			     MEGA_FETCH_COUNT(8));
    shader[i++] = VTX_DWORD1_GPR(DST_GPR(0),
				 DST_REL(0),
				 DST_SEL_X(SQ_SEL_X),
				 DST_SEL_Y(SQ_SEL_Y),
				 DST_SEL_Z(SQ_SEL_1),
				 DST_SEL_W(SQ_SEL_0),
				 USE_CONST_FIELDS(0),
				 DATA_FORMAT(FMT_32_32_FLOAT),
				 NUM_FORMAT_ALL(SQ_NUM_FORMAT_SCALED),
				 FORMAT_COMP_ALL(SQ_FORMAT_COMP_SIGNED),
				 SRF_MODE_ALL(SRF_MODE_ZERO_CLAMP_MINUS_ONE));
    shader[i++] = VTX_DWORD2(OFFSET(16),
#if X_BYTE_ORDER == X_BIG_ENDIAN
                             ENDIAN_SWAP(SQ_ENDIAN_8IN32),
#else
                             ENDIAN_SWAP(SQ_ENDIAN_NONE),
#endif
			     CONST_BUF_NO_STRIDE(0),
			     MEGA_FETCH(0),
			     ALT_CONST(0),
			     BUFFER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = VTX_DWORD_PAD;

    /* no mask vfetch - 50/51 - dst */
    shader[i++] = VTX_DWORD0(VTX_INST(SQ_VTX_INST_FETCH),
			     FETCH_TYPE(SQ_VTX_FETCH_VERTEX_DATA),
			     FETCH_WHOLE_QUAD(0),
			     BUFFER_ID(0),
			     SRC_GPR(0),
			     SRC_REL(ABSOLUTE),
			     SRC_SEL_X(SQ_SEL_X),
			     MEGA_FETCH_COUNT(16));
    shader[i++] = VTX_DWORD1_GPR(DST_GPR(1),
				 DST_REL(0),
				 DST_SEL_X(SQ_SEL_X),
				 DST_SEL_Y(SQ_SEL_Y),
				 DST_SEL_Z(SQ_SEL_0),
				 DST_SEL_W(SQ_SEL_1),
				 USE_CONST_FIELDS(0),
				 DATA_FORMAT(FMT_32_32_FLOAT),
				 NUM_FORMAT_ALL(SQ_NUM_FORMAT_SCALED),
				 FORMAT_COMP_ALL(SQ_FORMAT_COMP_SIGNED),
				 SRF_MODE_ALL(SRF_MODE_ZERO_CLAMP_MINUS_ONE));
    shader[i++] = VTX_DWORD2(OFFSET(0),
#if X_BYTE_ORDER == X_BIG_ENDIAN
                             ENDIAN_SWAP(SQ_ENDIAN_8IN32),
#else
                             ENDIAN_SWAP(SQ_ENDIAN_NONE),
#endif
			     CONST_BUF_NO_STRIDE(0),
			     MEGA_FETCH(1),
			     ALT_CONST(0),
			     BUFFER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = VTX_DWORD_PAD;
    /* 52/53 - src */
    shader[i++] = VTX_DWORD0(VTX_INST(SQ_VTX_INST_FETCH),
			     FETCH_TYPE(SQ_VTX_FETCH_VERTEX_DATA),
			     FETCH_WHOLE_QUAD(0),
			     BUFFER_ID(0),
			     SRC_GPR(0),
			     SRC_REL(ABSOLUTE),
			     SRC_SEL_X(SQ_SEL_X),
			     MEGA_FETCH_COUNT(8));
    shader[i++] = VTX_DWORD1_GPR(DST_GPR(0),
				 DST_REL(0),
				 DST_SEL_X(SQ_SEL_X),
				 DST_SEL_Y(SQ_SEL_Y),
				 DST_SEL_Z(SQ_SEL_1),
				 DST_SEL_W(SQ_SEL_0),
				 USE_CONST_FIELDS(0),
				 DATA_FORMAT(FMT_32_32_FLOAT),
				 NUM_FORMAT_ALL(SQ_NUM_FORMAT_SCALED),
				 FORMAT_COMP_ALL(SQ_FORMAT_COMP_SIGNED),
				 SRF_MODE_ALL(SRF_MODE_ZERO_CLAMP_MINUS_ONE));
    shader[i++] = VTX_DWORD2(OFFSET(8),
#if X_BYTE_ORDER == X_BIG_ENDIAN
                             ENDIAN_SWAP(SQ_ENDIAN_8IN32),
#else
                             ENDIAN_SWAP(SQ_ENDIAN_NONE),
#endif
			     CONST_BUF_NO_STRIDE(0),
			     MEGA_FETCH(0),
                             ALT_CONST(0),
                             BUFFER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = VTX_DWORD_PAD;

    return i;
}

/* comp ps --------------------------------------- */
int evergreen_comp_ps(RADEONChipFamily ChipSet, uint32_t* shader)
{
    int i = 0;

    /* 0 */
    shader[i++] = CF_DWORD0(ADDR(3),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
                            CF_CONST(0),
                            COND(SQ_CF_COND_BOOL),
                            I_COUNT(0),
                            VALID_PIXEL_MODE(0),
                            END_OF_PROGRAM(0),
                            CF_INST(SQ_CF_INST_CALL),
                            WHOLE_QUAD_MODE(0),
                            BARRIER(0));
    /* 1 */
    shader[i++] = CF_DWORD0(ADDR(8),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
                            CF_CONST(0),
                            COND(SQ_CF_COND_NOT_BOOL),
                            I_COUNT(0),
                            VALID_PIXEL_MODE(0),
                            END_OF_PROGRAM(0),
                            CF_INST(SQ_CF_INST_CALL),
                            WHOLE_QUAD_MODE(0),
                            BARRIER(0));
    /* 2 */
    shader[i++] = CF_DWORD0(ADDR(0),
                            JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
                            CF_CONST(0),
                            COND(SQ_CF_COND_ACTIVE),
                            I_COUNT(0),
                            VALID_PIXEL_MODE(0),
                            END_OF_PROGRAM(1),
                            CF_INST(SQ_CF_INST_NOP),
                            WHOLE_QUAD_MODE(0),
                            BARRIER(1));

    /* 3 - mask sub */
    shader[i++] = CF_ALU_DWORD0(ADDR(12),
				KCACHE_BANK0(0),
				KCACHE_BANK1(0),
				KCACHE_MODE0(SQ_CF_KCACHE_NOP));
    shader[i++] = CF_ALU_DWORD1(KCACHE_MODE1(SQ_CF_KCACHE_NOP),
				KCACHE_ADDR0(0),
				KCACHE_ADDR1(0),
				I_COUNT(8),
				ALT_CONST(0),
				CF_INST(SQ_CF_INST_ALU),
				WHOLE_QUAD_MODE(0),
				BARRIER(1));

    /* 4 */
    shader[i++] = CF_DWORD0(ADDR(28),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
			    CF_CONST(0),
			    COND(SQ_CF_COND_ACTIVE),
			    I_COUNT(2),
			    VALID_PIXEL_MODE(0),
			    END_OF_PROGRAM(0),
			    CF_INST(SQ_CF_INST_TC),
			    WHOLE_QUAD_MODE(0),
			    BARRIER(1));

    /* 5 */
    shader[i++] = CF_ALU_DWORD0(ADDR(20),
				KCACHE_BANK0(0),
				KCACHE_BANK1(0),
				KCACHE_MODE0(SQ_CF_KCACHE_LOCK_1));
    shader[i++] = CF_ALU_DWORD1(KCACHE_MODE1(SQ_CF_KCACHE_NOP),
				KCACHE_ADDR0(0),
				KCACHE_ADDR1(0),
				I_COUNT(4),
				ALT_CONST(0),
				CF_INST(SQ_CF_INST_ALU),
				WHOLE_QUAD_MODE(0),
				BARRIER(1));

    /* 6 */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(CF_PIXEL_MRT0),
					  TYPE(SQ_EXPORT_PIXEL),
					  RW_GPR(2),
					  RW_REL(ABSOLUTE),
					  INDEX_GPR(0),
					  ELEM_SIZE(1));

    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
					       SRC_SEL_Y(SQ_SEL_Y),
					       SRC_SEL_Z(SQ_SEL_Z),
					       SRC_SEL_W(SQ_SEL_W),
					       BURST_COUNT(1),
					       VALID_PIXEL_MODE(0),
					       END_OF_PROGRAM(0),
					       CF_INST(SQ_CF_INST_EXPORT_DONE),
					       MARK(0),
					       BARRIER(1));
    /* 7 */
    shader[i++] = CF_DWORD0(ADDR(0),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
			    CF_CONST(0),
			    COND(SQ_CF_COND_ACTIVE),
			    I_COUNT(0),
			    VALID_PIXEL_MODE(0),
			    END_OF_PROGRAM(0),
			    CF_INST(SQ_CF_INST_RETURN),
			    WHOLE_QUAD_MODE(0),
			    BARRIER(1));

    /* 8 - non-mask sub */
    shader[i++] = CF_ALU_DWORD0(ADDR(24),
				KCACHE_BANK0(0),
				KCACHE_BANK1(0),
				KCACHE_MODE0(SQ_CF_KCACHE_NOP));
    shader[i++] = CF_ALU_DWORD1(KCACHE_MODE1(SQ_CF_KCACHE_NOP),
				KCACHE_ADDR0(0),
				KCACHE_ADDR1(0),
				I_COUNT(4),
				ALT_CONST(0),
				CF_INST(SQ_CF_INST_ALU),
				WHOLE_QUAD_MODE(0),
				BARRIER(1));
    /* 9 */
    shader[i++] = CF_DWORD0(ADDR(32),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
			    CF_CONST(0),
			    COND(SQ_CF_COND_ACTIVE),
			    I_COUNT(1),
			    VALID_PIXEL_MODE(0),
			    END_OF_PROGRAM(0),
			    CF_INST(SQ_CF_INST_TC),
			    WHOLE_QUAD_MODE(0),
			    BARRIER(1));

    /* 10 */
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD0(ARRAY_BASE(CF_PIXEL_MRT0),
					  TYPE(SQ_EXPORT_PIXEL),
					  RW_GPR(0),
					  RW_REL(ABSOLUTE),
					  INDEX_GPR(0),
					  ELEM_SIZE(1));
    shader[i++] = CF_ALLOC_IMP_EXP_DWORD1_SWIZ(SRC_SEL_X(SQ_SEL_X),
					       SRC_SEL_Y(SQ_SEL_Y),
					       SRC_SEL_Z(SQ_SEL_Z),
					       SRC_SEL_W(SQ_SEL_W),
					       BURST_COUNT(1),
					       VALID_PIXEL_MODE(0),
					       END_OF_PROGRAM(0),
					       CF_INST(SQ_CF_INST_EXPORT_DONE),
					       MARK(0),
					       BARRIER(1));

    /* 11 */
    shader[i++] = CF_DWORD0(ADDR(0),
			    JUMPTABLE_SEL(SQ_CF_JUMPTABLE_SEL_CONST_A));
    shader[i++] = CF_DWORD1(POP_COUNT(0),
			    CF_CONST(0),
			    COND(SQ_CF_COND_ACTIVE),
			    I_COUNT(0),
			    VALID_PIXEL_MODE(0),
			    END_OF_PROGRAM(0),
			    CF_INST(SQ_CF_INST_RETURN),
			    WHOLE_QUAD_MODE(0),
			    BARRIER(1));

    /* 12 interpolate src tex coords - mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Y),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(1),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_X),
				 CLAMP(0));
    /* 13 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_X),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(1),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Y),
				 CLAMP(0));
    /* 14 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Y),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(0),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(1),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Z),
				 CLAMP(0));
    /* 15 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_X),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(0),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(1),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_W),
				 CLAMP(0));

    /* 16 interpolate mask tex coords */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Y),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 1),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_X),
				 CLAMP(0));
    /* 17 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_X),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 1),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Y),
				 CLAMP(0));
    /* 18 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Y),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 1),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(0),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Z),
				 CLAMP(0));
    /* 19 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_X),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 1),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(0),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_W),
				 CLAMP(0));

    /* 20 - alu 0 */
    /* MUL gpr[2].x gpr[0].x gpr[1].x */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_X),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_GPR_BASE + 1),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_LOOP),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_MUL),
				 BANK_SWIZZLE(SQ_ALU_VEC_012),
				 DST_GPR(2),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_X),
				 CLAMP(1));
    /* 21 - alu 1 */
    /* MUL gpr[2].y gpr[0].y gpr[1].y */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Y),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_GPR_BASE + 1),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_Y),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_LOOP),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_MUL),
				 BANK_SWIZZLE(SQ_ALU_VEC_012),
				 DST_GPR(2),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Y),
				 CLAMP(1));
    /* 22 - alu 2 */
    /* MUL gpr[2].z gpr[0].z gpr[1].z */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Z),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_GPR_BASE + 1),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_Z),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_LOOP),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_MUL),
				 BANK_SWIZZLE(SQ_ALU_VEC_012),
				 DST_GPR(2),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Z),
				 CLAMP(1));
    /* 23 - alu 3 */
    /* MUL gpr[2].w gpr[0].w gpr[1].w */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_W),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_GPR_BASE + 1),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_W),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_LOOP),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_MUL),
				 BANK_SWIZZLE(SQ_ALU_VEC_012),
				 DST_GPR(2),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_W),
				 CLAMP(1));

    /* 24 - interpolate tex coords - non-mask */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Y),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_X),
				 CLAMP(0));
    /* 25 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_X),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(1),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Y),
				 CLAMP(0));
    /* 26 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_Y),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(0));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(0),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_Z),
				 CLAMP(0));
    /* 27 */
    shader[i++] = ALU_DWORD0(SRC0_SEL(ALU_SRC_GPR_BASE + 0),
			     SRC0_REL(ABSOLUTE),
			     SRC0_ELEM(ELEM_X),
			     SRC0_NEG(0),
			     SRC1_SEL(ALU_SRC_PARAM_BASE + 0),
			     SRC1_REL(ABSOLUTE),
			     SRC1_ELEM(ELEM_X),
			     SRC1_NEG(0),
			     INDEX_MODE(SQ_INDEX_AR_X),
			     PRED_SEL(SQ_PRED_SEL_OFF),
			     LAST(1));
    shader[i++] = ALU_DWORD1_OP2(SRC0_ABS(0),
				 SRC1_ABS(0),
				 UPDATE_EXECUTE_MASK(0),
				 UPDATE_PRED(0),
				 WRITE_MASK(0),
				 OMOD(SQ_ALU_OMOD_OFF),
				 ALU_INST(SQ_OP2_INST_INTERP_XY),
				 BANK_SWIZZLE(SQ_ALU_VEC_210),
				 DST_GPR(0),
				 DST_REL(ABSOLUTE),
				 DST_ELEM(ELEM_W),
				 CLAMP(0));

    /* 28/29 - src - mask */
    shader[i++] = TEX_DWORD0(TEX_INST(SQ_TEX_INST_SAMPLE),
			     INST_MOD(0),
			     FETCH_WHOLE_QUAD(0),
			     RESOURCE_ID(0),
			     SRC_GPR(1),
			     SRC_REL(ABSOLUTE),
			     ALT_CONST(0),
			     RESOURCE_INDEX_MODE(SQ_CF_INDEX_NONE),
			     SAMPLER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = TEX_DWORD1(DST_GPR(1),
			     DST_REL(ABSOLUTE),
			     DST_SEL_X(SQ_SEL_X),
			     DST_SEL_Y(SQ_SEL_Y),
			     DST_SEL_Z(SQ_SEL_Z),
			     DST_SEL_W(SQ_SEL_W),
			     LOD_BIAS(0),
			     COORD_TYPE_X(TEX_NORMALIZED),
			     COORD_TYPE_Y(TEX_NORMALIZED),
			     COORD_TYPE_Z(TEX_NORMALIZED),
			     COORD_TYPE_W(TEX_NORMALIZED));
    shader[i++] = TEX_DWORD2(OFFSET_X(0),
			     OFFSET_Y(0),
			     OFFSET_Z(0),
			     SAMPLER_ID(0),
			     SRC_SEL_X(SQ_SEL_X),
			     SRC_SEL_Y(SQ_SEL_Y),
			     SRC_SEL_Z(SQ_SEL_0),
			     SRC_SEL_W(SQ_SEL_1));
    shader[i++] = TEX_DWORD_PAD;
    /* 30/31 - mask */
    shader[i++] = TEX_DWORD0(TEX_INST(SQ_TEX_INST_SAMPLE),
			     INST_MOD(0),
			     FETCH_WHOLE_QUAD(0),
			     RESOURCE_ID(1),
			     SRC_GPR(0),
			     SRC_REL(ABSOLUTE),
                             ALT_CONST(0),
                             RESOURCE_INDEX_MODE(SQ_CF_INDEX_NONE),
                             SAMPLER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = TEX_DWORD1(DST_GPR(0),
			     DST_REL(ABSOLUTE),
			     DST_SEL_X(SQ_SEL_X),
			     DST_SEL_Y(SQ_SEL_Y),
			     DST_SEL_Z(SQ_SEL_Z),
			     DST_SEL_W(SQ_SEL_W),
			     LOD_BIAS(0),
			     COORD_TYPE_X(TEX_NORMALIZED),
			     COORD_TYPE_Y(TEX_NORMALIZED),
			     COORD_TYPE_Z(TEX_NORMALIZED),
			     COORD_TYPE_W(TEX_NORMALIZED));
    shader[i++] = TEX_DWORD2(OFFSET_X(0),
			     OFFSET_Y(0),
			     OFFSET_Z(0),
			     SAMPLER_ID(1),
			     SRC_SEL_X(SQ_SEL_X),
			     SRC_SEL_Y(SQ_SEL_Y),
			     SRC_SEL_Z(SQ_SEL_0),
			     SRC_SEL_W(SQ_SEL_1));
    shader[i++] = TEX_DWORD_PAD;

    /* 32/33 - src - non-mask */
    shader[i++] = TEX_DWORD0(TEX_INST(SQ_TEX_INST_SAMPLE),
			     INST_MOD(0),
			     FETCH_WHOLE_QUAD(0),
			     RESOURCE_ID(0),
			     SRC_GPR(0),
			     SRC_REL(ABSOLUTE),
			     ALT_CONST(0),
			     RESOURCE_INDEX_MODE(SQ_CF_INDEX_NONE),
			     SAMPLER_INDEX_MODE(SQ_CF_INDEX_NONE));
    shader[i++] = TEX_DWORD1(DST_GPR(0),
			     DST_REL(ABSOLUTE),
			     DST_SEL_X(SQ_SEL_X),
			     DST_SEL_Y(SQ_SEL_Y),
			     DST_SEL_Z(SQ_SEL_Z),
			     DST_SEL_W(SQ_SEL_W),
			     LOD_BIAS(0),
			     COORD_TYPE_X(TEX_NORMALIZED),
			     COORD_TYPE_Y(TEX_NORMALIZED),
			     COORD_TYPE_Z(TEX_NORMALIZED),
			     COORD_TYPE_W(TEX_NORMALIZED));
    shader[i++] = TEX_DWORD2(OFFSET_X(0),
			     OFFSET_Y(0),
			     OFFSET_Z(0),
			     SAMPLER_ID(0),
			     SRC_SEL_X(SQ_SEL_X),
			     SRC_SEL_Y(SQ_SEL_Y),
			     SRC_SEL_Z(SQ_SEL_0),
			     SRC_SEL_W(SQ_SEL_1));
    shader[i++] = TEX_DWORD_PAD;

    return i;
}

#endif
