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

DevPrivateKeyRec glamor_pixmap_index;

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
	PixmapPtr screen_pixmap = screen->GetScreenPixmap(screen);
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONInfoPtr info = RADEONPTR(scrn);

	if (!info->use_glamor)
		return TRUE;

#ifdef HAVE_GLAMOR_GLYPHS_INIT
	if (!glamor_glyphs_init(screen))
		return FALSE;
#endif

	return radeon_glamor_create_textured_pixmap(screen_pixmap,
						    info->front_buffer);
}


Bool
radeon_glamor_pre_init(ScrnInfoPtr scrn)
{
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
	RADEONInfoPtr info = RADEONPTR(scrn);
	pointer glamor_module;
	CARD32 version;
	const char *s;

	if (!info->dri2.available)
		return FALSE;

	s = xf86GetOptValString(info->Options, OPTION_ACCELMETHOD);
	if (!s) {
		if (xorgGetVersion() >= XORG_VERSION_NUMERIC(1,18,3,0,0)) {
			if (info->ChipFamily < CHIP_FAMILY_R600)
				return FALSE;
		} else {
			if (info->ChipFamily < CHIP_FAMILY_TAHITI)
				return FALSE;
		}
	}

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

#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,20,99,0,0)
	if (scrn->depth < 24) {
#else
	if (scrn->depth < 15) {
#endif
		xf86DrvMsg(scrn->scrnIndex, s ? X_ERROR : X_WARNING,
			   "Depth %d not supported with glamor, disabling\n",
			   scrn->depth);
		return FALSE;
	}

	if (scrn->depth == 30 &&
	    xorgGetVersion() < XORG_VERSION_NUMERIC(1,19,99,1,0)) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "Depth 30 is not supported by GLAMOR with Xorg < "
			   "1.19.99.1\n");
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

	info->gbm = gbm_create_device(pRADEONEnt->fd);
	if (!info->gbm) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "gbm_create_device returned NULL\n");
		return FALSE;
	}

	/* Load glamor module */
	if ((glamor_module = xf86LoadSubModule(scrn, GLAMOR_EGL_MODULE_NAME))) {
		version = xf86GetModuleVersion(glamor_module);
		if (version < MODULE_VERSION_NUMERIC(0,3,1)) {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			"Incompatible glamor version, required >= 0.3.0.\n");
			return FALSE;
		} else {
			if (glamor_egl_init(scrn, pRADEONEnt->fd)) {
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
radeon_glamor_create_textured_pixmap(PixmapPtr pixmap, struct radeon_buffer *bo)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pixmap->drawable.pScreen);
	RADEONInfoPtr info = RADEONPTR(scrn);

	if (!info->use_glamor)
		return TRUE;

	if (bo->flags & RADEON_BO_FLAGS_GBM) {
		return glamor_egl_create_textured_pixmap_from_gbm_bo(pixmap,
								     bo->bo.gbm
#if XORG_VERSION_CURRENT > XORG_VERSION_NUMERIC(1,19,99,903,0)
								     , FALSE
#endif
								     );
	} else {
		return glamor_egl_create_textured_pixmap(pixmap,
							 bo->bo.radeon->handle,
							 pixmap->devKind);
	}
}

static Bool radeon_glamor_destroy_pixmap(PixmapPtr pixmap)
{
#ifndef HAVE_GLAMOR_EGL_DESTROY_TEXTURED_PIXMAP
	ScreenPtr screen = pixmap->drawable.pScreen;
	RADEONInfoPtr info = RADEONPTR(xf86ScreenToScrn(screen));
	Bool ret;
#endif

	if (pixmap->refcnt == 1) {
#ifdef HAVE_GLAMOR_EGL_DESTROY_TEXTURED_PIXMAP
		glamor_egl_destroy_textured_pixmap(pixmap);
#endif
		radeon_set_pixmap_bo(pixmap, NULL);
	}

#ifdef HAVE_GLAMOR_EGL_DESTROY_TEXTURED_PIXMAP
	fbDestroyPixmap(pixmap);
	return TRUE;
#else
	screen->DestroyPixmap = info->glamor.SavedDestroyPixmap;
	ret = screen->DestroyPixmap(pixmap);
	info->glamor.SavedDestroyPixmap = screen->DestroyPixmap;
	screen->DestroyPixmap = radeon_glamor_destroy_pixmap;

	return ret;
#endif
}

static PixmapPtr
radeon_glamor_create_pixmap(ScreenPtr screen, int w, int h, int depth,
			unsigned usage)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONInfoPtr info = RADEONPTR(scrn);
	struct radeon_pixmap *priv;
	PixmapPtr pixmap, new_pixmap = NULL;

	if (!xf86GetPixFormat(scrn, depth))
		return NULL;

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
		if (!priv)
			goto fallback_pixmap;

		priv->bo = radeon_alloc_pixmap_bo(scrn, w, h, depth, usage,
						  pixmap->drawable.bitsPerPixel,
						  &stride, NULL,
						  &priv->tiling_flags);
		if (!priv->bo)
			goto fallback_priv;

		radeon_set_pixmap_private(pixmap, priv);

		screen->ModifyPixmapHeader(pixmap, w, h, 0, 0, stride, NULL);

		if (!radeon_glamor_create_textured_pixmap(pixmap, priv->bo))
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

		radeon_glamor_destroy_pixmap(pixmap);
		return NullPixmap;
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
	radeon_buffer_unref(&priv->bo);
