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
POLYLINE(DrawablePtr drawable, GCPtr gc, int mode, int n_0, DDXPointPtr pt_0)
{
	int xoff = drawable->x;
	int yoff = drawable->y;
	unsigned int bias = miGetZeroLineBias(drawable->pScreen);
	const BoxRec *clip = RegionRects(gc->pCompositeClip);
	const BoxRec *const last_clip = clip + RegionNumRects(gc->pCompositeClip);

	FbBits *dst;
	int dstStride;
	int dstBpp;
	int dstXoff, dstYoff;

	BITS *bits, *bitsBase;
	FbStride bitsStride;
	BITS xor = fb_gc(gc)->xor;
	BITS and = fb_gc(gc)->and;


	int e, e1, e3, len;
	int stepmajor, stepminor;
	int octant;

	if (mode == CoordModePrevious)
		fbFixCoordModePrevious(n_0, pt_0);

	fbGetDrawable(drawable, dst, dstStride, dstBpp, dstXoff, dstYoff);
	bitsStride = dstStride * (sizeof(FbBits) / sizeof(BITS));
	bitsBase =
		((BITS *) dst) + (yoff + dstYoff) * bitsStride + (xoff + dstXoff);
	do {
		INT32 *pt = (INT32 *)pt_0;
		int n = n_0;
		INT32 pt1, pt2;

		INT32 ul = coordToInt(clip->x1 - xoff, clip->y1 - yoff);
		INT32 lr = coordToInt(clip->x2 - xoff - 1, clip->y2 - yoff - 1);

		pt1 = *pt++; n--;
		pt2 = *pt++; n--;
		for (;;) {
			if (isClipped(pt1, ul, lr) | isClipped(pt2, ul, lr)) {
				int dashoffset = 0;
				fbSegment1(drawable, gc, clip,
					  intToX(pt1) + xoff, intToY(pt1) + yoff,
					  intToX(pt2) + xoff, intToY(pt2) + yoff,
					  n == 0 && gc->capStyle != CapNotLast, &dashoffset);
				if (!n)
					return;

				pt1 = pt2;
				pt2 = *pt++;
				n--;
			} else {
				bits = bitsBase + intToY(pt1) * bitsStride + intToX(pt1);
				for (;;) {
					CalcLineDeltas(intToX(pt1), intToY(pt1),
						       intToX(pt2), intToY(pt2),
						       len, e1, stepmajor, stepminor, 1, bitsStride,
						       octant);
					if (len < e1) {
						e3 = len;
						len = e1;
						e1 = e3;

						e3 = stepminor;
						stepminor = stepmajor;
						stepmajor = e3;
						SetYMajorOctant(octant);
					}
					e = -len;
					e1 <<= 1;
					e3 = e << 1;
					FIXUP_ERROR(e, octant, bias);
					if (and == 0) {
						while (len--) {
							WRITE(bits, xor);
							bits += stepmajor;
							e += e1;
							if (e >= 0) {
								bits += stepminor;
								e += e3;
							}
						}
					} else {
						while (len--) {
							RROP(bits, and, xor);
							bits += stepmajor;
							e += e1;
							if (e >= 0) {
								bits += stepminor;
								e += e3;
							}
						}
					}
					if (!n) {
						if (gc->capStyle != CapNotLast &&
						    pt2 != *((INT32 *)pt_0)) {
							RROP(bits, and, xor);
						}
						return;
					}
					pt1 = pt2;
					pt2 = *pt++;
					--n;
					if (isClipped(pt2, ul, lr))
						break;
				}
			}
		}
	} while (++clip != last_clip);
}

