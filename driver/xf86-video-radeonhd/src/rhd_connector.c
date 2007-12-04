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
#include "edid.h"

/* for usleep */
#include "xf86_ansic.h"

#include "rhd.h"
#include "rhd_connector.h"
#include "rhd_output.h"
#include "rhd_regs.h"
#include "rhd_monitor.h"
#include "rhd_card.h"

#ifdef ATOM_BIOS
#include "rhd_atombios.h"
#endif

#include "xf86i2c.h"
#include "rhd_i2c.h"

/*
 *
 */
struct rhdHPD {
    Bool Stored;
    CARD32 StoreMask;
    CARD32 StoreEnable;
};

/*
 *
 */
void
RHDHPDSave(RHDPtr rhdPtr)
{
    struct rhdHPD *hpd = rhdPtr->HPD;

    RHDFUNC(rhdPtr);

    hpd->StoreMask = RHDRegRead(rhdPtr, DC_GPIO_HPD_MASK);
    hpd->StoreEnable = RHDRegRead(rhdPtr, DC_GPIO_HPD_EN);

    hpd->Stored = TRUE;
}

/*
 *
 */
void
RHDHPDRestore(RHDPtr rhdPtr)
{
    struct rhdHPD *hpd = rhdPtr->HPD;

    RHDFUNC(rhdPtr);

    if (hpd->Stored) {
	RHDRegWrite(rhdPtr, DC_GPIO_HPD_MASK, hpd->StoreMask);
	RHDRegWrite(rhdPtr, DC_GPIO_HPD_EN, hpd->StoreEnable);
    } else
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
		   "%s: no registers stored.\n", __func__);
}

/*
 *
 */
static void
RHDHPDSet(RHDPtr rhdPtr)
{
    RHDFUNC(rhdPtr);

    /* give the hw full control */
    RHDRegWrite(rhdPtr, DC_GPIO_HPD_MASK, 0);
    RHDRegWrite(rhdPtr, DC_GPIO_HPD_EN, 0);

    usleep(1);
}

/*
 *
 */
static Bool
RHDHPDCheck(struct rhdConnector *Connector)
{
    return (RHDRegRead(Connector, DC_GPIO_HPD_Y) & Connector->HPDMask);
}

struct rhdCsState {
    int vga_cnt;
    int dvi_cnt;
};

/*
 *
 */
static char *
rhdConnectorSynthName(struct rhdConnectorInfo *ConnectorInfo, 
		      struct rhdCsState **state)
{
    char *str = NULL;
    char *typec;
    char *str1, *str2;

    assert(state != NULL); 

    if (!*state) {
	if (!(*state = xcalloc(sizeof(struct rhdCsState), 1)))
	    return NULL;
    }
    switch (ConnectorInfo->Type) {
	case RHD_CONNECTOR_NONE:
	    return NULL;
	case RHD_CONNECTOR_DVI:
	    if (ConnectorInfo->Output[0] && ConnectorInfo->Output[1])
		typec = "I";
	    else if (ConnectorInfo->Output[0] == RHD_OUTPUT_DACA 
		     || ConnectorInfo->Output[0] == RHD_OUTPUT_DACB
		     || ConnectorInfo->Output[1] == RHD_OUTPUT_DACA
		     || ConnectorInfo->Output[1] == RHD_OUTPUT_DACB
		)
		typec = "A";
	    else 
		typec = "D";
	    str = xalloc(12);
	    snprintf(str, 11, "DVI-%s %i",typec, ++(*state)->dvi_cnt);
	    return str;

	case RHD_CONNECTOR_VGA:
	    str = xalloc(10);
	    snprintf(str, 9, "VGA %i",++(*state)->vga_cnt);
	    return str;

	case RHD_CONNECTOR_PANEL:
	    str = xalloc(10);
	    snprintf(str, 9, "PANEL");
	    return str;

	case RHD_CONNECTOR_TV:
	    str1 = strdup(ConnectorInfo->Name);
	    str = xalloc(20);
	    str2 = strchr(str1, ' ');
	    if (str2) *(str2) = '\0';
	    snprintf(str, 20, "TV %s",str1);
	    xfree(str1);

	    return str;
    }
    return NULL;
}

