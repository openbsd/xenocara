/*

Copyright (c) 1991  X Consortium
Copyright (c) 2023  q3k

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/

/*
 * Eyes.c
 *
 * a widget which follows the mouse around
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

# include <X11/Xos.h>
# include <stdio.h>
# include <X11/IntrinsicP.h>
# include <X11/StringDefs.h>
# include <X11/Xmu/Converters.h>
# include "EyesP.h"
# include <math.h>
# include <X11/extensions/shape.h>
# include <X11/Xlibint.h>
# include <stdlib.h>
# include <X11/extensions/XInput2.h>
# include <assert.h>

#define offset(field) XtOffsetOf(EyesRec, eyes.field)
#define goffset(field) XtOffsetOf(WidgetRec, core.field)

static XtResource resources[] = {
    {(char *) XtNwidth, (char *) XtCWidth, XtRDimension, sizeof(Dimension),
	goffset(width), XtRImmediate, (XtPointer) 150},
    {(char *) XtNheight, (char *) XtCHeight, XtRDimension, sizeof(Dimension),
	goffset(height), XtRImmediate, (XtPointer) 100},
    {(char *) XtNforeground, (char *) XtCForeground, XtRPixel, sizeof(Pixel),
        offset(pixel[PART_PUPIL]), XtRString, (char *) XtDefaultForeground},
    {(char *) XtNbackgroundPixmap, (char *) XtCPixmap, XtRPixmap, sizeof(Pixmap),
     XtOffsetOf(CoreRec,core.background_pixmap),
     XtRImmediate, (XtPointer)None},
    {(char *) XtNoutline, (char *) XtCForeground, XtRPixel, sizeof(Pixel),
        offset(pixel[PART_OUTLINE]), XtRString, (char *) XtDefaultForeground},
    {(char *) XtNcenterColor, (char *) XtCBackground, XtRPixel, sizeof (Pixel),
	offset(pixel[PART_CENTER]), XtRString, (char *) XtDefaultBackground},
    {(char *) XtNreverseVideo, (char *) XtCReverseVideo, XtRBoolean, sizeof (Boolean),
	offset (reverse_video), XtRImmediate, (XtPointer) FALSE},
    {(char *) XtNbackingStore, (char *) XtCBackingStore, (char *) XtRBackingStore, sizeof (int),
    	offset (backing_store), XtRString, (char *) "default"},
    {(char *) XtNshapeWindow, (char *) XtCShapeWindow, XtRBoolean, sizeof (Boolean),
	offset (shape_window), XtRImmediate, (XtPointer) TRUE},
#ifdef XRENDER
    {(char *) XtNrender, (char *) XtCBoolean, XtRBoolean, sizeof(Boolean),
	offset(render), XtRImmediate, (XtPointer) TRUE },
#endif
#ifdef PRESENT
    {(char *) XtNpresent, (char *) XtCBoolean, XtRBoolean, sizeof(Boolean),
     offset(present), XtRImmediate, (XtPointer) TRUE },
#endif
    {(char *) XtNdistance, (char *) XtCBoolean, XtRBoolean, sizeof(Boolean),
	offset(distance), XtRImmediate, (XtPointer) FALSE },
    {(char *) XtNbiblicallyAccurate, (char *) XtCBoolean, XtRBoolean, sizeof(Boolean),
	offset(biblically_accurate), XtRImmediate, (XtPointer) FALSE },
};

#undef offset
#undef goffset

# define EYE_OFFSET	(0.1)	/* padding between eyes */
# define EYE_THICK	(0.175)	/* thickness of eye rim */
# define BALL_DIAM	(0.3)
# define BALL_PAD	(0.175)
# define EYE_DIAM	(2.0 - (EYE_THICK + EYE_OFFSET) * 2)
# define BALL_DIST	((EYE_DIAM - BALL_DIAM) / 2.0 - BALL_PAD)

# define TPOINT_NONE	(-1000)	/* special value meaning "not yet set" */
# define TPointEqual(a, b)  ((a).x == (b).x && (a).y == (b).y)
# define XPointEqual(a, b)  ((a).x == (b).x && (a).y == (b).y)
# define AngleBetween(A, A0, A1) (A0 <= A1 ? A0 <= A && A <= A1 : \
					     A0 <= A || A <= A1)

