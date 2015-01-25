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

/** @file
 * This file covers the initialization and teardown of SAA, and has various
 * functions not responsible for performing rendering, pixmap migration, or
 * memory management.
 */

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <stdlib.h>

#include "saa_priv.h"
#include <X11/fonts/fontstruct.h>
#include "regionstr.h"
#include "saa.h"
#include "saa_priv.h"

#ifdef SAA_DEVPRIVATEKEYREC
DevPrivateKeyRec saa_screen_index;
DevPrivateKeyRec saa_pixmap_index;
DevPrivateKeyRec saa_gc_index;
#else
int saa_screen_index = -1;
int saa_pixmap_index = -1;
int saa_gc_index = -1;
#endif

/**
 * saa_get_drawable_pixmap() returns a backing pixmap for a given drawable.
 *
 * @param pDrawable the drawable being requested.
 *
 * This function returns the backing pixmap for a drawable, whether it is a
 * redirected window, unredirected window, or already a pixmap.  Note that
 * coordinate translation is needed when drawing to the backing pixmap of a
 * redirected window, and the translation coordinates are provided by calling
 * saa_get_drawable_pixmap() on the drawable.
 */
PixmapPtr
saa_get_drawable_pixmap(DrawablePtr pDrawable)
{
    if (pDrawable->type == DRAWABLE_WINDOW)
	return pDrawable->pScreen->GetWindowPixmap((WindowPtr) pDrawable);
    else
	return (PixmapPtr) pDrawable;
}

/**
 * Sets the offsets to add to coordinates to make them address the same bits in
 * the backing drawable. These coordinates are nonzero only for redirected
 * windows.
 */
void
saa_get_drawable_deltas(DrawablePtr pDrawable, PixmapPtr pPixmap,
			int *xp, int *yp)
{
#ifdef COMPOSITE
    if (pDrawable->type == DRAWABLE_WINDOW) {
	*xp = -pPixmap->screen_x;
	*yp = -pPixmap->screen_y;
	return;
    }
#endif

    *xp = 0;
    *yp = 0;
}

/**
 * Returns the pixmap which backs a drawable, and the offsets to add to
 * coordinates to make them address the same bits in the backing drawable.
 */
PixmapPtr
saa_get_pixmap(DrawablePtr drawable, int *xp, int *yp)
{
    PixmapPtr pixmap = saa_get_drawable_pixmap(drawable);

    saa_get_drawable_deltas(drawable, pixmap, xp, yp);

    return pixmap;
}

static Bool
saa_download_from_hw(PixmapPtr pix, RegionPtr readback)
{
    struct saa_screen_priv *sscreen = saa_screen(pix->drawable.pScreen);
    struct saa_driver *driver = sscreen->driver;
    struct saa_pixmap *spix = saa_pixmap(pix);
    void *addr;
    Bool ret;

    if (spix->mapped_access)
	driver->release_from_cpu(driver, pix, spix->mapped_access);

    ret = driver->download_from_hw(driver, pix, readback);

    if (spix->mapped_access) {
	addr = driver->sync_for_cpu(driver, pix, spix->mapped_access);
	if (addr != NULL)
	    spix->addr = addr;
    }

    return ret;
}

Bool
saa_prepare_access_pixmap(PixmapPtr pix, saa_access_t access,
			  RegionPtr read_reg)
{
    ScreenPtr pScreen = pix->drawable.pScreen;
    struct saa_screen_priv *sscreen = saa_screen(pScreen);
    struct saa_driver *driver = sscreen->driver;
    struct saa_pixmap *spix = saa_pixmap(pix);
    saa_access_t map_access = 0;
    Bool ret = TRUE;

    if (read_reg && REGION_NOTEMPTY(pScreen, read_reg))
	ret = saa_download_from_hw(pix, read_reg);

    if (!ret) {
	LogMessage(X_ERROR, "Prepare access pixmap failed.\n");
	return ret;
    }

    if ((access & SAA_ACCESS_R) != 0 && spix->read_access++ == 0)
	map_access = SAA_ACCESS_R;
    if ((access & SAA_ACCESS_W) != 0 && spix->write_access++ == 0)
	map_access |= SAA_ACCESS_W;

    if (map_access) {
	if (spix->auth_loc != saa_loc_override) {
	    (void)driver->sync_for_cpu(driver, pix, map_access);
	    spix->addr = driver->map(driver, pix, map_access);
	} else
	    spix->addr = spix->override;
	spix->mapped_access |= map_access;
    }

    pix->devPrivate.ptr = spix->addr;
    return TRUE;
}

