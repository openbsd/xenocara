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

/*
 * Deals with the Shared LVDS/TMDS encoder.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

/* for usleep */
#include "xf86_ansic.h"

#include "rhd.h"
#include "rhd_crtc.h"
#include "rhd_connector.h"
#include "rhd_output.h"
#include "rhd_regs.h"
#include "rhd_card.h"
#ifdef ATOM_BIOS
#include "rhd_atombios.h"
#endif

/*
 * First of all, make it more managable to code for both R500 and R600, as
 * there was a 1 register shift, right in the middle of the register block.
 * There are of course much nicer ways to do the workaround i am doing here,
 * but speed is not an issue here.
 */
static inline CARD16
LVTMARegisterShift(int ChipSet, CARD16 R500, CARD16 R600)
{
    if (ChipSet >= RHD_RS690)
	return R600;
    else
	return R500;
}

#define LVTMAREGSHIFT(r500, r600) LVTMARegisterShift(rhdPtr->ChipSet, (r500), (r600))

#define LVTMA_DATA_SYNCHRONIZATION \
    LVTMAREGSHIFT(LVTMA_R500_DATA_SYNCHRONIZATION, LVTMA_R600_DATA_SYNCHRONIZATION)
#define LVTMA_PWRSEQ_REF_DIV \
    LVTMAREGSHIFT(LVTMA_R500_PWRSEQ_REF_DIV, LVTMA_R600_PWRSEQ_REF_DIV)
#define LVTMA_PWRSEQ_DELAY1 \
    LVTMAREGSHIFT(LVTMA_R500_PWRSEQ_DELAY1, LVTMA_R600_PWRSEQ_DELAY1)
#define LVTMA_PWRSEQ_DELAY2 \
    LVTMAREGSHIFT(LVTMA_R500_PWRSEQ_DELAY2, LVTMA_R600_PWRSEQ_DELAY2)
#define LVTMA_PWRSEQ_CNTL \
    LVTMAREGSHIFT(LVTMA_R500_PWRSEQ_CNTL, LVTMA_R600_PWRSEQ_CNTL)
#define LVTMA_PWRSEQ_STATE \
    LVTMAREGSHIFT(LVTMA_R500_PWRSEQ_STATE, LVTMA_R600_PWRSEQ_STATE)
#define LVTMA_LVDS_DATA_CNTL \
    LVTMAREGSHIFT(LVTMA_R500_LVDS_DATA_CNTL, LVTMA_R600_LVDS_DATA_CNTL)
#define LVTMA_MODE LVTMAREGSHIFT(LVTMA_R500_MODE, LVTMA_R600_MODE)
#define LVTMA_TRANSMITTER_ENABLE \
    LVTMAREGSHIFT(LVTMA_R500_TRANSMITTER_ENABLE, LVTMA_R600_TRANSMITTER_ENABLE)
#define LVTMA_MACRO_CONTROL \
    LVTMAREGSHIFT(LVTMA_R500_MACRO_CONTROL, LVTMA_R600_MACRO_CONTROL)
#define LVTMA_TRANSMITTER_CONTROL \
    LVTMAREGSHIFT(LVTMA_R500_TRANSMITTER_CONTROL, LVTMA_R600_TRANSMITTER_CONTROL)
#define LVTMA_REG_TEST_OUTPUT \
    LVTMAREGSHIFT(LVTMA_R500_REG_TEST_OUTPUT, LVTMA_R600_REG_TEST_OUTPUT)

/*
 *
 * Handling for LVTMA block as LVDS.
 *
 */

struct LVDSPrivate {
    Bool DualLink;
    Bool LVDS24Bit;
    Bool FPDI; /* LDI otherwise */
    CARD16 TXClockPattern;

    CARD32 MacroControl;

    /* Power timing for LVDS */
    CARD16 PowerRefDiv;
    CARD16 BlonRefDiv;
    CARD16 PowerDigToDE;
    CARD16 PowerDEToBL;
    CARD16 OffDelay;

    Bool Stored;

    CARD32 StoreControl;
    CARD32 StoreSourceSelect;
    CARD32 StoreBitDepthControl;
    CARD32 StoreDataSynchronisation;
    CARD32 StorePWRSEQRefDiv;
    CARD32 StorePWRSEQDelay1;
    CARD32 StorePWRSEQDelay2;
    CARD32 StorePWRSEQControl;
    CARD32 StorePWRSEQState;
    CARD32 StoreLVDSDataControl;
    CARD32 StoreMode;
    CARD32 StoreTxEnable;
    CARD32 StoreMacroControl;
    CARD32 StoreTXControl;
};

/*
 *
 */
static ModeStatus
LVDSModeValid(struct rhdOutput *Output, DisplayModePtr Mode)
{
    RHDFUNC(Output);

    return MODE_OK;
}

/*
 *
 */
