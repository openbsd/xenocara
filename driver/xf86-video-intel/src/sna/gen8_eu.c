/*
 * Copyright © 2013 Intel Corporation
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "compiler.h"
#include "brw/brw.h"
#include "gen8_eu.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
#endif

/* EU ISA */

#define MRF_HACK_START 111

struct gen8_instruction {
	uint32_t data[4];
};

static inline unsigned
__gen8_mask(unsigned high, unsigned low)
{
	assert(high >= low);
	return (1 << (high - low + 1)) - 1;
}

/**
 * Fetch a set of contiguous bits from the instruction.
 *
 * Bits indexes range from 0..127; fields may not cross 32-bit boundaries.
 */
static inline unsigned
__gen8_bits(struct gen8_instruction *insn, unsigned high, unsigned low)
{
	/* We assume the field doesn't cross 32-bit boundaries. */
	const unsigned word = high / 32;

	assert(word == low / 32);

	high %= 32;
	low %= 32;

	return (insn->data[word] >> low) & __gen8_mask(high, low);
}

/**
 * Set bits in the instruction, with proper shifting and masking.
 *
 * Bits indexes range from 0..127; fields may not cross 32-bit boundaries.
 */
static inline void
__gen8_set_bits(struct gen8_instruction *insn,
		unsigned high,
		unsigned low,
		unsigned value)
{
	const unsigned word = high / 32;
	unsigned mask;

	assert(word == low / 32);

	high %= 32;
	low %= 32;
	assert(value < __gen8_mask(high, low) + 1);

	mask = __gen8_mask(high, low) << low;
	insn->data[word] &= ~mask;
	insn->data[word] |= (value << low) & mask;

	assert(__gen8_bits(insn, 32*word+high, 32*word+low) == value);
}

#define F(name, high, low) \
static inline void __gen8_set_##name(struct gen8_instruction *insn, unsigned v) \
{ \
	__gen8_set_bits(insn, high, low, v); \
} \
static inline unsigned __gen8_##name(struct gen8_instruction *insn) \
{ \
	return __gen8_bits(insn, high, low); \
}

/**
* Direct addressing only:
*  @{
*/
F(src1_da_reg_nr,      108, 101);
F(src0_da_reg_nr,       76,  69);
F(dst_da1_hstride,      62,  61);
F(dst_da_reg_nr,        60,  53);
F(dst_da16_subreg_nr,   52,  52);
F(dst_da1_subreg_nr,    52,  48);
F(da16_writemask,       51,  48); /* Dst.ChanEn */
/** @} */

F(src1_vert_stride,    120, 117)
F(src1_da1_width,      116, 114)
F(src1_da16_swiz_w,    115, 114)
F(src1_da16_swiz_z,    113, 112)
F(src1_da1_hstride,    113, 112)
F(src1_address_mode,   111, 111)
/** Src1.SrcMod @{ */
F(src1_negate,         110, 110)
F(src1_abs,            109, 109)
/** @} */
F(src1_da16_subreg_nr, 100, 100)
F(src1_da1_subreg_nr,  100,  96)
F(src1_da16_swiz_y,     99,  98)
F(src1_da16_swiz_x,     97,  96)
F(src1_reg_type,        94,  91)
F(src1_reg_file,        90,  89)
F(src0_vert_stride,     88,  85)
F(src0_da1_width,       84,  82)
F(src0_da16_swiz_w,     83,  82)
F(src0_da16_swiz_z,     81,  80)
F(src0_da1_hstride,     81,  80)
F(src0_address_mode,    79,  79)
/** Src0.SrcMod @{ */
F(src0_negate,          78,  78)
F(src0_abs,             77,  77)
/** @} */
F(src0_da16_subreg_nr,  68,  68)
F(src0_da1_subreg_nr,   68,  64)
F(src0_da16_swiz_y,     67,  66)
F(src0_da16_swiz_x,     65,  64)
F(dst_address_mode,     63,  63)
F(src0_reg_type,        46,  43)
F(src0_reg_file,        42,  41)
F(dst_reg_type,         40,  37)
F(dst_reg_file,         36,  35)
F(mask_control,         34,  34)
F(flag_reg_nr,          33,  33)
F(flag_subreg_nr,       32,  32)
F(saturate,             31,  31)
F(branch_control,       30,  30)
F(debug_control,        30,  30)
F(cmpt_control,         29,  29)
F(acc_wr_control,       28,  28)
F(cond_modifier,        27,  24)
F(exec_size,            23,  21)
F(pred_inv,             20,  20)
F(pred_control,         19,  16)
F(thread_control,       15,  14)
F(qtr_control,          13,  12)
F(nib_control,          11,  11)
F(dep_control,          10,   9)
F(access_mode,           8,   8)
/* Bit 7 is Reserved (for future Opcode expansion) */
F(opcode,                6,   0)

/**
* Three-source instructions:
*  @{
*/
F(src2_3src_reg_nr,    125, 118)
F(src2_3src_subreg_nr, 117, 115)
F(src2_3src_swizzle,   114, 107)
F(src2_3src_rep_ctrl,  106, 106)
F(src1_3src_reg_nr,    104,  97)
F(src1_3src_subreg_hi,  96,  96)
F(src1_3src_subreg_lo,  95,  94)
F(src1_3src_swizzle,    93,  86)
F(src1_3src_rep_ctrl,   85,  85)
F(src0_3src_reg_nr,     83,  76)
F(src0_3src_subreg_nr,  75,  73)
F(src0_3src_swizzle,    72,  65)
F(src0_3src_rep_ctrl,   64,  64)
F(dst_3src_reg_nr,      63,  56)
F(dst_3src_subreg_nr,   55,  53)
F(dst_3src_writemask,   52,  49)
F(dst_3src_type,        48,  46)
F(src_3src_type,        45,  43)
F(src2_3src_negate,     42,  42)
F(src2_3src_abs,        41,  41)
F(src1_3src_negate,     40,  40)
F(src1_3src_abs,        39,  39)
F(src0_3src_negate,     38,  38)
F(src0_3src_abs,        37,  37)
/** @} */

