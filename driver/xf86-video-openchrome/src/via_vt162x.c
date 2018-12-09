/*
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via_driver.h"
#include "via_vt162x.h"

static void
ViaSetTVClockSource(xf86CrtcPtr crtc)
{
	drmmode_crtc_private_ptr iga = crtc->driver_private;
	ScrnInfoPtr pScrn = crtc->scrn;
	VIAPtr pVia = VIAPTR(pScrn);
	VIADisplayPtr pVIADisplay = pVia->pVIADisplay;
	vgaHWPtr hwp = VGAHWPTR(pScrn);

	DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaSetTVClockSource\n"));

    switch(pVIADisplay->TVEncoder) {
        case VIA_VT1625:
            /* External TV: */
            switch(pVia->Chipset) {
                case VIA_CX700:
                case VIA_VX800:
                case VIA_VX855:
                case VIA_VX900:
                    /* IGA1 */
                    if (!iga->index) {
                        /* Fixing it to DVP1 for IGA1. */
                        ViaCrtcMask(hwp, 0x6C, 0xB0, 0xF0);
                    /* IGA2 */
                    } else {
                        /* Fixing it to DVP1 for IGA2. */
                        ViaCrtcMask(hwp, 0x6C, 0x0B, 0x0F);
                    }
                    break;
                default:
                    if (!iga->index)
                        ViaCrtcMask(hwp, 0x6C, 0x21, 0x21);
                    else
                        ViaCrtcMask(hwp, 0x6C, 0xA1, 0xA1);
                    break;
            }
            break;
        default:
			if (!iga->index)
				ViaCrtcMask(hwp, 0x6C, 0x50, 0xF0);
			else
				ViaCrtcMask(hwp, 0x6C, 0x05, 0x0F);
            break;
    }

}

static void
VT162xPrintRegs(ScrnInfoPtr pScrn)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;
    CARD8 i, buf;


    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Printing registers for %s\n",
               pVIADisplay->TVI2CDev->DevName);

    for (i = 0; i < pVIADisplay->TVNumRegs; i++) {
        xf86I2CReadByte(pVIADisplay->TVI2CDev, i, &buf);
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "TV%02X: 0x%02X\n", i, buf);
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "End of TV registers.\n");
}


I2CDevPtr
ViaVT162xDetect(ScrnInfoPtr pScrn, I2CBusPtr pBus, CARD8 Address)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;
    I2CDevPtr pDev = xf86CreateI2CDevRec();
    CARD8 buf;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaVT162xDetect\n"));

    pDev->DevName = "VT162x";
    pDev->SlaveAddr = Address;
    pDev->pI2CBus = pBus;

    if (!xf86I2CDevInit(pDev)) {
        xf86DestroyI2CDevRec(pDev, TRUE);
        return NULL;
    }

    if (!xf86I2CReadByte(pDev, 0x1B, &buf)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Unable to read from %s Slave %d.\n",
                   pBus->BusName, Address);
        xf86DestroyI2CDevRec(pDev, TRUE);
        return NULL;
    }

    switch (buf) {
        case 0x02:
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "Detected VIA Technologies VT1621 TV Encoder\n");
            pVIADisplay->TVEncoder = VIA_VT1621;
            pDev->DevName = "VT1621";
            break;
        case 0x03:
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "Detected VIA Technologies VT1622 TV Encoder\n");
            pVIADisplay->TVEncoder = VIA_VT1622;
            pDev->DevName = "VT1622";
            break;
        case 0x10:
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "Detected VIA Technologies VT1622A/VT1623 TV Encoder\n");
            pVIADisplay->TVEncoder = VIA_VT1623;
            pDev->DevName = "VT1623";
            break;
        case 0x50:
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "Detected VIA Technologies VT1625 TV Encoder\n");
            pVIADisplay->TVEncoder = VIA_VT1625;
            pDev->DevName = "VT1625";
            break;
        default:
            pVIADisplay->TVEncoder = VIA_NONETV;
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                       "Unknown TV Encoder found at %s %X.\n",
                       pBus->BusName, Address);
            xf86DestroyI2CDevRec(pDev, TRUE);
            pDev = NULL;
            break;
    }

    return pDev;
}


