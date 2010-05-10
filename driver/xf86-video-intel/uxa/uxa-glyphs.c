/*
 * Copyright © 2008 Red Hat, Inc.
 * Partly based on code Copyright © 2000 SuSE, Inc.
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
 * Author: Owen Taylor <otaylor@fishsoup.net>
 * Based on code by: Keith Packard
 */

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <stdlib.h>

#include "uxa-priv.h"
#include "../src/common.h"

#include "mipict.h"

#if DEBUG_GLYPH_CACHE
#define DBG_GLYPH_CACHE(a) ErrorF a
#else
#define DBG_GLYPH_CACHE(a)
#endif

/* Width of the pixmaps we use for the caches; this should be less than
 * max texture size of the driver; this may need to actually come from
 * the driver.
 */
#define CACHE_PICTURE_WIDTH 1024

/* Maximum number of glyphs we buffer on the stack before flushing
 * rendering to the mask or destination surface.
 */
#define GLYPH_BUFFER_SIZE 256

typedef struct {
	PicturePtr source;
	uxa_composite_rect_t rects[GLYPH_BUFFER_SIZE];
	int count;
} uxa_glyph_buffer_t;

typedef enum {
	UXA_GLYPH_SUCCESS,	/* Glyph added to render buffer */
	UXA_GLYPH_FAIL,		/* out of memory, etc */
	UXA_GLYPH_NEED_FLUSH,	/* would evict a glyph already in the buffer */
} uxa_glyph_cache_result_t;

void uxa_glyphs_init(ScreenPtr pScreen)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pScreen);
	int i = 0;

	memset(uxa_screen->glyphCaches, 0, sizeof(uxa_screen->glyphCaches));

	uxa_screen->glyphCaches[i].format = PICT_a8;
	uxa_screen->glyphCaches[i].glyphWidth =
	    uxa_screen->glyphCaches[i].glyphHeight = 16;
	i++;
	uxa_screen->glyphCaches[i].format = PICT_a8;
	uxa_screen->glyphCaches[i].glyphWidth =
	    uxa_screen->glyphCaches[i].glyphHeight = 32;
	i++;
	uxa_screen->glyphCaches[i].format = PICT_a8r8g8b8;
	uxa_screen->glyphCaches[i].glyphWidth =
	    uxa_screen->glyphCaches[i].glyphHeight = 16;
	i++;
	uxa_screen->glyphCaches[i].format = PICT_a8r8g8b8;
	uxa_screen->glyphCaches[i].glyphWidth =
	    uxa_screen->glyphCaches[i].glyphHeight = 32;
	i++;

	assert(i == UXA_NUM_GLYPH_CACHES);

	for (i = 0; i < UXA_NUM_GLYPH_CACHES; i++) {
		uxa_screen->glyphCaches[i].columns =
		    CACHE_PICTURE_WIDTH / uxa_screen->glyphCaches[i].glyphWidth;
		uxa_screen->glyphCaches[i].size = 256;
		uxa_screen->glyphCaches[i].hashSize = 557;
	}
}

static void uxa_unrealize_glyph_caches(ScreenPtr pScreen, unsigned int format)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pScreen);
	int i;

	for (i = 0; i < UXA_NUM_GLYPH_CACHES; i++) {
		uxa_glyph_cache_t *cache = &uxa_screen->glyphCaches[i];

		if (cache->format != format)
			continue;

		if (cache->picture) {
			FreePicture((pointer) cache->picture, (XID) 0);
			cache->picture = NULL;
		}

		if (cache->hashEntries) {
			xfree(cache->hashEntries);
			cache->hashEntries = NULL;
		}

		if (cache->glyphs) {
			xfree(cache->glyphs);
			cache->glyphs = NULL;
		}
		cache->glyphCount = 0;
	}
}

#define NeedsComponent(f) (PICT_FORMAT_A(f) != 0 && PICT_FORMAT_RGB(f) != 0)

/* All caches for a single format share a single pixmap for glyph storage,
 * allowing mixing glyphs of different sizes without paying a penalty
 * for switching between source pixmaps. (Note that for a size of font
 * right at the border between two sizes, we might be switching for almost
 * every glyph.)
 *
 * This function allocates the storage pixmap, and then fills in the
 * rest of the allocated structures for all caches with the given format.
 */