/**
* Fields for SEND messages:
*  @{
*/
F(eot,                 127, 127)
F(mlen,                124, 121)
F(rlen,                120, 116)
F(header_present,      115, 115)
F(function_control,    114,  96)
F(sfid,                 27,  24)
F(math_function,        27,  24)
/** @} */

/**
* URB message function control bits:
*  @{
*/
F(urb_per_slot_offset, 113, 113)
F(urb_interleave,      111, 111)
F(urb_global_offset,   110, 100)
F(urb_opcode,           99,  96)
/** @} */

/**
* Sampler message function control bits:
*  @{
*/
F(sampler_simd_mode,   114, 113)
F(sampler_msg_type,    112, 108)
F(sampler,             107, 104)
F(binding_table_index, 103,  96)
/** @} */

/**
 * Data port message function control bits:
 *  @ {
 */
F(dp_category,            114, 114)
F(dp_message_type,        113, 110)
F(dp_message_control,     109, 104)
F(dp_binding_table_index, 103,  96)
/** @} */

/**
 * Thread Spawn message function control bits:
 *  @ {
 */
F(ts_resource_select,     100, 100)
F(ts_request_type,         97,  97)
F(ts_opcode,               96,  96)
/** @} */

/**
 * Video Motion Estimation message function control bits:
 *  @ {
 */
F(vme_message_type,        110, 109)
F(vme_binding_table_index, 103,  96)
/** @} */

/**
 * Check & Refinement Engine message function control bits:
 *  @ {
 */
F(cre_message_type,        110, 109)
F(cre_binding_table_index, 103,  96)
/** @} */

#undef F

/**
* Flow control instruction bits:
*  @{
*/
static inline unsigned __gen8_uip(struct gen8_instruction *insn)
{
	return insn->data[2];
}

static inline void __gen8_set_uip(struct gen8_instruction *insn, unsigned uip)
{
	insn->data[2] = uip;
}

static inline unsigned __gen8_jip(struct gen8_instruction *insn)
{
	return insn->data[3];
}

static inline void __gen8_set_jip(struct gen8_instruction *insn, unsigned jip)
{
	insn->data[3] = jip;
}
/** @} */

static inline int __gen8_src1_imm_d(struct gen8_instruction *insn)
{
	return insn->data[3];
}

static inline unsigned __gen8_src1_imm_ud(struct gen8_instruction *insn)
{
	return insn->data[3];
}

static inline float __gen8_src1_imm_f(struct gen8_instruction *insn)
{
	union {
		uint32_t u;
		float f;
	} ft = { insn->data[3] };
	return ft.f;
}

static void
__gen8_set_dst(struct brw_compile *p,
	       struct gen8_instruction *inst,
	       struct brw_reg reg)
{
	/* MRFs haven't existed since Gen7, so we better not be using them. */
	if (reg.file == BRW_MESSAGE_REGISTER_FILE) {
		reg.file = BRW_GENERAL_REGISTER_FILE;
		reg.nr += MRF_HACK_START;
	}

	assert(reg.file != BRW_MESSAGE_REGISTER_FILE);

	if (reg.file == BRW_GENERAL_REGISTER_FILE)
		assert(reg.nr < BRW_MAX_GRF);

	__gen8_set_dst_reg_file(inst, reg.file);
	__gen8_set_dst_reg_type(inst, reg.type);

	assert(reg.address_mode == BRW_ADDRESS_DIRECT);

	__gen8_set_dst_da_reg_nr(inst, reg.nr);

	if (__gen8_access_mode(inst) == BRW_ALIGN_1) {
		/* Set Dst.SubRegNum[4:0] */
		__gen8_set_dst_da1_subreg_nr(inst, reg.subnr);

		/* Set Dst.HorzStride */
		if (reg.hstride == BRW_HORIZONTAL_STRIDE_0)
			reg.hstride = BRW_HORIZONTAL_STRIDE_1;
		__gen8_set_dst_da1_hstride(inst, reg.hstride);
	} else {
		/* Align16 SubRegNum only has a single bit (bit 4; bits 3:0 MBZ). */
		assert(reg.subnr == 0 || reg.subnr == 16);
		__gen8_set_dst_da16_subreg_nr(inst, reg.subnr >> 4);
		__gen8_set_da16_writemask(inst, reg.dw1.bits.writemask);
	}

#if 1
	if (reg.width == BRW_WIDTH_8 && p->compressed)
		__gen8_set_exec_size(inst, BRW_EXECUTE_16);
	else
		__gen8_set_exec_size(inst, reg.width);
#else
	if (reg.width < BRW_EXECUTE_8)
		__gen8_set_exec_size(inst, reg.width);
#endif
}

