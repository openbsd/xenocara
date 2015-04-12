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

inline static xFixed
line_x_for_y(const xLineFixed *l, xFixed y, bool ceil)
{
	xFixed_32_32 ex = (xFixed_32_32)(y - l->p1.y) * (l->p2.x - l->p1.x);
	xFixed d = l->p2.y - l->p1.y;

	if (ceil)
		ex += (d - 1);

	return l->p1.x + (xFixed) (ex / d);
}

bool trapezoids_bounds(int n, const xTrapezoid *t, BoxPtr box)
{
	xFixed x1, y1, x2, y2;

	/* XXX need 33 bits... */
	x1 = y1 = INT_MAX / 2;
	x2 = y2 = INT_MIN / 2;

	do {
		xFixed fx1, fx2, v;

		if (!xTrapezoidValid(t))
			continue;

		if (t->top < y1)
			y1 = t->top;
		if (t->bottom > y2)
			y2 = t->bottom;

		if (((t->left.p1.x - x1) | (t->left.p2.x - x1)) < 0) {
			if (pixman_fixed_floor(t->left.p1.x) == pixman_fixed_floor(t->left.p2.x)) {
				x1 = pixman_fixed_floor(t->left.p1.x);
			} else {
				if (t->left.p1.y == t->top)
					fx1 = t->left.p1.x;
				else
					fx1 = line_x_for_y(&t->left, t->top, false);

				if (t->left.p2.y == t->bottom)
					fx2 = t->left.p2.x;
				else
					fx2 = line_x_for_y(&t->left, t->bottom, false);

				v = min(fx1, fx2);
				if (v < x1)
					x1 = pixman_fixed_floor(v);
			}
		}

		if (((x2 - t->right.p1.x) | (x2 - t->right.p2.x)) < 0) {
			if (pixman_fixed_floor(t->right.p1.x) == pixman_fixed_floor(t->right.p2.x)) {
				x2 = pixman_fixed_ceil(t->right.p1.x);
			} else {
				if (t->right.p1.y == t->top)
					fx1 = t->right.p1.x;
				else
					fx1 = line_x_for_y(&t->right, t->top, true);

				if (t->right.p2.y == t->bottom)
					fx2 = t->right.p2.x;
				else
					fx2 = line_x_for_y(&t->right, t->bottom, true);

				v = max(fx1, fx2);
				if (v > x2)
					x2 = pixman_fixed_ceil(v);
			}
		}
	} while (t++, --n);

	box->x1 = pixman_fixed_to_int(x1);
	box->x2 = pixman_fixed_to_int(x2);
	box->y1 = pixman_fixed_integer_floor(y1);
	box->y2 = pixman_fixed_integer_ceil(y2);

	return box->x2 > box->x1 && box->y2 > box->y1;
}

static bool
trapezoids_inplace_fallback(struct sna *sna,
			    CARD8 op,
			    PicturePtr src, PicturePtr dst, PictFormatPtr mask,
			    int ntrap, xTrapezoid *traps)
{
	pixman_image_t *image;
	BoxRec box;
	uint32_t color;
	int dx, dy;

	if (op != PictOpAdd)
		return false;

	if (is_mono(dst, mask)) {
		if (dst->format != PICT_a1)
			return false;
	} else {
		if (dst->format != PICT_a8)
			return false;
	}

	if (!sna_picture_is_solid(src, &color) || (color >> 24) != 0xff) {
		DBG(("%s: not an opaque solid source\n", __FUNCTION__));
		return false;
	}

	box.x1 = dst->pDrawable->x;
	box.y1 = dst->pDrawable->y;
	box.x2 = dst->pDrawable->width;
	box.y2 = dst->pDrawable->height;
	if (pixman_region_contains_rectangle(dst->pCompositeClip,
					     &box) != PIXMAN_REGION_IN) {
		DBG(("%s: requires clipping, drawable (%d,%d), (%d, %d), clip (%d, %d), (%d, %d)\n", __FUNCTION__,
		     box.x1, box.y1, box.x2, box.y2,
		     dst->pCompositeClip->extents.x1,
		     dst->pCompositeClip->extents.y1,
		     dst->pCompositeClip->extents.x2,
		     dst->pCompositeClip->extents.y2));
		return false;
	}

	if (is_gpu(sna, dst->pDrawable, PREFER_GPU_SPANS)) {
		DBG(("%s: not performing inplace as dst is already on the GPU\n",
		     __FUNCTION__));
		return false;
	}

	DBG(("%s\n", __FUNCTION__));

	image = NULL;
	if (sna_drawable_move_to_cpu(dst->pDrawable, MOVE_READ | MOVE_WRITE))
		image = image_from_pict(dst, false, &dx, &dy);
	if (image) {
		dx += dst->pDrawable->x;
		dy += dst->pDrawable->y;

		if (sigtrap_get() == 0) {
			for (; ntrap; ntrap--, traps++)
				if (xTrapezoidValid(traps))
					pixman_rasterize_trapezoid(image,
								   (pixman_trapezoid_t *)traps,
								   dx, dy);
			sigtrap_put();
		}

		pixman_image_unref(image);
	}

	return true;
}