static void
LVDSSet(struct rhdOutput *Output)
{
    struct LVDSPrivate *Private = (struct LVDSPrivate *) Output->Private;
    RHDPtr rhdPtr = RHDPTRI(Output);

    RHDFUNC(Output);

    RHDRegMask(Output, LVTMA_CNTL, 0x00000001, 0x00000001); /* enable */
    usleep(20);

    RHDRegWrite(Output, LVTMA_MODE, 0); /* set to LVDS */

    /* Select CRTC, select syncA, no stereosync */
    RHDRegMask(Output, LVTMA_SOURCE_SELECT, Output->Crtc->Id, 0x00010101);

    if (Private->LVDS24Bit) { /* 24bits */
	RHDRegMask(Output, LVTMA_LVDS_DATA_CNTL, 0x00000001, 0x00000001); /* enable 24bits */
	RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0x00100000, 0x00100000); /* dithering bit depth = 24 */

	if (Private->FPDI) /* FPDI? */
	    RHDRegMask(Output, LVTMA_LVDS_DATA_CNTL, 0x00000010, 0x00000010); /* 24 bit format: FPDI or LDI? */
	else
	    RHDRegMask(Output, LVTMA_LVDS_DATA_CNTL, 0, 0x00000010);
    } else {
	RHDRegMask(Output, LVTMA_LVDS_DATA_CNTL, 0, 0x00000001); /* disable 24bits */
	RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0, 0x00100101); /* dithering bit depth != 24 */
    }

#if 0
    if (LVDS_Info->LVDS_Misc & 0x40) { /* enable dithering? */
	if (LVDS_Info->LVDS_Misc & 0x0C) /* no idea. */
	    RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0x01000000, 0x01000000); /* grey level 4 */
	else
	    RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0, 0x01000000); /* grey level 2 */

	/* enable temporal bit depth reduction */
	RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0x00010000, 0x00010000);
    } else
	RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0, 0x00010101);
#endif
    RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0x01010100, 0x01010101);

    /* reset the temporal dithering */
    RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0x04000000, 0x04000000);
    RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0, 0x04000000);

    /* go for RGB 4:4:4 RGB/YCbCr  */
    RHDRegMask(Output, LVTMA_CNTL, 0, 0x00010000);

    if (Private->DualLink)
	RHDRegMask(Output, LVTMA_CNTL, 0x01000000, 0x01000000);
    else
	RHDRegMask(Output, LVTMA_CNTL, 0, 0x01000000);

    /* PLL and TX voltages */
    RHDRegWrite(Output, LVTMA_MACRO_CONTROL, Private->MacroControl);

    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0x00000010, 0x00000010); /* use pclk_lvtma_direct */
    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0, 0xCC000000);
    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, Private->TXClockPattern << 16, 0x03FF0000);
    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0x00000001, 0x00000001); /* enable PLL */
    usleep(20);

    /* reset transmitter */
    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0x00000002, 0x00000002);
    usleep(2);
    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0, 0x00000002);
    usleep(20);

    /* start data synchronisation */
    RHDRegMask(Output, LVTMA_DATA_SYNCHRONIZATION, 0x00000001, 0x00000001);
    RHDRegMask(Output, LVTMA_DATA_SYNCHRONIZATION, 0x00000100, 0x00000100); /* reset */
    usleep(2);
    RHDRegMask(Output, LVTMA_DATA_SYNCHRONIZATION, 0, 0x00000100);
}

/*
 *
 */
static void
LVDSPWRSEQInit(struct rhdOutput *Output)
{
    struct LVDSPrivate *Private = (struct LVDSPrivate *) Output->Private;
    RHDPtr rhdPtr = RHDPTRI(Output);

    CARD32 tmp = 0;

    tmp = Private->PowerDigToDE >> 2;
    RHDRegMask(Output, LVTMA_PWRSEQ_DELAY1, tmp, 0x000000FF);
    RHDRegMask(Output, LVTMA_PWRSEQ_DELAY1, tmp << 24, 0xFF000000);

    tmp = Private->PowerDEToBL >> 2;
    RHDRegMask(Output, LVTMA_PWRSEQ_DELAY1, tmp << 8, 0x0000FF00);
    RHDRegMask(Output, LVTMA_PWRSEQ_DELAY1, tmp << 16, 0x00FF0000);

    RHDRegWrite(Output, LVTMA_PWRSEQ_DELAY2, Private->OffDelay >> 2);
    RHDRegWrite(Output, LVTMA_PWRSEQ_REF_DIV,
		Private->PowerRefDiv | (Private->BlonRefDiv << 16));

    /* Enable power sequencer and allow it to override everything */
    RHDRegMask(Output, LVTMA_PWRSEQ_CNTL, 0x0000000D, 0x0000000D);

    /* give full control to the sequencer */
    RHDRegMask(Output, LVTMA_PWRSEQ_CNTL, 0, 0x02020200);
}

