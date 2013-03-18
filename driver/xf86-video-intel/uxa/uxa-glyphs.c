/*
 * Copyright © 2010 Intel Corporation
 * Partly based on code Copyright © 2008 Red Hat, Inc.
 * Partly based on code Copyright © 2000 SuSE, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Intel not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Intel makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * INTEL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL INTEL
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Red Hat not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Red Hat makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * Red Hat DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL Red Hat
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of SuSE not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  SuSE makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * SuSE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL SuSE
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Chris Wilson <chris@chris-wilson.co.uk>
 * Based on code by: Keith Packard <keithp@keithp.com> and Owen Taylor <otaylor@fishsoup.net>
 */

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <stdlib.h>

#include "uxa-priv.h"
#include "uxa-glamor.h"
#include "../src/common.h"

#include "mipict.h"

/* Width of the pixmaps we use for the caches; this should be less than
 * max texture size of the driver; this may need to actually come from
 * the driver.
 */
#define CACHE_PICTURE_SIZE 1024
#define GLYPH_MIN_SIZE 8
#define GLYPH_MAX_SIZE 64
#define GLYPH_CACHE_SIZE (CACHE_PICTURE_SIZE * CACHE_PICTURE_SIZE / (GLYPH_MIN_SIZE * GLYPH_MIN_SIZE))

struct uxa_glyph {
	uxa_glyph_cache_t *cache;
	uint16_t x, y;
	uint16_t size, pos;
};

#if HAS_DEVPRIVATEKEYREC
static DevPrivateKeyRec uxa_glyph_key;
#else
static int uxa_glyph_key;
#endif

static inline struct uxa_glyph *uxa_glyph_get_private(GlyphPtr glyph)
{
#if HAS_DEVPRIVATEKEYREC
	return dixGetPrivate(&glyph->devPrivates, &uxa_glyph_key);
#else
	return dixLookupPrivate(&glyph->devPrivates, &uxa_glyph_key);
#endif
}

static inline void uxa_glyph_set_private(GlyphPtr glyph, struct uxa_glyph *priv)
{
	dixSetPrivate(&glyph->devPrivates, &uxa_glyph_key, priv);
}

#define NeedsComponent(f) (PICT_FORMAT_A(f) != 0 && PICT_FORMAT_RGB(f) != 0)

static void uxa_unrealize_glyph_caches(ScreenPtr pScreen)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pScreen);
	int i;

	if (!uxa_screen->glyph_cache_initialized)
		return;

	for (i = 0; i < UXA_NUM_GLYPH_CACHE_FORMATS; i++) {
		uxa_glyph_cache_t *cache = &uxa_screen->glyphCaches[i];

		if (cache->picture)
			FreePicture(cache->picture, 0);

		if (cache->glyphs)
			free(cache->glyphs);
	}
	uxa_screen->glyph_cache_initialized = FALSE;
}

void uxa_glyphs_fini(ScreenPtr pScreen)
{
	uxa_unrealize_glyph_caches(pScreen);
}

/* All caches for a single format share a single pixmap for glyph storage,
 * allowing mixing glyphs of different sizes without paying a penalty
 * for switching between source pixmaps. (Note that for a size of font
 * right at the border between two sizes, we might be switching for almost
 * every glyph.)
 *
 * This function allocates the storage pixmap, and then fills in the
 * rest of the allocated structures for all caches with the given format.
 */
