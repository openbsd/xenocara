/*
 * Copyright © 2001 Keith Packard
 *
 * Partly based on code that is Copyright © The XFree86 Project Inc.
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

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <stdlib.h>

#include "uxa-priv.h"

#ifdef RENDER
#include "mipict.h"

static void uxa_composite_fallback_pict_desc(PicturePtr pict, char *string, int n)
{
    char format[20];
    char size[20];
    char loc;

    if (!pict) {
	snprintf(string, n, "None");
	return;
    }

    switch (pict->format)
    {
    case PICT_a8r8g8b8:
	snprintf(format, 20, "ARGB8888");
	break;
    case PICT_x8r8g8b8:
	snprintf(format, 20, "XRGB8888");
	break;
    case PICT_r5g6b5:
	snprintf(format, 20, "RGB565  ");
	break;
    case PICT_x1r5g5b5:
	snprintf(format, 20, "RGB555  ");
	break;
    case PICT_a8:
	snprintf(format, 20, "A8      ");
	break;
    case PICT_a1:
	snprintf(format, 20, "A1      ");
	break;
    default:
	snprintf(format, 20, "0x%x", (int)pict->format);
	break;
    }

    loc = uxa_drawable_is_offscreen(pict->pDrawable) ? 's' : 'm';

    snprintf(size, 20, "%dx%d%s", pict->pDrawable->width,
	     pict->pDrawable->height, pict->repeat ?
	     " R" : "");

    snprintf(string, n, "%p:%c fmt %s (%s)", pict->pDrawable, loc, format, size);
}

static void
uxa_print_composite_fallback(CARD8 op,
			  PicturePtr pSrc,
			  PicturePtr pMask,
			  PicturePtr pDst)
{
    char sop[20];
    char srcdesc[40], maskdesc[40], dstdesc[40];

    switch(op)
    {
    case PictOpSrc:
	sprintf(sop, "Src");
	break;
    case PictOpOver:
	sprintf(sop, "Over");
	break;
    default:
	sprintf(sop, "0x%x", (int)op);
	break;
    }

    uxa_composite_fallback_pict_desc(pSrc, srcdesc, 40);
    uxa_composite_fallback_pict_desc(pMask, maskdesc, 40);
    uxa_composite_fallback_pict_desc(pDst, dstdesc, 40);

    ErrorF("Composite fallback: op %s, \n"
	   "                    src  %s, \n"
	   "                    mask %s, \n"
	   "                    dst  %s, \n",
	   sop, srcdesc, maskdesc, dstdesc);
}

Bool
uxa_op_reads_destination (CARD8 op)
{
    /* FALSE (does not read destination) is the list of ops in the protocol
     * document with "0" in the "Fb" column and no "Ab" in the "Fa" column.
     * That's just Clear and Src.  ReduceCompositeOp() will already have
     * converted con/disjoint clear/src to Clear or Src.
     */
    switch (op) {
    case PictOpClear:
    case PictOpSrc:
	return FALSE;
    default:
	return TRUE;
    }
}


static Bool
uxa_get_pixel_from_rgba(CARD32	*pixel,
		    CARD16	red,
		    CARD16	green,
		    CARD16	blue,
		    CARD16	alpha,
		    CARD32	format)
{
    int rbits, bbits, gbits, abits;
    int rshift, bshift, gshift, ashift;

    *pixel = 0;

    if (!PICT_FORMAT_COLOR(format))
	return FALSE;

    rbits = PICT_FORMAT_R(format);
    gbits = PICT_FORMAT_G(format);
    bbits = PICT_FORMAT_B(format);
    abits = PICT_FORMAT_A(format);

    if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ARGB) {
	bshift = 0;
	gshift = bbits;
	rshift = gshift + gbits;
	ashift = rshift + rbits;
    } else {  /* PICT_TYPE_ABGR */
	rshift = 0;
	gshift = rbits;
	bshift = gshift + gbits;
	ashift = bshift + bbits;
    }

    *pixel |=  ( blue >> (16 - bbits)) << bshift;
    *pixel |=  (  red >> (16 - rbits)) << rshift;
    *pixel |=  (green >> (16 - gbits)) << gshift;
    *pixel |=  (alpha >> (16 - abits)) << ashift;

    return TRUE;
}

