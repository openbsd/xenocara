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

#define WRITE_ADDR1(n)	    (n)
#define WRITE_ADDR2(n)	    (n)
#define WRITE_ADDR4(n)	    (n)

#define WRITE1(d,n,fg)	    WRITE(d + WRITE_ADDR1(n), (BITS) (fg))

#ifdef BITS2
#define WRITE2(d,n,fg)	    WRITE((BITS2 *) &((d)[WRITE_ADDR2(n)]), (BITS2) (fg))
#else
#define WRITE2(d,n,fg)	    (WRITE1(d,n,fg), WRITE1(d,(n)+1,fg))
#endif

#ifdef BITS4
#define WRITE4(d,n,fg)	    WRITE((BITS4 *) &((d)[WRITE_ADDR4(n)]), (BITS4) (fg))
#else
#define WRITE4(d,n,fg)	    (WRITE2(d,n,fg), WRITE2(d,(n)+2,fg))
#endif

static void
GLYPH(FbBits * dstBits,
      FbStride dstStride,
      int dstBpp, FbStip * stipple, FbBits fg, int x, int height)
{
	int lshift;
	FbStip bits;
	BITS *dstLine;
	BITS *dst;
	int n;
	int shift;

	dstLine = (BITS *) dstBits;
	dstLine += x & ~3;
	dstStride *= (sizeof(FbBits) / sizeof(BITS));
	shift = x & 3;
	lshift = 4 - shift;
	while (height--) {
		bits = *stipple++;
		dst = (BITS *) dstLine;
		n = lshift;
		while (bits) {
			switch (FbStipMoveLsb(FbLeftStipBits(bits, n), 4, n)) {
			case 0:
				break;
			case 1:
				WRITE1(dst, 0, fg);
				break;
			case 2:
				WRITE1(dst, 1, fg);
				break;
			case 3:
				WRITE2(dst, 0, fg);
				break;
			case 4:
				WRITE1(dst, 2, fg);
				break;
			case 5:
				WRITE1(dst, 0, fg);
				WRITE1(dst, 2, fg);
				break;
			case 6:
				WRITE1(dst, 1, fg);
				WRITE1(dst, 2, fg);
				break;
			case 7:
				WRITE2(dst, 0, fg);
				WRITE1(dst, 2, fg);
				break;
			case 8:
				WRITE1(dst, 3, fg);
				break;
			case 9:
				WRITE1(dst, 0, fg);
				WRITE1(dst, 3, fg);
				break;
			case 10:
				WRITE1(dst, 1, fg);
				WRITE1(dst, 3, fg);
				break;
			case 11:
				WRITE2(dst, 0, fg);
				WRITE1(dst, 3, fg);
				break;
			case 12:
				WRITE2(dst, 2, fg);
				break;
			case 13:
				WRITE1(dst, 0, fg);
				WRITE2(dst, 2, fg);
				break;
			case 14:
				WRITE1(dst, 1, fg);
				WRITE2(dst, 2, fg);
				break;
			case 15:
				WRITE4(dst, 0, fg);
				break;
			}
			bits = FbStipLeft(bits, n);
			n = 4;
			dst += 4;
		}
		dstLine += dstStride;
	}
}

#undef WRITE_ADDR1
#undef WRITE_ADDR2
#undef WRITE_ADDR4
#undef WRITE1
#undef WRITE2
#undef WRITE4

#undef RROP
#undef isClipped
