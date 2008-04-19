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

#if (RANDR_MAJOR == 1 && RANDR_MINOR >= 2) || RANDR_MAJOR >= 2
# define RANDR_12_SUPPORT
#endif

#ifdef RANDR_12_SUPPORT
/* Xserver interface */
# include "randrstr.h"
# include "xf86i2c.h"		/* Missing in old versions of xf86Crtc.h */
# include "xf86Crtc.h"
# include "xf86RandR12.h"
# define DPMS_SERVER
# include "X11/extensions/dpms.h"
# include "X11/Xatom.h"
#endif /* RANDR_12_SUPPORT */

#if HAVE_XF86_ANSIC_H
# include "xf86_ansic.h"
#endif

/* Driver specific headers */
#include "rhd.h"
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

/* Outputs and Connectors are combined for RandR due to missing abstraction */
typedef struct _rhdRandrOutput {
    char                 Name[64];
    struct rhdConnector *Connector;
    struct rhdOutput    *Output;
} rhdRandrOutputRec, *rhdRandrOutputPtr;

#define ATOM_SIGNAL_FORMAT    "RANDR_SIGNAL_FORMAT"
#define ATOM_CONNECTOR_TYPE   "RANDR_CONNECTOR_TYPE"
#define ATOM_CONNECTOR_NUMBER "RANDR_CONNECTOR_NUMBER"
#define ATOM_OUTPUT_NUMBER    "RANDR_OUTPUT_NUMBER"
#define ATOM_PANNING_AREA     "RANDR_PANNING_AREA"

