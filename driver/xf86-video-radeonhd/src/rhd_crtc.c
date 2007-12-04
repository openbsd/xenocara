/*
 * Copyright 2007  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2007  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007  Egbert Eich   <eich@novell.com>
 * Copyright 2007  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

/* for usleep */
#include "xf86_ansic.h"

#include "rhd.h"
#include "rhd_crtc.h"
#include "rhd_pll.h"
#include "rhd_lut.h"
#include "rhd_regs.h"

#define D1_REG_OFFSET 0x0000
#define D2_REG_OFFSET 0x0800

struct rhdCrtcStore {
    CARD32 GrphEnable;
    CARD32 GrphControl;
    CARD32 GrphXStart;
    CARD32 GrphYStart;
    CARD32 GrphXEnd;
    CARD32 GrphYEnd;
    CARD32 GrphPrimarySurfaceAddress;
    CARD32 GrphSurfaceOffsetX;
    CARD32 GrphSurfaceOffsetY;
    CARD32 GrphPitch;
    CARD32 GrphLutSel;

    CARD32 ModeViewPortSize;
    CARD32 ModeViewPortStart;
    CARD32 ModeDesktopHeight;
    CARD32 ModeOverScanH;
    CARD32 ModeOverScanV;

    CARD32 CrtcControl;

    CARD32 CrtcHTotal;
    CARD32 CrtcHBlankStartEnd;
    CARD32 CrtcHSyncA;
    CARD32 CrtcHSyncACntl;
    CARD32 CrtcHSyncB;
    CARD32 CrtcHSyncBCntl;

    CARD32 CrtcVTotal;
    CARD32 CrtcVBlankStartEnd;
    CARD32 CrtcVSyncA;
    CARD32 CrtcVSyncACntl;
    CARD32 CrtcVSyncB;
    CARD32 CrtcVSyncBCntl;

    CARD32 CrtcBlackColor;
    CARD32 CrtcBlankControl;
    CARD32 CrtcPCLKControl;
};

/*
 * Checks whether Width, Height are within boundaries.
 * If MODE_OK is returned and pPitch is not NULL, it is set.
 */
static ModeStatus
DxFBValid(struct rhdCrtc *Crtc, CARD16 Width, CARD16 Height, int bpp,
	  CARD32 Offset, CARD32 Size, CARD32 *pPitch)
{
    ScrnInfoPtr pScrn = xf86Screens[Crtc->scrnIndex];
    CARD16 Pitch;
    unsigned int BytesPerPixel;
    CARD8 PitchMask = 0xFF;

    RHDDebug(Crtc->scrnIndex, "FUNCTION: %s: %s\n", __func__, Crtc->Name);

    /* If we hit this, then the memory claimed so far is not properly aligned */
    if (Offset & 0xFFF) {
	xf86DrvMsg(Crtc->scrnIndex, X_ERROR, "%s: Offset (0x%08X) is invalid!\n",
		   __func__, (int) Offset);
	return MODE_ERROR;
    }

    switch (pScrn->bitsPerPixel) {
    case 8:
	BytesPerPixel = 1;
	break;
    case 15:
    case 16:
	BytesPerPixel = 2;
	PitchMask /= BytesPerPixel;
	break;
    case 24:
    case 32:
	BytesPerPixel = 4;
	PitchMask /= BytesPerPixel;
	break;
    default:
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "%s: %dbpp is not implemented!\n",
		   __func__, pScrn->bitsPerPixel);
	return MODE_BAD;
    }

     /* Be reasonable */
     if (Width < 0x100)
	 return MODE_H_ILLEGAL;
     if (Height < 0x100)
	 return MODE_V_ILLEGAL;

    /* D1GRPH_X_START is 14bits while D1_MODE_VIEWPORT_X_START is only 13 bits.
     * Since it is reasonable to assume that modes will be at least 1x1
     * limit at 13bits + 1 */
    if (Width > 0x2000)
	return MODE_VIRTUAL_X;

    /* D1GRPH_Y_START is 14bits while D1_MODE_VIEWPORT_Y_START is only 13 bits.
     * Since it is reasonable to assume that modes will be at least 1x1
     * limit at 13bits + 1 */
    if (Height > 0x2000)
	return MODE_VIRTUAL_Y;

    Pitch = (Width + PitchMask) & ~PitchMask;
    /* D1_PITCH limit: should never happen after clamping Width to 0x2000 */
    if (Pitch >= 0x4000)
	return MODE_VIRTUAL_X;

    if ((Pitch * BytesPerPixel * Height) > Size)
	return MODE_MEM_VIRT;

    if (pPitch)
	*pPitch = Pitch;
    return MODE_OK;
}

