/*
 *
 * Copyright © 1999 Keith Packard
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

#include "uxa-priv.h"

#ifdef RENDER
#include "mipict.h"
#endif

/*
 * These functions wrap the low-level fb rendering functions and
 * synchronize framebuffer/accelerated drawing by stalling until
 * the accelerator is idle
 */

/**
 * Calls uxa_prepare_access with UXA_PREPARE_SRC for the tile, if that is the
 * current fill style.
 *
 * Solid doesn't use an extra pixmap source, and Stippled/OpaqueStippled are
 * 1bpp and never in fb, so we don't worry about them.
 * We should worry about them for completeness sake and going forward.
 */
Bool uxa_prepare_access_gc(GCPtr pGC)
{
	if (pGC->stipple)
		if (!uxa_prepare_access(&pGC->stipple->drawable, UXA_ACCESS_RO))
			return FALSE;
	if (pGC->fillStyle == FillTiled)
		if (!uxa_prepare_access
		    (&pGC->tile.pixmap->drawable, UXA_ACCESS_RO)) {
			if (pGC->stipple)
				uxa_finish_access(&pGC->stipple->drawable);
			return FALSE;
		}
	return TRUE;
}

/**
 * Finishes access to the tile in the GC, if used.
 */
void uxa_finish_access_gc(GCPtr pGC)
{
	if (pGC->fillStyle == FillTiled)
		uxa_finish_access(&pGC->tile.pixmap->drawable);
	if (pGC->stipple)
		uxa_finish_access(&pGC->stipple->drawable);
}

Bool uxa_picture_prepare_access(PicturePtr picture, int mode)
{
	if (picture->pDrawable == NULL)
		return TRUE;

	if (!uxa_prepare_access(picture->pDrawable, mode))
		return FALSE;

	if (picture->alphaMap &&
	    !uxa_prepare_access(picture->alphaMap->pDrawable, mode)) {
		uxa_finish_access(picture->pDrawable);
		return FALSE;
	}

	return TRUE;
}

void uxa_picture_finish_access(PicturePtr picture)
{
	if (picture->pDrawable == NULL)
		return;

	uxa_finish_access(picture->pDrawable);
	if (picture->alphaMap)
		uxa_finish_access(picture->alphaMap->pDrawable);
}


char uxa_drawable_location(DrawablePtr pDrawable)
{
	return uxa_drawable_is_offscreen(pDrawable) ? 's' : 'm';
}

void
uxa_check_fill_spans(DrawablePtr pDrawable, GCPtr pGC, int nspans,
		     DDXPointPtr ppt, int *pwidth, int fSorted)
{
	ScreenPtr screen = pDrawable->pScreen;

	UXA_FALLBACK(("to %p (%c)\n", pDrawable,
		      uxa_drawable_location(pDrawable)));
	if (uxa_prepare_access(pDrawable, UXA_ACCESS_RW)) {
		if (uxa_prepare_access_gc(pGC)) {
			fbFillSpans(pDrawable, pGC, nspans, ppt, pwidth,
				    fSorted);
			uxa_finish_access_gc(pGC);
		}
		uxa_finish_access(pDrawable);
	}
}

void
uxa_check_set_spans(DrawablePtr pDrawable, GCPtr pGC, char *psrc,
		    DDXPointPtr ppt, int *pwidth, int nspans, int fSorted)
{
	ScreenPtr screen = pDrawable->pScreen;

	UXA_FALLBACK(("to %p (%c)\n", pDrawable,
		      uxa_drawable_location(pDrawable)));
	if (uxa_prepare_access(pDrawable, UXA_ACCESS_RW)) {
		fbSetSpans(pDrawable, pGC, psrc, ppt, pwidth, nspans, fSorted);
		uxa_finish_access(pDrawable);
	}
}

