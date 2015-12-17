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
 * Copyright © 2008-2012 Red Hat, Inc.
 * Copyright © 2011 The Chromium OS Authors
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
#include <math.h>
#include <stdio.h>
#include <xf86_OSproc.h>
#include <xf86Xinput.h>
#include <exevents.h>

#include <X11/Xatom.h>
#include <X11/extensions/XI2.h>
#include <xserver-properties.h>
#include <ptrveloc.h>

#include "synapticsstr.h"
#include "synaptics-properties.h"

enum EdgeType {
    NO_EDGE = 0,
    BOTTOM_EDGE = 1,
    TOP_EDGE = 2,
    LEFT_EDGE = 4,
    RIGHT_EDGE = 8,
    LEFT_BOTTOM_EDGE = BOTTOM_EDGE | LEFT_EDGE,
    RIGHT_BOTTOM_EDGE = BOTTOM_EDGE | RIGHT_EDGE,
    RIGHT_TOP_EDGE = TOP_EDGE | RIGHT_EDGE,
    LEFT_TOP_EDGE = TOP_EDGE | LEFT_EDGE
};

/*
 * We expect to be receiving a steady 80 packets/sec (which gives 40
 * reports/sec with more than one finger on the pad, as Advanced Gesture Mode
 * requires two PS/2 packets per report).  Instead of a random scattering of
 * magic 13 and 20ms numbers scattered throughout the driver, introduce
 * POLL_MS as 14ms, which is slightly less than 80Hz.  13ms is closer to
 * 80Hz, but if the kernel event reporting was even slightly delayed,
 * we would produce synthetic motion followed immediately by genuine
 * motion, so use 14.
 *
 * We use this to call back at a constant rate to at least produce the
 * illusion of smooth motion.  It works a lot better than you'd expect.
*/
#define POLL_MS 14

#define MAX(a, b) (((a)>(b))?(a):(b))
#define MIN(a, b) (((a)<(b))?(a):(b))
#define TIME_DIFF(a, b) ((int)((a)-(b)))

#define SQR(x) ((x) * (x))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define INPUT_BUFFER_SIZE 200

/*****************************************************************************
 * Forward declaration
 ****************************************************************************/
static int SynapticsPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags);
static void SynapticsUnInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags);
static Bool DeviceControl(DeviceIntPtr, int);
static void ReadInput(InputInfoPtr);
static int HandleState(InputInfoPtr, struct SynapticsHwState *, CARD32 now,
                       Bool from_timer);
static int ControlProc(InputInfoPtr, xDeviceCtl *);
static int SwitchMode(ClientPtr, DeviceIntPtr, int);
static int DeviceInit(DeviceIntPtr);
static int DeviceOn(DeviceIntPtr);
static int DeviceOff(DeviceIntPtr);
static int DeviceClose(DeviceIntPtr);
static Bool QueryHardware(InputInfoPtr);
static void ReadDevDimensions(InputInfoPtr);
#ifndef NO_DRIVER_SCALING
static void ScaleCoordinates(SynapticsPrivate * priv,
                             struct SynapticsHwState *hw);
static void CalculateScalingCoeffs(SynapticsPrivate * priv);
#endif
static void SanitizeDimensions(InputInfoPtr pInfo);

void InitDeviceProperties(InputInfoPtr pInfo);
int SetProperty(DeviceIntPtr dev, Atom property, XIPropertyValuePtr prop,
                BOOL checkonly);

const static struct {
    const char *name;
    struct SynapticsProtocolOperations *proto_ops;
} protocols[] = {
#ifdef BUILD_EVENTCOMM
    { "event", &event_proto_operations },
#endif
#ifdef BUILD_PSMCOMM
    { "psm", &psm_proto_operations },
#endif
#ifdef BUILD_PS2COMM
    { "psaux", &psaux_proto_operations },
    { "alps", &alps_proto_operations },
#endif
#ifdef BUILD_WSCONSCOMM
    { "wscons", &wscons_proto_operations },
#endif
    { NULL, NULL }
};

