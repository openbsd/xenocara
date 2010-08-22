/*
 * Copyright (c) 2007 NVIDIA, Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#include <xf86_OSproc.h>

#include "g80_type.h"
#include "g80_display.h"
#include "g80_output.h"

static void
G80DacSetPClk(xf86OutputPtr output, int pclk)
{
    G80Ptr pNv = G80PTR(output->scrn);
    G80OutputPrivPtr pPriv = output->driver_private;
    const int orOff = 0x800 * pPriv->or;

    pNv->reg[(0x00614280+orOff)/4] = 0;
}

static void
G80DacDPMSSet(xf86OutputPtr output, int mode)
{
    G80Ptr pNv = G80PTR(output->scrn);
    G80OutputPrivPtr pPriv = output->driver_private;
    const int off = 0x800 * pPriv->or;
    CARD32 tmp;

    /*
     * DPMSModeOn       everything on
     * DPMSModeStandby  hsync disabled, vsync enabled
     * DPMSModeSuspend  hsync enabled, vsync disabled
     * DPMSModeOff      sync disabled
     */
    while(pNv->reg[(0x0061A004+off)/4] & 0x80000000);

    tmp = pNv->reg[(0x0061A004+off)/4];
    tmp &= ~0x7f;
    tmp |= 0x80000000;

    if(mode == DPMSModeStandby || mode == DPMSModeOff)
        tmp |= 1;
    if(mode == DPMSModeSuspend || mode == DPMSModeOff)
        tmp |= 4;
    if(mode != DPMSModeOn)
        tmp |= 0x10;
    if(mode == DPMSModeOff)
        tmp |= 0x40;

    pNv->reg[(0x0061A004+off)/4] = tmp;
}

static Bool
G80DacModeFixup(xf86OutputPtr output, DisplayModePtr mode,
                DisplayModePtr adjusted_mode)
{
    return TRUE;
}

static void
G80DacModeSet(xf86OutputPtr output, DisplayModePtr mode,
              DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr pScrn = output->scrn;
    G80OutputPrivPtr pPriv = output->driver_private;
    const int dacOff = 0x80 * pPriv->or;

    if(!adjusted_mode) {
        C(0x00000400 + dacOff, 0);
        return;
    }

    // This wouldn't be necessary, but the server is stupid and calls
    // G80DacDPMSSet after the output is disconnected, even though the hardware
    // turns it off automatically.
    G80DacDPMSSet(output, DPMSModeOn);

    C(0x00000400 + dacOff,
        (G80CrtcGetHead(output->crtc) == HEAD0 ? 1 : 2) | 0x40);
    C(0x00000404 + dacOff,
        (adjusted_mode->Flags & V_NHSYNC) ? 1 : 0 |
        (adjusted_mode->Flags & V_NVSYNC) ? 2 : 0);

    G80CrtcSetScale(output->crtc, adjusted_mode, G80_SCALE_OFF);
}

/*
 * Perform DAC load detection to determine if there is a connected display.
 */
static xf86OutputStatus
G80DacDetect(xf86OutputPtr output)
{
    G80OutputPrivPtr pPriv = output->driver_private;

    /* Assume physical status isn't going to change before the BlockHandler */
    if(pPriv->cached_status != XF86OutputStatusUnknown)
        return pPriv->cached_status;

    G80OutputPartnersDetect(output, pPriv->partner, pPriv->i2c);
    return pPriv->cached_status;
}

Bool
G80DacLoadDetect(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    G80Ptr pNv = G80PTR(pScrn);
    G80OutputPrivPtr pPriv = output->driver_private;
    const int scrnIndex = pScrn->scrnIndex;
    const int dacOff = 2048 * pPriv->or;
    int sigstate;
    CARD32 load, tmp;

    xf86DrvMsg(scrnIndex, X_PROBED, "Trying load detection on VGA%i ... ",
            pPriv->or);

    pNv->reg[(0x0061A010+dacOff)/4] = 0x00000001;
    tmp = pNv->reg[(0x0061A004+dacOff)/4];
    pNv->reg[(0x0061A004+dacOff)/4] = 0x80150000;
    while(pNv->reg[(0x0061A004+dacOff)/4] & 0x80000000);
    pNv->reg[(0x0061A00C+dacOff)/4] = pNv->loadVal | 0x100000;
    sigstate = xf86BlockSIGIO();
    usleep(45000);
    xf86UnblockSIGIO(sigstate);
    load = pNv->reg[(0x0061A00C+dacOff)/4];
    pNv->reg[(0x0061A00C+dacOff)/4] = 0;
    pNv->reg[(0x0061A004+dacOff)/4] = 0x80000000 | tmp;

    // Use this DAC if all three channels show load.
    if((load & 0x38000000) == 0x38000000) {
        xf86ErrorF("found one!\n");
        return TRUE;
    }

    xf86ErrorF("nothing.\n");
    return FALSE;
}

static void
G80DacDestroy(xf86OutputPtr output)
{
    G80OutputDestroy(output);

    free(output->driver_private);
    output->driver_private = NULL;
}

static const xf86OutputFuncsRec G80DacOutputFuncs = {
    .dpms = G80DacDPMSSet,
    .save = NULL,
    .restore = NULL,
    .mode_valid = G80OutputModeValid,
    .mode_fixup = G80DacModeFixup,
    .prepare = G80OutputPrepare,
    .commit = G80OutputCommit,
    .mode_set = G80DacModeSet,
    .detect = G80DacDetect,
    .get_modes = G80OutputGetDDCModes,
    .destroy = G80DacDestroy,
};

xf86OutputPtr
G80CreateDac(ScrnInfoPtr pScrn, ORNum or)
{
    G80OutputPrivPtr pPriv = xnfcalloc(sizeof(*pPriv), 1);
    xf86OutputPtr output;
    char orName[5];

    if(!pPriv)
        return NULL;

    snprintf(orName, 5, "VGA%i", or);
    output = xf86OutputCreate(pScrn, &G80DacOutputFuncs, orName);

    pPriv->type = DAC;
    pPriv->or = or;
    pPriv->cached_status = XF86OutputStatusUnknown;
    pPriv->set_pclk = G80DacSetPClk;
    output->driver_private = pPriv;
    output->interlaceAllowed = TRUE;
    output->doubleScanAllowed = TRUE;

    return output;
}