/*
 *
 */
static void
LVDSEnable(struct rhdOutput *Output)
{
    struct LVDSPrivate *Private = (struct LVDSPrivate *) Output->Private;
    RHDPtr rhdPtr = RHDPTRI(Output);
    CARD32 tmp = 0;
    int i;

    RHDFUNC(Output);

    LVDSPWRSEQInit(Output);

    /* set up the transmitter */
    RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0x0000001E, 0x0000001E);
    if (Private->LVDS24Bit) /* 24bit ? */
	RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0x00000020, 0x00000020);

    if (Private->DualLink) {
	RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0x00001E00, 0x00001E00);

	if (Private->LVDS24Bit)
	    RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0x00002000, 0x00002000);
    }

    RHDRegMask(Output, LVTMA_PWRSEQ_CNTL, 0x00000010, 0x00000010);

    for (i = 0; i <= Private->OffDelay; i++) {
	usleep(1000);

	tmp = (RHDRegRead(Output, LVTMA_PWRSEQ_STATE) >> 8) & 0x0F;
	if (tmp == 4)
	    break;
    }

    if (i == Private->OffDelay) {
	xf86DrvMsg(Output->scrnIndex, X_ERROR, "%s: failed to reach "
		   "POWERUP_DONE state after %d loops (%d)\n",
		   __func__, i, (int) tmp);
    }
}

/*
 *
 */
static void
LVDSDisable(struct rhdOutput *Output)
{
    struct LVDSPrivate *Private = (struct LVDSPrivate *) Output->Private;
    RHDPtr rhdPtr = RHDPTRI(Output);
    CARD32 tmp = 0;
    int i;

    RHDFUNC(Output);

    if (!(RHDRegRead(Output, LVTMA_PWRSEQ_CNTL) & 0x00000010))
	return;

    LVDSPWRSEQInit(Output);

    RHDRegMask(Output, LVTMA_PWRSEQ_CNTL, 0, 0x00000010);

    for (i = 0; i <= Private->OffDelay; i++) {
	usleep(1000);

	tmp = (RHDRegRead(Output, LVTMA_PWRSEQ_STATE) >> 8) & 0x0F;
	if (tmp == 9)
	    break;
    }

    if (i == Private->OffDelay) {
	xf86DrvMsg(Output->scrnIndex, X_ERROR, "%s: failed to reach "
		   "POWERDOWN_DONE state after %d loops (%d)\n",
		   __func__, i, (int) tmp);
    }

    RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0, 0x0000FFFF);
}

#if 0
/*
 *
 */
static void
LVDSShutdown(struct rhdOutput *Output)
{
    RHDFUNC(Output);

    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0x00000002, 0x00000002); /* PLL in reset */
    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0, 0x00000001); /* disable LVDS */
    RHDRegMask(Output, LVTMA_DATA_SYNCHRONIZATION, 0, 0x00000001);
    RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0x04000000, 0x04000000); /* reset temp dithering */
    RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0, 0x00111111); /* disable all dithering */
    RHDRegWrite(Output, LVTMA_CNTL, 0); /* disable */
}
#endif

/*
 *
 */
static void
LVDSPower(struct rhdOutput *Output, int Power)
{
    RHDFUNC(Output);

    switch (Power) {
    case RHD_POWER_ON:
	LVDSEnable(Output);
	break;
    case RHD_POWER_RESET:
	/*	LVDSDisable(Output);
		break;*/
    case RHD_POWER_SHUTDOWN:
    default:
	LVDSDisable(Output);
	/* LVDSShutdown(Output); */
	break;
    }
    return;
}

/*
 *
 */
static void
LVDSSave(struct rhdOutput *Output)
{
    struct LVDSPrivate *Private = (struct LVDSPrivate *) Output->Private;
    RHDPtr rhdPtr = RHDPTRI(Output);

    RHDFUNC(Output);

    Private->StoreControl = RHDRegRead(Output, LVTMA_CNTL);
    Private->StoreSourceSelect = RHDRegRead(Output,  LVTMA_SOURCE_SELECT);
    Private->StoreBitDepthControl = RHDRegRead(Output, LVTMA_BIT_DEPTH_CONTROL);
    Private->StoreDataSynchronisation = RHDRegRead(Output, LVTMA_DATA_SYNCHRONIZATION);
    Private->StorePWRSEQRefDiv = RHDRegRead(Output, LVTMA_PWRSEQ_REF_DIV);
    Private->StorePWRSEQDelay1 = RHDRegRead(Output, LVTMA_PWRSEQ_DELAY1);
    Private->StorePWRSEQDelay2 = RHDRegRead(Output, LVTMA_PWRSEQ_DELAY2);
    Private->StorePWRSEQControl = RHDRegRead(Output, LVTMA_PWRSEQ_CNTL);
    Private->StorePWRSEQState = RHDRegRead(Output, LVTMA_PWRSEQ_STATE);
    Private->StoreLVDSDataControl = RHDRegRead(Output, LVTMA_LVDS_DATA_CNTL);
    Private->StoreMode = RHDRegRead(Output, LVTMA_MODE);
    Private->StoreTxEnable = RHDRegRead(Output, LVTMA_TRANSMITTER_ENABLE);
    Private->StoreMacroControl = RHDRegRead(Output, LVTMA_MACRO_CONTROL);
    Private->StoreTXControl = RHDRegRead(Output, LVTMA_TRANSMITTER_CONTROL);

    Private->Stored = TRUE;
}