void
saa_finish_access_pixmap(PixmapPtr pix, saa_access_t access)
{
    struct saa_screen_priv *sscreen = saa_screen(pix->drawable.pScreen);
    struct saa_driver *driver = sscreen->driver;
    struct saa_pixmap *spix = saa_pixmap(pix);
    saa_access_t unmap_access = 0;

    if ((access & SAA_ACCESS_R) != 0 && --spix->read_access == 0)
	unmap_access = SAA_ACCESS_R;
    if ((access & SAA_ACCESS_W) != 0 && --spix->write_access == 0)
	unmap_access |= SAA_ACCESS_W;

    if (spix->read_access < 0)
	LogMessage(X_ERROR, "Incorrect read access.\n");
    if (spix->write_access < 0)
	LogMessage(X_ERROR, "Incorrect write access.\n");

    if (unmap_access) {
	if (spix->auth_loc != saa_loc_override) {
	    driver->unmap(driver, pix, unmap_access);
	    driver->release_from_cpu(driver, pix, unmap_access);
	}
	spix->mapped_access &= ~unmap_access;
    }
    if (!spix->mapped_access) {
	spix->addr = NULL;
	pix->devPrivate.ptr = SAA_INVALID_ADDRESS;
    }
}

/*
 * Callback that is called after a rendering operation. We try to
 * determine whether it's a shadow damage or a hw damage and call the
 * driver callback.
 */

static void
saa_report_damage(DamagePtr damage, RegionPtr reg, void *closure)
{
    PixmapPtr pixmap = (PixmapPtr) closure;
    struct saa_pixmap *spix = saa_get_saa_pixmap(pixmap);
    struct saa_driver *driver = saa_screen(pixmap->drawable.pScreen)->driver;

    if (spix->read_access || spix->write_access)
	LogMessage(X_ERROR, "Damage report inside prepare access.\n");

    driver->operation_complete(driver, pixmap);
    DamageEmpty(damage);
}

Bool
saa_add_damage(PixmapPtr pixmap)
{
    ScreenPtr pScreen = pixmap->drawable.pScreen;
    struct saa_pixmap *spix = saa_get_saa_pixmap(pixmap);

    if (spix->damage)
	return TRUE;

    spix->damage = DamageCreate(saa_report_damage, NULL,
				DamageReportRawRegion, TRUE, pScreen, pixmap);
    if (!spix->damage)
	return FALSE;

    DamageRegister(&pixmap->drawable, spix->damage);
    DamageSetReportAfterOp(spix->damage, TRUE);

    return TRUE;
}

static inline RegionPtr
saa_pix_damage_region(struct saa_pixmap *spix)
{
    return (spix->damage ? DamageRegion(spix->damage) : NULL);
}

Bool
saa_pad_read(DrawablePtr draw)
{
    ScreenPtr pScreen = draw->pScreen;
    PixmapPtr pix;
    int xp;
    int yp;
    BoxRec box;
    RegionRec entire;
    Bool ret;

    (void)pScreen;
    pix = saa_get_pixmap(draw, &xp, &yp);

    box.x1 = draw->x + xp;
    box.y1 = draw->y + yp;
    box.x2 = box.x1 + draw->width;
    box.y2 = box.y1 + draw->height;

    REGION_INIT(pScreen, &entire, &box, 1);
    ret = saa_prepare_access_pixmap(pix, SAA_ACCESS_R, &entire);
    REGION_UNINIT(pScreen, &entire);
    return ret;
}

