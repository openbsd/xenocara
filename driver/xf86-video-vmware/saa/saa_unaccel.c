/*
 * Copyright © 1999 Keith Packard
 * Copyright 2011 VMWare, Inc. All Rights Reserved.
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
 * Author: Based on "exa_unaccel.c"
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 */

#include "saa_priv.h"
#include "saa.h"
#include "mipict.h"

/**
 * Calls saa_prepare_access with SAA_ACCESS_R for the tile, if that is the
 * current fill style.
 *
 * Solid doesn't use an extra pixmap source, and Stippled/OpaqueStippled are
 * 1bpp and never in fb, so we don't worry about them.
 * We should worry about them for completeness sake and going forward.
 */
static Bool
saa_prepare_access_gc(GCPtr pGC)
{
    if (pGC->stipple)
	if (!saa_pad_read(&pGC->stipple->drawable))
	    return FALSE;
    if (pGC->fillStyle == FillTiled)
	if (!saa_pad_read(&pGC->tile.pixmap->drawable)) {
	    if (pGC->stipple)
		saa_fad_read(&pGC->stipple->drawable);
	    return FALSE;
	}
    return TRUE;
}

/**
 * Finishes access to the tile in the GC, if used.
 */
static void
saa_finish_access_gc(GCPtr pGC)
{
    if (pGC->fillStyle == FillTiled)
	saa_fad_read(&pGC->tile.pixmap->drawable);
    if (pGC->stipple)
	saa_fad_read(&pGC->stipple->drawable);
}

void
saa_check_fill_spans(DrawablePtr pDrawable, GCPtr pGC, int nspans,
		     DDXPointPtr ppt, int *pwidth, int fSorted)
{
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);
    struct saa_gc_priv *sgc = saa_gc(pGC);
    saa_access_t access;

    SAA_FALLBACK(("to %p (%c)\n", pDrawable, saa_drawable_location(pDrawable)));

    sscreen->fallback_count++;
    if (saa_pad_write(pDrawable, NULL, FALSE, &access)) {
	if (saa_prepare_access_gc(pGC)) {
	    saa_swap(sgc, pGC, ops);
	    pGC->ops->FillSpans(pDrawable, pGC, nspans, ppt, pwidth, fSorted);
	    saa_swap(sgc, pGC, ops);
	    saa_finish_access_gc(pGC);
	}
	saa_fad_write(pDrawable, access);
    }
    sscreen->fallback_count--;
}

static void
saa_check_set_spans(DrawablePtr pDrawable, GCPtr pGC, char *psrc,
		    DDXPointPtr ppt, int *pwidth, int nspans, int fSorted)
{
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);
    struct saa_gc_priv *sgc = saa_gc(pGC);
    saa_access_t access
	SAA_FALLBACK(("to %p (%c)\n", pDrawable, saa_drawable_loc(pDrawable)));

    sscreen->fallback_count++;
    if (saa_pad_write(pDrawable, NULL, FALSE, &access)) {
	saa_swap(sgc, pGC, ops);
	pGC->ops->SetSpans(pDrawable, pGC, psrc, ppt, pwidth, nspans, fSorted);
	saa_swap(sgc, pGC, ops);
	saa_fad_write(pDrawable, access);
    }
    sscreen->fallback_count--;
}

static void
saa_check_put_image(DrawablePtr pDrawable, GCPtr pGC, int depth,
		    int x, int y, int w, int h, int leftPad, int format,
		    char *bits)
{
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);
    struct saa_gc_priv *sgc = saa_gc(pGC);
    saa_access_t access;

    SAA_FALLBACK(("to %p (%c)\n", pDrawable, saa_drawable_loc(pDrawable)));
    sscreen->fallback_count++;
    if (saa_pad_write(pDrawable, pGC, TRUE, &access)) {
	saa_swap(sgc, pGC, ops);
	pGC->ops->PutImage(pDrawable, pGC, depth, x, y, w, h, leftPad,
			   format, bits);
	saa_swap(sgc, pGC, ops);
	saa_fad_write(pDrawable, access);
    }
    sscreen->fallback_count--;
}

RegionPtr
saa_boxes_to_region(ScreenPtr pScreen, int nbox, BoxPtr pbox, int ordering)
{
    xRectangle *rects = malloc(nbox * sizeof(*rects));
    int i;
    RegionPtr reg;

    if (!rects)
	return NULL;

    for (i = 0; i < nbox; i++) {
	rects[i].x = pbox[i].x1;
	rects[i].y = pbox[i].y1;
	rects[i].width = pbox[i].x2 - pbox[i].x1;
	rects[i].height = pbox[i].y2 - pbox[i].y1;
    }

    reg = RECTS_TO_REGION(pScreen, nbox, rects, ordering);
    free(rects);
    return reg;
}

