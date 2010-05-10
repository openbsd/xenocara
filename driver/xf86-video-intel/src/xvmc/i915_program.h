#ifndef _I915_PROGRAM_H
#define _I915_PROGRAM_H

#define REG_TYPE_R                 0	/* temporary regs, no need to
					 * dcl, must be written before
					 * read -- Preserved between
					 * phases.
					 */
#define REG_TYPE_T                 1	/* Interpolated values, must be
					 * dcl'ed before use.
					 *
					 * 0..7: texture coord,
					 * 8: diffuse spec,
					 * 9: specular color,
					 * 10: fog parameter in w.
					 */
#define REG_TYPE_CONST             2	/* Restriction: only one const
					 * can be referenced per
					 * instruction, though it may be
					 * selected for multiple inputs.
					 * Constants not initialized
					 * default to zero.
					 */
#define REG_TYPE_S                 3	/* sampler */
#define REG_TYPE_OC                4	/* output color (rgba) */
#define REG_TYPE_OD                5	/* output depth (w), xyz are
					 * temporaries.  If not written,
					 * interpolated depth is used?
					 */
#define REG_TYPE_U                 6	/* unpreserved temporaries */
#define REG_TYPE_MASK              0x7
#define REG_NR_MASK                0xf

/* REG_TYPE_T:
 */
#define T_TEX0     0
#define T_TEX1     1
#define T_TEX2     2
#define T_TEX3     3
#define T_TEX4     4
#define T_TEX5     5
#define T_TEX6     6
#define T_TEX7     7
#define T_DIFFUSE  8
#define T_SPECULAR 9
#define T_FOG_W    10		/* interpolated fog is in W coord */

/* Arithmetic instructions */

/* .replicate_swizzle == selection and replication of a particular
 * scalar channel, ie., .xxxx, .yyyy, .zzzz or .wwww
 */
#define A0_NOP    (0x0<<24)	/* no operation */
#define A0_ADD    (0x1<<24)	/* dst = src0 + src1 */
#define A0_MOV    (0x2<<24)	/* dst = src0 */
#define A0_MUL    (0x3<<24)	/* dst = src0 * src1 */
#define A0_MAD    (0x4<<24)	/* dst = src0 * src1 + src2 */
#define A0_DP2ADD (0x5<<24)	/* dst.xyzw = src0.xy dot src1.xy + src2.replicate_swizzle */
#define A0_DP3    (0x6<<24)	/* dst.xyzw = src0.xyz dot src1.xyz */
#define A0_DP4    (0x7<<24)	/* dst.xyzw = src0.xyzw dot src1.xyzw */
#define A0_FRC    (0x8<<24)	/* dst = src0 - floor(src0) */
#define A0_RCP    (0x9<<24)	/* dst.xyzw = 1/(src0.replicate_swizzle) */
#define A0_RSQ    (0xa<<24)	/* dst.xyzw = 1/(sqrt(abs(src0.replicate_swizzle))) */
#define A0_EXP    (0xb<<24)	/* dst.xyzw = exp2(src0.replicate_swizzle) */
#define A0_LOG    (0xc<<24)	/* dst.xyzw = log2(abs(src0.replicate_swizzle)) */
#define A0_CMP    (0xd<<24)	/* dst = (src0 >= 0.0) ? src1 : src2 */
#define A0_MIN    (0xe<<24)	/* dst = (src0 < src1) ? src0 : src1 */
#define A0_MAX    (0xf<<24)	/* dst = (src0 >= src1) ? src0 : src1 */
#define A0_FLR    (0x10<<24)	/* dst = floor(src0) */
#define A0_MOD    (0x11<<24)	/* dst = src0 fmod 1.0 */
#define A0_TRC    (0x12<<24)	/* dst = int(src0) */
#define A0_SGE    (0x13<<24)	/* dst = src0 >= src1 ? 1.0 : 0.0 */
#define A0_SLT    (0x14<<24)	/* dst = src0 < src1 ? 1.0 : 0.0 */
#define A0_DEST_SATURATE                 (1<<22)
#define A0_DEST_TYPE_SHIFT                19
/* Allow: R, OC, OD, U */
#define A0_DEST_NR_SHIFT                 14
/* Allow R: 0..15, OC,OD: 0..0, U: 0..2 */
#define A0_DEST_CHANNEL_X                (1<<10)
#define A0_DEST_CHANNEL_Y                (2<<10)
#define A0_DEST_CHANNEL_Z                (4<<10)
#define A0_DEST_CHANNEL_W                (8<<10)
#define A0_DEST_CHANNEL_ALL              (0xf<<10)
#define A0_DEST_CHANNEL_SHIFT            10
#define A0_SRC0_TYPE_SHIFT               7
#define A0_SRC0_NR_SHIFT                 2