/*
 *
 */
static void
DxFBSet(struct rhdCrtc *Crtc, CARD16 Pitch, CARD16 Width, CARD16 Height,
	int bpp, CARD32 Offset)
{
    RHDPtr rhdPtr = RHDPTRI(Crtc);
    CARD16 RegOff;

    RHDDebug(Crtc->scrnIndex, "%s: %s\n", __func__, Crtc->Name);

    if (Crtc->Id == RHD_CRTC_1)
	RegOff = D1_REG_OFFSET;
    else
	RegOff = D2_REG_OFFSET;

    RHDRegMask(Crtc, RegOff + D1GRPH_ENABLE, 1, 0x00000001);

    switch (bpp) {
    case 8:
	RHDRegMask(Crtc, RegOff + D1GRPH_CONTROL, 0, 0xF10703);
	break;
    case 15:
	RHDRegMask(Crtc, RegOff + D1GRPH_CONTROL, 0x000001, 0xF10703);
	break;
    case 16:
	RHDRegMask(Crtc, RegOff + D1GRPH_CONTROL, 0x000101, 0xF10703);
	break;
    case 24:
    case 32:
    default:
	RHDRegMask(Crtc, RegOff + D1GRPH_CONTROL, 0x000002, 0xF10703);
	break;
    /* TODO: 64bpp ;p */
    }

    RHDRegWrite(Crtc, RegOff + D1GRPH_PRIMARY_SURFACE_ADDRESS,
		rhdPtr->FbIntAddress + Offset);
    RHDRegWrite(Crtc, RegOff + D1GRPH_PITCH, Pitch);
    RHDRegWrite(Crtc, RegOff + D1GRPH_SURFACE_OFFSET_X, 0);
    RHDRegWrite(Crtc, RegOff + D1GRPH_SURFACE_OFFSET_Y, 0);
    RHDRegWrite(Crtc, RegOff + D1GRPH_X_START, 0);
    RHDRegWrite(Crtc, RegOff + D1GRPH_Y_START, 0);
    RHDRegWrite(Crtc, RegOff + D1GRPH_X_END, Width);
    RHDRegWrite(Crtc, RegOff + D1GRPH_Y_END, Height);

    /* D1Mode registers */
    RHDRegWrite(rhdPtr, RegOff + D1MODE_DESKTOP_HEIGHT, Height);

    Crtc->Pitch = Pitch;
    Crtc->Width = Width;
    Crtc->Height = Height;
    Crtc->bpp = bpp;
    Crtc->Offset = Offset;
}

/*
 *
 */