Bool
saa_pad_read_box(DrawablePtr draw, int x, int y, int w, int h)
{
    ScreenPtr pScreen = draw->pScreen;
    PixmapPtr pix;
    int xp;
    int yp;
    BoxRec box;
    RegionRec entire;
    Bool ret;

    (void)pScreen;
    pix = saa_get_pixmap(draw, &xp, &yp);

    box.x1 = x + xp;
    box.y1 = y + yp;
    box.x2 = box.x1 + w;
    box.y2 = box.y1 + h;

    REGION_INIT(pScreen, &entire, &box, 1);
    ret = saa_prepare_access_pixmap(pix, SAA_ACCESS_R, &entire);
    REGION_UNINIT(pScreen, &entire);
    return ret;
}

/**
 * Prepares a drawable destination for access, and maps it read-write.
 * If check_read is TRUE, pGC should point to a valid GC. The drawable
 * may then be mapped write-only if the pending operation admits.
 */

Bool
saa_pad_write(DrawablePtr draw, GCPtr pGC, Bool check_read,
	      saa_access_t * access)
{
    int xp;
    int yp;
    PixmapPtr pixmap = saa_get_pixmap(draw, &xp, &yp);
    struct saa_pixmap *spix = saa_pixmap(pixmap);

    *access = SAA_ACCESS_W;

    /*
     * If the to-be-damaged area doesn't depend at all on previous
     * rendered contents, we don't need to do any readback.
     */

    if (check_read && !saa_gc_reads_destination(draw, pGC))
	return saa_prepare_access_pixmap(pixmap, *access, NULL);

    *access |= SAA_ACCESS_R;

    /*
     * Read back the area to be damaged.
     */

    return saa_prepare_access_pixmap(pixmap, *access,
				     saa_pix_damage_pending(spix));
}

void
saa_fad_read(DrawablePtr draw)
{
    saa_finish_access_pixmap(saa_get_drawable_pixmap(draw), SAA_ACCESS_R);
}

void
saa_fad_write(DrawablePtr draw, saa_access_t access)
{
    PixmapPtr pix = saa_get_drawable_pixmap(draw);
    struct saa_pixmap *spix = saa_pixmap(pix);

    saa_finish_access_pixmap(pix, access);
    if (spix->damage)
	saa_pixmap_dirty(pix, FALSE, saa_pix_damage_pending(spix));
}

Bool
saa_gc_reads_destination(DrawablePtr pDrawable, GCPtr pGC)
{
    return ((pGC->alu != GXcopy && pGC->alu != GXclear && pGC->alu != GXset &&
	     pGC->alu != GXcopyInverted) || pGC->fillStyle == FillStippled ||
	    pGC->clientClip != NULL ||
	    !SAA_PM_IS_SOLID(pDrawable, pGC->planemask));
}

Bool
saa_op_reads_destination(CARD8 op)
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

