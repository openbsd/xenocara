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

#ifndef FB_H
#define FB_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include <servermd.h>
#include <gcstruct.h>
#include <colormap.h>
#include <windowstr.h>

#include <stdbool.h>
#include <pixman.h>

#if HAS_DEBUG_FULL
#define DBG(x) ErrorF x
#else
#define DBG(x)
#endif

#include "sfb.h"

#define WRITE(ptr, val) (*(ptr) = (val))
#define READ(ptr) (*(ptr))

/*
 * This single define controls the basic size of data manipulated
 * by this software; it must be log2(sizeof (FbBits) * 8)
 */
#define FB_SHIFT    LOG2_BITMAP_PAD

#define FB_UNIT	    (1 << FB_SHIFT)
#define FB_HALFUNIT (1 << (FB_SHIFT-1))
#define FB_MASK	    (FB_UNIT - 1)
#define FB_ALLONES  ((FbBits) -1)

#if IMAGE_BYTE_ORDER != LSBFirst
#error "IMAGE_BYTE_ORDER must be LSBFirst"
#endif

#if GLYPHPADBYTES != 4
#error "GLYPHPADBYTES must be 4"
#endif

#if FB_SHIFT != 5
#error "FB_SHIFT ala LOG2_BITMAP_PAD must be 5"
#endif

#define FB_STIP_SHIFT	LOG2_BITMAP_PAD
#define FB_STIP_UNIT	(1 << FB_STIP_SHIFT)
#define FB_STIP_MASK	(FB_STIP_UNIT - 1)
#define FB_STIP_ALLONES	((FbStip) -1)
#define FbFullMask(n)   ((n) == FB_UNIT ? FB_ALLONES : ((((FbBits) 1) << n) - 1))

typedef uint32_t FbBits;
typedef FbBits FbStip;
typedef int FbStride;

#include "fbrop.h"

#define FbScrLeft(x,n)	((x) >> (n))
#define FbScrRight(x,n)	((x) << (n))
/* #define FbLeftBits(x,n)	((x) & ((((FbBits) 1) << (n)) - 1)) */
#define FbLeftStipBits(x,n) ((x) & ((((FbStip) 1) << (n)) - 1))
#define FbStipMoveLsb(x,s,n)	(FbStipRight (x,(s)-(n)))
#define FbPatternOffsetBits	0

#define FbStipLeft(x,n)	FbScrLeft(x,n)
#define FbStipRight(x,n) FbScrRight(x,n)

#define FbRotLeft(x,n)	FbScrLeft(x,n) | (n ? FbScrRight(x,FB_UNIT-n) : 0)
#define FbRotRight(x,n)	FbScrRight(x,n) | (n ? FbScrLeft(x,FB_UNIT-n) : 0)

#define FbRotStipLeft(x,n)  FbStipLeft(x,n) | (n ? FbStipRight(x,FB_STIP_UNIT-n) : 0)
#define FbRotStipRight(x,n)  FbStipRight(x,n) | (n ? FbStipLeft(x,FB_STIP_UNIT-n) : 0)

#define FbLeftMask(x)	    ( ((x) & FB_MASK) ? \
			     FbScrRight(FB_ALLONES,(x) & FB_MASK) : 0)
#define FbRightMask(x)	    ( ((FB_UNIT - (x)) & FB_MASK) ? \
			     FbScrLeft(FB_ALLONES,(FB_UNIT - (x)) & FB_MASK) : 0)

#define FbLeftStipMask(x)   ( ((x) & FB_STIP_MASK) ? \
			     FbStipRight(FB_STIP_ALLONES,(x) & FB_STIP_MASK) : 0)
#define FbRightStipMask(x)  ( ((FB_STIP_UNIT - (x)) & FB_STIP_MASK) ? \
			     FbScrLeft(FB_STIP_ALLONES,(FB_STIP_UNIT - (x)) & FB_STIP_MASK) : 0)

#define FbBitsMask(x,w)	(FbScrRight(FB_ALLONES,(x) & FB_MASK) & \
			 FbScrLeft(FB_ALLONES,(FB_UNIT - ((x) + (w))) & FB_MASK))

#define FbStipMask(x,w)	(FbStipRight(FB_STIP_ALLONES,(x) & FB_STIP_MASK) & \
			 FbStipLeft(FB_STIP_ALLONES,(FB_STIP_UNIT - ((x)+(w))) & FB_STIP_MASK))

