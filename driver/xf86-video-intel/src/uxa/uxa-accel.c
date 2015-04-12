
/*
 * Copyright ® 2001 Keith Packard
 *
 * Partly based on code that is Copyright ® The XFree86 Project Inc.
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
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *    Michel Dänzer <michel@tungstengraphics.com>
 *
 */

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif
#include "uxa-priv.h"
#include "uxa-glamor.h"
#include <X11/fonts/fontstruct.h>
#include "dixfontstr.h"
#include "uxa.h"

static void
uxa_fill_spans(DrawablePtr pDrawable, GCPtr pGC, int n,
	       DDXPointPtr ppt, int *pwidth, int fSorted)
{
	ScreenPtr screen = pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);
	RegionPtr pClip = fbGetCompositeClip(pGC);
	PixmapPtr dst_pixmap;
	BoxPtr pbox;
	int nbox;
	int x1, x2, y;
	int off_x, off_y;

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok = 0;

		if (uxa_prepare_access(pDrawable, UXA_GLAMOR_ACCESS_RW)) {
			ok = glamor_fill_spans_nf(pDrawable,
						  pGC, n, ppt, pwidth, fSorted);
			uxa_finish_access(pDrawable, UXA_GLAMOR_ACCESS_RW);
		}

		if (!ok)
			goto fallback;

		return;
	}

	if (uxa_screen->force_fallback)
		goto fallback;

	if (pGC->fillStyle != FillSolid)
		goto fallback;

	dst_pixmap = uxa_get_offscreen_pixmap(pDrawable, &off_x, &off_y);
	if (!dst_pixmap)
		goto fallback;

	if (uxa_screen->info->check_solid &&
	    !uxa_screen->info->check_solid(pDrawable, pGC->alu, pGC->planemask))
		goto fallback;

	if (!(*uxa_screen->info->prepare_solid) (dst_pixmap,
						 pGC->alu,
						 pGC->planemask,
						 pGC->fgPixel))
		goto fallback;

	while (n--) {
		x1 = ppt->x;
		y = ppt->y;
		x2 = x1 + (int)*pwidth;
		ppt++;
		pwidth++;

		nbox = REGION_NUM_RECTS(pClip);
		pbox = REGION_RECTS(pClip);
		while (nbox--) {
			int X1 = x1, X2 = x2;
			if (X1 < pbox->x1)
				X1 = pbox->x1;

			if (X2 > pbox->x2)
				X2 = pbox->x2;

			if (X2 > X1 && pbox->y1 <= y && pbox->y2 > y)
				(*uxa_screen->info->solid) (dst_pixmap,
							    X1 + off_x, y + off_y,
							    X2 + off_x, y + 1 + off_y);
			pbox++;
		}
	}
	(*uxa_screen->info->done_solid) (dst_pixmap);

	return;

fallback:
	uxa_check_fill_spans(pDrawable, pGC, n, ppt, pwidth, fSorted);
}

static Bool
uxa_do_put_image(DrawablePtr pDrawable, GCPtr pGC, int depth, int x, int y,
		 int w, int h, int format, char *bits, int src_stride)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pDrawable->pScreen);
	PixmapPtr pPix;
	RegionPtr pClip;
	BoxPtr pbox;
	int nbox;
	int xoff, yoff;
	int bpp = pDrawable->bitsPerPixel;

	/* Don't bother with under 8bpp, XYPixmaps. */
	if (format != ZPixmap || bpp < 8)
		return FALSE;

	if (uxa_screen->force_fallback)
		return FALSE;

	if (!uxa_screen->info->put_image)
		return FALSE;

	/* Only accelerate copies: no rop or planemask. */
	if (!UXA_PM_IS_SOLID(pDrawable, pGC->planemask) || pGC->alu != GXcopy)
		return FALSE;

	pPix = uxa_get_offscreen_pixmap(pDrawable, &xoff, &yoff);
	if (!pPix)
		return FALSE;

	x += pDrawable->x;
	y += pDrawable->y;

	pClip = fbGetCompositeClip(pGC);
	for (nbox = REGION_NUM_RECTS(pClip),
	     pbox = REGION_RECTS(pClip); nbox--; pbox++) {
		int x1 = x;
		int y1 = y;
		int x2 = x + w;
		int y2 = y + h;
		char *src;
		Bool ok;

		if (x1 < pbox->x1)
			x1 = pbox->x1;
		if (y1 < pbox->y1)
			y1 = pbox->y1;
		if (x2 > pbox->x2)
			x2 = pbox->x2;
		if (y2 > pbox->y2)
			y2 = pbox->y2;
		if (x1 >= x2 || y1 >= y2)
			continue;

		src = bits + (y1 - y) * src_stride + (x1 - x) * (bpp / 8);
		ok = uxa_screen->info->put_image(pPix, x1 + xoff, y1 + yoff,
						 x2 - x1, y2 - y1, src,
						 src_stride);
		/* If we fail to accelerate the upload, fall back to using
		 * unaccelerated fb calls.
		 */
		if (!ok) {
			FbStip *dst;
			FbStride dst_stride;
			int dstBpp;
			int dstXoff, dstYoff;

			if (!uxa_prepare_access(pDrawable, UXA_ACCESS_RW))
				return FALSE;

			fbGetStipDrawable(pDrawable, dst, dst_stride, dstBpp,
					  dstXoff, dstYoff);

			fbBltStip((FbStip *) bits +
				  (y1 - y) * (src_stride / sizeof(FbStip)),
				  src_stride / sizeof(FbStip),
				  (x1 - x) * dstBpp,
				  dst + (y1 + dstYoff) * dst_stride, dst_stride,
				  (x1 + dstXoff) * dstBpp, (x2 - x1) * dstBpp,
				  y2 - y1, GXcopy, FB_ALLONES, dstBpp);

			uxa_finish_access(pDrawable, UXA_ACCESS_RW);
		}
	}


	return TRUE;
}