static void
__gen8_validate_reg(struct gen8_instruction *inst, struct brw_reg reg)
{
	int hstride_for_reg[] = {0, 1, 2, 4};
	int vstride_for_reg[] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 256};
	int width_for_reg[] = {1, 2, 4, 8, 16};
	int execsize_for_reg[] = {1, 2, 4, 8, 16};
	int width, hstride, vstride, execsize;

	if (reg.file == BRW_IMMEDIATE_VALUE) {
		/* TODO: check immediate vectors */
		return;
	}

	if (reg.file == BRW_ARCHITECTURE_REGISTER_FILE)
		return;

	assert(reg.hstride >= 0 && reg.hstride < ARRAY_SIZE(hstride_for_reg));
	hstride = hstride_for_reg[reg.hstride];

	if (reg.vstride == 0xf) {
		vstride = -1;
	} else {
		assert(reg.vstride >= 0 && reg.vstride < ARRAY_SIZE(vstride_for_reg));
		vstride = vstride_for_reg[reg.vstride];
	}

	assert(reg.width >= 0 && reg.width < ARRAY_SIZE(width_for_reg));
	width = width_for_reg[reg.width];

	assert(__gen8_exec_size(inst) >= 0 &&
	       __gen8_exec_size(inst) < ARRAY_SIZE(execsize_for_reg));
	execsize = execsize_for_reg[__gen8_exec_size(inst)];

	/* Restrictions from 3.3.10: Register Region Restrictions. */
	/* 3. */
	assert(execsize >= width);

	/* 4. */
	if (execsize == width && hstride != 0) {
		assert(vstride == -1 || vstride == width * hstride);
	}

	/* 5. */
	if (execsize == width && hstride == 0) {
		/* no restriction on vstride. */
	}

	/* 6. */
	if (width == 1) {
		assert(hstride == 0);
	}

	/* 7. */
	if (execsize == 1 && width == 1) {
		assert(hstride == 0);
		assert(vstride == 0);
	}

	/* 8. */
	if (vstride == 0 && hstride == 0) {
		assert(width == 1);
	}

	/* 10. Check destination issues. */
}

static void
__gen8_set_src0(struct gen8_instruction *inst, struct brw_reg reg)
{
	/* MRFs haven't existed since Gen7, so we better not be using them. */
	if (reg.file == BRW_MESSAGE_REGISTER_FILE) {
		reg.file = BRW_GENERAL_REGISTER_FILE;
		reg.nr += MRF_HACK_START;
	}

	if (reg.file == BRW_GENERAL_REGISTER_FILE)
		assert(reg.nr < BRW_MAX_GRF);

	__gen8_validate_reg(inst, reg);

	__gen8_set_src0_reg_file(inst, reg.file);
	__gen8_set_src0_reg_type(inst, reg.type);
	__gen8_set_src0_abs(inst, reg.abs);
	__gen8_set_src0_negate(inst, reg.negate);

	assert(reg.address_mode == BRW_ADDRESS_DIRECT);

	if (reg.file == BRW_IMMEDIATE_VALUE) {
		inst->data[3] = reg.dw1.ud;

		/* Required to set some fields in src1 as well: */
		__gen8_set_src1_reg_file(inst, 0); /* arf */
		__gen8_set_src1_reg_type(inst, reg.type);
	} else {
		__gen8_set_src0_da_reg_nr(inst, reg.nr);

		if (__gen8_access_mode(inst) == BRW_ALIGN_1) {
			/* Set Src0.SubRegNum[4:0] */
			__gen8_set_src0_da1_subreg_nr(inst, reg.subnr);

			if (reg.width == BRW_WIDTH_1 &&
			    __gen8_exec_size(inst) == BRW_EXECUTE_1) {
				__gen8_set_src0_da1_hstride(inst, BRW_HORIZONTAL_STRIDE_0);
				__gen8_set_src0_vert_stride(inst, BRW_VERTICAL_STRIDE_0);
			} else {
				__gen8_set_src0_da1_hstride(inst, reg.hstride);
				__gen8_set_src0_vert_stride(inst, reg.vstride);
			}
			__gen8_set_src0_da1_width(inst, reg.width);
		} else {
			/* Align16 SubRegNum only has a single bit (bit 4; bits 3:0 MBZ). */
			assert(reg.subnr == 0 || reg.subnr == 16);
			__gen8_set_src0_da16_subreg_nr(inst, reg.subnr >> 4);

			__gen8_set_src0_da16_swiz_x(inst,
						  BRW_GET_SWZ(reg.dw1.bits.swizzle,
							      BRW_CHANNEL_X));
			__gen8_set_src0_da16_swiz_y(inst,
						  BRW_GET_SWZ(reg.dw1.bits.swizzle,
							      BRW_CHANNEL_Y));
			__gen8_set_src0_da16_swiz_z(inst,
						  BRW_GET_SWZ(reg.dw1.bits.swizzle,
							      BRW_CHANNEL_Z));
			__gen8_set_src0_da16_swiz_w(inst,
						  BRW_GET_SWZ(reg.dw1.bits.swizzle,
							      BRW_CHANNEL_W));

			/* This is an oddity of the fact that we're using the same
			 * descriptions for registers in both Align16 and Align1 modes.
			 */
			if (reg.vstride == BRW_VERTICAL_STRIDE_8)
				__gen8_set_src0_vert_stride(inst, BRW_VERTICAL_STRIDE_4);
			else
				__gen8_set_src0_vert_stride(inst, reg.vstride);
		}
	}
}

