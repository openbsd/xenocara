/*
 * Copyright 2016 Kevin Brace
 * Copyright 2015-2016 The OpenChrome Project
 *                     [https://www.freedesktop.org/wiki/Openchrome]
 * Copyright 2014 SHS SERVICES GmbH
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
 * via_tmds.c
 *
 * Handles initialization of TMDS (DVI) related resources and 
 * controls the integrated TMDS transmitter found in CX700 and 
 * later VIA Technologies chipsets.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include "via_driver.h"
#include "via_vt1632.h"
#include "via_sii164.h"


/*
 * Sets the polarity of horizontal synchronization and vertical
 * synchronization.
 */
static void
viaTMDSSyncPolarity(ScrnInfoPtr pScrn, unsigned int flags)
{
    CARD8 syncPolarity = 0x00;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaTMDSSyncPolarity.\n"));

    if (flags & V_NHSYNC) {
        syncPolarity |= BIT(0);
    }

    if (flags & V_NVSYNC) {
        syncPolarity |= BIT(1);
    }

    viaTMDSSetSyncPolarity(pScrn, syncPolarity);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                "TMDS (DVI) Horizontal Sync Polarity: %s\n",
                (syncPolarity & BIT(0)) ? "-" : "+");
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                "TMDS (DVI) Vertical Sync Polarity: %s\n",
                (syncPolarity & BIT(1)) ? "-" : "+");

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaTMDSSyncPolarity.\n"));
}

/*
 * Initializes most registers related to VIA Technologies IGP
 * integrated TMDS transmitter. Synchronization polarity and
 * display output source need to be set separately. */
static void
viaTMDSInitReg(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaTMDSInitReg.\n"));

    /* Turn off hardware controlled FP power on / off circuit. */
    viaFPSetPrimaryHardPower(pScrn, FALSE);

    /* Use software FP power sequence control. */
    viaFPSetPrimaryPowerSeqType(pScrn, FALSE);

    /* Turn off software controlled primary FP power rails. */
    viaFPSetPrimarySoftVDD(pScrn, FALSE);
    viaFPSetPrimarySoftVEE(pScrn, FALSE);

    /* Turn off software controlled primary FP back light
     * control. */
    viaFPSetPrimarySoftBackLight(pScrn, FALSE);

    /* Turn off direct control of FP back light. */
    viaFPSetPrimaryDirectBackLightCtrl(pScrn, FALSE);

    /* Activate DVI + LVDS2 mode. */
    /* 3X5.D2[5:4] - Display Channel Select
     *               00: LVDS1 + LVDS2
     *               01: DVI + LVDS2
     *               10: One Dual LVDS Channel (High Resolution Pannel)
     *               11: Single Channel DVI */
    ViaCrtcMask(hwp, 0xD2, 0x10, 0x30);

    /* Various DVI PLL settings should be set to default settings. */
    /* 3X5.D1[7]   - PLL2 Reference Clock Edge Select Bit
     *               0: PLLCK lock to rising edge of reference clock
     *               1: PLLCK lock to falling edge of reference clock
     * 3X5.D1[6:5] - PLL2 Charge Pump Current Set Bits
     *               00: ICH = 12.5 uA
     *               01: ICH = 25.0 uA
     *               10: ICH = 37.5 uA
     *               11: ICH = 50.0 uA
     * 3X5.D1[4:1] - Reserved
     * 3X5.D1[0]   - PLL2 Control Voltage Measurement Enable Bit */
    ViaCrtcMask(hwp, 0xD1, 0x00, 0xE1);

    /* Disable DVI test mode. */
    /* 3X5.D5[7] - PD1 Enable Selection
     *             1: Select by power flag
     *             0: By register
     * 3X5.D5[5] - DVI Testing Mode Enable
     * 3X5.D5[4] - DVI Testing Format Selection
     *             0: Half cycle
     *             1: LFSR mode */
    ViaCrtcMask(hwp, 0xD5, 0x00, 0xB0);

    /* Disable DVI sense interrupt. */
    /* 3C5.2B[7] - DVI Sense Interrupt Enable
     *             0: Disable
     *             1: Enable */
    ViaSeqMask(hwp, 0x2B, 0x00, 0x80);

    /* Clear DVI sense interrupt status. */
    /* 3C5.2B[6] - DVI Sense Interrupt Status
     *             (This bit has a RW1C attribute.) */
    ViaSeqMask(hwp, 0x2B, 0x40, 0x40);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaTMDSInitReg.\n"));
}

