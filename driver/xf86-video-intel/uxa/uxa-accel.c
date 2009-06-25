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
#include <X11/fonts/fontstruct.h>
#include "dixfontstr.h"
#include "uxa.h"

static void
uxa_fill_spans(DrawablePtr pDrawable, GCPtr pGC, int n,
	       DDXPointPtr ppt, int *pwidth, int fSorted)
{
    ScreenPtr	    pScreen = pDrawable->pScreen;
    uxa_screen_t    *uxa_screen = uxa_get_screen(pScreen);
    RegionPtr	    pClip = fbGetCompositeClip(pGC);
    PixmapPtr	    pPixmap = uxa_get_drawable_pixmap (pDrawable);
    BoxPtr	    pextent, pbox;
    int		    nbox;
    int		    extentX1, extentX2, extentY1, extentY2;
    int		    fullX1, fullX2, fullY1;
    int		    partX1, partX2;
    int		    off_x, off_y;

    if (uxa_screen->swappedOut || pGC->fillStyle != FillSolid ||
	!(pPixmap = uxa_get_offscreen_pixmap (pDrawable, &off_x, &off_y)) ||
	!(*uxa_screen->info->prepare_solid) (pPixmap,
					     pGC->alu,
					     pGC->planemask,
					     pGC->fgPixel))
    {
	uxa_check_fill_spans (pDrawable, pGC, n, ppt, pwidth, fSorted);
	return;
    }

    pextent = REGION_EXTENTS(pGC->pScreen, pClip);
    extentX1 = pextent->x1;
    extentY1 = pextent->y1;
    extentX2 = pextent->x2;
    extentY2 = pextent->y2;
    while (n--)
    {
	fullX1 = ppt->x;
	fullY1 = ppt->y;
	fullX2 = fullX1 + (int) *pwidth;
	ppt++;
	pwidth++;

	if (fullY1 < extentY1 || extentY2 <= fullY1)
	    continue;

	if (fullX1 < extentX1)
	    fullX1 = extentX1;

	if (fullX2 > extentX2)
	    fullX2 = extentX2;

	if (fullX1 >= fullX2)
	    continue;

	nbox = REGION_NUM_RECTS (pClip);
	if (nbox == 1)
	{
	    (*uxa_screen->info->solid) (pPixmap,
					fullX1 + off_x, fullY1 + off_y,
					fullX2 + off_x, fullY1 + 1 + off_y);
	}
	else
	{
	    pbox = REGION_RECTS(pClip);
	    while(nbox--)
	    {
		if (pbox->y1 <= fullY1 && fullY1 < pbox->y2)
		{
		    partX1 = pbox->x1;
		    if (partX1 < fullX1)
			partX1 = fullX1;
		    partX2 = pbox->x2;
		    if (partX2 > fullX2)
			partX2 = fullX2;
		    if (partX2 > partX1) {
			(*uxa_screen->info->solid) (pPixmap,
						    partX1 + off_x, fullY1 + off_y,
						    partX2 + off_x, fullY1 + 1 + off_y);
		    }
		}
		pbox++;
	    }
	}
    }
    (*uxa_screen->info->done_solid) (pPixmap);
}

