/*
 * Copyright 2003 Red Hat, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <sys/mman.h>

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86fbman.h"

#ifdef HAVE_DRI
#include "xf86drm.h"
#endif

#include "drm_fourcc.h"
#include "via_driver.h"
#ifdef HAVE_DRI
#include "via_drm.h"
#else
#include "drm_fourcc.h"
#endif

/*
 *	Isolate the wonders of X memory allocation and DRI memory allocation
 *	and 4.3 or 4.4 differences in one abstraction.
 *
 *	The pool code indicates who provided the memory:
 *	0  -  nobody
 *	1  -  xf86 linear
 *	2  -  DRM
 */
int
viaOffScreenLinear(struct buffer_object *obj, ScrnInfoPtr pScrn,
                   unsigned long size)
{
    int depth = pScrn->bitsPerPixel >> 3;
    FBLinearPtr linear;

    linear = xf86AllocateOffscreenLinear(pScrn->pScreen,
                                        (size + depth - 1) / depth,
                                        32, NULL, NULL, NULL);
    if (!linear)
        return BadAlloc;
    obj->offset = linear->offset * depth;
    obj->handle = (unsigned long) linear;
    obj->domain = TTM_PL_FLAG_VRAM;
    obj->size = size;
    return Success;
}

struct buffer_object *
drm_bo_alloc_surface(ScrnInfoPtr pScrn, unsigned int width, unsigned int height,
                    int format, unsigned int alignment, int domain)
{
    struct buffer_object *obj = NULL;
    int pitch;

    switch (format) {
    case DRM_FORMAT_C8:
        pitch = width;
        break;

    case DRM_FORMAT_XRGB1555:
    case DRM_FORMAT_RGB565:
        pitch = width * 2;
        break;

    case DRM_FORMAT_RGB888:
        pitch = width * 3;
        break;

    case DRM_FORMAT_XRGB2101010:
    case DRM_FORMAT_XRGB8888:
        pitch = width * 4;
        break;
    }

    pitch = ALIGN_TO(pitch, alignment);
    obj = drm_bo_alloc(pScrn, pitch * height, alignment, domain);
    if (!obj->pitch)
        obj->pitch = pitch;
    return obj;
}

struct buffer_object *
drm_bo_alloc(ScrnInfoPtr pScrn, unsigned int size, unsigned int alignment, int domain)
{
    struct buffer_object *obj = NULL;
    VIAPtr pVia = VIAPTR(pScrn);
    int ret = 0;

    obj = xnfcalloc(1, sizeof(*obj));
    if (obj) {
        switch (domain) {
        case TTM_PL_FLAG_TT:
        case TTM_PL_FLAG_VRAM:
            if (pVia->directRenderingType == DRI_NONE) {
                if (Success != viaOffScreenLinear(obj, pScrn, size)) {
                    ErrorF("Linear memory allocation failed\n");
                    ret = -ENOMEM;
                } else
                    DEBUG(ErrorF("%lu bytes of Linear memory allocated at %lx, handle %lu\n", obj->size, obj->offset, obj->handle));
#ifdef HAVE_DRI
            } else if (pVia->directRenderingType == DRI_1) {
                drm_via_mem_t drm;

                size = ALIGN_TO(size, alignment);
                drm.context = DRIGetContext(pScrn->pScreen);
                drm.size = size;
                drm.type = (domain == TTM_PL_FLAG_TT ? VIA_MEM_AGP : VIA_MEM_VIDEO);
                ret = drmCommandWriteRead(pVia->drmmode.fd, DRM_VIA_ALLOCMEM,
                                            &drm, sizeof(drm_via_mem_t));
                if (!ret && (size == drm.size)) {
                    if (domain == TTM_PL_FLAG_VRAM)
                        drm.offset -= pVia->FBFreeStart;
                    obj->offset = ALIGN_TO(drm.offset, alignment);
                    obj->handle = drm.index;
                    obj->domain = domain;
                    obj->size = drm.size;
                    DEBUG(ErrorF("%lu bytes of DRI memory allocated at %lx, handle %lu\n",
                                obj->size, obj->offset, obj->handle));
                }
            } else if (pVia->directRenderingType == DRI_2) {
                struct drm_via_gem_object args;

                /* Some day this will be moved to libdrm. */
                args.domains = domain;
                args.alignment = alignment;
                args.pitch = 0;
                args.size = size;
                ret = drmCommandWriteRead(pVia->drmmode.fd, DRM_VIA_GEM_CREATE,
                                        &args, sizeof(struct drm_via_gem_object));
                if (!ret) {
                    /* Okay the X server expects to know the offset because
                     * of non-KMS. Once we have KMS working the offset
                     * will not be valid. */
                    obj->map_offset = args.map_handle;
                    obj->offset = args.offset;
                    obj->handle = args.handle;
                    obj->pitch = args.pitch;
                    obj->size = args.size;
                    obj->domain = domain;
                    DEBUG(ErrorF("%lu bytes of DRI2 memory allocated at %lx, handle %lu\n",
                                obj->size, obj->offset, obj->handle));
                }
#endif
            }
            break;

        case TTM_PL_FLAG_SYSTEM:
        default:
            ret = -ENXIO;
            break;
        }

        if (ret) {
            DEBUG(ErrorF("DRM memory allocation failed %d\n", ret));
            free(obj);
            obj = NULL;
        };
    }
    return obj;
}

