/*
 * Acceleration for the Creator and Creator3D framebuffer - DGA support.
 *
 * Copyright (C) 2000 David S. Miller (davem@redhat.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * DAVID MILLER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "dgaproc.h"

#include "ffb.h"
#include "ffb_regs.h"
#include "ffb_rcache.h"
#include "ffb_fifo.h"
#include "ffb_loops.h"

static Bool FFB_OpenFramebuffer(ScrnInfoPtr, char **, unsigned char **,
				int *, int *, int *);
static void FFB_CloseFramebuffer(ScrnInfoPtr);
static Bool FFB_SetMode(ScrnInfoPtr, DGAModePtr);
static void FFB_SetViewport(ScrnInfoPtr, int, int, int);
static int FFB_GetViewport(ScrnInfoPtr);
static void FFB_Flush(ScrnInfoPtr);
#ifdef HAVE_XAA_H
static void FFB_FillRect(ScrnInfoPtr, int, int, int, int, unsigned long);
static void FFB_BlitRect(ScrnInfoPtr, int, int, int, int,
			 int, int);
#endif

static DGAFunctionRec FFB_DGAFuncs = {
	FFB_OpenFramebuffer,
	FFB_CloseFramebuffer,
	FFB_SetMode,
	FFB_SetViewport,
	FFB_GetViewport,
	FFB_Flush,
#ifdef HAVE_XAA_H
	FFB_FillRect,
	FFB_BlitRect,
#else
	NULL, NULL,
#endif
	NULL
};

void FFB_InitDGA(ScreenPtr pScreen)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	FFBPtr pFfb;
	DGAModePtr mode;
	Bool result;

	pFfb = GET_FFB_FROM_SCRN(pScrn);

	mode = xnfcalloc(sizeof(DGAModeRec), 1);
	if (!mode) {
		xf86Msg(X_WARNING, "%s: DGA init failed, cannot alloc DGAMode.\n",
			pFfb->psdp->device);
		return;
	}

	mode->num = 0;
	mode->mode = pScrn->modes;

	/* Hmmm, what does concurrent access really mean? -DaveM */
	mode->flags = (DGA_CONCURRENT_ACCESS | DGA_PIXMAP_AVAILABLE | DGA_FILL_RECT);
	mode->flags |= DGA_BLIT_RECT;

	mode->imageWidth = 2048;
	mode->imageHeight = 2048;
	mode->pixmapWidth = 2048;
	mode->pixmapHeight = 2048;

	/* XXX I would imagine that this value states how many bytes
	 * XXX you add to advance exactly one full horizontal line in
	 * XXX the framebuffer addressing, but the way we set the pScrn
	 * XXX mode values do not match that definition.
	 * XXX
	 * XXX Ask Jakub what is going on here. -DaveM
	 */
#if 1
	mode->bytesPerScanline = pScrn->modes->HDisplay * 4;
#else
	mode->bytesPerScanline = (2048 * 4);
#endif

	mode->byteOrder = pScrn->imageByteOrder;
	mode->depth = 32;
	mode->bitsPerPixel = 32;
	mode->red_mask = 0xff;
	mode->green_mask = 0xff00;
	mode->blue_mask = 0xff0000;
	mode->visualClass = TrueColor;
	mode->viewportWidth = pScrn->modes->HDisplay;
	mode->viewportHeight = pScrn->modes->VDisplay;

	/* Do these values even matter if we do not support
	 * viewports? -DaveM
	 */
	mode->xViewportStep = 0;
	mode->yViewportStep = 0;
	mode->maxViewportX = 0;
	mode->maxViewportY = 0;

	mode->viewportFlags = 0;
	mode->offset = 0;

	result = DGAInit(pScreen, &FFB_DGAFuncs, mode, 1);
	if (result == FALSE) {
		xf86Msg(X_WARNING,
			"%s: DGA init failed, DGAInit returns FALSE.\n",
			pFfb->psdp->device);
	} else {
		xf86Msg(X_INFO, "%s: DGA support initialized.\n",
			pFfb->psdp->device);
	}
}

static Bool FFB_OpenFramebuffer(ScrnInfoPtr pScrn, char **name, unsigned char **mem,
				int *size, int *offset, int *extra)
{
	FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);

	*name = pFfb->psdp->device;

	/* We give the user the dumb frame buffer. */
	*mem = (unsigned char *)FFB_DFB24_VOFF;
	*size = 0x1000000;
	*offset = 0;
	*extra = 0;

	return TRUE;
}

static void FFB_CloseFramebuffer(ScrnInfoPtr pScrn)
{
}

static Bool FFB_SetMode(ScrnInfoPtr pScrn, DGAModePtr pMode)
{
	/* Nothing to do, we currently only support one mode
	 * and we are always in it.
	 */
	return TRUE;
}

static void FFB_SetViewport(ScrnInfoPtr pScrn, int x, int y, int flags)
{
	/* We don't support viewports, so... */
}

static int FFB_GetViewport(ScrnInfoPtr pScrn)
{
	/* No viewports, none pending... */
	return 0;
}

static void FFB_Flush(ScrnInfoPtr pScrn)
{
	FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
	ffb_fbcPtr ffb = pFfb->regs;

	FFBWait(pFfb, ffb);
}

#ifdef HAVE_XAA_H
extern void FFB_SetupForSolidFill(ScrnInfoPtr, int, int, unsigned int);
extern void FFB_SubsequentSolidFillRect(ScrnInfoPtr, int, int, int, int);

static void FFB_FillRect(ScrnInfoPtr pScrn, int x, int y, int w, int h, unsigned long color)
{
	FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);

	FFB_SetupForSolidFill(pScrn, color, GXcopy, ~0);
	FFB_SubsequentSolidFillRect(pScrn, x, y, w, h);
	SET_SYNC_FLAG(pFfb->pXAAInfo);
}

extern void FFB_SetupForScreenToScreenCopy(ScrnInfoPtr, int, int, int,
					   unsigned int, int);
extern void FFB_SubsequentScreenToScreenCopy(ScrnInfoPtr, int, int,
					     int, int, int, int);

static void FFB_BlitRect(ScrnInfoPtr pScrn, int srcx, int srcy,
			 int w, int h, int dstx, int dsty)
{
	FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);
	int xdir = ((srcx < dstx) && (srcy == dsty)) ? -1 : 1;
	int ydir = (srcy < dsty) ? -1 : 1;

	FFB_SetupForScreenToScreenCopy(pScrn, xdir, ydir, GXcopy, ~0, -1);
	FFB_SubsequentScreenToScreenCopy(pScrn, srcx, srcy, dstx,dsty, w, h);
	SET_SYNC_FLAG(pFfb->pXAAInfo);
}
#endif
