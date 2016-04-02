/*
 * Copyright © 2001 Keith Packard
 *             2010 Intel Corporation
 *             2012,2015 Advanced Micro Devices, Inc.
 *
 * Partly based on code Copyright © 2008 Red Hat, Inc.
 * Partly based on code Copyright © 2000 SuSE, Inc.
 *
 * Partly based on code that is Copyright © The XFree86 Project Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the opyright holders not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The copyright holders make no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef USE_GLAMOR

#include "radeon.h"
#include "radeon_glamor.h"


/**
 * get_drawable_pixmap() returns the backing pixmap for a given drawable.
 *
 * @param pDrawable the drawable being requested.
 *
 * This function returns the backing pixmap for a drawable, whether it is a
 * redirected window, unredirected window, or already a pixmap.
 */
static PixmapPtr
get_drawable_pixmap(DrawablePtr pDrawable)
{
	if (pDrawable->type == DRAWABLE_WINDOW)
		return pDrawable->pScreen->
		    GetWindowPixmap((WindowPtr) pDrawable);
	else
		return (PixmapPtr) pDrawable;
}

/* Are there any outstanding GPU operations for this pixmap? */
static Bool
radeon_glamor_gpu_pending(uint_fast32_t gpu_synced, uint_fast32_t gpu_access)
{
	return (int_fast32_t)(gpu_access - gpu_synced) > 0;
}

/*
 * Pixmap CPU access wrappers
 */

static Bool
radeon_glamor_prepare_access_cpu(ScrnInfoPtr scrn, RADEONInfoPtr info,
				 PixmapPtr pixmap, struct radeon_pixmap *priv,
				 Bool need_sync)
{
	struct radeon_bo *bo = priv->bo;
	int ret;

	/* When falling back to swrast, flush all pending operations */
	if (need_sync) {
		glamor_block_handler(scrn->pScreen);
		info->gpu_flushed++;
	}

	if (!pixmap->devPrivate.ptr) {
		ret = radeon_bo_map(bo, 1);
		if (ret) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "%s: bo map (tiling_flags %d) failed: %s\n",
				   __FUNCTION__,
				   priv->tiling_flags,
				   strerror(-ret));
			return FALSE;
		}

		pixmap->devPrivate.ptr = bo->ptr;
		info->gpu_synced = info->gpu_flushed;
	} else if (need_sync) {
		radeon_bo_wait(bo);
		info->gpu_synced = info->gpu_flushed;
	}

	return TRUE;
}

static Bool
radeon_glamor_prepare_access_cpu_ro(ScrnInfoPtr scrn, PixmapPtr pixmap,
				    struct radeon_pixmap *priv)
{
	RADEONInfoPtr info;
	Bool need_sync;

	if (!priv)
		return TRUE;

	info = RADEONPTR(scrn);
	need_sync = radeon_glamor_gpu_pending(info->gpu_synced, priv->gpu_write);
	return radeon_glamor_prepare_access_cpu(scrn, RADEONPTR(scrn), pixmap,
						priv, need_sync);
}

static Bool
radeon_glamor_prepare_access_cpu_rw(ScrnInfoPtr scrn, PixmapPtr pixmap,
				    struct radeon_pixmap *priv)
{
	RADEONInfoPtr info;
	uint_fast32_t gpu_synced;
	Bool need_sync;

	if (!priv)
		return TRUE;

	info = RADEONPTR(scrn);
	gpu_synced = info->gpu_synced;
	need_sync = radeon_glamor_gpu_pending(gpu_synced, priv->gpu_write) |
		radeon_glamor_gpu_pending(gpu_synced, priv->gpu_read);
	return radeon_glamor_prepare_access_cpu(scrn, info, pixmap, priv,
						need_sync);
}

static void
radeon_glamor_finish_access_cpu(PixmapPtr pixmap)
{
	/* Nothing to do */
}

/*
 * Pixmap GPU access wrappers
 */

static Bool
radeon_glamor_prepare_access_gpu(struct radeon_pixmap *priv)
{
	return priv != NULL;
}

static void
radeon_glamor_finish_access_gpu_ro(RADEONInfoPtr info,
				   struct radeon_pixmap *priv)
{
	priv->gpu_read = info->gpu_flushed + 1;
}

static void
radeon_glamor_finish_access_gpu_rw(RADEONInfoPtr info,
				   struct radeon_pixmap *priv)
{
	priv->gpu_write = priv->gpu_read = info->gpu_flushed + 1;
}

/*
 * GC CPU access wrappers
 */