InputDriverRec SYNAPTICS = {
    1,
    "synaptics",
    NULL,
    SynapticsPreInit,
    SynapticsUnInit,
    NULL,
    NULL,
#ifdef XI86_DRV_CAP_SERVER_FD
    XI86_DRV_CAP_SERVER_FD
#endif
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
static inline void
SynapticsCloseFd(InputInfoPtr pInfo)
{
    if (pInfo->fd > -1 && !(pInfo->flags & XI86_SERVER_FD)) {
        xf86CloseSerial(pInfo->fd);
        pInfo->fd = -1;
    }
}

/**
 * Fill in default dimensions for backends that cannot query the hardware.
 * Eventually, we want the edges to be 1900/5400 for x, 1900/4000 for y.
 * These values are based so that calculate_edge_widths() will give us the
 * right values.
 *
 * The default values 1900, etc. come from the dawn of time, when men where
 * men, or possibly apes.
 */
static void
SanitizeDimensions(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;

    if (priv->minx >= priv->maxx) {
        priv->minx = 1615;
        priv->maxx = 5685;
        priv->resx = 0;

        xf86IDrvMsg(pInfo, X_PROBED,
                    "invalid x-axis range.  defaulting to %d - %d\n",
                    priv->minx, priv->maxx);
    }

    if (priv->miny >= priv->maxy) {
        priv->miny = 1729;
        priv->maxy = 4171;
        priv->resy = 0;

        xf86IDrvMsg(pInfo, X_PROBED,
                    "invalid y-axis range.  defaulting to %d - %d\n",
                    priv->miny, priv->maxy);
    }

    if (priv->minp >= priv->maxp) {
        priv->minp = 0;
        priv->maxp = 255;

        xf86IDrvMsg(pInfo, X_PROBED,
                    "invalid pressure range.  defaulting to %d - %d\n",
                    priv->minp, priv->maxp);
    }

    if (priv->minw >= priv->maxw) {
        priv->minw = 0;
        priv->maxw = 15;

        xf86IDrvMsg(pInfo, X_PROBED,
                    "invalid finger width range.  defaulting to %d - %d\n",
                    priv->minw, priv->maxw);
    }
}

static Bool
SetDeviceAndProtocol(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = pInfo->private;
    char *proto, *device;
    int i;

    proto = xf86SetStrOption(pInfo->options, "Protocol", NULL);
    device = xf86SetStrOption(pInfo->options, "Device", NULL);

    /* If proto is auto-dev, unset and let the code do the rest */
    if (proto && !strcmp(proto, "auto-dev")) {
        free(proto);
        proto = NULL;
    }

    for (i = 0; protocols[i].name; i++) {
        if ((!device || !proto) &&
            protocols[i].proto_ops->AutoDevProbe &&
            protocols[i].proto_ops->AutoDevProbe(pInfo, device))
            break;
        else if (proto && !strcmp(proto, protocols[i].name))
            break;
    }
    free(proto);
    free(device);

    priv->proto_ops = protocols[i].proto_ops;

    return (priv->proto_ops != NULL);
}

static void
calculate_edge_widths(SynapticsPrivate * priv, int *l, int *r, int *t, int *b)
{
    int width, height;
    int ewidth, eheight;        /* edge width/height */

    width = abs(priv->maxx - priv->minx);
    height = abs(priv->maxy - priv->miny);

    if (priv->model == MODEL_SYNAPTICS) {
        ewidth = width * .07;
        eheight = height * .07;
    }
    else if (priv->model == MODEL_ALPS) {
        ewidth = width * .15;
        eheight = height * .15;
    }
    else if (priv->model == MODEL_APPLETOUCH ||
             priv->model == MODEL_UNIBODY_MACBOOK) {
        ewidth = width * .085;
        eheight = height * .085;
    }
    else {
        ewidth = width * .04;
        eheight = height * .054;
    }

    *l = priv->minx + ewidth;
    *r = priv->maxx - ewidth;
    *t = priv->miny + eheight;
    *b = priv->maxy - eheight;
}

static void
calculate_tap_hysteresis(SynapticsPrivate * priv, int range,
                         int *fingerLow, int *fingerHigh)
{
    switch (priv->model) {
    case MODEL_ELANTECH:
        /* All Elantech touchpads don't need the Z filtering to get the
         * number of fingers correctly. See Documentation/elantech.txt
         * in the kernel.
         */
        *fingerLow = priv->minp + 1;
        *fingerHigh = priv->minp + 1;
        break;
    case MODEL_UNIBODY_MACBOOK:
        *fingerLow = 70;
        *fingerHigh = 75;
        break;
    default:
        *fingerLow = priv->minp + range * (25.0 / 256);
        *fingerHigh = priv->minp + range * (30.0 / 256);
        break;
    }
}

/* Area options support both percent values and absolute values. This is
 * awkward. The xf86Set* calls will print to the log, but they'll
 * also print an error if we request a percent value but only have an
 * int. So - check first for percent, then call xf86Set* again to get
 * the log message.
 */
static int
set_percent_option(pointer options, const char *optname,
                   const int range, const int offset, const int default_value)
{
    int result;
    double percent = xf86CheckPercentOption(options, optname, -1);

    if (percent >= 0.0) {
        percent = xf86SetPercentOption(options, optname, -1);
        result = percent / 100.0 * range + offset;
    } else
        result = xf86SetIntOption(options, optname, default_value);

    return result;
}

Bool
SynapticsIsSoftButtonAreasValid(int *values)
{
    Bool right_disabled = FALSE;
    Bool middle_disabled = FALSE;

    enum {
        /* right button left, right, top, bottom */
        RBL = 0,
        RBR = 1,
        RBT = 2,
        RBB = 3,
        /* middle button left, right, top, bottom */
        MBL = 4,
        MBR = 5,
        MBT = 6,
        MBB = 7,
    };

    /* Check right button area */
    if ((((values[RBL] != 0) && (values[RBR] != 0)) && (values[RBL] > values[RBR])) ||
        (((values[RBT] != 0) && (values[RBB] != 0)) && (values[RBT] > values[RBB])))
        return FALSE;

    /* Check middle button area */
    if ((((values[MBL] != 0) && (values[MBR] != 0)) && (values[MBL] > values[MBR])) ||
        (((values[MBT] != 0) && (values[MBB] != 0)) && (values[MBT] > values[MBB])))
        return FALSE;

    if (values[RBL] == 0 && values[RBR] == 0 && values[RBT] == 0 && values[RBB] == 0)
        right_disabled = TRUE;

    if (values[MBL] == 0 && values[MBR] == 0 && values[MBT] == 0 && values[MBB] == 0)
        middle_disabled = TRUE;

    if (!right_disabled &&
        ((values[RBL] && values[RBL] == values[RBR]) ||
         (values[RBT] && values[RBT] == values[RBB])))
        return FALSE;

    if (!middle_disabled &&
        ((values[MBL] && values[MBL] == values[MBR]) ||
         (values[MBT] && values[MBT] == values[MBB])))
        return FALSE;

    /* Check for overlapping button areas */
    if (!right_disabled && !middle_disabled) {
        int right_left = values[RBL] ? values[RBL] : INT_MIN;
        int right_right = values[RBR] ? values[RBR] : INT_MAX;
        int right_top = values[RBT] ? values[RBT] : INT_MIN;
        int right_bottom = values[RBB] ? values[RBB] : INT_MAX;
        int middle_left = values[MBL] ? values[MBL] : INT_MIN;
        int middle_right = values[MBR] ? values[MBR] : INT_MAX;
        int middle_top = values[MBT] ? values[MBT] : INT_MIN;
        int middle_bottom = values[MBB] ? values[MBB] : INT_MAX;

        /* If areas overlap in the Y axis */
        if ((right_bottom <= middle_bottom && right_bottom >= middle_top) ||
            (right_top <= middle_bottom && right_top >= middle_top)) {
            /* Check for overlapping left edges */
            if ((right_left < middle_left && right_right > middle_left) ||
                (middle_left < right_left && middle_right > right_left))
                return FALSE;

            /* Check for overlapping right edges */
            if ((right_right > middle_right && right_left < middle_right) ||
                (middle_right > right_right && middle_left < right_right))
                return FALSE;
        }

        /* If areas overlap in the X axis */
        if ((right_left >= middle_left && right_left <= middle_right) ||
            (right_right >= middle_left && right_right <= middle_right)) {
            /* Check for overlapping top edges */
            if ((right_top < middle_top && right_bottom > middle_top) ||
                (middle_top < right_top && middle_bottom > right_top))
                return FALSE;

            /* Check for overlapping bottom edges */
            if ((right_bottom > middle_bottom && right_top < middle_bottom) ||
                (middle_bottom > right_bottom && middle_top < right_bottom))
                return FALSE;
        }
    }

    return TRUE;
}

static void
set_softbutton_areas_option(InputInfoPtr pInfo, char *option_name, int offset)
{
    SynapticsPrivate *priv = pInfo->private;
    SynapticsParameters *pars = &priv->synpara;
    int values[8];
    int in_percent = 0;         /* bitmask for which ones are in % */
    char *option_string;
    char *next_num;
    char *end_str;
    int i;
    int width, height;

    if (!pars->clickpad)
        return;

    option_string = xf86SetStrOption(pInfo->options, option_name, NULL);
    if (!option_string)
        return;

    next_num = option_string;

    for (i = 0; i < 8 && *next_num != '\0'; i++) {
        long int value = strtol(next_num, &end_str, 0);

        if (value > INT_MAX || value < -INT_MAX)
            goto fail;

        values[i] = value;

        if (next_num != end_str) {
            if (end_str && *end_str == '%') {
                in_percent |= 1 << i;
                end_str++;
            }
            next_num = end_str;
        }
        else
            goto fail;
    }

    if (i < 8 || *next_num != '\0')
        goto fail;

    width = priv->maxx - priv->minx;
    height = priv->maxy - priv->miny;

    for (i = 0; in_percent && i < 8; i++) {
        int base, size;

        if ((in_percent & (1 << i)) == 0 || values[i] == 0)
            continue;

        size = ((i % 4) < 2) ? width : height;
        base = ((i % 4) < 2) ? priv->minx : priv->miny;
        values[i] = base + size * values[i] / 100.0;
    }

    if (!SynapticsIsSoftButtonAreasValid(values))
        goto fail;

    memcpy(pars->softbutton_areas[offset], values, 4 * sizeof(int));
    memcpy(pars->softbutton_areas[offset + 1], values + 4, 4 * sizeof(int));

    free(option_string);

    return;

 fail:
    xf86IDrvMsg(pInfo, X_ERROR,
                "invalid %s value '%s', keeping defaults\n",
                option_name, option_string);
    free(option_string);
}

static void
set_primary_softbutton_areas_option(InputInfoPtr pInfo)
{
    set_softbutton_areas_option(pInfo, "SoftButtonAreas", BOTTOM_BUTTON_AREA);
}

static void
set_secondary_softbutton_areas_option(InputInfoPtr pInfo)
{
    set_softbutton_areas_option(pInfo, "SecondarySoftButtonAreas", TOP_BUTTON_AREA);
}

static void
set_default_parameters(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = pInfo->private;    /* read-only */
    pointer opts = pInfo->options;      /* read-only */
    SynapticsParameters *pars = &priv->synpara; /* modified */

    int horizScrollDelta, vertScrollDelta;      /* pixels */
    int tapMove;                /* pixels */
    int l, r, t, b;             /* left, right, top, bottom */
    double accelFactor;         /* 1/pixels */
    int fingerLow, fingerHigh;  /* pressure */
    int emulateTwoFingerMinZ;   /* pressure */
    int emulateTwoFingerMinW;   /* width */
    int pressureMotionMinZ, pressureMotionMaxZ; /* pressure */
    int palmMinWidth, palmMinZ; /* pressure */
    int tapButton1, tapButton2, tapButton3;
    int clickFinger1, clickFinger2, clickFinger3;
    Bool vertEdgeScroll, horizEdgeScroll;
    Bool vertTwoFingerScroll, horizTwoFingerScroll;
    int horizResolution = 1;
    int vertResolution = 1;
    int width, height, diag, range;
    int horizHyst, vertHyst;
    int middle_button_timeout;
    int grab_event_device = 0;
    const char *source;

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

    width = abs(priv->maxx - priv->minx);
    height = abs(priv->maxy - priv->miny);
    diag = sqrt(width * width + height * height);

    calculate_edge_widths(priv, &l, &r, &t, &b);

    /* Again, based on typical x/y range and defaults */
    horizScrollDelta = diag * .020;
    vertScrollDelta = diag * .020;
    tapMove = diag * .044;
    accelFactor = 200.0 / diag; /* trial-and-error */

    /* hysteresis, assume >= 0 is a detected value (e.g. evdev fuzz) */
    horizHyst = pars->hyst_x >= 0 ? pars->hyst_x : diag * 0.005;
    vertHyst = pars->hyst_y >= 0 ? pars->hyst_y : diag * 0.005;

    range = priv->maxp - priv->minp + 1;

    calculate_tap_hysteresis(priv, range, &fingerLow, &fingerHigh);

    /* scaling based on defaults and a pressure of 256 */
    emulateTwoFingerMinZ = priv->minp + range * (282.0 / 256);
    pressureMotionMinZ = priv->minp + range * (30.0 / 256);
    pressureMotionMaxZ = priv->minp + range * (160.0 / 256);
    palmMinZ = priv->minp + range * (200.0 / 256);

    range = priv->maxw - priv->minw + 1;

    /* scaling based on defaults below and a tool width of 16 */
    palmMinWidth = priv->minw + range * (10.0 / 16);
    emulateTwoFingerMinW = priv->minw + range * (7.0 / 16);

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

    pars->area_top_edge =
        set_percent_option(opts, "AreaTopEdge", height, priv->miny, 0);
    pars->area_bottom_edge =
        set_percent_option(opts, "AreaBottomEdge", height, priv->miny, 0);
    pars->area_left_edge =
        set_percent_option(opts, "AreaLeftEdge", width, priv->minx, 0);
    pars->area_right_edge =
        set_percent_option(opts, "AreaRightEdge", width, priv->minx, 0);

    pars->hyst_x =
        set_percent_option(opts, "HorizHysteresis", width, 0, horizHyst);
    pars->hyst_y =
        set_percent_option(opts, "VertHysteresis", height, 0, vertHyst);

    pars->finger_low = xf86SetIntOption(opts, "FingerLow", fingerLow);
    pars->finger_high = xf86SetIntOption(opts, "FingerHigh", fingerHigh);
    pars->tap_time = xf86SetIntOption(opts, "MaxTapTime", 180);
    pars->tap_move = xf86SetIntOption(opts, "MaxTapMove", tapMove);
    pars->tap_time_2 = xf86SetIntOption(opts, "MaxDoubleTapTime", 100);
    pars->click_time = xf86SetIntOption(opts, "ClickTime", 100);
    pars->clickpad = xf86SetBoolOption(opts, "ClickPad", pars->clickpad);       /* Probed */
    if (pars->clickpad)
        pars->has_secondary_buttons = xf86SetBoolOption(opts,
                                                        "HasSecondarySoftButtons",
                                                        pars->has_secondary_buttons);
    pars->clickpad_ignore_motion_time = 100; /* ms */
    /* middle mouse button emulation on a clickpad? nah, you're joking */
    middle_button_timeout = pars->clickpad ? 0 : 75;
    pars->emulate_mid_button_time =
        xf86SetIntOption(opts, "EmulateMidButtonTime", middle_button_timeout);
    pars->emulate_twofinger_z =
        xf86SetIntOption(opts, "EmulateTwoFingerMinZ", emulateTwoFingerMinZ);
    pars->emulate_twofinger_w =
        xf86SetIntOption(opts, "EmulateTwoFingerMinW", emulateTwoFingerMinW);
    pars->scroll_dist_vert =
        xf86SetIntOption(opts, "VertScrollDelta", vertScrollDelta);
    pars->scroll_dist_horiz =
        xf86SetIntOption(opts, "HorizScrollDelta", horizScrollDelta);
    pars->scroll_edge_vert =
        xf86SetBoolOption(opts, "VertEdgeScroll", vertEdgeScroll);
    pars->scroll_edge_horiz =
        xf86SetBoolOption(opts, "HorizEdgeScroll", horizEdgeScroll);
    pars->scroll_edge_corner = xf86SetBoolOption(opts, "CornerCoasting", FALSE);
    pars->scroll_twofinger_vert =
        xf86SetBoolOption(opts, "VertTwoFingerScroll", vertTwoFingerScroll);
    pars->scroll_twofinger_horiz =
        xf86SetBoolOption(opts, "HorizTwoFingerScroll", horizTwoFingerScroll);
    pars->touchpad_off = xf86SetIntOption(opts, "TouchpadOff", TOUCHPAD_ON);

    if (priv->has_scrollbuttons) {
        pars->updown_button_scrolling =
            xf86SetBoolOption(opts, "UpDownScrolling", TRUE);
        pars->leftright_button_scrolling =
            xf86SetBoolOption(opts, "LeftRightScrolling", TRUE);
        pars->updown_button_repeat =
            xf86SetBoolOption(opts, "UpDownScrollRepeat", TRUE);
        pars->leftright_button_repeat =
            xf86SetBoolOption(opts, "LeftRightScrollRepeat", TRUE);
    }
    pars->scroll_button_repeat =
        xf86SetIntOption(opts, "ScrollButtonRepeat", 100);

    pars->locked_drags = xf86SetBoolOption(opts, "LockedDrags", FALSE);
    pars->locked_drag_time = xf86SetIntOption(opts, "LockedDragTimeout", 5000);
    pars->tap_action[RT_TAP] = xf86SetIntOption(opts, "RTCornerButton", 0);
    pars->tap_action[RB_TAP] = xf86SetIntOption(opts, "RBCornerButton", 0);
    pars->tap_action[LT_TAP] = xf86SetIntOption(opts, "LTCornerButton", 0);
    pars->tap_action[LB_TAP] = xf86SetIntOption(opts, "LBCornerButton", 0);
    pars->tap_action[F1_TAP] = xf86SetIntOption(opts, "TapButton1", tapButton1);
    pars->tap_action[F2_TAP] = xf86SetIntOption(opts, "TapButton2", tapButton2);
    pars->tap_action[F3_TAP] = xf86SetIntOption(opts, "TapButton3", tapButton3);
    pars->click_action[F1_CLICK1] =
        xf86SetIntOption(opts, "ClickFinger1", clickFinger1);
    pars->click_action[F2_CLICK1] =
        xf86SetIntOption(opts, "ClickFinger2", clickFinger2);
    pars->click_action[F3_CLICK1] =
        xf86SetIntOption(opts, "ClickFinger3", clickFinger3);
    pars->circular_scrolling =
        xf86SetBoolOption(opts, "CircularScrolling", FALSE);
    pars->circular_trigger = xf86SetIntOption(opts, "CircScrollTrigger", 0);
    pars->circular_pad = xf86SetBoolOption(opts, "CircularPad", FALSE);
    pars->palm_detect = xf86SetBoolOption(opts, "PalmDetect", FALSE);
    pars->palm_min_width = xf86SetIntOption(opts, "PalmMinWidth", palmMinWidth);
    pars->palm_min_z = xf86SetIntOption(opts, "PalmMinZ", palmMinZ);
    pars->single_tap_timeout = xf86SetIntOption(opts, "SingleTapTimeout", 180);
    pars->press_motion_min_z =
        xf86SetIntOption(opts, "PressureMotionMinZ", pressureMotionMinZ);
    pars->press_motion_max_z =
        xf86SetIntOption(opts, "PressureMotionMaxZ", pressureMotionMaxZ);

    pars->min_speed = xf86SetRealOption(opts, "MinSpeed", 0.4);
    pars->max_speed = xf86SetRealOption(opts, "MaxSpeed", 0.7);
    pars->accl = xf86SetRealOption(opts, "AccelFactor", accelFactor);
    pars->scroll_dist_circ = xf86SetRealOption(opts, "CircScrollDelta", 0.1);
    pars->coasting_speed = xf86SetRealOption(opts, "CoastingSpeed", 20.0);
    pars->coasting_friction = xf86SetRealOption(opts, "CoastingFriction", 50);
    pars->press_motion_min_factor =
        xf86SetRealOption(opts, "PressureMotionMinFactor", 1.0);
    pars->press_motion_max_factor =
        xf86SetRealOption(opts, "PressureMotionMaxFactor", 1.0);

    /* Only grab the device by default if it's not coming from a config
       backend. This way we avoid the device being added twice and sending
       duplicate events.
      */
    source = xf86CheckStrOption(opts, "_source", NULL);
    if (source == NULL || strncmp(source, "server/", 7) != 0)
        grab_event_device = TRUE;
    pars->grab_event_device = xf86SetBoolOption(opts, "GrabEventDevice", grab_event_device);

    pars->tap_and_drag_gesture =
        xf86SetBoolOption(opts, "TapAndDragGesture", TRUE);
    pars->resolution_horiz =
        xf86SetIntOption(opts, "HorizResolution", horizResolution);
    pars->resolution_vert =
        xf86SetIntOption(opts, "VertResolution", vertResolution);
    if (pars->resolution_horiz <= 0) {
        xf86IDrvMsg(pInfo, X_ERROR,
                    "Invalid X resolution, using 1 instead.\n");
        pars->resolution_horiz = 1;
    }
    if (pars->resolution_vert <= 0) {
        xf86IDrvMsg(pInfo, X_ERROR,
                    "Invalid Y resolution, using 1 instead.\n");
        pars->resolution_vert = 1;
    }

    /* Warn about (and fix) incorrectly configured TopEdge/BottomEdge parameters */
    if (pars->top_edge > pars->bottom_edge) {
        int tmp = pars->top_edge;

        pars->top_edge = pars->bottom_edge;
        pars->bottom_edge = tmp;
        xf86IDrvMsg(pInfo, X_WARNING,
                    "TopEdge is bigger than BottomEdge. Fixing.\n");
    }

    set_primary_softbutton_areas_option(pInfo);
    if (pars->has_secondary_buttons)
        set_secondary_softbutton_areas_option(pInfo);
}

static double
SynapticsAccelerationProfile(DeviceIntPtr dev,
                             DeviceVelocityPtr vel,
                             double velocity, double thr, double acc)
{
    InputInfoPtr pInfo = dev->public.devicePrivate;
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    SynapticsParameters *para = &priv->synpara;

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

        if (priv->hwState->z <= minZ) {
            accelfct *= minFctr;
        }
        else if (priv->hwState->z >= maxZ) {
            accelfct *= maxFctr;
        }
        else {
            accelfct *=
                minFctr + (priv->hwState->z - minZ) * (maxFctr -
                                                       minFctr) / (maxZ - minZ);
        }
    }

    return accelfct;
}

