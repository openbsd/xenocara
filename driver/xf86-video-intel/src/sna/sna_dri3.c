/*
 * Copyright (c) 2014 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <xf86drm.h>

#include "sna.h"

#include <xf86.h>
#include <dri3.h>
#include <misyncshm.h>
#include <misyncstr.h>

static DevPrivateKeyRec sna_sync_fence_private_key;
struct sna_sync_fence {
	SyncFenceSetTriggeredFunc set_triggered;
};

static inline struct sna_sync_fence *sna_sync_fence(SyncFence *fence)
{
	return dixLookupPrivate(&fence->devPrivates, &sna_sync_fence_private_key);
}

static void sna_sync_flush(struct sna *sna, struct sna_pixmap *priv)
{
	struct kgem_bo *bo = NULL;

	DBG(("%s(pixmap=%ld)\n", __FUNCTION__, priv->pixmap->drawable.serialNumber));
	assert(priv);

	if (priv->pinned & PIN_DRI3) {
		assert(priv->gpu_bo);
		assert(priv->pinned & PIN_DRI3);
		DBG(("%s: flushing prime GPU bo, handle=%ld\n", __FUNCTION__, priv->gpu_bo->handle));
		if (sna_pixmap_move_to_gpu(priv->pixmap, MOVE_READ | MOVE_WRITE | MOVE_ASYNC_HINT | __MOVE_FORCE)) {
			sna_damage_all(&priv->gpu_damage, priv->pixmap);
			bo = priv->gpu_bo;
		}
	} else {
		assert(priv->cpu_bo);
		assert(IS_STATIC_PTR(priv->ptr));
		DBG(("%s: flushing prime CPU bo, handle=%ld\n", __FUNCTION__, priv->cpu_bo->handle));
		if (sna_pixmap_move_to_cpu(priv->pixmap, MOVE_READ | MOVE_WRITE | MOVE_ASYNC_HINT))
			bo = priv->cpu_bo;
	}

	if (bo != NULL) {
		kgem_bo_submit(&sna->kgem, bo);
		kgem_bo_unclean(&sna->kgem, bo);
	}
}

static void
sna_sync_fence_set_triggered(SyncFence *fence)
{
	struct sna *sna = to_sna_from_screen(fence->pScreen);
	struct sna_sync_fence *sna_fence = sna_sync_fence(fence);
	DrawablePtr draw = NULL;

	DBG(("%s()\n", __FUNCTION__));

#if 0
	draw = miSyncShmFenceGetDrawable(fence);
#endif
	if (draw) {
		DBG(("%s: associated pixmap=%ld\n", __FUNCTION__, get_drawable_pixmap(draw)->drawable.serialNumber));
		sna_sync_flush(sna, sna_pixmap(get_drawable_pixmap(draw)));
	} else { /* SyncFence are currently per-screen, sigh */
		struct sna_pixmap *priv;

		DBG(("%s: flushing all DRI3 pixmaps\n", __FUNCTION__));
		list_for_each_entry(priv, &sna->dri3.pixmaps, cow_list)
			sna_sync_flush(sna, priv);

		sna_accel_flush(sna);
	}

	DBG(("%s: complete, chaining up\n", __FUNCTION__));
	fence->funcs.SetTriggered = sna_fence->set_triggered;
	sna_fence->set_triggered(fence);
	sna_fence->set_triggered = fence->funcs.SetTriggered;
	fence->funcs.SetTriggered = sna_sync_fence_set_triggered;
}

static void
sna_sync_create_fence(ScreenPtr screen, SyncFence *fence, Bool initially_triggered)
{
	struct sna *sna = to_sna_from_screen(screen);
	SyncScreenFuncsPtr funcs = miSyncGetScreenFuncs(screen);

	DBG(("%s()\n", __FUNCTION__));

	funcs->CreateFence = sna->dri3.create_fence;
	sna->dri3.create_fence(screen, fence, initially_triggered);
	sna->dri3.create_fence = funcs->CreateFence;
	funcs->CreateFence = sna_sync_create_fence;

	sna_sync_fence(fence)->set_triggered = fence->funcs.SetTriggered;
	fence->funcs.SetTriggered = sna_sync_fence_set_triggered;
}

