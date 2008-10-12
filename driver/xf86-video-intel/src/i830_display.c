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
#include "i830.h"
#include "i830_bios.h"
#include "i830_display.h"
#include "i830_debug.h"
#include "xf86Modes.h"

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

typedef struct {
    intel_range_t   dot, vco, n, m, m1, m2, p, p1;
    intel_p2_t	    p2;
} intel_limit_t;

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

/* Haven't found any reason to go this fast, but newer chips support it */
#define I96X_VCO_MAX		3200000

/*
 * These values are taken from the broadwater/crestline PLL spreadsheet.
 * All of the defines here are for the programmed register value, not
 * the 'counter' value (e.g. Ncounter = Nregister + 2)
 */
#define I9XX_N_MIN		      1
#define I9XX_N_MAX		      6
#define I9XX_M_MIN		     70
#define I9XX_M_MAX		    120

/* these two come from the calm1 macro */
#define I9XX_M1_MIN		     10
#define I9XX_M1_MAX		     22
#define I9XX_M2_MIN		      5
#define I9XX_M2_MAX		      9

#define I9XX_P_SDVO_DAC_MIN	      5
#define I9XX_P_SDVO_DAC_MAX	     80
#define I9XX_P_LVDS_MIN		      7
#define I9XX_P_LVDS_MAX		     98
#define I9XX_P1_MIN		      1
#define I9XX_P1_MAX		      8
#define I9XX_P2_SDVO_DAC_SLOW		     10
#define I9XX_P2_SDVO_DAC_FAST		      5
#define I9XX_P2_SDVO_DAC_SLOW_LIMIT	 200000
#define I9XX_P2_LVDS_SLOW		     14
#define I9XX_P2_LVDS_FAST		      7
#define I9XX_P2_LVDS_SLOW_LIMIT		 112000

#define INTEL_LIMIT_I8XX_DVO_DAC    0
#define INTEL_LIMIT_I8XX_LVDS	    1
#define INTEL_LIMIT_I9XX_SDVO_DAC   2
#define INTEL_LIMIT_I9XX_LVDS	    3

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
    },
};

