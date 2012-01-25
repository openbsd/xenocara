/*
 * Cayman shaders
 *
 * Copyright (C) 2011  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Shader macros
 */

#ifndef __SHADER_H__
#define __SHADER_H__

#include "radeon.h"

/* Oder of instructions: All CF, All ALU, All Tex/Vtx fetches */


// CF insts
// addr
#define ADDR(x)  (x)
// jumptable
#define JUMPTABLE_SEL(x) (x)
// pc
#define POP_COUNT(x)      (x)
// const
#define CF_CONST(x)       (x)
// cond
#define COND(x)        (x)		// SQ_COND_*
// count
#define I_COUNT(x)        ((x) ? ((x) - 1) : 0)
// vpm
#define VALID_PIXEL_MODE(x) (x)
// cf inst
#define CF_INST(x)        (x)		// SQ_CF_INST_*
// wqm
#define WHOLE_QUAD_MODE(x)  (x)
// barrier
#define BARRIER(x)          (x)
//kb0
#define KCACHE_BANK0(x)          (x)
//kb1
#define KCACHE_BANK1(x)          (x)
// km0/1
#define KCACHE_MODE0(x)          (x)
#define KCACHE_MODE1(x)          (x)	// SQ_CF_KCACHE_*
//
#define KCACHE_ADDR0(x)          (x)
#define KCACHE_ADDR1(x)          (x)

#define ALT_CONST(x)            (x)

#define ARRAY_BASE(x)        (x)
// export pixel
#define CF_PIXEL_MRT0         0
#define CF_PIXEL_MRT1         1
#define CF_PIXEL_MRT2         2
#define CF_PIXEL_MRT3         3
#define CF_PIXEL_MRT4         4
#define CF_PIXEL_MRT5         5
#define CF_PIXEL_MRT6         6
#define CF_PIXEL_MRT7         7
// computed Z
#define CF_COMPUTED_Z         61
// export pos
#define CF_POS0               60
#define CF_POS1               61
#define CF_POS2               62
#define CF_POS3               63
// export param
// 0...31
#define TYPE(x)              (x)	// SQ_EXPORT_*
#define RW_GPR(x)            (x)
#define RW_REL(x)            (x)
#define ABSOLUTE                  0
#define RELATIVE                  1
#define INDEX_GPR(x)            (x)
#define ELEM_SIZE(x)            (x ? (x - 1) : 0)
#define BURST_COUNT(x)          (x ? (x - 1) : 0)
#define MARK(x)         (x)

// swiz
#define SRC_SEL_X(x)    (x)		// SQ_SEL_* each
#define SRC_SEL_Y(x)    (x)
#define SRC_SEL_Z(x)    (x)
#define SRC_SEL_W(x)    (x)

#define CF_DWORD0(addr, jmptbl) cpu_to_le32(((addr) | ((jmptbl) << 24)))
#define CF_DWORD1(pc, cf_const, cond, count, vpm, cf_inst, b) \
    cpu_to_le32((((pc) << 0) | ((cf_const) << 3) | ((cond) << 8) | ((count) << 10) | \
		 ((vpm) << 20) | ((cf_inst) << 22) | ((b) << 31)))

#define CF_ALU_DWORD0(addr, kb0, kb1, km0) cpu_to_le32((((addr) << 0) | ((kb0) << 22) | ((kb1) << 26) | ((km0) << 30)))
#define CF_ALU_DWORD1(km1, kcache_addr0, kcache_addr1, count, alt_const, cf_inst, wqm, b) \
    cpu_to_le32((((km1) << 0) | ((kcache_addr0) << 2) | ((kcache_addr1) << 10) | \
		 ((count) << 18) | ((alt_const) << 25) | ((cf_inst) << 26) | ((wqm) << 30) | ((b) << 31)))

#define CF_ALLOC_IMP_EXP_DWORD0(array_base, type, rw_gpr, rr, index_gpr, es) \
    cpu_to_le32((((array_base) << 0) | ((type) << 13) | ((rw_gpr) << 15) | ((rr) << 22) | \
		 ((index_gpr) << 23) | ((es) << 30)))
#define CF_ALLOC_IMP_EXP_DWORD1_SWIZ(sel_x, sel_y, sel_z, sel_w, bc, vpm, cf_inst, m, b) \
    cpu_to_le32((((sel_x) << 0) | ((sel_y) << 3) | ((sel_z) << 6) | ((sel_w) << 9) | \
		 ((bc) << 16) | ((vpm) << 20) | ((cf_inst) << 22) | ((m) << 30) | ((b) << 31)))