static void
saa_validate_gc(GCPtr pGC, unsigned long changes, DrawablePtr pDrawable)
{
    /* fbValidateGC will do direct access to pixmaps if the tiling has changed.
     * Do a few smart things so fbValidateGC can do it's work.
     */

    ScreenPtr pScreen = pDrawable->pScreen;
    struct saa_screen_priv *sscreen = saa_screen(pScreen);
    struct saa_gc_priv *sgc = saa_gc(pGC);
    PixmapPtr pTile = NULL;
    Bool finish_current_tile = FALSE;

    /* Either of these conditions is enough to trigger access to a tile pixmap. */
    /* With pGC->tileIsPixel == 1, you run the risk of dereferencing an invalid tile pixmap pointer. */
    if (pGC->fillStyle == FillTiled
	|| ((changes & GCTile) && !pGC->tileIsPixel)) {
	pTile = pGC->tile.pixmap;

	/* Sometimes tile pixmaps are swapped, you need access to:
	 * - The current tile if it depth matches.
	 * - Or the rotated tile if that one matches depth and !(changes & GCTile).
	 * - Or the current tile pixmap and a newly created one.
	 */
	if (pTile && pTile->drawable.depth != pDrawable->depth
	    && !(changes & GCTile)) {
	    PixmapPtr pRotatedTile = fbGetRotatedPixmap(pGC);

	    if (pRotatedTile
		&& pRotatedTile->drawable.depth == pDrawable->depth)
		pTile = pRotatedTile;
	    else
		finish_current_tile = TRUE;	/* CreatePixmap will be called. */
	}
    }

    if (pGC->stipple && !saa_pad_read(&pGC->stipple->drawable)) {
	LogMessage(X_ERROR, "Failed stipple prepareaccess.\n");
	return;
    }

    if (pTile && !saa_pad_read(&pTile->drawable)) {
	LogMessage(X_ERROR, "Failed stipple prepareaccess.\n");
	goto out_no_tile;
    }

    /* Calls to Create/DestroyPixmap have to be identified as special, so
     * up sscreen->fallback_count.
     */

    sscreen->fallback_count++;
    saa_swap(sgc, pGC, funcs);
    (*pGC->funcs->ValidateGC) (pGC, changes, pDrawable);
    saa_swap(sgc, pGC, funcs);

    if (finish_current_tile && pGC->tile.pixmap)
	saa_fad_write(&pGC->tile.pixmap->drawable, SAA_ACCESS_W);
    sscreen->fallback_count--;

    if (pTile)
	saa_fad_read(&pTile->drawable);
 out_no_tile:
    if (pGC->stipple)
	saa_fad_read(&pGC->stipple->drawable);
}

static void
saa_destroy_gc(GCPtr pGC)
{
    struct saa_gc_priv *sgc = saa_gc(pGC);

    saa_swap(sgc, pGC, funcs);
    (*pGC->funcs->DestroyGC) (pGC);
    saa_swap(sgc, pGC, funcs);
}

static void
saa_change_gc(GCPtr pGC, unsigned long mask)
{
    struct saa_gc_priv *sgc = saa_gc(pGC);

    saa_swap(sgc, pGC, funcs);
    (*pGC->funcs->ChangeGC) (pGC, mask);
    saa_swap(sgc, pGC, funcs);
}

static void
saa_copy_gc(GCPtr pGCSrc, unsigned long mask, GCPtr pGCDst)
{
    struct saa_gc_priv *sgc = saa_gc(pGCDst);

    saa_swap(sgc, pGCDst, funcs);
    (*pGCDst->funcs->CopyGC) (pGCSrc, mask, pGCDst);
    saa_swap(sgc, pGCDst, funcs);
}

static void
saa_change_clip(GCPtr pGC, int type, pointer pvalue, int nrects)
{
    struct saa_gc_priv *sgc = saa_gc(pGC);

    saa_swap(sgc, pGC, funcs);
    (*pGC->funcs->ChangeClip) (pGC, type, pvalue, nrects);
    saa_swap(sgc, pGC, funcs);
}

static void
saa_copy_clip(GCPtr pGCDst, GCPtr pGCSrc)
{
    struct saa_gc_priv *sgc = saa_gc(pGCDst);

    saa_swap(sgc, pGCDst, funcs);
    (*pGCDst->funcs->CopyClip) (pGCDst, pGCSrc);
    saa_swap(sgc, pGCDst, funcs);
}

static void
saa_destroy_clip(GCPtr pGC)
{
    struct saa_gc_priv *sgc = saa_gc(pGC);

    saa_swap(sgc, pGC, funcs);
    (*pGC->funcs->DestroyClip) (pGC);
    saa_swap(sgc, pGC, funcs);
}

static GCFuncs saa_gc_funcs = {
    saa_validate_gc,
    saa_change_gc,
    saa_copy_gc,
    saa_destroy_gc,
    saa_change_clip,
    saa_destroy_clip,
    saa_copy_clip
};

/**
 * saa_create_gc makes a new GC and hooks up its funcs handler, so that
 * saa_validate_gc() will get called.
 */