#define FbMaskBits(x,w,l,n,r) { \
    n = (w); \
    r = FbRightMask((x)+n); \
    l = FbLeftMask(x); \
    if (l) { \
	n -= FB_UNIT - ((x) & FB_MASK); \
	if (n < 0) { \
	    n = 0; \
	    l &= r; \
	    r = 0; \
	} \
    } \
    n >>= FB_SHIFT; \
}

#define FbByteMaskInvalid   0x10

#define FbPatternOffset(o,t)  ((o) ^ (FbPatternOffsetBits & ~(sizeof (t) - 1)))

#define FbPtrOffset(p,o,t)		((t *) ((CARD8 *) (p) + (o)))
#define FbSelectPatternPart(xor,o,t)	((xor) >> (FbPatternOffset (o,t) << 3))
#define FbStorePart(dst,off,t,xor)	(WRITE(FbPtrOffset(dst,off,t), \
					 FbSelectPart(xor,off,t)))
#ifndef FbSelectPart
#define FbSelectPart(x,o,t) FbSelectPatternPart(x,o,t)
#endif

#define FbMaskBitsBytes(x,w,copy,l,lb,n,r,rb) { \
    n = (w); \
    lb = 0; \
    rb = 0; \
    r = FbRightMask((x)+n); \
    if (r) { \
	/* compute right byte length */ \
	if ((copy) && (((x) + n) & 7) == 0) { \
	    rb = (((x) + n) & FB_MASK) >> 3; \
	} else { \
	    rb = FbByteMaskInvalid; \
	} \
    } \
    l = FbLeftMask(x); \
    if (l) { \
	/* compute left byte length */ \
	if ((copy) && ((x) & 7) == 0) { \
	    lb = ((x) & FB_MASK) >> 3; \
	} else { \
	    lb = FbByteMaskInvalid; \
	} \
	/* subtract out the portion painted by leftMask */ \
	n -= FB_UNIT - ((x) & FB_MASK); \
	if (n < 0) { \
	    if (lb != FbByteMaskInvalid) { \
		if (rb == FbByteMaskInvalid) { \
		    lb = FbByteMaskInvalid; \
		} else if (rb) { \
		    lb |= (rb - lb) << (FB_SHIFT - 3); \
		    rb = 0; \
		} \
	    } \
	    n = 0; \
	    l &= r; \
	    r = 0; \
	}\
    } \
    n >>= FB_SHIFT; \
}

#define FbDoLeftMaskByteRRop(dst,lb,l,and,xor) { \
    switch (lb) { \
    case (sizeof (FbBits) - 3) | (1 << (FB_SHIFT - 3)): \
	FbStorePart(dst,sizeof (FbBits) - 3,CARD8,xor); \
	break; \
    case (sizeof (FbBits) - 3) | (2 << (FB_SHIFT - 3)): \
	FbStorePart(dst,sizeof (FbBits) - 3,CARD8,xor); \
	FbStorePart(dst,sizeof (FbBits) - 2,CARD8,xor); \
	break; \
    case (sizeof (FbBits) - 2) | (1 << (FB_SHIFT - 3)): \
	FbStorePart(dst,sizeof (FbBits) - 2,CARD8,xor); \
	break; \
    case sizeof (FbBits) - 3: \
	FbStorePart(dst,sizeof (FbBits) - 3,CARD8,xor); \
    case sizeof (FbBits) - 2: \
	FbStorePart(dst,sizeof (FbBits) - 2,CARD16,xor); \
	break; \
    case sizeof (FbBits) - 1: \
	FbStorePart(dst,sizeof (FbBits) - 1,CARD8,xor); \
	break; \
    default: \
	WRITE(dst, FbDoMaskRRop(READ(dst), and, xor, l)); \
	break; \
    } \
}

#define FbDoRightMaskByteRRop(dst,rb,r,and,xor) { \
    switch (rb) { \
    case 1: \
	FbStorePart(dst,0,CARD8,xor); \
	break; \
    case 2: \
	FbStorePart(dst,0,CARD16,xor); \
	break; \
    case 3: \
	FbStorePart(dst,0,CARD16,xor); \
	FbStorePart(dst,2,CARD8,xor); \
	break; \
    default: \
	WRITE(dst, FbDoMaskRRop (READ(dst), and, xor, r)); \
    } \
}

