/*
 * Copyright © 2002-2005,2007 Peter Osterlund
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
 *      Peter Osterlund (petero2@telia.com)
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <limits.h>

#include <X11/Xdefs.h>
#include <X11/Xatom.h>
#include <X11/extensions/XI.h>
#include <X11/extensions/XInput.h>
#include "synaptics-properties.h"

#ifndef XATOM_FLOAT
#define XATOM_FLOAT "FLOAT"
#endif

#define SYN_MAX_BUTTONS 12
#define SBR_MIN 10
#define SBR_MAX 1000

union flong {                   /* Xlibs 64-bit property handling madness */
    long l;
    float f;
};

enum ParaType {
    PT_INT,
    PT_BOOL,
    PT_DOUBLE
};

struct Parameter {
    char *name;                 /* Name of parameter */
    enum ParaType type;         /* Type of parameter */
    double min_val;             /* Minimum allowed value */
    double max_val;             /* Maximum allowed value */
    char *prop_name;            /* Property name */
    int prop_format;            /* Property format (0 for floats) */
    int prop_offset;            /* Offset inside property */
};

static struct Parameter params[] = {
    {"LeftEdge",              PT_INT,    0, 10000, SYNAPTICS_PROP_EDGES,	32,	0},
    {"RightEdge",             PT_INT,    0, 10000, SYNAPTICS_PROP_EDGES,	32,	1},
    {"TopEdge",               PT_INT,    0, 10000, SYNAPTICS_PROP_EDGES,	32,	2},
    {"BottomEdge",            PT_INT,    0, 10000, SYNAPTICS_PROP_EDGES,	32,	3},
    {"FingerLow",             PT_INT,    0, 255,   SYNAPTICS_PROP_FINGER,	32,	0},
    {"FingerHigh",            PT_INT,    0, 255,   SYNAPTICS_PROP_FINGER,	32,	1},
    {"MaxTapTime",            PT_INT,    0, 1000,  SYNAPTICS_PROP_TAP_TIME,	32,	0},
    {"MaxTapMove",            PT_INT,    0, 2000,  SYNAPTICS_PROP_TAP_MOVE,	32,	0},
    {"MaxDoubleTapTime",      PT_INT,    0, 1000,  SYNAPTICS_PROP_TAP_DURATIONS,32,	1},
    {"SingleTapTimeout",      PT_INT,    0, 1000,  SYNAPTICS_PROP_TAP_DURATIONS,32,	0},
    {"ClickTime",             PT_INT,    0, 1000,  SYNAPTICS_PROP_TAP_DURATIONS,32,	2},
    {"EmulateMidButtonTime",  PT_INT,    0, 1000,  SYNAPTICS_PROP_MIDDLE_TIMEOUT,32,	0},
    {"EmulateTwoFingerMinZ",  PT_INT,    0, 1000,  SYNAPTICS_PROP_TWOFINGER_PRESSURE,	32,	0},
    {"EmulateTwoFingerMinW",  PT_INT,    0, 15,    SYNAPTICS_PROP_TWOFINGER_WIDTH,	32,	0},
    {"VertScrollDelta",       PT_INT,    -1000, 1000,  SYNAPTICS_PROP_SCROLL_DISTANCE,	32,	0},
    {"HorizScrollDelta",      PT_INT,    -1000, 1000,  SYNAPTICS_PROP_SCROLL_DISTANCE,	32,	1},
    {"VertEdgeScroll",        PT_BOOL,   0, 1,     SYNAPTICS_PROP_SCROLL_EDGE,	8,	0},
    {"HorizEdgeScroll",       PT_BOOL,   0, 1,     SYNAPTICS_PROP_SCROLL_EDGE,	8,	1},
    {"CornerCoasting",        PT_BOOL,   0, 1,     SYNAPTICS_PROP_SCROLL_EDGE,	8,	2},
    {"VertTwoFingerScroll",   PT_BOOL,   0, 1,     SYNAPTICS_PROP_SCROLL_TWOFINGER,	8,	0},
    {"HorizTwoFingerScroll",  PT_BOOL,   0, 1,     SYNAPTICS_PROP_SCROLL_TWOFINGER,	8,	1},
    {"MinSpeed",              PT_DOUBLE, 0, 255.0,   SYNAPTICS_PROP_SPEED,	0, /*float */	0},
    {"MaxSpeed",              PT_DOUBLE, 0, 255.0,   SYNAPTICS_PROP_SPEED,	0, /*float */	1},
    {"AccelFactor",           PT_DOUBLE, 0, 1.0,   SYNAPTICS_PROP_SPEED,	0, /*float */	2},
    {"UpDownScrolling",       PT_BOOL,   0, 1,     SYNAPTICS_PROP_BUTTONSCROLLING,  8,	0},
    {"LeftRightScrolling",    PT_BOOL,   0, 1,     SYNAPTICS_PROP_BUTTONSCROLLING,  8,	1},
    {"UpDownScrollRepeat",    PT_BOOL,   0, 1,     SYNAPTICS_PROP_BUTTONSCROLLING_REPEAT,   8,	0},
    {"LeftRightScrollRepeat", PT_BOOL,   0, 1,     SYNAPTICS_PROP_BUTTONSCROLLING_REPEAT,   8,	1},
    {"ScrollButtonRepeat",    PT_INT,    SBR_MIN , SBR_MAX, SYNAPTICS_PROP_BUTTONSCROLLING_TIME, 32,	0},
    {"TouchpadOff",           PT_INT,    0, 2,     SYNAPTICS_PROP_OFF,		8,	0},
    {"LockedDrags",           PT_BOOL,   0, 1,     SYNAPTICS_PROP_LOCKED_DRAGS,	8,	0},
    {"LockedDragTimeout",     PT_INT,    0, 30000, SYNAPTICS_PROP_LOCKED_DRAGS_TIMEOUT,	32,	0},
    {"RTCornerButton",        PT_INT,    0, SYN_MAX_BUTTONS, SYNAPTICS_PROP_TAP_ACTION,	8,	0},
    {"RBCornerButton",        PT_INT,    0, SYN_MAX_BUTTONS, SYNAPTICS_PROP_TAP_ACTION,	8,	1},
    {"LTCornerButton",        PT_INT,    0, SYN_MAX_BUTTONS, SYNAPTICS_PROP_TAP_ACTION,	8,	2},
    {"LBCornerButton",        PT_INT,    0, SYN_MAX_BUTTONS, SYNAPTICS_PROP_TAP_ACTION,	8,	3},
    {"TapButton1",            PT_INT,    0, SYN_MAX_BUTTONS, SYNAPTICS_PROP_TAP_ACTION,	8,	4},
    {"TapButton2",            PT_INT,    0, SYN_MAX_BUTTONS, SYNAPTICS_PROP_TAP_ACTION,	8,	5},
    {"TapButton3",            PT_INT,    0, SYN_MAX_BUTTONS, SYNAPTICS_PROP_TAP_ACTION,	8,	6},
    {"ClickFinger1",          PT_INT,    0, SYN_MAX_BUTTONS, SYNAPTICS_PROP_CLICK_ACTION,	8,	0},
    {"ClickFinger2",          PT_INT,    0, SYN_MAX_BUTTONS, SYNAPTICS_PROP_CLICK_ACTION,	8,	1},
    {"ClickFinger3",          PT_INT,    0, SYN_MAX_BUTTONS, SYNAPTICS_PROP_CLICK_ACTION,	8,	2},
    {"CircularScrolling",     PT_BOOL,   0, 1,     SYNAPTICS_PROP_CIRCULAR_SCROLLING,	8,	0},
    {"CircScrollDelta",       PT_DOUBLE, .01, 3,   SYNAPTICS_PROP_CIRCULAR_SCROLLING_DIST,	0 /* float */,	0},
    {"CircScrollTrigger",     PT_INT,    0, 8,     SYNAPTICS_PROP_CIRCULAR_SCROLLING_TRIGGER,	8,	0},
    {"CircularPad",           PT_BOOL,   0, 1,     SYNAPTICS_PROP_CIRCULAR_PAD,	8,	0},
    {"PalmDetect",            PT_BOOL,   0, 1,     SYNAPTICS_PROP_PALM_DETECT,	8,	0},
    {"PalmMinWidth",          PT_INT,    0, 15,    SYNAPTICS_PROP_PALM_DIMENSIONS,	32,	0},
    {"PalmMinZ",              PT_INT,    0, 255,   SYNAPTICS_PROP_PALM_DIMENSIONS,	32,	1},
    {"CoastingSpeed",         PT_DOUBLE, 0, 255,    SYNAPTICS_PROP_COASTING_SPEED,	0 /* float*/,	0},
    {"CoastingFriction",      PT_DOUBLE, 0, 255,   SYNAPTICS_PROP_COASTING_SPEED,	0 /* float*/,	1},
    {"PressureMotionMinZ",    PT_INT,    1, 255,   SYNAPTICS_PROP_PRESSURE_MOTION,	32,	0},
    {"PressureMotionMaxZ",    PT_INT,    1, 255,   SYNAPTICS_PROP_PRESSURE_MOTION,	32,	1},
    {"PressureMotionMinFactor", PT_DOUBLE, 0, 10.0,SYNAPTICS_PROP_PRESSURE_MOTION_FACTOR,	0 /*float*/,	0},
    {"PressureMotionMaxFactor", PT_DOUBLE, 0, 10.0,SYNAPTICS_PROP_PRESSURE_MOTION_FACTOR,	0 /*float*/,	1},
    {"GrabEventDevice",       PT_BOOL,   0, 1,     SYNAPTICS_PROP_GRAB,	8,	0},
    {"TapAndDragGesture",     PT_BOOL,   0, 1,     SYNAPTICS_PROP_GESTURES,	8,	0},
    {"AreaLeftEdge",          PT_INT, INT_MIN, INT_MAX, SYNAPTICS_PROP_AREA,	32,	0},
    {"AreaRightEdge",         PT_INT, INT_MIN, INT_MAX, SYNAPTICS_PROP_AREA,	32,	1},
    {"AreaTopEdge",           PT_INT, INT_MIN, INT_MAX, SYNAPTICS_PROP_AREA,	32,	2},
    {"AreaBottomEdge",        PT_INT, INT_MIN, INT_MAX, SYNAPTICS_PROP_AREA,	32,	3},
    {"HorizHysteresis",       PT_INT,    0, 10000, SYNAPTICS_PROP_NOISE_CANCELLATION, 32,	0},
    {"VertHysteresis",        PT_INT,    0, 10000, SYNAPTICS_PROP_NOISE_CANCELLATION, 32,	1},
    {"ClickPad",              PT_BOOL,   0, 1,     SYNAPTICS_PROP_CLICKPAD,	8,	0},
    {"RightButtonAreaLeft",   PT_INT, INT_MIN, INT_MAX, SYNAPTICS_PROP_SOFTBUTTON_AREAS,	32,	0},
    {"RightButtonAreaRight",  PT_INT, INT_MIN, INT_MAX, SYNAPTICS_PROP_SOFTBUTTON_AREAS,	32,	1},
    {"RightButtonAreaTop",    PT_INT, INT_MIN, INT_MAX, SYNAPTICS_PROP_SOFTBUTTON_AREAS,	32,	2},
    {"RightButtonAreaBottom", PT_INT, INT_MIN, INT_MAX, SYNAPTICS_PROP_SOFTBUTTON_AREAS,	32,	3},
    {"MiddleButtonAreaLeft",  PT_INT, INT_MIN, INT_MAX, SYNAPTICS_PROP_SOFTBUTTON_AREAS,	32,	4},
    {"MiddleButtonAreaRight", PT_INT, INT_MIN, INT_MAX, SYNAPTICS_PROP_SOFTBUTTON_AREAS,	32,	5},
    {"MiddleButtonAreaTop",   PT_INT, INT_MIN, INT_MAX, SYNAPTICS_PROP_SOFTBUTTON_AREAS,	32,	6},
    {"MiddleButtonAreaBottom", PT_INT, INT_MIN, INT_MAX, SYNAPTICS_PROP_SOFTBUTTON_AREAS,	32,	7},
    { NULL, 0, 0, 0, 0 }
};