static const intel_limit_t *intel_limit (xf86CrtcPtr crtc)
{
    ScrnInfoPtr	pScrn = crtc->scrn;
    I830Ptr	pI830 = I830PTR(pScrn);
    const intel_limit_t *limit;

    if (IS_I9XX(pI830)) {
	if (i830PipeHasType (crtc, I830_OUTPUT_LVDS))
	    limit = &intel_limits[INTEL_LIMIT_I9XX_LVDS];
	else
	    limit = &intel_limits[INTEL_LIMIT_I9XX_SDVO_DAC];
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

static void intel_clock(I830Ptr pI830, int refclk, intel_clock_t *clock)
{
    if (IS_I9XX(pI830))
	i9xx_clock (refclk, clock);
    else
	i8xx_clock (refclk, clock);
}

static void
i830PrintPll(ScrnInfoPtr pScrn, char *prefix, intel_clock_t *clock)
{
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "%s: dotclock %d vco %d ((m %d, m1 %d, m2 %d), n %d, "
	       "(p %d, p1 %d, p2 %d))\n",
	       prefix, clock->dot, clock->vco,
	       clock->m, clock->m1, clock->m2,
	       clock->n,
	       clock->p, clock->p1, clock->p2);
}

/**
 * Returns whether any output on the specified pipe is of the specified type
 */
Bool
i830PipeHasType (xf86CrtcPtr crtc, int type)
{
    ScrnInfoPtr	pScrn = crtc->scrn;
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
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

#define i830PllInvalid(s)   { /* ErrorF (s) */; return FALSE; }
/**
 * Returns whether the given set of divisors are valid for a given refclk with
 * the given outputs.
 */

static Bool
i830PllIsValid(xf86CrtcPtr crtc, intel_clock_t *clock)
{
    const intel_limit_t *limit = intel_limit (crtc);

    if (clock->p1  < limit->p1.min  || limit->p1.max  < clock->p1)
	i830PllInvalid ("p1 out of range\n");
    if (clock->p   < limit->p.min   || limit->p.max   < clock->p)
	i830PllInvalid ("p out of range\n");
    if (clock->m2  < limit->m2.min  || limit->m2.max  < clock->m2)
	i830PllInvalid ("m2 out of range\n");
    if (clock->m1  < limit->m1.min  || limit->m1.max  < clock->m1)
	i830PllInvalid ("m1 out of range\n");
    if (clock->m1 <= clock->m2)
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

/**
 * Returns a set of divisors for the desired target clock with the given
 * refclk, or FALSE.  The returned values represent the clock equation:
 * reflck * (5 * (m1 + 2) + (m2 + 2)) / (n + 2) / p1 / p2.
 */
static Bool
i830FindBestPLL(xf86CrtcPtr crtc, int target, int refclk, intel_clock_t *best_clock)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    intel_clock_t   clock;
    const intel_limit_t   *limit = intel_limit (crtc);
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
	for (clock.m2 = limit->m2.min; clock.m2 < clock.m1 && clock.m2 <= limit->m2.max; clock.m2++) 
	{
	    for (clock.n = limit->n.min; clock.n <= limit->n.max; clock.n++) 
	    {
		for (clock.p1 = limit->p1.min; clock.p1 <= limit->p1.max; clock.p1++) 
		{
		    int this_err;

		    intel_clock (pI830, refclk, &clock);
		    
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

void
i830WaitForVblank(ScrnInfoPtr pScreen)
{
    /* Wait for 20ms, i.e. one cycle at 50hz. */
    usleep(30000);
}

void
i830PipeSetBase(xf86CrtcPtr crtc, int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    int pipe = intel_crtc->pipe;
    int plane = intel_crtc->plane;
    unsigned long Start, Offset;
    int dspbase = (plane == 0 ? DSPABASE : DSPBBASE);
    int dspsurf = (plane == 0 ? DSPASURF : DSPBSURF);
    int dsptileoff = (plane == 0 ? DSPATILEOFF : DSPBTILEOFF);

    Offset = ((y * pScrn->displayWidth + x) * pI830->cpp);
    if (pI830->front_buffer == NULL) {
	/* During startup we may be called as part of monitor detection while
	 * there is no memory allocation done, so just supply a dummy base
	 * address.
	 */
	Start = 0;
    } else if (crtc->rotatedData != NULL) {
	/* offset is done by shadow painting code, not here */
	Start = (char *)crtc->rotatedData - (char *)pI830->FbBase;
	Offset = 0;
    } else if (I830IsPrimary(pScrn)) {
	Start = pI830->front_buffer->offset;
    } else {
	I830Ptr pI8301 = I830PTR(pI830->entityPrivate->pScrn_1);
	Start = pI8301->front_buffer_2->offset;
    }

    if (IS_I965G(pI830)) {
        OUTREG(dspbase, Offset);
	POSTING_READ(dspbase);
        OUTREG(dspsurf, Start);
	POSTING_READ(dspsurf);
	OUTREG(dsptileoff, (y << 16) | x);
    } else {
	OUTREG(dspbase, Start + Offset);
	POSTING_READ(dspbase);
    }

#ifdef XF86DRI
    if (pI830->directRenderingEnabled) {
	drmI830Sarea *sPriv = (drmI830Sarea *) DRIGetSAREAPrivate(pScrn->pScreen);

	if (!sPriv)
	    return;

	switch (plane) {
	case 0:
	    sPriv->planeA_x = x;
	    sPriv->planeA_y = y;
	    break;
	case 1:
	    sPriv->planeB_x = x;
	    sPriv->planeB_y = y;
	    break;
	default:
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Can't update pipe %d in SAREA\n", pipe);
	    break;
	}
    }
#endif
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
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);

    /* Rotated data is currently linear, allocated either via XAA or EXA */
    if (crtc->rotatedData)
	return FALSE;

    if (pI830->front_buffer && pI830->front_buffer->tiling != TILE_NONE)
	return TRUE;

    return FALSE;
}

/*
 * Several restrictions:
 *   - DSP[AB]CNTR - no line duplication && no pixel multiplier
 *   - pixel format == 15 bit, 16 bit, or 32 bit xRGB_8888
 *   - no alpha buffer discard
 *   - no dual wide display
 *   - progressive mode only (DSP[AB]CNTR)
 *   - uncompressed fb is <= 2048 in width, 0 mod 8
 *   - uncompressed fb is <= 1536 in height, 0 mod 2
 *   - SR display watermarks must be equal between 16bpp and 32bpp?
 *
 * FIXME: verify above conditions are true
 *
 * Enable 8xx style FB compression
 */
static void
i830_enable_fb_compression_8xx(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    uint32_t fbc_ctl = 0;
    unsigned long compressed_stride;
    int plane = (intel_crtc->plane == 0 ? FBC_CTL_PLANEA : FBC_CTL_PLANEB);
    unsigned long uncompressed_stride = pScrn->displayWidth * pI830->cpp;
    unsigned long interval = 1000;

    if (INREG(FBC_CONTROL) & FBC_CTL_EN)
	return;

    compressed_stride = pI830->compressed_front_buffer->size /
	FBC_LL_SIZE;

    if (uncompressed_stride < compressed_stride)
	compressed_stride = uncompressed_stride;

    /* FBC_CTL wants 64B units */
    compressed_stride = (compressed_stride / 64) - 1;

    /* Set it up... */
    /* Wait for compressing bit to clear */
    while (INREG(FBC_STATUS) & FBC_STAT_COMPRESSING)
	; /* nothing */
    i830WaitForVblank(pScrn);
    OUTREG(FBC_CFB_BASE, pI830->compressed_front_buffer->bus_addr);
    OUTREG(FBC_LL_BASE, pI830->compressed_ll_buffer->bus_addr + 6);
    OUTREG(FBC_CONTROL2, FBC_CTL_FENCE_DBL | FBC_CTL_IDLE_IMM |
	   FBC_CTL_CPU_FENCE | plane);
    OUTREG(FBC_FENCE_OFF, crtc->y);

    /* Zero buffers */
    memset(pI830->FbBase + pI830->compressed_front_buffer->offset, 0,
	   pI830->compressed_front_buffer->size);
    memset(pI830->FbBase + pI830->compressed_ll_buffer->offset, 0,
	   pI830->compressed_ll_buffer->size);

    /* enable it... */
    fbc_ctl |= FBC_CTL_EN | FBC_CTL_PERIODIC;
    fbc_ctl |= (compressed_stride & 0xff) << FBC_CTL_STRIDE_SHIFT;
    fbc_ctl |= (interval & 0x2fff) << FBC_CTL_INTERVAL_SHIFT;
    fbc_ctl |= FBC_CTL_UNCOMPRESSIBLE;
    fbc_ctl |= pI830->front_buffer->fence_nr;
    OUTREG(FBC_CONTROL, fbc_ctl);
}

/*
 * Disable 8xx style FB compression
 */
static void
i830_disable_fb_compression_8xx(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    uint32_t fbc_ctl;

    /* Disable compression */
    fbc_ctl = INREG(FBC_CONTROL);
    fbc_ctl &= ~FBC_CTL_EN;
    OUTREG(FBC_CONTROL, fbc_ctl);

    /* Wait for compressing bit to clear */
    while (INREG(FBC_STATUS) & FBC_STAT_COMPRESSING)
	; /* nothing */
}

static void
i830_disable_fb_compression2(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    uint32_t dpfc_ctl;

    /* Disable compression */
    dpfc_ctl = INREG(DPFC_CONTROL);
    dpfc_ctl &= ~DPFC_CTL_EN;
    OUTREG(DPFC_CONTROL, dpfc_ctl);
    i830WaitForVblank(pScrn);
}

static void
i830_enable_fb_compression2(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    int plane = (intel_crtc->plane == 0 ? DPFC_CTL_PLANEA : DPFC_CTL_PLANEB);
    unsigned long stall_watermark = 200, frames = 50;

    if (INREG(DPFC_CONTROL) & DPFC_CTL_EN)
	return;

    /* Set it up... */
    i830_disable_fb_compression2(crtc);
    OUTREG(DPFC_CB_BASE, pI830->compressed_front_buffer->offset);
    /* Update i830_memory.c too if compression ratio changes */
    OUTREG(DPFC_CONTROL, plane | DPFC_CTL_FENCE_EN | DPFC_CTL_LIMIT_4X |
	   pI830->front_buffer->fence_nr);
    OUTREG(DPFC_RECOMP_CTL, DPFC_RECOMP_STALL_EN |
	   (stall_watermark << DPFC_RECOMP_STALL_WM_SHIFT) |
	   (frames << DPFC_RECOMP_TIMER_COUNT_SHIFT));
    OUTREG(DPFC_FENCE_YOFF, crtc->y);

    /* Zero buffers */
    memset(pI830->FbBase + pI830->compressed_front_buffer->offset, 0,
	   pI830->compressed_front_buffer->size);

    /* enable it... */
    OUTREG(DPFC_CONTROL, INREG(DPFC_CONTROL) | DPFC_CTL_EN);
}

static void
i830_enable_fb_compression(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);

    if (IS_GM45(pI830))
	return i830_enable_fb_compression2(crtc);

    i830_enable_fb_compression_8xx(crtc);
}

static void
i830_disable_fb_compression(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);

    if (IS_GM45(pI830))
	return i830_disable_fb_compression2(crtc);

    i830_disable_fb_compression_8xx(crtc);
}

static Bool
i830_use_fb_compression(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    I830Ptr pI830 = I830PTR(pScrn);
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    unsigned long uncompressed_size;
    int plane = (intel_crtc->plane == 0 ? FBC_CTL_PLANEA : FBC_CTL_PLANEB);
    int i, count = 0;

    /* Only available on one pipe at a time */
    for (i = 0; i < xf86_config->num_crtc; i++) {
	if (xf86_config->crtc[i]->enabled)
	    count++;
    }

    /* Here we disable it to catch one->two pipe enabled configs */
    if (count > 1) {
	if (i830_fb_compression_supported(pI830))
	    i830_disable_fb_compression(crtc);
	return FALSE;
    }

    if (!pI830->fb_compression)
	return FALSE;

    if (!i830_display_tiled(crtc))
	return FALSE;

    /* Pre-965 only supports plane A */
    if (!IS_I965GM(pI830) && plane != FBC_CTL_PLANEA)
	return FALSE;

    /* Need 15, 16, or 32 (w/alpha) pixel format */
    if (!(pScrn->bitsPerPixel == 16 || /* covers 15 bit mode as well */
	  pScrn->bitsPerPixel == 32)) /* mode_set dtrt if fbc is in use */
	return FALSE;

    /* Can't cache more lines than we can track */
    if (crtc->mode.VDisplay > FBC_LL_SIZE)
	return FALSE;

    /*
     * Make sure the compressor doesn't go past the end of our compressed
     * buffer if the uncompressed size is large.
     */
    uncompressed_size = crtc->mode.HDisplay * crtc->mode.VDisplay *
	pI830->cpp;
    if (pI830->compressed_front_buffer->size < uncompressed_size)
	return FALSE;

    /*
     * No checks for pixel multiply, incl. horizontal, or interlaced modes
     * since they're currently unused.
     */
    return TRUE;
}

#if defined(DRM_IOCTL_MODESET_CTL) && defined(XF86DRI)
static void i830_modeset_ctl(xf86CrtcPtr crtc, int pre)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    struct drm_modeset_ctl modeset;

    if (!pI830->directRenderingEnabled)
	return;

    modeset.crtc = intel_crtc->plane;

    /*
     * DPMS will be called many times (especially off), but we only
     * want to catch the transition from on->off and off->on.
     */
    if (pre && intel_crtc->dpms_mode != DPMSModeOff) {
	/* On -> off is a pre modeset */
	modeset.cmd = _DRM_PRE_MODESET;
	ioctl(pI830->drmSubFD, DRM_IOCTL_MODESET_CTL, &modeset);
    } else if (!pre && intel_crtc->dpms_mode == DPMSModeOff) {
	/* Off -> on means post modeset */
	modeset.cmd = _DRM_POST_MODESET;
	ioctl(pI830->drmSubFD, DRM_IOCTL_MODESET_CTL, &modeset);
    }
}
#else
static void i830_modeset_ctl(xf86CrtcPtr crtc, int dpms_state)
{
    return;
}
#endif /* DRM_IOCTL_MODESET_CTL && XF86DRI */

/**
 * Sets the power management mode of the pipe and plane.
 *
 * This code should probably grow support for turning the cursor off and back
 * on appropriately at the same time as we're turning the pipe off/on.
 */
static void
i830_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    int pipe = intel_crtc->pipe;
    int plane = intel_crtc->plane;
    int dpll_reg = (pipe == 0) ? DPLL_A : DPLL_B;
    int pipeconf_reg = (pipe == 0) ? PIPEACONF : PIPEBCONF;
    int dspcntr_reg = (plane == 0) ? DSPACNTR : DSPBCNTR;
    int dspbase_reg = (plane == 0) ? DSPABASE : DSPBBASE;
    uint32_t temp;

    /* XXX: When our outputs are all unaware of DPMS modes other than off and
     * on, we should map those modes to DPMSModeOff in the CRTC.
     */
    switch (mode) {
    case DPMSModeOn:
    case DPMSModeStandby:
    case DPMSModeSuspend:
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

	/* Reenable compression if needed */
	if (i830_use_fb_compression(crtc))
	    i830_enable_fb_compression(crtc);
	i830_modeset_ctl(crtc, 0);
	break;
    case DPMSModeOff:
	i830_modeset_ctl(crtc, 1);
	/* Shut off compression if in use */
	if (i830_use_fb_compression(crtc))
	    i830_disable_fb_compression(crtc);

	/* Give the overlay scaler a chance to disable if it's on this pipe */
	i830_crtc_dpms_video(crtc, FALSE);

	/* Disable the VGA plane that we never use */
	OUTREG(VGACNTRL, VGA_DISP_DISABLE);

	/* May need to leave pipe A on */
	if ((pipe != 0) || !(pI830->quirk_flag & QUIRK_PIPEA_FORCE))
	{
		/* Disable display plane */
		temp = INREG(dspcntr_reg);
		if ((temp & DISPLAY_PLANE_ENABLE) != 0)
		{
		    OUTREG(dspcntr_reg, temp & ~DISPLAY_PLANE_ENABLE);
		    /* Flush the plane changes */
		    OUTREG(dspbase_reg, INREG(dspbase_reg));
		    POSTING_READ(dspbase_reg);
		}

		if (!IS_I9XX(pI830)) {
		    /* Wait for vblank for the disable to take effect */
		    i830WaitForVblank(pScrn);
		}

		/* Next, disable display pipes */
		temp = INREG(pipeconf_reg);
		if ((temp & PIPEACONF_ENABLE) != 0) {
		    OUTREG(pipeconf_reg, temp & ~PIPEACONF_ENABLE);
		    POSTING_READ(pipeconf_reg);
		}

		/* Wait for vblank for the disable to take effect. */
		i830WaitForVblank(pScrn);

		temp = INREG(dpll_reg);
		if ((temp & DPLL_VCO_ENABLE) != 0) {
		    OUTREG(dpll_reg, temp & ~DPLL_VCO_ENABLE);
		    POSTING_READ(dpll_reg);
		}
	}
	/* Wait for the clocks to turn off. */
	usleep(150);
	break;
    }

    intel_crtc->dpms_mode = mode;

#ifdef XF86DRI
    if (pI830->directRenderingEnabled) {
	drmI830Sarea *sPriv = (drmI830Sarea *) DRIGetSAREAPrivate(pScrn->pScreen);
	Bool enabled = crtc->enabled && mode != DPMSModeOff;

	I830DRISetVBlankInterrupt (pScrn, TRUE);

	if (!sPriv)
	    return;

	switch (plane) {
	case 0:
	    sPriv->planeA_w = enabled ? crtc->mode.HDisplay : 0;
	    sPriv->planeA_h = enabled ? crtc->mode.VDisplay : 0;
	    break;
	case 1:
	    sPriv->planeB_w = enabled ? crtc->mode.HDisplay : 0;
	    sPriv->planeB_h = enabled ? crtc->mode.VDisplay : 0;
	    break;
	default:
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Can't update pipe %d in SAREA\n", pipe);
	    break;
	}
    }
#endif
}

static Bool
i830_crtc_lock (xf86CrtcPtr crtc)
{
   /* Sync the engine before mode switch */
   i830WaitSync(crtc->scrn);

#ifdef XF86DRI
    return I830DRILock(crtc->scrn);
#else
    return FALSE;
#endif
}

static void
i830_crtc_unlock (xf86CrtcPtr crtc)
{
#ifdef XF86DRI
    I830DRIUnlock (crtc->scrn);
#endif
}

static void
i830_crtc_prepare (xf86CrtcPtr crtc)
{
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    /* Temporarily turn off FB compression during modeset */
    if (i830_use_fb_compression(crtc))
        i830_disable_fb_compression(crtc);
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

    /* Reenable FB compression if possible */
    if (i830_use_fb_compression(crtc))
	i830_enable_fb_compression(crtc);
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
    return TRUE;
}

/** Returns the core display clock speed for i830 - i945 */
static int
i830_get_core_clock_speed(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);

    /* Core clock values taken from the published datasheets.
     * The 830 may go up to 166 Mhz, which we should check.
     */
    if (IS_I945G(pI830) || IS_G33CLASS(pI830))
	return 400000;
    else if (IS_I915G(pI830))
	return 333000;
    else if (IS_I945GM(pI830) || IS_845G(pI830))
	return 200000;
    else if (IS_I915GM(pI830)) {
	uint16_t gcfgc;

#if XSERVER_LIBPCIACCESS
      pci_device_cfg_read_u16 (pI830->PciInfo, &gcfgc, I915_GCFGC);
#else
      gcfgc = pciReadWord(pI830->PciTag, I915_GCFGC);
#endif
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
    } else if (IS_I865G(pI830))
	return 266000;
    else if (IS_I855(pI830)) {
#if XSERVER_LIBPCIACCESS
        struct pci_device *bridge = intel_host_bridge ();
	uint16_t hpllcc;
	pci_device_cfg_read_u16 (bridge, &hpllcc, I855_HPLLCC);
#else
	PCITAG bridge = pciTag(0, 0, 0); /* This is always the host bridge */
	uint16_t hpllcc = pciReadWord(bridge, I855_HPLLCC);
#endif

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
i830_panel_fitter_pipe(I830Ptr pI830)
{
    uint32_t pfit_control;

    /* i830 doesn't have a panel fitter */
    if (IS_I830(pI830))
	return -1;

    pfit_control = INREG(PFIT_CONTROL);

    /* See if the panel fitter is in use */
    if ((pfit_control & PFIT_ENABLE) == 0)
	return -1;

    /* 965 can place panel fitter on either pipe */
    if (IS_I965G(pI830))
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
i830_update_dsparb(ScrnInfoPtr pScrn)
{
   xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
   I830Ptr pI830 = I830PTR(pScrn);
   int total_hdisplay = 0, planea_hdisplay = 0, planeb_hdisplay = 0;
   int fifo_entries = 0, planea_entries = 0, planeb_entries = 0, i;

   if ((INREG(DSPACNTR) & DISPLAY_PLANE_ENABLE) &&
       (INREG(DSPBCNTR) & DISPLAY_PLANE_ENABLE))
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		  "tried to update DSPARB with both planes enabled!\n");

  /*
    * FIFO entries will be split based on programmed modes
    */
   if (IS_I965GM(pI830) || IS_GM45(pI830))
       fifo_entries = 127;
   else if (IS_I9XX(pI830))
       fifo_entries = 95;
   else if (IS_MOBILE(pI830)) {
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

   if (IS_I9XX(pI830))
       OUTREG(DSPARB,
	      ((planea_entries + planeb_entries) << DSPARB_CSTART_SHIFT) |
	      (planea_entries << DSPARB_BSTART_SHIFT));
   else if (IS_MOBILE(pI830))
       OUTREG(DSPARB,
	      ((planea_entries + planeb_entries) << DSPARB_BEND_SHIFT) |
	      (planea_entries << DSPARB_AEND_SHIFT));
   else
       OUTREG(DSPARB, planea_entries << DSPARB_AEND_SHIFT);
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
    ScrnInfoPtr pScrn = crtc->scrn;
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    I830Ptr pI830 = I830PTR(pScrn);
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
    int dspstride_reg = (plane == 0) ? DSPASTRIDE : DSPBSTRIDE;
    int dsppos_reg = (plane == 0) ? DSPAPOS : DSPBPOS;
    int dspsize_reg = (plane == 0) ? DSPASIZE : DSPBSIZE;
    int pipestat_reg = (pipe == 0) ? PIPEASTAT : PIPEBSTAT;
    int i, num_outputs = 0;
    int refclk;
    intel_clock_t clock;
    uint32_t dpll = 0, fp = 0, dspcntr, pipeconf, lvds_bits = 0;
    Bool ok, is_sdvo = FALSE, is_dvo = FALSE;
    Bool is_crt = FALSE, is_lvds = FALSE, is_tv = FALSE;

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
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "clone detected, disabling SSC\n");

    /* Don't use SSC when cloned */
    if (is_lvds && pI830->lvds_use_ssc && num_outputs < 2) {
	refclk = pI830->lvds_ssc_freq * 1000;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "using SSC reference clock of %d MHz\n", refclk / 1000);
    } else if (IS_I9XX(pI830)) {
	refclk = 96000;
    } else {
	refclk = 48000;
    }

    ok = i830FindBestPLL(crtc, adjusted_mode->Clock, refclk, &clock);
    if (!ok)
	FatalError("Couldn't find PLL settings for mode!\n");

    if (fabs(adjusted_mode->Clock - clock.dot) / clock.dot > .02) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Chosen PLL clock of %.1f Mhz more than 2%% away from "
		   "desired %.1f Mhz\n",
		   (float)clock.dot / 1000,
		   (float)adjusted_mode->Clock / 1000);
    }

    fp = clock.n << 16 | clock.m1 << 8 | clock.m2;

    dpll = DPLL_VGA_MODE_DIS;
    if (IS_I9XX(pI830)) {
	if (is_lvds)
	    dpll |= DPLLB_MODE_LVDS;
	else
	    dpll |= DPLLB_MODE_DAC_SERIAL;
	if (is_sdvo)
	{
	    dpll |= DPLL_DVO_HIGH_SPEED;
	    if ((IS_I945G(pI830) || IS_I945GM(pI830) || IS_G33CLASS(pI830)) &&
		!is_tv)
	    {
		int sdvo_pixel_multiply = adjusted_mode->Clock / mode->Clock;
		dpll |= (sdvo_pixel_multiply - 1) << SDVO_MULTIPLIER_SHIFT_HIRES;
	    }
	}
	
	/* compute bitmask from p1 value */
	dpll |= (1 << (clock.p1 - 1)) << 16;
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
	if (IS_I965G(pI830) && !IS_GM45(pI830))
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
    else if (is_lvds && pI830->lvds_use_ssc && num_outputs < 2)
	dpll |= PLLB_REF_INPUT_SPREADSPECTRUMIN;
    else
	dpll |= PLL_REF_INPUT_DREFCLK;

    /* Set up the display plane register */
    dspcntr = DISPPLANE_GAMMA_ENABLE;
    switch (pScrn->bitsPerPixel) {
    case 8:
	dspcntr |= DISPPLANE_8BPP;
	break;
    case 16:
	if (pScrn->depth == 15)
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

    if (pipe == 0)
	dspcntr |= DISPPLANE_SEL_PIPE_A;
    else
	dspcntr |= DISPPLANE_SEL_PIPE_B;

    if (IS_I965G(pI830) && i830_display_tiled(crtc))
	dspcntr |= DISPLAY_PLANE_TILED;

    pipeconf = INREG(pipeconf_reg);
    if (pipe == 0 && !IS_I965G(pI830))
    {
	/* Enable pixel doubling when the dot clock is > 90% of the (display)
	 * core speed.
	 *
	 * XXX: No double-wide on 915GM pipe B. Is that the only reason for the
	 * pipe == 0 check?
	 */
	if (mode->Clock > i830_get_core_clock_speed(pScrn) * 9 / 10)
	    pipeconf |= PIPEACONF_DOUBLE_WIDE;
	else
	    pipeconf &= ~PIPEACONF_DOUBLE_WIDE;
    }
    /*
     * This "shouldn't" be needed as the dpms on code
     * will be run after the mode is set. On 9xx, it helps.
     * On 855, it can lock up the chip (and the entire machine)
     */
    if (!IS_I85X (pI830))
    {
	dspcntr |= DISPLAY_PLANE_ENABLE;
	pipeconf |= PIPEACONF_ENABLE;
	dpll |= DPLL_VCO_ENABLE;
    }
    
    /* Disable the panel fitter if it was on our pipe */
    if (i830_panel_fitter_pipe (pI830) == pipe)
	OUTREG(PFIT_CONTROL, 0);

    if (pI830->debug_modes) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Mode for pipe %c:\n", pipe == 0 ? 'A' : 'B');
	xf86PrintModeline(pScrn->scrnIndex, mode);
	if (!xf86ModesEqual(mode, adjusted_mode)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Adjusted mode for pipe %c:\n", pipe == 0 ? 'A' : 'B');
	    xf86PrintModeline(pScrn->scrnIndex, adjusted_mode);
	}
	i830PrintPll(pScrn, "chosen", &clock);
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
	uint32_t lvds = INREG(LVDS);

	lvds |= LVDS_PORT_EN | LVDS_A0A2_CLKA_POWER_UP | LVDS_PIPEB_SELECT;
	/* Set the B0-B3 data pairs corresponding to whether we're going to
	 * set the DPLLs for dual-channel mode or not.
	 */
	if (clock.p2 == I9XX_P2_LVDS_FAST)
	    lvds |= LVDS_B0B3_POWER_UP | LVDS_CLKB_POWER_UP;
	else
	    lvds &= ~(LVDS_B0B3_POWER_UP | LVDS_CLKB_POWER_UP);

	if (pI830->lvds_24_bit_mode) {
	    /* Option set which requests 24-bit mode
	     * (LVDS_A3_POWER_UP, as opposed to 18-bit mode) here; we
	     * still need to look more thoroughly into how panels
	     * behave in the two modes.  This option enables that
	     * experimentation.
	     */
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Selecting less common 24 bit TMDS pixel format.\n");
	    lvds |= LVDS_A3_POWER_UP;
	    lvds |= LVDS_DATA_FORMAT_DOT_ONE;
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Selecting standard 18 bit TMDS pixel format.\n");
	}

	/* Enable dithering if we're in 18-bit mode. */
	if (IS_I965G(pI830))
	{
	    if ((lvds & LVDS_A3_POWER_MASK) == LVDS_A3_POWER_UP)
		lvds &= ~LVDS_DITHER_ENABLE;
	    else
		lvds |= LVDS_DITHER_ENABLE;
	}

	lvds |= lvds_bits;

	OUTREG(LVDS, lvds);
	POSTING_READ(LVDS);
    }

    OUTREG(fp_reg, fp);
    OUTREG(dpll_reg, dpll);
    POSTING_READ(dpll_reg);
    /* Wait for the clocks to stabilize. */
    usleep(150);
    
    if (IS_I965G(pI830)) {
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

    if (!DSPARB_HWCONTROL(pI830))
	i830_update_dsparb(pScrn);

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
    OUTREG(dspstride_reg, pScrn->displayWidth * pI830->cpp);
    /* pipesrc and dspsize control the size that is scaled from, which should
     * always be the user's requested size.
     */
    OUTREG(dspsize_reg, ((mode->VDisplay - 1) << 16) | (mode->HDisplay - 1));
    OUTREG(dsppos_reg, 0);
    OUTREG(pipesrc_reg, ((mode->HDisplay - 1) << 16) | (mode->VDisplay - 1));
    OUTREG(pipeconf_reg, pipeconf);
    POSTING_READ(pipeconf_reg);
    i830WaitForVblank(pScrn);

    OUTREG(dspcntr_reg, dspcntr);
    /* Flush the plane changes */
    i830PipeSetBase(crtc, x, y);
#ifdef XF86DRI
   I830DRISetVBlankInterrupt (pScrn, TRUE);
#endif
    
    i830WaitForVblank(pScrn);

    /* Clear any FIFO underrun status that may have occurred normally */
    OUTREG(pipestat_reg, INREG(pipestat_reg) | FIFO_UNDERRUN);
}


/** Loads the palette/gamma unit for the CRTC with the prepared values */
void
i830_crtc_load_lut(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    int palreg = (intel_crtc->pipe == 0) ? PALETTE_A : PALETTE_B;
    int i;

    /* The clocks have to be on to load the palette. */
    if (!crtc->enabled)
	return;

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
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    int i;

    assert(size == 256);

    for (i = 0; i < 256; i++) {
	intel_crtc->lut_r[i] = red[i] >> 8;
	intel_crtc->lut_g[i] = green[i] >> 8;
	intel_crtc->lut_b[i] = blue[i] >> 8;
    }

    i830_crtc_load_lut(crtc);
}

/**
 * Allocates memory for a locked-in-framebuffer shadow of the given
 * width and height for this CRTC's rotated shadow framebuffer.
 */
 
static void *
i830_crtc_shadow_allocate (xf86CrtcPtr crtc, int width, int height)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    unsigned long rotate_pitch;
    int align = KB(4), size;

    rotate_pitch = pScrn->displayWidth * pI830->cpp;
    size = rotate_pitch * height;

    assert(intel_crtc->rotate_mem == NULL);
    intel_crtc->rotate_mem = i830_allocate_memory(pScrn, "rotated crtc",
						  size, align, 0);
    if (intel_crtc->rotate_mem == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Couldn't allocate shadow memory for rotated CRTC\n");
	return NULL;
    }
    memset(pI830->FbBase + intel_crtc->rotate_mem->offset, 0, size);

    return pI830->FbBase + intel_crtc->rotate_mem->offset;
}
    