struct rasterize_traps_thread {
	xTrapezoid *traps;
	char *ptr;
	int stride;
	BoxRec bounds;
	pixman_format_code_t format;
	int ntrap;
};

static void rasterize_traps_thread(void *arg)
{
	struct rasterize_traps_thread *thread = arg;
	pixman_image_t *image;
	int width, height, n;

	width = thread->bounds.x2 - thread->bounds.x1;
	height = thread->bounds.y2 - thread->bounds.y1;

	memset(thread->ptr, 0, thread->stride*height);
	if (PIXMAN_FORMAT_DEPTH(thread->format) < 8)
		image = pixman_image_create_bits(thread->format,
						 width, height,
						 NULL, 0);
	else
		image = pixman_image_create_bits(thread->format,
						 width, height,
						 (uint32_t *)thread->ptr,
						 thread->stride);
	if (image == NULL)
		return;

	for (n = 0; n < thread->ntrap; n++)
		if (xTrapezoidValid(&thread->traps[n]))
			pixman_rasterize_trapezoid(image,
						   (pixman_trapezoid_t *)&thread->traps[n],
						   -thread->bounds.x1, -thread->bounds.y1);

	if (PIXMAN_FORMAT_DEPTH(thread->format) < 8) {
		pixman_image_t *a8;

		a8 = pixman_image_create_bits(PIXMAN_a8,
					      width, height,
					      (uint32_t *)thread->ptr,
					      thread->stride);
		if (a8) {
			pixman_image_composite(PIXMAN_OP_SRC,
					       image, NULL, a8,
					       0, 0,
					       0, 0,
					       0, 0,
					       width, height);
			pixman_image_unref(a8);
		}
	}

	pixman_image_unref(image);
}

static void
trapezoids_fallback(struct sna *sna,
		    CARD8 op, PicturePtr src, PicturePtr dst,
		    PictFormatPtr maskFormat, INT16 xSrc, INT16 ySrc,
		    int ntrap, xTrapezoid * traps)
{
	ScreenPtr screen = dst->pDrawable->pScreen;

	if (maskFormat) {
		PixmapPtr scratch;
		PicturePtr mask;
		INT16 dst_x, dst_y;
		BoxRec bounds;
		int width, height, depth;
		pixman_image_t *image;
		pixman_format_code_t format;
		int error;

		trapezoid_origin(&traps[0].left, &dst_x, &dst_y);

		if (!trapezoids_bounds(ntrap, traps, &bounds))
			return;

		DBG(("%s: bounds (%d, %d), (%d, %d)\n", __FUNCTION__,
		     bounds.x1, bounds.y1, bounds.x2, bounds.y2));

		if (!sna_compute_composite_extents(&bounds,
						   src, NULL, dst,
						   xSrc, ySrc,
						   0, 0,
						   bounds.x1, bounds.y1,
						   bounds.x2 - bounds.x1,
						   bounds.y2 - bounds.y1))
			return;

		DBG(("%s: extents (%d, %d), (%d, %d)\n", __FUNCTION__,
		     bounds.x1, bounds.y1, bounds.x2, bounds.y2));

		width  = bounds.x2 - bounds.x1;
		height = bounds.y2 - bounds.y1;
		bounds.x1 -= dst->pDrawable->x;
		bounds.y1 -= dst->pDrawable->y;
		bounds.x2 -= dst->pDrawable->x;
		bounds.y2 -= dst->pDrawable->y;
		depth = maskFormat->depth;
		if (depth == 1) {
			format = PIXMAN_a1;
		} else if (depth <= 4) {
			format = PIXMAN_a4;
			depth = 4;
		} else
			format = PIXMAN_a8;

		DBG(("%s: mask (%dx%d) depth=%d, format=%08x\n",
		     __FUNCTION__, width, height, depth, format));
		if (is_gpu(sna, dst->pDrawable, PREFER_GPU_RENDER) ||
		    picture_is_gpu(sna, src, PREFER_GPU_RENDER)) {
			int num_threads;

			scratch = sna_pixmap_create_upload(screen,
							   width, height, 8,
							   KGEM_BUFFER_WRITE);
			if (!scratch)
				return;

			num_threads = sna_use_threads(width, height, 8);
			if (num_threads == 1) {
				if (depth < 8) {
					image = pixman_image_create_bits(format, width, height,
									 NULL, 0);
				} else {
					memset(scratch->devPrivate.ptr, 0, scratch->devKind*height);

					image = pixman_image_create_bits(format, width, height,
									 scratch->devPrivate.ptr,
									 scratch->devKind);
				}
				if (image) {
					for (; ntrap; ntrap--, traps++)
						if (xTrapezoidValid(traps))
							pixman_rasterize_trapezoid(image,
										   (pixman_trapezoid_t *)traps,
										   -bounds.x1, -bounds.y1);
					if (depth < 8) {
						pixman_image_t *a8;

						a8 = pixman_image_create_bits(PIXMAN_a8, width, height,
									      scratch->devPrivate.ptr,
									      scratch->devKind);
						if (a8) {
							pixman_image_composite(PIXMAN_OP_SRC,
									       image, NULL, a8,
									       0, 0,
									       0, 0,
									       0, 0,
									       width, height);
							format = PIXMAN_a8;
							depth = 8;
							pixman_image_unref(a8);
						}
					}

					pixman_image_unref(image);
				}
				if (format != PIXMAN_a8) {
					sna_pixmap_destroy(scratch);
					return;
				}
			} else {
				struct rasterize_traps_thread threads[num_threads];
				int y, dy, n;

				threads[0].ptr = scratch->devPrivate.ptr;
				threads[0].stride = scratch->devKind;
				threads[0].traps = traps;
				threads[0].ntrap = ntrap;
				threads[0].bounds = bounds;
				threads[0].format = format;

				y = bounds.y1;
				dy = (height + num_threads - 1) / num_threads;
				num_threads -= (num_threads-1) * dy >= bounds.y2 - bounds.y1;

				if (sigtrap_get() == 0) {
					for (n = 1; n < num_threads; n++) {
						threads[n] = threads[0];
						threads[n].ptr += (y - bounds.y1) * threads[n].stride;
						threads[n].bounds.y1 = y;
						threads[n].bounds.y2 = y += dy;

						sna_threads_run(n, rasterize_traps_thread, &threads[n]);
					}

					assert(y < threads[0].bounds.y2);
					threads[0].ptr += (y - bounds.y1) * threads[0].stride;
					threads[0].bounds.y1 = y;
					rasterize_traps_thread(&threads[0]);

					sna_threads_wait();
					sigtrap_put();
				} else
					sna_threads_kill();

				format = PIXMAN_a8;
				depth = 8;
			}
		} else {
			scratch = sna_pixmap_create_unattached(screen,
							       width, height,
							       depth);
			if (!scratch)
				return;

			memset(scratch->devPrivate.ptr, 0, scratch->devKind*height);
			image = pixman_image_create_bits(format, width, height,
							 scratch->devPrivate.ptr,
							 scratch->devKind);
			if (image) {
				for (; ntrap; ntrap--, traps++)
					if (xTrapezoidValid(traps))
						pixman_rasterize_trapezoid(image,
									   (pixman_trapezoid_t *)traps,
									   -bounds.x1, -bounds.y1);
				pixman_image_unref(image);
			}
		}

		mask = CreatePicture(0, &scratch->drawable,
				     PictureMatchFormat(screen, depth, format),
				     0, 0, serverClient, &error);
		if (mask) {
			CompositePicture(op, src, mask, dst,
					 xSrc + bounds.x1 - dst_x,
					 ySrc + bounds.y1 - dst_y,
					 0, 0,
					 bounds.x1, bounds.y1,
					 width, height);
			FreePicture(mask, 0);
		}
		sna_pixmap_destroy(scratch);
	} else {
		if (dst->polyEdge == PolyEdgeSharp)
			maskFormat = PictureMatchFormat(screen, 1, PICT_a1);
		else
			maskFormat = PictureMatchFormat(screen, 8, PICT_a8);

		for (; ntrap; ntrap--, traps++)
			trapezoids_fallback(sna, op,
					    src, dst, maskFormat,
					    xSrc, ySrc, 1, traps);
	}
}

