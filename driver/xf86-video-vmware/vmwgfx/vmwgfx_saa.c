/*
 * Copyright 2011 VMWare, Inc.
 * All Rights Reserved.
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
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 */

#include <xorg-server.h>
#include <xorgVersion.h>
#include <mi.h>
#include <fb.h>
#include <xf86drmMode.h>
#include <xa_context.h>
#include "vmwgfx_saa.h"
#include "vmwgfx_drmi.h"
#include "vmwgfx_saa_priv.h"

/*
 * Damage to be added as soon as we attach storage to the pixmap.
 */
static Bool
vmwgfx_pixmap_add_damage(PixmapPtr pixmap)
{
    struct saa_pixmap *spix = saa_get_saa_pixmap(pixmap);
    struct vmwgfx_saa_pixmap *vpix = to_vmwgfx_saa_pixmap(spix);
    DrawablePtr draw = &pixmap->drawable;
    BoxRec box;

    if (spix->damage)
	return TRUE;

    if (!saa_add_damage(pixmap))
	return FALSE;

    box.x1 = 0;
    box.x2 = draw->width;
    box.y1 = 0;
    box.y2 = draw->height;

    if (vpix->hw) {
	REGION_RESET(draw->pScreen, &spix->dirty_hw, &box);
	REGION_EMPTY(draw->pScreen, &spix->dirty_shadow);
    } else {
	REGION_RESET(draw->pScreen, &spix->dirty_shadow, &box);
	REGION_EMPTY(draw->pScreen, &spix->dirty_hw);
    }

    return TRUE;
}

static void
vmwgfx_pixmap_remove_damage(PixmapPtr pixmap)
{
    struct saa_pixmap *spix = saa_get_saa_pixmap(pixmap);
    struct vmwgfx_saa_pixmap *vpix = to_vmwgfx_saa_pixmap(spix);

    if (!spix->damage || vpix->hw || vpix->gmr || vpix->malloc)
	return;

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,14,99,2,0)
    DamageUnregister(spix->damage);
#else
    DamageUnregister(&pixmap->drawable, spix->damage);
#endif

    DamageDestroy(spix->damage);
    spix->damage = NULL;
}

static void
vmwgfx_pixmap_remove_present(struct vmwgfx_saa_pixmap *vpix)
{
    if (vpix->dirty_present)
	REGION_DESTROY(pixmap->drawable.pScreen, vpix->dirty_present);
    if (vpix->present_damage)
	REGION_DESTROY(pixmap->drawable.pScreen, vpix->present_damage);
    if (vpix->pending_update)
	REGION_DESTROY(pixmap->drawable.pScreen, vpix->pending_update);
    if (vpix->pending_present)
	REGION_DESTROY(pixmap->drawable.pScreen, vpix->pending_present);
    vpix->dirty_present = NULL;
    vpix->present_damage = NULL;
    vpix->pending_update = NULL;
    vpix->pending_present = NULL;
}

static Bool
vmwgfx_pixmap_add_present(PixmapPtr pixmap, Bool present_opt)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);
    ScreenPtr pScreen = pixmap->drawable.pScreen;
    (void) pScreen;

    if (present_opt) {
	vpix->dirty_present = REGION_CREATE(pScreen, NULL, 0);
	if (!vpix->dirty_present)
	    return FALSE;
	vpix->present_damage = REGION_CREATE(pScreen, NULL, 0);
	if (!vpix->present_damage)
	    goto out_no_present_damage;
    }
    vpix->pending_update = REGION_CREATE(pScreen, NULL, 0);
    if (!vpix->pending_update)
	goto out_no_pending_update;
    vpix->pending_present = REGION_CREATE(pScreen, NULL, 0);
    if (!vpix->pending_present)
	goto out_no_pending_present;

    return TRUE;
  out_no_pending_present:
    REGION_DESTROY(pScreen, vpix->pending_update);
  out_no_pending_update:
    if (vpix->present_damage)
	REGION_DESTROY(pScreen, vpix->present_damage);
  out_no_present_damage:
    if (vpix->dirty_present)
	REGION_DESTROY(pScreen, vpix->dirty_present);
    return FALSE;
}

static void
vmwgfx_pixmap_free_storage(struct vmwgfx_saa_pixmap *vpix)
{
    if (!(vpix->backing & VMWGFX_PIX_MALLOC) && vpix->malloc) {
	free(vpix->malloc);
	vpix->malloc = NULL;
    }
    if (!(vpix->backing & VMWGFX_PIX_SURFACE) && vpix->hw) {
	xa_surface_destroy(vpix->hw);
	vpix->hw = NULL;
    }
    if (!(vpix->backing & VMWGFX_PIX_GMR) && vpix->gmr) {
	vmwgfx_dmabuf_destroy(vpix->gmr);
	vpix->gmr = NULL;
    }
}

static Bool
vmwgfx_pixmap_create_gmr(struct vmwgfx_saa *vsaa, PixmapPtr pixmap)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);
    size_t size;
    struct vmwgfx_dmabuf *gmr;
    void *addr;

    if (vpix->gmr)
	return TRUE;

    size = pixmap->devKind * pixmap->drawable.height;
    gmr = vmwgfx_dmabuf_alloc(vsaa->drm_fd, size);
    if (!gmr)
	return FALSE;

    if (vpix->malloc) {

	addr = vmwgfx_dmabuf_map(gmr);
	if (!addr)
	    goto out_no_transfer;
	memcpy(addr, vpix->malloc, size);
	vmwgfx_dmabuf_unmap(gmr);

    } else if (!vmwgfx_pixmap_add_damage(pixmap))
	goto out_no_transfer;

    vpix->backing |= VMWGFX_PIX_GMR;
    vpix->backing &= ~VMWGFX_PIX_MALLOC;
    vpix->gmr = gmr;

    vmwgfx_pixmap_free_storage(vpix);

    return TRUE;

  out_no_transfer:
    vmwgfx_dmabuf_destroy(gmr);
    return FALSE;
}

static Bool
vmwgfx_pixmap_create_sw(struct vmwgfx_saa *vsaa, PixmapPtr pixmap)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);

    if (!(vpix->backing & (VMWGFX_PIX_MALLOC | VMWGFX_PIX_GMR)))
	return FALSE;

    if (!vpix->malloc && (vpix->backing & VMWGFX_PIX_MALLOC)) {
	vpix->malloc = malloc(pixmap->devKind * pixmap->drawable.height);
	if (!vpix->malloc)
	    goto out_no_malloc;
	if (!vmwgfx_pixmap_add_damage(pixmap))
	    goto out_no_damage;
    } else if (vpix->backing & VMWGFX_PIX_GMR)
	return vmwgfx_pixmap_create_gmr(vsaa, pixmap);

    return TRUE;

  out_no_damage:
    free(vpix->malloc);
    vpix->malloc = NULL;
  out_no_malloc:
    return FALSE;
}


/**
 *
 * Makes sure all presented contents covered by @region are read
 * back and are present in a valid GMR.
 */

