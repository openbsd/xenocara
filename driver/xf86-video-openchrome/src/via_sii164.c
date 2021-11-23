/*
 * Copyright 2016 Kevin Brace
 * Copyright 2016 The OpenChrome Project
 *                [https://www.freedesktop.org/wiki/Openchrome]
 * Copyright 2014 SHS SERVICES GmbH
 * Copyright 2006-2009 Luc Verhaegen.
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

#include "via_driver.h"
#include "via_ums.h"
#include "via_sii164.h"

static void
viaSiI164DumpRegisters(ScrnInfoPtr pScrn, I2CDevPtr pDev)
{
    int i;
    CARD8 tmp;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaSiI164DumpRegisters.\n"));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Dumping SiI 164 registers.\n"));
    for (i = 0; i <= 0x0f; i++) {
        xf86I2CReadByte(pDev, i, &tmp);
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "0x%02x: 0x%02x\n", i, tmp));
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaSiI164DumpRegisters.\n"));
}

static void
viaSiI164InitRegisters(ScrnInfoPtr pScrn, I2CDevPtr pDev)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaSiI164InitRegisters.\n"));

    xf86I2CWriteByte(pDev, 0x08,
                        VIA_SII164_VEN | VIA_SII164_HEN |
                        VIA_SII164_DSEL | VIA_SII164_EDGE | VIA_SII164_PDB);

    /* Route receiver detect bit (Offset 0x09[2]) as the output of
     * MSEN pin. */
    xf86I2CWriteByte(pDev, 0x09, 0x20);

    xf86I2CWriteByte(pDev, 0x0A, 0x90);

    xf86I2CWriteByte(pDev, 0x0C, 0x89);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaSiI164InitRegisters.\n"));
}

/*
 * Returns TMDS receiver detection state for Silicon Image SiI 164
 * external TMDS transmitter.
 */
static Bool
viaSiI164Sense(ScrnInfoPtr pScrn, I2CDevPtr pDev)
{
    CARD8 tmp;
    Bool receiverDetected = FALSE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaSiI164Sense.\n"));

    xf86I2CReadByte(pDev, 0x09, &tmp);
    if (tmp & 0x04) {
        receiverDetected = TRUE;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                "SiI 164 %s a TMDS receiver.\n",
                receiverDetected ? "detected" : "did not detect");

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaSiI164Sense.\n"));
    return receiverDetected;
}

static void
viaSiI164Power(ScrnInfoPtr pScrn, I2CDevPtr pDev, Bool powerState)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaSiI164Power.\n"));

    xf86I2CMaskByte(pDev, 0x08, powerState ? 0x01 : 0x00, 0x01);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "SiI 164 (DVI) Power: %s\n",
                powerState ? "On" : "Off");

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaSiI164Power.\n"));
}

static void
viaSiI164SaveRegisters(ScrnInfoPtr pScrn, I2CDevPtr pDev,
                        viaSiI164RecPtr pSiI164Rec)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaSiI164SaveRegisters.\n"));

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                "Saving SiI 164 registers.\n");
    xf86I2CReadByte(pDev, 0x08, &pSiI164Rec->Register08);
    xf86I2CReadByte(pDev, 0x09, &pSiI164Rec->Register09);
    xf86I2CReadByte(pDev, 0x0A, &pSiI164Rec->Register0A);
    xf86I2CReadByte(pDev, 0x0C, &pSiI164Rec->Register0C);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaSiI164SaveRegisters.\n"));
}

static void
viaSiI164RestoreRegisters(ScrnInfoPtr pScrn, I2CDevPtr pDev,
                            viaSiI164RecPtr pSiI164Rec)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaSiI164RestoreRegisters.\n"));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Restoring SiI 164 registers.\n"));
    xf86I2CWriteByte(pDev, 0x08, pSiI164Rec->Register08);
    xf86I2CWriteByte(pDev, 0x09, pSiI164Rec->Register09);
    xf86I2CWriteByte(pDev, 0x0A, pSiI164Rec->Register0A);
    xf86I2CWriteByte(pDev, 0x0C, pSiI164Rec->Register0C);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaSiI164RestoreRegisters.\n"));
}

static int
viaSiI164CheckModeValidity(xf86OutputPtr output, DisplayModePtr pMode)
{
    viaSiI164RecPtr pSiI164Rec = output->driver_private;
    int status = MODE_OK;

    if (pMode->Clock < pSiI164Rec->DotclockMin) {
        status = MODE_CLOCK_LOW;
        goto exit;
    }

    if (pMode->Clock > pSiI164Rec->DotclockMax) {
        status = MODE_CLOCK_HIGH;
    }

exit:
    return status;
}

