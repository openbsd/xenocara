/**************************************************************************

Copyright 2001 VA Linux Systems Inc., Fremont, California.
Copyright © 2002 by David Dawes

All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
on the rights to use, copy, modify, merge, publish, distribute, sub
license, and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice (including the next
paragraph) shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
ATI, VA LINUX SYSTEMS AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

/*
 * Authors: Jeff Hartmann <jhartmann@valinux.com>
 *          David Dawes <dawes@xfree86.org>
 *          Keith Whitwell <keith@tungstengraphics.com>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

#include "sna.h"
#include "intel_options.h"

#include <xf86drm.h>
#include <i915_drm.h>
#include <dri2.h>
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,12,99,901,0) && defined(COMPOSITE)
#include <compositeext.h>
#define CHECK_FOR_COMPOSITOR
#endif

#define DBG_CAN_FLIP 1
#define DBG_CAN_XCHG 1

#define DBG_FORCE_COPY -1 /* KGEM_BLT or KGEM_3D */

#if DRI2INFOREC_VERSION < 2
#error DRI2 version supported by the Xserver is too old
#endif

static inline struct kgem_bo *ref(struct kgem_bo *bo)
{
	assert(bo->refcnt);
	bo->refcnt++;
	return bo;
}

static inline void unref(struct kgem_bo *bo)
{
	assert(bo->refcnt > 1);
	bo->refcnt--;
}

struct sna_dri2_private {
	PixmapPtr pixmap;
	struct kgem_bo *bo;
	DRI2Buffer2Ptr proxy;
	bool stale;
	uint32_t size;
	int refcnt;
};

static inline struct sna_dri2_private *
get_private(void *buffer)
{
	return (struct sna_dri2_private *)((DRI2Buffer2Ptr)buffer+1);
}

#if DRI2INFOREC_VERSION >= 4
enum event_type {
	WAITMSC = 0,
	SWAP,
	SWAP_WAIT,
	SWAP_THROTTLE,
	FLIP,
	FLIP_THROTTLE,
	FLIP_COMPLETE,
	FLIP_ASYNC,
};

struct dri_bo {
	struct list link;
	struct kgem_bo *bo;
	uint32_t name;
};

struct sna_dri2_event {
	DrawablePtr draw;
	ClientPtr client;
	enum event_type type;
	xf86CrtcPtr crtc;
	int pipe;
	bool queued;

	/* for swaps & flips only */
	DRI2SwapEventPtr event_complete;
	void *event_data;
	DRI2BufferPtr front;
	DRI2BufferPtr back;
	struct kgem_bo *bo;

	struct sna_dri2_event *chain;

	struct list cache;
	struct list link;

	int mode;
};

static void sna_dri2_flip_event(struct sna *sna,
				struct sna_dri2_event *flip);

static void
sna_dri2_get_back(struct sna *sna,
		  DrawablePtr draw,
		  DRI2BufferPtr back,
		  struct sna_dri2_event *info)
{
	struct kgem_bo *bo;
	uint32_t name;
	bool reuse;

	DBG(("%s: draw size=%dx%d, buffer size=%dx%d\n",
	     __FUNCTION__, draw->width, draw->height,
	     get_private(back)->size & 0xffff, get_private(back)->size >> 16));
	reuse = (draw->height << 16 | draw->width) == get_private(back)->size;
	if (reuse) {
		bo = get_private(back)->bo;
		assert(bo->refcnt);
		DBG(("%s: back buffer handle=%d, scanout?=%d, refcnt=%d\n",
					__FUNCTION__, bo->handle, bo->active_scanout, get_private(back)->refcnt));
		if (bo->active_scanout == 0) {
			DBG(("%s: reuse unattached back\n", __FUNCTION__));
			get_private(back)->stale = false;
			return;
		}
	}

	bo = NULL;
	if (info) {
		struct dri_bo *c;
		list_for_each_entry(c, &info->cache, link) {
			if (c->bo && c->bo->scanout == 0) {
				bo = c->bo;
				name = c->name;
				DBG(("%s: reuse cache handle=%d\n", __FUNCTION__, bo->handle));
				list_move_tail(&c->link, &info->cache);
				c->bo = NULL;
			}
		}
	}
	if (bo == NULL) {
		DBG(("%s: allocating new backbuffer\n", __FUNCTION__));
		bo = kgem_create_2d(&sna->kgem,
				    draw->width, draw->height, draw->bitsPerPixel,
				    get_private(back)->bo->tiling,
				    get_private(back)->bo->scanout ? CREATE_SCANOUT : 0);
		if (bo == NULL)
			return;

		name = kgem_bo_flink(&sna->kgem, bo);
		if (name == 0) {
			kgem_bo_destroy(&sna->kgem, bo);
			return;
		}
	}
	assert(bo->active_scanout == 0);

	if (info && reuse) {
		bool found = false;
		struct dri_bo *c;

		list_for_each_entry_reverse(c, &info->cache, link) {
			if (c->bo == NULL) {
				found = true;
				_list_del(&c->link);
				break;
			}
		}
		if (!found)
			c = malloc(sizeof(*c));
		if (c != NULL) {
			c->bo = ref(get_private(back)->bo);
			c->name = back->name;
			list_add(&c->link, &info->cache);
			DBG(("%s: cacheing handle=%d (name=%d)\n", __FUNCTION__, c->bo->handle, c->name));
		}
	}

	assert(bo != get_private(back)->bo);
	kgem_bo_destroy(&sna->kgem, get_private(back)->bo);

	get_private(back)->bo = bo;
	get_private(back)->size = draw->height << 16 | draw->width;
	back->pitch = bo->pitch;
	back->name = name;

	get_private(back)->stale = false;
}

struct dri2_window {
	DRI2BufferPtr front;
	struct sna_dri2_event *chain;
	xf86CrtcPtr crtc;
	int64_t msc_delta;
};

static struct dri2_window *dri2_window(WindowPtr win)
{
	assert(win->drawable.type != DRAWABLE_PIXMAP);
	return ((void **)__get_private(win, sna_window_key))[1];
}

static struct sna_dri2_event *
dri2_chain(DrawablePtr d)
{
	struct dri2_window *priv = dri2_window((WindowPtr)d);
	assert(priv != NULL);
	return priv->chain;
}
inline static DRI2BufferPtr dri2_window_get_front(WindowPtr win) { return dri2_window(win)->front; }
#else
inline static void *dri2_window_get_front(WindowPtr win) { return NULL; }
#endif

#if DRI2INFOREC_VERSION < 6

#define xorg_can_triple_buffer(ptr) 0
#define swap_limit(d, l) false

#else

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,15,99,904,0)
/* Prime fixed for triple buffer support */
#define xorg_can_triple_buffer(ptr) 1
#elif XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,12,99,901,0)
/* Before numGPUScreens was introduced */
#define xorg_can_triple_buffer(ptr) 1
#else
/* Subject to crashers when combining triple buffering and Prime */
inline static bool xorg_can_triple_buffer(struct sna *sna)
{
	return screenInfo.numGPUScreens == 0;
}
#endif

static Bool
sna_dri2_swap_limit_validate(DrawablePtr draw, int swap_limit)
{
	DBG(("%s: swap limit set to %d\n", __FUNCTION__, swap_limit));
	return swap_limit >= 1;
}

static void
sna_dri2_reuse_buffer(DrawablePtr draw, DRI2BufferPtr buffer)
{
	DBG(("%s: reusing buffer pixmap=%ld, attachment=%d, handle=%d, name=%d\n",
	     __FUNCTION__, get_drawable_pixmap(draw)->drawable.serialNumber,
	     buffer->attachment, get_private(buffer)->bo->handle, buffer->name));
	assert(get_private(buffer)->refcnt);
	assert(get_private(buffer)->bo->refcnt > get_private(buffer)->bo->active_scanout);

	if (buffer->attachment == DRI2BufferBackLeft &&
	    draw->type != DRAWABLE_PIXMAP) {
		DBG(("%s: replacing back buffer\n", __FUNCTION__));
		sna_dri2_get_back(to_sna_from_drawable(draw), draw, buffer, dri2_chain(draw));

		assert(kgem_bo_flink(&to_sna_from_drawable(draw)->kgem, get_private(buffer)->bo) == buffer->name);
		assert(get_private(buffer)->bo->refcnt);
		assert(get_private(buffer)->bo->active_scanout == 0);
	}
}

static bool swap_limit(DrawablePtr draw, int limit)
{
	DBG(("%s: draw=%ld setting swap limit to %d\n", __FUNCTION__, (long)draw->id, limit));
	DRI2SwapLimit(draw, limit);
	return true;
}
#endif

#if DRI2INFOREC_VERSION < 10
#undef USE_ASYNC_SWAP
#define USE_ASYNC_SWAP 0
#endif

#define COLOR_PREFER_TILING_Y 0

/* Prefer to enable TILING_Y if this buffer will never be a
 * candidate for pageflipping
 */
static uint32_t color_tiling(struct sna *sna, DrawablePtr draw)
{
	uint32_t tiling;

	if (COLOR_PREFER_TILING_Y &&
	    (draw->width  != sna->front->drawable.width ||
	     draw->height != sna->front->drawable.height))
		tiling = I915_TILING_Y;
	else
		tiling = I915_TILING_X;

	return kgem_choose_tiling(&sna->kgem, -tiling,
				  draw->width,
				  draw->height,
				  draw->bitsPerPixel);
}

static uint32_t other_tiling(struct sna *sna, DrawablePtr draw)
{
	/* XXX Can mix color X / depth Y? */
	return kgem_choose_tiling(&sna->kgem,
				  sna->kgem.gen >= 040 ? -I915_TILING_Y : -I915_TILING_X,
				  draw->width,
				  draw->height,
				  draw->bitsPerPixel);
}

static struct kgem_bo *sna_pixmap_set_dri(struct sna *sna,
					  PixmapPtr pixmap)
{
	struct sna_pixmap *priv;
	int tiling;

	DBG(("%s: attaching DRI client to pixmap=%ld\n",
	     __FUNCTION__, pixmap->drawable.serialNumber));

	priv = sna_pixmap(pixmap);
	if (priv != NULL && IS_STATIC_PTR(priv->ptr) && priv->cpu_bo) {
		DBG(("%s: SHM or unattached Pixmap, BadAlloc\n", __FUNCTION__));
		return NULL;
	}

	priv = sna_pixmap_move_to_gpu(pixmap,
				      MOVE_READ | __MOVE_FORCE | __MOVE_DRI);
	if (priv == NULL) {
		DBG(("%s: failed to move to GPU, BadAlloc\n", __FUNCTION__));
		return NULL;
	}

	assert(priv->flush == false);
	assert(priv->cpu_damage == NULL);
	assert(priv->gpu_bo);
	assert(priv->gpu_bo->proxy == NULL);
	assert(priv->gpu_bo->flush == false);

	tiling = color_tiling(sna, &pixmap->drawable);
	if (tiling < 0)
		tiling = -tiling;
	if (priv->gpu_bo->tiling != tiling)
		sna_pixmap_change_tiling(pixmap, tiling);

	return priv->gpu_bo;
}

pure static inline void *sna_pixmap_get_buffer(PixmapPtr pixmap)
{
	assert(pixmap->refcnt);
	return ((void **)__get_private(pixmap, sna_pixmap_key))[2];
}

static inline void sna_pixmap_set_buffer(PixmapPtr pixmap, void *ptr)
{
	assert(pixmap->refcnt);
	((void **)__get_private(pixmap, sna_pixmap_key))[2] = ptr;
}

void
sna_dri2_pixmap_update_bo(struct sna *sna, PixmapPtr pixmap, struct kgem_bo *bo)
{
	DRI2BufferPtr buffer;
	struct sna_dri2_private *private;

	buffer = sna_pixmap_get_buffer(pixmap);
	if (buffer == NULL)
		return;

	DBG(("%s: pixmap=%ld, old handle=%d, new handle=%d\n", __FUNCTION__,
	     pixmap->drawable.serialNumber,
	     get_private(buffer)->bo->handle,
	     sna_pixmap(pixmap)->gpu_bo->handle));

	private = get_private(buffer);
	assert(private->pixmap == pixmap);

	assert(bo != private->bo);
	if (private->bo == bo)
		return;

	DBG(("%s: dropping flush hint from handle=%d\n", __FUNCTION__, private->bo->handle));
	private->bo->flush = false;
	kgem_bo_destroy(&sna->kgem, private->bo);

	buffer->name = kgem_bo_flink(&sna->kgem, bo);
	private->bo = ref(bo);

	DBG(("%s: adding flush hint to handle=%d\n", __FUNCTION__, bo->handle));
	bo->flush = true;
	assert(sna_pixmap(pixmap)->flush);

	/* XXX DRI2InvalidateDrawable(&pixmap->drawable); */
}

