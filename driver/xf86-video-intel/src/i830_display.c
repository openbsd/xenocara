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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <sys/ioctl.h>

#include "xf86.h"
#include "intel.h"
#include "i830_bios.h"
#include "i830_display.h"
#include "xf86Modes.h"
#include "i830_reg.h"

typedef struct {
    /* given values */    
    int n;
    int m1, m2;
    int p1, p2;
    /* derived values */
    int	dot;
    int	vco;
    int	m;
    int	p;
} intel_clock_t;

typedef struct {
    int	min, max;
} intel_range_t;

typedef struct {
    int	dot_limit;
    int	p2_slow, p2_fast;
} intel_p2_t;

#define INTEL_P2_NUM		      2

typedef struct intel_limit intel_limit_t;
struct intel_limit {
    intel_range_t   dot, vco, n, m, m1, m2, p, p1;
    intel_p2_t	    p2;
    Bool (* find_pll)(const intel_limit_t *, xf86CrtcPtr,
                      int, int, intel_clock_t *);
};

#define I8XX_DOT_MIN		  25000
#define I8XX_DOT_MAX		 350000
#define I8XX_VCO_MIN		 930000
#define I8XX_VCO_MAX		1400000
#define I8XX_N_MIN		      3
#define I8XX_N_MAX		     16
#define I8XX_M_MIN		     96
#define I8XX_M_MAX		    140
#define I8XX_M1_MIN		     18
#define I8XX_M1_MAX		     26
#define I8XX_M2_MIN		      6
#define I8XX_M2_MAX		     16
#define I8XX_P_MIN		      4
#define I8XX_P_MAX		    128
#define I8XX_P1_MIN		      2
#define I8XX_P1_MAX		     33
#define I8XX_P1_LVDS_MIN	      1
#define I8XX_P1_LVDS_MAX	      6
#define I8XX_P2_SLOW		      4
#define I8XX_P2_FAST		      2
#define I8XX_P2_LVDS_SLOW	      14
#define I8XX_P2_LVDS_FAST	      7
#define I8XX_P2_SLOW_LIMIT	 165000

#define I9XX_DOT_MIN		  20000
#define I9XX_DOT_MAX		 400000
#define I9XX_VCO_MIN		1400000
#define I9XX_VCO_MAX		2800000
#define IGD_VCO_MIN		1700000
#define IGD_VCO_MAX		3500000

/* Haven't found any reason to go this fast, but newer chips support it */
#define I96X_VCO_MAX		3200000

/*
 * These values are taken from the broadwater/crestline PLL spreadsheet.
 * All of the defines here are for the programmed register value, not
 * the 'counter' value (e.g. Ncounter = Nregister + 2)
 */
#define I9XX_N_MIN		      1
#define I9XX_N_MAX		      6
/* IGD's Ncounter is a ring counter */
#define IGD_N_MIN		      3
#define IGD_N_MAX		      6
#define I9XX_M_MIN		     70
#define I9XX_M_MAX		    120
#define IGD_M_MIN		      2
#define IGD_M_MAX		    256

/* these two come from the calm1 macro */
#define I9XX_M1_MIN		     10
#define I9XX_M1_MAX		     22
#define I9XX_M2_MIN		      5
#define I9XX_M2_MAX		      9
/* IGD M1 is reserved, and must be 0 */
#define IGD_M1_MIN		      0
#define IGD_M1_MAX		      0
#define IGD_M2_MIN		      0
#define IGD_M2_MAX		      254

#define I9XX_P_SDVO_DAC_MIN	      5
#define I9XX_P_SDVO_DAC_MAX	     80
#define I9XX_P_LVDS_MIN		      7
#define I9XX_P_LVDS_MAX		     98
#define IGD_P_LVDS_MIN		      7
#define IGD_P_LVDS_MAX		     112
#define I9XX_P1_MIN		      1
#define I9XX_P1_MAX		      8
#define I9XX_P2_SDVO_DAC_SLOW		     10
#define I9XX_P2_SDVO_DAC_FAST		      5
#define I9XX_P2_SDVO_DAC_SLOW_LIMIT	 200000
#define I9XX_P2_LVDS_SLOW		     14
#define I9XX_P2_LVDS_FAST		      7
#define I9XX_P2_LVDS_SLOW_LIMIT		 112000

#define IRONLAKE_P2_DOT_LIMIT    225000 /* 225Mhz */
#define IRONLAKE_DOT_MIN         25000
#define IRONLAKE_DOT_MAX         350000
#define IRONLAKE_VCO_MIN         1760000
#define IRONLAKE_VCO_MAX         3510000
#define IRONLAKE_M1_MIN          12
#define IRONLAKE_M1_MAX          22
#define IRONLAKE_M2_MIN          5
#define IRONLAKE_M2_MAX          9

#define INTEL_LIMIT_I8XX_DVO_DAC    0
#define INTEL_LIMIT_I8XX_LVDS	    1
#define INTEL_LIMIT_I9XX_SDVO_DAC   2
#define INTEL_LIMIT_I9XX_LVDS	    3
#define INTEL_LIMIT_IGD_SDVO_DAC    4
#define INTEL_LIMIT_IGD_LVDS	    5
#define INTEL_LIMIT_G4X_SDVO	    6
#define INTEL_LIMIT_G4X_HDMI_DAC    7
#define INTEL_LIMIT_G4X_SINGLE_LVDS 8
#define INTEL_LIMIT_G4X_DUAL_LVDS   9

/*The parameter is for SDVO on G4x platform*/
#define G4X_VCO_MIN                1750000
#define G4X_VCO_MAX                3500000
#define G4X_DOT_SDVO_MIN           25000
#define G4X_DOT_SDVO_MAX           270000
#define G4X_N_SDVO_MIN             1
#define G4X_N_SDVO_MAX             4
#define G4X_M_SDVO_MIN             104
#define G4X_M_SDVO_MAX             138
#define G4X_M1_SDVO_MIN            17
#define G4X_M1_SDVO_MAX            23
#define G4X_M2_SDVO_MIN            5
#define G4X_M2_SDVO_MAX            11
#define G4X_P_SDVO_MIN             10
#define G4X_P_SDVO_MAX             30
#define G4X_P1_SDVO_MIN            1
#define G4X_P1_SDVO_MAX            3
#define G4X_P2_SDVO_SLOW           10
#define G4X_P2_SDVO_FAST           10
#define G4X_P2_SDVO_LIMIT          270000

/*The parameter is for HDMI_DAC on G4x platform*/
#define G4X_DOT_HDMI_DAC_MIN           22000
#define G4X_DOT_HDMI_DAC_MAX           400000
#define G4X_N_HDMI_DAC_MIN             1
#define G4X_N_HDMI_DAC_MAX             4
#define G4X_M_HDMI_DAC_MIN             104
#define G4X_M_HDMI_DAC_MAX             138
#define G4X_M1_HDMI_DAC_MIN            16
#define G4X_M1_HDMI_DAC_MAX            23
#define G4X_M2_HDMI_DAC_MIN            5
#define G4X_M2_HDMI_DAC_MAX            11
#define G4X_P_HDMI_DAC_MIN             5
#define G4X_P_HDMI_DAC_MAX             80
#define G4X_P1_HDMI_DAC_MIN            1
#define G4X_P1_HDMI_DAC_MAX            8
#define G4X_P2_HDMI_DAC_SLOW           10
#define G4X_P2_HDMI_DAC_FAST           5
#define G4X_P2_HDMI_DAC_LIMIT          165000

/*The parameter is for SINGLE_LVDS on G4x platform*/
#define G4X_DOT_SINGLE_LVDS_MIN           20000
#define G4X_DOT_SINGLE_LVDS_MAX           115000
#define G4X_N_SINGLE_LVDS_MIN             1
#define G4X_N_SINGLE_LVDS_MAX             3
#define G4X_M_SINGLE_LVDS_MIN             104
#define G4X_M_SINGLE_LVDS_MAX             138
#define G4X_M1_SINGLE_LVDS_MIN            17
#define G4X_M1_SINGLE_LVDS_MAX            23
#define G4X_M2_SINGLE_LVDS_MIN            5
#define G4X_M2_SINGLE_LVDS_MAX            11
#define G4X_P_SINGLE_LVDS_MIN             28
#define G4X_P_SINGLE_LVDS_MAX             112
#define G4X_P1_SINGLE_LVDS_MIN            2
#define G4X_P1_SINGLE_LVDS_MAX            8
#define G4X_P2_SINGLE_LVDS_SLOW           14
#define G4X_P2_SINGLE_LVDS_FAST           14
#define G4X_P2_SINGLE_LVDS_LIMIT          0

/*The parameter is for DUAL_LVDS on G4x platform*/
#define G4X_DOT_DUAL_LVDS_MIN           80000
#define G4X_DOT_DUAL_LVDS_MAX           224000
#define G4X_N_DUAL_LVDS_MIN             1
#define G4X_N_DUAL_LVDS_MAX             3
#define G4X_M_DUAL_LVDS_MIN             104
#define G4X_M_DUAL_LVDS_MAX             138
#define G4X_M1_DUAL_LVDS_MIN            17
#define G4X_M1_DUAL_LVDS_MAX            23
#define G4X_M2_DUAL_LVDS_MIN            5
#define G4X_M2_DUAL_LVDS_MAX            11
#define G4X_P_DUAL_LVDS_MIN             14
#define G4X_P_DUAL_LVDS_MAX             42
#define G4X_P1_DUAL_LVDS_MIN            2
#define G4X_P1_DUAL_LVDS_MAX            6
#define G4X_P2_DUAL_LVDS_SLOW           7
#define G4X_P2_DUAL_LVDS_FAST           7
#define G4X_P2_DUAL_LVDS_LIMIT          0

/* DAC & HDMI Refclk 120Mhz */
#define IRONLAKE_DAC_N_MIN     1
#define IRONLAKE_DAC_N_MAX     5
#define IRONLAKE_DAC_M_MIN     79
#define IRONLAKE_DAC_M_MAX     127
#define IRONLAKE_DAC_P_MIN     5
#define IRONLAKE_DAC_P_MAX     80
#define IRONLAKE_DAC_P1_MIN    1
#define IRONLAKE_DAC_P1_MAX    8
#define IRONLAKE_DAC_P2_SLOW   10
#define IRONLAKE_DAC_P2_FAST   5

/* LVDS single-channel 120Mhz refclk */
#define IRONLAKE_LVDS_S_N_MIN  1
#define IRONLAKE_LVDS_S_N_MAX  3
#define IRONLAKE_LVDS_S_M_MIN  79
#define IRONLAKE_LVDS_S_M_MAX  118
#define IRONLAKE_LVDS_S_P_MIN  28
#define IRONLAKE_LVDS_S_P_MAX  112
#define IRONLAKE_LVDS_S_P1_MIN 2
#define IRONLAKE_LVDS_S_P1_MAX 8
#define IRONLAKE_LVDS_S_P2_SLOW        14
#define IRONLAKE_LVDS_S_P2_FAST        14

/* LVDS dual-channel 120Mhz refclk */
#define IRONLAKE_LVDS_D_N_MIN  1
#define IRONLAKE_LVDS_D_N_MAX  3
#define IRONLAKE_LVDS_D_M_MIN  79
#define IRONLAKE_LVDS_D_M_MAX  127
#define IRONLAKE_LVDS_D_P_MIN  14
#define IRONLAKE_LVDS_D_P_MAX  56
#define IRONLAKE_LVDS_D_P1_MIN 2
#define IRONLAKE_LVDS_D_P1_MAX 8
#define IRONLAKE_LVDS_D_P2_SLOW        7
#define IRONLAKE_LVDS_D_P2_FAST        7

/* LVDS single-channel 100Mhz refclk */
#define IRONLAKE_LVDS_S_SSC_N_MIN      1
#define IRONLAKE_LVDS_S_SSC_N_MAX      2
#define IRONLAKE_LVDS_S_SSC_M_MIN      79
#define IRONLAKE_LVDS_S_SSC_M_MAX      126
#define IRONLAKE_LVDS_S_SSC_P_MIN      28
#define IRONLAKE_LVDS_S_SSC_P_MAX      112
#define IRONLAKE_LVDS_S_SSC_P1_MIN     2
#define IRONLAKE_LVDS_S_SSC_P1_MAX     8
#define IRONLAKE_LVDS_S_SSC_P2_SLOW    14
#define IRONLAKE_LVDS_S_SSC_P2_FAST    14

/* LVDS dual-channel 100Mhz refclk */
#define IRONLAKE_LVDS_D_SSC_N_MIN      1
#define IRONLAKE_LVDS_D_SSC_N_MAX      3
#define IRONLAKE_LVDS_D_SSC_M_MIN      79
#define IRONLAKE_LVDS_D_SSC_M_MAX      126
#define IRONLAKE_LVDS_D_SSC_P_MIN      14
#define IRONLAKE_LVDS_D_SSC_P_MAX      42
#define IRONLAKE_LVDS_D_SSC_P1_MIN     2
#define IRONLAKE_LVDS_D_SSC_P1_MAX     6
#define IRONLAKE_LVDS_D_SSC_P2_SLOW    7
#define IRONLAKE_LVDS_D_SSC_P2_FAST    7

static Bool
intel_find_pll_i8xx_and_i9xx(const intel_limit_t *, xf86CrtcPtr,
                             int, int, intel_clock_t *);
static Bool
intel_find_pll_g4x(const intel_limit_t *, xf86CrtcPtr,
                   int, int, intel_clock_t *);
static Bool
intel_igdng_find_best_PLL(const intel_limit_t *, xf86CrtcPtr,
			  int, int, intel_clock_t *);
static void
i830_crtc_load_lut(xf86CrtcPtr crtc);

