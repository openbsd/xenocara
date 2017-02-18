/*
Copyright (C) 1994-1999 The XFree86 Project, Inc.  All Rights Reserved.
Copyright (C) 2000 Silicon Motion, Inc.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FIT-
NESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the names of the XFree86 Project and
Silicon Motion shall not be used in advertising or otherwise to promote the
sale, use or other dealings in this Software without prior written
authorization from the XFree86 Project and silicon Motion.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "smi.h"
#include "smi_501.h"

#ifdef HAVE_XAA_H

#include "miline.h"
#include "xaalocal.h"
#include "xaarop.h"
#include "servermd.h"

static void SMI_SetupForScreenToScreenCopy(ScrnInfoPtr, int, int, int,
					   unsigned int, int);
static void SMI_SubsequentScreenToScreenCopy(ScrnInfoPtr, int, int, int, int,
					     int, int);
static void SMI_SetupForSolidFill(ScrnInfoPtr, int, int, unsigned);
static void SMI_SubsequentSolidFillRect(ScrnInfoPtr, int, int, int, int);
static void SMI_SubsequentSolidHorVertLine(ScrnInfoPtr, int, int, int, int);
static void SMI_SetupForCPUToScreenColorExpandFill(ScrnInfoPtr, int, int, int,
						   unsigned int);
static void SMI_SubsequentCPUToScreenColorExpandFill(ScrnInfoPtr, int, int, int,
						     int, int);
static void SMI_SetupForMono8x8PatternFill(ScrnInfoPtr, int, int, int, int, int,
					   unsigned int);
static void SMI_SubsequentMono8x8PatternFillRect(ScrnInfoPtr, int, int, int,
						 int, int, int);
static void SMI_SetupForColor8x8PatternFill(ScrnInfoPtr, int, int, int,
					    unsigned int, int);
static void SMI_SubsequentColor8x8PatternFillRect(ScrnInfoPtr, int, int, int,
						  int, int, int);
#if SMI_USE_IMAGE_WRITES
static void SMI_SetupForImageWrite(ScrnInfoPtr, int, unsigned int, int, int,
				   int);
static void SMI_SubsequentImageWriteRect(ScrnInfoPtr, int, int, int, int, int);
#endif

#endif
Bool
SMI_XAAInit(ScreenPtr pScreen)
{
#ifdef HAVE_XAA_H
    XAAInfoRecPtr infoPtr;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    SMIPtr pSmi = SMIPTR(pScrn);
    /*BoxRec AvailFBArea;*/
    Bool ret;
    /*int numLines, maxLines;*/

    ENTER();

    pSmi->XAAInfoRec = infoPtr = XAACreateInfoRec();
    if (infoPtr == NULL)
	LEAVE(FALSE);

    infoPtr->Flags = PIXMAP_CACHE
		   | LINEAR_FRAMEBUFFER
		   | OFFSCREEN_PIXMAPS;

    infoPtr->Sync = SMI_AccelSync;

    if (xf86IsEntityShared(pScrn->entityList[0]))
	infoPtr->RestoreAccelState = SMI_EngineReset;

    /* Screen to screen copies */
    infoPtr->ScreenToScreenCopyFlags = NO_PLANEMASK
				     | ONLY_TWO_BITBLT_DIRECTIONS;
    infoPtr->SetupForScreenToScreenCopy = SMI_SetupForScreenToScreenCopy;
    infoPtr->SubsequentScreenToScreenCopy = SMI_SubsequentScreenToScreenCopy;
    if (pScrn->bitsPerPixel == 24) {
	infoPtr->ScreenToScreenCopyFlags |= NO_TRANSPARENCY;
    }
    if ((pSmi->Chipset == SMI_LYNX3D) && (pScrn->bitsPerPixel == 8)) {
	infoPtr->ScreenToScreenCopyFlags |= GXCOPY_ONLY;
    }

    /* Solid Fills */
    infoPtr->SolidFillFlags = NO_PLANEMASK;
    infoPtr->SetupForSolidFill = SMI_SetupForSolidFill;
    infoPtr->SubsequentSolidFillRect = SMI_SubsequentSolidFillRect;

    /* Solid Lines */
    infoPtr->SolidLineFlags = NO_PLANEMASK;
    infoPtr->SetupForSolidLine = SMI_SetupForSolidFill;
    infoPtr->SubsequentSolidHorVertLine = SMI_SubsequentSolidHorVertLine;

    /* Color Expansion Fills */
    infoPtr->CPUToScreenColorExpandFillFlags = ROP_NEEDS_SOURCE
					     | NO_PLANEMASK
					     | BIT_ORDER_IN_BYTE_MSBFIRST
					     | LEFT_EDGE_CLIPPING
					     | CPU_TRANSFER_PAD_DWORD
					     | SCANLINE_PAD_DWORD;
    infoPtr->ColorExpandBase = pSmi->DataPortBase;
    infoPtr->ColorExpandRange = pSmi->DataPortSize;
    infoPtr->SetupForCPUToScreenColorExpandFill =
	    SMI_SetupForCPUToScreenColorExpandFill;
    infoPtr->SubsequentCPUToScreenColorExpandFill =
	    SMI_SubsequentCPUToScreenColorExpandFill;

    /* 8x8 Mono Pattern Fills */
    infoPtr->Mono8x8PatternFillFlags = NO_PLANEMASK
				     | HARDWARE_PATTERN_PROGRAMMED_BITS
				     | HARDWARE_PATTERN_SCREEN_ORIGIN
				     | BIT_ORDER_IN_BYTE_MSBFIRST;
    infoPtr->SetupForMono8x8PatternFill = SMI_SetupForMono8x8PatternFill;
    infoPtr->SubsequentMono8x8PatternFillRect =
	SMI_SubsequentMono8x8PatternFillRect;

    /* 8x8 Color Pattern Fills */
    if (!SMI_LYNX3D_SERIES(pSmi->Chipset) || (pScrn->bitsPerPixel != 24)) {
	infoPtr->Color8x8PatternFillFlags = NO_PLANEMASK
					  | HARDWARE_PATTERN_SCREEN_ORIGIN;
	infoPtr->SetupForColor8x8PatternFill =
		SMI_SetupForColor8x8PatternFill;
	infoPtr->SubsequentColor8x8PatternFillRect =
		SMI_SubsequentColor8x8PatternFillRect;
    }