static bool
trapezoid_spans_maybe_inplace(struct sna *sna,
			      CARD8 op, PicturePtr src, PicturePtr dst,
			      PictFormatPtr maskFormat)
{
	struct sna_pixmap *priv;

	if (NO_SCAN_CONVERTER)
		return false;

	if (dst->alphaMap)
		return false;
	if (is_mono(dst, maskFormat))
		goto out;

	switch ((int)dst->format) {
	case PICT_a8:
		if (!sna_picture_is_solid(src, NULL))
			return false;

		switch (op) {
		case PictOpIn:
		case PictOpAdd:
		case PictOpSrc:
			break;
		default:
			return false;
		}
		break;

	case PICT_x8r8g8b8:
	case PICT_a8r8g8b8:
		if (picture_is_gpu(sna, src, 0))
			return false;

		switch (op) {
		case PictOpOver:
		case PictOpAdd:
		case PictOpOutReverse:
			break;
		case PictOpSrc:
			if (sna_picture_is_solid(src, NULL))
				break;

			if (!sna_drawable_is_clear(dst->pDrawable))
				return false;
			break;
		default:
			return false;
		}
		break;
	default:
		return false;
	}

out:
	priv = sna_pixmap_from_drawable(dst->pDrawable);
	if (priv == NULL) {
		DBG(("%s? yes -- unattached\n", __FUNCTION__));
		return true;
	}

	if (priv->cpu_bo && kgem_bo_is_busy(priv->cpu_bo)) {
		DBG(("%s? no -- CPU bo is busy\n", __FUNCTION__));
		return false;
	}

	if (DAMAGE_IS_ALL(priv->cpu_damage) || priv->gpu_damage == NULL) {
		DBG(("%s? yes -- damaged on CPU only (all? %d)\n", __FUNCTION__, DAMAGE_IS_ALL(priv->cpu_damage)));
		return true;
	}

	if (priv->clear) {
		DBG(("%s? clear, %s\n", __FUNCTION__,
		     dst->pDrawable->width <= TOR_INPLACE_SIZE ? "yes" : "no"));
		return dst->pDrawable->width <= TOR_INPLACE_SIZE;
	}

	if (kgem_bo_is_busy(priv->gpu_bo)) {
		DBG(("%s? no, GPU bo is busy\n", __FUNCTION__));
		return false;
	}

	if (priv->cpu_damage) {
		DBG(("%s? yes, idle GPU bo and damage on idle CPU\n", __FUNCTION__));
		return true;
	}

	DBG(("%s? small enough? %s\n", __FUNCTION__,
	     dst->pDrawable->width <= TOR_INPLACE_SIZE ? "yes" : "no"));
	return dst->pDrawable->width <= TOR_INPLACE_SIZE;
}