static void
via_sii164_create_resources(xf86OutputPtr output)
{
}

static void
via_sii164_dpms(xf86OutputPtr output, int mode)
{
    ScrnInfoPtr pScrn = output->scrn;
    viaSiI164RecPtr pSiI164Rec = output->driver_private;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_sii164_dpms.\n"));

    switch (mode) {
    case DPMSModeOn:
        viaSiI164Power(pScrn, pSiI164Rec->pSiI164I2CDev, TRUE);
        viaIOPadState(pScrn, pSiI164Rec->diPort, 0x03);
        break;
    case DPMSModeStandby:
    case DPMSModeSuspend:
    case DPMSModeOff:
        viaSiI164Power(pScrn, pSiI164Rec->pSiI164I2CDev, FALSE);
        viaIOPadState(pScrn, pSiI164Rec->diPort, 0x00);
        break;
    default:
        break;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_sii164_dpms.\n"));
}

static void
via_sii164_save(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    viaSiI164RecPtr pSiI164Rec = output->driver_private;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_sii164_save.\n"));

    viaSiI164SaveRegisters(pScrn, pSiI164Rec->pSiI164I2CDev, pSiI164Rec);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_sii164_save.\n"));
}

static void
via_sii164_restore(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    viaSiI164RecPtr pSiI164Rec = output->driver_private;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_sii164_restore.\n"));

    viaSiI164RestoreRegisters(pScrn, pSiI164Rec->pSiI164I2CDev,
                                pSiI164Rec);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_sii164_restore.\n"));
}

static int
via_sii164_mode_valid(xf86OutputPtr output, DisplayModePtr pMode)
{
    return viaSiI164CheckModeValidity(output, pMode);
}

static Bool
via_sii164_mode_fixup(xf86OutputPtr output, DisplayModePtr mode,
                   DisplayModePtr adjusted_mode)
{
    return TRUE;
}

static void
via_sii164_prepare(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    VIAPtr pVia = VIAPTR(pScrn);
    viaSiI164RecPtr pSiI164Rec = output->driver_private;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    viaSiI164Power(pScrn, pSiI164Rec->pSiI164I2CDev, FALSE);
    viaIOPadState(pScrn, pSiI164Rec->diPort, 0x00);

    if (pVia->Chipset == VIA_CLE266) {
        viaOutputEnable(pScrn, pSiI164Rec->diPort, FALSE);
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

static void
via_sii164_commit(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    VIAPtr pVia = VIAPTR(pScrn);
    viaSiI164RecPtr pSiI164Rec = output->driver_private;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    viaSiI164Power(pScrn, pSiI164Rec->pSiI164I2CDev, TRUE);
    viaIOPadState(pScrn, pSiI164Rec->diPort, 0x03);

    if (pVia->Chipset == VIA_CLE266) {
        viaOutputEnable(pScrn, pSiI164Rec->diPort, TRUE);
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

static void
via_sii164_mode_set(xf86OutputPtr output, DisplayModePtr mode,
                    DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr pScrn = output->scrn;
    drmmode_crtc_private_ptr iga = output->crtc->driver_private;
    VIAPtr pVia = VIAPTR(pScrn);
    viaSiI164RecPtr pSiI164Rec = output->driver_private;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_sii164_mode_set.\n"));

    if (output->crtc) {
        viaExtTMDSSetClockDriveStrength(pScrn, 0x03);
        viaExtTMDSSetDataDriveStrength(pScrn, 0x03);
        viaIOPadState(pScrn, pSiI164Rec->diPort, 0x03);

        if (pVia->Chipset == VIA_CLE266) {
            viaClockSource(pScrn, pSiI164Rec->diPort, TRUE);
        }

        viaSiI164DumpRegisters(pScrn, pSiI164Rec->pSiI164I2CDev);
        viaSiI164InitRegisters(pScrn, pSiI164Rec->pSiI164I2CDev);
        viaSiI164DumpRegisters(pScrn, pSiI164Rec->pSiI164I2CDev);

        viaDisplaySource(pScrn, pSiI164Rec->diPort, iga->index);
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                "Exiting via_sii164_mode_set.\n"));
}

static xf86OutputStatus
via_sii164_detect(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    xf86OutputStatus status = XF86OutputStatusDisconnected;
    viaSiI164RecPtr pSiI164Rec = (viaSiI164RecPtr) output->driver_private;
    Bool connectorDetected;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered via_sii_164_detect.\n"));

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                "Probing for a DVI connector . . .\n");

    connectorDetected = viaSiI164Sense(pScrn, pSiI164Rec->pSiI164I2CDev);
    if (!connectorDetected) {
        xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                    "DVI connector not detected.\n");
        goto exit;
    }

    status = XF86OutputStatusConnected;
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                "DVI connector detected.\n");

exit:
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting via_sii_164_detect.\n"));
    return status;
}

static DisplayModePtr
via_sii164_get_modes(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    xf86MonPtr pMon;
    DisplayModePtr pDisplay_Mode = NULL;
    I2CBusPtr pI2CBus;
    VIAPtr pVia = VIAPTR(pScrn);
    VIADisplayPtr pVIADisplay = pVia->pVIADisplay;
    viaSiI164RecPtr pSiI164Rec = (viaSiI164RecPtr) output->driver_private;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    if (pSiI164Rec->i2cBus & VIA_I2C_BUS2) {
        pI2CBus = pVIADisplay->pI2CBus2;
    } else if (pSiI164Rec->i2cBus & VIA_I2C_BUS3) {
        pI2CBus = pVIADisplay->pI2CBus3;
    } else {
        pI2CBus = NULL;
    }

    if (pI2CBus) {
        pMon = xf86OutputGetEDID(output, pI2CBus);

        /* Is the interface type digital? */
        if (pMon && DIGITAL(pMon->features.input_type)) {
            xf86OutputSetEDID(output, pMon);
            pDisplay_Mode = xf86OutputGetEDIDModes(output);
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                        "Could not obtain EDID from a monitor "
                        "connected to DVI.\n");
        }
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
    return pDisplay_Mode;
}

#ifdef RANDR_12_INTERFACE
static Bool
via_sii164_set_property(xf86OutputPtr output, Atom property,
                     RRPropertyValuePtr value)
{
    return TRUE;
}
#endif

#ifdef RANDR_13_INTERFACE
static Bool
via_sii164_get_property(xf86OutputPtr output, Atom property)
{
    return FALSE;
}
#endif

static void
via_sii164_destroy(xf86OutputPtr output)
{
}

const xf86OutputFuncsRec via_sii164_funcs = {
    .create_resources   = via_sii164_create_resources,
    .dpms               = via_sii164_dpms,
    .save               = via_sii164_save,
    .restore            = via_sii164_restore,
    .mode_valid         = via_sii164_mode_valid,
    .mode_fixup         = via_sii164_mode_fixup,
    .prepare            = via_sii164_prepare,
    .commit             = via_sii164_commit,
    .mode_set           = via_sii164_mode_set,
    .detect             = via_sii164_detect,
    .get_modes          = via_sii164_get_modes,
#ifdef RANDR_12_INTERFACE
    .set_property       = via_sii164_set_property,
#endif
#ifdef RANDR_13_INTERFACE
    .get_property       = via_sii164_get_property,
#endif
    .destroy            = via_sii164_destroy,
};

Bool
viaSiI164Probe(ScrnInfoPtr pScrn, I2CBusPtr pI2CBus)
{
    I2CDevPtr pI2CDevice = NULL;
    I2CSlaveAddr i2cAddr = 0x70;
    CARD8 i2cData;
    CARD16 vendorID, deviceID;
    Bool status = FALSE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaSiI164Probe.\n"));

    if (!pI2CBus) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                            "Invalid I2C bus.\n"));
        goto exit;
    }

    if (!xf86I2CProbeAddress(pI2CBus, i2cAddr)) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                            "I2C bus device not found.\n"));
        goto exit;
    }

    pI2CDevice = xf86CreateI2CDevRec();
    if (!pI2CDevice) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                            "Failed to create an I2C bus device "
                            "record.\n"));
        goto exit;
    }

    pI2CDevice->DevName = "SiI 164";
    pI2CDevice->SlaveAddr = i2cAddr;
    pI2CDevice->pI2CBus = pI2CBus;
    if (!xf86I2CDevInit(pI2CDevice)) {
        xf86DestroyI2CDevRec(pI2CDevice, TRUE);
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                            "Failed to initialize a device on "
                            "I2C bus.\n"));
        goto exit;
    }

    xf86I2CReadByte(pI2CDevice, 0, &i2cData);
    vendorID = i2cData;
    xf86I2CReadByte(pI2CDevice, 1, &i2cData);
    vendorID |= i2cData << 8;
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Vendor ID: 0x%04x\n", vendorID));

    xf86I2CReadByte(pI2CDevice, 2, &i2cData);
    deviceID = i2cData;
    xf86I2CReadByte(pI2CDevice, 3, &i2cData);
    deviceID |= i2cData << 8;
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Device ID: 0x%04x\n", deviceID));

    if ((vendorID != 0x0001) || (deviceID != 0x0006)) {
        xf86DestroyI2CDevRec(pI2CDevice, TRUE);
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                            "SiI 164 external TMDS transmitter not "
                            "detected.\n"));
        goto exit;
    }

    status = TRUE;
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                "SiI 164 external TMDS transmitter detected.\n");
    xf86DestroyI2CDevRec(pI2CDevice, TRUE);