static Bool
vmwgfx_pixmap_present_readback(struct vmwgfx_saa *vsaa,
			       PixmapPtr pixmap,
			       RegionPtr region)
{
    struct saa_pixmap *spix = saa_get_saa_pixmap(pixmap);
    struct vmwgfx_saa_pixmap *vpix = to_vmwgfx_saa_pixmap(spix);
    RegionRec intersection;

    if (!spix->damage || !REGION_NOTEMPTY(vsaa->pScreen, &spix->dirty_hw) ||
	!vpix->dirty_present)
	return TRUE;

    /*
     * Intersect dirty region with region to be read back, if any.
     */

    REGION_NULL(vsaa->pScreen, &intersection);
    REGION_COPY(vsaa->pScreen, &intersection, &spix->dirty_hw);
    REGION_INTERSECT(vsaa->pScreen, &intersection, &intersection,
		     vpix->dirty_present);

    if (region)
	REGION_INTERSECT(vsaa->pScreen, &intersection, &intersection, region);

    if (!REGION_NOTEMPTY(vsaa->pScreen, &intersection))
	goto out;

    /*
     * Make really sure there is a GMR to read back to.
     */

    if (!vmwgfx_pixmap_create_gmr(vsaa, pixmap))
	goto out_err;

    if (vmwgfx_present_readback(vsaa->drm_fd, vpix->fb_id,
				&intersection) != 0)
	goto out_err;

    REGION_SUBTRACT(vsaa->pScreen, &spix->dirty_hw,
		    &spix->dirty_hw, &intersection);
  out:
    REGION_UNINIT(vsaa->pScreen, &intersection);
    return TRUE;

  out_err:
    REGION_UNINIT(vsaa->pScreen, &intersection);
    return FALSE;
}

static Bool
vmwgfx_saa_dma(struct vmwgfx_saa *vsaa,
	       PixmapPtr pixmap,
	       RegionPtr reg,
	       Bool to_hw,
	       int dx,
	       int dy,
	       struct xa_surface *srf)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);

    if (!srf)
	srf = vpix->hw;

    if (!srf || (!vpix->gmr && !vpix->malloc))
	return TRUE;

    if (vpix->gmr && vsaa->can_optimize_dma) {
	uint32_t handle, dummy;

	if (_xa_surface_handle(srf, &handle, &dummy) != 0)
	    goto out_err;
	if (vmwgfx_dma(dx, dy, reg, vpix->gmr, pixmap->devKind, handle,
		       to_hw) != 0)
	    goto out_err;
    } else {
	uint8_t *data = (uint8_t *) vpix->malloc;
	int ret;

	if (vpix->gmr) {
	    data = (uint8_t *) vmwgfx_dmabuf_map(vpix->gmr);
	    if (!data)
		goto out_err;
	}

	if (dx || dy) {
	    REGION_TRANSLATE(pScreen, reg, dx, dy);
	    data -= ((dx * pixmap->drawable.bitsPerPixel + 7)/8 +
		     dy * pixmap->devKind);
	}

	ret = xa_surface_dma(vsaa->xa_ctx, srf, data, pixmap->devKind,
			     (int) to_hw,
			     (struct xa_box *) REGION_RECTS(reg),
			     REGION_NUM_RECTS(reg));
	if (to_hw)
	    xa_context_flush(vsaa->xa_ctx);
	if (vpix->gmr)
	    vmwgfx_dmabuf_unmap(vpix->gmr);
	if (dx || dy)
	    REGION_TRANSLATE(pScreen, reg, -dx, -dy);
	if (ret)
	    goto out_err;
    }
    return TRUE;
  out_err:
    LogMessage(X_ERROR, "DMA %s surface failed.\n",
	       to_hw ? "to" : "from");
    return FALSE;
}


static Bool
vmwgfx_download_from_hw(struct saa_driver *driver, PixmapPtr pixmap,
			RegionPtr readback)
{
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(driver);
    struct saa_pixmap *spix = saa_get_saa_pixmap(pixmap);
    struct vmwgfx_saa_pixmap *vpix = to_vmwgfx_saa_pixmap(spix);

    RegionRec intersection;

    if (!vmwgfx_pixmap_present_readback(vsaa, pixmap, readback))
	return FALSE;

    if (!REGION_NOTEMPTY(vsaa->pScreen, &spix->dirty_hw))
	return TRUE;

    if (!vpix->hw)
	return TRUE;

    REGION_NULL(vsaa->pScreen, &intersection);
    REGION_INTERSECT(vsaa->pScreen, &intersection, readback,
		     &spix->dirty_hw);
    readback = &intersection;

    if (!vmwgfx_pixmap_create_sw(vsaa, pixmap))
	goto out_err;

    if (!vmwgfx_saa_dma(vsaa, pixmap, readback, FALSE, 0, 0, NULL))
	goto out_err;
    REGION_SUBTRACT(vsaa->pScreen, &spix->dirty_hw, &spix->dirty_hw, readback);
    REGION_UNINIT(vsaa->pScreen, &intersection);
    return TRUE;
 out_err:
    REGION_UNINIT(vsaa->pScreen, &intersection);
    return FALSE;
}


static Bool
vmwgfx_upload_to_hw(struct saa_driver *driver, PixmapPtr pixmap,
		    RegionPtr upload)
{
    return vmwgfx_saa_dma(to_vmwgfx_saa(driver), pixmap, upload, TRUE,
			  0, 0, NULL);
}

static void
vmwgfx_release_from_cpu(struct saa_driver *driver, PixmapPtr pixmap, saa_access_t access)
{
  //    LogMessage(X_INFO, "Release 0x%08lx access 0x%08x\n",
  //	       (unsigned long) pixmap, (unsigned) access);
}

static void *
vmwgfx_sync_for_cpu(struct saa_driver *driver, PixmapPtr pixmap, saa_access_t access)
{
    /*
     * Errors in this functions will turn up in subsequent map
     * calls.
     */

    (void) vmwgfx_pixmap_create_sw(to_vmwgfx_saa(driver), pixmap);

    return NULL;
}

static void *
vmwgfx_map(struct saa_driver *driver, PixmapPtr pixmap, saa_access_t access)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);

    if (vpix->malloc)
	return vpix->malloc;
    else if (vpix->gmr)
	return vmwgfx_dmabuf_map(vpix->gmr);
    else
	return NULL;
}

static void
vmwgfx_unmap(struct saa_driver *driver, PixmapPtr pixmap, saa_access_t access)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);

    if (vpix->gmr)
	return vmwgfx_dmabuf_unmap(vpix->gmr);

//    LogMessage(X_INFO, "Unmap 0x%08lx access 0x%08x\n",
    //       (unsigned long) pixmap, (unsigned) access);
    ;
}

static Bool
vmwgfx_create_pixmap(struct saa_driver *driver, struct saa_pixmap *spix,
		     int w, int h, int depth,
		     unsigned int usage_hint, int bpp, int *new_pitch)
{
    struct vmwgfx_saa_pixmap *vpix = to_vmwgfx_saa_pixmap(spix);

    *new_pitch = ((w * bpp + FB_MASK) >> FB_SHIFT) * sizeof(FbBits);

    WSBMINITLISTHEAD(&vpix->sync_x_head);
    WSBMINITLISTHEAD(&vpix->scanout_list);
    WSBMINITLISTHEAD(&vpix->pixmap_list);

    return TRUE;
}

Bool
vmwgfx_hw_kill(struct vmwgfx_saa *vsaa,
	       struct saa_pixmap *spix)
{
    struct vmwgfx_saa_pixmap *vpix = to_vmwgfx_saa_pixmap(spix);

    if (!vpix->hw)
	return TRUE;

    /*
     * Read back any dirty regions from hardware.
     */

