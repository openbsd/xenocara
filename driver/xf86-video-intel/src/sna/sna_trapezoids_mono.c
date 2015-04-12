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

#if 0
#define __DBG(x) LogF x
#else
#define __DBG(x)
#endif

struct quorem {
	int32_t quo;
	int32_t rem;
};

struct mono_edge {
	struct mono_edge *next, *prev;

	int32_t height_left;
	int32_t dir;

	int32_t dy;
	struct quorem x;
	struct quorem dxdy;
};

struct mono_polygon {
	int num_edges;
	struct mono_edge *edges;
	struct mono_edge **y_buckets;

	struct mono_edge *y_buckets_embedded[64];
	struct mono_edge edges_embedded[32];
};

struct mono {
	/* Leftmost edge on the current scan line. */
	struct mono_edge head, tail;
	int is_vertical;

	struct sna *sna;
	struct sna_composite_op op;
	pixman_region16_t clip;

	fastcall void (*span)(struct mono *, int, int, BoxPtr);

	struct mono_polygon polygon;
};

#define I(x) pixman_fixed_to_int ((x) + pixman_fixed_1_minus_e/2)

static struct quorem
floored_muldivrem(int32_t x, int32_t a, int32_t b)
{
	struct quorem qr;
	int64_t xa = (int64_t)x*a;
	qr.quo = xa/b;
	qr.rem = xa%b;
	if (qr.rem < 0) {
		qr.quo -= 1;
		qr.rem += b;
	}
	return qr;
}

