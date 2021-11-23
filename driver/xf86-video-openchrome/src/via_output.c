/*
 * Copyright 2016 Kevin Brace
 * Copyright 2005-2016 The OpenChrome Project
 *                     [https://www.freedesktop.org/wiki/Openchrome]
 * Copyright 2004-2005 The Unichrome Project  [unichrome.sf.net]
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

/*
 * via_outputs.c
 *
 * Everything to do with setting and changing xf86Outputs.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via_driver.h"
#include <unistd.h>

void
viaIOPadState(ScrnInfoPtr pScrn, uint32_t diPort, uint8_t ioPadState)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    switch(diPort) {
    case VIA_DI_PORT_DIP0:
        viaDIP0SetIOPadState(pScrn, ioPadState);
        break;
    case VIA_DI_PORT_DIP1:
        viaDIP1SetIOPadState(pScrn, ioPadState);
        break;
    case VIA_DI_PORT_DVP0:
        viaDVP0SetIOPadState(pScrn, ioPadState);
        break;
    case VIA_DI_PORT_DVP1:
        viaDVP1SetIOPadState(pScrn, ioPadState);
        break;
    case VIA_DI_PORT_FPDPLOW:
        viaFPDPLowSetIOPadState(pScrn, ioPadState);
        break;
    case VIA_DI_PORT_FPDPHIGH:
        viaFPDPHighSetIOPadState(pScrn, ioPadState);
        break;
    case (VIA_DI_PORT_FPDPLOW |
          VIA_DI_PORT_FPDPHIGH):
        viaFPDPLowSetIOPadState(pScrn, ioPadState);
        viaFPDPHighSetIOPadState(pScrn, ioPadState);
        break;
    case VIA_DI_PORT_LVDS1:
        viaLVDS1SetIOPadSetting(pScrn, ioPadState);
        break;
    case VIA_DI_PORT_LVDS2:
        viaLVDS2SetIOPadSetting(pScrn, ioPadState);
        break;
    case (VIA_DI_PORT_LVDS1 |
          VIA_DI_PORT_LVDS2):
        viaLVDS1SetIOPadSetting(pScrn, ioPadState);
        viaLVDS2SetIOPadSetting(pScrn, ioPadState);
        break;
    default:
        break;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

void
viaOutputEnable(ScrnInfoPtr pScrn, uint32_t diPort, Bool outputEnable)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    switch(diPort) {
    case VIA_DI_PORT_DIP0:
        viaDIP0SetOutputEnable(pScrn, outputEnable);
        break;
    case VIA_DI_PORT_DIP1:
        viaDIP1SetOutputEnable(pScrn, outputEnable);
        break;
    default:
        break;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

void
viaClockSource(ScrnInfoPtr pScrn, uint32_t diPort, Bool clockSource)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    switch(diPort) {
    case VIA_DI_PORT_DIP0:
        viaDIP0SetClockSource(pScrn, clockSource);
        break;
    case VIA_DI_PORT_DIP1:
        viaDIP1SetClockSource(pScrn, clockSource);
        break;
    default:
        break;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

void
viaDisplaySource(ScrnInfoPtr pScrn, uint32_t diPort, int index)
{
    CARD8 displaySource = index & 0x01;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    switch(diPort) {
    case VIA_DI_PORT_DIP0:
        viaDIP0SetDisplaySource(pScrn, displaySource);
        break;
    case VIA_DI_PORT_DIP1:
        viaDIP1SetDisplaySource(pScrn, displaySource);
        break;
    case VIA_DI_PORT_DVP0:
        viaDVP0SetDisplaySource(pScrn, displaySource);
        break;
    case VIA_DI_PORT_DVP1:
        viaDVP1SetDisplaySource(pScrn, displaySource);
        break;
    case VIA_DI_PORT_FPDPLOW:
        viaFPDPLowSetDisplaySource(pScrn, displaySource);
        viaDVP1SetDisplaySource(pScrn, displaySource);
        break;
    case VIA_DI_PORT_FPDPHIGH:
        viaFPDPHighSetDisplaySource(pScrn, displaySource);
        viaDVP0SetDisplaySource(pScrn, displaySource);
        break;
    case (VIA_DI_PORT_FPDPLOW |
          VIA_DI_PORT_FPDPHIGH):
        viaFPDPLowSetDisplaySource(pScrn, displaySource);
        viaFPDPHighSetDisplaySource(pScrn, displaySource);
        break;
    case VIA_DI_PORT_LVDS1:
        viaLVDS1SetDisplaySource(pScrn, displaySource);
        break;
    case VIA_DI_PORT_LVDS2:
        viaLVDS2SetDisplaySource(pScrn, displaySource);
        break;
    case (VIA_DI_PORT_LVDS1 |
          VIA_DI_PORT_LVDS2):
        viaLVDS1SetDisplaySource(pScrn, displaySource);
        viaLVDS2SetDisplaySource(pScrn, displaySource);
        break;
    default:
        break;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

void
viaInitDisplay(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIADisplayPtr pVIADisplay = pVia->pVIADisplay;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaInitDisplay.\n"));

    /* Initialize the number of VGA connectors. */
    pVIADisplay->numberVGA = 0;

    /* Initialize the number of DVI connectors. */
    pVIADisplay->numberDVI = 0;

    /* Initialize the number of FP connectors. */
    pVIADisplay->numberFP = 0;

    viaExtTMDSProbe(pScrn);
    viaTMDSProbe(pScrn);

    viaFPProbe(pScrn);

    viaAnalogProbe(pScrn);


    /* TV */
    via_tv_init(pScrn);

    /* DVI */
    viaExtTMDSInit(pScrn);
    viaTMDSInit(pScrn);

    /* VGA */
    viaAnalogInit(pScrn);

    /* FP (Flat Panel) */
    viaFPInit(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaInitDisplay.\n"));
}

