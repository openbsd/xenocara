/*
 * Copyright 2005-2019 OpenChrome Project
 *                     [https://www.freedesktop.org/wiki/Openchrome]
 * Copyright 2004-2006 Luc Verhaegen.
 * Copyright 2004-2005 The Unichrome Project  [unichrome.sf.net]
 * Copyright 1998-2003 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2003 S3 Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sub license, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including
 * the next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via_driver.h"


typedef enum
{
#ifdef HAVE_DEBUG
    OPTION_PRINTTVREGS,
#endif
    OPTION_NOACCEL,
    OPTION_EXA_NOCOMPOSITE,
    OPTION_EXA_SCRATCH_SIZE,
    OPTION_SWCURSOR,
    OPTION_SHADOW_FB,
    OPTION_ROTATION_TYPE,
    OPTION_ROTATE,
    OPTION_CENTER,
    OPTION_TVDOTCRAWL,
    OPTION_TVTYPE,
    OPTION_TVOUTPUT,
    OPTION_DISABLEVQ,
    OPTION_DISABLEIRQ,
    OPTION_TVDEFLICKER,
    OPTION_AGP_DMA,
    OPTION_2D_DMA,
    OPTION_XV_DMA,
    OPTION_MAX_DRIMEM,
    OPTION_AGPMEM,
    OPTION_DISABLE_XV_BW_CHECK
} VIAOpts;

static OptionInfoRec VIAOptions[] = {
    {OPTION_NOACCEL,             "NoAccel",          OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_EXA_NOCOMPOSITE,     "ExaNoComposite",   OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_EXA_SCRATCH_SIZE,    "ExaScratchSize",   OPTV_INTEGER, {0}, FALSE},
    {OPTION_SWCURSOR,            "SWCursor",         OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_SHADOW_FB,           "ShadowFB",         OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_ROTATION_TYPE,       "RotationType",     OPTV_ANYSTR,  {0}, FALSE},
    {OPTION_ROTATE,              "Rotate",           OPTV_ANYSTR,  {0}, FALSE},
    {OPTION_TVDOTCRAWL,          "TVDotCrawl",       OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_TVDEFLICKER,         "TVDeflicker",      OPTV_INTEGER, {0}, FALSE},
    {OPTION_TVTYPE,              "TVType",           OPTV_ANYSTR,  {0}, FALSE},
    {OPTION_TVOUTPUT,            "TVOutput",         OPTV_ANYSTR,  {0}, FALSE},
    {OPTION_DISABLEVQ,           "DisableVQ",        OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_DISABLEIRQ,          "DisableIRQ",       OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_AGP_DMA,             "EnableAGPDMA",     OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_2D_DMA,              "NoAGPFor2D",       OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_XV_DMA,              "NoXVDMA",          OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_DISABLE_XV_BW_CHECK, "DisableXvBWCheck", OPTV_BOOLEAN, {0}, FALSE},
    {OPTION_MAX_DRIMEM,          "MaxDRIMem",        OPTV_INTEGER, {0}, FALSE},
    {OPTION_AGPMEM,              "AGPMem",           OPTV_INTEGER, {0}, FALSE},
    {-1,                         NULL,               OPTV_NONE,    {0}, FALSE}
};

const OptionInfoRec *
VIAAvailableOptions(int chipid, int busid)
{
    return VIAOptions;
}


void
viaSetupDefaultOptions(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    pVia->shadowFB = FALSE;
    pVia->NoAccel = FALSE;
    pVia->noComposite = FALSE;
    pVia->useEXA = TRUE;
    pVia->exaScratchSize = VIA_SCRATCH_SIZE / 1024;
    pVia->drmmode.hwcursor = TRUE;
    pVia->VQEnable = TRUE;
    pVia->DRIIrqEnable = TRUE;
    pVia->agpEnable = TRUE;
    pVia->dma2d = TRUE;
    pVia->dmaXV = TRUE;
#ifdef HAVE_DEBUG
    pVia->disableXvBWCheck = FALSE;
#endif
    pVia->maxDriSize = 0;
    pVia->agpMem = AGP_SIZE / 1024;
    pVia->VideoEngine = VIDEO_ENGINE_CLE;

    /*
     * Disable vertical interpolation because the size of
     * line buffer (limited to 800) is too small to do interpolation.
     */
    pVia->swov.maxWInterp = 800;
    pVia->swov.maxHInterp = 600;

    switch (pVia->Chipset) {
    case VIA_CLE266:
        break;
    case VIA_KM400:
        /*
         * IRQ is not broken on KM400A, but testing
         * (pVia->ChipRev < 0x80) is not enough to make sure we
         * have an older, broken KM400.
         */
        pVia->DRIIrqEnable = FALSE;
        break;
    case VIA_K8M800:
        pVia->DRIIrqEnable = FALSE;
        break;
    case VIA_PM800:
        pVia->VideoEngine = VIDEO_ENGINE_CME;
        break;
    case VIA_P4M800PRO:
        break;
    case VIA_CX700:
        pVia->VideoEngine = VIDEO_ENGINE_CME;
        pVia->swov.maxWInterp = 1920;
        pVia->swov.maxHInterp = 1080;
        break;
    case VIA_P4M890:
        pVia->VideoEngine = VIDEO_ENGINE_CME;
        pVia->dmaXV = FALSE;
        break;
    case VIA_K8M890:
        pVia->VideoEngine = VIDEO_ENGINE_CME;
        pVia->agpEnable = FALSE;
        pVia->dmaXV = FALSE;
        break;
    case VIA_P4M900:
        pVia->VideoEngine = VIDEO_ENGINE_CME;
        pVia->agpEnable = FALSE;
        /* FIXME: this needs to be tested */
        pVia->dmaXV = FALSE;
        break;
    case VIA_VX800:
    case VIA_VX855:
    case VIA_VX900:
        pVia->VideoEngine = VIDEO_ENGINE_CME;
        pVia->agpEnable = FALSE;
        pVia->dmaXV = FALSE;
        break;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

static void
viaProcessUMSOptions(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIADisplayPtr pVIADisplay = pVia->pVIADisplay;
    MessageType from = X_DEFAULT;
    const char *s = NULL;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

/*
    pVia->VQEnable = TRUE;
*/
    from = xf86GetOptValBool(VIAOptions,
                                OPTION_DISABLEVQ, &pVia->VQEnable) ?
            X_CONFIG : X_DEFAULT;
    if (from == X_CONFIG)
        pVia->VQEnable = !pVia->VQEnable;
    xf86DrvMsg(pScrn->scrnIndex, from,
                "GPU virtual command queue will be %s.\n",
                (pVia->VQEnable) ? "enabled" : "disabled");

/*
    pVia->DRIIrqEnable = TRUE;
*/
    from = xf86GetOptValBool(VIAOptions,
                                OPTION_DISABLEIRQ,
                                &pVia->DRIIrqEnable) ?
            X_CONFIG : X_DEFAULT;
    if (from == X_CONFIG)
        pVia->DRIIrqEnable = !pVia->DRIIrqEnable;
    xf86DrvMsg(pScrn->scrnIndex, from,
                "DRI IRQ will be %s if DRI is enabled.\n",
                (pVia->DRIIrqEnable) ? "enabled" : "disabled");

/*
    pVia->agpEnable = FALSE;
*/
    from = xf86GetOptValBool(VIAOptions,
                                OPTION_AGP_DMA, &pVia->agpEnable) ?
            X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from,
                "AGP DMA will be %s if DRI is enabled.\n",
                (pVia->agpEnable) ? "enabled" : "disabled");

/*
    pVia->dma2d = TRUE;
*/
    if (pVia->agpEnable) {
        from = xf86GetOptValBool(VIAOptions,
                                    OPTION_2D_DMA, &pVia->dma2d) ?
                X_CONFIG : X_DEFAULT;
        if (from == X_CONFIG)
            pVia->dma2d = !pVia->dma2d;
        xf86DrvMsg(pScrn->scrnIndex, from,
                    "AGP DMA will %sbe used for 2D acceleration.\n",
                    (pVia->dma2d) ? "" : "not ");
    }

/*
    pVia->dmaXV = TRUE;
*/
    from = xf86GetOptValBool(VIAOptions,
                                OPTION_XV_DMA, &pVia->dmaXV) ?
            X_CONFIG : X_DEFAULT;
    if (from == X_CONFIG)
        pVia->dmaXV = !pVia->dmaXV;
    xf86DrvMsg(pScrn->scrnIndex, from, "PCI DMA will %sbe used for XV "
               "image transfer if DRI is enabled.\n",
               (pVia->dmaXV) ? "" : "not ");

#ifdef HAVE_DEBUG
/*
    pVia->disableXvBWCheck = FALSE;
*/
    from = xf86GetOptValBool(VIAOptions,
                                OPTION_DISABLE_XV_BW_CHECK,
                                &pVia->disableXvBWCheck) ?
            X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from,
                "Xv Bandwidth check is %s.\n",
                pVia->disableXvBWCheck ? "disabled" : "enabled");
    if (pVia->disableXvBWCheck) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                    "You may get a \"snowy\" screen when using the Xv "
                    "overlay.\n");
    }
