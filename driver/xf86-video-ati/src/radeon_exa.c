/*
 * Copyright 2005 Eric Anholt
 * Copyright 2005 Benjamin Herrenschmidt
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
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "radeon.h"
#include "radeon_reg.h"
#include "r600_reg.h"
#include "radeon_bo_helper.h"
#include "radeon_probe.h"
#include "radeon_version.h"
#include "radeon_exa_shared.h"
#include "xf86.h"


/***********************************************************************/
#define RINFO_FROM_SCREEN(pScr) ScrnInfoPtr pScrn =  xf86ScreenToScrn(pScr); \
    RADEONInfoPtr info   = RADEONPTR(pScrn)

static struct {
    int rop;
    int pattern;
} RADEON_ROP[] = {
    { RADEON_ROP3_ZERO, RADEON_ROP3_ZERO }, /* GXclear        */
    { RADEON_ROP3_DSa,  RADEON_ROP3_DPa  }, /* Gxand          */
    { RADEON_ROP3_SDna, RADEON_ROP3_PDna }, /* GXandReverse   */
    { RADEON_ROP3_S,    RADEON_ROP3_P    }, /* GXcopy         */
    { RADEON_ROP3_DSna, RADEON_ROP3_DPna }, /* GXandInverted  */
    { RADEON_ROP3_D,    RADEON_ROP3_D    }, /* GXnoop         */
    { RADEON_ROP3_DSx,  RADEON_ROP3_DPx  }, /* GXxor          */
    { RADEON_ROP3_DSo,  RADEON_ROP3_DPo  }, /* GXor           */
    { RADEON_ROP3_DSon, RADEON_ROP3_DPon }, /* GXnor          */
    { RADEON_ROP3_DSxn, RADEON_ROP3_PDxn }, /* GXequiv        */
    { RADEON_ROP3_Dn,   RADEON_ROP3_Dn   }, /* GXinvert       */
    { RADEON_ROP3_SDno, RADEON_ROP3_PDno }, /* GXorReverse    */
    { RADEON_ROP3_Sn,   RADEON_ROP3_Pn   }, /* GXcopyInverted */
    { RADEON_ROP3_DSno, RADEON_ROP3_DPno }, /* GXorInverted   */
    { RADEON_ROP3_DSan, RADEON_ROP3_DPan }, /* GXnand         */
    { RADEON_ROP3_ONE,  RADEON_ROP3_ONE  }  /* GXset          */
};

static __inline__ uint32_t F_TO_DW(float val)
{
    union {
	float f;
	uint32_t l;
    } tmp;
    tmp.f = val;
    return tmp.l;
}


/* Assumes that depth 15 and 16 can be used as depth 16, which is okay since we
 * require src and dest datatypes to be equal.
 */
Bool RADEONGetDatatypeBpp(int bpp, uint32_t *type)
{
	switch (bpp) {
	case 8:
		*type = ATI_DATATYPE_CI8;
		return TRUE;
	case 16:
		*type = ATI_DATATYPE_RGB565;
		return TRUE;
	case 24:
		*type = ATI_DATATYPE_CI8;
		return TRUE;
	case 32:
		*type = ATI_DATATYPE_ARGB8888;
		return TRUE;
	default:
		RADEON_FALLBACK(("Unsupported bpp: %d\n", bpp));
		return FALSE;
	}
}

static Bool RADEONPixmapIsColortiled(PixmapPtr pPix)
{
    return FALSE;
}

static Bool RADEONGetOffsetPitch(PixmapPtr pPix, int bpp, uint32_t *pitch_offset,
				 unsigned int offset, unsigned int pitch)
{
	RINFO_FROM_SCREEN(pPix->drawable.pScreen);

	if (pitch > 16320 || pitch % info->accel_state->exa->pixmapPitchAlign != 0)
		RADEON_FALLBACK(("Bad pitch 0x%08x\n", pitch));

	if (offset % info->accel_state->exa->pixmapOffsetAlign != 0)
		RADEON_FALLBACK(("Bad offset 0x%08x\n", offset));

	pitch = pitch >> 6;
	*pitch_offset = (pitch << 22) | (offset >> 10);

	/* If it's the front buffer, we've got to note that it's tiled? */
	if (RADEONPixmapIsColortiled(pPix))
		*pitch_offset |= RADEON_DST_TILE_MACRO;
	return TRUE;
}

