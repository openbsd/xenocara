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
#include "rhd_connector.h"
#include "rhd_output.h"
#include "rhd_crtc.h"
#include "rhd_regs.h"


#define REG_DACA_OFFSET 0
#define REG_DACB_OFFSET 0x200

struct rhdDACPrivate {
    Bool Stored;

    CARD32 Store_Powerdown;
    CARD32 Store_Force_Output_Control;
    CARD32 Store_Source_Select;
    CARD32 Store_Enable;
    CARD32 Store_Control1;
    CARD32 Store_Control2;
};

/*
 *
 */
static CARD8
DACASense(struct rhdOutput *Output, Bool TV)
{
    CARD32 CompEnable, Control1, Control2, DetectControl, Enable;
    CARD8 ret;

    CompEnable = RHDRegRead(Output, DACA_COMPARATOR_ENABLE);
    Control1 = RHDRegRead(Output, DACA_CONTROL1);
    Control2 = RHDRegRead(Output, DACA_CONTROL2);
    DetectControl = RHDRegRead(Output, DACA_AUTODETECT_CONTROL);
    Enable = RHDRegRead(Output, DACA_ENABLE);

    RHDRegWrite(Output, DACA_ENABLE, 1);
    RHDRegMask(Output, DACA_AUTODETECT_CONTROL, 0, 0x00000003);
    RHDRegMask(Output, DACA_CONTROL2, 0, 0x00000001);

    if (TV)
	RHDRegMask(Output, DACA_CONTROL2, 0x00000100, 0x00000100);
    else
	RHDRegMask(Output, DACA_CONTROL2, 0, 0x00000100);

    RHDRegWrite(Output, DACA_FORCE_DATA, 0);
    RHDRegMask(Output, DACA_CONTROL2, 0x00000001, 0x0000001);

    RHDRegMask(Output, DACA_COMPARATOR_ENABLE, 0x00070000, 0x00070000);
    RHDRegWrite(Output, DACA_CONTROL1, 0x00050802);
    RHDRegMask(Output, DACA_POWERDOWN, 0, 0x00000001); /* Shut down Bandgap Voltage Reference Power */
    usleep(5);

    RHDRegMask(Output, DACA_POWERDOWN, 0, 0x01010100); /* Shut down RGB */

    RHDRegWrite(Output, DACA_FORCE_DATA, 0x1e6); /* 486 out of 1024 */
    usleep(200);

    RHDRegMask(Output, DACA_POWERDOWN, 0x01010100, 0x01010100); /* Enable RGB */
    usleep(88);

    RHDRegMask(Output, DACA_POWERDOWN, 0, 0x01010100); /* Shut down RGB */

    RHDRegMask(Output, DACA_COMPARATOR_ENABLE, 0x00000100, 0x00000100);
    usleep(100);

    /* Get RGB detect values
     * If only G is detected, we could have a monochrome monitor,
     * but we don't bother with this at the moment.
     */
    ret = (RHDRegRead(Output, DACA_COMPARATOR_OUTPUT) & 0x0E) >> 1;

    RHDRegMask(Output, DACA_COMPARATOR_ENABLE, CompEnable, 0x00FFFFFF);
    RHDRegWrite(Output, DACA_CONTROL1, Control1);
    RHDRegMask(Output, DACA_CONTROL2, Control2, 0x000001FF);
    RHDRegMask(Output, DACA_AUTODETECT_CONTROL, DetectControl, 0x000000FF);
    RHDRegMask(Output, DACA_ENABLE, Enable, 0x000000FF);

    RHDDebug(Output->scrnIndex, "%s: DAC: 0x0%1X\n", __func__, ret);

    return ret;
}

/*
 * TV detection is for later.
 */
static Bool
DACASenseCRT(struct rhdOutput *Output, enum rhdConnectorType Type)
{
    RHDFUNC(Output);

    switch (Type) {
    case RHD_CONNECTOR_DVI:
    case RHD_CONNECTOR_VGA:
	return (DACASense(Output, FALSE) & 0xFF);
    case RHD_CONNECTOR_TV:
    default:
	xf86DrvMsg(Output->scrnIndex, X_WARNING,
		   "%s: connector type %d is not supported.\n",
		   __func__, Type);
	return FALSE;
    }
}

/*
 *
 */
