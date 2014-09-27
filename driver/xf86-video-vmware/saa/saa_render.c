/*
 * Copyright © 2001 Keith Packard
 * Copyright 2011 VMWare, Inc. All Rights Reserved.
 * May partly be based on code that is Copyright Â© The XFree86 Project Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author: Eric Anholt <eric@anholt.net>
 * Author: Michel Dänzer <michel@tungstengraphics.com>
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 */
#include "saa.h"
#include "saa_priv.h"

#ifdef RENDER
#include <mipict.h>

/**
 * Same as miCreateAlphaPicture, except it uses
 * saa_check_poly_fill_rect instead
 */

static PicturePtr
saa_create_alpha_picture(ScreenPtr pScreen,
			 PicturePtr pDst,
			 PictFormatPtr pPictFormat, CARD16 width, CARD16 height)
{
    PixmapPtr pPixmap;
    PicturePtr pPicture;
    GCPtr pGC;
    int error;
    xRectangle rect;

    if (width > 32767 || height > 32767)
	return 0;

    if (!pPictFormat) {
	if (pDst->polyEdge == PolyEdgeSharp)
	    pPictFormat = PictureMatchFormat(pScreen, 1, PICT_a1);
	else
	    pPictFormat = PictureMatchFormat(pScreen, 8, PICT_a8);
	if (!pPictFormat)
	    return 0;
    }

    pPixmap = (*pScreen->CreatePixmap) (pScreen, width, height,
					pPictFormat->depth, 0);
    if (!pPixmap)
	return 0;
    pGC = GetScratchGC(pPixmap->drawable.depth, pScreen);
    if (!pGC) {
	(*pScreen->DestroyPixmap) (pPixmap);
	return 0;
    }
    ValidateGC(&pPixmap->drawable, pGC);
    rect.x = 0;
    rect.y = 0;
    rect.width = width;
    rect.height = height;
    saa_check_poly_fill_rect(&pPixmap->drawable, pGC, 1, &rect);
    FreeScratchGC(pGC);
    pPicture = CreatePicture(0, &pPixmap->drawable, pPictFormat,
			     0, 0, serverClient, &error);
    (*pScreen->DestroyPixmap) (pPixmap);
    return pPicture;
}

/**
 * saa_trapezoids is essentially a copy of miTrapezoids that uses
 * saa_create_alpha_picture instead of miCreateAlphaPicture.
 *
 * The problem with miCreateAlphaPicture is that it calls PolyFillRect
 * to initialize the contents after creating the pixmap, which
 * causes the pixmap to be moved in for acceleration. The subsequent
 * call to RasterizeTrapezoid won't be accelerated however, which
 * forces the pixmap to be moved out again.
 *
 */
static void
saa_trapezoids(CARD8 op, PicturePtr pSrc, PicturePtr pDst,
	       PictFormatPtr maskFormat, INT16 xSrc, INT16 ySrc,
	       int ntrap, xTrapezoid * traps)
{
    ScreenPtr pScreen = pDst->pDrawable->pScreen;
    PictureScreenPtr ps = GetPictureScreen(pScreen);
    BoxRec bounds;

    if (maskFormat) {
	PicturePtr pPicture;
	INT16 xDst, yDst;
	INT16 xRel, yRel;
	saa_access_t access;

	miTrapezoidBounds(ntrap, traps, &bounds);

	if (bounds.y1 >= bounds.y2 || bounds.x1 >= bounds.x2)
	    return;

	xDst = traps[0].left.p1.x >> 16;
	yDst = traps[0].left.p1.y >> 16;

	pPicture = saa_create_alpha_picture(pScreen, pDst, maskFormat,
					    bounds.x2 - bounds.x1,
					    bounds.y2 - bounds.y1);
	if (!pPicture)
	    return;

	if (saa_pad_write(pPicture->pDrawable, NULL, FALSE, &access)) {
	    for (; ntrap; ntrap--, traps++)
		(*ps->RasterizeTrapezoid) (pPicture, traps,
					   -bounds.x1, -bounds.y1);
	    saa_fad_write(pPicture->pDrawable, access);
	}

	xRel = bounds.x1 + xSrc - xDst;
	yRel = bounds.y1 + ySrc - yDst;
	CompositePicture(op, pSrc, pPicture, pDst,
			 xRel, yRel, 0, 0, bounds.x1, bounds.y1,
			 bounds.x2 - bounds.x1, bounds.y2 - bounds.y1);
	FreePicture(pPicture, 0);
    } else {
	if (pDst->polyEdge == PolyEdgeSharp)
	    maskFormat = PictureMatchFormat(pScreen, 1, PICT_a1);
	else
	    maskFormat = PictureMatchFormat(pScreen, 8, PICT_a8);
	for (; ntrap; ntrap--, traps++)
	    saa_trapezoids(op, pSrc, pDst, maskFormat, xSrc, ySrc, 1, traps);
    }
}