exit:
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaSiI164Probe.\n"));
    return status;
}

void
viaSiI164Init(ScrnInfoPtr pScrn)
{
    xf86OutputPtr output;
    VIAPtr pVia = VIAPTR(pScrn);
    VIADisplayPtr pVIADisplay = pVia->pVIADisplay;
    viaSiI164RecPtr pVIASiI164;
    I2CBusPtr pI2CBus;
    I2CDevPtr pI2CDevice;
    I2CSlaveAddr i2cAddr = 0x70;
    CARD8 i2cData;
    char outputNameBuffer[32];

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaSiI164Init.\n"));

    if ((!(pVIADisplay->extTMDSPresence)) ||
        (pVIADisplay->extTMDSTransmitter != VIA_TMDS_SII164)) {
        goto exit;
    }

    if (pVIADisplay->extTMDSI2CBus & VIA_I2C_BUS2) {
        pI2CBus = pVIADisplay->pI2CBus2;
    } else if (pVIADisplay->extTMDSI2CBus & VIA_I2C_BUS3) {
        pI2CBus = pVIADisplay->pI2CBus3;
    } else {
        goto exit;
    }

    if (!xf86I2CProbeAddress(pI2CBus, i2cAddr)) {
        xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                    "I2C device not found.\n");
        goto exit;
    }

    pI2CDevice = xf86CreateI2CDevRec();
    if (!pI2CDevice) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Failed to create an I2C bus device record.\n");
        goto exit;
    }

    pI2CDevice->DevName = "SiI 164";
    pI2CDevice->SlaveAddr = i2cAddr;
    pI2CDevice->pI2CBus = pI2CBus;
    if (!xf86I2CDevInit(pI2CDevice)) {
        xf86DestroyI2CDevRec(pI2CDevice, TRUE);
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Failed to initialize a device on I2C bus.\n");
        goto exit;
    }

    pVIASiI164 = (viaSiI164RecPtr) xnfcalloc(1, sizeof(viaSiI164Rec));
    if (!pVIASiI164) {
        xf86DestroyI2CDevRec(pI2CDevice, TRUE);
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                            "Failed to allocate storage for "
                            "SiI 164.\n"));
        goto exit;
    }

    /* The code to dynamically designate a particular DVI (i.e., DVI-1,
     * DVI-2, etc.) for xrandr was borrowed from xf86-video-r128 DDX. */
    sprintf(outputNameBuffer, "DVI-%d", (pVIADisplay->numberDVI + 1));
    output = xf86OutputCreate(pScrn, &via_sii164_funcs, outputNameBuffer);
    if (!output) {
        free(pVIASiI164);
        xf86DestroyI2CDevRec(pI2CDevice, TRUE);
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Failed to create X Server display output "
                    "for SiI 164.\n");
        goto exit;
    }

    /* Increment the number of DVI connectors. */
    pVIADisplay->numberDVI++;

    // Remembering which I2C bus is used for SiI 164.
    pVIASiI164->pSiI164I2CDev = pI2CDevice;

    pVIASiI164->diPort = pVIADisplay->extTMDSDIPort;

    /* Hint about which I2C bus to access for obtaining EDID. */
    pVIASiI164->i2cBus = pVIADisplay->extTMDSI2CBus;

    pVIASiI164->transmitter = pVIADisplay->extTMDSTransmitter;

    xf86I2CReadByte(pI2CDevice, 0x06, &i2cData);
    pVIASiI164->DotclockMin = i2cData * 1000;

    xf86I2CReadByte(pI2CDevice, 0x07, &i2cData);
    pVIASiI164->DotclockMax = (i2cData + 65) * 1000;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Supported SiI 164 Dot Clock Range: "
                "%d to %d MHz\n",
                pVIASiI164->DotclockMin / 1000,
                pVIASiI164->DotclockMax / 1000);

    output->driver_private = pVIASiI164;

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
                        "Exiting viaSiI164Init.\n"));
}
