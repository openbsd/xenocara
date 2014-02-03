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

#include <string.h>
#include "fb.h"

typedef struct _mergeRopBits {
    FbBits ca1, cx1, ca2, cx2;
} FbMergeRopRec, *FbMergeRopPtr;

#define O 0
#define I FB_ALLONES

static const FbMergeRopRec FbMergeRopBits[16] = {
	{O, O, O, O},               /* clear         0x0         0 */
	{I, O, O, O},               /* and           0x1         src AND dst */
	{I, O, I, O},               /* andReverse    0x2         src AND NOT dst */
	{O, O, I, O},               /* copy          0x3         src */
	{I, I, O, O},               /* andInverted   0x4         NOT src AND dst */
	{O, I, O, O},               /* noop          0x5         dst */
	{O, I, I, O},               /* xor           0x6         src XOR dst */
	{I, I, I, O},               /* or            0x7         src OR dst */
	{I, I, I, I},               /* nor           0x8         NOT src AND NOT dst */
	{O, I, I, I},               /* equiv         0x9         NOT src XOR dst */
	{O, I, O, I},               /* invert        0xa         NOT dst */
	{I, I, O, I},               /* orReverse     0xb         src OR NOT dst */
	{O, O, I, I},               /* copyInverted  0xc         NOT src */
	{I, O, I, I},               /* orInverted    0xd         NOT src OR dst */
	{I, O, O, I},               /* nand          0xe         NOT src OR NOT dst */
	{O, O, O, I},               /* set           0xf         1 */
};

#undef O
#undef I

#define FbDeclareMergeRop() FbBits   _ca1, _cx1, _ca2, _cx2;
#define FbDeclarePrebuiltMergeRop()	FbBits	_cca, _ccx;

#define FbInitializeMergeRop(alu,pm) {\
    const FbMergeRopRec  *_bits; \
    _bits = &FbMergeRopBits[alu]; \
    _ca1 = _bits->ca1 &  pm; \
    _cx1 = _bits->cx1 | ~pm; \
    _ca2 = _bits->ca2 &  pm; \
    _cx2 = _bits->cx2 &  pm; \
}

#define InitializeShifts(sx,dx,ls,rs) { \
    if (sx != dx) { \
	if (sx > dx) { \
	    ls = sx - dx; \
	    rs = FB_UNIT - ls; \
	} else { \
	    rs = dx - sx; \
	    ls = FB_UNIT - rs; \
	} \
    } \
}