static int
SynapticsPreInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
{
    SynapticsPrivate *priv;

    /* allocate memory for SynapticsPrivateRec */
    priv = calloc(1, sizeof(SynapticsPrivate));
    if (!priv)
        return BadAlloc;

    pInfo->type_name = XI_TOUCHPAD;
    pInfo->device_control = DeviceControl;
    pInfo->read_input = ReadInput;
    pInfo->control_proc = ControlProc;
    pInfo->switch_mode = SwitchMode;
    pInfo->private = priv;

    /* allocate now so we don't allocate in the signal handler */
    priv->timer = TimerSet(NULL, 0, 0, NULL, NULL);
    if (!priv->timer) {
        free(priv);
        return BadAlloc;
    }

    /* may change pInfo->options */
    if (!SetDeviceAndProtocol(pInfo)) {
        xf86IDrvMsg(pInfo, X_ERROR,
                    "Synaptics driver unable to detect protocol\n");
        goto SetupProc_fail;
    }

    priv->device = xf86FindOptionValue(pInfo->options, "Device");

    /* open the touchpad device */
    pInfo->fd = xf86OpenSerial(pInfo->options);
    if (pInfo->fd == -1) {
        xf86IDrvMsg(pInfo, X_ERROR, "Synaptics driver unable to open device\n");
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

    set_default_parameters(pInfo);

#ifndef NO_DRIVER_SCALING
    CalculateScalingCoeffs(priv);
#endif


    priv->comm.buffer = XisbNew(pInfo->fd, INPUT_BUFFER_SIZE);

    if (!QueryHardware(pInfo)) {
        xf86IDrvMsg(pInfo, X_ERROR,
                    "Unable to query/initialize Synaptics hardware.\n");
        goto SetupProc_fail;
    }

    xf86ProcessCommonOptions(pInfo, pInfo->options);

    if (priv->comm.buffer) {
        XisbFree(priv->comm.buffer);
        priv->comm.buffer = NULL;
    }
    SynapticsCloseFd(pInfo);

    return Success;

 SetupProc_fail:
    SynapticsCloseFd(pInfo);

    if (priv->comm.buffer)
        XisbFree(priv->comm.buffer);
    free(priv->proto_data);
    free(priv->timer);
    free(priv);
    pInfo->private = NULL;
    return BadAlloc;
}

/*
 *  Uninitialize the device.
 */
static void
SynapticsUnInit(InputDriverPtr drv, InputInfoPtr pInfo, int flags)
{
    SynapticsPrivate *priv = ((SynapticsPrivate *) pInfo->private);

    if (priv && priv->timer)
        free(priv->timer);
    if (priv && priv->proto_data)
        free(priv->proto_data);
    if (priv && priv->scroll_events_mask)
        valuator_mask_free(&priv->scroll_events_mask);
    if (priv && priv->open_slots)
        free(priv->open_slots);
    free(pInfo->private);
    pInfo->private = NULL;
    xf86DeleteInput(pInfo, 0);
}

/*
 *  Alter the control parameters for the mouse. Note that all special
 *  protocol values are handled by dix.
 */
static void
SynapticsCtrl(DeviceIntPtr device, PtrCtrl * ctrl)
{
}

static int
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

static int
DeviceOn(DeviceIntPtr dev)
{
    InputInfoPtr pInfo = dev->public.devicePrivate;
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);

    DBG(3, "Synaptics DeviceOn called\n");

    pInfo->fd = xf86OpenSerial(pInfo->options);
    if (pInfo->fd == -1) {
        xf86IDrvMsg(pInfo, X_WARNING, "cannot open input device\n");
        return !Success;
    }

    if (priv->proto_ops->DeviceOnHook &&
        !priv->proto_ops->DeviceOnHook(pInfo, &priv->synpara))
         goto error;

    priv->comm.buffer = XisbNew(pInfo->fd, INPUT_BUFFER_SIZE);
    if (!priv->comm.buffer)
        goto error;

    xf86FlushInput(pInfo->fd);

    /* reinit the pad */
    if (!QueryHardware(pInfo))
        goto error;

    xf86AddEnabledDevice(pInfo);
    dev->public.on = TRUE;

    return Success;

error:
    if (priv->comm.buffer) {
        XisbFree(priv->comm.buffer);
        priv->comm.buffer = NULL;
    }
    SynapticsCloseFd(pInfo);
    return !Success;
}

static void
SynapticsReset(SynapticsPrivate * priv)
{
    int i;

    SynapticsResetHwState(priv->hwState);
    SynapticsResetHwState(priv->local_hw_state);
    SynapticsResetHwState(priv->comm.hwState);

    memset(priv->move_hist, 0, sizeof(priv->move_hist));
    priv->hyst_center_x = 0;
    priv->hyst_center_y = 0;
    memset(&priv->scroll, 0, sizeof(priv->scroll));
    priv->count_packet_finger = 0;
    priv->finger_state = FS_UNTOUCHED;
    priv->last_motion_millis = 0;
    priv->clickpad_click_millis = 0;
    priv->last_button_area = NO_BUTTON_AREA;
    priv->tap_state = TS_START;
    priv->tap_button = 0;
    priv->tap_button_state = TBS_BUTTON_UP;
    priv->moving_state = MS_FALSE;
    priv->vert_scroll_edge_on = FALSE;
    priv->horiz_scroll_edge_on = FALSE;
    priv->vert_scroll_twofinger_on = FALSE;
    priv->horiz_scroll_twofinger_on = FALSE;
    priv->circ_scroll_on = FALSE;
    priv->circ_scroll_vert = FALSE;
    priv->mid_emu_state = MBE_OFF;
    priv->nextRepeat = 0;
    priv->lastButtons = 0;
    priv->prev_z = 0;
    priv->prevFingers = 0;
    priv->num_active_touches = 0;

    for (i = 0; i < priv->num_slots; i++)
        priv->open_slots[i] = -1;
}

static int
DeviceOff(DeviceIntPtr dev)
{
    InputInfoPtr pInfo = dev->public.devicePrivate;
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    Bool rc = Success;

    DBG(3, "Synaptics DeviceOff called\n");

    if (pInfo->fd != -1) {
        TimerCancel(priv->timer);
        xf86RemoveEnabledDevice(pInfo);
        SynapticsReset(priv);

        if (priv->proto_ops->DeviceOffHook &&
            !priv->proto_ops->DeviceOffHook(pInfo))
            rc = !Success;
        if (priv->comm.buffer) {
            XisbFree(priv->comm.buffer);
            priv->comm.buffer = NULL;
        }
        SynapticsCloseFd(pInfo);
    }
    dev->public.on = FALSE;
    return rc;
}

static int
DeviceClose(DeviceIntPtr dev)
{
    Bool RetValue;
    InputInfoPtr pInfo = dev->public.devicePrivate;
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;

    RetValue = DeviceOff(dev);
    TimerFree(priv->timer);
    priv->timer = NULL;
    free(priv->touch_axes);
    priv->touch_axes = NULL;
    SynapticsHwStateFree(&priv->hwState);
    SynapticsHwStateFree(&priv->local_hw_state);
    SynapticsHwStateFree(&priv->comm.hwState);
    return RetValue;
}

static void
InitAxesLabels(Atom *labels, int nlabels, const SynapticsPrivate * priv)
{
    int i;

    memset(labels, 0, nlabels * sizeof(Atom));
    switch (nlabels) {
    default:
    case 4:
        labels[3] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_VSCROLL);
    case 3:
        labels[2] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_HSCROLL);
    case 2:
        labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_Y);
    case 1:
        labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_X);
        break;
    }

    for (i = 0; i < priv->num_mt_axes; i++) {
        SynapticsTouchAxisRec *axis = &priv->touch_axes[i];
        int axnum = nlabels - priv->num_mt_axes + i;

        labels[axnum] = XIGetKnownProperty(axis->label);
    }
}

