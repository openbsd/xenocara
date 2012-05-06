/*
 * Copyright © 2009 Maarten Maathuis
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
 * Author: Based on "exa_driver.c"
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 */

#include "saa_priv.h"
#include "saa.h"

PixmapPtr
saa_create_pixmap(ScreenPtr pScreen, int w, int h, int depth,
		  unsigned usage_hint)
{
    PixmapPtr pPixmap;
    struct saa_pixmap *spix;
    int bpp;
    size_t paddedWidth;
    struct saa_screen_priv *sscreen = saa_screen(pScreen);
    int new_pitch = 0;
    struct saa_driver *driver = sscreen->driver;

    if (w > 32767 || h > 32767)
	return NullPixmap;

    /*
     * Create a scratch pixmap without backing storage (w and h are zero)
     */

    saa_swap(sscreen, pScreen, CreatePixmap);
    pPixmap = pScreen->CreatePixmap(pScreen, 0, 0, depth, usage_hint);
    saa_swap(sscreen, pScreen, CreatePixmap);

    if (!pPixmap)
	goto out_no_pix;

    spix = saa_pixmap(pPixmap);
    memset(spix, 0, driver->pixmap_size);
    REGION_NULL(pScreen, &spix->dirty_shadow);
    REGION_NULL(pScreen, &spix->dirty_hw);
    REGION_NULL(pScreen, &spix->shadow_damage);
    spix->read_access = 0;
    spix->write_access = 0;
    spix->mapped_access = 0;
    spix->addr = NULL;
    spix->auth_loc = saa_loc_override;
    spix->override = SAA_INVALID_ADDRESS;
    spix->pixmap = pPixmap;
    bpp = pPixmap->drawable.bitsPerPixel;

    if (!driver->create_pixmap(driver, spix, w, h, depth,
			       usage_hint, bpp, &new_pitch))
	goto out_no_driver_priv;

    paddedWidth = new_pitch;
    spix->damage = NULL;

    /*
     * Now set w and h to the correct value. This might allocate
     * backing store if w and h are NON-NULL.
     */

    if (!(*pScreen->ModifyPixmapHeader) (pPixmap, w, h, 0, 0,
					 paddedWidth, NULL))
	goto out_no_pixmap_header;

    /*
     * During a fallback we must prepare access. This hack is initially used
     * for pixmaps created during ValidateGC.
     */

    spix->fallback_created = FALSE;
    if (sscreen->fallback_count) {
	if (!saa_prepare_access_pixmap(pPixmap, SAA_ACCESS_W, NULL))
	    goto out_no_access;

	spix->fallback_created = TRUE;
    }

    return pPixmap;
 out_no_access:
 out_no_pixmap_header:
    driver->destroy_pixmap(driver, pPixmap);
 out_no_driver_priv:
    saa_swap(sscreen, pScreen, DestroyPixmap);
    pScreen->DestroyPixmap(pPixmap);
    saa_swap(sscreen, pScreen, DestroyPixmap);
 out_no_pix:
    LogMessage(X_ERROR, "Failing pixmap creation.\n");
    return NullPixmap;
}

Bool
saa_destroy_pixmap(PixmapPtr pPixmap)
{
    ScreenPtr pScreen = pPixmap->drawable.pScreen;
    struct saa_screen_priv *sscreen = saa_screen(pScreen);
    Bool ret;
    struct saa_driver *driver = sscreen->driver;

    if (pPixmap->refcnt == 1) {
	struct saa_pixmap *spix = saa_pixmap(pPixmap);

	if (spix->fallback_created) {
	    if (!sscreen->fallback_count)
		LogMessage(X_ERROR, "Fallback pixmap destroyed outside "
			   "fallback.\n");

	    saa_finish_access_pixmap(pPixmap, SAA_ACCESS_W);
	}

	driver->destroy_pixmap(driver, pPixmap);

	REGION_UNINIT(pScreen, &spix->dirty_hw);
	REGION_UNINIT(pScreen, &spix->dirty_shadow);
	spix->damage = NULL;
    }

    saa_swap(sscreen, pScreen, DestroyPixmap);
    ret = pScreen->DestroyPixmap(pPixmap);
    saa_swap(sscreen, pScreen, DestroyPixmap);

    return ret;
}

Bool
saa_modify_pixmap_header(PixmapPtr pPixmap, int width, int height, int depth,
			 int bitsPerPixel, int devKind, pointer pPixData)
{
    ScreenPtr pScreen;
    struct saa_screen_priv *sscreen;
    struct saa_pixmap *spix;
    struct saa_driver *driver;
    Bool ret = TRUE;

    if (!pPixmap)
	return FALSE;

    pScreen = pPixmap->drawable.pScreen;
    sscreen = saa_screen(pScreen);
    spix = saa_pixmap(pPixmap);
    driver = sscreen->driver;

    if (spix && driver->modify_pixmap_header &&
	driver->modify_pixmap_header(pPixmap, width, height, depth,
				     bitsPerPixel, devKind, pPixData)) {
	spix->auth_loc = saa_loc_driver;
	spix->override = SAA_INVALID_ADDRESS;
	goto out;
    }

    saa_swap(sscreen, pScreen, ModifyPixmapHeader);
    ret = pScreen->ModifyPixmapHeader(pPixmap, width, height, depth,
				      bitsPerPixel, devKind, pPixData);
    saa_swap(sscreen, pScreen, ModifyPixmapHeader);
    spix->override = pPixmap->devPrivate.ptr;
    spix->auth_loc = saa_loc_override;

 out:
    pPixmap->devPrivate.ptr = NULL;
    return ret;
}

struct saa_pixmap *
saa_get_saa_pixmap(PixmapPtr pPixmap)
{
    return saa_pixmap(pPixmap);
}

void
saa_pixmap_dirty(PixmapPtr pixmap, Bool hw, RegionPtr reg)
{
    struct saa_pixmap *spix = saa_pixmap(pixmap);
    struct saa_screen_priv *sscreen = saa_screen(pixmap->drawable.pScreen);

    if (hw) {
	REGION_UNION(pixmap->drawable.pScreen, &spix->dirty_hw,
		     &spix->dirty_hw, reg);
	REGION_SUBTRACT(pixmap->drawable.pScreen, &spix->dirty_shadow,
			&spix->dirty_shadow, reg);
    } else {
	REGION_UNION(pixmap->drawable.pScreen, &spix->dirty_shadow,
		     &spix->dirty_shadow, reg);
	REGION_SUBTRACT(pixmap->drawable.pScreen, &spix->dirty_hw,
			&spix->dirty_hw, reg);
    }

    sscreen->driver->damage(sscreen->driver, pixmap, hw, reg);
}

void
saa_drawable_dirty(DrawablePtr draw, Bool hw, RegionPtr reg)
{
    PixmapPtr pixmap;
    int x_offset, y_offset;

    pixmap = saa_get_pixmap(draw, &x_offset, &y_offset);
    REGION_TRANSLATE(draw->pScreen, reg, x_offset, y_offset);
    saa_pixmap_dirty(pixmap, hw, reg);
    REGION_TRANSLATE(draw->pScreen, reg, -x_offset, -y_offset);
}