/*
 * This needs to reset things like the temporal dithering and the TX appropriately.
 * Currently it's a dumb register dump.
 */
static void
LVDSRestore(struct rhdOutput *Output)
{
    struct LVDSPrivate *Private = (struct LVDSPrivate *) Output->Private;
    RHDPtr rhdPtr = RHDPTRI(Output);

    RHDFUNC(Output);

    if (!Private->Stored) {
	xf86DrvMsg(Output->scrnIndex, X_ERROR,
		   "%s: No registers stored.\n", __func__);
	return;
    }

    RHDRegWrite(Output, LVTMA_CNTL, Private->StoreControl);
    RHDRegWrite(Output, LVTMA_SOURCE_SELECT, Private->StoreSourceSelect);
    RHDRegWrite(Output, LVTMA_BIT_DEPTH_CONTROL,  Private->StoreBitDepthControl);
    RHDRegWrite(Output, LVTMA_DATA_SYNCHRONIZATION, Private->StoreDataSynchronisation);
    RHDRegWrite(Output, LVTMA_PWRSEQ_REF_DIV, Private->StorePWRSEQRefDiv);
    RHDRegWrite(Output, LVTMA_PWRSEQ_DELAY1, Private->StorePWRSEQDelay1);
    RHDRegWrite(Output, LVTMA_PWRSEQ_DELAY2,  Private->StorePWRSEQDelay2);
    RHDRegWrite(Output, LVTMA_PWRSEQ_CNTL, Private->StorePWRSEQControl);
    RHDRegWrite(Output, LVTMA_PWRSEQ_STATE, Private->StorePWRSEQState);
    RHDRegWrite(Output, LVTMA_LVDS_DATA_CNTL, Private->StoreLVDSDataControl);
    RHDRegWrite(Output, LVTMA_MODE, Private->StoreMode);
    RHDRegWrite(Output, LVTMA_TRANSMITTER_ENABLE, Private->StoreTxEnable);
    RHDRegWrite(Output, LVTMA_MACRO_CONTROL, Private->StoreMacroControl);
    RHDRegWrite(Output, LVTMA_TRANSMITTER_CONTROL,  Private->StoreTXControl);
}

/*
 * Here we pretty much assume that ATOM has either initialised the panel already
 * or that we can find information from ATOM BIOS data tables. We know that the
 * latter assumption is false for some values, but there is no getting around
 * ATI clinging desperately to a broken concept.
 */
static struct LVDSPrivate *
LVDSInfoRetrieve(RHDPtr rhdPtr)
{
    struct LVDSPrivate *Private = xnfcalloc(sizeof(struct LVDSPrivate), 1);
    CARD32 tmp;

    /* These values are not available from atombios data tables at all. */
    Private->MacroControl = RHDRegRead(rhdPtr, LVTMA_MACRO_CONTROL);
    Private->TXClockPattern =
	(RHDRegRead(rhdPtr, LVTMA_TRANSMITTER_CONTROL) >> 16) & 0x3FF;

    /* For these values, we try to retrieve them from register space first,
       and later override with atombios data table information */
    Private->PowerDigToDE =
	(RHDRegRead(rhdPtr, LVTMA_PWRSEQ_DELAY1) & 0x000000FF) << 2;

    Private->PowerDEToBL =
	(RHDRegRead(rhdPtr, LVTMA_PWRSEQ_DELAY1) & 0x0000FF00) >> 6;

    Private->OffDelay = (RHDRegRead(rhdPtr, LVTMA_PWRSEQ_DELAY2) & 0xFF) << 2;

    tmp = RHDRegRead(rhdPtr, LVTMA_PWRSEQ_REF_DIV);
    Private->PowerRefDiv = tmp & 0x0FFF;
    Private->BlonRefDiv = (tmp >> 16) & 0x0FFF;

    Private->DualLink = (RHDRegRead(rhdPtr, LVTMA_CNTL) >> 24) & 0x00000001;
    Private->LVDS24Bit = RHDRegRead(rhdPtr, LVTMA_LVDS_DATA_CNTL) & 0x00000001;
    Private->FPDI = RHDRegRead(rhdPtr, LVTMA_LVDS_DATA_CNTL) & 0x00000001;

#ifdef ATOM_BIOS
    {
	AtomBiosArgRec data;

	if(RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			   ATOM_LVDS_SEQ_DIG_ONTO_DE, &data) == ATOM_SUCCESS)
	    Private->PowerDigToDE = data.val;

	if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			    ATOM_LVDS_SEQ_DE_TO_BL, &data) == ATOM_SUCCESS)
	    Private->PowerDEToBL = data.val;

	if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			    ATOM_LVDS_OFF_DELAY, &data) == ATOM_SUCCESS)
	    Private->OffDelay = data.val;

	if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			    ATOM_LVDS_DUALLINK, &data) == ATOM_SUCCESS)
	    Private->DualLink = data.val;

	if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			    ATOM_LVDS_24BIT, &data) == ATOM_SUCCESS)
	    Private->LVDS24Bit = data.val;

	if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
				 ATOM_LVDS_FPDI, &data) == ATOM_SUCCESS)
	    Private->FPDI = data.val;
    }
