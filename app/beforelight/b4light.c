/*
 * $XConsortium: b4light.c,v 1.3 94/04/17 20:59:38 rws Exp $
 *
Copyright (c) 1992  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.
 *
 * Author:  Keith Packard, MIT X Consortium
 */
/* $XFree86: xc/programs/beforelight/b4light.c,v 3.6tsi Exp $ */

#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/extensions/scrnsaver.h>
#include <X11/Xcms.h>
#include <stdlib.h>
#include <time.h>

#ifndef ZERO
# define ZERO ((Cardinal)0)
#endif


static int  ss_event, ss_error;

static Display *display;
static Window  root, saver;
static int screen;
static int  scr_wid, scr_hei;
static Colormap	cmap;
static GC gc, black_gc, erase_gc;
static int  screen_saved;
static XtAppContext app_con;
static GC   bit_1_gc, bit_0_gc;
static Bool	filled = False;

#define MAX_POINTS  16

typedef  struct _moving {
    int  x, y, dx, dy;
} Moving;

static Moving	p[MAX_POINTS];

#define NUM_HISTORY 32

static XPoint	history[NUM_HISTORY][MAX_POINTS];
static Pixmap	old_pixmaps[NUM_HISTORY];
static unsigned long	old_pixels[NUM_HISTORY];
static int	num_points = 3;
static int	history_head, history_tail;
#define hist_bump(h)	((++(h) == NUM_HISTORY) ? ((h) = 0) : 0)

#define NUM_COLORS  64

static unsigned long	black_pixel;
static unsigned long	pixels[NUM_COLORS];
static int  cur_pen = 0;

static void
AllocateColors (void)
{
    double	angle;
    double	step;
    XcmsColor	cms_color;
    int		i;
    XColor	hard, exact;

    XAllocNamedColor (display, cmap, "black", &hard, &exact);
    black_pixel = hard.pixel;
    step = 360.0 / NUM_COLORS;
    for (i = 0; i < NUM_COLORS; i++) {
	angle = i * step;
	cms_color.spec.TekHVC.H = angle;
	cms_color.spec.TekHVC.V = 75.0;
	cms_color.spec.TekHVC.C = 75.0;
	cms_color.format = XcmsTekHVCFormat;
	XcmsAllocColor (display, cmap, &cms_color, XcmsRGBFormat);
	pixels[i] = cms_color.pixel;
    }
}


static void
StepPen (void)
{
    XSetForeground (display, gc, pixels[cur_pen]);
    cur_pen++;
    if (cur_pen == NUM_COLORS)
	cur_pen = 0;
}

static void
DrawPoints (Drawable draw, GC draw_gc, XPoint *pt, int n)
{
    XPoint  xp[MAX_POINTS + 1];
    int	    i;

    switch (n) {
    case 1:
	XDrawPoint (display, draw, draw_gc, pt->x, pt->y);
	break;
    case 2:
	XDrawLine (display, draw, draw_gc, pt[0].x, pt[0].y, pt[1].x, pt[1].y);
	break;
    default:
	for (i = 0; i < n; i++) {
	    xp[i].x = pt[i].x; xp[i].y = pt[i].y;
	}
	xp[i].x = pt[0].x; xp[i].y = pt[0].y;
	if (filled)
	    XFillPolygon (display, draw, draw_gc, xp, i+1, Complex, CoordModeOrigin);
	else
	    XDrawLines (display, draw, draw_gc, xp, i + 1, CoordModeOrigin);
    }
}

static void
Draw (Moving *mp, int n)
{
    XPoint  xp[MAX_POINTS];
    int	    i;
    for (i = 0; i < n; i++) 
    {
	xp[i].x = mp[i].x; xp[i].y = mp[i].y;
    }
    old_pixels[history_head] = pixels[cur_pen];
    StepPen ();
    DrawPoints (saver, gc, xp, n);
    if (filled)
    {
	XFillRectangle (display, old_pixmaps[history_head], bit_0_gc,
			0, 0, scr_wid, scr_hei);
	DrawPoints (old_pixmaps[history_head], bit_1_gc, xp, n);
	for (i = history_tail; i != history_head; hist_bump(i))
	    DrawPoints (old_pixmaps[i], bit_0_gc, xp, n);
    }
}

static void
Erase (XPoint *pt, int n)
{
    if (filled) {
	XSetForeground (display, erase_gc, black_pixel ^ old_pixels[history_tail]);
	XCopyPlane (display, old_pixmaps[history_tail], saver, erase_gc,
		    0, 0, scr_wid, scr_hei, 0, 0, 1);
    }
    else
	DrawPoints (saver, black_gc, pt, n);
}

#define STEP_MAX    32

static int
RandomStep (void)
{
    return (rand () % STEP_MAX) + 1;
}

static void
StepMoving (Moving *m)
{
    int	maxx, maxy;

    maxx = DisplayWidth (display, screen);
    maxy = DisplayHeight (display, screen);
    m->x += m->dx;
    if (m->x <= 0) {
	m->x = 0;
	m->dx = RandomStep ();
    }
    if (m->x >= maxx) {
	m->x = maxx - 1;
	m->dx = -RandomStep ();
    }
    m->y += m->dy;
    if (m->y <= 0) {
	m->y = 0;
	m->dy = RandomStep ();
    }
    if (m->y >= maxy) {
	m->y = maxy - 1;
	m->dy = -RandomStep ();
    }
}