static ModeStatus
DxModeValid(struct rhdCrtc *Crtc, DisplayModePtr Mode)
{
    CARD32 tmp;

    RHDDebug(Crtc->scrnIndex, "%s: %s\n", __func__, Crtc->Name);

    /* D1_MODE_VIEWPORT_WIDTH: 14bits */
    if (Mode->CrtcHDisplay >= 0x4000)
	return MODE_BAD_HVALUE;

    /* D1CRTC_H_TOTAL - 1 : 13bits */
    if (Mode->CrtcHTotal > 0x2000)
	return MODE_BAD_HVALUE;

    tmp = Mode->CrtcHTotal + Mode->CrtcHBlankStart - Mode->CrtcHSyncStart;
    /* D1CRTC_H_BLANK_START: 13bits */
    if (tmp >= 0x2000)
	return MODE_BAD_HVALUE;

    tmp = Mode->CrtcHBlankEnd - Mode->CrtcHSyncStart;
    /* D1CRTC_H_BLANK_END: 13bits */
    if (tmp >= 0x2000)
	return MODE_BAD_HVALUE;

    tmp = Mode->CrtcHSyncEnd - Mode->CrtcHSyncStart;
    /* D1CRTC_H_SYNC_A_END: 13bits */
    if (tmp >= 0x2000)
	return MODE_HSYNC_WIDE;

    /* D1_MODE_VIEWPORT_HEIGHT: 14bits */
    if (Mode->CrtcVDisplay >= 0x4000)
	return MODE_BAD_VVALUE;

    /* D1CRTC_V_TOTAL - 1 : 13bits */
    if (Mode->CrtcVTotal > 0x2000)
	return MODE_BAD_VVALUE;

    tmp = Mode->CrtcVTotal + Mode->CrtcVBlankStart - Mode->CrtcVSyncStart;
    /* D1CRTC_V_BLANK_START: 13bits */
    if (tmp >= 0x2000)
	return MODE_BAD_VVALUE;

    tmp = Mode->CrtcVBlankEnd - Mode->CrtcVSyncStart;
    /* D1CRTC_V_BLANK_END: 13bits */
    if (tmp >= 0x2000)
	return MODE_BAD_VVALUE;

    tmp = Mode->CrtcVSyncEnd - Mode->CrtcVSyncStart;
    /* D1CRTC_V_SYNC_A_END: 13bits */
    if (tmp >= 0x2000)
	return MODE_VSYNC_WIDE;

    return MODE_OK;
}

/*
 *
 */
static void
DxModeSet(struct rhdCrtc *Crtc, DisplayModePtr Mode)
{
    CARD16 BlankStart, BlankEnd;
    CARD16 RegOff;

    RHDDebug(Crtc->scrnIndex, "%s: %s\n", __func__, Crtc->Name);

    if (Crtc->Id == RHD_CRTC_1)
	RegOff = D1_REG_OFFSET;
    else
	RegOff = D2_REG_OFFSET;

    /* D1Mode registers */
    RHDRegWrite(Crtc, RegOff + D1MODE_VIEWPORT_SIZE,
		Mode->CrtcVDisplay | (Mode->CrtcHDisplay << 16));
    RHDRegWrite(Crtc, RegOff + D1MODE_VIEWPORT_START, 0);
    /* Docs for D1MODE_VIEWPORT_END ???? */
    RHDRegWrite(Crtc, RegOff + D1MODE_EXT_OVERSCAN_LEFT_RIGHT,
		((Mode->CrtcHTotal - Mode->CrtcHBlankEnd) << 16) |
		(Mode->CrtcHBlankStart - Mode->CrtcHDisplay));
    RHDRegWrite(Crtc, RegOff + D1MODE_EXT_OVERSCAN_TOP_BOTTOM,
		((Mode->CrtcVTotal - Mode->CrtcVBlankEnd) << 16) |
		(Mode->CrtcVBlankStart - Mode->CrtcVDisplay));

    /* Crtc registers */

    /* enable read requests */
    RHDRegMask(Crtc, RegOff + D1CRTC_CONTROL, 0, 0x01000000);

    /* Horizontal */
    RHDRegWrite(Crtc, RegOff + D1CRTC_H_TOTAL, Mode->CrtcHTotal - 1);

    BlankStart = Mode->CrtcHTotal + Mode->CrtcHBlankStart - Mode->CrtcHSyncStart;
    BlankEnd = Mode->CrtcHBlankEnd - Mode->CrtcHSyncStart;
    RHDRegWrite(Crtc, RegOff + D1CRTC_H_BLANK_START_END,
		BlankStart | (BlankEnd << 16));

    RHDRegWrite(Crtc, RegOff + D1CRTC_H_SYNC_A,
		(Mode->CrtcHSyncEnd - Mode->CrtcHSyncStart) << 16);
    RHDRegWrite(Crtc, RegOff + D1CRTC_H_SYNC_A_CNTL, Mode->Flags & V_NHSYNC);

    /* Vertical */
    RHDRegWrite(Crtc, RegOff + D1CRTC_V_TOTAL, Mode->CrtcVTotal - 1);

    BlankStart = Mode->CrtcVTotal + Mode->CrtcVBlankStart - Mode->CrtcVSyncStart;
    BlankEnd = Mode->CrtcVBlankEnd - Mode->CrtcVSyncStart;
    RHDRegWrite(Crtc, RegOff + D1CRTC_V_BLANK_START_END,
		BlankStart | (BlankEnd << 16));

    RHDRegWrite(Crtc, RegOff + D1CRTC_V_SYNC_A,
		(Mode->CrtcVSyncEnd - Mode->CrtcVSyncStart) << 16);
    RHDRegWrite(Crtc, RegOff + D1CRTC_V_SYNC_A_CNTL, Mode->Flags & V_NVSYNC);

    Crtc->CurrentMode = Mode;
}

