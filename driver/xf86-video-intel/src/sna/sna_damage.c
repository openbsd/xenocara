/**************************************************************************

Copyright (c) 2011 Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 **************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sna.h"
#include "sna_damage.h"

/*
 * sna_damage is a batching layer on top of the regular pixman_region_t.
 * It is required as the ever-growing accumulation of invidual small
 * damage regions is an O(n^2) operation. Instead the accumulation of a
 * batch can be done in closer to O(n.lgn), and so prevents abysmal
 * performance in x11perf -copywinwin10.
 *
 * As with the core of SNA, damage is handled modally. That is, it
 * accumulates whilst rendering and then subtracts during migration of the
 * pixmap from GPU to CPU or vice versa. As such we can track the current
 * mode globally and when that mode switches perform the update of the region
 * in a single operation.
 *
 * Furthermore, we can track whether the whole pixmap is damaged and so
 * cheapy discard no-ops.
 */

struct sna_damage_box {
	struct list list;
	int size;
} __attribute__((packed));

static struct sna_damage *__freed_damage;

static inline bool region_is_singular(const RegionRec *r)
{
	return r->data == NULL;
}

static inline bool region_is_singular_or_empty(const RegionRec *r)
{
	return r->data == NULL || r->data->numRects == 0;
}

#if HAS_DEBUG_FULL
static const char *_debug_describe_region(char *buf, int max,
					  const RegionRec *region)
{
	const BoxRec *box;
	int n, len;

	if (region == NULL)
		return "nil";

	n = region_num_rects(region);
	if (n == 0)
		return "[0]";

	if (n == 1) {
		sprintf(buf,
			"[(%d, %d), (%d, %d)]",
			region->extents.x1, region->extents.y1,
			region->extents.x2, region->extents.y2);
		return buf;
	}

	len = sprintf(buf,
		      "[(%d, %d), (%d, %d) x %d: ",
		      region->extents.x1, region->extents.y1,
		      region->extents.x2, region->extents.y2,
		      n) + 3;
	max -= 2;
	box = region_rects(region);
	while (n--) {
		char tmp[80];
		int this;

		this = snprintf(tmp, sizeof(tmp),
				"((%d, %d), (%d, %d))%s",
				box->x1, box->y1,
				box->x2, box->y2,
				n ? ", ..." : "");
		box++;

		if (this > max - len)
			break;

		len -= 3;
		memcpy(buf + len, tmp, this);
		len += this;
	}
	buf[len++] = ']';
	buf[len] = '\0';
	return buf;
}

static const char *_debug_describe_damage(char *buf, int max,
					  const struct sna_damage *damage)
{
	char damage_str[500], region_str[500];
	int str_max;

	if (damage == NULL)
		return "None";

	str_max = max/2 - 6;
	if (str_max > sizeof(damage_str))
		str_max = sizeof(damage_str);

	if (damage->mode == DAMAGE_ALL) {
		snprintf(buf, max, "[[(%d, %d), (%d, %d)]: all]",
			 damage->extents.x1, damage->extents.y1,
			 damage->extents.x2, damage->extents.y2);
	} else {
		if (damage->dirty) {
			sprintf(damage_str, "%c[ ...]",
				damage->mode == DAMAGE_SUBTRACT ? '-' : '+');
		} else
			damage_str[0] = '\0';
		snprintf(buf, max, "[[(%d, %d), (%d, %d)]: %s %s]%c",
			 damage->extents.x1, damage->extents.y1,
			 damage->extents.x2, damage->extents.y2,
			 _debug_describe_region(region_str, str_max,
						&damage->region),
			 damage_str, damage->dirty ? '*' : ' ');
	}

	return buf;
}
#endif

static struct sna_damage_box *
last_box(struct sna_damage *damage)
{
	return list_entry(damage->embedded_box.list.prev,
			  struct sna_damage_box,
			  list);
}

static void
reset_embedded_box(struct sna_damage *damage)
{
	damage->dirty = false;
	damage->box = damage->embedded_box.box;
	damage->embedded_box.size =
		damage->remain = ARRAY_SIZE(damage->embedded_box.box);
	list_init(&damage->embedded_box.list);
}

static void reset_extents(struct sna_damage *damage)
{
	damage->extents.x1 = damage->extents.y1 = MAXSHORT;
	damage->extents.x2 = damage->extents.y2 = MINSHORT;
}

static struct sna_damage *_sna_damage_create(void)
{
	struct sna_damage *damage;

	if (__freed_damage) {
		damage = __freed_damage;
		__freed_damage = *(void **)__freed_damage;
	} else {
		damage = malloc(sizeof(*damage));
		if (damage == NULL)
			return NULL;
	}
	reset_embedded_box(damage);
	damage->mode = DAMAGE_ADD;
	pixman_region_init(&damage->region);
	reset_extents(damage);

	return damage;
}

struct sna_damage *sna_damage_create(void)
{
	return _sna_damage_create();
}

static void free_list(struct list *head)
{
	while (!list_is_empty(head)) {
		struct list *l = head->next;
		list_del(l);
		free(l);
	}
}

