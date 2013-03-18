#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xutil.h> /* for XDestroyImage */

#include "test.h"

#define SIZE 20000
struct draw {
	Pixmap a, b;
	Picture pa, pb;
	GC gc;
	XRenderPictFormat *format;
};

static void target_init(struct test_display *t, struct draw *tt, int size)
{
	XRenderColor color;

	tt->format = XRenderFindStandardFormat(t->dpy, PictStandardARGB32);

	tt->a = XCreatePixmap(t->dpy, DefaultRootWindow(t->dpy),
			      size, size, tt->format->depth);
	tt->pa = XRenderCreatePicture(t->dpy, tt->a, tt->format, 0, NULL);

	color.alpha = 0xffff;
	color.red = 0xffff;
	color.green = 0;
	color.blue = 0;
	XRenderFillRectangle(t->dpy, PictOpSrc, tt->pa, &color, 0, 0, size, size);

	tt->b = XCreatePixmap(t->dpy, DefaultRootWindow(t->dpy),
			      size, size, tt->format->depth);
	tt->pb = XRenderCreatePicture(t->dpy, tt->b, tt->format, 0, NULL);

	color.alpha = 0xffff;
	color.red = 0;
	color.green = 0;
	color.blue = 0xffff;
	XRenderFillRectangle(t->dpy, PictOpSrc, tt->pb, &color, 0, 0, size, size);
}

static void target_fini(struct test_display *t, struct draw *tt)
{
	XRenderFreePicture(t->dpy, tt->pa);
	XFreePixmap(t->dpy, tt->a);

	XRenderFreePicture(t->dpy, tt->pb);
	XFreePixmap(t->dpy, tt->b);
}

int main(int argc, char **argv)
{
	struct test test;
	struct draw real, ref;
	int size, i;

	test_init(&test, argc, argv);

	/* Copy back and forth betwenn two pixmaps, gradually getting larger */
	for (size = 1; size <= SIZE; size = (size * 3 + 1) / 2) {
		target_init(&test.real, &real, size);
		target_init(&test.ref, &ref, size);

		printf("size=%d\n", size);
		for (i = 0; i <= DEFAULT_ITERATIONS; i++) {
			int reps = 1 << i;
			do {
				int sx = rand() % (2*size) - size;
				int sy = rand() % (2*size) - size;

				int dx = rand() % (2*size) - size;
				int dy = rand() % (2*size) - size;

				int w = rand() % size;
				int h = rand() % size;

				int order = rand() & 1;

				XRenderComposite(test.real.dpy, PictOpSrc,
						 order ? real.pa : real.pb,
						 0,
						 (!order) ? real.pa : real.pb,
						 sx, sy,
						 0, 0,
						 dx, dy,
						 w, h);

				XRenderComposite(test.ref.dpy, PictOpSrc,
						 order ? ref.pa : ref.pb,
						 0,
						 (!order) ? ref.pa : ref.pb,
						 sx, sy,
						 0, 0,
						 dx, dy,
						 w, h);
			} while (--reps);
		}

		test_compare(&test,
			     real.a, real.format,
			     ref.a, ref.format,
			     0, 0, size, size,
			     "");
		test_compare(&test,
			     real.b, real.format,
			     ref.b, ref.format,
			     0, 0, size, size,
			     "");

		target_fini(&test.real, &real);
		target_fini(&test.ref, &ref);
	}

	return 0;
}
