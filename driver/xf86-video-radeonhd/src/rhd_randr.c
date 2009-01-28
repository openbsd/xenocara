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

/*
 * RandR interface.
 *
 * Only supports RandR 1.2 ATM or no RandR at all.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* Xserver interface */
#include "xf86.h"

#ifdef RANDR_12_SUPPORT
/* Xserver interface */
# include "randrstr.h"
# include "xf86i2c.h"		/* Missing in old versions of xf86Crtc.h */
# include "xf86Crtc.h"
# include "xf86RandR12.h"
# include "xaa.h"
# include "exa.h"
# define DPMS_SERVER
# include "X11/extensions/dpms.h"
# include "X11/Xatom.h"
#endif /* RANDR_12_SUPPORT */

#if HAVE_XF86_ANSIC_H
# include "xf86_ansic.h"
#endif

/* Driver specific headers */
#include "rhd.h"
#include "rhd_atombios.h"
#include "rhd_randr.h"

#ifdef RANDR_12_SUPPORT
# include "rhd_crtc.h"
# include "rhd_cursor.h"
# include "rhd_connector.h"
# include "rhd_output.h"
# include "rhd_modes.h"
# include "rhd_monitor.h"
# include "rhd_vga.h"
# include "rhd_pll.h"
# include "rhd_lut.h"
# include "rhd_mc.h"
# include "rhd_card.h"
# include "rhd_i2c.h"
# include "rhd_audio.h"

/*
 * Driver internal
 */

# define WRAP_SCRNINFO(func,new)					\
    do {							\
	rhdPtr->randr->func = pScrn->func;			\
	pScrn->func = new;					\
    } while (0)
# define UNWRAP_SCRNINFO(func)					\
	pScrn->func = rhdPtr->randr->func

struct rhdRandr {
    xf86CrtcPtr    RandrCrtc[2];
    xf86OutputPtr *RandrOutput;  /* NULL-terminated */
    void         (*PointerMoved)(int, int, int);
} ;

#define MAX_CONNECTORS_PER_RR_OUTPUT 4
/* Outputs and Connectors are combined for RandR due to missing abstraction */
typedef struct _rhdRandrOutput {
    char                 Name[64];
    struct rhdConnector *Connector;
    struct rhdOutput    *Output;
    DisplayModePtr	ScaledToMode;
    struct rhdCrtc      *Crtc;
    struct rhdConnector *AllConnectors[MAX_CONNECTORS_PER_RR_OUTPUT];
    Bool	        OutputActive;
} rhdRandrOutputRec, *rhdRandrOutputPtr;

struct rhdRandrCrtc {
    struct rhdCrtc    *rhdCrtc;
    union {
	FBLinearPtr MemXAA;
	ExaOffscreenArea *MemEXA;
    } u;
};

#define ATOM_SIGNAL_FORMAT    "RANDR_SIGNAL_FORMAT"
#define ATOM_CONNECTOR_TYPE   "RANDR_CONNECTOR_TYPE"
#define ATOM_CONNECTOR_NUMBER "RANDR_CONNECTOR_NUMBER"
#define ATOM_OUTPUT_NUMBER    "RANDR_OUTPUT_NUMBER"
#define ATOM_PANNING_AREA     "RANDR_PANNING_AREA"
#define ATOM_BACKLIGHT        "BACKLIGHT"
#define ATOM_COHERENT         "COHERENT"

static Atom atomSignalFormat, atomConnectorType, atomConnectorNumber,
    atomOutputNumber, atomPanningArea, atomBacklight, atomCoherent;


/* Get RandR property values */
static const char *
rhdGetSignalFormat(rhdRandrOutputPtr ro)
{
    switch (ro->Output->Id) {
    case RHD_OUTPUT_DACA:
    case RHD_OUTPUT_DACB:
	switch (ro->Connector->Type) {
	case RHD_CONNECTOR_VGA:
	case RHD_CONNECTOR_DVI:
	case RHD_CONNECTOR_DVI_SINGLE:
	    return "VGA";
	case RHD_CONNECTOR_TV:		/* TODO: depending on current format */
	default:
	    return "unknown";
	}
    case RHD_OUTPUT_LVTMA:
    case RHD_OUTPUT_KLDSKP_LVTMA:
    case RHD_OUTPUT_LVDS:
    case RHD_OUTPUT_UNIPHYA:
    case RHD_OUTPUT_UNIPHYB:
    case RHD_OUTPUT_UNIPHYC:
    case RHD_OUTPUT_UNIPHYD:
    case RHD_OUTPUT_UNIPHYE:
    case RHD_OUTPUT_UNIPHYF:
#if RHD_OUTPUT_LVTMB != RHD_OUTPUT_LVDS
    case RHD_OUTPUT_LVTMB:
#endif
	switch (ro->Connector->Type) {
	case RHD_CONNECTOR_DVI:
	case RHD_CONNECTOR_DVI_SINGLE:
	    return "TMDS";
	case RHD_CONNECTOR_PANEL:
	    return "LVDS";
	default:
	    return "unknown";
	}
    case RHD_OUTPUT_TMDSA:
#if RHD_OUTPUT_TMDSB != RHD_OUTPUT_LVDS
    case RHD_OUTPUT_TMDSB:
#endif
	return "TMDS";
    default:
	return "unknown";
    }
}
static const char *
rhdGetConnectorType(rhdRandrOutputPtr ro)
{
    switch (ro->Connector->Type) {
    case RHD_CONNECTOR_VGA:
	return "VGA";
    case RHD_CONNECTOR_DVI:
	return "DVI";			/* TODO: DVI-I/A/D / HDMI */
    case RHD_CONNECTOR_DVI_SINGLE:
	return "DVI (single link)";
    case RHD_CONNECTOR_PANEL:
	return "PANEL";
    case RHD_CONNECTOR_TV:
	return "TV";
    default:
	return "unknown";
    }
}

/* Set crtc pos according to mouse pos and panning information */
static void
rhdUpdateCrtcPos(RHDPtr rhdPtr, struct rhdCrtc *Crtc, int x, int y)
{
    int i;

    if (Crtc->MaxX > 0) {
	int cx = Crtc->X, cy = Crtc->Y;
	int w  = Crtc->CurrentMode->HDisplay;
	int h  = Crtc->CurrentMode->VDisplay;
	if (x < cx)
	    cx = x > Crtc->MinX ? x : Crtc->MinX;
	if (x >= cx + w)
	    cx = x < Crtc->MaxX ? x-w+1 : Crtc->MaxX-w;
	if (y < cy)
	    cy = y > Crtc->MinY ? y : Crtc->MinY;
	if (y >= cy + h)
	    cy = y < Crtc->MaxY ? y-h+1 : Crtc->MaxY-h;
	if (cx != Crtc->X || cy != Crtc->Y)
	    Crtc->FrameSet(Crtc, cx, cy);
	for (i = 0; i < 2; i++) {
	    xf86CrtcPtr crtc = (xf86CrtcPtr) rhdPtr->randr->RandrCrtc[i];
	    if (Crtc == ((struct rhdRandrCrtc *)crtc->driver_private)->rhdCrtc) {
		crtc->x = cx;
		crtc->y = cy;
	    }
	}
    }
}

/* Debug: print out state */
void
RHDDebugRandrState (RHDPtr rhdPtr, const char *msg)
{
    int i;
    xf86OutputPtr *ro;
    RHDDebug(rhdPtr->scrnIndex, "State at %s:\n", msg);

    for (i = 0; i < 2; i++) {
	xf86CrtcPtr    crtc = rhdPtr->randr->RandrCrtc[i];
	struct rhdCrtc *c = ((struct rhdRandrCrtc *)crtc->driver_private)->rhdCrtc;
	RHDDebugCont("   RRCrtc #%d [rhd %s]: active %d [%d]  "
		     "mode %s (%dx%d) +%d+%d\n",
		     i, c->Name, crtc->enabled, c->Active,
		     crtc->mode.name ? crtc->mode.name : "unnamed",
		     crtc->mode.HDisplay, crtc->mode.VDisplay,
		     crtc->x, crtc->y);
    }
    for (ro = rhdPtr->randr->RandrOutput; *ro; ro++) {
	rhdRandrOutputPtr o = (rhdRandrOutputPtr) (*ro)->driver_private;
	ASSERT(!strcmp((*ro)->name,o->Name));
	RHDDebugCont("   RROut  %s [Out %s Conn %s]  Crtc %s [%s]  "
		     "[%sactive]  %s\n",
		     (*ro)->name, o->Output->Name, o->Connector->Name,
		     (*ro)->crtc ? ((struct rhdRandrCrtc *)((*ro)->crtc->driver_private))->rhdCrtc->Name : "null",
		     o->Output->Crtc ? o->Output->Crtc->Name : "null",
		     o->Output->Active ? "" : "in",
		     (*ro)->status == XF86OutputStatusConnected ? "connected" :
		     (*ro)->status == XF86OutputStatusDisconnected ? "disconnected" :
		     (*ro)->status == XF86OutputStatusUnknown ? "unknownState" :
		     "badState" );
    }
}


/*
 * xf86CrtcConfig callback functions
 */

static Bool
rhdRRXF86CrtcResize(ScrnInfoPtr pScrn, int width, int height)
{
    RHDFUNC(pScrn);
    /* This is strange... if we set virtualX/virtualY like the intel driver
     * does, we limit ourself in the future to this maximum size.
     * The check for this is internally in RandR, no idea why the intel driver
     * actually works this way...
     * Even more curious: if we DON'T update virtual, everything seems to
     * work as expected... */
#if 0
    pScrn->virtualX = width;
    pScrn->virtualY = height;
#endif
    return TRUE;
}


/*
 * xf86Crtc callback functions
 */

