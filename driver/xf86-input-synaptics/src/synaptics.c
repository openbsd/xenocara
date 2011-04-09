/*
 * Copyright © 1999 Henry Davies
 * Copyright © 2001 Stefan Gmeiner
 * Copyright © 2002 S. Lehner
 * Copyright © 2002 Peter Osterlund
 * Copyright © 2002 Linuxcare Inc. David Kennedy
 * Copyright © 2003 Hartwig Felger
 * Copyright © 2003 Jörg Bösner
 * Copyright © 2003 Fred Hucht
 * Copyright © 2004 Alexei Gilchrist
 * Copyright © 2004 Matthias Ihmig
 * Copyright © 2006 Stefan Bethge
 * Copyright © 2006 Christian Thaeter
 * Copyright © 2007 Joseph P. Skudlarek
 * Copyright © 2008 Fedor P. Goncharov
 * Copyright © 2008-2009 Red Hat, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of Red Hat
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.  Red
 * Hat makes no representations about the suitability of this software
 * for any purpose.  It is provided "as is" without express or implied
 * warranty.
 *
 * THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
 * NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors:
 *      Joseph P. Skudlarek <Jskud@Jskud.com>
 *      Christian Thaeter <chth@gmx.net>
 *      Stefan Bethge <stefan.bethge@web.de>
 *      Matthias Ihmig <m.ihmig@gmx.net>
 *      Alexei Gilchrist <alexei@physics.uq.edu.au>
 *      Jörg Bösner <ich@joerg-boesner.de>
 *      Hartwig Felger <hgfelger@hgfelger.de>
 *      Peter Osterlund <petero2@telia.com>
 *      S. Lehner <sam_x@bluemail.ch>
 *      Stefan Gmeiner <riddlebox@freesurf.ch>
 *      Henry Davies <hdavies@ameritech.net> for the
 *      Linuxcare Inc. David Kennedy <dkennedy@linuxcare.com>
 *      Fred Hucht <fred@thp.Uni-Duisburg.de>
 *      Fedor P. Goncharov <fedgo@gorodok.net>
 *      Simon Thum <simon.thum@gmx.de>
 *
 * Trademarks are the property of their respective owners.
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include <unistd.h>
#include <misc.h>
#include <xf86.h>
#include <sys/shm.h>
#include <math.h>
#include <stdio.h>
#include <xf86_OSproc.h>
#include <xf86Xinput.h>
#include <exevents.h>

#include "synaptics.h"
#include "synapticsstr.h"
#include "synaptics-properties.h"

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
#include <X11/Xatom.h>
#include <xserver-properties.h>
#include <ptrveloc.h>
#endif

typedef enum {
    NO_EDGE = 0,
    BOTTOM_EDGE = 1,
    TOP_EDGE = 2,
    LEFT_EDGE = 4,
    RIGHT_EDGE = 8,
    LEFT_BOTTOM_EDGE = BOTTOM_EDGE | LEFT_EDGE,
    RIGHT_BOTTOM_EDGE = BOTTOM_EDGE | RIGHT_EDGE,
    RIGHT_TOP_EDGE = TOP_EDGE | RIGHT_EDGE,
    LEFT_TOP_EDGE = TOP_EDGE | LEFT_EDGE
} edge_type;

#define MAX(a, b) (((a)>(b))?(a):(b))
#define MIN(a, b) (((a)<(b))?(a):(b))
#define TIME_DIFF(a, b) ((int)((a)-(b)))

#define SQR(x) ((x) * (x))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2  0.70710678118654752440  /* 1/sqrt(2) */
#endif

#define INPUT_BUFFER_SIZE 200

/*****************************************************************************
 * Forward declaration
 ****************************************************************************/
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
static int SynapticsPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags);
#else
static InputInfoPtr SynapticsPreInit(InputDriverPtr drv, IDevPtr dev, int flags);
#endif
static void SynapticsUnInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags);
static Bool DeviceControl(DeviceIntPtr, int);
static void ReadInput(InputInfoPtr);
static int HandleState(InputInfoPtr, struct SynapticsHwState*);
static int ControlProc(InputInfoPtr, xDeviceCtl*);
static int SwitchMode(ClientPtr, DeviceIntPtr, int);
static Bool DeviceInit(DeviceIntPtr);
static Bool DeviceOn(DeviceIntPtr);
static Bool DeviceOff(DeviceIntPtr);
static Bool DeviceClose(DeviceIntPtr);
static Bool QueryHardware(InputInfoPtr);
static void ReadDevDimensions(InputInfoPtr);
static void ScaleCoordinates(SynapticsPrivate *priv, struct SynapticsHwState *hw);
static void CalculateScalingCoeffs(SynapticsPrivate *priv);

void InitDeviceProperties(InputInfoPtr pInfo);
int SetProperty(DeviceIntPtr dev, Atom property, XIPropertyValuePtr prop,
                BOOL checkonly);

InputDriverRec SYNAPTICS = {
    1,
    "synaptics",
    NULL,
    SynapticsPreInit,
    SynapticsUnInit,
    NULL,
};

static XF86ModuleVersionInfo VersionRec = {
    "synaptics",
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, PACKAGE_VERSION_PATCHLEVEL,
    ABI_CLASS_XINPUT,
    ABI_XINPUT_VERSION,
    MOD_CLASS_XINPUT,
    {0, 0, 0, 0}
};

static pointer
SetupProc(pointer module, pointer options, int *errmaj, int *errmin)
{
    xf86AddInputDriver(&SYNAPTICS, module, 0);
    return module;
}

_X_EXPORT XF86ModuleData synapticsModuleData = {
    &VersionRec,
    &SetupProc,
    NULL
};


/*****************************************************************************
 *	Function Definitions
 ****************************************************************************/
/**
 * Fill in default dimensions for backends that cannot query the hardware.
 * Eventually, we want the edges to be 1900/5400 for x, 1900/4000 for y.
 * These values are based so that calculate_edge_widths() will give us the
 * right values.
 *
 * The default values 1900, etc. come from the dawn of time, when men where
 * men, or possibly apes.
 */
void
SynapticsDefaultDimensions(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *)pInfo->private;

    if (priv->minx >= priv->maxx)
    {
	priv->minx = 1615;
	priv->maxx = 5685;
	priv->resx = 0;

	xf86Msg(X_PROBED,
		"%s: invalid x-axis range.  defaulting to %d - %d\n",
		pInfo->name, priv->minx, priv->maxx);
    }

    if (priv->miny >= priv->maxy)
    {
	priv->miny = 1729;
	priv->maxy = 4171;
	priv->resx = 0;

	xf86Msg(X_PROBED,
		"%s: invalid y-axis range.  defaulting to %d - %d\n",
		pInfo->name, priv->miny, priv->maxy);
    }

    if (priv->minp >= priv->maxp)
    {
	priv->minp = 0;
	priv->maxp = 256;

	xf86Msg(X_PROBED,
		"%s: invalid pressure range.  defaulting to %d - %d\n",
		pInfo->name, priv->minp, priv->maxp);
    }

    if (priv->minw >= priv->maxw)
    {
	priv->minw = 0;
	priv->maxw = 16;

	xf86Msg(X_PROBED,
		"%s: invalid finger width range.  defaulting to %d - %d\n",
		pInfo->name, priv->minw, priv->maxw);
    }
}

static void
SetDeviceAndProtocol(InputInfoPtr pInfo)
{
    char *str_par, *device;
    SynapticsPrivate *priv = pInfo->private;
    enum SynapticsProtocol proto = SYN_PROTO_PSAUX;

    device = xf86SetStrOption(pInfo->options, "Device", NULL);
    if (!device) {
	device = xf86SetStrOption(pInfo->options, "Path", NULL);
	if (device) {
	    pInfo->options =
	    	xf86ReplaceStrOption(pInfo->options, "Device", device);
	}
    }
    if (device && strstr(device, "/dev/input/event")) {
#ifdef BUILD_EVENTCOMM
	proto = SYN_PROTO_EVENT;
#endif
    } else {
	str_par = xf86FindOptionValue(pInfo->options, "Protocol");
	if (str_par && !strcmp(str_par, "psaux")) {
	    /* Already set up */
#ifdef BUILD_EVENTCOMM
	} else if (str_par && !strcmp(str_par, "event")) {
	    proto = SYN_PROTO_EVENT;
#endif /* BUILD_EVENTCOMM */
#ifdef BUILD_PSMCOMM
	} else if (str_par && !strcmp(str_par, "psm")) {
	    proto = SYN_PROTO_PSM;
#endif /* BUILD_PSMCOMM */
	} else if (str_par && !strcmp(str_par, "alps")) {
	    proto = SYN_PROTO_ALPS;
	} else { /* default to auto-dev */
#ifdef BUILD_EVENTCOMM
	    if (!device && event_proto_operations.AutoDevProbe(pInfo))
		proto = SYN_PROTO_EVENT;
#endif
	}
    }
    switch (proto) {
    case SYN_PROTO_PSAUX:
	priv->proto_ops = &psaux_proto_operations;
	break;
#ifdef BUILD_EVENTCOMM
    case SYN_PROTO_EVENT:
	priv->proto_ops = &event_proto_operations;
	break;
#endif /* BUILD_EVENTCOMM */
#ifdef BUILD_PSMCOMM
    case SYN_PROTO_PSM:
	priv->proto_ops = &psm_proto_operations;
	break;
#endif /* BUILD_PSMCOMM */
    case SYN_PROTO_ALPS:
	priv->proto_ops = &alps_proto_operations;
	break;
    }
}

/*
 * Allocate and initialize read-only memory for the SynapticsParameters data to hold
 * driver settings.
 * The function will allocate shared memory if priv->shm_config is TRUE.
 */
static Bool
alloc_shm_data(InputInfoPtr pInfo)
{
    int shmid;
    SynapticsPrivate *priv = pInfo->private;

    if (priv->synshm)
	return TRUE;			    /* Already allocated */

    if (priv->shm_config) {
	if ((shmid = shmget(SHM_SYNAPTICS, 0, 0)) != -1)
	    shmctl(shmid, IPC_RMID, NULL);
	if ((shmid = shmget(SHM_SYNAPTICS, sizeof(SynapticsSHM),
				0774 | IPC_CREAT)) == -1) {
	    xf86Msg(X_ERROR, "%s error shmget\n", pInfo->name);
	    return FALSE;
	}
	if ((priv->synshm = (SynapticsSHM*)shmat(shmid, NULL, 0)) == NULL) {
	    xf86Msg(X_ERROR, "%s error shmat\n", pInfo->name);
	    return FALSE;
	}
    } else {
	priv->synshm = calloc(1, sizeof(SynapticsSHM));
	if (!priv->synshm)
	    return FALSE;
    }

    return TRUE;
}

/*
 * Free SynapticsParameters data previously allocated by alloc_shm_data().
 */
static void
free_shm_data(SynapticsPrivate *priv)
{
    int shmid;

    if (!priv->synshm)
	return;

    if (priv->shm_config) {
	if ((shmid = shmget(SHM_SYNAPTICS, 0, 0)) != -1)
	    shmctl(shmid, IPC_RMID, NULL);
    } else {
	free(priv->synshm);
    }

    priv->synshm = NULL;
}

static void
calculate_edge_widths(SynapticsPrivate *priv, int *l, int *r, int *t, int *b)
{
    int width, height;
    int ewidth, eheight; /* edge width/height */

    width = abs(priv->maxx - priv->minx);
    height = abs(priv->maxy - priv->miny);

    if (priv->model == MODEL_SYNAPTICS)
    {
        ewidth = width * .07;
        eheight = height * .07;
    } else if (priv->model == MODEL_ALPS)
    {
        ewidth = width * .15;
        eheight = height * .15;
    } else if (priv->model == MODEL_APPLETOUCH)
    {
        ewidth = width * .085;
        eheight = height * .085;
    } else
    {
        ewidth = width * .04;
        eheight = height * .054;
    }

    *l = priv->minx + ewidth;
    *r = priv->maxx - ewidth;
    *t = priv->miny + eheight;
    *b = priv->maxy - eheight;
}