void
saa_check_copy_nton(DrawablePtr pSrc, DrawablePtr pDst, GCPtr pGC,
		    BoxPtr pbox, int nbox, int dx, int dy, Bool reverse,
		    Bool upsidedown, Pixel bitplane, void *closure)
{
    ScreenPtr pScreen = pSrc->pScreen;
    struct saa_screen_priv *sscreen = saa_screen(pScreen);
    RegionPtr reg, readback;
    int src_xoff, src_yoff, dst_xoff, dst_yoff;
    struct saa_gc_priv *sgc = saa_gc(pGC);
    PixmapPtr src_pixmap;
    PixmapPtr dst_pixmap;
    saa_access_t access = SAA_ACCESS_R;
    int ordering;

    sscreen->fallback_count++;
    SAA_FALLBACK(("from %p to %p (%c,%c)\n", pSrc, pDst,
		  saa_drawable_loc(pSrc), saa_drawable_loc(pDst)));

    src_pixmap = saa_get_pixmap(pSrc, &src_xoff, &src_yoff);
    dst_pixmap = saa_get_pixmap(pDst, &dst_xoff, &dst_yoff);

    ordering = (nbox == 1 || (dx > 0 && dy > 0) ||
		(pDst != pSrc &&
		 (pDst->type != DRAWABLE_WINDOW ||
		  pSrc->type != DRAWABLE_WINDOW))) ? CT_YXBANDED : CT_UNSORTED;

    reg = saa_boxes_to_region(pScreen, nbox, pbox, ordering);
    if (!reg)
	return;

    REGION_TRANSLATE(pScreen, reg, src_xoff + dx, src_yoff + dy);
    if (!saa_prepare_access_pixmap(src_pixmap, SAA_ACCESS_R, reg))
	goto out_no_access;

    REGION_TRANSLATE(pScreen, reg, dst_xoff - dx - src_xoff,
		     dst_yoff - dy - src_yoff);

    if (saa_gc_reads_destination(pDst, pGC)) {
	readback = reg;
	access = SAA_ACCESS_RW;
    } else {
	readback = NULL;
	access = SAA_ACCESS_W;
    }

    if (!saa_prepare_access_pixmap(dst_pixmap, access, readback))
	goto out_no_dst;

    saa_swap(sgc, pGC, ops);
    while (nbox--) {
	pGC->ops->CopyArea(pSrc, pDst, pGC, pbox->x1 - pSrc->x + dx,
			   pbox->y1 - pSrc->y + dy,
			   pbox->x2 - pbox->x1, pbox->y2 - pbox->y1,
			   pbox->x1 - pDst->x, pbox->y1 - pDst->y);
	pbox++;
    }

    saa_swap(sgc, pGC, ops);
    saa_finish_access_pixmap(dst_pixmap, access);
    saa_pixmap_dirty(dst_pixmap, FALSE, reg);
 out_no_dst:
    saa_fad_read(pSrc);
 out_no_access:
    sscreen->fallback_count--;
    REGION_DESTROY(pScreen, reg);
}

RegionPtr
saa_check_copy_area(DrawablePtr pSrc, DrawablePtr pDst, GCPtr pGC,
		    int srcx, int srcy, int w, int h, int dstx, int dsty)
{
    RegionPtr ret = NULL;
    struct saa_gc_priv *sgc = saa_gc(pGC);
    saa_access_t access;
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);

    SAA_FALLBACK(("from %p to %p (%c,%c)\n", pSrc, pDst,
		  saa_drawable_loc(pSrc), saa_drawable_loc(pDst)));
    sscreen->fallback_count++;
    if (!saa_pad_read_box(pSrc, srcx, srcy, w, h))
	goto out_no_access;
    if (!saa_pad_write(pDst, pGC, TRUE, &access))
	goto out_no_dst;

    saa_swap(sgc, pGC, ops);
    ret = pGC->ops->CopyArea(pSrc, pDst, pGC, srcx, srcy, w, h, dstx, dsty);
    saa_swap(sgc, pGC, ops);

    saa_fad_write(pDst, access);
 out_no_dst:
    saa_fad_read(pSrc);
 out_no_access:
    sscreen->fallback_count--;

    return ret;
}

static RegionPtr
saa_check_copy_plane(DrawablePtr pSrc, DrawablePtr pDst, GCPtr pGC,
		     int srcx, int srcy, int w, int h, int dstx, int dsty,
		     unsigned long bitplane)
{
    RegionPtr ret = NULL;
    struct saa_gc_priv *sgc = saa_gc(pGC);
    saa_access_t access;
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);

    SAA_FALLBACK(("from %p to %p (%c,%c)\n", pSrc, pDst,
		  saa_drawable_loc(pSrc), saa_drawable_loc(pDst)));
    sscreen->fallback_count++;
    if (!saa_pad_read_box(pSrc, srcx, srcy, w, h))
	goto out_no_src;
    if (!saa_pad_write(pDst, pGC, TRUE, &access))
	goto out_no_dst;

    saa_swap(sgc, pGC, ops);
    ret = pGC->ops->CopyPlane(pSrc, pDst, pGC, srcx, srcy, w, h, dstx, dsty,
			      bitplane);
    saa_swap(sgc, pGC, ops);

    saa_fad_write(pDst, access);
 out_no_dst:
    saa_fad_read(pSrc);
 out_no_src:
    sscreen->fallback_count--;

    return ret;
}