static Bool
uxa_get_rgba_from_pixel(CARD32	pixel,
		    CARD16	*red,
		    CARD16	*green,
		    CARD16	*blue,
		    CARD16	*alpha,
		    CARD32	format)
{
    int rbits, bbits, gbits, abits;
    int rshift, bshift, gshift, ashift;

    if (!PICT_FORMAT_COLOR(format))
	return FALSE;

    rbits = PICT_FORMAT_R(format);
    gbits = PICT_FORMAT_G(format);
    bbits = PICT_FORMAT_B(format);
    abits = PICT_FORMAT_A(format);

    if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ARGB) {
	bshift = 0;
	gshift = bbits;
	rshift = gshift + gbits;
	ashift = rshift + rbits;
    } else {  /* PICT_TYPE_ABGR */
	rshift = 0;
	gshift = rbits;
	bshift = gshift + gbits;
	ashift = bshift + bbits;
    }

    *red = ((pixel >> rshift ) & ((1 << rbits) - 1)) << (16 - rbits);
    while (rbits < 16) {
	*red |= *red >> rbits;
	rbits <<= 1;
    }

    *green = ((pixel >> gshift ) & ((1 << gbits) - 1)) << (16 - gbits);
    while (gbits < 16) {
	*green |= *green >> gbits;
	gbits <<= 1;
    }

    *blue = ((pixel >> bshift ) & ((1 << bbits) - 1)) << (16 - bbits);
    while (bbits < 16) {
	*blue |= *blue >> bbits;
	bbits <<= 1;
    }

    if (abits) {
	*alpha = ((pixel >> ashift ) & ((1 << abits) - 1)) << (16 - abits);
	while (abits < 16) {
	    *alpha |= *alpha >> abits;
	    abits <<= 1;
	}
    } else
	*alpha = 0xffff;

    return TRUE;
}

static int
uxa_try_driver_solid_fill(PicturePtr	pSrc,
		      PicturePtr	pDst,
		      INT16		xSrc,
		      INT16		ySrc,
		      INT16		xDst,
		      INT16		yDst,
		      CARD16		width,
		      CARD16		height)
{
    uxa_screen_t    *uxa_screen = uxa_get_screen(pDst->pDrawable->pScreen);
    RegionRec region;
    BoxPtr pbox;
    int nbox;
    int dst_off_x, dst_off_y;
    PixmapPtr pSrcPix, pDstPix;
    CARD32 pixel;
    CARD16 red, green, blue, alpha;

    pDstPix = uxa_get_drawable_pixmap (pDst->pDrawable);
    pSrcPix = uxa_get_drawable_pixmap (pSrc->pDrawable);

    xDst += pDst->pDrawable->x;
    yDst += pDst->pDrawable->y;
    xSrc += pSrc->pDrawable->x;
    ySrc += pSrc->pDrawable->y;

    if (!miComputeCompositeRegion (&region, pSrc, NULL, pDst,
				   xSrc, ySrc, 0, 0, xDst, yDst,
				   width, height))
	return 1;

    uxa_get_drawable_deltas (pDst->pDrawable, pDstPix, &dst_off_x, &dst_off_y);

    REGION_TRANSLATE(pScreen, &region, dst_off_x, dst_off_y);

    pixel = uxa_get_pixmap_first_pixel (pSrcPix);

    if (!uxa_pixmap_is_offscreen(pDstPix)) {
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return 0;
    }

    if (!uxa_get_rgba_from_pixel(pixel, &red, &green, &blue, &alpha,
			 pSrc->format))
    {
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return -1;
    }

    if (!uxa_get_pixel_from_rgba(&pixel, red, green, blue, alpha,
			pDst->format))
    {
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return -1;
    }

    if (!(*uxa_screen->info->prepare_solid) (pDstPix, GXcopy, 0xffffffff, pixel))
    {
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return -1;
    }

    nbox = REGION_NUM_RECTS(&region);
    pbox = REGION_RECTS(&region);

    while (nbox--)
    {
	(*uxa_screen->info->solid) (pDstPix, pbox->x1, pbox->y1, pbox->x2, pbox->y2);
	pbox++;
    }

    (*uxa_screen->info->done_solid) (pDstPix);

    REGION_UNINIT(pDst->pDrawable->pScreen, &region);
    return 1;
}

