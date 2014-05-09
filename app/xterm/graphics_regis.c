/* $XTermId: graphics_regis.c,v 1.23 2014/05/03 12:44:53 tom Exp $ */

/*
 * Copyright 2014 by Ross Combs
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

#include <xterm.h>

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include <data.h>
#include <VTparse.h>
#include <ptyx.h>

#include <assert.h>
#include <graphics.h>
#include <graphics_regis.h>

/* get rid of shadowing warnings (we will not draw Bessel functions) */
#define y1 my_y1
#define y0 my_y0

#undef DEBUG_BEZIER
#undef DEBUG_SPLINE_SEGMENTS
#undef DEBUG_SPLINE_POINTS
#undef DEBUG_SPLINE_WITH_ROTATION
#undef DEBUG_SPLINE_WITH_OVERDRAW

#define ITERATIONS_BEFORE_REFRESH 100U
/* *INDENT-OFF* */
typedef struct RegisWriteControls {
    unsigned        pv_multiplier;
    unsigned        pattern;
    unsigned        pattern_multiplier;
    unsigned        invert_pattern;
    unsigned        plane_mask;
    unsigned        write_style;
    RegisterNum     foreground;
    unsigned        shading_enabled;
    char            shading_character;
    int             shading_reference;
    unsigned        shading_reference_dim;
} RegisWriteControls;

typedef struct RegisDataFragment {
    char const     *start;
    unsigned        pos;
    unsigned        len;
} RegisDataFragment;

typedef enum RegisParseLevel {
    INPUT,
    OPTIONSET
} RegisParseLevel;
/* *INDENT-ON* */

#define CURVE_POSITION_ARC_EDGE     0U
#define CURVE_POSITION_ARC_CENTER   1U
#define CURVE_POSITION_OPEN_CURVE   2U
#define CURVE_POSITION_CLOSED_CURVE 3U

#define MAX_INPUT_CURVE_POINTS 16U
#define MAX_CURVE_POINTS (MAX_INPUT_CURVE_POINTS + 4U)

typedef struct RegisParseState {
    RegisDataFragment input;
    RegisDataFragment optionset;
    char *temp;
    unsigned templen;
    RegisParseLevel level;
    char command;
    char option;
    /* curve options */
    int curve_mode;
    int arclen;
    int x_points[MAX_CURVE_POINTS];
    int y_points[MAX_CURVE_POINTS];
    unsigned num_points;
} RegisParseState;

typedef struct RegisGraphicsContext {
    Graphic *graphic;
    int terminal_id;
    unsigned all_planes;
    RegisterNum background;
    RegisWriteControls persistent_write_controls;
    RegisWriteControls temporary_write_controls;
    int graphics_output_cursor_x;
    int graphics_output_cursor_y;
    unsigned pattern_count;
    unsigned pattern_bit;
} RegisGraphicsContext;

#define MAX_PATTERN_BITS 8U

#define WRITE_STYLE_OVERLAY 1U
#define WRITE_STYLE_REPLACE 2U
#define WRITE_STYLE_COMPLEMENT 3U
#define WRITE_STYLE_ERASE 4U

#define WRITE_SHADING_REF_Y 0U
#define WRITE_SHADING_REF_X 1U

#define ROT_LEFT_N(V, N) ( (((V) << ((N) & 3U )) & 255U) | ((V) >> (8U - ((N) & 3U))) )
#define ROT_LEFT(V) ( (((V) << 1U) & 255U) | ((V) >> 7U) )

static int
ifloor(double d)
{
    double dl = floor(d);
    return (int) dl;
}

static int
isqrt(double d)
{
    double dl = sqrt(d);
    return (int) dl;
}

static void
draw_regis_pixel(RegisGraphicsContext *context, int x, int y)
{
    unsigned color = 0;

    switch (context->temporary_write_controls.write_style) {
    case WRITE_STYLE_OVERLAY:
	/* update pixels with foreground when pattern is 1, unchanged when pattern is 0 */
	if (!(context->temporary_write_controls.pattern & context->pattern_bit)) {
	    return;
	}

	if (context->temporary_write_controls.invert_pattern) {
	    color = context->background;
	} else {
	    color = context->temporary_write_controls.foreground;
	}
	break;

    case WRITE_STYLE_REPLACE:
	/* update pixels with foreground when pattern is 1, background when pattern is 0 */
	{
	    unsigned fg, bg;

	    if (context->temporary_write_controls.invert_pattern) {
		fg = context->background;
		bg = context->temporary_write_controls.foreground;
	    } else {
		fg = context->temporary_write_controls.foreground;
		bg = context->background;
	    }
	    color = ((context->temporary_write_controls.pattern &
		      context->pattern_bit)
		     ? fg
		     : bg);
	}
	break;

    case WRITE_STYLE_COMPLEMENT:
	/* update pixels with background when pattern is 1, unchanged when pattern is 0 */
	color = read_pixel(context->graphic, x, y);
	if (color == COLOR_HOLE)
	    color = context->background;
	color = color ^ context->all_planes;
	break;

    case WRITE_STYLE_ERASE:
	/* update pixels with foreground */
	if (context->temporary_write_controls.invert_pattern) {
	    color = context->temporary_write_controls.foreground;
	} else {
	    color = context->background;
	}
	break;
    }

    draw_solid_pixel(context->graphic, x, y, color);
}

static void
fill_to_pixel(RegisGraphicsContext *context, int x, int y)
{
    unsigned dim = context->temporary_write_controls.shading_reference_dim;
    int ref = context->temporary_write_controls.shading_reference;

    if (dim == WRITE_SHADING_REF_X) {
	int delta = x > ref ? 1 : -1;
	int curr_x;

	context->pattern_bit = 1U << (((unsigned) y) & 7U);
	for (curr_x = ref; curr_x != x + delta; curr_x += delta) {
	    draw_regis_pixel(context, curr_x, y);
	}
    } else {
	int delta = y > ref ? 1 : -1;
	int curr_y;

	for (curr_y = ref; curr_y != y + delta; curr_y += delta) {
	    context->pattern_bit = 1U << (((unsigned) curr_y) & 7U);
	    draw_regis_pixel(context, x, curr_y);
	}
    }
}

static void
draw_patterned_pixel(RegisGraphicsContext *context, int x, int y)
{
    if (context->temporary_write_controls.shading_enabled) {
	if (context->temporary_write_controls.shading_character != '\0') {
	    /* FIXME: handle character fills */
	    TRACE(("pixel shaded with character\n"));
	} else {
	    fill_to_pixel(context, x, y);
	}
    } else {
	if (context->pattern_count >= context->temporary_write_controls.pattern_multiplier) {
	    context->pattern_count = 0U;
	    context->pattern_bit = ROT_LEFT(context->pattern_bit);
	}
	context->pattern_count++;

	draw_regis_pixel(context, x, y);
    }
}

static void
draw_patterned_line(RegisGraphicsContext *context, int x1, int y1, int x2, int y2)
{
    int x, y;
    int dx, dy;
    int dir, diff;

    dx = abs(x1 - x2);
    dy = abs(y1 - y2);

    if (dx > dy) {
	if (x1 > x2) {
	    int tmp;
	    EXCHANGE(x1, x2, tmp);
	    EXCHANGE(y1, y2, tmp);
	}
	if (y1 < y2)
	    dir = 1;
	else if (y1 > y2)
	    dir = -1;
	else
	    dir = 0;

	diff = 0;
	y = y1;
	for (x = x1; x <= x2; x++) {
	    if (diff >= dx) {
		diff -= dx;
		y += dir;
	    }
	    diff += dy;
	    draw_patterned_pixel(context, x, y);
	}
    } else {
	if (y1 > y2) {
	    int tmp;
	    EXCHANGE(y1, y2, tmp);
	    EXCHANGE(x1, x2, tmp);
	}
	if (x1 < x2)
	    dir = 1;
	else if (x1 > x2)
	    dir = -1;
	else
	    dir = 0;

	diff = 0;
	x = x1;
	for (y = y1; y <= y2; y++) {
	    if (diff >= dy) {
		diff -= dy;
		x += dir;
	    }
	    diff += dx;
	    draw_patterned_pixel(context, x, y);
	}
    }
}

typedef struct {
    int dxx;
    int dxy;
    int dyx;
    int dyy;
} quadmap_coords;

static void
draw_patterned_arc(RegisGraphicsContext *context,
		   int cx, int cy,
		   int ex, int ey,
		   int a_start, int a_length)
{
    const double third = hypot((double) (cx - ex), (double) (cy - ey));
    const int radius = (int) third;
    const int ra = radius;
    const int rb = radius;
    const quadmap_coords neg_quadmap[4] =
    {
	{-1, 0, 0, +1},
	{0, -1, -1, 0},
	{+1, 0, 0, -1},
	{0, +1, +1, 0},
    };
    const quadmap_coords pos_quadmap[4] =
    {
	{-1, 0, 0, -1},
	{0, -1, +1, 0},
	{+1, 0, 0, +1},
	{0, +1, -1, 0},
    };
    const quadmap_coords *quadmap;
    int total_points;
    int points_start, points_stop;
    int points;
    int iterations;
    int quad;
    long rx, ry;
    long dx, dy;
    long e2;
    long error;

    if (a_length == 0)
	return;
    if (a_length > 0) {
	quadmap = pos_quadmap;
    } else {
	quadmap = neg_quadmap;
	if (a_start != 0)
	    a_start = 360 - a_start;
    }

    rx = -ra;
    ry = 0;
    e2 = rb;
    dx = (2 * rx + 1) * e2 * e2;
    dy = rx * rx;
    error = dx + dy;
    total_points = 0;
    do {
	total_points += 4;
	e2 = 2 * error;
	if (e2 >= dx) {
	    rx++;
	    dx += 2 * rb * rb;
	    error += dx;
	}
	if (e2 <= dy) {
	    ry++;
	    dy += 2 * ra * ra;
	    error += dy;
	}
    }
    while (rx <= 0);
    points_start = (total_points * a_start) / 360;
    points_stop = (total_points * a_start +
		   total_points * abs(a_length) + 359) / 360;
    TRACE(("drawing arc with %d points from %d angle for %d degrees (from point %d to %d)\n",
	   total_points, a_start, a_length, points_start, points_stop));

    points = 0;
    for (iterations = 0; iterations < 8; iterations++) {
	quad = iterations & 0x3;

	rx = -ra;
	ry = 0;
	e2 = rb;
	dx = (2 * rx + 1) * e2 * e2;
	dy = rx * rx;
	error = dx + dy;
	do {
	    if (points >= points_start && points <= points_stop) {
		draw_patterned_pixel(context,
				     (int) (cx +
					    quadmap[quad].dxx * rx +
					    quadmap[quad].dxy * ry),
				     (int) (cy +
					    quadmap[quad].dyx * rx +
					    quadmap[quad].dyy * ry));
	    }
	    points++;

	    e2 = 2 * error;
	    if (e2 >= dx) {
		rx++;
		dx += 2 * rb * rb;
		error += dx;
	    }
	    if (e2 <= dy) {
		ry++;
		dy += 2 * ra * ra;
		error += dy;
	    }
	}
	while (rx <= 0);
    }
}

/*
 * The plot* functions are based on optimized rasterization primitves written by Zingl Alois.
 * See http://members.chello.at/easyfilter/bresenham.html
 */

/*
 * FIXME:
 * This is a terrible temporary hack.  The plot functions below can be adapted
 * to work like the other rasterization functions but there's no point in doing
 * that until we know we don't have to write something completely different.
 */
static RegisGraphicsContext *global_context;
static void
setPixel(int x, int y)
{
    draw_patterned_pixel(global_context, x, y);
}

static void
plotLine(int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;	/* error value e_xy */

    for (;;) {			/* loop */
	setPixel(x0, y0);
	e2 = 2 * err;
	if (e2 >= dy) {		/* e_xy+e_x > 0 */
	    if (x0 == x1)
		break;
	    err += dy;
	    x0 += sx;
	}
	if (e2 <= dx) {		/* e_xy+e_y < 0 */
	    if (y0 == y1)
		break;
	    err += dx;
	    y0 += sy;
	}
    }
}

static void
plotQuadBezierSeg(int x0, int y0, int x1, int y1, int x2, int y2)
{				/* plot a limited quadratic Bezier segment */
    int sx = x2 - x1;
    int sy = y2 - y1;
    long xx = (x0 - x1);	/* relative values for checks */
    long yy = (y0 - y1);
    long xy;
    double dx, dy, err;
    double cur = (double) (xx * sy - yy * sx);	/* curvature */

    assert(xx * sx <= 0 && yy * sy <= 0);	/* sign of gradient must not change */

    if (sx * (long) sx + sy * (long) sy > xx * xx + yy * yy) {	/* begin with longer part */
	x2 = x0;
	x0 = sx + x1;
	y2 = y0;
	y0 = sy + y1;
	cur = -cur;		/* swap P0 P2 */
    }
    if (cur != 0) {		/* no straight line */
	xx += sx;
	xx *= sx = x0 < x2 ? 1 : -1;	/* x step direction */
	yy += sy;
	yy *= sy = y0 < y2 ? 1 : -1;	/* y step direction */
	xy = 2 * xx * yy;
	xx *= xx;
	yy *= yy;		/* differences 2nd degree */
	if (cur * sx * sy < 0) {	/* negated curvature? */
	    xx = -xx;
	    yy = -yy;
	    xy = -xy;
	    cur = -cur;
	}
	/* differences 1st degree */
	dx = ((4.0 * sy * cur * (x1 - x0)) + (double) xx) - (double) xy;
	dy = ((4.0 * sx * cur * (y0 - y1)) + (double) yy) - (double) xy;
	xx += xx;
	yy += yy;
	err = dx + dy + (double) xy;	/* error 1st step */
	do {
	    setPixel(x0, y0);	/* plot curve */
	    if (x0 == x2 && y0 == y2)
		return;		/* last pixel -> curve finished */
	    y1 = (2 * err) < dx;	/* save value for test of y step */
	    if ((2 * err) > dy) {
		x0 += sx;
		dx -= (double) xy;
		dy += (double) yy;
		err += dy;
	    }			/* x step */
	    if (y1) {
		y0 += sy;
		dy -= (double) xy;
		dx += (double) xx;
		err += dx;
	    }			/* y step */
	} while (dy < 0 && dx > 0);	/* gradient negates -> algorithm fails */
    }
    plotLine(x0, y0, x2, y2);	/* plot remaining part to end */
}

