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

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

#include "xorg-server.h"
#include "xf86.h"
#include "xf86_OSproc.h"

#include "xf86Pci.h"
#include "xf86drm.h"

#include "windowstr.h"
#include "shadow.h"
#include "fb.h"

#include "intel.h"
#include "i830_reg.h"

#include "i915_drm.h"

#include "present.h"

#include "intel_glamor.h"
#include "uxa.h"

struct intel_present_vblank_event {
	uint64_t        event_id;
};

static uint32_t pipe_select(int pipe)
{
	if (pipe > 1)
		return pipe << DRM_VBLANK_HIGH_CRTC_SHIFT;
	else if (pipe > 0)
		return DRM_VBLANK_SECONDARY;
	else
		return 0;
}

static RRCrtcPtr
intel_present_get_crtc(WindowPtr window)
{
	ScreenPtr screen = window->drawable.pScreen;
	ScrnInfoPtr pScrn = xf86ScreenToScrn(screen);
	BoxRec box, crtcbox;
	xf86CrtcPtr crtc;
	RRCrtcPtr randr_crtc = NULL;

	box.x1 = window->drawable.x;
	box.y1 = window->drawable.y;
	box.x2 = box.x1 + window->drawable.width;
	box.y2 = box.y1 + window->drawable.height;

	crtc = intel_covering_crtc(pScrn, &box, NULL, &crtcbox);

	/* Make sure the CRTC is valid and this is the real front buffer */
	if (crtc != NULL && !crtc->rotatedData)
		randr_crtc = crtc->randr_crtc;

	return randr_crtc;
}

static int
intel_present_crtc_pipe(ScreenPtr screen, RRCrtcPtr randr_crtc)
{
	xf86CrtcPtr crtc;

	if (randr_crtc == NULL)
		return 0;

	crtc = randr_crtc->devPrivate;
	return intel_crtc_to_pipe(crtc);
}

static int
intel_present_get_ust_msc(RRCrtcPtr crtc, CARD64 *ust, CARD64 *msc)
{
	xf86CrtcPtr             xf86_crtc = crtc->devPrivate;
	ScreenPtr               screen = crtc->pScreen;
	ScrnInfoPtr             scrn = xf86ScreenToScrn(screen);

	return intel_get_crtc_msc_ust(scrn, xf86_crtc, msc, ust);
}

/*
 * Flush the DRM event queue when full; this
 * makes space for new requests
 */
static Bool
intel_present_flush_drm_events(ScreenPtr screen)
{
	ScrnInfoPtr             scrn = xf86ScreenToScrn(screen);
	intel_screen_private    *intel = intel_get_screen_private(scrn);

	return intel_mode_read_drm_events(intel) >= 0;
}

/*
 * Called when the queued vblank event has occurred
 */
static void
intel_present_vblank_handler(ScrnInfoPtr scrn, xf86CrtcPtr crtc, uint64_t msc, uint64_t usec, void *data)
{
	struct intel_present_vblank_event       *event = data;

	present_event_notify(event->event_id, usec, msc);
	free(event);
}

/*
 * Called when the queued vblank is aborted
 */
static void
intel_present_vblank_abort(ScrnInfoPtr scrn, xf86CrtcPtr crtc, void *data)
{
	struct intel_present_vblank_event       *event = data;

	free(event);
}

/*
 * Queue an event to report back to the Present extension when the specified
 * MSC has past
 */
static int
intel_present_queue_vblank(RRCrtcPtr                    crtc,
                           uint64_t                     event_id,
                           uint64_t                     msc)
{
	xf86CrtcPtr                             xf86_crtc = crtc->devPrivate;
	ScreenPtr                               screen = crtc->pScreen;
	ScrnInfoPtr                             scrn = xf86ScreenToScrn(screen);
	intel_screen_private                    *intel = intel_get_screen_private(scrn);
	int                                     pipe = intel_present_crtc_pipe(screen, crtc);
	struct intel_present_vblank_event       *event;
	drmVBlank                               vbl;
	int                                     ret;
	uint32_t                                seq;

	event = calloc(sizeof(struct intel_present_vblank_event), 1);
	if (!event)
		return BadAlloc;
	event->event_id = event_id;
	seq = intel_drm_queue_alloc(scrn, xf86_crtc, event,
				    intel_present_vblank_handler,
				    intel_present_vblank_abort);
	if (!seq) {
		free(event);
		return BadAlloc;
	}

	vbl.request.type = DRM_VBLANK_ABSOLUTE | DRM_VBLANK_EVENT | pipe_select(pipe);
	vbl.request.sequence = intel_crtc_msc_to_sequence(scrn, xf86_crtc, msc);
	vbl.request.signal = seq;
	for (;;) {
		ret = drmWaitVBlank(intel->drmSubFD, &vbl);
		if (!ret)
			break;
		if (errno != EBUSY || !intel_present_flush_drm_events(screen))
			return BadAlloc;
	}
	DebugPresent(("\t\tiq %lld seq %u msc %u (hw msc %u)\n", event_id, seq, low_msc, vbl.request.sequence));
	return Success;
}

static Bool
intel_present_event_match(void *data, void *match_data)
{
	struct intel_present_vblank_event       *event = data;
	uint64_t                                *match = match_data;

	return *match == event->event_id;
}

/*
 * Remove a pending vblank event from the DRM queue so that it is not reported
 * to the extension
 */
static void
intel_present_abort_vblank(RRCrtcPtr crtc, uint64_t event_id, uint64_t msc)
{
	ScreenPtr       screen = crtc->pScreen;
	ScrnInfoPtr     scrn = xf86ScreenToScrn(screen);

	intel_drm_abort(scrn, intel_present_event_match, &event_id);
}

