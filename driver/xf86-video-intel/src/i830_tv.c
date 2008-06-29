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
 * Integrated TV-out support for the 915GM and 945GM.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "i830.h"
#include "i830_display.h"
#include "X11/Xatom.h"
#include <string.h>

enum tv_type {
    TV_TYPE_NONE,
    TV_TYPE_UNKNOWN,
    TV_TYPE_COMPOSITE,
    TV_TYPE_SVIDEO,
    TV_TYPE_COMPONENT
};

enum tv_margin {
    TV_MARGIN_LEFT, TV_MARGIN_TOP,
    TV_MARGIN_RIGHT, TV_MARGIN_BOTTOM
};

/** Private structure for the integrated TV support */
struct i830_tv_priv {
    int type;
    char *tv_format;
    int margin[4];
    uint32_t save_TV_H_CTL_1;
    uint32_t save_TV_H_CTL_2;
    uint32_t save_TV_H_CTL_3;
    uint32_t save_TV_V_CTL_1;
    uint32_t save_TV_V_CTL_2;
    uint32_t save_TV_V_CTL_3;
    uint32_t save_TV_V_CTL_4;
    uint32_t save_TV_V_CTL_5;
    uint32_t save_TV_V_CTL_6;
    uint32_t save_TV_V_CTL_7;
    uint32_t save_TV_SC_CTL_1, save_TV_SC_CTL_2, save_TV_SC_CTL_3;

    uint32_t save_TV_CSC_Y;
    uint32_t save_TV_CSC_Y2;
    uint32_t save_TV_CSC_U;
    uint32_t save_TV_CSC_U2;
    uint32_t save_TV_CSC_V;
    uint32_t save_TV_CSC_V2;
    uint32_t save_TV_CLR_KNOBS;
    uint32_t save_TV_CLR_LEVEL;
    uint32_t save_TV_WIN_POS;
    uint32_t save_TV_WIN_SIZE;
    uint32_t save_TV_FILTER_CTL_1;
    uint32_t save_TV_FILTER_CTL_2;
    uint32_t save_TV_FILTER_CTL_3;

    uint32_t save_TV_H_LUMA[60];
    uint32_t save_TV_H_CHROMA[60];
    uint32_t save_TV_V_LUMA[43];
    uint32_t save_TV_V_CHROMA[43];

    uint32_t save_TV_DAC;
    uint32_t save_TV_CTL;
};

typedef struct {
    int	blank, black, burst;
} video_levels_t;

typedef struct {
    float   ry, gy, by, ay;
    float   ru, gu, bu, au;
    float   rv, gv, bv, av;
} color_conversion_t;

static const uint32_t filter_table[] = {
    0xB1403000, 0x2E203500, 0x35002E20, 0x3000B140,
    0x35A0B160, 0x2DC02E80, 0xB1403480, 0xB1603000,
    0x2EA03640, 0x34002D80, 0x3000B120, 0x36E0B160,
    0x2D202EF0, 0xB1203380, 0xB1603000, 0x2F303780,
    0x33002CC0, 0x3000B100, 0x3820B160, 0x2C802F50,
    0xB10032A0, 0xB1603000, 0x2F9038C0, 0x32202C20,
    0x3000B0E0, 0x3980B160, 0x2BC02FC0, 0xB0E031C0,
    0xB1603000, 0x2FF03A20, 0x31602B60, 0xB020B0C0,
    0x3AE0B160, 0x2B001810, 0xB0C03120, 0xB140B020,
    0x18283BA0, 0x30C02A80, 0xB020B0A0, 0x3C60B140,
    0x2A201838, 0xB0A03080, 0xB120B020, 0x18383D20,
    0x304029C0, 0xB040B080, 0x3DE0B100, 0x29601848,
    0xB0803000, 0xB100B040, 0x18483EC0, 0xB0402900,
    0xB040B060, 0x3F80B0C0, 0x28801858, 0xB060B080,
    0xB0A0B060, 0x18602820, 0xB0A02820, 0x0000B060,
    0xB1403000, 0x2E203500, 0x35002E20, 0x3000B140,
    0x35A0B160, 0x2DC02E80, 0xB1403480, 0xB1603000,
    0x2EA03640, 0x34002D80, 0x3000B120, 0x36E0B160,
    0x2D202EF0, 0xB1203380, 0xB1603000, 0x2F303780,
    0x33002CC0, 0x3000B100, 0x3820B160, 0x2C802F50,
    0xB10032A0, 0xB1603000, 0x2F9038C0, 0x32202C20,
    0x3000B0E0, 0x3980B160, 0x2BC02FC0, 0xB0E031C0,
    0xB1603000, 0x2FF03A20, 0x31602B60, 0xB020B0C0,
    0x3AE0B160, 0x2B001810, 0xB0C03120, 0xB140B020,
    0x18283BA0, 0x30C02A80, 0xB020B0A0, 0x3C60B140,
    0x2A201838, 0xB0A03080, 0xB120B020, 0x18383D20,
    0x304029C0, 0xB040B080, 0x3DE0B100, 0x29601848,
    0xB0803000, 0xB100B040, 0x18483EC0, 0xB0402900,
    0xB040B060, 0x3F80B0C0, 0x28801858, 0xB060B080,
    0xB0A0B060, 0x18602820, 0xB0A02820, 0x0000B060,
    0x36403000, 0x2D002CC0, 0x30003640, 0x2D0036C0,
    0x35C02CC0, 0x37403000, 0x2C802D40, 0x30003540,
    0x2D8037C0, 0x34C02C40, 0x38403000, 0x2BC02E00,
    0x30003440, 0x2E2038C0, 0x34002B80, 0x39803000,
    0x2B402E40, 0x30003380, 0x2E603A00, 0x33402B00,
    0x3A803040, 0x2A802EA0, 0x30403300, 0x2EC03B40,
    0x32802A40, 0x3C003040, 0x2A002EC0, 0x30803240,
    0x2EC03C80, 0x320029C0, 0x3D403080, 0x29402F00,
    0x308031C0, 0x2F203DC0, 0x31802900, 0x3E8030C0,
    0x28802F40, 0x30C03140, 0x2F203F40, 0x31402840,
    0x28003100, 0x28002F00, 0x00003100, 0x36403000, 
    0x2D002CC0, 0x30003640, 0x2D0036C0,
    0x35C02CC0, 0x37403000, 0x2C802D40, 0x30003540,
    0x2D8037C0, 0x34C02C40, 0x38403000, 0x2BC02E00,
    0x30003440, 0x2E2038C0, 0x34002B80, 0x39803000,
    0x2B402E40, 0x30003380, 0x2E603A00, 0x33402B00,
    0x3A803040, 0x2A802EA0, 0x30403300, 0x2EC03B40,
    0x32802A40, 0x3C003040, 0x2A002EC0, 0x30803240,
    0x2EC03C80, 0x320029C0, 0x3D403080, 0x29402F00,
    0x308031C0, 0x2F203DC0, 0x31802900, 0x3E8030C0,
    0x28802F40, 0x30C03140, 0x2F203F40, 0x31402840,
    0x28003100, 0x28002F00, 0x00003100,
};

typedef struct {
    char *name;
    int	clock;
    double refresh;
    uint32_t oversample;
    int hsync_end, hblank_start, hblank_end, htotal;
    Bool progressive, trilevel_sync, component_only;
    int vsync_start_f1, vsync_start_f2, vsync_len;
    Bool veq_ena;
    int veq_start_f1, veq_start_f2, veq_len;
    int vi_end_f1, vi_end_f2, nbr_end;
    Bool burst_ena;
    int hburst_start, hburst_len;
    int vburst_start_f1, vburst_end_f1;
    int vburst_start_f2, vburst_end_f2;
    int vburst_start_f3, vburst_end_f3;
    int vburst_start_f4, vburst_end_f4;
    /*
     * subcarrier programming
     */
    int dda2_size, dda3_size, dda1_inc, dda2_inc, dda3_inc;
    uint32_t sc_reset;
    Bool pal_burst;
    /*
     * blank/black levels
     */
    video_levels_t	composite_levels, svideo_levels;
    color_conversion_t	composite_color, svideo_color;
    const uint32_t *filter_table;
    int max_srcw;
} tv_mode_t;


/*
 * Sub carrier DDA
 *
 *  I think this works as follows:
 *
 *  subcarrier freq = pixel_clock * (dda1_inc + dda2_inc / dda2_size) / 4096
 *
 * Presumably, when dda3 is added in, it gets to adjust the dda2_inc value
 *
 * So,
 *  dda1_ideal = subcarrier/pixel * 4096
 *  dda1_inc = floor (dda1_ideal)
 *  dda2 = dda1_ideal - dda1_inc
 *
 *  then pick a ratio for dda2 that gives the closest approximation. If
 *  you can't get close enough, you can play with dda3 as well. This
 *  seems likely to happen when dda2 is small as the jumps would be larger
 *
 * To invert this,
 *
 *  pixel_clock = subcarrier * 4096 / (dda1_inc + dda2_inc / dda2_size)
 *
 * The constants below were all computed using a 107.520MHz clock
 */
 
