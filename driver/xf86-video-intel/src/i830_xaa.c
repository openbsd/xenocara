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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <assert.h>
#include "xf86.h"
#include "xaarop.h"
#include "i830.h"
#include "i810_reg.h"
#include "mipict.h"

#ifndef DO_SCANLINE_IMAGE_WRITE
#define DO_SCANLINE_IMAGE_WRITE 0
#endif

/* I830 Accel Functions */
static void I830SetupForMono8x8PatternFill(ScrnInfoPtr pScrn,
					   int pattx, int patty,
					   int fg, int bg, int rop,
					   unsigned int planemask);
static void I830SubsequentMono8x8PatternFillRect(ScrnInfoPtr pScrn,
						 int pattx, int patty,
						 int x, int y, int w, int h);

static void I830SetupForScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
							   int fg, int bg,
							   int rop,
							   unsigned int mask);

static void I830SubsequentScanlineCPUToScreenColorExpandFill(ScrnInfoPtr
							     pScrn, int x,
							     int y, int w,
							     int h,
							     int skipleft);

static void I830SubsequentColorExpandScanline(ScrnInfoPtr pScrn, int bufno);

#if DO_SCANLINE_IMAGE_WRITE
static void I830SetupForScanlineImageWrite(ScrnInfoPtr pScrn, int rop,
					   unsigned int planemask,
					   int trans_color, int bpp,
					   int depth);
static void I830SubsequentScanlineImageWriteRect(ScrnInfoPtr pScrn,
						 int x, int y, int w, int h,
						 int skipleft);
static void I830SubsequentImageWriteScanline(ScrnInfoPtr pScrn, int bufno);
#endif

void
i830_xaa_composite(CARD8	op,
		   PicturePtr	pSrc,
		   PicturePtr	pMask,
		   PicturePtr	pDst,
		   INT16	xSrc,
		   INT16	ySrc,
		   INT16	xMask,
		   INT16	yMask,
		   INT16	xDst,
		   INT16	yDst,
		   CARD16	width,
		   CARD16	height);