static CARD8
DACBSense(struct rhdOutput *Output, Bool TV)
{
    CARD32 CompEnable, Control1, Control2, DetectControl, Enable;
    CARD8 ret;

    CompEnable = RHDRegRead(Output, DACB_COMPARATOR_ENABLE);
    Control1 = RHDRegRead(Output, DACB_CONTROL1);
    Control2 = RHDRegRead(Output, DACB_CONTROL2);
    DetectControl = RHDRegRead(Output, DACB_AUTODETECT_CONTROL);
    Enable = RHDRegRead(Output, DACB_ENABLE);

    RHDRegWrite(Output, DACB_ENABLE, 1);
    RHDRegMask(Output, DACB_AUTODETECT_CONTROL, 0, 0x00000003);
    RHDRegMask(Output, DACB_CONTROL2, 0, 0x00000001);

    if (TV)
	RHDRegMask(Output, DACB_CONTROL2, 0x00000100, 0x00000100);
    else
	RHDRegMask(Output, DACB_CONTROL2, 0, 0x00000100);

    RHDRegWrite(Output, DACB_FORCE_DATA, 0);
    RHDRegMask(Output, DACB_CONTROL2, 0x00000001, 0x0000001);

    RHDRegMask(Output, DACB_COMPARATOR_ENABLE, 0x00070000, 0x00070000);
    RHDRegWrite(Output, DACB_CONTROL1, 0x00050802);
    RHDRegMask(Output, DACB_POWERDOWN, 0, 0x00000001); /* Shut down Bandgap Voltage Reference Power */
    usleep(5);

    RHDRegMask(Output, DACB_POWERDOWN, 0, 0x01010100); /* Shut down RGB */

    RHDRegWrite(Output, DACB_FORCE_DATA, 0x1e6); /* 486 out of 1024 */
    usleep(200);

    RHDRegMask(Output, DACB_POWERDOWN, 0x01010100, 0x01010100); /* Enable RGB */
    usleep(88);

    RHDRegMask(Output, DACB_POWERDOWN, 0, 0x01010100); /* Shut down RGB */

    RHDRegMask(Output, DACB_COMPARATOR_ENABLE, 0x00000100, 0x00000100);
    usleep(100);

    /* Get RGB detect values
     * If only G is detected, we could have a monochrome monitor,
     * but we don't bother with this at the moment.
     */
    ret = (RHDRegRead(Output, DACB_COMPARATOR_OUTPUT) & 0x0E) >> 1;

    RHDRegMask(Output, DACB_COMPARATOR_ENABLE, CompEnable, 0x00FFFFFF);
    RHDRegWrite(Output, DACB_CONTROL1, Control1);
    RHDRegMask(Output, DACB_CONTROL2, Control2, 0x000001FF);
    RHDRegMask(Output, DACB_AUTODETECT_CONTROL, DetectControl, 0x000000FF);
    RHDRegMask(Output, DACB_ENABLE, Enable, 0x000000FF);

    RHDDebug(Output->scrnIndex, "%s: DAC: 0x0%1X\n", __func__, ret);

    return ret;
}

/*
 * TV detection is for later.
 */
static Bool
DACBSenseCRT(struct rhdOutput *Output, enum rhdConnectorType Type)
{
    RHDFUNC(Output);

    switch (Type) {
    case RHD_CONNECTOR_DVI:
    case RHD_CONNECTOR_VGA:
	return (DACBSense(Output, FALSE) & 0xFF);
    case RHD_CONNECTOR_TV:
    default:
	xf86DrvMsg(Output->scrnIndex, X_WARNING,
		   "%s: connector type %d is not supported.\n",
		   __func__, Type);
	return FALSE;
    }
}

/*
 *
 */
static ModeStatus
DACModeValid(struct rhdOutput *Output, DisplayModePtr Mode)
{
    RHDFUNC(Output);

    if (Mode->Clock < 20000)
	return MODE_CLOCK_LOW;

    if (Mode->Clock > 400000)
	return MODE_CLOCK_HIGH;

    return MODE_OK;
}

/*
 *
 */
static inline void
DACSet(struct rhdOutput *Output, CARD16 offset)
{
    RHDRegWrite(Output, offset + DACA_FORCE_OUTPUT_CNTL, 0);
    RHDRegMask(Output, offset + DACA_SOURCE_SELECT, Output->Crtc->Id, 0x00000001);
    RHDRegMask(Output, offset + DACA_CONTROL1, 0x00000002, 0x00000003);
    RHDRegMask(Output, offset + DACA_CONTROL2, 0, 0x00000001);
}

/*
 *
 */
static void
DACASet(struct rhdOutput *Output)
{
    RHDFUNC(Output);

    DACSet(Output, REG_DACA_OFFSET);
}

/*
 *
 */
static void
DACBSet(struct rhdOutput *Output)
{
    RHDFUNC(Output);

    DACSet(Output, REG_DACB_OFFSET);
}

/*
 *
 */
static inline void
DACPower(struct rhdOutput *Output, CARD16 offset, int Power)
{
    switch (Power) {
    case RHD_POWER_ON:
	RHDRegWrite(Output, offset + DACA_POWERDOWN, 0);
	RHDRegWrite(Output, offset + DACA_ENABLE, 1);
	return;
    case RHD_POWER_RESET: /* don't bother */
	return;
    case RHD_POWER_SHUTDOWN:
    default:
	RHDRegWrite(Output, offset + DACA_POWERDOWN, 0x01010101);
	RHDRegWrite(Output, offset + DACA_ENABLE, 0);
	return;
    }
}

/*
 *
 */