static Bool uxa_realize_glyph_caches(ScreenPtr pScreen, unsigned int format)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pScreen);
	int depth = PIXMAN_FORMAT_DEPTH(format);
	PictFormatPtr pPictFormat;
	PixmapPtr pPixmap;
	PicturePtr pPicture;
	CARD32 component_alpha;
	int height;
	int i;
	int error;

	pPictFormat = PictureMatchFormat(pScreen, depth, format);
	if (!pPictFormat)
		return FALSE;

	/* Compute the total vertical size needed for the format */

	height = 0;
	for (i = 0; i < UXA_NUM_GLYPH_CACHES; i++) {
		uxa_glyph_cache_t *cache = &uxa_screen->glyphCaches[i];
		int rows;

		if (cache->format != format)
			continue;

		cache->yOffset = height;

		rows = (cache->size + cache->columns - 1) / cache->columns;
		height += rows * cache->glyphHeight;
	}

	/* Now allocate the pixmap and picture */

	pPixmap = (*pScreen->CreatePixmap) (pScreen,
					    CACHE_PICTURE_WIDTH,
					    height, depth,
					    INTEL_CREATE_PIXMAP_TILING_X);
	if (!pPixmap)
		return FALSE;

	component_alpha = NeedsComponent(pPictFormat->format);
	pPicture = CreatePicture(0, &pPixmap->drawable, pPictFormat,
				 CPComponentAlpha, &component_alpha,
				 serverClient, &error);

	(*pScreen->DestroyPixmap) (pPixmap);	/* picture holds a refcount */

	if (!pPicture)
		return FALSE;

	/* And store the picture in all the caches for the format */

	for (i = 0; i < UXA_NUM_GLYPH_CACHES; i++) {
		uxa_glyph_cache_t *cache = &uxa_screen->glyphCaches[i];
		int j;

		if (cache->format != format)
			continue;

		cache->picture = pPicture;
		cache->picture->refcnt++;
		cache->hashEntries = xalloc(sizeof(int) * cache->hashSize);
		cache->glyphs =
		    xalloc(sizeof(uxa_cached_glyph_t) * cache->size);
		cache->glyphCount = 0;

		if (!cache->hashEntries || !cache->glyphs)
			goto bail;

		for (j = 0; j < cache->hashSize; j++)
			cache->hashEntries[j] = -1;

		cache->evictionPosition = rand() % cache->size;
	}

	/* Each cache references the picture individually */
	FreePicture((pointer) pPicture, (XID) 0);
	return TRUE;

bail:
	uxa_unrealize_glyph_caches(pScreen, format);
	return FALSE;
}

void uxa_glyphs_fini(ScreenPtr pScreen)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pScreen);
	int i;

	for (i = 0; i < UXA_NUM_GLYPH_CACHES; i++) {
		uxa_glyph_cache_t *cache = &uxa_screen->glyphCaches[i];

		if (cache->picture)
			uxa_unrealize_glyph_caches(pScreen, cache->format);
	}
}

static int
uxa_glyph_cache_hash_lookup(uxa_glyph_cache_t * cache, GlyphPtr pGlyph)
{
	int slot;

	slot = (*(CARD32 *) pGlyph->sha1) % cache->hashSize;

	while (TRUE) {		/* hash table can never be full */
		int entryPos = cache->hashEntries[slot];
		if (entryPos == -1)
			return -1;

		if (memcmp
		    (pGlyph->sha1, cache->glyphs[entryPos].sha1,
		     sizeof(pGlyph->sha1)) == 0) {
			return entryPos;
		}

		slot--;
		if (slot < 0)
			slot = cache->hashSize - 1;
	}
}

static void
uxa_glyph_cache_hash_insert(uxa_glyph_cache_t * cache, GlyphPtr pGlyph, int pos)
{
	int slot;

	memcpy(cache->glyphs[pos].sha1, pGlyph->sha1, sizeof(pGlyph->sha1));

	slot = (*(CARD32 *) pGlyph->sha1) % cache->hashSize;

	while (TRUE) {		/* hash table can never be full */
		if (cache->hashEntries[slot] == -1) {
			cache->hashEntries[slot] = pos;
			return;
		}

		slot--;
		if (slot < 0)
			slot = cache->hashSize - 1;
	}
}

