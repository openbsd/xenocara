/*
 * Copyright 2005-2007 The Openchrome Project [openchrome.org]
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

#include "via.h"
#include "via_driver.h"
#include "via_vgahw.h"
#include "via_id.h"

#include "via_mode.h"

#include <xorg/xf86Crtc.h>

static void
ViaCRTCSetGraphicsRegisters(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    /* graphics registers */
    hwp->writeGr(hwp, 0x00, 0x00);
    hwp->writeGr(hwp, 0x01, 0x00);
    hwp->writeGr(hwp, 0x02, 0x00);
    hwp->writeGr(hwp, 0x03, 0x00);
    hwp->writeGr(hwp, 0x04, 0x00);
    hwp->writeGr(hwp, 0x05, 0x40);
    hwp->writeGr(hwp, 0x06, 0x05);
    hwp->writeGr(hwp, 0x07, 0x0F);
    hwp->writeGr(hwp, 0x08, 0xFF);

    ViaGrMask(hwp, 0x20, 0, 0xFF);
    ViaGrMask(hwp, 0x21, 0, 0xFF);
    ViaGrMask(hwp, 0x22, 0, 0xFF);
}

static void
ViaCRTCSetAttributeRegisters(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD8 i;

    /* attribute registers */
    for (i = 0; i <= 0xF; i++) {
        hwp->writeAttr(hwp, i, i);
    }
    hwp->writeAttr(hwp, 0x10, 0x41);
    hwp->writeAttr(hwp, 0x11, 0xFF);
    hwp->writeAttr(hwp, 0x12, 0x0F);
    hwp->writeAttr(hwp, 0x13, 0x00);
    hwp->writeAttr(hwp, 0x14, 0x00);
}

static Bool
via_xf86crtc_resize (ScrnInfoPtr scrn, int width, int height)
{
    scrn->virtualX = width;
    scrn->virtualY = height;
    return TRUE;
}

static const
xf86CrtcConfigFuncsRec via_xf86crtc_config_funcs = {
    via_xf86crtc_resize
};

void 
ViaPreInitCRTCConfig(ScrnInfoPtr pScrn)
{
     xf86CrtcConfigInit (pScrn, &via_xf86crtc_config_funcs);
}


void
ViaCRTCInit(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    hwp->writeSeq(hwp, 0x10, 0x01); /* unlock extended registers */
    ViaCrtcMask(hwp, 0x47, 0x00, 0x01); /* unlock CRT registers */
    ViaCRTCSetGraphicsRegisters(pScrn);
    ViaCRTCSetAttributeRegisters(pScrn);
}

void
ViaFirstCRTCSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    CARD16 temp;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaFirstCRTCSetMode\n"));

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Setting up %s\n", mode->name));

    ViaCrtcMask(hwp, 0x11, 0x00, 0x80); /* modify starting address */
    ViaCrtcMask(hwp, 0x03, 0x80, 0x80); /* enable vertical retrace access */

    /* Set Misc Register */
    temp = 0x23;
    if (mode->Flags & V_NHSYNC)
        temp |= 0x40;
    if (mode->Flags & V_NVSYNC)
        temp |= 0x80;
    temp |= 0x0C; /* Undefined/external clock */
    hwp->writeMiscOut(hwp, temp);

    /* Sequence registers */
    hwp->writeSeq(hwp, 0x00, 0x00);

#if 0
    if (mode->Flags & V_CLKDIV2)
        hwp->writeSeq(hwp, 0x01, 0x09);
    else