/*
 * Flush our batch buffer when requested by the Present extension.
 */
static void
intel_present_flush(WindowPtr window)
{
	ScreenPtr                               screen = window->drawable.pScreen;
	ScrnInfoPtr                             scrn = xf86ScreenToScrn(screen);
	intel_screen_private                    *intel = intel_get_screen_private(scrn);

	if (intel->flush_rendering)
		intel->flush_rendering(intel);
}

/*
 * Test to see if page flipping is possible on the target crtc
 */
static Bool
intel_present_check_flip(RRCrtcPtr              crtc,
                         WindowPtr              window,
                         PixmapPtr              pixmap,
                         Bool                   sync_flip)
{
	ScreenPtr               screen = window->drawable.pScreen;
	ScrnInfoPtr             scrn = xf86ScreenToScrn(screen);
	intel_screen_private    *intel = intel_get_screen_private(scrn);

	if (!scrn->vtSema)
		return FALSE;

	if (intel->shadow_present)
		return FALSE;

	if (!intel->use_pageflipping)
		return FALSE;

	if (crtc && !intel_crtc_on(crtc->devPrivate))
		return FALSE;

	return TRUE;
}

/*
 * Once the flip has been completed on all pipes, notify the
 * extension code telling it when that happened
 */
static void
intel_present_flip_event(uint64_t msc, uint64_t ust, void *pageflip_data)
{
	struct intel_present_vblank_event *event = pageflip_data;

	present_event_notify(event->event_id, ust, msc);
	free(event);
}

/*
 * The flip has been aborted, free the structure
 */
static void
intel_present_flip_abort(void *pageflip_data)
{
	struct intel_present_vblank_event *event = pageflip_data;

	free(event);
}

/*
 * Queue a flip on 'crtc' to 'pixmap' at 'target_msc'. If 'sync_flip' is true,
 * then wait for vblank. Otherwise, flip immediately
 */
static Bool
intel_present_flip(RRCrtcPtr                    crtc,
                   uint64_t                     event_id,
                   uint64_t                     target_msc,
                   PixmapPtr                    pixmap,
                   Bool                         sync_flip)
{
	ScreenPtr                               screen = crtc->pScreen;
	ScrnInfoPtr                             scrn = xf86ScreenToScrn(screen);
	intel_screen_private                    *intel = intel_get_screen_private(scrn);
	struct intel_present_vblank_event       *event;
	int                                     pipe = intel_present_crtc_pipe(screen, crtc);
	dri_bo                                  *bo;
	Bool                                    ret;

	if (!intel_present_check_flip(crtc, screen->root, pixmap, sync_flip))
		return FALSE;

	bo = intel_get_pixmap_bo(pixmap);
	if (!bo)
		return FALSE;

	event = calloc(1, sizeof(struct intel_present_vblank_event));
	if (!event)
		return FALSE;

	event->event_id = event_id;

	ret = intel_do_pageflip(intel, bo, pipe, !sync_flip,
				event,
				intel_present_flip_event,
				intel_present_flip_abort);
	if (!ret)
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "present flip failed\n");
	return ret;
}

/*
 * Queue a flip back to the normal frame buffer
 */
static void
intel_present_unflip(ScreenPtr screen, uint64_t event_id)
{
	ScrnInfoPtr                             scrn = xf86ScreenToScrn(screen);
	intel_screen_private                    *intel = intel_get_screen_private(scrn);
	struct intel_present_vblank_event       *event;
	PixmapPtr                               pixmap = screen->GetScreenPixmap(screen);
	dri_bo                                  *bo;
	Bool                                    ret;

	if (!intel_present_check_flip(NULL, screen->root, pixmap, true))
		return;

	bo = intel_get_pixmap_bo(pixmap);
	if (!bo)
		return;

	event = calloc(1, sizeof(struct intel_present_vblank_event));
	if (!event)
		return;

	event->event_id = event_id;

	ret = intel_do_pageflip(intel, bo, -1, FALSE, event, intel_present_flip_event, intel_present_flip_abort);
	if (!ret) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "present unflip failed\n");
	}
}

static present_screen_info_rec intel_present_screen_info = {
	.version = PRESENT_SCREEN_INFO_VERSION,

	.get_crtc = intel_present_get_crtc,
	.get_ust_msc = intel_present_get_ust_msc,
	.queue_vblank = intel_present_queue_vblank,
	.abort_vblank = intel_present_abort_vblank,
	.flush = intel_present_flush,

	.capabilities = PresentCapabilityNone,
	.check_flip = intel_present_check_flip,
	.flip = intel_present_flip,
	.unflip = intel_present_unflip,
};

static Bool
intel_present_has_async_flip(ScreenPtr screen)
{
#ifdef DRM_CAP_ASYNC_PAGE_FLIP
	ScrnInfoPtr             scrn = xf86ScreenToScrn(screen);
	intel_screen_private    *intel = intel_get_screen_private(scrn);
	int                     ret;
	uint64_t                value;

	ret = drmGetCap(intel->drmSubFD, DRM_CAP_ASYNC_PAGE_FLIP, &value);
	if (ret == 0)
		return value == 1;
#endif
	return FALSE;
}

Bool
intel_present_screen_init(ScreenPtr screen)
{
	if (intel_present_has_async_flip(screen))
		intel_present_screen_info.capabilities |= PresentCapabilityAsync;

	return present_screen_init(screen, &intel_present_screen_info);
}