static void __sna_damage_reduce(struct sna_damage *damage)
{
	int n, nboxes;
	BoxPtr boxes, free_boxes = NULL;
	pixman_region16_t *region = &damage->region;
	struct sna_damage_box *iter;

	assert(damage->mode != DAMAGE_ALL);
	assert(damage->dirty);

	DBG(("    reduce: before region.n=%d\n", region_num_rects(region)));

	nboxes = damage->embedded_box.size;
	list_for_each_entry(iter, &damage->embedded_box.list, list)
		nboxes += iter->size;
	DBG(("   nboxes=%d, residual=%d\n", nboxes, damage->remain));
	nboxes -= damage->remain;
	if (nboxes == 0)
		goto done;
	if (nboxes == 1) {
		pixman_region16_t tmp;

		tmp.extents = damage->embedded_box.box[0];
		tmp.data = NULL;

		if (damage->mode == DAMAGE_ADD)
			pixman_region_union(region, region, &tmp);
		else
			pixman_region_subtract(region, region, &tmp);
		damage->extents = region->extents;

		goto done;
	}

	if (damage->mode == DAMAGE_ADD)
		nboxes += region_num_rects(region);

	iter = last_box(damage);
	n = iter->size - damage->remain;
	boxes = (BoxRec *)(iter+1);
	DBG(("   last box count=%d/%d, need=%d\n", n, iter->size, nboxes));
	if (nboxes > iter->size) {
		boxes = malloc(sizeof(BoxRec)*nboxes);
		if (boxes == NULL)
			goto done;

		free_boxes = boxes;
	}

	if (boxes != damage->embedded_box.box) {
		if (list_is_empty(&damage->embedded_box.list)) {
			DBG(("   copying embedded boxes\n"));
			memcpy(boxes,
			       damage->embedded_box.box,
			       n*sizeof(BoxRec));
		} else {
			if (boxes != (BoxPtr)(iter+1)) {
				DBG(("   copying %d boxes from last\n", n));
				memcpy(boxes, iter+1, n*sizeof(BoxRec));
			}

			iter = list_entry(iter->list.prev,
					  struct sna_damage_box,
					  list);
			while (&iter->list != &damage->embedded_box.list) {
				DBG(("   copy %d boxes from %d\n",
				     iter->size, n));
				memcpy(boxes + n, iter+1,
				       iter->size * sizeof(BoxRec));
				n += iter->size;

				iter = list_entry(iter->list.prev,
						  struct sna_damage_box,
						  list);
			}

			DBG(("   copying embedded boxes to %d\n", n));
			memcpy(boxes + n,
			       damage->embedded_box.box,
			       sizeof(damage->embedded_box.box));
			n += damage->embedded_box.size;
		}
	}

	if (damage->mode == DAMAGE_ADD) {
		memcpy(boxes + n,
		       region_rects(region),
		       region_num_rects(region)*sizeof(BoxRec));
		assert(n + region_num_rects(region) == nboxes);
		pixman_region_fini(region);
		pixman_region_init_rects(region, boxes, nboxes);

		assert(pixman_region_not_empty(region));
		assert(damage->extents.x1 == region->extents.x1 &&
		       damage->extents.y1 == region->extents.y1 &&
		       damage->extents.x2 == region->extents.x2 &&
		       damage->extents.y2 == region->extents.y2);
	} else {
		pixman_region16_t tmp;

		assert(n == nboxes);
		pixman_region_init_rects(&tmp, boxes, nboxes);
		pixman_region_subtract(region, region, &tmp);
		pixman_region_fini(&tmp);

		assert(damage->extents.x1 <= region->extents.x1 &&
		       damage->extents.y1 <= region->extents.y1 &&
		       damage->extents.x2 >= region->extents.x2 &&
		       damage->extents.y2 >= region->extents.y2);
		if (pixman_region_not_empty(region))
			damage->extents = region->extents;
		else
			reset_extents(damage);
	}

	free(free_boxes);

done:
	damage->mode = DAMAGE_ADD;
	free_list(&damage->embedded_box.list);
	reset_embedded_box(damage);

	DBG(("    reduce: after region.n=%d\n", region_num_rects(region)));
}

static bool _sna_damage_create_boxes(struct sna_damage *damage,
				     int count)
{
	struct sna_damage_box *box;
	int n;

	box = last_box(damage);
	n = 4*box->size;
	if (n < count)
		n = ALIGN(count, 64);

	DBG(("    %s(%d->%d): new\n", __FUNCTION__, count, n));

	if (n >= (INT_MAX - sizeof(*box)) / sizeof(BoxRec))
		return false;

	box = malloc(sizeof(*box) + sizeof(BoxRec)*n);
	if (box == NULL)
		return false;

	list_add_tail(&box->list, &damage->embedded_box.list);

	box->size = damage->remain = n;
	damage->box = (BoxRec *)(box + 1);
	return true;
}

static struct sna_damage *
_sna_damage_create_elt(struct sna_damage *damage,
		       const BoxRec *boxes, int count)
{
	int n;

	DBG(("    %s: prev=(remain %d), count=%d\n",
	     __FUNCTION__, damage->remain, count));
	assert(count);

restart:
	n = count;
	if (n > damage->remain)
		n = damage->remain;
	if (n) {
		memcpy(damage->box, boxes, n * sizeof(BoxRec));
		damage->box += n;
		damage->remain -= n;
		damage->dirty = true;

		count -= n;
		boxes += n;
		if (count == 0)
			return damage;
	}

	DBG(("    %s(): new elt\n", __FUNCTION__));
	assert(damage->remain == 0);
	assert(damage->box - (BoxRec *)(last_box(damage)+1) == last_box(damage)->size);

	if (!_sna_damage_create_boxes(damage, count)) {
		unsigned mode;

		if (!damage->dirty)
			return damage;

		mode = damage->mode;
		__sna_damage_reduce(damage);
		damage->mode = mode;

		goto restart;
	}

	memcpy(damage->box, boxes, count * sizeof(BoxRec));
	damage->box += count;
	damage->remain -= count;
	damage->dirty = true;
	assert(damage->remain >= 0);

	return damage;
}

static struct sna_damage *
_sna_damage_create_elt_from_boxes(struct sna_damage *damage,
				  const BoxRec *boxes, int count,
				  int16_t dx, int16_t dy)
{
	int i, n;

	DBG(("    %s: prev=(remain %d)\n", __FUNCTION__, damage->remain));
	assert(count);

restart:
	n = count;
	if (n > damage->remain)
		n = damage->remain;
	if (n) {
		for (i = 0; i < n; i++) {
			damage->box[i].x1 = boxes[i].x1 + dx;
			damage->box[i].x2 = boxes[i].x2 + dx;
			damage->box[i].y1 = boxes[i].y1 + dy;
			damage->box[i].y2 = boxes[i].y2 + dy;
		}
		damage->box += n;
		damage->remain -= n;
		damage->dirty = true;

		count -= n;
		boxes += n;
		if (count == 0)
			return damage;
	}

	DBG(("    %s(): new elt\n", __FUNCTION__));
	assert(damage->remain == 0);
	assert(damage->box - (BoxRec *)(last_box(damage)+1) == last_box(damage)->size);

	if (!_sna_damage_create_boxes(damage, count)) {
		unsigned mode;

		if (!damage->dirty)
			return damage;

		mode = damage->mode;
		__sna_damage_reduce(damage);
		damage->mode = mode;

		goto restart;
	}

	for (i = 0; i < count; i++) {
		damage->box[i].x1 = boxes[i].x1 + dx;
		damage->box[i].x2 = boxes[i].x2 + dx;
		damage->box[i].y1 = boxes[i].y1 + dy;
		damage->box[i].y2 = boxes[i].y2 + dy;
	}
	damage->box += count;
	damage->remain -= count;
	damage->dirty = true;
	assert(damage->remain >= 0);

	return damage;
}