// ALU clause insts
#define SRC0_SEL(x)        (x)
#define SRC1_SEL(x)        (x)
#define SRC2_SEL(x)        (x)
// src[0-2]_sel
//   0-127 GPR
// 128-159 kcache constants bank 0
// 160-191 kcache constants bank 1
// 192-255 inline const values
// 256-287 kcache constants bank 2
// 288-319 kcache constants bank 3
// 219-255 special SQ_ALU_SRC_* (0, 1, etc.)
// 488-520 src param space
#define ALU_SRC_GPR_BASE        0
#define ALU_SRC_KCACHE0_BASE  128
#define ALU_SRC_KCACHE1_BASE  160
#define ALU_SRC_INLINE_K_BASE 192
#define ALU_SRC_KCACHE2_BASE  256
#define ALU_SRC_KCACHE3_BASE  288
#define ALU_SRC_PARAM_BASE    448

#define SRC0_REL(x)        (x)
#define SRC1_REL(x)        (x)
#define SRC2_REL(x)        (x)
// elem
#define SRC0_ELEM(x)        (x)
#define SRC1_ELEM(x)        (x)
#define SRC2_ELEM(x)        (x)
#define ELEM_X        0
#define ELEM_Y        1
#define ELEM_Z        2
#define ELEM_W        3
// neg
#define SRC0_NEG(x)        (x)
#define SRC1_NEG(x)        (x)
#define SRC2_NEG(x)        (x)
// im
#define INDEX_MODE(x)    (x)		// SQ_INDEX_*
// ps
#define PRED_SEL(x)      (x)		// SQ_PRED_SEL_*
// last
#define LAST(x)          (x)
// abs
#define SRC0_ABS(x)       (x)
#define SRC1_ABS(x)       (x)
// uem
#define UPDATE_EXECUTE_MASK(x) (x)
// up
#define UPDATE_PRED(x)      (x)
// wm
#define WRITE_MASK(x)   (x)
// omod
#define OMOD(x)        (x)		// SQ_ALU_OMOD_*
// alu inst
#define ALU_INST(x)        (x)		// SQ_ALU_INST_*
//bs
#define BANK_SWIZZLE(x)        (x)	// SQ_ALU_VEC_*
#define DST_GPR(x)        (x)
#define DST_REL(x)        (x)
#define DST_ELEM(x)       (x)
#define CLAMP(x)          (x)

#define ALU_DWORD0(src0_sel, s0r, s0e, s0n, src1_sel, s1r, s1e, s1n, im, ps, last) \
    cpu_to_le32((((src0_sel) << 0) | ((s0r) << 9) | ((s0e) << 10) | ((s0n) << 12) | \
		 ((src1_sel) << 13) | ((s1r) << 22) | ((s1e) << 23) | ((s1n) << 25) | \
		 ((im) << 26) | ((ps) << 29) | ((last) << 31)))

#define ALU_DWORD1_OP2(s0a, s1a, uem, up, wm, omod, alu_inst, bs, dst_gpr, dr, de, clamp) \
    cpu_to_le32((((s0a) << 0) | ((s1a) << 1) | ((uem) << 2) | ((up) << 3) | ((wm) << 4) | \
		 ((omod) << 5) | ((alu_inst) << 7) | ((bs) << 18) | ((dst_gpr) << 21) | \
		 ((dr) << 28) | ((de) << 29) | ((clamp) << 31)))

#define ALU_DWORD1_OP3(src2_sel, s2r, s2e, s2n, alu_inst, bs, dst_gpr, dr, de, clamp) \
    cpu_to_le32((((src2_sel) << 0) | ((s2r) << 9) | ((s2e) << 10) | ((s2n) << 12) | \
		 ((alu_inst) << 13) | ((bs) << 18) | ((dst_gpr) << 21) | ((dr) << 28) | \
		 ((de) << 29) | ((clamp) << 31)))

// VTX clause insts
// vxt insts
#define VTX_INST(x)        (x)		// SQ_VTX_INST_*

// fetch type
#define FETCH_TYPE(x)        (x)	// SQ_VTX_FETCH_*

#define FETCH_WHOLE_QUAD(x)        (x)
#define BUFFER_ID(x)        (x)
#define SRC_GPR(x)          (x)
#define SRC_REL(x)          (x)

#define STRUCTURED_READ(x)    (x)
#define LDS_REQ(x)            (x)
#define COALESCED_READ(x)     (x)