static Bool
uxa_do_put_image (DrawablePtr pDrawable, GCPtr pGC, int depth, int x, int y,
		  int w, int h, int format, char *bits, int src_stride)
{
    uxa_screen_t    *uxa_screen = uxa_get_screen(pDrawable->pScreen);
    PixmapPtr pPix = uxa_get_drawable_pixmap (pDrawable);
    RegionPtr pClip;
    BoxPtr pbox;
    int nbox;
    int xoff, yoff;
    int bpp = pDrawable->bitsPerPixel;
    Bool access_prepared = FALSE;

    /* Don't bother with under 8bpp, XYPixmaps. */
    if (format != ZPixmap || bpp < 8)
	return FALSE;

    /* Only accelerate copies: no rop or planemask. */
    if (!UXA_PM_IS_SOLID(pDrawable, pGC->planemask) || pGC->alu != GXcopy)
	return FALSE;

    if (uxa_screen->swappedOut)
	return FALSE;

    pPix = uxa_get_offscreen_pixmap (pDrawable, &xoff, &yoff);

    if (!pPix || !uxa_screen->info->put_image)
	return FALSE;

    x += pDrawable->x;
    y += pDrawable->y;

    pClip = fbGetCompositeClip(pGC);
    for (nbox = REGION_NUM_RECTS(pClip),
	 pbox = REGION_RECTS(pClip);
	 nbox--;
	 pbox++)
    {
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
					 x2 - x1, y2 - y1, src, src_stride);
	/* If we fail to accelerate the upload, fall back to using unaccelerated
	 * fb calls.
	 */
	if (!ok) {
	    FbStip *dst;
	    FbStride dst_stride;
	    int	dstBpp;
	    int	dstXoff, dstYoff;

	    if (!access_prepared) {
		if (!uxa_prepare_access(pDrawable, UXA_ACCESS_RW))
		    return FALSE;
		access_prepared = TRUE;
	    }

	    fbGetStipDrawable(pDrawable, dst, dst_stride, dstBpp,
			      dstXoff, dstYoff);

	    fbBltStip((FbStip *)bits + (y1 - y) * (src_stride / sizeof(FbStip)),
		      src_stride / sizeof(FbStip),
		      (x1 - x) * dstBpp,
		      dst + (y1 + dstYoff) * dst_stride,
		      dst_stride,
		      (x1 + dstXoff) * dstBpp,
		      (x2 - x1) * dstBpp,
		      y2 - y1,
		      GXcopy, FB_ALLONES, dstBpp);
	}
    }

    if (access_prepared)
	uxa_finish_access(pDrawable);

    return TRUE;
}

#ifdef MITSHM

static Bool
uxa_do_shm_put_image(DrawablePtr pDrawable, GCPtr pGC, int depth,
		 unsigned int format, int w, int h, int sx, int sy, int sw,
		 int sh, int dx, int dy, char *data)
{
    int src_stride = PixmapBytePad(w, depth);

    if (uxa_do_put_image(pDrawable, pGC, depth, dx, dy, sw, sh, format, data +
			 sy * src_stride + sx * BitsPerPixel(depth) / 8,
			 src_stride))
	return TRUE;

    if (format == ZPixmap)
    {
	PixmapPtr pPixmap;

	pPixmap = GetScratchPixmapHeader(pDrawable->pScreen, w, h, depth,
					 BitsPerPixel(depth), PixmapBytePad(w, depth),
					 (pointer)data);
	if (!pPixmap)
	    return FALSE;

        if (!uxa_prepare_access (pDrawable, UXA_ACCESS_RW))
	    return FALSE;
	fbCopyArea((DrawablePtr)pPixmap, pDrawable, pGC, sx, sy, sw, sh, dx, dy);
	uxa_finish_access(pDrawable);

	FreeScratchPixmapHeader(pPixmap);

	return TRUE;
    }

    return FALSE;
}

/* The actual ShmPutImage isn't wrapped by the damage layer, so we need to
 * inform any interested parties of the damage incurred to the drawable.
 *
 * We also need to set the pending damage to ensure correct migration in all
 * cases.
 */
void
uxa_shm_put_image(DrawablePtr pDrawable, GCPtr pGC, int depth, unsigned int format,
		  int w, int h, int sx, int sy, int sw, int sh, int dx, int dy,
		  char *data)
{
    if (!uxa_do_shm_put_image(pDrawable, pGC, depth, format, w, h, sx, sy, sw, sh,
			      dx, dy, data)) {
	if (!uxa_prepare_access (pDrawable, UXA_ACCESS_RW))
	    return;
	fbShmPutImage(pDrawable, pGC, depth, format, w, h, sx, sy, sw, sh, dx, dy,
		      data);
	uxa_finish_access(pDrawable);
    }
}

ShmFuncs uxa_shm_funcs = { NULL, uxa_shm_put_image };

#endif