/**
 * Creates a pixmap for this CRTC's rotated shadow framebuffer.
 */
static PixmapPtr
i830_crtc_shadow_create(xf86CrtcPtr crtc, void *data, int width, int height)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    unsigned long rotate_pitch;
    PixmapPtr rotate_pixmap;

    if (!data)
	data = i830_crtc_shadow_allocate (crtc, width, height);
    
    rotate_pitch = pScrn->displayWidth * pI830->cpp;

    rotate_pixmap = GetScratchPixmapHeader(pScrn->pScreen,
					   width, height,
					   pScrn->depth,
					   pScrn->bitsPerPixel,
					   rotate_pitch,
					   data);

    if (rotate_pixmap == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Couldn't allocate shadow pixmap for rotated CRTC\n");
    }
    return rotate_pixmap;
}

static void
i830_crtc_shadow_destroy(xf86CrtcPtr crtc, PixmapPtr rotate_pixmap, void *data)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;

    if (rotate_pixmap)
	FreeScratchPixmapHeader(rotate_pixmap);

    if (data) {
	/* Be sure to sync acceleration before the memory gets unbound. */
	I830Sync(pScrn);
	i830_free_memory(pScrn, intel_crtc->rotate_mem);
	intel_crtc->rotate_mem = NULL;
    }
}