    if (!vmwgfx_download_from_hw(&vsaa->driver, spix->pixmap,
				 &spix->dirty_hw))
	return FALSE;

    xa_surface_destroy(vpix->hw);
    vpix->hw = NULL;

    /*
     * Remove damage tracking if this is not a scanout pixmap.
     */

    if (WSBMLISTEMPTY(&vpix->scanout_list))
	vmwgfx_pixmap_remove_damage(spix->pixmap);

    return TRUE;
}

void
vmwgfx_flush_dri2(ScreenPtr pScreen)
{
    struct vmwgfx_saa *vsaa =
	to_vmwgfx_saa(saa_get_driver(pScreen));
    struct _WsbmListHead *list, *next;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);

    if (!pScrn->vtSema)
	return;

    WSBMLISTFOREACHSAFE(list, next, &vsaa->sync_x_list) {
	struct vmwgfx_saa_pixmap *vpix =
	    WSBMLISTENTRY(list, struct vmwgfx_saa_pixmap, sync_x_head);
	struct saa_pixmap *spix = &vpix->base;
	PixmapPtr pixmap = spix->pixmap;

	if (vmwgfx_upload_to_hw(&vsaa->driver, pixmap, &spix->dirty_shadow)) {
	    REGION_EMPTY(vsaa->pScreen, &spix->dirty_shadow);
	    WSBMLISTDELINIT(list);
	}
    }
}


static void
vmwgfx_destroy_pixmap(struct saa_driver *driver, PixmapPtr pixmap)
{
    ScreenPtr pScreen = to_vmwgfx_saa(driver)->pScreen;
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);
    (void) pScreen;

    vpix->backing = 0;
    vmwgfx_pixmap_free_storage(vpix);

    /*
     * Any damage we've registered has already been removed by the server
     * at this point. Any attempt to unregister / destroy it will result
     * in a double free.
     */

    vmwgfx_pixmap_remove_present(vpix);
    WSBMLISTDELINIT(&vpix->pixmap_list);
    WSBMLISTDELINIT(&vpix->sync_x_head);

    if (vpix->hw_is_dri2_fronts)
	LogMessage(X_ERROR, "Incorrect dri2 front count.\n");
}



/**
 *
 * Makes sure we have a surface with valid contents.
 */

static void
vmwgfx_copy_stride(uint8_t *dst, uint8_t *src, unsigned int dst_pitch,
		   unsigned int src_pitch, unsigned int dst_height,
		   unsigned int src_height)
{
    unsigned int i;
    unsigned int height = (dst_height < src_height) ? dst_height : src_height;
    unsigned int pitch = (dst_pitch < src_pitch) ? dst_pitch : src_pitch;

    for(i=0; i<height; ++i) {
	memcpy(dst, src, pitch);
	dst += dst_pitch;
	src += src_pitch;
    }
}


static Bool
vmwgfx_pix_resize(PixmapPtr pixmap, unsigned int old_pitch,
		  unsigned int old_height, unsigned int old_width)
{
    ScreenPtr pScreen = pixmap->drawable.pScreen;
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(saa_get_driver(pScreen));
    struct saa_pixmap *spix = saa_get_saa_pixmap(pixmap);
    struct vmwgfx_saa_pixmap *vpix = to_vmwgfx_saa_pixmap(spix);
    DrawablePtr draw = &pixmap->drawable;
    unsigned int size = pixmap->devKind * draw->height;
    BoxRec b_box;
    RegionRec b_reg;

    /*
     * Ignore copying errors. At worst they will show up as rendering
     * artefacts.
     */

    if (vpix->malloc) {

	void *new_malloc = malloc(size);
	if (!new_malloc)
	    return FALSE;

	vmwgfx_copy_stride(new_malloc, vpix->malloc, pixmap->devKind,
			   old_pitch, draw->height,
			   old_height);
	free(vpix->malloc);
	vpix->malloc = new_malloc;
    }

    if (vpix->gmr) {
	struct vmwgfx_dmabuf *gmr;
	void *new_addr;
	void *old_addr;

	gmr = vmwgfx_dmabuf_alloc(vsaa->drm_fd, size);
	if (!gmr)
	    return FALSE;

	new_addr = vmwgfx_dmabuf_map(gmr);
	old_addr = vmwgfx_dmabuf_map(vpix->gmr);

	if (new_addr && old_addr)
	    vmwgfx_copy_stride(new_addr, old_addr, pixmap->devKind,
			       old_pitch, draw->height,
			       old_height);
	else
	    LogMessage(X_ERROR, "Failed pixmap resize copy.\n");

	if (old_addr)
	    vmwgfx_dmabuf_unmap(vpix->gmr);
	if (new_addr)
	    vmwgfx_dmabuf_unmap(gmr);
	vmwgfx_dmabuf_destroy(vpix->gmr);
	vpix->gmr = gmr;
    }

    if (vpix->hw) {
      if (!vmwgfx_xa_surface_redefine(vpix, vpix->hw, draw->width,
				      draw->height, draw->depth, xa_type_argb,
				      xa_format_unknown, vpix->xa_flags, 1))
	    return FALSE;
    }

    b_box.x1 = 0;
    b_box.x2 = draw->width;
    b_box.y1 = 0;
    b_box.y2 = draw->height;

    REGION_INIT(pScreen, &b_reg, &b_box, 1);
    REGION_INTERSECT(pScreen, &spix->dirty_shadow, &spix->dirty_shadow,
		     &b_reg);
    REGION_INTERSECT(pScreen, &spix->dirty_hw, &spix->dirty_hw, &b_reg);
    if (vpix->dirty_present)
	REGION_INTERSECT(pScreen, vpix->dirty_present, vpix->dirty_present,
			 &b_reg);
    if (vpix->pending_update)
	REGION_INTERSECT(pScreen, vpix->pending_update, vpix->pending_update,
			 &b_reg);
    if (vpix->pending_present)
	REGION_INTERSECT(pScreen, vpix->pending_present,
			 vpix->pending_present, &b_reg);
    if (vpix->present_damage)
	REGION_INTERSECT(pScreen, vpix->present_damage, vpix->present_damage,
			 &b_reg);

    REGION_UNINIT(pScreen, &b_reg);

    return TRUE;
}


static Bool
vmwgfx_modify_pixmap_header (PixmapPtr pixmap, int w, int h, int depth,
			     int bpp, int devkind, void *pixdata)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);
    ScreenPtr pScreen = pixmap->drawable.pScreen;
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(saa_get_driver(pScreen));
    unsigned int old_height;
    unsigned int old_width;
    unsigned int old_pitch;

    if (!vpix) {
	LogMessage(X_ERROR, "Not an SAA pixmap.\n");
	return FALSE;
    }

    if (pixdata) {
	vpix->backing = 0;
	vmwgfx_pixmap_free_storage(vpix);
	return FALSE;
    }

    if (depth <= 0)
	depth = pixmap->drawable.depth;

    if (bpp <= 0)
	bpp = pixmap->drawable.bitsPerPixel;

    if (w <= 0)
	w = pixmap->drawable.width;

    if (h <= 0)
	h = pixmap->drawable.height;

    if (w <= 0 || h <= 0 || depth <= 0)
	return FALSE;

    old_height = pixmap->drawable.height;
    old_width = pixmap->drawable.width;
    old_pitch = pixmap->devKind;

    if (!miModifyPixmapHeader(pixmap, w, h, depth,
			      bpp, devkind, NULL))
	goto out_no_modify;

    if (!vpix->backing)
	vpix->backing = VMWGFX_PIX_MALLOC;

    vmwgfx_pix_resize(pixmap, old_pitch, old_height, old_width);
    vmwgfx_pixmap_free_storage(vpix);
    if (WSBMLISTEMPTY(&vpix->pixmap_list))
	WSBMLISTADDTAIL(&vpix->pixmap_list, &vsaa->pixmaps);

    return TRUE;

  out_no_modify:
    return FALSE;
}