static struct sna_damage *
_sna_damage_create_elt_from_rectangles(struct sna_damage *damage,
				       const xRectangle *r, int count,
				       int16_t dx, int16_t dy)
{
	int i, n;

	DBG(("    %s: prev=(remain %d), count=%d\n",
	     __FUNCTION__, damage->remain, count));
	assert(count);

restart:
	n = count;
	if (n > damage->remain)
		n = damage->remain;
	if (n) {
		for (i = 0; i < n; i++) {
			damage->box[i].x1 = r[i].x + dx;
			damage->box[i].x2 = damage->box[i].x1 + r[i].width;
			damage->box[i].y1 = r[i].y + dy;
			damage->box[i].y2 = damage->box[i].y1 + r[i].height;
		}
		damage->box += n;
		damage->remain -= n;
		damage->dirty = true;

		count -= n;
		r += n;
		if (count == 0)
			return damage;
	}

	DBG(("    %s(): new elt\n", __FUNCTION__));
	assert(damage->remain == 0);
	assert(damage->box - (BoxRec *)(last_box(damage)+1) == last_box(damage)->size);

	if (!_sna_damage_create_boxes(damage, count)) {
		unsigned mode;

		if (!damage->dirty)
			return damage;

		mode = damage->mode;
		__sna_damage_reduce(damage);
		damage->mode = mode;

		goto restart;
	}

	for (i = 0; i < count; i++) {
		damage->box[i].x1 = r[i].x + dx;
		damage->box[i].x2 = damage->box[i].x1 + r[i].width;
		damage->box[i].y1 = r[i].y + dy;
		damage->box[i].y2 = damage->box[i].y1 + r[i].height;
	}
	damage->box += count;
	damage->remain -= count;
	damage->dirty = true;
	assert(damage->remain >= 0);

	return damage;
}

static struct sna_damage *
_sna_damage_create_elt_from_points(struct sna_damage *damage,
				   const DDXPointRec *p, int count,
				   int16_t dx, int16_t dy)
{
	int i, n;

	DBG(("    %s: prev=(remain %d), count=%d\n",
	     __FUNCTION__, damage->remain, count));
	assert(count);

restart:
	n = count;
	if (n > damage->remain)
		n = damage->remain;
	if (n) {
		for (i = 0; i < n; i++) {
			damage->box[i].x1 = p[i].x + dx;
			damage->box[i].x2 = damage->box[i].x1 + 1;
			damage->box[i].y1 = p[i].y + dy;
			damage->box[i].y2 = damage->box[i].y1 + 1;
		}
		damage->box += n;
		damage->remain -= n;
		damage->dirty = true;

		count -= n;
		p += n;
		if (count == 0)
			return damage;
	}

	DBG(("    %s(): new elt\n", __FUNCTION__));
	assert(damage->remain == 0);
	assert(damage->box - (BoxRec *)(last_box(damage)+1) == last_box(damage)->size);

	if (!_sna_damage_create_boxes(damage, count)) {
		unsigned mode;

		if (!damage->dirty)
			return damage;

		mode = damage->mode;
		__sna_damage_reduce(damage);
		damage->mode = mode;

		goto restart;
	}

	for (i = 0; i < count; i++) {
		damage->box[i].x1 = p[i].x + dx;
		damage->box[i].x2 = damage->box[i].x1 + 1;
		damage->box[i].y1 = p[i].y + dy;
		damage->box[i].y2 = damage->box[i].y1 + 1;
	}
	damage->box += count;
	damage->remain -= count;
	damage->dirty = true;
	assert(damage->remain >= 0);

	return damage;
}

static void damage_union(struct sna_damage *damage, const BoxRec *box)
{
	DBG(("%s: extending damage (%d, %d), (%d, %d) by (%d, %d), (%d, %d)\n",
	     __FUNCTION__,
	     damage->extents.x1, damage->extents.y1,
	     damage->extents.x2, damage->extents.y2,
	     box->x1, box->y1, box->x2, box->y2));
	assert(box->x2 > box->x1 && box->y2 > box->y1);
	if (damage->extents.x2 < damage->extents.x1) {
		damage->extents = *box;
	} else {
		if (damage->extents.x1 > box->x1)
			damage->extents.x1 = box->x1;
		if (damage->extents.x2 < box->x2)
			damage->extents.x2 = box->x2;

		if (damage->extents.y1 > box->y1)
			damage->extents.y1 = box->y1;
		if (damage->extents.y2 < box->y2)
			damage->extents.y2 = box->y2;
	}
	assert(damage->extents.x2 > damage->extents.x1);
	assert(damage->extents.y2 > damage->extents.y1);
}

static void _pixman_region_union_box(RegionRec *region, const BoxRec *box)
{
	RegionRec u = { *box, NULL };
	pixman_region_union(region, region, &u);
}

static bool box_contains_region(const BoxRec *b, const RegionRec *r)
{
	return (b->x1 <= r->extents.x1 && b->x2 >= r->extents.x2 &&
		b->y1 <= r->extents.y1 && b->y2 >= r->extents.y2);
}

static struct sna_damage *__sna_damage_add_box(struct sna_damage *damage,
					       const BoxRec *box)
{
	if (box->y2 <= box->y1 || box->x2 <= box->x1)
		return damage;

	if (!damage) {
		damage = _sna_damage_create();
		if (damage == NULL)
			return NULL;
	} else switch (damage->mode) {
	case DAMAGE_ALL:
		return damage;
	case DAMAGE_SUBTRACT:
		__sna_damage_reduce(damage);
	case DAMAGE_ADD:
		break;
	}

	if (region_is_singular_or_empty(&damage->region) ||
	    box_contains_region(box, &damage->region)) {
		_pixman_region_union_box(&damage->region, box);
		assert(damage->region.extents.x2 > damage->region.extents.x1);
		assert(damage->region.extents.y2 > damage->region.extents.y1);
		damage_union(damage, box);
		return damage;
	}

	if (pixman_region_contains_rectangle(&damage->region,
					     (BoxPtr)box) == PIXMAN_REGION_IN)
		return damage;

	damage_union(damage, box);
	return _sna_damage_create_elt(damage, box, 1);
}

inline static struct sna_damage *__sna_damage_add(struct sna_damage *damage,
						  RegionPtr region)
{
	assert(RegionNotEmpty(region));

	if (!damage) {
		damage = _sna_damage_create();
		if (damage == NULL)
			return NULL;
	} else switch (damage->mode) {
	case DAMAGE_ALL:
		return damage;
	case DAMAGE_SUBTRACT:
		__sna_damage_reduce(damage);
	case DAMAGE_ADD:
		break;
	}

	if (region_is_singular(region))
		return __sna_damage_add_box(damage, &region->extents);

	if (region_is_singular_or_empty(&damage->region)) {
		pixman_region_union(&damage->region, &damage->region, region);
		assert(damage->region.extents.x2 > damage->region.extents.x1);
		assert(damage->region.extents.y2 > damage->region.extents.y1);
		damage_union(damage, &region->extents);
		return damage;
	}

	if (pixman_region_contains_rectangle(&damage->region,
					     &region->extents) == PIXMAN_REGION_IN)
		return damage;

	damage_union(damage, &region->extents);
	return _sna_damage_create_elt(damage,
				      region_rects(region),
				      region_num_rects(region));
}