static void
uxa_put_image(DrawablePtr pDrawable, GCPtr pGC, int depth, int x, int y,
	      int w, int h, int leftPad, int format, char *bits)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pDrawable->pScreen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok = 0;

		if (uxa_prepare_access(pDrawable, UXA_GLAMOR_ACCESS_RW)) {
			ok = glamor_put_image_nf(pDrawable,
						 pGC, depth, x, y, w, h,
						 leftPad, format, bits);
			uxa_finish_access(pDrawable, UXA_GLAMOR_ACCESS_RW);
		}
		if (!ok)
			goto fallback;

		return;
	}

	if (!uxa_do_put_image(pDrawable, pGC, depth, x, y, w, h, format, bits,
			      PixmapBytePad(w, pDrawable->depth))) {
fallback:
		uxa_check_put_image(pDrawable, pGC, depth, x, y, w, h, leftPad,
				    format, bits);
	}
}

static inline Bool
uxa_copy_n_to_n_two_dir(DrawablePtr pSrcDrawable, DrawablePtr pDstDrawable,
			GCPtr pGC, BoxPtr pbox, int nbox, int dx, int dy)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pDstDrawable->pScreen);
	PixmapPtr pSrcPixmap, pDstPixmap;
	int src_off_x, src_off_y, dst_off_x, dst_off_y;
	int dirsetup;

	/* Need to get both pixmaps to call the driver routines */
	pSrcPixmap =
	    uxa_get_offscreen_pixmap(pSrcDrawable, &src_off_x, &src_off_y);
	pDstPixmap =
	    uxa_get_offscreen_pixmap(pDstDrawable, &dst_off_x, &dst_off_y);
	if (!pSrcPixmap || !pDstPixmap)
		return FALSE;

	/*
	 * Now the case of a chip that only supports xdir = ydir = 1 or
	 * xdir = ydir = -1, but we have xdir != ydir.
	 */
	dirsetup = 0;		/* No direction set up yet. */
	for (; nbox; pbox++, nbox--) {
		if (dx >= 0 && (src_off_y + pbox->y1 + dy) != pbox->y1) {
			/* Do a xdir = ydir = -1 blit instead. */
			if (dirsetup != -1) {
				if (dirsetup != 0)
					uxa_screen->info->done_copy(pDstPixmap);
				dirsetup = -1;
				if (!(*uxa_screen->info->prepare_copy)
				    (pSrcPixmap, pDstPixmap, -1, -1,
				     pGC ? pGC->alu : GXcopy,
				     pGC ? pGC->planemask : FB_ALLONES))
					return FALSE;
			}
			(*uxa_screen->info->copy) (pDstPixmap,
						   src_off_x + pbox->x1 + dx,
						   src_off_y + pbox->y1 + dy,
						   dst_off_x + pbox->x1,
						   dst_off_y + pbox->y1,
						   pbox->x2 - pbox->x1,
						   pbox->y2 - pbox->y1);
		} else if (dx < 0 && (src_off_y + pbox->y1 + dy) != pbox->y1) {
			/* Do a xdir = ydir = 1 blit instead. */
			if (dirsetup != 1) {
				if (dirsetup != 0)
					uxa_screen->info->done_copy(pDstPixmap);
				dirsetup = 1;
				if (!(*uxa_screen->info->prepare_copy)
				    (pSrcPixmap, pDstPixmap, 1, 1,
				     pGC ? pGC->alu : GXcopy,
				     pGC ? pGC->planemask : FB_ALLONES))
					return FALSE;
			}
			(*uxa_screen->info->copy) (pDstPixmap,
						   src_off_x + pbox->x1 + dx,
						   src_off_y + pbox->y1 + dy,
						   dst_off_x + pbox->x1,
						   dst_off_y + pbox->y1,
						   pbox->x2 - pbox->x1,
						   pbox->y2 - pbox->y1);
		} else if (dx >= 0) {
			/*
			 * xdir = 1, ydir = -1.
			 * Perform line-by-line xdir = ydir = 1 blits, going up.
			 */
			int i;
			if (dirsetup != 1) {
				if (dirsetup != 0)
					uxa_screen->info->done_copy(pDstPixmap);
				dirsetup = 1;
				if (!(*uxa_screen->info->prepare_copy)
				    (pSrcPixmap, pDstPixmap, 1, 1,
				     pGC ? pGC->alu : GXcopy,
				     pGC ? pGC->planemask : FB_ALLONES))
					return FALSE;
			}
			for (i = pbox->y2 - pbox->y1 - 1; i >= 0; i--)
				(*uxa_screen->info->copy) (pDstPixmap,
							   src_off_x +
							   pbox->x1 + dx,
							   src_off_y +
							   pbox->y1 + dy + i,
							   dst_off_x + pbox->x1,
							   dst_off_y +
							   pbox->y1 + i,
							   pbox->x2 - pbox->x1,
							   1);
		} else {
			/*
			 * xdir = -1, ydir = 1.
			 * Perform line-by-line xdir = ydir = -1 blits,
			 * going down.
			 */
			int i;
			if (dirsetup != -1) {
				if (dirsetup != 0)
					uxa_screen->info->done_copy(pDstPixmap);
				dirsetup = -1;
				if (!(*uxa_screen->info->prepare_copy)
				    (pSrcPixmap, pDstPixmap, -1, -1,
				     pGC ? pGC->alu : GXcopy,
				     pGC ? pGC->planemask : FB_ALLONES))
					return FALSE;
			}
			for (i = 0; i < pbox->y2 - pbox->y1; i++)
				(*uxa_screen->info->copy) (pDstPixmap,
							   src_off_x +
							   pbox->x1 + dx,
							   src_off_y +
							   pbox->y1 + dy + i,
							   dst_off_x + pbox->x1,
							   dst_off_y +
							   pbox->y1 + i,
							   pbox->x2 - pbox->x1,
							   1);
		}
	}
	if (dirsetup != 0)
		uxa_screen->info->done_copy(pDstPixmap);
	return TRUE;
}