static Bool
vmwgfx_present_prepare(struct vmwgfx_saa *vsaa,
		       struct vmwgfx_saa_pixmap *src_vpix,
		       struct vmwgfx_saa_pixmap *dst_vpix)
{
    ScreenPtr pScreen = vsaa->pScreen;
    unsigned int dummy;

    (void) pScreen;
    if (src_vpix == dst_vpix || !src_vpix->hw ||
	_xa_surface_handle(src_vpix->hw, &vsaa->src_handle, &dummy) != 0)
	return FALSE;

    REGION_NULL(pScreen, &vsaa->present_region);
    vsaa->diff_valid = FALSE;
    vsaa->dst_vpix = dst_vpix;
    vsaa->present_flush(pScreen);

    return TRUE;
}

/**
 * Determine whether we should try present copies on this pixmap.
 */

static Bool
vmwgfx_is_present_hw(PixmapPtr pixmap)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);
    return (vpix->dirty_present != NULL);
}

static void
vmwgfx_check_hw_contents(struct vmwgfx_saa *vsaa,
			 struct vmwgfx_saa_pixmap *vpix,
			 RegionPtr region,
			 Bool *has_dirty_hw,
			 Bool *has_valid_hw)
{
    RegionRec intersection;


    if (!vpix->hw) {
	*has_dirty_hw = FALSE;
	*has_valid_hw = FALSE;
	return;
    }

    if (!region) {
	*has_dirty_hw = REGION_NOTEMPTY(vsaa->pScreen,
					&vpix->base.dirty_hw);
	*has_valid_hw = !REGION_NOTEMPTY(vsaa->pScreen,
					 &vpix->base.dirty_shadow);
	return;
    }

    REGION_NULL(vsaa->pScreen, &intersection);
    REGION_INTERSECT(vsaa->pScreen, &intersection, &vpix->base.dirty_hw,
		     region);
    *has_dirty_hw = REGION_NOTEMPTY(vsaa->pScreen, &intersection);
    REGION_INTERSECT(vsaa->pScreen, &intersection, &vpix->base.dirty_shadow,
		     region);
    *has_valid_hw = !REGION_NOTEMPTY(vsaa->pScreen, &intersection);
    REGION_UNINIT(vsaa->pScreen, &intersection);
}

/**
 * vmwgfx_prefer_gmr: Prefer a dma buffer over malloced memory for software
 * rendered storage
 *
 * @vsaa: Pointer to a struct vmwgfx_saa accelerator.
 * @pixmap: Pointer to pixmap whose storage preference we want to alter.
 *
 * If possible, alter the storage or future storage of the software contents
 * of this pixmap to be in a DMA buffer rather than in malloced memory.
 * This function should be called when it's likely that frequent DMA operations
 * will occur between a surface and the memory holding the software
 * contents.
 */
static void
vmwgfx_prefer_gmr(struct vmwgfx_saa *vsaa, PixmapPtr pixmap)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);

    if (vsaa->can_optimize_dma) {
	if (vpix->malloc) {
	    (void) vmwgfx_pixmap_create_gmr(vsaa, pixmap);
	} else if (vpix->backing & VMWGFX_PIX_MALLOC) {
	    vpix->backing |= VMWGFX_PIX_GMR;
	    vpix->backing &= ~VMWGFX_PIX_MALLOC;
	}
    }
}

Bool
vmwgfx_create_hw(struct vmwgfx_saa *vsaa,
		 PixmapPtr pixmap)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);
    struct xa_surface *hw;
    uint32_t new_flags;

    if (!vsaa->xat)
	return FALSE;

    if (vpix->hw)
	return TRUE;

    new_flags = (vpix->xa_flags & ~vpix->staging_remove_flags) |
	vpix->staging_add_flags | XA_FLAG_SHARED;

    hw = xa_surface_create(vsaa->xat,
			   pixmap->drawable.width,
			   pixmap->drawable.height,
			   0,
			   xa_type_other,
			   vpix->staging_format,
			   new_flags);
    if (hw == NULL)
	return FALSE;

    vpix->xa_flags = new_flags;
    vpix->hw = hw;

    if (!vmwgfx_pixmap_add_damage(pixmap))
	goto out_no_damage;

    vpix->backing |= VMWGFX_PIX_SURFACE;
    vmwgfx_pixmap_free_storage(vpix);

    /*
     * If there is a HW surface, make sure that the shadow is
     * (or will be) a GMR, provided we can do fast DMAs from / to it.
     */
    vmwgfx_prefer_gmr(vsaa, pixmap);

    return TRUE;

out_no_damage:
    vpix->hw = NULL;
    xa_surface_destroy(hw);
    return FALSE;
}


Bool
vmwgfx_hw_validate(PixmapPtr pixmap, RegionPtr region)
{
    struct vmwgfx_saa *vsaa =
	to_vmwgfx_saa(saa_get_driver(pixmap->drawable.pScreen));
    struct saa_pixmap *spix = saa_get_saa_pixmap(pixmap);
    struct vmwgfx_saa_pixmap *vpix = to_vmwgfx_saa_pixmap(spix);
    RegionRec intersection;

    if (!vmwgfx_pixmap_present_readback(vsaa, pixmap, region))
	return FALSE;

    REGION_NULL(vsaa->pScreen, &intersection);
    REGION_COPY(vsaa->pScreen, &intersection, &spix->dirty_shadow);

    if (vpix->dirty_present)
	REGION_UNION(vsaa->pScreen, &intersection, vpix->dirty_present,
		     &spix->dirty_shadow);

    if (spix->damage && REGION_NOTEMPTY(vsaa->pScreen, &intersection)) {
	RegionPtr upload = &intersection;

	/*
	 * Check whether we need to upload from GMR.
	 */

	if (region) {
	    REGION_INTERSECT(vsaa->pScreen, &intersection, region,
			     &intersection);
	    upload = &intersection;
	}

	if (REGION_NOTEMPTY(vsaa->pScreen, upload)) {
	    Bool ret = vmwgfx_upload_to_hw(&vsaa->driver, pixmap, upload);
	    if (ret) {
		REGION_SUBTRACT(vsaa->pScreen, &spix->dirty_shadow,
				&spix->dirty_shadow, upload);
		if (vpix->dirty_present)
		    REGION_SUBTRACT(vsaa->pScreen, vpix->dirty_present,
				    vpix->dirty_present, upload);
	    } else {
		REGION_UNINIT(vsaa->pScreen, &intersection);
		return FALSE;
	    }
	}
    }
    REGION_UNINIT(vsaa->pScreen, &intersection);
    return TRUE;
}

