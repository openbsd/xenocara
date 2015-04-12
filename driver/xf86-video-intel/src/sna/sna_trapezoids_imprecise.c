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

#undef SAMPLES_X
#undef SAMPLES_Y

#if 0
#define __DBG(x) LogF x
#else
#define __DBG(x)
#endif

/* TODO: Emit unantialiased and MSAA triangles. */

#ifndef MAX
#define MAX(x,y) ((x) >= (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x,y) ((x) <= (y) ? (x) : (y))
#endif

typedef void (*span_func_t)(struct sna *sna,
			    struct sna_composite_spans_op *op,
			    pixman_region16_t *clip,
			    const BoxRec *box,
			    int coverage);

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

#define FAST_SAMPLES_X_TO_INT_FRAC(x, i, f) \
	_GRID_TO_INT_FRAC_shift(x, i, f, FAST_SAMPLES_shift)

#define FAST_SAMPLES_INT(x) ((x) >> (FAST_SAMPLES_shift))
#define FAST_SAMPLES_FRAC(x) ((x) & (FAST_SAMPLES_mask))

#define _GRID_TO_INT_FRAC_shift(t, i, f, b) do {	\
    (f) = FAST_SAMPLES_FRAC(t);				\
    (i) = FAST_SAMPLES_INT(t);				\
} while (0)

#define FAST_SAMPLES_XY (2*FAST_SAMPLES_X*FAST_SAMPLES_Y) /* Unit area on the grid. */
#define AREA_TO_ALPHA(c)  ((c) / (float)FAST_SAMPLES_XY)

struct quorem {
	int32_t quo;
	int32_t rem;
};

struct edge {
	struct edge *next, *prev;

	int dir;

	int height_left;

	struct quorem x;

	/* Advance of the current x when moving down a subsample line. */
	struct quorem dxdy;
	int dy;

	/* The clipped y of the top of the edge. */
	int ytop;

	/* y2-y1 after orienting the edge downwards.  */
};

/* Number of subsample rows per y-bucket. Must be SAMPLES_Y. */
#define EDGE_Y_BUCKET_HEIGHT FAST_SAMPLES_Y
#define EDGE_Y_BUCKET_INDEX(y, ymin) (((y) - (ymin))/EDGE_Y_BUCKET_HEIGHT)

/* A collection of sorted and vertically clipped edges of the polygon.
 * Edges are moved from the polygon to an active list while scan
 * converting. */
struct polygon {
	/* The vertical clip extents. */
	int ymin, ymax;

	/* Array of edges all starting in the same bucket.	An edge is put
	 * into bucket EDGE_BUCKET_INDEX(edge->ytop, polygon->ymin) when
	 * it is added to the polygon. */
	struct edge **y_buckets;
	struct edge *y_buckets_embedded[64];

	struct edge edges_embedded[32];
	struct edge *edges;
	int num_edges;
};

/* A cell records the effect on pixel coverage of polygon edges
 * passing through a pixel.  It contains two accumulators of pixel
 * coverage.
 *
 * Consider the effects of a polygon edge on the coverage of a pixel
 * it intersects and that of the following one.  The coverage of the
 * following pixel is the height of the edge multiplied by the width
 * of the pixel, and the coverage of the pixel itself is the area of
 * the trapezoid formed by the edge and the right side of the pixel.
 *
 * +-----------------------+-----------------------+
 * |                       |                       |
 * |                       |                       |
 * |_______________________|_______________________|
 * |   \...................|.......................|\
 * |    \..................|.......................| |
 * |     \.................|.......................| |
 * |      \....covered.....|.......................| |
 * |       \....area.......|.......................| } covered height
 * |        \..............|.......................| |
 * |uncovered\.............|.......................| |
 * |  area    \............|.......................| |
 * |___________\...........|.......................|/
 * |                       |                       |
 * |                       |                       |
 * |                       |                       |
 * +-----------------------+-----------------------+
 *
 * Since the coverage of the following pixel will always be a multiple
 * of the width of the pixel, we can store the height of the covered
 * area instead.  The coverage of the pixel itself is the total
 * coverage minus the area of the uncovered area to the left of the
 * edge.  As it's faster to compute the uncovered area we only store
 * that and subtract it from the total coverage later when forming
 * spans to blit.
 *
 * The heights and areas are signed, with left edges of the polygon
 * having positive sign and right edges having negative sign.  When
 * two edges intersect they swap their left/rightness so their
 * contribution above and below the intersection point must be
 * computed separately. */
struct cell {
	struct cell *next;
	int x;
	int16_t uncovered_area;
	int16_t covered_height;
};

/* A cell list represents the scan line sparsely as cells ordered by
 * ascending x.  It is geared towards scanning the cells in order
 * using an internal cursor. */
struct cell_list {
	struct cell *cursor;

	/* Points to the left-most cell in the scan line. */
	struct cell head, tail;

	int16_t x1, x2;
	int16_t count, size;
	struct cell *cells;
	struct cell embedded[256];
};

/* The active list contains edges in the current scan line ordered by
 * the x-coordinate of the intercept of the edge and the scan line. */
struct active_list {
	/* Leftmost edge on the current scan line. */
	struct edge head, tail;
};

struct tor {
    struct polygon	polygon[1];
    struct active_list	active[1];
    struct cell_list	coverages[1];

    BoxRec extents;
};

/* Compute the floored division a/b. Assumes / and % perform symmetric
 * division. */
inline static struct quorem
floored_divrem(int a, int b)
{
	struct quorem qr;
	assert(b>0);
	qr.quo = a/b;
	qr.rem = a%b;
	if (qr.rem < 0) {
		qr.quo -= 1;
		qr.rem += b;
	}
	return qr;
}

/* Compute the floored division (x*a)/b. Assumes / and % perform symmetric
 * division. */
static struct quorem
floored_muldivrem(int32_t x, int32_t a, int32_t b)
{
	struct quorem qr;
	int64_t xa = (int64_t)x*a;
	assert(b>0);
	qr.quo = xa/b;
	qr.rem = xa%b;
	if (qr.rem < 0) {
		qr.quo -= 1;
		qr.rem += b;
	}
	return qr;
}

/* Rewinds the cell list's cursor to the beginning.  After rewinding
 * we're good to cell_list_find() the cell any x coordinate. */
inline static void
cell_list_rewind(struct cell_list *cells)
{
	cells->cursor = &cells->head;
}

static bool
cell_list_init(struct cell_list *cells, int x1, int x2)
{
	cells->tail.next = NULL;
	cells->tail.x = INT_MAX;
	cells->head.x = INT_MIN;
	cells->head.next = &cells->tail;
	cells->head.covered_height = 0;
	cell_list_rewind(cells);
	cells->count = 0;
	cells->x1 = x1;
	cells->x2 = x2;
	cells->size = x2 - x1 + 1;
	cells->cells = cells->embedded;
	if (cells->size > ARRAY_SIZE(cells->embedded))
		cells->cells = malloc(cells->size * sizeof(struct cell));
	return cells->cells != NULL;
}

static void
cell_list_fini(struct cell_list *cells)
{
	if (cells->cells != cells->embedded)
		free(cells->cells);
}

inline static void
cell_list_reset(struct cell_list *cells)
{
	cell_list_rewind(cells);
	cells->head.next = &cells->tail;
	cells->head.covered_height = 0;
	cells->count = 0;
}

inline static struct cell *
cell_list_alloc(struct cell_list *cells,
		struct cell *tail,
		int x)
{
	struct cell *cell;

	assert(cells->count < cells->size);
	cell = cells->cells + cells->count++;
	cell->next = tail->next;
	tail->next = cell;

	cell->x = x;
	cell->covered_height = 0;
	cell->uncovered_area = 0;
	return cell;
}

/* Find a cell at the given x-coordinate.  Returns %NULL if a new cell
 * needed to be allocated but couldn't be.  Cells must be found with
 * non-decreasing x-coordinate until the cell list is rewound using
 * cell_list_rewind(). Ownership of the returned cell is retained by
 * the cell list. */
inline static struct cell *
cell_list_find(struct cell_list *cells, int x)
{
	struct cell *tail;

	if (x >= cells->x2)
		return &cells->tail;

	if (x < cells->x1)
		return &cells->head;

	tail = cells->cursor;
	if (tail->x == x)
		return tail;

	do {
		if (tail->next->x > x)
			break;

		tail = tail->next;
		if (tail->next->x > x)
			break;

		tail = tail->next;
		if (tail->next->x > x)
			break;

		tail = tail->next;
	} while (1);

	if (tail->x != x)
		tail = cell_list_alloc(cells, tail, x);

	return cells->cursor = tail;
}

/* Add a subpixel span covering [x1, x2) to the coverage cells. */
inline static void
cell_list_add_subspan(struct cell_list *cells, int x1, int x2)
{
	struct cell *cell;
	int ix1, fx1;
	int ix2, fx2;

	if (x1 == x2)
		return;

	FAST_SAMPLES_X_TO_INT_FRAC(x1, ix1, fx1);
	FAST_SAMPLES_X_TO_INT_FRAC(x2, ix2, fx2);

	__DBG(("%s: x1=%d (%d+%d), x2=%d (%d+%d)\n", __FUNCTION__,
	       x1, ix1, fx1, x2, ix2, fx2));

	cell = cell_list_find(cells, ix1);
	if (ix1 != ix2) {
		cell->uncovered_area += 2*fx1;
		++cell->covered_height;

		cell = cell_list_find(cells, ix2);
		cell->uncovered_area -= 2*fx2;
		--cell->covered_height;
	} else
		cell->uncovered_area += 2*(fx1-fx2);
}

inline static void
cell_list_add_span(struct cell_list *cells, int x1, int x2)
{
	struct cell *cell;
	int ix1, fx1;
	int ix2, fx2;

	FAST_SAMPLES_X_TO_INT_FRAC(x1, ix1, fx1);
	FAST_SAMPLES_X_TO_INT_FRAC(x2, ix2, fx2);

	__DBG(("%s: x1=%d (%d+%d), x2=%d (%d+%d)\n", __FUNCTION__,
	       x1, ix1, fx1, x2, ix2, fx2));

	cell = cell_list_find(cells, ix1);
	if (ix1 != ix2) {
		cell->uncovered_area += 2*fx1*FAST_SAMPLES_Y;
		cell->covered_height += FAST_SAMPLES_Y;

		cell = cell_list_find(cells, ix2);
		cell->uncovered_area -= 2*fx2*FAST_SAMPLES_Y;
		cell->covered_height -= FAST_SAMPLES_Y;
	} else
		cell->uncovered_area += 2*(fx1-fx2)*FAST_SAMPLES_Y;
}

static void
polygon_fini(struct polygon *polygon)
{
	if (polygon->y_buckets != polygon->y_buckets_embedded)
		free(polygon->y_buckets);

	if (polygon->edges != polygon->edges_embedded)
		free(polygon->edges);
}

static bool
polygon_init(struct polygon *polygon, int num_edges, int ymin, int ymax)
{
	unsigned num_buckets = EDGE_Y_BUCKET_INDEX(ymax-1, ymin) + 1;

	if (unlikely(ymax - ymin > 0x7FFFFFFFU - EDGE_Y_BUCKET_HEIGHT))
		return false;

	polygon->edges = polygon->edges_embedded;
	polygon->y_buckets = polygon->y_buckets_embedded;

	polygon->num_edges = 0;
	if (num_edges > (int)ARRAY_SIZE(polygon->edges_embedded)) {
		polygon->edges = malloc(sizeof(struct edge)*num_edges);
		if (unlikely(NULL == polygon->edges))
			goto bail_no_mem;
	}

	if (num_buckets >= ARRAY_SIZE(polygon->y_buckets_embedded)) {
		polygon->y_buckets = malloc((1+num_buckets)*sizeof(struct edge *));
		if (unlikely(NULL == polygon->y_buckets))
			goto bail_no_mem;
	}
	memset(polygon->y_buckets, 0, num_buckets * sizeof(struct edge *));
	polygon->y_buckets[num_buckets] = (void *)-1;

	polygon->ymin = ymin;
	polygon->ymax = ymax;
	return true;

bail_no_mem:
	polygon_fini(polygon);
	return false;
}

static void
_polygon_insert_edge_into_its_y_bucket(struct polygon *polygon, struct edge *e)
{
	unsigned ix = EDGE_Y_BUCKET_INDEX(e->ytop, polygon->ymin);
	struct edge **ptail = &polygon->y_buckets[ix];
	assert(e->ytop < polygon->ymax);
	e->next = *ptail;
	*ptail = e;
}

inline static void
polygon_add_edge(struct polygon *polygon,
		 int x1, int x2,
		 int y1, int y2,
		 int top, int bottom,
		 int dir)
{
	struct edge *e = &polygon->edges[polygon->num_edges];
	int dx = x2 - x1;
	int dy = y2 - y1;
	int ytop, ybot;
	int ymin = polygon->ymin;
	int ymax = polygon->ymax;

	__DBG(("%s: edge=(%d [%d.%d], %d [%d.%d]), (%d [%d.%d], %d [%d.%d]), top=%d [%d.%d], bottom=%d [%d.%d], dir=%d\n",
	       __FUNCTION__,
	       x1, FAST_SAMPLES_INT(x1), FAST_SAMPLES_FRAC(x1),
	       y1, FAST_SAMPLES_INT(y1), FAST_SAMPLES_FRAC(y1),
	       x2, FAST_SAMPLES_INT(x2), FAST_SAMPLES_FRAC(x2),
	       y2, FAST_SAMPLES_INT(y2), FAST_SAMPLES_FRAC(y2),
	       top, FAST_SAMPLES_INT(top), FAST_SAMPLES_FRAC(top),
	       bottom, FAST_SAMPLES_INT(bottom), FAST_SAMPLES_FRAC(bottom),
	       dir));
	assert (dy > 0);

	e->dy = dy;
	e->dir = dir;

	ytop = top >= ymin ? top : ymin;
	ybot = bottom <= ymax ? bottom : ymax;
	e->ytop = ytop;
	e->height_left = ybot - ytop;
	if (e->height_left <= 0)
		return;

	if (dx == 0) {
		e->x.quo = x1;
		e->x.rem = 0;
		e->dy = 0;
		e->dxdy.quo = 0;
		e->dxdy.rem = 0;
	} else {
		e->dxdy = floored_divrem(dx, dy);
		if (ytop == y1) {
			e->x.quo = x1;
			e->x.rem = 0;
		} else {
			e->x = floored_muldivrem(ytop - y1, dx, dy);
			e->x.quo += x1;
		}
	}
	e->x.rem -= dy; /* Bias the remainder for faster edge advancement. */

	_polygon_insert_edge_into_its_y_bucket(polygon, e);
	polygon->num_edges++;
}