void
uxa_copy_n_to_n(DrawablePtr pSrcDrawable,
		DrawablePtr pDstDrawable,
		GCPtr pGC,
		BoxPtr pbox,
		int nbox,
		int dx,
		int dy,
		Bool reverse, Bool upsidedown, Pixel bitplane, void *closure)
{
	ScreenPtr screen = pDstDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);
	int src_off_x, src_off_y;
	int dst_off_x, dst_off_y;
	PixmapPtr pSrcPixmap, pDstPixmap;

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok = 0;

		if (uxa_prepare_access(pSrcDrawable, UXA_GLAMOR_ACCESS_RO)) {
			if (uxa_prepare_access(pDstDrawable, UXA_GLAMOR_ACCESS_RW)) {
				ok = glamor_copy_n_to_n_nf(pSrcDrawable, pDstDrawable,
							   pGC, pbox, nbox, dx, dy,
							   reverse, upsidedown, bitplane,
							   closure);
				uxa_finish_access(pDstDrawable, UXA_GLAMOR_ACCESS_RW);
			}
			uxa_finish_access(pSrcDrawable, UXA_GLAMOR_ACCESS_RO);
		}

		if (!ok)
			goto fallback;

		return;
	}

	if (uxa_screen->force_fallback)
		goto fallback;

	pSrcPixmap = uxa_get_drawable_pixmap(pSrcDrawable);
	pDstPixmap = uxa_get_drawable_pixmap(pDstDrawable);
	if (!pSrcPixmap || !pDstPixmap)
		goto fallback;

	if (uxa_screen->info->check_copy &&
	    !uxa_screen->info->check_copy(pSrcPixmap, pDstPixmap,
					  pGC ? pGC->alu : GXcopy,
					  pGC ? pGC->planemask : FB_ALLONES))
		goto fallback;

	uxa_get_drawable_deltas(pSrcDrawable, pSrcPixmap, &src_off_x,
				&src_off_y);
	uxa_get_drawable_deltas(pDstDrawable, pDstPixmap, &dst_off_x,
				&dst_off_y);

	/* Mixed directions must be handled specially if the card is lame */
	if ((uxa_screen->info->flags & UXA_TWO_BITBLT_DIRECTIONS) &&
	    reverse != upsidedown) {
		if (uxa_copy_n_to_n_two_dir
		    (pSrcDrawable, pDstDrawable, pGC, pbox, nbox, dx, dy))
			return;
		goto fallback;
	}

	if (!uxa_pixmap_is_offscreen(pDstPixmap)) {
		int stride, bpp;
		char *dst;

		if (!uxa_pixmap_is_offscreen(pSrcPixmap))
			goto fallback;

		if (!uxa_screen->info->get_image)
			goto fallback;

		/* Don't bother with under 8bpp, XYPixmaps. */
		bpp = pSrcPixmap->drawable.bitsPerPixel;
		if (bpp != pDstDrawable->bitsPerPixel || bpp < 8)
			goto fallback;

		/* Only accelerate copies: no rop or planemask. */
		if (pGC && (!UXA_PM_IS_SOLID(pSrcDrawable, pGC->planemask) || pGC->alu != GXcopy))
			goto fallback;

		dst = pDstPixmap->devPrivate.ptr;
		stride = pDstPixmap->devKind;
		bpp /= 8;
		while (nbox--) {
			if (!uxa_screen->info->get_image(pSrcPixmap,
							 pbox->x1 + dx + src_off_x,
							 pbox->y1 + dy + src_off_y,
							 pbox->x2 - pbox->x1,
							 pbox->y2 - pbox->y1,
							 (char *) dst +
							 (pbox->y1 + dst_off_y) * stride +
							 (pbox->x1 + dst_off_x) * bpp,
							 stride))
				goto fallback;

			pbox++;
		}

		return;
	}

	if (uxa_pixmap_is_offscreen(pSrcPixmap)) {
	    if (!(*uxa_screen->info->prepare_copy) (pSrcPixmap, pDstPixmap,
						reverse ? -1 : 1,
						upsidedown ? -1 : 1,
						pGC ? pGC->alu : GXcopy,
						pGC ? pGC->
						planemask : FB_ALLONES))
		goto fallback;

	    while (nbox--) {
		(*uxa_screen->info->copy) (pDstPixmap,
					   pbox->x1 + dx + src_off_x,
					   pbox->y1 + dy + src_off_y,
					   pbox->x1 + dst_off_x,
					   pbox->y1 + dst_off_y,
					   pbox->x2 - pbox->x1,
					   pbox->y2 - pbox->y1);
		pbox++;
	    }

	    (*uxa_screen->info->done_copy) (pDstPixmap);
	} else {
	    int stride, bpp;
	    char *src;

	    if (!uxa_screen->info->put_image)
		goto fallback;

	    /* Don't bother with under 8bpp, XYPixmaps. */
	    bpp = pSrcPixmap->drawable.bitsPerPixel;
	    if (bpp != pDstDrawable->bitsPerPixel || bpp < 8)
		goto fallback;

	    /* Only accelerate copies: no rop or planemask. */
	    if (pGC && (!UXA_PM_IS_SOLID(pSrcDrawable, pGC->planemask) || pGC->alu != GXcopy))
		goto fallback;

	    src = pSrcPixmap->devPrivate.ptr;
	    stride = pSrcPixmap->devKind;
	    bpp /= 8;
	    while (nbox--) {
		if (!uxa_screen->info->put_image(pDstPixmap,
						 pbox->x1 + dst_off_x,
						 pbox->y1 + dst_off_y,
						 pbox->x2 - pbox->x1,
						 pbox->y2 - pbox->y1,
						 (char *) src +
						 (pbox->y1 + dy + src_off_y) * stride +
						 (pbox->x1 + dx + src_off_x) * bpp,
						 stride))
		    goto fallback;

		pbox++;
	    }
	}

	return;

