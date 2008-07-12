/*
 * Copyright 2000 ATI Technologies Inc., Markham, Ontario, and
 *                VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include <stdio.h>

/* X and server generic header files */
#include "xf86.h"
#include "xf86_OSproc.h"
#include "vgaHW.h"
#include "xf86Modes.h"

/* Driver data structures */
#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_macros.h"
#include "radeon_probe.h"
#include "radeon_version.h"

#ifdef XF86DRI
#define _XF86DRI_SERVER_
#include "radeon_dri.h"
#include "radeon_sarea.h"
#include "sarea.h"
#endif

extern void atombios_crtc_mode_set(xf86CrtcPtr crtc,
				   DisplayModePtr mode,
				   DisplayModePtr adjusted_mode,
				   int x, int y);
extern void atombios_crtc_dpms(xf86CrtcPtr crtc, int mode);

void
radeon_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
    RADEONInfoPtr info = RADEONPTR(crtc->scrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(crtc->scrn);
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    xf86CrtcPtr crtc0 = pRADEONEnt->pCrtc[0];

    if ((mode == DPMSModeOn) && radeon_crtc->enabled)
	return;

    if (IS_AVIVO_VARIANT) {
	atombios_crtc_dpms(crtc, mode);
    } else {

	/* need to restore crtc1 before crtc0 or we may get a blank screen
	 * in some cases
	 */
	if ((radeon_crtc->crtc_id == 1) && (mode == DPMSModeOn)) {
	    if (crtc0->enabled)
		legacy_crtc_dpms(crtc0,  DPMSModeOff);
	}

	legacy_crtc_dpms(crtc, mode);

	if ((radeon_crtc->crtc_id == 1) && (mode == DPMSModeOn)) {
	    if (crtc0->enabled)
		legacy_crtc_dpms(crtc0, mode);
	}
    }

    if (mode == DPMSModeOn)
	radeon_crtc->enabled = TRUE;
    else
	radeon_crtc->enabled = FALSE;
}

static Bool
radeon_crtc_mode_fixup(xf86CrtcPtr crtc, DisplayModePtr mode,
		     DisplayModePtr adjusted_mode)
{
    return TRUE;
}

static void
radeon_crtc_mode_prepare(xf86CrtcPtr crtc)
{
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;

    if (radeon_crtc->enabled)
	crtc->funcs->hide_cursor(crtc);
}

static uint32_t RADEONDiv(CARD64 n, uint32_t d)
{
    return (n + (d / 2)) / d;
}