/**
 * Register programming values for TV modes.
 *
 * These values account for -1s required.
 */

const static tv_mode_t tv_modes[] = {
    {
	.name		= "NTSC-M",
	.clock		= 107520,	
	.refresh	= 29.97,
	.oversample	= TV_OVERSAMPLE_8X,
	.component_only = 0,
	/* 525 Lines, 60 Fields, 15.734KHz line, Sub-Carrier 3.580MHz */

	.hsync_end	= 64,		    .hblank_end		= 124,
	.hblank_start	= 836,		    .htotal		= 857,

	.progressive	= FALSE,	    .trilevel_sync = FALSE,

	.vsync_start_f1	= 6,		    .vsync_start_f2	= 7,
	.vsync_len	= 6,

	.veq_ena	= TRUE,		    .veq_start_f1    	= 0,
	.veq_start_f2	= 1,		    .veq_len		= 18,

	.vi_end_f1	= 20,		    .vi_end_f2		= 21,
	.nbr_end	= 240,

	.burst_ena	= TRUE,
	.hburst_start	= 72,		    .hburst_len		= 34,
	.vburst_start_f1 = 9,		    .vburst_end_f1	= 240,
	.vburst_start_f2 = 10,		    .vburst_end_f2	= 240,
	.vburst_start_f3 = 9,		    .vburst_end_f3	= 240, 
	.vburst_start_f4 = 10,		    .vburst_end_f4	= 240,

	/* desired 3.5800000 actual 3.5800000 clock 107.52 */
	.dda1_inc	=    136,
	.dda2_inc	=   7624,	    .dda2_size		=  20013,
	.dda3_inc	=      0,	    .dda3_size		=      0,
	.sc_reset	= TV_SC_RESET_EVERY_4,
	.pal_burst	= FALSE,

	.composite_levels = { .blank = 225, .black = 267, .burst = 113 },
	.composite_color = {
	    .ry = 0.2990, .gy = 0.5870, .by = 0.1140, .ay = 0.5082,
	    .ru =-0.0749, .gu =-0.1471, .bu = 0.2220, .au = 1.0000,
	    .rv = 0.3125, .gv =-0.2616, .bv =-0.0508, .av = 1.0000,
	},

	.svideo_levels    = { .blank = 266, .black = 316, .burst = 133 },
	.svideo_color = {
	    .ry = 0.2990, .gy = 0.5870, .by = 0.1140, .ay = 0.6006,
	    .ru =-0.0885, .gu =-0.1738, .bu = 0.2624, .au = 1.0000,
	    .rv = 0.3693, .gv =-0.3092, .bv =-0.0601, .av = 1.0000,
	},
	.filter_table = filter_table,
    },
    {
	.name		= "NTSC-443",
	.clock		= 107520,	
	.refresh	= 29.97,
	.oversample	= TV_OVERSAMPLE_8X,
	.component_only = 0,
	/* 525 Lines, 60 Fields, 15.734KHz line, Sub-Carrier 4.43MHz */
	.hsync_end	= 64,		    .hblank_end		= 124,
	.hblank_start	= 836,		    .htotal		= 857,

	.progressive	= FALSE,	    .trilevel_sync = FALSE,

	.vsync_start_f1 = 6,		    .vsync_start_f2	= 7,
	.vsync_len	= 6,

	.veq_ena	= TRUE,		    .veq_start_f1    	= 0,
	.veq_start_f2	= 1,		    .veq_len		= 18,

	.vi_end_f1	= 20,		    .vi_end_f2		= 21,
	.nbr_end	= 240,

	.burst_ena	= 8,
	.hburst_start	= 72,		    .hburst_len		= 34,
	.vburst_start_f1 = 9,		    .vburst_end_f1	= 240,
	.vburst_start_f2 = 10,		    .vburst_end_f2	= 240,
	.vburst_start_f3 = 9,		    .vburst_end_f3	= 240, 
	.vburst_start_f4 = 10,		    .vburst_end_f4	= 240,

	/* desired 4.4336180 actual 4.4336180 clock 107.52 */
	.dda1_inc       =    168,
	.dda2_inc       =  18557,       .dda2_size      =  20625,
	.dda3_inc       =      0,       .dda3_size      =      0,
	.sc_reset   = TV_SC_RESET_EVERY_8,
	.pal_burst  = TRUE,

	.composite_levels = { .blank = 225, .black = 267, .burst = 113 },
	.composite_color = {
	    .ry = 0.2990, .gy = 0.5870, .by = 0.1140, .ay = 0.5082,
	    .ru =-0.0749, .gu =-0.1471, .bu = 0.2220, .au = 1.0000,
	    .rv = 0.3125, .gv =-0.2616, .bv =-0.0508, .av = 1.0000,
	},

	.svideo_levels    = { .blank = 266, .black = 316, .burst = 133 },
	.svideo_color = {
	    .ry = 0.2990, .gy = 0.5870, .by = 0.1140, .ay = 0.6006,
	    .ru =-0.0885, .gu =-0.1738, .bu = 0.2624, .au = 1.0000,
	    .rv = 0.3693, .gv =-0.3092, .bv =-0.0601, .av = 1.0000,
	},
	.filter_table = filter_table,
    },
    {
	.name		= "NTSC-J",
	.clock		= 107520,	
	.refresh	= 29.97,
	.oversample	= TV_OVERSAMPLE_8X,
	.component_only = 0,

	/* 525 Lines, 60 Fields, 15.734KHz line, Sub-Carrier 3.580MHz */
	.hsync_end	= 64,		    .hblank_end		= 124,
	.hblank_start = 836,	    .htotal		= 857,

	.progressive	= FALSE,    .trilevel_sync = FALSE,

	.vsync_start_f1	= 6,	    .vsync_start_f2	= 7,
	.vsync_len	= 6,

	.veq_ena	= TRUE,		    .veq_start_f1    	= 0,
	.veq_start_f2 = 1,	    .veq_len		= 18,

	.vi_end_f1	= 20,		    .vi_end_f2		= 21,
	.nbr_end	= 240,

	.burst_ena	= TRUE,
	.hburst_start	= 72,		    .hburst_len		= 34,
	.vburst_start_f1 = 9,		    .vburst_end_f1	= 240,
	.vburst_start_f2 = 10,		    .vburst_end_f2	= 240,
	.vburst_start_f3 = 9,		    .vburst_end_f3	= 240, 
	.vburst_start_f4 = 10,		    .vburst_end_f4	= 240,

	/* desired 3.5800000 actual 3.5800000 clock 107.52 */
	.dda1_inc	=    136,
	.dda2_inc	=   7624,	    .dda2_size		=  20013,
	.dda3_inc	=      0,	    .dda3_size		=      0,
	.sc_reset	= TV_SC_RESET_EVERY_4,
	.pal_burst	= FALSE,

	.composite_levels = { .blank = 225, .black = 225, .burst = 113 },
	.composite_color = {
	    .ry = 0.2990, .gy = 0.5870, .by = 0.1140, .ay = 0.5495,
	    .ru =-0.0810, .gu =-0.1590, .bu = 0.2400, .au = 1.0000,
	    .rv = 0.3378, .gv =-0.2829, .bv =-0.0549, .av = 1.0000,
	},

	.svideo_levels    = { .blank = 266, .black = 266, .burst = 133 },
	.svideo_color = {
	    .ry = 0.2990, .gy = 0.5870, .by = 0.1140, .ay = 0.6494,
	    .ru =-0.0957, .gu =-0.1879, .bu = 0.2836, .au = 1.0000,
	    .rv = 0.3992, .gv =-0.3343, .bv =-0.0649, .av = 1.0000,
	},
	.filter_table = filter_table,
    },
    {
	.name		= "PAL-M",
	.clock		= 107520,	
	.refresh	= 29.97,
	.oversample	= TV_OVERSAMPLE_8X,
	.component_only = 0,

	/* 525 Lines, 60 Fields, 15.734KHz line, Sub-Carrier 3.580MHz */
	.hsync_end	= 64,		  .hblank_end		= 124,
	.hblank_start = 836,	  .htotal		= 857,

	.progressive	= FALSE,	    .trilevel_sync = FALSE,

	.vsync_start_f1	= 6,		    .vsync_start_f2	= 7,
	.vsync_len	= 6,

	.veq_ena	= TRUE,		    .veq_start_f1    	= 0,
	.veq_start_f2	= 1,		    .veq_len		= 18,

	.vi_end_f1	= 20,		    .vi_end_f2		= 21,
	.nbr_end	= 240,

	.burst_ena	= TRUE,
	.hburst_start	= 72,		    .hburst_len		= 34,
	.vburst_start_f1 = 9,		    .vburst_end_f1	= 240,
	.vburst_start_f2 = 10,		    .vburst_end_f2	= 240,
	.vburst_start_f3 = 9,		    .vburst_end_f3	= 240, 
	.vburst_start_f4 = 10,		    .vburst_end_f4	= 240,

	/* desired 3.5800000 actual 3.5800000 clock 107.52 */
	.dda1_inc	=    136,
	.dda2_inc	=    7624,	    .dda2_size		=  20013,
	.dda3_inc	=      0,	    .dda3_size		=      0,
	.sc_reset	= TV_SC_RESET_EVERY_4,
	.pal_burst  = FALSE,

	.composite_levels = { .blank = 225, .black = 267, .burst = 113 },
	.composite_color = {
	    .ry = 0.2990, .gy = 0.5870, .by = 0.1140, .ay = 0.5082,
	    .ru =-0.0749, .gu =-0.1471, .bu = 0.2220, .au = 1.0000,
	    .rv = 0.3125, .gv =-0.2616, .bv =-0.0508, .av = 1.0000,
	},

	.svideo_levels    = { .blank = 266, .black = 316, .burst = 133 },
	.svideo_color = {
	    .ry = 0.2990, .gy = 0.5870, .by = 0.1140, .ay = 0.6006,
	    .ru =-0.0885, .gu =-0.1738, .bu = 0.2624, .au = 1.0000,
	    .rv = 0.3693, .gv =-0.3092, .bv =-0.0601, .av = 1.0000,
	},
	.filter_table = filter_table,
    },
    {
	/* 625 Lines, 50 Fields, 15.625KHz line, Sub-Carrier 4.434MHz */
	.name	    = "PAL-N",
	.clock		= 107520,	
	.refresh	= 25.0,
	.oversample	= TV_OVERSAMPLE_8X,
	.component_only = 0,

	.hsync_end	= 64,		    .hblank_end		= 128,
	.hblank_start = 844,	    .htotal		= 863,

	.progressive  = FALSE,    .trilevel_sync = FALSE,


	.vsync_start_f1	= 6,	   .vsync_start_f2	= 7,
	.vsync_len	= 6,

	.veq_ena	= TRUE,		    .veq_start_f1    	= 0,
	.veq_start_f2	= 1,		    .veq_len		= 18,

	.vi_end_f1	= 24,		    .vi_end_f2		= 25,
	.nbr_end	= 286,

	.burst_ena	= TRUE,
	.hburst_start = 73,	    	    .hburst_len		= 34,
	.vburst_start_f1 = 8,	    .vburst_end_f1	= 285,
	.vburst_start_f2 = 8,	    .vburst_end_f2	= 286,
	.vburst_start_f3 = 9,	    .vburst_end_f3	= 286, 
	.vburst_start_f4 = 9,	    .vburst_end_f4	= 285,


	/* desired 4.4336180 actual 4.4336180 clock 107.52 */
	.dda1_inc       =    168,
	.dda2_inc       =  18557,       .dda2_size      =  20625,
	.dda3_inc       =      0,       .dda3_size      =      0,
	.sc_reset   = TV_SC_RESET_EVERY_8,
	.pal_burst  = TRUE,

	.composite_levels = { .blank = 225, .black = 267, .burst = 118 },
	.composite_color = {
	    .ry = 0.2990, .gy = 0.5870, .by = 0.1140, .ay = 0.5082,
	    .ru =-0.0749, .gu =-0.1471, .bu = 0.2220, .au = 1.0000,
	    .rv = 0.3125, .gv =-0.2616, .bv =-0.0508, .av = 1.0000,
	},

	.svideo_levels    = { .blank = 266, .black = 316, .burst = 139 },
	.svideo_color = {
	    .ry = 0.2990, .gy = 0.5870, .by = 0.1140, .ay = 0.6006,
	    .ru =-0.0885, .gu =-0.1738, .bu = 0.2624, .au = 1.0000,
	    .rv = 0.3693, .gv =-0.3092, .bv =-0.0601, .av = 1.0000,
	},
	.filter_table = filter_table,
    },
    {
	/* 625 Lines, 50 Fields, 15.625KHz line, Sub-Carrier 4.434MHz */
	.name	    = "PAL",
	.clock		= 107520,	
	.refresh	= 25.0,
	.oversample	= TV_OVERSAMPLE_8X,
	.component_only = 0,

	.hsync_end	= 64,		    .hblank_end		= 128,
	.hblank_start	= 844,	    .htotal		= 863,

	.progressive	= FALSE,    .trilevel_sync = FALSE,

	.vsync_start_f1	= 5,	    .vsync_start_f2	= 6,
	.vsync_len	= 5,

	.veq_ena	= TRUE,		    .veq_start_f1    	= 0,
	.veq_start_f2	= 1,	    .veq_len		= 15,

	.vi_end_f1	= 24,		    .vi_end_f2		= 25,
	.nbr_end	= 286,

	.burst_ena	= TRUE,
	.hburst_start	= 73,		    .hburst_len		= 32,
	.vburst_start_f1 = 8,		    .vburst_end_f1	= 285,
	.vburst_start_f2 = 8,		    .vburst_end_f2	= 286,
	.vburst_start_f3 = 9,		    .vburst_end_f3	= 286, 
	.vburst_start_f4 = 9,		    .vburst_end_f4	= 285,

	/* desired 4.4336180 actual 4.4336180 clock 107.52 */
	.dda1_inc       =    168,
	.dda2_inc       =  18557,       .dda2_size      =  20625,
	.dda3_inc       =      0,       .dda3_size      =      0,
	.sc_reset   = TV_SC_RESET_EVERY_8,
	.pal_burst  = TRUE,

	.composite_levels = { .blank = 237, .black = 237, .burst = 118 },
	.composite_color = {
	    .ry = 0.2990, .gy = 0.5870, .by = 0.1140, .ay = 0.5379,
	    .ru =-0.0793, .gu =-0.1557, .bu = 0.2350, .au = 1.0000,
	    .rv = 0.3307, .gv =-0.2769, .bv =-0.0538, .av = 1.0000,
	},

	.svideo_levels    = { .blank = 280, .black = 280, .burst = 139 },
	.svideo_color = {
	    .ry = 0.2990, .gy = 0.5870, .by = 0.1140, .ay = 0.6357,
	    .ru =-0.0937, .gu =-0.1840, .bu = 0.2777, .au = 1.0000,
	    .rv = 0.3908, .gv =-0.3273, .bv =-0.0636, .av = 1.0000,
	},
	.filter_table = filter_table,
    },
    {
	.name       = "480p@59.94Hz",
	.clock 	= 107520,	
	.refresh	= 59.94,
	.oversample     = TV_OVERSAMPLE_4X,
	.component_only = 1,

	.hsync_end      = 64,               .hblank_end         = 122,
	.hblank_start   = 842,              .htotal             = 857,

	.progressive    = TRUE,.trilevel_sync = FALSE,

	.vsync_start_f1 = 12,               .vsync_start_f2     = 12,
	.vsync_len      = 12,

	.veq_ena        = FALSE,

	.vi_end_f1      = 44,               .vi_end_f2          = 44,
	.nbr_end        = 496,

	.burst_ena      = FALSE,

	.filter_table = filter_table,
    },
    {
	.name       = "480p@60Hz",
	.clock 	= 107520,	
	.refresh	= 60.0,
	.oversample     = TV_OVERSAMPLE_4X,
	.component_only = 1,

	.hsync_end      = 64,               .hblank_end         = 122,
	.hblank_start   = 842,              .htotal             = 856,

	.progressive    = TRUE,.trilevel_sync = FALSE,

	.vsync_start_f1 = 12,               .vsync_start_f2     = 12,
	.vsync_len      = 12,

	.veq_ena        = FALSE,

	.vi_end_f1      = 44,               .vi_end_f2          = 44,
	.nbr_end        = 496,

	.burst_ena      = FALSE,

	.filter_table = filter_table,
    },
    {
	.name       = "576p",
	.clock 	= 107520,	
	.refresh	= 50.0,
	.oversample     = TV_OVERSAMPLE_4X,
	.component_only = 1,

	.hsync_end      = 64,               .hblank_end         = 139,
	.hblank_start   = 859,              .htotal             = 863,

	.progressive    = TRUE,		.trilevel_sync = FALSE,

	.vsync_start_f1 = 10,               .vsync_start_f2     = 10,
	.vsync_len      = 10,

	.veq_ena        = FALSE,

	.vi_end_f1      = 48,               .vi_end_f2          = 48,
	.nbr_end        = 575,

	.burst_ena      = FALSE,

	.filter_table = filter_table,
    },
    {
	.name       = "720p@60Hz",
	.clock		= 148800,	
	.refresh	= 60.0,
	.oversample     = TV_OVERSAMPLE_2X,
	.component_only = 1,

	.hsync_end      = 80,               .hblank_end         = 300,
	.hblank_start   = 1580,             .htotal             = 1649,

	.progressive    = TRUE, 	    .trilevel_sync = TRUE,

	.vsync_start_f1 = 10,               .vsync_start_f2     = 10,
	.vsync_len      = 10,

	.veq_ena        = FALSE,

	.vi_end_f1      = 29,               .vi_end_f2          = 29,
	.nbr_end        = 719,

	.burst_ena      = FALSE,

	.filter_table = filter_table,
    },
    {
	.name       = "720p@59.94Hz",
	.clock		= 148800,	
	.refresh	= 59.94,
	.oversample     = TV_OVERSAMPLE_2X,
	.component_only = 1,

	.hsync_end      = 80,               .hblank_end         = 300,
	.hblank_start   = 1580,             .htotal             = 1651,

	.progressive    = TRUE, 	    .trilevel_sync = TRUE,

	.vsync_start_f1 = 10,               .vsync_start_f2     = 10,
	.vsync_len      = 10,

	.veq_ena        = FALSE,

	.vi_end_f1      = 29,               .vi_end_f2          = 29,
	.nbr_end        = 719,

	.burst_ena      = FALSE,

	.filter_table = filter_table,
    },
    {
	.name       = "720p@50Hz",
	.clock		= 148800,	
	.refresh	= 50.0,
	.oversample     = TV_OVERSAMPLE_2X,
	.component_only = 1,

	.hsync_end      = 80,               .hblank_end         = 300,
	.hblank_start   = 1580,             .htotal             = 1979,

	.progressive    = TRUE, 	        .trilevel_sync = TRUE,

	.vsync_start_f1 = 10,               .vsync_start_f2     = 10,
	.vsync_len      = 10,

	.veq_ena        = FALSE,

	.vi_end_f1      = 29,               .vi_end_f2          = 29,
	.nbr_end        = 719,

	.burst_ena      = FALSE,

	.filter_table = filter_table,
	.max_srcw = 800
    },
    {
	.name       = "1080i@50Hz",
	.clock		= 148800,	
	.refresh	= 25.0,
	.oversample     = TV_OVERSAMPLE_2X,
	.component_only = 1,

	.hsync_end      = 88,               .hblank_end         = 235,
	.hblank_start   = 2155,             .htotal             = 2639,

	.progressive    = FALSE, 	    .trilevel_sync = TRUE,

	.vsync_start_f1 = 4,              .vsync_start_f2     = 5,
	.vsync_len      = 10,

	.veq_ena	= TRUE,		    .veq_start_f1    	= 4,
	.veq_start_f2   = 4,	    .veq_len		= 10,


	.vi_end_f1      = 21,           .vi_end_f2          = 22,
	.nbr_end        = 539,

	.burst_ena      = FALSE,

	.filter_table = filter_table,
    },
    {
	.name       = "1080i@60Hz",
	.clock		= 148800,	
	.refresh	= 30.0,
	.oversample     = TV_OVERSAMPLE_2X,
	.component_only = 1,

	.hsync_end      = 88,               .hblank_end         = 235,
	.hblank_start   = 2155,             .htotal             = 2199,

	.progressive    = FALSE, 	    .trilevel_sync = TRUE,

	.vsync_start_f1 = 4,               .vsync_start_f2     = 5,
	.vsync_len      = 10,

	.veq_ena	= TRUE,		    .veq_start_f1    	= 4,
	.veq_start_f2	= 4,		    .veq_len		= 10,


	.vi_end_f1      = 21,               .vi_end_f2          = 22,
	.nbr_end        = 539,

	.burst_ena      = FALSE,

	.filter_table = filter_table,
    },
    {
	.name       = "1080i@59.94Hz",
	.clock		= 148800,	
	.refresh	= 29.97,
	.oversample     = TV_OVERSAMPLE_2X,
	.component_only = 1,

	.hsync_end      = 88,               .hblank_end         = 235,
	.hblank_start   = 2155,             .htotal             = 2200,

	.progressive    = FALSE, 	    .trilevel_sync = TRUE,

	.vsync_start_f1 = 4,            .vsync_start_f2    = 5,
	.vsync_len      = 10,

	.veq_ena	= TRUE,		    .veq_start_f1	= 4,
	.veq_start_f2 = 4,	    	    .veq_len = 10,


	.vi_end_f1      = 21,           .vi_end_f2         	= 22,
	.nbr_end        = 539,

	.burst_ena      = FALSE,

	.filter_table = filter_table,
    },
};