#if HAS_DEBUG_FULL
static void _assert_pixmap_contains_box(PixmapPtr pixmap, BoxPtr box, const char *function)
{
	if (box->x1 < 0 || box->y1 < 0 ||
	    box->x2 > pixmap->drawable.width ||
	    box->y2 > pixmap->drawable.height)
		FatalError("%s: damage box is beyond the pixmap: box=(%d, %d), (%d, %d), pixmap=(%d, %d)\n",
			   __FUNCTION__,
			   box->x1, box->y1, box->x2, box->y2,
			   pixmap->drawable.width,
			   pixmap->drawable.height);
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

static bool
mono_polygon_init(struct mono_polygon *polygon, BoxPtr box, int num_edges)
{
	unsigned h = box->y2 - box->y1;

	polygon->y_buckets = polygon->y_buckets_embedded;
	if (h > ARRAY_SIZE (polygon->y_buckets_embedded)) {
		polygon->y_buckets = malloc (h * sizeof (struct mono_edge *));
		if (unlikely (NULL == polygon->y_buckets))
			return false;
	}

	polygon->num_edges = 0;
	polygon->edges = polygon->edges_embedded;
	if (num_edges > (int)ARRAY_SIZE (polygon->edges_embedded)) {
		polygon->edges = malloc (num_edges * sizeof (struct mono_edge));
		if (unlikely (polygon->edges == NULL)) {
			if (polygon->y_buckets != polygon->y_buckets_embedded)
				free(polygon->y_buckets);
			return false;
		}
	}

	memset(polygon->y_buckets, 0, h * sizeof (struct edge *));
	return true;
}

static void
mono_polygon_fini(struct mono_polygon *polygon)
{
	if (polygon->y_buckets != polygon->y_buckets_embedded)
		free(polygon->y_buckets);

	if (polygon->edges != polygon->edges_embedded)
		free(polygon->edges);
}

static void
mono_add_line(struct mono *mono,
	      int dst_x, int dst_y,
	      xFixed top, xFixed bottom,
	      const xPointFixed *p1, const xPointFixed *p2,
	      int dir)
{
	struct mono_polygon *polygon = &mono->polygon;
	struct mono_edge *e;
	pixman_fixed_t dx;
	pixman_fixed_t dy;
	int y, ytop, ybot;

	__DBG(("%s: top=%d, bottom=%d, line=(%d, %d), (%d, %d) delta=%dx%d, dir=%d\n",
	       __FUNCTION__,
	       (int)top, (int)bottom,
	       (int)p1->x, (int)p1->y, (int)p2->x, (int)p2->y,
	       dst_x, dst_y,
	       dir));

	if (top > bottom) {
		const xPointFixed *t;

		y = top;
		top = bottom;
		bottom = y;

		t = p1;
		p1 = p2;
		p2 = t;

		dir = -dir;
	}

	y = I(top) + dst_y;
	ytop = MAX(y, mono->clip.extents.y1);

	y = I(bottom) + dst_y;
	ybot = MIN(y, mono->clip.extents.y2);

	if (ybot <= ytop) {
		__DBG(("discard clipped line\n"));
		return;
	}

	e = polygon->edges + polygon->num_edges++;
	e->height_left = ybot - ytop;
	e->dir = dir;

	dx = p2->x - p1->x;
	dy = p2->y - p1->y;

	if (dx == 0) {
		e->x.quo = p1->x;
		e->x.rem = 0;
		e->dxdy.quo = 0;
		e->dxdy.rem = 0;
		e->dy = 0;
	} else {
		e->dxdy = floored_muldivrem (dx, pixman_fixed_1, dy);
		e->dy = dy;

		e->x = floored_muldivrem ((ytop-dst_y) * pixman_fixed_1 + pixman_fixed_1_minus_e/2 - p1->y,
					  dx, dy);
		e->x.quo += p1->x;
		e->x.rem -= dy;
	}
	e->x.quo += dst_x*pixman_fixed_1;

	{
		struct mono_edge **ptail = &polygon->y_buckets[ytop - mono->clip.extents.y1];
		if (*ptail)
			(*ptail)->prev = e;
		e->next = *ptail;
		e->prev = NULL;
		*ptail = e;
	}
}

static struct mono_edge *
mono_merge_sorted_edges(struct mono_edge *head_a, struct mono_edge *head_b)
{
	struct mono_edge *head, **next, *prev;
	int32_t x;

	if (head_b == NULL)
		return head_a;

	prev = head_a->prev;
	next = &head;
	if (head_a->x.quo <= head_b->x.quo) {
		head = head_a;
	} else {
		head = head_b;
		head_b->prev = prev;
		goto start_with_b;
	}

	do {
		x = head_b->x.quo;
		while (head_a != NULL && head_a->x.quo <= x) {
			prev = head_a;
			next = &head_a->next;
			head_a = head_a->next;
		}

		head_b->prev = prev;
		*next = head_b;
		if (head_a == NULL)
			return head;

start_with_b:
		x = head_a->x.quo;
		while (head_b != NULL && head_b->x.quo <= x) {
			prev = head_b;
			next = &head_b->next;
			head_b = head_b->next;
		}

		head_a->prev = prev;
		*next = head_a;
		if (head_b == NULL)
			return head;
	} while (1);
}

static struct mono_edge *
mono_sort_edges(struct mono_edge *list,
		unsigned int level,
		struct mono_edge **head_out)
{
	struct mono_edge *head_other, *remaining;
	unsigned int i;

	head_other = list->next;

	if (head_other == NULL) {
		*head_out = list;
		return NULL;
	}

	remaining = head_other->next;
	if (list->x.quo <= head_other->x.quo) {
		*head_out = list;
		head_other->next = NULL;
	} else {
		*head_out = head_other;
		head_other->prev = list->prev;
		head_other->next = list;
		list->prev = head_other;
		list->next = NULL;
	}

	for (i = 0; i < level && remaining; i++) {
		remaining = mono_sort_edges(remaining, i, &head_other);
		*head_out = mono_merge_sorted_edges(*head_out, head_other);
	}

	return remaining;
}

static struct mono_edge *mono_filter(struct mono_edge *edges)
{
	struct mono_edge *e;

	e = edges;
	do {
		struct mono_edge *n = e->next;
		if (e->dir == -n->dir &&
		    e->height_left == n->height_left &&
		    *(uint64_t *)&e->x == *(uint64_t *)&n->x &&
		    *(uint64_t *)&e->dxdy == *(uint64_t *)&n->dxdy) {
			if (e->prev)
				e->prev->next = n->next;
			else
				edges = n->next;
			if (n->next)
				n->next->prev = e->prev;
			else
				break;

			e = n->next;
		} else
			e = e->next;
	} while (e->next);

	return edges;
}

static struct mono_edge *
mono_merge_unsorted_edges(struct mono_edge *head, struct mono_edge *unsorted)
{
	mono_sort_edges(unsorted, UINT_MAX, &unsorted);
	return mono_merge_sorted_edges(head, mono_filter(unsorted));
}

#if 0
static inline void
__dbg_mono_edges(const char *function, struct mono_edge *edges)
{
	DBG(("%s: ", function));
	while (edges) {
		if (edges->x.quo < INT16_MAX << 16) {
			DBG(("(%d.%06d)+(%d.%06d)x%d, ",
			     edges->x.quo, edges->x.rem,
			     edges->dxdy.quo, edges->dxdy.rem,
			     edges->dy*edges->dir));
		}
		edges = edges->next;
	}
	DBG(("\n"));
}
#define DBG_MONO_EDGES(x) __dbg_mono_edges(__FUNCTION__, x)
static inline void
VALIDATE_MONO_EDGES(struct mono_edge *edges)
{
	int prev_x = edges->x.quo;
	while ((edges = edges->next)) {
		assert(edges->x.quo >= prev_x);
		prev_x = edges->x.quo;
	}
}

#else
#define DBG_MONO_EDGES(x)
#define VALIDATE_MONO_EDGES(x)
#endif

inline static void
mono_merge_edges(struct mono *c, struct mono_edge *edges)
{
	struct mono_edge *e;

	DBG_MONO_EDGES(edges);

	for (e = edges; c->is_vertical && e; e = e->next)
		c->is_vertical = e->dy == 0;

	c->head.next = mono_merge_unsorted_edges(c->head.next, edges);
}

fastcall static void
mono_span(struct mono *c, int x1, int x2, BoxPtr box)
{
	__DBG(("%s [%d, %d]\n", __FUNCTION__, x1, x2));

	box->x1 = x1;
	box->x2 = x2;

	if (c->clip.data) {
		pixman_region16_t region;

		pixman_region_init_rects(&region, box, 1);
		RegionIntersect(&region, &region, &c->clip);
		if (region_num_rects(&region)) {
			c->op.boxes(c->sna, &c->op,
				    region_rects(&region),
				    region_num_rects(&region));
			apply_damage(&c->op, &region);
		}
		pixman_region_fini(&region);
	} else {
		c->op.box(c->sna, &c->op, box);
		apply_damage_box(&c->op, box);
	}
}

fastcall static void
mono_span__fast(struct mono *c, int x1, int x2, BoxPtr box)
{
	__DBG(("%s [%d, %d]\n", __FUNCTION__, x1, x2));

	box->x1 = x1;
	box->x2 = x2;

	c->op.box(c->sna, &c->op, box);
}

struct mono_span_thread_boxes {
	const struct sna_composite_op *op;
#define MONO_SPAN_MAX_BOXES (8192/sizeof(BoxRec))
	BoxRec boxes[MONO_SPAN_MAX_BOXES];
	int num_boxes;
};

inline static void
thread_mono_span_add_boxes(struct mono *c, const BoxRec *box, int count)
{
	struct mono_span_thread_boxes *b = c->op.priv;

	assert(count > 0 && count <= MONO_SPAN_MAX_BOXES);
	if (unlikely(b->num_boxes + count > MONO_SPAN_MAX_BOXES)) {
		b->op->thread_boxes(c->sna, b->op, b->boxes, b->num_boxes);
		b->num_boxes = 0;
	}

	memcpy(b->boxes + b->num_boxes, box, count*sizeof(BoxRec));
	b->num_boxes += count;
	assert(b->num_boxes <= MONO_SPAN_MAX_BOXES);
}

fastcall static void
thread_mono_span_clipped(struct mono *c, int x1, int x2, BoxPtr box)
{
	pixman_region16_t region;

	__DBG(("%s [%d, %d]\n", __FUNCTION__, x1, x2));

	box->x1 = x1;
	box->x2 = x2;

	assert(c->clip.data);

	pixman_region_init_rects(&region, box, 1);
	RegionIntersect(&region, &region, &c->clip);
	if (region_num_rects(&region))
		thread_mono_span_add_boxes(c,
					   region_rects(&region),
					   region_num_rects(&region));
	pixman_region_fini(&region);
}

fastcall static void
thread_mono_span(struct mono *c, int x1, int x2, BoxPtr box)
{
	__DBG(("%s [%d, %d]\n", __FUNCTION__, x1, x2));

	box->x1 = x1;
	box->x2 = x2;
	thread_mono_span_add_boxes(c, box, 1);
}

inline static void
mono_row(struct mono *c, int16_t y, int16_t h)
{
	struct mono_edge *edge = c->head.next;
	int prev_x = INT_MIN;
	int16_t xstart = INT16_MIN;
	int winding = 0;
	BoxRec box;

	DBG_MONO_EDGES(edge);
	VALIDATE_MONO_EDGES(&c->head);

	box.y1 = c->clip.extents.y1 + y;
	box.y2 = box.y1 + h;

	while (&c->tail != edge) {
		struct mono_edge *next = edge->next;
		int16_t xend = I(edge->x.quo);

		if (--edge->height_left) {
			if (edge->dy) {
				edge->x.quo += edge->dxdy.quo;
				edge->x.rem += edge->dxdy.rem;
				if (edge->x.rem >= 0) {
					++edge->x.quo;
					edge->x.rem -= edge->dy;
				}
			}

			if (edge->x.quo < prev_x) {
				struct mono_edge *pos = edge->prev;
				pos->next = next;
				next->prev = pos;
				do {
					pos = pos->prev;
				} while (edge->x.quo < pos->x.quo);
				pos->next->prev = edge;
				edge->next = pos->next;
				edge->prev = pos;
				pos->next = edge;
			} else
				prev_x = edge->x.quo;
		} else {
			edge->prev->next = next;
			next->prev = edge->prev;
		}

		winding += edge->dir;
		if (winding == 0) {
			assert(I(next->x.quo) >= xend);
			if (I(next->x.quo) > xend + 1) {
				if (xstart < c->clip.extents.x1)
					xstart = c->clip.extents.x1;
				if (xend > c->clip.extents.x2)
					xend = c->clip.extents.x2;
				if (xend > xstart)
					c->span(c, xstart, xend, &box);
				xstart = INT16_MIN;
			}
		} else if (xstart == INT16_MIN)
			xstart = xend;

		edge = next;
	}

	DBG_MONO_EDGES(c->head.next);
	VALIDATE_MONO_EDGES(&c->head);
}

static bool
mono_init(struct mono *c, int num_edges)
{
	if (!mono_polygon_init(&c->polygon, &c->clip.extents, num_edges))
		return false;

	c->head.dy = 0;
	c->head.height_left = INT_MAX;
	c->head.x.quo = INT16_MIN << 16;
	c->head.prev = NULL;
	c->head.next = &c->tail;
	c->tail.prev = &c->head;
	c->tail.next = NULL;
	c->tail.x.quo = INT16_MAX << 16;
	c->tail.height_left = INT_MAX;
	c->tail.dy = 0;

	c->is_vertical = 1;

	return true;
}

static void
mono_fini(struct mono *mono)
{
	mono_polygon_fini(&mono->polygon);
}

static void
mono_step_edges(struct mono *c, int count)
{
	struct mono_edge *edge;

	for (edge = c->head.next; edge != &c->tail; edge = edge->next) {
		edge->height_left -= count;
		if (! edge->height_left) {
			edge->prev->next = edge->next;
			edge->next->prev = edge->prev;
		}
	}
}

flatten static void
mono_render(struct mono *mono)
{
	struct mono_polygon *polygon = &mono->polygon;
	int i, j, h = mono->clip.extents.y2 - mono->clip.extents.y1;

	assert(mono->span);

	for (i = 0; i < h; i = j) {
		j = i + 1;

		if (polygon->y_buckets[i])
			mono_merge_edges(mono, polygon->y_buckets[i]);

		if (mono->is_vertical) {
			struct mono_edge *e = mono->head.next;
			int min_height = h - i;

			while (e != &mono->tail) {
				if (e->height_left < min_height)
					min_height = e->height_left;
				e = e->next;
			}

			while (--min_height >= 1 && polygon->y_buckets[j] == NULL)
				j++;
			if (j != i + 1)
				mono_step_edges(mono, j - (i + 1));
		}

		mono_row(mono, i, j-i);

		/* XXX recompute after dropping edges? */
		if (mono->head.next == &mono->tail)
			mono->is_vertical = 1;
	}
}

static int operator_is_bounded(uint8_t op)
{
	switch (op) {
	case PictOpOver:
	case PictOpOutReverse:
	case PictOpAdd:
		return true;
	default:
		return false;
	}
}

struct mono_span_thread {
	struct sna *sna;
	const xTrapezoid *traps;
	const struct sna_composite_op *op;
	RegionPtr clip;
	int ntrap;
	BoxRec extents;
	int dx, dy;
};

static void
mono_span_thread(void *arg)
{
	struct mono_span_thread *thread = arg;
	struct mono mono;
	struct mono_span_thread_boxes boxes;
	const xTrapezoid *t;
	int n;

	mono.sna = thread->sna;

	mono.clip.extents = thread->extents;
	mono.clip.data = NULL;
	if (thread->clip->data) {
		RegionIntersect(&mono.clip, &mono.clip, thread->clip);
		if (RegionNil(&mono.clip))
			return;
	}

	boxes.op = thread->op;
	boxes.num_boxes = 0;
	mono.op.priv = &boxes;

	if (!mono_init(&mono, 2*thread->ntrap)) {
		RegionUninit(&mono.clip);
		return;
	}

	for (n = thread->ntrap, t = thread->traps; n--; t++) {
		if (!xTrapezoidValid(t))
			continue;

		if (pixman_fixed_to_int(t->top) + thread->dy >= thread->extents.y2 ||
		    pixman_fixed_to_int(t->bottom) + thread->dy <= thread->extents.y1)
			continue;

		mono_add_line(&mono, thread->dx, thread->dy,
			      t->top, t->bottom,
			      &t->left.p1, &t->left.p2, 1);
		mono_add_line(&mono, thread->dx, thread->dy,
			      t->top, t->bottom,
			      &t->right.p1, &t->right.p2, -1);
	}

	if (mono.clip.data == NULL)
		mono.span = thread_mono_span;
	else
		mono.span = thread_mono_span_clipped;

	mono_render(&mono);
	mono_fini(&mono);

	if (boxes.num_boxes)
		thread->op->thread_boxes(thread->sna, thread->op,
					 boxes.boxes, boxes.num_boxes);
	RegionUninit(&mono.clip);
}

bool
mono_trapezoids_span_converter(struct sna *sna,
			       CARD8 op, PicturePtr src, PicturePtr dst,
			       INT16 src_x, INT16 src_y,
			       int ntrap, xTrapezoid *traps)
{
	struct mono mono;
	BoxRec extents;
	int16_t dst_x, dst_y;
	int16_t dx, dy;
	bool unbounded;
	int num_threads, n;

	if (NO_SCAN_CONVERTER)
		return false;

	trapezoid_origin(&traps[0].left, &dst_x, &dst_y);

	if (!trapezoids_bounds(ntrap, traps, &extents))
		return true;

	DBG(("%s: extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__, extents.x1, extents.y1, extents.x2, extents.y2));

	if (!sna_compute_composite_region(&mono.clip,
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

	dx = dst->pDrawable->x;
	dy = dst->pDrawable->y;

	DBG(("%s: after clip -- extents (%d, %d), (%d, %d), delta=(%d, %d) src -> (%d, %d)\n",
	     __FUNCTION__,
	     mono.clip.extents.x1, mono.clip.extents.y1,
	     mono.clip.extents.x2, mono.clip.extents.y2,
	     dx, dy,
	     src_x + mono.clip.extents.x1 - dst_x - dx,
	     src_y + mono.clip.extents.y1 - dst_y - dy));

	unbounded = (!sna_drawable_is_clear(dst->pDrawable) &&
		     !operator_is_bounded(op));

	if (op == PictOpClear && sna->clear)
		src = sna->clear;

	mono.sna = sna;
	if (!mono.sna->render.composite(mono.sna, op, src, NULL, dst,
				       src_x + mono.clip.extents.x1 - dst_x - dx,
				       src_y + mono.clip.extents.y1 - dst_y - dy,
				       0, 0,
				       mono.clip.extents.x1,  mono.clip.extents.y1,
				       mono.clip.extents.x2 - mono.clip.extents.x1,
				       mono.clip.extents.y2 - mono.clip.extents.y1,
				       COMPOSITE_PARTIAL, memset(&mono.op, 0, sizeof(mono.op))))
		return false;

	num_threads = 1;
	if (!NO_GPU_THREADS &&
	    mono.op.thread_boxes &&
	    mono.op.damage == NULL &&
	    !unbounded)
		num_threads = sna_use_threads(mono.clip.extents.x2 - mono.clip.extents.x1,
					      mono.clip.extents.y2 - mono.clip.extents.y1,
					      32);
	if (num_threads > 1) {
		struct mono_span_thread threads[num_threads];
		int y, h;

		DBG(("%s: using %d threads for mono span compositing %dx%d\n",
		     __FUNCTION__, num_threads,
		     mono.clip.extents.x2 - mono.clip.extents.x1,
		     mono.clip.extents.y2 - mono.clip.extents.y1));

		threads[0].sna = mono.sna;
		threads[0].op = &mono.op;
		threads[0].traps = traps;
		threads[0].ntrap = ntrap;
		threads[0].extents = mono.clip.extents;
		threads[0].clip = &mono.clip;
		threads[0].dx = dx;
		threads[0].dy = dy;

		y = extents.y1;
		h = extents.y2 - extents.y1;
		h = (h + num_threads - 1) / num_threads;
		num_threads -= (num_threads-1) * h >= extents.y2 - extents.y1;

		for (n = 1; n < num_threads; n++) {
			threads[n] = threads[0];
			threads[n].extents.y1 = y;
			threads[n].extents.y2 = y += h;

			sna_threads_run(n, mono_span_thread, &threads[n]);
		}

		threads[0].extents.y1 = y;
		threads[0].extents.y2 = extents.y2;
		mono_span_thread(&threads[0]);

		sna_threads_wait();
		mono.op.done(mono.sna, &mono.op);
		return true;
	}

	if (!mono_init(&mono, 2*ntrap))
		return false;

	for (n = 0; n < ntrap; n++) {
		if (!xTrapezoidValid(&traps[n]))
			continue;

		if (pixman_fixed_integer_floor(traps[n].top) + dy >= mono.clip.extents.y2 ||
		    pixman_fixed_integer_ceil(traps[n].bottom) + dy <= mono.clip.extents.y1)
			continue;

		mono_add_line(&mono, dx, dy,
			      traps[n].top, traps[n].bottom,
			      &traps[n].left.p1, &traps[n].left.p2, 1);
		mono_add_line(&mono, dx, dy,
			      traps[n].top, traps[n].bottom,
			      &traps[n].right.p1, &traps[n].right.p2, -1);
	}

	if (mono.clip.data == NULL && mono.op.damage == NULL)
		mono.span = mono_span__fast;
	else
		mono.span = mono_span;

	mono_render(&mono);
	mono.op.done(mono.sna, &mono.op);
	mono_fini(&mono);

	if (unbounded) {
		xPointFixed p1, p2;

		if (!mono_init(&mono, 2+2*ntrap))
			return false;

		p1.y = mono.clip.extents.y1 * pixman_fixed_1;
		p2.y = mono.clip.extents.y2 * pixman_fixed_1;

		p1.x = mono.clip.extents.x1 * pixman_fixed_1;
		p2.x = mono.clip.extents.x1 * pixman_fixed_1;
		mono_add_line(&mono, 0, 0, p1.y, p2.y, &p1, &p2, -1);

		p1.x = mono.clip.extents.x2 * pixman_fixed_1;
		p2.x = mono.clip.extents.x2 * pixman_fixed_1;
		mono_add_line(&mono, 0, 0, p1.y, p2.y, &p1, &p2, 1);

		for (n = 0; n < ntrap; n++) {
			if (!xTrapezoidValid(&traps[n]))
				continue;

			if (pixman_fixed_to_int(traps[n].top) + dy >= mono.clip.extents.y2 ||
			    pixman_fixed_to_int(traps[n].bottom) + dy < mono.clip.extents.y1)
				continue;

			mono_add_line(&mono, dx, dy,
				      traps[n].top, traps[n].bottom,
				      &traps[n].left.p1, &traps[n].left.p2, 1);
			mono_add_line(&mono, dx, dy,
				      traps[n].top, traps[n].bottom,
				      &traps[n].right.p1, &traps[n].right.p2, -1);
		}
		if (mono.sna->render.composite(mono.sna,
					       PictOpClear,
					       mono.sna->clear, NULL, dst,
					       0, 0,
					       0, 0,
					       mono.clip.extents.x1,  mono.clip.extents.y1,
					       mono.clip.extents.x2 - mono.clip.extents.x1,
					       mono.clip.extents.y2 - mono.clip.extents.y1,
					       COMPOSITE_PARTIAL, memset(&mono.op, 0, sizeof(mono.op)))) {
			mono_render(&mono);
			mono.op.done(mono.sna, &mono.op);
		}
		mono_fini(&mono);
	}

	REGION_UNINIT(NULL, &mono.clip);
	return true;
}

struct mono_inplace_composite {
	pixman_image_t *src, *dst;
	int dx, dy;
	int sx, sy;
	int op;
};
struct mono_inplace_fill {
	uint32_t *data, stride;
	uint32_t color;
	int bpp;
};

fastcall static void
mono_inplace_fill_box(struct sna *sna,
		      const struct sna_composite_op *op,
		      const BoxRec *box)
{
	struct mono_inplace_fill *fill = op->priv;

	DBG(("(%s: (%d, %d)x(%d, %d):%08x\n",
	     __FUNCTION__,
	     box->x1, box->y1,
	     box->x2 - box->x1,
	     box->y2 - box->y1,
	     fill->color));
	pixman_fill(fill->data, fill->stride, fill->bpp,
		    box->x1, box->y1,
		    box->x2 - box->x1,
		    box->y2 - box->y1,
		    fill->color);
}

static void
mono_inplace_fill_boxes(struct sna *sna,
			const struct sna_composite_op *op,
			const BoxRec *box, int nbox)
{
	struct mono_inplace_fill *fill = op->priv;

	do {
		DBG(("(%s: (%d, %d)x(%d, %d):%08x\n",
		     __FUNCTION__,
		     box->x1, box->y1,
		     box->x2 - box->x1,
		     box->y2 - box->y1,
		     fill->color));
		pixman_fill(fill->data, fill->stride, fill->bpp,
			    box->x1, box->y1,
			    box->x2 - box->x1,
			    box->y2 - box->y1,
			    fill->color);
		box++;
	} while (--nbox);
}

fastcall static void
mono_inplace_composite_box(struct sna *sna,
			   const struct sna_composite_op *op,
			   const BoxRec *box)
{
	struct mono_inplace_composite *c = op->priv;

	pixman_image_composite(c->op, c->src, NULL, c->dst,
			       box->x1 + c->sx, box->y1 + c->sy,
			       0, 0,
			       box->x1 + c->dx, box->y1 + c->dy,
			       box->x2 - box->x1,
			       box->y2 - box->y1);
}

static void
mono_inplace_composite_boxes(struct sna *sna,
			     const struct sna_composite_op *op,
			     const BoxRec *box, int nbox)
{
	struct mono_inplace_composite *c = op->priv;

	do {
		pixman_image_composite(c->op, c->src, NULL, c->dst,
				       box->x1 + c->sx, box->y1 + c->sy,
				       0, 0,
				       box->x1 + c->dx, box->y1 + c->dy,
				       box->x2 - box->x1,
				       box->y2 - box->y1);
		box++;
	} while (--nbox);
}

bool
mono_trapezoid_span_inplace(struct sna *sna,
			    CARD8 op,
			    PicturePtr src,
			    PicturePtr dst,
			    INT16 src_x, INT16 src_y,
			    int ntrap, xTrapezoid *traps)
{
	struct mono mono;
	union {
		struct mono_inplace_fill fill;
		struct mono_inplace_composite composite;
	} inplace;
	int was_clear;
	int x, y, n;

	if (!trapezoids_bounds(ntrap, traps, &mono.clip.extents))
		return true;

	DBG(("%s: extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     mono.clip.extents.x1, mono.clip.extents.y1,
	     mono.clip.extents.x2, mono.clip.extents.y2));

	if (!sna_compute_composite_region(&mono.clip,
					  src, NULL, dst,
					  src_x, src_y,
					  0, 0,
					  mono.clip.extents.x1, mono.clip.extents.y1,
					  mono.clip.extents.x2 - mono.clip.extents.x1,
					  mono.clip.extents.y2 - mono.clip.extents.y1)) {
		DBG(("%s: trapezoids do not intersect drawable clips\n",
		     __FUNCTION__)) ;
		return true;
	}

	DBG(("%s: clipped extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     mono.clip.extents.x1, mono.clip.extents.y1,
	     mono.clip.extents.x2, mono.clip.extents.y2));

	was_clear = sna_drawable_is_clear(dst->pDrawable);
	if (!sna_drawable_move_region_to_cpu(dst->pDrawable, &mono.clip,
					     MOVE_WRITE | MOVE_READ))
		return true;

	mono.sna = sna;
	if (!mono_init(&mono, 2*ntrap))
		return false;

	mono.op.damage = NULL;

	x = dst->pDrawable->x;
	y = dst->pDrawable->y;

	for (n = 0; n < ntrap; n++) {
		if (!xTrapezoidValid(&traps[n]))
			continue;

		if (pixman_fixed_to_int(traps[n].top) + y >= mono.clip.extents.y2 ||
		    pixman_fixed_to_int(traps[n].bottom) + y < mono.clip.extents.y1)
			continue;

		mono_add_line(&mono, x, y,
			      traps[n].top, traps[n].bottom,
			      &traps[n].left.p1, &traps[n].left.p2, 1);
		mono_add_line(&mono, x, y,
			      traps[n].top, traps[n].bottom,
			      &traps[n].right.p1, &traps[n].right.p2, -1);
	}

	if (sna_picture_is_solid(src, &inplace.fill.color) &&
	    (op == PictOpSrc || op == PictOpClear ||
	     (was_clear && (op == PictOpOver || op == PictOpAdd)) ||
	     (op == PictOpOver && inplace.fill.color >> 24 == 0xff))) {
		PixmapPtr pixmap;
		int16_t dx, dy;
		uint8_t *ptr;

unbounded_pass:
		pixmap = get_drawable_pixmap(dst->pDrawable);

		ptr = pixmap->devPrivate.ptr;
		if (get_drawable_deltas(dst->pDrawable, pixmap, &dx, &dy))
			ptr += dy * pixmap->devKind + dx * pixmap->drawable.bitsPerPixel / 8;
		inplace.fill.data = (uint32_t *)ptr;
		inplace.fill.stride = pixmap->devKind / sizeof(uint32_t);
		inplace.fill.bpp = pixmap->drawable.bitsPerPixel;

		if (op == PictOpClear)
			inplace.fill.color = 0;
		else if (dst->format != PICT_a8r8g8b8)
			inplace.fill.color = sna_rgba_to_color(inplace.fill.color, dst->format);

		DBG(("%s: fill %x\n", __FUNCTION__, inplace.fill.color));

		mono.op.priv = &inplace.fill;
		mono.op.box = mono_inplace_fill_box;
		mono.op.boxes = mono_inplace_fill_boxes;

		op = 0;
	} else {
		if (src->pDrawable) {
			if (!sna_drawable_move_to_cpu(src->pDrawable,
						      MOVE_READ)) {
				mono_fini(&mono);
				return false;
			}
			if (src->alphaMap &&
			    !sna_drawable_move_to_cpu(src->alphaMap->pDrawable,
						      MOVE_READ)) {
				mono_fini(&mono);
				return false;
			}
		}

		inplace.composite.dst = image_from_pict(dst, false,
							&inplace.composite.dx,
							&inplace.composite.dy);
		inplace.composite.src = image_from_pict(src, false,
							&inplace.composite.sx,
							&inplace.composite.sy);
		inplace.composite.sx +=
			src_x - pixman_fixed_to_int(traps[0].left.p1.x),
		inplace.composite.sy +=
			src_y - pixman_fixed_to_int(traps[0].left.p1.y),
		inplace.composite.op = op;

		mono.op.priv = &inplace.composite;
		mono.op.box = mono_inplace_composite_box;
		mono.op.boxes = mono_inplace_composite_boxes;
	}

	if (mono.clip.data == NULL && mono.op.damage == NULL)
		mono.span = mono_span__fast;
	else
		mono.span = mono_span;
	if (sigtrap_get() == 0) {
		mono_render(&mono);
		sigtrap_put();
	}
	mono_fini(&mono);

	if (op) {
		free_pixman_pict(src, inplace.composite.src);
		free_pixman_pict(dst, inplace.composite.dst);

		if (!was_clear && !operator_is_bounded(op)) {
			xPointFixed p1, p2;

			DBG(("%s: unbounded fixup\n", __FUNCTION__));

			if (!mono_init(&mono, 2+2*ntrap))
				return false;

			p1.y = mono.clip.extents.y1 * pixman_fixed_1;
			p2.y = mono.clip.extents.y2 * pixman_fixed_1;

			p1.x = mono.clip.extents.x1 * pixman_fixed_1;
			p2.x = mono.clip.extents.x1 * pixman_fixed_1;
			mono_add_line(&mono, 0, 0, p1.y, p2.y, &p1, &p2, -1);

			p1.x = mono.clip.extents.x2 * pixman_fixed_1;
			p2.x = mono.clip.extents.x2 * pixman_fixed_1;
			mono_add_line(&mono, 0, 0, p1.y, p2.y, &p1, &p2, 1);

			for (n = 0; n < ntrap; n++) {
				if (!xTrapezoidValid(&traps[n]))
					continue;

				if (pixman_fixed_to_int(traps[n].top) + x >= mono.clip.extents.y2 ||
				    pixman_fixed_to_int(traps[n].bottom) + y < mono.clip.extents.y1)
					continue;

				mono_add_line(&mono, x, y,
					      traps[n].top, traps[n].bottom,
					      &traps[n].left.p1, &traps[n].left.p2, 1);
				mono_add_line(&mono, x, y,
					      traps[n].top, traps[n].bottom,
					      &traps[n].right.p1, &traps[n].right.p2, -1);
			}

			op = PictOpClear;
			goto unbounded_pass;
		}
	}

	return true;
}

bool
mono_trap_span_converter(struct sna *sna,
			 PicturePtr dst,
			 INT16 x, INT16 y,
			 int ntrap, xTrap *traps)
{
	struct mono mono;
	xRenderColor white;
	PicturePtr src;
	int error;
	int n;

	white.red = white.green = white.blue = white.alpha = 0xffff;
	src = CreateSolidPicture(0, &white, &error);
	if (src == NULL)
		return true;

	mono.clip = *dst->pCompositeClip;
	x += dst->pDrawable->x;
	y += dst->pDrawable->y;

	DBG(("%s: after clip -- extents (%d, %d), (%d, %d), delta=(%d, %d)\n",
	     __FUNCTION__,
	     mono.clip.extents.x1, mono.clip.extents.y1,
	     mono.clip.extents.x2, mono.clip.extents.y2,
	     x, y));

	mono.sna = sna;
	if (!mono_init(&mono, 2*ntrap))
		return false;

	for (n = 0; n < ntrap; n++) {
		xPointFixed p1, p2;

		if (pixman_fixed_to_int(traps[n].top.y) + y >= mono.clip.extents.y2 ||
		    pixman_fixed_to_int(traps[n].bot.y) + y < mono.clip.extents.y1)
			continue;

		p1.y = traps[n].top.y;
		p2.y = traps[n].bot.y;

		p1.x = traps[n].top.l;
		p2.x = traps[n].bot.l;
		mono_add_line(&mono, x, y,
			      traps[n].top.y, traps[n].bot.y,
			      &p1, &p2, 1);

		p1.x = traps[n].top.r;
		p2.x = traps[n].bot.r;
		mono_add_line(&mono, x, y,
			      traps[n].top.y, traps[n].bot.y,
			      &p1, &p2, -1);
	}

	if (mono.sna->render.composite(mono.sna, PictOpAdd, src, NULL, dst,
				       0, 0,
				       0, 0,
				       mono.clip.extents.x1,  mono.clip.extents.y1,
				       mono.clip.extents.x2 - mono.clip.extents.x1,
				       mono.clip.extents.y2 - mono.clip.extents.y1,
				       COMPOSITE_PARTIAL, memset(&mono.op, 0, sizeof(mono.op)))) {
		mono_render(&mono);
		mono.op.done(mono.sna, &mono.op);
	}

	mono_fini(&mono);
	FreePicture(src, 0);
	return true;
}

bool
mono_triangles_span_converter(struct sna *sna,
			      CARD8 op, PicturePtr src, PicturePtr dst,
			      INT16 src_x, INT16 src_y,
			      int count, xTriangle *tri)
{
	struct mono mono;
	BoxRec extents;
	int16_t dst_x, dst_y;
	int16_t dx, dy;
	bool was_clear;
	int n;

	mono.sna = sna;

	dst_x = pixman_fixed_to_int(tri[0].p1.x);
	dst_y = pixman_fixed_to_int(tri[0].p1.y);

	miTriangleBounds(count, tri, &extents);
	DBG(("%s: extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__, extents.x1, extents.y1, extents.x2, extents.y2));

	if (extents.y1 >= extents.y2 || extents.x1 >= extents.x2)
		return true;

	if (!sna_compute_composite_region(&mono.clip,
					  src, NULL, dst,
					  src_x + extents.x1 - dst_x,
					  src_y + extents.y1 - dst_y,
					  0, 0,
					  extents.x1, extents.y1,
					  extents.x2 - extents.x1,
					  extents.y2 - extents.y1)) {
		DBG(("%s: triangles do not intersect drawable clips\n",
		     __FUNCTION__)) ;
		return true;
	}

	dx = dst->pDrawable->x;
	dy = dst->pDrawable->y;

	DBG(("%s: after clip -- extents (%d, %d), (%d, %d), delta=(%d, %d) src -> (%d, %d)\n",
	     __FUNCTION__,
	     mono.clip.extents.x1, mono.clip.extents.y1,
	     mono.clip.extents.x2, mono.clip.extents.y2,
	     dx, dy,
	     src_x + mono.clip.extents.x1 - dst_x - dx,
	     src_y + mono.clip.extents.y1 - dst_y - dy));

	was_clear = sna_drawable_is_clear(dst->pDrawable);

	if (!mono_init(&mono, 3*count))
		return false;

	for (n = 0; n < count; n++) {
		mono_add_line(&mono, dx, dy,
			      tri[n].p1.y, tri[n].p2.y,
			      &tri[n].p1, &tri[n].p2, 1);
		mono_add_line(&mono, dx, dy,
			      tri[n].p2.y, tri[n].p3.y,
			      &tri[n].p2, &tri[n].p3, 1);
		mono_add_line(&mono, dx, dy,
			      tri[n].p3.y, tri[n].p1.y,
			      &tri[n].p3, &tri[n].p1, 1);
	}

	if (mono.sna->render.composite(mono.sna, op, src, NULL, dst,
				       src_x + mono.clip.extents.x1 - dst_x - dx,
				       src_y + mono.clip.extents.y1 - dst_y - dy,
				       0, 0,
				       mono.clip.extents.x1,  mono.clip.extents.y1,
				       mono.clip.extents.x2 - mono.clip.extents.x1,
				       mono.clip.extents.y2 - mono.clip.extents.y1,
				       COMPOSITE_PARTIAL, memset(&mono.op, 0, sizeof(mono.op)))) {
		if (mono.clip.data == NULL && mono.op.damage == NULL)
			mono.span = mono_span__fast;
		else
			mono.span = mono_span;
		mono_render(&mono);
		mono.op.done(mono.sna, &mono.op);
	}

	if (!was_clear && !operator_is_bounded(op)) {
		xPointFixed p1, p2;

		if (!mono_init(&mono, 2+3*count))
			return false;

		p1.y = mono.clip.extents.y1 * pixman_fixed_1;
		p2.y = mono.clip.extents.y2 * pixman_fixed_1;

		p1.x = mono.clip.extents.x1 * pixman_fixed_1;
		p2.x = mono.clip.extents.x1 * pixman_fixed_1;
		mono_add_line(&mono, 0, 0, p1.y, p2.y, &p1, &p2, -1);

		p1.x = mono.clip.extents.x2 * pixman_fixed_1;
		p2.x = mono.clip.extents.x2 * pixman_fixed_1;
		mono_add_line(&mono, 0, 0, p1.y, p2.y, &p1, &p2, 1);

		for (n = 0; n < count; n++) {
			mono_add_line(&mono, dx, dy,
				      tri[n].p1.y, tri[n].p2.y,
				      &tri[n].p1, &tri[n].p2, 1);
			mono_add_line(&mono, dx, dy,
				      tri[n].p2.y, tri[n].p3.y,
				      &tri[n].p2, &tri[n].p3, 1);
			mono_add_line(&mono, dx, dy,
				      tri[n].p3.y, tri[n].p1.y,
				      &tri[n].p3, &tri[n].p1, 1);
		}

		if (mono.sna->render.composite(mono.sna,
					       PictOpClear,
					       mono.sna->clear, NULL, dst,
					       0, 0,
					       0, 0,
					       mono.clip.extents.x1,  mono.clip.extents.y1,
					       mono.clip.extents.x2 - mono.clip.extents.x1,
					       mono.clip.extents.y2 - mono.clip.extents.y1,
					       COMPOSITE_PARTIAL, memset(&mono.op, 0, sizeof(mono.op)))) {
			if (mono.clip.data == NULL && mono.op.damage == NULL)
				mono.span = mono_span__fast;
			else
				mono.span = mono_span;
			mono_render(&mono);
			mono.op.done(mono.sna, &mono.op);
		}
		mono_fini(&mono);
	}

	mono_fini(&mono);
	REGION_UNINIT(NULL, &mono.clip);
	return true;
}
