/*
 * Copyright © 2012 Intel Corporation
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

#include "fb.h"
#include "fbclip.h"

static const BoxRec *
find_clip_row_for_y(const BoxRec *begin, const BoxRec *end, int16_t y)
{
	const BoxRec *mid;

	if (end == begin)
		return end;

	if (end - begin == 1) {
		if (begin->y2 > y)
			return begin;
		else
			return end;
	}

	mid = begin + (end - begin) / 2;
	if (mid->y2 > y)
		return find_clip_row_for_y(begin, mid, y);
	else
		return find_clip_row_for_y(mid, end, y);
}

const BoxRec *
fbClipBoxes(const RegionRec *region, const BoxRec *box, const BoxRec **end)
{
	const BoxRec *c0, *c1;

	DBG(("%s: box=(%d, %d),(%d, %d); region=(%d, %d),(%d, %d) x %ld\n",
	     __FUNCTION__,
	     box->x1, box->y1, box->x2, box->y2,
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2,
	     region->data ? region->data->numRects : 1));

	if (box->x1 >= region->extents.x2 || box->x2 <= region->extents.x1 ||
	    box->y1 >= region->extents.y2 || box->y2 <= region->extents.y1) {
		DBG(("%s: no intersection\n", __FUNCTION__));
		return *end = box;
	}

	if (region->data == NULL) {
		*end = &region->extents + 1;
		return &region->extents;
	}

	c0 = (const BoxRec *)(region->data + 1);
	c1 = c0 + region->data->numRects;

	if (c0->y2 <= box->y1) {
		DBG(("%s: first clip (%d, %d), (%d, %d) before box (%d, %d), (%d, %d)\n",
		     __FUNCTION__,
		     c0->x1, c0->y1, c0->x2, c0->y2,
		     box->x1, box->y1, box->x2, box->y2));
		c0 = find_clip_row_for_y(c0, c1, box->y1);
	}

	DBG(("%s: c0=(%d, %d),(%d, %d) x %ld\n",
	     __FUNCTION__, c0->x1, c0->y1, c0->x2, c0->y2, (long)(c1 - c0)));

	*end = c1;
	return c0;
}