fallback:
	UXA_FALLBACK(("from %p to %p (%c,%c)\n", pSrcDrawable, pDstDrawable,
		      uxa_drawable_location(pSrcDrawable),
		      uxa_drawable_location(pDstDrawable)));
	if (uxa_prepare_access(pDstDrawable, UXA_ACCESS_RW)) {
		if (pSrcDrawable == pDstDrawable ||
		    uxa_prepare_access(pSrcDrawable, UXA_ACCESS_RO)) {
			fbCopyNtoN(pSrcDrawable, pDstDrawable, pGC, pbox, nbox,
				   dx, dy, reverse, upsidedown, bitplane,
				   closure);
			if (pSrcDrawable != pDstDrawable)
				uxa_finish_access(pSrcDrawable, UXA_ACCESS_RO);
		}
		uxa_finish_access(pDstDrawable, UXA_ACCESS_RW);
	}
}

RegionPtr
uxa_copy_area(DrawablePtr pSrcDrawable, DrawablePtr pDstDrawable, GCPtr pGC,
	      int srcx, int srcy, int width, int height, int dstx, int dsty)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pDstDrawable->pScreen);

	if (uxa_screen->force_fallback) {
		return uxa_check_copy_area(pSrcDrawable, pDstDrawable, pGC,
					   srcx, srcy, width, height, dstx,
					   dsty);
	}

	return miDoCopy(pSrcDrawable, pDstDrawable, pGC,
			srcx, srcy, width, height,
			dstx, dsty, uxa_copy_n_to_n, 0, NULL);
}

static void
uxa_poly_point(DrawablePtr pDrawable, GCPtr pGC, int mode, int npt,
	       DDXPointPtr ppt)
{
	int i;
	xRectangle *prect;
	uxa_screen_t *uxa_screen = uxa_get_screen(pDrawable->pScreen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok = 0;

		if (uxa_prepare_access(pDrawable, UXA_GLAMOR_ACCESS_RW)) {
			ok = glamor_poly_point_nf(pDrawable, pGC, mode, npt, ppt);
			uxa_finish_access(pDrawable, UXA_GLAMOR_ACCESS_RW);
		}

		if (ok)
			return;
	}

	/* If we can't reuse the current GC as is, don't bother accelerating the
	 * points.
	 */
	if (pGC->fillStyle != FillSolid) {
		uxa_check_poly_point(pDrawable, pGC, mode, npt, ppt);
		return;
	}

	prect = malloc(sizeof(xRectangle) * npt);
	if (!prect)
		return;
	for (i = 0; i < npt; i++) {
		prect[i].x = ppt[i].x;
		prect[i].y = ppt[i].y;
		if (i > 0 && mode == CoordModePrevious) {
			prect[i].x += prect[i - 1].x;
			prect[i].y += prect[i - 1].y;
		}
		prect[i].width = 1;
		prect[i].height = 1;
	}
	pGC->ops->PolyFillRect(pDrawable, pGC, npt, prect);
	free(prect);
}

/**
 * uxa_poly_lines() checks if it can accelerate the lines as a group of
 * horizontal or vertical lines (rectangles), and uses existing rectangle fill
 * acceleration if so.
 */
static void
uxa_poly_lines(DrawablePtr pDrawable, GCPtr pGC, int mode, int npt,
	       DDXPointPtr ppt)
{
	xRectangle *prect;
	int x1, x2, y1, y2;
	int i;
	uxa_screen_t *uxa_screen = uxa_get_screen(pDrawable->pScreen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok = 0;

		if (uxa_prepare_access(pDrawable, UXA_GLAMOR_ACCESS_RW)) {
			ok = glamor_poly_lines_nf(pDrawable, pGC, mode, npt, ppt);
			uxa_finish_access(pDrawable, UXA_GLAMOR_ACCESS_RW);
		}

		if (ok)
			return;
	}

	/* Don't try to do wide lines or non-solid fill style. */
	if (pGC->lineWidth != 0 || pGC->lineStyle != LineSolid ||
	    pGC->fillStyle != FillSolid) {
		uxa_check_poly_lines(pDrawable, pGC, mode, npt, ppt);
		return;
	}

	prect = malloc(sizeof(xRectangle) * (npt - 1));
	if (!prect)
		return;
	x1 = ppt[0].x;
	y1 = ppt[0].y;
	/* If we have any non-horizontal/vertical, fall back. */
	for (i = 0; i < npt - 1; i++) {
		if (mode == CoordModePrevious) {
			x2 = x1 + ppt[i + 1].x;
			y2 = y1 + ppt[i + 1].y;
		} else {
			x2 = ppt[i + 1].x;
			y2 = ppt[i + 1].y;
		}

		if (x1 != x2 && y1 != y2) {
			free(prect);
			uxa_check_poly_lines(pDrawable, pGC, mode, npt, ppt);
			return;
		}

		if (x1 < x2) {
			prect[i].x = x1;
			prect[i].width = x2 - x1 + 1;
		} else {
			prect[i].x = x2;
			prect[i].width = x1 - x2 + 1;
		}
		if (y1 < y2) {
			prect[i].y = y1;
			prect[i].height = y2 - y1 + 1;
		} else {
			prect[i].y = y2;
			prect[i].height = y1 - y2 + 1;
		}

		x1 = x2;
		y1 = y2;
	}
	pGC->ops->PolyFillRect(pDrawable, pGC, npt - 1, prect);
	free(prect);
}