/*
 *
 */
Bool
RHDConnectorsInit(RHDPtr rhdPtr, struct rhdCard *Card)
{
    struct rhdConnectorInfo *ConnectorInfo;
    struct rhdConnector *Connector;
    struct rhdOutput *Output;
    struct rhdCsState *csstate = NULL;
    int i, j, k, l;
    Bool InfoAllocated = FALSE;

    RHDFUNC(rhdPtr);

    /* Card->ConnectorInfo is there to work around quirks, so check it first */
    if (Card && (Card->ConnectorInfo[0].Type != RHD_CONNECTOR_NONE))
	ConnectorInfo = Card->ConnectorInfo;
    else {
#ifdef ATOM_BIOS
	/* common case */
	AtomBiosArgRec data;
	AtomBiosResult result;

	result = RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
				 ATOMBIOS_GET_CONNECTORS, &data);
	if (result == ATOM_SUCCESS) {
	    ConnectorInfo = data.connectorInfo;
	    InfoAllocated = TRUE;
	} else
#endif
        {
	    xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Failed to retrieve "
		       "Connector information.\n", __func__);
	    return FALSE;
	}
    }

    /* Init HPD */
    rhdPtr->HPD = xnfcalloc(sizeof(struct rhdHPD), 1);
    RHDHPDSave(rhdPtr);
    RHDHPDSet(rhdPtr);

    for (i = 0, j = 0; i < RHD_CONNECTORS_MAX; i++) {
	if (ConnectorInfo[i].Type == RHD_CONNECTOR_NONE)
	    continue;

	RHDDebug(rhdPtr->scrnIndex, "%s: %d (%s) type %d, ddc %d, hpd %d\n",
		 __func__, i, ConnectorInfo[i].Name, ConnectorInfo[i].Type,
		 ConnectorInfo[i].DDC, ConnectorInfo[i].HPD);

	Connector = xnfcalloc(sizeof(struct rhdConnector), 1);

	Connector->scrnIndex = rhdPtr->scrnIndex;

	Connector->Type = ConnectorInfo[i].Type;
	Connector->Name = rhdConnectorSynthName(&ConnectorInfo[i], &csstate);

	/* Get the DDC bus of this connector */
	if (ConnectorInfo[i].DDC != RHD_DDC_NONE) {
	    RHDI2CDataArg data;
	    int ret;

	    data.i = ConnectorInfo[i].DDC;
	    ret = RHDI2CFunc(rhdPtr->scrnIndex,
			     rhdPtr->I2C, RHD_I2C_GETBUS, &data);
	    if (ret == RHD_I2C_SUCCESS)
		Connector->DDC = data.i2cBusPtr;
	}

	/* attach HPD */
	switch(ConnectorInfo[i].HPD) {
	case RHD_HPD_0:
	    Connector->HPDMask = 0x00000001;
	    Connector->HPDCheck = RHDHPDCheck;
	    break;
	case RHD_HPD_1:
	    Connector->HPDMask = 0x00000100;
	    Connector->HPDCheck = RHDHPDCheck;
	    break;
	case RHD_HPD_2:
	    Connector->HPDMask = 0x00010000;
	    Connector->HPDCheck = RHDHPDCheck;
	    break;
	default:
	    Connector->HPDCheck = NULL;
	    break;
	}

	/* create Outputs */
	for (k = 0; k < 2; k++) {
	    if (ConnectorInfo[i].Output[k] == RHD_OUTPUT_NONE)
		continue;

	    /* Check whether the output exists already */
	    for (Output = rhdPtr->Outputs; Output; Output = Output->Next)
		if (Output->Id == ConnectorInfo[i].Output[k])
		    break;

	    if (!Output) {
		switch (ConnectorInfo[i].Output[k]) {
		case RHD_OUTPUT_DACA:
		    Output = RHDDACAInit(rhdPtr);
		    RHDOutputAdd(rhdPtr, Output);
		    break;
		case RHD_OUTPUT_DACB:
		    Output = RHDDACBInit(rhdPtr);
		    RHDOutputAdd(rhdPtr, Output);
		    break;
		case RHD_OUTPUT_TMDSA:
		    Output = RHDTMDSAInit(rhdPtr);
		    RHDOutputAdd(rhdPtr, Output);
		    break;
		case RHD_OUTPUT_LVTMA:
		    Output = RHDLVTMAInit(rhdPtr, ConnectorInfo[i].Type);
		    RHDOutputAdd(rhdPtr, Output);
		    break;
		default:
		    xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
			       "%s: unhandled output id: %d\n", __func__,
			       ConnectorInfo[i].Output[k]);
		    break;
		}
	    }

	    if (Output) {
		xf86DrvMsg(rhdPtr->scrnIndex, X_PROBED,
			   "Attaching Output %s to Connector %s\n",
			   Output->Name, Connector->Name);
		for (l = 0; l < 2; l++)
		    if (!Connector->Output[l]) {
			Connector->Output[l] = Output;
			break;
		    }
	    }
	}

	rhdPtr->Connector[j] = Connector;
	j++;
    }
    if (csstate)
	xfree(csstate);

    /* Deallocate what atombios code allocated */
    if (ConnectorInfo && InfoAllocated) {
	for (i = 0; i < RHD_CONNECTORS_MAX; i++)
	    if (ConnectorInfo[i].Type != RHD_CONNECTOR_NONE)
		xfree(ConnectorInfo[i].Name);
	xfree(ConnectorInfo);
    }

    RHDHPDRestore(rhdPtr);

    return (j && 1);
}