void
RADEONComputePLL(RADEONPLLPtr pll,
		 unsigned long freq,
		 uint32_t *chosen_dot_clock_freq,
		 uint32_t *chosen_feedback_div,
		 uint32_t *chosen_reference_div,
		 uint32_t *chosen_post_div,
		 int flags)
{
    uint32_t min_ref_div = pll->min_ref_div;
    uint32_t max_ref_div = pll->max_ref_div;
    uint32_t best_vco = pll->best_vco;
    uint32_t best_post_div = 1;
    uint32_t best_ref_div = 1;
    uint32_t best_feedback_div = 1;
    uint32_t best_freq = -1;
    uint32_t best_error = 0xffffffff;
    uint32_t best_vco_diff = 1;
    uint32_t post_div;

    freq = freq * 1000;

    ErrorF("freq: %lu\n", freq);

    if (flags & RADEON_PLL_USE_REF_DIV)
	min_ref_div = max_ref_div = pll->reference_div;
    else {
	while (min_ref_div < max_ref_div-1) {
	    uint32_t mid=(min_ref_div+max_ref_div)/2;
	    uint32_t pll_in = pll->reference_freq / mid;
	    if (pll_in < pll->pll_in_min)
		max_ref_div = mid;
	    else if (pll_in > pll->pll_in_max)
		min_ref_div = mid;
	    else break;
	}
    }

    for (post_div = pll->min_post_div; post_div <= pll->max_post_div; ++post_div) {
	uint32_t ref_div;

	if ((flags & RADEON_PLL_NO_ODD_POST_DIV) && (post_div & 1))
	    continue;

	/* legacy radeons only have a few post_divs */
	if (flags & RADEON_PLL_LEGACY) {
	    if ((post_div == 5) ||
		(post_div == 7) ||
		(post_div == 9) ||
		(post_div == 10) ||
		(post_div == 11))
		continue;
	}

	for (ref_div = min_ref_div; ref_div <= max_ref_div; ++ref_div) {
	    uint32_t feedback_div, current_freq, error, vco_diff;
	    uint32_t pll_in = pll->reference_freq / ref_div;
	    uint32_t min_feed_div = pll->min_feedback_div;
	    uint32_t max_feed_div = pll->max_feedback_div+1;

	    if (pll_in < pll->pll_in_min || pll_in > pll->pll_in_max)
		continue;

	    while (min_feed_div < max_feed_div) {
		uint32_t vco;

		feedback_div = (min_feed_div+max_feed_div)/2;

		vco = RADEONDiv((CARD64)pll->reference_freq * feedback_div,
				ref_div);

		if (vco < pll->pll_out_min) {
		    min_feed_div = feedback_div+1;
		    continue;
		} else if(vco > pll->pll_out_max) {
		    max_feed_div = feedback_div;
		    continue;
		}

		current_freq = RADEONDiv((CARD64)pll->reference_freq * 10000 * feedback_div,
					 ref_div * post_div);

		error = abs(current_freq - freq);
		vco_diff = abs(vco - best_vco);

		if ((best_vco == 0 && error < best_error) ||
		    (best_vco != 0 &&
		     (error < best_error - 100 ||
		      (abs(error - best_error) < 100 && vco_diff < best_vco_diff )))) {
		    best_post_div = post_div;
		    best_ref_div = ref_div;
		    best_feedback_div = feedback_div;
		    best_freq = current_freq;
		    best_error = error;
		    best_vco_diff = vco_diff;
		} else if (current_freq == freq) {
		    if (best_freq == -1) {
			best_post_div = post_div;
			best_ref_div = ref_div;
			best_feedback_div = feedback_div;
			best_freq = current_freq;
			best_error = error;
			best_vco_diff = vco_diff;
		    } else if ((flags & RADEON_PLL_PREFER_LOW_REF_DIV) && (ref_div < best_ref_div)) {
			best_post_div = post_div;
			best_ref_div = ref_div;
			best_feedback_div = feedback_div;
			best_freq = current_freq;
			best_error = error;
			best_vco_diff = vco_diff;
		    }
		}

		if (current_freq < freq)
		    min_feed_div = feedback_div+1;
		else
		    max_feed_div = feedback_div;
	    }
	}
    }

    ErrorF("best_freq: %u\n", (unsigned int)best_freq);
    ErrorF("best_feedback_div: %u\n", (unsigned int)best_feedback_div);
    ErrorF("best_ref_div: %u\n", (unsigned int)best_ref_div);
    ErrorF("best_post_div: %u\n", (unsigned int)best_post_div);

    if (best_freq == -1)
	FatalError("Couldn't find valid PLL dividers\n");
    *chosen_dot_clock_freq = best_freq / 10000;
    *chosen_feedback_div = best_feedback_div;
    *chosen_reference_div = best_ref_div;
    *chosen_post_div = best_post_div;

}

static void
radeon_crtc_mode_set(xf86CrtcPtr crtc, DisplayModePtr mode,
		     DisplayModePtr adjusted_mode, int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);

    if (IS_AVIVO_VARIANT) {
	atombios_crtc_mode_set(crtc, mode, adjusted_mode, x, y);
    } else {
	legacy_crtc_mode_set(crtc, mode, adjusted_mode, x, y);
    }
}

static void
radeon_crtc_mode_commit(xf86CrtcPtr crtc)
{
    if (crtc->scrn->pScreen != NULL)
	xf86_reload_cursors(crtc->scrn->pScreen);
}