static const intel_limit_t intel_limits[] = {
    { /* INTEL_LIMIT_I8XX_DVO_DAC */
        .dot = { .min = I8XX_DOT_MIN,		.max = I8XX_DOT_MAX },
        .vco = { .min = I8XX_VCO_MIN,		.max = I8XX_VCO_MAX },
        .n   = { .min = I8XX_N_MIN,		.max = I8XX_N_MAX },
        .m   = { .min = I8XX_M_MIN,		.max = I8XX_M_MAX },
        .m1  = { .min = I8XX_M1_MIN,		.max = I8XX_M1_MAX },
        .m2  = { .min = I8XX_M2_MIN,		.max = I8XX_M2_MAX },
        .p   = { .min = I8XX_P_MIN,		.max = I8XX_P_MAX },
        .p1  = { .min = I8XX_P1_MIN,		.max = I8XX_P1_MAX },
	.p2  = { .dot_limit = I8XX_P2_SLOW_LIMIT,
		 .p2_slow = I8XX_P2_SLOW,	.p2_fast = I8XX_P2_FAST },
        .find_pll = intel_find_pll_i8xx_and_i9xx,
    },
    { /* INTEL_LIMIT_I8XX_LVDS */
        .dot = { .min = I8XX_DOT_MIN,		.max = I8XX_DOT_MAX },
        .vco = { .min = I8XX_VCO_MIN,		.max = I8XX_VCO_MAX },
        .n   = { .min = I8XX_N_MIN,		.max = I8XX_N_MAX },
        .m   = { .min = I8XX_M_MIN,		.max = I8XX_M_MAX },
        .m1  = { .min = I8XX_M1_MIN,		.max = I8XX_M1_MAX },
        .m2  = { .min = I8XX_M2_MIN,		.max = I8XX_M2_MAX },
        .p   = { .min = I8XX_P_MIN,		.max = I8XX_P_MAX },
        .p1  = { .min = I8XX_P1_LVDS_MIN,	.max = I8XX_P1_LVDS_MAX },
	.p2  = { .dot_limit = I8XX_P2_SLOW_LIMIT,
		 .p2_slow = I8XX_P2_LVDS_SLOW,	.p2_fast = I8XX_P2_LVDS_FAST },
        .find_pll = intel_find_pll_i8xx_and_i9xx,
    },
    { /* INTEL_LIMIT_I9XX_SDVO_DAC */
        .dot = { .min = I9XX_DOT_MIN,		.max = I9XX_DOT_MAX },
        .vco = { .min = I9XX_VCO_MIN,		.max = I9XX_VCO_MAX },
        .n   = { .min = I9XX_N_MIN,		.max = I9XX_N_MAX },
        .m   = { .min = I9XX_M_MIN,		.max = I9XX_M_MAX },
        .m1  = { .min = I9XX_M1_MIN,		.max = I9XX_M1_MAX },
        .m2  = { .min = I9XX_M2_MIN,		.max = I9XX_M2_MAX },
        .p   = { .min = I9XX_P_SDVO_DAC_MIN,	.max = I9XX_P_SDVO_DAC_MAX },
        .p1  = { .min = I9XX_P1_MIN,		.max = I9XX_P1_MAX },
	.p2  = { .dot_limit = I9XX_P2_SDVO_DAC_SLOW_LIMIT,
		 .p2_slow = I9XX_P2_SDVO_DAC_SLOW,	.p2_fast = I9XX_P2_SDVO_DAC_FAST },
        .find_pll = intel_find_pll_i8xx_and_i9xx,
    },
    { /* INTEL_LIMIT_I9XX_LVDS */
        .dot = { .min = I9XX_DOT_MIN,		.max = I9XX_DOT_MAX },
        .vco = { .min = I9XX_VCO_MIN,		.max = I9XX_VCO_MAX },
        .n   = { .min = I9XX_N_MIN,		.max = I9XX_N_MAX },
        .m   = { .min = I9XX_M_MIN,		.max = I9XX_M_MAX },
        .m1  = { .min = I9XX_M1_MIN,		.max = I9XX_M1_MAX },
        .m2  = { .min = I9XX_M2_MIN,		.max = I9XX_M2_MAX },
        .p   = { .min = I9XX_P_LVDS_MIN,	.max = I9XX_P_LVDS_MAX },
        .p1  = { .min = I9XX_P1_MIN,		.max = I9XX_P1_MAX },
	/* The single-channel range is 25-112Mhz, and dual-channel
	 * is 80-224Mhz.  Prefer single channel as much as possible.
	 */
	.p2  = { .dot_limit = I9XX_P2_LVDS_SLOW_LIMIT,
		 .p2_slow = I9XX_P2_LVDS_SLOW,	.p2_fast = I9XX_P2_LVDS_FAST },
        .find_pll = intel_find_pll_i8xx_and_i9xx,
    },
    { /* INTEL_LIMIT_IGD_SDVO */
        .dot = { .min = I9XX_DOT_MIN,		.max = I9XX_DOT_MAX},
        .vco = { .min = IGD_VCO_MIN,		.max = IGD_VCO_MAX },
        .n   = { .min = IGD_N_MIN,		.max = IGD_N_MAX },
        .m   = { .min = IGD_M_MIN,		.max = IGD_M_MAX },
        .m1  = { .min = IGD_M1_MIN,		.max = IGD_M1_MAX },
        .m2  = { .min = IGD_M2_MIN,		.max = IGD_M2_MAX },
        .p   = { .min = I9XX_P_SDVO_DAC_MIN,    .max = I9XX_P_SDVO_DAC_MAX },
        .p1  = { .min = I9XX_P1_MIN,		.max = I9XX_P1_MAX },
	.p2  = { .dot_limit = I9XX_P2_SDVO_DAC_SLOW_LIMIT,
		 .p2_slow = I9XX_P2_SDVO_DAC_SLOW,	.p2_fast = I9XX_P2_SDVO_DAC_FAST },
        .find_pll = intel_find_pll_i8xx_and_i9xx,
    },
    { /* INTEL_LIMIT_IGD_LVDS */
        .dot = { .min = I9XX_DOT_MIN,		.max = I9XX_DOT_MAX },
        .vco = { .min = IGD_VCO_MIN,		.max = IGD_VCO_MAX },
        .n   = { .min = IGD_N_MIN,		.max = IGD_N_MAX },
        .m   = { .min = IGD_M_MIN,		.max = IGD_M_MAX },
        .m1  = { .min = IGD_M1_MIN,		.max = IGD_M1_MAX },
        .m2  = { .min = IGD_M2_MIN,		.max = IGD_M2_MAX },
        .p   = { .min = IGD_P_LVDS_MIN,	.max = IGD_P_LVDS_MAX },
        .p1  = { .min = I9XX_P1_MIN,		.max = I9XX_P1_MAX },
	/* IGD only supports single-channel mode. */
	.p2  = { .dot_limit = I9XX_P2_LVDS_SLOW_LIMIT,
		 .p2_slow = I9XX_P2_LVDS_SLOW,	.p2_fast = I9XX_P2_LVDS_SLOW },
        .find_pll = intel_find_pll_i8xx_and_i9xx,
    },
    /* below parameter and function is for G4X Chipset Family*/
    {   /* INTEL_LIMIT_G4X_SDVO */
        .dot = { .min = G4X_DOT_SDVO_MIN,       .max = G4X_DOT_SDVO_MAX },
        .vco = { .min = G4X_VCO_MIN,            .max = G4X_VCO_MAX},
        .n   = { .min = G4X_N_SDVO_MIN,         .max = G4X_N_SDVO_MAX },
        .m   = { .min = G4X_M_SDVO_MIN,         .max = G4X_M_SDVO_MAX },
        .m1  = { .min = G4X_M1_SDVO_MIN,        .max = G4X_M1_SDVO_MAX },
        .m2  = { .min = G4X_M2_SDVO_MIN,        .max = G4X_M2_SDVO_MAX },
        .p   = { .min = G4X_P_SDVO_MIN,         .max = G4X_P_SDVO_MAX },
        .p1  = { .min = G4X_P1_SDVO_MIN,        .max = G4X_P1_SDVO_MAX},
        .p2  = { .dot_limit = G4X_P2_SDVO_LIMIT,
                 .p2_slow = G4X_P2_SDVO_SLOW,
                 .p2_fast = G4X_P2_SDVO_FAST },
        .find_pll = intel_find_pll_g4x,
    },
    {   /* INTEL_LIMIT_G4X_HDMI_DAC */
        .dot = { .min = G4X_DOT_HDMI_DAC_MIN,	.max = G4X_DOT_HDMI_DAC_MAX },
        .vco = { .min = G4X_VCO_MIN,	        .max = G4X_VCO_MAX},
        .n   = { .min = G4X_N_HDMI_DAC_MIN,	.max = G4X_N_HDMI_DAC_MAX },
        .m   = { .min = G4X_M_HDMI_DAC_MIN,	.max = G4X_M_HDMI_DAC_MAX },
        .m1  = { .min = G4X_M1_HDMI_DAC_MIN,	.max = G4X_M1_HDMI_DAC_MAX },
        .m2  = { .min = G4X_M2_HDMI_DAC_MIN,	.max = G4X_M2_HDMI_DAC_MAX },
        .p   = { .min = G4X_P_HDMI_DAC_MIN,	.max = G4X_P_HDMI_DAC_MAX },
        .p1  = { .min = G4X_P1_HDMI_DAC_MIN,	.max = G4X_P1_HDMI_DAC_MAX},
        .p2  = { .dot_limit = G4X_P2_HDMI_DAC_LIMIT,
                 .p2_slow = G4X_P2_HDMI_DAC_SLOW,
                 .p2_fast = G4X_P2_HDMI_DAC_FAST },
        .find_pll = intel_find_pll_g4x,
    },
    {   /* INTEL_LIMIT_G4X_SINGLE_LVDS */
        .dot = { .min = G4X_DOT_SINGLE_LVDS_MIN,
                 .max = G4X_DOT_SINGLE_LVDS_MAX },
        .vco = { .min = G4X_VCO_MIN,
                 .max = G4X_VCO_MAX },
        .n   = { .min = G4X_N_SINGLE_LVDS_MIN,
                 .max = G4X_N_SINGLE_LVDS_MAX },
        .m   = { .min = G4X_M_SINGLE_LVDS_MIN,
                 .max = G4X_M_SINGLE_LVDS_MAX },
        .m1  = { .min = G4X_M1_SINGLE_LVDS_MIN,
                 .max = G4X_M1_SINGLE_LVDS_MAX },
        .m2  = { .min = G4X_M2_SINGLE_LVDS_MIN,
                 .max = G4X_M2_SINGLE_LVDS_MAX },
        .p   = { .min = G4X_P_SINGLE_LVDS_MIN,
                 .max = G4X_P_SINGLE_LVDS_MAX },
        .p1  = { .min = G4X_P1_SINGLE_LVDS_MIN,
                 .max = G4X_P1_SINGLE_LVDS_MAX },
        .p2  = { .dot_limit = G4X_P2_SINGLE_LVDS_LIMIT,
                 .p2_slow = G4X_P2_SINGLE_LVDS_SLOW,
                 .p2_fast = G4X_P2_SINGLE_LVDS_FAST },
        .find_pll = intel_find_pll_g4x,
    },
    {   /* INTEL_LIMIT_G4X_DUAL_LVDS */
        .dot = { .min = G4X_DOT_DUAL_LVDS_MIN,
                 .max = G4X_DOT_DUAL_LVDS_MAX },
        .vco = { .min = G4X_VCO_MIN,
                 .max = G4X_VCO_MAX},
        .n   = { .min = G4X_N_DUAL_LVDS_MIN,
                 .max = G4X_N_DUAL_LVDS_MAX },
        .m   = { .min = G4X_M_DUAL_LVDS_MIN,
                 .max = G4X_M_DUAL_LVDS_MAX },
        .m1  = { .min = G4X_M1_DUAL_LVDS_MIN,
                 .max = G4X_M1_DUAL_LVDS_MAX },
        .m2  = { .min = G4X_M2_DUAL_LVDS_MIN,
                 .max = G4X_M2_DUAL_LVDS_MAX },
        .p   = { .min = G4X_P_DUAL_LVDS_MIN,
                 .max = G4X_P_DUAL_LVDS_MAX },
        .p1  = { .min = G4X_P1_DUAL_LVDS_MIN,
                 .max = G4X_P1_DUAL_LVDS_MAX},
        .p2  = { .dot_limit = G4X_P2_DUAL_LVDS_LIMIT,
                 .p2_slow = G4X_P2_DUAL_LVDS_SLOW,
                 .p2_fast = G4X_P2_DUAL_LVDS_FAST },
        .find_pll = intel_find_pll_g4x,
    },
};

static const intel_limit_t intel_limits_ironlake_dac = {
    .dot = { .min = IRONLAKE_DOT_MIN,          .max = IRONLAKE_DOT_MAX },
    .vco = { .min = IRONLAKE_VCO_MIN,          .max = IRONLAKE_VCO_MAX },
    .n   = { .min = IRONLAKE_DAC_N_MIN,        .max = IRONLAKE_DAC_N_MAX },
    .m   = { .min = IRONLAKE_DAC_M_MIN,        .max = IRONLAKE_DAC_M_MAX },
    .m1  = { .min = IRONLAKE_M1_MIN,           .max = IRONLAKE_M1_MAX },
    .m2  = { .min = IRONLAKE_M2_MIN,           .max = IRONLAKE_M2_MAX },
    .p   = { .min = IRONLAKE_DAC_P_MIN,        .max = IRONLAKE_DAC_P_MAX },
    .p1  = { .min = IRONLAKE_DAC_P1_MIN,       .max = IRONLAKE_DAC_P1_MAX },
    .p2  = { .dot_limit = IRONLAKE_P2_DOT_LIMIT,
	.p2_slow = IRONLAKE_DAC_P2_SLOW,
	.p2_fast = IRONLAKE_DAC_P2_FAST },
    .find_pll = intel_igdng_find_best_PLL,
};

static const intel_limit_t intel_limits_ironlake_single_lvds = {
    .dot = { .min = IRONLAKE_DOT_MIN,          .max = IRONLAKE_DOT_MAX },
    .vco = { .min = IRONLAKE_VCO_MIN,          .max = IRONLAKE_VCO_MAX },
    .n   = { .min = IRONLAKE_LVDS_S_N_MIN,     .max = IRONLAKE_LVDS_S_N_MAX },
    .m   = { .min = IRONLAKE_LVDS_S_M_MIN,     .max = IRONLAKE_LVDS_S_M_MAX },
    .m1  = { .min = IRONLAKE_M1_MIN,           .max = IRONLAKE_M1_MAX },
    .m2  = { .min = IRONLAKE_M2_MIN,           .max = IRONLAKE_M2_MAX },
    .p   = { .min = IRONLAKE_LVDS_S_P_MIN,     .max = IRONLAKE_LVDS_S_P_MAX },
    .p1  = { .min = IRONLAKE_LVDS_S_P1_MIN,    .max = IRONLAKE_LVDS_S_P1_MAX },
    .p2  = { .dot_limit = IRONLAKE_P2_DOT_LIMIT,
	.p2_slow = IRONLAKE_LVDS_S_P2_SLOW,
	.p2_fast = IRONLAKE_LVDS_S_P2_FAST },
    .find_pll = intel_igdng_find_best_PLL,
};

static const intel_limit_t intel_limits_ironlake_dual_lvds = {
    .dot = { .min = IRONLAKE_DOT_MIN,          .max = IRONLAKE_DOT_MAX },
    .vco = { .min = IRONLAKE_VCO_MIN,          .max = IRONLAKE_VCO_MAX },
    .n   = { .min = IRONLAKE_LVDS_D_N_MIN,     .max = IRONLAKE_LVDS_D_N_MAX },
    .m   = { .min = IRONLAKE_LVDS_D_M_MIN,     .max = IRONLAKE_LVDS_D_M_MAX },
    .m1  = { .min = IRONLAKE_M1_MIN,           .max = IRONLAKE_M1_MAX },
    .m2  = { .min = IRONLAKE_M2_MIN,           .max = IRONLAKE_M2_MAX },
    .p   = { .min = IRONLAKE_LVDS_D_P_MIN,     .max = IRONLAKE_LVDS_D_P_MAX },
    .p1  = { .min = IRONLAKE_LVDS_D_P1_MIN,    .max = IRONLAKE_LVDS_D_P1_MAX },
    .p2  = { .dot_limit = IRONLAKE_P2_DOT_LIMIT,
	.p2_slow = IRONLAKE_LVDS_D_P2_SLOW,
	.p2_fast = IRONLAKE_LVDS_D_P2_FAST },
   .find_pll = intel_igdng_find_best_PLL,
};

static const intel_limit_t intel_limits_ironlake_single_lvds_100m = {
    .dot = { .min = IRONLAKE_DOT_MIN,          .max = IRONLAKE_DOT_MAX },
    .vco = { .min = IRONLAKE_VCO_MIN,          .max = IRONLAKE_VCO_MAX },
    .n   = { .min = IRONLAKE_LVDS_S_SSC_N_MIN, .max = IRONLAKE_LVDS_S_SSC_N_MAX },
    .m   = { .min = IRONLAKE_LVDS_S_SSC_M_MIN, .max = IRONLAKE_LVDS_S_SSC_M_MAX },
    .m1  = { .min = IRONLAKE_M1_MIN,           .max = IRONLAKE_M1_MAX },
    .m2  = { .min = IRONLAKE_M2_MIN,           .max = IRONLAKE_M2_MAX },
    .p   = { .min = IRONLAKE_LVDS_S_SSC_P_MIN, .max = IRONLAKE_LVDS_S_SSC_P_MAX },
    .p1  = { .min = IRONLAKE_LVDS_S_SSC_P1_MIN,.max = IRONLAKE_LVDS_S_SSC_P1_MAX },
    .p2  = { .dot_limit = IRONLAKE_P2_DOT_LIMIT,
	.p2_slow = IRONLAKE_LVDS_S_SSC_P2_SLOW,
	.p2_fast = IRONLAKE_LVDS_S_SSC_P2_FAST },
   .find_pll = intel_igdng_find_best_PLL,
};

static const intel_limit_t intel_limits_ironlake_dual_lvds_100m = {
    .dot = { .min = IRONLAKE_DOT_MIN,          .max = IRONLAKE_DOT_MAX },
    .vco = { .min = IRONLAKE_VCO_MIN,          .max = IRONLAKE_VCO_MAX },
    .n   = { .min = IRONLAKE_LVDS_D_SSC_N_MIN, .max = IRONLAKE_LVDS_D_SSC_N_MAX },
    .m   = { .min = IRONLAKE_LVDS_D_SSC_M_MIN, .max = IRONLAKE_LVDS_D_SSC_M_MAX },
    .m1  = { .min = IRONLAKE_M1_MIN,           .max = IRONLAKE_M1_MAX },
    .m2  = { .min = IRONLAKE_M2_MIN,           .max = IRONLAKE_M2_MAX },
    .p   = { .min = IRONLAKE_LVDS_D_SSC_P_MIN, .max = IRONLAKE_LVDS_D_SSC_P_MAX },
    .p1  = { .min = IRONLAKE_LVDS_D_SSC_P1_MIN,.max = IRONLAKE_LVDS_D_SSC_P1_MAX },
    .p2  = { .dot_limit = IRONLAKE_P2_DOT_LIMIT,
	.p2_slow = IRONLAKE_LVDS_D_SSC_P2_SLOW,
	.p2_fast = IRONLAKE_LVDS_D_SSC_P2_FAST },
   .find_pll = intel_igdng_find_best_PLL,
};


static const intel_limit_t *intel_ironlake_limit(xf86CrtcPtr crtc)
{
    ScrnInfoPtr	scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    const intel_limit_t *limit;

    if (i830PipeHasType(crtc, I830_OUTPUT_LVDS)) {
	int refclk = 120;

	if (intel->lvds_use_ssc && intel->lvds_ssc_freq)
	    refclk = 100;

	if ((INREG(PCH_LVDS) & LVDS_CLKB_POWER_MASK) == LVDS_CLKB_POWER_UP) {
	    if (refclk == 100)
		limit = &intel_limits_ironlake_dual_lvds_100m;
	    else
		limit = &intel_limits_ironlake_dual_lvds;
	} else {
	    if (refclk == 100)
		limit = &intel_limits_ironlake_single_lvds_100m;
	    else
		limit = &intel_limits_ironlake_single_lvds;
	}
    } else
	limit = &intel_limits_ironlake_dac;

    return limit;
}


