/*
 * Copyright 2008 Tungsten Graphics, Inc., Cedar Park, Texas.
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
 *
 * Author: Alan Hourihane <alanh@tungstengraphics.com>
 * Author: Jakob Bornecrantz <wallbraker@gmail.com>
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 */

#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "xorg-server.h"
#include <xf86.h>
#include <xf86i2c.h>
#include <xf86Crtc.h>
#include <cursorstr.h>
#include "vmwgfx_driver.h"
#include "xf86Modes.h"
#include "vmwgfx_saa.h"

#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

struct crtc_private
{
    drmModeCrtcPtr drm_crtc;

    /* hwcursor */
    struct vmwgfx_dmabuf *cursor_bo;
    uint32_t scanout_id;
    unsigned cursor_handle;

    /* Scanout info for pixmaps */
    struct vmwgfx_screen_entry entry;
};

static void
crtc_dpms(xf86CrtcPtr crtc, int mode)
{
    struct crtc_private *crtcp = crtc->driver_private;
    /* ScrnInfoPtr pScrn = crtc->scrn; */

    switch (mode) {
    case DPMSModeOn:
    case DPMSModeStandby:
    case DPMSModeSuspend:
	break;
    case DPMSModeOff:

      /*
       * The xf86 modesetting code uses DPMS off to turn off
       * crtcs that are not enabled. However, the DPMS code does the same.
       * We assume, that if we get this call with the crtc not enabled,
       * it's a permanent switch off which will only be reversed by a
       * major modeset.
       *
       * If it's a DPMS switch off, (crtc->enabled == TRUE),
       * the crtc may be turned on again by
       * another dpms call, so don't release the scanout pixmap ref.
       */
	if (!crtc->enabled && crtcp->entry.pixmap) {
	    vmwgfx_scanout_unref(&crtcp->entry);
	}
	break;
    }
}

/*
 * Disable outputs and crtcs and drop the scanout reference from
 * scanout pixmaps. This will essentialy free all kms fb allocations.
 */

void
vmwgfx_disable_scanout(ScrnInfoPtr pScrn)
{
    int i;
    Bool save_enabled;
    xf86CrtcPtr crtc;
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);

    xf86DPMSSet(pScrn, DPMSModeOff, 0);
    for (i=0; i < config->num_crtc; ++i) {
	crtc = config->crtc[i];
	save_enabled = crtc->enabled;
	crtc->enabled = FALSE;
	crtc_dpms(crtc, DPMSModeOff);
	crtc->enabled = save_enabled;
    }
    xf86RotateFreeShadow(pScrn);
}

static Bool
crtc_set_mode_major(xf86CrtcPtr crtc, DisplayModePtr mode,
		    Rotation rotation, int x, int y)
{
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    modesettingPtr ms = modesettingPTR(crtc->scrn);
    ScreenPtr pScreen = crtc->scrn->pScreen;
    xf86OutputPtr output = NULL;
    struct crtc_private *crtcp = crtc->driver_private;
    drmModeCrtcPtr drm_crtc = crtcp->drm_crtc;
    drmModeModeInfo drm_mode;
    int i, ret;
    unsigned int connector_id;
    PixmapPtr pixmap;

    for (i = 0; i < config->num_output; output = NULL, i++) {
	output = config->output[i];

	if (output->crtc == crtc)
	    break;
    }

    if (!output) {
	LogMessage(X_ERROR, "No output for this crtc.\n");
	return FALSE;
    }

    connector_id = xorg_output_get_id(output);

    drm_mode.clock = mode->Clock;
    drm_mode.hdisplay = mode->HDisplay;
    drm_mode.hsync_start = mode->HSyncStart;
    drm_mode.hsync_end = mode->HSyncEnd;
    drm_mode.htotal = mode->HTotal;
    drm_mode.vdisplay = mode->VDisplay;
    drm_mode.vsync_start = mode->VSyncStart;
    drm_mode.vsync_end = mode->VSyncEnd;
    drm_mode.vtotal = mode->VTotal;
    drm_mode.flags = mode->Flags;
    drm_mode.hskew = mode->HSkew;
    drm_mode.vscan = mode->VScan;
    drm_mode.vrefresh = mode->VRefresh;
    if (!mode->name)
	xf86SetModeDefaultName(mode);
    strncpy(drm_mode.name, mode->name, DRM_DISPLAY_MODE_LEN - 1);
    drm_mode.name[DRM_DISPLAY_MODE_LEN - 1] = '\0';

    /*
     * Check if we need to scanout from something else than the root
     * pixmap. In that case, xf86CrtcRotate will take care of allocating
     * new opaque scanout buffer data "crtc->rotatedData".
     * However, it will not wrap
     * that data into pixmaps until the first rotated damage composite.
     * In out case, the buffer data is actually already a pixmap.
     */

    if (!xf86CrtcRotate(crtc))
	return FALSE;

    if (crtc->transform_in_use && crtc->rotatedData) {
	x = 0;
	y = 0;
	pixmap = (PixmapPtr) crtc->rotatedData;
    } else
	pixmap = pScreen->GetScreenPixmap(pScreen);

    if (crtcp->entry.pixmap != pixmap) {
	if (crtcp->entry.pixmap)
	    vmwgfx_scanout_unref(&crtcp->entry);

	crtcp->entry.pixmap = pixmap;
	crtcp->scanout_id = vmwgfx_scanout_ref(&crtcp->entry);
	if (crtcp->scanout_id == -1) {
	    crtcp->entry.pixmap = NULL;
	    LogMessage(X_ERROR, "Failed to convert pixmap to scanout.\n");
	    return FALSE;
	}
    }
    ret = drmModeSetCrtc(ms->fd, drm_crtc->crtc_id, crtcp->scanout_id, x, y,
			 &connector_id, 1, &drm_mode);
    if (ret)
	return FALSE;

    vmwgfx_scanout_refresh(pixmap);

    /* Only set gamma when needed, to avoid unneeded delays. */
#if defined(XF86_CRTC_VERSION) && XF86_CRTC_VERSION >= 3
    if (!crtc->active && crtc->version >= 3)
	crtc->funcs->gamma_set(crtc, crtc->gamma_red, crtc->gamma_green,
			       crtc->gamma_blue, crtc->gamma_size);
    crtc->active = TRUE;
#endif

    /*
     * Strictly, this needs to be done only once per configuration change,
     * not once per crtc, but there's no better place to put this. Since
     * Intel wrote the crtc code, let's do what the xf86-video-intel driver
     * does.
     */
    if (pScreen)
	xf86_reload_cursors(pScreen);

    return TRUE;
}

