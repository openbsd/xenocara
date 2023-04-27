/*      $OpenBSD: xtsscale.c,v 1.25 2023/04/27 18:17:25 bentley Exp $ */
/*
 * Copyright (c) 2007 Robert Nagy <robert@openbsd.org>
 * Copyright (c) 2009,2011 Matthieu Herrb <matthieu@herrb.eu>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE OPENBSD PROJECT AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OPENBSD
 * PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copyright 1996 by Frederic Lepied, France. <Frederic.Lepied@sugix.frmug.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is  hereby granted without fee, provided that
 * the  above copyright   notice appear  in   all  copies and  that both  that
 * copyright  notice   and   this  permission   notice  appear  in  supporting
 * documentation, and that   the  name of  the authors  not  be  used  in
 * advertising or publicity pertaining to distribution of the software without
 * specific,  written      prior  permission.     The authors  make  no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * THE AUTHORS DISCLAIM ALL   WARRANTIES WITH REGARD  TO  THIS SOFTWARE,
 * INCLUDING ALL IMPLIED   WARRANTIES OF MERCHANTABILITY  AND   FITNESS, IN NO
 * EVENT  SHALL THE AUTHORS  BE   LIABLE   FOR ANY  SPECIAL, INDIRECT   OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA  OR PROFITS, WHETHER  IN  AN ACTION OF  CONTRACT,  NEGLIGENCE OR OTHER
 * TORTIOUS  ACTION, ARISING    OUT OF OR   IN  CONNECTION  WITH THE USE    OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xft/Xft.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/XInput.h>

#include <X11/extensions/Xrandr.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <ws-properties.h>

#define FONT_NAME		"mono"
#define FONT_SIZE		14

#define Background		"white"
#define TouchCross		"black"
#define PromptText		"black"
#define Error			"red"

#define INVALID_EVENT_TYPE	-1

static int           motion_type = INVALID_EVENT_TYPE;
static int           button_press_type = INVALID_EVENT_TYPE;
static int           button_release_type = INVALID_EVENT_TYPE;
static int           proximity_in_type = INVALID_EVENT_TYPE;
static int           proximity_out_type = INVALID_EVENT_TYPE;

int has_xrandr = False;
int has_xrandr_1_2 = False;
int has_xrandr_1_3 = False;
int has_xinerama = False;

Atom prop_calibration, prop_swap;

/* where the calibration points are placed */
#define SCREEN_DIVIDE	16
#define SCREEN_MAX	0x800
#define M_POINT		(SCREEN_MAX/SCREEN_DIVIDE)
int             MARK_POINT[] = {M_POINT, SCREEN_MAX - 1 - M_POINT};

int             touched = 0;
char           *deviceName;

Display        *display;
int             screen;
Window          root;
Window          win;
XftFont	       *font;
XftColor	cross, errorColor, promptColor, bg;
XftDraw	       *draw;
unsigned int    xpos, ypos, width, height;	/* window size */
Bool		interrupted = False;
Bool		verbose = False;

int    cx[5], cy[5];
int    x[5], y[5];

extern char * __progname;

struct { int minx, maxx, miny, maxy, swapxy, resx, resy; } calib, old_calib;
Bool old_swap;

static char    *prompt_message[] = {
	"TOUCH SCREEN CALIBRATION",
	"Press on the crosshairs please...",
	"Use the ESC key to cancel.",
	NULL
};

static char *error_message[] = {
	"Not accurate enough.",
	"Try again...",
	NULL
};

void
cleanup_exit(XDevice *device)
{
	long values[4];

	values[0] = old_calib.minx;
	values[1] = old_calib.maxx;
	values[2] = old_calib.miny;
	values[3] = old_calib.maxy;

	XChangeDeviceProperty(display, device, prop_calibration,
	    XA_INTEGER, 32, PropModeReplace, (unsigned char *)values, 4);

	XChangeDeviceProperty(display, device, prop_swap,
	    XA_INTEGER, 8, PropModeReplace, (unsigned char *)&old_swap, 1);

	XCloseDevice(display, device);
	XUngrabServer(display);
	XUngrabKeyboard(display, CurrentTime);
	XCloseDisplay(display);
	exit(1);
}