Bool RADEONGetPixmapOffsetPitch(PixmapPtr pPix, uint32_t *pitch_offset)
{
	uint32_t pitch;
	int bpp;

	bpp = pPix->drawable.bitsPerPixel;
	if (bpp == 24)
		bpp = 8;

	pitch = exaGetPixmapPitch(pPix);

	return RADEONGetOffsetPitch(pPix, bpp, pitch_offset, 0, pitch);
}

/**
 * Returns whether the provided transform is affine.
 *
 * transform may be null.
 */
Bool radeon_transform_is_affine_or_scaled(PictTransformPtr t)
{
	if (t == NULL)
		return TRUE;
	/* the shaders don't handle scaling either */
	return t->matrix[2][0] == 0 && t->matrix[2][1] == 0 && t->matrix[2][2] == IntToxFixed(1);
}

Bool RADEONPrepareAccess_CS(PixmapPtr pPix, int index)
{
    ScreenPtr pScreen = pPix->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_exa_pixmap_priv *driver_priv;
    uint32_t possible_domains = ~0U;
    uint32_t current_domain = 0;
    Bool can_fail = !(pPix->drawable.bitsPerPixel < 8) &&
	pPix != pScreen->GetScreenPixmap(pScreen);
    Bool flush = FALSE;
    int ret;

#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* May need to handle byte swapping in DownloadFrom/UploadToScreen */
    if (pPix->drawable.bitsPerPixel > 8)
	return FALSE;
#endif

    driver_priv = exaGetPixmapDriverPrivate(pPix);
    if (!driver_priv)
      return FALSE;

    /* untile in DFS/UTS */
    if (driver_priv->tiling_flags & (RADEON_TILING_MACRO | RADEON_TILING_MICRO))
	return FALSE;

    /* if we have more refs than just the BO then flush */
    if (radeon_bo_is_referenced_by_cs(driver_priv->bo, info->cs)) {
	flush = TRUE;

	if (can_fail) {
	    possible_domains = radeon_bo_get_src_domain(driver_priv->bo);
	    if (possible_domains == RADEON_GEM_DOMAIN_VRAM)
		return FALSE; /* use DownloadFromScreen */
	}
    }

    /* if the BO might end up in VRAM, prefer DownloadFromScreen */
    if (can_fail && (possible_domains & RADEON_GEM_DOMAIN_VRAM)) {
	radeon_bo_is_busy(driver_priv->bo, &current_domain);

	if (current_domain & possible_domains) {
	    if (current_domain == RADEON_GEM_DOMAIN_VRAM)
		return FALSE;
	} else if (possible_domains & RADEON_GEM_DOMAIN_VRAM)
	    return FALSE;
    }

    if (flush)
        radeon_cs_flush_indirect(pScrn);
    
    /* flush IB */
    ret = radeon_bo_map(driver_priv->bo, 1);
    if (ret) {
      FatalError("failed to map pixmap %d\n", ret);
      return FALSE;
    }
    driver_priv->bo_mapped = TRUE;

    pPix->devPrivate.ptr = driver_priv->bo->ptr;

    return TRUE;
}

void RADEONFinishAccess_CS(PixmapPtr pPix, int index)
{
    struct radeon_exa_pixmap_priv *driver_priv;

    driver_priv = exaGetPixmapDriverPrivate(pPix);
    if (!driver_priv || !driver_priv->bo_mapped)
        return;

    radeon_bo_unmap(driver_priv->bo);
    driver_priv->bo_mapped = FALSE;
    pPix->devPrivate.ptr = NULL;
}