static void
InitButtonLabels(Atom *labels, int nlabels)
{
    memset(labels, 0, nlabels * sizeof(Atom));
    switch (nlabels) {
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

static void
DeviceInitTouch(DeviceIntPtr dev, Atom *axes_labels)
{
    InputInfoPtr pInfo = dev->public.devicePrivate;
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    int i;

    if (priv->has_touch) {
        priv->num_slots =
            priv->max_touches ? priv->max_touches : SYNAPTICS_MAX_TOUCHES;

        priv->open_slots = malloc(priv->num_slots * sizeof(int));
        if (!priv->open_slots) {
            xf86IDrvMsg(pInfo, X_ERROR,
                        "failed to allocate open touch slots array\n");
            priv->has_touch = 0;
            priv->num_slots = 0;
            return;
        }

        /* x/y + whatever other MT axes we found */
        if (!InitTouchClassDeviceStruct(dev, priv->max_touches,
                                        XIDependentTouch,
                                        2 + priv->num_mt_axes)) {
            xf86IDrvMsg(pInfo, X_ERROR,
                        "failed to initialize touch class device\n");
            priv->has_touch = 0;
            priv->num_slots = 0;
            free(priv->open_slots);
            priv->open_slots = NULL;
            return;
        }

        for (i = 0; i < priv->num_mt_axes; i++) {
            SynapticsTouchAxisRec *axis = &priv->touch_axes[i];
            int axnum = 4 + i;  /* Skip x, y, and scroll axes */

            if (!xf86InitValuatorAxisStruct(dev, axnum, axes_labels[axnum],
                                            axis->min, axis->max, axis->res, 0,
                                            axis->res, Absolute)) {
                xf86IDrvMsg(pInfo, X_WARNING,
                            "failed to initialize axis %s, skipping\n",
                            axis->label);
                continue;
            }

            xf86InitValuatorDefaults(dev, axnum);
        }
    }
}

static int
DeviceInit(DeviceIntPtr dev)
{
    InputInfoPtr pInfo = dev->public.devicePrivate;
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    Atom float_type, prop;
    float tmpf;
    unsigned char map[SYN_MAX_BUTTONS + 1];
    int i;
    int min, max;
    int num_axes = 2;
    Atom btn_labels[SYN_MAX_BUTTONS] = { 0 };
    Atom *axes_labels;
    DeviceVelocityPtr pVel;

    num_axes += 2;

    num_axes += priv->num_mt_axes;

    axes_labels = calloc(num_axes, sizeof(Atom));
    if (!axes_labels) {
        xf86IDrvMsg(pInfo, X_ERROR, "failed to allocate axis labels\n");
        return !Success;
    }

    InitAxesLabels(axes_labels, num_axes, priv);
    InitButtonLabels(btn_labels, SYN_MAX_BUTTONS);

    DBG(3, "Synaptics DeviceInit called\n");

    for (i = 0; i <= SYN_MAX_BUTTONS; i++)
        map[i] = i;

    dev->public.on = FALSE;

    InitPointerDeviceStruct((DevicePtr) dev, map,
                            SYN_MAX_BUTTONS,
                            btn_labels,
                            SynapticsCtrl,
                            GetMotionHistorySize(), num_axes, axes_labels);

    /*
     * setup dix acceleration to match legacy synaptics settings, and
     * etablish a device-specific profile to do stuff like pressure-related
     * acceleration.
     */
    if (NULL != (pVel = GetDevicePredictableAccelData(dev))) {
        SetDeviceSpecificAccelerationProfile(pVel,
                                             SynapticsAccelerationProfile);

        /* float property type */
        float_type = XIGetKnownProperty(XATOM_FLOAT);

        /* translate MinAcc to constant deceleration.
         * May be overridden in xf86InitValuatorDefaults */
        tmpf = 1.0 / priv->synpara.min_speed;

        xf86IDrvMsg(pInfo, X_CONFIG,
                    "(accel) MinSpeed is now constant deceleration " "%.1f\n",
                    tmpf);
        prop = XIGetKnownProperty(ACCEL_PROP_CONSTANT_DECELERATION);
        XIChangeDeviceProperty(dev, prop, float_type, 32,
                               PropModeReplace, 1, &tmpf, FALSE);

        /* adjust accordingly */
        priv->synpara.max_speed /= priv->synpara.min_speed;
        priv->synpara.min_speed = 1.0;

        /* synaptics seems to report 80 packet/s, but dix scales for
         * 100 packet/s by default. */
        pVel->corr_mul = 12.5f; /*1000[ms]/80[/s] = 12.5 */

        xf86IDrvMsg(pInfo, X_CONFIG, "(accel) MaxSpeed is now %.2f\n",
                    priv->synpara.max_speed);
        xf86IDrvMsg(pInfo, X_CONFIG, "(accel) AccelFactor is now %.3f\n",
                    priv->synpara.accl);

        prop = XIGetKnownProperty(ACCEL_PROP_PROFILE_NUMBER);
        i = AccelProfileDeviceSpecific;
        XIChangeDeviceProperty(dev, prop, XA_INTEGER, 32,
                               PropModeReplace, 1, &i, FALSE);
    }

    /* X valuator */
    if (priv->minx < priv->maxx) {
        min = priv->minx;
        max = priv->maxx;
    }
    else {
        min = 0;
        max = -1;
    }

    xf86InitValuatorAxisStruct(dev, 0, axes_labels[0], min, max,
			       priv->resx * 1000, 0, priv->resx * 1000,
			       Relative);
    xf86InitValuatorDefaults(dev, 0);

    /* Y valuator */
    if (priv->miny < priv->maxy) {
        min = priv->miny;
        max = priv->maxy;
    }
    else {
        min = 0;
        max = -1;
    }

    xf86InitValuatorAxisStruct(dev, 1, axes_labels[1], min, max,
			       priv->resy * 1000, 0, priv->resy * 1000,
			       Relative);
    xf86InitValuatorDefaults(dev, 1);

    xf86InitValuatorAxisStruct(dev, 2, axes_labels[2], 0, -1, 0, 0, 0,
                               Relative);
    priv->scroll_axis_horiz = 2;
    xf86InitValuatorAxisStruct(dev, 3, axes_labels[3], 0, -1, 0, 0, 0,
                               Relative);
    priv->scroll_axis_vert = 3;
    priv->scroll_events_mask = valuator_mask_new(MAX_VALUATORS);
    if (!priv->scroll_events_mask) {
        free(axes_labels);
        return !Success;
    }

    SetScrollValuator(dev, priv->scroll_axis_horiz, SCROLL_TYPE_HORIZONTAL,
                      priv->synpara.scroll_dist_horiz, 0);
    SetScrollValuator(dev, priv->scroll_axis_vert, SCROLL_TYPE_VERTICAL,
                      priv->synpara.scroll_dist_vert, 0);

    DeviceInitTouch(dev, axes_labels);

    free(axes_labels);

    priv->hwState = SynapticsHwStateAlloc(priv);
    if (!priv->hwState)
        goto fail;

    priv->local_hw_state = SynapticsHwStateAlloc(priv);
    if (!priv->local_hw_state)
        goto fail;

    priv->comm.hwState = SynapticsHwStateAlloc(priv);

    InitDeviceProperties(pInfo);
    XIRegisterPropertyHandler(pInfo->dev, SetProperty, NULL, NULL);

    SynapticsReset(priv);

    return Success;

 fail:
    free(priv->local_hw_state);
    free(priv->hwState);
    free(priv->open_slots);
    return !Success;
}

/*
 * Convert from absolute X/Y coordinates to a coordinate system where
 * -1 corresponds to the left/upper edge and +1 corresponds to the
 * right/lower edge.
 */
static void
relative_coords(SynapticsPrivate * priv, int x, int y,
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
    }
    else {
        *relX = 0;
        *relY = 0;
    }
}

/* return angle of point relative to center */
static double
angle(SynapticsPrivate * priv, int x, int y)
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

static enum EdgeType
circular_edge_detection(SynapticsPrivate * priv, int x, int y)
{
    enum EdgeType edge = 0;
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

static enum EdgeType
edge_detection(SynapticsPrivate * priv, int x, int y)
{
    enum EdgeType edge = NO_EDGE;

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
is_inside_active_area(SynapticsPrivate * priv, int x, int y)
{
    Bool inside_area = TRUE;

    /* If a finger is down, then it must have started inside the active_area,
       allow the motion to complete using the entire area */
    if (priv->finger_state >= FS_TOUCHED)
        return TRUE;

    if ((priv->synpara.area_left_edge != 0) &&
        (x < priv->synpara.area_left_edge))
        inside_area = FALSE;
    else if ((priv->synpara.area_right_edge != 0) &&
             (x > priv->synpara.area_right_edge))
        inside_area = FALSE;

    if ((priv->synpara.area_top_edge != 0) && (y < priv->synpara.area_top_edge))
        inside_area = FALSE;
    else if ((priv->synpara.area_bottom_edge != 0) &&
             (y > priv->synpara.area_bottom_edge))
        inside_area = FALSE;

    return inside_area;
}

static Bool
is_inside_button_area(SynapticsParameters * para, int which, int x, int y)
{
    Bool inside_area = TRUE;

    if (para->softbutton_areas[which][LEFT] == 0 &&
        para->softbutton_areas[which][RIGHT] == 0 &&
        para->softbutton_areas[which][TOP] == 0 &&
        para->softbutton_areas[which][BOTTOM] == 0)
        return FALSE;

    if (para->softbutton_areas[which][LEFT] &&
        x < para->softbutton_areas[which][LEFT])
        inside_area = FALSE;
    else if (para->softbutton_areas[which][RIGHT] &&
             x > para->softbutton_areas[which][RIGHT])
        inside_area = FALSE;
    else if (para->softbutton_areas[which][TOP] &&
             y < para->softbutton_areas[which][TOP])
        inside_area = FALSE;
    else if (para->softbutton_areas[which][BOTTOM] &&
             y > para->softbutton_areas[which][BOTTOM])
        inside_area = FALSE;

    return inside_area;
}

static Bool
is_inside_rightbutton_area(SynapticsParameters * para, int x, int y)
{
    return is_inside_button_area(para, BOTTOM_RIGHT_BUTTON_AREA, x, y);
}

static Bool
is_inside_middlebutton_area(SynapticsParameters * para, int x, int y)
{
    return is_inside_button_area(para, BOTTOM_MIDDLE_BUTTON_AREA, x, y);
}

static Bool
is_inside_sec_rightbutton_area(SynapticsParameters * para, int x, int y)
{
    return is_inside_button_area(para, TOP_RIGHT_BUTTON_AREA, x, y);
}

static Bool
is_inside_sec_middlebutton_area(SynapticsParameters * para, int x, int y)
{
    return is_inside_button_area(para, TOP_MIDDLE_BUTTON_AREA, x, y);
}

static Bool
is_inside_top_or_bottom_button_area(SynapticsParameters * para, int offset,
                                    int x, int y)
{
    Bool inside_area = TRUE;
    Bool right_valid, middle_valid;
    int top, bottom;

    /* We don't have a left button area, so we only check the y axis */
    right_valid = para->softbutton_areas[offset][TOP] ||
                  para->softbutton_areas[offset][BOTTOM];
    middle_valid = para->softbutton_areas[offset + 1][TOP] ||
                   para->softbutton_areas[offset + 1][BOTTOM];

    if (!right_valid && !middle_valid)
        return FALSE;

    /* Check both buttons are horizontally aligned */
    if (right_valid && middle_valid && (
            para->softbutton_areas[offset][TOP] !=
                para->softbutton_areas[offset + 1][TOP] ||
            para->softbutton_areas[offset][BOTTOM] !=
                para->softbutton_areas[offset + 1][BOTTOM]))
        return FALSE;

    if (right_valid) {
        top    = para->softbutton_areas[offset][TOP];
        bottom = para->softbutton_areas[offset][BOTTOM];
    }
    else {
        top    = para->softbutton_areas[offset + 1][TOP];
        bottom = para->softbutton_areas[offset + 1][BOTTOM];
    }

    if (top && y < top)
        inside_area = FALSE;
    else if (bottom && y > bottom)
        inside_area = FALSE;

    return inside_area;
}

static enum SoftButtonAreas
current_button_area(SynapticsParameters * para, int x, int y)
{
    if (is_inside_top_or_bottom_button_area(para, BOTTOM_BUTTON_AREA, x, y))
        return BOTTOM_BUTTON_AREA;
    else if (is_inside_top_or_bottom_button_area(para, TOP_BUTTON_AREA, x, y))
        return TOP_BUTTON_AREA;
    else
        return NO_BUTTON_AREA;
}

static CARD32
timerFunc(OsTimerPtr timer, CARD32 now, pointer arg)
{
    InputInfoPtr pInfo = arg;
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    struct SynapticsHwState *hw = priv->local_hw_state;
    int delay;
    int sigstate;

    sigstate = xf86BlockSIGIO();

    priv->hwState->millis += now - priv->timer_time;
    SynapticsCopyHwState(hw, priv->hwState);
    SynapticsResetTouchHwState(hw, FALSE);
    delay = HandleState(pInfo, hw, hw->millis, TRUE);

    priv->timer_time = now;
    priv->timer = TimerSet(priv->timer, 0, delay, timerFunc, pInfo);

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
SynapticsGetHwState(InputInfoPtr pInfo, SynapticsPrivate * priv,
                    struct SynapticsHwState *hw)
{
    return priv->proto_ops->ReadHwState(pInfo, &priv->comm, hw);
}

/*
 *  called for each full received packet from the touchpad
 */
static void
ReadInput(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    struct SynapticsHwState *hw = priv->local_hw_state;
    int delay = 0;
    Bool newDelay = FALSE;

    SynapticsResetTouchHwState(hw, FALSE);

    while (SynapticsGetHwState(pInfo, priv, hw)) {
        /* Semi-mt device touch slots do not track touches. When there is a
         * change in the number of touches, we must disregard the temporary
         * motion changes. */
        if (priv->has_semi_mt && hw->numFingers != priv->hwState->numFingers) {
            hw->cumulative_dx = priv->hwState->cumulative_dx;
            hw->cumulative_dy = priv->hwState->cumulative_dy;
        }

        /* timer may cause actual events to lag behind (#48777) */
        if (priv->hwState->millis > hw->millis)
            hw->millis = priv->hwState->millis;

        SynapticsCopyHwState(priv->hwState, hw);
        delay = HandleState(pInfo, hw, hw->millis, FALSE);
        newDelay = TRUE;
    }

    if (newDelay) {
        priv->timer_time = GetTimeInMillis();
        priv->timer = TimerSet(priv->timer, 0, delay, timerFunc, pInfo);
    }
}

static int
HandleMidButtonEmulation(SynapticsPrivate * priv, struct SynapticsHwState *hw,
                         CARD32 now, int *delay)
{
    SynapticsParameters *para = &priv->synpara;
    Bool done = FALSE;
    int timeleft;
    int mid = 0;

    if (para->emulate_mid_button_time <= 0)
        return mid;

    while (!done) {
        switch (priv->mid_emu_state) {
        case MBE_LEFT_CLICK:
        case MBE_RIGHT_CLICK:
        case MBE_OFF:
            priv->button_delay_millis = now;
            if (hw->left) {
                priv->mid_emu_state = MBE_LEFT;
            }
            else if (hw->right) {
                priv->mid_emu_state = MBE_RIGHT;
            }
            else {
                done = TRUE;
            }
            break;
        case MBE_LEFT:
            timeleft =
                TIME_DIFF(priv->button_delay_millis +
                          para->emulate_mid_button_time, now);
            if (timeleft > 0)
                *delay = MIN(*delay, timeleft);

            /* timeout, but within the same ReadInput cycle! */
            if ((timeleft <= 0) && !hw->left) {
                priv->mid_emu_state = MBE_LEFT_CLICK;
                done = TRUE;
            }
            else if ((!hw->left) || (timeleft <= 0)) {
                hw->left = TRUE;
                priv->mid_emu_state = MBE_TIMEOUT;
                done = TRUE;
            }
            else if (hw->right) {
                priv->mid_emu_state = MBE_MID;
            }
            else {
                hw->left = FALSE;
                done = TRUE;
            }
            break;
        case MBE_RIGHT:
            timeleft =
                TIME_DIFF(priv->button_delay_millis +
                          para->emulate_mid_button_time, now);
            if (timeleft > 0)
                *delay = MIN(*delay, timeleft);

            /* timeout, but within the same ReadInput cycle! */
            if ((timeleft <= 0) && !hw->right) {
                priv->mid_emu_state = MBE_RIGHT_CLICK;
                done = TRUE;
            }
            else if (!hw->right || (timeleft <= 0)) {
                hw->right = TRUE;
                priv->mid_emu_state = MBE_TIMEOUT;
                done = TRUE;
            }
            else if (hw->left) {
                priv->mid_emu_state = MBE_MID;
            }
            else {
                hw->right = FALSE;
                done = TRUE;
            }
            break;
        case MBE_MID:
            if (!hw->left && !hw->right) {
                priv->mid_emu_state = MBE_OFF;
            }
            else {
                mid = TRUE;
                hw->left = hw->right = FALSE;
                done = TRUE;
            }
            break;
        case MBE_TIMEOUT:
            if (!hw->left && !hw->right) {
                priv->mid_emu_state = MBE_OFF;
            }
            else {
                done = TRUE;
            }
        }
    }
    return mid;
}

static enum FingerState
SynapticsDetectFinger(SynapticsPrivate * priv, struct SynapticsHwState *hw)
{
    SynapticsParameters *para = &priv->synpara;
    enum FingerState finger;

    /* finger detection thru pressure and threshold */
    if (hw->z < para->finger_low)
        return FS_UNTOUCHED;

    if (priv->finger_state == FS_BLOCKED)
        return FS_BLOCKED;

    if (hw->z > para->finger_high && priv->finger_state == FS_UNTOUCHED)
        finger = FS_TOUCHED;
    else
        finger = priv->finger_state;

    if (!para->palm_detect)
        return finger;

    /* palm detection */

    if ((hw->z > para->palm_min_z) && (hw->fingerWidth > para->palm_min_width))
        return FS_BLOCKED;

    if (hw->x == 0 || priv->finger_state == FS_UNTOUCHED)
        priv->avg_width = 0;
    else
        priv->avg_width += (hw->fingerWidth - priv->avg_width + 1) / 2;

    if (finger != FS_UNTOUCHED && priv->finger_state == FS_UNTOUCHED) {
        int safe_width = MAX(hw->fingerWidth, priv->avg_width);

        if (hw->numFingers > 1 ||       /* more than one finger -> not a palm */
            ((safe_width < 6) && (priv->prev_z < para->finger_high)) || /* thin finger, distinct touch -> not a palm */
            ((safe_width < 7) && (priv->prev_z < para->finger_high / 2))) {     /* thin finger, distinct touch -> not a palm */
            /* leave finger value as is */
        }
        else if (hw->z > priv->prev_z + 1)      /* z not stable, may be a palm */
            finger = FS_UNTOUCHED;
        else if (hw->z < priv->prev_z - 5)      /* z not stable, may be a palm */
            finger = FS_UNTOUCHED;
        else if (hw->fingerWidth > para->palm_min_width)        /* finger width too large -> probably palm */
            finger = FS_UNTOUCHED;
    }
    priv->prev_z = hw->z;

    return finger;
}

static void
SelectTapButton(SynapticsPrivate * priv, enum EdgeType edge)
{
    enum TapEvent tap;

    if (priv->synpara.touchpad_off == TOUCHPAD_TAP_OFF) {
        priv->tap_button = 0;
        return;
    }

    switch (priv->tap_max_fingers) {
    case 1:
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
    default:
        priv->tap_button = 0;
        return;
    }

    priv->tap_button = priv->synpara.tap_action[tap];
    priv->tap_button = clamp(priv->tap_button, 0, SYN_MAX_BUTTONS);
}

static void
SetTapState(SynapticsPrivate * priv, enum TapState tap_state, CARD32 millis)
{
    DBG(3, "SetTapState - %d -> %d (millis:%u)\n", priv->tap_state, tap_state,
        millis);
    switch (tap_state) {
    case TS_START:
        priv->tap_button_state = TBS_BUTTON_UP;
        priv->tap_max_fingers = 0;
        break;
    case TS_1:
        priv->tap_button_state = TBS_BUTTON_UP;
        break;
    case TS_2A:
	priv->tap_button_state = TBS_BUTTON_UP;
        break;
    case TS_2B:
        priv->tap_button_state = TBS_BUTTON_UP;
        break;
    case TS_3:
        priv->tap_button_state = TBS_BUTTON_DOWN;
        break;
    case TS_SINGLETAP:
	priv->tap_button_state = TBS_BUTTON_DOWN;
        priv->touch_on.millis = millis;
        break;
    default:
        break;
    }
    priv->tap_state = tap_state;
}

static void
SetMovingState(SynapticsPrivate * priv, enum MovingState moving_state,
               CARD32 millis)
{
    DBG(7, "SetMovingState - %d -> %d center at %d/%d (millis:%u)\n",
        priv->moving_state, moving_state, priv->hwState->x, priv->hwState->y,
        millis);

    priv->moving_state = moving_state;
}

static int
GetTimeOut(SynapticsPrivate * priv)
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
        return -1;              /* No timeout */
    }
}

static int
HandleTapProcessing(SynapticsPrivate * priv, struct SynapticsHwState *hw,
                    CARD32 now, enum FingerState finger,
                    Bool inside_active_area)
{
    SynapticsParameters *para = &priv->synpara;
    Bool touch, release, is_timeout, move, press;
    int timeleft, timeout;
    enum EdgeType edge;
    int delay = 1000000000;

    if (para->touchpad_off == TOUCHPAD_OFF ||
        priv->finger_state == FS_BLOCKED)
        return delay;

    touch = finger >= FS_TOUCHED && priv->finger_state == FS_UNTOUCHED;
    release = finger == FS_UNTOUCHED && priv->finger_state >= FS_TOUCHED;
    move = (finger >= FS_TOUCHED &&
            (priv->tap_max_fingers <=
             ((priv->horiz_scroll_twofinger_on ||
               priv->vert_scroll_twofinger_on) ? 2 : 1)) &&
            (priv->prevFingers == hw->numFingers &&
             ((abs(hw->x - priv->touch_on.x) >= para->tap_move) ||
              (abs(hw->y - priv->touch_on.y) >= para->tap_move))));
    press = (hw->left || hw->right || hw->middle);

    if (touch) {
        priv->touch_on.x = hw->x;
        priv->touch_on.y = hw->y;
        priv->touch_on.millis = now;
    }
    else if (release) {
        priv->touch_on.millis = now;
    }
    if (hw->z > para->finger_high)
        if (priv->tap_max_fingers < hw->numFingers)
            priv->tap_max_fingers = hw->numFingers;
    timeout = GetTimeOut(priv);
    timeleft = TIME_DIFF(priv->touch_on.millis + timeout, now);
    is_timeout = timeleft <= 0;

 restart:
    switch (priv->tap_state) {
    case TS_START:
        if (touch)
            SetTapState(priv, TS_1, now);
        break;
    case TS_1:
        if (para->clickpad && press) {
            SetTapState(priv, TS_CLICKPAD_MOVE, now);
            goto restart;
        }
        if (move) {
            SetMovingState(priv, MS_TOUCHPAD_RELATIVE, now);
            SetTapState(priv, TS_MOVE, now);
            goto restart;
        }
        else if (is_timeout) {
            if (finger == FS_TOUCHED) {
                SetMovingState(priv, MS_TOUCHPAD_RELATIVE, now);
            }
            SetTapState(priv, TS_MOVE, now);
            goto restart;
        }
        else if (release) {
            edge = edge_detection(priv, priv->touch_on.x, priv->touch_on.y);
            SelectTapButton(priv, edge);
            /* Disable taps outside of the active area */
            if (!inside_active_area) {
                priv->tap_button = 0;
            }
            SetTapState(priv, TS_2A, now);
        }
        break;
    case TS_MOVE:
        if (para->clickpad && press) {
            SetTapState(priv, TS_CLICKPAD_MOVE, now);
            goto restart;
        }
        if (release) {
            SetMovingState(priv, MS_FALSE, now);
            SetTapState(priv, TS_START, now);
        }
        break;
    case TS_2A:
        if (touch)
            SetTapState(priv, TS_3, now);
        else if (is_timeout)
            SetTapState(priv, TS_SINGLETAP, now);
        break;
    case TS_2B:
        if (touch)
            SetTapState(priv, TS_3, now);
        else if (is_timeout)
            SetTapState(priv, TS_SINGLETAP, now);
        break;
    case TS_SINGLETAP:
        if (touch)
            SetTapState(priv, TS_1, now);
        else if (is_timeout)
            SetTapState(priv, TS_START, now);
        break;
    case TS_3:
        if (move) {
            if (para->tap_and_drag_gesture) {
                SetMovingState(priv, MS_TOUCHPAD_RELATIVE, now);
                SetTapState(priv, TS_DRAG, now);
            }
            else {
                SetTapState(priv, TS_1, now);
            }
            goto restart;
        }
        else if (is_timeout) {
            if (para->tap_and_drag_gesture) {
                if (finger == FS_TOUCHED) {
                    SetMovingState(priv, MS_TOUCHPAD_RELATIVE, now);
                }
                SetTapState(priv, TS_DRAG, now);
            }
            else {
                SetTapState(priv, TS_1, now);
            }
            goto restart;
        }
        else if (release) {
            SetTapState(priv, TS_2B, now);
        }
        break;
    case TS_DRAG:
        if (para->clickpad && press) {
            SetTapState(priv, TS_CLICKPAD_MOVE, now);
            goto restart;
        }
        if (move)
            SetMovingState(priv, MS_TOUCHPAD_RELATIVE, now);
        if (release) {
            SetMovingState(priv, MS_FALSE, now);
            if (para->locked_drags) {
                SetTapState(priv, TS_4, now);
            }
            else {
                SetTapState(priv, TS_START, now);
            }
        }
        break;
    case TS_4:
        if (is_timeout) {
            SetTapState(priv, TS_START, now);
            goto restart;
        }
        if (touch)
            SetTapState(priv, TS_5, now);
        break;
    case TS_5:
        if (is_timeout || move) {
            SetTapState(priv, TS_DRAG, now);
            goto restart;
        }
        else if (release) {
            SetMovingState(priv, MS_FALSE, now);
            SetTapState(priv, TS_START, now);
        }
        break;
    case TS_CLICKPAD_MOVE:
        /* Disable scrolling once a button is pressed on a clickpad */
        priv->vert_scroll_edge_on = FALSE;
        priv->horiz_scroll_edge_on = FALSE;
        priv->vert_scroll_twofinger_on = FALSE;
        priv->horiz_scroll_twofinger_on = FALSE;

        /* Assume one touch is only for holding the clickpad button down */
        if (hw->numFingers > 1)
            hw->numFingers--;
        SetMovingState(priv, MS_TOUCHPAD_RELATIVE, now);
        if (!press) {
            SetMovingState(priv, MS_FALSE, now);
            SetTapState(priv, TS_MOVE, now);
            priv->count_packet_finger = 0;
        }
        break;
    }

    timeout = GetTimeOut(priv);
    if (timeout >= 0) {
        timeleft = TIME_DIFF(priv->touch_on.millis + timeout, now);
        delay = clamp(timeleft, 1, delay);
    }
    return delay;
}

#define HIST(a) (priv->move_hist[((priv->hist_index - (a) + SYNAPTICS_MOVE_HISTORY) % SYNAPTICS_MOVE_HISTORY)])
#define HIST_DELTA(a, b, e) ((HIST((a)).e) - (HIST((b)).e))

static void
store_history(SynapticsPrivate * priv, int x, int y, CARD32 millis)
{
    int idx = (priv->hist_index + 1) % SYNAPTICS_MOVE_HISTORY;

    priv->move_hist[idx].x = x;
    priv->move_hist[idx].y = y;
    priv->move_hist[idx].millis = millis;
    priv->hist_index = idx;
    if (priv->count_packet_finger < SYNAPTICS_MOVE_HISTORY)
        priv->count_packet_finger++;
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
static int
hysteresis(int in, int center, int margin)
{
    int diff = in - center;

    if (abs(diff) <= margin) {
        diff = 0;
    }
    else if (diff > margin) {
        diff -= margin;
    }
    else if (diff < -margin) {
        diff += margin;
    }
    return center + diff;
}

static void
get_delta(SynapticsPrivate *priv, const struct SynapticsHwState *hw,
          enum EdgeType edge, double *dx, double *dy)
{
    double integral;
    double tmpf;

    *dx = hw->x - HIST(0).x;
    *dy = hw->y - HIST(0).y;

    /* report edge speed as synthetic motion. Of course, it would be
     * cooler to report floats than to buffer, but anyway. */

    /* FIXME: When these values go NaN, bad things happen. Root cause is unknown
     * thus far though. */
    if (isnan(priv->frac_x))
        priv->frac_x = 0;
    if (isnan(priv->frac_y))
        priv->frac_y = 0;

    tmpf = *dx + priv->frac_x;
    priv->frac_x = modf(tmpf, &integral);
    *dx = integral;
    tmpf = *dy + priv->frac_y;
    priv->frac_y = modf(tmpf, &integral);
    *dy = integral;
}

/**
 * Compute relative motion ('deltas') including edge motion.
 */
static int
ComputeDeltas(SynapticsPrivate * priv, const struct SynapticsHwState *hw,
              enum EdgeType edge, int *dxP, int *dyP, Bool inside_area)
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
            moving_state = MS_TOUCHPAD_RELATIVE;
            break;
        default:
            break;
        }
    }

    if (!inside_area || !moving_state || priv->finger_state == FS_BLOCKED ||
        priv->vert_scroll_edge_on || priv->horiz_scroll_edge_on ||
        priv->vert_scroll_twofinger_on || priv->horiz_scroll_twofinger_on ||
        priv->circ_scroll_on || priv->prevFingers != hw->numFingers ||
        (moving_state == MS_TOUCHPAD_RELATIVE && hw->numFingers != 1)) {
        /* reset packet counter. */
        priv->count_packet_finger = 0;
        goto out;
    }

    /* To create the illusion of fluid motion, call back at roughly the report
     * rate, even in the absence of new hardware events; see comment above
     * POLL_MS declaration. */
    delay = MIN(delay, POLL_MS);

    if (priv->count_packet_finger <= 1)
        goto out;               /* skip the lot */

    if (moving_state == MS_TOUCHPAD_RELATIVE)
        get_delta(priv, hw, edge, &dx, &dy);

 out:
    priv->prevFingers = hw->numFingers;

    *dxP = dx;
    *dyP = dy;

    return delay;
}