Bool
I830XAAInit(ScreenPtr pScreen)
{
    XAAInfoRecPtr infoPtr;
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    PictureScreenPtr ps = GetPictureScreenIfSet(pScreen);
    I830Ptr pI830 = I830PTR(pScrn);
    int i;
    int width = 0;
    int nr_buffers = 0;
    unsigned char *ptr = NULL;

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("I830XAAInit\n");

    pI830->AccelInfoRec = infoPtr = XAACreateInfoRec();
    if (!infoPtr)
	return FALSE;

    infoPtr->Flags = LINEAR_FRAMEBUFFER | OFFSCREEN_PIXMAPS | PIXMAP_CACHE;

    /* Use the same sync function as the I830.
     */
    infoPtr->Sync = I830Sync;

    /* Everything else is different enough to justify different functions */
    {
	infoPtr->SolidFillFlags = NO_PLANEMASK;
	infoPtr->SetupForSolidFill = I830SetupForSolidFill;
	infoPtr->SubsequentSolidFillRect = I830SubsequentSolidFillRect;
    }

    {
	infoPtr->ScreenToScreenCopyFlags = (NO_PLANEMASK | NO_TRANSPARENCY);

	infoPtr->SetupForScreenToScreenCopy = I830SetupForScreenToScreenCopy;
	infoPtr->SubsequentScreenToScreenCopy =
	    I830SubsequentScreenToScreenCopy;
    }

    {
	infoPtr->SetupForMono8x8PatternFill = I830SetupForMono8x8PatternFill;
	infoPtr->SubsequentMono8x8PatternFillRect =
	    I830SubsequentMono8x8PatternFillRect;

	infoPtr->Mono8x8PatternFillFlags = (HARDWARE_PATTERN_PROGRAMMED_BITS |
					    HARDWARE_PATTERN_SCREEN_ORIGIN |
					    HARDWARE_PATTERN_PROGRAMMED_ORIGIN|
					    BIT_ORDER_IN_BYTE_MSBFIRST |
					    NO_PLANEMASK);

    }

    if (pI830->xaa_scratch->size != 0) {
	width = ((pScrn->displayWidth + 31) & ~31) / 8;
	nr_buffers = pI830->xaa_scratch->size / width;
	ptr = pI830->FbBase + pI830->xaa_scratch->offset;
    }

    if (nr_buffers) {
	pI830->NumScanlineColorExpandBuffers = nr_buffers;
	pI830->ScanlineColorExpandBuffers = (unsigned char **)
	    xnfcalloc(nr_buffers, sizeof(unsigned char *));

	for (i = 0; i < nr_buffers; i++, ptr += width)
	    pI830->ScanlineColorExpandBuffers[i] = ptr;

	infoPtr->ScanlineCPUToScreenColorExpandFillFlags =
	    (NO_PLANEMASK | ROP_NEEDS_SOURCE | BIT_ORDER_IN_BYTE_MSBFIRST);

	infoPtr->ScanlineColorExpandBuffers = (unsigned char **)
	    xnfcalloc(1, sizeof(unsigned char *));
	infoPtr->NumScanlineColorExpandBuffers = 1;

	infoPtr->ScanlineColorExpandBuffers[0] =
	    pI830->ScanlineColorExpandBuffers[0];
	pI830->nextColorExpandBuf = 0;

	infoPtr->SetupForScanlineCPUToScreenColorExpandFill =
	    I830SetupForScanlineCPUToScreenColorExpandFill;

	infoPtr->SubsequentScanlineCPUToScreenColorExpandFill =
	    I830SubsequentScanlineCPUToScreenColorExpandFill;

	infoPtr->SubsequentColorExpandScanline =
	    I830SubsequentColorExpandScanline;

#if DO_SCANLINE_IMAGE_WRITE
	infoPtr->NumScanlineImageWriteBuffers = 1;
	infoPtr->ScanlineImageWriteBuffers =
	    infoPtr->ScanlineColorExpandBuffers;
	infoPtr->SetupForScanlineImageWrite = I830SetupForScanlineImageWrite;
	infoPtr->SubsequentScanlineImageWriteRect =
	    I830SubsequentScanlineImageWriteRect;
	infoPtr->SubsequentImageWriteScanline =
	    I830SubsequentImageWriteScanline;
	infoPtr->ScanlineImageWriteFlags = NO_GXCOPY |
	    NO_PLANEMASK |
	    ROP_NEEDS_SOURCE |
	    SCANLINE_PAD_DWORD;
#endif
    }

    /* Set up pI830->bufferOffset */
    I830SelectBuffer(pScrn, I830_SELECT_FRONT);

    if (!XAAInit(pScreen, infoPtr))
	return FALSE;

    if (ps != NULL) {
	if (IS_I865G(pI830) || IS_I855(pI830) ||
	    IS_845G(pI830) || IS_I830(pI830))
	{
	    pI830->xaa_check_composite = i830_check_composite;
	    pI830->xaa_prepare_composite = i830_prepare_composite;
	    pI830->xaa_composite = i830_composite;
	    pI830->xaa_done_composite = i830_done_composite;
	} else if (IS_I915G(pI830) || IS_I915GM(pI830) ||
		   IS_I945G(pI830) || IS_I945GM(pI830) || IS_G33CLASS(pI830))
	{
	    pI830->xaa_check_composite = i915_check_composite;
	    pI830->xaa_prepare_composite = i915_prepare_composite;
	    pI830->xaa_composite = i830_composite;
	    pI830->xaa_done_composite = i830_done_composite;
	} else {
	    pI830->xaa_check_composite = i965_check_composite;
	    pI830->xaa_prepare_composite = i965_prepare_composite;
	    pI830->xaa_composite = i965_composite;
	    pI830->xaa_done_composite = i830_done_composite;
	}

	pI830->saved_composite = ps->Composite;
	ps->Composite = i830_xaa_composite;
    }

    return TRUE;
}