static Bool
radeon_glamor_prepare_access_gc(ScrnInfoPtr scrn, GCPtr pGC)
{
	struct radeon_pixmap *priv;

	if (pGC->stipple) {
		priv = radeon_get_pixmap_private(pGC->stipple);
		if (!radeon_glamor_prepare_access_cpu_ro(scrn, pGC->stipple, priv))
			return FALSE;
	}
	if (pGC->fillStyle == FillTiled) {
		priv = radeon_get_pixmap_private(pGC->tile.pixmap);
		if (!radeon_glamor_prepare_access_cpu_ro(scrn, pGC->tile.pixmap,
						      priv)) {
			if (pGC->stipple)
				radeon_glamor_finish_access_cpu(pGC->stipple);
			return FALSE;
		}
	}
	return TRUE;
}

static void
radeon_glamor_finish_access_gc(GCPtr pGC)
{
	if (pGC->fillStyle == FillTiled)
		radeon_glamor_finish_access_cpu(pGC->tile.pixmap);
	if (pGC->stipple)
		radeon_glamor_finish_access_cpu(pGC->stipple);
}

/*
 * Picture CPU access wrappers
 */

static void
radeon_glamor_picture_finish_access_cpu(PicturePtr picture)
{
	/* Nothing to do */
}

static Bool
radeon_glamor_picture_prepare_access_cpu_ro(ScrnInfoPtr scrn,
					    PicturePtr picture)
{
	PixmapPtr pixmap;
	struct radeon_pixmap *priv;

	if (picture->pDrawable == NULL)
		return TRUE;

	pixmap = get_drawable_pixmap(picture->pDrawable);
	priv = radeon_get_pixmap_private(pixmap);
	if (!radeon_glamor_prepare_access_cpu_ro(scrn, pixmap, priv))
		return FALSE;

	if (picture->alphaMap) {
		pixmap = get_drawable_pixmap(picture->alphaMap->pDrawable);
		priv = radeon_get_pixmap_private(pixmap);
		if (!radeon_glamor_prepare_access_cpu_ro(scrn, pixmap, priv)) {
			radeon_glamor_picture_finish_access_cpu(picture);
			return FALSE;
		}
	}

	return TRUE;
}

static Bool
radeon_glamor_picture_prepare_access_cpu_rw(ScrnInfoPtr scrn,
					    PicturePtr picture)
{
	PixmapPtr pixmap;
	struct radeon_pixmap *priv;

	pixmap = get_drawable_pixmap(picture->pDrawable);
	priv = radeon_get_pixmap_private(pixmap);
	if (!radeon_glamor_prepare_access_cpu_rw(scrn, pixmap, priv))
		return FALSE;

	if (picture->alphaMap) {
		pixmap = get_drawable_pixmap(picture->alphaMap->pDrawable);
		priv = radeon_get_pixmap_private(pixmap);
		if (!radeon_glamor_prepare_access_cpu_rw(scrn, pixmap, priv)) {
			radeon_glamor_picture_finish_access_cpu(picture);
			return FALSE;
		}
	}

	return TRUE;
}

/*
 * GC rendering wrappers
 */

static void
radeon_glamor_fill_spans(DrawablePtr pDrawable, GCPtr pGC, int nspans,
			 DDXPointPtr ppt, int *pwidth, int fSorted)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
	PixmapPtr pixmap = get_drawable_pixmap(pDrawable);
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

	if (radeon_glamor_prepare_access_cpu_rw(scrn, pixmap, priv)) {
		if (radeon_glamor_prepare_access_gc(scrn, pGC)) {
			fbFillSpans(pDrawable, pGC, nspans, ppt, pwidth,
				    fSorted);
			radeon_glamor_finish_access_gc(pGC);
		}
		radeon_glamor_finish_access_cpu(pixmap);
	}
}

static void
radeon_glamor_set_spans(DrawablePtr pDrawable, GCPtr pGC, char *psrc,
			DDXPointPtr ppt, int *pwidth, int nspans, int fSorted)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
	PixmapPtr pixmap = get_drawable_pixmap(pDrawable);
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

	if (radeon_glamor_prepare_access_cpu_rw(scrn, pixmap, priv)) {
		fbSetSpans(pDrawable, pGC, psrc, ppt, pwidth, nspans, fSorted);
		radeon_glamor_finish_access_cpu(pixmap);
	}
}