static Bool
vmwgfx_copy_prepare(struct saa_driver *driver,
		    PixmapPtr src_pixmap,
		    PixmapPtr dst_pixmap,
		    int dx,
		    int dy,
		    int alu,
		    RegionPtr src_reg,
		    uint32_t plane_mask)
{
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(driver);
    struct vmwgfx_saa_pixmap *src_vpix;
    struct vmwgfx_saa_pixmap *dst_vpix;
    Bool has_dirty_hw;
    Bool has_valid_hw;

    if (!vsaa->xat || !SAA_PM_IS_SOLID(&dst_pixmap->drawable, plane_mask) ||
	alu != GXcopy || !vsaa->is_master)
	return FALSE;

    src_vpix = vmwgfx_saa_pixmap(src_pixmap);
    dst_vpix = vmwgfx_saa_pixmap(dst_pixmap);

    vmwgfx_check_hw_contents(vsaa, src_vpix, src_reg,
			     &has_dirty_hw, &has_valid_hw);

    if (vmwgfx_is_present_hw(dst_pixmap) &&
	src_vpix->backing & VMWGFX_PIX_SURFACE) {

	if (!has_dirty_hw && !has_valid_hw)
	    return FALSE;

	if (!vmwgfx_hw_accel_validate(src_pixmap, 0, 0, 0, src_reg))
	    return FALSE;
	if (vmwgfx_present_prepare(vsaa, src_vpix, dst_vpix)) {
	    vsaa->present_copy = TRUE;
	    return TRUE;
	}
	return FALSE;
    }

    vsaa->present_copy = FALSE;
    if (src_vpix != dst_vpix) {

	/*
	 * Use hardware acceleration either if source is partially only
	 * in hardware, or if source is entirely in hardware and destination
	 * has a hardware surface.
	 */

	if (!has_dirty_hw && !(has_valid_hw && (dst_vpix->hw != NULL)))
	    return FALSE;

	/*
	 * Determine surface formats.
	 */

	if (src_vpix->base.src_format == 0) {
	    if (!vmwgfx_hw_accel_stage(src_pixmap, 0, XA_FLAG_RENDER_TARGET, 0))
		return FALSE;
	} else {
	    if (PICT_FORMAT_TYPE(src_vpix->base.src_format) != PICT_TYPE_ARGB ||
		!vmwgfx_hw_composite_src_stage(src_pixmap, src_vpix->base.src_format))
		return FALSE;
	}

	if (dst_vpix->base.dst_format == 0) {
	    if (!vmwgfx_hw_accel_stage(dst_pixmap, 0, XA_FLAG_RENDER_TARGET, 0))
		return FALSE;
	} else {
	    if (PICT_FORMAT_TYPE(dst_vpix->base.dst_format) != PICT_TYPE_ARGB ||
		!vmwgfx_hw_composite_dst_stage(dst_pixmap, dst_vpix->base.dst_format))
		return FALSE;
	}

	/*
	 * Create hardware surfaces.
	 */

	if (!vmwgfx_hw_commit(src_pixmap))
	    return FALSE;
	if (!vmwgfx_hw_commit(dst_pixmap))
	    return FALSE;

	/*
	 * Migrate data.
	 */

	if (!vmwgfx_hw_validate(src_pixmap, src_reg)) {
	    xa_copy_done(vsaa->xa_ctx);
	    xa_context_flush(vsaa->xa_ctx);
	    return FALSE;
	}

	/*
	 * Setup copy state.
	 */

	if (xa_copy_prepare(vsaa->xa_ctx, dst_vpix->hw, src_vpix->hw) !=
	    XA_ERR_NONE)
	    return FALSE;

	return TRUE;
    }

    return FALSE;
}


static void
vmwgfx_present_done(struct vmwgfx_saa *vsaa)
{
    ScreenPtr pScreen = vsaa->pScreen;
    struct vmwgfx_saa_pixmap *dst_vpix = vsaa->dst_vpix;

    (void) pScreen;
    if (!vsaa->diff_valid)
	return;

    (void) vmwgfx_present(vsaa->drm_fd, dst_vpix->fb_id,
			  vsaa->xdiff, vsaa->ydiff,
			  &vsaa->present_region, vsaa->src_handle);

    REGION_TRANSLATE(pScreen, &vsaa->present_region, vsaa->xdiff, vsaa->ydiff);
    REGION_UNION(pScreen, dst_vpix->present_damage, dst_vpix->present_damage,
		 &vsaa->present_region);
    vsaa->diff_valid = FALSE;
    REGION_UNINIT(pScreen, &vsaa->present_region);
}

static void
vmwgfx_present_copy(struct vmwgfx_saa *vsaa,
		    int src_x,
		    int src_y,
		    int dst_x,
		    int dst_y,
		    int w,
		    int h)
{
    int xdiff = dst_x - src_x;
    int ydiff = dst_y - src_y;
    BoxRec box;
    RegionRec reg;

    if (vsaa->diff_valid && ((xdiff != vsaa->xdiff) || (ydiff != vsaa->ydiff)))
	(void) vmwgfx_present_done(vsaa);

    if (!vsaa->diff_valid) {
	vsaa->xdiff = xdiff;
	vsaa->ydiff = ydiff;
	vsaa->diff_valid = TRUE;
    }

    box.x1 = src_x;
    box.x2 = src_x + w;
    box.y1 = src_y;
    box.y2 = src_y + h;

    REGION_INIT(pScreen, &reg, &box, 1);
    REGION_UNION(pScreen, &vsaa->present_region, &vsaa->present_region, &reg);
    REGION_UNINIT(pScreen, &reg);
}

static void
vmwgfx_copy(struct saa_driver *driver,
	    int src_x,
	    int src_y,
	    int dst_x,
	    int dst_y,
	    int w,
	    int h)
{
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(driver);

    if (vsaa->present_copy) {
	vmwgfx_present_copy(vsaa, src_x, src_y, dst_x, dst_y, w, h);
	return;
    }
    xa_copy(vsaa->xa_ctx, dst_x, dst_y, src_x, src_y, w, h);
}

static void
vmwgfx_copy_done(struct saa_driver *driver)
{
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(driver);

    if (vsaa->present_copy) {
	vmwgfx_present_done(vsaa);
	return;
    }
    xa_copy_done(vsaa->xa_ctx);
    xa_context_flush(vsaa->xa_ctx);
}