/* Area options support both percent values and absolute values. This is
 * awkward. The xf86Set* calls will print to the log, but they'll
 * also print an error if we request a percent value but only have an
 * int. So - check first for percent, then call xf86Set* again to get
 * the log message.
 */
static int set_percent_option(pointer options, const char* optname,
                              const int range, const int offset,
                              const int default_value)
{
    int result;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 11
    double percent = xf86CheckPercentOption(options, optname, -1);

    if (percent >= 0.0) {
        percent = xf86SetPercentOption(options, optname, -1);
        result = percent/100.0 * range + offset;
    } else
#endif
        result = xf86SetIntOption(options, optname, default_value);

    return result;
}

static void set_default_parameters(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = pInfo->private; /* read-only */
    pointer opts = pInfo->options; /* read-only */
    SynapticsParameters *pars = &priv->synpara; /* modified */

    int horizScrollDelta, vertScrollDelta;		/* pixels */
    int tapMove;					/* pixels */
    int l, r, t, b; /* left, right, top, bottom */
    int edgeMotionMinSpeed, edgeMotionMaxSpeed;		/* pixels/second */
    double accelFactor;					/* 1/pixels */
    int fingerLow, fingerHigh, fingerPress;		/* pressure */
    int emulateTwoFingerMinZ;				/* pressure */
    int emulateTwoFingerMinW;				/* width */
    int edgeMotionMinZ, edgeMotionMaxZ;			/* pressure */
    int pressureMotionMinZ, pressureMotionMaxZ;		/* pressure */
    int palmMinWidth, palmMinZ;				/* pressure */
    int tapButton1, tapButton2, tapButton3;
    int clickFinger1, clickFinger2, clickFinger3;
    Bool vertEdgeScroll, horizEdgeScroll;
    Bool vertTwoFingerScroll, horizTwoFingerScroll;
    int horizResolution = 1;
    int vertResolution = 1;
    int width, height, diag, range;
    int horizHyst, vertHyst;

    /* read the parameters */
    if (priv->synshm)
        priv->synshm->version = (PACKAGE_VERSION_MAJOR*10000+PACKAGE_VERSION_MINOR*100+PACKAGE_VERSION_PATCHLEVEL);

    /* The synaptics specs specify typical edge widths of 4% on x, and 5.4% on
     * y (page 7) [Synaptics TouchPad Interfacing Guide, 510-000080 - A
     * Second Edition, http://www.synaptics.com/support/dev_support.cfm, 8 Sep
     * 2008]. We use 7% for both instead for synaptics devices, and 15% for
     * ALPS models.
     * http://bugs.freedesktop.org/show_bug.cgi?id=21214
     *
     * If the range was autodetected, apply these edge widths to all four
     * sides.
     */
    SynapticsDefaultDimensions(pInfo);

    width = abs(priv->maxx - priv->minx);
    height = abs(priv->maxy - priv->miny);
    diag = sqrt(width * width + height * height);

    calculate_edge_widths(priv, &l, &r, &t, &b);

    /* Again, based on typical x/y range and defaults */
    horizScrollDelta = diag * .020;
    vertScrollDelta = diag * .020;
    tapMove = diag * .044;
    edgeMotionMinSpeed = 1;
    edgeMotionMaxSpeed = diag * .080;
    accelFactor = 200.0 / diag; /* trial-and-error */

    /* hysteresis, assume >= 0 is a detected value (e.g. evdev fuzz) */
    horizHyst = pars->hyst_x >= 0 ? pars->hyst_x : diag * 0.005;
    vertHyst = pars->hyst_y >= 0 ? pars->hyst_y : diag * 0.005;

    range = priv->maxp - priv->minp;

    /* scaling based on defaults and a pressure of 256 */
    fingerLow = priv->minp + range * (25.0/256);
    fingerHigh = priv->minp + range * (30.0/256);
    fingerPress = priv->minp + range * 1.000;
    edgeMotionMinZ = priv->minp + range * (30.0/256);
    edgeMotionMaxZ = priv->minp + range * (160.0/256);
    pressureMotionMinZ = priv->minp + range * (30.0/256);
    pressureMotionMaxZ = priv->minp + range * (160.0/256);
    palmMinZ = priv->minp + range * (200.0/256);

    /* Enable emulation when hw supports both pressure and width. */
    if (!priv->has_double && priv->has_width)
	emulateTwoFingerMinZ = fingerHigh;
    else
	emulateTwoFingerMinZ = priv->minp + range * (282.0/256);

    range = priv->maxw - priv->minw;

    /* scaling based on defaults below and a tool width of 16 */
    palmMinWidth = priv->minw + range * (10.0/16);
    emulateTwoFingerMinW = priv->minw + range * (7.0/16);

    /* Enable tap if we don't have a phys left button */
    tapButton1 = priv->has_left ? 0 : 1;
    tapButton2 = priv->has_left ? 0 : 3;
    tapButton3 = priv->has_left ? 0 : 2;

    /* Enable multifinger-click if only have one physical button,
       otherwise clickFinger is always button 1. */
    clickFinger1 = 1;
    clickFinger2 = (priv->has_right || priv->has_middle) ? 1 : 3;
    clickFinger3 = (priv->has_right || priv->has_middle) ? 1 : 2;

    /* Enable vert edge scroll if we can't detect doubletap */
    vertEdgeScroll = priv->has_double ? FALSE : TRUE;
    horizEdgeScroll = FALSE;

    /* Enable twofinger scroll if we can detect doubletap */
    vertTwoFingerScroll = priv->has_double ? TRUE : FALSE;
    horizTwoFingerScroll = FALSE;

    /* Use resolution reported by hardware if available */
    if ((priv->resx > 0) && (priv->resy > 0)) {
        horizResolution = priv->resx;
        vertResolution = priv->resy;
    }

    /* set the parameters */
    pars->left_edge = xf86SetIntOption(opts, "LeftEdge", l);
    pars->right_edge = xf86SetIntOption(opts, "RightEdge", r);
    pars->top_edge = xf86SetIntOption(opts, "TopEdge", t);
    pars->bottom_edge = xf86SetIntOption(opts, "BottomEdge", b);

    pars->area_top_edge = set_percent_option(opts, "AreaTopEdge", height, priv->miny, 0);
    pars->area_bottom_edge = set_percent_option(opts, "AreaBottomEdge", height, priv->miny, 0);
    pars->area_left_edge = set_percent_option(opts, "AreaLeftEdge", width, priv->minx, 0);
    pars->area_right_edge = set_percent_option(opts, "AreaRightEdge", width, priv->minx, 0);

    pars->hyst_x = set_percent_option(opts, "HorizHysteresis", width, 0, horizHyst);
    pars->hyst_y = set_percent_option(opts, "VertHysteresis", height, 0, vertHyst);

    pars->finger_low = xf86SetIntOption(opts, "FingerLow", fingerLow);
    pars->finger_high = xf86SetIntOption(opts, "FingerHigh", fingerHigh);
    pars->finger_press = xf86SetIntOption(opts, "FingerPress", fingerPress);
    pars->tap_time = xf86SetIntOption(opts, "MaxTapTime", 180);
    pars->tap_move = xf86SetIntOption(opts, "MaxTapMove", tapMove);
    pars->tap_time_2 = xf86SetIntOption(opts, "MaxDoubleTapTime", 180);
    pars->click_time = xf86SetIntOption(opts, "ClickTime", 100);
    pars->fast_taps = xf86SetBoolOption(opts, "FastTaps", FALSE);
    pars->emulate_mid_button_time = xf86SetIntOption(opts, "EmulateMidButtonTime", 75);
    pars->emulate_twofinger_z = xf86SetIntOption(opts, "EmulateTwoFingerMinZ", emulateTwoFingerMinZ);
    pars->emulate_twofinger_w = xf86SetIntOption(opts, "EmulateTwoFingerMinW", emulateTwoFingerMinW);
    pars->scroll_dist_vert = xf86SetIntOption(opts, "VertScrollDelta", horizScrollDelta);
    pars->scroll_dist_horiz = xf86SetIntOption(opts, "HorizScrollDelta", vertScrollDelta);
    pars->scroll_edge_vert = xf86SetBoolOption(opts, "VertEdgeScroll", vertEdgeScroll);
    pars->scroll_edge_horiz = xf86SetBoolOption(opts, "HorizEdgeScroll", horizEdgeScroll);
    pars->scroll_edge_corner = xf86SetBoolOption(opts, "CornerCoasting", FALSE);
    pars->scroll_twofinger_vert = xf86SetBoolOption(opts, "VertTwoFingerScroll", vertTwoFingerScroll);
    pars->scroll_twofinger_horiz = xf86SetBoolOption(opts, "HorizTwoFingerScroll", horizTwoFingerScroll);
    pars->edge_motion_min_z = xf86SetIntOption(opts, "EdgeMotionMinZ", edgeMotionMinZ);
    pars->edge_motion_max_z = xf86SetIntOption(opts, "EdgeMotionMaxZ", edgeMotionMaxZ);
    pars->edge_motion_min_speed = xf86SetIntOption(opts, "EdgeMotionMinSpeed", edgeMotionMinSpeed);
    pars->edge_motion_max_speed = xf86SetIntOption(opts, "EdgeMotionMaxSpeed", edgeMotionMaxSpeed);
    pars->edge_motion_use_always = xf86SetBoolOption(opts, "EdgeMotionUseAlways", FALSE);
    if (priv->has_scrollbuttons) {
	pars->updown_button_scrolling = xf86SetBoolOption(opts, "UpDownScrolling", TRUE);
	pars->leftright_button_scrolling = xf86SetBoolOption(opts, "LeftRightScrolling", TRUE);
	pars->updown_button_repeat = xf86SetBoolOption(opts, "UpDownScrollRepeat", TRUE);
	pars->leftright_button_repeat = xf86SetBoolOption(opts, "LeftRightScrollRepeat", TRUE);
    }
    pars->scroll_button_repeat = xf86SetIntOption(opts,"ScrollButtonRepeat", 100);
    pars->touchpad_off = xf86SetIntOption(opts, "TouchpadOff", 0);
    pars->locked_drags = xf86SetBoolOption(opts, "LockedDrags", FALSE);
    pars->locked_drag_time = xf86SetIntOption(opts, "LockedDragTimeout", 5000);
    pars->tap_action[RT_TAP] = xf86SetIntOption(opts, "RTCornerButton", 0);
    pars->tap_action[RB_TAP] = xf86SetIntOption(opts, "RBCornerButton", 0);
    pars->tap_action[LT_TAP] = xf86SetIntOption(opts, "LTCornerButton", 0);
    pars->tap_action[LB_TAP] = xf86SetIntOption(opts, "LBCornerButton", 0);
    pars->tap_action[F1_TAP] = xf86SetIntOption(opts, "TapButton1",     tapButton1);
    pars->tap_action[F2_TAP] = xf86SetIntOption(opts, "TapButton2",     tapButton2);
    pars->tap_action[F3_TAP] = xf86SetIntOption(opts, "TapButton3",     tapButton3);
    pars->click_action[F1_CLICK1] = xf86SetIntOption(opts, "ClickFinger1", clickFinger1);
    pars->click_action[F2_CLICK1] = xf86SetIntOption(opts, "ClickFinger2", clickFinger2);
    pars->click_action[F3_CLICK1] = xf86SetIntOption(opts, "ClickFinger3", clickFinger3);
    pars->circular_scrolling = xf86SetBoolOption(opts, "CircularScrolling", FALSE);
    pars->circular_trigger   = xf86SetIntOption(opts, "CircScrollTrigger", 0);
    pars->circular_pad       = xf86SetBoolOption(opts, "CircularPad", FALSE);
    pars->palm_detect        = xf86SetBoolOption(opts, "PalmDetect", FALSE);
    pars->palm_min_width     = xf86SetIntOption(opts, "PalmMinWidth", palmMinWidth);
    pars->palm_min_z         = xf86SetIntOption(opts, "PalmMinZ", palmMinZ);
    pars->single_tap_timeout = xf86SetIntOption(opts, "SingleTapTimeout", 180);
    pars->press_motion_min_z = xf86SetIntOption(opts, "PressureMotionMinZ", pressureMotionMinZ);
    pars->press_motion_max_z = xf86SetIntOption(opts, "PressureMotionMaxZ", pressureMotionMaxZ);

    pars->min_speed = xf86SetRealOption(opts, "MinSpeed", 0.4);
    pars->max_speed = xf86SetRealOption(opts, "MaxSpeed", 0.7);
    pars->accl = xf86SetRealOption(opts, "AccelFactor", accelFactor);
    pars->trackstick_speed = xf86SetRealOption(opts, "TrackstickSpeed", 40);
    pars->scroll_dist_circ = xf86SetRealOption(opts, "CircScrollDelta", 0.1);
    pars->coasting_speed = xf86SetRealOption(opts, "CoastingSpeed", 20.0);
    pars->coasting_friction = xf86SetRealOption(opts, "CoastingFriction", 50);
    pars->press_motion_min_factor = xf86SetRealOption(opts, "PressureMotionMinFactor", 1.0);
    pars->press_motion_max_factor = xf86SetRealOption(opts, "PressureMotionMaxFactor", 1.0);
    pars->grab_event_device = xf86SetBoolOption(opts, "GrabEventDevice", TRUE);
    pars->tap_and_drag_gesture = xf86SetBoolOption(opts, "TapAndDragGesture", TRUE);
    pars->resolution_horiz = xf86SetIntOption(opts, "HorizResolution", horizResolution);
    pars->resolution_vert = xf86SetIntOption(opts, "VertResolution", vertResolution);

    /* Warn about (and fix) incorrectly configured TopEdge/BottomEdge parameters */
    if (pars->top_edge > pars->bottom_edge) {
	int tmp = pars->top_edge;
	pars->top_edge = pars->bottom_edge;
	pars->bottom_edge = tmp;
	xf86Msg(X_WARNING, "%s: TopEdge is bigger than BottomEdge. Fixing.\n",
		pInfo->name);
    }
}

