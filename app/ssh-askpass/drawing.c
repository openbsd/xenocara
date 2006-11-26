/* xscreensaver, Copyright (c) 1991-1998 Jamie Zawinski <jwz@netscape.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

/* 1999-Nov-21 Modified by Jim Knoble <jmknoble@jmknoble.cx>
 * Modifications:
 *   - Extracted draw_shaded_rectangle() from xscreensaver source for use
 *     in a separate application.  Constructed separate header file.
 */

#include <X11/Intrinsic.h>

void
draw_shaded_rectangle (Display *dpy, Window window,
		       int x, int y,
		       int width, int height,
		       int thickness,
		       unsigned long top_color,
		       unsigned long bottom_color)
{
  XPoint points[4];
  XGCValues gcv;
  GC gc1, gc2;
  if (thickness == 0) return;

  gcv.foreground = top_color;
  gc1 = XCreateGC (dpy, window, GCForeground, &gcv);
  gcv.foreground = bottom_color;
  gc2 = XCreateGC (dpy, window, GCForeground, &gcv);

  points [0].x = x;
  points [0].y = y;
  points [1].x = x + width;
  points [1].y = y;
  points [2].x = x + width - thickness;
  points [2].y = y + thickness;
  points [3].x = x;
  points [3].y = y + thickness;
  XFillPolygon (dpy, window, gc1, points, 4, Convex, CoordModeOrigin);

  points [0].x = x;
  points [0].y = y + thickness;
  points [1].x = x;
  points [1].y = y + height;
  points [2].x = x + thickness;
  points [2].y = y + height - thickness;
  points [3].x = x + thickness;
  points [3].y = y + thickness;
  XFillPolygon (dpy, window, gc1, points, 4, Convex, CoordModeOrigin);

  points [0].x = x + width;
  points [0].y = y;
  points [1].x = x + width - thickness;
  points [1].y = y + thickness;
  points [2].x = x + width - thickness;
  points [2].y = y + height - thickness;
  points [3].x = x + width;
  points [3].y = y + height - thickness;
  XFillPolygon (dpy, window, gc2, points, 4, Convex, CoordModeOrigin);

  points [0].x = x;
  points [0].y = y + height;
  points [1].x = x + width;
  points [1].y = y + height;
  points [2].x = x + width;
  points [2].y = y + height - thickness;
  points [3].x = x + thickness;
  points [3].y = y + height - thickness;
  XFillPolygon (dpy, window, gc2, points, 4, Convex, CoordModeOrigin);

  XFreeGC (dpy, gc1);
  XFreeGC (dpy, gc2);
}