/*
 *
 */
static void
D1PLLSelect(struct rhdCrtc *Crtc, struct rhdPLL *PLL)
{
    RHDFUNC(Crtc);

    RHDRegMask(Crtc, PCLK_CRTC1_CNTL, PLL->Id << 16, 0x00010000);
    Crtc->PLL = PLL;
}

/*
 *
 */
static void
D2PLLSelect(struct rhdCrtc *Crtc, struct rhdPLL *PLL)
{
    RHDFUNC(Crtc);

    RHDRegMask(Crtc, PCLK_CRTC2_CNTL, PLL->Id << 16, 0x00010000);
    Crtc->PLL = PLL;
}

/*
 *
 */
static void
D1LUTSelect(struct rhdCrtc *Crtc, struct rhdLUT *LUT)
{
    RHDFUNC(Crtc);

    RHDRegWrite(Crtc, D1GRPH_LUT_SEL, LUT->Id & 1);
    Crtc->LUT = LUT;
}

/*
 *
 */
static void
D2LUTSelect(struct rhdCrtc *Crtc, struct rhdLUT *LUT)
{
    RHDFUNC(Crtc);

    RHDRegWrite(Crtc, D2GRPH_LUT_SEL, LUT->Id & 1);
    Crtc->LUT = LUT;
}

/*
 *
 */
static void
D1ViewPortStart(struct rhdCrtc *Crtc, CARD16 X, CARD16 Y)
{
    RHDFUNC(Crtc);

    /* not as granular as docs make it seem to be.
     * if the lower two bits are set the line buffer might screw up, requiring
     * a power cycle. */
    X = (X + 0x02) & ~0x03;
    Y &= ~0x01;

    RHDRegMask(Crtc, D1SCL_UPDATE, 0x00010000, 0x0001000);
    RHDRegWrite(Crtc, D1MODE_VIEWPORT_START, (X << 16) | Y);
    RHDRegMask(Crtc, D1SCL_UPDATE, 0, 0x0001000);

    Crtc->X = X;
    Crtc->Y = Y;
}

/*
 *
 */
static void
D2ViewPortStart(struct rhdCrtc *Crtc, CARD16 X, CARD16 Y)
{
    RHDFUNC(Crtc);

    /* not as granular as docs make it seem to be. */
    X = (X + 0x02) & ~0x03;
    Y &= ~0x01;

    RHDRegMask(Crtc, D2SCL_UPDATE, 0x00010000, 0x0001000);
    RHDRegWrite(Crtc, D2MODE_VIEWPORT_START, (X << 16) | Y);
    RHDRegMask(Crtc, D2SCL_UPDATE, 0, 0x0001000);

    Crtc->X = X;
    Crtc->Y = Y;
}

#define CRTC_SYNC_WAIT 0x100000
/*
 *
 */
static void
D1CRTCDisable(struct rhdCrtc *Crtc)
{
    if (RHDRegRead(Crtc, D1CRTC_CONTROL) & 1) {
	CARD32 Control = RHDRegRead(Crtc, D1CRTC_CONTROL);
	int i;

	RHDRegMask(Crtc, D1CRTC_CONTROL, 0, 0x0301);

	for (i = 0; i < CRTC_SYNC_WAIT; i++)
	    if (!(RHDRegRead(Crtc, D1CRTC_STATUS) & 1)) {
		RHDRegMask(Crtc, D1CRTC_CONTROL, Control, 0x0300);
		RHDDebug(Crtc->scrnIndex, "%s: %d loops\n", __func__, i);
		return;
	    }
	xf86DrvMsg(Crtc->scrnIndex, X_WARNING,
		   "%s: Failed to Unsync %s\n", __func__, Crtc->Name);
    }
}