static void
radeon_glamor_put_image(DrawablePtr pDrawable, GCPtr pGC, int depth,
			int x, int y, int w, int h, int leftPad, int format,
			char *bits)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
	PixmapPtr pixmap = get_drawable_pixmap(pDrawable);
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

	if (radeon_glamor_prepare_access_cpu_rw(scrn, pixmap, priv)) {
		fbPutImage(pDrawable, pGC, depth, x, y, w, h, leftPad, format,
			   bits);
		radeon_glamor_finish_access_cpu(pixmap);
	}
}

static RegionPtr
radeon_glamor_copy_plane(DrawablePtr pSrc, DrawablePtr pDst, GCPtr pGC,
			 int srcx, int srcy, int w, int h, int dstx, int dsty,
			 unsigned long bitPlane)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDst->pScreen);
	PixmapPtr dst_pix = get_drawable_pixmap(pDst);
	struct radeon_pixmap *dst_priv = radeon_get_pixmap_private(dst_pix);
	RegionPtr ret = NULL;

	if (radeon_glamor_prepare_access_cpu_rw(scrn, dst_pix, dst_priv)) {
		PixmapPtr src_pix = get_drawable_pixmap(pSrc);
		struct radeon_pixmap *src_priv = radeon_get_pixmap_private(src_pix);
		if (radeon_glamor_prepare_access_cpu_ro(scrn, src_pix, src_priv)) {
			ret =
			    fbCopyPlane(pSrc, pDst, pGC, srcx, srcy, w, h, dstx,
					dsty, bitPlane);
			radeon_glamor_finish_access_cpu(src_pix);
		}
		radeon_glamor_finish_access_cpu(dst_pix);
	}
	return ret;
}

static RegionPtr
radeon_glamor_copy_plane_nodstbo(DrawablePtr pSrc, DrawablePtr pDst, GCPtr pGC,
				 int srcx, int srcy, int w, int h,
				 int dstx, int dsty, unsigned long bitPlane)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDst->pScreen);
	PixmapPtr src_pix = get_drawable_pixmap(pSrc);
	struct radeon_pixmap *src_priv = radeon_get_pixmap_private(src_pix);
	RegionPtr ret = NULL;

	if (radeon_glamor_prepare_access_cpu_ro(scrn, src_pix, src_priv)) {
		ret = fbCopyPlane(pSrc, pDst, pGC, srcx, srcy, w, h,
				  dstx,	dsty, bitPlane);
		radeon_glamor_finish_access_cpu(src_pix);
	}
	return ret;
}

static void
radeon_glamor_poly_point(DrawablePtr pDrawable, GCPtr pGC, int mode, int npt,
			 DDXPointPtr pptInit)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
	PixmapPtr pixmap = get_drawable_pixmap(pDrawable);
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

	if (radeon_glamor_prepare_access_cpu_rw(scrn, pixmap, priv)) {
		fbPolyPoint(pDrawable, pGC, mode, npt, pptInit);
		radeon_glamor_finish_access_cpu(pixmap);
	}
}

static void
radeon_glamor_poly_lines(DrawablePtr pDrawable, GCPtr pGC,
			 int mode, int npt, DDXPointPtr ppt)
{
	if (pGC->lineWidth == 0) {
		ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
		PixmapPtr pixmap = get_drawable_pixmap(pDrawable);
		struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

		if (radeon_glamor_prepare_access_cpu_rw(scrn, pixmap, priv)) {
			if (radeon_glamor_prepare_access_gc(scrn, pGC)) {
				fbPolyLine(pDrawable, pGC, mode, npt, ppt);
				radeon_glamor_finish_access_gc(pGC);
			}
			radeon_glamor_finish_access_cpu(pixmap);
		}
		return;
	}
	/* fb calls mi functions in the lineWidth != 0 case. */
	fbPolyLine(pDrawable, pGC, mode, npt, ppt);
}

static void
radeon_glamor_poly_segment(DrawablePtr pDrawable, GCPtr pGC,
			   int nsegInit, xSegment *pSegInit)
{
	if (pGC->lineWidth == 0) {
		ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
		PixmapPtr pixmap = get_drawable_pixmap(pDrawable);
		struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

		if (radeon_glamor_prepare_access_cpu_rw(scrn, pixmap, priv)) {
			if (radeon_glamor_prepare_access_gc(scrn, pGC)) {
				fbPolySegment(pDrawable, pGC, nsegInit,
					      pSegInit);
				radeon_glamor_finish_access_gc(pGC);
			}
			radeon_glamor_finish_access_cpu(pixmap);
		}
		return;
	}
	/* fb calls mi functions in the lineWidth != 0 case. */
	fbPolySegment(pDrawable, pGC, nsegInit, pSegInit);
}

