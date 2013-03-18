/*
 * Copyright © 1998 Keith Packard
 * Copyright © 2012 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Keith Packard not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Keith Packard makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include "fb.h"
#include "fbclip.h"

inline static void
fbFillSpan(DrawablePtr drawable, GCPtr gc, const BoxRec *b, void *data)
{
	DBG(("%s (%d,%d)+%d\n", __FUNCTION__, b->x1, b->y1, b->x2-b->x1));
	fbFill(drawable, gc, b->x1, b->y1, b->x2 - b->x1, 1);
}

void
fbFillSpans(DrawablePtr drawable, GCPtr gc,
	    int n, DDXPointPtr pt, int *width, int fSorted)
{
	DBG(("%s x %d\n", __FUNCTION__, n));
	while (n--) {
		BoxRec box;

		*(DDXPointPtr)&box = *pt++;
		box.x2 = box.x1 + *width++;
		box.y2 = box.y1 + 1;

		/* XXX fSorted */
		fbDrawableRun(drawable, gc, &box, fbFillSpan, NULL);
	}
}

struct fbSetSpan {
	char *src;
	DDXPointRec pt;
	FbStride stride;
	FbBits *dst;
	int dx, dy;
};

inline static void
fbSetSpan(DrawablePtr drawable, GCPtr gc, const BoxRec *b, void *_data)
{
	struct fbSetSpan *data = _data;
	int xoff, bpp;

	xoff = (int) (((long)data->src) & (FB_MASK >> 3));
	bpp = drawable->bitsPerPixel;

	fbBlt((FbBits *)(data->src - xoff), 0,
	      (b->x1 - data->pt.x) * bpp + (xoff << 3),
	      data->dst + (b->y1 + data->dy) * data->stride, data->stride,
	      (b->x1 + data->dx) * bpp,
	      (b->x2 - b->x1) * bpp, 1,
	      gc->alu, fb_gc(gc)->pm, bpp,
	      FALSE, FALSE);
}

void
fbSetSpans(DrawablePtr drawable, GCPtr gc,
           char *src, DDXPointPtr pt, int *width, int n, int fSorted)
{
	struct fbSetSpan data;
	PixmapPtr pixmap;

	DBG(("%s x %d\n", __FUNCTION__, n));

	fbGetDrawablePixmap(drawable, pixmap, data.dx, data.dy);
	data.dst = pixmap->devPrivate.ptr;
	data.stride = pixmap->devKind / sizeof(FbStip);

	data.src = src;
	while (n--) {
		BoxRec box;

		*(DDXPointPtr)&box = data.pt = *pt;
		box.x2 = box.x1 + *width;
		box.y2 = box.y1 + 1;

		fbDrawableRun(drawable, gc, &box, fbSetSpan, &data);

		data.src += PixmapBytePad(*width, drawable->depth);
		width++;
		pt++;
	}
}

void
fbGetSpans(DrawablePtr drawable, int wMax,
           DDXPointPtr pt, int *width, int n, char *dst)
{
	FbBits *src, *d;
	FbStride srcStride;
	int srcBpp;
	int srcXoff, srcYoff;
	int xoff;

	fbGetDrawable(drawable, src, srcStride, srcBpp, srcXoff, srcYoff);

	DBG(("%s x %d\n", __FUNCTION__, n));
	while (n--) {
		xoff = (int) (((long) dst) & (FB_MASK >> 3));
		d = (FbBits *) (dst - xoff);
		fbBlt(src + (pt->y + srcYoff) * srcStride, srcStride,
		      (pt->x + srcXoff) * srcBpp,
		      d, 1, xoff << 3, *width * srcBpp,
		      1, GXcopy, FB_ALLONES, srcBpp,
		      FALSE, FALSE);
		dst += PixmapBytePad(*width, drawable->depth);
		pt++;
		width++;
	}
}
