
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * Authors:
 *   Keith Whitwell <keith@tungstengraphics.com>
 *
 */

#include "xf86.h"
#include "xaarop.h"
#include "i810.h"

const int I810CopyROP[16] = {
	ROP_0,			/* GXclear */
	ROP_DSa,		/* GXand */
	ROP_SDna,		/* GXandReverse */
	ROP_S,			/* GXcopy */
	ROP_DSna,		/* GXandInverted */
	ROP_D,			/* GXnoop */
	ROP_DSx,		/* GXxor */
	ROP_DSo,		/* GXor */
	ROP_DSon,		/* GXnor */
	ROP_DSxn,		/* GXequiv */
	ROP_Dn,			/* GXinvert */
	ROP_SDno,		/* GXorReverse */
	ROP_Sn,			/* GXcopyInverted */
	ROP_DSno,		/* GXorInverted */
	ROP_DSan,		/* GXnand */
	ROP_1			/* GXset */
};

const int I810PatternROP[16] = {
	ROP_0,
	ROP_DPa,
	ROP_PDna,
	ROP_P,
	ROP_DPna,
	ROP_D,
	ROP_DPx,
	ROP_DPo,
	ROP_DPon,
	ROP_PDxn,
	ROP_Dn,
	ROP_PDno,
	ROP_Pn,
	ROP_DPno,
	ROP_DPan,
	ROP_1
};

int
I810WaitLpRing(ScrnInfoPtr pScrn, int n, int timeout_millis)
{
   I810Ptr pI810 = I810PTR(pScrn);
   I810RingBuffer *ring = pI810->LpRing;
   int iters = 0;
   int start = 0;
   int now = 0;
   int last_head = 0;
   int first = 0;

   /* If your system hasn't moved the head pointer in 2 seconds, I'm going to
    * call it crashed.
    */
   if (timeout_millis == 0)
      timeout_millis = 2000;

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL) {
      ErrorF("I810WaitLpRing %d\n", n);
      first = GetTimeInMillis();
   }

   while (ring->space < n) {
      ring->head = INREG(LP_RING + RING_HEAD) & HEAD_ADDR;
      ring->space = ring->head - (ring->tail + 8);

      if (ring->space < 0)
	 ring->space += ring->mem.Size;

      iters++;
      now = GetTimeInMillis();
      if (start == 0 || now < start || ring->head != last_head) {
	 if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	    if (now > start)
	       ErrorF("space: %d wanted %d\n", ring->space, n);
	 start = now;
	 last_head = ring->head;
      } else if (now - start > timeout_millis) {
	 ErrorF("Error in I810WaitLpRing(), now is %d, start is %d\n", now,
		start);
	 I810PrintErrorState(pScrn);
	 ErrorF("space: %d wanted %d\n", ring->space, n);
#ifdef HAVE_DRI1
	 if (pI810->directRenderingEnabled) {
	    DRIUnlock(xf86ScrnToScreen(pScrn));
	    DRICloseScreen(xf86ScrnToScreen(pScrn));
	 }
#endif
#if HAVE_XAA_H
	 pI810->AccelInfoRec = NULL;	/* Stops recursive behavior */
#endif
	 FatalError("lockup\n");
      }

      DELAY(10000);
   }

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL) {
      now = GetTimeInMillis();
      if (now - first) {
	 ErrorF("Elapsed %d ms\n", now - first);
	 ErrorF("space: %d wanted %d\n", ring->space, n);
      }
   }

   return iters;
}

void
I810Sync(ScrnInfoPtr pScrn)
{
   I810Ptr pI810 = I810PTR(pScrn);

   if (I810_DEBUG & (DEBUG_VERBOSE_ACCEL | DEBUG_VERBOSE_SYNC))
      ErrorF("I810Sync\n");

#ifdef HAVE_DRI1
   /* VT switching tries to do this.  
    */
   if (!pI810->LockHeld && pI810->directRenderingEnabled) {
      return;
   }
#endif

   /* Send a flush instruction and then wait till the ring is empty.
    * This is stronger than waiting for the blitter to finish as it also
    * flushes the internal graphics caches.
    */
   {
      BEGIN_LP_RING(2);
      OUT_RING(INST_PARSER_CLIENT | INST_OP_FLUSH | INST_FLUSH_MAP_CACHE);
      OUT_RING(0);			/* pad to quadword */
      ADVANCE_LP_RING();
   }

   I810WaitLpRing(pScrn, pI810->LpRing->mem.Size - 8, 0);

   pI810->LpRing->space = pI810->LpRing->mem.Size - 8;
   pI810->nextColorExpandBuf = 0;
}

void
I810SetupForSolidFill(ScrnInfoPtr pScrn, int color, int rop,
		      unsigned int planemask)
{
   I810Ptr pI810 = I810PTR(pScrn);

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
      ErrorF("I810SetupForFillRectSolid color: %x rop: %x mask: %x\n",
	     color, rop, planemask);

   /* Color blit, p166 */
   pI810->BR[13] = (BR13_SOLID_PATTERN |
		    (I810PatternROP[rop] << 16) |
		    (pScrn->displayWidth * pI810->cpp));
   pI810->BR[16] = color;
}