static void
radeon_glamor_poly_fill_rect(DrawablePtr pDrawable, GCPtr pGC,
			     int nrect, xRectangle *prect)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
	RADEONInfoPtr info = RADEONPTR(scrn);
	PixmapPtr pixmap = get_drawable_pixmap(pDrawable);
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

	if ((info->accel_state->force || (priv && !priv->bo)) &&
	    radeon_glamor_prepare_access_gpu(priv)) {
		info->glamor.SavedPolyFillRect(pDrawable, pGC, nrect, prect);
		radeon_glamor_finish_access_gpu_rw(info, priv);
		return;
	}

	if (radeon_glamor_prepare_access_cpu_rw(scrn, pixmap, priv)) {
		if (radeon_glamor_prepare_access_gc(scrn, pGC)) {
			fbPolyFillRect(pDrawable, pGC, nrect, prect);
			radeon_glamor_finish_access_gc(pGC);
		}
		radeon_glamor_finish_access_cpu(pixmap);
	}
}

static void
radeon_glamor_image_glyph_blt(DrawablePtr pDrawable, GCPtr pGC,
			      int x, int y, unsigned int nglyph,
			      CharInfoPtr *ppci, pointer pglyphBase)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
	PixmapPtr pixmap = get_drawable_pixmap(pDrawable);
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

	if (radeon_glamor_prepare_access_cpu_rw(scrn, pixmap, priv)) {
		if (radeon_glamor_prepare_access_gc(scrn, pGC)) {
			fbImageGlyphBlt(pDrawable, pGC, x, y, nglyph, ppci,
					pglyphBase);
			radeon_glamor_finish_access_gc(pGC);
		}
		radeon_glamor_finish_access_cpu(pixmap);
	}
}

static void
radeon_glamor_poly_glyph_blt(DrawablePtr pDrawable, GCPtr pGC,
			     int x, int y, unsigned int nglyph,
			     CharInfoPtr *ppci, pointer pglyphBase)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
	PixmapPtr pixmap = get_drawable_pixmap(pDrawable);
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

	if (radeon_glamor_prepare_access_cpu_rw(scrn, pixmap, priv)) {
		if (radeon_glamor_prepare_access_gc(scrn, pGC)) {
			fbPolyGlyphBlt(pDrawable, pGC, x, y, nglyph, ppci,
				       pglyphBase);
			radeon_glamor_finish_access_gc(pGC);
		}
		radeon_glamor_finish_access_cpu(pixmap);
	}
}

static void
radeon_glamor_push_pixels(GCPtr pGC, PixmapPtr pBitmap,
			  DrawablePtr pDrawable, int w, int h, int x, int y)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
	PixmapPtr pixmap = get_drawable_pixmap(pDrawable);
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

	if (radeon_glamor_prepare_access_cpu_rw(scrn, pixmap, priv)) {
		priv = radeon_get_pixmap_private(pBitmap);
		if (radeon_glamor_prepare_access_cpu_ro(scrn, pBitmap, priv)) {
			if (radeon_glamor_prepare_access_gc(scrn, pGC)) {
				fbPushPixels(pGC, pBitmap, pDrawable, w, h, x,
					     y);
				radeon_glamor_finish_access_gc(pGC);
			}
			radeon_glamor_finish_access_cpu(pBitmap);
		}
		radeon_glamor_finish_access_cpu(pixmap);
	}
}

static void
radeon_glamor_push_pixels_nodstbo(GCPtr pGC, PixmapPtr pBitmap,
				  DrawablePtr pDrawable, int w, int h,
				  int x, int y)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pBitmap);

	if (radeon_glamor_prepare_access_cpu_ro(scrn, pBitmap, priv)) {
		fbPushPixels(pGC, pBitmap, pDrawable, w, h, x, y);
		radeon_glamor_finish_access_cpu(pBitmap);
	}
}