static int
uxa_try_driver_composite_rects(CARD8		    op,
			       PicturePtr	    pSrc,
			       PicturePtr	    pDst,
			       int		    nrect,
			       uxa_composite_rect_t *rects)
{
    uxa_screen_t    *uxa_screen = uxa_get_screen(pDst->pDrawable->pScreen);
    int src_off_x, src_off_y, dst_off_x, dst_off_y;
    PixmapPtr pSrcPix, pDstPix;

    if (!uxa_screen->info->prepare_composite || uxa_screen->swappedOut)
	return -1;

    if (uxa_screen->info->check_composite &&
	!(*uxa_screen->info->check_composite) (op, pSrc, NULL, pDst))
    {
	return -1;
    }
    
    pDstPix = uxa_get_offscreen_pixmap(pDst->pDrawable, &dst_off_x, &dst_off_y);
    if (!pDstPix)
	return 0;

    pSrcPix = uxa_get_offscreen_pixmap(pSrc->pDrawable, &src_off_x, &src_off_y);
    if (!pSrcPix)
	return 0;

    if (!(*uxa_screen->info->prepare_composite) (op, pSrc, NULL, pDst, pSrcPix,
						 NULL, pDstPix))
	return -1;

    while (nrect--)
    {
	INT16 xDst = rects->xDst + pDst->pDrawable->x;
	INT16 yDst = rects->yDst + pDst->pDrawable->y;
	INT16 xSrc = rects->xSrc + pSrc->pDrawable->x;
	INT16 ySrc = rects->ySrc + pSrc->pDrawable->y;
	
	RegionRec region;
	BoxPtr pbox;
	int nbox;
	
	if (!miComputeCompositeRegion (&region, pSrc, NULL, pDst,
				       xSrc, ySrc, 0, 0, xDst, yDst,
				       rects->width, rects->height))
	    goto next_rect;
	
	REGION_TRANSLATE(pScreen, &region, dst_off_x, dst_off_y);
	
	nbox = REGION_NUM_RECTS(&region);
	pbox = REGION_RECTS(&region);

	xSrc = xSrc + src_off_x - xDst - dst_off_x;
	ySrc = ySrc + src_off_y - yDst - dst_off_y;
	
	while (nbox--)
	{
	    (*uxa_screen->info->composite) (pDstPix,
					    pbox->x1 + xSrc,
					    pbox->y1 + ySrc,
					    0, 0,
					    pbox->x1,
					    pbox->y1,
					    pbox->x2 - pbox->x1,
					    pbox->y2 - pbox->y1);
	    pbox++;
	}

    next_rect:
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);

	rects++;
    }
    (*uxa_screen->info->done_composite) (pDstPix);
	
    return 1;
}

/**
 * Copy a number of rectangles from source to destination in a single
 * operation. This is specialized for building a glyph mask: we don'y
 * have a mask argument because we don't need it for that, and we
 * don't have he special-case fallbacks found in uxa_composite() - if the
 * driver can support it, we use the driver functionality, otherwise we
 * fallback straight to software.
 */
void
uxa_composite_rects(CARD8	          op,
		    PicturePtr		  pSrc,
		    PicturePtr		  pDst,
		    int			  nrect,
		    uxa_composite_rect_t  *rects)
{
    int			   n;
    uxa_composite_rect_t   *r;
    
    /************************************************************/
    
    ValidatePicture (pSrc);
    ValidatePicture (pDst);
    
    if (uxa_try_driver_composite_rects(op, pSrc, pDst, nrect, rects) != 1) {
	n = nrect;
	r = rects;
	while (n--) {
	    uxa_check_composite (op, pSrc, NULL, pDst,
			       r->xSrc, r->ySrc,
			       0, 0,
			       r->xDst, r->yDst,
			       r->width, r->height);
	    r++;
	}
    }
    
    /************************************************************/

}