#endif
        hwp->writeSeq(hwp, 0x01, 0x01);

    hwp->writeSeq(hwp, 0x02, 0x0F);
    hwp->writeSeq(hwp, 0x03, 0x00);
    hwp->writeSeq(hwp, 0x04, 0x0E);

    ViaSeqMask(hwp, 0x15, 0x02, 0x02);

    /* bpp */
    switch (pScrn->bitsPerPixel) {
        case 8:
            /* Only CLE266.AX use 6bits LUT. */
            if (pVia->Chipset == VIA_CLE266 && pVia->ChipRev < 15)
                ViaSeqMask(hwp, 0x15, 0x22, 0xFE);
            else
                ViaSeqMask(hwp, 0x15, 0xA2, 0xFE);

            break;
        case 16:
            ViaSeqMask(hwp, 0x15, 0xB6, 0xFE);
            break;
        case 24:
        case 32:
            ViaSeqMask(hwp, 0x15, 0xAE, 0xFE);
            break;
        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unhandled bitdepth: %d\n",
                       pScrn->bitsPerPixel);
            break;
    }

    switch (pVia->ChipId) {
        case VIA_K8M890:
        case VIA_CX700:
        case VIA_P4M900:
	case VIA_VX800:
	case VIA_VX855:
            break;
        default:
            ViaSeqMask(hwp, 0x16, 0x08, 0xBF);
            ViaSeqMask(hwp, 0x17, 0x1F, 0xFF);
            ViaSeqMask(hwp, 0x18, 0x4E, 0xFF);
            ViaSeqMask(hwp, 0x1A, 0x08, 0xFD);
            break;
    }

    /* Crtc registers */
    /* horizontal total : 4100 */
    temp = (mode->CrtcHTotal >> 3) - 5;
    hwp->writeCrtc(hwp, 0x00, temp & 0xFF);
    ViaCrtcMask(hwp, 0x36, temp >> 5, 0x08);

    /* horizontal address : 2048 */
    temp = (mode->CrtcHDisplay >> 3) - 1;
    hwp->writeCrtc(hwp, 0x01, temp & 0xFF);

    /* horizontal blanking start : 2048 */
    /* temp = (mode->CrtcHDisplay >> 3) - 1; */
    temp = (mode->CrtcHBlankStart >> 3) - 1;
    hwp->writeCrtc(hwp, 0x02, temp & 0xFF);
    /* If HblankStart has more bits anywhere, add them here */

    /* horizontal blanking end : start + 1025 */
    /* temp = (mode->CrtcHTotal >> 3) - 1; */
    temp = (mode->CrtcHBlankEnd >> 3) - 1;
    ViaCrtcMask(hwp, 0x03, temp, 0x1F);
    ViaCrtcMask(hwp, 0x05, temp << 2, 0x80);
    ViaCrtcMask(hwp, 0x33, temp >> 1, 0x20);

    /* CrtcHSkew ??? */

    /* horizontal sync start : 4095 */
    temp = mode->CrtcHSyncStart >> 3;
    hwp->writeCrtc(hwp, 0x04, temp & 0xFF);
    ViaCrtcMask(hwp, 0x33, temp >> 4, 0x10);

    /* horizontal sync end : start + 256 */
    temp = mode->CrtcHSyncEnd >> 3;
    ViaCrtcMask(hwp, 0x05, temp, 0x1F);

    /* vertical total : 2049 */
    temp = mode->CrtcVTotal - 2;
    hwp->writeCrtc(hwp, 0x06, temp & 0xFF);
    ViaCrtcMask(hwp, 0x07, temp >> 8, 0x01);
    ViaCrtcMask(hwp, 0x07, temp >> 4, 0x20);
    ViaCrtcMask(hwp, 0x35, temp >> 10, 0x01);

    /* vertical address : 2048 */
    temp = mode->CrtcVDisplay - 1;
    hwp->writeCrtc(hwp, 0x12, temp & 0xFF);
    ViaCrtcMask(hwp, 0x07, temp >> 7, 0x02);
    ViaCrtcMask(hwp, 0x07, temp >> 3, 0x40);
    ViaCrtcMask(hwp, 0x35, temp >> 8, 0x04);

    /* Primary starting address -> 0x00, adjustframe does the rest */
    hwp->writeCrtc(hwp, 0x0C, 0x00);
    hwp->writeCrtc(hwp, 0x0D, 0x00);
    hwp->writeCrtc(hwp, 0x34, 0x00);
    ViaCrtcMask(hwp, 0x48, 0x00, 0x03); /* is this even possible on CLE266A ? */

    /* vertical sync start : 2047 */
    temp = mode->CrtcVSyncStart;
    hwp->writeCrtc(hwp, 0x10, temp & 0xFF);
    ViaCrtcMask(hwp, 0x07, temp >> 6, 0x04);
    ViaCrtcMask(hwp, 0x07, temp >> 2, 0x80);
    ViaCrtcMask(hwp, 0x35, temp >> 9, 0x02);

    /* vertical sync end : start + 16 -- other bits someplace? */
    ViaCrtcMask(hwp, 0x11, mode->CrtcVSyncEnd, 0x0F);

    /* line compare: We are not doing splitscreen so 0x3FFF */
    hwp->writeCrtc(hwp, 0x18, 0xFF);
    ViaCrtcMask(hwp, 0x07, 0x10, 0x10);
    ViaCrtcMask(hwp, 0x09, 0x40, 0x40);
    ViaCrtcMask(hwp, 0x33, 0x07, 0x06);
    ViaCrtcMask(hwp, 0x35, 0x10, 0x10);

    /* zero Maximum scan line */
    ViaCrtcMask(hwp, 0x09, 0x00, 0x1F);
    hwp->writeCrtc(hwp, 0x14, 0x00);

    /* vertical blanking start : 2048 */
    /* temp = mode->CrtcVDisplay - 1; */
    temp = mode->CrtcVBlankStart - 1;
    hwp->writeCrtc(hwp, 0x15, temp & 0xFF);
    ViaCrtcMask(hwp, 0x07, temp >> 5, 0x08);
    ViaCrtcMask(hwp, 0x09, temp >> 4, 0x20);
    ViaCrtcMask(hwp, 0x35, temp >> 7, 0x08);

    /* vertical blanking end : start + 257 */
    /* temp = mode->CrtcVTotal - 1; */
    temp = mode->CrtcVBlankEnd - 1;
    hwp->writeCrtc(hwp, 0x16, temp);

    /* FIXME: check if this is really necessary here */
    switch (pVia->ChipId) {
        case VIA_K8M890:
        case VIA_CX700:
        case VIA_P4M900:
	case VIA_VX800:
	case VIA_VX855:
            break;
        default:
            /* some leftovers */
            hwp->writeCrtc(hwp, 0x08, 0x00);
            ViaCrtcMask(hwp, 0x32, 0, 0xFF);  /* ? */
            ViaCrtcMask(hwp, 0x33, 0, 0xC8);
            break;
    }

    /* offset */
    temp = (pScrn->displayWidth * (pScrn->bitsPerPixel >> 3)) >> 3;
    /* Make sure that this is 32-byte aligned. */
    if (temp & 0x03) {
        temp += 0x03;
        temp &= ~0x03;
    }
    hwp->writeCrtc(hwp, 0x13, temp & 0xFF);
    ViaCrtcMask(hwp, 0x35, temp >> 3, 0xE0);

    /* fetch count */
    temp = (mode->CrtcHDisplay * (pScrn->bitsPerPixel >> 3)) >> 3;
    /* Make sure that this is 32-byte aligned. */
    if (temp & 0x03) {
        temp += 0x03;
        temp &= ~0x03;
    }

    hwp->writeSeq(hwp, 0x1C, ((temp >> 1)+1) & 0xFF);
    ViaSeqMask(hwp, 0x1D, temp >> 9, 0x03);

    switch (pVia->ChipId) {
        case VIA_K8M890:
        case VIA_CX700:
        case VIA_P4M900:
	case VIA_VX800:
	case VIA_VX855:
            break;
        default:
            /* some leftovers */
            ViaCrtcMask(hwp, 0x32, 0, 0xFF);
            ViaCrtcMask(hwp, 0x33, 0, 0xC8);
            break;
    }
}

