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
	int x, y, n, step, max_size;

	/* Use a small step to accommodate enlargement through tile alignment */
	step = sna->render.max_3d_size;
	if (tile->dst_x & (8*512 / tile->dst->pDrawable->bitsPerPixel - 1) ||
	    tile->dst_y & 63)
		step /= 2;

	max_size = sna_max_tile_copy_size(sna, op->dst.bo, op->dst.bo);
	if (max_size == 0)
		goto done;

	while (step * step * 4 > max_size)
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
			if (sna->render.composite(sna, tile->op,
						  tile->src, tile->mask, tile->dst,
						  tile->src_x + x,  tile->src_y + y,
						  tile->mask_x + x, tile->mask_y + y,
						  tile->dst_x + x,  tile->dst_y + y,
						  width, height,
						  COMPOSITE_PARTIAL, memset(&tmp, 0, sizeof(tmp)))) {
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

				if (sigtrap_get() == 0) {
					fbComposite(tile->op,
						    tile->src, tile->mask, tile->dst,
						    tile->src_x + x,  tile->src_y + y,
						    tile->mask_x + x, tile->mask_y + y,
						    tile->dst_x + x,  tile->dst_y + y,
						    width, height);
					sigtrap_put();
				}
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
	tmp->dst.bo = priv->gpu_bo;
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
	int x, y, n, step, max_size;
	bool force_fallback = false;

	/* Use a small step to accommodate enlargement through tile alignment */
	step = sna->render.max_3d_size;
	if (tile->dst_x & (8*512 / tile->dst->pDrawable->bitsPerPixel - 1) ||
	    tile->dst_y & 63)
		step /= 2;

	max_size = sna_max_tile_copy_size(sna, op->base.dst.bo, op->base.dst.bo);
	if (max_size == 0)
		goto done;

	while (step * step * 4 > max_size)
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

						if (sigtrap_get() == 0) {
							fbComposite(tile->op,
								    tile->src, mask, tile->dst,
								    tile->src_x + x,  tile->src_y + y,
								    0, 0,
								    tile->dst_x + x,  tile->dst_y + y,
								    width, height);
							sigtrap_put();
						}

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
	tmp->base.dst.bo = priv->gpu_bo;
	return true;
}