/*
 *
 */
void
RHDConnectorsDestroy(RHDPtr rhdPtr)
{
    struct rhdConnector *Connector;
    int i;

    RHDFUNC(rhdPtr);

    for (i = 0; i < RHD_CONNECTORS_MAX; i++) {
	Connector = rhdPtr->Connector[i];
	if (Connector) {
	    if (Connector->Monitor)
		RHDMonitorDestroy(Connector->Monitor);
	    xfree(Connector->Name);
	    xfree(Connector);
	}
    }
}

/*
 *
 */
void
RhdPrintConnectorInfo(int scrnIndex, struct rhdConnectorInfo *cp)
{
    int n;
    const char *c_name[] =
	{ "RHD_CONNECTOR_NONE", "RHD_CONNECTOR_VGA", "RHD_CONNECTOR_DVI",
	  "RHD_CONNECTOR_PANEL", "RHD_CONNECTOR_TV" };

    const char *ddc_name[] =
	{ "RHD_DDC_0", "RHD_DDC_1", "RHD_DDC_2", "RHD_DDC_3" };

    const char *hpd_name[] =
	{ "RHD_HPD_NONE", "RHD_HPD_0", "RHD_HPD_1", "RHD_HPD_2" };

    const char *output_name[] =
	{ "RHD_OUTPUT_NONE", "RHD_OUTPUT_DACA", "RHD_OUTPUT_DACB", "RHD_OUTPUT_TMDSA",
	  "RHD_OUTPUT_LVTMA"
	};

    for (n = 0; n < RHD_CONNECTORS_MAX; n++) {
	if (cp[n].Type == RHD_CONNECTOR_NONE)
	    break;
	xf86DrvMsg(scrnIndex, X_INFO, "Connector[%i] {%s, \"%s\", %s, %s, { %s, %s } }\n",
		   n, c_name[cp[n].Type], cp[n].Name,
		   cp[n].DDC == RHD_DDC_NONE ? "DDC_NONE" : ddc_name[cp[n].DDC],
		   hpd_name[cp[n].HPD], output_name[cp[n].Output[0]],
		   output_name[cp[n].Output[1]]);
    }
}
