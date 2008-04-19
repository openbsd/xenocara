/*
 * Copyright 2007, 2008  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2007, 2008  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007, 2008  Egbert Eich   <eich@novell.com>
 * Copyright 2007, 2008  Advanced Micro Devices, Inc.
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
#if HAVE_XF86_ANSIC_H
# include "xf86_ansic.h"
#else
# include <unistd.h>
#endif

#include "rhd.h"
#include "rhd_crtc.h"
#include "rhd_connector.h"
#include "rhd_output.h"
#include "rhd_regs.h"
#include "rhd_card.h"
#ifdef ATOM_BIOS
#include "rhd_atombios.h"
#endif

#define FMT2_OFFSET 0x800
#define DIG1_OFFSET 0x000
#define DIG2_OFFSET 0x400

/*
 * Transmitter
 */
struct transmitter {
    enum rhdSensedOutput (*Sense) (struct rhdOutput *Output,
				   enum rhdConnectorType Type);
    ModeStatus (*ModeValid) (struct rhdOutput *Output, DisplayModePtr Mode);
    void (*Mode) (struct rhdOutput *Output, struct rhdCrtc *Crtc, DisplayModePtr Mode);
    void (*Power) (struct rhdOutput *Output, int Power);
    void (*Save) (struct rhdOutput *Output);
    void (*Restore) (struct rhdOutput *Output);
    void (*Destroy) (struct rhdOutput *Output);
    void *Private;
};

/*
 * Encoder
 */
struct encoder {
    ModeStatus (*ModeValid) (struct rhdOutput *Output, DisplayModePtr Mode);
    void (*Mode) (struct rhdOutput *Output, struct rhdCrtc *Crtc, DisplayModePtr Mode);
    void (*Power) (struct rhdOutput *Output, int Power);
    void (*Save) (struct rhdOutput *Output);
    void (*Restore) (struct rhdOutput *Output);
    void (*Destroy) (struct rhdOutput *Output);
    void *Private;
};

/*
 *
 */
enum encoderMode {
    DISPLAYPORT = 0,
    LVDS = 1,
    TMDS_DVI = 2,
    TMDS_HDMI = 3,
    SDVO = 4
};

enum encoderID {
    ENCODER_DIG1,
    ENCODER_DIG2
};

struct DIGPrivate
{
    struct encoder Encoder;
    struct transmitter Transmitter;
    enum encoderID EncoderID;
    enum encoderMode EncoderMode;
    Bool Coherent;
    Bool RunDualLink;
    /* LVDS */
    Bool FPDI;
    struct rhdFMTDither FMTDither;
};

/*
 * LVTMA Transmitter
 */

struct LVTMATransmitterPrivate
{
    Bool Stored;

    CARD32 StoredTransmitterControl;
    CARD32 StoredTransmitterAdjust;
    CARD32 StoredPreemphasisControl;
    CARD32 StoredMacroControl;
    CARD32 StoredLVTMADataSynchronization;
    CARD32 StoredTransmiterEnable;
};

/*
 *
 */
static ModeStatus
LVTMATransmitterModeValid(struct rhdOutput *Output, DisplayModePtr Mode)
{
    RHDFUNC(Output);

    if (Mode->Flags & V_INTERLACE)
        return MODE_NO_INTERLACE;

    if (Output->Connector->Type == RHD_CONNECTOR_DVI_SINGLE
	&& Mode->SynthClock > 165000)
	return MODE_CLOCK_HIGH;

    return MODE_OK;
}

/*
 *
 */
static void
LVTMATransmitterSet(struct rhdOutput *Output, struct rhdCrtc *Crtc, DisplayModePtr Mode)
{
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;
    CARD32 value = 0;
    AtomBiosArgRec data;
    RHDPtr rhdPtr = RHDPTRI(Output);

    RHDFUNC(Output);

     /* ?? */
    RHDRegMask(Output, RV620_LVTMA_TRANSMITTER_CONTROL,
	       RV62_LVTMA_USE_CLK_DATA, RV62_LVTMA_USE_CLK_DATA);
    /* set coherent / not coherent mode; whatever that is */
    RHDRegMask(Output, RV620_LVTMA_TRANSMITTER_CONTROL,
	       Private->Coherent ? 0 : RV62_LVTMA_BYPASS_PLL, RV62_LVTMA_BYPASS_PLL);

#ifdef ATOM_BIOS
    RHDDebug(Output->scrnIndex, "%s: SynthClock: %i Hex: %x EncoderMode: %x\n",__func__,
	     (Mode->SynthClock),(Mode->SynthClock / 10), Private->EncoderMode);

    /* Set up magic value that's used for list lookup */
    value = ((Mode->SynthClock / 10 / ((Private->RunDualLink) ? 2 : 1)) & 0xffff)
	| (Private->EncoderMode << 16)
	| ((Private->Coherent ? 0x2 : 0) << 24);

    RHDDebug(Output->scrnIndex, "%s: GetConditionalGoldenSettings for: %x\n", __func__, value);

    /* Get data from DIG2TransmitterControl table */
    data.val = 0x4d;
    if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS, ATOMBIOS_GET_CODE_DATA_TABLE,
			&data) == ATOM_SUCCESS) {
	AtomBiosArgRec data1;
	CARD32 *d_p;

	data1.GoldenSettings.BIOSPtr = data.CommandDataTable.loc;
	data1.GoldenSettings.End = data1.GoldenSettings.BIOSPtr + data.CommandDataTable.size;
	data1.GoldenSettings.value = value;

	/* now find pointer */
	if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			    ATOM_GET_CONDITIONAL_GOLDEN_SETTINGS, &data1) == ATOM_SUCCESS) {
	    d_p = (CARD32*)data1.GoldenSettings.BIOSPtr;

	    RHDDebug(Output->scrnIndex, "TransmitterAdjust: 0x%8.8x\n",d_p[0]);
	    RHDRegWrite(Output, RV620_LVTMA_TRANSMITTER_ADJUST, d_p[0]);

	    RHDDebug(Output->scrnIndex, "PreemphasisControl: 0x%8.8x\n",d_p[1]);
	    RHDRegWrite(Output, RV620_LVTMA_PREEMPHASIS_CONTROL, d_p[1]);

	    RHDDebug(Output->scrnIndex, "MacroControl: 0x%8.8x\n",d_p[2]);
	    RHDRegWrite(Output, RV620_LVTMA_MACRO_CONTROL, d_p[2]);
	} else
	    xf86DrvMsg(Output->scrnIndex, X_WARNING, "%s: cannot get golden settings\n",__func__);
    } else