int
saa_create_gc(GCPtr pGC)
{
    ScreenPtr pScreen = pGC->pScreen;
    struct saa_screen_priv *sscreen = saa_screen(pScreen);
    struct saa_gc_priv *sgc = saa_gc(pGC);
    Bool ret;

    saa_swap(sscreen, pScreen, CreateGC);
    ret = pScreen->CreateGC(pGC);
    if (ret) {
	saa_wrap(sgc, pGC, funcs, &saa_gc_funcs);
	saa_wrap(sgc, pGC, ops, &saa_gc_ops);
    }
    saa_swap(sscreen, pScreen, CreateGC);

    return ret;
}

static Bool
saa_prepare_access_window(WindowPtr pWin)
{
    if (pWin->backgroundState == BackgroundPixmap) {
	if (!saa_pad_read(&pWin->background.pixmap->drawable))
	    return FALSE;
    }

    if (pWin->borderIsPixel == FALSE) {
	if (!saa_pad_read(&pWin->border.pixmap->drawable)) {
	    if (pWin->backgroundState == BackgroundPixmap)
		saa_fad_read(&pWin->background.pixmap->drawable);
	    return FALSE;
	}
    }
    return TRUE;
}

static void
saa_finish_access_window(WindowPtr pWin)
{
    if (pWin->backgroundState == BackgroundPixmap)
	saa_fad_read(&pWin->background.pixmap->drawable);

    if (pWin->borderIsPixel == FALSE)
	saa_fad_read(&pWin->border.pixmap->drawable);
}

static Bool
saa_change_window_attributes(WindowPtr pWin, unsigned long mask)
{
    Bool ret;

    if (!saa_prepare_access_window(pWin))
	return FALSE;
    ret = fbChangeWindowAttributes(pWin, mask);
    saa_finish_access_window(pWin);
    return ret;
}

RegionPtr
saa_bitmap_to_region(PixmapPtr pPix)
{
    RegionPtr ret;

    if (!saa_pad_read(&pPix->drawable))
	return NULL;
    ret = fbPixmapToRegion(pPix);
    saa_fad_read(&pPix->drawable);
    return ret;
}

void
saa_set_fallback_debug(ScreenPtr screen, Bool enable)
{
    struct saa_screen_priv *sscreen = saa_screen(screen);

    sscreen->fallback_debug = enable;
}

/**
 * saa_close_screen() unwraps its wrapped screen functions and tears down SAA's
 * screen private, before calling down to the next CloseScreen.
 */
Bool
saa_close_screen(CLOSE_SCREEN_ARGS_DECL)
{
    struct saa_screen_priv *sscreen = saa_screen(pScreen);
    struct saa_driver *driver = sscreen->driver;

    if (pScreen->devPrivate) {
	/* Destroy the pixmap created by miScreenInit() *before*
	 * chaining up as we finalize ourselves here and so this
	 * is the last chance we have of releasing our resources
	 * associated with the Pixmap. So do it first.
	 */
	(void)(*pScreen->DestroyPixmap) (pScreen->devPrivate);
	pScreen->devPrivate = NULL;
    }

    saa_unwrap(sscreen, pScreen, CloseScreen);
    saa_unwrap(sscreen, pScreen, CreateGC);
    saa_unwrap(sscreen, pScreen, ChangeWindowAttributes);
    saa_unwrap(sscreen, pScreen, CreatePixmap);
    saa_unwrap(sscreen, pScreen, DestroyPixmap);
    saa_unwrap(sscreen, pScreen, ModifyPixmapHeader);
    saa_unwrap(sscreen, pScreen, BitmapToRegion);
#ifdef RENDER
    saa_render_takedown(pScreen);
#endif
    saa_unaccel_takedown(pScreen);
    driver->takedown(driver);

    free(sscreen);

    return (*pScreen->CloseScreen) (CLOSE_SCREEN_ARGS);
}

struct saa_driver *
saa_get_driver(ScreenPtr pScreen)
{
    return saa_screen(pScreen)->driver;
}

/**
 * @param pScreen screen being initialized
 * @param pScreenInfo SAA driver record
 *
 * saa_driver_init sets up SAA given a driver record filled in by the driver.
 * pScreenInfo should have been allocated by saa_driver_alloc().  See the
 * comments in _SaaDriver for what must be filled in and what is optional.
 *
 * @return TRUE if SAA was successfully initialized.
 */