static void
VT162xSave(ScrnInfoPtr pScrn)
{
    int i;
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT162xSave\n"));

    for (i = 0; i < pVIADisplay->TVNumRegs; i++)
        xf86I2CReadByte(pVIADisplay->TVI2CDev, i, &(pVIADisplay->TVRegs[i]));

}

static void
VT162xRestore(ScrnInfoPtr pScrn)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;
    int i;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT162xRestore\n"));

    for (i = 0; i < pVIADisplay->TVNumRegs; i++)
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, i, pVIADisplay->TVRegs[i]);
}


/*
 * For VT1621 the same as for VT1622/VT1622A/VT1623, but result is different.
 * Still needs testing on VT1621, of course.
 */
static CARD8
VT162xDACSenseI2C(I2CDevPtr pDev)
{
    CARD8 save, sense;

    xf86I2CReadByte(pDev, 0x0E, &save);
    xf86I2CWriteByte(pDev, 0x0E, 0x00);
    xf86I2CWriteByte(pDev, 0x0E, 0x80);
    xf86I2CWriteByte(pDev, 0x0E, 0x00);
    xf86I2CReadByte(pDev, 0x0F, &sense);
    xf86I2CWriteByte(pDev, 0x0E, save);

    return (sense & 0x0F);
}

/*
 * VT1625/VT1625S sense connected TV outputs.
 *
 * The lower six bits of the return byte stand for each of the six DACs:
 *  - bit 0: DACf (Cb)
 *  - bit 1: DACe (Cr)
 *  - bit 2: DACd (Y)
 *  - bit 3: DACc (Composite)
 *  - bit 4: DACb (S-Video C)
 *  - bit 5: DACa (S-Video Y)
 *
 * If a bit is 0 it means a cable is connected. Note the VT1625S only has
 * four DACs, corresponding to bit 0-3 above.
 */
static CARD8
VT1625DACSenseI2C(I2CDevPtr pDev)
{
    CARD8 power, status, overflow, dacPresent;

    xf86I2CReadByte(pDev, 0x0E, &power);     // save power state

    // VT1625S will always report 0 for bits 4 and 5 of the status register as
    // it only has four DACs instead of six. This will result in a false
    // positive for the S-Video cable. It will also do this on the power
    // register, which is abused to check which DACs are actually present.
    xf86I2CWriteByte(pDev, 0x0E, 0xFF);
    xf86I2CReadByte(pDev, 0x0E, &dacPresent);

    xf86I2CWriteByte(pDev, 0x0E, 0x00);      // power on DACs/circuits
    xf86I2CReadByte(pDev, 0x1C, &overflow);  // save overflow reg
                                             // (DAC sense bit should be off)
    xf86I2CWriteByte(pDev, 0x1C, 0x80);      // enable DAC sense bit
    xf86I2CWriteByte(pDev, 0x1C, overflow);  // disable DAC sense bit
    xf86I2CReadByte(pDev, 0x0F, &status);    // read connection status
    xf86I2CWriteByte(pDev, 0x0E, power);     // restore power state
    status |= ~dacPresent;

    return (status & 0x3F);
}

/*
 * VT1621 only knows composite and s-video.
 */
static Bool
VT1621DACSense(ScrnInfoPtr pScrn)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;
    CARD8 sense;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1621DACSense\n"));

    sense = VT162xDACSenseI2C(pVIADisplay->TVI2CDev);
    switch (sense) {
        case 0x00:
            pVIADisplay->TVOutput = TVOUTPUT_SC;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT1621: S-Video & Composite connected.\n");
            return TRUE;
        case 0x01:
            pVIADisplay->TVOutput = TVOUTPUT_COMPOSITE;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT1621: Composite connected.\n");
            return TRUE;
        case 0x02:
            pVIADisplay->TVOutput = TVOUTPUT_SVIDEO;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT1621: S-Video connected.\n");
            return TRUE;
        case 0x03:
            pVIADisplay->TVOutput = TVOUTPUT_NONE;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT1621: Nothing connected.\n");
            return FALSE;
        default:
            pVIADisplay->TVOutput = TVOUTPUT_NONE;
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                       "VT1621: Unknown cable combination: 0x0%2X.\n", sense);
            return FALSE;
    }
}