#endif
    {
	xf86DrvMsg(Output->scrnIndex, X_WARNING, "%s: No AtomBIOS supplied "
		   "electrical parameters available\n", __func__);
    }
    /* use differential post divider input */
    RHDRegMask(Output, RV620_LVTMA_TRANSMITTER_CONTROL,
	       RV62_LVTMA_IDSCKSEL, RV62_LVTMA_IDSCKSEL);
}

/*
 *
 */
static void
LVTMATransmitterPower(struct rhdOutput *Output, int Power)
{
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;

    RHDFUNC(Output);

    switch (Power) {
	case RHD_POWER_ON:
	    /* enable PLL */
	    RHDRegMask(Output, RV620_LVTMA_TRANSMITTER_CONTROL,
		       RV62_LVTMA_PLL_ENABLE, RV62_LVTMA_PLL_ENABLE);
	    usleep(14);
	    /* PLL reset on */
	    RHDRegMask(Output, RV620_LVTMA_TRANSMITTER_CONTROL,
		       RV62_LVTMA_PLL_RESET, RV62_LVTMA_PLL_RESET);
	    usleep(10);
	    /* PLL reset off */
	    RHDRegMask(Output, RV620_LVTMA_TRANSMITTER_CONTROL,
		       0, RV62_LVTMA_PLL_RESET);
	    usleep(1000);
	    /* start data synchronization */
	    RHDRegMask(Output, RV620_LVTMA_DATA_SYNCHRONIZATION,
		       RV62_LVTMA_PFREQCHG, RV62_LVTMA_PFREQCHG);
	    usleep(1);
	    /* restart write address logic */
	    RHDRegMask(Output, RV620_LVTMA_DATA_SYNCHRONIZATION,
		       RV62_LVTMA_DSYNSEL, RV62_LVTMA_DSYNSEL);
	    /* ?? */
	    RHDRegMask(Output, RV620_LVTMA_TRANSMITTER_CONTROL,
		       RV62_LVTMA_MODE, RV62_LVTMA_MODE);
	    /* enable lower link */
	    RHDRegMask(Output, RV620_LVTMA_TRANSMITTER_ENABLE,
		       RV62_LVTMA_LNKL,
		       RV62_LVTMA_LNK_ALL);
	    if (Private->RunDualLink) {
		usleep (28);
		/* enable upper link */
		RHDRegMask(Output, RV620_LVTMA_TRANSMITTER_ENABLE,
			   RV62_LVTMA_LNKU,
			   RV62_LVTMA_LNKU);
	    }
	    return;
	case RHD_POWER_RESET:
	    /* disable all links */
	    RHDRegMask(Output, RV620_LVTMA_TRANSMITTER_ENABLE,
		       0, RV62_LVTMA_LNK_ALL);
	    return;
	case RHD_POWER_SHUTDOWN:
	default:
	    /* disable transmitter */
	    RHDRegMask(Output, RV620_LVTMA_TRANSMITTER_ENABLE,
		       0, RV62_LVTMA_LNK_ALL);
	    /* PLL reset */
	    RHDRegMask(Output, RV620_LVTMA_TRANSMITTER_CONTROL,
		       RV62_LVTMA_PLL_RESET, RV62_LVTMA_PLL_RESET);
	    usleep(10);
	    /* end PLL reset */
	    RHDRegMask(Output, RV620_LVTMA_TRANSMITTER_CONTROL,
		       0, RV62_LVTMA_PLL_RESET);
	    /* disable data synchronization */
	    RHDRegMask(Output, RV620_LVTMA_DATA_SYNCHRONIZATION,
		       0, RV62_LVTMA_DSYNSEL);
	    /* reset macro control */
	    RHDRegWrite(Output, RV620_LVTMA_TRANSMITTER_ADJUST, 0);
	    return;
    }
}