static float SynapticsAccelerationProfile(DeviceIntPtr dev,
                                          DeviceVelocityPtr vel,
                                          float velocity,
                                          float thr,
                                          float acc) {
    InputInfoPtr pInfo = dev->public.devicePrivate;
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    SynapticsParameters* para = &priv->synpara;

    double accelfct;

    /*
     * synaptics accel was originally base on device coordinate based
     * velocity, which we recover this way so para->accl retains its scale.
     */
    velocity /= vel->const_acceleration;

    /* speed up linear with finger velocity */
    accelfct = velocity * para->accl;

    /* clip acceleration factor */
    if (accelfct > para->max_speed * acc)
	accelfct = para->max_speed * acc;
    else if (accelfct < para->min_speed)
	accelfct = para->min_speed;

    /* modify speed according to pressure */
    if (priv->moving_state == MS_TOUCHPAD_RELATIVE) {
	int minZ = para->press_motion_min_z;
	int maxZ = para->press_motion_max_z;
	double minFctr = para->press_motion_min_factor;
	double maxFctr = para->press_motion_max_factor;
	if (priv->hwState.z <= minZ) {
	    accelfct *= minFctr;
	} else if (priv->hwState.z >= maxZ) {
	    accelfct *= maxFctr;
	} else {
	    accelfct *= minFctr + (priv->hwState.z - minZ) * (maxFctr - minFctr) / (maxZ - minZ);
	}
    }

    return accelfct;
}

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) < 12
static int
NewSynapticsPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags);
/*
 *  called by the module loader for initialization
 */
static InputInfoPtr
SynapticsPreInit(InputDriverPtr drv, IDevPtr dev, int flags)
{
    InputInfoPtr pInfo;

    /* Allocate a new InputInfoRec and add it to the head xf86InputDevs. */
    pInfo = xf86AllocateInput(drv, 0);
    if (!pInfo) {
	return NULL;
    }

    /* initialize the InputInfoRec */
    pInfo->name                    = dev->identifier;
    pInfo->reverse_conversion_proc = NULL;
    pInfo->dev                     = NULL;
    pInfo->private_flags           = 0;
    pInfo->flags                   = XI86_SEND_DRAG_EVENTS;
    pInfo->conf_idev               = dev;
    pInfo->always_core_feedback    = 0;

    xf86CollectInputOptions(pInfo, NULL, NULL);

    if (NewSynapticsPreInit(drv, pInfo, flags) != Success)
        return NULL;

    pInfo->flags |= XI86_CONFIGURED;

    return pInfo;
}

static int
NewSynapticsPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
#else
static int
SynapticsPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
#endif
{
    SynapticsPrivate *priv;

    /* allocate memory for SynapticsPrivateRec */
    priv = calloc(1, sizeof(SynapticsPrivate));
    if (!priv)
	return BadAlloc;

    pInfo->type_name               = XI_TOUCHPAD;
    pInfo->device_control          = DeviceControl;
    pInfo->read_input              = ReadInput;
    pInfo->control_proc            = ControlProc;
    pInfo->switch_mode             = SwitchMode;
    pInfo->private                 = priv;

    /* allocate now so we don't allocate in the signal handler */
    priv->timer = TimerSet(NULL, 0, 0, NULL, NULL);
    if (!priv->timer) {
	free(priv);
	return BadAlloc;
    }

    /* may change pInfo->options */
    SetDeviceAndProtocol(pInfo);

    /* open the touchpad device */
    pInfo->fd = xf86OpenSerial(pInfo->options);
    if (pInfo->fd == -1) {
	xf86Msg(X_ERROR, "Synaptics driver unable to open device\n");
	goto SetupProc_fail;
    }
    xf86ErrorFVerb(6, "port opened successfully\n");

    /* initialize variables */
    priv->repeatButtons = 0;
    priv->nextRepeat = 0;
    priv->count_packet_finger = 0;
    priv->tap_state = TS_START;
    priv->tap_button = 0;
    priv->tap_button_state = TBS_BUTTON_UP;
    priv->touch_on.millis = 0;
    priv->synpara.hyst_x = -1;
    priv->synpara.hyst_y = -1;

    /* read hardware dimensions */
    ReadDevDimensions(pInfo);

    /* install shared memory or normal memory for parameters */
    priv->shm_config = xf86SetBoolOption(pInfo->options, "SHMConfig", FALSE);

    set_default_parameters(pInfo);

    CalculateScalingCoeffs(priv);

    if (!alloc_shm_data(pInfo))
	goto SetupProc_fail;

    priv->comm.buffer = XisbNew(pInfo->fd, INPUT_BUFFER_SIZE);

    if (!QueryHardware(pInfo)) {
	xf86Msg(X_ERROR, "%s Unable to query/initialize Synaptics hardware.\n", pInfo->name);
	goto SetupProc_fail;
    }

    xf86ProcessCommonOptions(pInfo, pInfo->options);

    if (pInfo->fd != -1) {
	if (priv->comm.buffer) {
	    XisbFree(priv->comm.buffer);
	    priv->comm.buffer = NULL;
	}
	xf86CloseSerial(pInfo->fd);
    }
    pInfo->fd = -1;

    return Success;

 SetupProc_fail:
    if (pInfo->fd >= 0) {
	xf86CloseSerial(pInfo->fd);
	pInfo->fd = -1;
    }

    if (priv->comm.buffer)
	XisbFree(priv->comm.buffer);
    free_shm_data(priv);
    free(priv->proto_data);
    free(priv->timer);
    free(priv);
    pInfo->private = NULL;
    return BadAlloc;
}


/*
 *  Uninitialize the device.
 */
static void SynapticsUnInit(InputDriverPtr drv,
                            InputInfoPtr   pInfo,
                            int            flags)
{
    SynapticsPrivate *priv = ((SynapticsPrivate *)pInfo->private);
    if (priv && priv->timer)
        free(priv->timer);
    if (priv && priv->proto_data)
        free(priv->proto_data);
    free(pInfo->private);
    pInfo->private = NULL;
    xf86DeleteInput(pInfo, 0);
}


/*
 *  Alter the control parameters for the mouse. Note that all special
 *  protocol values are handled by dix.
 */
static void
SynapticsCtrl(DeviceIntPtr device, PtrCtrl *ctrl)
{
}

static Bool
DeviceControl(DeviceIntPtr dev, int mode)
{
    Bool RetValue;

    switch (mode) {
    case DEVICE_INIT:
	RetValue = DeviceInit(dev);
	break;
    case DEVICE_ON:
	RetValue = DeviceOn(dev);
	break;
    case DEVICE_OFF:
	RetValue = DeviceOff(dev);
	break;
    case DEVICE_CLOSE:
	RetValue = DeviceClose(dev);
	break;
    default:
	RetValue = BadValue;
    }

    return RetValue;
}

static Bool
DeviceOn(DeviceIntPtr dev)
{
    InputInfoPtr pInfo = dev->public.devicePrivate;
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);

    DBG(3, "Synaptics DeviceOn called\n");

    SetDeviceAndProtocol(pInfo);
    pInfo->fd = xf86OpenSerial(pInfo->options);
    if (pInfo->fd == -1) {
	xf86Msg(X_WARNING, "%s: cannot open input device\n", pInfo->name);
	return !Success;
    }

    if (priv->proto_ops->DeviceOnHook)
        priv->proto_ops->DeviceOnHook(pInfo, &priv->synpara);

    priv->comm.buffer = XisbNew(pInfo->fd, INPUT_BUFFER_SIZE);
    if (!priv->comm.buffer) {
	xf86CloseSerial(pInfo->fd);
	pInfo->fd = -1;
	return !Success;
    }

    xf86FlushInput(pInfo->fd);

    /* reinit the pad */
    if (!QueryHardware(pInfo))
    {
        XisbFree(priv->comm.buffer);
        priv->comm.buffer = NULL;
        xf86CloseSerial(pInfo->fd);
        pInfo->fd = -1;
        return !Success;
    }

    xf86AddEnabledDevice(pInfo);
    dev->public.on = TRUE;

    return Success;
}

static Bool
DeviceOff(DeviceIntPtr dev)
{
    InputInfoPtr pInfo = dev->public.devicePrivate;
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);

    DBG(3, "Synaptics DeviceOff called\n");

    if (pInfo->fd != -1) {
	TimerCancel(priv->timer);
	xf86RemoveEnabledDevice(pInfo);
        if (priv->proto_ops->DeviceOffHook)
            priv->proto_ops->DeviceOffHook(pInfo);
	if (priv->comm.buffer) {
	    XisbFree(priv->comm.buffer);
	    priv->comm.buffer = NULL;
	}
	xf86CloseSerial(pInfo->fd);
	pInfo->fd = -1;
    }
    dev->public.on = FALSE;
    return Success;
}

static Bool
DeviceClose(DeviceIntPtr dev)
{
    Bool RetValue;
    InputInfoPtr pInfo = dev->public.devicePrivate;
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;

    RetValue = DeviceOff(dev);
    TimerFree(priv->timer);
    priv->timer = NULL;
    free_shm_data(priv);
    return RetValue;
}

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
static void InitAxesLabels(Atom *labels, int nlabels)
{
    memset(labels, 0, nlabels * sizeof(Atom));
    switch(nlabels)
    {
        default:
        case 2:
            labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_Y);
        case 1:
            labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_X);
            break;
    }
}