static Atom atomSignalFormat, atomConnectorType, atomConnectorNumber,
	    atomOutputNumber, atomPanningArea;


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
rhdUpdateCrtcPos(struct rhdCrtc *Crtc, int x, int y)
{
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
	xf86CrtcPtr    rc = rhdPtr->randr->RandrCrtc[i];
	struct rhdCrtc *c = (struct rhdCrtc *) rc->driver_private;
	RHDDebugCont("   RRCrtc #%d [rhd %s]: active %d [%d]  "
		     "mode %s (%dx%d) +%d+%d\n",
		     i, c->Name, rc->enabled, c->Active,
		     rc->mode.name ? rc->mode.name : "unnamed",
		     rc->mode.HDisplay, rc->mode.VDisplay,
		     rc->x, rc->y);
    }
    for (ro = rhdPtr->randr->RandrOutput; *ro; ro++) {
	rhdRandrOutputPtr o = (rhdRandrOutputPtr) (*ro)->driver_private;
	ASSERT(!strcmp((*ro)->name,o->Name));
	RHDDebugCont("   RROut  %s [Out %s Conn %s]  Crtc %s [%s]  "
		     "[%sactive]  %s\n",
		     (*ro)->name, o->Output->Name, o->Connector->Name,
		     (*ro)->crtc ? ((struct rhdCrtc *)(*ro)->crtc->driver_private)->Name : "null",
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
rhdRRCrtcDpms(xf86CrtcPtr crtc, int mode)
{
    RHDPtr rhdPtr        = RHDPTR(crtc->scrn);
    struct rhdCrtc *Crtc = (struct rhdCrtc *) crtc->driver_private;

    RHDDebug(Crtc->scrnIndex, "%s: %s: %s\n", __func__, Crtc->Name,
	     mode==DPMSModeOn ? "On" : mode==DPMSModeOff ? "Off" : "Other");

    switch (mode) {
    case DPMSModeOn:
	if (Crtc->PLL)
	    Crtc->PLL->Power(Crtc->PLL, RHD_POWER_ON);
	Crtc->Power(Crtc, RHD_POWER_ON);
	Crtc->Active = TRUE;
	break;
    case DPMSModeSuspend:
    case DPMSModeStandby:
	Crtc->Power(Crtc, RHD_POWER_RESET);
	if (Crtc->PLL)
	    Crtc->PLL->Power(Crtc->PLL, RHD_POWER_RESET);
	Crtc->Active = FALSE;
	break;
    case DPMSModeOff:
	Crtc->Power(Crtc, RHD_POWER_SHUTDOWN);
	if (Crtc->PLL)
	    Crtc->PLL->Power(Crtc->PLL, RHD_POWER_SHUTDOWN);
	Crtc->Active = FALSE;
	break;
    default:
	ASSERT(!"Unknown DPMS mode");
    }
    RHDDebugRandrState(rhdPtr, "POST-CrtcDpms");
}

/* Lock CRTC prior to mode setting, mostly for DRI.
 * Returns whether unlock is needed */
static Bool
rhdRRCrtcLock(xf86CrtcPtr crtc)
{
    return FALSE;
}

/* Unlock CRTC after mode setting, mostly for DRI */
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
    struct rhdCrtc *Crtc   = (struct rhdCrtc *) crtc->driver_private;

    RHDFUNC(rhdPtr);
    setupCrtc(rhdPtr, Crtc);
    pScrn->vtSema = TRUE;

    /* Disable CRTCs to stop noise from appearing. */
    Crtc->Power(Crtc, RHD_POWER_RESET);

    /* Verify panning area */
    if (Crtc->MaxX > Crtc->Width)
	Crtc->MaxX = Crtc->Width;
    if (Crtc->MaxY > Crtc->Height)
	Crtc->MaxY = Crtc->Height;
}

static void
rhdRRCrtcModeSet(xf86CrtcPtr  crtc,
		 DisplayModePtr OrigMode, DisplayModePtr Mode,
		 int x, int y)
{
    RHDPtr          rhdPtr = RHDPTR(crtc->scrn);
    ScrnInfoPtr     pScrn  = xf86Screens[rhdPtr->scrnIndex];
    struct rhdCrtc *Crtc   = (struct rhdCrtc *) crtc->driver_private;

    /* RandR may give us a mode without a name... (xf86RandRModeConvert) */
    if (!Mode->name && crtc->mode.name)
	Mode->name = xstrdup(crtc->mode.name);

    RHDDebug(rhdPtr->scrnIndex, "%s: %s : %s at %d/%d\n", __func__,
	     Crtc->Name, Mode->name, x, y);

    /* Set up mode */
    Crtc->FBSet(Crtc, pScrn->displayWidth, pScrn->virtualX, pScrn->virtualY,
		pScrn->depth, rhdPtr->FbScanoutStart);
    Crtc->ModeSet(Crtc, Mode);
    Crtc->ScaleSet(Crtc, RHD_CRTC_SCALE_TYPE_NONE, Mode, NULL);
    Crtc->FrameSet(Crtc, x, y);
    rhdUpdateCrtcPos(Crtc, Crtc->Cursor->X, Crtc->Cursor->Y);
    RHDPLLSet(Crtc->PLL, Mode->Clock);		/* This also powers up PLL */
    Crtc->PLLSelect(Crtc, Crtc->PLL);
    Crtc->LUTSelect(Crtc, Crtc->LUT);
}
static void
rhdRRCrtcCommit(xf86CrtcPtr crtc)
{
    RHDPtr          rhdPtr = RHDPTR(crtc->scrn);
    struct rhdCrtc *Crtc   = (struct rhdCrtc *) crtc->driver_private;

    RHDFUNC(rhdPtr);

    Crtc->Active = TRUE;
    Crtc->Power(Crtc, RHD_POWER_ON);

    RHDDebugRandrState(rhdPtr, Crtc->Name);
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
    struct rhdCrtc *Crtc = (struct rhdCrtc *) crtc->driver_private;
    int indices[0x100]; /* would RandR use a size larger than 256? */
    LOCO colors[0x100];
    int i;

    RHDDebug(Crtc->scrnIndex, "%s: %s.\n", __func__, Crtc->Name);

    /* thanks so very much */
    for (i = 0; i < size; i++) {
	indices[i] = i;
	colors[i].red = red[i];
	colors[i].green = green[i];
	colors[i].blue = blue[i];
    }

    Crtc->LUT->Set(Crtc->LUT, size, indices, colors);
}

/* Dummy, because not tested for NULL */
static Bool
rhdRRCrtcModeFixupDUMMY(xf86CrtcPtr    crtc,
			DisplayModePtr mode,
			DisplayModePtr adjusted_mode)
{ return TRUE; }

#if 0 /* Needed if we want to support rotation w/o own hardware support */
    void *
    crtc->funcs->shadow_allocate (xf86CrtcPtr crtc, int width, int height)

This function allocates frame buffer space for a shadow frame buffer. When
allocated, the crtc must scan from the shadow instead of the main frame
buffer. This is used for rotation. The address returned is passed to the
shadow_create function. This function should return NULL on failure.

    PixmapPtr
    crtc->funcs->shadow_create (xf86CrtcPtr crtc, void *data,
                                int width, int height)

This function creates a pixmap object that will be used as a shadow of the
main frame buffer for CRTCs which are rotated or reflected. 'data' is the
value returned by shadow_allocate.

    void
    crtc->funcs->shadow_destroy (xf86CrtcPtr crtc, PixmapPtr pPixmap,
                                 void *data)

Destroys any associated shadow objects. If pPixmap is NULL, then a pixmap
was not created, but 'data' may still be non-NULL indicating that the shadow
had been allocated.
#endif


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
    struct rhdCrtc   *Crtc = out->crtc ?
        (struct rhdCrtc *) out->crtc->driver_private : NULL;
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
	ASSERT(Crtc);
	rout->Output->Crtc = Crtc;
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
	rout->Output->Crtc = NULL;
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
	rout->Output->Crtc = NULL;
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

    /* RandR may give us a mode without a name... (xf86RandRModeConvert)
     * xf86DuplicateMode should fill it up, though */
    if (!Mode->name)
	Mode->name = xstrdup("n/a");

    RHDDebug(rhdPtr->scrnIndex, "%s: Output %s : %s\n", __func__,
	     rout->Name, Mode->name);
    ASSERT(rout->Connector);
    ASSERT(rout->Output);

    /* If out->crtc is not NULL, it is not necessarily the Crtc that will
     * be used, so let's better skip crtc based checks... */
    /* Monitor is handled by RandR */
    Status = RHDRRModeFixup(out->scrn, Mode, NULL, rout->Connector,
			    rout->Output, NULL);
    RHDDebug(rhdPtr->scrnIndex, "%s: %s -> Status %d\n", __func__,
	     Mode->name, Status);
    xfree(Mode->name);
    xfree(Mode);
    return Status;
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
    struct rhdCrtc    *Crtc   = NULL;
    int                Status;

    /* !@#$ xf86RandRModeConvert doesn't initialize Mode with 0
     * Fixed in xserver git c6c284e6 */
    xfree(Mode->name);
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
    /* RHDRRModeFixup will set up the remaining bits */

    RHDDebug(rhdPtr->scrnIndex, "%s: Output %s : %s\n", __func__,
	     rout->Name, Mode->name);
    ASSERT(rout->Connector);
    ASSERT(rout->Output);

    if (out->crtc)
	Crtc = (struct rhdCrtc *) out->crtc->driver_private;
    setupCrtc(rhdPtr, Crtc);

    /* Monitor is handled by RandR */
    Status = RHDRRModeFixup(out->scrn, Mode, Crtc, rout->Connector,
			    rout->Output, NULL);
    if (Status != MODE_OK) {
	RHDDebug(rhdPtr->scrnIndex, "%s: %s FAILED: %d\n", __func__,
		 Mode->name, Status);
	return FALSE;
    }
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
    rout->Output->Crtc = NULL;
}
static void
rhdRROutputModeSet(xf86OutputPtr  out,
		   DisplayModePtr OrigMode, DisplayModePtr Mode)
{
    RHDPtr            rhdPtr = RHDPTR(out->scrn);
    rhdRandrOutputPtr rout   = (rhdRandrOutputPtr) out->driver_private;
    struct rhdCrtc   *Crtc   = (struct rhdCrtc *) out->crtc->driver_private;

    /* RandR may give us a mode without a name... (xf86RandRModeConvert) */
    if (!Mode->name && out->crtc->mode.name)
	Mode->name = xstrdup(out->crtc->mode.name);

    RHDDebug(rhdPtr->scrnIndex, "%s: Output %s : %s to %s\n", __func__,
	     rout->Name, Mode->name, Crtc->Name);

    /* RandR might want to set up several outputs (RandR speech) with different
     * crtcs, while the outputs differ in fact only by the connector and thus
     * cannot be used by different crtcs */
    if (rout->Output->Crtc && rout->Output->Crtc != Crtc)
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
		   "RandR: Output %s has already CRTC attached - "
		   "assuming ouput/connector clash\n", rout->Name);
    /* Set up mode */
    rout->Output->Crtc = Crtc;
    rout->Output->Mode(rout->Output, Mode);
}
static void
rhdRROutputCommit(xf86OutputPtr out)
{
    RHDPtr            rhdPtr = RHDPTR(out->scrn);
    rhdRandrOutputPtr rout   = (rhdRandrOutputPtr) out->driver_private;
    const char       *val;
    char              buf[32];
    struct rhdCrtc   *Crtc   = (struct rhdCrtc *) out->crtc->driver_private;

    RHDFUNC(rhdPtr);
    ASSERT(Crtc == rout->Output->Crtc);

    rout->Output->Active    = TRUE;
    rout->Output->Connector = rout->Connector; /* @@@ */
    rout->Output->Power(rout->Output, RHD_POWER_ON);

    /* Some outputs may have physical protocol changes (e.g. TV) */
    val = rhdGetSignalFormat(rout);
    RRChangeOutputProperty(out->randr_output, atomConnectorType,
			   XA_STRING, 8, PropModeReplace,
			   strlen(val), (char *) val, TRUE, FALSE);
    /* Should be a crtc property */
    if (Crtc->MaxX > Crtc->MinX && Crtc->MaxY > Crtc->MinY)
	sprintf(buf, "%dx%d+%d+%d", Crtc->MaxX - Crtc->MinX,
		Crtc->MaxY - Crtc->MinY, Crtc->MinX, Crtc->MinY);
    else
	buf[0] = 0;
    RRChangeOutputProperty(out->randr_output, atomPanningArea,
			   XA_STRING, 8, PropModeReplace,
			   strlen(buf), buf, TRUE, FALSE);

    RHDDebugRandrState(rhdPtr, rout->Name);
}


