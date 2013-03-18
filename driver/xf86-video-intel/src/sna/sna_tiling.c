/*
 * Copyright © 2010 Intel Corporation
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
#include "fb/fbpict.h"

struct sna_tile_span {
	BoxRec box;
	float opacity;
};

struct sna_tile_state {
	int op;
	PicturePtr src, mask, dst;
	PixmapPtr dst_pixmap;
	uint32_t dst_format;
	int16_t src_x, src_y;
	int16_t mask_x, mask_y;
	int16_t dst_x, dst_y;
	int16_t width, height;
	unsigned flags;

	int rect_count;
	int rect_size;
	struct sna_composite_rectangles rects_embedded[16], *rects;
};

static void
sna_tiling_composite_add_rect(struct sna_tile_state *tile,
			      const struct sna_composite_rectangles *r)
{
	if (tile->rect_count == tile->rect_size) {
		struct sna_composite_rectangles *a;
		int newsize = tile->rect_size * 2;

		if (tile->rects == tile->rects_embedded) {
			a = malloc (sizeof(struct sna_composite_rectangles) * newsize);
			if (a == NULL)
				return;

			memcpy(a,
			       tile->rects_embedded,
			       sizeof(struct sna_composite_rectangles) * tile->rect_count);
		} else {
			a = realloc(tile->rects,
				    sizeof(struct sna_composite_rectangles) * newsize);
			if (a == NULL)
				return;
		}

		tile->rects = a;
		tile->rect_size = newsize;
	}

	tile->rects[tile->rect_count++] = *r;
}

fastcall static void
sna_tiling_composite_blt(struct sna *sna,
			 const struct sna_composite_op *op,
			 const struct sna_composite_rectangles *r)
{
	sna_tiling_composite_add_rect(op->priv, r);
	(void)sna;
}

fastcall static void
sna_tiling_composite_box(struct sna *sna,
			 const struct sna_composite_op *op,
			 const BoxRec *box)
{
	struct sna_composite_rectangles r;

	r.dst.x = box->x1;
	r.dst.y = box->y1;
	r.mask = r.src = r.dst;

	r.width  = box->x2 - box->x1;
	r.height = box->y2 - box->y1;

	sna_tiling_composite_add_rect(op->priv, &r);
	(void)sna;
}

static void
sna_tiling_composite_boxes(struct sna *sna,
			   const struct sna_composite_op *op,
			   const BoxRec *box, int nbox)
{
	while (nbox--) {
		struct sna_composite_rectangles r;

		r.dst.x = box->x1;
		r.dst.y = box->y1;
		r.mask = r.src = r.dst;

		r.width  = box->x2 - box->x1;
		r.height = box->y2 - box->y1;

		sna_tiling_composite_add_rect(op->priv, &r);
		box++;
	}
	(void)sna;
}

static void
sna_tiling_composite_done(struct sna *sna,
			  const struct sna_composite_op *op)
{
	struct sna_tile_state *tile = op->priv;
	struct sna_composite_op tmp;
	int x, y, n, step;

	/* Use a small step to accommodate enlargement through tile alignment */
	step = sna->render.max_3d_size;
	if (tile->dst_x & (8*512 / tile->dst->pDrawable->bitsPerPixel - 1) ||
	    tile->dst_y & 63)
		step /= 2;
	while (step * step * 4 > sna->kgem.max_copy_tile_size)
		step /= 2;

	DBG(("%s -- %dx%d, count=%d, step size=%d\n", __FUNCTION__,
	     tile->width, tile->height, tile->rect_count, step));

	if (tile->rect_count == 0)
		goto done;

	for (y = 0; y < tile->height; y += step) {
		int height = step;
		if (y + height > tile->height)
			height = tile->height - y;
		for (x = 0; x < tile->width; x += step) {
			int width = step;
			if (x + width > tile->width)
				width = tile->width - x;
			memset(&tmp, 0, sizeof(tmp));
			if (sna->render.composite(sna, tile->op,
						  tile->src, tile->mask, tile->dst,
						  tile->src_x + x,  tile->src_y + y,
						  tile->mask_x + x, tile->mask_y + y,
						  tile->dst_x + x,  tile->dst_y + y,
						  width, height,
						  &tmp)) {
				for (n = 0; n < tile->rect_count; n++) {
					const struct sna_composite_rectangles *r = &tile->rects[n];
					int x1, x2, dx, y1, y2, dy;

					x1 = r->dst.x - tile->dst_x, dx = 0;
					if (x1 < x)
						dx = x - x1, x1 = x;
					y1 = r->dst.y - tile->dst_y, dy = 0;
					if (y1 < y)
						dy = y - y1, y1 = y;

					x2 = r->dst.x + r->width - tile->dst_x;
					if (x2 > x + width)
						x2 = x + width;
					y2 = r->dst.y + r->height - tile->dst_y;
					if (y2 > y + height)
						y2 = y + height;

					DBG(("%s: rect[%d] = (%d, %d)x(%d,%d), tile=(%d,%d)x(%d, %d), blt=(%d,%d),(%d,%d), delta=(%d,%d)\n",
					     __FUNCTION__, n,
					     r->dst.x, r->dst.y,
					     r->width, r->height,
					     x, y, width, height,
					     x1, y1, x2, y2,
					     dx, dy));

					if (y2 > y1 && x2 > x1) {
						struct sna_composite_rectangles rr;
						rr.src.x = dx + r->src.x;
						rr.src.y = dy + r->src.y;

						rr.mask.x = dx + r->mask.x;
						rr.mask.y = dy + r->mask.y;

						rr.dst.x = dx + r->dst.x;
						rr.dst.y = dy + r->dst.y;

						rr.width  = x2 - x1;
						rr.height = y2 - y1;

						tmp.blt(sna, &tmp, &rr);
					}
				}
				tmp.done(sna, &tmp);
			} else {
				unsigned int flags;
				DBG(("%s -- falback\n", __FUNCTION__));

				if (tile->op <= PictOpSrc)
					flags = MOVE_WRITE;
				else
					flags = MOVE_WRITE | MOVE_READ;
				if (!sna_drawable_move_to_cpu(tile->dst->pDrawable,
							      flags))
					goto done;
				if (tile->dst->alphaMap &&
				    !sna_drawable_move_to_cpu(tile->dst->alphaMap->pDrawable,
							      flags))
					goto done;

				if (tile->src->pDrawable &&
				    !sna_drawable_move_to_cpu(tile->src->pDrawable,
							      MOVE_READ))
					goto done;
				if (tile->src->alphaMap &&
				    !sna_drawable_move_to_cpu(tile->src->alphaMap->pDrawable,
							      MOVE_READ))
					goto done;

				if (tile->mask && tile->mask->pDrawable &&
				    !sna_drawable_move_to_cpu(tile->mask->pDrawable,
							      MOVE_READ))
					goto done;

				if (tile->mask && tile->mask->alphaMap &&
				    !sna_drawable_move_to_cpu(tile->mask->alphaMap->pDrawable,
							      MOVE_READ))
					goto done;

				fbComposite(tile->op,
					    tile->src, tile->mask, tile->dst,
					    tile->src_x + x,  tile->src_y + y,
					    tile->mask_x + x, tile->mask_y + y,
					    tile->dst_x + x,  tile->dst_y + y,
					    width, height);
			}
		}
	}

