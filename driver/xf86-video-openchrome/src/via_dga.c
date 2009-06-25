/*
 * Copyright 1998-2003 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2003 S3 Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xaalocal.h"
#include "via_driver.h"
#include "dgaproc.h"


static Bool VIADGAOpenFramebuffer(ScrnInfoPtr, char **, unsigned char **,
                                  int *, int *, int *);
static Bool VIADGASetMode(ScrnInfoPtr, DGAModePtr);
static int  VIADGAGetViewport(ScrnInfoPtr);
static void VIADGASetViewport(ScrnInfoPtr, int, int, int);

static
DGAFunctionRec VIADGAFuncs = {
    VIADGAOpenFramebuffer,
    NULL,                   /* CloseFrameBuffer */
    VIADGASetMode,
    VIADGASetViewport,
    VIADGAGetViewport,
    viaAccelSyncMarker,
    viaAccelFillRect,
    viaAccelBlitRect,
    NULL                    /* BlitTransRect */
};

#define DGATRACE    4


static DGAModePtr
VIASetupDGAMode(
    ScrnInfoPtr pScrn,
    DGAModePtr modes,
    int *num,
    int bitsPerPixel,
    int depth,
    Bool pixmap,
    int secondPitch,
    unsigned long red,
    unsigned long green,
    unsigned long blue,
    short visualClass
)
{
    VIAPtr pVia = VIAPTR(pScrn);
    DGAModePtr mode, newmodes = NULL;
    DisplayModePtr pMode, firstMode;
    int otherPitch, Bpp = bitsPerPixel >> 3;
    Bool oneMore;

    xf86ErrorFVerb(DGATRACE, "      VIASetupDGAMode\n");

    pMode = firstMode = pScrn->modes;

    /*
     * DGA 1.0 would only provide modes where the depth and stride
     * matched the current desktop.  Some DGA apps might still expect
     * this, so we provide them, too.
     */

    while (pMode) {

        otherPitch = secondPitch ? secondPitch : pMode->HDisplay;

        if (pMode->HDisplay != otherPitch) {
            newmodes = xrealloc(modes, (*num + 2) * sizeof(DGAModeRec));
            oneMore = TRUE;
        }
        else {
            newmodes = xrealloc(modes, (*num + 1) * sizeof(DGAModeRec));
            oneMore = FALSE;
        }

        if (!newmodes) {
            xfree(modes);
            return NULL;
        }

        modes = newmodes;

SECOND_PASS:

        mode = modes + *num;
        (*num)++;

        mode->mode = pMode;
        mode->flags = DGA_CONCURRENT_ACCESS | DGA_PIXMAP_AVAILABLE;

        if(!pVia->NoAccel)
            mode->flags |= DGA_FILL_RECT | DGA_BLIT_RECT;

        if (pMode->Flags & V_DBLSCAN)
            mode->flags |= DGA_DOUBLESCAN;

        if (pMode->Flags & V_INTERLACE)
            mode->flags |= DGA_INTERLACED;

        mode->byteOrder = pScrn->imageByteOrder;
        mode->depth = depth;
        mode->bitsPerPixel = bitsPerPixel;
        mode->red_mask = red;
        mode->green_mask = green;
        mode->blue_mask = blue;
        mode->visualClass = visualClass;
        mode->viewportWidth = pMode->HDisplay;
        mode->viewportHeight = pMode->VDisplay;
        mode->xViewportStep = 2;
        mode->yViewportStep = 1;
        mode->viewportFlags = DGA_FLIP_RETRACE;
        mode->offset = 0;
        mode->address = pVia->FBBase;

        xf86ErrorFVerb(DGATRACE,
                       "VIADGAInit vpWid=%d, vpHgt=%d, Bpp=%d, mdbitsPP=%d\n",
                       mode->viewportWidth,
                       mode->viewportHeight,
                       Bpp,
                       mode->bitsPerPixel);

        if (oneMore) { /* first one is narrow width */
            mode->bytesPerScanline = ((pMode->HDisplay * Bpp) + 3) & ~3L;
            mode->imageWidth = pMode->HDisplay;
            mode->imageHeight =  pMode->VDisplay;
            mode->pixmapWidth = mode->imageWidth;
            mode->pixmapHeight = mode->imageHeight;
            mode->maxViewportX = mode->imageWidth - mode->viewportWidth;

            /* this might need to get clamped to some maximum */
            mode->maxViewportY = mode->imageHeight - mode->viewportHeight;
            oneMore = FALSE;

            xf86ErrorFVerb(DGATRACE,
                           "VIADGAInit 1 imgHgt=%d, stride=%d\n",
                           mode->imageHeight,
                           mode->bytesPerScanline );

            goto SECOND_PASS;
        }
        else {
            mode->bytesPerScanline = ((pScrn->displayWidth * Bpp) + 3) & ~3L;
            mode->imageWidth = pScrn->displayWidth;
            mode->imageHeight = pVia->videoRambytes / mode->bytesPerScanline;
            mode->pixmapWidth = mode->imageWidth;
            mode->pixmapHeight = mode->imageHeight;
            mode->maxViewportX = mode->imageWidth - mode->viewportWidth;
            /* this might need to get clamped to some maximum */
            mode->maxViewportY = mode->imageHeight - mode->viewportHeight;

            xf86ErrorFVerb(DGATRACE,
                           "VIADGAInit 2 imgHgt=%d, stride=%d\n",
                           mode->imageHeight,
                           mode->bytesPerScanline);
        }

        pMode = pMode->next;

        if (pMode == firstMode)
            break;
    }

    return modes;
}


