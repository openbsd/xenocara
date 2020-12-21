/*
 * Copyright © 2013-2014 Intel Corporation
 * Copyright © 2015 Advanced Micro Devices, Inc.
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

#include "radeon.h"

#ifdef HAVE_DRI3_H

#include "radeon_bo_gem.h"
#include "radeon_glamor.h"
#include "dri3.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <gbm.h>
#include <errno.h>
#include <libgen.h>

#ifdef X_PRIVSEP
extern int priv_open_device(const char *);
#else
#define priv_open_device(n)	open(n,O_RDWR|O_CLOEXEC);
#endif

static int open_master_node(ScreenPtr screen, int *out)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
	RADEONInfoPtr info = RADEONPTR(scrn);
	drm_magic_t magic;
	int fd;

	fd = priv_open_device(info->dri2.device_name);
	if (fd < 0)
		return BadAlloc;

	/* Before FD passing in the X protocol with DRI3 (and increased
	 * security of rendering with per-process address spaces on the
	 * GPU), the kernel had to come up with a way to have the server
	 * decide which clients got to access the GPU, which was done by
	 * each client getting a unique (magic) number from the kernel,
	 * passing it to the server, and the server then telling the
	 * kernel which clients were authenticated for using the device.
	 *
	 * Now that we have FD passing, the server can just set up the
	 * authentication on its own and hand the prepared FD off to the
	 * client.
	 */
	if (drmGetMagic(fd, &magic) < 0) {
		if (errno == EACCES) {
			/* Assume that we're on a render node, and the fd is
			 * already as authenticated as it should be.
			 */
			*out = fd;
			return Success;
		} else {
			close(fd);
			return BadMatch;
		}
	}

	if (drmAuthMagic(pRADEONEnt->fd, magic) < 0) {
		close(fd);
		return BadMatch;
	}

	*out = fd;
	return Success;
}

static int open_render_node(ScreenPtr screen, int *out)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
	int fd;

	fd = priv_open_device(pRADEONEnt->render_node);
	if (fd < 0)
		return BadAlloc;

	*out = fd;
	return Success;
}

static int
radeon_dri3_open(ScreenPtr screen, RRProviderPtr provider, int *out)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
	int ret = BadAlloc;

	if (pRADEONEnt->render_node)
		ret = open_render_node(screen, out);

	if (ret != Success)
		ret = open_master_node(screen, out);

	return ret;
}

#if DRI3_SCREEN_INFO_VERSION >= 1 && XORG_VERSION_CURRENT <= XORG_VERSION_NUMERIC(1,18,99,1,0)

static int
radeon_dri3_open_client(ClientPtr client, ScreenPtr screen,
			RRProviderPtr provider, int *out)
{
	const char *cmdname = GetClientCmdName(client);
	Bool is_ssh = FALSE;

	/* If the executable name is "ssh", assume that this client connection
	 * is forwarded from another host via SSH
	 */
	if (cmdname) {
		char *cmd = strdup(cmdname);

		/* Cut off any colon and whatever comes after it, see
		 * https://lists.freedesktop.org/archives/xorg-devel/2015-December/048164.html
		 */
		cmd = strtok(cmd, ":");

		is_ssh = strcmp(basename(cmd), "ssh") == 0;
		free(cmd);
	}

	if (!is_ssh)
		return radeon_dri3_open(screen, provider, out);

	return BadAccess;
}

#endif /* DRI3_SCREEN_INFO_VERSION >= 1 && XORG_VERSION_CURRENT <= XORG_VERSION_NUMERIC(1,18,99,1,0) */

static PixmapPtr radeon_dri3_pixmap_from_fd(ScreenPtr screen,
					    int fd,
					    CARD16 width,
					    CARD16 height,
					    CARD16 stride,
					    CARD8 depth,
					    CARD8 bpp)
{
	PixmapPtr pixmap;

#ifdef USE_GLAMOR
	/* Avoid generating a GEM flink name if possible */
	if (RADEONPTR(xf86ScreenToScrn(screen))->use_glamor) {
		pixmap = glamor_pixmap_from_fd(screen, fd, width, height,
					       stride, depth, bpp);
		if (pixmap) {
			struct radeon_pixmap *priv =
				calloc(1, sizeof(struct radeon_pixmap));

			if (priv) {
				radeon_set_pixmap_private(pixmap, priv);
				pixmap->usage_hint |= RADEON_CREATE_PIXMAP_DRI2;
				return pixmap;
			}

			screen->DestroyPixmap(pixmap);
			return NULL;
		}
	}
#endif

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

	pixmap = screen->CreatePixmap(screen, 0, 0, depth, RADEON_CREATE_PIXMAP_DRI2);
	if (!pixmap)
		return NULL;

	if (!screen->ModifyPixmapHeader(pixmap, width, height, 0, bpp, stride,
					NULL))
		goto free_pixmap;

	if (screen->SetSharedPixmapBacking(pixmap, (void*)(intptr_t)fd))
		return pixmap;

free_pixmap:
	fbDestroyPixmap(pixmap);
	return NULL;
}

static int radeon_dri3_fd_from_pixmap(ScreenPtr screen,
				      PixmapPtr pixmap,
				      CARD16 *stride,
				      CARD32 *size)
{
	struct radeon_buffer *bo;
	int fd;
#ifdef USE_GLAMOR
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONInfoPtr info = RADEONPTR(scrn);

	if (info->use_glamor) {
		int ret = glamor_fd_from_pixmap(screen, pixmap, stride, size);

		/* Any pending drawing operations need to be flushed to the
		 * kernel driver before the client starts using the pixmap
		 * storage for direct rendering.
		 */
		if (ret >= 0)
			radeon_cs_flush_indirect(scrn);

		return ret;
	}
#endif

	bo = radeon_get_pixmap_bo(pixmap);
	if (!bo) {
		exaMoveInPixmap(pixmap);
		bo = radeon_get_pixmap_bo(pixmap);
		if (!bo)
			return -1;
	}

	if (pixmap->devKind > UINT16_MAX)
		return -1;

	if (radeon_gem_prime_share_bo(bo->bo.radeon, &fd) < 0)
		return -1;

	*stride = pixmap->devKind;
	*size = bo->bo.radeon->size;
	return fd;
}

static dri3_screen_info_rec radeon_dri3_screen_info = {
#if DRI3_SCREEN_INFO_VERSION >= 1 && XORG_VERSION_CURRENT <= XORG_VERSION_NUMERIC(1,18,99,1,0)
	.version = 1,
	.open_client = radeon_dri3_open_client,
#else
	.version = 0,
	.open = radeon_dri3_open,
#endif
	.pixmap_from_fd = radeon_dri3_pixmap_from_fd,
	.fd_from_pixmap = radeon_dri3_fd_from_pixmap
};

Bool
radeon_dri3_screen_init(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);

	pRADEONEnt->render_node = drmGetRenderDeviceNameFromFd(pRADEONEnt->fd);

	if (!dri3_screen_init(screen, &radeon_dri3_screen_info)) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "dri3_screen_init failed\n");
		return FALSE;
	}

	return TRUE;
}

#else /* !HAVE_DRI3_H */

Bool
radeon_dri3_screen_init(ScreenPtr screen)
{
	xf86DrvMsg(xf86ScreenToScrn(screen)->scrnIndex, X_INFO,
		   "Can't initialize DRI3 because dri3.h not available at "
		   "build time\n");

	return FALSE;
}

#endif