void
i830DescribeOutputConfiguration(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    I830Ptr pI830 = I830PTR(pScrn);
    int i;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Output configuration:\n");

    for (i = 0; i < xf86_config->num_crtc; i++) {
	xf86CrtcPtr crtc = xf86_config->crtc[i];
	I830CrtcPrivatePtr intel_crtc = crtc ? crtc->driver_private : NULL;
	uint32_t dspcntr = intel_crtc->plane == 0 ? INREG(DSPACNTR) :
	    INREG(DSPBCNTR);
	uint32_t pipeconf = i == 0 ? INREG(PIPEACONF) :
	    INREG(PIPEBCONF);
	Bool hw_plane_enable = (dspcntr & DISPLAY_PLANE_ENABLE) != 0;
	Bool hw_pipe_enable = (pipeconf & PIPEACONF_ENABLE) != 0;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "  Pipe %c is %s\n",
		   'A' + i, crtc->enabled ? "on" : "off");
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "  Display plane %c is now %s and connected to pipe %c.\n",
		   'A' + intel_crtc->plane,
		   hw_plane_enable ? "enabled" : "disabled",
		   dspcntr & DISPPLANE_SEL_PIPE_MASK ? 'B' : 'A');
	if (hw_pipe_enable != crtc->enabled) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "  Hardware claims pipe %c is %s while software "
		       "believes it is %s\n",
		       'A' + i, hw_pipe_enable ? "on" : "off",
		       crtc->enabled ? "on" : "off");
	}
	if (hw_plane_enable != crtc->enabled) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
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
	
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
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
    ScrnInfoPtr		    pScrn = output->scrn;
    xf86CrtcConfigPtr	    xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
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
    i830WaitForVblank (pScrn);

    return crtc;
}