/*
 *
 */
static void
D2CRTCDisable(struct rhdCrtc *Crtc)
{
    if (RHDRegRead(Crtc, D2CRTC_CONTROL) & 1) {
	CARD32 Control = RHDRegRead(Crtc, D2CRTC_CONTROL);
	int i;

	RHDRegMask(Crtc, D2CRTC_CONTROL, 0, 0xFF01);

	for (i = 0; i < CRTC_SYNC_WAIT; i++)
	    if (!(RHDRegRead(Crtc, D2CRTC_STATUS) & 1)) {
		RHDRegMask(Crtc, D2CRTC_CONTROL, Control, 0x0300);
		RHDDebug(Crtc->scrnIndex, "%s: %d loops\n", __func__, i);
		return;
	    }
	xf86DrvMsg(Crtc->scrnIndex, X_WARNING,
		   "%s: Failed to Unsync %s\n", __func__, Crtc->Name);
    }
}

/*
 *
 */
static void
D1Power(struct rhdCrtc *Crtc, int Power)
{
    RHDFUNC(Crtc);

    switch (Power) {
    case RHD_POWER_ON:
	RHDRegMask(Crtc, D1GRPH_ENABLE, 0x00000001, 0x00000001);
	usleep(2);
	RHDRegMask(Crtc, D1CRTC_CONTROL, 0, 0x01000000); /* enable read requests */
	RHDRegMask(Crtc, D1CRTC_CONTROL, 1, 1);
	return;
    case RHD_POWER_RESET:
	RHDRegMask(Crtc, D1CRTC_CONTROL, 0x01000000, 0x01000000); /* disable read requests */
	D1CRTCDisable(Crtc);
	return;
    case RHD_POWER_SHUTDOWN:
    default:
	RHDRegMask(Crtc, D1CRTC_CONTROL, 0x01000000, 0x01000000); /* disable read requests */
	D1CRTCDisable(Crtc);
	RHDRegMask(Crtc, D1GRPH_ENABLE, 0, 0x00000001);
	return;
    }
}

/*
 *
 */
static void
D2Power(struct rhdCrtc *Crtc, int Power)
{
    RHDFUNC(Crtc);

    switch (Power) {
    case RHD_POWER_ON:
	RHDRegMask(Crtc, D2GRPH_ENABLE, 0x00000001, 0x00000001);
	usleep(2);
	RHDRegMask(Crtc, D2CRTC_CONTROL, 0, 0x01000000); /* enable read requests */
	RHDRegMask(Crtc, D2CRTC_CONTROL, 1, 1);
	return;
    case RHD_POWER_RESET:
	RHDRegMask(Crtc, D2CRTC_CONTROL, 0x01000000, 0x01000000); /* disable read requests */
	D2CRTCDisable(Crtc);
	return;
    case RHD_POWER_SHUTDOWN:
    default:
	RHDRegMask(Crtc, D2CRTC_CONTROL, 0x01000000, 0x01000000); /* disable read requests */
	D2CRTCDisable(Crtc);
	RHDRegMask(Crtc, D2GRPH_ENABLE, 0, 0x00000001);
	return;
    }
}

/*
 * This is quite different from Power. Power disables and enables things,
 * this here makes the hw send out black, and can switch back and forth
 * immediately. Useful for covering up a framebuffer that is not filled
 * in yet.
 */
static void
D1Blank(struct rhdCrtc *Crtc, Bool Blank)
{
    RHDFUNC(Crtc);

    RHDRegWrite(Crtc, D1CRTC_BLACK_COLOR, 0);
    if (Blank)
	RHDRegMask(Crtc, D1CRTC_BLANK_CONTROL, 0x00000100, 0x00000100);
    else
	RHDRegMask(Crtc, D1CRTC_BLANK_CONTROL, 0, 0x00000100);
}

/*
 *
 */
static void
D2Blank(struct rhdCrtc *Crtc, Bool Blank)
{
    RHDFUNC(Crtc);

    RHDRegWrite(Crtc, D2CRTC_BLACK_COLOR, 0);
    if (Blank)
	RHDRegMask(Crtc, D2CRTC_BLANK_CONTROL, 0x00000100, 0x00000100);
    else
	RHDRegMask(Crtc, D2CRTC_BLANK_CONTROL, 0, 0x00000100);
}