static Bool
vmwgfx_composite_prepare(struct saa_driver *driver, CARD8 op,
			 PicturePtr src_pict, PicturePtr mask_pict,
			 PicturePtr dst_pict,
			 PixmapPtr src_pix, PixmapPtr mask_pix,
			 PixmapPtr dst_pix,
			 RegionPtr src_region,
			 RegionPtr mask_region,
			 RegionPtr dst_region)
{
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(driver);
    struct vmwgfx_saa_pixmap *src_vpix;
    struct vmwgfx_saa_pixmap *dst_vpix;
    struct vmwgfx_saa_pixmap *mask_vpix;
    Bool tmp_valid_hw;
    Bool dirty_hw;
    Bool valid_hw;
    RegionRec empty;
    struct xa_composite *xa_comp;

    if (!vsaa->is_master)
	return FALSE;

    REGION_NULL(pScreen, &empty);

    /*
     * First we define our migration policy. We accelerate only if there
     * are dirty hw regions to be read or if all source data is
     * available in hw, and the destination has a hardware surface.
     */
    dst_vpix = vmwgfx_saa_pixmap(dst_pix);
    valid_hw = (dst_vpix->hw != NULL);
    if (saa_op_reads_destination(op)) {
	vmwgfx_check_hw_contents(vsaa, dst_vpix, dst_region,
				 &dirty_hw, &tmp_valid_hw);
	valid_hw = (valid_hw && tmp_valid_hw);
    } else {
	dirty_hw = FALSE;
	dst_region = &empty;
    }

    if (src_pix && !dirty_hw) {
	src_vpix = vmwgfx_saa_pixmap(src_pix);
	vmwgfx_check_hw_contents(vsaa, src_vpix, src_region,
				 &dirty_hw, &tmp_valid_hw);
	valid_hw = (valid_hw && tmp_valid_hw);
    }

    if (mask_pict && mask_pix && !dirty_hw) {
	mask_vpix = vmwgfx_saa_pixmap(mask_pix);
	vmwgfx_check_hw_contents(vsaa, mask_vpix, mask_region,
				 &dirty_hw, &tmp_valid_hw);
	valid_hw = (valid_hw && tmp_valid_hw);
    }

    /*
     * In rendercheck mode we try to accelerate all supported
     * composite operations.
     */

    if (!valid_hw && !dirty_hw && !vsaa->rendercheck)
	goto out_err;

    /*
     * Then, setup most of the XA composite state (except hardware surfaces)
     * and check whether XA can accelerate.
     */

    if (!mask_pix)
	mask_pict = NULL;
    xa_comp = vmwgfx_xa_setup_comp(vsaa->vcomp, op,
				   src_pict, mask_pict, dst_pict);
    if (!xa_comp)
	goto out_err;

    if (xa_composite_check_accelerated(xa_comp) != XA_ERR_NONE)
	goto out_err;

    /*
     * Check that we can create the needed hardware surfaces.
     */
    if (src_pix && !vmwgfx_hw_composite_src_stage(src_pix, src_pict->format))
	goto out_err;
    if (mask_pict && mask_pix &&
	!vmwgfx_hw_composite_src_stage(mask_pix, mask_pict->format))
	goto out_err;
    if (!vmwgfx_hw_composite_dst_stage(dst_pix, dst_pict->format))
	goto out_err;

    /*
     * Seems OK. Commit the changes, creating hardware surfaces.
     */
    if (src_pix && !vmwgfx_hw_commit(src_pix))
	goto out_err;
    if (mask_pict && mask_pix && !vmwgfx_hw_commit(mask_pix))
	goto out_err;
    if (!vmwgfx_hw_commit(dst_pix))
	goto out_err;

    /*
     * Update the XA state with our hardware surfaces and
     * surface formats
     */
    if (!vmwgfx_xa_update_comp(xa_comp, src_pix, mask_pix, dst_pix))
	goto out_err;

    /*
     * Migrate data to surfaces.
     */
    if (src_pix && src_region && !vmwgfx_hw_validate(src_pix, NULL))
	goto out_err;
    if (mask_pict && mask_pix && mask_region &&
	!vmwgfx_hw_validate(mask_pix, NULL))
	goto out_err;
    if (dst_region && !vmwgfx_hw_validate(dst_pix, NULL))
	goto out_err;


    /*
     * Bind the XA state. This must be done after data migration, since
     * migration may change the hardware surfaces.
     */
    if (xa_composite_prepare(vsaa->xa_ctx, xa_comp))
	goto out_err;

    return TRUE;

  out_err:
    return FALSE;
}

static void
vmwgfx_composite(struct saa_driver *driver,
		 int src_x, int src_y, int mask_x, int mask_y,
		 int dst_x, int dst_y,
		 int width, int height)
{
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(driver);

    xa_composite_rect(vsaa->xa_ctx, src_x, src_y, mask_x, mask_y,
		      dst_x, dst_y, width, height);
}

static void
vmwgfx_composite_done(struct saa_driver *driver)
{
   struct vmwgfx_saa *vsaa = to_vmwgfx_saa(driver);

   xa_composite_done(vsaa->xa_ctx);
   xa_context_flush(vsaa->xa_ctx);
}

static void
vmwgfx_takedown(struct saa_driver *driver)
{
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(driver);

    if (vsaa->vcomp)
	vmwgfx_free_composite(vsaa->vcomp);
    free(vsaa);
}

/*
 * This function call originates from the damage layer (outside SAA)
 * to indicate that an operation is complete, and that damage is being
 * processed.
 */
static void
vmwgfx_operation_complete(struct saa_driver *driver,
			  PixmapPtr pixmap)
{
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(driver);
    struct saa_pixmap *spix = saa_get_saa_pixmap(pixmap);
    struct vmwgfx_saa_pixmap *vpix = to_vmwgfx_saa_pixmap(spix);
    ScrnInfoPtr pScrn = xf86ScreenToScrn(vsaa->pScreen);

    /*
     * Make dri2 drawables up to date, or add them to the flush list
     * executed at glxWaitX(). Currently glxWaitX() is broken, so
     * we flush immediately, unless we're VT-switched away, in which
     * case a flush would deadlock in the kernel.
     *
     * For pixmaps for which vpix->hw_is_hosted is true, we can explicitly
     * inform the compositor when contents has changed, so for those pixmaps
     * we defer the upload until the compositor is informed, by putting
     * them on the sync_x_list. Note that hw_is_dri2_fronts take precedence.
     */
    if (vpix->hw && (vpix->hw_is_dri2_fronts || vpix->hw_is_hosted)) {
	if (pScrn->vtSema && vpix->hw_is_dri2_fronts &&
	    vmwgfx_upload_to_hw(driver, pixmap, &spix->dirty_shadow)) {

	    REGION_EMPTY(vsaa->pScreen, &spix->dirty_shadow);
	    return;
	}

	if (WSBMLISTEMPTY(&vpix->sync_x_head))
	    WSBMLISTADDTAIL(&vpix->sync_x_head, &vsaa->sync_x_list);
    }
}

/*
 * This function is called by SAA to indicate that SAA has
 * dirtied a region of a pixmap, either as hw (accelerated) or as
 * !hw (not accelerated).
 */
static Bool
vmwgfx_dirty(struct saa_driver *driver, PixmapPtr pixmap,
	     Bool hw, RegionPtr damage)
{
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(driver);
    struct saa_pixmap *spix = saa_get_saa_pixmap(pixmap);
    struct vmwgfx_saa_pixmap *vpix = to_vmwgfx_saa_pixmap(spix);

    /*
     * Return if this is not a scanout pixmap.
     */
    if (WSBMLISTEMPTY(&vpix->scanout_list))
	return TRUE;

#if 0
    /*
     * This code can be enabled to immediately upload scanout sw
     * contents to the hw surface. Otherwise this is done
     * just before we call the kms update function for the hw
     * surface.
     */
    if (vsaa->only_hw_presents) {
	if (!hw && !vmwgfx_upload_to_hw(&vsaa->driver, pixmap, damage))
	    return FALSE;

	REGION_SUBTRACT(&vsaa->pScreen, &spix->dirty_shadow,
			&spix->dirty_shadow, damage);
	hw = TRUE;
    }
#endif

    /*
     * Is the new scanout damage hw or sw?
     */
    if (hw) {
	/*
	 * Dump pending present into present tracking region.
	 */
	if (vpix->dirty_present &&
	    REGION_NOTEMPTY(vsaa->pScreen, vpix->present_damage)) {
	    REGION_UNION(vsaa->pScreen, vpix->dirty_present,
			 vpix->dirty_present, damage);
	    REGION_EMPTY(vsaa->pScreen, vpix->present_damage);
	} else {
	    if (REGION_NOTEMPTY(vsaa->pScreen, vpix->pending_update)) {
		RegionRec reg;

		REGION_NULL(vsaa->pScreen, &reg);
		REGION_INTERSECT(vsaa->pScreen, &reg, vpix->pending_update,
				 damage);
		if (REGION_NOTEMPTY(vsaa->pScreen, &reg))
		    vsaa->present_flush(vsaa->pScreen);
		REGION_UNINIT(pScreen, &reg);
	    }
	    REGION_UNION(vsaa->pScreen, vpix->pending_present,
			 vpix->pending_present, damage);
	    if (vpix->dirty_present)
		REGION_SUBTRACT(vsaa->pScreen, vpix->dirty_present,
				vpix->dirty_present, damage);
	}
    } else {
	    if (REGION_NOTEMPTY(vsaa->pScreen, vpix->pending_present)) {
		RegionRec reg;

		REGION_NULL(vsaa->pScreen, &reg);
		REGION_INTERSECT(vsaa->pScreen, &reg, vpix->pending_present,
				 damage);
		if (REGION_NOTEMPTY(vsaa->pScreen, &reg))
		    vsaa->present_flush(vsaa->pScreen);
		REGION_UNINIT(pScreen, &reg);
	    }
	    REGION_UNION(vsaa->pScreen, vpix->pending_update,
			 vpix->pending_update, damage);
	    if (vpix->dirty_present)
		REGION_SUBTRACT(vsaa->pScreen, vpix->dirty_present,
				vpix->dirty_present, damage);
    }

    return TRUE;
}


