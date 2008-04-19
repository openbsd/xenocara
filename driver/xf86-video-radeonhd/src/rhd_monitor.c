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

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86DDC.h"
#if HAVE_XF86_ANSIC_H
# include "xf86_ansic.h"
#else
# include <string.h>
#endif

#include "rhd.h"
#include "rhd_connector.h"
#include "rhd_modes.h"
#include "rhd_monitor.h"
#ifdef ATOM_BIOS
# include "rhd_atombios.h"
#endif

/* From rhd_edid.c */
void RHDMonitorEDIDSet(struct rhdMonitor *Monitor, xf86MonPtr EDID);

/*
 *
 */
void
RHDMonitorPrint(struct rhdMonitor *Monitor)
{
    int i;

    xf86Msg(X_NONE, "    Bandwidth: %dMHz\n", Monitor->Bandwidth / 1000);
    xf86Msg(X_NONE, "    Horizontal timing:\n");
    for (i = 0; i < Monitor->numHSync; i++)
	xf86Msg(X_NONE, "        %3.1f - %3.1fkHz\n",  Monitor->HSync[i].lo,
		Monitor->HSync[i].hi);
    xf86Msg(X_NONE, "    Vertical timing:\n");
    for (i = 0; i < Monitor->numVRefresh; i++)
	xf86Msg(X_NONE, "        %3.1f - %3.1fHz\n",  Monitor->VRefresh[i].lo,
		Monitor->VRefresh[i].hi);
    xf86Msg(X_NONE, "    DPI: %dx%d\n", Monitor->xDpi, Monitor->yDpi);
    if (Monitor->ReducedAllowed)
	xf86Msg(X_NONE, "    Allows reduced blanking.\n");
    if (Monitor->UseFixedModes)
	xf86Msg(X_NONE, "    Uses Fixed Modes.\n");

    if (!Monitor->Modes)
	xf86Msg(X_NONE, "    No modes are provided.\n");
    else {
	DisplayModePtr Mode;

	xf86Msg(X_NONE, "    Attached modes:\n");
	for (Mode = Monitor->Modes; Mode; Mode = Mode->next) {
	    xf86Msg(X_NONE, "        ");
	    RHDPrintModeline(Mode);
	}
    }
}

/*
 *
 */
static struct rhdMonitor *
rhdMonitorFromConfig(int scrnIndex, MonPtr Config)
{
    struct rhdMonitor *Monitor;
    DisplayModePtr Mode;
    int i;

    Monitor = xnfcalloc(sizeof(struct rhdMonitor), 1);

    Monitor->Name = xnfstrdup(Config->id);
    Monitor->scrnIndex = scrnIndex;

    if (Config->nHsync) {
        Monitor->numHSync = Config->nHsync;
        for (i = 0; i < Config->nHsync; i++) {
            Monitor->HSync[i].lo = Config->hsync[i].lo;
            Monitor->HSync[i].hi = Config->hsync[i].hi;
        }
    } else if (!Monitor->numHSync) {
        Monitor->numHSync = 3;
        Monitor->HSync[0].lo = 31.5;
        Monitor->HSync[0].hi = 31.5;
        Monitor->HSync[1].lo = 35.15;
        Monitor->HSync[1].hi = 35.15;
        Monitor->HSync[2].lo = 35.5;
        Monitor->HSync[2].hi = 35.5;
    }

    if (Config->nVrefresh) {
        Monitor->numVRefresh = Config->nVrefresh;
        for (i = 0; i < Config->nVrefresh; i++) {
            Monitor->VRefresh[i].lo = Config->vrefresh[i].lo;
            Monitor->VRefresh[i].hi = Config->vrefresh[i].hi;
        }
    } else if (!Monitor->numVRefresh) {
        Monitor->numVRefresh = 1;
        Monitor->VRefresh[0].lo = 50;
        Monitor->VRefresh[0].hi = 61;
    }

#ifdef MONREC_HAS_REDUCED
    if (Config->reducedblanking)
        Monitor->ReducedAllowed = TRUE;
#endif

#ifdef MONREC_HAS_BANDWIDTH
    if (Config->maxPixClock)
        Monitor->Bandwidth = Config->maxPixClock;
#endif

    for (Mode = Config->Modes; Mode; Mode = Mode->next)
	Monitor->Modes = RHDModesAdd(Monitor->Modes, RHDModeCopy(Mode));

    return Monitor;
}

