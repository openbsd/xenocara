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
#ifndef INTEL_XVMC_H
#define INTEL_XVMC_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <stdint.h>

#include <xf86drm.h>
#include <X11/X.h>
#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include <fourcc.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/XvMC.h>
#include <X11/extensions/XvMClib.h>
#include <X11/extensions/vldXvMC.h>
#include <drm_sarea.h>

#include "i915_drm.h"
#include "intel_bufmgr.h"

#include "intel_xvmc.h"
#include "intel_batchbuffer.h"

#define GTT_PAGE_SIZE 4*1024

#define XVMC_ERR(s, arg...)					\
    do {							\
	fprintf(stderr, "[intel_xvmc] err: " s "\n", ##arg);	\
    } while (0)

#define XVMC_INFO(s, arg...)					\
    do {							\
	fprintf(stderr, "[intel_xvmc] info: " s "\n", ##arg);	\
    } while (0)

/* Subpicture fourcc */
#define FOURCC_IA44 0x34344149

/*
  Definitions for temporary wire protocol hooks to be replaced
  when a HW independent libXvMC is created.
*/
extern Status _xvmc_create_context(Display * dpy, XvMCContext * context,
				   int *priv_count, CARD32 ** priv_data);

extern Status _xvmc_destroy_context(Display * dpy, XvMCContext * context);

extern Status _xvmc_create_surface(Display * dpy, XvMCContext * context,
				   XvMCSurface * surface, int *priv_count,
				   CARD32 ** priv_data);

extern Status _xvmc_destroy_surface(Display * dpy, XvMCSurface * surface);

extern Status _xvmc_create_subpicture(Display * dpy, XvMCContext * context,
				      XvMCSubpicture * subpicture,
				      int *priv_count, uint ** priv_data);

extern Status _xvmc_destroy_subpicture(Display * dpy,
				       XvMCSubpicture * subpicture);

struct intel_xvmc_context {
	struct intel_xvmc_hw_context *hw;
	uint32_t surface_bo_size;
	drm_context_t hw_context;	/* context id to kernel drm */
};
typedef struct intel_xvmc_context *intel_xvmc_context_ptr;

struct intel_xvmc_surface {
	XvMCContext *context;
	XvImage *image;
	GC gc;
	Bool gc_init;
	Drawable last_draw;
	drm_intel_bo *bo;
	uint32_t gem_handle;
};
typedef struct intel_xvmc_surface *intel_xvmc_surface_ptr;

typedef struct _intel_xvmc_drm_map {
	drm_handle_t handle;
	unsigned long offset;
	unsigned long size;
	unsigned long bus_addr;
	drmAddress map;
} intel_xvmc_drm_map_t, *intel_xvmc_drm_map_ptr;

typedef struct _intel_xvmc_driver {
	int type;		/* hw xvmc type - i830_hwmc.h */
	int screen;		/* current screen num */

	int fd;			/* drm file handler */

	dri_bufmgr *bufmgr;

	struct {
		unsigned int init_offset;
		unsigned int size;
		unsigned int space;
		unsigned char *ptr;
		unsigned char *init_ptr;
		dri_bo *buf;
	} batch;

	struct {
		void *ptr;
		unsigned int size;
		unsigned int offset;
		unsigned int active_buf;
		unsigned int irq_emitted;
	} alloc;
	intel_xvmc_drm_map_t batchbuffer;

	sigset_t sa_mask;
	pthread_mutex_t ctxmutex;
	int locked;

	int num_ctx;
	intel_xvmc_context_ptr ctx_list;
	int num_surf;
	struct intel_xvmc_surface * surf_list;

	void *private;

	/* driver specific xvmc callbacks */
	 Status(*create_context) (Display * display, XvMCContext * context,
				  int priv_count, CARD32 * priv_data);

	 Status(*destroy_context) (Display * display, XvMCContext * context);

	 Status(*render_surface) (Display * display, XvMCContext * context,
				  unsigned int picture_structure,
				  XvMCSurface * target_surface,
				  XvMCSurface * past_surface,
				  XvMCSurface * future_surface,
				  unsigned int flags,
				  unsigned int num_macroblocks,
				  unsigned int first_macroblock,
				  XvMCMacroBlockArray * macroblock_array,
				  XvMCBlockArray * blocks);

	 Status(*begin_surface) (Display * display, XvMCContext * context,
				 XvMCSurface * target_surface,
				 XvMCSurface * past_surface,
				 XvMCSurface * future_surface,
				 const XvMCMpegControl * control);
	 Status(*load_qmatrix) (Display * display, XvMCContext * context,
				const XvMCQMatrix * qmx);
	 Status(*put_slice) (Display * display, XvMCContext * context,
			     unsigned char *slice, int bytes);
	 Status(*put_slice2) (Display * display, XvMCContext * context,
			      unsigned char *slice, int bytes, int slice_code);

} intel_xvmc_driver_t, *intel_xvmc_driver_ptr;

extern struct _intel_xvmc_driver i915_xvmc_mc_driver;
extern struct _intel_xvmc_driver i965_xvmc_mc_driver;
extern struct _intel_xvmc_driver xvmc_vld_driver;
extern struct _intel_xvmc_driver *xvmc_driver;

#define SET_BLOCKED_SIGSET()   do {    \
        sigset_t bl_mask;                       \
        sigfillset(&bl_mask);           \
        sigdelset(&bl_mask, SIGFPE);    \
        sigdelset(&bl_mask, SIGILL);    \
        sigdelset(&bl_mask, SIGSEGV);   \
        sigdelset(&bl_mask, SIGBUS);    \
        sigdelset(&bl_mask, SIGKILL);   \
        pthread_sigmask(SIG_SETMASK, &bl_mask, &xvmc_driver->sa_mask); \
    } while (0)

#define RESTORE_BLOCKED_SIGSET() do {    \
        pthread_sigmask(SIG_SETMASK, &xvmc_driver->sa_mask, NULL); \
    } while (0)