/**
 * uxa_poly_segment() checks if it can accelerate the lines as a group of
 * horizontal or vertical lines (rectangles), and uses existing rectangle fill
 * acceleration if so.
 */
static void
uxa_poly_segment(DrawablePtr pDrawable, GCPtr pGC, int nseg, xSegment * pSeg)
{
	xRectangle *prect;
	int i;
	uxa_screen_t *uxa_screen = uxa_get_screen(pDrawable->pScreen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok = 0;

		if (uxa_prepare_access(pDrawable, UXA_GLAMOR_ACCESS_RW)) {
			ok = glamor_poly_segment_nf(pDrawable, pGC, nseg, pSeg);
			uxa_finish_access(pDrawable, UXA_GLAMOR_ACCESS_RW);
		}

		if (ok)
			return;
	}

	/* Don't try to do wide lines or non-solid fill style. */
	if (pGC->lineWidth != 0 || pGC->lineStyle != LineSolid ||
	    pGC->fillStyle != FillSolid) {
		uxa_check_poly_segment(pDrawable, pGC, nseg, pSeg);
		return;
	}

	/* If we have any non-horizontal/vertical, fall back. */
	for (i = 0; i < nseg; i++) {
		if (pSeg[i].x1 != pSeg[i].x2 && pSeg[i].y1 != pSeg[i].y2) {
			uxa_check_poly_segment(pDrawable, pGC, nseg, pSeg);
			return;
		}
	}

	prect = malloc(sizeof(xRectangle) * nseg);
	if (!prect)
		return;
	for (i = 0; i < nseg; i++) {
		if (pSeg[i].x1 < pSeg[i].x2) {
			prect[i].x = pSeg[i].x1;
			prect[i].width = pSeg[i].x2 - pSeg[i].x1 + 1;
		} else {
			prect[i].x = pSeg[i].x2;
			prect[i].width = pSeg[i].x1 - pSeg[i].x2 + 1;
		}
		if (pSeg[i].y1 < pSeg[i].y2) {
			prect[i].y = pSeg[i].y1;
			prect[i].height = pSeg[i].y2 - pSeg[i].y1 + 1;
		} else {
			prect[i].y = pSeg[i].y2;
			prect[i].height = pSeg[i].y1 - pSeg[i].y2 + 1;
		}

		/* don't paint last pixel */
		if (pGC->capStyle == CapNotLast) {
			if (prect[i].width == 1)
				prect[i].height--;
			else
				prect[i].width--;
		}
	}
	pGC->ops->PolyFillRect(pDrawable, pGC, nseg, prect);
	free(prect);
}

static Bool uxa_fill_region_solid(DrawablePtr pDrawable, RegionPtr pRegion,
				  Pixel pixel, CARD32 planemask, CARD32 alu);