/* Turns the crtc on/off, or sets intermediate power levels if available. */
static void
rhdRRCrtcDpms(xf86CrtcPtr Crtc, int mode)
{
    RHDPtr rhdPtr        = RHDPTR(Crtc->scrn);
    struct rhdCrtc *rhdCrtc = ((struct rhdRandrCrtc *)(Crtc->driver_private))->rhdCrtc;

    RHDDebug(rhdCrtc->scrnIndex, "%s: %s: %s\n", __func__, rhdCrtc->Name,
	     mode==DPMSModeOn ? "On" : mode==DPMSModeOff ? "Off" : "Other");

    switch (mode) {
    case DPMSModeOn:
	if (rhdCrtc->PLL)
	    rhdCrtc->PLL->Power(rhdCrtc->PLL, RHD_POWER_ON);
	rhdCrtc->Power(rhdCrtc, RHD_POWER_ON);
	rhdCrtc->Active = TRUE;
	break;
    case DPMSModeSuspend:
    case DPMSModeStandby:
	rhdCrtc->Power(rhdCrtc, RHD_POWER_RESET);
	if (rhdCrtc->PLL)
	    rhdCrtc->PLL->Power(rhdCrtc->PLL, RHD_POWER_RESET);
	rhdCrtc->Active = FALSE;
	break;
    case DPMSModeOff:
	rhdCrtc->Power(rhdCrtc, RHD_POWER_SHUTDOWN);
	if (rhdCrtc->PLL)
	    rhdCrtc->PLL->Power(rhdCrtc->PLL, RHD_POWER_SHUTDOWN);
	rhdCrtc->Active = FALSE;
	break;
    default:
	ASSERT(!"Unknown DPMS mode");
    }
    RHDDebugRandrState(rhdPtr, "POST-CrtcDpms");
}

/* Lock CRTC prior to mode setting. Returns whether unlock is needed */
static Bool
rhdRRCrtcLock(xf86CrtcPtr crtc)
{
    /* Looks like we don't have to lock for mode setting. Only as long as
     * buffers are fixed, of course */
    return FALSE;
}

/* Unlock CRTC after mode setting */
static void
rhdRRCrtcUnlock (xf86CrtcPtr crtc)
{ }

/* Helper: setup PLL and LUT for Crtc */
static void
setupCrtc(RHDPtr rhdPtr, struct rhdCrtc *Crtc)
{
    int i;

    /* PLL & LUT setup - static at the moment */
    if (Crtc->PLL)
	return;
    for (i = 0; i < 2; i++)
	if (Crtc == rhdPtr->Crtc[i])
	    break;
    ASSERT(i<2);
    Crtc->PLL = rhdPtr->PLLs[i];
    Crtc->LUT = rhdPtr->LUT[i];
}

/* Set video mode.
 * randr calls for Crtc and Output separately, while order should be
 * up to the driver. Well. */
static void
rhdRRCrtcPrepare(xf86CrtcPtr crtc)
{
    RHDPtr          rhdPtr = RHDPTR(crtc->scrn);
    ScrnInfoPtr     pScrn  = xf86Screens[rhdPtr->scrnIndex];
    struct rhdCrtc *rhdCrtc = ((struct rhdRandrCrtc *)(crtc->driver_private))->rhdCrtc;

    RHDFUNC(rhdPtr);
    setupCrtc(rhdPtr, rhdCrtc);
    pScrn->vtSema = TRUE;

    /* Disable CRTCs to stop noise from appearing. */
    rhdCrtc->Power(rhdCrtc, RHD_POWER_RESET);

    /* Verify panning area */
    if (rhdCrtc->MaxX > rhdCrtc->Width)
	rhdCrtc->MaxX = rhdCrtc->Width;
    if (rhdCrtc->MaxY > rhdCrtc->Height)
	rhdCrtc->MaxY = rhdCrtc->Height;
}

static void
rhdRRCrtcModeSet(xf86CrtcPtr  crtc,
		 DisplayModePtr OrigMode, DisplayModePtr Mode,
		 int x, int y)
{
    RHDPtr          rhdPtr = RHDPTR(crtc->scrn);
    ScrnInfoPtr     pScrn  = xf86Screens[rhdPtr->scrnIndex];
    struct rhdCrtc  *rhdCrtc  = ((struct rhdRandrCrtc*) (crtc->driver_private))->rhdCrtc;
    xf86CrtcConfigPtr xf86CrtcConfig = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    CARD32 ScanOutOffset;
    int i;

    /* RandR may give us a mode without a name... (xf86RandRModeConvert) */
    if (!Mode->name && crtc->mode.name)
	Mode->name = xstrdup(crtc->mode.name);

    RHDDebug(rhdPtr->scrnIndex, "%s: %s : %s at %d/%d\n", __func__,
	     rhdCrtc->Name, Mode->name, x, y);

    /*
     * for AtomBIOS SetPixelClock we need information about the outputs.
     * So find the output(s) matching this Crtc and assign the Crtc to it.
     */
    for (i = 0; i < xf86CrtcConfig->num_output; i++) {
	if (xf86CrtcConfig->output[i]->crtc == crtc) {
	    rhdRandrOutputPtr rout = xf86CrtcConfig->output[i]->driver_private;
	    rout->Output->Crtc = rhdCrtc;
	}
    }

    if (rhdPtr->verbosity >= 3) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "On Crtc %i Setting %3.1f Hz Mode: ",
		   rhdCrtc->Id, Mode->VRefresh);
	RHDPrintModeline(Mode);
	if (OrigMode->VDisplay != Mode->VDisplay || OrigMode->HDisplay != Mode->HDisplay) {
	    xf86DrvMsg(-1, X_NONE, "Scaled from: ");
	    RHDPrintModeline(OrigMode);
	}
    }
    /* Set up mode */
    if (crtc->rotatedData != NULL) {
	ScanOutOffset = (unsigned long) crtc->rotatedData - (unsigned long)rhdPtr->FbBase;
	x = y = 0;
    } else {
	ScanOutOffset = rhdPtr->FbScanoutStart;
    }
    rhdCrtc->FBSet(rhdCrtc, pScrn->displayWidth, pScrn->virtualX, pScrn->virtualY,
		pScrn->depth, ScanOutOffset);
    rhdCrtc->ModeSet(rhdCrtc, Mode);
    if (OrigMode->VDisplay != Mode->VDisplay || OrigMode->HDisplay != Mode->HDisplay)
	rhdCrtc->ScaleSet(rhdCrtc, rhdCrtc->ScaleType, OrigMode, Mode);
    else
	rhdCrtc->ScaleSet(rhdCrtc, RHD_CRTC_SCALE_TYPE_NONE, Mode, NULL);

    rhdCrtc->FrameSet(rhdCrtc, x, y);
    rhdUpdateCrtcPos(rhdPtr, rhdCrtc, rhdCrtc->Cursor->X, rhdCrtc->Cursor->Y);
    RHDPLLSet(rhdCrtc->PLL, Mode->Clock);		/* This also powers up PLL */
    rhdCrtc->LUTSelect(rhdCrtc, rhdCrtc->LUT);

    /*
     * RandR is able to bring up new Crtcs, but can't be bothered to set up
     * a cmap on them.
     *
     * The pScreen check tells us whether we are still in PreInit. If we are
     * still in PreInit, the xserver will still do the right thing and call
     * LoadPalette accordingly after the modeset. VT switch will also do the
     * right thing still, but at that time no new CRTC gets initialised, so
     * LUT->Initialised is either set, or the current function isn't called.
     */
    if (!rhdCrtc->LUT->Initialised && pScrn->pScreen)
	RHDLUTCopyForRR(rhdCrtc->LUT);
}

static void
rhdRRCrtcCommit(xf86CrtcPtr crtc)
{
    RHDPtr          rhdPtr = RHDPTR(crtc->scrn);
    struct rhdCrtc *rhdCrtc = ((struct rhdRandrCrtc *)(crtc->driver_private))->rhdCrtc;

    RHDFUNC(rhdPtr);

    rhdCrtc->Active = TRUE;
    rhdCrtc->Power(rhdCrtc, RHD_POWER_ON);

    if (crtc->scrn->pScreen != NULL)
	xf86_reload_cursors(crtc->scrn->pScreen);

    RHDDebugRandrState(rhdPtr, rhdCrtc->Name);
}

/*
 * They just had to do NIH again here: Old X functionality provides a size, a
 * list of indices, and a table of RGB unsigned shorts. RandR provides what
 * is below. Apart from horribly breaking any attempt at being backwards
 * compatible, this also pretty much rules out the usage of indexed colours, as
 * each time even a single colour is changed an entirely new table has to be
 * uploaded. Just cute. -- libv.
 */
static void
rhdRRCrtcGammaSet(xf86CrtcPtr crtc, CARD16 *red, CARD16 *green, CARD16 *blue,
		  int size)
{
    struct rhdCrtc *rhdCrtc = ((struct rhdRandrCrtc *)(crtc->driver_private))->rhdCrtc;
    int indices[0x100]; /* would RandR use a size larger than 256? */
    LOCO colors[0x100];
    int i;

    RHDDebug(rhdCrtc->scrnIndex, "%s: %s.\n", __func__, rhdCrtc->Name);

    /* thanks so very much */
    for (i = 0; i < size; i++) {
	indices[i] = i;
	colors[i].red = red[i];
	colors[i].green = green[i];
	colors[i].blue = blue[i];
    }

    rhdCrtc->LUT->Set(rhdCrtc->LUT, size, indices, colors);
}

/* Dummy, because not tested for NULL */
static Bool
rhdRRCrtcModeFixupDUMMY(xf86CrtcPtr    crtc,
			DisplayModePtr mode,
			DisplayModePtr adjusted_mode)
{
    return TRUE;
}

static void
rhdRRCrtcSetOrigin(xf86CrtcPtr  crtc, int x, int y)
{
    struct rhdCrtc  *rhdCrtc  = ((struct rhdRandrCrtc*) (crtc->driver_private))->rhdCrtc;

    rhdCrtc->FrameSet(rhdCrtc, x, y);
}