/*
 * Sets integrated TMDS (DVI) monitor power state.
 */
static void
viaTMDSPower(ScrnInfoPtr pScrn, Bool powerState)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaTMDSPower.\n"));

    if (powerState) {
        viaFPSetPrimaryDirectDisplayPeriod(pScrn, TRUE);

        /* Turn on software controlled primary FP data transmission. */
        viaFPSetPrimarySoftData(pScrn, TRUE);

        viaTMDSSetPower(pScrn, TRUE);
    } else {
        viaTMDSSetPower(pScrn, FALSE);

        /* Turn off software controlled primary FP data transmission. */
        viaFPSetPrimarySoftData(pScrn, TRUE);

        viaFPSetPrimaryDirectDisplayPeriod(pScrn, FALSE);
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                "DVI Power: %s\n",
                powerState ? "On" : "Off");

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaTMDSPower.\n"));
}

void
viaExtTMDSSetClockDriveStrength(ScrnInfoPtr pScrn, CARD8 clockDriveStrength)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    CARD8 sr12, sr13;
    CARD8 sr5a = 0x00;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaExtTMDSSetClockDriveStrength.\n"));

    if ((pVia->Chipset == VIA_CX700)
        || (pVia->Chipset == VIA_VX800)
        || (pVia->Chipset == VIA_VX855)
        || (pVia->Chipset == VIA_VX900)) {

        sr5a = hwp->readSeq(hwp, 0x5A);
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "SR5A: 0x%02X\n", sr5a));
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Setting 3C5.5A[0] to 0.\n"));
        ViaSeqMask(hwp, 0x5A, sr5a & 0xFE, 0x01);
    }

    sr12 = hwp->readSeq(hwp, 0x12);
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "SR12: 0x%02X\n", sr12));
    sr13 = hwp->readSeq(hwp, 0x13);
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "SR13: 0x%02X\n", sr13));
    switch (pVia->Chipset) {
    case VIA_CLE266:
        break;
    case VIA_KM400:
    case VIA_K8M800:
    case VIA_PM800:
    case VIA_P4M800PRO:
        /* 3C5.12[6] - DVP0D6 pin strapping
         *             0: Disable DVP0 (Digital Video Port 0)
         *             1: Enable DVP0 (Digital Video Port 0)
         * 3C5.12[5] - DVP0D5 pin strapping
         *             0: DVP0 is used by a TMDS transmitter (DVI)
         *             1: DVP0 is used by a TV encoder */
        if ((sr12 & 0x40) && (!(sr12 & 0x20))) {
            viaDVP0SetClockDriveStrength(pScrn, clockDriveStrength);
        }

        break;
    case VIA_P4M890:
    case VIA_K8M890:
    case VIA_P4M900:
        /* 3C5.12[6] - FPD6 pin strapping
         *             0: Disable DVP0 (Digital Video Port 0)
         *             1: Enable DVP0 (Digital Video Port 0)
         * 3C5.12[5] - FPD5 pin strapping
         *             0: DVP0 is used by a TMDS transmitter (DVI)
         *             1: DVP0 is used by a TV encoder */
        if ((sr12 & 0x40) && (!(sr12 & 0x20))) {
            viaDVP0SetClockDriveStrength(pScrn, clockDriveStrength);
        }

        break;
    case VIA_CX700:
    case VIA_VX800:
    case VIA_VX855:
    case VIA_VX900:
        /* 3C5.13[6] - DVP1 DVP / capture port selection
         *             0: DVP1 is used as a DVP (Digital Video Port)
         *             1: DVP1 is used as a capture port */
        if (!(sr13 & 0x40)) {
            viaDVP1SetClockDriveStrength(pScrn, clockDriveStrength);
        }

        break;
    default:
        break;
    }

    if ((pVia->Chipset == VIA_CX700)
        || (pVia->Chipset == VIA_VX800)
        || (pVia->Chipset == VIA_VX855)
        || (pVia->Chipset == VIA_VX900)) {

        hwp->writeSeq(hwp, 0x5A, sr5a);
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Restoring 3C5.5A[0].\n"));
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaExtTMDSSetClockDriveStrength.\n"));
}