static double
parse_cmd(char *cmd, struct Parameter **par)
{
    char *eqp = strchr(cmd, '=');

    *par = NULL;

    if (eqp) {
        int j;
        int found = 0;

        *eqp = 0;
        for (j = 0; params[j].name; j++) {
            if (strcasecmp(cmd, params[j].name) == 0) {
                found = 1;
                break;
            }
        }
        if (found) {
            double val = atof(&eqp[1]);

            *par = &params[j];

            if (val < (*par)->min_val)
                val = (*par)->min_val;
            if (val > (*par)->max_val)
                val = (*par)->max_val;

            return val;
        }
        else {
            printf("Unknown parameter %s\n", cmd);
        }
    }
    else {
        printf("Invalid command: %s\n", cmd);
    }

    return 0;
}

/** Init display connection or NULL on error */
static Display *
dp_init()
{
    Display *dpy = NULL;
    XExtensionVersion *v = NULL;
    Atom touchpad_type = 0;
    Atom synaptics_property = 0;
    int error = 0;

    dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Failed to connect to X Server.\n");
        error = 1;
        goto unwind;
    }

    v = XGetExtensionVersion(dpy, INAME);
    if (!v->present ||
        (v->major_version * 1000 + v->minor_version) <
        (XI_Add_DeviceProperties_Major * 1000 +
         XI_Add_DeviceProperties_Minor)) {
        fprintf(stderr, "X server supports X Input %d.%d. I need %d.%d.\n",
                v->major_version, v->minor_version,
                XI_Add_DeviceProperties_Major, XI_Add_DeviceProperties_Minor);
        error = 1;
        goto unwind;
    }

    /* We know synaptics sets XI_TOUCHPAD for all the devices. */
    touchpad_type = XInternAtom(dpy, XI_TOUCHPAD, True);
    if (!touchpad_type) {
        fprintf(stderr, "XI_TOUCHPAD not initialised.\n");
        error = 1;
        goto unwind;
    }

    synaptics_property = XInternAtom(dpy, SYNAPTICS_PROP_EDGES, True);
    if (!synaptics_property) {
        fprintf(stderr, "Couldn't find synaptics properties. No synaptics "
                "driver loaded?\n");
        error = 1;
        goto unwind;
    }

 unwind:
    XFree(v);
    if (error && dpy) {
        XCloseDisplay(dpy);
        dpy = NULL;
    }
    return dpy;
}