done:
	if (tile->rects != tile->rects_embedded)
		free(tile->rects);
	free(tile);
}

bool
sna_tiling_composite(uint32_t op,
		     PicturePtr src,
		     PicturePtr mask,
		     PicturePtr dst,
		     int16_t src_x,  int16_t src_y,
		     int16_t mask_x, int16_t mask_y,
		     int16_t dst_x,  int16_t dst_y,
		     int16_t width,  int16_t height,
		     struct sna_composite_op *tmp)
{
	struct sna_tile_state *tile;
	struct sna_pixmap *priv;

	DBG(("%s size=(%d, %d), tile=%d\n",
	     __FUNCTION__, width, height,
	     to_sna_from_drawable(dst->pDrawable)->render.max_3d_size));

	priv = sna_pixmap(get_drawable_pixmap(dst->pDrawable));
	if (priv == NULL || priv->gpu_bo == NULL)
		return false;

	tile = malloc(sizeof(*tile));
	if (!tile)
		return false;

	tile->op = op;

	tile->src  = src;
	tile->mask = mask;
	tile->dst  = dst;

	tile->src_x = src_x;
	tile->src_y = src_y;
	tile->mask_x = mask_x;
	tile->mask_y = mask_y;
	tile->dst_x = dst_x;
	tile->dst_y = dst_y;
	tile->width = width;
	tile->height = height;
	tile->rects = tile->rects_embedded;
	tile->rect_count = 0;
	tile->rect_size = ARRAY_SIZE(tile->rects_embedded);

