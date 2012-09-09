/*
 * Copyright 2000 ATI Technologies Inc., Markham, Ontario, and
 *                VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *
 * Credits:
 *
 *   Thanks to Ove Kåven <ovek@transgaming.com> for writing the Rage 128
 *   DGA support.  Portions of this file are based on the initialization
 *   code for that driver.
 *
 */

#include <string.h>

				/* Driver data structures */
#include "radeon.h"
#include "radeon_probe.h"
#ifdef XF86DRI
#include "radeon_drm.h"
#endif

				/* X and server generic header files */
#include "xf86.h"

				/* DGA support */
#include "dgaproc.h"


static Bool RADEON_OpenFramebuffer(ScrnInfoPtr, char **, unsigned char **,
				   int *, int *, int *);
static Bool RADEON_SetMode(ScrnInfoPtr, DGAModePtr);
static int  RADEON_GetViewport(ScrnInfoPtr);
static void RADEON_SetViewport(ScrnInfoPtr, int, int, int);
#ifdef USE_XAA
static void RADEON_FillRect(ScrnInfoPtr, int, int, int, int, unsigned long);
static void RADEON_BlitRect(ScrnInfoPtr, int, int, int, int, int, int);
static void RADEON_BlitTransRect(ScrnInfoPtr, int, int, int, int, int, int,
				 unsigned long);
#endif

static DGAModePtr RADEONSetupDGAMode(ScrnInfoPtr pScrn,
				     DGAModePtr modes,
				     int *num,
				     int bitsPerPixel,
				     int depth,
				     Bool pixmap,
				     int secondPitch,
				     unsigned long red,
				     unsigned long green,
				     unsigned long blue,
				     short visualClass)
{
    RADEONInfoPtr   info     = RADEONPTR(pScrn);
    DGAModePtr      newmodes = NULL;
    DGAModePtr      currentMode;
    DisplayModePtr  pMode;
    DisplayModePtr  firstMode;
    unsigned int    size;
    int             pitch;
    int             Bpp      = bitsPerPixel >> 3;

SECOND_PASS:

    pMode = firstMode = pScrn->modes;

    while (1) {
	pitch = pScrn->displayWidth;
	size = pitch * Bpp * pMode->VDisplay;

	if ((!secondPitch || (pitch != secondPitch)) &&
	    (size <= info->FbMapSize)) {

	    if (secondPitch)
		pitch = secondPitch;

	    if (!(newmodes = realloc(modes, (*num + 1) * sizeof(DGAModeRec))))
		break;

	    modes       = newmodes;
	    currentMode = modes + *num;

	    currentMode->mode           = pMode;
	    currentMode->flags          = DGA_CONCURRENT_ACCESS;

	    if (pixmap)
		currentMode->flags     |= DGA_PIXMAP_AVAILABLE;

#ifdef USE_EXA
	    if (info->useEXA) {
		/* We need to fill in RADEON_FillRect and RADEON_BlitRect and
		 * connect them in RADEONDGAInit before turning these on.
		 */
		/*if (info->exa.accel.PrepareSolid && info->exa.accel.Solid)
		    currentMode->flags    |= DGA_FILL_RECT;
		if (info->exa.accel.PrepareCopy && info->exa.accel.Copy)
		    currentMode->flags    |= DGA_BLIT_RECT | DGA_BLIT_RECT_TRANS;*/
	    }
#endif /* USE_EXA */
#ifdef USE_XAA
	    if (!info->useEXA && info->accel_state->accel) {
	      if (info->accel_state->accel->SetupForSolidFill &&
		  info->accel_state->accel->SubsequentSolidFillRect)
		 currentMode->flags    |= DGA_FILL_RECT;
	      if (info->accel_state->accel->SetupForScreenToScreenCopy &&
		  info->accel_state->accel->SubsequentScreenToScreenCopy)
		 currentMode->flags    |= DGA_BLIT_RECT | DGA_BLIT_RECT_TRANS;
	      if (currentMode->flags &
		  (DGA_PIXMAP_AVAILABLE | DGA_FILL_RECT |
		   DGA_BLIT_RECT | DGA_BLIT_RECT_TRANS))
		  currentMode->flags   &= ~DGA_CONCURRENT_ACCESS;
	    }
#endif /* USE_XAA */

	    if (pMode->Flags & V_DBLSCAN)
		currentMode->flags     |= DGA_DOUBLESCAN;
	    if (pMode->Flags & V_INTERLACE)
		currentMode->flags     |= DGA_INTERLACED;

	    currentMode->byteOrder      = pScrn->imageByteOrder;
	    currentMode->depth          = depth;
	    currentMode->bitsPerPixel   = bitsPerPixel;
	    currentMode->red_mask       = red;
	    currentMode->green_mask     = green;
	    currentMode->blue_mask      = blue;
	    currentMode->visualClass    = visualClass;
	    currentMode->viewportWidth  = pMode->HDisplay;
	    currentMode->viewportHeight = pMode->VDisplay;
	    currentMode->xViewportStep  = 8;
	    currentMode->yViewportStep  = 1;
	    currentMode->viewportFlags  = DGA_FLIP_RETRACE;
	    currentMode->offset         = 0;
	    currentMode->address        = (unsigned char*)(uintptr_t)info->LinearAddr;
	    currentMode->bytesPerScanline = pitch * Bpp;
	    currentMode->imageWidth     = pitch;
	    currentMode->imageHeight    = (info->FbMapSize
					   / currentMode->bytesPerScanline);
	    currentMode->pixmapWidth    = currentMode->imageWidth;
	    currentMode->pixmapHeight   = currentMode->imageHeight;
	    currentMode->maxViewportX   = (currentMode->imageWidth
					   - currentMode->viewportWidth);
	    /* this might need to get clamped to some maximum */
	    currentMode->maxViewportY   = (currentMode->imageHeight
					   - currentMode->viewportHeight);
	    (*num)++;
	}

	pMode = pMode->next;
	if (pMode == firstMode)
	    break;
    }

    if (secondPitch) {
	secondPitch = 0;
	goto SECOND_PASS;
    }

    return modes;
}