#if 0
static void
plotQuadBezier(int x0, int y0, int x1, int y1, int x2, int y2)
{				/* plot any quadratic Bezier curve */
    int x = x0 - x1;
    int y = y0 - y1;
    double t = x0 - 2 * x1 + x2;
    double r;

    if ((long) x * (x2 - x1) > 0) {	/* horizontal cut at P4? */
	if ((long) y * (y2 - y1) > 0)	/* vertical cut at P6 too? */
	    if (fabs((y0 - 2 * y1 + y2) / t * x) > abs(y)) {	/* which first? */
		x0 = x2;
		x2 = x + x1;
		y0 = y2;
		y2 = y + y1;	/* swap points */
	    }			/* now horizontal cut at P4 comes first */
	t = (x0 - x1) / t;
	r = (1 - t) * ((1 - t) * y0 + 2.0 * t * y1) + t * t * y2;	/* By(t=P4) */
	t = (x0 * x2 - x1 * x1) * t / (x0 - x1);	/* gradient dP4/dx=0 */
	x = ifloor(t + 0.5);
	y = ifloor(r + 0.5);
	r = (y1 - y0) * (t - x0) / (x1 - x0) + y0;	/* intersect P3 | P0 P1 */
	plotQuadBezierSeg(x0, y0, x, ifloor(r + 0.5), x, y);
	r = (y1 - y2) * (t - x2) / (x1 - x2) + y2;	/* intersect P4 | P1 P2 */
	x0 = x1 = x;
	y0 = y;
	y1 = ifloor(r + 0.5);	/* P0 = P4, P1 = P8 */
    }
    if ((long) (y0 - y1) * (y2 - y1) > 0) {	/* vertical cut at P6? */
	t = y0 - 2 * y1 + y2;
	t = (y0 - y1) / t;
	r = (1 - t) * ((1 - t) * x0 + 2.0 * t * x1) + t * t * x2;	/* Bx(t=P6) */
	t = (y0 * y2 - y1 * y1) * t / (y0 - y1);	/* gradient dP6/dy=0 */
	x = ifloor(r + 0.5);
	y = ifloor(t + 0.5);
	r = (x1 - x0) * (t - y0) / (y1 - y0) + x0;	/* intersect P6 | P0 P1 */
	plotQuadBezierSeg(x0, y0, ifloor(r + 0.5), y, x, y);
	r = (x1 - x2) * (t - y2) / (y1 - y2) + x2;	/* intersect P7 | P1 P2 */
	x0 = x;
	x1 = ifloor(r + 0.5);
	y0 = y1 = y;		/* P0 = P6, P1 = P7 */
    }
    plotQuadBezierSeg(x0, y0, x1, y1, x2, y2);	/* remaining part */
}
#endif

static void
plotCubicBezierSeg(int x0, int y0,
		   double x1, double y1,
		   double x2, double y2,
		   int x3, int y3)
{				/* plot limited cubic Bezier segment */
    int f, fx, fy, tt;
    int leg = 1;
    int sx = x0 < x3 ? 1 : -1;
    int sy = y0 < y3 ? 1 : -1;	/* step direction */
    double xc = -fabs(x0 + x1 - x2 - x3);
    double xa = xc - 4 * sx * (x1 - x2);
    double xb = sx * (x0 - x1 - x2 + x3);
    double yc = -fabs(y0 + y1 - y2 - y3);
    double ya = yc - 4 * sy * (y1 - y2);
    double yb = sy * (y0 - y1 - y2 + y3);
    double ab, ac, bc, cb, xx, xy, yy, dx, dy, ex, *pxy;
    double EP = 0.01;
    /* check for curve restrains */
    /* slope P0-P1 == P2-P3    and  (P0-P3 == P1-P2      or   no slope change) */
    assert((x1 - x0) * (x2 - x3) < EP &&
	   ((x3 - x0) * (x1 - x2) < EP || xb * xb < xa * xc + EP));
    assert((y1 - y0) * (y2 - y3) < EP &&
	   ((y3 - y0) * (y1 - y2) < EP || yb * yb < ya * yc + EP));

    if (xa == 0 && ya == 0) {	/* quadratic Bezier */
	sx = ifloor((3 * x1 - x0 + 1) / 2);
	sy = ifloor((3 * y1 - y0 + 1) / 2);	/* new midpoint */
	plotQuadBezierSeg(x0, y0, sx, sy, x3, y3);
	return;
    }
    x1 = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) + 1;	/* line lengths */
    x2 = (x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3) + 1;
    do {			/* loop over both ends */
	ab = xa * yb - xb * ya;
	ac = xa * yc - xc * ya;
	bc = xb * yc - xc * yb;
	ex = ab * (ab + ac - 3 * bc) + ac * ac;		/* P0 part of self-intersection loop? */
	f = ((ex > 0.0)
	     ? 1
	     : isqrt(1 + 1024 / x1));	/* calculate resolution */
	ab *= f;
	ac *= f;
	bc *= f;
	ex *= f * f;		/* increase resolution */
	xy = 9 * (ab + ac + bc) / 8;
	cb = 8 * (xa - ya);	/* init differences of 1st degree */
	dx = 27 * (8 * ab * (yb * yb - ya * yc) +
		   ex * (ya + 2 * yb + yc)) / 64 - ya * ya * (xy - ya);
	dy = 27 * (8 * ab * (xb * xb - xa * xc) -
		   ex * (xa + 2 * xb + xc)) / 64 - xa * xa * (xy + xa);
	/* init differences of 2nd degree */
	xx = 3 * (3 * ab * (3 * yb * yb - ya * ya - 2 * ya * yc) -
		  ya * (3 * ac * (ya + yb) + ya * cb)) / 4;
	yy = 3 * (3 * ab * (3 * xb * xb - xa * xa - 2 * xa * xc) -
		  xa * (3 * ac * (xa + xb) + xa * cb)) / 4;
	xy = xa * ya * (6 * ab + 6 * ac - 3 * bc + cb);
	ac = ya * ya;
	cb = xa * xa;
	xy = 3 * (xy + 9 * f * (cb * yb * yc - xb * xc * ac) -
		  18 * xb * yb * ab) / 8;

	if (ex < 0) {		/* negate values if inside self-intersection loop */
	    dx = -dx;
	    dy = -dy;
	    xx = -xx;
	    yy = -yy;
	    xy = -xy;
	    ac = -ac;
	    cb = -cb;
	}			/* init differences of 3rd degree */
	ab = 6 * ya * ac;
	ac = -6 * xa * ac;
	bc = 6 * ya * cb;
	cb = -6 * xa * cb;
	dx += xy;
	ex = dx + dy;
	dy += xy;		/* error of 1st step */

	for (pxy = &xy, fx = fy = f; x0 != x3 && y0 != y3;) {
	    setPixel(x0, y0);	/* plot curve */
	    do {		/* move sub-steps of one pixel */
		if (dx > *pxy || dy < *pxy)
		    goto exit;	/* confusing values */
		y1 = 2 * ex - dy;	/* save value for test of y step */
		if (2 * ex >= dx) {	/* x sub-step */
		    fx--;
		    ex += dx += xx;
		    dy += xy += ac;
		    yy += bc;
		    xx += ab;
		}
		if (y1 <= 0) {	/* y sub-step */
		    fy--;
		    ex += dy += yy;
		    dx += xy += bc;
		    xx += ac;
		    yy += cb;
		}
	    } while (fx > 0 && fy > 0);		/* pixel complete? */
	    if (2 * fx <= f) {
		x0 += sx;
		fx += f;
	    }			/* x step */
	    if (2 * fy <= f) {
		y0 += sy;
		fy += f;
	    }			/* y step */
	    if (pxy == &xy && dx < 0 && dy > 0)
		pxy = &EP;	/* pixel ahead valid */
	}
      exit:
	EXCHANGE(x0, x3, tt);
	sx = -sx;
	xb = -xb;		/* swap legs */
	EXCHANGE(y0, y3, tt);
	sy = -sy;
	yb = -yb;
	x1 = x2;
    } while (leg--);		/* try other end */
    plotLine(x0, y0, x3, y3);	/* remaining part in case of cusp or crunode */
}

static void
plotCubicBezier(int x0, int y0, int x1, int y1,
		int x2, int y2, int x3, int y3)
{				/* plot any cubic Bezier curve */
    int n = 0, i = 0;
    long xc = x0 + x1 - x2 - x3;
    long xa = xc - 4 * (x1 - x2);
    long xb = x0 - x1 - x2 + x3;
    long xd = xb + 4 * (x1 + x2);
    long yc = y0 + y1 - y2 - y3;
    long ya = yc - 4 * (y1 - y2);
    long yb = y0 - y1 - y2 + y3;
    long yd = yb + 4 * (y1 + y2);
    double fx0 = x0, fx1, fx2, fx3, fy0 = y0, fy1, fy2, fy3;
    double t1 = (double) (xb * xb - xa * xc), t2, t[5];

#ifdef DEBUG_BEZIER
    printf("plotCubicBezier(%d,%d, %d,%d, %d,%d, %d,%d\n",
	   x0, y0, x1, y1, x2, y2, x3, y3);
#endif
    /* sub-divide curve at gradient sign changes */
    if (xa == 0) {		/* horizontal */
	if (labs(xc) < 2 * labs(xb))
	    t[n++] = (double) xc / (2.0 * (double) xb);		/* one change */
    } else if (t1 > 0.0) {	/* two changes */
	t2 = sqrt(t1);
	t1 = ((double) xb - t2) / (double) xa;
	if (fabs(t1) < 1.0)
	    t[n++] = t1;
	t1 = ((double) xb + t2) / (double) xa;
	if (fabs(t1) < 1.0)
	    t[n++] = t1;
    }
    t1 = (double) (yb * yb - ya * yc);
    if (ya == 0) {		/* vertical */
	if (labs(yc) < 2 * labs(yb))
	    t[n++] = (double) yc / (2.0 * (double) yb);		/* one change */
    } else if (t1 > 0.0) {	/* two changes */
	t2 = sqrt(t1);
	t1 = ((double) yb - t2) / (double) ya;
	if (fabs(t1) < 1.0)
	    t[n++] = t1;
	t1 = ((double) yb + t2) / (double) ya;
	if (fabs(t1) < 1.0)
	    t[n++] = t1;
    }
    for (i = 1; i < n; i++)	/* bubble sort of 4 points */
	if ((t1 = t[i - 1]) > t[i]) {
	    t[i - 1] = t[i];
	    t[i] = t1;
	    i = 0;
	}

    t1 = -1.0;
    t[n] = 1.0;			/* begin / end point */
    for (i = 0; i <= n; i++) {	/* plot each segment separately */
	t2 = t[i];		/* sub-divide at t[i-1], t[i] */
	fx1 = (t1 * (t1 * (double) xb - (double) (2 * xc)) -
	       t2 * (t1 * (t1 * (double) xa - (double) (2 * xb)) + (double)
		     xc) + (double) xd) / 8 - fx0;
	fy1 = (t1 * (t1 * (double) yb - (double) (2 * yc)) -
	       t2 * (t1 * (t1 * (double) ya - (double) (2 * yb)) + (double)
		     yc) + (double) yd) / 8 - fy0;
	fx2 = (t2 * (t2 * (double) xb - (double) (2 * xc)) -
	       t1 * (t2 * (t2 * (double) xa - (double) (2 * xb)) + (double)
		     xc) + (double) xd) / 8 - fx0;
	fy2 = (t2 * (t2 * (double) yb - (double) (2 * yc)) -
	       t1 * (t2 * (t2 * (double) ya - (double) (2 * yb)) + (double)
		     yc) + (double) yd) / 8 - fy0;
	fx0 -= fx3 = (t2 * (t2 * ((double) (3 * xb) - t2 * (double) xa) -
			    (double) (3 * xc)) + (double) xd) / 8;
	fy0 -= fy3 = (t2 * (t2 * ((double) (3 * yb) - t2 * (double) ya) -
			    (double) (3 * yc)) + (double) yd) / 8;
	x3 = ifloor(fx3 + 0.5);
	y3 = ifloor(fy3 + 0.5);	/* scale bounds to int */
	if (fx0 != 0.0) {
	    fx1 *= fx0 = (x0 - x3) / fx0;
	    fx2 *= fx0;
	}
	if (fy0 != 0.0) {
	    fy1 *= fy0 = (y0 - y3) / fy0;
	    fy2 *= fy0;
	}
	if (x0 != x3 || y0 != y3)	/* segment t1 - t2 */
	    plotCubicBezierSeg(x0, y0,
			       x0 + fx1, y0 + fy1,
			       x0 + fx2, y0 + fy2,
			       x3, y3);
	x0 = x3;
	y0 = y3;
	fx0 = fx3;
	fy0 = fy3;
	t1 = t2;
    }
}

