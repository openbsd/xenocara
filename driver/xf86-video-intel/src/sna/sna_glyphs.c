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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sna.h"
#include "sna_render.h"
#include "sna_render_inline.h"
#include "fb/fbpict.h"

#define FALLBACK 0
#define NO_GLYPH_CACHE 0
#define NO_GLYPHS_TO_DST 0
#define FORCE_GLYPHS_TO_DST 0
#define NO_GLYPHS_VIA_MASK 0
#define FORCE_SMALL_MASK 0 /* -1 = never, 1 = always */
#define NO_GLYPHS_SLOW 0
#define NO_DISCARD_MASK 0

#define CACHE_PICTURE_SIZE 1024
#define GLYPH_MIN_SIZE 8
#define GLYPH_MAX_SIZE 64
#define GLYPH_CACHE_SIZE (CACHE_PICTURE_SIZE * CACHE_PICTURE_SIZE / (GLYPH_MIN_SIZE * GLYPH_MIN_SIZE))

#define N_STACK_GLYPHS 512
#define NO_ATLAS ((PicturePtr)-1)
#define GLYPH_TOLERANCE 3

#define glyph_valid(g) *((uint32_t *)&(g)->info.width)
#define glyph_copy_size(r, g) *(uint32_t *)&(r)->width = *(uint32_t *)&g->info.width

#if HAS_PIXMAN_GLYPHS
static  pixman_glyph_cache_t *__global_glyph_cache;
#endif

#if HAS_DEBUG_FULL
static void _assert_pixmap_contains_box(PixmapPtr pixmap, BoxPtr box, const char *function)
{
	if (box->x1 < 0 || box->y1 < 0 ||
	    box->x2 > pixmap->drawable.width ||
	    box->y2 > pixmap->drawable.height)
	{
		FatalError("%s: damage box is beyond the pixmap: box=(%d, %d), (%d, %d), pixmap=(%d, %d)\n",
			   function,
			   box->x1, box->y1, box->x2, box->y2,
			   pixmap->drawable.width,
			   pixmap->drawable.height);
	}
}
#define assert_pixmap_contains_box(p, b) _assert_pixmap_contains_box(p, b, __FUNCTION__)
#else
#define assert_pixmap_contains_box(p, b)
#endif

extern DevPrivateKeyRec sna_glyph_key;

static inline struct sna_glyph *sna_glyph(GlyphPtr glyph)
{
	return __get_private(glyph, sna_glyph_key);
}

static inline struct sna_glyph *sna_glyph0(GlyphPtr glyph)
{
	return (struct sna_glyph *)glyph->devPrivates;
}

static inline bool can_use_glyph0(void)
{
#if HAS_DEVPRIVATEKEYREC
	return sna_glyph_key.offset == 0;
#else
	return 0;
#endif
}

#define NeedsComponent(f) (PICT_FORMAT_A(f) != 0 && PICT_FORMAT_RGB(f) != 0)

static bool op_is_bounded(uint8_t op)
{
	switch (op) {
	case PictOpOver:
	case PictOpOutReverse:
	case PictOpAdd:
	case PictOpXor:
		return true;
	default:
		return false;
	}
}