#define DST_SEL_X(x)          (x)
#define DST_SEL_Y(x)          (x)
#define DST_SEL_Z(x)          (x)
#define DST_SEL_W(x)          (x)
#define USE_CONST_FIELDS(x)   (x)
#define DATA_FORMAT(x)        (x)
// num format
#define NUM_FORMAT_ALL(x)     (x)	// SQ_NUM_FORMAT_*
// format comp
#define FORMAT_COMP_ALL(x)     (x)	// SQ_FORMAT_COMP_*
// sma
#define SRF_MODE_ALL(x)     (x)
#define SRF_MODE_ZERO_CLAMP_MINUS_ONE      0
#define SRF_MODE_NO_ZERO                   1
#define OFFSET(x)     (x)
// endian swap
#define ENDIAN_SWAP(x)     (x)		// SQ_ENDIAN_*
#define CONST_BUF_NO_STRIDE(x)     (x)
#define BUFFER_INDEX_MODE(x) (x)

#define VTX_DWORD0(vtx_inst, ft, fwq, buffer_id, src_gpr, sr, ssx, ssy, str, ldsr, cr) \
    cpu_to_le32((((vtx_inst) << 0) | ((ft) << 5) | ((fwq) << 7) | ((buffer_id) << 8) | \
		 ((src_gpr) << 16) | ((sr) << 23) | ((ssx) << 24) | ((ssy) << 26) | \
		 ((str) << 28) | ((ldsr) << 30) | ((cr) << 31)))
#define VTX_DWORD1_GPR(dst_gpr, dr, dsx, dsy, dsz, dsw, ucf, data_format, nfa, fca, sma) \
    cpu_to_le32((((dst_gpr) << 0) | ((dr) << 7) | ((dsx) << 9) | ((dsy) << 12) | ((dsz) << 15) | ((dsw) << 18) | \
		 ((ucf) << 21) | ((data_format) << 22) | ((nfa) << 28) | ((fca) << 30) | ((sma) << 31)))
#define VTX_DWORD2(offset, es, cbns, alt_const, bim)			\
    cpu_to_le32((((offset) << 0) | ((es) << 16) | ((cbns) << 18) | ((alt_const) << 20) | ((bim) << 21)))
#define VTX_DWORD_PAD cpu_to_le32(0x00000000)

// TEX clause insts
// tex insts
#define TEX_INST(x)     (x)		// SQ_TEX_INST_*
#define INST_MOD(x)     (x)
#define FETCH_WHOLE_QUAD(x)     (x)
#define RESOURCE_ID(x)          (x)
#define RESOURCE_INDEX_MODE(x)          (x)
#define SAMPLER_INDEX_MODE(x)          (x)

#define LOD_BIAS(x)     (x)
//ct
#define COORD_TYPE_X(x)     (x)
#define COORD_TYPE_Y(x)     (x)
#define COORD_TYPE_Z(x)     (x)
#define COORD_TYPE_W(x)     (x)
#define TEX_UNNORMALIZED                0
#define TEX_NORMALIZED                  1
#define OFFSET_X(x) (((int)(x) * 2) & 0x1f) /* 4:1-bits 2's-complement fixed-point: [-8.0..7.5] */
#define OFFSET_Y(x) (((int)(x) * 2) & 0x1f)
#define OFFSET_Z(x) (((int)(x) * 2) & 0x1f)
#define SAMPLER_ID(x)     (x)

#define TEX_DWORD0(tex_inst, im, fwq, resource_id, src_gpr, sr, ac, rim, sim) \
    cpu_to_le32((((tex_inst) << 0) | ((im) << 5) | ((fwq) << 7) | ((resource_id) << 8) | \
		 ((src_gpr) << 16) | ((sr) << 23) | ((ac) << 24) | ((rim) << 25) | ((sim) << 27)))
#define TEX_DWORD1(dst_gpr, dr, dsx, dsy, dsz, dsw, lod_bias, ctx, cty, ctz, ctw) \
    cpu_to_le32((((dst_gpr) << 0) | ((dr) << 7) | ((dsx) << 9) | ((dsy) << 12) | ((dsz) << 15) | ((dsw) << 18) | \
		 ((lod_bias) << 21) | ((ctx) << 28) | ((cty) << 29) | ((ctz) << 30) | ((ctw) << 31)))
#define TEX_DWORD2(offset_x, offset_y, offset_z, sampler_id, ssx, ssy, ssz, ssw) \
    cpu_to_le32((((offset_x) << 0) | ((offset_y) << 5) | ((offset_z) << 10) | ((sampler_id) << 15) | \
		 ((ssx) << 20) | ((ssy) << 23) | ((ssz) << 26) | ((ssw) << 29)))
#define TEX_DWORD_PAD cpu_to_le32(0x00000000)

#endif