/*
 *
 */
static struct rhdMonitor *
rhdMonitorFromDefault(int scrnIndex, MonPtr Config)
{
    struct rhdMonitor *Monitor;
    DisplayModePtr Mode;

    Monitor = xnfcalloc(sizeof(struct rhdMonitor), 1);

    Monitor->Name = xnfstrdup("Default (SVGA)");
    Monitor->scrnIndex = scrnIndex;

    /* timing for pathetic 14" svga monitors */
    Monitor->numHSync = 3;
    Monitor->HSync[0].lo = 31.5;
    Monitor->HSync[0].hi = 31.5;
    Monitor->HSync[1].lo = 35.15;
    Monitor->HSync[1].hi = 35.15;
    Monitor->HSync[2].lo = 35.5;
    Monitor->HSync[2].hi = 35.5;

    Monitor->numVRefresh = 1;
    Monitor->VRefresh[0].lo = 50;
    Monitor->VRefresh[0].hi = 61;

    /* Try to add configged modes anyway */
    if (Config)
	for (Mode = Config->Modes; Mode; Mode = Mode->next)
	    Monitor->Modes = RHDModesAdd(Monitor->Modes, RHDModeCopy(Mode));

    return Monitor;
}

/*
 * This function tries to handle a configured monitor correctly.
 *
 * This either can be forced through the option, or is used when
 * no monitors are autodetected.
 */
void
RHDConfigMonitorSet(int scrnIndex, Bool UseConfig)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    RHDPtr rhdPtr = RHDPTR(pScrn);
    MonPtr Config = pScrn->confScreen->monitor;
    Bool HasConfig;

    int i;

    if (Config && Config->id && strcasecmp(Config->id, "<default monitor>"))
	HasConfig = TRUE;
    else
	HasConfig = FALSE;

    for (i = 0; i < RHD_CONNECTORS_MAX; i++)
	if (rhdPtr->Connector[i] && rhdPtr->Connector[i]->Monitor)
	    break;

    if (i == RHD_CONNECTORS_MAX)
	xf86DrvMsg(scrnIndex, X_INFO, "No monitors autodetected; "
		   "attempting to work around this.\n");
    else if (HasConfig) {
	xf86DrvMsg(scrnIndex, X_WARNING, "A Monitor section has been specified"
		   " in the config file.\n");
	xf86Msg(X_NONE, "     This might badly affect mode validation, and might "
		"make X fail.\n");
	xf86Msg(X_NONE, "     Unless this section is absolutely necessary, comment"
		" out the line\n" "\t\tMonitor \"%s\"\n"
		"     from the Screen section in your config file.\n", Config->id);
    }

    if ((i == RHD_CONNECTORS_MAX) || UseConfig) {
	if (HasConfig)
	    rhdPtr->ConfigMonitor = rhdMonitorFromConfig(scrnIndex, Config);
	else
	    rhdPtr->ConfigMonitor = rhdMonitorFromDefault(scrnIndex, Config);

	xf86DrvMsg(scrnIndex, X_INFO, "Created monitor from %s: \"%s\":\n",
		   HasConfig ? "config" : "default",
		   rhdPtr->ConfigMonitor->Name);

	RHDMonitorPrint(rhdPtr->ConfigMonitor);
    }
}

/*
 * Make sure that we keep only a single mode in our list. This mode should
 * hopefully match our panel at native resolution correctly.
 */
