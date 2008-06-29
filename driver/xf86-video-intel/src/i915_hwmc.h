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
 *    Xiang Haihao <haihao.xiang@intel.com>
 *
 */
#ifndef _I915_HWMC_H
#define _I915_HWMC_H

#include "i830_hwmc.h"

#define STRIDE(w)               (((w) + 0x3ff) & ~0x3ff)
#define SIZE_Y420(w, h)         (h * STRIDE(w))
#define SIZE_UV420(w, h)        ((h >> 1) * STRIDE(w >> 1))
#define SIZE_YUV420(w, h)       (SIZE_Y420(w,h) + SIZE_UV420(w,h) * 2)
#define SIZE_XX44(w, h)         (h * STRIDE(w))

#define I915_NUM_XVMC_ATTRIBUTES       0x02
#define I915_XVMC_VALID 0x80000000

typedef struct
{
    struct _intel_xvmc_common comm;
    unsigned int ctxno; /* XvMC private context reference number */
    struct hwmc_buffer sis;
    struct hwmc_buffer ssb;
    struct hwmc_buffer msb;
    struct hwmc_buffer psp;
    struct hwmc_buffer psc;
    struct hwmc_buffer corrdata;/* Correction Data Buffer */
    unsigned int sarea_priv_offset;
    int deviceID;
} I915XvMCCreateContextRec;

typedef struct 
{
    unsigned int srfno;
    struct hwmc_buffer srf;
} I915XvMCCreateSurfaceRec;

#endif /* _I915_HWMC_H */
