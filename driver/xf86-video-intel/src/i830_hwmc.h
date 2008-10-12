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

/* supported surface types */
enum {
    SURFACE_TYPE_MPEG2_MPML = FOURCC_XVMC, /* mpeg2 MP@ML */
    SURFACE_TYPE_MPEG1_MPML,		   /* mpeg1 MP@ML */
    SURFACE_TYPE_MAX
};

/* common header for context private */
struct hwmc_buffer
{
    drm_handle_t handle;
    unsigned long offset;
    unsigned long size;
    unsigned long bus_addr;
};

struct _intel_xvmc_common {
    unsigned int type;
    unsigned int sarea_size;
    struct hwmc_buffer batchbuffer;
};

/* Intel private XvMC command to DDX driver */
struct intel_xvmc_command {
    unsigned int command;
    unsigned int ctxNo;
    unsigned int srfNo;
    unsigned int subPicNo;
    unsigned int flags;
    unsigned int real_id;
    unsigned int pad[6];
};

#ifdef _INTEL_XVMC_SERVER_
#include <xf86xvmc.h>

struct intel_xvmc_driver {
    char *name;
    XF86MCAdaptorPtr adaptor;
    unsigned int flag;
    i830_memory *batch;
    drm_handle_t batch_handle;

    /* more items for xvmv surface manage? */
    Bool (*init)(ScrnInfoPtr, XF86VideoAdaptorPtr);
    void (*fini)(ScrnInfoPtr);
    void* devPrivate;
};

extern struct intel_xvmc_driver *xvmc_driver;
extern struct intel_xvmc_driver i915_xvmc_driver;
/* extern struct intel_xvmc_driver i965_xvmc_driver; */

extern Bool intel_xvmc_set_driver(struct intel_xvmc_driver *);
extern Bool intel_xvmc_probe(ScrnInfoPtr);
extern Bool intel_xvmc_driver_init(ScreenPtr, XF86VideoAdaptorPtr);
extern Bool intel_xvmc_screen_init(ScreenPtr);
extern void intel_xvmc_finish(ScrnInfoPtr);
extern int  intel_xvmc_put_image_size(ScrnInfoPtr);
extern Bool intel_xvmc_init_batch(ScrnInfoPtr);
extern void intel_xvmc_fini_batch(ScrnInfoPtr);
#endif

#endif