/*
 *
 */
static void
DxSave(struct rhdCrtc *Crtc)
{
    struct rhdCrtcStore *Store;
    CARD16 RegOff;

    RHDDebug(Crtc->scrnIndex, "%s: %s\n", __func__, Crtc->Name);

    if (Crtc->Id == RHD_CRTC_1)
	RegOff = D1_REG_OFFSET;
    else
	RegOff = D2_REG_OFFSET;

    if (!Crtc->Store)
	Store = xnfcalloc(sizeof(struct rhdCrtcStore), 1);
    else
	Store = Crtc->Store;

    Store->GrphEnable = RHDRegRead(Crtc, RegOff + D1GRPH_ENABLE);
    Store->GrphControl = RHDRegRead(Crtc, RegOff + D1GRPH_CONTROL);
    Store->GrphXStart = RHDRegRead(Crtc, RegOff + D1GRPH_X_START);
    Store->GrphYStart = RHDRegRead(Crtc, RegOff + D1GRPH_Y_START);
    Store->GrphXEnd = RHDRegRead(Crtc, RegOff + D1GRPH_X_END);
    Store->GrphYEnd = RHDRegRead(Crtc, RegOff + D1GRPH_Y_END);
    Store->GrphPrimarySurfaceAddress =
	RHDRegRead(Crtc, RegOff + D1GRPH_PRIMARY_SURFACE_ADDRESS);
    Store->GrphSurfaceOffsetX =
	RHDRegRead(Crtc, RegOff + D1GRPH_SURFACE_OFFSET_X);
    Store->GrphSurfaceOffsetY =
	RHDRegRead(Crtc, RegOff + D1GRPH_SURFACE_OFFSET_Y);
    Store->GrphPitch = RHDRegRead(Crtc, RegOff + D1GRPH_PITCH);
    Store->GrphLutSel = RHDRegRead(Crtc, RegOff + D1GRPH_LUT_SEL);

    Store->ModeViewPortSize = RHDRegRead(Crtc, RegOff + D1MODE_VIEWPORT_SIZE);
    Store->ModeViewPortStart = RHDRegRead(Crtc, RegOff + D1MODE_VIEWPORT_START);
    Store->ModeDesktopHeight = RHDRegRead(Crtc, RegOff + D1MODE_DESKTOP_HEIGHT);
    Store->ModeOverScanH =
	RHDRegRead(Crtc, RegOff + D1MODE_EXT_OVERSCAN_LEFT_RIGHT);
    Store->ModeOverScanV =
	RHDRegRead(Crtc, RegOff + D1MODE_EXT_OVERSCAN_TOP_BOTTOM);

    Store->CrtcControl = RHDRegRead(Crtc, RegOff + D1CRTC_CONTROL);

    Store->CrtcHTotal = RHDRegRead(Crtc, RegOff + D1CRTC_H_TOTAL);
    Store->CrtcHBlankStartEnd =
	RHDRegRead(Crtc, RegOff + D1CRTC_H_BLANK_START_END);
    Store->CrtcHSyncA = RHDRegRead(Crtc, RegOff + D1CRTC_H_SYNC_A);
    Store->CrtcHSyncACntl = RHDRegRead(Crtc, RegOff + D1CRTC_H_SYNC_A_CNTL);
    Store->CrtcHSyncB = RHDRegRead(Crtc, RegOff + D1CRTC_H_SYNC_B);
    Store->CrtcHSyncBCntl = RHDRegRead(Crtc, RegOff + D1CRTC_H_SYNC_B_CNTL);

    Store->CrtcVTotal = RHDRegRead(Crtc, RegOff + D1CRTC_V_TOTAL);
    Store->CrtcVBlankStartEnd =
	RHDRegRead(Crtc, RegOff + D1CRTC_V_BLANK_START_END);
    Store->CrtcVSyncA = RHDRegRead(Crtc, RegOff + D1CRTC_V_SYNC_A);
    Store->CrtcVSyncACntl = RHDRegRead(Crtc, RegOff + D1CRTC_V_SYNC_A_CNTL);
    Store->CrtcVSyncB = RHDRegRead(Crtc, RegOff + D1CRTC_V_SYNC_B);
    Store->CrtcVSyncBCntl = RHDRegRead(Crtc, RegOff + D1CRTC_V_SYNC_B_CNTL);

    Store->CrtcBlackColor = RHDRegRead(Crtc, RegOff + D1CRTC_BLACK_COLOR);
    Store->CrtcBlankControl = RHDRegRead(Crtc, RegOff + D1CRTC_BLANK_CONTROL);

    if (Crtc->Id == RHD_CRTC_1)
	Store->CrtcPCLKControl = RHDRegRead(Crtc, PCLK_CRTC1_CNTL);
    else
	Store->CrtcPCLKControl = RHDRegRead(Crtc, PCLK_CRTC2_CNTL);

    Crtc->Store = Store;
}