static const intel_limit_t *intel_limit_g4x (xf86CrtcPtr crtc)
{
    ScrnInfoPtr	scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    const intel_limit_t *limit;

    if (i830PipeHasType (crtc, I830_OUTPUT_LVDS)) {
        if ((INREG(LVDS) & LVDS_CLKB_POWER_MASK) == LVDS_CLKB_POWER_UP) {
            /* LVDS with dual channel */
            limit = &intel_limits[INTEL_LIMIT_G4X_DUAL_LVDS];
        } else /* LVDS with single channel */
            limit = &intel_limits[INTEL_LIMIT_G4X_SINGLE_LVDS];
    } else if (i830PipeHasType (crtc, I830_OUTPUT_HDMI) ||
               i830PipeHasType (crtc, I830_OUTPUT_ANALOG)) {
        limit = &intel_limits[INTEL_LIMIT_G4X_HDMI_DAC];
    } else if (i830PipeHasType (crtc, I830_OUTPUT_SDVO)) {
        limit = &intel_limits[INTEL_LIMIT_G4X_SDVO];
    } else /* The option is for other outputs */
        limit = &intel_limits[INTEL_LIMIT_I9XX_SDVO_DAC];
    return limit;
}

static const intel_limit_t *intel_limit (xf86CrtcPtr crtc)
{
    ScrnInfoPtr	scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    const intel_limit_t *limit;

    if (HAS_PCH_SPLIT(intel)) {
	limit = intel_ironlake_limit(crtc);
    } else if (IS_G4X(intel)) {
        limit = intel_limit_g4x(crtc);
    } else if (IS_I9XX(intel) && !IS_IGD(intel)) {
	if (i830PipeHasType (crtc, I830_OUTPUT_LVDS))
	    limit = &intel_limits[INTEL_LIMIT_I9XX_LVDS];
	else
	    limit = &intel_limits[INTEL_LIMIT_I9XX_SDVO_DAC];
    } else if (IS_IGD(intel)) {
	if (i830PipeHasType (crtc, I830_OUTPUT_LVDS))
	    limit = &intel_limits[INTEL_LIMIT_IGD_LVDS];
	else
	    limit = &intel_limits[INTEL_LIMIT_IGD_SDVO_DAC];
    } else {
	if (i830PipeHasType (crtc, I830_OUTPUT_LVDS))
	    limit = &intel_limits[INTEL_LIMIT_I8XX_LVDS];
	else
	    limit = &intel_limits[INTEL_LIMIT_I8XX_DVO_DAC];
    }

    return limit;
}

/** Derive the pixel clock for the given refclk and divisors for 8xx chips. */

static void i8xx_clock(int refclk, intel_clock_t *clock)
{
    clock->m = 5 * (clock->m1 + 2) + (clock->m2 + 2);
    clock->p = clock->p1 * clock->p2;
    clock->vco = refclk * clock->m / (clock->n + 2);
    clock->dot = clock->vco / clock->p;
}

/** Derive the pixel clock for the given refclk and divisors for 9xx chips. */

static void i9xx_clock(int refclk, intel_clock_t *clock)
{
    clock->m = 5 * (clock->m1 + 2) + (clock->m2 + 2);
    clock->p = clock->p1 * clock->p2;
    clock->vco = refclk * clock->m / (clock->n + 2);
    clock->dot = clock->vco / clock->p;
}

/* m1 is reserved as 0 in IGD, n is a ring counter */
static void igd_clock(int refclk, intel_clock_t *clock)
{
    clock->m = clock->m2 + 2;
    clock->p = clock->p1 * clock->p2;
    clock->vco = refclk * clock->m / clock->n;
    clock->dot = clock->vco / clock->p;
}

static void intel_clock(intel_screen_private *intel, int refclk, intel_clock_t *clock)
{
    if (IS_I9XX(intel)) {
	if (IS_IGD(intel))
	    igd_clock(refclk, clock);
	else
	    i9xx_clock (refclk, clock);
    } else
	i8xx_clock (refclk, clock);
}

static void
i830PrintPll(ScrnInfoPtr scrn, char *prefix, intel_clock_t *clock)
{
    xf86DrvMsg(scrn->scrnIndex, X_INFO,
	       "%s: dotclock %d vco %d ((m %d, m1 %d, m2 %d), n %d, "
	       "(p %d, p1 %d, p2 %d))\n",
	       prefix, clock->dot, clock->vco,
	       clock->m, clock->m1, clock->m2,
	       clock->n,
	       clock->p, clock->p1, clock->p2);
}

static Bool
i830PllIsValid(xf86CrtcPtr crtc, intel_clock_t *clock);

static Bool
intel_igdng_find_best_PLL(const intel_limit_t *limit, xf86CrtcPtr crtc,
			  int target, int refclk, intel_clock_t *best_clock)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    intel_clock_t clock;
    int max_n;
    Bool found = FALSE;
    /* Approximately equals target * 0.00585 */
    int err_most = (target >> 8) + (target >> 9);

    if (i830PipeHasType(crtc, I830_OUTPUT_LVDS)) {
	if ((INREG(PCH_LVDS) & LVDS_CLKB_POWER_MASK) == LVDS_CLKB_POWER_UP)
	    clock.p2 = limit->p2.p2_fast;
	else
	    clock.p2 = limit->p2.p2_slow;
    } else {
	if (target < limit->p2.dot_limit)
	    clock.p2 = limit->p2.p2_slow;
	else
	    clock.p2 = limit->p2.p2_fast;
    }

    memset(best_clock, 0, sizeof(*best_clock));
    max_n = limit->n.max;
    /* based on hardware requriment prefer smaller n to precision */
    for (clock.n = limit->n.min; clock.n <= max_n; clock.n++) {
	/* based on hardware requirment prefere larger m1,m2 */
	for (clock.m1 = limit->m1.max; clock.m1 >= limit->m1.min; clock.m1--) {
	    for (clock.m2 = limit->m2.max;
		 clock.m2 >= limit->m2.min; clock.m2--) {
		for (clock.p1 = limit->p1.max;
		     clock.p1 >= limit->p1.min; clock.p1--) {
		    int this_err;

		    intel_clock(intel, refclk, &clock);
		    if (!i830PllIsValid(crtc, &clock))
			continue;
		    this_err = abs(clock.dot - target) ;
		    if (this_err < err_most) {
			*best_clock = clock;
			err_most = this_err;
			max_n = clock.n;
			found = TRUE;
		    }
		}
	    }
	}
    }
    return found;
}

/**
 * Returns whether any output on the specified pipe is of the specified type
 */
Bool
i830PipeHasType (xf86CrtcPtr crtc, int type)
{
    ScrnInfoPtr	scrn = crtc->scrn;
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    int		i;

    for (i = 0; i < xf86_config->num_output; i++)
    {
	xf86OutputPtr  output = xf86_config->output[i];
	if (output->crtc == crtc)
	{
	    I830OutputPrivatePtr    intel_output = output->driver_private;
	    if (intel_output->type == type)
		return TRUE;
	}
    }
    return FALSE;
}

#if 1
#define i830PllInvalid(s)   { /* ErrorF (s) */; return FALSE; }
#else
#define i830PllInvalid(s)   { ErrorF (s) ; return FALSE; }
#endif
/**
 * Returns whether the given set of divisors are valid for a given refclk with
 * the given outputs.
 */

static Bool
i830PllIsValid(xf86CrtcPtr crtc, intel_clock_t *clock)
{
    const intel_limit_t *limit = intel_limit (crtc);
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);

    if (clock->p1  < limit->p1.min  || limit->p1.max  < clock->p1)
	i830PllInvalid ("p1 out of range\n");
    if (clock->p   < limit->p.min   || limit->p.max   < clock->p)
	i830PllInvalid ("p out of range\n");
    if (clock->m2  < limit->m2.min  || limit->m2.max  < clock->m2)
	i830PllInvalid ("m2 out of range\n");
    if (clock->m1  < limit->m1.min  || limit->m1.max  < clock->m1)
	i830PllInvalid ("m1 out of range\n");
    if (clock->m1 <= clock->m2 && !IS_IGD(intel))
	i830PllInvalid ("m1 <= m2\n");
    if (clock->m   < limit->m.min   || limit->m.max   < clock->m)
	i830PllInvalid ("m out of range\n");
    if (clock->n   < limit->n.min   || limit->n.max   < clock->n)
	i830PllInvalid ("n out of range\n");
    if (clock->vco < limit->vco.min || limit->vco.max < clock->vco)
	i830PllInvalid ("vco out of range\n");
    /* XXX: We may need to be checking "Dot clock" depending on the multiplier,
     * output, etc., rather than just a single range.
     */
    if (clock->dot < limit->dot.min || limit->dot.max < clock->dot)
	i830PllInvalid ("dot out of range\n");

    return TRUE;
}

static Bool
intel_find_pll_i8xx_and_i9xx(const intel_limit_t * limit, xf86CrtcPtr crtc,
	                     int target, int refclk, intel_clock_t *best_clock)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    intel_clock_t clock;
    int err = target;

    if (i830PipeHasType(crtc, I830_OUTPUT_LVDS))
    {
	/* For LVDS, if the panel is on, just rely on its current settings for
	 * dual-channel.  We haven't figured out how to reliably set up
	 * different single/dual channel state, if we even can.
	 */
	if ((INREG(LVDS) & LVDS_CLKB_POWER_MASK) == LVDS_CLKB_POWER_UP)
	    clock.p2 = limit->p2.p2_fast;
	else
	    clock.p2 = limit->p2.p2_slow;
    } else {
	if (target < limit->p2.dot_limit)
	    clock.p2 = limit->p2.p2_slow;
	else
	    clock.p2 = limit->p2.p2_fast;
    }

    memset (best_clock, 0, sizeof (*best_clock));

    for (clock.m1 = limit->m1.min; clock.m1 <= limit->m1.max; clock.m1++) 
    {
	for (clock.m2 = limit->m2.min; clock.m2 <= limit->m2.max; clock.m2++)
	{
	    /* m1 is always 0 in IGD */
	    if (clock.m2 >= clock.m1 && !IS_IGD(intel))
		break;
	    for (clock.n = limit->n.min; clock.n <= limit->n.max; clock.n++) 
	    {
		for (clock.p1 = limit->p1.min; clock.p1 <= limit->p1.max; clock.p1++) 
		{
		    int this_err;

		    intel_clock (intel, refclk, &clock);
		    
		    if (!i830PllIsValid(crtc, &clock))
			continue;

		    this_err = abs(clock.dot - target);
		    if (this_err < err) {
			*best_clock = clock;
			err = this_err;
		    }
		}
	    }
	}
    }
    return (err != target);
}

static Bool
intel_find_pll_g4x(const intel_limit_t * limit, xf86CrtcPtr crtc,
                   int target, int refclk, intel_clock_t *best_clock)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    intel_clock_t clock;
    int  max_n;
    Bool found = FALSE;
    int err_most = target * 0.0048;

    if (i830PipeHasType(crtc, I830_OUTPUT_LVDS))
    {
        /* For LVDS, if the panel is on, just rely on its current settings for
         * dual-channel.  We haven't figured out how to reliably set up
         * different single/dual channel state, if we even can.
         */
        if ((INREG(LVDS) & LVDS_CLKB_POWER_MASK) == LVDS_CLKB_POWER_UP)
            clock.p2 = limit->p2.p2_fast;
        else
            clock.p2 = limit->p2.p2_slow;
    } else {
        if (target < limit->p2.dot_limit)
            clock.p2 = limit->p2.p2_slow;
        else
            clock.p2 = limit->p2.p2_fast;
    }

    max_n = limit->n.max;
    /* based on hardware requirement prefer smaller n to precision */
    for (clock.n = limit->n.min; clock.n <= max_n; clock.n++) {
        /* based on hardware requirement prefere larger m1,m2, p1*/
        for (clock.m1 = limit->m1.max;
            clock.m1 >= limit->m1.min; clock.m1--) {
            for (clock.m2 = limit->m2.max;
                clock.m2 >= limit->m2.min; clock.m2--) {
                for (clock.p1 = limit->p1.max;
                    clock.p1 >= limit->p1.min; clock.p1--) {
                    int this_err;

                    intel_clock (intel, refclk, &clock);
                    if (!i830PllIsValid(crtc, &clock))
                        continue;
                    this_err = abs(clock.dot - target) ;
                    if (this_err < err_most) {
                        memcpy(best_clock, &clock, sizeof(intel_clock_t));
                        err_most = this_err;
                        /* prefer smaller n to precision */
                        max_n = clock.n;
                        found = TRUE;
                    }
                }
            }
        }
    }
    return found;
}

void
i830WaitForVblank(ScrnInfoPtr pScreen)
{
    /* Wait for 20ms, i.e. one cycle at 50hz. */
    usleep(30000);
}

void
i830PipeSetBase(xf86CrtcPtr crtc, int x, int y)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    int plane = intel_crtc->plane;
    unsigned long Start, Offset, Stride;
    drm_intel_bo *bo;
    int dspbase = (plane == 0 ? DSPABASE : DSPBBASE);
    int dspsurf = (plane == 0 ? DSPASURF : DSPBSURF);
    int dsptileoff = (plane == 0 ? DSPATILEOFF : DSPBTILEOFF);
    int dspstride = (plane == 0) ? DSPASTRIDE : DSPBSTRIDE;

    crtc->x = x;
    crtc->y = y;

    Stride = intel->front_pitch;
    bo = intel->front_buffer;
    Offset = y * Stride + x * intel->cpp;
    if (intel_crtc->rotate_bo != NULL) {
	Stride = intel_crtc->rotate_pitch;
	bo = intel_crtc->rotate_bo;
	Offset = 0;
	x = y = 0;
    }

    /*
     * During startup we may be called as part of monitor detection while
     * there is no memory allocation done, so just supply a dummy base
     * address.
     */
    if (bo == NULL) {
	Start = 0;
    } else {
	Start = bo->offset;
    }

    OUTREG(dspstride, Stride);
    if (IS_I965G(intel)) {
        OUTREG(dspbase, Offset);
	POSTING_READ(dspbase);
        OUTREG(dspsurf, Start);
	POSTING_READ(dspsurf);
	OUTREG(dsptileoff, (y << 16) | x);
    } else {
	OUTREG(dspbase, Start + Offset);
	POSTING_READ(dspbase);
    }
}

/*
 * Both crtc activation and video overlay enablement on pipe B
 * will fail on i830 if pipe A is not running. This function
 * makes sure pipe A is active for these cases
 */

int
i830_crtc_pipe (xf86CrtcPtr crtc)
{
    if (crtc == NULL)
	return 0;
    return ((I830CrtcPrivatePtr) crtc->driver_private)->pipe;
}

static xf86CrtcPtr
i830_crtc_for_pipe (ScrnInfoPtr scrn, int pipe)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    int			c;

    for (c = 0; c < xf86_config->num_crtc; c++)
    {
	xf86CrtcPtr crtc = xf86_config->crtc[c];
	if (i830_crtc_pipe (crtc) == pipe)
	    return crtc;
    }
    return NULL;
}

Bool
i830_pipe_a_require_activate (ScrnInfoPtr scrn)
{
    xf86CrtcPtr	crtc = i830_crtc_for_pipe (scrn, 0);
    intel_screen_private *intel = intel_get_screen_private(scrn);
    /* VESA 640x480x72Hz mode to set on the pipe */
    static DisplayModeRec   mode = {
	NULL, NULL, "640x480", MODE_OK, M_T_DEFAULT,
	31500,
	640, 664, 704, 832, 0,
	480, 489, 491, 520, 0,
	V_NHSYNC | V_NVSYNC,
	0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0,
	FALSE, FALSE, 0, NULL, 0, 0.0, 0.0
    };

    if (HAS_PCH_SPLIT(intel))
	return FALSE;

    if (!crtc)
	return FALSE;
    if (crtc->enabled)
	return FALSE;
    xf86SetModeCrtc (&mode, INTERLACE_HALVE_V);
    crtc->funcs->mode_set (crtc, &mode, &mode, 0, 0);
    crtc->funcs->dpms (crtc, DPMSModeOn);
    return TRUE;
}

void
i830_pipe_a_require_deactivate (ScrnInfoPtr scrn)
{
    xf86CrtcPtr	crtc = i830_crtc_for_pipe (scrn, 0);

    if (!crtc)
	return;
    if (crtc->enabled)
	return;
    crtc->funcs->dpms (crtc, DPMSModeOff);
    return;
}

/* FIXME: use pixmap private instead if possible */
static Bool
i830_display_tiled(xf86CrtcPtr crtc)
{
    ScrnInfoPtr scrn = crtc->scrn;
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    drm_intel_bo *bo = intel->front_buffer;

    if (intel_crtc->rotate_bo != NULL)
	bo = intel_crtc->rotate_bo;

    if (bo != NULL) {
        uint32_t tiling_mode, swizzle;
	if (drm_intel_bo_get_tiling(bo, &tiling_mode, &swizzle) == 0 &&
	    tiling_mode != I915_TILING_NONE)
		return TRUE;
    }

    return FALSE;
}

#if defined(DRM_IOCTL_MODESET_CTL)
static void i830_modeset_ctl(xf86CrtcPtr crtc, int pre)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    struct drm_modeset_ctl modeset;

    if (intel->directRenderingType <= DRI_NONE)
      return;

    modeset.crtc = intel_crtc->pipe;

    /*
     * DPMS will be called many times (especially off), but we only
     * want to catch the transition from on->off and off->on.
     */
    if (pre && intel_crtc->dpms_mode != DPMSModeOff) {
	/* On -> off is a pre modeset */
	modeset.cmd = _DRM_PRE_MODESET;
	ioctl(intel->drmSubFD, DRM_IOCTL_MODESET_CTL, &modeset);
    } else if (!pre && intel_crtc->dpms_mode == DPMSModeOff) {
	/* Off -> on means post modeset */
	modeset.cmd = _DRM_POST_MODESET;
	ioctl(intel->drmSubFD, DRM_IOCTL_MODESET_CTL, &modeset);
    }
}
#else
static void i830_modeset_ctl(xf86CrtcPtr crtc, int dpms_state)
{
    return;
}
#endif /* DRM_IOCTL_MODESET_CTL */