#endif

/*
    pVia->maxDriSize = 0;
*/
    from = xf86GetOptValInteger(VIAOptions,
                                OPTION_MAX_DRIMEM,
                                &pVia->maxDriSize) ?
            X_CONFIG : X_DEFAULT;
    if (pVia->maxDriSize > 0)
        xf86DrvMsg(pScrn->scrnIndex, from,
                    "Will impose a %d kB limit on video RAM reserved "
                    "for DRI.\n",
                    pVia->maxDriSize);
    else
        xf86DrvMsg(pScrn->scrnIndex, from,
                    "Will not impose a limit on video RAM reserved "
                    "for DRI.\n");

/*
    pVia->agpMem = AGP_SIZE / 1024;
*/
    from = xf86GetOptValInteger(VIAOptions,
                                OPTION_AGPMEM, &pVia->agpMem) ?
            X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from,
                "Will try to allocate %d KB of AGP memory.\n",
                pVia->agpMem);

    pVIADisplay->TVDotCrawl = FALSE;
    from = xf86GetOptValBool(VIAOptions,
                                OPTION_TVDOTCRAWL,
                                &pVIADisplay->TVDotCrawl) ?
            X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from,
                "TV dotCrawl is %s.\n",
                pVIADisplay->TVDotCrawl ? "enabled" : "disabled");

    /*
     * TV Deflicker
     */
    pVIADisplay->TVDeflicker = 0;
    from = xf86GetOptValInteger(VIAOptions, OPTION_TVDEFLICKER,
                                &pVIADisplay->TVDeflicker) ?
            X_CONFIG : X_DEFAULT;
    xf86DrvMsg(pScrn->scrnIndex, from,
                "TV deflicker is set to %d.\n",
                pVIADisplay->TVDeflicker);

    pVIADisplay->TVType = TVTYPE_NONE;
    if ((s = xf86GetOptValString(VIAOptions, OPTION_TVTYPE))) {
        if (!xf86NameCmp(s, "NTSC")) {
            pVIADisplay->TVType = TVTYPE_NTSC;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "TV Type is NTSC.\n");
        } else if (!xf86NameCmp(s, "PAL")) {
            pVIADisplay->TVType = TVTYPE_PAL;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "TV Type is PAL.\n");
        } else if (!xf86NameCmp(s, "480P")) {
            pVIADisplay->TVType = TVTYPE_480P;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "TV Type is SDTV 480P.\n");
        } else if (!xf86NameCmp(s, "576P")) {
            pVIADisplay->TVType = TVTYPE_576P;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "TV Type is SDTV 576P.\n");
        } else if (!xf86NameCmp(s, "720P")) {
            pVIADisplay->TVType = TVTYPE_720P;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "TV Type is HDTV 720P.\n");
        } else if (!xf86NameCmp(s, "1080I")) {
            pVIADisplay->TVType = TVTYPE_1080I;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "TV Type is HDTV 1080i.\n");
        }
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT,
                    "No default TV type is set.\n");
    }

    /*
     * TV output signal option
     */
    pVIADisplay->TVOutput = TVOUTPUT_NONE;
    if ((s = xf86GetOptValString(VIAOptions, OPTION_TVOUTPUT))) {
        if (!xf86NameCmp(s, "S-Video")) {
            pVIADisplay->TVOutput = TVOUTPUT_SVIDEO;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "TV Output Signal is S-Video.\n");
        } else if (!xf86NameCmp(s, "Composite")) {
            pVIADisplay->TVOutput = TVOUTPUT_COMPOSITE;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "TV Output Signal is Composite.\n");
        } else if (!xf86NameCmp(s, "SC")) {
            pVIADisplay->TVOutput = TVOUTPUT_SC;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "TV Output Signal is SC.\n");
        } else if (!xf86NameCmp(s, "RGB")) {
            pVIADisplay->TVOutput = TVOUTPUT_RGB;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "TV Output Signal is RGB.\n");
        } else if (!xf86NameCmp(s, "YCbCr")) {
            pVIADisplay->TVOutput = TVOUTPUT_YCBCR;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "TV Output Signal is YCbCr.\n");
        }
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT,
                    "No default TV output signal type is set.\n");
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