#if 0
static void
plotQuadSpline(int n, int x[], int y[], int skip_segments)
{				/* plot quadratic spline, destroys input arrays x,y */
#define M_MAX 12
    double mi = 1, m[M_MAX];	/* diagonal constants of matrix */
    int i, x0, y0, x1, y1, x2, y2;
#ifdef DEBUG_SPLINE_SEGMENTS
    int color = 0;
#endif

    assert(n > 1);		/* need at least 3 points P[0]..P[n] */

#ifdef DEBUG_SPLINE_POINTS
    {
	int save_pattern;

	i = 0;
	global_context->temporary_write_controls.foreground = 11;
	save_pattern = global_context->temporary_write_controls.pattern;
	global_context->temporary_write_controls.pattern = 0xff;
	draw_patterned_arc(global_context, x[i], y[i], x[i] + 2, y[i], 0, 360);
	i++;
	global_context->temporary_write_controls.foreground = 15;
	for (; i < n; i++) {
	    draw_patterned_arc(global_context,
			       x[i], y[i],
			       x[i] + 2, y[i],
			       0, 360);
	}
	global_context->temporary_write_controls.foreground = 10;
	draw_patterned_arc(global_context, x[i], y[n], x[i] + 2, y[i], 0, 360);
	global_context->temporary_write_controls.pattern = save_pattern;
    }
#endif

    x2 = x[n];
    y2 = y[n];

    x[1] = x0 = 8 * x[1] - 2 * x[0];	/* first row of matrix */
    y[1] = y0 = 8 * y[1] - 2 * y[0];

    for (i = 2; i < n; i++) {	/* forward sweep */
	if (i - 2 < M_MAX)
	    m[i - 2] = mi = 1.0 / (6.0 - mi);
	x[i] = x0 = ifloor(8 * x[i] - x0 * mi + 0.5);	/* store yi */
	y[i] = y0 = ifloor(8 * y[i] - y0 * mi + 0.5);
    }
    x1 = ifloor((x0 - 2 * x2) / (5.0 - mi) + 0.5);	/* correction last row */
    y1 = ifloor((y0 - 2 * y2) / (5.0 - mi) + 0.5);

    for (i = n - 2; i > 0; i--) {	/* back substitution */
	if (i <= M_MAX)
	    mi = m[i - 1];
	x0 = ifloor((x[i] - x1) * mi + 0.5);	/* next corner */
	y0 = ifloor((y[i] - y1) * mi + 0.5);
#ifdef DEBUG_SPLINE_SEGMENTS
	color++;
	global_context->temporary_write_controls.foreground = color;
#endif
	if ((n - 2) - i < skip_segments)
	    plotQuadBezier((x0 + x1) / 2, (y0 + y1) / 2, x1, y1, x2, y2);
	x2 = (x0 + x1) / 2;
	x1 = x0;
	y2 = (y0 + y1) / 2;
	y1 = y0;
    }
#ifdef DEBUG_SPLINE_SEGMENTS
    color++;
    global_context->temporary_write_controls.foreground = color;
#endif
    if (skip_segments > 0)
	plotQuadBezier(x[0], y[0], x1, y1, x2, y2);
}
#endif

static void
plotCubicSpline(int n, int x[], int y[], int skip_first_last)
{
#define M_MAX 12
    double mi = 0.25, m[M_MAX];	/* diagonal constants of matrix */
    int x3, y3, x4, y4;
    int i, x0, y0, x1, y1, x2, y2;
#ifdef DEBUG_SPLINE_SEGMENTS
    int color = 0;
#endif

    assert(n > 2);		/* need at least 4 points P[0]..P[n] */

#ifdef DEBUG_SPLINE_POINTS
    {
	int save_pattern;

	i = 0;
	global_context->temporary_write_controls.foreground = 11;
	save_pattern = global_context->temporary_write_controls.pattern;
	global_context->temporary_write_controls.pattern = 0xff;
	draw_patterned_arc(global_context, x[i], y[i], x[i] + 2, y[i], 0, 360);
	i++;
	global_context->temporary_write_controls.foreground = 15;
	for (; i < n; i++) {
	    draw_patterned_arc(global_context,
			       x[i], y[i],
			       x[i] + 2, y[i],
			       0, 360);
	}
	global_context->temporary_write_controls.foreground = 10;
	draw_patterned_arc(global_context, x[i], y[i], x[i] + 2, y[i], 0, 360);
	global_context->temporary_write_controls.pattern = save_pattern;
    }
#endif

    x3 = x[n - 1];
    y3 = y[n - 1];
    x4 = x[n];
    y4 = y[n];

    x[1] = x0 = 12 * x[1] - 3 * x[0];	/* first row of matrix */
    y[1] = y0 = 12 * y[1] - 3 * y[0];

    for (i = 2; i < n; i++) {	/* forward sweep */
	if (i - 2 < M_MAX)
	    m[i - 2] = mi = 0.25 / (2.0 - mi);
	x[i] = x0 = ifloor(12 * x[i] - 2 * x0 * mi + 0.5);
	y[i] = y0 = ifloor(12 * y[i] - 2 * y0 * mi + 0.5);
    }
    x2 = ifloor((x0 - 3 * x4) / (7 - 4 * mi) + 0.5);	/* correct last row */
    printf("y0=%d, y4=%d mi=%g\n", y0, y4, mi);
    y2 = ifloor((y0 - 3 * y4) / (7 - 4 * mi) + 0.5);
    printf("y2=%d, y3=%d, y4=%d\n", y2, y3, y4);
#ifdef DEBUG_SPLINE_SEGMENTS
    color++;
    global_context->temporary_write_controls.foreground = color;
#endif
    if (!skip_first_last)
	plotCubicBezier(x3, y3, (x2 + x4) / 2, (y2 + y4) / 2, x4, y4, x4, y4);

    if (n - 3 < M_MAX)
	mi = m[n - 3];
    x1 = ifloor((x[n - 2] - 2 * x2) * mi + 0.5);
    y1 = ifloor((y[n - 2] - 2 * y2) * mi + 0.5);
    for (i = n - 3; i > 0; i--) {	/* back substitution */
	if (i <= M_MAX)
	    mi = m[i - 1];
	x0 = ifloor((x[i] - 2 * x1) * mi + 0.5);
	y0 = ifloor((y[i] - 2 * y1) * mi + 0.5);
	x4 = ifloor((x0 + 4 * x1 + x2 + 3) / 6.0);	/* reconstruct P[i] */
	y4 = ifloor((y0 + 4 * y1 + y2 + 3) / 6.0);
#ifdef DEBUG_SPLINE_SEGMENTS
	color++;
	global_context->temporary_write_controls.foreground = color;
#endif
	plotCubicBezier(x4, y4,
			ifloor((2 * x1 + x2) / 3 + 0.5),
			ifloor((2 * y1 + y2) / 3 + 0.5),
			ifloor((x1 + 2 * x2) / 3 + 0.5),
			ifloor((y1 + 2 * y2) / 3 + 0.5),
			x3, y3);
	x3 = x4;
	y3 = y4;
	x2 = x1;
	y2 = y1;
	x1 = x0;
	y1 = y0;
    }
    x0 = x[0];
    x4 = ifloor((3 * x0 + 7 * x1 + 2 * x2 + 6) / 12.0);		/* reconstruct P[1] */
    y0 = y[0];
    y4 = ifloor((3 * y0 + 7 * y1 + 2 * y2 + 6) / 12.0);
#ifdef DEBUG_SPLINE_SEGMENTS
    global_context->temporary_write_controls.foreground = 4;
#endif
    plotCubicBezier(x4, y4,
		    ifloor((2 * x1 + x2) / 3 + 0.5),
		    ifloor((2 * y1 + y2) / 3 + 0.5),
		    ifloor((x1 + 2 * x2) / 3 + 0.5),
		    ifloor((y1 + 2 * y2) / 3 + 0.5),
		    x3, y3);
#ifdef DEBUG_SPLINE_SEGMENTS
    color++;
    global_context->temporary_write_controls.foreground = color;
#endif
    if (!skip_first_last)
	plotCubicBezier(x0, y0, x0, y0, (x0 + x1) / 2, (y0 + y1) / 2, x4, y4);
}

static void
init_fragment(RegisDataFragment *fragment, char const *str)
{
    fragment->start = str;
    fragment->len = (unsigned) strlen(str);
    fragment->pos = 0U;
}

static void
copy_fragment(RegisDataFragment *dst, RegisDataFragment const *src)
{
    dst->start = src->start;
    dst->len = src->len;
    dst->pos = src->pos;
}

static char
peek_fragment(RegisDataFragment const *fragment)
{
    if (fragment->pos < fragment->len) {
	return fragment->start[fragment->pos];
    }
    return '\0';
}

static char
pop_fragment(RegisDataFragment *fragment)
{
    if (fragment->pos < fragment->len) {
	return fragment->start[fragment->pos++];
    }
    return '\0';
}

static size_t
fragment_len(RegisDataFragment const *fragment)
{
    return fragment->len - fragment->pos;
}

#define MAX_FRAG 1024
static char const *
fragment_to_tempstr(RegisDataFragment const *fragment)
{
    static char tempstr[MAX_FRAG + 1];
    size_t remaininglen = fragment_len(fragment);
    size_t minlen = ((remaininglen < MAX_FRAG)
		     ? remaininglen
		     : MAX_FRAG);

    (void) strncpy(tempstr, &fragment->start[fragment->pos], minlen);
    tempstr[minlen] = '\0';
    return tempstr;
}

static int
skip_regis_whitespace(RegisDataFragment *input)
{
    int skipped = 0;
    char ch;

    assert(input);

    for (; input->pos < input->len; input->pos++) {
	/* FIXME: the semicolon isn't whitespace -- it also terminates the current command even if inside of an optionset or extent */
	ch = input->start[input->pos];
	if (ch != ',' && ch != ';' && !IsSpace(ch)) {
	    break;
	}
	if (ch == '\n') {
	    TRACE(("end of input line\n\n"));
	}
	skipped = 1;
    }

    if (skipped)
	return 1;
    return 0;
}

static int
extract_regis_extent(RegisDataFragment *input, RegisDataFragment *output)
{
    char ch;

    assert(input);
    assert(output);

    output->start = &input->start[input->pos];
    output->len = 0U;
    output->pos = 0U;

    if (input->pos >= input->len)
	return 0;

    ch = input->start[input->pos];
    if (ch != '[')
	return 0;
    input->pos++;
    output->start++;

    /* FIXME: truncate to 16 bit signed integers */
    for (; input->pos < input->len; input->pos++, output->len++) {
	ch = input->start[input->pos];
	if (ch == ';') {
	    TRACE(("DATA_ERROR: end of input before closing bracket\n"));
	    break;
	}
	if (ch == ']')
	    break;
    }
    if (ch == ']')
	input->pos++;

    return 1;
}

static int
extract_regis_num(RegisDataFragment *input, RegisDataFragment *output)
{
    char ch = 0;
    int has_digits = 0;

    assert(input);
    assert(output);

    output->start = &input->start[input->pos];
    output->len = 0U;
    output->pos = 0U;

    for (; input->pos < input->len; input->pos++, output->len++) {
	ch = input->start[input->pos];
	if (ch != '0' && ch != '1' && ch != '2' && ch != '3' &&
	    ch != '4' && ch != '5' && ch != '6' && ch != '7' &&
	    ch != '8' && ch != '9') {
	    break;
	}
	has_digits = 1;
    }

    /* FIXME: what degenerate forms should be accepted ("E10" "1E" "1e" "1." "1ee10")? */
    /* FIXME: the terminal is said to support "floating point values", truncating to int... what do these look like? */
    if (has_digits && ch == 'E') {
	input->pos++;
	output->len++;
	for (; input->pos < input->len; input->pos++, output->len++) {
	    ch = input->start[input->pos];
	    if (ch != '0' && ch != '1' && ch != '2' && ch != '3' &&
		ch != '4' && ch != '5' && ch != '6' && ch != '7' &&
		ch != '8' && ch != '9') {
		break;
	    }
	}
    }

    if (output->len < 1U)
	return 0;

    return 1;
}

static int
extract_regis_pixelvector(RegisDataFragment *input, RegisDataFragment *output)
{
    char ch;
    int has_digits;

    assert(input);
    assert(output);

    output->start = &input->start[input->pos];
    output->len = 0U;
    output->pos = 0U;

    if (input->pos < input->len) {
	ch = input->start[input->pos];
	if (ch == '+' || ch == '-') {
	    input->pos++;
	    output->len++;
	}
    }

    has_digits = 0;
    for (; input->pos < input->len; input->pos++, output->len++) {
	ch = input->start[input->pos];
	if (ch != '0' && ch != '1' && ch != '2' && ch != '3' &&
	    ch != '4' && ch != '5' && ch != '6' && ch != '7') {
	    break;
	}
	has_digits = 1;
    }

    return has_digits;
}

static int
extract_regis_command(RegisDataFragment *input, char *command)
{
    char ch;

    assert(input);
    assert(command);

    if (input->pos >= input->len)
	return 0;

    ch = input->start[input->pos];
    if (ch == '\0' || ch == ';') {
	return 0;
    }
    if (!islower(ch) && !isupper(ch)) {
	return 0;
    }
    *command = ch;
    input->pos++;

    return 1;
}

static int
extract_regis_string(RegisDataFragment *input, char *out, unsigned maxlen)
{
    char first_ch;
    char ch;
    char prev_ch;
    unsigned outlen = 0U;

    assert(input);
    assert(out);

    if (input->pos >= input->len)
	return 0;

    ch = input->start[input->pos];
    if (ch != '\'' && ch != '"')
	return 0;
    first_ch = ch;
    input->pos++;

    ch = '\0';
    for (; input->pos < input->len; input->pos++) {
	prev_ch = ch;
	ch = input->start[input->pos];
	/* ';' (resync) is not recognized in strings */
	if (prev_ch == first_ch) {
	    if (ch == first_ch) {
		if (outlen < maxlen) {
		    out[outlen] = ch;
		}
		outlen++;
		ch = '\0';
		continue;
	    }
	    if (outlen < maxlen)
		out[outlen] = '\0';
	    else
		out[maxlen] = '\0';
	    return 1;
	}
	if (ch == '\0')
	    break;
	if (ch != first_ch) {
	    if (outlen < maxlen) {
		out[outlen] = ch;
	    }
	    outlen++;
	}
    }
    if (ch == first_ch) {
	if (outlen < maxlen)
	    out[outlen] = '\0';
	else
	    out[maxlen] = '\0';
	return 1;
    }
    /* FIXME: handle multiple strings concatenated with commas */

    TRACE(("DATA_ERROR: end of input during before closing quote\n"));
    return 0;
}

