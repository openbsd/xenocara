/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

/** @file
 * Register names and fields for Intel graphics.
 */

/*
 * Authors:
 *   Keith Whitwell <keith@tungstengraphics.com>
 *   Eric Anholt <eric@anholt.net>
 *
 *   based on the i740 driver by
 *        Kevin E. Martin <kevin@precisioninsight.com> 
 *   
 *
 */

#ifndef _I810_REG_H
#define _I810_REG_H

/* I/O register offsets
 */
#define SRX 0x3C4		/* p208 */
#define GRX 0x3CE		/* p213 */
#define ARX 0x3C0		/* p224 */

/* VGA Color Palette Registers */
#define DACMASK  0x3C6		/* p232 */
#define DACSTATE 0x3C7		/* p232 */
#define DACRX    0x3C7		/* p233 */
#define DACWX    0x3C8		/* p233 */
#define DACDATA  0x3C9		/* p233 */

/* CRT Controller Registers (CRX) */
#define START_ADDR_HI        0x0C /* p246 */
#define START_ADDR_LO        0x0D /* p247 */
#define VERT_SYNC_END        0x11 /* p249 */
#define EXT_VERT_TOTAL       0x30 /* p257 */
#define EXT_VERT_DISPLAY     0x31 /* p258 */
#define EXT_VERT_SYNC_START  0x32 /* p259 */
#define EXT_VERT_BLANK_START 0x33 /* p260 */
#define EXT_HORIZ_TOTAL      0x35 /* p261 */
#define EXT_HORIZ_BLANK      0x39 /* p261 */
#define EXT_START_ADDR       0x40 /* p262 */
#define EXT_START_ADDR_ENABLE    0x80 
#define EXT_OFFSET           0x41 /* p263 */
#define EXT_START_ADDR_HI    0x42 /* p263 */
#define INTERLACE_CNTL       0x70 /* p264 */
#define INTERLACE_ENABLE         0x80 
#define INTERLACE_DISABLE        0x00 

/* Miscellaneous Output Register 
 */
#define MSR_R          0x3CC	/* p207 */
#define MSR_W          0x3C2	/* p207 */
#define IO_ADDR_SELECT     0x01

#define MDA_BASE       0x3B0	/* p207 */
#define CGA_BASE       0x3D0	/* p207 */

/* CR80 - IO Control, p264
 */
#define IO_CTNL            0x80
#define EXTENDED_ATTR_CNTL     0x02
#define EXTENDED_CRTC_CNTL     0x01

/* GR10 - Address mapping, p221
 */
#define ADDRESS_MAPPING    0x10
#define PAGE_TO_LOCAL_MEM_ENABLE 0x10
#define GTT_MEM_MAP_ENABLE     0x08
#define PACKED_MODE_ENABLE     0x04
#define LINEAR_MODE_ENABLE     0x02
#define PAGE_MAPPING_ENABLE    0x01

#define HOTKEY_VBIOS_SWITCH_BLOCK	0x80
#define HOTKEY_SWITCH			0x20
#define HOTKEY_TOGGLE			0x10

/* Blitter control, p378
 */
#define BITBLT_CNTL        0x7000c
#define COLEXP_MODE            0x30
#define COLEXP_8BPP            0x00
#define COLEXP_16BPP           0x10
#define COLEXP_24BPP           0x20
#define COLEXP_RESERVED        0x30
#define BITBLT_STATUS          0x01

#define CHDECMISC	0x10111
#define DCC			0x10200
#define C0DRB0			0x10200
#define C0DRB1			0x10202
#define C0DRB2			0x10204
#define C0DRB3			0x10206
#define C0DRA01			0x10208
#define C0DRA23			0x1020a
#define C1DRB0			0x10600
#define C1DRB1			0x10602
#define C1DRB2			0x10604
#define C1DRB3			0x10606
#define C1DRA01			0x10608
#define C1DRA23			0x1060a

/* p375. 
 */
#define DISPLAY_CNTL       0x70008
#define VGA_WRAP_MODE          0x02
#define VGA_WRAP_AT_256KB      0x00
#define VGA_NO_WRAP            0x02
#define GUI_MODE               0x01
#define STANDARD_VGA_MODE      0x00
#define HIRES_MODE             0x01

/* p375
 */
#define PIXPIPE_CONFIG_0   0x70009
#define DAC_8_BIT              0x80
#define DAC_6_BIT              0x00
#define HW_CURSOR_ENABLE       0x10
#define EXTENDED_PALETTE       0x01

/* p375
 */
#define PIXPIPE_CONFIG_1   0x7000a
#define DISPLAY_COLOR_MODE     0x0F
#define DISPLAY_VGA_MODE       0x00
#define DISPLAY_8BPP_MODE      0x02
#define DISPLAY_15BPP_MODE     0x04
#define DISPLAY_16BPP_MODE     0x05
#define DISPLAY_24BPP_MODE     0x06
#define DISPLAY_32BPP_MODE     0x07

/* p375
 */
#define PIXPIPE_CONFIG_2   0x7000b
#define DISPLAY_GAMMA_ENABLE   0x08
#define DISPLAY_GAMMA_DISABLE  0x00
#define OVERLAY_GAMMA_ENABLE   0x04
#define OVERLAY_GAMMA_DISABLE  0x00


/* p380
 */
#define DISPLAY_BASE       0x70020
#define DISPLAY_BASE_MASK  0x03fffffc


/* Cursor control registers, pp383-384
 */
/* Desktop (845G, 865G) */
#define CURSOR_CONTROL     0x70080
#define CURSOR_ENABLE          0x80000000
#define CURSOR_GAMMA_ENABLE    0x40000000
#define CURSOR_STRIDE_MASK     0x30000000
#define CURSOR_FORMAT_SHIFT    24
#define CURSOR_FORMAT_MASK     (0x07 << CURSOR_FORMAT_SHIFT)
#define CURSOR_FORMAT_2C       (0x00 << CURSOR_FORMAT_SHIFT)
#define CURSOR_FORMAT_3C       (0x01 << CURSOR_FORMAT_SHIFT)
#define CURSOR_FORMAT_4C       (0x02 << CURSOR_FORMAT_SHIFT)
#define CURSOR_FORMAT_ARGB     (0x04 << CURSOR_FORMAT_SHIFT)
#define CURSOR_FORMAT_XRGB     (0x05 << CURSOR_FORMAT_SHIFT)

/* Mobile and i810 */
#define CURSOR_A_CONTROL   CURSOR_CONTROL
#define CURSOR_ORIGIN_SCREEN   0x00	/* i810 only */
#define CURSOR_ORIGIN_DISPLAY  0x1	/* i810 only */
#define CURSOR_MODE            0x27
#define CURSOR_MODE_DISABLE    0x00
#define CURSOR_MODE_32_4C_AX   0x01	/* i810 only */
#define CURSOR_MODE_64_3C      0x04
#define CURSOR_MODE_64_4C_AX   0x05
#define CURSOR_MODE_64_4C      0x06
#define CURSOR_MODE_64_32B_AX  0x07
#define CURSOR_MODE_64_ARGB_AX (0x20 | CURSOR_MODE_64_32B_AX)
#define MCURSOR_PIPE_SELECT    (1 << 28)
#define MCURSOR_PIPE_A         0x00
#define MCURSOR_PIPE_B         (1 << 28)
#define MCURSOR_GAMMA_ENABLE   (1 << 26)
#define MCURSOR_MEM_TYPE_LOCAL (1 << 25)


#define CURSOR_BASEADDR    0x70084
#define CURSOR_A_BASE      CURSOR_BASEADDR
#define CURSOR_BASEADDR_MASK 0x1FFFFF00
#define CURSOR_A_POSITION  0x70088
#define CURSOR_POS_SIGN        0x8000
#define CURSOR_POS_MASK        0x007FF
#define CURSOR_X_SHIFT	       0
#define CURSOR_Y_SHIFT         16
#define CURSOR_X_LO        0x70088
#define CURSOR_X_HI        0x70089
#define CURSOR_X_POS           0x00
#define CURSOR_X_NEG           0x80
#define CURSOR_Y_LO        0x7008A
#define CURSOR_Y_HI        0x7008B
#define CURSOR_Y_POS           0x00
#define CURSOR_Y_NEG           0x80

#define CURSOR_A_PALETTE0  0x70090
#define CURSOR_A_PALETTE1  0x70094
#define CURSOR_A_PALETTE2  0x70098
#define CURSOR_A_PALETTE3  0x7009C

#define CURSOR_SIZE	   0x700A0
#define CURSOR_SIZE_MASK       0x3FF
#define CURSOR_SIZE_HSHIFT     0
#define CURSOR_SIZE_VSHIFT     12

#define CURSOR_B_CONTROL   0x700C0
#define CURSOR_B_BASE      0x700C4
#define CURSOR_B_POSITION  0x700C8
#define CURSOR_B_PALETTE0  0x700D0
#define CURSOR_B_PALETTE1  0x700D4
#define CURSOR_B_PALETTE2  0x700D8
#define CURSOR_B_PALETTE3  0x700DC


/* Similar registers exist in Device 0 on the i810 (pp55-65), but I'm
 * not sure they refer to local (graphics) memory.
 *
 * These details are for the local memory control registers,
 * (pp301-310).  The test machines are not equiped with local memory,
 * so nothing is tested.  Only a single row seems to be supported.
 */
#define DRAM_ROW_TYPE      0x3000
#define DRAM_ROW_0             0x01
#define DRAM_ROW_0_SDRAM       0x01
#define DRAM_ROW_0_EMPTY       0x00
#define DRAM_ROW_CNTL_LO   0x3001
#define DRAM_PAGE_MODE_CTRL    0x10
#define DRAM_RAS_TO_CAS_OVRIDE 0x08
#define DRAM_CAS_LATENCY       0x04
#define DRAM_RAS_TIMING        0x02
#define DRAM_RAS_PRECHARGE     0x01
#define DRAM_ROW_CNTL_HI   0x3002
#define DRAM_REFRESH_RATE      0x18
#define DRAM_REFRESH_DISABLE   0x00
#define DRAM_REFRESH_60HZ      0x08
#define DRAM_REFRESH_FAST_TEST 0x10
#define DRAM_REFRESH_RESERVED  0x18
#define DRAM_SMS               0x07
#define DRAM_SMS_NORMAL        0x00
#define DRAM_SMS_NOP_ENABLE    0x01
#define DRAM_SMS_ABPCE         0x02
#define DRAM_SMS_MRCE          0x03
#define DRAM_SMS_CBRCE         0x04

/* p307
 */
#define DPMS_SYNC_SELECT   0x5002
#define VSYNC_CNTL             0x08
#define VSYNC_ON               0x00
#define VSYNC_OFF              0x08
#define HSYNC_CNTL             0x02
#define HSYNC_ON               0x00
#define HSYNC_OFF              0x02

#define GPIOA			0x5010
#define GPIOB			0x5014
#define GPIOC			0x5018
#define GPIOD			0x501c
#define GPIOE			0x5020
#define GPIOF			0x5024
#define GPIOG			0x5028
#define GPIOH			0x502c
# define GPIO_CLOCK_DIR_MASK		(1 << 0)
# define GPIO_CLOCK_DIR_IN		(0 << 1)
# define GPIO_CLOCK_DIR_OUT		(1 << 1)
# define GPIO_CLOCK_VAL_MASK		(1 << 2)
# define GPIO_CLOCK_VAL_OUT		(1 << 3)
# define GPIO_CLOCK_VAL_IN		(1 << 4)
# define GPIO_CLOCK_PULLUP_DISABLE	(1 << 5)
# define GPIO_DATA_DIR_MASK		(1 << 8)
# define GPIO_DATA_DIR_IN		(0 << 9)
# define GPIO_DATA_DIR_OUT		(1 << 9)
# define GPIO_DATA_VAL_MASK		(1 << 10)
# define GPIO_DATA_VAL_OUT		(1 << 11)
# define GPIO_DATA_VAL_IN		(1 << 12)
# define GPIO_DATA_PULLUP_DISABLE	(1 << 13)

/* GMBus registers for hardware-assisted (non-bitbanging) I2C access */
#define GMBUS0			0x5100
#define GMBUS1			0x5104
#define GMBUS2			0x5108
#define GMBUS3			0x510c
#define GMBUS4			0x5110
#define GMBUS5			0x5120

/* p317, 319
 */
#define VCLK2_VCO_M        0x6008 /* treat as 16 bit? (includes msbs) */
#define VCLK2_VCO_N        0x600a
#define VCLK2_VCO_DIV_SEL  0x6012

#define VCLK_DIVISOR_VGA0   0x6000
#define VCLK_DIVISOR_VGA1   0x6004
#define VCLK_POST_DIV	    0x6010
/** Selects a post divisor of 4 instead of 2. */
# define VGA1_PD_P2_DIV_4	(1 << 15)
/** Overrides the p2 post divisor field */
# define VGA1_PD_P1_DIV_2	(1 << 13)
# define VGA1_PD_P1_SHIFT	8
/** P1 value is 2 greater than this field */
# define VGA1_PD_P1_MASK	(0x1f << 8)
/** Selects a post divisor of 4 instead of 2. */
# define VGA0_PD_P2_DIV_4	(1 << 7)
/** Overrides the p2 post divisor field */
# define VGA0_PD_P1_DIV_2	(1 << 5)
# define VGA0_PD_P1_SHIFT	0
/** P1 value is 2 greater than this field */
# define VGA0_PD_P1_MASK	(0x1f << 0)

#define POST_DIV_SELECT        0x70
#define POST_DIV_1             0x00
#define POST_DIV_2             0x10
#define POST_DIV_4             0x20
#define POST_DIV_8             0x30
#define POST_DIV_16            0x40
#define POST_DIV_32            0x50
#define VCO_LOOP_DIV_BY_4M     0x00
#define VCO_LOOP_DIV_BY_16M    0x04


/* Instruction Parser Mode Register 
 *    - p281
 *    - 2 new bits.
 */
#define INST_PM                  0x20c0	
#define AGP_SYNC_PACKET_FLUSH_ENABLE 0x20 /* reserved */
#define SYNC_PACKET_FLUSH_ENABLE     0x10
#define TWO_D_INST_DISABLE           0x08
#define THREE_D_INST_DISABLE         0x04
#define STATE_VAR_UPDATE_DISABLE     0x02
#define PAL_STIP_DISABLE             0x01


#define MEMMODE                  0x20dc


/* Instruction parser error register.  p279
 */
#define IPEIR                  0x2088
#define IPEHR                  0x208C

#define INST_DONE                0x2090
# define IDCT_DONE			(1 << 30)
# define IQ_DONE			(1 << 29)
# define PR_DONE			(1 << 28)
# define VLD_DONE			(1 << 27)
# define IP_DONE			(1 << 26)
# define FBC_DONE			(1 << 25)
# define BINNER_DONE			(1 << 24)
# define SF_DONE			(1 << 23)
# define SE_DONE			(1 << 22)
# define WM_DONE			(1 << 21)
# define IZ_DONE			(1 << 20)
# define PERSPECTIVE_INTERP_DONE	(1 << 19)
# define DISPATCHER_DONE		(1 << 18)
# define PROJECTION_DONE		(1 << 17)
# define DEPENDENT_ADDRESS_DONE		(1 << 16)
# define QUAD_CACHE_DONE		(1 << 15)
# define TEXTURE_FETCH_DONE		(1 << 14)
# define TEXTURE_DECOMPRESS_DONE	(1 << 13)
# define SAMPLER_CACHE_DONE		(1 << 12)
# define FILTER_DONE			(1 << 11)
# define BYPASS_FIFO_DONE		(1 << 10)
# define PS_DONE			(1 << 9)
# define CC_DONE			(1 << 8)
# define MAP_FILTER_DONE		(1 << 7)
# define MAP_L2_IDLE			(1 << 6)
# define RING_2_ENABLE			(1 << 2)
# define RING_1_ENABLE			(1 << 1)
# define RING_0_ENABLE			(1 << 0)

#define SCPD0                    0x209c	/* debug */
#define INST_PS                  0x20c4
#define IPEIR_I965                  0x2064 /* i965 */
#define IPEHR_I965                  0x2068 /* i965 */
#define INST_DONE_I965              0x206c
# define I965_SF_DONE			(1 << 23)
# define I965_SE_DONE			(1 << 22)
# define I965_WM_DONE			(1 << 21)
# define I965_TEXTURE_FETCH_DONE	(1 << 14)
# define I965_SAMPLER_CACHE_DONE	(1 << 12)
# define I965_FILTER_DONE		(1 << 11)
# define I965_PS_DONE			(1 << 9)
# define I965_CC_DONE			(1 << 8)
# define I965_MAP_FILTER_DONE		(1 << 7)
# define I965_MAP_L2_IDLE		(1 << 6)
# define I965_CP_DONE			(1 << 1)
# define I965_RING_0_ENABLE		(1 << 0)
#define INST_PS_I965                0x2070

/* Current active ring head address: 
 */
#define ACTHD_I965                 0x2074
#define ACTHD			   0x20C8

/* Current primary/secondary DMA fetch addresses:
 */
#define DMA_FADD_P             0x2078
#define DMA_FADD_S               0x20d4
#define INST_DONE_1              0x207c

#define CACHE_MODE_0           0x2120
#define CACHE_MODE_1           0x2124
#define MI_MODE		       0x209c
#define MI_DISPLAY_POWER_DOWN  0x20e0
#define MI_ARB_STATE           0x20e4
#define MI_RDRET_STATE	       0x20fc

/* Start addresses for each of the primary rings:
 */
#define PR0_STR                  0x20f0
#define PR1_STR                  0x20f4
#define PR2_STR                  0x20f8

#define WIZ_CTL                0x7c00
#define WIZ_CTL_SINGLE_SUBSPAN  (1<<6)
#define WIZ_CTL_IGNORE_STALLS  (1<<5)

#define SVG_WORK_CTL           0x7408

#define TS_CTL                 0x7e00
#define TS_MUX_ERR_CODE        (0<<8)
#define TS_MUX_URB_0           (1<<8)
#define TS_MUX_DISPATCH_ID_0   (10<<8)
#define TS_MUX_ERR_CODE_VALID  (15<<8)
#define TS_MUX_TID_0           (16<<8)
#define TS_MUX_EUID_0          (18<<8)
#define TS_MUX_FFID_0          (22<<8)
#define TS_MUX_EOT             (26<<8)
#define TS_MUX_SIDEBAND_0      (27<<8)
#define TS_SNAP_ALL_CHILD      (1<<2)
#define TS_SNAP_ALL_ROOT       (1<<1)
#define TS_SNAP_ENABLE         (1<<0)

#define TS_DEBUG_DATA          0x7e0c

#define TD_CTL                 0x8000
#define TD_CTL2                0x8004


#define ECOSKPD 0x21d0
#define EXCC    0x2028

/* I965 debug regs:
 */