/*
 *
 */
static void
LVTMATransmitterSave(struct rhdOutput *Output)
{
    struct DIGPrivate *digPrivate = (struct DIGPrivate *)Output->Private;
    struct LVTMATransmitterPrivate *Private = (struct LVTMATransmitterPrivate*)digPrivate->Transmitter.Private;

    RHDFUNC(Output);

    Private->StoredTransmitterControl       = RHDRegRead(Output, RV620_LVTMA_TRANSMITTER_CONTROL);
    Private->StoredTransmitterAdjust        = RHDRegRead(Output, RV620_LVTMA_TRANSMITTER_ADJUST);
    Private->StoredPreemphasisControl       = RHDRegRead(Output, RV620_LVTMA_PREEMPHASIS_CONTROL);
    Private->StoredMacroControl             = RHDRegRead(Output, RV620_LVTMA_MACRO_CONTROL);
    Private->StoredLVTMADataSynchronization = RHDRegRead(Output, RV620_LVTMA_DATA_SYNCHRONIZATION);
    Private->StoredTransmiterEnable         = RHDRegRead(Output, RV620_LVTMA_TRANSMITTER_ENABLE);

    Private->Stored = TRUE;
}

/*
 *
 */
static void
LVTMATransmitterRestore(struct rhdOutput *Output)
{
    struct DIGPrivate *digPrivate = (struct DIGPrivate *)Output->Private;
    struct LVTMATransmitterPrivate *Private = (struct LVTMATransmitterPrivate*)digPrivate->Transmitter.Private;

    RHDFUNC(Output);

    if (!Private->Stored) {
	xf86DrvMsg(Output->scrnIndex, X_ERROR,
		   "%s: No registers stored.\n", __func__);
	return;
    }

    /* write control values back */
    RHDRegWrite(Output, RV620_LVTMA_TRANSMITTER_CONTROL,Private->StoredTransmitterControl);
    usleep (14);
    /* reset PLL */
    RHDRegWrite(Output, RV620_LVTMA_TRANSMITTER_CONTROL,Private->StoredTransmitterControl
		| RV62_LVTMA_PLL_RESET);
    usleep (10);
    /* unreset PLL */
    RHDRegWrite(Output, RV620_LVTMA_TRANSMITTER_CONTROL,Private->StoredTransmitterControl);
    usleep(1000);
    RHDRegWrite(Output, RV620_LVTMA_TRANSMITTER_ADJUST, Private->StoredTransmitterAdjust);
    RHDRegWrite(Output, RV620_LVTMA_PREEMPHASIS_CONTROL, Private->StoredPreemphasisControl);
    RHDRegWrite(Output, RV620_LVTMA_MACRO_CONTROL, Private->StoredMacroControl);
    /* start data synchronization */
    RHDRegWrite(Output, RV620_LVTMA_DATA_SYNCHRONIZATION, (Private->StoredLVTMADataSynchronization
							   & ~(CARD32)RV62_LVTMA_DSYNSEL)
		| RV62_LVTMA_PFREQCHG);
    usleep (1);
    RHDRegWrite(Output, RV620_LVTMA_DATA_SYNCHRONIZATION, Private->StoredLVTMADataSynchronization);
    usleep(10);
    RHDRegWrite(Output, RV620_LVTMA_DATA_SYNCHRONIZATION, Private->StoredLVTMADataSynchronization);
    RHDRegWrite(Output, RV620_LVTMA_TRANSMITTER_ENABLE, Private->StoredTransmiterEnable);
}

/*
 *
 */
static void
LVTMATransmitterDestroy(struct rhdOutput *Output)
{
    struct DIGPrivate *digPrivate = (struct DIGPrivate *)Output->Private;

    RHDFUNC(Output);

    if (!digPrivate)
	return;

    xfree(digPrivate->Transmitter.Private);
}

#ifdef ATOM_BIOS

struct ATOMTransmitterPrivate
{
    struct atomTransmitterConfig atomTransmitterConfig;
    enum atomTransmitter atomTransmitterID;
};

/*
 *
 */
static ModeStatus
ATOMTransmitterModeValid(struct rhdOutput *Output, DisplayModePtr Mode)
{

    RHDFUNC(Output);

    if (Output->Connector->Type == RHD_CONNECTOR_DVI_SINGLE
	&& Mode->SynthClock > 165000)
	return MODE_CLOCK_HIGH;

    return MODE_OK;
}

/*
 *
 */
static void
ATOMTransmitterSet(struct rhdOutput *Output, struct rhdCrtc *Crtc, DisplayModePtr Mode)
{
    RHDPtr rhdPtr = RHDPTRI(Output);
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;
    struct ATOMTransmitterPrivate *transPrivate
	= (struct ATOMTransmitterPrivate*) Private->Transmitter.Private;
    struct atomTransmitterConfig *atc = &transPrivate->atomTransmitterConfig;

    RHDFUNC(Output);

    if (Private->RunDualLink)
	atc->mode = atomDVI_2Link;
    else
	atc->mode = atomDVI_1Link;
    atc->pixelClock = Mode->SynthClock;

    rhdAtomDigTransmitterControl(rhdPtr->atomBIOS, transPrivate->atomTransmitterID,
				 atomTransSetup, atc);
}