static int delays[] = { 50, 100, 200, 400, 0 };

static EyeLayout layout_standard[] = {
	{ .x = 0.0, .y = 0.0, },
	{ .x = 2.0, .y = 0.0, },
};

static EyeLayout layout_biblical[] = {
	{ .x = 0.0+0.75, .y = 0.0, },
	{ .x = 1.5+0.75, .y = 0.0, },
	{ .x = 3.0+0.75, .y = 0.0, },

	{ .x = 0.0+0.00, .y = 1.4, },
	{ .x = 1.5+0.00, .y = 1.4, },
	{ .x = 3.0+0.00, .y = 1.4, },
	{ .x = 4.5+0.00, .y = 1.4, },

	{ .x = 0.0+0.75, .y = 2.8, },
	{ .x = 1.5+0.75, .y = 2.8, },
	{ .x = 3.0+0.75, .y = 2.8, },
};

static EyeConfiguration *EyesConfigure(Boolean biblically_accurate)
{
	EyeConfiguration *c = calloc(sizeof(EyeConfiguration), 1);
	assert(c != NULL);

	if (biblically_accurate) {
		c->eyes = layout_biblical;
		c->count = sizeof(layout_biblical) / sizeof(EyeLayout);
	} else {
		c->eyes = layout_standard;
		c->count = sizeof(layout_standard) / sizeof(EyeLayout);
	}

	// Calculate the bounding box of the eyes.
	c->w_min_x = c->eyes[0].x;
	c->w_max_x = c->eyes[0].x;
	c->w_min_y = c->eyes[0].y;
	c->w_max_y = c->eyes[0].y;

	for (int i = 0; i < c->count; i++) {
		EyeLayout *l = &c->eyes[i];
		if (l->x > c->w_max_x) {
			c->w_max_x = l->x;
		}
		if (l->x < c->w_min_x) {
			c->w_min_x = l->x;
		}
		if (l->y > c->w_max_y) {
			c->w_max_y = l->y;
		}
		if (l->y < c->w_min_y) {
			c->w_min_y = l->y;
		}
	}

	// Add half size of eye (2.0) minus padding to each edge.
	c->w_min_x -= (1.0 - EYE_OFFSET);
	c->w_max_x += (1.0 - EYE_OFFSET);
	c->w_min_y -= (1.0 - EYE_OFFSET);
	c->w_max_y += (1.0 - EYE_OFFSET);
	return c;
}

static void ClassInitialize(void)
{
    XtAddConverter( XtRString, XtRBackingStore, XmuCvtStringToBackingStore,
		    NULL, 0 );
}

WidgetClass eyesWidgetClass = (WidgetClass) &eyesClassRec;

#ifdef PRESENT
static void CheckPresent(EyesWidget w) {
    const xcb_query_extension_reply_t 	    *xfixes_ext_reply;
    const xcb_query_extension_reply_t 	    *damage_ext_reply;
    const xcb_query_extension_reply_t 	    *present_ext_reply;
    xcb_xfixes_query_version_cookie_t       xfixes_cookie;
    xcb_xfixes_query_version_reply_t        *xfixes_reply;
    xcb_damage_query_version_cookie_t       damage_cookie;
    xcb_damage_query_version_reply_t        *damage_reply;
    xcb_present_query_version_cookie_t      present_cookie;
    xcb_present_query_version_reply_t       *present_reply;

    if (!w->eyes.present)
	return;

    xcb_prefetch_extension_data(xt_xcb(w), &xcb_xfixes_id);
    xcb_prefetch_extension_data(xt_xcb(w), &xcb_damage_id);
    xcb_prefetch_extension_data(xt_xcb(w), &xcb_present_id);

    xfixes_ext_reply = xcb_get_extension_data(xt_xcb(w), &xcb_xfixes_id);
    damage_ext_reply = xcb_get_extension_data(xt_xcb(w), &xcb_damage_id);
    present_ext_reply = xcb_get_extension_data(xt_xcb(w), &xcb_present_id);
    if (xfixes_ext_reply == NULL || !xfixes_ext_reply->present
	|| damage_ext_reply == NULL || !damage_ext_reply->present
	|| present_ext_reply == NULL || !present_ext_reply->present)
    {
	w->eyes.present = FALSE;
    }

    if (!w->eyes.present)
	return;

    /* Now tell the server which versions of the extensions we support */
    xfixes_cookie = xcb_xfixes_query_version(xt_xcb(w),
					     XCB_XFIXES_MAJOR_VERSION,
					     XCB_XFIXES_MINOR_VERSION);

    damage_cookie = xcb_damage_query_version(xt_xcb(w),
					     XCB_DAMAGE_MAJOR_VERSION,
					     XCB_DAMAGE_MINOR_VERSION);

    present_cookie = xcb_present_query_version(xt_xcb(w),
					       XCB_PRESENT_MAJOR_VERSION,
					       XCB_PRESENT_MINOR_VERSION);

    xfixes_reply = xcb_xfixes_query_version_reply(xt_xcb(w),
						  xfixes_cookie,
						  NULL);
    free(xfixes_reply);

    damage_reply = xcb_damage_query_version_reply(xt_xcb(w),
						  damage_cookie,
						  NULL);
    free(damage_reply);

    present_reply = xcb_present_query_version_reply(xt_xcb(w),
						    present_cookie,
						    NULL);
    free(present_reply);
}