static void
i830_disable_vga_plane (xf86CrtcPtr crtc)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint8_t sr01 = 0;
    uint32_t vga_reg, vgacntrl;

    if (HAS_PCH_SPLIT(intel))
	vga_reg = CPU_VGACNTRL;
    else
	vga_reg = VGACNTRL;

    vgacntrl = INREG(vga_reg);
    if (vgacntrl & VGA_DISP_DISABLE)
	return;

    /*
     * Bug #17235: G4X machine needs following steps
     * for disable VGA.
     * - set bit 5 of SR01;
     * - Wait 30us;
     * - disable vga plane;
     * - restore SR01;
     */
    if (IS_G4X(intel)) {
	OUTREG8(SRX, 1);
	sr01 = INREG8(SRX + 1);
	OUTREG8(SRX + 1, sr01 | (1 << 5));
	usleep(30);
    }

    while (!(INREG(vga_reg) & VGA_DISP_DISABLE)) {
	vgacntrl |= VGA_DISP_DISABLE;
	OUTREG(vga_reg, vgacntrl);
	i830WaitForVblank(scrn);
    }

    /* restore SR01 */
    if (IS_G4X(intel)) {
	OUTREG8(SRX, 1);
	OUTREG8(SRX + 1, sr01);
    }
}

static void
i830_crtc_enable(xf86CrtcPtr crtc)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    int pipe = intel_crtc->pipe;
    int plane = intel_crtc->plane;
    int dpll_reg = (pipe == 0) ? DPLL_A : DPLL_B;
    int pipeconf_reg = (pipe == 0) ? PIPEACONF : PIPEBCONF;
    int dspcntr_reg = (plane == 0) ? DSPACNTR : DSPBCNTR;
    int dspbase_reg = (plane == 0) ? DSPABASE : DSPBBASE;
    uint32_t temp;

    /* Enable the DPLL */
    temp = INREG(dpll_reg);
    if ((temp & DPLL_VCO_ENABLE) == 0)
    {
	OUTREG(dpll_reg, temp);
	POSTING_READ(dpll_reg);
	/* Wait for the clocks to stabilize. */
	usleep(150);
	OUTREG(dpll_reg, temp | DPLL_VCO_ENABLE);
	POSTING_READ(dpll_reg);
	/* Wait for the clocks to stabilize. */
	usleep(150);
	OUTREG(dpll_reg, temp | DPLL_VCO_ENABLE);
	POSTING_READ(dpll_reg);
	/* Wait for the clocks to stabilize. */
	usleep(150);
    }

    /* Enable the pipe */
    temp = INREG(pipeconf_reg);
    if ((temp & PIPEACONF_ENABLE) == 0)
	OUTREG(pipeconf_reg, temp | PIPEACONF_ENABLE);

    /* Enable the plane */
    temp = INREG(dspcntr_reg);
    if ((temp & DISPLAY_PLANE_ENABLE) == 0)
    {
	OUTREG(dspcntr_reg, temp | DISPLAY_PLANE_ENABLE);
	/* Flush the plane changes */
	OUTREG(dspbase_reg, INREG(dspbase_reg));
    }

    i830_crtc_load_lut(crtc);

    /* Give the overlay scaler a chance to enable if it's on this pipe */
    i830_crtc_dpms_video(crtc, TRUE);

    i830_modeset_ctl(crtc, 0);
}

void
i830_crtc_disable(xf86CrtcPtr crtc, Bool disable_pipe)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    int pipe = intel_crtc->pipe;
    int plane = intel_crtc->plane;
    int dpll_reg = (pipe == 0) ? DPLL_A : DPLL_B;
    int pipeconf_reg = (pipe == 0) ? PIPEACONF : PIPEBCONF;
    int dspcntr_reg = (plane == 0) ? DSPACNTR : DSPBCNTR;
    int dspbase_reg = (plane == 0) ? DSPABASE : DSPBBASE;
    uint32_t temp;

    i830_modeset_ctl(crtc, 1);

    /* Give the overlay scaler a chance to disable if it's on this pipe */
    i830_crtc_dpms_video(crtc, FALSE);

    /* 
     * The documentation says :
     * - Disable planes (VGA or hires)
     * - Disable pipe
     * - Disable VGA display
     */

    /* Disable display plane */
    temp = INREG(dspcntr_reg);
    if ((temp & DISPLAY_PLANE_ENABLE) != 0)
    {
	OUTREG(dspcntr_reg, temp & ~DISPLAY_PLANE_ENABLE);
	/* Flush the plane changes */
	OUTREG(dspbase_reg, INREG(dspbase_reg));
	POSTING_READ(dspbase_reg);
    }

    if (!IS_I9XX(intel)) {
	/* Wait for vblank for the disable to take effect */
	i830WaitForVblank(scrn);
    }

    /* May need to leave pipe A on */
    if (disable_pipe)
    {
	/* Next, disable display pipes */
	temp = INREG(pipeconf_reg);
	if ((temp & PIPEACONF_ENABLE) != 0) {
	    OUTREG(pipeconf_reg, temp & ~PIPEACONF_ENABLE);
	    POSTING_READ(pipeconf_reg);
	}

	/* Wait for vblank for the disable to take effect. */
	i830WaitForVblank(scrn);

	temp = INREG(dpll_reg);
	if ((temp & DPLL_VCO_ENABLE) != 0) {
	    OUTREG(dpll_reg, temp & ~DPLL_VCO_ENABLE);
	    POSTING_READ(dpll_reg);
	}

	/* Wait for the clocks to turn off. */
	usleep(150);
    }

    /* Disable the VGA plane that we never use. */
    i830_disable_vga_plane (crtc);
}

static void ironlake_fdi_link_train(xf86CrtcPtr crtc)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    int pipe = intel_crtc->pipe;
    int fdi_tx_reg = (pipe == 0) ? FDI_TXA_CTL : FDI_TXB_CTL;
    int fdi_rx_reg = (pipe == 0) ? FDI_RXA_CTL : FDI_RXB_CTL;
    int fdi_rx_iir_reg = (pipe == 0) ? FDI_RXA_IIR : FDI_RXB_IIR;
    int fdi_rx_imr_reg = (pipe == 0) ? FDI_RXA_IMR : FDI_RXB_IMR;
    uint32_t temp, tries = 0;
    
    /* enable CPU FDI TX and PCH FDI RX */
    temp = INREG(fdi_tx_reg);
    temp |= FDI_TX_ENABLE;
    temp &= ~(7 << 19);
    temp |= /*(intel_crtc->fdi_lanes - 1)*/3 << 19;
    temp &= ~FDI_LINK_TRAIN_NONE;
    temp |= FDI_LINK_TRAIN_PATTERN_1;
    OUTREG(fdi_tx_reg, temp);
    INREG(fdi_tx_reg);
    
    temp = INREG(fdi_rx_reg);
    temp &= ~FDI_LINK_TRAIN_NONE;
    temp |= FDI_LINK_TRAIN_PATTERN_1;
    OUTREG(fdi_rx_reg, temp | FDI_RX_ENABLE);
    INREG(fdi_rx_reg);
    usleep(150);
    
    /* Train 1: umask FDI RX Interrupt symbol_lock and bit_lock bit
       for train result */
    temp = INREG(fdi_rx_imr_reg);
    temp &= ~FDI_RX_SYMBOL_LOCK;
    temp &= ~FDI_RX_BIT_LOCK;
    OUTREG(fdi_rx_imr_reg, temp);
    INREG(fdi_rx_imr_reg);
    usleep(150);

    for (;;) {
	temp = INREG(fdi_rx_iir_reg);
	ErrorF("FDI_RX_IIR 0x%x\n", temp);
	
	if ((temp & FDI_RX_BIT_LOCK)) {
	    ErrorF("FDI train 1 done.\n");
	    OUTREG(fdi_rx_iir_reg,
		       temp | FDI_RX_BIT_LOCK);
	    break;
	}
	
	tries++;

	if (tries > 5) {
	    ErrorF("FDI train 1 fail!\n");
	    break;
	}
    }
    
    /* Train 2 */
    temp = INREG(fdi_tx_reg);
    temp &= ~FDI_LINK_TRAIN_NONE;
    temp |= FDI_LINK_TRAIN_PATTERN_2;
    OUTREG(fdi_tx_reg, temp);

    temp = INREG(fdi_rx_reg);
    temp &= ~FDI_LINK_TRAIN_NONE;
    temp |= FDI_LINK_TRAIN_PATTERN_2;
    OUTREG(fdi_rx_reg, temp);
    usleep(150);

    tries = 0;

    for (;;) {
	temp = INREG(fdi_rx_iir_reg);
	ErrorF("FDI_RX_IIR 0x%x\n", temp);
	
	if (temp & FDI_RX_SYMBOL_LOCK) {
	    OUTREG(fdi_rx_iir_reg,
		       temp | FDI_RX_SYMBOL_LOCK);
	    ErrorF("FDI train 2 done.\n");
	    break;
	}

	tries++;

	if (tries > 5) {
	    ErrorF("FDI train 2 fail!\n");
	    break;
	}
    }
    
    ErrorF("FDI train done\n");
}

static const int snb_b_fdi_train_param [] = {
        FDI_LINK_TRAIN_400MV_0DB_SNB_B,
        FDI_LINK_TRAIN_400MV_6DB_SNB_B,
        FDI_LINK_TRAIN_600MV_3_5DB_SNB_B,
        FDI_LINK_TRAIN_800MV_0DB_SNB_B,
};

static void gen6_fdi_link_train(xf86CrtcPtr crtc)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    int pipe = intel_crtc->pipe;
    int fdi_tx_reg = (pipe == 0) ? FDI_TXA_CTL : FDI_TXB_CTL;
    int fdi_rx_reg = (pipe == 0) ? FDI_RXA_CTL : FDI_RXB_CTL;
    int fdi_rx_iir_reg = (pipe == 0) ? FDI_RXA_IIR : FDI_RXB_IIR;
    int fdi_rx_imr_reg = (pipe == 0) ? FDI_RXA_IMR : FDI_RXB_IMR;
    uint32_t temp, i = 0;
    
    /* Train 1: umask FDI RX Interrupt symbol_lock and bit_lock bit
       for train result */
    temp = INREG(fdi_rx_imr_reg);
    temp &= ~FDI_RX_SYMBOL_LOCK;
    temp &= ~FDI_RX_BIT_LOCK;
    OUTREG(fdi_rx_imr_reg, temp);
    INREG(fdi_rx_imr_reg);
    usleep(150);

    /* enable CPU FDI TX and PCH FDI RX */
    temp = INREG(fdi_tx_reg);
    temp |= FDI_TX_ENABLE;
    temp &= ~(7 << 19);
    temp |= /*(intel_crtc->fdi_lanes - 1)*/3 << 19;
    temp &= ~FDI_LINK_TRAIN_NONE;
    temp |= FDI_LINK_TRAIN_PATTERN_1;
    temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
    /* SNB-B */
    temp |= FDI_LINK_TRAIN_400MV_0DB_SNB_B;
    OUTREG(fdi_tx_reg, temp);
    INREG(fdi_tx_reg);
    
    temp = INREG(fdi_rx_reg);
    if (HAS_PCH_CPT(intel)) {
        temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
        temp |= FDI_LINK_TRAIN_PATTERN_1_CPT;
    } else {
        temp &= ~FDI_LINK_TRAIN_NONE;
        temp |= FDI_LINK_TRAIN_PATTERN_1;
    }
    OUTREG(fdi_rx_reg, temp | FDI_RX_ENABLE);
    INREG(fdi_rx_reg);
    usleep(150);
    
    for (i = 0; i < 4; i++) {
        temp = INREG(fdi_tx_reg);
        temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
        temp |= snb_b_fdi_train_param[i];
        OUTREG(fdi_tx_reg, temp);
        INREG(fdi_tx_reg);
	
        temp = INREG(fdi_rx_iir_reg);
	ErrorF("FDI_RX_IIR 0x%x\n", temp);

	if ((temp & FDI_RX_BIT_LOCK)) {
	    ErrorF("FDI train 1 done.\n");
	    OUTREG(fdi_rx_iir_reg,
		       temp | FDI_RX_BIT_LOCK);
	    break;
	}
    }
    if (i == 4)
	ErrorF("FDI train 1 fail!\n");

    /* Train 2 */
    temp = INREG(fdi_tx_reg);
    temp &= ~FDI_LINK_TRAIN_NONE;
    temp |= FDI_LINK_TRAIN_PATTERN_2;
    if (IS_GEN6(intel)) {
	temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
	/* SNB-B */
	temp |= FDI_LINK_TRAIN_400MV_0DB_SNB_B;
    }
    OUTREG(fdi_tx_reg, temp);

    temp = INREG(fdi_rx_reg);
    if (HAS_PCH_CPT(intel)) {
	temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
	temp |= FDI_LINK_TRAIN_PATTERN_2_CPT;
    } else {
	temp &= ~FDI_LINK_TRAIN_NONE;
	temp |= FDI_LINK_TRAIN_PATTERN_2;
    }
    OUTREG(fdi_rx_reg, temp);
    usleep(150);

    for (i = 0; i < 4; i++) {
        temp = INREG(fdi_tx_reg);
        temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
        temp |= snb_b_fdi_train_param[i];
        OUTREG(fdi_tx_reg, temp);
        INREG(fdi_tx_reg);
	
        temp = INREG(fdi_rx_iir_reg);
	ErrorF("FDI_RX_IIR 0x%x\n", temp);

	if ((temp & FDI_RX_SYMBOL_LOCK)) {
	    ErrorF("FDI train 2 done.\n");
	    OUTREG(fdi_rx_iir_reg,
		       temp | FDI_RX_SYMBOL_LOCK);
	    break;
	}
    }
    if (i == 4)
   	ErrorF("FDI train 2 fail!\n");

    ErrorF("FDI train done\n");
}

/* Manual link training for Ivy Bridge A0 parts */
static void ivb_manual_fdi_link_train(xf86CrtcPtr crtc)
{
	ScrnInfoPtr scrn = crtc->scrn;
	intel_screen_private *intel = intel_get_screen_private(scrn);
	I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
	int pipe = intel_crtc->pipe;
	int fdi_tx_reg = (pipe == 0) ? FDI_TXA_CTL : FDI_TXB_CTL;
	int fdi_rx_reg = (pipe == 0) ? FDI_RXA_CTL : FDI_RXB_CTL;
	int fdi_rx_iir_reg = (pipe == 0) ? FDI_RXA_IIR : FDI_RXB_IIR;
	int fdi_rx_imr_reg = (pipe == 0) ? FDI_RXA_IMR : FDI_RXB_IMR;

	uint32_t temp, i;

	/* Train 1: umask FDI RX Interrupt symbol_lock and bit_lock bit
	   for train result */
	temp = INREG(fdi_rx_imr_reg);
	temp &= ~FDI_RX_SYMBOL_LOCK;
	temp &= ~FDI_RX_BIT_LOCK;
	OUTREG(fdi_rx_imr_reg, temp);
	INREG(fdi_rx_imr_reg);
	usleep(150);

	/* enable CPU FDI TX and PCH FDI RX */
	temp = INREG(fdi_tx_reg);
	temp &= ~(7 << 19);
	temp |= /*(intel_crtc->fdi_lanes - 1)*/3 << 19;
	temp &= ~(FDI_LINK_TRAIN_AUTO | FDI_LINK_TRAIN_NONE_IVB);
	temp |= FDI_LINK_TRAIN_PATTERN_1_IVB;
	temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
	temp |= FDI_LINK_TRAIN_400MV_0DB_SNB_B;
	temp |= FDI_COMPOSITE_SYNC;
	OUTREG(fdi_tx_reg, temp | FDI_TX_ENABLE);
	INREG(fdi_tx_reg);

	temp = INREG(fdi_rx_reg);
	temp &= ~FDI_LINK_TRAIN_AUTO;
	temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
	temp |= FDI_LINK_TRAIN_PATTERN_1_CPT;
	temp |= FDI_COMPOSITE_SYNC;
	OUTREG(fdi_rx_reg, temp | FDI_RX_ENABLE);
	INREG(fdi_rx_reg);

	usleep(150);
		
	for (i = 0; i < 4; i++) {
		temp = INREG(fdi_tx_reg);
		temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
		temp |= snb_b_fdi_train_param[i];
		OUTREG(fdi_tx_reg, temp);
		INREG(fdi_tx_reg);

		usleep(500);

		temp = INREG(fdi_rx_iir_reg);
		ErrorF("FDI_RX_IIR 0x%x\n", temp);

		if (temp & FDI_RX_BIT_LOCK) {
			OUTREG(fdi_rx_iir_reg,
			           temp | FDI_RX_BIT_LOCK);
			ErrorF("FDI train 1 done.\n");
			break;
		}
	}
	if (i == 4)
		ErrorF("FDI train 1 fail!\n");

	/* Train 2 */
	temp = INREG(fdi_tx_reg);
	temp &= ~FDI_LINK_TRAIN_NONE_IVB;
	temp |= FDI_LINK_TRAIN_PATTERN_2_IVB;
	temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
	temp |= FDI_LINK_TRAIN_400MV_0DB_SNB_B;
	OUTREG(fdi_tx_reg, temp);

	temp = INREG(fdi_rx_reg);
	temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
	temp |= FDI_LINK_TRAIN_PATTERN_2_CPT;
	OUTREG(fdi_rx_reg, temp);

	usleep(150);

	for (i = 0; i < 4; i++) {
		temp = INREG(fdi_tx_reg);
		temp &= ~FDI_LINK_TRAIN_VOL_EMP_MASK;
		temp |= snb_b_fdi_train_param[i];
		OUTREG(fdi_tx_reg, temp);
		INREG(fdi_tx_reg);

		usleep(500);

		temp = INREG(fdi_rx_iir_reg);
		ErrorF("FDI_RX_IIR 0x%x\n", temp);

		if (temp & FDI_RX_SYMBOL_LOCK) {
			OUTREG(fdi_rx_iir_reg,
			           temp | FDI_RX_SYMBOL_LOCK);
			ErrorF("FDI train 2 done.\n");
			break;
		}
	}
	if (i == 4)
		ErrorF("FDI train 2 fail!\n");

	ErrorF("FDI train done\n");
}