#define IA_VERTICES_COUNT_QW   0x2310
#define IA_PRIMITIVES_COUNT_QW 0x2318
#define VS_INVOCATION_COUNT_QW 0x2320
#define GS_INVOCATION_COUNT_QW 0x2328
#define GS_PRIMITIVES_COUNT_QW 0x2330
#define CL_INVOCATION_COUNT_QW 0x2338
#define CL_PRIMITIVES_COUNT_QW 0x2340
#define PS_INVOCATION_COUNT_QW 0x2348
#define PS_DEPTH_COUNT_QW      0x2350
#define TIMESTAMP_QW           0x2358
#define CLKCMP_QW              0x2360






/* General error reporting regs, p296
 */
#define EIR               0x20B0
#define EMR               0x20B4
#define ESR               0x20B8
# define ERR_VERTEX_MAX				(1 << 5) /* lpt/cst */
# define ERR_PGTBL_ERROR			(1 << 4)
# define ERR_DISPLAY_OVERLAY_UNDERRUN		(1 << 3)
# define ERR_MAIN_MEMORY_REFRESH		(1 << 1)
# define ERR_INSTRUCTION_ERROR			(1 << 0)


/* Interrupt Control Registers 
 *   - new bits for i810
 *   - new register hwstam (mask)
 */
#define HWS_PGA		     0x2080
#define PWRCTXA		     0x2088 /* 965GM+ only */
#define   PWRCTX_EN	     (1<<0)
#define HWSTAM               0x2098 /* p290 */
#define IER                  0x20a0 /* p291 */
#define IIR                  0x20a4 /* p292 */
#define IMR                  0x20a8 /* p293 */
#define ISR                  0x20ac /* p294 */
#define HW_ERROR                 0x8000
#define SYNC_STATUS_TOGGLE       0x1000
#define DPY_0_FLIP_PENDING       0x0800
#define DPY_1_FLIP_PENDING       0x0400	/* not implemented on i810 */
#define OVL_0_FLIP_PENDING       0x0200
#define OVL_1_FLIP_PENDING       0x0100	/* not implemented on i810 */
#define DPY_0_VBLANK             0x0080
#define DPY_0_EVENT              0x0040
#define DPY_1_VBLANK             0x0020	/* not implemented on i810 */
#define DPY_1_EVENT              0x0010	/* not implemented on i810 */
#define HOST_PORT_EVENT          0x0008	/*  */
#define CAPTURE_EVENT            0x0004	/*  */
#define USER_DEFINED             0x0002
#define BREAKPOINT               0x0001


#define INTR_RESERVED            (0x6000 | 		\
				  DPY_1_FLIP_PENDING |	\
				  OVL_1_FLIP_PENDING |	\
				  DPY_1_VBLANK |	\
				  DPY_1_EVENT |		\
				  HOST_PORT_EVENT |	\
				  CAPTURE_EVENT )

/* FIFO Watermark and Burst Length Control Register 
 *
 * - different offset and contents on i810 (p299) (fewer bits per field)
 * - some overlay fields added
 * - what does it all mean?
 */
#define FWATER_BLC       0x20d8
#define FWATER_BLC2	 0x20dc
#define MM_BURST_LENGTH     0x00700000
#define MM_FIFO_WATERMARK   0x0001F000
#define LM_BURST_LENGTH     0x00000700
#define LM_FIFO_WATERMARK   0x0000001F


/* Fence/Tiling ranges [0..7]
 */
#define FENCE            0x2000
#define FENCE_NR         8

#define FENCE_NEW        0x3000
#define FENCE_NEW_NR     16

#define FENCE_LINEAR     0
#define FENCE_XMAJOR	 1
#define FENCE_YMAJOR  	 2

#define I915G_FENCE_START_MASK	0x0ff00000

#define I830_FENCE_START_MASK	0x07f80000

#define FENCE_START_MASK    0x03F80000
#define FENCE_X_MAJOR       0x00000000
#define FENCE_Y_MAJOR       0x00001000
#define FENCE_SIZE_MASK     0x00000700
#define FENCE_SIZE_512K     0x00000000
#define FENCE_SIZE_1M       0x00000100
#define FENCE_SIZE_2M       0x00000200
#define FENCE_SIZE_4M       0x00000300
#define FENCE_SIZE_8M       0x00000400
#define FENCE_SIZE_16M      0x00000500
#define FENCE_SIZE_32M      0x00000600
#define FENCE_SIZE_64M	    0x00000700
#define I915G_FENCE_SIZE_1M       0x00000000
#define I915G_FENCE_SIZE_2M       0x00000100
#define I915G_FENCE_SIZE_4M       0x00000200
#define I915G_FENCE_SIZE_8M       0x00000300
#define I915G_FENCE_SIZE_16M      0x00000400
#define I915G_FENCE_SIZE_32M      0x00000500
#define I915G_FENCE_SIZE_64M	0x00000600
#define I915G_FENCE_SIZE_128M	0x00000700
#define I965_FENCE_X_MAJOR	0x00000000
#define I965_FENCE_Y_MAJOR	0x00000002
#define FENCE_PITCH_1       0x00000000
#define FENCE_PITCH_2       0x00000010
#define FENCE_PITCH_4       0x00000020
#define FENCE_PITCH_8       0x00000030
#define FENCE_PITCH_16      0x00000040
#define FENCE_PITCH_32      0x00000050
#define FENCE_PITCH_64	    0x00000060
#define FENCE_VALID         0x00000001


/* Registers to control page table, p274
 */
#define PGETBL_CTL       0x2020
#define PGETBL_ADDR_MASK    0xFFFFF000
#define PGETBL_ENABLE_MASK  0x00000001
#define PGETBL_ENABLED      0x00000001
/** Added in 965G, this field has the actual size of the global GTT */
#define PGETBL_SIZE_MASK    0x0000000e
#define PGETBL_SIZE_512KB   (0 << 1)
#define PGETBL_SIZE_256KB   (1 << 1)
#define PGETBL_SIZE_128KB   (2 << 1)
#define PGETBL_SIZE_1MB     (3 << 1)
#define PGETBL_SIZE_2MB     (4 << 1)
#define PGETBL_SIZE_1_5MB   (5 << 1)
#define G33_PGETBL_SIZE_MASK		(3 << 8)
#define G33_PGETBL_SIZE_1M		(1 << 8)
#define G33_PGETBL_SIZE_2M		(2 << 8)

#define I830_PTE_BASE			0x10000
#define PTE_ADDRESS_MASK		0xfffff000
#define PTE_ADDRESS_MASK_HIGH		0x000000f0 /* i915+ */
#define PTE_MAPPING_TYPE_UNCACHED	(0 << 1)
#define PTE_MAPPING_TYPE_DCACHE		(1 << 1) /* i830 only */
#define PTE_MAPPING_TYPE_CACHED		(3 << 1)
#define PTE_MAPPING_TYPE_MASK		(3 << 1)
#define PTE_VALID			(1 << 0)

/** @defgroup PGE_ERR
 * @{
 */
/** Page table debug register for i845 */
#define PGE_ERR          0x2024
#define PGE_ERR_ADDR_MASK   0xFFFFF000
#define PGE_ERR_ID_MASK     0x00000038
#define PGE_ERR_CAPTURE     0x00000000
#define PGE_ERR_OVERLAY     0x00000008
#define PGE_ERR_DISPLAY     0x00000010
#define PGE_ERR_HOST        0x00000018
#define PGE_ERR_RENDER      0x00000020
#define PGE_ERR_BLITTER     0x00000028
#define PGE_ERR_MAPPING     0x00000030
#define PGE_ERR_CMD_PARSER  0x00000038
#define PGE_ERR_TYPE_MASK   0x00000007
#define PGE_ERR_INV_TABLE   0x00000000
#define PGE_ERR_INV_PTE     0x00000001
#define PGE_ERR_MIXED_TYPES 0x00000002
#define PGE_ERR_PAGE_MISS   0x00000003
#define PGE_ERR_ILLEGAL_TRX 0x00000004
#define PGE_ERR_LOCAL_MEM   0x00000005
#define PGE_ERR_TILED       0x00000006
/** @} */

/** @defgroup PGTBL_ER
 * @{
 */
/** Page table debug register for i945 */
# define PGTBL_ER	0x2024
# define PGTBL_ERR_MT_TILING			(1 << 27)
# define PGTBL_ERR_MT_GTT_PTE			(1 << 26)
# define PGTBL_ERR_LC_TILING			(1 << 25)
# define PGTBL_ERR_LC_GTT_PTE			(1 << 24)
# define PGTBL_ERR_BIN_VERTEXDATA_GTT_PTE	(1 << 23)
# define PGTBL_ERR_BIN_INSTRUCTION_GTT_PTE	(1 << 22)
# define PGTBL_ERR_CS_VERTEXDATA_GTT_PTE	(1 << 21)
# define PGTBL_ERR_CS_INSTRUCTION_GTT_PTE	(1 << 20)
# define PGTBL_ERR_CS_GTT		(1 << 19)
# define PGTBL_ERR_OVERLAY_TILING		(1 << 18)
# define PGTBL_ERR_OVERLAY_GTT_PTE		(1 << 16)
# define PGTBL_ERR_DISPC_TILING			(1 << 14)
# define PGTBL_ERR_DISPC_GTT_PTE		(1 << 12)
# define PGTBL_ERR_DISPB_TILING			(1 << 10)
# define PGTBL_ERR_DISPB_GTT_PTE		(1 << 8)
# define PGTBL_ERR_DISPA_TILING			(1 << 6)
# define PGTBL_ERR_DISPA_GTT_PTE		(1 << 4)
# define PGTBL_ERR_HOST_PTE_DATA		(1 << 1)
# define PGTBL_ERR_HOST_GTT_PTE			(1 << 0)
/** @} */

/* Ring buffer registers, p277, overview p19
 */
#define LP_RING     0x2030
#define HP_RING     0x2040

#define RING_TAIL      0x00
#define TAIL_ADDR           0x000FFFF8
#define I830_TAIL_MASK	    0x001FFFF8

#define RING_HEAD      0x04
#define HEAD_WRAP_COUNT     0xFFE00000
#define HEAD_WRAP_ONE       0x00200000
#define HEAD_ADDR           0x001FFFFC
#define I830_HEAD_MASK      0x001FFFFC

#define RING_START     0x08
#define START_ADDR          0x03FFFFF8
#define I830_RING_START_MASK	0xFFFFF000

#define RING_LEN       0x0C
#define RING_NR_PAGES       0x001FF000 
#define I830_RING_NR_PAGES	0x001FF000
#define RING_REPORT_MASK    0x00000006
#define RING_REPORT_64K     0x00000002
#define RING_REPORT_128K    0x00000004
#define RING_NO_REPORT      0x00000000
#define RING_VALID_MASK     0x00000001
#define RING_VALID          0x00000001
#define RING_INVALID        0x00000000



/* BitBlt Instructions
 *
 * There are many more masks & ranges yet to add.
 */
#define BR00_BITBLT_CLIENT   0x40000000
#define BR00_OP_COLOR_BLT    0x10000000
#define BR00_OP_SRC_COPY_BLT 0x10C00000
#define BR00_OP_FULL_BLT     0x11400000
#define BR00_OP_MONO_SRC_BLT 0x11800000
#define BR00_OP_MONO_SRC_COPY_BLT 0x11000000
#define BR00_OP_MONO_PAT_BLT 0x11C00000
#define BR00_OP_MONO_SRC_COPY_IMMEDIATE_BLT (0x61 << 22)
#define BR00_OP_TEXT_IMMEDIATE_BLT 0xc000000


#define BR00_TPCY_DISABLE    0x00000000
#define BR00_TPCY_ENABLE     0x00000010

#define BR00_TPCY_ROP        0x00000000
#define BR00_TPCY_NO_ROP     0x00000020
#define BR00_TPCY_EQ         0x00000000
#define BR00_TPCY_NOT_EQ     0x00000040

#define BR00_PAT_MSB_FIRST   0x00000000	/* ? */

#define BR00_PAT_VERT_ALIGN  0x000000e0

#define BR00_LENGTH          0x0000000F

#define BR09_DEST_ADDR       0x03FFFFFF

#define BR11_SOURCE_PITCH    0x00003FFF

#define BR12_SOURCE_ADDR     0x03FFFFFF

#define BR13_SOLID_PATTERN   0x80000000
#define BR13_RIGHT_TO_LEFT   0x40000000
#define BR13_LEFT_TO_RIGHT   0x00000000
#define BR13_MONO_TRANSPCY   0x20000000
#define BR13_MONO_PATN_TRANS 0x10000000
#define BR13_USE_DYN_DEPTH   0x04000000
#define BR13_DYN_8BPP        0x00000000
#define BR13_DYN_16BPP       0x01000000
#define BR13_DYN_24BPP       0x02000000
#define BR13_ROP_MASK        0x00FF0000
#define BR13_DEST_PITCH      0x0000FFFF
#define BR13_PITCH_SIGN_BIT  0x00008000

#define BR14_DEST_HEIGHT     0xFFFF0000
#define BR14_DEST_WIDTH      0x0000FFFF

#define BR15_PATTERN_ADDR    0x03FFFFFF

#define BR16_SOLID_PAT_COLOR 0x00FFFFFF
#define BR16_BACKGND_PAT_CLR 0x00FFFFFF

#define BR17_FGND_PAT_CLR    0x00FFFFFF

#define BR18_SRC_BGND_CLR    0x00FFFFFF
#define BR19_SRC_FGND_CLR    0x00FFFFFF


/* Instruction parser instructions
 */

#define INST_PARSER_CLIENT   0x00000000
#define INST_OP_FLUSH        0x02000000
#define INST_FLUSH_MAP_CACHE 0x00000001


#define GFX_OP_USER_INTERRUPT ((0<<29)|(2<<23))


/* Registers in the i810 host-pci bridge pci config space which affect
 * the i810 graphics operations.  
 */
#define SMRAM_MISCC         0x70
#define GMS                    0x000000c0
#define GMS_DISABLE            0x00000000
#define GMS_ENABLE_BARE        0x00000040
#define GMS_ENABLE_512K        0x00000080
#define GMS_ENABLE_1M          0x000000c0
#define USMM                   0x00000030 
#define USMM_DISABLE           0x00000000
#define USMM_TSEG_ZERO         0x00000010
#define USMM_TSEG_512K         0x00000020
#define USMM_TSEG_1M           0x00000030  
#define GFX_MEM_WIN_SIZE       0x00010000
#define GFX_MEM_WIN_32M        0x00010000
#define GFX_MEM_WIN_64M        0x00000000

/* Overkill?  I don't know.  Need to figure out top of mem to make the
 * SMRAM calculations come out.  Linux seems to have problems
 * detecting it all on its own, so this seems a reasonable double
 * check to any user supplied 'mem=...' boot param.
 *
 * ... unfortunately this reg doesn't work according to spec on the
 * test hardware.
 */
#define WHTCFG_PAMR_DRP      0x50
#define SYS_DRAM_ROW_0_SHIFT    16
#define SYS_DRAM_ROW_1_SHIFT    20
#define DRAM_MASK           0x0f
#define DRAM_VALUE_0        0
#define DRAM_VALUE_1        8
/* No 2 value defined */
#define DRAM_VALUE_3        16
#define DRAM_VALUE_4        16
#define DRAM_VALUE_5        24
#define DRAM_VALUE_6        32
#define DRAM_VALUE_7        32
#define DRAM_VALUE_8        48
#define DRAM_VALUE_9        64
#define DRAM_VALUE_A        64
#define DRAM_VALUE_B        96
#define DRAM_VALUE_C        128
#define DRAM_VALUE_D        128
#define DRAM_VALUE_E        192
#define DRAM_VALUE_F        256	/* nice one, geezer */
#define LM_FREQ_MASK        0x10
#define LM_FREQ_133         0x10
#define LM_FREQ_100         0x00




/* These are 3d state registers, but the state is invarient, so we let
 * the X server handle it:
 */



/* GFXRENDERSTATE_COLOR_CHROMA_KEY, p135
 */
#define GFX_OP_COLOR_CHROMA_KEY  ((0x3<<29)|(0x1d<<24)|(0x2<<16)|0x1)
#define CC1_UPDATE_KILL_WRITE    (1<<28)
#define CC1_ENABLE_KILL_WRITE    (1<<27)
#define CC1_DISABLE_KILL_WRITE    0
#define CC1_UPDATE_COLOR_IDX     (1<<26)
#define CC1_UPDATE_CHROMA_LOW    (1<<25)
#define CC1_UPDATE_CHROMA_HI     (1<<24)
#define CC1_CHROMA_LOW_MASK      ((1<<24)-1)
#define CC2_COLOR_IDX_SHIFT      24
#define CC2_COLOR_IDX_MASK       (0xff<<24)
#define CC2_CHROMA_HI_MASK       ((1<<24)-1)


#define GFX_CMD_CONTEXT_SEL      ((0<<29)|(0x5<<23))
#define CS_UPDATE_LOAD           (1<<17)
#define CS_UPDATE_USE            (1<<16)
#define CS_UPDATE_LOAD           (1<<17)
#define CS_LOAD_CTX0             0
#define CS_LOAD_CTX1             (1<<8)
#define CS_USE_CTX0              0
#define CS_USE_CTX1              (1<<0)

/* I810 LCD/TV registers */
#define LCD_TV_HTOTAL	0x60000
#define LCD_TV_C	0x60018
#define LCD_TV_OVRACT   0x6001C

#define LCD_TV_ENABLE (1 << 31)
#define LCD_TV_VGAMOD (1 << 28)

/* I830 CRTC registers */
#define HTOTAL_A	0x60000
#define HBLANK_A	0x60004
#define HSYNC_A 	0x60008
#define VTOTAL_A	0x6000c
#define VBLANK_A	0x60010
#define VSYNC_A 	0x60014
#define PIPEASRC	0x6001c
#define BCLRPAT_A	0x60020
#define VSYNCSHIFT_A	0x60028

#define HTOTAL_B	0x61000
#define HBLANK_B	0x61004
#define HSYNC_B 	0x61008
#define VTOTAL_B	0x6100c
#define VBLANK_B	0x61010
#define VSYNC_B 	0x61014
#define PIPEBSRC	0x6101c
#define BCLRPAT_B	0x61020
#define VSYNCSHIFT_B	0x61028

#define PP_STATUS	0x61200
# define PP_ON					(1 << 31)
/**
 * Indicates that all dependencies of the panel are on:
 *
 * - PLL enabled
 * - pipe enabled
 * - LVDS/DVOB/DVOC on
 */
# define PP_READY				(1 << 30)
# define PP_SEQUENCE_NONE			(0 << 28)
# define PP_SEQUENCE_ON				(1 << 28)
# define PP_SEQUENCE_OFF			(2 << 28)
# define PP_SEQUENCE_MASK			0x30000000

#define PP_CONTROL	0x61204
# define POWER_DOWN_ON_RESET			(1 << 1)
# define POWER_TARGET_ON			(1 << 0)

#define PP_ON_DELAYS	0x61208
#define PP_OFF_DELAYS	0x6120c
#define PP_DIVISOR	0x61210