static XDevice *
dp_get_device(Display * dpy)
{
    XDevice *dev = NULL;
    XDeviceInfo *info = NULL;
    int ndevices = 0;
    Atom touchpad_type = 0;
    Atom synaptics_property = 0;
    Atom *properties = NULL;
    int nprops = 0;
    int error = 0;

    touchpad_type = XInternAtom(dpy, XI_TOUCHPAD, True);
    synaptics_property = XInternAtom(dpy, SYNAPTICS_PROP_EDGES, True);
    info = XListInputDevices(dpy, &ndevices);

    while (ndevices--) {
        if (info[ndevices].type == touchpad_type) {
            dev = XOpenDevice(dpy, info[ndevices].id);
            if (!dev) {
                fprintf(stderr, "Failed to open device '%s'.\n",
                        info[ndevices].name);
                error = 1;
                goto unwind;
            }

            properties = XListDeviceProperties(dpy, dev, &nprops);
            if (!properties || !nprops) {
                fprintf(stderr, "No properties on device '%s'.\n",
                        info[ndevices].name);
                error = 1;
                goto unwind;
            }

            while (nprops--) {
                if (properties[nprops] == synaptics_property)
                    break;
            }
            if (!nprops) {
                fprintf(stderr, "No synaptics properties on device '%s'.\n",
                        info[ndevices].name);
                error = 1;
                goto unwind;
            }

            break;              /* Yay, device is suitable */
        }
    }

 unwind:
    XFree(properties);
    XFreeDeviceList(info);
    if (!dev)
        fprintf(stderr, "Unable to find a synaptics device.\n");
    else if (error && dev) {
        XCloseDevice(dpy, dev);
        dev = NULL;
    }
    return dev;
}