#if SMI_USE_IMAGE_WRITES
    /* Image Writes */
    infoPtr->ImageWriteFlags = ROP_NEEDS_SOURCE
			     | NO_PLANEMASK
			     | CPU_TRANSFER_PAD_DWORD
			     | SCANLINE_PAD_DWORD;
    infoPtr->ImageWriteBase = pSmi->DataPortBase;
    infoPtr->ImageWriteRange = pSmi->DataPortSize;
    infoPtr->SetupForImageWrite = SMI_SetupForImageWrite;
    infoPtr->SubsequentImageWriteRect = SMI_SubsequentImageWriteRect;
#endif

    /* Clipping */
    infoPtr->ClippingFlags = HARDWARE_CLIP_SCREEN_TO_SCREEN_COPY
			   | HARDWARE_CLIP_MONO_8x8_FILL
			   | HARDWARE_CLIP_COLOR_8x8_FILL
			   | HARDWARE_CLIP_SOLID_FILL
			   | HARDWARE_CLIP_SOLID_LINE
			   | HARDWARE_CLIP_DASHED_LINE;
    infoPtr->SetClippingRectangle = SMI_SetClippingRectangle;
    infoPtr->DisableClipping = SMI_DisableClipping;

    /* Pixmap Cache */
    if (pScrn->bitsPerPixel == 24) {
	infoPtr->CachePixelGranularity = 16;
    } else {
	infoPtr->CachePixelGranularity = 128 / pScrn->bitsPerPixel;
    }

    /* Offscreen Pixmaps */
    infoPtr->maxOffPixWidth  = 4096;
    infoPtr->maxOffPixHeight = 4096;
    if (pScrn->bitsPerPixel == 24) {
	infoPtr->maxOffPixWidth = 4096 / 3;

	if (pSmi->Chipset == SMI_LYNX) {
	    infoPtr->maxOffPixHeight = 4096 / 3;
	}
    }

    SMI_EngineReset(pScrn);

    ret = XAAInit(pScreen, infoPtr);

    LEAVE(ret);
#else
    return FALSE;
#endif
}


#ifdef HAVE_XAA_H
/******************************************************************************/
/*	Screen to Screen Copies						      */
/******************************************************************************/