#define PFIT_CONTROL	0x61230
# define PFIT_ENABLE				(1 << 31)
/* Pre-965 */
# define VERT_INTERP_DISABLE			(0 << 10)
# define VERT_INTERP_BILINEAR			(1 << 10)
# define VERT_INTERP_MASK			(3 << 10)
# define VERT_AUTO_SCALE			(1 << 9)
# define HORIZ_INTERP_DISABLE			(0 << 6)
# define HORIZ_INTERP_BILINEAR			(1 << 6)
# define HORIZ_INTERP_MASK			(3 << 6)
# define HORIZ_AUTO_SCALE			(1 << 5)
# define PANEL_8TO6_DITHER_ENABLE		(1 << 3)
/* 965+ */
# define PFIT_PIPE_MASK				(3 << 29)
# define PFIT_PIPE_SHIFT			29
# define PFIT_SCALING_MODE_MASK			(7 << 26)
#  define PFIT_SCALING_AUTO			(0 << 26)
#  define PFIT_SCALING_PROGRAMMED		(1 << 26)
#  define PFIT_SCALING_PILLAR			(2 << 26)
#  define PFIT_SCALING_LETTER			(3 << 26)
# define PFIT_FILTER_SELECT_MASK		(3 << 24)
#  define PFIT_FILTER_FUZZY			(0 << 24)
#  define PFIT_FILTER_CRISP			(1 << 24)
#  define PFIT_FILTER_MEDIAN			(2 << 24)

#define PFIT_PGM_RATIOS	0x61234
/* Pre-965 */
# define PFIT_VERT_SCALE_SHIFT			20
# define PFIT_VERT_SCALE_MASK			0xfff00000
# define PFIT_HORIZ_SCALE_SHIFT			4
# define PFIT_HORIZ_SCALE_MASK			0x0000fff0
/* 965+ */
# define PFIT_VERT_SCALE_SHIFT_965		16
# define PFIT_VERT_SCALE_MASK_965		0x1fff0000
# define PFIT_HORIZ_SCALE_SHIFT_965		0
# define PFIT_HORIZ_SCALE_MASK_965		0x00001fff

#define DPLL_A		0x06014
#define DPLL_B		0x06018
# define DPLL_VCO_ENABLE			(1 << 31)
# define DPLL_DVO_HIGH_SPEED			(1 << 30)
# define DPLL_SYNCLOCK_ENABLE			(1 << 29)
# define DPLL_VGA_MODE_DIS			(1 << 28)
# define DPLLB_MODE_DAC_SERIAL			(1 << 26) /* i915 */
# define DPLLB_MODE_LVDS			(2 << 26) /* i915 */
# define DPLL_MODE_MASK				(3 << 26)
# define DPLL_DAC_SERIAL_P2_CLOCK_DIV_10	(0 << 24) /* i915 */
# define DPLL_DAC_SERIAL_P2_CLOCK_DIV_5		(1 << 24) /* i915 */
# define DPLLB_LVDS_P2_CLOCK_DIV_14		(0 << 24) /* i915 */
# define DPLLB_LVDS_P2_CLOCK_DIV_7		(1 << 24) /* i915 */
# define DPLL_P2_CLOCK_DIV_MASK			0x03000000 /* i915 */
# define DPLL_FPA01_P1_POST_DIV_MASK		0x00ff0000 /* i915 */
# define DPLL_FPA01_P1_POST_DIV_MASK_IGD	0x00ff8000 /* IGD */
/**
 *  The i830 generation, in DAC/serial mode, defines p1 as two plus this
 * bitfield, or just 2 if PLL_P1_DIVIDE_BY_TWO is set.
 */
# define DPLL_FPA01_P1_POST_DIV_MASK_I830	0x001f0000
/**
 * The i830 generation, in LVDS mode, defines P1 as the bit number set within
 * this field (only one bit may be set).
 */
# define DPLL_FPA01_P1_POST_DIV_MASK_I830_LVDS	0x003f0000
# define DPLL_FPA01_P1_POST_DIV_SHIFT		16
# define DPLL_FPA01_P1_POST_DIV_SHIFT_IGD	15
/* Ironlake */
# define DPLL_FPA0_P1_POST_DIV_SHIFT		16

# define PLL_P2_DIVIDE_BY_4			(1 << 23) /* i830, required in DVO non-gang */
# define PLL_P1_DIVIDE_BY_TWO			(1 << 21) /* i830 */
# define PLL_REF_INPUT_DREFCLK			(0 << 13)
# define PLL_REF_INPUT_TVCLKINA			(1 << 13) /* i830 */
# define PLL_REF_INPUT_SUPER_SSC		(1 << 13) /* Ironlake: 120M SSC */
# define PLL_REF_INPUT_TVCLKINBC		(2 << 13) /* SDVO TVCLKIN */
# define PLLB_REF_INPUT_SPREADSPECTRUMIN	(3 << 13)
# define PLL_REF_INPUT_MASK			(3 << 13)
# define PLL_REF_INPUT_DMICLK			(5 << 13) /* Ironlake: DMI refclk */
# define PLL_LOAD_PULSE_PHASE_SHIFT		9
/*
 * Parallel to Serial Load Pulse phase selection.
 * Selects the phase for the 10X DPLL clock for the PCIe
 * digital display port. The range is 4 to 13; 10 or more
 * is just a flip delay. The default is 6
 */
# define PLL_LOAD_PULSE_PHASE_MASK		(0xf << PLL_LOAD_PULSE_PHASE_SHIFT)
# define DISPLAY_RATE_SELECT_FPA1		(1 << 8)
/* Ironlake */
# define PLL_REF_SDVO_HDMI_MULTIPLIER_SHIFT	9
# define PLL_REF_SDVO_HDMI_MULTIPLIER_MASK	(7 << 9)
# define PLL_REF_SDVO_HDMI_MULTIPLIER(x)	(((x)-1)<< PLL_REF_SDVO_HDMI_MULTIPLIER_SHIFT)
# define DPLL_FPA1_P1_POST_DIV_SHIFT		0
# define DPLL_FPA1_P1_POST_DIV_MASK		0xff

/**
 * SDVO multiplier for 945G/GM. Not used on 965.
 *
 * \sa DPLL_MD_UDI_MULTIPLIER_MASK
 */
# define SDVO_MULTIPLIER_MASK			0x000000ff
# define SDVO_MULTIPLIER_SHIFT_HIRES		4
# define SDVO_MULTIPLIER_SHIFT_VGA		0

/** @defgroup DPLL_MD
 * @{
 */
/** Pipe A SDVO/UDI clock multiplier/divider register for G965. */
#define DPLL_A_MD		0x0601c
/** Pipe B SDVO/UDI clock multiplier/divider register for G965. */
#define DPLL_B_MD		0x06020
/**
 * UDI pixel divider, controlling how many pixels are stuffed into a packet.
 *
 * Value is pixels minus 1.  Must be set to 1 pixel for SDVO.
 */
# define DPLL_MD_UDI_DIVIDER_MASK		0x3f000000
# define DPLL_MD_UDI_DIVIDER_SHIFT		24
/** UDI pixel divider for VGA, same as DPLL_MD_UDI_DIVIDER_MASK. */
# define DPLL_MD_VGA_UDI_DIVIDER_MASK		0x003f0000
# define DPLL_MD_VGA_UDI_DIVIDER_SHIFT		16
/**
 * SDVO/UDI pixel multiplier.
 *
 * SDVO requires that the bus clock rate be between 1 and 2 Ghz, and the bus
 * clock rate is 10 times the DPLL clock.  At low resolution/refresh rate
 * modes, the bus rate would be below the limits, so SDVO allows for stuffing
 * dummy bytes in the datastream at an increased clock rate, with both sides of
 * the link knowing how many bytes are fill.
 *
 * So, for a mode with a dotclock of 65Mhz, we would want to double the clock
 * rate to 130Mhz to get a bus rate of 1.30Ghz.  The DPLL clock rate would be
 * set to 130Mhz, and the SDVO multiplier set to 2x in this register and
 * through an SDVO command.
 *
 * This register field has values of multiplication factor minus 1, with
 * a maximum multiplier of 5 for SDVO.
 */
# define DPLL_MD_UDI_MULTIPLIER_MASK		0x00003f00
# define DPLL_MD_UDI_MULTIPLIER_SHIFT		8
/** SDVO/UDI pixel multiplier for VGA, same as DPLL_MD_UDI_MULTIPLIER_MASK. 
 * This best be set to the default value (3) or the CRT won't work. No,
 * I don't entirely understand what this does...
 */
# define DPLL_MD_VGA_UDI_MULTIPLIER_MASK	0x0000003f
# define DPLL_MD_VGA_UDI_MULTIPLIER_SHIFT	0
/** @} */

#define DPLL_TEST		0x606c
# define DPLLB_TEST_SDVO_DIV_1			(0 << 22)
# define DPLLB_TEST_SDVO_DIV_2			(1 << 22)
# define DPLLB_TEST_SDVO_DIV_4			(2 << 22)
# define DPLLB_TEST_SDVO_DIV_MASK		(3 << 22)
# define DPLLB_TEST_N_BYPASS			(1 << 19)
# define DPLLB_TEST_M_BYPASS			(1 << 18)
# define DPLLB_INPUT_BUFFER_ENABLE		(1 << 16)
# define DPLLA_TEST_N_BYPASS			(1 << 3)
# define DPLLA_TEST_M_BYPASS			(1 << 2)
# define DPLLA_INPUT_BUFFER_ENABLE		(1 << 0)

#define D_STATE			0x6104
#define DSPCLK_GATE_D		0x6200
# define DPUNIT_B_CLOCK_GATE_DISABLE		(1 << 30) /* 965 */
# define VSUNIT_CLOCK_GATE_DISABLE		(1 << 29) /* 965 */
# define VRHUNIT_CLOCK_GATE_DISABLE		(1 << 28) /* 965 */
# define VRDUNIT_CLOCK_GATE_DISABLE		(1 << 27) /* 965 */
# define AUDUNIT_CLOCK_GATE_DISABLE		(1 << 26) /* 965 */
# define DPUNIT_A_CLOCK_GATE_DISABLE		(1 << 25) /* 965 */
# define DPCUNIT_CLOCK_GATE_DISABLE		(1 << 24) /* 965 */
# define TVRUNIT_CLOCK_GATE_DISABLE		(1 << 23) /* 915-945 */
# define TVCUNIT_CLOCK_GATE_DISABLE		(1 << 22) /* 915-945 */
# define TVFUNIT_CLOCK_GATE_DISABLE		(1 << 21) /* 915-945 */
# define TVEUNIT_CLOCK_GATE_DISABLE		(1 << 20) /* 915-945 */
# define DVSUNIT_CLOCK_GATE_DISABLE		(1 << 19) /* 915-945 */
# define DSSUNIT_CLOCK_GATE_DISABLE		(1 << 18) /* 915-945 */
# define DDBUNIT_CLOCK_GATE_DISABLE		(1 << 17) /* 915-945 */
# define DPRUNIT_CLOCK_GATE_DISABLE		(1 << 16) /* 915-945 */
# define DPFUNIT_CLOCK_GATE_DISABLE		(1 << 15) /* 915-945 */
# define DPBMUNIT_CLOCK_GATE_DISABLE		(1 << 14) /* 915-945 */
# define DPLSUNIT_CLOCK_GATE_DISABLE		(1 << 13) /* 915-945 */
# define DPLUNIT_CLOCK_GATE_DISABLE		(1 << 12) /* 915-945 */
# define DPOUNIT_CLOCK_GATE_DISABLE		(1 << 11)
# define DPBUNIT_CLOCK_GATE_DISABLE		(1 << 10)
# define DCUNIT_CLOCK_GATE_DISABLE		(1 << 9)
# define DPUNIT_CLOCK_GATE_DISABLE		(1 << 8)
# define VRUNIT_CLOCK_GATE_DISABLE		(1 << 7) /* 915+: reserved */
# define OVHUNIT_CLOCK_GATE_DISABLE		(1 << 6) /* 830-865 */
# define DPIOUNIT_CLOCK_GATE_DISABLE		(1 << 6) /* 915-945 */
# define OVFUNIT_CLOCK_GATE_DISABLE		(1 << 5)
# define OVBUNIT_CLOCK_GATE_DISABLE		(1 << 4)
/**
 * This bit must be set on the 830 to prevent hangs when turning off the
 * overlay scaler.
 */
# define OVRUNIT_CLOCK_GATE_DISABLE		(1 << 3)
# define OVCUNIT_CLOCK_GATE_DISABLE		(1 << 2)
# define OVUUNIT_CLOCK_GATE_DISABLE		(1 << 1)
# define ZVUNIT_CLOCK_GATE_DISABLE		(1 << 0) /* 830 */
# define OVLUNIT_CLOCK_GATE_DISABLE		(1 << 0) /* 845,865 */

#define RENCLK_GATE_D1		0x6204
# define BLITTER_CLOCK_GATE_DISABLE		(1 << 13) /* 945GM only */
# define MPEG_CLOCK_GATE_DISABLE		(1 << 12) /* 945GM only */
# define PC_FE_CLOCK_GATE_DISABLE		(1 << 11)
# define PC_BE_CLOCK_GATE_DISABLE		(1 << 10)
# define WINDOWER_CLOCK_GATE_DISABLE		(1 << 9)
# define INTERPOLATOR_CLOCK_GATE_DISABLE	(1 << 8)
# define COLOR_CALCULATOR_CLOCK_GATE_DISABLE	(1 << 7)
# define MOTION_COMP_CLOCK_GATE_DISABLE		(1 << 6)
# define MAG_CLOCK_GATE_DISABLE			(1 << 5)
/** This bit must be unset on 855,865 */
# define MECI_CLOCK_GATE_DISABLE		(1 << 4)
# define DCMP_CLOCK_GATE_DISABLE		(1 << 3)
# define MEC_CLOCK_GATE_DISABLE			(1 << 2)
# define MECO_CLOCK_GATE_DISABLE		(1 << 1)
/** This bit must be set on 855,865. */
# define SV_CLOCK_GATE_DISABLE			(1 << 0)
# define I915_MPEG_CLOCK_GATE_DISABLE		(1 << 16)
# define I915_VLD_IP_PR_CLOCK_GATE_DISABLE	(1 << 15)
# define I915_MOTION_COMP_CLOCK_GATE_DISABLE	(1 << 14)
# define I915_BD_BF_CLOCK_GATE_DISABLE		(1 << 13)
# define I915_SF_SE_CLOCK_GATE_DISABLE		(1 << 12)
# define I915_WM_CLOCK_GATE_DISABLE		(1 << 11)
# define I915_IZ_CLOCK_GATE_DISABLE		(1 << 10)
# define I915_PI_CLOCK_GATE_DISABLE		(1 << 9)
# define I915_DI_CLOCK_GATE_DISABLE		(1 << 8)
# define I915_SH_SV_CLOCK_GATE_DISABLE		(1 << 7)
# define I915_PL_DG_QC_FT_CLOCK_GATE_DISABLE	(1 << 6)
# define I915_SC_CLOCK_GATE_DISABLE		(1 << 5)
# define I915_FL_CLOCK_GATE_DISABLE		(1 << 4)
# define I915_DM_CLOCK_GATE_DISABLE		(1 << 3)
# define I915_PS_CLOCK_GATE_DISABLE		(1 << 2)
# define I915_CC_CLOCK_GATE_DISABLE		(1 << 1)
# define I915_BY_CLOCK_GATE_DISABLE		(1 << 0)

# define I965_RCZ_CLOCK_GATE_DISABLE		(1 << 30)
/** This bit must always be set on 965G/965GM */
# define I965_RCC_CLOCK_GATE_DISABLE		(1 << 29)
# define I965_RCPB_CLOCK_GATE_DISABLE		(1 << 28)
# define I965_DAP_CLOCK_GATE_DISABLE		(1 << 27)
# define I965_ROC_CLOCK_GATE_DISABLE		(1 << 26)
# define I965_GW_CLOCK_GATE_DISABLE		(1 << 25)
# define I965_TD_CLOCK_GATE_DISABLE		(1 << 24)
/** This bit must always be set on 965G */
# define I965_ISC_CLOCK_GATE_DISABLE		(1 << 23)
# define I965_IC_CLOCK_GATE_DISABLE		(1 << 22)
# define I965_EU_CLOCK_GATE_DISABLE		(1 << 21)
# define I965_IF_CLOCK_GATE_DISABLE		(1 << 20)
# define I965_TC_CLOCK_GATE_DISABLE		(1 << 19)
# define I965_SO_CLOCK_GATE_DISABLE		(1 << 17)
# define I965_FBC_CLOCK_GATE_DISABLE		(1 << 16)
# define I965_MARI_CLOCK_GATE_DISABLE		(1 << 15)
# define I965_MASF_CLOCK_GATE_DISABLE		(1 << 14)
# define I965_MAWB_CLOCK_GATE_DISABLE		(1 << 13)
# define I965_EM_CLOCK_GATE_DISABLE		(1 << 12)
# define I965_UC_CLOCK_GATE_DISABLE		(1 << 11)
# define I965_SI_CLOCK_GATE_DISABLE		(1 << 6)
# define I965_MT_CLOCK_GATE_DISABLE		(1 << 5)
# define I965_PL_CLOCK_GATE_DISABLE		(1 << 4)
# define I965_DG_CLOCK_GATE_DISABLE		(1 << 3)
# define I965_QC_CLOCK_GATE_DISABLE		(1 << 2)
# define I965_FT_CLOCK_GATE_DISABLE		(1 << 1)
# define I965_DM_CLOCK_GATE_DISABLE		(1 << 0)

#define RENCLK_GATE_D2		0x6208
#define VF_UNIT_CLOCK_GATE_DISABLE		(1 << 9)
#define GS_UNIT_CLOCK_GATE_DISABLE		(1 << 7)
#define CL_UNIT_CLOCK_GATE_DISABLE		(1 << 6)
#define RAMCLK_GATE_D		0x6210		/* CRL only */
#define DEUC			0x6214          /* CRL only */

/*
 * This is a PCI config space register to manipulate backlight brightness
 * It is used when the BLM_LEGACY_MODE is turned on. When enabled, the first
 * byte of this config register sets brightness within the range from
 * 0 to 0xff
 */
#define LEGACY_BACKLIGHT_BRIGHTNESS 0xf4

#define BLC_PWM_CTL		0x61254
#define BACKLIGHT_MODULATION_FREQ_SHIFT		(17)
#define BACKLIGHT_MODULATION_FREQ_SHIFT2	(16)
/**
 * This is the most significant 15 bits of the number of backlight cycles in a
 * complete cycle of the modulated backlight control.
 *
 * The actual value is this field multiplied by two.
 */
#define BACKLIGHT_MODULATION_FREQ_MASK		(0x7fff << 17)
#define BACKLIGHT_MODULATION_FREQ_MASK2		(0xffff << 16)
#define BLM_LEGACY_MODE				(1 << 16)

/**
 * This is the number of cycles out of the backlight modulation cycle for which
 * the backlight is on.
 *
 * This field must be no greater than the number of cycles in the complete
 * backlight modulation cycle.
 */
#define BACKLIGHT_DUTY_CYCLE_SHIFT		(0)
#define BACKLIGHT_DUTY_CYCLE_MASK		(0xffff)

/* On 965+ backlight control is in another register */
#define BLC_PWM_CTL2			0x61250
#define 	BLM_LEGACY_MODE2	(1 << 30)