#endif

    if (Private->LVDS24Bit)
	xf86DrvMsg(rhdPtr->scrnIndex, X_PROBED,
		   "Detected a 24bit %s, %s link panel.\n",
		   Private->DualLink ? "dual" : "single",
		   Private->FPDI ? "FPDI": "LDI");
    else
	xf86DrvMsg(rhdPtr->scrnIndex, X_PROBED,
		   "Detected a 18bit %s link panel.\n",
		   Private->DualLink ? "dual" : "single");

    /* extra noise */
    RHDDebug(rhdPtr->scrnIndex, "Printing LVDS paramaters:\n");
    xf86MsgVerb(X_NONE, LOG_DEBUG, "\tMacroControl: 0x%08X\n",
		(unsigned int) Private->MacroControl);
    xf86MsgVerb(X_NONE, LOG_DEBUG, "\tTXClockPattern: 0x%04X\n",
		Private->TXClockPattern);
    xf86MsgVerb(X_NONE, LOG_DEBUG, "\tPowerDigToDE: 0x%04X\n",
		Private->PowerDigToDE);
    xf86MsgVerb(X_NONE, LOG_DEBUG, "\tPowerDEToBL: 0x%04X\n",
		Private->PowerDEToBL);
    xf86MsgVerb(X_NONE, LOG_DEBUG, "\tOffDelay: 0x%04X\n",
		Private->OffDelay);
    xf86MsgVerb(X_NONE, LOG_DEBUG, "\tPowerRefDiv: 0x%04X\n",
		Private->PowerRefDiv);
    xf86MsgVerb(X_NONE, LOG_DEBUG, "\tBlonRefDiv: 0x%04X\n",
		Private->BlonRefDiv);

    return Private;
}

/*
 *
 * Handling for LVTMA block as TMDS.
 *
 */
struct rhdTMDSBPrivate {
    Bool Stored;

    CARD32 StoreControl;
    CARD32 StoreSource;
    CARD32 StoreFormat;
    CARD32 StoreForce;
    CARD32 StoreReduction;
    CARD32 StoreDCBalancer;
    CARD32 StoreDataSynchro;
    CARD32 StoreMode;
    CARD32 StoreTXEnable;
    CARD32 StoreMacro;
    CARD32 StoreTXControl;
    CARD32 StoreTXAdjust;
    CARD32 StoreTestOutput;

    CARD32 StoreRs690Unknown;
    CARD32 StoreRv600TXAdjust;
    CARD32 StoreRv600PreEmphasis;
};

/*
 *
 */
static ModeStatus
TMDSBModeValid(struct rhdOutput *Output, DisplayModePtr Mode)
{
    RHDFUNC(Output);

    if (Mode->Clock < 25000)
	return MODE_CLOCK_LOW;

    if (Mode->Clock > 165000)
	return MODE_CLOCK_HIGH;

    return MODE_OK;
}

/*
 * This information is not provided in an atombios data table.
 */
static struct R5xxTMDSBMacro {
    CARD16 Device;
    CARD32 Macro;
} R5xxTMDSBMacro[] = {
    { 0x7104, 0x00F20616 }, /* R520  */
    { 0x7142, 0x00F2061C }, /* RV515 */
    { 0x7146, 0x00F1061D }, /* RV515 */
    { 0x7147, 0x0082041D }, /* RV505 */
    { 0x7152, 0x00F2061C }, /* RV515 */
    { 0x7183, 0x00b2050C }, /* RV530 */
    { 0x71C1, 0x0062041D }, /* RV535 */
    { 0x71C2, 0x00F1061D }, /* RV530 */
    { 0x71D2, 0x00F10610 }, /* RV530: atombios uses 0x00F1061D */
    { 0x7249, 0x00F1061D }, /* R580  */
    { 0x724B, 0x00F10610 }, /* R580: atombios uses 0x00F1061D */
    { 0x7280, 0x0042041F }, /* RV570 */
    { 0x7288, 0x0042041F }, /* RV570 */
    { 0x791E, 0x0001642F }, /* RS690 */
    { 0x791F, 0x0001642F }, /* RS690 */
    { 0x9400, 0x00020213 }, /* R600  */
    { 0x9401, 0x00020213 }, /* R600  */
    { 0x9402, 0x00020213 }, /* R600  */
    { 0x9403, 0x00020213 }, /* R600  */
    { 0x9405, 0x00020213 }, /* R600  */
    { 0x940A, 0x00020213 }, /* R600  */
    { 0x940B, 0x00020213 }, /* R600  */
    { 0x940F, 0x00020213 }, /* R600  */
    { 0, 0} /* End marker */
};