void
i830ReleaseLoadDetectPipe(xf86OutputPtr output, int dpms_mode)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    xf86CrtcPtr		    crtc = output->crtc;
    
    if (intel_output->load_detect_temp) 
    {
	output->crtc = NULL;
	intel_output->load_detect_temp = FALSE;
	crtc->enabled = xf86CrtcInUse (crtc);
	xf86DisableUnusedFunctions(pScrn);
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
i830_crtc_clock_get(ScrnInfoPtr pScrn, xf86CrtcPtr crtc)
{
    I830Ptr pI830 = I830PTR(pScrn);
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
    clock.m2 = (fp & FP_M2_DIV_MASK) >> FP_M2_DIV_SHIFT;
    clock.n = (fp & FP_N_DIV_MASK) >> FP_N_DIV_SHIFT;
    if (IS_I9XX(pI830)) {
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
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Unknown DPLL mode %08x in programmed mode\n",
		       (int)(dpll & DPLL_MODE_MASK));
	    return 0;
	}

	if ((dpll & PLL_REF_INPUT_MASK) == PLLB_REF_INPUT_SPREADSPECTRUMIN)
	    i9xx_clock(100000, &clock);
	else
	    i9xx_clock(96000, &clock);
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
		i8xx_clock(66000, &clock); /* XXX: might not be 66MHz */
	    else
		i8xx_clock(48000, &clock);		
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

	    i8xx_clock(48000, &clock);
	}
    }

    /* XXX: It would be nice to validate the clocks, but we can't reuse
     * i830PllIsValid() because it relies on the xf86_config output
     * configuration being accurate, which it isn't necessarily.
     */
    if (0)
	i830PrintPll(pScrn, "probed", &clock);

    return clock.dot;
}