#if HAS_DEBUG_FULL
fastcall struct sna_damage *_sna_damage_add(struct sna_damage *damage,
					    RegionPtr region)
{
	char region_buf[120];
	char damage_buf[1000];

	DBG(("%s(%s + %s)\n", __FUNCTION__,
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage),
	     _debug_describe_region(region_buf, sizeof(region_buf), region)));

	damage = __sna_damage_add(damage, region);

	DBG(("  = %s\n",
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage)));
	assert(region_num_rects(&damage->region));
	assert(damage->region.extents.x2 > damage->region.extents.x1);
	assert(damage->region.extents.y2 > damage->region.extents.y1);

	return damage;
}
#else
fastcall struct sna_damage *_sna_damage_add(struct sna_damage *damage,
					    RegionPtr region)
{
	return __sna_damage_add(damage, region);
}
#endif

inline static struct sna_damage *
__sna_damage_add_boxes(struct sna_damage *damage,
		       const BoxRec *box, int n,
		       int16_t dx, int16_t dy)
{
	BoxRec extents;
	int i;

	assert(n);

	if (!damage) {
		damage = _sna_damage_create();
		if (damage == NULL)
			return NULL;
	} else switch (damage->mode) {
	case DAMAGE_ALL:
		return damage;
	case DAMAGE_SUBTRACT:
		__sna_damage_reduce(damage);
	case DAMAGE_ADD:
		break;
	}

	assert(box[0].x2 > box[0].x1 && box[0].y2 > box[0].y1);
	extents = box[0];
	for (i = 1; i < n; i++) {
		assert(box[i].x2 > box[i].x1 && box[i].y2 > box[i].y1);
		if (extents.x1 > box[i].x1)
			extents.x1 = box[i].x1;
		if (extents.x2 < box[i].x2)
			extents.x2 = box[i].x2;
		if (extents.y1 > box[i].y1)
			extents.y1 = box[i].y1;
		if (extents.y2 < box[i].y2)
			extents.y2 = box[i].y2;
	}

	assert(extents.y2 > extents.y1 && extents.x2 > extents.x1);

	extents.x1 += dx;
	extents.x2 += dx;
	extents.y1 += dy;
	extents.y2 += dy;

	if (n == 1)
		return __sna_damage_add_box(damage, &extents);

	if (pixman_region_contains_rectangle(&damage->region,
					     &extents) == PIXMAN_REGION_IN)
		return damage;

	damage_union(damage, &extents);
	return _sna_damage_create_elt_from_boxes(damage, box, n, dx, dy);
}

#if HAS_DEBUG_FULL
struct sna_damage *_sna_damage_add_boxes(struct sna_damage *damage,
					 const BoxRec *b, int n,
					 int16_t dx, int16_t dy)
{
	char damage_buf[1000];

	DBG(("%s(%s + [(%d, %d), (%d, %d) ... x %d])\n", __FUNCTION__,
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage),
	     b->x1, b->y1, b->x2, b->y2, n));

	damage = __sna_damage_add_boxes(damage, b, n, dx, dy);

	DBG(("  = %s\n",
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage)));
	if (region_num_rects(&damage->region)) {
		assert(damage->region.extents.x2 > damage->region.extents.x1);
		assert(damage->region.extents.y2 > damage->region.extents.y1);
	}

	return damage;
}
#else
struct sna_damage *_sna_damage_add_boxes(struct sna_damage *damage,
					 const BoxRec *b, int n,
					 int16_t dx, int16_t dy)
{
	return __sna_damage_add_boxes(damage, b, n, dx, dy);
}
#endif

inline static struct sna_damage *
__sna_damage_add_rectangles(struct sna_damage *damage,
			    const xRectangle *r, int n,
			    int16_t dx, int16_t dy)
{
	BoxRec extents;
	int i;

	assert(n);

	assert(r[0].width && r[0].height);
	extents.x1 = r[0].x;
	extents.x2 = r[0].x + r[0].width;
	extents.y1 = r[0].y;
	extents.y2 = r[0].y + r[0].height;
	for (i = 1; i < n; i++) {
		assert(r[i].width && r[i].height);
		if (extents.x1 > r[i].x)
			extents.x1 = r[i].x;
		if (extents.x2 < r[i].x + r[i].width)
			extents.x2 = r[i].x + r[i].width;
		if (extents.y1 > r[i].y)
			extents.y1 = r[i].y;
		if (extents.y2 < r[i].y + r[i].height)
			extents.y2 = r[i].y + r[i].height;
	}

	assert(extents.y2 > extents.y1 && extents.x2 > extents.x1);

	extents.x1 += dx;
	extents.x2 += dx;
	extents.y1 += dy;
	extents.y2 += dy;

	if (n == 1)
		return __sna_damage_add_box(damage, &extents);

	if (!damage) {
		damage = _sna_damage_create();
		if (damage == NULL)
			return NULL;
	} else switch (damage->mode) {
	case DAMAGE_ALL:
		return damage;
	case DAMAGE_SUBTRACT:
		__sna_damage_reduce(damage);
	case DAMAGE_ADD:
		break;
	}

	if (pixman_region_contains_rectangle(&damage->region,
					     &extents) == PIXMAN_REGION_IN)
		return damage;

	damage_union(damage, &extents);
	return _sna_damage_create_elt_from_rectangles(damage, r, n, dx, dy);
}

#if HAS_DEBUG_FULL
struct sna_damage *_sna_damage_add_rectangles(struct sna_damage *damage,
					      const xRectangle *r, int n,
					      int16_t dx, int16_t dy)
{
	char damage_buf[1000];

	DBG(("%s(%s + [(%d, %d)x(%d, %d) ... x %d])\n", __FUNCTION__,
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage),
	     r->x, r->y, r->width, r->height, n));

	damage = __sna_damage_add_rectangles(damage, r, n, dx, dy);

	DBG(("  = %s\n",
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage)));
	if (region_num_rects(&damage->region)) {
		assert(damage->region.extents.x2 > damage->region.extents.x1);
		assert(damage->region.extents.y2 > damage->region.extents.y1);
	}

	return damage;
}
#else
struct sna_damage *_sna_damage_add_rectangles(struct sna_damage *damage,
					      const xRectangle *r, int n,
					      int16_t dx, int16_t dy)
{
	return __sna_damage_add_rectangles(damage, r, n, dx, dy);
}
#endif

/* XXX pass in extents? */
inline static struct sna_damage *
__sna_damage_add_points(struct sna_damage *damage,
			const DDXPointRec *p, int n,
			int16_t dx, int16_t dy)
{
	BoxRec extents;
	int i;

	assert(n);

	extents.x2 = extents.x1 = p[0].x;
	extents.y2 = extents.y1 = p[0].y;
	for (i = 1; i < n; i++) {
		if (extents.x1 > p[i].x)
			extents.x1 = p[i].x;
		else if (extents.x2 < p[i].x)
			extents.x2 = p[i].x;
		if (extents.y1 > p[i].y)
			extents.y1 = p[i].y;
		else if (extents.y2 < p[i].y)
			extents.y2 = p[i].y;
	}

	extents.x1 += dx;
	extents.x2 += dx + 1;
	extents.y1 += dy;
	extents.y2 += dy + 1;

	if (n == 1)
		return __sna_damage_add_box(damage, &extents);

	if (!damage) {
		damage = _sna_damage_create();
		if (damage == NULL)
			return NULL;
	} else switch (damage->mode) {
	case DAMAGE_ALL:
		return damage;
	case DAMAGE_SUBTRACT:
		__sna_damage_reduce(damage);
	case DAMAGE_ADD:
		break;
	}

	if (pixman_region_contains_rectangle(&damage->region,
					     &extents) == PIXMAN_REGION_IN)
		return damage;

	damage_union(damage, &extents);
	return _sna_damage_create_elt_from_points(damage, p, n, dx, dy);
}