static struct RV6xxTMDSBMacro {
    CARD16 Device;
    CARD32 Macro;
    CARD32 TX;
    CARD32 PreEmphasis;
} RV6xxTMDSBMacro[] = {
    { 0x94C1, 0x01030311, 0x10001A00, 0x01801015},
    { 0x94C3, 0x01030311, 0x10001A00, 0x01801015},
    { 0x9588, 0x01030311, 0x10001C00, 0x01C01011},
    { 0x9589, 0x01030311, 0x10001C00, 0x01C01011},
    { 0, 0, 0, 0} /* End marker */
};

static void
TMDSBVoltageControl(struct rhdOutput *Output)
{
    RHDPtr rhdPtr = RHDPTRI(Output);
    int i;

    if (rhdPtr->ChipSet < RHD_RV610) { /* R5xx, RS690 and R600 */
	for (i = 0; R5xxTMDSBMacro[i].Device; i++)
	    if (R5xxTMDSBMacro[i].Device == rhdPtr->PciDeviceID) {
		RHDRegWrite(Output, LVTMA_MACRO_CONTROL, R5xxTMDSBMacro[i].Macro);
		return;
	    }

	xf86DrvMsg(Output->scrnIndex, X_ERROR, "%s: unhandled chipset: 0x%04X.\n",
		   __func__, rhdPtr->PciDeviceID);
	xf86DrvMsg(Output->scrnIndex, X_INFO, "LVTMA_MACRO_CONTROL: 0x%08X\n",
		   (unsigned int) RHDRegRead(Output, LVTMA_MACRO_CONTROL));
    } else { /* RV6x0 and up */
	for (i = 0; RV6xxTMDSBMacro[i].Device; i++)
	    if (RV6xxTMDSBMacro[i].Device == rhdPtr->PciDeviceID) {
		RHDRegWrite(Output, LVTMA_MACRO_CONTROL, RV6xxTMDSBMacro[i].Macro);
		RHDRegWrite(Output, LVTMA_TRANSMITTER_ADJUST, RV6xxTMDSBMacro[i].TX);
		RHDRegWrite(Output, LVTMA_PREEMPHASIS_CONTROL, RV6xxTMDSBMacro[i].PreEmphasis);
		return;
	    }

	xf86DrvMsg(Output->scrnIndex, X_ERROR, "%s: unhandled chipset: 0x%04X.\n",
		   __func__, rhdPtr->PciDeviceID);
	xf86DrvMsg(Output->scrnIndex, X_INFO, "LVTMA_MACRO_CONTROL: 0x%08X\n",
		   (unsigned int) RHDRegRead(Output, LVTMA_MACRO_CONTROL));
	xf86DrvMsg(Output->scrnIndex, X_INFO, "LVTMA_TRANSMITTER_ADJUST: 0x%08X\n",
		   (unsigned int) RHDRegRead(Output, LVTMA_TRANSMITTER_ADJUST));
	xf86DrvMsg(Output->scrnIndex, X_INFO, "LVTMA_PREEMPHASIS_CONTROL: 0x%08X\n",
		   (unsigned int) RHDRegRead(Output, LVTMA_PREEMPHASIS_CONTROL));
    }
}

/*
 *
 */