static void
uxa_poly_fill_rect(DrawablePtr pDrawable,
		   GCPtr pGC, int nrect, xRectangle * prect)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pDrawable->pScreen);
	RegionPtr pClip = fbGetCompositeClip(pGC);
	PixmapPtr pPixmap;
	RegionPtr pReg;
	BoxPtr pbox;
	int fullX1, fullX2, fullY1, fullY2;
	int xoff, yoff;
	int xorg, yorg;
	int n;

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok = 0;

		if (uxa_prepare_access(pDrawable, UXA_GLAMOR_ACCESS_RW)) {
			ok = glamor_poly_fill_rect_nf(pDrawable, pGC, nrect, prect);
			uxa_finish_access(pDrawable, UXA_GLAMOR_ACCESS_RW);
		}

		if (!ok)
			uxa_check_poly_fill_rect(pDrawable, pGC, nrect, prect);

		return;
	}

	/* Compute intersection of rects and clip region */
	pReg = RECTS_TO_REGION(pScreen, nrect, prect, CT_UNSORTED);
	REGION_TRANSLATE(pScreen, pReg, pDrawable->x, pDrawable->y);
	REGION_INTERSECT(pScreen, pReg, pClip, pReg);

	if (!REGION_NUM_RECTS(pReg))
		goto out;

	if (uxa_screen->force_fallback)
		goto fallback;

	pPixmap = uxa_get_offscreen_pixmap (pDrawable, &xoff, &yoff);
	if (!pPixmap)
		goto fallback;

	/* For ROPs where overlaps don't matter, convert rectangles to region
	 * and call uxa_fill_region_{solid,tiled}.
	 */
	if ((pGC->fillStyle == FillSolid || pGC->fillStyle == FillTiled) &&
	    (nrect == 1 || pGC->alu == GXcopy || pGC->alu == GXclear ||
	     pGC->alu == GXnoop || pGC->alu == GXcopyInverted ||
	     pGC->alu == GXset)) {
		if (((pGC->fillStyle == FillSolid || pGC->tileIsPixel) &&
		     uxa_fill_region_solid(pDrawable, pReg,
					   pGC->fillStyle ==
					   FillSolid ? pGC->fgPixel : pGC->tile.
					   pixel, pGC->planemask, pGC->alu))
		    || (pGC->fillStyle == FillTiled && !pGC->tileIsPixel
			&& uxa_fill_region_tiled(pDrawable, pReg,
						 pGC->tile.pixmap, &pGC->patOrg,
						 pGC->planemask, pGC->alu))) {
			goto out;
		}
	}

	if (pGC->fillStyle != FillSolid &&
	    !(pGC->tileIsPixel && pGC->fillStyle == FillTiled)) {
		goto fallback;
	}

	if (uxa_screen->info->check_solid &&
	    !uxa_screen->info->check_solid(pDrawable, pGC->alu, pGC->planemask)) {
		goto fallback;
	}

	if (!(*uxa_screen->info->prepare_solid) (pPixmap,
						 pGC->alu,
						 pGC->planemask,
						 pGC->fgPixel)) {
fallback:
		uxa_check_poly_fill_rect(pDrawable, pGC, nrect, prect);
		goto out;
	}

	xorg = pDrawable->x;
	yorg = pDrawable->y;

	while (nrect--) {
		fullX1 = prect->x + xorg;
		fullY1 = prect->y + yorg;
		fullX2 = fullX1 + (int)prect->width;
		fullY2 = fullY1 + (int)prect->height;
		prect++;

		n = REGION_NUM_RECTS(pClip);
		pbox = REGION_RECTS(pClip);
		/*
		 * clip the rectangle to each box in the clip region
		 * this is logically equivalent to calling Intersect(),
		 * but rectangles may overlap each other here.
		 */
		while (n--) {
			int x1 = fullX1;
			int x2 = fullX2;
			int y1 = fullY1;
			int y2 = fullY2;

			if (pbox->x1 > x1)
				x1 = pbox->x1;
			if (pbox->x2 < x2)
				x2 = pbox->x2;
			if (pbox->y1 > y1)
				y1 = pbox->y1;
			if (pbox->y2 < y2)
				y2 = pbox->y2;
			pbox++;

			if (x1 >= x2 || y1 >= y2)
				continue;

			(*uxa_screen->info->solid) (pPixmap,
						    x1 + xoff,
						    y1 + yoff,
						    x2 + xoff,
						    y2 + yoff);
		}
	}
	(*uxa_screen->info->done_solid) (pPixmap);

out:
	REGION_UNINIT(pScreen, pReg);
	REGION_DESTROY(pScreen, pReg);
}

void
uxa_get_spans(DrawablePtr pDrawable,
	      int wMax,
	      DDXPointPtr ppt, int *pwidth, int nspans, char *pdstStart)
{
	ScreenPtr screen = pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok = 0;

		if (uxa_prepare_access(pDrawable, UXA_GLAMOR_ACCESS_RW)) {
			ok = glamor_get_spans_nf(pDrawable, wMax, ppt,
						 pwidth, nspans, pdstStart);
			uxa_finish_access(pDrawable, UXA_GLAMOR_ACCESS_RW);
		}

		if (!ok)
			goto fallback;

		return;
	}

fallback:
	uxa_check_get_spans(pDrawable, wMax, ppt, pwidth, nspans, pdstStart);
}

static void
uxa_set_spans(DrawablePtr pDrawable, GCPtr gc, char *src,
                 DDXPointPtr points, int *widths, int n, int sorted)
{
	ScreenPtr screen = pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok = 0;

		if (uxa_prepare_access(pDrawable, UXA_GLAMOR_ACCESS_RW)) {
			ok = glamor_set_spans_nf(pDrawable, gc, src,
						 points, widths, n, sorted);
			uxa_finish_access(pDrawable, UXA_GLAMOR_ACCESS_RW);
		}

		if (!ok)
			goto fallback;

		return;
	}

fallback:
	uxa_check_set_spans(pDrawable, gc, src, points, widths, n, sorted);
}

static RegionPtr
uxa_copy_plane(DrawablePtr pSrc, DrawablePtr pDst, GCPtr pGC,
	       int srcx, int srcy, int w, int h, int dstx, int dsty,
	       unsigned long bitPlane)
{
	ScreenPtr screen = pDst->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		RegionPtr region = NULL;
		int ok = 0;

		if (uxa_prepare_access(pDst, UXA_GLAMOR_ACCESS_RW)) {
			if (uxa_prepare_access(pSrc, UXA_GLAMOR_ACCESS_RO)) {
				ok = glamor_copy_plane_nf(pSrc, pDst, pGC, srcx, srcy, w, h,
							  dstx, dsty, bitPlane, &region);
				uxa_finish_access(pSrc, UXA_GLAMOR_ACCESS_RO);
			}
			uxa_finish_access(pDst, UXA_GLAMOR_ACCESS_RW);
		}
		if (!ok)
			goto fallback;
		return region;
	}

fallback:
	return uxa_check_copy_plane(pSrc, pDst, pGC, srcx, srcy, w, h,
				    dstx, dsty, bitPlane);
}