static void
SMI_SetupForScreenToScreenCopy(ScrnInfoPtr pScrn, int xdir, int ydir, int rop,
			       unsigned int planemask, int trans)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();
    DEBUG("xdir=%d ydir=%d rop=%02X trans=%08X\n", xdir, ydir, rop, trans);

#if X_BYTE_ORDER == X_BIG_ENDIAN
    if (pScrn->depth >= 24)
	trans = lswapl(trans);
#endif
    pSmi->AccelCmd = XAAGetCopyROP(rop)
		   | SMI_BITBLT
		   | SMI_START_ENGINE;

    if ((xdir == -1) || (ydir == -1)) {
	pSmi->AccelCmd |= SMI_RIGHT_TO_LEFT;
    }

    if (trans != -1) {
	pSmi->AccelCmd |= SMI_TRANSPARENT_SRC | SMI_TRANSPARENT_PXL;
	WaitQueue();
	WRITE_DPR(pSmi, 0x20, trans);
    }

    if (pSmi->ClipTurnedOn) {
	WaitQueue();
	WRITE_DPR(pSmi, 0x2C, pSmi->ScissorsLeft);
	pSmi->ClipTurnedOn = FALSE;
    }

    LEAVE();
}

static void
SMI_SubsequentScreenToScreenCopy(ScrnInfoPtr pScrn, int x1, int y1, int x2,
				 int y2, int w, int h)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();
    DEBUG("x1=%d y1=%d x2=%d y2=%d w=%d h=%d\n", x1, y1, x2, y2, w, h);

    if (pSmi->AccelCmd & SMI_RIGHT_TO_LEFT) {
	x1 += w - 1;
	y1 += h - 1;
	x2 += w - 1;
	y2 += h - 1;
    }

    if (pScrn->bitsPerPixel == 24) {
	x1 *= 3;
	x2 *= 3;
	w  *= 3;

	if (pSmi->Chipset == SMI_LYNX) {
	    y1 *= 3;
	    y2 *= 3;
	}

	if (pSmi->AccelCmd & SMI_RIGHT_TO_LEFT) {
	    x1 += 2;
	    x2 += 2;
	}
    }

    WaitIdle();
    WRITE_DPR(pSmi, 0x00, (x1 << 16) + (y1 & 0xFFFF));
    WRITE_DPR(pSmi, 0x04, (x2 << 16) + (y2 & 0xFFFF));
    WRITE_DPR(pSmi, 0x08, (w  << 16) + (h  & 0xFFFF));
    WRITE_DPR(pSmi, 0x0C, pSmi->AccelCmd);

    LEAVE();
}

/******************************************************************************/
/*   Solid Fills							      */
/******************************************************************************/

static void
SMI_SetupForSolidFill(ScrnInfoPtr pScrn, int color, int rop,
		      unsigned int planemask)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();
    DEBUG("color=%08X rop=%02X\n", color, rop);

    pSmi->AccelCmd = XAAGetPatternROP(rop)
		   | SMI_BITBLT
		   | SMI_START_ENGINE;

#if X_BYTE_ORDER == X_BIG_ENDIAN
    if (pScrn->depth >= 24) {
	/* because of the BGR values are in the MSB bytes,
	 * 'white' is not possible and -1 has a different meaning.
	 * As a work around (assuming white is more used as
	 * light yellow (#FFFF7F), we handle this as beining white.
	 * Thanks to the SM501 not being able to work in MSB on PCI
	 * on the PowerPC */
	if (color == 0x7FFFFFFF)
	    color = -1;
	color = lswapl(color);
    }
#endif
    if (pSmi->ClipTurnedOn) {
	WaitQueue();
	WRITE_DPR(pSmi, 0x2C, pSmi->ScissorsLeft);
	pSmi->ClipTurnedOn = FALSE;
    } else {
	WaitQueue();
    }
    WRITE_DPR(pSmi, 0x14, color);
    WRITE_DPR(pSmi, 0x34, 0xFFFFFFFF);
    WRITE_DPR(pSmi, 0x38, 0xFFFFFFFF);

    LEAVE();
}