/*
 * xf86Output callback functions
 */

static void
rhdRROutputCreateResources(xf86OutputPtr out)
{
    RHDPtr rhdPtr          = RHDPTR(out->scrn);
    rhdRandrOutputPtr rout = (rhdRandrOutputPtr) out->driver_private;
    struct rhdOutput *o;
    const char       *val;
    CARD32            num;
    int              err;
    INT32            range[2];

    RHDFUNC(rhdPtr);

    /* Create atoms for potential RandR 1.3 properties */
    atomSignalFormat    = MakeAtom(ATOM_SIGNAL_FORMAT,
				   sizeof(ATOM_SIGNAL_FORMAT)-1, TRUE);
    atomConnectorType   = MakeAtom(ATOM_CONNECTOR_TYPE,
				   sizeof(ATOM_CONNECTOR_TYPE)-1, TRUE);
    atomConnectorNumber = MakeAtom(ATOM_CONNECTOR_NUMBER,
				   sizeof(ATOM_CONNECTOR_NUMBER)-1, TRUE);
    atomOutputNumber    = MakeAtom(ATOM_OUTPUT_NUMBER,
				   sizeof(ATOM_OUTPUT_NUMBER)-1, TRUE);
    atomPanningArea     = MakeAtom(ATOM_PANNING_AREA,
				   sizeof(ATOM_PANNING_AREA)-1, TRUE);

    /* Set up properties */
    RRConfigureOutputProperty(out->randr_output, atomSignalFormat,
			      FALSE, FALSE, TRUE, 0, NULL);
    RRConfigureOutputProperty(out->randr_output, atomConnectorType,
			      FALSE, FALSE, TRUE, 0, NULL);
    RRConfigureOutputProperty(out->randr_output, atomConnectorNumber,
			      FALSE, FALSE, TRUE, 0, NULL);
    RRConfigureOutputProperty(out->randr_output, atomPanningArea,
			      FALSE, FALSE, FALSE, 0, NULL);

    if (rout->Output->Property) {
	if (rout->Output->Property(rout->Output, rhdPropertyCheck, RHD_OUTPUT_BACKLIGHT, NULL)) {
	    atomBacklight = MakeAtom(ATOM_BACKLIGHT,
				     sizeof(ATOM_BACKLIGHT)-1, TRUE);

	    range[0] = 0;
	    range[1] = 255;
	    err = RRConfigureOutputProperty(out->randr_output, atomBacklight,
					    FALSE, TRUE, FALSE, 2, range);
	    if (err != 0)
		xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
			   "RRConfigureOutputProperty error: %d\n", err);
	    else {
		union rhdPropertyData val;

		if (!rout->Output->Property(rout->Output, rhdPropertyGet, RHD_OUTPUT_BACKLIGHT, &val))
		    val.integer = 255;

		err = RRChangeOutputProperty(out->randr_output, atomBacklight,
					     XA_INTEGER, 32, PropModeReplace,
					     1, &val.integer, FALSE, FALSE);
		if (err != 0)
		    xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
			       "In %s RRChangeOutputProperty error: %d\n",
			       __func__, err);
	    }
	}
	if (rout->Output->Property(rout->Output, rhdPropertyCheck, RHD_OUTPUT_COHERENT, NULL)) {
	    atomCoherent = MakeAtom(ATOM_COHERENT,
				     sizeof(ATOM_COHERENT)-1, TRUE);

	    range[0] = 0;
	    range[1] = 1;
	    err = RRConfigureOutputProperty(out->randr_output, atomCoherent,
					    FALSE, TRUE, FALSE, 2, range);
	    if (err != 0)
		xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
			   "RRConfigureOutputProperty error: %d\n", err);
	    else {
		union rhdPropertyData val;

		if (!rout->Output->Property(rout->Output, rhdPropertyGet, RHD_OUTPUT_COHERENT, &val))
		    val.Bool = 1;
		err = RRChangeOutputProperty(out->randr_output, atomCoherent,
					     XA_INTEGER, 32, PropModeReplace,
					     1, &val.Bool, FALSE, FALSE);
		if (err != 0)
		    xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
			       "In %s RRChangeOutputProperty error: %d\n",
			       __func__, err);
	    }
	}
    }

    val = rhdGetSignalFormat(rout);
    RRChangeOutputProperty(out->randr_output, atomSignalFormat,
			   XA_STRING, 8, PropModeReplace,
			   strlen(val), (char *) val, FALSE, FALSE);
    val = rhdGetConnectorType(rout);
    RRChangeOutputProperty(out->randr_output, atomConnectorType,
			   XA_STRING, 8, PropModeReplace,
			   strlen(val), (char *) val, FALSE, FALSE);
    for (num = 0; num < RHD_CONNECTORS_MAX; num++)
	if (rout->Connector == rhdPtr->Connector[num])
	    break;
    ASSERT(num < RHD_CONNECTORS_MAX);
    num++;		/* For RANDR_CONNECTOR_NUMBER 0 is unknown */
    RRChangeOutputProperty(out->randr_output, atomConnectorNumber,
			   XA_INTEGER, 32, PropModeReplace,
			   1, &num, FALSE, FALSE);
    for (num = 1, o = rhdPtr->Outputs; o; num++, o = o->Next)
	if (rout->Output == o)
	    break;
    ASSERT(o);
    RRChangeOutputProperty(out->randr_output, atomOutputNumber,
			   XA_INTEGER, 32, PropModeReplace,
			   1, &num, FALSE, FALSE);
    RRChangeOutputProperty(out->randr_output, atomPanningArea,
			   XA_STRING, 8, PropModeReplace,
			   0, NULL, FALSE, FALSE);
}

/* Turns the output on/off, or sets intermediate power levels if available. */
/* Something that cannot be solved with the current RandR model is that
 * multiple connectors might be attached to the same output. They cannot be
 * driven with different crtcs then, but RandR sees them as different outputs
 * because of a missing abstraction layer. This implementation will silently
 * set one crtc or the other, and maybe even won't warn. */
static void
rhdRROutputDpms(xf86OutputPtr       out,
		int                 mode)
{
    RHDPtr rhdPtr          = RHDPTR(out->scrn);
    rhdRandrOutputPtr rout = (rhdRandrOutputPtr) out->driver_private;
    xf86OutputPtr    *ro;
    struct rhdCrtc   *rhdCrtc = out->crtc ? ((struct rhdRandrCrtc *)(out->crtc->driver_private))->rhdCrtc : NULL;
    const char *outUsedBy = NULL;

    RHDDebug(rhdPtr->scrnIndex, "%s: Output %s : %s\n", __func__, rout->Name,
	     mode==DPMSModeOn ? "On" : mode==DPMSModeOff ? "Off" : "Other");

    for (ro = rhdPtr->randr->RandrOutput; *ro; ro++) {
	rhdRandrOutputPtr o = (rhdRandrOutputPtr) (*ro)->driver_private;
	if (o != rout && o->Output == rout->Output && (*ro)->crtc)
	    outUsedBy = (*ro)->name;
    }
    switch (mode) {
    case DPMSModeOn:
	rout->Output->Power(rout->Output, RHD_POWER_ON);
	rout->Output->Active = TRUE;
	ASSERT(rhdCrtc);
	ASSERT(rhdCrtc == rout->Output->Crtc);
	rout->Crtc = rhdCrtc;
	break;
    case DPMSModeSuspend:
    case DPMSModeStandby:
	if (outUsedBy) {
	    xf86DrvMsg(rhdPtr->scrnIndex, X_WARNING,
		   "RandR: While resetting %s: output %s is also used "
		   "by %s - ignoring\n",
		   out->name, rout->Output->Name, outUsedBy);
	    break;
	}
	rout->Output->Power(rout->Output, RHD_POWER_RESET);
	rout->Output->Active = FALSE;
	rout->Crtc = NULL;
	break;
    case DPMSModeOff:
	if (outUsedBy) {
	    xf86DrvMsg(rhdPtr->scrnIndex, X_WARNING,
		   "RandR: While switching off %s: output %s is also used "
		   "by %s - ignoring\n",
		   out->name, rout->Output->Name, outUsedBy);
	    break;
	}
	rout->Output->Power(rout->Output, RHD_POWER_SHUTDOWN);
	rout->Output->Active = FALSE;
	rout->Crtc = NULL;
	break;
    default:
	ASSERT(!"Unknown DPMS mode");
    }
    RHDDebugRandrState(rhdPtr, "POST-OutputDpms");
}

/* RandR has a weird sense of how validation and fixup should be handled:
 * - Fixup and validation can interfere, they would have to be looped
 * - Validation should be done after fixup
 * - There is no crtc validation AT ALL
 * - The necessity of adjusted_mode is questionable
 * - Outputs and crtcs are checked independently, and one at a time.
 *   This can interfere, the driver should know the complete setup for
 *   validation and fixup.
 *   We also cannot emulate, because we never know when validation is finished.
 * Therefore we only use a single function for all and have to work around
 * not knowing what the other crtcs might be needed for. */
static int
rhdRROutputModeValid(xf86OutputPtr  out,
		     DisplayModePtr OrigMode)
{
    RHDPtr             rhdPtr = RHDPTR(out->scrn);
    rhdRandrOutputPtr  rout   = (rhdRandrOutputPtr) out->driver_private;
    DisplayModePtr     Mode   = xf86DuplicateMode(OrigMode);
    int                Status;

    RHDFUNC(rhdPtr);

    /* RandR may give us a mode without a name... (xf86RandRModeConvert)
     * xf86DuplicateMode should fill it up, though */
    if (!Mode->name)
	Mode->name = xstrdup("n/a");

    RHDDebug(rhdPtr->scrnIndex, "%s: Output %s : %s\n", __func__,
	     rout->Name, Mode->name);
    if (rhdPtr->verbosity >= LOG_DEBUG)
	RHDPrintModeline(Mode);
    ASSERT(rout->Connector);
    ASSERT(rout->Output);

    /* If out->crtc is not NULL, it is not necessarily the Crtc that will
     * be used, so let's better skip crtc based checks... */
    /* Monitor is handled by RandR */
    if (!rout->Output->Connector)
	return MODE_OUTPUT_UNDEF;
    Status = RHDRRModeFixup(out->scrn, Mode, NULL, rout->Connector,
			    rout->Output, NULL, rout->ScaledToMode ? TRUE : FALSE);
    RHDDebug(rhdPtr->scrnIndex, "%s: %s: %s\n", __func__,
	     Mode->name, RHDModeStatusToString(Status));
    xfree(Mode->name);
    xfree(Mode);
    return Status;
}