#define BLM_CTL			0x61260
#define BLM_THRESHOLD_0		0x61270
#define BLM_THRESHOLD_1		0x61274
#define BLM_THRESHOLD_2		0x61278
#define BLM_THRESHOLD_3		0x6127c
#define BLM_THRESHOLD_4		0x61280
#define BLM_THRESHOLD_5		0x61284

#define BLM_ACCUMULATOR_0	0x61290
#define BLM_ACCUMULATOR_1	0x61294
#define BLM_ACCUMULATOR_2	0x61298
#define BLM_ACCUMULATOR_3	0x6129c
#define BLM_ACCUMULATOR_4	0x612a0
#define BLM_ACCUMULATOR_5	0x612a4

#define FPA0		0x06040
#define FPA1		0x06044
#define FPB0		0x06048
#define FPB1		0x0604c
# define FP_N_DIV_MASK				0x003f0000
# define FP_N_IGD_DIV_MASK			0x00ff0000
# define FP_N_DIV_SHIFT				16
# define FP_M1_DIV_MASK				0x00003f00
# define FP_M1_DIV_SHIFT			8
# define FP_M2_DIV_MASK				0x0000003f
# define FP_M2_IGD_DIV_MASK			0x000000ff
# define FP_M2_DIV_SHIFT			0

#define PORT_HOTPLUG_EN		0x61110
# define HDMIB_HOTPLUG_INT_EN			(1 << 29)
# define HDMIC_HOTPLUG_INT_EN			(1 << 28)
# define HDMID_HOTPLUG_INT_EN			(1 << 27)
# define SDVOB_HOTPLUG_INT_EN			(1 << 26)
# define SDVOC_HOTPLUG_INT_EN			(1 << 25)
# define TV_HOTPLUG_INT_EN			(1 << 18)
# define CRT_HOTPLUG_INT_EN			(1 << 9)
# define CRT_HOTPLUG_ACTIVATION_PERIOD_32	(0 << 8)
/* must use period 64 on GM45 according to docs */
# define CRT_HOTPLUG_ACTIVATION_PERIOD_64	(1 << 8)
# define CRT_HOTPLUG_DAC_ON_TIME_2M		(0 << 7)
# define CRT_HOTPLUG_DAC_ON_TIME_4M		(1 << 7)
# define CRT_HOTPLUG_VOLTAGE_COMPARE_40		(0 << 5)
# define CRT_HOTPLUG_VOLTAGE_COMPARE_50		(1 << 5)
# define CRT_HOTPLUG_VOLTAGE_COMPARE_60		(2 << 5)
# define CRT_HOTPLUG_VOLTAGE_COMPARE_70		(3 << 5)
# define CRT_HOTPLUG_VOLTAGE_COMPARE_MASK	(3 << 5)
# define CRT_HOTPLUG_DETECT_DELAY_1G		(0 << 4)
# define CRT_HOTPLUG_DETECT_DELAY_2G		(1 << 4)
# define CRT_HOTPLUG_FORCE_DETECT		(1 << 3)
# define CRT_HOTPLUG_DETECT_VOLTAGE_325MV	(0 << 2)
# define CRT_HOTPLUG_DETECT_VOLTAGE_475MV	(1 << 2)
# define CRT_HOTPLUG_MASK			(0x3fc)	/* Bits 9-2 */
# define CRT_FORCE_HOTPLUG_MASK                 0xfffffe1f

#define PORT_HOTPLUG_STAT	0x61114
# define HDMIB_HOTPLUG_INT_STATUS		(1 << 29)
# define HDMIC_HOTPLUG_INT_STATUS		(1 << 28)
# define HDMID_HOTPLUG_INT_STATUS		(1 << 27)
# define CRT_HOTPLUG_INT_STATUS			(1 << 11)
# define TV_HOTPLUG_INT_STATUS			(1 << 10)
# define CRT_HOTPLUG_MONITOR_MASK		(3 << 8)
# define CRT_HOTPLUG_MONITOR_COLOR		(3 << 8)
# define CRT_HOTPLUG_MONITOR_MONO		(2 << 8)
# define CRT_HOTPLUG_MONITOR_NONE		(0 << 8)
# define SDVOC_HOTPLUG_INT_STATUS		(1 << 7)
# define SDVOB_HOTPLUG_INT_STATUS		(1 << 6)

#define SDVOB			0x61140
#define SDVOC			0x61160
#define SDVO_ENABLE				(1 << 31)
#define SDVO_PIPE_B_SELECT			(1 << 30)
#define SDVO_STALL_SELECT			(1 << 29)
#define SDVO_INTERRUPT_ENABLE			(1 << 26)
/**
 * 915G/GM SDVO pixel multiplier.
 *
 * Programmed value is multiplier - 1, up to 5x.
 *
 * \sa DPLL_MD_UDI_MULTIPLIER_MASK
 */
#define SDVO_PORT_MULTIPLY_MASK			(7 << 23)
#define SDVO_PORT_MULTIPLY_SHIFT		23
#define SDVO_PHASE_SELECT_MASK			(15 << 19)
#define SDVO_PHASE_SELECT_DEFAULT		(6 << 19)
#define SDVO_CLOCK_OUTPUT_INVERT		(1 << 18)
#define SDVOC_GANG_MODE				(1 << 16)
#define SDVO_ENCODING_SDVO			(0x0 << 10)
#define SDVO_ENCODING_HDMI			(0x2 << 10)
/** Requird for HDMI operation */
#define SDVO_NULL_PACKETS_DURING_VSYNC		(1 << 9)
#define SDVO_COLOR_NOT_FULL_RANGE		(1 << 8)
#define SDVO_BORDER_ENABLE			(1 << 7)
#define SDVO_AUDIO_ENABLE			(1 << 6)
/** New with 965, default is to be set */
#define SDVO_VSYNC_ACTIVE_HIGH			(1 << 4)
/** New with 965, default is to be set */
#define SDVO_HSYNC_ACTIVE_HIGH			(1 << 3)
/** 915/945 only, read-only bit */
#define SDVOB_PCIE_CONCURRENCY			(1 << 3)
#define SDVO_DETECTED				(1 << 2)
/* Bits to be preserved when writing */
#define SDVOB_PRESERVE_MASK			((1 << 17) | (1 << 16) | (1 << 14))
#define SDVOC_PRESERVE_MASK			(1 << 17)

#define UDIB_SVB_SHB_CODES    		0x61144
#define UDIB_SHA_BLANK_CODES		0x61148
#define UDIB_START_END_FILL_CODES	0x6114c


#define SDVOUDI				0x61150

#define I830_HTOTAL_MASK 	0xfff0000
#define I830_HACTIVE_MASK	0x7ff

#define I830_HBLANKEND_MASK	0xfff0000
#define I830_HBLANKSTART_MASK    0xfff

#define I830_HSYNCEND_MASK	0xfff0000
#define I830_HSYNCSTART_MASK    0xfff

#define I830_VTOTAL_MASK 	0xfff0000
#define I830_VACTIVE_MASK	0x7ff

#define I830_VBLANKEND_MASK	0xfff0000
#define I830_VBLANKSTART_MASK    0xfff

#define I830_VSYNCEND_MASK	0xfff0000
#define I830_VSYNCSTART_MASK    0xfff

#define I830_PIPEA_HORZ_MASK	0x7ff0000
#define I830_PIPEA_VERT_MASK	0x7ff

#define ADPA			0x61100
#define ADPA_DAC_ENABLE 	(1<<31)
#define ADPA_DAC_DISABLE	0
#define ADPA_PIPE_SELECT_MASK	(1<<30)
#define ADPA_PIPE_A_SELECT	0
#define ADPA_PIPE_B_SELECT	(1<<30)
#define ADPA_USE_VGA_HVPOLARITY (1<<15)
#define ADPA_SETS_HVPOLARITY	0
#define ADPA_VSYNC_CNTL_DISABLE (1<<11)
#define ADPA_VSYNC_CNTL_ENABLE	0
#define ADPA_HSYNC_CNTL_DISABLE (1<<10)
#define ADPA_HSYNC_CNTL_ENABLE	0
#define ADPA_VSYNC_ACTIVE_HIGH	(1<<4)
#define ADPA_VSYNC_ACTIVE_LOW	0
#define ADPA_HSYNC_ACTIVE_HIGH	(1<<3)
#define ADPA_HSYNC_ACTIVE_LOW	0


#define DVOA			0x61120
#define DVOB			0x61140
#define DVOC			0x61160
#define DVO_ENABLE			(1 << 31)
#define DVO_PIPE_B_SELECT		(1 << 30)
#define DVO_PIPE_STALL_UNUSED		(0 << 28)
#define DVO_PIPE_STALL			(1 << 28)
#define DVO_PIPE_STALL_TV		(2 << 28)
#define DVO_PIPE_STALL_MASK		(3 << 28)
#define DVO_USE_VGA_SYNC		(1 << 15)
#define DVO_DATA_ORDER_I740		(0 << 14)
#define DVO_DATA_ORDER_FP		(1 << 14)
#define DVO_VSYNC_DISABLE		(1 << 11)
#define DVO_HSYNC_DISABLE		(1 << 10)
#define DVO_VSYNC_TRISTATE		(1 << 9)
#define DVO_HSYNC_TRISTATE		(1 << 8)
#define DVO_BORDER_ENABLE		(1 << 7)
#define DVO_DATA_ORDER_GBRG		(1 << 6)
#define DVO_DATA_ORDER_RGGB		(0 << 6)
#define DVO_DATA_ORDER_GBRG_ERRATA	(0 << 6)
#define DVO_DATA_ORDER_RGGB_ERRATA	(1 << 6)
#define DVO_VSYNC_ACTIVE_HIGH		(1 << 4)
#define DVO_HSYNC_ACTIVE_HIGH		(1 << 3)
#define DVO_BLANK_ACTIVE_HIGH		(1 << 2)
#define DVO_OUTPUT_CSTATE_PIXELS	(1 << 1)	/* SDG only */
#define DVO_OUTPUT_SOURCE_SIZE_PIXELS	(1 << 0)	/* SDG only */
#define DVO_PRESERVE_MASK	(0x7<<24)

#define DVOA_SRCDIM		0x61124
#define DVOB_SRCDIM		0x61144
#define DVOC_SRCDIM		0x61164
#define DVO_SRCDIM_HORIZONTAL_SHIFT	12
#define DVO_SRCDIM_VERTICAL_SHIFT	0

/** @defgroup LVDS
 * @{
 */
/**
 * This register controls the LVDS output enable, pipe selection, and data
 * format selection.
 *
 * All of the clock/data pairs are force powered down by power sequencing.
 */
#define LVDS			0x61180
/**
 * Enables the LVDS port.  This bit must be set before DPLLs are enabled, as
 * the DPLL semantics change when the LVDS is assigned to that pipe.
 */
# define LVDS_PORT_EN			(1 << 31)
/** Selects pipe B for LVDS data.  Must be set on pre-965. */
# define LVDS_PIPEB_SELECT		(1 << 30)

/* on 965, dithering is enabled in this register, not PFIT_CONTROL */
# define LVDS_DITHER_ENABLE		(1 << 25)

/*
 * Selects between .0 and .1 formats:
 *
 * 0 = 1x18.0, 2x18.0, 1x24.0 or 2x24.0
 * 1 = 1x24.1 or 2x24.1
 */
# define LVDS_DATA_FORMAT_DOT_ONE	(1 << 24)

/* Using LE instead of HS on second channel control signal */
# define LVDS_LE_CONTROL_ENABLE		(1 << 23)

/* Using LF instead of VS on second channel control signal */
# define LVDS_LF_CONTROL_ENABLE		(1 << 22)

/* invert vsync signal polarity */
# define LVDS_VSYNC_POLARITY_INVERT	(1 << 21)

/* invert hsync signal polarity */
# define LVDS_HSYNC_POLARITY_INVERT	(1 << 20)

/* invert display enable signal polarity */
# define LVDS_DE_POLARITY_INVERT	(1 << 19)

/*
 * Control signals for second channel, ignored in single channel modes
 */

/* send DE, HS, VS on second channel */
# define LVDS_SECOND_CHANNEL_DE_HS_VS	(0 << 17)

# define LVDS_SECOND_CHANNEL_RESERVED	(1 << 17)

/* Send zeros instead of DE, HS, VS on second channel */
# define LVDS_SECOND_CHANNEL_ZEROS	(2 << 17)

/* Set DE=0, HS=LE, VS=LF on second channel */
# define LVDS_SECOND_CHANNEL_HS_VS	(3 << 17)

/*
 * Send duplicate data for channel reserved bits, otherwise send zeros
 */
# define LVDS_CHANNEL_DUP_RESERVED	(1 << 16)

/*
 * Enable border for unscaled (or aspect-scaled) display
 */
# define LVDS_BORDER_ENABLE		(1 << 15)

/*
 * Tri-state the LVDS buffers when powered down, otherwise
 * they are set to 0V
 */
# define LVDS_POWER_DOWN_TRI_STATE	(1 << 10)

/**
 * Enables the A0-A2 data pairs and CLKA, containing 18 bits of color data per
 * pixel.
 */
# define LVDS_A0A2_CLKA_POWER_MASK	(3 << 8)
# define LVDS_A0A2_CLKA_POWER_DOWN	(0 << 8)
# define LVDS_A0A2_CLKA_POWER_UP	(3 << 8)
/**
 * Controls the A3 data pair, which contains the additional LSBs for 24 bit
 * mode.  Only enabled if LVDS_A0A2_CLKA_POWER_UP also indicates it should be
 * on.
 */
# define LVDS_A3_POWER_MASK		(3 << 6)
# define LVDS_A3_POWER_DOWN		(0 << 6)
# define LVDS_A3_POWER_UP		(3 << 6)
/**
 * Controls the CLKB pair.  This should only be set when LVDS_B0B3_POWER_UP
 * is set.
 */
# define LVDS_CLKB_POWER_MASK		(3 << 4)
# define LVDS_CLKB_POWER_DOWN		(0 << 4)
# define LVDS_CLKB_POWER_UP		(3 << 4)

/**
 * Controls the B0-B3 data pairs.  This must be set to match the DPLL p2
 * setting for whether we are in dual-channel mode.  The B3 pair will
 * additionally only be powered up when LVDS_A3_POWER_UP is set.
 */
# define LVDS_B0B3_POWER_MASK		(3 << 2)
# define LVDS_B0B3_POWER_DOWN		(0 << 2)
# define LVDS_B0B3_POWER_UP		(3 << 2)

/** @} */

#define DP_B			0x64100
#define DPB_AUX_CH_CTL		0x64110
#define DPB_AUX_CH_DATA1	0x64114
#define DPB_AUX_CH_DATA2	0x64118
#define DPB_AUX_CH_DATA3	0x6411c
#define DPB_AUX_CH_DATA4	0x64120
#define DPB_AUX_CH_DATA5	0x64124

#define DP_C			0x64200
#define DPC_AUX_CH_CTL		0x64210
#define DPC_AUX_CH_DATA1	0x64214
#define DPC_AUX_CH_DATA2	0x64218
#define DPC_AUX_CH_DATA3	0x6421c
#define DPC_AUX_CH_DATA4	0x64220
#define DPC_AUX_CH_DATA5	0x64224

#define DP_D			0x64300
#define DPD_AUX_CH_CTL		0x64310
#define DPD_AUX_CH_DATA1	0x64314
#define DPD_AUX_CH_DATA2	0x64318
#define DPD_AUX_CH_DATA3	0x6431c
#define DPD_AUX_CH_DATA4	0x64320
#define DPD_AUX_CH_DATA5	0x64324

/*
 * Two channel clock control. Turn this on if you need clkb for two channel mode
 * Overridden by global LVDS power sequencing
 */

/* clkb off */
# define LVDS_CLKB_POWER_DOWN		(0 << 4)

/* powered up, but clkb forced to 0 */
# define LVDS_CLKB_POWER_PARTIAL	(1 << 4)

/* clock B running */
# define LVDS_CLKB_POWER_UP		(3 << 4)

/*
 * Two channel mode B0-B2 control. Sets state when power is on.
 * Set to POWER_DOWN in single channel mode, other settings enable
 * two channel mode. The CLKB power control controls whether that clock
 * is enabled during two channel mode.
 *
 */
/* Everything is off, including B3 and CLKB */
# define LVDS_B_POWER_DOWN		(0 << 2)

/* B0, B1, B2 and data lines forced to 0. timing is active */
# define LVDS_B_POWER_PARTIAL		(1 << 2)

/* data lines active (both timing and colour) */
# define LVDS_B_POWER_UP		(3 << 2)

/** @defgroup TV_CTL
 * @{
 */
#define TV_CTL			0x68000
/** Enables the TV encoder */
# define TV_ENC_ENABLE			(1 << 31)
/** Sources the TV encoder input from pipe B instead of A. */
# define TV_ENC_PIPEB_SELECT		(1 << 30)
/** Outputs composite video (DAC A only) */
# define TV_ENC_OUTPUT_COMPOSITE	(0 << 28)
/** Outputs SVideo video (DAC B/C) */
# define TV_ENC_OUTPUT_SVIDEO		(1 << 28)
/** Outputs Component video (DAC A/B/C) */
# define TV_ENC_OUTPUT_COMPONENT	(2 << 28)
/** Outputs Composite and SVideo (DAC A/B/C) */
# define TV_ENC_OUTPUT_SVIDEO_COMPOSITE	(3 << 28)
# define TV_TRILEVEL_SYNC		(1 << 21)
/** Enables slow sync generation (945GM only) */
# define TV_SLOW_SYNC			(1 << 20)
/** Selects 4x oversampling for 480i and 576p */
# define TV_OVERSAMPLE_4X		(0 << 18)
/** Selects 2x oversampling for 720p and 1080i */
# define TV_OVERSAMPLE_2X		(1 << 18)
/** Selects no oversampling for 1080p */
# define TV_OVERSAMPLE_NONE		(2 << 18)
/** Selects 8x oversampling */
# define TV_OVERSAMPLE_8X		(3 << 18)
/** Selects progressive mode rather than interlaced */
# define TV_PROGRESSIVE			(1 << 17)
/** Sets the colorburst to PAL mode.  Required for non-M PAL modes. */
# define TV_PAL_BURST			(1 << 16)
/** Field for setting delay of Y compared to C */
# define TV_YC_SKEW_MASK		(7 << 12)
/** Enables a fix for 480p/576p standard definition modes on the 915GM only */
# define TV_ENC_SDP_FIX			(1 << 11)
/**
 * Enables a fix for the 915GM only.
 *
 * Not sure what it does.
 */
