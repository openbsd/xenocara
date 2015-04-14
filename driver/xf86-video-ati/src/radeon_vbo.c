/*
 * Copyright © 2009 Red Hat, Inc.
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
 *    Dave Airlie <airlied@redhat.com>
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include "radeon.h"
#include "radeon_bo.h"
#include "radeon_cs.h"

#define VBO_SIZE (16*1024)

/* KMS vertex buffer support - for R600 only but could be used on previous gpus */

static struct radeon_bo *radeon_vbo_get_bo(ScrnInfoPtr pScrn);

void radeon_vbo_put(ScrnInfoPtr pScrn, struct radeon_vbo_object *vbo)
{

    if (vbo->vb_bo) {
	radeon_bo_unmap(vbo->vb_bo);
	radeon_bo_unref(vbo->vb_bo);
	vbo->vb_bo = NULL;
	vbo->vb_total = 0;
    }

    vbo->vb_offset = 0;
}

void radeon_vbo_get(ScrnInfoPtr pScrn, struct radeon_vbo_object *vbo)
{
    int ret;

    vbo->vb_bo = radeon_vbo_get_bo(pScrn);
    if (vbo->vb_bo) {
	radeon_bo_ref(vbo->vb_bo);
	ret = radeon_bo_map(vbo->vb_bo, 1);
	if (ret)
	    FatalError("Failed to map vb %d\n", ret);
    }

    vbo->vb_total = VBO_SIZE;
    vbo->vb_offset = 0;
    vbo->vb_start_op = vbo->vb_offset;
}

/* these functions could migrate to libdrm and
   be shared with the radeon 3D driver */
static int radeon_bo_is_idle(struct radeon_bo *bo)
{
    uint32_t domain;
    int ret = radeon_bo_is_busy(bo, &domain);
    return ret != -EBUSY;
}

void radeon_vbo_init_lists(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    accel_state->use_vbos = TRUE;
    make_empty_list(&accel_state->bo_free);
    make_empty_list(&accel_state->bo_wait);
    make_empty_list(&accel_state->bo_reserved);
}

void radeon_vbo_free_lists(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    struct radeon_dma_bo *dma_bo, *temp;

    foreach_s(dma_bo, temp, &accel_state->bo_free) {
	remove_from_list(dma_bo);
	radeon_bo_unref(dma_bo->bo);
	free(dma_bo);
    }

    foreach_s(dma_bo, temp, &accel_state->bo_wait) {
	remove_from_list(dma_bo);
	radeon_bo_unref(dma_bo->bo);
	free(dma_bo);
    }

    foreach_s(dma_bo, temp, &accel_state->bo_reserved) {
	remove_from_list(dma_bo);
	radeon_bo_unref(dma_bo->bo);
	free(dma_bo);
    }
}

void radeon_vbo_flush_bos(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    struct radeon_dma_bo *dma_bo, *temp;
    const int expire_at = ++accel_state->bo_free.expire_counter + DMA_BO_FREE_TIME;
    const int time = accel_state->bo_free.expire_counter;

    foreach_s(dma_bo, temp, &accel_state->bo_wait) {
	if (dma_bo->expire_counter == time) {
	    ErrorF("leaking dma buffer\n");
	    while ((dma_bo->bo = radeon_bo_unref(dma_bo->bo))) {}
	    remove_from_list(dma_bo);
	    free(dma_bo);
	    continue;
	}

	if (!radeon_bo_is_idle(dma_bo->bo))
	    continue;

	if (dma_bo->bo->ptr) {
	    ErrorF("bo with pointer on wait list!\n");
	    continue;
	}

	remove_from_list(dma_bo);
	dma_bo->expire_counter = expire_at;
	insert_at_tail(&accel_state->bo_free, dma_bo);
    }

    /* move reserved to wait list */
    foreach_s(dma_bo, temp, &accel_state->bo_reserved) {
	remove_from_list(dma_bo);
	dma_bo->expire_counter = expire_at;
	insert_at_tail(&accel_state->bo_wait, dma_bo);
    }

    /* free bos that have been unused */
    foreach_s(dma_bo, temp, &accel_state->bo_free) {
	if (dma_bo->expire_counter != time)
	    break;
	/* always keep one hanging around at end */
	if (at_end(&accel_state->bo_free, dma_bo)) {
	    dma_bo->expire_counter = time + DMA_BO_FREE_TIME;
	    break;
	}

	remove_from_list(dma_bo);
	radeon_bo_unref(dma_bo->bo);
	free(dma_bo);
    }
}

static struct radeon_bo *radeon_vbo_get_bo(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    struct radeon_dma_bo *dma_bo = NULL;
    struct radeon_bo *bo;

    if (is_empty_list(&accel_state->bo_free)) {
	dma_bo = calloc(1, sizeof(struct radeon_dma_bo));
	if (!dma_bo)
	    return NULL;

again_alloc:
	dma_bo->bo = radeon_bo_open(info->bufmgr, 0, VBO_SIZE,
				    0, RADEON_GEM_DOMAIN_GTT, 0);

	if (!dma_bo->bo) {
	    ErrorF("failure to allocate DMA BO\n");
	    free(dma_bo);
	    return NULL;
	}
	insert_at_head(&accel_state->bo_reserved, dma_bo);
    } else {
	dma_bo = last_elem(&accel_state->bo_free);
	remove_from_list(dma_bo);
	insert_at_head(&accel_state->bo_reserved, dma_bo);
    }

    if (is_empty_list(&accel_state->bo_reserved))
	goto again_alloc;

    bo = first_elem(&accel_state->bo_reserved)->bo;

    /* need a space check */
    if (radeon_cs_space_check_with_bo(info->cs,
				      bo,
				      RADEON_GEM_DOMAIN_GTT, 0))
	ErrorF("failed to revalidate\n");

    return bo;
}