#define A0_DEST_CHANNEL_XY              (A0_DEST_CHANNEL_X|A0_DEST_CHANNEL_Y)
#define A0_DEST_CHANNEL_XYZ             (A0_DEST_CHANNEL_XY|A0_DEST_CHANNEL_Z)

#define SRC_X        0
#define SRC_Y        1
#define SRC_Z        2
#define SRC_W        3
#define SRC_ZERO     4
#define SRC_ONE      5

#define A1_SRC0_CHANNEL_X_NEGATE         (1<<31)
#define A1_SRC0_CHANNEL_X_SHIFT          28
#define A1_SRC0_CHANNEL_Y_NEGATE         (1<<27)
#define A1_SRC0_CHANNEL_Y_SHIFT          24
#define A1_SRC0_CHANNEL_Z_NEGATE         (1<<23)
#define A1_SRC0_CHANNEL_Z_SHIFT          20
#define A1_SRC0_CHANNEL_W_NEGATE         (1<<19)
#define A1_SRC0_CHANNEL_W_SHIFT          16
#define A1_SRC1_TYPE_SHIFT               13
#define A1_SRC1_NR_SHIFT                 8
#define A1_SRC1_CHANNEL_X_NEGATE         (1<<7)
#define A1_SRC1_CHANNEL_X_SHIFT          4
#define A1_SRC1_CHANNEL_Y_NEGATE         (1<<3)
#define A1_SRC1_CHANNEL_Y_SHIFT          0

#define A2_SRC1_CHANNEL_Z_NEGATE         (1<<31)
#define A2_SRC1_CHANNEL_Z_SHIFT          28
#define A2_SRC1_CHANNEL_W_NEGATE         (1<<27)
#define A2_SRC1_CHANNEL_W_SHIFT          24
#define A2_SRC2_TYPE_SHIFT               21
#define A2_SRC2_NR_SHIFT                 16
#define A2_SRC2_CHANNEL_X_NEGATE         (1<<15)
#define A2_SRC2_CHANNEL_X_SHIFT          12
#define A2_SRC2_CHANNEL_Y_NEGATE         (1<<11)
#define A2_SRC2_CHANNEL_Y_SHIFT          8
#define A2_SRC2_CHANNEL_Z_NEGATE         (1<<7)
#define A2_SRC2_CHANNEL_Z_SHIFT          4
#define A2_SRC2_CHANNEL_W_NEGATE         (1<<3)
#define A2_SRC2_CHANNEL_W_SHIFT          0

/* Declaration instructions */
#define D0_DCL       (0x19<<24)	/* Declare a t (interpolated attrib)
				 * register or an s (sampler)
				 * register. */
#define D0_SAMPLE_TYPE_SHIFT              22
#define D0_SAMPLE_TYPE_2D                 (0x0<<22)
#define D0_SAMPLE_TYPE_CUBE               (0x1<<22)
#define D0_SAMPLE_TYPE_VOLUME             (0x2<<22)
#define D0_SAMPLE_TYPE_MASK               (0x3<<22)

