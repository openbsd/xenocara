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
	ErrorF("OUT_RING %lx: %x, (mask %x)\n",				\
	       (unsigned long)(outring), (unsigned int)(n), ringmask);	\
    *(volatile unsigned int *)(virt + outring) = n;			\
    outring += 4; ringused += 4;					\
    outring &= ringmask;						\
} while (0)

#define ADVANCE_LP_RING() do {						\
    if (ringused > needed)						\
	FatalError("%s: ADVANCE_LP_RING: exceeded allocation %d/%d\n ",	\
		   __FUNCTION__, ringused, needed);   			\
    else if (ringused < needed)						\
	FatalError("%s: ADVANCE_LP_RING: under-used allocation %d/%d\n ", \
		   __FUNCTION__, ringused, needed);   			\
    pI810->LpRing->tail = outring;					\
    pI810->LpRing->space -= ringused;					\
    if (outring & 0x07)							\
	FatalError("%s: ADVANCE_LP_RING: "				\
		   "outring (0x%x) isn't on a QWord boundary\n",	\
		   __FUNCTION__, outring);				\
    OUTREG(LP_RING + RING_TAIL, outring);				\
} while (0)

/*
 * XXX Note: the head/tail masks are different for 810 and i830.
 * If the i810 always sets the higher bits to 0, then this shouldn't be
 * a problem.  Check this!
 */
#define DO_RING_IDLE() do {						\
    int _head;								\
    int _tail;								\
    do {								\
	_head = INREG(LP_RING + RING_HEAD) & I830_HEAD_MASK;		\
	_tail = INREG(LP_RING + RING_TAIL) & I830_TAIL_MASK;		\
	DELAY(10);							\
    } while (_head != _tail);						\
} while( 0)

#define BEGIN_LP_RING(n)						\
    unsigned int outring, ringmask, ringused = 0;			\
    volatile unsigned char *virt;					\
    int needed;								\
    if ((n) & 1)							\
	ErrorF("BEGIN_LP_RING called with odd argument: %d\n", n);	\
    if ((n) > 2 && (I810_DEBUG&DEBUG_ALWAYS_SYNC))			\
	DO_RING_IDLE();							\
    needed = (n) * 4;							\
    if (pI810->LpRing->space < needed)					\
	WaitRingFunc(pScrn, needed, 0);					\
    outring = pI810->LpRing->tail;					\
    ringmask = pI810->LpRing->tail_mask;				\
    virt = pI810->LpRing->virtual_start;				\
    if (I810_DEBUG & DEBUG_VERBOSE_RING)				\
	ErrorF( "BEGIN_LP_RING %d in %s\n", n, FUNCTION_NAME);

#endif /* _INTEL_RING_H */