static Bool uxa_realize_glyph_caches(ScreenPtr pScreen)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pScreen);
	unsigned int formats[] = {
		PIXMAN_a8,
		PIXMAN_a8r8g8b8,
	};
	int i;

	if (uxa_screen->glyph_cache_initialized)
		return TRUE;

	uxa_screen->glyph_cache_initialized = TRUE;
	memset(uxa_screen->glyphCaches, 0, sizeof(uxa_screen->glyphCaches));

	for (i = 0; i < sizeof(formats)/sizeof(formats[0]); i++) {
		uxa_glyph_cache_t *cache = &uxa_screen->glyphCaches[i];
		PixmapPtr pixmap;
		PicturePtr picture;
		CARD32 component_alpha;
		int depth = PIXMAN_FORMAT_DEPTH(formats[i]);
		int error;
		PictFormatPtr pPictFormat = PictureMatchFormat(pScreen, depth, formats[i]);
		if (!pPictFormat)
			goto bail;

		/* Now allocate the pixmap and picture */
		pixmap = pScreen->CreatePixmap(pScreen,
					       CACHE_PICTURE_SIZE, CACHE_PICTURE_SIZE, depth,
					       INTEL_CREATE_PIXMAP_TILING_X);
		if (!pixmap)
			goto bail;
		if (!uxa_pixmap_is_offscreen(pixmap)) {
			/* Presume shadow is in-effect */
			pScreen->DestroyPixmap(pixmap);
			uxa_unrealize_glyph_caches(pScreen);
			return TRUE;
		}

		component_alpha = NeedsComponent(pPictFormat->format);
		picture = CreatePicture(0, &pixmap->drawable, pPictFormat,
					CPComponentAlpha, &component_alpha,
					serverClient, &error);

		pScreen->DestroyPixmap(pixmap);

		if (!picture)
			goto bail;

		ValidatePicture(picture);

		cache->picture = picture;
		cache->glyphs = calloc(sizeof(GlyphPtr), GLYPH_CACHE_SIZE);
		if (!cache->glyphs)
			goto bail;

		cache->evict = rand() % GLYPH_CACHE_SIZE;
	}
	assert(i == UXA_NUM_GLYPH_CACHE_FORMATS);

	return TRUE;

bail:
	uxa_unrealize_glyph_caches(pScreen);
	return FALSE;
}


Bool uxa_glyphs_init(ScreenPtr pScreen)
{

	uxa_screen_t *uxa_screen = uxa_get_screen(pScreen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR)
		return TRUE;
#if HAS_DIXREGISTERPRIVATEKEY
	if (!dixRegisterPrivateKey(&uxa_glyph_key, PRIVATE_GLYPH, 0))
		return FALSE;
#else
	if (!dixRequestPrivate(&uxa_glyph_key, 0))
		return FALSE;
#endif

	/* Skip pixmap creation if we don't intend to use it. */
	if (uxa_get_screen(pScreen)->force_fallback)
		return TRUE;

	return uxa_realize_glyph_caches(pScreen);
}

/* The most efficient thing to way to upload the glyph to the screen
 * is to use CopyArea; uxa pixmaps are always offscreen.
 */
static void
uxa_glyph_cache_upload_glyph(ScreenPtr screen,
			     uxa_glyph_cache_t * cache,
			     GlyphPtr glyph,
			     int x, int y)
{
	PicturePtr pGlyphPicture = GetGlyphPicture(glyph, screen);
	PixmapPtr pGlyphPixmap = (PixmapPtr) pGlyphPicture->pDrawable;
	PixmapPtr pCachePixmap = (PixmapPtr) cache->picture->pDrawable;
	PixmapPtr scratch;
	GCPtr gc;

	gc = GetScratchGC(pCachePixmap->drawable.depth, screen);
	if (!gc)
		return;

	ValidateGC(&pCachePixmap->drawable, gc);

	scratch = pGlyphPixmap;
	/* Create a temporary bo to stream the updates to the cache */
	if (pGlyphPixmap->drawable.depth != pCachePixmap->drawable.depth ||
	    !uxa_pixmap_is_offscreen(scratch)) {
		scratch = screen->CreatePixmap(screen,
					       glyph->info.width,
					       glyph->info.height,
					       pCachePixmap->drawable.depth,
					       UXA_CREATE_PIXMAP_FOR_MAP);
		if (scratch) {
			if (pGlyphPixmap->drawable.depth != pCachePixmap->drawable.depth) {
				PicturePtr picture;
				int error;

				picture = CreatePicture(0, &scratch->drawable,
							PictureMatchFormat(screen,
									   pCachePixmap->drawable.depth,
									   cache->picture->format),
							0, NULL,
							serverClient, &error);
				if (picture) {
					ValidatePicture(picture);
					uxa_composite(PictOpSrc, pGlyphPicture, NULL, picture,
						      0, 0,
						      0, 0,
						      0, 0,
						      glyph->info.width, glyph->info.height);
					FreePicture(picture, 0);
				}
			} else {
				uxa_copy_area(&pGlyphPixmap->drawable,
					      &scratch->drawable,
					      gc,
					      0, 0,
					      glyph->info.width, glyph->info.height,
					      0, 0);
			}
		} else {
			scratch = pGlyphPixmap;
		}
	}

	uxa_copy_area(&scratch->drawable, &pCachePixmap->drawable, gc,
		      0, 0,
		      glyph->info.width, glyph->info.height,
		      x, y);

	if (scratch != pGlyphPixmap)
		screen->DestroyPixmap(scratch);

	FreeScratchGC(gc);
}