#define D0_TYPE_SHIFT                19
/* Allow: T, S */
#define D0_NR_SHIFT                  14
/* Allow T: 0..10, S: 0..15 */
#define D0_CHANNEL_X                (1<<10)
#define D0_CHANNEL_Y                (2<<10)
#define D0_CHANNEL_Z                (4<<10)
#define D0_CHANNEL_W                (8<<10)
#define D0_CHANNEL_ALL              (0xf<<10)
#define D0_CHANNEL_NONE             (0<<10)

#define D0_CHANNEL_XY               (D0_CHANNEL_X|D0_CHANNEL_Y)
#define D0_CHANNEL_XYZ              (D0_CHANNEL_XY|D0_CHANNEL_Z)

/* I915 Errata: Do not allow (xz), (xw), (xzw) combinations for diffuse
 * or specular declarations.
 *
 * For T dcls, only allow: (x), (xy), (xyz), (w), (xyzw)
 *
 * Must be zero for S (sampler) dcls
 */
#define D1_MBZ                          0
#define D2_MBZ                          0

/* Texture instructions */
#define T0_TEXLD     (0x15<<24)	/* Sample texture using predeclared
				 * sampler and address, and output
				 * filtered texel data to destination
				 * register */
#define T0_TEXLDP    (0x16<<24)	/* Same as texld but performs a
				 * perspective divide of the texture
				 * coordinate .xyz values by .w before
				 * sampling. */
#define T0_TEXLDB    (0x17<<24)	/* Same as texld but biases the
				 * computed LOD by w.  Only S4.6 two's
				 * comp is used.  This implies that a
				 * float to fixed conversion is
				 * done. */
#define T0_TEXKILL   (0x18<<24)	/* Does not perform a sampling
				 * operation.  Simply kills the pixel
				 * if any channel of the address
				 * register is < 0.0. */
#define T0_DEST_TYPE_SHIFT                19
/* Allow: R, OC, OD, U */
/* Note: U (unpreserved) regs do not retain their values between
 * phases (cannot be used for feedback)
 *
 * Note: oC and OD registers can only be used as the destination of a
 * texture instruction once per phase (this is an implementation
 * restriction).
 */
#define T0_DEST_NR_SHIFT                 14
/* Allow R: 0..15, OC,OD: 0..0, U: 0..2 */
#define T0_SAMPLER_NR_SHIFT              0	/* This field ignored for TEXKILL */
#define T0_SAMPLER_NR_MASK               (0xf<<0)

#define T1_ADDRESS_REG_TYPE_SHIFT        24	/* Reg to use as texture coord */
/* Allow R, T, OC, OD -- R, OC, OD are 'dependent' reads, new program phase */
#define T1_ADDRESS_REG_NR_SHIFT          17
#define T2_MBZ                           0

/* Having zero and one in here makes the definition of swizzle a lot
 * easier.
 */
#define UREG_TYPE_SHIFT               29
#define UREG_NR_SHIFT                 24
#define UREG_CHANNEL_X_NEGATE_SHIFT   23
#define UREG_CHANNEL_X_SHIFT          20
#define UREG_CHANNEL_Y_NEGATE_SHIFT   19
#define UREG_CHANNEL_Y_SHIFT          16
#define UREG_CHANNEL_Z_NEGATE_SHIFT   15
#define UREG_CHANNEL_Z_SHIFT          12
#define UREG_CHANNEL_W_NEGATE_SHIFT   11
#define UREG_CHANNEL_W_SHIFT          8
#define UREG_CHANNEL_ZERO_NEGATE_MBZ  5
#define UREG_CHANNEL_ZERO_SHIFT       4
#define UREG_CHANNEL_ONE_NEGATE_MBZ   1
#define UREG_CHANNEL_ONE_SHIFT        0

#define UREG_BAD          0xffffffff	/* not a valid ureg */

#define X    SRC_X
#define Y    SRC_Y
#define Z    SRC_Z
#define W    SRC_W
#define ZERO SRC_ZERO
#define ONE  SRC_ONE

/* Construct a ureg:
 */