static void
fbBlt__rop(FbBits *srcLine, FbStride srcStride, int srcX,
	   FbBits *dstLine, FbStride dstStride, int dstX,
	   int width, int height,
	   int alu, FbBits pm, int bpp,
	   Bool reverse, Bool upsidedown)
{
	FbBits *src, *dst;
	int leftShift, rightShift;
	FbBits startmask, endmask;
	FbBits bits, bits1;
	int n, nmiddle;
	Bool destInvarient;
	int startbyte, endbyte;

	FbDeclareMergeRop();

	FbInitializeMergeRop(alu, pm);
	destInvarient = FbDestInvarientMergeRop();
	if (upsidedown) {
		srcLine += (height - 1) * (srcStride);
		dstLine += (height - 1) * (dstStride);
		srcStride = -srcStride;
		dstStride = -dstStride;
	}
	FbMaskBitsBytes(dstX, width, destInvarient, startmask, startbyte,
			nmiddle, endmask, endbyte);
	if (reverse) {
		srcLine += ((srcX + width - 1) >> FB_SHIFT) + 1;
		dstLine += ((dstX + width - 1) >> FB_SHIFT) + 1;
		srcX = (srcX + width - 1) & FB_MASK;
		dstX = (dstX + width - 1) & FB_MASK;
	} else {
		srcLine += srcX >> FB_SHIFT;
		dstLine += dstX >> FB_SHIFT;
		srcX &= FB_MASK;
		dstX &= FB_MASK;
	}
	if (srcX == dstX) {
		while (height--) {
			src = srcLine;
			srcLine += srcStride;
			dst = dstLine;
			dstLine += dstStride;
			if (reverse) {
				if (endmask) {
					bits = READ(--src);
					--dst;
					FbDoRightMaskByteMergeRop(dst, bits, endbyte, endmask);
				}
				n = nmiddle;
				if (destInvarient) {
					while (n--)
						WRITE(--dst, FbDoDestInvarientMergeRop(READ(--src)));
				} else {
					while (n--) {
						bits = READ(--src);
						--dst;
						WRITE(dst, FbDoMergeRop(bits, READ(dst)));
					}
				}
				if (startmask) {
					bits = READ(--src);
					--dst;
					FbDoLeftMaskByteMergeRop(dst, bits, startbyte, startmask);
				}
			} else {
				if (startmask) {
					bits = READ(src++);
					FbDoLeftMaskByteMergeRop(dst, bits, startbyte, startmask);
					dst++;
				}
				n = nmiddle;
				if (destInvarient) {
					while (n--)
						WRITE(dst++, FbDoDestInvarientMergeRop(READ(src++)));
				} else {
					while (n--) {
						bits = READ(src++);
						WRITE(dst, FbDoMergeRop(bits, READ(dst)));
						dst++;
					}
				}
				if (endmask) {
					bits = READ(src);
					FbDoRightMaskByteMergeRop(dst, bits, endbyte, endmask);
				}
			}
		}
	} else {
		if (srcX > dstX) {
			leftShift = srcX - dstX;
			rightShift = FB_UNIT - leftShift;
		} else {
			rightShift = dstX - srcX;
			leftShift = FB_UNIT - rightShift;
		}
		while (height--) {
			src = srcLine;
			srcLine += srcStride;
			dst = dstLine;
			dstLine += dstStride;

			bits1 = 0;
			if (reverse) {
				if (srcX < dstX)
					bits1 = READ(--src);
				if (endmask) {
					bits = FbScrRight(bits1, rightShift);
					if (FbScrRight(endmask, leftShift)) {
						bits1 = READ(--src);
						bits |= FbScrLeft(bits1, leftShift);
					}
					--dst;
					FbDoRightMaskByteMergeRop(dst, bits, endbyte, endmask);
				}
				n = nmiddle;
				if (destInvarient) {
					while (n--) {
						bits = FbScrRight(bits1, rightShift);
						bits1 = READ(--src);
						bits |= FbScrLeft(bits1, leftShift);
						--dst;
						WRITE(dst, FbDoDestInvarientMergeRop(bits));
					}
				} else {
					while (n--) {
						bits = FbScrRight(bits1, rightShift);
						bits1 = READ(--src);
						bits |= FbScrLeft(bits1, leftShift);
						--dst;
						WRITE(dst, FbDoMergeRop(bits, READ(dst)));
					}
				}
				if (startmask) {
					bits = FbScrRight(bits1, rightShift);
					if (FbScrRight(startmask, leftShift)) {
						bits1 = READ(--src);
						bits |= FbScrLeft(bits1, leftShift);
					}
					--dst;
					FbDoLeftMaskByteMergeRop(dst, bits, startbyte, startmask);
				}
			} else {
				if (srcX > dstX)
					bits1 = READ(src++);
				if (startmask) {
					bits = FbScrLeft(bits1, leftShift);
					if (FbScrLeft(startmask, rightShift)) {
						bits1 = READ(src++);
						bits |= FbScrRight(bits1, rightShift);
					}
					FbDoLeftMaskByteMergeRop(dst, bits, startbyte, startmask);
					dst++;
				}
				n = nmiddle;
				if (destInvarient) {
					while (n--) {
						bits = FbScrLeft(bits1, leftShift);
						bits1 = READ(src++);
						bits |= FbScrRight(bits1, rightShift);
						WRITE(dst, FbDoDestInvarientMergeRop(bits));
						dst++;
					}
				} else {
					while (n--) {
						bits = FbScrLeft(bits1, leftShift);
						bits1 = READ(src++);
						bits |= FbScrRight(bits1, rightShift);
						WRITE(dst, FbDoMergeRop(bits, READ(dst)));
						dst++;
					}
				}
				if (endmask) {
					bits = FbScrLeft(bits1, leftShift);
					if (FbScrLeft(endmask, rightShift)) {
						bits1 = READ(src);
						bits |= FbScrRight(bits1, rightShift);
					}
					FbDoRightMaskByteMergeRop(dst, bits, endbyte, endmask);
				}
			}
		}
	}
}

void
fbBlt(FbBits *srcLine, FbStride srcStride, int srcX,
      FbBits *dstLine, FbStride dstStride, int dstX,
      int width, int height,
      int alu, FbBits pm, int bpp,
      Bool reverse, Bool upsidedown)
{
	DBG(("%s %dx%d, alu=%d, pm=%x, bpp=%d (reverse=%d, upsidedown=%d)\n",
	     __FUNCTION__, width, height, alu, pm, bpp, reverse, upsidedown));

	if (alu == GXcopy && pm == FB_ALLONES && ((srcX|dstX|width) & 7) == 0) {
		CARD8 *s = (CARD8 *) srcLine;
		CARD8 *d = (CARD8 *) dstLine;
		void *(*func)(void *, const void *, size_t);
		int i;

		srcStride *= sizeof(FbBits);
		dstStride *= sizeof(FbBits);
		width >>= 3;
		s += srcX >> 3;
		d += dstX >> 3;

		DBG(("%s fast blt, src_stride=%d, dst_stride=%d, width=%d (offset=%ld)\n",
		     __FUNCTION__,
		     srcStride, dstStride, width, (long)(s - d)));

		if (width == srcStride && width == dstStride) {
			width *= height;
			height = 1;
		}

		if ((s < d && s + width > d) || (d < s && d + width > s))
			func = memmove;
		else
			func = memcpy;
		if (!upsidedown) {
			for (i = 0; i < height; i++)
				func(d + i * dstStride,
				     s + i * srcStride,
				     width);
		} else {
			for (i = height; i--; )
				func(d + i * dstStride,
				     s + i * srcStride,
				     width);
		}

		return;
	}

	fbBlt__rop(srcLine, srcStride, srcX,
		   dstLine, dstStride, dstX,
		   width, height,
		   alu, pm, bpp,
		   reverse, upsidedown);
}
