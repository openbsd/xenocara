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

#ifndef RADEON_GLAMOR_H
#define RADEON_GLAMOR_H

#include "xf86xv.h"
#ifdef USE_GLAMOR

#include "radeon_surface.h"

Bool radeon_glamor_pre_init(ScrnInfoPtr scrn);
Bool radeon_glamor_init(ScreenPtr screen);
Bool radeon_glamor_create_screen_resources(ScreenPtr screen);
void radeon_glamor_free_screen(int scrnIndex, int flags);

void radeon_glamor_flush(ScrnInfoPtr pScrn);

Bool radeon_glamor_create_textured_pixmap(PixmapPtr pixmap);
void radeon_glamor_exchange_buffers(PixmapPtr src, PixmapPtr dst);

Bool radeon_glamor_pixmap_is_offscreen(PixmapPtr pixmap);

XF86VideoAdaptorPtr radeon_glamor_xv_init(ScreenPtr pScreen, int num_adapt);

struct radeon_pixmap {
	struct radeon_surface surface;
	struct radeon_bo *bo;

	uint32_t tiling_flags;
	int stride;
};

#if HAS_DEVPRIVATEKEYREC
extern DevPrivateKeyRec glamor_pixmap_index;
#else
extern int glamor_pixmap_index;
#endif

static inline struct radeon_pixmap *radeon_get_pixmap_private(PixmapPtr pixmap)
{
#if HAS_DEVPRIVATEKEYREC
	return dixGetPrivate(&pixmap->devPrivates, &glamor_pixmap_index);
#else
	return dixLookupPrivate(&pixmap->devPrivates, &glamor_pixmap_index);
#endif
}

static inline void radeon_set_pixmap_private(PixmapPtr pixmap, struct radeon_pixmap *priv)
{
	dixSetPrivate(&pixmap->devPrivates, &glamor_pixmap_index, priv);
}

#else

static inline Bool radeon_glamor_pre_init(ScrnInfoPtr scrn) { return FALSE; }
static inline Bool radeon_glamor_init(ScreenPtr screen) { return FALSE; }
static inline Bool radeon_glamor_create_screen_resources(ScreenPtr screen) { return FALSE; }
static inline void radeon_glamor_free_screen(int scrnIndex, int flags) { }

static inline void radeon_glamor_flush(ScrnInfoPtr pScrn) { }

static inline Bool radeon_glamor_create_textured_pixmap(PixmapPtr pixmap) { return TRUE; }

static inline void radeon_glamor_exchange_buffers(PixmapPtr src, PixmapPtr dst) {}

static inline Bool radeon_glamor_pixmap_is_offscreen(PixmapPtr pixmap) { return FALSE; }

static inline struct radeon_pixmap *radeon_get_pixmap_private(PixmapPtr pixmap) { return NULL; }

static inline XF86VideoAdaptorPtr radeon_glamor_xv_init(ScreenPtr pScreen, int num_adapt) { return NULL; }
#endif

#endif /* RADEON_GLAMOR_H */
