/*      $OpenBSD: xtsscale.c,v 1.6 2007/08/31 21:53:55 matthieu Exp $ */
/*
 * Copyright (c) 2007 Robert Nagy <robert@openbsd.org>
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

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/Xft/Xft.h>
#include <X11/extensions/Xrender.h>


#include <sys/param.h>
#include <sys/ioctl.h>
#include <dev/wscons/wsconsio.h>

#include <err.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <math.h>


#define FONT_NAME		"mono"
#define FONT_SIZE		14

#define Background		"white"
#define TouchCross	        "black"
#define PromptText		"black"
#define Error			"red"

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
unsigned int    width, height;	/* window size */
char           *progname;
int             evfd;

int    cx[5], cy[5];
int    x[5], y[5];

static char    *prompt_message[] = {
	"TOUCH SCREEN CALIBRATION",
	"Press on the cross hairs please...",
	NULL
};

static char *error_message[] = {
	"Not accurate enough.",
	"Try again...",
	NULL
};

void
get_events(int i)
{
	ssize_t         len;
	int             down;
	struct wscons_event ev;

	down = 0;
	x[i] = y[i] = -1;
	while (down || x[i] == -1 || y[i] == -1) {
		len = read(evfd, &ev, sizeof(ev));
		if (len != 16)
			break;
		switch (ev.type) {
		case WSCONS_EVENT_MOUSE_DOWN:
			down = 1;
			break;
		case WSCONS_EVENT_MOUSE_UP:
			down = 0;
			break;
		case WSCONS_EVENT_MOUSE_ABSOLUTE_X:
			if (down)
				x[i] = ev.value;
			break;
		case WSCONS_EVENT_MOUSE_ABSOLUTE_Y:
			if (down)
				y[i] = ev.value;
			break;
		default:
			break;
		}
	}
}


void
cleanup_exit()
{
	XUngrabServer(display);
	XUngrabKeyboard(display, CurrentTime);
	XCloseDisplay(display);
	close(evfd);
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
		fprintf(stderr, "cannot get color");
		exit(2);
	}
	if (!XftColorAllocName(display, XDefaultVisual(display, screen),
		DefaultColormap(display, screen), PromptText, &promptColor)) {
		fprintf(stderr, "cannot get color");
		exit(2);
	}
	if (!XftColorAllocName(display, XDefaultVisual(display, screen),
		DefaultColormap(display, screen), Background, &bg)) {
		fprintf(stderr, "cannot get bg color");
		exit(2);
	}
	if (!XftColorAllocName(display, XDefaultVisual(display, screen),
		DefaultColormap(display, screen), Error, &errorColor)) {
		fprintf(stderr, "cannot get color");
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
	int      	len;
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
create_empty_cursor()
{
	char            nothing[] = {0};
	XColor          nullcolor;
	Pixmap          src = XCreateBitmapFromData(display, root, nothing, 1, 1);
	Pixmap          msk = XCreateBitmapFromData(display, root, nothing, 1, 1);
	Cursor          mcyursor = XCreatePixmapCursor(display, src, msk,
					      &nullcolor, &nullcolor, 0, 0);
	XFreePixmap(display, src);
	XFreePixmap(display, msk);

	return mcyursor;
}

int
main(int argc, char *argv[], char *env[])
{
	char           *display_name = NULL;
	XSetWindowAttributes xswa;
	int             i = 0, orawmode;
	double          a, a1, a2, b, b1, b2, xerr, yerr;
	struct		wsmouse_calibcoords wmcoords;
	extern char	*__progname;

	/* Crosshair placement */
	int		cpx[] = { 0, 0, 1, 1, 1 };
	int		cpy[] = { 0, 1, 0, 0, 1 };

	if (argc != 2) {
		fprintf(stderr, "usage: %s <device>\n", __progname);
		return 1;
	}

	if ((evfd = open(argv[1], O_RDONLY)) == -1)
		err(1, "open()");

	/* connect to X server */
	if ((display = XOpenDisplay(display_name)) == NULL) {
		fprintf(stderr, "%s: cannot connect to X server %s\n",
			progname, XDisplayName(display_name));
		close(evfd);
		exit(1);
	}
	screen = DefaultScreen(display);
	root = RootWindow(display, screen);

	/* setup window attributes */
	xswa.override_redirect = True;
	xswa.background_pixel = BlackPixel(display, screen);
	xswa.event_mask = ExposureMask | KeyPressMask;
	xswa.cursor = create_empty_cursor();

	/* get screen size from display structure macro */
	width = DisplayWidth(display, screen);
	height = DisplayHeight(display, screen);

	win = XCreateWindow(display, RootWindow(display, screen),
			    0, 0, width, height, 0,
			    CopyFromParent, InputOutput, CopyFromParent,
			    CWOverrideRedirect | CWBackPixel | CWEventMask |
			    CWCursor, &xswa);
	render_init();
	XMapWindow(display, win);
	XGrabKeyboard(display, win, False, GrabModeAsync, GrabModeAsync,
		      CurrentTime);
	XGrabServer(display);

	XClearWindow(display, win);

        if (ioctl(evfd, WSMOUSEIO_GCALIBCOORDS, &wmcoords) < 0)
                err(1, "WSMOUSEIO_GCALIBCOORDS");

        orawmode = wmcoords.samplelen;
        wmcoords.samplelen = 1;

        if (ioctl(evfd, WSMOUSEIO_SCALIBCOORDS, &wmcoords) < 0)
                err(1, "WSMOUSEIO_SCALIBCOORDS");

calib:
	XftDrawRect(draw, &bg, 0, 0, width, height);

	for (i = 0; i < 5; i++) {
		draw_graphics(cpx[i], cpy[i], i);
		XFlush(display);
		get_events(i);
		XftDrawRect(draw, &bg, 0, 0, width, height);
	}

	/* Check if  X and Y should be swapped */
	if (fabs(x[0] - x[1]) > fabs(y[0] - y[1])) {
		wmcoords.swapxy = 1;
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
		fprintf(stderr, "X error (%.2f) too high, try again\n",
			fabs(xerr));
		goto err;
	}
	wmcoords.minx = (int) (b + 0.5);
	wmcoords.maxx = (int) (a * width + b + 0.5);

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
		fprintf(stderr, "Y error (%.2f) too high, try again\n",
			fabs(yerr));
		goto err;
	}
	wmcoords.miny = (int) (b + 0.5);
	wmcoords.maxy = (int) (a * height + b + 0.5);

	XFlush(display);

        wmcoords.samplelen = orawmode;
	wmcoords.resx = width;
	wmcoords.resy = height;

        if (ioctl(evfd, WSMOUSEIO_SCALIBCOORDS, &wmcoords) < 0)
                err(1, "WSMOUSEIO_SCALIBCOORDS");

	printf("mouse.scale=%d,%d,%d,%d,%d,%d,%d\n",
	    wmcoords.minx, wmcoords.maxx,
	    wmcoords.miny, wmcoords.maxy,
	    wmcoords.swapxy,
	    wmcoords.resx, wmcoords.resy);

	return 0;
err:
	draw_text(error_message, &errorColor);
	XFlush(display);
	sleep(2);
	goto calib;
}