void
render_init(void)
{
	font = XftFontOpen(display, screen,
	    XFT_FAMILY, XftTypeString, FONT_NAME,
	    XFT_SIZE, XftTypeInteger, FONT_SIZE,
	    NULL);
	if (!XftColorAllocName(display, XDefaultVisual(display, screen),
		DefaultColormap(display, screen), TouchCross, &cross)) {
		fprintf(stderr, "Cannot get color");
		exit(2);
	}
	if (!XftColorAllocName(display, XDefaultVisual(display, screen),
		DefaultColormap(display, screen), PromptText, &promptColor)) {
		fprintf(stderr, "Cannot get color");
		exit(2);
	}
	if (!XftColorAllocName(display, XDefaultVisual(display, screen),
		DefaultColormap(display, screen), Background, &bg)) {
		fprintf(stderr, "Cannot get bg color");
		exit(2);
	}
	if (!XftColorAllocName(display, XDefaultVisual(display, screen),
		DefaultColormap(display, screen), Error, &errorColor)) {
		fprintf(stderr, "Cannot get color");
		exit(2);
	}
	draw = XftDrawCreate(display, win, DefaultVisual(display, screen),
	    DefaultColormap(display, screen));
}

void
draw_point(int x, int y, int width, int size, XftColor *color)
{
	XPointDouble p[4];

	p[0].x = x - size;
	p[0].y = y - 1;
	p[1].x = x - size;
	p[1].y = y + 1;
	p[2].x = x + size;
	p[2].y = y + 1;
	p[3].x = x + size;
	p[3].y = y - 1;

	XRenderCompositeDoublePoly(display, PictOpOver,
	    XftDrawSrcPicture(draw, color),
	    XftDrawPicture(draw),
	    XRenderFindStandardFormat(display, PictStandardA8),
	    0, 0, 0, 0, p, 4, 0);
	p[0].x = x - 1;
	p[0].y = y - size;
	p[1].x = x + 1;
	p[1].y = y - size;
	p[2].x = x + 1;
	p[2].y = y + size;
	p[3].x = x - 1;
	p[3].y = y + size;
	XRenderCompositeDoublePoly(display, PictOpOver,
	    XftDrawSrcPicture(draw, color),
	    XftDrawPicture(draw),
	    XRenderFindStandardFormat(display, PictStandardA8),
	    0, 0, 0, 0, p, 4, 0);
}

void
draw_text(char **message, XftColor *color)
{
	int	 	len;
	int             i, x, y;
	XGlyphInfo	extents;

	i = 0;
	y = height / 3;
	while (message[i] != NULL) {
		len = strlen(message[i]);
		XftTextExtents8(display, font, message[i], len, &extents);
		x = (width - extents.width)/2;
		XftDrawString8(draw, color, font, x, y, message[i], len);
		y += extents.height * 1.5;
		i++;
	}
}

void
draw_graphics(int i, int j, int n)
{

	draw_text(prompt_message, &promptColor);

	if (n == 2) {
		cx[n] = width / 2;
		cy[n] = height / 2;
	} else {
		cx[n] = (MARK_POINT[i] * width) / SCREEN_MAX;
		cy[n] = (MARK_POINT[j] * height) / SCREEN_MAX;
	}
	draw_point(cx[n], cy[n], width / 200, width / 64, &cross);
}

Cursor
create_empty_cursor(void)
{
	char            nothing[] = {0};
	XColor          nullcolor;
	Pixmap          src = XCreateBitmapFromData(display, root, nothing, 1, 1);
	Pixmap          msk = XCreateBitmapFromData(display, root, nothing, 1, 1);
	Cursor          mycursor = XCreatePixmapCursor(display, src, msk,
					      &nullcolor, &nullcolor, 0, 0);
	XFreePixmap(display, src);
	XFreePixmap(display, msk);

	return mycursor;
}