void
uxa_check_put_image(DrawablePtr pDrawable, GCPtr pGC, int depth,
		    int x, int y, int w, int h, int leftPad, int format,
		    char *bits)
{
	ScreenPtr screen = pDrawable->pScreen;

	UXA_FALLBACK(("to %p (%c)\n", pDrawable,
		      uxa_drawable_location(pDrawable)));
	if (uxa_prepare_access(pDrawable, UXA_ACCESS_RW)) {
		fbPutImage(pDrawable, pGC, depth, x, y, w, h, leftPad, format,
			   bits);
		uxa_finish_access(pDrawable);
	}
}

RegionPtr
uxa_check_copy_area(DrawablePtr pSrc, DrawablePtr pDst, GCPtr pGC,
		    int srcx, int srcy, int w, int h, int dstx, int dsty)
{
	ScreenPtr screen = pSrc->pScreen;
	RegionPtr ret = NULL;

	UXA_FALLBACK(("from %p to %p (%c,%c)\n", pSrc, pDst,
		      uxa_drawable_location(pSrc),
		      uxa_drawable_location(pDst)));
	if (uxa_prepare_access(pDst, UXA_ACCESS_RW)) {
		if (uxa_prepare_access(pSrc, UXA_ACCESS_RO)) {
			ret =
			    fbCopyArea(pSrc, pDst, pGC, srcx, srcy, w, h, dstx,
				       dsty);
			uxa_finish_access(pSrc);
		}
		uxa_finish_access(pDst);
	}
	return ret;
}

RegionPtr
uxa_check_copy_plane(DrawablePtr pSrc, DrawablePtr pDst, GCPtr pGC,
		     int srcx, int srcy, int w, int h, int dstx, int dsty,
		     unsigned long bitPlane)
{
	ScreenPtr screen = pSrc->pScreen;
	RegionPtr ret = NULL;

	UXA_FALLBACK(("from %p to %p (%c,%c)\n", pSrc, pDst,
		      uxa_drawable_location(pSrc),
		      uxa_drawable_location(pDst)));
	if (uxa_prepare_access(pDst, UXA_ACCESS_RW)) {
		if (uxa_prepare_access(pSrc, UXA_ACCESS_RO)) {
			ret =
			    fbCopyPlane(pSrc, pDst, pGC, srcx, srcy, w, h, dstx,
					dsty, bitPlane);
			uxa_finish_access(pSrc);
		}
		uxa_finish_access(pDst);
	}
	return ret;
}

void
uxa_check_poly_point(DrawablePtr pDrawable, GCPtr pGC, int mode, int npt,
		     DDXPointPtr pptInit)
{
	ScreenPtr screen = pDrawable->pScreen;

	UXA_FALLBACK(("to %p (%c)\n", pDrawable,
		      uxa_drawable_location(pDrawable)));
	if (uxa_prepare_access(pDrawable, UXA_ACCESS_RW)) {
		fbPolyPoint(pDrawable, pGC, mode, npt, pptInit);
		uxa_finish_access(pDrawable);
	}
}

void
uxa_check_poly_lines(DrawablePtr pDrawable, GCPtr pGC,
		     int mode, int npt, DDXPointPtr ppt)
{
	ScreenPtr screen = pDrawable->pScreen;

	UXA_FALLBACK(("to %p (%c), width %d, mode %d, count %d\n",
		      pDrawable, uxa_drawable_location(pDrawable),
		      pGC->lineWidth, mode, npt));

	if (pGC->lineWidth == 0) {
		if (uxa_prepare_access(pDrawable, UXA_ACCESS_RW)) {
			if (uxa_prepare_access_gc(pGC)) {
				fbPolyLine(pDrawable, pGC, mode, npt, ppt);
				uxa_finish_access_gc(pGC);
			}
			uxa_finish_access(pDrawable);
		}
		return;
	}
	/* fb calls mi functions in the lineWidth != 0 case. */
	fbPolyLine(pDrawable, pGC, mode, npt, ppt);
}

