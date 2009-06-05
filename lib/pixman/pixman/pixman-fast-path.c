/* -*- Mode: c; c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t; -*- */
/*
 * Copyright © 2000 SuSE, Inc.
 * Copyright © 2007 Red Hat, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of SuSE not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  SuSE makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * SuSE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL SuSE
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, SuSE, Inc.
 */

#include <config.h>
#include <string.h>
#include "pixman-private.h"
#include "pixman-combine32.h"
#define FbFullMask(n)   ((n) == 32 ? (uint32_t)-1 : ((((uint32_t) 1) << n) - 1))

#undef READ
#undef WRITE
#define READ(img,x) (*(x))
#define WRITE(img,ptr,v) ((*(ptr)) = (v))

static force_inline uint32_t
fbOver (uint32_t src, uint32_t dest)
{
    // dest = (dest * (255 - alpha)) / 255 + src
    uint32_t a = ~src >> 24; // 255 - alpha == 255 + (~alpha + 1) == ~alpha
    FbByteMulAdd(dest, a, src);

    return dest;
}

static uint32_t
fbOver24 (uint32_t x, uint32_t y)
{
    uint16_t  a = ~x >> 24;
    uint16_t  t;
    uint32_t  m,n,o;

    m = FbOverU(x,y,0,a,t);
    n = FbOverU(x,y,8,a,t);
    o = FbOverU(x,y,16,a,t);
    return m|n|o;
}

static uint32_t
fbIn (uint32_t x, uint8_t y)
{
    uint16_t  a = y;
    uint16_t  t;
    uint32_t  m,n,o,p;

    m = FbInU(x,0,a,t);
    n = FbInU(x,8,a,t);
    o = FbInU(x,16,a,t);
    p = FbInU(x,24,a,t);
    return m|n|o|p;
}

/*
 * Naming convention:
 *
 *  opSRCxMASKxDST
 */

static void
fbCompositeOver_x888x8x8888 (pixman_implementation_t *imp,
			     pixman_op_t      op,
			     pixman_image_t * pSrc,
			     pixman_image_t * pMask,
			     pixman_image_t * pDst,
			     int32_t      xSrc,
			     int32_t      ySrc,
			     int32_t      xMask,
			     int32_t      yMask,
			     int32_t      xDst,
			     int32_t      yDst,
			     int32_t     width,
			     int32_t     height)
{
    uint32_t	*src, *srcLine;
    uint32_t    *dst, *dstLine;
    uint8_t	*mask, *maskLine;
    int		 srcStride, maskStride, dstStride;
    uint8_t m;
    uint32_t s, d;
    uint16_t w;

    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint8_t, maskStride, maskLine, 1);
    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);

    while (height--)
    {
	src = srcLine;
	srcLine += srcStride;
	dst = dstLine;
	dstLine += dstStride;
	mask = maskLine;
	maskLine += maskStride;

	w = width;
	while (w--)
	{
	    m = READ(pMask, mask++);
	    if (m)
	    {
		s = READ(pSrc, src) | 0xff000000;

		if (m == 0xff)
		    WRITE(pDst, dst, s);
		else
		{
		    d = fbIn (s, m);
		    WRITE(pDst, dst, fbOver (d, READ(pDst, dst)));
		}
	    }
	    src++;
	    dst++;
	}
    }
}

static void
fbCompositeSolidMaskIn_nx8x8 (pixman_implementation_t *imp,
			      pixman_op_t      op,
			      pixman_image_t    *iSrc,
			      pixman_image_t    *iMask,
			      pixman_image_t    *iDst,
			      int32_t      xSrc,
			      int32_t      ySrc,
			      int32_t      xMask,
			      int32_t      yMask,
			      int32_t      xDst,
			      int32_t      yDst,
			      int32_t     width,
			      int32_t     height)
{
    uint32_t	src, srca;
    uint8_t	*dstLine, *dst, dstMask;
    uint8_t	*maskLine, *mask, m;
    int	dstStride, maskStride;
    uint16_t	w;
    uint16_t    t;

    fbComposeGetSolid(iSrc, src, iDst->bits.format);

    dstMask = FbFullMask (PIXMAN_FORMAT_DEPTH (iDst->bits.format));
    srca = src >> 24;

    fbComposeGetStart (iDst, xDst, yDst, uint8_t, dstStride, dstLine, 1);
    fbComposeGetStart (iMask, xMask, yMask, uint8_t, maskStride, maskLine, 1);

    if (srca == 0xff) {
	while (height--)
	{
	    dst = dstLine;
	    dstLine += dstStride;
	    mask = maskLine;
	    maskLine += maskStride;
	    w = width;

	    while (w--)
	    {
		m = *mask++;
		if (m == 0)
		{
		    *dst = 0;
		}
		else if (m != 0xff)
		{
		    *dst = FbIntMult(m, *dst, t);
		}
		dst++;
	    }
	}
    }
    else
    {
	while (height--)
	{
	    dst = dstLine;
	    dstLine += dstStride;
	    mask = maskLine;
	    maskLine += maskStride;
	    w = width;

	    while (w--)
	    {
		m = *mask++;
		m = FbIntMult(m, srca, t);
		if (m == 0)
		{
		    *dst = 0;
		}
		else if (m != 0xff)
		{
		    *dst = FbIntMult(m, *dst, t);
		}
		dst++;
	    }
	}
    }
}