/*
 *
 */
static void
ATOMTransmitterPower(struct rhdOutput *Output, int Power)
{
    RHDPtr rhdPtr = RHDPTRI(Output);
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;
    struct ATOMTransmitterPrivate *transPrivate
	= (struct ATOMTransmitterPrivate*) Private->Transmitter.Private;
    struct atomTransmitterConfig *atc = &transPrivate->atomTransmitterConfig;

    if (Private->RunDualLink)
	atc->mode = atomDVI_2Link;
    else
	atc->mode = atomDVI_1Link;

    RHDFUNC(Output);

    switch (Power) {
	case RHD_POWER_ON:
	    rhdAtomDigTransmitterControl(rhdPtr->atomBIOS, transPrivate->atomTransmitterID,
					 atomTransEnable, atc);
	    rhdAtomDigTransmitterControl(rhdPtr->atomBIOS, transPrivate->atomTransmitterID,
					 atomTransEnableOutput, atc);
	    break;
	case RHD_POWER_RESET:
	    rhdAtomDigTransmitterControl(rhdPtr->atomBIOS, transPrivate->atomTransmitterID,
					 atomTransDisableOutput, atc);
	    break;
	case RHD_POWER_SHUTDOWN:
	    if (!Output->Connector || Output->Connector->Type == RHD_CONNECTOR_DVI)
		atc->mode = atomDVI_2Link;

	    rhdAtomDigTransmitterControl(rhdPtr->atomBIOS, transPrivate->atomTransmitterID,
					 atomTransDisableOutput, atc);
	    rhdAtomDigTransmitterControl(rhdPtr->atomBIOS, transPrivate->atomTransmitterID,
					 atomTransDisable, atc);
	    break;
    }
}

/*
 *
 */
static void
ATOMTransmitterSave(struct rhdOutput *Output)
{
    RHDFUNC(Output);
}

/*
 *
 */
static void
ATOMTransmitterRestore(struct rhdOutput *Output)
{
    RHDFUNC(Output);
}

/*
 *
 */
static void
ATOMTransmitterDestroy(struct rhdOutput *Output)
{
    struct DIGPrivate *digPrivate = (struct DIGPrivate *)Output->Private;

    RHDFUNC(Output);

    if (!digPrivate)
	return;

    xfree(digPrivate->Transmitter.Private);
}

#endif

/*
 *  Encoder
 */

struct DIGEncoder
{
    Bool Stored;

    CARD32 StoredDIGClockPattern;
    CARD32 StoredLVDSDataCntl;
    CARD32 StoredTMDSPixelEncoding;
    CARD32 StoredTMDSCntl;
    CARD32 StoredDIGCntl;
    CARD32 StoredDIGMisc1;
    CARD32 StoredDIGMisc2;
    CARD32 StoredDIGMisc3;
    CARD32 StoredDCCGPclkDigCntl;
    CARD32 StoredDCCGSymclkCntl;
    CARD32 StoredDCIOLinkSteerCntl;
};

/*
 *
 */
static ModeStatus
EncoderModeValid(struct rhdOutput *Output, DisplayModePtr Mode)
{
    RHDFUNC(Output);

    return MODE_OK;
}

/*
 *
 */
static void
LVDSEncoder(struct rhdOutput *Output)
{
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;
    CARD32 off = (Private->EncoderID == ENCODER_DIG2) ? DIG2_OFFSET : DIG1_OFFSET;

    RHDFUNC(Output);

    /* Clock pattern ? */
    RHDRegMask(Output, off + RV620_DIG1_CLOCK_PATTERN, 0x0063, 0xFFFF);
    /* set panel type: 18/24 bit mode */
    RHDRegMask(Output, off + RV620_LVDS1_DATA_CNTL,
	       (Private->FMTDither.LVDS24Bit ? RV62_LVDS_24BIT_ENABLE : 0)
	       | (Private->FPDI ? RV62_LVDS_24BIT_FORMAT : 0),
	       RV62_LVDS_24BIT_ENABLE | RV62_LVDS_24BIT_FORMAT);

    Output->Crtc->FMTModeSet(Output->Crtc, &Private->FMTDither);
}

/*
 *
 */
static void
TMDSEncoder(struct rhdOutput *Output)
{
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;
    CARD32 off = (Private->EncoderID == ENCODER_DIG2) ? DIG2_OFFSET : DIG1_OFFSET;

    RHDFUNC(Output);

    /* clock pattern ? */
    RHDRegMask(Output, off + RV620_DIG1_CLOCK_PATTERN, 0x001F, 0xFFFF);
    /* color format RGB - normal color format 24bpp, Twin-Single 30bpp or Dual 48bpp*/
    RHDRegMask(Output, off + RV620_TMDS1_CNTL, 0x0,
	       RV62_TMDS_PIXEL_ENCODING | RV62_TMDS_COLOR_FORMAT);
    /* no dithering */
    Output->Crtc->FMTModeSet(Output->Crtc, NULL);
}

