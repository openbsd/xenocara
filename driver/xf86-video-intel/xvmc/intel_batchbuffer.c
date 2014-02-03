/**************************************************************************
 *
 * Copyright 2003 Tungsten Graphics, Inc., Cedar Park, Texas.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>

#include <sys/ioctl.h>
#include <X11/Xlibint.h>
#include <fourcc.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/XvMC.h>
#include <X11/extensions/XvMClib.h>

#include "intel_xvmc_private.h"
#include "intel_batchbuffer.h"
#include "brw_defines.h"
#include "brw_structs.h"
#define MI_BATCH_BUFFER_END     (0xA << 23)
#define BATCH_SIZE 8*1024	/* one bo is allocated each time, so the size can be small */

static void i965_end_batch(void)
{
	unsigned int size = xvmc_driver->batch.ptr -
	    xvmc_driver->batch.init_ptr;
	if ((size & 4) == 0) {
		*(unsigned int *)xvmc_driver->batch.ptr = 0;
		xvmc_driver->batch.ptr += 4;
	}
	*(unsigned int *)xvmc_driver->batch.ptr = MI_BATCH_BUFFER_END;
	xvmc_driver->batch.ptr += 4;
}

static void reset_batch(void)
{
	dri_bo *bo = xvmc_driver->batch.buf;

	xvmc_driver->batch.ptr = xvmc_driver->batch.init_ptr = bo->virtual;
	xvmc_driver->batch.size = bo->size;
	xvmc_driver->batch.space = bo->size - 8;
}

Bool intelInitBatchBuffer(void)
{
	if ((xvmc_driver->batch.buf =
	     drm_intel_bo_alloc(xvmc_driver->bufmgr,
				"batch buffer", BATCH_SIZE, 0x1000)) == NULL) {
		fprintf(stderr, "unable to alloc batch buffer\n");
		return False;
	}

	if (drm_intel_gem_bo_map_gtt(xvmc_driver->batch.buf)) {
		drm_intel_bo_unreference(xvmc_driver->batch.buf);
		return False;
	}

	reset_batch();
	return True;
}

void intelFiniBatchBuffer(void)
{
	if (xvmc_driver->batch.buf == NULL)
		return;

	drm_intel_bo_unreference(xvmc_driver->batch.buf);
}

void intelFlushBatch(void)
{
	dri_bo *bo;

	i965_end_batch();

	drm_intel_bo_exec(xvmc_driver->batch.buf,
			  xvmc_driver->batch.ptr - xvmc_driver->batch.init_ptr,
			  0, 0, 0);

	bo = drm_intel_bo_alloc(xvmc_driver->bufmgr,
				"batch buffer", BATCH_SIZE, 0x1000);
	if (bo != NULL && drm_intel_gem_bo_map_gtt(bo) == 0) {
		drm_intel_bo_unreference(xvmc_driver->batch.buf);
		xvmc_driver->batch.buf = bo;
	} else {
		if (bo != NULL)
			drm_intel_bo_unreference(bo);
		drm_intel_gem_bo_map_gtt(xvmc_driver->batch.buf);
	}

	reset_batch();
}

void intelBatchbufferData(const void *data, unsigned bytes, unsigned flags)
{
	assert(bytes <= xvmc_driver->batch.space);
	memcpy(xvmc_driver->batch.ptr, data, bytes);
	xvmc_driver->batch.ptr += bytes;
	xvmc_driver->batch.space -= bytes;
}

void intel_batch_emit_reloc(dri_bo * bo, uint32_t read_domain,
			    uint32_t write_domain, uint32_t delta,
			    unsigned char *ptr)
{
	drm_intel_bo_emit_reloc(xvmc_driver->batch.buf,
				ptr - xvmc_driver->batch.init_ptr, bo, delta,
				read_domain, write_domain);
}