static void
rhdPanelEDIDModesFilter(struct rhdMonitor *Monitor)
{
    DisplayModeRec *Best = Monitor->Modes, *Mode, *Temp;

    RHDFUNC(Monitor);

    if (!Best || !Best->next)
	return; /* don't bother */

    /* don't go for preferred, just take the biggest */
    for (Mode = Best->next; Mode; Mode = Mode->next) {
	if (((Best->HDisplay <= Mode->HDisplay) &&
	     (Best->VDisplay < Mode->VDisplay)) ||
	    ((Best->HDisplay < Mode->HDisplay) &&
	     (Best->VDisplay <= Mode->VDisplay)))
	    Best = Mode;
    }

    xf86DrvMsg(Monitor->scrnIndex, X_INFO, "Monitor \"%s\": Using Mode \"%s\""
	       " for native resolution.\n", Monitor->Name, Best->name);

    /* kill all other modes */
    Mode = Monitor->Modes;
    while (Mode) {
	Temp = Mode->next;

	if (Mode != Best) {
	    RHDDebug(Monitor->scrnIndex, "Monitor \"%s\": Discarding Mode \"%s\"\n",
		     Monitor->Name, Mode->name);

	    xfree(Mode->name);
	    xfree(Mode);
	}

	Mode = Temp;
    }

    Best->next = NULL;
    Best->prev = NULL;
    Best->type |= M_T_PREFERRED;
    Monitor->Modes = Best;

    Monitor->numHSync = 1;
    Monitor->HSync[0].lo = Best->HSync;
    Monitor->HSync[0].hi = Best->HSync;
    Monitor->numVRefresh = 1;
    Monitor->VRefresh[0].lo = Best->VRefresh;
    Monitor->VRefresh[0].hi = Best->VRefresh;
    Monitor->Bandwidth = Best->Clock;
}

/*
 *
 */
void
rhdMonitorPrintEDID(struct rhdMonitor *Monitor, xf86MonPtr EDID)
{
    xf86DrvMsg(EDID->scrnIndex, X_INFO, "EDID data for %s\n",
	       Monitor->Name);
    xf86PrintEDID(EDID);
}

/*
 * Panels are the most complicated case we need to handle here.
 * Information can come from several places, and we need to make sure
 * that we end up with only the native resolution in our table.
 */
static struct rhdMonitor *
rhdMonitorPanel(struct rhdConnector *Connector)
{
    struct rhdMonitor *Monitor;
    DisplayModeRec *Mode = NULL;
    xf86MonPtr EDID = NULL;

    /* has priority over AtomBIOS EDID */
    if (Connector->DDC)
	EDID = xf86DoEDID_DDC2(Connector->scrnIndex, Connector->DDC);

#ifdef ATOM_BIOS
    {
	RHDPtr rhdPtr = RHDPTR(xf86Screens[Connector->scrnIndex]);
	AtomBiosArgRec data;
	AtomBiosResult Result;

	Result = RHDAtomBiosFunc(Connector->scrnIndex, rhdPtr->atomBIOS,
				 ATOMBIOS_GET_PANEL_MODE, &data);
	if (Result == ATOM_SUCCESS) {
	    Mode = data.mode;
	    Mode->type |= M_T_PREFERRED;
	}
	if (!EDID) {
	    Result = RHDAtomBiosFunc(Connector->scrnIndex,
				     rhdPtr->atomBIOS,
				     ATOMBIOS_GET_PANEL_EDID, &data);
	    if (Result == ATOM_SUCCESS)
		EDID = xf86InterpretEDID(Connector->scrnIndex,
					 data.EDIDBlock);
	}
    }
#endif

    Monitor = xnfcalloc(sizeof(struct rhdMonitor), 1);

    Monitor->scrnIndex = Connector->scrnIndex;
    Monitor->EDID      = EDID;

    if (Mode) {
	Monitor->Name = xstrdup("LVDS Panel");
	Monitor->Modes = RHDModesAdd(Monitor->Modes, Mode);
	Monitor->numHSync = 1;
	Monitor->HSync[0].lo = Mode->HSync;
	Monitor->HSync[0].hi = Mode->HSync;
	Monitor->numVRefresh = 1;
	Monitor->VRefresh[0].lo = Mode->VRefresh;
	Monitor->VRefresh[0].hi = Mode->VRefresh;
	Monitor->Bandwidth = Mode->SynthClock;

	/* Clueless atombios does give us a mode, but doesn't give us a
	 * DPI or a size. It is just perfect, right? */
	if (EDID) {
	    if (EDID->features.hsize)
		Monitor->xDpi = (Mode->HDisplay * 2.54) / ((float) EDID->features.hsize) + 0.5;
	    if (EDID->features.vsize)
		Monitor->yDpi = (Mode->VDisplay * 2.54) / ((float) EDID->features.vsize) + 0.5;
	}
    } else if (EDID) {
	RHDMonitorEDIDSet(Monitor, EDID);
	rhdPanelEDIDModesFilter(Monitor);
    } else {
	xf86DrvMsg(Connector->scrnIndex, X_ERROR,
		   "%s: No panel mode information found.\n", __func__);
	xfree(Monitor);
	return NULL;
    }

    /* panel should be driven at native resolution only. */
    Monitor->UseFixedModes = TRUE;
    Monitor->ReducedAllowed = TRUE;

    if (EDID)
	rhdMonitorPrintEDID(Monitor, EDID);

    return Monitor;
}