/*
 *
 */
static void
EncoderSet(struct rhdOutput *Output, struct rhdCrtc *Crtc, DisplayModePtr Mode)
{
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;
    CARD32 off = (Private->EncoderID == ENCODER_DIG2) ? DIG2_OFFSET : DIG1_OFFSET;

    RHDFUNC(Output);
    if (Output->Id == RHD_OUTPUT_UNIPHYA) {
	/* select LinkA ?? */
	RHDRegMask(Output, RV620_DCIO_LINK_STEER_CNTL, 0,
		   ((Private->EncoderID == ENCODER_DIG2)
		    ? RV62_LINK_STEER_SWAP
		    : 0)); /* swap if DIG2 */
	if (!Private->RunDualLink) {
	    RHDRegMask(Output, off + RV620_DIG1_CNTL, 0, RV62_DIG_SWAP |  RV62_DIG_DUAL_LINK_ENABLE);
	} else {
	    RHDRegMask(Output, off + RV620_DIG1_CNTL,
		       ((Private->EncoderID == ENCODER_DIG2)
			?  RV62_DIG_SWAP
			: 0) | RV62_DIG_DUAL_LINK_ENABLE,
		       RV62_DIG_SWAP | RV62_DIG_DUAL_LINK_ENABLE );
	}
    } else if (Output->Id == RHD_OUTPUT_UNIPHYB) {
	RHDRegMask(Output, off + RV620_DIG1_CNTL, 0, RV62_DIG_SWAP | RV62_DIG_DUAL_LINK_ENABLE);
	/* select LinkB ?? */
	RHDRegMask(Output, RV620_DCIO_LINK_STEER_CNTL,
		   ((Private->EncoderID == ENCODER_DIG2)
		    ? 0
		    : RV62_LINK_STEER_SWAP), RV62_LINK_STEER_SWAP);
    }

    if (Private->EncoderMode == LVDS)
	LVDSEncoder(Output);
    else if (Private->EncoderMode == DISPLAYPORT)
	RhdAssertFailed("No displayport support yet!",__FILE__, __LINE__, __func__);  /* bugger ! */
    else
	TMDSEncoder(Output);

    /* Start DIG, set links, disable stereo sync, select FMT source */
    RHDRegMask(Output, off + RV620_DIG1_CNTL,
	       (Private->EncoderMode & 0x7) << 8
	       | RV62_DIG_START
	       | (Private->RunDualLink ? RV62_DIG_DUAL_LINK_ENABLE : 0)
	       | Output->Crtc->Id,
	       RV62_DIG_MODE
	       | RV62_DIG_START
	       | RV62_DIG_DUAL_LINK_ENABLE
	       | RV62_DIG_STEREOSYNC_SELECT
	       | RV62_DIG_SOURCE_SELECT);
}

/*
 *
 */
static void
EncoderPower(struct rhdOutput *Output, int Power)
{
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;
    CARD32 off = (Private->EncoderID == ENCODER_DIG2) ? DIG2_OFFSET : DIG1_OFFSET;

    RHDFUNC(Output);
    /* disable DP ?*/
    RHDRegMask(Output, RV620_DCCG_SYMCLK_CNTL, 0x0,
	       0x3 << ((Private->EncoderID == ENCODER_DIG2)
		       ? RV62_SYMCLKB_SRC_SHIFT
		       : RV62_SYMCLKA_SRC_SHIFT));

    switch (Power) {
	case RHD_POWER_ON:
	    /* enable DIG */
	    RHDRegMask(Output, off + RV620_DIG1_CNTL, 0x10, 0x10);
	    RHDRegMask(Output, (Private->EncoderID == ENCODER_DIG2)
		       ? RV620_DCCG_PCLK_DIGB_CNTL
		       : RV620_DCCG_PCLK_DIGA_CNTL,
		       RV62_PCLK_DIGA_ON, RV62_PCLK_DIGA_ON); /* @@@ */
	    return;
	case RHD_POWER_RESET:
	case RHD_POWER_SHUTDOWN:
	default:
	    /* disable DIG */
	    RHDRegMask(Output, off + RV620_DIG1_CNTL, 0x0, 0x1010);
	    RHDRegMask(Output, (Private->EncoderID == ENCODER_DIG2)
		       ? RV620_DCCG_PCLK_DIGB_CNTL
		       : RV620_DCCG_PCLK_DIGA_CNTL,
		       RV62_PCLK_DIGA_ON, RV62_PCLK_DIGA_ON); /* @@@ */
	    return;
    }
}

/*
 *
 */
