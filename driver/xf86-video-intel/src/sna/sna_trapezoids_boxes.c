/*
 * Copyright (c) 2007  David Turner
 * Copyright (c) 2008  M Joonas Pihlaja
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
#include "sna_trapezoids.h"
#include "fb/fbpict.h"

#include <mipict.h>

/* TODO: Emit unantialiased and MSAA triangles. */

#ifndef MAX
#define MAX(x,y) ((x) >= (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x,y) ((x) <= (y) ? (x) : (y))
#endif

#define region_count(r) ((r)->data ? (r)->data->numRects : 1)
#define region_boxes(r) ((r)->data ? (BoxPtr)((r)->data + 1) : &(r)->extents)

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
	DBG(("%s: damage=%p, region=%dx[(%d, %d), (%d, %d)]\n",
	     __FUNCTION__, op->damage,
	     region_num_rects(region),
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2));

	if (op->damage == NULL)
		return;

	RegionTranslate(region, op->dst.x, op->dst.y);

	assert_pixmap_contains_box(op->dst.pixmap, RegionExtents(region));
	sna_damage_add(op->damage, region);
}

static void _apply_damage_box(struct sna_composite_op *op, const BoxRec *box)
{
	BoxRec r;

	r.x1 = box->x1 + op->dst.x;
	r.x2 = box->x2 + op->dst.x;
	r.y1 = box->y1 + op->dst.y;
	r.y2 = box->y2 + op->dst.y;

	assert_pixmap_contains_box(op->dst.pixmap, &r);
	sna_damage_add_box(op->damage, &r);
}

inline static void apply_damage_box(struct sna_composite_op *op, const BoxRec *box)
{
	if (op->damage)
		_apply_damage_box(op, box);
}

bool
composite_aligned_boxes(struct sna *sna,
			CARD8 op,
			PicturePtr src,
			PicturePtr dst,
			PictFormatPtr maskFormat,
			INT16 src_x, INT16 src_y,
			int ntrap, const xTrapezoid *traps,
			bool force_fallback)
{
	BoxRec stack_boxes[64], *boxes;
	pixman_region16_t region, clip;
	struct sna_composite_op tmp;
	int16_t dst_x, dst_y;
	bool ret = true;
	int dx, dy, n, num_boxes;

	if (NO_ALIGNED_BOXES)
		return false;

	DBG(("%s: pixmap=%ld, nboxes=%d, dx=(%d, %d)\n", __FUNCTION__,
	    get_drawable_pixmap(dst->pDrawable)->drawable.serialNumber,
	    ntrap, dst->pDrawable->x, dst->pDrawable->y));

	boxes = stack_boxes;
	if (ntrap > (int)ARRAY_SIZE(stack_boxes)) {
		boxes = malloc(sizeof(BoxRec)*ntrap);
		if (boxes == NULL)
			return false;
	}

	dx = dst->pDrawable->x;
	dy = dst->pDrawable->y;

	region.extents.x1 = region.extents.y1 = 32767;
	region.extents.x2 = region.extents.y2 = -32767;
	num_boxes = 0;
	for (n = 0; n < ntrap; n++) {
		boxes[num_boxes].x1 = dx + pixman_fixed_to_int(traps[n].left.p1.x + pixman_fixed_1_minus_e/2);
		boxes[num_boxes].y1 = dy + pixman_fixed_to_int(traps[n].top + pixman_fixed_1_minus_e/2);
		boxes[num_boxes].x2 = dx + pixman_fixed_to_int(traps[n].right.p2.x + pixman_fixed_1_minus_e/2);
		boxes[num_boxes].y2 = dy + pixman_fixed_to_int(traps[n].bottom + pixman_fixed_1_minus_e/2);

		if (boxes[num_boxes].x1 >= boxes[num_boxes].x2)
			continue;
		if (boxes[num_boxes].y1 >= boxes[num_boxes].y2)
			continue;

		if (boxes[num_boxes].x1 < region.extents.x1)
			region.extents.x1 = boxes[num_boxes].x1;
		if (boxes[num_boxes].x2 > region.extents.x2)
			region.extents.x2 = boxes[num_boxes].x2;

		if (boxes[num_boxes].y1 < region.extents.y1)
			region.extents.y1 = boxes[num_boxes].y1;
		if (boxes[num_boxes].y2 > region.extents.y2)
			region.extents.y2 = boxes[num_boxes].y2;

		num_boxes++;
	}

	if (num_boxes == 0)
		goto free_boxes;

	trapezoid_origin(&traps[0].left, &dst_x, &dst_y);

	DBG(("%s: extents (%d, %d), (%d, %d) offset of (%d, %d), origin (%d, %d)\n",
	     __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2,
	     region.extents.x1 - boxes[0].x1,
	     region.extents.y1 - boxes[0].y1,
	     dst_x, dst_y));

	if (!sna_compute_composite_region(&clip,
					  src, NULL, dst,
					  src_x + region.extents.x1 - dst_x - dx,
					  src_y + region.extents.y1 - dst_y - dy,
					  0, 0,
					  region.extents.x1 - dx, region.extents.y1 - dy,
					  region.extents.x2 - region.extents.x1,
					  region.extents.y2 - region.extents.y1)) {
		DBG(("%s: trapezoids do not intersect drawable clips\n",
		     __FUNCTION__)) ;
		goto done;
	}

	if (op == PictOpClear && sna->clear)
		src = sna->clear;

	DBG(("%s: clipped extents (%d, %d), (%d, %d);  now offset by (%d, %d), orgin (%d, %d)\n",
	     __FUNCTION__,
	     clip.extents.x1, clip.extents.y1,
	     clip.extents.x2, clip.extents.y2,
	     clip.extents.x1 - boxes[0].x1,
	     clip.extents.y1 - boxes[0].y1,
	     dst_x, dst_y));

	if (force_fallback ||
	    !sna->render.composite(sna, op, src, NULL, dst,
				   src_x + clip.extents.x1 - dst_x,
				   src_y + clip.extents.y1 - dst_y,
				   0, 0,
				   clip.extents.x1,  clip.extents.y1,
				   clip.extents.x2 - clip.extents.x1,
				   clip.extents.y2 - clip.extents.y1,
				   (clip.data || num_boxes > 1) ?  COMPOSITE_PARTIAL : 0,
				   memset(&tmp, 0, sizeof(tmp)))) {
		unsigned int flags;
		const pixman_box16_t *b;
		int i, count;

		DBG(("%s: composite render op not supported\n",
		     __FUNCTION__));

		flags = MOVE_READ | MOVE_WRITE;
		if (op <= PictOpSrc) {
			flags |= MOVE_INPLACE_HINT;
			if (n == 1)
				flags &= ~MOVE_READ;
		}

		if (!sna_drawable_move_region_to_cpu(dst->pDrawable, &clip, flags))
			goto done;
		if (dst->alphaMap  &&
		    !sna_drawable_move_to_cpu(dst->alphaMap->pDrawable,
					      MOVE_READ | MOVE_WRITE))
			goto done;
		if (src->pDrawable) {
			if (!sna_drawable_move_to_cpu(src->pDrawable,
						      MOVE_READ))
				goto done;
			if (src->alphaMap &&
			    !sna_drawable_move_to_cpu(src->alphaMap->pDrawable,
						      MOVE_READ))
				goto done;
		}

		DBG(("%s: fbComposite()\n", __FUNCTION__));
		src_x -= dst_x - dx;
		src_y -= dst_y - dy;
		if (maskFormat) {
			pixman_region_init_rects(&region, boxes, num_boxes);
			RegionIntersect(&region, &region, &clip);

			if (sigtrap_get() == 0) {
				b = region_rects(&region);
				count = region_num_rects(&region);
				for (i = 0; i < count; i++) {
					fbComposite(op, src, NULL, dst,
						    src_x + b[i].x1,
						    src_y + b[i].y1,
						    0, 0,
						    b[i].x1, b[i].y1,
						    b[i].x2 - b[i].x1, b[i].y2 - b[i].y1);
				}
				sigtrap_put();
			}
			pixman_region_fini(&region);
		} else {
			for (n = 0; n < num_boxes; n++) {
				pixman_region_init_rects(&region, &boxes[n], 1);
				RegionIntersect(&region, &region, &clip);
				b = region_rects(&region);
				count = region_num_rects(&region);
				if (sigtrap_get() == 0) {
					for (i = 0; i < count; i++) {
						fbComposite(op, src, NULL, dst,
							    src_x + b[i].x1,
							    src_y + b[i].y1,
							    0, 0,
							    b[i].x1, b[i].y1,
							    b[i].x2 - b[i].x1, b[i].y2 - b[i].y1);
					}
					sigtrap_put();
				}
				pixman_region_fini(&region);
				pixman_region_fini(&region);
			}
		}
		ret = true;
		goto done;
	}

	if (maskFormat ||
	    (op == PictOpSrc || op == PictOpClear) ||
	    num_boxes == 1) {
		pixman_region_init_rects(&region, boxes, num_boxes);
		RegionIntersect(&region, &region, &clip);
		if (region_num_rects(&region)) {
			tmp.boxes(sna, &tmp,
				  region_rects(&region),
				  region_num_rects(&region));
			apply_damage(&tmp, &region);
		}
		pixman_region_fini(&region);
	} else {
		for (n = 0; n < num_boxes; n++) {
			pixman_region_init_rects(&region, &boxes[n], 1);
			RegionIntersect(&region, &region, &clip);
			if (region_num_rects(&region)) {
				tmp.boxes(sna, &tmp,
					  region_rects(&region),
					  region_num_rects(&region));
				apply_damage(&tmp, &region);
			}
			pixman_region_fini(&region);
		}
	}
	tmp.done(sna, &tmp);

done:
	REGION_UNINIT(NULL, &clip);
free_boxes:
	if (boxes != stack_boxes)
		free(boxes);

	return ret;
}