static const struct saa_driver vmwgfx_saa_driver = {
    .saa_major = SAA_VERSION_MAJOR,
    .saa_minor = SAA_VERSION_MINOR,
    .pixmap_size = sizeof(struct vmwgfx_saa_pixmap),
    .damage = vmwgfx_dirty,
    .operation_complete = vmwgfx_operation_complete,
    .download_from_hw = vmwgfx_download_from_hw,
    .release_from_cpu = vmwgfx_release_from_cpu,
    .sync_for_cpu = vmwgfx_sync_for_cpu,
    .map = vmwgfx_map,
    .unmap = vmwgfx_unmap,
    .create_pixmap = vmwgfx_create_pixmap,
    .destroy_pixmap = vmwgfx_destroy_pixmap,
    .modify_pixmap_header = vmwgfx_modify_pixmap_header,
    .copy_prepare = vmwgfx_copy_prepare,
    .copy = vmwgfx_copy,
    .copy_done = vmwgfx_copy_done,
    .composite_prepare = vmwgfx_composite_prepare,
    .composite = vmwgfx_composite,
    .composite_done = vmwgfx_composite_done,
    .takedown = vmwgfx_takedown,
};


Bool
vmwgfx_saa_init(ScreenPtr pScreen, int drm_fd, struct xa_tracker *xat,
		void (*present_flush)(ScreenPtr pScreen),
		Bool direct_presents,
		Bool only_hw_presents,
		Bool rendercheck)
{
    struct vmwgfx_saa *vsaa;

    vsaa = calloc(1, sizeof(*vsaa));
    if (!vsaa)
	return FALSE;

    if (xat == NULL) {
	direct_presents = FALSE;
	only_hw_presents = FALSE;
    }

    vsaa->pScreen = pScreen;
    vsaa->xat = xat;
    if (xat)
	vsaa->xa_ctx = xa_context_default(xat);
    vsaa->drm_fd = drm_fd;
    vsaa->present_flush = present_flush;
    vsaa->can_optimize_dma = TRUE;
    vsaa->use_present_opt = direct_presents;
    vsaa->only_hw_presents = only_hw_presents;
    vsaa->rendercheck = rendercheck;
    vsaa->is_master = TRUE;
    vsaa->known_prime_format = FALSE;
    WSBMINITLISTHEAD(&vsaa->sync_x_list);
    WSBMINITLISTHEAD(&vsaa->pixmaps);

    vsaa->driver = vmwgfx_saa_driver;
    vsaa->vcomp = vmwgfx_alloc_composite();

    if (!vsaa->vcomp)
	vsaa->driver.composite_prepare = NULL;

    if (!saa_driver_init(pScreen, &vsaa->driver))
	goto out_no_saa;

    return TRUE;
  out_no_saa:
    free(vsaa);
    return FALSE;
}

/*
 * *************************************************************************
 * Scanout functions.
 * These do not strictly belong here, but we choose to hide the scanout
 * pixmap private data in the saa pixmaps. Might want to revisit this.
 */

/*
 * Make sure we flush / update this scanout on next update run.
 */

void
vmwgfx_scanout_refresh(PixmapPtr pixmap)
{
    ScreenPtr pScreen = pixmap->drawable.pScreen;
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);
    BoxRec box;

    (void) pScreen;
    box.x1 = 0;
    box.y1 = 0;
    box.x2 = pixmap->drawable.width;
    box.y2 = pixmap->drawable.height;

    REGION_RESET(vsaa->pScreen, vpix->pending_present, &box);
    if (vpix->dirty_present)
	REGION_SUBTRACT(vsaa->pScreen, vpix->pending_present,
			vpix->pending_present, vpix->dirty_present);
    REGION_SUBTRACT(vsaa->pScreen, vpix->pending_present,
		    vpix->pending_present, &vpix->base.dirty_shadow);
    REGION_COPY(vsaa->pScreen, vpix->pending_update,
		&vpix->base.dirty_shadow);
}

/*
 * Take a "scanout reference" on a pixmap. If this is the first scanout
 * reference, allocate resources needed for scanout, like proper
 * damage tracking and kms fbs.
 */

uint32_t
vmwgfx_scanout_ref(struct vmwgfx_screen_entry  *entry)
{
    PixmapPtr pixmap = entry->pixmap;
    struct vmwgfx_saa *vsaa =
	to_vmwgfx_saa(saa_get_driver(pixmap->drawable.pScreen));
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);

    if (WSBMLISTEMPTY(&vpix->scanout_list)) {
	uint32_t handle, dummy;
	unsigned int depth;

	if (vsaa->only_hw_presents) {
	    /*
	     * The KMS fb will be a HW surface. Create it, add damage
	     * and get the handle.
	     */
	    if (!vmwgfx_hw_accel_validate(pixmap, 0, XA_FLAG_SCANOUT |
					  XA_FLAG_RENDER_TARGET, 0, NULL))
		goto out_err;
	    if (_xa_surface_handle(vpix->hw, &handle, &dummy) != 0)
		goto out_err;
	    depth = xa_format_depth(xa_surface_format(vpix->hw));

	} else {
	    /*
	     * The KMS fb will be a Guest Memory Region. Create it,
	     * add damage and get the handle.
	     */
	    if (!vmwgfx_pixmap_create_gmr(vsaa, pixmap))
		goto out_err;

	    handle = vpix->gmr->handle;
	    depth = pixmap->drawable.depth;

	}

        if (!vmwgfx_pixmap_add_present(pixmap, vsaa->use_present_opt))
	    goto out_no_present;

	if (drmModeAddFB(vsaa->drm_fd,
			 pixmap->drawable.width,
			 pixmap->drawable.height,
			 depth,
			 pixmap->drawable.bitsPerPixel,
			 pixmap->devKind,
			 handle,
			 &vpix->fb_id) != 0)
	    goto out_no_fb;;
    }
    pixmap->refcnt += 1;
    WSBMLISTADDTAIL(&entry->scanout_head, &vpix->scanout_list);
    return vpix->fb_id;

  out_no_fb:
    vmwgfx_pixmap_remove_present(vpix);
  out_no_present:
    vmwgfx_pixmap_remove_damage(pixmap);
  out_err:
    vpix->fb_id = -1;
    return -1;
}

