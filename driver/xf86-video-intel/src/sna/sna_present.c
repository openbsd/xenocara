/*
 * Copyright © 2014 Intel Corporation
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

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <xf86drm.h>

#include "sna.h"

#include <xf86.h>
#include <present.h>

static present_screen_info_rec present_info;

struct sna_present_event {
	uint64_t event_id;
	xf86CrtcPtr crtc;
};

static inline struct sna_present_event *
to_present_event(uintptr_t  data)
{
	return (struct sna_present_event *)(data & ~3);
}

#define MARK_PRESENT(x) ((void *)((uintptr_t)(x) | 2))

static int pipe_from_crtc(RRCrtcPtr crtc)
{
	return crtc ? sna_crtc_to_pipe(crtc->devPrivate) : -1;
}

static uint32_t pipe_select(int pipe)
{
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
	vbl->request.type |= pipe_select(pipe);
	return drmIoctl(sna->kgem.fd, DRM_IOCTL_WAIT_VBLANK, vbl);
}

static RRCrtcPtr
sna_present_get_crtc(WindowPtr window)
{
	struct sna *sna = to_sna_from_drawable(&window->drawable);
	BoxRec box;
	xf86CrtcPtr crtc;

	DBG(("%s\n", __FUNCTION__));

	box.x1 = window->drawable.x;
	box.y1 = window->drawable.y;
	box.x2 = box.x1 + window->drawable.width;
	box.y2 = box.y1 + window->drawable.height;

	crtc = sna_covering_crtc(sna, &box, NULL);
	if (crtc)
		return crtc->randr_crtc;

	return NULL;
}

static int
sna_present_get_ust_msc(RRCrtcPtr crtc, CARD64 *ust, CARD64 *msc)
{
	struct sna *sna = to_sna_from_screen(crtc->pScreen);
	int pipe = pipe_from_crtc(crtc);
	union drm_wait_vblank vbl;

	DBG(("%s(pipe=%d)\n", __FUNCTION__, pipe));

	VG_CLEAR(vbl);
	vbl.request.type = DRM_VBLANK_RELATIVE;
	vbl.request.sequence = 0;
	if (sna_wait_vblank(sna, &vbl, pipe) == 0) {
		*ust = ust64(vbl.reply.tval_sec, vbl.reply.tval_usec);
		*msc = sna_crtc_record_vblank(crtc->devPrivate, &vbl);
	} else {
		const struct ust_msc *swap = sna_crtc_last_swap(crtc->devPrivate);
		*ust = ust64(swap->tv_sec, swap->tv_usec);
		*msc = swap->msc;
	}

	DBG(("%s: pipe=%d, tv=%d.%06d msc=%lld\n", __FUNCTION__, pipe,
	     (int)(*ust / 1000000), (int)(*ust % 1000000),
	     (long long)*msc));

	return Success;
}

void
sna_present_vblank_handler(struct sna *sna, struct drm_event_vblank *event)
{
	struct sna_present_event *info = to_present_event(event->user_data);

	DBG(("%s: pipe=%d event=%lld, tv=%d.%06d msc=%d\n", __FUNCTION__,
	     sna_crtc_to_pipe(info->crtc), (long long)info->event_id,
	     event->tv_sec, event->tv_usec, event->sequence));
	present_event_notify(info->event_id,
			     ust64(event->tv_sec, event->tv_usec),
			     sna_crtc_record_event(info->crtc, event));
	free(info);
}

static int
sna_present_queue_vblank(RRCrtcPtr crtc, uint64_t event_id, uint64_t msc)
{
	struct sna *sna = to_sna_from_screen(crtc->pScreen);
	struct sna_present_event *event;
	union drm_wait_vblank vbl;

	DBG(("%s(pipe=%d, event=%lld, msc=%lld)\n",
	     __FUNCTION__, pipe_from_crtc(crtc),
	     (long long)event_id, (long long)msc));

	event = malloc(sizeof(struct sna_present_event));
	if (event == NULL)
		return BadAlloc;

	event->event_id = event_id;
	event->crtc = crtc->devPrivate;

	VG_CLEAR(vbl);
	vbl.request.type = DRM_VBLANK_ABSOLUTE | DRM_VBLANK_EVENT;
	vbl.request.sequence = msc;
	vbl.request.signal = (uintptr_t)MARK_PRESENT(event);
	if (sna_wait_vblank(sna, &vbl, sna_crtc_to_pipe(event->crtc))) {
		DBG(("%s: vblank enqueue failed\n", __FUNCTION__));
		free(event);
		return BadMatch;
	}

	return Success;
}

static void
sna_present_abort_vblank(RRCrtcPtr crtc, uint64_t event_id, uint64_t msc)
{
	DBG(("%s(pipe=%d, event=%lld, msc=%lld)\n",
	     __FUNCTION__, pipe_from_crtc(crtc),
	     (long long)event_id, (long long)msc));
}

static void
sna_present_flush(WindowPtr window)
{
	PixmapPtr pixmap = get_window_pixmap(window);
	struct sna_pixmap *priv;

	DBG(("%s(pixmap=%ld)\n", __FUNCTION__, pixmap->drawable.serialNumber));

	priv = sna_pixmap_move_to_gpu(pixmap, MOVE_READ | MOVE_ASYNC_HINT | __MOVE_FORCE);
	if (priv && priv->gpu_bo)
		kgem_scanout_flush(&to_sna_from_pixmap(pixmap)->kgem, priv->gpu_bo);
}

static bool
check_flip__crtc(struct sna *sna,
		 RRCrtcPtr crtc)
{
	if (!sna_crtc_is_on(crtc->devPrivate)) {
		DBG(("%s: CRTC off\n", __FUNCTION__));
		return false;
	}

	assert(sna->scrn->vtSema);

	if (sna->mode.shadow_active) {
		DBG(("%s: shadow buffer active\n", __FUNCTION__));
		return false;
	}

	return true;
}

static Bool
sna_present_check_flip(RRCrtcPtr crtc,
		       WindowPtr window,
		       PixmapPtr pixmap,
		       Bool sync_flip)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *flip;

	DBG(("%s(pipe=%d, pixmap=%ld, sync_flip=%d)\n",
	     __FUNCTION__,
	     pipe_from_crtc(crtc),
	     pixmap->drawable.serialNumber,
	     sync_flip));

	if (sna->flags & SNA_NO_FLIP) {
		DBG(("%s: flips not suported\n", __FUNCTION__));
		return FALSE;
	}

	if (sync_flip) {
		if ((sna->flags & SNA_HAS_FLIP) == 0) {
			DBG(("%s: async flips not suported\n", __FUNCTION__));
			return FALSE;
		}
	} else {
		if ((sna->flags & SNA_HAS_ASYNC_FLIP) == 0) {
			DBG(("%s: async flips not suported\n", __FUNCTION__));
			return FALSE;
		}
	}

	if (!check_flip__crtc(sna, crtc)) {
		DBG(("%s: flip invalid for CRTC\n", __FUNCTION__));
		return FALSE;
	}

	flip = sna_pixmap(pixmap);
	if (flip == NULL) {
		DBG(("%s: unattached pixmap\n", __FUNCTION__));
		return FALSE;
	}

	if (flip->cpu_bo && IS_STATIC_PTR(flip->ptr)) {
		DBG(("%s: SHM pixmap\n", __FUNCTION__));
		return FALSE;
	}

	return TRUE;
}

static uint64_t gettime_ust64(void)
{
	struct timespec tv;

	if (clock_gettime(CLOCK_MONOTONIC, &tv))
		return 0;

	return ust64(tv.tv_sec, tv.tv_nsec / 1000);
}

static Bool
page_flip__async(RRCrtcPtr crtc,
		 uint64_t event_id,
		 uint64_t target_msc,
		 struct kgem_bo *bo)
{
	DBG(("%s(pipe=%d, event=%lld, handle=%d)\n",
	     __FUNCTION__,
	     pipe_from_crtc(crtc),
	     (long long)event_id,
	     bo->handle));

	if (!sna_page_flip(to_sna_from_screen(crtc->pScreen), bo, NULL, NULL)) {
		DBG(("%s: async pageflip failed\n", __FUNCTION__));
		present_info.capabilities &= ~PresentCapabilityAsync;
		return FALSE;
	}

	present_event_notify(event_id, gettime_ust64(), target_msc);
	return TRUE;
}

static void
present_flip_handler(struct sna *sna,
		     struct drm_event_vblank *event,
		     void *data)
{
	struct sna_present_event *info = data;
	struct ust_msc swap;

	DBG(("%s(sequence=%d)\n", __FUNCTION__, event->sequence));

	if (info->crtc == NULL) {
		swap.tv_sec = event->tv_sec;
		swap.tv_usec = event->tv_usec;
		swap.msc = event->sequence;
	} else
		swap = *sna_crtc_last_swap(info->crtc);

	DBG(("%s: pipe=%d, tv=%d.%06d msc %lld, complete\n", __FUNCTION__,
	     info->crtc ? sna_crtc_to_pipe(info->crtc) : -1,
	     swap.tv_sec, swap.tv_usec, (long long)swap.msc));
	present_event_notify(info->event_id, ust64(swap.tv_sec, swap.tv_usec), swap.msc);
	free(info);
}

static Bool
page_flip(ScreenPtr screen,
	  RRCrtcPtr crtc,
	  uint64_t event_id,
	  struct kgem_bo *bo)
{
	struct sna *sna = to_sna_from_screen(screen);
	struct sna_present_event *event;

	DBG(("%s(pipe=%d, event=%lld, handle=%d)\n",
	     __FUNCTION__,
	     pipe_from_crtc(crtc),
	     (long long)event_id,
	     bo->handle));

	event = malloc(sizeof(struct sna_present_event));
	if (event == NULL)
		return FALSE;

	event->event_id = event_id;
	event->crtc = crtc ? crtc->devPrivate : NULL;
	if (!sna_page_flip(sna, bo, present_flip_handler, event)) {
		DBG(("%s: pageflip failed\n", __FUNCTION__));
		free(event);
		return FALSE;
	}

	return TRUE;
}

static struct kgem_bo *
get_flip_bo(PixmapPtr pixmap)
{
	struct sna *sna = to_sna_from_pixmap(pixmap);
	struct sna_pixmap *priv;

	DBG(("%s(pixmap=%ld)\n", __FUNCTION__, pixmap->drawable.serialNumber));

	priv = sna_pixmap_move_to_gpu(pixmap, MOVE_READ | __MOVE_FORCE);
	if (priv == NULL) {
		DBG(("%s: cannot force pixmap to the GPU\n", __FUNCTION__));
		return NULL;
	}

	if (sna->flags & SNA_LINEAR_FB &&
	    priv->gpu_bo->tiling &&
	    !sna_pixmap_change_tiling(pixmap, I915_TILING_NONE)) {
		DBG(("%s: invalid tiling for scanout, user requires linear\n", __FUNCTION__));
		return NULL;
	}

	if (priv->gpu_bo->tiling == I915_TILING_Y &&
	    !sna_pixmap_change_tiling(pixmap, I915_TILING_X)) {
		DBG(("%s: invalid Y-tiling, cannot convert\n", __FUNCTION__));
		return NULL;
	}

	priv->pinned |= PIN_SCANOUT;
	return priv->gpu_bo;
}

static Bool
sna_present_flip(RRCrtcPtr crtc,
		 uint64_t event_id,
		 uint64_t target_msc,
		 PixmapPtr pixmap,
		 Bool sync_flip)
{
	struct kgem_bo *bo;

	DBG(("%s(pipe=%d, event=%lld, msc=%lld, pixmap=%ld, sync?=%d)\n",
	     __FUNCTION__,
	     pipe_from_crtc(crtc),
	     (long long)event_id,
	     (long long)target_msc,
	     pixmap->drawable.serialNumber, sync_flip));

	if (!check_flip__crtc(to_sna_from_pixmap(pixmap), crtc)) {
		DBG(("%s: flip invalid for CRTC\n", __FUNCTION__));
		return FALSE;
	}

	bo = get_flip_bo(pixmap);
	if (bo == NULL) {
		DBG(("%s: flip invalid bo\n", __FUNCTION__));
		return FALSE;
	}

	if (sync_flip)
		return page_flip(crtc->pScreen, crtc, event_id, bo);
	else
		return page_flip__async(crtc, event_id, target_msc, bo);
}

static void
sna_present_unflip(ScreenPtr screen, uint64_t event_id)
{
	struct sna *sna = to_sna_from_screen(screen);
	struct kgem_bo *bo;

	DBG(("%s(event=%lld)\n", __FUNCTION__, (long long)event_id));
	if (sna->mode.front_active == 0) {
		const struct ust_msc *swap;

		DBG(("%s: no CRTC active, perform no-op flip\n", __FUNCTION__));

notify:
		swap = sna_crtc_last_swap(sna_mode_first_crtc(sna));
		present_event_notify(event_id,
				     ust64(swap->tv_sec, swap->tv_usec),
				     swap->msc);
		return;
	}

	bo = get_flip_bo(screen->GetScreenPixmap(screen));
	if (bo == NULL || !page_flip(screen, NULL, event_id, bo)) {
		DBG(("%s: failed, trying to restore original mode\n", __FUNCTION__));
		xf86SetDesiredModes(sna->scrn);
		goto notify;
	}
}

static present_screen_info_rec present_info = {
	.version = PRESENT_SCREEN_INFO_VERSION,

	.get_crtc = sna_present_get_crtc,
	.get_ust_msc = sna_present_get_ust_msc,
	.queue_vblank = sna_present_queue_vblank,
	.abort_vblank = sna_present_abort_vblank,
	.flush = sna_present_flush,

	.capabilities = PresentCapabilityNone,
	.check_flip = sna_present_check_flip,
	.flip = sna_present_flip,
	.unflip = sna_present_unflip,
};

bool sna_present_open(struct sna *sna, ScreenPtr screen)
{
	if (sna->mode.num_real_crtc == 0)
		return false;

	sna_present_update(sna);

	return present_screen_init(screen, &present_info);
}

void sna_present_update(struct sna *sna)
{
	if (sna->flags & SNA_HAS_ASYNC_FLIP)
		present_info.capabilities |= PresentCapabilityAsync;
	else
		present_info.capabilities &= ~PresentCapabilityAsync;

	DBG(("%s: has_async_flip? %d\n", __FUNCTION__,
	     !!(present_info.capabilities & PresentCapabilityAsync)));
}

void sna_present_close(struct sna *sna, ScreenPtr screen)
{
	DBG(("%s()\n", __FUNCTION__));
}