static void
saa_check_poly_point(DrawablePtr pDrawable, GCPtr pGC, int mode, int npt,
		     DDXPointPtr pptInit)
{
    struct saa_gc_priv *sgc = saa_gc(pGC);
    saa_access_t access;
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);

    sscreen->fallback_count++;
    SAA_FALLBACK(("to %p (%c)\n", pDrawable, saa_drawable_loc(pDrawable)));
    if (!saa_pad_write(pDrawable, NULL, FALSE, &access))
	goto out_no_access;
    saa_swap(sgc, pGC, ops);
    pGC->ops->PolyPoint(pDrawable, pGC, mode, npt, pptInit);
    saa_swap(sgc, pGC, ops);
    saa_fad_write(pDrawable, access);

 out_no_access:
    sscreen->fallback_count--;
}

static void
saa_check_poly_lines(DrawablePtr pDrawable, GCPtr pGC,
		     int mode, int npt, DDXPointPtr ppt)
{
    struct saa_gc_priv *sgc = saa_gc(pGC);
    saa_access_t access;
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);

    SAA_FALLBACK(("to %p (%c), width %d, mode %d, count %d\n",
		  pDrawable, saa_drawable_loc(pDrawable),
		  pGC->lineWidth, mode, npt));

    sscreen->fallback_count++;
    if (!saa_pad_write(pDrawable, NULL, FALSE, &access))
	goto out_no_access;
    if (!saa_prepare_access_gc(pGC))
	goto out_no_gc;
    saa_swap(sgc, pGC, ops);
    pGC->ops->Polylines(pDrawable, pGC, mode, npt, ppt);
    saa_swap(sgc, pGC, ops);
    saa_finish_access_gc(pGC);
 out_no_gc:
    saa_fad_write(pDrawable, access);
 out_no_access:
    sscreen->fallback_count--;
}

static void
saa_check_poly_segment(DrawablePtr pDrawable, GCPtr pGC,
		       int nsegInit, xSegment * pSegInit)
{
    struct saa_gc_priv *sgc = saa_gc(pGC);
    saa_access_t access;
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);

    SAA_FALLBACK(("to %p (%c) width %d, count %d\n", pDrawable,
		  saa_drawable_loc(pDrawable), pGC->lineWidth, nsegInit));

    sscreen->fallback_count++;
    if (!saa_pad_write(pDrawable, NULL, FALSE, &access))
	goto out_no_access;;
    if (!saa_prepare_access_gc(pGC))
	goto out_no_gc;
    saa_swap(sgc, pGC, ops);
    pGC->ops->PolySegment(pDrawable, pGC, nsegInit, pSegInit);
    saa_swap(sgc, pGC, ops);
    saa_finish_access_gc(pGC);
 out_no_gc:
    saa_fad_write(pDrawable, access);
 out_no_access:
    sscreen->fallback_count--;
}

static void
saa_check_poly_arc(DrawablePtr pDrawable, GCPtr pGC, int narcs, xArc * pArcs)
{
    struct saa_gc_priv *sgc = saa_gc(pGC);
    saa_access_t access;
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);

    SAA_FALLBACK(("to %p (%c)\n", pDrawable, saa_drawable_loc(pDrawable)));

    sscreen->fallback_count++;
    if (!saa_pad_write(pDrawable, NULL, FALSE, &access))
	goto out_no_access;;
    if (!saa_prepare_access_gc(pGC))
	goto out_no_gc;
    saa_swap(sgc, pGC, ops);
    pGC->ops->PolyArc(pDrawable, pGC, narcs, pArcs);
    saa_swap(sgc, pGC, ops);
    saa_finish_access_gc(pGC);
 out_no_gc:
    saa_fad_write(pDrawable, access);
 out_no_access:
    sscreen->fallback_count--;
}


/**
 * saa_check_poly_fill_rect_noreadback - PolyFillRect avoiding unnecessary readbacks.
 *
 * @pDrawable: The drawable on which to fill.
 * @pGC: Pointer to the GC to use.
 * @nrect: Number of rectangles to fill.
 * @xRectangle: Pointer to rectangles to fill.
 *
 * During a standard saa polyFillRect, the damage region is usually the bounding
 * box of all rectangles. Since we mark the software pixmap dirty based on that
 * damage region, we need to read all of it back first, even if the fill operation
 * itself doesn't read anything. This version of polyFillRect improves on that by
 * only damaging the area we actually fill. If it's a non-reading fill we thus don't
 * need to read back anything, but this may come at the cost of increased dirty
 * region fragmentation. In any case, this greatly improves on the performance of
 * shaped windows on top of accelerated contents, for example unscaled OSD in xine.
 */