static void
crtc_gamma_set(xf86CrtcPtr crtc, CARD16 * red, CARD16 * green, CARD16 * blue,
	       int size)
{
    modesettingPtr ms = modesettingPTR(crtc->scrn);
    struct crtc_private *crtcp = crtc->driver_private;

    drmModeCrtcSetGamma(ms->fd, crtcp->drm_crtc->crtc_id, size, red, green, blue);
}

static void *
crtc_shadow_allocate(xf86CrtcPtr crtc, int width, int height)
{
    ScreenPtr pScreen = crtc->scrn->pScreen;
    PixmapPtr rootpix = pScreen->GetScreenPixmap(pScreen);

    /*
     * Use the same depth as for the root pixmap.
     * The associated kms fb will be created on demand once this pixmap
     * is used as scanout by a crtc.
     */

    return pScreen->CreatePixmap(pScreen, width, height,
				 rootpix->drawable.depth, 0);
}

static PixmapPtr
crtc_shadow_create(xf86CrtcPtr crtc, void *data, int width, int height)
{
    return (PixmapPtr) data;
}

static void
crtc_shadow_destroy(xf86CrtcPtr crtc, PixmapPtr rotate_pixmap, void *data)
{
    ScreenPtr pScreen;

    if (rotate_pixmap == NULL)
        return;

    pScreen = rotate_pixmap->drawable.pScreen;
    pScreen->DestroyPixmap(rotate_pixmap);
}


/*
 * Cursor functions
 */

static void
crtc_set_cursor_colors(xf86CrtcPtr crtc, int bg, int fg)
{
    /* XXX: See if this one is needed, as we only support ARGB cursors */
}

static void
crtc_set_cursor_position(xf86CrtcPtr crtc, int x, int y)
{
    modesettingPtr ms = modesettingPTR(crtc->scrn);
    struct crtc_private *crtcp = crtc->driver_private;

    /* Seems like newer X servers try to move cursors without images */
    if (!crtcp->cursor_bo)
	return;

    drmModeMoveCursor(ms->fd, crtcp->drm_crtc->crtc_id, x, y);
}

static void
crtc_load_cursor_argb_kms(xf86CrtcPtr crtc, CARD32 * image)
{
    modesettingPtr ms = modesettingPTR(crtc->scrn);
    struct crtc_private *crtcp = crtc->driver_private;
    unsigned char *ptr;
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    CursorPtr c = config->cursor;

    if (vmwgfx_cursor_bypass(ms->fd, c->bits->xhot, c->bits->yhot) != 0) {
	xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR,
		   "Failed to set VMWare cursor bypass.\n");
    }

    if (!crtcp->cursor_bo) {
	size_t size = 64*64*4;
        crtcp->cursor_bo = vmwgfx_dmabuf_alloc(ms->fd, size);
	if (!crtcp->cursor_bo) {
	    xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR,
		       "Failed to create a dmabuf for cursor.\n");
	    return;
	}
	crtcp->cursor_handle = crtcp->cursor_bo->handle;
    }

    ptr = vmwgfx_dmabuf_map(crtcp->cursor_bo);
    if (ptr) {
	memcpy(ptr, image, 64*64*4);
	vmwgfx_dmabuf_unmap(crtcp->cursor_bo);
    } else {
	xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR,
		   "Failed to map cursor dmabuf.\n");
    }

    if (crtc->cursor_shown)
	drmModeSetCursor(ms->fd, crtcp->drm_crtc->crtc_id,
			 crtcp->cursor_handle, 64, 64);

    return;
}