#if HAS_DEBUG_FULL
struct sna_damage *_sna_damage_add_points(struct sna_damage *damage,
					  const DDXPointRec *p, int n,
					  int16_t dx, int16_t dy)
{
	char damage_buf[1000];

	DBG(("%s(%s + [(%d, %d) ... x %d])\n", __FUNCTION__,
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage),
	     p->x, p->y, n));

	damage = __sna_damage_add_points(damage, p, n, dx, dy);

	DBG(("  = %s\n",
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage)));
	if (region_num_rects(&damage->region)) {
		assert(damage->region.extents.x2 > damage->region.extents.x1);
		assert(damage->region.extents.y2 > damage->region.extents.y1);
	}

	return damage;
}
#else
struct sna_damage *_sna_damage_add_points(struct sna_damage *damage,
					  const DDXPointRec *p, int n,
					  int16_t dx, int16_t dy)
{
	return __sna_damage_add_points(damage, p, n, dx, dy);
}
#endif

#if HAS_DEBUG_FULL
fastcall struct sna_damage *_sna_damage_add_box(struct sna_damage *damage,
						const BoxRec *box)
{
	char damage_buf[1000];

	DBG(("%s(%s + [(%d, %d), (%d, %d)])\n", __FUNCTION__,
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage),
	     box->x1, box->y1, box->x2, box->y2));

	damage = __sna_damage_add_box(damage, box);

	DBG(("  = %s\n",
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage)));
	assert(region_num_rects(&damage->region));
	assert(damage->region.extents.x2 > damage->region.extents.x1);
	assert(damage->region.extents.y2 > damage->region.extents.y1);

	return damage;
}
#else
fastcall struct sna_damage *_sna_damage_add_box(struct sna_damage *damage,
						const BoxRec *box)
{
	return __sna_damage_add_box(damage, box);
}
#endif

struct sna_damage *__sna_damage_all(struct sna_damage *damage,
				    int width, int height)
{
	DBG(("%s(%d, %d)\n", __FUNCTION__, width, height));

	if (damage) {
		pixman_region_fini(&damage->region);
		free_list(&damage->embedded_box.list);
		reset_embedded_box(damage);
	} else {
		damage = _sna_damage_create();
		if (damage == NULL)
			return NULL;
	}

	pixman_region_init_rect(&damage->region, 0, 0, width, height);
	damage->extents = damage->region.extents;
	damage->mode = DAMAGE_ALL;

	return damage;
}

struct sna_damage *_sna_damage_is_all(struct sna_damage *damage,
				      int width, int height)
{
	DBG(("%s(%d, %d)%s?\n", __FUNCTION__, width, height,
	     damage->dirty ? "*" : ""));
	DBG(("%s: (%d, %d), (%d, %d)\n", __FUNCTION__,
	     damage->extents.x1, damage->extents.y1,
	     damage->extents.x2, damage->extents.y2));

	assert(damage->mode == DAMAGE_ADD);
	assert(damage->extents.x1 == 0 &&
	       damage->extents.y1 == 0 &&
	       damage->extents.x2 == width &&
	       damage->extents.y2 == height);

	if (damage->dirty) {
		__sna_damage_reduce(damage);
		assert(RegionNotEmpty(&damage->region));
	}

	if (damage->region.data) {
		DBG(("%s: no, not singular\n", __FUNCTION__));
		return damage;
	}

	assert(damage->extents.x1 == 0 &&
	       damage->extents.y1 == 0 &&
	       damage->extents.x2 == width &&
	       damage->extents.y2 == height);

	return __sna_damage_all(damage, width, height);
}

static bool box_contains(const BoxRec *a, const BoxRec *b)
{
	if (b->x1 < a->x1 || b->x2 > a->x2)
		return false;

	if (b->y1 < a->y1 || b->y2 > a->y2)
		return false;

	return true;
}

static struct sna_damage *__sna_damage_subtract(struct sna_damage *damage,
						RegionPtr region)
{
	if (damage == NULL)
		return NULL;

	if (RegionNil(&damage->region)) {
no_damage:
		__sna_damage_destroy(damage);
		return NULL;
	}

	assert(RegionNotEmpty(region));

	if (!sna_damage_overlaps_box(damage, &region->extents))
		return damage;

	if (region_is_singular(region) &&
	    box_contains(&region->extents, &damage->extents))
		goto no_damage;

	if (damage->mode == DAMAGE_ALL) {
		pixman_region_subtract(&damage->region,
				       &damage->region,
				       region);
		if (damage->region.extents.x2 <= damage->region.extents.x1 ||
		    damage->region.extents.y2 <= damage->region.extents.y1)
			goto no_damage;

		damage->extents = damage->region.extents;
		damage->mode = DAMAGE_ADD;
		return damage;
	}

	if (damage->mode != DAMAGE_SUBTRACT) {
		if (damage->dirty) {
			__sna_damage_reduce(damage);
			assert(RegionNotEmpty(&damage->region));
		}

		if (pixman_region_equal(region, &damage->region))
			goto no_damage;

		if (region_is_singular(&damage->region) &&
		    region_is_singular(region)) {
			pixman_region_subtract(&damage->region,
					       &damage->region,
					       region);
			if (damage->region.extents.x2 <= damage->region.extents.x1 ||
			    damage->region.extents.y2 <= damage->region.extents.y1)
				goto no_damage;

			damage->extents = damage->region.extents;
			assert(pixman_region_not_empty(&damage->region));
			return damage;
		}

		damage->mode = DAMAGE_SUBTRACT;
	}

	return _sna_damage_create_elt(damage,
				      region_rects(region),
				      region_num_rects(region));
}

#if HAS_DEBUG_FULL
fastcall struct sna_damage *_sna_damage_subtract(struct sna_damage *damage,
						 RegionPtr region)
{
	char damage_buf[1000];
	char region_buf[120];

	DBG(("%s(%s - %s)...\n", __FUNCTION__,
	       _debug_describe_damage(damage_buf, sizeof(damage_buf), damage),
	       _debug_describe_region(region_buf, sizeof(region_buf), region)));

	damage = __sna_damage_subtract(damage, region);

	DBG(("  = %s\n",
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage)));

	return damage;
}
#else
fastcall struct sna_damage *_sna_damage_subtract(struct sna_damage *damage,
						 RegionPtr region)
{
	return __sna_damage_subtract(damage, region);
}
#endif

