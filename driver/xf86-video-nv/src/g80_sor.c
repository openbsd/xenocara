/*
 * Copyright (c) 2007-2008 NVIDIA, Corporation
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

#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#include <X11/Xatom.h>

#include "g80_type.h"
#include "g80_display.h"
#include "g80_output.h"

static void
G80SorSetPClk(xf86OutputPtr output, int pclk)
{
    G80Ptr pNv = G80PTR(output->scrn);
    G80OutputPrivPtr pPriv = output->driver_private;
    const int orOff = 0x800 * pPriv->or;
    const int limit = 165000;

    pNv->reg[(0x00614300+orOff)/4] = 0x70000 | (pclk > limit ? 0x101 : 0);
}

static void
G80SorDPMSSet(xf86OutputPtr output, int mode)
{
    G80Ptr pNv = G80PTR(output->scrn);
    G80OutputPrivPtr pPriv = output->driver_private;
    const int off = 0x800 * pPriv->or;
    CARD32 tmp;

    while(pNv->reg[(0x0061C004+off)/4] & 0x80000000);

    tmp = pNv->reg[(0x0061C004+off)/4];
    tmp |= 0x80000000;

    if(mode == DPMSModeOn)
        tmp |= 1;
    else
        tmp &= ~1;

    pNv->reg[(0x0061C004+off)/4] = tmp;
    while((pNv->reg[(0x61C030+off)/4] & 0x10000000));
}

static int
G80TMDSModeValid(xf86OutputPtr output, DisplayModePtr mode)
{
    G80Ptr pNv = G80PTR(output->scrn);

    // Disable dual-link modes unless enabled in the config file.
    if (mode->Clock > 165000 && !pNv->AllowDualLink)
        return MODE_CLOCK_HIGH;

    return G80OutputModeValid(output, mode);
}

static int
G80LVDSModeValid(xf86OutputPtr output, DisplayModePtr mode)
{
    G80OutputPrivPtr pPriv = output->driver_private;
    DisplayModePtr native = pPriv->nativeMode;

    // Ignore modes larger than the native res.
    if (mode->HDisplay > native->HDisplay || mode->VDisplay > native->VDisplay)
        return MODE_PANEL;

    return G80OutputModeValid(output, mode);
}

static void
G80SorModeSet(xf86OutputPtr output, DisplayModePtr mode,
              DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr pScrn = output->scrn;
    G80OutputPrivPtr pPriv = output->driver_private;
    const int sorOff = 0x40 * pPriv->or;
    CARD32 type;

    if(!adjusted_mode) {
        /* Disconnect the SOR */
        C(0x00000600 + sorOff, 0);
        return;
    }

    if(pPriv->panelType == LVDS)
        type = 0;
    else if(adjusted_mode->Clock > 165000)
        type = 0x500;
    else
        type = 0x100;

    // This wouldn't be necessary, but the server is stupid and calls
    // G80SorDPMSSet after the output is disconnected, even though the hardware
    // turns it off automatically.
    G80SorDPMSSet(output, DPMSModeOn);

    C(0x00000600 + sorOff,
        (G80CrtcGetHead(output->crtc) == HEAD0 ? 1 : 2) |
        type |
        ((adjusted_mode->Flags & V_NHSYNC) ? 0x1000 : 0) |
        ((adjusted_mode->Flags & V_NVSYNC) ? 0x2000 : 0));

    G80CrtcSetScale(output->crtc, adjusted_mode, pPriv->scale);
}

static xf86OutputStatus
G80SorDetect(xf86OutputPtr output)
{
    G80OutputPrivPtr pPriv = output->driver_private;

    /* Assume physical status isn't going to change before the BlockHandler */
    if(pPriv->cached_status != XF86OutputStatusUnknown)
        return pPriv->cached_status;

    G80OutputPartnersDetect(pPriv->partner, output, pPriv->i2c);
    return pPriv->cached_status;
}

static xf86OutputStatus
G80SorLVDSDetect(xf86OutputPtr output)
{
    G80OutputPrivPtr pPriv = output->driver_private;

    if(pPriv->i2c) {
        /* If LVDS has an I2C port, use the normal probe routine to get the
         * EDID, if possible. */
        G80SorDetect(output);
    }

    /* Ignore G80SorDetect and assume LVDS is always connected */
    return XF86OutputStatusConnected;
}

