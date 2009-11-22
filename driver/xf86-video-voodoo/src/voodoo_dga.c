/* 
 *  XFree86 driver for the Voodoo 2 using native X support and no
 * Glide2. This is done for two reasons, firstly Glide2 is not portable
 * to other than x86_32 which is becoming an issue, and secondly to get
 * accelerations that Glide does not expose.  The Voodoo 2 hardware has
 * bit blit (screen->screen, cpu->screen), some colour expansion and 
 * also alpha (so could do hw render even!). Also can in theory use
 * texture ram and engine to do arbitary Xv support as we have
 * colour match on the 2D blit (ie 3D blit to back, 2D blit to front)
 * along with alpha on the Xv 8) and with some care rotation of Xv.
 * 
 * Alan Cox <alan@redhat.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of Red Hat, Alan Cox and Henrik Harmsen
 * not be used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.  Th authors make no 
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL RICHARD HECKER BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 * 
 * THIS SOFTWARE IS NOT DESIGNED FOR USE IN SAFETY CRITICAL SYSTEMS OF
 * ANY KIND OR FORM.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "fb.h"
#include "mibank.h"
#include "micmap.h"
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86PciInfo.h"
#include "xf86Pci.h"
#include "xf86cmap.h"
#include "shadowfb.h"
#include "vgaHW.h"
#include "compiler.h"
#include "xaa.h"
#include "dgaproc.h"

#include "voodoo.h"

#define _XF86DGA_SERVER_
#include <X11/extensions/xf86dgastr.h>

#include "opaque.h"
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif



 /***********************************************************************
 * DGA stuff
 ***********************************************************************/
static Bool VoodooDGAOpenFramebuffer(ScrnInfoPtr pScrn, char **DeviceName,
				   unsigned char **ApertureBase,
				   int *ApertureSize, int *ApertureOffset,
				   int *flags);
static Bool VoodooDGASetMode(ScrnInfoPtr pScrn, DGAModePtr pDGAMode);

static Bool VoodooDGAOpenFramebuffer(ScrnInfoPtr pScrn, char **DeviceName,
		       unsigned char **ApertureBase, int *ApertureSize,
		       int *ApertureOffset, int *flags)
{
    *DeviceName = NULL;		/* No special device */
    *ApertureBase = (unsigned char *)(pScrn->memPhysBase);
    *ApertureSize = pScrn->videoRam;
    *ApertureOffset = pScrn->fbOffset;
    *flags = 0;

    return TRUE;
}

static Bool VoodooDGASetMode(ScrnInfoPtr pScrn, DGAModePtr pDGAMode)
{
    DisplayModePtr pMode;
    int scrnIdx = pScrn->pScreen->myNum;
    int frameX0, frameY0;

    if (pDGAMode) {
	pMode = pDGAMode->mode;
	frameX0 = frameY0 = 0;
    }
    else {
	if (!(pMode = pScrn->currentMode))
	    return TRUE;

	frameX0 = pScrn->frameX0;
	frameY0 = pScrn->frameY0;
    }

    if (!(*pScrn->SwitchMode)(scrnIdx, pMode, 0))
	return FALSE;
    return TRUE;
}

static int VoodooDGAGetViewport(ScrnInfoPtr pScrn)
{
    return (0);
}

static DGAFunctionRec VoodooDGAFunctions =
{
    VoodooDGAOpenFramebuffer,
    NULL,       /* CloseFramebuffer */
    VoodooDGASetMode,
    NULL,
    VoodooDGAGetViewport,
    /* TODO - can do Sync/blit/fill on Voodoo2 */
    NULL,       /* Sync */
    NULL,       /* FillRect */
    NULL,       /* BlitRect */
    NULL,       /* BlitTransRect */
};

static void VoodooDGAAddModes(ScrnInfoPtr pScrn)
{
    VoodooPtr pVoo = VoodooPTR(pScrn);
    DisplayModePtr pMode = pScrn->modes;
    DGAModePtr pDGAMode;

    do {
	pDGAMode = xrealloc(pVoo->pDGAMode,
			    (pVoo->nDGAMode + 1) * sizeof(DGAModeRec));
	if (!pDGAMode)
	    break;

	pVoo->pDGAMode = pDGAMode;
	pDGAMode += pVoo->nDGAMode;
	(void)memset(pDGAMode, 0, sizeof(DGAModeRec));

	++pVoo->nDGAMode;
	pDGAMode->mode = pMode;
	pDGAMode->flags = DGA_CONCURRENT_ACCESS | DGA_PIXMAP_AVAILABLE;
	pDGAMode->byteOrder = pScrn->imageByteOrder;
	pDGAMode->depth = pScrn->depth;
	pDGAMode->bitsPerPixel = pScrn->bitsPerPixel;
	pDGAMode->red_mask = pScrn->mask.red;
	pDGAMode->green_mask = pScrn->mask.green;
	pDGAMode->blue_mask = pScrn->mask.blue;
	pDGAMode->visualClass = TrueColor;
	pDGAMode->xViewportStep = 1;
	pDGAMode->yViewportStep = 1;
	pDGAMode->viewportWidth = pMode->HDisplay;
	pDGAMode->viewportHeight = pMode->VDisplay;

	pDGAMode->bytesPerScanline = 2048;

	pDGAMode->imageWidth = pMode->HDisplay;
	pDGAMode->imageHeight =  pMode->VDisplay;
	pDGAMode->pixmapWidth = pDGAMode->imageWidth;
	pDGAMode->pixmapHeight = pDGAMode->imageHeight;
	pDGAMode->maxViewportX = pScrn->virtualX -
				    pDGAMode->viewportWidth;
	pDGAMode->maxViewportY = pScrn->virtualY -
				    pDGAMode->viewportHeight;

	pDGAMode->address = pVoo->FBBase;

	pMode = pMode->next;
    } while (pMode != pScrn->modes);
}

Bool VoodooDGAInit(ScrnInfoPtr pScrn, ScreenPtr pScreen)
{
    VoodooPtr pVoo = VoodooPTR(pScrn);
    if (!pVoo->nDGAMode)
	VoodooDGAAddModes(pScrn);
    return (DGAInit(pScreen, &VoodooDGAFunctions,
	    pVoo->pDGAMode, pVoo->nDGAMode));
}