void
viaExtTMDSSetDataDriveStrength(ScrnInfoPtr pScrn, CARD8 dataDriveStrength)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    CARD8 sr12, sr13;
    CARD8 sr5a = 0x00;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaExtTMDSSetDataDriveStrength.\n"));

    if ((pVia->Chipset == VIA_CX700)
        || (pVia->Chipset == VIA_VX800)
        || (pVia->Chipset == VIA_VX855)
        || (pVia->Chipset == VIA_VX900)) {

        sr5a = hwp->readSeq(hwp, 0x5A);
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "SR5A: 0x%02X\n", sr5a));
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Setting 3C5.5A[0] to 0.\n"));
        ViaSeqMask(hwp, 0x5A, sr5a & 0xFE, 0x01);
    }

    sr12 = hwp->readSeq(hwp, 0x12);
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "SR12: 0x%02X\n", sr12));
    sr13 = hwp->readSeq(hwp, 0x13);
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "SR13: 0x%02X\n", sr13));
    switch (pVia->Chipset) {
    case VIA_CLE266:
        break;
    case VIA_KM400:
    case VIA_K8M800:
    case VIA_PM800:
    case VIA_P4M800PRO:
        /* 3C5.12[6] - DVP0D6 pin strapping
         *             0: Disable DVP0 (Digital Video Port 0)
         *             1: Enable DVP0 (Digital Video Port 0)
         * 3C5.12[5] - DVP0D5 pin strapping
         *             0: DVP0 is used by a TMDS transmitter (DVI)
         *             1: DVP0 is used by a TV encoder */
        if ((sr12 & 0x40) && (!(sr12 & 0x20))) {
            viaDVP0SetDataDriveStrength(pScrn, dataDriveStrength);
        }

        break;
    case VIA_P4M890:
    case VIA_K8M890:
    case VIA_P4M900:
        /* 3C5.12[6] - FPD6 pin strapping
         *             0: Disable DVP0 (Digital Video Port 0)
         *             1: Enable DVP0 (Digital Video Port 0)
         * 3C5.12[5] - FPD5 pin strapping
         *             0: DVP0 is used by a TMDS transmitter (DVI)
         *             1: DVP0 is used by a TV encoder */
        if ((sr12 & 0x40) && (!(sr12 & 0x20))) {
            viaDVP0SetDataDriveStrength(pScrn, dataDriveStrength);
        }

        break;
    case VIA_CX700:
    case VIA_VX800:
    case VIA_VX855:
    case VIA_VX900:
        /* 3C5.13[6] - DVP1 DVP / capture port selection
         *             0: DVP1 is used as a DVP (Digital Video Port)
         *             1: DVP1 is used as a capture port */
        if (!(sr13 & 0x40)) {
            viaDVP1SetDataDriveStrength(pScrn, dataDriveStrength);
        }

        break;
    default:
        break;
    }

    if ((pVia->Chipset == VIA_CX700)
        || (pVia->Chipset == VIA_VX800)
        || (pVia->Chipset == VIA_VX855)
        || (pVia->Chipset == VIA_VX900)) {

        hwp->writeSeq(hwp, 0x5A, sr5a);
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Restoring 3C5.5A[0].\n"));
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaExtTMDSSetDataDriveStrength.\n"));
}

static void
via_tmds_create_resources(xf86OutputPtr output)
{
#ifdef HAVE_DEBUG
    ScrnInfoPtr pScrn = output->scrn;
#endif /* HAVE_DEBUG */

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_tmds_create_resources.\n"));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_tmds_create_resources.\n"));
}

static void
via_tmds_dpms(xf86OutputPtr output, int mode)
{
    ScrnInfoPtr pScrn = output->scrn;
    VIATMDSPtr pVIATMDS = (VIATMDSPtr) output->driver_private;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_tmds_dpms.\n"));

    switch (mode) {
    case DPMSModeOn:
        viaTMDSPower(pScrn, TRUE);
        viaIOPadState(pScrn, pVIATMDS->diPort, 0x03);
        break;
    case DPMSModeStandby:
    case DPMSModeSuspend:
    case DPMSModeOff:
        viaTMDSPower(pScrn, FALSE);
        viaIOPadState(pScrn, pVIATMDS->diPort, 0x00);
        break;
    default:
        break;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_tmds_dpms.\n"));
}

static void
via_tmds_save(xf86OutputPtr output)
{
#ifdef HAVE_DEBUG
    ScrnInfoPtr pScrn = output->scrn;
#endif /* HAVE_DEBUG */

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_tmds_save.\n"));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_tmds_save.\n"));
}