static void
dp_set_variables(Display * dpy, XDevice * dev, int argc, char *argv[],
                 int first_cmd)
{
    int i;
    double val;
    struct Parameter *par;
    Atom prop, type, float_type;
    int format;
    unsigned char *data;
    unsigned long nitems, bytes_after;

    union flong *f;
    long *n;
    char *b;

    float_type = XInternAtom(dpy, XATOM_FLOAT, True);
    if (!float_type)
        fprintf(stderr, "Float properties not available.\n");

    for (i = first_cmd; i < argc; i++) {
        val = parse_cmd(argv[i], &par);
        if (!par)
            continue;

        prop = XInternAtom(dpy, par->prop_name, True);
        if (!prop) {
            fprintf(stderr, "Property for '%s' not available. Skipping.\n",
                    par->name);
            continue;

        }

        XGetDeviceProperty(dpy, dev, prop, 0, 1000, False, AnyPropertyType,
                           &type, &format, &nitems, &bytes_after, &data);

        if (type == None) {
            fprintf(stderr, "Property for '%s' not available. Skipping.\n",
                    par->name);
            continue;
        }

        switch (par->prop_format) {
        case 8:
            if (format != par->prop_format || type != XA_INTEGER) {
                fprintf(stderr, "   %-23s = format mismatch (%d)\n",
                        par->name, format);
                break;
            }
            b = (char *) data;
            b[par->prop_offset] = rint(val);
            break;
        case 32:
            if (format != par->prop_format ||
                (type != XA_INTEGER && type != XA_CARDINAL)) {
                fprintf(stderr, "   %-23s = format mismatch (%d)\n",
                        par->name, format);
                break;
            }
            n = (long *) data;
            n[par->prop_offset] = rint(val);
            break;
        case 0:                /* float */
            if (!float_type)
                continue;
            if (format != 32 || type != float_type) {
                fprintf(stderr, "   %-23s = format mismatch (%d)\n",
                        par->name, format);
                break;
            }
            f = (union flong *) data;
            f[par->prop_offset].f = val;
            break;
        }

        XChangeDeviceProperty(dpy, dev, prop, type, format,
                              PropModeReplace, data, nitems);
        XFlush(dpy);
    }
}