static void
DACAPower(struct rhdOutput *Output, int Power)
{
    RHDFUNC(Output);

    DACPower(Output, REG_DACA_OFFSET, Power);
}

/*
 *
 */
static void
DACBPower(struct rhdOutput *Output, int Power)
{
    RHDFUNC(Output);

    DACPower(Output, REG_DACB_OFFSET, Power);
}

/*
 *
 */
static inline void
DACSave(struct rhdOutput *Output, CARD16 offset)
{
    struct rhdDACPrivate *Private = (struct rhdDACPrivate *) Output->Private;

    Private->Store_Powerdown = RHDRegRead(Output, offset + DACA_POWERDOWN);
    Private->Store_Force_Output_Control = RHDRegRead(Output, offset + DACA_FORCE_OUTPUT_CNTL);
    Private->Store_Source_Select = RHDRegRead(Output, offset + DACA_SOURCE_SELECT);
    Private->Store_Enable = RHDRegRead(Output, offset + DACA_ENABLE);
    Private->Store_Control1 = RHDRegRead(Output, offset + DACA_CONTROL1);
    Private->Store_Control2 = RHDRegRead(Output, offset + DACA_CONTROL2);

    Private->Stored = TRUE;
}

/*
 *
 */
static void
DACASave(struct rhdOutput *Output)
{
    RHDFUNC(Output);

    DACSave(Output, REG_DACA_OFFSET);
}

/*
 *
 */
static void
DACBSave(struct rhdOutput *Output)
{
    RHDFUNC(Output);

    DACSave(Output, REG_DACB_OFFSET);
}

/*
 *
 */
static inline void
DACRestore(struct rhdOutput *Output, CARD16 offset)
{
    struct rhdDACPrivate *Private = (struct rhdDACPrivate *) Output->Private;

    RHDRegWrite(Output, offset + DACA_POWERDOWN, Private->Store_Powerdown);
    RHDRegWrite(Output, offset + DACA_FORCE_OUTPUT_CNTL, Private->Store_Force_Output_Control);
    RHDRegWrite(Output, offset + DACA_SOURCE_SELECT, Private->Store_Source_Select);
    RHDRegWrite(Output, offset + DACA_ENABLE, Private->Store_Enable);
    RHDRegWrite(Output, offset + DACA_CONTROL1, Private->Store_Control1);
    RHDRegWrite(Output, offset + DACA_CONTROL2, Private->Store_Control2);
}

/*
 *
 */
static void
DACARestore(struct rhdOutput *Output)
{
    RHDFUNC(Output);

    if (!((struct rhdDACPrivate *) Output->Private)->Stored) {
	xf86DrvMsg(Output->scrnIndex, X_ERROR,
		   "%s: No registers stored.\n", __func__);
	return;
    }

    DACRestore(Output, REG_DACA_OFFSET);
}

/*
 *
 */
static void
DACBRestore(struct rhdOutput *Output)
{
    RHDFUNC(Output);

    if (!((struct rhdDACPrivate *) Output->Private)->Stored) {
	xf86DrvMsg(Output->scrnIndex, X_ERROR,
		   "%s: No registers stored.\n", __func__);
	return;
    }

    DACRestore(Output, REG_DACB_OFFSET);
}

/*
 *
 */
static void
DACDestroy(struct rhdOutput *Output)
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
RHDDACAInit(RHDPtr rhdPtr)
{
    struct rhdOutput *Output;
    struct rhdDACPrivate *Private;

    RHDFUNC(rhdPtr);

    Output = xnfcalloc(sizeof(struct rhdOutput), 1);

    Output->scrnIndex = rhdPtr->scrnIndex;
    Output->Name = "DAC A";
    Output->Id = RHD_OUTPUT_DACA;

    Output->Sense = DACASenseCRT;
    Output->ModeValid = DACModeValid;
    Output->Mode = DACASet;
    Output->Power = DACAPower;
    Output->Save = DACASave;
    Output->Restore = DACARestore;
    Output->Destroy = DACDestroy;

    Private = xnfcalloc(sizeof(struct rhdDACPrivate), 1);
    Output->Private = Private;

    return Output;
}

/*
 *
 */
struct rhdOutput *
RHDDACBInit(RHDPtr rhdPtr)
{
    struct rhdOutput *Output;
    struct rhdDACPrivate *Private;

    RHDFUNC(rhdPtr);

    Output = xnfcalloc(sizeof(struct rhdOutput), 1);

    Output->scrnIndex = rhdPtr->scrnIndex;
    Output->Name = "DAC B";
    Output->Id = RHD_OUTPUT_DACB;

    Output->Sense = DACBSenseCRT;
    Output->ModeValid = DACModeValid;
    Output->Mode = DACBSet;
    Output->Power = DACBPower;
    Output->Save = DACBSave;
    Output->Restore = DACBRestore;
    Output->Destroy = DACDestroy;

    Private = xnfcalloc(sizeof(struct rhdDACPrivate), 1);
    Output->Private = Private;

    return Output;
}