static double
estimate_delta_circ(SynapticsPrivate * priv)
{
    double a1 = angle(priv, HIST(3).x, HIST(3).y);
    double a2 = angle(priv, HIST(2).x, HIST(2).y);
    double a3 = angle(priv, HIST(1).x, HIST(1).y);
    double a4 = angle(priv, HIST(0).x, HIST(0).y);
    double d1 = diffa(a2, a1);
    double d2 = d1 + diffa(a3, a2);
    double d3 = d2 + diffa(a4, a3);

    return estimate_delta(d3, d2, d1, 0);
}

/* vert and horiz are to know which direction to start coasting
 * circ is true if the user had been circular scrolling.
 */
static void
start_coasting(SynapticsPrivate * priv, struct SynapticsHwState *hw,
               Bool vert, Bool horiz, Bool circ)
{
    SynapticsParameters *para = &priv->synpara;

    priv->scroll.coast_delta_y = 0.0;
    priv->scroll.coast_delta_x = 0.0;

    if ((priv->scroll.packets_this_scroll > 3) && (para->coasting_speed > 0.0)) {
        double pkt_time = HIST_DELTA(0, 3, millis) / 1000.0;

        if (vert && !circ) {
            double dy =
                estimate_delta(HIST(0).y, HIST(1).y, HIST(2).y, HIST(3).y);
            if (pkt_time > 0) {
                double scrolls_per_sec = (dy / abs(para->scroll_dist_vert)) / pkt_time;

                if (fabs(scrolls_per_sec) >= para->coasting_speed) {
                    priv->scroll.coast_speed_y = scrolls_per_sec;
                    priv->scroll.coast_delta_y = (hw->y - priv->scroll.last_y);
                }
            }
        }
        if (horiz && !circ) {
            double dx =
                estimate_delta(HIST(0).x, HIST(1).x, HIST(2).x, HIST(3).x);
            if (pkt_time > 0) {
                double scrolls_per_sec = (dx / abs(para->scroll_dist_vert)) / pkt_time;

                if (fabs(scrolls_per_sec) >= para->coasting_speed) {
                    priv->scroll.coast_speed_x = scrolls_per_sec;
                    priv->scroll.coast_delta_x = (hw->x - priv->scroll.last_x);
                }
            }
        }
        if (circ) {
            double da = estimate_delta_circ(priv);

            if (pkt_time > 0) {
                double scrolls_per_sec = (da / para->scroll_dist_circ) / pkt_time;

                if (fabs(scrolls_per_sec) >= para->coasting_speed) {
                    if (vert) {
                        priv->scroll.coast_speed_y = scrolls_per_sec;
                        priv->scroll.coast_delta_y =
                            diffa(priv->scroll.last_a,
                                  angle(priv, hw->x, hw->y));
                    }
                    else if (horiz) {
                        priv->scroll.coast_speed_x = scrolls_per_sec;
                        priv->scroll.coast_delta_x =
                            diffa(priv->scroll.last_a,
                                  angle(priv, hw->x, hw->y));
                    }
                }
            }
        }
    }
    priv->scroll.packets_this_scroll = 0;
}