static void
crtc_load_cursor_argb(xf86CrtcPtr crtc, CARD32 * image)
{
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    modesettingPtr ms = modesettingPTR(crtc->scrn);

    /* Older X servers have cursor reference counting bugs leading to use of
     * freed memory and consequently random crashes. Should be fixed as of
     * xserver 1.8, but this workaround shouldn't hurt anyway.
     */
    if (config->cursor)
       config->cursor->refcnt++;

    if (ms->cursor)
       FreeCursor(ms->cursor, None);

    ms->cursor = config->cursor;
    crtc_load_cursor_argb_kms(crtc, image);
}

static void
crtc_show_cursor(xf86CrtcPtr crtc)
{
    modesettingPtr ms = modesettingPTR(crtc->scrn);
    struct crtc_private *crtcp = crtc->driver_private;

    if (crtcp->cursor_bo)
	drmModeSetCursor(ms->fd, crtcp->drm_crtc->crtc_id,
			 crtcp->cursor_handle, 64, 64);
}

static void
crtc_hide_cursor(xf86CrtcPtr crtc)
{
    modesettingPtr ms = modesettingPTR(crtc->scrn);
    struct crtc_private *crtcp = crtc->driver_private;

    drmModeSetCursor(ms->fd, crtcp->drm_crtc->crtc_id, 0, 0, 0);
}

/**
 * Called at vt leave
 */
void
xorg_crtc_cursor_destroy(xf86CrtcPtr crtc)
{
    struct crtc_private *crtcp = crtc->driver_private;

    if (crtcp->cursor_bo) {
	vmwgfx_dmabuf_destroy(crtcp->cursor_bo);
	crtcp->cursor_bo = NULL;
    }
}

/*
 * Misc functions
 */

static void
crtc_destroy(xf86CrtcPtr crtc)
{
    struct crtc_private *crtcp = crtc->driver_private;

    if (!WSBMLISTEMPTY(&crtcp->entry.scanout_head))
	vmwgfx_scanout_unref(&crtcp->entry);

    xorg_crtc_cursor_destroy(crtc);

    drmModeFreeCrtc(crtcp->drm_crtc);

    free(crtcp);
    crtc->driver_private = NULL;
}

static const xf86CrtcFuncsRec crtc_funcs = {
    .dpms = crtc_dpms,
    .set_mode_major = crtc_set_mode_major,

    .set_cursor_colors = crtc_set_cursor_colors,
    .set_cursor_position = crtc_set_cursor_position,
    .show_cursor = crtc_show_cursor,
    .hide_cursor = crtc_hide_cursor,
    .load_cursor_argb = crtc_load_cursor_argb,

    .shadow_create = crtc_shadow_create,
    .shadow_allocate = crtc_shadow_allocate,
    .shadow_destroy = crtc_shadow_destroy,

    .gamma_set = crtc_gamma_set,
    .destroy = crtc_destroy,
};

void
xorg_crtc_init(ScrnInfoPtr pScrn)
{
    modesettingPtr ms = modesettingPTR(pScrn);
    xf86CrtcPtr crtc;
    drmModeResPtr res;
    drmModeCrtcPtr drm_crtc = NULL;
    struct crtc_private *crtcp;
    int c;

    res = drmModeGetResources(ms->fd);
    if (res == 0) {
	ErrorF("Failed drmModeGetResources %d\n", errno);
	return;
    }

    for (c = 0; c < res->count_crtcs; c++) {
	drm_crtc = drmModeGetCrtc(ms->fd, res->crtcs[c]);

	if (!drm_crtc)
	    continue;

	crtc = xf86CrtcCreate(pScrn, &crtc_funcs);
	if (crtc == NULL)
	    goto out;

	crtcp = calloc(1, sizeof(struct crtc_private));
	if (!crtcp) {
	    xf86CrtcDestroy(crtc);
	    goto out;
	}

	crtcp->drm_crtc = drm_crtc;
	crtcp->entry.pixmap = NULL;
	WSBMINITLISTHEAD(&crtcp->entry.scanout_head);

	crtc->driver_private = crtcp;
    }

  out:
    drmModeFreeResources(res);
}

PixmapPtr
crtc_get_scanout(xf86CrtcPtr crtc)
{
    struct crtc_private *crtcp = crtc->driver_private;
    return crtcp->entry.pixmap;
}

/* vim: set sw=4 ts=8 sts=4: */