static int
uxa_try_driver_composite(CARD8		op,
			 PicturePtr	pSrc,
			 PicturePtr	pMask,
			 PicturePtr	pDst,
			 INT16		xSrc,
			 INT16		ySrc,
			 INT16		xMask,
			 INT16		yMask,
			 INT16		xDst,
			 INT16		yDst,
			 CARD16		width,
			 CARD16		height)
{
    uxa_screen_t    *uxa_screen = uxa_get_screen(pDst->pDrawable->pScreen);
    RegionRec region;
    BoxPtr pbox;
    int nbox;
    int src_off_x, src_off_y, mask_off_x, mask_off_y, dst_off_x, dst_off_y;
    PixmapPtr pSrcPix, pMaskPix = NULL, pDstPix;

    xDst += pDst->pDrawable->x;
    yDst += pDst->pDrawable->y;

    if (pMask) {
	xMask += pMask->pDrawable->x;
	yMask += pMask->pDrawable->y;
    }

    xSrc += pSrc->pDrawable->x;
    ySrc += pSrc->pDrawable->y;

    if (uxa_screen->info->check_composite &&
	!(*uxa_screen->info->check_composite) (op, pSrc, pMask, pDst))
    {
	return -1;
    }

    if (!miComputeCompositeRegion (&region, pSrc, pMask, pDst,
				   xSrc, ySrc, xMask, yMask, xDst, yDst,
				   width, height))
	return 1;

    pDstPix = uxa_get_offscreen_pixmap (pDst->pDrawable, &dst_off_x, &dst_off_y);

    pSrcPix = uxa_get_offscreen_pixmap (pSrc->pDrawable, &src_off_x, &src_off_y);

    if (pMask)
	pMaskPix = uxa_get_offscreen_pixmap (pMask->pDrawable, &mask_off_x,
					     &mask_off_y);

    if (!pDstPix || !pSrcPix || (pMask && !pMaskPix)) {
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return 0;
    }

    REGION_TRANSLATE(pScreen, &region, dst_off_x, dst_off_y);

    if (!(*uxa_screen->info->prepare_composite) (op, pSrc, pMask, pDst, pSrcPix,
						 pMaskPix, pDstPix))
    {
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return -1;
    }

    nbox = REGION_NUM_RECTS(&region);
    pbox = REGION_RECTS(&region);

    xMask = xMask + mask_off_x - xDst - dst_off_x;
    yMask = yMask + mask_off_y - yDst - dst_off_y;

    xSrc = xSrc + src_off_x - xDst - dst_off_x;
    ySrc = ySrc + src_off_y - yDst - dst_off_y;

    while (nbox--)
    {
	(*uxa_screen->info->composite) (pDstPix,
					pbox->x1 + xSrc,
					pbox->y1 + ySrc,
					pbox->x1 + xMask,
					pbox->y1 + yMask,
					pbox->x1,
					pbox->y1,
					pbox->x2 - pbox->x1,
					pbox->y2 - pbox->y1);
	pbox++;
    }
    (*uxa_screen->info->done_composite) (pDstPix);

    REGION_UNINIT(pDst->pDrawable->pScreen, &region);
    return 1;
}

/**
 * uxa_try_magic_two_pass_composite_helper implements PictOpOver using two passes of
 * simpler operations PictOpOutReverse and PictOpAdd. Mainly used for component
 * alpha and limited 1-tmu cards.
 *
 * From http://anholt.livejournal.com/32058.html:
 *
 * The trouble is that component-alpha rendering requires two different sources
 * for blending: one for the source value to the blender, which is the
 * per-channel multiplication of source and mask, and one for the source alpha
 * for multiplying with the destination channels, which is the multiplication
 * of the source channels by the mask alpha. So the equation for Over is:
 *
 * dst.A = src.A * mask.A + (1 - (src.A * mask.A)) * dst.A
 * dst.R = src.R * mask.R + (1 - (src.A * mask.R)) * dst.R
 * dst.G = src.G * mask.G + (1 - (src.A * mask.G)) * dst.G
 * dst.B = src.B * mask.B + (1 - (src.A * mask.B)) * dst.B
 *
 * But we can do some simpler operations, right? How about PictOpOutReverse,
 * which has a source factor of 0 and dest factor of (1 - source alpha). We
 * can get the source alpha value (srca.X = src.A * mask.X) out of the texture
 * blenders pretty easily. So we can do a component-alpha OutReverse, which
 * gets us:
 *
 * dst.A = 0 + (1 - (src.A * mask.A)) * dst.A
 * dst.R = 0 + (1 - (src.A * mask.R)) * dst.R
 * dst.G = 0 + (1 - (src.A * mask.G)) * dst.G
 * dst.B = 0 + (1 - (src.A * mask.B)) * dst.B
 *
 * OK. And if an op doesn't use the source alpha value for the destination
 * factor, then we can do the channel multiplication in the texture blenders
 * to get the source value, and ignore the source alpha that we wouldn't use.
 * We've supported this in the Radeon driver for a long time. An example would
 * be PictOpAdd, which does:
 *
 * dst.A = src.A * mask.A + dst.A
 * dst.R = src.R * mask.R + dst.R
 * dst.G = src.G * mask.G + dst.G
 * dst.B = src.B * mask.B + dst.B
 *
 * Hey, this looks good! If we do a PictOpOutReverse and then a PictOpAdd right
 * after it, we get:
 *
 * dst.A = src.A * mask.A + ((1 - (src.A * mask.A)) * dst.A)
 * dst.R = src.R * mask.R + ((1 - (src.A * mask.R)) * dst.R)
 * dst.G = src.G * mask.G + ((1 - (src.A * mask.G)) * dst.G)
 * dst.B = src.B * mask.B + ((1 - (src.A * mask.B)) * dst.B)
 */