static void
EncoderSave(struct rhdOutput *Output)
{
    struct DIGPrivate *digPrivate = (struct DIGPrivate *)Output->Private;
    struct DIGEncoder *Private = (struct DIGEncoder *)(digPrivate->Encoder.Private);
    CARD32 off = (digPrivate->EncoderID == ENCODER_DIG2) ? DIG2_OFFSET : DIG1_OFFSET;

    RHDFUNC(Output);

    Private->StoredDIGClockPattern = RHDRegRead(Output, off + RV620_DIG1_CLOCK_PATTERN);
    Private->StoredLVDSDataCntl    = RHDRegRead(Output, off + RV620_LVDS1_DATA_CNTL);
    Private->StoredDIGCntl         = RHDRegRead(Output, off + RV620_DIG1_CNTL);
    Private->StoredTMDSCntl        = RHDRegRead(Output, off + RV620_TMDS1_CNTL);
    Private->StoredDCIOLinkSteerCntl = RHDRegRead(Output, RV620_DCIO_LINK_STEER_CNTL);
    Private->StoredDCCGPclkDigCntl    = RHDRegRead(Output,
						   (digPrivate->EncoderID
						    == ENCODER_DIG2)
						   ? RV620_DCCG_PCLK_DIGB_CNTL
						   : RV620_DCCG_PCLK_DIGA_CNTL);
    Private->StoredDCCGSymclkCntl     = RHDRegRead(Output, RV620_DCCG_SYMCLK_CNTL);

    Private->Stored = TRUE;
}

/*
 *
 */
static void
EncoderRestore(struct rhdOutput *Output)
{
    struct DIGPrivate *digPrivate = (struct DIGPrivate *)Output->Private;
    struct DIGEncoder *Private = (struct DIGEncoder *)(digPrivate->Encoder.Private);
    CARD32 off = (digPrivate->EncoderID == ENCODER_DIG2) ? DIG2_OFFSET : DIG1_OFFSET;

    RHDFUNC(Output);

    if (!Private->Stored) {
	xf86DrvMsg(Output->scrnIndex, X_ERROR,
		   "%s: No registers stored.\n", __func__);
	return;
    }

    /* reprogram all values but don't start the encoder, yet */
    RHDRegWrite(Output, off + RV620_DIG1_CNTL, Private->StoredDIGCntl & ~(CARD32)RV62_DIG_START);
    RHDRegWrite(Output, RV620_DCIO_LINK_STEER_CNTL, Private->StoredDCIOLinkSteerCntl);
    RHDRegWrite(Output, off + RV620_DIG1_CLOCK_PATTERN, Private->StoredDIGClockPattern);
    RHDRegWrite(Output, off + RV620_LVDS1_DATA_CNTL, Private->StoredLVDSDataCntl);
    RHDRegWrite(Output, off + RV620_TMDS1_CNTL, Private->StoredTMDSCntl);
    RHDRegWrite(Output, (digPrivate->EncoderID == ENCODER_DIG2)
		? RV620_DCCG_PCLK_DIGB_CNTL
		: RV620_DCCG_PCLK_DIGA_CNTL,
		Private->StoredDCCGPclkDigCntl);
    /* now enable the encoder */
    RHDRegWrite(Output, off + RV620_DIG1_CNTL, Private->StoredDIGCntl);
    RHDRegWrite(Output, RV620_DCCG_SYMCLK_CNTL, Private->StoredDCCGSymclkCntl);
}

/*
 *
 */
static void
EncoderDestroy(struct rhdOutput *Output)
{
    struct DIGPrivate *digPrivate = (struct DIGPrivate *)Output->Private;

    RHDFUNC(Output);

    if (!digPrivate || !digPrivate->Encoder.Private)
	return;

    xfree(digPrivate->Encoder.Private);
}

/*
 * Housekeeping
 */
void
GetLVDSInfo(RHDPtr rhdPtr, struct DIGPrivate *Private)
{
    CARD32 off = (Private->EncoderID == ENCODER_DIG2) ? DIG2_OFFSET : DIG1_OFFSET;

    RHDFUNC(rhdPtr);

    Private->FPDI = ((RHDRegRead(rhdPtr, off + RV620_LVDS1_DATA_CNTL)
				 & RV62_LVDS_24BIT_FORMAT) != 0);
    Private->RunDualLink = ((RHDRegRead(rhdPtr, off + RV620_DIG1_CNTL)
				 & RV62_DIG_DUAL_LINK_ENABLE) != 0);
    Private->FMTDither.LVDS24Bit = ((RHDRegRead(rhdPtr, off  + RV620_LVDS1_DATA_CNTL)
			   & RV62_LVDS_24BIT_ENABLE) != 0);
    /* This is really ugly! */
    {
	CARD32 fmt_offset;
	CARD32 tmp;

	tmp = RHDRegRead(rhdPtr, off + RV620_DIG1_CNTL);
	fmt_offset = (tmp & RV62_DIG_SOURCE_SELECT) ? FMT2_OFFSET :0;
	tmp = RHDRegRead(rhdPtr, fmt_offset + RV620_FMT1_BIT_DEPTH_CONTROL);
	Private->FMTDither.LVDSSpatialDither = ((tmp & 0x100) != 0);
	Private->FMTDither.LVDSGreyLevel = ((tmp & 0x10000) != 0);
	Private->FMTDither.LVDSTemporalDither
	    = (Private->FMTDither.LVDSGreyLevel > 0) || ((tmp & 0x1000000) != 0);
    }

#ifdef ATOM_BIOS
    {
	AtomBiosArgRec data;

	if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
				 ATOM_LVDS_FPDI, &data) == ATOM_SUCCESS)
	    Private->FPDI = data.val;

	if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			    ATOM_LVDS_DUALLINK, &data) == ATOM_SUCCESS)
	    Private->RunDualLink = data.val;

	if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			    ATOM_LVDS_24BIT, &data) == ATOM_SUCCESS)
	    Private->FMTDither.LVDS24Bit = data.val;

	if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			    ATOM_LVDS_SPATIAL_DITHER, &data) == ATOM_SUCCESS)
	    Private->FMTDither.LVDSSpatialDither = data.val;

	if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			    ATOM_LVDS_TEMPORAL_DITHER, &data) == ATOM_SUCCESS)
	    Private->FMTDither.LVDSTemporalDither = data.val;

	if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			    ATOM_LVDS_GREYLVL, &data) == ATOM_SUCCESS)
	    Private->FMTDither.LVDSGreyLevel = data.val;
    }