static void
fbCompositeSrcIn_8x8 (pixman_implementation_t *imp,
		      pixman_op_t      op,
		      pixman_image_t  *iSrc,
		      pixman_image_t  *iMask,
		      pixman_image_t  *iDst,
		      int32_t          xSrc,
		      int32_t          ySrc,
		      int32_t          xMask,
		      int32_t          yMask,
		      int32_t          xDst,
		      int32_t          yDst,
		      int32_t         width,
		      int32_t         height)
{
    uint8_t	*dstLine, *dst;
    uint8_t	*srcLine, *src;
    int	dstStride, srcStride;
    uint16_t	w;
    uint8_t	s;
    uint16_t	t;

    fbComposeGetStart (iSrc, xSrc, ySrc, uint8_t, srcStride, srcLine, 1);
    fbComposeGetStart (iDst, xDst, yDst, uint8_t, dstStride, dstLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	while (w--)
	{
	    s = *src++;
	    if (s == 0)
	    {
		*dst = 0;
	    }
	    else if (s != 0xff)
	    {
		*dst = FbIntMult(s, *dst, t);
	    }
	    dst++;
	}
    }
}

static void
fbCompositeSolidMask_nx8x8888 (pixman_implementation_t *imp,
			       pixman_op_t      op,
			       pixman_image_t * pSrc,
			       pixman_image_t * pMask,
			       pixman_image_t * pDst,
			       int32_t      xSrc,
			       int32_t      ySrc,
			       int32_t      xMask,
			       int32_t      yMask,
			       int32_t      xDst,
			       int32_t      yDst,
			       int32_t     width,
			       int32_t     height)
{
    uint32_t	 src, srca;
    uint32_t	*dstLine, *dst, d, dstMask;
    uint8_t	*maskLine, *mask, m;
    int		 dstStride, maskStride;
    uint16_t	 w;

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    dstMask = FbFullMask (PIXMAN_FORMAT_DEPTH (pDst->bits.format));
    srca = src >> 24;
    if (src == 0)
	return;

    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint8_t, maskStride, maskLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	mask = maskLine;
	maskLine += maskStride;
	w = width;

	while (w--)
	{
	    m = READ(pMask, mask++);
	    if (m == 0xff)
	    {
		if (srca == 0xff)
		    WRITE(pDst, dst, src & dstMask);
		else
		    WRITE(pDst, dst, fbOver (src, READ(pDst, dst)) & dstMask);
	    }
	    else if (m)
	    {
		d = fbIn (src, m);
		WRITE(pDst, dst, fbOver (d, READ(pDst, dst)) & dstMask);
	    }
	    dst++;
	}
    }
}

static void
fbCompositeSolidMask_nx8888x8888C (pixman_implementation_t *imp,
				   pixman_op_t op,
				   pixman_image_t * pSrc,
				   pixman_image_t * pMask,
				   pixman_image_t * pDst,
				   int32_t      xSrc,
				   int32_t      ySrc,
				   int32_t      xMask,
				   int32_t      yMask,
				   int32_t      xDst,
				   int32_t      yDst,
				   int32_t     width,
				   int32_t     height)
{
    uint32_t	src, srca;
    uint32_t	*dstLine, *dst, d, dstMask;
    uint32_t	*maskLine, *mask, ma;
    int	dstStride, maskStride;
    uint16_t	w;
    uint32_t	m, n, o, p;

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    dstMask = FbFullMask (PIXMAN_FORMAT_DEPTH (pDst->bits.format));
    srca = src >> 24;
    if (src == 0)
	return;

    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint32_t, maskStride, maskLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	mask = maskLine;
	maskLine += maskStride;
	w = width;

	while (w--)
	{
	    ma = READ(pMask, mask++);
	    if (ma == 0xffffffff)
	    {
		if (srca == 0xff)
		    WRITE(pDst, dst, src & dstMask);
		else
		    WRITE(pDst, dst, fbOver (src, READ(pDst, dst)) & dstMask);
	    }
	    else if (ma)
	    {
		d = READ(pDst, dst);
#define FbInOverC(src,srca,msk,dst,i,result) { \
    uint16_t  __a = FbGet8(msk,i); \
    uint32_t  __t, __ta; \
    uint32_t  __i; \
    __t = FbIntMult (FbGet8(src,i), __a,__i); \
    __ta = (uint8_t) ~FbIntMult (srca, __a,__i); \
    __t = __t + FbIntMult(FbGet8(dst,i),__ta,__i); \
    __t = (uint32_t) (uint8_t) (__t | (-(__t >> 8))); \
    result = __t << (i); \
}
		FbInOverC (src, srca, ma, d, 0, m);
		FbInOverC (src, srca, ma, d, 8, n);
		FbInOverC (src, srca, ma, d, 16, o);
		FbInOverC (src, srca, ma, d, 24, p);
		WRITE(pDst, dst, m|n|o|p);
	    }
	    dst++;
	}
    }
}