void*
drm_bo_map(ScrnInfoPtr pScrn, struct buffer_object *obj)
{
    VIAPtr pVia = VIAPTR(pScrn);

    if (pVia->directRenderingType == DRI_2) {
        obj->ptr = mmap(0, obj->size, PROT_READ | PROT_WRITE,
                        MAP_SHARED, pVia->drmmode.fd, obj->map_offset);
        if (obj->ptr == MAP_FAILED) {
            DEBUG(ErrorF("mmap failed with error %d\n", -errno));
            obj->ptr = NULL;
        }
    } else {
        switch (obj->domain) {
#ifdef HAVE_DRI
        case TTM_PL_FLAG_TT:
            obj->ptr = (pVia->agpMappedAddr + obj->offset);
            break;
#endif
        case TTM_PL_FLAG_VRAM:
            obj->ptr = (pVia->FBBase + obj->offset);
            break;
        default:
            obj->ptr = NULL;
            break;
        }
    }
    return obj->ptr;
}

void
drm_bo_unmap(ScrnInfoPtr pScrn, struct buffer_object *obj)
{
    VIAPtr pVia = VIAPTR(pScrn);

    if (pVia->directRenderingType == DRI_2) {
        munmap(obj->ptr, obj->size);
    }

    obj->ptr = NULL;
}

void
drm_bo_free(ScrnInfoPtr pScrn, struct buffer_object *obj)
{
    VIAPtr pVia = VIAPTR(pScrn);

    if (obj) {
        DEBUG(ErrorF("Freed %lu (pool %d)\n", obj->offset, obj->domain));
        switch (obj->domain) {
        case TTM_PL_FLAG_VRAM:
        case TTM_PL_FLAG_TT:
            if (pVia->directRenderingType == DRI_NONE) {
                FBLinearPtr linear = (FBLinearPtr) obj->handle;

                xf86FreeOffscreenLinear(linear);
#ifdef HAVE_DRI
            } else if (pVia->directRenderingType == DRI_1) {
                drm_via_mem_t drm;

                drm.index = obj->handle;
                if (drmCommandWrite(pVia->drmmode.fd, DRM_VIA_FREEMEM,
                                    &drm, sizeof(drm_via_mem_t)) < 0)
                    ErrorF("DRM failed to free for handle %lu.\n", obj->handle);
            } else  if (pVia->directRenderingType == DRI_2) {
                struct drm_gem_close close;

                close.handle = obj->handle;
                if (drmIoctl(pVia->drmmode.fd, DRM_IOCTL_GEM_CLOSE, &close) < 0)
                    ErrorF("DRM failed to free for handle %lu.\n", obj->handle);
#endif
            }
            break;

        default:
            break;
        }
        free(obj);
    }
}

Bool
drm_bo_manager_init(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    Bool ret = TRUE;

    if (pVia->directRenderingType == DRI_2)
        return ret;
    ret = umsCreate(pScrn);
#ifdef HAVE_DRI
    if (pVia->directRenderingType == DRI_1)
        ret = VIADRIKernelInit(pScrn);
#endif
    return ret;
}
