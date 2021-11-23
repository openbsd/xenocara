/*
 * Copyright © 2007 Red Hat, Inc.
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

#ifdef OPENCHROMEDRI
#include "xf86drm.h"
#endif

#include "drm_fourcc.h"
#include "via_driver.h"
#ifdef OPENCHROMEDRI
#include "via_drm.h"
#include "openchrome_drm.h"

#else
#include "drm_fourcc.h"
#endif

static int
viaOffScreenLinear(ScrnInfoPtr pScrn, struct buffer_object *obj,
                    unsigned long size, unsigned long alignment)
{
    FBLinearPtr linear;
    int depth = pScrn->bitsPerPixel / 8;
    int newAlignment;
    int ret = 0;

    newAlignment = alignment;
    linear = xf86AllocateOffscreenLinear(pScrn->pScreen,
                                            (size + depth - 1) / depth,
                                            newAlignment,
                                            NULL, NULL, NULL);
    if (!linear) {
        ret = -ENOMEM;
        goto exit;
    }

    obj->offset = linear->offset * depth;
    obj->handle = (unsigned long) linear;
    obj->domain = TTM_PL_VRAM;
    obj->size = size;

exit:
    return ret;
}

struct buffer_object *
drm_bo_alloc(ScrnInfoPtr pScrn, unsigned long size,
                unsigned long alignment, int domain)
{
    struct buffer_object *obj = NULL;
    VIAPtr pVia = VIAPTR(pScrn);
    int ret = 0;

    obj = xnfcalloc(1, sizeof(*obj));
    if (!obj) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                            "Allocation of a buffer object used for "
                            "memory allocation failed.\n"));
        goto exit;
    }

    switch (domain) {
    case TTM_PL_TT:
    case TTM_PL_VRAM:
        if (pVia->directRenderingType == DRI_NONE) {
            if (!pVia->useEXA) {
                ret = viaOffScreenLinear(pScrn, obj,
                                            size, alignment);
                if (ret) {
                    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                                        "Linear memory allocation "
                                        "failed.\n"));
                } else {
                    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                        "%lu bytes of linear memory "
                                        "allocated at 0x%lx, handle "
                                        "0x%lx.\n",
                                        obj->size, obj->offset,
                                        obj->handle));
                }
            } else {
                ret = viaEXAOffscreenAlloc(pScrn, obj,
                                            size, alignment);
                if (ret) {
                    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                                        "EXA offscreen memory "
                                        "allocation failed.\n"));
                } else {
                    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                        "%lu bytes of EXA offscreen "
                                        "memory allocated at 0x%lx, "
                                        "handle 0x%lx.\n",
                                        obj->size, obj->offset,
                                        obj->handle));
                }
            }
#ifdef OPENCHROMEDRI
        } else if (pVia->directRenderingType == DRI_1) {
            drm_via_mem_t drm;

            size = ALIGN_TO(size, alignment);
            drm.context = DRIGetContext(pScrn->pScreen);
            drm.size = size;
            drm.type = (domain == TTM_PL_TT ? VIA_MEM_AGP : VIA_MEM_VIDEO);
            ret = drmCommandWriteRead(pVia->drmmode.fd, DRM_VIA_ALLOCMEM,
                                        &drm, sizeof(drm_via_mem_t));
            if (!ret && (size == drm.size)) {
                if (domain == TTM_PL_VRAM)
                    drm.offset -= pVia->FBFreeStart;
                obj->offset = ALIGN_TO(drm.offset, alignment);
                obj->handle = drm.index;
                obj->domain = domain;
                obj->size = drm.size;
                DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                    "%lu bytes of DRI memory "
                                    "allocated at 0x%lx, "
                                    "handle 0x%lx.\n",
                                    obj->size, obj->offset,
                                    obj->handle));
            }
        } else if (pVia->directRenderingType == DRI_2) {
            struct drm_openchrome_gem_create args;

            memset(&args, 0, sizeof(args));
            args.size = size;
            args.alignment = alignment;
            args.domain = domain;
            ret = drmCommandWriteRead(pVia->drmmode.fd,
                            DRM_OPENCHROME_GEM_CREATE,
                            &args,
                            sizeof(struct drm_openchrome_gem_create));
            if (!ret) {
                /* Okay the X server expects to know the offset because
                 * of non-KMS. Once we have KMS working the offset
                 * will not be valid. */
                obj->size = args.size;
                obj->domain = args.domain;
                obj->handle = args.handle;
                obj->offset = args.offset;
                DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                    "%lu bytes of DRI2 memory "
                                    "allocated at 0x%lx, "
                                    "handle 0x%lx.\n",
                                    obj->size, obj->offset,
                                    obj->handle));
            }