#define NUM_TV_MODES sizeof(tv_modes) / sizeof (tv_modes[0])

static const video_levels_t component_level = {
	.blank = 279, .black = 279 
};

static const color_conversion_t sdtv_component_color = {
    .ry = 0.2990, .gy = 0.5870, .by = 0.1140, .ay = 0.6364,
    .ru =-0.1687, .gu =-0.3313, .bu = 0.5000, .au = 1.0000,
    .rv = 0.5000, .gv =-0.4187, .bv =-0.0813, .av = 1.0000,
};

static const color_conversion_t hdtv_component_color = {
    .ry = 0.2126, .gy = 0.7152, .by = 0.0722, .ay = 0.6364,
    .ru =-0.1146, .gu =-0.3854, .bu = 0.5000, .au = 1.0000,
    .rv = 0.5000, .gv =-0.4542, .bv =-0.0458, .av = 1.0000,
};

static void
i830_tv_dpms(xf86OutputPtr output, int mode)
{
    ScrnInfoPtr pScrn = output->scrn;
    I830Ptr pI830 = I830PTR(pScrn);

    switch(mode) {
	case DPMSModeOn:
	    OUTREG(TV_CTL, INREG(TV_CTL) | TV_ENC_ENABLE);
	    break;
	case DPMSModeStandby:
	case DPMSModeSuspend:
	case DPMSModeOff:
	    OUTREG(TV_CTL, INREG(TV_CTL) & ~TV_ENC_ENABLE);
	    break;
    }
    i830WaitForVblank(pScrn);
}

