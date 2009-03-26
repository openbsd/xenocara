/*
Copyright (C) 2008 Francisco Jerez.  All Rights Reserved.

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
*/

#ifndef _SMI_CRTC_H
#define _SMI_CRTC_H

#include "xf86Crtc.h"

typedef struct {
    /* Memory copy of the CRTC color palette */
    CARD16 lut_r[256],lut_g[256],lut_b[256];
    /* Allocated memory area used as shadow pixmap (for rotation) */
    void* shadowArea;

    /* Setup the CRTC registers to show the specified framebuffer location*/
    void (*adjust_frame)(xf86CrtcPtr crtc, int x, int y);
    /* Setup the CRTC framebuffer format. Called when the FB is
       resized to modify the screen stride */
    void (*video_init)(xf86CrtcPtr crtc);
    /* Load the LUT fields above to the hardware */
    void (*load_lut)(xf86CrtcPtr crtc);

#if SMI_CURSOR_ALPHA_PLANE
    Bool	argb_cursor;
#endif
} SMICrtcPrivateRec, *SMICrtcPrivatePtr;

#define SMICRTC(crtc) ((SMICrtcPrivatePtr)(crtc)->driver_private)

/* smi_crtc.c */
/* Initialize the xf86CrtcFuncsRec with functions common to all the hardware */
void SMI_CrtcFuncsInit_base(xf86CrtcFuncsPtr* crtcFuncs, SMICrtcPrivatePtr* crtcPriv);
/* Create and initialize the display controllers. */
Bool SMI_CrtcPreInit(ScrnInfoPtr pScrn);

/* smi_output.c */
/* Initialize the xf86OutputFuncsRec with functions common to all the hardware */
void SMI_OutputFuncsInit_base(xf86OutputFuncsPtr* outputFuncs);
/* Create and initialize the video outputs. */
Bool SMI_OutputPreInit(ScrnInfoPtr pScrn);

/* Output detect dummy implementation that always returns connected. */
xf86OutputStatus SMI_OutputDetect_lcd(xf86OutputPtr output);
/* Output get_modes implementation that returns the LCD native mode */
DisplayModePtr SMI_OutputGetModes_native(xf86OutputPtr output);

#endif