static void
stop_coasting(SynapticsPrivate * priv)
{
    priv->scroll.coast_speed_x = 0;
    priv->scroll.coast_speed_y = 0;
    priv->scroll.packets_this_scroll = 0;
}

static int
HandleScrolling(SynapticsPrivate * priv, struct SynapticsHwState *hw,
                enum EdgeType edge, Bool finger)
{
    SynapticsParameters *para = &priv->synpara;
    int delay = 1000000000;

    if (priv->synpara.touchpad_off == TOUCHPAD_TAP_OFF ||
        priv->synpara.touchpad_off == TOUCHPAD_OFF ||
        priv->finger_state == FS_BLOCKED) {
        stop_coasting(priv);
        priv->circ_scroll_on = FALSE;
        priv->vert_scroll_edge_on = FALSE;
        priv->horiz_scroll_edge_on = FALSE;
        priv->vert_scroll_twofinger_on = FALSE;
        priv->horiz_scroll_twofinger_on = FALSE;
        return delay;
    }

    /* scroll detection */
    if (finger && priv->finger_state == FS_UNTOUCHED) {
        stop_coasting(priv);
        priv->scroll.delta_y = 0;
        priv->scroll.delta_x = 0;
        if (para->circular_scrolling) {
            if ((para->circular_trigger == 0 && edge) ||
                (para->circular_trigger == 1 && edge & TOP_EDGE) ||
                (para->circular_trigger == 2 && edge & TOP_EDGE &&
                 edge & RIGHT_EDGE) || (para->circular_trigger == 3 &&
                                        edge & RIGHT_EDGE) ||
                (para->circular_trigger == 4 && edge & RIGHT_EDGE &&
                 edge & BOTTOM_EDGE) || (para->circular_trigger == 5 &&
                                         edge & BOTTOM_EDGE) ||
                (para->circular_trigger == 6 && edge & BOTTOM_EDGE &&
                 edge & LEFT_EDGE) || (para->circular_trigger == 7 &&
                                       edge & LEFT_EDGE) ||
                (para->circular_trigger == 8 && edge & LEFT_EDGE &&
                 edge & TOP_EDGE)) {
                priv->circ_scroll_on = TRUE;
                priv->circ_scroll_vert = TRUE;
                priv->scroll.last_a = angle(priv, hw->x, hw->y);
                DBG(7, "circular scroll detected on edge\n");
            }
        }
    }
    if (!priv->circ_scroll_on) {
        if (finger) {
            if (hw->numFingers == 2) {
                if (!priv->vert_scroll_twofinger_on &&
                    (para->scroll_twofinger_vert) &&
                    (para->scroll_dist_vert != 0)) {
                    stop_coasting(priv);
                    priv->vert_scroll_twofinger_on = TRUE;
                    priv->vert_scroll_edge_on = FALSE;
                    priv->scroll.last_y = hw->y;
                    DBG(7, "vert two-finger scroll detected\n");
                }
                if (!priv->horiz_scroll_twofinger_on &&
                    (para->scroll_twofinger_horiz) &&
                    (para->scroll_dist_horiz != 0)) {
                    stop_coasting(priv);
                    priv->horiz_scroll_twofinger_on = TRUE;
                    priv->horiz_scroll_edge_on = FALSE;
                    priv->scroll.last_x = hw->x;
                    DBG(7, "horiz two-finger scroll detected\n");
                }
            }
        }
        if (finger && priv->finger_state == FS_UNTOUCHED) {
            if (!priv->vert_scroll_twofinger_on &&
                !priv->horiz_scroll_twofinger_on) {
                if ((para->scroll_edge_vert) && (para->scroll_dist_vert != 0) &&
                    (edge & RIGHT_EDGE)) {
                    priv->vert_scroll_edge_on = TRUE;
                    priv->scroll.last_y = hw->y;
                    DBG(7, "vert edge scroll detected on right edge\n");
                }
                if ((para->scroll_edge_horiz) && (para->scroll_dist_horiz != 0)
                    && (edge & BOTTOM_EDGE)) {
                    priv->horiz_scroll_edge_on = TRUE;
                    priv->scroll.last_x = hw->x;
                    DBG(7, "horiz edge scroll detected on bottom edge\n");
                }
            }
        }
    }
    {
        Bool oldv = priv->vert_scroll_twofinger_on || priv->vert_scroll_edge_on
            || (priv->circ_scroll_on && priv->circ_scroll_vert);

        Bool oldh = priv->horiz_scroll_twofinger_on ||
            priv->horiz_scroll_edge_on || (priv->circ_scroll_on &&
                                           !priv->circ_scroll_vert);

        Bool oldc = priv->circ_scroll_on;

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
        if (para->scroll_edge_corner &&
            (priv->scroll.coast_speed_x || priv->scroll.coast_speed_y)) {
            Bool is_in_corner = ((edge & RIGHT_EDGE) &&
                                 (edge & (TOP_EDGE | BOTTOM_EDGE))) ||
                ((edge & BOTTOM_EDGE) && (edge & (LEFT_EDGE | RIGHT_EDGE)));
            if (!is_in_corner || !finger) {
                DBG(7, "corner edge scroll off\n");
                stop_coasting(priv);
            }
        }
        /* if we were scrolling, but couldn't corner edge scroll,
         * and are no longer scrolling, then start coasting */
        oldv = oldv && !(priv->vert_scroll_twofinger_on ||
                         priv->vert_scroll_edge_on || (priv->circ_scroll_on &&
                                                       priv->circ_scroll_vert));

        oldh = oldh && !(priv->horiz_scroll_twofinger_on ||
                         priv->horiz_scroll_edge_on || (priv->circ_scroll_on &&
                                                        !priv->
                                                        circ_scroll_vert));

        oldc = oldc && !priv->circ_scroll_on;

        if ((oldv || oldh) && !para->scroll_edge_corner) {
            start_coasting(priv, hw, oldv, oldh, oldc);
        }
    }

    /* if hitting a corner (top right or bottom right) while vertical
     * scrolling is active, consider starting corner edge scrolling or
     * switching over to circular scrolling smoothly */
    if (priv->vert_scroll_edge_on && !priv->horiz_scroll_edge_on &&
        (edge & RIGHT_EDGE) && (edge & (TOP_EDGE | BOTTOM_EDGE))) {
        if (para->scroll_edge_corner) {
            if (priv->scroll.coast_speed_y == 0) {
                /* FYI: We can generate multiple start_coasting requests if
                 * we're in the corner, but we were moving so slowly when we
                 * got here that we didn't actually start coasting. */
                DBG(7, "corner edge scroll on\n");
                start_coasting(priv, hw, TRUE, FALSE, FALSE);
            }
        }
        else if (para->circular_scrolling) {
            priv->vert_scroll_edge_on = FALSE;
            priv->circ_scroll_on = TRUE;
            priv->circ_scroll_vert = TRUE;
            priv->scroll.last_a = angle(priv, hw->x, hw->y);
            DBG(7, "switching to circular scrolling\n");
        }
    }
    /* Same treatment for horizontal scrolling */
    if (priv->horiz_scroll_edge_on && !priv->vert_scroll_edge_on &&
        (edge & BOTTOM_EDGE) && (edge & (LEFT_EDGE | RIGHT_EDGE))) {
        if (para->scroll_edge_corner) {
            if (priv->scroll.coast_speed_x == 0) {
                /* FYI: We can generate multiple start_coasting requests if
                 * we're in the corner, but we were moving so slowly when we
                 * got here that we didn't actually start coasting. */
                DBG(7, "corner edge scroll on\n");
                start_coasting(priv, hw, FALSE, TRUE, FALSE);
            }
        }
        else if (para->circular_scrolling) {
            priv->horiz_scroll_edge_on = FALSE;
            priv->circ_scroll_on = TRUE;
            priv->circ_scroll_vert = FALSE;
            priv->scroll.last_a = angle(priv, hw->x, hw->y);
            DBG(7, "switching to circular scrolling\n");
        }
    }

