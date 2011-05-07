/*
Copyright (C) 1994-1999 The XFree86 Project, Inc.  All Rights Reserved.
Copyright (C) 2000 Silicon Motion, Inc.  All Rights Reserved.
Copyright (C) 2008 Francisco Jerez. All Rights Reserved.

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

Except as contained in this notice, the names of The XFree86 Project and
Silicon Motion shall not be used in advertising or otherwise to promote the
sale, use or other dealings in this Software without prior written
authorization from The XFree86 Project or Silicon Motion.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "smi.h"
#include "smi_crtc.h"
#include "smilynx.h"
#include "smi_501.h"

static void
SMI_OutputCreateResources(xf86OutputPtr output)
{
    ENTER();
    /* Nothing */
    LEAVE();
}

static int
SMI_OutputModeValid(xf86OutputPtr output, DisplayModePtr mode)
{
    ScrnInfoPtr pScrn = output->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    /* FIXME May also need to test for IS_MSOC(pSmi) here.
     * Only accept modes matching the panel size because the panel cannot
     * be centered neither shrinked/expanded due to hardware bugs.
     * Note that as long as plane tr/br and plane window x/y are set to 0
     * and the mode height matches the panel height, it will work and
     * set the mode, but at offset 0, and properly program the crt.
     * But use panel dimensions so that "full screen" programs will do
     * their own centering. */
    if (output->name && strcmp(output->name, "LVDS") == 0 &&
	(mode->HDisplay != pSmi->lcdWidth || mode->VDisplay != pSmi->lcdHeight))
	LEAVE(MODE_PANEL);

    /* The driver is actually programming modes, instead of loading registers
     * state from static tables. But still, only accept modes that should
     * be handled correctly by all hardwares. On the MSOC, currently, only
     * the crt can be programmed to different resolution modes.
     */
    if (mode->HDisplay & 15)
	LEAVE(MODE_BAD_WIDTH);

    if((mode->Clock < pSmi->clockRange.minClock) ||
       (mode->Clock > pSmi->clockRange.maxClock) ||
       ((mode->Flags & V_INTERLACE) && !pSmi->clockRange.interlaceAllowed) ||
       ((mode->Flags & V_DBLSCAN) && (mode->VScan > 1) && !pSmi->clockRange.doubleScanAllowed)){
	LEAVE(MODE_CLOCK_RANGE);
    }


    LEAVE(MODE_OK);
}

static Bool
SMI_OutputModeFixup(xf86OutputPtr output, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
    ENTER();

    /* Nothing */

    LEAVE(TRUE);
}

static void
SMI_OutputPrepare(xf86OutputPtr output)
{
    ENTER();

    /* Nothing */

    LEAVE();
}

static void
SMI_OutputCommit(xf86OutputPtr output)
{
    ENTER();

    output->funcs->dpms(output,DPMSModeOn);

    LEAVE();
}

static void
SMI_OutputModeSet(xf86OutputPtr output, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
    ENTER();

    /* Nothing */

    LEAVE();
}

static xf86OutputStatus
SMI_OutputDetect(xf86OutputPtr output)
{
    ENTER();

    LEAVE(XF86OutputStatusUnknown);
}

xf86OutputStatus
SMI_OutputDetect_lcd(xf86OutputPtr output)
{
    ENTER();

    LEAVE(XF86OutputStatusConnected);
}

DisplayModePtr
SMI_OutputGetModes_native(xf86OutputPtr output)
{
    SMIPtr pSmi = SMIPTR(output->scrn);
    ENTER();

#ifdef HAVE_XMODES
    LEAVE(xf86CVTMode(pSmi->lcdWidth, pSmi->lcdHeight, 60.0f, FALSE, FALSE));
#else
    LEAVE(NULL);
#endif
}

static void
SMI_OutputDestroy(xf86OutputPtr output)
{
    ENTER();

    free((xf86OutputFuncsPtr)output->funcs);

    LEAVE();
}

void
SMI_OutputFuncsInit_base(xf86OutputFuncsPtr* outputFuncs)
{
    *outputFuncs = xnfcalloc(sizeof(xf86OutputFuncsRec), 1);

    (*outputFuncs)->create_resources = SMI_OutputCreateResources;
    (*outputFuncs)->mode_valid = SMI_OutputModeValid;
    (*outputFuncs)->mode_fixup = SMI_OutputModeFixup;
    (*outputFuncs)->prepare = SMI_OutputPrepare;
    (*outputFuncs)->commit = SMI_OutputCommit;
    (*outputFuncs)->mode_set = SMI_OutputModeSet;
    (*outputFuncs)->detect = SMI_OutputDetect;
    (*outputFuncs)->destroy = SMI_OutputDestroy;
}

Bool
SMI_OutputPreInit(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    if(SMI_MSOC_SERIES(pSmi->Chipset)){
	LEAVE( SMI501_OutputPreInit(pScrn) );
    }else{
	LEAVE( SMILynx_OutputPreInit(pScrn) );
    }
}