static inline int grid_coverage(int samples, pixman_fixed_t f)
{
	return (samples * pixman_fixed_frac(f) + pixman_fixed_1/2) / pixman_fixed_1;
}

inline static void
composite_unaligned_box(struct sna *sna,
			struct sna_composite_spans_op *tmp,
			const BoxRec *box,
			float opacity,
			pixman_region16_t *clip)
{
	assert(opacity != 0.);

	if (clip) {
		pixman_region16_t region;

		pixman_region_init_rects(&region, box, 1);
		RegionIntersect(&region, &region, clip);
		if (region_num_rects(&region))
			tmp->boxes(sna, tmp,
				   region_rects(&region),
				   region_num_rects(&region),
				   opacity);
		pixman_region_fini(&region);
	} else
		tmp->box(sna, tmp, box, opacity);
}

inline static void
composite_unaligned_trap_row(struct sna *sna,
			     struct sna_composite_spans_op *tmp,
			     const xTrapezoid *trap, int dx,
			     int y1, int y2, int covered,
			     pixman_region16_t *clip)
{
	BoxRec box;
	int opacity;
	int x1, x2;
#define u8_to_float(x) ((x) * (1.f/255))

	if (covered == 0)
		return;

	x1 = dx + pixman_fixed_to_int(trap->left.p1.x);
	x2 = dx + pixman_fixed_to_int(trap->right.p1.x);
	if (clip) {
		if (y2 > clip->extents.y2)
			y2 = clip->extents.y2;
		if (y1 < clip->extents.y1)
			y1 = clip->extents.y1;
		if (y1 >= y2)
			return;

		if (x2 < clip->extents.x1 || x1 > clip->extents.x2)
			return;
	}

	box.y1 = y1;
	box.y2 = y2;

	if (x1 == x2) {
		box.x1 = x1;
		box.x2 = x2 + 1;

		opacity = covered;
		opacity *= grid_coverage(SAMPLES_X, trap->right.p1.x) - grid_coverage(SAMPLES_X, trap->left.p1.x);

		if (opacity)
			composite_unaligned_box(sna, tmp, &box,
						u8_to_float(opacity), clip);
	} else {
		if (pixman_fixed_frac(trap->left.p1.x)) {
			box.x1 = x1;
			box.x2 = ++x1;

			opacity = covered;
			opacity *= SAMPLES_X - grid_coverage(SAMPLES_X, trap->left.p1.x);

			if (opacity)
				composite_unaligned_box(sna, tmp, &box,
							u8_to_float(opacity), clip);
		}

		if (x2 > x1) {
			box.x1 = x1;
			box.x2 = x2;

			composite_unaligned_box(sna, tmp, &box,
						covered == SAMPLES_Y ? 1. : u8_to_float(covered*SAMPLES_X),
						clip);
		}

		if (pixman_fixed_frac(trap->right.p1.x)) {
			box.x1 = x2;
			box.x2 = x2 + 1;

			opacity = covered;
			opacity *= grid_coverage(SAMPLES_X, trap->right.p1.x);

			if (opacity)
				composite_unaligned_box(sna, tmp, &box,
							u8_to_float(opacity), clip);
		}
	}
}

