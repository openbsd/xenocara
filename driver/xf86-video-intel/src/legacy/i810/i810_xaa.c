
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

#include "xorg-server.h"
#include "xf86.h"
#include "xaarop.h"
#include "i810.h"

static void
I810SetupForMono8x8PatternFill(ScrnInfoPtr pScrn, int pattx, int patty,
			       int fg, int bg, int rop,
			       unsigned int planemask)
{
   I810Ptr pI810 = I810PTR(pScrn);

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
      ErrorF("I810SetupFor8x8PatternColorExpand\n");

   /* FULL_MONO_PAT_BLT, p176 */
   pI810->BR[0] = (BR00_BITBLT_CLIENT | BR00_OP_MONO_PAT_BLT | 0x9);
   pI810->BR[18] = bg;
   pI810->BR[19] = fg;
   pI810->BR[13] = (pScrn->displayWidth * pI810->cpp);
   pI810->BR[13] |= I810PatternROP[rop] << 16;
   if (bg == -1)
      pI810->BR[13] |= BR13_MONO_PATN_TRANS;
}

static void
I810SubsequentMono8x8PatternFillRect(ScrnInfoPtr pScrn, int pattx, int patty,
				     int x, int y, int w, int h)
{
   I810Ptr pI810 = I810PTR(pScrn);
   int addr =
	 pI810->bufferOffset + (y * pScrn->displayWidth + x) * pI810->cpp;

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
      ErrorF("I810Subsequent8x8PatternColorExpand\n");

   {
      BEGIN_LP_RING(12);
      OUT_RING(pI810->BR[0] | ((y << 5) & BR00_PAT_VERT_ALIGN));
      OUT_RING(pI810->BR[13]);
      OUT_RING((h << 16) | (w * pI810->cpp));
      OUT_RING(addr);
      OUT_RING(pI810->BR[13] & 0xFFFF);	/* src pitch */
      OUT_RING(addr);			/* src addr */
      OUT_RING(0);			/* transparency color */
      OUT_RING(pI810->BR[18]);		/* bg */
      OUT_RING(pI810->BR[19]);		/* fg */
      OUT_RING(pattx);			/* pattern data */
      OUT_RING(patty);
      OUT_RING(0);
      ADVANCE_LP_RING();
   }
}

static void
I810GetNextScanlineColorExpandBuffer(ScrnInfoPtr pScrn)
{
   I810Ptr pI810 = I810PTR(pScrn);
   XAAInfoRecPtr infoPtr = pI810->AccelInfoRec;

   if (pI810->nextColorExpandBuf == pI810->NumScanlineColorExpandBuffers)
      I810Sync(pScrn);

   infoPtr->ScanlineColorExpandBuffers[0] =
	 pI810->ScanlineColorExpandBuffers[pI810->nextColorExpandBuf];

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
      ErrorF("using color expand buffer %d\n", pI810->nextColorExpandBuf);

   pI810->nextColorExpandBuf++;
}

static void
I810SetupForScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
					       int fg, int bg, int rop,
					       unsigned int planemask)
{
   I810Ptr pI810 = I810PTR(pScrn);

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
      ErrorF("I810SetupForScanlineScreenToScreenColorExpand %d %d %x %x\n",
	     fg, bg, rop, planemask);

   pI810->BR[13] = (pScrn->displayWidth * pI810->cpp);
   pI810->BR[13] |= I810CopyROP[rop] << 16;
   pI810->BR[13] |= (1 << 27);
   if (bg == -1)
      pI810->BR[13] |= BR13_MONO_TRANSPCY;

   pI810->BR[18] = bg;
   pI810->BR[19] = fg;

   I810GetNextScanlineColorExpandBuffer(pScrn);
}

static void
I810SubsequentScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
						 int x, int y,
						 int w, int h, int skipleft)
{
   I810Ptr pI810 = I810PTR(pScrn);

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
      ErrorF("I810SubsequentScanlineCPUToScreenColorExpandFill "
	     "%d,%d %dx%x %d\n", x, y, w, h, skipleft);

   pI810->BR[0] = BR00_BITBLT_CLIENT | BR00_OP_MONO_SRC_COPY_BLT | 0x06;
   pI810->BR[9] = (pI810->bufferOffset +
		   (y * pScrn->displayWidth + x) * pI810->cpp);
   pI810->BR[14] = ((1 << 16) | (w * pI810->cpp));
   pI810->BR[11] = ((w + 31) / 32) - 1;
}

static void
I810SubsequentColorExpandScanline(ScrnInfoPtr pScrn, int bufno)
{
   I810Ptr pI810 = I810PTR(pScrn);

   pI810->BR[12] = (pI810->AccelInfoRec->ScanlineColorExpandBuffers[0] -
		    pI810->FbBase);

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
      ErrorF("I810SubsequentColorExpandScanline %d (addr %x)\n",
	     bufno, pI810->BR[12]);

   {
      BEGIN_LP_RING(8);
      OUT_RING(pI810->BR[0]);
      OUT_RING(pI810->BR[13]);
      OUT_RING(pI810->BR[14]);
      OUT_RING(pI810->BR[9]);
      OUT_RING(pI810->BR[11]);
      OUT_RING(pI810->BR[12]);		/* srcaddr */
      OUT_RING(pI810->BR[18]);
      OUT_RING(pI810->BR[19]);
      ADVANCE_LP_RING();
   }

   /* Advance to next scanline.
    */
   pI810->BR[9] += pScrn->displayWidth * pI810->cpp;
   I810GetNextScanlineColorExpandBuffer(pScrn);
}