void
I810SubsequentSolidFillRect(ScrnInfoPtr pScrn, int x, int y, int w, int h)
{
   I810Ptr pI810 = I810PTR(pScrn);

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
      ErrorF("I810SubsequentFillRectSolid %d,%d %dx%d\n", x, y, w, h);

   {
      BEGIN_LP_RING(6);

      OUT_RING(BR00_BITBLT_CLIENT | BR00_OP_COLOR_BLT | 0x3);
      OUT_RING(pI810->BR[13]);
      OUT_RING((h << 16) | (w * pI810->cpp));
      OUT_RING(pI810->bufferOffset +
	       (y * pScrn->displayWidth + x) * pI810->cpp);

      OUT_RING(pI810->BR[16]);
      OUT_RING(0);			/* pad to quadword */

      ADVANCE_LP_RING();
   }
}

void
I810SetupForScreenToScreenCopy(ScrnInfoPtr pScrn, int xdir, int ydir, int rop,
			       unsigned int planemask, int transparency_color)
{
   I810Ptr pI810 = I810PTR(pScrn);

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
      ErrorF("I810SetupForScreenToScreenCopy %d %d %x %x %d\n",
	     xdir, ydir, rop, planemask, transparency_color);

   pI810->BR[13] = (pScrn->displayWidth * pI810->cpp);

   if (ydir == -1)
      pI810->BR[13] = (-pI810->BR[13]) & 0xFFFF;
   if (xdir == -1)
      pI810->BR[13] |= BR13_RIGHT_TO_LEFT;

   pI810->BR[13] |= I810CopyROP[rop] << 16;

   pI810->BR[18] = 0;
}

void
I810SubsequentScreenToScreenCopy(ScrnInfoPtr pScrn, int x1, int y1,
				 int x2, int y2, int w, int h)
{
    I810Ptr pI810 = I810PTR(pScrn);
    int src, dst;
    int w_back = w;

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF( "I810SubsequentScreenToScreenCopy %d,%d - %d,%d %dx%d\n",
		x1,y1,x2,y2,w,h);
    /* 
     * This works around a bug in the i810 drawing engine.
     * This was developed empirically so it may not catch all
     * cases.
     */
#define I810_MWIDTH 8

    if ( !(pI810->BR[13] & BR13_RIGHT_TO_LEFT) && (y2 - y1) < 3 
	 && (y2 - y1) >= 0 && (x2 - x1) <= (w + I810_MWIDTH)
	 && (w > I810_MWIDTH))
	w = I810_MWIDTH;
    do {

	if (pI810->BR[13] & BR13_PITCH_SIGN_BIT) {
	    src = (y1 + h - 1) * pScrn->displayWidth * pI810->cpp;
	    dst = (y2 + h - 1) * pScrn->displayWidth * pI810->cpp;
	} else {
	    src = y1 * pScrn->displayWidth * pI810->cpp;
	    dst = y2 * pScrn->displayWidth * pI810->cpp;
	}

	if (pI810->BR[13] & BR13_RIGHT_TO_LEFT) {
	    src += (x1 + w - 1) * pI810->cpp + pI810->cpp - 1;
	    dst += (x2 + w - 1) * pI810->cpp + pI810->cpp - 1;
	} else {
	    src += x1 * pI810->cpp;
	    dst += x2 * pI810->cpp;
	}


	/* SRC_COPY_BLT, p169 */
	{
	    BEGIN_LP_RING(6);
	    OUT_RING( BR00_BITBLT_CLIENT | BR00_OP_SRC_COPY_BLT | 0x4 );
	    OUT_RING( pI810->BR[13]);

	    OUT_RING( (h << 16) | (w * pI810->cpp));
	    OUT_RING( pI810->bufferOffset + dst);

	    OUT_RING( pI810->BR[13] & 0xFFFF);
	    OUT_RING( pI810->bufferOffset + src);
	    ADVANCE_LP_RING();
	}
	w_back -= w;
	if (w_back <= 0)
	    break;
	x2 += w;
	x1 += w;
	if (w_back > I810_MWIDTH)
	    w = I810_MWIDTH;
	else
	    w = w_back;
    }  while (1);
}

void
I810EmitFlush(ScrnInfoPtr pScrn)
{
   I810Ptr pI810 = I810PTR(pScrn);

   BEGIN_LP_RING(2);
   OUT_RING(INST_PARSER_CLIENT | INST_OP_FLUSH | INST_FLUSH_MAP_CACHE);
   OUT_RING(0);
   ADVANCE_LP_RING();
}

void
I810SelectBuffer(ScrnInfoPtr pScrn, int buffer)
{
   I810Ptr pI810 = I810PTR(pScrn);

   switch (buffer) {
   case I810_SELECT_BACK:
      pI810->bufferOffset = pI810->BackBuffer.Start;
      break;
   case I810_SELECT_DEPTH:
      pI810->bufferOffset = pI810->DepthBuffer.Start;
      break;
   default:
   case I810_SELECT_FRONT:
      pI810->bufferOffset = pI810->FrontBuffer.Start;
      break;
   }

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
      ErrorF("I810SelectBuffer %d --> offset %x\n",
	     buffer, pI810->bufferOffset);
}

void
I810RefreshRing(ScrnInfoPtr pScrn)
{
   I810Ptr pI810 = I810PTR(pScrn);

   pI810->LpRing->head = INREG(LP_RING + RING_HEAD) & HEAD_ADDR;
   pI810->LpRing->tail = INREG(LP_RING + RING_TAIL);
   pI810->LpRing->space = pI810->LpRing->head - (pI810->LpRing->tail + 8);
   if (pI810->LpRing->space < 0)
      pI810->LpRing->space += pI810->LpRing->mem.Size;

#if HAVE_XAA_H
   if (pI810->AccelInfoRec)
      pI810->AccelInfoRec->NeedToSync = TRUE;
#endif
}