static void
__gen8_set_src1(struct gen8_instruction *inst, struct brw_reg reg)
{
	/* MRFs haven't existed since Gen7, so we better not be using them. */
	if (reg.file == BRW_MESSAGE_REGISTER_FILE) {
		reg.file = BRW_GENERAL_REGISTER_FILE;
		reg.nr += MRF_HACK_START;
	}

	if (reg.file == BRW_GENERAL_REGISTER_FILE)
		assert(reg.nr < BRW_MAX_GRF);

	__gen8_validate_reg(inst, reg);

	__gen8_set_src1_reg_file(inst, reg.file);
	__gen8_set_src1_reg_type(inst, reg.type);
	__gen8_set_src1_abs(inst, reg.abs);
	__gen8_set_src1_negate(inst, reg.negate);

	/* Only src1 can be an immediate in two-argument instructions. */
	assert(__gen8_src0_reg_file(inst) != BRW_IMMEDIATE_VALUE);

	assert(reg.address_mode == BRW_ADDRESS_DIRECT);

	if (reg.file == BRW_IMMEDIATE_VALUE) {
		inst->data[3] = reg.dw1.ud;
	} else {
		__gen8_set_src1_da_reg_nr(inst, reg.nr);

		if (__gen8_access_mode(inst) == BRW_ALIGN_1) {
			/* Set Src0.SubRegNum[4:0] */
			__gen8_set_src1_da1_subreg_nr(inst, reg.subnr);

			if (reg.width == BRW_WIDTH_1 &&
			    __gen8_exec_size(inst) == BRW_EXECUTE_1) {
				__gen8_set_src1_da1_hstride(inst, BRW_HORIZONTAL_STRIDE_0);
				__gen8_set_src1_vert_stride(inst, BRW_VERTICAL_STRIDE_0);
			} else {
				__gen8_set_src1_da1_hstride(inst, reg.hstride);
				__gen8_set_src1_vert_stride(inst, reg.vstride);
			}
			__gen8_set_src1_da1_width(inst, reg.width);
		} else {
			/* Align16 SubRegNum only has a single bit (bit 4; bits 3:0 MBZ). */
			assert(reg.subnr == 0 || reg.subnr == 16);
			__gen8_set_src1_da16_subreg_nr(inst, reg.subnr >> 4);

			__gen8_set_src1_da16_swiz_x(inst,
						  BRW_GET_SWZ(reg.dw1.bits.swizzle,
							      BRW_CHANNEL_X));
			__gen8_set_src1_da16_swiz_y(inst,
						  BRW_GET_SWZ(reg.dw1.bits.swizzle,
							      BRW_CHANNEL_Y));
			__gen8_set_src1_da16_swiz_z(inst,
						  BRW_GET_SWZ(reg.dw1.bits.swizzle,
							      BRW_CHANNEL_Z));
			__gen8_set_src1_da16_swiz_w(inst,
						  BRW_GET_SWZ(reg.dw1.bits.swizzle,
							      BRW_CHANNEL_W));

			/* This is an oddity of the fact that we're using the same
			 * descriptions for registers in both Align16 and Align1 modes.
			 */
			if (reg.vstride == BRW_VERTICAL_STRIDE_8)
				__gen8_set_src1_vert_stride(inst, BRW_VERTICAL_STRIDE_4);
			else
				__gen8_set_src1_vert_stride(inst, reg.vstride);
		}
	}
}

/**
 * Set the Message Descriptor and Extended Message Descriptor fields
 * for SEND messages.
 *
 * \note This zeroes out the Function Control bits, so it must be called
 *       \b before filling out any message-specific data.  Callers can
 *       choose not to fill in irrelevant bits; they will be zero.
 */
static void
__gen8_set_message_descriptor(struct gen8_instruction *inst,
			      enum brw_message_target sfid,
			      unsigned msg_length,
			      unsigned response_length,
			      bool header_present,
			      bool end_of_thread)
{
	__gen8_set_src1(inst, brw_imm_d(0));

	__gen8_set_sfid(inst, sfid);
	__gen8_set_mlen(inst, msg_length);
	__gen8_set_rlen(inst, response_length);
	__gen8_set_header_present(inst, header_present);
	__gen8_set_eot(inst, end_of_thread);
}

#if 0
static void
__gen8_set_urb_message(struct gen8_instruction *inst,
		       unsigned opcode,
		       unsigned msg_length,
		       unsigned response_length,
		       bool end_of_thread,
		       unsigned offset,
		       bool interleave)
{
	__gen8_set_message_descriptor(inst, BRW_SFID_URB, msg_length, response_length,
				      true, end_of_thread);
	__gen8_set_src0(inst, brw_vec8_grf(MRF_HACK_START + 1, 0));
	__gen8_set_urb_opcode(inst, 0); /* URB_WRITE_HWORD */
	__gen8_set_urb_global_offset(inst, offset);
	__gen8_set_urb_interleave(inst, interleave);
	/* per_slot_offset = 0 makes it ignore offsets in message header */
	__gen8_set_urb_per_slot_offset(inst, 0);
}
#endif

static void
__gen8_set_sampler_message(struct gen8_instruction *inst,
			   unsigned binding_table_index,
			   unsigned sampler,
			   unsigned msg_type,
			   unsigned response_length,
			   unsigned msg_length,
			   bool header_present,
			   unsigned simd_mode)
{
	__gen8_set_message_descriptor(inst, BRW_SFID_SAMPLER, msg_length,
				      response_length, header_present, false);

	__gen8_set_binding_table_index(inst, binding_table_index);
	__gen8_set_sampler(inst, sampler);
	__gen8_set_sampler_msg_type(inst, msg_type);
	__gen8_set_sampler_simd_mode(inst, simd_mode);
}

static void
__gen8_set_dp_message(struct gen8_instruction *inst,
		      enum brw_message_target sfid,
		      unsigned binding_table_index,
		      unsigned msg_type,
		      unsigned msg_control,
		      unsigned mlen,
		      unsigned rlen,
		      bool header_present,
		      bool end_of_thread)
{
	/* Binding table index is from 0..255 */
	assert((binding_table_index & 0xff) == binding_table_index);

	/* Message Type is only 5 bits */
	assert((msg_type & 0x1f) == msg_type);

	/* Message Control is only 6 bits */
	assert((msg_control & 0x3f) == msg_control);

	__gen8_set_message_descriptor(inst, sfid, mlen, rlen, header_present,
				      end_of_thread);
	__gen8_set_function_control(inst,
				    binding_table_index | msg_type << 14 | msg_control << 8);
}

static inline struct gen8_instruction *
gen8_next_insn(struct brw_compile *p, int opcode)
{
	struct gen8_instruction *insn;

	assert(p->nr_insn + 1 < BRW_EU_MAX_INSN);

	insn = memcpy(&p->store[p->nr_insn++], p->current, sizeof(*insn));
	__gen8_set_opcode(insn, opcode);

	return insn;
}