int
check_device(XDeviceInfo *info)
{
	XDevice *device;
	Atom type;
	int format;
	unsigned long nitems, nbytes;
	unsigned char *retval;

	if (verbose)
		printf("Checking device %lu: %s...", info->id, info->name);
	device = XOpenDevice(display, info->id);
	XGetDeviceProperty(display,
			   device, prop_calibration,
			   0, 4, False,
			   XA_INTEGER, &type, &format,
			   &nitems, &nbytes, &retval);
	XCloseDevice(display, device);
	if (nitems != 4) {
		if (verbose)
			printf("can't be calibrated\n");
		return False;
	}
	if (verbose)
		printf("can be calibrated\n");
	return True;
}

XDeviceInfo*
find_device_info(char *name)
{
	XDeviceInfo    *devices;
	XDeviceInfo    *found = NULL;
	int		i, max_id;
	int		num_devices, num_found;
	Bool		is_id = True;
	XID		id = (XID)-1;
	const char	       *errstr;

	devices = XListInputDevices(display, &num_devices);
	max_id = 0;
	for (i = 0; i < num_devices; i++)
		if (devices[i].id > max_id)
			max_id = devices[i].id;


	if (name != NULL) {
		for(i = 0; i < strlen(name); i++) {
			if (!isdigit(name[i])) {
				is_id = False;
				break;
			}
		}
		if (is_id) {
			id = strtonum(name, 0, max_id, &errstr);
			if (errstr != NULL) {
				fprintf(stderr, "Invalid device id %s: %s\n",
				    name, errstr);
				exit(1);
			}
		}
	}

	num_found = 0;
	for(i = 0; i < num_devices; i++) {
		if (devices[i].use != IsXExtensionPointer)
			continue;
		if (name == NULL) {
			if (check_device(&devices[i])) {
				found = &devices[i];
				num_found++;
			}
			continue;
		}
		if ((!is_id && strcmp(devices[i].name, name) == 0) ||
		    (is_id && devices[i].id == id)) {
			found = &devices[i];
			num_found++;
		}
	}
	if (num_found > 1) {
		fprintf(stderr,
		    "Error: found multiple matching devices.\n"
		    "To ensure the correct one is selected, please use "
		    "the device ID instead.\n");
		return NULL;
	}
	return found;
}

static int
register_events(XDeviceInfo *info, XDevice *device,
    Bool handle_proximity)
{
	int		 number = 0;	/* number of events registered */
	XEventClass	 event_list[7];
	int		 i;
	unsigned long	 screen;
	XInputClassInfo	*ip;

	screen = DefaultScreen(display);

	if (device->num_classes > 0) {
		for (ip = device->classes, i=0; i<info->num_classes;
		     ip++, i++) {
			switch (ip->input_class) {
			case ButtonClass:
				DeviceButtonPress(device, button_press_type,
				    event_list[number]);
				number++;
				DeviceButtonRelease(device,
				    button_release_type, event_list[number]);
				number++;
				break;

			case ValuatorClass:
				DeviceMotionNotify(device, motion_type,
				    event_list[number]); number++;
				if (handle_proximity) {
					ProximityIn(device, proximity_in_type,
					    event_list[number]); number++;
					ProximityOut(device,
					    proximity_out_type,
					    event_list[number]); number++;
				}
				break;

			default:
				fprintf(stderr,
				    "Found unknown device class %d\n",
				    ip->input_class);
				break;
			}
		}

		if (XSelectExtensionEvent(display, root, event_list, number)) {
			fprintf(stderr, "Error selecting extended events\n");
			return 0;
		}
	}
	return number;
}

static Bool
get_events(int i)
{
	XEvent Event;
	XDeviceMotionEvent *motion = (XDeviceMotionEvent *) &Event;
	int j, a;
	char c;

	x[i] = y[i] = -1;
	while (1) {
		XNextEvent(display, &Event);

		if (Event.type == motion_type) {
			for (j = 0; j < motion->axes_count; j++) {
				a = motion->first_axis + j;
				switch (a) {
				case 0:
					x[i] = motion->axis_data[j];
					break;
				case 1:
					y[i] = motion->axis_data[j];
					break;
				default:
					fprintf(stderr,
					    "Unknown axis %d\n", a);
				}
			}
		} else if (Event.type == button_release_type) {
			if (x[i] != -1 && y[i] != -1)
				break;
		} else if (Event.type == KeyPress) {
			a = XLookupString(&Event.xkey, &c, 1, NULL, NULL);
			if ((a == 1) && ((c == 'q') || (c == 'Q') ||
				(c == '\03') || (c == '\033'))) {
				interrupted++;
				return False;
			}
		}
	}
	if (verbose)
		printf("x[%d] = %d y[%d] = %d\n", i, x[i], i, y[i]);

	return True;
}

