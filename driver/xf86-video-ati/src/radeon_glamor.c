/*
 * Copyright © 2011 Intel Corporation.
 *             2012 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including
 * the next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xf86.h>

#include "radeon.h"
#include "radeon_bo_helper.h"
#include "radeon_glamor.h"

#if HAS_DEVPRIVATEKEYREC
DevPrivateKeyRec glamor_pixmap_index;
#else
int glamor_pixmap_index;
#endif

void
radeon_glamor_exchange_buffers(PixmapPtr src,
			       PixmapPtr dst)
{
	RADEONInfoPtr info = RADEONPTR(xf86ScreenToScrn(dst->drawable.pScreen));

	if (!info->use_glamor)
		return;
	glamor_egl_exchange_buffers(src, dst);
}

Bool
radeon_glamor_create_screen_resources(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONInfoPtr info = RADEONPTR(scrn);

	if (!info->use_glamor)
		return TRUE;

#ifdef HAVE_GLAMOR_GLYPHS_INIT
	if (!glamor_glyphs_init(screen))
		return FALSE;
#endif

	if (!glamor_egl_create_textured_screen_ext(screen,
						   info->front_bo->handle,
						   scrn->displayWidth *
						   info->pixel_bytes,
						   NULL))
		return FALSE;

	return TRUE;
}


Bool
radeon_glamor_pre_init(ScrnInfoPtr scrn)
{
	RADEONInfoPtr info = RADEONPTR(scrn);
	pointer glamor_module;
	CARD32 version;
	const char *s;

	if (!info->dri2.available)
		return FALSE;

	s = xf86GetOptValString(info->Options, OPTION_ACCELMETHOD);
	if (s == NULL && info->ChipFamily < CHIP_FAMILY_TAHITI)
		return FALSE;

	if (s && strcasecmp(s, "glamor") != 0) {
		if (info->ChipFamily >= CHIP_FAMILY_TAHITI)
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "EXA not supported, using glamor\n");
		else
			return FALSE;
	}

	if (info->ChipFamily < CHIP_FAMILY_R300) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "glamor requires R300 or higher GPU, disabling.\n");
		return FALSE;
	}

	if (info->ChipFamily < CHIP_FAMILY_RV515) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "glamor may not work (well) with GPUs < RV515.\n");
	}

	if (scrn->depth < 24) {
		xf86DrvMsg(scrn->scrnIndex, s ? X_ERROR : X_WARNING,
			   "glamor requires depth >= 24, disabling.\n");
		return FALSE;
	}

#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,15,0,0,0)
	if (!xf86LoaderCheckSymbol("glamor_egl_init")) {
		xf86DrvMsg(scrn->scrnIndex, s ? X_ERROR : X_WARNING,
			   "glamor requires Load \"glamoregl\" in "
			   "Section \"Module\", disabling.\n");
		return FALSE;
	}
#endif

	/* Load glamor module */
	if ((glamor_module = xf86LoadSubModule(scrn, GLAMOR_EGL_MODULE_NAME))) {
		version = xf86GetModuleVersion(glamor_module);
		if (version < MODULE_VERSION_NUMERIC(0,3,1)) {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			"Incompatible glamor version, required >= 0.3.0.\n");
			return FALSE;
		} else {
			if (glamor_egl_init(scrn, info->dri2.drm_fd)) {
				xf86DrvMsg(scrn->scrnIndex, X_INFO,
					   "glamor detected, initialising EGL layer.\n");
			} else {
				xf86DrvMsg(scrn->scrnIndex, X_ERROR,
					   "glamor detected, failed to initialize EGL.\n");
				return FALSE;
			}
		}
	} else {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR, "glamor not available\n");
		return FALSE;
	}

	info->use_glamor = TRUE;

	return TRUE;
}

Bool
radeon_glamor_create_textured_pixmap(PixmapPtr pixmap, struct radeon_pixmap *priv)
{
	return glamor_egl_create_textured_pixmap(pixmap, priv->bo->handle,
						 pixmap->devKind);
}

static PixmapPtr
radeon_glamor_create_pixmap(ScreenPtr screen, int w, int h, int depth,
			unsigned usage)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONInfoPtr info = RADEONPTR(scrn);
	struct radeon_pixmap *priv;
	PixmapPtr pixmap, new_pixmap = NULL;

	if (!RADEON_CREATE_PIXMAP_SHARED(usage)) {
		if (info->shadow_primary) {
			if (usage != CREATE_PIXMAP_USAGE_BACKING_PIXMAP)
				return fbCreatePixmap(screen, w, h, depth, usage);
		} else {
			pixmap = glamor_create_pixmap(screen, w, h, depth, usage);
			if (pixmap)
			    return pixmap;
		}
	}

	if (w > 32767 || h > 32767)
		return NullPixmap;

	if (depth == 1)
		return fbCreatePixmap(screen, w, h, depth, usage);

	if (usage == CREATE_PIXMAP_USAGE_GLYPH_PICTURE && w <= 32 && h <= 32)
		return fbCreatePixmap(screen, w, h, depth, usage);

	pixmap = fbCreatePixmap(screen, 0, 0, depth, usage);
	if (pixmap == NullPixmap)
		return pixmap;

	if (w && h) {
		int stride;

		priv = calloc(1, sizeof (struct radeon_pixmap));
		if (priv == NULL)
			goto fallback_pixmap;

		priv->bo = radeon_alloc_pixmap_bo(scrn, w, h, depth, usage,
						  pixmap->drawable.bitsPerPixel,
						  &stride,
						  &priv->surface,
						  &priv->tiling_flags);
		if (!priv->bo)
			goto fallback_priv;

		radeon_set_pixmap_private(pixmap, priv);

		screen->ModifyPixmapHeader(pixmap, w, h, 0, 0, stride, NULL);

		if (!radeon_glamor_create_textured_pixmap(pixmap, priv))
			goto fallback_glamor;

		pixmap->devPrivate.ptr = NULL;
	}

	return pixmap;