static void
fbCompositeSolidMask_nx8x0888 (pixman_implementation_t *imp,
			       pixman_op_t op,
			       pixman_image_t * pSrc,
			       pixman_image_t * pMask,
			       pixman_image_t * pDst,
			       int32_t      xSrc,
			       int32_t      ySrc,
			       int32_t      xMask,
			       int32_t      yMask,
			       int32_t      xDst,
			       int32_t      yDst,
			       int32_t     width,
			       int32_t     height)
{
    uint32_t	src, srca;
    uint8_t	*dstLine, *dst;
    uint32_t	d;
    uint8_t	*maskLine, *mask, m;
    int	dstStride, maskStride;
    uint16_t	w;

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    srca = src >> 24;
    if (src == 0)
	return;

    fbComposeGetStart (pDst, xDst, yDst, uint8_t, dstStride, dstLine, 3);
    fbComposeGetStart (pMask, xMask, yMask, uint8_t, maskStride, maskLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	mask = maskLine;
	maskLine += maskStride;
	w = width;

	while (w--)
	{
	    m = READ(pMask, mask++);
	    if (m == 0xff)
	    {
		if (srca == 0xff)
		    d = src;
		else
		{
		    d = Fetch24(pDst, dst);
		    d = fbOver24 (src, d);
		}
		Store24(pDst, dst,d);
	    }
	    else if (m)
	    {
		d = fbOver24 (fbIn(src,m), Fetch24(pDst, dst));
		Store24(pDst, dst, d);
	    }
	    dst += 3;
	}
    }
}

static void
fbCompositeSolidMask_nx8x0565 (pixman_implementation_t *imp,
			       pixman_op_t op,
				  pixman_image_t * pSrc,
				  pixman_image_t * pMask,
				  pixman_image_t * pDst,
				  int32_t      xSrc,
				  int32_t      ySrc,
				  int32_t      xMask,
				  int32_t      yMask,
				  int32_t      xDst,
				  int32_t      yDst,
				  int32_t     width,
				  int32_t     height)
{
    uint32_t	src, srca;
    uint16_t	*dstLine, *dst;
    uint32_t	d;
    uint8_t	*maskLine, *mask, m;
    int	dstStride, maskStride;
    uint16_t	w;

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    srca = src >> 24;
    if (src == 0)
	return;

    fbComposeGetStart (pDst, xDst, yDst, uint16_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint8_t, maskStride, maskLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	mask = maskLine;
	maskLine += maskStride;
	w = width;

	while (w--)
	{
	    m = READ(pMask, mask++);
	    if (m == 0xff)
	    {
		if (srca == 0xff)
		    d = src;
		else
		{
		    d = READ(pDst, dst);
		    d = fbOver24 (src, cvt0565to0888(d));
		}
		WRITE(pDst, dst, cvt8888to0565(d));
	    }
	    else if (m)
	    {
		d = READ(pDst, dst);
		d = fbOver24 (fbIn(src,m), cvt0565to0888(d));
		WRITE(pDst, dst, cvt8888to0565(d));
	    }
	    dst++;
	}
    }
}

static void
fbCompositeSolidMask_nx8888x0565C (pixman_implementation_t *imp,
				   pixman_op_t op,
				   pixman_image_t * pSrc,
				   pixman_image_t * pMask,
				   pixman_image_t * pDst,
				   int32_t      xSrc,
				   int32_t      ySrc,
				   int32_t      xMask,
				   int32_t      yMask,
				   int32_t      xDst,
				   int32_t      yDst,
				   int32_t     width,
				   int32_t     height)
{
    uint32_t	src, srca;
    uint16_t	src16;
    uint16_t	*dstLine, *dst;
    uint32_t	d;
    uint32_t	*maskLine, *mask, ma;
    int	dstStride, maskStride;
    uint16_t	w;
    uint32_t	m, n, o;

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    srca = src >> 24;
    if (src == 0)
	return;

    src16 = cvt8888to0565(src);

    fbComposeGetStart (pDst, xDst, yDst, uint16_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint32_t, maskStride, maskLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	mask = maskLine;
	maskLine += maskStride;
	w = width;

	while (w--)
	{
	    ma = READ(pMask, mask++);
	    if (ma == 0xffffffff)
	    {
		if (srca == 0xff)
		{
		    WRITE(pDst, dst, src16);
		}
		else
		{
		    d = READ(pDst, dst);
		    d = fbOver24 (src, cvt0565to0888(d));
		    WRITE(pDst, dst, cvt8888to0565(d));
		}
	    }
	    else if (ma)
	    {
		d = READ(pDst, dst);
		d = cvt0565to0888(d);
		FbInOverC (src, srca, ma, d, 0, m);
		FbInOverC (src, srca, ma, d, 8, n);
		FbInOverC (src, srca, ma, d, 16, o);
		d = m|n|o;
		WRITE(pDst, dst, cvt8888to0565(d));
	    }
	    dst++;
	}
    }
}