void
SMI_SubsequentSolidFillRect(ScrnInfoPtr pScrn, int x, int y, int w, int h)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();
    DEBUG("x=%d y=%d w=%d h=%d\n", x, y, w, h);

    if (pScrn->bitsPerPixel == 24) {
	x *= 3;
	w *= 3;

	if (pSmi->Chipset == SMI_LYNX) {
	    y *= 3;
	}
    }

    if (IS_MSOC(pSmi)) {
	/* Clip to prevent negative screen coordinates */
	if (x < 0)
	    x = 0;
	if (y < 0)
	    y = 0;
    }

    WaitQueue();
    WRITE_DPR(pSmi, 0x04, (x << 16) | (y & 0xFFFF));
    WRITE_DPR(pSmi, 0x08, (w << 16) | (h & 0xFFFF));
    WRITE_DPR(pSmi, 0x0C, pSmi->AccelCmd);

    LEAVE();
}

/******************************************************************************/
/*   Solid Lines							      */
/******************************************************************************/

static void
SMI_SubsequentSolidHorVertLine(ScrnInfoPtr pScrn, int x, int y, int len,
							   int dir)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    int w, h;

    ENTER();
    DEBUG("x=%d y=%d len=%d dir=%d\n", x, y, len, dir);

    if (dir == DEGREES_0) {
	w = len;
	h = 1;
    } else {
	w = 1;
	h = len;
    }

    if (pScrn->bitsPerPixel == 24) {
	x *= 3;
	w *= 3;

	if (pSmi->Chipset == SMI_LYNX) {
	    y *= 3;
	}
    }

    WaitQueue();
    WRITE_DPR(pSmi, 0x04, (x << 16) | (y & 0xFFFF));
    WRITE_DPR(pSmi, 0x08, (w << 16) | (h & 0xFFFF));
    WRITE_DPR(pSmi, 0x0C, pSmi->AccelCmd);

    LEAVE();
}

/******************************************************************************/
/*  Color Expansion Fills						      */
/******************************************************************************/

static void
SMI_SetupForCPUToScreenColorExpandFill(ScrnInfoPtr pScrn, int fg, int bg,
				       int rop, unsigned int planemask)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();
    DEBUG("fg=%08X bg=%08X rop=%02X\n", fg, bg, rop);

#if X_BYTE_ORDER == X_BIG_ENDIAN
    if (pScrn->depth >= 24) {
	/* see remark elswere */
	if (fg == 0x7FFFFFFF)
	    fg = -1;
	fg = lswapl(fg);
	bg = lswapl(bg);
    }
#endif

    pSmi->AccelCmd = XAAGetCopyROP(rop)
		   | SMI_HOSTBLT_WRITE
		   | SMI_SRC_MONOCHROME
		   | SMI_START_ENGINE;

    if (bg == -1) {
	pSmi->AccelCmd |= SMI_TRANSPARENT_SRC;

	WaitQueue();
	WRITE_DPR(pSmi, 0x14, fg);
	WRITE_DPR(pSmi, 0x18, ~fg);
	WRITE_DPR(pSmi, 0x20, fg);
    } else {
#if X_BYTE_ORDER == X_BIG_ENDIAN
	if (bg == 0xFFFFFF7F)
	    bg = -1;
#endif
	WaitQueue();
	WRITE_DPR(pSmi, 0x14, fg);
	WRITE_DPR(pSmi, 0x18, bg);
    }

    LEAVE();
}

void
SMI_SubsequentCPUToScreenColorExpandFill(ScrnInfoPtr pScrn, int x, int y, int w,
					 int h, int skipleft)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();
    DEBUG("x=%d y=%d w=%d h=%d skipleft=%d\n", x, y, w, h, skipleft);

    if (pScrn->bitsPerPixel == 24) {
	x        *= 3;
	w        *= 3;
	skipleft *= 3;

	if (pSmi->Chipset == SMI_LYNX) {
	    y *= 3;
	}
    }

    if (skipleft) {
	WaitQueue();
	WRITE_DPR(pSmi, 0x2C, (pSmi->ScissorsLeft & 0xFFFF0000)
			      | (x + skipleft) | 0x2000);
	pSmi->ClipTurnedOn = TRUE;
    } else {
	if (pSmi->ClipTurnedOn) {
	    WaitQueue();
	    WRITE_DPR(pSmi, 0x2C, pSmi->ScissorsLeft);
	    pSmi->ClipTurnedOn = FALSE;
	} else {
	    WaitQueue();
	}
    }
    WRITE_DPR(pSmi, 0x00, 0);
    WRITE_DPR(pSmi, 0x04, (x << 16) | (y & 0xFFFF));
    WRITE_DPR(pSmi, 0x08, (w << 16) | (h & 0xFFFF));
    WRITE_DPR(pSmi, 0x0C, pSmi->AccelCmd);

    LEAVE();
}