static void
TMDSBSet(struct rhdOutput *Output)
{
    RHDPtr rhdPtr = RHDPTRI(Output);

    RHDFUNC(Output);

    RHDRegMask(Output, LVTMA_MODE, 0x00000001, 0x00000001); /* select TMDS */
    if (rhdPtr->ChipSet < RHD_RS690) /* r5xx */
	RHDRegMask(Output, LVTMA_REG_TEST_OUTPUT, 0x00200000, 0x00200000);
    else if (rhdPtr->ChipSet == RHD_RS690)
	RHDRegWrite(Output, LVTMA_REG_TEST_OUTPUT, 0x01120000);
    else /* R600 and up */
	RHDRegMask(Output, LVTMA_REG_TEST_OUTPUT, 0x00100000, 0x00100000);

    /* Clear out some HPD events first: this should be under driver control. */
    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0, 0x0000000C);
    RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0, 0x00070000);
    RHDRegMask(Output, LVTMA_CNTL, 0, 0x00000010);

    /* Disable the transmitter */
    if (rhdPtr->ChipSet < RHD_RS690)
	RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0, 0x00001D1F);
    else
	RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0, 0x00003E3E);

    /* Disable bit reduction and reset temporal dither */
    RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0, 0x00010101);
    RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0x02000000, 0x02000000);
    usleep(2);
    RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0, 0x02000000);
    RHDRegMask(Output, LVTMA_BIT_DEPTH_CONTROL, 0, 0xF0000000); /* not documented */

    /* reset phase on vsync and use RGB */
    RHDRegMask(Output, LVTMA_CNTL, 0x00001000, 0x00011000);

    /* Select CRTC, select syncA, no stereosync */
    RHDRegMask(Output, LVTMA_SOURCE_SELECT, Output->Crtc->Id, 0x00010101);

    /* Single link, for now */
    RHDRegWrite(Output, LVTMA_COLOR_FORMAT, 0);
    RHDRegMask(Output, LVTMA_CNTL, 0, 0x01000000);

    if (rhdPtr->ChipSet > RHD_R600) /* Rv6xx: disable split mode */
	RHDRegMask(Output, LVTMA_CNTL, 0, 0x20000000);

    /* Disable force data */
    RHDRegMask(Output, LVTMA_FORCE_OUTPUT_CNTL, 0, 0x00000001);

    /* DC balancer enable */
    RHDRegMask(Output, LVTMA_DCBALANCER_CONTROL, 0x00000001, 0x00000001);

    TMDSBVoltageControl(Output);

    /* use IDCLK */
    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0, 0x00000010);
    /* coherent mode */
    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0, 0x10000000);
    /* LVTMA only: use clock selected by previous write */
    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0x20000000, 0x20000000);
    /* clear LVDS clock pattern */
    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0, 0x03FF0000);

    /* reset transmitter */
    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0x00000002, 0x00000002);
    usleep(2);
    RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0, 0x00000002);
    usleep(20);

    /* restart data synchronisation */
    RHDRegMask(Output, LVTMA_DATA_SYNCHRONIZATION, 0x00000001, 0x00000001);
    RHDRegMask(Output, LVTMA_DATA_SYNCHRONIZATION, 0x00000100, 0x00000100);
    usleep(2);
    RHDRegMask(Output, LVTMA_DATA_SYNCHRONIZATION, 0, 0x00000001);
}

/*
 *
 */
static void
TMDSBPower(struct rhdOutput *Output, int Power)
{
    RHDPtr rhdPtr = RHDPTRI(Output);

    RHDFUNC(Output);

    RHDRegMask(Output, LVTMA_MODE, 0x00000001, 0x00000001); /* select TMDS */

    switch (Power) {
    case RHD_POWER_ON:
	RHDRegMask(Output, LVTMA_CNTL, 0x00000001, 0x00000001);
	if (rhdPtr->ChipSet < RHD_RS690)
	    RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0x0000001F, 0x0000001F);
	else
	    RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0x0000003E, 0x0000003E);
	RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0x00000001, 0x00000001);
	usleep(2);
	RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0, 0x00000002);
	return;
    case RHD_POWER_RESET:
	if (rhdPtr->ChipSet < RHD_RS690)
	    RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0, 0x0000001F);
	else
	    RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0, 0x0000003E);
	return;
    case RHD_POWER_SHUTDOWN:
    default:
	RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0x00000002, 0x00000002);
	usleep(2);
	RHDRegMask(Output, LVTMA_TRANSMITTER_CONTROL, 0, 0x00000001);
	if (rhdPtr->ChipSet < RHD_RS690)
	    RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0, 0x0000001F);
	else
	    RHDRegMask(Output, LVTMA_TRANSMITTER_ENABLE, 0, 0x0000003E);
	RHDRegMask(Output, LVTMA_CNTL, 0, 0x00000001);
	return;
    }
}

/*
 *
 */