int
uncalibrate(XDevice *device)
{
	Atom type;
	int format;
	unsigned long nitems, nbytes;
	long values[4] = { 0, 32767, 0, 32767 }; /* uncalibrated */
	Bool swap = 0;
	unsigned char *retval;

	/* Save old values */
	XGetDeviceProperty(display, device, prop_calibration, 0,
	    4, False, XA_INTEGER, &type, &format, &nitems,
	    &nbytes, &retval);

	if (type != XA_INTEGER) {
		fprintf(stderr, "Device property \"%s\": invalid type %s\n",
		    WS_PROP_CALIBRATION, XGetAtomName(display, type));
		return -1;
	}
	if (nitems != 4 && nitems != 0) {
		fprintf(stderr, "Device property \"%s\": "
		    "invalid number of items %ld\n",
		    WS_PROP_CALIBRATION, nitems);
		return -1;
	}
	old_calib.minx = *(long *)retval;
	old_calib.maxx = *((long *)retval + 1);
	old_calib.miny = *((long *)retval + 2);
	old_calib.maxy = *((long *)retval + 3);

	XFree(retval);

	XGetDeviceProperty(display, device, prop_swap, 0,
	    1, False, XA_INTEGER, &type, &format, &nitems,
	    &nbytes, &retval);
	old_swap = *(Bool *)retval;
	XFree(retval);

	/* Force uncalibrated state */
	XChangeDeviceProperty(display, device, prop_calibration,
	    XA_INTEGER, 32, PropModeReplace, (unsigned char *)values, 4);
	XChangeDeviceProperty(display, device, prop_swap,
	    XA_INTEGER, 8, PropModeReplace, (unsigned char *)&swap, 1);

	return 0;
}

void
get_xrandr_config(Display *dpy, Window root, char *name,
    int *x, int *y, int *width, int *height)
{
	XRRScreenResources *res;
	XRROutputInfo *output_info;
	XRRCrtcInfo *crtc_info;
	int o, found = 0;

	res = XRRGetScreenResources(dpy, root);

	for (o = 0; o < res->noutput; o++) {
		output_info = XRRGetOutputInfo (dpy, res, res->outputs[o]);
		if (!output_info) {
			fprintf(stderr,
			    "could not get output 0x%lx information\n",
			    res->outputs[o]);
			exit(2);
		}
		if (output_info->crtc != 0) {
			crtc_info = XRRGetCrtcInfo(dpy, res,
			    output_info->crtc);
			if (!crtc_info) {
				fprintf(stderr,
				    "%s: could not get crtc 0x%lx "
				    "information\n", __progname,
				    output_info->crtc);
				exit(2);
			}
			printf("%s: %dx%d+%d+%d\n",
			    output_info->name,
			    crtc_info->width, crtc_info->height,
			    crtc_info->x, crtc_info->y);
			if (!strcmp(output_info->name, name)) {
				*x = crtc_info->x;
				*y = crtc_info->y;
				*width = crtc_info->width;
				*height = crtc_info->height;
				found = 1;
			}
		}
	}
	if (!found) {
		fprintf(stderr, "%s: output %s not found\n", __progname, name);
		exit(2);
	}
}

void __dead
usage(void)
{
	fprintf(stderr, "usage: xtsscale [-c][-D display]"
		"[-d device][-o output]\n");
	exit(2);
}