static void
StepPoints (void)
{
    int	i;

    for (i = 0; i < num_points; i++)
	StepMoving (&p[i]);
    hist_bump(history_head);
    if (history_tail == history_head)
    {
	Erase (history[history_tail], num_points);
	hist_bump(history_tail);
    }
    Draw (p, num_points);
    for (i = 0; i < num_points; i++)
    {
    	history[history_head][i].x = p[i].x;
    	history[history_head][i].y = p[i].y;
    }
}

static void
StartPoints (void)
{
    history_head = history_tail = 0;
}

static void
Timeout (XtPointer closure, XtIntervalId *id)
{
    if (screen_saved)
    {
	StepPoints ();
	(void) XtAppAddTimeOut (app_con, 50, Timeout, NULL);
    }
}

static void 
StartSaver (void)
{
    if (screen_saved)
	return;
    screen_saved = True;
    StartPoints ();
    StepPoints ();
    (void) XtAppAddTimeOut (app_con, 50, Timeout, NULL);
}

static void
StopSaver (void)
{
    if (!screen_saved)
	return;
    screen_saved = False;
}

static int 
ignoreError (Display *dpy, XErrorEvent *error)
{
    return 0;
}

int 
main(int argc, char *argv[])
{
    Widget toplevel;
    XEvent  event;
    XScreenSaverNotifyEvent *sevent;
    XSetWindowAttributes    attr;
    XScreenSaverInfo	    *info;
    unsigned long	    mask;
    Pixmap		    blank_pix;
    XColor		    dummyColor;
    XID			    kill_id;
    Atom		    kill_type;
    int			    i;
    int			    (*oldHandler)(Display*, XErrorEvent*);
    Window 		    r;
    int			    x, y;
    unsigned int	    w, h, b, d;
    Status		    s;

    srand((int)time((time_t *)NULL));

    toplevel = XtAppInitialize (&app_con, "Beforelight", NULL, ZERO,
				&argc, argv, NULL, NULL, ZERO);
    display = XtDisplay (toplevel);
    root = DefaultRootWindow (display);
    screen = DefaultScreen (display);
    scr_wid = DisplayWidth (display, screen);
    scr_hei = DisplayHeight (display, screen);
    if (!XScreenSaverQueryExtension (display, &ss_event, &ss_error))
	exit (1);
    oldHandler = XSetErrorHandler (ignoreError);
    if (XScreenSaverGetRegistered (display, screen, &kill_id, &kill_type)) {
	s = XGetGeometry(display, kill_id, &r, &x, &y, &w, &h, &b, &d);
	if (s == True && r == root && w == 1 && h == 1 && d == 1) {
	    /* Try to clean up existing saver & resources */
	    XKillClient (display, kill_id);
	    XScreenSaverUnregister(display, screen);
	}
    }
    XSync(display, FALSE);
    XSetErrorHandler(oldHandler);
    XScreenSaverSelectInput (display, root, ScreenSaverNotifyMask);
    cmap = DefaultColormap (display, screen);
    AllocateColors();
    blank_pix = XCreatePixmap (display, root, 1, 1, 1);
    XScreenSaverRegister (display, screen, (XID) blank_pix, XA_PIXMAP);
    bit_0_gc = XCreateGC (display, blank_pix, 0, NULL);
    XSetForeground (display, bit_0_gc, 0);
    bit_1_gc = XCreateGC (display, blank_pix, 0, NULL);
    XSetForeground (display, bit_1_gc, ~0);
    XFillRectangle (display, blank_pix, bit_0_gc, 0, 0, 1, 1);
    info = XScreenSaverAllocInfo ();
    XScreenSaverQueryInfo (display, root, info);
    mask = 0;
    attr.colormap = cmap;
    mask |= CWColormap;
    attr.background_pixel = black_pixel;
    mask |= CWBackPixel;
    attr.cursor = XCreatePixmapCursor (display, blank_pix, blank_pix, &dummyColor, &dummyColor, 0, 0);
    mask |= CWCursor;
    XScreenSaverSetAttributes (display, root, 0, 0,
	DisplayWidth (display, screen), DisplayHeight(display, screen), 0,
	CopyFromParent, CopyFromParent, (Visual *)CopyFromParent, mask, &attr);
    XSync (display, False);
    gc = XCreateGC (display, root, 0, NULL);
    black_gc = XCreateGC (display, root, 0, NULL);
    XSetForeground (display, black_gc, black_pixel);
    if (filled)
    {
    	erase_gc = XCreateGC (display, root, 0, NULL);
    	XSetBackground (display, erase_gc, 0);
    	XSetFunction (display, erase_gc, GXxor);
    	XSetGraphicsExposures (display, erase_gc, False);
    	for (i = 0; i < NUM_HISTORY; i++)
	    old_pixmaps[i] = XCreatePixmap (display, root, scr_wid, scr_hei, 1);
    }
    XSetErrorHandler (ignoreError);
    saver = info->window;
    if (info->state == ScreenSaverOn)
    {
	if (info->kind != ScreenSaverExternal) 
	{
	    XResetScreenSaver (display);
	    XActivateScreenSaver (display);
	}
	StartSaver ();
    }
    for (;;) 
    {
	XtAppNextEvent (app_con, &event);
	if (event.type == ss_event) {
	    sevent = (XScreenSaverNotifyEvent *) &event;
	    if (sevent->state == ScreenSaverOn) {
		if (sevent->kind != ScreenSaverExternal) {
	    	    XResetScreenSaver (display);
	    	    XActivateScreenSaver (display);
		} else {
		    StartSaver ();
		}
	    } else if (sevent->state == ScreenSaverOff) {
		StopSaver ();
	    }
	} else {
	    XtDispatchEvent(&event);
	}
    }
}