/*
 *
 */
static void
DxRestore(struct rhdCrtc *Crtc)
{
    struct rhdCrtcStore *Store = Crtc->Store;
    CARD16 RegOff;

    RHDDebug(Crtc->scrnIndex, "%s: %s\n", __func__, Crtc->Name);

    if (Crtc->Id == RHD_CRTC_1)
	RegOff = D1_REG_OFFSET;
    else
	RegOff = D2_REG_OFFSET;

    if (!Store) {
	xf86DrvMsg(Crtc->scrnIndex, X_ERROR, "%s: no registers stored!\n",
		   __func__);
	return;
    }

    RHDRegWrite(Crtc, RegOff + D1GRPH_ENABLE, Store->GrphEnable);
    RHDRegWrite(Crtc, RegOff + D1GRPH_CONTROL, Store->GrphControl);
    RHDRegWrite(Crtc, RegOff + D1GRPH_X_START, Store->GrphXStart);
    RHDRegWrite(Crtc, RegOff + D1GRPH_Y_START, Store->GrphYStart);
    RHDRegWrite(Crtc, RegOff + D1GRPH_X_END, Store->GrphXEnd);
    RHDRegWrite(Crtc, RegOff + D1GRPH_Y_END, Store->GrphYEnd);
    RHDRegWrite(Crtc, RegOff + D1GRPH_PRIMARY_SURFACE_ADDRESS,
		Store->GrphPrimarySurfaceAddress);
    RHDRegWrite(Crtc, RegOff + D1GRPH_SURFACE_OFFSET_X,
		Store->GrphSurfaceOffsetX);
    RHDRegWrite(Crtc, RegOff + D1GRPH_SURFACE_OFFSET_Y,
		Store->GrphSurfaceOffsetY);

    RHDRegWrite(Crtc, RegOff + D1GRPH_PITCH, Store->GrphPitch);
    RHDRegWrite(Crtc, RegOff + D1GRPH_LUT_SEL, Store->GrphLutSel);

    RHDRegWrite(Crtc, RegOff + D1MODE_VIEWPORT_SIZE, Store->ModeViewPortSize);
    RHDRegWrite(Crtc, RegOff + D1MODE_VIEWPORT_START, Store->ModeViewPortStart);
    RHDRegWrite(Crtc, RegOff + D1MODE_DESKTOP_HEIGHT, Store->ModeDesktopHeight);
    RHDRegWrite(Crtc, RegOff + D1MODE_EXT_OVERSCAN_LEFT_RIGHT,
		Store->ModeOverScanH);
    RHDRegWrite(Crtc, RegOff + D1MODE_EXT_OVERSCAN_TOP_BOTTOM,
		Store->ModeOverScanV);

    RHDRegWrite(Crtc, RegOff + D1CRTC_CONTROL, Store->CrtcControl);

    RHDRegWrite(Crtc, RegOff + D1CRTC_H_TOTAL, Store->CrtcHTotal);
    RHDRegWrite(Crtc, RegOff + D1CRTC_H_BLANK_START_END,
		Store->CrtcHBlankStartEnd);
    RHDRegWrite(Crtc, RegOff + D1CRTC_H_SYNC_A, Store->CrtcHSyncA);
    RHDRegWrite(Crtc, RegOff + D1CRTC_H_SYNC_A_CNTL, Store->CrtcHSyncACntl);
    RHDRegWrite(Crtc, RegOff + D1CRTC_H_SYNC_B, Store->CrtcHSyncB);
    RHDRegWrite(Crtc, RegOff + D1CRTC_H_SYNC_B_CNTL, Store->CrtcHSyncBCntl);

    RHDRegWrite(Crtc, RegOff + D1CRTC_V_TOTAL, Store->CrtcVTotal);
    RHDRegWrite(Crtc, RegOff + D1CRTC_V_BLANK_START_END,
		Store->CrtcVBlankStartEnd);
    RHDRegWrite(Crtc, RegOff + D1CRTC_V_SYNC_A, Store->CrtcVSyncA);
    RHDRegWrite(Crtc, RegOff + D1CRTC_V_SYNC_A_CNTL, Store->CrtcVSyncACntl);
    RHDRegWrite(Crtc, RegOff + D1CRTC_V_SYNC_B, Store->CrtcVSyncB);
    RHDRegWrite(Crtc, RegOff + D1CRTC_V_SYNC_B_CNTL, Store->CrtcVSyncBCntl);

    RHDRegWrite(Crtc, RegOff + D1CRTC_BLACK_COLOR, Store->CrtcBlackColor);
    RHDRegWrite(Crtc, RegOff + D1CRTC_BLANK_CONTROL, Store->CrtcBlankControl);

    if (Crtc->Id == RHD_CRTC_1)
	RHDRegWrite(Crtc, PCLK_CRTC1_CNTL, Store->CrtcPCLKControl);
    else
	RHDRegWrite(Crtc, PCLK_CRTC2_CNTL, Store->CrtcPCLKControl);

}