static void
via_tmds_restore(xf86OutputPtr output)
{
#ifdef HAVE_DEBUG
    ScrnInfoPtr pScrn = output->scrn;
#endif /* HAVE_DEBUG */

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_tmds_restore.\n"));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_tmds_restore.\n"));
}

static int
via_tmds_mode_valid(xf86OutputPtr output, DisplayModePtr pMode)
{
    ScrnInfoPtr pScrn = output->scrn;
    int status;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_tmds_mode_valid.\n"));

    if (!ViaModeDotClockTranslate(pScrn, pMode)) {
        status = MODE_NOCLOCK;
    } else {
        status = MODE_OK;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_tmds_mode_valid.\n"));
    return status;
}

static Bool
via_tmds_mode_fixup(xf86OutputPtr output, DisplayModePtr mode,
                      DisplayModePtr adjusted_mode)
{
#ifdef HAVE_DEBUG
    ScrnInfoPtr pScrn = output->scrn;
#endif /* HAVE_DEBUG */

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_tmds_mode_fixup.\n"));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_tmds_mode_fixup.\n"));
    return TRUE;
}

static void
via_tmds_prepare(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    VIATMDSPtr pVIATMDS = (VIATMDSPtr) output->driver_private;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_tmds_prepare.\n"));

    viaTMDSPower(pScrn, FALSE);
    viaIOPadState(pScrn, pVIATMDS->diPort, 0x00);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_tmds_prepare.\n"));
}

static void
via_tmds_commit(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    VIATMDSPtr pVIATMDS = (VIATMDSPtr) output->driver_private;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_tmds_commit.\n"));

    viaTMDSPower(pScrn, TRUE);
    viaIOPadState(pScrn, pVIATMDS->diPort, 0x03);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_tmds_commit.\n"));
}

static void
via_tmds_mode_set(xf86OutputPtr output, DisplayModePtr mode,
                    DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr pScrn = output->scrn;
    drmmode_crtc_private_ptr iga = output->crtc->driver_private;
    VIATMDSPtr pVIATMDS = (VIATMDSPtr) output->driver_private;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_tmds_mode_set.\n"));

    if (output->crtc) {
        /* Initialize VIA IGP integrated TMDS transmitter registers. */
        viaTMDSInitReg(pScrn);

        /* Set integrated TMDS transmitter sync polarity. */
        viaTMDSSyncPolarity(pScrn, adjusted_mode->Flags);

        viaDisplaySource(pScrn, pVIATMDS->diPort, iga->index);
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_tmds_mode_set.\n"));
}

static xf86OutputStatus
via_tmds_detect(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    xf86MonPtr pMon;
    xf86OutputStatus status = XF86OutputStatusDisconnected;
    I2CBusPtr pI2CBus;
    VIAPtr pVia = VIAPTR(pScrn);
    VIADisplayPtr pVIADisplay = pVia->pVIADisplay;
    VIATMDSPtr pVIATMDS = (VIATMDSPtr) output->driver_private;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_tmds_detect.\n"));

    if (pVIATMDS->i2cBus & VIA_I2C_BUS2) {
        pI2CBus = pVIADisplay->pI2CBus2;
    } else if (pVIATMDS->i2cBus & VIA_I2C_BUS3) {
        pI2CBus = pVIADisplay->pI2CBus3;
    } else {
        pI2CBus = NULL;
    }

    if (pI2CBus) {
        pMon = xf86OutputGetEDID(output, pI2CBus);
        if (pMon && DIGITAL(pMon->features.input_type)) {
            status = XF86OutputStatusConnected;
            xf86OutputSetEDID(output, pMon);
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                        "Detected a monitor connected to DVI.\n");
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                        "Could not obtain EDID from a monitor "
                        "connected to DVI.\n");
        }
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_tmds_detect.\n"));
    return status;
}