fallback_priv:
	free(priv);
fallback_pixmap:
	fbDestroyPixmap(pixmap);
	if (new_pixmap)
		return new_pixmap;
	else
		return fbCreatePixmap(screen, w, h, depth, usage);
}

PixmapPtr
radeon_glamor_set_pixmap_bo(DrawablePtr drawable, PixmapPtr pixmap)
{
	PixmapPtr old = get_drawable_pixmap(drawable);
	ScreenPtr screen = drawable->pScreen;
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);
	GCPtr gc;

	/* With a glamor pixmap, 2D pixmaps are created in texture
	 * and without a static BO attached to it. To support DRI,
	 * we need to create a new textured-drm pixmap and
	 * need to copy the original content to this new textured-drm
	 * pixmap, and then convert the old pixmap to a coherent
	 * textured-drm pixmap which has a valid BO attached to it
	 * and also has a valid texture, thus both glamor and DRI2
	 * can access it.
	 *
	 */

	/* Copy the current contents of the pixmap to the bo. */
	gc = GetScratchGC(drawable->depth, screen);
	if (gc) {
		ValidateGC(&pixmap->drawable, gc);
		gc->ops->CopyArea(&old->drawable, &pixmap->drawable,
				  gc,
				  0, 0,
				  old->drawable.width,
				  old->drawable.height, 0, 0);
		FreeScratchGC(gc);
	}

	/* And redirect the pixmap to the new bo (for 3D). */
	glamor_egl_exchange_buffers(old, pixmap);
	radeon_set_pixmap_private(pixmap, radeon_get_pixmap_private(old));
	radeon_set_pixmap_private(old, priv);

	screen->ModifyPixmapHeader(old,
				   old->drawable.width,
				   old->drawable.height,
				   0, 0, pixmap->devKind, NULL);
	old->devPrivate.ptr = NULL;

	screen->DestroyPixmap(pixmap);

	return old;
}


static Bool
radeon_glamor_share_pixmap_backing(PixmapPtr pixmap, ScreenPtr secondary,
				   void **handle_p)
{
	ScreenPtr screen = pixmap->drawable.pScreen;
	CARD16 stride;
	CARD32 size;
	int fd;

	if ((radeon_get_pixmap_tiling_flags(pixmap) &
	     RADEON_TILING_MASK) != RADEON_TILING_LINEAR) {
		PixmapPtr linear;

		/* We don't want to re-allocate the screen pixmap as
		 * linear, to avoid trouble with page flipping
		 */
		if (screen->GetScreenPixmap(screen) == pixmap)
			return FALSE;

		linear = screen->CreatePixmap(screen, pixmap->drawable.width,
					      pixmap->drawable.height,
					      pixmap->drawable.depth,
					      CREATE_PIXMAP_USAGE_SHARED);
		if (!linear)
			return FALSE;

		radeon_glamor_set_pixmap_bo(&pixmap->drawable, linear);
	}

	fd = glamor_fd_from_pixmap(screen, pixmap, &stride, &size);
	if (fd < 0)
		return FALSE;

	*handle_p = (void *)(long)fd;
	return TRUE;
}

static Bool
radeon_glamor_set_shared_pixmap_backing(PixmapPtr pixmap, void *handle)
{
	ScreenPtr screen = pixmap->drawable.pScreen;
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	int ihandle = (int)(long)handle;

	if (!radeon_set_shared_pixmap_backing(pixmap, handle, NULL))
		return FALSE;

	if (ihandle != -1 &&
	    !radeon_glamor_create_textured_pixmap(pixmap,
						  radeon_get_pixmap_bo(pixmap))) {
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

	if (!dixRegisterPrivateKey(&glamor_pixmap_index, PRIVATE_PIXMAP, 0))
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

	info->glamor.SavedCreatePixmap = screen->CreatePixmap;
	screen->CreatePixmap = radeon_glamor_create_pixmap;
	info->glamor.SavedDestroyPixmap = screen->DestroyPixmap;
	screen->DestroyPixmap = radeon_glamor_destroy_pixmap;
	info->glamor.SavedSharePixmapBacking = screen->SharePixmapBacking;
	screen->SharePixmapBacking = radeon_glamor_share_pixmap_backing;
	info->glamor.SavedSetSharedPixmapBacking = screen->SetSharedPixmapBacking;
	screen->SetSharedPixmapBacking = radeon_glamor_set_shared_pixmap_backing;

	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "Use GLAMOR acceleration.\n");
	return TRUE;
}

void
radeon_glamor_fini(ScreenPtr screen)
{
	RADEONInfoPtr info = RADEONPTR(xf86ScreenToScrn(screen));

	if (!info->use_glamor)
		return;

	screen->CreatePixmap = info->glamor.SavedCreatePixmap;
	screen->DestroyPixmap = info->glamor.SavedDestroyPixmap;
	screen->SharePixmapBacking = info->glamor.SavedSharePixmapBacking;
	screen->SetSharedPixmapBacking = info->glamor.SavedSetSharedPixmapBacking;
}

XF86VideoAdaptorPtr radeon_glamor_xv_init(ScreenPtr pScreen, int num_adapt)
{
	return glamor_xv_init(pScreen, num_adapt);
}