static DRI2Buffer2Ptr
sna_dri2_create_buffer(DrawablePtr draw,
		       unsigned int attachment,
		       unsigned int format)
{
	struct sna *sna = to_sna_from_drawable(draw);
	DRI2Buffer2Ptr buffer;
	struct sna_dri2_private *private;
	PixmapPtr pixmap;
	struct kgem_bo *bo;
	unsigned flags = 0;
	uint32_t size;
	int bpp;

	DBG(("%s pixmap=%ld, (attachment=%d, format=%d, drawable=%dx%d)\n",
	     __FUNCTION__,
	     get_drawable_pixmap(draw)->drawable.serialNumber,
	     attachment, format, draw->width, draw->height));

	pixmap = NULL;
	size = (uint32_t)draw->height << 16 | draw->width;
	switch (attachment) {
	case DRI2BufferFrontLeft:
		pixmap = get_drawable_pixmap(draw);
		buffer = NULL;
		if (draw->type != DRAWABLE_PIXMAP)
			buffer = dri2_window_get_front((WindowPtr)draw);
		if (buffer == NULL)
			buffer = sna_pixmap_get_buffer(pixmap);
		if (buffer) {
			private = get_private(buffer);

			DBG(("%s: reusing front buffer attachment, win=%lu %dx%d, pixmap=%ld %dx%d, handle=%d, name=%d\n",
			     __FUNCTION__,
			     draw->type != DRAWABLE_PIXMAP ? (long)draw->id : (long)0,
			     draw->width, draw->height,
			     pixmap->drawable.serialNumber,
			     pixmap->drawable.width,
			     pixmap->drawable.height,
			     private->bo->handle, buffer->name));

			assert(private->pixmap == pixmap);
			assert(sna_pixmap(pixmap)->flush);
			assert(sna_pixmap(pixmap)->pinned & PIN_DRI2);
			assert(kgem_bo_flink(&sna->kgem, private->bo) == buffer->name);

			private->refcnt++;
			return buffer;
		}

		bo = sna_pixmap_set_dri(sna, pixmap);
		if (bo == NULL)
			return NULL;

		assert(sna_pixmap(pixmap) != NULL);

		bo = ref(bo);
		bpp = pixmap->drawable.bitsPerPixel;
		if (pixmap == sna->front && !(sna->flags & SNA_LINEAR_FB))
			flags |= CREATE_SCANOUT;
		DBG(("%s: attaching to front buffer %dx%d [%p:%d], scanout? %d\n",
		     __FUNCTION__,
		     pixmap->drawable.width, pixmap->drawable.height,
		     pixmap, pixmap->refcnt, flags & CREATE_SCANOUT));
		size = (uint32_t)pixmap->drawable.height << 16 | pixmap->drawable.width;
		break;

	case DRI2BufferBackLeft:
		if (draw->type != DRAWABLE_PIXMAP) {
			if (dri2_window_get_front((WindowPtr)draw))
				flags |= CREATE_SCANOUT;
			if (draw->width  == sna->front->drawable.width &&
			    draw->height == sna->front->drawable.height &&
			    (sna->flags & (SNA_LINEAR_FB | SNA_NO_WAIT | SNA_NO_FLIP)) == 0)
				flags |= CREATE_SCANOUT;
		}
	case DRI2BufferBackRight:
	case DRI2BufferFrontRight:
	case DRI2BufferFakeFrontLeft:
	case DRI2BufferFakeFrontRight:
		bpp = draw->bitsPerPixel;
		DBG(("%s: creating back buffer %dx%d, suitable for scanout? %d\n",
		     __FUNCTION__,
		     draw->width, draw->height,
		     flags & CREATE_SCANOUT));

		bo = kgem_create_2d(&sna->kgem,
				    draw->width,
				    draw->height,
				    draw->bitsPerPixel,
				    color_tiling(sna, draw),
				    flags);
		break;

	case DRI2BufferStencil:
		/*
		 * The stencil buffer has quirky pitch requirements.  From Vol
		 * 2a, 11.5.6.2.1 3DSTATE_STENCIL_BUFFER, field "Surface
		 * Pitch":
		 *    The pitch must be set to 2x the value computed based on
		 *    width, as the stencil buffer is stored with two rows
		 *    interleaved.
		 * To accomplish this, we resort to the nasty hack of doubling
		 * the drm region's cpp and halving its height.
		 *
		 * If we neglect to double the pitch, then
		 * drm_intel_gem_bo_map_gtt() maps the memory incorrectly.
		 *
		 * The alignment for W-tiling is quite different to the
		 * nominal no-tiling case, so we have to account for
		 * the tiled access pattern explicitly.
		 *
		 * The stencil buffer is W tiled. However, we request from
		 * the kernel a non-tiled buffer because the kernel does
		 * not understand W tiling and the GTT is incapable of
		 * W fencing.
		 */
		bpp = format ? format : draw->bitsPerPixel;
		bpp *= 2;
		bo = kgem_create_2d(&sna->kgem,
				    ALIGN(draw->width, 64),
				    ALIGN((draw->height + 1) / 2, 64),
				    bpp, I915_TILING_NONE, flags);
		break;

	case DRI2BufferDepth:
	case DRI2BufferDepthStencil:
	case DRI2BufferHiz:
	case DRI2BufferAccum:
		bpp = format ? format : draw->bitsPerPixel,
		bo = kgem_create_2d(&sna->kgem,
				    draw->width, draw->height, bpp,
				    other_tiling(sna, draw),
				    flags);
		break;

	default:
		return NULL;
	}
	if (bo == NULL)
		return NULL;

	buffer = calloc(1, sizeof *buffer + sizeof *private);
	if (buffer == NULL)
		goto err;

	private = get_private(buffer);
	buffer->attachment = attachment;
	buffer->pitch = bo->pitch;
	buffer->cpp = bpp / 8;
	buffer->driverPrivate = private;
	buffer->format = format;
	buffer->flags = 0;
	buffer->name = kgem_bo_flink(&sna->kgem, bo);
	private->refcnt = 1;
	private->bo = bo;
	private->pixmap = pixmap;
	private->size = size;

	if (buffer->name == 0)
		goto err;

	if (pixmap) {
		struct sna_pixmap *priv;

		assert(attachment == DRI2BufferFrontLeft);
		assert(sna_pixmap_get_buffer(pixmap) == NULL);

		sna_pixmap_set_buffer(pixmap, buffer);
		assert(sna_pixmap_get_buffer(pixmap) == buffer);
		pixmap->refcnt++;

		priv = sna_pixmap(pixmap);
		assert(priv->flush == false);
		assert((priv->pinned & PIN_DRI2) == 0);

		/* Don't allow this named buffer to be replaced */
		priv->pinned |= PIN_DRI2;

		/* We need to submit any modifications to and reads from this
		 * buffer before we send any reply to the Client.
		 *
		 * As we don't track which Client, we flush for all.
		 */
		DBG(("%s: adding flush hint to handle=%d\n", __FUNCTION__, priv->gpu_bo->handle));
		priv->gpu_bo->flush = true;
		if (priv->gpu_bo->exec)
			sna->kgem.flush = 1;

		priv->flush |= 1;
		if (draw->type == DRAWABLE_PIXMAP) {
			/* DRI2 renders directly into GLXPixmaps, treat as hostile */
			kgem_bo_unclean(&sna->kgem, priv->gpu_bo);
			sna_damage_all(&priv->gpu_damage, pixmap);
			priv->clear = false;
			priv->cpu = false;
			priv->flush |= 2;
		}

		sna_accel_watch_flush(sna, 1);
	}

	return buffer;

err:
	kgem_bo_destroy(&sna->kgem, bo);
	free(buffer);
	return NULL;
}

static void _sna_dri2_destroy_buffer(struct sna *sna, DRI2Buffer2Ptr buffer)
{
	struct sna_dri2_private *private = get_private(buffer);

	if (buffer == NULL)
		return;

	DBG(("%s: %p [handle=%d] -- refcnt=%d, pixmap=%ld\n",
	     __FUNCTION__, buffer, private->bo->handle, private->refcnt,
	     private->pixmap ? private->pixmap->drawable.serialNumber : 0));
	assert(private->refcnt > 0);
	if (--private->refcnt)
		return;

	assert(private->bo);

	if (private->proxy) {
		DBG(("%s: destroying proxy\n", __FUNCTION__));
		_sna_dri2_destroy_buffer(sna, private->proxy);
		private->pixmap = NULL;
	}

	if (private->pixmap) {
		PixmapPtr pixmap = private->pixmap;
		struct sna_pixmap *priv = sna_pixmap(pixmap);

		assert(sna_pixmap_get_buffer(pixmap) == buffer);
		assert(priv->gpu_bo == private->bo);
		assert(priv->gpu_bo->flush);
		assert(priv->pinned & PIN_DRI2);
		assert(priv->flush);

		/* Undo the DRI markings on this pixmap */
		DBG(("%s: releasing last DRI pixmap=%ld, scanout?=%d\n",
		     __FUNCTION__,
		     pixmap->drawable.serialNumber,
		     pixmap == sna->front));

		list_del(&priv->flush_list);

		DBG(("%s: dropping flush hint from handle=%d\n", __FUNCTION__, private->bo->handle));
		priv->gpu_bo->flush = false;
		priv->pinned &= ~PIN_DRI2;

		priv->flush = false;
		sna_accel_watch_flush(sna, -1);

		sna_pixmap_set_buffer(pixmap, NULL);
		pixmap->drawable.pScreen->DestroyPixmap(pixmap);
	}
	assert(private->bo->flush == false);

	kgem_bo_destroy(&sna->kgem, private->bo);
	free(buffer);
}

static void sna_dri2_destroy_buffer(DrawablePtr draw, DRI2Buffer2Ptr buffer)
{
	_sna_dri2_destroy_buffer(to_sna_from_drawable(draw), buffer);
}

static DRI2BufferPtr sna_dri2_reference_buffer(DRI2BufferPtr buffer)
{
	get_private(buffer)->refcnt++;
	return buffer;
}

static inline void damage(PixmapPtr pixmap, struct sna_pixmap *priv, RegionPtr region)
{
	assert(priv->gpu_bo);
	if (DAMAGE_IS_ALL(priv->gpu_damage))
		goto done;

	if (region == NULL) {
damage_all:
		priv->gpu_damage = _sna_damage_all(priv->gpu_damage,
						   pixmap->drawable.width,
						   pixmap->drawable.height);
		sna_damage_destroy(&priv->cpu_damage);
		list_del(&priv->flush_list);
	} else {
		sna_damage_subtract(&priv->cpu_damage, region);
		if (priv->cpu_damage == NULL)
			goto damage_all;
		sna_damage_add(&priv->gpu_damage, region);
	}
done:
	priv->cpu = false;
	priv->clear = false;
}

static void set_bo(PixmapPtr pixmap, struct kgem_bo *bo)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	RegionRec region;

	DBG(("%s: pixmap=%ld, handle=%d\n",
	     __FUNCTION__, pixmap->drawable.serialNumber, bo->handle));

	assert(pixmap->drawable.width * pixmap->drawable.bitsPerPixel <= 8*bo->pitch);
	assert(pixmap->drawable.height * bo->pitch <= kgem_bo_size(bo));
	assert(bo->proxy == NULL);
	assert(priv->pinned & PIN_DRI2);
	assert((priv->pinned & (PIN_PRIME | PIN_DRI3)) == 0);
	assert(priv->flush);

	/* Post damage on the new front buffer so that listeners, such
	 * as DisplayLink know take a copy and shove it over the USB,
	 * also for software cursors and the like.
	 */
	region.extents.x1 = region.extents.y1 = 0;
	region.extents.x2 = pixmap->drawable.width;
	region.extents.y2 = pixmap->drawable.height;
	region.data = NULL;
	DamageRegionAppend(&pixmap->drawable, &region);

	damage(pixmap, priv, NULL);

	assert(bo->refcnt);
	if (priv->move_to_gpu)
		priv->move_to_gpu(sna, priv, 0);
	if (priv->gpu_bo != bo) {
		DBG(("%s: dropping flush hint from handle=%d\n", __FUNCTION__, priv->gpu_bo->handle));
		priv->gpu_bo->flush = false;
		if (priv->cow)
			sna_pixmap_undo_cow(sna, priv, 0);
		if (priv->gpu_bo) {
			sna_pixmap_unmap(pixmap, priv);
			kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		}
		DBG(("%s: adding flush hint to handle=%d\n", __FUNCTION__, bo->handle));
		bo->flush = true;
		if (bo->exec)
			sna->kgem.flush = 1;
		priv->gpu_bo = ref(bo);
	}
	if (bo->domain != DOMAIN_GPU)
		bo->domain = DOMAIN_NONE;
	assert(bo->flush);

	DamageRegionProcessPending(&pixmap->drawable);
}

static void sna_dri2_select_mode(struct sna *sna, struct kgem_bo *dst, struct kgem_bo *src, bool sync)
{
	struct drm_i915_gem_busy busy;
	int mode;

	if (sna->kgem.gen < 060)
		return;

	if (sync) {
		DBG(("%s: sync, force %s ring\n", __FUNCTION__,
		     sna->kgem.gen >= 070 ? "BLT" : "RENDER"));
		kgem_set_mode(&sna->kgem,
			      sna->kgem.gen >= 070 ? KGEM_BLT : KGEM_RENDER,
			      dst);
		return;
	}

	if (DBG_FORCE_COPY != -1) {
		DBG(("%s: forcing %d\n", __FUNCTION__, DBG_FORCE_COPY));
		kgem_set_mode(&sna->kgem, DBG_FORCE_COPY, dst);
		return;
	}

	if (sna->kgem.mode != KGEM_NONE) {
		DBG(("%s: busy, not switching\n", __FUNCTION__));
		return;
	}

	VG_CLEAR(busy);
	busy.handle = src->handle;
	if (drmIoctl(sna->kgem.fd, DRM_IOCTL_I915_GEM_BUSY, &busy))
		return;

	DBG(("%s: src handle=%d busy?=%x\n", __FUNCTION__, busy.handle, busy.busy));
	if (busy.busy == 0) {
		__kgem_bo_clear_busy(src);

		busy.handle = dst->handle;
		if (drmIoctl(sna->kgem.fd, DRM_IOCTL_I915_GEM_BUSY, &busy))
			return;

		DBG(("%s: dst handle=%d busy?=%x\n", __FUNCTION__, busy.handle, busy.busy));
		if (busy.busy == 0) {
			__kgem_bo_clear_busy(dst);
			DBG(("%s: src/dst is idle, using defaults\n", __FUNCTION__));
			return;
		}
	}

	/* Sandybridge introduced a separate ring which it uses to
	 * perform blits. Switching rendering between rings incurs
	 * a stall as we wait upon the old ring to finish and
	 * flush its render cache before we can proceed on with
	 * the operation on the new ring.
	 *
	 * As this buffer, we presume, has just been written to by
	 * the DRI client using the RENDER ring, we want to perform
	 * our operation on the same ring, and ideally on the same
	 * ring as we will flip from (which should be the RENDER ring
	 * as well).
	 *
	 * The ultimate question is whether preserving the ring outweighs
	 * the cost of the query.
	 */
	mode = KGEM_RENDER;
	if (busy.busy & (0xfffe << 16))
		mode = KGEM_BLT;
	kgem_bo_mark_busy(&sna->kgem, busy.handle == src->handle ? src : dst, mode);
	_kgem_set_mode(&sna->kgem, mode);
}

static bool can_copy_cpu(struct sna *sna,
			 struct kgem_bo *src,
			 struct kgem_bo *dst)
{
	if (src->tiling != dst->tiling)
		return false;

	if (src->pitch != dst->pitch)
		return false;

	if (!kgem_bo_can_map__cpu(&sna->kgem, src, false))
		return false;