void
uxa_glyph_unrealize(ScreenPtr screen,
		    GlyphPtr glyph)
{
	struct uxa_glyph *priv;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		glamor_glyph_unrealize(screen, glyph);
		return;
	}

	/* Use Lookup in case we have not attached to this glyph. */
	priv = dixLookupPrivate(&glyph->devPrivates, &uxa_glyph_key);
	if (priv == NULL)
		return;

	priv->cache->glyphs[priv->pos] = NULL;

	uxa_glyph_set_private(glyph, NULL);
	free(priv);
}

/* Cut and paste from render/glyph.c - probably should export it instead */
static void
uxa_glyph_extents(int nlist,
		  GlyphListPtr list, GlyphPtr * glyphs, BoxPtr extents)
{
	int x1, x2, y1, y2;
	int x, y, n;

	x1 = y1 = MAXSHORT;
	x2 = y2 = MINSHORT;
	x = y = 0;
	while (nlist--) {
		x += list->xOff;
		y += list->yOff;
		n = list->len;
		list++;
		while (n--) {
			GlyphPtr glyph = *glyphs++;
			int v;

			v = x - glyph->info.x;
			if (v < x1)
			    x1 = v;
			v += glyph->info.width;
			if (v > x2)
			    x2 = v;

			v = y - glyph->info.y;
			if (v < y1)
			    y1 = v;
			v += glyph->info.height;
			if (v > y2)
			    y2 = v;

			x += glyph->info.xOff;
			y += glyph->info.yOff;
		}
	}

	extents->x1 = x1 < MINSHORT ? MINSHORT : x1;
	extents->x2 = x2 > MAXSHORT ? MAXSHORT : x2;
	extents->y1 = y1 < MINSHORT ? MINSHORT : y1;
	extents->y2 = y2 > MAXSHORT ? MAXSHORT : y2;
}

/**
 * Returns TRUE if the glyphs in the lists intersect.  Only checks based on
 * bounding box, which appears to be good enough to catch most cases at least.
 */