void
ViaFirstCRTCSetStartingAddress(ScrnInfoPtr pScrn, int x, int y)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD32 Base;
    CARD32 tmp;

    Base = (y * pScrn->displayWidth + x) * (pScrn->bitsPerPixel / 8);
    Base = Base >> 1;

    hwp->writeCrtc(hwp, 0x0C, (Base & 0xFF00) >> 8);
    hwp->writeCrtc(hwp, 0x0D, Base & 0xFF);
    hwp->writeCrtc(hwp, 0x34, (Base & 0xFF0000) >> 16);

    if (!(pVia->Chipset == VIA_CLE266 && CLE266_REV_IS_AX(pVia->ChipRev)))
        ViaCrtcMask(hwp, 0x48, Base >> 24, 0x0F);
}

void
ViaSecondCRTCSetStartingAddress(ScrnInfoPtr pScrn, int x, int y)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD32 Base;
    CARD32 tmp;

    Base = (y * pScrn->displayWidth + x) * (pScrn->bitsPerPixel / 8);
    Base = (Base + pScrn->fbOffset) >> 3;

    tmp = hwp->readCrtc(hwp, 0x62) & 0x01;
    tmp |= (Base & 0x7F) << 1;
    hwp->writeCrtc(hwp, 0x62, tmp);

    hwp->writeCrtc(hwp, 0x63, (Base & 0x7F80) >> 7);
    hwp->writeCrtc(hwp, 0x64, (Base & 0x7F8000) >> 15);
    hwp->writeCrtc(hwp, 0xA3, (Base & 0x03800000) >> 23);
}