inline static struct sna_damage *__sna_damage_subtract_box(struct sna_damage *damage,
							   const BoxRec *box)
{
	assert(box->x2 > box->x1 && box->y2 > box->y1);

	if (damage == NULL)
		return NULL;

	if (RegionNil(&damage->region)) {
		__sna_damage_destroy(damage);
		return NULL;
	}

	if (!sna_damage_overlaps_box(damage, box))
		return damage;

	if (box_contains(box, &damage->extents)) {
		__sna_damage_destroy(damage);
		return NULL;
	}

	if (damage->mode != DAMAGE_SUBTRACT) {
		if (damage->dirty) {
			__sna_damage_reduce(damage);
			assert(RegionNotEmpty(&damage->region));
		}

		if (region_is_singular(&damage->region)) {
			pixman_region16_t region;

			pixman_region_init_rects(&region, box, 1);
			pixman_region_subtract(&damage->region,
					       &damage->region,
					       &region);
			damage->extents = damage->region.extents;
			damage->mode = DAMAGE_ADD;
			return damage;
		}

		damage->mode = DAMAGE_SUBTRACT;
	}

	return _sna_damage_create_elt(damage, box, 1);
}

#if HAS_DEBUG_FULL
fastcall struct sna_damage *_sna_damage_subtract_box(struct sna_damage *damage,
						     const BoxRec *box)
{
	char damage_buf[1000];

	DBG(("%s(%s - (%d, %d), (%d, %d))...\n", __FUNCTION__,
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage),
	     box->x1, box->y1, box->x2, box->y2));

	damage = __sna_damage_subtract_box(damage, box);

	DBG(("  = %s\n",
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage)));

	return damage;
}
#else
fastcall struct sna_damage *_sna_damage_subtract_box(struct sna_damage *damage,
						     const BoxRec *box)
{
	return __sna_damage_subtract_box(damage, box);
}
#endif

static struct sna_damage *__sna_damage_subtract_boxes(struct sna_damage *damage,
						      const BoxRec *box, int n,
						      int dx, int dy)
{
	BoxRec extents;
	int i;

	if (damage == NULL)
		return NULL;

	if (RegionNil(&damage->region)) {
		__sna_damage_destroy(damage);
		return NULL;
	}

	assert(n);

	assert(box[0].x2 > box[0].x1 && box[0].y2 > box[0].y1);
	extents = box[0];
	for (i = 1; i < n; i++) {
		assert(box[i].x2 > box[i].x1 && box[i].y2 > box[i].y1);
		if (extents.x1 > box[i].x1)
			extents.x1 = box[i].x1;
		if (extents.x2 < box[i].x2)
			extents.x2 = box[i].x2;
		if (extents.y1 > box[i].y1)
			extents.y1 = box[i].y1;
		if (extents.y2 < box[i].y2)
			extents.y2 = box[i].y2;
	}

	assert(extents.y2 > extents.y1 && extents.x2 > extents.x1);

	extents.x1 += dx;
	extents.x2 += dx;
	extents.y1 += dy;
	extents.y2 += dy;

	if (!sna_damage_overlaps_box(damage, &extents))
		return damage;

	if (n == 1)
		return __sna_damage_subtract_box(damage, &extents);

	if (damage->mode != DAMAGE_SUBTRACT) {
		if (damage->dirty) {
			__sna_damage_reduce(damage);
			assert(RegionNotEmpty(&damage->region));
		}

		damage->mode = DAMAGE_SUBTRACT;
	}

	return _sna_damage_create_elt_from_boxes(damage, box, n, dx, dy);
}

#if HAS_DEBUG_FULL
fastcall struct sna_damage *_sna_damage_subtract_boxes(struct sna_damage *damage,
						       const BoxRec *box, int n,
						       int dx, int dy)
{
	char damage_buf[1000];

	DBG(("%s(%s - [(%d,%d), (%d,%d)...x%d])...\n", __FUNCTION__,
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage),
	     box->x1 + dx, box->y1 + dy,
	     box->x2 + dx, box->y2 + dy,
	     n));

	damage = __sna_damage_subtract_boxes(damage, box, n, dx, dy);

	DBG(("  = %s\n",
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage)));

	return damage;
}
#else
fastcall struct sna_damage *_sna_damage_subtract_boxes(struct sna_damage *damage,
						       const BoxRec *box, int n,
						       int dx, int dy)
{
	return __sna_damage_subtract_boxes(damage, box, n, dx, dy);
}
#endif

static int __sna_damage_contains_box(struct sna_damage **_damage,
				     const BoxRec *box)
{
	struct sna_damage *damage = *_damage;
	const BoxRec *b;
	int n, count, ret;

	if (damage->mode == DAMAGE_ALL)
		return PIXMAN_REGION_IN;

	if (!sna_damage_overlaps_box(damage, box))
		return PIXMAN_REGION_OUT;

	ret = pixman_region_contains_rectangle(&damage->region, (BoxPtr)box);
	if (!damage->dirty)
		return ret;

	if (damage->mode == DAMAGE_ADD) {
		if (ret == PIXMAN_REGION_IN)
			return ret;

		count = damage->embedded_box.size;
		if (list_is_empty(&damage->embedded_box.list))
			count -= damage->remain;

		b = damage->embedded_box.box;
		for (n = 0; n < count; n++) {
			if (box_contains(&b[n], box))
				return PIXMAN_REGION_IN;
		}
	} else {
		if (ret == PIXMAN_REGION_OUT)
			return ret;

		count = damage->embedded_box.size;
		if (list_is_empty(&damage->embedded_box.list))
			count -= damage->remain;

		b = damage->embedded_box.box;
		for (n = 0; n < count; n++) {
			if (box_contains(&b[n], box))
				return PIXMAN_REGION_OUT;
		}
	}

	__sna_damage_reduce(damage);
	if (!pixman_region_not_empty(&damage->region)) {
		__sna_damage_destroy(damage);
		*_damage = NULL;
		return PIXMAN_REGION_OUT;
	}

	return pixman_region_contains_rectangle(&damage->region, (BoxPtr)box);
}

#if HAS_DEBUG_FULL
int _sna_damage_contains_box(struct sna_damage **damage,
			     const BoxRec *box)
{
	char damage_buf[1000];
	int ret;

	DBG(("%s(%s, [(%d, %d), (%d, %d)])\n", __FUNCTION__,
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), *damage),
	     box->x1, box->y1, box->x2, box->y2));

	ret = __sna_damage_contains_box(damage, box);
	DBG(("  = %d", ret));
	if (ret)
		DBG((" [(%d, %d), (%d, %d)...]",
		     box->x1, box->y1, box->x2, box->y2));
	DBG(("\n"));

	return ret;
}
#else
int _sna_damage_contains_box(struct sna_damage **damage,
			     const BoxRec *box)
{
	return __sna_damage_contains_box(damage, box);
}
#endif