/**
 * saa_triangles is essentially a copy of miTriangles that uses
 * saa_create_alpha_picture instead of miCreateAlphaPicture.
 *
 * The problem with miCreateAlphaPicture is that it calls PolyFillRect
 * to initialize the contents after creating the pixmap, which
 * causes the pixmap to be moved in for acceleration. The subsequent
 * call to AddTriangles won't be accelerated however, which forces the pixmap
 * to be moved out again.
 */
static void
saa_triangles(CARD8 op, PicturePtr pSrc, PicturePtr pDst,
	      PictFormatPtr maskFormat, INT16 xSrc, INT16 ySrc,
	      int ntri, xTriangle * tris)
{
    ScreenPtr pScreen = pDst->pDrawable->pScreen;
    PictureScreenPtr ps = GetPictureScreen(pScreen);
    BoxRec bounds;

    if (maskFormat) {
	PicturePtr pPicture;
	INT16 xDst, yDst;
	INT16 xRel, yRel;
	saa_access_t access;

	miTriangleBounds(ntri, tris, &bounds);

	if (bounds.y1 >= bounds.y2 || bounds.x1 >= bounds.x2)
	    return;

	xDst = tris[0].p1.x >> 16;
	yDst = tris[0].p1.y >> 16;

	pPicture = saa_create_alpha_picture(pScreen, pDst, maskFormat,
					    bounds.x2 - bounds.x1,
					    bounds.y2 - bounds.y1);
	if (!pPicture)
	    return;

	if (saa_pad_write(pPicture->pDrawable, NULL, FALSE, &access)) {
	    (*ps->AddTriangles) (pPicture, -bounds.x1, -bounds.y1, ntri, tris);
	    saa_fad_write(pPicture->pDrawable, access);
	}

	xRel = bounds.x1 + xSrc - xDst;
	yRel = bounds.y1 + ySrc - yDst;
	CompositePicture(op, pSrc, pPicture, pDst,
			 xRel, yRel, 0, 0, bounds.x1, bounds.y1,
			 bounds.x2 - bounds.x1, bounds.y2 - bounds.y1);
	FreePicture(pPicture, 0);
    } else {
	if (pDst->polyEdge == PolyEdgeSharp)
	    maskFormat = PictureMatchFormat(pScreen, 1, PICT_a1);
	else
	    maskFormat = PictureMatchFormat(pScreen, 8, PICT_a8);

	for (; ntri; ntri--, tris++)
	    saa_triangles(op, pSrc, pDst, maskFormat, xSrc, ySrc, 1, tris);
    }
}

