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

#ifndef CH7017_REG_H
#define CH7017_REG_H

#define CH7017_TV_DISPLAY_MODE		0x00
#define CH7017_FLICKER_FILTER		0x01
#define CH7017_VIDEO_BANDWIDTH		0x02
#define CH7017_TEXT_ENHANCEMENT		0x03
#define CH7017_START_ACTIVE_VIDEO	0x04
#define CH7017_HORIZONTAL_POSITION	0x05
#define CH7017_VERTICAL_POSITION	0x06
#define CH7017_BLACK_LEVEL		0x07
#define CH7017_CONTRAST_ENHANCEMENT	0x08
#define CH7017_TV_PLL			0x09
#define CH7017_TV_PLL_M			0x0a
#define CH7017_TV_PLL_N			0x0b
#define CH7017_SUB_CARRIER_0		0x0c
#define CH7017_CIV_CONTROL		0x10
#define CH7017_CIV_0			0x11
#define CH7017_CHROMA_BOOST		0x14
#define CH7017_CLOCK_MODE		0x1c
#define CH7017_INPUT_CLOCK		0x1d
#define CH7017_GPIO_CONTROL		0x1e
#define CH7017_INPUT_DATA_FORMAT	0x1f
#define CH7017_CONNECTION_DETECT	0x20
#define CH7017_DAC_CONTROL		0x21
#define CH7017_BUFFERED_CLOCK_OUTPUT	0x22
#define CH7017_DEFEAT_VSYNC		0x47
#define CH7017_TEST_PATTERN		0x48

#define CH7017_POWER_MANAGEMENT		0x49
/** Enables the TV output path. */
#define CH7017_TV_EN			(1 << 0)
#define CH7017_DAC0_POWER_DOWN		(1 << 1)
#define CH7017_DAC1_POWER_DOWN		(1 << 2)
#define CH7017_DAC2_POWER_DOWN		(1 << 3)
#define CH7017_DAC3_POWER_DOWN		(1 << 4)
/** Powers down the TV out block, and DAC0-3 */
#define CH7017_TV_POWER_DOWN_EN		(1 << 5)

#define CH7017_VERSION_ID		0x4a

#define CH7017_DEVICE_ID		0x4b
#define CH7017_DEVICE_ID_VALUE		0x1b
#define CH7018_DEVICE_ID_VALUE		0x1a
#define CH7019_DEVICE_ID_VALUE		0x19

#define CH7017_XCLK_D2_ADJUST		0x53
#define CH7017_UP_SCALER_COEFF_0	0x55
#define CH7017_UP_SCALER_COEFF_1	0x56
#define CH7017_UP_SCALER_COEFF_2	0x57
#define CH7017_UP_SCALER_COEFF_3	0x58
#define CH7017_UP_SCALER_COEFF_4	0x59
#define CH7017_UP_SCALER_VERTICAL_INC_0	0x5a
#define CH7017_UP_SCALER_VERTICAL_INC_1	0x5b
#define CH7017_GPIO_INVERT		0x5c
#define CH7017_UP_SCALER_HORIZONTAL_INC_0	0x5d
#define CH7017_UP_SCALER_HORIZONTAL_INC_1	0x5e

#define CH7017_HORIZONTAL_ACTIVE_PIXEL_INPUT	0x5f
/**< Low bits of horizontal active pixel input */

#define CH7017_ACTIVE_INPUT_LINE_OUTPUT	0x60
/** High bits of horizontal active pixel input */
#define CH7017_LVDS_HAP_INPUT_MASK	(0x7 << 0)
/** High bits of vertical active line output */
#define CH7017_LVDS_VAL_HIGH_MASK	(0x7 << 3)

#define CH7017_VERTICAL_ACTIVE_LINE_OUTPUT	0x61
/**< Low bits of vertical active line output */

#define CH7017_HORIZONTAL_ACTIVE_PIXEL_OUTPUT	0x62
/**< Low bits of horizontal active pixel output */

#define CH7017_LVDS_POWER_DOWN		0x63
/** High bits of horizontal active pixel output */
#define CH7017_LVDS_HAP_HIGH_MASK	(0x7 << 0)
/** Enables the LVDS power down state transition */
#define CH7017_LVDS_POWER_DOWN_EN	(1 << 6)
/** Enables the LVDS upscaler */
#define CH7017_LVDS_UPSCALER_EN		(1 << 7)
#define CH7017_LVDS_POWER_DOWN_DEFAULT_RESERVED 0x08

#define CH7017_LVDS_ENCODING		0x64
#define CH7017_LVDS_DITHER_2D		(1 << 2)
#define CH7017_LVDS_DITHER_DIS		(1 << 3)
#define CH7017_LVDS_DUAL_CHANNEL_EN	(1 << 4)
#define CH7017_LVDS_24_BIT		(1 << 5)

#define CH7017_LVDS_ENCODING_2		0x65

#define CH7017_LVDS_PLL_CONTROL		0x66
/** Enables the LVDS panel output path */
#define CH7017_LVDS_PANEN		(1 << 0)
/** Enables the LVDS panel backlight */
#define CH7017_LVDS_BKLEN		(1 << 3)

#define CH7017_POWER_SEQUENCING_T1	0x67
#define CH7017_POWER_SEQUENCING_T2	0x68
#define CH7017_POWER_SEQUENCING_T3	0x69
#define CH7017_POWER_SEQUENCING_T4	0x6a
#define CH7017_POWER_SEQUENCING_T5	0x6b
#define CH7017_GPIO_DRIVER_TYPE		0x6c
#define CH7017_GPIO_DATA		0x6d
#define CH7017_GPIO_DIRECTION_CONTROL	0x6e

#define CH7017_LVDS_PLL_FEEDBACK_DIV	0x71
# define CH7017_LVDS_PLL_FEED_BACK_DIVIDER_SHIFT 4
# define CH7017_LVDS_PLL_FEED_FORWARD_DIVIDER_SHIFT 0
# define CH7017_LVDS_PLL_FEEDBACK_DEFAULT_RESERVED 0x80

#define CH7017_LVDS_PLL_VCO_CONTROL	0x72
# define CH7017_LVDS_PLL_VCO_DEFAULT_RESERVED 0x80
# define CH7017_LVDS_PLL_VCO_SHIFT	4
# define CH7017_LVDS_PLL_POST_SCALE_DIV_SHIFT 0

#define CH7017_OUTPUTS_ENABLE		0x73
# define CH7017_CHARGE_PUMP_LOW		0x0
# define CH7017_CHARGE_PUMP_HIGH	0x3
# define CH7017_LVDS_CHANNEL_A		(1 << 3)
# define CH7017_LVDS_CHANNEL_B		(1 << 4)
# define CH7017_TV_DAC_A		(1 << 5)
# define CH7017_TV_DAC_B		(1 << 6)
# define CH7017_DDC_SELECT_DC2		(1 << 7)

#define CH7017_LVDS_OUTPUT_AMPLITUDE	0x74
#define CH7017_LVDS_PLL_EMI_REDUCTION	0x75
#define CH7017_LVDS_POWER_DOWN_FLICKER	0x76

#define CH7017_LVDS_CONTROL_2		0x78
# define CH7017_LOOP_FILTER_SHIFT	5
# define CH7017_PHASE_DETECTOR_SHIFT	0

#define CH7017_BANG_LIMIT_CONTROL	0x7f

#endif /* CH7017_REG_H */
