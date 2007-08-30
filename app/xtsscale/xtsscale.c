/*      $OpenBSD: xtsscale.c,v 1.2 2007/08/30 19:45:25 matthieu Exp $ */
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

#include <sys/param.h>
#include <sys/ioctl.h>
#include <dev/wscons/wsconsio.h>

#include <err.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <math.h>

#define FONT_NAME		"9x15"

#define Background		cWHITE
#define TouchCross		cBLACK
#define PromptText		cBLACK

#define N_Colors		3

static char     colors[N_Colors][10] =
{"BLACK", "WHITE"};

static unsigned long pixels[N_Colors];

#define cBLACK			(pixels[0])
#define cWHITE			(pixels[1])

/* where the calibration points are placed */
#define SCREEN_DIVIDE	16
#define SCREEN_MAX	0x800
#define M_POINT		(SCREEN_MAX/SCREEN_DIVIDE)
int             MARK_POINT[] = {M_POINT, SCREEN_MAX - 1 - M_POINT};

int             touched = 0;
char           *deviceName;

Display        *display;
int             screen;
GC              gc;
Window          root;
Window          win;
XFontStruct    *font_info;
unsigned int    width, height;	/* window size */
char           *progname;
int             evfd;

int    cx[5], cy[5];
int    x[5], y[5];

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
	XUnloadFont(display, font_info->fid);
	XUngrabServer(display);
	XUngrabKeyboard(display, CurrentTime);
	XFreeGC(display, gc);
	XCloseDisplay(display);
	close(evfd);
}


void
load_font(XFontStruct ** font_info)
{
	char           *fontname = FONT_NAME;

	if ((*font_info = XLoadQueryFont(display, fontname)) == NULL) {
		printf("Cannot open %s font\n", FONT_NAME);
		exit(1);
	}
}


void
draw_point(int x, int y, int width, int size, unsigned long color)
{
	XSetForeground(display, gc, color);
	XSetLineAttributes(display, gc, width, LineSolid,
			   CapRound, JoinRound);
	XDrawLine(display, win, gc, x - size, y, x + size, y);
	XDrawLine(display, win, gc, x, y - size, x, y + size);
}

void
draw_text()
{
	static char    *prompt[] = {
		"    TOUCH SCREEN CALIBRATION",
		"Press on the cross hairs please..."
	};

#define num	(sizeof(prompt) / sizeof(prompt[0]))
	static int      init = 0;
	static int      p_len[num];
	static int      p_width[num];
	static int      p_height;
	static int      p_maxwidth = 0;
	int             i, x, y;
	int             line_height;

	if (!init) {
		for (i = 0; i < num; i++) {
			p_len[i] = strlen(prompt[i]);
			p_width[i] = XTextWidth(font_info, prompt[i], p_len[i]);
			if (p_width[i] > p_maxwidth)
				p_maxwidth = p_width[i];
		}
		p_height = font_info->ascent + font_info->descent;
		init = 1;
	}
	line_height = p_height + 5;
	x = (width - p_maxwidth) / 2;
	y = height / 2 - 6 * line_height;

	XSetForeground(display, gc, PromptText);
	XClearArea(display, win, x - 11, y - 8 - p_height,
		   p_maxwidth + 11 * 2, num * line_height + 8 * 2, False);
	XSetLineAttributes(display, gc, 3, FillSolid,
			   CapRound, JoinRound);

	for (i = 0; i < num; i++) {
		XDrawString(display, win, gc, x, y + i * line_height, prompt[i],
			    p_len[i]);
	}
#undef num
}


void
draw_graphics(int i, int j, int n)
{
	unsigned long   color;

	draw_text();
	color = TouchCross;

	if (n == 2) {
		cx[n] = width / 2;
		cy[n] = height / 2;
	} else {
		cx[n] = (MARK_POINT[i] * width) / SCREEN_MAX;
		cy[n] = (MARK_POINT[j] * height) / SCREEN_MAX;
	}
	draw_point(cx[n], cy[n], width / 200, width / 64, color);
}

void
get_gc(Window win, GC * gc, XFontStruct * font_info)
{
	unsigned long   valuemask = 0;	/* ignore XGCvalues and use defaults */
	XGCValues       values;
	unsigned int    line_width = 5;
	int             line_style = LineSolid;
	int             cap_style = CapRound;
	int             join_style = JoinRound;

	*gc = XCreateGC(display, win, valuemask, &values);

	XSetFont(display, *gc, font_info->fid);

	XSetLineAttributes(display, *gc, line_width, line_style,
			   cap_style, join_style);
}


int
get_color()
{
	int             default_depth;
	Colormap        default_cmap;
	XColor          my_color;
	int             i;

	default_depth = DefaultDepth(display, screen);
	default_cmap = DefaultColormap(display, screen);

	for (i = 0; i < N_Colors; i++) {
		XParseColor(display, default_cmap, colors[i], &my_color);
		XAllocColor(display, default_cmap, &my_color);
		pixels[i] = my_color.pixel;
	}

	return 0;
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

	struct tsscale {
		int             ts_minx;
		int             ts_maxx;
		int             ts_miny;
		int             ts_maxy;
		int             ts_swapxy;
	} ts;

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
	XMapWindow(display, win);
	XGrabKeyboard(display, win, False, GrabModeAsync, GrabModeAsync,
		      CurrentTime);
	XGrabServer(display);
	load_font(&font_info);
	get_gc(win, &gc, font_info);
	get_color();

	XSetWindowBackground(display, win, Background);
	XClearWindow(display, win);

        if (ioctl(evfd, WSMOUSEIO_GCALIBCOORDS, &wmcoords) < 0)
                err(1, "WSMOUSEIO_GCALIBCOORDS");

        orawmode = wmcoords.samplelen;
        wmcoords.samplelen = 1;

        if (ioctl(evfd, WSMOUSEIO_SCALIBCOORDS, &wmcoords) < 0)
                err(1, "WSMOUSEIO_SCALIBCOORDS");

calib:
	for (i = 0; i < 5; i++) {
		draw_graphics(cpx[i], cpy[i], i);
		XFlush(display);
		get_events(i);
		XClearWindow(display, win);
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
	if (fabs(xerr) > (a * width + b) * .01) {
		fprintf(stderr, "X error (%.2f) too high, try again\n",
			fabs(xerr));
		goto calib;
	}
	ts.ts_minx = (int) (b + 0.5);
	ts.ts_maxx = (int) (a * width + b + 0.5);

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
	if (fabs(yerr) > (a * height + b) * 0.01) {
		fprintf(stderr, "Y error (%.2f) too high, try again\n",
			fabs(yerr));
		goto calib;
	}
	ts.ts_miny = (int) (b + 0.5);
	ts.ts_maxy = (int) (a * height + b + 0.5);

	XFlush(display);

        wmcoords.samplelen = orawmode;

        if (ioctl(evfd, WSMOUSEIO_SCALIBCOORDS, &wmcoords) < 0)
                err(1, "WSMOUSEIO_SCALIBCOORDS");

	printf("mouse.scale=%d,%d,%d,%d,%d,%d,%d\n",
	    ts.ts_minx, ts.ts_maxx, ts.ts_miny,
	    ts.ts_maxy, wmcoords.swapxy, width, height);
		 
	return 0;
}