void
uxa_check_poly_segment(DrawablePtr pDrawable, GCPtr pGC,
		       int nsegInit, xSegment * pSegInit)
{
	ScreenPtr screen = pDrawable->pScreen;

	UXA_FALLBACK(("to %p (%c) width %d, count %d\n", pDrawable,
		      uxa_drawable_location(pDrawable), pGC->lineWidth,
		      nsegInit));
	if (pGC->lineWidth == 0) {
		if (uxa_prepare_access(pDrawable, UXA_ACCESS_RW)) {
			if (uxa_prepare_access_gc(pGC)) {
				fbPolySegment(pDrawable, pGC, nsegInit,
					      pSegInit);
				uxa_finish_access_gc(pGC);
			}
			uxa_finish_access(pDrawable);
		}
		return;
	}
	/* fb calls mi functions in the lineWidth != 0 case. */
	fbPolySegment(pDrawable, pGC, nsegInit, pSegInit);
}

void
uxa_check_poly_arc(DrawablePtr pDrawable, GCPtr pGC, int narcs, xArc * pArcs)
{
	ScreenPtr screen = pDrawable->pScreen;

	UXA_FALLBACK(("to %p (%c)\n", pDrawable,
		      uxa_drawable_location(pDrawable)));

	/* Disable this as fbPolyArc can call miZeroPolyArc which in turn
	 * can call accelerated functions, that as yet, haven't been notified
	 * with uxa_finish_access().
	 */
#if 0
	if (pGC->lineWidth == 0) {
		if (uxa_prepare_access(pDrawable, UXA_ACCESS_RW)) {
			if (uxa_prepare_access_gc(pGC)) {
				fbPolyArc(pDrawable, pGC, narcs, pArcs);
				uxa_finish_access_gc(pGC);
			}
			uxa_finish_access(pDrawable);
		}
		return;
	}
#endif
	miPolyArc(pDrawable, pGC, narcs, pArcs);
}

void
uxa_check_poly_fill_rect(DrawablePtr pDrawable, GCPtr pGC,
			 int nrect, xRectangle * prect)
{
	ScreenPtr screen = pDrawable->pScreen;

	UXA_FALLBACK(("to %p (%c)\n", pDrawable,
		      uxa_drawable_location(pDrawable)));

	if (uxa_prepare_access(pDrawable, UXA_ACCESS_RW)) {
		if (uxa_prepare_access_gc(pGC)) {
			fbPolyFillRect(pDrawable, pGC, nrect, prect);
			uxa_finish_access_gc(pGC);
		}
		uxa_finish_access(pDrawable);
	}
}

void
uxa_check_image_glyph_blt(DrawablePtr pDrawable, GCPtr pGC,
			  int x, int y, unsigned int nglyph,
			  CharInfoPtr * ppci, pointer pglyphBase)
{
	ScreenPtr screen = pDrawable->pScreen;

	UXA_FALLBACK(("to %p (%c)\n", pDrawable,
		      uxa_drawable_location(pDrawable)));
	if (uxa_prepare_access(pDrawable, UXA_ACCESS_RW)) {
		if (uxa_prepare_access_gc(pGC)) {
			fbImageGlyphBlt(pDrawable, pGC, x, y, nglyph, ppci,
					pglyphBase);
			uxa_finish_access_gc(pGC);
		}
		uxa_finish_access(pDrawable);
	}
}

void
uxa_check_poly_glyph_blt(DrawablePtr pDrawable, GCPtr pGC,
			 int x, int y, unsigned int nglyph,
			 CharInfoPtr * ppci, pointer pglyphBase)
{
	ScreenPtr screen = pDrawable->pScreen;

	UXA_FALLBACK(("to %p (%c), style %d alu %d\n", pDrawable,
		      uxa_drawable_location(pDrawable), pGC->fillStyle,
		      pGC->alu));
	if (uxa_prepare_access(pDrawable, UXA_ACCESS_RW)) {
		if (uxa_prepare_access_gc(pGC)) {
			fbPolyGlyphBlt(pDrawable, pGC, x, y, nglyph, ppci,
				       pglyphBase);
			uxa_finish_access_gc(pGC);
		}
		uxa_finish_access(pDrawable);
	}
}