inline static void
polygon_add_line(struct polygon *polygon,
		 const xPointFixed *p1,
		 const xPointFixed *p2)
{
	struct edge *e = &polygon->edges[polygon->num_edges];
	int dx = p2->x - p1->x;
	int dy = p2->y - p1->y;
	int top, bot;

	if (dy == 0)
		return;

	__DBG(("%s: line=(%d, %d), (%d, %d)\n",
	       __FUNCTION__, (int)p1->x, (int)p1->y, (int)p2->x, (int)p2->y));

	e->dir = 1;
	if (dy < 0) {
		const xPointFixed *t;

		dx = -dx;
		dy = -dy;

		e->dir = -1;

		t = p1;
		p1 = p2;
		p2 = t;
	}
	assert (dy > 0);
	e->dy = dy;

	top = MAX(p1->y, polygon->ymin);
	bot = MIN(p2->y, polygon->ymax);
	if (bot <= top)
		return;

	e->ytop = top;
	e->height_left = bot - top;
	if (e->height_left <= 0)
		return;

	if (dx == 0) {
		e->x.quo = p1->x;
		e->x.rem = -dy;
		e->dxdy.quo = 0;
		e->dxdy.rem = 0;
		e->dy = 0;
	} else {
		e->dxdy = floored_divrem(dx, dy);
		if (top == p1->y) {
			e->x.quo = p1->x;
			e->x.rem = -dy;
		} else {
			e->x = floored_muldivrem(top - p1->y, dx, dy);
			e->x.quo += p1->x;
			e->x.rem -= dy;
		}
	}

	if (polygon->num_edges > 0) {
		struct edge *prev = &polygon->edges[polygon->num_edges-1];
		/* detect degenerate triangles inserted into tristrips */
		if (e->dir == -prev->dir &&
		    e->ytop == prev->ytop &&
		    e->height_left == prev->height_left &&
		    e->x.quo == prev->x.quo &&
		    e->x.rem == prev->x.rem &&
		    e->dxdy.quo == prev->dxdy.quo &&
		    e->dxdy.rem == prev->dxdy.rem) {
			unsigned ix = EDGE_Y_BUCKET_INDEX(e->ytop,
							  polygon->ymin);
			polygon->y_buckets[ix] = prev->next;
			return;
		}
	}

	_polygon_insert_edge_into_its_y_bucket(polygon, e);
	polygon->num_edges++;
}

static void
active_list_reset(struct active_list *active)
{
	active->head.height_left = INT_MAX;
	active->head.x.quo = INT_MIN;
	active->head.dy = 0;
	active->head.prev = NULL;
	active->head.next = &active->tail;
	active->tail.prev = &active->head;
	active->tail.next = NULL;
	active->tail.x.quo = INT_MAX;
	active->tail.height_left = INT_MAX;
	active->tail.dy = 0;
}

static struct edge *
merge_sorted_edges(struct edge *head_a, struct edge *head_b)
{
	struct edge *head, **next, *prev;
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

static struct edge *
sort_edges(struct edge  *list,
	   unsigned int  level,
	   struct edge **head_out)
{
	struct edge *head_other, *remaining;
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
		remaining = sort_edges(remaining, i, &head_other);
		*head_out = merge_sorted_edges(*head_out, head_other);
	}

	return remaining;
}

static struct edge *filter(struct edge *edges)
{
	struct edge *e;