static Bool
saa_check_poly_fill_rect_noreadback(DrawablePtr pDrawable, GCPtr pGC,
				    int nrect, xRectangle *prect)
{
    struct saa_gc_priv *sgc = saa_gc(pGC);
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);
    RegionPtr region;
    saa_access_t access;
    Bool ret;
    PixmapPtr pPixmap;
    xRectangle *prect_save = prect;
    int xoff, yoff;
    struct saa_pixmap *spix;

    if (!nrect)
	return TRUE;

    sscreen->fallback_count++;

    pPixmap = saa_get_pixmap(pDrawable, &xoff, &yoff);
    spix = saa_get_saa_pixmap(pPixmap);
    region = RECTS_TO_REGION(pGC->pScreen, nrect, prect, CT_UNSORTED);
    if (!region)
	goto out_no_region;

    REGION_TRANSLATE(pGC->pScreen, region, pDrawable->x, pDrawable->y);
    REGION_INTERSECT(pGC->pScreen, region, fbGetCompositeClip(pGC), region);
    REGION_TRANSLATE(pGC->pScreen, region, xoff, yoff);

    access = SAA_ACCESS_W;
    if (saa_gc_reads_destination(pDrawable, pGC)) {
	/*
	 * We need to do a readback anyway. In case of more than an
	 * ad hoc number of say 4 rectangles, we might as well do a
	 * readback of the whole damage area to avoid fragmentation.
	 */
	if (REGION_NUM_RECTS(region) > 4)
	    goto out_no_access;

	access |= SAA_ACCESS_R;
	ret = saa_prepare_access_pixmap(pPixmap, access, region);
    } else
	ret = saa_prepare_access_pixmap(pPixmap, access, NULL);

    if (!ret)
	goto out_no_access;

    if (!saa_prepare_access_gc(pGC))
	goto out_no_gc;

    saa_swap(sgc, pGC, ops);
    pGC->ops->PolyFillRect(pDrawable, pGC, nrect, prect_save);
    saa_swap(sgc, pGC, ops);

    saa_finish_access_gc(pGC);
    saa_finish_access_pixmap(pPixmap, access);

    if (spix->damage) {
	REGION_INTERSECT(pGC->pScreen, region, region,
			 saa_pix_damage_pending(spix));
	saa_pixmap_dirty(pPixmap, FALSE, region);
    }

    REGION_DESTROY(pGC->pScreen, region);

    sscreen->fallback_count--;

    return TRUE;

  out_no_gc:
    saa_finish_access_pixmap(pPixmap, access);
  out_no_access:
    REGION_DESTROY(pGC->pScreen, region);
  out_no_region:
    sscreen->fallback_count--;

    return FALSE;
}

void
saa_check_poly_fill_rect(DrawablePtr pDrawable, GCPtr pGC,
			 int nrect, xRectangle * prect)
{
    struct saa_gc_priv *sgc = saa_gc(pGC);
    saa_access_t access;
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);

    SAA_FALLBACK(("to %p (%c)\n", pDrawable, saa_drawable_loc(pDrawable)));

    if (saa_check_poly_fill_rect_noreadback(pDrawable, pGC, nrect, prect))
	return;

    sscreen->fallback_count++;

    /*
     * TODO: Use @prect for readback / damaging instead of
     * the damage region. This may fragment the dirty regions more
     * but should avoid unnecessary readbacks.
     */
    if (!saa_pad_write(pDrawable, pGC, FALSE, &access))
	goto out_no_access;;
    if (!saa_prepare_access_gc(pGC))
	goto out_no_gc;
    saa_swap(sgc, pGC, ops);
    pGC->ops->PolyFillRect(pDrawable, pGC, nrect, prect);
    saa_swap(sgc, pGC, ops);
    saa_finish_access_gc(pGC);
 out_no_gc:
    saa_fad_write(pDrawable, access);
 out_no_access:
    sscreen->fallback_count--;
}

static void
saa_check_image_glyph_blt(DrawablePtr pDrawable, GCPtr pGC,
			  int x, int y, unsigned int nglyph,
			  CharInfoPtr * ppci, pointer pglyphBase)
{
    struct saa_gc_priv *sgc = saa_gc(pGC);
    saa_access_t access;
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);

    SAA_FALLBACK(("to %p (%c)\n", pDrawable, saa_drawable_loc(pDrawable)));

    sscreen->fallback_count++;
    if (!saa_pad_write(pDrawable, NULL, FALSE, &access))
	goto out_no_access;;
    if (!saa_prepare_access_gc(pGC))
	goto out_no_gc;
    saa_swap(sgc, pGC, ops);
    pGC->ops->ImageGlyphBlt(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase);
    saa_swap(sgc, pGC, ops);
    saa_finish_access_gc(pGC);
 out_no_gc:
    saa_fad_write(pDrawable, access);
 out_no_access:
    sscreen->fallback_count--;
}