static void
i830_tv_save(xf86OutputPtr output)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_tv_priv	    *dev_priv = intel_output->dev_priv;
    int			    i;

    dev_priv->save_TV_H_CTL_1 = INREG(TV_H_CTL_1);
    dev_priv->save_TV_H_CTL_2 = INREG(TV_H_CTL_2);
    dev_priv->save_TV_H_CTL_3 = INREG(TV_H_CTL_3);
    dev_priv->save_TV_V_CTL_1 = INREG(TV_V_CTL_1);
    dev_priv->save_TV_V_CTL_2 = INREG(TV_V_CTL_2);
    dev_priv->save_TV_V_CTL_3 = INREG(TV_V_CTL_3);
    dev_priv->save_TV_V_CTL_4 = INREG(TV_V_CTL_4);
    dev_priv->save_TV_V_CTL_5 = INREG(TV_V_CTL_5);
    dev_priv->save_TV_V_CTL_6 = INREG(TV_V_CTL_6);
    dev_priv->save_TV_V_CTL_7 = INREG(TV_V_CTL_7);
    dev_priv->save_TV_SC_CTL_1 = INREG(TV_SC_CTL_1);
    dev_priv->save_TV_SC_CTL_2 = INREG(TV_SC_CTL_2);
    dev_priv->save_TV_SC_CTL_3 = INREG(TV_SC_CTL_3);

    dev_priv->save_TV_CSC_Y = INREG(TV_CSC_Y);
    dev_priv->save_TV_CSC_Y2 = INREG(TV_CSC_Y2);
    dev_priv->save_TV_CSC_U = INREG(TV_CSC_U);
    dev_priv->save_TV_CSC_U2 = INREG(TV_CSC_U2);
    dev_priv->save_TV_CSC_V = INREG(TV_CSC_V);
    dev_priv->save_TV_CSC_V2 = INREG(TV_CSC_V2);
    dev_priv->save_TV_CLR_KNOBS = INREG(TV_CLR_KNOBS);
    dev_priv->save_TV_CLR_LEVEL = INREG(TV_CLR_LEVEL);
    dev_priv->save_TV_WIN_POS = INREG(TV_WIN_POS);
    dev_priv->save_TV_WIN_SIZE = INREG(TV_WIN_SIZE);
    dev_priv->save_TV_FILTER_CTL_1 = INREG(TV_FILTER_CTL_1);
    dev_priv->save_TV_FILTER_CTL_2 = INREG(TV_FILTER_CTL_2);
    dev_priv->save_TV_FILTER_CTL_3 = INREG(TV_FILTER_CTL_3);

    for (i = 0; i < 60; i++)
	dev_priv->save_TV_H_LUMA[i] = INREG(TV_H_LUMA_0 + (i <<2));
    for (i = 0; i < 60; i++)
	dev_priv->save_TV_H_CHROMA[i] = INREG(TV_H_CHROMA_0 + (i <<2));
    for (i = 0; i < 43; i++)
	dev_priv->save_TV_V_LUMA[i] = INREG(TV_V_LUMA_0 + (i <<2));
    for (i = 0; i < 43; i++)
	dev_priv->save_TV_V_CHROMA[i] = INREG(TV_V_CHROMA_0 + (i <<2));

    dev_priv->save_TV_DAC = INREG(TV_DAC);
    dev_priv->save_TV_CTL = INREG(TV_CTL);
}