static bool
sna_sync_open(struct sna *sna, ScreenPtr screen)
{
	SyncScreenFuncsPtr funcs;

	DBG(("%s()\n", __FUNCTION__));

	if (!miSyncShmScreenInit(screen))
		return false;

	if (!dixPrivateKeyRegistered(&sna_sync_fence_private_key)) {
		if (!dixRegisterPrivateKey(&sna_sync_fence_private_key,
					   PRIVATE_SYNC_FENCE,
					   sizeof(struct sna_sync_fence)))
			return false;
	}

	funcs = miSyncGetScreenFuncs(screen);
	sna->dri3.create_fence = funcs->CreateFence;
	funcs->CreateFence = sna_sync_create_fence;

	return true;
}

static int sna_dri3_open_device(ScreenPtr screen,
				RRProviderPtr provider,
				int *out)
{
	int fd;

	DBG(("%s()\n", __FUNCTION__));
	fd = intel_get_client_fd(xf86ScreenToScrn(screen));
	if (fd < 0)
		return -fd;

	*out = fd;
	return Success;
}

static PixmapPtr sna_dri3_pixmap_from_fd(ScreenPtr screen,
					 int fd,
					 CARD16 width,
					 CARD16 height,
					 CARD16 stride,
					 CARD8 depth,
					 CARD8 bpp)
{
	struct sna *sna = to_sna_from_screen(screen);
	PixmapPtr pixmap;
	struct sna_pixmap *priv;
	struct kgem_bo *bo;

	DBG(("%s(fd=%d, width=%d, height=%d, stride=%d, depth=%d, bpp=%d)\n",
	     __FUNCTION__, fd, width, height, stride, depth, bpp));
	if (width > INT16_MAX || height > INT16_MAX)
		return NULL;

	if ((uint32_t)width * bpp > (uint32_t)stride * 8)
		return NULL;

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

	bo = kgem_create_for_prime(&sna->kgem, fd, (uint32_t)stride * height);
	if (bo == NULL)
		return NULL;

	/* Check for a duplicate */
	list_for_each_entry(priv, &sna->dri3.pixmaps, cow_list) {
		int other_stride = 0;
		if (bo->snoop) {
			assert(priv->cpu_bo);
			assert(IS_STATIC_PTR(priv->ptr));
			if (bo->handle == priv->cpu_bo->handle)
				other_stride = priv->cpu_bo->pitch;
		} else  {
			assert(priv->gpu_bo);
			assert(priv->pinned & PIN_DRI3);
			if (bo->handle == priv->gpu_bo->handle)
				other_stride = priv->gpu_bo->pitch;
		}
		if (other_stride) {
			pixmap = priv->pixmap;
			DBG(("%s: imported fd matches existing DRI3 pixmap=%ld\n", __FUNCTION__, pixmap->drawable.serialNumber));
			bo->handle = 0; /* fudge to prevent gem_close */
			kgem_bo_destroy(&sna->kgem, bo);
			if (width != pixmap->drawable.width ||
			    height != pixmap->drawable.height ||
			    depth != pixmap->drawable.depth ||
			    bpp != pixmap->drawable.bitsPerPixel ||
			    stride != other_stride) {
				DBG(("%s: imported fd mismatches existing DRI3 pixmap (width=%d, height=%d, depth=%d, bpp=%d, stride=%d)\n", __FUNCTION__,
				     pixmap->drawable.width,
				     pixmap->drawable.height,
				     pixmap->drawable.depth,
				     pixmap->drawable.bitsPerPixel,
				     other_stride));
				return NULL;
			}
			sna_sync_flush(sna, priv);
			pixmap->refcnt++;
			return pixmap;
		}
	}

	if (!kgem_check_surface_size(&sna->kgem,
				     width, height, bpp,
				     bo->tiling, stride, kgem_bo_size(bo))) {
		DBG(("%s: client supplied pitch=%d, size=%d too small for %dx%d surface\n",
		     __FUNCTION__, stride, kgem_bo_size(bo), width, height));
		goto free_bo;
	}

	pixmap = sna_pixmap_create_unattached(screen, 0, 0, depth);
	if (pixmap == NullPixmap)
		goto free_bo;

	if (!screen->ModifyPixmapHeader(pixmap, width, height,
					depth, bpp, stride, NULL))
		goto free_pixmap;

	priv = sna_pixmap_attach_to_bo(pixmap, bo);
	if (priv == NULL)
		goto free_pixmap;

	bo->pitch = stride;
	priv->stride = stride;

	if (bo->snoop) {
		assert(priv->cpu_bo == bo);
		pixmap->devPrivate.ptr = kgem_bo_map__cpu(&sna->kgem, priv->cpu_bo);
		if (pixmap->devPrivate.ptr == NULL)
			goto free_pixmap;

		pixmap->devKind = stride;
		priv->ptr = MAKE_STATIC_PTR(pixmap->devPrivate.ptr);
	} else {
		assert(priv->gpu_bo == bo);
		priv->pinned |= PIN_DRI3;
	}
	list_add(&priv->cow_list, &sna->dri3.pixmaps);

	return pixmap;

free_pixmap:
	screen->DestroyPixmap(pixmap);
free_bo:
	kgem_bo_destroy(&sna->kgem, bo);
	return NULL;
}