void
sna_composite_trapezoids(CARD8 op,
			 PicturePtr src,
			 PicturePtr dst,
			 PictFormatPtr maskFormat,
			 INT16 xSrc, INT16 ySrc,
			 int ntrap, xTrapezoid *traps)
{
	PixmapPtr pixmap = get_drawable_pixmap(dst->pDrawable);
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;
	bool force_fallback = false;
	bool rectilinear, pixel_aligned;
	unsigned flags;
	int n;

	DBG(("%s(op=%d, src=(%d, %d), mask=%08x, ntrap=%d)\n", __FUNCTION__,
	     op, xSrc, ySrc,
	     maskFormat ? (int)maskFormat->format : 0,
	     ntrap));

	if (ntrap == 0)
		return;

	if (NO_ACCEL)
		goto force_fallback;

	if (FORCE_FALLBACK > 0)
		goto force_fallback;

	if (wedged(sna)) {
		DBG(("%s: fallback -- wedged\n", __FUNCTION__));
		goto force_fallback;
	}

	if (dst->alphaMap) {
		DBG(("%s: fallback -- dst alpha map\n", __FUNCTION__));
		goto force_fallback;
	}

	priv = sna_pixmap(pixmap);
	if (priv == NULL) {
		DBG(("%s: fallback -- dst is unattached\n", __FUNCTION__));
		goto force_fallback;
	}

	if (FORCE_FALLBACK == 0 &&
	    !is_gpu_dst(priv) && !picture_is_gpu(sna, src, 0) && untransformed(src)) {
		DBG(("%s: force fallbacks -- (!gpu dst, %dx%d? %d) && (src-is-cpu? %d && untransformed? %d)\n",
		     __FUNCTION__, dst->pDrawable->width, dst->pDrawable->height,
		     !is_gpu_dst(priv), !picture_is_gpu(sna, src, 0), untransformed(src)));

force_fallback:
		force_fallback = true;
	}

	/* scan through for fast rectangles */
	rectilinear = pixel_aligned = true;
	if (is_mono(dst, maskFormat)) {
		for (n = 0; n < ntrap && rectilinear; n++) {
			int lx1 = pixman_fixed_to_int(traps[n].left.p1.x + pixman_fixed_1_minus_e/2);
			int lx2 = pixman_fixed_to_int(traps[n].left.p2.x + pixman_fixed_1_minus_e/2);
			int rx1 = pixman_fixed_to_int(traps[n].right.p1.x + pixman_fixed_1_minus_e/2);
			int rx2 = pixman_fixed_to_int(traps[n].right.p2.x + pixman_fixed_1_minus_e/2);
			rectilinear &= lx1 == lx2 && rx1 == rx2;
		}
	} else if (dst->polyMode != PolyModePrecise) {
		for (n = 0; n < ntrap && rectilinear; n++) {
			int lx1 = pixman_fixed_to_fast(traps[n].left.p1.x);
			int lx2 = pixman_fixed_to_fast(traps[n].left.p2.x);
			int rx1 = pixman_fixed_to_fast(traps[n].right.p1.x);
			int rx2 = pixman_fixed_to_fast(traps[n].right.p2.x);
			int top = pixman_fixed_to_fast(traps[n].top);
			int bot = pixman_fixed_to_fast(traps[n].bottom);

			rectilinear &= lx1 == lx2 && rx1 == rx2;
			pixel_aligned &= ((top | bot | lx1 | lx2 | rx1 | rx2) & FAST_SAMPLES_mask) == 0;
		}
	} else {
		for (n = 0; n < ntrap && rectilinear; n++) {
			rectilinear &=
				traps[n].left.p1.x == traps[n].left.p2.x &&
				traps[n].right.p1.x == traps[n].right.p2.x;
			pixel_aligned &=
				((traps[n].top | traps[n].bottom |
				  traps[n].left.p1.x | traps[n].left.p2.x |
				  traps[n].right.p1.x | traps[n].right.p2.x)
				 & pixman_fixed_1_minus_e) == 0;
		}
	}

	DBG(("%s: rectilinear? %d, pixel-aligned? %d, mono? %d precise? %d\n",
	     __FUNCTION__, rectilinear, pixel_aligned,
	     is_mono(dst, maskFormat), is_precise(dst, maskFormat)));

	flags = 0;
	if (rectilinear) {
		if (pixel_aligned) {
			if (composite_aligned_boxes(sna, op, src, dst,
						    maskFormat,
						    xSrc, ySrc,
						    ntrap, traps,
						    force_fallback))
			    return;
		} else {
			if (composite_unaligned_boxes(sna, op, src, dst,
						      maskFormat,
						      xSrc, ySrc,
						      ntrap, traps,
						      force_fallback))
				return;
		}
		flags |= COMPOSITE_SPANS_RECTILINEAR;
	}

	if (force_fallback)
		goto fallback;

	if (is_mono(dst, maskFormat) &&
	    mono_trapezoids_span_converter(sna, op, src, dst,
					   xSrc, ySrc,
					   ntrap, traps))
		return;

	if (trapezoid_spans_maybe_inplace(sna, op, src, dst, maskFormat)) {
		flags |= COMPOSITE_SPANS_INPLACE_HINT;
		if (trapezoid_span_inplace(sna, op, src, dst, maskFormat, flags,
					   xSrc, ySrc, ntrap, traps,
					   false))
			return;
	}

	if (trapezoid_span_converter(sna, op, src, dst, maskFormat, flags,
				     xSrc, ySrc, ntrap, traps))
		return;

	if (trapezoid_span_inplace(sna, op, src, dst, maskFormat, flags,
				   xSrc, ySrc, ntrap, traps,
				   false))
		return;

	if (trapezoid_mask_converter(op, src, dst, maskFormat, flags,
				     xSrc, ySrc, ntrap, traps))
		return;

fallback:
	if (trapezoid_span_inplace(sna, op, src, dst, maskFormat, flags,
				   xSrc, ySrc, ntrap, traps,
				   true))
		return;

	if (trapezoid_span_fallback(op, src, dst, maskFormat, flags,
				    xSrc, ySrc, ntrap, traps))
		return;

	if (trapezoids_inplace_fallback(sna, op, src, dst, maskFormat,
					ntrap, traps))
		return;

	DBG(("%s: fallback mask=%08x, ntrap=%d\n", __FUNCTION__,
	     maskFormat ? (unsigned)maskFormat->format : 0, ntrap));
	trapezoids_fallback(sna, op, src, dst, maskFormat,
			    xSrc, ySrc,
			    ntrap, traps);
}