#define FbMaskStip(x,w,l,n,r) { \
    n = (w); \
    r = FbRightStipMask((x)+n); \
    l = FbLeftStipMask(x); \
    if (l) { \
	n -= FB_STIP_UNIT - ((x) & FB_STIP_MASK); \
	if (n < 0) { \
	    n = 0; \
	    l &= r; \
	    r = 0; \
	} \
    } \
    n >>= FB_STIP_SHIFT; \
}

/*
 * These macros are used to transparently stipple
 * in copy mode; the expected usage is with 'n' constant
 * so all of the conditional parts collapse into a minimal
 * sequence of partial word writes
 *
 * 'n' is the bytemask of which bytes to store, 'a' is the address
 * of the FbBits base unit, 'o' is the offset within that unit
 *
 * The term "lane" comes from the hardware term "byte-lane" which
 */

#define FbLaneCase1(n,a,o)						\
    if ((n) == 0x01) {							\
	WRITE((CARD8 *) ((a)+FbPatternOffset(o,CARD8)), fgxor);		\
    }

#define FbLaneCase2(n,a,o)						\
    if ((n) == 0x03) {							\
	WRITE((CARD16 *) ((a)+FbPatternOffset(o,CARD16)), fgxor);	\
    } else {								\
	FbLaneCase1((n)&1,a,o)						\
	FbLaneCase1((n)>>1,a,(o)+1)					\
    }

#define FbLaneCase4(n,a,o)						\
    if ((n) == 0x0f) {							\
	WRITE((CARD32 *) ((a)+FbPatternOffset(o,CARD32)), fgxor);	\
    } else {								\
	FbLaneCase2((n)&3,a,o)						\
	FbLaneCase2((n)>>2,a,(o)+2)					\
    }

#define FbLaneCase(n,a)   FbLaneCase4(n,(CARD8 *) (a),0)

typedef struct {
	long changes;
	long serial;
	GCFuncs *old_funcs;
	void *priv;

	FbBits and, xor;            /* reduced rop values */
	FbBits bgand, bgxor;        /* for stipples */
	FbBits fg, bg, pm;          /* expanded and filled */
	unsigned int dashLength;    /* total of all dash elements */
	unsigned char evenStipple;  /* stipple is even */
	unsigned char bpp;          /* current drawable bpp */
} FbGCPrivate, *FbGCPrivPtr;

extern DevPrivateKeyRec sna_gc_key;
extern DevPrivateKeyRec sna_window_key;

static inline FbGCPrivate *fb_gc(GCPtr gc)
{
	return dixGetPrivateAddr(&gc->devPrivates, &sna_gc_key);
}

static inline PixmapPtr fbGetWindowPixmap(WindowPtr window)
{
	return *(PixmapPtr *)dixGetPrivateAddr(&window->devPrivates, &sna_window_key);
}

#ifdef ROOTLESS
#define __fbPixDrawableX(p)	((p)->drawable.x)
#define __fbPixDrawableY(p)	((p)->drawable.y)
#else
#define __fbPixDrawableX(p)	0
#define __fbPixDrawableY(p)	0
#endif

#ifdef COMPOSITE
#define __fbPixOffXWin(p)	(__fbPixDrawableX(p) - (p)->screen_x)
#define __fbPixOffYWin(p)	(__fbPixDrawableY(p) - (p)->screen_y)
#else
#define __fbPixOffXWin(p)	(__fbPixDrawableX(p))
#define __fbPixOffYWin(p)	(__fbPixDrawableY(p))
#endif
#define __fbPixOffXPix(p)	(__fbPixDrawableX(p))
#define __fbPixOffYPix(p)	(__fbPixDrawableY(p))

#define fbGetDrawablePixmap(drawable, pixmap, xoff, yoff) {		\
    if ((drawable)->type != DRAWABLE_PIXMAP) {				\
	(pixmap) = fbGetWindowPixmap((WindowPtr)drawable);		\
	(xoff) = __fbPixOffXWin(pixmap);				\
	(yoff) = __fbPixOffYWin(pixmap);				\
    } else {								\
	(pixmap) = (PixmapPtr) (drawable);				\
	(xoff) = __fbPixOffXPix(pixmap);				\
	(yoff) = __fbPixOffYPix(pixmap);				\
    }									\
}

#define fbGetPixmapBitsData(pixmap, pointer, stride, bpp) {		\
    (pointer) = (FbBits *) (pixmap)->devPrivate.ptr;			\
    (stride) = ((int) (pixmap)->devKind) / sizeof (FbBits); (void)(stride);\
    (bpp) = (pixmap)->drawable.bitsPerPixel;  (void)(bpp);		\
}

