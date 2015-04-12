#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xutil.h> /* for XDestroyImage */
#include <pixman.h> /* for pixman blt functions */

#include "test.h"

static const XPoint points[]= {
	/* top */
	{ 0, 0},
	{ 1, 0},
	{ 2, 0},
	{ 3, 0},
	{ 4, 0},
	{ 5, 0},
	{ 6, 0},
	{ 7, 0},
	{ 8, 0},
	/* right */
	{ 8, 1},
	{ 8, 2},
	{ 8, 3},
	{ 8, 4},
	{ 8, 5},
	{ 8, 6},
	{ 8, 7},
	{ 8, 8},
	/* bottom */
	{ 7, 8},
	{ 6, 8},
	{ 5, 8},
	{ 4, 8},
	{ 3, 8},
	{ 2, 8},
	{ 1, 8},
	{ 0, 8},
	/* left */
	{ 0, 7},
	{ 0, 6},
	{ 0, 5},
	{ 0, 4},
	{ 0, 3},
	{ 0, 2},
	{ 0, 1},
	{ 0, 0} /* and origin again for luck */
};
#define NUM_POINTS (sizeof(points)/sizeof(points[0]))

static void clear(struct test_display *dpy, struct test_target *tt)
{
	XRenderColor render_color = {0};
	XRenderFillRectangle(dpy->dpy, PictOpClear, tt->picture, &render_color,
			     0, 0, tt->width, tt->height);
}

static void draw_line(struct test_display *dpy, struct test_target *tt,
		      int alu, int width, int style, int cap,
		      const XPoint *p1, const XPoint *p2,
		      int dx, int dy)
{
	XGCValues val;
	GC gc;

	val.function = GXcopy;
	val.foreground = WhitePixel(dpy->dpy, 0);
	val.line_width = width;
	val.line_style = style;
	val.cap_style = cap;

	gc = XCreateGC(dpy->dpy, tt->draw,
		       GCForeground |
		       GCFunction |
		       GCLineWidth |
		       GCLineStyle |
		       GCCapStyle,
		       &val);
	XDrawLine(dpy->dpy, tt->draw, gc,
		  p1->x + dx, p1->y + dy,
		  p2->x + dx, p2->y + dy);
	XFreeGC(dpy->dpy, gc);
}

static void line_tests(struct test *t, enum target target)
{
	char buf[1024];
	struct test_target out, ref;
	int a, b, alu, lw, style, cap;

	printf("Testing drawing of single line segments (%s): ",
	       test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->out, target, &out);
	test_target_create_render(&t->ref, target, &ref);

	style = LineSolid;

	for (alu = 0; alu < 16; alu++) {
		for (cap = CapNotLast; cap <= CapProjecting; cap++) {
			for (lw = 0; lw <= 4; lw++) {
				for (a = 0; a < NUM_POINTS; a++) {
					for (b = 0; b < NUM_POINTS; b++) {
						sprintf(buf,
							"p1=(%d, %d), p2=(%d, %d), width=%d, cap=%d, alu=%d",
							points[a].x, points[a].y,
							points[b].x, points[b].y,
							lw, cap, alu);

						clear(&t->out, &out);
						clear(&t->ref, &ref);

						draw_line(&t->out, &out, alu, lw, style, cap,
							  &points[a], &points[b], 64, 64);
						draw_line(&t->ref, &ref, alu, lw, style, cap,
							  &points[a], &points[b], 64, 64);

						test_compare(t,
							     out.draw, out.format,
							     ref.draw, ref.format,
							     0, 0, out.width, out.height,
							     buf);
					}
				}
			}
		}
	}

	test_target_destroy_render(&t->out, &out);
	test_target_destroy_render(&t->ref, &ref);

	printf("\n");
}

int main(int argc, char **argv)
{
	struct test test;
	enum target t;

	test_init(&test, argc, argv);

	for (t = TARGET_FIRST; t <= TARGET_LAST; t++)
		line_tests(&test, t);

	return 0;
}