void sna_glyphs_close(struct sna *sna)
{
	struct sna_render *render = &sna->render;
	unsigned int i;

	DBG(("%s\n", __FUNCTION__));

	for (i = 0; i < ARRAY_SIZE(render->glyph); i++) {
		struct sna_glyph_cache *cache = &render->glyph[i];

		if (cache->picture)
			FreePicture(cache->picture, 0);

		free(cache->glyphs);
	}
	memset(render->glyph, 0, sizeof(render->glyph));

	if (render->white_image) {
		pixman_image_unref(render->white_image);
		render->white_image = NULL;
	}
	if (render->white_picture) {
		FreePicture(render->white_picture, 0);
		render->white_picture = NULL;
	}
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
bool sna_glyphs_create(struct sna *sna)
{
	ScreenPtr screen = sna->scrn->pScreen;
	pixman_color_t white = { 0xffff, 0xffff, 0xffff, 0xffff };
	unsigned int formats[] = {
		PIXMAN_a8,
		PIXMAN_a8r8g8b8,
	};
	unsigned int i;
	int error;

	DBG(("%s\n", __FUNCTION__));

#if HAS_PIXMAN_GLYPHS
	if (__global_glyph_cache == NULL) {
		__global_glyph_cache = pixman_glyph_cache_create();
		if (__global_glyph_cache == NULL)
			goto bail;
	}
#endif

	sna->render.white_image = pixman_image_create_solid_fill(&white);
	if (sna->render.white_image == NULL)
		goto bail;

	if (!can_render(sna)) {
		DBG(("%s: no render acceleration, no render glyph caches\n",
		     __FUNCTION__));
		return true;
	}

	if (xf86IsEntityShared(sna->scrn->entityList[0])) {
		DBG(("%s: shared GlyphPictures, no render glyph caches\n",
		     __FUNCTION__));
		return true;
	}

	for (i = 0; i < ARRAY_SIZE(formats); i++) {
		struct sna_glyph_cache *cache = &sna->render.glyph[i];
		struct sna_pixmap *priv;
		PixmapPtr pixmap;
		PicturePtr picture = NULL;
		PictFormatPtr pPictFormat;
		CARD32 component_alpha;
		int depth = PIXMAN_FORMAT_DEPTH(formats[i]);

		pPictFormat = PictureMatchFormat(screen, depth, formats[i]);
		if (!pPictFormat)
			goto bail;

		/* Now allocate the pixmap and picture */
		pixmap = screen->CreatePixmap(screen,
					      CACHE_PICTURE_SIZE,
					      CACHE_PICTURE_SIZE,
					      depth,
					      SNA_CREATE_SCRATCH);
		if (!pixmap) {
			DBG(("%s: failed to allocate pixmap for Glyph cache\n",
			     __FUNCTION__));
			goto bail;
		}

		priv = sna_pixmap(pixmap);
		if (priv != NULL) {
			/* Prevent the cache from ever being paged out */
			assert(priv->gpu_bo);
			priv->pinned = PIN_SCANOUT;

			component_alpha = NeedsComponent(pPictFormat->format);
			picture = CreatePicture(0, &pixmap->drawable, pPictFormat,
						CPComponentAlpha, &component_alpha,
						serverClient, &error);
		}

		screen->DestroyPixmap(pixmap);
		if (!picture)
			goto bail;

		ValidatePicture(picture);
		assert(picture->pDrawable == &pixmap->drawable);

		cache->count = cache->evict = 0;
		cache->picture = picture;
		cache->glyphs = calloc(sizeof(struct sna_glyph *),
				       GLYPH_CACHE_SIZE);
		if (!cache->glyphs)
			goto bail;

		cache->evict = rand() % GLYPH_CACHE_SIZE;
	}

	sna->render.white_picture =
		CreateSolidPicture(0, (xRenderColor *)&white, &error);
	if (sna->render.white_picture == NULL)
		goto bail;

	return true;

bail:
	sna_glyphs_close(sna);
	return false;
}

static void
glyph_cache_upload(struct sna_glyph_cache *cache,
		   GlyphPtr glyph, PicturePtr glyph_picture,
		   int16_t x, int16_t y)
{
	DBG(("%s: upload glyph %p to cache (%d, %d)x(%d, %d)\n",
	     __FUNCTION__,
	     glyph, x, y,
	     glyph_picture->pDrawable->width,
	     glyph_picture->pDrawable->height));
	sna_composite(PictOpSrc,
		      glyph_picture, 0, cache->picture,
		      0, 0,
		      0, 0,
		      x, y,
		      glyph_picture->pDrawable->width,
		      glyph_picture->pDrawable->height);
}

static void
glyph_extents(int nlist,
	      GlyphListPtr list,
	      GlyphPtr *glyphs,
	      BoxPtr extents)
{
	int x1, x2, y1, y2;
	int x, y;

	x1 = y1 = MAXSHORT;
	x2 = y2 = MINSHORT;
	x = y = 0;
	while (nlist--) {
		int n = list->len;
		x += list->xOff;
		y += list->yOff;
		list++;
		while (n--) {
			GlyphPtr glyph = *glyphs++;

			if (glyph_valid(glyph)) {
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
			}

			x += glyph->info.xOff;
			y += glyph->info.yOff;
		}
	}

	extents->x1 = x1 > MINSHORT ? x1 : MINSHORT;
	extents->y1 = y1 > MINSHORT ? y1 : MINSHORT;
	extents->x2 = x2 < MAXSHORT ? x2 : MAXSHORT;
	extents->y2 = y2 < MAXSHORT ? y2 : MAXSHORT;
}

#if HAS_DEBUG_FULL
static int
glyph_count(int nlist,
	    GlyphListPtr list)
{
	int count = 0;
	while (nlist--) {
		count += list->len;
		list++;
	}
	return count;
}
#endif

static inline unsigned int
glyph_size_to_count(int size)
{
	size /= GLYPH_MIN_SIZE;
	return size * size;
}

static inline unsigned int
glyph_count_to_mask(int count)
{
	return ~(count - 1);
}

static inline unsigned int
glyph_size_to_mask(int size)
{
	return glyph_count_to_mask(glyph_size_to_count(size));
}

static int
glyph_cache(ScreenPtr screen,
	    struct sna_render *render,
	    GlyphPtr glyph)
{
	PicturePtr glyph_picture;
	struct sna_glyph_cache *cache;
	struct sna_glyph *p;
	int size, mask, pos, s;

	assert(glyph_valid(glyph));

	glyph_picture = GetGlyphPicture(glyph, screen);
	if (unlikely(glyph_picture == NULL)) {
		glyph->info.width = glyph->info.height = 0;
		return false;
	}

	if (NO_GLYPH_CACHE ||
	    glyph->info.width > GLYPH_MAX_SIZE ||
	    glyph->info.height > GLYPH_MAX_SIZE) {
		PixmapPtr pixmap = (PixmapPtr)glyph_picture->pDrawable;
		assert(glyph_picture->pDrawable->type == DRAWABLE_PIXMAP);
		if (pixmap->drawable.depth >= 8) {
			pixmap->usage_hint = 0;
			sna_pixmap_force_to_gpu(pixmap, MOVE_READ);
		}

		/* no cache for this glyph */
		p = sna_glyph(glyph);
		p->atlas = glyph_picture;
		p->coordinate.x = p->coordinate.y = 0;
		return true;
	}

	for (size = GLYPH_MIN_SIZE; size <= GLYPH_MAX_SIZE; size *= 2)
		if (glyph->info.width <= size && glyph->info.height <= size)
			break;

	cache = &render->glyph[PICT_FORMAT_RGB(glyph_picture->format) != 0];
	s = glyph_size_to_count(size);
	mask = glyph_count_to_mask(s);
	pos = (cache->count + s - 1) & mask;
	if (pos < GLYPH_CACHE_SIZE) {
		cache->count = pos + s;
	} else {
		p = NULL;
		for (s = size; s <= GLYPH_MAX_SIZE; s *= 2) {
			int i = cache->evict & glyph_size_to_mask(s);
			p = cache->glyphs[i];
			if (p == NULL)
				continue;

			if (p->size >= s) {
				cache->glyphs[i] = NULL;
				p->atlas = NULL;
				pos = i;
			} else
				p = NULL;
			break;
		}
		if (p == NULL) {
			int count = glyph_size_to_count(size);
			pos = cache->evict & glyph_count_to_mask(count);
			for (s = 0; s < count; s++) {
				p = cache->glyphs[pos + s];
				if (p != NULL) {
					p->atlas =NULL;
					cache->glyphs[pos + s] = NULL;
				}
			}
		}

		/* And pick a new eviction position */
		cache->evict = rand() % GLYPH_CACHE_SIZE;
	}
	assert(cache->glyphs[pos] == NULL);

	p = sna_glyph(glyph);
	DBG(("%s(%d): adding glyph to cache %d, pos %d\n",
	     __FUNCTION__, screen->myNum,
	     PICT_FORMAT_RGB(glyph_picture->format) != 0, pos));
	cache->glyphs[pos] = p;
	p->atlas = cache->picture;
	p->size = size;
	p->pos = pos << 1 | (PICT_FORMAT_RGB(glyph_picture->format) != 0);
	s = pos / ((GLYPH_MAX_SIZE / GLYPH_MIN_SIZE) * (GLYPH_MAX_SIZE / GLYPH_MIN_SIZE));
	p->coordinate.x = s % (CACHE_PICTURE_SIZE / GLYPH_MAX_SIZE) * GLYPH_MAX_SIZE;
	p->coordinate.y = (s / (CACHE_PICTURE_SIZE / GLYPH_MAX_SIZE)) * GLYPH_MAX_SIZE;
	for (s = GLYPH_MIN_SIZE; s < GLYPH_MAX_SIZE; s *= 2) {
		if (pos & 1)
			p->coordinate.x += s;
		if (pos & 2)
			p->coordinate.y += s;
		pos >>= 2;
	}

	glyph_cache_upload(cache, glyph, glyph_picture,
			   p->coordinate.x, p->coordinate.y);

	return true;
}

static void apply_damage(struct sna_composite_op *op,
			 const struct sna_composite_rectangles *r)
{
	BoxRec box;

	if (op->damage == NULL)
		return;

	box.x1 = r->dst.x + op->dst.x;
	box.y1 = r->dst.y + op->dst.y;
	box.x2 = box.x1 + r->width;
	box.y2 = box.y1 + r->height;

	assert_pixmap_contains_box(op->dst.pixmap, &box);
	sna_damage_add_box(op->damage, &box);
}

static void apply_damage_clipped_to_dst(struct sna_composite_op *op,
					const struct sna_composite_rectangles *r,
					DrawablePtr dst)
{
	BoxRec box;

	if (op->damage == NULL)
		return;

	box.x1 = r->dst.x + op->dst.x;
	box.y1 = r->dst.y + op->dst.y;
	box.x2 = box.x1 + r->width;
	box.y2 = box.y1 + r->height;

	if (box.x1 < dst->x)
		box.x1 = dst->x;

	if (box.x2 > op->dst.width)
		box.x2 = op->dst.width;

	if (box.y1 < dst->y)
		box.y1 = dst->y;

	if (box.y2 > op->dst.height)
		box.y2 = op->dst.height;

	assert_pixmap_contains_box(op->dst.pixmap, &box);
	sna_damage_add_box(op->damage, &box);
}

static inline bool region_matches_pixmap(const RegionRec *r, PixmapPtr pixmap)
{
	return (r->extents.x2 - r->extents.x1 >= pixmap->drawable.width &&
		r->extents.y2 - r->extents.y1 >= pixmap->drawable.height);
}

static inline bool clipped_glyphs(PicturePtr dst, int nlist, GlyphListPtr list, GlyphPtr *glyphs)
{
	BoxRec box;

	if (dst->pCompositeClip->data == NULL &&
	    region_matches_pixmap(dst->pCompositeClip,
				  get_drawable_pixmap(dst->pDrawable))) {
		DBG(("%s: no, clip region (%d, %d), (%d, %d) matches drawable pixmap=%ld size=%dx%d\n",
		     __FUNCTION__,
		     dst->pCompositeClip->extents.x1,
		     dst->pCompositeClip->extents.y1,
		     dst->pCompositeClip->extents.x2,
		     dst->pCompositeClip->extents.y2,
		     get_drawable_pixmap(dst->pDrawable),
		     get_drawable_pixmap(dst->pDrawable)->drawable.width,
		     get_drawable_pixmap(dst->pDrawable)->drawable.height));
		return false;
	}

	glyph_extents(nlist, list, glyphs, &box);

	box.x1 += dst->pDrawable->x;
	box.x2 += dst->pDrawable->x;
	box.y1 += dst->pDrawable->y;
	box.y2 += dst->pDrawable->y;

	DBG(("%s? %d glyph in %d lists extents (%d, %d), (%d, %d), region (%d, %d), (%d, %d): %s\n",
	     __FUNCTION__, glyph_count(nlist, list), nlist, box.x1, box.y1, box.x2, box.y2,
	     dst->pCompositeClip->extents.x1, dst->pCompositeClip->extents.y1,
	     dst->pCompositeClip->extents.x2, dst->pCompositeClip->extents.y2,
	     pixman_region_contains_rectangle(dst->pCompositeClip,
					      &box) != PIXMAN_REGION_IN ?  "yes" : "no"));

	return pixman_region_contains_rectangle(dst->pCompositeClip,
						&box) != PIXMAN_REGION_IN;
}

flatten static bool
glyphs_to_dst(struct sna *sna,
	      CARD8 op,
	      PicturePtr src,
	      PicturePtr dst,
	      INT16 src_x, INT16 src_y,
	      int nlist, GlyphListPtr list, GlyphPtr *glyphs)
{
	struct sna_composite_op tmp;
	ScreenPtr screen = dst->pDrawable->pScreen;
	PicturePtr glyph_atlas;
	const BoxRec *rects;
	int nrect;
	int16_t x, y;

	if (NO_GLYPHS_TO_DST)
		return false;

	memset(&tmp, 0, sizeof(tmp));

	DBG(("%s(op=%d, src=(%d, %d), nlist=%d,  dst=(%d, %d)+(%d, %d))\n",
	     __FUNCTION__, op, src_x, src_y, nlist,
	     list->xOff, list->yOff, dst->pDrawable->x, dst->pDrawable->y));

	if (clipped_glyphs(dst, nlist, list, glyphs)) {
		rects = region_rects(dst->pCompositeClip);
		nrect = region_num_rects(dst->pCompositeClip);
	} else
		nrect = 0;

	x = dst->pDrawable->x;
	y = dst->pDrawable->y;
	src_x -= list->xOff + x;
	src_y -= list->yOff + y;

	glyph_atlas = NO_ATLAS;
	while (nlist--) {
		int n = list->len;
		x += list->xOff;
		y += list->yOff;
		while (n--) {
			GlyphPtr glyph = *glyphs++;
			struct sna_glyph *p;
			int i;

			p = sna_glyph(glyph);
			if (unlikely(p->atlas != glyph_atlas)) {
				if (unlikely(!glyph_valid(glyph)))
					goto next_glyph;

				if (glyph_atlas != NO_ATLAS) {
					tmp.done(sna, &tmp);
					glyph_atlas = NO_ATLAS;
				}

				if (p->atlas == NULL &&
				    !glyph_cache(screen, &sna->render, glyph))
					goto next_glyph;

				if (!sna->render.composite(sna,
							   op, src, p->atlas, dst,
							   0, 0, 0, 0, 0, 0,
							   0, 0,
							   COMPOSITE_PARTIAL, &tmp))
					return false;

				glyph_atlas = p->atlas;
			}

			if (nrect) {
				int xi = x - glyph->info.x;
				int yi = y - glyph->info.y;

				if (xi < dst->pCompositeClip->extents.x2 &&
				    yi < dst->pCompositeClip->extents.y2 &&
				    xi + glyph->info.width  > dst->pCompositeClip->extents.x1 &&
				    yi + glyph->info.height > dst->pCompositeClip->extents.y1) {
					for (i = 0; i < nrect; i++) {
						struct sna_composite_rectangles r;
						int16_t dx, dy;
						int16_t x2, y2;

						r.dst.x = xi;
						r.dst.y = yi;
						x2 = xi + glyph->info.width;
						y2 = yi + glyph->info.height;
						dx = dy = 0;

						DBG(("%s: glyph=(%d, %d), (%d, %d), clip=(%d, %d), (%d, %d)\n",
						     __FUNCTION__,
						     r.dst.x, r.dst.y, x2, y2,
						     rects[i].x1, rects[i].y1,
						     rects[i].x2, rects[i].y2));
						if (rects[i].y1 >= y2)
							break;

						if (r.dst.x < rects[i].x1)
							dx = rects[i].x1 - r.dst.x, r.dst.x = rects[i].x1;
						if (x2 > rects[i].x2)
							x2 = rects[i].x2;
						if (r.dst.y < rects[i].y1)
							dy = rects[i].y1 - r.dst.y, r.dst.y = rects[i].y1;
						if (y2 > rects[i].y2)
							y2 = rects[i].y2;

						assert(dx >= 0 && dy >= 0);

						if (r.dst.x < x2 && r.dst.y < y2) {
							DBG(("%s: blt=(%d, %d), (%d, %d)\n",
							     __FUNCTION__, r.dst.x, r.dst.y, x2, y2));

							r.src.x = r.dst.x + src_x;
							r.src.y = r.dst.y + src_y;
							r.mask.x = dx + p->coordinate.x;
							r.mask.y = dy + p->coordinate.y;
							r.width  = x2 - r.dst.x;
							r.height = y2 - r.dst.y;
							tmp.blt(sna, &tmp, &r);
							apply_damage(&tmp, &r);
						}
					}
				}
			} else {
				struct sna_composite_rectangles r;

				r.dst.x = x - glyph->info.x;
				r.dst.y = y - glyph->info.y;
				r.src.x = r.dst.x + src_x;
				r.src.y = r.dst.y + src_y;
				r.mask = p->coordinate;
				glyph_copy_size(&r, glyph);

				DBG(("%s: glyph=(%d, %d)x(%d, %d), unclipped\n",
				     __FUNCTION__,
				     r.dst.x, r.dst.y,
				     r.width, r.height));

				tmp.blt(sna, &tmp, &r);
				apply_damage_clipped_to_dst(&tmp, &r, dst->pDrawable);
			}

next_glyph:
			x += glyph->info.xOff;
			y += glyph->info.yOff;
		}
		list++;
	}
	if (glyph_atlas)
		tmp.done(sna, &tmp);

	return true;
}

flatten static bool
glyphs0_to_dst(struct sna *sna,
	       CARD8 op,
	       PicturePtr src,
	       PicturePtr dst,
	       INT16 src_x, INT16 src_y,
	       int nlist, GlyphListPtr list, GlyphPtr *glyphs)
{
	struct sna_composite_op tmp;
	ScreenPtr screen = dst->pDrawable->pScreen;
	PicturePtr glyph_atlas = NO_ATLAS;
	int x, y;

	if (NO_GLYPHS_TO_DST)
		return false;

	memset(&tmp, 0, sizeof(tmp));

	DBG(("%s(op=%d, src=(%d, %d), nlist=%d,  dst=(%d, %d)+(%d, %d))\n",
	     __FUNCTION__, op, src_x, src_y, nlist,
	     list->xOff, list->yOff, dst->pDrawable->x, dst->pDrawable->y));

	x = dst->pDrawable->x;
	y = dst->pDrawable->y;
	src_x -= list->xOff + x;
	src_y -= list->yOff + y;

	if (clipped_glyphs(dst, nlist, list, glyphs)) {
		const BoxRec *rects = region_rects(dst->pCompositeClip);
		int nrect = region_num_rects(dst->pCompositeClip);
		if (nrect == 0)
			return true;

		while (nlist--) {
			int n = list->len;
			x += list->xOff;
			y += list->yOff;
			while (n--) {
				GlyphPtr glyph = *glyphs++;
				struct sna_glyph *p = sna_glyph0(glyph);
				int i, xi, yi;

				if (unlikely(p->atlas != glyph_atlas)) {
					if (unlikely(!glyph_valid(glyph)))
						goto next_glyph_N;

					if (glyph_atlas != NO_ATLAS) {
						tmp.done(sna, &tmp);
						glyph_atlas = NO_ATLAS;
					}

					if (unlikely(p->atlas == NULL)) {
						if (!glyph_cache(screen, &sna->render, glyph))
							goto next_glyph_N;
					}

					if (!sna->render.composite(sna,
								   op, src, p->atlas, dst,
								   0, 0, 0, 0, 0, 0,
								   0, 0,
								   COMPOSITE_PARTIAL, &tmp))
						return false;

					glyph_atlas = p->atlas;
				}

				xi = x - glyph->info.x;
				yi = y - glyph->info.y;

				if (xi < dst->pCompositeClip->extents.x2 &&
				    yi < dst->pCompositeClip->extents.y2 &&
				    xi + glyph->info.width  > dst->pCompositeClip->extents.x1 &&
				    yi + glyph->info.height > dst->pCompositeClip->extents.y1) {
					for (i = 0; i < nrect; i++) {
						struct sna_composite_rectangles r;
						int16_t dx, dy;
						int16_t x2, y2;

						r.dst.x = xi;
						r.dst.y = yi;
						x2 = xi + glyph->info.width;
						y2 = yi + glyph->info.height;
						dx = dy = 0;

						DBG(("%s: glyph=(%d, %d), (%d, %d), clip=(%d, %d), (%d, %d)\n",
						     __FUNCTION__,
						     r.dst.x, r.dst.y, x2, y2,
						     rects[i].x1, rects[i].y1,
						     rects[i].x2, rects[i].y2));
						if (rects[i].y1 >= y2)
							break;

						if (r.dst.x < rects[i].x1)
							dx = rects[i].x1 - r.dst.x, r.dst.x = rects[i].x1;
						if (x2 > rects[i].x2)
							x2 = rects[i].x2;
						if (r.dst.y < rects[i].y1)
							dy = rects[i].y1 - r.dst.y, r.dst.y = rects[i].y1;
						if (y2 > rects[i].y2)
							y2 = rects[i].y2;

						assert(dx >= 0 && dy >= 0);

						if (r.dst.x < x2 && r.dst.y < y2) {
							DBG(("%s: blt=(%d, %d), (%d, %d)\n",
							     __FUNCTION__, r.dst.x, r.dst.y, x2, y2));

							r.src.x = r.dst.x + src_x;
							r.src.y = r.dst.y + src_y;
							r.mask.x = dx + p->coordinate.x;
							r.mask.y = dy + p->coordinate.y;
							r.width  = x2 - r.dst.x;
							r.height = y2 - r.dst.y;
							tmp.blt(sna, &tmp, &r);
							apply_damage(&tmp, &r);
						}
					}
				}

next_glyph_N:
				x += glyph->info.xOff;
				y += glyph->info.yOff;
			}
			list++;
		}
	} else while (nlist--) {
		int n = list->len;
		x += list->xOff;
		y += list->yOff;
		while (n--) {
			GlyphPtr glyph = *glyphs++;
			struct sna_glyph *p = sna_glyph0(glyph);
			struct sna_composite_rectangles r;

			if (unlikely(p->atlas != glyph_atlas)) {
				if (unlikely(!glyph_valid(glyph)))
					goto next_glyph_0;

				if (glyph_atlas != NO_ATLAS) {
					tmp.done(sna, &tmp);
					glyph_atlas = NO_ATLAS;
				}

				if (unlikely(p->atlas == NULL)) {
					if (!glyph_cache(screen, &sna->render, glyph))
						goto next_glyph_0;
				}

				if (!sna->render.composite(sna,
							   op, src, p->atlas, dst,
							   0, 0, 0, 0, 0, 0,
							   0, 0,
							   COMPOSITE_PARTIAL, &tmp))
					return false;

				glyph_atlas = p->atlas;
			}

			r.dst.x = x - glyph->info.x;
			r.dst.y = y - glyph->info.y;
			r.src.x = r.dst.x + src_x;
			r.src.y = r.dst.y + src_y;
			r.mask = p->coordinate;
			glyph_copy_size(&r, glyph);

			DBG(("%s: glyph=(%d, %d)x(%d, %d), unclipped\n",
			     __FUNCTION__,
			     r.dst.x, r.dst.y,
			     r.width, r.height));

			tmp.blt(sna, &tmp, &r);
			apply_damage_clipped_to_dst(&tmp, &r, dst->pDrawable);

next_glyph_0:
			x += glyph->info.xOff;
			y += glyph->info.yOff;
		}
		list++;
	}
	if (glyph_atlas != NO_ATLAS)
		tmp.done(sna, &tmp);

	return true;
}

static bool
glyphs_slow(struct sna *sna,
	    CARD8 op,
	    PicturePtr src,
	    PicturePtr dst,
	    INT16 src_x, INT16 src_y,
	    int nlist, GlyphListPtr list, GlyphPtr *glyphs)
{
	struct sna_composite_op tmp;
	ScreenPtr screen = dst->pDrawable->pScreen;
	int16_t x, y;

	if (NO_GLYPHS_SLOW)
		return false;

	DBG(("%s(op=%d, src=(%d, %d), nlist=%d,  dst=(%d, %d)+(%d, %d))\n",
	     __FUNCTION__, op, src_x, src_y, nlist,
	     list->xOff, list->yOff, dst->pDrawable->x, dst->pDrawable->y));

	x = dst->pDrawable->x;
	y = dst->pDrawable->y;
	src_x -= list->xOff + x;
	src_y -= list->yOff + y;

	while (nlist--) {
		int n = list->len;
		x += list->xOff;
		y += list->yOff;
		while (n--) {
			GlyphPtr glyph = *glyphs++;
			struct sna_glyph *p;
			const BoxRec *rects;
			BoxRec box;
			int nrect;

			box.x1 = x - glyph->info.x;
			box.y1 = y - glyph->info.y;
			box.x2 = bound(box.x1, glyph->info.width);
			box.y2 = bound(box.y1, glyph->info.height);

			if (!box_intersect(&box,
					   &dst->pCompositeClip->extents))
				goto next_glyph;

			p = sna_glyph(glyph);
			if (unlikely(p->atlas == NULL)) {
				if (unlikely(!glyph_valid(glyph)))
					goto next_glyph;

				if (!glyph_cache(screen, &sna->render, glyph))
					goto next_glyph;
			}

			DBG(("%s: glyph=(%d, %d)x(%d, %d), src=(%d, %d), mask=(%d, %d)\n",
			     __FUNCTION__,
			     x - glyph->info.x,
			     y - glyph->info.y,
			     glyph->info.width,
			     glyph->info.height,
			     src_x + x - glyph->info.x,
			     src_y + y - glyph->info.y,
			     p->coordinate.x, p->coordinate.y));

			if (!sna->render.composite(sna,
						   op, src, p->atlas, dst,
						   src_x + x - glyph->info.x,
						   src_y + y - glyph->info.y,
						   p->coordinate.x, p->coordinate.y,
						   x - glyph->info.x,
						   y - glyph->info.y,
						   glyph->info.width,
						   glyph->info.height,
						   COMPOSITE_PARTIAL, memset(&tmp, 0, sizeof(tmp))))
				return false;

			rects = region_rects(dst->pCompositeClip);
			nrect = region_num_rects(dst->pCompositeClip);
			do {
				struct sna_composite_rectangles r;
				int16_t x2, y2;

				r.dst.x = x - glyph->info.x;
				r.dst.y = y - glyph->info.y;
				x2 = r.dst.x + glyph->info.width;
				y2 = r.dst.y + glyph->info.height;

				DBG(("%s: glyph=(%d, %d), (%d, %d), clip=(%d, %d), (%d, %d)\n",
				     __FUNCTION__,
				     r.dst.x, r.dst.y, x2, y2,
				     rects->x1, rects->y1,
				     rects->x2, rects->y2));
				if (rects->y1 >= y2)
					break;

				if (r.dst.x < rects->x1)
					r.dst.x = rects->x1;
				if (x2 > rects->x2)
					x2 = rects->x2;

				if (r.dst.y < rects->y1)
					r.dst.y = rects->y1;
				if (y2 > rects->y2)
					y2 = rects->y2;

				if (r.dst.x < x2 && r.dst.y < y2) {
					DBG(("%s: blt=(%d, %d), (%d, %d)\n",
					     __FUNCTION__, r.dst.x, r.dst.y, x2, y2));
					r.width  = x2 - r.dst.x;
					r.height = y2 - r.dst.y;
					r.src = r.mask = r .dst;
					tmp.blt(sna, &tmp, &r);
					apply_damage(&tmp, &r);
				}
				rects++;
			} while (--nrect);
			tmp.done(sna, &tmp);

next_glyph:
			x += glyph->info.xOff;
			y += glyph->info.yOff;
		}
		list++;
	}

	return true;
}

static bool
clear_pixmap(struct sna *sna, PixmapPtr pixmap)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	return sna->render.clear(sna, pixmap, priv->gpu_bo);
}

