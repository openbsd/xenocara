/*
 * Copyright 2012  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "radeon.h"
#include "radeon_glamor.h"
#include "radeon_bo_gem.h"


#ifdef USE_GLAMOR

static uint32_t
radeon_get_gbm_format(int depth, int bitsPerPixel)
{
    switch (depth) {
#ifdef GBM_FORMAT_R8
    case 8:
	return GBM_FORMAT_R8;
#endif
    case 15:
	return GBM_FORMAT_ARGB1555;
    case 16:
	return GBM_FORMAT_RGB565;
    case 32:
	return GBM_FORMAT_ARGB8888;
    case 30:
	return GBM_FORMAT_XRGB2101010;
    case 24:
	if (bitsPerPixel == 32)
	    return GBM_FORMAT_XRGB8888;
	/* fall through */
    default:
	ErrorF("%s: Unsupported depth/bpp %d/%d\n", __func__, depth,
	       bitsPerPixel);
	return ~0U;
    }
}

#endif /* USE_GLAMOR */


static const unsigned MicroBlockTable[5][3][2] = {
    /*linear  tiled   square-tiled */
    {{32, 1}, {8, 4}, {0, 0}}, /*   8 bits per pixel */
    {{16, 1}, {8, 2}, {4, 4}}, /*  16 bits per pixel */
    {{ 8, 1}, {4, 2}, {0, 0}}, /*  32 bits per pixel */
    {{ 4, 1}, {0, 0}, {2, 2}}, /*  64 bits per pixel */
    {{ 2, 1}, {0, 0}, {0, 0}}  /* 128 bits per pixel */
};

/* Return true if macrotiling can be enabled */
static Bool RADEONMacroSwitch(int width, int height, int bpp,
                              uint32_t flags, Bool rv350_mode)
{
    unsigned tilew, tileh, microtiled, logbpp;

    logbpp = RADEONLog2(bpp / 8);
    if (logbpp > 4)
        return 0;

    microtiled = !!(flags & RADEON_TILING_MICRO);
    tilew = MicroBlockTable[logbpp][microtiled][0] * 8;
    tileh = MicroBlockTable[logbpp][microtiled][1] * 8;

    /* See TX_FILTER1_n.MACRO_SWITCH. */
    if (rv350_mode) {
        return width >= tilew && height >= tileh;
    } else {
        return width > tilew && height > tileh;
    }
}

static unsigned eg_tile_split_opp(unsigned tile_split)
{
    switch (tile_split) {
        case 0:     tile_split = 64;    break;
        case 1:     tile_split = 128;   break;
        case 2:     tile_split = 256;   break;
        case 3:     tile_split = 512;   break;
        default:
        case 4:     tile_split = 1024;  break;
        case 5:     tile_split = 2048;  break;
        case 6:     tile_split = 4096;  break;
    }
    return tile_split;
}

Bool
radeon_surface_initialize(RADEONInfoPtr info, struct radeon_surface *surface,
			  int width, int height, int cpp, uint32_t tiling_flags,
			  int usage_hint)
{
	memset(surface, 0, sizeof(struct radeon_surface));

	surface->npix_x = width;
	/* need to align height to 8 for old kernel */
	surface->npix_y = RADEON_ALIGN(height, 8);
	surface->npix_z = 1;
	surface->blk_w = 1;
	surface->blk_h = 1;
	surface->blk_d = 1;
	surface->array_size = 1;
	surface->last_level = 0;
	surface->bpe = cpp;
	surface->nsamples = 1;
	if (height < 128) {
	    /* disable 2d tiling for small surface to work around
	     * the fact that ddx align height to 8 pixel for old
	     * obscure reason i can't remember
	     */
	    tiling_flags &= ~RADEON_TILING_MACRO;
	}

	surface->flags = RADEON_SURF_SCANOUT | RADEON_SURF_HAS_TILE_MODE_INDEX |
	    RADEON_SURF_SET(RADEON_SURF_TYPE_2D, TYPE);

	if (usage_hint & RADEON_CREATE_PIXMAP_SZBUFFER) {
	    surface->flags |= RADEON_SURF_ZBUFFER;
	    surface->flags |= RADEON_SURF_SBUFFER;
	}

	if ((tiling_flags & RADEON_TILING_MACRO)) {
	    surface->flags = RADEON_SURF_CLR(surface->flags, MODE);
	    surface->flags |= RADEON_SURF_SET(RADEON_SURF_MODE_2D, MODE);
	} else if ((tiling_flags & RADEON_TILING_MICRO)) {
	    surface->flags = RADEON_SURF_CLR(surface->flags, MODE);
	    surface->flags |= RADEON_SURF_SET(RADEON_SURF_MODE_1D, MODE);
	} else
	    surface->flags |= RADEON_SURF_SET(RADEON_SURF_MODE_LINEAR, MODE);

	if (info->ChipFamily >= CHIP_FAMILY_CEDAR) {
	    surface->bankw = (tiling_flags >> RADEON_TILING_EG_BANKW_SHIFT) &
		RADEON_TILING_EG_BANKW_MASK;
	    surface->bankh = (tiling_flags >> RADEON_TILING_EG_BANKH_SHIFT) &
		RADEON_TILING_EG_BANKH_MASK;
	    surface->tile_split = eg_tile_split_opp((tiling_flags >> RADEON_TILING_EG_TILE_SPLIT_SHIFT) &
						    RADEON_TILING_EG_TILE_SPLIT_MASK);
	    if (surface->flags & RADEON_SURF_SBUFFER) {
		surface->stencil_tile_split =
		    (tiling_flags >> RADEON_TILING_EG_STENCIL_TILE_SPLIT_SHIFT) &
		    RADEON_TILING_EG_STENCIL_TILE_SPLIT_MASK;
	    }
	    surface->mtilea = (tiling_flags >> RADEON_TILING_EG_MACRO_TILE_ASPECT_SHIFT) &
		RADEON_TILING_EG_MACRO_TILE_ASPECT_MASK;
	}

	if (radeon_surface_best(info->surf_man, surface))
	    return FALSE;

	if (radeon_surface_init(info->surf_man, surface))
	    return FALSE;

	return TRUE;
}