static RegionPtr
radeon_glamor_copy_area(DrawablePtr pSrcDrawable, DrawablePtr pDstDrawable,
			GCPtr pGC, int srcx, int srcy, int width, int height,
			int dstx, int dsty)
{
	ScreenPtr screen = pDstDrawable->pScreen;
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONInfoPtr info = RADEONPTR(scrn);
	PixmapPtr src_pixmap = get_drawable_pixmap(pSrcDrawable);
	PixmapPtr dst_pixmap = get_drawable_pixmap(pDstDrawable);
	struct radeon_pixmap *src_priv = radeon_get_pixmap_private(src_pixmap);
	struct radeon_pixmap *dst_priv = radeon_get_pixmap_private(dst_pixmap);
	RegionPtr ret = NULL;

	if (info->accel_state->force || (src_priv && !src_priv->bo) ||
	    (dst_priv && !dst_priv->bo)) {
		if (!radeon_glamor_prepare_access_gpu(dst_priv))
			goto fallback;
		if (src_priv != dst_priv &&
		    !radeon_glamor_prepare_access_gpu(src_priv))
			goto fallback;

		ret = info->glamor.SavedCopyArea(pSrcDrawable, pDstDrawable,
						 pGC, srcx, srcy,
						 width, height, dstx, dsty);
		radeon_glamor_finish_access_gpu_rw(info, dst_priv);
		if (src_priv != dst_priv)
			radeon_glamor_finish_access_gpu_ro(info, src_priv);

		return ret;
	}

fallback:
	if (radeon_glamor_prepare_access_cpu_rw(scrn, dst_pixmap, dst_priv)) {
		if (pSrcDrawable == pDstDrawable ||
			radeon_glamor_prepare_access_cpu_ro(scrn, src_pixmap,
							    src_priv)) {
			ret = fbCopyArea(pSrcDrawable, pDstDrawable, pGC,
					 srcx, srcy, width, height, dstx, dsty);
			if (pSrcDrawable != pDstDrawable)
				radeon_glamor_finish_access_cpu(src_pixmap);
		}
		radeon_glamor_finish_access_cpu(dst_pixmap);
	}

	return ret;
}

static RegionPtr
radeon_glamor_copy_area_nodstbo(DrawablePtr pSrcDrawable,
				DrawablePtr pDstDrawable, GCPtr pGC,
				int srcx, int srcy, int width, int height,
				int dstx, int dsty)
{
	ScreenPtr screen = pDstDrawable->pScreen;
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	PixmapPtr src_pixmap = get_drawable_pixmap(pSrcDrawable);
	PixmapPtr dst_pixmap = get_drawable_pixmap(pDstDrawable);
	struct radeon_pixmap *src_priv;
	RegionPtr ret = NULL;

	if (src_pixmap != dst_pixmap) {
		src_priv = radeon_get_pixmap_private(src_pixmap);

		if (!radeon_glamor_prepare_access_cpu_ro(scrn, src_pixmap,
							 src_priv))
			return ret;
	}

	ret = fbCopyArea(pSrcDrawable, pDstDrawable, pGC, srcx, srcy,
			 width, height, dstx, dsty);

	if (src_pixmap != dst_pixmap)
		radeon_glamor_finish_access_cpu(src_pixmap);

	return ret;
}

static const GCOps radeon_glamor_ops = {
	radeon_glamor_fill_spans,
	radeon_glamor_set_spans,
	radeon_glamor_put_image,
	radeon_glamor_copy_area,
	radeon_glamor_copy_plane,
	radeon_glamor_poly_point,
	radeon_glamor_poly_lines,
	radeon_glamor_poly_segment,
	miPolyRectangle,
	miPolyArc,
	miFillPolygon,
	radeon_glamor_poly_fill_rect,
	miPolyFillArc,
	miPolyText8,
	miPolyText16,
	miImageText8,
	miImageText16,
	radeon_glamor_image_glyph_blt,
	radeon_glamor_poly_glyph_blt,
	radeon_glamor_push_pixels,
};

static GCOps radeon_glamor_nodstbo_ops;

/**
 * radeon_glamor_validate_gc() sets the ops to our implementations, which may be
 * accelerated or may sync the card and fall back to fb.
 */
static void
radeon_glamor_validate_gc(GCPtr pGC, unsigned long changes, DrawablePtr pDrawable)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pGC->pScreen);
	RADEONInfoPtr info = RADEONPTR(scrn);

	glamor_validate_gc(pGC, changes, pDrawable);
	info->glamor.SavedCopyArea = pGC->ops->CopyArea;
	info->glamor.SavedPolyFillRect = pGC->ops->PolyFillRect;

	if (radeon_get_pixmap_private(get_drawable_pixmap(pDrawable)) ||
	    (pGC->stipple && radeon_get_pixmap_private(pGC->stipple)) ||
	    (pGC->fillStyle == FillTiled &&
	     radeon_get_pixmap_private(pGC->tile.pixmap)))
		pGC->ops = (GCOps *)&radeon_glamor_ops;
	else
		pGC->ops = &radeon_glamor_nodstbo_ops;
}

static GCFuncs glamorGCFuncs = {
	radeon_glamor_validate_gc,
	miChangeGC,
	miCopyGC,
	miDestroyGC,
	miChangeClip,
	miDestroyClip,
	miCopyClip
};

