/* -*- c-basic-offset: 4 -*- */
/*
 * Copyright © 2006 Intel Corporation
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
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <assert.h>
#include <stdlib.h>

#include "xf86.h"
#include "i830.h"
#include "i830_ring.h"
#include "i915_drm.h"

#define DUMP_BATCHBUFFERS NULL /* "/tmp/i915-batchbuffers.dump" */

static int
intel_nondrm_exec(dri_bo *bo, unsigned int used, void *priv)
{
    ScrnInfoPtr scrn = priv;
    intel_screen_private *intel = intel_get_screen_private(scrn);

    BEGIN_LP_RING(4);
    OUT_RING(MI_BATCH_BUFFER_START | (2 << 6));
    OUT_RING(bo->offset);
    OUT_RING(MI_NOOP);
    OUT_RING(MI_NOOP);
    ADVANCE_LP_RING();

    return 0;
}

static int
intel_nondrm_exec_i830(dri_bo *bo, unsigned int used, void *priv)
{
    ScrnInfoPtr scrn = priv;
    intel_screen_private *intel = intel_get_screen_private(scrn);

    BEGIN_LP_RING(4);
    OUT_RING(MI_BATCH_BUFFER);
    OUT_RING(bo->offset);
    OUT_RING(bo->offset + intel->batch_used - 4);
    OUT_RING(MI_NOOP);
    ADVANCE_LP_RING();

    return 0;
}

/**
 * Creates a fence value representing a request to be passed.
 *
 * Stub implementation that should be avoided when DRM functions are available.
 */
static unsigned int
intel_nondrm_emit(void *priv)
{
    static unsigned int fence = 0;

    /* Match DRM in not using half the range. The fake bufmgr relies on this. */
    if (++fence >= 0x8000000)
	fence = 1;

    return fence;
}

/**
 * Waits on a fence representing a request to be passed.
 *
 * Stub implementation that should be avoided when DRM functions are available.
 */
static void
intel_nondrm_wait(unsigned int fence, void *priv)
{
    ScrnInfoPtr scrn = priv;

    i830_wait_ring_idle(scrn);
}

static void intel_next_batch(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int ret;

	/* The 865 has issues with larger-than-page-sized batch buffers. */
	if (IS_I865G(intel))
		intel->batch_bo =
		    dri_bo_alloc(intel->bufmgr, "batch", 4096, 4096);
	else
		intel->batch_bo =
		    dri_bo_alloc(intel->bufmgr, "batch", 4096 * 4, 4096);

	ret = dri_bo_map(intel->batch_bo, 1);
	if (ret != 0)
		FatalError("Failed to map batchbuffer: %s\n", strerror(-ret));

	intel->batch_used = 0;
	intel->batch_ptr = intel->batch_bo->virtual;

	/* We don't know when another client has executed, so we have
	 * to reinitialize our 3D state per batch.
	 */
	intel->last_3d = LAST_3D_OTHER;
}

void intel_batch_init(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	intel->batch_emit_start = 0;
	intel->batch_emitting = 0;

	intel_next_batch(scrn);

	if (!intel->have_gem) {
		if (IS_I830(intel) || IS_845G(intel)) {
			intel_bufmgr_fake_set_exec_callback(intel->bufmgr,
			    intel_nondrm_exec_i830, scrn);
		} else {
			intel_bufmgr_fake_set_exec_callback(intel->bufmgr,
			    intel_nondrm_exec, scrn);
		}
		intel_bufmgr_fake_set_fence_callback(intel->bufmgr,
		    intel_nondrm_emit, intel_nondrm_wait, scrn);
	}
}

void intel_batch_teardown(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->batch_ptr != NULL) {
		dri_bo_unmap(intel->batch_bo);
		intel->batch_ptr = NULL;

		dri_bo_unreference(intel->batch_bo);
		intel->batch_bo = NULL;

		dri_bo_unreference(intel->last_batch_bo);
		intel->last_batch_bo = NULL;
	}
}

