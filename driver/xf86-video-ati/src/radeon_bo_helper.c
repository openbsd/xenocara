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

#ifdef RADEON_PIXMAP_SHARING
#include "radeon_bo_gem.h"
#endif

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

/* Calculate appropriate tiling and pitch for a pixmap and allocate a BO that
 * can hold it.
 */
struct radeon_bo*
radeon_alloc_pixmap_bo(ScrnInfoPtr pScrn, int width, int height, int depth,
		       int usage_hint, int bitsPerPixel, int *new_pitch,
		       struct radeon_surface *new_surface, uint32_t *new_tiling)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    int pitch, base_align;
    uint32_t size, heighta;
    int cpp = bitsPerPixel / 8;
    uint32_t tiling = 0;
    struct radeon_surface surface;
    struct radeon_bo *bo;
    int domain = RADEON_GEM_DOMAIN_VRAM;
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
	     info->shadow_primary)
#ifdef CREATE_PIXMAP_USAGE_SHARED
	    || (usage_hint & 0xffff) == CREATE_PIXMAP_USAGE_SHARED
#endif
	    ) {
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
    memset(&surface, 0, sizeof(struct radeon_surface));

    if (info->ChipFamily >= CHIP_FAMILY_R600 && info->surf_man) {
		if (width) {
			surface.npix_x = width;
			/* need to align height to 8 for old kernel */
			surface.npix_y = RADEON_ALIGN(height, 8);
			surface.npix_z = 1;
			surface.blk_w = 1;
			surface.blk_h = 1;
			surface.blk_d = 1;
			surface.array_size = 1;
			surface.last_level = 0;
			surface.bpe = cpp;
			surface.nsamples = 1;
			if (height < 128) {
				/* disable 2d tiling for small surface to work around
				 * the fact that ddx align height to 8 pixel for old
				 * obscure reason i can't remember
				 */
				tiling &= ~RADEON_TILING_MACRO;
			}
			surface.flags = RADEON_SURF_SCANOUT;
			/* we are requiring a recent enough libdrm version */
			surface.flags |= RADEON_SURF_HAS_TILE_MODE_INDEX;
			surface.flags |= RADEON_SURF_SET(RADEON_SURF_TYPE_2D, TYPE);
			surface.flags |= RADEON_SURF_SET(RADEON_SURF_MODE_LINEAR, MODE);
			if ((tiling & RADEON_TILING_MICRO)) {
				surface.flags = RADEON_SURF_CLR(surface.flags, MODE);
				surface.flags |= RADEON_SURF_SET(RADEON_SURF_MODE_1D, MODE);
			}
			if ((tiling & RADEON_TILING_MACRO)) {
				surface.flags = RADEON_SURF_CLR(surface.flags, MODE);
				surface.flags |= RADEON_SURF_SET(RADEON_SURF_MODE_2D, MODE);
			}
			if (usage_hint & RADEON_CREATE_PIXMAP_SZBUFFER) {
				surface.flags |= RADEON_SURF_ZBUFFER;
				surface.flags |= RADEON_SURF_SBUFFER;
			}
			if (radeon_surface_best(info->surf_man, &surface)) {
				return NULL;
			}
			if (radeon_surface_init(info->surf_man, &surface)) {
				return NULL;
			}
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
				tiling |= eg_tile_split(surface.stencil_tile_split) << RADEON_TILING_EG_STENCIL_TILE_SPLIT_SHIFT;
				break;
			case RADEON_SURF_MODE_1D:
				tiling |= RADEON_TILING_MICRO;
				break;
			default:
				break;
			}
		}
	}

    bo = radeon_bo_open(info->bufmgr, 0, size, base_align,
			domain, 0);

    if (bo && tiling && radeon_bo_set_tiling(bo, tiling, pitch) == 0)
	*new_tiling = tiling;

    *new_surface = surface;
    *new_pitch = pitch;
    return bo;
}