static Bool
uxa_glyphs_intersect(int nlist, GlyphListPtr list, GlyphPtr * glyphs)
{
	int x1, x2, y1, y2;
	int n;
	int x, y;
	BoxRec extents;
	Bool first = TRUE;

	x = 0;
	y = 0;
	extents.x1 = 0;
	extents.y1 = 0;
	extents.x2 = 0;
	extents.y2 = 0;
	while (nlist--) {
		x += list->xOff;
		y += list->yOff;
		n = list->len;
		list++;
		while (n--) {
			GlyphPtr glyph = *glyphs++;

			if (glyph->info.width == 0 || glyph->info.height == 0) {
				x += glyph->info.xOff;
				y += glyph->info.yOff;
				continue;
			}

			x1 = x - glyph->info.x;
			if (x1 < MINSHORT)
				x1 = MINSHORT;
			y1 = y - glyph->info.y;
			if (y1 < MINSHORT)
				y1 = MINSHORT;
			x2 = x1 + glyph->info.width;
			if (x2 > MAXSHORT)
				x2 = MAXSHORT;
			y2 = y1 + glyph->info.height;
			if (y2 > MAXSHORT)
				y2 = MAXSHORT;

			if (first) {
				extents.x1 = x1;
				extents.y1 = y1;
				extents.x2 = x2;
				extents.y2 = y2;
				first = FALSE;
			} else {
				if (x1 < extents.x2 && x2 > extents.x1 &&
				    y1 < extents.y2 && y2 > extents.y1) {
					return TRUE;
				}

				if (x1 < extents.x1)
					extents.x1 = x1;
				if (x2 > extents.x2)
					extents.x2 = x2;
				if (y1 < extents.y1)
					extents.y1 = y1;
				if (y2 > extents.y2)
					extents.y2 = y2;
			}
			x += glyph->info.xOff;
			y += glyph->info.yOff;
		}
	}

	return FALSE;
}

static void
uxa_check_glyphs(CARD8 op,
		 PicturePtr src,
		 PicturePtr dst,
		 PictFormatPtr maskFormat,
		 INT16 xSrc,
		 INT16 ySrc, int nlist, GlyphListPtr list, GlyphPtr * glyphs)
{
	pixman_image_t *image;
	PixmapPtr scratch;
	PicturePtr mask, mask_src = NULL, mask_dst = NULL, white = NULL;
	int width = 0, height = 0;
	int x, y, n;
	int xDst = list->xOff, yDst = list->yOff;
	BoxRec extents = { 0, 0, 0, 0 };
	CARD8 mask_op = 0;

	if (maskFormat) {
		pixman_format_code_t format;
		CARD32 component_alpha;
		xRenderColor color;
		int error;

		uxa_glyph_extents(nlist, list, glyphs, &extents);
		if (extents.x2 <= extents.x1 || extents.y2 <= extents.y1)
			return;

		width = extents.x2 - extents.x1;
		height = extents.y2 - extents.y1;

		format = maskFormat->format |
			(BitsPerPixel(maskFormat->depth) << 24);
		image =
			pixman_image_create_bits(format, width, height, NULL, 0);
		if (!image)
			return;

		scratch = GetScratchPixmapHeader(dst->pDrawable->pScreen, width, height,
						 PIXMAN_FORMAT_DEPTH(format),
						 PIXMAN_FORMAT_BPP(format),
						 pixman_image_get_stride(image),
						 pixman_image_get_data(image));

		if (!scratch) {
			pixman_image_unref(image);
			return;
		}

		component_alpha = NeedsComponent(maskFormat->format);
		mask = CreatePicture(0, &scratch->drawable,
				     maskFormat, CPComponentAlpha,
				     &component_alpha, serverClient, &error);
		if (!mask) {
			FreeScratchPixmapHeader(scratch);
			pixman_image_unref(image);
			return;
		}
		ValidatePicture(mask);

		x = -extents.x1;
		y = -extents.y1;

		color.red = color.green = color.blue = color.alpha = 0xffff;
		white = CreateSolidPicture(0, &color, &error);

		mask_op = op;
		op = PictOpAdd;

		mask_src = src;
		src = white;

		mask_dst = dst;
		dst = mask;
	} else {
		mask = dst;
		x = 0;
		y = 0;
	}

	while (nlist--) {
		x += list->xOff;
		y += list->yOff;
		n = list->len;
		while (n--) {
			GlyphPtr glyph = *glyphs++;
			PicturePtr g = GetGlyphPicture(glyph, dst->pDrawable->pScreen);
			if (g) {
				CompositePicture(op, src, g, dst,
						 xSrc + (x - glyph->info.x) - xDst,
						 ySrc + (y - glyph->info.y) - yDst,
						 0, 0,
						 x - glyph->info.x,
						 y - glyph->info.y,
						 glyph->info.width,
						 glyph->info.height);
			}

			x += glyph->info.xOff;
			y += glyph->info.yOff;
		}
		list++;
	}

	if (white)
		FreePicture(white, 0);

	if (maskFormat) {
		x = extents.x1;
		y = extents.y1;
		CompositePicture(mask_op, mask_src, mask, mask_dst,
				 xSrc + x - xDst,
				 ySrc + y - yDst,
				 0, 0,
				 x, y,
				 width, height);
		FreePicture(mask, 0);
		FreeScratchPixmapHeader(scratch);
		pixman_image_unref(image);
	}
}