/*
 * VT1622, VT1622A and VT1623 know composite, s-video, RGB and YCBCR.
 */
static Bool
VT1622DACSense(ScrnInfoPtr pScrn)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;
    CARD8 sense;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1622DACSense\n"));

    sense = VT162xDACSenseI2C(pVIADisplay->TVI2CDev);
    switch (sense) {
        case 0x00:  /* DAC A,B,C,D */
            pVIADisplay->TVOutput = TVOUTPUT_RGB;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT162x: RGB connected.\n");
            return TRUE;
        case 0x01:  /* DAC A,B,C */
            pVIADisplay->TVOutput = TVOUTPUT_SC;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT162x: S-Video & Composite connected.\n");
            return TRUE;
        case 0x07:  /* DAC A */
            pVIADisplay->TVOutput = TVOUTPUT_COMPOSITE;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT162x: Composite connected.\n");
            return TRUE;
        case 0x08:  /* DAC B,C,D */
            pVIADisplay->TVOutput = TVOUTPUT_YCBCR;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT162x: YcBcR connected.\n");
            return TRUE;
        case 0x09:  /* DAC B,C */
            pVIADisplay->TVOutput = TVOUTPUT_SVIDEO;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT162x: S-Video connected.\n");
            return TRUE;
        case 0x0F:
            pVIADisplay->TVOutput = TVOUTPUT_NONE;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT162x: Nothing connected.\n");
            return FALSE;
        default:
            pVIADisplay->TVOutput = TVOUTPUT_NONE;
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                       "VT162x: Unknown cable combination: 0x0%2X.\n", sense);
            return FALSE;
    }
}

/*
 * VT1625 knows composite, s-video, RGB and YCBCR.
 */
static Bool
VT1625DACSense(ScrnInfoPtr pScrn)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;
    CARD8 sense;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1625DACSense\n"));

    sense = VT1625DACSenseI2C(pVIADisplay->TVI2CDev);
    switch (sense) {
        case 0x00:  /* DAC A,B,C,D,E,F */
            pVIADisplay->TVOutput = TVOUTPUT_RGB;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT1625: RGB connected.\n");
            return TRUE;
        case 0x07:  /* DAC A,B,C */
            pVIADisplay->TVOutput = TVOUTPUT_SC;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT1625: S-Video & Composite connected.\n");
            return TRUE;
        case 0x37:  /* DAC C */
            pVIADisplay->TVOutput = TVOUTPUT_COMPOSITE;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT1625: Composite connected.\n");
            return TRUE;
        case 0x38:  /* DAC D,E,F */
            pVIADisplay->TVOutput = TVOUTPUT_YCBCR;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT1625: YCbCr connected.\n");
            return TRUE;
        case 0x0F:  /* DAC A,B */
            pVIADisplay->TVOutput = TVOUTPUT_SVIDEO;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT1625: S-Video connected.\n");
            return TRUE;
        case 0x3F:
            pVIADisplay->TVOutput = TVOUTPUT_NONE;
            xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                       "VT1625: Nothing connected.\n");
            return FALSE;
        default:
            pVIADisplay->TVOutput = TVOUTPUT_NONE;
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                       "VT1625: Unknown cable combination: 0x0%2X.\n", sense);
            return FALSE;
    }
}