/* Calculate appropriate tiling and pitch for a pixmap and allocate a BO that
 * can hold it.
 */
struct radeon_buffer *
radeon_alloc_pixmap_bo(ScrnInfoPtr pScrn, int width, int height, int depth,
		       int usage_hint, int bitsPerPixel, int *new_pitch,
		       struct radeon_surface *new_surface, uint32_t *new_tiling)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    int pitch, base_align;
    uint32_t size, heighta;
    int cpp = bitsPerPixel / 8;
    uint32_t tiling = 0, flags = 0;
    struct radeon_surface surface;
    struct radeon_buffer *bo;
    int domain = RADEON_GEM_DOMAIN_VRAM;

#ifdef USE_GLAMOR
    if (info->use_glamor &&
	!(usage_hint == CREATE_PIXMAP_USAGE_BACKING_PIXMAP &&
	  info->shadow_primary)) {
	uint32_t bo_use = GBM_BO_USE_RENDERING;
	uint32_t gbm_format = radeon_get_gbm_format(depth, bitsPerPixel);

	if (gbm_format == ~0U)
	    return NULL;

	bo = calloc(1, sizeof(struct radeon_buffer));
	if (!bo)
	    return NULL;

	bo->ref_count = 1;

	if (bitsPerPixel == pScrn->bitsPerPixel)
	    bo_use |= GBM_BO_USE_SCANOUT;

	if ((usage_hint == CREATE_PIXMAP_USAGE_BACKING_PIXMAP &&
	     info->shadow_primary) ||
	    (usage_hint & 0xffff) == CREATE_PIXMAP_USAGE_SHARED)
	    bo_use |= GBM_BO_USE_LINEAR;

	bo->bo.gbm = gbm_bo_create(info->gbm, width, height, gbm_format, bo_use);
	if (!bo->bo.gbm) {
	    free(bo);
	    return NULL;
	}

	bo->flags |= RADEON_BO_FLAGS_GBM;

	if (new_pitch)
	    *new_pitch = gbm_bo_get_stride(bo->bo.gbm);

	return bo;
    }