static Bool
saa_driver_composite(CARD8		op,
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
		     CARD16		height,
		     RegionPtr src_reg,
		     RegionPtr mask_reg,
		     RegionPtr dst_reg)
{
    struct saa_screen_priv *sscreen = saa_screen(pDst->pDrawable->pScreen);
    BoxPtr pbox;
    int nbox;
    int src_off_x, src_off_y, mask_off_x, mask_off_y, dst_off_x, dst_off_y;
    PixmapPtr src_pix = NULL, mask_pix = NULL, dst_pix;
    struct saa_driver *driver = sscreen->driver;

    if (!driver->composite_prepare)
	return FALSE;

    dst_pix = saa_get_pixmap(pDst->pDrawable, &dst_off_x, &dst_off_y);
    if (saa_pixmap(dst_pix)->auth_loc != saa_loc_driver)
	return FALSE;

    if (pMask && pMask->pDrawable) {
	mask_pix = saa_get_pixmap(pMask->pDrawable, &mask_off_x, &mask_off_y);
	if (saa_pixmap(mask_pix)->auth_loc != saa_loc_driver)
	    return FALSE;
    }
    if (pSrc->pDrawable) {
	src_pix = saa_get_pixmap(pSrc->pDrawable, &src_off_x, &src_off_y);
	if (saa_pixmap(src_pix)->auth_loc != saa_loc_driver)
	    return FALSE;
    }

    if (!driver->composite_prepare(driver, op, pSrc, pMask, pDst,
				   src_pix, mask_pix, dst_pix,
				   src_reg, mask_reg, dst_reg))
	return FALSE;

    nbox = REGION_NUM_RECTS(dst_reg);
    pbox = REGION_RECTS(dst_reg);

    xDst += pDst->pDrawable->x + dst_off_x;
    yDst += pDst->pDrawable->y + dst_off_y;

    if (src_pix) {
	xSrc += pSrc->pDrawable->x + src_off_x - xDst;
	ySrc += pSrc->pDrawable->y + src_off_y - yDst;
    }
    if (mask_pix) {
	xMask += pMask->pDrawable->x + mask_off_x - xDst;
	yMask += pMask->pDrawable->y + mask_off_y - yDst;
    }

    while (nbox--) {
	driver->composite(driver,
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

    driver->composite_done(driver);
    saa_pixmap_dirty(dst_pix, TRUE, dst_reg);

    return TRUE;
}

/*
 * Try to turn a composite operation into an accelerated copy.
 * We can do that in some special cases for PictOpSrc and PictOpOver.
 */

static Bool
saa_copy_composite(CARD8 op,
		   PicturePtr pSrc,
		   PicturePtr pMask,
		   PicturePtr pDst,
		   INT16 xSrc,
		   INT16 ySrc,
		   INT16 xMask,
		   INT16 yMask,
		   INT16 xDst, INT16 yDst, CARD16 width, CARD16 height,
		   RegionPtr dst_region)
{
    if (!pSrc->pDrawable || pSrc->transform ||
	pSrc->repeat || xSrc < 0 || ySrc < 0 ||
	xSrc + width > pSrc->pDrawable->width ||
	ySrc + height > pSrc->pDrawable->height)
	return FALSE;

    if (op == PictOpSrc ||
	(op == PictOpOver && PICT_FORMAT_A(pSrc->format) == 0 &&
	 pMask == NULL)) {

	int xoff, yoff;
	PixmapPtr dst_pix = saa_get_pixmap(pDst->pDrawable, &xoff, &yoff);
	struct saa_pixmap *dst_spix = saa_pixmap(dst_pix);
	struct saa_pixmap *src_spix =
	    saa_pixmap(saa_get_drawable_pixmap(pSrc->pDrawable));
	int ret;

	if (src_spix->auth_loc != saa_loc_driver ||
	    dst_spix->auth_loc != saa_loc_driver)
	    return FALSE;

	src_spix->src_format = pSrc->format;
	dst_spix->dst_format = pDst->format;

	xDst += pDst->pDrawable->x;
	yDst += pDst->pDrawable->y;
	xSrc += pSrc->pDrawable->x;
	ySrc += pSrc->pDrawable->y;

	/*
	 * Dst region is in backing pixmap space. We need to
	 * translate it.
	 */
	REGION_TRANSLATE(pScreen, dst_region, -xoff, -yoff);
	ret = saa_hw_copy_nton(pSrc->pDrawable, pDst->pDrawable, NULL,
			       REGION_RECTS(dst_region),
			       REGION_NUM_RECTS(dst_region),
			       xSrc - xDst, ySrc - yDst, FALSE, FALSE);
	REGION_TRANSLATE(pScreen, dst_region, xoff, yoff);

	src_spix->src_format = 0;
	dst_spix->dst_format = 0;

	if (ret)
	    return TRUE;
    }
    return FALSE;
}

static void
saa_composite(CARD8 op,
	      PicturePtr pSrc,
	      PicturePtr pMask,
	      PicturePtr pDst,
	      INT16 xSrc,
	      INT16 ySrc,
	      INT16 xMask,
	      INT16 yMask, INT16 xDst, INT16 yDst, CARD16 width, CARD16 height)
{
    ScreenPtr pScreen = pDst->pDrawable->pScreen;
    RegionRec dst_region;
    RegionPtr src_region;
    RegionPtr mask_region;

    REGION_NULL(pScreen, &dst_region);
    if (!saa_compute_composite_regions(pScreen, pSrc, pMask, pDst,
				       xSrc, ySrc, xMask, yMask, xDst,
				       yDst, width, height,
				       &dst_region, &src_region, &mask_region))
	goto out;

    if (saa_copy_composite(op, pSrc, pMask, pDst, xSrc, ySrc, xMask, yMask,
			   xDst, yDst, width, height, &dst_region))
	goto out;

    if (saa_driver_composite(op, pSrc, pMask, pDst, xSrc, ySrc, xMask, yMask,
			     xDst, yDst, width, height, src_region,
			     mask_region, &dst_region))
	goto out;

    saa_check_composite(op, pSrc, pMask, pDst, xSrc, ySrc, xMask, yMask,
			xDst, yDst, width, height,
			src_region, mask_region, &dst_region);
  out:
    if (src_region)
	REGION_UNINIT(pScreen, src_region);
    if (mask_region && mask_region != src_region)
	REGION_UNINIT(pScreen, mask_region);
    REGION_UNINIT(pScreen, &dst_region);
}

void
saa_render_setup(ScreenPtr pScreen)
{
    PictureScreenPtr ps = GetPictureScreenIfSet(pScreen);
    struct saa_screen_priv *sscreen = saa_screen(pScreen);

    if (ps) {
	saa_wrap(sscreen, ps, Trapezoids, saa_trapezoids);
	saa_wrap(sscreen, ps, Triangles, saa_triangles);
	saa_wrap(sscreen, ps, Composite, saa_composite);
	saa_wrap(sscreen, ps, Glyphs,    miGlyphs);
	saa_wrap(sscreen, ps, UnrealizeGlyph, miUnrealizeGlyph);
    }
}

void
saa_render_takedown(ScreenPtr pScreen)
{
    PictureScreenPtr ps = GetPictureScreenIfSet(pScreen);
    struct saa_screen_priv *sscreen = saa_screen(pScreen);

    if (ps) {
	saa_unwrap(sscreen, ps, Trapezoids);
	saa_unwrap(sscreen, ps, Triangles);
	saa_unwrap(sscreen, ps, Composite);
	saa_unwrap(sscreen, ps, Glyphs);
	saa_unwrap(sscreen, ps, UnrealizeGlyph);
    }
}
#endif