static void
uxa_image_glyph_blt(DrawablePtr pDrawable, GCPtr pGC,
		    int x, int y, unsigned int nglyph,
		    CharInfoPtr * ppci, pointer pglyphBase)
{
	ScreenPtr screen = pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok = 0;

		if (uxa_prepare_access(pDrawable, UXA_GLAMOR_ACCESS_RW)) {
			ok = glamor_image_glyph_blt_nf(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase);
			uxa_finish_access(pDrawable, UXA_GLAMOR_ACCESS_RW);
		}
		if (!ok)
			goto fallback;
		return;
	}

fallback:
	uxa_check_image_glyph_blt(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase);
}

static void
uxa_poly_glyph_blt(DrawablePtr pDrawable, GCPtr pGC,
		   int x, int y, unsigned int nglyph,
		   CharInfoPtr * ppci, pointer pglyphBase)
{
	ScreenPtr screen = pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok = 0;

		if (uxa_prepare_access(pDrawable, UXA_GLAMOR_ACCESS_RW)) {
			ok = glamor_poly_glyph_blt_nf(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase);
			uxa_finish_access(pDrawable, UXA_GLAMOR_ACCESS_RW);
		}
		if (!ok)
			goto fallback;
		return;
	}

fallback:
	uxa_check_poly_glyph_blt(pDrawable, pGC, x, y, nglyph, ppci, pglyphBase);
}

static void
uxa_push_pixels(GCPtr pGC, PixmapPtr pBitmap,
		DrawablePtr pDrawable, int w, int h, int x, int y)
{
	ScreenPtr screen = pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		int ok = 0;

		if (uxa_prepare_access(pDrawable, UXA_GLAMOR_ACCESS_RW)) {
			if (uxa_prepare_access(&pBitmap->drawable, UXA_GLAMOR_ACCESS_RO)) {
				ok = glamor_push_pixels_nf(pGC, pBitmap, pDrawable, w, h, x, y);
				uxa_finish_access(&pBitmap->drawable, UXA_GLAMOR_ACCESS_RO);
			}
			uxa_finish_access(pDrawable, UXA_GLAMOR_ACCESS_RW);
		}
		if (!ok)
			goto fallback;
		return;
	}

fallback:
	uxa_check_push_pixels(pGC, pBitmap, pDrawable, w, h, x, y);
}

const GCOps uxa_ops = {
	uxa_fill_spans,
	uxa_set_spans,
	uxa_put_image,
	uxa_copy_area,
	uxa_copy_plane,
	uxa_poly_point,
	uxa_poly_lines,
	uxa_poly_segment,
	miPolyRectangle,
	uxa_check_poly_arc,
	miFillPolygon,
	uxa_poly_fill_rect,
	miPolyFillArc,
	miPolyText8,
	miPolyText16,
	miImageText8,
	miImageText16,
	uxa_image_glyph_blt,
	uxa_poly_glyph_blt,
	uxa_push_pixels,
};

void uxa_copy_window(WindowPtr pWin, DDXPointRec ptOldOrg, RegionPtr prgnSrc)
{
	RegionRec rgnDst;
	int dx, dy;
	PixmapPtr pPixmap = (*pWin->drawable.pScreen->GetWindowPixmap) (pWin);

	dx = ptOldOrg.x - pWin->drawable.x;
	dy = ptOldOrg.y - pWin->drawable.y;
	REGION_TRANSLATE(pWin->drawable.pScreen, prgnSrc, -dx, -dy);

	REGION_INIT(pWin->drawable.pScreen, &rgnDst, NullBox, 0);

	REGION_INTERSECT(pWin->drawable.pScreen, &rgnDst, &pWin->borderClip,
			 prgnSrc);
#ifdef COMPOSITE
	if (pPixmap->screen_x || pPixmap->screen_y)
		REGION_TRANSLATE(pWin->drawable.pScreen, &rgnDst,
				 -pPixmap->screen_x, -pPixmap->screen_y);
#endif

	miCopyRegion(&pPixmap->drawable, &pPixmap->drawable,
		     NULL, &rgnDst, dx, dy, uxa_copy_n_to_n, 0, NULL);

	REGION_UNINIT(pWin->drawable.pScreen, &rgnDst);
}

static Bool
uxa_fill_region_solid(DrawablePtr pDrawable,
		      RegionPtr pRegion,
		      Pixel pixel, CARD32 planemask, CARD32 alu)
{
	ScreenPtr screen = pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);
	PixmapPtr pixmap;
	int xoff, yoff;
	int nbox;
	BoxPtr pBox;
	Bool ret = FALSE;

	pixmap = uxa_get_offscreen_pixmap(pDrawable, &xoff, &yoff);
	if (!pixmap)
		return FALSE;

	REGION_TRANSLATE(screen, pRegion, xoff, yoff);

	nbox = REGION_NUM_RECTS(pRegion);
	pBox = REGION_RECTS(pRegion);

	if (uxa_screen->info->check_solid &&
	    !uxa_screen->info->check_solid(&pixmap->drawable, alu, planemask))
		goto err;

	if (!uxa_screen->info->prepare_solid(pixmap, alu, planemask, pixel))
		goto err;

	while (nbox--) {
		uxa_screen->info->solid(pixmap,
					pBox->x1, pBox->y1,
					pBox->x2, pBox->y2);
		pBox++;
	}
	uxa_screen->info->done_solid(pixmap);
	ret = TRUE;

err:
	REGION_TRANSLATE(screen, pRegion, -xoff, -yoff);
	return ret;
}