flatten static void
composite_unaligned_trap(struct sna *sna,
			struct sna_composite_spans_op *tmp,
			const xTrapezoid *trap,
			int dx, int dy,
			pixman_region16_t *clip)
{
	int y1, y2;

	y1 = dy + pixman_fixed_to_int(trap->top);
	y2 = dy + pixman_fixed_to_int(trap->bottom);

	DBG(("%s: y1=%d, y2=%d\n", __FUNCTION__, y1, y2));

	if (y1 == y2) {
		composite_unaligned_trap_row(sna, tmp, trap, dx,
					     y1, y1 + 1,
					     grid_coverage(SAMPLES_Y, trap->bottom) - grid_coverage(SAMPLES_Y, trap->top),
					     clip);
	} else {
		if (pixman_fixed_frac(trap->top)) {
			composite_unaligned_trap_row(sna, tmp, trap, dx,
						     y1, y1 + 1,
						     SAMPLES_Y - grid_coverage(SAMPLES_Y, trap->top),
						     clip);
			y1++;
		}

		if (y2 > y1)
			composite_unaligned_trap_row(sna, tmp, trap, dx,
						     y1, y2,
						     SAMPLES_Y,
						     clip);

		if (pixman_fixed_frac(trap->bottom))
			composite_unaligned_trap_row(sna, tmp, trap, dx,
						     y2, y2 + 1,
						     grid_coverage(SAMPLES_Y, trap->bottom),
						     clip);
	}

	if (tmp->base.damage) {
		BoxRec box;

		box.x1 = dx + pixman_fixed_to_int(trap->left.p1.x);
		box.x2 = dx + pixman_fixed_to_int(trap->right.p1.x + pixman_fixed_1_minus_e);
		box.y1 = dy + pixman_fixed_to_int(trap->top);
		box.y2 = dy + pixman_fixed_to_int(trap->bottom + pixman_fixed_1_minus_e);

		if (clip) {
			pixman_region16_t region;

			pixman_region_init_rects(&region, &box, 1);
			RegionIntersect(&region, &region, clip);
			if (region_num_rects(&region))
				apply_damage(&tmp->base, &region);
			RegionUninit(&region);
		} else
			apply_damage_box(&tmp->base, &box);
	}
}

inline static void
blt_opacity(PixmapPtr scratch,
	    int x1, int x2,
	    int y, int h,
	    uint8_t opacity)
{
	uint8_t *ptr;

	if (opacity == 0xff)
		return;

	if (x1 < 0)
		x1 = 0;
	if (x2 > scratch->drawable.width)
		x2 = scratch->drawable.width;
	if (x1 >= x2)
		return;

	x2 -= x1;

	ptr = scratch->devPrivate.ptr;
	ptr += scratch->devKind * y;
	ptr += x1;
	do {
		if (x2 == 1)
			*ptr = opacity;
		else
			memset(ptr, opacity, x2);
		ptr += scratch->devKind;
	} while (--h);
}

static void
blt_unaligned_box_row(PixmapPtr scratch,
		      BoxPtr extents,
		      const xTrapezoid *trap,
		      int y1, int y2,
		      int covered)
{
	int x1, x2;

	if (y2 > scratch->drawable.height)
		y2 = scratch->drawable.height;
	if (y1 < 0)
		y1 = 0;
	if (y1 >= y2)
		return;

	y2 -= y1;

	x1 = pixman_fixed_to_int(trap->left.p1.x);
	x2 = pixman_fixed_to_int(trap->right.p1.x);

	x1 -= extents->x1;
	x2 -= extents->x1;

	if (x1 == x2) {
		blt_opacity(scratch,
			    x1, x1+1,
			    y1, y2,
			    covered * (grid_coverage(SAMPLES_X, trap->right.p1.x) - grid_coverage(SAMPLES_X, trap->left.p1.x)));
	} else {
		if (pixman_fixed_frac(trap->left.p1.x)) {
			blt_opacity(scratch,
				    x1, x1 + 1,
				    y1, y2,
				    covered * (SAMPLES_X - grid_coverage(SAMPLES_X, trap->left.p1.x)));
			x1++;
		}

		if (x2 > x1) {
			blt_opacity(scratch,
				    x1, x2,
				    y1, y2,
				    covered*SAMPLES_X);
		}

		if (pixman_fixed_frac(trap->right.p1.x))
			blt_opacity(scratch,
				    x2, x2 + 1,
				    y1, y2,
				    covered * grid_coverage(SAMPLES_X, trap->right.p1.x));
	}
}

inline static void
lerp32_opacity(PixmapPtr scratch,
	       uint32_t color,
	       int16_t x, int16_t w,
	       int16_t y, int16_t h,
	       uint8_t opacity)
{
	uint32_t *ptr;
	int stride, i;

	ptr = (uint32_t*)((uint8_t *)scratch->devPrivate.ptr + scratch->devKind * y);
	ptr += x;
	stride = scratch->devKind / 4;

	if (opacity == 0xff) {
		if ((w | h) == 1) {
			*ptr = color;
		} else {
			if (w < 16) {
				do {
					for (i = 0; i < w; i++)
						ptr[i] = color;
					ptr += stride;
				} while (--h);
			} else {
				pixman_fill(ptr, stride, 32,
					    0, 0, w, h, color);
			}
		}
	} else {
		if ((w | h) == 1) {
			*ptr = lerp8x4(color, opacity, *ptr);
		} else if (w == 1) {
			do {
				*ptr = lerp8x4(color, opacity, *ptr);
				ptr += stride;
			} while (--h);
		} else{
			do {
				for (i = 0; i < w; i++)
					ptr[i] = lerp8x4(color, opacity, ptr[i]);
				ptr += stride;
			} while (--h);
		}
	}
}

