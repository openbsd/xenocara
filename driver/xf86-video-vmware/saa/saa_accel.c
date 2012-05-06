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
#include <mi.h>

Bool
saa_hw_copy_nton(DrawablePtr pSrcDrawable,
		 DrawablePtr pDstDrawable,
		 GCPtr pGC,
		 BoxPtr pbox,
		 int nbox, int dx, int dy, Bool reverse, Bool upsidedown)
{
    ScreenPtr pScreen = pDstDrawable->pScreen;
    struct saa_screen_priv *sscreen = saa_screen(pDstDrawable->pScreen);
    struct saa_driver *driver = sscreen->driver;
    PixmapPtr pSrcPixmap, pDstPixmap;
    struct saa_pixmap *src_spix, *dst_spix;
    int src_off_x, src_off_y;
    int dst_off_x, dst_off_y;
    RegionRec dst_reg, *src_reg;
    int ordering;
    Bool ret = TRUE;

    (void)pScreen;

    /* avoid doing copy operations if no boxes */
    if (nbox == 0)
	return TRUE;

    pSrcPixmap = saa_get_pixmap(pSrcDrawable, &src_off_x, &src_off_y);
    pDstPixmap = saa_get_pixmap(pDstDrawable, &dst_off_x, &dst_off_y);
    src_spix = saa_pixmap(pSrcPixmap);
    dst_spix = saa_pixmap(pDstPixmap);

    if (src_spix->auth_loc != saa_loc_driver ||
	dst_spix->auth_loc != saa_loc_driver)
	return FALSE;


    ordering = (nbox == 1 || (dx > 0 && dy > 0) ||
		(pDstDrawable != pSrcDrawable &&
		 (pDstDrawable->type != DRAWABLE_WINDOW ||
		  pSrcDrawable->type != DRAWABLE_WINDOW))) ?
	CT_YXBANDED : CT_UNSORTED;

    src_reg = saa_boxes_to_region(pScreen, nbox, pbox, ordering);
    if (!src_reg)
	return FALSE;

    REGION_NULL(pScreen, &dst_reg);
    REGION_COPY(pScreen, &dst_reg, src_reg);
    REGION_TRANSLATE(pScreen, src_reg, dx + src_off_x, dy + src_off_y);
    REGION_TRANSLATE(pScreen, &dst_reg, dst_off_x, dst_off_y);

    if (!(driver->copy_prepare) (driver, pSrcPixmap, pDstPixmap,
				 reverse ? -1 : 1,
				 upsidedown ? -1 : 1,
				 pGC ? pGC->alu : GXcopy,
				 src_reg, pGC ? pGC->planemask : FB_ALLONES)) {
	goto fallback;
    }

    while (nbox--) {
	(driver->copy) (driver,
			pbox->x1 + dx + src_off_x,
			pbox->y1 + dy + src_off_y,
			pbox->x1 + dst_off_x, pbox->y1 + dst_off_y,
			pbox->x2 - pbox->x1, pbox->y2 - pbox->y1);
	pbox++;
    }

    (driver->copy_done) (driver);
    saa_pixmap_dirty(pDstPixmap, TRUE, &dst_reg);
    goto out;

 fallback:
    ret = FALSE;

 out:
    REGION_UNINIT(pScreen, &dst_reg);
    REGION_DESTROY(pScreen, src_reg);

    return ret;
}

static void
saa_copy_nton(DrawablePtr pSrcDrawable,
	      DrawablePtr pDstDrawable,
	      GCPtr pGC,
	      BoxPtr pbox,
	      int nbox,
	      int dx,
	      int dy,
	      Bool reverse, Bool upsidedown, Pixel bitplane, void *closure)
{
    if (saa_hw_copy_nton(pSrcDrawable, pDstDrawable, pGC, pbox, nbox, dx, dy,
			 reverse, upsidedown))
	return;

    saa_check_copy_nton(pSrcDrawable, pDstDrawable, pGC, pbox, nbox, dx, dy,
			reverse, upsidedown, bitplane, closure);
}

RegionPtr
saa_copy_area(DrawablePtr pSrcDrawable, DrawablePtr pDstDrawable, GCPtr pGC,
	      int srcx, int srcy, int width, int height, int dstx, int dsty)
{
    struct saa_screen_priv *sscreen = saa_screen(pDstDrawable->pScreen);

    if (sscreen->fallback_count) {
	return saa_check_copy_area(pSrcDrawable, pDstDrawable, pGC,
				   srcx, srcy, width, height, dstx, dsty);
    }

#if (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 6)
    return miDoCopy(pSrcDrawable, pDstDrawable, pGC,
		    srcx, srcy, width, height,
		    dstx, dsty, saa_copy_nton, 0, NULL);
#else
    return fbDoCopy(pSrcDrawable, pDstDrawable, pGC,
		    srcx, srcy, width, height,
		    dstx, dsty, saa_copy_nton, 0, NULL);
#endif
}
