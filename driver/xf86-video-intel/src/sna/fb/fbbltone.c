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

#ifdef __clang__
/* shift overflow is intentional */
#pragma clang diagnostic ignored "-Wshift-overflow"
#endif

/*
 *  Example: srcX = 13 dstX = 8	(FB unit 32 dstBpp 8)
 *
 *	**** **** **** **** **** **** **** ****
 *			^
 *	********  ********  ********  ********
 *		  ^
 *  leftShift = 12
 *  rightShift = 20
 *
 *  Example: srcX = 0 dstX = 8 (FB unit 32 dstBpp 8)
 *
 *	**** **** **** **** **** **** **** ****
 *	^		
 *	********  ********  ********  ********
 *		  ^
 *
 *  leftShift = 24
 *  rightShift = 8
 */

#define LoadBits {\
    if (leftShift) { \
	bitsRight = (src < srcEnd ? READ(src++) : 0); \
	bits = (FbStipLeft (bitsLeft, leftShift) | \
		FbStipRight(bitsRight, rightShift)); \
	bitsLeft = bitsRight; \
    } else \
	bits = (src < srcEnd ? READ(src++) : 0); \
}

#define LaneCases1(n,a)	    case n: FbLaneCase(n,a); break
#define LaneCases2(n,a)	    LaneCases1(n,a); LaneCases1(n+1,a)
#define LaneCases4(n,a)	    LaneCases2(n,a); LaneCases2(n+2,a)
#define LaneCases8(n,a)	    LaneCases4(n,a); LaneCases4(n+4,a)
#define LaneCases16(n,a)    LaneCases8(n,a); LaneCases8(n+8,a)
#define LaneCases32(n,a)    LaneCases16(n,a); LaneCases16(n+16,a)
#define LaneCases64(n,a)    LaneCases32(n,a); LaneCases32(n+32,a)
#define LaneCases128(n,a)   LaneCases64(n,a); LaneCases64(n+64,a)
#define LaneCases256(n,a)   LaneCases128(n,a); LaneCases128(n+128,a)

#define LaneCases(a)	    LaneCases16(0,a)