# define TV_ENC_C0_FIX			(1 << 10)
/** Bits that must be preserved by software */
# define TV_CTL_SAVE			((1 << 11) | (3 << 9) | (7 << 6) | 0xf)
# define TV_FUSE_STATE_MASK		(3 << 4)
/** Read-only state that reports all features enabled */
# define TV_FUSE_STATE_ENABLED		(0 << 4)
/** Read-only state that reports that Macrovision is disabled in hardware*/
# define TV_FUSE_STATE_NO_MACROVISION	(1 << 4)
/** Read-only state that reports that TV-out is disabled in hardware. */
# define TV_FUSE_STATE_DISABLED		(2 << 4)
/** Normal operation */
# define TV_TEST_MODE_NORMAL		(0 << 0)
/** Encoder test pattern 1 - combo pattern */
# define TV_TEST_MODE_PATTERN_1		(1 << 0)
/** Encoder test pattern 2 - full screen vertical 75% color bars */
# define TV_TEST_MODE_PATTERN_2		(2 << 0)
/** Encoder test pattern 3 - full screen horizontal 75% color bars */
# define TV_TEST_MODE_PATTERN_3		(3 << 0)
/** Encoder test pattern 4 - random noise */
# define TV_TEST_MODE_PATTERN_4		(4 << 0)
/** Encoder test pattern 5 - linear color ramps */
# define TV_TEST_MODE_PATTERN_5		(5 << 0)
/**
 * This test mode forces the DACs to 50% of full output.
 *
 * This is used for load detection in combination with TVDAC_SENSE_MASK
 */
# define TV_TEST_MODE_MONITOR_DETECT	(7 << 0)
# define TV_TEST_MODE_MASK		(7 << 0)
/** @} */

/** @defgroup TV_DAC
 * @{
 */
#define TV_DAC			0x68004
/**
 * Reports that DAC state change logic has reported change (RO).
 *
 * This gets cleared when TV_DAC_STATE_EN is cleared
*/
# define TVDAC_STATE_CHG		(1 << 31)
# define TVDAC_SENSE_MASK		(7 << 28)
/** Reports that DAC A voltage is above the detect threshold */
# define TVDAC_A_SENSE			(1 << 30)
/** Reports that DAC B voltage is above the detect threshold */
# define TVDAC_B_SENSE			(1 << 29)
/** Reports that DAC C voltage is above the detect threshold */
# define TVDAC_C_SENSE			(1 << 28)
/**
 * Enables DAC state detection logic, for load-based TV detection.
 *
 * The PLL of the chosen pipe (in TV_CTL) must be running, and the encoder set
 * to off, for load detection to work.
 */
# define TVDAC_STATE_CHG_EN		(1 << 27)
/** Sets the DAC A sense value to high */
# define TVDAC_A_SENSE_CTL		(1 << 26)
/** Sets the DAC B sense value to high */
# define TVDAC_B_SENSE_CTL		(1 << 25)
/** Sets the DAC C sense value to high */
# define TVDAC_C_SENSE_CTL		(1 << 24)
/** Overrides the ENC_ENABLE and DAC voltage levels */
# define DAC_CTL_OVERRIDE		(1 << 7)
/** Sets the slew rate.  Must be preserved in software */
# define ENC_TVDAC_SLEW_FAST		(1 << 6)
# define DAC_A_1_3_V			(0 << 4)
# define DAC_A_1_1_V			(1 << 4)
# define DAC_A_0_7_V			(2 << 4)
# define DAC_A_MASK			(3 << 4)
# define DAC_B_1_3_V			(0 << 2)
# define DAC_B_1_1_V			(1 << 2)
# define DAC_B_0_7_V			(2 << 2)
# define DAC_B_MASK			(3 << 2)
# define DAC_C_1_3_V			(0 << 0)
# define DAC_C_1_1_V			(1 << 0)
# define DAC_C_0_7_V			(2 << 0)
# define DAC_C_MASK			(3 << 0)
/** @} */

/**
 * CSC coefficients are stored in a floating point format with 9 bits of
 * mantissa and 2 or 3 bits of exponent.  The exponent is represented as 2**-n,
 * where 2-bit exponents are unsigned n, and 3-bit exponents are signed n with
 * -1 (0x3) being the only legal negative value.
 */
#define TV_CSC_Y		0x68010
# define TV_RY_MASK			0x07ff0000
# define TV_RY_SHIFT			16
# define TV_GY_MASK			0x00000fff
# define TV_GY_SHIFT			0

#define TV_CSC_Y2		0x68014
# define TV_BY_MASK			0x07ff0000
# define TV_BY_SHIFT			16
/**
 * Y attenuation for component video.
 *
 * Stored in 1.9 fixed point.
 */
# define TV_AY_MASK			0x000003ff
# define TV_AY_SHIFT			0

#define TV_CSC_U		0x68018
# define TV_RU_MASK			0x07ff0000
# define TV_RU_SHIFT			16
# define TV_GU_MASK			0x000007ff
# define TV_GU_SHIFT			0

#define TV_CSC_U2		0x6801c
# define TV_BU_MASK			0x07ff0000
# define TV_BU_SHIFT			16
/**
 * U attenuation for component video.
 *
 * Stored in 1.9 fixed point.
 */
# define TV_AU_MASK			0x000003ff
# define TV_AU_SHIFT			0

#define TV_CSC_V		0x68020
# define TV_RV_MASK			0x0fff0000
# define TV_RV_SHIFT			16
# define TV_GV_MASK			0x000007ff
# define TV_GV_SHIFT			0

#define TV_CSC_V2		0x68024
# define TV_BV_MASK			0x07ff0000
# define TV_BV_SHIFT			16
/**
 * V attenuation for component video.
 *
 * Stored in 1.9 fixed point.
 */
# define TV_AV_MASK			0x000007ff
# define TV_AV_SHIFT			0

/** @defgroup TV_CSC_KNOBS
 * @{
 */
#define TV_CLR_KNOBS		0x68028
/** 2s-complement brightness adjustment */
# define TV_BRIGHTNESS_MASK		0xff000000
# define TV_BRIGHTNESS_SHIFT		24
/** Contrast adjustment, as a 2.6 unsigned floating point number */
# define TV_CONTRAST_MASK		0x00ff0000
# define TV_CONTRAST_SHIFT		16
/** Saturation adjustment, as a 2.6 unsigned floating point number */
# define TV_SATURATION_MASK		0x0000ff00
# define TV_SATURATION_SHIFT		8
/** Hue adjustment, as an integer phase angle in degrees */
# define TV_HUE_MASK			0x000000ff
# define TV_HUE_SHIFT			0
/** @} */

/** @defgroup TV_CLR_LEVEL
 * @{
 */
#define TV_CLR_LEVEL		0x6802c
/** Controls the DAC level for black */
# define TV_BLACK_LEVEL_MASK		0x01ff0000
# define TV_BLACK_LEVEL_SHIFT		16
/** Controls the DAC level for blanking */
# define TV_BLANK_LEVEL_MASK		0x000001ff
# define TV_BLANK_LEVEL_SHIFT		0
/* @} */

/** @defgroup TV_H_CTL_1
 * @{
 */
#define TV_H_CTL_1		0x68030
/** Number of pixels in the hsync. */
# define TV_HSYNC_END_MASK		0x1fff0000
# define TV_HSYNC_END_SHIFT		16
/** Total number of pixels minus one in the line (display and blanking). */
# define TV_HTOTAL_MASK			0x00001fff
# define TV_HTOTAL_SHIFT		0
/** @} */

/** @defgroup TV_H_CTL_2
 * @{
 */
#define TV_H_CTL_2		0x68034
/** Enables the colorburst (needed for non-component color) */
# define TV_BURST_ENA			(1 << 31)
/** Offset of the colorburst from the start of hsync, in pixels minus one. */
# define TV_HBURST_START_SHIFT		16
# define TV_HBURST_START_MASK		0x1fff0000
/** Length of the colorburst */
# define TV_HBURST_LEN_SHIFT		0
# define TV_HBURST_LEN_MASK		0x0001fff
/** @} */

/** @defgroup TV_H_CTL_3
 * @{
 */
#define TV_H_CTL_3		0x68038
/** End of hblank, measured in pixels minus one from start of hsync */
# define TV_HBLANK_END_SHIFT		16
# define TV_HBLANK_END_MASK		0x1fff0000
/** Start of hblank, measured in pixels minus one from start of hsync */
# define TV_HBLANK_START_SHIFT		0
# define TV_HBLANK_START_MASK		0x0001fff
/** @} */

/** @defgroup TV_V_CTL_1
 * @{
 */
#define TV_V_CTL_1		0x6803c
/** XXX */
# define TV_NBR_END_SHIFT		16
# define TV_NBR_END_MASK		0x07ff0000
/** XXX */
# define TV_VI_END_F1_SHIFT		8
# define TV_VI_END_F1_MASK		0x00003f00
/** XXX */
# define TV_VI_END_F2_SHIFT		0
# define TV_VI_END_F2_MASK		0x0000003f
/** @} */

/** @defgroup TV_V_CTL_2
 * @{
 */
#define TV_V_CTL_2		0x68040
/** Length of vsync, in half lines */
# define TV_VSYNC_LEN_MASK		0x07ff0000
# define TV_VSYNC_LEN_SHIFT		16
/** Offset of the start of vsync in field 1, measured in one less than the
 * number of half lines.
 */
# define TV_VSYNC_START_F1_MASK		0x00007f00
# define TV_VSYNC_START_F1_SHIFT	8
/**
 * Offset of the start of vsync in field 2, measured in one less than the
 * number of half lines.
 */
# define TV_VSYNC_START_F2_MASK		0x0000007f
# define TV_VSYNC_START_F2_SHIFT	0
/** @} */

/** @defgroup TV_V_CTL_3
 * @{
 */
#define TV_V_CTL_3		0x68044
/** Enables generation of the equalization signal */
# define TV_EQUAL_ENA			(1 << 31)
/** Length of vsync, in half lines */
# define TV_VEQ_LEN_MASK		0x007f0000
# define TV_VEQ_LEN_SHIFT		16
/** Offset of the start of equalization in field 1, measured in one less than
 * the number of half lines.
 */
# define TV_VEQ_START_F1_MASK		0x0007f00
# define TV_VEQ_START_F1_SHIFT		8
/**
 * Offset of the start of equalization in field 2, measured in one less than
 * the number of half lines.
 */
# define TV_VEQ_START_F2_MASK		0x000007f
# define TV_VEQ_START_F2_SHIFT		0
/** @} */

/** @defgroup TV_V_CTL_4
 * @{
 */
#define TV_V_CTL_4		0x68048
/**
 * Offset to start of vertical colorburst, measured in one less than the
 * number of lines from vertical start.
 */
# define TV_VBURST_START_F1_MASK	0x003f0000
# define TV_VBURST_START_F1_SHIFT	16
/**
 * Offset to the end of vertical colorburst, measured in one less than the
 * number of lines from the start of NBR.
 */
# define TV_VBURST_END_F1_MASK		0x000000ff
# define TV_VBURST_END_F1_SHIFT		0
/** @} */

/** @defgroup TV_V_CTL_5
 * @{
 */
#define TV_V_CTL_5		0x6804c
/**
 * Offset to start of vertical colorburst, measured in one less than the
 * number of lines from vertical start.
 */
# define TV_VBURST_START_F2_MASK	0x003f0000
# define TV_VBURST_START_F2_SHIFT	16
/**
 * Offset to the end of vertical colorburst, measured in one less than the
 * number of lines from the start of NBR.
 */
# define TV_VBURST_END_F2_MASK		0x000000ff
# define TV_VBURST_END_F2_SHIFT		0
/** @} */

/** @defgroup TV_V_CTL_6
 * @{
 */
#define TV_V_CTL_6		0x68050
/**
 * Offset to start of vertical colorburst, measured in one less than the
 * number of lines from vertical start.
 */
# define TV_VBURST_START_F3_MASK	0x003f0000
# define TV_VBURST_START_F3_SHIFT	16
/**
 * Offset to the end of vertical colorburst, measured in one less than the
 * number of lines from the start of NBR.
 */
# define TV_VBURST_END_F3_MASK		0x000000ff
# define TV_VBURST_END_F3_SHIFT		0
/** @} */

/** @defgroup TV_V_CTL_7
 * @{
 */
#define TV_V_CTL_7		0x68054
/**
 * Offset to start of vertical colorburst, measured in one less than the
 * number of lines from vertical start.
 */
# define TV_VBURST_START_F4_MASK	0x003f0000
# define TV_VBURST_START_F4_SHIFT	16
/**
 * Offset to the end of vertical colorburst, measured in one less than the
 * number of lines from the start of NBR.
 */
# define TV_VBURST_END_F4_MASK		0x000000ff
# define TV_VBURST_END_F4_SHIFT		0
/** @} */

/** @defgroup TV_SC_CTL_1
 * @{
 */
#define TV_SC_CTL_1		0x68060
/** Turns on the first subcarrier phase generation DDA */
# define TV_SC_DDA1_EN			(1 << 31)
/** Turns on the first subcarrier phase generation DDA */
# define TV_SC_DDA2_EN			(1 << 30)
/** Turns on the first subcarrier phase generation DDA */
# define TV_SC_DDA3_EN			(1 << 29)
/** Sets the subcarrier DDA to reset frequency every other field */
# define TV_SC_RESET_EVERY_2		(0 << 24)
/** Sets the subcarrier DDA to reset frequency every fourth field */
# define TV_SC_RESET_EVERY_4		(1 << 24)
/** Sets the subcarrier DDA to reset frequency every eighth field */
# define TV_SC_RESET_EVERY_8		(2 << 24)
/** Sets the subcarrier DDA to never reset the frequency */
# define TV_SC_RESET_NEVER		(3 << 24)
/** Sets the peak amplitude of the colorburst.*/
# define TV_BURST_LEVEL_MASK		0x00ff0000
# define TV_BURST_LEVEL_SHIFT		16
/** Sets the increment of the first subcarrier phase generation DDA */
# define TV_SCDDA1_INC_MASK		0x00000fff
# define TV_SCDDA1_INC_SHIFT		0
/** @} */

/** @defgroup TV_SC_CTL_2
 * @{
 */
#define TV_SC_CTL_2		0x68064
/** Sets the rollover for the second subcarrier phase generation DDA */
# define TV_SCDDA2_SIZE_MASK		0x7fff0000
# define TV_SCDDA2_SIZE_SHIFT		16
/** Sets the increent of the second subcarrier phase generation DDA */
# define TV_SCDDA2_INC_MASK		0x00007fff
# define TV_SCDDA2_INC_SHIFT		0
/** @} */

/** @defgroup TV_SC_CTL_3
 * @{
 */
#define TV_SC_CTL_3		0x68068
/** Sets the rollover for the third subcarrier phase generation DDA */
# define TV_SCDDA3_SIZE_MASK		0x7fff0000
# define TV_SCDDA3_SIZE_SHIFT		16
/** Sets the increent of the third subcarrier phase generation DDA */
# define TV_SCDDA3_INC_MASK		0x00007fff
# define TV_SCDDA3_INC_SHIFT		0
/** @} */

/** @defgroup TV_WIN_POS
 * @{
 */
#define TV_WIN_POS		0x68070
/** X coordinate of the display from the start of horizontal active */
# define TV_XPOS_MASK			0x1fff0000
# define TV_XPOS_SHIFT			16
/** Y coordinate of the display from the start of vertical active (NBR) */
# define TV_YPOS_MASK			0x00000fff
# define TV_YPOS_SHIFT			0
/** @} */

/** @defgroup TV_WIN_SIZE
 * @{
 */
#define TV_WIN_SIZE		0x68074
/** Horizontal size of the display window, measured in pixels*/
# define TV_XSIZE_MASK			0x1fff0000
# define TV_XSIZE_SHIFT			16
/**
 * Vertical size of the display window, measured in pixels.
 *
 * Must be even for interlaced modes.
 */
# define TV_YSIZE_MASK			0x00000fff
# define TV_YSIZE_SHIFT			0
/** @} */

/** @defgroup TV_FILTER_CTL_1
 * @{
 */
#define TV_FILTER_CTL_1		0x68080
/**
 * Enables automatic scaling calculation.
 *
 * If set, the rest of the registers are ignored, and the calculated values can
 * be read back from the register.
 */
# define TV_AUTO_SCALE			(1 << 31)
/**
 * Disables the vertical filter.
 *
 * This is required on modes more than 1024 pixels wide */
# define TV_V_FILTER_BYPASS		(1 << 29)
/** Enables adaptive vertical filtering */
# define TV_VADAPT			(1 << 28)
# define TV_VADAPT_MODE_MASK		(3 << 26)
/** Selects the least adaptive vertical filtering mode */
# define TV_VADAPT_MODE_LEAST		(0 << 26)
/** Selects the moderately adaptive vertical filtering mode */
# define TV_VADAPT_MODE_MODERATE	(1 << 26)
/** Selects the most adaptive vertical filtering mode */
# define TV_VADAPT_MODE_MOST		(3 << 26)
/**
 * Sets the horizontal scaling factor.
 *
 * This should be the fractional part of the horizontal scaling factor divided
 * by the oversampling rate.  TV_HSCALE should be less than 1, and set to:
 *
 * (src width - 1) / ((oversample * dest width) - 1)
 */
# define TV_HSCALE_FRAC_MASK		0x00003fff
# define TV_HSCALE_FRAC_SHIFT		0
/** @} */

/** @defgroup TV_FILTER_CTL_2
 * @{
 */
#define TV_FILTER_CTL_2		0x68084
/**
 * Sets the integer part of the 3.15 fixed-point vertical scaling factor.
 *
 * TV_VSCALE should be (src height - 1) / ((interlace * dest height) - 1)
 */
# define TV_VSCALE_INT_MASK		0x00038000
# define TV_VSCALE_INT_SHIFT		15
/**
 * Sets the fractional part of the 3.15 fixed-point vertical scaling factor.
 *
 * \sa TV_VSCALE_INT_MASK
 */
# define TV_VSCALE_FRAC_MASK		0x00007fff
# define TV_VSCALE_FRAC_SHIFT		0
/** @} */

/** @defgroup TV_FILTER_CTL_3
 * @{
 */
#define TV_FILTER_CTL_3		0x68088
/**
 * Sets the integer part of the 3.15 fixed-point vertical scaling factor.
 *
 * TV_VSCALE should be (src height - 1) / (1/4 * (dest height - 1))
 *
 * For progressive modes, TV_VSCALE_IP_INT should be set to zeroes.
 */
# define TV_VSCALE_IP_INT_MASK		0x00038000
# define TV_VSCALE_IP_INT_SHIFT		15
/**
 * Sets the fractional part of the 3.15 fixed-point vertical scaling factor.
 *
 * For progressive modes, TV_VSCALE_IP_INT should be set to zeroes.
 *
 * \sa TV_VSCALE_IP_INT_MASK
 */
# define TV_VSCALE_IP_FRAC_MASK		0x00007fff
# define TV_VSCALE_IP_FRAC_SHIFT		0
/** @} */

/** @defgroup TV_CC_CONTROL
 * @{
 */
#define TV_CC_CONTROL		0x68090
# define TV_CC_ENABLE			(1 << 31)
/**
 * Specifies which field to send the CC data in.
 *
 * CC data is usually sent in field 0.
 */
# define TV_CC_FID_MASK			(1 << 27)
# define TV_CC_FID_SHIFT		27
/** Sets the horizontal position of the CC data.  Usually 135. */
# define TV_CC_HOFF_MASK		0x03ff0000
# define TV_CC_HOFF_SHIFT		16
/** Sets the vertical position of the CC data.  Usually 21 */
# define TV_CC_LINE_MASK		0x0000003f
# define TV_CC_LINE_SHIFT		0
/** @} */

