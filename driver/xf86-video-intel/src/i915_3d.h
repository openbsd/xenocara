/* -*- c-basic-offset: 4 -*- */
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
 *    Eric Anholt <eric@anholt.net>
 *
 */

/* MASK_* are the unshifted bitmasks of the destination mask in arithmetic
 * operations
 */
#define MASK_X			0x1
#define MASK_Y			0x2
#define MASK_Z			0x4
#define MASK_W			0x8
#define MASK_XYZ		(MASK_X | MASK_Y | MASK_Z)
#define MASK_XYZW		(MASK_XYZ | MASK_W)
#define MASK_SATURATE		0x10

/* Temporary, undeclared regs. Preserved between phases */
#define FS_R0			((REG_TYPE_R << 8) | 0)
#define FS_R1			((REG_TYPE_R << 8) | 1)
#define FS_R2			((REG_TYPE_R << 8) | 2)
#define FS_R3			((REG_TYPE_R << 8) | 3)

/* Texture coordinate regs.  Must be declared. */
#define FS_T0			((REG_TYPE_T << 8) | 0)
#define FS_T1			((REG_TYPE_T << 8) | 1)
#define FS_T2			((REG_TYPE_T << 8) | 2)
#define FS_T3			((REG_TYPE_T << 8) | 3)
#define FS_T4			((REG_TYPE_T << 8) | 4)
#define FS_T5			((REG_TYPE_T << 8) | 5)
#define FS_T6			((REG_TYPE_T << 8) | 6)
#define FS_T7			((REG_TYPE_T << 8) | 7)
#define FS_T8			((REG_TYPE_T << 8) | 8)
#define FS_T9			((REG_TYPE_T << 8) | 9)
#define FS_T10			((REG_TYPE_T << 8) | 10)

/* Constant values */
#define FS_C0			((REG_TYPE_CONST << 8) | 0)
#define FS_C1			((REG_TYPE_CONST << 8) | 1)
#define FS_C2			((REG_TYPE_CONST << 8) | 2)
#define FS_C3			((REG_TYPE_CONST << 8) | 3)
#define FS_C4			((REG_TYPE_CONST << 8) | 4)
#define FS_C5			((REG_TYPE_CONST << 8) | 5)
#define FS_C6			((REG_TYPE_CONST << 8) | 6)
#define FS_C7			((REG_TYPE_CONST << 8) | 7)

/* Sampler regs */
#define FS_S0			((REG_TYPE_S << 8) | 0)
#define FS_S1			((REG_TYPE_S << 8) | 1)
#define FS_S2			((REG_TYPE_S << 8) | 2)
#define FS_S3			((REG_TYPE_S << 8) | 3)

/* Output color */
#define FS_OC			((REG_TYPE_OC << 8) | 0)

/* Output depth */
#define FS_OD			((REG_TYPE_OD << 8) | 0)

/* Unpreserved temporary regs */
#define FS_U0			((REG_TYPE_U << 8) | 0)
#define FS_U1			((REG_TYPE_U << 8) | 1)
#define FS_U2			((REG_TYPE_U << 8) | 2)
#define FS_U3			((REG_TYPE_U << 8) | 3)

#define REG_TYPE(reg)		((reg) >> 8)
#define REG_NR(reg)		((reg) & 0xff)

struct i915_fs_op {
    uint32_t ui[3];
};

#define X_CHANNEL_VAL		1
#define Y_CHANNEL_VAL		2
#define Z_CHANNEL_VAL		3
#define W_CHANNEL_VAL		4
#define ZERO_CHANNEL_VAL	5
#define ONE_CHANNEL_VAL		6

/**
 * This structure represents the contents of an operand to an i915 fragment
 * shader.
 *
 * It is not a hardware representation, though closely related.
 */
struct i915_fs_operand {
    /**< REG_TYPE_* register type */
    int reg;
    /**< *_CHANNEL_VAL swizzle value, with optional negation */
    int x;
    /**< *_CHANNEL_VAL swizzle value, with optional negation */
    int y;
    /**< *_CHANNEL_VAL swizzle value, with optional negation */
    int z;
    /**< *_CHANNEL_VAL swizzle value, with optional negation */
    int w;
};

/**
 * Construct an operand description for the fragment shader.
 *
 * \param regtype FS_* register used as the source value for X/Y/Z/W sources.
 * \param x *_CHANNEL_VAL swizzle value prefix for operand X channel, with
 *          optional negation.
 * \param y *_CHANNEL_VAL swizzle value prefix for operand Y channel, with
 *          optional negation.
 * \param z *_CHANNEL_VAL swizzle value prefix for operand Z channel, with
 *          optional negation.
 * \param w *_CHANNEL_VAL swizzle value prefix for operand W channel, with
 *          optional negation.
 */