static bool
too_large(struct sna *sna, int width, int height)
{
	return (width > sna->render.max_3d_size ||
		height > sna->render.max_3d_size);
}

static pixman_image_t *
__sna_glyph_get_image(GlyphPtr g, ScreenPtr s)
{
	pixman_image_t *image;
	PicturePtr p;
	int dx, dy;

	DBG(("%s: creating image cache for glyph %p (on screen %d)\n", __FUNCTION__, g, s->myNum));

	p = GetGlyphPicture(g, s);
	if (unlikely(p == NULL))
		return NULL;

	image = image_from_pict(p, FALSE, &dx, &dy);
	if (!image)
		return NULL;

	assert(dx == 0 && dy == 0);
	return sna_glyph(g)->image = image;
}

static inline pixman_image_t *
sna_glyph_get_image(GlyphPtr g, ScreenPtr s)
{
	pixman_image_t *image;

	image = sna_glyph(g)->image;
	if (image == NULL)
		image = __sna_glyph_get_image(g, s);

	return image;
}

static inline bool use_small_mask(struct sna *sna, int16_t width, int16_t height, int depth)
{
	if (FORCE_SMALL_MASK)
		return FORCE_SMALL_MASK > 0;

	if (depth * width * height < 8 * 4096)
		return true;

	return too_large(sna, width, height);
}