static void uxa_glyph_cache_hash_remove(uxa_glyph_cache_t * cache, int pos)
{
	int slot;
	int emptiedSlot = -1;

	slot = (*(CARD32 *) cache->glyphs[pos].sha1) % cache->hashSize;

	while (TRUE) {		/* hash table can never be full */
		int entryPos = cache->hashEntries[slot];

		if (entryPos == -1)
			return;

		if (entryPos == pos) {
			cache->hashEntries[slot] = -1;
			emptiedSlot = slot;
		} else if (emptiedSlot != -1) {
			/* See if we can move this entry into the emptied slot,
			 * we can't do that if if entry would have hashed
			 * between the current position and the emptied slot.
			 * (taking wrapping into account). Bad positions
			 * are:
			 *
			 * |   XXXXXXXXXX             |
			 *     i         j
			 *
			 * |XXX                   XXXX|
			 *     j                  i
			 *
			 * i - slot, j - emptiedSlot
			 *
			 * (Knuth 6.4R)
			 */

			int entrySlot =
			    (*(CARD32 *) cache->glyphs[entryPos].sha1) %
			    cache->hashSize;

			if (!((entrySlot >= slot && entrySlot < emptiedSlot) ||
			      (emptiedSlot < slot
			       && (entrySlot < emptiedSlot
				   || entrySlot >= slot)))) {
				cache->hashEntries[emptiedSlot] = entryPos;
				cache->hashEntries[slot] = -1;
				emptiedSlot = slot;
			}
		}

		slot--;
		if (slot < 0)
			slot = cache->hashSize - 1;
	}
}

#define CACHE_X(pos) (((pos) % cache->columns) * cache->glyphWidth)
#define CACHE_Y(pos) (cache->yOffset + ((pos) / cache->columns) * cache->glyphHeight)

/* The most efficient thing to way to upload the glyph to the screen
 * is to use CopyArea; uxa pixmaps are always offscreen.
 */
static Bool
uxa_glyph_cache_upload_glyph(ScreenPtr pScreen,
			     uxa_glyph_cache_t * cache,
			     int pos, GlyphPtr pGlyph)
{
	PicturePtr pGlyphPicture = GlyphPicture(pGlyph)[pScreen->myNum];
	PixmapPtr pGlyphPixmap = (PixmapPtr) pGlyphPicture->pDrawable;
	PixmapPtr pCachePixmap = (PixmapPtr) cache->picture->pDrawable;
	PixmapPtr scratch;
	GCPtr pGC;

	/* UploadToScreen only works if bpp match */
	if (pGlyphPixmap->drawable.bitsPerPixel !=
	    pCachePixmap->drawable.bitsPerPixel)
		return FALSE;

	pGC = GetScratchGC(pCachePixmap->drawable.depth, pScreen);
	ValidateGC(&pCachePixmap->drawable, pGC);

	/* Create a temporary bo to stream the updates to the cache */
	scratch = (*pScreen->CreatePixmap)(pScreen,
					   pGlyph->info.width,
					   pGlyph->info.height,
					   pGlyphPixmap->drawable.depth,
					   UXA_CREATE_PIXMAP_FOR_MAP);
	if (scratch) {
		(void)uxa_copy_area(&pGlyphPixmap->drawable,
				    &scratch->drawable,
				    pGC,
				    0, 0,
				    pGlyph->info.width, pGlyph->info.height,
				    0, 0);
	} else {
		scratch = pGlyphPixmap;
	}

	(void)uxa_copy_area(&scratch->drawable,
			    &pCachePixmap->drawable,
			    pGC,
			    0, 0, pGlyph->info.width, pGlyph->info.height,
			    CACHE_X(pos), CACHE_Y(pos));

	if (scratch != pGlyphPixmap)
		(*pScreen->DestroyPixmap)(scratch);

	FreeScratchGC(pGC);

	return TRUE;
}