/*
 *
 */
void
RHDCrtcsInit(RHDPtr rhdPtr)
{
    struct rhdCrtc *Crtc;

    RHDFUNC(rhdPtr);

    Crtc = xnfcalloc(sizeof(struct rhdCrtc), 1);
    Crtc->scrnIndex = rhdPtr->scrnIndex;
    Crtc->Name = "CRTC 1";
    Crtc->Id = RHD_CRTC_1;

    Crtc->FBValid = DxFBValid;
    Crtc->FBSet = DxFBSet;
    Crtc->ModeValid = DxModeValid;
    Crtc->ModeSet = DxModeSet;
    Crtc->PLLSelect = D1PLLSelect;
    Crtc->LUTSelect = D1LUTSelect;
    Crtc->FrameSet = D1ViewPortStart;

    Crtc->Power = D1Power;
    Crtc->Blank = D1Blank;

    Crtc->Save = DxSave;
    Crtc->Restore = DxRestore;

    rhdPtr->Crtc[0] = Crtc;

    Crtc = xnfcalloc(sizeof(struct rhdCrtc), 1);
    Crtc->scrnIndex = rhdPtr->scrnIndex;
    Crtc->Name = "CRTC 2";
    Crtc->Id = RHD_CRTC_2;

    Crtc->FBValid = DxFBValid;
    Crtc->FBSet = DxFBSet;
    Crtc->ModeValid = DxModeValid;
    Crtc->ModeSet = DxModeSet;
    Crtc->PLLSelect = D2PLLSelect;
    Crtc->LUTSelect = D2LUTSelect;
    Crtc->FrameSet = D2ViewPortStart;

    Crtc->Power = D2Power;
    Crtc->Blank = D2Blank;

    Crtc->Save = DxSave;
    Crtc->Restore = DxRestore;

    rhdPtr->Crtc[1] = Crtc;
}

/*
 *
 */
void
RHDCrtcsDestroy(RHDPtr rhdPtr)
{
    struct rhdCrtc *Crtc;

    RHDFUNC(rhdPtr);

    Crtc = rhdPtr->Crtc[0];
    if (Crtc) {
	if (Crtc->Store)
	    xfree(Crtc->Store);
	xfree(Crtc);
    }

    Crtc = rhdPtr->Crtc[1];
    if (Crtc) {
	if (Crtc->Store)
	    xfree(Crtc->Store);
	xfree(Crtc);
    }
}