static void
lerp32_unaligned_box_row(PixmapPtr scratch, uint32_t color,
			 const BoxRec *extents,
			 const xTrapezoid *trap, int16_t dx,
			 int16_t y, int16_t h,
			 uint8_t covered)
{
	int16_t x1 = pixman_fixed_to_int(trap->left.p1.x) + dx;
	uint16_t fx1 = grid_coverage(SAMPLES_X, trap->left.p1.x);
	int16_t x2 = pixman_fixed_to_int(trap->right.p2.x) + dx;
	uint16_t fx2 = grid_coverage(SAMPLES_X, trap->right.p2.x);

	if (x1 < extents->x1)
		x1 = extents->x1, fx1 = 0;
	if (x2 >= extents->x2)
		x2 = extents->x2, fx2 = 0;

	DBG(("%s: x=(%d.%d, %d.%d), y=%dx%d, covered=%d\n", __FUNCTION__,
	     x1, fx1, x2, fx2, y, h, covered));

	if (x1 < x2) {
		if (fx1) {
			lerp32_opacity(scratch, color,
				       x1, 1,
				       y, h,
				       covered * (SAMPLES_X - fx1));
			x1++;
		}

		if (x2 > x1) {
			lerp32_opacity(scratch, color,
				       x1, x2-x1,
				       y, h,
				       covered*SAMPLES_X);
		}

		if (fx2) {
			lerp32_opacity(scratch, color,
				       x2, 1,
				       y, h,
				       covered * fx2);
		}
	} else if (x1 == x2 && fx2 > fx1) {
		lerp32_opacity(scratch, color,
			       x1, 1,
			       y, h,
			       covered * (fx2 - fx1));
	}
}

struct pixman_inplace {
	pixman_image_t *image, *source, *mask;
	uint32_t color;
	uint32_t *bits;
	int dx, dy;
	int sx, sy;
	uint8_t op;
};

inline static void
pixsolid_opacity(struct pixman_inplace *pi,
		 int16_t x, int16_t w,
		 int16_t y, int16_t h,
		 uint8_t opacity)
{
	if (opacity == 0xff)
		*pi->bits = pi->color;
	else
		*pi->bits = mul_4x8_8(pi->color, opacity);
	sna_image_composite(pi->op, pi->source, NULL, pi->image,
			    0, 0, 0, 0, pi->dx + x, pi->dy + y, w, h);
}

static void
pixsolid_unaligned_box_row(struct pixman_inplace *pi,
			   const BoxRec *extents,
			   const xTrapezoid *trap,
			   int16_t y, int16_t h,
			   uint8_t covered)
{
	int16_t x1 = pixman_fixed_to_int(trap->left.p1.x);
	uint16_t fx1 = grid_coverage(SAMPLES_X, trap->left.p1.x);
	int16_t x2 = pixman_fixed_to_int(trap->right.p1.x);
	uint16_t fx2 = grid_coverage(SAMPLES_X, trap->right.p1.x);

	if (x1 < extents->x1)
		x1 = extents->x1, fx1 = 0;
	if (x2 >= extents->x2)
		x2 = extents->x2, fx2 = 0;

	if (x1 < x2) {
		if (fx1) {
			pixsolid_opacity(pi, x1, 1, y, h,
					 covered * (SAMPLES_X - fx1));
			x1++;
		}

		if (x2 > x1)
			pixsolid_opacity(pi, x1, x2-x1, y, h, covered*SAMPLES_X);

		if (fx2)
			pixsolid_opacity(pi, x2, 1, y, h, covered * fx2);
	} else if (x1 == x2 && fx2 > fx1) {
		pixsolid_opacity(pi, x1, 1, y, h, covered * (fx2 - fx1));
	}
}