static void
POLYSEGMENT(DrawablePtr drawable, GCPtr gc, int n_0, xSegment *seg_0)
{
	int xoff = drawable->x;
	int yoff = drawable->y;
	unsigned int bias = miGetZeroLineBias(drawable->pScreen);
	const BoxRec *clip = RegionRects(gc->pCompositeClip);
	const BoxRec *const last_clip = clip + RegionNumRects(gc->pCompositeClip);

	FbBits *dst;
	int dstStride;
	int dstBpp;
	int dstXoff, dstYoff;

	BITS *bits, *bitsBase;
	FbStride bitsStride;
	FbBits xor = fb_gc(gc)->xor;
	FbBits and = fb_gc(gc)->and;

	int e, e1, e3, len;
	int stepmajor, stepminor;
	int octant;
	bool capNotLast = gc->capStyle == CapNotLast;

	fbGetDrawable(drawable, dst, dstStride, dstBpp, dstXoff, dstYoff);
	bitsStride = dstStride * (sizeof(FbBits) / sizeof(BITS));
	bitsBase =
		((BITS *) dst) + (yoff + dstYoff) * bitsStride + (xoff + dstXoff);

	do {
		INT32 ul = coordToInt(clip->x1 - xoff, clip->y1 - yoff);
		INT32 lr = coordToInt(clip->x2 - xoff - 1, clip->y2 - yoff - 1);
		uint64_t *pt = (uint64_t *)seg_0;
		int n = n_0;

		while (n--) {
			union {
				int32_t pt32[2];
				uint64_t pt64;
			} u;

			u.pt64 = *pt++;
			if (isClipped(u.pt32[0], ul, lr) | isClipped(u.pt32[1], ul, lr)) {
				int dashoffset = 0;
				fbSegment1(drawable, gc, clip,
					  intToX(u.pt32[0]) + xoff, intToY(u.pt32[0]) + yoff,
					  intToX(u.pt32[1]) + xoff, intToY(u.pt32[1]) + yoff,
					  !capNotLast, &dashoffset);
			} else {
				CalcLineDeltas(intToX(u.pt32[0]), intToY(u.pt32[0]),
					       intToX(u.pt32[1]), intToY(u.pt32[1]),
					       len, e1, stepmajor, stepminor, 1, bitsStride,
					       octant);
				if (e1 == 0 && len > 3) {
					int x1, x2;
					FbBits *dstLine;
					int dstX, width;
					FbBits startmask, endmask;
					int nmiddle;

					if (stepmajor < 0) {
						x1 = intToX(u.pt32[1]);
						x2 = intToX(u.pt32[0]) + 1;
						if (capNotLast)
							x1++;
					} else {
						x1 = intToX(u.pt32[0]);
						x2 = intToX(u.pt32[1]);
						if (!capNotLast)
							x2++;
					}
					dstX = (x1 + xoff + dstXoff) * (sizeof(BITS) * 8);
					width = (x2 - x1) * (sizeof(BITS) * 8);

					dstLine = dst + (intToY(u.pt32[0]) + yoff + dstYoff) * dstStride;
					dstLine += dstX >> FB_SHIFT;
					dstX &= FB_MASK;
					FbMaskBits(dstX, width, startmask, nmiddle, endmask);
					if (startmask) {
						WRITE(dstLine,
						      FbDoMaskRRop(READ(dstLine), and, xor,
								   startmask));
						dstLine++;
					}
					if (!and)
						while (nmiddle--)
							WRITE(dstLine++, xor);
					else
						while (nmiddle--) {
							WRITE(dstLine,
							      FbDoRRop(READ(dstLine), and, xor));
							dstLine++;
						}
					if (endmask)
						WRITE(dstLine,
						      FbDoMaskRRop(READ(dstLine), and, xor,
								   endmask));
				} else {
					bits = bitsBase + intToY(u.pt32[0]) * bitsStride + intToX(u.pt32[0]);
					if (len < e1) {
						e3 = len;
						len = e1;
						e1 = e3;

						e3 = stepminor;
						stepminor = stepmajor;
						stepmajor = e3;
						SetYMajorOctant(octant);
					}
					e = -len;
					e1 <<= 1;
					e3 = e << 1;
					FIXUP_ERROR(e, octant, bias);
					if (!capNotLast)
						len++;
					if (and == 0) {
						while (len--) {
							WRITE(bits, xor);
							bits += stepmajor;
							e += e1;
							if (e >= 0) {
								bits += stepminor;
								e += e3;
							}
						}
					} else {
						while (len--) {
							RROP(bits, and, xor);
							bits += stepmajor;
							e += e1;
							if (e >= 0) {
								bits += stepminor;
								e += e3;
							}
						}
					}
				}
			}
		}
	} while (++clip != last_clip);
}

#undef RROP
#undef isClipped