/******************************************************************************/
/* 8x8 Mono Pattern Fills						      */
/******************************************************************************/

static void
SMI_SetupForMono8x8PatternFill(ScrnInfoPtr pScrn, int patx, int paty, int fg,
			       int bg, int rop, unsigned int planemask)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();
    DEBUG("patx=%08X paty=%08X fg=%08X bg=%08X rop=%02X\n",
	  patx, paty, fg, bg, rop);

#if X_BYTE_ORDER == X_BIG_ENDIAN
    if (pScrn->depth >= 24) {
	if (fg == 0x7FFFFFFF)
	    fg = -1;
	fg = lswapl(fg);
	bg = lswapl(bg);
    }
#endif
    pSmi->AccelCmd = XAAGetPatternROP(rop)
		   | SMI_BITBLT
		   | SMI_START_ENGINE;

    if (pSmi->ClipTurnedOn) {
	WaitQueue();
	WRITE_DPR(pSmi, 0x2C, pSmi->ScissorsLeft);
	pSmi->ClipTurnedOn = FALSE;
    }

    if (bg == -1) {
	WaitQueue();
	WRITE_DPR(pSmi, 0x14, fg);
	WRITE_DPR(pSmi, 0x18, ~fg);
	WRITE_DPR(pSmi, 0x20, fg);
	WRITE_DPR(pSmi, 0x34, patx);
	WRITE_DPR(pSmi, 0x38, paty);
    } else {
#if X_BYTE_ORDER == X_BIG_ENDIAN
	if (bg == 0xFFFFFF7F)
	    bg = -1;
#endif
	WaitQueue();
	WRITE_DPR(pSmi, 0x14, fg);
	WRITE_DPR(pSmi, 0x18, bg);
	WRITE_DPR(pSmi, 0x34, patx);
	WRITE_DPR(pSmi, 0x38, paty);
    }

    LEAVE();
}

static void
SMI_SubsequentMono8x8PatternFillRect(ScrnInfoPtr pScrn, int patx, int paty,
				     int x, int y, int w, int h)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();
    DEBUG("x=%d y=%d w=%d h=%d\n", x, y, w, h);

    if (pScrn->bitsPerPixel == 24) {
	x *= 3;
	w *= 3;
	if (pSmi->Chipset == SMI_LYNX) {
	    y *= 3;
	}
    }

    WaitQueue();
    WRITE_DPR(pSmi, 0x04, (x << 16) | (y & 0xFFFF));
    WRITE_DPR(pSmi, 0x08, (w << 16) | (h & 0xFFFF));
    WRITE_DPR(pSmi, 0x0C, pSmi->AccelCmd);

    LEAVE();
}

/******************************************************************************/
/* 8x8 Color Pattern Fills						      */
/******************************************************************************/

static void
SMI_SetupForColor8x8PatternFill(ScrnInfoPtr pScrn, int patx, int paty, int rop,
				unsigned int planemask, int trans_color)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();
    DEBUG("patx=%d paty=%d rop=%02X trans_color=%08X\n",
	  patx, paty, rop, trans_color);

    pSmi->AccelCmd = XAAGetPatternROP(rop)
		   | SMI_BITBLT
		   | SMI_COLOR_PATTERN
		   | SMI_START_ENGINE;

#if X_BYTE_ORDER == X_BIG_ENDIAN
    if (pScrn->depth >= 24)
	trans_color = lswapl(trans_color);