#ifdef RANDR_12_INTERFACE
static Bool
via_tmds_set_property(xf86OutputPtr output, Atom property,
                     RRPropertyValuePtr value)
{
#ifdef HAVE_DEBUG
    ScrnInfoPtr pScrn = output->scrn;
#endif /* HAVE_DEBUG */

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_tmds_set_property.\n"));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_tmds_set_property.\n"));
    return TRUE;
}
#endif

#ifdef RANDR_13_INTERFACE
static Bool
via_tmds_get_property(xf86OutputPtr output, Atom property)
{
#ifdef HAVE_DEBUG
    ScrnInfoPtr pScrn = output->scrn;
#endif /* HAVE_DEBUG */

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_tmds_get_property.\n"));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_tmds_get_property.\n"));
    return FALSE;
}
#endif

static void
via_tmds_destroy(xf86OutputPtr output)
{
#ifdef HAVE_DEBUG
    ScrnInfoPtr pScrn = output->scrn;
#endif /* HAVE_DEBUG */

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_tmds_destroy.\n"));

    if (output->driver_private) {
        free(output->driver_private);
    }

    output->driver_private = NULL;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_tmds_destroy.\n"));
}




static const xf86OutputFuncsRec via_tmds_funcs = {
    .create_resources   = via_tmds_create_resources,
    .dpms               = via_tmds_dpms,
    .save               = via_tmds_save,
    .restore            = via_tmds_restore,
    .mode_valid         = via_tmds_mode_valid,
    .mode_fixup         = via_tmds_mode_fixup,
    .prepare            = via_tmds_prepare,
    .commit             = via_tmds_commit,
    .mode_set           = via_tmds_mode_set,
    .detect             = via_tmds_detect,
    .get_modes          = xf86OutputGetEDIDModes,
#ifdef RANDR_12_INTERFACE
    .set_property       = via_tmds_set_property,
#endif
#ifdef RANDR_13_INTERFACE
    .get_property       = via_tmds_get_property,
#endif
    .destroy            = via_tmds_destroy,
};

void
viaTMDSProbe(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIADisplayPtr pVIADisplay = pVia->pVIADisplay;
    CARD8 sr13, sr5a;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaTMDSProbe.\n"));

    /* Detect the presence of integrated TMDS transmitter. */
    switch (pVia->Chipset) {
    case VIA_CX700:
    case VIA_VX800:
        sr5a = hwp->readSeq(hwp, 0x5A);

        /* Setting SR5A[0] to 1.
         * This allows the reading out the alternative
         * pin strapping information from SR12 and SR13. */
        ViaSeqMask(hwp, 0x5A, BIT(0), BIT(0));

        sr13 = hwp->readSeq(hwp, 0x13);
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "SR13: 0x%02X\n", sr13));

        /* 3C5.13[7:6] - Integrated LVDS / DVI Mode Select
         *               (DVP1D15-14 pin strapping)
         *               00: LVDS1 + LVDS2
         *               01: DVI + LVDS2
         *               10: Dual LVDS Channel (High Resolution Panel)
         *               11: One DVI only (decrease the clock jitter) */
        /* Check for DVI presence using pin strappings.
         * VIA Technologies NanoBook reference design based products
         * have their pin strappings set to a wrong setting to communicate
         * the presence of DVI, so it requires special handling here. */
        if (pVIADisplay->isVIANanoBook) {
                    pVIADisplay->intTMDSPresence = TRUE;
                    pVIADisplay->intTMDSDIPort = VIA_DI_PORT_TMDS;
                    pVIADisplay->intTMDSI2CBus = VIA_I2C_BUS2;
                    pVIADisplay->mappedI2CBus |= VIA_I2C_BUS2;
                    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                "Integrated TMDS (DVI) transmitter detected.\n");
        } else if (((!(sr13 & BIT(7))) && (sr13 & BIT(6)))
                    || ((sr13 & BIT(7)) && (sr13 & BIT(6)))) {
            pVIADisplay->intTMDSPresence = TRUE;
            pVIADisplay->intTMDSDIPort = VIA_DI_PORT_TMDS;
            pVIADisplay->intTMDSI2CBus = VIA_I2C_BUS2;
            pVIADisplay->mappedI2CBus |= VIA_I2C_BUS2;
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Integrated TMDS (DVI) transmitter detected via pin strapping.\n");
        } else {
            pVIADisplay->intTMDSPresence = FALSE;
            pVIADisplay->intTMDSDIPort = VIA_DI_PORT_NONE;
            pVIADisplay->intTMDSI2CBus = VIA_I2C_NONE;
        }

        hwp->writeSeq(hwp, 0x5A, sr5a);
        break;
    default:
        pVIADisplay->intTMDSPresence = FALSE;
        pVIADisplay->intTMDSDIPort = VIA_DI_PORT_NONE;
        pVIADisplay->intTMDSI2CBus = VIA_I2C_NONE;
        break;
    }

     DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaTMDSProbe.\n"));
}