static void MakePresentData(EyesWidget w) {

    if (!w->eyes.present)
        return;

    if (!w->eyes.back_buffer) {
        xcb_create_pixmap(xt_xcb(w),
                          w->core.depth,
                          w->eyes.back_buffer = xcb_generate_id(xt_xcb(w)),
                          XtWindow(w),
                          w->core.width,
                          w->core.height);
    }
    if (!w->eyes.back_damage) {
        xcb_damage_create(xt_xcb(w),
                          w->eyes.back_damage = xcb_generate_id(xt_xcb(w)),
                          w->eyes.back_buffer,
                          XCB_DAMAGE_REPORT_LEVEL_NON_EMPTY);
        xcb_xfixes_create_region(xt_xcb(w),
                                 w->eyes.back_region = xcb_generate_id(xt_xcb(w)),
                                 0, NULL);
    }
}

static void UpdatePresent(EyesWidget w) {
    if (w->eyes.back_buffer) {
        xcb_damage_subtract(xt_xcb(w),
                            w->eyes.back_damage,
                            None,
                            w->eyes.back_region);
        xcb_present_pixmap(xt_xcb(w),
                           XtWindow(w),
                           w->eyes.back_buffer,
                           0,
                           None,
                           w->eyes.back_region,
                           0, 0,
                           None,
			   None,
			   None,
			   0,
			   0, 1, 0,
			   0, NULL);
    }
}

#endif

#ifdef PRESENT
#define EyesDrawable(w) (w->eyes.back_buffer ? w->eyes.back_buffer : XtWindow(w))
#else
#define EyesDrawable(w) XtWindow(w)
#endif

static void draw_it_core(EyesWidget w);

static void EyesGeneric(Widget w, XtPointer closure, XEvent *event, Boolean *continue_to_dispatch)
{
        draw_it_core((EyesWidget) w);
}

struct root_listen_list {
    struct root_listen_list *next;
    Widget      widget;
};

static struct root_listen_list *root_listen_list;

static Boolean xi2_dispatcher(XEvent *event) {
    struct root_listen_list *rll;
    Boolean was_dispatched = False;

    for (rll = root_listen_list; rll; rll = rll->next) {
        if (XtDisplay(rll->widget) == event->xany.display) {
            XtDispatchEventToWidget(rll->widget, event);
            was_dispatched = True;
        }
    }
    return was_dispatched;
}

static void select_xi2_events(Widget w)
{
    XIEventMask evmasks[1];
    unsigned char mask1[(XI_LASTEVENT + 7)/8];

    memset(mask1, 0, sizeof(mask1));

    /* select for button and key events from all master devices */
    XISetMask(mask1, XI_RawMotion);

    evmasks[0].deviceid = XIAllMasterDevices;
    evmasks[0].mask_len = sizeof(mask1);
    evmasks[0].mask = mask1;

    XISelectEvents(XtDisplay(w),
                   RootWindowOfScreen(XtScreen(w)),
                   evmasks, 1);
    XtSetEventDispatcher(XtDisplay(w),
                         GenericEvent,
                         xi2_dispatcher);
}