static void
fbCompositeSrc_8888x8888 (pixman_implementation_t *imp,
			  pixman_op_t op,
			 pixman_image_t * pSrc,
			 pixman_image_t * pMask,
			 pixman_image_t * pDst,
			 int32_t      xSrc,
			 int32_t      ySrc,
			 int32_t      xMask,
			 int32_t      yMask,
			 int32_t      xDst,
			 int32_t      yDst,
			 int32_t     width,
			 int32_t     height)
{
    uint32_t	*dstLine, *dst, dstMask;
    uint32_t	*srcLine, *src, s;
    int	dstStride, srcStride;
    uint8_t	a;
    uint16_t	w;

    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);
    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);

    dstMask = FbFullMask (PIXMAN_FORMAT_DEPTH (pDst->bits.format));

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	while (w--)
	{
	    s = READ(pSrc, src++);
	    a = s >> 24;
	    if (a == 0xff)
		WRITE(pDst, dst, s & dstMask);
	    else if (s)
		WRITE(pDst, dst, fbOver (s, READ(pDst, dst)) & dstMask);
	    dst++;
	}
    }
}

static void
fbCompositeSrc_8888x0888 (pixman_implementation_t *imp,
			  pixman_op_t op,
			 pixman_image_t * pSrc,
			 pixman_image_t * pMask,
			 pixman_image_t * pDst,
			 int32_t      xSrc,
			 int32_t      ySrc,
			 int32_t      xMask,
			 int32_t      yMask,
			 int32_t      xDst,
			 int32_t      yDst,
			 int32_t     width,
			 int32_t     height)
{
    uint8_t	*dstLine, *dst;
    uint32_t	d;
    uint32_t	*srcLine, *src, s;
    uint8_t	a;
    int	dstStride, srcStride;
    uint16_t	w;

    fbComposeGetStart (pDst, xDst, yDst, uint8_t, dstStride, dstLine, 3);
    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	while (w--)
	{
	    s = READ(pSrc, src++);
	    a = s >> 24;
	    if (a)
	    {
		if (a == 0xff)
		    d = s;
		else
		    d = fbOver24 (s, Fetch24(pDst, dst));
		Store24(pDst, dst, d);
	    }
	    dst += 3;
	}
    }
}

static void
fbCompositeSrc_8888x0565 (pixman_implementation_t *imp,
			  pixman_op_t op,
			 pixman_image_t * pSrc,
			 pixman_image_t * pMask,
			 pixman_image_t * pDst,
			 int32_t      xSrc,
			 int32_t      ySrc,
			 int32_t      xMask,
			 int32_t      yMask,
			 int32_t      xDst,
			 int32_t      yDst,
			 int32_t     width,
			 int32_t     height)
{
    uint16_t	*dstLine, *dst;
    uint32_t	d;
    uint32_t	*srcLine, *src, s;
    uint8_t	a;
    int	dstStride, srcStride;
    uint16_t	w;

    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);
    fbComposeGetStart (pDst, xDst, yDst, uint16_t, dstStride, dstLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	while (w--)
	{
	    s = READ(pSrc, src++);
	    a = s >> 24;
	    if (s)
	    {
		if (a == 0xff)
		    d = s;
		else
		{
		    d = READ(pDst, dst);
		    d = fbOver24 (s, cvt0565to0888(d));
		}
		WRITE(pDst, dst, cvt8888to0565(d));
	    }
	    dst++;
	}
    }
}

static void
fbCompositeSrc_x888x0565 (pixman_implementation_t *imp,
			  pixman_op_t op,
                          pixman_image_t * pSrc,
                          pixman_image_t * pMask,
                          pixman_image_t * pDst,
                          int32_t      xSrc,
                          int32_t      ySrc,
                          int32_t      xMask,
                          int32_t      yMask,
                          int32_t      xDst,
                          int32_t      yDst,
                          int32_t     width,
                          int32_t     height)
{
    uint16_t	*dstLine, *dst;
    uint32_t	*srcLine, *src, s;
    int	dstStride, srcStride;
    uint16_t	w;

    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);
    fbComposeGetStart (pDst, xDst, yDst, uint16_t, dstStride, dstLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	while (w--)
	{
	    s = READ(pSrc, src++);
	    WRITE(pDst, dst, cvt8888to0565(s));
	    dst++;
	}
    }
}

static void
fbCompositeSrcAdd_8000x8000 (pixman_implementation_t *imp,
			     pixman_op_t	op,
			     pixman_image_t * pSrc,
			     pixman_image_t * pMask,
			     pixman_image_t * pDst,
			     int32_t      xSrc,
			     int32_t      ySrc,
			     int32_t      xMask,
			     int32_t      yMask,
			     int32_t      xDst,
			     int32_t      yDst,
			     int32_t     width,
			     int32_t     height)
{
    uint8_t	*dstLine, *dst;
    uint8_t	*srcLine, *src;
    int	dstStride, srcStride;
    uint16_t	w;
    uint8_t	s, d;
    uint16_t	t;

    fbComposeGetStart (pSrc, xSrc, ySrc, uint8_t, srcStride, srcLine, 1);
    fbComposeGetStart (pDst, xDst, yDst, uint8_t, dstStride, dstLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	while (w--)
	{
	    s = READ(pSrc, src++);
	    if (s)
	    {
		if (s != 0xff)
		{
		    d = READ(pDst, dst);
		    t = d + s;
		    s = t | (0 - (t >> 8));
		}
		WRITE(pDst, dst, s);
	    }
	    dst++;
	}
    }
}