/*
 * Probe (pre-initialization detection) of external TMDS transmitters.
 */
void
viaExtTMDSProbe(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIADisplayPtr pVIADisplay = pVia->pVIADisplay;
    CARD8 sr12;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaExtTMDSProbe.\n"));

    pVIADisplay->extTMDSPresence = FALSE;
    pVIADisplay->extTMDSI2CBus = VIA_I2C_NONE;
    pVIADisplay->extTMDSTransmitter = VIA_TMDS_NONE;

    if ((!(pVIADisplay->extTMDSPresence)) &&
        ((pVIADisplay->pI2CBus2) &&
        (~(pVIADisplay->mappedI2CBus & VIA_I2C_BUS2)))) {
        if (viaVT1632Probe(pScrn, pVIADisplay->pI2CBus2)) {
            pVIADisplay->extTMDSPresence = TRUE;
            pVIADisplay->extTMDSI2CBus = VIA_I2C_BUS2;
            pVIADisplay->extTMDSTransmitter = VIA_TMDS_VT1632;
            pVIADisplay->mappedI2CBus |= VIA_I2C_BUS2;
        } else if (viaSiI164Probe(pScrn, pVIADisplay->pI2CBus2)) {
            pVIADisplay->extTMDSPresence = TRUE;
            pVIADisplay->extTMDSI2CBus = VIA_I2C_BUS2;
            pVIADisplay->extTMDSTransmitter = VIA_TMDS_SII164;
            pVIADisplay->mappedI2CBus |= VIA_I2C_BUS2;
        }
    }

    if ((!(pVIADisplay->extTMDSPresence)) &&
        ((pVIADisplay->pI2CBus3) &&
            (~(pVIADisplay->mappedI2CBus & VIA_I2C_BUS3)))) {
        if (viaVT1632Probe(pScrn, pVIADisplay->pI2CBus3)) {
            pVIADisplay->extTMDSPresence = TRUE;
            pVIADisplay->extTMDSI2CBus = VIA_I2C_BUS3;
            pVIADisplay->extTMDSTransmitter = VIA_TMDS_VT1632;
            pVIADisplay->mappedI2CBus |= VIA_I2C_BUS3;
        } else if (viaSiI164Probe(pScrn, pVIADisplay->pI2CBus3)) {
            pVIADisplay->extTMDSPresence = TRUE;
            pVIADisplay->extTMDSI2CBus = VIA_I2C_BUS3;
            pVIADisplay->extTMDSTransmitter = VIA_TMDS_SII164;
            pVIADisplay->mappedI2CBus |= VIA_I2C_BUS3;
        }
    }

    sr12 = hwp->readSeq(hwp, 0x12);
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "SR12: 0x%02X\n", sr12));
    if (pVIADisplay->extTMDSPresence) {
        switch (pVia->Chipset) {
        case VIA_CLE266:
            /*
             *  3C5.12[5] - FPD18 pin strapping (DIP0)
             *             0: DVI
             *             1: TV
             */
            if (!(sr12 & BIT(5))) {
                pVIADisplay->extTMDSDIPort = VIA_DI_PORT_DIP0;
            /*
             * 3C5.12[4] - FPD17 pin strapping (DIP1)
             *             0: DVI / Capture
             *             1: Panel
             */
            } else if (!(sr12 & BIT(4))) {
                pVIADisplay->extTMDSDIPort = VIA_DI_PORT_DIP1;
            } else {
                pVIADisplay->extTMDSDIPort = VIA_DI_PORT_NONE;
            }

            break;
        case VIA_KM400:
        case VIA_P4M800PRO:
        case VIA_PM800:
        case VIA_K8M800:
            /* 3C5.12[6] - DVP0D6 pin strapping
             *             0: Disable DVP0 (Digital Video Port 0) for
             *                DVI or TV out use
             *             1: Enable DVP0 (Digital Video Port 0) for
             *                DVI or TV out use
             * 3C5.12[5] - DVP0D5 pin strapping
             *             0: TMDS transmitter (DVI)
             *             1: TV encoder */
            if ((sr12 & BIT(6)) && (!(sr12 & BIT(5)))) {
                pVIADisplay->extTMDSDIPort = VIA_DI_PORT_DVP0;
            } else {
                pVIADisplay->extTMDSDIPort = VIA_DI_PORT_DVP1;
            }

            break;
        case VIA_P4M890:
        case VIA_K8M890:
        case VIA_P4M900:
            /* Assume DVP2 as DVP0. Hence, VIA_DI_PORT_DVP0
             * is used. */
            /* 3C5.12[6] - DVP2D6 pin strapping
             *             0: Disable DVP2 (Digital Video Port 2)
             *             1: Enable DVP2 (Digital Video Port 2)
             * 3C5.12[5] - DVP2D5 pin strapping
             *             0: TMDS transmitter (DVI)
             *             1: TV encoder */
            if ((sr12 & BIT(6)) && (!(sr12 & BIT(5)))) {
                pVIADisplay->extTMDSDIPort = VIA_DI_PORT_DVP0;
            } else {
                pVIADisplay->extTMDSDIPort = VIA_DI_PORT_NONE;
            }

            break;
        case VIA_CX700:
        case VIA_VX800:
        case VIA_VX855:
        case VIA_VX900:
            pVIADisplay->extTMDSDIPort = VIA_DI_PORT_DVP1;
            break;
        default:
            pVIADisplay->extTMDSDIPort = VIA_DI_PORT_NONE;
            break;
        }
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaExtTMDSProbe.\n"));
}