static void
rhdRRModeCopy(DisplayModePtr  OrigMode, DisplayModePtr Mode)
{
    memset(Mode, 0, sizeof(DisplayModeRec));
    Mode->name       = xstrdup(OrigMode->name ? OrigMode->name : "n/a");
    Mode->status     = OrigMode->status;
    Mode->type       = OrigMode->type;
    Mode->Clock      = OrigMode->Clock;
    Mode->HDisplay   = OrigMode->HDisplay;
    Mode->HSyncStart = OrigMode->HSyncStart;
    Mode->HSyncEnd   = OrigMode->HSyncEnd;
    Mode->HTotal     = OrigMode->HTotal;
    Mode->HSkew      = OrigMode->HSkew;
    Mode->VDisplay   = OrigMode->VDisplay;
    Mode->VSyncStart = OrigMode->VSyncStart;
    Mode->VSyncEnd   = OrigMode->VSyncEnd;
    Mode->VTotal     = OrigMode->VTotal;
    Mode->VScan      = OrigMode->VScan;
    Mode->Flags      = OrigMode->Flags;

    if ((Mode->type & M_T_CRTC_C) == M_T_BUILTIN) {
	Mode->CrtcHDisplay = OrigMode->CrtcHDisplay;
	Mode->CrtcHBlankStart = OrigMode->CrtcHBlankStart;
	Mode->CrtcHSyncStart = OrigMode->CrtcHSyncStart;
	Mode->CrtcHBlankEnd = OrigMode->CrtcHBlankEnd;
	Mode->CrtcHSyncEnd = OrigMode->CrtcHSyncEnd;
	Mode->CrtcHTotal = OrigMode->CrtcHTotal;
	Mode->CrtcVDisplay = OrigMode->CrtcVDisplay;
	Mode->CrtcVBlankStart = OrigMode->CrtcVBlankStart;
	Mode->CrtcVSyncStart = OrigMode->CrtcVSyncStart;
	Mode->CrtcVSyncEnd = OrigMode->CrtcVSyncEnd;
	Mode->CrtcVBlankEnd = OrigMode->CrtcVBlankEnd;
	Mode->CrtcVTotal = OrigMode->CrtcVTotal;
    }
}

/*
 *
 */
static void
rhdRRSanitizeMode(DisplayModePtr Mode)
{
    if (!Mode->name)
	Mode->name = xstrdup("n/a");
    Mode->status = MODE_OK;
    if ((Mode->type & M_T_CRTC_C) != M_T_BUILTIN) {
	Mode->CrtcHDisplay = Mode->CrtcHBlankStart =
	    Mode->CrtcHSyncStart = Mode->CrtcHBlankEnd =
	    Mode->CrtcHSyncEnd = Mode->CrtcHTotal = 0;
	Mode->CrtcVDisplay = Mode->CrtcVBlankStart =
	    Mode->CrtcVSyncStart = Mode->CrtcVSyncEnd =
	    Mode->CrtcVBlankEnd = Mode->CrtcVTotal = 0;
	Mode->SynthClock = 0;
    }
}

/*
 *
 */
static void
rhdRRFreeOutputs( RHDPtr rhdPtr)
{
    xf86OutputPtr *ro;
    struct rhdOutput *Output;
    /*
     * modesUpdated indicates if we are entering here for a first time after a
     * OutputsModeValid(). In this case a new mode setting round starts, thus
     * we need to free all outputs so that all inactive outputs are freed.
     * We later on allocate each output.
     */
    for (Output = rhdPtr->Outputs; Output; Output = Output->Next) {
	if (Output->AllocFree) {
	    for (ro = rhdPtr->randr->RandrOutput; *ro; ro++) {
		rhdRandrOutputPtr o = (rhdRandrOutputPtr) (*ro)->driver_private;
		if (o->Output == Output) {
		    if (!(*ro)->crtc) {
			Output->AllocFree(Output, RHD_OUTPUT_FREE);
			RHDDebug(rhdPtr->scrnIndex, "%s: Freeing Output: %s\n",__func__,
				 Output->Name);
		    }
		}
	    }
	}
    }
}

/* The crtc is only known on fixup time. Now it's actually to late to reject a
 * mode and give a reasonable answer why (return is bool), but we'll better not
 * set a mode than scrap our hardware */
static Bool
rhdRROutputModeFixup(xf86OutputPtr  out,
		     DisplayModePtr OrigMode,
		     DisplayModePtr Mode)
{
    RHDPtr             rhdPtr = RHDPTR(out->scrn);
    rhdRandrOutputPtr  rout   = (rhdRandrOutputPtr) out->driver_private;
    struct rhdCrtc    *rhdCrtc   = NULL;
    int                Status;
    DisplayModePtr     DisplayedMode;
    Bool               Scaled = FALSE;

    RHDFUNC(rhdPtr);
    ASSERT(out->crtc);
    rhdCrtc = ((struct rhdRandrCrtc *)(out->crtc->driver_private))->rhdCrtc;

    rhdRRFreeOutputs(rhdPtr);

    xfree(Mode->name);
    if (rout->ScaledToMode) {
	DisplayModePtr tmp = RHDModeCopy(rout->ScaledToMode);
	/* validate against CRTC. */
	if ((Status = RHDValidateScaledToMode(rhdCrtc, tmp))!= MODE_OK) {
	    RHDDebug(rhdPtr->scrnIndex, "%s: %s ScaledToMode INVALID: [0x%x] %s\n", __func__,
		     tmp->name, Status, RHDModeStatusToString(Status));
	    xfree(tmp);
	    return FALSE; /* failing here doesn't help */
	}
	memcpy(Mode, tmp, sizeof(DisplayModeRec));
	Mode->name = xstrdup(tmp->name);
	Mode->prev = Mode->next = NULL;
	xfree(tmp->name);
	xfree(tmp);

        /* sanitize OrigMode */
	rhdRRSanitizeMode(OrigMode);
	DisplayedMode = OrigMode;
	rhdCrtc->ScaledToMode = Mode;
	Scaled = TRUE;
    } else {
	/* !@#$ xf86RandRModeConvert doesn't initialize Mode with 0
	 * Fixed in xserver git c6c284e6 */
	rhdRRModeCopy(OrigMode, Mode);

	DisplayedMode = Mode;
    }

    /* RHDRRModeFixup will set up the remaining bits */

    RHDDebug(rhdPtr->scrnIndex, "%s: Output %s : %s\n", __func__,
	     rout->Name, Mode->name);
    ASSERT(rout->Connector);
    ASSERT(rout->Output);

    setupCrtc(rhdPtr, rhdCrtc);

    /* Monitor is handled by RandR */
    if (!rout->Output->AllocFree || rout->Output->AllocFree(rout->Output, RHD_OUTPUT_ALLOC)) {
	Status = RHDRRModeFixup(out->scrn, DisplayedMode, rhdCrtc, rout->Connector,
				rout->Output, NULL, Scaled);
    } else
	Status = MODE_NO_ENCODER;

    if (Status != MODE_OK) {
	rout->OutputActive = FALSE;
	RHDDebug(rhdPtr->scrnIndex, "%s: %s FAILED: [0x%x] %s\n", __func__,
		 Mode->name, Status, RHDModeStatusToString(Status));
	return FALSE;
    }
    rout->OutputActive = TRUE;
    return TRUE;
}

/* Set output mode.
 * Again, randr calls for Crtc and Output separately, while order should be
 * up to the driver. There wouldn't be a need for prepare/set/commit then.
 * We cannot do everything in OutputModeSet, because the viewport isn't known
 * here. */