Bool
saa_driver_init(ScreenPtr screen, struct saa_driver * saa_driver)
{
    struct saa_screen_priv *sscreen;

    if (!saa_driver)
	return FALSE;

    if (saa_driver->saa_major != SAA_VERSION_MAJOR ||
	saa_driver->saa_minor > SAA_VERSION_MINOR) {
	LogMessage(X_ERROR,
		   "SAA(%d): driver's SAA version requirements "
		   "(%d.%d) are incompatible with SAA version (%d.%d)\n",
		   screen->myNum, saa_driver->saa_major,
		   saa_driver->saa_minor, SAA_VERSION_MAJOR, SAA_VERSION_MINOR);
	return FALSE;
    }
#if 0
    if (!saa_driver->prepare_solid) {
	LogMessage(X_ERROR,
		   "SAA(%d): saa_driver_t::prepare_solid must be "
		   "non-NULL\n", screen->myNum);
	return FALSE;
    }

    if (!saa_driver->prepare_copy) {
	LogMessage(X_ERROR,
		   "SAA(%d): saa_driver_t::prepare_copy must be "
		   "non-NULL\n", screen->myNum);
	return FALSE;
    }
#endif
#ifdef SAA_DEVPRIVATEKEYREC
    if (!dixRegisterPrivateKey(&saa_screen_index, PRIVATE_SCREEN, 0)) {
	LogMessage(X_ERROR, "Failed to register SAA screen private.\n");
	return FALSE;
    }
    if (!dixRegisterPrivateKey(&saa_pixmap_index, PRIVATE_PIXMAP,
			       saa_driver->pixmap_size)) {
	LogMessage(X_ERROR, "Failed to register SAA pixmap private.\n");
	return FALSE;
    }
    if (!dixRegisterPrivateKey(&saa_gc_index, PRIVATE_GC,
			       sizeof(struct saa_gc_priv))) {
	LogMessage(X_ERROR, "Failed to register SAA gc private.\n");
	return FALSE;
    }
#else
    if (!dixRequestPrivate(&saa_screen_index, 0)) {
	LogMessage(X_ERROR, "Failed to register SAA screen private.\n");
	return FALSE;
    }
    if (!dixRequestPrivate(&saa_pixmap_index, saa_driver->pixmap_size)) {
	LogMessage(X_ERROR, "Failed to register SAA pixmap private.\n");
	return FALSE;
    }
    if (!dixRequestPrivate(&saa_gc_index, sizeof(struct saa_gc_priv))) {
	LogMessage(X_ERROR, "Failed to register SAA gc private.\n");
	return FALSE;
    }
#endif

    sscreen = calloc(1, sizeof(*sscreen));

    if (!sscreen) {
	LogMessage(X_WARNING,
		   "SAA(%d): Failed to allocate screen private\n",
		   screen->myNum);
	return FALSE;
    }

    sscreen->driver = saa_driver;
    dixSetPrivate(&screen->devPrivates, &saa_screen_index, sscreen);

    /*
     * Replace various fb screen functions
     */

    saa_wrap(sscreen, screen, CloseScreen, saa_close_screen);
    saa_wrap(sscreen, screen, CreateGC, saa_create_gc);
    saa_wrap(sscreen, screen, ChangeWindowAttributes,
	     saa_change_window_attributes);
    saa_wrap(sscreen, screen, CreatePixmap, saa_create_pixmap);
    saa_wrap(sscreen, screen, DestroyPixmap, saa_destroy_pixmap);
    saa_wrap(sscreen, screen, ModifyPixmapHeader, saa_modify_pixmap_header);

    saa_wrap(sscreen, screen, BitmapToRegion, saa_bitmap_to_region);
    saa_unaccel_setup(screen);
#ifdef RENDER
    saa_render_setup(screen);
#endif

    return TRUE;
}

Bool
saa_resources_init(ScreenPtr screen)
{
/*    if (!saa_glyphs_init(screen))
	return FALSE;
*/
    return TRUE;
}