void
radeon_crtc_load_lut(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int i;

    if (!crtc->enabled)
	return;

    if (IS_AVIVO_VARIANT) {
	OUTREG(AVIVO_DC_LUTA_CONTROL + radeon_crtc->crtc_offset, 0);

	OUTREG(AVIVO_DC_LUTA_BLACK_OFFSET_BLUE + radeon_crtc->crtc_offset, 0);
	OUTREG(AVIVO_DC_LUTA_BLACK_OFFSET_GREEN + radeon_crtc->crtc_offset, 0);
	OUTREG(AVIVO_DC_LUTA_BLACK_OFFSET_RED + radeon_crtc->crtc_offset, 0);

	OUTREG(AVIVO_DC_LUTA_WHITE_OFFSET_BLUE + radeon_crtc->crtc_offset, 0x0000ffff);
	OUTREG(AVIVO_DC_LUTA_WHITE_OFFSET_GREEN + radeon_crtc->crtc_offset, 0x0000ffff);
	OUTREG(AVIVO_DC_LUTA_WHITE_OFFSET_RED + radeon_crtc->crtc_offset, 0x0000ffff);
    }

    PAL_SELECT(radeon_crtc->crtc_id);

    if (IS_AVIVO_VARIANT) {
	OUTREG(AVIVO_DC_LUT_RW_MODE, 0);
	OUTREG(AVIVO_DC_LUT_WRITE_EN_MASK, 0x0000003f);
    }

    for (i = 0; i < 256; i++) {
	OUTPAL(i, radeon_crtc->lut_r[i], radeon_crtc->lut_g[i], radeon_crtc->lut_b[i]);
    }

    if (IS_AVIVO_VARIANT) {
	OUTREG(AVIVO_D1GRPH_LUT_SEL + radeon_crtc->crtc_offset, radeon_crtc->crtc_id);
    }

}


static void
radeon_crtc_gamma_set(xf86CrtcPtr crtc, uint16_t *red, uint16_t *green,
		      uint16_t *blue, int size)
{
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    ScrnInfoPtr		pScrn = crtc->scrn;
    int i, j;

    if (pScrn->depth == 16) {
	for (i = 0; i < 64; i++) {
	    if (i <= 31) {
		for (j = 0; j < 8; j++) {
		    radeon_crtc->lut_r[i * 8 + j] = red[i] >> 8;
		    radeon_crtc->lut_b[i * 8 + j] = blue[i] >> 8;
		}
	    }

	    for (j = 0; j < 4; j++) {
		radeon_crtc->lut_g[i * 4 + j] = green[i] >> 8;
	    }
	}
    } else {
	for (i = 0; i < 256; i++) {
	    radeon_crtc->lut_r[i] = red[i] >> 8;
	    radeon_crtc->lut_g[i] = green[i] >> 8;
	    radeon_crtc->lut_b[i] = blue[i] >> 8;
	}
    }

    radeon_crtc_load_lut(crtc);
}

static Bool
radeon_crtc_lock(xf86CrtcPtr crtc)
{
    ScrnInfoPtr		pScrn = crtc->scrn;
    RADEONInfoPtr  info = RADEONPTR(pScrn);

#ifdef XF86DRI
    if (info->CPStarted && pScrn->pScreen) {
	DRILock(pScrn->pScreen, 0);
	if (info->accelOn)
	    RADEON_SYNC(info, pScrn);
	return TRUE;
    }
#endif
    if (info->accelOn)
        RADEON_SYNC(info, pScrn);

    return FALSE;

}

static void
radeon_crtc_unlock(xf86CrtcPtr crtc)
{
    ScrnInfoPtr		pScrn = crtc->scrn;
    RADEONInfoPtr  info = RADEONPTR(pScrn);

#ifdef XF86DRI
	if (info->CPStarted && pScrn->pScreen) DRIUnlock(pScrn->pScreen);
#endif

    if (info->accelOn)
        RADEON_SYNC(info, pScrn);
}

#ifdef USE_XAA
/**
 * Allocates memory from the XF86 linear allocator, but also purges
 * memory if possible to cause the allocation to succeed.
 */
static FBLinearPtr
radeon_xf86AllocateOffscreenLinear(ScreenPtr pScreen, int length,
				 int granularity,
				 MoveLinearCallbackProcPtr moveCB,
				 RemoveLinearCallbackProcPtr removeCB,
				 pointer privData)
{
    FBLinearPtr linear;
    int max_size;

    linear = xf86AllocateOffscreenLinear(pScreen, length, granularity, moveCB,
					 removeCB, privData);
    if (linear != NULL)
	return linear;

    /* The above allocation didn't succeed, so purge unlocked stuff and try
     * again.
     */
    xf86QueryLargestOffscreenLinear(pScreen, &max_size, granularity,
				    PRIORITY_EXTREME);

    if (max_size < length)
	return NULL;

    xf86PurgeUnlockedOffscreenAreas(pScreen);

    linear = xf86AllocateOffscreenLinear(pScreen, length, granularity, moveCB,
					 removeCB, privData);

    return linear;
}
#endif