static void
fbCompositeSrcAdd_8888x8888 (pixman_implementation_t *imp,
			     pixman_op_t	op,
			     pixman_image_t * pSrc,
			     pixman_image_t * pMask,
			     pixman_image_t * pDst,
			     int32_t      xSrc,
			     int32_t      ySrc,
			     int32_t      xMask,
			     int32_t      yMask,
			     int32_t      xDst,
			     int32_t      yDst,
			     int32_t     width,
			     int32_t     height)
{
    uint32_t	*dstLine, *dst;
    uint32_t	*srcLine, *src;
    int	dstStride, srcStride;
    uint16_t	w;
    uint32_t	s, d;
    uint16_t	t;
    uint32_t	m,n,o,p;

    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, srcLine, 1);
    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dstLine, 1);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	src = srcLine;
	srcLine += srcStride;
	w = width;

	while (w--)
	{
	    s = READ(pSrc, src++);
	    if (s)
	    {
		if (s != 0xffffffff)
		{
		    d = READ(pDst, dst);
		    if (d)
		    {
			m = FbAdd(s,d,0,t);
			n = FbAdd(s,d,8,t);
			o = FbAdd(s,d,16,t);
			p = FbAdd(s,d,24,t);
			s = m|n|o|p;
		    }
		}
		WRITE(pDst, dst, s);
	    }
	    dst++;
	}
    }
}

static void
fbCompositeSrcAdd_8888x8x8 (pixman_implementation_t *imp,
			    pixman_op_t op,
			    pixman_image_t * pSrc,
			    pixman_image_t * pMask,
			    pixman_image_t * pDst,
			    int32_t      xSrc,
			    int32_t      ySrc,
			    int32_t      xMask,
			    int32_t      yMask,
			    int32_t      xDst,
			    int32_t      yDst,
			    int32_t     width,
			    int32_t     height)
{
    uint8_t	*dstLine, *dst;
    uint8_t	*maskLine, *mask;
    int	dstStride, maskStride;
    uint16_t	w;
    uint32_t	src;
    uint8_t	sa;

    fbComposeGetStart (pDst, xDst, yDst, uint8_t, dstStride, dstLine, 1);
    fbComposeGetStart (pMask, xMask, yMask, uint8_t, maskStride, maskLine, 1);
    fbComposeGetSolid (pSrc, src, pDst->bits.format);
    sa = (src >> 24);

    while (height--)
    {
	dst = dstLine;
	dstLine += dstStride;
	mask = maskLine;
	maskLine += maskStride;
	w = width;

	while (w--)
	{
	    uint16_t	tmp;
	    uint16_t	a;
	    uint32_t	m, d;
	    uint32_t	r;

	    a = READ(pMask, mask++);
	    d = READ(pDst, dst);

	    m = FbInU (sa, 0, a, tmp);
	    r = FbAdd (m, d, 0, tmp);

	    WRITE(pDst, dst++, r);
	}
    }
}

/*
 * Simple bitblt
 */

static void
fbCompositeSolidFill (pixman_implementation_t *imp,
		      pixman_op_t op,
		      pixman_image_t * pSrc,
		      pixman_image_t * pMask,
		      pixman_image_t * pDst,
		      int32_t      xSrc,
		      int32_t      ySrc,
		      int32_t      xMask,
		      int32_t      yMask,
		      int32_t      xDst,
		      int32_t      yDst,
		      int32_t     width,
		      int32_t     height)
{
    uint32_t	src;

    fbComposeGetSolid(pSrc, src, pDst->bits.format);

    if (pDst->bits.format == PIXMAN_a8)
	src = src >> 24;
    else if (pDst->bits.format == PIXMAN_r5g6b5 ||
	     pDst->bits.format == PIXMAN_b5g6r5)
	src = cvt8888to0565 (src);

    pixman_fill (pDst->bits.bits, pDst->bits.rowstride,
		 PIXMAN_FORMAT_BPP (pDst->bits.format),
		 xDst, yDst,
		 width, height,
		 src);
}

static void
fbCompositeSrc_8888xx888 (pixman_implementation_t *imp,
			  pixman_op_t op,
			  pixman_image_t * pSrc,
			  pixman_image_t * pMask,
			  pixman_image_t * pDst,
			  int32_t      xSrc,
			  int32_t      ySrc,
			  int32_t      xMask,
			  int32_t      yMask,
			  int32_t      xDst,
			  int32_t      yDst,
			  int32_t     width,
			  int32_t     height)
{
    uint32_t	*dst;
    uint32_t    *src;
    int		 dstStride, srcStride;
    uint32_t	 n_bytes = width * sizeof (uint32_t);

    fbComposeGetStart (pSrc, xSrc, ySrc, uint32_t, srcStride, src, 1);
    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dst, 1);

    while (height--)
    {
	memcpy (dst, src, n_bytes);

	dst += dstStride;
	src += srcStride;
    }
}