void
viaProcessOptions(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    MessageType from = X_DEFAULT;
    const char *s = NULL;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                "Processing DDX options . . .\n");

    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, VIAOptions);

    /*
     * When rotating, switch shadow frame buffer on and acceleration
     * off.
     */
    if ((s = xf86GetOptValString(VIAOptions, OPTION_ROTATION_TYPE))) {
        if (!xf86NameCmp(s, "SWRandR")) {
            pVia->shadowFB = TRUE;
            pVia->NoAccel = TRUE;
            pVia->RandRRotation = TRUE;
            pVia->rotate = RR_Rotate_0;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "Rotating screen RandR enabled, "
                        "acceleration disabled\n");
        } else if (!xf86NameCmp(s, "HWRandR")) {
            pVia->shadowFB = TRUE;
            pVia->NoAccel = TRUE;
            pVia->RandRRotation = TRUE;
            pVia->rotate = RR_Rotate_0;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "Hardware accelerated rotating screen is not "
                        "implemented. Using SW RandR.\n");
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "\"%s\" is not a valid"
                        "value for Option \"RotationType\".\n", s);
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Valid options are \"SWRandR\" and "
                        "\"HWRandR\".\n");
        }
    }

    /*
     * When rotating, switch shadow frame buffer on and acceleration
     * off.
     */
    if ((s = xf86GetOptValString(VIAOptions, OPTION_ROTATE))) {
        if (!xf86NameCmp(s, "CW")) {
            pVia->shadowFB = TRUE;
            pVia->NoAccel = TRUE;
            pVia->RandRRotation = TRUE;
            pVia->rotate = RR_Rotate_270;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "Rotating screen clockwise -- "
                        "acceleration is disabled.\n");
        } else if (!xf86NameCmp(s, "CCW")) {
            pVia->shadowFB = TRUE;
            pVia->NoAccel = TRUE;
            pVia->RandRRotation = TRUE;
            pVia->rotate = RR_Rotate_90;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "Rotating screen counterclockwise -- "
                        "acceleration is disabled.\n");
        } else if (!xf86NameCmp(s, "UD")) {
            pVia->shadowFB = TRUE;
            pVia->NoAccel = TRUE;
            pVia->RandRRotation = TRUE;
            pVia->rotate = RR_Rotate_180;
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "Rotating screen upside-down -- "
                        "acceleration is disabled.\n");
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                        "\"%s\" is not a valid value for "
                        "Option \"Rotate\".\n", s);
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Valid options are \"CW\", \"CCW\" "
                        "or \"UD\".\n");
        }
    }

    from = (xf86GetOptValBool(VIAOptions,
                                OPTION_SHADOW_FB, &pVia->shadowFB)
            ? X_CONFIG : X_DEFAULT);
    xf86DrvMsg(pScrn->scrnIndex, from,
                "Shadow framebuffer is %s.\n",
                pVia->shadowFB ? "enabled" : "disabled");

    /*
     * Use hardware acceleration, unless on shadow frame buffer.
     */
    from = (xf86GetOptValBool(VIAOptions,
                                OPTION_NOACCEL, &pVia->NoAccel) ?
            X_CONFIG : X_DEFAULT);
    if (!pVia->NoAccel && pVia->shadowFB) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                    "Acceleration is not supported when using shadow "
                    "frame buffer.\n");
        pVia->NoAccel = TRUE;
        from = X_DEFAULT;
    }

    /*
     * Disable EXA for KMS case
     */
    if (pVia->KMS)
        pVia->NoAccel = TRUE;

    xf86DrvMsg(pScrn->scrnIndex, from,
                "Hardware acceleration is %s.\n",
                !pVia->NoAccel ? "enabled" : "disabled");

    if (!pVia->NoAccel) {
        from = X_DEFAULT;
        xf86DrvMsg(pScrn->scrnIndex, from,
                    "Using EXA acceleration architecture.\n");
/*
        pVia->noComposite = FALSE;
*/
        if (pVia->useEXA) {
            from = xf86GetOptValBool(VIAOptions,
                                        OPTION_EXA_NOCOMPOSITE,
                                        &pVia->noComposite) ?
                    X_CONFIG : X_DEFAULT;
            xf86DrvMsg(pScrn->scrnIndex, from,
                        "EXA composite acceleration %s.\n",
                        !pVia->noComposite ? "enabled" : "disabled");

/*
            pVia->exaScratchSize = VIA_SCRATCH_SIZE / 1024;
*/
            from = xf86GetOptValInteger(VIAOptions,
                                            OPTION_EXA_SCRATCH_SIZE,
                                            &pVia->exaScratchSize) ?
                    X_CONFIG : X_DEFAULT;
            xf86DrvMsg(pScrn->scrnIndex, from,
                        "EXA scratch area size is %d KB.\n",
                        pVia->exaScratchSize);
        }
    }

    /*
     * Use a hardware cursor, unless on secondary or on shadow
     * frame buffer.
     */
    from = X_DEFAULT;
    if (pVia->IsSecondary || pVia->shadowFB)
        pVia->drmmode.hwcursor = FALSE;
    else if (xf86GetOptValBool(VIAOptions, OPTION_SWCURSOR,
                                &pVia->drmmode.hwcursor)) {
        pVia->drmmode.hwcursor = !pVia->drmmode.hwcursor;
        from = X_CONFIG;
    }

    if (pVia->drmmode.hwcursor)
        xf86DrvMsg(pScrn->scrnIndex, from,
                    "Using hardware two-color cursors and software "
                    "full-color cursors.\n");
    else
        xf86DrvMsg(pScrn->scrnIndex, from,
                    "Using software cursors.\n");

    if (!pVia->KMS) {
        viaProcessUMSOptions(pScrn);
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}