#define UREG(type, nr) (((type) << UREG_TYPE_SHIFT) |           \
                        ((nr)  << UREG_NR_SHIFT) |              \
                        (X     << UREG_CHANNEL_X_SHIFT) |       \
                        (Y     << UREG_CHANNEL_Y_SHIFT) |       \
                        (Z     << UREG_CHANNEL_Z_SHIFT) |       \
                        (W     << UREG_CHANNEL_W_SHIFT) |       \
                        (ZERO  << UREG_CHANNEL_ZERO_SHIFT) |    \
                        (ONE   << UREG_CHANNEL_ONE_SHIFT))

#define GET_CHANNEL_SRC( reg, channel ) ((reg<<(channel*4)) & (0xf<<20))
#define CHANNEL_SRC( src, channel ) (src>>(channel*4))

#define GET_UREG_TYPE(reg) (((reg) >> UREG_TYPE_SHIFT) & REG_TYPE_MASK)
#define GET_UREG_NR(reg)   (((reg) >> UREG_NR_SHIFT) & REG_NR_MASK)

#define UREG_XYZW_CHANNEL_MASK 0x00ffff00

#define A0_DEST(reg) (((reg) & UREG_TYPE_NR_MASK) >> UREG_A0_DEST_SHIFT_LEFT)
#define D0_DEST(reg) (((reg) & UREG_TYPE_NR_MASK) >> UREG_A0_DEST_SHIFT_LEFT)
#define T0_DEST(reg) (((reg) & UREG_TYPE_NR_MASK) >> UREG_A0_DEST_SHIFT_LEFT)
#define A0_SRC0(reg) (((reg) & UREG_MASK) >> UREG_A0_SRC0_SHIFT_LEFT)
#define A1_SRC0(reg) (((reg) & UREG_MASK) << UREG_A1_SRC0_SHIFT_RIGHT)
#define A1_SRC1(reg) (((reg) & UREG_MASK) >> UREG_A1_SRC1_SHIFT_LEFT)
#define A2_SRC1(reg) (((reg) & UREG_MASK) << UREG_A2_SRC1_SHIFT_RIGHT)
#define A2_SRC2(reg) (((reg) & UREG_MASK) >> UREG_A2_SRC2_SHIFT_LEFT)

/* These are special, and don't have swizzle/negate bits.
 */
#define T0_SAMPLER( reg )     (GET_UREG_NR(reg) << T0_SAMPLER_NR_SHIFT)
#define T1_ADDRESS_REG( reg ) ((GET_UREG_NR(reg) << T1_ADDRESS_REG_NR_SHIFT) | \
                               (GET_UREG_TYPE(reg) << T1_ADDRESS_REG_TYPE_SHIFT))

/* Macros for translating UREG's into the various register fields used
 * by the I915 programmable unit.
 */
#define UREG_A0_DEST_SHIFT_LEFT  (UREG_TYPE_SHIFT - A0_DEST_TYPE_SHIFT)
#define UREG_A0_SRC0_SHIFT_LEFT  (UREG_TYPE_SHIFT - A0_SRC0_TYPE_SHIFT)
#define UREG_A1_SRC0_SHIFT_RIGHT (A1_SRC0_CHANNEL_W_SHIFT - UREG_CHANNEL_W_SHIFT)
#define UREG_A1_SRC1_SHIFT_LEFT  (UREG_TYPE_SHIFT - A1_SRC1_TYPE_SHIFT)
#define UREG_A2_SRC1_SHIFT_RIGHT (A2_SRC1_CHANNEL_W_SHIFT - UREG_CHANNEL_W_SHIFT)
#define UREG_A2_SRC2_SHIFT_LEFT  (UREG_TYPE_SHIFT - A2_SRC2_TYPE_SHIFT)

#define UREG_MASK         0xffffff00
#define UREG_TYPE_NR_MASK ((REG_TYPE_MASK << UREG_TYPE_SHIFT) | \
                           (REG_NR_MASK << UREG_NR_SHIFT))

#endif