static const FastPathInfo c_fast_paths[] =
{
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_r5g6b5,   fbCompositeSolidMask_nx8x0565, 0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_b5g6r5,   fbCompositeSolidMask_nx8x0565, 0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_r8g8b8,   fbCompositeSolidMask_nx8x0888, 0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_b8g8r8,   fbCompositeSolidMask_nx8x0888, 0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_a8r8g8b8, fbCompositeSolidMask_nx8x8888, 0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_x8r8g8b8, fbCompositeSolidMask_nx8x8888, 0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_a8b8g8r8, fbCompositeSolidMask_nx8x8888, 0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8,       PIXMAN_x8b8g8r8, fbCompositeSolidMask_nx8x8888, 0 },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8r8g8b8, PIXMAN_a8r8g8b8, fbCompositeSolidMask_nx8888x8888C, NEED_COMPONENT_ALPHA },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8r8g8b8, PIXMAN_x8r8g8b8, fbCompositeSolidMask_nx8888x8888C, NEED_COMPONENT_ALPHA },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8r8g8b8, PIXMAN_r5g6b5,   fbCompositeSolidMask_nx8888x0565C, NEED_COMPONENT_ALPHA },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8b8g8r8, PIXMAN_a8b8g8r8, fbCompositeSolidMask_nx8888x8888C, NEED_COMPONENT_ALPHA },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8b8g8r8, PIXMAN_x8b8g8r8, fbCompositeSolidMask_nx8888x8888C, NEED_COMPONENT_ALPHA },
    { PIXMAN_OP_OVER, PIXMAN_solid,    PIXMAN_a8b8g8r8, PIXMAN_b5g6r5,   fbCompositeSolidMask_nx8888x0565C, NEED_COMPONENT_ALPHA },
    { PIXMAN_OP_OVER, PIXMAN_x8r8g8b8, PIXMAN_a8,	PIXMAN_x8r8g8b8, fbCompositeOver_x888x8x8888,       0 },
    { PIXMAN_OP_OVER, PIXMAN_x8r8g8b8, PIXMAN_a8,	PIXMAN_a8r8g8b8, fbCompositeOver_x888x8x8888,       0 },
    { PIXMAN_OP_OVER, PIXMAN_x8b8g8r8, PIXMAN_a8,	PIXMAN_x8b8g8r8, fbCompositeOver_x888x8x8888,       0 },
    { PIXMAN_OP_OVER, PIXMAN_x8b8g8r8, PIXMAN_a8,	PIXMAN_a8b8g8r8, fbCompositeOver_x888x8x8888,       0 },
    { PIXMAN_OP_OVER, PIXMAN_a8r8g8b8, PIXMAN_null,     PIXMAN_a8r8g8b8, fbCompositeSrc_8888x8888,	   0 },
    { PIXMAN_OP_OVER, PIXMAN_a8r8g8b8, PIXMAN_null,	PIXMAN_x8r8g8b8, fbCompositeSrc_8888x8888,	   0 },
    { PIXMAN_OP_OVER, PIXMAN_a8r8g8b8, PIXMAN_null,	PIXMAN_r5g6b5,	 fbCompositeSrc_8888x0565,	   0 },
    { PIXMAN_OP_OVER, PIXMAN_a8b8g8r8, PIXMAN_null,	PIXMAN_a8b8g8r8, fbCompositeSrc_8888x8888,	   0 },
    { PIXMAN_OP_OVER, PIXMAN_a8b8g8r8, PIXMAN_null,	PIXMAN_x8b8g8r8, fbCompositeSrc_8888x8888,	   0 },
    { PIXMAN_OP_OVER, PIXMAN_a8b8g8r8, PIXMAN_null,     PIXMAN_b5g6r5,   fbCompositeSrc_8888x0565,	   0 },
    { PIXMAN_OP_ADD, PIXMAN_a8r8g8b8,  PIXMAN_null,	PIXMAN_a8r8g8b8, fbCompositeSrcAdd_8888x8888,   0 },
    { PIXMAN_OP_ADD, PIXMAN_a8b8g8r8,  PIXMAN_null,	PIXMAN_a8b8g8r8, fbCompositeSrcAdd_8888x8888,   0 },
    { PIXMAN_OP_ADD, PIXMAN_a8,        PIXMAN_null,     PIXMAN_a8,       fbCompositeSrcAdd_8000x8000,   0 },
    { PIXMAN_OP_ADD, PIXMAN_solid,     PIXMAN_a8,       PIXMAN_a8,       fbCompositeSrcAdd_8888x8x8,    0 },
    { PIXMAN_OP_SRC, PIXMAN_solid,     PIXMAN_null,     PIXMAN_a8r8g8b8, fbCompositeSolidFill, 0 },
    { PIXMAN_OP_SRC, PIXMAN_solid,     PIXMAN_null,     PIXMAN_x8r8g8b8, fbCompositeSolidFill, 0 },
    { PIXMAN_OP_SRC, PIXMAN_solid,     PIXMAN_null,     PIXMAN_a8b8g8r8, fbCompositeSolidFill, 0 },
    { PIXMAN_OP_SRC, PIXMAN_solid,     PIXMAN_null,     PIXMAN_x8b8g8r8, fbCompositeSolidFill, 0 },
    { PIXMAN_OP_SRC, PIXMAN_solid,     PIXMAN_null,     PIXMAN_a8,       fbCompositeSolidFill, 0 },
    { PIXMAN_OP_SRC, PIXMAN_solid,     PIXMAN_null,     PIXMAN_r5g6b5,   fbCompositeSolidFill, 0 },
    { PIXMAN_OP_SRC, PIXMAN_a8r8g8b8,  PIXMAN_null,     PIXMAN_x8r8g8b8, fbCompositeSrc_8888xx888, 0 },
    { PIXMAN_OP_SRC, PIXMAN_x8r8g8b8,  PIXMAN_null,     PIXMAN_x8r8g8b8, fbCompositeSrc_8888xx888, 0 },
    { PIXMAN_OP_SRC, PIXMAN_a8b8g8r8,  PIXMAN_null,     PIXMAN_x8b8g8r8, fbCompositeSrc_8888xx888, 0 },
    { PIXMAN_OP_SRC, PIXMAN_x8b8g8r8,  PIXMAN_null,     PIXMAN_x8b8g8r8, fbCompositeSrc_8888xx888, 0 },
    { PIXMAN_OP_SRC, PIXMAN_a8r8g8b8,  PIXMAN_null,     PIXMAN_r5g6b5,   fbCompositeSrc_x888x0565, 0 },
    { PIXMAN_OP_SRC, PIXMAN_x8r8g8b8,  PIXMAN_null,     PIXMAN_r5g6b5,   fbCompositeSrc_x888x0565, 0 },
    { PIXMAN_OP_SRC, PIXMAN_a8b8g8r8,  PIXMAN_null,     PIXMAN_b5g6r5,   fbCompositeSrc_x888x0565, 0 },
    { PIXMAN_OP_SRC, PIXMAN_x8b8g8r8,  PIXMAN_null,     PIXMAN_b5g6r5,   fbCompositeSrc_x888x0565, 0 },
    { PIXMAN_OP_IN,  PIXMAN_a8,        PIXMAN_null,     PIXMAN_a8,       fbCompositeSrcIn_8x8,   0 },
    { PIXMAN_OP_IN,  PIXMAN_solid,     PIXMAN_a8,	PIXMAN_a8,	 fbCompositeSolidMaskIn_nx8x8, 0 },
    { PIXMAN_OP_NONE },
};