    if (priv->vert_scroll_edge_on || priv->horiz_scroll_edge_on ||
        priv->vert_scroll_twofinger_on || priv->horiz_scroll_twofinger_on ||
        priv->circ_scroll_on) {
        priv->scroll.packets_this_scroll++;
    }

    if (priv->vert_scroll_edge_on || priv->vert_scroll_twofinger_on) {
        /* + = down, - = up */
        if (para->scroll_dist_vert != 0 && hw->y != priv->scroll.last_y) {
            priv->scroll.delta_y += (hw->y - priv->scroll.last_y);
            priv->scroll.last_y = hw->y;
        }
    }
    if (priv->horiz_scroll_edge_on || priv->horiz_scroll_twofinger_on) {
        /* + = right, - = left */
        if (para->scroll_dist_horiz != 0 && hw->x != priv->scroll.last_x) {
            priv->scroll.delta_x += (hw->x - priv->scroll.last_x);
            priv->scroll.last_x = hw->x;
        }
    }
    if (priv->circ_scroll_on) {
        /* + = counter clockwise, - = clockwise */
        double delta = para->scroll_dist_circ;
        double diff = diffa(priv->scroll.last_a, angle(priv, hw->x, hw->y));

        if (delta >= 0.005 && diff != 0.0) {
            if (priv->circ_scroll_vert)
                priv->scroll.delta_y -= diff / delta * para->scroll_dist_vert;
            else
                priv->scroll.delta_x -= diff / delta * para->scroll_dist_horiz;
            priv->scroll.last_a = angle(priv, hw->x, hw->y);
        }
    }

    if (priv->scroll.coast_speed_y) {
        double dtime = (hw->millis - priv->scroll.last_millis) / 1000.0;
        double ddy = para->coasting_friction * dtime;

        priv->scroll.delta_y += priv->scroll.coast_speed_y * dtime * abs(para->scroll_dist_vert);
        delay = MIN(delay, POLL_MS);
        if (abs(priv->scroll.coast_speed_y) < ddy) {
            priv->scroll.coast_speed_y = 0;
            priv->scroll.packets_this_scroll = 0;
        }
        else {
            priv->scroll.coast_speed_y +=
                (priv->scroll.coast_speed_y < 0 ? ddy : -ddy);
        }
    }

    if (priv->scroll.coast_speed_x) {
        double dtime = (hw->millis - priv->scroll.last_millis) / 1000.0;
        double ddx = para->coasting_friction * dtime;
        priv->scroll.delta_x += priv->scroll.coast_speed_x * dtime * abs(para->scroll_dist_horiz);
        delay = MIN(delay, POLL_MS);
        if (abs(priv->scroll.coast_speed_x) < ddx) {
            priv->scroll.coast_speed_x = 0;
            priv->scroll.packets_this_scroll = 0;
        }
        else {
            priv->scroll.coast_speed_x +=
                (priv->scroll.coast_speed_x < 0 ? ddx : -ddx);
        }
    }

    return delay;
}

/**
 * Check if any 2+ fingers are close enough together to assume this is a
 * ClickFinger action.
 */
static int
clickpad_guess_clickfingers(SynapticsPrivate * priv,
                            struct SynapticsHwState *hw)
{
    int nfingers = 0;
    uint32_t close_point = 0; /* 1 bit for each point close to another one */
    int i, j;

    BUG_RETURN_VAL(hw->num_mt_mask > sizeof(close_point) * 8, 0);

    for (i = 0; i < hw->num_mt_mask - 1; i++) {
        ValuatorMask *f1;

        if (hw->slot_state[i] == SLOTSTATE_EMPTY ||
            hw->slot_state[i] == SLOTSTATE_CLOSE)
            continue;

        f1 = hw->mt_mask[i];

        for (j = i + 1; j < hw->num_mt_mask; j++) {
            ValuatorMask *f2;
            double x1, x2, y1, y2;

            if (hw->slot_state[j] == SLOTSTATE_EMPTY ||
                hw->slot_state[j] == SLOTSTATE_CLOSE)
                continue;

            f2 = hw->mt_mask[j];

            x1 = valuator_mask_get_double(f1, 0);
            y1 = valuator_mask_get_double(f1, 1);

            x2 = valuator_mask_get_double(f2, 0);
            y2 = valuator_mask_get_double(f2, 1);

            /* FIXME: fingers closer together than 30% of touchpad width, but
             * really, this should be dependent on the touchpad size. Also,
             * you'll need to find a touchpad that doesn't lie about it's
             * size. Good luck. */
            if (abs(x1 - x2) < (priv->maxx - priv->minx) * .3 &&
                abs(y1 - y2) < (priv->maxy - priv->miny) * .3) {
                close_point |= (1 << j);
                close_point |= (1 << i);
            }
        }
    }

    while (close_point > 0) {
        nfingers += close_point & 0x1;
        close_point >>= 1;
    }

    /* Some trackpads touchpad only track two touchpoints but announce
     * BTN_TOOL_TRIPLETAP (which sets hw->numFingers to 3), when this happens
     * the user likely intents to do a 3 finger click, so handle it as such.
     */
    if (hw->numFingers >= 3 && hw->num_mt_mask < 3)
        nfingers = 3;

    return nfingers;
}

static void
handle_clickfinger(SynapticsPrivate * priv, struct SynapticsHwState *hw)
{
    SynapticsParameters *para = &priv->synpara;
    int action = 0;
    int nfingers = hw->numFingers;

    /* if this is a clickpad, clickfinger handling is:
     * one finger down: no action, this is a normal click
     * two fingers down: F2_CLICK
     * three fingers down: F3_CLICK
     */

    if (para->clickpad)
        nfingers = clickpad_guess_clickfingers(priv, hw);

    switch (nfingers) {
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
    switch (action) {
    case 1:
        hw->left = 1 | BTN_EMULATED_FLAG;
        break;
    case 2:
        hw->left = 0;
        hw->middle = 1 | BTN_EMULATED_FLAG;
        break;
    case 3:
        hw->left = 0;
        hw->right = 1 | BTN_EMULATED_FLAG;
        break;
    }
}

/* Adjust the hardware state according to the extra buttons (if the touchpad
 * has any and not many touchpads do these days). These buttons are up/down
 * tilt buttons and/or left/right buttons that then map into a specific
 * function (or scrolling into).
 */
static Bool
adjust_state_from_scrollbuttons(const InputInfoPtr pInfo,
                                struct SynapticsHwState *hw)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    SynapticsParameters *para = &priv->synpara;
    Bool double_click = FALSE;

    if (!para->updown_button_scrolling) {
        if (hw->down) {         /* map down button to middle button */
            hw->middle = TRUE;
        }

        if (hw->up) {           /* up button generates double click */
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
update_hw_button_state(const InputInfoPtr pInfo, struct SynapticsHwState *hw,
                       CARD32 now, int *delay)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    SynapticsParameters *para = &priv->synpara;

    /* Treat the first two multi buttons as up/down for now. */
    hw->up |= hw->multi[0];
    hw->down |= hw->multi[1];

    /* 3rd button emulation */
    hw->middle |= HandleMidButtonEmulation(priv, hw, now, delay);

    /* If this is a clickpad and the user clicks in a soft button area, press
     * the soft button instead. */
    if (para->clickpad) {
        /* hw->left is down, but no other buttons were already down */
        if (!(priv->lastButtons & 7) && hw->left && !hw->right && !hw->middle) {
            /* If the finger down event is delayed, the x and y
             * coordinates are stale so we delay processing the click */
            if (hw->z < para->finger_low) {
                hw->left = 0;
                goto out;
            }
            if (is_inside_rightbutton_area(para, hw->x, hw->y)) {
                hw->left = 0;
                hw->right = 1;
            }
            else if (is_inside_sec_rightbutton_area(para, hw->x, hw->y)) {
                hw->left = 0;
                hw->right = 1;
            }
            else if (is_inside_middlebutton_area(para, hw->x, hw->y)) {
                hw->left = 0;
                hw->middle = 1;
            }
            else if (is_inside_sec_middlebutton_area(para, hw->x, hw->y)) {
                hw->left = 0;
                hw->middle = 1;
            }
            priv->clickpad_click_millis = now;
        }
        else if (hw->left) {
            hw->left   = (priv->lastButtons & 1) ? 1 : 0;
            hw->middle = (priv->lastButtons & 2) ? 1 : 0;
            hw->right  = (priv->lastButtons & 4) ? 1 : 0;
        }
    }

    /* Fingers emulate other buttons. ClickFinger can only be
       triggered on transition, when left is pressed
     */
    if (hw->left && !(priv->lastButtons & 7) && hw->numFingers >= 1)
        handle_clickfinger(priv, hw);

out:
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
post_scroll_events(const InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);

    valuator_mask_zero(priv->scroll_events_mask);

    if (priv->scroll.delta_y != 0.0) {
        valuator_mask_set_double(priv->scroll_events_mask,
                                 priv->scroll_axis_vert, priv->scroll.delta_y);
        priv->scroll.delta_y = 0;
    }
    if (priv->scroll.delta_x != 0.0) {
        valuator_mask_set_double(priv->scroll_events_mask,
                                 priv->scroll_axis_horiz, priv->scroll.delta_x);
        priv->scroll.delta_x = 0;
    }
    if (valuator_mask_num_valuators(priv->scroll_events_mask))
        xf86PostMotionEventM(pInfo->dev, FALSE, priv->scroll_events_mask);
}

static inline int
repeat_scrollbuttons(const InputInfoPtr pInfo,
                     const struct SynapticsHwState *hw,
                     int buttons, CARD32 now, int delay)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    SynapticsParameters *para = &priv->synpara;
    int repeat_delay, timeleft;
    int rep_buttons = 0;

    if (para->updown_button_repeat)
        rep_buttons |= (1 << (4 - 1)) | (1 << (5 - 1));
    if (para->leftright_button_repeat)
        rep_buttons |= (1 << (6 - 1)) | (1 << (7 - 1));

    /* Handle auto repeat buttons */
    repeat_delay = clamp(para->scroll_button_repeat, SBR_MIN, SBR_MAX);
    if (((hw->up || hw->down) && para->updown_button_repeat &&
         para->updown_button_scrolling) ||
        ((hw->multi[2] || hw->multi[3]) && para->leftright_button_repeat &&
         para->leftright_button_scrolling)) {
        priv->repeatButtons = buttons & rep_buttons;
        if (!priv->nextRepeat) {
            priv->nextRepeat = now + repeat_delay * 2;
        }
    }
    else {
        priv->repeatButtons = 0;
        priv->nextRepeat = 0;
    }

    if (priv->repeatButtons) {
        timeleft = TIME_DIFF(priv->nextRepeat, now);
        if (timeleft > 0)
            delay = MIN(delay, timeleft);
        if (timeleft <= 0) {
            int change, id;

            change = priv->repeatButtons;
            while (change) {
                id = ffs(change);
                change &= ~(1 << (id - 1));
                if (id == 4)
                    priv->scroll.delta_y -= para->scroll_dist_vert;
                else if (id == 5)
                    priv->scroll.delta_y += para->scroll_dist_vert;
                else if (id == 6)
                    priv->scroll.delta_x -= para->scroll_dist_horiz;
                else if (id == 7)
                    priv->scroll.delta_x += para->scroll_dist_horiz;
            }

            priv->nextRepeat = now + repeat_delay;
            delay = MIN(delay, repeat_delay);
        }
    }

    return delay;
}

/* Update the open slots and number of active touches */
static void
UpdateTouchState(InputInfoPtr pInfo, struct SynapticsHwState *hw)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    int i;

    for (i = 0; i < hw->num_mt_mask; i++) {
        if (hw->slot_state[i] == SLOTSTATE_OPEN) {
            priv->open_slots[priv->num_active_touches] = i;
            priv->num_active_touches++;
            BUG_WARN(priv->num_active_touches > priv->num_slots);
        }
        else if (hw->slot_state[i] == SLOTSTATE_CLOSE) {
            Bool found = FALSE;
            int j;

            for (j = 0; j < priv->num_active_touches - 1; j++) {
                if (priv->open_slots[j] == i)
                    found = TRUE;

                if (found)
                    priv->open_slots[j] = priv->open_slots[j + 1];
            }

            BUG_WARN(priv->num_active_touches == 0);
            if (priv->num_active_touches > 0)
                priv->num_active_touches--;
        }
    }

    SynapticsResetTouchHwState(hw, FALSE);
}