/** @defgroup TV_CC_DATA
 * @{
 */
#define TV_CC_DATA		0x68094
# define TV_CC_RDY			(1 << 31)
/** Second word of CC data to be transmitted. */
# define TV_CC_DATA_2_MASK		0x007f0000
# define TV_CC_DATA_2_SHIFT		16
/** First word of CC data to be transmitted. */
# define TV_CC_DATA_1_MASK		0x0000007f
# define TV_CC_DATA_1_SHIFT		0
/** @}
 */

/** @{ */
#define TV_H_LUMA_0		0x68100
#define TV_H_LUMA_59		0x681ec
#define TV_H_CHROMA_0		0x68200
#define TV_H_CHROMA_59		0x682ec
#define TV_V_LUMA_0		0x68300
#define TV_V_LUMA_42		0x683a8
#define TV_V_CHROMA_0		0x68400
#define TV_V_CHROMA_42		0x684a8
/** @} */

#define PIPEA_DSL		0x70000

#define PIPEACONF 0x70008
#define PIPEACONF_ENABLE	(1<<31)
#define PIPEACONF_DISABLE	0
#define PIPEACONF_DOUBLE_WIDE	(1<<30)
#define I965_PIPECONF_ACTIVE	(1<<30)
#define PIPEACONF_SINGLE_WIDE	0
#define PIPEACONF_PIPE_UNLOCKED 0
#define PIPEACONF_PIPE_LOCKED	(1<<25)
#define PIPEACONF_PALETTE	0
#define PIPEACONF_GAMMA 	(1<<24)
/* Ironlake: gamma */
#define PIPECONF_PALETTE_8BIT	(0<<24)
#define PIPECONF_PALETTE_10BIT	(1<<24)
#define PIPECONF_PALETTE_12BIT	(2<<24)
#define PIPECONF_FORCE_BORDER	(1<<25)
#define PIPECONF_PROGRESSIVE	(0 << 21)
#define PIPECONF_INTERLACE_W_FIELD_INDICATION	(6 << 21)
#define PIPECONF_INTERLACE_FIELD_0_ONLY		(7 << 21)
/* Ironlake */
#define PIPECONF_MSA_TIMING_DELAY	(0<<18) /* for eDP */
#define PIPECONF_NO_DYNAMIC_RATE_CHANGE	(0 << 16)
#define PIPECONF_NO_ROTATION		(0<<14)
#define PIPECONF_FULL_COLOR_RANGE	(0<<13)
#define PIPECONF_CE_COLOR_RANGE		(1<<13)
#define PIPECONF_COLOR_SPACE_RGB	(0<<11)
#define PIPECONF_COLOR_SPACE_YUV601	(1<<11)
#define PIPECONF_COLOR_SPACE_YUV709	(2<<11)
#define PIPECONF_CONNECT_DEFAULT	(0<<9)
#define PIPECONF_8BPP			(0<<5)
#define PIPECONF_10BPP			(1<<5)
#define PIPECONF_6BPP			(2<<5)
#define PIPECONF_12BPP			(3<<5)
#define PIPECONF_ENABLE_DITHER		(1<<4)
#define PIPECONF_DITHER_SPATIAL		(0<<2)
#define PIPECONF_DITHER_ST1		(1<<2)
#define PIPECONF_DITHER_ST2		(2<<2)
#define PIPECONF_DITHER_TEMPORAL	(3<<2)

#define PIPEAGCMAXRED		0x70010
#define PIPEAGCMAXGREEN		0x70014
#define PIPEAGCMAXBLUE		0x70018
#define PIPEASTAT		0x70024
# define FIFO_UNDERRUN		(1 << 31)
# define CRC_ERROR_ENABLE	(1 << 29)
# define CRC_DONE_ENABLE	(1 << 28)
# define GMBUS_EVENT_ENABLE	(1 << 27)
# define VSYNC_INT_ENABLE	(1 << 25)
# define DLINE_COMPARE_ENABLE	(1 << 24)
# define DPST_EVENT_ENABLE	(1 << 23)
# define LBLC_EVENT_ENABLE	(1 << 22)
# define OFIELD_INT_ENABLE	(1 << 21)
# define EFIELD_INT_ENABLE	(1 << 20)
# define SVBLANK_INT_ENABLE	(1 << 18)
# define VBLANK_INT_ENABLE	(1 << 17)
# define OREG_UPDATE_ENABLE	(1 << 16)
# define CRC_ERROR_INT_STATUS	(1 << 13)
# define CRC_DONE_INT_STATUS	(1 << 12)
# define GMBUS_INT_STATUS	(1 << 11)
# define VSYNC_INT_STATUS	(1 << 9)
# define DLINE_COMPARE_STATUS	(1 << 8)
# define DPST_EVENT_STATUS	(1 << 7)
# define LBLC_EVENT_STATUS	(1 << 6)
# define OFIELD_INT_STATUS	(1 << 5)
# define EFIELD_INT_STATUS	(1 << 4)
# define SVBLANK_INT_STATUS	(1 << 2)
# define VBLANK_INT_STATUS	(1 << 1)
# define OREG_UPDATE_STATUS	(1 << 0)
				 

#define DSPARB			0x70030
#define   DSPARB_CSTART_SHIFT	7
#define   DSPARB_BSTART_SHIFT	0
#define   DSPARB_BEND_SHIFT	9 /* on 855 */
#define   DSPARB_AEND_SHIFT	0
#define DSPFW1			0x70034
#define DSPFW2			0x70038
#define DSPFW3			0x7003c
/*
 * The two pipe frame counter registers are not synchronized, so
 * reading a stable value is somewhat tricky. The following code 
 * should work:
 *
 *  do {
 *    high1 = ((INREG(PIPEAFRAMEHIGH) & PIPE_FRAME_HIGH_MASK) >> PIPE_FRAME_HIGH_SHIFT;
 *    low1 =  ((INREG(PIPEAFRAMEPIXEL) & PIPE_FRAME_LOW_MASK) >> PIPE_FRAME_LOW_SHIFT);
 *    high2 = ((INREG(PIPEAFRAMEHIGH) & PIPE_FRAME_HIGH_MASK) >> PIPE_FRAME_HIGH_SHIFT);
 *  } while (high1 != high2);
 *  frame = (high1 << 8) | low1;
 */
#define PIPEAFRAMEHIGH		0x70040
#define PIPE_FRAME_HIGH_MASK	0x0000ffff
#define PIPE_FRAME_HIGH_SHIFT	0
#define PIPEAFRAMEPIXEL		0x70044
#define PIPE_FRAME_LOW_MASK	0xff000000
#define PIPE_FRAME_LOW_SHIFT	24
/*
 * Pixel within the current frame is counted in the PIPEAFRAMEPIXEL register
 * and is 24 bits wide.
 */
#define PIPE_PIXEL_MASK		0x00ffffff
#define PIPE_PIXEL_SHIFT	0

/*
 * Computing GMCH M and N values.
 *
 * GMCH M/N = dot clock * bytes per pixel / ls_clk * # of lanes
 *
 * ls_clk (we assume) is the DP link clock (1.62 or 2.7 GHz)
 *
 * The GMCH value is used internally
 */
#define PIPEA_GMCH_DATA_M	0x70050

/* Transfer unit size for display port - 1, default is 0x3f (for TU size 64) */
#define PIPE_GMCH_DATA_M_TU_SIZE_MASK	(0x3f << 25)
#define PIPE_GMCH_DATA_M_TU_SIZE_SHIFT	25

#define PIPE_GMCH_DATA_M_MASK		(0xffffff)

#define PIPEA_GMCH_DATA_N	0x70054
#define PIPE_GMCH_DATA_N_MASK		(0xffffff)

/*
 * Computing Link M and N values.
 *
 * Link M / N = pixel_clock / ls_clk
 *
 * (the DP spec calls pixel_clock the 'strm_clk')
 *
 * The Link value is transmitted in the Main Stream
 * Attributes and VB-ID.
 */

#define PIPEA_DP_LINK_M		0x70060
#define PIPEA_DP_LINK_M_MASK	(0xffffff)

#define PIPEA_DP_LINK_N		0x70064
#define PIPEA_DP_LINK_N_MASK	(0xffffff)

#define PIPEB_DSL		0x71000

#define PIPEBCONF 0x71008
#define PIPEBCONF_ENABLE	(1<<31)
#define PIPEBCONF_DISABLE	0
#define PIPEBCONF_DOUBLE_WIDE	(1<<30)
#define PIPEBCONF_DISABLE	0
#define PIPEBCONF_GAMMA 	(1<<24)
#define PIPEBCONF_PALETTE	0

#define PIPEBGCMAXRED		0x71010
#define PIPEBGCMAXGREEN		0x71014
#define PIPEBGCMAXBLUE		0x71018
#define PIPEBSTAT		0x71024
#define PIPEBFRAMEHIGH		0x71040
#define PIPEBFRAMEPIXEL		0x71044

#define PIPEB_GMCH_DATA_M	0x71050
#define PIPEB_GMCH_DATA_N	0x71054
#define PIPEB_DP_LINK_M		0x71060
#define PIPEB_DP_LINK_N		0x71064

#define DSPACNTR		0x70180
#define DSPBCNTR		0x71180
#define DISPLAY_PLANE_ENABLE 			(1<<31)
#define DISPLAY_PLANE_DISABLE			0
#define DISPLAY_PLANE_TILED			(1<<10)
#define DISPPLANE_GAMMA_ENABLE			(1<<30)
#define DISPPLANE_GAMMA_DISABLE			0
#define DISPPLANE_PIXFORMAT_MASK		(0xf<<26)
#define DISPPLANE_8BPP				(0x2<<26)
#define DISPPLANE_15_16BPP			(0x4<<26)
#define DISPPLANE_16BPP				(0x5<<26)
#define DISPPLANE_32BPP_NO_ALPHA 		(0x6<<26) /* Ironlake: BGRX */
#define DISPPLANE_32BPP				(0x7<<26) /* Ironlake: not support */
/* Ironlake */
#define DISPPLANE_32BPP_10			(0x8<<26) /* 2:10:10:10 */
#define DISPPLANE_32BPP_BGRX			(0xa<<26)
#define DISPPLANE_64BPP				(0xc<<26)
#define DISPPLANE_32BPP_RGBX			(0xe<<26)
#define DISPPLANE_STEREO_ENABLE			(1<<25)
#define DISPPLANE_STEREO_DISABLE		0
#define DISPPLANE_SEL_PIPE_MASK			(1<<24)
#define DISPPLANE_SEL_PIPE_A			0	/* Ironlake: don't use */
#define DISPPLANE_SEL_PIPE_B			(1<<24)
#define DISPPLANE_NORMAL_RANGE			(0<<25)
#define DISPPLANE_EXT_RANGE			(1<<25)
/* Ironlake */
#define DISPPLANE_CSC_BYPASS			(0<<24)
#define DISPPLANE_CSC_PASSTHROUGH		(1<<24)
#define DISPPLANE_SRC_KEY_ENABLE		(1<<22)
#define DISPPLANE_SRC_KEY_DISABLE		0
#define DISPPLANE_LINE_DOUBLE			(1<<20)
#define DISPPLANE_NO_LINE_DOUBLE		0
#define DISPPLANE_STEREO_POLARITY_FIRST		0
#define DISPPLANE_STEREO_POLARITY_SECOND	(1<<18)
/* plane B only */
#define DISPPLANE_ALPHA_TRANS_ENABLE		(1<<15)
#define DISPPLANE_ALPHA_TRANS_DISABLE		0
#define DISPPLANE_SPRITE_ABOVE_DISPLAYA		0
#define DISPPLANE_SPRITE_ABOVE_OVERLAY		(1)
/* Ironlake */
#define DISPPLANE_X_TILE			(1<<10)
#define DISPPLANE_LINEAR			(0<<10)

#define DSPABASE		0x70184
/* Ironlake */
#define DSPALINOFF		0x70184
#define DSPASTRIDE		0x70188

#define DSPBBASE		0x71184
/* Ironlake */
#define DSPBLINOFF		0x71184
#define DSPBADDR		DSPBBASE
#define DSPBSTRIDE		0x71188

#define DSPAKEYVAL		0x70194
#define DSPAKEYMASK		0x70198

#define DSPAPOS			0x7018C /* reserved */
#define DSPASIZE		0x70190
#define DSPBPOS			0x7118C
#define DSPBSIZE		0x71190

#define DSPASURF		0x7019C
#define DSPATILEOFF		0x701A4

#define DSPBSURF		0x7119C
#define DSPBTILEOFF		0x711A4

#define VGACNTRL		0x71400
# define VGA_DISP_DISABLE			(1 << 31)
# define VGA_2X_MODE				(1 << 30)
# define VGA_PIPE_B_SELECT			(1 << 29)

/* Various masks for reserved bits, etc. */
#define I830_FWATER1_MASK        (~((1<<11)|(1<<10)|(1<<9)|      \
        (1<<8)|(1<<26)|(1<<25)|(1<<24)|(1<<5)|(1<<4)|(1<<3)|    \
        (1<<2)|(1<<1)|1|(1<<20)|(1<<19)|(1<<18)|(1<<17)|(1<<16)))
#define I830_FWATER2_MASK ~(0)

#define DV0A_RESERVED ((1<<26)|(1<<25)|(1<<24)|(1<<23)|(1<<22)|(1<<21)|(1<<20)|(1<<19)|(1<<18)|(1<<16)|(1<<5)|(1<<1)|1)
#define DV0B_RESERVED ((1<<27)|(1<<26)|(1<<25)|(1<<24)|(1<<23)|(1<<22)|(1<<21)|(1<<20)|(1<<19)|(1<<18)|(1<<16)|(1<<5)|(1<<1)|1)
#define VGA0_N_DIVISOR_MASK     ((1<<21)|(1<<20)|(1<<19)|(1<<18)|(1<<17)|(1<<16))
#define VGA0_M1_DIVISOR_MASK    ((1<<13)|(1<<12)|(1<<11)|(1<<10)|(1<<9)|(1<<8))
#define VGA0_M2_DIVISOR_MASK    ((1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|1)
#define VGA0_M1M2N_RESERVED	~(VGA0_N_DIVISOR_MASK|VGA0_M1_DIVISOR_MASK|VGA0_M2_DIVISOR_MASK)
#define VGA0_POSTDIV_MASK       ((1<<7)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|1)
#define VGA1_POSTDIV_MASK       ((1<<15)|(1<<13)|(1<<12)|(1<<11)|(1<<10)|(1<<9)|(1<<8))
#define VGA_POSTDIV_RESERVED	~(VGA0_POSTDIV_MASK|VGA1_POSTDIV_MASK|(1<<7)|(1<<15))
#define DPLLA_POSTDIV_MASK ((1<<23)|(1<<21)|(1<<20)|(1<<19)|(1<<18)|(1<<17)|(1<<16))
#define DPLLA_RESERVED     ((1<<27)|(1<<26)|(1<<25)|(1<<24)|(1<<22)|(1<<15)|(1<<12)|(1<<11)|(1<<10)|(1<<9)|(1<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|1)
#define ADPA_RESERVED	((1<<2)|(1<<1)|1|(1<<9)|(1<<8)|(1<<7)|(1<<6)|(1<<5)|(1<<30)|(1<<29)|(1<<28)|(1<<27)|(1<<26)|(1<<25)|(1<<24)|(1<<23)|(1<<22)|(1<<21)|(1<<20)|(1<<19)|(1<<18)|(1<<17)|(1<<16))
#define SUPER_WORD              32
#define BURST_A_MASK    ((1<<11)|(1<<10)|(1<<9)|(1<<8))
#define BURST_B_MASK    ((1<<26)|(1<<25)|(1<<24))
#define WATER_A_MASK    ((1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|1)
#define WATER_B_MASK    ((1<<20)|(1<<19)|(1<<18)|(1<<17)|(1<<16))
#define WATER_RESERVED	((1<<31)|(1<<30)|(1<<29)|(1<<28)|(1<<27)|(1<<23)|(1<<22)|(1<<21)|(1<<15)|(1<<14)|(1<<13)|(1<<12)|(1<<7)|(1<<6))
#define PIPEACONF_RESERVED ((1<<29)|(1<<28)|(1<<27)|(1<<23)|(1<<22)|(1<<21)|(1<<20)|(1<<19)|(1<<18)|(1<<17)|(1<<16)|0xffff)
#define PIPEBCONF_RESERVED ((1<<30)|(1<<29)|(1<<28)|(1<<27)|(1<<26)|(1<<25)|(1<<23)|(1<<22)|(1<<21)|(1<<20)|(1<<19)|(1<<18)|(1<<17)|(1<<16)|0xffff)
#define DSPACNTR_RESERVED ((1<<23)|(1<<19)|(1<<17)|(1<<16)|0xffff)
#define DSPBCNTR_RESERVED ((1<<23)|(1<<19)|(1<<17)|(1<<16)|0x7ffe)

#define I830_GMCH_CTRL		0x52

#define I830_GMCH_ENABLED	0x4
#define I830_GMCH_MEM_MASK	0x1
#define I830_GMCH_MEM_64M	0x1
#define I830_GMCH_MEM_128M	0

#define I830_GMCH_GMS_MASK			0x70
#define I830_GMCH_GMS_DISABLED			0x00
#define I830_GMCH_GMS_LOCAL			0x10
#define I830_GMCH_GMS_STOLEN_512		0x20
#define I830_GMCH_GMS_STOLEN_1024		0x30
#define I830_GMCH_GMS_STOLEN_8192		0x40

#define I830_RDRAM_CHANNEL_TYPE		0x03010
#define I830_RDRAM_ND(x)			(((x) & 0x20) >> 5)
#define I830_RDRAM_DDT(x)			(((x) & 0x18) >> 3)

#define I855_GMCH_GMS_MASK			(0xF << 4)
#define I855_GMCH_GMS_DISABLED			0x00
#define I855_GMCH_GMS_STOLEN_1M			(0x1 << 4)
#define I855_GMCH_GMS_STOLEN_4M			(0x2 << 4)
#define I855_GMCH_GMS_STOLEN_8M			(0x3 << 4)
#define I855_GMCH_GMS_STOLEN_16M		(0x4 << 4)
#define I855_GMCH_GMS_STOLEN_32M		(0x5 << 4)
#define I915G_GMCH_GMS_STOLEN_48M		(0x6 << 4)
#define I915G_GMCH_GMS_STOLEN_64M		(0x7 << 4)
#define G33_GMCH_GMS_STOLEN_128M		(0x8 << 4)
#define G33_GMCH_GMS_STOLEN_256M		(0x9 << 4)
#define INTEL_GMCH_GMS_STOLEN_96M		(0xa << 4)
#define INTEL_GMCH_GMS_STOLEN_160M		(0xb << 4)
#define INTEL_GMCH_GMS_STOLEN_224M		(0xc << 4)
#define INTEL_GMCH_GMS_STOLEN_352M		(0xd << 4)