static void
rhdRROutputPrepare(xf86OutputPtr out)
{
    RHDPtr            rhdPtr = RHDPTR(out->scrn);
    ScrnInfoPtr       pScrn  = xf86Screens[rhdPtr->scrnIndex];
    rhdRandrOutputPtr rout   = (rhdRandrOutputPtr) out->driver_private;

    RHDFUNC(rhdPtr);
    pScrn->vtSema = TRUE;

    /* no active output == no mess */
    rout->Output->Power(rout->Output, RHD_POWER_RESET);
    rout->Output->Crtc = rout->Crtc = NULL;
}
static void
rhdRROutputModeSet(xf86OutputPtr  out,
		   DisplayModePtr OrigMode, DisplayModePtr Mode)
{
    RHDPtr            rhdPtr = RHDPTR(out->scrn);
    rhdRandrOutputPtr rout   = (rhdRandrOutputPtr) out->driver_private;
    struct rhdCrtc   *rhdCrtc   = ((struct rhdRandrCrtc *)(out->crtc->driver_private))->rhdCrtc;

    RHDFUNC(rhdPtr);

    /* RandR may give us a mode without a name... (xf86RandRModeConvert) */
    if (!Mode->name && out->crtc->mode.name)
	Mode->name = xstrdup(out->crtc->mode.name);

    RHDDebug(rhdPtr->scrnIndex, "%s: Output %s : %s to %s\n", __func__,
	     rout->Name, Mode->name,
	     rhdCrtc->Name);

    /* RandR might want to set up several outputs (RandR speech) with different
     * crtcs, while the outputs differ in fact only by the connector and thus
     * cannot be used by different crtcs */
    if (rout->Crtc && rout->Crtc != rhdCrtc)
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
		   "RandR: Output %s has already CRTC attached - "
		   "assuming ouput/connector clash\n", rout->Name);
    /* Set up mode */
    rout->Crtc = rhdCrtc;
    ASSERT(rhdCrtc == rout->Output->Crtc);
    rout->Output->Mode(rout->Output, Mode);
}
static void
rhdRROutputCommit(xf86OutputPtr out)
{
    RHDPtr            rhdPtr = RHDPTR(out->scrn);
    rhdRandrOutputPtr rout   = (rhdRandrOutputPtr) out->driver_private;
    const char       *val;
    char              buf[32];
    struct rhdCrtc   *rhdCrtc   = ((struct rhdRandrCrtc *)(out->crtc->driver_private))->rhdCrtc;

    RHDFUNC(rhdPtr);
    ASSERT(rhdCrtc == rout->Output->Crtc);

    rout->Output->Active    = TRUE;
    rout->Output->Connector = rout->Connector; /* @@@ */
    rout->Output->Power(rout->Output, RHD_POWER_ON);

    /* Some outputs may have physical protocol changes (e.g. TV) */
    val = rhdGetSignalFormat(rout);
    RRChangeOutputProperty(out->randr_output, atomConnectorType,
			   XA_STRING, 8, PropModeReplace,
			   strlen(val), (char *) val, TRUE, FALSE);
    /* Should be a crtc property */
    if (rhdCrtc->MaxX > rhdCrtc->MinX && rhdCrtc->MaxY > rhdCrtc->MinY)
	sprintf(buf, "%dx%d+%d+%d", rhdCrtc->MaxX - rhdCrtc->MinX,
		rhdCrtc->MaxY - rhdCrtc->MinY, rhdCrtc->MinX, rhdCrtc->MinY);
    else
	buf[0] = 0;
    RRChangeOutputProperty(out->randr_output, atomPanningArea,
			   XA_STRING, 8, PropModeReplace,
			   strlen(buf), buf, TRUE, FALSE);

    RHDDebugRandrState(rhdPtr, rout->Name);
}

/*
 * This function looks for other outputs on the connector rout is connected to.
 * If one of those outputs can be sensed and is sensed the function will return
 * one of those.
 */
static rhdRandrOutputPtr
rhdRROtherOutputOnConnectorHelper(RHDPtr rhdPtr, rhdRandrOutputPtr rout)
{
    xf86OutputPtr    *ro;

    for (ro = rhdPtr->randr->RandrOutput; *ro; ro++) {
	rhdRandrOutputPtr o =
	    (rhdRandrOutputPtr) (*ro)->driver_private;
	if (o != rout &&
	    o->Connector == rout->Connector &&
	    o->Output->Sense) {
	    /* Yes, this looks wrong, but is correct */
	    enum rhdSensedOutput SensedType =
		o->Output->Sense(o->Output, o->Connector);
	    if (SensedType != RHD_SENSED_NONE) {
		RHDOutputPrintSensedType(o->Output);
		return o;
	    }
	}
    }
    return NULL;
}

/* Probe for a connected output. */
static xf86OutputStatus
rhdRROutputDetect(xf86OutputPtr output)
{
    RHDPtr            rhdPtr = RHDPTR(output->scrn);
    rhdRandrOutputPtr rout   = (rhdRandrOutputPtr) output->driver_private;

    RHDDebug(rhdPtr->scrnIndex, "%s: Output %s\n", __func__, rout->Name);

    /* Assume that a panel is always connected */
    if (rout->Connector->Type == RHD_CONNECTOR_PANEL) {
	rout->Output->Connector = rout->Connector; /* @@@ */
	return XF86OutputStatusConnected;
    } else if (rout->Connector->Type ==  RHD_CONNECTOR_TV) /* until TV_OUT is fixed we bail here */
	return XF86OutputStatusDisconnected;

    if (rout->Connector->HPDCheck) {
	/* Hot Plug Detection available, use it */
	if (rout->Connector->HPDCheck(rout->Connector)) {
	    /*
	     * HPD returned true
	     */
	    if (rout->Output->Sense) {
		if ((rout->Output->SensedType
		     = rout->Output->Sense(rout->Output,
					   rout->Connector)) != RHD_SENSED_NONE) {
		    RHDOutputPrintSensedType(rout->Output);
		    rout->Output->Connector = rout->Connector; /* @@@ */
		    return XF86OutputStatusConnected;
		} else
		    return XF86OutputStatusDisconnected;
	    } else {
		/* HPD returned true, but no Sense() available
		 * Typically the case on TMDSB.
		 * Check if there is another output attached to this connector
		 * and use Sense() on that one to verify whether something
		 * is attached to this one */
		if (rhdRROtherOutputOnConnectorHelper(rhdPtr, rout))
		    return XF86OutputStatusDisconnected;
		rout->Output->Connector = rout->Connector; /* @@@ */
		return XF86OutputStatusConnected;
	    }
	} else {
	    /*
	     * HPD returned false
	     */
	    /* There is the infamous DMS-59 connector, on which HPD returns
	     * false when 'only' VGA is connected. */
	    if (rhdPtr->Card && (rhdPtr->Card->flags & RHD_CARD_FLAG_DMS59)) {
		xf86DrvMsg(rhdPtr->scrnIndex, X_INFO,
			   "RandR: Verifying state of DMS-59 VGA connector.\n");
		if (rout->Output->Sense) {
		    rout->Output->SensedType
			= rout->Output->Sense(rout->Output,
					      rout->Connector);
		    if (rout->Output->SensedType != RHD_SENSED_NONE) {
			rout->Output->Connector = rout->Connector; /* @@@ */
			RHDOutputPrintSensedType(rout->Output);
			return XF86OutputStatusConnected;
		    }
		}
	    }
	    return XF86OutputStatusDisconnected;
	}
    } else {
	/*
	 * No HPD available, Sense() if possible
	 */
	if (rout->Output->Sense) {
	    rout->Output->SensedType
		= rout->Output->Sense(rout->Output, rout->Connector);
	    if (rout->Output->SensedType != RHD_SENSED_NONE) {
		    rout->Output->Connector = rout->Connector; /* @@@ */
		    RHDOutputPrintSensedType(rout->Output);
		    return XF86OutputStatusConnected;
	    } else
		return XF86OutputStatusDisconnected;
	}
	/* Use DDC address probing if possible otherwise */
	if (rout->Connector->DDC) {
	    RHDI2CDataArg i2cRec;
	    i2cRec.probe.slave = 0xa0;
	    i2cRec.probe.i2cBusPtr = rout->Connector->DDC;
	    if (RHDI2CFunc(rhdPtr->scrnIndex, rhdPtr->I2C,RHD_I2C_PROBE_ADDR,&i2cRec)
		== RHD_I2C_SUCCESS) {
		rhdRandrOutputPtr rout_tmp;
		RHDDebug(rout->Output->scrnIndex, "DDC Probing for Output %s returned connected\n",
			 rout->Output->Name);
		if ((rout_tmp = rhdRROtherOutputOnConnectorHelper(rhdPtr, rout))) {
		RHDDebug(rout->Output->scrnIndex, "Output %s on same connector already connected\n",
			 rout_tmp->Output->Name);
		    return XF86OutputStatusDisconnected;
		}
		rout->Output->Connector = rout->Connector; /* @@@ */
		return XF86OutputStatusConnected;
	    } else {
		RHDDebug(rout->Output->scrnIndex, "DDC Probing for Output %s returned disconnected\n",
			 rout->Output->Name);
		return XF86OutputStatusDisconnected;
	    }
	}
	rout->Output->Connector = rout->Connector; /* @@@ */
	return XF86OutputStatusUnknown;
    }
}

/*
 * RHDRRMonitorInit(): adds synthesized modes for scaling to list generated by RHDMonitorInit()
 * as with RandR this is the only chance we have access to the full modes list.
 */
static struct rhdMonitor *
RHDRRMonitorInit(struct rhdConnector *Connector)
{
    struct rhdMonitor *m = RHDMonitorInit(Connector);

    RHDFUNC(Connector);
    if (RHDScalePolicy(m, Connector))
	RHDSynthModes(Connector->scrnIndex, m->Modes);

    return m;
}

