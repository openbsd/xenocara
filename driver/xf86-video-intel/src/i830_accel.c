/*
 * XXX So far, for GXxor this is about 40% of the speed of SW, but CPU
 * utilisation falls from 95% to < 5%.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
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
/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/i810/i830_accel.c,v 1.8 2003/04/24 18:00:24 eich Exp $ */

/*
 * Reformatted with GNU indent (2.2.8), using the following options:
 *
 *    -bad -bap -c41 -cd0 -ncdb -ci6 -cli0 -cp0 -ncs -d0 -di3 -i3 -ip3 -l78
 *    -lp -npcs -psl -sob -ss -br -ce -sc -hnl
 *
 * This provides a good match with the original i810 code and preferred
 * XFree86 formatting conventions.
 *
 * When editing this driver, please follow the existing formatting, and edit
 * with <TAB> characters expanded at 8-column intervals.
 */

/*
 * Authors:
 *   Keith Whitwell <keith@tungstengraphics.com>
 *
 */

#include "xf86.h"
#include "xaarop.h"
#include "i830.h"
#include "i810_reg.h"
#include "i830_debug.h"

unsigned long
intel_get_pixmap_offset(PixmapPtr pPix)
{
    ScreenPtr pScreen = pPix->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);

#ifdef I830_USE_EXA
    if (pI830->useEXA)
	return exaGetPixmapOffset(pPix);
#endif
    return (unsigned long)pPix->devPrivate.ptr - (unsigned long)pI830->FbBase;
}

unsigned long
intel_get_pixmap_pitch(PixmapPtr pPix)
{
    ScreenPtr pScreen = pPix->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);

#ifdef I830_USE_EXA
    if (pI830->useEXA)
	return exaGetPixmapPitch(pPix);
#endif
#ifdef I830_USE_XAA
    return (unsigned long)pPix->devKind;
#endif
}

int
I830WaitLpRing(ScrnInfoPtr pScrn, int n, int timeout_millis)
{
   I830Ptr pI830 = I830PTR(pScrn);
   I830RingBuffer *ring = pI830->LpRing;
   int iters = 0;
   unsigned int start = 0;
   unsigned int now = 0;
   int last_head = 0;
   unsigned int first = 0;

   /* If your system hasn't moved the head pointer in 2 seconds, I'm going to
    * call it crashed.
    */
   if (timeout_millis == 0)
      timeout_millis = 2000;

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL) {
      ErrorF("I830WaitLpRing %d\n", n);
      first = GetTimeInMillis();
   }

   while (ring->space < n) {
      ring->head = INREG(LP_RING + RING_HEAD) & I830_HEAD_MASK;
      ring->space = ring->head - (ring->tail + 8);

      if (ring->space < 0)
	 ring->space += ring->mem->size;

      iters++;
      now = GetTimeInMillis();
      if (start == 0 || now < start || ring->head != last_head) {
	 if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	    if (now > start)
	       ErrorF("space: %d wanted %d\n", ring->space, n);
	 start = now;
	 last_head = ring->head;
      } else if (now - start > timeout_millis) {
	 ErrorF("Error in I830WaitLpRing(), timeout for %d seconds\n",
		timeout_millis/1000);
	 if (IS_I965G(pI830))
	     i965_dump_error_state(pScrn);
	 else
	     i830_dump_error_state(pScrn);
	 ErrorF("space: %d wanted %d\n", ring->space, n);
#ifdef XF86DRI
	 if (pI830->directRenderingEnabled) {
	    DRIUnlock(screenInfo.screens[pScrn->scrnIndex]);
	    DRICloseScreen(screenInfo.screens[pScrn->scrnIndex]);
	 }
#endif
#ifdef I830_USE_XAA
	 pI830->AccelInfoRec = NULL;	/* Stops recursive behavior */
#endif
#ifdef I830_USE_EXA
	 pI830->EXADriverPtr = NULL;
#endif
	 FatalError("lockup\n");
      }

      DELAY(10);
   }

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL) {
      now = GetTimeInMillis();
      if (now - first) {
	 ErrorF("Elapsed %u ms\n", now - first);
	 ErrorF("space: %d wanted %d\n", ring->space, n);
      }
   }

   return iters;
}

void
I830Sync(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   int flags = MI_WRITE_DIRTY_STATE | MI_INVALIDATE_MAP_CACHE;

   if (I810_DEBUG & (DEBUG_VERBOSE_ACCEL | DEBUG_VERBOSE_SYNC))
      ErrorF("I830Sync\n");

#ifdef XF86DRI
   /* VT switching tries to do this.
    */
   if (!pI830->LockHeld && pI830->directRenderingEnabled) {
      return;
   }
#endif

   if (pI830->entityPrivate && !pI830->entityPrivate->RingRunning) return;

   if (IS_I96X(pI830))
      flags = 0;

   /* Send a flush instruction and then wait till the ring is empty.
    * This is stronger than waiting for the blitter to finish as it also
    * flushes the internal graphics caches.
    */
   
   {
      BEGIN_LP_RING(2);
      OUT_RING(MI_FLUSH | flags);
      OUT_RING(MI_NOOP);		/* pad to quadword */
      ADVANCE_LP_RING();
   }

   I830WaitLpRing(pScrn, pI830->LpRing->mem->size - 8, 0);

   pI830->LpRing->space = pI830->LpRing->mem->size - 8;
   pI830->nextColorExpandBuf = 0;
}

void
I830EmitFlush(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   int flags = MI_WRITE_DIRTY_STATE | MI_INVALIDATE_MAP_CACHE;

   if (IS_I96X(pI830))
      flags = 0;

   {
       BEGIN_LP_RING(2);
       OUT_RING(MI_FLUSH | flags);
       OUT_RING(MI_NOOP);		/* pad to quadword */
       ADVANCE_LP_RING();
   }
}

void
I830SelectBuffer(ScrnInfoPtr pScrn, int buffer)
{
   I830Ptr pI830 = I830PTR(pScrn);

   switch (buffer) {
#ifdef XF86DRI
   case I830_SELECT_BACK:
      pI830->bufferOffset = pI830->back_buffer->offset;
      break;
   case I830_SELECT_THIRD:
      pI830->bufferOffset = pI830->third_buffer->offset;
      break;
   case I830_SELECT_DEPTH:
      pI830->bufferOffset = pI830->depth_buffer->offset;
      break;
#endif
   default:
   case I830_SELECT_FRONT:
      pI830->bufferOffset = pScrn->fbOffset;
      break;
   }

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
      ErrorF("I830SelectBuffer %d --> offset %x\n",
	     buffer, pI830->bufferOffset);
}

/* The following function sets up the supported acceleration. Call it
 * from the FbInit() function in the SVGA driver, or before ScreenInit
 * in a monolithic server.
 */
Bool
I830AccelInit(ScreenPtr pScreen)
{
#ifdef I830_USE_EXA
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);

    if (pI830->useEXA)
	return I830EXAInit(pScreen);
#endif
#ifdef I830_USE_XAA
    return I830XAAInit(pScreen);
#endif
    return FALSE;
}