#define PPTHREAD_MUTEX_LOCK() do {             \
        SET_BLOCKED_SIGSET();                  \
        pthread_mutex_lock(&xvmc_driver->ctxmutex);       \
    } while (0)

#define PPTHREAD_MUTEX_UNLOCK() do {           \
        pthread_mutex_unlock(&xvmc_driver->ctxmutex);     \
        RESTORE_BLOCKED_SIGSET();              \
    } while (0)

extern void LOCK_HARDWARE(drm_context_t);
extern void UNLOCK_HARDWARE(drm_context_t);

static inline const char *intel_xvmc_decoder_string(int flag)
{
	switch (flag) {
	case XVMC_I915_MPEG2_MC:
		return "i915/945 MPEG2 MC decoder";
	case XVMC_I965_MPEG2_MC:
		return "i965 MPEG2 MC decoder";
	case XVMC_I945_MPEG2_VLD:
		return "i945 MPEG2 VLD decoder";
	case XVMC_I965_MPEG2_VLD:
		return "i965 MPEG2 VLD decoder";
	default:
		return "Unknown decoder";
	}
}

extern unsigned int mb_bytes_420[64];

/* dump function */
extern void intel_xvmc_dump_open(void);
extern void intel_xvmc_dump_close(void);
extern void intel_xvmc_dump_render(XvMCContext * context,
				   unsigned int picture_structure,
				   XvMCSurface * target_surface,
				   XvMCSurface * past_surface,
				   XvMCSurface * future_surface,
				   unsigned int flags,
				   unsigned int num_macroblocks,
				   unsigned int first_macroblock,
				   XvMCMacroBlockArray * macroblock_array,
				   XvMCBlockArray * blocks);

#define	VFE_GENERIC_MODE	0x0
#define	VFE_VLD_MODE		0x1
#define VFE_IS_MODE		0x2
#define VFE_AVC_MC_MODE		0x4
#define VFE_AVC_IT_MODE		0x7
#define VFE_VC1_IT_MODE		0x7

#endif
