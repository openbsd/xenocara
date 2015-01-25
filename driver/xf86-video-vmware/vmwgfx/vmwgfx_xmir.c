/*
 * Copyright 2013 VMWare, Inc.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "vmwgfx_hosted_priv.h"
#include <xa_tracker.h>

#if XMIR && (XA_TRACKER_VERSION_MAJOR >= 2) && defined(HAVE_LIBDRM_2_4_38)

#include "vmwgfx_hosted.h"
#include "vmwgfx_saa.h"
#include <xf86Crtc.h>
#include <xf86Priv.h>
#include <xmir.h>

struct vmwgfx_hosted {
    xmir_screen *xmir;
    ScrnInfoPtr pScrn;
    ScreenPtr pScreen;
};

static void
vmwgfx_xmir_copy_to_mir(xmir_window *xmir_win, RegionPtr region);

static xmir_driver vmwgfx_xmir_driver = {
    XMIR_DRIVER_VERSION,
    vmwgfx_xmir_copy_to_mir
};

static struct vmwgfx_hosted *
vmwgfx_xmir_create(ScrnInfoPtr pScrn)
{
    struct vmwgfx_hosted *hosted;

    hosted = calloc(1, sizeof(*hosted));
    if (!hosted)
	return NULL;

    hosted->xmir = xmir_screen_create(pScrn);
    if (!hosted->xmir) {
	free(hosted);
	return NULL;
    }

    hosted->pScrn = pScrn;
    return hosted;
}

static void
vmwgfx_xmir_destroy(struct vmwgfx_hosted *hosted)
{
    xmir_screen_destroy(hosted->xmir);
    free(hosted);
}

static Bool
vmwgfx_xmir_pre_init(struct vmwgfx_hosted *hosted, int flags)
{
    return xmir_screen_pre_init(hosted->pScrn, hosted->xmir,
				&vmwgfx_xmir_driver);
}

static int
vmwgfx_xmir_drm_fd(struct vmwgfx_hosted *hosted, const struct pci_device *pci)
{
    char bus_id[20];

    snprintf(bus_id, sizeof(bus_id), "pci:%04x:%02x:%02x.%d",
	     pci->domain, pci->bus, pci->dev, pci->func);
    return xmir_get_drm_fd(bus_id);
}

static Bool
vmwgfx_xmir_screen_init(struct vmwgfx_hosted *hosted, ScreenPtr pScreen)
{
    if (!xmir_screen_init(pScreen, hosted->xmir))
	return FALSE;

    hosted->pScreen = pScreen;
    if (!xf86SetDesiredModes(hosted->pScrn)) {
	xmir_screen_close(hosted->pScreen, hosted->xmir);
	hosted->pScreen = NULL;
	return FALSE;
    }

    return TRUE;
}

static void
vmwgfx_xmir_screen_close(struct vmwgfx_hosted *hosted)
{
    if (hosted->pScreen)
	xmir_screen_close(hosted->pScreen, hosted->xmir);

    hosted->pScreen = NULL;
}

static void
vmwgfx_xmir_post_damage(struct vmwgfx_hosted *hosted)
{
    xmir_screen_for_each_damaged_window(hosted->xmir, vmwgfx_xmir_copy_to_mir);
}

static int
vmwgfx_xmir_dri_auth(struct vmwgfx_hosted *hosted, ClientPtr client,
		     uint32_t magic)
{
    return xmir_auth_drm_magic(hosted->xmir, magic);
}

static void
vmwgfx_xmir_copy_to_mir(xmir_window *xmir_win, RegionPtr region)
{
    DrawablePtr pDraw = (DrawablePtr) xmir_window_to_windowptr(xmir_win);
    const BoxRec *dst_box = xmir_window_get_drawable_region(xmir_win);

    if (vmwgfx_saa_copy_to_surface(pDraw, xmir_window_get_fd(xmir_win),
				   dst_box, region))
	xmir_submit_rendering_for_window(xmir_win, region);
}

static const struct vmwgfx_hosted_driver vmwgfx_hosted_xmir_driver = {
    .create = vmwgfx_xmir_create,
    .destroy = vmwgfx_xmir_destroy,
    .drm_fd = vmwgfx_xmir_drm_fd,
    .pre_init = vmwgfx_xmir_pre_init,
    .screen_init = vmwgfx_xmir_screen_init,
    .screen_close = vmwgfx_xmir_screen_close,
    .post_damage = vmwgfx_xmir_post_damage,
    .dri_auth = vmwgfx_xmir_dri_auth
};

const struct vmwgfx_hosted_driver *
vmwgfx_xmir_detect(void)
{
    return (xorgMir) ? &vmwgfx_hosted_xmir_driver : NULL;
}

void vmwgfx_xmir_modify_flags(uint32_t *flags)
{
    if (xorgMir)
	*flags |= HW_SKIP_CONSOLE;
}

#else

const struct vmwgfx_hosted_driver *
vmwgfx_xmir_detect(void)
{
    return NULL;
}

void
vmwgfx_xmir_modify_flags(uint32_t *flags)
{
}
#endif
