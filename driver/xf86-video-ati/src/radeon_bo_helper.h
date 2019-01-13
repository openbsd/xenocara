/*
 * Copyright 2012  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef RADEON_BO_HELPER_H
#define RADEON_BO_HELPER_H 1

#ifdef USE_GLAMOR
#include <gbm.h>
#endif

#define RADEON_BO_FLAGS_GBM	0x1

struct radeon_buffer {
	union {
#ifdef USE_GLAMOR
		struct gbm_bo *gbm;
#endif
		struct radeon_bo *radeon;
	} bo;
	uint32_t ref_count;
    uint32_t flags;
};

extern struct radeon_buffer *
radeon_alloc_pixmap_bo(ScrnInfoPtr pScrn, int width, int height, int depth,
		       int usage_hint, int bitsPerPixel, int *new_pitch,
		       struct radeon_surface *new_surface, uint32_t *new_tiling);

extern void
radeon_finish(ScrnInfoPtr scrn, struct radeon_buffer *bo);

extern void
radeon_pixmap_clear(PixmapPtr pixmap);

extern uint32_t
radeon_get_pixmap_tiling_flags(PixmapPtr pPix);

extern Bool
radeon_share_pixmap_backing(struct radeon_bo *bo, void **handle_p);

extern Bool
radeon_set_shared_pixmap_backing(PixmapPtr ppix, void *fd_handle,
				 struct radeon_surface *surface);

/**
 * get_drawable_pixmap() returns the backing pixmap for a given drawable.
 *
 * @param drawable the drawable being requested.
 *
 * This function returns the backing pixmap for a drawable, whether it is a
 * redirected window, unredirected window, or already a pixmap.
 */
static inline PixmapPtr get_drawable_pixmap(DrawablePtr drawable)
{
    if (drawable->type == DRAWABLE_PIXMAP)
	return (PixmapPtr)drawable;
    else
	return drawable->pScreen->GetWindowPixmap((WindowPtr)drawable);
}

static inline void
radeon_buffer_ref(struct radeon_buffer *buffer)
{
    buffer->ref_count++;
}

static inline void
radeon_buffer_unref(struct radeon_buffer **buffer)
{
    struct radeon_buffer *buf = *buffer;

    if (!buf)
	return;

    if (buf->ref_count > 1) {
	buf->ref_count--;
	return;
    }

#ifdef USE_GLAMOR
    if (buf->flags & RADEON_BO_FLAGS_GBM) {
	gbm_bo_destroy(buf->bo.gbm);
    } else
#endif
    {
	radeon_bo_unmap(buf->bo.radeon);
	radeon_bo_unref(buf->bo.radeon);
    }

    free(buf);
    *buffer = NULL;
}

#endif /* RADEON_BO_HELPER_H */