static const CARD8 fb8Lane[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

static const CARD8 fb16Lane[16] = {
    0, 3, 12, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const CARD8 fb32Lane[16] = {
    0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const CARD8 * const fbLaneTable[33] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    fb8Lane, 0, 0, 0, 0, 0, 0, 0,
    fb16Lane, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    fb32Lane
};

void
fbBltOne(FbStip * src, FbStride srcStride,      /* FbStip units per scanline */
         int srcX,              /* bit position of source */
         FbBits * dst, FbStride dstStride,      /* FbBits units per scanline */
         int dstX,              /* bit position of dest */
         int dstBpp,            /* bits per destination unit */
         int width,             /* width in bits of destination */
         int height,            /* height in scanlines */
         FbBits fgand,          /* rrop values */
         FbBits fgxor, FbBits bgand, FbBits bgxor)
{
	const FbBits *fbBits;
	FbBits *srcEnd;
	int pixelsPerDst;           /* dst pixels per FbBits */
	int unitsPerSrc;            /* src patterns per FbStip */
	int leftShift, rightShift;  /* align source with dest */
	FbBits startmask, endmask;  /* dest scanline masks */
	FbStip bits = 0, bitsLeft, bitsRight;       /* source bits */
	FbStip left;
	FbBits mask;
	int nDst;                   /* dest longwords (w.o. end) */
	int w;
	int n, nmiddle;
	int dstS;                   /* stipple-relative dst X coordinate */
	Bool copy;                  /* accelerate dest-invariant */
	Bool transparent;           /* accelerate 0 nop */
	int srcinc;                 /* source units consumed */
	Bool endNeedsLoad = FALSE;  /* need load for endmask */
	const CARD8 *fbLane;
	int startbyte, endbyte;

	/*
	 * Do not read past the end of the buffer!
	 */
	srcEnd = src + height * srcStride;

	/*
	 * Number of destination units in FbBits == number of stipple pixels
	 * used each time
	 */
	pixelsPerDst = FB_UNIT / dstBpp;

	/*
	 * Number of source stipple patterns in FbStip 
	 */
	unitsPerSrc = FB_STIP_UNIT / pixelsPerDst;

	copy = FALSE;
	transparent = FALSE;
	if (bgand == 0 && fgand == 0)
		copy = TRUE;
	else if (bgand == FB_ALLONES && bgxor == 0)
		transparent = TRUE;

	/*
	 * Adjust source and dest to nearest FbBits boundary
	 */
	src += srcX >> FB_STIP_SHIFT;
	dst += dstX >> FB_SHIFT;
	srcX &= FB_STIP_MASK;
	dstX &= FB_MASK;

	FbMaskBitsBytes(dstX, width, copy,
			startmask, startbyte, nmiddle, endmask, endbyte);

	/*
	 * Compute effective dest alignment requirement for
	 * source -- must align source to dest unit boundary
	 */
	dstS = dstX / dstBpp;
	/*
	 * Compute shift constants for effective alignement
	 */
	if (srcX >= dstS) {
		leftShift = srcX - dstS;
		rightShift = FB_STIP_UNIT - leftShift;
	} else {
		rightShift = dstS - srcX;
		leftShift = FB_STIP_UNIT - rightShift;
	}
	/*
	 * Get pointer to stipple mask array for this depth
	 */
	fbBits = 0;                 /* unused */
	if (pixelsPerDst <= 8)
		fbBits = fbStippleTable[pixelsPerDst];
	fbLane = 0;
	if (transparent && fgand == 0 && dstBpp >= 8)
		fbLane = fbLaneTable[dstBpp];

	/*
	 * Compute total number of destination words written, but 
	 * don't count endmask 
	 */
	nDst = nmiddle;
	if (startmask)
		nDst++;

	dstStride -= nDst;

	/*
	 * Compute total number of source words consumed
	 */

	srcinc = (nDst + unitsPerSrc - 1) / unitsPerSrc;

	if (srcX > dstS)
		srcinc++;
	if (endmask) {
		endNeedsLoad = nDst % unitsPerSrc == 0;
		if (endNeedsLoad)
			srcinc++;
	}

	srcStride -= srcinc;

	/*
	 * Copy rectangle
	 */
	while (height--) {
		w = nDst;               /* total units across scanline */
		n = unitsPerSrc;        /* units avail in single stipple */
		if (n > w)
			n = w;

		bitsLeft = 0;
		if (srcX > dstS)
			bitsLeft = READ(src++);
		if (n) {
			/*
			 * Load first set of stipple bits
			 */
			LoadBits;

			/*
			 * Consume stipple bits for startmask
			 */
			if (startmask) {
				mask = fbBits[FbLeftStipBits(bits, pixelsPerDst)];
				if (fbLane) {
					fbTransparentSpan(dst, mask & startmask, fgxor, 1);
				} else {
					if (mask || !transparent)
						FbDoLeftMaskByteStippleRRop(dst, mask,
									    fgand, fgxor, bgand, bgxor,
									    startbyte, startmask);
				}
				bits = FbStipLeft(bits, pixelsPerDst);
				dst++;
				n--;
				w--;
			}
			/*
			 * Consume stipple bits across scanline
			 */
			for (;;) {
				w -= n;
				if (copy) {
					while (n--) {
#if FB_UNIT > 32
						if (pixelsPerDst == 16)
							mask = FbStipple16Bits(FbLeftStipBits(bits, 16));
						else
#endif
							mask = fbBits[FbLeftStipBits(bits, pixelsPerDst)];
						WRITE(dst, FbOpaqueStipple(mask, fgxor, bgxor));
						dst++;
						bits = FbStipLeft(bits, pixelsPerDst);
					}
				}
				else {
					if (fbLane) {
						while (bits && n) {
							switch (fbLane[FbLeftStipBits(bits, pixelsPerDst)]) {
								LaneCases((CARD8 *) dst);
							}
							bits = FbStipLeft(bits, pixelsPerDst);
							dst++;
							n--;
						}
						dst += n;
					} else {
						while (n--) {
							left = FbLeftStipBits(bits, pixelsPerDst);
							if (left || !transparent) {
								mask = fbBits[left];
								WRITE(dst, FbStippleRRop(READ(dst), mask,
											 fgand, fgxor, bgand,
											 bgxor));
							}
							dst++;
							bits = FbStipLeft(bits, pixelsPerDst);
						}
					}
				}
				if (!w)
					break;
				/*
				 * Load another set and reset number of available units
				 */
				LoadBits;
				n = unitsPerSrc;
				if (n > w)
					n = w;
			}
		}
		/*
		 * Consume stipple bits for endmask
		 */
		if (endmask) {
			if (endNeedsLoad) {
				LoadBits;
			}
			mask = fbBits[FbLeftStipBits(bits, pixelsPerDst)];
			if (fbLane) {
				fbTransparentSpan(dst, mask & endmask, fgxor, 1);
			} else {
				if (mask || !transparent)
					FbDoRightMaskByteStippleRRop(dst, mask,
								     fgand, fgxor, bgand, bgxor,
								     endbyte, endmask);
			}
		}
		dst += dstStride;
		src += srcStride;
	}
}

/*
 * Not very efficient, but simple -- copy a single plane
 * from an N bit image to a 1 bit image
 */

void
fbBltPlane(FbBits * src,
           FbStride srcStride,
           int srcX,
           int srcBpp,
           FbStip * dst,
           FbStride dstStride,
           int dstX,
           int width,
           int height,
           FbStip fgand,
           FbStip fgxor, FbStip bgand, FbStip bgxor, Pixel planeMask)
{
	FbBits *s;
	FbBits pm;
	FbBits srcMask;
	FbBits srcMaskFirst;
	FbBits srcMask0 = 0;
	FbBits srcBits;

	FbStip dstBits;
	FbStip *d;
	FbStip dstMask;
	FbStip dstMaskFirst;
	FbStip dstUnion;
	int w;
	int wt;

	if (!width)
		return;

	src += srcX >> FB_SHIFT;
	srcX &= FB_MASK;

	dst += dstX >> FB_STIP_SHIFT;
	dstX &= FB_STIP_MASK;

	w = width / srcBpp;

	pm = fbReplicatePixel(planeMask, srcBpp);
	srcMaskFirst = pm & FbBitsMask(srcX, srcBpp);
	srcMask0 = pm & FbBitsMask(0, srcBpp);

	dstMaskFirst = FbStipMask(dstX, 1);
	while (height--) {
		d = dst;
		dst += dstStride;
		s = src;
		src += srcStride;

		srcMask = srcMaskFirst;
		srcBits = READ(s++);

		dstMask = dstMaskFirst;
		dstUnion = 0;
		dstBits = 0;

		wt = w;

		while (wt--) {
			if (!srcMask) {
				srcBits = READ(s++);
				srcMask = srcMask0;
			}
			if (!dstMask) {
				WRITE(d, FbStippleRRopMask(READ(d), dstBits,
							   fgand, fgxor, bgand, bgxor,
							   dstUnion));
				d++;
				dstMask = FbStipMask(0, 1);
				dstUnion = 0;
				dstBits = 0;
			}
			if (srcBits & srcMask)
				dstBits |= dstMask;
			dstUnion |= dstMask;
			if (srcBpp == FB_UNIT)
				srcMask = 0;
			else
				srcMask = FbScrRight(srcMask, srcBpp);
			dstMask = FbStipRight(dstMask, 1);
		}
		if (dstUnion)
			WRITE(d, FbStippleRRopMask(READ(d), dstBits,
						   fgand, fgxor, bgand, bgxor, dstUnion));
	}
}