static bool
composite_unaligned_boxes_inplace__solid(struct sna *sna,
					 CARD8 op, uint32_t color,
					 PicturePtr dst,
					 int n, const xTrapezoid *t,
					 bool force_fallback)
{
	PixmapPtr pixmap;
	int16_t dx, dy;

	DBG(("%s: force=%d, is_gpu=%d, op=%d, color=%x\n", __FUNCTION__,
	     force_fallback, is_gpu(sna, dst->pDrawable, PREFER_GPU_SPANS), op, color));

	if (!force_fallback && is_gpu(sna, dst->pDrawable, PREFER_GPU_SPANS)) {
		DBG(("%s: fallback -- can not perform operation in place, destination busy\n",
		     __FUNCTION__));

		return false;
	}

	/* XXX a8 boxes */
	if (!(dst->format == PICT_a8r8g8b8 || dst->format == PICT_x8r8g8b8)) {
		DBG(("%s: fallback -- can not perform operation in place, unhanbled format %08lx\n",
		     __FUNCTION__, (long)dst->format));

		goto pixman;
	}

	pixmap = get_drawable_pixmap(dst->pDrawable);
	get_drawable_deltas(dst->pDrawable, pixmap, &dx, &dy);

	if (op == PictOpOver && (color >> 24) == 0xff)
		op = PictOpSrc;
	if (op == PictOpOver || op == PictOpAdd) {
		struct sna_pixmap *priv = sna_pixmap(pixmap);
		if (priv && priv->clear && priv->clear_color == 0)
			op = PictOpSrc;
	}

	switch (op) {
	case PictOpSrc:
		break;
	default:
		DBG(("%s: fallback -- can not perform op [%d] in place\n",
		     __FUNCTION__, op));
		goto pixman;
	}

	DBG(("%s: inplace operation on argb32 destination x %d\n",
	     __FUNCTION__, n));
	do {
		RegionRec clip;
		const BoxRec *extents;
		int count;

		clip.extents.x1 = pixman_fixed_to_int(t->left.p1.x);
		clip.extents.x2 = pixman_fixed_to_int(t->right.p1.x + pixman_fixed_1_minus_e);
		clip.extents.y1 = pixman_fixed_to_int(t->top);
		clip.extents.y2 = pixman_fixed_to_int(t->bottom + pixman_fixed_1_minus_e);
		clip.data = NULL;

		if (!sna_compute_composite_region(&clip,
						   NULL, NULL, dst,
						   0, 0,
						   0, 0,
						   clip.extents.x1, clip.extents.y1,
						   clip.extents.x2 - clip.extents.x1,
						   clip.extents.y2 - clip.extents.y1))
			continue;

		if (!sna_drawable_move_region_to_cpu(dst->pDrawable, &clip,
						     MOVE_WRITE | MOVE_READ)) {
			RegionUninit(&clip);
			continue;
		}

		if (sigtrap_get() == 0) {
			RegionTranslate(&clip, dx, dy);
			count = region_num_rects(&clip);
			extents = region_rects(&clip);
			while (count--) {
				int16_t y1 = dy + pixman_fixed_to_int(t->top);
				uint16_t fy1 = pixman_fixed_frac(t->top);
				int16_t y2 = dy + pixman_fixed_to_int(t->bottom);
				uint16_t fy2 = pixman_fixed_frac(t->bottom);

				DBG(("%s: t=(%d, %d), (%d, %d), extents (%d, %d), (%d, %d)\n",
				     __FUNCTION__,
				     pixman_fixed_to_int(t->left.p1.x),
				     pixman_fixed_to_int(t->top),
				     pixman_fixed_to_int(t->right.p2.x),
				     pixman_fixed_to_int(t->bottom),
				     extents->x1, extents->y1,
				     extents->x2, extents->y2));

				if (y1 < extents->y1)
					y1 = extents->y1, fy1 = 0;
				if (y2 >= extents->y2)
					y2 = extents->y2, fy2 = 0;

				if (y1 < y2) {
					if (fy1) {
						lerp32_unaligned_box_row(pixmap, color, extents,
									 t, dx, y1, 1,
									 SAMPLES_Y - grid_coverage(SAMPLES_Y, fy1));
						y1++;
					}

					if (y2 > y1)
						lerp32_unaligned_box_row(pixmap, color, extents,
									 t, dx, y1, y2 - y1,
									 SAMPLES_Y);

					if (fy2)
						lerp32_unaligned_box_row(pixmap, color,  extents,
									 t, dx, y2, 1,
									 grid_coverage(SAMPLES_Y, fy2));
				} else if (y1 == y2 && fy2 > fy1) {
					lerp32_unaligned_box_row(pixmap, color, extents,
								 t, dx, y1, 1,
								 grid_coverage(SAMPLES_Y, fy2) - grid_coverage(SAMPLES_Y, fy1));
				}
				extents++;
			}
			sigtrap_put();
		}

		RegionUninit(&clip);
	} while (--n && t++);

	return true;

pixman:
	do {
		struct pixman_inplace pi;
		RegionRec clip;
		const BoxRec *extents;
		int count;

		clip.extents.x1 = pixman_fixed_to_int(t->left.p1.x);
		clip.extents.x2 = pixman_fixed_to_int(t->right.p1.x + pixman_fixed_1_minus_e);
		clip.extents.y1 = pixman_fixed_to_int(t->top);
		clip.extents.y2 = pixman_fixed_to_int(t->bottom + pixman_fixed_1_minus_e);
		clip.data = NULL;

		if (!sna_compute_composite_region(&clip,
						   NULL, NULL, dst,
						   0, 0,
						   0, 0,
						   clip.extents.x1, clip.extents.y1,
						   clip.extents.x2 - clip.extents.x1,
						   clip.extents.y2 - clip.extents.y1))
			continue;

		if (!sna_drawable_move_region_to_cpu(dst->pDrawable, &clip,
						     MOVE_WRITE | MOVE_READ)) {
			RegionUninit(&clip);
			continue;
		}

		pi.image = image_from_pict(dst, false, &pi.dx, &pi.dy);
		pi.source = pixman_image_create_bits(PIXMAN_a8r8g8b8, 1, 1, NULL, 0);
		pixman_image_set_repeat(pi.source, PIXMAN_REPEAT_NORMAL);
		pi.bits = pixman_image_get_data(pi.source);
		pi.color = color;
		pi.op = op;

		if (sigtrap_get() == 0) {
			count = region_num_rects(&clip);
			extents = region_rects(&clip);
			while (count--) {
				int16_t y1 = pixman_fixed_to_int(t->top);
				uint16_t fy1 = pixman_fixed_frac(t->top);
				int16_t y2 = pixman_fixed_to_int(t->bottom);
				uint16_t fy2 = pixman_fixed_frac(t->bottom);

				if (y1 < extents->y1)
					y1 = extents->y1, fy1 = 0;
				if (y2 >= extents->y2)
					y2 = extents->y2, fy2 = 0;
				if (y1 < y2) {
					if (fy1) {
						pixsolid_unaligned_box_row(&pi, extents, t, y1, 1,
									   SAMPLES_Y - grid_coverage(SAMPLES_Y, fy1));
						y1++;
					}

					if (y2 > y1)
						pixsolid_unaligned_box_row(&pi, extents, t, y1, y2 - y1,
									   SAMPLES_Y);

					if (fy2)
						pixsolid_unaligned_box_row(&pi, extents, t, y2, 1,
									   grid_coverage(SAMPLES_Y, fy2));
				} else if (y1 == y2 && fy2 > fy1) {
					pixsolid_unaligned_box_row(&pi, extents, t, y1, 1,
								   grid_coverage(SAMPLES_Y, fy2) - grid_coverage(SAMPLES_Y, fy1));
				}
				extents++;
			}
			sigtrap_put();
		}

		RegionUninit(&clip);
		pixman_image_unref(pi.image);
		pixman_image_unref(pi.source);
	} while (--n && t++);
	return true;
}

inline static void
pixmask_opacity(struct pixman_inplace *pi,
		int16_t x, int16_t w,
		int16_t y, int16_t h,
		uint8_t opacity)
{
	if (opacity == 0xff) {
		pixman_image_composite(pi->op, pi->source, NULL, pi->image,
				       pi->sx + x, pi->sy + y,
				       0, 0,
				       pi->dx + x, pi->dy + y,
				       w, h);
	} else {
		*pi->bits = opacity;
		pixman_image_composite(pi->op, pi->source, pi->mask, pi->image,
				       pi->sx + x, pi->sy + y,
				       0, 0,
				       pi->dx + x, pi->dy + y,
				       w, h);
	}
}

