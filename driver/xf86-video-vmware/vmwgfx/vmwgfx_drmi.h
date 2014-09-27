/*
 * Copyright 2011 VMWare, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author: Jakob Bornecrantz <wallbraker@gmail.com>
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 */

#ifndef _VMWGFX_DRMI_H_
#define _VMWGFX_DRMI_H_

#include <xorg-server.h>
#include <regionstr.h>
#include <stdint.h>
#include "vmwgfx_drm.h"

struct vmwgfx_dma_ctx;

extern int
vmwgfx_present_readback(int drm_fd, uint32_t fb_id, RegionPtr region);

extern int
vmwgfx_present(int drm_fd, uint32_t fb_id, unsigned int dst_x,
	       unsigned int dst_y, RegionPtr region, uint32_t handle);

struct vmwgfx_dmabuf {
  uint32_t handle;
  uint32_t gmr_id;
  uint32_t gmr_offset;
  size_t size;
};

extern struct vmwgfx_dmabuf*
vmwgfx_dmabuf_alloc(int drm_fd, size_t size);
extern void
vmwgfx_dmabuf_destroy(struct vmwgfx_dmabuf *buf);
extern void *
vmwgfx_dmabuf_map(struct vmwgfx_dmabuf *buf);
extern void
vmwgfx_dmabuf_unmap(struct vmwgfx_dmabuf *buf);

extern int
vmwgfx_dma(int host_x, int host_y,
	   RegionPtr region, struct vmwgfx_dmabuf *buf,
	   uint32_t buf_pitch, uint32_t surface_handle, int to_surface);

extern int
vmwgfx_num_streams(int drm_fd, uint32_t *ntot, uint32_t *nfree);

extern int
vmwgfx_claim_stream(int drm_fd, uint32_t *out);

extern int
vmwgfx_unref_stream(int drm_fd, uint32_t stream_id);

int
vmwgfx_cursor_bypass(int drm_fd, int xhot, int yhot);

int
vmwgfx_max_fb_size(int drm_fd, size_t *size);

int
vmwgfx_update_gui_layout(int drm_fd, unsigned int num_rects,
			 struct drm_vmw_rect *rects);
int
vmwgfx_get_param(int drm_fd, uint32_t param, uint64_t *out);

#ifdef HAVE_LIBDRM_2_4_38
int
vmwgfx_prime_fd_to_handle(int drm_fd, int prime_fd, uint32_t *handle);

void
vmwgfx_prime_release_handle(int drm_fd, uint32_t handle);
#endif /* HAVE_LIBDRM_2_4_38 */
#endif