static void
ironlake_crtc_enable(xf86CrtcPtr crtc)
{
	ScrnInfoPtr scrn = crtc->scrn;
	intel_screen_private *intel = intel_get_screen_private(scrn);
	I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;
	int pch_dpll_reg = (pipe == 0) ? PCH_DPLL_A : PCH_DPLL_B;
	int pipeconf_reg = (pipe == 0) ? PIPEACONF : PIPEBCONF;
	int dspcntr_reg = (plane == 0) ? DSPACNTR : DSPBCNTR;
	int dspbase_reg = (plane == 0) ? DSPABASE : DSPBBASE;
	int fdi_tx_reg = (pipe == 0) ? FDI_TXA_CTL : FDI_TXB_CTL;
	int fdi_rx_reg = (pipe == 0) ? FDI_RXA_CTL : FDI_RXB_CTL;
	int transconf_reg = (pipe == 0) ? TRANSACONF : TRANSBCONF;
	int pf_ctl_reg = (pipe == 0) ? PFA_CTL_1 : PFB_CTL_1;
	int pf_win_size = (pipe == 0) ? PFA_WIN_SZ : PFB_WIN_SZ;
	int pf_win_pos = (pipe == 0) ? PFA_WIN_POS : PFB_WIN_POS;
	int cpu_htot_reg = (pipe == 0) ? HTOTAL_A : HTOTAL_B;
	int cpu_hblank_reg = (pipe == 0) ? HBLANK_A : HBLANK_B;
	int cpu_hsync_reg = (pipe == 0) ? HSYNC_A : HSYNC_B;
	int cpu_vtot_reg = (pipe == 0) ? VTOTAL_A : VTOTAL_B;
	int cpu_vblank_reg = (pipe == 0) ? VBLANK_A : VBLANK_B;
	int cpu_vsync_reg = (pipe == 0) ? VSYNC_A : VSYNC_B;
	int trans_htot_reg = (pipe == 0) ? TRANS_HTOTAL_A : TRANS_HTOTAL_B;
	int trans_hblank_reg = (pipe == 0) ? TRANS_HBLANK_A : TRANS_HBLANK_B;
	int trans_hsync_reg = (pipe == 0) ? TRANS_HSYNC_A : TRANS_HSYNC_B;
	int trans_vtot_reg = (pipe == 0) ? TRANS_VTOTAL_A : TRANS_VTOTAL_B;
	int trans_vblank_reg = (pipe == 0) ? TRANS_VBLANK_A : TRANS_VBLANK_B;
	int trans_vsync_reg = (pipe == 0) ? TRANS_VSYNC_A : TRANS_VSYNC_B;
	uint32_t temp, pipe_bpc;

	temp = INREG(pipeconf_reg);
	pipe_bpc = temp & PIPE_BPC_MASK;

	/* XXX no LVDS port force */
	if (i830PipeHasType(crtc, I830_OUTPUT_LVDS)) {
	    temp = INREG(PCH_LVDS);
	    if ((temp & LVDS_PORT_EN) == 0) {
		OUTREG(PCH_LVDS, temp | LVDS_PORT_EN);
		INREG(PCH_LVDS);
	    }
	}

	ErrorF("PCH FDI RX PLL enable\n");
	temp = INREG(fdi_rx_reg);
	temp &= ~(0x7 << 16);
	temp |= (pipe_bpc << 11);
	temp &= ~(FDI_DP_PORT_WIDTH_MASK);
	temp |= FDI_DP_PORT_WIDTH_X4;

	OUTREG(fdi_rx_reg, temp | FDI_RX_PLL_ENABLE); /* default 4 lanes */
	INREG(fdi_rx_reg);
	usleep(200);

	/* Switch from Rawclk to PCDclk */
	temp = INREG(fdi_rx_reg);
	OUTREG(fdi_rx_reg, temp | FDI_SEL_PCDCLK); 
	temp = INREG(fdi_rx_reg);
	usleep(200);

	ErrorF("PCH FDI TX PLL enable %08x\n", temp);
	temp = INREG(fdi_tx_reg);
	if ((temp & FDI_TX_PLL_ENABLE) == 0) {
	    OUTREG(fdi_tx_reg, temp | FDI_TX_PLL_ENABLE);
	    INREG(fdi_tx_reg);
	    usleep(100);
	}

	i830WaitForVblank(scrn);


	/* Enable panel fitting for LVDS */
#define PF_FILTER_MASK		(3<<23)
#define PF_FILTER_MED_3x3	(1<<23)
	if (i830PipeHasType(crtc, I830_OUTPUT_LVDS)) {
	    temp = INREG(pf_ctl_reg);
	    temp &= ~PF_FILTER_MASK;
	    OUTREG(pf_ctl_reg, temp | PF_ENABLE | PF_FILTER_MED_3x3);
	    /* currently full aspect */
	    OUTREG(pf_win_pos, 0);
	    OUTREG(pf_win_size, (intel->lvds_fixed_mode->HDisplay << 16) |
		(intel->lvds_fixed_mode->VDisplay));
	}

	ErrorF("Pipe enable\n");
	/* Enable CPU pipe */
	temp = INREG(pipeconf_reg);
	if ((temp & PIPEACONF_ENABLE) == 0) {
	    OUTREG(pipeconf_reg, temp | PIPEACONF_ENABLE);
	    INREG(pipeconf_reg);
	    usleep(100);
	}

	ErrorF("Plane enable\n");
	/* configure and enable CPU plane */
	temp = INREG(dspcntr_reg);
	if ((temp & DISPLAY_PLANE_ENABLE) == 0) {
	    OUTREG(dspcntr_reg, temp | DISPLAY_PLANE_ENABLE);
	    /* Flush the plane changes */
	    OUTREG(dspbase_reg, INREG(dspbase_reg));
	    usleep(10);
	}
	/* twice, like the BIOS */
	OUTREG(dspcntr_reg, temp | DISPLAY_PLANE_ENABLE);

	/* Train FDI. */
	if (IS_GEN6(intel))
		gen6_fdi_link_train(crtc);
	else if (IS_IVYBRIDGE(intel))
		ivb_manual_fdi_link_train(crtc);
	else
		ironlake_fdi_link_train(crtc);

	/* enable PCH DPLL */
	temp = INREG(pch_dpll_reg);
	if ((temp & DPLL_VCO_ENABLE) == 0) {
	    OUTREG(pch_dpll_reg, temp | DPLL_VCO_ENABLE);
	    INREG(pch_dpll_reg);
	}
	usleep(200);

	if (HAS_PCH_CPT(intel)) {
		/* Be sure PCH DPLL SEL is set */
		temp = INREG(PCH_DPLL_SEL);
		if (pipe == 0 && (temp & TRANSA_DPLL_ENABLE) == 0)
			temp |= (TRANSA_DPLL_ENABLE | TRANSA_DPLLA_SEL);
		else if (pipe == 1 && (temp & TRANSB_DPLL_ENABLE) == 0)
			temp |= (TRANSB_DPLL_ENABLE | TRANSB_DPLLB_SEL);
		OUTREG(PCH_DPLL_SEL, temp);
	}

	/* set transcoder timing */
	OUTREG(trans_htot_reg, INREG(cpu_htot_reg));
	OUTREG(trans_hblank_reg, INREG(cpu_hblank_reg));
	OUTREG(trans_hsync_reg, INREG(cpu_hsync_reg));

	OUTREG(trans_vtot_reg, INREG(cpu_vtot_reg));
	OUTREG(trans_vblank_reg, INREG(cpu_vblank_reg));
	OUTREG(trans_vsync_reg, INREG(cpu_vsync_reg));

	/* enable normal train */

	ErrorF("FDI TX link normal\n");
	temp = INREG(fdi_tx_reg);
	if (IS_IVYBRIDGE(intel)) {
		temp &= ~FDI_LINK_TRAIN_NONE_IVB;
		temp |= FDI_LINK_TRAIN_NONE_IVB | FDI_TX_ENHANCE_FRAME_ENABLE;
	} else {
		temp &= ~FDI_LINK_TRAIN_NONE;
		temp |= FDI_LINK_TRAIN_NONE | FDI_TX_ENHANCE_FRAME_ENABLE;
	}
	OUTREG(fdi_tx_reg, temp);
	INREG(fdi_tx_reg);

	temp = INREG(fdi_rx_reg);
        if (HAS_PCH_CPT(intel)) {
                temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
                temp |= FDI_LINK_TRAIN_NORMAL_CPT;
        } else {
                temp &= ~FDI_LINK_TRAIN_NONE;
                temp |= FDI_LINK_TRAIN_NONE;
        }
	OUTREG(fdi_rx_reg, temp | FDI_RX_ENHANCE_FRAME_ENABLE);
	INREG(fdi_rx_reg);

	usleep(100);

	ErrorF("transcoder enable\n");
	/* enable transcoder */
	temp = INREG(transconf_reg);
	temp &= ~PIPE_BPC_MASK;
	temp |= pipe_bpc;
	OUTREG(transconf_reg, temp | TRANS_ENABLE);
	INREG(transconf_reg);

	while ((INREG(transconf_reg) & TRANS_STATE_ENABLE) == 0)
	    ;

	ErrorF("LUT load\n");
	i830_crtc_load_lut(crtc);

	ErrorF("DPMS on done\n");

	i830_modeset_ctl(crtc, 0);
}

void
ironlake_crtc_disable(xf86CrtcPtr crtc)
{
	ScrnInfoPtr scrn = crtc->scrn;
	intel_screen_private *intel = intel_get_screen_private(scrn);
	I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
	int pipe = intel_crtc->pipe;
	int plane = intel_crtc->plane;
	int pch_dpll_reg = (pipe == 0) ? PCH_DPLL_A : PCH_DPLL_B;
	int pipeconf_reg = (pipe == 0) ? PIPEACONF : PIPEBCONF;
	int dspcntr_reg = (plane == 0) ? DSPACNTR : DSPBCNTR;
	int dspbase_reg = (plane == 0) ? DSPABASE : DSPBBASE;
	int fdi_tx_reg = (pipe == 0) ? FDI_TXA_CTL : FDI_TXB_CTL;
	int fdi_rx_reg = (pipe == 0) ? FDI_RXA_CTL : FDI_RXB_CTL;
	int transconf_reg = (pipe == 0) ? TRANSACONF : TRANSBCONF;
	int pf_ctl_reg = (pipe == 0) ? PFA_CTL_1 : PFB_CTL_1;
	int pf_win_size = (pipe == 0) ? PFA_WIN_SZ : PFB_WIN_SZ;
	uint32_t temp, pipe_bpc;
	int n;

	i830_modeset_ctl(crtc, 1);

	temp = INREG(pipeconf_reg);
	pipe_bpc = temp & PIPE_BPC_MASK;

	ErrorF("Plane disable\n");
	temp = INREG(dspcntr_reg);
	if ((temp & DISPLAY_PLANE_ENABLE) != 0) {
		OUTREG(dspcntr_reg, temp & ~DISPLAY_PLANE_ENABLE);
		/* flush plane changes */
		OUTREG(dspbase_reg, INREG(dspbase_reg));
		INREG(dspbase_reg);
	}

	i830_disable_vga_plane (crtc);

	ErrorF("Pipe disable\n");
	/* disable cpu pipe, disable after all planes disabled */
	temp = INREG(pipeconf_reg);
	if ((temp & PIPEACONF_ENABLE) != 0) {
	    OUTREG(pipeconf_reg, temp & ~PIPEACONF_ENABLE);
	    INREG(pipeconf_reg);
	    n = 0;
	    /* wait for cpu pipe off, pipe state */
	    while ((INREG(pipeconf_reg) & I965_PIPECONF_ACTIVE) != 0) {
		n++;
		if (n < 60) {
		    usleep(500);
		    continue;
		} else {
		    ErrorF("aborting pipeconf disable early\n");
		    break;
		}
	    }
	}

	usleep(100);

	ErrorF("PFIT disable\n");
	temp = INREG(pf_ctl_reg);
	if ((temp & PF_ENABLE) != 0) {
		OUTREG(pf_ctl_reg, temp & ~PF_ENABLE);
		INREG(pf_ctl_reg);
	}
	OUTREG(pf_win_size, 0);
	INREG(pf_win_size);

	ErrorF("FDI RX disable\n");
	temp = INREG(fdi_rx_reg);
	temp &= ~(0x07 << 16);
	temp |= pipe_bpc << 11;
	OUTREG(fdi_rx_reg, temp & ~FDI_RX_ENABLE);
	INREG(fdi_rx_reg);

	usleep(100);

	ErrorF("FDI TX disable\n");
	temp = INREG(fdi_tx_reg);
	OUTREG(fdi_tx_reg, temp & ~FDI_TX_ENABLE);
	INREG(fdi_tx_reg);

	usleep(100);

	ErrorF("FDI RX train 1 preload\n");
	temp = INREG(fdi_rx_reg);
	if (HAS_PCH_CPT(intel)) {
		temp &= ~FDI_LINK_TRAIN_PATTERN_MASK_CPT;
		temp |= FDI_LINK_TRAIN_PATTERN_1_CPT;
	} else {
		temp &= ~FDI_LINK_TRAIN_NONE;
		temp |= FDI_LINK_TRAIN_PATTERN_1;
	}
	OUTREG(fdi_rx_reg, temp);
	INREG(fdi_rx_reg);

	usleep(100);

	ErrorF("FDI TX train 1 preload\n");
	/* still set train pattern 1 */
	temp = INREG(fdi_tx_reg);
	temp &= ~FDI_LINK_TRAIN_NONE;
	temp |= FDI_LINK_TRAIN_PATTERN_1;
	OUTREG(fdi_tx_reg, temp);
	INREG(fdi_tx_reg);

	usleep(100);

	if (i830PipeHasType(crtc, I830_OUTPUT_LVDS)) {
	    ErrorF("LVDS port force off\n");
	    while ((temp = INREG(PCH_LVDS)) & PORT_ENABLE) {
		OUTREG(PCH_LVDS, temp & ~LVDS_PORT_EN);
		INREG(PCH_LVDS);
		usleep(100);
	    }
	}

	ErrorF("Transcoder disable\n");
	/* disable PCH transcoder */
	temp = INREG(transconf_reg);
	if ((temp & TRANS_STATE_ENABLE) != 0) {
	    OUTREG(transconf_reg, temp & ~TRANS_ENABLE);
	    INREG(transconf_reg);
	    n = 0;
	    /* wait for PCH transcoder off, transcoder state */
	    while ((INREG(transconf_reg) & TRANS_STATE_ENABLE) != 0) {
		n++;
		if (n < 600) {
		    usleep(500);
		    continue;
		} else {
		    ErrorF("aborting transcoder disable early, 0x%08x\n", INREG(transconf_reg));
		    break;
		}
	    }
	}

	temp = INREG(transconf_reg);
	temp &= ~PIPE_BPC_MASK;
	temp |= pipe_bpc;
	OUTREG(transconf_reg, temp);
	INREG(transconf_reg);
	usleep(100);

	if (HAS_PCH_CPT(intel)) {
		/* disable DPLL_SEL */
		temp = INREG(PCH_DPLL_SEL);
		switch (pipe) {
		case 0:
			temp &= ~(TRANSA_DPLL_ENABLE | TRANSA_DPLLB_SEL);
			break;
		case 1:
			temp &= ~(TRANSB_DPLL_ENABLE | TRANSB_DPLLB_SEL);
			break;
		case 2:
			/* C shares PLL A or B */
			temp &= ~(TRANSC_DPLL_ENABLE | TRANSC_DPLLB_SEL);
			break;
		}
		OUTREG(PCH_DPLL_SEL, temp);
		INREG(PCH_DPLL_SEL);
	}

	ErrorF("PCH DPLL disable\n");
	/* disable PCH DPLL */
	temp = INREG(pch_dpll_reg);
	OUTREG(pch_dpll_reg, temp & ~DPLL_VCO_ENABLE);
	INREG(pch_dpll_reg);

	ErrorF("FDI RX PLL PCD disable\n");
	/* Switch from PCDclk to Rawclk */
	temp = INREG(fdi_rx_reg);
	temp &= ~FDI_SEL_PCDCLK;
	OUTREG(fdi_rx_reg, temp);
	temp = INREG(fdi_rx_reg);

	ErrorF("FDI TX PLL disable %08x\n", temp);
	temp = INREG(fdi_tx_reg);
	temp &= ~FDI_TX_PLL_ENABLE;
	OUTREG(fdi_tx_reg, temp);
	temp = INREG(fdi_tx_reg);
	usleep(100);

	ErrorF("FDI RX PLL disable %08x\n", temp);
	temp = INREG(fdi_tx_reg);
	OUTREG(fdi_tx_reg, temp & ~FDI_TX_PLL_ENABLE);
	INREG(fdi_tx_reg);
	usleep(100);

	ErrorF("DPMS off done\n");
	/* Wait for the clocks to turn off. */
	usleep(150);
}

