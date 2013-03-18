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

#ifndef FBCLIP_H
#define FBCLIP_H

extern const BoxRec *
fbClipBoxes(const RegionRec *region, const BoxRec *box, const BoxRec **end);

inline static bool
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

#define run_box(b, c) \
	DBG(("%s: box=(%d, %d), (%d, %d), clip=(%d, %d), (%d, %d)\n", \
	     __FUNCTION__, (b)->x1, (b)->y1, (b)->x2, (b)->y2, (c)->x1, (c)->y1, (c)->x2, (c)->y2)); \
	if ((b)->y2 <= (c)->y1) break; \
	if ((b)->x1 >= (c)->x2) continue; \
	if ((b)->x2 <= (c)->x1) { if ((b)->y2 <= (c)->y2) break; continue; }

static inline void
fbDrawableRun(DrawablePtr d, GCPtr gc, const BoxRec *box,
	      void (*func)(DrawablePtr, GCPtr, const BoxRec *b, void *data),
	      void *data)
{
	const BoxRec *c, *end;
	for (c = fbClipBoxes(gc->pCompositeClip, box, &end); c != end; c++) {
		BoxRec b;

		run_box(box, c);

		b = *box;
		if (box_intersect(&b, c))
			func(d, gc, &b, data);
	}
}

static inline void
fbDrawableRunUnclipped(DrawablePtr d, GCPtr gc, const BoxRec *box,
		       void (*func)(DrawablePtr, GCPtr, const BoxRec *b, void *data),
		       void *data)
{
	const BoxRec *c, *end;
	for (c = fbClipBoxes(gc->pCompositeClip, box, &end); c != end; c++) {
		run_box(box, c);
		func(d, gc, c, data);
	}
}

#endif /* FBCLIP_H */