#endif

    if (usage_hint) {
	if (info->allowColorTiling) {
	    if (usage_hint & RADEON_CREATE_PIXMAP_TILING_MACRO)
		tiling |= RADEON_TILING_MACRO;
	    if (usage_hint & RADEON_CREATE_PIXMAP_TILING_MICRO)
                tiling |= RADEON_TILING_MICRO;
	}
	if (usage_hint & RADEON_CREATE_PIXMAP_DEPTH)
		tiling |= RADEON_TILING_MACRO | RADEON_TILING_MICRO;

	if ((usage_hint == CREATE_PIXMAP_USAGE_BACKING_PIXMAP &&
	     info->shadow_primary) ||
	    (usage_hint & 0xffff) == CREATE_PIXMAP_USAGE_SHARED) {
		tiling = 0;
		domain = RADEON_GEM_DOMAIN_GTT;
	}
    }

    /* Small pixmaps must not be macrotiled on R300, hw cannot sample them
     * correctly because samplers automatically switch to macrolinear. */
    if (info->ChipFamily >= CHIP_FAMILY_R300 &&
        info->ChipFamily <= CHIP_FAMILY_RS740 &&
        (tiling & RADEON_TILING_MACRO) &&
        !RADEONMacroSwitch(width, height, bitsPerPixel, tiling,
                           info->ChipFamily >= CHIP_FAMILY_RV350)) {
        tiling &= ~RADEON_TILING_MACRO;
    }

    heighta = RADEON_ALIGN(height, drmmode_get_height_align(pScrn, tiling));
    pitch = RADEON_ALIGN(width, drmmode_get_pitch_align(pScrn, cpp, tiling)) * cpp;
    base_align = drmmode_get_base_align(pScrn, cpp, tiling);
    size = RADEON_ALIGN(heighta * pitch, RADEON_GPU_PAGE_SIZE);

    if (width && info->surf_man) {
	if (!radeon_surface_initialize(info, &surface, width, height, cpp,
				       tiling, usage_hint))
	    return NULL;

	size = surface.bo_size;
	base_align = surface.bo_alignment;
	pitch = surface.level[0].pitch_bytes;
	tiling = 0;
	switch (surface.level[0].mode) {
	case RADEON_SURF_MODE_2D:
	    tiling |= RADEON_TILING_MACRO;
	    tiling |= surface.bankw << RADEON_TILING_EG_BANKW_SHIFT;
	    tiling |= surface.bankh << RADEON_TILING_EG_BANKH_SHIFT;
	    tiling |= surface.mtilea << RADEON_TILING_EG_MACRO_TILE_ASPECT_SHIFT;
	    if (surface.tile_split)
		tiling |= eg_tile_split(surface.tile_split) << RADEON_TILING_EG_TILE_SPLIT_SHIFT;
	    if (surface.flags & RADEON_SURF_SBUFFER)
		tiling |= eg_tile_split(surface.stencil_tile_split) << RADEON_TILING_EG_STENCIL_TILE_SPLIT_SHIFT;
	    break;
	case RADEON_SURF_MODE_1D:
	    tiling |= RADEON_TILING_MICRO;
	    break;
	default:
	    break;
	}

	if (new_surface)
	    *new_surface = surface;
    }

    if (tiling)
	flags |= RADEON_GEM_NO_CPU_ACCESS;

    bo = calloc(1, sizeof(struct radeon_buffer));
    if (!bo)
	return NULL;

    bo->ref_count = 1;
    bo->bo.radeon = radeon_bo_open(info->bufmgr, 0, size, base_align,
				   domain, flags);

    if (bo && tiling && radeon_bo_set_tiling(bo->bo.radeon, tiling, pitch) == 0)
	*new_tiling = tiling;

    *new_pitch = pitch;
    return bo;
}


/* Flush and wait for the BO to become idle */
void
radeon_finish(ScrnInfoPtr scrn, struct radeon_buffer *bo)
{
    RADEONInfoPtr info = RADEONPTR(scrn);

    if (info->use_glamor) {
	radeon_glamor_finish(scrn);
	return;
    }

    radeon_cs_flush_indirect(scrn);
    radeon_bo_wait(bo->bo.radeon);
}


/* Clear the pixmap contents to black */
void
radeon_pixmap_clear(PixmapPtr pixmap)
{
    ScreenPtr screen = pixmap->drawable.pScreen;
    RADEONInfoPtr info = RADEONPTR(xf86ScreenToScrn(screen));
    GCPtr gc = GetScratchGC(pixmap->drawable.depth, screen);
    Bool force = info->accel_state->force;
    xRectangle rect;

    info->accel_state->force = TRUE;
    ValidateGC(&pixmap->drawable, gc);
    rect.x = 0;
    rect.y = 0;
    rect.width = pixmap->drawable.width;
    rect.height = pixmap->drawable.height;
    gc->ops->PolyFillRect(&pixmap->drawable, gc, 1, &rect);
    FreeScratchGC(gc);
    info->accel_state->force = force;
}

/* Get GEM handle for the pixmap */
Bool radeon_get_pixmap_handle(PixmapPtr pixmap, uint32_t *handle)
{
    struct radeon_buffer *bo = radeon_get_pixmap_bo(pixmap);
#ifdef USE_GLAMOR
    ScreenPtr screen = pixmap->drawable.pScreen;
    ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
    RADEONInfoPtr info = RADEONPTR(scrn);
#endif

    if (bo && !(bo->flags & RADEON_BO_FLAGS_GBM)) {
	*handle = bo->bo.radeon->handle;
	return TRUE;
    }

#ifdef USE_GLAMOR
    if (info->use_glamor) {
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);
	CARD16 stride;
	CARD32 size;
	int fd, r;

	if (!priv) {
	    priv = calloc(1, sizeof(*priv));
	    radeon_set_pixmap_private(pixmap, priv);
	}

	if (priv->handle_valid) {
	    *handle = priv->handle;
	    return TRUE;
	}

	fd = glamor_fd_from_pixmap(screen, pixmap, &stride, &size);
	if (fd < 0)
	    return FALSE;

	r = drmPrimeFDToHandle(pRADEONEnt->fd, fd, &priv->handle);
	close(fd);
	if (r == 0) {
	    struct drm_radeon_gem_set_tiling args = { .handle = priv->handle };

	    priv->handle_valid = TRUE;
	    *handle = priv->handle;

	    if (drmCommandWriteRead(pRADEONEnt->fd,
				    DRM_RADEON_GEM_GET_TILING, &args,
				    sizeof(args)) == 0)
		priv->tiling_flags = args.tiling_flags;

	    return TRUE;
	}
    }