void
ViaSecondCRTCHorizontalQWCount(ScrnInfoPtr pScrn, int width)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD16 temp;

    /* fetch count */
    temp = (width * (pScrn->bitsPerPixel >> 3)) >> 3;
    /* Make sure that this is 32-byte aligned. */
    if (temp & 0x03) {
        temp += 0x03;
        temp &= ~0x03;
    }
    hwp->writeCrtc(hwp, 0x65, (temp >> 1) & 0xFF);
    ViaCrtcMask(hwp, 0x67, temp >> 7, 0x0C);
}

void
ViaSecondCRTCHorizontalOffset(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD16 temp;

    /* offset */
    temp = (pScrn->displayWidth * (pScrn->bitsPerPixel >> 3)) >> 3;
    /* Make sure that this is 32-byte aligned. */
    if (temp & 0x03) {
        temp += 0x03;
        temp &= ~0x03;
	}
    
    hwp->writeCrtc(hwp, 0x66, temp & 0xFF);
    ViaCrtcMask(hwp, 0x67, temp >> 8, 0x03);
}

void
ViaSecondCRTCSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD16 temp;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "mode: %p\n", mode);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "mode->name: %p\n", mode->name);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "mode->name: %s\n", mode->name);


    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaSecondCRTCSetMode\n"));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Setting up %s\n", mode->name));
    /* bpp */
    switch (pScrn->bitsPerPixel) {
        case 8:
            ViaCrtcMask(hwp, 0x67, 0x00, 0xC0);
            break;
        case 16:
            ViaCrtcMask(hwp, 0x67, 0x40, 0xC0);
            break;
        case 24:
        case 32:
            ViaCrtcMask(hwp, 0x67, 0xC0, 0xC0);
            break;
        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unhandled bitdepth: %d\n",
                       pScrn->bitsPerPixel);
            break;
    }

    switch (pVia->ChipId) {
        case VIA_K8M890:
        case VIA_CX700:
        case VIA_P4M900:
	case VIA_VX800:
	case VIA_VX855:
            break;
        default:
            ViaSeqMask(hwp, 0x16, 0x08, 0xBF);
            ViaSeqMask(hwp, 0x17, 0x1F, 0xFF);
            ViaSeqMask(hwp, 0x18, 0x4E, 0xFF);
            ViaSeqMask(hwp, 0x1A, 0x08, 0xFD);
            break;
    }

    /* Crtc registers */
    /* horizontal total : 4096 */
    temp = mode->CrtcHTotal - 1;
    hwp->writeCrtc(hwp, 0x50, temp & 0xFF);
    ViaCrtcMask(hwp, 0x55, temp >> 8, 0x0F);

    /* horizontal address : 2048 */
    temp = mode->CrtcHDisplay - 1;
    hwp->writeCrtc(hwp, 0x51, temp & 0xFF);
    ViaCrtcMask(hwp, 0x55, temp >> 4, 0x70);

    /* horizontal blanking start : 2048 */
    /* temp = mode->CrtcHDisplay - 1; */
    temp = mode->CrtcHBlankStart - 1;
    hwp->writeCrtc(hwp, 0x52, temp & 0xFF);
    ViaCrtcMask(hwp, 0x54, temp >> 8, 0x07);

    /* horizontal blanking end : 4096 */
    /* temp = mode->CrtcHTotal - 1; */
    temp = mode->CrtcHBlankEnd - 1;
    hwp->writeCrtc(hwp, 0x53, temp & 0xFF);
    ViaCrtcMask(hwp, 0x54, temp >> 5, 0x38);
    ViaCrtcMask(hwp, 0x5D, temp >> 5, 0x40);

    /* horizontal sync start : 2047 */
    temp = mode->CrtcHSyncStart;
    hwp->writeCrtc(hwp, 0x56, temp & 0xFF);
    ViaCrtcMask(hwp, 0x54, temp >> 2, 0xC0);
    ViaCrtcMask(hwp, 0x5C, temp >> 3, 0x80);

    if (pVia->ChipId != VIA_CLE266 && pVia->ChipId != VIA_KM400)
        ViaCrtcMask(hwp, 0x5D, temp >> 4, 0x80);

    /* horizontal sync end : sync start + 512 */
    temp = mode->CrtcHSyncEnd;
    hwp->writeCrtc(hwp, 0x57, temp & 0xFF);
    ViaCrtcMask(hwp, 0x5C, temp >> 2, 0x40);

    /* vertical total : 2048 */
    temp = mode->CrtcVTotal - 1;
    hwp->writeCrtc(hwp, 0x58, temp & 0xFF);
    ViaCrtcMask(hwp, 0x5D, temp >> 8, 0x07);

    /* vertical address : 2048 */
    temp = mode->CrtcVDisplay - 1;
    hwp->writeCrtc(hwp, 0x59, temp & 0xFF);
    ViaCrtcMask(hwp, 0x5D, temp >> 5, 0x38);

    /* vertical blanking start : 2048 */
    /* temp = mode->CrtcVDisplay - 1; */
    temp = mode->CrtcVBlankStart - 1;
    hwp->writeCrtc(hwp, 0x5A, temp & 0xFF);
    ViaCrtcMask(hwp, 0x5C, temp >> 8, 0x07);

    /* vertical blanking end : 2048 */
    /* temp = mode->CrtcVTotal - 1; */
    temp = mode->CrtcVBlankEnd - 1;
    hwp->writeCrtc(hwp, 0x5B, temp & 0xFF);
    ViaCrtcMask(hwp, 0x5C, temp >> 5, 0x38);

    /* vertical sync start : 2047 */
    temp = mode->CrtcVSyncStart;
    hwp->writeCrtc(hwp, 0x5E, temp & 0xFF);
    ViaCrtcMask(hwp, 0x5F, temp >> 3, 0xE0);

    /* vertical sync end : start + 32 */
    temp = mode->CrtcVSyncEnd;
    ViaCrtcMask(hwp, 0x5F, temp, 0x1F);

    switch (pVia->ChipId) {
        case VIA_K8M890:
        case VIA_CX700:
        case VIA_P4M900:
	case VIA_VX800:
	case VIA_VX855:
            break;
        default:
            /* some leftovers */
            hwp->writeCrtc(hwp, 0x08, 0x00);
            ViaCrtcMask(hwp, 0x32, 0, 0xFF);  /* ? */
            ViaCrtcMask(hwp, 0x33, 0, 0xC8);
            break;
    }

    ViaSecondCRTCHorizontalOffset(pScrn);
    ViaSecondCRTCHorizontalQWCount(pScrn, mode->CrtcHDisplay);

}

