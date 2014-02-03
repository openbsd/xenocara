/*
 * Copyright © 2007 Intel Corporation
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
 *    Zhenyu Wang <zhenyu.z.wang@intel.com>
 *
 */
#ifndef I830_HWMC_H
#define I830_HWMC_H

#define INTEL_XVMC_LIBNAME	"IntelXvMC"
#define INTEL_XVMC_MAJOR	0
#define INTEL_XVMC_MINOR	1
#define INTEL_XVMC_PATCHLEVEL	0

#define FOURCC_XVMC     (('C' << 24) + ('M' << 16) + ('V' << 8) + 'X')

/*
 * Commands that client submits through XvPutImage:
 */

#define INTEL_XVMC_COMMAND_DISPLAY      0x00
#define INTEL_XVMC_COMMAND_UNDISPLAY    0x01

/* hw xvmc support type */
#define XVMC_I915_MPEG2_MC	0x01
#define XVMC_I965_MPEG2_MC	0x02
#define XVMC_I945_MPEG2_VLD	0x04
#define XVMC_I965_MPEG2_VLD	0x08

struct intel_xvmc_hw_context {
	unsigned int type;
	union {
		struct {
			unsigned int use_phys_addr : 1;
		} i915;
		struct {
			unsigned int is_g4x:1;
			unsigned int is_965_q:1;
			unsigned int is_igdng:1;
		} i965;
	};
};

/* Intel private XvMC command to DDX driver */
struct intel_xvmc_command {
	uint32_t handle;
};

#ifdef _INTEL_XVMC_SERVER_
#include <xf86xvmc.h>

extern Bool intel_xvmc_adaptor_init(ScreenPtr);
#endif

#endif