#ifdef HAVE_DEBUG
/*
 * Returns:
 *   Bit[7] 2nd Path
 *   Bit[6] 1/0 MHS Enable/Disable
 *   Bit[5] 0 = Bypass Callback, 1 = Enable Callback
 *   Bit[4] 0 = Hot-Key Sequence Control (OEM Specific)
 *   Bit[3] LCD
 *   Bit[2] TV
 *   Bit[1] CRT
 *   Bit[0] DVI
 */
static CARD8
VIAGetActiveDisplay(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD8 tmp;

    tmp = (hwp->readCrtc(hwp, 0x3E) >> 4);
    tmp |= ((hwp->readCrtc(hwp, 0x3B) & 0x18) << 3);

    return tmp;
}
#endif /* HAVE_DEBUG */

/*
 *
 */
CARD32
ViaGetMemoryBandwidth(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "ViaGetMemoryBandwidth. Memory type: %d\n",
                     pVia->MemClk));

    switch (pVia->MemClk) {
        case VIA_MEM_SDR66:
        case VIA_MEM_SDR100:
        case VIA_MEM_SDR133:
            return VIA_BW_MIN;
        case VIA_MEM_DDR200:
            return VIA_BW_DDR200;
        case VIA_MEM_DDR266:
        case VIA_MEM_DDR333:
        case VIA_MEM_DDR400:
            return VIA_BW_DDR400;
        case VIA_MEM_DDR533:
        case VIA_MEM_DDR667:
            return VIA_BW_DDR667;
        case VIA_MEM_DDR800:
        case VIA_MEM_DDR1066:
            return VIA_BW_DDR1066;
        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "ViaBandwidthAllowed: Unknown memory type: %d\n",
                       pVia->MemClk);
            return VIA_BW_MIN;
    }
}

/*
 *
 * Some very common abstractions.
 *
 */

/*
 * Standard vga call really.
 * Needs to be called to reset the dotclock (after SR40:2/1 reset)
 */