static void InitButtonLabels(Atom *labels, int nlabels)
{
    memset(labels, 0, nlabels * sizeof(Atom));
    switch(nlabels)
    {
        default:
        case 7:
            labels[6] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_HWHEEL_RIGHT);
        case 6:
            labels[5] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_HWHEEL_LEFT);
        case 5:
            labels[4] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_WHEEL_DOWN);
        case 4:
            labels[3] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_WHEEL_UP);
        case 3:
            labels[2] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_RIGHT);
        case 2:
            labels[1] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_MIDDLE);
        case 1:
            labels[0] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_LEFT);
            break;
    }
}
#endif

static Bool
DeviceInit(DeviceIntPtr dev)
{
    InputInfoPtr pInfo = dev->public.devicePrivate;
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    Atom float_type, prop;
    float tmpf;
    unsigned char map[SYN_MAX_BUTTONS + 1];
    int i;
    int min, max;
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
    Atom btn_labels[SYN_MAX_BUTTONS] = { 0 };
    Atom axes_labels[2] = { 0 };
    DeviceVelocityPtr pVel;

    InitAxesLabels(axes_labels, 2);
    InitButtonLabels(btn_labels, SYN_MAX_BUTTONS);
#endif

    DBG(3, "Synaptics DeviceInit called\n");

    for (i = 0; i <= SYN_MAX_BUTTONS; i++)
	map[i] = i;

    dev->public.on = FALSE;

    InitPointerDeviceStruct((DevicePtr)dev, map,
			    SYN_MAX_BUTTONS,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
                            btn_labels,
#endif
			    SynapticsCtrl,
			    GetMotionHistorySize(), 2
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
                            , axes_labels
#endif
			    );

    /*
     * setup dix acceleration to match legacy synaptics settings, and
     * etablish a device-specific profile to do stuff like pressure-related
     * acceleration.
     */
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
    if (NULL != (pVel = GetDevicePredictableAccelData(dev))) {
	SetDeviceSpecificAccelerationProfile(pVel,
	                                     SynapticsAccelerationProfile);

	/* float property type */
	float_type = XIGetKnownProperty(XATOM_FLOAT);

	/* translate MinAcc to constant deceleration.
	 * May be overridden in xf86InitValuatorDefaults */
	tmpf = 1.0 / priv->synpara.min_speed;

	xf86Msg(X_CONFIG, "%s: (accel) MinSpeed is now constant deceleration "
	        "%.1f\n", dev->name, tmpf);
	prop = XIGetKnownProperty(ACCEL_PROP_CONSTANT_DECELERATION);
	XIChangeDeviceProperty(dev, prop, float_type, 32,
	                       PropModeReplace, 1, &tmpf, FALSE);

	/* adjust accordingly */
	priv->synpara.max_speed /= priv->synpara.min_speed;
	priv->synpara.min_speed = 1.0;

	/* synaptics seems to report 80 packet/s, but dix scales for
	 * 100 packet/s by default. */
	pVel->corr_mul = 12.5f; /*1000[ms]/80[/s] = 12.5 */

	xf86Msg(X_CONFIG, "%s: MaxSpeed is now %.2f\n",
		dev->name, priv->synpara.max_speed);
	xf86Msg(X_CONFIG, "%s: AccelFactor is now %.3f\n",
		dev->name, priv->synpara.accl);

	prop = XIGetKnownProperty(ACCEL_PROP_PROFILE_NUMBER);
	i = AccelProfileDeviceSpecific;
	XIChangeDeviceProperty(dev, prop, XA_INTEGER, 32,
	                       PropModeReplace, 1, &i, FALSE);
    }
#endif

    /* X valuator */
    if (priv->minx < priv->maxx)
    {
        min = priv->minx;
        max = priv->maxx;
    } else
    {
        min = 0;
        max = -1;
    }

    xf86InitValuatorAxisStruct(dev, 0,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
            axes_labels[0],
#endif
            min, max, priv->resx * 1000, 0, priv->resx * 1000
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
            , Relative
#endif
            );
    xf86InitValuatorDefaults(dev, 0);

    /* Y valuator */
    if (priv->miny < priv->maxy)
    {
        min = priv->miny;
        max = priv->maxy;
    } else
    {
        min = 0;
        max = -1;
    }

    xf86InitValuatorAxisStruct(dev, 1,
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 7
            axes_labels[1],
#endif
            min, max, priv->resy * 1000, 0, priv->resy * 1000
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 12
            , Relative
#endif
            );
    xf86InitValuatorDefaults(dev, 1);

    if (!alloc_shm_data(pInfo))
	return !Success;

    InitDeviceProperties(pInfo);
    XIRegisterPropertyHandler(pInfo->dev, SetProperty, NULL, NULL);

    return Success;
}


/*
 * Convert from absolute X/Y coordinates to a coordinate system where
 * -1 corresponds to the left/upper edge and +1 corresponds to the
 * right/lower edge.
 */
static void
relative_coords(SynapticsPrivate *priv, int x, int y,
		double *relX, double *relY)
{
    int minX = priv->synpara.left_edge;
    int maxX = priv->synpara.right_edge;
    int minY = priv->synpara.top_edge;
    int maxY = priv->synpara.bottom_edge;
    double xCenter = (minX + maxX) / 2.0;
    double yCenter = (minY + maxY) / 2.0;

    if ((maxX - xCenter > 0) && (maxY - yCenter > 0)) {
	*relX = (x - xCenter) / (maxX - xCenter);
	*relY = (y - yCenter) / (maxY - yCenter);
    } else {
	*relX = 0;
	*relY = 0;
    }
}

/* return angle of point relative to center */
static double
angle(SynapticsPrivate *priv, int x, int y)
{
    double xCenter = (priv->synpara.left_edge + priv->synpara.right_edge) / 2.0;
    double yCenter = (priv->synpara.top_edge + priv->synpara.bottom_edge) / 2.0;

    return atan2(-(y - yCenter), x - xCenter);
}

/* return angle difference */
static double
diffa(double a1, double a2)
{
    double da = fmod(a2 - a1, 2 * M_PI);
    if (da < 0)
	da += 2 * M_PI;
    if (da > M_PI)
	da -= 2 * M_PI;
    return da;
}

static edge_type
circular_edge_detection(SynapticsPrivate *priv, int x, int y)
{
    edge_type edge = 0;
    double relX, relY, relR;

    relative_coords(priv, x, y, &relX, &relY);
    relR = SQR(relX) + SQR(relY);

    if (relR > 1) {
	/* we are outside the ellipse enclosed by the edge parameters */
	if (relX > M_SQRT1_2)
	    edge |= RIGHT_EDGE;
	else if (relX < -M_SQRT1_2)
	    edge |= LEFT_EDGE;

	if (relY < -M_SQRT1_2)
	    edge |= TOP_EDGE;
	else if (relY > M_SQRT1_2)
	    edge |= BOTTOM_EDGE;
    }

    return edge;
}

static edge_type
edge_detection(SynapticsPrivate *priv, int x, int y)
{
    edge_type edge = NO_EDGE;

    if (priv->synpara.circular_pad)
	return circular_edge_detection(priv, x, y);

    if (x > priv->synpara.right_edge)
	edge |= RIGHT_EDGE;
    else if (x < priv->synpara.left_edge)
	edge |= LEFT_EDGE;

    if (y < priv->synpara.top_edge)
	edge |= TOP_EDGE;
    else if (y > priv->synpara.bottom_edge)
	edge |= BOTTOM_EDGE;

    return edge;
}

/* Checks whether coordinates are in the Synaptics Area
 * or not. If no Synaptics Area is defined (i.e. if
 * priv->synpara.area_{left|right|top|bottom}_edge are
 * all set to zero), the function returns TRUE.
 */
static Bool
is_inside_active_area(SynapticsPrivate *priv, int x, int y)
{
    Bool inside_area = TRUE;

    if ((priv->synpara.area_left_edge != 0) && (x < priv->synpara.area_left_edge))
	inside_area = FALSE;
    else if ((priv->synpara.area_right_edge != 0) && (x > priv->synpara.area_right_edge))
	inside_area = FALSE;

    if ((priv->synpara.area_top_edge != 0) && (y < priv->synpara.area_top_edge))
	inside_area = FALSE;
    else if ((priv->synpara.area_bottom_edge != 0) && (y > priv->synpara.area_bottom_edge))
	inside_area = FALSE;

    return inside_area;
}

static CARD32
timerFunc(OsTimerPtr timer, CARD32 now, pointer arg)
{
    InputInfoPtr pInfo = arg;
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    struct SynapticsHwState hw;
    int delay;
    int sigstate;
    CARD32 wakeUpTime;

    sigstate = xf86BlockSIGIO();

    hw = priv->hwState;
    hw.millis = now;
    delay = HandleState(pInfo, &hw);

    /*
     * Workaround for wraparound bug in the TimerSet function. This bug is already
     * fixed in CVS, but this driver needs to work with XFree86 versions 4.2.x and
     * 4.3.x too.
     */
    wakeUpTime = now + delay;
    if (wakeUpTime <= now)
	wakeUpTime = 0xffffffffL;

    priv->timer = TimerSet(priv->timer, TimerAbsolute, wakeUpTime, timerFunc, pInfo);

    xf86UnblockSIGIO(sigstate);

    return 0;
}

static int
clamp(int val, int min, int max)
{
    if (val < min)
	return min;
    else if (val < max)
	return val;
    else
	return max;
}

static Bool
SynapticsGetHwState(InputInfoPtr pInfo, SynapticsPrivate *priv,
		    struct SynapticsHwState *hw)
{
    return priv->proto_ops->ReadHwState(pInfo, priv->proto_ops,
					&priv->comm, hw);
}

/*
 *  called for each full received packet from the touchpad
 */
static void
ReadInput(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    struct SynapticsHwState hw;
    int delay = 0;
    Bool newDelay = FALSE;

    while (SynapticsGetHwState(pInfo, priv, &hw)) {
	hw.millis = GetTimeInMillis();
	priv->hwState = hw;
	delay = HandleState(pInfo, &hw);
	newDelay = TRUE;
    }

    if (newDelay)
	priv->timer = TimerSet(priv->timer, 0, delay, timerFunc, pInfo);
}

static int
HandleMidButtonEmulation(SynapticsPrivate *priv, struct SynapticsHwState *hw, int *delay)
{
    SynapticsParameters *para = &priv->synpara;
    Bool done = FALSE;
    int timeleft;
    int mid = 0;

    while (!done) {
	switch (priv->mid_emu_state) {
	case MBE_LEFT_CLICK:
	case MBE_RIGHT_CLICK:
	case MBE_OFF:
	    priv->button_delay_millis = hw->millis;
	    if (hw->left) {
		priv->mid_emu_state = MBE_LEFT;
	    } else if (hw->right) {
		priv->mid_emu_state = MBE_RIGHT;
	    } else {
		done = TRUE;
	    }
	    break;
	case MBE_LEFT:
	    timeleft = TIME_DIFF(priv->button_delay_millis + para->emulate_mid_button_time,
				 hw->millis);
	    if (timeleft > 0)
		*delay = MIN(*delay, timeleft);

            /* timeout, but within the same ReadInput cycle! */
            if ((timeleft <= 0) && !hw->left) {
		priv->mid_emu_state = MBE_LEFT_CLICK;
		done = TRUE;
            } else if ((!hw->left) || (timeleft <= 0)) {
		hw->left = TRUE;
		priv->mid_emu_state = MBE_TIMEOUT;
		done = TRUE;
	    } else if (hw->right) {
		priv->mid_emu_state = MBE_MID;
	    } else {
		hw->left = FALSE;
		done = TRUE;
	    }
	    break;
	case MBE_RIGHT:
	    timeleft = TIME_DIFF(priv->button_delay_millis + para->emulate_mid_button_time,
				 hw->millis);
	    if (timeleft > 0)
		*delay = MIN(*delay, timeleft);

	     /* timeout, but within the same ReadInput cycle! */
            if ((timeleft <= 0) && !hw->right) {
		priv->mid_emu_state = MBE_RIGHT_CLICK;
		done = TRUE;
            } else if (!hw->right || (timeleft <= 0)) {
		hw->right = TRUE;
		priv->mid_emu_state = MBE_TIMEOUT;
		done = TRUE;
	    } else if (hw->left) {
		priv->mid_emu_state = MBE_MID;
	    } else {
		hw->right = FALSE;
		done = TRUE;
	    }
	    break;
	case MBE_MID:
	    if (!hw->left && !hw->right) {
		priv->mid_emu_state = MBE_OFF;
	    } else {
		mid = TRUE;
		hw->left = hw->right = FALSE;
		done = TRUE;
	    }
	    break;
	case MBE_TIMEOUT:
	    if (!hw->left && !hw->right) {
		priv->mid_emu_state = MBE_OFF;
	    } else {
		done = TRUE;
	    }
	}
    }
    return mid;
}