fallback_glamor:
	if (RADEON_CREATE_PIXMAP_SHARED(usage)) {
	/* XXX need further work to handle the DRI2 failure case.
	 * Glamor don't know how to handle a BO only pixmap. Put
	 * a warning indicator here.
	 */
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "Failed to create textured DRI2/PRIME pixmap.");
		return pixmap;
	}
	/* Create textured pixmap failed means glamor failed to
	 * create a texture from current BO for some reasons. We turn
	 * to create a new glamor pixmap and clean up current one.
	 * One thing need to be noted, this new pixmap doesn't
	 * has a priv and bo attached to it. It's glamor's responsbility
	 * to take care of it. Glamor will mark this new pixmap as a
	 * texture only pixmap and will never fallback to DDX layer
	 * afterwards.
	 */
	new_pixmap = glamor_create_pixmap(screen, w, h,	depth, usage);
	radeon_bo_unref(priv->bo);
fallback_priv:
	free(priv);
fallback_pixmap:
	fbDestroyPixmap(pixmap);
	if (new_pixmap)
		return new_pixmap;
	else
		return fbCreatePixmap(screen, w, h, depth, usage);
}

static Bool radeon_glamor_destroy_pixmap(PixmapPtr pixmap)
{
	if (pixmap->refcnt == 1) {
		if (pixmap->devPrivate.ptr) {
			struct radeon_bo *bo = radeon_get_pixmap_bo(pixmap);

			if (bo)
				radeon_bo_unmap(bo);
		}

		glamor_egl_destroy_textured_pixmap(pixmap);
		radeon_set_pixmap_bo(pixmap, NULL);
	}
	fbDestroyPixmap(pixmap);
	return TRUE;
}

#ifdef RADEON_PIXMAP_SHARING

static Bool
radeon_glamor_share_pixmap_backing(PixmapPtr pixmap, ScreenPtr slave,
				   void **handle_p)
{
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

	if (!priv)
		return FALSE;

	return radeon_share_pixmap_backing(priv->bo, handle_p);
}

static Bool
radeon_glamor_set_shared_pixmap_backing(PixmapPtr pixmap, void *handle)
{
	ScreenPtr screen = pixmap->drawable.pScreen;
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	struct radeon_surface surface;
	struct radeon_pixmap *priv;

	if (!radeon_set_shared_pixmap_backing(pixmap, handle, &surface))
		return FALSE;

	priv = radeon_get_pixmap_private(pixmap);
	priv->surface = surface;

	if (!radeon_glamor_create_textured_pixmap(pixmap, priv)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Failed to get PRIME drawable for glamor pixmap.\n");
		return FALSE;
	}

	screen->ModifyPixmapHeader(pixmap,
				   pixmap->drawable.width,
				   pixmap->drawable.height,
				   0, 0, 0, NULL);

	return TRUE;
}

#endif /* RADEON_PIXMAP_SHARING */

Bool
radeon_glamor_init(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONInfoPtr info = RADEONPTR(scrn);
#ifdef RENDER
#ifdef HAVE_FBGLYPHS
	UnrealizeGlyphProcPtr SavedUnrealizeGlyph = NULL;
#endif
	PictureScreenPtr ps = NULL;

	if (info->shadow_primary) {
		ps = GetPictureScreenIfSet(screen);

		if (ps) {
#ifdef HAVE_FBGLYPHS
			SavedUnrealizeGlyph = ps->UnrealizeGlyph;
#endif
			info->glamor.SavedGlyphs = ps->Glyphs;
			info->glamor.SavedTriangles = ps->Triangles;
			info->glamor.SavedTrapezoids = ps->Trapezoids;
		}
	}
#endif /* RENDER */

	if (!glamor_init(screen, GLAMOR_USE_EGL_SCREEN | GLAMOR_USE_SCREEN |
			 GLAMOR_USE_PICTURE_SCREEN | GLAMOR_INVERTED_Y_AXIS |
			 GLAMOR_NO_DRI3)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Failed to initialize glamor.\n");
		return FALSE;
	}

	if (!glamor_egl_init_textured_pixmap(screen)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Failed to initialize textured pixmap of screen for glamor.\n");
		return FALSE;
	}

#if HAS_DIXREGISTERPRIVATEKEY
	if (!dixRegisterPrivateKey(&glamor_pixmap_index, PRIVATE_PIXMAP, 0))
#else
	if (!dixRequestPrivate(&glamor_pixmap_index, 0))
#endif
		return FALSE;

	if (info->shadow_primary)
		radeon_glamor_screen_init(screen);

#if defined(RENDER) && defined(HAVE_FBGLYPHS)
	/* For ShadowPrimary, we need fbUnrealizeGlyph instead of
	 * glamor_unrealize_glyph
	 */
	if (ps)
		ps->UnrealizeGlyph = SavedUnrealizeGlyph;
#endif

	screen->CreatePixmap = radeon_glamor_create_pixmap;
	screen->DestroyPixmap = radeon_glamor_destroy_pixmap;
#ifdef RADEON_PIXMAP_SHARING
	screen->SharePixmapBacking = radeon_glamor_share_pixmap_backing;
	screen->SetSharedPixmapBacking = radeon_glamor_set_shared_pixmap_backing;
#endif

	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "Use GLAMOR acceleration.\n");
	return TRUE;
}

XF86VideoAdaptorPtr radeon_glamor_xv_init(ScreenPtr pScreen, int num_adapt)
{
	return glamor_xv_init(pScreen, num_adapt);
}