static void
TMDSBSave(struct rhdOutput *Output)
{
    struct rhdTMDSBPrivate *Private = (struct rhdTMDSBPrivate *) Output->Private;
    RHDPtr rhdPtr = RHDPTRI(Output);

    RHDFUNC(Output);

    Private->StoreControl = RHDRegRead(Output, LVTMA_CNTL);
    Private->StoreSource = RHDRegRead(Output, LVTMA_SOURCE_SELECT);
    Private->StoreFormat = RHDRegRead(Output, LVTMA_COLOR_FORMAT);
    Private->StoreForce = RHDRegRead(Output, LVTMA_FORCE_OUTPUT_CNTL);
    Private->StoreReduction = RHDRegRead(Output, LVTMA_BIT_DEPTH_CONTROL);
    Private->StoreDCBalancer = RHDRegRead(Output, LVTMA_DCBALANCER_CONTROL);

    Private->StoreDataSynchro = RHDRegRead(Output, LVTMA_DATA_SYNCHRONIZATION);
    Private->StoreMode = RHDRegRead(Output, LVTMA_MODE);
    Private->StoreTXEnable = RHDRegRead(Output, LVTMA_TRANSMITTER_ENABLE);
    Private->StoreMacro = RHDRegRead(Output, LVTMA_MACRO_CONTROL);
    Private->StoreTXControl = RHDRegRead(Output, LVTMA_TRANSMITTER_CONTROL);
    Private->StoreTestOutput = RHDRegRead(Output, LVTMA_REG_TEST_OUTPUT);

    if (rhdPtr->ChipSet > RHD_R600) { /* Rv6x0 */
       Private->StoreRv600TXAdjust = RHDRegRead(Output, LVTMA_TRANSMITTER_ADJUST);
       Private->StoreRv600PreEmphasis = RHDRegRead(Output, LVTMA_PREEMPHASIS_CONTROL);
    }

    Private->Stored = TRUE;
}

/*
 *
 */
static void
TMDSBRestore(struct rhdOutput *Output)
{
    struct rhdTMDSBPrivate *Private = (struct rhdTMDSBPrivate *) Output->Private;
    RHDPtr rhdPtr = RHDPTRI(Output);

    RHDFUNC(Output);

    if (!Private->Stored) {
	xf86DrvMsg(Output->scrnIndex, X_ERROR,
		   "%s: No registers stored.\n", __func__);
	return;
    }

    RHDRegWrite(Output, LVTMA_CNTL, Private->StoreControl);
    RHDRegWrite(Output, LVTMA_SOURCE_SELECT, Private->StoreSource);
    RHDRegWrite(Output, LVTMA_COLOR_FORMAT, Private->StoreFormat);
    RHDRegWrite(Output, LVTMA_FORCE_OUTPUT_CNTL, Private->StoreForce);
    RHDRegWrite(Output, LVTMA_BIT_DEPTH_CONTROL, Private->StoreReduction);
    RHDRegWrite(Output, LVTMA_DCBALANCER_CONTROL, Private->StoreDCBalancer);

    RHDRegWrite(Output, LVTMA_DATA_SYNCHRONIZATION, Private->StoreDataSynchro);
    RHDRegWrite(Output, LVTMA_MODE, Private->StoreMode);
    RHDRegWrite(Output, LVTMA_TRANSMITTER_ENABLE, Private->StoreTXEnable);
    RHDRegWrite(Output, LVTMA_MACRO_CONTROL, Private->StoreMacro);
    RHDRegWrite(Output, LVTMA_TRANSMITTER_CONTROL, Private->StoreTXControl);
    RHDRegWrite(Output, LVTMA_REG_TEST_OUTPUT, Private->StoreTestOutput);

    if (rhdPtr->ChipSet > RHD_R600) { /* Rv6x0 */
	RHDRegWrite(Output, LVTMA_TRANSMITTER_ADJUST, Private->StoreRv600TXAdjust);
	RHDRegWrite(Output, LVTMA_PREEMPHASIS_CONTROL, Private->StoreRv600PreEmphasis);
    }
}


/*
 *
 */
static void
LVTMADestroy(struct rhdOutput *Output)
{
    RHDFUNC(Output);

    if (!Output->Private)
	return;

    xfree(Output->Private);
    Output->Private = NULL;
}

/*
 *
 */
struct rhdOutput *
RHDLVTMAInit(RHDPtr rhdPtr, CARD8 Type)
{
    struct rhdOutput *Output;

    RHDFUNC(rhdPtr);

    /* Stop weird connector types */
    if ((Type != RHD_CONNECTOR_PANEL) && (Type != RHD_CONNECTOR_DVI)) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: unhandled connector type:"
		   " %d\n", __func__, Type);
	return NULL;
    }

    Output = xnfcalloc(sizeof(struct rhdOutput), 1);

    Output->scrnIndex = rhdPtr->scrnIndex;
    Output->Id = RHD_OUTPUT_LVTMA;

    Output->Sense = NULL; /* not implemented in hw */
    Output->Destroy = LVTMADestroy;

    if (Type == RHD_CONNECTOR_PANEL) {
	Output->Name = "LVDS";

	Output->ModeValid = LVDSModeValid;
	Output->Mode = LVDSSet;
	Output->Power = LVDSPower;
	Output->Save = LVDSSave;
	Output->Restore = LVDSRestore;

	Output->Private = LVDSInfoRetrieve(rhdPtr);
    } else {
	Output->Name = "TMDS B";

	Output->ModeValid = TMDSBModeValid;
	Output->Mode = TMDSBSet;
	Output->Power = TMDSBPower;
	Output->Save = TMDSBSave;
	Output->Restore = TMDSBRestore;

	Output->Private = xnfcalloc(sizeof(struct rhdTMDSBPrivate), 1);
    }

    return Output;
}