Bool
VIADGAInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    VIAPtr pVia = VIAPTR(pScrn);
    DGAModePtr modes = NULL;
    int num = 0;

    xf86ErrorFVerb(DGATRACE, "      VIADGAInit\n");

    /* 8 */
    modes = VIASetupDGAMode(pScrn, modes, &num, 8, 8,
                            (pScrn->bitsPerPixel == 8),
                            (pScrn->bitsPerPixel != 8) ? 0 : pScrn->displayWidth,
                            0, 0, 0, PseudoColor);

    /* 16 */
    modes = VIASetupDGAMode(pScrn, modes, &num, 16, 16,
                            (pScrn->bitsPerPixel == 16),
                            (pScrn->depth != 16) ? 0 : pScrn->displayWidth,
                            0xf800, 0x07e0, 0x001f, TrueColor);

    modes = VIASetupDGAMode(pScrn, modes, &num, 16, 16,
                            (pScrn->bitsPerPixel == 16),
                            (pScrn->depth != 16) ? 0 : pScrn->displayWidth,
                            0xf800, 0x07e0, 0x001f, DirectColor);

    /* 24-in-32 */
    modes = VIASetupDGAMode(pScrn, modes, &num, 32, 24,
                            (pScrn->bitsPerPixel == 32),
                            (pScrn->bitsPerPixel != 32) ? 0 : pScrn->displayWidth,
                            0xff0000, 0x00ff00, 0x0000ff, TrueColor);

    modes = VIASetupDGAMode(pScrn, modes, &num, 32, 24,
                            (pScrn->bitsPerPixel == 32),
                            (pScrn->bitsPerPixel != 32) ? 0 : pScrn->displayWidth,
                            0xff0000, 0x00ff00, 0x0000ff, DirectColor);

    pVia->numDGAModes = num;
    pVia->DGAModes = modes;

    return DGAInit(pScreen, &VIADGAFuncs, modes, num);
}


static Bool
VIADGASetMode(ScrnInfoPtr pScrn, DGAModePtr pMode)
{
    int index = pScrn->pScreen->myNum;
    VIAPtr pVia = VIAPTR(pScrn);

    if (!pMode) { /* restore the original mode */
        /* put the ScreenParameters back */

        pScrn->displayWidth = pVia->DGAOldDisplayWidth;
        pScrn->bitsPerPixel = pVia->DGAOldBitsPerPixel;
        pScrn->depth = pVia->DGAOldDepth;

        pScrn->SwitchMode(index, pScrn->currentMode, 0);
        if (pVia->hwcursor)
            viaShowCursor(pScrn);

        pVia->DGAactive = FALSE;
    }
    else {
#if 0
        ErrorF("pScrn->bitsPerPixel %d, pScrn->depth %d\n",
               pScrn->bitsPerPixel, pScrn->depth);
        ErrorF(" want  bitsPerPixel %d,  want  depth %d\n",
               pMode->bitsPerPixel, pMode->depth);
#endif

        if (pVia->hwcursor)
            viaHideCursor(pScrn);

        if (!pVia->DGAactive) {  /* save the old parameters */
            pVia->DGAOldDisplayWidth = pScrn->displayWidth;
            pVia->DGAOldBitsPerPixel = pScrn->bitsPerPixel;
            pVia->DGAOldDepth = pScrn->depth;

            pVia->DGAactive = TRUE;
        }

        pScrn->bitsPerPixel = pMode->bitsPerPixel;
        pScrn->depth = pMode->depth;
        pScrn->displayWidth = pMode->bytesPerScanline /
                              (pMode->bitsPerPixel >> 3);

        pScrn->SwitchMode(index, pMode->mode, 0);
    }

    return TRUE;
}


static int
VIADGAGetViewport(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    return pVia->DGAViewportStatus;
}


static void
VIADGASetViewport(ScrnInfoPtr pScrn, int x, int y, int flags)
{
    VIAPtr pVia = VIAPTR(pScrn);

    pScrn->AdjustFrame(pScrn->pScreen->myNum, x, y, flags);
    pVia->DGAViewportStatus = 0;  /* MGAAdjustFrame loops until finished */
}

static Bool
VIADGAOpenFramebuffer(
    ScrnInfoPtr pScrn,
    char **name,
    unsigned char **mem,
    int *size,
    int *offset,
    int *flags)
{
    VIAPtr pVia = VIAPTR(pScrn);

    *name = NULL;    /* no special device */
    *mem = (unsigned char*)pVia->FrameBufferBase;
    *size = pVia->videoRambytes;
    *offset = 0;
    *flags = DGA_NEED_ROOT;

    return TRUE;
}