/* FIXME: horribly inefficient. */
static void
dp_show_settings(Display * dpy, XDevice * dev)
{
    int j;
    Atom a, type, float_type;
    int format;
    unsigned long nitems, bytes_after;
    unsigned char *data;
    int len;

    union flong *f;
    long *i;
    char *b;

    float_type = XInternAtom(dpy, XATOM_FLOAT, True);
    if (!float_type)
        fprintf(stderr, "Float properties not available.\n");

    printf("Parameter settings:\n");
    for (j = 0; params[j].name; j++) {
        struct Parameter *par = &params[j];

        a = XInternAtom(dpy, par->prop_name, True);
        if (!a)
            continue;

        len =
            1 +
            ((par->prop_offset * (par->prop_format ? par->prop_format : 32) /
              8)) / 4;

        XGetDeviceProperty(dpy, dev, a, 0, len, False,
                           AnyPropertyType, &type, &format,
                           &nitems, &bytes_after, &data);
        if (type == None)
            continue;

        switch (par->prop_format) {
        case 8:
            if (format != par->prop_format || type != XA_INTEGER) {
                fprintf(stderr, "    %-23s = format mismatch (%d)\n",
                        par->name, format);
                break;
            }

            b = (char *) data;
            printf("    %-23s = %d\n", par->name, b[par->prop_offset]);
            break;
        case 32:
            if (format != par->prop_format ||
                (type != XA_INTEGER && type != XA_CARDINAL)) {
                fprintf(stderr, "    %-23s = format mismatch (%d)\n",
                        par->name, format);
                break;
            }

            i = (long *) data;
            printf("    %-23s = %ld\n", par->name, i[par->prop_offset]);
            break;
        case 0:                /* Float */
            if (!float_type)
                continue;
            if (format != 32 || type != float_type) {
                fprintf(stderr, "    %-23s = format mismatch (%d)\n",
                        par->name, format);
                break;
            }

            f = (union flong *) data;
            printf("    %-23s = %g\n", par->name, f[par->prop_offset].f);
            break;
        }

        XFree(data);
    }
}

static void
usage(void)
{
    fprintf(stderr, "Usage: synclient [-h] [-l] [-V] [-?] [var1=value1 [var2=value2] ...]\n");
    fprintf(stderr, "  -l List current user settings\n");
    fprintf(stderr, "  -V Print synclient version string and exit\n");
    fprintf(stderr, "  -? Show this help message\n");
    fprintf(stderr, "  var=value  Set user parameter 'var' to 'value'.\n");
    exit(1);
}

int
main(int argc, char *argv[])
{
    int c;
    int dump_settings = 0;
    int first_cmd;

    Display *dpy;
    XDevice *dev;

    if (argc == 1)
        dump_settings = 1;

    /* Parse command line parameters */
    while ((c = getopt(argc, argv, "lV?")) != -1) {
        switch (c) {
        case 'l':
            dump_settings = 1;
            break;
        case 'V':
            printf("%s\n", VERSION);
            exit(0);
        case '?':
        default:
            usage();
        }
    }

    first_cmd = optind;
    if (!dump_settings && first_cmd == argc)
        usage();

    dpy = dp_init();
    if (!dpy || !(dev = dp_get_device(dpy)))
        return 1;

    dp_set_variables(dpy, dev, argc, argv, first_cmd);
    if (dump_settings)
        dp_show_settings(dpy, dev);

    XCloseDevice(dpy, dev);
    XCloseDisplay(dpy);

    return 0;
}