/* Query the device for the modes it provides. Set MonInfo, mm_width/height. */
static DisplayModePtr
rhdRROutputGetModes(xf86OutputPtr output)
{
    RHDPtr            rhdPtr = RHDPTR(output->scrn);
    rhdRandrOutputPtr rout = (rhdRandrOutputPtr) output->driver_private;
    struct rhdOutput  *o;

    RHDDebug(rhdPtr->scrnIndex, "%s: Output %s\n", __func__, rout->Name);
    /* TODO: per-output options ForceReduced & UseXF86Edid */

    /* Nuke old monitor */
    if (rout->Connector->Monitor) {
	/* EDID is already freed by RandR (OutputSetEDID+return) */
	rout->Connector->Monitor->EDID = NULL;
	RHDMonitorDestroy(rout->Connector->Monitor);
    }

    /* Get new one */
    if (! (rout->Connector->Monitor = RHDRRMonitorInit(rout->Connector)) ) {
	xf86OutputSetEDID (output, NULL);
	return NULL;
    }

    ASSERT(rout->Output);
    o = rout->Output;

    if (RHDScalePolicy(rout->Connector->Monitor, rout->Connector)) {
	if (o->Connector->Monitor) {
	    rout->ScaledToMode = RHDModeCopy(o->Connector->Monitor->NativeMode);
	    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "Found native mode: ");
	    RHDPrintModeline(rout->ScaledToMode);
	    if (RHDRRValidateScaledToMode(rout->Output, rout->ScaledToMode) != MODE_OK) {
		xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "Native mode doesn't validate: deleting\n");
		xfree(rout->ScaledToMode->name);
		xfree(rout->ScaledToMode);
		rout->ScaledToMode = NULL;
	    }
	}
    } else
	rout->ScaledToMode = NULL;

    /* If digitally attached, enable reduced blanking */
    if (rout->Output->Id == RHD_OUTPUT_TMDSA ||
	rout->Output->Id == RHD_OUTPUT_LVTMA ||
	rout->Output->Id == RHD_OUTPUT_KLDSKP_LVTMA ||
	rout->Output->Id == RHD_OUTPUT_UNIPHYA ||
	rout->Output->Id == RHD_OUTPUT_UNIPHYB ||
	rout->Output->Id == RHD_OUTPUT_UNIPHYC ||
	rout->Output->Id == RHD_OUTPUT_UNIPHYD ||
	rout->Output->Id == RHD_OUTPUT_UNIPHYE ||
	rout->Output->Id == RHD_OUTPUT_UNIPHYF
	)
	rout->Connector->Monitor->ReducedAllowed = TRUE;
    /* Allow user overrides */
    if (rhdPtr->forceReduced.set)
	rout->Connector->Monitor->ReducedAllowed =
	    rhdPtr->forceReduced.val.bool;

    xf86OutputSetEDID (output, rout->Connector->Monitor->EDID);

    /* If no EDID data is available, calculate mm_size by assuming 96dpi
     * for the preferred (or first if no preferred) mode */
    if (! rout->Connector->Monitor->EDID) {
	DisplayModePtr m;
	for (m = rout->Connector->Monitor->Modes; m; m = m->next)
	    if (m->type & M_T_PREFERRED)
		break;
	if (! m)
	    m = rout->Connector->Monitor->Modes;
	if (m) {
#define DEFAULT_MM_PER_DOT (25.4 / 96)
	    output->mm_width  = m->HDisplay * DEFAULT_MM_PER_DOT;
	    output->mm_height = m->VDisplay * DEFAULT_MM_PER_DOT;
	    xf86DrvMsg(rhdPtr->scrnIndex, X_WARNING,
		       "No monitor size info, assuming 96dpi.\n");
	}
    }
    RHDDebug(rhdPtr->scrnIndex, "%s: Adding Output Modes:\n",__func__);
    if (rhdPtr->verbosity >= 7) {
	DisplayModePtr mode =  rout->Connector->Monitor->Modes;
	while (mode) {
	    RHDPrintModeline(mode);
	    mode = mode->next;
	}
    }
    return xf86DuplicateModes(output->scrn, rout->Connector->Monitor->Modes);
}

/* An output's property has changed. */
static Bool
rhdRROutputSetProperty(xf86OutputPtr out, Atom property,
		       RRPropertyValuePtr value)
{
    RHDPtr            rhdPtr = RHDPTR(out->scrn);
    rhdRandrOutputPtr rout   = (rhdRandrOutputPtr) out->driver_private;

    RHDFUNC(rhdPtr);

    if (property == atomPanningArea) {
	int w = 0, h = 0, x = 0, y = 0;
	struct rhdCrtc *Crtc = rout->Output->Crtc;
	int i;

	if (!Crtc)
	    return FALSE;
	for (i = 0; i < 2; i++) {
	    xf86CrtcPtr crtc = (xf86CrtcPtr) rhdPtr->randr->RandrCrtc[i];
	    if (Crtc == ((struct rhdRandrCrtc *)crtc->driver_private)->rhdCrtc) {
		/* Don't allow panning while rotated */
		if (crtc->rotation != RR_Rotate_0)
		    return FALSE;
		else
		    break;
	    }
	}
	if (value->type != XA_STRING || value->format != 8)
	    return FALSE;
	switch (sscanf(value->data, "%dx%d+%d+%d", &w, &h, &x, &y)) {
	case 0:
	case 2:
	case 4:
	    if (w < 0 || h < 0 || x < 0 || y < 0 ||
		x+w > Crtc->Width || y+h > Crtc->Height)
		return FALSE;
	    Crtc->MinX = x;
	    Crtc->MinY = y;
	    Crtc->MaxX = x + w;
	    Crtc->MaxY = y + h;
	    rhdUpdateCrtcPos(rhdPtr, Crtc, Crtc->Cursor->X, Crtc->Cursor->Y);
	    RHDDebug(rhdPtr->scrnIndex, "%s: PanningArea %d/%d - %d/%d\n",
		     x, y, x+w, y+h);
	    return TRUE;
	default:
	    return FALSE;
	}
    } else if (property == atomBacklight) {
	if (value->type != XA_INTEGER || value->format != 32) {
	    xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: wrong value\n", __func__);
	    return FALSE;
	}
	if (rout->Output->Property) {
	    union rhdPropertyData val;
	    val.integer = *(int*)(value->data);
	    return rout->Output->Property(rout->Output, rhdPropertySet,
					  RHD_OUTPUT_BACKLIGHT, &val);
	}
	return FALSE;
    } else if (property == atomCoherent) {
	if (value->type != XA_INTEGER || value->format != 32) {
	    xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: wrong value\n", __func__);
	    return FALSE;
	}
	if (rout->Output->Property) {
	    union rhdPropertyData val;
	    val.Bool = *(int*)(value->data);
	    return rout->Output->Property(rout->Output, rhdPropertySet,
					  RHD_OUTPUT_COHERENT, &val);
	}
	return FALSE;
    }

    return FALSE;	/* Others are not mutable */
}

/*
 *
 */
static void *
rhdRRCrtcShadowAllocate(xf86CrtcPtr crtc, int Width, int Height)
{
    ScrnInfoPtr	      pScrn = crtc->scrn;
    RHDPtr            rhdPtr = RHDPTR(crtc->scrn);
    ScreenPtr         pScreen = screenInfo.screens[pScrn->scrnIndex];
    struct rhdRandrCrtc *rhdRRCrtc = (struct rhdRandrCrtc*) crtc->driver_private;
    int		      OctPerPixel = pScrn->bitsPerPixel >> 3;
    int               Size = (pScrn->displayWidth * OctPerPixel) * Height;

    if (rhdPtr->AccelMethod == RHD_ACCEL_SHADOWFB
	|| rhdPtr->AccelMethod == RHD_ACCEL_NONE)
	return NULL;

#ifdef USE_EXA
    if (rhdPtr->AccelMethod == RHD_ACCEL_EXA) {

	ASSERT(rhdRRCrtc->u.MemEXA == NULL);

	rhdRRCrtc->u.MemEXA = exaOffscreenAlloc(pScreen, Size, 4096,
						TRUE, NULL, NULL);
	if (rhdRRCrtc->u.MemEXA == NULL) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Unable to allocate shadow memory for rotated CRTC\n");
	    return NULL;
	}
	return ((char *)rhdPtr->FbBase
		+ rhdRRCrtc->u.MemEXA->offset);
    }

#endif /* USE_EXA */
    if (rhdPtr->AccelMethod == RHD_ACCEL_XAA) {
	int Align = (4096 + OctPerPixel - 1) / OctPerPixel;
	Size = (Size + OctPerPixel - 1) / OctPerPixel;

	ASSERT(rhdRRCrtc->u.MemXAA == NULL);

	rhdRRCrtc->u.MemXAA =
	    xf86AllocateOffscreenLinear(pScreen, Size, Align,  /* @@@ */
					       NULL, NULL, NULL);
	if (rhdRRCrtc->u.MemXAA == NULL) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Unable to allocate shadow memory for rotated CRTC\n");
	    return NULL;
	}

	return  ((char *)rhdPtr->FbBase
		 + rhdPtr->FbScanoutStart
		 + rhdRRCrtc->u.MemXAA->offset * OctPerPixel);
    }

    return NULL;
}

/*
 *
 */
static PixmapPtr
rhdRRCrtcShadowCreate(xf86CrtcPtr Crtc, void *Data, int Width, int Height)
{
    ScrnInfoPtr pScrn = Crtc->scrn;
    PixmapPtr RPixmap;

    if (!Data)
	Data = rhdRRCrtcShadowAllocate(Crtc, Width, Height);

    RPixmap = GetScratchPixmapHeader(pScrn->pScreen,
					   Width, Height,
					   pScrn->depth,
					   pScrn->bitsPerPixel,
					   pScrn->displayWidth * pScrn->bitsPerPixel >> 3,
					   Data);

    if (RPixmap == NULL)
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Unable to allocate shadow pixmap for rotated CRTC\n");

    return RPixmap;
}

/*
 *
 */
static void
rhdRRCrtcShadowDestroy(xf86CrtcPtr crtc, PixmapPtr RPixmap, void *Data)
{

    ScrnInfoPtr pScrn = crtc->scrn;
    RHDPtr rhdPtr = RHDPTR(crtc->scrn);
    struct rhdRandrCrtc *rhdRRCrtc = (struct rhdRandrCrtc*) crtc->driver_private;

    if (RPixmap)
	FreeScratchPixmapHeader(RPixmap);

    if (Data) {
#ifdef USE_EXA
	if (rhdPtr->AccelMethod == RHD_ACCEL_EXA) {
	    exaOffscreenFree(pScrn->pScreen, rhdRRCrtc->u.MemEXA);
	    rhdRRCrtc->u.MemEXA = NULL;
	}

#endif /* USE_EXA */
	if (rhdPtr->AccelMethod == RHD_ACCEL_XAA) {
	    xf86FreeOffscreenLinear(rhdRRCrtc->u.MemXAA);
	    rhdRRCrtc->u.MemXAA = NULL;
	}
    }
}