static int
extract_regis_optionset(RegisDataFragment *input, RegisDataFragment *output)
{
    char ch;
    int nesting;

    assert(input);
    assert(output);

    output->start = &input->start[input->pos];
    output->len = 0U;
    output->pos = 0U;

    if (input->pos >= input->len)
	return 0;

    ch = input->start[input->pos];
    if (ch != '(')
	return 0;
    input->pos++;
    output->start++;
    nesting = 1;

    /* FIXME: handle strings with parens */
    for (; input->pos < input->len; input->pos++, output->len++) {
	ch = input->start[input->pos];
	if (ch == ';')
	    break;
	if (ch == '(')
	    nesting++;
	if (ch == ')') {
	    nesting--;
	    if (nesting == 0) {
		input->pos++;
		return 1;
	    }
	}
    }

    TRACE(("DATA_ERROR: end of input before closing paren (%d levels deep)\n", nesting));
    return 0;
}

static int
extract_regis_option(RegisDataFragment *input,
		     char *option,
		     RegisDataFragment *output)
{
    char ch;
    int nesting;

    assert(input);
    assert(option);
    assert(output);

    /* LETTER suboptions* value? */
    /* FIXME: can there be whitespace or commas inside of an option? */
    /* FIXME: what are the rules for using separate parens vs. sharing between options? */

    output->start = &input->start[input->pos];
    output->len = 0U;
    output->pos = 0U;

    if (input->pos >= input->len) {
	return 0;
    }

    ch = input->start[input->pos];
    if (ch == ';' || ch == ',' || ch == '(' || ch == ')' || isdigit(ch)) {
	return 0;
    }
    *option = ch;
    input->pos++;
    output->start++;
    nesting = 0;

    /* FIXME: handle strings with parens, nested parens, etc. */
    for (; input->pos < input->len; input->pos++, output->len++) {
	ch = input->start[input->pos];
	TRACE(("looking at option char %c\n", ch));
	/* FIXME: any special rules for commas?  any need to track parens? */
	if (ch == '(') {
	    TRACE(("nesting++\n"));
	    nesting++;
	}
	if (ch == ')') {
	    TRACE(("nesting--\n"));
	    nesting--;
	    if (nesting < 0) {
		TRACE(("DATA_ERROR: found ReGIS option has value with too many close parens \"%c\"\n", *option));
		return 0;
	    }
	}
	/* top-level commas indicate the end of this option and the start of another */
	if (nesting == 0 && ch == ',')
	    break;
	if (ch == ';')
	    break;
    }
    if (nesting != 0) {
	TRACE(("DATA_ERROR: mismatched parens in argument to ReGIS option \"%c\"\n", *option));
	return 0;
    }

    TRACE(("found ReGIS option and value \"%c\" \"%s\"\n",
	   *option,
	   fragment_to_tempstr(output)));
    return 1;
}

static int
regis_num_to_int(RegisDataFragment const *input, int *out)
{
    char ch;

    /* FIXME: handle exponential notation and rounding */
    /* FIXME: check for junk after the number */
    ch = peek_fragment(input);
    if (ch != '0' &&
	ch != '1' &&
	ch != '2' &&
	ch != '3' &&
	ch != '4' &&
	ch != '5' &&
	ch != '6' &&
	ch != '7' &&
	ch != '8' &&
	ch != '9' &&
	ch != '+' &&
	ch != '-') {
	return 0;
    }

    TRACE(("converting \"%s\" to an int\n", fragment_to_tempstr(input)));
    *out = atoi(fragment_to_tempstr(input));
    return 1;
}

static int
load_regis_colorspec(Graphic const *graphic, RegisDataFragment const *input, RegisterNum *out)
{
    int val;
    RegisDataFragment colorspec;
    RegisDataFragment coloroption;

    copy_fragment(&colorspec, input);
    TRACE(("looking at colorspec pattern: \"%s\"\n", fragment_to_tempstr(&colorspec)));

    if (regis_num_to_int(&colorspec, &val)) {
	if (val < 0 || val >= (int) graphic->valid_registers) {		/* FIXME: wrap? */
	    TRACE(("DATA_ERROR: colorspec value %d\n", val));
	    return 0;
	}
	TRACE(("colorspec contains index for register %u\n", val));
	*out = (RegisterNum) val;
	return 1;
    }

    if (extract_regis_optionset(&colorspec, &coloroption)) {
	short r, g, b;
	TRACE(("option: \"%s\"\n", fragment_to_tempstr(&coloroption)));

	if (fragment_len(&coloroption) == 1) {
	    char ch = pop_fragment(&coloroption);

	    TRACE(("got regis RGB colorspec pattern: \"%s\"\n",
		   fragment_to_tempstr(&coloroption)));
	    switch (ch) {
	    case 'D':
	    case 'd':
		r = 0;
		g = 0;
		b = 0;
		break;
	    case 'R':
	    case 'r':
		r = 100;
		g = 0;
		b = 0;
		break;
	    case 'G':
	    case 'g':
		r = 0;
		g = 100;
		b = 0;
		break;
	    case 'B':
	    case 'b':
		r = 0;
		g = 0;
		b = 100;
		break;
	    case 'C':
	    case 'c':
		r = 0;
		g = 100;
		b = 100;
		break;
	    case 'Y':
	    case 'y':
		r = 100;
		g = 100;
		b = 0;
		break;
	    case 'M':
	    case 'm':
		r = 100;
		g = 0;
		b = 100;
		break;
	    case 'W':
	    case 'w':
		r = 100;
		g = 100;
		b = 100;
		break;
	    default:
		TRACE(("unknown RGB color name: \"%c\"\n", ch));
		return 0;
	    }
	} else {
	    short h, l, s;

	    if (sscanf(fragment_to_tempstr(&coloroption),
		       "%*1[Hh]%hd%*1[Ll]%hd%*1[Ss]%hd",
		       &h, &l, &s) != 3) {
		TRACE(("unrecognized colorspec format: \"%s\"\n",
		       fragment_to_tempstr(&coloroption)));
		return 0;
	    }
	    hls2rgb(h, l, s, &r, &g, &b);
	}
	/* FIXME: check for trailing junk? */
	*out = find_color_register(graphic->color_registers, r, g, b);
	TRACE(("colorspec maps to closest register %u\n", *out));
	return 1;
    }

    TRACE(("unrecognized colorspec format: \"%s\"\n", fragment_to_tempstr(&colorspec)));
    return 0;
}

static int
load_regis_extent(char const *extent, int origx, int origy, int *xloc, int *yloc)
{
    int xsign, ysign;
    char const *xpart;
    char const *ypart;

    xpart = extent;
    if ((ypart = strchr(extent, ','))) {
	ypart++;
    } else {
	ypart = "";
    }

    if (xpart[0] == '-') {
	xsign = -1;
	xpart++;
    } else if (xpart[0] == '+') {
	xsign = +1;
	xpart++;
    } else {
	xsign = 0;
    }
    if (ypart[0] == '-') {
	ysign = -1;
	ypart++;
    } else if (ypart[0] == '+') {
	ysign = +1;
	ypart++;
    } else {
	ysign = 0;
    }

    if (xpart[0] == '\0' || xpart[0] == ',') {
	*xloc = origx;
    } else if (xsign == 0) {
	*xloc = atoi(xpart);
    } else {
	*xloc = origx + xsign * atoi(xpart);
    }
    if (ypart[0] == '\0') {
	*yloc = origy;
    } else if (ysign == 0) {
	*yloc = atoi(ypart);
    } else {
	*yloc = origy + ysign * atoi(ypart);
    }

    return 1;
}

static int
load_regis_pixelvector(char const *pixelvector,
		       unsigned mul,
		       int origx, int origy,
		       int *xloc, int *yloc)
{
    int dx = 0, dy = 0;
    int i;

    for (i = 0; pixelvector[i] != '\0'; i++) {
	switch (pixelvector[i]) {
	case '0':
	    dx += 1;
	    break;
	case '1':
	    dx += 1;
	    dy -= 1;
	    break;
	case '2':
	    dy -= 1;
	    break;
	case '3':
	    dx -= 1;
	    dy -= 1;
	    break;
	case '4':
	    dx -= 1;
	    break;
	case '5':
	    dx -= 1;
	    dy += 1;
	    break;
	case '6':
	    dy += 1;
	    break;
	case '7':
	    dx += 1;
	    dy += 1;
	    break;
	default:
	    break;
	}
    }

    *xloc = origx + dx * (int) mul;
    *yloc = origy + dy * (int) mul;

    return 1;
}