static void gen8_math(struct brw_compile *p,
		      struct brw_reg dst,
		      unsigned function,
		      unsigned saturate,
		      unsigned msg_reg_nr,
		      struct brw_reg src,
		      unsigned data_type,
		      unsigned precision)
{
	struct gen8_instruction *insn = gen8_next_insn(p, BRW_OPCODE_MATH);

	assert(dst.file == BRW_GENERAL_REGISTER_FILE);
	assert(src.file == BRW_GENERAL_REGISTER_FILE);

	assert(dst.hstride == BRW_HORIZONTAL_STRIDE_1);
	assert(src.hstride == BRW_HORIZONTAL_STRIDE_1);

	/* Source modifiers are ignored for extended math instructions. */
	assert(!src.negate);
	assert(!src.abs);

	if (function != BRW_MATH_FUNCTION_INT_DIV_QUOTIENT &&
	    function != BRW_MATH_FUNCTION_INT_DIV_QUOTIENT_AND_REMAINDER) {
		assert(src.type == BRW_REGISTER_TYPE_F);
	}

	/* Math is the same ISA format as other opcodes, except that CondModifier
	 * becomes FC[3:0] and ThreadCtrl becomes FC[5:4].
	 */
	__gen8_set_cond_modifier(insn, function);
	__gen8_set_saturate(insn, saturate);

	__gen8_set_dst(p, insn, dst);
	__gen8_set_src0(insn, src);
	__gen8_set_src1(insn, brw_null_reg());
}

static inline void gen8_math_invert(struct brw_compile *p,
				    struct brw_reg dst,
				    struct brw_reg src)
{
	gen8_math(p,
		  dst,
		  BRW_MATH_FUNCTION_INV,
		  BRW_MATH_SATURATE_NONE,
		  0,
		  src,
		  BRW_MATH_PRECISION_FULL,
		  BRW_MATH_DATA_VECTOR);

}

/* Helpers for regular instructions: */
static inline struct gen8_instruction *gen8_alu1(struct brw_compile *p,
						 unsigned opcode,
						 struct brw_reg dst,
						 struct brw_reg src)
{
	struct gen8_instruction *insn = gen8_next_insn(p, opcode);
	__gen8_set_dst(p, insn, dst);
	__gen8_set_src0(insn, src);
	return insn;
}

static inline struct gen8_instruction *gen8_alu2(struct brw_compile *p,
						 unsigned opcode,
						 struct brw_reg dst,
						 struct brw_reg src0,
						 struct brw_reg src1)
{
	struct gen8_instruction *insn = gen8_next_insn(p, opcode);
	__gen8_set_dst(p, insn, dst);
	__gen8_set_src0(insn, src0);
	__gen8_set_src1(insn, src1);
	return insn;
}

#define ALU1(OP)							\
static inline struct gen8_instruction *gen8_##OP(struct brw_compile *p,	\
						 struct brw_reg dst,	\
						 struct brw_reg src0)	\
{									\
   return gen8_alu1(p, BRW_OPCODE_##OP, dst, src0);			\
}

#define ALU2(OP)							\
static inline struct gen8_instruction *gen8_##OP(struct brw_compile *p,	\
						 struct brw_reg dst,	\
						 struct brw_reg src0,	\
						 struct brw_reg src1)	\
{									\
   return gen8_alu2(p, BRW_OPCODE_##OP, dst, src0, src1);		\
}

static inline struct gen8_instruction *gen8_ADD(struct brw_compile *p,
						struct brw_reg dst,
						struct brw_reg src0,
						struct brw_reg src1)
{
	/* 6.2.2: add */
	if (src0.type == BRW_REGISTER_TYPE_F ||
	    (src0.file == BRW_IMMEDIATE_VALUE &&
	     src0.type == BRW_REGISTER_TYPE_VF)) {
		assert(src1.type != BRW_REGISTER_TYPE_UD);
		assert(src1.type != BRW_REGISTER_TYPE_D);
	}

	if (src1.type == BRW_REGISTER_TYPE_F ||
	    (src1.file == BRW_IMMEDIATE_VALUE &&
	     src1.type == BRW_REGISTER_TYPE_VF)) {
		assert(src0.type != BRW_REGISTER_TYPE_UD);
		assert(src0.type != BRW_REGISTER_TYPE_D);
	}

	return gen8_alu2(p, BRW_OPCODE_ADD, dst, src0, src1);
}

static inline struct gen8_instruction *gen8_MUL(struct brw_compile *p,
						struct brw_reg dst,
						struct brw_reg src0,
						struct brw_reg src1)
{
	/* 6.32.38: mul */
	if (src0.type == BRW_REGISTER_TYPE_D ||
	    src0.type == BRW_REGISTER_TYPE_UD ||
	    src1.type == BRW_REGISTER_TYPE_D ||
	    src1.type == BRW_REGISTER_TYPE_UD) {
		assert(dst.type != BRW_REGISTER_TYPE_F);
	}

	if (src0.type == BRW_REGISTER_TYPE_F ||
	    (src0.file == BRW_IMMEDIATE_VALUE &&
	     src0.type == BRW_REGISTER_TYPE_VF)) {
		assert(src1.type != BRW_REGISTER_TYPE_UD);
		assert(src1.type != BRW_REGISTER_TYPE_D);
	}

	if (src1.type == BRW_REGISTER_TYPE_F ||
	    (src1.file == BRW_IMMEDIATE_VALUE &&
	     src1.type == BRW_REGISTER_TYPE_VF)) {
		assert(src0.type != BRW_REGISTER_TYPE_UD);
		assert(src0.type != BRW_REGISTER_TYPE_D);
	}

	assert(src0.file != BRW_ARCHITECTURE_REGISTER_FILE ||
	       src0.nr != BRW_ARF_ACCUMULATOR);
	assert(src1.file != BRW_ARCHITECTURE_REGISTER_FILE ||
	       src1.nr != BRW_ARF_ACCUMULATOR);