#endif
        }
        break;

    case TTM_PL_SYSTEM:
    default:
        ret = -ENXIO;
        break;
    }

    if (ret) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                            "DRM memory allocation failed.\n"
                            "Error Code: %d\n", ret));
        free(obj);
        obj = NULL;
        goto exit;
    }

exit:
     return obj;
}

void*
drm_bo_map(ScrnInfoPtr pScrn, struct buffer_object *obj)
{
    VIAPtr pVia = VIAPTR(pScrn);
#ifdef OPENCHROMEDRI
    struct drm_openchrome_gem_map args;
    int ret;
#endif /* OPENCHROMEDRI */

    if ((pVia->directRenderingType == DRI_NONE)
#ifdef OPENCHROMEDRI
        || (pVia->directRenderingType == DRI_1)
#endif /* OPENCHROMEDRI */
    ) {
        switch (obj->domain) {
#ifdef OPENCHROMEDRI
        case TTM_PL_TT:
            obj->ptr = (uint8_t*)pVia->agpMappedAddr + obj->offset;
            break;
#endif /* OPENCHROMEDRI */
        case TTM_PL_VRAM:
            obj->ptr = pVia->FBBase + obj->offset;
            break;
        default:
            obj->ptr = NULL;
            break;
        }
#ifdef OPENCHROMEDRI
    } else if (pVia->directRenderingType == DRI_2) {
        memset(&args, 0, sizeof(args));
        args.handle = obj->handle;
        ret = drmCommandWriteRead(pVia->drmmode.fd,
                        DRM_OPENCHROME_GEM_MAP,
                        &args,
                        sizeof(struct drm_openchrome_gem_map));
        if (ret) {
            obj->ptr = NULL;
            goto exit;
        }

        obj->ptr = mmap(0, obj->size, PROT_READ | PROT_WRITE,
                        MAP_SHARED, pVia->drmmode.fd, args.map_offset);
        if (obj->ptr == MAP_FAILED) {
            DEBUG(ErrorF("mmap failed with error %d\n", -errno));
            obj->ptr = NULL;
        }
#endif /* OPENCHROMEDRI */
    }

#ifdef OPENCHROMEDRI
exit:
#endif /* OPENCHROMEDRI */
    return obj->ptr;
}

void
drm_bo_unmap(ScrnInfoPtr pScrn, struct buffer_object *obj)
{
    VIAPtr pVia = VIAPTR(pScrn);
#ifdef OPENCHROMEDRI
    struct drm_openchrome_gem_unmap args;
    int ret;
#endif /* OPENCHROMEDRI */

    if ((pVia->directRenderingType == DRI_NONE)
#ifdef OPENCHROMEDRI
        || (pVia->directRenderingType == DRI_1)
#endif /* OPENCHROMEDRI */
    ) {
#ifdef OPENCHROMEDRI
    } else if (pVia->directRenderingType == DRI_2) {
        munmap(obj->ptr, obj->size);

        memset(&args, 0, sizeof(struct drm_openchrome_gem_unmap));
        args.handle = obj->handle;
        ret = drmCommandRead(pVia->drmmode.fd,
                        DRM_OPENCHROME_GEM_UNMAP,
                        &args,
                        sizeof(struct drm_openchrome_gem_unmap));
        if (ret) {
            goto exit;
        }
#endif /* OPENCHROMEDRI */
    }

#ifdef OPENCHROMEDRI
exit:
#endif /* OPENCHROMEDRI */
    obj->ptr = NULL;
}

void
drm_bo_free(ScrnInfoPtr pScrn, struct buffer_object *obj)
{
    VIAPtr pVia = VIAPTR(pScrn);

    if (obj) {
        DEBUG(ErrorF("Freed %lu (pool %d)\n", obj->offset, obj->domain));
        switch (obj->domain) {
        case TTM_PL_VRAM:
        case TTM_PL_TT:
            if (pVia->directRenderingType == DRI_NONE) {
                if (!pVia->useEXA) {
                    FBLinearPtr linear = (FBLinearPtr) obj->handle;

                    xf86FreeOffscreenLinear(linear);
                } else {
                    ExaOffscreenArea *pArea =
                                    (ExaOffscreenArea *)obj->handle;

                    exaOffscreenFree(pScrn->pScreen, pArea);
                }
#ifdef OPENCHROMEDRI
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