#ifdef RANDR_13_INTERFACE
static Bool
rhdRROutputGetProperty(xf86OutputPtr out, Atom property)
{
    RHDPtr rhdPtr          = RHDPTR(out->scrn);
    rhdRandrOutputPtr rout = (rhdRandrOutputPtr) out->driver_private;
    int err = BadValue;
    union rhdPropertyData val;

    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "In %s\n", __func__);

    if (property == atomBacklight) {
	if (rout->Output->Property == NULL)
	    return FALSE;

	if (!rout->Output->Property(rout->Output, rhdPropertyGet,
				    RHD_OUTPUT_BACKLIGHT, &val))
	    return FALSE;
	err = RRChangeOutputProperty(out->randr_output, atomBacklight,
				     XA_INTEGER, 32, PropModeReplace,
				     1, &val.integer, FALSE, FALSE);

    } else if (property == atomCoherent) {
	if (rout->Output->Property == NULL)
	    return FALSE;

	if (!rout->Output->Property(rout->Output, rhdPropertyGet,
				    RHD_OUTPUT_COHERENT, &val))
	    return FALSE;

	err = RRChangeOutputProperty(out->randr_output, atomCoherent,
				     XA_INTEGER, 32, PropModeReplace,
				     1, &val.Bool, FALSE, FALSE);
    }

    if (err != 0) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "In %s RRChangeOutputProperty error: %d\n", __func__, err);
	return FALSE;
    }
    return TRUE;
}

#endif

/*
 *
 */
Bool
RHDRRInitCursor(ScreenPtr pScreen)
{
    RHDFUNCI(pScreen->myNum);

    /* still need to alloc fb mem for cursors */
    return xf86_cursors_init(pScreen, MAX_CURSOR_WIDTH, MAX_CURSOR_HEIGHT,
			     HARDWARE_CURSOR_TRUECOLOR_AT_8BPP
			     | HARDWARE_CURSOR_UPDATE_UNHIDDEN
			     | HARDWARE_CURSOR_AND_SOURCE_WITH_MASK
			     | HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_1
			     | HARDWARE_CURSOR_ARGB);
}

/*
 *
 */
static void
rhdRRShowCursor(xf86CrtcPtr crtc)
{
    struct rhdCrtc *rhdCrtc = ((struct rhdRandrCrtc *)crtc->driver_private)->rhdCrtc;
    rhdCrtcShowCursor(rhdCrtc);
}

/*
 *
 */
static void
rhdRRHideCursor(xf86CrtcPtr crtc)
{
    struct rhdCrtc *rhdCrtc = ((struct rhdRandrCrtc *)crtc->driver_private)->rhdCrtc;
    rhdCrtcHideCursor(rhdCrtc);
}

/*
 *
 */
static void
rhdRRLoadCursorARGB(xf86CrtcPtr crtc, CARD32 *Image)
{
    struct rhdCrtc *rhdCrtc = ((struct rhdRandrCrtc *)crtc->driver_private)->rhdCrtc;
    rhdCrtcLoadCursorARGB(rhdCrtc, Image);
}

/*
 *
 */
static void
rhdRRSetCursorColors(xf86CrtcPtr crtc, int bg, int fg)
{
    struct rhdCrtc *rhdCrtc = ((struct rhdRandrCrtc *)crtc->driver_private)->rhdCrtc;
    rhdCrtcSetCursorColors(rhdCrtc, bg, fg);
}

/*
 *
 */
static void
rhdRRSetCursorPosition(xf86CrtcPtr crtc, int x, int y)
{
    struct rhdCrtc *rhdCrtc = ((struct rhdRandrCrtc *)crtc->driver_private)->rhdCrtc;
    /*
     * Given cursor pos is always relative to frame - make absolute
     * NOTE: This is hardware specific, it doesn't really fit here,
     * but it's the only place where the relevant information is
     * available.
     */
    if (!crtc->rotatedData) {
	x += crtc->x;
	y += crtc->y;
    }
    rhdCrtcSetCursorPosition(rhdCrtc, x, y);
}

/*
 * Xorg Interface
 */

static const xf86CrtcConfigFuncsRec rhdRRCrtcConfigFuncs = {
    rhdRRXF86CrtcResize
};

static xf86CrtcFuncsRec rhdRRCrtcFuncs = {
    rhdRRCrtcDpms,
    NULL, NULL,						/* Save,Restore */
    rhdRRCrtcLock, rhdRRCrtcUnlock,
    rhdRRCrtcModeFixupDUMMY,
    rhdRRCrtcPrepare, rhdRRCrtcModeSet, rhdRRCrtcCommit,
    rhdRRCrtcGammaSet,
    rhdRRCrtcShadowAllocate, rhdRRCrtcShadowCreate, rhdRRCrtcShadowDestroy,
    rhdRRSetCursorColors, rhdRRSetCursorPosition, rhdRRShowCursor, rhdRRHideCursor,
    NULL, rhdRRLoadCursorARGB, NULL
    /* SetCursorColors,SetCursorPosition,ShowCursor,HideCursor,
     * LoadCursorImage,LoadCursorArgb,CrtcDestroy */
#ifdef XF86CRTCFUNCS_HAS_SETMODEMAJOR
    /* set_mode_major */
    , NULL
#endif
#if XF86_CRTC_VERSION >= 2
    /* set_origin */
    , rhdRRCrtcSetOrigin
#endif
};

/*
 *
 */
void
RHDRRFreeShadow(ScrnInfoPtr pScrn)
{
#ifndef HAVE_FREE_SHADOW
    int i;
    xf86CrtcConfigPtr   CrtcConfig = XF86_CRTC_CONFIG_PTR(pScrn);

    for (i = 0; i < CrtcConfig->num_crtc; i++) {
	xf86CrtcPtr Crtc = CrtcConfig->crtc[i];
	if (Crtc->rotatedPixmap || Crtc->rotatedData) {
	    Crtc->funcs->shadow_destroy(Crtc, Crtc->rotatedPixmap,
					Crtc->rotatedData);
	    Crtc->rotatedPixmap = NULL;
	    Crtc->rotatedData = NULL;
	}
    }
#else
    xf86RotateFreeShadow(pScrn);
#endif
}

static const xf86OutputFuncsRec rhdRROutputFuncs = {
    rhdRROutputCreateResources, rhdRROutputDpms,
    NULL, NULL,						/* Save,Restore */
    rhdRROutputModeValid, rhdRROutputModeFixup,
    rhdRROutputPrepare, rhdRROutputCommit,
    rhdRROutputModeSet, rhdRROutputDetect, rhdRROutputGetModes,
#ifdef RANDR_12_INTERFACE
    rhdRROutputSetProperty,		       /* Only(!) RANDR_12_INTERFACE */
#endif
#ifdef RANDR_13_INTERFACE
    rhdRROutputGetProperty,  /* get_property */
#endif
#ifdef RANDR_GET_CRTC_INTERFACE
    NULL,
#endif
    NULL						/* Destroy */
};


/* Helper: Create rhdRandrOutput with unique name per Connector/Output combo */
static rhdRandrOutputPtr
createRandrOutput(ScrnInfoPtr pScrn,
		  struct rhdConnector *conn, struct rhdOutput *out)
{
    rhdRandrOutputPtr rro;
    char *c;

    rro = xnfcalloc(1, sizeof(rhdRandrOutputRec));
    rro->Connector = conn;
    rro->Output    = out;
    sprintf(rro->Name, "%.30s", conn->Name);
    for (c = rro->Name; *c; c++)
	if (isspace(*c))
	    *c='_';
    return rro;
}

/* Helper: Consolidate names, improve duplicates */
static void
consolidateRandrOutputNames(rhdRandrOutputPtr *rop, int num)
{
    int i, j, changed;
    const char *outname;
    char *c;
    /* First try to come up with something sensible */
    for (i = 0; i < num; i++) {
	for (j = i+1; j < num; j++)
	    if (strcmp(rop[i]->Name, rop[j]->Name) == 0)
		break;
	if (j < num) {
	    for (j = num-1; j >= i; j--)	/* Include i */
		if (strcmp(rop[i]->Name, rop[j]->Name) == 0) {
		    switch (rop[j]->Output->Id) {
		    case RHD_OUTPUT_DACA:
		    case RHD_OUTPUT_DACB:
			outname = "analog";
			break;
		    case RHD_OUTPUT_TMDSA:
		    case RHD_OUTPUT_LVTMA:
		    case RHD_OUTPUT_KLDSKP_LVTMA:
		    case RHD_OUTPUT_UNIPHYA:
		    case RHD_OUTPUT_UNIPHYB:
		    case RHD_OUTPUT_UNIPHYC:
		    case RHD_OUTPUT_UNIPHYD:
		    case RHD_OUTPUT_UNIPHYE:
		    case RHD_OUTPUT_UNIPHYF:
			outname = "digital";
			break;
		    default:
			outname = rop[j]->Output->Name;
		    }
		    sprintf(rop[j]->Name, "%.30s/%.30s",
			    rop[j]->Connector->Name, outname);
		    for (c = rop[j]->Name; *c; c++)
			if (isspace(*c))
			    *c='_';
		}
	}
    }
    /* This shouldn't happen, but better be safe:
     * fix up every remaining name with the old naming scheme */
    for (i = 0; i < num; i++) {
	changed=0;
	for (j = i+1; j < num; j++)
	    if (strcmp(rop[i]->Name, rop[j]->Name) == 0) {
		changed++;
		sprintf(rop[j]->Name, "%.30s/%.30s",
			rop[j]->Connector->Name, rop[j]->Output->Name);
		for (c = rop[j]->Name; *c; c++)
		    if (isspace(*c))
			*c='_';
	    }
	if (changed) {
	    sprintf(rop[i]->Name, "%.30s/%.30s",
		    rop[i]->Connector->Name, rop[i]->Output->Name);
	    for (c = rop[i]->Name; *c; c++)
		if (isspace(*c))
		    *c='_';
	}
    }
}