/**
 * Allocates memory for a locked-in-framebuffer shadow of the given
 * width and height for this CRTC's rotated shadow framebuffer.
 */

static void *
radeon_crtc_shadow_allocate (xf86CrtcPtr crtc, int width, int height)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    /* if this is called during ScreenInit() we don't have pScrn->pScreen yet */
    ScreenPtr pScreen = screenInfo.screens[pScrn->scrnIndex];
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    unsigned long rotate_pitch;
    unsigned long rotate_offset;
    int align = 4096, size;
    int cpp = pScrn->bitsPerPixel / 8;

    rotate_pitch = pScrn->displayWidth * cpp;
    size = rotate_pitch * height;

#ifdef USE_EXA
    /* We could get close to what we want here by just creating a pixmap like
     * normal, but we have to lock it down in framebuffer, and there is no
     * setter for offscreen area locking in EXA currently.  So, we just
     * allocate offscreen memory and fake up a pixmap header for it.
     */
    if (info->useEXA) {
	assert(radeon_crtc->rotate_mem_exa == NULL);

	radeon_crtc->rotate_mem_exa = exaOffscreenAlloc(pScreen, size, align,
						       TRUE, NULL, NULL);
	if (radeon_crtc->rotate_mem_exa == NULL) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Couldn't allocate shadow memory for rotated CRTC\n");
	    return NULL;
	}
	rotate_offset = radeon_crtc->rotate_mem_exa->offset;
    }
#endif /* USE_EXA */
#ifdef USE_XAA
    if (!info->useEXA) {
	/* The XFree86 linear allocator operates in units of screen pixels,
	 * sadly.
	 */
	size = (size + cpp - 1) / cpp;
	align = (align + cpp - 1) / cpp;

	assert(radeon_crtc->rotate_mem_xaa == NULL);

	radeon_crtc->rotate_mem_xaa =
	    radeon_xf86AllocateOffscreenLinear(pScreen, size, align,
					       NULL, NULL, NULL);
	if (radeon_crtc->rotate_mem_xaa == NULL) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Couldn't allocate shadow memory for rotated CRTC\n");
	    return NULL;
	}
#ifdef XF86DRI
	rotate_offset = info->frontOffset +
	    radeon_crtc->rotate_mem_xaa->offset * cpp;
#endif
    }
#endif /* USE_XAA */

    return info->FB + rotate_offset;
}
    
/**
 * Creates a pixmap for this CRTC's rotated shadow framebuffer.
 */
static PixmapPtr
radeon_crtc_shadow_create(xf86CrtcPtr crtc, void *data, int width, int height)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    unsigned long rotate_pitch;
    PixmapPtr rotate_pixmap;
    int cpp = pScrn->bitsPerPixel / 8;

    if (!data)
	data = radeon_crtc_shadow_allocate(crtc, width, height);

    rotate_pitch = pScrn->displayWidth * cpp;

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
radeon_crtc_shadow_destroy(xf86CrtcPtr crtc, PixmapPtr rotate_pixmap, void *data)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;

    if (rotate_pixmap)
	FreeScratchPixmapHeader(rotate_pixmap);
    
    if (data) {
#ifdef USE_EXA
	if (info->useEXA && radeon_crtc->rotate_mem_exa != NULL) {
	    exaOffscreenFree(pScrn->pScreen, radeon_crtc->rotate_mem_exa);
	    radeon_crtc->rotate_mem_exa = NULL;
	}
#endif /* USE_EXA */
#ifdef USE_XAA
	if (!info->useEXA) {
	    xf86FreeOffscreenLinear(radeon_crtc->rotate_mem_xaa);
	    radeon_crtc->rotate_mem_xaa = NULL;
	}
#endif /* USE_XAA */
    }
}

