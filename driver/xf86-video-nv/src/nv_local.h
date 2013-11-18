/*
 * Copyright (c) 1993-2003 NVIDIA, Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __NV_LOCAL_H__
#define __NV_LOCAL_H__

/*
 * This file includes any environment or machine specific values to access the
 * HW.  Put all affected includes, typdefs, etc. here so the riva_hw.* files
 * can stay generic in nature.
 */ 
#include "compiler.h"
#include "xf86_OSproc.h"

#include "shadow.h"
/*
 * Typedefs to force certain sized values.
 */
typedef unsigned char  U008;
typedef unsigned short U016;
typedef unsigned int   U032;

/*
 * HW access macros.  These assume memory-mapped I/O, and not normal I/O space.
 */
#define NV_WR08(p,i,d)  MMIO_OUT8((pointer)(p), (i), (d))
#define NV_RD08(p,i)    MMIO_IN8((pointer)(p), (i))
#define NV_WR16(p,i,d)  MMIO_OUT16((pointer)(p), (i), (d))
#define NV_RD16(p,i)    MMIO_IN16((pointer)(p), (i))
#define NV_WR32(p,i,d)  MMIO_OUT32((pointer)(p), (i), (d))
#define NV_RD32(p,i)    MMIO_IN32((pointer)(p), (i))

/* VGA I/O is now always done through MMIO */
#define VGA_WR08(p,i,d) NV_WR08(p,i,d)
#define VGA_RD08(p,i)   NV_RD08(p,i)

#define NVDmaNext(pNv, data) \
     (pNv)->dmaBase[(pNv)->dmaCurrent++] = (data)

#define NVDmaStart(pNv, tag, size) {          \
     if((pNv)->dmaFree <= (size))             \
        NVDmaWait(pNv, size);                 \
     NVDmaNext(pNv, ((size) << 18) | (tag));  \
     (pNv)->dmaFree -= ((size) + 1);          \
}

#if defined(__i386__)
#define _NV_FENCE() outb(0x3D0, 0);
#else
#define _NV_FENCE() mem_barrier();
#endif

#define WRITE_PUT(pNv, data) {       \
  volatile CARD8 scratch;            \
  _NV_FENCE()                        \
  scratch = (pNv)->FbStart[0];       \
  (pNv)->FIFO[0x0010] = (data) << 2; \
  mem_barrier();                     \
}

#define READ_GET(pNv) ((pNv)->FIFO[0x0011] >> 2)


#endif /* __NV_LOCAL_H__ */