void
viaSetUseExternalClock(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD8 data;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    data = hwp->readMiscOut(hwp);
    hwp->writeMiscOut(hwp, data | 0x0C);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

/*
 *
 */
static void
ViaSetDotclock(ScrnInfoPtr pScrn, CARD32 clock, int base, int probase)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO,
                     "ViaSetDotclock to 0x%06x\n", (unsigned)clock));

    if ((pVia->Chipset == VIA_CLE266) || (pVia->Chipset == VIA_KM400)) {
        hwp->writeSeq(hwp, base, clock >> 8);
        hwp->writeSeq(hwp, base+1, clock & 0xFF);
    } else {  /* unichrome pro */
        union pllparams pll;
        int dtz, dr, dn, dm;
        pll.packed = clock;
        dtz = pll.params.dtz;
        dr  = pll.params.dr;
        dn  = pll.params.dn;
        dm  = pll.params.dm;

        /* The VX855 and VX900 do not modify dm/dn, but earlier chipsets do. */
        if ((pVia->Chipset != VIA_VX855) && (pVia->Chipset != VIA_VX900)) {
            dm -= 2;
            dn -= 2;
        }

        hwp->writeSeq(hwp, probase, dm & 0xff);
        hwp->writeSeq(hwp, probase+1,
                      ((dm >> 8) & 0x03) | (dr << 2) | ((dtz & 1) << 7));
        hwp->writeSeq(hwp, probase+2, (dn & 0x7f) | ((dtz & 2) << 6));
    }
}

/*
 *
 */
void
ViaSetPrimaryDotclock(ScrnInfoPtr pScrn, CARD32 clock)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    ViaSetDotclock(pScrn, clock, 0x46, 0x44);

    ViaSeqMask(hwp, 0x40, 0x02, 0x02);
    ViaSeqMask(hwp, 0x40, 0x00, 0x02);
}

/*
 *
 */
void
ViaSetSecondaryDotclock(ScrnInfoPtr pScrn, CARD32 clock)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    ViaSetDotclock(pScrn, clock, 0x44, 0x4A);

    ViaSeqMask(hwp, 0x40, 0x04, 0x04);
    ViaSeqMask(hwp, 0x40, 0x00, 0x04);
}

/*
 *
 */
static void
ViaSetECKDotclock(ScrnInfoPtr pScrn, CARD32 clock)
{
    /* Does the non-pro chip have an ECK clock ? */
    ViaSetDotclock(pScrn, clock, 0, 0x47);
}

static CARD32
ViaComputeDotClock(unsigned clock)
{
    double fout, fref, err, minErr;
    CARD32 dr, dn, dm, maxdm, maxdn;
    CARD32 factual, best;

    fref = 14.31818e6;
    fout = (double)clock * 1.e3;

    factual = ~0;
    maxdm = 127;
    maxdn = 7;
    minErr = 1e10;
    best = 0;

    for (dr = 0; dr < 4; ++dr) {
        for (dn = (dr == 0) ? 2 : 1; dn <= maxdn; ++dn) {
            for (dm = 1; dm <= maxdm; ++dm) {
                factual = fref * dm;
                factual /= (dn << dr);
                err = fabs((double)factual / fout - 1.);
                if (err < minErr) {
                    minErr = err;
                    best = (dm & 127) | ((dn & 31) << 8) | (dr << 14);
                }
            }
        }
    }
    return best;
}

static CARD32
ViaComputeProDotClock(unsigned clock)
{
    double fvco, fout, err, minErr;
    CARD32 dr = 0, dn, dm, maxdm, maxdn;
    CARD32 factual;
    union pllparams bestClock;

    fout = (double)clock * 1.e3;

    factual = ~0;
    maxdm = factual / 14318000U;
    minErr = 1.e10;
    bestClock.packed = 0U;

    do {
        fvco = fout * (1 << dr);
    } while (fvco < 300.e6 && dr++ < 8);

    if (dr == 8) {
        return 0;
    }

    if (clock < 30000)
        maxdn = 8;
    else if (clock < 45000)
        maxdn = 7;
    else if (clock < 170000)
        maxdn = 6;
    else
        maxdn = 5;

    for (dn = 2; dn < maxdn; ++dn) {
        for (dm = 2; dm < maxdm; ++dm) {
            factual = 14318000U * dm;
            factual /= dn << dr;
            if ((err = fabs((double)factual / fout - 1.)) < 0.005) {
                if (err < minErr) {
                    minErr = err;
                    bestClock.params.dtz = 1;
                    bestClock.params.dr = dr;
                    bestClock.params.dn = dn;
                    bestClock.params.dm = dm;
                }
            }
        }
    }

    return bestClock.packed;
}

CARD32
ViaModeDotClockTranslate(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIAPtr pVia = VIAPTR(pScrn);

    if ((pVia->Chipset == VIA_CLE266) || (pVia->Chipset == VIA_KM400)) {
        return ViaComputeDotClock(mode->Clock);
    } else {
        return ViaComputeProDotClock(mode->Clock);
    }
}