/* Try to do an accelerated tile of the pTile into pRegion of pDrawable.
 * Based on fbFillRegionTiled(), fbTile().
 */
Bool
uxa_fill_region_tiled(DrawablePtr pDrawable,
		      RegionPtr pRegion,
		      PixmapPtr pTile,
		      DDXPointPtr pPatOrg, CARD32 planemask, CARD32 alu)
{
	uxa_screen_t *uxa_screen = uxa_get_screen(pDrawable->pScreen);
	PixmapPtr pPixmap;
	int xoff, yoff;
	int tileWidth, tileHeight;
	int nbox = REGION_NUM_RECTS(pRegion);
	BoxPtr pBox = REGION_RECTS(pRegion);
	Bool ret = FALSE;

	tileWidth = pTile->drawable.width;
	tileHeight = pTile->drawable.height;

	/* If we're filling with a solid color, grab it out and go to
	 * FillRegionsolid, saving numerous copies.
	 */
	if (tileWidth == 1 && tileHeight == 1)
		return uxa_fill_region_solid(pDrawable, pRegion,
					     uxa_get_pixmap_first_pixel(pTile),
					     planemask, alu);

	pPixmap = uxa_get_offscreen_pixmap(pDrawable, &xoff, &yoff);
	if (!pPixmap || !uxa_pixmap_is_offscreen(pTile))
		goto out;

	if (uxa_screen->info->check_copy &&
	    !uxa_screen->info->check_copy(pTile, pPixmap, alu, planemask))
		return FALSE;

	REGION_TRANSLATE(pScreen, pRegion, xoff, yoff);

	if ((*uxa_screen->info->prepare_copy) (pTile, pPixmap, 1, 1, alu,
					       planemask)) {
		while (nbox--) {
			int height = pBox->y2 - pBox->y1;
			int dstY = pBox->y1;
			int tileY;

			modulus(dstY - yoff - pDrawable->y - pPatOrg->y,
				tileHeight, tileY);

			while (height > 0) {
				int width = pBox->x2 - pBox->x1;
				int dstX = pBox->x1;
				int tileX;
				int h = tileHeight - tileY;

				if (h > height)
					h = height;
				height -= h;

				modulus(dstX - xoff - pDrawable->x - pPatOrg->x,
					tileWidth, tileX);

				while (width > 0) {
					int w = tileWidth - tileX;
					if (w > width)
						w = width;
					width -= w;

					(*uxa_screen->info->copy) (pPixmap,
								   tileX, tileY,
								   dstX, dstY,
								   w, h);
					dstX += w;
					tileX = 0;
				}
				dstY += h;
				tileY = 0;
			}
			pBox++;
		}
		(*uxa_screen->info->done_copy) (pPixmap);

		ret = TRUE;
	}

out:
	REGION_TRANSLATE(pScreen, pRegion, -xoff, -yoff);

	return ret;
}

/**
 * Accelerates GetImage for solid ZPixmap downloads from framebuffer memory.
 *
 * This is probably the only case we actually care about.  The rest fall through
 * to migration and fbGetImage, which hopefully will result in migration pushing
 * the pixmap out of framebuffer.
 */
void
uxa_get_image(DrawablePtr pDrawable, int x, int y, int w, int h,
	      unsigned int format, unsigned long planeMask, char *d)
{
	ScreenPtr screen = pDrawable->pScreen;
	uxa_screen_t *uxa_screen = uxa_get_screen(screen);
	BoxRec Box;
	PixmapPtr pPix = uxa_get_drawable_pixmap(pDrawable);
	int xoff, yoff;
	Bool ok;

	uxa_get_drawable_deltas(pDrawable, pPix, &xoff, &yoff);

	Box.x1 = pDrawable->y + x + xoff;
	Box.y1 = pDrawable->y + y + yoff;
	Box.x2 = Box.x1 + w;
	Box.y2 = Box.y1 + h;

	if (uxa_screen->info->flags & UXA_USE_GLAMOR) {
		ok = 0;
		if (uxa_prepare_access(pDrawable, UXA_GLAMOR_ACCESS_RW)) {
			ok = glamor_get_image_nf(pDrawable, x, y, w, h,
						 format, planeMask, d);
			uxa_finish_access(pDrawable, UXA_GLAMOR_ACCESS_RW);
		}

		if (!ok)
			goto fallback;

		return;
	}

	if (uxa_screen->force_fallback)
		goto fallback;

	pPix = uxa_get_offscreen_pixmap(pDrawable, &xoff, &yoff);

	if (pPix == NULL || uxa_screen->info->get_image == NULL)
		goto fallback;

	/* Only cover the ZPixmap, solid copy case. */
	if (format != ZPixmap || !UXA_PM_IS_SOLID(pDrawable, planeMask))
		goto fallback;

	/* Only try to handle the 8bpp and up cases, since we don't want to
	 * think about <8bpp.
	 */
	if (pDrawable->bitsPerPixel < 8)
		goto fallback;

	ok = uxa_screen->info->get_image(pPix, pDrawable->x + x + xoff,
					 pDrawable->y + y + yoff, w, h, d,
					 PixmapBytePad(w, pDrawable->depth));
	if (ok)
		return;

fallback:
	UXA_FALLBACK(("from %p (%c)\n", pDrawable,
		      uxa_drawable_location(pDrawable)));

	if (uxa_prepare_access(pDrawable, UXA_ACCESS_RO)) {
		fbGetImage(pDrawable, x, y, w, h, format, planeMask, d);
		uxa_finish_access(pDrawable, UXA_ACCESS_RO);
	}

	return;
}