int
main(int argc, char *argv[], char *env[])
{
	char           *display_name = NULL;
	char	       *device_name = NULL;
	char	       *output_name = NULL;
	XSetWindowAttributes xswa;
	int             i = 0;
	double          a, a1, a2, b, b1, b2, xerr, yerr;
	int		xi_opcode, event, error;
	XExtensionVersion *version;
	XDeviceInfo	*info;
	XDevice		*device;
	long		 calib_data[4];
	unsigned long	 mask;
	unsigned char	 swap;
	int 		 keep_cursor = 0, ch;

	/* Crosshair placement */
	int		cpx[] = { 0, 0, 1, 1, 1 };
	int		cpy[] = { 0, 1, 0, 0, 1 };

	while ((ch = getopt(argc, argv, "cD:d:o:v")) != -1) {
		switch (ch) {
		case 'c':
			keep_cursor++;
			break;
		case 'D':
			display_name = optarg;
			break;
		case 'd':
			device_name = optarg;
			break;
		case 'o':
			output_name = optarg;
			break;
		case 'v':
			verbose = True;
			break;
		default:
			usage();
			/* NOTREACHED */
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 0)
		usage();

	/* connect to X server */
	if ((display = XOpenDisplay(display_name)) == NULL) {
		fprintf(stderr, "%s: cannot connect to X server %s\n",
		    __progname, XDisplayName(display_name));
		exit(1);
	}
	screen = DefaultScreen(display);
	root = RootWindow(display, screen);

	/* get screen size from display structure macro */
	xpos = 0;
	ypos = 0;
	width = DisplayWidth(display, screen);
	height = DisplayHeight(display, screen);

	if (XRRQueryExtension(display, &event, &error)) {
		int major, minor;

		if (XRRQueryVersion(display, &major, &minor) != True) {
			fprintf(stderr, "Error querying XRandR version");
		} else {
			printf("XRandR extension version %d.%d present\n",
			    major, minor);
			has_xrandr = True;
			if (major > 1 || (major == 1 && minor >=2))
				has_xrandr_1_2 = True;
			if (major > 1 || (major == 1 && minor >=3))
				has_xrandr_1_3 = True;
		}
	}

	if (output_name != NULL) {
		if (has_xrandr_1_2) {
			get_xrandr_config(display, root, output_name,
			    &xpos, &ypos, &width, &height);
		} else {
			fprintf(stderr, "%s: can not specify an output "
			    "whithout XRandr 1.2 or later", __progname);
			exit(2);
		}
	}
	if (!XQueryExtension(display, INAME, &xi_opcode,
		&event, &error)) {
		fprintf(stderr, "%s: X Input extension not available.\n",
		    __progname);
		exit(1);
	}

	version = XGetExtensionVersion(display, INAME);
	if (version == NULL ||
	    version == (XExtensionVersion *)NoSuchExtension) {
		fprintf(stderr, "Cannot query X Input version.\n");
		exit(1);
	}
	XFree(version);
	prop_calibration = XInternAtom(display, WS_PROP_CALIBRATION, True);
	if (prop_calibration == None) {
		fprintf(stderr, "Unable to find the \"%s\" device property.\n"
		    "There are probably no calibrable devices "
		    "on this system.\n", WS_PROP_CALIBRATION);
		exit(1);
	}
	prop_swap = XInternAtom(display, WS_PROP_SWAP_AXES, True);
	if (prop_swap == None) {
		fprintf(stderr, "Unable to find the \"%s\" device property\n",
		    WS_PROP_SWAP_AXES);
		exit(1);
	}
	info = find_device_info(device_name);
	if (info == NULL) {
		fprintf(stderr, "Unable to find the %s device\n",
			device_name ? device_name : "default");
		exit(1);
	}


	/* setup window attributes */
	xswa.override_redirect = True;
	xswa.background_pixel = BlackPixel(display, screen);
	xswa.event_mask = ExposureMask | KeyPressMask;
	mask = CWOverrideRedirect | CWBackPixel | CWEventMask;
	if (!keep_cursor) {
		xswa.cursor = create_empty_cursor();
		mask |= CWCursor;
	}
	win = XCreateWindow(display, RootWindow(display, screen),
			    xpos, ypos, width, height, 0,
			    CopyFromParent, InputOutput, CopyFromParent,
			    mask, &xswa);
	render_init();
	XMapWindow(display, win);
	XGrabKeyboard(display, win, False, GrabModeAsync, GrabModeAsync,
		      CurrentTime);
	XGrabServer(display);

	XClearWindow(display, win);

	if (verbose)
		printf("Calibrating %s\n", info->name);
	device = XOpenDevice(display, info->id);
	if (!device) {
		fprintf(stderr, "Unable to open the X input device \"%s\"\n",
		    info->name);
		return 0;
	}

	if (!register_events(info, device, 0))
		exit(1);

	uncalibrate(device);
calib:
	XftDrawRect(draw, &bg, 0, 0, width, height);

	for (i = 0; i < 5; i++) {
		draw_graphics(cpx[i], cpy[i], i);
		XFlush(display);
		if (!get_events(i))
			break;
		XftDrawRect(draw, &bg, 0, 0, width, height);
	}
	if (interrupted)
		cleanup_exit(device);

	/* Check if  X and Y should be swapped */
	if (abs(x[0] - x[1]) > abs(y[0] - y[1])) {

		calib.swapxy = 1;

		for (i = 0; i < 5; i++) {
			int t = x[i];
			x[i] = y[i];
			y[i] = t;
		}
	}

	/* get touch pad resolution to screen resolution ratio */
	a1 = (double) (x[4] - x[0]) / (double) (cx[4] - cx[0]);
	a2 = (double) (x[3] - x[1]) / (double) (cx[3] - cx[1]);
	/* get the minimum pad position on the X-axis */
	b1 = x[0] - a1 * cx[0];
	b2 = x[1] - a2 * cx[1];
	/* use the average ratio and average minimum position */
	a = (a1 + a2) / 2.0;
	b = (b1 + b2) / 2.0;
	xerr = a * width / 2 + b - x[2];
	if (fabs(xerr) > fabs(a * width * .01)) {
		fprintf(stderr, "Calibration problem: X axis error (%.2f) too high, try again\n",
			fabs(xerr));
		goto err;
	}
	calib.minx = (int) (b + 0.5);
	calib.maxx = (int) (a * width + b + 0.5);

	/* get touch pad resolution to screen resolution ratio */
	a1 = (double) (y[4] - y[0]) / (double) (cy[4] - cy[0]);
	a2 = (double) (y[3] - y[1]) / (double) (cy[3] - cy[1]);
	/* get the minimum pad position on the Y-axis */
	b1 = y[0] - a1 * cy[0];
	b2 = y[1] - a2 * cy[1];
	/* use the average ratio and average minimum position */
	a = (a1 + a2) / 2.0;
	b = (b1 + b2) / 2.0;
	yerr = a * height / 2 + b - y[2];
	if (fabs(yerr) > fabs(a * height * 0.01)) {
		fprintf(stderr, "Calibration problem: Y axis error (%.2f) too high, try again\n",
			fabs(yerr));
		goto err;
	}
	calib.miny = (int) (b + 0.5);
	calib.maxy = (int) (a * height + b + 0.5);

	XFlush(display);

	calib.resx = width;
	calib.resy = height;

	/* Send new values to the X server */
	calib_data[0] = calib.minx;
	calib_data[1] = calib.maxx;
	calib_data[2] = calib.miny;
	calib_data[3] = calib.maxy;
	XChangeDeviceProperty(display, device, prop_calibration,
	    XA_INTEGER, 32, PropModeReplace, (unsigned char *)calib_data, 4);

	swap = calib.swapxy;
	XChangeDeviceProperty(display, device, prop_swap,
	    XA_INTEGER, 8, PropModeReplace, (unsigned char *)&swap, 1);

	XCloseDevice(display, device);

	XCloseDisplay(display);

	/* And print them for storage in wsconsctl.conf */
	printf("mouse.scale=%d,%d,%d,%d,%d,%d,%d\n",
	    calib.minx, calib.maxx,
	    calib.miny, calib.maxy,
	    calib.swapxy,
	    calib.resx, calib.resy);

	return 0;
err:
	draw_text(error_message, &errorColor);
	XFlush(display);
	sleep(2);
	goto calib;
}