	tmp->blt   = sna_tiling_composite_blt;
	tmp->box   = sna_tiling_composite_box;
	tmp->boxes = sna_tiling_composite_boxes;
	tmp->done  = sna_tiling_composite_done;

	tmp->priv = tile;
	return true;
}

fastcall static void
sna_tiling_composite_spans_box(struct sna *sna,
			       const struct sna_composite_spans_op *op,
			       const BoxRec *box, float opacity)
{
	struct sna_tile_state *tile = op->base.priv;
	struct sna_tile_span *a;

	if (tile->rect_count == tile->rect_size) {
		int newsize = tile->rect_size * 2;

		if (tile->rects == tile->rects_embedded) {
			a = malloc (sizeof(struct sna_tile_span) * newsize);
			if (a == NULL)
				return;

			memcpy(a,
			       tile->rects_embedded,
			       sizeof(struct sna_tile_span) * tile->rect_count);
		} else {
			a = realloc(tile->rects,
				    sizeof(struct sna_tile_span) * newsize);
			if (a == NULL)
				return;
		}

		tile->rects = (void *)a;
		tile->rect_size = newsize;
	} else
		a = (void *)tile->rects;

	a[tile->rect_count].box = *box;
	a[tile->rect_count].opacity = opacity;
	tile->rect_count++;
	(void)sna;
}

static void
sna_tiling_composite_spans_boxes(struct sna *sna,
				 const struct sna_composite_spans_op *op,
				 const BoxRec *box, int nbox, float opacity)
{
	while (nbox--)
		sna_tiling_composite_spans_box(sna, op->base.priv, box++, opacity);
	(void)sna;
}