static void
pixmask_unaligned_box_row(struct pixman_inplace *pi,
			  const BoxRec *extents,
			  const xTrapezoid *trap,
			  int16_t y, int16_t h,
			  uint8_t covered)
{
	int16_t x1 = pixman_fixed_to_int(trap->left.p1.x);
	uint16_t fx1 = grid_coverage(SAMPLES_X, trap->left.p1.x);
	int16_t x2 = pixman_fixed_to_int(trap->right.p1.x);
	uint16_t fx2 = grid_coverage(SAMPLES_X, trap->right.p1.x);

	if (x1 < extents->x1)
		x1 = extents->x1, fx1 = 0;
	if (x2 >= extents->x2)
		x2 = extents->x2, fx2 = 0;

	if (x1 < x2) {
		if (fx1) {
			pixmask_opacity(pi, x1, 1, y, h,
					 covered * (SAMPLES_X - fx1));
			x1++;
		}

		if (x2 > x1)
			pixmask_opacity(pi, x1, x2-x1, y, h, covered*SAMPLES_X);

		if (fx2)
			pixmask_opacity(pi, x2, 1, y, h, covered * fx2);
	} else if (x1 == x2 && fx2 > fx1) {
		pixmask_opacity(pi, x1, 1, y, h, covered * (fx2 - fx1));
	}
}

struct rectilinear_inplace_thread {
	pixman_image_t *dst, *src;
	const RegionRec *clip;
	const xTrapezoid *trap;
	int dx, dy, sx, sy;
	int y1, y2;
	CARD8 op;
};

static void rectilinear_inplace_thread(void *arg)
{
	struct rectilinear_inplace_thread *thread = arg;
	const xTrapezoid *t = thread->trap;
	struct pixman_inplace pi;
	const BoxRec *extents;
	int count;

	pi.image = thread->dst;
	pi.dx = thread->dx;
	pi.dy = thread->dy;

	pi.source = thread->src;
	pi.sx = thread->sx;
	pi.sy = thread->sy;

	pi.mask = pixman_image_create_bits(PIXMAN_a8, 1, 1, &pi.color, 4);
	pixman_image_set_repeat(pi.mask, PIXMAN_REPEAT_NORMAL);
	pi.bits = pixman_image_get_data(pi.mask);
	pi.op = thread->op;

	count = region_count(thread->clip);
	extents = region_boxes(thread->clip);
	while (count--) {
		int16_t y1 = pixman_fixed_to_int(t->top);
		uint16_t fy1 = pixman_fixed_frac(t->top);
		int16_t y2 = pixman_fixed_to_int(t->bottom);
		uint16_t fy2 = pixman_fixed_frac(t->bottom);

		if (y1 < MAX(thread->y1, extents->y1))
			y1 = MAX(thread->y1, extents->y1), fy1 = 0;
		if (y2 > MIN(thread->y2, extents->y2))
			y2 = MIN(thread->y2, extents->y2), fy2 = 0;
		if (y1 < y2) {
			if (fy1) {
				pixmask_unaligned_box_row(&pi, extents, t, y1, 1,
							  SAMPLES_Y - grid_coverage(SAMPLES_Y, fy1));
				y1++;
			}

			if (y2 > y1)
				pixmask_unaligned_box_row(&pi, extents, t, y1, y2 - y1,
							  SAMPLES_Y);

			if (fy2)
				pixmask_unaligned_box_row(&pi, extents, t, y2, 1,
							  grid_coverage(SAMPLES_Y, fy2));
		} else if (y1 == y2 && fy2 > fy1) {
			pixmask_unaligned_box_row(&pi, extents, t, y1, 1,
						  grid_coverage(SAMPLES_Y, fy2) - grid_coverage(SAMPLES_Y, fy1));
		}
		extents++;
	}

	pixman_image_unref(pi.mask);
}

static bool
composite_unaligned_boxes_inplace(struct sna *sna,
				  CARD8 op,
				  PicturePtr src, int16_t src_x, int16_t src_y,
				  PicturePtr dst, int n, const xTrapezoid *t,
				  bool force_fallback)
{
	if (!force_fallback &&
	    (is_gpu(sna, dst->pDrawable, PREFER_GPU_SPANS) ||
	     picture_is_gpu(sna, src, PREFER_GPU_SPANS))) {
		DBG(("%s: fallback -- not forcing\n", __FUNCTION__));
		return false;
	}

	DBG(("%s\n", __FUNCTION__));

	src_x -= pixman_fixed_to_int(t[0].left.p1.x);
	src_y -= pixman_fixed_to_int(t[0].left.p1.y);
	do {
		RegionRec clip;
		const BoxRec *extents;
		int count;
		int num_threads;

		clip.extents.x1 = pixman_fixed_to_int(t->left.p1.x);
		clip.extents.x2 = pixman_fixed_to_int(t->right.p1.x + pixman_fixed_1_minus_e);
		clip.extents.y1 = pixman_fixed_to_int(t->top);
		clip.extents.y2 = pixman_fixed_to_int(t->bottom + pixman_fixed_1_minus_e);
		clip.data = NULL;

		if (!sna_compute_composite_region(&clip,
						   src, NULL, dst,
						   clip.extents.x1 + src_x,
						   clip.extents.y1 + src_y,
						   0, 0,
						   clip.extents.x1, clip.extents.y1,
						   clip.extents.x2 - clip.extents.x1,
						   clip.extents.y2 - clip.extents.y1))
			continue;

		if (!sna_drawable_move_region_to_cpu(dst->pDrawable, &clip,
						     MOVE_WRITE | MOVE_READ)) {
			RegionUninit(&clip);
			continue;
		}

		if (src->pDrawable) {
			if (!sna_drawable_move_to_cpu(src->pDrawable,
						      MOVE_READ)) {
				RegionUninit(&clip);
				continue;
			}
			if (src->alphaMap) {
				if (!sna_drawable_move_to_cpu(src->alphaMap->pDrawable,
							      MOVE_READ)) {
					RegionUninit(&clip);
					continue;
				}
			}
		}

		num_threads = sna_use_threads(clip.extents.x2 - clip.extents.x1,
					      clip.extents.y2 - clip.extents.y1,
					      32);
		if (num_threads == 1) {
			struct pixman_inplace pi;

			pi.image = image_from_pict(dst, false, &pi.dx, &pi.dy);
			pi.source = image_from_pict(src, false, &pi.sx, &pi.sy);
			pi.sx += src_x;
			pi.sy += src_y;
			pi.mask = pixman_image_create_bits(PIXMAN_a8, 1, 1, &pi.color, 4);
			pixman_image_set_repeat(pi.mask, PIXMAN_REPEAT_NORMAL);
			pi.bits = pixman_image_get_data(pi.mask);
			pi.op = op;

			if (sigtrap_get() == 0) {
				count = region_num_rects(&clip);
				extents = region_rects(&clip);
				while (count--) {
					int16_t y1 = pixman_fixed_to_int(t->top);
					uint16_t fy1 = pixman_fixed_frac(t->top);
					int16_t y2 = pixman_fixed_to_int(t->bottom);
					uint16_t fy2 = pixman_fixed_frac(t->bottom);

					if (y1 < extents->y1)
						y1 = extents->y1, fy1 = 0;
					if (y2 > extents->y2)
						y2 = extents->y2, fy2 = 0;
					if (y1 < y2) {
						if (fy1) {
							pixmask_unaligned_box_row(&pi, extents, t, y1, 1,
										  SAMPLES_Y - grid_coverage(SAMPLES_Y, fy1));
							y1++;
						}

						if (y2 > y1)
							pixmask_unaligned_box_row(&pi, extents, t, y1, y2 - y1,
										  SAMPLES_Y);

						if (fy2)
							pixmask_unaligned_box_row(&pi, extents, t, y2, 1,
										  grid_coverage(SAMPLES_Y, fy2));
					} else if (y1 == y2 && fy2 > fy1) {
						pixmask_unaligned_box_row(&pi, extents, t, y1, 1,
									  grid_coverage(SAMPLES_Y, fy2) - grid_coverage(SAMPLES_Y, fy1));
					}
					extents++;
				}
				sigtrap_put();
			}

			pixman_image_unref(pi.image);
			pixman_image_unref(pi.source);
			pixman_image_unref(pi.mask);
		} else {
			struct rectilinear_inplace_thread thread[num_threads];
			int i, y, dy;


			thread[0].trap = t;
			thread[0].dst = image_from_pict(dst, false, &thread[0].dx, &thread[0].dy);
			thread[0].src = image_from_pict(src, false, &thread[0].sx, &thread[0].sy);
			thread[0].sx += src_x;
			thread[0].sy += src_y;

			thread[0].clip = &clip;
			thread[0].op = op;

			y = clip.extents.y1;
			dy = (clip.extents.y2 - clip.extents.y1 + num_threads - 1) / num_threads;
			num_threads = (clip.extents.y2 - clip.extents.y1 + dy - 1) / dy;

			if (sigtrap_get() == 0) {
				for (i = 1; i < num_threads; i++) {
					thread[i] = thread[0];
					thread[i].y1 = y;
					thread[i].y2 = y += dy;
					sna_threads_run(i, rectilinear_inplace_thread, &thread[i]);
				}

				assert(y < clip.extents.y2);
				thread[0].y1 = y;
				thread[0].y2 = clip.extents.y2;
				rectilinear_inplace_thread(&thread[0]);

				sna_threads_wait();
				sigtrap_put();
			} else
				sna_threads_kill();

			pixman_image_unref(thread[0].dst);
			pixman_image_unref(thread[0].src);
		}

		RegionUninit(&clip);
	} while (--n && t++);

	return true;
}