static CARD8
VT1621ModeIndex(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;
    int i;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1621ModeIndex\n"));

    for (i = 0; VT1621Table[i].Width; i++) {
        if ((VT1621Table[i].Width == mode->CrtcHDisplay) &&
            (VT1621Table[i].Height == mode->CrtcVDisplay) &&
            (VT1621Table[i].Standard == pVIADisplay->TVType) &&
            !(strcmp(VT1621Table[i].name, mode->name)))
            return i;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "VT1621ModeIndex:"
               " Mode \"%s\" not found in Table\n", mode->name);
    return 0xFF;
}

static ModeStatus
VT1621ModeValid(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1621ModeValid\n"));

    if ((mode->PrivSize != sizeof(struct VT162xModePrivate)) ||
        ((mode->Private != (void *)&VT162xModePrivateNTSC) &&
         (mode->Private != (void *)&VT162xModePrivatePAL))) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "Not a mode defined by the TV Encoder.\n");
        return MODE_BAD;
    }

    if ((pVIADisplay->TVType == TVTYPE_NTSC) &&
        (mode->Private != (void *)&VT162xModePrivateNTSC)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "TV standard is NTSC. This is a PAL mode.\n");
        return MODE_BAD;
    } else if ((pVIADisplay->TVType == TVTYPE_PAL) &&
               (mode->Private != (void *)&VT162xModePrivatePAL)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "TV standard is PAL. This is a NTSC mode.\n");
        return MODE_BAD;
    }

    if (VT1621ModeIndex(pScrn, mode) != 0xFF)
        return MODE_OK;
    return MODE_BAD;
}

static CARD8
VT1622ModeIndex(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;
    struct VT162XTableRec *Table;
    int i;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1622ModeIndex\n"));

    if (pVIADisplay->TVEncoder == VIA_VT1622)
        Table = VT1622Table;
    else if (pVIADisplay->TVEncoder == VIA_VT1625)
        Table = VT1625Table;
    else
        Table = VT1623Table;

    for (i = 0; Table[i].Width; i++) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "width=%d:%d, height=%d:%d, std=%d:%d, name=%s:%s.\n",
                   Table[i].Width, mode->CrtcHDisplay,
                   Table[i].Height, mode->CrtcVDisplay,
                   Table[i].Standard, pVIADisplay->TVType,
                   Table[i].name, mode->name);

        if ((Table[i].Width == mode->CrtcHDisplay) &&
            (Table[i].Height == mode->CrtcVDisplay) &&
            (Table[i].Standard == pVIADisplay->TVType) &&
            !strcmp(Table[i].name, mode->name))
            return i;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "VT1622ModeIndex:"
               " Mode \"%s\" not found in Table\n", mode->name);
    return 0xFF;
}

static ModeStatus
VT1622ModeValid(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1622ModeValid\n"));

    if ((mode->PrivSize != sizeof(struct VT162xModePrivate)) ||
        ((mode->Private != (void *)&VT162xModePrivateNTSC) &&
         (mode->Private != (void *)&VT162xModePrivatePAL))) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "Not a mode defined by the TV Encoder.\n");
        return MODE_BAD;
    }

    if ((pVIADisplay->TVType == TVTYPE_NTSC) &&
        (mode->Private != (void *)&VT162xModePrivateNTSC)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "TV standard is NTSC. This is a PAL mode.\n");
        return MODE_BAD;
    } else if ((pVIADisplay->TVType == TVTYPE_PAL) &&
               (mode->Private != (void *)&VT162xModePrivatePAL)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "TV standard is PAL. This is a NTSC mode.\n");
        return MODE_BAD;
    }

    if (VT1622ModeIndex(pScrn, mode) != 0xFF)
        return MODE_OK;
    return MODE_BAD;
}