static bool box_overlaps(const BoxRec *a, const BoxRec *b)
{
	return (a->x1 < b->x2 && a->x2 > b->x1 &&
		a->y1 < b->y2 && a->y2 > b->y1);
}

bool _sna_damage_contains_box__no_reduce(const struct sna_damage *damage,
					 const BoxRec *box)
{
	int n, count;
	const BoxRec *b;

	assert(damage && damage->mode != DAMAGE_ALL);
	if (!box_contains(&damage->extents, box))
		return false;

	n = pixman_region_contains_rectangle((pixman_region16_t *)&damage->region, (BoxPtr)box);
	if (!damage->dirty)
		return n == PIXMAN_REGION_IN;

	if (damage->mode == DAMAGE_ADD) {
		if (n == PIXMAN_REGION_IN)
			return true;

		count = damage->embedded_box.size;
		if (list_is_empty(&damage->embedded_box.list))
			count -= damage->remain;

		b = damage->embedded_box.box;
		for (n = 0; n < count; n++) {
			if (box_contains(&b[n], box))
				return true;
		}

		return false;
	} else {
		if (n != PIXMAN_REGION_IN)
			return false;

		if (!list_is_empty(&damage->embedded_box.list))
			return false;

		count = damage->embedded_box.size - damage->remain;
		b = damage->embedded_box.box;
		for (n = 0; n < count; n++) {
			if (box_overlaps(&b[n], box))
				return false;
		}

		return true;
	}
}

static bool __sna_damage_intersect(struct sna_damage *damage,
				   RegionPtr region, RegionPtr result)
{
	assert(damage && damage->mode != DAMAGE_ALL);
	assert(RegionNotEmpty(region));

	if (region->extents.x2 <= damage->extents.x1 ||
	    region->extents.x1 >= damage->extents.x2)
		return false;

	if (region->extents.y2 <= damage->extents.y1 ||
	    region->extents.y1 >= damage->extents.y2)
		return false;

	if (damage->dirty)
		__sna_damage_reduce(damage);

	if (!pixman_region_not_empty(&damage->region))
		return false;

	RegionNull(result);
	RegionIntersect(result, &damage->region, region);

	return RegionNotEmpty(result);
}

#if HAS_DEBUG_FULL
bool _sna_damage_intersect(struct sna_damage *damage,
			   RegionPtr region, RegionPtr result)
{
	char damage_buf[1000];
	char region_buf[120];
	bool ret;

	DBG(("%s(%s, %s)...\n", __FUNCTION__,
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage),
	     _debug_describe_region(region_buf, sizeof(region_buf), region)));

	ret = __sna_damage_intersect(damage, region, result);
	if (ret)
		DBG(("  = %s\n",
		     _debug_describe_region(region_buf, sizeof(region_buf), result)));
	else
		DBG(("  = none\n"));

	return ret;
}
#else
bool _sna_damage_intersect(struct sna_damage *damage,
			  RegionPtr region, RegionPtr result)
{
	return __sna_damage_intersect(damage, region, result);
}
#endif

static int __sna_damage_get_boxes(struct sna_damage *damage, const BoxRec **boxes)
{
	assert(damage && damage->mode != DAMAGE_ALL);

	if (damage->dirty)
		__sna_damage_reduce(damage);

	assert(!damage->dirty);
	assert(damage->mode == DAMAGE_ADD);

	*boxes = region_rects(&damage->region);
	return region_num_rects(&damage->region);
}

struct sna_damage *_sna_damage_reduce(struct sna_damage *damage)
{
	DBG(("%s\n", __FUNCTION__));

	__sna_damage_reduce(damage);

	assert(!damage->dirty);
	assert(damage->mode == DAMAGE_ADD);

	if (!pixman_region_not_empty(&damage->region)) {
		__sna_damage_destroy(damage);
		damage = NULL;
	}

	return damage;
}

#if HAS_DEBUG_FULL
int _sna_damage_get_boxes(struct sna_damage *damage, const BoxRec **boxes)
{
	char damage_buf[1000];
	int count;

	DBG(("%s(%s)...\n", __FUNCTION__,
	     _debug_describe_damage(damage_buf, sizeof(damage_buf), damage)));

	count = __sna_damage_get_boxes(damage, boxes);
	DBG(("  = %d\n", count));

	return count;
}
#else
int _sna_damage_get_boxes(struct sna_damage *damage, const BoxRec **boxes)
{
	return __sna_damage_get_boxes(damage, boxes);
}
#endif

struct sna_damage *_sna_damage_combine(struct sna_damage *l,
				       struct sna_damage *r,
				       int dx, int dy)
{
	if (r->dirty)
		__sna_damage_reduce(r);

	if (pixman_region_not_empty(&r->region)) {
		pixman_region_translate(&r->region, dx, dy);
		l = __sna_damage_add(l, &r->region);
	}

	return l;
}

void __sna_damage_destroy(struct sna_damage *damage)
{
	free_list(&damage->embedded_box.list);

	pixman_region_fini(&damage->region);
	*(void **)damage = __freed_damage;
	__freed_damage = damage;
}

#if TEST_DAMAGE && HAS_DEBUG_FULL
struct sna_damage_selftest{
	int width, height;
};

static void st_damage_init_random_box(struct sna_damage_selftest *test,
				      BoxPtr box)
{
	int x, y, w, h;

	if (test->width == 1) {
		x = 0, w = 1;
	} else {
		x = rand() % (test->width - 1);
		w = 1 + rand() % (test->width - x - 1);
	}

	if (test->height == 1) {
		y = 0, h = 1;
	} else {
		y = rand() % (test->height - 1);
		h = 1 + rand() % (test->height - y - 1);
	}

	box->x1 = x;
	box->x2 = x+w;

	box->y1 = y;
	box->y2 = y+h;
}

static void st_damage_init_random_region1(struct sna_damage_selftest *test,
					  pixman_region16_t *region)
{
	int x, y, w, h;

	if (test->width == 1) {
		x = 0, w = 1;
	} else {
		x = rand() % (test->width - 1);
		w = 1 + rand() % (test->width - x - 1);
	}

	if (test->height == 1) {
		y = 0, h = 1;
	} else {
		y = rand() % (test->height - 1);
		h = 1 + rand() % (test->height - y - 1);
	}

	pixman_region_init_rect(region, x, y, w, h);
}

static void st_damage_add(struct sna_damage_selftest *test,
			  struct sna_damage **damage,
			  pixman_region16_t *region)
{
	pixman_region16_t tmp;

	st_damage_init_random_region1(test, &tmp);

	if (!DAMAGE_IS_ALL(*damage))
		sna_damage_add(damage, &tmp);
	pixman_region_union(region, region, &tmp);
}

