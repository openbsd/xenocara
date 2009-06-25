/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
Copyright © 2002 David Dawes

All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

#ifndef _INTEL_RING_H
#define _INTEL_RING_H

#define OUT_RING(n) do {						\
    if (I810_DEBUG & DEBUG_VERBOSE_RING)				\
	ErrorF("OUT_RING 0x%08x: 0x%08x, (mask %x)\n",			\
	       pI830->ring_next, (unsigned int)(n),			\
	       pI830->ring.tail_mask);					\
    *(volatile uint32_t *)(pI830->ring.virtual_start +			\
			   pI830->ring_next) = n;			\
    pI830->ring_used += 4;						\
    pI830->ring_next += 4;						\
    pI830->ring_next &= pI830->ring.tail_mask;				\
} while (0)

#define OUT_RING_F(x) do {			\
	union intfloat tmp;			\
	tmp.f = (float)(x);			\
	OUT_RING(tmp.ui);			\
} while(0)

#define ADVANCE_LP_RING() do {						\
    if (pI830->ring_emitting == 0)					\
	FatalError("%s: ADVANCE_LP_RING called with no matching "	\
		   "BEGIN_LP_RING\n", __FUNCTION__);			\
    if (pI830->ring_used > pI830->ring_emitting)			\
	FatalError("%s: ADVANCE_LP_RING: exceeded allocation %d/%d\n ",	\
		   __FUNCTION__, pI830->ring_used,			\
		   pI830->ring_emitting);				\
    if (pI830->ring_used < pI830->ring_emitting)			\
	FatalError("%s: ADVANCE_LP_RING: under-used allocation %d/%d\n ", \
		   __FUNCTION__, pI830->ring_used,			\
		   pI830->ring_emitting);				\
    pI830->ring.tail = pI830->ring_next;				\
    pI830->ring.space -= pI830->ring_used;				\
    if (pI830->ring_next & 0x07)					\
	FatalError("%s: ADVANCE_LP_RING: "				\
		   "ring_next (0x%x) isn't on a QWord boundary\n",	\
		   __FUNCTION__, pI830->ring_next);			\
    OUTREG(LP_RING + RING_TAIL, pI830->ring_next);			\
    pI830->ring_emitting = 0;						\
} while (0)

#define BEGIN_LP_RING(n)						\
do {									\
    if (pI830->ring_emitting != 0)					\
	FatalError("%s: BEGIN_LP_RING called without closing "		\
		   "ADVANCE_LP_RING\n", __FUNCTION__);			\
    if ((n) > 2 && (I810_DEBUG&DEBUG_ALWAYS_SYNC))			\
	i830_wait_ring_idle(pScrn);					\
    pI830->ring_emitting = (n) * 4;					\
    if ((n) & 1)							\
	pI830->ring_emitting += 4;					\
    if (pI830->ring.space < pI830->ring_emitting)			\
	WaitRingFunc(pScrn, pI830->ring_emitting, 0);			\
    pI830->ring_next = pI830->ring.tail;				\
    if (I810_DEBUG & DEBUG_VERBOSE_RING)				\
	ErrorF( "BEGIN_LP_RING %d in %s\n", n, FUNCTION_NAME);		\
    pI830->ring_used = 0;						\
    if ((n) & 1)							\
	OUT_RING(MI_NOOP);						\
} while (0)

#endif /* _INTEL_RING_H */