static int
uxa_try_magic_two_pass_composite_helper(CARD8 op,
				  PicturePtr pSrc,
				  PicturePtr pMask,
				  PicturePtr pDst,
				  INT16 xSrc,
				  INT16 ySrc,
				  INT16 xMask,
				  INT16 yMask,
				  INT16 xDst,
				  INT16 yDst,
				  CARD16 width,
				  CARD16 height)
{
    uxa_screen_t    *uxa_screen = uxa_get_screen(pDst->pDrawable->pScreen);

    assert(op == PictOpOver);

    if (uxa_screen->info->check_composite &&
	(!(*uxa_screen->info->check_composite)(PictOpOutReverse, pSrc, pMask,
					   pDst) ||
	 !(*uxa_screen->info->check_composite)(PictOpAdd, pSrc, pMask, pDst)))
    {
	return -1;
    }

    /* Now, we think we should be able to accelerate this operation. First,
     * composite the destination to be the destination times the source alpha
     * factors.
     */
    uxa_composite(PictOpOutReverse, pSrc, pMask, pDst, xSrc, ySrc, xMask, yMask,
		 xDst, yDst, width, height);

    /* Then, add in the source value times the destination alpha factors (1.0).
     */
    uxa_composite(PictOpAdd, pSrc, pMask, pDst, xSrc, ySrc, xMask, yMask,
		 xDst, yDst, width, height);

    return 1;
}