	if (!kgem_bo_can_map__cpu(&sna->kgem, dst, true))
		return false;

	DBG(("%s -- yes, src handle=%d, dst handle=%d\n", __FUNCTION__, src->handle, dst->handle));
	return true;
}

static void
sna_dri2_copy_fallback(struct sna *sna,
		       const DrawableRec *draw,
		       struct kgem_bo *src_bo, int sx, int sy,
		       struct kgem_bo *dst_bo, int dx, int dy,
		       const BoxRec *box, int n)
{
	void *dst, *src;
	bool clipped;

	clipped = (n > 1 ||
		   box->x1 + sx > 0 ||
		   box->y1 + sy > 0 ||
		   box->x2 + sx < draw->width ||
		   box->y2 + sy < draw->height);

	dst = src = NULL;
	if (!clipped && can_copy_cpu(sna, src_bo, dst_bo)) {
		dst = kgem_bo_map__cpu(&sna->kgem, dst_bo);
		src = kgem_bo_map__cpu(&sna->kgem, src_bo);
	}

	if (dst == NULL || src == NULL) {
		dst = kgem_bo_map__gtt(&sna->kgem, dst_bo);
		src = kgem_bo_map__gtt(&sna->kgem, src_bo);
		if (dst == NULL || src == NULL)
			return;
	} else {
		kgem_bo_sync__cpu_full(&sna->kgem, dst_bo, true);
		kgem_bo_sync__cpu_full(&sna->kgem, src_bo, false);
	}

	DBG(("%s: src(%d, %d), dst(%d, %d) x %d\n",
	     __FUNCTION__, sx, sy, dx, dy, n));

	if (sigtrap_get() == 0) {
		do {
			memcpy_blt(src, dst, draw->bitsPerPixel,
				   src_bo->pitch, dst_bo->pitch,
				   box->x1 + sx, box->y1 + sy,
				   box->x1 + dx, box->y1 + dy,
				   box->x2 - box->x1, box->y2 - box->y1);
			box++;
		} while (--n);
		sigtrap_put();
	}
}

static bool is_front(int attachment)
{
	return attachment == DRI2BufferFrontLeft;
}

static struct kgem_bo *
__sna_dri2_copy_region(struct sna *sna, DrawablePtr draw, RegionPtr region,
		      DRI2BufferPtr src, DRI2BufferPtr dst,
		      bool sync)
{
	PixmapPtr pixmap = get_drawable_pixmap(draw);
	DrawableRec scratch, *src_draw = &pixmap->drawable, *dst_draw = &pixmap->drawable;
	struct sna_dri2_private *src_priv = get_private(src);
	struct sna_dri2_private *dst_priv = get_private(dst);
	pixman_region16_t clip;
	struct kgem_bo *bo = NULL;
	struct kgem_bo *src_bo;
	struct kgem_bo *dst_bo;
	const BoxRec *boxes;
	int16_t dx, dy, sx, sy;
	int n;

	/* To hide a stale DRI2Buffer, one may choose to substitute
	 * pixmap->gpu_bo instead of dst/src->bo, however you then run
	 * the risk of copying around invalid data. So either you may not
	 * see the results of the copy, or you may see the wrong pixels.
	 * Either way you eventually lose.
	 *
	 * We also have to be careful in case that the stale buffers are
	 * now attached to invalid (non-DRI) pixmaps.
	 */

	assert(is_front(dst->attachment) || is_front(src->attachment));
	assert(dst->attachment != src->attachment);

	clip.extents.x1 = draw->x;
	clip.extents.y1 = draw->y;
	clip.extents.x2 = draw->x + draw->width;
	clip.extents.y2 = draw->y + draw->height;
	clip.data = NULL;

	if (region) {
		pixman_region_translate(region, draw->x, draw->y);
		pixman_region_intersect(&clip, &clip, region);
		region = &clip;
	}

	if (clip.extents.x1 >= clip.extents.x2 ||
	    clip.extents.y1 >= clip.extents.y2) {
		DBG(("%s: all clipped\n", __FUNCTION__));
		return NULL;
	}

	sx = sy = dx = dy = 0;
	if (is_front(dst->attachment)) {
		sx = -draw->x;
		sy = -draw->y;
	} else {
		dx = -draw->x;
		dy = -draw->y;
	}
	if (draw->type == DRAWABLE_WINDOW) {
		WindowPtr win = (WindowPtr)draw;
		int16_t tx, ty;

		if (is_clipped(&win->clipList, draw)) {
			DBG(("%s: draw=(%d, %d), delta=(%d, %d), draw=(%d, %d),(%d, %d), clip.extents=(%d, %d), (%d, %d)\n",
			     __FUNCTION__, draw->x, draw->y,
			     get_drawable_dx(draw), get_drawable_dy(draw),
			     clip.extents.x1, clip.extents.y1,
			     clip.extents.x2, clip.extents.y2,
			     win->clipList.extents.x1, win->clipList.extents.y1,
			     win->clipList.extents.x2, win->clipList.extents.y2));

			assert(region == NULL || region == &clip);
			pixman_region_intersect(&clip, &win->clipList, &clip);
			if (!pixman_region_not_empty(&clip)) {
				DBG(("%s: all clipped\n", __FUNCTION__));
				return NULL;
			}

			region = &clip;
		}

		if (get_drawable_deltas(draw, pixmap, &tx, &ty)) {
			if (is_front(dst->attachment)) {
				pixman_region_translate(region ?: &clip, tx, ty);
				sx -= tx;
				sy -= ty;
			} else {
				sx += tx;
				sy += ty;
			}
		}
	} else
		sync = false;

	scratch.x = scratch.y = 0;
	scratch.width = scratch.height = 0;
	scratch.depth = draw->depth;
	scratch.bitsPerPixel = draw->bitsPerPixel;

	src_bo = src_priv->bo;
	assert(src_bo->refcnt);
	if (is_front(src->attachment)) {
		struct sna_pixmap *priv;

		priv = sna_pixmap_move_to_gpu(pixmap, MOVE_READ);
		if (priv)
			src_bo = priv->gpu_bo;
		DBG(("%s: updated FrontLeft src_bo from handle=%d to handle=%d\n",
		     __FUNCTION__, src_priv->bo->handle, src_bo->handle));
		assert(src_bo->refcnt);
	} else {
		RegionRec source;

		scratch.width = src_priv->size & 0xffff;
		scratch.height = src_priv->size >> 16;
		src_draw = &scratch;

		DBG(("%s: source size %dx%d, region size %dx%d\n",
		     __FUNCTION__,
		     scratch.width, scratch.height,
		     clip.extents.x2 - clip.extents.x1,
		     clip.extents.y2 - clip.extents.y1));

		source.extents.x1 = -sx;
		source.extents.y1 = -sy;
		source.extents.x2 = source.extents.x1 + scratch.width;
		source.extents.y2 = source.extents.y1 + scratch.height;
		source.data = NULL;

		assert(region == NULL || region == &clip);
		pixman_region_intersect(&clip, &clip, &source);

	}

	dst_bo = dst_priv->bo;
	assert(dst_bo->refcnt);
	if (is_front(dst->attachment)) {
		struct sna_pixmap *priv;
		unsigned int flags;

		flags = MOVE_WRITE | __MOVE_FORCE;
		if (clip.data)
			flags |= MOVE_READ;

		assert(region == NULL || region == &clip);
		priv = sna_pixmap_move_area_to_gpu(pixmap, &clip.extents, flags);
		if (priv) {
			damage(pixmap, priv, region);
			dst_bo = priv->gpu_bo;
		}
		DBG(("%s: updated FrontLeft dst_bo from handle=%d to handle=%d\n",
		     __FUNCTION__, dst_priv->bo->handle, dst_bo->handle));
		assert(dst_bo->refcnt);
	} else {
		RegionRec target;

		scratch.width = dst_priv->size & 0xffff;
		scratch.height = dst_priv->size >> 16;
		dst_draw = &scratch;

		DBG(("%s: target size %dx%d, region size %dx%d\n",
		     __FUNCTION__,
		     scratch.width, scratch.height,
		     clip.extents.x2 - clip.extents.x1,
		     clip.extents.y2 - clip.extents.y1));

		target.extents.x1 = -dx;
		target.extents.y1 = -dy;
		target.extents.x2 = target.extents.x1 + scratch.width;
		target.extents.y2 = target.extents.y1 + scratch.height;
		target.data = NULL;

		assert(region == NULL || region == &clip);
		pixman_region_intersect(&clip, &clip, &target);

		sync = false;
	}

	if (!wedged(sna)) {
		xf86CrtcPtr crtc;

		crtc = NULL;
		if (sync && sna_pixmap_is_scanout(sna, pixmap))
			crtc = sna_covering_crtc(sna, &clip.extents, NULL);
		sna_dri2_select_mode(sna, dst_bo, src_bo, crtc != NULL);

		sync = (crtc != NULL&&
			sna_wait_for_scanline(sna, pixmap, crtc,
					      &clip.extents));
	}

	if (region) {
		boxes = region_rects(region);
		n = region_num_rects(region);
		assert(n);
	} else {
		region = &clip;
		boxes = &clip.extents;
		n = 1;
	}
	DamageRegionAppend(&pixmap->drawable, region);

	if (wedged(sna)) {
fallback:
		sna_dri2_copy_fallback(sna, src_draw,
				      src_bo, sx, sy,
				      dst_bo, dx, dy,
				      boxes, n);
	} else {
		unsigned flags;

		DBG(("%s: copying [(%d, %d), (%d, %d)]x%d src=(%d, %d), dst=(%d, %d)\n",
		     __FUNCTION__,
		     boxes[0].x1, boxes[0].y1,
		     boxes[0].x2, boxes[0].y2,
		     n, sx, sy, dx, dy));

		flags = COPY_LAST;
		if (sync)
			flags |= COPY_SYNC;
		if (!sna->render.copy_boxes(sna, GXcopy,
					    src_draw, src_bo, sx, sy,
					    dst_draw, dst_bo, dx, dy,
					    boxes, n, flags))
			goto fallback;

		DBG(("%s: flushing? %d\n", __FUNCTION__, sync));
		if (sync) { /* STAT! */
			struct kgem_request *rq = sna->kgem.next_request;
			kgem_submit(&sna->kgem);
			if (rq->bo) {
				bo = ref(rq->bo);
				DBG(("%s: recording sync fence handle=%d\n", __FUNCTION__, bo->handle));
			}
		}
	}

	DamageRegionProcessPending(&pixmap->drawable);

	if (clip.data)
		pixman_region_fini(&clip);

	return bo;
}

static void
sna_dri2_copy_region(DrawablePtr draw,
		     RegionPtr region,
		     DRI2BufferPtr dst,
		     DRI2BufferPtr src)
{
	PixmapPtr pixmap = get_drawable_pixmap(draw);
	struct sna *sna = to_sna_from_pixmap(pixmap);

	DBG(("%s: pixmap=%ld, src=%u (refs=%d/%d, flush=%d, attach=%d) , dst=%u (refs=%d/%d, flush=%d, attach=%d)\n",
	     __FUNCTION__,
	     pixmap->drawable.serialNumber,
	     get_private(src)->bo->handle,
	     get_private(src)->refcnt,
	     get_private(src)->bo->refcnt,
	     get_private(src)->bo->flush,
	     src->attachment,
	     get_private(dst)->bo->handle,
	     get_private(dst)->refcnt,
	     get_private(dst)->bo->refcnt,
	     get_private(dst)->bo->flush,
	     dst->attachment));

	assert(src != dst);

	assert(get_private(src)->refcnt);
	assert(get_private(dst)->refcnt);

	assert(get_private(src)->bo->refcnt);
	assert(get_private(dst)->bo->refcnt);

	DBG(("%s: region (%d, %d), (%d, %d) x %d\n",
	     __FUNCTION__,
	     region->extents.x1, region->extents.y1,
	     region->extents.x2, region->extents.y2,
	     region_num_rects(region)));

	__sna_dri2_copy_region(sna, draw, region, src, dst, false);
}

inline static uint32_t pipe_select(int pipe)
{
	/* The third pipe was introduced with IvyBridge long after
	 * multiple pipe support was added to the kernel, hence
	 * we can safely ignore the capability check - if we have more
	 * than two pipes, we can assume that they are fully supported.
	 */
	if (pipe > 1)
		return pipe << DRM_VBLANK_HIGH_CRTC_SHIFT;
	else if (pipe > 0)
		return DRM_VBLANK_SECONDARY;
	else
		return 0;
}

static inline int sna_wait_vblank(struct sna *sna, union drm_wait_vblank *vbl, int pipe)
{
	DBG(("%s(pipe=%d, waiting until seq=%u%s)\n",
	     __FUNCTION__, pipe, vbl->request.sequence,
	     vbl->request.type & DRM_VBLANK_RELATIVE ? " [relative]" : ""));
	assert(pipe != -1);

	vbl->request.type |= pipe_select(pipe);
	return drmIoctl(sna->kgem.fd, DRM_IOCTL_WAIT_VBLANK, vbl);
}

#if DRI2INFOREC_VERSION >= 4

static void dri2_window_attach(WindowPtr win, struct dri2_window *priv)
{
	assert(win->drawable.type == DRAWABLE_WINDOW);
	assert(dri2_window(win) == NULL);
	((void **)__get_private(win, sna_window_key))[1] = priv;
	assert(dri2_window(win) == priv);
}

static uint64_t
draw_current_msc(DrawablePtr draw, xf86CrtcPtr crtc, uint64_t msc)
{
	struct dri2_window *priv;

	if (draw->type != DRAWABLE_WINDOW)
		return msc;

	priv = dri2_window((WindowPtr)draw);
	if (priv == NULL) {
		priv = malloc(sizeof(*priv));
		if (priv != NULL) {
			priv->front = NULL;
			priv->crtc = crtc;
			priv->msc_delta = 0;
			priv->chain = NULL;
			dri2_window_attach((WindowPtr)draw, priv);
		}
	} else {
		if (priv->crtc != crtc) {
			const struct ust_msc *last = sna_crtc_last_swap(priv->crtc);
			const struct ust_msc *this = sna_crtc_last_swap(crtc);
			DBG(("%s: Window transferring from pipe=%d [msc=%llu] to pipe=%d [msc=%llu], delta now %lld\n",
			     __FUNCTION__,
			     sna_crtc_to_pipe(priv->crtc), (long long)last->msc,
			     sna_crtc_to_pipe(crtc), (long long)this->msc,
			     (long long)(priv->msc_delta + this->msc - last->msc)));
			priv->msc_delta += this->msc - last->msc;
			priv->crtc = crtc;
		}
		msc -= priv->msc_delta;
	}
	return  msc;
}