/**
 * radeon_glamor_create_gc makes a new GC and hooks up its funcs handler, so that
 * radeon_glamor_validate_gc() will get called.
 */
static int
radeon_glamor_create_gc(GCPtr pGC)
{
	static Bool nodstbo_ops_initialized;

	if (!fbCreateGC(pGC))
		return FALSE;

	if (!nodstbo_ops_initialized) {
		radeon_glamor_nodstbo_ops = radeon_glamor_ops;

		radeon_glamor_nodstbo_ops.FillSpans = pGC->ops->FillSpans;
		radeon_glamor_nodstbo_ops.SetSpans = pGC->ops->SetSpans;
		radeon_glamor_nodstbo_ops.PutImage = pGC->ops->PutImage;
		radeon_glamor_nodstbo_ops.CopyArea = radeon_glamor_copy_area_nodstbo;
		radeon_glamor_nodstbo_ops.CopyPlane = radeon_glamor_copy_plane_nodstbo;
		radeon_glamor_nodstbo_ops.PolyPoint = pGC->ops->PolyPoint;
		radeon_glamor_nodstbo_ops.Polylines = pGC->ops->Polylines;
		radeon_glamor_nodstbo_ops.PolySegment = pGC->ops->PolySegment;
		radeon_glamor_nodstbo_ops.PolyFillRect = pGC->ops->PolyFillRect;
		radeon_glamor_nodstbo_ops.ImageGlyphBlt = pGC->ops->ImageGlyphBlt;
		radeon_glamor_nodstbo_ops.PolyGlyphBlt = pGC->ops->PolyGlyphBlt;
		radeon_glamor_nodstbo_ops.PushPixels = radeon_glamor_push_pixels_nodstbo;

		nodstbo_ops_initialized = TRUE;
	}

	pGC->funcs = &glamorGCFuncs;

	return TRUE;
}

/*
 * Screen rendering wrappers
 */

static RegionPtr
radeon_glamor_bitmap_to_region(PixmapPtr pPix)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pPix->drawable.pScreen);
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pPix);
	RegionPtr ret;

	if (!radeon_glamor_prepare_access_cpu_ro(scrn, pPix, priv))
		return NULL;
	ret = fbPixmapToRegion(pPix);
	radeon_glamor_finish_access_cpu(pPix);
	return ret;
}

static void
radeon_glamor_copy_window(WindowPtr pWin, DDXPointRec ptOldOrg,
			  RegionPtr prgnSrc)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pWin->drawable.pScreen);
	PixmapPtr pixmap = get_drawable_pixmap(&pWin->drawable);
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

	if (radeon_glamor_prepare_access_cpu_rw(scrn, pixmap, priv)) {
		fbCopyWindow(pWin, ptOldOrg, prgnSrc);
		radeon_glamor_finish_access_cpu(pixmap);
	}
}

static void
radeon_glamor_get_image(DrawablePtr pDrawable, int x, int y, int w, int h,
			unsigned int format, unsigned long planeMask, char *d)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
	PixmapPtr pixmap = get_drawable_pixmap(pDrawable);
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

	if (radeon_glamor_prepare_access_cpu_ro(scrn, pixmap, priv)) {
		fbGetImage(pDrawable, x, y, w, h, format, planeMask, d);
		radeon_glamor_finish_access_cpu(pixmap);
	}
}

static void
radeon_glamor_get_spans(DrawablePtr pDrawable, int wMax, DDXPointPtr ppt,
			int *pwidth, int nspans, char *pdstStart)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDrawable->pScreen);
	PixmapPtr pixmap = get_drawable_pixmap(pDrawable);
	struct radeon_pixmap *priv = radeon_get_pixmap_private(pixmap);

	if (radeon_glamor_prepare_access_cpu_ro(scrn, pixmap, priv)) {
		fbGetSpans(pDrawable, wMax, ppt, pwidth, nspans, pdstStart);
		radeon_glamor_finish_access_cpu(pixmap);
	}
}

/*
 * Picture screen rendering wrappers
 */

#ifdef RENDER