/* Probe for a connected output. */
static xf86OutputStatus
rhdRROutputDetect(xf86OutputPtr output)
{
    RHDPtr            rhdPtr = RHDPTR(output->scrn);
    rhdRandrOutputPtr rout   = (rhdRandrOutputPtr) output->driver_private;
    xf86OutputPtr    *ro;

    RHDDebug(rhdPtr->scrnIndex, "%s: Output %s\n", __func__, rout->Name);

    /* Assume that a panel is always connected */
    if (rout->Connector->Type == RHD_CONNECTOR_PANEL) {
	rout->Output->Connector = rout->Connector; /* @@@ */
	return XF86OutputStatusConnected;
    }

    if (rout->Connector->HPDCheck) {
	/* Hot Plug Detection available, use it */
	if (rout->Connector->HPDCheck(rout->Connector)) {
	    /*
	     * HPD returned true
	     */
	    if (rout->Output->Sense) {
		if ((rout->Output->SensedType
		     = rout->Output->Sense(rout->Output,
					   rout->Connector->Type))) {
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
		for (ro = rhdPtr->randr->RandrOutput; *ro; ro++) {
		    rhdRandrOutputPtr o =
			(rhdRandrOutputPtr) (*ro)->driver_private;
		    if (o != rout &&
			o->Connector == rout->Connector &&
			o->Output->Sense) {
			/* Yes, this looks wrong, but is correct */
			if ((o->Output->SensedType =
			     o->Output->Sense(o->Output, o->Connector->Type))) {
			    RHDOutputPrintSensedType(o->Output);
			    return XF86OutputStatusDisconnected;
			}
		    }
		}
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
		if (rout->Output->Sense &&
		    (rout->Output->SensedType
		     = rout->Output->Sense(rout->Output,
					   rout->Connector->Type))) {
		    rout->Output->Connector = rout->Connector; /* @@@ */
		    RHDOutputPrintSensedType(rout->Output);
		    return XF86OutputStatusConnected;
		}
	    }
	    return XF86OutputStatusDisconnected;
	}
    } else {
	/*
	 * No HPD available, Sense() if possible
	 */
	if (rout->Output->Sense) {
	    if ((rout->Output->SensedType
		 = rout->Output->Sense(rout->Output, rout->Connector->Type))) {
		    rout->Output->Connector = rout->Connector; /* @@@ */
		    RHDOutputPrintSensedType(rout->Output);
		    return XF86OutputStatusConnected;
	    } else
		return XF86OutputStatusDisconnected;
	}
	/* Use DDC address probing if possible otherwise */
	if (rout->Connector->DDC) {
	    if (xf86I2CProbeAddress(rout->Connector->DDC, 0xa0)) {
		rout->Output->Connector = rout->Connector; /* @@@ */
		return XF86OutputStatusConnected;
	    }  else
		return XF86OutputStatusDisconnected;
	}
	rout->Output->Connector = rout->Connector; /* @@@ */
	return XF86OutputStatusUnknown;
    }
}

/* Query the device for the modes it provides. Set MonInfo, mm_width/height. */
static DisplayModePtr
rhdRROutputGetModes(xf86OutputPtr output)
{
    RHDPtr            rhdPtr = RHDPTR(output->scrn);
    rhdRandrOutputPtr rout = (rhdRandrOutputPtr) output->driver_private;
    xf86MonPtr	      edid_mon = NULL;

    RHDDebug(rhdPtr->scrnIndex, "%s: Output %s\n", __func__, rout->Name);
    /* TODO: per-output options ForceReduced & UseXF86Edid */

    /* Use RandR edid parsing if requested */
    if (rhdPtr->rrUseXF86Edid.set && rhdPtr->rrUseXF86Edid.val.bool) {
	if (rout->Connector->DDC)
	    edid_mon = xf86OutputGetEDID (output, rout->Connector->DDC);
	xf86OutputSetEDID (output, edid_mon);
	return xf86OutputGetEDIDModes (output);
    }

    /* Nuke old monitor */
    if (rout->Connector->Monitor) {
	/* Modes and EDID are already freed by RandR (OutputSetEDID+return) */
	rout->Connector->Monitor->Modes = NULL;
	rout->Connector->Monitor->EDID = NULL;
	RHDMonitorDestroy(rout->Connector->Monitor);
    }
    /* Get new one */
    if (! (rout->Connector->Monitor = RHDMonitorInit(rout->Connector)) ) {
	xf86OutputSetEDID (output, NULL);
	return NULL;
    }

    /* If digitally attached, enable reduced blanking */
    if (rout->Output->Id == RHD_OUTPUT_TMDSA ||
	rout->Output->Id == RHD_OUTPUT_LVTMA ||
	rout->Output->Id == RHD_OUTPUT_KLDSKP_LVTMA ||
	rout->Output->Id == RHD_OUTPUT_UNIPHYA ||
	rout->Output->Id == RHD_OUTPUT_UNIPHYB)
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

    return rout->Connector->Monitor->Modes;
}

/* An output's property has changed. */
static Bool
rhdRROutputSetProperty(xf86OutputPtr out, Atom property,
		       RRPropertyValuePtr value)
{
    RHDPtr            rhdPtr = RHDPTR(out->scrn);
    rhdRandrOutputPtr rout   = (rhdRandrOutputPtr) out->driver_private;

    if (property == atomPanningArea) {
	int w = 0, h = 0, x = 0, y = 0;
	struct rhdCrtc *Crtc = rout->Output->Crtc;
	if (!Crtc)
	    return FALSE;
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
	    rhdUpdateCrtcPos(Crtc, Crtc->Cursor->X, Crtc->Cursor->Y);
	    RHDDebug(rhdPtr->scrnIndex, "%s: PanningArea %d/%d - %d/%d\n",
		     x, y, x+w, y+h);
	    return TRUE;
	default:
	    return FALSE;
	}
    }

    return FALSE;	/* Others are not mutable */
}