/** Returns the currently programmed mode of the given pipe. */
DisplayModePtr
i830_crtc_mode_get(ScrnInfoPtr pScrn, xf86CrtcPtr crtc)
{
    I830Ptr pI830 = I830PTR(pScrn);
    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;
    int pipe = intel_crtc->pipe;
    DisplayModePtr mode;
    int htot = INREG((pipe == 0) ? HTOTAL_A : HTOTAL_B);
    int hsync = INREG((pipe == 0) ? HSYNC_A : HSYNC_B);
    int vtot = INREG((pipe == 0) ? VTOTAL_A : VTOTAL_B);
    int vsync = INREG((pipe == 0) ? VSYNC_A : VSYNC_B);

    mode = xcalloc(1, sizeof(DisplayModeRec));
    if (mode == NULL)
	return NULL;

    mode->Clock = i830_crtc_clock_get(pScrn, crtc);
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
/*    .load_cursor_image = i830_crtc_load_cursor_image, */
    .load_cursor_argb = i830_crtc_load_cursor_argb,
    .destroy = NULL, /* XXX */
};

void
i830_crtc_init(ScrnInfoPtr pScrn, int pipe)
{
    xf86CrtcPtr crtc;
    I830CrtcPrivatePtr intel_crtc;
    int i;

    crtc = xf86CrtcCreate (pScrn, &i830_crtc_funcs);
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