static inline unsigned int
uxa_glyph_size_to_count(int size)
{
	size /= GLYPH_MIN_SIZE;
	return size * size;
}

static inline unsigned int
uxa_glyph_count_to_mask(int count)
{
	return ~(count - 1);
}

static inline unsigned int
uxa_glyph_size_to_mask(int size)
{
	return uxa_glyph_count_to_mask(uxa_glyph_size_to_count(size));
}

static PicturePtr
uxa_glyph_cache(ScreenPtr screen, GlyphPtr glyph, int *out_x, int *out_y)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);
	PicturePtr glyph_picture = GetGlyphPicture(glyph, screen);
	uxa_glyph_cache_t *cache = &uxa_screen->glyphCaches[PICT_FORMAT_RGB(glyph_picture->format) != 0];
	struct uxa_glyph *priv = NULL;
	int size, mask, pos, s;

	if (glyph->info.width > GLYPH_MAX_SIZE || glyph->info.height > GLYPH_MAX_SIZE)
		return NULL;

	for (size = GLYPH_MIN_SIZE; size <= GLYPH_MAX_SIZE; size *= 2)
		if (glyph->info.width <= size && glyph->info.height <= size)
			break;

	s = uxa_glyph_size_to_count(size);
	mask = uxa_glyph_count_to_mask(s);
	pos = (cache->count + s - 1) & mask;
	if (pos < GLYPH_CACHE_SIZE) {
		cache->count = pos + s;
	} else {
		for (s = size; s <= GLYPH_MAX_SIZE; s *= 2) {
			int i = cache->evict & uxa_glyph_size_to_mask(s);
			GlyphPtr evicted = cache->glyphs[i];
			if (evicted == NULL)
				continue;

			priv = uxa_glyph_get_private(evicted);
			if (priv->size >= s) {
				cache->glyphs[i] = NULL;
				uxa_glyph_set_private(evicted, NULL);
				pos = cache->evict & uxa_glyph_size_to_mask(size);
			} else
				priv = NULL;
			break;
		}
		if (priv == NULL) {
			int count = uxa_glyph_size_to_count(size);
			mask = uxa_glyph_count_to_mask(count);
			pos = cache->evict & mask;
			for (s = 0; s < count; s++) {
				GlyphPtr evicted = cache->glyphs[pos + s];
				if (evicted != NULL) {
					if (priv != NULL)
						free(priv);

					priv = uxa_glyph_get_private(evicted);
					uxa_glyph_set_private(evicted, NULL);
					cache->glyphs[pos + s] = NULL;
				}
			}
		}

		/* And pick a new eviction position */
		cache->evict = rand() % GLYPH_CACHE_SIZE;
	}

	if (priv == NULL) {
		priv = malloc(sizeof(struct uxa_glyph));
		if (priv == NULL)
			return NULL;
	}

	uxa_glyph_set_private(glyph, priv);
	cache->glyphs[pos] = glyph;

	priv->cache = cache;
	priv->size = size;
	priv->pos = pos;
	s = pos / ((GLYPH_MAX_SIZE / GLYPH_MIN_SIZE) * (GLYPH_MAX_SIZE / GLYPH_MIN_SIZE));
	priv->x = s % (CACHE_PICTURE_SIZE / GLYPH_MAX_SIZE) * GLYPH_MAX_SIZE;
	priv->y = (s / (CACHE_PICTURE_SIZE / GLYPH_MAX_SIZE)) * GLYPH_MAX_SIZE;
	for (s = GLYPH_MIN_SIZE; s < GLYPH_MAX_SIZE; s *= 2) {
		if (pos & 1)
			priv->x += s;
		if (pos & 2)
			priv->y += s;
		pos >>= 2;
	}

	uxa_glyph_cache_upload_glyph(screen, cache, glyph, priv->x, priv->y);

	*out_x = priv->x;
	*out_y = priv->y;
	return cache->picture;
}

