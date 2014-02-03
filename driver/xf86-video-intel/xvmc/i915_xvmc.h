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

#ifndef _I915XVMC_H
#define _I915XVMC_H

#include "intel_xvmc_private.h"

#define I915_SUBPIC_PALETTE_SIZE        16
#define MAX_SUBCONTEXT_LEN              1024

#define PCI_CHIP_I915_G                 0x2582
#define PCI_CHIP_I915_GM                0x2592
#define PCI_CHIP_I945_G                 0x2772
#define PCI_CHIP_I945_GM                0x27A2
#define PCI_CHIP_I945_GME               0x27AE
#define PCI_CHIP_G33_G                  0x29C2
#define PCI_CHIP_Q35_G                  0x29B2
#define PCI_CHIP_Q33_G                  0x29D2

#define CORRDATA_SIZE			128*GTT_PAGE_SIZE
/*
 * i915XvMCContext:
 *	Private Context data referenced via the privData
 *      pointer in the XvMCContext structure.
 */
typedef struct _i915XvMCContext {
	struct intel_xvmc_context comm;
	unsigned int yStride;
	unsigned int uvStride;
	unsigned int use_phys_addr;

	drm_intel_bo *sis_bo;
	drm_intel_bo *msb_bo;
	drm_intel_bo *ssb_bo;
	drm_intel_bo *psp_bo;
	drm_intel_bo *psc_bo;
	drm_intel_bo *corrdata_bo;
} i915XvMCContext;

/*
 * i915XvMCSubpicture:
 *  Private data structure for each XvMCSubpicture. This
 *  structure is referenced by the privData pointer in the XvMCSubpicture
 *  structure.
 */
typedef struct _i915XvMCSubpicture {
	unsigned int srfNo;
	unsigned int pitch;
	unsigned char palette[3][16];
	intel_xvmc_drm_map_t srf;
	i915XvMCContext *privContext;
} i915XvMCSubpicture;

/* Number of YUV buffers per surface */
#define I830_MAX_BUFS 2

#endif /* _I915XVMC_H */