static void
G80SorDestroy(xf86OutputPtr output)
{
    G80OutputPrivPtr pPriv = output->driver_private;

    G80OutputDestroy(output);

    xf86DeleteMode(&pPriv->nativeMode, pPriv->nativeMode);

    free(output->driver_private);
    output->driver_private = NULL;
}

static void G80SorSetModeBackend(DisplayModePtr dst, const DisplayModePtr src)
{
    // Stash the backend mode timings from src into dst
    dst->Clock           = src->Clock;
    dst->Flags           = src->Flags;
    dst->CrtcHDisplay    = src->CrtcHDisplay;
    dst->CrtcHBlankStart = src->CrtcHBlankStart;
    dst->CrtcHSyncStart  = src->CrtcHSyncStart;
    dst->CrtcHSyncEnd    = src->CrtcHSyncEnd;
    dst->CrtcHBlankEnd   = src->CrtcHBlankEnd;
    dst->CrtcHTotal      = src->CrtcHTotal;
    dst->CrtcHSkew       = src->CrtcHSkew;
    dst->CrtcVDisplay    = src->CrtcVDisplay;
    dst->CrtcVBlankStart = src->CrtcVBlankStart;
    dst->CrtcVSyncStart  = src->CrtcVSyncStart;
    dst->CrtcVSyncEnd    = src->CrtcVSyncEnd;
    dst->CrtcVBlankEnd   = src->CrtcVBlankEnd;
    dst->CrtcVTotal      = src->CrtcVTotal;
    dst->CrtcHAdjusted   = src->CrtcHAdjusted;
    dst->CrtcVAdjusted   = src->CrtcVAdjusted;
}

static Bool
G80SorModeFixup(xf86OutputPtr output, DisplayModePtr mode,
                DisplayModePtr adjusted_mode)
{
    G80OutputPrivPtr pPriv = output->driver_private;
    DisplayModePtr native = pPriv->nativeMode;

    if(native && pPriv->scale != G80_SCALE_OFF) {
        G80SorSetModeBackend(adjusted_mode, native);
        // This mode is already "fixed"
        G80CrtcSkipModeFixup(output->crtc);
    }

    return TRUE;
}

static Bool
G80SorTMDSModeFixup(xf86OutputPtr output, DisplayModePtr mode,
                    DisplayModePtr adjusted_mode)
{
    int scrnIndex = output->scrn->scrnIndex;
    G80OutputPrivPtr pPriv = output->driver_private;
    DisplayModePtr modes = output->probed_modes;

    xf86DeleteMode(&pPriv->nativeMode, pPriv->nativeMode);

    if(modes) {
        // Find the preferred mode and use that as the "native" mode.
        // If no preferred mode is available, use the first one.
        DisplayModePtr mode;

        // Find the preferred mode.
        for(mode = modes; mode; mode = mode->next) {
            if(mode->type & M_T_PREFERRED) {
                xf86DrvMsgVerb(scrnIndex, X_INFO, 5,
                               "%s: preferred mode is %s\n",
                               output->name, mode->name);
                break;
            }
        }

        // XXX: May not want to allow scaling if no preferred mode is found.
        if(!mode) {
            mode = modes;
            xf86DrvMsgVerb(scrnIndex, X_INFO, 5,
                    "%s: no preferred mode found, using %s\n",
                    output->name, mode->name);
        }

        pPriv->nativeMode = xf86DuplicateMode(mode);
        G80CrtcDoModeFixup(pPriv->nativeMode, mode);
    }

    return G80SorModeFixup(output, mode, adjusted_mode);
}

static DisplayModePtr
G80SorGetLVDSModes(xf86OutputPtr output)
{
    G80OutputPrivPtr pPriv = output->driver_private;

    /* If an EDID was read during detection, use the modes from that. */
    DisplayModePtr modes = G80OutputGetDDCModes(output);
    if(modes)
        return modes;

    /* Otherwise, feed in the mode we read during initialization. */
    return xf86DuplicateMode(pPriv->nativeMode);
}

#define MAKE_ATOM(a) MakeAtom((a), sizeof(a) - 1, TRUE);

struct property {
    Atom atom;
    INT32 range[2];
};

static struct {
    struct property dither;
    struct property scale;
} properties;