/*
 * rhdMonitorTV(): get TV modes. Currently we can only get this from AtomBIOS.
 */
static struct rhdMonitor *
rhdMonitorTV(struct rhdConnector *Connector)
{
    struct rhdMonitor *Monitor = NULL;
#ifdef ATOM_BIOS
    ScrnInfoPtr pScrn = xf86Screens[Connector->scrnIndex];
    RHDPtr rhdPtr = RHDPTR(pScrn);
    DisplayModeRec *Mode = NULL;
    AtomBiosArgRec arg;

    RHDFUNC(Connector);

    arg.tvMode = rhdPtr->tvMode;
    if (RHDAtomBiosFunc(Connector->scrnIndex, rhdPtr->atomBIOS, 
			ATOM_ANALOG_TV_MODE, &arg)
	!= ATOM_SUCCESS)
	return NULL;

    Mode = arg.mode;
    Mode->type |= M_T_PREFERRED;

    Monitor = xnfcalloc(sizeof(struct rhdMonitor), 1);

    Monitor->scrnIndex = Connector->scrnIndex;
    Monitor->EDID      = NULL;

    Monitor->Name      = xstrdup("TV");
    Monitor->Modes     = RHDModesAdd(Monitor->Modes, Mode);
    Monitor->numHSync  = 1;
    Monitor->HSync[0].lo = Mode->HSync;
    Monitor->HSync[0].hi = Mode->HSync;
    Monitor->numVRefresh = 1;
    Monitor->VRefresh[0].lo = Mode->VRefresh;
    Monitor->VRefresh[0].hi = Mode->VRefresh;
    Monitor->Bandwidth = Mode->SynthClock;

    /* TV should be driven at native resolution only. */
    Monitor->UseFixedModes = TRUE;
    Monitor->ReducedAllowed = FALSE;
    /* 
     *  hack: the TV encoder takes care of that. 
     *  The mode that goes in isn't what comes out.
     */
    Mode->Flags &= ~(V_INTERLACE);
#endif
    return Monitor;
}

/*
 *
 */
struct rhdMonitor *
RHDMonitorInit(struct rhdConnector *Connector)
{
    struct rhdMonitor *Monitor = NULL;

    RHDFUNC(Connector);

    if (Connector->Type == RHD_CONNECTOR_PANEL)
	Monitor = rhdMonitorPanel(Connector);
    else if (Connector->Type == RHD_CONNECTOR_TV)
	Monitor = rhdMonitorTV(Connector);
    else if (Connector->DDC) {
	xf86MonPtr EDID = xf86DoEDID_DDC2(Connector->scrnIndex, Connector->DDC);
	if (EDID) {
	    Monitor = xnfcalloc(sizeof(struct rhdMonitor), 1);
	    Monitor->scrnIndex = Connector->scrnIndex;
	    Monitor->EDID      = EDID;

	    RHDMonitorEDIDSet(Monitor, EDID);
	    rhdMonitorPrintEDID(Monitor, EDID);
	}
    }

    return Monitor;
}

/*
 *
 */
void
RHDMonitorDestroy(struct rhdMonitor *Monitor)
{
    DisplayModePtr Mode, Next;

    for (Mode = Monitor->Modes; Mode;) {
	Next = Mode->next;

	xfree(Mode->name);
	xfree(Mode);

	Mode = Next;
    }

    if (Monitor->EDID)
	xfree(Monitor->EDID->rawData);
    xfree(Monitor->EDID);
    xfree(Monitor->Name);
    xfree(Monitor);
}