	e = edges;
	do {
		struct edge *n = e->next;
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

static struct edge *
merge_unsorted_edges(struct edge *head, struct edge *unsorted)
{
	sort_edges(unsorted, UINT_MAX, &unsorted);
	return merge_sorted_edges(head, filter(unsorted));
}

/* Test if the edges on the active list can be safely advanced by a
 * full row without intersections or any edges ending. */
inline static int
can_full_step(struct active_list *active)
{
	const struct edge *e;
	int min_height = INT_MAX;

	assert(active->head.next != &active->tail);
	for (e = active->head.next; &active->tail != e; e = e->next) {
		assert(e->height_left > 0);

		if (e->dy != 0)
			return 0;

		if (e->height_left < min_height) {
			min_height = e->height_left;
			if (min_height < FAST_SAMPLES_Y)
				return 0;
		}
	}

	return min_height;
}

inline static void
merge_edges(struct active_list *active, struct edge *edges)
{
	active->head.next = merge_unsorted_edges(active->head.next, edges);
}

inline static void
fill_buckets(struct active_list *active,
	     struct edge *edge,
	     struct edge **buckets)
{
	while (edge) {
		struct edge *next = edge->next;
		struct edge **b = &buckets[edge->ytop & (FAST_SAMPLES_Y-1)];
		if (*b)
			(*b)->prev = edge;
		edge->next = *b;
		edge->prev = NULL;
		*b = edge;
		edge = next;
	}
}

inline static void
nonzero_subrow(struct active_list *active, struct cell_list *coverages)
{
	struct edge *edge = active->head.next;
	int prev_x = INT_MIN;
	int winding = 0, xstart = edge->x.quo;

	cell_list_rewind(coverages);

	while (&active->tail != edge) {
		struct edge *next = edge->next;

		winding += edge->dir;
		if (0 == winding && edge->next->x.quo != edge->x.quo) {
			cell_list_add_subspan(coverages,
					      xstart, edge->x.quo);
			xstart = edge->next->x.quo;
		}

		assert(edge->height_left > 0);
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
				struct edge *pos = edge->prev;
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

		edge = next;
	}
}

static void
nonzero_row(struct active_list *active, struct cell_list *coverages)
{
	struct edge *left = active->head.next;

	while (&active->tail != left) {
		struct edge *right;
		int winding = left->dir;

		left->height_left -= FAST_SAMPLES_Y;
		assert(left->height_left >= 0);
		if (!left->height_left) {
			left->prev->next = left->next;
			left->next->prev = left->prev;
		}

		right = left->next;
		do {
			right->height_left -= FAST_SAMPLES_Y;
			assert(right->height_left >= 0);
			if (!right->height_left) {
				right->prev->next = right->next;
				right->next->prev = right->prev;
			}

			winding += right->dir;
			if (0 == winding)
				break;

			right = right->next;
		} while (1);

		cell_list_add_span(coverages, left->x.quo, right->x.quo);
		left = right->next;
	}
}

static void
tor_fini(struct tor *converter)
{
	polygon_fini(converter->polygon);
	cell_list_fini(converter->coverages);
}

static bool
tor_init(struct tor *converter, const BoxRec *box, int num_edges)
{
	__DBG(("%s: (%d, %d),(%d, %d) x (%d, %d), num_edges=%d\n",
	       __FUNCTION__,
	       box->x1, box->y1, box->x2, box->y2,
	       FAST_SAMPLES_X, FAST_SAMPLES_Y,
	       num_edges));

	converter->extents = *box;

	if (!cell_list_init(converter->coverages, box->x1, box->x2))
		return false;

	active_list_reset(converter->active);
	if (!polygon_init(converter->polygon, num_edges,
			  (int)box->y1 * FAST_SAMPLES_Y,
			  (int)box->y2 * FAST_SAMPLES_Y)) {
		cell_list_fini(converter->coverages);
		return false;
	}

	return true;
}

static void
tor_add_edge(struct tor *converter,
	     const xTrapezoid *t,
	     const xLineFixed *edge,
	     int dir)
{
	polygon_add_edge(converter->polygon,
			 edge->p1.x, edge->p2.x,
			 edge->p1.y, edge->p2.y,
			 t->top, t->bottom,
			 dir);
}

static void
step_edges(struct active_list *active, int count)
{
	struct edge *edge;

	count *= FAST_SAMPLES_Y;
	for (edge = active->head.next; edge != &active->tail; edge = edge->next) {
		edge->height_left -= count;
		assert(edge->height_left >= 0);
		if (!edge->height_left) {
			edge->prev->next = edge->next;
			edge->next->prev = edge->prev;
		}
	}
}

static void
tor_blt_span(struct sna *sna,
	     struct sna_composite_spans_op *op,
	     pixman_region16_t *clip,
	     const BoxRec *box,
	     int coverage)
{
	__DBG(("%s: %d -> %d @ %d\n", __FUNCTION__, box->x1, box->x2, coverage));

	op->box(sna, op, box, AREA_TO_ALPHA(coverage));
	apply_damage_box(&op->base, box);
}

static void
tor_blt_span__no_damage(struct sna *sna,
			struct sna_composite_spans_op *op,
			pixman_region16_t *clip,
			const BoxRec *box,
			int coverage)
{
	__DBG(("%s: %d -> %d @ %d\n", __FUNCTION__, box->x1, box->x2, coverage));

	op->box(sna, op, box, AREA_TO_ALPHA(coverage));
}

static void
tor_blt_span_clipped(struct sna *sna,
		     struct sna_composite_spans_op *op,
		     pixman_region16_t *clip,
		     const BoxRec *box,
		     int coverage)
{
	pixman_region16_t region;
	float opacity;

	opacity = AREA_TO_ALPHA(coverage);
	__DBG(("%s: %d -> %d @ %f\n", __FUNCTION__, box->x1, box->x2, opacity));

	pixman_region_init_rects(&region, box, 1);
	RegionIntersect(&region, &region, clip);
	if (region_num_rects(&region)) {
		op->boxes(sna, op,
			  region_rects(&region),
			  region_num_rects(&region),
			  opacity);
		apply_damage(&op->base, &region);
	}
	pixman_region_fini(&region);
}

static void
tor_blt_span_mono(struct sna *sna,
		  struct sna_composite_spans_op *op,
		  pixman_region16_t *clip,
		  const BoxRec *box,
		  int coverage)
{
	if (coverage < FAST_SAMPLES_XY/2)
		return;

	tor_blt_span(sna, op, clip, box, FAST_SAMPLES_XY);
}

static void
tor_blt_span_mono_clipped(struct sna *sna,
			  struct sna_composite_spans_op *op,
			  pixman_region16_t *clip,
			  const BoxRec *box,
			  int coverage)
{
	if (coverage < FAST_SAMPLES_XY/2)
		return;

	tor_blt_span_clipped(sna, op, clip, box, FAST_SAMPLES_XY);
}

static void
tor_blt_span_mono_unbounded(struct sna *sna,
			    struct sna_composite_spans_op *op,
			    pixman_region16_t *clip,
			    const BoxRec *box,
			    int coverage)
{
	tor_blt_span(sna, op, clip, box,
		     coverage < FAST_SAMPLES_XY/2 ? 0 : FAST_SAMPLES_XY);
}

static void
tor_blt_span_mono_unbounded_clipped(struct sna *sna,
				    struct sna_composite_spans_op *op,
				    pixman_region16_t *clip,
				    const BoxRec *box,
				    int coverage)
{
	tor_blt_span_clipped(sna, op, clip, box,
			     coverage < FAST_SAMPLES_XY/2 ? 0 : FAST_SAMPLES_XY);
}

static void
tor_blt(struct sna *sna,
	struct tor *converter,
	struct sna_composite_spans_op *op,
	pixman_region16_t *clip,
	void (*span)(struct sna *sna,
		     struct sna_composite_spans_op *op,
		     pixman_region16_t *clip,
		     const BoxRec *box,
		     int coverage),
	int y, int height,
	int unbounded)
{
	struct cell_list *cells = converter->coverages;
	struct cell *cell;
	BoxRec box;
	int cover;

	box.y1 = y + converter->extents.y1;
	box.y2 = box.y1 + height;
	assert(box.y2 <= converter->extents.y2);
	box.x1 = converter->extents.x1;

	/* Form the spans from the coverages and areas. */
	cover = cells->head.covered_height*FAST_SAMPLES_X*2;
	assert(cover >= 0);
	for (cell = cells->head.next; cell != &cells->tail; cell = cell->next) {
		int x = cell->x;

		assert(x >= converter->extents.x1);
		assert(x < converter->extents.x2);
		__DBG(("%s: cell=(%d, %d, %d), cover=%d, max=%d\n", __FUNCTION__,
		       cell->x, cell->covered_height, cell->uncovered_area,
		       cover, xmax));

		if (cell->covered_height || cell->uncovered_area) {
			box.x2 = x;
			if (box.x2 > box.x1 && (unbounded || cover)) {
				__DBG(("%s: span (%d, %d)x(%d, %d) @ %d\n", __FUNCTION__,
				       box.x1, box.y1,
				       box.x2 - box.x1,
				       box.y2 - box.y1,
				       cover));
				span(sna, op, clip, &box, cover);
			}
			box.x1 = box.x2;
			cover += cell->covered_height*FAST_SAMPLES_X*2;
		}

		if (cell->uncovered_area) {
			int area = cover - cell->uncovered_area;
			box.x2 = x + 1;
			if (unbounded || area) {
				__DBG(("%s: span (%d, %d)x(%d, %d) @ %d\n", __FUNCTION__,
				       box.x1, box.y1,
				       box.x2 - box.x1,
				       box.y2 - box.y1,
				       area));
				span(sna, op, clip, &box, area);
			}
			box.x1 = box.x2;
		}
	}

	box.x2 = converter->extents.x2;
	if (box.x2 > box.x1 && (unbounded || cover)) {
		__DBG(("%s: span (%d, %d)x(%d, %d) @ %d\n", __FUNCTION__,
		       box.x1, box.y1,
		       box.x2 - box.x1,
		       box.y2 - box.y1,
		       cover));
		span(sna, op, clip, &box, cover);
	}
}

flatten static void
tor_render(struct sna *sna,
	   struct tor *converter,
	   struct sna_composite_spans_op *op,
	   pixman_region16_t *clip,
	   void (*span)(struct sna *sna,
			struct sna_composite_spans_op *op,
			pixman_region16_t *clip,
			const BoxRec *box,
			int coverage),
	   int unbounded)
{
	struct polygon *polygon = converter->polygon;
	struct cell_list *coverages = converter->coverages;
	struct active_list *active = converter->active;
	struct edge *buckets[FAST_SAMPLES_Y] = { 0 };
	int16_t i, j, h = converter->extents.y2 - converter->extents.y1;

	__DBG(("%s: unbounded=%d\n", __FUNCTION__, unbounded));

	/* Render each pixel row. */
	for (i = 0; i < h; i = j) {
		int do_full_step = 0;

		j = i + 1;

		/* Determine if we can ignore this row or use the full pixel
		 * stepper. */
		if (polygon->y_buckets[i] == NULL) {
			if (active->head.next == &active->tail) {
				for (; polygon->y_buckets[j] == NULL; j++)
					;
				__DBG(("%s: no new edges and no exisiting edges, skipping, %d -> %d\n",
				       __FUNCTION__, i, j));

				assert(j <= h);
				if (unbounded) {
					BoxRec box;

					box = converter->extents;
					box.y1 += i;
					box.y2 = converter->extents.y1 + j;

					span(sna, op, clip, &box, 0);
				}
				continue;
			}

			do_full_step = can_full_step(active);
		}

		__DBG(("%s: y=%d [%d], do_full_step=%d, new edges=%d, min_height=%d, vertical=%d\n",
		       __FUNCTION__,
		       i, i+ymin, do_full_step,
		       polygon->y_buckets[i] != NULL));
		if (do_full_step) {
			nonzero_row(active, coverages);

			while (polygon->y_buckets[j] == NULL &&
			       do_full_step >= 2*FAST_SAMPLES_Y) {
				do_full_step -= FAST_SAMPLES_Y;
				j++;
			}
			assert(j >= i + 1 && j <= h);
			if (j != i + 1)
				step_edges(active, j - (i + 1));

			__DBG(("%s: vertical edges, full step (%d, %d)\n",
			       __FUNCTION__,  i, j));
		} else {
			int suby;

			fill_buckets(active, polygon->y_buckets[i], buckets);

			/* Subsample this row. */
			for (suby = 0; suby < FAST_SAMPLES_Y; suby++) {
				if (buckets[suby]) {
					merge_edges(active, buckets[suby]);
					buckets[suby] = NULL;
				}

				nonzero_subrow(active, coverages);
			}
		}

		assert(j > i);
		tor_blt(sna, converter, op, clip, span, i, j-i, unbounded);
		cell_list_reset(coverages);
	}
}

static void
inplace_row(struct active_list *active, uint8_t *row, int width)
{
	struct edge *left = active->head.next;

	while (&active->tail != left) {
		struct edge *right;
		int winding = left->dir;
		int lfx, rfx;
		int lix, rix;

		left->height_left -= FAST_SAMPLES_Y;
		assert(left->height_left >= 0);
		if (!left->height_left) {
			left->prev->next = left->next;
			left->next->prev = left->prev;
		}

		right = left->next;
		do {
			right->height_left -= FAST_SAMPLES_Y;
			assert(right->height_left >= 0);
			if (!right->height_left) {
				right->prev->next = right->next;
				right->next->prev = right->prev;
			}

			winding += right->dir;
			if (0 == winding && right->x.quo != right->next->x.quo)
				break;

			right = right->next;
		} while (1);

		if (left->x.quo < 0) {
			lix = lfx = 0;
		} else if (left->x.quo >= width * FAST_SAMPLES_X) {
			lix = width;
			lfx = 0;
		} else
			FAST_SAMPLES_X_TO_INT_FRAC(left->x.quo, lix, lfx);

		if (right->x.quo < 0) {
			rix = rfx = 0;
		} else if (right->x.quo >= width * FAST_SAMPLES_X) {
			rix = width;
			rfx = 0;
		} else
			FAST_SAMPLES_X_TO_INT_FRAC(right->x.quo, rix, rfx);
		if (lix == rix) {
			if (rfx != lfx) {
				assert(lix < width);
				row[lix] += (rfx-lfx) * 256 / FAST_SAMPLES_X;
			}
		} else {
			assert(lix < width);
			if (lfx == 0)
				row[lix] = 0xff;
			else
				row[lix] += 256 - lfx * 256 / FAST_SAMPLES_X;

			assert(rix <= width);
			if (rfx) {
				assert(rix < width);
				row[rix] += rfx * 256 / FAST_SAMPLES_X;
			}

			if (rix > ++lix) {
				uint8_t *r = row + lix;
				rix -= lix;
#if 0
				if (rix == 1)
					*row = 0xff;
				else
					memset(row, 0xff, rix);
#else
				if ((uintptr_t)r & 1 && rix) {
					*r++ = 0xff;
					rix--;
				}
				if ((uintptr_t)r & 2 && rix >= 2) {
					*(uint16_t *)r = 0xffff;
					r += 2;
					rix -= 2;
				}
				if ((uintptr_t)r & 4 && rix >= 4) {
					*(uint32_t *)r = 0xffffffff;
					r += 4;
					rix -= 4;
				}
				while (rix >= 8) {
					*(uint64_t *)r = 0xffffffffffffffff;
					r += 8;
					rix -= 8;
				}
				if (rix & 4) {
					*(uint32_t *)r = 0xffffffff;
					r += 4;
				}
				if (rix & 2) {
					*(uint16_t *)r = 0xffff;
					r += 2;
				}
				if (rix & 1)
					*r = 0xff;
#endif
			}
		}

		left = right->next;
	}
}

inline static void
inplace_subrow(struct active_list *active, int8_t *row,
	       int width, int *min, int *max)
{
	struct edge *edge = active->head.next;
	int prev_x = INT_MIN;
	int winding = 0, xstart = INT_MIN;

	while (&active->tail != edge) {
		struct edge *next = edge->next;

		winding += edge->dir;
		if (0 == winding) {
			if (edge->next->x.quo != edge->x.quo) {
				if (edge->x.quo <= xstart) {
					xstart = INT_MIN;
				} else  {
					int fx;
					int ix;

					if (xstart < FAST_SAMPLES_X * width) {
						FAST_SAMPLES_X_TO_INT_FRAC(xstart, ix, fx);
						if (ix < *min)
							*min = ix;

						row[ix++] += FAST_SAMPLES_X - fx;
						if (fx && ix < width)
							row[ix] += fx;
					}

					xstart = edge->x.quo;
					if (xstart < FAST_SAMPLES_X * width) {
						FAST_SAMPLES_X_TO_INT_FRAC(xstart, ix, fx);
						row[ix] -= FAST_SAMPLES_X - fx;
						if (fx && ix + 1 < width)
							row[++ix] -= fx;

						if (ix >= *max)
							*max = ix + 1;

						xstart = INT_MIN;
					} else
						*max = width;
				}
			}
		} else if (xstart < 0) {
			xstart = MAX(edge->x.quo, 0);
		}

		assert(edge->height_left > 0);
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
				struct edge *pos = edge->prev;
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

		edge = next;
	}
}

inline static void
inplace_end_subrows(struct active_list *active, uint8_t *row,
		    int8_t *buf, int width)
{
	int cover = 0;

	while (width >= 4) {
		uint32_t dw;
		int v;

		dw = *(uint32_t *)buf;
		buf += 4;

		if (dw == 0) {
			v = cover * 256 / (FAST_SAMPLES_X * FAST_SAMPLES_Y);
			v -= v >> 8;
			v |= v << 8;
			dw = v | v << 16;
		} else {
			cover += (int8_t)(dw & 0xff);
			if (cover) {
				assert(cover > 0);
				v = cover * 256 / (FAST_SAMPLES_X * FAST_SAMPLES_Y);
				v -= v >> 8;
				dw >>= 8;
				dw |= v << 24;
			} else
				dw >>= 8;

			cover += (int8_t)(dw & 0xff);
			if (cover) {
				assert(cover > 0);
				v = cover * 256 / (FAST_SAMPLES_X * FAST_SAMPLES_Y);
				v -= v >> 8;
				dw >>= 8;
				dw |= v << 24;
			} else
				dw >>= 8;

			cover += (int8_t)(dw & 0xff);
			if (cover) {
				assert(cover > 0);
				v = cover * 256 / (FAST_SAMPLES_X * FAST_SAMPLES_Y);
				v -= v >> 8;
				dw >>= 8;
				dw |= v << 24;
			} else
				dw >>= 8;

			cover += (int8_t)(dw & 0xff);
			if (cover) {
				assert(cover > 0);
				v = cover * 256 / (FAST_SAMPLES_X * FAST_SAMPLES_Y);
				v -= v >> 8;
				dw >>= 8;
				dw |= v << 24;
			} else
				dw >>= 8;
		}

		*(uint32_t *)row = dw;
		row += 4;
		width -= 4;
	}

	while (width--) {
		int v;

		cover += *buf++;
		assert(cover >= 0);

		v = cover * 256 / (FAST_SAMPLES_X * FAST_SAMPLES_Y);
		v -= v >> 8;
		*row++ = v;
	}
}

static void
convert_mono(uint8_t *ptr, int w)
{
	while (w--) {
		*ptr = 0xff * (*ptr >= 0xf0);
		ptr++;
	}
}

static void
tor_inplace(struct tor *converter, PixmapPtr scratch, int mono, uint8_t *buf)
{
	int i, j, h = converter->extents.y2;
	struct polygon *polygon = converter->polygon;
	struct active_list *active = converter->active;
	struct edge *buckets[FAST_SAMPLES_Y] = { 0 };
	uint8_t *row = scratch->devPrivate.ptr;
	int stride = scratch->devKind;
	int width = scratch->drawable.width;

	__DBG(("%s: mono=%d, buf?=%d\n", __FUNCTION__, mono, buf != NULL));
	assert(converter->extents.y1 == 0);
	assert(converter->extents.x1 == 0);
	assert(scratch->drawable.depth == 8);

	/* Render each pixel row. */
	for (i = 0; i < h; i = j) {
		int do_full_step = 0;
		void *ptr = buf ?: row;

		j = i + 1;

		/* Determine if we can ignore this row or use the full pixel
		 * stepper. */
		if (!polygon->y_buckets[i]) {
			if (active->head.next == &active->tail) {
				for (; !polygon->y_buckets[j]; j++)
					;
				__DBG(("%s: no new edges and no exisiting edges, skipping, %d -> %d\n",
				       __FUNCTION__, i, j));

				memset(row, 0, stride*(j-i));
				row += stride*(j-i);
				continue;
			}

			do_full_step = can_full_step(active);
		}

		__DBG(("%s: y=%d, do_full_step=%d, new edges=%d, min_height=%d, vertical=%d\n",
		       __FUNCTION__,
		       i, do_full_step,
		       polygon->y_buckets[i] != NULL));
		if (do_full_step) {
			memset(ptr, 0, width);
			inplace_row(active, ptr, width);
			if (mono)
				convert_mono(ptr, width);
			if (row != ptr)
				memcpy(row, ptr, width);

			while (polygon->y_buckets[j] == NULL &&
			       do_full_step >= 2*FAST_SAMPLES_Y) {
				do_full_step -= FAST_SAMPLES_Y;
				row += stride;
				memcpy(row, ptr, width);
				j++;
			}
			if (j != i + 1)
				step_edges(active, j - (i + 1));

			__DBG(("%s: vertical edges, full step (%d, %d)\n",
			       __FUNCTION__,  i, j));
		} else {
			int min = width, max = 0, suby;

			fill_buckets(active, polygon->y_buckets[i], buckets);

			/* Subsample this row. */
			memset(ptr, 0, width);
			for (suby = 0; suby < FAST_SAMPLES_Y; suby++) {
				if (buckets[suby]) {
					merge_edges(active, buckets[suby]);
					buckets[suby] = NULL;
				}

				inplace_subrow(active, ptr, width, &min, &max);
			}
			assert(min >= 0 && max <= width);
			memset(row, 0, min);
			if (max > min) {
				inplace_end_subrows(active, row+min, (int8_t*)ptr+min, max-min);
				if (mono)
					convert_mono(row+min, max-min);
			}
			if (max < width)
				memset(row+max, 0, width-max);
		}

		row += stride;
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

static inline bool
project_trapezoid_onto_grid(const xTrapezoid *in,
			    int dx, int dy,
			    xTrapezoid *out)
{
	__DBG(("%s: in: L:(%d, %d), (%d, %d); R:(%d, %d), (%d, %d), [%d, %d]\n",
	       __FUNCTION__,
	       in->left.p1.x, in->left.p1.y, in->left.p2.x, in->left.p2.y,
	       in->right.p1.x, in->right.p1.y, in->right.p2.x, in->right.p2.y,
	       in->top, in->bottom));

	out->left.p1.x = dx + pixman_fixed_to_fast(in->left.p1.x);
	out->left.p1.y = dy + pixman_fixed_to_fast(in->left.p1.y);
	out->left.p2.x = dx + pixman_fixed_to_fast(in->left.p2.x);
	out->left.p2.y = dy + pixman_fixed_to_fast(in->left.p2.y);

	out->right.p1.x = dx + pixman_fixed_to_fast(in->right.p1.x);
	out->right.p1.y = dy + pixman_fixed_to_fast(in->right.p1.y);
	out->right.p2.x = dx + pixman_fixed_to_fast(in->right.p2.x);
	out->right.p2.y = dy + pixman_fixed_to_fast(in->right.p2.y);

	out->top = dy + pixman_fixed_to_fast(in->top);
	out->bottom = dy + pixman_fixed_to_fast(in->bottom);

	__DBG(("%s: out: L:(%d, %d), (%d, %d); R:(%d, %d), (%d, %d), [%d, %d]\n",
	       __FUNCTION__,
	       out->left.p1.x, out->left.p1.y, out->left.p2.x, out->left.p2.y,
	       out->right.p1.x, out->right.p1.y, out->right.p2.x, out->right.p2.y,
	       out->top, out->bottom));

	return xTrapezoidValid(out);
}

static span_func_t
choose_span(struct sna_composite_spans_op *tmp,
	    PicturePtr dst,
	    PictFormatPtr maskFormat,
	    RegionPtr clip)
{
	span_func_t span;

	if (is_mono(dst, maskFormat)) {
		/* XXX An imprecise approximation */
		if (maskFormat && !operator_is_bounded(tmp->base.op)) {
			span = tor_blt_span_mono_unbounded;
			if (clip->data)
				span = tor_blt_span_mono_unbounded_clipped;
		} else {
			span = tor_blt_span_mono;
			if (clip->data)
				span = tor_blt_span_mono_clipped;
		}
	} else {
		if (clip->data)
			span = tor_blt_span_clipped;
		else if (tmp->base.damage == NULL)
			span = tor_blt_span__no_damage;
		else
			span = tor_blt_span;
	}

	return span;
}

struct span_thread {
	struct sna *sna;
	const struct sna_composite_spans_op *op;
	const xTrapezoid *traps;
	RegionPtr clip;
	span_func_t span;
	BoxRec extents;
	int dx, dy, draw_y;
	int ntrap;
	bool unbounded;
};

#define SPAN_THREAD_MAX_BOXES (8192/sizeof(struct sna_opacity_box))
struct span_thread_boxes {
	const struct sna_composite_spans_op *op;
	int num_boxes;
	struct sna_opacity_box boxes[SPAN_THREAD_MAX_BOXES];
};

static void span_thread_add_boxes(struct sna *sna, void *data,
				  const BoxRec *box, int count, float alpha)
{
	struct span_thread_boxes *b = data;

	__DBG(("%s: adding %d boxes with alpha=%f\n",
	       __FUNCTION__, count, alpha));

	assert(count > 0 && count <= SPAN_THREAD_MAX_BOXES);
	if (unlikely(b->num_boxes + count > SPAN_THREAD_MAX_BOXES)) {
		DBG(("%s: flushing %d boxes, adding %d\n", __FUNCTION__, b->num_boxes, count));
		assert(b->num_boxes <= SPAN_THREAD_MAX_BOXES);
		b->op->thread_boxes(sna, b->op, b->boxes, b->num_boxes);
		b->num_boxes = 0;
	}

	do {
		b->boxes[b->num_boxes].box = *box++;
		b->boxes[b->num_boxes].alpha = alpha;
		b->num_boxes++;
	} while (--count);
	assert(b->num_boxes <= SPAN_THREAD_MAX_BOXES);
}

static void
span_thread_box(struct sna *sna,
		struct sna_composite_spans_op *op,
		pixman_region16_t *clip,
		const BoxRec *box,
		int coverage)
{
	__DBG(("%s: %d -> %d @ %d\n", __FUNCTION__, box->x1, box->x2, coverage));
	span_thread_add_boxes(sna, op, box, 1, AREA_TO_ALPHA(coverage));
}

static void
span_thread_clipped_box(struct sna *sna,
			struct sna_composite_spans_op *op,
			pixman_region16_t *clip,
			const BoxRec *box,
			int coverage)
{
	pixman_region16_t region;

	__DBG(("%s: %d -> %d @ %f\n", __FUNCTION__, box->x1, box->x2,
	       AREA_TO_ALPHA(coverage)));

	pixman_region_init_rects(&region, box, 1);
	RegionIntersect(&region, &region, clip);
	if (region_num_rects(&region)) {
		span_thread_add_boxes(sna, op,
				      region_rects(&region),
				      region_num_rects(&region),
				      AREA_TO_ALPHA(coverage));
	}
	pixman_region_fini(&region);
}

static span_func_t
thread_choose_span(struct sna_composite_spans_op *tmp,
		   PicturePtr dst,
		   PictFormatPtr maskFormat,
		   RegionPtr clip)
{
	span_func_t span;

	if (tmp->base.damage) {
		DBG(("%s: damaged -> no thread support\n", __FUNCTION__));
		return NULL;
	}

	if (is_mono(dst, maskFormat)) {
		DBG(("%s: mono rendering -> no thread support\n", __FUNCTION__));
		return NULL;
	} else {
		assert(tmp->thread_boxes);
		DBG(("%s: clipped? %d\n", __FUNCTION__, clip->data != NULL));
		if (clip->data)
			span = span_thread_clipped_box;
		else
			span = span_thread_box;
	}

	return span;
}

static void
span_thread(void *arg)
{
	struct span_thread *thread = arg;
	struct span_thread_boxes boxes;
	struct tor tor;
	const xTrapezoid *t;
	int n, y1, y2;

	if (!tor_init(&tor, &thread->extents, 2*thread->ntrap))
		return;

	boxes.op = thread->op;
	boxes.num_boxes = 0;

	y1 = thread->extents.y1 - thread->draw_y;
	y2 = thread->extents.y2 - thread->draw_y;
	for (n = thread->ntrap, t = thread->traps; n--; t++) {
		xTrapezoid tt;

		if (pixman_fixed_integer_floor(t->top) >= y2 ||
		    pixman_fixed_integer_ceil(t->bottom) <= y1)
			continue;

		if (!project_trapezoid_onto_grid(t, thread->dx, thread->dy, &tt))
			continue;

		tor_add_edge(&tor, &tt, &tt.left, 1);
		tor_add_edge(&tor, &tt, &tt.right, -1);
	}

	tor_render(thread->sna, &tor,
		   (struct sna_composite_spans_op *)&boxes, thread->clip,
		   thread->span, thread->unbounded);

	tor_fini(&tor);

	if (boxes.num_boxes) {
		DBG(("%s: flushing %d boxes\n", __FUNCTION__, boxes.num_boxes));
		assert(boxes.num_boxes <= SPAN_THREAD_MAX_BOXES);
		thread->op->thread_boxes(thread->sna, thread->op,
					 boxes.boxes, boxes.num_boxes);
	}
}

bool
imprecise_trapezoid_span_converter(struct sna *sna,
				   CARD8 op, PicturePtr src, PicturePtr dst,
				   PictFormatPtr maskFormat, unsigned int flags,
				   INT16 src_x, INT16 src_y,
				   int ntrap, xTrapezoid *traps)
{
	struct sna_composite_spans_op tmp;
	pixman_region16_t clip;
	int16_t dst_x, dst_y;
	bool was_clear;
	int dx, dy, n;
	int num_threads;

	if (NO_IMPRECISE)
		return false;

	if (!sna->render.check_composite_spans(sna, op, src, dst, 0, 0, flags)) {
		DBG(("%s: fallback -- composite spans not supported\n",
		     __FUNCTION__));
		return false;
	}

	if (!trapezoids_bounds(ntrap, traps, &clip.extents))
		return true;

#if 0
	if (clip.extents.y2 - clip.extents.y1 < 64 &&
	    clip.extents.x2 - clip.extents.x1 < 64) {
		DBG(("%s: fallback -- traps extents too small %dx%d\n",
		     __FUNCTION__, extents.y2 - extents.y1, extents.x2 - extents.x1));
		return false;
	}
#endif

	DBG(("%s: extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     clip.extents.x1, clip.extents.y1,
	     clip.extents.x2, clip.extents.y2));

	trapezoid_origin(&traps[0].left, &dst_x, &dst_y);

	if (!sna_compute_composite_region(&clip,
					  src, NULL, dst,
					  src_x + clip.extents.x1 - dst_x,
					  src_y + clip.extents.y1 - dst_y,
					  0, 0,
					  clip.extents.x1, clip.extents.y1,
					  clip.extents.x2 - clip.extents.x1,
					  clip.extents.y2 - clip.extents.y1)) {
		DBG(("%s: trapezoids do not intersect drawable clips\n",
		     __FUNCTION__)) ;
		return true;
	}

	if (!sna->render.check_composite_spans(sna, op, src, dst,
					       clip.extents.x2 - clip.extents.x1,
					       clip.extents.y2 - clip.extents.y1,
					       flags)) {
		DBG(("%s: fallback -- composite spans not supported\n",
		     __FUNCTION__));
		return false;
	}

	dx = dst->pDrawable->x;
	dy = dst->pDrawable->y;

	DBG(("%s: after clip -- extents (%d, %d), (%d, %d), delta=(%d, %d) src -> (%d, %d)\n",
	     __FUNCTION__,
	     clip.extents.x1, clip.extents.y1,
	     clip.extents.x2, clip.extents.y2,
	     dx, dy,
	     src_x + clip.extents.x1 - dst_x - dx,
	     src_y + clip.extents.y1 - dst_y - dy));

	was_clear = sna_drawable_is_clear(dst->pDrawable);
	switch (op) {
	case PictOpAdd:
	case PictOpOver:
		if (was_clear)
			op = PictOpSrc;
		break;
	case PictOpIn:
		if (was_clear)
			return true;
		break;
	}

	if (!sna->render.composite_spans(sna, op, src, dst,
					 src_x + clip.extents.x1 - dst_x - dx,
					 src_y + clip.extents.y1 - dst_y - dy,
					 clip.extents.x1,  clip.extents.y1,
					 clip.extents.x2 - clip.extents.x1,
					 clip.extents.y2 - clip.extents.y1,
					 flags, memset(&tmp, 0, sizeof(tmp)))) {
		DBG(("%s: fallback -- composite spans render op not supported\n",
		     __FUNCTION__));
		return false;
	}

	dx *= FAST_SAMPLES_X;
	dy *= FAST_SAMPLES_Y;

	num_threads = 1;
	if (!NO_GPU_THREADS &&
	    (flags & COMPOSITE_SPANS_RECTILINEAR) == 0 &&
	    tmp.thread_boxes &&
	    thread_choose_span(&tmp, dst, maskFormat, &clip))
		num_threads = sna_use_threads(clip.extents.x2-clip.extents.x1,
					      clip.extents.y2-clip.extents.y1,
					      16);
	DBG(("%s: using %d threads\n", __FUNCTION__, num_threads));
	if (num_threads == 1) {
		struct tor tor;

		if (!tor_init(&tor, &clip.extents, 2*ntrap))
			goto skip;

		for (n = 0; n < ntrap; n++) {
			xTrapezoid t;

			if (pixman_fixed_integer_floor(traps[n].top) + dst->pDrawable->y >= clip.extents.y2 ||
			    pixman_fixed_integer_ceil(traps[n].bottom) + dst->pDrawable->y <= clip.extents.y1)
				continue;

			if (!project_trapezoid_onto_grid(&traps[n], dx, dy, &t))
				continue;

			tor_add_edge(&tor, &t, &t.left, 1);
			tor_add_edge(&tor, &t, &t.right, -1);
		}

		tor_render(sna, &tor, &tmp, &clip,
			   choose_span(&tmp, dst, maskFormat, &clip),
			   !was_clear && maskFormat && !operator_is_bounded(op));

		tor_fini(&tor);
	} else {
		struct span_thread threads[num_threads];
		int y, h;

		DBG(("%s: using %d threads for span compositing %dx%d\n",
		     __FUNCTION__, num_threads,
		     clip.extents.x2 - clip.extents.x1,
		     clip.extents.y2 - clip.extents.y1));

		threads[0].sna = sna;
		threads[0].op = &tmp;
		threads[0].traps = traps;
		threads[0].ntrap = ntrap;
		threads[0].extents = clip.extents;
		threads[0].clip = &clip;
		threads[0].dx = dx;
		threads[0].dy = dy;
		threads[0].draw_y = dst->pDrawable->y;
		threads[0].unbounded = !was_clear && maskFormat && !operator_is_bounded(op);
		threads[0].span = thread_choose_span(&tmp, dst, maskFormat, &clip);

		y = clip.extents.y1;
		h = clip.extents.y2 - clip.extents.y1;
		h = (h + num_threads - 1) / num_threads;
		num_threads -= (num_threads-1) * h >= clip.extents.y2 - clip.extents.y1;

		for (n = 1; n < num_threads; n++) {
			threads[n] = threads[0];
			threads[n].extents.y1 = y;
			threads[n].extents.y2 = y += h;

			sna_threads_run(n, span_thread, &threads[n]);
		}

		assert(y < threads[0].extents.y2);
		threads[0].extents.y1 = y;
		span_thread(&threads[0]);

		sna_threads_wait();
	}
skip:
	tmp.done(sna, &tmp);

	REGION_UNINIT(NULL, &clip);
	return true;
}

static void
tor_blt_mask(struct sna *sna,
	     struct sna_composite_spans_op *op,
	     pixman_region16_t *clip,
	     const BoxRec *box,
	     int coverage)
{
	uint8_t *ptr = (uint8_t *)op;
	int stride = (intptr_t)clip;
	int h, w;

	coverage = 256 * coverage / FAST_SAMPLES_XY;
	coverage -= coverage >> 8;

	ptr += box->y1 * stride + box->x1;

	h = box->y2 - box->y1;
	w = box->x2 - box->x1;
	if ((w | h) == 1) {
		*ptr = coverage;
	} else if (w == 1) {
		do {
			*ptr = coverage;
			ptr += stride;
		} while (--h);
	} else do {
		memset(ptr, coverage, w);
		ptr += stride;
	} while (--h);
}

static void
tor_blt_mask_mono(struct sna *sna,
		  struct sna_composite_spans_op *op,
		  pixman_region16_t *clip,
		  const BoxRec *box,
		  int coverage)
{
	tor_blt_mask(sna, op, clip, box,
		     coverage < FAST_SAMPLES_XY/2 ? 0 : FAST_SAMPLES_XY);
}

bool
imprecise_trapezoid_mask_converter(CARD8 op, PicturePtr src, PicturePtr dst,
				   PictFormatPtr maskFormat, unsigned flags,
				   INT16 src_x, INT16 src_y,
				   int ntrap, xTrapezoid *traps)
{
	struct tor tor;
	ScreenPtr screen = dst->pDrawable->pScreen;
	PixmapPtr scratch;
	PicturePtr mask;
	BoxRec extents;
	int16_t dst_x, dst_y;
	int dx, dy;
	int error, n;

	if (NO_IMPRECISE)
		return false;

	if (maskFormat == NULL && ntrap > 1) {
		DBG(("%s: individual rasterisation requested\n",
		     __FUNCTION__));
		do {
			/* XXX unwind errors? */
			if (!imprecise_trapezoid_mask_converter(op, src, dst, NULL, flags,
								src_x, src_y, 1, traps++))
				return false;
		} while (--ntrap);
		return true;
	}

	if (!trapezoids_bounds(ntrap, traps, &extents))
		return true;

	DBG(("%s: ntraps=%d, extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__, ntrap, extents.x1, extents.y1, extents.x2, extents.y2));

	if (!sna_compute_composite_extents(&extents,
					   src, NULL, dst,
					   src_x, src_y,
					   0, 0,
					   extents.x1, extents.y1,
					   extents.x2 - extents.x1,
					   extents.y2 - extents.y1))
		return true;

	DBG(("%s: extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__, extents.x1, extents.y1, extents.x2, extents.y2));

	extents.y2 -= extents.y1;
	extents.x2 -= extents.x1;
	extents.x1 -= dst->pDrawable->x;
	extents.y1 -= dst->pDrawable->y;
	dst_x = extents.x1;
	dst_y = extents.y1;
	dx = -extents.x1 * FAST_SAMPLES_X;
	dy = -extents.y1 * FAST_SAMPLES_Y;
	extents.x1 = extents.y1 = 0;

	DBG(("%s: mask (%dx%d), dx=(%d, %d)\n",
	     __FUNCTION__, extents.x2, extents.y2, dx, dy));
	scratch = sna_pixmap_create_upload(screen,
					   extents.x2, extents.y2, 8,
					   KGEM_BUFFER_WRITE_INPLACE);
	if (!scratch)
		return true;

	DBG(("%s: created buffer %p, stride %d\n",
	     __FUNCTION__, scratch->devPrivate.ptr, scratch->devKind));

	if (!tor_init(&tor, &extents, 2*ntrap)) {
		sna_pixmap_destroy(scratch);
		return true;
	}

	for (n = 0; n < ntrap; n++) {
		xTrapezoid t;

		if (pixman_fixed_to_int(traps[n].top) - dst_y >= extents.y2 ||
		    pixman_fixed_to_int(traps[n].bottom) - dst_y < 0)
			continue;

		if (!project_trapezoid_onto_grid(&traps[n], dx, dy, &t))
			continue;

		tor_add_edge(&tor, &t, &t.left, 1);
		tor_add_edge(&tor, &t, &t.right, -1);
	}

	if (extents.x2 <= TOR_INPLACE_SIZE) {
		uint8_t buf[TOR_INPLACE_SIZE];
		tor_inplace(&tor, scratch, is_mono(dst, maskFormat),
			    scratch->usage_hint ? NULL : buf);
	} else {
		tor_render(NULL, &tor,
			   scratch->devPrivate.ptr,
			   (void *)(intptr_t)scratch->devKind,
			   is_mono(dst, maskFormat) ? tor_blt_mask_mono : tor_blt_mask,
			   true);
	}
	tor_fini(&tor);

	mask = CreatePicture(0, &scratch->drawable,
			     PictureMatchFormat(screen, 8, PICT_a8),
			     0, 0, serverClient, &error);
	if (mask) {
		int16_t x0, y0;

		trapezoid_origin(&traps[0].left, &x0, &y0);

		CompositePicture(op, src, mask, dst,
				 src_x + dst_x - x0,
				 src_y + dst_y - y0,
				 0, 0,
				 dst_x, dst_y,
				 extents.x2, extents.y2);
		FreePicture(mask, 0);
	}
	sna_pixmap_destroy(scratch);

	return true;
}

struct inplace {
	uint8_t *ptr;
	uint32_t stride;
	union {
		uint8_t opacity;
		uint32_t color;
	};
};

static force_inline uint8_t coverage_opacity(int coverage, uint8_t opacity)
{
	coverage = coverage * 256 / FAST_SAMPLES_XY;
	coverage -= coverage >> 8;
	return opacity == 255 ? coverage : mul_8_8(coverage, opacity);
}

static void _tor_blt_src(struct inplace *in, const BoxRec *box, uint8_t v)
{
	uint8_t *ptr = in->ptr;
	int h, w;

	ptr += box->y1 * in->stride + box->x1;

	h = box->y2 - box->y1;
	w = box->x2 - box->x1;
	if ((w | h) == 1) {
		*ptr = v;
	} else if (w == 1) {
		do {
			*ptr = v;
			ptr += in->stride;
		} while (--h);
	} else do {
		memset(ptr, v, w);
		ptr += in->stride;
	} while (--h);
}

static void
tor_blt_src(struct sna *sna,
	    struct sna_composite_spans_op *op,
	    pixman_region16_t *clip,
	    const BoxRec *box,
	    int coverage)
{
	struct inplace *in = (struct inplace *)op;

	_tor_blt_src(in, box, coverage_opacity(coverage, in->opacity));
}

static void
tor_blt_src_clipped(struct sna *sna,
		    struct sna_composite_spans_op *op,
		    pixman_region16_t *clip,
		    const BoxRec *box,
		    int coverage)
{
	pixman_region16_t region;
	int n;

	pixman_region_init_rects(&region, box, 1);
	RegionIntersect(&region, &region, clip);
	n = region_num_rects(&region);
	box = region_rects(&region);
	while (n--)
		tor_blt_src(sna, op, NULL, box++, coverage);
	pixman_region_fini(&region);
}

static void
tor_blt_in(struct sna *sna,
	   struct sna_composite_spans_op *op,
	   pixman_region16_t *clip,
	   const BoxRec *box,
	   int coverage)
{
	struct inplace *in = (struct inplace *)op;
	uint8_t *ptr = in->ptr;
	int h, w, i;

	if (coverage == 0) {
		_tor_blt_src(in, box, 0);
		return;
	}

	coverage = coverage_opacity(coverage, in->opacity);
	if (coverage == 0xff)
		return;

	ptr += box->y1 * in->stride + box->x1;

	h = box->y2 - box->y1;
	w = box->x2 - box->x1;
	do {
		for (i = 0; i < w; i++)
			ptr[i] = mul_8_8(ptr[i], coverage);
		ptr += in->stride;
	} while (--h);
}

static void
tor_blt_in_clipped(struct sna *sna,
		   struct sna_composite_spans_op *op,
		   pixman_region16_t *clip,
		   const BoxRec *box,
		   int coverage)
{
	pixman_region16_t region;
	int n;

	pixman_region_init_rects(&region, box, 1);
	RegionIntersect(&region, &region, clip);
	n = region_num_rects(&region);
	box = region_rects(&region);
	while (n--)
		tor_blt_in(sna, op, NULL, box++, coverage);
	pixman_region_fini(&region);
}

static void
tor_blt_add(struct sna *sna,
	    struct sna_composite_spans_op *op,
	    pixman_region16_t *clip,
	    const BoxRec *box,
	    int coverage)
{
	struct inplace *in = (struct inplace *)op;
	uint8_t *ptr = in->ptr;
	int h, w, v, i;

	if (coverage == 0)
		return;

	coverage = coverage_opacity(coverage, in->opacity);
	if (coverage == 0xff) {
		_tor_blt_src(in, box, 0xff);
		return;
	}

	ptr += box->y1 * in->stride + box->x1;

	h = box->y2 - box->y1;
	w = box->x2 - box->x1;
	if ((w | h) == 1) {
		v = coverage + *ptr;
		*ptr = v >= 255 ? 255 : v;
	} else {
		do {
			for (i = 0; i < w; i++) {
				v = coverage + ptr[i];
				ptr[i] = v >= 255 ? 255 : v;
			}
			ptr += in->stride;
		} while (--h);
	}
}

static void
tor_blt_add_clipped(struct sna *sna,
		    struct sna_composite_spans_op *op,
		    pixman_region16_t *clip,
		    const BoxRec *box,
		    int coverage)
{
	pixman_region16_t region;
	int n;

	pixman_region_init_rects(&region, box, 1);
	RegionIntersect(&region, &region, clip);
	n = region_num_rects(&region);
	box = region_rects(&region);
	while (n--)
		tor_blt_add(sna, op, NULL, box++, coverage);
	pixman_region_fini(&region);
}

static void
tor_blt_lerp32(struct sna *sna,
	       struct sna_composite_spans_op *op,
	       pixman_region16_t *clip,
	       const BoxRec *box,
	       int coverage)
{
	struct inplace *in = (struct inplace *)op;
	uint32_t *ptr = (uint32_t *)in->ptr;
	int stride = in->stride / sizeof(uint32_t);
	int h, w, i;

	if (coverage == 0)
		return;

	ptr += box->y1 * stride + box->x1;

	h = box->y2 - box->y1;
	w = box->x2 - box->x1;
	if (coverage == FAST_SAMPLES_XY) {
		if ((w | h) == 1) {
			*ptr = in->color;
		} else {
			if (w < 16) {
				do {
					for (i = 0; i < w; i++)
						ptr[i] = in->color;
					ptr += stride;
				} while (--h);
			} else {
				pixman_fill(ptr, stride, 32,
					    0, 0, w, h, in->color);
			}
		}
	} else {
		coverage = coverage * 256 / FAST_SAMPLES_XY;
		coverage -= coverage >> 8;

		if ((w | h) == 1) {
			*ptr = lerp8x4(in->color, coverage, *ptr);
		} else if (w == 1) {
			do {
				*ptr = lerp8x4(in->color, coverage, *ptr);
				ptr += stride;
			} while (--h);
		} else{
			do {
				for (i = 0; i < w; i++)
					ptr[i] = lerp8x4(in->color, coverage, ptr[i]);
				ptr += stride;
			} while (--h);
		}
	}
}

static void
tor_blt_lerp32_clipped(struct sna *sna,
		       struct sna_composite_spans_op *op,
		       pixman_region16_t *clip,
		       const BoxRec *box,
		       int coverage)
{
	pixman_region16_t region;
	int n;

	pixman_region_init_rects(&region, box, 1);
	RegionIntersect(&region, &region, clip);
	n = region_num_rects(&region);
	box = region_rects(&region);
	while (n--)
		tor_blt_lerp32(sna, op, NULL, box++, coverage);
	pixman_region_fini(&region);
}

struct pixman_inplace {
	pixman_image_t *image, *source, *mask;
	uint32_t color;
	uint32_t *bits;
	int dx, dy;
	int sx, sy;
	uint8_t op;
};

static void
pixmask_span_solid(struct sna *sna,
		   struct sna_composite_spans_op *op,
		   pixman_region16_t *clip,
		   const BoxRec *box,
		   int coverage)
{
	struct pixman_inplace *pi = (struct pixman_inplace *)op;
	if (coverage != FAST_SAMPLES_XY) {
		coverage = coverage * 256 / FAST_SAMPLES_XY;
		coverage -= coverage >> 8;
		*pi->bits = mul_4x8_8(pi->color, coverage);
	} else
		*pi->bits = pi->color;
	pixman_image_composite(pi->op, pi->source, NULL, pi->image,
			       box->x1, box->y1,
			       0, 0,
			       pi->dx + box->x1, pi->dy + box->y1,
			       box->x2 - box->x1, box->y2 - box->y1);
}
static void
pixmask_span_solid__clipped(struct sna *sna,
			    struct sna_composite_spans_op *op,
			    pixman_region16_t *clip,
			    const BoxRec *box,
			    int coverage)
{
	pixman_region16_t region;
	int n;

	pixman_region_init_rects(&region, box, 1);
	RegionIntersect(&region, &region, clip);
	n = region_num_rects(&region);
	box = region_rects(&region);
	while (n--)
		pixmask_span_solid(sna, op, NULL, box++, coverage);
	pixman_region_fini(&region);
}

static void
pixmask_span(struct sna *sna,
	     struct sna_composite_spans_op *op,
	     pixman_region16_t *clip,
	     const BoxRec *box,
	     int coverage)
{
	struct pixman_inplace *pi = (struct pixman_inplace *)op;
	pixman_image_t *mask = NULL;
	if (coverage != FAST_SAMPLES_XY) {
		coverage = coverage * 256 / FAST_SAMPLES_XY;
		coverage -= coverage >> 8;
		*pi->bits = coverage;
		mask = pi->mask;
	}
	pixman_image_composite(pi->op, pi->source, mask, pi->image,
			       pi->sx + box->x1, pi->sy + box->y1,
			       0, 0,
			       pi->dx + box->x1, pi->dy + box->y1,
			       box->x2 - box->x1, box->y2 - box->y1);
}
static void
pixmask_span__clipped(struct sna *sna,
		      struct sna_composite_spans_op *op,
		      pixman_region16_t *clip,
		      const BoxRec *box,
		      int coverage)
{
	pixman_region16_t region;
	int n;

	pixman_region_init_rects(&region, box, 1);
	RegionIntersect(&region, &region, clip);
	n = region_num_rects(&region);
	box = region_rects(&region);
	while (n--)
		pixmask_span(sna, op, NULL, box++, coverage);
	pixman_region_fini(&region);
}

struct inplace_x8r8g8b8_thread {
	xTrapezoid *traps;
	PicturePtr dst, src;
	BoxRec extents;
	int dx, dy;
	int ntrap;
	bool lerp, is_solid;
	uint32_t color;
	int16_t src_x, src_y;
	uint8_t op;
};

static void inplace_x8r8g8b8_thread(void *arg)
{
	struct inplace_x8r8g8b8_thread *thread = arg;
	struct tor tor;
	span_func_t span;
	RegionPtr clip;
	int y1, y2, n;

	if (!tor_init(&tor, &thread->extents, 2*thread->ntrap))
		return;

	y1 = thread->extents.y1 - thread->dst->pDrawable->y;
	y2 = thread->extents.y2 - thread->dst->pDrawable->y;
	for (n = 0; n < thread->ntrap; n++) {
		xTrapezoid t;

		if (pixman_fixed_to_int(thread->traps[n].top) >= y2 ||
		    pixman_fixed_to_int(thread->traps[n].bottom) < y1)
			continue;

		if (!project_trapezoid_onto_grid(&thread->traps[n], thread->dx, thread->dy, &t))
			continue;

		tor_add_edge(&tor, &t, &t.left, 1);
		tor_add_edge(&tor, &t, &t.right, -1);
	}

	clip = thread->dst->pCompositeClip;
	if (thread->lerp) {
		struct inplace inplace;
		int16_t dst_x, dst_y;
		PixmapPtr pixmap;

		pixmap = get_drawable_pixmap(thread->dst->pDrawable);

		inplace.ptr = pixmap->devPrivate.ptr;
		if (get_drawable_deltas(thread->dst->pDrawable, pixmap, &dst_x, &dst_y))
			inplace.ptr += dst_y * pixmap->devKind + dst_x * 4;
		inplace.stride = pixmap->devKind;
		inplace.color = thread->color;

		if (clip->data)
			span = tor_blt_lerp32_clipped;
		else
			span = tor_blt_lerp32;

		tor_render(NULL, &tor, (void*)&inplace, clip, span, false);
	} else if (thread->is_solid) {
		struct pixman_inplace pi;

		pi.image = image_from_pict(thread->dst, false, &pi.dx, &pi.dy);
		pi.op = thread->op;
		pi.color = thread->color;

		pi.bits = (uint32_t *)&pi.sx;
		pi.source = pixman_image_create_bits(PIXMAN_a8r8g8b8,
						     1, 1, pi.bits, 0);
		pixman_image_set_repeat(pi.source, PIXMAN_REPEAT_NORMAL);

		if (clip->data)
			span = pixmask_span_solid__clipped;
		else
			span = pixmask_span_solid;

		tor_render(NULL, &tor, (void*)&pi, clip, span, false);

		pixman_image_unref(pi.source);
		pixman_image_unref(pi.image);
	} else {
		struct pixman_inplace pi;
		int16_t x0, y0;

		trapezoid_origin(&thread->traps[0].left, &x0, &y0);

		pi.image = image_from_pict(thread->dst, false, &pi.dx, &pi.dy);
		pi.source = image_from_pict(thread->src, false, &pi.sx, &pi.sy);
		pi.sx += thread->src_x - x0;
		pi.sy += thread->src_y - y0;
		pi.mask = pixman_image_create_bits(PIXMAN_a8, 1, 1, NULL, 0);
		pixman_image_set_repeat(pi.mask, PIXMAN_REPEAT_NORMAL);
		pi.bits = pixman_image_get_data(pi.mask);
		pi.op = thread->op;

		if (clip->data)
			span = pixmask_span__clipped;
		else
			span = pixmask_span;

		tor_render(NULL, &tor, (void*)&pi, clip, span, false);

		pixman_image_unref(pi.mask);
		pixman_image_unref(pi.source);
		pixman_image_unref(pi.image);
	}

	tor_fini(&tor);
}

static bool
trapezoid_span_inplace__x8r8g8b8(CARD8 op,
				 PicturePtr dst,
				 PicturePtr src, int16_t src_x, int16_t src_y,
				 PictFormatPtr maskFormat,
				 int ntrap, xTrapezoid *traps)
{
	uint32_t color;
	bool lerp, is_solid;
	RegionRec region;
	int dx, dy;
	int num_threads, n;

	lerp = false;
	is_solid = sna_picture_is_solid(src, &color);
	if (is_solid) {
		if (op == PictOpOver && (color >> 24) == 0xff)
			op = PictOpSrc;
		if (op == PictOpOver && sna_drawable_is_clear(dst->pDrawable))
			op = PictOpSrc;
		lerp = op == PictOpSrc;
	}
	if (!lerp) {
		switch (op) {
		case PictOpOver:
		case PictOpAdd:
		case PictOpOutReverse:
			break;
		case PictOpSrc:
			if (!sna_drawable_is_clear(dst->pDrawable))
				return false;
			break;
		default:
			return false;
		}
	}

	if (maskFormat == NULL && ntrap > 1) {
		DBG(("%s: individual rasterisation requested\n",
		     __FUNCTION__));
		do {
			/* XXX unwind errors? */
			if (!trapezoid_span_inplace__x8r8g8b8(op, dst,
							      src, src_x, src_y,
							      NULL, 1, traps++))
				return false;
		} while (--ntrap);
		return true;
	}

	if (!trapezoids_bounds(ntrap, traps, &region.extents))
		return true;

	DBG(("%s: extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	if (!sna_compute_composite_extents(&region.extents,
					   src, NULL, dst,
					   src_x, src_y,
					   0, 0,
					   region.extents.x1, region.extents.y1,
					   region.extents.x2 - region.extents.x1,
					   region.extents.y2 - region.extents.y1))
		return true;

	DBG(("%s: clipped extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	region.data = NULL;
	if (!sna_drawable_move_region_to_cpu(dst->pDrawable, &region,
					    MOVE_WRITE | MOVE_READ))
		return true;

	if (!is_solid && src->pDrawable) {
		if (!sna_drawable_move_to_cpu(src->pDrawable,
					      MOVE_READ))
			return true;

		if (src->alphaMap &&
		    !sna_drawable_move_to_cpu(src->alphaMap->pDrawable,
					      MOVE_READ))
			return true;
	}

	dx = dst->pDrawable->x * FAST_SAMPLES_X;
	dy = dst->pDrawable->y * FAST_SAMPLES_Y;

	num_threads = sna_use_threads(4*(region.extents.x2 - region.extents.x1),
				      region.extents.y2 - region.extents.y1,
				      16);

	DBG(("%s: %dx%d, format=%x, op=%d, lerp?=%d, num_threads=%d\n",
	     __FUNCTION__,
	     region.extents.x2 - region.extents.x1,
	     region.extents.y2 - region.extents.y1,
	     dst->format, op, lerp, num_threads));

	if (num_threads == 1) {
		struct tor tor;
		span_func_t span;

		if (!tor_init(&tor, &region.extents, 2*ntrap))
			return true;

		for (n = 0; n < ntrap; n++) {
			xTrapezoid t;

			if (pixman_fixed_to_int(traps[n].top) >= region.extents.y2 - dst->pDrawable->y ||
			    pixman_fixed_to_int(traps[n].bottom) < region.extents.y1 - dst->pDrawable->y)
				continue;

			if (!project_trapezoid_onto_grid(&traps[n], dx, dy, &t))
				continue;

			tor_add_edge(&tor, &t, &t.left, 1);
			tor_add_edge(&tor, &t, &t.right, -1);
		}

		if (lerp) {
			struct inplace inplace;
			PixmapPtr pixmap;
			int16_t dst_x, dst_y;

			pixmap = get_drawable_pixmap(dst->pDrawable);

			inplace.ptr = pixmap->devPrivate.ptr;
			if (get_drawable_deltas(dst->pDrawable, pixmap, &dst_x, &dst_y))
				inplace.ptr += dst_y * pixmap->devKind + dst_x * 4;
			inplace.stride = pixmap->devKind;
			inplace.color = color;

			if (dst->pCompositeClip->data)
				span = tor_blt_lerp32_clipped;
			else
				span = tor_blt_lerp32;

			DBG(("%s: render inplace op=%d, color=%08x\n",
			     __FUNCTION__, op, color));

			if (sigtrap_get() == 0) {
				tor_render(NULL, &tor, (void*)&inplace,
					   dst->pCompositeClip, span, false);
				sigtrap_put();
			}
		} else if (is_solid) {
			struct pixman_inplace pi;

			pi.image = image_from_pict(dst, false, &pi.dx, &pi.dy);
			pi.op = op;
			pi.color = color;

			pi.bits = (uint32_t *)&pi.sx;
			pi.source = pixman_image_create_bits(PIXMAN_a8r8g8b8,
							     1, 1, pi.bits, 0);
			pixman_image_set_repeat(pi.source, PIXMAN_REPEAT_NORMAL);

			if (dst->pCompositeClip->data)
				span = pixmask_span_solid__clipped;
			else
				span = pixmask_span_solid;

			if (sigtrap_get() == 0) {
				tor_render(NULL, &tor, (void*)&pi,
					   dst->pCompositeClip, span,
					   false);
				sigtrap_put();
			}

			pixman_image_unref(pi.source);
			pixman_image_unref(pi.image);
		} else {
			struct pixman_inplace pi;
			int16_t x0, y0;

			trapezoid_origin(&traps[0].left, &x0, &y0);

			pi.image = image_from_pict(dst, false, &pi.dx, &pi.dy);
			pi.source = image_from_pict(src, false, &pi.sx, &pi.sy);
			pi.sx += src_x - x0;
			pi.sy += src_y - y0;
			pi.mask = pixman_image_create_bits(PIXMAN_a8, 1, 1, NULL, 0);
			pixman_image_set_repeat(pi.mask, PIXMAN_REPEAT_NORMAL);
			pi.bits = pixman_image_get_data(pi.mask);
			pi.op = op;

			if (dst->pCompositeClip->data)
				span = pixmask_span__clipped;
			else
				span = pixmask_span;

			if (sigtrap_get() == 0) {
				tor_render(NULL, &tor, (void*)&pi,
					   dst->pCompositeClip, span,
					   false);
				sigtrap_put();
			}

			pixman_image_unref(pi.mask);
			pixman_image_unref(pi.source);
			pixman_image_unref(pi.image);
		}

		tor_fini(&tor);
	} else {
		struct inplace_x8r8g8b8_thread threads[num_threads];
		int y, h;

		DBG(("%s: using %d threads for inplace compositing %dx%d\n",
		     __FUNCTION__, num_threads,
		     region.extents.x2 - region.extents.x1,
		     region.extents.y2 - region.extents.y1));

		threads[0].traps = traps;
		threads[0].ntrap = ntrap;
		threads[0].extents = region.extents;
		threads[0].lerp = lerp;
		threads[0].is_solid = is_solid;
		threads[0].color = color;
		threads[0].dx = dx;
		threads[0].dy = dy;
		threads[0].dst = dst;
		threads[0].src = src;
		threads[0].op = op;
		threads[0].src_x = src_x;
		threads[0].src_y = src_y;

		y = region.extents.y1;
		h = region.extents.y2 - region.extents.y1;
		h = (h + num_threads - 1) / num_threads;
		num_threads -= (num_threads-1) * h >= region.extents.y2 - region.extents.y1;

		if (sigtrap_get() == 0) {
			for (n = 1; n < num_threads; n++) {
				threads[n] = threads[0];
				threads[n].extents.y1 = y;
				threads[n].extents.y2 = y += h;

				sna_threads_run(n, inplace_x8r8g8b8_thread, &threads[n]);
			}

			assert(y < threads[0].extents.y2);
			threads[0].extents.y1 = y;
			inplace_x8r8g8b8_thread(&threads[0]);

			sna_threads_wait();
			sigtrap_put();
		} else
			sna_threads_kill(); /* leaks thread allocations */
	}

	return true;
}

struct inplace_thread {
	xTrapezoid *traps;
	RegionPtr clip;
	span_func_t span;
	struct inplace inplace;
	BoxRec extents;
	int dx, dy;
	int draw_x, draw_y;
	bool unbounded;
	int ntrap;
};

static void inplace_thread(void *arg)
{
	struct inplace_thread *thread = arg;
	struct tor tor;
	int n;

	if (!tor_init(&tor, &thread->extents, 2*thread->ntrap))
		return;

	for (n = 0; n < thread->ntrap; n++) {
		xTrapezoid t;

		if (pixman_fixed_to_int(thread->traps[n].top) >= thread->extents.y2 - thread->draw_y ||
		    pixman_fixed_to_int(thread->traps[n].bottom) < thread->extents.y1 - thread->draw_y)
			continue;

		if (!project_trapezoid_onto_grid(&thread->traps[n], thread->dx, thread->dy, &t))
			continue;

		tor_add_edge(&tor, &t, &t.left, 1);
		tor_add_edge(&tor, &t, &t.right, -1);
	}

	tor_render(NULL, &tor, (void*)&thread->inplace,
		   thread->clip, thread->span, thread->unbounded);

	tor_fini(&tor);
}

bool
imprecise_trapezoid_span_inplace(struct sna *sna,
				 CARD8 op, PicturePtr src, PicturePtr dst,
				 PictFormatPtr maskFormat, unsigned flags,
				 INT16 src_x, INT16 src_y,
				 int ntrap, xTrapezoid *traps,
				 bool fallback)
{
	struct inplace inplace;
	span_func_t span;
	PixmapPtr pixmap;
	struct sna_pixmap *priv;
	RegionRec region;
	uint32_t color;
	bool unbounded;
	int16_t dst_x, dst_y;
	int dx, dy;
	int num_threads, n;

	if (NO_IMPRECISE)
		return false;

	if (dst->format == PICT_a8r8g8b8 || dst->format == PICT_x8r8g8b8)
		return trapezoid_span_inplace__x8r8g8b8(op, dst,
							src, src_x, src_y,
							maskFormat,
							ntrap, traps);

	if (!sna_picture_is_solid(src, &color)) {
		DBG(("%s: fallback -- can not perform operation in place, requires solid source\n",
		     __FUNCTION__));
		return false;
	}

	if (dst->format != PICT_a8) {
		DBG(("%s: fallback -- can not perform operation in place, format=%x\n",
		     __FUNCTION__, dst->format));
		return false;
	}

	pixmap = get_drawable_pixmap(dst->pDrawable);

	unbounded = false;
	priv = sna_pixmap(pixmap);
	if (priv) {
		switch (op) {
		case PictOpAdd:
			if (priv->clear && priv->clear_color == 0) {
				unbounded = true;
				op = PictOpSrc;
			}
			if ((color >> 24) == 0)
				return true;
			break;
		case PictOpIn:
			if (priv->clear && priv->clear_color == 0)
				return true;
			if (priv->clear && priv->clear_color == 0xff)
				op = PictOpSrc;
			unbounded = true;
			break;
		case PictOpSrc:
			unbounded = true;
			break;
		default:
			DBG(("%s: fallback -- can not perform op [%d] in place\n",
			     __FUNCTION__, op));
			return false;
		}
	} else {
		switch (op) {
		case PictOpAdd:
			if ((color >> 24) == 0)
				return true;
			break;
		case PictOpIn:
		case PictOpSrc:
			unbounded = true;
			break;
		default:
			DBG(("%s: fallback -- can not perform op [%d] in place\n",
			     __FUNCTION__, op));
			return false;
		}
	}

	DBG(("%s: format=%x, op=%d, color=%x\n",
	     __FUNCTION__, dst->format, op, color));

	if (maskFormat == NULL && ntrap > 1) {
		DBG(("%s: individual rasterisation requested\n",
		     __FUNCTION__));
		do {
			/* XXX unwind errors? */
			if (!imprecise_trapezoid_span_inplace(sna, op, src, dst, NULL, flags,
							      src_x, src_y, 1, traps++,
							      fallback))
				return false;
		} while (--ntrap);
		return true;
	}

	if (!trapezoids_bounds(ntrap, traps, &region.extents))
		return true;

	DBG(("%s: extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	if (!sna_compute_composite_extents(&region.extents,
					   NULL, NULL, dst,
					   0, 0,
					   0, 0,
					   region.extents.x1, region.extents.y1,
					   region.extents.x2 - region.extents.x1,
					   region.extents.y2 - region.extents.y1))
		return true;

	DBG(("%s: clipped extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     region.extents.x1, region.extents.y1,
	     region.extents.x2, region.extents.y2));

	if (op == PictOpSrc) {
		if (dst->pCompositeClip->data)
			span = tor_blt_src_clipped;
		else
			span = tor_blt_src;
	} else if (op == PictOpIn) {
		if (dst->pCompositeClip->data)
			span = tor_blt_in_clipped;
		else
			span = tor_blt_in;
	} else {
		assert(op == PictOpAdd);
		if (dst->pCompositeClip->data)
			span = tor_blt_add_clipped;
		else
			span = tor_blt_add;
	}

	DBG(("%s: move-to-cpu\n", __FUNCTION__));
	region.data = NULL;
	if (!sna_drawable_move_region_to_cpu(dst->pDrawable, &region,
					     op == PictOpSrc ? MOVE_WRITE | MOVE_INPLACE_HINT : MOVE_WRITE | MOVE_READ))
		return true;

	dx = dst->pDrawable->x * FAST_SAMPLES_X;
	dy = dst->pDrawable->y * FAST_SAMPLES_Y;

	inplace.ptr = pixmap->devPrivate.ptr;
	if (get_drawable_deltas(dst->pDrawable, pixmap, &dst_x, &dst_y))
		inplace.ptr += dst_y * pixmap->devKind + dst_x;
	inplace.stride = pixmap->devKind;
	inplace.opacity = color >> 24;

	num_threads = 1;
	if ((flags & COMPOSITE_SPANS_RECTILINEAR) == 0)
		num_threads = sna_use_threads(region.extents.x2 - region.extents.x1,
					      region.extents.y2 - region.extents.y1,
					      16);
	if (num_threads == 1) {
		struct tor tor;

		if (!tor_init(&tor, &region.extents, 2*ntrap))
			return true;

		for (n = 0; n < ntrap; n++) {
			xTrapezoid t;

			if (pixman_fixed_to_int(traps[n].top) >= region.extents.y2 - dst->pDrawable->y ||
			    pixman_fixed_to_int(traps[n].bottom) < region.extents.y1 - dst->pDrawable->y)
				continue;

			if (!project_trapezoid_onto_grid(&traps[n], dx, dy, &t))
				continue;

			tor_add_edge(&tor, &t, &t.left, 1);
			tor_add_edge(&tor, &t, &t.right, -1);
		}

		if (sigtrap_get() == 0) {
			tor_render(NULL, &tor, (void*)&inplace,
				   dst->pCompositeClip, span, unbounded);
			sigtrap_put();
		}

		tor_fini(&tor);
	} else {
		struct inplace_thread threads[num_threads];
		int y, h;

		DBG(("%s: using %d threads for inplace compositing %dx%d\n",
		     __FUNCTION__, num_threads,
		     region.extents.x2 - region.extents.x1,
		     region.extents.y2 - region.extents.y1));

		threads[0].traps = traps;
		threads[0].ntrap = ntrap;
		threads[0].inplace = inplace;
		threads[0].extents = region.extents;
		threads[0].clip = dst->pCompositeClip;
		threads[0].span = span;
		threads[0].unbounded = unbounded;
		threads[0].dx = dx;
		threads[0].dy = dy;
		threads[0].draw_x = dst->pDrawable->x;
		threads[0].draw_y = dst->pDrawable->y;

		y = region.extents.y1;
		h = region.extents.y2 - region.extents.y1;
		h = (h + num_threads - 1) / num_threads;
		num_threads -= (num_threads-1) * h >= region.extents.y2 - region.extents.y1;

		if (sigtrap_get() == 0) {
			for (n = 1; n < num_threads; n++) {
				threads[n] = threads[0];
				threads[n].extents.y1 = y;
				threads[n].extents.y2 = y += h;

				sna_threads_run(n, inplace_thread, &threads[n]);
			}

			assert(y < threads[0].extents.y2);
			threads[0].extents.y1 = y;
			inplace_thread(&threads[0]);

			sna_threads_wait();
			sigtrap_put();
		} else
			sna_threads_kill(); /* leaks thread allocations */
	}

	return true;
}

bool
imprecise_trapezoid_span_fallback(CARD8 op, PicturePtr src, PicturePtr dst,
				  PictFormatPtr maskFormat, unsigned flags,
				  INT16 src_x, INT16 src_y,
				  int ntrap, xTrapezoid *traps)
{
	struct tor tor;
	ScreenPtr screen = dst->pDrawable->pScreen;
	PixmapPtr scratch;
	PicturePtr mask;
	BoxRec extents;
	int16_t dst_x, dst_y;
	int dx, dy;
	int error, n;

	if (NO_IMPRECISE)
		return false;

	if (maskFormat == NULL && ntrap > 1) {
		DBG(("%s: individual rasterisation requested\n",
		     __FUNCTION__));
		do {
			/* XXX unwind errors? */
			if (!imprecise_trapezoid_span_fallback(op, src, dst, NULL, flags,
							       src_x, src_y, 1, traps++))
				return false;
		} while (--ntrap);
		return true;
	}

	if (!trapezoids_bounds(ntrap, traps, &extents))
		return true;

	DBG(("%s: ntraps=%d, extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__, ntrap, extents.x1, extents.y1, extents.x2, extents.y2));

	if (!sna_compute_composite_extents(&extents,
					   src, NULL, dst,
					   src_x, src_y,
					   0, 0,
					   extents.x1, extents.y1,
					   extents.x2 - extents.x1,
					   extents.y2 - extents.y1))
		return true;

	DBG(("%s: extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__, extents.x1, extents.y1, extents.x2, extents.y2));

	extents.y2 -= extents.y1;
	extents.x2 -= extents.x1;
	extents.x1 -= dst->pDrawable->x;
	extents.y1 -= dst->pDrawable->y;
	dst_x = extents.x1;
	dst_y = extents.y1;
	dx = -extents.x1 * FAST_SAMPLES_X;
	dy = -extents.y1 * FAST_SAMPLES_Y;
	extents.x1 = extents.y1 = 0;

	DBG(("%s: mask (%dx%d), dx=(%d, %d)\n",
	     __FUNCTION__, extents.x2, extents.y2, dx, dy));
	scratch = sna_pixmap_create_unattached(screen,
					       extents.x2, extents.y2, 8);
	if (!scratch)
		return true;

	DBG(("%s: created buffer %p, stride %d\n",
	     __FUNCTION__, scratch->devPrivate.ptr, scratch->devKind));

	if (!tor_init(&tor, &extents, 2*ntrap)) {
		sna_pixmap_destroy(scratch);
		return true;
	}

	for (n = 0; n < ntrap; n++) {
		xTrapezoid t;

		if (pixman_fixed_to_int(traps[n].top) - dst_y >= extents.y2 ||
		    pixman_fixed_to_int(traps[n].bottom) - dst_y < 0)
			continue;

		if (!project_trapezoid_onto_grid(&traps[n], dx, dy, &t))
			continue;

		tor_add_edge(&tor, &t, &t.left, 1);
		tor_add_edge(&tor, &t, &t.right, -1);
	}

	if (extents.x2 <= TOR_INPLACE_SIZE) {
		tor_inplace(&tor, scratch, is_mono(dst, maskFormat), NULL);
	} else {
		tor_render(NULL, &tor,
			   scratch->devPrivate.ptr,
			   (void *)(intptr_t)scratch->devKind,
			   is_mono(dst, maskFormat) ? tor_blt_mask_mono : tor_blt_mask,
			   true);
	}
	tor_fini(&tor);

	mask = CreatePicture(0, &scratch->drawable,
			     PictureMatchFormat(screen, 8, PICT_a8),
			     0, 0, serverClient, &error);
	if (mask) {
		RegionRec region;
		int16_t x0, y0;

		region.extents.x1 = dst_x + dst->pDrawable->x;
		region.extents.y1 = dst_y + dst->pDrawable->y;
		region.extents.x2 = region.extents.x1 + extents.x2;
		region.extents.y2 = region.extents.y1 + extents.y2;
		region.data = NULL;

		trapezoid_origin(&traps[0].left, &x0, &y0);

		DBG(("%s: fbComposite()\n", __FUNCTION__));
		sna_composite_fb(op, src, mask, dst, &region,
				 src_x + dst_x - x0, src_y + dst_y - y0,
				 0, 0,
				 dst_x, dst_y,
				 extents.x2, extents.y2);

		FreePicture(mask, 0);
	}
	sna_pixmap_destroy(scratch);

	return true;
}

static inline bool
project_trap_onto_grid(const xTrap *in,
		       int dx, int dy,
		       xTrap *out)
{
	out->top.l = dx + pixman_fixed_to_fast(in->top.l);
	out->top.r = dx + pixman_fixed_to_fast(in->top.r);
	out->top.y = dy + pixman_fixed_to_fast(in->top.y);

	out->bot.l = dx + pixman_fixed_to_fast(in->bot.l);
	out->bot.r = dx + pixman_fixed_to_fast(in->bot.r);
	out->bot.y = dy + pixman_fixed_to_fast(in->bot.y);

	return out->bot.y > out->top.y;
}

bool
trap_span_converter(struct sna *sna,
		    PicturePtr dst,
		    INT16 src_x, INT16 src_y,
		    int ntrap, xTrap *trap)
{
	struct sna_composite_spans_op tmp;
	struct tor tor;
	BoxRec extents;
	pixman_region16_t *clip;
	int dx, dy, n;

	if (NO_SCAN_CONVERTER)
		return false;

	if (dst->pDrawable->depth < 8)
		return false;

	if (dst->polyEdge == PolyEdgeSharp)
		return mono_trap_span_converter(sna, dst, src_x, src_y, ntrap, trap);

	if (!sna->render.check_composite_spans(sna, PictOpAdd, sna->render.white_picture, dst,
					       dst->pCompositeClip->extents.x2 - dst->pCompositeClip->extents.x1,
					       dst->pCompositeClip->extents.y2 - dst->pCompositeClip->extents.y1,
					       0)) {
		DBG(("%s: fallback -- composite spans not supported\n",
		     __FUNCTION__));
		return false;
	}

	clip = dst->pCompositeClip;
	extents = *RegionExtents(clip);
	dx = dst->pDrawable->x;
	dy = dst->pDrawable->y;

	DBG(("%s: after clip -- extents (%d, %d), (%d, %d), delta=(%d, %d)\n",
	     __FUNCTION__,
	     extents.x1, extents.y1,
	     extents.x2, extents.y2,
	     dx, dy));

	memset(&tmp, 0, sizeof(tmp));
	if (!sna->render.composite_spans(sna, PictOpAdd, sna->render.white_picture, dst,
					 0, 0,
					 extents.x1,  extents.y1,
					 extents.x2 - extents.x1,
					 extents.y2 - extents.y1,
					 0,
					 &tmp)) {
		DBG(("%s: fallback -- composite spans render op not supported\n",
		     __FUNCTION__));
		return false;
	}

	dx *= FAST_SAMPLES_X;
	dy *= FAST_SAMPLES_Y;
	if (!tor_init(&tor, &extents, 2*ntrap))
		goto skip;

	for (n = 0; n < ntrap; n++) {
		xTrap t;
		xPointFixed p1, p2;

		if (pixman_fixed_to_int(trap[n].top.y) + dst->pDrawable->y >= extents.y2 ||
		    pixman_fixed_to_int(trap[n].bot.y) + dst->pDrawable->y < extents.y1)
			continue;

		if (!project_trap_onto_grid(&trap[n], dx, dy, &t))
			continue;

		p1.y = t.top.y;
		p2.y = t.bot.y;
		p1.x = t.top.l;
		p2.x = t.bot.l;
		polygon_add_line(tor.polygon, &p1, &p2);

		p1.y = t.bot.y;
		p2.y = t.top.y;
		p1.x = t.top.r;
		p2.x = t.bot.r;
		polygon_add_line(tor.polygon, &p1, &p2);
	}

	tor_render(sna, &tor, &tmp, clip,
		   choose_span(&tmp, dst, NULL, clip), false);

	tor_fini(&tor);
skip:
	tmp.done(sna, &tmp);
	return true;
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

bool
trap_mask_converter(struct sna *sna,
		    PicturePtr picture,
		    INT16 x, INT16 y,
		    int ntrap, xTrap *trap)
{
	struct tor tor;
	ScreenPtr screen = picture->pDrawable->pScreen;
	PixmapPtr scratch, pixmap;
	struct sna_pixmap *priv;
	BoxRec extents;
	span_func_t span;
	int dx, dy, n;

	if (NO_SCAN_CONVERTER)
		return false;

	pixmap = get_drawable_pixmap(picture->pDrawable);
	priv = sna_pixmap_move_to_gpu(pixmap, MOVE_READ | MOVE_WRITE);
	if (priv == NULL)
		return false;

	/* XXX strict adherence to the Render specification */
	if (picture->polyMode == PolyModePrecise &&
	    picture->polyEdge != PolyEdgeSharp) {
		DBG(("%s: fallback -- precise rasterisation requested\n",
		     __FUNCTION__));
		return false;
	}

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

	scratch = sna_pixmap_create_upload(screen,
					   extents.x2-extents.x1,
					   extents.y2-extents.y1,
					   8, KGEM_BUFFER_WRITE_INPLACE);
	if (!scratch)
		return true;

	dx = picture->pDrawable->x;
	dy = picture->pDrawable->y;
	dx *= FAST_SAMPLES_X;
	dy *= FAST_SAMPLES_Y;
	if (!tor_init(&tor, &extents, 2*ntrap)) {
		sna_pixmap_destroy(scratch);
		return true;
	}

	for (n = 0; n < ntrap; n++) {
		xTrap t;
		xPointFixed p1, p2;

		if (pixman_fixed_to_int(trap[n].top.y) + picture->pDrawable->y >= extents.y2 ||
		    pixman_fixed_to_int(trap[n].bot.y) + picture->pDrawable->y < extents.y1)
			continue;

		if (!project_trap_onto_grid(&trap[n], dx, dy, &t))
			continue;

		p1.y = t.top.y;
		p2.y = t.bot.y;
		p1.x = t.top.l;
		p2.x = t.bot.l;
		polygon_add_line(tor.polygon, &p1, &p2);

		p1.y = t.bot.y;
		p2.y = t.top.y;
		p1.x = t.top.r;
		p2.x = t.bot.r;
		polygon_add_line(tor.polygon, &p1, &p2);
	}

	if (picture->polyEdge == PolyEdgeSharp)
		span = tor_blt_mask_mono;
	else
		span = tor_blt_mask;

	tor_render(NULL, &tor,
		   scratch->devPrivate.ptr,
		   (void *)(intptr_t)scratch->devKind,
		   span, true);

	tor_fini(&tor);

	/* XXX clip boxes */
	get_drawable_deltas(picture->pDrawable, pixmap, &x, &y);
	sna = to_sna_from_screen(screen);
	sna->render.copy_boxes(sna, GXcopy,
			       &scratch->drawable, __sna_pixmap_get_bo(scratch), -extents.x1, -extents.x1,
			       &pixmap->drawable, priv->gpu_bo, x, y,
			       &extents, 1, 0);
	mark_damaged(pixmap, priv, &extents ,x, y);
	sna_pixmap_destroy(scratch);
	return true;
}

static inline void
project_point_onto_grid(const xPointFixed *in,
			int dx, int dy,
			xPointFixed *out)
{
	out->x = dx + pixman_fixed_to_fast(in->x);
	out->y = dy + pixman_fixed_to_fast(in->y);
}

#if HAS_PIXMAN_TRIANGLES
static inline bool
project_triangle_onto_grid(const xTriangle *in,
			   int dx, int dy,
			   xTriangle *out)
{
	project_point_onto_grid(&in->p1, dx, dy, &out->p1);
	project_point_onto_grid(&in->p2, dx, dy, &out->p2);
	project_point_onto_grid(&in->p3, dx, dy, &out->p3);

	return xTriangleValid(out);
}

bool
triangles_span_converter(struct sna *sna,
			 CARD8 op, PicturePtr src, PicturePtr dst,
			 PictFormatPtr maskFormat, INT16 src_x, INT16 src_y,
			 int count, xTriangle *tri)
{
	struct sna_composite_spans_op tmp;
	struct tor tor;
	BoxRec extents;
	pixman_region16_t clip;
	int16_t dst_x, dst_y;
	int dx, dy, n;
	bool was_clear;

	if (NO_SCAN_CONVERTER)
		return false;

	if (is_mono(dst, maskFormat))
		return mono_triangles_span_converter(sna, op, src, dst,
						     src_x, src_y,
						     count, tri);

	/* XXX strict adherence to the Render specification */
	if (dst->polyMode == PolyModePrecise) {
		DBG(("%s: fallback -- precise rasterisation requested\n",
		     __FUNCTION__));
		return false;
	}

	if (!sna->render.check_composite_spans(sna, op, src, dst, 0, 0, 0)) {
		DBG(("%s: fallback -- composite spans not supported\n",
		     __FUNCTION__));
		return false;
	}

	dst_x = pixman_fixed_to_int(tri[0].p1.x);
	dst_y = pixman_fixed_to_int(tri[0].p1.y);

	miTriangleBounds(count, tri, &extents);
	DBG(("%s: extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__, extents.x1, extents.y1, extents.x2, extents.y2));

	if (extents.y1 >= extents.y2 || extents.x1 >= extents.x2)
		return true;

#if 0
	if (extents.y2 - extents.y1 < 64 && extents.x2 - extents.x1 < 64) {
		DBG(("%s: fallback -- traps extents too small %dx%d\n",
		     __FUNCTION__, extents.y2 - extents.y1, extents.x2 - extents.x1));
		return false;
	}
#endif

	if (!sna_compute_composite_region(&clip,
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

	if (!sna->render.check_composite_spans(sna, op, src, dst,
					       clip.extents.x2 - clip.extents.x1,
					       clip.extents.y2 - clip.extents.y1,
					       0)) {
		DBG(("%s: fallback -- composite spans not supported\n",
		     __FUNCTION__));
		return false;
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

	was_clear = sna_drawable_is_clear(dst->pDrawable);

	memset(&tmp, 0, sizeof(tmp));
	if (!sna->render.composite_spans(sna, op, src, dst,
					 src_x + extents.x1 - dst_x - dx,
					 src_y + extents.y1 - dst_y - dy,
					 extents.x1,  extents.y1,
					 extents.x2 - extents.x1,
					 extents.y2 - extents.y1,
					 0,
					 &tmp)) {
		DBG(("%s: fallback -- composite spans render op not supported\n",
		     __FUNCTION__));
		return false;
	}

	dx *= FAST_SAMPLES_X;
	dy *= FAST_SAMPLES_Y;
	if (!tor_init(&tor, &extents, 3*count))
		goto skip;

	for (n = 0; n < count; n++) {
		xTriangle t;

		if (!project_triangle_onto_grid(&tri[n], dx, dy, &t))
			continue;

		polygon_add_line(tor.polygon, &t.p1, &t.p2);
		polygon_add_line(tor.polygon, &t.p2, &t.p3);
		polygon_add_line(tor.polygon, &t.p3, &t.p1);
	}

	tor_render(sna, &tor, &tmp, &clip,
		   choose_span(&tmp, dst, maskFormat, &clip),
		   !was_clear && maskFormat && !operator_is_bounded(op));

	tor_fini(&tor);
skip:
	tmp.done(sna, &tmp);

	REGION_UNINIT(NULL, &clip);
	return true;
}

bool
triangles_mask_converter(CARD8 op, PicturePtr src, PicturePtr dst,
			 PictFormatPtr maskFormat, INT16 src_x, INT16 src_y,
			 int count, xTriangle *tri)
{
	struct tor tor;
	void (*span)(struct sna *sna,
		     struct sna_composite_spans_op *op,
		     pixman_region16_t *clip,
		     const BoxRec *box,
		     int coverage);
	ScreenPtr screen = dst->pDrawable->pScreen;
	PixmapPtr scratch;
	PicturePtr mask;
	BoxRec extents;
	int16_t dst_x, dst_y;
	int dx, dy;
	int error, n;

	if (NO_SCAN_CONVERTER)
		return false;

	if (is_precise(dst, maskFormat)) {
		DBG(("%s: fallback -- precise rasterisation requested\n",
		     __FUNCTION__));
		return false;
	}

	if (maskFormat == NULL && count > 1) {
		DBG(("%s: fallback -- individual rasterisation requested\n",
		     __FUNCTION__));
		return false;
	}

	miTriangleBounds(count, tri, &extents);
	DBG(("%s: extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__, extents.x1, extents.y1, extents.x2, extents.y2));

	if (extents.y1 >= extents.y2 || extents.x1 >= extents.x2)
		return true;

	if (!sna_compute_composite_extents(&extents,
					   src, NULL, dst,
					   src_x, src_y,
					   0, 0,
					   extents.x1, extents.y1,
					   extents.x2 - extents.x1,
					   extents.y2 - extents.y1))
		return true;

	DBG(("%s: extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__, extents.x1, extents.y1, extents.x2, extents.y2));

	extents.y2 -= extents.y1;
	extents.x2 -= extents.x1;
	extents.x1 -= dst->pDrawable->x;
	extents.y1 -= dst->pDrawable->y;
	dst_x = extents.x1;
	dst_y = extents.y1;
	dx = -extents.x1 * FAST_SAMPLES_X;
	dy = -extents.y1 * FAST_SAMPLES_Y;
	extents.x1 = extents.y1 = 0;

	DBG(("%s: mask (%dx%d)\n",
	     __FUNCTION__, extents.x2, extents.y2));
	scratch = sna_pixmap_create_upload(screen,
					   extents.x2, extents.y2, 8,
					   KGEM_BUFFER_WRITE_INPLACE);
	if (!scratch)
		return true;

	DBG(("%s: created buffer %p, stride %d\n",
	     __FUNCTION__, scratch->devPrivate.ptr, scratch->devKind));

	if (!tor_init(&tor, &extents, 3*count)) {
		sna_pixmap_destroy(scratch);
		return true;
	}

	for (n = 0; n < count; n++) {
		xTriangle t;

		if (!project_triangle_onto_grid(&tri[n], dx, dy, &t))
			continue;

		polygon_add_line(tor.polygon, &t.p1, &t.p2);
		polygon_add_line(tor.polygon, &t.p2, &t.p3);
		polygon_add_line(tor.polygon, &t.p3, &t.p1);
	}

	if (maskFormat ? maskFormat->depth < 8 : dst->polyEdge == PolyEdgeSharp)
		span = tor_blt_mask_mono;
	else
		span = tor_blt_mask;

	tor_render(NULL, &tor,
		   scratch->devPrivate.ptr,
		   (void *)(intptr_t)scratch->devKind,
		   span, true);

	mask = CreatePicture(0, &scratch->drawable,
			     PictureMatchFormat(screen, 8, PICT_a8),
			     0, 0, serverClient, &error);
	if (mask) {
		CompositePicture(op, src, mask, dst,
				 src_x + dst_x - pixman_fixed_to_int(tri[0].p1.x),
				 src_y + dst_y - pixman_fixed_to_int(tri[0].p1.y),
				 0, 0,
				 dst_x, dst_y,
				 extents.x2, extents.y2);
		FreePicture(mask, 0);
	}
	tor_fini(&tor);
	sna_pixmap_destroy(scratch);

	return true;
}

bool
tristrip_span_converter(struct sna *sna,
			CARD8 op, PicturePtr src, PicturePtr dst,
			PictFormatPtr maskFormat, INT16 src_x, INT16 src_y,
			int count, xPointFixed *points)
{
	struct sna_composite_spans_op tmp;
	struct tor tor;
	BoxRec extents;
	pixman_region16_t clip;
	xPointFixed p[4];
	int16_t dst_x, dst_y;
	int dx, dy;
	int cw, ccw, n;
	bool was_clear;

	if (NO_SCAN_CONVERTER)
		return false;

	/* XXX strict adherence to the Render specification */
	if (is_precise(dst, maskFormat)) {
		DBG(("%s: fallback -- precise rasterisation requested\n",
		     __FUNCTION__));
		return false;
	}

	if (!sna->render.check_composite_spans(sna, op, src, dst, 0, 0, 0)) {
		DBG(("%s: fallback -- composite spans not supported\n",
		     __FUNCTION__));
		return false;
	}

	dst_x = pixman_fixed_to_int(points[0].x);
	dst_y = pixman_fixed_to_int(points[0].y);

	miPointFixedBounds(count, points, &extents);
	DBG(("%s: extents (%d, %d), (%d, %d)\n",
	     __FUNCTION__, extents.x1, extents.y1, extents.x2, extents.y2));

	if (extents.y1 >= extents.y2 || extents.x1 >= extents.x2)
		return true;

#if 0
	if (extents.y2 - extents.y1 < 64 && extents.x2 - extents.x1 < 64) {
		DBG(("%s: fallback -- traps extents too small %dx%d\n",
		     __FUNCTION__, extents.y2 - extents.y1, extents.x2 - extents.x1));
		return false;
	}
#endif

	if (!sna_compute_composite_region(&clip,
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

	if (!sna->render.check_composite_spans(sna, op, src, dst,
					       clip.extents.x2 - clip.extents.x1,
					       clip.extents.y2 - clip.extents.y1,
					       0)) {
		DBG(("%s: fallback -- composite spans not supported\n",
		     __FUNCTION__));
		return false;
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

	was_clear = sna_drawable_is_clear(dst->pDrawable);

	memset(&tmp, 0, sizeof(tmp));
	if (!sna->render.composite_spans(sna, op, src, dst,
					 src_x + extents.x1 - dst_x - dx,
					 src_y + extents.y1 - dst_y - dy,
					 extents.x1,  extents.y1,
					 extents.x2 - extents.x1,
					 extents.y2 - extents.y1,
					 0,
					 &tmp)) {
		DBG(("%s: fallback -- composite spans render op not supported\n",
		     __FUNCTION__));
		return false;
	}

	dx *= FAST_SAMPLES_X;
	dy *= FAST_SAMPLES_Y;
	if (!tor_init(&tor, &extents, 2*count))
		goto skip;

	cw = ccw = 0;
	project_point_onto_grid(&points[0], dx, dy, &p[cw]);
	project_point_onto_grid(&points[1], dx, dy, &p[2+ccw]);
	polygon_add_line(tor.polygon, &p[cw], &p[2+ccw]);
	n = 2;
	do {
		cw = !cw;
		project_point_onto_grid(&points[n], dx, dy, &p[cw]);
		polygon_add_line(tor.polygon, &p[!cw], &p[cw]);
		if (++n == count)
			break;

		ccw = !ccw;
		project_point_onto_grid(&points[n], dx, dy, &p[2+ccw]);
		polygon_add_line(tor.polygon, &p[2+ccw], &p[2+!ccw]);
		if (++n == count)
			break;
	} while (1);
	polygon_add_line(tor.polygon, &p[2+ccw], &p[cw]);
	assert(tor.polygon->num_edges <= 2*count);

	tor_render(sna, &tor, &tmp, &clip,
		   choose_span(&tmp, dst, maskFormat, &clip),
		   !was_clear && maskFormat && !operator_is_bounded(op));

	tor_fini(&tor);
skip:
	tmp.done(sna, &tmp);

	REGION_UNINIT(NULL, &clip);
	return true;
}

#endif