flatten static bool
glyphs_via_mask(struct sna *sna,
		CARD8 op,
		PicturePtr src,
		PicturePtr dst,
		PictFormatPtr format,
		INT16 src_x, INT16 src_y,
		int nlist, GlyphListPtr list, GlyphPtr *glyphs)
{
	ScreenPtr screen = dst->pDrawable->pScreen;
	CARD32 component_alpha;
	PixmapPtr pixmap;
	PicturePtr mask;
	int16_t x, y, width, height;
	int error;
	bool ret = false;
	BoxRec box;

	if (NO_GLYPHS_VIA_MASK)
		return false;

	DBG(("%s(op=%d, src=(%d, %d), nlist=%d,  dst=(%d, %d)+(%d, %d))\n",
	     __FUNCTION__, op, src_x, src_y, nlist,
	     list->xOff, list->yOff, dst->pDrawable->x, dst->pDrawable->y));

	glyph_extents(nlist, list, glyphs, &box);
	if (box.x2 <= box.x1 || box.y2 <= box.y1)
		return true;

	DBG(("%s: nlist=%d, count=%d, bounds=((%d, %d), (%d, %d))\n", __FUNCTION__,
	     nlist, glyph_count(nlist, list), box.x1, box.y1, box.x2, box.y2));

	if (!sna_compute_composite_extents(&box,
					   src, NULL, dst,
					   src_x, src_y,
					   0, 0,
					   box.x1, box.y1,
					   box.x2 - box.x1,
					   box.y2 - box.y1))
		return true;

	DBG(("%s: extents=((%d, %d), (%d, %d))\n", __FUNCTION__,
	     box.x1, box.y1, box.x2, box.y2));

	width  = box.x2 - box.x1;
	height = box.y2 - box.y1;
	box.x1 -= dst->pDrawable->x;
	box.y1 -= dst->pDrawable->y;
	x = -box.x1;
	y = -box.y1;
	src_x += box.x1 - list->xOff;
	src_y += box.y1 - list->yOff;

	if (format->depth < 8) {
		format = PictureMatchFormat(screen, 8, PICT_a8);
		if (!format)
			return false;
	}

	component_alpha = NeedsComponent(format->format);
	if (use_small_mask(sna, width, height, format->depth)) {
		pixman_image_t *mask_image;

use_small_mask:
		DBG(("%s: small mask [format=%lx, depth=%d, size=%d], rendering glyphs to upload buffer\n",
		     __FUNCTION__, (unsigned long)format->format,
		     format->depth, (uint32_t)width*height*format->depth));

		pixmap = sna_pixmap_create_upload(screen,
						  width, height,
						  format->depth,
						  KGEM_BUFFER_WRITE);
		if (!pixmap)
			return false;

		mask_image =
			pixman_image_create_bits(format->depth << 24 | format->format,
						 width, height,
						 pixmap->devPrivate.ptr,
						 pixmap->devKind);
		if (mask_image == NULL)
			goto err_pixmap;

		if (sigtrap_get()) {
			pixman_image_unref(mask_image);
			goto err_pixmap;
		}

		memset(pixmap->devPrivate.ptr, 0, pixmap->devKind*height);
#if HAS_PIXMAN_GLYPHS
		if (__global_glyph_cache) {
			pixman_glyph_t stack_glyphs[N_STACK_GLYPHS];
			pixman_glyph_t *pglyphs = stack_glyphs;
			int count, n;

			count = 0;
			for (n = 0; n < nlist; ++n)
				count += list[n].len;
			if (count > N_STACK_GLYPHS) {
				pglyphs = malloc (count * sizeof(pixman_glyph_t));
				if (pglyphs == NULL)
					goto err_pixmap;
			}

			pixman_glyph_cache_freeze(__global_glyph_cache);
			count = 0;
			do {
				n = list->len;
				x += list->xOff;
				y += list->yOff;
				while (n--) {
					GlyphPtr g = *glyphs++;
					const void *ptr;

					if (!glyph_valid(g))
						goto next_pglyph;

					ptr = pixman_glyph_cache_lookup(__global_glyph_cache, g, NULL);
					if (ptr == NULL) {
						pixman_image_t *glyph_image;

						glyph_image = sna_glyph_get_image(g, screen);
						if (glyph_image == NULL)
							goto next_pglyph;

						DBG(("%s: inserting glyph %p into pixman cache\n", __FUNCTION__, g));
						ptr = pixman_glyph_cache_insert(__global_glyph_cache, g, NULL,
										g->info.x,
										g->info.y,
										glyph_image);
						if (ptr == NULL)
							goto next_pglyph;
					}

					assert(sna_glyph_get_image(g, screen) != NULL);

					pglyphs[count].x = x;
					pglyphs[count].y = y;
					pglyphs[count].glyph = ptr;
					count++;

next_pglyph:
					x += g->info.xOff;
					y += g->info.yOff;
				}
				list++;
			} while (--nlist);

			pixman_composite_glyphs_no_mask(PIXMAN_OP_ADD,
							sna->render.white_image,
							mask_image,
							0, 0,
							0, 0,
							__global_glyph_cache, count, pglyphs);
			pixman_glyph_cache_thaw(__global_glyph_cache);
			if (pglyphs != stack_glyphs)
				free(pglyphs);
		} else
#endif
		do {
			int n = list->len;
			x += list->xOff;
			y += list->yOff;
			while (n--) {
				GlyphPtr g = *glyphs++;
				pixman_image_t *glyph_image;
				int16_t xi, yi;

				if (!glyph_valid(g))
					goto next_image;

				/* If the mask has been cropped, it is likely
				 * that some of the glyphs fall outside.
				 */
				xi = x - g->info.x;
				yi = y - g->info.y;
				if (xi >= width || yi >= height)
					goto next_image;
				if (xi + g->info.width  <= 0 ||
				    yi + g->info.height <= 0)
					goto next_image;

				glyph_image =
					sna_glyph_get_image(g, dst->pDrawable->pScreen);
				if (glyph_image == NULL)
					goto next_image;

				DBG(("%s: glyph to mask (%d, %d)x(%d, %d)\n",
				     __FUNCTION__,
				     xi, yi,
				     g->info.width,
				     g->info.height));

				if (list->format == format) {
					assert(pixman_image_get_format(glyph_image) == pixman_image_get_format(mask_image));
					pixman_image_composite(PictOpAdd,
							       glyph_image,
							       NULL,
							       mask_image,
							       0, 0,
							       0, 0,
							       xi, yi,
							       g->info.width,
							       g->info.height);
				} else {
					pixman_image_composite(PictOpAdd,
							       sna->render.white_image,
							       glyph_image,
							       mask_image,
							       0, 0,
							       0, 0,
							       xi, yi,
							       g->info.width,
							       g->info.height);
				}

next_image:
				x += g->info.xOff;
				y += g->info.yOff;
			}
			list++;
		} while (--nlist);
		pixman_image_unref(mask_image);

		sigtrap_put();

		mask = CreatePicture(0, &pixmap->drawable,
				     format, CPComponentAlpha,
				     &component_alpha, serverClient, &error);
		if (!mask)
			goto err_pixmap;

		ValidatePicture(mask);
	} else {
		struct sna_composite_op tmp;
		PicturePtr glyph_atlas = NO_ATLAS;

		pixmap = screen->CreatePixmap(screen,
					      width, height, format->depth,
					      SNA_CREATE_SCRATCH);
		if (!pixmap)
			goto use_small_mask;

		assert(__sna_pixmap_get_bo(pixmap));

		mask = CreatePicture(0, &pixmap->drawable,
				     format, CPComponentAlpha,
				     &component_alpha, serverClient, &error);
		if (!mask)
			goto err_pixmap;

		ValidatePicture(mask);
		if (!clear_pixmap(sna, pixmap))
			goto err_mask;

		do {
			int n = list->len;
			x += list->xOff;
			y += list->yOff;
			while (n--) {
				GlyphPtr glyph = *glyphs++;
				struct sna_glyph *p = sna_glyph(glyph);
				struct sna_composite_rectangles r;

				if (unlikely(p->atlas != glyph_atlas)) {
					bool ok;

					if (unlikely(!glyph_valid(glyph)))
						goto next_glyph;

					if (glyph_atlas != NO_ATLAS) {
						tmp.done(sna, &tmp);
						glyph_atlas = NO_ATLAS;
					}

					if (unlikely(p->atlas == NULL)) {
						if (!glyph_cache(screen, &sna->render, glyph))
							goto next_glyph;
					}

					DBG(("%s: atlas format=%08x, mask format=%08x\n",
					     __FUNCTION__,
					     (int)p->atlas->format,
					     (int)(format->depth << 24 | format->format)));

					memset(&tmp, 0, sizeof(tmp));
					if (p->atlas->format == (format->depth << 24 | format->format)) {
						ok = sna->render.composite(sna, PictOpAdd,
									   p->atlas, NULL, mask,
									   0, 0, 0, 0, 0, 0,
									   0, 0,
									   COMPOSITE_PARTIAL, &tmp);
					} else {
						ok = sna->render.composite(sna, PictOpAdd,
									   sna->render.white_picture, p->atlas, mask,
									   0, 0, 0, 0, 0, 0,
									   0, 0,
									   COMPOSITE_PARTIAL, &tmp);
					}
					if (!ok) {
						DBG(("%s: fallback -- can not handle PictOpAdd of glyph onto mask!\n",
						     __FUNCTION__));
						goto err_mask;
					}

					glyph_atlas = p->atlas;
				}

				DBG(("%s: blt glyph origin (%d, %d), offset (%d, %d), src (%d, %d), size (%d, %d)\n",
				     __FUNCTION__,
				     x, y,
				     glyph->info.x, glyph->info.y,
				     p->coordinate.x, p->coordinate.y,
				     glyph->info.width, glyph->info.height));

				r.mask = r.src = p->coordinate;
				r.dst.x = x - glyph->info.x;
				r.dst.y = y - glyph->info.y;
				glyph_copy_size(&r, glyph);
				tmp.blt(sna, &tmp, &r);

next_glyph:
				x += glyph->info.xOff;
				y += glyph->info.yOff;
			}
			list++;
		} while (--nlist);
		if (glyph_atlas != NO_ATLAS)
			tmp.done(sna, &tmp);
	}

	sna_composite(op,
		      src, mask, dst,
		      src_x, src_y,
		      0, 0,
		      box.x1, box.y1,
		      width, height);
	ret = true;
err_mask:
	FreePicture(mask, 0);
err_pixmap:
	sna_pixmap_destroy(pixmap);
	return ret;
}

