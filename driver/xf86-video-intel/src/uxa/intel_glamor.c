/*
 * Copyright © 2011 Intel Corporation.
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
 *
 * Authors:
 *    Zhigang Gong <zhigang.gong@linux.intel.com>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xorg-server.h"
#include <xf86.h>
#define GLAMOR_FOR_XORG  1
#include <glamor.h>

#include "intel.h"
#include "i915_drm.h"
#include "intel_glamor.h"
#include "uxa.h"
#include "intel_options.h"

void
intel_glamor_exchange_buffers(struct intel_screen_private *intel,
			      PixmapPtr src,
			      PixmapPtr dst)
{
	if (!(intel->uxa_flags & UXA_USE_GLAMOR))
		return;
	glamor_egl_exchange_buffers(src, dst);
}

XF86VideoAdaptorPtr intel_glamor_xv_init(ScreenPtr screen, int num_ports)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if ((intel->uxa_flags & UXA_USE_GLAMOR) == 0)
		return NULL;

	return glamor_xv_init(screen, num_ports);
}

Bool
intel_glamor_create_screen_resources(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (!(intel->uxa_flags & UXA_USE_GLAMOR))
		return TRUE;

	if (!glamor_glyphs_init(screen))
		return FALSE;

	if (!glamor_egl_create_textured_screen_ext(screen,
						   intel->front_buffer->handle,
						   intel->front_pitch,
						   &intel->back_pixmap))
		return FALSE;

	return TRUE;
}

static Bool
intel_glamor_enabled(intel_screen_private *intel)
{
	const char *s;

	s = xf86GetOptValString(intel->Options, OPTION_ACCEL_METHOD);
	if (s == NULL)
		return IS_DEFAULT_ACCEL_METHOD(GLAMOR);

	return strcasecmp(s, "glamor") == 0;
}

Bool
intel_glamor_pre_init(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	pointer glamor_module;
	CARD32 version;

	if (!intel_glamor_enabled(intel))
		return TRUE;

#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,15,0,0,0)
	if (!xf86LoaderCheckSymbol("glamor_egl_init")) {
		xf86DrvMsg(scrn->scrnIndex,  X_ERROR,
			   "glamor requires Load \"glamoregl\" in "
			   "Section \"Module\", disabling.\n");
		return TRUE;
	}
#endif

	/* Load glamor module */
	if ((glamor_module = xf86LoadSubModule(scrn, GLAMOR_EGL_MODULE_NAME))) {
		version = xf86GetModuleVersion(glamor_module);
		if (version < MODULE_VERSION_NUMERIC(0,3,1)) {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			"Incompatible glamor version, required >= 0.3.0.\n");
		} else {
			if (glamor_egl_init(scrn, intel->drmSubFD)) {
				xf86DrvMsg(scrn->scrnIndex, X_INFO,
					   "glamor detected, initialising egl layer.\n");
				intel->uxa_flags = UXA_GLAMOR_EGL_INITIALIZED;
			} else
				xf86DrvMsg(scrn->scrnIndex, X_WARNING,
					   "glamor detected, failed to initialize egl.\n");
		}
	} else
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "glamor not available\n");

	return TRUE;
}

PixmapPtr
intel_glamor_create_pixmap(ScreenPtr screen, int w, int h,
			   int depth, unsigned int usage)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->uxa_flags & UXA_USE_GLAMOR)
		return glamor_create_pixmap(screen, w, h, depth, usage);
	else
		return NULL;
}

Bool
intel_glamor_create_textured_pixmap(PixmapPtr pixmap)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pixmap->drawable.pScreen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct intel_pixmap *priv;

	if ((intel->uxa_flags & UXA_USE_GLAMOR) == 0)
		return TRUE;

	priv = intel_get_pixmap_private(pixmap);
	if (glamor_egl_create_textured_pixmap(pixmap, priv->bo->handle,
					      priv->stride)) {
		drm_intel_bo_disable_reuse(priv->bo);
		priv->pinned |= PIN_GLAMOR;
		return TRUE;
	} else
		return FALSE;
}

void
intel_glamor_destroy_pixmap(PixmapPtr pixmap)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pixmap->drawable.pScreen);
	intel_screen_private * intel;

	intel = intel_get_screen_private(scrn);
	if (intel->uxa_flags & UXA_USE_GLAMOR)
		glamor_egl_destroy_textured_pixmap(pixmap);
}

static void
intel_glamor_need_flush(DrawablePtr pDrawable)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
	intel_screen_private * intel;

	intel = intel_get_screen_private(scrn);
	intel->needs_flush = TRUE;
}

static void
intel_glamor_finish_access(PixmapPtr pixmap, uxa_access_t access)
{
	switch(access) {
	case UXA_ACCESS_RO:
	case UXA_ACCESS_RW:
	case UXA_GLAMOR_ACCESS_RO:
		break;
	case UXA_GLAMOR_ACCESS_RW:
		intel_glamor_need_flush(&pixmap->drawable);
		break;
	default:
		ErrorF("Invalid access mode %d\n", access);
	}

	return;
}

Bool
intel_glamor_init(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if ((intel->uxa_flags & UXA_GLAMOR_EGL_INITIALIZED) == 0)
		goto fail;

	if (!glamor_init(screen,
#if defined(GLAMOR_NO_DRI3)
			 /* Not doing DRI3 yet, since Present support hasn't landed. */
			 GLAMOR_NO_DRI3 |
#endif
			 GLAMOR_INVERTED_Y_AXIS |
			 GLAMOR_USE_EGL_SCREEN)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Failed to initialize glamor.\n");
		goto fail;
	}

	if (!glamor_egl_init_textured_pixmap(screen)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Failed to initialize textured pixmap of screen for glamor.\n");
		goto fail;
	}

	intel->uxa_driver->flags |= UXA_USE_GLAMOR;
	intel->uxa_flags |= intel->uxa_driver->flags;

	intel->uxa_driver->finish_access = intel_glamor_finish_access;

	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "Use GLAMOR acceleration.\n");
	return TRUE;

  fail:
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "Use legacy UXA acceleration.\n");
	return FALSE;
}

void
intel_glamor_flush(intel_screen_private * intel)
{
	ScreenPtr screen;

	screen = xf86ScrnToScreen(intel->scrn);
	if (intel->uxa_flags & UXA_USE_GLAMOR)
		glamor_block_handler(screen);
}

Bool
intel_glamor_close_screen(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->uxa_flags & UXA_USE_GLAMOR)
		intel->uxa_flags &= ~UXA_USE_GLAMOR;

	return TRUE;
}