void
uxa_composite(CARD8	op,
	     PicturePtr pSrc,
	     PicturePtr pMask,
	     PicturePtr pDst,
	     INT16	xSrc,
	     INT16	ySrc,
	     INT16	xMask,
	     INT16	yMask,
	     INT16	xDst,
	     INT16	yDst,
	     CARD16	width,
	     CARD16	height)
{
    uxa_screen_t    *uxa_screen = uxa_get_screen(pDst->pDrawable->pScreen);
    int ret = -1;
    Bool saveSrcRepeat = pSrc->repeat;
    Bool saveMaskRepeat = pMask ? pMask->repeat : 0;
    RegionRec region;

    /* We currently don't support acceleration of gradients, or other pictures
     * with a NULL pDrawable.
     */
    if (uxa_screen->swappedOut ||
	pSrc->pDrawable == NULL || (pMask != NULL && pMask->pDrawable == NULL))
    {
	goto fallback;
    }

    /* Remove repeat in source if useless */
    if (pSrc->repeat && !pSrc->transform && xSrc >= 0 &&
	(xSrc + width) <= pSrc->pDrawable->width && ySrc >= 0 &&
	(ySrc + height) <= pSrc->pDrawable->height)
	    pSrc->repeat = 0;

    if (!pMask)
    {
      if ((op == PictOpSrc &&
	   ((pSrc->format == pDst->format) ||
	    (pSrc->format==PICT_a8r8g8b8 && pDst->format==PICT_x8r8g8b8) ||
	    (pSrc->format==PICT_a8b8g8r8 && pDst->format==PICT_x8b8g8r8))) ||
	  (op == PictOpOver && !pSrc->alphaMap && !pDst->alphaMap &&
	   pSrc->format == pDst->format &&
	   (pSrc->format==PICT_x8r8g8b8 || pSrc->format==PICT_x8b8g8r8)))
	{
	    if (pSrc->pDrawable->width == 1 &&
		pSrc->pDrawable->height == 1 &&
		pSrc->repeat)
	    {
		ret = uxa_try_driver_solid_fill(pSrc, pDst, xSrc, ySrc, xDst, yDst,
					    width, height);
		if (ret == 1)
		    goto done;
	    }
	    else if (pSrc->pDrawable != NULL &&
		     !pSrc->repeat &&
		     !pSrc->transform)
	    {
		xDst += pDst->pDrawable->x;
		yDst += pDst->pDrawable->y;
		xSrc += pSrc->pDrawable->x;
		ySrc += pSrc->pDrawable->y;

		if (!miComputeCompositeRegion (&region, pSrc, pMask, pDst,
					       xSrc, ySrc, xMask, yMask, xDst,
					       yDst, width, height))
		    goto done;


		uxa_copy_n_to_n (pSrc->pDrawable, pDst->pDrawable, NULL,
			     REGION_RECTS(&region), REGION_NUM_RECTS(&region),
			     xSrc - xDst, ySrc - yDst,
			     FALSE, FALSE, 0, NULL);
		REGION_UNINIT(pDst->pDrawable->pScreen, &region);
		goto done;
	    }
	    else if (pSrc->pDrawable != NULL &&
		     pSrc->pDrawable->type == DRAWABLE_PIXMAP &&
		     !pSrc->transform &&
		     pSrc->repeatType == RepeatNormal)
	    {
		DDXPointRec patOrg;

		/* Let's see if the driver can do the repeat in one go */
		if (uxa_screen->info->prepare_composite && !pSrc->alphaMap &&
		    !pDst->alphaMap)
		{
		    ret = uxa_try_driver_composite(op, pSrc, pMask, pDst, xSrc,
						ySrc, xMask, yMask, xDst, yDst,
						width, height);
		    if (ret == 1)
			goto done;
		}

		/* Now see if we can use uxa_fill_region_tiled() */
		xDst += pDst->pDrawable->x;
		yDst += pDst->pDrawable->y;
		xSrc += pSrc->pDrawable->x;
		ySrc += pSrc->pDrawable->y;

		if (!miComputeCompositeRegion (&region, pSrc, pMask, pDst, xSrc,
					       ySrc, xMask, yMask, xDst, yDst,
					       width, height))
		    goto done;

		/* pattern origin is the point in the destination drawable
		 * corresponding to (0,0) in the source */
		patOrg.x = xDst - xSrc;
		patOrg.y = yDst - ySrc;

		ret = uxa_fill_region_tiled(pDst->pDrawable, &region,
					 (PixmapPtr)pSrc->pDrawable,
					 &patOrg, FB_ALLONES, GXcopy);

		REGION_UNINIT(pDst->pDrawable->pScreen, &region);

		if (ret)
		    goto done;
	    }
	}
    }

    /* Remove repeat in mask if useless */
    if (pMask && pMask->repeat && !pMask->transform && xMask >= 0 &&
	(xMask + width) <= pMask->pDrawable->width && yMask >= 0 &&
	(yMask + height) <= pMask->pDrawable->height)
	    pMask->repeat = 0;

    if (uxa_screen->info->prepare_composite &&
	!pSrc->alphaMap && (!pMask || !pMask->alphaMap) && !pDst->alphaMap)
    {
	Bool isSrcSolid;

	ret = uxa_try_driver_composite(op, pSrc, pMask, pDst, xSrc, ySrc, xMask,
				    yMask, xDst, yDst, width, height);
	if (ret == 1)
	    goto done;

	/* For generic masks and solid src pictures, mach64 can do Over in two
	 * passes, similar to the component-alpha case.
	 */
	isSrcSolid = pSrc->pDrawable->width == 1 &&
		     pSrc->pDrawable->height == 1 &&
		     pSrc->repeat;

	/* If we couldn't do the Composite in a single pass, and it was a
	 * component-alpha Over, see if we can do it in two passes with
	 * an OutReverse and then an Add.
	 */
	if (ret == -1 && op == PictOpOver && pMask &&
	    (pMask->componentAlpha || isSrcSolid)) {
	    ret = uxa_try_magic_two_pass_composite_helper(op, pSrc, pMask, pDst,
						    xSrc, ySrc,
						    xMask, yMask, xDst, yDst,
						    width, height);
	    if (ret == 1)
		goto done;
	}
    }

fallback:
    if (uxa_screen->fallback_debug)
	uxa_print_composite_fallback (op, pSrc, pMask, pDst);

    uxa_check_composite (op, pSrc, pMask, pDst, xSrc, ySrc,
		      xMask, yMask, xDst, yDst, width, height);

done:
    pSrc->repeat = saveSrcRepeat;
    if (pMask)
	pMask->repeat = saveMaskRepeat;
}
#endif

/**
 * Same as miCreateAlphaPicture, except it uses uxa_check_poly_fill_rect instead
 * of PolyFillRect to initialize the pixmap after creating it, to prevent
 * the pixmap from being migrated.
 *
 * See the comments about uxa_trapezoids and uxa_triangles.
 */