static void
radeon_glamor_composite(CARD8 op,
		    PicturePtr pSrc,
		    PicturePtr pMask,
		    PicturePtr pDst,
		    INT16 xSrc, INT16 ySrc,
		    INT16 xMask, INT16 yMask,
		    INT16 xDst, INT16 yDst,
		    CARD16 width, CARD16 height)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pDst->pDrawable->pScreen);
	RADEONInfoPtr info;
	PixmapPtr pixmap;
	struct radeon_pixmap *dst_priv, *src_priv = NULL, *mask_priv = NULL;
	Bool gpu_done = FALSE;

	if (pDst->alphaMap || pSrc->alphaMap || (pMask && pMask->alphaMap))
		goto fallback;

	pixmap = get_drawable_pixmap(pDst->pDrawable);
	if (&pixmap->drawable != pDst->pDrawable ||
	    pixmap->usage_hint != RADEON_CREATE_PIXMAP_SCANOUT)
		goto fallback;

	dst_priv = radeon_get_pixmap_private(pixmap);
	if (!radeon_glamor_prepare_access_gpu(dst_priv))
		goto fallback;

	info = RADEONPTR(scrn);
	if (!pSrc->pDrawable ||
	    ((pixmap = get_drawable_pixmap(pSrc->pDrawable)) &&
	     (src_priv = radeon_get_pixmap_private(pixmap)) &&
	     radeon_glamor_prepare_access_gpu(src_priv))) {
		if (!pMask || !pMask->pDrawable ||
		    ((pixmap = get_drawable_pixmap(pMask->pDrawable)) &&
		     (mask_priv = radeon_get_pixmap_private(pixmap)) &&
		     radeon_glamor_prepare_access_gpu(mask_priv))) {
			info->glamor.SavedComposite(op, pSrc, pMask, pDst,
						    xSrc, ySrc, xMask, yMask,
						    xDst, yDst, width, height);
			gpu_done = TRUE;

			if (mask_priv)
				radeon_glamor_finish_access_gpu_ro(info, mask_priv);
		}

		if (src_priv)
			radeon_glamor_finish_access_gpu_ro(info, src_priv);
	}
	radeon_glamor_finish_access_gpu_rw(info, dst_priv);

	if (gpu_done)
		return;

fallback:
	if (radeon_glamor_picture_prepare_access_cpu_rw(scrn, pDst)) {
		if (radeon_glamor_picture_prepare_access_cpu_ro(scrn, pSrc)) {
			if (!pMask ||
			    radeon_glamor_picture_prepare_access_cpu_ro(scrn, pMask)) {
				fbComposite(op, pSrc, pMask, pDst,
					    xSrc, ySrc,
					    xMask, yMask,
					    xDst, yDst,
					    width, height);
				if (pMask)
					radeon_glamor_picture_finish_access_cpu(pMask);
			}
			radeon_glamor_picture_finish_access_cpu(pSrc);
		}
		radeon_glamor_picture_finish_access_cpu(pDst);
	}
}

static void
radeon_glamor_add_traps(PicturePtr pPicture,
		    INT16 x_off, INT16 y_off, int ntrap, xTrap *traps)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pPicture->pDrawable->pScreen);

	if (radeon_glamor_picture_prepare_access_cpu_rw(scrn, pPicture)) {
		fbAddTraps(pPicture, x_off, y_off, ntrap, traps);
		radeon_glamor_picture_finish_access_cpu(pPicture);
	}
}

static void
radeon_glamor_glyphs(CARD8 op,
		 PicturePtr src,
		 PicturePtr dst,
		 PictFormatPtr maskFormat,
		 INT16 xSrc,
		 INT16 ySrc, int nlist, GlyphListPtr list, GlyphPtr *glyphs)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(dst->pDrawable->pScreen);

	if (radeon_glamor_picture_prepare_access_cpu_rw(scrn, dst)) {
		if (radeon_glamor_picture_prepare_access_cpu_ro(scrn, src)) {
			RADEONInfoPtr info = RADEONPTR(scrn);

			info->glamor.SavedGlyphs(op, src, dst, maskFormat, xSrc,
						 ySrc, nlist, list, glyphs);
			radeon_glamor_picture_finish_access_cpu(src);
		}
		radeon_glamor_picture_finish_access_cpu(dst);
	}
}

static void
radeon_glamor_trapezoids(CARD8 op, PicturePtr src, PicturePtr dst,
		     PictFormatPtr maskFormat, INT16 xSrc, INT16 ySrc,
		     int ntrap, xTrapezoid *traps)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(dst->pDrawable->pScreen);

	if (radeon_glamor_picture_prepare_access_cpu_rw(scrn, dst)) {
		if (radeon_glamor_picture_prepare_access_cpu_ro(scrn, src)) {
			RADEONInfoPtr info = RADEONPTR(scrn);

			info->glamor.SavedTrapezoids(op, src, dst, maskFormat,
						     xSrc, ySrc, ntrap, traps);
			radeon_glamor_picture_finish_access_cpu(src);
		}
		radeon_glamor_picture_finish_access_cpu(dst);
	}
}

