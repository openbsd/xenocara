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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

/** @file
 * This file contains the register definitions for the i82807aa.
 *
 * Documentation on this chipset can be found in datasheet #29069001 at
 * intel.com.
 */
#ifndef I82807AA_REG_H
#define I82807AA_REG_H

/** @defgroup VR00 VCH Revision & GMBus Base Addr
 * @{
 */
#define VR00		0x00
# define VR00_BASE_ADDRESS_MASK		0x007f
/** @} */

/** @defgroup VR01 VCH Functionality Enable
 * @{
 */
#define VR01		0x01
/**
 * Enable the panel fitter
 */
# define VR01_PANEL_FIT_ENABLE		(1 << 3)
/**
 * Enables the LCD display.
 *
 * This must not be set while VR01_DVO_BYPASS_ENABLE is set.
 */
# define VR01_LCD_ENABLE		(1 << 2)
/** Enables the DVO repeater. */
# define VR01_DVO_BYPASS_ENABLE		(1 << 1)
/** Enables the DVO clock */
# define VR01_DVO_ENABLE		(1 << 0)
/** @} */

/** @defgroup VR10 LCD Interface Format
 * @{
 */
#define VR10		0x10
/** Enables LVDS output instead of CMOS */
# define VR10_LVDS_ENABLE		(1 << 4)
/** Enables 18-bit LVDS output. */
# define VR10_INTERFACE_1X18		(0 << 2)
/** Enables 24-bit LVDS or CMOS output */
# define VR10_INTERFACE_1X24		(1 << 2)
/** Enables 2x18-bit LVDS or CMOS output. */
# define VR10_INTERFACE_2X18		(2 << 2)
/** Enables 2x24-bit LVDS output */
# define VR10_INTERFACE_2X24		(3 << 2)
/** @} */

/** @defgroup VR11 CMOS Output Control
 * @{
 */
/** @} */

/** @defgroup VR12 LVDS Output Control
 * @{
 */
/** @} */

/** @defgroup VR18 PLL clock select
 * @{
 */
/** @} */

/** @defgroup VR19 PLL clock divisor M
 * @{
 */
/** @} */

/** @defgroup VR1A PLL clock divisor N
 * @{
 */
/** @} */

/** @defgroup VR1F FIFO Pre-load
 * @{
 */
/** @} */

/** @defgroup VR20 LCD Horizontal Display Size
 * @{
 */
#define VR20	0x20
/** @} */

/** @defgroup VR21 LCD Vertical Display Size
 * @{
 */
#define VR21	0x20
/** @} */

/** @defgroup VR22 Horizontal TRP to DE Start Delay
 * @{
 */
/** @} */

/** @defgroup VR23 Horizontal TRP to DE End Delay
 * @{
 */
/** @} */

/** @defgroup VR24 Horizontal TRP To LP Start Delay
 * @{
 */
/** @} */

/** @defgroup VR25 Horizontal TRP To LP End Delay
 * @{
 */
/** @} */

/** @defgroup VR26 Vertical TRP To FLM Start Delay
 * @{
 */
/** @} */

/** @defgroup VR27 Vertical TRP To FLM End Delay
 * @{
 */
/** @} */

/** @defgroup VR30 Panel power down status
 * @{
 */
#define VR30		0x30
/** Read only bit indicating that the panel is not in a safe poweroff state. */
# define VR30_PANEL_ON			(1 << 15)
/** @} */

/** @defgroup VR31 Tpon Panel power on sequencing delay
 * @{
 */
/** @} */

/** @defgroup VR32 Tpon Panel power off sequencing delay
 * @{
 */
/** @} */

/** @defgroup VR33 Tstay Panel power off stay down delay
 * @{
 */
/** @} */

/** @defgroup VR34 Maximal FLM Pulse Interval
 * @{
 */
/** @} */

/** @defgroup VR35 Maximal LP Pulse Interval
 * @{
 */
/** @} */

/** @defgroup VR40
 * @{
 */
#define VR40		0x40
# define VR40_STALL_ENABLE		(1 << 13)
# define VR40_VERTICAL_INTERP_ENABLE	(1 << 12)
# define VR40_ENHANCED_PANEL_FITTING	(1 << 11)
# define VR40_HORIZONTAL_INTERP_ENABLE	(1 << 10)
# define VR40_AUTO_RATIO_ENABLE		(1 << 9)
# define VR40_CLOCK_GATING_ENABLE	(1 << 8)
/** @} */

/** @defgroup VR41 Panel Fitting Vertical Ratio
 * @{
 *
 * (((image_height - 1) << 16) / ((panel_height - 1))) >> 2
 */
/** @} */
#define VR41		0x41

/** @defgroup VR42 Panel Fitting Horizontal Ratio
 * @{
 * (((image_width - 1) << 16) / ((panel_width - 1))) >> 2
 */
/** @} */
#define VR42		0x42

/** @defgroup VR43 Horizontal Image Size
 * @{
 */
/** @} */
#define VR43		0x43

/** @defgroup VR44 Panel Fitting Coefficient 0
 * @{
 */
/** @} */

/** @defgroup VR45 Panel Fitting Coefficient 1
 * @{
 */
/** @} */

/** @defgroup VR46 Panel Fitting Coefficient 2
 * @{
 */
/** @} */

/** @defgroup VR47 Panel Fitting Coefficient 3
 * @{
 */
/** @} */

/** @defgroup VR48 Panel Fitting Coefficient 4
 * @{
 */
/** @} */

/** @defgroup VR49 Panel Fitting Coefficient 5
 * @{
 */
/** @} */

/** @defgroup VR80 GPIO 0
 * @{
 */
/** @} */

#define VR80	    0x80
#define VR81	    0x81
#define VR82	    0x82
#define VR83	    0x83
#define VR84	    0x84
#define VR85	    0x85
#define VR86	    0x86
#define VR87	    0x87
    
/** @defgroup VR88 GPIO 8
 * @{
 */
/** @} */

#define VR88	    0x88

/** @defgroup VR8E Graphics BIOS scratch 0
 * @{
 */
#define VR8E	    0x8E
# define VR8E_PANEL_TYPE_MASK		(0xf << 0)
# define VR8E_PANEL_INTERFACE_CMOS	(0 << 4)
# define VR8E_PANEL_INTERFACE_LVDS	(1 << 4)
# define VR8E_FORCE_DEFAULT_PANEL	(1 << 5)
/** @} */

/** @defgroup VR8F Graphics BIOS scratch 1
 * @{
 */
#define VR8F	    0x8F
# define VR8F_VCH_PRESENT		(1 << 0)
# define VR8F_DISPLAY_CONN		(1 << 1)
# define VR8F_POWER_MASK		(0x3c)
# define VR8F_POWER_POS			(2)
/** @} */


#endif /* I82807AA_REG_H */