static Boolean xi2_add_root_listener(Widget widget)
{
    struct root_listen_list *rll = malloc (sizeof (struct root_listen_list));

    if (!rll)
        return False;
    rll->widget = widget;
    rll->next = root_listen_list;
    if (!root_listen_list)
            select_xi2_events(widget);
    root_listen_list = rll;
    XtInsertEventTypeHandler(widget, GenericEvent, NULL, EyesGeneric, NULL, XtListHead);
    return True;
}

static void xi2_remove_root_listener(Widget widget)
{
    struct root_listen_list *rll, **prev;

    for (prev = &root_listen_list; (rll = *prev) != NULL; prev = &rll->next) {
        if (rll->widget == widget) {
            *prev = rll->next;
            free(rll);
            break;
        }
    }
}

/* Return 1 if XI2 is available, 0 otherwise */
static int has_xi2(Display *dpy)
{
    int major, minor;
    int rc;

    /* We need at least XI 2.0 */
    major = 2;
    minor = 0;

    rc = XIQueryVersion(dpy, &major, &minor);
    if (rc == BadRequest) {
	return 0;
    } else if (rc != Success) {
        return 0;
    }
    return 1;
}


/* ARGSUSED */
static void Initialize (
    Widget greq,
    Widget gnew,
    ArgList args,
    Cardinal *num_args)
{
    EyesWidget w = (EyesWidget)gnew;
    XtGCMask	valuemask;
    XGCValues	myXGCV;
    int shape_event_base, shape_error_base;
#ifdef XRENDER
    enum EyesPart i;
#endif

    EyeConfiguration *config = EyesConfigure(w->eyes.biblically_accurate);
    TPoint *pupils = calloc(sizeof(TPoint), config->count);
    assert(pupils != NULL);
    for (int j = 0; j < config->count; j++) {
        pupils[j].x = TPOINT_NONE;
        pupils[j].y = TPOINT_NONE;
    }
    w->eyes.configuration = config;
    w->eyes.pupils = pupils;


    /*
     * set the colors if reverse video; these are the colors used:
     *
     *     background - paper		white
     *     foreground - text, ticks	black
     *     border - border		black (foreground)
     *
     * This doesn't completely work since the parent has already made up a
     * border.  Sigh.
     */
    if (w->eyes.reverse_video) {
	Pixel fg = w->eyes.pixel[PART_PUPIL];
	Pixel bg = w->core.background_pixel;

	if (w->core.border_pixel == fg)
 	    w->core.border_pixel = bg;
	if (w->eyes.pixel[PART_OUTLINE] == fg)
	    w->eyes.pixel[PART_OUTLINE] = bg;
	if (w->eyes.pixel[PART_CENTER] == bg)
	    w->eyes.pixel[PART_CENTER] = fg;
	w->eyes.pixel[PART_PUPIL] = bg;
	w->core.background_pixel = fg;
    }

    myXGCV.foreground = w->eyes.pixel[PART_PUPIL];
    myXGCV.background = w->core.background_pixel;
    valuemask = GCForeground | GCBackground;
    w->eyes.gc[PART_PUPIL] = XtGetGC(gnew, valuemask, &myXGCV);

    myXGCV.foreground = w->eyes.pixel[PART_OUTLINE];
    valuemask = GCForeground | GCBackground;
    w->eyes.gc[PART_OUTLINE] = XtGetGC(gnew, valuemask, &myXGCV);

    myXGCV.foreground = w->eyes.pixel[PART_CENTER];
    myXGCV.background = w->eyes.pixel[PART_PUPIL];
    valuemask = GCForeground | GCBackground;
    w->eyes.gc[PART_CENTER] = XtGetGC(gnew, valuemask, &myXGCV);

    w->eyes.update = 0;
    /* wait for Realize to add the timeout */
    w->eyes.interval_id = 0;

    w->eyes.mouse.x = w->eyes.mouse.y = TPOINT_NONE;

    if (w->eyes.shape_window && !XShapeQueryExtension (XtDisplay (w),
						       &shape_event_base,
						       &shape_error_base))
	w->eyes.shape_window = False;
    w->eyes.shape_mask = 0;
    w->eyes.gc[PART_SHAPE] = NULL;

    w->eyes.has_xi2 = has_xi2(XtDisplay(w));

#ifdef XRENDER
    for (i = 0; i < PART_SHAPE; i ++) {
	XColor c;
	XRenderColor rc;

	c.pixel = w->eyes.pixel[i];
	XQueryColor(XtDisplay (w), w->core.colormap, &c);

	rc.red = c.red;
	rc.green = c.green;
	rc.blue = c.blue;
	rc.alpha = -1;
	w->eyes.fill[i] = XRenderCreateSolidFill(XtDisplay (w), &rc);
    }
#endif
#ifdef PRESENT
    w->eyes.back_buffer = None;
    w->eyes.back_damage = None;
    CheckPresent(w);
#endif
}