static void ironlake_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;

    if (intel_crtc->pipe != intel_crtc->plane)
	FatalError("pipe/plane mismatch, aborting\n");

    /* XXX: When our outputs are all unaware of DPMS modes other than off
     * and on, we should map those modes to DRM_MODE_DPMS_OFF in the CRTC.
     */
    switch (mode) {
    case DPMSModeOn:
    case DPMSModeStandby:
    case DPMSModeSuspend:
	ironlake_crtc_enable(crtc);
	break;
    case DPMSModeOff:
	ironlake_crtc_disable(crtc);
	intel_crtc->enabled = FALSE;
	break;
    }
}

/**
 * Sets the power management mode of the pipe and plane.
 *
 * This code should probably grow support for turning the cursor off and back
 * on appropriately at the same time as we're turning the pipe off/on.
 */
static void
i9xx_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    int pipe = intel_crtc->pipe;
    Bool disable_pipe = TRUE;

    /* XXX: When our outputs are all unaware of DPMS modes other than off and
     * on, we should map those modes to DPMSModeOff in the CRTC.
     */
    switch (mode) {
    case DPMSModeOn:
    case DPMSModeStandby:
    case DPMSModeSuspend:
	i830_crtc_enable(crtc);
	break;
    case DPMSModeOff:
	if ((pipe == 0) && (intel->quirk_flag & QUIRK_PIPEA_FORCE))
	    disable_pipe = FALSE;
	i830_crtc_disable(crtc, disable_pipe);
	intel_crtc->enabled = FALSE;
	break;
    }
}

static void
i830_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;

    if (HAS_PCH_SPLIT(intel))
	ironlake_crtc_dpms(crtc, mode);
    else
	i9xx_crtc_dpms(crtc, mode);

    intel_crtc->dpms_mode = mode;
}

static Bool
i830_crtc_lock (xf86CrtcPtr crtc)
{
    /* Sync the engine before mode switch, to finish any outstanding
     * WAIT_FOR_EVENTS that may rely on CRTC state.
     */
    intel_sync(crtc->scrn);

    return FALSE;
}

static void
i830_crtc_unlock (xf86CrtcPtr crtc)
{
}

static void
i830_crtc_prepare (xf86CrtcPtr crtc)
{
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    if (intel_crtc->enabled)
	crtc->funcs->hide_cursor (crtc);
    crtc->funcs->dpms (crtc, DPMSModeOff);
}

static void
i830_crtc_commit (xf86CrtcPtr crtc)
{
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    Bool		deactivate = FALSE;

    if (!intel_crtc->enabled && intel_crtc->pipe != 0)
	deactivate = i830_pipe_a_require_activate (crtc->scrn);
    
    intel_crtc->enabled = TRUE;
    
    crtc->funcs->dpms (crtc, DPMSModeOn);
    if (crtc->scrn->pScreen != NULL)
	xf86_reload_cursors (crtc->scrn->pScreen);
    if (deactivate)
	i830_pipe_a_require_deactivate (crtc->scrn);
}

void
i830_output_prepare (xf86OutputPtr output)
{
    output->funcs->dpms (output, DPMSModeOff);
}

void
i830_output_commit (xf86OutputPtr output)
{
    output->funcs->dpms (output, DPMSModeOn);
}

static Bool
i830_crtc_mode_fixup(xf86CrtcPtr crtc, DisplayModePtr mode,
		     DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);

    if (HAS_PCH_SPLIT(intel)) {
	/* FDI link clock is fixed at 2.7G */
	if (mode->Clock * 3 > 27000 * 4)
	    return MODE_CLOCK_HIGH;
    }

    return TRUE;
}

/** Returns the core display clock speed for i830 - i945 */
static int
i830_get_core_clock_speed(ScrnInfoPtr scrn)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);

    /* Core clock values taken from the published datasheets.
     * The 830 may go up to 166 Mhz, which we should check.
     */
    if (IS_I945G(intel) || (IS_G33CLASS(intel) && !IS_IGDGM(intel)))
	return 400000;
    else if (IS_I915G(intel))
	return 333000;
    else if (IS_I945GM(intel) || IS_845G(intel) || IS_IGDGM(intel))
	return 200000;
    else if (IS_I915GM(intel)) {
	uint16_t gcfgc;

      pci_device_cfg_read_u16 (intel->PciInfo, &gcfgc, I915_GCFGC);
      if (gcfgc & I915_LOW_FREQUENCY_ENABLE)
	    return 133000;
	else {
	    switch (gcfgc & I915_DISPLAY_CLOCK_MASK) {
	    case I915_DISPLAY_CLOCK_333_MHZ:
		return 333000;
	    default:
	    case I915_DISPLAY_CLOCK_190_200_MHZ:
		return 190000;
	    }
	}
    } else if (IS_I865G(intel))
	return 266000;
    else if (IS_I855(intel)) {
        struct pci_device *bridge = intel_host_bridge ();
	uint16_t hpllcc;
	pci_device_cfg_read_u16 (bridge, &hpllcc, I855_HPLLCC);

	/* Assume that the hardware is in the high speed state.  This
	 * should be the default.
	 */
	switch (hpllcc & I855_CLOCK_CONTROL_MASK) {
	case I855_CLOCK_133_200:
	case I855_CLOCK_100_200:
	    return 200000;
	case I855_CLOCK_166_250:
	    return 250000;
	case I855_CLOCK_100_133:
	    return 133000;
	}
    } else /* 852, 830 */
	return 133000;

    return 0; /* Silence gcc warning */
}

/**
 * Return the pipe currently connected to the panel fitter,
 * or -1 if the panel fitter is not present or not in use
 */
static int
i830_panel_fitter_pipe(intel_screen_private *intel)
{
    uint32_t pfit_control;

    /* i830 doesn't have a panel fitter */
    if (IS_I830(intel))
	return -1;

    pfit_control = INREG(PFIT_CONTROL);

    /* See if the panel fitter is in use */
    if ((pfit_control & PFIT_ENABLE) == 0)
	return -1;

    /* 965 can place panel fitter on either pipe */
    if (IS_I965G(intel))
	return (pfit_control & PFIT_PIPE_MASK) >> PFIT_PIPE_SHIFT;

    /* older chips can only use pipe 1 */
    return 1;
}

/**
 * Sets up the DSPARB register to split the display fifo appropriately between
 * the display planes.
 *
 * Adjusting this register requires that the planes be off.
 */
static void
i830_update_dsparb(ScrnInfoPtr scrn)
{
   xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
   intel_screen_private *intel = intel_get_screen_private(scrn);
   int total_hdisplay = 0, planea_hdisplay = 0, planeb_hdisplay = 0;
   int fifo_entries = 0, planea_entries = 0, planeb_entries = 0, i;

   if ((INREG(DSPACNTR) & DISPLAY_PLANE_ENABLE) &&
       (INREG(DSPBCNTR) & DISPLAY_PLANE_ENABLE))
       xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		  "tried to update DSPARB with both planes enabled!\n");

  /*
    * FIFO entries will be split based on programmed modes
    */
   if (IS_I965GM(intel))
       fifo_entries = 127;
   else if (IS_I9XX(intel))
       fifo_entries = 95;
   else if (IS_MOBILE(intel)) {
       fifo_entries = 255;
   } else {
	/* The 845/865 only have a AEND field.  Though the field size would
	* allow 128 entries, the 865 rendered the cursor wrong then.
	* The BIOS set it up for 96.
	*/
	fifo_entries = 95;
   }

   for (i = 0; i < xf86_config->num_crtc; i++) {
      xf86CrtcPtr crtc = xf86_config->crtc[i];
      I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
      if (crtc->enabled) {
	  total_hdisplay += crtc->mode.HDisplay;
	  if (intel_crtc->plane == 0)
	      planea_hdisplay = crtc->mode.HDisplay;
	  else
	      planeb_hdisplay = crtc->mode.HDisplay;
      }
   }

   planea_entries = fifo_entries * planea_hdisplay / total_hdisplay;
   planeb_entries = fifo_entries * planeb_hdisplay / total_hdisplay;

   if (IS_I9XX(intel))
       OUTREG(DSPARB,
	      ((planea_entries + planeb_entries) << DSPARB_CSTART_SHIFT) |
	      (planea_entries << DSPARB_BSTART_SHIFT));
   else if (IS_MOBILE(intel))
       OUTREG(DSPARB,
	      ((planea_entries + planeb_entries) << DSPARB_BEND_SHIFT) |
	      (planea_entries << DSPARB_AEND_SHIFT));
   else
       OUTREG(DSPARB, planea_entries << DSPARB_AEND_SHIFT);
}

struct fdi_m_n {
    CARD32        tu;
    CARD32        gmch_m;
    CARD32        gmch_n;
    CARD32        link_m;
    CARD32        link_n;
};

static void
fdi_reduce_ratio(CARD32 *num, CARD32 *den)
{
    while (*num > 0xffffff || *den > 0xffffff) {
	*num >>= 1;
	*den >>= 1;
    }
}

#define DATA_N 0x800000
#define LINK_N 0x80000

static void
igdng_compute_m_n(int bits_per_pixel, int nlanes,
		  int pixel_clock, int link_clock,
		  struct fdi_m_n *m_n)
{
    uint64_t temp;

    m_n->tu = 64; /* default size */

    temp = (uint64_t) DATA_N * pixel_clock;
    temp = temp / link_clock;
    m_n->gmch_m = (temp * bits_per_pixel) / nlanes;
    m_n->gmch_m >>= 3; /* convert to bytes per pixel */
    m_n->gmch_n = DATA_N;
    fdi_reduce_ratio(&m_n->gmch_m, &m_n->gmch_n);

    temp = (uint64_t) LINK_N * pixel_clock;
    m_n->link_m = temp / link_clock;
    m_n->link_n = LINK_N;
    fdi_reduce_ratio(&m_n->link_m, &m_n->link_n);
}

/**
 * Sets up registers for the given mode/adjusted_mode pair.
 *
 * The clocks, CRTCs and outputs attached to this CRTC must be off.
 *
 * This shouldn't enable any clocks, CRTCs, or outputs, but they should
 * be easily turned on/off after this.
 */