static void
saa_check_poly_glyph_blt(DrawablePtr pDrawable, GCPtr pGC,
			 int x, int y, unsigned int nglyph,
			 CharInfoPtr * ppci, pointer pglyphBase)
{
    struct saa_gc_priv *sgc = saa_gc(pGC);
    saa_access_t access;
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);

    SAA_FALLBACK(("to %p (%c), style %d alu %d\n", pDrawable,
		  saa_drawable_loc(pDrawable), pGC->fillStyle, pGC->alu));

    sscreen->fallback_count++;
    if (!saa_pad_write(pDrawable, NULL, FALSE, &access))
	goto out_no_access;;
    if (!saa_prepare_access_gc(pGC))
	goto out_no_gc;
    saa_swap(sgc, pGC, ops);
    pGC->ops->PolyGlyphBlt(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase);
    saa_swap(sgc, pGC, ops);
    saa_finish_access_gc(pGC);
 out_no_gc:
    saa_fad_write(pDrawable, access);
 out_no_access:
    sscreen->fallback_count--;
}

static void
saa_check_push_pixels(GCPtr pGC, PixmapPtr pBitmap,
		      DrawablePtr pDrawable, int w, int h, int x, int y)
{
    struct saa_gc_priv *sgc = saa_gc(pGC);
    saa_access_t access;
    struct saa_screen_priv *sscreen = saa_screen(pGC->pScreen);

    SAA_FALLBACK(("from %p to %p (%c,%c)\n", pBitmap, pDrawable,
		  saa_drawable_loc(&pBitmap->drawable),
		  saa_drawable_loc(pDrawable)));

    sscreen->fallback_count++;
    if (!saa_pad_write(pDrawable, pGC, TRUE, &access))
	goto out_no_access;;
    if (!saa_pad_read_box(&pBitmap->drawable, 0, 0, w, h))
	goto out_no_src;
    if (!saa_prepare_access_gc(pGC))
	goto out_no_gc;
    saa_swap(sgc, pGC, ops);
    pGC->ops->PushPixels(pGC, pBitmap, pDrawable, w, h, x, y);
    saa_swap(sgc, pGC, ops);
    saa_finish_access_gc(pGC);
 out_no_gc:
    saa_fad_read(&pBitmap->drawable);
 out_no_src:
    saa_fad_write(pDrawable, access);
 out_no_access:
    sscreen->fallback_count--;
}

static void
saa_check_copy_window(WindowPtr pWin, DDXPointRec ptOldOrg, RegionPtr prgnSrc)
{
    DrawablePtr pDrawable = &pWin->drawable;
    ScreenPtr pScreen = pDrawable->pScreen;
    struct saa_screen_priv *sscreen = saa_screen(pScreen);
    int xoff, yoff;
    PixmapPtr pPixmap = saa_get_pixmap(&pWin->drawable, &xoff, &yoff);
    Bool ret;

    SAA_FALLBACK(("from %p\n", pWin));

    /* Only need the source bits, the destination region will be overwritten */

    sscreen->fallback_count++;
    REGION_TRANSLATE(pScreen, prgnSrc, xoff, yoff);
    ret = saa_prepare_access_pixmap(pPixmap, SAA_ACCESS_R, prgnSrc);
    REGION_TRANSLATE(pScreen, prgnSrc, -xoff, -yoff);
    if (!ret)
	goto out_no_access;;

    if (saa_prepare_access_pixmap(pPixmap, SAA_ACCESS_W, NULL)) {
	struct saa_pixmap *spix;
	RegionRec rgnDst;

	REGION_NULL(pScreen, &rgnDst);
	REGION_COPY(pScreen, &rgnDst, prgnSrc);

	saa_swap(sscreen, pScreen, CopyWindow);
	pScreen->CopyWindow(pWin, ptOldOrg, prgnSrc);
	saa_swap(sscreen, pScreen, CopyWindow);
	saa_finish_access_pixmap(pPixmap, SAA_ACCESS_W);

	spix = saa_get_saa_pixmap(pPixmap);
	if (spix->damage) {
	    int dx, dy;

	    dx = ptOldOrg.x - pWin->drawable.x;
	    dy = ptOldOrg.y - pWin->drawable.y;
	    REGION_TRANSLATE(pScreen, &rgnDst, -dx, -dy);
	    REGION_INTERSECT(pSreen, &rgnDst, &pWin->borderClip, &rgnDst);
	    REGION_TRANSLATE(pScreen, &rgnDst, xoff, yoff);

	    REGION_INTERSECT(pScreen, &rgnDst, &rgnDst,
			     saa_pix_damage_pending(spix));
	    saa_pixmap_dirty(pPixmap, FALSE, &rgnDst);
	}
	REGION_UNINIT(pScreen, &rgnDst);
    }
    saa_fad_read(pDrawable);
 out_no_access:
    sscreen->fallback_count--;
}

#ifdef RENDER

#if (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 10)
static void
saa_src_validate(DrawablePtr pDrawable,
		 int x,
		 int y, int width, int height, unsigned int subWindowMode)