static void
drawEllipse(EyesWidget w, enum EyesPart part,
	    double centerx, double centery,
	    double oldx, double oldy,
	    double diam)
{
    const TRectangle tpos = {
	centerx - diam/2.0,
	centery - diam/2.0,
	diam, diam };
    TRectangle pos;
    Trectangle(&w->eyes.t, &tpos, &pos);

    if (part == PART_CLEAR) {
	XFillRectangle(XtDisplay(w), EyesDrawable(w),
		       w->eyes.gc[PART_CENTER],
		       (int)pos.x, (int)pos.y,
		       (int)pos.width+2, (int)pos.height+2);
	return;
    }
#ifdef XRENDER
    if (w->eyes.render && part != PART_SHAPE && (!w->eyes.shape_window ||
						 part != PART_OUTLINE) &&
	w->eyes.picture) {
	int n, i;
	double hd, c, s, sx, sy, x, y, px, py;
	XPointDouble *p;

	pos.x = pos.x + pos.width/2.0;
	pos.y = pos.y + pos.height/2.0;

	/* determine number of segments to draw */
	hd = hypot(pos.width, pos.height)/2;
	n = (M_PI / acos(hd/(hd+1.0))) + 0.5;
	if (n < 2) n = 2;

	c = cos(M_PI/n);
	s = sin(M_PI/n);
	sx = -(pos.width*s)/pos.height;
	sy = (pos.height*s)/pos.width;

	n *= 2;
	p = Xmalloc(sizeof(*p)*n);
	if (!p)
	    return;
	x = 0;
	y = pos.height/2.0;
	for (i = 0; i < n; i ++)
	{
	    p[i].x = x + pos.x;
	    p[i].y = y + pos.y;
	    px = x;
	    py = y;
	    x = c*px + sx*py;
	    y = c*py + sy*px;
	}

	if (oldx != TPOINT_NONE || oldy != TPOINT_NONE)
	    drawEllipse(w, PART_CLEAR, oldx, oldy,
			TPOINT_NONE, TPOINT_NONE, diam);

	XRenderCompositeDoublePoly(XtDisplay(w), PictOpOver,
				   w->eyes.fill[part], w->eyes.picture,
				   XRenderFindStandardFormat(XtDisplay(w),
							     PictStandardA8),
				   0, 0, 0, 0, p, n, 0);

	Xfree(p);
	return;
    }
#endif
    if (oldx != TPOINT_NONE || oldy != TPOINT_NONE)
	drawEllipse(w, PART_CLEAR, oldx, oldy,
		    TPOINT_NONE, TPOINT_NONE, diam);

    XFillArc(XtDisplay(w),
	     part == PART_SHAPE ? w->eyes.shape_mask : EyesDrawable(w),
	     w->eyes.gc[part],
	     (int)(pos.x + 0.5), (int)(pos.y + 0.5),
	     (int)(pos.width + 0.0), (int)(pos.height + 0.0),
	     90*64, 360*64);
}


static void
eyeLiner(EyesWidget	w,
	 Boolean	draw,
	 int		num)
{
    EyeLayout *l = &w->eyes.configuration->eyes[num];
    drawEllipse(w, draw ? PART_OUTLINE : PART_SHAPE,
		l->x, l->y,
		TPOINT_NONE, TPOINT_NONE,
		EYE_DIAM + 2.0*EYE_THICK);
    if (draw) {
	drawEllipse(w, PART_CENTER, l->x, l->y,
		    TPOINT_NONE, TPOINT_NONE,
		    EYE_DIAM);
    }
}

