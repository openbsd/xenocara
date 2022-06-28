/*
 * Copyright 2017 VMWare, Inc.
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
 *
 */
#ifdef _HAVE_CONFIG_H_
#include "config.h"
#endif

#include <xorg-server.h>

#ifdef DRI3
#include "vmwgfx_driver.h"
#if (XA_TRACKER_VERSION_MAJOR == VMW_XA_VERSION_MAJOR_DRI3 &&   \
     XA_TRACKER_VERSION_MINOR >= VMW_XA_VERSION_MINOR_DRI3)

#include "vmwgfx_driver.h"
#include "vmwgfx_saa_priv.h"
#include <dri3.h>
#include <misyncshm.h>
#include <xf86drm.h>
#include <unistd.h>


/**
 * \brief DRI3 fd_from_pixmap callback.
 *
 */
static int
vmwgfx_dri3_fd_from_pixmap(ScreenPtr screen, PixmapPtr pixmap,
                           CARD16 *stride, CARD32 *size)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);
    uint32_t handle;
    unsigned int byte_stride;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(screen);

    if (!vmwgfx_hw_dri2_validate(pixmap, 0)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "DRI3 pixmap export failed to create HW surface.\n");
        return -1;
    }

    if (xa_surface_handle(vpix->hw, xa_handle_type_fd, &handle,
                          &byte_stride)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "DRI3 pixmap export failed to create handle.\n");
        return -1;
    }

    *stride = byte_stride;
    *size = byte_stride * pixmap->drawable.height;

    /*
     * hw_is_dri2_fronts will make sure any software rendering to
     * this pixmap is immediately flushed to the underlying surface.
     * Strictly, we could wait for glxWaitX to do that, but alas,
     * even the dri3 glxWaitX appears as broken as the dri2 version.
     * If we, however, wanted to do that, we'd hook up a shm fence
     * trigger callback. (Like glamor does).
     */
    vpix->hw_is_dri2_fronts = 1;

    return handle;
}

/**
 * \brief DRI3 pixmap_from_fd callback.
 *
 */
static PixmapPtr
vmwgfx_dri3_pixmap_from_fd(ScreenPtr screen, int fd,
                           CARD16 width, CARD16 height, CARD16 stride,
                           CARD8 depth, CARD8 bpp)
{
    struct vmwgfx_saa *vsaa = to_vmwgfx_saa(saa_get_driver(screen));
    struct xa_surface *srf;
    struct vmwgfx_saa_pixmap *vpix;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(screen);
    PixmapPtr pixmap;

    if (width == 0 || height == 0 ||
        depth < 15 || bpp != BitsPerPixel(depth) || stride < width * bpp / 8)
        return NULL;

    pixmap = screen->CreatePixmap(screen, width, height, depth, 0);
    if (!pixmap) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "DRI3 pixmap creation failed.\n");
        return NULL;
    }

    vpix = vmwgfx_saa_pixmap(pixmap);

    if (!vmwgfx_hw_dri2_stage(pixmap, depth)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "DRI3 pixmap creation bad format.\n");
        goto out_bad_format;
    }

    srf = xa_surface_from_handle2(vsaa->xat, width, height, depth,
                                  xa_type_other,
                                  vpix->staging_format,
                                  vpix->staging_add_flags,
                                  xa_handle_type_fd,
                                  fd, stride);
    if (!srf) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "DRI3 pixmap creation surface sharing failed.\n");
        goto out_bad_format;
    }

    vpix->xa_flags = vpix->staging_add_flags;
    vpix->hw = srf;
    if (!vmwgfx_create_hw(vsaa, pixmap, TRUE)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "DRI3 pixmap creation failed SAA enabling.\n");
        goto out_no_damage;
    }

    vpix->hw_is_dri2_fronts = 1;
    return pixmap;

  out_no_damage:
    xa_surface_unref(srf);
  out_bad_format:
    screen->DestroyPixmap(pixmap);

    return NULL;
}

/**
 * \brief Open a render node.
 *
 * \param screen[IN]  Pointer to the screen
 * \return  A valid file descriptor or -1 on failure.
 */