static void
uxa_put_image (DrawablePtr pDrawable, GCPtr pGC, int depth, int x, int y,
	     int w, int h, int leftPad, int format, char *bits)
{
#ifdef MITSHM
    if (!uxa_do_shm_put_image(pDrawable, pGC, depth, format, w, h, 0, 0, w, h, x, y,
			  bits))
#else
    if (!uxa_do_put_image(pDrawable, pGC, depth, x, y, w, h, format, bits,
		       PixmapBytePad(w, pDrawable->depth)))
#endif
	uxa_check_put_image(pDrawable, pGC, depth, x, y, w, h, leftPad, format,
			 bits);
}

static Bool inline
uxa_copy_n_to_n_two_dir (DrawablePtr pSrcDrawable, DrawablePtr pDstDrawable,
		   GCPtr pGC, BoxPtr pbox, int nbox, int dx, int dy)
{
    uxa_screen_t *uxa_screen = uxa_get_screen(pDstDrawable->pScreen);
    PixmapPtr pSrcPixmap, pDstPixmap;
    int src_off_x, src_off_y, dst_off_x, dst_off_y;
    int dirsetup;

    /* Need to get both pixmaps to call the driver routines */
    pSrcPixmap = uxa_get_offscreen_pixmap (pSrcDrawable, &src_off_x, &src_off_y);
    pDstPixmap = uxa_get_offscreen_pixmap (pDstDrawable, &dst_off_x, &dst_off_y);
    if (!pSrcPixmap || !pDstPixmap)
	return FALSE;

    /*
     * Now the case of a chip that only supports xdir = ydir = 1 or
     * xdir = ydir = -1, but we have xdir != ydir.
     */
    dirsetup = 0;	/* No direction set up yet. */
    for (; nbox; pbox++, nbox--) {
	if (dx >= 0 && (src_off_y + pbox->y1 + dy) != pbox->y1) {
	    /* Do a xdir = ydir = -1 blit instead. */
	    if (dirsetup != -1) {
		if (dirsetup != 0)
		    uxa_screen->info->done_copy(pDstPixmap);
		dirsetup = -1;
		if (!(*uxa_screen->info->prepare_copy)(pSrcPixmap,
						   pDstPixmap,
						   -1, -1,
						   pGC ? pGC->alu : GXcopy,
						   pGC ? pGC->planemask :
							 FB_ALLONES))
		    return FALSE;
	    }
	    (*uxa_screen->info->copy)(pDstPixmap,
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
		if (!(*uxa_screen->info->prepare_copy)(pSrcPixmap,
						   pDstPixmap,
						   1, 1,
						   pGC ? pGC->alu : GXcopy,
						   pGC ? pGC->planemask :
							 FB_ALLONES))
		    return FALSE;
	    }
	    (*uxa_screen->info->copy)(pDstPixmap,
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
		if (!(*uxa_screen->info->prepare_copy)(pSrcPixmap,
						   pDstPixmap,
						   1, 1,
						   pGC ? pGC->alu : GXcopy,
						   pGC ? pGC->planemask :
							 FB_ALLONES))
		    return FALSE;
	    }
	    for (i = pbox->y2 - pbox->y1 - 1; i >= 0; i--)
		(*uxa_screen->info->copy)(pDstPixmap,
				       src_off_x + pbox->x1 + dx,
				       src_off_y + pbox->y1 + dy + i,
				       dst_off_x + pbox->x1,
				       dst_off_y + pbox->y1 + i,
				       pbox->x2 - pbox->x1, 1);
	} else {
	    /*
	     * xdir = -1, ydir = 1.
	     * Perform line-by-line xdir = ydir = -1 blits, going down.
	     */
	    int i;
	    if (dirsetup != -1) {
		if (dirsetup != 0)
		    uxa_screen->info->done_copy(pDstPixmap);
		dirsetup = -1;
		if (!(*uxa_screen->info->prepare_copy)(pSrcPixmap,
						   pDstPixmap,
						   -1, -1,
						   pGC ? pGC->alu : GXcopy,
						   pGC ? pGC->planemask :
							 FB_ALLONES))
		    return FALSE;
	    }
	    for (i = 0; i < pbox->y2 - pbox->y1; i++)
		(*uxa_screen->info->copy)(pDstPixmap,
				       src_off_x + pbox->x1 + dx,
				       src_off_y + pbox->y1 + dy + i,
				       dst_off_x + pbox->x1,
				       dst_off_y + pbox->y1 + i,
				       pbox->x2 - pbox->x1, 1);
	}
    }
    if (dirsetup != 0)
	uxa_screen->info->done_copy(pDstPixmap);
    return TRUE;
}