static ModeStatus
VT1625ModeValid(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1625ModeValid\n"));

    if ((mode->PrivSize != sizeof(struct VT162xModePrivate)) ||
        ((mode->Private != (void *)&VT162xModePrivateNTSC) &&
         (mode->Private != (void *)&VT162xModePrivatePAL) &&
         (mode->Private != (void *)&VT162xModePrivate480P) &&
         (mode->Private != (void *)&VT162xModePrivate576P) &&
         (mode->Private != (void *)&VT162xModePrivate720P) &&
         (mode->Private != (void *)&VT162xModePrivate1080I))) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "Not a mode defined by the TV Encoder.\n");
        return MODE_BAD;
    }

    if ((pVIADisplay->TVType == TVTYPE_NTSC) &&
        (mode->Private != (void *)&VT162xModePrivateNTSC)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "TV standard is NTSC. This is an incompatible mode.\n");
        return MODE_BAD;
    } else if ((pVIADisplay->TVType == TVTYPE_PAL) &&
               (mode->Private != (void *)&VT162xModePrivatePAL)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "TV standard is PAL. This is an incompatible mode.\n");
        return MODE_BAD;
    } else if ((pVIADisplay->TVType == TVTYPE_480P) &&
               (mode->Private != (void *)&VT162xModePrivate480P)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "TV standard is 480P. This is an incompatible mode.\n");
        return MODE_BAD;
    } else if ((pVIADisplay->TVType == TVTYPE_576P) &&
               (mode->Private != (void *)&VT162xModePrivate576P)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "TV standard is 576P. This is an incompatible mode.\n");
        return MODE_BAD;
    } else if ((pVIADisplay->TVType == TVTYPE_720P) &&
               (mode->Private != (void *)&VT162xModePrivate720P)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "TV standard is 720P. This is an incompatible mode.\n");
        return MODE_BAD;
    } else if ((pVIADisplay->TVType == TVTYPE_1080I) &&
               (mode->Private != (void *)&VT162xModePrivate1080I)) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "TV standard is 1080I. This is an incompatible mode.\n");
        return MODE_BAD;
    }

    if (VT1622ModeIndex(pScrn, mode) != 0xFF)
        return MODE_OK;
    return MODE_BAD;
}


static void
VT162xSetSubCarrier(I2CDevPtr pDev, CARD32 SubCarrier)
{
    xf86I2CWriteByte(pDev, 0x16, SubCarrier & 0xFF);
    xf86I2CWriteByte(pDev, 0x17, (SubCarrier >> 8) & 0xFF);
    xf86I2CWriteByte(pDev, 0x18, (SubCarrier >> 16) & 0xFF);
    xf86I2CWriteByte(pDev, 0x19, (SubCarrier >> 24) & 0xFF);
}

static void
VT1621ModeI2C(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;
    struct VT1621TableRec Table = VT1621Table[VT1621ModeIndex(pScrn, mode)];
    CARD8 i;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1621ModeI2C\n"));

    for (i = 0; i < 0x16; i++)
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, i, Table.TV[i]);

    VT162xSetSubCarrier(pVIADisplay->TVI2CDev, Table.SubCarrier);

    /* Skip reserved (1A) and version ID (1B). */
    xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x1C, Table.TV[0x1C]);

    /* Skip software reset (1D). */
    for (i = 0x1E; i < 0x24; i++)
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, i, Table.TV[i]);

    /* Write some zeroes? */
    xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x24, 0x00);
    for (i = 0; i < 0x08; i++)
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x4A + i, 0x00);

    if (pVIADisplay->TVOutput == TVOUTPUT_COMPOSITE)
        for (i = 0; i < 0x10; i++)
            xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x52 + i, Table.TVC[i]);
    else
        for (i = 0; i < 0x10; i++)
            xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x52 + i, Table.TVS[i]);

    /* Turn on all Composite and S-Video output. */
    xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x0E, 0x00);

    if (pVIADisplay->TVDotCrawl) {
        if (Table.DotCrawlSubCarrier) {
            xf86I2CReadByte(pVIADisplay->TVI2CDev, 0x11, &i);
            xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x11, i | 0x08);

            VT162xSetSubCarrier(pVIADisplay->TVI2CDev, Table.DotCrawlSubCarrier);
        } else
            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "This mode does not currently "
                       "support DotCrawl suppression.\n");
    }
}