#define i915_fs_operand(reg, x, y, z, w)				\
    _i915_fs_operand(reg,						\
		     x##_CHANNEL_VAL, y##_CHANNEL_VAL,			\
		     z##_CHANNEL_VAL, w##_CHANNEL_VAL)

/**
 * Construct an oeprand description for using a register with no swizzling
 */
#define i915_fs_operand_reg(reg)					\
    i915_fs_operand(reg, X, Y, Z, W)

static inline struct i915_fs_operand
_i915_fs_operand(int reg, int x, int y, int z, int w)
{
    struct i915_fs_operand operand;

    operand.reg = reg;
    operand.x = x;
    operand.y = y;
    operand.z = z;
    operand.w = w;

    return operand;
}

/**
 * Returns an operand containing (0.0, 0.0, 0.0, 0.0).
 */
static inline struct i915_fs_operand
i915_fs_operand_zero(void)
{
    return i915_fs_operand(FS_R0, ZERO, ZERO, ZERO, ZERO);
}

/**
 * Returns an unused operand
 */
#define i915_fs_operand_none() i915_fs_operand_zero()

/**
 * Returns an operand containing (1.0, 1.0, 1.0, 1.0).
 */
static inline struct i915_fs_operand
i915_fs_operand_one(void)
{
    return i915_fs_operand(FS_R0, ONE, ONE, ONE, ONE);
}

static inline int
i915_get_hardware_channel_val(int channel_val)
{
    if (channel_val < 0)
	channel_val = -channel_val;

    switch (channel_val) {
    case X_CHANNEL_VAL:
	return SRC_X;
    case Y_CHANNEL_VAL:
	return SRC_Y;
    case Z_CHANNEL_VAL:
	return SRC_Z;
    case W_CHANNEL_VAL:
	return SRC_W;
    case ZERO_CHANNEL_VAL:
	return SRC_ZERO;
    case ONE_CHANNEL_VAL:
	return SRC_ONE;
    }
    FatalError("Bad channel value %d\n", channel_val);
}

/**
 * Outputs a fragment shader command to declare a sampler or texture register.
 */
#define i915_fs_dcl(reg)						\
do {									\
    FS_OUT(_i915_fs_dcl(reg));						\
} while (0)

/**
 * Constructs a fragment shader command to declare a sampler or texture
 * register.
 */
static inline struct i915_fs_op
_i915_fs_dcl(int reg)
{
    struct i915_fs_op op;

    op.ui[0] = D0_DCL | (REG_TYPE(reg) << D0_TYPE_SHIFT) |
	(REG_NR(reg) << D0_NR_SHIFT);
    op.ui[1] = 0;
    op.ui[2] = 0;
    if (REG_TYPE(reg) != REG_TYPE_S)
	op.ui[0] |= D0_CHANNEL_ALL;

    return op;
}

/**
 * Constructs a fragment shader command to load from a texture sampler.
 */
#define i915_fs_texld(dest_reg, sampler_reg, address_reg)		\
do {									\
     FS_OUT(_i915_fs_texld(T0_TEXLD, dest_reg, sampler_reg, address_reg)); \
} while (0)

#define i915_fs_texldp(dest_reg, sampler_reg, address_reg)		\
do {									\
     FS_OUT(_i915_fs_texld(T0_TEXLDP, dest_reg, sampler_reg, address_reg)); \
} while (0)

static inline struct i915_fs_op
_i915_fs_texld(int load_op, int dest_reg, int sampler_reg, int address_reg)
{
    struct i915_fs_op op;

    op.ui[0] = 0;
    op.ui[1] = 0;
    op.ui[2] = 0;

    if (REG_TYPE(sampler_reg) != REG_TYPE_S)
	FatalError("Bad sampler reg type\n");

    op.ui[0] |= load_op;
    op.ui[0] |= REG_TYPE(dest_reg) << T0_DEST_TYPE_SHIFT;
    op.ui[0] |= REG_NR(dest_reg) << T0_DEST_NR_SHIFT;
    op.ui[0] |= REG_NR(sampler_reg) << T0_SAMPLER_NR_SHIFT;
    op.ui[1] |= REG_TYPE(address_reg) << T1_ADDRESS_REG_TYPE_SHIFT;
    op.ui[1] |= REG_NR(address_reg) << T1_ADDRESS_REG_NR_SHIFT;

    return op;
}

#define i915_fs_arith(op, dest_reg, operand0, operand1, operand2)	\
    _i915_fs_arith(A0_##op, dest_reg, operand0, operand1, operand2)

static inline struct i915_fs_op
_i915_fs_arith(int cmd, int dest_reg,
	       struct i915_fs_operand operand0,
	       struct i915_fs_operand operand1,
	       struct i915_fs_operand operand2)
{
    struct i915_fs_op op;

    op.ui[0] = 0;
    op.ui[1] = 0;
    op.ui[2] = 0;

    /* Set up destination register and write mask */
    op.ui[0] |= cmd;
    op.ui[0] |= REG_TYPE(dest_reg) << A0_DEST_TYPE_SHIFT;
    op.ui[0] |= REG_NR(dest_reg) << A0_DEST_NR_SHIFT;
    op.ui[0] |= A0_DEST_CHANNEL_ALL;

    /* Set up operand 0 */
    op.ui[0] |= REG_TYPE(operand0.reg) << A0_SRC0_TYPE_SHIFT;
    op.ui[0] |= REG_NR(operand0.reg) << A0_SRC0_NR_SHIFT;

    op.ui[1] |= i915_get_hardware_channel_val(operand0.x) <<
	A1_SRC0_CHANNEL_X_SHIFT;
    if (operand0.x < 0)
	op.ui[1] |= A1_SRC0_CHANNEL_X_NEGATE;

    op.ui[1] |= i915_get_hardware_channel_val(operand0.y) <<
	A1_SRC0_CHANNEL_Y_SHIFT;
    if (operand0.y < 0)
	op.ui[1] |= A1_SRC0_CHANNEL_Y_NEGATE;

    op.ui[1] |= i915_get_hardware_channel_val(operand0.z) <<
	A1_SRC0_CHANNEL_Z_SHIFT;
    if (operand0.z < 0)
	op.ui[1] |= A1_SRC0_CHANNEL_Z_NEGATE;

    op.ui[1] |= i915_get_hardware_channel_val(operand0.w) <<
	A1_SRC0_CHANNEL_W_SHIFT;
    if (operand0.w < 0)
	op.ui[1] |= A1_SRC0_CHANNEL_W_NEGATE;

    /* Set up operand 1 */
    op.ui[1] |= REG_TYPE(operand1.reg) << A1_SRC1_TYPE_SHIFT;
    op.ui[1] |= REG_NR(operand1.reg) << A1_SRC1_NR_SHIFT;

    op.ui[1] |= i915_get_hardware_channel_val(operand1.x) <<
	A1_SRC1_CHANNEL_X_SHIFT;
    if (operand1.x < 0)
	op.ui[1] |= A1_SRC1_CHANNEL_X_NEGATE;

    op.ui[1] |= i915_get_hardware_channel_val(operand1.y) <<
	A1_SRC1_CHANNEL_Y_SHIFT;
    if (operand1.y < 0)
	op.ui[1] |= A1_SRC1_CHANNEL_Y_NEGATE;

    op.ui[2] |= i915_get_hardware_channel_val(operand1.z) <<
	A2_SRC1_CHANNEL_Z_SHIFT;
    if (operand1.z < 0)
	op.ui[2] |= A2_SRC1_CHANNEL_Z_NEGATE;

    op.ui[2] |= i915_get_hardware_channel_val(operand1.w) <<
	A2_SRC1_CHANNEL_W_SHIFT;
    if (operand1.w < 0)
	op.ui[2] |= A2_SRC1_CHANNEL_W_NEGATE;

    /* Set up operand 2 */
    op.ui[2] |= REG_TYPE(operand2.reg) << A2_SRC2_TYPE_SHIFT;
    op.ui[2] |= REG_NR(operand2.reg) << A2_SRC2_NR_SHIFT;

    op.ui[2] |= i915_get_hardware_channel_val(operand2.x) <<
	A2_SRC2_CHANNEL_X_SHIFT;
    if (operand2.x < 0)
	op.ui[2] |= A2_SRC2_CHANNEL_X_NEGATE;

    op.ui[2] |= i915_get_hardware_channel_val(operand2.y) <<
	A2_SRC2_CHANNEL_Y_SHIFT;
    if (operand2.y < 0)
	op.ui[2] |= A2_SRC2_CHANNEL_Y_NEGATE;

    op.ui[2] |= i915_get_hardware_channel_val(operand2.z) <<
	A2_SRC2_CHANNEL_Z_SHIFT;
    if (operand2.z < 0)
	op.ui[2] |= A2_SRC2_CHANNEL_Z_NEGATE;

    op.ui[2] |= i915_get_hardware_channel_val(operand2.w) <<
	A2_SRC2_CHANNEL_W_SHIFT;
    if (operand2.w < 0)
	op.ui[2] |= A2_SRC2_CHANNEL_W_NEGATE;

    return op;
}

/** Move operand0 to dest_reg */
#define i915_fs_mov(dest_reg, operand0)					\
do {									\
    FS_OUT(i915_fs_arith(MOV, dest_reg, operand0,			\
			 i915_fs_operand_none(),			\
			 i915_fs_operand_none()));			\
} while (0)

/**
 * Move the value in operand0 to the dest reg with the masking/saturation
 * specified.
 */
#define i915_fs_mov_masked(dest_reg, dest_mask, operand0)		\
do {									\
    struct i915_fs_op op;						\
									\
    op = i915_fs_arith(MOV, dest_reg, operand0, i915_fs_operand_none(),	\
		       i915_fs_operand_none());				\
    op.ui[0] &= ~A0_DEST_CHANNEL_ALL;					\
    op.ui[0] |= ((dest_mask) & ~MASK_SATURATE) << A0_DEST_CHANNEL_SHIFT; \
    if ((dest_mask) & MASK_SATURATE)					\
	op.ui[0] |= A0_DEST_SATURATE;					\
									\
    FS_OUT(op);								\
} while (0)

/** Add operand0 and operand1 and put the result in dest_reg */
#define i915_fs_add(dest_reg, operand0, operand1)			\
do {									\
    FS_OUT(i915_fs_arith(ADD, dest_reg, operand0, operand1,		\
			 i915_fs_operand_none()));			\
} while (0)

/** Add operand0 and operand1 and put the result in dest_reg */
#define i915_fs_mul(dest_reg, operand0, operand1)			\
do {									\
    FS_OUT(i915_fs_arith(MUL, dest_reg, operand0, operand1,		\
			 i915_fs_operand_none()));			\
} while (0)

/**
 * Perform a 3-component dot-product of operand0 and operand1 and put the
 * resulting scalar in the channels of dest_reg specified by the dest_mask.
 */
#define i915_fs_dp3_masked(dest_reg, dest_mask, operand0, operand1)	\
do {									\
    struct i915_fs_op op;						\
									\
    op = i915_fs_arith(DP3, dest_reg, operand0, operand1,		\
		       i915_fs_operand_none());				\
    op.ui[0] &= ~A0_DEST_CHANNEL_ALL;					\
    op.ui[0] |= ((dest_mask) & ~MASK_SATURATE) << A0_DEST_CHANNEL_SHIFT; \
    if ((dest_mask) & MASK_SATURATE)					\
	op.ui[0] |= A0_DEST_SATURATE;					\
									\
    FS_OUT(op);								\
} while (0)

/**
 * Sets up local state for accumulating a fragment shader buffer.
 *
 * \param x maximum number of shader commands that may be used between
 *        a FS_START and FS_END
 */
#define FS_LOCALS(x)							\
    uint32_t _shader_buf[(x) * 3];					\
    int _max_shader_commands = x;					\
    int _cur_shader_commands

#define FS_BEGIN()							\
do {									\
    _cur_shader_commands = 0;						\
} while (0)

#define FS_OUT(_shaderop)						\
do {									\
    _shader_buf[_cur_shader_commands * 3 + 0] = _shaderop.ui[0];	\
    _shader_buf[_cur_shader_commands * 3 + 1] = _shaderop.ui[1];	\
    _shader_buf[_cur_shader_commands * 3 + 2] = _shaderop.ui[2];	\
    if (++_cur_shader_commands > _max_shader_commands)			\
	 FatalError("fragment shader command buffer exceeded (%d)\n",	\
		    _cur_shader_commands);				\
} while (0)

#define FS_END()							\
do {									\
    int _i, _pad = (_cur_shader_commands & 0x1) ? 0 : 1;		\
    BEGIN_BATCH(_cur_shader_commands * 3 + 1 + _pad);			\
    OUT_BATCH(_3DSTATE_PIXEL_SHADER_PROGRAM |				\
	     (_cur_shader_commands * 3 - 1));				\
    for (_i = 0; _i < _cur_shader_commands * 3; _i++)			\
	OUT_BATCH(_shader_buf[_i]);					\
    if (_pad != 0)							\
	OUT_BATCH(MI_NOOP);						\
    ADVANCE_BATCH();							\
} while (0);