static TPoint computePupil (
    EyeLayout *layout,
    TPoint	mouse,
    const TRectangle *screen)
{
	double	cx, cy;
	double	dist;
	double	angle;
	double	dx, dy;
	double	cosa, sina;
	TPoint	ret;

	cx = layout->x; dx = mouse.x - cx;
	cy = layout->y; dy = mouse.y - cy;
	if (dx == 0 && dy == 0);
	else {
		angle = atan2 ((double) dy, (double) dx);
		cosa = cos (angle);
		sina = sin (angle);
		dist = BALL_DIST;
		if (screen)
		{
		    /* use distance mapping */
		    double x0, y0, x1, y1;
		    double a[4];
		    x0 = screen->x - cx;
		    y0 = screen->y - cy;
		    x1 = x0 + screen->width;
		    y1 = y0 + screen->height;
		    a[0] = atan2(y0, x0);
		    a[1] = atan2(y1, x0);
		    a[2] = atan2(y1, x1);
		    a[3] = atan2(y0, x1);
		    if (AngleBetween(angle, a[0], a[1]))
		    {
			/* left */
			dist *= dx / x0;
		    }
		    else if (AngleBetween(angle, a[1], a[2]))
		    {
			/* bottom */
			dist *= dy / y1;
		    }
		    else if (AngleBetween(angle, a[2], a[3]))
		    {
			/* right */
			dist *= dx / x1;
		    }
		    else if (AngleBetween(angle, a[3], a[0]))
		    {
			/* top */
			dist *= dy / y0;
		    }
		    if (dist > BALL_DIST)
			dist = BALL_DIST;
		}
		if (dist > hypot ((double) dx, (double) dy)) {
			cx += dx;
			cy += dy;
		} else {
			cx += dist * cosa;
			cy += dist * sina;
		}
	}
	ret.x = cx;
	ret.y = cy;
	return ret;
}

static void computePupils (
    EyesWidget	w,
    TPoint	mouse,
    TPoint  *pupils)
{
    TRectangle screen, *sp = NULL;
    if (w->eyes.distance) {
	Window r, cw;
	int x, y;
	r = RootWindowOfScreen(w->core.screen);
	XTranslateCoordinates(XtDisplay(w), XtWindow(w), r, 0, 0, &x, &y, &cw);
	screen.x = Tx(-x, -y, &w->eyes.t);
	screen.y = Ty(-x, -y, &w->eyes.t);
	screen.width  = Twidth (w->core.screen->width, w->core.screen->height,
				&w->eyes.t);
	screen.height = Theight(w->core.screen->width, w->core.screen->height,
				&w->eyes.t);
	sp = &screen;
    }
    for (int i = 0; i < w->eyes.configuration->count; i++) {
        pupils[i] = computePupil(&w->eyes.configuration->eyes[i], mouse, sp);
    }
}

static void
eyeBall(EyesWidget	w,
	Boolean draw,
	TPoint	*old,
	int	num)
{
    //printf("eyeBall(_, %d, %p, %d)\n", draw, old, num);
    drawEllipse(w, draw ? PART_PUPIL : PART_CLEAR,
		w->eyes.pupils[num].x, w->eyes.pupils[num].y,
		old ? old->x : TPOINT_NONE, old ? old->y : TPOINT_NONE,
		BALL_DIAM);
}

static void repaint_window (EyesWidget w)
{
	if (XtIsRealized ((Widget) w)) {
#ifdef PRESENT
                MakePresentData(w);
#endif
		for (int i = 0; i < w->eyes.configuration->count; i++) {
			eyeLiner (w, TRUE, i);
		}
		computePupils (w, w->eyes.mouse, w->eyes.pupils);
		for (int i = 0; i < w->eyes.configuration->count; i++) {
			eyeBall (w, TRUE, NULL, i);
		}
#ifdef PRESENT
                UpdatePresent(w);
#endif
	}
}