#endif

}

/*
 * Infrastructure
 */

static ModeStatus
DigModeValid(struct rhdOutput *Output, DisplayModePtr Mode)
{
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;
    struct transmitter *Transmitter = &Private->Transmitter;
    struct encoder *Encoder = &Private->Encoder;
    ModeStatus Status;

    RHDFUNC(Output);

    if ((Status = Transmitter->ModeValid(Output, Mode)) == MODE_OK)
	return ((Encoder->ModeValid(Output, Mode)));
    else
	return Status;
}

/*
 *
 */
static void
DigPower(struct rhdOutput *Output, int Power)
{
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;
    struct transmitter *Transmitter = &Private->Transmitter;
    struct encoder *Encoder = &Private->Encoder;

    RHDFUNC(Output);

    switch (Power) {
	case RHD_POWER_ON:
	    Encoder->Power(Output, Power);
	    Transmitter->Power(Output, Power);
	    return;
	case RHD_POWER_RESET:
	    Transmitter->Power(Output, Power);
	    Encoder->Power(Output, Power);
	    return;
	case RHD_POWER_SHUTDOWN:
	default:
	    Transmitter->Power(Output, Power);
	    Encoder->Power(Output, Power);
	    return;
    }
}

/*
 *
 */
static void
DigMode(struct rhdOutput *Output, DisplayModePtr Mode)
{
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;
    struct transmitter *Transmitter = &Private->Transmitter;
    struct encoder *Encoder = &Private->Encoder;
    struct rhdCrtc *Crtc = Output->Crtc;

    RHDFUNC(Output);

    /*
     * For dual link capable DVI we need to decide from the pix clock if we are dual link.
     * Do it here as it is convenient.
     */
    if (Output->Connector->Type == RHD_CONNECTOR_DVI)
	Private->RunDualLink = (Mode->SynthClock > 165000) ? TRUE : FALSE;

    Encoder->Mode(Output, Crtc, Mode);
    Transmitter->Mode(Output, Crtc, Mode);
}

/*
 *
 */
static void
DigSave(struct rhdOutput *Output)
{
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;
    struct transmitter *Transmitter = &Private->Transmitter;
    struct encoder *Encoder = &Private->Encoder;

    RHDFUNC(Output);

    Encoder->Save(Output);
    Transmitter->Save(Output);
}

/*
 *
 */
static void
DigRestore(struct rhdOutput *Output)
{
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;
    struct transmitter *Transmitter = &Private->Transmitter;
    struct encoder *Encoder = &Private->Encoder;

    RHDFUNC(Output);

    Encoder->Restore(Output);
    Transmitter->Restore(Output);
}

/*
 *
 */
static void
DigDestroy(struct rhdOutput *Output)
{
    struct DIGPrivate *Private = (struct DIGPrivate *)Output->Private;
    struct transmitter *Transmitter = &Private->Transmitter;
    struct encoder *Encoder = &Private->Encoder;

    RHDFUNC(Output);

    Encoder->Destroy(Output);
    Transmitter->Destroy(Output);

    xfree(Private);
    Output->Private = NULL;
}

/*
 *
 */