static uint32_t
draw_target_seq(DrawablePtr draw, uint64_t msc)
{
	struct dri2_window *priv = dri2_window((WindowPtr)draw);
	if (priv == NULL)
		return msc;
	DBG(("%s: converting target_msc=%llu to seq %u\n",
	     __FUNCTION__, (long long)msc, (unsigned)(msc + priv->msc_delta)));
	return msc + priv->msc_delta;
}

static xf86CrtcPtr
sna_dri2_get_crtc(DrawablePtr draw)
{
	struct sna *sna = to_sna_from_drawable(draw);
	BoxRec box;

	if (draw->type == DRAWABLE_PIXMAP)
		return NULL;

	box.x1 = draw->x;
	box.y1 = draw->y;
	box.x2 = box.x1 + draw->width;
	box.y2 = box.y1 + draw->height;

	/* Make sure the CRTC is valid and this is the real front buffer */
	return sna_covering_crtc(sna, &box, NULL);
}

static void
sna_dri2_remove_event(WindowPtr win, struct sna_dri2_event *info)
{
	struct dri2_window *priv;
	struct sna_dri2_event *chain;

	assert(win->drawable.type == DRAWABLE_WINDOW);
	DBG(("%s: remove[%p] from window %ld, active? %d\n",
	     __FUNCTION__, info, (long)win->drawable.id, info->draw != NULL));

	priv = dri2_window(win);
	assert(priv);
	assert(priv->chain != NULL);

	if (priv->chain == info) {
		priv->chain = info->chain;
		return;
	}

	chain = priv->chain;
	while (chain->chain != info)
		chain = chain->chain;
	assert(chain != info);
	assert(info->chain != chain);
	chain->chain = info->chain;
}

static void
sna_dri2_event_free(struct sna *sna,
		    struct sna_dri2_event *info)
{
	DrawablePtr draw = info->draw;

	DBG(("%s(draw?=%d)\n", __FUNCTION__, draw != NULL));
	if (draw && draw->type == DRAWABLE_WINDOW)
		sna_dri2_remove_event((WindowPtr)draw, info);

	_sna_dri2_destroy_buffer(sna, info->front);
	_sna_dri2_destroy_buffer(sna, info->back);

	while (!list_is_empty(&info->cache)) {
		struct dri_bo *c;

		c = list_first_entry(&info->cache, struct dri_bo, link);
		list_del(&c->link);

		DBG(("%s: releasing cached handle=%d\n", __FUNCTION__, c->bo ? c->bo->handle : 0));
		if (c->bo)
			kgem_bo_destroy(&sna->kgem, c->bo);

		free(c);
	}

	if (info->bo) {
		DBG(("%s: releasing batch handle=%d\n", __FUNCTION__, info->bo->handle));
		kgem_bo_destroy(&sna->kgem, info->bo);
	}

	_list_del(&info->link);
	free(info);
}

static void
sna_dri2_client_gone(CallbackListPtr *list, void *closure, void *data)
{
	NewClientInfoRec *clientinfo = data;
	ClientPtr client = clientinfo->client;
	struct sna_client *priv = sna_client(client);
	struct sna *sna = closure;

	if (priv->events.next == NULL)
		return;

	if (client->clientState != ClientStateGone)
		return;

	DBG(("%s(active?=%d)\n", __FUNCTION__,
	     !list_is_empty(&priv->events)));

	while (!list_is_empty(&priv->events)) {
		struct sna_dri2_event *event;

		event = list_first_entry(&priv->events, struct sna_dri2_event, link);
		assert(event->client == client);

		if (event->queued) {
			if (event->draw)
				sna_dri2_remove_event((WindowPtr)event->draw,
						      event);
			event->client = NULL;
			event->draw = NULL;
			list_del(&event->link);
		} else
			sna_dri2_event_free(sna, event);
	}

	if (--sna->dri2.client_count == 0)
		DeleteCallback(&ClientStateCallback, sna_dri2_client_gone, sna);
}

static bool add_event_to_client(struct sna_dri2_event *info, struct sna *sna, ClientPtr client)
{
	struct sna_client *priv = sna_client(client);

	if (priv->events.next == NULL) {
		if (sna->dri2.client_count++ == 0 &&
		    !AddCallback(&ClientStateCallback, sna_dri2_client_gone, sna))
			return false;

		list_init(&priv->events);
	}

	list_add(&info->link, &priv->events);
	info->client = client;
	return true;
}

static struct sna_dri2_event *
sna_dri2_add_event(struct sna *sna, DrawablePtr draw, ClientPtr client)
{
	struct dri2_window *priv;
	struct sna_dri2_event *info, *chain;

	assert(draw->type == DRAWABLE_WINDOW);
	DBG(("%s: adding event to window %ld)\n",
	     __FUNCTION__, (long)draw->id));

	priv = dri2_window((WindowPtr)draw);
	if (priv == NULL)
		return NULL;

	info = calloc(1, sizeof(struct sna_dri2_event));
	if (info == NULL)
		return NULL;

	list_init(&info->cache);
	info->draw = draw;
	info->crtc = priv->crtc;
	info->pipe = sna_crtc_to_pipe(priv->crtc);

	if (!add_event_to_client(info, sna, client)) {
		free(info);
		return NULL;
	}

	assert(priv->chain != info);

	if (priv->chain == NULL) {
		priv->chain = info;
		return info;
	}

	chain = priv->chain;
	while (chain->chain != NULL)
		chain = chain->chain;

	assert(chain != info);
	chain->chain = info;
	return info;
}

void sna_dri2_destroy_window(WindowPtr win)
{
	struct sna *sna;
	struct dri2_window *priv;

	priv = dri2_window(win);
	if (priv == NULL)
		return;

	DBG(("%s: window=%ld\n", __FUNCTION__, win->drawable.serialNumber));
	sna = to_sna_from_drawable(&win->drawable);

	if (priv->front) {
		assert(priv->crtc);
		sna_shadow_unset_crtc(sna, priv->crtc);
		_sna_dri2_destroy_buffer(sna, priv->front);
	}

	if (priv->chain) {
		struct sna_dri2_event *info, *chain;

		DBG(("%s: freeing chain\n", __FUNCTION__));

		chain = priv->chain;
		while ((info = chain)) {
			info->draw = NULL;
			info->client = NULL;

			chain = info->chain;
			info->chain = NULL;

			if (!info->queued)
				sna_dri2_event_free(sna, info);
		}
	}

	free(priv);
}

static void
sna_dri2_flip_handler(struct sna *sna,
		      struct drm_event_vblank *event,
		      void *data)
{
	DBG(("%s: sequence=%d\n", __FUNCTION__, event->sequence));
	sna_dri2_flip_event(sna, data);
}

static bool
sna_dri2_flip(struct sna *sna, struct sna_dri2_event *info)
{
	struct kgem_bo *bo = get_private(info->back)->bo;
	struct kgem_bo *tmp_bo;
	uint32_t tmp_name;

	DBG(("%s(type=%d)\n", __FUNCTION__, info->type));

	assert(sna_pixmap_get_buffer(sna->front) == info->front);
	assert(get_drawable_pixmap(info->draw)->drawable.height * bo->pitch <= kgem_bo_size(bo));
	assert(bo->refcnt);

	if (!sna_page_flip(sna, bo, sna_dri2_flip_handler,
			   info->type == FLIP_ASYNC ? NULL : info))
		return false;

	assert(sna->dri2.flip_pending == NULL || sna->dri2.flip_pending == info);
	if (info->type != FLIP_ASYNC)
		sna->dri2.flip_pending = info;

	DBG(("%s: marked handle=%d as scanout, swap front (handle=%d, name=%d) and back (handle=%d, name=%d)\n",
	     __FUNCTION__, bo->handle,
	     get_private(info->front)->bo->handle, info->front->name,
	     get_private(info->back)->bo->handle, info->back->name));

	tmp_bo = get_private(info->front)->bo;
	tmp_name = info->front->name;

	set_bo(sna->front, bo);

	info->front->name = info->back->name;
	get_private(info->front)->bo = bo;

	info->back->name = tmp_name;
	get_private(info->back)->bo = tmp_bo;
	get_private(info->back)->stale = true;

	assert(get_private(info->front)->bo->refcnt);
	assert(get_private(info->back)->bo->refcnt);
	assert(get_private(info->front)->bo != get_private(info->back)->bo);

	info->queued = true;
	return true;
}

static bool
can_flip(struct sna * sna,
	 DrawablePtr draw,
	 DRI2BufferPtr front,
	 DRI2BufferPtr back,
	 xf86CrtcPtr crtc)
{
	WindowPtr win = (WindowPtr)draw;
	PixmapPtr pixmap;

	assert((sna->flags & SNA_NO_WAIT) == 0);

	if (!DBG_CAN_FLIP)
		return false;

	if (draw->type == DRAWABLE_PIXMAP)
		return false;

	if (!sna->mode.front_active) {
		DBG(("%s: no, active CRTC\n", __FUNCTION__));
		return false;
	}

	assert(sna->scrn->vtSema);

	if ((sna->flags & (SNA_HAS_FLIP | SNA_HAS_ASYNC_FLIP)) == 0) {
		DBG(("%s: no, pageflips disabled\n", __FUNCTION__));
		return false;
	}

	if (front->format != back->format) {
		DBG(("%s: no, format mismatch, front = %d, back = %d\n",
		     __FUNCTION__, front->format, back->format));
		return false;
	}

	if (sna->mode.shadow_active) {
		DBG(("%s: no, shadow enabled\n", __FUNCTION__));
		return false;
	}

	if (!sna_crtc_is_on(crtc)) {
		DBG(("%s: ref-pipe=%d is disabled\n", __FUNCTION__, sna_crtc_to_pipe(crtc)));
		return false;
	}

	pixmap = get_window_pixmap(win);
	if (pixmap != sna->front) {
		DBG(("%s: no, window (pixmap=%ld) is not attached to the front buffer (pixmap=%ld)\n",
		     __FUNCTION__, pixmap->drawable.serialNumber, sna->front->drawable.serialNumber));
		return false;
	}

	if (sna_pixmap_get_buffer(pixmap) != front) {
		DBG(("%s: no, DRI2 drawable is no longer attached (old name=%d, new name=%d) to pixmap=%ld\n",
		     __FUNCTION__, front->name,
		     sna_pixmap_get_buffer(pixmap) ? ((DRI2BufferPtr)sna_pixmap_get_buffer(pixmap))->name : 0,
		     pixmap->drawable.serialNumber));
		return false;
	}

	assert(get_private(front)->pixmap == sna->front);
	assert(sna_pixmap(sna->front)->gpu_bo == get_private(front)->bo);

	if (!get_private(back)->bo->scanout) {
		DBG(("%s: no, DRI2 drawable was too small at time of creation)\n",
		     __FUNCTION__));
		return false;
	}

	if (get_private(back)->size != get_private(front)->size) {
		DBG(("%s: no, DRI2 drawable does not fit into scanout\n",
		     __FUNCTION__));
		return false;
	}

	DBG(("%s: window size: %dx%d, clip=(%d, %d), (%d, %d) x %d\n",
	     __FUNCTION__,
	     win->drawable.width, win->drawable.height,
	     win->clipList.extents.x1, win->clipList.extents.y1,
	     win->clipList.extents.x2, win->clipList.extents.y2,
	     region_num_rects(&win->clipList)));
	if (!RegionEqual(&win->clipList, &draw->pScreen->root->winSize)) {
		DBG(("%s: no, window is clipped: clip region=(%d, %d), (%d, %d), root size=(%d, %d), (%d, %d)\n",
		     __FUNCTION__,
		     win->clipList.extents.x1,
		     win->clipList.extents.y1,
		     win->clipList.extents.x2,
		     win->clipList.extents.y2,
		     draw->pScreen->root->winSize.extents.x1,
		     draw->pScreen->root->winSize.extents.y1,
		     draw->pScreen->root->winSize.extents.x2,
		     draw->pScreen->root->winSize.extents.y2));
		return false;
	}

	if (draw->x != 0 || draw->y != 0 ||
#ifdef COMPOSITE
	    draw->x != pixmap->screen_x ||
	    draw->y != pixmap->screen_y ||
#endif
	    draw->width != pixmap->drawable.width ||
	    draw->height != pixmap->drawable.height) {
		DBG(("%s: no, window is not full size (%dx%d)!=(%dx%d)\n",
		     __FUNCTION__,
		     draw->width, draw->height,
		     pixmap->drawable.width,
		     pixmap->drawable.height));
		return false;
	}

	/* prevent an implicit tiling mode change */
	if (get_private(back)->bo->tiling > I915_TILING_X) {
		DBG(("%s -- no, tiling mismatch: front %d, back=%d, want-tiled?=%d\n",
		     __FUNCTION__,
		     get_private(front)->bo->tiling,
		     get_private(back)->bo->tiling,
		     !!(sna->flags & SNA_LINEAR_FB)));
		return false;
	}

	if (get_private(front)->bo->pitch != get_private(back)->bo->pitch) {
		DBG(("%s -- no, pitch mismatch: front %d, back=%d\n",
		     __FUNCTION__,
		     get_private(front)->bo->pitch,
		     get_private(back)->bo->pitch));
		return false;
	}

	if (sna_pixmap(pixmap)->pinned & ~(PIN_DRI2 | PIN_SCANOUT)) {
		DBG(("%s -- no, pinned: front %x\n",
		     __FUNCTION__, sna_pixmap(pixmap)->pinned));
		return false;
	}

	DBG(("%s: yes, pixmap=%ld\n", __FUNCTION__, pixmap->drawable.serialNumber));
	assert(dri2_window(win)->front == NULL);
	return true;
}