#endif

    return FALSE;
}

uint32_t radeon_get_pixmap_tiling_flags(PixmapPtr pPix)
{
#ifdef USE_GLAMOR
    RADEONInfoPtr info = RADEONPTR(xf86ScreenToScrn(pPix->drawable.pScreen));

    if (info->use_glamor) {
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pPix);

	if (!priv || (!priv->bo && !priv->handle_valid)) {
	    uint32_t handle;

	    radeon_get_pixmap_handle(pPix, &handle);
	    priv = radeon_get_pixmap_private(pPix);
	}

	return priv ? priv->tiling_flags : 0;
    } else
#endif
    {
	struct radeon_exa_pixmap_priv *driver_priv;
	driver_priv = exaGetPixmapDriverPrivate(pPix);
	return driver_priv ? driver_priv->tiling_flags : 0;
    }
}


Bool radeon_share_pixmap_backing(struct radeon_bo *bo, void **handle_p)
{
    int handle;

    if (radeon_gem_prime_share_bo(bo, &handle) != 0)
	return FALSE;

    *handle_p = (void *)(long)handle;
    return TRUE;
}

Bool radeon_set_shared_pixmap_backing(PixmapPtr ppix, void *fd_handle,
				      struct radeon_surface *surface)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(ppix->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_buffer *bo;
    int ihandle = (int)(long)fd_handle;
    uint32_t size = ppix->devKind * ppix->drawable.height;
    Bool ret = FALSE;

    if (ihandle == -1)
	return radeon_set_pixmap_bo(ppix, NULL);

    bo = (struct radeon_buffer *)calloc(1, sizeof(struct radeon_buffer));
    if (!bo)
	goto error;

#ifdef USE_GLAMOR
    if (info->use_glamor) {
	struct gbm_import_fd_data data;
	uint32_t bo_use = GBM_BO_USE_RENDERING;

	data.format = radeon_get_gbm_format(ppix->drawable.depth,
					    ppix->drawable.bitsPerPixel);
	if (data.format == ~0U)
	    goto error;

	bo->ref_count = 1;

	data.fd = ihandle;
	data.width = ppix->drawable.width;
	data.height = ppix->drawable.height;
	data.stride = ppix->devKind;

	if (ppix->drawable.bitsPerPixel == pScrn->bitsPerPixel)
	    bo_use |= GBM_BO_USE_SCANOUT;

	bo->bo.gbm = gbm_bo_import(info->gbm, GBM_BO_IMPORT_FD, &data, bo_use);
	if (!bo->bo.gbm)
	    goto error;

	bo->flags |= RADEON_BO_FLAGS_GBM;

	if (!radeon_glamor_create_textured_pixmap(ppix, bo)) {
	    radeon_buffer_unref(&bo);
	    return FALSE;
	}

	ret = radeon_set_pixmap_bo(ppix, bo);
	/* radeon_set_pixmap_bo increments ref_count if it succeeds */
	radeon_buffer_unref(&bo);
	return ret;
    }
#endif

    bo->bo.radeon = radeon_gem_bo_open_prime(info->bufmgr, ihandle, size);
    if (!bo)
        goto error;

    bo->ref_count = 1;
    ret = radeon_set_pixmap_bo(ppix, bo);
    if (!ret)
	goto error;

    if (surface) {
	struct radeon_exa_pixmap_priv *driver_priv;
	uint32_t tiling_flags;

	driver_priv = exaGetPixmapDriverPrivate(ppix);
	tiling_flags = driver_priv->tiling_flags;

	if (!radeon_surface_initialize(info, surface, ppix->drawable.width,
				       ppix->drawable.height,
				       ppix->drawable.bitsPerPixel / 8,
				       tiling_flags, 0)) {
	    ret = FALSE;
	    goto error;
	}

	/* we have to post hack the surface to reflect the actual size
	   of the shared pixmap */
	surface->level[0].pitch_bytes = ppix->devKind;
	surface->level[0].nblk_x = ppix->devKind / surface->bpe;
    }

 error:
    close(ihandle);
    /* we have a reference from the alloc and one from set pixmap bo,
       drop one */
    radeon_buffer_unref(&bo);
    return ret;
}
