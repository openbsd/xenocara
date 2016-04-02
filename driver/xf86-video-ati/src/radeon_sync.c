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

#include "radeon.h"

#ifdef HAVE_MISYNCSHM_H

#include "misync.h"
#include "misyncshm.h"
#include "misyncstr.h"

/*
 * This whole file exists to wrap a sync fence trigger operation
 * so that we can flush the batch buffer to provide serialization
 * between the server and the shm fence client
 */

static DevPrivateKeyRec radeon_sync_fence_private_key;

typedef struct _radeon_sync_fence_private {
        SyncFenceSetTriggeredFunc set_triggered;
} radeon_sync_fence_private;

#define SYNC_FENCE_PRIV(pFence)                                         \
        (radeon_sync_fence_private *) dixLookupPrivate(&pFence->devPrivates, &radeon_sync_fence_private_key)

static void
radeon_sync_fence_set_triggered (SyncFence *fence)
{
	ScreenPtr screen = fence->pScreen;
	radeon_sync_fence_private *private = SYNC_FENCE_PRIV(fence);

	/* Flush pending rendering operations */
	radeon_cs_flush_indirect(xf86ScreenToScrn(screen));

	fence->funcs.SetTriggered = private->set_triggered;
	fence->funcs.SetTriggered(fence);
	private->set_triggered = fence->funcs.SetTriggered;
	fence->funcs.SetTriggered = radeon_sync_fence_set_triggered;
}

static void
radeon_sync_create_fence(ScreenPtr screen,
                        SyncFence *fence,
                        Bool initially_triggered)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONInfoPtr info = RADEONPTR(scrn);
	SyncScreenFuncsPtr screen_funcs = miSyncGetScreenFuncs(screen);
	radeon_sync_fence_private *private = SYNC_FENCE_PRIV(fence);

	screen_funcs->CreateFence = info->CreateFence;
	screen_funcs->CreateFence(screen, fence, initially_triggered);
	info->CreateFence = screen_funcs->CreateFence;
	screen_funcs->CreateFence = radeon_sync_create_fence;

	private->set_triggered = fence->funcs.SetTriggered;
	fence->funcs.SetTriggered = radeon_sync_fence_set_triggered;
}

Bool
radeon_sync_init(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONInfoPtr info = RADEONPTR(scrn);
	SyncScreenFuncsPtr screen_funcs;

	if (!miSyncShmScreenInit(screen)) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "SYNC extension fences disabled because "
			   "miSyncShmScreenInit failed\n");
		return FALSE;
	}

	if (!dixPrivateKeyRegistered(&radeon_sync_fence_private_key)) {
		if (!dixRegisterPrivateKey(&radeon_sync_fence_private_key,
					   PRIVATE_SYNC_FENCE,
					   sizeof (radeon_sync_fence_private))) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "SYNC extension fences disabled because "
				   "dixRegisterPrivateKey failed\n");
			return FALSE;
		}
	}

	xf86DrvMsg(xf86ScreenToScrn(screen)->scrnIndex, X_INFO,
		   "SYNC extension fences enabled\n");

	screen_funcs = miSyncGetScreenFuncs(screen);
	info->CreateFence = screen_funcs->CreateFence;
	screen_funcs->CreateFence = radeon_sync_create_fence;
	return TRUE;
}

void
radeon_sync_close(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONInfoPtr info = RADEONPTR(scrn);
	SyncScreenFuncsPtr screen_funcs = miSyncGetScreenFuncs(screen);

	if (screen_funcs && info->CreateFence)
		screen_funcs->CreateFence = info->CreateFence;

	info->CreateFence = NULL;
}

#else /* !HAVE_MISYNCSHM_H */

Bool
radeon_sync_init(ScreenPtr screen)
{
	xf86DrvMsg(xf86ScreenToScrn(screen)->scrnIndex, X_INFO,
		   "SYNC extension fences disabled because misyncshm.h not "
		   "available at build time\n");

	return FALSE;
}

void
radeon_sync_close(ScreenPtr screen)
{
}

#endif