static void
HandleTouches(InputInfoPtr pInfo, struct SynapticsHwState *hw)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;
    SynapticsParameters *para = &priv->synpara;
    int new_active_touches = priv->num_active_touches;
    int min_touches = 2;
    Bool restart_touches = FALSE;
    int i;

    if (para->click_action[F3_CLICK1] || para->tap_action[F3_TAP])
        min_touches = 4;
    else if (para->click_action[F2_CLICK1] || para->tap_action[F2_TAP] ||
             para->scroll_twofinger_vert || para->scroll_twofinger_horiz)
        min_touches = 3;

    /* Count new number of active touches */
    for (i = 0; i < hw->num_mt_mask; i++) {
        if (hw->slot_state[i] == SLOTSTATE_OPEN)
            new_active_touches++;
        else if (hw->slot_state[i] == SLOTSTATE_CLOSE)
            new_active_touches--;
    }

    if (priv->has_semi_mt)
        goto out;

    if (priv->num_active_touches < min_touches &&
        new_active_touches < min_touches) {
        /* We stayed below number of touches needed to send events */
        goto out;
    }
    else if (priv->num_active_touches >= min_touches &&
             new_active_touches < min_touches) {
        /* We are transitioning to less than the number of touches needed to
         * send events. End all currently open touches. */
        for (i = 0; i < priv->num_active_touches; i++) {
            int slot = priv->open_slots[i];

            xf86PostTouchEvent(pInfo->dev, slot, XI_TouchEnd, 0,
                               hw->mt_mask[slot]);
        }

        /* Don't send any more events */
        goto out;
    }
    else if (priv->num_active_touches < min_touches &&
             new_active_touches >= min_touches) {
        /* We are transitioning to more than the number of touches needed to
         * send events. Begin all already open touches. */
        restart_touches = TRUE;
        for (i = 0; i < priv->num_active_touches; i++) {
            int slot = priv->open_slots[i];

            xf86PostTouchEvent(pInfo->dev, slot, XI_TouchBegin, 0,
                               hw->mt_mask[slot]);
        }
    }

    /* Send touch begin events for all new touches */
    for (i = 0; i < hw->num_mt_mask; i++)
        if (hw->slot_state[i] == SLOTSTATE_OPEN)
            xf86PostTouchEvent(pInfo->dev, i, XI_TouchBegin, 0, hw->mt_mask[i]);

    /* Send touch update/end events for all the rest */
    for (i = 0; i < priv->num_active_touches; i++) {
        int slot = priv->open_slots[i];

        /* Don't send update event if we just reopened the touch above */
        if (hw->slot_state[slot] == SLOTSTATE_UPDATE && !restart_touches)
            xf86PostTouchEvent(pInfo->dev, slot, XI_TouchUpdate, 0,
                               hw->mt_mask[slot]);
        else if (hw->slot_state[slot] == SLOTSTATE_CLOSE)
            xf86PostTouchEvent(pInfo->dev, slot, XI_TouchEnd, 0,
                               hw->mt_mask[slot]);
    }

 out:
    UpdateTouchState(pInfo, hw);
}

static void
filter_jitter(SynapticsPrivate * priv, int *x, int *y)
{
    SynapticsParameters *para = &priv->synpara;

    priv->hyst_center_x = hysteresis(*x, priv->hyst_center_x, para->hyst_x);
    priv->hyst_center_y = hysteresis(*y, priv->hyst_center_y, para->hyst_y);
    *x = priv->hyst_center_x;
    *y = priv->hyst_center_y;
}

static void
reset_hw_state(struct SynapticsHwState *hw)
{
    hw->x = 0;
    hw->y = 0;
    hw->z = 0;
    hw->numFingers = 0;
    hw->fingerWidth = 0;
}

/*
 * React on changes in the hardware state. This function is called every time
 * the hardware state changes. The return value is used to specify how many
 * milliseconds to wait before calling the function again if no state change
 * occurs.
 *
 * from_timer denotes if HandleState was triggered from a timer (e.g. to
 * generate fake motion events, or for the tap-to-click state machine), rather
 * than from having received a motion event.
 */
static int
HandleState(InputInfoPtr pInfo, struct SynapticsHwState *hw, CARD32 now,
            Bool from_timer)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) (pInfo->private);
    SynapticsParameters *para = &priv->synpara;
    enum FingerState finger = FS_UNTOUCHED;
    int dx = 0, dy = 0, buttons, id;
    enum EdgeType edge = NO_EDGE;
    int change;
    int double_click = FALSE;
    int delay = 1000000000;
    int timeleft;
    Bool inside_active_area;
    Bool using_cumulative_coords = FALSE;
    Bool ignore_motion;

    /* We need both and x/y, the driver can't handle just one of the two
     * yet. But since it's possible to hit a phys button on non-clickpads
     * without ever getting motion data first, we must continue with 0/0 for
     * that case. */
    if (hw->x == INT_MIN || hw->y == INT_MAX) {
        if (para->clickpad)
            return delay;
        else if (hw->left || hw->right || hw->middle) {
            hw->x = (hw->x == INT_MIN) ? 0 : hw->x;
            hw->y = (hw->y == INT_MIN) ? 0 : hw->y;
        }
    }

    /* If a physical button is pressed on a clickpad or a two-finger scrolling
     * is ongoing, use cumulative relative touch movements for motion */
    if (para->clickpad &&
        ((priv->lastButtons & 7) ||
        (priv->vert_scroll_twofinger_on || priv->horiz_scroll_twofinger_on)) &&
        priv->last_button_area != TOP_BUTTON_AREA) {
        hw->x = hw->cumulative_dx;
        hw->y = hw->cumulative_dy;
        using_cumulative_coords = TRUE;
    }

    /* apply hysteresis before doing anything serious. This cancels
     * out a lot of noise which might surface in strange phenomena
     * like flicker in scrolling or noise motion. */
    filter_jitter(priv, &hw->x, &hw->y);

    inside_active_area = is_inside_active_area(priv, hw->x, hw->y);

    /* Ignore motion *starting* inside softbuttonareas */
    if (priv->finger_state < FS_TOUCHED)
        priv->last_button_area = current_button_area(para, hw->x, hw->y);
    /* If we already have a finger down, clear last_button_area if it goes
       outside of the softbuttonareas */
    else if (priv->last_button_area != NO_BUTTON_AREA &&
             current_button_area(para, hw->x, hw->y) == NO_BUTTON_AREA)
        priv->last_button_area = NO_BUTTON_AREA;

    ignore_motion = para->touchpad_off == TOUCHPAD_OFF ||
        (!using_cumulative_coords && priv->last_button_area != NO_BUTTON_AREA);

    /* these two just update hw->left, right, etc. */
    update_hw_button_state(pInfo, hw, now, &delay);
    if (priv->has_scrollbuttons)
        double_click = adjust_state_from_scrollbuttons(pInfo, hw);

    /* Ignore motion the first X ms after a clickpad click */
    if (priv->clickpad_click_millis) {
        if(TIME_DIFF(priv->clickpad_click_millis +
                     para->clickpad_ignore_motion_time, now) > 0)
            ignore_motion = TRUE;
        else
            priv->clickpad_click_millis = 0;
    }

    /* now we know that these _coordinates_ aren't in the area.
       invalid are: x, y, z, numFingers, fingerWidth
       valid are: millis, left/right/middle/up/down/etc.
     */
    if (!inside_active_area)
        reset_hw_state(hw);

    /* no edge or finger detection outside of area */
    if (inside_active_area) {
        edge = edge_detection(priv, hw->x, hw->y);
        if (!from_timer)
            finger = SynapticsDetectFinger(priv, hw);
        else
            finger = priv->finger_state;
    }

    /* tap and drag detection. Needs to be performed even if the finger is in
     * the dead area to reset the state. */
    timeleft = HandleTapProcessing(priv, hw, now, finger, inside_active_area);
    if (timeleft > 0)
        delay = MIN(delay, timeleft);

    if (inside_active_area) {
        /* Don't bother about scrolling in the dead area of the touchpad. */
        timeleft = HandleScrolling(priv, hw, edge, (finger >= FS_TOUCHED));
        if (timeleft > 0)
            delay = MIN(delay, timeleft);

        /*
         * Compensate for unequal x/y resolution. This needs to be done after
         * calculations that require unadjusted coordinates, for example edge
         * detection.
         */
#ifndef NO_DRIVER_SCALING
        ScaleCoordinates(priv, hw);
#endif
    }

    dx = dy = 0;

    timeleft = ComputeDeltas(priv, hw, edge, &dx, &dy, inside_active_area);
    delay = MIN(delay, timeleft);

    buttons = ((hw->left ? 0x01 : 0) |
               (hw->middle ? 0x02 : 0) |
               (hw->right ? 0x04 : 0) |
               (hw->up ? 0x08 : 0) |
               (hw->down ? 0x10 : 0) |
               (hw->multi[2] ? 0x20 : 0) | (hw->multi[3] ? 0x40 : 0));

    if (priv->tap_button > 0 && priv->tap_button_state == TBS_BUTTON_DOWN)
        buttons |= 1 << (priv->tap_button - 1);

    /* Post events */
    if (finger >= FS_TOUCHED && (dx || dy) && !ignore_motion)
        xf86PostMotionEvent(pInfo->dev, 0, 0, 2, dx, dy);

    if (priv->mid_emu_state == MBE_LEFT_CLICK) {
        post_button_click(pInfo, 1);
        priv->mid_emu_state = MBE_OFF;
    }
    else if (priv->mid_emu_state == MBE_RIGHT_CLICK) {
        post_button_click(pInfo, 3);
        priv->mid_emu_state = MBE_OFF;
    }

    change = buttons ^ priv->lastButtons;
    while (change) {
        id = ffs(change);       /* number of first set bit 1..32 is returned */
        change &= ~(1 << (id - 1));
        xf86PostButtonEvent(pInfo->dev, FALSE, id, (buttons & (1 << (id - 1))),
                            0, 0);
    }

    if (priv->has_scrollbuttons)
        delay = repeat_scrollbuttons(pInfo, hw, buttons, now, delay);

    /* Process scroll events only if coordinates are
     * in the Synaptics Area
     */
    if (inside_active_area &&
        (priv->scroll.delta_x != 0.0 || priv->scroll.delta_y != 0.0)) {
        post_scroll_events(pInfo);
        priv->scroll.last_millis = hw->millis;
    }

    if (double_click) {
        post_button_click(pInfo, 1);
        post_button_click(pInfo, 1);
    }

    HandleTouches(pInfo, hw);

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
    DBG(3, "SwitchMode called\n");

    return XI_BadMode;
}

static void
ReadDevDimensions(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;

    if (priv->proto_ops->ReadDevDimensions)
        priv->proto_ops->ReadDevDimensions(pInfo);

    SanitizeDimensions(pInfo);
}

static Bool
QueryHardware(InputInfoPtr pInfo)
{
    SynapticsPrivate *priv = (SynapticsPrivate *) pInfo->private;

    priv->comm.protoBufTail = 0;

    if (!priv->proto_ops->QueryHardware(pInfo)) {
        xf86IDrvMsg(pInfo, X_PROBED, "no supported touchpad found\n");
        if (priv->proto_ops->DeviceOffHook)
            priv->proto_ops->DeviceOffHook(pInfo);
        return FALSE;
    }

    return TRUE;
}

#ifndef NO_DRIVER_SCALING
static void
ScaleCoordinates(SynapticsPrivate * priv, struct SynapticsHwState *hw)
{
    int xCenter = (priv->synpara.left_edge + priv->synpara.right_edge) / 2;
    int yCenter = (priv->synpara.top_edge + priv->synpara.bottom_edge) / 2;

    hw->x = (hw->x - xCenter) * priv->horiz_coeff + xCenter;
    hw->y = (hw->y - yCenter) * priv->vert_coeff + yCenter;
}

void
CalculateScalingCoeffs(SynapticsPrivate * priv)
{
    int vertRes = priv->synpara.resolution_vert;
    int horizRes = priv->synpara.resolution_horiz;

    if ((horizRes > vertRes) && (horizRes > 0)) {
        priv->horiz_coeff = vertRes / (double) horizRes;
        priv->vert_coeff = 1;
    }
    else if ((horizRes < vertRes) && (vertRes > 0)) {
        priv->horiz_coeff = 1;
        priv->vert_coeff = horizRes / (double) vertRes;
    }
    else {
        priv->horiz_coeff = 1;
        priv->vert_coeff = 1;
    }
}
#endif