static const xf86CrtcFuncsRec radeon_crtc_funcs = {
    .dpms = radeon_crtc_dpms,
    .save = NULL, /* XXX */
    .restore = NULL, /* XXX */
    .mode_fixup = radeon_crtc_mode_fixup,
    .prepare = radeon_crtc_mode_prepare,
    .mode_set = radeon_crtc_mode_set,
    .commit = radeon_crtc_mode_commit,
    .gamma_set = radeon_crtc_gamma_set,
    .lock = radeon_crtc_lock,
    .unlock = radeon_crtc_unlock,
    .shadow_create = radeon_crtc_shadow_create,
    .shadow_allocate = radeon_crtc_shadow_allocate,
    .shadow_destroy = radeon_crtc_shadow_destroy,
    .set_cursor_colors = radeon_crtc_set_cursor_colors,
    .set_cursor_position = radeon_crtc_set_cursor_position,
    .show_cursor = radeon_crtc_show_cursor,
    .hide_cursor = radeon_crtc_hide_cursor,
    .load_cursor_argb = radeon_crtc_load_cursor_argb,
    .destroy = NULL, /* XXX */
};

Bool RADEONAllocateControllers(ScrnInfoPtr pScrn, int mask)
{
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (mask & 1) {
	if (pRADEONEnt->Controller[0])
	    return TRUE;

	pRADEONEnt->pCrtc[0] = xf86CrtcCreate(pScrn, &radeon_crtc_funcs);
	if (!pRADEONEnt->pCrtc[0])
	    return FALSE;

	pRADEONEnt->Controller[0] = xnfcalloc(sizeof(RADEONCrtcPrivateRec), 1);
	if (!pRADEONEnt->Controller[0])
	    return FALSE;

	pRADEONEnt->pCrtc[0]->driver_private = pRADEONEnt->Controller[0];
	pRADEONEnt->Controller[0]->crtc_id = 0;
	pRADEONEnt->Controller[0]->crtc_offset = 0;
	if (info->allowColorTiling)
	    pRADEONEnt->Controller[0]->can_tile = 1;
	else
	    pRADEONEnt->Controller[0]->can_tile = 0;
    }

    if (mask & 2) {
	if (!pRADEONEnt->HasCRTC2)
	    return TRUE;

	pRADEONEnt->pCrtc[1] = xf86CrtcCreate(pScrn, &radeon_crtc_funcs);
	if (!pRADEONEnt->pCrtc[1])
	    return FALSE;

	pRADEONEnt->Controller[1] = xnfcalloc(sizeof(RADEONCrtcPrivateRec), 1);
	if (!pRADEONEnt->Controller[1])
	    {
		xfree(pRADEONEnt->Controller[0]);
		return FALSE;
	    }

	pRADEONEnt->pCrtc[1]->driver_private = pRADEONEnt->Controller[1];
	pRADEONEnt->Controller[1]->crtc_id = 1;
	pRADEONEnt->Controller[1]->crtc_offset = AVIVO_D2CRTC_H_TOTAL - AVIVO_D1CRTC_H_TOTAL;
	if (info->allowColorTiling)
	    pRADEONEnt->Controller[1]->can_tile = 1;
	else
	    pRADEONEnt->Controller[1]->can_tile = 0;
    }

    return TRUE;
}

/**
 * In the current world order, there are lists of modes per output, which may
 * or may not include the mode that was asked to be set by XFree86's mode
 * selection.  Find the closest one, in the following preference order:
 *
 * - Equality
 * - Closer in size to the requested mode, but no larger
 * - Closer in refresh rate to the requested mode.
 */