static enum FingerState
SynapticsDetectFinger(SynapticsPrivate *priv, struct SynapticsHwState *hw)
{
    SynapticsParameters *para = &priv->synpara;
    enum FingerState finger;

    /* finger detection thru pressure and threshold */
    if (hw->z > para->finger_press && priv->finger_state < FS_PRESSED)
        finger = FS_PRESSED;
    else if (hw->z > para->finger_high && priv->finger_state < FS_TOUCHED)
        finger = FS_TOUCHED;
    else if (hw->z < para->finger_low &&  priv->finger_state > FS_UNTOUCHED)
        finger = FS_UNTOUCHED;
    else
	finger = priv->finger_state;

    if (!para->palm_detect)
	return finger;

    /* palm detection */
    if (finger) {
	if ((hw->z > para->palm_min_z) && (hw->fingerWidth > para->palm_min_width))
	    priv->palm = TRUE;
    } else {
	priv->palm = FALSE;
    }
    if (hw->x == 0)
	priv->avg_width = 0;
    else
	priv->avg_width += (hw->fingerWidth - priv->avg_width + 1) / 2;
    if (finger && !priv->finger_state) {
	int safe_width = MAX(hw->fingerWidth, priv->avg_width);

	if (hw->numFingers > 1 ||	/* more than one finger -> not a palm */
	    ((safe_width < 6) && (priv->prev_z < para->finger_high)) ||  /* thin finger, distinct touch -> not a palm */
	    ((safe_width < 7) && (priv->prev_z < para->finger_high / 2)))/* thin finger, distinct touch -> not a palm */
	{
	    /* leave finger value as is */
	} else if (hw->z > priv->prev_z + 1)	/* z not stable, may be a palm */
	    finger = FS_UNTOUCHED;
	else if (hw->z < priv->prev_z - 5)	/* z not stable, may be a palm */
	    finger = FS_UNTOUCHED;
	else if (hw->fingerWidth > para->palm_min_width) /* finger width too large -> probably palm */
	    finger = FS_UNTOUCHED;
    }
    priv->prev_z = hw->z;

    if (priv->palm)
	finger = FS_UNTOUCHED;

    return finger;
}

static void
SelectTapButton(SynapticsPrivate *priv, edge_type edge)
{
    TapEvent tap;

    if (priv->synpara.touchpad_off == 2) {
	priv->tap_button = 0;
	return;
    }

    switch (priv->tap_max_fingers) {
    case 1:
    default:
	switch (edge) {
	case RIGHT_TOP_EDGE:
	    DBG(7, "right top edge\n");
	    tap = RT_TAP;
	    break;
	case RIGHT_BOTTOM_EDGE:
	    DBG(7, "right bottom edge\n");
	    tap = RB_TAP;
	    break;
	case LEFT_TOP_EDGE:
	    DBG(7, "left top edge\n");
	    tap = LT_TAP;
	    break;
	case LEFT_BOTTOM_EDGE:
	    DBG(7, "left bottom edge\n");
	    tap = LB_TAP;
	    break;
	default:
	    DBG(7, "no edge\n");
	    tap = F1_TAP;
	    break;
	}
	break;
    case 2:
	DBG(7, "two finger tap\n");
	tap = F2_TAP;
	break;
    case 3:
	DBG(7, "three finger tap\n");
	tap = F3_TAP;
	break;
    }

    priv->tap_button = priv->synpara.tap_action[tap];
    priv->tap_button = clamp(priv->tap_button, 0, SYN_MAX_BUTTONS);
}

static void
SetTapState(SynapticsPrivate *priv, enum TapState tap_state, int millis)
{
    SynapticsParameters *para = &priv->synpara;
    DBG(7, "SetTapState - %d -> %d (millis:%d)\n", priv->tap_state, tap_state, millis);
    switch (tap_state) {
    case TS_START:
	priv->tap_button_state = TBS_BUTTON_UP;
	priv->tap_max_fingers = 0;
	break;
    case TS_1:
	priv->tap_button_state = TBS_BUTTON_UP;
	break;
    case TS_2A:
	if (para->fast_taps)
	    priv->tap_button_state = TBS_BUTTON_DOWN;
	else
	    priv->tap_button_state = TBS_BUTTON_UP;
	break;
    case TS_2B:
	priv->tap_button_state = TBS_BUTTON_UP;
	break;
    case TS_3:
	if (para->tap_and_drag_gesture)
	    priv->tap_button_state = TBS_BUTTON_DOWN;
	else
	    priv->tap_button_state = TBS_BUTTON_UP;
	break;
    case TS_SINGLETAP:
	if (para->fast_taps)
	    priv->tap_button_state = TBS_BUTTON_UP;
	else
	    priv->tap_button_state = TBS_BUTTON_DOWN;
	priv->touch_on.millis = millis;
	break;
    default:
	break;
    }
    priv->tap_state = tap_state;
}

static void
SetMovingState(SynapticsPrivate *priv, enum MovingState moving_state, int millis)
{
    DBG(7, "SetMovingState - %d -> %d center at %d/%d (millis:%d)\n", priv->moving_state,
		  moving_state,priv->hwState.x, priv->hwState.y, millis);

    if (moving_state == MS_TRACKSTICK) {
	priv->trackstick_neutral_x = priv->hwState.x;
	priv->trackstick_neutral_y = priv->hwState.y;
    }
    priv->moving_state = moving_state;
}

static int
GetTimeOut(SynapticsPrivate *priv)
{
    SynapticsParameters *para = &priv->synpara;

    switch (priv->tap_state) {
    case TS_1:
    case TS_3:
    case TS_5:
	return para->tap_time;
    case TS_SINGLETAP:
	return para->click_time;
    case TS_2A:
	return para->single_tap_timeout;
    case TS_2B:
	return para->tap_time_2;
    case TS_4:
	return para->locked_drag_time;
    default:
	return -1;			    /* No timeout */
    }
}

static int
HandleTapProcessing(SynapticsPrivate *priv, struct SynapticsHwState *hw,
		    enum FingerState finger, Bool inside_active_area)
{
    SynapticsParameters *para = &priv->synpara;
    Bool touch, release, is_timeout, move;
    int timeleft, timeout;
    edge_type edge;
    int delay = 1000000000;

    if (priv->palm)
	return delay;

    touch = finger && !priv->finger_state;
    release = !finger && priv->finger_state;
    move = (finger &&
	     (priv->tap_max_fingers <= ((priv->horiz_scroll_twofinger_on || priv->vert_scroll_twofinger_on)? 2 : 1)) &&
	     ((abs(hw->x - priv->touch_on.x) >= para->tap_move) ||
	     (abs(hw->y - priv->touch_on.y) >= para->tap_move)));

    if (touch) {
	priv->touch_on.x = hw->x;
	priv->touch_on.y = hw->y;
	priv->touch_on.millis = hw->millis;
    } else if (release) {
	priv->touch_on.millis = hw->millis;
    }
    if (hw->z > para->finger_high)
	if (priv->tap_max_fingers < hw->numFingers)
	    priv->tap_max_fingers = hw->numFingers;
    timeout = GetTimeOut(priv);
    timeleft = TIME_DIFF(priv->touch_on.millis + timeout, hw->millis);
    is_timeout = timeleft <= 0;

 restart:
    switch (priv->tap_state) {
    case TS_START:
	if (touch)
	    SetTapState(priv, TS_1, hw->millis);
	break;
    case TS_1:
	if (move) {
	    SetMovingState(priv, MS_TOUCHPAD_RELATIVE, hw->millis);
	    SetTapState(priv, TS_MOVE, hw->millis);
	    goto restart;
	} else if (is_timeout) {
	    if (finger == FS_TOUCHED) {
		SetMovingState(priv, MS_TOUCHPAD_RELATIVE, hw->millis);
	    } else if (finger == FS_PRESSED) {
		SetMovingState(priv, MS_TRACKSTICK, hw->millis);
	    }
	    SetTapState(priv, TS_MOVE, hw->millis);
	    goto restart;
	} else if (release) {
	    edge = edge_detection(priv, priv->touch_on.x, priv->touch_on.y);
	    SelectTapButton(priv, edge);
	    /* Disable taps outside of the active area */
	    if (!inside_active_area) {
		priv->tap_button = 0;
	    }
	    SetTapState(priv, TS_2A, hw->millis);
	}
	break;
    case TS_MOVE:
	if (move && priv->moving_state == MS_TRACKSTICK) {
	    SetMovingState(priv, MS_TOUCHPAD_RELATIVE, hw->millis);
	}
	if (release) {
	    SetMovingState(priv, MS_FALSE, hw->millis);
	    SetTapState(priv, TS_START, hw->millis);
	}
	break;
    case TS_2A:
	if (touch)
	    SetTapState(priv, TS_3, hw->millis);
	else if (is_timeout)
	    SetTapState(priv, TS_SINGLETAP, hw->millis);
	break;
    case TS_2B:
	if (touch) {
	    SetTapState(priv, TS_3, hw->millis);
	} else if (is_timeout) {
	    SetTapState(priv, TS_START, hw->millis);
	    priv->tap_button_state = TBS_BUTTON_DOWN_UP;
	}
	break;
    case TS_SINGLETAP:
	if (touch)
	    SetTapState(priv, TS_1, hw->millis);
	else if (is_timeout)
	    SetTapState(priv, TS_START, hw->millis);
	break;
    case TS_3:
	if (move) {
	    if (para->tap_and_drag_gesture) {
		SetMovingState(priv, MS_TOUCHPAD_RELATIVE, hw->millis);
		SetTapState(priv, TS_DRAG, hw->millis);
	    } else {
		SetTapState(priv, TS_1, hw->millis);
	    }
	    goto restart;
	} else if (is_timeout) {
	    if (para->tap_and_drag_gesture) {
		if (finger == FS_TOUCHED) {
		    SetMovingState(priv, MS_TOUCHPAD_RELATIVE, hw->millis);
		} else if (finger == FS_PRESSED) {
		    SetMovingState(priv, MS_TRACKSTICK, hw->millis);
		}
		SetTapState(priv, TS_DRAG, hw->millis);
	    } else {
		SetTapState(priv, TS_1, hw->millis);
	    }
	    goto restart;
	} else if (release) {
	    SetTapState(priv, TS_2B, hw->millis);
	}
	break;
    case TS_DRAG:
	if (move)
	    SetMovingState(priv, MS_TOUCHPAD_RELATIVE, hw->millis);
	if (release) {
	    SetMovingState(priv, MS_FALSE, hw->millis);
	    if (para->locked_drags) {
		SetTapState(priv, TS_4, hw->millis);
	    } else {
		SetTapState(priv, TS_START, hw->millis);
	    }
	}
	break;
    case TS_4:
	if (is_timeout) {
	    SetTapState(priv, TS_START, hw->millis);
	    goto restart;
	}
	if (touch)
	    SetTapState(priv, TS_5, hw->millis);
	break;
    case TS_5:
	if (is_timeout || move) {
	    SetTapState(priv, TS_DRAG, hw->millis);
	    goto restart;
	} else if (release) {
	    SetMovingState(priv, MS_FALSE, hw->millis);
	    SetTapState(priv, TS_START, hw->millis);
	}
	break;
    }