static unsigned int
I830CheckTiling(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

   if (pI830->bufferOffset == pI830->front_buffer->offset &&
       pI830->front_buffer->tiling != TILE_NONE)
   {
       return TRUE;
   }
#ifdef XF86DRI
   if (pI830->back_buffer != NULL &&
       pI830->bufferOffset == pI830->back_buffer->offset &&
       pI830->back_buffer->tiling != TILE_NONE)
   {
       return TRUE;
   }
   if (pI830->depth_buffer != NULL &&
       pI830->bufferOffset == pI830->depth_buffer->offset &&
       pI830->depth_buffer->tiling != TILE_NONE)
   {
       return TRUE;
   }
#endif

   return FALSE;
}

void
I830SetupForSolidFill(ScrnInfoPtr pScrn, int color, int rop,
		      unsigned int planemask)
{
    I830Ptr pI830 = I830PTR(pScrn);

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("I830SetupForFillRectSolid color: %x rop: %x mask: %x\n",
	       color, rop, planemask);

    if (IS_I965G(pI830) && I830CheckTiling(pScrn)) {
	pI830->BR[13] = (pScrn->displayWidth * pI830->cpp) >> 2;
    } else {
	pI830->BR[13] = (pScrn->displayWidth * pI830->cpp);
    }

#ifdef I830_USE_EXA
    /* This function gets used by I830DRIInitBuffers(), and we might not have
     * XAAGetPatternROP() available.  So just use the ROPs from our EXA code
     * if available.
     */
    pI830->BR[13] |= (I830PatternROP[rop] << 16);
#else
    pI830->BR[13] |= (XAAGetPatternROP(rop) << 16);
#endif

    pI830->BR[16] = color;

    switch (pScrn->bitsPerPixel) {
    case 8:
	break;
    case 16:
	pI830->BR[13] |= (1 << 24);
	break;
    case 32:
	pI830->BR[13] |= ((1 << 25) | (1 << 24));
	break;
    }
}

void
I830SubsequentSolidFillRect(ScrnInfoPtr pScrn, int x, int y, int w, int h)
{
    I830Ptr pI830 = I830PTR(pScrn);

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("I830SubsequentFillRectSolid %d,%d %dx%d\n", x, y, w, h);

    {
	BEGIN_BATCH(6);

	if (pScrn->bitsPerPixel == 32) {
	    OUT_BATCH(COLOR_BLT_CMD | COLOR_BLT_WRITE_ALPHA |
		      COLOR_BLT_WRITE_RGB);
	} else {
	    OUT_BATCH(COLOR_BLT_CMD);
	}
	OUT_BATCH(pI830->BR[13]);
	OUT_BATCH((h << 16) | (w * pI830->cpp));
	OUT_BATCH(pI830->bufferOffset + (y * pScrn->displayWidth + x) *
		  pI830->cpp);
	OUT_BATCH(pI830->BR[16]);
	OUT_BATCH(0);

	ADVANCE_BATCH();
    }

    if (IS_I965G(pI830))
      I830EmitFlush(pScrn);
}

void
I830SetupForScreenToScreenCopy(ScrnInfoPtr pScrn, int xdir, int ydir, int rop,
			       unsigned int planemask, int transparency_color)
{
    I830Ptr pI830 = I830PTR(pScrn);

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("I830SetupForScreenToScreenCopy %d %d %x %x %d\n",
	       xdir, ydir, rop, planemask, transparency_color);

    if (IS_I965G(pI830) && I830CheckTiling(pScrn)) {
	pI830->BR[13] = (pScrn->displayWidth * pI830->cpp) >> 2;
    } else {
	pI830->BR[13] = (pScrn->displayWidth * pI830->cpp);
    }

#ifdef I830_USE_EXA
    /* This function gets used by I830DRIInitBuffers(), and we might not have
     * XAAGetCopyROP() available.  So just use the ROPs from our EXA code
     * if available.
     */
    pI830->BR[13] |= I830CopyROP[rop] << 16;
#else
    pI830->BR[13] |= XAAGetCopyROP(rop) << 16;
#endif

    switch (pScrn->bitsPerPixel) {
    case 8:
	break;
    case 16:
	pI830->BR[13] |= (1 << 24);
	break;
    case 32:
	pI830->BR[13] |= ((1 << 25) | (1 << 24));
	break;
    }

}