static void
radeon_glamor_triangles(CARD8 op, PicturePtr src, PicturePtr dst,
		    PictFormatPtr maskFormat, INT16 xSrc, INT16 ySrc,
		    int ntri, xTriangle *tri)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(dst->pDrawable->pScreen);

	if (radeon_glamor_picture_prepare_access_cpu_rw(scrn, dst)) {
		if (radeon_glamor_picture_prepare_access_cpu_ro(scrn, src)) {
			RADEONInfoPtr info = RADEONPTR(scrn);

			info->glamor.SavedTriangles(op, src, dst, maskFormat,
						    xSrc, ySrc, ntri, tri);
			radeon_glamor_picture_finish_access_cpu(src);
		}
		radeon_glamor_picture_finish_access_cpu(dst);
	}
}

#endif /* RENDER */


/**
 * radeon_glamor_close_screen() unwraps its wrapped screen functions and tears
 * down our screen private, before calling down to the next CloseScreen.
 */
static Bool
radeon_glamor_close_screen(CLOSE_SCREEN_ARGS_DECL)
{
	RADEONInfoPtr info = RADEONPTR(xf86ScreenToScrn(pScreen));
#ifdef RENDER
	PictureScreenPtr ps = GetPictureScreenIfSet(pScreen);
#endif

	pScreen->CreateGC = info->glamor.SavedCreateGC;
	pScreen->CloseScreen = info->glamor.SavedCloseScreen;
	pScreen->GetImage = info->glamor.SavedGetImage;
	pScreen->GetSpans = info->glamor.SavedGetSpans;
	pScreen->CreatePixmap = info->glamor.SavedCreatePixmap;
	pScreen->DestroyPixmap = info->glamor.SavedDestroyPixmap;
	pScreen->CopyWindow = info->glamor.SavedCopyWindow;
	pScreen->ChangeWindowAttributes =
	    info->glamor.SavedChangeWindowAttributes;
	pScreen->BitmapToRegion = info->glamor.SavedBitmapToRegion;
#ifdef RENDER
	if (ps) {
		ps->Composite = info->glamor.SavedComposite;
		ps->Glyphs = info->glamor.SavedGlyphs;
		ps->UnrealizeGlyph = info->glamor.SavedUnrealizeGlyph;
		ps->Trapezoids = info->glamor.SavedTrapezoids;
		ps->AddTraps = info->glamor.SavedAddTraps;
		ps->Triangles = info->glamor.SavedTriangles;

		ps->UnrealizeGlyph = info->glamor.SavedUnrealizeGlyph;
	}
#endif

	return (*pScreen->CloseScreen) (CLOSE_SCREEN_ARGS);
}

/**
 * @param screen screen being initialized
 */
void
radeon_glamor_screen_init(ScreenPtr screen)
{
	RADEONInfoPtr info = RADEONPTR(xf86ScreenToScrn(screen));

	/*
	 * Replace various fb screen functions
	 */
	info->glamor.SavedCloseScreen = screen->CloseScreen;
	screen->CloseScreen = radeon_glamor_close_screen;

	info->glamor.SavedCreateGC = screen->CreateGC;
	screen->CreateGC = radeon_glamor_create_gc;

	info->glamor.SavedGetImage = screen->GetImage;
	screen->GetImage = radeon_glamor_get_image;

	info->glamor.SavedGetSpans = screen->GetSpans;
	screen->GetSpans = radeon_glamor_get_spans;

	info->glamor.SavedCreatePixmap = screen->CreatePixmap;
	info->glamor.SavedDestroyPixmap = screen->DestroyPixmap;

	info->glamor.SavedCopyWindow = screen->CopyWindow;
	screen->CopyWindow = radeon_glamor_copy_window;

	info->glamor.SavedBitmapToRegion = screen->BitmapToRegion;
	screen->BitmapToRegion = radeon_glamor_bitmap_to_region;

#ifdef RENDER
	{
		PictureScreenPtr ps = GetPictureScreenIfSet(screen);
		if (ps) {
			info->glamor.SavedComposite = ps->Composite;
			ps->Composite = radeon_glamor_composite;

			info->glamor.SavedUnrealizeGlyph = ps->UnrealizeGlyph;

			ps->Glyphs = radeon_glamor_glyphs;
			ps->Triangles = radeon_glamor_triangles;
			ps->Trapezoids = radeon_glamor_trapezoids;

			info->glamor.SavedAddTraps = ps->AddTraps;
			ps->AddTraps = radeon_glamor_add_traps;
		}
	}
#endif
}

#endif /* USE_GLAMOR */
