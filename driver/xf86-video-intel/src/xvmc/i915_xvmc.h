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

#include "intel_xvmc.h"
#include "i915_hwmc.h"

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

/*
 * i915XvMCContext:
 *	Private Context data referenced via the privData
 *      pointer in the XvMCContext structure.
 */
typedef struct _i915XvMCContext {
	unsigned int ctxno;
	unsigned int last_flip;
	unsigned int dual_prime;	/* Flag to identify when dual prime is in use. */
	unsigned int yStride;
	unsigned int uvStride;
	unsigned short ref;
	unsigned int depth;
	XvPortID port;		/* Xv Port ID when displaying */
	int haveXv;		/* Have I initialized the Xv
				 * connection for this surface? */
	XvImage *xvImage;	/* Fake Xv Image used for command
				 * buffer transport to the X server */
	GC gc;			/* X GC needed for displaying */
	Drawable draw;		/* Drawable to undisplay from */
	void *drawHash;
	int deviceID;

	intel_xvmc_drm_map_t sis;
	intel_xvmc_drm_map_t msb;
	intel_xvmc_drm_map_t ssb;
	intel_xvmc_drm_map_t psp;
	intel_xvmc_drm_map_t psc;
	intel_xvmc_drm_map_t corrdata;
} i915XvMCContext;

/*
 * i915XvMCSubpicture:
 *  Private data structure for each XvMCSubpicture. This
 *  structure is referenced by the privData pointer in the XvMCSubpicture
 *  structure.
 */
typedef struct _i915XvMCSubpicture {
	unsigned int srfNo;
	unsigned int last_render;
	unsigned int last_flip;
	unsigned int pitch;
	unsigned char palette[3][16];
	intel_xvmc_drm_map_t srf;
	i915XvMCContext *privContext;
} i915XvMCSubpicture;

/* Number of YUV buffers per surface */
#define I830_MAX_BUFS 2

/*
 * i915XvMCSurface: Private data structure for each XvMCSurface. This
 *  structure is referenced by the privData pointer in the XvMCSurface
 *  structure.
 */
typedef struct _i915XvMCSurface {
	unsigned int srfNo;	/* XvMC private surface numbers */
	unsigned int last_render;
	unsigned int last_flip;
	unsigned int yStride;	/* Stride of YUV420 Y component. */
	unsigned int uvStride;
	unsigned int width;	/* Dimensions */
	unsigned int height;
	intel_xvmc_drm_map_t srf;
	i915XvMCContext *privContext;
	i915XvMCSubpicture *privSubPic;	/* Subpicture to be blended when
					 * displaying. NULL if none. */
} i915XvMCSurface;

#endif /* _I915XVMC_H */