static bool
composite_unaligned_boxes_fallback(struct sna *sna,
				   CARD8 op,
				   PicturePtr src,
				   PicturePtr dst,
				   INT16 src_x, INT16 src_y,
				   int ntrap, const xTrapezoid *traps,
				   bool force_fallback)
{
	ScreenPtr screen = dst->pDrawable->pScreen;
	uint32_t color;
	int16_t dst_x, dst_y;
	int16_t dx, dy;
	int n;

	if (sna_picture_is_solid(src, &color) &&
	    composite_unaligned_boxes_inplace__solid(sna, op, color, dst,
						     ntrap, traps,
						     force_fallback))
		return true;

	if (composite_unaligned_boxes_inplace(sna, op, src, src_x, src_y,
					      dst, ntrap, traps,
					      force_fallback))
		return true;

	trapezoid_origin(&traps[0].left, &dst_x, &dst_y);
	dx = dst->pDrawable->x;
	dy = dst->pDrawable->y;
	for (n = 0; n < ntrap; n++) {
		const xTrapezoid *t = &traps[n];
		PixmapPtr scratch;
		PicturePtr mask;
		BoxRec extents;
		int error;
		int y1, y2;

		extents.x1 = pixman_fixed_to_int(t->left.p1.x);
		extents.x2 = pixman_fixed_to_int(t->right.p1.x + pixman_fixed_1_minus_e);
		extents.y1 = pixman_fixed_to_int(t->top);
		extents.y2 = pixman_fixed_to_int(t->bottom + pixman_fixed_1_minus_e);

		if (!sna_compute_composite_extents(&extents,
						   src, NULL, dst,
						   src_x, src_y,
						   0, 0,
						   extents.x1, extents.y1,
						   extents.x2 - extents.x1,
						   extents.y2 - extents.y1))
			continue;

		if (force_fallback)
			scratch = sna_pixmap_create_unattached(screen,
							       extents.x2 - extents.x1,
							       extents.y2 - extents.y1,
							       8);
		else
			scratch = sna_pixmap_create_upload(screen,
							   extents.x2 - extents.x1,
							   extents.y2 - extents.y1,
							   8, KGEM_BUFFER_WRITE_INPLACE);
		if (!scratch)
			continue;

		memset(scratch->devPrivate.ptr, 0xff,
		       scratch->devKind * (extents.y2 - extents.y1));

		extents.x1 -= dx;
		extents.x2 -= dx;
		extents.y1 -= dy;
		extents.y2 -= dy;

		y1 = pixman_fixed_to_int(t->top) - extents.y1;
		y2 = pixman_fixed_to_int(t->bottom) - extents.y1;

		if (y1 == y2) {
			blt_unaligned_box_row(scratch, &extents, t, y1, y1 + 1,
					      grid_coverage(SAMPLES_Y, t->bottom) - grid_coverage(SAMPLES_Y, t->top));
		} else {
			if (pixman_fixed_frac(t->top)) {
				blt_unaligned_box_row(scratch, &extents, t, y1, y1 + 1,
						      SAMPLES_Y - grid_coverage(SAMPLES_Y, t->top));
				y1++;
			}

			if (y2 > y1)
				blt_unaligned_box_row(scratch, &extents, t, y1, y2,
						      SAMPLES_Y);

			if (pixman_fixed_frac(t->bottom))
				blt_unaligned_box_row(scratch, &extents, t, y2, y2+1,
						      grid_coverage(SAMPLES_Y, t->bottom));
		}

		mask = CreatePicture(0, &scratch->drawable,
				     PictureMatchFormat(screen, 8, PICT_a8),
				     0, 0, serverClient, &error);
		if (mask) {
			CompositePicture(op, src, mask, dst,
					 src_x + extents.x1 - dst_x,
					 src_y + extents.y1 - dst_y,
					 0, 0,
					 extents.x1, extents.y1,
					 extents.x2 - extents.x1,
					 extents.y2 - extents.y1);
			FreePicture(mask, 0);
		}
		sna_pixmap_destroy(scratch);
	}

	return true;
}