static void
i830_crtc_mode_set(xf86CrtcPtr crtc, DisplayModePtr mode,
		   DisplayModePtr adjusted_mode,
		   int x, int y)
{
    ScrnInfoPtr scrn = crtc->scrn;
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    I830OutputPrivatePtr intel_output;
    int pipe = intel_crtc->pipe;
    int plane = intel_crtc->plane;
    int fp_reg = (pipe == 0) ? FPA0 : FPB0;
    int dpll_reg = (pipe == 0) ? DPLL_A : DPLL_B;
    int dpll_md_reg = (pipe == 0) ? DPLL_A_MD : DPLL_B_MD;
    int pipeconf_reg = (pipe == 0) ? PIPEACONF : PIPEBCONF;
    int htot_reg = (pipe == 0) ? HTOTAL_A : HTOTAL_B;
    int hblank_reg = (pipe == 0) ? HBLANK_A : HBLANK_B;
    int hsync_reg = (pipe == 0) ? HSYNC_A : HSYNC_B;
    int vtot_reg = (pipe == 0) ? VTOTAL_A : VTOTAL_B;
    int vblank_reg = (pipe == 0) ? VBLANK_A : VBLANK_B;
    int vsync_reg = (pipe == 0) ? VSYNC_A : VSYNC_B;
    int pipesrc_reg = (pipe == 0) ? PIPEASRC : PIPEBSRC;
    int dspcntr_reg = (plane == 0) ? DSPACNTR : DSPBCNTR;
    int dsppos_reg = (plane == 0) ? DSPAPOS : DSPBPOS;
    int dspsize_reg = (plane == 0) ? DSPASIZE : DSPBSIZE;
    int i, num_outputs = 0;
    int refclk;
    intel_clock_t clock;
    uint32_t dpll = 0, fp = 0, dspcntr, pipeconf, lvds_bits = 0;
    Bool ok, is_sdvo = FALSE, is_dvo = FALSE;
    Bool is_crt = FALSE, is_lvds = FALSE, is_tv = FALSE;
    const intel_limit_t *limit;

    struct fdi_m_n m_n = {0};
    int data_m1_reg = (pipe == 0) ? PIPEA_DATA_M1 : PIPEB_DATA_M1;
    int data_n1_reg = (pipe == 0) ? PIPEA_DATA_N1 : PIPEB_DATA_N1;
    int link_m1_reg = (pipe == 0) ? PIPEA_LINK_M1 : PIPEB_LINK_M1;
    int link_n1_reg = (pipe == 0) ? PIPEA_LINK_N1 : PIPEB_LINK_N1;
    int pch_fp_reg = (pipe == 0) ? PCH_FPA0 : PCH_FPB0;
    int pch_dpll_reg = (pipe == 0) ? PCH_DPLL_A : PCH_DPLL_B;
    int fdi_rx_reg = (pipe == 0) ? FDI_RXA_CTL : FDI_RXB_CTL;
    int fdi_tx_reg = (pipe == 0) ? FDI_TXA_CTL : FDI_TXB_CTL;
    int lvds_reg = LVDS;
    uint32_t temp;
    int sdvo_pixel_multiply;

    /* Set up some convenient bools for what outputs are connected to
     * our pipe, used in DPLL setup.
     */
    for (i = 0; i < xf86_config->num_output; i++) {
	xf86OutputPtr  output = xf86_config->output[i];
	intel_output = output->driver_private;

	if (output->crtc != crtc)
	    continue;

	switch (intel_output->type) {
	case I830_OUTPUT_LVDS:
	    is_lvds = TRUE;
	    lvds_bits = intel_output->lvds_bits;
	    break;
	case I830_OUTPUT_SDVO:
	case I830_OUTPUT_HDMI:
	    is_sdvo = TRUE;
	    if (intel_output->needs_tv_clock)
		is_tv = TRUE;
	    break;
	case I830_OUTPUT_DVO_TMDS:
	case I830_OUTPUT_DVO_LVDS:
	case I830_OUTPUT_DVO_TVOUT:
	    is_dvo = TRUE;
	    break;
	case I830_OUTPUT_TVOUT:
	    is_tv = TRUE;
	    break;
	case I830_OUTPUT_ANALOG:
	    is_crt = TRUE;
	    break;
	}

	num_outputs++;
    }

    if (num_outputs > 1)
	xf86DrvMsg(scrn->scrnIndex, X_INFO, "clone detected, disabling SSC\n");

    /* Don't use SSC when cloned */
    if (is_lvds && intel->lvds_use_ssc && num_outputs < 2) {
	refclk = intel->lvds_ssc_freq * 1000;
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "using SSC reference clock of %d MHz\n", refclk / 1000);
    } else if (IS_I9XX(intel)) {
	refclk = 96000;
	if (HAS_PCH_SPLIT(intel))
	    refclk = 120000; /* 120Mhz refclk */
    } else {
	refclk = 48000;
    }

    /*
     * Returns a set of divisors for the desired target clock with the given
     * refclk, or FALSE.  The returned values represent the clock equation:
     * reflck * (5 * (m1 + 2) + (m2 + 2)) / (n + 2) / p1 / p2.
     */
    limit = intel_limit (crtc);
    ok = limit->find_pll(limit, crtc, adjusted_mode->Clock, refclk, &clock);
    if (!ok)
	FatalError("Couldn't find PLL settings for mode!\n");

    if (fabs(adjusted_mode->Clock - clock.dot) / clock.dot > .02) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		   "Chosen PLL clock of %.1f Mhz more than 2%% away from "
		   "desired %.1f Mhz\n",
		   (float)clock.dot / 1000,
		   (float)adjusted_mode->Clock / 1000);
    }

    /* SDVO TV has fixed PLL values depends on its clock range,
       this mirrors vbios setting. */
    if (is_sdvo && is_tv) {
	if (adjusted_mode->Clock >= 100000 &&
		adjusted_mode->Clock < 140500) {
	    clock.p1 = 2;
	    clock.p2 = 10;
	    clock.n = 3;
	    clock.m1 = 16;
	    clock.m2 = 8;
	} else if (adjusted_mode->Clock >= 140500 &&
		adjusted_mode->Clock <= 200000) {
	    clock.p1 = 1;
	    clock.p2 = 10;
	    clock.n = 6;
	    clock.m1 = 12;
	    clock.m2 = 8;
	}
    }

    if (HAS_PCH_SPLIT(intel)) {
	int bpp = 24;
	if (is_lvds) {
	    uint32_t lvds_reg = INREG(PCH_LVDS);

	    if (!((lvds_reg & LVDS_A3_POWER_MASK) == LVDS_A3_POWER_UP))
		bpp = 18;
	}

	igdng_compute_m_n(bpp, 4, /* lane num 4 */
			  adjusted_mode->Clock,
			  270000, /* lane clock */
			  &m_n);
	ErrorF("bpp %d\n", bpp / 3);
	intel_crtc->bpc = bpp / 3;
    }

    if (HAS_PCH_SPLIT(intel)) {
	uint32_t temp;

	temp = INREG(PCH_DREF_CONTROL);
	/* Always enable nonspread source */
	temp &= ~DREF_NONSPREAD_SOURCE_MASK;
	temp |= DREF_NONSPREAD_SOURCE_ENABLE;
	OUTREG(PCH_DREF_CONTROL, temp);
	temp = INREG(PCH_DREF_CONTROL);

	temp &= ~DREF_SSC_SOURCE_MASK;
	temp |= DREF_SSC_SOURCE_ENABLE;
	OUTREG(PCH_DREF_CONTROL, temp);
	temp = INREG(PCH_DREF_CONTROL);
    }

    if (IS_IGD(intel))
	fp = (1 << clock.n) << 16 | clock.m1 << 8 | clock.m2;
    else
	fp = clock.n << 16 | clock.m1 << 8 | clock.m2;

    if (!HAS_PCH_SPLIT(intel))
	dpll = DPLL_VGA_MODE_DIS;
    if (IS_I9XX(intel)) {
	if (is_lvds)
	    dpll |= DPLLB_MODE_LVDS;
	else
	    dpll |= DPLLB_MODE_DAC_SERIAL;
	if (is_sdvo)
	{
	    dpll |= DPLL_DVO_HIGH_SPEED;
	    sdvo_pixel_multiply = adjusted_mode->Clock / mode->Clock;
	    if ((IS_I945G(intel) || IS_I945GM(intel) || IS_G33CLASS(intel)))
		dpll |= (sdvo_pixel_multiply - 1) << SDVO_MULTIPLIER_SHIFT_HIRES;
	    else if (HAS_PCH_SPLIT(intel))
		dpll |= (sdvo_pixel_multiply - 1) << PLL_REF_SDVO_HDMI_MULTIPLIER_SHIFT;
		
	}
	
	/* compute bitmask from p1 value */
	if (IS_IGD(intel))
	    dpll |= (1 << (clock.p1 - 1)) << DPLL_FPA01_P1_POST_DIV_SHIFT_IGD;
	else
	    dpll |= (1 << (clock.p1 - 1)) << DPLL_FPA01_P1_POST_DIV_SHIFT;
	switch (clock.p2) {
	case 5:
	    dpll |= DPLL_DAC_SERIAL_P2_CLOCK_DIV_5;
	    break;
	case 7:
	    dpll |= DPLLB_LVDS_P2_CLOCK_DIV_7;
	    break;
	case 10:
	    dpll |= DPLL_DAC_SERIAL_P2_CLOCK_DIV_10;
	    break;
	case 14:
	    dpll |= DPLLB_LVDS_P2_CLOCK_DIV_14;
	    break;
	}
	if (IS_I965G(intel) && !IS_GM45(intel) && !HAS_PCH_SPLIT(intel))
	    dpll |= (6 << PLL_LOAD_PULSE_PHASE_SHIFT);
    } else {
	if (is_lvds) {
	    dpll |= (1 << (clock.p1 - 1)) << DPLL_FPA01_P1_POST_DIV_SHIFT;
	} else {
	    if (clock.p1 == 2)
		dpll |= PLL_P1_DIVIDE_BY_TWO;
	    else
		dpll |= (clock.p1 - 2) << DPLL_FPA01_P1_POST_DIV_SHIFT;
	    if (clock.p2 == 4)
		dpll |= PLL_P2_DIVIDE_BY_4;
	}
    }

    if (is_sdvo && is_tv)
	dpll |= PLL_REF_INPUT_TVCLKINBC;
    else if (is_tv)
    {
	/* XXX: just matching BIOS for now */
/*	dpll |= PLL_REF_INPUT_TVCLKINBC; */
	dpll |= 3;
    }
    else if (is_lvds && intel->lvds_use_ssc && num_outputs < 2)
	dpll |= PLLB_REF_INPUT_SPREADSPECTRUMIN;
    else
	dpll |= PLL_REF_INPUT_DREFCLK;

    /* Set up the display plane register */
    dspcntr = DISPPLANE_GAMMA_ENABLE;
    /* this is "must be enabled" in the docs, but not set by bios */
    if (HAS_PCH_SPLIT(intel))
	dspcntr |= DISPPLANE_TRICKLE_FEED_DISABLE;

    switch (scrn->bitsPerPixel) {
    case 8:
	dspcntr |= DISPPLANE_8BPP;
	break;
    case 16:
	if (scrn->depth == 15)
	    dspcntr |= DISPPLANE_15_16BPP;
	else
	    dspcntr |= DISPPLANE_16BPP;
	break;
    case 32:
	dspcntr |= DISPPLANE_32BPP_NO_ALPHA;
	break;
    default:
	FatalError("unknown display bpp\n");
    }

    /* Ironlake's plane is forced to pipe, bit 24 is to
       enable color space conversion */
    if (!HAS_PCH_SPLIT(intel)) {
	if (pipe == 0)
	    dspcntr |= DISPPLANE_SEL_PIPE_A;
	else
	    dspcntr |= DISPPLANE_SEL_PIPE_B;
    }

    if (IS_I965G(intel) && i830_display_tiled(crtc))
	dspcntr |= DISPLAY_PLANE_TILED;

    pipeconf = INREG(pipeconf_reg);
    if (pipe == 0 && !IS_I965G(intel))
    {
	/* Enable pixel doubling when the dot clock is > 90% of the (display)
	 * core speed.
	 *
	 * XXX: No double-wide on 915GM pipe B. Is that the only reason for the
	 * pipe == 0 check?
	 */
	if (mode->Clock > i830_get_core_clock_speed(scrn) * 9 / 10)
	    pipeconf |= PIPEACONF_DOUBLE_WIDE;
	else
	    pipeconf &= ~PIPEACONF_DOUBLE_WIDE;
    }

    if (HAS_PCH_SPLIT(intel)) {
	pipeconf &= ~(7 << 5);
	if (intel_crtc->bpc == 6)
	    pipeconf |= (1 << 6); /* 0 is 8bpc */
	if (intel_crtc->bpc != 8)
	    pipeconf |= (1 << 4); /* enable dithering */
    }

    /*
     * This "shouldn't" be needed as the dpms on code
     * will be run after the mode is set. On 9xx, it helps.
     * On 855, it can lock up the chip (and the entire machine)
     */
    if (!IS_I85X (intel) && !HAS_PCH_SPLIT(intel))
    {
	dspcntr |= DISPLAY_PLANE_ENABLE;
	pipeconf |= PIPEACONF_ENABLE;
	dpll |= DPLL_VCO_ENABLE;
    }
    
    /* Disable the panel fitter if it was on our pipe */
    if (!HAS_PCH_SPLIT(intel) && i830_panel_fitter_pipe (intel) == pipe)
	OUTREG(PFIT_CONTROL, 0);

    if (intel->debug_modes) {
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "Mode for pipe %c:\n", pipe == 0 ? 'A' : 'B');
	xf86PrintModeline(scrn->scrnIndex, mode);
	if (!xf86ModesEqual(mode, adjusted_mode)) {
	    xf86DrvMsg(scrn->scrnIndex, X_INFO,
		       "Adjusted mode for pipe %c:\n", pipe == 0 ? 'A' : 'B');
	    xf86PrintModeline(scrn->scrnIndex, adjusted_mode);
	}
	i830PrintPll(scrn, "chosen", &clock);
    }

    /* assign to PCH registers */
    if (HAS_PCH_SPLIT(intel)) {
	fp_reg = pch_fp_reg;
	dpll_reg = pch_dpll_reg;
    }

    if (dpll & DPLL_VCO_ENABLE)
    {
	OUTREG(fp_reg, fp);
	OUTREG(dpll_reg, dpll & ~DPLL_VCO_ENABLE);
	POSTING_READ(dpll_reg);
	usleep(150);
    }

    /* The LVDS pin pair needs to be on before the DPLLs are enabled.
     * This is an exception to the general rule that mode_set doesn't turn
     * things on.
     */
    if (is_lvds)
    {
	uint32_t lvds;

	if (HAS_PCH_SPLIT(intel))
	    lvds_reg = PCH_LVDS;

	lvds = INREG(lvds_reg);
	lvds |= LVDS_PORT_EN | LVDS_A0A2_CLKA_POWER_UP;
	if (HAS_PCH_CPT(intel)) {
	    lvds &= ~PORT_TRANS_SEL_MASK;
	    lvds |= PORT_TRANS_SEL_CPT(pipe);
	} else {
	    if (pipe == 1)
		lvds |= LVDS_PIPEB_SELECT;	
	    else
		lvds &= ~LVDS_PIPEB_SELECT;
	}

	/* Set the B0-B3 data pairs corresponding to whether we're going to
	 * set the DPLLs for dual-channel mode or not.
	 */
	if (clock.p2 == I9XX_P2_LVDS_FAST)
	    lvds |= LVDS_B0B3_POWER_UP | LVDS_CLKB_POWER_UP;
	else
	    lvds &= ~(LVDS_B0B3_POWER_UP | LVDS_CLKB_POWER_UP);

	if (intel->lvds_24_bit_mode) {
	    /* Option set which requests 24-bit mode
	     * (LVDS_A3_POWER_UP, as opposed to 18-bit mode) here; we
	     * still need to look more thoroughly into how panels
	     * behave in the two modes.  This option enables that
	     * experimentation.
	     */
	    xf86DrvMsg(scrn->scrnIndex, X_INFO,
		       "Selecting less common 24 bit TMDS pixel format.\n");
	    lvds |= LVDS_A3_POWER_UP;
	    lvds |= LVDS_DATA_FORMAT_DOT_ONE;
	} else {
	    xf86DrvMsg(scrn->scrnIndex, X_INFO,
		       "Selecting standard 18 bit TMDS pixel format.\n");
	}

	/* Enable dithering if we're in 18-bit mode. */
	if (IS_I965G(intel))
	{
	    if ((lvds & LVDS_A3_POWER_MASK) == LVDS_A3_POWER_UP)
		lvds &= ~LVDS_DITHER_ENABLE;
	    else
		lvds |= LVDS_DITHER_ENABLE;
	}

	lvds |= lvds_bits;

	OUTREG(lvds_reg, lvds);
	POSTING_READ(lvds_reg);
    }

    if (HAS_PCH_SPLIT(intel)) {
	    /* For non-DP output, clear any trans DP clock recovery setting.*/
	    if (pipe == 0) {
		    OUTREG(TRANSA_DATA_M1, 0);
		    OUTREG(TRANSA_DATA_N1, 0);
		    OUTREG(TRANSA_DP_LINK_M1, 0);
		    OUTREG(TRANSA_DP_LINK_N1, 0);
	    } else {
		    OUTREG(TRANSB_DATA_M1, 0);
		    OUTREG(TRANSB_DATA_N1, 0);
		    OUTREG(TRANSB_DP_LINK_M1, 0);
		    OUTREG(TRANSB_DP_LINK_N1, 0);
	    }
    }

    OUTREG(fp_reg, fp);
/*    OUTREG(fp_reg + 4, fp); RHEL had this... wtf? */
    OUTREG(dpll_reg, dpll);
    POSTING_READ(dpll_reg);
    /* Wait for the clocks to stabilize. */
    usleep(150);
    
    if (IS_I965G(intel) && !HAS_PCH_SPLIT(intel)) {
	int sdvo_pixel_multiply = adjusted_mode->Clock / mode->Clock;
	OUTREG(dpll_md_reg, (0 << DPLL_MD_UDI_DIVIDER_SHIFT) |
	       ((sdvo_pixel_multiply - 1) << DPLL_MD_UDI_MULTIPLIER_SHIFT));
    } else {
	/* write it again -- the BIOS does, after all */
	OUTREG(dpll_reg, dpll);
    }
    POSTING_READ(dpll_reg);
    /* Wait for the clocks to stabilize. */
    usleep(150);

    if (!DSPARB_HWCONTROL(intel))
	i830_update_dsparb(scrn);

    OUTREG(htot_reg, (adjusted_mode->CrtcHDisplay - 1) |
	((adjusted_mode->CrtcHTotal - 1) << 16));
    OUTREG(hblank_reg, (adjusted_mode->CrtcHBlankStart - 1) |
	((adjusted_mode->CrtcHBlankEnd - 1) << 16));
    OUTREG(hsync_reg, (adjusted_mode->CrtcHSyncStart - 1) |
	((adjusted_mode->CrtcHSyncEnd - 1) << 16));
    OUTREG(vtot_reg, (adjusted_mode->CrtcVDisplay - 1) |
	((adjusted_mode->CrtcVTotal - 1) << 16));

    OUTREG(vblank_reg, (adjusted_mode->CrtcVBlankStart - 1) |
	((adjusted_mode->CrtcVBlankEnd - 1) << 16));
    OUTREG(vsync_reg, (adjusted_mode->CrtcVSyncStart - 1) |
	((adjusted_mode->CrtcVSyncEnd - 1) << 16));
    /* pipesrc and dspsize control the size that is scaled from, which should
     * always be the user's requested size.
     */
    if (!HAS_PCH_SPLIT(intel)) {
	    OUTREG(dspsize_reg, ((mode->VDisplay - 1) << 16) | (mode->HDisplay - 1));
	    OUTREG(dsppos_reg, 0);
    }
    OUTREG(pipesrc_reg, ((mode->HDisplay - 1) << 16) | (mode->VDisplay - 1));

    if (HAS_PCH_SPLIT(intel)) {
	OUTREG(data_m1_reg, TU_SIZE(m_n.tu) | m_n.gmch_m);
	OUTREG(data_n1_reg, m_n.gmch_n);
	OUTREG(link_m1_reg, m_n.link_m);
	OUTREG(link_n1_reg, m_n.link_n);

	/* enable FDI RX PLL too */
	temp = INREG(fdi_rx_reg);
	OUTREG(fdi_rx_reg, temp | FDI_RX_PLL_ENABLE);
	INREG(fdi_rx_reg);
	usleep(200);

	temp = INREG(fdi_tx_reg);
	OUTREG(fdi_tx_reg, temp | FDI_TX_PLL_ENABLE);
	INREG(fdi_tx_reg);

	temp = INREG(fdi_rx_reg);
	OUTREG(fdi_rx_reg, temp | FDI_RX_PLL_ENABLE);
	INREG(fdi_rx_reg);
	usleep(200);
    }

    OUTREG(pipeconf_reg, pipeconf);
    POSTING_READ(pipeconf_reg);
    i830WaitForVblank(scrn);

    if (IS_GEN5(intel)) {
	/* enable address swizzle for tiling buffer */
	temp = INREG(DISP_ARB_CTL);
	OUTREG(DISP_ARB_CTL, temp | DISP_TILE_SURFACE_SWIZZLING);
    }

    OUTREG(dspcntr_reg, dspcntr);
    /* Flush the plane changes */
    i830PipeSetBase(crtc, x, y);

    i830WaitForVblank(scrn);
}


/** Loads the palette/gamma unit for the CRTC with the prepared values */
static void
i830_crtc_load_lut(xf86CrtcPtr crtc)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    int palreg = (intel_crtc->pipe == 0) ? PALETTE_A : PALETTE_B;
    int i;

    /* The clocks have to be on to load the palette. */
    if (!crtc->enabled)
	return;

    /* use legacy palette for Ironlake */
    if (HAS_PCH_SPLIT(intel))
	palreg = (intel_crtc->pipe == 0) ? LGC_PALETTE_A : LGC_PALETTE_B;

    for (i = 0; i < 256; i++) {
	OUTREG(palreg + 4 * i,
	       (intel_crtc->lut_r[i] << 16) |
	       (intel_crtc->lut_g[i] << 8) |
	       intel_crtc->lut_b[i]);
    }
}

/** Sets the color ramps on behalf of RandR */
static void
i830_crtc_gamma_set(xf86CrtcPtr crtc, CARD16 *red, CARD16 *green, CARD16 *blue,
		    int size)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    int i;

    assert(size == 256);

    for (i = 0; i < 256; i++) {
	intel_crtc->lut_r[i] = red[i] >> 8;
	intel_crtc->lut_g[i] = green[i] >> 8;
	intel_crtc->lut_b[i] = blue[i] >> 8;
    }

    /*
     * 855 at least really doesn't seem like like you poking its
     * pallette registers other than at mode set time. so just disable
     * this for now on 8xx. Stops hard machine lockups for me.
     */
    if (IS_I9XX(intel))
	i830_crtc_load_lut(crtc);
}

/**
 * Allocates memory for a locked-in-framebuffer shadow of the given
 * width and height for this CRTC's rotated shadow framebuffer.
 */
 