Bool RADEONDGAInit(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86Screens[pScreen->myNum];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    DGAModePtr     modes = NULL;
    int            num   = 0;

    /* 8 */
    modes = RADEONSetupDGAMode(pScrn, modes, &num, 8, 8,
			       (pScrn->bitsPerPixel == 8),
				((pScrn->bitsPerPixel != 8)
				 ? 0 : pScrn->displayWidth),
			       0, 0, 0, PseudoColor);

    /* 15 */
    modes = RADEONSetupDGAMode(pScrn, modes, &num, 16, 15,
			       (pScrn->bitsPerPixel == 16),
			       ((pScrn->depth != 15)
				? 0 : pScrn->displayWidth),
			       0x7c00, 0x03e0, 0x001f, TrueColor);

    modes = RADEONSetupDGAMode(pScrn, modes, &num, 16, 15,
			       (pScrn->bitsPerPixel == 16),
			       ((pScrn->depth != 15)
				? 0 : pScrn->displayWidth),
			       0x7c00, 0x03e0, 0x001f, DirectColor);

    /* 16 */
    modes = RADEONSetupDGAMode(pScrn, modes, &num, 16, 16,
			       (pScrn->bitsPerPixel == 16),
			       ((pScrn->depth != 16)
				? 0 : pScrn->displayWidth),
			       0xf800, 0x07e0, 0x001f, TrueColor);

    modes = RADEONSetupDGAMode(pScrn, modes, &num, 16, 16,
			       (pScrn->bitsPerPixel == 16),
			       ((pScrn->depth != 16)
				? 0 : pScrn->displayWidth),
			       0xf800, 0x07e0, 0x001f, DirectColor);

    /* 32 */
    modes = RADEONSetupDGAMode(pScrn, modes, &num, 32, 24,
			       (pScrn->bitsPerPixel == 32),
			       ((pScrn->bitsPerPixel != 32)
				? 0 : pScrn->displayWidth),
			       0xff0000, 0x00ff00, 0x0000ff, TrueColor);

    modes = RADEONSetupDGAMode(pScrn, modes, &num, 32, 24,
			       (pScrn->bitsPerPixel == 32),
			       ((pScrn->bitsPerPixel != 32)
				? 0 : pScrn->displayWidth),
			       0xff0000, 0x00ff00, 0x0000ff, DirectColor);

    info->numDGAModes = num;
    info->DGAModes    = modes;

    info->DGAFuncs.OpenFramebuffer       = RADEON_OpenFramebuffer;
    info->DGAFuncs.CloseFramebuffer      = NULL;
    info->DGAFuncs.SetMode               = RADEON_SetMode;
    info->DGAFuncs.SetViewport           = RADEON_SetViewport;
    info->DGAFuncs.GetViewport           = RADEON_GetViewport;

    info->DGAFuncs.Sync                  = NULL;
    info->DGAFuncs.FillRect              = NULL;
    info->DGAFuncs.BlitRect              = NULL;
    info->DGAFuncs.BlitTransRect         = NULL;

#ifdef USE_EXA
    /*info->DGAFuncs.Sync              = info->exa.accel->Sync;*/
    if (info->useEXA) {
	/*if (info->exa.accel.PrepareSolid && info->exa.accel.Solid) {
	    info->DGAFuncs.FillRect      = RADEON_FillRect;
	}
	if (info->exa.accel.PrepareCopy && info->exa.accel.Copy) {
	    info->DGAFuncs.BlitRect      = RADEON_BlitRect;
	}*/
    }
#endif /* USE_EXA */
#ifdef USE_XAA
    if (!info->useEXA && info->accel_state->accel) {
	info->DGAFuncs.Sync              = info->accel_state->accel->Sync;
	if (info->accel_state->accel->SetupForSolidFill &&
	    info->accel_state->accel->SubsequentSolidFillRect)
	    info->DGAFuncs.FillRect      = RADEON_FillRect;
	if (info->accel_state->accel->SetupForScreenToScreenCopy &&
	    info->accel_state->accel->SubsequentScreenToScreenCopy) {
	    info->DGAFuncs.BlitRect      = RADEON_BlitRect;
	    info->DGAFuncs.BlitTransRect = RADEON_BlitTransRect;
	}
    }
#endif /* USE_XAA */

    return DGAInit(pScreen, &info->DGAFuncs, modes, num);
}