bool
composite_unaligned_boxes(struct sna *sna,
			  CARD8 op,
			  PicturePtr src,
			  PicturePtr dst,
			  PictFormatPtr maskFormat,
			  INT16 src_x, INT16 src_y,
			  int ntrap, const xTrapezoid *traps,
			  bool force_fallback)
{
	BoxRec extents;
	struct sna_composite_spans_op tmp;
	struct sna_pixmap *priv;
	pixman_region16_t clip, *c;
	int16_t dst_x, dst_y;
	int dx, dy, n;

	if (NO_UNALIGNED_BOXES)
		return false;

	DBG(("%s: force_fallback=%d, mask=%x, n=%d, op=%d\n",
	     __FUNCTION__, force_fallback, maskFormat ? (int)maskFormat->format : 0, ntrap, op));

	/* need a span converter to handle overlapping traps */
	if (ntrap > 1 && maskFormat)
		return false;

	if (force_fallback ||
	    !sna->render.check_composite_spans(sna, op, src, dst, 0, 0,
					       COMPOSITE_SPANS_RECTILINEAR)) {
fallback:
		return composite_unaligned_boxes_fallback(sna, op, src, dst,
							  src_x, src_y,
							  ntrap, traps,
							  force_fallback);
	}

	trapezoid_origin(&traps[0].left, &dst_x, &dst_y);

	extents.x1 = pixman_fixed_to_int(traps[0].left.p1.x);
	extents.x2 = pixman_fixed_to_int(traps[0].right.p1.x + pixman_fixed_1_minus_e);
	extents.y1 = pixman_fixed_to_int(traps[0].top);
	extents.y2 = pixman_fixed_to_int(traps[0].bottom + pixman_fixed_1_minus_e);

	DBG(("%s: src=(%d, %d), dst=(%d, %d)\n",
	     __FUNCTION__, src_x, src_y, dst_x, dst_y));

	for (n = 1; n < ntrap; n++) {
		int x1 = pixman_fixed_to_int(traps[n].left.p1.x);
		int x2 = pixman_fixed_to_int(traps[n].right.p1.x + pixman_fixed_1_minus_e);
		int y1 = pixman_fixed_to_int(traps[n].top);
		int y2 = pixman_fixed_to_int(traps[n].bottom + pixman_fixed_1_minus_e);

		if (x1 < extents.x1)
			extents.x1 = x1;
		if (x2 > extents.x2)
			extents.x2 = x2;
		if (y1 < extents.y1)
			extents.y1 = y1;
		if (y2 > extents.y2)
			extents.y2 = y2;
	}

	DBG(("%s: extents (%d, %d), (%d, %d)\n", __FUNCTION__,
	     extents.x1, extents.y1, extents.x2, extents.y2));

	if (!sna_compute_composite_region(&clip,
					  src, NULL, dst,
					  src_x + extents.x1 - dst_x,
					  src_y + extents.y1 - dst_y,
					  0, 0,
					  extents.x1, extents.y1,
					  extents.x2 - extents.x1,
					  extents.y2 - extents.y1)) {
		DBG(("%s: trapezoids do not intersect drawable clips\n",
		     __FUNCTION__)) ;
		return true;
	}

	if (!sna->render.check_composite_spans(sna, op, src, dst,
					       clip.extents.x2 - clip.extents.x1,
					       clip.extents.y2 - clip.extents.y1,
					       COMPOSITE_SPANS_RECTILINEAR)) {
		DBG(("%s: fallback -- composite spans not supported\n",
		     __FUNCTION__));
		goto fallback;
	}

	c = NULL;
	if (extents.x2 - extents.x1 > clip.extents.x2 - clip.extents.x1 ||
	    extents.y2 - extents.y1 > clip.extents.y2 - clip.extents.y1) {
		DBG(("%s: forcing clip\n", __FUNCTION__));
		c = &clip;
	}

	extents = *RegionExtents(&clip);
	dx = dst->pDrawable->x;
	dy = dst->pDrawable->y;

	DBG(("%s: after clip -- extents (%d, %d), (%d, %d), delta=(%d, %d) src -> (%d, %d)\n",
	     __FUNCTION__,
	     extents.x1, extents.y1,
	     extents.x2, extents.y2,
	     dx, dy,
	     src_x + extents.x1 - dst_x - dx,
	     src_y + extents.y1 - dst_y - dy));

	switch (op) {
	case PictOpAdd:
	case PictOpOver:
		priv = sna_pixmap(get_drawable_pixmap(dst->pDrawable));
		assert(priv != NULL);
		if (priv->clear && priv->clear_color == 0) {
			DBG(("%s: converting %d to PictOpSrc\n",
			     __FUNCTION__, op));
			op = PictOpSrc;
		}
		break;
	case PictOpIn:
		priv = sna_pixmap(get_drawable_pixmap(dst->pDrawable));
		assert(priv != NULL);
		if (priv->clear && priv->clear_color == 0) {
			DBG(("%s: clear destination using In, skipping\n",
			     __FUNCTION__));
			return true;
		}
		break;
	}

	if (!sna->render.composite_spans(sna, op, src, dst,
					 src_x + extents.x1 - dst_x - dx,
					 src_y + extents.y1 - dst_y - dy,
					 extents.x1,  extents.y1,
					 extents.x2 - extents.x1,
					 extents.y2 - extents.y1,
					 COMPOSITE_SPANS_RECTILINEAR,
					 memset(&tmp, 0, sizeof(tmp)))) {
		DBG(("%s: composite spans render op not supported\n",
		     __FUNCTION__));
		REGION_UNINIT(NULL, &clip);
		goto fallback;
	}

	for (n = 0; n < ntrap; n++)
		composite_unaligned_trap(sna, &tmp, &traps[n], dx, dy, c);
	tmp.done(sna, &tmp);
	REGION_UNINIT(NULL, &clip);
	return true;
}