#else
static void
saa_src_validate(DrawablePtr pDrawable, int x, int y, int width, int height)
#endif
{
    ScreenPtr pScreen = pDrawable->pScreen;
    struct saa_screen_priv *sscreen = saa_screen(pScreen);
    int xoff, yoff;
    BoxRec box;
    RegionRec reg;
    RegionPtr dst;

    (void) saa_get_pixmap(pDrawable, &xoff, &yoff);
    box.x1 = x + xoff;
    box.y1 = y + yoff;
    box.x2 = box.x1 + width;
    box.y2 = box.y1 + height;

    dst = (sscreen->srcDraw == pDrawable) ?
	&sscreen->srcReg : &sscreen->maskReg;

    REGION_INIT(pScreen, &reg, &box, 1);
    REGION_UNION(pScreen, dst, dst, &reg);
    REGION_UNINIT(pScreen, &reg);

    if (sscreen->saved_SourceValidate) {
	saa_swap(sscreen, pScreen, SourceValidate);
	pScreen->SourceValidate(pDrawable, x, y, width, height
#if (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 10)
				, subWindowMode
#endif
	    );
	saa_swap(sscreen, pScreen, SourceValidate);
    }
}

static void
saa_check_get_image(DrawablePtr pDrawable, int x, int y, int w, int h,
		    unsigned int format, unsigned long planeMask, char *d)
{
    ScreenPtr pScreen = pDrawable->pScreen;
    struct saa_screen_priv *sscreen = saa_screen(pScreen);

    SAA_FALLBACK(("from %p (%c)\n", pDrawable, saa_drawable_loc(pDrawable)));

    sscreen->fallback_count++;
    if (!saa_pad_read_box(pDrawable, x, y, w, h))
	goto out_no_access;;
    saa_swap(sscreen, pScreen, GetImage);
    pScreen->GetImage(pDrawable, x, y, w, h, format, planeMask, d);
    saa_swap(sscreen, pScreen, GetImage);
    saa_fad_read(pDrawable);
 out_no_access:
    sscreen->fallback_count--;
}

static void
saa_check_get_spans(DrawablePtr pDrawable,
		    int wMax,
		    DDXPointPtr ppt, int *pwidth, int nspans, char *pdstStart)
{
    ScreenPtr pScreen = pDrawable->pScreen;
    struct saa_screen_priv *sscreen = saa_screen(pScreen);

    SAA_FALLBACK(("from %p (%c)\n", pDrawable, saa_drawable_loc(pDrawable)));

    sscreen->fallback_count++;
    if (!saa_pad_read(pDrawable))
	goto out_no_access;;
    saa_swap(sscreen, pScreen, GetSpans);
    pScreen->GetSpans(pDrawable, wMax, ppt, pwidth, nspans, pdstStart);
    saa_swap(sscreen, pScreen, GetSpans);
    saa_fad_read(pDrawable);
 out_no_access:
    sscreen->fallback_count--;
}

/*
 * Compute composite regions taking transforms into account.
 * The caller must provide a pointer to an initialized dst_reg,
 * and the function returns pointers to set up source- and mask regions.
 * The source and mask regions must be uninitialized after use.
 */

Bool
saa_compute_composite_regions(ScreenPtr pScreen,
			      PicturePtr pSrc,
			      PicturePtr pMask,
			      PicturePtr pDst,
			      INT16 xSrc, INT16 ySrc, INT16 xMask,
			      INT16 yMask, INT16 xDst,
			      INT16 yDst, INT16 width, INT16 height,
			      RegionPtr dst_reg,
			      RegionPtr *src_reg,
			      RegionPtr *mask_reg)
{
    struct saa_screen_priv *sscreen = saa_screen(pScreen);
    RegionPtr srcReg = NULL;
    RegionPtr maskReg = NULL;
    Bool ret;
    int xoff, yoff;

    *src_reg = NULL;
    *mask_reg = NULL;

    if (pSrc->pDrawable) {
	REGION_NULL(pScreen, &sscreen->srcReg);
	srcReg = &sscreen->srcReg;
	sscreen->srcDraw = pSrc->pDrawable;
	if (pSrc != pDst)
	    REGION_TRANSLATE(pScreen, pSrc->pCompositeClip,
			     -pSrc->pDrawable->x, -pSrc->pDrawable->y);
    }

    if (pMask && pMask->pDrawable) {
	REGION_NULL(pScreen, &sscreen->maskReg);
	maskReg = &sscreen->maskReg;
	if (pMask != pDst && pMask != pSrc)
	    REGION_TRANSLATE(pScreen, pMask->pCompositeClip,
			     -pMask->pDrawable->x, -pMask->pDrawable->y);
    }

    REGION_TRANSLATE(pScreen, pDst->pCompositeClip,
		     -pDst->pDrawable->x, -pDst->pDrawable->y);

    sscreen->saved_SourceValidate = saa_src_validate;
    saa_swap(sscreen, pScreen, SourceValidate);
    ret = miComputeCompositeRegion(dst_reg, pSrc, pMask, pDst,
				   xSrc, ySrc, xMask, yMask,
				   xDst, yDst, width, height);
    saa_swap(sscreen, pScreen, SourceValidate);

    REGION_TRANSLATE(pScreen, pDst->pCompositeClip,
		     pDst->pDrawable->x, pDst->pDrawable->y);
    if (pSrc->pDrawable && pSrc != pDst)
	REGION_TRANSLATE(pScreen, pSrc->pCompositeClip,
			 pSrc->pDrawable->x, pSrc->pDrawable->y);
    if (pMask && pMask->pDrawable && pMask != pDst && pMask != pSrc)
	REGION_TRANSLATE(pScreen, pMask->pCompositeClip,
			 pMask->pDrawable->x, pMask->pDrawable->y);

    if (!ret) {
	if (srcReg)
	    REGION_UNINIT(pScreen, srcReg);
	if (maskReg)
	    REGION_UNINIT(pScreen, maskReg);

	return FALSE;
    }

    *src_reg = srcReg;
    *mask_reg = maskReg;

    /*
     * Translate dst region to pixmap space.
     */
    (void) saa_get_pixmap(pDst->pDrawable, &xoff, &yoff);
    REGION_TRANSLATE(pScreen, dst_reg, pDst->pDrawable->x + xoff,
		     pDst->pDrawable->y + yoff);


    return TRUE;
}