static void
i830_tv_restore(xf86OutputPtr output)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_tv_priv	    *dev_priv = intel_output->dev_priv;
    int			    i;

    xf86CrtcPtr	    crtc = output->crtc;
    I830CrtcPrivatePtr  intel_crtc;
    if (!crtc)
	return;
    intel_crtc = crtc->driver_private;
    OUTREG(TV_H_CTL_1, dev_priv->save_TV_H_CTL_1);
    OUTREG(TV_H_CTL_2, dev_priv->save_TV_H_CTL_2);
    OUTREG(TV_H_CTL_3, dev_priv->save_TV_H_CTL_3);
    OUTREG(TV_V_CTL_1, dev_priv->save_TV_V_CTL_1);
    OUTREG(TV_V_CTL_2, dev_priv->save_TV_V_CTL_2);
    OUTREG(TV_V_CTL_3, dev_priv->save_TV_V_CTL_3);
    OUTREG(TV_V_CTL_4, dev_priv->save_TV_V_CTL_4);
    OUTREG(TV_V_CTL_5, dev_priv->save_TV_V_CTL_5);
    OUTREG(TV_V_CTL_6, dev_priv->save_TV_V_CTL_6);
    OUTREG(TV_V_CTL_7, dev_priv->save_TV_V_CTL_7);
    OUTREG(TV_SC_CTL_1, dev_priv->save_TV_SC_CTL_1);
    OUTREG(TV_SC_CTL_2, dev_priv->save_TV_SC_CTL_2);
    OUTREG(TV_SC_CTL_3, dev_priv->save_TV_SC_CTL_3);

    OUTREG(TV_CSC_Y, dev_priv->save_TV_CSC_Y);
    OUTREG(TV_CSC_Y2, dev_priv->save_TV_CSC_Y2);
    OUTREG(TV_CSC_U, dev_priv->save_TV_CSC_U);
    OUTREG(TV_CSC_U2, dev_priv->save_TV_CSC_U2);
    OUTREG(TV_CSC_V, dev_priv->save_TV_CSC_V);
    OUTREG(TV_CSC_V2, dev_priv->save_TV_CSC_V2);
    OUTREG(TV_CLR_KNOBS, dev_priv->save_TV_CLR_KNOBS);
    OUTREG(TV_CLR_LEVEL, dev_priv->save_TV_CLR_LEVEL);

    {
	int pipeconf_reg = (intel_crtc->pipe == 0) ? PIPEACONF : PIPEBCONF;
	int dspcntr_reg = (intel_crtc->plane == 0) ? DSPACNTR : DSPBCNTR;
	int pipeconf = INREG(pipeconf_reg);
	int dspcntr = INREG(dspcntr_reg);
	int dspbase_reg = (intel_crtc->plane == 0) ? DSPABASE : DSPBBASE;
	/* Pipe must be off here */
	OUTREG(dspcntr_reg, dspcntr & ~DISPLAY_PLANE_ENABLE);
	/* Flush the plane changes */
	OUTREG(dspbase_reg, INREG(dspbase_reg));

	if (!IS_I9XX(pI830)) {
	    /* Wait for vblank for the disable to take effect */
	    i830WaitForVblank(pScrn);
	}

	OUTREG(pipeconf_reg, pipeconf & ~PIPEACONF_ENABLE);
	/* Wait for vblank for the disable to take effect. */
	i830WaitForVblank(pScrn);

	/* Filter ctl must be set before TV_WIN_SIZE */
	OUTREG(TV_FILTER_CTL_1, dev_priv->save_TV_FILTER_CTL_1);
	OUTREG(TV_FILTER_CTL_2, dev_priv->save_TV_FILTER_CTL_2);
	OUTREG(TV_FILTER_CTL_3, dev_priv->save_TV_FILTER_CTL_3);
	OUTREG(TV_WIN_POS, dev_priv->save_TV_WIN_POS);
	OUTREG(TV_WIN_SIZE, dev_priv->save_TV_WIN_SIZE);
	OUTREG(pipeconf_reg, pipeconf);
	OUTREG(dspcntr_reg, dspcntr);
	/* Flush the plane changes */
	OUTREG(dspbase_reg, INREG(dspbase_reg));
    }

    for (i = 0; i < 60; i++)
	OUTREG(TV_H_LUMA_0 + (i <<2), dev_priv->save_TV_H_LUMA[i]);
    for (i = 0; i < 60; i++)
	OUTREG(TV_H_CHROMA_0 + (i <<2), dev_priv->save_TV_H_CHROMA[i]);
    for (i = 0; i < 43; i++)
	OUTREG(TV_V_LUMA_0 + (i <<2), dev_priv->save_TV_V_LUMA[i]);
    for (i = 0; i < 43; i++)
	OUTREG(TV_V_CHROMA_0 + (i <<2), dev_priv->save_TV_V_CHROMA[i]);

    OUTREG(TV_DAC, dev_priv->save_TV_DAC);
    OUTREG(TV_CTL, dev_priv->save_TV_CTL);
    i830WaitForVblank(pScrn);
}

static const tv_mode_t *
i830_tv_mode_lookup (char *tv_format)
{
    int			    i;
    
    for (i = 0; i < sizeof(tv_modes) / sizeof (tv_modes[0]); i++) 
    {
	const tv_mode_t	*tv_mode = &tv_modes[i];

	if (xf86nameCompare (tv_format, tv_mode->name) == 0)
	    return tv_mode;
    }
    return NULL;
}

static const tv_mode_t *
i830_tv_mode_find (xf86OutputPtr output)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_tv_priv	    *dev_priv = intel_output->dev_priv;

    return i830_tv_mode_lookup (dev_priv->tv_format);
}

static int
i830_tv_mode_valid(xf86OutputPtr output, DisplayModePtr mode)
{
    const tv_mode_t	*tv_mode = i830_tv_mode_find (output);
    
    if (tv_mode && fabs (tv_mode->refresh - xf86ModeVRefresh (mode)) < 1.0)
	return MODE_OK;
    return MODE_CLOCK_RANGE;
}


static Bool
i830_tv_mode_fixup(xf86OutputPtr output, DisplayModePtr mode,
		DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr		pScrn = output->scrn;
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int			i;
    const tv_mode_t	*tv_mode = i830_tv_mode_find (output);

    if (!tv_mode)
	return FALSE;
    
    for (i = 0; i < xf86_config->num_output; i++) 
    {
	xf86OutputPtr other_output = xf86_config->output[i];

	if (other_output != output && other_output->crtc == output->crtc)
	    return FALSE;
    }

    adjusted_mode->Clock = tv_mode->clock;
    return TRUE;
}

static uint32_t
i830_float_to_csc (float fin)
{
    uint32_t exp;
    uint32_t mant;
    uint32_t ret;
    float f = fin;

    /* somehow the color conversion knows the signs of all the values */
    if (f < 0) f = -f;

    if (f >= 1)
    {
	exp = 0x7;
	mant = 1 << 8;
    }
    else
    {
	for (exp = 0; exp < 3 && f < 0.5; exp++)
	    f *= 2.0;
	mant = (f * (1 << 9) + 0.5);
	if (mant >= (1 << 9))
	    mant = (1 << 9) - 1;
    }
    ret = (exp << 9) | mant;
    return ret;
}

static uint16_t
i830_float_to_luma (float f)
{
    uint16_t ret;

    ret = (f * (1 << 9));
    return ret;
}