#define fbGetPixmapStipData(pixmap, pointer, stride, bpp) {		\
    (pointer) = (FbStip *) (pixmap)->devPrivate.ptr;			\
    (stride) = ((int) (pixmap)->devKind) / sizeof (FbStip); (void)(stride);\
    (bpp) = (pixmap)->drawable.bitsPerPixel;  (void)(bpp);		\
}

#define fbGetDrawable(drawable, pointer, stride, bpp, xoff, yoff) {	\
    PixmapPtr   _pPix;							\
    fbGetDrawablePixmap(drawable, _pPix, xoff, yoff);			\
    fbGetPixmapBitsData(_pPix, pointer, stride, bpp);			\
}

#define fbGetStipDrawable(drawable, pointer, stride, bpp, xoff, yoff) {	\
    PixmapPtr   _pPix;							\
    fbGetDrawablePixmap(drawable, _pPix, xoff, yoff);			\
    fbGetPixmapStipData(_pPix, pointer, stride, bpp);			\
}

/*
 * XFree86 empties the root BorderClip when the VT is inactive,
 * here's a macro which uses that to disable GetImage and GetSpans
 */
#define fbWindowEnabled(pWin) \
    RegionNotEmpty(&(pWin)->drawable.pScreen->root->borderClip)
#define fbDrawableEnabled(drawable) \
    ((drawable)->type == DRAWABLE_PIXMAP ? \
     TRUE : fbWindowEnabled((WindowPtr) drawable))

#define FbPowerOfTwo(w)	    (((w) & ((w) - 1)) == 0)
/*
 * Accelerated tiles are power of 2 width <= FB_UNIT
 */
#define FbEvenTile(w)	    ((w) <= FB_UNIT && FbPowerOfTwo(w))
/*
 * Accelerated stipples are power of 2 width and <= FB_UNIT/dstBpp
 * with dstBpp a power of 2 as well
 */
#define FbEvenStip(w,bpp)   ((w) * (bpp) <= FB_UNIT && FbPowerOfTwo(w) && FbPowerOfTwo(bpp))

inline static int16_t fbBound(int16_t a, uint16_t b)
{
	int v = (int)a + (int)b;
	if (v > MAXSHORT)
		return MAXSHORT;
	return v;
}

extern void
fbPolyArc(DrawablePtr drawable, GCPtr gc, int narcs, xArc * parcs);

extern void
fbBlt(FbBits *src, FbStride srcStride, int srcX,
      FbBits *dst, FbStride dstStride, int dstX,
      int width, int height,
      int alu, FbBits pm, int bpp,
      Bool reverse, Bool upsidedown);

#if FB_STIP_SHIFT == FB_SHIFT
static inline void
fbBltStip(FbStip *src, FbStride srcStride, int srcX,
	  FbStip *dst, FbStride dstStride, int dstX,
	  int width, int height, int alu, FbBits pm, int bpp)
{
	fbBlt((FbBits *)src, srcStride, srcX,
	      (FbBits *)dst, dstStride, dstX,
	      width, height, alu, pm, bpp,
	      FALSE, FALSE);
}
#else
#error FB_STIP_SHIFT must equal FB_SHIFT
#endif

extern void
fbBltOne(FbStip *src, FbStride srcStride, int srcX,
         FbBits *dst, FbStride dstStride, int dstX,
         int dstBpp, int width, int height,
	 FbBits fgand, FbBits fbxor, FbBits bgand, FbBits bgxor);

extern void
fbBltPlane(FbBits *src, FbStride srcStride, int srcX, int srcBpp,
           FbStip *dst, FbStride dstStride, int dstX,
           int width, int height,
           FbStip fgand, FbStip fgxor, FbStip bgand, FbStip bgxor,
	   Pixel planeMask);

extern void
fbCopyNtoN(DrawablePtr src, DrawablePtr dst, GCPtr gc,
           BoxPtr pbox, int nbox,
           int dx, int dy,
           Bool reverse, Bool upsidedown, Pixel bitplane, void *closure);

extern void
fbCopy1toN(DrawablePtr src, DrawablePtr dst, GCPtr gc,
           BoxPtr pbox, int nbox,
           int dx, int dy,
           Bool reverse, Bool upsidedown, Pixel bitplane, void *closure);

extern void
fbCopyNto1(DrawablePtr src, DrawablePtr dst, GCPtr gc,
           BoxPtr pbox, int nbox,
           int dx, int dy,
           Bool reverse, Bool upsidedown, Pixel bitplane, void *closure);

