/* $XTermId: graphics.h,v 1.6 2013/07/10 08:28:55 Ross.Combs Exp $ */

/*
 * Copyright 2013 by Ross Combs
 * Copyright 2013 by Thomas E. Dickey
 *
 *                         All Rights Reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization.
 */

#ifndef included_graphics_h
#define included_graphics_h
/* *INDENT-OFF* */

#include <ptyx.h>

#if OPT_SIXEL_GRAPHICS

typedef unsigned short RegisterNum;

extern void chararea_clear_displayed_graphics(TScreen const *screen, int leftcol, int toprow, int ncols, int nrows);
extern void parse_regis(XtermWidget xw, ANSI *params, char const *string);
extern void parse_sixel(XtermWidget xw, ANSI *params, char const *string);
extern void pixelarea_clear_displayed_graphics(TScreen const *screen, int winx, int winy, int w, int h);
extern void refresh_displayed_graphics(TScreen const *screen, int leftcol, int toprow, int ncols, int nrows);
extern void refresh_modified_displayed_graphics(TScreen const *screen);
extern void reset_displayed_graphics(TScreen const *screen);
extern void scroll_displayed_graphics(int rows);
extern void update_displayed_graphics_color_registers(TScreen const *screen, RegisterNum color, short r, short g, short b);

#else

#define chararea_clear_displayed_graphics(screen, leftcol, toprow, ncols, nrows) /* nothing */
#define parse_regis(xw, params, string) /* nothing */
#define parse_sixel(xw, params, string) /* nothing */
#define pixelarea_clear_displayed_graphics(screen, winx, winy, w, h) /* nothing */
#define refresh_displayed_graphics(screen, leftcol, toprow, ncols, nrows) /* nothing */
#define refresh_modified_displayed_graphics(screen) /* nothing */
#define reset_displayed_graphics(screen) /* nothing */
#define scroll_displayed_graphics(rows) /* nothing */
#define update_displayed_graphics_color_registers(screen, color, r, g, b) /* nothing */

#endif

/* *INDENT-ON* */

#endif /* included_graphics_h */