/* Helper: Create and set up xf86Output */
static xf86OutputPtr
createXF86Output(ScrnInfoPtr pScrn, rhdRandrOutputPtr rro)
{
    xf86OutputPtr xo;
    xo = xf86OutputCreate(pScrn, &rhdRROutputFuncs, rro->Name);
    ASSERT(xo);
    xo->driver_private = rro;
    xo->possible_crtcs  = ~0;				/* No limitations */
    xo->possible_clones = ~0;				/* No limitations */
    xo->interlaceAllowed = TRUE;
    xo->doubleScanAllowed = TRUE;
    xo->subpixel_order = SubPixelUnknown;
    xo->use_screen_monitor = FALSE;
    return xo;
}

/* Call in PreInit after memory + io has been set up */
Bool
RHDRandrPreInit(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct rhdRandr *randr;
    int i, j, k, numCombined = 0;
    rhdRandrOutputPtr *RandrOutput, *r;
    char *outputorder;

    RHDFUNC(rhdPtr);
    if (rhdPtr->noRandr.val.bool) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "RandR 1.2 support disabled due to configuration\n");
	return FALSE;
    }

    randr = xnfcalloc(1, sizeof(struct rhdRandr));
    xf86CrtcConfigInit(pScrn, &rhdRRCrtcConfigFuncs);
    /* Maximum for D1GRPH_X/Y_END: 8k */
    xf86CrtcSetSizeRange (pScrn, 320, 200, 8000, 8000);

    for (i = 0; i < 2; i++) {
	randr->RandrCrtc[i] = xf86CrtcCreate(pScrn, &rhdRRCrtcFuncs);
	ASSERT(randr->RandrCrtc[i]);
	randr->RandrCrtc[i]->driver_private = xnfcalloc(sizeof(struct rhdRandrCrtc),1);
	((struct rhdRandrCrtc*) randr->RandrCrtc[i]->driver_private)->rhdCrtc = rhdPtr->Crtc[i]; /* TODO: not cleaning up correctly */
    }

    /* First count, then allocate */
    for (i = 0; i < RHD_CONNECTORS_MAX; i++) {
	struct rhdConnector *conn = rhdPtr->Connector[i];
	if (conn)
	    for (j = 0; j < MAX_OUTPUTS_PER_CONNECTOR; j++) {
		struct rhdOutput *out = conn->Output[j];
		if (out)
		    numCombined++;
	    }
    }
    RandrOutput = r =
	xnfcalloc(numCombined+1, sizeof(struct rhdRandrOutputPtr *));

    /* Create combined unique output names */
    for (i = 0; i < RHD_CONNECTORS_MAX; i++) {
	struct rhdConnector *conn = rhdPtr->Connector[i];
	if (conn) {
	    for (j = 0; j < MAX_OUTPUTS_PER_CONNECTOR; j++) {
		struct rhdOutput *out = conn->Output[j];
		if (out)
		    *r++ = createRandrOutput(pScrn, conn, out);
	    }
	}
    }

    /*
     * Each rhdRandrOutputRec carries a list of all connectors this output belongs to.
     * Since the output can only drive one connector but RandR doesn't seem to have
     * any notion of 'shared outputs' we need to probe them all and decide which
     * output that reports connected we pass to RandR as connected.
     */
    for (i = 0; i < numCombined; i++) {
	int cnt = 0;
	for (j = 0; j < RHD_CONNECTORS_MAX; j++) {
	    struct rhdConnector *conn = rhdPtr->Connector[j];
	    if (!conn) continue;
	    for (k = 0; k < MAX_OUTPUTS_PER_CONNECTOR; k++) {
		if (conn->Output[k] == RandrOutput[i]->Output) {
		    if (cnt >= MAX_CONNECTORS_PER_RR_OUTPUT)
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
				   "%s: Number of Connectors for Output %s exceeds %i\n",
				   __func__,RandrOutput[i]->Name, MAX_CONNECTORS_PER_RR_OUTPUT);
		    else
			RandrOutput[i]->AllConnectors[cnt++] = conn;
		    break;
		}
	    }
	}
    }
    consolidateRandrOutputNames(RandrOutput, numCombined);
    for (i = 0; i < numCombined; i++)
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "RandR: Adding RRoutput %s for Output %s\n",
		   RandrOutput[i]->Name, RandrOutput[i]->Output->Name);

    /* Reorder for xinerama screen enumeration if requested */
    outputorder = rhdPtr->rrOutputOrder.val.string;
    if (outputorder && *outputorder) {
    	rhdRandrOutputPtr *RandrOutputReorder = r =
	    xnfcalloc(numCombined+1, sizeof(struct xf86OutputPtr *));
	while (*outputorder) {
	    char *end = strchr(outputorder, ' ');
	    int len = end ? end-outputorder : (signed) strlen(outputorder);
	    end = strchr(outputorder, ',');
	    if (end)
		len = end-outputorder < len ? end-outputorder : len;
	    for (i = 0; i < numCombined; i++) {
		if (RandrOutput[i] &&
		    strncmp(RandrOutput[i]->Name, outputorder, len) == 0 &&
		    RandrOutput[i]->Name[len] == 0) {
		    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			       "RandR: Reordering output %s\n",
			       RandrOutput[i]->Name);
		    *r++ = RandrOutput[i];
		    RandrOutput[i] = NULL;
		}
	    }
	    outputorder += len;
	    while (*outputorder == ' ' || *outputorder == ',')
		outputorder++;
	}
	for (i = 0; i < numCombined; i++)
	    if (RandrOutput[i])
		*r++ = RandrOutput[i];
	ASSERT(r - RandrOutputReorder == numCombined);
	xfree(RandrOutput);
	RandrOutput = RandrOutputReorder;
    }

    /* Initialize RandR structures */
    randr->RandrOutput =
	xnfcalloc(numCombined+1, sizeof(struct xf86OutputPtr *));
    for (i = 0; i < numCombined; i++) {
	randr->RandrOutput[i] = createXF86Output(pScrn, RandrOutput[i]);
    }
    xfree(RandrOutput);
    rhdPtr->randr = randr;

    if (!xf86InitialConfiguration(pScrn, FALSE)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "RandR: No valid modes. Disabling RandR support.\n");
	for (i = 0; i < 2; i++)
	    xfree(randr->RandrCrtc[i]->driver_private);
	xfree(randr);
	rhdPtr->randr = NULL;		/* TODO: not cleaning up correctly */
	return FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "RandR 1.2 support enabled\n");

    /* No docs, but radeon, avivo, and nv drivers calling this
     * after(!) xf86InitialConfiguration */
    if (!xf86RandR12PreInit (pScrn)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "RandR: xf86RandR12PreInit failed. Disabled.\n");
	for (i = 0; i < 2; i++)
	    xfree(randr->RandrCrtc[i]->driver_private);
	xfree(randr);
	rhdPtr->randr = NULL;		/* TODO: not cleaning up correctly */
	return FALSE;
    }

    return TRUE;
}

/* Wrapped PointerMoved for driver-level panning */
static void
rhdRRPointerMoved(int scrnIndex, int x, int y)
{
    ScrnInfoPtr pScrn  = xf86Screens[scrnIndex];
    RHDPtr      rhdPtr = RHDPTR(xf86Screens[scrnIndex]);
    int i;

    for (i = 0; i < 2; i++) {
	struct rhdCrtc *Crtc = rhdPtr->Crtc[i];
	if (Crtc->scrnIndex == scrnIndex && Crtc->Active)
	    rhdUpdateCrtcPos(rhdPtr, Crtc, x + pScrn->frameX0, y + pScrn->frameY0);
    }
    UNWRAP_SCRNINFO(PointerMoved);
    pScrn->PointerMoved(scrnIndex, x, y);
    WRAP_SCRNINFO(PointerMoved, rhdRRPointerMoved);
}

/* Call in ScreenInit after frame buffer + acceleration layers */
Bool
RHDRandrScreenInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RHDPtr rhdPtr = RHDPTR(pScrn);

    RHDFUNC(rhdPtr);

    if (rhdPtr->AccelMethod == RHD_ACCEL_NONE || rhdPtr->AccelMethod == RHD_ACCEL_SHADOWFB) {
	rhdRRCrtcFuncs.shadow_allocate = NULL;
	rhdRRCrtcFuncs.shadow_create = NULL;
	rhdRRCrtcFuncs.shadow_destroy = NULL;
    }

    if (!xf86CrtcScreenInit(pScreen))
	return FALSE;
    /* Wrap cursor for driver-level panning */
    WRAP_SCRNINFO(PointerMoved, rhdRRPointerMoved);

    RHDDebugRandrState(rhdPtr, "POST-ScreenInit");
    return TRUE;
}

/* ModeInit: Set modes according to current layout */
Bool
RHDRandrModeInit(ScrnInfoPtr pScrn)
{
    Bool ret;
    RHDPtr rhdPtr = RHDPTR(pScrn);

    RHDFUNC(rhdPtr);

    /* Stop crap from being shown: gets reenabled through SaveScreen */
    rhdPtr->Crtc[0]->Blank(rhdPtr->Crtc[0], TRUE);
    rhdPtr->Crtc[1]->Blank(rhdPtr->Crtc[1], TRUE);

    RHDPrepareMode(rhdPtr);
    ret = xf86SetDesiredModes(pScrn);
    RHDDebugRandrState(rhdPtr, "POST-ModeInit");

    return ret;
}

/* SwitchMode: Legacy: Set one mode */
Bool
RHDRandrSwitchMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    return xf86SetSingleMode(pScrn, mode, RR_Rotate_0);
}


#else /* RANDR_12_SUPPORT */

Bool
RHDRandrPreInit(ScrnInfoPtr pScrn)
{
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "RandR 1.2 support disabled (not available at compile time)\n");
    return FALSE;
}

Bool
RHDRandrScreenInit(ScreenPtr pScreen)
{ ASSERT(0); return FALSE; }

Bool
RHDRandrModeInit(ScrnInfoPtr pScrn)
{ ASSERT(0); return FALSE; }

Bool
RHDRandrSwitchMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{ ASSERT(0); return FALSE; }


#endif /* RANDR_12_SUPPORT */