static bool
can_xchg(struct sna * sna,
	 DrawablePtr draw,
	 DRI2BufferPtr front,
	 DRI2BufferPtr back)
{
	WindowPtr win = (WindowPtr)draw;
	PixmapPtr pixmap;

	if (!DBG_CAN_XCHG)
		return false;

	if (draw->type == DRAWABLE_PIXMAP)
		return false;

	if (front->format != back->format) {
		DBG(("%s: no, format mismatch, front = %d, back = %d\n",
		     __FUNCTION__, front->format, back->format));
		return false;
	}

	pixmap = get_window_pixmap(win);
	if (get_private(front)->pixmap != pixmap) {
		DBG(("%s: no, DRI2 drawable is no longer attached, old pixmap=%ld, now pixmap=%ld\n",
		     __FUNCTION__,
		     get_private(front)->pixmap->drawable.serialNumber,
		     pixmap->drawable.serialNumber));
		return false;
	}

	DBG(("%s: window size: %dx%d, clip=(%d, %d), (%d, %d) x %d, pixmap size=%dx%d\n",
	     __FUNCTION__,
	     win->drawable.width, win->drawable.height,
	     win->clipList.extents.x1, win->clipList.extents.y1,
	     win->clipList.extents.x2, win->clipList.extents.y2,
	     region_num_rects(&win->clipList),
	     pixmap->drawable.width,
	     pixmap->drawable.height));
	if (is_clipped(&win->clipList, &pixmap->drawable)) {
		DBG(("%s: no, %dx%d window is clipped: clip region=(%d, %d), (%d, %d)\n",
		     __FUNCTION__,
		     draw->width, draw->height,
		     win->clipList.extents.x1,
		     win->clipList.extents.y1,
		     win->clipList.extents.x2,
		     win->clipList.extents.y2));
		return false;
	}

	if (get_private(back)->size != get_private(front)->size) {
		DBG(("%s: no, back buffer %dx%d does not match front buffer %dx%d\n",
		     __FUNCTION__,
		     get_private(back)->size & 0x7fff, (get_private(back)->size >> 16) & 0x7fff,
		     get_private(front)->size & 0x7fff, (get_private(front)->size >> 16) & 0x7fff));
		return false;
	}

	if (pixmap == sna->front && !(sna->flags & SNA_TEAR_FREE) && sna->mode.front_active) {
		DBG(("%s: no, front buffer, requires flipping\n",
		     __FUNCTION__));
		return false;
	}

	if (sna_pixmap(pixmap)->pinned & ~(PIN_DRI2 | PIN_SCANOUT)) {
		DBG(("%s: no, pinned: %x\n",
		     __FUNCTION__, sna_pixmap(pixmap)->pinned));
		return false;
	}

	DBG(("%s: yes, pixmap=%ld\n", __FUNCTION__, pixmap->drawable.serialNumber));
	return true;
}

static bool
overlaps_other_crtc(struct sna *sna, xf86CrtcPtr desired)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(sna->scrn);
	int c;

	for (c = 0; c < sna->mode.num_real_crtc; c++) {
		xf86CrtcPtr crtc = config->crtc[c];

		if (crtc == desired)
			continue;

		if (!crtc->enabled)
			continue;

		if (desired->bounds.x1 < crtc->bounds.x2 &&
		    desired->bounds.x2 > crtc->bounds.x1 &&
		    desired->bounds.y1 < crtc->bounds.y2 &&
		    desired->bounds.y2 > crtc->bounds.y1)
			return true;
	}

	return false;
}

static bool
can_xchg_crtc(struct sna *sna,
	      DrawablePtr draw,
	      DRI2BufferPtr front,
	      DRI2BufferPtr back,
	      xf86CrtcPtr crtc)
{
	WindowPtr win = (WindowPtr)draw;
	PixmapPtr pixmap;

	if (!DBG_CAN_XCHG)
		return false;

	if ((sna->flags & SNA_TEAR_FREE) == 0) {
		DBG(("%s: no, requires TearFree\n",
		     __FUNCTION__));
		return false;
	}

	if (draw->type == DRAWABLE_PIXMAP)
		return false;

	if (front->format != back->format) {
		DBG(("%s: no, format mismatch, front = %d, back = %d\n",
		     __FUNCTION__, front->format, back->format));
		return false;
	}

	if (memcmp(&win->clipList.extents, &crtc->bounds, sizeof(crtc->bounds))) {
		DBG(("%s: no, window [(%d, %d), (%d, %d)] does not cover CRTC [(%d, %d), (%d, %d)]\n",
		     __FUNCTION__,
		     win->clipList.extents.x1, win->clipList.extents.y1,
		     win->clipList.extents.x2, win->clipList.extents.y2,
		     crtc->bounds.x1, crtc->bounds.y1,
		     crtc->bounds.x2, crtc->bounds.y2));
		return false;
	}

	if (sna_crtc_is_transformed(crtc)) {
		DBG(("%s: no, CRTC is rotated\n", __FUNCTION__));
		return false;
	}

	pixmap = get_window_pixmap(win);
	if (pixmap != sna->front) {
		DBG(("%s: no, not attached to front buffer\n", __FUNCTION__));
		return false;
	}

	if (get_private(front)->pixmap != pixmap) {
		DBG(("%s: no, DRI2 drawable is no longer attached, old pixmap=%ld, now pixmap=%ld\n",
		     __FUNCTION__,
		     get_private(front)->pixmap->drawable.serialNumber,
		     pixmap->drawable.serialNumber));
		return false;
	}

	DBG(("%s: window size: %dx%d, clip=(%d, %d), (%d, %d) x %d\n",
	     __FUNCTION__,
	     win->drawable.width, win->drawable.height,
	     win->clipList.extents.x1, win->clipList.extents.y1,
	     win->clipList.extents.x2, win->clipList.extents.y2,
	     region_num_rects(&win->clipList)));
	if (is_clipped(&win->clipList, &win->drawable)) {
		DBG(("%s: no, %dx%d window is clipped: clip region=(%d, %d), (%d, %d)\n",
		     __FUNCTION__,
		     draw->width, draw->height,
		     win->clipList.extents.x1,
		     win->clipList.extents.y1,
		     win->clipList.extents.x2,
		     win->clipList.extents.y2));
		return false;
	}

	if (overlaps_other_crtc(sna, crtc)) {
		DBG(("%s: no, overlaps other CRTC\n", __FUNCTION__));
		return false;
	}

	if (get_private(back)->size != (draw->height << 16 | draw->width)) {
		DBG(("%s: no, DRI2 buffers does not fit window\n",
		     __FUNCTION__));
		return false;
	}

	assert(win != win->drawable.pScreen->root);
	DBG(("%s: yes, pixmap=%ld\n", __FUNCTION__, pixmap->drawable.serialNumber));
	return true;
}

static void
sna_dri2_xchg(DrawablePtr draw, DRI2BufferPtr front, DRI2BufferPtr back)
{
	WindowPtr win = (WindowPtr)draw;
	struct kgem_bo *back_bo, *front_bo;
	PixmapPtr pixmap;
	int tmp;

	assert(draw->type != DRAWABLE_PIXMAP);
	pixmap = get_window_pixmap(win);

	back_bo = get_private(back)->bo;
	front_bo = get_private(front)->bo;
	assert(front_bo != back_bo);

	DBG(("%s: win=%ld, exchange front=%d/%d and back=%d/%d, pixmap=%ld %dx%d\n",
	     __FUNCTION__, win->drawable.id,
	     front_bo->handle, front->name,
	     back_bo->handle, back->name,
	     pixmap->drawable.serialNumber,
	     pixmap->drawable.width,
	     pixmap->drawable.height));

	DBG(("%s: back_bo pitch=%d, size=%d, ref=%d, active_scanout?=%d\n",
	     __FUNCTION__, back_bo->pitch, kgem_bo_size(back_bo), back_bo->refcnt, back_bo->active_scanout));
	DBG(("%s: front_bo pitch=%d, size=%d, ref=%d, active_scanout?=%d\n",
	     __FUNCTION__, front_bo->pitch, kgem_bo_size(front_bo), front_bo->refcnt, front_bo->active_scanout));
	assert(front_bo->refcnt);
	assert(back_bo->refcnt);

	assert(sna_pixmap_get_buffer(pixmap) == front);

	assert(pixmap->drawable.height * back_bo->pitch <= kgem_bo_size(back_bo));
	assert(pixmap->drawable.height * front_bo->pitch <= kgem_bo_size(front_bo));

	set_bo(pixmap, back_bo);

	get_private(front)->bo = back_bo;
	get_private(back)->bo = front_bo;
	get_private(back)->stale = true;

	tmp = front->name;
	front->name = back->name;
	back->name = tmp;

	assert(front_bo->refcnt);
	assert(back_bo->refcnt);

	assert(get_private(front)->bo == sna_pixmap(pixmap)->gpu_bo);
}

static void sna_dri2_xchg_crtc(struct sna *sna, DrawablePtr draw, xf86CrtcPtr crtc, DRI2BufferPtr front, DRI2BufferPtr back)
{
	WindowPtr win = (WindowPtr)draw;
	DRI2Buffer2Ptr tmp;
	struct kgem_bo *bo;

	DBG(("%s: exchange front=%d/%d and back=%d/%d, win id=%lu, pixmap=%ld %dx%d\n",
	     __FUNCTION__,
	     get_private(front)->bo->handle, front->name,
	     get_private(back)->bo->handle, back->name,
	     win->drawable.id,
	     get_window_pixmap(win)->drawable.serialNumber,
	     get_window_pixmap(win)->drawable.width,
	     get_window_pixmap(win)->drawable.height));

	DamageRegionAppend(&win->drawable, &win->clipList);
	sna_shadow_set_crtc(sna, crtc, get_private(back)->bo);
	DamageRegionProcessPending(&win->drawable);

	assert(dri2_window(win)->front == NULL);

	tmp = calloc(1, sizeof(*tmp) + sizeof(struct sna_dri2_private));
	if (tmp == NULL) {
		back->attachment = -1;
		if (get_private(back)->proxy == NULL) {
			get_private(back)->pixmap = get_window_pixmap(win);
			get_private(back)->proxy = sna_dri2_reference_buffer(sna_pixmap_get_buffer(get_private(back)->pixmap));
		}
		dri2_window(win)->front = sna_dri2_reference_buffer(back);
		return;
	}

	*tmp = *back;
	tmp->attachment = DRI2BufferFrontLeft;
	tmp->driverPrivate = tmp + 1;
	get_private(tmp)->refcnt = 1;
	get_private(tmp)->bo = get_private(back)->bo;
	get_private(tmp)->size = get_private(back)->size;
	get_private(tmp)->pixmap = get_window_pixmap(win);
	get_private(tmp)->proxy = sna_dri2_reference_buffer(sna_pixmap_get_buffer(get_private(tmp)->pixmap));
	dri2_window(win)->front = tmp;

	DBG(("%s: allocating new backbuffer\n", __FUNCTION__));
	back->name = 0;
	bo = kgem_create_2d(&sna->kgem,
			    draw->width, draw->height, draw->bitsPerPixel,
			    get_private(back)->bo->tiling,
			    CREATE_SCANOUT);
	if (bo != NULL) {
		get_private(back)->bo = bo;
		back->pitch = bo->pitch;
		back->name = kgem_bo_flink(&sna->kgem, bo);
	}
	if (back->name == 0) {
		if (bo != NULL)
			kgem_bo_destroy(&sna->kgem, bo);
		get_private(back)->bo = NULL;
		back->attachment = -1;
	}
}

static void frame_swap_complete(struct sna *sna,
				struct sna_dri2_event *frame,
				int type)
{
	const struct ust_msc *swap;

	if (frame->draw == NULL)
		return;

	assert(frame->client);

	swap = sna_crtc_last_swap(frame->crtc);
	DBG(("%s: draw=%ld, pipe=%d, frame=%lld [msc=%lld], tv=%d.%06d\n",
	     __FUNCTION__, (long)frame->draw, frame->pipe,
	     (long long)swap->msc,
	     (long long)draw_current_msc(frame->draw, frame->crtc, swap->msc),
	     swap->tv_sec, swap->tv_usec));

	DRI2SwapComplete(frame->client, frame->draw,
			 draw_current_msc(frame->draw, frame->crtc, swap->msc),
			 swap->tv_sec, swap->tv_usec,
			 type, frame->event_complete, frame->event_data);
}

static void fake_swap_complete(struct sna *sna, ClientPtr client,
			       DrawablePtr draw, xf86CrtcPtr crtc,
			       int type, DRI2SwapEventPtr func, void *data)
{
	const struct ust_msc *swap;

	swap = sna_crtc_last_swap(crtc);
	DBG(("%s: draw=%ld, pipe=%d, frame=%lld [msc %lld], tv=%d.%06d\n",
	     __FUNCTION__, (long)draw->id, crtc ? sna_crtc_to_pipe(crtc) : -1,
	     (long long)swap->msc,
	     (long long)draw_current_msc(draw, crtc, swap->msc),
	     swap->tv_sec, swap->tv_usec));

	DRI2SwapComplete(client, draw,
			 draw_current_msc(draw, crtc, swap->msc),
			 swap->tv_sec, swap->tv_usec,
			 type, func, data);
}

static void chain_swap(struct sna *sna, struct sna_dri2_event *chain)
{
	union drm_wait_vblank vbl;

	if (chain->draw == NULL) {
		sna_dri2_event_free(sna, chain);
		return;
	}

	if (chain->queued) /* too early! */
		return;

	assert(chain == dri2_chain(chain->draw));
	DBG(("%s: chaining draw=%ld, type=%d\n",
	     __FUNCTION__, (long)chain->draw->id, chain->type));
	chain->queued = true;

	switch (chain->type) {
	case SWAP_THROTTLE:
		DBG(("%s: emitting chained vsync'ed blit\n", __FUNCTION__));
		if (sna->mode.shadow && !sna->mode.shadow_damage) {
			/* recursed from wait_for_shadow(), simply requeue */
			DBG(("%s -- recursed from wait_for_shadow(), requeuing\n", __FUNCTION__));
			VG_CLEAR(vbl);
			vbl.request.type =
				DRM_VBLANK_RELATIVE |
				DRM_VBLANK_EVENT;
			vbl.request.sequence = 1;
			vbl.request.signal = (uintptr_t)chain;

			if (!sna_wait_vblank(sna, &vbl, chain->pipe))
				return;

			DBG(("%s -- requeue failed, errno=%d\n", __FUNCTION__, errno));
		}

		if (can_xchg(sna, chain->draw, chain->front, chain->back)) {
			sna_dri2_xchg(chain->draw, chain->front, chain->back);
		} else if (can_xchg_crtc(sna, chain->draw, chain->front, chain->back, chain->crtc)) {
			sna_dri2_xchg_crtc(sna, chain->draw, chain->crtc, chain->front, chain->back);
		} else {
			assert(chain->queued);
			chain->bo = __sna_dri2_copy_region(sna, chain->draw, NULL,
							   chain->back, chain->front,
							   true);
		}
	case SWAP:
		break;
	default:
		return;
	}

	VG_CLEAR(vbl);
	vbl.request.type =
		DRM_VBLANK_RELATIVE |
		DRM_VBLANK_EVENT;
	vbl.request.sequence = 1;
	vbl.request.signal = (uintptr_t)chain;
	if (sna_wait_vblank(sna, &vbl, chain->pipe)) {
		DBG(("%s: vblank wait failed, unblocking client\n", __FUNCTION__));
		frame_swap_complete(sna, chain, DRI2_BLIT_COMPLETE);
		sna_dri2_event_free(sna, chain);
	} else {
		if (chain->type == SWAP_THROTTLE && !swap_limit(chain->draw, 2)) {
			DBG(("%s: fake triple buffering, unblocking client\n", __FUNCTION__));
			frame_swap_complete(sna, chain, DRI2_BLIT_COMPLETE);
		}
	}
}

