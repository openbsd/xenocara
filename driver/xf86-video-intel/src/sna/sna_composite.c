/*
 * Copyright (c) 2011 Intel Corporation
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
 *    Chris Wilson <chris@chris-wilson.co.uk>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sna.h"
#include "sna_render.h"
#include "sna_render_inline.h"
#include "fb/fbpict.h"

#include <mipict.h>

#define NO_COMPOSITE 0
#define NO_COMPOSITE_RECTANGLES 0

#define BOUND(v)	(INT16) ((v) < MINSHORT ? MINSHORT : (v) > MAXSHORT ? MAXSHORT : (v))

bool sna_composite_create(struct sna *sna)
{
	xRenderColor color = { 0 };
	int error;

	sna->clear = CreateSolidPicture(0, &color, &error);
	return sna->clear != NULL;
}

void sna_composite_close(struct sna *sna)
{
	DBG(("%s\n", __FUNCTION__));

	if (sna->clear) {
		FreePicture(sna->clear, 0);
		sna->clear = NULL;
	}
}

static inline bool
region_is_singular(pixman_region16_t *region)
{
	return region->data == NULL;
}

static inline bool
region_is_empty(pixman_region16_t *region)
{
	return region->data && region->data->numRects == 0;
}

static inline pixman_bool_t
clip_to_dst(pixman_region16_t *region,
	    pixman_region16_t *clip,
	    int		dx,
	    int		dy)
{
	DBG(("%s: region: %dx[(%d, %d), (%d, %d)], clip: %dx[(%d, %d), (%d, %d)]\n",
	     __FUNCTION__,
	     pixman_region_n_rects(region),
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2,
	     pixman_region_n_rects(clip),
	     clip->extents.x1, clip->extents.y1,
	     clip->extents.x2, clip->extents.y2));

	if (region_is_singular(region) && region_is_singular(clip)) {
		pixman_box16_t *r = &region->extents;
		pixman_box16_t *c = &clip->extents;
		int v;

		if (r->x1 < (v = c->x1 + dx))
			r->x1 = BOUND(v);
		if (r->x2 > (v = c->x2 + dx))
			r->x2 = BOUND(v);
		if (r->y1 < (v = c->y1 + dy))
			r->y1 = BOUND(v);
		if (r->y2 > (v = c->y2 + dy))
			r->y2 = BOUND(v);

		if (r->x1 >= r->x2 || r->y1 >= r->y2) {
			pixman_region_init(region);
			return FALSE;
		}

		return true;
	} else if (region_is_empty(clip)) {
		return FALSE;
	} else {
		if (dx | dy)
			pixman_region_translate(region, -dx, -dy);
		if (!pixman_region_intersect(region, region, clip))
			return FALSE;
		if (dx | dy)
			pixman_region_translate(region, dx, dy);

		return !region_is_empty(region);
	}
}

static inline bool
clip_to_src(RegionPtr region, PicturePtr p, int dx, int	 dy)
{
	bool result;

	if (p->clientClipType == CT_NONE)
		return true;

	pixman_region_translate(p->clientClip,
				p->clipOrigin.x + dx,
				p->clipOrigin.y + dy);

	result = RegionIntersect(region, region, p->clientClip);

	pixman_region_translate(p->clientClip,
				-(p->clipOrigin.x + dx),
				-(p->clipOrigin.y + dy));

	return result && !region_is_empty(region);
}

bool
sna_compute_composite_region(RegionPtr region,
			     PicturePtr src, PicturePtr mask, PicturePtr dst,
			     INT16 src_x,  INT16 src_y,
			     INT16 mask_x, INT16 mask_y,
			     INT16 dst_x,  INT16 dst_y,
			     CARD16 width, CARD16 height)
{
	int v;

	DBG(("%s: dst=(%d, %d)x(%d, %d)\n",
	     __FUNCTION__,
	     dst_x, dst_y,
	     width, height));

	region->extents.x1 = dst_x < 0 ? 0 : dst_x;
	v = dst_x + width;
	if (v > dst->pDrawable->width)
		v = dst->pDrawable->width;
	region->extents.x2 = v;

	region->extents.y1 = dst_y < 0 ? 0 : dst_y;
	v = dst_y + height;
	if (v > dst->pDrawable->height)
		v = dst->pDrawable->height;
	region->extents.y2 = v;

	region->data = 0;

	DBG(("%s: initial clip against dst->pDrawable: (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2));

	if (region->extents.x1 >= region->extents.x2 ||
	    region->extents.y1 >= region->extents.y2)
		return false;

	region->extents.x1 += dst->pDrawable->x;
	region->extents.x2 += dst->pDrawable->x;
	region->extents.y1 += dst->pDrawable->y;
	region->extents.y2 += dst->pDrawable->y;

	dst_x += dst->pDrawable->x;
	dst_y += dst->pDrawable->y;

	/* clip against dst */
	if (!clip_to_dst(region, dst->pCompositeClip, 0, 0))
		return false;

	DBG(("%s: clip against dst->pCompositeClip: (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2));

	if (dst->alphaMap) {
		if (!clip_to_dst(region, dst->alphaMap->pCompositeClip,
				 -dst->alphaOrigin.x,
				 -dst->alphaOrigin.y)) {
			pixman_region_fini (region);
			return false;
		}
	}

	/* clip against src */
	if (src) {
		if (src->pDrawable) {
			src_x += src->pDrawable->x;
			src_y += src->pDrawable->y;
		}
		if (!clip_to_src(region, src, dst_x - src_x, dst_y - src_y)) {
			pixman_region_fini (region);
			return false;
		}
		DBG(("%s: clip against src (%dx%d clip=%d): (%d, %d), (%d, %d)\n",
		       __FUNCTION__,
		       src->pDrawable ? src->pDrawable->width : 0,
		       src->pDrawable ? src->pDrawable->height : 0,
		       src->clientClipType,
		       region->extents.x1, region->extents.y1,
		       region->extents.x2, region->extents.y2));

		if (src->alphaMap) {
			if (!clip_to_src(region, src->alphaMap,
					 dst_x - (src_x - src->alphaOrigin.x),
					 dst_y - (src_y - src->alphaOrigin.y))) {
				pixman_region_fini(region);
				return false;
			}
		}
	}

	/* clip against mask */
	if (mask) {
		if (mask->pDrawable) {
			mask_x += mask->pDrawable->x;
			mask_y += mask->pDrawable->y;
		}
		if (!clip_to_src(region, mask, dst_x - mask_x, dst_y - mask_y)) {
			pixman_region_fini(region);
			return false;
		}
		if (mask->alphaMap) {
			if (!clip_to_src(region, mask->alphaMap,
					 dst_x - (mask_x - mask->alphaOrigin.x),
					 dst_y - (mask_y - mask->alphaOrigin.y))) {
				pixman_region_fini(region);
				return false;
			}
		}

		DBG(("%s: clip against mask: (%d, %d), (%d, %d)\n",
		     __FUNCTION__,
		     region->extents.x1, region->extents.y1,
		     region->extents.x2, region->extents.y2));
	}

	return !region_is_empty(region);
}

static void
trim_extents(BoxPtr extents, const PicturePtr p, int dx, int dy)
{
	const BoxPtr box = REGION_EXTENTS(NULL, p->pCompositeClip);

	DBG(("%s: trim((%d, %d), (%d, %d)) against ((%d, %d), (%d, %d)) + (%d, %d)\n",
	     __FUNCTION__,
	     extents->x1, extents->y1, extents->x2, extents->y2,
	     box->x1, box->y1, box->x2, box->y2,
	     dx, dy));

	if (extents->x1 < box->x1 + dx)
		extents->x1 = box->x1 + dx;
	if (extents->x2 > box->x2 + dx)
		extents->x2 = box->x2 + dx;

	if (extents->y1 < box->y1 + dy)
		extents->y1 = box->y1 + dy;
	if (extents->y2 > box->y2 + dy)
		extents->y2 = box->y2 + dy;
}

static void
_trim_source_extents(BoxPtr extents, const PicturePtr p, int dx, int dy)
{
	if (p->clientClipType != CT_NONE)
		trim_extents(extents, p, dx, dy);
}

static void
trim_source_extents(BoxPtr extents, const PicturePtr p, int dx, int dy)
{
	if (p->pDrawable) {
		dx += p->pDrawable->x;
		dy += p->pDrawable->y;
	}
	_trim_source_extents(extents, p, dx, dy);
	if (p->alphaMap)
		_trim_source_extents(extents, p->alphaMap,
				     dx - p->alphaOrigin.x,
				     dy - p->alphaOrigin.y);

	DBG(("%s: -> (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2));
}

bool
sna_compute_composite_extents(BoxPtr extents,
			      PicturePtr src, PicturePtr mask, PicturePtr dst,
			      INT16 src_x,  INT16 src_y,
			      INT16 mask_x, INT16 mask_y,
			      INT16 dst_x,  INT16 dst_y,
			      CARD16 width, CARD16 height)
{
	int v;

	DBG(("%s: dst=(%d, %d)x(%d, %d)\n",
	     __FUNCTION__,
	     dst_x, dst_y,
	     width, height));

	extents->x1 = dst_x < 0 ? 0 : dst_x;
	v = dst_x + width;
	if (v > dst->pDrawable->width)
		v = dst->pDrawable->width;
	extents->x2 = v;

	extents->y1 = dst_y < 0 ? 0 : dst_y;
	v = dst_y + height;
	if (v > dst->pDrawable->height)
		v = dst->pDrawable->height;
	extents->y2 = v;

	DBG(("%s: initial clip against dst->pDrawable: (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2));

	if (extents->x1 >= extents->x2 || extents->y1 >= extents->y2)
		return false;

	extents->x1 += dst->pDrawable->x;
	extents->x2 += dst->pDrawable->x;
	extents->y1 += dst->pDrawable->y;
	extents->y2 += dst->pDrawable->y;

	if (extents->x1 < dst->pCompositeClip->extents.x1)
		extents->x1 = dst->pCompositeClip->extents.x1;
	if (extents->x2 > dst->pCompositeClip->extents.x2)
		extents->x2 = dst->pCompositeClip->extents.x2;

	if (extents->y1 < dst->pCompositeClip->extents.y1)
		extents->y1 = dst->pCompositeClip->extents.y1;
	if (extents->y2 > dst->pCompositeClip->extents.y2)
		extents->y2 = dst->pCompositeClip->extents.y2;

	DBG(("%s: initial clip against dst->pCompositeClip: (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2));

	if (extents->x1 >= extents->x2 || extents->y1 >= extents->y2)
		return false;

	dst_x += dst->pDrawable->x;
	dst_y += dst->pDrawable->y;

	/* clip against dst */
	trim_extents(extents, dst, 0, 0);
	if (dst->alphaMap)
		trim_extents(extents, dst->alphaMap,
			     -dst->alphaOrigin.x,
			     -dst->alphaOrigin.y);

	DBG(("%s: clip against dst: (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     extents->x1, extents->y1,
	     extents->x2, extents->y2));

	if (src)
		trim_source_extents(extents, src, dst_x - src_x, dst_y - src_y);
	if (mask)
		trim_source_extents(extents, mask,
				    dst_x - mask_x, dst_y - mask_y);

	if (extents->x1 >= extents->x2 || extents->y1 >= extents->y2)
		return false;

	if (region_is_singular(dst->pCompositeClip))
		return true;

	return pixman_region_contains_rectangle(dst->pCompositeClip,
						extents) != PIXMAN_REGION_OUT;
}

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

static void apply_damage(struct sna_composite_op *op, RegionPtr region)
{
	DBG(("%s: damage=%p, region=%d [(%d, %d), (%d, %d) + (%d, %d)]\n",
	     __FUNCTION__, op->damage, region_num_rects(region),
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2,
	     op->dst.x, op->dst.y));

	if (op->damage == NULL)
		return;

	if (op->dst.x | op->dst.y)
		RegionTranslate(region, op->dst.x, op->dst.y);

	assert_pixmap_contains_box(op->dst.pixmap, RegionExtents(region));
	if (region->data == NULL &&
	    region->extents.x2 - region->extents.x1 == op->dst.width &&
	    region->extents.y2 - region->extents.y1 == op->dst.height) {
		*op->damage = _sna_damage_all(*op->damage,
					      op->dst.width,
					      op->dst.height);
		op->damage = NULL;
	} else
		sna_damage_add(op->damage, region);
}

static inline bool use_cpu(PixmapPtr pixmap, struct sna_pixmap *priv,
			   CARD8 op, INT16 width, INT16 height)
{
	if (priv->cpu_bo && kgem_bo_is_busy(priv->cpu_bo))
		return false;

	if (DAMAGE_IS_ALL(priv->cpu_damage) &&
	    (op > PictOpSrc ||
	     width  < pixmap->drawable.width ||
	     height < pixmap->drawable.height))
		return true;

	if (priv->gpu_bo)
		return false;

	return (priv->create & KGEM_CAN_CREATE_GPU) == 0;
}

static void validate_source(PicturePtr picture)
{
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,10,99,901,0)
	miCompositeSourceValidate(picture);
#else
	miCompositeSourceValidate(picture,
				  0, 0,
				  picture->pDrawable ? picture->pDrawable->width : 0,
				  picture->pDrawable ? picture->pDrawable->height : 0);
#endif
}

void
sna_composite_fb(CARD8 op,
		 PicturePtr src,
		 PicturePtr mask,
		 PicturePtr dst,
		 RegionPtr region,
		 INT16 src_x, INT16 src_y,
		 INT16 msk_x, INT16 msk_y,
		 INT16 dst_x, INT16 dst_y,
		 CARD16 width, CARD16 height)
{
	pixman_image_t *src_image, *mask_image, *dest_image;
	int src_xoff, src_yoff;
	int msk_xoff, msk_yoff;
	int dst_xoff, dst_yoff;
	int16_t tx, ty;
	unsigned flags;

	DBG(("%s -- op=%d, fallback dst=(%d, %d)+(%d, %d), size=(%d, %d): region=((%d,%d), (%d, %d))\n",
	     __FUNCTION__, op,
	     dst_x, dst_y,
	     dst->pDrawable->x, dst->pDrawable->y,
	     width, height,
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2));

	if (src->pDrawable) {
		DBG(("%s: fallback -- move src to cpu\n", __FUNCTION__));
		if (!sna_drawable_move_to_cpu(src->pDrawable,
					      MOVE_READ))
			return;

		if (src->alphaMap &&
		    !sna_drawable_move_to_cpu(src->alphaMap->pDrawable,
					      MOVE_READ))
			return;
	}

	validate_source(src);

	if (mask) {
		if (mask->pDrawable) {
			DBG(("%s: fallback -- move mask to cpu\n", __FUNCTION__));
			if (!sna_drawable_move_to_cpu(mask->pDrawable,
						      MOVE_READ))
				return;

			if (mask->alphaMap &&
			    !sna_drawable_move_to_cpu(mask->alphaMap->pDrawable,
						      MOVE_READ))
				return;
		}

		validate_source(mask);
	}

	DBG(("%s: fallback -- move dst to cpu\n", __FUNCTION__));
	if (op <= PictOpSrc && !dst->alphaMap)
		flags = MOVE_WRITE | MOVE_INPLACE_HINT;
	else
		flags = MOVE_WRITE | MOVE_READ;
	if (!sna_drawable_move_region_to_cpu(dst->pDrawable, region, flags))
		return;
	if (dst->alphaMap &&
	    !sna_drawable_move_to_cpu(dst->alphaMap->pDrawable, flags))
		return;

	if (mask == NULL &&
	    src->pDrawable &&
	    dst->pDrawable->bitsPerPixel >= 8 &&
	    src->filter != PictFilterConvolution &&
	    (op == PictOpSrc || (op == PictOpOver && !PICT_FORMAT_A(src->format))) &&
	    (dst->format == src->format || dst->format == alphaless(src->format)) &&
	    sna_transform_is_imprecise_integer_translation(src->transform, src->filter,
							   dst->polyMode == PolyModePrecise,
							   &tx, &ty)) {
		PixmapPtr dst_pixmap = get_drawable_pixmap(dst->pDrawable);
		PixmapPtr src_pixmap = get_drawable_pixmap(src->pDrawable);
		int16_t sx = src_x + tx - (dst->pDrawable->x + dst_x);
		int16_t sy = src_y + ty - (dst->pDrawable->y + dst_y);

		assert(src->pDrawable->bitsPerPixel == dst->pDrawable->bitsPerPixel);
		assert(src_pixmap->drawable.bitsPerPixel == dst_pixmap->drawable.bitsPerPixel);

		if (region->extents.x1 + sx >= 0 &&
		    region->extents.y1 + sy >= 0 &&
		    region->extents.x2 + sx <= src->pDrawable->width &&
		    region->extents.y2 + sy <= src->pDrawable->height) {
			if (sigtrap_get() == 0) {
				const BoxRec *box = region_rects(region);
				int nbox = region_num_rects(region);

				sx += src->pDrawable->x;
				sy += src->pDrawable->y;
				if (get_drawable_deltas(src->pDrawable, src_pixmap, &tx, &ty))
					sx += tx, sy += ty;

				assert(region->extents.x1 + sx >= 0);
				assert(region->extents.x2 + sx <= src_pixmap->drawable.width);
				assert(region->extents.y1 + sy >= 0);
				assert(region->extents.y2 + sy <= src_pixmap->drawable.height);

				get_drawable_deltas(dst->pDrawable, dst_pixmap, &tx, &ty);

				assert(nbox);
				do {
					assert(box->x1 + sx >= 0);
					assert(box->x2 + sx <= src_pixmap->drawable.width);
					assert(box->y1 + sy >= 0);
					assert(box->y2 + sy <= src_pixmap->drawable.height);

					assert(box->x1 + tx >= 0);
					assert(box->x2 + tx <= dst_pixmap->drawable.width);
					assert(box->y1 + ty >= 0);
					assert(box->y2 + ty <= dst_pixmap->drawable.height);

					assert(box->x2 > box->x1 && box->y2 > box->y1);

					sigtrap_assert_active();
					memcpy_blt(src_pixmap->devPrivate.ptr,
						   dst_pixmap->devPrivate.ptr,
						   dst_pixmap->drawable.bitsPerPixel,
						   src_pixmap->devKind,
						   dst_pixmap->devKind,
						   box->x1 + sx, box->y1 + sy,
						   box->x1 + tx, box->y1 + ty,
						   box->x2 - box->x1, box->y2 - box->y1);
					box++;
				} while (--nbox);
				sigtrap_put();
			}

			return;
		}
	}

	src_image = image_from_pict(src, FALSE, &src_xoff, &src_yoff);
	mask_image = image_from_pict(mask, FALSE, &msk_xoff, &msk_yoff);
	dest_image = image_from_pict(dst, TRUE, &dst_xoff, &dst_yoff);

	if (src_image && dest_image && !(mask && !mask_image))
		sna_image_composite(op, src_image, mask_image, dest_image,
				    src_x + src_xoff, src_y + src_yoff,
				    msk_x + msk_xoff, msk_y + msk_yoff,
				    dst_x + dst_xoff, dst_y + dst_yoff,
				    width, height);

	free_pixman_pict(src, src_image);
	free_pixman_pict(mask, mask_image);
	free_pixman_pict(dst, dest_image);
}

void
sna_composite(CARD8 op,
	      PicturePtr src,
	      PicturePtr mask,
	      PicturePtr dst,
	      INT16 src_x,  INT16 src_y,
	      INT16 mask_x, INT16 mask_y,
	      INT16 dst_x,  INT16 dst_y,
	      CARD16 width, CARD16 height)
{
	PixmapPtr pixmap = get_drawable_pixmap(dst->pDrawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;
	struct sna_composite_op tmp;
	RegionRec region;
	int dx, dy;

	DBG(("%s(%d src=%ld+(%d, %d), mask=%ld+(%d, %d), dst=%ld+(%d, %d)+(%d, %d), size=(%d, %d)\n",
	     __FUNCTION__, op,
	     get_picture_id(src), src_x, src_y,
	     get_picture_id(mask), mask_x, mask_y,
	     get_picture_id(dst), dst_x, dst_y,
	     dst->pDrawable->x, dst->pDrawable->y,
	     width, height));

	if (region_is_empty(dst->pCompositeClip)) {
		DBG(("%s: empty clip, skipping\n", __FUNCTION__));
		return;
	}

	if (op == PictOpClear) {
		DBG(("%s: discarding source and mask for clear\n", __FUNCTION__));
		mask = NULL;
		if (sna->clear)
			src = sna->clear;
	}

	if (mask && sna_composite_mask_is_opaque(mask)) {
		DBG(("%s: removing opaque %smask\n",
		     __FUNCTION__,
		     mask->componentAlpha && PICT_FORMAT_RGB(mask->format) ? "CA " : ""));
		mask = NULL;
	}

	if (!sna_compute_composite_region(&region,
					  src, mask, dst,
					  src_x,  src_y,
					  mask_x, mask_y,
					  dst_x,  dst_y,
					  width,  height))
		return;

	if (NO_COMPOSITE)
		goto fallback;

	if (wedged(sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		goto fallback;
	}

	if (dst->alphaMap) {
		DBG(("%s: fallback due to unhandled alpha-map\n", __FUNCTION__));
		goto fallback;
	}

	priv = sna_pixmap(pixmap);
	if (priv == NULL) {
		DBG(("%s: fallback as destination pixmap=%ld is unattached\n",
		     __FUNCTION__, pixmap->drawable.serialNumber));
		goto fallback;
	}

	if (use_cpu(pixmap, priv, op, width, height) &&
	    !picture_is_gpu(sna, src, PREFER_GPU_RENDER) &&
	    !picture_is_gpu(sna, mask, PREFER_GPU_RENDER)) {
		DBG(("%s: fallback, dst pixmap=%ld is too small (or completely damaged)\n",
		     __FUNCTION__, pixmap->drawable.serialNumber));
		goto fallback;
	}

	dx = region.extents.x1 - (dst_x + dst->pDrawable->x);
	dy = region.extents.y1 - (dst_y + dst->pDrawable->y);

	DBG(("%s: composite region extents:+(%d, %d) -> (%d, %d), (%d, %d) + (%d, %d)\n",
	     __FUNCTION__,
	     dx, dy,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2,
	     get_drawable_dx(dst->pDrawable),
	     get_drawable_dy(dst->pDrawable)));

	if (op <= PictOpSrc && priv->cpu_damage) {
		int16_t x, y;

		if (get_drawable_deltas(dst->pDrawable, pixmap, &x, &y))
			pixman_region_translate(&region, x, y);

		sna_damage_subtract(&priv->cpu_damage, &region);
		if (priv->cpu_damage == NULL) {
			list_del(&priv->flush_list);
			priv->cpu = false;
		}

		if (x|y)
			pixman_region_translate(&region, -x, -y);
	}

	if (!sna->render.composite(sna,
				   op, src, mask, dst,
				   src_x + dx,  src_y + dy,
				   mask_x + dx, mask_y + dy,
				   region.extents.x1,
				   region.extents.y1,
				   region.extents.x2 - region.extents.x1,
				   region.extents.y2 - region.extents.y1,
				   region.data ? COMPOSITE_PARTIAL : 0,
				   memset(&tmp, 0, sizeof(tmp)))) {
		DBG(("%s: fallback due unhandled composite op\n", __FUNCTION__));
		goto fallback;
	}

	if (region.data == NULL)
		tmp.box(sna, &tmp, &region.extents);
	else
		tmp.boxes(sna, &tmp,
			  RegionBoxptr(&region),
			  region_num_rects(&region));
	apply_damage(&tmp, &region);
	tmp.done(sna, &tmp);

	goto out;

fallback:
	DBG(("%s: fallback -- fbComposite\n", __FUNCTION__));
	sna_composite_fb(op, src, mask, dst, &region,
			 src_x,  src_y,
			 mask_x, mask_y,
			 dst_x,  dst_y,
			 width,  height);
out:
	REGION_UNINIT(NULL, &region);
}

void
sna_composite_rectangles(CARD8		 op,
			 PicturePtr	 dst,
			 xRenderColor	*color,
			 int		 num_rects,
			 xRectangle	*rects)
{
	struct sna *sna = to_sna_from_drawable(dst->pDrawable);
	PixmapPtr pixmap;
	struct sna_pixmap *priv;
	struct kgem_bo *bo;
	struct sna_damage **damage;
	pixman_region16_t region;
	pixman_box16_t stack_boxes[64], *boxes = stack_boxes, *b;
	int16_t dst_x, dst_y;
	int i, num_boxes;
	unsigned hint;

	DBG(("%s(op=%d, %08x x %d [(%d, %d)x(%d, %d) ...])\n",
	     __FUNCTION__, op,
	     (color->alpha >> 8 << 24) |
	     (color->red   >> 8 << 16) |
	     (color->green >> 8 << 8) |
	     (color->blue  >> 8 << 0),
	     num_rects,
	     rects[0].x, rects[0].y, rects[0].width, rects[0].height));

	if (!num_rects)
		return;

	if (region_is_empty(dst->pCompositeClip)) {
		DBG(("%s: empty clip, skipping\n", __FUNCTION__));
		return;
	}

	if ((color->red|color->green|color->blue|color->alpha) <= 0x00ff) {
		switch (op) {
		case PictOpOver:
		case PictOpOutReverse:
		case PictOpAdd:
			return;
		case  PictOpInReverse:
		case  PictOpSrc:
			op = PictOpClear;
			break;
		case  PictOpAtopReverse:
			op = PictOpOut;
			break;
		case  PictOpXor:
			op = PictOpOverReverse;
			break;
		}
	}
	if (color->alpha <= 0x00ff) {
		switch (op) {
		case PictOpOver:
		case PictOpOutReverse:
			return;
		case  PictOpInReverse:
			op = PictOpClear;
			break;
		case  PictOpAtopReverse:
			op = PictOpOut;
			break;
		case  PictOpXor:
			op = PictOpOverReverse;
			break;
		}
	} else if (color->alpha >= 0xff00) {
		switch (op) {
		case PictOpOver:
			op = PictOpSrc;
			break;
		case PictOpInReverse:
			return;
		case PictOpOutReverse:
			op = PictOpClear;
			break;
		case  PictOpAtopReverse:
			op = PictOpOverReverse;
			break;
		case  PictOpXor:
			op = PictOpOut;
			break;
		}
	}

	/* Avoid reducing overlapping translucent rectangles */
	if (op == PictOpOver &&
	    num_rects == 1 &&
	    sna_drawable_is_clear(dst->pDrawable))
		op = PictOpSrc;

	DBG(("%s: converted to op %d\n", __FUNCTION__, op));

	if (num_rects > ARRAY_SIZE(stack_boxes)) {
		boxes = malloc(sizeof(pixman_box16_t) * num_rects);
		if (boxes == NULL)
			return;
	}

	for (i = num_boxes = 0; i < num_rects; i++) {
		boxes[num_boxes].x1 = rects[i].x + dst->pDrawable->x;
		if (boxes[num_boxes].x1 < dst->pCompositeClip->extents.x1)
			boxes[num_boxes].x1 = dst->pCompositeClip->extents.x1;

		boxes[num_boxes].y1 = rects[i].y + dst->pDrawable->y;
		if (boxes[num_boxes].y1 < dst->pCompositeClip->extents.y1)
			boxes[num_boxes].y1 = dst->pCompositeClip->extents.y1;

		boxes[num_boxes].x2 = bound(rects[i].x + dst->pDrawable->x, rects[i].width);
		if (boxes[num_boxes].x2 > dst->pCompositeClip->extents.x2)
			boxes[num_boxes].x2 = dst->pCompositeClip->extents.x2;

		boxes[num_boxes].y2 = bound(rects[i].y + dst->pDrawable->y, rects[i].height);
		if (boxes[num_boxes].y2 > dst->pCompositeClip->extents.y2)
			boxes[num_boxes].y2 = dst->pCompositeClip->extents.y2;

		DBG(("%s[%d] (%d, %d)x(%d, %d) -> (%d, %d), (%d, %d)\n",
		     __FUNCTION__, i,
		     rects[i].x, rects[i].y, rects[i].width, rects[i].height,
		     boxes[num_boxes].x1, boxes[num_boxes].y1, boxes[num_boxes].x2, boxes[num_boxes].y2));

		if (boxes[num_boxes].x2 > boxes[num_boxes].x1 &&
		    boxes[num_boxes].y2 > boxes[num_boxes].y1)
			num_boxes++;
	}

	if (num_boxes == 0)
		goto cleanup_boxes;

	if (!pixman_region_init_rects(&region, boxes, num_boxes))
		goto cleanup_boxes;

	DBG(("%s: nrects=%d, region=(%d, %d), (%d, %d) x %d\n",
	     __FUNCTION__, num_rects,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2,
	     num_boxes));

	if (dst->pCompositeClip->data &&
	    (!pixman_region_intersect(&region, &region, dst->pCompositeClip) ||
	     region_is_empty(&region))) {
		DBG(("%s: zero-intersection between rectangles and clip\n",
		     __FUNCTION__));
		goto cleanup_region;
	}

	DBG(("%s: clipped extents (%d, %d),(%d, %d) x %d\n",
	     __FUNCTION__,
	     RegionExtents(&region)->x1, RegionExtents(&region)->y1,
	     RegionExtents(&region)->x2, RegionExtents(&region)->y2,
	     region_num_rects(&region)));

	/* XXX xserver-1.8: CompositeRects is not tracked by Damage, so we must
	 * manually append the damaged regions ourselves.
	 *
	 * Note that DamageRegionAppend() will apply the drawable-deltas itself.
	 */
	DamageRegionAppend(dst->pDrawable, &region);

	pixmap = get_drawable_pixmap(dst->pDrawable);
	if (get_drawable_deltas(dst->pDrawable, pixmap, &dst_x, &dst_y))
		pixman_region_translate(&region, dst_x, dst_y);

	DBG(("%s: pixmap +(%d, %d) extents (%d, %d),(%d, %d)\n",
	     __FUNCTION__, dst_x, dst_y,
	     RegionExtents(&region)->x1, RegionExtents(&region)->y1,
	     RegionExtents(&region)->x2, RegionExtents(&region)->y2));
	assert_pixmap_contains_box(pixmap, RegionExtents(&region));

	if (NO_COMPOSITE_RECTANGLES)
		goto fallback;

	if (wedged(sna))
		goto fallback;

	if (dst->alphaMap) {
		DBG(("%s: fallback, dst has an alpha-map\n", __FUNCTION__));
		goto fallback;
	}

	priv = sna_pixmap(pixmap);
	if (priv == NULL || too_small(priv)) {
		DBG(("%s: fallback, dst pixmap=%ld too small or not attached\n",
		     __FUNCTION__, pixmap->drawable.serialNumber));
		goto fallback;
	}

	/* If we going to be overwriting any CPU damage with a subsequent
	 * operation, then we may as well delete it without moving it
	 * first to the GPU.
	 */
	hint = can_render(sna) ? PREFER_GPU : 0;
	if (op <= PictOpSrc) {
		if (priv->clear) {
			uint32_t pixel;
			bool ok;

			if (op == PictOpClear) {
				ok = sna_get_pixel_from_rgba(&pixel,
							     0, 0, 0, 0,
							     dst->format);
			} else {
				ok = sna_get_pixel_from_rgba(&pixel,
							     color->red,
							     color->green,
							     color->blue,
							     color->alpha,
							     dst->format);
			}
			if (ok && priv->clear_color == pixel)
				goto done;
		}

		if (region.data == NULL) {
			hint |= IGNORE_DAMAGE;
			if (region_subsumes_drawable(&region, &pixmap->drawable))
				hint |= REPLACES;
			if (priv->cpu_damage &&
			    (hint & REPLACES ||
			     region_subsumes_damage(&region, priv->cpu_damage))) {
				DBG(("%s: discarding existing CPU damage\n", __FUNCTION__));
				if (priv->gpu_bo && priv->gpu_bo->proxy) {
					assert(priv->gpu_damage == NULL);
					kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
					priv->gpu_bo = NULL;
				}
				sna_damage_destroy(&priv->cpu_damage);
				list_del(&priv->flush_list);
			}
			if (hint & REPLACES ||
			    box_inplace(pixmap, &region.extents)) {
				if (priv->gpu_bo && priv->cpu_damage == NULL) {
					DBG(("%s: promoting to full GPU\n", __FUNCTION__));
					assert(priv->gpu_bo->proxy == NULL);
					sna_damage_all(&priv->gpu_damage, pixmap);
				}
			}
		}
		if (priv->cpu_damage == NULL) {
			DBG(("%s: dropping last-cpu hint\n", __FUNCTION__));
			priv->cpu = false;
		}
	}

	bo = sna_drawable_use_bo(&pixmap->drawable, hint,
				 &region.extents, &damage);
	if (bo == NULL) {
		DBG(("%s: fallback due to no GPU bo\n", __FUNCTION__));
		goto fallback;
	}
	if (hint & REPLACES)
		kgem_bo_pair_undo(&sna->kgem, priv->gpu_bo, priv->cpu_bo);

	if (op <= PictOpSrc) {
		b = pixman_region_rectangles(&region, &num_boxes);
		if (!sna->render.fill_boxes(sna, op, dst->format, color,
					    &pixmap->drawable, bo, b, num_boxes)) {
			DBG(("%s: fallback - acceleration failed\n", __FUNCTION__));
			goto fallback;
		}
	} else if (dst->pCompositeClip->data == NULL) {
		for (i = 0; i < num_boxes; i++) {
			boxes[i].x1 += dst_x;
			boxes[i].x2 += dst_x;
			boxes[i].y1 += dst_y;
			boxes[i].y2 += dst_y;
		}
		if (!sna->render.fill_boxes(sna, op, dst->format, color,
					    &pixmap->drawable, bo, boxes, num_boxes)) {
			DBG(("%s: fallback - acceleration failed\n", __FUNCTION__));
			goto fallback;
		}
	} else {
		for (i = 0; i < num_boxes; i++) {
			RegionRec tmp = { boxes[i] };
			if (pixman_region_intersect(&tmp, &tmp, dst->pCompositeClip)) {
				int n = 0;

				b = pixman_region_rectangles(&tmp, &n);
				if (n) {
					if (dst_x | dst_y)
						pixman_region_translate(&tmp, dst_x, dst_y);

					n = !sna->render.fill_boxes(sna, op, dst->format, color,
								    &pixmap->drawable, bo, b, n);
				}

				pixman_region_fini(&tmp);

				if (n) {
					DBG(("%s: fallback - acceleration failed\n", __FUNCTION__));
					goto fallback;
				}
			}
		}
	}

	if (damage)
		sna_damage_add(damage, &region);

	/* Clearing a pixmap after creation is a common operation, so take
	 * advantage and reduce further damage operations.
	 */
	if (region_subsumes_drawable(&region, &pixmap->drawable)) {
		if (damage) {
			sna_damage_all(damage, pixmap);
			sna_damage_destroy(damage == &priv->gpu_damage ?
					   &priv->cpu_damage : &priv->gpu_damage);
		}

		if (op <= PictOpSrc && bo == priv->gpu_bo) {
			bool ok;

			assert(DAMAGE_IS_ALL(priv->gpu_damage));

			priv->clear_color = 0;
			ok = true;
			if (op == PictOpSrc)
				ok = sna_get_pixel_from_rgba(&priv->clear_color,
							     color->red,
							     color->green,
							     color->blue,
							     color->alpha,
							     dst->format);
			priv->clear = ok;
			DBG(("%s: pixmap=%ld marking clear [%08x]? %d\n",
			     __FUNCTION__, pixmap->drawable.serialNumber,
			     priv->clear_color, ok));
		}
	}
	goto done;

fallback:
	DBG(("%s: fallback\n", __FUNCTION__));
	if (op <= PictOpSrc)
		hint = MOVE_WRITE;
	else
		hint = MOVE_WRITE | MOVE_READ;
	if (!sna_drawable_move_region_to_cpu(&pixmap->drawable, &region, hint))
		goto done;

	if (dst->alphaMap &&
	    !sna_drawable_move_to_cpu(dst->alphaMap->pDrawable, hint))
		goto done;

	assert(pixmap->devPrivate.ptr);

	if (sigtrap_get() == 0) {
		if (op <= PictOpSrc) {
			int nbox = region_num_rects(&region);
			const BoxRec *box = region_rects(&region);
			uint32_t pixel;

			if (op == PictOpClear)
				pixel = 0;
			else if (!sna_get_pixel_from_rgba(&pixel,
							  color->red,
							  color->green,
							  color->blue,
							  color->alpha,
							  dst->format))
				goto fallback_composite;

			sigtrap_assert_active();
			if (pixel == 0 &&
			    box->x2 - box->x1 == pixmap->drawable.width &&
			    box->y2 - box->y1 == pixmap->drawable.height) {
				memset(pixmap->devPrivate.ptr, 0,
				       pixmap->devKind*pixmap->drawable.height);
			} else do {
				DBG(("%s: fallback fill: (%d, %d)x(%d, %d) %08x\n",
				     __FUNCTION__,
				     box->x1, box->y1,
				     box->x2 - box->x1,
				     box->y2 - box->y1,
				     pixel));

				pixman_fill(pixmap->devPrivate.ptr,
					    pixmap->devKind/sizeof(uint32_t),
					    pixmap->drawable.bitsPerPixel,
					    box->x1, box->y1,
					    box->x2 - box->x1,
					    box->y2 - box->y1,
					    pixel);
				box++;
			} while (--nbox);
		} else {
			PicturePtr src;
			int error;

fallback_composite:
			DBG(("%s: fallback -- fbComposite()\n", __FUNCTION__));
			src = CreateSolidPicture(0, color, &error);
			if (src) {
				do {
					fbComposite(op, src, NULL, dst,
						    0, 0,
						    0, 0,
						    rects->x, rects->y,
						    rects->width, rects->height);
					rects++;
				} while (--num_rects);
				FreePicture(src, 0);
			}
		}
		sigtrap_put();
	}

done:
	DamageRegionProcessPending(dst->pDrawable);

cleanup_region:
	pixman_region_fini(&region);
cleanup_boxes:
	if (boxes != stack_boxes)
		free(boxes);
}