bool
sna_tiling_fill_boxes(struct sna *sna,
		      CARD8 op,
		      PictFormat format,
		      const xRenderColor *color,
		      const DrawableRec *dst, struct kgem_bo *dst_bo,
		      const BoxRec *box, int n)
{
	RegionRec region, tile, this;
	struct kgem_bo *bo;
	int step, max_size;
	bool ret = false;

	pixman_region_init_rects(&region, box, n);

	/* Use a small step to accommodate enlargement through tile alignment */
	step = sna->render.max_3d_size;
	if (region.extents.x1 & (8*512 / dst->bitsPerPixel - 1) ||
	    region.extents.y1 & 63)
		step /= 2;

	max_size = sna_max_tile_copy_size(sna, dst_bo, dst_bo);
	if (max_size == 0)
		goto done;

	while (step * step * 4 > max_size)
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
		int y2 = tile.extents.y1 + step;
		if (y2 > region.extents.y2)
			y2 = region.extents.y2;
		tile.extents.y2 = y2;

		for (tile.extents.x1 = tile.extents.x2 = region.extents.x1;
		     tile.extents.x2 < region.extents.x2;
		     tile.extents.x1 = tile.extents.x2) {
			DrawableRec tmp;
			int x2 = tile.extents.x1 + step;
			if (x2 > region.extents.x2)
				x2 = region.extents.x2;
			tile.extents.x2 = x2;

			tile.data = NULL;

			RegionNull(&this);
			RegionIntersect(&this, &region, &tile);
			if (RegionNil(&this))
				continue;

			tmp.width  = this.extents.x2 - this.extents.x1;
			tmp.height = this.extents.y2 - this.extents.y1;
			tmp.depth  = dst->depth;
			tmp.bitsPerPixel = dst->bitsPerPixel;

			bo = kgem_create_2d(&sna->kgem,
					    tmp.width,
					    tmp.height,
					    dst->bitsPerPixel,
					    kgem_choose_tiling(&sna->kgem,
							       I915_TILING_X,
							       tmp.width,
							       tmp.height,
							       dst->bitsPerPixel),
					    CREATE_TEMPORARY);
			if (bo) {
				int16_t dx = this.extents.x1;
				int16_t dy = this.extents.y1;

				assert(kgem_bo_can_blt(&sna->kgem, bo));

				if (op > PictOpSrc &&
				    !sna->render.copy_boxes(sna, GXcopy,
							    dst, dst_bo, 0, 0,
							    &tmp, bo, -dx, -dy,
							    region_rects(&this), region_num_rects(&this), 0))
					goto err;

				RegionTranslate(&this, -dx, -dy);
				if (!sna->render.fill_boxes(sna, op, format, color, &tmp, bo,
							    region_rects(&this), region_num_rects(&this)))
					goto err;

				if (!sna->render.copy_boxes(sna, GXcopy,
							    &tmp, bo, 0, 0,
							    dst, dst_bo, dx, dy,
							    region_rects(&this), region_num_rects(&this), 0))
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

fastcall static void
tiling_blt(struct sna *sna,
	   const struct sna_composite_op *op,
	   const struct sna_composite_rectangles *r)
{
	int x1, x2, y1, y2;
	int src_x, src_y;
	BoxRec box;

	DBG(("%s: src=(%d, %d), dst=(%d, %d), size=(%d, %d)\n",
	     __FUNCTION__,
	     r->src.x, r->src.y,
	     r->dst.x, r->dst.y,
	     r->width, r->height));

	/* XXX higher layer should have clipped? */

	x1 = r->dst.x + op->dst.x;
	y1 = r->dst.y + op->dst.y;
	x2 = x1 + r->width;
	y2 = y1 + r->height;

	src_x = r->src.x - x1 + op->u.blt.sx;
	src_y = r->src.y - y1 + op->u.blt.sy;

	/* clip against dst */
	if (x1 < 0)
		x1 = 0;
	if (y1 < 0)
		y1 = 0;

	if (x2 > op->dst.width)
		x2 = op->dst.width;

	if (y2 > op->dst.height)
		y2 = op->dst.height;

	DBG(("%s: box=(%d, %d), (%d, %d)\n", __FUNCTION__, x1, y1, x2, y2));

	if (x2 <= x1 || y2 <= y1)
		return;

	box.x1 = x1; box.y1 = y1;
	box.x2 = x2; box.y2 = y2;
	sna_tiling_blt_copy_boxes(sna, GXcopy,
				  op->src.bo, src_x, src_y,
				  op->dst.bo, 0, 0,
				  op->u.blt.bpp,
				  &box, 1);
}

fastcall static void
tiling_blt_box(struct sna *sna,
	       const struct sna_composite_op *op,
	       const BoxRec *box)
{
	DBG(("%s: box (%d, %d), (%d, %d)\n",
	     __FUNCTION__, box->x1, box->y1, box->x2, box->y2));
	sna_tiling_blt_copy_boxes(sna, GXcopy,
				  op->src.bo, op->u.blt.sx, op->u.blt.sy,
				  op->dst.bo, op->dst.x, op->dst.y,
				  op->u.blt.bpp,
				  box, 1);
}

static void
tiling_blt_boxes(struct sna *sna,
		 const struct sna_composite_op *op,
		 const BoxRec *box, int nbox)
{
	DBG(("%s: nbox=%d\n", __FUNCTION__, nbox));
	sna_tiling_blt_copy_boxes(sna, GXcopy,
				  op->src.bo, op->u.blt.sx, op->u.blt.sy,
				  op->dst.bo, op->dst.x, op->dst.y,
				  op->u.blt.bpp,
				  box, nbox);
}

static bool
sna_tiling_blt_copy_boxes__with_alpha(struct sna *sna, uint8_t alu,
				      struct kgem_bo *src_bo, int16_t src_dx, int16_t src_dy,
				      struct kgem_bo *dst_bo, int16_t dst_dx, int16_t dst_dy,
				      int bpp, int alpha_fixup,
				      const BoxRec *box, int nbox)
{
	RegionRec region, tile, this;
	struct kgem_bo *bo;
	int max_size, step;
	bool ret = false;

	if (wedged(sna) ||
	    !kgem_bo_can_blt(&sna->kgem, src_bo) ||
	    !kgem_bo_can_blt(&sna->kgem, dst_bo)) {
		/* XXX */
		DBG(("%s: tiling blt fail: src?=%d, dst?=%d\n",
		     __FUNCTION__,
		     kgem_bo_can_blt(&sna->kgem, src_bo),
		     kgem_bo_can_blt(&sna->kgem, dst_bo)));
		return false;
	}

	max_size = sna_max_tile_copy_size(sna, src_bo, dst_bo);
	if (max_size == 0)
		return false;

	pixman_region_init_rects(&region, box, nbox);

	/* Use a small step to accommodate enlargement through tile alignment */
	step = sna->render.max_3d_size;
	if (region.extents.x1 & (8*512 / bpp - 1) || region.extents.y1 & 63)
		step /= 2;
	while (step * step * 4 > max_size)
		step /= 2;
	if (sna->kgem.gen < 033)
		step /= 2; /* accommodate severe fence restrictions */
	if (step == 0) {
		DBG(("%s: tiles cannot fit into aperture\n", __FUNCTION__));
		return false;
	}

	DBG(("%s (alu=%d), tile.size=%d, box=%dx[(%d, %d), (%d, %d)])\n",
	     __FUNCTION__, alu, step, nbox,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	for (tile.extents.y1 = tile.extents.y2 = region.extents.y1;
	     tile.extents.y2 < region.extents.y2;
	     tile.extents.y1 = tile.extents.y2) {
		int y2 = tile.extents.y1 + step;
		if (y2 > region.extents.y2)
			y2 = region.extents.y2;
		tile.extents.y2 = y2;

		for (tile.extents.x1 = tile.extents.x2 = region.extents.x1;
		     tile.extents.x2 < region.extents.x2;
		     tile.extents.x1 = tile.extents.x2) {
			int w, h;
			int x2 = tile.extents.x1 + step;
			if (x2 > region.extents.x2)
				x2 = region.extents.x2;
			tile.extents.x2 = x2;

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

				assert(kgem_bo_can_blt(&sna->kgem, bo));

				if (!sna_blt_copy_boxes(sna, GXcopy,
							src_bo, src_dx, src_dy,
							bo, -dx, -dy,
							bpp, region_rects(&this), region_num_rects(&this)))
					goto err;

				if (!sna_blt_copy_boxes__with_alpha(sna, alu,
								    bo, -dx, -dy,
								    dst_bo, dst_dx, dst_dy,
								    bpp, alpha_fixup,
								    region_rects(&this), region_num_rects(&this)))
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

fastcall static void
tiling_blt__with_alpha(struct sna *sna,
		       const struct sna_composite_op *op,
		       const struct sna_composite_rectangles *r)
{
	int x1, x2, y1, y2;
	int src_x, src_y;
	BoxRec box;

	DBG(("%s: src=(%d, %d), dst=(%d, %d), size=(%d, %d)\n",
	     __FUNCTION__,
	     r->src.x, r->src.y,
	     r->dst.x, r->dst.y,
	     r->width, r->height));

	/* XXX higher layer should have clipped? */

	x1 = r->dst.x + op->dst.x;
	y1 = r->dst.y + op->dst.y;
	x2 = x1 + r->width;
	y2 = y1 + r->height;

	src_x = r->src.x - x1 + op->u.blt.sx;
	src_y = r->src.y - y1 + op->u.blt.sy;

	/* clip against dst */
	if (x1 < 0)
		x1 = 0;
	if (y1 < 0)
		y1 = 0;

	if (x2 > op->dst.width)
		x2 = op->dst.width;

	if (y2 > op->dst.height)
		y2 = op->dst.height;

	DBG(("%s: box=(%d, %d), (%d, %d)\n", __FUNCTION__, x1, y1, x2, y2));

	if (x2 <= x1 || y2 <= y1)
		return;

	box.x1 = x1; box.y1 = y1;
	box.x2 = x2; box.y2 = y2;
	sna_tiling_blt_copy_boxes__with_alpha(sna, GXcopy,
					      op->src.bo, src_x, src_y,
					      op->dst.bo, 0, 0,
					      op->u.blt.bpp, op->u.blt.pixel,
					      &box, 1);
}

fastcall static void
tiling_blt_box__with_alpha(struct sna *sna,
			   const struct sna_composite_op *op,
			   const BoxRec *box)
{
	DBG(("%s: box (%d, %d), (%d, %d)\n",
	     __FUNCTION__, box->x1, box->y1, box->x2, box->y2));
	sna_tiling_blt_copy_boxes__with_alpha(sna, GXcopy,
					      op->src.bo, op->u.blt.sx, op->u.blt.sy,
					      op->dst.bo, op->dst.x, op->dst.y,
					      op->u.blt.bpp, op->u.blt.pixel,
					      box, 1);
}

static void
tiling_blt_boxes__with_alpha(struct sna *sna,
			     const struct sna_composite_op *op,
			     const BoxRec *box, int nbox)
{
	DBG(("%s: nbox=%d\n", __FUNCTION__, nbox));
	sna_tiling_blt_copy_boxes__with_alpha(sna, GXcopy,
					      op->src.bo, op->u.blt.sx, op->u.blt.sy,
					      op->dst.bo, op->dst.x, op->dst.y,
					      op->u.blt.bpp, op->u.blt.pixel,
					      box, nbox);
}

static void nop_done(struct sna *sna, const struct sna_composite_op *op)
{
	assert(sna->kgem.nbatch <= KGEM_BATCH_SIZE(&sna->kgem));
	(void)op;
}

bool
sna_tiling_blt_composite(struct sna *sna,
			 struct sna_composite_op *op,
			 struct kgem_bo *bo,
			 int bpp,
			 uint32_t alpha_fixup)
{
	assert(op->dst.bo);
	assert(kgem_bo_can_blt(&sna->kgem, op->dst.bo));
	assert(kgem_bo_can_blt(&sna->kgem, bo));

	op->src.bo = bo;
	op->u.blt.bpp = bpp;
	op->u.blt.pixel = alpha_fixup;

	if (alpha_fixup) {
		op->blt   = tiling_blt__with_alpha;
		op->box   = tiling_blt_box__with_alpha;
		op->boxes = tiling_blt_boxes__with_alpha;
	} else {
		op->blt   = tiling_blt;
		op->box   = tiling_blt_box;
		op->boxes = tiling_blt_boxes;
	}
	op->done  = nop_done;

	return true;
}

bool sna_tiling_blt_copy_boxes(struct sna *sna, uint8_t alu,
			       struct kgem_bo *src_bo, int16_t src_dx, int16_t src_dy,
			       struct kgem_bo *dst_bo, int16_t dst_dx, int16_t dst_dy,
			       int bpp, const BoxRec *box, int nbox)
{
	RegionRec region, tile, this;
	struct kgem_bo *bo;
	int max_size, step;
	bool ret = false;

	DBG(("%s: alu=%d, src size=%d, dst size=%d\n", __FUNCTION__,
	     alu, kgem_bo_size(src_bo), kgem_bo_size(dst_bo)));

	if (wedged(sna) ||
	    !kgem_bo_can_blt(&sna->kgem, src_bo) ||
	    !kgem_bo_can_blt(&sna->kgem, dst_bo)) {
		/* XXX */
		DBG(("%s: tiling blt fail: src?=%d, dst?=%d\n",
		     __FUNCTION__,
		     kgem_bo_can_blt(&sna->kgem, src_bo),
		     kgem_bo_can_blt(&sna->kgem, dst_bo)));
		return false;
	}

	max_size = sna_max_tile_copy_size(sna, src_bo, dst_bo);
	if (max_size == 0)
		return false;

	pixman_region_init_rects(&region, box, nbox);

	/* Use a small step to accommodate enlargement through tile alignment */
	step = sna->render.max_3d_size;
	if (region.extents.x1 & (8*512 / bpp - 1) || region.extents.y1 & 63)
		step /= 2;
	while (step * step * 4 > max_size)
		step /= 2;
	if (sna->kgem.gen < 033)
		step /= 2; /* accommodate severe fence restrictions */
	if (step == 0) {
		DBG(("%s: tiles cannot fit into aperture\n", __FUNCTION__));
		return false;
	}

	DBG(("%s (alu=%d), tile.size=%d, box=%dx[(%d, %d), (%d, %d)])\n",
	     __FUNCTION__, alu, step, nbox,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	for (tile.extents.y1 = tile.extents.y2 = region.extents.y1;
	     tile.extents.y2 < region.extents.y2;
	     tile.extents.y1 = tile.extents.y2) {
		int y2 = tile.extents.y1 + step;
		if (y2 > region.extents.y2)
			y2 = region.extents.y2;
		tile.extents.y2 = y2;

		for (tile.extents.x1 = tile.extents.x2 = region.extents.x1;
		     tile.extents.x2 < region.extents.x2;
		     tile.extents.x1 = tile.extents.x2) {
			int w, h;
			int x2 = tile.extents.x1 + step;
			if (x2 > region.extents.x2)
				x2 = region.extents.x2;
			tile.extents.x2 = x2;

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

				assert(kgem_bo_can_blt(&sna->kgem, bo));

				if (!sna_blt_copy_boxes(sna, GXcopy,
							src_bo, src_dx, src_dy,
							bo, -dx, -dy,
							bpp, region_rects(&this), region_num_rects(&this)))
					goto err;

				if (!sna_blt_copy_boxes(sna, alu,
							bo, -dx, -dy,
							dst_bo, dst_dx, dst_dy,
							bpp, region_rects(&this), region_num_rects(&this)))
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

bool
sna_tiling_copy_boxes(struct sna *sna, uint8_t alu,
		      const DrawableRec *src, struct kgem_bo *src_bo, int16_t src_dx, int16_t src_dy,
		      const DrawableRec *dst, struct kgem_bo *dst_bo, int16_t dst_dx, int16_t dst_dy,
		      const BoxRec *box, int n)
{
	BoxRec extents, tile, stack[64], *clipped, *c;
	DrawableRec p;
	int i, step, tiling;
	bool create = true;
	bool ret = false;

	extents = box[0];
	for (i = 1; i < n; i++) {
		if (box[i].x1 < extents.x1)
			extents.x1 = box[i].x1;
		if (box[i].y1 < extents.y1)
			extents.y1 = box[i].y1;

		if (box[i].x2 > extents.x2)
			extents.x2 = box[i].x2;
		if (box[i].y2 > extents.y2)
			extents.y2 = box[i].y2;
	}

	tiling = I915_TILING_X;
	if (!kgem_bo_can_blt(&sna->kgem, src_bo) ||
	    !kgem_bo_can_blt(&sna->kgem, dst_bo))
		tiling = I915_TILING_Y;

	create = (src_bo->pitch > sna->render.max_3d_pitch ||
		  dst_bo->pitch > sna->render.max_3d_pitch);

	step = sna->render.max_3d_size / 2;
	if (create) {
		while (step * step * 4 > sna->kgem.max_upload_tile_size)
			step /= 2;
	}

	DBG(("%s: tiling copy %dx%d, %s %dx%d %c tiles\n", __FUNCTION__,
	     extents.x2-extents.x1, extents.y2-extents.y1,
	     create ? "creating" : "using",
	     step, step, tiling == I915_TILING_X ? 'X' : 'Y'));

	if (n > ARRAY_SIZE(stack)) {
		clipped = malloc(sizeof(BoxRec) * n);
		if (clipped == NULL)
			goto tiled_error;
	} else
		clipped = stack;

	p.depth = src->depth;
	p.bitsPerPixel = src->bitsPerPixel;

	for (tile.y1 = extents.y1; tile.y1 < extents.y2; tile.y1 = tile.y2) {
		int y2 = tile.y1 + step;
		if (y2 > extents.y2)
			y2 = extents.y2;
		tile.y2 = y2;

		for (tile.x1 = extents.x1; tile.x1 < extents.x2; tile.x1 = tile.x2) {
			struct kgem_bo *tmp_bo;
			int x2 = tile.x1 + step;
			if (x2 > extents.x2)
				x2 = extents.x2;
			tile.x2 = x2;

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

			p.width  = tile.x2 - tile.x1;
			p.height = tile.y2 - tile.y1;

			DBG(("%s: tile (%d, %d), (%d, %d)\n",
			     __FUNCTION__, tile.x1, tile.y1, tile.x2, tile.y2));

			if (create) {
				tmp_bo = kgem_create_2d(&sna->kgem,
							p.width,
							p.height,
							p.bitsPerPixel,
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
			} else {
				i = sna->render.copy_boxes(sna, GXcopy,
							   src, src_bo, src_dx, src_dy,
							   dst, dst_bo, dst_dx, dst_dy,
							   clipped, c - clipped, 0);
			}

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
