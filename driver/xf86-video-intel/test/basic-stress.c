#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xutil.h> /* for XDestroyImage */
#include <pixman.h>

#include "test.h"

static void fill_rect(struct test_target *tt,
		      int alu, int color,
		      int x, int y, int w, int h)
{
	XGCValues val;

	val.function = alu;
	val.foreground = color;
	XChangeGC(tt->dpy->dpy, tt->gc, GCFunction | GCForeground, &val);

	XFillRectangle(tt->dpy->dpy, tt->draw, tt->gc, x, y, w, h);
}

static void clear(struct test_target *tt)
{
	fill_rect(tt,
		  GXcopy, 0,
		  0, 0, tt->width, tt->height);
}

static void fill(struct test_target *out,
		 struct test_target *ref)
{
	int x = rand() % (2*out->width) - out->width;
	int y = rand() % (2*out->height) - out->height;
	int w = rand() % (2*out->width);
	int h = rand() % (2*out->height);
	int color = rand();
	int alu = rand() % 16;

	fill_rect(out, alu, color, x, y, w, h);
	fill_rect(ref, alu, color, x, y, w, h);
}

static void copy(struct test_target *out,
		 struct test_target *ref)
{
	int sx = rand() % (2*out->width) - ref->width;
	int sy = rand() % (2*out->height) - ref->height;
	int dx = rand() % (2*out->width) - ref->width;
	int dy = rand() % (2*out->height) - ref->height;
	int w = rand() % (2*out->width);
	int h = rand() % (2*out->height);
	XGCValues val;

	val.function = rand() % 16;

	XChangeGC(out->dpy->dpy, out->gc, GCFunction, &val);
	XCopyArea(out->dpy->dpy,
		  out->draw, out->draw, out->gc,
		  sx, sy, w, h, dx, dy);

	XChangeGC(ref->dpy->dpy, ref->gc, GCFunction, &val);
	XCopyArea(ref->dpy->dpy,
		  ref->draw, ref->draw, ref->gc,
		  sx, sy, w, h, dx, dy);
}

static void _put(struct test_target *tt,
		 int x, int y, int w,int h, int color, int alu)
{
	XImage image;
	XGCValues val;

	val.function = alu;

	test_init_image(&image, &tt->dpy->shm, tt->format, w, h);
	pixman_fill((uint32_t*)image.data,
		    image.bytes_per_line/sizeof(uint32_t),
		    image.bits_per_pixel,
		    0, 0, w, h, color);

	XChangeGC(tt->dpy->dpy, tt->gc, GCFunction, &val);
	if (rand() & 1) {
		XShmPutImage(tt->dpy->dpy, tt->draw, tt->gc, &image,
			     0, 0, x, y, w, h, 0);
		XSync(tt->dpy->dpy, 1);
	} else {
		XPutImage(tt->dpy->dpy, tt->draw, tt->gc, &image,
			  0, 0, x, y, w, h);
	}
}

static void put(struct test_target *out,
		struct test_target *ref)
{
	int x = rand() % (2*out->width) - out->width;
	int y = rand() % (2*out->height) - out->height;
	int w = rand() % out->width;
	int h = rand() % out->height;
	int color = rand();
	int alu = rand() % 16;

	_put(out, x, y, w, h, color, alu);
	_put(ref, x, y, w, h, color, alu);
}

static void rect_tests(struct test *test, int iterations, enum target target)
{
	struct test_target out, ref;
	void (* const ops[])(struct test_target *, struct test_target *) = {
		copy,
		fill,
		put,
	};
	int n;

	printf("Running mixed ops stress against %s: ",
	       test_target_name(target));
	fflush(stdout);

	test_target_create_render(&test->out, target, &out);
	test_target_create_render(&test->ref, target, &ref);

	clear(&out);
	clear(&ref);

	for (n = 0; n < iterations; n++)
		ops[rand() % ARRAY_SIZE(ops)](&out, &ref);

	test_compare(test,
		     out.draw, out.format,
		     ref.draw, ref.format,
		     0, 0, out.width, out.height,
		     "");

	printf("passed [%d iterations]\n", n);

	test_target_destroy_render(&test->out, &out);
	test_target_destroy_render(&test->ref, &ref);
}

int main(int argc, char **argv)
{
	struct test test;
	int i;

	test_init(&test, argc, argv);

	for (i = 0; i <= DEFAULT_ITERATIONS; i++) {
		int iterations = REPS(i);
		rect_tests(&test, iterations, 0);
		rect_tests(&test, iterations, 1);
	}

	return 0;
}