static void
fbCompositeSrcScaleNearest (pixman_implementation_t *imp,
			    pixman_op_t     op,
			    pixman_image_t *pSrc,
			    pixman_image_t *pMask,
			    pixman_image_t *pDst,
			    int32_t         xSrc,
			    int32_t         ySrc,
			    int32_t         xMask,
			    int32_t         yMask,
			    int32_t         xDst,
			    int32_t         yDst,
			    int32_t        width,
			    int32_t        height)
{
    uint32_t       *dst;
    uint32_t       *src;
    int             dstStride, srcStride;
    int             i, j;
    pixman_vector_t v;
    
    fbComposeGetStart (pDst, xDst, yDst, uint32_t, dstStride, dst, 1);
    /* pass in 0 instead of xSrc and ySrc because xSrc and ySrc need to be
     * transformed from destination space to source space */
    fbComposeGetStart (pSrc, 0, 0, uint32_t, srcStride, src, 1);
    
    /* reference point is the center of the pixel */
    v.vector[0] = pixman_int_to_fixed(xSrc) + pixman_fixed_1 / 2;
    v.vector[1] = pixman_int_to_fixed(ySrc) + pixman_fixed_1 / 2;
    v.vector[2] = pixman_fixed_1;
    
    if (!pixman_transform_point_3d (pSrc->common.transform, &v))
        return;
    
    /* Round down to closest integer, ensuring that 0.5 rounds to 0, not 1 */
    v.vector[0] -= pixman_fixed_e;
    v.vector[1] -= pixman_fixed_e;
    
    for (j = 0; j < height; j++) {
        pixman_fixed_t vx = v.vector[0];
        pixman_fixed_t vy = v.vector[1];
        for (i = 0; i < width; ++i) {
            pixman_bool_t inside_bounds;
            uint32_t result;
            int x, y;
            x = vx >> 16;
            y = vy >> 16;
	    
            /* apply the repeat function */
            switch (pSrc->common.repeat) {
	    case PIXMAN_REPEAT_NORMAL:
		x = MOD (x, pSrc->bits.width);
		y = MOD (y, pSrc->bits.height);
		inside_bounds = TRUE;
		break;
		
	    case PIXMAN_REPEAT_PAD:
		x = CLIP (x, 0, pSrc->bits.width-1);
		y = CLIP (y, 0, pSrc->bits.height-1);
		inside_bounds = TRUE;
		break;
		
	    case PIXMAN_REPEAT_REFLECT:
		x = MOD (x, pSrc->bits.width * 2);
		if (x >= pSrc->bits.width)
		    x = pSrc->bits.width * 2 - x - 1;
		y = MOD (y, pSrc->bits.height * 2);
		if (y >= pSrc->bits.height)
		    y = pSrc->bits.height * 2 - y - 1;
		inside_bounds = TRUE;
		break;
		
	    case PIXMAN_REPEAT_NONE:
	    default:
		inside_bounds = (x >= 0 && x < pSrc->bits.width && y >= 0 && y < pSrc->bits.height);
		break;
            }
	    
            if (inside_bounds) {
                //XXX: we should move this multiplication out of the loop
                result = READ(pSrc, src + y * srcStride + x);
            } else {
                result = 0;
            }
            WRITE(pDst, dst + i, result);
	    
            /* adjust the x location by a unit vector in the x direction:
             * this is equivalent to transforming x+1 of the destination point to source space */
            vx += pSrc->common.transform->matrix[0][0];
        }
        /* adjust the y location by a unit vector in the y direction
         * this is equivalent to transforming y+1 of the destination point to source space */
        v.vector[1] += pSrc->common.transform->matrix[1][1];
        dst += dstStride;
    }
}