static PictFormatPtr
glyphs_format(int nlist, GlyphListPtr list, GlyphPtr * glyphs)
{
	PictFormatPtr format = list[0].format;
	int16_t x1, x2, y1, y2;
	int16_t x, y;
	BoxRec stack_extents[64], *list_extents = stack_extents;
	int i, j;

	if (nlist > ARRAY_SIZE(stack_extents)) {
		list_extents = malloc(sizeof(BoxRec) * nlist);
		if (list_extents == NULL)
			return NULL;
	}

	x = y = 0; i = 0;
	while (nlist--) {
		BoxRec extents;
		bool first = true;
		int n = list->len;

		/* Check the intersection of each glyph within the list and
		 * then each list against the previous lists.
		 *
		 * If we overlap then we cannot substitute a mask as the
		 * rendering will be altered.
		 */
		if (format->format != list->format->format) {
			DBG(("%s: switching formats from %x to %x\n",
			     __FUNCTION__,
			     (unsigned)format->format,
			     (unsigned)list->format->format));
			format = NULL;
			goto out;
		}

		x += list->xOff;
		y += list->yOff;
		list++;
		while (n--) {
			GlyphPtr glyph = *glyphs++;

			if (!glyph_valid(glyph))
				goto skip_glyph;

			x1 = x - glyph->info.x;
			y1 = y - glyph->info.y;
			x2 = x1 + glyph->info.width;
			y2 = y1 + glyph->info.height;

			if (first) {
				extents.x1 = x1;
				extents.y1 = y1;
				extents.x2 = x2;
				extents.y2 = y2;
				first = false;
			} else {
				/* Potential overlap?
				 * We cheat and ignore the boundary pixels, as
				 * the likelihood of an actual overlap of
				 * inkedk pixels being noticeable in the
				 * boundary is small, yet glyphs frequently
				 * overlap on the boundaries.
				 */
				if (x1 < extents.x2-GLYPH_TOLERANCE &&
				    x2 > extents.x1+GLYPH_TOLERANCE &&
				    y1 < extents.y2-GLYPH_TOLERANCE &&
				    y2 > extents.y1+GLYPH_TOLERANCE) {
					DBG(("%s: overlapping glyph inside line, current bbox (%d, %d), (%d, %d), glyph (%d, %d), (%d, %d)\n",
					     __FUNCTION__,
					     extents.x1, extents.y1, extents.x2, extents.y2,
					     x1, y1, x2, y2));
					format = NULL;
					goto out;
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
skip_glyph:
			x += glyph->info.xOff;
			y += glyph->info.yOff;
		}

		/* Incrementally building a region is expensive. We expect
		 * the number of lists to be small, so just keep a list
		 * of the previous boxes and walk those.
		 */
		if (!first) {
			for (j = 0; j < i; j++) {
				if (extents.x1 < list_extents[j].x2-GLYPH_TOLERANCE &&
				    extents.x2 > list_extents[j].x1+GLYPH_TOLERANCE &&
				    extents.y1 < list_extents[j].y2-GLYPH_TOLERANCE &&
				    extents.y2 > list_extents[j].y1+GLYPH_TOLERANCE) {
					DBG(("%s: overlapping lines, current bbox (%d, %d), (%d, %d), previous line (%d, %d), (%d, %d)\n",
					     __FUNCTION__,
					     extents.x1, extents.y1, extents.x2, extents.y2,
					     list_extents[j].x1, list_extents[j].y1,
					     list_extents[j].x2, list_extents[j].y2));
					format = NULL;
					goto out;
				}
			}
			list_extents[i++] = extents;
		}
	}

out:
	if (list_extents != stack_extents)
		free(list_extents);
	return format;
}

static bool can_discard_mask(uint8_t op, PicturePtr src, PictFormatPtr mask,
			     int nlist, GlyphListPtr list, GlyphPtr *glyphs)
{
	PictFormatPtr g;
	uint32_t color;

	if (NO_DISCARD_MASK)
		return false;

	DBG(("%s: nlist=%d, mask=%08x, depth %d, op=%d (bounded? %d)\n",
	     __FUNCTION__, nlist,
	     mask ? (unsigned)mask->format : 0, mask ? mask->depth : 0,
	     op, op_is_bounded(op)));

	if (nlist == 1 && list->len == 1)
		return true;

	if (!op_is_bounded(op))
		return false;

	/* No glyphs overlap and we are not performing a mask conversion. */
	g = glyphs_format(nlist, list, glyphs);
	if (mask == g)
		return true;

	DBG(("%s: preferred mask format %08x, depth %d\n",
	     __FUNCTION__, g ? (unsigned)g->format : 0,  g ? g->depth : 0));

	/* Otherwise if the glyphs are all bitmaps and we have an
	 * opaque source we can also render directly to the dst.
	 */
	if (g == NULL) {
		while (nlist--) {
			if (list->format->depth != 1)
				return false;

			list++;
		}
	} else {
		if (PICT_FORMAT_A(mask->format) >= PICT_FORMAT_A(g->format))
			return true;

		if (g->depth != 1)
			return false;
	}

	if (!sna_picture_is_solid(src, &color))
		return false;

	return color >> 24 == 0xff;
}

static void
glyphs_fallback(CARD8 op,
		PicturePtr src,
		PicturePtr dst,
		PictFormatPtr mask_format,
		int src_x, int src_y,
		int nlist, GlyphListPtr list, GlyphPtr *glyphs)
{
	struct sna *sna = to_sna_from_drawable(dst->pDrawable);
	pixman_image_t *src_image, *dst_image;
	int src_dx, src_dy;
	ScreenPtr screen = dst->pDrawable->pScreen;
	RegionRec region;
	int x, y, n;

	glyph_extents(nlist, list, glyphs, &region.extents);
	DBG(("%s: nlist=%d, count=%d, extents (%d, %d), (%d, %d)\n", __FUNCTION__,
	     nlist, glyph_count(nlist, list),
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	if (region.extents.x2 <= region.extents.x1 ||
	    region.extents.y2 <= region.extents.y1)
		return;

	region.data = NULL;
	RegionTranslate(&region, dst->pDrawable->x, dst->pDrawable->y);
	RegionIntersect(&region, &region, dst->pCompositeClip);
	DBG(("%s: clipped extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     RegionExtents(&region)->x1, RegionExtents(&region)->y1,
	     RegionExtents(&region)->x2, RegionExtents(&region)->y2));
	if (RegionNil(&region))
		return;

	if (!sna_drawable_move_region_to_cpu(dst->pDrawable, &region,
					     MOVE_READ | MOVE_WRITE))
		return;
	if (dst->alphaMap &&
	    !sna_drawable_move_to_cpu(dst->alphaMap->pDrawable,
				      MOVE_READ | MOVE_WRITE))
		return;

	if (src->pDrawable) {
		if (!sna_drawable_move_to_cpu(src->pDrawable,
					      MOVE_READ))
			return;

		if (src->alphaMap &&
		    !sna_drawable_move_to_cpu(src->alphaMap->pDrawable,
					      MOVE_READ))
			return;
	}
	RegionTranslate(&region, -dst->pDrawable->x, -dst->pDrawable->y);

	if (mask_format &&
	    can_discard_mask(op, src, mask_format, nlist, list, glyphs)) {
		DBG(("%s: discarding mask\n", __FUNCTION__));
		mask_format = NULL;
	}

#if HAS_PIXMAN_GLYPHS
	if (__global_glyph_cache) {
		pixman_glyph_t stack_glyphs[N_STACK_GLYPHS];
		pixman_glyph_t *pglyphs = stack_glyphs;
		int dst_x = list->xOff, dst_y = list->yOff;
		int dst_dx, dst_dy, count;

		pixman_glyph_cache_freeze(__global_glyph_cache);

		count = 0;
		for (n = 0; n < nlist; ++n)
			count += list[n].len;
		if (count > N_STACK_GLYPHS) {
			pglyphs = malloc(count * sizeof(pixman_glyph_t));
			if (pglyphs == NULL)
				goto out;
		}

		count = 0;
		x = y = 0;
		while (nlist--) {
			n = list->len;
			x += list->xOff;
			y += list->yOff;
			while (n--) {
				GlyphPtr g = *glyphs++;
				const void *ptr;

				if (!glyph_valid(g))
					goto next;

				ptr = pixman_glyph_cache_lookup(__global_glyph_cache, g, NULL);
				if (ptr == NULL) {
					pixman_image_t *glyph_image;

					glyph_image = sna_glyph_get_image(g, screen);
					if (glyph_image == NULL)
						goto next;

					DBG(("%s: inserting glyph %p into pixman cache\n", __FUNCTION__, g));
					ptr = pixman_glyph_cache_insert(__global_glyph_cache, g, NULL,
									g->info.x,
									g->info.y,
									glyph_image);
					if (ptr == NULL)
						goto next;
				}

				assert(sna_glyph_get_image(g, screen) != NULL);

				pglyphs[count].x = x;
				pglyphs[count].y = y;
				pglyphs[count].glyph = ptr;
				count++;

next:
				x += g->info.xOff;
				y += g->info.yOff;
			}
			list++;
		}

		if (count == 0)
			goto out;

		src_image = image_from_pict(src, FALSE, &src_dx, &src_dy);
		if (src_image == NULL)
			goto out;

		dst_image = image_from_pict(dst, TRUE, &dst_dx, &dst_dy);
		if (dst_image == NULL)
			goto out_free_src;

		if (sigtrap_get() == 0) {
			if (mask_format) {
				pixman_composite_glyphs(op, src_image, dst_image,
							mask_format->format | (mask_format->depth << 24),
							src_x + src_dx + region.extents.x1 - dst_x,
							src_y + src_dy + region.extents.y1 - dst_y,
							region.extents.x1, region.extents.y1,
							region.extents.x1 + dst_dx, region.extents.y1 + dst_dy,
							region.extents.x2 - region.extents.x1,
							region.extents.y2 - region.extents.y1,
							__global_glyph_cache, count, pglyphs);
			} else {
				pixman_composite_glyphs_no_mask(op, src_image, dst_image,
								src_x + src_dx - dst_x, src_y + src_dy - dst_y,
								dst_dx, dst_dy,
								__global_glyph_cache, count, pglyphs);
			}
			sigtrap_put();
		}

		free_pixman_pict(dst, dst_image);

out_free_src:
		free_pixman_pict(src, src_image);

out:
		pixman_glyph_cache_thaw(__global_glyph_cache);
		if (pglyphs != stack_glyphs)
			free(pglyphs);
	} else
#endif
	{
		pixman_image_t *mask_image;

		dst_image = image_from_pict(dst, TRUE, &x, &y);
		if (dst_image == NULL)
			goto cleanup_region;
		DBG(("%s: dst offset (%d, %d)\n", __FUNCTION__, x, y));
		if (x | y) {
			region.extents.x1 += x;
			region.extents.x2 += x;
			region.extents.y1 += y;
			region.extents.y2 += y;
		}

		src_image = image_from_pict(src, FALSE, &src_dx, &src_dy);
		if (src_image == NULL)
			goto cleanup_dst;
		DBG(("%s: src offset (%d, %d)\n", __FUNCTION__, src_dx, src_dy));
		src_x += src_dx - list->xOff;
		src_y += src_dy - list->yOff;

		if (mask_format) {
			DBG(("%s: create mask (%d, %d)x(%d,%d) + (%d,%d) + (%d,%d), depth=%d, format=%lx [%lx], ca? %d\n",
			     __FUNCTION__,
			     region.extents.x1, region.extents.y1,
			     region.extents.x2 - region.extents.x1,
			     region.extents.y2 - region.extents.y1,
			     dst->pDrawable->x, dst->pDrawable->y,
			     x, y,
			     mask_format->depth,
			     (long)mask_format->format,
			     (long)(mask_format->depth << 24 | mask_format->format),
			     NeedsComponent(mask_format->format)));
			mask_image =
				pixman_image_create_bits(mask_format->depth << 24 | mask_format->format,
							 region.extents.x2 - region.extents.x1,
							 region.extents.y2 - region.extents.y1,
							 NULL, 0);
			if (mask_image == NULL)
				goto cleanup_src;
			if (NeedsComponent(mask_format->format))
				pixman_image_set_component_alpha(mask_image, TRUE);

			x -= region.extents.x1;
			y -= region.extents.y1;
		} else {
			mask_image = dst_image;
			src_x -= x - dst->pDrawable->x;
			src_y -= y - dst->pDrawable->y;
		}

		if (sigtrap_get() == 0) {
			do {
				n = list->len;
				x += list->xOff;
				y += list->yOff;
				while (n--) {
					GlyphPtr g = *glyphs++;
					pixman_image_t *glyph_image;

					if (!glyph_valid(g))
						goto next_glyph;

					glyph_image = sna_glyph_get_image(g, screen);
					if (glyph_image == NULL)
						goto next_glyph;

					if (mask_format) {
						DBG(("%s: glyph to mask (%d, %d)x(%d, %d)\n",
						     __FUNCTION__,
						     x - g->info.x,
						     y - g->info.y,
						     g->info.width,
						     g->info.height));

						if (list->format == mask_format) {
							assert(pixman_image_get_format(glyph_image) == pixman_image_get_format(mask_image));
							pixman_image_composite(PictOpAdd,
									       glyph_image,
									       NULL,
									       mask_image,
									       0, 0,
									       0, 0,
									       x - g->info.x,
									       y - g->info.y,
									       g->info.width,
									       g->info.height);
						} else {
							pixman_image_composite(PictOpAdd,
									       sna->render.white_image,
									       glyph_image,
									       mask_image,
									       0, 0,
									       0, 0,
									       x - g->info.x,
									       y - g->info.y,
									       g->info.width,
									       g->info.height);
						}
					} else {
						int xi = x - g->info.x;
						int yi = y - g->info.y;

						DBG(("%s: glyph to dst (%d, %d)x(%d, %d)/[(%d, %d)x(%d, %d)], src (%d, %d) [op=%d]\n",
						     __FUNCTION__,
						     xi, yi,
						     g->info.width, g->info.height,
						     dst->pDrawable->x,
						     dst->pDrawable->y,
						     dst->pDrawable->width,
						     dst->pDrawable->height,
						     src_x + xi,
						     src_y + yi,
						     op));

						pixman_image_composite(op,
								       src_image,
								       glyph_image,
								       dst_image,
								       src_x + xi,
								       src_y + yi,
								       0, 0,
								       xi, yi,
								       g->info.width,
								       g->info.height);
					}
next_glyph:
					x += g->info.xOff;
					y += g->info.yOff;
				}
				list++;
			} while (--nlist);
			sigtrap_put();
		}

		if (mask_format) {
			DBG(("%s: glyph mask composite src=(%d+%d,%d+%d) dst=(%d, %d)x(%d, %d)\n",
			     __FUNCTION__,
			     src_x, region.extents.x1, src_y, region.extents.y1,
			     region.extents.x1, region.extents.y1,
			     region.extents.x2 - region.extents.x1,
			     region.extents.y2 - region.extents.y1));
			pixman_image_composite(op, src_image, mask_image, dst_image,
					       src_x, src_y,
					       0, 0,
					       region.extents.x1, region.extents.y1,
					       region.extents.x2 - region.extents.x1,
					       region.extents.y2 - region.extents.y1);
			pixman_image_unref(mask_image);
		}

cleanup_src:
		free_pixman_pict(src, src_image);
cleanup_dst:
		free_pixman_pict(dst, dst_image);
	}

cleanup_region:
	RegionUninit(&region);
}

void
sna_glyphs(CARD8 op,
	   PicturePtr src,
	   PicturePtr dst,
	   PictFormatPtr mask,
	   INT16 src_x, INT16 src_y,
	   int nlist, GlyphListPtr list, GlyphPtr *glyphs)
{
	PixmapPtr pixmap = get_drawable_pixmap(dst->pDrawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;

	DBG(("%s(op=%d, nlist=%d, src=(%d, %d))\n",
	     __FUNCTION__, op, nlist, src_x, src_y));

	if (RegionNil(dst->pCompositeClip))
		return;

	if (FALLBACK)
		goto fallback;

	if (!can_render(sna)) {
		DBG(("%s: wedged\n", __FUNCTION__));
		goto fallback;
	}

	if (dst->alphaMap) {
		DBG(("%s: fallback -- dst alpha map\n", __FUNCTION__));
		goto fallback;
	}

	priv = sna_pixmap(pixmap);
	if (priv == NULL) {
		DBG(("%s: fallback -- destination unattached\n", __FUNCTION__));
		goto fallback;
	}

	if (!is_gpu_dst(priv) && !picture_is_gpu(sna, src, 0)) {
		DBG(("%s: fallback -- too small (%dx%d)\n",
		     __FUNCTION__, dst->pDrawable->width, dst->pDrawable->height));
		goto fallback;
	}

	/* Try to discard the mask for non-overlapping glyphs */
	if (FORCE_GLYPHS_TO_DST ||
	    mask == NULL ||
	    (dst->pCompositeClip->data == NULL &&
	     can_discard_mask(op, src, mask, nlist, list, glyphs))) {
		DBG(("%s: discarding mask\n", __FUNCTION__));
		if (can_use_glyph0()) {
			if (glyphs0_to_dst(sna, op,
					   src, dst,
					   src_x, src_y,
					   nlist, list, glyphs))
				return;
		} else {
			if (glyphs_to_dst(sna, op,
					  src, dst,
					  src_x, src_y,
					  nlist, list, glyphs))
				return;
		}
	}

	/* Otherwise see if we can substitute a mask */
	if (!mask) {
		mask = glyphs_format(nlist, list, glyphs);
		DBG(("%s: substituting mask? %d\n", __FUNCTION__, mask!=NULL));
	}
	if (mask) {
		if (glyphs_via_mask(sna, op,
				    src, dst, mask,
				    src_x, src_y,
				    nlist, list, glyphs))
			return;
	} else {
		if (glyphs_slow(sna, op,
				src, dst,
				src_x, src_y,
				nlist, list, glyphs))
			return;
	}

fallback:
	glyphs_fallback(op, src, dst, mask, src_x, src_y, nlist, list, glyphs);
}

static bool
glyphs_via_image(struct sna *sna,
		 CARD8 op,
		 PicturePtr src,
		 PicturePtr dst,
		 PictFormatPtr format,
		 INT16 src_x, INT16 src_y,
		 int nlist, GlyphListPtr list, GlyphPtr *glyphs)
{
	ScreenPtr screen = dst->pDrawable->pScreen;
	CARD32 component_alpha;
	PixmapPtr pixmap;
	PicturePtr mask;
	int16_t x, y, width, height;
	pixman_image_t *mask_image;
	int error;
	bool ret = false;
	BoxRec box;

	if (NO_GLYPHS_VIA_MASK)
		return false;

	DBG(("%s(op=%d, src=(%d, %d), nlist=%d,  dst=(%d, %d)+(%d, %d))\n",
	     __FUNCTION__, op, src_x, src_y, nlist,
	     list->xOff, list->yOff, dst->pDrawable->x, dst->pDrawable->y));

	glyph_extents(nlist, list, glyphs, &box);
	if (box.x2 <= box.x1 || box.y2 <= box.y1)
		return true;

	DBG(("%s: nlist=%d, count=%d, bounds=((%d, %d), (%d, %d))\n", __FUNCTION__,
	     nlist, glyph_count(nlist, list), box.x1, box.y1, box.x2, box.y2));

	if (!sna_compute_composite_extents(&box,
					   src, NULL, dst,
					   src_x, src_y,
					   0, 0,
					   box.x1, box.y1,
					   box.x2 - box.x1,
					   box.y2 - box.y1))
		return true;

	DBG(("%s: extents=((%d, %d), (%d, %d))\n", __FUNCTION__,
	     box.x1, box.y1, box.x2, box.y2));

	width  = box.x2 - box.x1;
	height = box.y2 - box.y1;
	box.x1 -= dst->pDrawable->x;
	box.y1 -= dst->pDrawable->y;
	x = -box.x1;
	y = -box.y1;
	src_x += box.x1 - list->xOff;
	src_y += box.y1 - list->yOff;

	if (format->depth < 8) {
		format = PictureMatchFormat(screen, 8, PICT_a8);
		if (!format)
			return false;
	}

	DBG(("%s: small mask [format=%lx, depth=%d, size=%d], rendering glyphs to upload buffer\n",
	     __FUNCTION__, (unsigned long)format->format,
	     format->depth, (uint32_t)width*height*format->depth));

	pixmap = sna_pixmap_create_upload(screen,
					  width, height,
					  format->depth,
					  KGEM_BUFFER_WRITE);
	if (!pixmap)
		return false;

	mask_image =
		pixman_image_create_bits(format->depth << 24 | format->format,
					 width, height,
					 pixmap->devPrivate.ptr,
					 pixmap->devKind);
	if (mask_image == NULL)
		goto err_pixmap;

	if (sigtrap_get()) {
		pixman_image_unref(mask_image);
		goto err_pixmap;
	}

	memset(pixmap->devPrivate.ptr, 0, pixmap->devKind*height);
#if HAS_PIXMAN_GLYPHS
	if (__global_glyph_cache) {
		pixman_glyph_t stack_glyphs[N_STACK_GLYPHS];
		pixman_glyph_t *pglyphs = stack_glyphs;
		int count, n;

		count = 0;
		for (n = 0; n < nlist; ++n)
			count += list[n].len;
		if (count > N_STACK_GLYPHS) {
			pglyphs = malloc(count * sizeof(pixman_glyph_t));
			if (pglyphs == NULL)
				goto err_pixmap;
		}

		pixman_glyph_cache_freeze(__global_glyph_cache);
		count = 0;
		do {
			n = list->len;
			x += list->xOff;
			y += list->yOff;
			while (n--) {
				GlyphPtr g = *glyphs++;
				const void *ptr;

				if (!glyph_valid(g))
					goto next_pglyph;

				ptr = pixman_glyph_cache_lookup(__global_glyph_cache, g, NULL);
				if (ptr == NULL) {
					pixman_image_t *glyph_image;

					glyph_image = sna_glyph_get_image(g, screen);
					if (glyph_image == NULL)
						goto next_pglyph;

					DBG(("%s: inserting glyph %p into pixman cache\n", __FUNCTION__, g));
					ptr = pixman_glyph_cache_insert(__global_glyph_cache, g, NULL,
									g->info.x,
									g->info.y,
									glyph_image);
					if (ptr == NULL)
						goto next_pglyph;
				}

				assert(sna_glyph_get_image(g, screen) != NULL);

				pglyphs[count].x = x;
				pglyphs[count].y = y;
				pglyphs[count].glyph = ptr;
				count++;

next_pglyph:
				x += g->info.xOff;
				y += g->info.yOff;
			}
			list++;
		} while (--nlist);

		pixman_composite_glyphs_no_mask(PIXMAN_OP_ADD,
						sna->render.white_image,
						mask_image,
						0, 0,
						0, 0,
						__global_glyph_cache, count, pglyphs);
		pixman_glyph_cache_thaw(__global_glyph_cache);
		if (pglyphs != stack_glyphs)
			free(pglyphs);
	} else
#endif
		do {
			int n = list->len;
			x += list->xOff;
			y += list->yOff;
			while (n--) {
				GlyphPtr g = *glyphs++;
				pixman_image_t *glyph_image;
				int16_t xi, yi;

				if (!glyph_valid(g))
					goto next_image;

				/* If the mask has been cropped, it is likely
				 * that some of the glyphs fall outside.
				 */
				xi = x - g->info.x;
				yi = y - g->info.y;
				if (xi >= width || yi >= height)
					goto next_image;
				if (xi + g->info.width  <= 0 ||
				    yi + g->info.height <= 0)
					goto next_image;

				glyph_image = sna_glyph_get_image(g, screen);
				if (glyph_image == NULL)
					goto next_image;

				DBG(("%s: glyph to mask (%d, %d)x(%d, %d)\n",
				     __FUNCTION__,
				     xi, yi,
				     g->info.width,
				     g->info.height));

				if (list->format == format) {
					assert(pixman_image_get_format(glyph_image) == pixman_image_get_format(mask_image));
					pixman_image_composite(PictOpAdd,
							       glyph_image,
							       NULL,
							       mask_image,
							       0, 0,
							       0, 0,
							       xi, yi,
							       g->info.width,
							       g->info.height);
				} else {
					pixman_image_composite(PictOpAdd,
							       sna->render.white_image,
							       glyph_image,
							       mask_image,
							       0, 0,
							       0, 0,
							       xi, yi,
							       g->info.width,
							       g->info.height);
				}

next_image:
				x += g->info.xOff;
				y += g->info.yOff;
			}
			list++;
		} while (--nlist);
	pixman_image_unref(mask_image);
	sigtrap_put();

	component_alpha = NeedsComponent(format->format);

	mask = CreatePicture(0, &pixmap->drawable,
			     format, CPComponentAlpha,
			     &component_alpha, serverClient, &error);
	if (!mask)
		goto err_pixmap;

	ValidatePicture(mask);

	sna_composite(op,
		      src, mask, dst,
		      src_x, src_y,
		      0, 0,
		      box.x1, box.y1,
		      width, height);
	FreePicture(mask, 0);
	ret = true;
err_pixmap:
	sna_pixmap_destroy(pixmap);
	return ret;
}

void
sna_glyphs__shared(CARD8 op,
		   PicturePtr src,
		   PicturePtr dst,
		   PictFormatPtr mask,
		   INT16 src_x, INT16 src_y,
		   int nlist, GlyphListPtr list, GlyphPtr *glyphs)
{
	PixmapPtr pixmap = get_drawable_pixmap(dst->pDrawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;

	DBG(("%s(op=%d, nlist=%d, src=(%d, %d))\n",
	     __FUNCTION__, op, nlist, src_x, src_y));

	if (RegionNil(dst->pCompositeClip))
		return;

	if (FALLBACK)
		goto fallback;

	if (!can_render(sna)) {
		DBG(("%s: wedged\n", __FUNCTION__));
		goto fallback;
	}

	if (dst->alphaMap) {
		DBG(("%s: fallback -- dst alpha map\n", __FUNCTION__));
		goto fallback;
	}

	priv = sna_pixmap(pixmap);
	if (priv == NULL) {
		DBG(("%s: fallback -- destination unattached\n", __FUNCTION__));
		goto fallback;
	}

	if (!is_gpu_dst(priv) && !picture_is_gpu(sna, src, 0)) {
		DBG(("%s: fallback -- too small (%dx%d)\n",
		     __FUNCTION__, dst->pDrawable->width, dst->pDrawable->height));
		goto fallback;
	}

	if (!mask) {
		mask = glyphs_format(nlist, list, glyphs);
		DBG(("%s: substituting mask? %d\n", __FUNCTION__, mask!=NULL));
	}
	if (mask) {
		if (glyphs_via_image(sna, op,
				     src, dst, mask,
				     src_x, src_y,
				     nlist, list, glyphs))
			return;
	}

fallback:
	glyphs_fallback(op, src, dst, mask, src_x, src_y, nlist, list, glyphs);
}

void
sna_glyph_unrealize(ScreenPtr screen, GlyphPtr glyph)
{
	struct sna_glyph *p = sna_glyph(glyph);

	DBG(("%s: screen=%d, glyph=%p (image?=%d, atlas?=%d)\n",
	     __FUNCTION__, screen->myNum, glyph, !!p->image,
	     p->atlas && p->atlas != GetGlyphPicture(glyph, screen)));

	if (p->image) {
#if HAS_PIXMAN_GLYPHS
		if (__global_glyph_cache) {
			DBG(("%s: removing glyph %p from pixman cache\n",
			     __FUNCTION__, glyph));
			pixman_glyph_cache_remove(__global_glyph_cache,
						  glyph, NULL);
		}
#endif
		pixman_image_unref(p->image);
		p->image = NULL;
	}

	if (p->atlas && p->atlas != GetGlyphPicture(glyph, screen)) {
		struct sna *sna = to_sna_from_screen(screen);
		struct sna_glyph_cache *cache = &sna->render.glyph[p->pos&1];
		DBG(("%s: releasing glyph pos %d from cache %d\n",
		     __FUNCTION__, p->pos >> 1, p->pos & 1));
		assert(cache->glyphs[p->pos >> 1] == p);
		cache->glyphs[p->pos >> 1] = NULL;
		p->atlas = NULL;
	}

#if HAS_PIXMAN_GLYPHS
	assert(__global_glyph_cache == NULL ||
	       pixman_glyph_cache_lookup(__global_glyph_cache, glyph, NULL) == NULL);
#endif
}