static void
drawEye(EyesWidget w, TPoint newpupil, int num)
{
    XPoint		xnewpupil, xpupil;

    xpupil.x = Xx(w->eyes.pupils[num].x, w->eyes.pupils[num].y, &w->eyes.t);
    xpupil.y = Xy(w->eyes.pupils[num].x, w->eyes.pupils[num].y, &w->eyes.t);
    xnewpupil.x = Xx(newpupil.x, newpupil.y, &w->eyes.t);
    xnewpupil.y = Xy(newpupil.x, newpupil.y, &w->eyes.t);
    if (
#ifdef XRENDER
	w->eyes.picture ? !TPointEqual(w->eyes.pupils[num], newpupil) :
#endif
	!XPointEqual(xpupil, xnewpupil)) {
	TPoint oldpupil = w->eyes.pupils[num];
	w->eyes.pupils[num] = newpupil;
	eyeBall (w, TRUE, &oldpupil, num);
    }
}

static void
drawEyes(EyesWidget w, TPoint mouse)
{
    int			num;
    TPoint newpupils[w->eyes.configuration->count];

#ifdef PRESENT
    MakePresentData(w);
#endif
    if (TPointEqual (mouse, w->eyes.mouse)) {
	if (delays[w->eyes.update + 1] != 0)
	    ++w->eyes.update;
	return;
    }
    computePupils (w, mouse, newpupils);
    for (num = 0; num < w->eyes.configuration->count; num++) {
        drawEye(w, newpupils[num], num);
    }

    w->eyes.mouse = mouse;
    w->eyes.update = 0;
#ifdef PRESENT
    UpdatePresent(w);
#endif
}

static void draw_it_core(EyesWidget w)
{
    Window		rep_root, rep_child;
    int			rep_rootx, rep_rooty;
    unsigned int	rep_mask;
    int			dx, dy;
    TPoint		mouse;
    Display		*dpy = XtDisplay (w);
    Window		win = XtWindow (w);

    XQueryPointer (dpy, win, &rep_root, &rep_child,
	    &rep_rootx, &rep_rooty, &dx, &dy, &rep_mask);
    mouse.x = Tx(dx, dy, &w->eyes.t);
    mouse.y = Ty(dx, dy, &w->eyes.t);

    drawEyes(w, mouse);
}

/* ARGSUSED */
static void draw_it (
     XtPointer client_data,
     XtIntervalId *id)		/* unused */
{
        EyesWidget	w = (EyesWidget)client_data;

	if (XtIsRealized((Widget)w)) {
	        draw_it_core(w);
	}
        if (!w->eyes.has_xi2) {
                w->eyes.interval_id =
                        XtAppAddTimeOut(XtWidgetToApplicationContext((Widget) w),
                                        delays[w->eyes.update], draw_it, (XtPointer)w);
        }
} /* draw_it */