static void
uxa_clear_pixmap(ScreenPtr screen,
		 uxa_screen_t *uxa_screen,
		 PixmapPtr pixmap)
{
	if (uxa_screen->info->check_solid &&
	    !uxa_screen->info->check_solid(&pixmap->drawable, GXcopy, FB_ALLONES))
		goto fallback;

	if (!uxa_screen->info->prepare_solid(pixmap, GXcopy, FB_ALLONES, 0))
		goto fallback;

	uxa_screen->info->solid(pixmap,
				0, 0,
				pixmap->drawable.width,
				pixmap->drawable.height);

	uxa_screen->info->done_solid(pixmap);
	return;

fallback:
	{
		GCPtr gc;

		gc = GetScratchGC(pixmap->drawable.depth, screen);
		if (gc) {
			xRectangle rect;

			ValidateGC(&pixmap->drawable, gc);

			rect.x = 0;
			rect.y = 0;
			rect.width  = pixmap->drawable.width;
			rect.height = pixmap->drawable.height;
			gc->ops->PolyFillRect(&pixmap->drawable, gc, 1, &rect);

			FreeScratchGC(gc);
		}
	}
}

static PicturePtr
create_white_solid(ScreenPtr screen)
{
	PicturePtr white, ret = NULL;
	xRenderColor color;
	int error;

	color.red = color.green = color.blue = color.alpha = 0xffff;
	white = CreateSolidPicture(0, &color, &error);
	if (white) {
		ret = uxa_acquire_solid(screen, white->pSourcePict);
		FreePicture(white, 0);
	}

	return ret;
}