static int
vmwgfx_dri3_open_render(ScreenPtr screen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(screen);
    modesettingPtr ms = modesettingPTR(pScrn);
    char bus_id[64];
    int fd;

    snprintf(bus_id, sizeof(bus_id), "PCI:%d:%d:%d",
             ((ms->PciInfo->domain << 8) | ms->PciInfo->bus),
             ms->PciInfo->dev, ms->PciInfo->func);

    /* Render nodes can't be opened by busid yet.. */
    fd = drmOpenWithType("vmwgfx", bus_id, DRM_NODE_RENDER);
    if (fd < 0)
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "DRI3 client open busid \"%s\" failed.\n", bus_id);

    return fd;
}

/**
 * \brief DRI3 open_client callback.
 *
 */
static int
vmwgfx_dri3_open_client(ClientPtr client, ScreenPtr screen,
                        RRProviderPtr provider, int *pfd)
{
    *pfd = vmwgfx_dri3_open_render(screen);

    return (*pfd >= 0) ? Success : BadAlloc;
}

/**
 * \brief Verify that surface sharing between render client and X server
 * works.
 *
 * \param screen[IN,OUT]  A pointer to the current screen.
 * \return TRUE if successful, FALSE otherwise.
 *
 * Opens a render client, creates a surface and tries to share that surface
 * with the X server. There is a vmwgfx kernel driver bug that, combined
 * with a pre-guest-backed-surface svga mesa driver bug,
 * prevents this sharing to happen and thus breaks dri3.
 *
 * Also, we need to make sure that we can share an XRGB surface as an
 * ARGB surface since DRI3 does not appear to be as strict about internal
 * surface formats as DRI2.
 */
static Bool
vmwgfx_dri3_verify_sharing(ScreenPtr screen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(screen);
    modesettingPtr ms = modesettingPTR(pScrn);
    int fd = vmwgfx_dri3_open_render(screen);
    struct xa_tracker *xat;
    struct xa_surface *srf1;
    unsigned int stride;
    uint32_t handle;
    Bool ret = FALSE;

    if (fd < 0)
        return FALSE;

    xat = xa_tracker_create(fd);
    if (!xat)
        goto out_no_xa;

    /* Here we're the render client (xat) */
    srf1 = xa_surface_create(xat, 16, 16, 32, xa_type_argb,
                             xa_format_unknown,
                             XA_FLAG_RENDER_TARGET | XA_FLAG_SHARED);
    if (!srf1)
        goto out_no_surface;

    if (xa_surface_handle(srf1, xa_handle_type_fd, &handle, &stride) !=
        XA_ERR_NONE)
        goto out_no_handle;

    xa_surface_unref(srf1);

    /* Now we're the X server (ms->xat) */
    srf1 = xa_surface_from_handle2(ms->xat, 16, 16, 24, xa_type_argb,
                                   xa_format_unknown,
                                   XA_FLAG_RENDER_TARGET | XA_FLAG_SHARED,
                                   xa_handle_type_fd, handle, stride);
    if (!srf1)
        goto out_no_surface;

    ret = TRUE;
    close(handle);

  out_no_handle:
    xa_surface_unref(srf1);
  out_no_surface:
    xa_tracker_destroy(xat);
  out_no_xa:
    close(fd);

    return ret;
}

static dri3_screen_info_rec vmwgfx_dri3_info = {
    .version = 1,
    .open = NULL,
    .pixmap_from_fd = vmwgfx_dri3_pixmap_from_fd,
    .fd_from_pixmap = vmwgfx_dri3_fd_from_pixmap,
    .open_client = vmwgfx_dri3_open_client,
};


/**
 * \brief Initialize dri3.
 *
 * \param screen[IN,OUT]  A pointer to the current screen.
 * \return TRUE if successful, FALSE otherwise.
 */
Bool
vmwgfx_dri3_init(ScreenPtr screen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(screen);

    if (!vmwgfx_dri3_verify_sharing(screen)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Failed to verify XA surface sharing for DRI3.\n");
        return FALSE;
    }

    if (!miSyncShmScreenInit(screen)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Failed to initialize xshm sync for DRI3.\n");
        return FALSE;
    }

    if (!dri3_screen_init(screen, &vmwgfx_dri3_info)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to initialize DRI3.\n");
        return FALSE;
    }

    return TRUE;
}

#else /* XA INCLUDES SUFFICIENT */
Bool
vmwgfx_dri3_init(ScreenPtr screen)
{
    return FALSE;
}

#endif /* !XA INCLUDES SUFFICIENT */
#endif /* DRI3 */
