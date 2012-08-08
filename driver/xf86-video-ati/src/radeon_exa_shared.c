/*
 * Copyright 2005 Eric Anholt
 * Copyright 2005 Benjamin Herrenschmidt
 * Copyright 2008 Advanced Micro Devices, Inc.
 * All Rights Reserved.
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
 *    Eric Anholt <anholt@FreeBSD.org>
 *    Zack Rusin <zrusin@trolltech.com>
 *    Benjamin Herrenschmidt <benh@kernel.crashing.org>
 *    Alex Deucher <alexander.deucher@amd.com>
 *    Matthias Hopf <mhopf@suse.de>
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "radeon.h"
#ifdef XF86DRI
#include "radeon_drm.h"
#endif
#include "radeon_macros.h"
#include "radeon_probe.h"
#include "radeon_version.h"
#include "radeon_vbo.h"

PixmapPtr
RADEONGetDrawablePixmap(DrawablePtr pDrawable)
{
    if (pDrawable->type == DRAWABLE_WINDOW)
	return pDrawable->pScreen->GetWindowPixmap((WindowPtr)pDrawable);
    else
	return (PixmapPtr)pDrawable;
}

void RADEONVlineHelperClear(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    accel_state->vline_crtc = NULL;
    accel_state->vline_y1 = -1;
    accel_state->vline_y2 = 0;
}

void RADEONVlineHelperSet(ScrnInfoPtr pScrn, int x1, int y1, int x2, int y2)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    accel_state->vline_crtc = radeon_pick_best_crtc(pScrn, x1, x2, y1, y2);
    if (accel_state->vline_y1 == -1)
	accel_state->vline_y1 = y1;
    if (y1 < accel_state->vline_y1)
	accel_state->vline_y1 = y1;
    if (y2 > accel_state->vline_y2)
	accel_state->vline_y2 = y2;
}

Bool RADEONValidPM(uint32_t pm, int bpp)
{
    uint8_t r, g, b, a;
    Bool ret = FALSE;

    switch (bpp) {
    case 8:
	a = pm & 0xff;
	if ((a == 0) || (a == 0xff))
	    ret = TRUE;
	break;
    case 16:
	r = (pm >> 11) & 0x1f;
	g = (pm >> 5) & 0x3f;
	b = (pm >> 0) & 0x1f;
	if (((r == 0) || (r == 0x1f)) &&
	    ((g == 0) || (g == 0x3f)) &&
	    ((b == 0) || (b == 0x1f)))
	    ret = TRUE;
	break;
    case 32:
	a = (pm >> 24) & 0xff;
	r = (pm >> 16) & 0xff;
	g = (pm >> 8) & 0xff;
	b = (pm >> 0) & 0xff;
	if (((a == 0) || (a == 0xff)) &&
	    ((r == 0) || (r == 0xff)) &&
	    ((g == 0) || (g == 0xff)) &&
	    ((b == 0) || (b == 0xff)))
	    ret = TRUE;
	break;
    default:
	break;
    }
    return ret;
}

Bool RADEONCheckBPP(int bpp)
{
	switch (bpp) {
	case 8:
	case 16:
	case 32:
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

PixmapPtr RADEONSolidPixmap(ScreenPtr pScreen, uint32_t solid)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    PixmapPtr pPix = pScreen->CreatePixmap(pScreen, 1, 1, 32, 0);

    exaMoveInPixmap(pPix);

#if defined(XF86DRM_MODE)
    if (info->cs) {
	struct radeon_bo *bo;

	bo = radeon_get_pixmap_bo(pPix);

	if (radeon_bo_map(bo, 1)) {
	    pScreen->DestroyPixmap(pPix);
	    return NULL;
	}

	memcpy(bo->ptr, &solid, 4);
	radeon_bo_unmap(bo);

	return pPix;
    }
#endif

    if (!exaDrawableIsOffscreen(&pPix->drawable)) {
	pScreen->DestroyPixmap(pPix);
	return NULL;
    }

    /* XXX: Big hammer... */
    info->accel_state->exa->WaitMarker(pScreen, info->accel_state->exaSyncMarker);
    memcpy(info->FB + exaGetPixmapOffset(pPix), &solid, 4);

    return pPix;
}

static Bool radeon_vb_get(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    accel_state->vbo.vb_mc_addr = info->gartLocation + info->dri->bufStart +
	(accel_state->ib->idx*accel_state->ib->total)+
	(accel_state->ib->total / 2);
    accel_state->vbo.vb_total = (accel_state->ib->total / 2);
    accel_state->vbo.vb_ptr = (pointer)((char*)accel_state->ib->address +
				    (accel_state->ib->total / 2));
    accel_state->vbo.vb_offset = 0;
    return TRUE;
}

int radeon_cp_start(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

#if defined(XF86DRM_MODE)
    if (info->cs) {
	if (CS_FULL(info->cs)) {
	    radeon_cs_flush_indirect(pScrn);
	}
	accel_state->ib_reset_op = info->cs->cdw;
    } else
#endif
    {
	accel_state->ib = RADEONCPGetBuffer(pScrn);
	if (!radeon_vb_get(pScrn)) {
	    return -1;
	}
    }
    accel_state->vbo.vb_start_op = accel_state->vbo.vb_offset;
    accel_state->cbuf.vb_start_op = accel_state->cbuf.vb_offset;
    return 0;
}

void radeon_vb_no_space(ScrnInfoPtr pScrn,
			struct radeon_vbo_object *vbo,
			int vert_size)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

#if defined(XF86DRM_MODE)
    if (info->cs) {
	if (vbo->vb_bo) {
	    if (vbo->vb_start_op != vbo->vb_offset) {
		accel_state->finish_op(pScrn, vert_size);
		accel_state->ib_reset_op = info->cs->cdw;
	    }

	    /* release the current VBO */
	    radeon_vbo_put(pScrn, vbo);
	}
	/* get a new one */
	radeon_vbo_get(pScrn, vbo);
	return;
    }
#endif
    if (vbo->vb_start_op != -1) {
        accel_state->finish_op(pScrn, vert_size);
        radeon_cp_start(pScrn);
    }
    return;
}

#if defined(XF86DRM_MODE)
void radeon_ib_discard(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    int ret;

    if (info->accel_state->ib_reset_op) {
        /* if we have data just reset the CS and ignore the operation */
	info->cs->cdw = info->accel_state->ib_reset_op;
	info->accel_state->ib_reset_op = 0;
	goto out;
    }

    info->accel_state->vbo.vb_offset = 0;
    info->accel_state->vbo.vb_start_op = -1;
    info->accel_state->cbuf.vb_offset = 0;
    info->accel_state->cbuf.vb_start_op = -1;

    if (CS_FULL(info->cs)) {
	radeon_cs_flush_indirect(pScrn);
	return;
    }
    radeon_cs_erase(info->cs);
    ret = radeon_cs_space_check_with_bo(info->cs,
					info->accel_state->vbo.vb_bo,
					RADEON_GEM_DOMAIN_GTT, 0);
    if (ret)
	ErrorF("space check failed in flush\n");

    if (info->accel_state->cbuf.vb_bo) {
	ret = radeon_cs_space_check_with_bo(info->cs,
					    info->accel_state->cbuf.vb_bo,
					    RADEON_GEM_DOMAIN_GTT, 0);
	if (ret)
	    ErrorF("space check failed in flush\n");
    }

 out:
    if (info->dri2.enabled) {
	info->accel_state->XInited3D = FALSE;
	info->accel_state->engineMode = EXA_ENGINEMODE_UNKNOWN;
    }

}
#endif