static int
load_regis_write_control(RegisParseState *state,
			 Graphic const *graphic,
			 int cur_x, int cur_y,
			 int option,
			 RegisDataFragment *arg,
			 RegisWriteControls *out)
{
    TRACE(("checking write control option \"%c\" with arg \"%s\"\n",
	   option, fragment_to_tempstr(arg)));
    switch (option) {
    case 'E':
    case 'e':
	TRACE(("write control erase writing mode \"%s\"\n",
	       fragment_to_tempstr(arg)));
	out->write_style = WRITE_STYLE_ERASE;
	break;
    case 'F':
    case 'f':
	TRACE(("write control plane write mask \"%s\"\n",
	       fragment_to_tempstr(arg)));
	{
	    int val;
	    if (!regis_num_to_int(arg, &val) ||
		val < 0 || val >= (int) graphic->valid_registers) {
		TRACE(("interpreting out of range value as 0 FIXME\n"));
		out->plane_mask = 0U;
	    } else {
		out->plane_mask = (unsigned) val;
	    }
	}
	break;
    case 'I':
    case 'i':
	TRACE(("write control foreground color \"%s\"\n",
	       fragment_to_tempstr(arg)));
	if (!load_regis_colorspec(graphic, arg, &out->foreground)) {
	    TRACE(("DATA_ERROR: write control foreground color specifier not recognized: \"%s\"\n",
		   fragment_to_tempstr(arg)));
	    return 0;
	}
	break;
    case 'M':
    case 'm':
	TRACE(("write control found pixel multiplication factor \"%s\"\n",
	       fragment_to_tempstr(arg)));
	{
	    int val;
	    if (!regis_num_to_int(arg, &val) || val <= 0) {
		TRACE(("interpreting out of range value %d as 1 FIXME\n", val));
		out->pv_multiplier = 1U;
	    } else {
		out->pv_multiplier = (unsigned) val;
	    }
	}
	break;
    case 'N':
    case 'n':
	TRACE(("write control negative pattern control \"%s\"\n",
	       fragment_to_tempstr(arg)));
	{
	    int val;
	    if (!regis_num_to_int(arg, &val)) {
		val = -1;
	    }
	    switch (val) {
	    default:
		TRACE(("interpreting out of range value %d as 0 FIXME\n", val));
		out->invert_pattern = 0U;
		break;
	    case 0:
		out->invert_pattern = 0U;
		break;
	    case 1:
		out->invert_pattern = 1U;
		break;
	    }
	}
	break;
    case 'P':
    case 'p':
	TRACE(("write control found pattern control \"%s\"\n",
	       fragment_to_tempstr(arg)));
	{
	    RegisDataFragment suboptionset;
	    RegisDataFragment suboptionarg;
	    RegisDataFragment item;
	    char suboption;

	    while (arg->pos < arg->len) {
		skip_regis_whitespace(arg);
		TRACE(("looking for option in \"%s\"\n", fragment_to_tempstr(arg)));
		if (extract_regis_optionset(arg, &suboptionset)) {
		    TRACE(("got regis write pattern suboptionset: \"%s\"\n",
			   fragment_to_tempstr(&suboptionset)));
		    while (suboptionset.pos < suboptionset.len) {
			skip_regis_whitespace(&suboptionset);
			if (peek_fragment(&suboptionset) == ',') {
			    pop_fragment(&suboptionset);
			    continue;
			}
			if (extract_regis_option(&suboptionset, &suboption, &suboptionarg)) {
			    TRACE(("inspecting write pattern suboption \"%c\" with value \"%s\"\n",
				   suboption, fragment_to_tempstr(&suboptionarg)));
			    switch (suboption) {
			    case 'M':
			    case 'm':
				TRACE(("found pattern multiplier \"%s\"\n",
				       fragment_to_tempstr(&suboptionarg)));
				{
				    RegisDataFragment num;
				    int val;

				    skip_regis_whitespace(&suboptionarg);
				    if (extract_regis_num(&suboptionarg, &num)) {
					if (!regis_num_to_int(&num, &val)
					    || val < 1) {
					    TRACE(("interpreting out of range pattern multiplier \"%s\" as 2 FIXME\n",
						   fragment_to_tempstr(&num)));
					    out->pattern_multiplier = 2U;
					} else {
					    out->pattern_multiplier =
						(unsigned) val;
					}
				    }
				    skip_regis_whitespace(&suboptionarg);
				    if (fragment_len(&suboptionarg)) {
					TRACE(("DATA_ERROR: unknown content after pattern multiplier \"%s\"\n",
					       fragment_to_tempstr(&suboptionarg)));
					return 0;
				    }
				}
				break;
			    default:
				TRACE(("DATA_ERROR: unknown ReGIS write pattern suboption '%c' arg \"%s\"\n",
				       suboption, fragment_to_tempstr(&suboptionarg)));
				return 0;
			    }
			    continue;
			}

			TRACE(("DATA_ERROR: skipping unknown token in pattern control suboptionset (expecting option): \"%s\"\n",
			       fragment_to_tempstr(&suboptionset)));
			pop_fragment(&suboptionset);
		    }
		    continue;
		}

		TRACE(("looking for int in \"%s\"\n", fragment_to_tempstr(arg)));
		if (extract_regis_num(arg, &item)) {
		    if (peek_fragment(&item) == '0' ||
			peek_fragment(&item) == '1') {
			unsigned pattern = 0U;
			unsigned bitcount;
			char ch;

			TRACE(("converting pattern bits \"%s\"\n",
			       fragment_to_tempstr(&item)));
			for (bitcount = 0;; bitcount++) {
			    ch = pop_fragment(&item);
			    if (ch == '\0')
				break;
			    switch (ch) {
			    case '0':
				if (bitcount < MAX_PATTERN_BITS) {
				    pattern <<= 1U;
				}
				break;
			    case '1':
				if (bitcount < MAX_PATTERN_BITS) {
				    pattern <<= 1U;
				    pattern |= 1U;
				}
				break;
			    default:
				TRACE(("DATA_ERROR: unknown ReGIS write pattern bit value \"%c\"\n",
				       ch));
				return 0;
			    }
			}

			if (bitcount > 0U) {
			    unsigned extrabits;

			    for (extrabits = 0;
				 bitcount + extrabits < MAX_PATTERN_BITS;
				 extrabits++) {
				if (pattern & (1U << (bitcount - 1U))) {
				    pattern <<= 1U;
				    pattern |= 1U;
				} else {
				    pattern <<= 1U;
				}
			    }
			}

			out->pattern = pattern;
		    } else {
			int val;

			TRACE(("converting pattern id \"%s\"\n",
			       fragment_to_tempstr(&item)));
			if (!regis_num_to_int(&item, &val))
			    val = -1;
			switch (val) {	/* FIXME: exponential allowed? */
			case 0:
			    out->pattern = 0x00;	/* solid bg */
			    break;
			case 1:
			    out->pattern = 0xff;	/* solid fg */
			    break;
			case 2:
			    out->pattern = 0xf0;	/* dash */
			    break;
			case 3:
			    out->pattern = 0xe4;	/* dash dot */
			    break;
			case 4:
			    out->pattern = 0xaa;	/* dot */
			    break;
			case 5:
			    out->pattern = 0xea;	/* dash dot dot */
			    break;
			case 6:
			    out->pattern = 0x88;	/* sparse dot */
			    break;
			case 7:
			    out->pattern = 0x84;	/* asymmetric sparse dot */
			    break;
			case 8:
			    out->pattern = 0xc8;	/* sparse dash dot */
			    break;
			case 9:
			    out->pattern = 0x86;	/* sparse dot dash */
			    break;
			default:
			    TRACE(("DATA_ERROR: unknown ReGIS standard write pattern \"%d\"\n", val));
			    return 0;
			}
		    }

		    TRACE(("final pattern is %02x\n", out->pattern));
		    continue;
		}

		TRACE(("DATA_ERROR: skipping unknown token in pattern suboption: \"%s\"\n",
		       fragment_to_tempstr(arg)));
		pop_fragment(arg);
	    }
	}
	break;
    case 'C':
    case 'c':
	TRACE(("write control compliment writing mode \"%s\"\n",
	       fragment_to_tempstr(arg)));
	out->write_style = WRITE_STYLE_COMPLEMENT;
	break;
    case 'R':
    case 'r':
	TRACE(("write control switch to replacement writing mode \"%s\"\n",
	       fragment_to_tempstr(arg)));
	out->write_style = WRITE_STYLE_REPLACE;
	break;
    case 'S':
    case 's':
	TRACE(("write control shading control \"%s\"\n",
	       fragment_to_tempstr(arg)));
	{
	    RegisDataFragment suboptionset;
	    RegisDataFragment suboptionarg;
	    RegisDataFragment item;
	    char suboption;
	    char shading_character = '\0';
	    unsigned reference_dim = WRITE_SHADING_REF_Y;
	    int ref_x = cur_x, ref_y = cur_y;
	    int shading_enabled = 0;

	    while (arg->pos < arg->len) {
		skip_regis_whitespace(arg);

		if (extract_regis_string(arg, state->temp, state->templen)) {
		    TRACE(("found fill char \"%s\"\n", state->temp));
		    /* FIXME: allow longer strings ignoring extra chars? */
		    if (strlen(state->temp) != 1) {
			TRACE(("DATA_ERROR: expected exactly one char in fill string FIXME\n"));
			return 0;
		    }
		    shading_character = state->temp[0];
		    TRACE(("shading character is: %d\n", (int) shading_character));
		    continue;
		}

		if (extract_regis_optionset(arg, &suboptionset)) {
		    TRACE(("got regis shading control suboptionset: \"%s\"\n",
			   fragment_to_tempstr(&suboptionset)));
		    while (suboptionset.pos < suboptionset.len) {
			skip_regis_whitespace(&suboptionset);
			if (peek_fragment(&suboptionset) == ',') {
			    pop_fragment(&suboptionset);
			    continue;
			}
			if (extract_regis_option(&suboptionset, &suboption, &suboptionarg)) {
			    TRACE(("inspecting write shading suboption \"%c\" with value \"%s\"\n",
				   suboption, fragment_to_tempstr(&suboptionarg)));
			    switch (suboption) {
			    case 'X':
			    case 'x':
				TRACE(("found vertical shading suboption \"%s\"\n",
				       fragment_to_tempstr(&suboptionarg)));
				if (fragment_len(&suboptionarg)) {
				    TRACE(("DATA_ERROR: unexpected value to vertical shading suboption FIXME\n"));
				    return 0;
				}
				reference_dim = WRITE_SHADING_REF_X;
				break;
			    default:
				TRACE(("DATA_ERROR: unknown ReGIS write pattern suboption '%c' arg \"%s\"\n",
				       suboption, fragment_to_tempstr(&suboptionarg)));
				return 0;
			    }
			    continue;
			}

			TRACE(("DATA_ERROR: skipping unknown token in shading control suboptionset (expecting option): \"%s\"\n",
			       fragment_to_tempstr(&suboptionset)));
			pop_fragment(&suboptionset);
		    }
		    continue;
		}

		if (extract_regis_extent(arg, &item)) {
		    if (!load_regis_extent(fragment_to_tempstr(&item),
					   ref_x, ref_y,
					   &ref_x, &ref_y)) {
			TRACE(("DATA_ERROR: unable to parse extent in write shading option '%c': \"%s\"\n",
			       option, fragment_to_tempstr(&item)));
			return 0;
		    }
		    TRACE(("shading reference = %d,%d (%s)\n", ref_x, ref_y,
			   ((reference_dim == WRITE_SHADING_REF_X)
			    ? "X"
			    : "Y")));
		    continue;
		}

		if (extract_regis_num(arg, &item)) {
		    if (!regis_num_to_int(&item, &shading_enabled)) {
			TRACE(("DATA_ERROR: unable to parse int in write shading option '%c': \"%s\"\n",
			       option, fragment_to_tempstr(&item)));
			return 0;
		    }
		    if (shading_enabled < 0 || shading_enabled > 1) {
			TRACE(("interpreting out of range value %d as 0 FIXME\n", shading_enabled));
			shading_enabled = 0;
		    }
		    TRACE(("shading enabled = %d\n", shading_enabled));
		    continue;
		}

		TRACE(("DATA_ERROR: skipping unknown token in shade suboption: \"%s\"\n",
		       fragment_to_tempstr(arg)));
		pop_fragment(arg);
	    }

	    if (shading_enabled) {
		out->shading_enabled = 1U;
		out->shading_reference_dim = reference_dim;
		out->shading_reference = ((reference_dim == WRITE_SHADING_REF_X)
					  ? ref_x
					  : ref_y);
		out->shading_character = shading_character;
	    } else {
		/* FIXME: confirm there is no effect if shading isn't enabled
		 * in the same command
		 */
		out->shading_enabled = 0U;
	    }
	}
	break;
    case 'V':
    case 'v':
	TRACE(("write control switch to overlay writing mode \"%s\"\n",
	       fragment_to_tempstr(arg)));
	out->write_style = WRITE_STYLE_OVERLAY;
	break;
    default:
	TRACE(("DATA_ERROR: ignoring unknown ReGIS write option \"%c\" arg \"%s\"\n",
	       option, fragment_to_tempstr(arg)));
	return 0;
    }

    return 1;
}

static int
load_regis_write_control_set(RegisParseState *state,
			     Graphic const *graphic,
			     int cur_x, int cur_y,
			     RegisDataFragment *controls,
			     RegisWriteControls *out)
{
    RegisDataFragment optionset;
    RegisDataFragment arg;
    char option;

    while (controls->pos < controls->len) {
	skip_regis_whitespace(controls);

	if (extract_regis_optionset(controls, &optionset)) {
	    TRACE(("got regis write control optionset: \"%s\"\n",
		   fragment_to_tempstr(&optionset)));
	    while (optionset.pos < optionset.len) {
		skip_regis_whitespace(&optionset);
		if (peek_fragment(&optionset) == ',') {
		    pop_fragment(&optionset);
		    continue;
		}
		if (extract_regis_option(&optionset, &option, &arg)) {
		    TRACE(("got regis write control option and value: \"%c\" \"%s\"\n",
			   option, fragment_to_tempstr(&arg)));
		    if (!load_regis_write_control(state, graphic,
						  cur_x, cur_y,
						  option, &arg, out)) {
			return 0;
		    }
		    continue;
		}

		TRACE(("DATA_ERROR: skipping unknown token in write control optionset (expecting option): \"%s\"\n",
		       fragment_to_tempstr(&optionset)));
		pop_fragment(&optionset);
	    }
	    continue;
	}

	TRACE(("DATA_ERROR: skipping unknown token in write controls (expecting optionset): \"%s\"\n",
	       fragment_to_tempstr(controls)));
	pop_fragment(controls);
    }

    return 1;
}

static void
init_regis_write_controls(int terminal_id, unsigned all_planes, RegisWriteControls *controls)
{
    controls->pv_multiplier = 1U;
    controls->pattern = 0xff;	/* solid */
    controls->pattern_multiplier = 2U;
    controls->invert_pattern = 0U;
    controls->plane_mask = all_planes;
    controls->write_style = WRITE_STYLE_OVERLAY;
    switch (terminal_id) {
    case 125:			/* FIXME */
    case 240:			/* FIXME */
    case 241:			/* FIXME */
    case 330:
	controls->foreground = 3U;
	break;
    case 340:
	controls->foreground = 7U;
	break;
    default:			/* FIXME */
	controls->foreground = 63U;
	break;
    }
    controls->shading_enabled = 0U;
    controls->shading_character = '\0';
    controls->shading_reference = 0;	/* no meaning if shading is disabled */
    controls->shading_reference_dim = WRITE_SHADING_REF_Y;
    /* FIXME: add the rest */
}

static void
copy_regis_write_controls(RegisWriteControls const *src,
			  RegisWriteControls *dst)
{
    dst->pv_multiplier = src->pv_multiplier;
    dst->pattern = src->pattern;
    dst->pattern_multiplier = src->pattern_multiplier;
    dst->invert_pattern = src->invert_pattern;
    dst->foreground = src->foreground;
    dst->plane_mask = src->plane_mask;
    dst->write_style = src->write_style;
    dst->shading_enabled = src->shading_enabled;
    dst->shading_character = src->shading_character;
    dst->shading_reference = src->shading_reference;
    dst->shading_reference_dim = src->shading_reference_dim;
}

static void
init_regis_graphics_context(int terminal_id, RegisGraphicsContext *context)
{
    context->terminal_id = terminal_id;
    /*
     * Generate a mask covering all valid color register address bits
     * (but don't bother past 2**16).
     */
    context->all_planes = (unsigned) context->graphic->valid_registers;
    context->all_planes--;
    context->all_planes |= 1U;
    context->all_planes |= context->all_planes >> 1U;
    context->all_planes |= context->all_planes >> 2U;
    context->all_planes |= context->all_planes >> 4U;
    context->all_planes |= context->all_planes >> 8U;

    init_regis_write_controls(terminal_id, context->all_planes, &context->persistent_write_controls);
    copy_regis_write_controls(&context->persistent_write_controls, &context->temporary_write_controls);

    /* FIXME: coordinates */
    /* FIXME: scrolling */
    /* FIXME: output maps */
    context->background = 0U;
    /* FIXME: input cursor location */
    /* FIXME: input cursor style */
    context->graphics_output_cursor_x = 0;
    context->graphics_output_cursor_y = 0;
    /* FIXME: output cursor style */
    /* FIXME: text settings */
}

