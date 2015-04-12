/*
 * Copyright © 2013-2014 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xorg-server.h"
#include "xf86.h"
#include "fb.h"

#include "intel.h"
#include "dri3.h"

static int
intel_dri3_open(ScreenPtr screen,
                RRProviderPtr provider,
                int *out)
{
	int fd;

	fd = intel_get_client_fd(xf86ScreenToScrn(screen));
	if (fd < 0)
		return -fd;

	*out = fd;
	return Success;
}

static PixmapPtr intel_dri3_pixmap_from_fd(ScreenPtr screen,
					   int fd,
					   CARD16 width,
					   CARD16 height,
					   CARD16 stride,
					   CARD8 depth,
					   CARD8 bpp)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct intel_pixmap *priv;
	PixmapPtr pixmap;
	dri_bo *bo;

	if (depth < 8)
		return NULL;

	switch (bpp) {
	case 8:
	case 16:
	case 32:
		break;
	default:
		return NULL;
	}

	pixmap = fbCreatePixmap(screen, 0, 0, depth, 0);
	if (!pixmap)
		return NULL;

	if (!screen->ModifyPixmapHeader(pixmap, width, height, 0, 0, stride, NULL))
		goto free_pixmap;

	bo = drm_intel_bo_gem_create_from_prime(intel->bufmgr,
						fd, (uint32_t)height * stride);
	if (bo == NULL)
		goto free_pixmap;

	intel_set_pixmap_bo(pixmap, bo);
	dri_bo_unreference(bo);

	priv = intel_get_pixmap_private(pixmap);
	if (priv == NULL)
		goto free_pixmap;

	priv->pinned |= PIN_DRI3;

	return pixmap;

free_pixmap:
	fbDestroyPixmap(pixmap);
	return NULL;
}

static int intel_dri3_fd_from_pixmap(ScreenPtr screen,
				     PixmapPtr pixmap,
				     CARD16 *stride,
				     CARD32 *size)
{
	struct intel_pixmap *priv;
	int fd;

	priv = intel_get_pixmap_private(pixmap);
	if (!priv)
		return -1;

	if (priv->stride > UINT16_MAX)
		return -1;

	if (drm_intel_bo_gem_export_to_prime(priv->bo, &fd) < 0)
		return -1;

	priv->pinned |= PIN_DRI3;

	*stride = priv->stride;
	*size = priv->bo->size;
	return fd;
}

static dri3_screen_info_rec intel_dri3_screen_info = {
        .version = DRI3_SCREEN_INFO_VERSION,

        .open = intel_dri3_open,
        .pixmap_from_fd = intel_dri3_pixmap_from_fd,
        .fd_from_pixmap = intel_dri3_fd_from_pixmap
};

Bool
intel_dri3_screen_init(ScreenPtr screen)
{
        return dri3_screen_init(screen, &intel_dri3_screen_info);
}