static PicturePtr
uxa_create_alpha_picture (ScreenPtr     pScreen,
                       PicturePtr    pDst,
                       PictFormatPtr pPictFormat,
                       CARD16        width,
                       CARD16        height)
{
    PixmapPtr	    pPixmap;
    PicturePtr	    pPicture;
    GCPtr	    pGC;
    int		    error;
    xRectangle	    rect;

    if (width > 32767 || height > 32767)
	return 0;

    if (!pPictFormat)
    {
	if (pDst->polyEdge == PolyEdgeSharp)
	    pPictFormat = PictureMatchFormat (pScreen, 1, PICT_a1);
	else
	    pPictFormat = PictureMatchFormat (pScreen, 8, PICT_a8);
	if (!pPictFormat)
	    return 0;
    }

#ifdef SERVER_1_5
    pPixmap = (*pScreen->CreatePixmap) (pScreen, width, height,
					pPictFormat->depth, 0);
#else
    pPixmap = (*pScreen->CreatePixmap) (pScreen, width, height,
					pPictFormat->depth);
#endif
    if (!pPixmap)
	return 0;
    pGC = GetScratchGC (pPixmap->drawable.depth, pScreen);
    if (!pGC)
    {
	(*pScreen->DestroyPixmap) (pPixmap);
	return 0;
    }
    ValidateGC (&pPixmap->drawable, pGC);
    rect.x = 0;
    rect.y = 0;
    rect.width = width;
    rect.height = height;
    uxa_check_poly_fill_rect (&pPixmap->drawable, pGC, 1, &rect);
    FreeScratchGC (pGC);
    pPicture = CreatePicture (0, &pPixmap->drawable, pPictFormat,
			      0, 0, serverClient, &error);
    (*pScreen->DestroyPixmap) (pPixmap);
    return pPicture;
}

/**
 * uxa_trapezoids is essentially a copy of miTrapezoids that uses
 * uxa_create_alpha_picture instead of miCreateAlphaPicture.
 *
 * The problem with miCreateAlphaPicture is that it calls PolyFillRect
 * to initialize the contents after creating the pixmap, which
 * causes the pixmap to be moved in for acceleration. The subsequent
 * call to RasterizeTrapezoid won't be accelerated however, which
 * forces the pixmap to be moved out again.
 *
 * uxa_create_alpha_picture avoids this roundtrip by using uxa_check_poly_fill_rect
 * to initialize the contents.
 */
void
uxa_trapezoids (CARD8 op, PicturePtr pSrc, PicturePtr pDst,
               PictFormatPtr maskFormat, INT16 xSrc, INT16 ySrc,
               int ntrap, xTrapezoid *traps)
{
    ScreenPtr		pScreen = pDst->pDrawable->pScreen;
    PictureScreenPtr    ps = GetPictureScreen(pScreen);
    BoxRec		bounds;
    Bool		direct = op == PictOpAdd && miIsSolidAlpha (pSrc);

    if (maskFormat || direct) {
	miTrapezoidBounds (ntrap, traps, &bounds);

	if (bounds.y1 >= bounds.y2 || bounds.x1 >= bounds.x2)
	    return;
    }

    /*
     * Check for solid alpha add
     */
    if (direct)
    {
	DrawablePtr pDraw = pDst->pDrawable;
	PixmapPtr pixmap = uxa_get_drawable_pixmap (pDraw);
	int xoff, yoff;

	uxa_get_drawable_deltas(pDraw, pixmap, &xoff, &yoff);

	xoff += pDraw->x;
	yoff += pDraw->y;

	if (uxa_prepare_access(pDraw, UXA_ACCESS_RW))
	{
	    for (; ntrap; ntrap--, traps++)
		(*ps->RasterizeTrapezoid) (pDst, traps, 0, 0);
	    uxa_finish_access(pDraw);
	}
    }
    else if (maskFormat)
    {
	PicturePtr	pPicture;
	INT16		xDst, yDst;
	INT16		xRel, yRel;

	xDst = traps[0].left.p1.x >> 16;
	yDst = traps[0].left.p1.y >> 16;

	pPicture = uxa_create_alpha_picture (pScreen, pDst, maskFormat,
	                                  bounds.x2 - bounds.x1,
	                                  bounds.y2 - bounds.y1);
	if (!pPicture)
	    return;

	if (uxa_prepare_access(pPicture->pDrawable, UXA_ACCESS_RW)) {
	    for (; ntrap; ntrap--, traps++)
		(*ps->RasterizeTrapezoid) (pPicture, traps,
					   -bounds.x1, -bounds.y1);
	    uxa_finish_access(pPicture->pDrawable);
	}

	xRel = bounds.x1 + xSrc - xDst;
	yRel = bounds.y1 + ySrc - yDst;
	CompositePicture (op, pSrc, pPicture, pDst,
			  xRel, yRel, 0, 0, bounds.x1, bounds.y1,
			  bounds.x2 - bounds.x1,
			  bounds.y2 - bounds.y1);
	FreePicture (pPicture, 0);
    }
    else
    {
	if (pDst->polyEdge == PolyEdgeSharp)
	    maskFormat = PictureMatchFormat (pScreen, 1, PICT_a1);
	else
	    maskFormat = PictureMatchFormat (pScreen, 8, PICT_a8);
	for (; ntrap; ntrap--, traps++)
	    uxa_trapezoids (op, pSrc, pDst, maskFormat, xSrc, ySrc, 1, traps);
    }
}