void
I830SubsequentScreenToScreenCopy(ScrnInfoPtr pScrn, int src_x1, int src_y1,
				 int dst_x1, int dst_y1, int w, int h)
{
    I830Ptr pI830 = I830PTR(pScrn);
    int dst_x2, dst_y2;
    unsigned int tiled = I830CheckTiling(pScrn);

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("I830SubsequentScreenToScreenCopy %d,%d - %d,%d %dx%d\n",
	       src_x1, src_y1, dst_x1, dst_y1, w, h);

    dst_x2 = dst_x1 + w;
    dst_y2 = dst_y1 + h;

    {
	BEGIN_BATCH(8);

	if (pScrn->bitsPerPixel == 32) {
	    OUT_BATCH(XY_SRC_COPY_BLT_CMD | XY_SRC_COPY_BLT_WRITE_ALPHA |
		      XY_SRC_COPY_BLT_WRITE_RGB | tiled << 15 | tiled << 11);
	} else {
	    OUT_BATCH(XY_SRC_COPY_BLT_CMD | tiled << 15 | tiled << 11);
	}
	OUT_BATCH(pI830->BR[13]);
	OUT_BATCH((dst_y1 << 16) | (dst_x1 & 0xffff));
	OUT_BATCH((dst_y2 << 16) | (dst_x2 & 0xffff));
	OUT_BATCH(pI830->bufferOffset);
	OUT_BATCH((src_y1 << 16) | (src_x1 & 0xffff));
	OUT_BATCH(pI830->BR[13] & 0xFFFF);
	OUT_BATCH(pI830->bufferOffset);

	ADVANCE_BATCH();
    }

    if (IS_I965G(pI830))
      I830EmitFlush(pScrn);
}

static void
I830SetupForMono8x8PatternFill(ScrnInfoPtr pScrn, int pattx, int patty,
			       int fg, int bg, int rop,
			       unsigned int planemask)
{
    I830Ptr pI830 = I830PTR(pScrn);

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("I830SetupForMono8x8PatternFill\n");

    pI830->BR[16] = pattx;
    pI830->BR[17] = patty;
    pI830->BR[18] = bg;
    pI830->BR[19] = fg;

    if (IS_I965G(pI830) && I830CheckTiling(pScrn)) {
	pI830->BR[13] = (pScrn->displayWidth * pI830->cpp) >> 2;
    } else {
	pI830->BR[13] = (pScrn->displayWidth * pI830->cpp);
    }
    pI830->BR[13] |= XAAGetPatternROP(rop) << 16;
    if (bg == -1)
	pI830->BR[13] |= (1 << 28);

    switch (pScrn->bitsPerPixel) {
    case 8:
	break;
    case 16:
	pI830->BR[13] |= (1 << 24);
	break;
    case 32:
	pI830->BR[13] |= ((1 << 25) | (1 << 24));
	break;
    }

}

static void
I830SubsequentMono8x8PatternFillRect(ScrnInfoPtr pScrn, int pattx, int patty,
				     int x, int y, int w, int h)
{
    I830Ptr pI830 = I830PTR(pScrn);
    int x1, x2, y1, y2;
    unsigned int tiled = I830CheckTiling(pScrn);

    x1 = x;
    x2 = x + w;
    y1 = y;
    y2 = y + h;

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("I830SubsequentMono8x8PatternFillRect\n");

    {
	BEGIN_BATCH(10);

	if (pScrn->bitsPerPixel == 32) {
	    OUT_BATCH(XY_MONO_PAT_BLT_CMD | XY_MONO_PAT_BLT_WRITE_ALPHA |
		      XY_MONO_PAT_BLT_WRITE_RGB | tiled << 11 |
		      ((patty << 8) & XY_MONO_PAT_VERT_SEED) |
		      ((pattx << 12) & XY_MONO_PAT_HORT_SEED));
	} else {
	    OUT_BATCH(XY_MONO_PAT_BLT_CMD | tiled << 11 |
		      ((patty << 8) & XY_MONO_PAT_VERT_SEED) |
		      ((pattx << 12) & XY_MONO_PAT_HORT_SEED));
	}
	OUT_BATCH(pI830->BR[13]);
	OUT_BATCH((y1 << 16) | x1);
	OUT_BATCH((y2 << 16) | x2);
	OUT_BATCH(pI830->bufferOffset);
	OUT_BATCH(pI830->BR[18]);		/* bg */
	OUT_BATCH(pI830->BR[19]);		/* fg */
	OUT_BATCH(pI830->BR[16]);		/* pattern data */
	OUT_BATCH(pI830->BR[17]);
	OUT_BATCH(0);
	ADVANCE_BATCH();
    }

    if (IS_I965G(pI830))
      I830EmitFlush(pScrn);
}