static int
uxa_glyphs_via_mask(CARD8 op,
		    PicturePtr pSrc,
		    PicturePtr pDst,
		    PictFormatPtr maskFormat,
		    INT16 xSrc, INT16 ySrc,
		    int nlist, GlyphListPtr list, GlyphPtr * glyphs)
{
	ScreenPtr screen = pDst->pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);
	CARD32 component_alpha;
	PixmapPtr pixmap, white_pixmap;
	PicturePtr glyph_atlas, mask, white;
	int xDst = list->xOff, yDst = list->yOff;
	int x, y, width, height;
	int dst_off_x, dst_off_y;
	int n, error;
	BoxRec box;

	uxa_glyph_extents(nlist, list, glyphs, &box);
	if (box.x2 <= box.x1 || box.y2 <= box.y1)
		return 0;

	dst_off_x = box.x1;
	dst_off_y = box.y1;

	width  = box.x2 - box.x1;
	height = box.y2 - box.y1;
	x = -box.x1;
	y = -box.y1;

	if (maskFormat->depth == 1) {
		PictFormatPtr a8Format =
			PictureMatchFormat(screen, 8, PICT_a8);

		if (!a8Format)
			return -1;

		maskFormat = a8Format;
	}

	pixmap = screen->CreatePixmap(screen, width, height,
				      maskFormat->depth,
				      CREATE_PIXMAP_USAGE_SCRATCH);
	if (!pixmap)
		return 1;

	if (!uxa_pixmap_is_offscreen(pixmap)) {
		screen->DestroyPixmap(pixmap);
		return -1;
	}

	white_pixmap = NULL;
	white = create_white_solid(screen);
	if (white)
		white_pixmap = uxa_get_drawable_pixmap(white->pDrawable);
	if (!white_pixmap) {
		if (white)
			FreePicture(white, 0);
		screen->DestroyPixmap(pixmap);
		return -1;
	}

	uxa_clear_pixmap(screen, uxa_screen, pixmap);

	component_alpha = NeedsComponent(maskFormat->format);
	mask = CreatePicture(0, &pixmap->drawable,
			      maskFormat, CPComponentAlpha,
			      &component_alpha, serverClient, &error);
	screen->DestroyPixmap(pixmap);

	if (!mask) {
		FreePicture(white, 0);
		return 1;
	}

	ValidatePicture(mask);

	glyph_atlas = NULL;
	while (nlist--) {
		x += list->xOff;
		y += list->yOff;
		n = list->len;
		while (n--) {
			GlyphPtr glyph = *glyphs++;
			PicturePtr this_atlas;
			int glyph_x, glyph_y;
			struct uxa_glyph *priv;

			if (glyph->info.width == 0 || glyph->info.height == 0)
				goto next_glyph;

			priv = uxa_glyph_get_private(glyph);
			if (priv != NULL) {
				glyph_x = priv->x;
				glyph_y = priv->y;
				this_atlas = priv->cache->picture;
			} else {
				if (glyph_atlas) {
					uxa_screen->info->done_composite(pixmap);
					glyph_atlas = NULL;
				}
				this_atlas = uxa_glyph_cache(screen, glyph, &glyph_x, &glyph_y);
				if (this_atlas == NULL) {
					/* no cache for this glyph */
					this_atlas = GetGlyphPicture(glyph, screen);
					glyph_x = glyph_y = 0;
				}
			}

			if (this_atlas != glyph_atlas) {
				PixmapPtr glyph_pixmap;

				if (glyph_atlas)
					uxa_screen->info->done_composite(pixmap);

				glyph_pixmap =
					uxa_get_drawable_pixmap(this_atlas->pDrawable);
				if (!uxa_pixmap_is_offscreen(glyph_pixmap) ||
				    !uxa_screen->info->prepare_composite(PictOpAdd,
									 white, this_atlas, mask,
									 white_pixmap, glyph_pixmap, pixmap)) {
					FreePicture(white, 0);
					FreePicture(mask, 0);
					return -1;
				}

				glyph_atlas = this_atlas;
			}

			uxa_screen->info->composite(pixmap,
						    0, 0,
						    glyph_x, glyph_y,
						    x - glyph->info.x,
						    y - glyph->info.y,
						    glyph->info.width,
						    glyph->info.height);

next_glyph:
			x += glyph->info.xOff;
			y += glyph->info.yOff;
		}
		list++;
	}
	if (glyph_atlas)
		uxa_screen->info->done_composite(pixmap);

	uxa_composite(op,
		      pSrc, mask, pDst,
		      dst_off_x + xSrc - xDst,
		      dst_off_y + ySrc - yDst,
		      0, 0,
		      dst_off_x, dst_off_y,
		      width, height);

	FreePicture(white, 0);
	FreePicture(mask, 0);
	return 0;
}

static int
uxa_glyphs_to_dst(CARD8 op,
		  PicturePtr pSrc,
		  PicturePtr pDst,
		  INT16 xSrc, INT16 ySrc,
		  int nlist, GlyphListPtr list, GlyphPtr * glyphs)
{
	ScreenPtr screen = pDst->pDrawable->pScreen;
	int x, y, n;

	xSrc -= list->xOff;
	ySrc -= list->yOff;
	x = y = 0;
	while (nlist--) {
		x += list->xOff;
		y += list->yOff;
		n = list->len;
		while (n--) {
			GlyphPtr glyph = *glyphs++;
			PicturePtr glyph_atlas;
			int glyph_x, glyph_y;
			struct uxa_glyph *priv;

			if (glyph->info.width == 0 || glyph->info.height == 0)
				goto next_glyph;

			priv = uxa_glyph_get_private(glyph);
			if (priv != NULL) {
				glyph_x = priv->x;
				glyph_y = priv->y;
				glyph_atlas = priv->cache->picture;
			} else {
				glyph_atlas = uxa_glyph_cache(screen, glyph, &glyph_x, &glyph_y);
				if (glyph_atlas == NULL) {
					/* no cache for this glyph */
					glyph_atlas = GetGlyphPicture(glyph, screen);
					glyph_x = glyph_y = 0;
				}
			}

			uxa_composite(op,
				      pSrc, glyph_atlas, pDst,
				      xSrc + x - glyph->info.x,
				      ySrc + y - glyph->info.y,
				      glyph_x, glyph_y,
				      x - glyph->info.x,
				      y - glyph->info.y,
				      glyph->info.width, glyph->info.height);

next_glyph:
			x += glyph->info.xOff;
			y += glyph->info.yOff;
		}
		list++;
	}

	return 0;
}