static inline bool rq_is_busy(struct kgem *kgem, struct kgem_bo *bo)
{
	if (bo == NULL)
		return false;

	DBG(("%s: handle=%d, domain: %d exec? %d, rq? %d\n", __FUNCTION__,
	     bo->handle, bo->domain, bo->exec != NULL, bo->rq != NULL));
	assert(bo->refcnt);

	if (bo->exec)
		return true;

	if (bo->rq == NULL)
		return false;

	return __kgem_busy(kgem, bo->handle);
}

static bool sna_dri2_blit_complete(struct sna *sna,
				   struct sna_dri2_event *info)
{
	if (rq_is_busy(&sna->kgem, info->bo)) {
		union drm_wait_vblank vbl;

		DBG(("%s: vsync'ed blit is still busy, postponing\n",
		     __FUNCTION__));

		VG_CLEAR(vbl);
		vbl.request.type =
			DRM_VBLANK_RELATIVE |
			DRM_VBLANK_EVENT;
		vbl.request.sequence = 1;
		vbl.request.signal = (uintptr_t)info;
		assert(info->queued);
		if (!sna_wait_vblank(sna, &vbl, info->pipe))
			return false;
	}

	DBG(("%s: blit finished\n", __FUNCTION__));
	return true;
}

void sna_dri2_vblank_handler(struct sna *sna, struct drm_event_vblank *event)
{
	struct sna_dri2_event *info = (void *)(uintptr_t)event->user_data;
	DrawablePtr draw;
	union drm_wait_vblank vbl;
	uint64_t msc;

	DBG(("%s(type=%d, sequence=%d)\n", __FUNCTION__, info->type, event->sequence));
	assert(info->queued);
	msc = sna_crtc_record_event(info->crtc, event);

	draw = info->draw;
	if (draw == NULL) {
		DBG(("%s -- drawable gone\n", __FUNCTION__));
		goto done;
	}

	switch (info->type) {
	case FLIP:
		/* If we can still flip... */
		if (can_flip(sna, draw, info->front, info->back, info->crtc) &&
		    sna_dri2_flip(sna, info))
			return;

		/* else fall through to blit */
	case SWAP:
		assert(info->queued);
		if (sna->mode.shadow && !sna->mode.shadow_damage) {
			/* recursed from wait_for_shadow(), simply requeue */
			DBG(("%s -- recursed from wait_for_shadow(), requeuing\n", __FUNCTION__));

		} else if (can_xchg(sna, draw, info->front, info->back)) {
			sna_dri2_xchg(draw, info->front, info->back);
			info->type = SWAP_WAIT;
		} else if (can_xchg_crtc(sna, draw, info->front, info->back, info->crtc)) {
			sna_dri2_xchg_crtc(sna, draw, info->crtc, info->front, info->back);
			info->type = SWAP_WAIT;
		}  else {
			assert(info->queued);
			info->bo = __sna_dri2_copy_region(sna, draw, NULL,
							  info->back, info->front, true);
			info->type = SWAP_WAIT;
		}

		VG_CLEAR(vbl);
		vbl.request.type =
			DRM_VBLANK_RELATIVE |
			DRM_VBLANK_EVENT;
		vbl.request.sequence = 1;
		vbl.request.signal = (uintptr_t)info;

		assert(info->queued);
		if (!sna_wait_vblank(sna, &vbl, info->pipe))
			return;

		DBG(("%s -- requeue failed, errno=%d\n", __FUNCTION__, errno));
		/* fall through to SwapComplete */
	case SWAP_WAIT:
		if (!sna_dri2_blit_complete(sna, info))
			return;

		DBG(("%s: swap complete, unblocking client (frame=%d, tv=%d.%06d)\n", __FUNCTION__,
		     event->sequence, event->tv_sec, event->tv_usec));
		frame_swap_complete(sna, info, DRI2_BLIT_COMPLETE);
		break;

	case SWAP_THROTTLE:
		DBG(("%s: %d complete, frame=%d tv=%d.%06d\n",
		     __FUNCTION__, info->type,
		     event->sequence, event->tv_sec, event->tv_usec));

		if (xorg_can_triple_buffer(sna)) {
			if (!sna_dri2_blit_complete(sna, info))
				return;

			DBG(("%s: triple buffer swap complete, unblocking client (frame=%d, tv=%d.%06d)\n", __FUNCTION__,
			     event->sequence, event->tv_sec, event->tv_usec));
			frame_swap_complete(sna, info, DRI2_BLIT_COMPLETE);
		}
		break;

	case WAITMSC:
		assert(info->client);
		DRI2WaitMSCComplete(info->client, draw, msc,
				    event->tv_sec, event->tv_usec);
		break;
	default:
		xf86DrvMsg(sna->scrn->scrnIndex, X_WARNING,
			   "%s: unknown vblank event received\n", __func__);
		/* Unknown type */
		break;
	}

	if (info->chain) {
		assert(info->chain != info);
		assert(info->draw == draw);
		sna_dri2_remove_event((WindowPtr)draw, info);
		chain_swap(sna, info->chain);
		info->draw = NULL;
	}

done:
	sna_dri2_event_free(sna, info);
	DBG(("%s complete\n", __FUNCTION__));
}

static bool
sna_dri2_immediate_blit(struct sna *sna,
			struct sna_dri2_event *info,
			bool sync, bool event)
{
	DrawablePtr draw = info->draw;
	bool ret = false;

	if (sna->flags & SNA_NO_WAIT)
		sync = false;

	DBG(("%s: emitting immediate blit, throttling client, synced? %d, chained? %d, send-event? %d\n",
	     __FUNCTION__, sync, dri2_chain(draw) != info,
	     event));

	info->type = SWAP_THROTTLE;
	if (!sync || dri2_chain(draw) == info) {
		DBG(("%s: no pending blit, starting chain\n",
		     __FUNCTION__));

		info->queued = true;
		info->bo = __sna_dri2_copy_region(sna, draw, NULL,
						  info->back,
						  info->front,
						  sync);
		if (event) {
			if (sync) {
				union drm_wait_vblank vbl;

				VG_CLEAR(vbl);
				vbl.request.type =
					DRM_VBLANK_RELATIVE |
					DRM_VBLANK_EVENT;
				vbl.request.sequence = 1;
				vbl.request.signal = (uintptr_t)info;
				ret = !sna_wait_vblank(sna, &vbl, info->pipe);
				if (ret)
					event = !swap_limit(draw, 2);
			}
			if (event) {
				DBG(("%s: fake triple buffering, unblocking client\n", __FUNCTION__));
				frame_swap_complete(sna, info, DRI2_BLIT_COMPLETE);
			}
		}
	} else {
		DBG(("%s: pending blit, chained\n", __FUNCTION__));
		ret = true;
	}

	DBG(("%s: continue? %d\n", __FUNCTION__, ret));
	return ret;
}

static bool
sna_dri2_flip_continue(struct sna *sna, struct sna_dri2_event *info)
{
	DBG(("%s(mode=%d)\n", __FUNCTION__, info->mode));

	if (info->mode > 0){
		struct kgem_bo *bo = get_private(info->front)->bo;

		info->type = info->mode;

		if (bo != sna_pixmap(sna->front)->gpu_bo)
			return false;

		if (!sna_page_flip(sna, bo, sna_dri2_flip_handler, info))
			return false;

		assert(sna->dri2.flip_pending == NULL || sna->dri2.flip_pending == info);
		sna->dri2.flip_pending = info;
		assert(info->queued);
	} else {
		info->type = -info->mode;

		if (!info->draw)
			return false;

		if (!can_flip(sna, info->draw, info->front, info->back, info->crtc))
			return false;

		assert(sna_pixmap_get_buffer(get_drawable_pixmap(info->draw)) == info->front);
		if (!sna_dri2_flip(sna, info))
			return false;

		if (!xorg_can_triple_buffer(sna)) {
			sna_dri2_get_back(sna, info->draw, info->back, info);
			DBG(("%s: fake triple buffering, unblocking client\n", __FUNCTION__));
			frame_swap_complete(sna, info, DRI2_FLIP_COMPLETE);
		}
	}

	info->mode = 0;
	return true;
}

static void chain_flip(struct sna *sna)
{
	struct sna_dri2_event *chain = sna->dri2.flip_pending;

	assert(chain->type == FLIP);
	DBG(("%s: chaining type=%d, cancelled?=%d\n",
	     __FUNCTION__, chain->type, chain->draw == NULL));

	sna->dri2.flip_pending = NULL;
	if (chain->draw == NULL) {
		sna_dri2_event_free(sna, chain);
		return;
	}

	assert(chain == dri2_chain(chain->draw));
	assert(!chain->queued);
	chain->queued = true;

	if (can_flip(sna, chain->draw, chain->front, chain->back, chain->crtc) &&
	    sna_dri2_flip(sna, chain)) {
		DBG(("%s: performing chained flip\n", __FUNCTION__));
	} else {
		DBG(("%s: emitting chained vsync'ed blit\n", __FUNCTION__));
		chain->bo = __sna_dri2_copy_region(sna, chain->draw, NULL,
						  chain->back, chain->front,
						  true);

		if (xorg_can_triple_buffer(sna)) {
			union drm_wait_vblank vbl;

			VG_CLEAR(vbl);

			chain->type = SWAP_WAIT;
			vbl.request.type =
				DRM_VBLANK_RELATIVE |
				DRM_VBLANK_EVENT;
			vbl.request.sequence = 1;
			vbl.request.signal = (uintptr_t)chain;

			assert(chain->queued);
			if (!sna_wait_vblank(sna, &vbl, chain->pipe))
				return;
		}

		DBG(("%s: fake triple buffering (or vblank wait failed), unblocking client\n", __FUNCTION__));
		frame_swap_complete(sna, chain, DRI2_BLIT_COMPLETE);
		sna_dri2_event_free(sna, chain);
	}
}

static void sna_dri2_flip_event(struct sna *sna,
				struct sna_dri2_event *flip)
{
	DBG(("%s(pipe=%d, event=%d)\n", __FUNCTION__, flip->pipe, flip->type));
	assert(flip->queued);

	if (sna->dri2.flip_pending == flip)
		sna->dri2.flip_pending = NULL;

	/* We assume our flips arrive in order, so we don't check the frame */
	switch (flip->type) {
	case FLIP:
		DBG(("%s: swap complete, unblocking client\n", __FUNCTION__));
		frame_swap_complete(sna, flip, DRI2_FLIP_COMPLETE);
		sna_dri2_event_free(sna, flip);

		if (sna->dri2.flip_pending)
			chain_flip(sna);
		break;

	case FLIP_THROTTLE:
		DBG(("%s: triple buffer swap complete, unblocking client\n", __FUNCTION__));
		frame_swap_complete(sna, flip, DRI2_FLIP_COMPLETE);
	case FLIP_COMPLETE:
		if (sna->dri2.flip_pending) {
			sna_dri2_event_free(sna, flip);
			chain_flip(sna);
		} else if (!flip->mode) {
			DBG(("%s: flip chain complete\n", __FUNCTION__));

			if (flip->chain) {
				sna_dri2_remove_event((WindowPtr)flip->draw,
						      flip);
				chain_swap(sna, flip->chain);
				flip->draw = NULL;
			}

			sna_dri2_event_free(sna, flip);
		} else if (!sna_dri2_flip_continue(sna, flip)) {
			DBG(("%s: no longer able to flip\n", __FUNCTION__));
			if (flip->draw == NULL || !sna_dri2_immediate_blit(sna, flip, false, flip->mode < 0))
				sna_dri2_event_free(sna, flip);
		}
		break;

	default: /* Unknown type */
		xf86DrvMsg(sna->scrn->scrnIndex, X_WARNING,
			   "%s: unknown vblank event received\n", __func__);
		sna_dri2_event_free(sna, flip);
		if (sna->dri2.flip_pending)
			chain_flip(sna);
		break;
	}
}

static uint64_t
get_current_msc(struct sna *sna, DrawablePtr draw, xf86CrtcPtr crtc)
{
	union drm_wait_vblank vbl;
	uint64_t ret = -1;

	VG_CLEAR(vbl);
	vbl.request.type = _DRM_VBLANK_RELATIVE;
	vbl.request.sequence = 0;
	if (sna_wait_vblank(sna, &vbl, sna_crtc_to_pipe(crtc)) == 0)
		ret = sna_crtc_record_vblank(crtc, &vbl);

	return draw_current_msc(draw, crtc, ret);
}

#if defined(CHECK_FOR_COMPOSITOR)
static Bool find(pointer value, XID id, pointer cdata)
{
	return TRUE;
}
#endif