static int
parse_regis_command(RegisParseState *state)
{
    char ch = peek_fragment(&state->input);
    if (ch == '\0')
	return 0;

    if (!extract_regis_command(&state->input, &ch))
	return 0;

    switch (ch) {
    case 'C':
    case 'c':
	/* Curve

	 * C
	 * (A)  # set the arc length in degrees (+ or nothing for
	 *      # counter-clockwise, - for clockwise, rounded to the
	 *      # closest integer degree)
	 * (B)  # begin closed curve sequence (must have at least two
	 *      # values; this option can not be nested)
	 * (C)  # position is the center, current location is the
	 *      # circumference (stays in effect until next command)
	 * (E)  # end curve sequence (drawing is performed here)
	 * (S)  # begin open curve sequence
	 * (W)  # temporary write options (see write command)
	 * [<center, circumference position>]  # center if (C), otherwise point on circumference
	 * [<point in curve sequence>]...  # if between (B) and (E)
	 * <pv>...  # if between (B) and (E)
	 */
	TRACE(("found ReGIS command \"%c\" (curve)\n", ch));
	state->command = 'c';
	state->curve_mode = CURVE_POSITION_ARC_EDGE;
	state->arclen = 360;
	state->num_points = 0U;
	break;
    case 'F':
    case 'f':
	/* Fill

	 * F
	 * (V)  # polygon (see vector command)
	 * (C)  # curve (see curve command)
	 * (W)  # temporary write options (see write command)
	 */
	TRACE(("found ReGIS command \"%c\" (filled polygon)\n", ch));
	state->command = 'f';
	break;
    case 'L':
    case 'l':
	/* Load

	 * L
	 * (A)  # set character set number and name
	 * "ascii"xx,xx,xx,xx,xx,xx,xx,xx  # pixel values
	 */
	TRACE(("found ReGIS command \"%c\" (load charset)\n", ch));
	state->command = 'l';
	break;
    case 'P':
    case 'p':
	/* Position

	 * P
	 * (B)  # begin bounded position stack (last point returns to first)
	 * (E)  # end position stack
	 * (S)  # begin unbounded position stack
	 * (W)  # temporary write options (see write command)
	 * <pv>  # move: 0 == right, 1 == upper right, ..., 7 == lower right
	 * [<position>]  # move to position (X, Y, or both)
	 *
	 * Note the stack does not need to be ended before the next command
	 * Note: maximum depth is 16 levels
	 */
	TRACE(("found ReGIS command \"%c\" (position)\n", ch));
	state->command = 'p';
	break;
    case 'R':
    case 'r':
	/* Report

	 * R
	 * (E)  # parse error
	 * (I<val>)  # set input mode (0 == oneshot, 1 == multiple) (always returns CR)
	 * (L)  # character set
	 * (M(<name>)  # macrograph contents
	 * (M(=)  # macrograph storage
	 * (P)  # cursor position
	 * (P(I))  # interactive cursor position
	 */
	TRACE(("found ReGIS command \"%c\" (report status)\n", ch));
	state->command = 'r';
	break;
    case 'S':
    case 's':
	/* Screen

	 * S
	 * (A[<upper left>][<lower right>])
	 * (C<setting>  # 0 (cursor output off), 1 (cursor output on)
	 * (E  # erase to background color, resets shades, curves, and stacks
	 * (H(P<printer offset>)[<print area cornet>][<print area corner>)
	 * (I<color register>)  # set the background to a specific register
	 * (I(<rgb>))  # set the background to the register closest to an RGB value
	 * (I(<hls>))  # set the background to the register closest to an HLS color
	 * (M<color index to set>(L<mono level>)...)  # level is 0 ... 100 (sets grayscale registers only)
	 * (M<color index to set>(<RGB code>)...)  # codes are D (black), R (red), G (green), B (blue), C (cyan), Y (yellow), M (magenta), W (white) (sets color and grayscale registers)
	 * (M<color index to set>(A<RGB code>)...)  # codes are D (black), R (red), G (green), B (blue), C (cyan), Y (yellow), M (magenta), W (white) (sets color registers only)
	 * (M<color index to set>(H<hue>L<lightness>S<saturation>)...)  # 0..360, 0..100, 0..100 (sets color and grayscale registers)
	 * (M<color index to set>(AH<hue>L<lightness>S<saturation>)...)  # 0..360, 0..100, 0..100 (sets color registers only)
	 * (P<graphics page number>)  # 0 (default) or 1
	 * (T(<time delay ticks>)  # 60 ticks per second, up to 32767 ticks
	 * (W(M<factor>)  # PV value
	 * [scroll offset]  # optional
	 */
	TRACE(("found ReGIS command \"%c\" (screen)\n", ch));
	state->command = 's';
	break;
    case 'T':
    case 't':
	/* Text

	 * T
	 * (A0L"<designator>"))  # specify a built-in set for GL via two-char designator
	 * (A0R"<designator>"))  # specify a built-in set for GR via two-char or three-char designator
	 * (A<num>R"<designator>"))  # specify a user-loaded (1-3) set for GR via two-char or three-char designator
	 * (B)  # begin temporary text control
	 * (D<angle>)  # specify a string tilt
	 * (E)  # end temporary text control
	 * (H<factor>)  # select a height multiplier (1-256)
	 * (I<angle>)  # italics: no slant (0), lean back (-1 though -45), lean forward (+1 through +45)
	 * (M[width factor,height factor])  # select size multipliers (width 1-16) (height 1-256)
	 * (S<size id>)  # select one of the 17 standard cell sizes
	 * (S[dimensions])  # set a custom display cell size (char with border)
	 * (U[dimensions])  # set a custom unit cell size (char size)
	 * (W<write command>)  # temporary write options (see write command)
	 * [<char offset>]  # optional offset between characters
	 * <PV spacing>  # for subscripts and superscripts
	 * '<text>'  # optional
	 * "<text>"  # optional
	 */
	TRACE(("found ReGIS command \"%c\" (text)\n", ch));
	state->command = 't';
	break;
    case 'V':
    case 'v':
	/* Vector

	 * V
	 * (B)  # begin bounded position stack (last point returns to first)
	 * (E)  # end position stack
	 * (S)  # begin unbounded position stack
	 * (W)  # temporary write options (see write command)
	 * <pv>  # draw a line to the pixel vector
	 * []  # draw a dot at the current location
	 * [<position>]  # draw a line to position
	 */
	TRACE(("found ReGIS command \"%c\" (vector)\n", ch));
	state->command = 'v';
	break;
    case 'W':
    case 'w':
	/* Write

	 * W
	 * (C)  # complement writing mode
	 * (E)  # erase writing mode
	 * (F<plane>)  # set the foreground intensity to a specific register
	 * (I<color register>)  # set the foreground to a specific register
	 * (I(<rgb>))  # set the foreground to the register closest to an RGB value
	 * (I(<hls>))  # set the foreground to the register closest to an HLS color
	 * (M<pixel vector multiplier>)  # set the multiplication factor
	 * (N<setting>)  # 0 == negative patterns disabled, 1 == negative patterns enabled
	 * (P<pattern number>)  # 0..9: 0 == none, 1 == solid, 2 == 50% dash, 3 == dash-dot
	 * (P<pattern bits>)  # 2 to 8 bits represented as a 0/1 sequence
	 * (P<(M<pattern multiplier>))
	 * (R)  # replacement writing mode
	 * (S'<character>')  # set shading character
	 * (S<setting>)  # 0 == disable shding, 1 == enable shading
	 * (S[reference point])  # set a horizontal reference line including this point
	 * (S(X)[reference point])  # set a vertical reference line including this point
	 * (V)  # overlay writing mode
	 */
	TRACE(("found ReGIS command \"%c\" (write parameters)\n", ch));
	state->command = 'w';
	break;
    case '@':
	/* Macrograph */
	TRACE(("found ReGIS macrograph command\n"));
	ch = pop_fragment(&state->input);
	TRACE(("inspecting macrograph character \"%c\"\n", ch));
	switch (ch) {
	case '.':
	    TRACE(("clearing all macrographs FIXME\n"));
	    /* FIXME: handle */
	    break;
	case ':':
	    TRACE(("defining macrograph FIXME\n"));
	    /* FIXME: parse, handle  :<name> */
	    break;
	case ';':
	    TRACE(("DATA_ERROR: found extraneous terminator for macrograph definition\n"));
	    break;
	default:
	    if ((ch > 'A' && ch < 'Z') || (ch > 'a' && ch < 'z')) {
		TRACE(("expanding macrograph \"%c\" FIXME\n", ch));
		/* FIXME: handle */
	    } else {
		TRACE(("DATA_ERROR: unknown macrograph subcommand \"%c\"\n", ch));
	    }
	    /* FIXME: parse, handle */
	    break;
	}
	break;
    default:
	TRACE(("DATA_ERROR: unknown ReGIS command %04x (%c)\n",
	       (int) ch, ch));
	state->command = '_';
	state->option = '_';
	return 0;
    }

    state->option = '_';

    return 1;
}

static int
parse_regis_optionset(RegisParseState *state)
{
    if (!extract_regis_optionset(&state->input, &state->optionset))
	return 0;

    TRACE(("found ReGIS optionset \"%s\"\n", fragment_to_tempstr(&state->optionset)));
    state->option = '_';

    return 1;
}