static int sna_dri3_fd_from_pixmap(ScreenPtr screen,
				   PixmapPtr pixmap,
				   CARD16 *stride,
				   CARD32 *size)
{
	struct sna *sna = to_sna_from_screen(screen);
	struct sna_pixmap *priv;
	struct kgem_bo *bo = NULL;
	int fd;

	DBG(("%s(pixmap=%ld, width=%d, height=%d)\n", __FUNCTION__,
	     pixmap->drawable.serialNumber, pixmap->drawable.width, pixmap->drawable.height));
	if (pixmap == sna->front && sna->flags & SNA_TEAR_FREE) {
		DBG(("%s: DRI3 protocol cannot support TearFree frontbuffers\n", __FUNCTION__));
		return -1;
	}

	priv = sna_pixmap(pixmap);
	if (priv && IS_STATIC_PTR(priv->ptr) && priv->cpu_bo) {
		if (sna_pixmap_move_to_cpu(pixmap, MOVE_READ | MOVE_WRITE | MOVE_ASYNC_HINT))
			bo = priv->cpu_bo;
	} else {
		priv = sna_pixmap_move_to_gpu(pixmap, MOVE_READ | MOVE_WRITE | MOVE_ASYNC_HINT | __MOVE_FORCE | __MOVE_DRI);
		if (priv != NULL) {
			sna_damage_all(&priv->gpu_damage, pixmap);
			bo = priv->gpu_bo;
		}
	}
	if (bo == NULL) {
		DBG(("%s: pixmap not supported by GPU\n", __FUNCTION__));
		return -1;
	}
	assert(priv != NULL);

	if (bo->pitch > UINT16_MAX) {
		DBG(("%s: pixmap pitch (%d) too large for DRI3 protocol\n",
		     __FUNCTION__, bo->pitch));
		return -1;
	}

	fd = kgem_bo_export_to_prime(&sna->kgem, bo);
	if (fd == -1) {
		DBG(("%s: exporting handle=%d to fd failed\n", __FUNCTION__, bo->handle));
		return -1;
	}

	if (bo == priv->gpu_bo)
		priv->pinned |= PIN_DRI3;
	list_move(&priv->cow_list, &sna->dri3.pixmaps);

	*stride = (priv->pinned & PIN_DRI3) ? priv->gpu_bo->pitch : priv->cpu_bo->pitch;
	*size = kgem_bo_size((priv->pinned & PIN_DRI3) ? priv->gpu_bo : priv->cpu_bo);
	DBG(("%s: exporting %s pixmap=%ld, handle=%d, stride=%d, size=%d\n",
	     __FUNCTION__,
	     (priv->pinned & PIN_DRI3) ? "GPU" : "CPU", pixmap->drawable.serialNumber,
	     (priv->pinned & PIN_DRI3) ? priv->gpu_bo->handle : priv->cpu_bo->handle,
	     *stride, *size));
	return fd;
}

static dri3_screen_info_rec sna_dri3_info = {
	.version = DRI3_SCREEN_INFO_VERSION,

	.open = sna_dri3_open_device,
	.pixmap_from_fd = sna_dri3_pixmap_from_fd,
	.fd_from_pixmap = sna_dri3_fd_from_pixmap,
};

bool sna_dri3_open(struct sna *sna, ScreenPtr screen)
{
	DBG(("%s()\n", __FUNCTION__));

	if (!sna_sync_open(sna, screen))
		return false;

	list_init(&sna->dri3.pixmaps);
	return dri3_screen_init(screen, &sna_dri3_info);
}

void sna_dri3_close(struct sna *sna, ScreenPtr screen)
{
	SyncScreenFuncsPtr funcs;

	DBG(("%s()\n", __FUNCTION__));

	funcs = miSyncGetScreenFuncs(screen);
	if (funcs)
		funcs->CreateFence = sna->dri3.create_fence;
}