static int use_triple_buffer(struct sna *sna, ClientPtr client, bool async)
{
	if ((sna->flags & SNA_TRIPLE_BUFFER) == 0) {
		DBG(("%s: triple buffer disabled, using FLIP\n", __FUNCTION__));
		return FLIP;
	}

	if (async) {
		DBG(("%s: running async, using %s\n", __FUNCTION__,
		     sna->flags & SNA_HAS_ASYNC_FLIP ? "FLIP_ASYNC" : "FLIP_COMPLETE"));
		return sna->flags & SNA_HAS_ASYNC_FLIP ? FLIP_ASYNC : FLIP_COMPLETE;
	}

	if (xorg_can_triple_buffer(sna)) {
		DBG(("%s: triple buffer enabled, using FLIP_THROTTLE\n", __FUNCTION__));
		return FLIP_THROTTLE;
	}

#if defined(CHECK_FOR_COMPOSITOR)
	/* Hack: Disable triple buffering for compositors */
	{
		struct sna_client *priv = sna_client(client);
		if (priv->is_compositor == 0)
			priv->is_compositor =
				LookupClientResourceComplex(client,
							    CompositeClientWindowType+1,
							    find, NULL) ? FLIP : FLIP_COMPLETE;

		DBG(("%s: fake triple buffer enabled?=%d using %s\n", __FUNCTION__,
		     priv->is_compositor != FLIP, priv->is_compositor == FLIP ? "FLIP" : "FLIP_COMPLETE"));
		return priv->is_compositor;
	}
#else
	DBG(("%s: fake triple buffer enabled, using FLIP_COMPLETE\n", __FUNCTION__));
	return FLIP_COMPLETE;
#endif
}

static bool immediate_swap(struct sna *sna,
			   uint64_t target_msc,
			   uint64_t divisor,
			   DrawablePtr draw,
			   xf86CrtcPtr crtc,
			   uint64_t *current_msc)
{
	if (divisor == 0) {
		*current_msc = -1;

		if (sna->flags & SNA_NO_WAIT) {
			DBG(("%s: yes, waits are disabled\n", __FUNCTION__));
			return true;
		}

		if (target_msc)
			*current_msc = get_current_msc(sna, draw, crtc);

		DBG(("%s: current_msc=%ld, target_msc=%ld -- %s\n",
		     __FUNCTION__, (long)*current_msc, (long)target_msc,
		     (*current_msc >= target_msc - 1) ? "yes" : "no"));
		return *current_msc >= target_msc - 1;
	}

	DBG(("%s: explicit waits requests, divisor=%ld\n",
	     __FUNCTION__, (long)divisor));
	*current_msc = get_current_msc(sna, draw, crtc);
	return false;
}

static bool
sna_dri2_schedule_flip(ClientPtr client, DrawablePtr draw, xf86CrtcPtr crtc,
		       DRI2BufferPtr front, DRI2BufferPtr back,
		       CARD64 *target_msc, CARD64 divisor, CARD64 remainder,
		       DRI2SwapEventPtr func, void *data)
{
	struct sna *sna = to_sna_from_drawable(draw);
	struct sna_dri2_event *info;
	uint64_t current_msc;

	if (immediate_swap(sna, *target_msc, divisor, draw, crtc, &current_msc)) {
		int type;

		info = sna->dri2.flip_pending;
		DBG(("%s: performing immediate swap on pipe %d, pending? %d, mode: %d, continuation? %d\n",
		     __FUNCTION__, sna_crtc_to_pipe(crtc),
		     info != NULL, info ? info->mode : 0,
		     info && info->draw == draw));

		if (info && info->draw == draw) {
			assert(info->type != FLIP);
			assert(info->front == front);
			if (info->back != back) {
				_sna_dri2_destroy_buffer(sna, info->back);
				info->back = sna_dri2_reference_buffer(back);
			}
			if (info->mode || current_msc >= *target_msc) {
				DBG(("%s: executing xchg of pending flip\n",
				     __FUNCTION__));
				sna_dri2_xchg(draw, front, back);
				info->mode = type = FLIP_COMPLETE;
				goto new_back;
			} else {
				DBG(("%s: chaining flip\n", __FUNCTION__));
				type = FLIP_THROTTLE;
				if (xorg_can_triple_buffer(sna))
					info->mode = -type;
				else
					info->mode = -FLIP_COMPLETE;
				goto out;
			}
		}

		info = sna_dri2_add_event(sna, draw, client);
		if (info == NULL)
			return false;

		assert(info->crtc == crtc);
		info->event_complete = func;
		info->event_data = data;

		info->front = sna_dri2_reference_buffer(front);
		info->back = sna_dri2_reference_buffer(back);

		if (sna->dri2.flip_pending) {
			/* We need to first wait (one vblank) for the
			 * async flips to complete before this client
			 * can take over.
			 */
			DBG(("%s: queueing flip after pending completion\n",
			     __FUNCTION__));
			info->type = type = FLIP;
			sna->dri2.flip_pending = info;
			assert(info->queued);
			current_msc++;
		} else {
			info->type = type = use_triple_buffer(sna, client, *target_msc == 0);
			if (!sna_dri2_flip(sna, info)) {
				DBG(("%s: flip failed, falling back\n", __FUNCTION__));
				sna_dri2_event_free(sna, info);
				return false;
			}
		}

		swap_limit(draw, 1 + (type == FLIP_THROTTLE));
		if (type >= FLIP_COMPLETE) {
new_back:
			if (!xorg_can_triple_buffer(sna))
				sna_dri2_get_back(sna, draw, back, info);
			DBG(("%s: fake triple buffering, unblocking client\n", __FUNCTION__));
			frame_swap_complete(sna, info, DRI2_EXCHANGE_COMPLETE);
			if (info->type == FLIP_ASYNC)
				sna_dri2_event_free(sna, info);
		}
out:
		DBG(("%s: target_msc=%llu\n", __FUNCTION__, current_msc + 1));
		*target_msc = current_msc + 1;
		return true;
	}

	info = sna_dri2_add_event(sna, draw, client);
	if (info == NULL)
		return false;

	assert(info->crtc == crtc);
	info->event_complete = func;
	info->event_data = data;
	info->type = FLIP;

	info->front = sna_dri2_reference_buffer(front);
	info->back = sna_dri2_reference_buffer(back);

	/*
	 * If divisor is zero, or current_msc is smaller than target_msc
	 * we just need to make sure target_msc passes before initiating
	 * the swap.
	 */
	if (divisor && current_msc >= *target_msc) {
		DBG(("%s: missed target, queueing event for next: current=%lld, target=%lld, divisor=%lld, remainder=%lld\n",
		     __FUNCTION__,
		     (long long)current_msc,
		     (long long)*target_msc,
		     (long long)divisor,
		     (long long)remainder));

		*target_msc = current_msc + remainder - current_msc % divisor;
		if (*target_msc <= current_msc)
			*target_msc += divisor;
	}

	if (*target_msc <= current_msc + 1) {
		if (!sna_dri2_flip(sna, info)) {
			sna_dri2_event_free(sna, info);
			return false;
		}
		*target_msc = current_msc + 1;
	} else {
		union drm_wait_vblank vbl;

		VG_CLEAR(vbl);

		vbl.request.type =
			DRM_VBLANK_ABSOLUTE |
			DRM_VBLANK_EVENT;

		/* Account for 1 frame extra pageflip delay */
		vbl.reply.sequence = draw_target_seq(draw, *target_msc - 1);
		vbl.request.signal = (uintptr_t)info;

		info->queued = true;
		if (sna_wait_vblank(sna, &vbl, info->pipe)) {
			sna_dri2_event_free(sna, info);
			return false;
		}
	}

	DBG(("%s: reported target_msc=%llu\n", __FUNCTION__, *target_msc));
	swap_limit(draw, 1);
	return true;
}

static bool
sna_dri2_schedule_xchg(ClientPtr client, DrawablePtr draw, xf86CrtcPtr crtc,
		       DRI2BufferPtr front, DRI2BufferPtr back,
		       CARD64 *target_msc, CARD64 divisor, CARD64 remainder,
		       DRI2SwapEventPtr func, void *data)
{
	struct sna *sna = to_sna_from_drawable(draw);
	uint64_t current_msc;
	bool sync, event;

	if (!immediate_swap(sna, *target_msc, divisor, draw, crtc, &current_msc))
		return false;

	sync = current_msc < *target_msc;
	event = dri2_chain(draw) == NULL;
	if (!sync || event) {
		DBG(("%s: performing immediate xchg on pipe %d\n",
		     __FUNCTION__, sna_crtc_to_pipe(crtc)));
		sna_dri2_xchg(draw, front, back);
	}
	if (sync) {
		struct sna_dri2_event *info;

		info = sna_dri2_add_event(sna, draw, client);
		if (!info)
			goto complete;

		info->event_complete = func;
		info->event_data = data;

		info->front = sna_dri2_reference_buffer(front);
		info->back = sna_dri2_reference_buffer(back);
		info->type = SWAP_THROTTLE;

		if (event) {
			union drm_wait_vblank vbl;

			VG_CLEAR(vbl);
			vbl.request.type =
				DRM_VBLANK_RELATIVE |
				DRM_VBLANK_EVENT;
			vbl.request.sequence = 1;
			vbl.request.signal = (uintptr_t)info;

			info->queued = true;
			if (sna_wait_vblank(sna, &vbl, info->pipe)) {
				sna_dri2_event_free(sna, info);
				goto complete;
			}

			swap_limit(draw, 2);
		}
	} else {
complete:
		fake_swap_complete(sna, client, draw, crtc, DRI2_EXCHANGE_COMPLETE, func, data);
	}

	*target_msc = current_msc + 1;
	return true;
}

static bool
sna_dri2_schedule_xchg_crtc(ClientPtr client, DrawablePtr draw, xf86CrtcPtr crtc,
			    DRI2BufferPtr front, DRI2BufferPtr back,
			    CARD64 *target_msc, CARD64 divisor, CARD64 remainder,
			    DRI2SwapEventPtr func, void *data)
{
	struct sna *sna = to_sna_from_drawable(draw);
	uint64_t current_msc;
	bool sync, event;

	if (!immediate_swap(sna, *target_msc, divisor, draw, crtc, &current_msc))
		return false;

	sync = current_msc < *target_msc;
	event = dri2_chain(draw) == NULL;
	if (!sync || event) {
		DBG(("%s: performing immediate xchg only on pipe %d\n",
		     __FUNCTION__, sna_crtc_to_pipe(crtc)));
		sna_dri2_xchg_crtc(sna, draw, crtc, front, back);
	}
	if (sync) {
		struct sna_dri2_event *info;

		info = sna_dri2_add_event(sna, draw, client);
		if (!info)
			goto complete;

		info->event_complete = func;
		info->event_data = data;

		info->front = sna_dri2_reference_buffer(front);
		info->back = sna_dri2_reference_buffer(back);
		info->type = SWAP_THROTTLE;

		if (event) {
			union drm_wait_vblank vbl;

			VG_CLEAR(vbl);
			vbl.request.type =
				DRM_VBLANK_RELATIVE |
				DRM_VBLANK_EVENT;
			vbl.request.sequence = 1;
			vbl.request.signal = (uintptr_t)info;

			info->queued = true;
			if (sna_wait_vblank(sna, &vbl, info->pipe)) {
				sna_dri2_event_free(sna, info);
				goto complete;
			}

			swap_limit(draw, 2);
		}
	} else {
complete:
		fake_swap_complete(sna, client, draw, crtc, DRI2_EXCHANGE_COMPLETE, func, data);
	}

	*target_msc = current_msc + 1;
	return true;
}

static bool has_pending_events(struct sna *sna)
{
	struct pollfd pfd;
	pfd.fd = sna->kgem.fd;
	pfd.events = POLLIN;
	return poll(&pfd, 1, 0) == 1;
}

/*
 * ScheduleSwap is responsible for requesting a DRM vblank event for the
 * appropriate frame.
 *
 * In the case of a blit (e.g. for a windowed swap) or buffer exchange,
 * the vblank requested can simply be the last queued swap frame + the swap
 * interval for the drawable.
 *
 * In the case of a page flip, we request an event for the last queued swap
 * frame + swap interval - 1, since we'll need to queue the flip for the frame
 * immediately following the received event.
 *
 * The client will be blocked if it tries to perform further GL commands
 * after queueing a swap, though in the Intel case after queueing a flip, the
 * client is free to queue more commands; they'll block in the kernel if
 * they access buffers busy with the flip.
 *
 * When the swap is complete, the driver should call into the server so it
 * can send any swap complete events that have been requested.
 */