/*
 * Checks for limitations imposed by the available VGA timing registers.
 */
ModeStatus
ViaFirstCRTCModeValid(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaFirstCRTCModeValid\n"));

    if (mode->CrtcHTotal > 4100)
        return MODE_BAD_HVALUE;

    if (mode->CrtcHDisplay > 2048)
        return MODE_BAD_HVALUE;

    if (mode->CrtcHBlankStart > 2048)
        return MODE_BAD_HVALUE;

    if ((mode->CrtcHBlankEnd - mode->CrtcHBlankStart) > 1025)
        return MODE_HBLANK_WIDE;

    if (mode->CrtcHSyncStart > 4095)
        return MODE_BAD_HVALUE;

    if ((mode->CrtcHSyncEnd - mode->CrtcHSyncStart) > 256)
        return MODE_HSYNC_WIDE;

    if (mode->CrtcVTotal > 2049)
        return MODE_BAD_VVALUE;

    if (mode->CrtcVDisplay > 2048)
        return MODE_BAD_VVALUE;

    if (mode->CrtcVSyncStart > 2047)
        return MODE_BAD_VVALUE;

    if ((mode->CrtcVSyncEnd - mode->CrtcVSyncStart) > 16)
        return MODE_VSYNC_WIDE;

    if (mode->CrtcVBlankStart > 2048)
        return MODE_BAD_VVALUE;

    if ((mode->CrtcVBlankEnd - mode->CrtcVBlankStart) > 257)
        return MODE_VBLANK_WIDE;

    return MODE_OK;
}