	return gen8_alu2(p, BRW_OPCODE_MUL, dst, src0, src1);
}

ALU1(MOV);
ALU2(SEL);
ALU1(NOT);
ALU2(AND);
ALU2(OR);
ALU2(XOR);
ALU2(SHR);
ALU2(SHL);
ALU2(RSR);
ALU2(RSL);
ALU2(ASR);
ALU1(FRC);
ALU1(RNDD);
ALU2(MAC);
ALU2(MACH);
ALU1(LZD);
ALU2(DP4);
ALU2(DPH);
ALU2(DP3);
ALU2(DP2);
ALU2(LINE);
ALU2(PLN);

ALU1(RNDZ);
ALU1(RNDE);

#undef ALU1
#undef ALU2

static void gen8_set_compression_control(struct brw_compile *p,
					 enum brw_compression compression_control)
{
	unsigned v;

	p->compressed = compression_control == BRW_COMPRESSION_COMPRESSED;

	switch (compression_control) {
	default: assert(0);
	case BRW_COMPRESSION_NONE:       v = GEN6_COMPRESSION_1Q; break;
	case BRW_COMPRESSION_2NDHALF:    v = GEN6_COMPRESSION_2Q; break;
	case BRW_COMPRESSION_COMPRESSED: v = GEN6_COMPRESSION_1H; break;
	}
	__gen8_set_cmpt_control((struct gen8_instruction *)p->current, v);
}

static inline void gen8_set_mask_control(struct brw_compile *p, unsigned value)
{
	__gen8_set_mask_control((struct gen8_instruction *)p->current, value);
}

static inline void gen8_set_saturate(struct brw_compile *p, unsigned value)
{
	__gen8_set_saturate((struct gen8_instruction *)p->current, value);
}

static inline void gen8_set_acc_write_control(struct brw_compile *p, unsigned value)
{
	__gen8_set_acc_wr_control((struct gen8_instruction *)p->current, value);
}

static void gen8_SAMPLE(struct brw_compile *p,
			struct brw_reg dst,
			unsigned msg_reg_nr,
			unsigned binding_table_index,
			unsigned sampler,
			unsigned writemask,
			unsigned msg_type,
			unsigned response_length,
			unsigned msg_length,
			bool header_present,
			unsigned simd_mode)
{
	struct brw_reg src0 = brw_message_reg(msg_reg_nr);

	assert(writemask);

	if (writemask != WRITEMASK_XYZW) {
		writemask = ~writemask & WRITEMASK_XYZW;

		brw_push_insn_state(p);

		gen8_set_compression_control(p, BRW_COMPRESSION_NONE);
		gen8_set_mask_control(p, BRW_MASK_DISABLE);

		gen8_MOV(p, __retype_ud(src0), __retype_ud(brw_vec8_grf(0,0)));
		gen8_MOV(p, get_element_ud(src0, 2), brw_imm_ud(writemask << 12));

		brw_pop_insn_state(p);
	}

	{
		struct gen8_instruction *insn;

		insn = gen8_next_insn(p, BRW_OPCODE_SEND);
		__gen8_set_pred_control(insn, 0); /* XXX */
		__gen8_set_cmpt_control(insn, GEN6_COMPRESSION_1Q);

		__gen8_set_dst(p, insn, dst);
		__gen8_set_src0(insn, src0);
		__gen8_set_sampler_message(insn,
					   binding_table_index,
					   sampler,
					   msg_type,
					   response_length,
					   msg_length,
					   header_present,
					   simd_mode);
	}
}

/* shader logic */

static void wm_affine_st(struct brw_compile *p, int dw, int channel, int msg)
{
	int uv;

	if (dw == 16) {
		gen8_set_compression_control(p, BRW_COMPRESSION_COMPRESSED);
		uv = 6;
	} else {
		gen8_set_compression_control(p, BRW_COMPRESSION_NONE);
		uv = 4;
	}
	uv += 2*channel;

	msg++;
	gen8_PLN(p,
		 brw_message_reg(msg),
		 brw_vec1_grf(uv, 0),
		 brw_vec8_grf(2, 0));
	msg += dw/8;

	gen8_PLN(p,
		 brw_message_reg(msg),
		 brw_vec1_grf(uv, 4),
		 brw_vec8_grf(2, 0));
}

static inline unsigned simd(int dw)
{
	return dw == 16 ? BRW_SAMPLER_SIMD_MODE_SIMD16 : BRW_SAMPLER_SIMD_MODE_SIMD8;
}

static inline struct brw_reg sample_result(int dw, int result)
{
	return brw_reg(BRW_GENERAL_REGISTER_FILE, result, 0,
		       BRW_REGISTER_TYPE_UW,
		       dw == 16 ? BRW_VERTICAL_STRIDE_16 : BRW_VERTICAL_STRIDE_8,
		       dw == 16 ? BRW_WIDTH_16 : BRW_WIDTH_8,
		       BRW_HORIZONTAL_STRIDE_1,
		       BRW_SWIZZLE_XYZW,
		       WRITEMASK_XYZW);
}

static int wm_sample(struct brw_compile *p, int dw,
		     int channel, int msg, int result)
{
	int len = dw == 16 ? 4 : 2;
	gen8_SAMPLE(p, sample_result(dw, result), ++msg,
		    channel+1, channel, WRITEMASK_XYZW, 0,
		    2*len, len, false, simd(dw));
	return result;
}

static int wm_sample__alpha(struct brw_compile *p, int dw,
			    int channel, int msg, int result)
{
	int mlen, rlen;

	if (dw == 8) {
		mlen = 3;
		rlen = 1;
	} else {
		mlen = 5;
		rlen = 2;
	}

	gen8_SAMPLE(p, sample_result(dw, result), msg,
		    channel+1, channel, WRITEMASK_W, 0,
		    rlen, mlen, true, simd(dw));

	return result;
}

static int wm_affine(struct brw_compile *p, int dw,
		     int channel, int msg, int result)
{
	wm_affine_st(p, dw, channel, msg);
	return wm_sample(p, dw, channel, msg, result);
}

static int wm_affine__alpha(struct brw_compile *p, int dw,
			    int channel, int msg, int result)
{
	wm_affine_st(p, dw, channel, msg);
	return wm_sample__alpha(p, dw, channel, msg, result);
}

static inline struct brw_reg null_result(int dw)
{
	return brw_reg(BRW_ARCHITECTURE_REGISTER_FILE, BRW_ARF_NULL, 0,
		       BRW_REGISTER_TYPE_UW,
		       dw == 16 ? BRW_VERTICAL_STRIDE_16 : BRW_VERTICAL_STRIDE_8,
		       dw == 16 ? BRW_WIDTH_16 : BRW_WIDTH_8,
		       BRW_HORIZONTAL_STRIDE_1,
		       BRW_SWIZZLE_XYZW,
		       WRITEMASK_XYZW);
}

static void fb_write(struct brw_compile *p, int dw)
{
	struct gen8_instruction *insn;
	unsigned msg_control, msg_len;
	struct brw_reg src0;

	if (dw == 16) {
		msg_control = BRW_DATAPORT_RENDER_TARGET_WRITE_SIMD16_SINGLE_SOURCE;
		msg_len = 8;
	} else {
		msg_control = BRW_DATAPORT_RENDER_TARGET_WRITE_SIMD8_SINGLE_SOURCE_SUBSPAN01;
		msg_len = 4;
	}
	msg_control |= 1 << 4; /* Last Render Target */

	/* The execution mask is ignored for render target writes. */
	insn = gen8_next_insn(p, BRW_OPCODE_SEND);
	__gen8_set_pred_control(insn, 0);
	__gen8_set_cmpt_control(insn, GEN6_COMPRESSION_1Q);

	src0 = brw_message_reg(2);

	__gen8_set_dst(p, insn, null_result(dw));
	__gen8_set_src0(insn, src0);
	__gen8_set_dp_message(insn,
			      GEN6_SFID_DATAPORT_RENDER_CACHE,
			      0,
			      GEN6_DATAPORT_WRITE_MESSAGE_RENDER_TARGET_WRITE,
			      msg_control,
			      msg_len, 0,
			      false, true);
}

static void wm_write(struct brw_compile *p, int dw, int src)
{
	int n;

	if (dw == 8) {
		/* XXX pixel execution mask? */
		gen8_set_compression_control(p, BRW_COMPRESSION_NONE);
		for (n = 0; n < 4; n++)
			gen8_MOV(p,
				 brw_message_reg(2 + n),
				 brw_vec8_grf(src + n, 0));
	} else {
		gen8_set_compression_control(p, BRW_COMPRESSION_COMPRESSED);
		for (n = 0; n < 4; n++)
			gen8_MOV(p,
				 brw_message_reg(2 + 2*n),
				 brw_vec8_grf(src + 2*n, 0));
	}

	fb_write(p, dw);
}

static void wm_write__mask(struct brw_compile *p, int dw,
			   int src, int mask)
{
	int n;

	if (dw == 8) {
		gen8_set_compression_control(p, BRW_COMPRESSION_NONE);
		for (n = 0; n < 4; n++)
			gen8_MUL(p,
				 brw_message_reg(2 + n),
				 brw_vec8_grf(src + n, 0),
				 brw_vec8_grf(mask, 0));
	} else {
		gen8_set_compression_control(p, BRW_COMPRESSION_COMPRESSED);
		for (n = 0; n < 4; n++)
			gen8_MUL(p,
				 brw_message_reg(2 + 2*n),
				 brw_vec8_grf(src + 2*n, 0),
				 brw_vec8_grf(mask, 0));
	}

	fb_write(p, dw);
}

static void wm_write__opacity(struct brw_compile *p, int dw, int src, int mask)
{
	int n;

	if (dw == 8) {
		gen8_set_compression_control(p, BRW_COMPRESSION_NONE);
		for (n = 0; n < 4; n++)
			gen8_MUL(p,
				 brw_message_reg(2 + n),
				 brw_vec8_grf(src + n, 0),
				 brw_vec1_grf(mask, 3));
	} else {
		gen8_set_compression_control(p, BRW_COMPRESSION_COMPRESSED);
		for (n = 0; n < 4; n++)
			gen8_MUL(p,
				 brw_message_reg(2 + 2*n),
				 brw_vec8_grf(src + 2*n, 0),
				 brw_vec1_grf(mask, 3));
	}

	fb_write(p, dw);
}

static void wm_write__mask_ca(struct brw_compile *p, int dw,
			      int src, int mask)
{
	int n;

	if (dw == 8) {
		gen8_set_compression_control(p, BRW_COMPRESSION_NONE);
		for (n = 0; n < 4; n++)
			gen8_MUL(p,
				 brw_message_reg(2 + n),
				 brw_vec8_grf(src + n, 0),
				 brw_vec8_grf(mask + n, 0));
	} else {
		gen8_set_compression_control(p, BRW_COMPRESSION_COMPRESSED);
		for (n = 0; n < 4; n++)
			gen8_MUL(p,
				 brw_message_reg(2 + 2*n),
				 brw_vec8_grf(src + 2*n, 0),
				 brw_vec8_grf(mask + 2*n, 0));
	}

	fb_write(p, dw);
}

static void gen8_compile_init(struct brw_compile *p)
{
	struct gen8_instruction *insn = memset(p->current, 0, sizeof(*insn));
	COMPILE_TIME_ASSERT(sizeof(*insn) == sizeof(*p->current));
	__gen8_set_mask_control(insn, BRW_MASK_ENABLE);
	__gen8_set_saturate(insn, 0);
	__gen8_set_cmpt_control(insn, GEN6_COMPRESSION_1Q);
	//__gen8_set_pred_control(insn, 0xf);
}

bool
gen8_wm_kernel__affine(struct brw_compile *p, int dispatch)
{
	gen8_compile_init(p);

	wm_write(p, dispatch, wm_affine(p, dispatch, 0, 1, 12));
	return true;
}

bool
gen8_wm_kernel__affine_mask(struct brw_compile *p, int dispatch)
{
	int src, mask;

	gen8_compile_init(p);

	src = wm_affine(p, dispatch, 0, 1, 12);
	mask = wm_affine__alpha(p, dispatch, 1, 6, 20);
	wm_write__mask(p, dispatch, src, mask);

	return true;
}

bool
gen8_wm_kernel__affine_mask_ca(struct brw_compile *p, int dispatch)
{
	int src, mask;

	gen8_compile_init(p);

	src = wm_affine(p, dispatch, 0, 1, 12);
	mask = wm_affine(p, dispatch, 1, 6, 20);
	wm_write__mask_ca(p, dispatch, src, mask);

	return true;
}

bool
gen8_wm_kernel__affine_mask_sa(struct brw_compile *p, int dispatch)
{
	int src, mask;

	gen8_compile_init(p);

	src = wm_affine__alpha(p, dispatch, 0, 1, 12);
	mask = wm_affine(p, dispatch, 1, 6, 16);
	wm_write__mask(p, dispatch, mask, src);

	return true;
}

/* Projective variants */

static void wm_projective_st(struct brw_compile *p, int dw,
			     int channel, int msg)
{
	int uv;

	gen8_compile_init(p);

	if (dw == 16) {
		gen8_set_compression_control(p, BRW_COMPRESSION_COMPRESSED);
		uv = 6;
	} else {
		gen8_set_compression_control(p, BRW_COMPRESSION_NONE);
		uv = 4;
	}
	uv += 2*channel;

	msg++;
	/* First compute 1/z */
	gen8_PLN(p,
		 brw_vec8_grf(30, 0),
		 brw_vec1_grf(uv+1, 0),
		 brw_vec8_grf(2, 0));

	if (dw == 16) {
		gen8_set_compression_control(p, BRW_COMPRESSION_NONE);
		gen8_math_invert(p, brw_vec8_grf(30, 0), brw_vec8_grf(30, 0));
		gen8_math_invert(p, brw_vec8_grf(31, 0), brw_vec8_grf(31, 0));
		gen8_set_compression_control(p, BRW_COMPRESSION_COMPRESSED);
	} else
		gen8_math_invert(p, brw_vec8_grf(30, 0), brw_vec8_grf(30, 0));

	gen8_PLN(p,
		 brw_vec8_grf(26, 0),
		 brw_vec1_grf(uv, 0),
		 brw_vec8_grf(2, 0));
	gen8_PLN(p,
		 brw_vec8_grf(28, 0),
		 brw_vec1_grf(uv, 0),
		 brw_vec8_grf(4, 0));

	gen8_MUL(p,
		 brw_message_reg(msg),
		 brw_vec8_grf(26, 0),
		 brw_vec8_grf(30, 0));
	gen8_MUL(p,
		 brw_message_reg(msg + dw/8),
		 brw_vec8_grf(28, 0),
		 brw_vec8_grf(30, 0));
}

static int wm_projective(struct brw_compile *p, int dw,
			 int channel, int msg, int result)
{
	gen8_compile_init(p);

	wm_projective_st(p, dw, channel, msg);
	return wm_sample(p, dw, channel, msg, result);
}

static int wm_projective__alpha(struct brw_compile *p, int dw,
				int channel, int msg, int result)
{
	gen8_compile_init(p);

	wm_projective_st(p, dw, channel, msg);
	return wm_sample__alpha(p, dw, channel, msg, result);
}

bool
gen8_wm_kernel__projective(struct brw_compile *p, int dispatch)
{
	gen8_compile_init(p);

	wm_write(p, dispatch, wm_projective(p, dispatch, 0, 1, 12));
	return true;
}

bool
gen8_wm_kernel__projective_mask(struct brw_compile *p, int dispatch)
{
	int src, mask;

	gen8_compile_init(p);

	src = wm_projective(p, dispatch, 0, 1, 12);
	mask = wm_projective__alpha(p, dispatch, 1, 6, 20);
	wm_write__mask(p, dispatch, src, mask);

	return true;
}

bool
gen8_wm_kernel__projective_mask_ca(struct brw_compile *p, int dispatch)
{
	int src, mask;

	gen8_compile_init(p);

	src = wm_projective(p, dispatch, 0, 1, 12);
	mask = wm_projective(p, dispatch, 1, 6, 20);
	wm_write__mask_ca(p, dispatch, src, mask);

	return true;
}

bool
gen8_wm_kernel__projective_mask_sa(struct brw_compile *p, int dispatch)
{
	int src, mask;

	gen8_compile_init(p);

	src = wm_projective__alpha(p, dispatch, 0, 1, 12);
	mask = wm_projective(p, dispatch, 1, 6, 16);
	wm_write__mask(p, dispatch, mask, src);

	return true;
}

bool
gen8_wm_kernel__affine_opacity(struct brw_compile *p, int dispatch)
{
	int src, mask;

	gen8_compile_init(p);

	src = wm_affine(p, dispatch, 0, 1, 12);
	mask = dispatch == 16 ? 8 : 6;
	wm_write__opacity(p, dispatch, src, mask);

	return true;
}

bool
gen8_wm_kernel__projective_opacity(struct brw_compile *p, int dispatch)
{
	int src, mask;

	gen8_compile_init(p);

	mask = dispatch == 16 ? 8 : 6;
	src = wm_projective(p, dispatch, 0, 1, 12);
	wm_write__opacity(p, dispatch, src, mask);

	return true;
}