static void
VT1621ModeCrtc(xf86CrtcPtr crtc, DisplayModePtr mode)
{
	ScrnInfoPtr pScrn = crtc->scrn;
	vgaHWPtr hwp = VGAHWPTR(pScrn);
	VIAPtr pVia = VIAPTR(pScrn);
	VIADisplayPtr pVIADisplay = pVia->pVIADisplay;
	struct VT1621TableRec Table = VT1621Table[VT1621ModeIndex(pScrn, mode)];

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1621ModeCrtc\n"));

    if (pVia->IsSecondary) {
        hwp->writeCrtc(hwp, 0x6A, 0x80);
        hwp->writeCrtc(hwp, 0x6B, 0x20);
        hwp->writeCrtc(hwp, 0x6C, 0x80);

        /* Disable LCD Scaling */
        if (!pVia->SAMM || pVia->FirstInit)
            hwp->writeCrtc(hwp, 0x79, 0x00);

    } else {
        hwp->writeCrtc(hwp, 0x6A, 0x00);
        hwp->writeCrtc(hwp, 0x6B, 0x80);
        hwp->writeCrtc(hwp, 0x6C, Table.PrimaryCR6C);
    }
    pVIADisplay->ClockExternal = TRUE;
    ViaCrtcMask(hwp, 0x6A, 0x40, 0x40);
    ViaCrtcMask(hwp, 0x6C, 0x01, 0x01);
}

/*
 * Also suited for VT1622A, VT1623, VT1625.
 */
static void
VT1622ModeI2C(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;
    struct VT162XTableRec Table;
    CARD8 save, i;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1622ModeI2C\n"));

    if (pVIADisplay->TVEncoder == VIA_VT1622)
        Table = VT1622Table[VT1622ModeIndex(pScrn, mode)];
    else if (pVIADisplay->TVEncoder == VIA_VT1625)
        Table = VT1625Table[VT1622ModeIndex(pScrn, mode)];
    else        /* VT1622A/VT1623 */
        Table = VT1623Table[VT1622ModeIndex(pScrn, mode)];

    /* TV reset. */
    xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x1D, 0x00);
    xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x1D, 0x80);

    for (i = 0; i < 0x16; i++)
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, i, Table.TV1[i]);

    VT162xSetSubCarrier(pVIADisplay->TVI2CDev, Table.SubCarrier);

    xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x1A, Table.TV1[0x1A]);

    /* Skip version ID. */
    xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x1C, Table.TV1[0x1C]);

    /* Skip software reset. */
    for (i = 0x1E; i < 0x30; i++)
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, i, Table.TV1[i]);

    for (i = 0; i < 0x1B; i++)
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x4A + i, Table.TV2[i]);

    /* Turn on all Composite and S-Video output. */
    xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x0E, 0x00);

    if (pVIADisplay->TVDotCrawl) {
        if (Table.DotCrawlSubCarrier) {
            xf86I2CReadByte(pVIADisplay->TVI2CDev, 0x11, &save);
            xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x11, save | 0x08);

            VT162xSetSubCarrier(pVIADisplay->TVI2CDev, Table.DotCrawlSubCarrier);
        } else
            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "This mode does not currently "
                       "support DotCrawl suppression.\n");
    }

    if (pVIADisplay->TVOutput == TVOUTPUT_RGB) {
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x02, 0x2A);
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x65, Table.RGB[0]);
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x66, Table.RGB[1]);
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x67, Table.RGB[2]);
        if (Table.RGB[3])
            xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x27, Table.RGB[3]);
        if (Table.RGB[4])
            xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x2B, Table.RGB[4]);
        if (Table.RGB[5])
            xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x2C, Table.RGB[5]);
        if (pVIADisplay->TVEncoder == VIA_VT1625) {
            if (pVIADisplay->TVType < TVTYPE_480P) {
                xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x02, 0x12);
                xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x23, 0x7E);
                xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x4A, 0x85);
                xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x4B, 0x0A);
                xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x4E, 0x00);
            } else {
                xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x02, 0x12);
                xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x4A, 0x85);
                xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x4B, 0x0A);
            }
        }
    } else if (pVIADisplay->TVOutput == TVOUTPUT_YCBCR) {
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x02, 0x03);
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x65, Table.YCbCr[0]);
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x66, Table.YCbCr[1]);
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x67, Table.YCbCr[2]);
        if (pVIADisplay->TVEncoder == VIA_VT1625) {
            if (pVIADisplay->TVType < TVTYPE_480P) {
                xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x23, 0x7E);
                xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x4E, 0x00);
            }
        }
    }

    /* Configure flicker filter. */
    xf86I2CReadByte(pVIADisplay->TVI2CDev, 0x03, &save);
    save &= 0xFC;
    if (pVIADisplay->TVDeflicker == 1)
        save |= 0x01;
    else if (pVIADisplay->TVDeflicker == 2)
        save |= 0x02;
    xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x03, save);
}