#define I915_GCFGC			0xf0
#define I915_LOW_FREQUENCY_ENABLE		(1 << 7)
#define I915_DISPLAY_CLOCK_190_200_MHZ		(0 << 4)
#define I915_DISPLAY_CLOCK_333_MHZ		(4 << 4)
#define I915_DISPLAY_CLOCK_MASK			(7 << 4)

#define I855_HPLLCC			0xc0
#define I855_CLOCK_CONTROL_MASK			(3 << 0)
#define I855_CLOCK_133_200			(0 << 0)
#define I855_CLOCK_100_200			(1 << 0)
#define I855_CLOCK_100_133			(2 << 0)
#define I855_CLOCK_166_250			(3 << 0)

/* BLT commands */
#define COLOR_BLT_CMD		((2<<29)|(0x40<<22)|(0x3))
#define COLOR_BLT_WRITE_ALPHA	(1<<21)
#define COLOR_BLT_WRITE_RGB	(1<<20)

#define XY_COLOR_BLT_CMD		((2<<29)|(0x50<<22)|(0x4))
#define XY_COLOR_BLT_WRITE_ALPHA	(1<<21)
#define XY_COLOR_BLT_WRITE_RGB		(1<<20)
#define XY_COLOR_BLT_TILED		(1<<11)

#define XY_SETUP_CLIP_BLT_CMD		((2<<29)|(3<<22)|1)

#define XY_SRC_COPY_BLT_CMD		((2<<29)|(0x53<<22)|6)
#define XY_SRC_COPY_BLT_WRITE_ALPHA	(1<<21)
#define XY_SRC_COPY_BLT_WRITE_RGB	(1<<20)
#define XY_SRC_COPY_BLT_SRC_TILED	(1<<15)
#define XY_SRC_COPY_BLT_DST_TILED	(1<<11)

#define SRC_COPY_BLT_CMD		((2<<29)|(0x43<<22)|0x4)
#define SRC_COPY_BLT_WRITE_ALPHA	(1<<21)
#define SRC_COPY_BLT_WRITE_RGB		(1<<20)

#define XY_PAT_BLT_IMMEDIATE		((2<<29)|(0x72<<22))

#define XY_MONO_PAT_BLT_CMD		((0x2<<29)|(0x52<<22)|0x7)
#define XY_MONO_PAT_VERT_SEED		((1<<10)|(1<<9)|(1<<8))
#define XY_MONO_PAT_HORT_SEED		((1<<14)|(1<<13)|(1<<12))
#define XY_MONO_PAT_BLT_WRITE_ALPHA	(1<<21)
#define XY_MONO_PAT_BLT_WRITE_RGB	(1<<20)

#define XY_MONO_SRC_BLT_CMD		((0x2<<29)|(0x54<<22)|(0x6))
#define XY_MONO_SRC_BLT_WRITE_ALPHA	(1<<21)
#define XY_MONO_SRC_BLT_WRITE_RGB	(1<<20)

#define STATE3D_FOG_MODE		((3<<29)|(0x1d<<24)|(0x89<<16)|2)
#define FOG_MODE_VERTEX 		(1<<31)

#define DISABLE_TEX_TRANSFORM		(1<<28)
#define TEXTURE_SET(x)			(x<<29)

#define STATE3D_VERTEX_TRANSFORM	((3<<29)|(0x1d<<24)|(0x8b<<16))
#define DISABLE_VIEWPORT_TRANSFORM	(1<<31)
#define DISABLE_PERSPECTIVE_DIVIDE	(1<<29)

#define MI_SET_CONTEXT			(0x18<<23)
#define CTXT_NO_RESTORE 		(1)
#define CTXT_PALETTE_SAVE_DISABLE	(1<<3)
#define CTXT_PALETTE_RESTORE_DISABLE	(1<<2)

/* Dword 0 */
#define MI_VERTEX_BUFFER		(0x17<<23)
#define MI_VERTEX_BUFFER_IDX(x) 	(x<<20)
#define MI_VERTEX_BUFFER_PITCH(x)	(x<<13)
#define MI_VERTEX_BUFFER_WIDTH(x)	(x<<6)
/* Dword 1 */
#define MI_VERTEX_BUFFER_DISABLE	(1)

/* Overlay Flip */
#define MI_OVERLAY_FLIP			(0x11<<23)
#define MI_OVERLAY_FLIP_CONTINUE	(0<<21)
#define MI_OVERLAY_FLIP_ON		(1<<21)
#define MI_OVERLAY_FLIP_OFF		(2<<21)

/* Wait for Events */
#define MI_WAIT_FOR_EVENT			(0x03<<23)
#define MI_WAIT_FOR_PIPEB_SVBLANK		(1<<18)
#define MI_WAIT_FOR_PIPEA_SVBLANK		(1<<17)
#define MI_WAIT_FOR_OVERLAY_FLIP		(1<<16)
#define MI_WAIT_FOR_PIPEB_VBLANK		(1<<7)
#define MI_WAIT_FOR_PIPEB_SCAN_LINE_WINDOW	(1<<5)
#define MI_WAIT_FOR_PIPEA_VBLANK		(1<<3)
#define MI_WAIT_FOR_PIPEA_SCAN_LINE_WINDOW	(1<<1)

/* Set the scan line for MI_WAIT_FOR_PIPE?_SCAN_LINE_WINDOW */
#define MI_LOAD_SCAN_LINES_INCL			(0x12<<23)
#define MI_LOAD_SCAN_LINES_DISPLAY_PIPEA	(0)
#define MI_LOAD_SCAN_LINES_DISPLAY_PIPEB	(0x1<<20)

/* Flush */
#define MI_FLUSH			(0x04<<23)
#define MI_WRITE_DIRTY_STATE		(1<<4)
#define MI_END_SCENE			(1<<3)
#define MI_GLOBAL_SNAPSHOT_COUNT_RESET	(1<<3)
#define MI_INHIBIT_RENDER_CACHE_FLUSH	(1<<2)
#define MI_STATE_INSTRUCTION_CACHE_FLUSH (1<<1)
#define MI_INVALIDATE_MAP_CACHE		(1<<0)
/* broadwater flush bits */
#define BRW_MI_GLOBAL_SNAPSHOT_RESET   (1 << 3)

/* Noop */
#define MI_NOOP				0x00
#define MI_NOOP_WRITE_ID		(1<<22)
#define MI_NOOP_ID_MASK			(1<<22 - 1)

#define STATE3D_COLOR_FACTOR	((0x3<<29)|(0x1d<<24)|(0x01<<16))

/* Batch */
#define MI_BATCH_BUFFER		((0x30 << 23) | 1)
#define MI_BATCH_BUFFER_START	(0x31 << 23)
#define MI_BATCH_BUFFER_END	(0xA << 23)
#define MI_BATCH_NON_SECURE		(1)
#define MI_BATCH_NON_SECURE_I965	(1 << 8)

/* STATE3D_FOG_MODE stuff */
#define ENABLE_FOG_SOURCE	(1<<27)
#define ENABLE_FOG_CONST	(1<<24)
#define ENABLE_FOG_DENSITY	(1<<23)

#define MAX_DISPLAY_PIPES	2

typedef enum {
   CrtIndex = 0,
   TvIndex,
   DfpIndex,
   LfpIndex,
   Crt2Index,
   Tv2Index,
   Dfp2Index,
   Lfp2Index,
   NumDisplayTypes
} DisplayType;

/* What's connected to the pipes (as reported by the BIOS) */
#define PIPE_ACTIVE_MASK		0xff
#define PIPE_CRT_ACTIVE			(1 << CrtIndex)
#define PIPE_TV_ACTIVE			(1 << TvIndex)
#define PIPE_DFP_ACTIVE			(1 << DfpIndex)
#define PIPE_LCD_ACTIVE			(1 << LfpIndex)
#define PIPE_CRT2_ACTIVE		(1 << Crt2Index)
#define PIPE_TV2_ACTIVE			(1 << Tv2Index)
#define PIPE_DFP2_ACTIVE		(1 << Dfp2Index)
#define PIPE_LCD2_ACTIVE		(1 << Lfp2Index)

#define PIPE_SIZED_DISP_MASK		(PIPE_DFP_ACTIVE |	\
					 PIPE_LCD_ACTIVE |	\
					 PIPE_DFP2_ACTIVE)

#define PIPE_A_SHIFT			0
#define PIPE_B_SHIFT			8
#define PIPE_SHIFT(n)			((n) == 0 ? \
					 PIPE_A_SHIFT : PIPE_B_SHIFT)

/*
 * Some BIOS scratch area registers.  The 845 (and 830?) store the amount
 * of video memory available to the BIOS in SWF1.
 */

#define SWF0			0x71410
#define SWF1			0x71414
#define SWF2			0x71418
#define SWF3			0x7141c
#define SWF4			0x71420
#define SWF5			0x71424
#define SWF6			0x71428

/*
 * 855 scratch registers.
 */
#define SWF00			0x70410
#define SWF01			0x70414
#define SWF02			0x70418
#define SWF03			0x7041c
#define SWF04			0x70420
#define SWF05			0x70424
#define SWF06			0x70428

#define SWF10			SWF0
#define SWF11			SWF1
#define SWF12			SWF2
#define SWF13			SWF3
#define SWF14			SWF4
#define SWF15			SWF5
#define SWF16			SWF6

#define SWF30			0x72414
#define SWF31			0x72418
#define SWF32			0x7241c

/*
 * Overlay registers.  These are overlay registers accessed via MMIO.
 * Those loaded via the overlay register page are defined in i830_video.c.
 */
#define OVADD			0x30000

#define DOVSTA			0x30008
#define OC_BUF			(0x3<<20)

#define OGAMC5			0x30010
#define OGAMC4			0x30014
#define OGAMC3			0x30018
#define OGAMC2			0x3001c
#define OGAMC1			0x30020
#define OGAMC0			0x30024


/*
 * Palette registers
 */
#define PALETTE_A		0x0a000
#define PALETTE_B		0x0a800

/* Framebuffer compression */
#define FBC_CFB_BASE		0x03200 /* 4k page aligned */
#define FBC_LL_BASE		0x03204 /* 4k page aligned */
#define FBC_CONTROL		0x03208
#define   FBC_CTL_EN		(1<<31)
#define   FBC_CTL_PERIODIC	(1<<30)
#define   FBC_CTL_INTERVAL_SHIFT (16)
#define   FBC_CTL_UNCOMPRESSIBLE (1<<14)
#define   FBC_CTL_STRIDE_SHIFT	(5)
#define   FBC_CTL_FENCENO	(1<<0)
#define FBC_COMMAND		0x0320c
#define   FBC_CMD_COMPRESS	(1<<0)
#define FBC_STATUS		0x03210
#define   FBC_STAT_COMPRESSING	(1<<31)
#define   FBC_STAT_COMPRESSED	(1<<30)
#define   FBC_STAT_MODIFIED	(1<<29)
#define   FBC_STAT_CURRENT_LINE	(1<<0)
#define FBC_CONTROL2		0x03214
#define   FBC_CTL_FENCE_DBL	(0<<4)
#define   FBC_CTL_IDLE_IMM	(0<<2)
#define   FBC_CTL_IDLE_FULL	(1<<2)
#define   FBC_CTL_IDLE_LINE	(2<<2)
#define   FBC_CTL_IDLE_DEBUG	(3<<2)
#define   FBC_CTL_CPU_FENCE	(1<<1)
#define   FBC_CTL_PLANEA	(0<<0)
#define   FBC_CTL_PLANEB	(1<<0)
#define FBC_FENCE_OFF		0x0321b
#define FBC_MOD_NUM		0x03220
#define FBC_TAG_DEBUG		0x03300

#define FBC_LL_SIZE		(1536)
#define FBC_LL_PAD		(32)

/* Framebuffer compression version 2 */
#define DPFC_CB_BASE		0x3200
#define DPFC_CONTROL		0x3208
#define   DPFC_CTL_EN		(1<<31)
#define   DPFC_CTL_PLANEA	(0<<30)
#define   DPFC_CTL_PLANEB	(1<<30)
#define   DPFC_CTL_FENCE_EN	(1<<29)
#define   DPFC_CTL_LIMIT_1X	(0<<6)
#define   DPFC_CTL_LIMIT_2X	(1<<6)
#define   DPFC_CTL_LIMIT_4X	(2<<6)
#define DPFC_RECOMP_CTL		0x320c
#define   DPFC_RECOMP_STALL_EN	(1<<27)
#define   DPFC_RECOMP_STALL_WM_SHIFT (16)
#define   DPFC_RECOMP_STALL_WM_MASK (0x07ff0000)
#define   DPFC_RECOMP_TIMER_COUNT_SHIFT (0)
#define   DPFC_RECOMP_TIMER_COUNT_MASK (0x0000003f)
#define DPFC_STATUS		0x3210
#define   DPFC_INVAL_SEG_SHIFT  (16)
#define   DPFC_INVAL_SEG_MASK	(0x07ff0000)
#define   DPFC_COMP_SEG_SHIFT	(0)
#define   DPFC_COMP_SEG_MASK	(0x000003ff)
#define DPFC_STATUS2		0x3214
#define DPFC_FENCE_YOFF		0x3218

#define PEG_BAND_GAP_DATA	0x14d68

#define MCHBAR_RENDER_STANDBY	0x111B8
#define RENDER_STANDBY_ENABLE	(1 << 30)

/* Ironlake */

/* warmup time in us */
#define WARMUP_PCH_REF_CLK_SSC_MOD	1
#define WARMUP_PCH_FDI_RECEIVER_PLL	25
#define WARMUP_PCH_DPLL			50
#define WARMUP_CPU_DP_PLL		20
#define WARMUP_CPU_FDI_TRANSMITTER_PLL	10
#define WARMUP_DMI_LATENCY		20
#define FDI_TRAIN_PATTERN_1_TIME	0.5
#define FDI_TRAIN_PATTERN_2_TIME	1.5
#define FDI_ONE_IDLE_PATTERN_TIME	31

#define CPU_VGACNTRL		0x41000

#define DIGITAL_PORT_HOTPLUG_CNTRL	0x44030
#define  DIGITAL_PORTA_HOTPLUG_ENABLE		(1 << 4)
#define  DIGITAL_PORTA_SHORT_PULSE_2MS		(0 << 2)
#define  DIGITAL_PORTA_SHORT_PULSE_4_5MS	(1 << 2)
#define  DIGITAL_PORTA_SHORT_PULSE_6MS		(2 << 2)
#define  DIGITAL_PORTA_SHORT_PULSE_100MS	(3 << 2)
#define  DIGITAL_PORTA_NO_DETECT		(0 << 0)
#define  DIGITAL_PORTA_LONG_PULSE_DETECT_MASK	(1 << 1)
#define  DIGITAL_PORTA_SHORT_PULSE_DETECT_MASK	(1 << 0)

/* refresh rate hardware control */
#define RR_HW_CTL	0x45300
#define  RR_HW_LOW_POWER_FRAMES_MASK	0xff
#define  RR_HW_HIGH_POWER_FRAMES_MASK	0xff00

#define FDI_PLL_BIOS_0			0x46000
#define FDI_PLL_BIOS_1			0x46004
#define FDI_PLL_BIOS_2			0x46008
#define DISPLAY_PORT_PLL_BIOS_0		0x4600c
#define DISPLAY_PORT_PLL_BIOS_1		0x46010
#define DISPLAY_PORT_PLL_BIOS_2		0x46014

#define FDI_PLL_FREQ_CTL	0x46030
#define  FDI_PLL_FREQ_CHANGE_REQUEST	(1<<24)
#define  FDI_PLL_FREQ_LOCK_LIMIT_MASK	0xfff00
#define  FDI_PLL_FREQ_DISABLE_COUNT_LIMIT_MASK	0xff

#define PIPEA_DATA_M1		0x60030
#define  TU_SIZE(x)		(((x)-1) << 25) /* default size 64 */
#define  TU_SIZE_MASK		0x7e000000
#define  PIPEA_DATA_M1_OFFSET	0
#define PIPEA_DATA_N1		0x60034
#define  PIPEA_DATA_N1_OFFSET	0

#define PIPEA_DATA_M2		0x60038
#define  PIPEA_DATA_M2_OFFSET	0
#define PIPEA_DATA_N2		0x6003c
#define  PIPEA_DATA_N2_OFFSET	0

#define PIPEA_LINK_M1		0x60040
#define  PIPEA_LINK_M1_OFFSET	0
#define PIPEA_LINK_N1		0x60044
#define  PIPEA_LINK_N1_OFFSET	0

#define PIPEA_LINK_M2		0x60048
#define  PIPEA_LINK_M2_OFFSET	0
#define PIPEA_LINK_N2		0x6004c
#define  PIPEA_LINK_N2_OFFSET	0

/* PIPEB timing regs are same start from 0x61000 */

#define PIPEB_DATA_M1		0x61030
#define  PIPEB_DATA_M1_OFFSET	0
#define PIPEB_DATA_N1		0x61034
#define  PIPEB_DATA_N1_OFFSET	0

#define PIPEB_DATA_M2		0x61038
#define  PIPEB_DATA_M2_OFFSET	0
#define PIPEB_DATA_N2		0x6103c
#define  PIPEB_DATA_N2_OFFSET	0

#define PIPEB_LINK_M1		0x61040
#define  PIPEB_LINK_M1_OFFSET	0
#define PIPEB_LINK_N1		0x61044
#define  PIPEB_LINK_N1_OFFSET	0

#define PIPEB_LINK_M2		0x61048
#define  PIPEB_LINK_M2_OFFSET	0
#define PIPEB_LINK_N2		0x6104c
#define  PIPEB_LINK_N2_OFFSET	0

/* PIPECONF for pipe A/B addr is same */

/* cusor A is only connected to pipe A,
   cursor B is connected to pipe B. Otherwise no change. */

/* Plane A/B, DSPACNTR/DSPBCNTR addr not changed */

/* CPU panel fitter */
#define PFA_CTL_1		0x68080
#define PFB_CTL_1		0x68880
#define  PF_ENABLE		(1<<31)

#define PFA_WIN_POS		0x68070
#define PFB_WIN_POS		0x68870
#define PFA_WIN_SIZE		0x68074
#define PFB_WIN_SIZE		0x68874

/* legacy palette */
#define LGC_PALETTE_A		0x4a000
#define LGC_PALETTE_B		0x4a800

/* interrupts */
#define DE_MASTER_IRQ_CONTROL	(1 << 31)
#define DE_SPRITEB_FLIP_DONE	(1 << 29)
#define DE_SPRITEA_FLIP_DONE	(1 << 28)
#define DE_PLANEB_FLIP_DONE	(1 << 27)
#define DE_PLANEA_FLIP_DONE	(1 << 26)
#define DE_PCU_EVENT		(1 << 25)
#define DE_GTT_FAULT		(1 << 24)
#define DE_POISON		(1 << 23)
#define DE_PERFORM_COUNTER	(1 << 22)
#define DE_PCH_EVENT		(1 << 21)
#define DE_AUX_CHANNEL_A	(1 << 20)
#define DE_DP_A_HOTPLUG		(1 << 19)
#define DE_GSE			(1 << 18)
#define DE_PIPEB_VBLANK		(1 << 15)
#define DE_PIPEB_EVEN_FIELD	(1 << 14)
#define DE_PIPEB_ODD_FIELD	(1 << 13)
#define DE_PIPEB_LINE_COMPARE	(1 << 12)
#define DE_PIPEB_VSYNC		(1 << 11)
#define DE_PIPEB_FIFO_UNDERRUN	(1 << 8)
#define DE_PIPEA_VBLANK		(1 << 7)
#define DE_PIPEA_EVEN_FIELD	(1 << 6)
#define DE_PIPEA_ODD_FIELD	(1 << 5)
#define DE_PIPEA_LINE_COMPARE	(1 << 4)
#define DE_PIPEA_VSYNC		(1 << 3)
#define DE_PIPEA_FIFO_UNDERRUN	(1 << 0)

