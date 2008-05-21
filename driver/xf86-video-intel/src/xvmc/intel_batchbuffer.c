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

#include "intel_xvmc.h"
#include "intel_batchbuffer.h"

#define MI_BATCH_BUFFER_END     (0xA << 23)

static int intelEmitIrqLocked(void)
{
   drmI830IrqEmit ie;
   int ret, seq;

   ie.irq_seq = &seq;
   ret = drmCommandWriteRead(xvmc_driver->fd, DRM_I830_IRQ_EMIT,
                             &ie, sizeof(ie));

   if ( ret ) {
      fprintf(stderr, "%s: drmI830IrqEmit: %d\n", __FUNCTION__, ret);
      exit(1);
   }

   return seq;
}

static void intelWaitIrq(int seq)
{
   int ret;
   drmI830IrqWait iw;

   iw.irq_seq = seq;

   do {
      ret = drmCommandWrite(xvmc_driver->fd, DRM_I830_IRQ_WAIT, &iw, sizeof(iw) );
   } while (ret == -EAGAIN || ret == -EINTR);

   if (ret) {
      fprintf(stderr, "%s: drmI830IrqWait: %d\n", __FUNCTION__, ret);
      exit(1);
   }
}

static void intelDestroyBatchBuffer(void)
{
   if (xvmc_driver->alloc.offset) {
       xvmc_driver->alloc.ptr = NULL;
       xvmc_driver->alloc.offset = 0;
   } else if (xvmc_driver->alloc.ptr) {
      free(xvmc_driver->alloc.ptr);
      xvmc_driver->alloc.ptr = NULL;
   }

   memset(&xvmc_driver->batch, 0, sizeof(xvmc_driver->batch));
}


Bool intelInitBatchBuffer(void)
{
    if (drmMap(xvmc_driver->fd,
		xvmc_driver->batchbuffer.handle,
		xvmc_driver->batchbuffer.size,
		(drmAddress *)&xvmc_driver->batchbuffer.map) != 0) {
	XVMC_ERR("fail to map batch buffer\n");
	return False;
    }

    if (xvmc_driver->batchbuffer.map) {
	xvmc_driver->alloc.size = xvmc_driver->batchbuffer.size;
	xvmc_driver->alloc.offset = xvmc_driver->batchbuffer.offset;
	xvmc_driver->alloc.ptr = xvmc_driver->batchbuffer.map;
    } else {
	xvmc_driver->alloc.size = 8 * 1024;
	xvmc_driver->alloc.offset = 0;
	xvmc_driver->alloc.ptr = malloc(xvmc_driver->alloc.size);
    }

    xvmc_driver->alloc.active_buf = 0;
    assert(xvmc_driver->alloc.ptr);
    return True;
}

void intelFiniBatchBuffer(void)
{
    if (xvmc_driver->batchbuffer.map) {
        drmUnmap(xvmc_driver->batchbuffer.map, xvmc_driver->batchbuffer.size);
        xvmc_driver->batchbuffer.map = NULL;
    }
    intelDestroyBatchBuffer();
}

static void intelBatchbufferRequireSpace(unsigned int sz)
{
   if (xvmc_driver->batch.space < sz)
      intelFlushBatch(TRUE);
}

void intelBatchbufferData(const void *data, unsigned bytes, unsigned flags)
{
   assert((bytes & 0x3) == 0);

   intelBatchbufferRequireSpace(bytes);
   memcpy(xvmc_driver->batch.ptr, data, bytes);
   xvmc_driver->batch.ptr += bytes;
   xvmc_driver->batch.space -= bytes;

   assert(xvmc_driver->batch.space >= 0);
}

#define MI_FLUSH                ((0 << 29) | (4 << 23))
#define FLUSH_MAP_CACHE         (1 << 0)
#define FLUSH_RENDER_CACHE      (0 << 2)
#define FLUSH_WRITE_DIRTY_STATE (1 << 4)