void intel_batch_emit_flush(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int flags;

	assert (!intel->in_batch_atomic);

	/* Big hammer, look to the pipelined flushes in future. */
	flags = MI_WRITE_DIRTY_STATE | MI_INVALIDATE_MAP_CACHE;
	if (IS_I965G(intel))
		flags = 0;

	BEGIN_BATCH(1);
	OUT_BATCH(MI_FLUSH | flags);
	ADVANCE_BATCH();

	while (!list_is_empty(&intel->flush_pixmaps)) {
		struct intel_pixmap *entry;

		entry = list_first_entry(&intel->flush_pixmaps,
					 struct intel_pixmap,
					 flush);

		entry->flush_read_domains = entry->flush_write_domain = 0;
		list_del(&entry->flush);
	}

	intel->need_mi_flush = FALSE;
}

void intel_batch_submit(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int ret;

	assert (!intel->in_batch_atomic);

	if (intel->batch_used == 0)
		return;

	/* Emit a padding dword if we aren't going to be quad-word aligned. */
	if ((intel->batch_used & 4) == 0) {
		*(uint32_t *) (intel->batch_ptr + intel->batch_used) = MI_NOOP;
		intel->batch_used += 4;
	}

	/* Mark the end of the batchbuffer. */
	*(uint32_t *) (intel->batch_ptr + intel->batch_used) =
	    MI_BATCH_BUFFER_END;
	intel->batch_used += 4;

	if (DUMP_BATCHBUFFERS) {
	    FILE *file = fopen(DUMP_BATCHBUFFERS, "a");
	    if (file) {
		fwrite (intel->batch_ptr, intel->batch_used, 1, file);
		fclose(file);
	    }
	}

	dri_bo_unmap(intel->batch_bo);
	intel->batch_ptr = NULL;

	ret =
	    dri_bo_exec(intel->batch_bo, intel->batch_used, NULL, 0,
			0xffffffff);
	if (ret != 0) {
		static int once;

		if (!once) {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
				   "Failed to submit batch buffer, expect rendering corruption "
				   "or even a frozen display: %s.\n",
				   strerror(-ret));
			once = 1;
		}
	}

	while (!list_is_empty(&intel->batch_pixmaps)) {
		struct intel_pixmap *entry;

		entry = list_first_entry(&intel->batch_pixmaps,
					 struct intel_pixmap,
					 batch);

		entry->batch_read_domains = entry->batch_write_domain = 0;
		list_del(&entry->batch);
	}

	/* Mark that we need to flush whatever potential rendering we've done in the
	 * blockhandler.  We could set this less often, but it's probably not worth
	 * the work.
	 */
	intel->need_mi_flush = !list_is_empty(&intel->flush_pixmaps);
	while (!list_is_empty(&intel->flush_pixmaps)) {
		struct intel_pixmap *entry;

		entry = list_first_entry(&intel->flush_pixmaps,
					 struct intel_pixmap,
					 flush);

		entry->flush_read_domains = entry->flush_write_domain = 0;
		list_del(&entry->flush);
	}

	while (!list_is_empty(&intel->in_flight)) {
		struct intel_pixmap *entry;

		entry = list_first_entry(&intel->in_flight,
					 struct intel_pixmap,
					 in_flight);

		dri_bo_unreference(entry->bo);
		list_del(&entry->in_flight);
		xfree(entry);
	}

	/* Save a ref to the last batch emitted, which we use for syncing
	 * in debug code.
	 */
	dri_bo_unreference(intel->last_batch_bo);
	intel->last_batch_bo = intel->batch_bo;
	intel->batch_bo = NULL;

	intel_next_batch(scrn);

	if (intel->debug_flush & DEBUG_FLUSH_WAIT)
		intel_batch_wait_last(scrn);

	if (intel->batch_flush_notify)
		intel->batch_flush_notify(scrn);
}

/** Waits on the last emitted batchbuffer to be completed. */
void intel_batch_wait_last(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* Map it CPU write, which guarantees it's done.  This is a completely
	 * non performance path, so we don't need anything better.
	 */
	drm_intel_bo_map(intel->last_batch_bo, TRUE);
	drm_intel_bo_unmap(intel->last_batch_bo);
}

void intel_sync(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (I810_DEBUG & (DEBUG_VERBOSE_ACCEL | DEBUG_VERBOSE_SYNC))
		ErrorF("I830Sync\n");

	if (!scrn->vtSema || !intel->batch_bo || !intel->batch_ptr)
		return;

	intel_batch_emit_flush(scrn);
	intel_batch_submit(scrn);
	intel_batch_wait_last(scrn);
}
