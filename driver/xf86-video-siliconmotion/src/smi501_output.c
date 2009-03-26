/*
Copyright (C) 1994-1999 The XFree86 Project, Inc.  All Rights Reserved.
Copyright (C) 2000 Silicon Motion, Inc.  All Rights Reserved.
Copyright (C) 2008 Mandriva Linux.  All Rights Reserved.
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
#include "smi_501.h"


static void
SMI501_OutputDPMS_lcd(xf86OutputPtr output, int dpms)
{
    ScrnInfoPtr		pScrn = output->scrn;
    SMIPtr		pSmi = SMIPTR(pScrn);
    MSOCRegPtr		mode = pSmi->mode;

    ENTER();

    mode->system_ctl.value = READ_SCR(pSmi, SYSTEM_CTL);
    switch (dpms) {
    case DPMSModeOn:
	SMI501_PowerPanel(pScrn, mode, TRUE);
    case DPMSModeStandby:
	break;
    case DPMSModeSuspend:
	break;
    case DPMSModeOff:
	SMI501_PowerPanel(pScrn, mode, FALSE);
	break;
    }

    LEAVE();
}

static void
SMI501_OutputDPMS_crt(xf86OutputPtr output, int dpms)
{
    ScrnInfoPtr pScrn = output->scrn;
    SMIPtr pSmi = SMIPTR(pScrn);
    MSOCRegPtr		mode = pSmi->mode;

    ENTER();

    mode->system_ctl.value = READ_SCR(pSmi, SYSTEM_CTL);
    switch (dpms) {
    case DPMSModeOn:
	mode->system_ctl.f.dpmsh = 0;
	mode->system_ctl.f.dpmsv = 0;
	break;
    case DPMSModeStandby:
	mode->system_ctl.f.dpmsh = 1;
	mode->system_ctl.f.dpmsv = 0;
	break;
    case DPMSModeSuspend:
	mode->system_ctl.f.dpmsh = 0;
	mode->system_ctl.f.dpmsv = 1;
	break;
    case DPMSModeOff:
	mode->system_ctl.f.dpmsh = 1;
	mode->system_ctl.f.dpmsv = 1;
	break;
    }
    WRITE_SCR(pSmi, SYSTEM_CTL, mode->system_ctl.value);

    LEAVE();
}

#ifdef USE_CRTC_DETECT
static xf86OutputStatus
SMI501_OutputDetect_crt(xf86OutputPtr output)
{
    ScrnInfoPtr		pScrn = output->scrn;
    SMIPtr		pSmi = SMIPTR(pScrn);
    MSOCRegPtr		mode = pSmi->mode;
    xf86OutputStatus	status;

    ENTER();

    mode->crt_detect.value = READ_SCR(pSmi, CRT_DETECT);
    mode->crt_detect.f.enable = 1;
    WRITE_SCR(pSmi, CRT_DETECT, mode->crt_detect.value);
    SMI501_WaitVSync(pSmi, 1);

    mode->crt_detect.value = READ_SCR(pSmi, CRT_DETECT);

    /* FIXME This appears to have a fixed value, whatever I do, and
     * the binary pattern is 1000000010000100
     * regardless of crt being connected or not, so maybe this is
     * just telling there is a VGA output?
     */
    status = mode->crt_detect.f.data ?
	XF86OutputStatusConnected : XF86OutputStatusUnknown;

    mode->crt_detect.f.enable = 0;
    WRITE_SCR(pSmi, CRT_DETECT, mode->crt_detect.value);
    SMI501_WaitVSync(pSmi, 1);

    LEAVE(status);
}
#endif

Bool
SMI501_OutputPreInit(ScrnInfoPtr pScrn)
{
    SMIPtr		pSmi = SMIPTR(pScrn);
    xf86OutputPtr	output;
    xf86OutputFuncsPtr	outputFuncs;

    ENTER();

    /* CRTC0 is LCD */
    SMI_OutputFuncsInit_base(&outputFuncs);
    outputFuncs->dpms		= SMI501_OutputDPMS_lcd;
    outputFuncs->get_modes	= SMI_OutputGetModes_native;
    outputFuncs->detect		= SMI_OutputDetect_lcd;

    if (! (output = xf86OutputCreate(pScrn, outputFuncs, "LVDS")))
	LEAVE(FALSE);

    output->possible_crtcs = 1 << 0;
    output->possible_clones = 0;
    output->interlaceAllowed = FALSE;
    output->doubleScanAllowed = FALSE;

    /* CRTC1 is CRT */
    if (pSmi->Dualhead) {
	SMI_OutputFuncsInit_base(&outputFuncs);
	outputFuncs->dpms	= SMI501_OutputDPMS_crt;
	outputFuncs->get_modes	= SMI_OutputGetModes_native;
#ifdef USE_CRTC_DETECT
	outputFuncs->detect	= SMI501_OutputDetect_crt;
#endif

	if (! (output = xf86OutputCreate(pScrn, outputFuncs, "VGA")))
	    LEAVE(FALSE);

	output->possible_crtcs = 1 << 1;
	output->possible_clones = 0;
	output->interlaceAllowed = FALSE;
	output->doubleScanAllowed = FALSE;
    }

    LEAVE(TRUE);
}
