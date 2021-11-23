/*
 * Copyright © 2020 Kevin Brace
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including
 * the next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
/*
 * Author(s):
 *
 * Kevin Brace <kevinbrace@gmx.com>
 */

#ifndef __OPENCHROME_DRM_H__
#define __OPENCHROME_DRM_H__


#include "drm.h"


#if defined(__cplusplus)
extern "C" {
#endif


#define DRM_OPENCHROME_GEM_CREATE	0x00
#define DRM_OPENCHROME_GEM_MAP		0x01
#define DRM_OPENCHROME_GEM_UNMAP	0x02


#define DRM_IOCTL_OPENCHROME_GEM_CREATE DRM_IOWR(DRM_COMMAND_BASE + DRM_OPENCHROME_GEM_CREATE, struct drm_openchrome_gem_create)
#define DRM_IOCTL_OPENCHROME_GEM_MAP    DRM_IOWR(DRM_COMMAND_BASE + DRM_OPENCHROME_GEM_MAP, struct drm_openchrome_gem_map)
#define DRM_IOCTL_OPENCHROME_GEM_UNMAP  DRM_IOR(DRM_COMMAND_BASE + DRM_OPENCHROME_GEM_UNMAP, struct drm_openchrome_gem_unmap)


struct drm_openchrome_gem_create {
	uint64_t size;
	uint32_t alignment;
	uint32_t domain;
	uint32_t handle;
	uint64_t offset;
};

struct drm_openchrome_gem_map {
	uint32_t handle;
	uint64_t map_offset;
};

struct drm_openchrome_gem_unmap {
	uint32_t handle;
};


#if defined(__cplusplus)
}
#endif

#endif /* __OPENCHROME_DRM_H__ */