static void
i830_tv_mode_set(xf86OutputPtr output, DisplayModePtr mode,
		DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);
    xf86CrtcPtr	    crtc = output->crtc;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    I830CrtcPrivatePtr	    intel_crtc = crtc->driver_private;
    struct i830_tv_priv	    *dev_priv = intel_output->dev_priv;
    const tv_mode_t	    *tv_mode = i830_tv_mode_find (output);
    uint32_t		    tv_ctl;
    uint32_t		    hctl1, hctl2, hctl3;
    uint32_t		    vctl1, vctl2, vctl3, vctl4, vctl5, vctl6, vctl7;
    uint32_t		    scctl1, scctl2, scctl3;
    int			    i, j;
    const video_levels_t	*video_levels;
    const color_conversion_t	*color_conversion;
    Bool burst_ena;
    
    if (!tv_mode)
	return;	/* can't happen (mode_prepare prevents this) */
    
    tv_ctl = 0;

    switch (dev_priv->type) {
	default:
	case TV_TYPE_UNKNOWN:
	case TV_TYPE_COMPOSITE:
	    tv_ctl |= TV_ENC_OUTPUT_COMPOSITE;
	    video_levels = &tv_mode->composite_levels;
	    color_conversion = &tv_mode->composite_color;
	    burst_ena = tv_mode->burst_ena;
	    break;
	case TV_TYPE_COMPONENT:
	    tv_ctl |= TV_ENC_OUTPUT_COMPONENT;
	    video_levels = &component_level;
	    if (tv_mode->burst_ena)
		color_conversion = &sdtv_component_color;
	    else
		color_conversion = &hdtv_component_color;
	    burst_ena = FALSE;
	    break;
	case TV_TYPE_SVIDEO:
	    tv_ctl |= TV_ENC_OUTPUT_SVIDEO;
	    video_levels = &tv_mode->svideo_levels;
	    color_conversion = &tv_mode->svideo_color;
	    burst_ena = tv_mode->burst_ena;
	    break;
    }
    hctl1 = (tv_mode->hsync_end << TV_HSYNC_END_SHIFT) |
	(tv_mode->htotal << TV_HTOTAL_SHIFT);

    hctl2 = (tv_mode->hburst_start << 16) |
	(tv_mode->hburst_len << TV_HBURST_LEN_SHIFT);

    if (burst_ena)
	hctl2 |= TV_BURST_ENA;

    hctl3 = (tv_mode->hblank_start << TV_HBLANK_START_SHIFT) |
	(tv_mode->hblank_end << TV_HBLANK_END_SHIFT);

    vctl1 = (tv_mode->nbr_end << TV_NBR_END_SHIFT) |
	(tv_mode->vi_end_f1 << TV_VI_END_F1_SHIFT) |
	(tv_mode->vi_end_f2 << TV_VI_END_F2_SHIFT);

    vctl2 = (tv_mode->vsync_len << TV_VSYNC_LEN_SHIFT) |
	(tv_mode->vsync_start_f1 << TV_VSYNC_START_F1_SHIFT) |
	(tv_mode->vsync_start_f2 << TV_VSYNC_START_F2_SHIFT);

    vctl3 = (tv_mode->veq_len << TV_VEQ_LEN_SHIFT) |
	(tv_mode->veq_start_f1 << TV_VEQ_START_F1_SHIFT) |
	(tv_mode->veq_start_f2 << TV_VEQ_START_F2_SHIFT);

    if (tv_mode->veq_ena)
	vctl3 |= TV_EQUAL_ENA;

    vctl4 = (tv_mode->vburst_start_f1 << TV_VBURST_START_F1_SHIFT) |
	(tv_mode->vburst_end_f1 << TV_VBURST_END_F1_SHIFT);

    vctl5 = (tv_mode->vburst_start_f2 << TV_VBURST_START_F2_SHIFT) |
	(tv_mode->vburst_end_f2 << TV_VBURST_END_F2_SHIFT);

    vctl6 = (tv_mode->vburst_start_f3 << TV_VBURST_START_F3_SHIFT) |
	(tv_mode->vburst_end_f3 << TV_VBURST_END_F3_SHIFT);

    vctl7 = (tv_mode->vburst_start_f4 << TV_VBURST_START_F4_SHIFT) |
	(tv_mode->vburst_end_f4 << TV_VBURST_END_F4_SHIFT);

    if (intel_crtc->pipe == 1)
	tv_ctl |= TV_ENC_PIPEB_SELECT;
    tv_ctl |= tv_mode->oversample;

    if (tv_mode->progressive)
	tv_ctl |= TV_PROGRESSIVE;
    if (tv_mode->trilevel_sync)
	tv_ctl |= TV_TRILEVEL_SYNC;
    if (tv_mode->pal_burst)
	tv_ctl |= TV_PAL_BURST;
    scctl1 = 0;
    if (tv_mode->dda1_inc)
	scctl1 |= TV_SC_DDA1_EN;

    if (tv_mode->dda2_inc)
	scctl1 |= TV_SC_DDA2_EN;

    if (tv_mode->dda3_inc)
	scctl1 |= TV_SC_DDA3_EN;

    scctl1 |= tv_mode->sc_reset;
    scctl1 |= video_levels->burst << TV_BURST_LEVEL_SHIFT;
    scctl1 |= tv_mode->dda1_inc << TV_SCDDA1_INC_SHIFT;

    scctl2 = tv_mode->dda2_size << TV_SCDDA2_SIZE_SHIFT |
	tv_mode->dda2_inc << TV_SCDDA2_INC_SHIFT;

    scctl3 = tv_mode->dda3_size << TV_SCDDA3_SIZE_SHIFT |
	tv_mode->dda3_inc << TV_SCDDA3_INC_SHIFT;

    /* Enable two fixes for the chips that need them. */
    if (DEVICE_ID(pI830->PciInfo) < PCI_CHIP_I945_G)
	tv_ctl |= TV_ENC_C0_FIX | TV_ENC_SDP_FIX;

    OUTREG(TV_H_CTL_1, hctl1);
    OUTREG(TV_H_CTL_2, hctl2);
    OUTREG(TV_H_CTL_3, hctl3);
    OUTREG(TV_V_CTL_1, vctl1);
    OUTREG(TV_V_CTL_2, vctl2);
    OUTREG(TV_V_CTL_3, vctl3);
    OUTREG(TV_V_CTL_4, vctl4);
    OUTREG(TV_V_CTL_5, vctl5);
    OUTREG(TV_V_CTL_6, vctl6);
    OUTREG(TV_V_CTL_7, vctl7);
    OUTREG(TV_SC_CTL_1, scctl1);
    OUTREG(TV_SC_CTL_2, scctl2);
    OUTREG(TV_SC_CTL_3, scctl3);

    OUTREG(TV_CSC_Y,
	    (i830_float_to_csc(color_conversion->ry) << 16) |
	    (i830_float_to_csc(color_conversion->gy)));
    OUTREG(TV_CSC_Y2,
	    (i830_float_to_csc(color_conversion->by) << 16) |
	    (i830_float_to_luma(color_conversion->ay)));

    OUTREG(TV_CSC_U,
	    (i830_float_to_csc(color_conversion->ru) << 16) |
	    (i830_float_to_csc(color_conversion->gu)));

    OUTREG(TV_CSC_U2,
	    (i830_float_to_csc(color_conversion->bu) << 16) |
	    (i830_float_to_luma(color_conversion->au)));

    OUTREG(TV_CSC_V,
	    (i830_float_to_csc(color_conversion->rv) << 16) |
	    (i830_float_to_csc(color_conversion->gv)));

    OUTREG(TV_CSC_V2,
	    (i830_float_to_csc(color_conversion->bv) << 16) |
	    (i830_float_to_luma(color_conversion->av)));

    OUTREG(TV_CLR_KNOBS, 0x00606000);
    OUTREG(TV_CLR_LEVEL, ((video_levels->black << TV_BLACK_LEVEL_SHIFT) |
		(video_levels->blank << TV_BLANK_LEVEL_SHIFT)));
    {
	int pipeconf_reg = (intel_crtc->pipe == 0) ? PIPEACONF : PIPEBCONF;
	int dspcntr_reg = (intel_crtc->plane == 0) ? DSPACNTR : DSPBCNTR;
	int pipeconf = INREG(pipeconf_reg);
	int dspcntr = INREG(dspcntr_reg);
	int dspbase_reg = (intel_crtc->plane == 0) ? DSPABASE : DSPBBASE;
	int xpos = 0x0, ypos = 0x0;
	unsigned int xsize, ysize;
	/* Pipe must be off here */
	OUTREG(dspcntr_reg, dspcntr & ~DISPLAY_PLANE_ENABLE);
	/* Flush the plane changes */
	OUTREG(dspbase_reg, INREG(dspbase_reg));

	if (!IS_I9XX(pI830)) {
	    /* Wait for vblank for the disable to take effect */
	    i830WaitForVblank(pScrn);
	}

	OUTREG(pipeconf_reg, pipeconf & ~PIPEACONF_ENABLE);
	/* Wait for vblank for the disable to take effect. */
	i830WaitForVblank(pScrn);

	/* Filter ctl must be set before TV_WIN_SIZE */
	OUTREG(TV_FILTER_CTL_1, TV_AUTO_SCALE); 
	xsize = tv_mode->hblank_start - tv_mode->hblank_end;
	if (tv_mode->progressive)
	    ysize = tv_mode->nbr_end + 1;
	else
	    ysize = 2*tv_mode->nbr_end + 1;

	xpos += dev_priv->margin[TV_MARGIN_LEFT];
	ypos += dev_priv->margin[TV_MARGIN_TOP];
	xsize -= (dev_priv->margin[TV_MARGIN_LEFT] + 
		  dev_priv->margin[TV_MARGIN_RIGHT]);
	ysize -= (dev_priv->margin[TV_MARGIN_TOP] + 
		  dev_priv->margin[TV_MARGIN_BOTTOM]);
	OUTREG(TV_WIN_POS, (xpos<<16)|ypos);
	OUTREG(TV_WIN_SIZE, (xsize<<16)|ysize);

	OUTREG(pipeconf_reg, pipeconf);
	OUTREG(dspcntr_reg, dspcntr);
	/* Flush the plane changes */
	OUTREG(dspbase_reg, INREG(dspbase_reg));
    } 	

    j = 0;
    for (i = 0; i < 60; i++)
	OUTREG(TV_H_LUMA_0 + (i<<2), tv_mode->filter_table[j++]);
    for (i = 0; i < 60; i++)
	OUTREG(TV_H_CHROMA_0 + (i<<2), tv_mode->filter_table[j++]);
    for (i = 0; i < 43; i++)
	OUTREG(TV_V_LUMA_0 + (i<<2), tv_mode->filter_table[j++]);
    for (i = 0; i < 43; i++)
	OUTREG(TV_V_CHROMA_0 + (i<<2), tv_mode->filter_table[j++]);
    OUTREG(TV_DAC, 0);
    OUTREG(TV_CTL, tv_ctl);
    i830WaitForVblank(pScrn);
}