static void
G80SorCreateResources(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    G80Ptr pNv = G80PTR(pScrn);
    int data, err;
    const char *s;

    /******** dithering ********/
    properties.dither.atom = MAKE_ATOM("dither");
    properties.dither.range[0] = 0;
    properties.dither.range[1] = 1;
    err = RRConfigureOutputProperty(output->randr_output,
                                    properties.dither.atom, FALSE, TRUE, FALSE,
                                    2, properties.dither.range);
    if(err)
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Failed to configure dithering property for %s: error %d\n",
                   output->name, err);

    // Set the default value
    data = pNv->Dither;
    err = RRChangeOutputProperty(output->randr_output, properties.dither.atom,
                                 XA_INTEGER, 32, PropModeReplace, 1, &data,
                                 FALSE, FALSE);
    if(err)
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Failed to set dithering property for %s: error %d\n",
                   output->name, err);

    /******** scaling ********/
    properties.scale.atom = MAKE_ATOM("scale");
    err = RRConfigureOutputProperty(output->randr_output,
                                    properties.scale.atom, FALSE, FALSE,
                                    FALSE, 0, NULL);
    if(err)
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Failed to configure scaling property for %s: error %d\n",
                   output->name, err);

    // Set the default value
    s = "aspect";
    err = RRChangeOutputProperty(output->randr_output, properties.scale.atom,
                                 XA_STRING, 8, PropModeReplace, strlen(s),
                                 (pointer)s, FALSE, FALSE);
    if(err)
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Failed to set scaling property for %s: error %d\n",
                   output->name, err);
}

static Bool
G80SorSetProperty(xf86OutputPtr output, Atom prop, RRPropertyValuePtr val)
{
    G80OutputPrivPtr pPriv = output->driver_private;

    if(prop == properties.dither.atom) {
        INT32 i;

        if(val->type != XA_INTEGER || val->format != 32 || val->size != 1)
            return FALSE;

        i = *(INT32*)val->data;
        if(i < properties.dither.range[0] || i > properties.dither.range[1])
            return FALSE;

        G80CrtcSetDither(output->crtc, i, TRUE);
    } else if(prop == properties.scale.atom) {
        const char *s;
        enum G80ScaleMode oldScale, scale;
        int i;
        const struct {
            const char *name;
            enum G80ScaleMode scale;
        } modes[] = {
            { "off",    G80_SCALE_OFF },
            { "aspect", G80_SCALE_ASPECT },
            { "fill",   G80_SCALE_FILL },
            { "center", G80_SCALE_CENTER },
            { NULL,     0 },
        };

        if(val->type != XA_STRING || val->format != 8)
            return FALSE;
        s = (char*)val->data;

        for(i = 0; modes[i].name; i++) {
            const char *name = modes[i].name;
            const int len = strlen(name);

            if(val->size == len && !strncmp(name, s, len)) {
                scale = modes[i].scale;
                break;
            }
        }
        if(!modes[i].name)
            return FALSE;
        if(scale == G80_SCALE_OFF && pPriv->panelType == LVDS)
            // LVDS requires scaling
            return FALSE;

        oldScale = pPriv->scale;
        pPriv->scale = scale;
        if(output->crtc) {
            xf86CrtcPtr crtc = output->crtc;

            if(!xf86CrtcSetMode(crtc, &crtc->desiredMode, crtc->desiredRotation,
                                crtc->desiredX, crtc->desiredY)) {
                xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR,
                           "Failed to set scaling to %s for output %s\n",
                           modes[i].name, output->name);

                // Restore old scale and try again.
                pPriv->scale = oldScale;
                if(!xf86CrtcSetMode(crtc, &crtc->desiredMode,
                                    crtc->desiredRotation, crtc->desiredX,
                                    crtc->desiredY)) {
                    xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR,
                               "Failed to restore old scaling for output %s\n",
                               output->name);
                }

                return FALSE;
            }
        }
    }

    return TRUE;
}

static const xf86OutputFuncsRec G80SorTMDSOutputFuncs = {
    .dpms = G80SorDPMSSet,
    .save = NULL,
    .restore = NULL,
    .mode_valid = G80TMDSModeValid,
    .mode_fixup = G80SorTMDSModeFixup,
    .prepare = G80OutputPrepare,
    .commit = G80OutputCommit,
    .mode_set = G80SorModeSet,
    .detect = G80SorDetect,
    .get_modes = G80OutputGetDDCModes,
    .create_resources = G80SorCreateResources,
    .set_property = G80SorSetProperty,
    .destroy = G80SorDestroy,
};