extern RegionPtr
fbCopyArea(DrawablePtr src, DrawablePtr dst, GCPtr gc,
	   int sx, int sy,
	   int width, int height,
	   int dx, int dy);

extern RegionPtr
fbCopyPlane(DrawablePtr src, DrawablePtr dst, GCPtr gc,
	    int sx, int sy,
	    int width, int height,
	    int dx, int dy,
	    unsigned long bitplane);

extern void
fbFill(DrawablePtr drawable, GCPtr gc, int x, int y, int width, int height);

extern void
fbSolidBoxClipped(DrawablePtr drawable, GCPtr gc,
                  int x1, int y1, int x2, int y2);

extern void
fbPolyFillRect(DrawablePtr drawable, GCPtr gc, int n, xRectangle *rec);

extern void
fbFillSpans(DrawablePtr drawable, GCPtr gc,
            int n, DDXPointPtr pt, int *width, int fSorted);

extern void
fbPadPixmap(PixmapPtr pPixmap);

extern void
fbValidateGC(GCPtr gc, unsigned long changes, DrawablePtr drawable);

extern void
fbGetSpans(DrawablePtr drawable, int wMax,
           DDXPointPtr pt, int *width, int n, char *dst);

extern void
fbPolyGlyphBlt(DrawablePtr drawable, GCPtr gc, int x, int y,
               unsigned int n, CharInfoPtr *info, pointer glyphs);

extern void
fbImageGlyphBlt(DrawablePtr drawable, GCPtr gc, int x, int y,
                unsigned int n, CharInfoPtr *info, pointer glyphs);

extern void
fbPutImage(DrawablePtr drawable, GCPtr gc, int depth,
           int x, int y, int w, int h,
	   int leftPad, int format, char *image);

extern void
fbPutXYImage(DrawablePtr drawable, GCPtr gc,
             FbBits fg, FbBits bg, FbBits pm,
             int alu, Bool opaque,
             int x, int y, int width, int height,
	     FbStip * src, FbStride srcStride, int srcX);

extern void
fbGetImage(DrawablePtr drawable,
           int x, int y, int w, int h,
	   unsigned int format, unsigned long planeMask, char *d);

extern void
fbPolyLine(DrawablePtr drawable, GCPtr gc, int mode, int n, DDXPointPtr pt);

extern void
fbFixCoordModePrevious(int n, DDXPointPtr pt);

extern void
fbPolySegment(DrawablePtr drawable, GCPtr gc, int n, xSegment *seg);

extern RegionPtr
fbBitmapToRegion(PixmapPtr pixmap);

extern void
fbPolyPoint(DrawablePtr drawable, GCPtr gc,
	    int mode, int n, xPoint *pt,
	    unsigned flags);

extern void
fbPushImage(DrawablePtr drawable, GCPtr gc,
            FbStip *src, FbStride srcStride, int srcX,
	    int x, int y, int width, int height);

extern void
fbPushPixels(GCPtr gc, PixmapPtr pBitmap, DrawablePtr drawable,
	     int dx, int dy, int xOrg, int yOrg);

extern void
fbSetSpans(DrawablePtr drawable, GCPtr gc,
           char *src, DDXPointPtr pt, int *width, int n, int fSorted);

extern void
fbSegment(DrawablePtr drawable, GCPtr gc,
          int xa, int ya, int xb, int yb,
	  bool drawLast, int *dashOffset);

extern void
fbSegment1(DrawablePtr drawable, GCPtr gc, const BoxRec *clip,
          int xa, int ya, int xb, int yb,
	  bool drawLast, int *dashOffset);

extern void
fbTransparentSpan(FbBits * dst, FbBits stip, FbBits fgxor, int n);

extern void
fbStipple(FbBits *dst, FbStride dstStride, int dstX, int dstBpp,
          int width, int height,
          FbStip *stip, FbStride stipStride,
          int stipWidth, int stipHeight,
          Bool even,
          FbBits fgand, FbBits fgxor, FbBits bgand, FbBits bgxor,
	  int xRot, int yRot);

extern void
fbTile(FbBits *dst, FbStride dstStride, int dstX, int width, int height,
       FbBits *tile, FbStride tileStride, int tileWidth, int tileHeight,
       int alu, FbBits pm, int bpp,
       int xRot, int yRot);

extern FbBits fbReplicatePixel(Pixel p, int bpp);

#endif  /* FB_H */