static void mark_damaged(PixmapPtr pixmap, struct sna_pixmap *priv,
			 BoxPtr box, int16_t x, int16_t y)
{
	box->x1 += x; box->x2 += x;
	box->y1 += y; box->y2 += y;
	if (box->x1 <= 0 && box->y1 <= 0 &&
	    box->x2 >= pixmap->drawable.width &&
	    box->y2 >= pixmap->drawable.height) {
		sna_damage_destroy(&priv->cpu_damage);
		sna_damage_all(&priv->gpu_damage, pixmap);
		list_del(&priv->flush_list);
	} else {
		sna_damage_add_box(&priv->gpu_damage, box);
		sna_damage_subtract_box(&priv->cpu_damage, box);
	}
}

static bool
trap_upload(PicturePtr picture,
	    INT16 x, INT16 y,
	    int ntrap, xTrap *trap)
{
	ScreenPtr screen = picture->pDrawable->pScreen;
	struct sna *sna = to_sna_from_screen(screen);
	PixmapPtr pixmap = get_drawable_pixmap(picture->pDrawable);
	PixmapPtr scratch;
	struct sna_pixmap *priv;
	BoxRec extents;
	pixman_image_t *image;
	int width, height, depth;
	int n;

	priv = sna_pixmap_move_to_gpu(pixmap, MOVE_READ | MOVE_WRITE);
	if (priv == NULL)
		return false;

	extents = *RegionExtents(picture->pCompositeClip);
	for (n = 0; n < ntrap; n++) {
		int v;

		v = x + pixman_fixed_integer_floor (MIN(trap[n].top.l, trap[n].bot.l));
		if (v < extents.x1)
			extents.x1 = v;

		v = x + pixman_fixed_integer_ceil (MAX(trap[n].top.r, trap[n].bot.r));
		if (v > extents.x2)
			extents.x2 = v;

		v = y + pixman_fixed_integer_floor (trap[n].top.y);
		if (v < extents.y1)
			extents.y1 = v;

		v = y + pixman_fixed_integer_ceil (trap[n].bot.y);
		if (v > extents.y2)
			extents.y2 = v;
	}

	DBG(("%s: extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__, extents.x1, extents.y1, extents.x2, extents.y2));

	width  = extents.x2 - extents.x1;
	height = extents.y2 - extents.y1;
	depth = picture->pDrawable->depth;

	DBG(("%s: tmp (%dx%d) depth=%d\n",
	     __FUNCTION__, width, height, depth));
	scratch = sna_pixmap_create_upload(screen,
					   width, height, depth,
					   KGEM_BUFFER_WRITE);
	if (!scratch)
		return true;

	memset(scratch->devPrivate.ptr, 0, scratch->devKind*height);
	image = pixman_image_create_bits((pixman_format_code_t)picture->format,
					 width, height,
					 scratch->devPrivate.ptr,
					 scratch->devKind);
	if (image) {
		pixman_add_traps (image, -extents.x1, -extents.y1,
				  ntrap, (pixman_trap_t *)trap);

		pixman_image_unref(image);
	}

	/* XXX clip boxes */
	get_drawable_deltas(picture->pDrawable, pixmap, &x, &y);
	sna->render.copy_boxes(sna, GXcopy,
			       &scratch->drawable, __sna_pixmap_get_bo(scratch), -extents.x1, -extents.x1,
			       &pixmap->drawable, priv->gpu_bo, x, y,
			       &extents, 1, 0);
	mark_damaged(pixmap, priv, &extents, x, y);

	sna_pixmap_destroy(scratch);
	return true;
}

void
sna_add_traps(PicturePtr picture, INT16 x, INT16 y, int n, xTrap *t)
{
	struct sna *sna;

	DBG(("%s (%d, %d) x %d\n", __FUNCTION__, x, y, n));

	sna = to_sna_from_drawable(picture->pDrawable);
	if (is_gpu(sna, picture->pDrawable, PREFER_GPU_SPANS)) {
		if (trap_span_converter(sna, picture, x, y, n, t))
			return;
	}

	if (is_gpu(sna, picture->pDrawable, PREFER_GPU_RENDER)) {
		if (trap_mask_converter(sna, picture, x, y, n, t))
			return;

		if (trap_upload(picture, x, y, n, t))
			return;
	}

	DBG(("%s -- fallback\n", __FUNCTION__));
	if (sna_drawable_move_to_cpu(picture->pDrawable,
				     MOVE_READ | MOVE_WRITE)) {
		pixman_image_t *image;
		int dx, dy;

		if (!(image = image_from_pict(picture, false, &dx, &dy)))
			return;

		if (sigtrap_get() == 0) {
			pixman_add_traps(image, x + dx, y + dy, n, (pixman_trap_t *)t);
			sigtrap_put();
		}

		free_pixman_pict(picture, image);
	}
}

#if HAS_PIXMAN_TRIANGLES
static void
triangles_fallback(CARD8 op,
		   PicturePtr src,
		   PicturePtr dst,
		   PictFormatPtr maskFormat,
		   INT16 xSrc, INT16 ySrc,
		   int n, xTriangle *tri)
{
	ScreenPtr screen = dst->pDrawable->pScreen;

	DBG(("%s op=%d, count=%d\n", __FUNCTION__, op, n));

	if (maskFormat) {
		PixmapPtr scratch;
		PicturePtr mask;
		INT16 dst_x, dst_y;
		BoxRec bounds;
		int width, height, depth;
		pixman_image_t *image;
		pixman_format_code_t format;
		int error;

		dst_x = pixman_fixed_to_int(tri[0].p1.x);
		dst_y = pixman_fixed_to_int(tri[0].p1.y);

		miTriangleBounds(n, tri, &bounds);
		DBG(("%s: bounds (%d, %d), (%d, %d)\n",
		     __FUNCTION__, bounds.x1, bounds.y1, bounds.x2, bounds.y2));

		if (bounds.y1 >= bounds.y2 || bounds.x1 >= bounds.x2)
			return;

		if (!sna_compute_composite_extents(&bounds,
						   src, NULL, dst,
						   xSrc, ySrc,
						   0, 0,
						   bounds.x1, bounds.y1,
						   bounds.x2 - bounds.x1,
						   bounds.y2 - bounds.y1))
			return;

		DBG(("%s: extents (%d, %d), (%d, %d)\n",
		     __FUNCTION__, bounds.x1, bounds.y1, bounds.x2, bounds.y2));

		width  = bounds.x2 - bounds.x1;
		height = bounds.y2 - bounds.y1;
		bounds.x1 -= dst->pDrawable->x;
		bounds.y1 -= dst->pDrawable->y;
		depth = maskFormat->depth;
		format = maskFormat->format | (BitsPerPixel(depth) << 24);

		DBG(("%s: mask (%dx%d) depth=%d, format=%08x\n",
		     __FUNCTION__, width, height, depth, format));
		scratch = sna_pixmap_create_upload(screen,
						   width, height, depth,
						   KGEM_BUFFER_WRITE);
		if (!scratch)
			return;

		memset(scratch->devPrivate.ptr, 0, (size_t)scratch->devKind*height);
		image = pixman_image_create_bits(format, width, height,
						 scratch->devPrivate.ptr,
						 scratch->devKind);
		if (image) {
			pixman_add_triangles(image,
					     -bounds.x1, -bounds.y1,
					     n, (pixman_triangle_t *)tri);
			pixman_image_unref(image);
		}

		mask = CreatePicture(0, &scratch->drawable,
				     PictureMatchFormat(screen, depth, format),
				     0, 0, serverClient, &error);
		if (mask) {
			CompositePicture(op, src, mask, dst,
					 xSrc + bounds.x1 - dst_x,
					 ySrc + bounds.y1 - dst_y,
					 0, 0,
					 bounds.x1, bounds.y1,
					 width, height);
			FreePicture(mask, 0);
		}
		sna_pixmap_destroy(scratch);
	} else {
		if (dst->polyEdge == PolyEdgeSharp)
			maskFormat = PictureMatchFormat(screen, 1, PICT_a1);
		else
			maskFormat = PictureMatchFormat(screen, 8, PICT_a8);

		for (; n--; tri++)
			triangles_fallback(op,
					   src, dst, maskFormat,
					   xSrc, ySrc, 1, tri);
	}
}

void
sna_composite_triangles(CARD8 op,
			 PicturePtr src,
			 PicturePtr dst,
			 PictFormatPtr maskFormat,
			 INT16 xSrc, INT16 ySrc,
			 int n, xTriangle *tri)
{
	struct sna *sna = to_sna_from_drawable(dst->pDrawable);

	if (triangles_span_converter(sna, op, src, dst, maskFormat,
				     xSrc, ySrc,
				     n, tri))
		return;

	if (triangles_mask_converter(op, src, dst, maskFormat,
				     xSrc, ySrc,
				     n, tri))
		return;

	triangles_fallback(op, src, dst, maskFormat, xSrc, ySrc, n, tri);
}

static void
tristrip_fallback(CARD8 op,
		  PicturePtr src,
		  PicturePtr dst,
		  PictFormatPtr maskFormat,
		  INT16 xSrc, INT16 ySrc,
		  int n, xPointFixed *points)
{
	ScreenPtr screen = dst->pDrawable->pScreen;

	if (maskFormat) {
		PixmapPtr scratch;
		PicturePtr mask;
		INT16 dst_x, dst_y;
		BoxRec bounds;
		int width, height, depth;
		pixman_image_t *image;
		pixman_format_code_t format;
		int error;

		dst_x = pixman_fixed_to_int(points->x);
		dst_y = pixman_fixed_to_int(points->y);

		miPointFixedBounds(n, points, &bounds);
		DBG(("%s: bounds (%d, %d), (%d, %d)\n",
		     __FUNCTION__, bounds.x1, bounds.y1, bounds.x2, bounds.y2));

		if (bounds.y1 >= bounds.y2 || bounds.x1 >= bounds.x2)
			return;

		if (!sna_compute_composite_extents(&bounds,
						   src, NULL, dst,
						   xSrc, ySrc,
						   0, 0,
						   bounds.x1, bounds.y1,
						   bounds.x2 - bounds.x1,
						   bounds.y2 - bounds.y1))
			return;

		DBG(("%s: extents (%d, %d), (%d, %d)\n",
		     __FUNCTION__, bounds.x1, bounds.y1, bounds.x2, bounds.y2));

		width  = bounds.x2 - bounds.x1;
		height = bounds.y2 - bounds.y1;
		bounds.x1 -= dst->pDrawable->x;
		bounds.y1 -= dst->pDrawable->y;
		depth = maskFormat->depth;
		format = maskFormat->format | (BitsPerPixel(depth) << 24);

		DBG(("%s: mask (%dx%d) depth=%d, format=%08x\n",
		     __FUNCTION__, width, height, depth, format));
		scratch = sna_pixmap_create_upload(screen,
						   width, height, depth,
						   KGEM_BUFFER_WRITE);
		if (!scratch)
			return;

		memset(scratch->devPrivate.ptr, 0, scratch->devKind*height);
		image = pixman_image_create_bits(format, width, height,
						 scratch->devPrivate.ptr,
						 scratch->devKind);
		if (image) {
			xTriangle tri;
			xPointFixed *p[3] = { &tri.p1, &tri.p2, &tri.p3 };
			int i;

			*p[0] = points[0];
			*p[1] = points[1];
			*p[2] = points[2];
			pixman_add_triangles(image,
					     -bounds.x1, -bounds.y1,
					     1, (pixman_triangle_t *)&tri);
			for (i = 3; i < n; i++) {
				*p[i%3] = points[i];
				pixman_add_triangles(image,
						     -bounds.x1, -bounds.y1,
						     1, (pixman_triangle_t *)&tri);
			}
			pixman_image_unref(image);
		}

		mask = CreatePicture(0, &scratch->drawable,
				     PictureMatchFormat(screen, depth, format),
				     0, 0, serverClient, &error);
		if (mask) {
			CompositePicture(op, src, mask, dst,
					 xSrc + bounds.x1 - dst_x,
					 ySrc + bounds.y1 - dst_y,
					 0, 0,
					 bounds.x1, bounds.y1,
					 width, height);
			FreePicture(mask, 0);
		}
		sna_pixmap_destroy(scratch);
	} else {
		xTriangle tri;
		xPointFixed *p[3] = { &tri.p1, &tri.p2, &tri.p3 };
		int i;

		if (dst->polyEdge == PolyEdgeSharp)
			maskFormat = PictureMatchFormat(screen, 1, PICT_a1);
		else
			maskFormat = PictureMatchFormat(screen, 8, PICT_a8);

		*p[0] = points[0];
		*p[1] = points[1];
		*p[2] = points[2];
		triangles_fallback(op,
				   src, dst, maskFormat,
				   xSrc, ySrc, 1, &tri);
		for (i = 3; i < n; i++) {
			*p[i%3] = points[i];
			/* Should xSrc,ySrc be updated? */
			triangles_fallback(op,
					   src, dst, maskFormat,
					   xSrc, ySrc, 1, &tri);
		}
	}
}

void
sna_composite_tristrip(CARD8 op,
		       PicturePtr src,
		       PicturePtr dst,
		       PictFormatPtr maskFormat,
		       INT16 xSrc, INT16 ySrc,
		       int n, xPointFixed *points)
{
	struct sna *sna = to_sna_from_drawable(dst->pDrawable);

	if (tristrip_span_converter(sna, op, src, dst, maskFormat, xSrc, ySrc, n, points))
		return;

	tristrip_fallback(op, src, dst, maskFormat, xSrc, ySrc, n, points);
}

static void
trifan_fallback(CARD8 op,
		PicturePtr src,
		PicturePtr dst,
		PictFormatPtr maskFormat,
		INT16 xSrc, INT16 ySrc,
		int n, xPointFixed *points)
{
	ScreenPtr screen = dst->pDrawable->pScreen;

	if (maskFormat) {
		PixmapPtr scratch;
		PicturePtr mask;
		INT16 dst_x, dst_y;
		BoxRec bounds;
		int width, height, depth;
		pixman_image_t *image;
		pixman_format_code_t format;
		int error;

		dst_x = pixman_fixed_to_int(points->x);
		dst_y = pixman_fixed_to_int(points->y);

		miPointFixedBounds(n, points, &bounds);
		DBG(("%s: bounds (%d, %d), (%d, %d)\n",
		     __FUNCTION__, bounds.x1, bounds.y1, bounds.x2, bounds.y2));

		if (bounds.y1 >= bounds.y2 || bounds.x1 >= bounds.x2)
			return;

		if (!sna_compute_composite_extents(&bounds,
						   src, NULL, dst,
						   xSrc, ySrc,
						   0, 0,
						   bounds.x1, bounds.y1,
						   bounds.x2 - bounds.x1,
						   bounds.y2 - bounds.y1))
			return;

		DBG(("%s: extents (%d, %d), (%d, %d)\n",
		     __FUNCTION__, bounds.x1, bounds.y1, bounds.x2, bounds.y2));

		width  = bounds.x2 - bounds.x1;
		height = bounds.y2 - bounds.y1;
		bounds.x1 -= dst->pDrawable->x;
		bounds.y1 -= dst->pDrawable->y;
		depth = maskFormat->depth;
		format = maskFormat->format | (BitsPerPixel(depth) << 24);

		DBG(("%s: mask (%dx%d) depth=%d, format=%08x\n",
		     __FUNCTION__, width, height, depth, format));
		scratch = sna_pixmap_create_upload(screen,
						   width, height, depth,
						   KGEM_BUFFER_WRITE);
		if (!scratch)
			return;

		memset(scratch->devPrivate.ptr, 0, scratch->devKind*height);
		image = pixman_image_create_bits(format, width, height,
						 scratch->devPrivate.ptr,
						 scratch->devKind);
		if (image) {
			xTriangle tri;
			xPointFixed *p[3] = { &tri.p1, &tri.p2, &tri.p3 };
			int i;

			*p[0] = points[0];
			*p[1] = points[1];
			*p[2] = points[2];
			pixman_add_triangles(image,
					     -bounds.x1, -bounds.y1,
					     1, (pixman_triangle_t *)&tri);
			for (i = 3; i < n; i++) {
				*p[2 - (i&1)] = points[i];
				pixman_add_triangles(image,
						     -bounds.x1, -bounds.y1,
						     1, (pixman_triangle_t *)&tri);
			}
			pixman_image_unref(image);
		}

		mask = CreatePicture(0, &scratch->drawable,
				     PictureMatchFormat(screen, depth, format),
				     0, 0, serverClient, &error);
		if (mask) {
			CompositePicture(op, src, mask, dst,
					 xSrc + bounds.x1 - dst_x,
					 ySrc + bounds.y1 - dst_y,
					 0, 0,
					 bounds.x1, bounds.y1,
					 width, height);
			FreePicture(mask, 0);
		}
		sna_pixmap_destroy(scratch);
	} else {
		xTriangle tri;
		xPointFixed *p[3] = { &tri.p1, &tri.p2, &tri.p3 };
		int i;

		if (dst->polyEdge == PolyEdgeSharp)
			maskFormat = PictureMatchFormat(screen, 1, PICT_a1);
		else
			maskFormat = PictureMatchFormat(screen, 8, PICT_a8);

		*p[0] = points[0];
		*p[1] = points[1];
		*p[2] = points[2];
		triangles_fallback(op,
				   src, dst, maskFormat,
				   xSrc, ySrc, 1, &tri);
		for (i = 3; i < n; i++) {
			*p[2 - (i&1)] = points[i];
			/* Should xSrc,ySrc be updated? */
			triangles_fallback(op,
					   src, dst, maskFormat,
					   xSrc, ySrc, 1, &tri);
		}
	}
}

void
sna_composite_trifan(CARD8 op,
		     PicturePtr src,
		     PicturePtr dst,
		     PictFormatPtr maskFormat,
		     INT16 xSrc, INT16 ySrc,
		     int n, xPointFixed *points)
{
	trifan_fallback(op, src, dst, maskFormat, xSrc, ySrc, n, points);
}
#endif