static Bool
is_solid(PicturePtr picture)
{
	if (picture->pSourcePict) {
		SourcePict *source = picture->pSourcePict;
		return source->type == SourcePictTypeSolidFill;
	} else {
		return (picture->repeat &&
			picture->pDrawable->width  == 1 &&
			picture->pDrawable->height == 1);
	}
}

void
uxa_glyphs(CARD8 op,
	   PicturePtr pSrc,
	   PicturePtr pDst,
	   PictFormatPtr maskFormat,
	   INT16 xSrc, INT16 ySrc,
	   int nlist, GlyphListPtr list, GlyphPtr * glyphs)
{
	ScreenPtr screen = pDst->pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok;

		uxa_picture_prepare_access(pDst, UXA_GLAMOR_ACCESS_RW);
		uxa_picture_prepare_access(pSrc, UXA_GLAMOR_ACCESS_RO);
		ok = glamor_glyphs_nf(op,
				     pSrc, pDst, maskFormat,
				     xSrc, ySrc, nlist, list, glyphs);
		uxa_picture_finish_access(pSrc, UXA_GLAMOR_ACCESS_RO);
		uxa_picture_finish_access(pDst, UXA_GLAMOR_ACCESS_RW);

		if (!ok)
			goto fallback;

		return;
	}

	if (!uxa_screen->info->prepare_composite ||
	    uxa_screen->force_fallback ||
	    !uxa_drawable_is_offscreen(pDst->pDrawable) ||
	    pDst->alphaMap || pSrc->alphaMap ||
	    /* XXX we fail to handle (rare) non-solid sources correctly. */
	    !is_solid(pSrc)) {
fallback:
	    uxa_check_glyphs(op, pSrc, pDst, maskFormat, xSrc, ySrc, nlist, list, glyphs);
	    return;
	}

	/* basic sanity check */
	if (uxa_screen->info->check_composite &&
	    !uxa_screen->info->check_composite(op, pSrc, NULL, pDst, 0, 0)) {
		goto fallback;
	}

	ValidatePicture(pSrc);
	ValidatePicture(pDst);

	if (!maskFormat) {
		/* If we don't have a mask format but all the glyphs have the same format,
		 * require ComponentAlpha and don't intersect, use the glyph format as mask
		 * format for the full benefits of the glyph cache.
		 */
		if (NeedsComponent(list[0].format->format)) {
			Bool sameFormat = TRUE;
			int i;

			maskFormat = list[0].format;

			for (i = 0; i < nlist; i++) {
				if (maskFormat->format != list[i].format->format) {
					sameFormat = FALSE;
					break;
				}
			}

			if (!sameFormat ||
			    uxa_glyphs_intersect(nlist, list, glyphs))
				maskFormat = NULL;
		}
	}

	if (!maskFormat) {
		if (uxa_glyphs_to_dst(op, pSrc, pDst,
				      xSrc, ySrc,
				      nlist, list, glyphs))
			goto fallback;
	} else {
		if (uxa_glyphs_via_mask(op,
					pSrc, pDst, maskFormat,
					xSrc, ySrc,
					nlist, list, glyphs))
			goto fallback;
	}
}