static void
I830GetNextScanlineColorExpandBuffer(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    XAAInfoRecPtr infoPtr = pI830->AccelInfoRec;

    if (pI830->nextColorExpandBuf == pI830->NumScanlineColorExpandBuffers)
	I830Sync(pScrn);

    infoPtr->ScanlineColorExpandBuffers[0] =
	pI830->ScanlineColorExpandBuffers[pI830->nextColorExpandBuf];

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("using color expand buffer %d\n", pI830->nextColorExpandBuf);

    pI830->nextColorExpandBuf++;
}

static void
I830SetupForScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
					       int fg, int bg, int rop,
					       unsigned int planemask)
{
    I830Ptr pI830 = I830PTR(pScrn);

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("I830SetupForScanlineScreenToScreenColorExpand %d %d %x %x\n",
	       fg, bg, rop, planemask);

    /* Fill out register values */
    if (IS_I965G(pI830) && I830CheckTiling(pScrn)) {
	pI830->BR[13] = (pScrn->displayWidth * pI830->cpp) >> 2;
    } else {
	pI830->BR[13] = (pScrn->displayWidth * pI830->cpp);
    }
    pI830->BR[13] |= XAAGetCopyROP(rop) << 16;
    if (bg == -1)
	pI830->BR[13] |= (1 << 29);

    switch (pScrn->bitsPerPixel) {
    case 8:
	break;
    case 16:
	pI830->BR[13] |= (1 << 24);
	break;
    case 32:
	pI830->BR[13] |= ((1 << 25) | (1 << 24));
	break;
    }

    pI830->BR[18] = bg;
    pI830->BR[19] = fg;

    I830GetNextScanlineColorExpandBuffer(pScrn);
}

static void
I830SubsequentScanlineCPUToScreenColorExpandFill(ScrnInfoPtr pScrn,
						 int x, int y,
						 int w, int h, int skipleft)
{
    I830Ptr pI830 = I830PTR(pScrn);

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("I830SubsequentScanlineCPUToScreenColorExpandFill "
	       "%d,%d %dx%x %d\n", x, y, w, h, skipleft);

    /* Fill out register values */
    pI830->BR[9] = (pI830->bufferOffset +
		    (y * pScrn->displayWidth + x) * pI830->cpp);
    pI830->BR[11] = ((1 << 16) | w);
}

static void
I830SubsequentColorExpandScanline(ScrnInfoPtr pScrn, int bufno)
{
    I830Ptr pI830 = I830PTR(pScrn);
    unsigned int tiled = I830CheckTiling(pScrn);

    pI830->BR[12] = (pI830->AccelInfoRec->ScanlineColorExpandBuffers[0] -
		     pI830->FbBase);

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("I830SubsequentColorExpandScanline %d (addr %x)\n",
	       bufno, pI830->BR[12]);

    {
	BEGIN_BATCH(8);

	if (pScrn->bitsPerPixel == 32) {
	    OUT_BATCH(XY_MONO_SRC_BLT_CMD | XY_MONO_SRC_BLT_WRITE_ALPHA |
		      tiled << 11 | XY_MONO_SRC_BLT_WRITE_RGB);
	} else {
	    OUT_BATCH(XY_MONO_SRC_BLT_CMD | tiled << 11);
	}
	OUT_BATCH(pI830->BR[13]);
	OUT_BATCH(0);			/* x1 = 0, y1 = 0 */
	OUT_BATCH(pI830->BR[11]);		/* x2 = w, y2 = 1 */
	OUT_BATCH(pI830->BR[9]);		/* dst addr */
	OUT_BATCH(pI830->BR[12]);		/* src addr */
	OUT_BATCH(pI830->BR[18]);		/* bg */
	OUT_BATCH(pI830->BR[19]);		/* fg */

	ADVANCE_BATCH();
    }

    /* Advance to next scanline.
     */
    pI830->BR[9] += pScrn->displayWidth * pI830->cpp;
    I830GetNextScanlineColorExpandBuffer(pScrn);

    if (IS_I965G(pI830))
      I830EmitFlush(pScrn);
}

