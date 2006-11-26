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

#ifndef _DRAWING_H_
#define _DRAWING_H_

#include <X11/Intrinsic.h>

void
draw_shaded_rectangle (Display *dpy, Window window,
		       int x, int y,
		       int width, int height,
		       int thickness,
		       unsigned long top_color,
		       unsigned long bottom_color);

#endif /* _DRAWING_H_ */