/* Emit on gaining VT?
 */
#if 0
static void
I810EmitInvarientState(ScrnInfoPtr pScrn)
{
   I810Ptr pI810 = I810PTR(pScrn);

   BEGIN_LP_RING(10);

   OUT_RING(INST_PARSER_CLIENT | INST_OP_FLUSH | INST_FLUSH_MAP_CACHE);
   OUT_RING(GFX_CMD_CONTEXT_SEL | CS_UPDATE_USE | CS_USE_CTX0);
   OUT_RING(INST_PARSER_CLIENT | INST_OP_FLUSH | INST_FLUSH_MAP_CACHE);
   OUT_RING(0);

   OUT_RING(GFX_OP_COLOR_CHROMA_KEY);
   OUT_RING(CC1_UPDATE_KILL_WRITE |
	    CC1_DISABLE_KILL_WRITE |
	    CC1_UPDATE_COLOR_IDX |
	    CC1_UPDATE_CHROMA_LOW | CC1_UPDATE_CHROMA_HI | 0);
   OUT_RING(0);
   OUT_RING(0);

/*     OUT_RING( CMD_OP_Z_BUFFER_INFO ); */
/*     OUT_RING( pI810->DepthBuffer.Start | pI810->auxPitchBits); */

   ADVANCE_LP_RING();
}
#endif

/* The following function sets up the supported acceleration. Call it
 * from the FbInit() function in the SVGA driver, or before ScreenInit
 * in a monolithic server.
 */
Bool
I810AccelInit(ScreenPtr pScreen)
{
   XAAInfoRecPtr infoPtr;
   ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
   I810Ptr pI810 = I810PTR(pScrn);

   if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
      ErrorF("I810AccelInit\n");

   pI810->AccelInfoRec = infoPtr = XAACreateInfoRec();
   if (!infoPtr)
      return FALSE;

   pI810->bufferOffset = 0;
   infoPtr->Flags = LINEAR_FRAMEBUFFER | OFFSCREEN_PIXMAPS;
   infoPtr->Flags |= PIXMAP_CACHE;

   /* Sync
    */
   infoPtr->Sync = I810Sync;

   /* Solid filled rectangles
    */
   {
      infoPtr->SolidFillFlags = NO_PLANEMASK;
      infoPtr->SetupForSolidFill = I810SetupForSolidFill;
      infoPtr->SubsequentSolidFillRect = I810SubsequentSolidFillRect;
   }

   /* Screen to screen copy
    *   - the transparency op hangs the blit engine, disable for now.
    */
   {
      infoPtr->ScreenToScreenCopyFlags = (0
					  | NO_PLANEMASK
					  | NO_TRANSPARENCY | 0);

      infoPtr->SetupForScreenToScreenCopy = I810SetupForScreenToScreenCopy;
      infoPtr->SubsequentScreenToScreenCopy =
	    I810SubsequentScreenToScreenCopy;
   }

   /* 8x8 pattern fills
    */
   {
      infoPtr->SetupForMono8x8PatternFill = I810SetupForMono8x8PatternFill;
      infoPtr->SubsequentMono8x8PatternFillRect =
	    I810SubsequentMono8x8PatternFillRect;

      infoPtr->Mono8x8PatternFillFlags = (HARDWARE_PATTERN_PROGRAMMED_BITS |
					  HARDWARE_PATTERN_SCREEN_ORIGIN |
					  BIT_ORDER_IN_BYTE_MSBFIRST |
					  NO_PLANEMASK | 0);
   }

   /* 8x8 color fills - not considered useful for XAA.
    */

   /* Scanline color expansion - Use the same scheme as the 3.3 driver.
    *
    */
   if (pI810->Scratch.Size != 0) {
      int i;
      int width = ALIGN(pScrn->displayWidth, 32) / 8;
      int nr_buffers = pI810->Scratch.Size / width;
      unsigned char *ptr = pI810->FbBase + pI810->Scratch.Start;

      pI810->NumScanlineColorExpandBuffers = nr_buffers;
      pI810->ScanlineColorExpandBuffers = (unsigned char **)
	    xnfcalloc(nr_buffers, sizeof(unsigned char *));

      for (i = 0; i < nr_buffers; i++, ptr += width)
	 pI810->ScanlineColorExpandBuffers[i] = ptr;

      infoPtr->ScanlineCPUToScreenColorExpandFillFlags = (NO_PLANEMASK |
							  ROP_NEEDS_SOURCE |
							  BIT_ORDER_IN_BYTE_MSBFIRST
							  | 0);

      infoPtr->ScanlineColorExpandBuffers = (unsigned char **)
	    xnfcalloc(1, sizeof(unsigned char *));
      infoPtr->NumScanlineColorExpandBuffers = 1;

      infoPtr->ScanlineColorExpandBuffers[0] =
	    pI810->ScanlineColorExpandBuffers[0];
      pI810->nextColorExpandBuf = 0;

      infoPtr->SetupForScanlineCPUToScreenColorExpandFill =
	    I810SetupForScanlineCPUToScreenColorExpandFill;

      infoPtr->SubsequentScanlineCPUToScreenColorExpandFill =
	    I810SubsequentScanlineCPUToScreenColorExpandFill;

      infoPtr->SubsequentColorExpandScanline =
	    I810SubsequentColorExpandScanline;
   }

   /* Possible todo: Image writes w/ non-GXCOPY rop.
    */

   I810SelectBuffer(pScrn, I810_SELECT_FRONT);

   return XAAInit(pScreen, infoPtr);
}