#if DO_SCANLINE_IMAGE_WRITE
static void
I830SetupForScanlineImageWrite(ScrnInfoPtr pScrn, int rop,
			       unsigned int planemask, int trans_color,
			       int bpp, int depth)
{
    I830Ptr pI830 = I830PTR(pScrn);

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("I830SetupForScanlineImageWrite %x %x\n", rop, planemask);

    /* Fill out register values */
    if (IS_I965G(pI830) && I830CheckTiling(pScrn)) {
	pI830->BR[13] = (pScrn->displayWidth * pI830->cpp) >> 2;
    } else {
	pI830->BR[13] = (pScrn->displayWidth * pI830->cpp);
    }
    pI830->BR[13] |= XAAGetCopyROP(rop) << 16;

    switch (pScrn->bitsPerPixel) {
    case 8:
	break;
    case 16:
	pI830->BR[13] |= (1 << 24);
	break;
    case 32:
	pI830->BR[13] |= ((1 << 25) | (1 << 24));
	break;
    }

    I830GetNextScanlineColorExpandBuffer(pScrn);
}

static void
I830SubsequentScanlineImageWriteRect(ScrnInfoPtr pScrn, int x, int y,
				     int w, int h, int skipleft)
{
    I830Ptr pI830 = I830PTR(pScrn);

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("I830SubsequentScanlineImageWriteRect "
	       "%d,%d %dx%x %d\n", x, y, w, h, skipleft);

    /* Fill out register values */
    pI830->BR[9] = (pI830->bufferOffset +
		    (y * pScrn->displayWidth + x) * pI830->cpp);
    pI830->BR[11] = ((1 << 16) | w);
}

static void
I830SubsequentImageWriteScanline(ScrnInfoPtr pScrn, int bufno)
{
    I830Ptr pI830 = I830PTR(pScrn);
    unsigned int tiled = I830CheckTiling(pScrn);

    pI830->BR[12] = (pI830->AccelInfoRec->ScanlineColorExpandBuffers[0] -
		     pI830->FbBase);

    if (I810_DEBUG & DEBUG_VERBOSE_ACCEL)
	ErrorF("I830SubsequentImageWriteScanline %d (addr %x)\n",
	       bufno, pI830->BR[12]);

    {
	BEGIN_BATCH(8);

	if (pScrn->bitsPerPixel == 32) {
	    OUT_BATCH(XY_SRC_COPY_BLT_CMD | XY_SRC_COPY_BLT_WRITE_ALPHA |
		      tiled << 11 | XY_SRC_COPY_BLT_WRITE_RGB);
	} else {
	    OUT_BATCH(XY_SRC_COPY_BLT_CMD | tiled << 11);
	}
	OUT_BATCH(pI830->BR[13]);
	OUT_BATCH(0);				/* x1 = 0, y1 = 0 */
	OUT_BATCH(pI830->BR[11]);		/* x2 = w, y2 = 1 */
	OUT_BATCH(pI830->BR[9]);			/* dst addr */
	OUT_BATCH(0);				/* source origin (0,0) */
	OUT_BATCH(pI830->BR[11] & 0xffff);	/* source pitch */
	OUT_BATCH(pI830->BR[12]);		/* src addr */

	ADVANCE_BATCH();
    }

    /* Advance to next scanline.
     */
    pI830->BR[9] += pScrn->displayWidth * pI830->cpp;
    I830GetNextScanlineColorExpandBuffer(pScrn);
}
#endif /* DO_SCANLINE_IMAGE_WRITE */
/* Support for multiscreen */