struct rhdOutput *
RHDDIGInit(RHDPtr rhdPtr,  enum rhdOutputType outputType, CARD8 ConnectorType)
{
    struct rhdOutput *Output;
    struct DIGPrivate *Private;
    struct DIGEncoder *Encoder;

    RHDFUNC(rhdPtr);

    Output = xnfcalloc(sizeof(struct rhdOutput), 1);

    Output->scrnIndex = rhdPtr->scrnIndex;
    Output->Id = outputType;

    Output->Sense = NULL;
    Output->ModeValid = DigModeValid;
    Output->Mode = DigMode;
    Output->Power = DigPower;
    Output->Save = DigSave;
    Output->Restore = DigRestore;
    Output->Destroy = DigDestroy;

    Private = xnfcalloc(sizeof(struct DIGPrivate), 1);
    Output->Private = Private;

    switch (outputType) {
	case RHD_OUTPUT_UNIPHYA:
#ifdef ATOM_BIOS
	    Output->Name = "UNIPHY_A";
	    Private->EncoderID = ENCODER_DIG1;
	    Private->Transmitter.Private =
		(struct ATOMTransmitterPrivate *)xnfcalloc(sizeof (struct ATOMTransmitterPrivate), 1);

	    Private->Transmitter.Sense = NULL;
	    Private->Transmitter.ModeValid = ATOMTransmitterModeValid;
	    Private->Transmitter.Mode = ATOMTransmitterSet;
	    Private->Transmitter.Power = ATOMTransmitterPower;
	    Private->Transmitter.Save = ATOMTransmitterSave;
	    Private->Transmitter.Restore = ATOMTransmitterRestore;
	    Private->Transmitter.Destroy = ATOMTransmitterDestroy;
	    {
		struct ATOMTransmitterPrivate *transPrivate =
		    (struct ATOMTransmitterPrivate *)Private->Transmitter.Private;
		struct atomTransmitterConfig *atc = &transPrivate->atomTransmitterConfig;
		atc->coherent = Private->Coherent;
		atc->encoder = atomEncoderDIG1;
		atc->link = atomTransLinkA;
		transPrivate->atomTransmitterID = atomTransmitterUNIPHY;
	    }
	    break;
#else
	    xfree(Private);
	    xfree(Output);
	    return NULL;
#endif

	case RHD_OUTPUT_UNIPHYB:
#ifdef ATOM_BIOS
	    Output->Name = "UNIPHY_B";
	    Private->EncoderID = ENCODER_DIG2;
	    Private->Transmitter.Private =
		(struct atomTransmitterPrivate *)xnfcalloc(sizeof (struct ATOMTransmitterPrivate), 1);

	    Private->Transmitter.Sense = NULL;
	    Private->Transmitter.ModeValid = ATOMTransmitterModeValid;
	    Private->Transmitter.Mode = ATOMTransmitterSet;
	    Private->Transmitter.Power = ATOMTransmitterPower;
	    Private->Transmitter.Save = ATOMTransmitterSave;
	    Private->Transmitter.Restore = ATOMTransmitterRestore;
	    Private->Transmitter.Destroy = ATOMTransmitterDestroy;
	    {
		struct ATOMTransmitterPrivate *transPrivate =
		    (struct ATOMTransmitterPrivate *)Private->Transmitter.Private;
		struct atomTransmitterConfig *atc = &transPrivate->atomTransmitterConfig;
		atc->coherent = Private->Coherent;
		atc->encoder = atomEncoderDIG2;
		atc->link = atomTransLinkB;
	    }
	    ((struct ATOMTransmitterPrivate *)Private->Transmitter.Private)->atomTransmitterID
		= atomTransmitterUNIPHY;
	    break;
#else
	    xfree(Private);
	    xfree(Output);
	    return NULL;
#endif

	case RHD_OUTPUT_KLDSKP_LVTMA:
	    Output->Name = "UNIPHY_KLDSKP_LVTMA";
	    Private->EncoderID = ENCODER_DIG2;
	    Private->Transmitter.Private =
		(struct LVTMATransmitterPrivate *)xnfcalloc(sizeof (struct LVTMATransmitterPrivate), 1);

	    Private->Transmitter.Sense = NULL;
	    Private->Transmitter.ModeValid = LVTMATransmitterModeValid;
	    Private->Transmitter.Mode = LVTMATransmitterSet;
	    Private->Transmitter.Power = LVTMATransmitterPower;
	    Private->Transmitter.Save = LVTMATransmitterSave;
	    Private->Transmitter.Restore = LVTMATransmitterRestore;
	    Private->Transmitter.Destroy = LVTMATransmitterDestroy;
	    break;

	default:
	    xfree(Private);
	    xfree(Output);
	    return NULL;
    }

    Encoder = (struct DIGEncoder *)(xnfcalloc(sizeof (struct DIGEncoder),1));
    Private->Encoder.Private = Encoder;
    Private->Encoder.ModeValid = EncoderModeValid;
    Private->Encoder.Mode = EncoderSet;
    Private->Encoder.Power = EncoderPower;
    Private->Encoder.Save = EncoderSave;
    Private->Encoder.Restore = EncoderRestore;
    Private->Encoder.Destroy = EncoderDestroy;

    switch (ConnectorType) {
	case RHD_CONNECTOR_PANEL:
	    Private->EncoderMode = LVDS;
	    GetLVDSInfo(rhdPtr, Private);
	    break;
	case RHD_CONNECTOR_DVI:
	    Private->RunDualLink = FALSE; /* will be set later acc to pxclk */
	    Private->EncoderMode = TMDS_DVI;
	    break;
	case RHD_CONNECTOR_DVI_SINGLE:
	    Private->RunDualLink = FALSE;
	    Private->EncoderMode = TMDS_DVI;  /* currently also HDMI */
	    break;
    }

    return Output;
}