DisplayModePtr
RADEONCrtcFindClosestMode(xf86CrtcPtr crtc, DisplayModePtr pMode)
{
    ScrnInfoPtr	pScrn = crtc->scrn;
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    DisplayModePtr pBest = NULL, pScan = NULL;
    int i;

    /* Assume that there's only one output connected to the given CRTC. */
    for (i = 0; i < xf86_config->num_output; i++) 
    {
	xf86OutputPtr  output = xf86_config->output[i];
	if (output->crtc == crtc && output->probed_modes != NULL)
	{
	    pScan = output->probed_modes;
	    break;
	}
    }

    /* If the pipe doesn't have any detected modes, just let the system try to
     * spam the desired mode in.
     */
    if (pScan == NULL) {
	RADEONCrtcPrivatePtr  radeon_crtc = crtc->driver_private;
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "No crtc mode list for crtc %d,"
		   "continuing with desired mode\n", radeon_crtc->crtc_id);
	return pMode;
    }

    for (; pScan != NULL; pScan = pScan->next) {
	assert(pScan->VRefresh != 0.0);

	/* If there's an exact match, we're done. */
	if (xf86ModesEqual(pScan, pMode)) {
	    pBest = pMode;
	    break;
	}

	/* Reject if it's larger than the desired mode. */
	if (pScan->HDisplay > pMode->HDisplay ||
	    pScan->VDisplay > pMode->VDisplay)
	{
	    continue;
	}

	if (pBest == NULL) {
	    pBest = pScan;
	    continue;
	}

	/* Find if it's closer to the right size than the current best
	 * option.
	 */
	if ((pScan->HDisplay > pBest->HDisplay &&
	     pScan->VDisplay >= pBest->VDisplay) ||
	    (pScan->HDisplay >= pBest->HDisplay &&
	     pScan->VDisplay > pBest->VDisplay))
	{
	    pBest = pScan;
	    continue;
	}

	/* Find if it's still closer to the right refresh than the current
	 * best resolution.
	 */
	if (pScan->HDisplay == pBest->HDisplay &&
	    pScan->VDisplay == pBest->VDisplay &&
	    (fabs(pScan->VRefresh - pMode->VRefresh) <
	     fabs(pBest->VRefresh - pMode->VRefresh))) {
	    pBest = pScan;
	}
    }

    if (pBest == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "No suitable mode found to program for the pipe.\n"
		   "	continuing with desired mode %dx%d@%.1f\n",
		   pMode->HDisplay, pMode->VDisplay, pMode->VRefresh);
    } else if (!xf86ModesEqual(pBest, pMode)) {
      RADEONCrtcPrivatePtr  radeon_crtc = crtc->driver_private;
      int		    crtc = radeon_crtc->crtc_id;
      xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Choosing pipe %d's mode %dx%d@%.1f instead of xf86 "
		   "mode %dx%d@%.1f\n", crtc,
		   pBest->HDisplay, pBest->VDisplay, pBest->VRefresh,
		   pMode->HDisplay, pMode->VDisplay, pMode->VRefresh);
	pMode = pBest;
    }
    return pMode;
}

void
RADEONBlank(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86OutputPtr output;
    xf86CrtcPtr crtc;
    int o, c;

    for (c = 0; c < xf86_config->num_crtc; c++) {
	crtc = xf86_config->crtc[c];
	for (o = 0; o < xf86_config->num_output; o++) {
	    output = xf86_config->output[o];
	    if (output->crtc != crtc)
		continue;

	    output->funcs->dpms(output, DPMSModeOff);
	}
	crtc->funcs->dpms(crtc, DPMSModeOff);
    }
}

void
RADEONUnblank(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86OutputPtr output;
    xf86CrtcPtr crtc;
    int o, c;

    for (c = 0; c < xf86_config->num_crtc; c++) {
	crtc = xf86_config->crtc[c];
	if(!crtc->enabled)
		continue;
	crtc->funcs->dpms(crtc, DPMSModeOn);
	for (o = 0; o < xf86_config->num_output; o++) {
	    output = xf86_config->output[o];
	    if (output->crtc != crtc)
		continue;

	    output->funcs->dpms(output, DPMSModeOn);
	}
    }
}

Bool
RADEONSetTiling(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONCrtcPrivatePtr radeon_crtc;
    xf86CrtcPtr crtc;
    int c;
    int can_tile = 1;
    Bool changed = FALSE;

    for (c = 0; c < xf86_config->num_crtc; c++) {
	crtc = xf86_config->crtc[c];
	radeon_crtc = crtc->driver_private;

	if (crtc->enabled) {
	    if (!radeon_crtc->can_tile)
		can_tile = 0;
	}
    }

    if (info->tilingEnabled != can_tile)
	changed = TRUE;

#ifdef XF86DRI
    if (info->directRenderingEnabled && (info->tilingEnabled != can_tile)) {
	RADEONSAREAPrivPtr pSAREAPriv;
	if (RADEONDRISetParam(pScrn, RADEON_SETPARAM_SWITCH_TILING, (can_tile ? 1 : 0)) < 0)
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "[drm] failed changing tiling status\n");
	/* if this is called during ScreenInit() we don't have pScrn->pScreen yet */
	pSAREAPriv = DRIGetSAREAPrivate(screenInfo.screens[pScrn->scrnIndex]);
	info->tilingEnabled = pSAREAPriv->tiling_enabled ? TRUE : FALSE;
    }
#endif

    return changed;
}