/**
 * Special case acceleration for Render acceleration of rotation operations
 * by xf86Rotate.c
 */
void
i830_xaa_composite(CARD8	op,
		   PicturePtr	pSrc,
		   PicturePtr	pMask,
		   PicturePtr	pDst,
		   INT16	xSrc,
		   INT16	ySrc,
		   INT16	xMask,
		   INT16	yMask,
		   INT16	xDst,
		   INT16	yDst,
		   CARD16	width,
		   CARD16	height)
{
    ScreenPtr pScreen = pDst->pDrawable->pScreen;
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    PictureScreenPtr ps;
    PixmapPtr pSrcPixmap, pDstPixmap;
    RegionRec region;
    BoxPtr pbox;
    int nbox;
    int	i;

    /* Throw out cases that aren't going to be our rotation first */
    if (pMask != NULL || op != PictOpSrc || pSrc->pDrawable == NULL)
	goto fallback;

    if (pSrc->pDrawable->type != DRAWABLE_WINDOW ||
	pDst->pDrawable->type != DRAWABLE_PIXMAP)
    {
	goto fallback;
    }
    pSrcPixmap = (*pScreen->GetWindowPixmap) ((WindowPtr) pSrc->pDrawable);
    pDstPixmap = (PixmapPtr)pDst->pDrawable;

    /* Check if the dest is one of our shadow pixmaps */
    for (i = 0; i < xf86_config->num_crtc; i++) {
	xf86CrtcPtr crtc = xf86_config->crtc[i];

	if (crtc->rotatedPixmap == pDstPixmap)
	    break;
    }
    if (i == xf86_config->num_crtc)
	goto fallback;

    if (pSrcPixmap != pScreen->GetScreenPixmap(pScreen))
	goto fallback;

    /* OK, so we've got a Render operation on one of our shadow pixmaps, with
     * the source being the real framebuffer.  We know that both of these are
     * in framebuffer, with no x/y offsets, i.e. normal pixmaps like our EXA-
     * based Render acceleration code expects.
     */
    assert(pSrcPixmap->drawable.x == 0);
    assert(pSrcPixmap->drawable.y == 0);
    assert(pDstPixmap->drawable.x == 0);
    assert(pDstPixmap->drawable.y == 0);

    if (!miComputeCompositeRegion (&region, pSrc, NULL, pDst,
				   xSrc, ySrc, 0, 0, xDst, yDst,
				   width, height))
	return;

    if (!pI830->xaa_check_composite(op, pSrc, NULL, pDst)) {
	REGION_UNINIT(pScreen, &region);
	goto fallback;
    }

    if (!pI830->xaa_prepare_composite(op, pSrc, NULL, pDst,
				      pSrcPixmap, NULL, pDstPixmap))
    {
	REGION_UNINIT(pScreen, &region);
	goto fallback;
    }

    nbox = REGION_NUM_RECTS(&region);
    pbox = REGION_RECTS(&region);

    xSrc -= xDst;
    ySrc -= yDst;

    while (nbox--)
    {
	pI830->xaa_composite(pDstPixmap,
			     pbox->x1 + xSrc,
			     pbox->y1 + ySrc,
			     0, 0,
			     pbox->x1,
			     pbox->y1,
			     pbox->x2 - pbox->x1,
			     pbox->y2 - pbox->y1);
	pbox++;
    }

    REGION_UNINIT(pDst->pDrawable->pScreen, &region);

    pI830->xaa_done_composite(pDstPixmap);
    i830MarkSync(pScrn);

    return;

fallback:
    /* Fallback path: Call down to the next level (XAA) */
    ps = GetPictureScreenIfSet(pScreen);

    ps->Composite = pI830->saved_composite;

    ps->Composite(op, pSrc, pMask, pDst, xSrc, ySrc, xMask, yMask, xDst, yDst,
		  width, height);

    pI830->saved_composite = ps->Composite;
    ps->Composite = i830_xaa_composite;
}