static const DisplayModeRec reported_modes[] = {
    {
	.name = "NTSC 480i",
	.Clock = 107520,
	.HDisplay   = 1280,
	.HSyncStart = 1368,
	.HSyncEnd   = 1496,
	.HTotal     = 1712,

	.VDisplay   = 1024,
	.VSyncStart = 1027,
	.VSyncEnd   = 1034,
	.VTotal     = 1104,
	.type       = M_T_DRIVER
    },
};

/**
 * Detects TV presence by checking for load.
 *
 * Requires that the current pipe's DPLL is active.

 * \return TRUE if TV is connected.
 * \return FALSE if TV is disconnected.
 */
static int
i830_tv_detect_type (xf86CrtcPtr    crtc,
		xf86OutputPtr  output)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    uint32_t		    tv_ctl, save_tv_ctl;
    uint32_t		    tv_dac, save_tv_dac;
    int			    type = TV_TYPE_UNKNOWN;

    tv_dac = INREG(TV_DAC);
    /*
     * Detect TV by polling)
     */
    if (intel_output->load_detect_temp)
    {
	/* TV not currently running, prod it with destructive detect */
	save_tv_dac = tv_dac;
	tv_ctl = INREG(TV_CTL);
	save_tv_ctl = tv_ctl;
	tv_ctl &= ~TV_ENC_ENABLE;
	tv_ctl &= ~TV_TEST_MODE_MASK;
	tv_ctl |= TV_TEST_MODE_MONITOR_DETECT;
	tv_dac &= ~TVDAC_SENSE_MASK;
	tv_dac |= (TVDAC_STATE_CHG_EN |
		TVDAC_A_SENSE_CTL |
		TVDAC_B_SENSE_CTL |
		TVDAC_C_SENSE_CTL |
		DAC_CTL_OVERRIDE |
		DAC_A_0_7_V |
		DAC_B_0_7_V |
		DAC_C_0_7_V);
	OUTREG(TV_CTL, tv_ctl);
	OUTREG(TV_DAC, tv_dac);
	i830WaitForVblank(pScrn);
	tv_dac = INREG(TV_DAC);
	OUTREG(TV_DAC, save_tv_dac);
	OUTREG(TV_CTL, save_tv_ctl);
	i830WaitForVblank(pScrn);
    }
    /*
     *  A B C
     *  0 1 1 Composite
     *  1 0 X svideo
     *  0 0 0 Component
     */
    if ((tv_dac & TVDAC_SENSE_MASK) == (TVDAC_B_SENSE | TVDAC_C_SENSE)) {
	if (pI830->debug_modes) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Detected Composite TV connection\n");
	}
	type = TV_TYPE_COMPOSITE;
    } else if ((tv_dac & (TVDAC_A_SENSE|TVDAC_B_SENSE)) == TVDAC_A_SENSE) {
	if (pI830->debug_modes) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Detected S-Video TV connection\n");
	}
	type = TV_TYPE_SVIDEO;
    } else if ((tv_dac & TVDAC_SENSE_MASK) == 0) {
	if (pI830->debug_modes) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Detected Component TV connection\n");
	}
	type = TV_TYPE_COMPONENT;
    } else {
	if (pI830->debug_modes) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "No TV connection detected\n");
	}
	type = TV_TYPE_NONE;
    }

    return type;
}

#ifdef RANDR_12_INTERFACE
static int
i830_tv_format_configure_property (xf86OutputPtr output);
#endif

/**
 * Detect the TV connection.
 *
 * Currently this always returns OUTPUT_STATUS_UNKNOWN, as we need to be sure
 * we have a pipe programmed in order to probe the TV.
 */
static xf86OutputStatus
i830_tv_detect(xf86OutputPtr output)
{
    xf86CrtcPtr		    crtc;
    DisplayModeRec	    mode;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_tv_priv	    *dev_priv = intel_output->dev_priv;
    int			    dpms_mode;
    int			    type = dev_priv->type;

    mode = reported_modes[0];
    xf86SetModeCrtc (&mode, INTERLACE_HALVE_V);
    crtc = i830GetLoadDetectPipe (output, &mode, &dpms_mode);
    if (crtc)
    {
        type = i830_tv_detect_type (crtc, output);
        i830ReleaseLoadDetectPipe (output, dpms_mode);
    }

    if (type != dev_priv->type)
    {
	dev_priv->type = type;
#ifdef RANDR_12_INTERFACE
	i830_tv_format_configure_property (output);
#endif
    }
	
    switch (type) {
    case TV_TYPE_NONE:
        return XF86OutputStatusDisconnected;
    case TV_TYPE_UNKNOWN:
        return XF86OutputStatusUnknown;
    default:
        return XF86OutputStatusConnected;
    }
}

static struct input_res {
    char *name;
    int w, h;	
} input_res_table[] = 
{
	{"640x480", 640, 480},
	{"800x600", 800, 600},
	{"1024x768", 1024, 768},
	{"1280x1024", 1280, 1024},
	{"848x480", 848, 480},
	{"1280x720", 1280, 720},
	{"1920x1080", 1920, 1080},
};

/**
 * Stub get_modes function.
 *
 * This should probably return a set of fixed modes, unless we can figure out
 * how to probe modes off of TV connections.
 */

static DisplayModePtr
i830_tv_get_modes(xf86OutputPtr output)
{
    DisplayModePtr	ret = NULL, mode_ptr;
    int			j;
    const tv_mode_t	*tv_mode = i830_tv_mode_find (output);

    for (j = 0; j < sizeof(input_res_table)/sizeof(input_res_table[0]); j++)
    {
	struct input_res *input = &input_res_table[j];
	unsigned int hactive_s = input->w;
	unsigned int vactive_s = input->h;
	
	if (tv_mode->max_srcw && input->w > tv_mode->max_srcw)
	    continue;

	if (input->w > 1024 && (!tv_mode->progressive 
				&& !tv_mode->component_only))
	    continue;

	mode_ptr = xnfcalloc(1, sizeof(DisplayModeRec));
    	mode_ptr->name = xnfalloc(strlen(input->name) + 1);
	strcpy (mode_ptr->name, input->name);

	mode_ptr->HDisplay = hactive_s;
	mode_ptr->HSyncStart = hactive_s + 1;
	mode_ptr->HSyncEnd = hactive_s + 64;
	if ( mode_ptr->HSyncEnd <= mode_ptr->HSyncStart)
	    mode_ptr->HSyncEnd = mode_ptr->HSyncStart  + 1;
	mode_ptr->HTotal = hactive_s + 96;

	mode_ptr->VDisplay = vactive_s;
	mode_ptr->VSyncStart = vactive_s + 1;
	mode_ptr->VSyncEnd = vactive_s + 32;
	if ( mode_ptr->VSyncEnd <= mode_ptr->VSyncStart)
	    mode_ptr->VSyncEnd = mode_ptr->VSyncStart  + 1;
	mode_ptr->VTotal = vactive_s + 33;

	mode_ptr->Clock = (int) (tv_mode->refresh * 
				 mode_ptr->VTotal * 
				 mode_ptr->HTotal / 1000.0);
	
	mode_ptr->type = M_T_DRIVER;
	mode_ptr->next = ret;
	mode_ptr->prev = NULL;
	if (ret != NULL)
	    ret->prev = mode_ptr;
	ret = mode_ptr;
    } 

    return ret;
}

static void
i830_tv_destroy (xf86OutputPtr output)
{
    if (output->driver_private)
	xfree (output->driver_private);
}

#ifdef RANDR_12_INTERFACE
#define TV_FORMAT_NAME	"TV_FORMAT"
static Atom tv_format_atom;
static Atom tv_format_name_atoms[NUM_TV_MODES];
static Atom margin_atoms[4];
static char *margin_names[4] = {
    "LEFT", "TOP", "RIGHT", "BOTTOM"
};

