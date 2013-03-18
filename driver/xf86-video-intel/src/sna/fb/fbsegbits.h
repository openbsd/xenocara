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

#define isClipped(c,ul,lr)  (((c) | ((c) - (ul)) | ((lr) - (c))) & 0x80008000)
#define RROP(b,a,x)	WRITE((b), FbDoRRop (READ(b), (a), (x)))

static void
BRESSOLID(DrawablePtr drawable, GCPtr gc, int dashOffset,
	  int sdx, int sdy, int axis,
	  int x1, int y1, int e, int e1, int e3, int len)
{
	FbBits *dst;
	FbStride stride;
	int bpp, dx, dy;
	BITS *bits;
	FbStride major, minor;
	BITS xor = fb_gc(gc)->xor;

	fbGetDrawable(drawable, dst, stride, bpp, dx, dy);
	bits = (BITS *)(dst + (y1 + dy) * stride) + (x1 + dx);
	stride = stride * (sizeof(FbBits) / sizeof(BITS));
	if (sdy < 0)
		stride = -stride;
	if (axis == X_AXIS) {
		major = sdx;
		minor = stride;
	} else {
		major = stride;
		minor = sdx;
	}
	while (len--) {
		WRITE(bits, xor);
		bits += major;
		e += e1;
		if (e >= 0) {
			bits += minor;
			e += e3;
		}
	}
}

static void
BRESSOLIDR(DrawablePtr drawable, GCPtr gc, int dashOffset,
	   int sdx, int sdy, int axis,
	   int x1, int y1, int e, int e1, int e3, int len)
{
	FbBits *dst;
	FbStride stride;
	int bpp, dx, dy;
	BITS *bits;
	FbStride major, minor;
	BITS and = fb_gc(gc)->and;
	BITS xor = fb_gc(gc)->xor;

	fbGetDrawable(drawable, dst, stride, bpp, dx, dy);
	bits = (BITS *)(dst + (y1 + dy) * stride) + (x1 + dx);
	stride = stride * (sizeof(FbBits) / sizeof(BITS));
	if (sdy < 0)
		stride = -stride;
	if (axis == X_AXIS) {
		major = sdx;
		minor = stride;
	} else {
		major = stride;
		minor = sdx;
	}
	while (len--) {
		RROP(bits, and, xor);
		bits += major;
		e += e1;
		if (e >= 0) {
			bits += minor;
			e += e3;
		}
	}
}

static void
BRESDASH(DrawablePtr drawable, GCPtr gc, int dashOffset,
	 int sdx, int sdy, int axis,
	 int x1, int y1, int e, int e1, int e3, int len)
{
	FbBits *dst;
	FbStride stride;
	int bpp, dx, dy;
	BITS *bits;
	FbStride major, minor;

	FbDashDeclare;
	int dashlen;
	bool even;
	bool doOdd = gc->lineStyle == LineDoubleDash;
	BITS xorfg = fb_gc(gc)->xor;
	BITS xorbg = fb_gc(gc)->bgxor;

	fbGetDrawable(drawable, dst, stride, bpp, dx, dy);

	FbDashInit(gc, fb_gc(gc), dashOffset, dashlen, even);

	bits = ((BITS *) (dst + ((y1 + dy) * stride))) + (x1 + dx);
	stride = stride * (sizeof(FbBits) / sizeof(BITS));
	if (sdy < 0)
		stride = -stride;
	if (axis == X_AXIS) {
		major = sdx;
		minor = stride;
	} else {
		major = stride;
		minor = sdx;
	}
	if (dashlen >= len)
		dashlen = len;
	if (doOdd) {
		if (!even)
			goto doubleOdd;
		for (;;) {
			len -= dashlen;
			while (dashlen--) {
				WRITE(bits, xorfg);
				bits += major;
				if ((e += e1) >= 0) {
					e += e3;
					bits += minor;
				}
			}
			if (!len)
				break;

			FbDashNextEven(dashlen);

			if (dashlen >= len)
				dashlen = len;
doubleOdd:
			len -= dashlen;
			while (dashlen--) {
				WRITE(bits, xorbg);
				bits += major;
				if ((e += e1) >= 0) {
					e += e3;
					bits += minor;
				}
			}
			if (!len)
				break;

			FbDashNextOdd(dashlen);

			if (dashlen >= len)
				dashlen = len;
		}
	} else {
		if (!even)
			goto onOffOdd;
		for (;;) {
			len -= dashlen;
			while (dashlen--) {
				WRITE(bits, xorfg);
				bits += major;
				if ((e += e1) >= 0) {
					e += e3;
					bits += minor;
				}
			}
			if (!len)
				break;

			FbDashNextEven(dashlen);

			if (dashlen >= len)
				dashlen = len;
onOffOdd:
			len -= dashlen;
			while (dashlen--) {
				bits += major;
				if ((e += e1) >= 0) {
					e += e3;
					bits += minor;
				}
			}
			if (!len)
				break;

			FbDashNextOdd(dashlen);

			if (dashlen >= len)
				dashlen = len;
		}
	}
}

#undef RROP
#undef isClipped