void
uxa_check_push_pixels(GCPtr pGC, PixmapPtr pBitmap,
		      DrawablePtr pDrawable, int w, int h, int x, int y)
{
	ScreenPtr screen = pDrawable->pScreen;

	UXA_FALLBACK(("from %p to %p (%c,%c)\n", pBitmap, pDrawable,
		      uxa_drawable_location(&pBitmap->drawable),
		      uxa_drawable_location(pDrawable)));
	if (uxa_prepare_access(pDrawable, UXA_ACCESS_RW)) {
		if (uxa_prepare_access(&pBitmap->drawable, UXA_ACCESS_RO)) {
			if (uxa_prepare_access_gc(pGC)) {
				fbPushPixels(pGC, pBitmap, pDrawable, w, h, x,
					     y);
				uxa_finish_access_gc(pGC);
			}
			uxa_finish_access(&pBitmap->drawable);
		}
		uxa_finish_access(pDrawable);
	}
}

void
uxa_check_get_spans(DrawablePtr pDrawable,
		    int wMax,
		    DDXPointPtr ppt, int *pwidth, int nspans, char *pdstStart)
{
	ScreenPtr screen = pDrawable->pScreen;

	UXA_FALLBACK(("from %p (%c)\n", pDrawable,
		      uxa_drawable_location(pDrawable)));
	if (uxa_prepare_access(pDrawable, UXA_ACCESS_RO)) {
		fbGetSpans(pDrawable, wMax, ppt, pwidth, nspans, pdstStart);
		uxa_finish_access(pDrawable);
	}
}

void
uxa_check_composite(CARD8 op,
		    PicturePtr pSrc,
		    PicturePtr pMask,
		    PicturePtr pDst,
		    INT16 xSrc, INT16 ySrc,
		    INT16 xMask, INT16 yMask,
		    INT16 xDst, INT16 yDst,
		    CARD16 width, CARD16 height)
{
	ScreenPtr screen = pDst->pDrawable->pScreen;

	UXA_FALLBACK(("from picts %p/%p to pict %p\n", pSrc, pMask, pDst));

	if (uxa_picture_prepare_access(pDst, UXA_ACCESS_RW)) {
		if (uxa_picture_prepare_access(pSrc, UXA_ACCESS_RO)) {
			if (!pMask || uxa_picture_prepare_access(pMask, UXA_ACCESS_RO)) {
				fbComposite(op, pSrc, pMask, pDst,
					    xSrc, ySrc,
					    xMask, yMask,
					    xDst, yDst,
					    width, height);
				if (pMask)
					uxa_picture_finish_access(pMask);
			}
			uxa_picture_finish_access(pSrc);
		}
		uxa_picture_finish_access(pDst);
	}
}

void
uxa_check_add_traps(PicturePtr pPicture,
		    INT16 x_off, INT16 y_off, int ntrap, xTrap * traps)
{
	ScreenPtr screen = pPicture->pDrawable->pScreen;

	UXA_FALLBACK(("to pict %p (%c)\n", pPicture,
		      uxa_drawable_location(pPicture->pDrawable)));
	if (uxa_picture_prepare_access(pPicture, UXA_ACCESS_RW)) {
		fbAddTraps(pPicture, x_off, y_off, ntrap, traps);
		uxa_picture_finish_access(pPicture);
	}
}

/**
 * Gets the 0,0 pixel of a pixmap.  Used for doing solid fills of tiled pixmaps
 * that happen to be 1x1.  Pixmap must be at least 8bpp.
 *
 * XXX This really belongs in fb, so it can be aware of tiling and etc.
 */
CARD32 uxa_get_pixmap_first_pixel(PixmapPtr pPixmap)
{
	CARD32 pixel;
	void *fb;

	if (!uxa_prepare_access(&pPixmap->drawable, UXA_ACCESS_RO))
		return 0;

	fb = pPixmap->devPrivate.ptr;

	switch (pPixmap->drawable.bitsPerPixel) {
	case 32:
		pixel = *(CARD32 *) fb;
		break;
	case 16:
		pixel = *(CARD16 *) fb;
		break;
	default:
		pixel = *(CARD8 *) fb;
		break;
	}
	uxa_finish_access(&pPixmap->drawable);

	return pixel;
}
