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

#define ARCCOPY(d)  WRITE(d,xorBits)
#define ARCRROP(d)  RROP(d,andBits,xorBits)

static void
ARC(FbBits * dst,
    FbStride dstStride,
    int dstBpp, xArc * arc, int drawX, int drawY, FbBits and, FbBits xor)
{
	BITS *bits;
	FbStride bitsStride;
	miZeroArcRec info;
	Bool do360;
	int x;
	BITS *yorgp, *yorgop;
	BITS andBits, xorBits;
	int yoffset, dyoffset;
	int y, a, b, d, mask;
	int k1, k3, dx, dy;

	bits = (BITS *) dst;
	bitsStride = dstStride * (sizeof(FbBits) / sizeof(BITS));
	andBits = (BITS) and;
	xorBits = (BITS) xor;
	do360 = miZeroArcSetup(arc, &info, TRUE);
	yorgp = bits + ((info.yorg + drawY) * bitsStride);
	yorgop = bits + ((info.yorgo + drawY) * bitsStride);
	info.xorg = (info.xorg + drawX);
	info.xorgo = (info.xorgo + drawX);
	MIARCSETUP();
	yoffset = y ? bitsStride : 0;
	dyoffset = 0;
	mask = info.initialMask;

	if (!(arc->width & 1)) {
		if (andBits == 0) {
			if (mask & 2)
				ARCCOPY(yorgp + info.xorgo);
			if (mask & 8)
				ARCCOPY(yorgop + info.xorgo);
		} else {
			if (mask & 2)
				ARCRROP(yorgp + info.xorgo);
			if (mask & 8)
				ARCRROP(yorgop + info.xorgo);
		}
	}
	if (!info.end.x || !info.end.y) {
		mask = info.end.mask;
		info.end = info.altend;
	}
	if (do360 && (arc->width == arc->height) && !(arc->width & 1)) {
		int xoffset = bitsStride;
		BITS *yorghb = yorgp + (info.h * bitsStride) + info.xorg;
		BITS *yorgohb = yorghb - info.h;

		yorgp += info.xorg;
		yorgop += info.xorg;
		yorghb += info.h;
		while (1) {
			if (andBits == 0) {
				ARCCOPY(yorgp + yoffset + x);
				ARCCOPY(yorgp + yoffset - x);
				ARCCOPY(yorgop - yoffset - x);
				ARCCOPY(yorgop - yoffset + x);
			} else {
				ARCRROP(yorgp + yoffset + x);
				ARCRROP(yorgp + yoffset - x);
				ARCRROP(yorgop - yoffset - x);
				ARCRROP(yorgop - yoffset + x);
			}
			if (a < 0)
				break;
			if (andBits == 0) {
				ARCCOPY(yorghb - xoffset - y);
				ARCCOPY(yorgohb - xoffset + y);
				ARCCOPY(yorgohb + xoffset + y);
				ARCCOPY(yorghb + xoffset - y);
			} else {
				ARCRROP(yorghb - xoffset - y);
				ARCRROP(yorgohb - xoffset + y);
				ARCRROP(yorgohb + xoffset + y);
				ARCRROP(yorghb + xoffset - y);
			}
			xoffset += bitsStride;
			MIARCCIRCLESTEP(yoffset += bitsStride;
				       );
		}
		yorgp -= info.xorg;
		yorgop -= info.xorg;
		x = info.w;
		yoffset = info.h * bitsStride;
	} else if (do360) {
		while (y < info.h || x < info.w) {
			MIARCOCTANTSHIFT(dyoffset = bitsStride;
					);
			if (andBits == 0) {
				ARCCOPY(yorgp + yoffset + info.xorg + x);
				ARCCOPY(yorgp + yoffset + info.xorgo - x);
				ARCCOPY(yorgop - yoffset + info.xorgo - x);
				ARCCOPY(yorgop - yoffset + info.xorg + x);
			} else {
				ARCRROP(yorgp + yoffset + info.xorg + x);
				ARCRROP(yorgp + yoffset + info.xorgo - x);
				ARCRROP(yorgop - yoffset + info.xorgo - x);
				ARCRROP(yorgop - yoffset + info.xorg + x);
			}
			MIARCSTEP(yoffset += dyoffset;
				  , yoffset += bitsStride;
				 );
		}
	} else {
		while (y < info.h || x < info.w) {
			MIARCOCTANTSHIFT(dyoffset = bitsStride;
					);
			if ((x == info.start.x) || (y == info.start.y)) {
				mask = info.start.mask;
				info.start = info.altstart;
			}
			if (andBits == 0) {
				if (mask & 1)
					ARCCOPY(yorgp + yoffset + info.xorg + x);
				if (mask & 2)
					ARCCOPY(yorgp + yoffset + info.xorgo - x);
				if (mask & 4)
					ARCCOPY(yorgop - yoffset + info.xorgo - x);
				if (mask & 8)
					ARCCOPY(yorgop - yoffset + info.xorg + x);
			} else {
				if (mask & 1)
					ARCRROP(yorgp + yoffset + info.xorg + x);
				if (mask & 2)
					ARCRROP(yorgp + yoffset + info.xorgo - x);
				if (mask & 4)
					ARCRROP(yorgop - yoffset + info.xorgo - x);
				if (mask & 8)
					ARCRROP(yorgop - yoffset + info.xorg + x);
			}
			if ((x == info.end.x) || (y == info.end.y)) {
				mask = info.end.mask;
				info.end = info.altend;
			}
			MIARCSTEP(yoffset += dyoffset;
				  , yoffset += bitsStride;
				 );
		}
	}
	if ((x == info.start.x) || (y == info.start.y))
		mask = info.start.mask;
	if (andBits == 0) {
		if (mask & 1)
			ARCCOPY(yorgp + yoffset + info.xorg + x);
		if (mask & 4)
			ARCCOPY(yorgop - yoffset + info.xorgo - x);
		if (arc->height & 1) {
			if (mask & 2)
				ARCCOPY(yorgp + yoffset + info.xorgo - x);
			if (mask & 8)
				ARCCOPY(yorgop - yoffset + info.xorg + x);
		}
	} else {
		if (mask & 1)
			ARCRROP(yorgp + yoffset + info.xorg + x);
		if (mask & 4)
			ARCRROP(yorgop - yoffset + info.xorgo - x);
		if (arc->height & 1) {
			if (mask & 2)
				ARCRROP(yorgp + yoffset + info.xorgo - x);
			if (mask & 8)
				ARCRROP(yorgop - yoffset + info.xorg + x);
		}
	}
}

#undef ARCCOPY
#undef ARCRROP

#undef RROP
#undef isClipped
