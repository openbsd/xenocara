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

#include "intel.h"
#include "misyncshm.h"
#include "misyncstr.h"

/*
 * This whole file exists to wrap a sync fence trigger operation
 * so that we can flush the batch buffer to provide serialization
 * between the server and the shm fence client
 */

static DevPrivateKeyRec intel_sync_fence_private_key;

typedef struct _intel_sync_fence_private {
        SyncFenceSetTriggeredFunc set_triggered;
} intel_sync_fence_private;

#define SYNC_FENCE_PRIV(pFence)                                         \
        (intel_sync_fence_private *) dixLookupPrivate(&pFence->devPrivates, &intel_sync_fence_private_key)

static void
intel_sync_fence_set_triggered (SyncFence *fence)
{
	ScreenPtr screen = fence->pScreen;
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	intel_sync_fence_private *private = SYNC_FENCE_PRIV(fence);

	/* Flush pending rendering operations */
	if (intel->flush_rendering)
		intel->flush_rendering(intel);

	fence->funcs.SetTriggered = private->set_triggered;
	fence->funcs.SetTriggered(fence);
	private->set_triggered = fence->funcs.SetTriggered;
	fence->funcs.SetTriggered = intel_sync_fence_set_triggered;
}

static void
intel_sync_create_fence(ScreenPtr screen,
                        SyncFence *fence,
                        Bool initially_triggered)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	SyncScreenFuncsPtr screen_funcs = miSyncGetScreenFuncs(screen);
	intel_sync_fence_private *private = SYNC_FENCE_PRIV(fence);

	screen_funcs->CreateFence = intel->save_sync_screen_funcs.CreateFence;
	screen_funcs->CreateFence(screen, fence, initially_triggered);
	intel->save_sync_screen_funcs.CreateFence = screen_funcs->CreateFence;
	screen_funcs->CreateFence = intel_sync_create_fence;

	private->set_triggered = fence->funcs.SetTriggered;
	fence->funcs.SetTriggered = intel_sync_fence_set_triggered;
}

Bool
intel_sync_init(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	SyncScreenFuncsPtr screen_funcs;

	if (!miSyncShmScreenInit(screen))
		return FALSE;

	if (!dixPrivateKeyRegistered(&intel_sync_fence_private_key)) {
		if (!dixRegisterPrivateKey(&intel_sync_fence_private_key,
					   PRIVATE_SYNC_FENCE,
					   sizeof (intel_sync_fence_private)))
			return FALSE;
	}

	screen_funcs = miSyncGetScreenFuncs(screen);
	intel->save_sync_screen_funcs.CreateFence = screen_funcs->CreateFence;
	screen_funcs->CreateFence = intel_sync_create_fence;
	return TRUE;
}

void
intel_sync_close(ScreenPtr screen)
{
        ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
        intel_screen_private *intel = intel_get_screen_private(scrn);
        SyncScreenFuncsPtr screen_funcs = miSyncGetScreenFuncs(screen);

        if (screen_funcs)
                screen_funcs->CreateFence = intel->save_sync_screen_funcs.CreateFence;
}