static Bool
saa_prepare_composite_reg(ScreenPtr pScreen,
			  CARD8 op,
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
			  CARD16 height,
			  RegionPtr src_region,
			  RegionPtr mask_region,
			  RegionPtr dst_region,
			  saa_access_t * access)
{
    RegionPtr dstReg = NULL;
    PixmapPtr pSrcPix = NULL;
    PixmapPtr pMaskPix = NULL;
    PixmapPtr pDstPix;
    struct saa_pixmap *dst_spix;

    *access = SAA_ACCESS_W;

    if (pSrc->pDrawable)
	pSrcPix = saa_get_drawable_pixmap(pSrc->pDrawable);
    if (pMask && pMask->pDrawable)
	pMaskPix = saa_get_drawable_pixmap(pMask->pDrawable);

    /*
     * Don't limit alphamaps readbacks for now until we've figured out how that
     * should be done.
     */

    if (pSrc->alphaMap && pSrc->alphaMap->pDrawable)
	if (!saa_pad_read(pSrc->alphaMap->pDrawable))
	    goto out_no_src_alpha;
    if (pMask && pMask->alphaMap && pMask->alphaMap->pDrawable)
	if (!saa_pad_read(pMask->alphaMap->pDrawable))
	    goto out_no_mask_alpha;
    if (pSrcPix)
	if (!saa_prepare_access_pixmap(pSrcPix, SAA_ACCESS_R, src_region))
	    goto out_no_src;
    if (pMaskPix)
	if (!saa_prepare_access_pixmap(pMaskPix, SAA_ACCESS_R, mask_region))
	    goto out_no_mask;

    pDstPix = saa_get_drawable_pixmap(pDst->pDrawable);
    dst_spix = saa_get_saa_pixmap(pDstPix);

    if (dst_spix->damage && saa_op_reads_destination(op)) {
	dstReg = dst_region;
	*access |= SAA_ACCESS_R;
    }

    if (pDst->alphaMap && pDst->alphaMap->pDrawable)
	if (!saa_prepare_access_pixmap
	    (saa_get_drawable_pixmap(pDst->alphaMap->pDrawable),
	     *access, dstReg))
	    goto out_no_dst_alpha;

    if (!saa_prepare_access_pixmap(pDstPix, *access, dstReg))
	goto out_no_dst;

    return TRUE;

 out_no_dst:
    LogMessage(X_ERROR, "No dst\n");
    saa_finish_access_pixmap
	(saa_get_drawable_pixmap(pDst->alphaMap->pDrawable), *access);
 out_no_dst_alpha:
    LogMessage(X_ERROR, "No dst alpha\n");
    if (pMaskPix)
	saa_finish_access_pixmap(pMaskPix, SAA_ACCESS_R);
 out_no_mask:
    LogMessage(X_ERROR, "No mask\n");
    if (pSrcPix)
	saa_finish_access_pixmap(pSrcPix, SAA_ACCESS_R);
 out_no_src:
    LogMessage(X_ERROR, "No src\n");
    if (pMask && pMask->alphaMap && pMask->alphaMap->pDrawable)
	saa_fad_read(pMask->alphaMap->pDrawable);
 out_no_mask_alpha:
    LogMessage(X_ERROR, "No mask alpha\n");
    if (pSrc && pSrc->alphaMap && pSrc->alphaMap->pDrawable)
	saa_fad_read(pSrc->alphaMap->pDrawable);
 out_no_src_alpha:
    LogMessage(X_ERROR, "No src alpha\n");
    return FALSE;

}