static uxa_glyph_cache_result_t
uxa_glyph_cache_buffer_glyph(ScreenPtr pScreen,
			     uxa_glyph_cache_t * cache,
			     uxa_glyph_buffer_t * buffer,
			     GlyphPtr pGlyph, int xGlyph, int yGlyph)
{
	uxa_composite_rect_t *rect;
	int pos;

	if (buffer->source && buffer->source != cache->picture)
		return UXA_GLYPH_NEED_FLUSH;

	if (!cache->picture) {
		if (!uxa_realize_glyph_caches(pScreen, cache->format))
			return UXA_GLYPH_FAIL;
	}

	DBG_GLYPH_CACHE(("(%d,%d,%s): buffering glyph %lx\n",
			 cache->glyphWidth, cache->glyphHeight,
			 cache->format == PICT_a8 ? "A" : "ARGB",
			 (long)*(CARD32 *) pGlyph->sha1));

	pos = uxa_glyph_cache_hash_lookup(cache, pGlyph);
	if (pos != -1) {
		DBG_GLYPH_CACHE(("  found existing glyph at %d\n", pos));
	} else {
		if (cache->glyphCount < cache->size) {
			/* Space remaining; we fill from the start */
			pos = cache->glyphCount;
			cache->glyphCount++;
			DBG_GLYPH_CACHE(("  storing glyph in free space at %d\n", pos));

			uxa_glyph_cache_hash_insert(cache, pGlyph, pos);

		} else {
			/* Need to evict an entry. We have to see if any glyphs
			 * already in the output buffer were at this position in
			 * the cache
			 */

			pos = cache->evictionPosition;
			DBG_GLYPH_CACHE(("  evicting glyph at %d\n", pos));
			if (buffer->count) {
				int x, y;
				int i;

				x = CACHE_X(pos);
				y = CACHE_Y(pos);

				for (i = 0; i < buffer->count; i++) {
					if (buffer->rects[i].xSrc == x
					    && buffer->rects[i].ySrc == y) {
						DBG_GLYPH_CACHE(("  must flush buffer\n"));
						return UXA_GLYPH_NEED_FLUSH;
					}
				}
			}

			/* OK, we're all set, swap in the new glyph */
			uxa_glyph_cache_hash_remove(cache, pos);
			uxa_glyph_cache_hash_insert(cache, pGlyph, pos);

			/* And pick a new eviction position */
			cache->evictionPosition = rand() % cache->size;
		}

		/* Now actually upload the glyph into the cache picture; if
		 * we can't do it with UploadToScreen (because the glyph is
		 * offscreen, etc), we fall back to CompositePicture.
		 */
		if (!uxa_glyph_cache_upload_glyph(pScreen, cache, pos, pGlyph)) {
			CompositePicture(PictOpSrc,
					 GlyphPicture(pGlyph)[pScreen->myNum],
					 None,
					 cache->picture,
					 0, 0,
					 0, 0,
					 CACHE_X(pos),
					 CACHE_Y(pos),
					 pGlyph->info.width,
					 pGlyph->info.height);
		}

	}

	buffer->source = cache->picture;

	rect = &buffer->rects[buffer->count];
	rect->xSrc = CACHE_X(pos);
	rect->ySrc = CACHE_Y(pos);
	rect->xDst = xGlyph - pGlyph->info.x;
	rect->yDst = yGlyph - pGlyph->info.y;
	rect->width = pGlyph->info.width;
	rect->height = pGlyph->info.height;

	buffer->count++;

	return UXA_GLYPH_SUCCESS;
}

#undef CACHE_X
#undef CACHE_Y