static void Resize (Widget gw)
{
    EyesWidget	w = (EyesWidget) gw;
    XGCValues	xgcv;
    Widget	parent;
    Display	*dpy = XtDisplay (w);
    int		x, y;

    if (XtIsRealized (gw))
    {
    	SetTransform (&w->eyes.t,
		    	0, w->core.width,
 		    	w->core.height, 0,
		    	w->eyes.configuration->w_min_x,
		    	w->eyes.configuration->w_max_x,
		    	w->eyes.configuration->w_min_y,
		    	w->eyes.configuration->w_max_y);
#ifdef PRESENT
        if (w->eyes.back_buffer) {
                xcb_free_pixmap(xt_xcb(w),
                                w->eyes.back_buffer);
                w->eyes.back_buffer = None;
                xcb_damage_destroy(xt_xcb(w),
                                   w->eyes.back_damage);
                w->eyes.back_damage = None;
        }
        MakePresentData(w);
#endif
        if (EyesDrawable(w) == XtWindow(w))
                XClearWindow (dpy, XtWindow (w));

#ifdef XRENDER
	if (w->eyes.picture) {
	    XRenderFreePicture(dpy, w->eyes.picture);
	    w->eyes.picture = 0;
	}
#endif
    	if (w->eyes.shape_window) {
	    w->eyes.shape_mask = XCreatePixmap (dpy, XtWindow (w),
	    	    w->core.width, w->core.height, 1);
	    if (!w->eyes.gc[PART_SHAPE])
		w->eyes.gc[PART_SHAPE] = XCreateGC (dpy, w->eyes.shape_mask,
						    0, &xgcv);
	    XSetForeground (dpy, w->eyes.gc[PART_SHAPE], 0);
	    XFillRectangle (dpy, w->eyes.shape_mask, w->eyes.gc[PART_SHAPE],
			    0, 0, w->core.width, w->core.height);
	    XSetForeground (dpy, w->eyes.gc[PART_SHAPE], 1);
	    for (int i = 0; i < w->eyes.configuration->count; i++) {
	        eyeLiner (w, FALSE, i);
	    }
	    x = y = 0;
	    for (parent = (Widget) w; XtParent (parent); parent = XtParent (parent)) {
	    	x += parent->core.x + parent->core.border_width;
	    	x += parent->core.y + parent->core.border_width;
	    }
    	    XShapeCombineMask (XtDisplay (parent), XtWindow (parent), ShapeBounding,
		       	       x, y, w->eyes.shape_mask, ShapeSet);
	    XFreePixmap (dpy, w->eyes.shape_mask);
    	}
#ifdef XRENDER
	if (w->eyes.render) {
	    XRenderPictureAttributes pa;
	    XRenderPictFormat *pf;
	    pf = XRenderFindVisualFormat(dpy,
					 DefaultVisualOfScreen(w->core.screen));
	    if (pf)
		w->eyes.picture = XRenderCreatePicture(dpy, EyesDrawable (w),
						       pf, 0, &pa);
	}
#endif
    }
}

static void Realize (
     Widget gw,
     XtValueMask *valueMask,
     XSetWindowAttributes *attrs)
{
    EyesWidget	w = (EyesWidget)gw;

    if (w->eyes.backing_store != Always + WhenMapped + NotUseful) {
     	attrs->backing_store = w->eyes.backing_store;
	*valueMask |= CWBackingStore;
    }
    XtCreateWindow( gw, (unsigned)InputOutput, (Visual *)CopyFromParent,
		     *valueMask, attrs );
    Resize (gw);

    if (w->eyes.has_xi2)
            xi2_add_root_listener(gw);
    else
            w->eyes.interval_id =
                    XtAppAddTimeOut(XtWidgetToApplicationContext(gw),
                                    delays[w->eyes.update], draw_it, (XtPointer)gw);
}

static void Destroy (Widget gw)
{
     EyesWidget w = (EyesWidget)gw;
     int i;

     if (w->eyes.interval_id)
	XtRemoveTimeOut (w->eyes.interval_id);
     for (i = 0; i < PART_MAX; i ++)
	     XtReleaseGC(gw, w->eyes.gc[i]);
     xi2_remove_root_listener(gw);
#ifdef XRENDER
     if (w->eyes.picture)
	     XRenderFreePicture (XtDisplay(w), w->eyes.picture);
#endif
}

/* ARGSUSED */
static void Redisplay(
     Widget gw,
     XEvent *event,
     Region region)
{
    EyesWidget	w;

    w = (EyesWidget) gw;
    for (int i = 0; i < w->eyes.configuration->count; i++) {
        w->eyes.pupils[i].x = TPOINT_NONE;
        w->eyes.pupils[i].y = TPOINT_NONE;
    }
    (void) repaint_window ((EyesWidget)gw);
}

/* ARGSUSED */
static Boolean SetValues (
    Widget current,
    Widget request,
    Widget new,
    ArgList args,
    Cardinal *num_args)
{
    return( FALSE );
}

EyesClassRec eyesClassRec = {
    { /* core fields */
    /* superclass		*/	&widgetClassRec,
    /* class_name		*/	(char *) "Eyes",
    /* size			*/	sizeof(EyesRec),
    /* class_initialize		*/	ClassInitialize,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	Realize,
    /* actions			*/	NULL,
    /* num_actions		*/	0,
    /* resources		*/	resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	Destroy,
    /* resize			*/	Resize,
    /* expose			*/	Redisplay,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	NULL,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	NULL,
    /* query_geometry		*/	XtInheritQueryGeometry,
    }
};
