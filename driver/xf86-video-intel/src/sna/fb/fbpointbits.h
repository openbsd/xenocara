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

#define RROP(b,a,x) WRITE((b), FbDoRRop (READ(b), (a), (x)))
#define isClipped(c,ul,lr)  (((c) | ((c) - (ul)) | ((lr) - (c))) & 0x80008000)

static void
DOTS(FbBits * dst,
     FbStride dstStride,
     int dstBpp,
     RegionPtr region,
     xPoint * ptsOrig,
     int npt, int xorg, int yorg, int xoff, int yoff, FbBits and, FbBits xor)
{
	uint32_t *pts = (uint32_t *) ptsOrig;
	BITS *bits = (BITS *) dst;
	BITS bxor = (BITS) xor;
	BITS band = (BITS) and;
	FbStride bitsStride = dstStride * (sizeof(FbBits) / sizeof(BITS));

	if (region->data == NULL) {
		INT32 ul = coordToInt(region->extents.x1 - xorg,
				      region->extents.y1 - yorg);
		INT32 lr = coordToInt(region->extents.x2 - xorg - 1,
				      region->extents.y2 - yorg - 1);

		bits += bitsStride * (yorg + yoff) + (xorg + xoff);
		if (and == 0) {
			while (npt >= 2) {
				union {
					uint32_t pt32[2];
					uint64_t pt64;
				} pt;
				pt.pt64 = *(uint64_t *)pts;
				if (!isClipped(pt.pt32[0], ul, lr)) {
					BITS *point = bits + intToY(pt.pt32[0]) * bitsStride + intToX(pt.pt32[0]);
					WRITE(point, bxor);
				}
				if (!isClipped(pt.pt32[1], ul, lr)) {
					BITS *point = bits + intToY(pt.pt32[1]) * bitsStride + intToX(pt.pt32[1]);
					WRITE(point, bxor);
				}

				pts += 2;
				npt -= 2;
			}
			if (npt) {
				uint32_t pt = *pts;
				if (!isClipped(pt, ul, lr)) {
					BITS *point = bits + intToY(pt) * bitsStride + intToX(pt);
					WRITE(point, bxor);
				}
			}
		} else {
			while (npt--) {
				uint32_t pt = *pts++;
				if (!isClipped(pt, ul, lr)) {
					BITS *point = bits + intToY(pt) * bitsStride + intToX(pt);
					RROP(point, band, bxor);
				}
			}
		}
	} else {
		bits += bitsStride * yoff + xoff;
		if (and == 0) {
			while (npt--) {
				uint32_t pt = *pts++;
				int x = intToX(pt) + xorg;
				int y = intToY(pt) + yorg;
				if (RegionContainsPoint(region, x, y, NULL)) {
					BITS *point = bits + y * bitsStride + x;
					WRITE(point, bxor);
				}
			}
		} else {
			while (npt--) {
				uint32_t pt = *pts++;
				int x = intToX(pt) + xorg;
				int y = intToY(pt) + yorg;
				if (RegionContainsPoint(region, x, y, NULL)) {
					BITS *point = bits + y * bitsStride + x;
					RROP(point, band, bxor);
				}
			}
		}
	}
}

static void
DOTS__SIMPLE(FbBits * dst,
	     FbStride dstStride,
	     int dstBpp,
	     RegionPtr region,
	     xPoint * ptsOrig,
	     int npt, int xorg, int yorg, int xoff, int yoff,
	     FbBits and, FbBits xor)
{
	uint32_t *pts = (uint32_t *) ptsOrig;
	BITS *bits = (BITS *) dst, *p;
	BITS bxor = (BITS) xor;
	FbStride bitsStride = dstStride * (sizeof(FbBits) / sizeof(BITS));

	bits += bitsStride * (yorg + yoff) + (xorg + xoff);
	while (npt >= 2) {
		union {
			uint32_t pt32[2];
			uint64_t pt64;
		} pt;
		pt.pt64 = *(uint64_t *)pts;

		p = bits + intToY(pt.pt32[0]) * bitsStride + intToX(pt.pt32[0]);
		WRITE(p, bxor);

		p = bits + intToY(pt.pt32[1]) * bitsStride + intToX(pt.pt32[1]);
		WRITE(p, bxor);

		pts += 2;
		npt -= 2;
	}
	if (npt) {
		uint32_t pt = *pts;
		p = bits + intToY(pt) * bitsStride + intToX(pt);
		WRITE(p, bxor);
	}
}

#undef RROP
#undef isClipped