/*
 * Free a "scanout reference" on a pixmap. If this was the last scanout
 * reference, free pixmap resources needed for scanout, like
 * damage tracking and kms fbs.
 */
void
vmwgfx_scanout_unref(struct vmwgfx_screen_entry *entry)
{
    struct vmwgfx_saa *vsaa;
    struct vmwgfx_saa_pixmap *vpix;
    PixmapPtr pixmap = entry->pixmap;

    if (!pixmap)
	return;

    vsaa = to_vmwgfx_saa(saa_get_driver(pixmap->drawable.pScreen));
    vpix = vmwgfx_saa_pixmap(pixmap);
    WSBMLISTDELINIT(&entry->scanout_head);

    if (WSBMLISTEMPTY(&vpix->scanout_list)) {
	REGION_EMPTY(vsaa->pScreen, vpix->pending_update);
	drmModeRmFB(vsaa->drm_fd, vpix->fb_id);
	vpix->fb_id = -1;
	vmwgfx_pixmap_present_readback(vsaa, pixmap, NULL);
	vmwgfx_pixmap_remove_present(vpix);
	vmwgfx_pixmap_remove_damage(pixmap);
    }

    entry->pixmap = NULL;
    pixmap->drawable.pScreen->DestroyPixmap(pixmap);
}

void
vmwgfx_saa_set_master(ScreenPtr pScreen)
{
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(saa_get_driver(pScreen));

    vsaa->is_master = TRUE;
    vmwgfx_flush_dri2(pScreen);
}

void
vmwgfx_saa_drop_master(ScreenPtr pScreen)
{
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(saa_get_driver(pScreen));
    struct _WsbmListHead *list;
    struct vmwgfx_saa_pixmap *vpix;
    struct saa_pixmap *spix;

    WSBMLISTFOREACH(list, &vsaa->pixmaps) {
	vpix = WSBMLISTENTRY(list, struct vmwgfx_saa_pixmap, pixmap_list);
	spix = &vpix->base;

	if (!vpix->hw)
	    continue;

	(void) vmwgfx_download_from_hw(&vsaa->driver, spix->pixmap,
				       &spix->dirty_hw);
	REGION_EMPTY(draw->pScreen, &spix->dirty_hw);
    }

    vsaa->is_master = FALSE;
}

/*
 * *************************************************************************
 * Helpers for hosted.
 */

#if (XA_TRACKER_VERSION_MAJOR >= 2) && defined(HAVE_LIBDRM_2_4_38)

/**
 * vmwgfx_saa_copy_to_surface - Copy Drawable contents to an external surface.
 *
 * @pDraw: Pointer to source drawable.
 * @surface_fd: Prime file descriptor of external surface to copy to.
 * @dst_box: BoxRec describing the destination bounding box.
 * @region: Region of drawable to copy. Note: The code assumes that the
 * region is relative to the drawable origin, not the underlying pixmap
 * origin.
 *
 * Copies the contents (both software- and accelerated contents) to an
 * external surface.
 */
Bool
vmwgfx_saa_copy_to_surface(DrawablePtr pDraw, uint32_t surface_fd,
			   const BoxRec *dst_box, RegionPtr region)
{
    ScreenPtr pScreen = pDraw->pScreen;
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(saa_get_driver(pScreen));
    PixmapPtr src;
    struct saa_pixmap *spix;
    struct vmwgfx_saa_pixmap *vpix;
    const BoxRec *box;
    int n;
    int sx, sy, dx, dy;
    struct xa_surface *dst;
    uint32_t handle;
    Bool ret = TRUE;
    RegionRec intersection;
    RegionPtr copy_region = region;

    if (vmwgfx_prime_fd_to_handle(vsaa->drm_fd, surface_fd, &handle) < 0)
	return FALSE;

    dst = xa_surface_from_handle(vsaa->xat, pDraw->width, pDraw->height,
				 pDraw->depth, xa_type_argb,
				 xa_format_unknown,
				 XA_FLAG_SHARED | XA_FLAG_RENDER_TARGET,
				 handle,
				 (pDraw->width * pDraw->bitsPerPixel + 7) / 8);

    if (!dst) {
	ret = FALSE;
	goto out_no_surface;
    }

    /*
     * Assume damage region is relative to the source window.
     */
    src = saa_get_pixmap(pDraw, &sx, &sy);
    sx += pDraw->x;
    sy += pDraw->y;
    if (sx || sy)
	REGION_TRANSLATE(pScreen, region, sx, sy);

    dx = dst_box->x1 - sx;
    dy = dst_box->y1 - sy;

    spix = saa_get_saa_pixmap(src);
    vpix = to_vmwgfx_saa_pixmap(spix);

    /*
     * Make sure software contents of the source pixmap is henceforth put
     * in a GMR to avoid the extra copy in the xa DMA.
     */
    vmwgfx_prefer_gmr(vsaa, src);

    /*
     * Determine the intersection between software contents and region to copy.
     */

    if (vsaa->known_prime_format) {
	REGION_NULL(pScreen, &intersection);
	if (!vpix->hw)
	    REGION_COPY(pScreen, &intersection, region);
	else if (spix->damage && REGION_NOTEMPTY(pScreen, &spix->dirty_shadow))
	    REGION_INTERSECT(pScreen, &intersection, region, &spix->dirty_shadow);

	/*
	 * DMA software contents directly into the destination. Then subtract
	 * the region we've DMA'd from the region to copy.
	 */
	if (REGION_NOTEMPTY(pScreen, &intersection)) {
	    if (vmwgfx_saa_dma(vsaa, src, &intersection, TRUE, dx, dy, dst)) {
		REGION_SUBTRACT(pScreen, &intersection, region, &intersection);
		copy_region = &intersection;
	    }
	}
    }

    if (!REGION_NOTEMPTY(pScreen, copy_region))
	goto out_no_copy;

    /*
     * Copy Hardware contents to the destination
     */
    box = REGION_RECTS(copy_region);
    n = REGION_NUM_RECTS(copy_region);

    if (!vmwgfx_hw_accel_validate(src, 0, 0, 0, copy_region)) {
	ret = FALSE;
	goto out_no_copy;
    }

    if (xa_copy_prepare(vsaa->xa_ctx, dst, vpix->hw) != XA_ERR_NONE) {
	ret = FALSE;
	goto out_no_copy;
    }

    while(n--) {
	xa_copy(vsaa->xa_ctx, box->x1 + dx, box->y1 + dy, box->x1, box->y1,
		box->x2 - box->x1, box->y2 - box->y1);
	box++;
    }

    xa_copy_done(vsaa->xa_ctx);
    xa_context_flush(vsaa->xa_ctx);

  out_no_copy:
    if (vsaa->known_prime_format)
	REGION_UNINIT(pScreen, &intersection);
    if (sx || sy)
	REGION_TRANSLATE(pScreen, region, -sx, -sy);
    xa_surface_unref(dst);
  out_no_surface:
    vmwgfx_prime_release_handle(vsaa->drm_fd, handle);

    return ret;
}
#endif