ModeStatus
ViaSecondCRTCModeValid(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaSecondCRTCModeValid\n"));

    if (mode->CrtcHTotal > 4096)
        return MODE_BAD_HVALUE;

    if (mode->CrtcHDisplay > 2048)
        return MODE_BAD_HVALUE;

    if (mode->CrtcHBlankStart > 2048)
        return MODE_BAD_HVALUE;

    if (mode->CrtcHBlankEnd > 4096)
        return MODE_HBLANK_WIDE;

    if (mode->CrtcHSyncStart > 2047)
        return MODE_BAD_HVALUE;

    if ((mode->CrtcHSyncEnd - mode->CrtcHSyncStart) > 512)
        return MODE_HSYNC_WIDE;

    if (mode->CrtcVTotal > 2048)
        return MODE_BAD_VVALUE;

    if (mode->CrtcVDisplay > 2048)
        return MODE_BAD_VVALUE;

    if (mode->CrtcVBlankStart > 2048)
        return MODE_BAD_VVALUE;

    if (mode->CrtcVBlankEnd > 2048)
        return MODE_VBLANK_WIDE;

    if (mode->CrtcVSyncStart > 2047)
        return MODE_BAD_VVALUE;

    if ((mode->CrtcVSyncEnd - mode->CrtcVSyncStart) > 32)
        return MODE_VSYNC_WIDE;

    return MODE_OK;
}

/*
 * Not tested yet
 */
void
ViaShadowCRTCSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaShadowCRTCSetMode\n"));

    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD16 temp;

    temp = (mode->CrtcHTotal >> 3) - 5;
    hwp->writeCrtc(hwp, 0x6D, temp & 0xFF);
    ViaCrtcMask(hwp, 0x71, temp >> 5, 0x08);

    temp = (mode->CrtcHBlankEnd >> 3) - 1;
    hwp->writeCrtc(hwp, 0x6E, temp & 0xFF);

    temp = mode->CrtcVTotal - 2;
    hwp->writeCrtc(hwp, 0x6F, temp & 0xFF);
    ViaCrtcMask(hwp, 0x71, temp >> 8, 0x07);

    temp = mode->CrtcVDisplay - 1;
    hwp->writeCrtc(hwp, 0x70, temp & 0xFF);
    ViaCrtcMask(hwp, 0x71, temp >> 4, 0x70);

    temp = mode->CrtcVBlankStart - 1;
    hwp->writeCrtc(hwp, 0x72, temp & 0xFF);
    ViaCrtcMask(hwp, 0x74, temp >> 4, 0x70);

    temp = mode->CrtcVTotal - 1;
    hwp->writeCrtc(hwp, 0x73, temp & 0xFF);
    ViaCrtcMask(hwp, 0x74, temp >> 8, 0x07);

    ViaCrtcMask(hwp, 0x76, mode->CrtcVSyncEnd, 0x0F);

    temp = mode->CrtcVSyncStart;
    hwp->writeCrtc(hwp, 0x75, temp & 0xFF);
    ViaCrtcMask(hwp, 0x76, temp >> 4, 0x70);
}