static void st_damage_add_box(struct sna_damage_selftest *test,
			      struct sna_damage **damage,
			      pixman_region16_t *region)
{
	RegionRec r;

	st_damage_init_random_box(test, &r.extents);
	r.data = NULL;

	if (!DAMAGE_IS_ALL(*damage))
		sna_damage_add_box(damage, &r.extents);
	pixman_region_union(region, region, &r);
}

static void st_damage_subtract(struct sna_damage_selftest *test,
			       struct sna_damage **damage,
			       pixman_region16_t *region)
{
	pixman_region16_t tmp;

	st_damage_init_random_region1(test, &tmp);

	sna_damage_subtract(damage, &tmp);
	pixman_region_subtract(region, region, &tmp);
}

static void st_damage_subtract_box(struct sna_damage_selftest *test,
				   struct sna_damage **damage,
				   pixman_region16_t *region)
{
	RegionRec r;

	st_damage_init_random_box(test, &r.extents);
	r.data = NULL;

	sna_damage_subtract_box(damage, &r.extents);
	pixman_region_subtract(region, region, &r);
}

static void st_damage_all(struct sna_damage_selftest *test,
			  struct sna_damage **damage,
			  pixman_region16_t *region)
{
	pixman_region16_t tmp;

	pixman_region_init_rect(&tmp, 0, 0, test->width, test->height);

	if (!DAMAGE_IS_ALL(*damage))
		sna_damage_all(damage, test->width, test->height);
	pixman_region_union(region, region, &tmp);
}

static bool st_check_equal(struct sna_damage_selftest *test,
			   struct sna_damage **damage,
			   pixman_region16_t *region)
{
	int d_num, r_num;
	BoxPtr d_boxes, r_boxes;

	d_num = *damage ? sna_damage_get_boxes(*damage, &d_boxes) : 0;
	r_boxes = pixman_region_rectangles(region, &r_num);

	if (d_num != r_num) {
		ERR(("%s: damage and ref contain different number of rectangles\n",
		     __FUNCTION__));
		return false;
	}

	if (memcmp(d_boxes, r_boxes, d_num*sizeof(BoxRec))) {
		ERR(("%s: damage and ref contain different rectangles\n",
		     __FUNCTION__));
		return false;
	}

	return true;
}

void sna_damage_selftest(void)
{
	void (*const op[])(struct sna_damage_selftest *test,
			   struct sna_damage **damage,
			   pixman_region16_t *region) = {
		st_damage_add,
		st_damage_add_box,
		st_damage_subtract,
		st_damage_subtract_box,
		st_damage_all
	};
	bool (*const check[])(struct sna_damage_selftest *test,
			      struct sna_damage **damage,
			      pixman_region16_t *region) = {
		st_check_equal,
		//st_check_contains,
	};
	char region_buf[120];
	char damage_buf[1000];
	int pass;

	for (pass = 0; pass < 16384; pass++) {
		struct sna_damage_selftest test;
		struct sna_damage *damage;
		pixman_region16_t ref;
		int iter, i;

		iter = 1 + rand() % (1 + (pass / 64));
		DBG(("%s: pass %d, iters=%d\n", __FUNCTION__, pass, iter));

		test.width = 1 + rand() % 2048;
		test.height = 1 + rand() % 2048;

		damage = _sna_damage_create();
		pixman_region_init(&ref);

		for (i = 0; i < iter; i++) {
			op[rand() % ARRAY_SIZE(op)](&test, &damage, &ref);
		}

		if (!check[rand() % ARRAY_SIZE(check)](&test, &damage, &ref)) {
			FatalError("%s: failed - region = %s, damage = %s\n", __FUNCTION__,
				   _debug_describe_region(region_buf, sizeof(region_buf), &ref),
				   _debug_describe_damage(damage_buf, sizeof(damage_buf), damage));
		}

		pixman_region_fini(&ref);
		sna_damage_destroy(&damage);
	}
}
#endif

void _sna_damage_debug_get_region(struct sna_damage *damage, RegionRec *r)
{
	int n, nboxes;
	BoxPtr boxes;
	struct sna_damage_box *iter;

	RegionCopy(r, &damage->region);
	if (!damage->dirty)
		return;

	nboxes = damage->embedded_box.size;
	list_for_each_entry(iter, &damage->embedded_box.list, list)
		nboxes += iter->size;
	nboxes -= damage->remain;
	if (nboxes == 0)
		return;

	if (nboxes == 1) {
		pixman_region16_t tmp;

		tmp.extents = damage->embedded_box.box[0];
		tmp.data = NULL;

		if (damage->mode == DAMAGE_ADD)
			pixman_region_union(r, r, &tmp);
		else
			pixman_region_subtract(r, r, &tmp);

		return;
	}

	if (damage->mode == DAMAGE_ADD)
		nboxes += region_num_rects(r);

	iter = last_box(damage);
	n = iter->size - damage->remain;
	boxes = malloc(sizeof(BoxRec)*nboxes);
	if (boxes == NULL)
		return;

	if (list_is_empty(&damage->embedded_box.list)) {
		memcpy(boxes,
		       damage->embedded_box.box,
		       n*sizeof(BoxRec));
	} else {
		if (boxes != (BoxPtr)(iter+1))
			memcpy(boxes, iter+1, n*sizeof(BoxRec));

		iter = list_entry(iter->list.prev,
				  struct sna_damage_box,
				  list);
		while (&iter->list != &damage->embedded_box.list) {
			memcpy(boxes + n, iter+1,
			       iter->size * sizeof(BoxRec));
			n += iter->size;

			iter = list_entry(iter->list.prev,
					  struct sna_damage_box,
					  list);
		}

		memcpy(boxes + n,
		       damage->embedded_box.box,
		       sizeof(damage->embedded_box.box));
		n += damage->embedded_box.size;
	}

	if (damage->mode == DAMAGE_ADD) {
		memcpy(boxes + n,
		       region_rects(r),
		       region_num_rects(r)*sizeof(BoxRec));
		assert(n + region_num_rects(r) == nboxes);
		pixman_region_fini(r);
		pixman_region_init_rects(r, boxes, nboxes);

		assert(pixman_region_not_empty(r));
		assert(damage->extents.x1 == r->extents.x1 &&
		       damage->extents.y1 == r->extents.y1 &&
		       damage->extents.x2 == r->extents.x2 &&
		       damage->extents.y2 == r->extents.y2);
	} else {
		pixman_region16_t tmp;

		pixman_region_init_rects(&tmp, boxes, nboxes);
		pixman_region_subtract(r, r, &tmp);
		pixman_region_fini(&tmp);

		assert(damage->extents.x1 <= r->extents.x1 &&
		       damage->extents.y1 <= r->extents.y1 &&
		       damage->extents.x2 >= r->extents.x2 &&
		       damage->extents.y2 >= r->extents.y2);
	}

	free(boxes);
}