static uxa_glyph_cache_result_t
uxa_buffer_glyph(ScreenPtr pScreen,
		 uxa_glyph_buffer_t * buffer,
		 GlyphPtr pGlyph, int xGlyph, int yGlyph)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pScreen);
	unsigned int format = (GlyphPicture(pGlyph)[pScreen->myNum])->format;
	int width = pGlyph->info.width;
	int height = pGlyph->info.height;
	uxa_composite_rect_t *rect;
	PicturePtr source;
	int i;

	if (buffer->count == GLYPH_BUFFER_SIZE)
		return UXA_GLYPH_NEED_FLUSH;

	if (PICT_FORMAT_BPP(format) == 1)
		format = PICT_a8;

	for (i = 0; i < UXA_NUM_GLYPH_CACHES; i++) {
		uxa_glyph_cache_t *cache = &uxa_screen->glyphCaches[i];

		if (format == cache->format &&
		    width <= cache->glyphWidth &&
		    height <= cache->glyphHeight) {
			uxa_glyph_cache_result_t result =
			    uxa_glyph_cache_buffer_glyph(pScreen,
							 &uxa_screen->
							 glyphCaches[i],
							 buffer,
							 pGlyph, xGlyph,
							 yGlyph);
			switch (result) {
			case UXA_GLYPH_FAIL:
				break;
			case UXA_GLYPH_SUCCESS:
			case UXA_GLYPH_NEED_FLUSH:
				return result;
			}
		}
	}

	/* Couldn't find the glyph in the cache, use the glyph picture directly */

	source = GlyphPicture(pGlyph)[pScreen->myNum];
	if (buffer->source && buffer->source != source)
		return UXA_GLYPH_NEED_FLUSH;

	buffer->source = source;

	rect = &buffer->rects[buffer->count];
	rect->xSrc = 0;
	rect->ySrc = 0;
	rect->xDst = xGlyph - pGlyph->info.x;
	rect->yDst = yGlyph - pGlyph->info.y;
	rect->width = pGlyph->info.width;
	rect->height = pGlyph->info.height;

	buffer->count++;

	return UXA_GLYPH_SUCCESS;
}

static void uxa_glyphs_to_mask(PicturePtr pMask, uxa_glyph_buffer_t * buffer)
{
	uxa_composite_rects(PictOpAdd, buffer->source, pMask,
			    buffer->count, buffer->rects);

	buffer->count = 0;
	buffer->source = NULL;
}

static void
uxa_glyphs_to_dst(CARD8 op,
		  PicturePtr pSrc,
		  PicturePtr pDst,
		  uxa_glyph_buffer_t * buffer,
		  INT16 xSrc, INT16 ySrc, INT16 xDst, INT16 yDst)
{
	int i;

	for (i = 0; i < buffer->count; i++) {
		uxa_composite_rect_t *rect = &buffer->rects[i];

		CompositePicture(op,
				 pSrc,
				 buffer->source,
				 pDst,
				 xSrc + rect->xDst - xDst,
				 ySrc + rect->yDst - yDst,
				 rect->xSrc,
				 rect->ySrc,
				 rect->xDst,
				 rect->yDst, rect->width, rect->height);
	}

	buffer->count = 0;
	buffer->source = NULL;
}

