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

extern struct radeon_bo*
radeon_alloc_pixmap_bo(ScrnInfoPtr pScrn, int width, int height, int depth,
		       int usage_hint, int bitsPerPixel, int *new_pitch,
		       struct radeon_surface *new_surface, uint32_t *new_tiling);

extern Bool
radeon_get_pixmap_handle(PixmapPtr pixmap, uint32_t *handle);

extern uint32_t
radeon_get_pixmap_tiling_flags(PixmapPtr pPix);

extern Bool
radeon_share_pixmap_backing(struct radeon_bo *bo, void **handle_p);

extern Bool
radeon_set_shared_pixmap_backing(PixmapPtr ppix, void *fd_handle,
				 struct radeon_surface *surface);

#endif /* RADEON_BO_HELPER_H */