/*
 * Also suited for VT1622A, VT1623, VT1625.
 */
static void
VT1622ModeCrtc(xf86CrtcPtr crtc, DisplayModePtr mode)
{
	ScrnInfoPtr pScrn = crtc->scrn;
	vgaHWPtr hwp = VGAHWPTR(pScrn);
	VIAPtr pVia = VIAPTR(pScrn);
	VIADisplayPtr pVIADisplay = pVia->pVIADisplay;
	struct VT162XTableRec Table;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1622ModeCrtc\n"));

    if (pVIADisplay->TVEncoder == VIA_VT1622)
        Table = VT1622Table[VT1622ModeIndex(pScrn, mode)];
    else if (pVIADisplay->TVEncoder == VIA_VT1625)
        Table = VT1625Table[VT1622ModeIndex(pScrn, mode)];
    else        /* VT1622A/VT1623 */
        Table = VT1623Table[VT1622ModeIndex(pScrn, mode)];

    hwp->writeCrtc(hwp, 0x6A, 0x00);
    hwp->writeCrtc(hwp, 0x6B, 0x00);
    hwp->writeCrtc(hwp, 0x6C, 0x00);

    if (pVia->IsSecondary) {
        hwp->writeCrtc(hwp, 0x6C, Table.SecondaryCR6C);

        ViaCrtcMask(hwp, 0x6A, 0x80, 0x80);
        ViaCrtcMask(hwp, 0x6C, 0x80, 0x80);

        /* CLE266Ax use 2x XCLK. */
        if ((pVia->Chipset == VIA_CLE266) && CLE266_REV_IS_AX(pVia->ChipRev)) {
            ViaCrtcMask(hwp, 0x6B, 0x20, 0x20);

            /* Fix TV clock polarity for CLE266A2. */
            if (pVia->ChipRev == 0x02)
                ViaCrtcMask(hwp, 0x6C, 0x1C, 0x1C);
        }

        /* Disable LCD scaling. */
        if (!pVia->SAMM || pVia->FirstInit)
            hwp->writeCrtc(hwp, 0x79, 0x00);

    } else {
        if ((pVia->Chipset == VIA_CLE266) && CLE266_REV_IS_AX(pVia->ChipRev)) {
            ViaCrtcMask(hwp, 0x6B, 0x80, 0x80);

            /* Fix TV clock polarity for CLE266A2. */
            if (pVia->ChipRev == 0x02)
                hwp->writeCrtc(hwp, 0x6C, Table.PrimaryCR6C);
        }
    }
    pVIADisplay->ClockExternal = TRUE;
    ViaCrtcMask(hwp, 0x6A, 0x40, 0x40);
    ViaSetTVClockSource(crtc);
}


static void
VT1621Power(ScrnInfoPtr pScrn, Bool On)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1621Power\n"));

    if (On)
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x0E, 0x00);
    else
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x0E, 0x03);
}

