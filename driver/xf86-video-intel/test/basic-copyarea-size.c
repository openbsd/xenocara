#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xutil.h> /* for XDestroyImage */

#include "test.h"

#define SIZE 20000
struct draw {
	Pixmap a, b;
	GC gc;
	XRenderPictFormat *format;
};

static void target_init(struct test_display *t, struct draw *tt, int size)
{
	XGCValues val;

	tt->a = XCreatePixmap(t->dpy, DefaultRootWindow(t->dpy),
			      size, size, 32);
	tt->b = XCreatePixmap(t->dpy, DefaultRootWindow(t->dpy),
			      size, size, 32);

	val.graphics_exposures = 0;
	tt->gc = XCreateGC(t->dpy, tt->a, GCGraphicsExposures, &val);

	tt->format = XRenderFindStandardFormat(t->dpy, PictStandardARGB32);

	val.foreground = 0xffff0000;
	XChangeGC(t->dpy, tt->gc, GCForeground, &val);
	XFillRectangle(t->dpy, tt->a, tt->gc, 0, 0, size, size);

	val.foreground = 0xff0000ff;
	XChangeGC(t->dpy, tt->gc, GCForeground, &val);
	XFillRectangle(t->dpy, tt->b, tt->gc, 0, 0, size, size);
}

static void target_fini(struct test_display *t, struct draw *tt)
{
	XFreePixmap(t->dpy, tt->a);
	XFreePixmap(t->dpy, tt->b);
}

int main(int argc, char **argv)
{
	struct test test;
	struct draw out, ref;
	int size, i;

	test_init(&test, argc, argv);

	/* Copy back and forth betwenn two pixmaps, gradually getting larger */
	for (size = 1; size <= SIZE; size = (size * 3 + 1) / 2) {
		target_init(&test.out, &out, size);
		target_init(&test.ref, &ref, size);

		printf("size=%d\n", size);
		for (i = 0; i <= DEFAULT_ITERATIONS; i++) {
			int reps = REPS(i);
			do {
				int sx = rand() % (2*size) - size;
				int sy = rand() % (2*size) - size;

				int dx = rand() % (2*size) - size;
				int dy = rand() % (2*size) - size;

				int order = rand() & 1;

				XCopyArea(test.out.dpy,
					  order ? out.a : out.b,
					  (!order) ? out.a : out.b,
					  out.gc,
					  sx, sy,
					  size, size,
					  dx, dy);

				XCopyArea(test.ref.dpy,
					  order ? ref.a : ref.b,
					  (!order) ? ref.a : ref.b,
					  ref.gc,
					  sx, sy,
					  size, size,
					  dx, dy);
			} while (--reps);
		}

		test_compare(&test,
			     out.a, out.format,
			     ref.a, ref.format,
			     0, 0, size, size,
			     "");
		test_compare(&test,
			     out.b, out.format,
			     ref.b, ref.format,
			     0, 0, size, size,
			     "");

		target_fini(&test.out, &out);
		target_fini(&test.ref, &ref);
	}

	return 0;
}