static Bool RADEON_SetMode(ScrnInfoPtr pScrn, DGAModePtr pMode)
{
    static RADEONFBLayout  SavedLayouts[MAXSCREENS];
    int                    indx = pScrn->pScreen->myNum;
    RADEONInfoPtr          info = RADEONPTR(pScrn);

    if (!pMode) { /* restore the original mode */
	/* put the ScreenParameters back */
	if (info->DGAactive)
	    memcpy(&info->CurrentLayout, &SavedLayouts[indx],
		   sizeof(RADEONFBLayout));

	pScrn->currentMode = info->CurrentLayout.mode;

	RADEONSwitchMode(SWITCH_MODE_ARGS(pScrn, pScrn->currentMode));
#ifdef XF86DRI
	if (info->directRenderingEnabled) {
	    RADEONCP_STOP(pScrn, info);
	}
#endif
	if (info->accelOn)
	    RADEONEngineInit(pScrn);
#ifdef XF86DRI
	if (info->directRenderingEnabled) {
	    RADEONCP_START(pScrn, info);
	}
#endif
	RADEONAdjustFrame(ADJUST_FRAME_ARGS(pScrn, 0, 0));
	info->DGAactive = FALSE;
    } else {
	if (!info->DGAactive) {  /* save the old parameters */
	    memcpy(&SavedLayouts[indx], &info->CurrentLayout,
		   sizeof(RADEONFBLayout));
	    info->DGAactive = TRUE;
	}

	info->CurrentLayout.bitsPerPixel = pMode->bitsPerPixel;
	info->CurrentLayout.depth        = pMode->depth;
	info->CurrentLayout.displayWidth = (pMode->bytesPerScanline /
					    (pMode->bitsPerPixel >> 3));
	info->CurrentLayout.pixel_bytes  = pMode->bitsPerPixel / 8;
	info->CurrentLayout.pixel_code   = (pMode->bitsPerPixel != 16
					    ? pMode->bitsPerPixel
					    : pMode->depth);
	/* RADEONModeInit() will set the mode field */

	RADEONSwitchMode(SWITCH_MODE_ARGS(pScrn, pMode->mode));

#ifdef XF86DRI
	if (info->directRenderingEnabled) {
	    RADEONCP_STOP(pScrn, info);
	}
#endif
	if (info->accelOn)
	    RADEONEngineInit(pScrn);
#ifdef XF86DRI
	if (info->directRenderingEnabled) {
	    RADEONCP_START(pScrn, info);
	}
#endif
    }

    return TRUE;
}