fastcall static void
sna_tiling_composite_spans_done(struct sna *sna,
				const struct sna_composite_spans_op *op)
{
	struct sna_tile_state *tile = op->base.priv;
	struct sna_composite_spans_op tmp;
	int x, y, n, step;
	bool force_fallback = false;

	/* Use a small step to accommodate enlargement through tile alignment */
	step = sna->render.max_3d_size;
	if (tile->dst_x & (8*512 / tile->dst->pDrawable->bitsPerPixel - 1) ||
	    tile->dst_y & 63)
		step /= 2;
	while (step * step * 4 > sna->kgem.max_copy_tile_size)
		step /= 2;

	DBG(("%s -- %dx%d, count=%d, step size=%d\n", __FUNCTION__,
	     tile->width, tile->height, tile->rect_count, step));

	if (tile->rect_count == 0)
		goto done;

	for (y = 0; y < tile->height; y += step) {
		int height = step;
		if (y + height > tile->height)
			height = tile->height - y;
		for (x = 0; x < tile->width; x += step) {
			const struct sna_tile_span *r = (void *)tile->rects;
			int width = step;
			if (x + width > tile->width)
				width = tile->width - x;
			if (!force_fallback &&
			    sna->render.composite_spans(sna, tile->op,
							tile->src, tile->dst,
							tile->src_x + x,  tile->src_y + y,
							tile->dst_x + x,  tile->dst_y + y,
							width, height, tile->flags,
							memset(&tmp, 0, sizeof(tmp)))) {
				for (n = 0; n < tile->rect_count; n++) {
					BoxRec b;

					b.x1 = r->box.x1 - tile->dst_x;
					if (b.x1 < x)
						b.x1 = x;

					b.y1 = r->box.y1 - tile->dst_y;
					if (b.y1 < y)
						b.y1 = y;

					b.x2 = r->box.x2 - tile->dst_x;
					if (b.x2 > x + width)
						b.x2 = x + width;

					b.y2 = r->box.y2 - tile->dst_y;
					if (b.y2 > y + height)
						b.y2 = y + height;

					DBG(("%s: rect[%d] = (%d, %d)x(%d,%d), tile=(%d,%d)x(%d, %d), blt=(%d,%d),(%d,%d)\n",
					     __FUNCTION__, n,
					     r->box.x1, r->box.y1,
					     r->box.x2-r->box.x1, r->box.y2-r->box.y1,
					     x, y, width, height,
					     b.x1, b.y1, b.x2, b.y2));

					if (b.y2 > b.y1 && b.x2 > b.x1)
						tmp.box(sna, &tmp, &b, r->opacity);
					r++;
				}
				tmp.done(sna, &tmp);
			} else {
				unsigned int flags;

				DBG(("%s -- falback\n", __FUNCTION__));

				if (tile->op <= PictOpSrc)
					flags = MOVE_WRITE;
				else
					flags = MOVE_WRITE | MOVE_READ;
				if (!sna_drawable_move_to_cpu(tile->dst->pDrawable,
							      flags))
					goto done;
				if (tile->dst->alphaMap &&
				    !sna_drawable_move_to_cpu(tile->dst->alphaMap->pDrawable,
							      flags))
					goto done;

				if (tile->src->pDrawable &&
				    !sna_drawable_move_to_cpu(tile->src->pDrawable,
							      MOVE_READ))
					goto done;
				if (tile->src->alphaMap &&
				    !sna_drawable_move_to_cpu(tile->src->alphaMap->pDrawable,
							      MOVE_READ))
					goto done;

				for (n = 0; n < tile->rect_count; n++) {
					BoxRec b;

					b.x1 = r->box.x1 - tile->dst_x;
					if (b.x1 < x)
						b.x1 = x;

					b.y1 = r->box.y1 - tile->dst_y;
					if (b.y1 < y)
						b.y1 = y;

					b.x2 = r->box.x2 - tile->dst_x;
					if (b.x2 > x + width)
						b.x2 = x + width;

					b.y2 = r->box.y2 - tile->dst_y;
					if (b.y2 > y + height)
						b.y2 = y + height;

					DBG(("%s: rect[%d] = (%d, %d)x(%d,%d), tile=(%d,%d)x(%d, %d), blt=(%d,%d),(%d,%d)\n",
					     __FUNCTION__, n,
					     r->box.x1, r->box.y1,
					     r->box.x2-r->box.x1, r->box.y2-r->box.y1,
					     x, y, width, height,
					     b.x1, b.y1, b.x2, b.y2));

					if (b.y2 > b.y1 && b.x2 > b.x1) {
						xRenderColor alpha;
						PicturePtr mask;
						int error;

						alpha.red = alpha.green = alpha.blue = 0;
						alpha.alpha = r->opacity * 0xffff;

						mask = CreateSolidPicture(0, &alpha, &error);
						if (!mask)
							goto done;

						fbComposite(tile->op,
							    tile->src, mask, tile->dst,
							    tile->src_x + x,  tile->src_y + y,
							    0, 0,
							    tile->dst_x + x,  tile->dst_y + y,
							    width, height);

						FreePicture(mask, 0);
					}
					r++;
				}

				force_fallback = true;
			}
		}
	}

done:
	if (tile->rects != tile->rects_embedded)
		free(tile->rects);
	free(tile);
}

bool
sna_tiling_composite_spans(uint32_t op,
			   PicturePtr src,
			   PicturePtr dst,
			   int16_t src_x,  int16_t src_y,
			   int16_t dst_x,  int16_t dst_y,
			   int16_t width,  int16_t height,
			   unsigned flags,
			   struct sna_composite_spans_op *tmp)
{
	struct sna_tile_state *tile;
	struct sna_pixmap *priv;

	DBG(("%s size=(%d, %d), tile=%d\n",
	     __FUNCTION__, width, height,
	     to_sna_from_drawable(dst->pDrawable)->render.max_3d_size));

	priv = sna_pixmap(get_drawable_pixmap(dst->pDrawable));
	if (priv == NULL || priv->gpu_bo == NULL)
		return false;

	tile = malloc(sizeof(*tile));
	if (!tile)
		return false;

	tile->op = op;
	tile->flags = flags;

	tile->src  = src;
	tile->mask = NULL;
	tile->dst  = dst;