static int
sna_dri2_schedule_swap(ClientPtr client, DrawablePtr draw, DRI2BufferPtr front,
		       DRI2BufferPtr back, CARD64 *target_msc, CARD64 divisor,
		       CARD64 remainder, DRI2SwapEventPtr func, void *data)
{
	struct sna *sna = to_sna_from_drawable(draw);
	union drm_wait_vblank vbl;
	xf86CrtcPtr crtc = NULL;
	struct sna_dri2_event *info = NULL;
	CARD64 current_msc;

	DBG(("%s: draw=%lu %dx%d, pixmap=%ld %dx%d, back=%u (refs=%d/%d, flush=%d) , front=%u (refs=%d/%d, flush=%d)\n",
	     __FUNCTION__,
	     (long)draw->id, draw->width, draw->height,
	     get_drawable_pixmap(draw)->drawable.serialNumber,
	     get_drawable_pixmap(draw)->drawable.width,
	     get_drawable_pixmap(draw)->drawable.height,
	     get_private(back)->bo->handle,
	     get_private(back)->refcnt,
	     get_private(back)->bo->refcnt,
	     get_private(back)->bo->flush,
	     get_private(front)->bo->handle,
	     get_private(front)->refcnt,
	     get_private(front)->bo->refcnt,
	     get_private(front)->bo->flush));

	DBG(("%s(target_msc=%llu, divisor=%llu, remainder=%llu)\n",
	     __FUNCTION__,
	     (long long)*target_msc,
	     (long long)divisor,
	     (long long)remainder));

	assert(get_private(front)->refcnt);
	assert(get_private(back)->refcnt);

	assert(get_private(front)->bo->refcnt);
	assert(get_private(back)->bo->refcnt);

	if (get_private(front)->pixmap != get_drawable_pixmap(draw))
		goto skip;

	if (get_private(back)->stale)
		goto skip;

	assert(sna_pixmap_from_drawable(draw)->flush);

	if (draw->type != DRAWABLE_PIXMAP) {
		WindowPtr win = (WindowPtr)draw;
		struct dri2_window *priv = dri2_window(win);
		if (priv->front) {
			assert(front == priv->front);
			assert(get_private(priv->front)->refcnt > 1);
			get_private(priv->front)->refcnt--;
			priv->front = NULL;
		}
		if (win->clipList.extents.x2 <= win->clipList.extents.x1 ||
		    win->clipList.extents.y2 <= win->clipList.extents.y1)
			goto skip;
	}

	/* Drawable not displayed... just complete the swap */
	if ((sna->flags & SNA_NO_WAIT) == 0)
		crtc = sna_dri2_get_crtc(draw);
	if (crtc == NULL) {
		DBG(("%s: off-screen, immediate update\n", __FUNCTION__));
		goto blit;
	}

	assert(draw->type != DRAWABLE_PIXMAP);

	while (dri2_chain(draw) && has_pending_events(sna)) {
		DBG(("%s: flushing pending events\n", __FUNCTION__));
		sna_mode_wakeup(sna);
	}

	if (can_xchg(sna, draw, front, back) &&
	    sna_dri2_schedule_xchg(client, draw, crtc, front, back,
				   target_msc, divisor, remainder,
				   func, data))
		return TRUE;

	if (can_xchg_crtc(sna, draw, front, back, crtc) &&
	    sna_dri2_schedule_xchg_crtc(client, draw, crtc, front, back,
					target_msc, divisor, remainder,
					func, data))
		return TRUE;

	if (can_flip(sna, draw, front, back, crtc) &&
	    sna_dri2_schedule_flip(client, draw, crtc, front, back,
				  target_msc, divisor, remainder,
				  func, data))
		return TRUE;

	VG_CLEAR(vbl);

	info = sna_dri2_add_event(sna, draw, client);
	if (!info)
		goto blit;

	assert(info->crtc == crtc);
	info->event_complete = func;
	info->event_data = data;

	info->front = sna_dri2_reference_buffer(front);
	info->back = sna_dri2_reference_buffer(back);

	if (immediate_swap(sna, *target_msc, divisor, draw, crtc, &current_msc)) {
		bool sync = current_msc < *target_msc;
		if (!sna_dri2_immediate_blit(sna, info, sync, true))
			sna_dri2_event_free(sna, info);
		*target_msc = current_msc + sync;
		return TRUE;
	}

	vbl.request.type =
		DRM_VBLANK_ABSOLUTE |
		DRM_VBLANK_EVENT;
	vbl.request.signal = (uintptr_t)info;

	/*
	 * If divisor is zero, or current_msc is smaller than target_msc
	 * we just need to make sure target_msc passes before initiating
	 * the swap.
	 */
	info->type = SWAP;
	info->queued = true;
	if (divisor && current_msc >= *target_msc) {
		DBG(("%s: missed target, queueing event for next: current=%lld, target=%lld, divisor=%lld, remainder=%lld\n",
		     __FUNCTION__,
		     (long long)current_msc,
		     (long long)*target_msc,
		     (long long)divisor,
		     (long long)remainder));

		*target_msc = current_msc + remainder - current_msc % divisor;
		if (*target_msc <= current_msc)
			*target_msc += divisor;
	}
	vbl.request.sequence = draw_target_seq(draw, *target_msc - 1);
	if (*target_msc <= current_msc + 1) {
		DBG(("%s: performing blit before queueing\n", __FUNCTION__));
		assert(info->queued);
		info->bo = __sna_dri2_copy_region(sna, draw, NULL,
						  back, front,
						  true);
		info->type = SWAP_WAIT;

		vbl.request.type =
			DRM_VBLANK_RELATIVE |
			DRM_VBLANK_EVENT;
		vbl.request.sequence = 1;
		*target_msc = current_msc + 1;
	}

	assert(info->queued);
	if (sna_wait_vblank(sna, &vbl, info->pipe))
		goto blit;

	DBG(("%s: reported target_msc=%llu\n", __FUNCTION__, *target_msc));
	swap_limit(draw, 1 + (info->type == SWAP_WAIT));
	return TRUE;

blit:
	DBG(("%s -- blit\n", __FUNCTION__));
	if (info)
		sna_dri2_event_free(sna, info);
	if (can_xchg(sna, draw, front, back))
		sna_dri2_xchg(draw, front, back);
	else
		__sna_dri2_copy_region(sna, draw, NULL, back, front, false);
skip:
	DBG(("%s: unable to show frame, unblocking client\n", __FUNCTION__));
	if (crtc == NULL)
		crtc = sna_mode_first_crtc(sna);
	fake_swap_complete(sna, client, draw, crtc, DRI2_BLIT_COMPLETE, func, data);
	*target_msc = 0; /* offscreen, so zero out target vblank count */
	return TRUE;
}

/*
 * Get current frame count and frame count timestamp, based on drawable's
 * crtc.
 */
static int
sna_dri2_get_msc(DrawablePtr draw, CARD64 *ust, CARD64 *msc)
{
	struct sna *sna = to_sna_from_drawable(draw);
	xf86CrtcPtr crtc = sna_dri2_get_crtc(draw);
	const struct ust_msc *swap;

	DBG(("%s(draw=%ld, pipe=%d)\n", __FUNCTION__, draw->id,
	     crtc ? sna_crtc_to_pipe(crtc) : -1));

	if (crtc != NULL) {
		union drm_wait_vblank vbl;

		VG_CLEAR(vbl);
		vbl.request.type = _DRM_VBLANK_RELATIVE;
		vbl.request.sequence = 0;
		if (sna_wait_vblank(sna, &vbl, sna_crtc_to_pipe(crtc)) == 0)
			sna_crtc_record_vblank(crtc, &vbl);
	} else
		/* Drawable not displayed, make up a *monotonic* value */
		crtc = sna_mode_first_crtc(sna);

	swap = sna_crtc_last_swap(crtc);
	*msc = draw_current_msc(draw, crtc, swap->msc);
	*ust = ust64(swap->tv_sec, swap->tv_usec);
	DBG(("%s: msc=%llu, ust=%llu\n", __FUNCTION__,
	     (long long)*msc, (long long)*ust));
	return TRUE;
}

/*
 * Request a DRM event when the requested conditions will be satisfied.
 *
 * We need to handle the event and ask the server to wake up the client when
 * we receive it.
 */
static int
sna_dri2_schedule_wait_msc(ClientPtr client, DrawablePtr draw, CARD64 target_msc,
			   CARD64 divisor, CARD64 remainder)
{
	struct sna *sna = to_sna_from_drawable(draw);
	struct sna_dri2_event *info = NULL;
	xf86CrtcPtr crtc;
	CARD64 current_msc;
	union drm_wait_vblank vbl;
	const struct ust_msc *swap;
	int pipe;

	crtc = sna_dri2_get_crtc(draw);
	DBG(("%s(pipe=%d, target_msc=%llu, divisor=%llu, rem=%llu)\n",
	     __FUNCTION__, crtc ? sna_crtc_to_pipe(crtc) : -1,
	     (long long)target_msc,
	     (long long)divisor,
	     (long long)remainder));

	/* Drawable not visible, return immediately */
	if (crtc == NULL)
		goto out_complete;

	pipe = sna_crtc_to_pipe(crtc);

	VG_CLEAR(vbl);

	/* Get current count */
	vbl.request.type = _DRM_VBLANK_RELATIVE;
	vbl.request.sequence = 0;
	if (sna_wait_vblank(sna, &vbl, pipe))
		goto out_complete;

	current_msc = draw_current_msc(draw, crtc, sna_crtc_record_vblank(crtc, &vbl));

	/* If target_msc already reached or passed, set it to
	 * current_msc to ensure we return a reasonable value back
	 * to the caller. This keeps the client from continually
	 * sending us MSC targets from the past by forcibly updating
	 * their count on this call.
	 */
	if (divisor == 0 && current_msc >= target_msc)
		goto out_complete;

	info = sna_dri2_add_event(sna, draw, client);
	if (!info)
		goto out_complete;

	assert(info->crtc == crtc);
	info->type = WAITMSC;

	vbl.request.signal = (uintptr_t)info;
	vbl.request.type = DRM_VBLANK_ABSOLUTE | DRM_VBLANK_EVENT;
	/*
	 * If divisor is zero, or current_msc is smaller than target_msc,
	 * we just need to make sure target_msc passes before waking up the
	 * client. Otherwise, compute the next msc to match divisor/remainder.
	 */
	if (divisor && current_msc >= target_msc) {
		DBG(("%s: missed target, queueing event for next: current=%lld, target=%lld, divisor=%lld, remainder=%lld\n",
		     __FUNCTION__,
		     (long long)current_msc,
		     (long long)target_msc,
		     (long long)divisor,
		     (long long)remainder));
		target_msc = current_msc + remainder - current_msc % divisor;
		if (target_msc <= current_msc)
			target_msc += divisor;
	}
	vbl.request.sequence = draw_target_seq(draw, target_msc);

	info->queued = true;
	if (sna_wait_vblank(sna, &vbl, pipe))
		goto out_free_info;

	DRI2BlockClient(client, draw);
	return TRUE;

out_free_info:
	sna_dri2_event_free(sna, info);
out_complete:
	if (crtc == NULL)
		crtc = sna_mode_first_crtc(sna);
	swap = sna_crtc_last_swap(crtc);
	DRI2WaitMSCComplete(client, draw,
			    draw_current_msc(draw, crtc, swap->msc),
			    swap->tv_sec, swap->tv_usec);
	return TRUE;
}
#else
void sna_dri2_destroy_window(WindowPtr win) { }
#endif

static bool has_i830_dri(void)
{
	return access(DRI_DRIVER_PATH "/i830_dri.so", R_OK) == 0;
}

static int
namecmp(const char *s1, const char *s2)
{
	char c1, c2;

	if (!s1 || *s1 == 0) {
		if (!s2 || *s2 == 0)
			return 0;
		else
			return 1;
	}

	while (*s1 == '_' || *s1 == ' ' || *s1 == '\t')
		s1++;

	while (*s2 == '_' || *s2 == ' ' || *s2 == '\t')
		s2++;

	c1 = isupper(*s1) ? tolower(*s1) : *s1;
	c2 = isupper(*s2) ? tolower(*s2) : *s2;
	while (c1 == c2) {
		if (c1 == '\0')
			return 0;

		s1++;
		while (*s1 == '_' || *s1 == ' ' || *s1 == '\t')
			s1++;

		s2++;
		while (*s2 == '_' || *s2 == ' ' || *s2 == '\t')
			s2++;

		c1 = isupper(*s1) ? tolower(*s1) : *s1;
		c2 = isupper(*s2) ? tolower(*s2) : *s2;
	}

	return c1 - c2;
}

static bool is_level(const char **str)
{
	const char *s = *str;
	char *end;
	unsigned val;

	if (s == NULL || *s == '\0')
		return true;

	if (namecmp(s, "on") == 0)
		return true;
	if (namecmp(s, "true") == 0)
		return true;
	if (namecmp(s, "yes") == 0)
		return true;

	if (namecmp(s, "0") == 0)
		return true;
	if (namecmp(s, "off") == 0)
		return true;
	if (namecmp(s, "false") == 0)
		return true;
	if (namecmp(s, "no") == 0)
		return true;

	val = strtoul(s, &end, 0);
	if (val && *end == '\0')
		return true;
	if (val && *end == ':')
		*str = end + 1;
	return false;
}

static const char *dri_driver_name(struct sna *sna)
{
	const char *s = xf86GetOptValString(sna->Options, OPTION_DRI);

	if (is_level(&s)) {
		if (sna->kgem.gen < 030)
			return has_i830_dri() ? "i830" : "i915";
		else if (sna->kgem.gen < 040)
			return "i915";
		else
			return "i965";
	}

	return s;
}

bool sna_dri2_open(struct sna *sna, ScreenPtr screen)
{
	DRI2InfoRec info;
	int major = 1, minor = 0;
#if DRI2INFOREC_VERSION >= 4
	const char *driverNames[2];
#endif

	DBG(("%s()\n", __FUNCTION__));

	if (wedged(sna)) {
		xf86DrvMsg(sna->scrn->scrnIndex, X_WARNING,
			   "loading DRI2 whilst the GPU is wedged.\n");
	}

	if (xf86LoaderCheckSymbol("DRI2Version"))
		DRI2Version(&major, &minor);

	if (minor < 1) {
		xf86DrvMsg(sna->scrn->scrnIndex, X_WARNING,
			   "DRI2 requires DRI2 module version 1.1.0 or later\n");
		return false;
	}

	memset(&info, '\0', sizeof(info));
	info.fd = sna->kgem.fd;
	info.driverName = dri_driver_name(sna);
	info.deviceName = intel_get_client_name(sna->scrn);

	DBG(("%s: loading dri driver '%s' [gen=%d] for device '%s'\n",
	     __FUNCTION__, info.driverName, sna->kgem.gen, info.deviceName));

#if DRI2INFOREC_VERSION == 2
	/* The ABI between 2 and 3 was broken so we could get rid of
	 * the multi-buffer alloc functions.  Make sure we indicate the
	 * right version so DRI2 can reject us if it's version 3 or above. */
	info.version = 2;
#else
	info.version = 3;
#endif
	info.CreateBuffer = sna_dri2_create_buffer;
	info.DestroyBuffer = sna_dri2_destroy_buffer;

	info.CopyRegion = sna_dri2_copy_region;
#if DRI2INFOREC_VERSION >= 4
	info.version = 4;
	info.ScheduleSwap = sna_dri2_schedule_swap;
	info.GetMSC = sna_dri2_get_msc;
	info.ScheduleWaitMSC = sna_dri2_schedule_wait_msc;
	info.numDrivers = 2;
	info.driverNames = driverNames;
	driverNames[0] = info.driverName;
	driverNames[1] = info.driverName;
#endif

#if DRI2INFOREC_VERSION >= 6
	if (xorg_can_triple_buffer(sna)) {
		info.version = 6;
		info.SwapLimitValidate = sna_dri2_swap_limit_validate;
		info.ReuseBufferNotify = sna_dri2_reuse_buffer;
	}
#endif

#if USE_ASYNC_SWAP
	info.version = 10;
	info.scheduleSwap0 = 1;
#endif

	return DRI2ScreenInit(screen, &info);
}

void sna_dri2_close(struct sna *sna, ScreenPtr screen)
{
	DBG(("%s()\n", __FUNCTION__));
	DRI2CloseScreen(screen);
}