static int RADEON_GetViewport(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    return info->DGAViewportStatus;
}

static void RADEON_SetViewport(ScrnInfoPtr pScrn, int x, int y, int flags)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    RADEONAdjustFrame(ADJUST_FRAME_ARGS(pScrn, x, y));
    info->DGAViewportStatus = 0;  /* FIXME */
}


#ifdef USE_XAA

static void RADEON_FillRect(ScrnInfoPtr pScrn,
			    int x, int y, int w, int h,
			    unsigned long color)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

#ifdef USE_EXA
    /* XXX */
    if (info->useEXA) {
	/*
	info->exa.accel.PrepareSolid(pScrn, color, GXcopy, (uint32_t)(~0));
	info->exa.accel.Solid(pScrn, x, y, x+w, y+h);
	info->exa.accel.DoneSolid();
	*/
	RADEON_MARK_SYNC(info, pScrn);
    }
#endif /* USE_EXA */
#ifdef USE_XAA
    if (!info->useEXA) {
	(*info->accel_state->accel->SetupForSolidFill)(pScrn, color, GXcopy, (uint32_t)(~0));
	(*info->accel_state->accel->SubsequentSolidFillRect)(pScrn, x, y, w, h);
        if (pScrn->bitsPerPixel == info->CurrentLayout.bitsPerPixel)
	    RADEON_MARK_SYNC(info, pScrn);
    }
#endif /* USE_XAA */

}

static void RADEON_BlitRect(ScrnInfoPtr pScrn,
			    int srcx, int srcy, int w, int h,
			    int dstx, int dsty)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    int            xdir = ((srcx < dstx) && (srcy == dsty)) ? -1 : 1;
    int            ydir = (srcy < dsty) ? -1 : 1;

	#ifdef USE_EXA
    /* XXX */
    if (info->useEXA) {
	/*
	info->exa.accel.PrepareCopy(pScrn, color, GXcopy, (uint32_t)(~0));
	info->exa.accel.Copy(pScrn, srcx, srcy, dstx, dsty, w, h);
	info->exa.accel.DoneCopy();
	*/
	RADEON_MARK_SYNC(info, pScrn);
    }
#endif /* USE_EXA */
#ifdef USE_XAA
    if (!info->useEXA) {
	(*info->accel_state->accel->SetupForScreenToScreenCopy)(pScrn, xdir, ydir,
								GXcopy, (uint32_t)(~0), -1);
	(*info->accel_state->accel->SubsequentScreenToScreenCopy)(pScrn, srcx, srcy,
								  dstx, dsty, w, h);
        if (pScrn->bitsPerPixel == info->CurrentLayout.bitsPerPixel)
	    RADEON_MARK_SYNC(info, pScrn);
    }
#endif /* USE_XAA */
}

static void RADEON_BlitTransRect(ScrnInfoPtr pScrn,
				 int srcx, int srcy, int w, int h,
				 int dstx, int dsty, unsigned long color)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    int            xdir = ((srcx < dstx) && (srcy == dsty)) ? -1 : 1;
    int            ydir = (srcy < dsty) ? -1 : 1;

    info->accel_state->XAAForceTransBlit = TRUE;
    (*info->accel_state->accel->SetupForScreenToScreenCopy)(pScrn, xdir, ydir,
							    GXcopy, (uint32_t)(~0), color);

    info->accel_state->XAAForceTransBlit = FALSE;

    (*info->accel_state->accel->SubsequentScreenToScreenCopy)(pScrn, srcx, srcy,
							      dstx, dsty, w, h);

    if (pScrn->bitsPerPixel == info->CurrentLayout.bitsPerPixel)
        RADEON_MARK_SYNC(info, pScrn);
}

#endif /* USE_XAA */

static Bool RADEON_OpenFramebuffer(ScrnInfoPtr pScrn,
				   char **name,
				   unsigned char **mem,
				   int *size, int *offset, int *flags)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    *name   = NULL;             /* no special device */
    *mem    = (unsigned char*)(uintptr_t)info->LinearAddr;
    *size   = info->FbMapSize;
    *offset = 0;
    *flags  = 0; /* DGA_NEED_ROOT; -- don't need root, just /dev/mem access */

    return TRUE;
}