void *RADEONEXACreatePixmap(ScreenPtr pScreen, int size, int align)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_exa_pixmap_priv *new_priv;

    if (size != 0 && !info->exa_force_create &&
	info->exa_pixmaps == FALSE)
        return NULL;
	    
    new_priv = calloc(1, sizeof(struct radeon_exa_pixmap_priv));
    if (!new_priv)
	return NULL;

    if (size == 0)
	return new_priv;

    new_priv->bo = radeon_bo_open(info->bufmgr, 0, size, align,
				  RADEON_GEM_DOMAIN_VRAM, 0);
    if (!new_priv->bo) {
	free(new_priv);
	ErrorF("Failed to alloc memory\n");
	return NULL;
    }
    
    return new_priv;

}

void *RADEONEXACreatePixmap2(ScreenPtr pScreen, int width, int height,
			     int depth, int usage_hint, int bitsPerPixel,
			     int *new_pitch)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_exa_pixmap_priv *new_priv;

    if (width != 0 && height != 0 && !info->exa_force_create &&
	info->exa_pixmaps == FALSE)
        return NULL;

    new_priv = calloc(1, sizeof(struct radeon_exa_pixmap_priv));
    if (!new_priv) {
	return NULL;
    }

    if (width == 0 || height == 0) {
	return new_priv;
    }

    new_priv->bo = radeon_alloc_pixmap_bo(pScrn, width, height, depth,
					  usage_hint, bitsPerPixel, new_pitch,
					  &new_priv->surface,
					  &new_priv->tiling_flags);
    if (!new_priv->bo) {
	free(new_priv);
	ErrorF("Failed to alloc memory\n");
	return NULL;
    }

    return new_priv;
}

void RADEONEXADestroyPixmap(ScreenPtr pScreen, void *driverPriv)
{
    struct radeon_exa_pixmap_priv *driver_priv = driverPriv;

    if (!driverPriv)
      return;

    if (driver_priv->bo)
	radeon_bo_unref(driver_priv->bo);
    free(driverPriv);
}

#ifdef RADEON_PIXMAP_SHARING
Bool RADEONEXASharePixmapBacking(PixmapPtr ppix, ScreenPtr slave, void **fd_handle)
{
    struct radeon_exa_pixmap_priv *driver_priv = exaGetPixmapDriverPrivate(ppix);

    if (!radeon_share_pixmap_backing(driver_priv->bo, fd_handle))
	return FALSE;

    driver_priv->shared = TRUE;
    return TRUE;
}

Bool RADEONEXASetSharedPixmapBacking(PixmapPtr ppix, void *fd_handle)
{
    struct radeon_exa_pixmap_priv *driver_priv = exaGetPixmapDriverPrivate(ppix);

    if (!radeon_set_shared_pixmap_backing(ppix, fd_handle, &driver_priv->surface))
	return FALSE;

    driver_priv->shared = TRUE;
    driver_priv->tiling_flags = 0;
    return TRUE;
}
#endif

uint32_t radeon_get_pixmap_tiling(PixmapPtr pPix)
{
    struct radeon_exa_pixmap_priv *driver_priv;
    driver_priv = exaGetPixmapDriverPrivate(pPix);
    return driver_priv->tiling_flags;
}

Bool RADEONEXAPixmapIsOffscreen(PixmapPtr pPix)
{
    struct radeon_exa_pixmap_priv *driver_priv;

    driver_priv = exaGetPixmapDriverPrivate(pPix);

    if (!driver_priv)
       return FALSE;
    if (driver_priv->bo)
       return TRUE;
    return FALSE;
}

#define ENTER_DRAW(x) TRACE
#define LEAVE_DRAW(x) TRACE
/***********************************************************************/

#ifdef RENDER
#include "radeon_exa_render.c"
#endif
#include "radeon_exa_funcs.c"