    timeout = GetTimeOut(priv);
    if (timeout >= 0) {
	timeleft = TIME_DIFF(priv->touch_on.millis + timeout, hw->millis);
	delay = clamp(timeleft, 1, delay);
    }
    return delay;
}

#define HIST(a) (priv->move_hist[((priv->hist_index - (a) + SYNAPTICS_MOVE_HISTORY) % SYNAPTICS_MOVE_HISTORY)])

static void
store_history(SynapticsPrivate *priv, int x, int y, unsigned int millis)
{
    int idx = (priv->hist_index + 1) % SYNAPTICS_MOVE_HISTORY;
    priv->move_hist[idx].x = x;
    priv->move_hist[idx].y = y;
    priv->move_hist[idx].millis = millis;
    priv->hist_index = idx;
}

/*
 * Estimate the slope for the data sequence [x3, x2, x1, x0] by using
 * linear regression to fit a line to the data and use the slope of the
 * line.
 */
static double
estimate_delta(double x0, double x1, double x2, double x3)
{
    return x0 * 0.3 + x1 * 0.1 - x2 * 0.1 - x3 * 0.3;
}

/**
 * Applies hysteresis. center is shifted such that it is in range with
 * in by the margin again. The new center is returned.
 * @param in the current value
 * @param center the current center
 * @param margin the margin to center in which no change is applied
 * @return the new center (which might coincide with the previous)
 */
static int hysteresis(int in, int center, int margin) {
    int diff = in - center;
    if (abs(diff) <= margin) {
	diff = 0;
    } else if (diff > margin) {
	diff -= margin;
    } else if (diff < -margin) {
	diff += margin;
    }
    return center + diff;
}

static void
get_delta_for_trackstick(SynapticsPrivate *priv, const struct SynapticsHwState *hw,
                         double *dx, double *dy)
{
    SynapticsParameters *para = &priv->synpara;
    double dtime = (hw->millis - HIST(0).millis) / 1000.0;

    *dx = (hw->x - priv->trackstick_neutral_x);
    *dy = (hw->y - priv->trackstick_neutral_y);

    *dx = *dx * dtime * para->trackstick_speed;
    *dy = *dy * dtime * para->trackstick_speed;
}

static void
get_edge_speed(SynapticsPrivate *priv, const struct SynapticsHwState *hw,
               edge_type edge, int *x_edge_speed, int *y_edge_speed)
{
    SynapticsParameters *para = &priv->synpara;

    int minZ = para->edge_motion_min_z;
    int maxZ = para->edge_motion_max_z;
    int minSpd = para->edge_motion_min_speed;
    int maxSpd = para->edge_motion_max_speed;
    int edge_speed;

    if (hw->z <= minZ) {
        edge_speed = minSpd;
    } else if (hw->z >= maxZ) {
        edge_speed = maxSpd;
    } else {
        edge_speed = minSpd + (hw->z - minZ) * (maxSpd - minSpd) / (maxZ - minZ);
    }
    if (!priv->synpara.circular_pad) {
        /* on rectangular pad */
        if (edge & RIGHT_EDGE) {
            *x_edge_speed = edge_speed;
        } else if (edge & LEFT_EDGE) {
            *x_edge_speed = -edge_speed;
        }
        if (edge & TOP_EDGE) {
            *y_edge_speed = -edge_speed;
        } else if (edge & BOTTOM_EDGE) {
            *y_edge_speed = edge_speed;
        }
    } else if (edge) {
        /* at edge of circular pad */
        double relX, relY;

        relative_coords(priv, hw->x, hw->y, &relX, &relY);
        *x_edge_speed = (int)(edge_speed * relX);
        *y_edge_speed = (int)(edge_speed * relY);
    }
}

static void
get_delta(SynapticsPrivate *priv, const struct SynapticsHwState *hw,
          edge_type edge, double *dx, double *dy)
{
    SynapticsParameters *para = &priv->synpara;
    double dtime = (hw->millis - HIST(0).millis) / 1000.0;
    double integral;
    double tmpf;
    int x_edge_speed = 0;
    int y_edge_speed = 0;

    /* HIST is full enough: priv->count_packet_finger > 3 */
    *dx = estimate_delta(hw->x, HIST(0).x, HIST(1).x, HIST(2).x);
    *dy = estimate_delta(hw->y, HIST(0).y, HIST(1).y, HIST(2).y);

    if ((priv->tap_state == TS_DRAG) || para->edge_motion_use_always)
        get_edge_speed(priv, hw, edge, &x_edge_speed, &y_edge_speed);

    /* report edge speed as synthetic motion. Of course, it would be
     * cooler to report floats than to buffer, but anyway. */
    tmpf = *dx + x_edge_speed * dtime + priv->frac_x;
    priv->frac_x = modf(tmpf, &integral);
    *dx = integral;
    tmpf = *dy + y_edge_speed * dtime + priv->frac_y;
    priv->frac_y = modf(tmpf, &integral);
    *dy = integral;
}

/**
 * Compute relative motion ('deltas') including edge motion xor trackstick.
 */
static int
ComputeDeltas(SynapticsPrivate *priv, const struct SynapticsHwState *hw,
	      edge_type edge, int *dxP, int *dyP, Bool inside_area)
{
    enum MovingState moving_state;
    double dx, dy;
    int delay = 1000000000;

    dx = dy = 0;

    moving_state = priv->moving_state;
    if (moving_state == MS_FALSE) {
	switch (priv->tap_state) {
	case TS_MOVE:
	case TS_DRAG:
	    moving_state = MS_TOUCHPAD_RELATIVE;
	    break;
	case TS_1:
	case TS_3:
	case TS_5:
	    if (hw->numFingers == 1)
		moving_state = MS_TOUCHPAD_RELATIVE;
	    break;
	default:
	    break;
	}
    }

    if (!inside_area || !moving_state || priv->palm ||
	priv->vert_scroll_edge_on || priv->horiz_scroll_edge_on ||
	priv->vert_scroll_twofinger_on || priv->horiz_scroll_twofinger_on ||
	priv->circ_scroll_on || priv->prevFingers != hw->numFingers)
    {
        /* reset packet counter. */
        priv->count_packet_finger = 0;
        goto out;
    }

    /* to create fluid edge motion, call back 'soon'
     * even in the absence of new hardware events */
    delay = MIN(delay, 13);

    if (priv->count_packet_finger <= 3) /* min. 3 packets, see get_delta() */
        goto skip; /* skip the lot */

    if (priv->moving_state == MS_TRACKSTICK)
        get_delta_for_trackstick(priv, hw, &dx, &dy);
    else if (moving_state == MS_TOUCHPAD_RELATIVE)
        get_delta(priv, hw, edge, &dx, &dy);

skip:
    priv->count_packet_finger++;
out:
    priv->prevFingers = hw->numFingers;

    *dxP = dx;
    *dyP = dy;

    return delay;
}

struct ScrollData {
    int left, right, up, down;
};

static void
start_coasting(SynapticsPrivate *priv, struct SynapticsHwState *hw, edge_type edge,
	       Bool vertical)
{
    SynapticsParameters *para = &priv->synpara;

    priv->autoscroll_y = 0.0;
    priv->autoscroll_x = 0.0;

    if ((priv->scroll_packet_count > 3) && (para->coasting_speed > 0.0)) {
	double pkt_time = (HIST(0).millis - HIST(3).millis) / 1000.0;
	if (para->scroll_twofinger_vert || vertical) {
	    double dy = estimate_delta(HIST(0).y, HIST(1).y, HIST(2).y, HIST(3).y);
	    int sdelta = para->scroll_dist_vert;
	    if ((para->scroll_twofinger_vert || (edge & RIGHT_EDGE)) && pkt_time > 0 && sdelta > 0) {
		double scrolls_per_sec = dy / pkt_time / sdelta;
		if (fabs(scrolls_per_sec) >= para->coasting_speed) {
		    priv->autoscroll_yspd = scrolls_per_sec;
		    priv->autoscroll_y = (hw->y - priv->scroll_y) / (double)sdelta;
		}
	    }
	}
	if (para->scroll_twofinger_horiz || !vertical){
	    double dx = estimate_delta(HIST(0).x, HIST(1).x, HIST(2).x, HIST(3).x);
	    int sdelta = para->scroll_dist_horiz;
	    if ((para->scroll_twofinger_horiz || (edge & BOTTOM_EDGE)) && pkt_time > 0 && sdelta > 0) {
		double scrolls_per_sec = dx / pkt_time / sdelta;
		if (fabs(scrolls_per_sec) >= para->coasting_speed) {
		    priv->autoscroll_xspd = scrolls_per_sec;
		    priv->autoscroll_x = (hw->x - priv->scroll_x) / (double)sdelta;
		}
	    }
	}
    }
    priv->scroll_packet_count = 0;
}

static void
stop_coasting(SynapticsPrivate *priv)
{
    priv->autoscroll_xspd = 0;
    priv->autoscroll_yspd = 0;
    priv->scroll_packet_count = 0;
}