void
uxa_copy_n_to_n (DrawablePtr    pSrcDrawable,
		 DrawablePtr    pDstDrawable,
		 GCPtr		pGC,
		 BoxPtr		pbox,
		 int		nbox,
		 int		dx,
		 int		dy,
		 Bool		reverse,
		 Bool		upsidedown,
		 Pixel		bitplane,
		 void		*closure)
{
    ScreenPtr       screen = pDstDrawable->pScreen;
    uxa_screen_t    *uxa_screen = uxa_get_screen(screen);
    int		    src_off_x, src_off_y;
    int		    dst_off_x, dst_off_y;
    PixmapPtr	    pSrcPixmap, pDstPixmap;

    pSrcPixmap = uxa_get_drawable_pixmap (pSrcDrawable);
    pDstPixmap = uxa_get_drawable_pixmap (pDstDrawable);

    uxa_get_drawable_deltas (pSrcDrawable, pSrcPixmap, &src_off_x, &src_off_y);
    uxa_get_drawable_deltas (pDstDrawable, pDstPixmap, &dst_off_x, &dst_off_y);

    /* Mixed directions must be handled specially if the card is lame */
    if ((uxa_screen->info->flags & UXA_TWO_BITBLT_DIRECTIONS) &&
	reverse != upsidedown) {
	if (uxa_copy_n_to_n_two_dir(pSrcDrawable, pDstDrawable, pGC, pbox, nbox,
			       dx, dy))
	    return;
	goto fallback;
    }

    if (!uxa_pixmap_is_offscreen(pSrcPixmap) ||
	!uxa_pixmap_is_offscreen(pDstPixmap) ||
	!(*uxa_screen->info->prepare_copy) (pSrcPixmap, pDstPixmap, reverse ? -1 : 1,
					   upsidedown ? -1 : 1,
					   pGC ? pGC->alu : GXcopy,
					   pGC ? pGC->planemask : FB_ALLONES)) {
	goto fallback;
    }

    while (nbox--)
    {
	(*uxa_screen->info->copy) (pDstPixmap,
				   pbox->x1 + dx + src_off_x,
				   pbox->y1 + dy + src_off_y,
				   pbox->x1 + dst_off_x, pbox->y1 + dst_off_y,
				   pbox->x2 - pbox->x1, pbox->y2 - pbox->y1);
	pbox++;
    }

    (*uxa_screen->info->done_copy) (pDstPixmap);

    return;

fallback:
    UXA_FALLBACK(("from %p to %p (%c,%c)\n", pSrcDrawable, pDstDrawable,
		  uxa_drawable_location(pSrcDrawable),
		  uxa_drawable_location(pDstDrawable)));
    if (uxa_prepare_access (pDstDrawable, UXA_ACCESS_RW)) {
	if (uxa_prepare_access (pSrcDrawable, UXA_ACCESS_RO)) {
	    fbCopyNtoN (pSrcDrawable, pDstDrawable, pGC, pbox, nbox, dx, dy,
			reverse, upsidedown, bitplane, closure);
	    uxa_finish_access (pSrcDrawable);
	}
	uxa_finish_access (pDstDrawable);
    }
}

RegionPtr
uxa_copy_area(DrawablePtr pSrcDrawable, DrawablePtr pDstDrawable, GCPtr pGC,
	    int srcx, int srcy, int width, int height, int dstx, int dsty)
{
    uxa_screen_t    *uxa_screen = uxa_get_screen(pDstDrawable->pScreen);

    if (uxa_screen->swappedOut) {
        return  uxa_check_copy_area(pSrcDrawable, pDstDrawable, pGC,
                                 srcx, srcy, width, height, dstx, dsty);
    }

    return miDoCopy (pSrcDrawable, pDstDrawable, pGC,
		     srcx, srcy, width, height,
		     dstx, dsty, uxa_copy_n_to_n, 0, NULL);
}