/* Get GEM handle for the pixmap */
Bool radeon_get_pixmap_handle(PixmapPtr pixmap, uint32_t *handle)
{
    struct radeon_bo *bo = radeon_get_pixmap_bo(pixmap);
#ifdef USE_GLAMOR
    ScreenPtr screen = pixmap->drawable.pScreen;
    RADEONInfoPtr info = RADEONPTR(xf86ScreenToScrn(screen));
#endif

    if (bo) {
	*handle = bo->handle;
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

	r = drmPrimeFDToHandle(info->dri2.drm_fd, fd, &priv->handle);
	close(fd);
	if (r == 0) {
	    struct drm_radeon_gem_set_tiling args = { .handle = priv->handle };

	    priv->handle_valid = TRUE;
	    *handle = priv->handle;

	    if (drmCommandWriteRead(info->dri2.drm_fd,
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

#ifdef RADEON_PIXMAP_SHARING

Bool radeon_share_pixmap_backing(struct radeon_bo *bo, void **handle_p)
{
    int handle;

    if (radeon_gem_prime_share_bo(bo, &handle) != 0)
	return FALSE;

    *handle_p = (void *)(long)handle;
    return TRUE;
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

Bool radeon_set_shared_pixmap_backing(PixmapPtr ppix, void *fd_handle,
				      struct radeon_surface *surface)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(ppix->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_bo *bo;
    int ihandle = (int)(long)fd_handle;
    uint32_t size = ppix->devKind * ppix->drawable.height;

    bo = radeon_gem_bo_open_prime(info->bufmgr, ihandle, size);
    if (!bo)
        return FALSE;

    memset(surface, 0, sizeof(struct radeon_surface));

    radeon_set_pixmap_bo(ppix, bo);

    if (info->ChipFamily >= CHIP_FAMILY_R600 && info->surf_man) {
	uint32_t tiling_flags;

#ifdef USE_GLAMOR
	if (info->use_glamor) {
	    tiling_flags = radeon_get_pixmap_private(ppix)->tiling_flags;
	} else
#endif
	{
	    struct radeon_exa_pixmap_priv *driver_priv;

	    driver_priv = exaGetPixmapDriverPrivate(ppix);
	    tiling_flags = driver_priv->tiling_flags;
	}

	surface->npix_x = ppix->drawable.width;
	surface->npix_y = ppix->drawable.height;
	surface->npix_z = 1;
	surface->blk_w = 1;
	surface->blk_h = 1;
	surface->blk_d = 1;
	surface->array_size = 1;
	surface->bpe = ppix->drawable.bitsPerPixel / 8;
	surface->nsamples = 1;
	/* we are requiring a recent enough libdrm version */
	surface->flags |= RADEON_SURF_HAS_TILE_MODE_INDEX;
	surface->flags |= RADEON_SURF_SET(RADEON_SURF_TYPE_2D, TYPE);
	if (tiling_flags & RADEON_TILING_MACRO)
	    surface->flags |= RADEON_SURF_SET(RADEON_SURF_MODE_2D, MODE);
	else if (tiling_flags & RADEON_TILING_MICRO)
	    surface->flags |= RADEON_SURF_SET(RADEON_SURF_MODE_1D, MODE);
	else
	    surface->flags |= RADEON_SURF_SET(RADEON_SURF_MODE_LINEAR_ALIGNED, MODE);
	surface->bankw = (tiling_flags >> RADEON_TILING_EG_BANKW_SHIFT) & RADEON_TILING_EG_BANKW_MASK;
	surface->bankh = (tiling_flags >> RADEON_TILING_EG_BANKH_SHIFT) & RADEON_TILING_EG_BANKH_MASK;
	surface->tile_split = eg_tile_split_opp((tiling_flags >> RADEON_TILING_EG_TILE_SPLIT_SHIFT) & RADEON_TILING_EG_TILE_SPLIT_MASK);
	surface->stencil_tile_split = (tiling_flags >> RADEON_TILING_EG_STENCIL_TILE_SPLIT_SHIFT) & RADEON_TILING_EG_STENCIL_TILE_SPLIT_MASK;
	surface->mtilea = (tiling_flags >> RADEON_TILING_EG_MACRO_TILE_ASPECT_SHIFT) & RADEON_TILING_EG_MACRO_TILE_ASPECT_MASK;
	if (radeon_surface_best(info->surf_man, surface)) {
	    return FALSE;
	}
	if (radeon_surface_init(info->surf_man, surface)) {
	    return FALSE;
	}
	/* we have to post hack the surface to reflect the actual size
	   of the shared pixmap */
	surface->level[0].pitch_bytes = ppix->devKind;
	surface->level[0].nblk_x = ppix->devKind / surface->bpe;
    }

    close(ihandle);
    /* we have a reference from the alloc and one from set pixmap bo,
       drop one */
    radeon_bo_unref(bo);
    return TRUE;
}

#endif /* RADEON_PIXMAP_SHARING */