/**
 * uxa_triangles is essentially a copy of miTriangles that uses
 * uxa_create_alpha_picture instead of miCreateAlphaPicture.
 *
 * The problem with miCreateAlphaPicture is that it calls PolyFillRect
 * to initialize the contents after creating the pixmap, which
 * causes the pixmap to be moved in for acceleration. The subsequent
 * call to AddTriangles won't be accelerated however, which forces the pixmap
 * to be moved out again.
 *
 * uxa_create_alpha_picture avoids this roundtrip by using uxa_check_poly_fill_rect
 * to initialize the contents.
 */
void
uxa_triangles (CARD8 op, PicturePtr pSrc, PicturePtr pDst,
	      PictFormatPtr maskFormat, INT16 xSrc, INT16 ySrc,
	      int ntri, xTriangle *tris)
{
    ScreenPtr		pScreen = pDst->pDrawable->pScreen;
    PictureScreenPtr    ps = GetPictureScreen(pScreen);
    BoxRec		bounds;
    Bool		direct = op == PictOpAdd && miIsSolidAlpha (pSrc);

    if (maskFormat || direct) {
	miTriangleBounds (ntri, tris, &bounds);

	if (bounds.y1 >= bounds.y2 || bounds.x1 >= bounds.x2)
	    return;
    }

    /*
     * Check for solid alpha add
     */
    if (direct)
    {
	DrawablePtr pDraw = pDst->pDrawable;
	if (uxa_prepare_access(pDraw, UXA_ACCESS_RW)) {
	    (*ps->AddTriangles) (pDst, 0, 0, ntri, tris);
	    uxa_finish_access(pDraw);
	}
    }
    else if (maskFormat)
    {
	PicturePtr	pPicture;
	INT16		xDst, yDst;
	INT16		xRel, yRel;
	
	xDst = tris[0].p1.x >> 16;
	yDst = tris[0].p1.y >> 16;

	pPicture = uxa_create_alpha_picture (pScreen, pDst, maskFormat,
					  bounds.x2 - bounds.x1,
					  bounds.y2 - bounds.y1);
	if (!pPicture)
	    return;

	if (uxa_prepare_access(pPicture->pDrawable, UXA_ACCESS_RW)) {
	    (*ps->AddTriangles) (pPicture, -bounds.x1, -bounds.y1, ntri, tris);
	    uxa_finish_access(pPicture->pDrawable);
	}
	
	xRel = bounds.x1 + xSrc - xDst;
	yRel = bounds.y1 + ySrc - yDst;
	CompositePicture (op, pSrc, pPicture, pDst,
			  xRel, yRel, 0, 0, bounds.x1, bounds.y1,
			  bounds.x2 - bounds.x1, bounds.y2 - bounds.y1);
	FreePicture (pPicture, 0);
    }
    else
    {
	if (pDst->polyEdge == PolyEdgeSharp)
	    maskFormat = PictureMatchFormat (pScreen, 1, PICT_a1);
	else
	    maskFormat = PictureMatchFormat (pScreen, 8, PICT_a8);
	
	for (; ntri; ntri--, tris++)
	    uxa_triangles (op, pSrc, pDst, maskFormat, xSrc, ySrc, 1, tris);
    }
}
