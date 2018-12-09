/*
 * Copyright (C) Thomas Hellstrom (2005)
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

#ifndef VIA_DMABUFFER_H
#define VIA_DMABUFFER_H

#include "via_3d_reg.h"

typedef struct _ViaCommandBuffer
{
    ScrnInfoPtr pScrn;
    CARD32 *buf;
    CARD32 waitFlags;
    unsigned pos;
    unsigned bufSize;
    int mode;
    int header_start;
    int rindex;
    Bool has3dState;
    void (*flushFunc) (struct _ViaCommandBuffer * cb);
} ViaCommandBuffer;

#define VIA_DMASIZE 16384

#define H1_ADDR(val) (((val) >> 2) | 0xF0000000)
#define WAITFLAGS(flags)			\
    (cb)->waitFlags |= (flags)

#define BEGIN_RING(size)					\
    do {								\
	if (cb->flushFunc && (cb->pos > (cb->bufSize-(size)))) {	\
	    cb->flushFunc(cb);					\
	}								\
    } while(0)

#define BEGIN_H2(paraType, h2size)			\
  do{							\
    BEGIN_RING((h2size)+6);				\
    if (cb->mode == 2 && (paraType) == cb->rindex)	\
      break;						\
    if (cb->pos & 1)					\
      OUT_RING(HC_DUMMY);				\
    cb->header_start = cb->pos;				\
    cb->rindex = paraType;				\
    cb->mode = 2;					\
    OUT_RING(HALCYON_HEADER2);				\
    OUT_RING((paraType) << 16);						\
    if (!cb->has3dState && ((paraType) != HC_ParaType_CmdVdata)) {	\
      cb->has3dState = TRUE;						\
    }									\
  } while(0);

#define OUT_RING(val) do{	\
	(cb)->buf[(cb)->pos++] = (val);	\
    } while(0);

#define OUT_RING_QW(val1, val2)			\
    do {						\
	(cb)->buf[(cb)->pos++] = (val1);		\
	(cb)->buf[(cb)->pos++] = (val2);		\
    } while (0)

#define ADVANCE_RING \
  cb->flushFunc(cb)

#define RING_VARS \
  ViaCommandBuffer *cb = &pVia->cb

#define OUT_RING_H1(val1, val2) \
  OUT_RING_QW(H1_ADDR(val1), val2)

#define OUT_RING_SubA(val1, val2) \
  OUT_RING(((val1) << HC_SubA_SHIFT) | ((val2) & HC_Para_MASK))

#endif
