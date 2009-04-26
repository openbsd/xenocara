/*
 * Copyright 2007, 2008  Luc Verhaegen <libv@exsuse.de>
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

#include "rhd.h"
#include "rhd_connector.h"
#include "rhd_output.h"
#include "rhd_crtc.h"

char *rhdPowerString[] = {
    "POWER_ON",
    "POWER_RESET",
    "POWER_SHUTDOWN",
    "POWER_UNKNOWN"
};

void
RHDOutputAdd(RHDPtr rhdPtr, struct rhdOutput *New)
{
    struct rhdOutput *Last = rhdPtr->Outputs;

    RHDFUNC(rhdPtr);

    if (!New)
	return;

    if (Last) {
	while (Last->Next)
	    Last = Last->Next;

	Last->Next = New;
    } else
	rhdPtr->Outputs = New;
}

/*
 *
 */
void
RHDOutputsMode(RHDPtr rhdPtr, struct rhdCrtc *Crtc, DisplayModePtr Mode)
{
    struct rhdOutput *Output = rhdPtr->Outputs;

    RHDFUNC(rhdPtr);

    while (Output) {
	if (Output->Active && Output->Mode && (Output->Crtc == Crtc))
	    Output->Mode(Output, Mode);

	Output = Output->Next;
    }
}

/*
 *
 */
void
RHDOutputsPower(RHDPtr rhdPtr, int Power)
{
    struct rhdOutput *Output = rhdPtr->Outputs;

    RHDFUNC(rhdPtr);

    while (Output) {
	if (Output->Active && Output->Power)
	    Output->Power(Output, Power);

	Output = Output->Next;
    }
}

/*
 *
 */
void
RHDOutputsShutdownInactive(RHDPtr rhdPtr)
{
    struct rhdOutput *Output = rhdPtr->Outputs;

    RHDFUNC(rhdPtr);

    while (Output) {
	if (!Output->Active && Output->Power) {
	    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "Shutting down %s\n", Output->Name);
	    Output->Power(Output, RHD_POWER_SHUTDOWN);
	}

	Output = Output->Next;
    }
}

/*
 *
 */
void
RHDOutputsSave(RHDPtr rhdPtr)
{
    struct rhdOutput *Output = rhdPtr->Outputs;

    RHDFUNC(rhdPtr);

    while (Output) {
	if (Output->Save)
	    Output->Save(Output);

	Output = Output->Next;
    }
}

/*
 *
 */
void
RHDOutputsRestore(RHDPtr rhdPtr)
{
    struct rhdOutput *Output = rhdPtr->Outputs;

    RHDFUNC(rhdPtr);

    while (Output) {
	if (Output->Restore)
	    Output->Restore(Output);

	Output = Output->Next;
    }
}

/*
 *
 */
void
RHDOutputsDestroy(RHDPtr rhdPtr)
{
    struct rhdOutput *Output = rhdPtr->Outputs, *Next;

    RHDFUNC(rhdPtr);

    while (Output) {
	Next = Output->Next;

	xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "Destroying %s\n", Output->Name);

	if (Output->Destroy)
	    Output->Destroy(Output);

	if (Output->OutputDriverPrivate)
	    xfree(Output->OutputDriverPrivate);
	xfree(Output);

	Output = Next;
    }
}

/*
 *
 */
void
RHDOutputPrintSensedType(struct rhdOutput *Output)
{
    struct { enum rhdSensedOutput type; char *name; }
    list[] = { { RHD_SENSED_NONE, "none" },
	     { RHD_SENSED_VGA, "VGA" },
	     { RHD_SENSED_DVI, "DVI" },
	     { RHD_SENSED_TV_SVIDEO, "TV_SVIDEO"},
	     { RHD_SENSED_TV_COMPOSITE, "TV_COMPOSITE" },
	     { RHD_SENSED_TV_COMPONENT, "TV_COMPONENT" },
	     { 0, NULL }
    };
    int i = 0;

    while (list[i].name) {
	if (list[i].type == Output->SensedType) {
	    xf86DrvMsgVerb(Output->scrnIndex, X_INFO, 3,
			   "%s: Sensed Output: %s\n",Output->Name,
			   list[i].name);
	    return;
	}
	i++;
    }
}

/*
 * Attach an connector to the specified output and set output properties depending on the connector
 */
void
RHDOutputAttachConnector(struct rhdOutput *Output, struct rhdConnector *Connector)
{
    RHDPtr rhdPtr = RHDPTRI(Output);

    if(Output->Connector == Connector)
	return; /* output is allready attached to this connector -> nothing todo */

    Output->Connector = Connector;

    if(!Output->Property) /* property controll available? */
	return;  /* no -> we are done here */

    /* yes -> check if we need to set any properties */
    if (Output->Property(Output, rhdPropertyCheck, RHD_OUTPUT_COHERENT, NULL)) {
	union rhdPropertyData val;
	switch(RhdParseBooleanOption(&rhdPtr->coherent, Connector->Name)) {
	    case RHD_OPTION_NOT_SET:
		/* for compatibility with old implementation, test also output name */
		switch(RhdParseBooleanOption(&rhdPtr->coherent, Output->Name)) {
		    case RHD_OPTION_NOT_SET:
		    case RHD_OPTION_DEFAULT:
		    case RHD_OPTION_OFF:
			val.Bool = FALSE;
			break;
		    case RHD_OPTION_ON:
			val.Bool = TRUE;
			break;
		}
		break;
	    case RHD_OPTION_DEFAULT:
	    case RHD_OPTION_OFF:
		val.Bool = FALSE;
		break;
	    case RHD_OPTION_ON:
		val.Bool = TRUE;
		break;
	}
	if(Output->Property(Output, rhdPropertySet, RHD_OUTPUT_COHERENT, &val))
	    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "Setting %s to %scoherent\n", Output->Name, val.Bool ? "" : "in");
	else
	    xf86DrvMsg(rhdPtr->scrnIndex, X_WARNING, "Failed to set %s to %scoherent\n", Output->Name, val.Bool ? "" : "in");
    }

    /* ask attached connector if EEDID or config options say we should enable HDMI */
    if (Output->Property(Output, rhdPropertyCheck, RHD_OUTPUT_HDMI, NULL)) {
	union rhdPropertyData val;
	val.Bool = RHDConnectorEnableHDMI(Connector);
	if(!Output->Property(Output, rhdPropertySet, RHD_OUTPUT_HDMI, &val))
	    xf86DrvMsg(rhdPtr->scrnIndex, X_WARNING, "Failed to %s HDMI on %s\n", val.Bool ? "disable" : "enable", Output->Name);
    }
}