/*
 * Xorg Interface
 */

static const xf86CrtcConfigFuncsRec rhdRRCrtcConfigFuncs = {
    rhdRRXF86CrtcResize
};

static const xf86CrtcFuncsRec rhdRRCrtcFuncs = {
    rhdRRCrtcDpms,
    NULL, NULL,						/* Save,Restore */
    rhdRRCrtcLock, rhdRRCrtcUnlock,
    rhdRRCrtcModeFixupDUMMY,
    rhdRRCrtcPrepare, rhdRRCrtcModeSet, rhdRRCrtcCommit,
    rhdRRCrtcGammaSet,
    /* rhdRRCrtcShadowAllocate,rhdRRCrtcShadowCreate,rhdRRCrtcShadowDestroy */
    NULL, NULL, NULL,
    /* SetCursorColors,SetCursorPosition,ShowCursor,HideCursor,
     * LoadCursorImage,LoadCursorArgb,CrtcDestroy */
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static const xf86OutputFuncsRec rhdRROutputFuncs = {
    rhdRROutputCreateResources, rhdRROutputDpms,
    NULL, NULL,						/* Save,Restore */
    rhdRROutputModeValid, rhdRROutputModeFixup,
    rhdRROutputPrepare, rhdRROutputCommit,
    rhdRROutputModeSet, rhdRROutputDetect, rhdRROutputGetModes,
    rhdRROutputSetProperty,		       /* Only(!) RANDR_12_INTERFACE */
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
    int i, j, numCombined = 0;
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
	randr->RandrCrtc[i]->driver_private = rhdPtr->Crtc[i];
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
		   "RandR: No valid modes. Disabled.\n");
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
	    rhdUpdateCrtcPos(Crtc, x + pScrn->frameX0, y + pScrn->frameY0);
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

    /* Stop crap from being shown: gets reenabled through SaveScreen */
    rhdPtr->Crtc[0]->Blank(rhdPtr->Crtc[0], TRUE);
    rhdPtr->Crtc[1]->Blank(rhdPtr->Crtc[1], TRUE);

    RHDVGADisable(rhdPtr);
    RHDMCSetup(rhdPtr);

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