/* Cut and paste from render/glyph.c - probably should export it instead */
static void
uxa_glyph_extents(int nlist,
		  GlyphListPtr list, GlyphPtr * glyphs, BoxPtr extents)
{
	int x1, x2, y1, y2;
	int n;
	GlyphPtr glyph;
	int x, y;

	x = 0;
	y = 0;
	extents->x1 = MAXSHORT;
	extents->x2 = MINSHORT;
	extents->y1 = MAXSHORT;
	extents->y2 = MINSHORT;
	while (nlist--) {
		x += list->xOff;
		y += list->yOff;
		n = list->len;
		list++;
		while (n--) {
			glyph = *glyphs++;
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
			if (x1 < extents->x1)
				extents->x1 = x1;
			if (x2 > extents->x2)
				extents->x2 = x2;
			if (y1 < extents->y1)
				extents->y1 = y1;
			if (y2 > extents->y2)
				extents->y2 = y2;
			x += glyph->info.xOff;
			y += glyph->info.yOff;
		}
	}
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
	GlyphPtr glyph;
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
			glyph = *glyphs++;

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

void
uxa_glyphs(CARD8 op,
	   PicturePtr pSrc,
	   PicturePtr pDst,
	   PictFormatPtr maskFormat,
	   INT16 xSrc,
	   INT16 ySrc, int nlist, GlyphListPtr list, GlyphPtr * glyphs)
{
	PicturePtr pPicture;
	PixmapPtr pMaskPixmap = 0;
	PicturePtr pMask;
	ScreenPtr pScreen = pDst->pDrawable->pScreen;
	int width = 0, height = 0;
	int x, y;
	int xDst = list->xOff, yDst = list->yOff;
	int n;
	GlyphPtr glyph;
	int error;
	BoxRec extents = { 0, 0, 0, 0 };
	CARD32 component_alpha;
	uxa_glyph_buffer_t buffer;

	/* If we don't have a mask format but all the glyphs have the same format
	 * and don't intersect, use the glyph format as mask format for the full
	 * benefits of the glyph cache.
	 */
	if (!maskFormat) {
		Bool sameFormat = TRUE;
		int i;

		maskFormat = list[0].format;

		for (i = 0; i < nlist; i++) {
			if (maskFormat->format != list[i].format->format) {
				sameFormat = FALSE;
				break;
			}
		}

		if (!sameFormat || (maskFormat->depth != 1 &&
				    uxa_glyphs_intersect(nlist, list,
							 glyphs))) {
			maskFormat = NULL;
		}
	}

	if (maskFormat) {
		GCPtr pGC;
		xRectangle rect;

		uxa_glyph_extents(nlist, list, glyphs, &extents);

		if (extents.x2 <= extents.x1 || extents.y2 <= extents.y1)
			return;
		width = extents.x2 - extents.x1;
		height = extents.y2 - extents.y1;

		if (maskFormat->depth == 1) {
			PictFormatPtr a8Format =
			    PictureMatchFormat(pScreen, 8, PICT_a8);

			if (a8Format)
				maskFormat = a8Format;
		}

		pMaskPixmap = (*pScreen->CreatePixmap) (pScreen, width, height,
							maskFormat->depth,
							CREATE_PIXMAP_USAGE_SCRATCH);
		if (!pMaskPixmap)
			return;
		component_alpha = NeedsComponent(maskFormat->format);
		pMask = CreatePicture(0, &pMaskPixmap->drawable,
				      maskFormat, CPComponentAlpha,
				      &component_alpha, serverClient, &error);
		if (!pMask) {
			(*pScreen->DestroyPixmap) (pMaskPixmap);
			return;
		}
		pGC = GetScratchGC(pMaskPixmap->drawable.depth, pScreen);
		ValidateGC(&pMaskPixmap->drawable, pGC);
		rect.x = 0;
		rect.y = 0;
		rect.width = width;
		rect.height = height;
		(*pGC->ops->PolyFillRect) (&pMaskPixmap->drawable, pGC, 1,
					   &rect);
		FreeScratchGC(pGC);
		x = -extents.x1;
		y = -extents.y1;
	} else {
		pMask = pDst;
		x = 0;
		y = 0;
	}
	buffer.count = 0;
	buffer.source = NULL;
	while (nlist--) {
		x += list->xOff;
		y += list->yOff;
		n = list->len;
		while (n--) {
			glyph = *glyphs++;
			pPicture = GlyphPicture(glyph)[pScreen->myNum];

			if (glyph->info.width > 0 && glyph->info.height > 0 &&
			    uxa_buffer_glyph(pScreen, &buffer, glyph, x,
					     y) == UXA_GLYPH_NEED_FLUSH) {
				if (maskFormat)
					uxa_glyphs_to_mask(pMask, &buffer);
				else
					uxa_glyphs_to_dst(op, pSrc, pDst,
							  &buffer, xSrc, ySrc,
							  xDst, yDst);

				uxa_buffer_glyph(pScreen, &buffer, glyph, x, y);
			}

			x += glyph->info.xOff;
			y += glyph->info.yOff;
		}
		list++;
	}

	if (buffer.count) {
		if (maskFormat)
			uxa_glyphs_to_mask(pMask, &buffer);
		else
			uxa_glyphs_to_dst(op, pSrc, pDst, &buffer,
					  xSrc, ySrc, xDst, yDst);
	}

	if (maskFormat) {
		x = extents.x1;
		y = extents.y1;
		CompositePicture(op,
				 pSrc,
				 pMask,
				 pDst,
				 xSrc + x - xDst,
				 ySrc + y - yDst, 0, 0, x, y, width, height);
		FreePicture((pointer) pMask, (XID) 0);
		(*pScreen->DestroyPixmap) (pMaskPixmap);
	}
}