static int
HandleScrolling(SynapticsPrivate *priv, struct SynapticsHwState *hw,
		edge_type edge, Bool finger, struct ScrollData *sd)
{
    SynapticsParameters *para = &priv->synpara;
    int delay = 1000000000;

    sd->left = sd->right = sd->up = sd->down = 0;

    if (priv->synpara.touchpad_off == 2) {
	stop_coasting(priv);
	priv->circ_scroll_on = FALSE;
	priv->vert_scroll_edge_on = FALSE;
	priv->horiz_scroll_edge_on = FALSE;
	priv->vert_scroll_twofinger_on = FALSE;
	priv->horiz_scroll_twofinger_on = FALSE;
	return delay;
    }

    /* scroll detection */
    if (finger && !priv->finger_state) {
	stop_coasting(priv);
	if (para->circular_scrolling) {
	    if ((para->circular_trigger == 0 && edge) ||
		(para->circular_trigger == 1 && edge & TOP_EDGE) ||
		(para->circular_trigger == 2 && edge & TOP_EDGE && edge & RIGHT_EDGE) ||
		(para->circular_trigger == 3 && edge & RIGHT_EDGE) ||
		(para->circular_trigger == 4 && edge & RIGHT_EDGE && edge & BOTTOM_EDGE) ||
		(para->circular_trigger == 5 && edge & BOTTOM_EDGE) ||
		(para->circular_trigger == 6 && edge & BOTTOM_EDGE && edge & LEFT_EDGE) ||
		(para->circular_trigger == 7 && edge & LEFT_EDGE) ||
		(para->circular_trigger == 8 && edge & LEFT_EDGE && edge & TOP_EDGE)) {
		priv->circ_scroll_on = TRUE;
		priv->circ_scroll_vert = TRUE;
		priv->scroll_a = angle(priv, hw->x, hw->y);
		DBG(7, "circular scroll detected on edge\n");
	    }
	}
    }
    if (!priv->circ_scroll_on) {
	if (finger) {
	    if (hw->numFingers == 2) {
		if (!priv->vert_scroll_twofinger_on &&
		    (para->scroll_twofinger_vert) && (para->scroll_dist_vert != 0)) {
		    priv->vert_scroll_twofinger_on = TRUE;
		    priv->vert_scroll_edge_on = FALSE;
		    priv->scroll_y = hw->y;
		    DBG(7, "vert two-finger scroll detected\n");
		}
		if (!priv->horiz_scroll_twofinger_on &&
		    (para->scroll_twofinger_horiz) && (para->scroll_dist_horiz != 0)) {
		    priv->horiz_scroll_twofinger_on = TRUE;
		    priv->horiz_scroll_edge_on = FALSE;
		    priv->scroll_x = hw->x;
		    DBG(7, "horiz two-finger scroll detected\n");
		}
	    }
	}
	if (finger && !priv->finger_state) {
	    if (!priv->vert_scroll_twofinger_on && !priv->horiz_scroll_twofinger_on) {
		if ((para->scroll_edge_vert) && (para->scroll_dist_vert != 0) &&
		    (edge & RIGHT_EDGE)) {
		    priv->vert_scroll_edge_on = TRUE;
		    priv->scroll_y = hw->y;
		    DBG(7, "vert edge scroll detected on right edge\n");
		}
		if ((para->scroll_edge_horiz) && (para->scroll_dist_horiz != 0) &&
		    (edge & BOTTOM_EDGE)) {
		    priv->horiz_scroll_edge_on = TRUE;
		    priv->scroll_x = hw->x;
		    DBG(7, "horiz edge scroll detected on bottom edge\n");
		}
	    }
	}
    }
    {
	Bool oldv = priv->vert_scroll_twofinger_on || priv->vert_scroll_edge_on ||
	              (priv->circ_scroll_on && priv->circ_scroll_vert);
	Bool oldh = priv->horiz_scroll_twofinger_on || priv->horiz_scroll_edge_on ||
	              (priv->circ_scroll_on && !priv->circ_scroll_vert);
	if (priv->circ_scroll_on && !finger) {
	    /* circular scroll locks in until finger is raised */
	    DBG(7, "cicular scroll off\n");
	    priv->circ_scroll_on = FALSE;
	}

	if (!finger || hw->numFingers != 2) {
	    if (priv->vert_scroll_twofinger_on) {
		DBG(7, "vert two-finger scroll off\n");
		priv->vert_scroll_twofinger_on = FALSE;
	    }
	    if (priv->horiz_scroll_twofinger_on) {
		DBG(7, "horiz two-finger scroll off\n");
		priv->horiz_scroll_twofinger_on = FALSE;
	    }
	}

	if (priv->vert_scroll_edge_on && (!(edge & RIGHT_EDGE) || !finger)) {
	    DBG(7, "vert edge scroll off\n");
	    priv->vert_scroll_edge_on = FALSE;
	}
	if (priv->horiz_scroll_edge_on && (!(edge & BOTTOM_EDGE) || !finger)) {
	    DBG(7, "horiz edge scroll off\n");
	    priv->horiz_scroll_edge_on = FALSE;
	}
	/* If we were corner edge scrolling (coasting),
	 * but no longer in corner or raised a finger, then stop coasting. */
	if (para->scroll_edge_corner && (priv->autoscroll_xspd || priv->autoscroll_yspd)) {
	    Bool is_in_corner =
		((edge & RIGHT_EDGE)  && (edge & (TOP_EDGE | BOTTOM_EDGE))) ||
		((edge & BOTTOM_EDGE) && (edge & (LEFT_EDGE | RIGHT_EDGE))) ;
	    if (!is_in_corner || !finger) {
		DBG(7, "corner edge scroll off\n");
		stop_coasting(priv);
	    }
	}
	/* if we were scrolling, but couldn't corner edge scroll,
	 * and are no longer scrolling, then start coasting */
	if ((oldv || oldh) && !para->scroll_edge_corner &&
	    !(priv->circ_scroll_on || priv->vert_scroll_edge_on ||
	      priv->horiz_scroll_edge_on || priv->horiz_scroll_twofinger_on ||
	      priv->vert_scroll_twofinger_on)) {
	    start_coasting(priv, hw, edge, oldv);
	}
    }

    /* if hitting a corner (top right or bottom right) while vertical
     * scrolling is active, consider starting corner edge scrolling or
     * switching over to circular scrolling smoothly */
    if (priv->vert_scroll_edge_on && !priv->horiz_scroll_edge_on &&
	(edge & RIGHT_EDGE) && (edge & (TOP_EDGE | BOTTOM_EDGE))) {
	if (para->scroll_edge_corner) {
	    if (priv->autoscroll_yspd == 0) {
		/* FYI: We can generate multiple start_coasting requests if
		 * we're in the corner, but we were moving so slowly when we
		 * got here that we didn't actually start coasting. */
		DBG(7, "corner edge scroll on\n");
		start_coasting(priv, hw, edge, TRUE);
	    }
	} else if (para->circular_scrolling) {
	    priv->vert_scroll_edge_on = FALSE;
	    priv->circ_scroll_on = TRUE;
	    priv->circ_scroll_vert = TRUE;
	    priv->scroll_a = angle(priv, hw->x, hw->y);
	    DBG(7, "switching to circular scrolling\n");
	}
    }
    /* Same treatment for horizontal scrolling */
    if (priv->horiz_scroll_edge_on && !priv->vert_scroll_edge_on &&
	(edge & BOTTOM_EDGE) && (edge & (LEFT_EDGE | RIGHT_EDGE))) {
	if (para->scroll_edge_corner) {
	    if (priv->autoscroll_xspd == 0) {
		/* FYI: We can generate multiple start_coasting requests if
		 * we're in the corner, but we were moving so slowly when we
		 * got here that we didn't actually start coasting. */
		DBG(7, "corner edge scroll on\n");
		start_coasting(priv, hw, edge, FALSE);
	    }
	} else if (para->circular_scrolling) {
	    priv->horiz_scroll_edge_on = FALSE;
	    priv->circ_scroll_on = TRUE;
	    priv->circ_scroll_vert = FALSE;
	    priv->scroll_a = angle(priv, hw->x, hw->y);
	    DBG(7, "switching to circular scrolling\n");
	}
    }

    if (priv->vert_scroll_edge_on || priv->horiz_scroll_edge_on ||
	priv->vert_scroll_twofinger_on || priv->horiz_scroll_twofinger_on ||
	priv->circ_scroll_on) {
	priv->scroll_packet_count++;
    }

    if (priv->vert_scroll_edge_on || priv->vert_scroll_twofinger_on) {
	/* + = down, - = up */
	int delta = para->scroll_dist_vert;
	if (delta > 0) {
	    while (hw->y - priv->scroll_y > delta) {
		sd->down++;
		priv->scroll_y += delta;
	    }
	    while (hw->y - priv->scroll_y < -delta) {
		sd->up++;
		priv->scroll_y -= delta;
	    }
	}
    }
    if (priv->horiz_scroll_edge_on || priv->horiz_scroll_twofinger_on) {
	/* + = right, - = left */
	int delta = para->scroll_dist_horiz;
	if (delta > 0) {
	    while (hw->x - priv->scroll_x > delta) {
		sd->right++;
		priv->scroll_x += delta;
	    }
	    while (hw->x - priv->scroll_x < -delta) {
		sd->left++;
		priv->scroll_x -= delta;
	    }
	}
    }
    if (priv->circ_scroll_on) {
	/* + = counter clockwise, - = clockwise */
	double delta = para->scroll_dist_circ;
	if (delta >= 0.005) {
	    while (diffa(priv->scroll_a, angle(priv, hw->x, hw->y)) > delta) {
		if (priv->circ_scroll_vert)
		    sd->up++;
		else
		    sd->right++;
		priv->scroll_a += delta;
		if (priv->scroll_a > M_PI)
		    priv->scroll_a -= 2 * M_PI;
	    }
	    while (diffa(priv->scroll_a, angle(priv, hw->x, hw->y)) < -delta) {
		if (priv->circ_scroll_vert)
		    sd->down++;
		else
		    sd->left++;
		priv->scroll_a -= delta;
		if (priv->scroll_a < -M_PI)
		    priv->scroll_a += 2 * M_PI;
	    }
	}
    }

    if (priv->autoscroll_yspd) {
	double dtime = (hw->millis - HIST(0).millis) / 1000.0;
	double ddy = para->coasting_friction * dtime;
	priv->autoscroll_y += priv->autoscroll_yspd * dtime;
	delay = MIN(delay, 20);
	while (priv->autoscroll_y > 1.0) {
	    sd->down++;
	    priv->autoscroll_y -= 1.0;
	}
	while (priv->autoscroll_y < -1.0) {
	    sd->up++;
	    priv->autoscroll_y += 1.0;
	}
	if (abs(priv->autoscroll_yspd) < ddy) {
	    priv->autoscroll_yspd = 0;
	    priv->scroll_packet_count = 0;
	} else {
	    priv->autoscroll_yspd += (priv->autoscroll_yspd < 0 ? ddy : -1*ddy);
	}
    }

    if (priv->autoscroll_xspd) {
	double dtime = (hw->millis - HIST(0).millis) / 1000.0;
	double ddx = para->coasting_friction * dtime;
	priv->autoscroll_x += priv->autoscroll_xspd * dtime;
	delay = MIN(delay, 20);
	while (priv->autoscroll_x > 1.0) {
	    sd->right++;
	    priv->autoscroll_x -= 1.0;
	}
	while (priv->autoscroll_x < -1.0) {
	    sd->left++;
	    priv->autoscroll_x += 1.0;
	}
	if (abs(priv->autoscroll_xspd) < ddx) {
	    priv->autoscroll_xspd = 0;
	    priv->scroll_packet_count = 0;
	} else {
	    priv->autoscroll_xspd += (priv->autoscroll_xspd < 0 ? ddx : -1*ddx);
	}
    }

    return delay;
}

static void
handle_clickfinger(SynapticsParameters *para, struct SynapticsHwState *hw)
{
    int action = 0;
    switch(hw->numFingers){
        case 1:
            action = para->click_action[F1_CLICK1];
            break;
        case 2:
            action = para->click_action[F2_CLICK1];
            break;
        case 3:
            action = para->click_action[F3_CLICK1];
            break;
    }
    switch(action){
        case 1:
            hw->left = 1;
            break;
        case 2:
            hw->left = 0;
            hw->middle = 1;
            break;
        case 3:
            hw->left = 0;
            hw->right = 1;
            break;
    }
}


/* Update the hardware state in shared memory. This is read-only these days,
 * nothing in the driver reads back from SHM. SHM configuration is a thing of the past.
 */
static void
update_shm(const InputInfoPtr pInfo, const struct SynapticsHwState *hw)
{
    int i;
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    SynapticsSHM *shm = priv->synshm;

    if (!shm)
	    return;

    shm->x = hw->x;
    shm->y = hw->y;
    shm->z = hw->z;
    shm->numFingers = hw->numFingers;
    shm->fingerWidth = hw->fingerWidth;
    shm->left = hw->left;
    shm->right = hw->right;
    shm->up = hw->up;
    shm->down = hw->down;
    for (i = 0; i < 8; i++)
	    shm->multi[i] = hw->multi[i];
    shm->middle = hw->middle;
}

/* Adjust the hardware state according to the extra buttons (if the touchpad
 * has any and not many touchpads do these days). These buttons are up/down
 * tilt buttons and/or left/right buttons that then map into a specific
 * function (or scrolling into).
 */