static void
uxa_poly_point(DrawablePtr pDrawable, GCPtr pGC, int mode, int npt,
	     DDXPointPtr ppt)
{
    int i;
    xRectangle *prect;

    /* If we can't reuse the current GC as is, don't bother accelerating the
     * points.
     */
    if (pGC->fillStyle != FillSolid) {
	uxa_check_poly_point(pDrawable, pGC, mode, npt, ppt);
	return;
    }

    prect = xalloc(sizeof(xRectangle) * npt);
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
    xfree(prect);
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

    /* Don't try to do wide lines or non-solid fill style. */
    if (pGC->lineWidth != 0 || pGC->lineStyle != LineSolid ||
	pGC->fillStyle != FillSolid) {
	uxa_check_poly_lines(pDrawable, pGC, mode, npt, ppt);
	return;
    }

    prect = xalloc(sizeof(xRectangle) * (npt - 1));
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
	    xfree(prect);
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
    xfree(prect);
}

/**
 * uxa_poly_segment() checks if it can accelerate the lines as a group of
 * horizontal or vertical lines (rectangles), and uses existing rectangle fill
 * acceleration if so.
 */
static void
uxa_poly_segment (DrawablePtr pDrawable, GCPtr pGC, int nseg,
		xSegment *pSeg)
{
    xRectangle *prect;
    int i;

    /* Don't try to do wide lines or non-solid fill style. */
    if (pGC->lineWidth != 0 || pGC->lineStyle != LineSolid ||
	pGC->fillStyle != FillSolid)
    {
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

    prect = xalloc(sizeof(xRectangle) * nseg);
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
    xfree(prect);
}

static Bool uxa_fill_region_solid (DrawablePtr pDrawable, RegionPtr pRegion,
				Pixel pixel, CARD32 planemask, CARD32 alu);

static void
uxa_poly_fill_rect(DrawablePtr pDrawable,
		GCPtr	    pGC,
		int	    nrect,
		xRectangle  *prect)
{
    uxa_screen_t    *uxa_screen = uxa_get_screen(pDrawable->pScreen);
    RegionPtr	    pClip = fbGetCompositeClip(pGC);
    PixmapPtr	    pPixmap = uxa_get_drawable_pixmap(pDrawable);
    register BoxPtr pbox;
    BoxPtr	    pextent;
    int		    extentX1, extentX2, extentY1, extentY2;
    int		    fullX1, fullX2, fullY1, fullY2;
    int		    partX1, partX2, partY1, partY2;
    int		    xoff, yoff;
    int		    xorg, yorg;
    int		    n;
    RegionPtr pReg = RECTS_TO_REGION(pScreen, nrect, prect, CT_UNSORTED);

    /* Compute intersection of rects and clip region */
    REGION_TRANSLATE(pScreen, pReg, pDrawable->x, pDrawable->y);
    REGION_INTERSECT(pScreen, pReg, pClip, pReg);

    if (!REGION_NUM_RECTS(pReg))
	goto out;

    uxa_get_drawable_deltas(pDrawable, pPixmap, &xoff, &yoff);

    if (uxa_screen->swappedOut)
	goto fallback;

    /* For ROPs where overlaps don't matter, convert rectangles to region and
     * call uxa_fill_region_{solid,tiled}.
     */
    if ((pGC->fillStyle == FillSolid || pGC->fillStyle == FillTiled) &&
	(nrect == 1 || pGC->alu == GXcopy || pGC->alu == GXclear ||
	 pGC->alu == GXnoop || pGC->alu == GXcopyInverted ||
	 pGC->alu == GXset)) {
	if (((pGC->fillStyle == FillSolid || pGC->tileIsPixel) &&
	     uxa_fill_region_solid(pDrawable, pReg, pGC->fillStyle == FillSolid ?
				pGC->fgPixel : pGC->tile.pixel,	pGC->planemask,
				pGC->alu)) ||
	    (pGC->fillStyle == FillTiled && !pGC->tileIsPixel &&
	     uxa_fill_region_tiled(pDrawable, pReg, pGC->tile.pixmap, &pGC->patOrg,
				pGC->planemask, pGC->alu))) {
	    goto out;
	}
    }

    if (pGC->fillStyle != FillSolid &&
	!(pGC->tileIsPixel && pGC->fillStyle == FillTiled))
    {
	goto fallback;
    }

    if (!uxa_pixmap_is_offscreen (pPixmap) ||
	!(*uxa_screen->info->prepare_solid) (pPixmap,
					 pGC->alu,
					 pGC->planemask,
					 pGC->fgPixel))
    {
fallback:
	uxa_check_poly_fill_rect (pDrawable, pGC, nrect, prect);
	goto out;
    }

    xorg = pDrawable->x;
    yorg = pDrawable->y;

    pextent = REGION_EXTENTS(pGC->pScreen, pClip);
    extentX1 = pextent->x1;
    extentY1 = pextent->y1;
    extentX2 = pextent->x2;
    extentY2 = pextent->y2;
    while (nrect--)
    {
	fullX1 = prect->x + xorg;
	fullY1 = prect->y + yorg;
	fullX2 = fullX1 + (int) prect->width;
	fullY2 = fullY1 + (int) prect->height;
	prect++;

	if (fullX1 < extentX1)
	    fullX1 = extentX1;

	if (fullY1 < extentY1)
	    fullY1 = extentY1;

	if (fullX2 > extentX2)
	    fullX2 = extentX2;

	if (fullY2 > extentY2)
	    fullY2 = extentY2;

	if ((fullX1 >= fullX2) || (fullY1 >= fullY2))
	    continue;
	n = REGION_NUM_RECTS (pClip);
	if (n == 1)
	{
	    (*uxa_screen->info->solid) (pPixmap,
				     fullX1 + xoff, fullY1 + yoff,
				     fullX2 + xoff, fullY2 + yoff);
	}
	else
	{
	    pbox = REGION_RECTS(pClip);
	    /*
	     * clip the rectangle to each box in the clip region
	     * this is logically equivalent to calling Intersect(),
	     * but rectangles may overlap each other here.
	     */
	    while(n--)
	    {
		partX1 = pbox->x1;
		if (partX1 < fullX1)
		    partX1 = fullX1;
		partY1 = pbox->y1;
		if (partY1 < fullY1)
		    partY1 = fullY1;
		partX2 = pbox->x2;
		if (partX2 > fullX2)
		    partX2 = fullX2;
		partY2 = pbox->y2;
		if (partY2 > fullY2)
		    partY2 = fullY2;

		pbox++;

		if (partX1 < partX2 && partY1 < partY2) {
		    (*uxa_screen->info->solid) (pPixmap,
					     partX1 + xoff, partY1 + yoff,
					     partX2 + xoff, partY2 + yoff);
		}
	    }
	}
    }
    (*uxa_screen->info->done_solid) (pPixmap);

out:
    REGION_UNINIT(pScreen, pReg);
    REGION_DESTROY(pScreen, pReg);
}

const GCOps uxa_ops = {
    uxa_fill_spans,
    uxa_check_set_spans,
    uxa_put_image,
    uxa_copy_area,
    uxa_check_copy_plane,
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
    uxa_check_image_glyph_blt,
    uxa_check_poly_glyph_blt,
    uxa_check_push_pixels,
};

void
uxa_copy_window(WindowPtr pWin, DDXPointRec ptOldOrg, RegionPtr prgnSrc)
{
    RegionRec	rgnDst;
    int		dx, dy;
    PixmapPtr	pPixmap = (*pWin->drawable.pScreen->GetWindowPixmap) (pWin);

    dx = ptOldOrg.x - pWin->drawable.x;
    dy = ptOldOrg.y - pWin->drawable.y;
    REGION_TRANSLATE(pWin->drawable.pScreen, prgnSrc, -dx, -dy);

    REGION_INIT (pWin->drawable.pScreen, &rgnDst, NullBox, 0);

    REGION_INTERSECT(pWin->drawable.pScreen, &rgnDst, &pWin->borderClip, prgnSrc);
#ifdef COMPOSITE
    if (pPixmap->screen_x || pPixmap->screen_y)
	REGION_TRANSLATE (pWin->drawable.pScreen, &rgnDst,
			  -pPixmap->screen_x, -pPixmap->screen_y);
#endif

    miCopyRegion (&pPixmap->drawable, &pPixmap->drawable,
		  NULL,
		  &rgnDst, dx, dy, uxa_copy_n_to_n, 0, NULL);

    REGION_UNINIT(pWin->drawable.pScreen, &rgnDst);
}

static Bool
uxa_fill_region_solid (DrawablePtr	pDrawable,
		    RegionPtr	pRegion,
		    Pixel	pixel,
		    CARD32	planemask,
		    CARD32	alu)
{
    uxa_screen_t    *uxa_screen = uxa_get_screen(pDrawable->pScreen);
    PixmapPtr	    pPixmap = uxa_get_drawable_pixmap (pDrawable);
    int		    xoff, yoff;
    Bool	    ret = FALSE;

    uxa_get_drawable_deltas(pDrawable, pPixmap, &xoff, &yoff);
    REGION_TRANSLATE(pScreen, pRegion, xoff, yoff);

    if (uxa_pixmap_is_offscreen (pPixmap) &&
	(*uxa_screen->info->prepare_solid) (pPixmap, alu, planemask, pixel))
    {
	int nbox;
	BoxPtr pBox;

	nbox = REGION_NUM_RECTS (pRegion);
	pBox = REGION_RECTS (pRegion);

	while (nbox--)
	{
	    (*uxa_screen->info->solid) (pPixmap, pBox->x1, pBox->y1, pBox->x2,
				     pBox->y2);
	    pBox++;
	}
	(*uxa_screen->info->done_solid) (pPixmap);

	ret = TRUE;
    }

    REGION_TRANSLATE(pScreen, pRegion, -xoff, -yoff);

    return ret;
}

/* Try to do an accelerated tile of the pTile into pRegion of pDrawable.
 * Based on fbFillRegionTiled(), fbTile().
 */
Bool
uxa_fill_region_tiled (DrawablePtr	pDrawable,
		       RegionPtr	pRegion,
		       PixmapPtr	pTile,
		       DDXPointPtr	pPatOrg,
		       CARD32		planemask,
		       CARD32		alu)
{
    uxa_screen_t    *uxa_screen = uxa_get_screen(pDrawable->pScreen);
    PixmapPtr	    pPixmap;
    int		    xoff, yoff;
    int		    tileWidth, tileHeight;
    int		    nbox = REGION_NUM_RECTS (pRegion);
    BoxPtr	    pBox = REGION_RECTS (pRegion);
    Bool	    ret = FALSE;

    tileWidth = pTile->drawable.width;
    tileHeight = pTile->drawable.height;

    /* If we're filling with a solid color, grab it out and go to
     * FillRegionsolid, saving numerous copies.
     */
    if (tileWidth == 1 && tileHeight == 1)
	return uxa_fill_region_solid(pDrawable, pRegion,
				     uxa_get_pixmap_first_pixel (pTile), planemask,
				     alu);

    pPixmap = uxa_get_drawable_pixmap (pDrawable);
    uxa_get_drawable_deltas(pDrawable, pPixmap, &xoff, &yoff);
    REGION_TRANSLATE(pScreen, pRegion, xoff, yoff);

    pPixmap = uxa_get_offscreen_pixmap (pDrawable, &xoff, &yoff);

    if (!pPixmap || !uxa_pixmap_is_offscreen(pTile))
	goto out;

    if ((*uxa_screen->info->prepare_copy) (pTile, pPixmap, 1, 1, alu, planemask))
    {
	while (nbox--)
	{
	    int height = pBox->y2 - pBox->y1;
	    int dstY = pBox->y1;
	    int tileY;

	    modulus(dstY - yoff - pDrawable->y - pPatOrg->y, tileHeight, tileY);

	    while (height > 0) {
		int width = pBox->x2 - pBox->x1;
		int dstX = pBox->x1;
		int tileX;
		int h = tileHeight - tileY;

		if (h > height)
		    h = height;
		height -= h;

		modulus(dstX - xoff - pDrawable->x - pPatOrg->x, tileWidth,
			tileX);

		while (width > 0) {
		    int w = tileWidth - tileX;
		    if (w > width)
			w = width;
		    width -= w;

		    (*uxa_screen->info->copy) (pPixmap, tileX, tileY, dstX, dstY,
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

#ifndef SERVER_1_5
void
uxa_paint_window(WindowPtr pWin, RegionPtr pRegion, int what)
{
    ScreenPtr       screen = pWin->drawable.pScreen;
    uxa_screen_t    *uxa_screen = uxa_get_screen(screen);
    DDXPointRec     zeros = { 0, 0 };

    if (REGION_NIL(pRegion))
	return;

    if (uxa_screen->swappedOut) {
	uxa_check_paint_window(pWin, pRegion, what);
	return;
    }

    switch (what) {
    case PW_BACKGROUND:
	switch (pWin->backgroundState) {
	case None:
	    return;
	case ParentRelative:
	    do {
		pWin = pWin->parent;
	    } while (pWin->backgroundState == ParentRelative);
	    (*pWin->drawable.pScreen->PaintWindowBackground)(pWin, pRegion,
							     what);
	    return;
	case BackgroundPixel:
	    if (uxa_fill_region_solid(&pWin->drawable, pRegion,
				      pWin->background.pixel,
				      FB_ALLONES, GXcopy))
		return;
	    break;
	case BackgroundPixmap:
	    if (uxa_fill_region_tiled(&pWin->drawable, pRegion,
				      pWin->background.pixmap,
				      &zeros, FB_ALLONES, GXcopy))
		return;
	    break;
	}
	break;
    case PW_BORDER:
	if (pWin->borderIsPixel) {
	    if (uxa_fill_region_solid(&pWin->drawable, pRegion,
				      pWin->border.pixel,
				      FB_ALLONES, GXcopy))
		return;
	    break;
	} else {
	    if (uxa_fill_region_tiled(&pWin->drawable, pRegion,
				      pWin->border.pixmap,
				      &zeros, FB_ALLONES, GXcopy))
		return;
	    break;
	}
	break;
    }

    uxa_check_paint_window(pWin, pRegion, what);
}
#endif /* !SERVER_1_5 */

/**
 * Accelerates GetImage for solid ZPixmap downloads from framebuffer memory.
 *
 * This is probably the only case we actually care about.  The rest fall through
 * to migration and fbGetImage, which hopefully will result in migration pushing
 * the pixmap out of framebuffer.
 */
void
uxa_get_image (DrawablePtr pDrawable, int x, int y, int w, int h,
	       unsigned int format, unsigned long planeMask, char *d)
{
    ScreenPtr       screen = pDrawable->pScreen;
    uxa_screen_t    *uxa_screen = uxa_get_screen(screen);
    BoxRec	    Box;
    PixmapPtr	    pPix = uxa_get_drawable_pixmap (pDrawable);
    int		    xoff, yoff;
    Bool	    ok;

    uxa_get_drawable_deltas (pDrawable, pPix, &xoff, &yoff);

    Box.x1 = pDrawable->y + x + xoff;
    Box.y1 = pDrawable->y + y + yoff;
    Box.x2 = Box.x1 + w;
    Box.y2 = Box.y1 + h;

    if (uxa_screen->swappedOut)
	goto fallback;

    pPix = uxa_get_offscreen_pixmap (pDrawable, &xoff, &yoff);

    if (pPix == NULL || uxa_screen->info->get_image == NULL)
	goto fallback;

    /* Only cover the ZPixmap, solid copy case. */
    if (format != ZPixmap || !UXA_PM_IS_SOLID(pDrawable, planeMask))
	goto fallback;

    /* Only try to handle the 8bpp and up cases, since we don't want to think
     * about <8bpp.
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

    if (uxa_prepare_access (pDrawable, UXA_ACCESS_RO)) {
	fbGetImage (pDrawable, x, y, w, h, format, planeMask, d);
	uxa_finish_access (pDrawable);
    }

   return;
}