static const xf86OutputFuncsRec G80SorLVDSOutputFuncs = {
    .dpms = G80SorDPMSSet,
    .save = NULL,
    .restore = NULL,
    .mode_valid = G80LVDSModeValid,
    .mode_fixup = G80SorModeFixup,
    .prepare = G80OutputPrepare,
    .commit = G80OutputCommit,
    .mode_set = G80SorModeSet,
    .detect = G80SorLVDSDetect,
    .get_modes = G80SorGetLVDSModes,
    .create_resources = G80SorCreateResources,
    .set_property = G80SorSetProperty,
    .destroy = G80SorDestroy,
};

static DisplayModePtr
ReadLVDSNativeMode(G80Ptr pNv, const int off)
{
    DisplayModePtr mode = xnfcalloc(1, sizeof(DisplayModeRec));
    const CARD32 size = pNv->reg[(0x00610B4C+off)/4];
    const int width = size & 0x3fff;
    const int height = (size >> 16) & 0x3fff;

    mode->HDisplay = mode->CrtcHDisplay = width;
    mode->VDisplay = mode->CrtcVDisplay = height;
    mode->Clock           = pNv->reg[(0x610AD4+off)/4] & 0x3fffff;
    mode->CrtcHBlankStart = pNv->reg[(0x610AFC+off)/4];
    mode->CrtcHSyncEnd    = pNv->reg[(0x610B04+off)/4];
    mode->CrtcHBlankEnd   = pNv->reg[(0x610AE8+off)/4];
    mode->CrtcHTotal      = pNv->reg[(0x610AF4+off)/4];

    mode->next = mode->prev = NULL;
    mode->status = MODE_OK;
    mode->type = M_T_DRIVER | M_T_PREFERRED;

    xf86SetModeDefaultName(mode);

    return mode;
}

static DisplayModePtr
GetLVDSNativeMode(G80Ptr pNv)
{
    CARD32 val = pNv->reg[0x00610050/4];

    if((val & 3) == 2)
        return ReadLVDSNativeMode(pNv, 0);
    else if((val & 0x300) == 0x200)
        return ReadLVDSNativeMode(pNv, 0x540);

    return NULL;
}

xf86OutputPtr
G80CreateSor(ScrnInfoPtr pScrn, ORNum or, PanelType panelType)
{
    G80Ptr pNv = G80PTR(pScrn);
    G80OutputPrivPtr pPriv = xnfcalloc(sizeof(*pPriv), 1);
    const int off = 0x800 * or;
    xf86OutputPtr output;
    char orName[5];
    const xf86OutputFuncsRec *funcs;

    if(!pPriv)
        return NULL;

    if(panelType == LVDS) {
        strcpy(orName, "LVDS");
        funcs = &G80SorLVDSOutputFuncs;

        pPriv->nativeMode = GetLVDSNativeMode(pNv);

        if(!pPriv->nativeMode) {
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                       "Failed to find LVDS native mode\n");
            free(pPriv);
            return NULL;
        }

        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "%s native size %dx%d\n",
                   orName, pPriv->nativeMode->HDisplay,
                   pPriv->nativeMode->VDisplay);
    } else {
        snprintf(orName, 5, "DVI%d", or);
        pNv->reg[(0x61C00C+off)/4] = 0x03010700;
        pNv->reg[(0x61C010+off)/4] = 0x0000152f;
        pNv->reg[(0x61C014+off)/4] = 0x00000000;
        pNv->reg[(0x61C018+off)/4] = 0x00245af8;
        funcs = &G80SorTMDSOutputFuncs;
    }

    output = xf86OutputCreate(pScrn, funcs, orName);

    pPriv->type = SOR;
    pPriv->or = or;
    pPriv->panelType = panelType;
    pPriv->cached_status = XF86OutputStatusUnknown;
    if(panelType == TMDS)
        pPriv->set_pclk = G80SorSetPClk;
    output->driver_private = pPriv;
    output->interlaceAllowed = TRUE;
    output->doubleScanAllowed = TRUE;

    return output;
}