void
viaTMDSInit(ScrnInfoPtr pScrn)
{
    xf86OutputPtr output;
    VIAPtr pVia = VIAPTR(pScrn);
    VIADisplayPtr pVIADisplay = pVia->pVIADisplay;
    VIATMDSPtr pVIATMDS;
    char outputNameBuffer[32];

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaTMDSInit.\n"));

    if (!pVIADisplay->intTMDSPresence) {
        goto exit;
    }

    pVIATMDS = (VIATMDSPtr) xnfcalloc(1, sizeof(VIATMDSRec));
    if (!pVIATMDS) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                            "Failed to allocate storage for "
                            "integrated TMDS (DVI) transmitter.\n"));
        goto exit;
    }

    /* The code to dynamically designate a particular DVI (i.e., DVI-1,
     * DVI-2, etc.) for xrandr was borrowed from xf86-video-r128 DDX. */
    sprintf(outputNameBuffer, "DVI-%d", (pVIADisplay->numberDVI + 1));
    output = xf86OutputCreate(pScrn, &via_tmds_funcs, outputNameBuffer);
    if (!output) {
        free(pVIATMDS);
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                            "Failed to create X Server display output "
                            "for integrated TMDS (DVI) "
                            "transmitter.\n"));
        goto exit;
    }

    /* Increment the number of DVI connectors. */
    pVIADisplay->numberDVI++;

    /* Leaving a hint for mode setting and DPMS to know which port
     * to access. For CX700 / VX700 and VX800 chipsets' integrated TMDS
     * transmitter, it is fixed to LVDS1 (TMDS uses LVDS1 pins). */
    pVIATMDS->diPort = pVIADisplay->intTMDSDIPort;

    /* Hint about which I2C bus to access for obtaining EDID. */
    pVIATMDS->i2cBus = pVIADisplay->intTMDSI2CBus;

    output->driver_private = pVIATMDS;

    /* Since there are two (2) display controllers registered with the
     * X.Org Server and both IGA1 and IGA2 can handle DVI without any
     * limitations, possible_crtcs should be set to 0x3 (0b11) so that
     * either display controller can get assigned to handle DVI. */
    output->possible_crtcs = BIT(1) | BIT(0);

    output->possible_clones = 0;
    output->interlaceAllowed = FALSE;
    output->doubleScanAllowed = FALSE;

exit:
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaTMDSInit.\n"));
}

void
viaExtTMDSInit(ScrnInfoPtr pScrn)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaExtTMDSInit.\n"));

    viaVT1632Init(pScrn);
    viaSiI164Init(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaExtTMDSInit.\n"));
}