static void *
i830_crtc_shadow_allocate (xf86CrtcPtr crtc, int width, int height)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    unsigned long rotate_pitch;
    uint32_t tiling;

    assert(intel_crtc->rotate_bo == NULL);
    intel_crtc->rotate_bo = intel_allocate_framebuffer(scrn, width, height,
        intel->cpp, &rotate_pitch, &tiling);
    if (intel_crtc->rotate_bo == NULL) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		   "Couldn't allocate shadow memory for rotated CRTC\n");
	return NULL;
    }

    intel_crtc->rotate_pitch = rotate_pitch;
    return intel_crtc->rotate_bo;
}
    
/**
 * Creates a pixmap for this CRTC's rotated shadow framebuffer.
 */
static PixmapPtr
i830_crtc_shadow_create(xf86CrtcPtr crtc, void *data, int width, int height)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    PixmapPtr rotate_pixmap;

    if (!data) {
	data = i830_crtc_shadow_allocate (crtc, width, height);
	if (!data) {
	    xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		"Couldn't allocate shadow pixmap for rotated CRTC\n");
		return NULL;
	}
    }

    if (intel_crtc->rotate_bo == NULL) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		   "Couldn't allocate shadow pixmap for rotated CRTC\n");
	return NULL;
    }
    
    rotate_pixmap = GetScratchPixmapHeader(scrn->pScreen,
					   width, height,
					   scrn->depth,
					   scrn->bitsPerPixel,
					   intel_crtc->rotate_pitch,
					   NULL);

    if (rotate_pixmap == NULL) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		   "Couldn't allocate shadow pixmap for rotated CRTC\n");
        return NULL;
    }
    intel_set_pixmap_bo(rotate_pixmap, intel_crtc->rotate_bo);

    intel->shadow_present = TRUE;

    return rotate_pixmap;
}

static void
i830_crtc_shadow_destroy(xf86CrtcPtr crtc, PixmapPtr rotate_pixmap, void *data)
{
    ScrnInfoPtr scrn = crtc->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;

    if (rotate_pixmap) {
	intel_set_pixmap_bo(rotate_pixmap, NULL);
	FreeScratchPixmapHeader(rotate_pixmap);
    }

    if (data) {
	/* Be sure to sync acceleration before the memory gets unbound. */
	intel_sync(scrn);
	if (scrn->vtSema)
		(void)dri_bo_unpin(intel_crtc->rotate_bo);
	drm_intel_bo_unreference(intel_crtc->rotate_bo);
	intel_crtc->rotate_bo = NULL;
    }

    intel->shadow_present = intel->use_shadow;
}

#if RANDR_13_INTERFACE
static void
i830_crtc_set_origin(xf86CrtcPtr crtc, int x, int y)
{
    if (crtc->enabled)
	i830PipeSetBase(crtc, x, y);
}
#endif

/* The screen bo has changed, reset each active crtc to point at
 * the same location that it currently points at, but in the new bo
 */
void
i830_set_new_crtc_bo(ScrnInfoPtr scrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    int			i;

    for (i = 0; i < xf86_config->num_crtc; i++) {
	xf86CrtcPtr crtc = xf86_config->crtc[i];

	if (crtc->enabled && !crtc->transform_in_use)
	    i830PipeSetBase(crtc, crtc->x, crtc->y);
    }
}

void
i830DescribeOutputConfiguration(ScrnInfoPtr scrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    intel_screen_private *intel = intel_get_screen_private(scrn);
    int i;

    xf86DrvMsg(scrn->scrnIndex, X_INFO, "Output configuration:\n");

    for (i = 0; i < xf86_config->num_crtc; i++) {
	xf86CrtcPtr crtc = xf86_config->crtc[i];
	I830CrtcPrivatePtr intel_crtc = crtc ? crtc->driver_private : NULL;
	uint32_t dspcntr = intel_crtc->plane == 0 ? INREG(DSPACNTR) :
	    INREG(DSPBCNTR);
	uint32_t pipeconf = i == 0 ? INREG(PIPEACONF) :
	    INREG(PIPEBCONF);
	Bool hw_plane_enable = (dspcntr & DISPLAY_PLANE_ENABLE) != 0;
	Bool hw_pipe_enable = (pipeconf & PIPEACONF_ENABLE) != 0;
	int pipe;

	if (HAS_PCH_SPLIT(intel))
	    pipe = intel_crtc->plane;
	else
	    pipe = !!(dspcntr & DISPPLANE_SEL_PIPE_MASK);

	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "  Pipe %c is %s\n",
		   'A' + i, crtc->enabled ? "on" : "off");
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "  Display plane %c is now %s and connected to pipe %c.\n",
		   'A' + intel_crtc->plane,
		   hw_plane_enable ? "enabled" : "disabled",
		   'A' + pipe);
	if (hw_pipe_enable != crtc->enabled) {
	    xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		       "  Hardware claims pipe %c is %s while software "
		       "believes it is %s\n",
		       'A' + i, hw_pipe_enable ? "on" : "off",
		       crtc->enabled ? "on" : "off");
	}
	if (hw_plane_enable != crtc->enabled) {
	    xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		       "  Hardware claims plane %c is %s while software "
		       "believes it is %s\n",
		       'A' + i, hw_plane_enable ? "on" : "off",
		       crtc->enabled ? "on" : "off");
	}
    }

    for (i = 0; i < xf86_config->num_output; i++) {
	xf86OutputPtr	output = xf86_config->output[i];
	xf86CrtcPtr	crtc = output->crtc;
	I830CrtcPrivatePtr	intel_crtc = crtc ? crtc->driver_private : NULL;
	
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "  Output %s is connected to pipe %s\n",
		   output->name, intel_crtc == NULL ? "none" :
		   (intel_crtc->pipe == 0 ? "A" : "B"));
    }
}

/**
 * Get a pipe with a simple mode set on it for doing load-based monitor
 * detection.
 *
 * It will be up to the load-detect code to adjust the pipe as appropriate for
 * its requirements.  The pipe will be connected to no other outputs.
 *
 * Currently this code will only succeed if there is a pipe with no outputs
 * configured for it.  In the future, it could choose to temporarily disable
 * some outputs to free up a pipe for its use.
 *
 * \return crtc, or NULL if no pipes are available.
 */
    
/* VESA 640x480x72Hz mode to set on the pipe */
static DisplayModeRec   load_detect_mode = {
    NULL, NULL, "640x480", MODE_OK, M_T_DEFAULT,
    31500,
    640, 664, 704, 832, 0,
    480, 489, 491, 520, 0,
    V_NHSYNC | V_NVSYNC,
    0, 0,

    640, 640, 664, 704, 832, 832, 0,
    480, 489, 489, 491, 520, 520,
    FALSE, FALSE, 0, NULL, 0, 0.0, 0.0
};

xf86CrtcPtr
i830GetLoadDetectPipe(xf86OutputPtr output, DisplayModePtr mode, int *dpms_mode)
{
    ScrnInfoPtr		    scrn = output->scrn;
    xf86CrtcConfigPtr	    xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    I830CrtcPrivatePtr	    intel_crtc;
    xf86CrtcPtr		    supported_crtc =NULL;
    xf86CrtcPtr		    crtc = NULL;
    int			    i;

    if (output->crtc) 
    {
	crtc = output->crtc;
	/*
	 * Make sure the crtc and output are running
	 */
	intel_crtc = crtc->driver_private;
	*dpms_mode = intel_crtc->dpms_mode;
	if (intel_crtc->dpms_mode != DPMSModeOn)
	{
	    crtc->funcs->dpms (crtc, DPMSModeOn);
	    output->funcs->dpms (output, DPMSModeOn);
	}
	return crtc;
    }

    for (i = 0; i < xf86_config->num_crtc; i++)
    {
	xf86CrtcPtr possible_crtc;
	if (!(output->possible_crtcs & (1 << i)))
	    continue;
	possible_crtc = xf86_config->crtc[i];
	if (!possible_crtc->enabled)
	{
	    crtc = possible_crtc;
	    break;
	}
	if (!supported_crtc)
	    supported_crtc = possible_crtc;
    }
    if (!crtc)
    {
	crtc = supported_crtc;
	if (!crtc)
	    return NULL;
    }

    output->crtc = crtc;
    intel_output->load_detect_temp = TRUE;
    
    intel_crtc = crtc->driver_private;
    *dpms_mode = intel_crtc->dpms_mode;

    if (!crtc->enabled)
    {
	if (!mode)
	    mode = &load_detect_mode;
	xf86CrtcSetMode (crtc, mode, RR_Rotate_0, 0, 0);
    }
    else
    {
	if (intel_crtc->dpms_mode != DPMSModeOn)
	    crtc->funcs->dpms (crtc, DPMSModeOn);

	/* Add this output to the crtc */
	output->funcs->mode_set (output, &crtc->mode, &crtc->mode);
	output->funcs->commit (output);
    }
    /* let the output get through one full cycle before testing */
    i830WaitForVblank (scrn);

    return crtc;
}

void
i830ReleaseLoadDetectPipe(xf86OutputPtr output, int dpms_mode)
{
    ScrnInfoPtr		    scrn = output->scrn;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    xf86CrtcPtr		    crtc = output->crtc;
    
    if (intel_output->load_detect_temp) 
    {
	output->crtc = NULL;
	intel_output->load_detect_temp = FALSE;
	crtc->enabled = xf86CrtcInUse (crtc);
	xf86DisableUnusedFunctions(scrn);
    }
    /*
     * Switch crtc and output back off if necessary
     */
    if (crtc->enabled && dpms_mode != DPMSModeOn)
    {
	if (output->crtc == crtc)
	    output->funcs->dpms (output, dpms_mode);
	crtc->funcs->dpms (crtc, dpms_mode);
    }
}

/* Returns the clock of the currently programmed mode of the given pipe. */
static int
i830_crtc_clock_get(ScrnInfoPtr scrn, xf86CrtcPtr crtc)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    int pipe = intel_crtc->pipe;
    uint32_t dpll = INREG((pipe == 0) ? DPLL_A : DPLL_B);
    uint32_t fp;
    intel_clock_t clock;

    if ((dpll & DISPLAY_RATE_SELECT_FPA1) == 0)
	fp = INREG((pipe == 0) ? FPA0 : FPB0);
    else
	fp = INREG((pipe == 0) ? FPA1 : FPB1);

    clock.m1 = (fp & FP_M1_DIV_MASK) >> FP_M1_DIV_SHIFT;
    if (IS_IGD(intel)) {
	clock.n = ffs((fp & FP_N_IGD_DIV_MASK) >> FP_N_DIV_SHIFT) - 1;
	clock.m2 = (fp & FP_M2_IGD_DIV_MASK) >> FP_M2_DIV_SHIFT;
    } else {
	clock.n = (fp & FP_N_DIV_MASK) >> FP_N_DIV_SHIFT;
	clock.m2 = (fp & FP_M2_DIV_MASK) >> FP_M2_DIV_SHIFT;
    }
    if (IS_I9XX(intel)) {
	if (IS_IGD(intel))
	    clock.p1 = ffs((dpll & DPLL_FPA01_P1_POST_DIV_MASK_IGD) >>
			   DPLL_FPA01_P1_POST_DIV_SHIFT_IGD);
	else
	    clock.p1 = ffs((dpll & DPLL_FPA01_P1_POST_DIV_MASK) >>
			   DPLL_FPA01_P1_POST_DIV_SHIFT);

	switch (dpll & DPLL_MODE_MASK) {
	case DPLLB_MODE_DAC_SERIAL:
	    clock.p2 = dpll & DPLL_DAC_SERIAL_P2_CLOCK_DIV_5 ? 5 : 10;
	    break;
	case DPLLB_MODE_LVDS:
	    clock.p2 = dpll & DPLLB_LVDS_P2_CLOCK_DIV_7 ? 7 : 14;
	    break;
	default:
	    xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		       "Unknown DPLL mode %08x in programmed mode\n",
		       (int)(dpll & DPLL_MODE_MASK));
	    return 0;
	}

	if ((dpll & PLL_REF_INPUT_MASK) == PLLB_REF_INPUT_SPREADSPECTRUMIN)
	    intel_clock(intel, 100000, &clock);
	else
	    intel_clock(intel, 96000, &clock);
    } else {
	Bool is_lvds = (pipe == 1) && (INREG(LVDS) & LVDS_PORT_EN);

	if (is_lvds) {
	    clock.p1 = ffs((dpll & DPLL_FPA01_P1_POST_DIV_MASK_I830_LVDS) >>
			   DPLL_FPA01_P1_POST_DIV_SHIFT);

	    /* if LVDS is dual-channel, p2 = 7 */
	    if ((INREG(LVDS) & LVDS_CLKB_POWER_MASK) == LVDS_CLKB_POWER_UP)
		clock.p2 = 7;
	    else
		clock.p2 = 14;

	    if ((dpll & PLL_REF_INPUT_MASK) == PLLB_REF_INPUT_SPREADSPECTRUMIN)
		intel_clock(intel, 66000, &clock); /* XXX: might not be 66MHz */
	    else
		intel_clock(intel, 48000, &clock);
	} else {
	    if (dpll & PLL_P1_DIVIDE_BY_TWO) {
		clock.p1 = 2;
	    } else {
		clock.p1 = ((dpll & DPLL_FPA01_P1_POST_DIV_MASK_I830) >>
		    DPLL_FPA01_P1_POST_DIV_SHIFT) + 2;
	    }
	    if (dpll & PLL_P2_DIVIDE_BY_4)
		clock.p2 = 4;
	    else
		clock.p2 = 2;

	    intel_clock(intel, 48000, &clock);
	}
    }

    /* XXX: It would be nice to validate the clocks, but we can't reuse
     * i830PllIsValid() because it relies on the xf86_config output
     * configuration being accurate, which it isn't necessarily.
     */
    if (0)
	i830PrintPll(scrn, "probed", &clock);

    return clock.dot;
}

/** Returns the currently programmed mode of the given pipe. */
DisplayModePtr
i830_crtc_mode_get(ScrnInfoPtr scrn, xf86CrtcPtr crtc)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    int pipe = intel_crtc->pipe;
    DisplayModePtr mode;
    int htot = INREG((pipe == 0) ? HTOTAL_A : HTOTAL_B);
    int hsync = INREG((pipe == 0) ? HSYNC_A : HSYNC_B);
    int vtot = INREG((pipe == 0) ? VTOTAL_A : VTOTAL_B);
    int vsync = INREG((pipe == 0) ? VSYNC_A : VSYNC_B);

    mode = calloc(1, sizeof(DisplayModeRec));
    if (mode == NULL)
	return NULL;

    mode->Clock = i830_crtc_clock_get(scrn, crtc);
    mode->HDisplay = (htot & 0xffff) + 1;
    mode->HTotal = ((htot & 0xffff0000) >> 16) + 1;
    mode->HSyncStart = (hsync & 0xffff) + 1;
    mode->HSyncEnd = ((hsync & 0xffff0000) >> 16) + 1;
    mode->VDisplay = (vtot & 0xffff) + 1;
    mode->VTotal = ((vtot & 0xffff0000) >> 16) + 1;
    mode->VSyncStart = (vsync & 0xffff) + 1;
    mode->VSyncEnd = ((vsync & 0xffff0000) >> 16) + 1;
    xf86SetModeDefaultName(mode);
    xf86SetModeCrtc(mode, 0);

    return mode;
}

static const xf86CrtcFuncsRec i830_crtc_funcs = {
    .dpms = i830_crtc_dpms,
    .save = NULL, /* XXX */
    .restore = NULL, /* XXX */
    .lock = i830_crtc_lock,
    .unlock = i830_crtc_unlock,
    .mode_fixup = i830_crtc_mode_fixup,
    .prepare = i830_crtc_prepare,
    .mode_set = i830_crtc_mode_set,
    .commit = i830_crtc_commit,
    .gamma_set = i830_crtc_gamma_set,
    .shadow_create = i830_crtc_shadow_create,
    .shadow_allocate = i830_crtc_shadow_allocate,
    .shadow_destroy = i830_crtc_shadow_destroy,
    .set_cursor_colors = i830_crtc_set_cursor_colors,
    .set_cursor_position = i830_crtc_set_cursor_position,
    .show_cursor = i830_crtc_show_cursor,
    .hide_cursor = i830_crtc_hide_cursor,
    .load_cursor_argb = i830_crtc_load_cursor_argb,
    .destroy = NULL, /* XXX */
#if RANDR_13_INTERFACE
    .set_origin = i830_crtc_set_origin,
#endif
};

void
i830_crtc_init(ScrnInfoPtr scrn, int pipe)
{
    xf86CrtcPtr crtc;
    I830CrtcPrivatePtr intel_crtc;
    int i;

    crtc = xf86CrtcCreate (scrn, &i830_crtc_funcs);
    if (crtc == NULL)
	return;

    intel_crtc = xnfcalloc (sizeof (I830CrtcPrivateRec), 1);
    intel_crtc->pipe = pipe;
    intel_crtc->dpms_mode = DPMSModeOff;
    intel_crtc->plane = pipe;

    /* Initialize the LUTs for when we turn on the CRTC. */
    for (i = 0; i < 256; i++) {
	intel_crtc->lut_r[i] = i;
	intel_crtc->lut_g[i] = i;
	intel_crtc->lut_b[i] = i;
    }
    crtc->driver_private = intel_crtc;
}