	tile->src_x = src_x;
	tile->src_y = src_y;
	tile->mask_x = 0;
	tile->mask_y = 0;
	tile->dst_x = dst_x;
	tile->dst_y = dst_y;
	tile->width = width;
	tile->height = height;
	tile->rects = tile->rects_embedded;
	tile->rect_count = 0;
	tile->rect_size = ARRAY_SIZE(tile->rects_embedded);

	tmp->box   = sna_tiling_composite_spans_box;
	tmp->boxes = sna_tiling_composite_spans_boxes;
	tmp->done  = sna_tiling_composite_spans_done;

	tmp->base.priv = tile;
	return true;
}

bool
sna_tiling_fill_boxes(struct sna *sna,
		      CARD8 op,
		      PictFormat format,
		      const xRenderColor *color,
		      PixmapPtr dst, struct kgem_bo *dst_bo,
		      const BoxRec *box, int n)
{
	RegionRec region, tile, this;
	struct kgem_bo *bo;
	int step;
	bool ret = false;

	pixman_region_init_rects(&region, box, n);

	/* Use a small step to accommodate enlargement through tile alignment */
	step = sna->render.max_3d_size;
	if (region.extents.x1 & (8*512 / dst->drawable.bitsPerPixel - 1) ||
	    region.extents.y1 & 63)
		step /= 2;
	while (step * step * 4 > sna->kgem.max_copy_tile_size)
		step /= 2;

	DBG(("%s (op=%d, format=%x, color=(%04x,%04x,%04x, %04x), tile.size=%d, box=%dx[(%d, %d), (%d, %d)])\n",
	     __FUNCTION__, op, (int)format,
	     color->red, color->green, color->blue, color->alpha,
	     step, n,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	for (tile.extents.y1 = tile.extents.y2 = region.extents.y1;
	     tile.extents.y2 < region.extents.y2;
	     tile.extents.y1 = tile.extents.y2) {
		tile.extents.y2 = tile.extents.y1 + step;
		if (tile.extents.y2 > region.extents.y2)
			tile.extents.y2 = region.extents.y2;

		for (tile.extents.x1 = tile.extents.x2 = region.extents.x1;
		     tile.extents.x2 < region.extents.x2;
		     tile.extents.x1 = tile.extents.x2) {
			PixmapRec tmp;

			tile.extents.x2 = tile.extents.x1 + step;
			if (tile.extents.x2 > region.extents.x2)
				tile.extents.x2 = region.extents.x2;

			tile.data = NULL;

			RegionNull(&this);
			RegionIntersect(&this, &region, &tile);
			if (RegionNil(&this))
				continue;

			tmp.drawable.width  = this.extents.x2 - this.extents.x1;
			tmp.drawable.height = this.extents.y2 - this.extents.y1;
			tmp.drawable.depth  = dst->drawable.depth;
			tmp.drawable.bitsPerPixel = dst->drawable.bitsPerPixel;
			tmp.devPrivate.ptr = NULL;

			bo = kgem_create_2d(&sna->kgem,
					    tmp.drawable.width,
					    tmp.drawable.height,
					    dst->drawable.bitsPerPixel,
					    kgem_choose_tiling(&sna->kgem,
							       I915_TILING_X,
							       tmp.drawable.width,
							       tmp.drawable.height,
							       dst->drawable.bitsPerPixel),
					    CREATE_TEMPORARY);
			if (bo) {
				int16_t dx = this.extents.x1;
				int16_t dy = this.extents.y1;

				assert(kgem_bo_can_blt(&sna->kgem, bo));

				if (!sna->render.copy_boxes(sna, GXcopy,
							     dst, dst_bo, 0, 0,
							     &tmp, bo, -dx, -dy,
							     REGION_RECTS(&this), REGION_NUM_RECTS(&this), 0))
					goto err;

				RegionTranslate(&this, -dx, -dy);
				if (!sna->render.fill_boxes(sna, op, format, color,
							     &tmp, bo,
							     REGION_RECTS(&this), REGION_NUM_RECTS(&this)))
					goto err;

				if (!sna->render.copy_boxes(sna, GXcopy,
							     &tmp, bo, 0, 0,
							     dst, dst_bo, dx, dy,
							     REGION_RECTS(&this), REGION_NUM_RECTS(&this), 0))
					goto err;

				kgem_bo_destroy(&sna->kgem, bo);
			}
			RegionUninit(&this);
		}
	}

	ret = true;
	goto done;
err:
	kgem_bo_destroy(&sna->kgem, bo);
	RegionUninit(&this);
done:
	pixman_region_fini(&region);
	return ret;
}

bool sna_tiling_blt_copy_boxes(struct sna *sna, uint8_t alu,
			       struct kgem_bo *src_bo, int16_t src_dx, int16_t src_dy,
			       struct kgem_bo *dst_bo, int16_t dst_dx, int16_t dst_dy,
			       int bpp, const BoxRec *box, int nbox)
{
	RegionRec region, tile, this;
	struct kgem_bo *bo;
	int step;
	bool ret = false;

	if (!kgem_bo_can_blt(&sna->kgem, src_bo) ||
	    !kgem_bo_can_blt(&sna->kgem, dst_bo)) {
		/* XXX */
		DBG(("%s: tiling blt fail: src?=%d, dst?=%d\n",
		     __FUNCTION__,
		     kgem_bo_can_blt(&sna->kgem, src_bo),
		     kgem_bo_can_blt(&sna->kgem, dst_bo)));
		return false;
	}

	pixman_region_init_rects(&region, box, nbox);

	/* Use a small step to accommodate enlargement through tile alignment */
	step = sna->render.max_3d_size;
	if (region.extents.x1 & (8*512 / bpp - 1) || region.extents.y1 & 63)
		step /= 2;
	while (step * step * 4 > sna->kgem.max_copy_tile_size)
		step /= 2;

	DBG(("%s (alu=%d), tile.size=%d, box=%dx[(%d, %d), (%d, %d)])\n",
	     __FUNCTION__, alu, step, nbox,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	for (tile.extents.y1 = tile.extents.y2 = region.extents.y1;
	     tile.extents.y2 < region.extents.y2;
	     tile.extents.y1 = tile.extents.y2) {
		tile.extents.y2 = tile.extents.y1 + step;
		if (tile.extents.y2 > region.extents.y2)
			tile.extents.y2 = region.extents.y2;

		for (tile.extents.x1 = tile.extents.x2 = region.extents.x1;
		     tile.extents.x2 < region.extents.x2;
		     tile.extents.x1 = tile.extents.x2) {
			int w, h;

			tile.extents.x2 = tile.extents.x1 + step;
			if (tile.extents.x2 > region.extents.x2)
				tile.extents.x2 = region.extents.x2;

			tile.data = NULL;

			RegionNull(&this);
			RegionIntersect(&this, &region, &tile);
			if (RegionNil(&this))
				continue;

			w = this.extents.x2 - this.extents.x1;
			h = this.extents.y2 - this.extents.y1;
			bo = kgem_create_2d(&sna->kgem, w, h, bpp,
					    kgem_choose_tiling(&sna->kgem,
							       I915_TILING_X,
							       w, h, bpp),
					    CREATE_TEMPORARY);
			if (bo) {
				int16_t dx = this.extents.x1;
				int16_t dy = this.extents.y1;

				assert(bo->pitch <= 8192);
				assert(bo->tiling != I915_TILING_Y);

				if (!sna_blt_copy_boxes(sna, alu,
							src_bo, src_dx, src_dy,
							bo, -dx, -dy,
							bpp, REGION_RECTS(&this), REGION_NUM_RECTS(&this)))
					goto err;

				if (!sna_blt_copy_boxes(sna, alu,
							bo, -dx, -dy,
							dst_bo, dst_dx, dst_dy,
							bpp, REGION_RECTS(&this), REGION_NUM_RECTS(&this)))
					goto err;

				kgem_bo_destroy(&sna->kgem, bo);
			}
			RegionUninit(&this);
		}
	}

	ret = true;
	goto done;
err:
	kgem_bo_destroy(&sna->kgem, bo);
	RegionUninit(&this);
done:
	pixman_region_fini(&region);
	return ret;
}

static bool
box_intersect(BoxPtr a, const BoxRec *b)
{
	if (a->x1 < b->x1)
		a->x1 = b->x1;
	if (a->x2 > b->x2)
		a->x2 = b->x2;
	if (a->y1 < b->y1)
		a->y1 = b->y1;
	if (a->y2 > b->y2)
		a->y2 = b->y2;

	return a->x1 < a->x2 && a->y1 < a->y2;
}

bool
sna_tiling_copy_boxes(struct sna *sna, uint8_t alu,
		      PixmapPtr src, struct kgem_bo *src_bo, int16_t src_dx, int16_t src_dy,
		      PixmapPtr dst, struct kgem_bo *dst_bo, int16_t dst_dx, int16_t dst_dy,
		      const BoxRec *box, int n)
{
	BoxRec extents, tile, stack[64], *clipped, *c;
	PixmapRec p;
	int i, step, tiling;
	bool ret = false;

	extents = box[0];
	for (i = 1; i < n; i++) {
		if (extents.x1 < box[i].x1)
			extents.x1 = box[i].x1;
		if (extents.y1 < box[i].y1)
			extents.y1 = box[i].y1;

		if (extents.x2 > box[i].x2)
			extents.x2 = box[i].x2;
		if (extents.y2 > box[i].y2)
			extents.y2 = box[i].y2;
	}

	step = sna->render.max_3d_size - 4096 / dst->drawable.bitsPerPixel;
	while (step * step * 4 > sna->kgem.max_upload_tile_size)
		step /= 2;

	tiling = I915_TILING_X;
	if (!kgem_bo_can_blt(&sna->kgem, src_bo) ||
	    !kgem_bo_can_blt(&sna->kgem, dst_bo))
		tiling = I915_TILING_Y;

	DBG(("%s: tiling copy, using %dx%d %c tiles\n",
	     __FUNCTION__, step, step, tiling == I915_TILING_X ? 'X' : 'Y'));

	if (n > ARRAY_SIZE(stack)) {
		clipped = malloc(sizeof(BoxRec) * n);
		if (clipped == NULL)
			goto tiled_error;
	} else
		clipped = stack;

	p.drawable.depth = src->drawable.depth;
	p.drawable.bitsPerPixel = src->drawable.bitsPerPixel;
	p.devPrivate.ptr = NULL;

	for (tile.y1 = extents.y1; tile.y1 < extents.y2; tile.y1 = tile.y2) {
		tile.y2 = tile.y1 + step;
		if (tile.y2 > extents.y2)
			tile.y2 = extents.y2;

		for (tile.x1 = extents.x1; tile.x1 < extents.x2; tile.x1 = tile.x2) {
			struct kgem_bo *tmp_bo;

			tile.x2 = tile.x1 + step;
			if (tile.x2 > extents.x2)
				tile.x2 = extents.x2;

			c = clipped;
			for (i = 0; i < n; i++) {
				*c = box[i];
				if (!box_intersect(c, &tile))
					continue;

				DBG(("%s: box(%d, %d), (%d, %d), src=(%d, %d), dst=(%d, %d)\n",
				     __FUNCTION__,
				     c->x1, c->y1,
				     c->x2, c->y2,
				     src_dx, src_dy,
				     c->x1 - tile.x1,
				     c->y1 - tile.y1));
				c++;
			}
			if (c == clipped)
				continue;

			p.drawable.width  = tile.x2 - tile.x1;
			p.drawable.height = tile.y2 - tile.y1;

			DBG(("%s: tile (%d, %d), (%d, %d)\n",
			     __FUNCTION__, tile.x1, tile.y1, tile.x2, tile.y2));

			tmp_bo = kgem_create_2d(&sna->kgem,
						p.drawable.width,
						p.drawable.height,
						p.drawable.bitsPerPixel,
						tiling, CREATE_TEMPORARY);
			if (!tmp_bo)
				goto tiled_error;

			i = (sna->render.copy_boxes(sna, GXcopy,
						    src, src_bo, src_dx, src_dy,
						    &p, tmp_bo, -tile.x1, -tile.y1,
						    clipped, c - clipped, 0) &&
			     sna->render.copy_boxes(sna, alu,
						    &p, tmp_bo, -tile.x1, -tile.y1,
						    dst, dst_bo, dst_dx, dst_dy,
						    clipped, c - clipped, 0));

			kgem_bo_destroy(&sna->kgem, tmp_bo);

			if (!i)
				goto tiled_error;
		}
	}

	ret = true;
tiled_error:
	if (clipped != stack)
		free(clipped);

	return ret;
}