static void
fast_path_composite (pixman_implementation_t *imp,
		     pixman_op_t     op,
		     pixman_image_t *src,
		     pixman_image_t *mask,
		     pixman_image_t *dest,
		     int32_t         src_x,
		     int32_t         src_y,
		     int32_t         mask_x,
		     int32_t         mask_y,
		     int32_t         dest_x,
		     int32_t         dest_y,
		     int32_t        width,
		     int32_t        height)
{
    if (src->type == BITS
        && src->common.transform
        && !mask
        && op == PIXMAN_OP_SRC
        && !src->common.alpha_map && !dest->common.alpha_map
        && (src->common.filter == PIXMAN_FILTER_NEAREST)
        && PIXMAN_FORMAT_BPP(dest->bits.format) == 32
        && src->bits.format == dest->bits.format
        && src->common.src_clip == &(src->common.full_region)
        && !src->common.read_func && !src->common.write_func
        && !dest->common.read_func && !dest->common.write_func)
    {
        /* ensure that the transform matrix only has a scale */
        if (src->common.transform->matrix[0][1] == 0 &&
            src->common.transform->matrix[1][0] == 0 &&
            src->common.transform->matrix[2][0] == 0 &&
            src->common.transform->matrix[2][1] == 0 &&
            src->common.transform->matrix[2][2] == pixman_fixed_1)
	{
	    _pixman_walk_composite_region (imp, op,
					   src, mask, dest,
					   src_x, src_y,
					   mask_x, mask_y,
					   dest_x, dest_y,
					   width, height,
					   FALSE, FALSE, 
					   fbCompositeSrcScaleNearest);
	    return;
	}
    }

    if (_pixman_run_fast_path (c_fast_paths, imp,
			       op, src, mask, dest,
			       src_x, src_y,
			       mask_x, mask_y,
			       dest_x, dest_y,
			       width, height))
    {
	return;
    }

    _pixman_implementation_composite (imp->delegate, op,
				      src, mask, dest,
				      src_x, src_y,
				      mask_x, mask_y,
				      dest_x, dest_y,
				      width, height);
}

static void
pixman_fill8 (uint32_t  *bits,
	      int	stride,
	      int	x,
	      int	y,
	      int	width,
	      int	height,
	      uint32_t  xor)
{
    int byte_stride = stride * (int) sizeof (uint32_t);
    uint8_t *dst = (uint8_t *) bits;
    uint8_t v = xor & 0xff;
    int i;

    dst = dst + y * byte_stride + x;

    while (height--)
    {
	for (i = 0; i < width; ++i)
	    dst[i] = v;

	dst += byte_stride;
    }
}

static void
pixman_fill16 (uint32_t *bits,
	       int       stride,
	       int       x,
	       int       y,
	       int       width,
	       int       height,
	       uint32_t  xor)
{
    int short_stride = (stride * (int) sizeof (uint32_t)) / (int) sizeof (uint16_t);
    uint16_t *dst = (uint16_t *)bits;
    uint16_t v = xor & 0xffff;
    int i;

    dst = dst + y * short_stride + x;

    while (height--)
    {
	for (i = 0; i < width; ++i)
	    dst[i] = v;

	dst += short_stride;
    }
}

static void
pixman_fill32 (uint32_t *bits,
	       int       stride,
	       int       x,
	       int       y,
	       int       width,
	       int       height,
	       uint32_t  xor)
{
    int i;

    bits = bits + y * stride + x;

    while (height--)
    {
	for (i = 0; i < width; ++i)
	    bits[i] = xor;

	bits += stride;
    }
}

static pixman_bool_t
fast_path_fill (pixman_implementation_t *imp,
		uint32_t *bits,
		int stride,
		int bpp,
		int x,
		int y,
		int width,
		int height,
		uint32_t xor)
{
    switch (bpp)
    {
    case 8:
	pixman_fill8 (bits, stride, x, y, width, height, xor);
	break;
	
    case 16:
	pixman_fill16 (bits, stride, x, y, width, height, xor);
	break;
	
    case 32:
	pixman_fill32 (bits, stride, x, y, width, height, xor);
	break;
	
    default:
	return _pixman_implementation_fill (
	    imp->delegate, bits, stride, bpp, x, y, width, height, xor);
	break;
    }
    
    return TRUE;
}

pixman_implementation_t *
_pixman_implementation_create_fast_path (pixman_implementation_t *toplevel)
{
    pixman_implementation_t *general = _pixman_implementation_create_general (NULL);
    pixman_implementation_t *imp = _pixman_implementation_create (toplevel, general);

    imp->composite = fast_path_composite;
    imp->fill = fast_path_fill;
    
    return imp;
}