static Bool
adjust_state_from_scrollbuttons(const InputInfoPtr pInfo, struct SynapticsHwState *hw)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    SynapticsParameters *para = &priv->synpara;
    Bool double_click = FALSE;

    if (!para->updown_button_scrolling) {
	if (hw->down) {		/* map down button to middle button */
	    hw->middle = TRUE;
	}

	if (hw->up) {		/* up button generates double click */
	    if (!priv->prev_up)
		double_click = TRUE;
	}
	priv->prev_up = hw->up;

	/* reset up/down button events */
	hw->up = hw->down = FALSE;
    }

    /* Left/right button scrolling, or middle clicks */
    if (!para->leftright_button_scrolling) {
	if (hw->multi[2] || hw->multi[3])
	    hw->middle = TRUE;

	/* reset left/right button events */
	hw->multi[2] = hw->multi[3] = FALSE;
    }

    return double_click;
}

static void
update_hw_button_state(const InputInfoPtr pInfo, struct SynapticsHwState *hw, int *delay)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    SynapticsParameters *para = &priv->synpara;

    /* Treat the first two multi buttons as up/down for now. */
    hw->up |= hw->multi[0];
    hw->down |= hw->multi[1];

    /* 3rd button emulation */
    hw->middle |= HandleMidButtonEmulation(priv, hw, delay);

    /* Fingers emulate other buttons */
    if(hw->left && hw->numFingers >= 1){
        handle_clickfinger(para, hw);
    }

    /* Two finger emulation */
    if (hw->numFingers == 1 && hw->z >= para->emulate_twofinger_z &&
        hw->fingerWidth >= para->emulate_twofinger_w) {
	hw->numFingers = 2;
    }
}

static void
post_button_click(const InputInfoPtr pInfo, const int button)
{
    xf86PostButtonEvent(pInfo->dev, FALSE, button, TRUE, 0, 0);
    xf86PostButtonEvent(pInfo->dev, FALSE, button, FALSE, 0, 0);
}


static void
post_scroll_events(const InputInfoPtr pInfo, struct ScrollData scroll)
{
    while (scroll.up-- > 0)
        post_button_click(pInfo, 4);

    while (scroll.down-- > 0)
        post_button_click(pInfo, 5);

    while (scroll.left-- > 0)
        post_button_click(pInfo, 6);

    while (scroll.right-- > 0)
        post_button_click(pInfo, 7);
}

static inline int
repeat_scrollbuttons(const InputInfoPtr pInfo,
                     const struct SynapticsHwState *hw,
		     int buttons, int delay)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    SynapticsParameters *para = &priv->synpara;
    int repeat_delay, timeleft;
    int rep_buttons = ((para->updown_button_repeat ? 0x18 : 0) |
			(para->leftright_button_repeat ? 0x60 : 0));

    /* Handle auto repeat buttons */
    repeat_delay = clamp(para->scroll_button_repeat, SBR_MIN, SBR_MAX);
    if (((hw->up || hw->down) && para->updown_button_repeat &&
	 para->updown_button_scrolling) ||
	((hw->multi[2] || hw->multi[3]) && para->leftright_button_repeat &&
	 para->leftright_button_scrolling)) {
	priv->repeatButtons = buttons & rep_buttons;
	if (!priv->nextRepeat) {
	    priv->nextRepeat = hw->millis + repeat_delay * 2;
	}
    } else {
	priv->repeatButtons = 0;
	priv->nextRepeat = 0;
    }

    if (priv->repeatButtons) {
	timeleft = TIME_DIFF(priv->nextRepeat, hw->millis);
	if (timeleft > 0)
	    delay = MIN(delay, timeleft);
	if (timeleft <= 0) {
	    int change, id;
	    change = priv->repeatButtons;
	    while (change) {
		id = ffs(change);
		change &= ~(1 << (id - 1));
		xf86PostButtonEvent(pInfo->dev, FALSE, id, FALSE, 0, 0);
		xf86PostButtonEvent(pInfo->dev, FALSE, id, TRUE, 0, 0);
	    }

	    priv->nextRepeat = hw->millis + repeat_delay;
	    delay = MIN(delay, repeat_delay);
	}
    }

    return delay;
}

/*
 * React on changes in the hardware state. This function is called every time
 * the hardware state changes. The return value is used to specify how many
 * milliseconds to wait before calling the function again if no state change
 * occurs.
 */
static int
HandleState(InputInfoPtr pInfo, struct SynapticsHwState *hw)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    SynapticsParameters *para = &priv->synpara;
    int finger;
    int dx, dy, buttons, id;
    edge_type edge = NO_EDGE;
    int change;
    struct ScrollData scroll;
    int double_click = FALSE;
    int delay = 1000000000;
    int timeleft;
    Bool inside_active_area;

    update_shm(pInfo, hw);

    /* If touchpad is switched off, we skip the whole thing and return delay */
    if (para->touchpad_off == 1)
	return delay;

    /* apply hysteresis before doing anything serious. This cancels
     * out a lot of noise which might surface in strange phenomena
     * like flicker in scrolling or noise motion. */
    priv->hyst_center_x = hysteresis(hw->x, priv->hyst_center_x, para->hyst_x);
    priv->hyst_center_y = hysteresis(hw->y, priv->hyst_center_y, para->hyst_y);
    hw->x = priv->hyst_center_x;
    hw->y = priv->hyst_center_y;

    inside_active_area = is_inside_active_area(priv, hw->x, hw->y);

    /* now we know that these _coordinates_ aren't in the area.
       invalid are: x, y, z, numFingers, fingerWidth
       valid are: millis, left/right/middle/up/down/etc.
    */
    if (!inside_active_area)
    {
	hw->x = 0;
	hw->y = 0;
	hw->z = 0;
	hw->numFingers = 0;
	hw->fingerWidth = 0;

	/* FIXME: if finger accidentally moves into the area and doesn't
	 * really release, the finger should remain down. */
	finger = FS_UNTOUCHED;
	edge = NO_EDGE;

	dx = dy = 0;
    }

    /* these two just update hw->left, right, etc. */
    update_hw_button_state(pInfo, hw, &delay);
    if (priv->has_scrollbuttons)
	double_click = adjust_state_from_scrollbuttons(pInfo, hw);

    /* no edge or finger detection outside of area */
    if (inside_active_area) {
	edge = edge_detection(priv, hw->x, hw->y);
	finger = SynapticsDetectFinger(priv, hw);
    }

    /* tap and drag detection. Needs to be performed even if the finger is in
     * the dead area to reset the state. */
    timeleft = HandleTapProcessing(priv, hw, finger, inside_active_area);
    if (timeleft > 0)
	delay = MIN(delay, timeleft);

    if (inside_active_area)
    {
	/* Don't bother about scrolling in the dead area of the touchpad. */
	timeleft = HandleScrolling(priv, hw, edge, finger, &scroll);
	if (timeleft > 0)
	    delay = MIN(delay, timeleft);

	/*
	 * Compensate for unequal x/y resolution. This needs to be done after
	 * calculations that require unadjusted coordinates, for example edge
	 * detection.
	 */
	ScaleCoordinates(priv, hw);
    }

    dx = dy = 0;

    if (!priv->absolute_events) {
      timeleft = ComputeDeltas(priv, hw, edge, &dx, &dy, inside_active_area);
      delay = MIN(delay, timeleft);
    }


    buttons = ((hw->left     ? 0x01 : 0) |
	       (hw->middle   ? 0x02 : 0) |
	       (hw->right    ? 0x04 : 0) |
	       (hw->up       ? 0x08 : 0) |
	       (hw->down     ? 0x10 : 0) |
	       (hw->multi[2] ? 0x20 : 0) |
	       (hw->multi[3] ? 0x40 : 0));

    if (priv->tap_button > 0) {
	int tap_mask = 1 << (priv->tap_button - 1);
	if (priv->tap_button_state == TBS_BUTTON_DOWN_UP) {
	    if (tap_mask != (priv->lastButtons & tap_mask)) {
		xf86PostButtonEvent(pInfo->dev, FALSE, priv->tap_button, TRUE, 0, 0);
		priv->lastButtons |= tap_mask;
	    }
	    priv->tap_button_state = TBS_BUTTON_UP;
	}
	if (priv->tap_button_state == TBS_BUTTON_DOWN)
	    buttons |= tap_mask;
    }

    /* Post events */
    if (finger > FS_UNTOUCHED) {
        if (priv->absolute_events && inside_active_area) {
            xf86PostMotionEvent(pInfo->dev, 1, 0, 2, hw->x, hw->y);
        } else if (dx || dy) {
            xf86PostMotionEvent(pInfo->dev, 0, 0, 2, dx, dy);
        }
    }

    if (priv->mid_emu_state == MBE_LEFT_CLICK)
    {
	post_button_click(pInfo, 1);
	priv->mid_emu_state = MBE_OFF;
    } else if (priv->mid_emu_state == MBE_RIGHT_CLICK)
    {
	post_button_click(pInfo, 3);
	priv->mid_emu_state = MBE_OFF;
    }

    change = buttons ^ priv->lastButtons;
    while (change) {
	id = ffs(change); /* number of first set bit 1..32 is returned */
	change &= ~(1 << (id - 1));
	xf86PostButtonEvent(pInfo->dev, FALSE, id, (buttons & (1 << (id - 1))), 0, 0);
    }

    /* Process scroll events only if coordinates are
     * in the Synaptics Area
     */
    if (inside_active_area)
	post_scroll_events(pInfo, scroll);

    if (double_click) {
	post_button_click(pInfo, 1);
	post_button_click(pInfo, 1);
    }

    if (priv->has_scrollbuttons)
	delay = repeat_scrollbuttons(pInfo, hw, buttons, delay);

    /* Save old values of some state variables */
    priv->finger_state = finger;
    priv->lastButtons = buttons;

    /* generate a history of the absolute positions */
    if (inside_active_area)
	store_history(priv, hw->x, hw->y, hw->millis);

    return delay;
}

static int
ControlProc(InputInfoPtr pInfo, xDeviceCtl * control)
{
    DBG(3, "Control Proc called\n");
    return Success;
}


static int
SwitchMode(ClientPtr client, DeviceIntPtr dev, int mode)
{
    InputInfoPtr pInfo = (InputInfoPtr) dev->public.devicePrivate;
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);

    DBG(3, "SwitchMode called\n");

    switch (mode) {
    case Absolute:
        priv->absolute_events = TRUE;
        break;

    case Relative:
        priv->absolute_events = FALSE;
        break;

    default:
        return XI_BadMode;
    }

    return Success;
}

static void
ReadDevDimensions(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;

    if (priv->proto_ops->ReadDevDimensions)
	priv->proto_ops->ReadDevDimensions(pInfo);
}

static Bool
QueryHardware(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;

    priv->comm.protoBufTail = 0;

    if (!priv->proto_ops->QueryHardware(pInfo)) {
	xf86Msg(X_PROBED, "%s: no supported touchpad found\n", pInfo->name);
	if (priv->proto_ops->DeviceOffHook)
            priv->proto_ops->DeviceOffHook(pInfo);
        return FALSE;
    }

    return TRUE;
}

static void
ScaleCoordinates(SynapticsPrivate *priv, struct SynapticsHwState *hw)
{
    int xCenter = (priv->synpara.left_edge + priv->synpara.right_edge) / 2;
    int yCenter = (priv->synpara.top_edge + priv->synpara.bottom_edge) / 2;

    hw->x = (hw->x - xCenter) * priv->horiz_coeff + xCenter;
    hw->y = (hw->y - yCenter) * priv->vert_coeff + yCenter;
}

void
CalculateScalingCoeffs(SynapticsPrivate *priv)
{
    int vertRes = priv->synpara.resolution_vert;
    int horizRes = priv->synpara.resolution_horiz;

    if ((horizRes > vertRes) && (horizRes > 0)) {
        priv->horiz_coeff = vertRes / (double)horizRes;
        priv->vert_coeff = 1;
    } else if ((horizRes < vertRes) && (vertRes > 0)) {
        priv->horiz_coeff = 1;
        priv->vert_coeff = horizRes / (double)vertRes;
    } else {
        priv->horiz_coeff = 1;
        priv->vert_coeff = 1;
    }
}