static int
parse_regis_option(RegisParseState *state, RegisGraphicsContext *context)
{
    RegisDataFragment optionarg;

    if (!extract_regis_option(&state->optionset, &state->option, &optionarg))
	return 0;

    TRACE(("found ReGIS option \"%c\": \"%s\"\n",
	   state->option, fragment_to_tempstr(&optionarg)));

    switch (state->command) {
    case 'c':
	TRACE(("inspecting curve option \"%c\" with value \"%s\"\n",
	       state->option, fragment_to_tempstr(&optionarg)));
	switch (state->option) {
	case 'A':
	case 'a':
	    TRACE(("found arc length \"%s\"\n", fragment_to_tempstr(&optionarg)));
	    {
		RegisDataFragment arclen;

		if (!extract_regis_num(&optionarg, &arclen)) {
		    TRACE(("DATA_ERROR: expected int in curve arclen option: \"%s\"\n",
			   fragment_to_tempstr(&arclen)));
		    break;
		}
		TRACE(("arc length string %s\n", fragment_to_tempstr(&arclen)));
		if (!regis_num_to_int(&arclen, &state->arclen)) {
		    TRACE(("DATA_ERROR: unable to parse int in curve arclen option: \"%s\"\n",
			   fragment_to_tempstr(&arclen)));
		    break;
		}
		TRACE(("value of arc length is %d\n", state->arclen));
		while (state->arclen < -360)
		    state->arclen += 360;
		while (state->arclen > 360)
		    state->arclen -= 360;
		TRACE(("using final arc length %d\n", state->arclen));
	    }
	    break;
	case 'B':
	case 'b':
	    TRACE(("begin closed curve \"%s\"\n", fragment_to_tempstr(&optionarg)));
	    if (fragment_len(&optionarg)) {
		TRACE(("DATA_ERROR: invalid closed curve option \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		break;
	    }
	    state->curve_mode = CURVE_POSITION_CLOSED_CURVE;
	    state->num_points = 0U;
	    state->x_points[state->num_points] = context->graphics_output_cursor_x;
	    state->y_points[state->num_points] = context->graphics_output_cursor_y;
	    state->num_points++;
	    break;
	case 'C':
	case 'c':
	    TRACE(("found center position mode \"%s\"\n",
		   fragment_to_tempstr(&optionarg)));
	    if (fragment_len(&optionarg)) {
		TRACE(("DATA_ERROR: invalid center position option \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		break;
	    }
	    state->curve_mode = CURVE_POSITION_ARC_CENTER;
	    break;
	case 'E':
	case 'e':
	    TRACE(("end curve \"%s\"\n", fragment_to_tempstr(&optionarg)));
	    switch (state->curve_mode) {
	    case CURVE_POSITION_CLOSED_CURVE:
		{
		    int i;

#ifdef DEBUG_SPLINE_POINTS
		    printf("points: \n");
		    for (i = 0; i < (int) state->num_points; i++)
			printf("  %d,%d\n",
			       state->x_points[i], state->y_points[i]);
#endif

#ifdef DEBUG_SPLINE_WITH_ROTATION
		    {
			static int shift = 0;
			int temp_x[MAX_CURVE_POINTS], temp_y[MAX_CURVE_POINTS];
			shift++;
			shift = shift % state->num_points;
			for (i = 0; i < (int) state->num_points; i++) {
			    temp_x[i] = state->x_points[i];
			    temp_y[i] = state->y_points[i];
			}
			for (i = 0; i < (int) state->num_points; i++) {
			    state->x_points[i] = temp_x[(i + shift) % state->num_points];
			    state->y_points[i] = temp_y[(i + shift) % state->num_points];
			}

#ifdef DEBUG_SPLINE_POINTS
			printf("after shift %d: \n", shift);
			for (i = 0; i < (int) state->num_points; i++)
			    printf("  %d,%d\n",
				   state->x_points[i], state->y_points[i]);
#endif
		    }
#endif

		    for (i = (int) state->num_points; i > 0; i--) {
			state->x_points[i] = state->x_points[i - 1];
			state->y_points[i] = state->y_points[i - 1];
		    }
		    state->x_points[0] = state->x_points[state->num_points];
		    state->y_points[0] = state->y_points[state->num_points];
		    state->num_points++;
		    for (i = (int) state->num_points; i > 0; i--) {
			state->x_points[i] = state->x_points[i - 1];
			state->y_points[i] = state->y_points[i - 1];
		    }
		    state->x_points[0] = state->x_points[state->num_points - 1];
		    state->y_points[0] = state->y_points[state->num_points - 1];
		    state->num_points++;
		    state->x_points[state->num_points] = state->x_points[2];
		    state->y_points[state->num_points] = state->y_points[2];
		    state->num_points++;
#ifdef DEBUG_SPLINE_WITH_OVERDRAW
		    state->x_points[state->num_points] = state->x_points[3];
		    state->y_points[state->num_points] = state->y_points[3];
		    state->num_points++;
		    state->x_points[state->num_points] = state->x_points[4];
		    state->y_points[state->num_points] = state->y_points[4];
		    state->num_points++;
#endif
#ifdef DEBUG_SPLINE_POINTS
		    printf("after points added: \n");
		    for (i = 0; i < (int) state->num_points; i++)
			printf("  %d,%d\n",
			       state->x_points[i], state->y_points[i]);
#endif
		}
		TRACE(("drawing closed spline\n"));
		global_context = context;	/* FIXME: remove after updating spline code */
		plotCubicSpline((int) state->num_points - 1,
				state->x_points, state->y_points,
				1);
		break;
	    case CURVE_POSITION_OPEN_CURVE:
		TRACE(("drawing open spline\n"));
#ifdef DEBUG_SPLINE_POINTS
		{
		    int i;

		    printf("points: \n");
		    for (i = 0; i < (int) state->num_points; i++)
			printf("  %d,%d\n",
			       state->x_points[i], state->y_points[i]);
		}
#endif
		global_context = context;	/* FIXME: remove after updating spline code */
		plotCubicSpline((int) state->num_points - 1,
				state->x_points, state->y_points,
				1);
		break;
	    default:
		TRACE(("DATA_ERROR: end curve option unexpected \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		break;
	    }
	    break;
	case 'S':
	case 's':
	    TRACE(("begin open curve \"%s\"\n", fragment_to_tempstr(&optionarg)));
	    if (fragment_len(&optionarg)) {
		TRACE(("DATA_ERROR: invalid open curve option \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		break;
	    }
	    state->curve_mode = CURVE_POSITION_OPEN_CURVE;
	    state->num_points = 0U;
	    state->x_points[state->num_points] = context->graphics_output_cursor_x;
	    state->y_points[state->num_points] = context->graphics_output_cursor_y;
	    state->num_points++;
	    TRACE(("first point on curve with location %d,%d\n",
		   context->graphics_output_cursor_x,
		   context->graphics_output_cursor_y));
	    break;
	case 'W':
	case 'w':
	    TRACE(("found temporary write options \"%s\"\n",
		   fragment_to_tempstr(&optionarg)));
	    if (!load_regis_write_control_set(state, context->graphic,
					      context->graphics_output_cursor_x, context->graphics_output_cursor_y,
					      &optionarg, &context->temporary_write_controls)) {
		TRACE(("DATA_ERROR: invalid temporary write options \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		break;
	    }
	    break;
	default:
	    TRACE(("DATA_ERROR: ignoring unknown ReGIS curve command option '%c' arg \"%s\"\n",
		   state->option, fragment_to_tempstr(&optionarg)));
	    break;
	}
	break;
    case 'f':
	TRACE(("inspecting fill option \"%c\" with value \"%s\"\n",
	       state->option, fragment_to_tempstr(&optionarg)));
	switch (state->option) {
	case 'C':
	case 'c':
	    state->command = 'c';
	    state->option = '_';
	    break;
	case 'V':
	case 'v':
	    state->command = 'v';
	    state->option = '_';
	    break;
	case 'W':
	case 'w':
	    TRACE(("found temporary write options \"%s\"\n",
		   fragment_to_tempstr(&optionarg)));
	    if (!load_regis_write_control_set(state, context->graphic,
					      context->graphics_output_cursor_x, context->graphics_output_cursor_y,
					      &optionarg, &context->temporary_write_controls)) {
		TRACE(("DATA_ERROR: invalid temporary write options \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		break;
	    }
	    break;
	default:
	    TRACE(("DATA_ERROR: ignoring unknown ReGIS fill command option '%c' arg \"%s\"\n",
		   state->option, fragment_to_tempstr(&optionarg)));
	    break;
	}
	break;
    case 'l':
	TRACE(("inspecting load option \"%c\" with value \"%s\"\n",
	       state->option, fragment_to_tempstr(&optionarg)));
	/* FIXME: parse options */
	switch (state->option) {
	case 'A':
	case 'a':
	    TRACE(("found character specifier option \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	default:
	    TRACE(("DATA_ERROR: ignoring unknown ReGIS load command option '%c' arg \"%s\"\n",
		   state->option, fragment_to_tempstr(&optionarg)));
	    break;
	}
	break;
    case 'p':
	TRACE(("inspecting position option \"%c\" with value \"%s\"\n",
	       state->option, fragment_to_tempstr(&optionarg)));
	switch (state->option) {
	case 'B':
	case 'b':
	    TRACE(("found begin bounded position stack \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'E':
	case 'e':
	    TRACE(("found end position stack \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'S':
	case 's':
	    TRACE(("found begin unbounded position stack \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'W':
	case 'w':
	    TRACE(("found temporary write options \"%s\"\n",
		   fragment_to_tempstr(&optionarg)));
	    if (!load_regis_write_control_set(state, context->graphic,
					      context->graphics_output_cursor_x, context->graphics_output_cursor_y,
					      &optionarg, &context->temporary_write_controls)) {
		TRACE(("DATA_ERROR: invalid temporary write options \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
	    }
	    break;
	default:
	    TRACE(("DATA_ERROR: ignoring unknown ReGIS position command option '%c' arg \"%s\"\n",
		   state->option, fragment_to_tempstr(&optionarg)));
	    break;
	}
	break;
    case 'r':
	TRACE(("inspecting report option \"%c\" with value \"%s\"\n",
	       state->option, fragment_to_tempstr(&optionarg)));
	switch (state->option) {
	case 'E':
	case 'e':
	    TRACE(("found parse error report \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'I':
	case 'i':
	    TRACE(("found set input mode \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'L':
	case 'l':
	    TRACE(("found character set report \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'M':
	case 'm':
	    TRACE(("found macrograph report \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'P':
	case 'p':
	    TRACE(("found cursor position report \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	default:
	    TRACE(("DATA_ERROR: ignoring unknown ReGIS report command option '%c' arg \"%s\"\n",
		   state->option, fragment_to_tempstr(&optionarg)));
	    break;
	}
	break;
    case 's':
	TRACE(("inspecting screen option \"%c\" with value \"%s\"\n",
	       state->option, fragment_to_tempstr(&optionarg)));
	switch (state->option) {
	case 'A':
	case 'a':
	    TRACE(("found address definition \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    if (!fragment_len(&optionarg)) {
		TRACE(("DATA_ERROR: ignoring malformed ReGIS screen address definition option value \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		return 0;
	    }
	    break;
	case 'C':
	case 'c':
	    TRACE(("found cursor control \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    if (!fragment_len(&optionarg)) {
		TRACE(("DATA_ERROR: ignoring malformed ReGIS screen cursor control option value \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		return 0;
	    }
	    break;
	case 'E':
	case 'e':
	    TRACE(("found erase request \"%s\"\n", fragment_to_tempstr(&optionarg)));
	    if (fragment_len(&optionarg)) {
		TRACE(("DATA_ERROR: ignoring unexpected argument to ReGIS screen erase option \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		return 0;
	    }
	    draw_solid_rectangle(context->graphic, 0, 0,
				 context->graphic->actual_width - 1,
				 context->graphic->actual_height - 1,
				 context->background);
	    break;
	case 'H':
	case 'h':
	    TRACE(("found hardcopy control \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    if (!fragment_len(&optionarg)) {
		TRACE(("DATA_ERROR: ignoring malformed ReGIS screen hardcopy control option value \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		return 0;
	    }
	    break;
	case 'I':
	case 'i':
	    TRACE(("found screen background color index \"%s\"\n",
		   fragment_to_tempstr(&optionarg)));
	    if (!load_regis_colorspec(context->graphic, &optionarg, &context->background)) {
		TRACE(("DATA_ERROR: screen background color specifier not recognized: \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		return 0;
	    }
	    break;
	case 'M':
	case 'm':
	    TRACE(("found screen color register mapping \"%s\"\n",
		   fragment_to_tempstr(&optionarg)));
	    {
		RegisDataFragment regnum;
		RegisDataFragment colorspec;
		char ch;

		while (fragment_len(&optionarg)) {
		    if (skip_regis_whitespace(&optionarg))
			continue;
		    if (extract_regis_num(&optionarg, &regnum)) {
			int register_num;
			int color_only;
			short r, g, b;

			if (!regis_num_to_int(&regnum, &register_num)) {
			    TRACE(("DATA_ERROR: unable to parse int in screen color register mapping option: \"%s\"\n",
				   fragment_to_tempstr(&regnum)));
			    return 0;
			}
			if (register_num < 0 ||
			    register_num > (int) context->graphic->valid_registers) {
			    TRACE(("interpreting out of range register number %d as 0 FIXME\n", register_num));
			    register_num = 0;
			}
			skip_regis_whitespace(&optionarg);
			if (!extract_regis_optionset(&optionarg, &colorspec)) {
			    TRACE(("DATA_ERROR: expected to find optionset after register number: \"%s\"\n",
				   fragment_to_tempstr(&optionarg)));
			    return 0;
			}

			switch (peek_fragment(&colorspec)) {
			case 'A':
			case 'a':
			    pop_fragment(&colorspec);
			    color_only = 1;
			    break;
			default:
			    color_only = 0;
			    break;
			}

			TRACE(("mapping register %d to color spec: \"%s\"\n",
			       register_num, fragment_to_tempstr(&colorspec)));
			if (fragment_len(&colorspec) == 1) {
			    short l;
			    ch = pop_fragment(&colorspec);

			    TRACE(("got regis RGB colorspec pattern: \"%s\"\n",
				   fragment_to_tempstr(&colorspec)));
			    switch (ch) {
			    case 'D':
			    case 'd':
				r = 0;
				g = 0;
				b = 0;
				l = 0;
				break;
			    case 'R':
			    case 'r':
				r = 100;
				g = 0;
				b = 0;
				l = 46;
				break;
			    case 'G':
			    case 'g':
				r = 0;
				g = 100;
				b = 0;
				l = 50;
				break;
			    case 'B':
			    case 'b':
				r = 0;
				g = 0;
				b = 100;
				l = 50;
				break;
			    case 'C':
			    case 'c':
				r = 0;
				g = 100;
				b = 100;
				l = 50;
				break;
			    case 'Y':
			    case 'y':
				r = 100;
				g = 100;
				b = 0;
				l = 50;
				break;
			    case 'M':
			    case 'm':
				r = 100;
				g = 0;
				b = 100;
				l = 50;
				break;
			    case 'W':
			    case 'w':
				r = 100;
				g = 100;
				b = 100;
				l = 100;
				break;
			    default:
				TRACE(("unknown RGB color name: \"%c\"\n", ch));
				return 0;
			    }
			    if (context->terminal_id == 240 ||
				context->terminal_id == 330) {
				/* The VT240 and VT330 models force saturation to zero. */
				hls2rgb(0, l, 0, &r, &g, &b);
			    }
			} else {
			    short h, l, s;

			    if (sscanf(fragment_to_tempstr(&colorspec),
				       "%*1[Hh]%hd%*1[Ll]%hd%*1[Ss]%hd",
				       &h, &l, &s) != 3) {
				h = 0;
				s = 0;
				if (sscanf(fragment_to_tempstr(&colorspec),
					   "%*1[Ll]%hd", &l) != 1) {
				    TRACE(("unrecognized colorspec: \"%s\"\n",
					   fragment_to_tempstr(&colorspec)));
				    return 0;
				}
			    }
			    if (context->terminal_id == 240 ||
				context->terminal_id == 330) {
				/* The VT240 and VT330 models force saturation to zero. */
				h = 0;
				s = 0;
			    }
			    hls2rgb(h, l, s, &r, &g, &b);
			}

			if (color_only &&
			    (context->terminal_id == 240 ||
			     context->terminal_id == 330))
			    continue;
			update_color_register(context->graphic,
					      (RegisterNum) register_num,
					      r, g, b);
			continue;
		    }

		    TRACE(("DATA_ERROR: ignoring unexpected character in ReGIS screen color register mapping value \"%c\"\n",
			   pop_fragment(&optionarg)));
		    return 0;
		}
	    }
	    break;
	case 'P':
	case 'p':
	    TRACE(("found graphics page request \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    if (!fragment_len(&optionarg)) {
		TRACE(("DATA_ERROR: ignoring malformed ReGIS screen graphics page option value \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		return 0;
	    }
	    break;
	case 'T':
	case 't':
	    TRACE(("found time delay \"%s\" FIXME\n", fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    if (!fragment_len(&optionarg)) {
		TRACE(("DATA_ERROR: ignoring malformed ReGIS screen time delay option value \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		return 0;
	    }
	    break;
	case 'W':
	case 'w':
	    TRACE(("found PV \"%s\" FIXME\n", fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    if (!fragment_len(&optionarg)) {
		TRACE(("DATA_ERROR: ignoring malformed ReGIS screen PV option value \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
		return 0;
	    }
	    break;
	default:
	    TRACE(("DATA_ERROR: ignoring unknown ReGIS screen command option '%c' arg \"%s\"\n",
		   state->option, fragment_to_tempstr(&optionarg)));
	    break;
	}
	break;
    case 't':
	TRACE(("inspecting text option \"%c\" with value \"%s\"\n",
	       state->option, fragment_to_tempstr(&optionarg)));
	if (!fragment_len(&optionarg)) {
	    TRACE(("DATA_ERROR: ignoring malformed ReGIS text command option value \"%s\"\n",
		   fragment_to_tempstr(&optionarg)));
	    return 0;
	}
	switch (state->option) {
	case 'A':
	case 'a':
	    TRACE(("found character set specifier \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'B':
	case 'b':
	    TRACE(("found beginning of temporary text control \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'D':
	case 'd':
	    TRACE(("found string tilt control \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'E':
	case 'e':
	    TRACE(("found end of temporary text control \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'H':
	case 'h':
	    TRACE(("found height multiplier \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'I':
	case 'i':
	    TRACE(("found italic control \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'M':
	case 'm':
	    TRACE(("found size multiplier \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'S':
	case 's':
	    TRACE(("found custom display cell size \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'U':
	case 'u':
	    TRACE(("found custom display unit size \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	default:
	    TRACE(("DATA_ERROR: ignoring unknown ReGIS text command option '%c' arg \"%s\"\n",
		   state->option, fragment_to_tempstr(&optionarg)));
	    break;
	}
	break;
    case 'v':
	TRACE(("inspecting vector option \"%c\" with value \"%s\"\n",
	       state->option, fragment_to_tempstr(&optionarg)));
	switch (state->option) {
	case 'B':
	case 'b':
	    TRACE(("found begin bounded position stack \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'E':
	case 'e':
	    TRACE(("found end position stack \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'S':
	case 's':
	    TRACE(("found begin unbounded position stack \"%s\" FIXME\n",
		   fragment_to_tempstr(&optionarg)));
	    /* FIXME: handle */
	    break;
	case 'W':
	case 'w':
	    TRACE(("found temporary write options \"%s\"\n",
		   fragment_to_tempstr(&optionarg)));
	    if (!load_regis_write_control_set(state, context->graphic,
					      context->graphics_output_cursor_x, context->graphics_output_cursor_y,
					      &optionarg, &context->temporary_write_controls)) {
		TRACE(("DATA_ERROR: invalid temporary write options \"%s\"\n",
		       fragment_to_tempstr(&optionarg)));
	    }
	    break;
	default:
	    TRACE(("DATA_ERROR: ignoring unknown ReGIS vector command option '%c' arg \"%s\"\n",
		   state->option, fragment_to_tempstr(&optionarg)));
	    break;
	}
	break;
    case 'w':
	TRACE(("inspecting write option \"%c\" with value \"%s\"\n",
	       state->option, fragment_to_tempstr(&optionarg)));
	if (!load_regis_write_control(state, context->graphic,
				      context->graphics_output_cursor_x, context->graphics_output_cursor_y,
				      state->option, &optionarg, &context->persistent_write_controls)) {
	    TRACE(("DATA_ERROR: invalid write options"));
	}
	break;
    default:
	TRACE(("DATA_ERROR: unexpected option in \"%c\" command: \"%s\"\n",
	       state->command, fragment_to_tempstr(&optionarg)));
	return 0;
    }

    return 1;
}

static int
parse_regis_items(RegisParseState *state, RegisGraphicsContext *context)
{
    RegisDataFragment *input;
    RegisDataFragment item;

    switch (state->level) {
    case INPUT:
	input = &state->input;
	break;
    case OPTIONSET:
	input = &state->optionset;
	break;
    default:
	TRACE(("invalid parse level: %d\n", state->level));
	return 0;
    }

    if (input->pos >= input->len)
	return 0;

    if (extract_regis_extent(input, &item)) {
	TRACE(("found extent \"%s\"\n", fragment_to_tempstr(&item)));
	switch (state->command) {
	case 'c':
	    {
		int orig_x, orig_y;
		int new_x, new_y;

		if (state->num_points > 0) {
		    orig_x = state->x_points[state->num_points - 1];
		    orig_y = state->y_points[state->num_points - 1];
		} else {
		    orig_x = context->graphics_output_cursor_x;
		    orig_y = context->graphics_output_cursor_y;
		}
		if (!load_regis_extent(fragment_to_tempstr(&item),
				       orig_x, orig_y,
				       &new_x, &new_y)) {
		    TRACE(("DATA_ERROR: unable to parse extent in '%c' command: \"%s\"\n",
			   state->command, fragment_to_tempstr(&item)));
		    break;
		}

		switch (state->curve_mode) {
		case CURVE_POSITION_ARC_CENTER:
		case CURVE_POSITION_ARC_EDGE:
		    {
			double radians;
			int degrees;
			int c_x, c_y;
			int e_x, e_y;

			if (state->curve_mode == CURVE_POSITION_ARC_CENTER) {
			    c_x = new_x;
			    c_y = new_y;
			    e_x = orig_x;
			    e_y = orig_y;
			} else {
			    c_x = orig_x;
			    c_y = orig_y;
			    e_x = new_x;
			    e_y = new_y;
			}

			radians = atan2((double) (new_y - orig_y),
					(double) (new_x - orig_x));
			degrees = (int) (360.0 * radians / (2.0 * M_PI));
			if (degrees < 0)
			    degrees += 360;

			TRACE(("drawing arc centered at location %d,%d to location %d,%d from %d degrees for %d degrees\n",
			       c_x, c_y,
			       e_x, e_y,
			       degrees, state->arclen));
			draw_patterned_arc(context,
					   c_x, c_y,
					   e_x, e_y,
					   degrees, state->arclen);
		    }
		    break;
		case CURVE_POSITION_OPEN_CURVE:
		case CURVE_POSITION_CLOSED_CURVE:
		    if (state->num_points >= MAX_INPUT_CURVE_POINTS) {
			TRACE(("DATA_ERROR: got curve point, but already have max points (%d)\n", state->num_points));
			break;
		    }
		    state->x_points[state->num_points] = new_x;
		    state->y_points[state->num_points] = new_y;
		    state->num_points++;
		    TRACE(("adding point to curve with location %d,%d\n",
			   new_x, new_y));
		    break;
		default:
		    TRACE(("ERROR: got position, but curve mode %d is unknown\n", state->curve_mode));
		    break;
		}
	    }
	    break;
	case 'p':
	    /* FIXME TRACE(("DATA_ERROR: ignoring pen command with no location\n")); */
	    if (!load_regis_extent(fragment_to_tempstr(&item),
				   context->graphics_output_cursor_x, context->graphics_output_cursor_y,
				   &context->graphics_output_cursor_x, &context->graphics_output_cursor_y)) {
		TRACE(("DATA_ERROR: unable to parse extent in '%c' command: \"%s\"\n",
		       state->command, fragment_to_tempstr(&item)));
		break;
	    }
	    TRACE(("moving pen to location %d,%d\n",
		   context->graphics_output_cursor_x,
		   context->graphics_output_cursor_y));
	    break;
	case 's':
	    /* FIXME: parse, handle */
	    TRACE(("extent in screen command FIXME\n"));
	    break;
	case 't':
	    /* FIXME: parse, handle */
	    TRACE(("extent in text command FIXME\n"));
	    break;
	case 'v':
	    {
		int orig_x, orig_y;

		orig_x = context->graphics_output_cursor_x;
		orig_y = context->graphics_output_cursor_y;
		if (!load_regis_extent(fragment_to_tempstr(&item),
				       orig_x, orig_y,
				       &context->graphics_output_cursor_x, &context->graphics_output_cursor_y)) {
		    TRACE(("DATA_ERROR: unable to parse extent in '%c' command: \"%s\"\n",
			   state->command, fragment_to_tempstr(&item)));
		    break;
		}
		TRACE(("drawing line to location %d,%d\n",
		       context->graphics_output_cursor_x,
		       context->graphics_output_cursor_y));
		draw_patterned_line(context,
				    orig_x, orig_y,
				    context->graphics_output_cursor_x,
				    context->graphics_output_cursor_y);
	    }
	    break;
	default:
	    TRACE(("DATA_ERROR: unexpected extent in \"%c\" command: \"%s\"\n",
		   state->command, fragment_to_tempstr(&item)));
	    break;
	}
	return 1;
    }

    if (extract_regis_pixelvector(input, &item)) {
	TRACE(("found pixel vector \"%s\"\n", fragment_to_tempstr(&item)));
	switch (state->command) {
	case 'c':
	    /* FIXME: parse, handle */
	    TRACE(("pixelvector in curve command FIXME\n"));
	    break;
	    /* FIXME: not sure if 'f' supports pvs */
	case 'p':
	    /* FIXME: error checking */
	    if (!load_regis_pixelvector(fragment_to_tempstr(&item), context->temporary_write_controls.pv_multiplier,
					context->graphics_output_cursor_x, context->graphics_output_cursor_y,
					&context->graphics_output_cursor_x, &context->graphics_output_cursor_y)) {
		TRACE(("DATA_ERROR: unable to parse pixel vector in '%c' command: \"%s\"\n",
		       state->command, fragment_to_tempstr(&item)));
		break;
	    }
	    TRACE(("moving pen to location %d,%d\n",
		   context->graphics_output_cursor_x,
		   context->graphics_output_cursor_y));
	    break;
	case 's':
	    /* FIXME: parse, handle scroll argument */
	    TRACE(("pixelvector in screen command FIXME\n"));
	    break;
	case 't':
	    /* FIXME: parse, handle */
	    TRACE(("pixelvector in text command FIXME\n"));
	    break;
	case 'v':
	    /* FIXME: error checking */
	    {
		int orig_x, orig_y;

		orig_x = context->graphics_output_cursor_x;
		orig_y = context->graphics_output_cursor_y;
		if (!load_regis_pixelvector(fragment_to_tempstr(&item), context->temporary_write_controls.pv_multiplier,
					    orig_x, orig_y,
					    &context->graphics_output_cursor_x, &context->graphics_output_cursor_y)) {
		    TRACE(("DATA_ERROR: unable to parse pixel vector in '%c' command: \"%s\"\n",
			   state->command, fragment_to_tempstr(&item)));
		    break;
		}
		TRACE(("drawing line to location %d,%d\n",
		       context->graphics_output_cursor_x,
		       context->graphics_output_cursor_y));
		draw_patterned_line(context, orig_x, orig_y,
				    context->graphics_output_cursor_x,
				    context->graphics_output_cursor_y);
	    }
	    break;
	default:
	    TRACE(("DATA_ERROR: unexpected pixel vector in \"%c\" command: \"%s\"\n",
		   state->command, fragment_to_tempstr(&item)));
	    break;
	}
	return 1;
    }

    if (extract_regis_string(input, state->temp, state->templen)) {
	switch (state->command) {
	case 'l':
	    TRACE(("found character to load: \"%s\" FIXME\n", state->temp));
	    /* FIXME: handle */
	case 't':
	    TRACE(("found string to draw: \"%s\" FIXME\n", state->temp));
	    /* FIXME: handle */
	    break;
	default:
	    TRACE(("DATA_ERROR: unexpected string in \"%c\" command: \"%s\"\n",
		   state->command, state->temp));
	    break;
	}
	return 1;
    }

    /* hex values */
    if (state->command == 'l') {
	char ch1 = peek_fragment(input);
	char ch2 = peek_fragment(input);
	if ((ch1 == '0' ||
	     ch1 == '1' ||
	     ch1 == '2' ||
	     ch1 == '3' ||
	     ch1 == '4' ||
	     ch1 == '5' ||
	     ch1 == '6' ||
	     ch1 == '7' ||
	     ch1 == '8' ||
	     ch1 == '9' ||
	     ch1 == 'a' ||
	     ch1 == 'b' ||
	     ch1 == 'c' ||
	     ch1 == 'd' ||
	     ch1 == 'e' ||
	     ch1 == 'f' ||
	     ch1 == 'A' ||
	     ch1 == 'B' ||
	     ch1 == 'C' ||
	     ch1 == 'D' ||
	     ch1 == 'E' ||
	     ch1 == 'F') &&
	    (ch2 == '0' ||
	     ch2 == '1' ||
	     ch2 == '2' ||
	     ch2 == '3' ||
	     ch2 == '4' ||
	     ch2 == '5' ||
	     ch2 == '6' ||
	     ch2 == '7' ||
	     ch2 == '8' ||
	     ch2 == '9' ||
	     ch2 == 'a' ||
	     ch2 == 'b' ||
	     ch2 == 'c' ||
	     ch2 == 'd' ||
	     ch2 == 'e' ||
	     ch2 == 'f' ||
	     ch2 == 'A' ||
	     ch2 == 'B' ||
	     ch2 == 'C' ||
	     ch2 == 'D' ||
	     ch2 == 'E' ||
	     ch2 == 'F')) {
	    /* FIXME: handle */
	    TRACE(("found hex number: \"%c%c\" FIXME\n", ch1, ch2));
	    pop_fragment(input);
	    pop_fragment(input);
	    if (peek_fragment(input) == ',')
		pop_fragment(input);
	    return 1;
	}
    }

    return 0;
}

/*
 * context:
 * two pages of 800x480
 * current page #
 * current command
 * persistent write options
 * temporary write options
 * output position stack
 */
void
parse_regis(XtermWidget xw, ANSI *params, char const *string)
{
    TScreen *screen = TScreenOf(xw);
    RegisGraphicsContext context;
    RegisParseState state;
    unsigned iterations;
    int charrow = 0;
    int charcol = 0;
    unsigned type = 1;		/* FIXME: use page number */

    (void) xw;
    (void) string;
    (void) params;

    TRACE(("ReGIS vector graphics mode, params=%d\n", params->a_nparam));

    init_fragment(&state.input, string);
    init_fragment(&state.optionset, "");
    state.level = INPUT;
    state.templen = (unsigned) strlen(string) + 1U;
    if (!(state.temp = malloc((size_t) state.templen))) {
	TRACE(("Unable to allocate temporary buffer of size %u\n", state.templen));
	return;
    }
    state.command = '_';
    state.option = '_';

    memset(&context, 0, sizeof(context));

    context.graphic = get_new_or_matching_graphic(xw,
						  charrow, charcol,
						  800, 480,
						  type);
    init_regis_graphics_context(screen->terminal_id, &context);
    context.graphic->valid = 1;
    context.graphic->dirty = 1;
    refresh_modified_displayed_graphics(screen);

    iterations = 0U;
    for (;;) {
	state.level = INPUT;
	TRACE(("parsing at top level: %d of %d (next char %c)\n",
	       state.input.pos,
	       state.input.len,
	       peek_fragment(&state.input)));
	if (skip_regis_whitespace(&state.input))
	    continue;
	iterations++;
	if (parse_regis_command(&state)) {
	    if (iterations > ITERATIONS_BEFORE_REFRESH) {
		iterations = 0U;
		refresh_modified_displayed_graphics(screen);
	    }
	    context.graphic->dirty = 1;
	    /* FIXME: verify that these are the things reset on a new command */
	    copy_regis_write_controls(&context.persistent_write_controls, &context.temporary_write_controls);
	    context.pattern_count = 0U;
	    context.pattern_bit = 1U;
	    continue;
	}
	if (parse_regis_optionset(&state)) {
	    state.level = OPTIONSET;
	    TRACE(("parsing at optionset level: %d of %d\n",
		   state.optionset.pos,
		   state.optionset.len));
	    for (;;) {
		if (state.optionset.pos >= state.optionset.len)
		    break;
		TRACE(("looking at optionset character: \"%c\"\n",
		       peek_fragment(&state.optionset)));
		if (skip_regis_whitespace(&state.optionset))
		    continue;
		if (peek_fragment(&state.optionset) == ',') {
		    pop_fragment(&state.optionset);
		    continue;
		}
		if (parse_regis_option(&state, &context))
		    continue;
		if (parse_regis_items(&state, &context))
		    continue;
		if (state.optionset.pos >= state.optionset.len)
		    break;
		TRACE(("DATA_ERROR: skipping unknown token in optionset: \"%c\"\n",
		       pop_fragment(&state.optionset)));
		/* FIXME: suboptions */
	    }
	    state.option = '_';
	    continue;
	}
	if (parse_regis_items(&state, &context))
	    continue;
	if (state.optionset.pos >= state.optionset.len)
	    break;
	TRACE(("DATA_ERROR: skipping unknown token at top level: \"%c\"\n",
	       pop_fragment(&state.input)));
    }

    free(state.temp);

    refresh_modified_displayed_graphics(screen);
    TRACE(("DONE! Successfully parsed ReGIS data.\n"));
}