#endif
    if (pScrn->bitsPerPixel <= 16) {
	/* PDR#950 */
	CARD8* pattern = pSmi->FBBase +
	    (patx + paty * pScrn->displayWidth) * pSmi->Bpp;

	WaitIdle();
	WRITE_DPR(pSmi, 0x0C, SMI_BITBLT | SMI_COLOR_PATTERN);
	memcpy(pSmi->DataPortBase, pattern, 8 * pSmi->Bpp * 8);
    } else {
	if (pScrn->bitsPerPixel == 24) {
	    patx *= 3;

	    if (pSmi->Chipset == SMI_LYNX) {
		paty *= 3;
	    }
	}

	WaitQueue();
	WRITE_DPR(pSmi, 0x00, (patx << 16) | (paty & 0xFFFF));
    }

    WaitQueue();

    if (trans_color == -1) {
	pSmi->AccelCmd |= SMI_TRANSPARENT_SRC | SMI_TRANSPARENT_PXL;

	WaitQueue();
	WRITE_DPR(pSmi, 0x20, trans_color);
    }

    if (pSmi->ClipTurnedOn) {
	WaitQueue();
	WRITE_DPR(pSmi, 0x2C, pSmi->ScissorsLeft);
	pSmi->ClipTurnedOn = FALSE;
    }

    LEAVE();
}

static void
SMI_SubsequentColor8x8PatternFillRect(ScrnInfoPtr pScrn, int patx, int paty,
				      int x, int y, int w, int h)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();
    DEBUG("x=%d y=%d w=%d h=%d\n", x, y, w, h);

    if (pScrn->bitsPerPixel == 24) {
	x *= 3;
	w *= 3;

	if (pSmi->Chipset == SMI_LYNX) {
	    y *= 3;
	}
    }

    WaitQueue();
    WRITE_DPR(pSmi, 0x04, (x << 16) | (y & 0xFFFF));
    WRITE_DPR(pSmi, 0x08, (w << 16) | (h & 0xFFFF));	/* PDR#950 */
    WRITE_DPR(pSmi, 0x0C, pSmi->AccelCmd);

    LEAVE();
}

#if SMI_USE_IMAGE_WRITES
/******************************************************************************/
/*  Image Writes							      */
/******************************************************************************/

static void
SMI_SetupForImageWrite(ScrnInfoPtr pScrn, int rop, unsigned int planemask,
		       int trans_color, int bpp, int depth)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();
    DEBUG("rop=%02X trans_color=%08X bpp=%d depth=%d\n",
	  rop, trans_color, bpp, depth);

#if X_BYTE_ORDER == X_BIG_ENDIAN
    if (pScrn->depth >= 24)
	trans_color = lswapl(trans_color);
#endif
    pSmi->AccelCmd = XAAGetCopyROP(rop)
		   | SMI_HOSTBLT_WRITE
		   | SMI_START_ENGINE;

    if (trans_color != -1) {
#if X_BYTE_ORDER == X_BIG_ENDIAN
	if (trans_color == 0xFFFFFF7F)
	    trans_color = -1;
#endif
	pSmi->AccelCmd |= SMI_TRANSPARENT_SRC | SMI_TRANSPARENT_PXL;

	WaitQueue();
	WRITE_DPR(pSmi, 0x20, trans_color);
    }

    LEAVE();
}

static void
SMI_SubsequentImageWriteRect(ScrnInfoPtr pScrn, int x, int y, int w, int h,
			     int skipleft)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();
    DEBUG("x=%d y=%d w=%d h=%d skipleft=%d\n", x, y, w, h, skipleft);

    if (pScrn->bitsPerPixel == 24) {
	x        *= 3;
	w        *= 3;
	skipleft *= 3;

	if (pSmi->Chipset == SMI_LYNX) {
	    y *= 3;
	}
    }

    if (skipleft) {
	WaitQueue();
	WRITE_DPR(pSmi, 0x2C, (pSmi->ScissorsLeft & 0xFFFF0000) |
			      (x + skipleft) | 0x2000);
	pSmi->ClipTurnedOn = TRUE;
    } else {
	if (pSmi->ClipTurnedOn) {
	    WaitQueue();
	    WRITE_DPR(pSmi, 0x2C, pSmi->ScissorsLeft);
	    pSmi->ClipTurnedOn = FALSE;
	} else {
	    WaitQueue();
	}
    }
    WRITE_DPR(pSmi, 0x00, 0);
    WRITE_DPR(pSmi, 0x04, (x << 16) | (y * 0xFFFF));
    WRITE_DPR(pSmi, 0x08, (w << 16) | (h & 0xFFFF));
    WRITE_DPR(pSmi, 0x0C, pSmi->AccelCmd);

    LEAVE();
}
#endif
#endif
