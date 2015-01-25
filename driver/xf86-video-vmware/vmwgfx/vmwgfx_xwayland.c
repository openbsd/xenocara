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
 * Author: Jakob Bornecrantz <jakob@vmware.com>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "vmwgfx_hosted_priv.h"

#ifdef XORG_WAYLAND

#include "vmwgfx_hosted.h"
#include "vmwgfx_saa.h"
#include <xf86Priv.h>
#include <xwayland.h>

struct vmwgfx_hosted {
    struct xwl_screen *xwl;
    ScrnInfoPtr pScrn;
    ScreenPtr pScreen;
};

static int
vmwgfx_create_window_buffer(struct xwl_window *xwl_window,
			    PixmapPtr pixmap)
{
    struct vmwgfx_saa_pixmap *vpix = vmwgfx_saa_pixmap(pixmap);
    uint32_t name, pitch;

    if (!vmwgfx_hw_dri2_validate(pixmap, 0))
	return BadDrawable;

    /*
     * Pixmaps with hw_is_hosted == TRUE are put on a flush list when
     * they've seen software rendering. When vmwgfx_flush_dri2 is called
     * on these pixmaps, software contents are flushed to the hardware
     * surface.
     */
    vpix->hw_is_hosted = TRUE;
    if (_xa_surface_handle(vpix->hw, &name, &pitch) != XA_ERR_NONE)
	return BadDrawable;

    return xwl_create_window_buffer_drm(xwl_window, pixmap, name);
}

static struct xwl_driver vmwgfx_xwl_driver = {
    .version = 2,
    .use_drm = 1,
    .create_window_buffer = vmwgfx_create_window_buffer
};

static struct vmwgfx_hosted *
vmwgfx_xwl_create(ScrnInfoPtr pScrn)
{
    struct vmwgfx_hosted *hosted;

    hosted = calloc(1, sizeof(*hosted));
    if (!hosted)
	return NULL;

    hosted->xwl = xwl_screen_create();
    if (!hosted->xwl) {
	free(hosted);
	return NULL;
    }

    hosted->pScrn = pScrn;
    return hosted;
}

static void
vmwgfx_xwl_destroy(struct vmwgfx_hosted *hosted)
{
    xwl_screen_destroy(hosted->xwl);
    free(hosted);
}

static Bool
vmwgfx_xwl_pre_init(struct vmwgfx_hosted *hosted, int flags)
{
    return xwl_screen_pre_init(hosted->pScrn, hosted->xwl, 0,
			       &vmwgfx_xwl_driver);
}

static int
vmwgfx_xwl_drm_fd(struct vmwgfx_hosted *hosted, const struct pci_device *pci)
{
    return xwl_screen_get_drm_fd(hosted->xwl);
}

static Bool
vmwgfx_xwl_screen_init(struct vmwgfx_hosted *hosted, ScreenPtr pScreen)
{
    if (xwl_screen_init(hosted->xwl, pScreen))
	return FALSE;

    hosted->pScreen = pScreen;

    return TRUE;
}

static void
vmwgfx_xwl_screen_close(struct vmwgfx_hosted *hosted)
{
    if (hosted->pScreen)
	xwl_screen_close(hosted->xwl);

    hosted->pScreen = NULL;
}

static void
vmwgfx_xwl_post_damage(struct vmwgfx_hosted *hosted)
{
    vmwgfx_flush_dri2(hosted->pScreen);
    xwl_screen_post_damage(hosted->xwl);
}

static int
vmwgfx_xwl_dri_auth(struct vmwgfx_hosted *hosted, ClientPtr client,
		    uint32_t magic)
{
    return xwl_drm_authenticate(client, hosted->xwl, magic);
}

static const struct vmwgfx_hosted_driver vmwgfx_hosted_xwl_driver = {
    .create = vmwgfx_xwl_create,
    .destroy = vmwgfx_xwl_destroy,
    .drm_fd = vmwgfx_xwl_drm_fd,
    .pre_init = vmwgfx_xwl_pre_init,
    .screen_init = vmwgfx_xwl_screen_init,
    .screen_close = vmwgfx_xwl_screen_close,
    .post_damage = vmwgfx_xwl_post_damage,
    .dri_auth = vmwgfx_xwl_dri_auth
};

const struct vmwgfx_hosted_driver *
vmwgfx_xwl_detect(void)
{
    return (xorgWayland) ? &vmwgfx_hosted_xwl_driver : NULL;
}

void
vmwgfx_xwl_modify_flags(uint32_t *flags)
{
    if (xorgWayland)
	*flags |= HW_SKIP_CONSOLE | HW_WAYLAND;
}

#else

const struct vmwgfx_hosted_driver *
vmwgfx_xwl_detect(void)
{
    return NULL;
}

void
vmwgfx_xwl_modify_flags(uint32_t *flags)
{
}
#endif
