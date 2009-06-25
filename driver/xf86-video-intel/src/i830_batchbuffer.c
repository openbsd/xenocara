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
#include <errno.h>

#include "xf86.h"
#include "i830.h"
#include "i830_ring.h"
#include "i915_drm.h"

static int
intel_nondrm_exec(dri_bo *bo, unsigned int used, void *priv)
{
    ScrnInfoPtr pScrn = priv;
    I830Ptr pI830 = I830PTR(pScrn);

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
    ScrnInfoPtr pScrn = priv;
    I830Ptr pI830 = I830PTR(pScrn);

    BEGIN_LP_RING(4);
    OUT_RING(MI_BATCH_BUFFER);
    OUT_RING(bo->offset);
    OUT_RING(bo->offset + pI830->batch_used - 4);
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
    ScrnInfoPtr pScrn = priv;

    i830_wait_ring_idle(pScrn);
}

static void
intel_next_batch(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);

    /* The 865 has issues with larger-than-page-sized batch buffers. */
    if (IS_I865G(pI830))
	pI830->batch_bo = dri_bo_alloc(pI830->bufmgr, "batch", 4096, 4096);
    else
	pI830->batch_bo = dri_bo_alloc(pI830->bufmgr, "batch", 4096 * 4, 4096);

    if (dri_bo_map(pI830->batch_bo, 1) != 0)
	FatalError("Failed to map batchbuffer: %s\n", strerror(errno));

    pI830->batch_used = 0;
    pI830->batch_ptr = pI830->batch_bo->virtual;

    /* If we are using DRI2, we don't know when another client has executed,
     * so we have to reinitialize our 3D state per batch.
     */
    if (pI830->directRenderingType == DRI_DRI2)
	pI830->last_3d = LAST_3D_OTHER;
}

void
intel_batch_init(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);

    pI830->batch_emit_start = 0;
    pI830->batch_emitting = 0;

    intel_next_batch(pScrn);

    if (pI830->directRenderingType <= DRI_NONE) {
	if (IS_I830(pI830) || IS_845G(pI830)) {
	    intel_bufmgr_fake_set_exec_callback(pI830->bufmgr,
						intel_nondrm_exec_i830,
						pScrn);
	} else {
	    intel_bufmgr_fake_set_exec_callback(pI830->bufmgr,
						intel_nondrm_exec,
						pScrn);
	}
	intel_bufmgr_fake_set_fence_callback(pI830->bufmgr,
					     intel_nondrm_emit,
					     intel_nondrm_wait,
					     pScrn);
    }
}

void
intel_batch_teardown(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);

    if (pI830->batch_ptr != NULL) {
	dri_bo_unmap(pI830->batch_bo);
	dri_bo_unreference(pI830->batch_bo);
	pI830->batch_ptr = NULL;
    }
}

void
intel_batch_flush(ScrnInfoPtr pScrn, Bool flushed)
{
    I830Ptr pI830 = I830PTR(pScrn);
    int ret;

    if (pI830->batch_used == 0)
	return;

    /* If we're not using GEM, then emit a flush after each batch buffer */
    if (pI830->memory_manager == NULL && !flushed) {
	int flags = MI_WRITE_DIRTY_STATE | MI_INVALIDATE_MAP_CACHE;

	if (IS_I965G(pI830))
	    flags = 0;

	*(uint32_t *)(pI830->batch_ptr + pI830->batch_used) = MI_FLUSH | flags;
	pI830->batch_used += 4;
    }
	
    /* Emit a padding dword if we aren't going to be quad-word aligned. */
    if ((pI830->batch_used & 4) == 0) {
	*(uint32_t *)(pI830->batch_ptr + pI830->batch_used) = MI_NOOP;
	pI830->batch_used += 4;
    }

    /* Mark the end of the batchbuffer. */
    *(uint32_t *)(pI830->batch_ptr + pI830->batch_used) = MI_BATCH_BUFFER_END;
    pI830->batch_used += 4;

    dri_bo_unmap(pI830->batch_bo);
    pI830->batch_ptr = NULL;

    ret = dri_bo_exec(pI830->batch_bo, pI830->batch_used, NULL, 0, 0xffffffff);
    if (ret != 0)
	FatalError("Failed to submit batchbuffer: %s\n", strerror(-ret));

    dri_bo_unreference(pI830->batch_bo);
    intel_next_batch(pScrn);

    /* Mark that we need to flush whatever potential rendering we've done in the
     * blockhandler.  We could set this less often, but it's probably not worth
     * the work.
     */
    if (pI830->memory_manager != NULL)
	pI830->need_mi_flush = TRUE;

    if (pI830->batch_flush_notify)
	pI830->batch_flush_notify (pScrn);
}