static void intelRefillBatchLocked(Bool allow_unlock)
{
   unsigned half = xvmc_driver->alloc.size >> 1;
   unsigned buf = (xvmc_driver->alloc.active_buf ^= 1);
   unsigned dword[2];

   dword[0] = MI_FLUSH | FLUSH_WRITE_DIRTY_STATE | FLUSH_RENDER_CACHE | FLUSH_MAP_CACHE;
   dword[1] = 0;
   intelCmdIoctl((char *)&dword[0], sizeof(dword));
   xvmc_driver->alloc.irq_emitted = intelEmitIrqLocked();

   if (xvmc_driver->alloc.irq_emitted) {
       intelWaitIrq(xvmc_driver->alloc.irq_emitted);
   }

   xvmc_driver->batch.start_offset = xvmc_driver->alloc.offset + buf * half;
   xvmc_driver->batch.ptr = (unsigned char *)xvmc_driver->alloc.ptr + buf * half;
   xvmc_driver->batch.size = half - 8;
   xvmc_driver->batch.space = half - 8;
   assert(xvmc_driver->batch.space >= 0);
}


static void intelFlushBatchLocked(Bool ignore_cliprects,
				  Bool refill,
				  Bool allow_unlock)
{
   drmI830BatchBuffer batch;

   if (xvmc_driver->batch.space != xvmc_driver->batch.size) {

      batch.start = xvmc_driver->batch.start_offset;
      batch.used = xvmc_driver->batch.size - xvmc_driver->batch.space;
      batch.cliprects = 0;
      batch.num_cliprects = 0;
      batch.DR1 = 0;
      batch.DR4 = 0;

      if (xvmc_driver->alloc.offset) {
          if ((batch.used & 0x4) == 0) {
              ((int *)xvmc_driver->batch.ptr)[0] = 0;
              ((int *)xvmc_driver->batch.ptr)[1] = MI_BATCH_BUFFER_END;
              batch.used += 0x8;
              xvmc_driver->batch.ptr += 0x8;
          } else {
              ((int *)xvmc_driver->batch.ptr)[0] = MI_BATCH_BUFFER_END;
              batch.used += 0x4;
              xvmc_driver->batch.ptr += 0x4;
          }
      }

      xvmc_driver->batch.start_offset += batch.used;
      xvmc_driver->batch.size -= batch.used;

      if (xvmc_driver->batch.size < 8) {
         refill = TRUE;
         xvmc_driver->batch.space = xvmc_driver->batch.size = 0;
      }
      else {
         xvmc_driver->batch.size -= 8;
         xvmc_driver->batch.space = xvmc_driver->batch.size;
      }

      assert(xvmc_driver->batch.space >= 0);
      assert(batch.start >= xvmc_driver->alloc.offset);
      assert(batch.start < xvmc_driver->alloc.offset + xvmc_driver->alloc.size);
      assert(batch.start + batch.used > xvmc_driver->alloc.offset);
      assert(batch.start + batch.used <= xvmc_driver->alloc.offset + xvmc_driver->alloc.size);

      if (xvmc_driver->alloc.offset) {
          if (drmCommandWrite(xvmc_driver->fd, DRM_I830_BATCHBUFFER, &batch, sizeof(batch))) {
              fprintf(stderr, "DRM_I830_BATCHBUFFER: %d\n",  -errno);
              exit(1);
          }
      } else {
         drmI830CmdBuffer cmd;
         cmd.buf = (char *)xvmc_driver->alloc.ptr + batch.start;
         cmd.sz = batch.used;
         cmd.DR1 = batch.DR1;
         cmd.DR4 = batch.DR4;
         cmd.num_cliprects = batch.num_cliprects;
         cmd.cliprects = batch.cliprects;

         if (drmCommandWrite(xvmc_driver->fd, DRM_I830_CMDBUFFER, 
                             &cmd, sizeof(cmd))) {
            fprintf(stderr, "DRM_I915_CMDBUFFER: %d\n",  -errno);
            exit(1);
         }
      }
   }

   if (refill)
      intelRefillBatchLocked(allow_unlock);
}

void intelFlushBatch(Bool refill )
{
   intelFlushBatchLocked(FALSE, refill, TRUE);
}

void intelCmdIoctl(char *buf, unsigned used)
{
   drmI830CmdBuffer cmd;

   cmd.buf = buf;
   cmd.sz = used;
   cmd.cliprects = 0;
   cmd.num_cliprects = 0;
   cmd.DR1 = 0;
   cmd.DR4 = 0;

   if (drmCommandWrite(xvmc_driver->fd, DRM_I830_CMDBUFFER, 
                       &cmd, sizeof(cmd))) {
      fprintf(stderr, "DRM_I830_CMDBUFFER: %d\n",  -errno);
      exit(1);
   }
}
