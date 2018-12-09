/*
 * Copyright 2011 James Simmons, All Rights Reserved.
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
#ifndef _VIA_MEMMGR_H_
#define _VIA_MEMMGR_H_

#include <sys/mman.h>
#include "xf86.h"

#define TTM_PL_FLAG_SYSTEM	1
#define TTM_PL_FLAG_TT		2
#define TTM_PL_FLAG_VRAM	4

struct buffer_object {
    off_t           map_offset;
    unsigned long   handle;
    unsigned long   offset;             /* Offset into fb */
    unsigned long   pitch;
    unsigned long   size;
    void            *ptr;
    int             domain;
};

/* In via_memory.c */
Bool drm_bo_manager_init(ScrnInfoPtr pScrn);

struct buffer_object *
drm_bo_alloc_surface(ScrnInfoPtr pScrn, unsigned int width, unsigned int height,
                    int format, unsigned int alignment, int domain);
struct buffer_object *
drm_bo_alloc(ScrnInfoPtr pScrn, unsigned int size, unsigned int alignment,
                int domain);
void *drm_bo_map(ScrnInfoPtr pScrn, struct buffer_object *obj);
void drm_bo_unmap(ScrnInfoPtr pScrn, struct buffer_object *obj);
void drm_bo_free(ScrnInfoPtr pScrn, struct buffer_object *);

#endif