static Bool
i830_tv_format_set_property (xf86OutputPtr output)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_tv_priv	    *dev_priv = intel_output->dev_priv;
    const tv_mode_t	    *tv_mode = i830_tv_mode_lookup (dev_priv->tv_format);
    int			    err;

    if (!tv_mode)
	tv_mode = &tv_modes[0];
    err = RRChangeOutputProperty (output->randr_output, tv_format_atom,
				  XA_ATOM, 32, PropModeReplace, 1,
				  &tv_format_name_atoms[tv_mode - tv_modes],
				  FALSE, TRUE);
    return err == Success;
}

    
/**
 * Configure the TV_FORMAT property to list only supported formats
 *
 * Unless the connector is component, list only the formats supported by
 * svideo and composite
 */

static int
i830_tv_format_configure_property (xf86OutputPtr output)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_tv_priv	    *dev_priv = intel_output->dev_priv;
    Atom		    current_atoms[NUM_TV_MODES];
    int			    num_atoms = 0;
    int			    i;
    
    if (!output->randr_output)
	return Success;

    for (i = 0; i < NUM_TV_MODES; i++)
	if (!tv_modes[i].component_only || dev_priv->type == TV_TYPE_COMPONENT)
	    current_atoms[num_atoms++] = tv_format_name_atoms[i];
    
    return RRConfigureOutputProperty(output->randr_output, tv_format_atom,
				     TRUE, FALSE, FALSE, 
				     num_atoms, (INT32 *) current_atoms);
}

#endif /* RANDR_12_INTERFACE */

static void
i830_tv_create_resources(xf86OutputPtr output)
{
#ifdef RANDR_12_INTERFACE
    ScrnInfoPtr		    pScrn = output->scrn;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_tv_priv	    *dev_priv = intel_output->dev_priv;
    int			    err;
    int			    i;

    /* Set up the tv_format property, which takes effect on mode set
     * and accepts strings that match exactly
     */
    tv_format_atom = MakeAtom(TV_FORMAT_NAME, sizeof(TV_FORMAT_NAME) - 1,
	TRUE);

    for (i = 0; i < NUM_TV_MODES; i++)
	tv_format_name_atoms[i] = MakeAtom (tv_modes[i].name,
					    strlen (tv_modes[i].name),
					    TRUE);

    err = i830_tv_format_configure_property (output);

    if (err != 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "RRConfigureOutputProperty error, %d\n", err);
    }

    /* Set the current value of the tv_format property */
    if (!i830_tv_format_set_property (output))
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "RRChangeOutputProperty error, %d\n", err);

    for (i = 0; i < 4; i++)
    {
	INT32	range[2];
	margin_atoms[i] = MakeAtom(margin_names[i], strlen (margin_names[i]),
				   TRUE);

	range[0] = 0;
	range[1] = 100;
	err = RRConfigureOutputProperty(output->randr_output, margin_atoms[i],
				    TRUE, TRUE, FALSE, 2, range);
    
	if (err != 0)
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRConfigureOutputProperty error, %d\n", err);

	err = RRChangeOutputProperty(output->randr_output, margin_atoms[i],
				     XA_INTEGER, 32, PropModeReplace,
				     1, &dev_priv->margin[i],
				     FALSE, TRUE);
	if (err != 0)
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", err);
    }
#endif /* RANDR_12_INTERFACE */
}

#ifdef RANDR_12_INTERFACE
static Bool
i830_tv_set_property(xf86OutputPtr output, Atom property,
		       RRPropertyValuePtr value)
{
    int	i;
    
    if (property == tv_format_atom) 
    {
	I830OutputPrivatePtr    intel_output = output->driver_private;
	struct i830_tv_priv	*dev_priv = intel_output->dev_priv;
	Atom			atom;
	char			*name;
	char			*val;

	if (value->type != XA_ATOM || value->format != 32 || value->size != 1)
	    return FALSE;

	memcpy (&atom, value->data, 4);
	name = NameForAtom (atom);
	
	val = xalloc (strlen (name) + 1);
	if (!val)
	    return FALSE;
	strcpy (val, name);
	if (!i830_tv_mode_lookup (val))
	{
	    xfree (val);
	    return FALSE;
	}
	xfree (dev_priv->tv_format);
	dev_priv->tv_format = val;
	return TRUE;
    }
    for (i = 0; i < 4; i++)
    {
	if (property == margin_atoms[i])
	{
	    I830OutputPrivatePtr    intel_output = output->driver_private;
	    struct i830_tv_priv	*dev_priv = intel_output->dev_priv;
	    INT32		val;

	    if (value->type != XA_INTEGER || value->format != 32 ||
		value->size != 1)
		return FALSE;

	    memcpy (&val, value->data, 4);
	    dev_priv->margin[i] = val;
	    return TRUE;
	}
    }

    return TRUE;
}
#endif /* RANDR_12_INTERFACE */

#ifdef RANDR_GET_CRTC_INTERFACE
static xf86CrtcPtr
i830_tv_get_crtc(xf86OutputPtr output)
{
    ScrnInfoPtr	pScrn = output->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    int pipe = !!(INREG(TV_CTL) & TV_ENC_PIPEB_SELECT);
   
    return i830_pipe_to_crtc(pScrn, pipe);
}
#endif

static const xf86OutputFuncsRec i830_tv_output_funcs = {
    .create_resources = i830_tv_create_resources,
    .dpms = i830_tv_dpms,
    .save = i830_tv_save,
    .restore = i830_tv_restore,
    .mode_valid = i830_tv_mode_valid,
    .mode_fixup = i830_tv_mode_fixup,
    .prepare = i830_output_prepare,
    .mode_set = i830_tv_mode_set,
    .commit = i830_output_commit,
    .detect = i830_tv_detect,
    .get_modes = i830_tv_get_modes,
    .destroy = i830_tv_destroy,
#ifdef RANDR_12_INTERFACE
    .set_property = i830_tv_set_property,
#endif
#ifdef RANDR_GET_CRTC_INTERFACE
    .get_crtc = i830_tv_get_crtc,
#endif
};

void
i830_tv_init(ScrnInfoPtr pScrn)
{
    I830Ptr		    pI830 = I830PTR(pScrn);
    xf86OutputPtr	    output;
    I830OutputPrivatePtr    intel_output;
    struct i830_tv_priv	    *dev_priv;
    uint32_t		    tv_dac_on, tv_dac_off, save_tv_dac;

    if (pI830->quirk_flag & QUIRK_IGNORE_TV)
	return;

    if ((INREG(TV_CTL) & TV_FUSE_STATE_MASK) == TV_FUSE_STATE_DISABLED)
	return;

    /*
     * Sanity check the TV output by checking to see if the
     * DAC register holds a value
     */
    save_tv_dac = INREG(TV_DAC);

    OUTREG(TV_DAC, save_tv_dac | TVDAC_STATE_CHG_EN);
    tv_dac_on = INREG(TV_DAC);

    OUTREG(TV_DAC, save_tv_dac & ~TVDAC_STATE_CHG_EN);
    tv_dac_off = INREG(TV_DAC);

    OUTREG(TV_DAC, save_tv_dac);

    /*
     * If the register does not hold the state change enable
     * bit, (either as a 0 or a 1), assume it doesn't really
     * exist
     */
    if ((tv_dac_on & TVDAC_STATE_CHG_EN) == 0 || 
	    (tv_dac_off & TVDAC_STATE_CHG_EN) != 0)
	return;

    output = xf86OutputCreate (pScrn, &i830_tv_output_funcs, "TV");

    if (!output)
	return;

    intel_output = xnfcalloc (sizeof (I830OutputPrivateRec) +
	    sizeof (struct i830_tv_priv), 1);
    if (!intel_output)
    {
	xf86OutputDestroy (output);
	return;
    }
    dev_priv = (struct i830_tv_priv *) (intel_output + 1);
    intel_output->type = I830_OUTPUT_TVOUT;
    intel_output->pipe_mask = ((1 << 0) | (1 << 1));
    intel_output->clone_mask = (1 << I830_OUTPUT_TVOUT);
    intel_output->dev_priv = dev_priv;
    dev_priv->type = TV_TYPE_UNKNOWN;

    dev_priv->tv_format = NULL;
    
    /* BIOS margin values */
    dev_priv->margin[TV_MARGIN_LEFT] = 54;
    dev_priv->margin[TV_MARGIN_TOP] = 36;
    dev_priv->margin[TV_MARGIN_RIGHT] = 46;
    dev_priv->margin[TV_MARGIN_BOTTOM] = 37;
    
    if (output->conf_monitor)
    {
	char	*tv_format;
	
	tv_format = xf86findOptionValue (output->conf_monitor->mon_option_lst, "TV Format");
	if (tv_format)
	    dev_priv->tv_format = xstrdup (tv_format);
    }
    if (!dev_priv->tv_format)
	dev_priv->tv_format = xstrdup (tv_modes[0].name);
    
    output->driver_private = intel_output;
    output->interlaceAllowed = FALSE;
    output->doubleScanAllowed = FALSE;
}