#define DEISR	0x44000
#define DEIMR	0x44004
#define DEIIR	0x44008
#define DEIER	0x4400c

/* GT interrupt */
#define GT_SYNC_STATUS		(1 << 2)
#define GT_USER_INTERRUPT	(1 << 0)

#define GTISR	0x44010
#define GTIMR	0x44014
#define GTIIR	0x44018
#define GTIER	0x4401c

/* PCH */

/* south display engine interrupt */
#define SDE_CRT_HOTPLUG		(1 << 11)
#define SDE_PORTD_HOTPLUG	(1 << 10)
#define SDE_PORTC_HOTPLUG	(1 << 9)
#define SDE_PORTB_HOTPLUG	(1 << 8)
#define SDE_SDVOB_HOTPLUG	(1 << 6)

#define SDEISR	0xc4000
#define SDEIMR	0xc4004
#define SDEIIR	0xc4008
#define SDEIER	0xc400c

/* digital port hotplug */
#define PCH_PORT_HOTPLUG	0xc4030
#define PORTD_HOTPLUG_ENABLE		(1 << 20)
#define PORTD_PULSE_DURATION_2ms	(0)
#define PORTD_PULSE_DURATION_4_5ms	(1 << 18)
#define PORTD_PULSE_DURATION_6ms	(2 << 18)
#define PORTD_PULSE_DURATION_100ms	(3 << 18)
#define PORTD_HOTPLUG_NO_DETECT		(0)
#define PORTD_HOTPLUG_SHORT_DETECT	(1 << 16)
#define PORTD_HOTPLUG_LONG_DETECT	(1 << 17)
#define PORTC_HOTPLUG_ENABLE		(1 << 12)
#define PORTC_PULSE_DURATION_2ms	(0)
#define PORTC_PULSE_DURATION_4_5ms	(1 << 10)
#define PORTC_PULSE_DURATION_6ms	(2 << 10)
#define PORTC_PULSE_DURATION_100ms	(3 << 10)
#define PORTC_HOTPLUG_NO_DETECT		(0)
#define PORTC_HOTPLUG_SHORT_DETECT	(1 << 8)
#define PORTC_HOTPLUG_LONG_DETECT	(1 << 9)
#define PORTB_HOTPLUG_ENABLE		(1 << 4)
#define PORTB_PULSE_DURATION_2ms	(0)
#define PORTB_PULSE_DURATION_4_5ms	(1 << 2)
#define PORTB_PULSE_DURATION_6ms	(2 << 2)
#define PORTB_PULSE_DURATION_100ms	(3 << 2)
#define PORTB_HOTPLUG_NO_DETECT		(0)
#define PORTB_HOTPLUG_SHORT_DETECT	(1 << 0)
#define PORTB_HOTPLUG_LONG_DETECT	(1 << 1)

#define PCH_GPIOA		0xc5010
#define PCH_GPIOB		0xc5014
#define PCH_GPIOC		0xc5018
#define PCH_GPIOD		0xc501c
#define PCH_GPIOE		0xc5020
#define PCH_GPIOF		0xc5024
#define PCH_GMBUS0		0xc5100
#define PCH_GMBUS1		0xc5104
#define PCH_GMBUS2		0xc5108
#define PCH_GMBUS3		0xc510c
#define PCH_GMBUS4		0xc5110
#define PCH_GMBUS5		0xc5120

#define PCH_DPLL_A		0xc6014
#define PCH_DPLL_B		0xc6018

#define PCH_FPA0		0xc6040
#define PCH_FPA1		0xc6044
#define PCH_FPB0		0xc6048
#define PCH_FPB1		0xc604c

#define PCH_DPLL_TEST		0xc606c

#define PCH_DREF_CONTROL	0xC6200
#define  DREF_CONTROL_MASK	0x7fc3
#define  DREF_CPU_SOURCE_OUTPUT_DISABLE		(0<<13)
#define  DREF_CPU_SOURCE_OUTPUT_DOWNSPREAD	(2<<13)
#define  DREF_CPU_SOURCE_OUTPUT_NONSPREAD	(3<<13)
#define  DREF_SSC_SOURCE_DISABLE		(0<<11)
#define  DREF_SSC_SOURCE_ENABLE			(2<<11)
#define  DREF_NONSPREAD_SOURCE_DISABLE		(0<<9)
#define  DREF_NONSPREAD_SOURCE_ENABLE		(2<<9)
#define  DREF_SUPERSPREAD_SOURCE_DISABLE	(0<<7)
#define  DREF_SUPERSPREAD_SOURCE_ENABLE		(2<<7)
#define  DREF_SSC4_DOWNSPREAD			(0<<6)
#define  DREF_SSC4_CENTERSPREAD			(1<<6)
#define  DREF_SSC1_DISABLE			(0<<1)
#define  DREF_SSC1_ENABLE			(1<<1)
#define  DREF_SSC4_DISABLE			(0)
#define  DREF_SSC4_ENABLE			(1)

#define PCH_RAWCLK_FREQ		0xc6204
#define  FDL_TP1_TIMER_SHIFT	12
#define  FDL_TP1_TIMER_MASK	(3<<12)
#define  FDL_TP2_TIMER_SHIFT	10
#define  FDL_TP2_TIMER_MASK	(3<<10)
#define  RAWCLK_FREQ_MASK	0x3ff

#define PCH_DPLL_TMR_CFG	0xc6208

#define PCH_SSC4_PARMS		0xc6210
#define PCH_SSC4_AUX_PARMS	0xc6214

/* transcoder */

#define TRANS_HTOTAL_A		0xe0000
#define  TRANS_HTOTAL_SHIFT	16
#define  TRANS_HACTIVE_SHIFT	0
#define TRANS_HBLANK_A		0xe0004
#define  TRANS_HBLANK_END_SHIFT	16
#define  TRANS_HBLANK_START_SHIFT 0
#define TRANS_HSYNC_A		0xe0008
#define  TRANS_HSYNC_END_SHIFT	16
#define  TRANS_HSYNC_START_SHIFT 0
#define TRANS_VTOTAL_A		0xe000c
#define  TRANS_VTOTAL_SHIFT	16
#define  TRANS_VACTIVE_SHIFT	0
#define TRANS_VBLANK_A		0xe0010
#define  TRANS_VBLANK_END_SHIFT	16
#define  TRANS_VBLANK_START_SHIFT 0
#define TRANS_VSYNC_A		0xe0014
#define  TRANS_VSYNC_END_SHIFT	16
#define  TRANS_VSYNC_START_SHIFT 0

#define TRANSA_DATA_M1		0xe0030
#define TRANSA_DATA_N1		0xe0034
#define TRANSA_DATA_M2		0xe0038
#define TRANSA_DATA_N2		0xe003c
#define TRANSA_DP_LINK_M1	0xe0040
#define TRANSA_DP_LINK_N1	0xe0044
#define TRANSA_DP_LINK_M2	0xe0048
#define TRANSA_DP_LINK_N2	0xe004c

#define TRANS_HTOTAL_B		0xe1000
#define TRANS_HBLANK_B		0xe1004
#define TRANS_HSYNC_B		0xe1008
#define TRANS_VTOTAL_B		0xe100c
#define TRANS_VBLANK_B		0xe1010
#define TRANS_VSYNC_B		0xe1014

#define TRANSB_DATA_M1		0xe1030
#define TRANSB_DATA_N1		0xe1034
#define TRANSB_DATA_M2		0xe1038
#define TRANSB_DATA_N2		0xe103c
#define TRANSB_DP_LINK_M1	0xe1040
#define TRANSB_DP_LINK_N1	0xe1044
#define TRANSB_DP_LINK_M2	0xe1048
#define TRANSB_DP_LINK_N2	0xe104c

#define TRANSACONF		0xf0008
#define TRANSBCONF		0xf1008
#define  TRANS_DISABLE		(0<<31)
#define  TRANS_ENABLE		(1<<31)
#define  TRANS_STATE_MASK	(1<<30)
#define  TRANS_STATE_DISABLE	(0<<30)
#define  TRANS_STATE_ENABLE	(1<<30)
#define  TRANS_FSYNC_DELAY_HB1	(0<<27)
#define  TRANS_FSYNC_DELAY_HB2	(1<<27)
#define  TRANS_FSYNC_DELAY_HB3	(2<<27)
#define  TRANS_FSYNC_DELAY_HB4	(3<<27)
#define  TRANS_DP_AUDIO_ONLY	(1<<26)
#define  TRANS_DP_VIDEO_AUDIO	(0<<26)
#define  TRANS_PROGRESSIVE	(0<<21)
#define  TRANS_8BPC		(0<<5)
#define  TRANS_10BPC		(1<<5)
#define  TRANS_6BPC		(2<<5)
#define  TRANS_12BPC		(3<<5)

#define FDI_RXA_CHICKEN		0xc200c
#define FDI_RXB_CHICKEN		0xc2010
#define  FDI_RX_PHASE_SYNC_POINTER_ENABLE	(1)

/* CPU: FDI_TX */
#define FDI_TXA_CTL		0x60100
#define FDI_TXB_CTL		0x61100
#define  FDI_TX_DISABLE		(0<<31)
#define  FDI_TX_ENABLE		(1<<31)
#define  FDI_LINK_TRAIN_PATTERN_1	(0<<28)
#define  FDI_LINK_TRAIN_PATTERN_2	(1<<28)
#define  FDI_LINK_TRAIN_PATTERN_IDLE	(2<<28)
#define  FDI_LINK_TRAIN_NONE		(3<<28)
#define  FDI_LINK_TRAIN_VOLTAGE_0_4V	(0<<25)
#define  FDI_LINK_TRAIN_VOLTAGE_0_6V	(1<<25)
#define  FDI_LINK_TRAIN_VOLTAGE_0_8V	(2<<25)
#define  FDI_LINK_TRAIN_VOLTAGE_1_2V	(3<<25)
#define  FDI_LINK_TRAIN_PRE_EMPHASIS_NONE (0<<22)
#define  FDI_LINK_TRAIN_PRE_EMPHASIS_1_5X (1<<22)
#define  FDI_LINK_TRAIN_PRE_EMPHASIS_2X	  (2<<22)
#define  FDI_LINK_TRAIN_PRE_EMPHASIS_3X	  (3<<22)
#define  FDI_DP_PORT_WIDTH_X1		(0<<19)
#define  FDI_DP_PORT_WIDTH_X2		(1<<19)
#define  FDI_DP_PORT_WIDTH_X3		(2<<19)
#define  FDI_DP_PORT_WIDTH_X4		(3<<19)
#define  FDI_TX_ENHANCE_FRAME_ENABLE	(1<<18)
/* Ironlake: hardwired to 1 */
#define  FDI_TX_PLL_ENABLE		(1<<14)
/* both Tx and Rx */
#define  FDI_SCRAMBLING_ENABLE		(0<<7)
#define  FDI_SCRAMBLING_DISABLE		(1<<7)

/* FDI_RX, FDI_X is hard-wired to Transcoder_X */
#define FDI_RXA_CTL		0xf000c
#define FDI_RXB_CTL		0xf100c
#define  FDI_RX_ENABLE		(1<<31)
#define  FDI_RX_DISABLE		(0<<31)
/* train, dp width same as FDI_TX */
#define  FDI_DP_PORT_WIDTH_X8		(7<<19)
#define  FDI_8BPC			(0<<16)
#define  FDI_10BPC			(1<<16)
#define  FDI_6BPC			(2<<16)
#define  FDI_12BPC			(3<<16)
#define  FDI_LINK_REVERSE_OVERWRITE	(1<<15)
#define  FDI_DMI_LINK_REVERSE_MASK	(1<<14)
#define  FDI_RX_PLL_ENABLE		(1<<13)
#define  FDI_FS_ERR_CORRECT_ENABLE	(1<<11)
#define  FDI_FE_ERR_CORRECT_ENABLE	(1<<10)
#define  FDI_FS_ERR_REPORT_ENABLE	(1<<9)
#define  FDI_FE_ERR_REPORT_ENABLE	(1<<8)
#define  FDI_RX_ENHANCE_FRAME_ENABLE	(1<<6)
#define  FDI_SEL_RAWCLK			(0<<4)
#define  FDI_SEL_PCDCLK			(1<<4)

#define FDI_RXA_MISC		0xf0010
#define FDI_RXB_MISC		0xf1010
#define FDI_RXA_TUSIZE1		0xf0030
#define FDI_RXA_TUSIZE2		0xf0038
#define FDI_RXB_TUSIZE1		0xf1030
#define FDI_RXB_TUSIZE2		0xf1038

/* FDI_RX interrupt register format */
#define FDI_RX_INTER_LANE_ALIGN		(1<<10)
#define FDI_RX_SYMBOL_LOCK		(1<<9) /* train 2 */
#define FDI_RX_BIT_LOCK			(1<<8) /* train 1 */
#define FDI_RX_TRAIN_PATTERN_2_FAIL	(1<<7)
#define FDI_RX_FS_CODE_ERR		(1<<6)
#define FDI_RX_FE_CODE_ERR		(1<<5)
#define FDI_RX_SYMBOL_ERR_RATE_ABOVE	(1<<4)
#define FDI_RX_HDCP_LINK_FAIL		(1<<3)
#define FDI_RX_PIXEL_FIFO_OVERFLOW	(1<<2)
#define FDI_RX_CROSS_CLOCK_OVERFLOW	(1<<1)
#define FDI_RX_SYMBOL_QUEUE_OVERFLOW	(1<<0)

#define FDI_RXA_IIR		0xf0014
#define FDI_RXA_IMR		0xf0018
#define FDI_RXB_IIR		0xf1014
#define FDI_RXB_IMR		0xf1018

#define FDI_PLL_CTL_1		0xfe000
#define FDI_PLL_CTL_2		0xfe004

/* CRT */
#define PCH_ADPA		0xe1100
#define  ADPA_TRANS_SELECT_MASK	(1<<30)
#define  ADPA_TRANS_A_SELECT	0
#define  ADPA_TRANS_B_SELECT	(1<<30)
/* HPD is here */
#define  ADPA_CRT_HOTPLUG_MASK	0x03ff0000 /* bit 25-16 */
#define	 ADPA_CRT_HOTPLUG_MONITOR_NONE	(0<<24)
#define  ADPA_CRT_HOTPLUG_MONITOR_MASK	(3<<24)
#define  ADPA_CRT_HOTPLUG_MONITOR_COLOR	(3<<24)
#define  ADPA_CRT_HOTPLUG_MONITOR_MONO	(2<<24)
#define  ADPA_CRT_HOTPLUG_ENABLE	(1<<23)
#define  ADPA_CRT_HOTPLUG_PERIOD_64	(0<<22)
#define  ADPA_CRT_HOTPLUG_PERIOD_128	(1<<22)
#define  ADPA_CRT_HOTPLUG_WARMUP_5MS	(0<<21)
#define  ADPA_CRT_HOTPLUG_WARMUP_10MS	(1<<21)
#define  ADPA_CRT_HOTPLUG_SAMPLE_2S	(0<<20)
#define  ADPA_CRT_HOTPLUG_SAMPLE_4S	(1<<20)
#define  ADPA_CRT_HOTPLUG_VOLTAGE_40	(0<<18)
#define  ADPA_CRT_HOTPLUG_VOLTAGE_50	(1<<18)
#define  ADPA_CRT_HOTPLUG_VOLTAGE_60	(2<<18)
#define  ADPA_CRT_HOTPLUG_VOLTAGE_70	(3<<18)
#define  ADPA_CRT_HOTPLUG_VOLREF_325MV	(0<<17)
#define  ADPA_CRT_HOTPLUG_VOLREF_475MV	(1<<17)
#define  ADPA_CRT_HOTPLUG_FORCE_TRIGGER	(1<<16)
/* polarity control not changed */

/* or SDVOB */
#define HDMIB	0xe1140
#define  PORT_ENABLE	(1 << 31)
#define  TRANSCODER_A	(0)
#define  TRANSCODER_B	(1 << 30)
#define  COLOR_FORMAT_8bpc	(0)
#define  COLOR_FORMAT_12bpc	(3 << 26)
#define  SDVOB_HOTPLUG_ENABLE	(1 << 23)
#define  SDVO_ENCODING		(0)
#define  TMDS_ENCODING		(2 << 10)
#define  NULL_PACKET_VSYNC_ENABLE	(1 << 9)
#define  SDVOB_BORDER_ENABLE	(1 << 7)
#define  AUDIO_ENABLE		(1 << 6)
#define  VSYNC_ACTIVE_HIGH	(1 << 4)
#define  HSYNC_ACTIVE_HIGH	(1 << 3)
#define  PORT_DETECTED		(1 << 2)

#define HDMIC	0xe1150
#define HDMID	0xe1160
#define PCH_LVDS		0xe1180

#define AUD_CONFIG              0x62000
#define AUD_DEBUG               0x62010
#define AUD_VID_DID             0x62020
#define AUD_RID                 0x62024
#define AUD_SUBN_CNT            0x62028
#define AUD_FUNC_GRP            0x62040
#define AUD_SUBN_CNT2           0x62044
#define AUD_GRP_CAP             0x62048
#define AUD_PWRST               0x6204c
#define AUD_SUPPWR              0x62050
#define AUD_SID                 0x62054
#define AUD_OUT_CWCAP           0x62070
#define AUD_OUT_PCMSIZE         0x62074
#define AUD_OUT_STR             0x62078
#define AUD_OUT_DIG_CNVT        0x6207c
#define AUD_OUT_CH_STR          0x62080
#define AUD_OUT_STR_DESC        0x62084
#define AUD_PINW_CAP            0x620a0
#define AUD_PIN_CAP             0x620a4
#define AUD_PINW_CONNLNG        0x620a8
#define AUD_PINW_CONNLST        0x620ac
#define AUD_PINW_CNTR           0x620b0
#define AUD_PINW_UNSOLRESP      0x620b8
#define AUD_CNTL_ST             0x620b4
#define AUD_PINW_CONFIG         0x620bc
#define AUD_HDMIW_STATUS        0x620d4
#define AUD_HDMIW_HDMIEDID      0x6210c
#define AUD_HDMIW_INFOFR        0x62118
#define AUD_CONV_CHCNT          0x62120
#define AUD_CTS_ENABLE          0x62128

#define VIDEO_DIP_CTL           0x61170

#endif /* _I810_REG_H */