void
saa_check_composite(CARD8 op,
		    PicturePtr pSrc,
		    PicturePtr pMask,
		    PicturePtr pDst,
		    INT16 xSrc,
		    INT16 ySrc,
		    INT16 xMask,
		    INT16 yMask,
		    INT16 xDst, INT16 yDst, CARD16 width, CARD16 height,
		    RegionPtr src_region,
		    RegionPtr mask_region,
		    RegionPtr dst_region)
{
    ScreenPtr pScreen = pDst->pDrawable->pScreen;
    PictureScreenPtr ps = GetPictureScreen(pScreen);
    struct saa_screen_priv *sscreen = saa_screen(pScreen);
    saa_access_t access;
    PixmapPtr pixmap;

    sscreen->fallback_count++;
    if (!saa_prepare_composite_reg(pScreen, op, pSrc, pMask, pDst, xSrc,
				   ySrc, xMask, yMask, xDst, yDst, width,
				   height,
				   src_region,
				   mask_region,
				   dst_region,
				   &access)) {
	goto out_no_access;;
    }

    saa_swap(sscreen, ps, Composite);
    ps->Composite(op,
		  pSrc,
		  pMask,
		  pDst, xSrc, ySrc, xMask, yMask, xDst, yDst, width, height);
    saa_swap(sscreen, ps, Composite);
    if (pMask && pMask->pDrawable != NULL)
	saa_fad_read(pMask->pDrawable);
    if (pSrc->pDrawable != NULL)
	saa_fad_read(pSrc->pDrawable);
    pixmap = saa_get_drawable_pixmap(pDst->pDrawable);
    saa_finish_access_pixmap(pixmap, access);
    saa_pixmap_dirty(pixmap, FALSE, dst_region);
    if (pDst->alphaMap && pDst->alphaMap->pDrawable) {
	pixmap = saa_get_drawable_pixmap(pDst->alphaMap->pDrawable);
	saa_finish_access_pixmap(pixmap, access);
	saa_pixmap_dirty(pixmap, FALSE, dst_region);
    }
    if (pSrc->alphaMap && pSrc->alphaMap->pDrawable)
	saa_fad_read(pSrc->alphaMap->pDrawable);
    if (pMask && pMask->alphaMap && pMask->alphaMap->pDrawable)
	saa_fad_read(pMask->alphaMap->pDrawable);
 out_no_access:
    sscreen->fallback_count--;
}

static void
saa_check_add_traps(PicturePtr pPicture,
		    INT16 x_off, INT16 y_off, int ntrap, xTrap * traps)
{
    ScreenPtr pScreen = pPicture->pDrawable->pScreen;
    PictureScreenPtr ps = GetPictureScreen(pScreen);
    struct saa_screen_priv *sscreen = saa_screen(pScreen);
    saa_access_t access;

    SAA_FALLBACK(("to pict %p (%c)\n", saa_drawable_loc(pPicture->pDrawable)));

    sscreen->fallback_count++;
    if (!saa_pad_write(pPicture->pDrawable, NULL, FALSE, &access))
	goto out_no_access;
    saa_swap(sscreen, ps, AddTraps);
    ps->AddTraps(pPicture, x_off, y_off, ntrap, traps);
    saa_swap(sscreen, ps, AddTraps);
    saa_fad_write(pPicture->pDrawable, access);
 out_no_access:
    sscreen->fallback_count--;
}

#endif

void
saa_unaccel_setup(ScreenPtr pScreen)
{
#ifdef RENDER
    PictureScreenPtr ps = GetPictureScreenIfSet(pScreen);
#endif
    struct saa_screen_priv *sscreen = saa_screen(pScreen);

    saa_wrap(sscreen, pScreen, GetImage, saa_check_get_image);
    saa_wrap(sscreen, pScreen, GetSpans, saa_check_get_spans);
    saa_wrap(sscreen, pScreen, CopyWindow, saa_check_copy_window);

#ifdef RENDER
    if (ps) {
	saa_wrap(sscreen, ps, AddTraps, saa_check_add_traps);
    }
#endif
}

void
saa_unaccel_takedown(ScreenPtr pScreen)
{
#ifdef RENDER
    PictureScreenPtr ps = GetPictureScreenIfSet(pScreen);
#endif
    struct saa_screen_priv *sscreen = saa_screen(pScreen);

    saa_unwrap(sscreen, pScreen, GetImage);
    saa_unwrap(sscreen, pScreen, GetSpans);
    saa_unwrap(sscreen, pScreen, CopyWindow);

#ifdef RENDER
    if (ps) {
	saa_unwrap(sscreen, ps, AddTraps);
    }
#endif
}

GCOps saa_gc_ops = {
    saa_check_fill_spans,
    saa_check_set_spans,
    saa_check_put_image,
    saa_copy_area,
    saa_check_copy_plane,
    saa_check_poly_point,
    saa_check_poly_lines,
    saa_check_poly_segment,
    miPolyRectangle,
    saa_check_poly_arc,
    miFillPolygon,
    saa_check_poly_fill_rect,
    miPolyFillArc,
    miPolyText8,
    miPolyText16,
    miImageText8,
    miImageText16,
    saa_check_image_glyph_blt,
    saa_check_poly_glyph_blt,
    saa_check_push_pixels,
};