static void
VT1622Power(ScrnInfoPtr pScrn, Bool On)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1622Power\n"));

    if (On)
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x0E, 0x00);
    else
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x0E, 0x0F);
}

static void
VT1625Power(ScrnInfoPtr pScrn, Bool On)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VT1625Power\n"));

    if (On)
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x0E, 0x00);
    else
        xf86I2CWriteByte(pVIADisplay->TVI2CDev, 0x0E, 0x3F);
}


void
ViaVT162xInit(ScrnInfoPtr pScrn)
{
    VIADisplayPtr pVIADisplay = VIAPTR(pScrn)->pVIADisplay;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaVT162xInit\n"));

    switch (pVIADisplay->TVEncoder) {
        case VIA_VT1621:
            pVIADisplay->TVSave = VT162xSave;
            pVIADisplay->TVRestore = VT162xRestore;
            pVIADisplay->TVDACSense = VT1621DACSense;
            pVIADisplay->TVModeValid = VT1621ModeValid;
            pVIADisplay->TVModeI2C = VT1621ModeI2C;
            pVIADisplay->TVModeCrtc = VT1621ModeCrtc;
            pVIADisplay->TVPower = VT1621Power;
            pVIADisplay->TVModes = VT1621Modes;
            pVIADisplay->TVNumModes = sizeof(VT1621Modes) / sizeof(DisplayModeRec);
            pVIADisplay->TVPrintRegs = VT162xPrintRegs;
            pVIADisplay->TVNumRegs = 0x68;
            break;
        case VIA_VT1622:
            pVIADisplay->TVSave = VT162xSave;
            pVIADisplay->TVRestore = VT162xRestore;
            pVIADisplay->TVDACSense = VT1622DACSense;
            pVIADisplay->TVModeValid = VT1622ModeValid;
            pVIADisplay->TVModeI2C = VT1622ModeI2C;
            pVIADisplay->TVModeCrtc = VT1622ModeCrtc;
            pVIADisplay->TVPower = VT1622Power;
            pVIADisplay->TVModes = VT1622Modes;
            pVIADisplay->TVNumModes = sizeof(VT1622Modes) / sizeof(DisplayModeRec);
            pVIADisplay->TVPrintRegs = VT162xPrintRegs;
            pVIADisplay->TVNumRegs = 0x68;
            break;
        case VIA_VT1623:
            pVIADisplay->TVSave = VT162xSave;
            pVIADisplay->TVRestore = VT162xRestore;
            pVIADisplay->TVDACSense = VT1622DACSense;
            pVIADisplay->TVModeValid = VT1622ModeValid;
            pVIADisplay->TVModeI2C = VT1622ModeI2C;
            pVIADisplay->TVModeCrtc = VT1622ModeCrtc;
            pVIADisplay->TVPower = VT1622Power;
            pVIADisplay->TVModes = VT1623Modes;
            pVIADisplay->TVNumModes = sizeof(VT1623Modes) / sizeof(DisplayModeRec);
            pVIADisplay->TVPrintRegs = VT162xPrintRegs;
            pVIADisplay->TVNumRegs = 0x6C;
            break;
        case VIA_VT1625:
            pVIADisplay->TVSave = VT162xSave;
            pVIADisplay->TVRestore = VT162xRestore;
            pVIADisplay->TVDACSense = VT1625DACSense;
            pVIADisplay->TVModeValid = VT1625ModeValid;
            pVIADisplay->TVModeI2C = VT1622ModeI2C;
            pVIADisplay->TVModeCrtc = VT1622ModeCrtc;
            pVIADisplay->TVPower = VT1625Power;
            pVIADisplay->TVModes = VT1625Modes;
            pVIADisplay->TVNumModes = sizeof(VT1625Modes) / sizeof(DisplayModeRec);
            pVIADisplay->TVPrintRegs = VT162xPrintRegs;
            pVIADisplay->TVNumRegs = 0x82;
            break;
        default:
            break;
    }
}
