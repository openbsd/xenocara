#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xutil.h> /* for XDestroyImage */

#include "test.h"

static void draw_rect(struct test_display *t, Drawable d, uint8_t alu,
		      int x, int y, int w, int h, uint32_t fg, int lw)
{
	XGCValues val;
	GC gc;

	val.function = alu;
	val.foreground = fg;
	val.line_width = lw;

	gc = XCreateGC(t->dpy, d, GCForeground | GCFunction | GCLineWidth, &val);
	XDrawRectangle(t->dpy, d, gc, x, y, w, h);
	XFreeGC(t->dpy, gc);
}

static void clear(struct test_display *dpy, struct test_target *tt)
{
	XRenderColor render_color = {0};
	XRenderFillRectangle(dpy->dpy, PictOpClear, tt->picture, &render_color,
			     0, 0, tt->width, tt->height);
}

static void zrect_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target out, ref;
	int r, s;

	printf("Testing empty rects (%s): ", test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->out, target, &out);
	clear(&t->out, &out);

	test_target_create_render(&t->ref, target, &ref);
	clear(&t->ref, &ref);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x = rand() % (2*out.width) - out.width;
			int y = rand() % (2*out.height) - out.height;
			uint8_t alu = rand() % (GXset + 1);
			uint32_t fg = rand();
			uint32_t lw = rand() % 4;

			draw_rect(&t->out, out.draw, alu,
				  x, y, 0, 0, fg, lw);
			draw_rect(&t->ref, ref.draw, alu,
				  x, y, 0, 0, fg, lw);
		}

		test_compare(t,
			     out.draw, out.format,
			     ref.draw, ref.format,
			     0, 0, out.width, out.height,
			     "");
	}

	printf("passed [%d iterations x %d]\n", reps, sets);

	test_target_destroy_render(&t->out, &out);
	test_target_destroy_render(&t->ref, &ref);
}

static void hrect_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target out, ref;
	int r, s;

	printf("Testing horizontal rects (%s): ", test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->out, target, &out);
	clear(&t->out, &out);

	test_target_create_render(&t->ref, target, &ref);
	clear(&t->ref, &ref);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x = rand() % (2*out.width) - out.width;
			int y = rand() % (2*out.height) - out.height;
			int w = rand() % (2*out.width);
			uint8_t alu = rand() % (GXset + 1);
			uint32_t fg = rand();
			uint32_t lw = rand() % 4;

			draw_rect(&t->out, out.draw, alu,
				  x, y, w, 0, fg, lw);
			draw_rect(&t->ref, ref.draw, alu,
				  x, y, w, 0, fg, lw);
		}

		test_compare(t,
			     out.draw, out.format,
			     ref.draw, ref.format,
			     0, 0, out.width, out.height,
			     "");
	}

	printf("passed [%d iterations x %d]\n", reps, sets);

	test_target_destroy_render(&t->out, &out);
	test_target_destroy_render(&t->ref, &ref);
}

static void vrect_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target out, ref;
	int r, s;

	printf("Testing vertical rects (%s): ", test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->out, target, &out);
	clear(&t->out, &out);

	test_target_create_render(&t->ref, target, &ref);
	clear(&t->ref, &ref);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x = rand() % (2*out.width) - out.width;
			int y = rand() % (2*out.height) - out.height;
			int h = rand() % (2*out.width);
			uint8_t alu = rand() % (GXset + 1);
			uint32_t fg = rand();
			uint32_t lw = rand() % 4;

			draw_rect(&t->out, out.draw, alu,
				  x, y, 0, h, fg, lw);
			draw_rect(&t->ref, ref.draw, alu,
				  x, y, 0, h, fg, lw);
		}

		test_compare(t,
			     out.draw, out.format,
			     ref.draw, ref.format,
			     0, 0, out.width, out.height,
			     "");
	}

	printf("passed [%d iterations x %d]\n", reps, sets);

	test_target_destroy_render(&t->out, &out);
	test_target_destroy_render(&t->ref, &ref);
}

static void rect_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target out, ref;
	int r, s;

	printf("Testing general (%s): ", test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->out, target, &out);
	clear(&t->out, &out);

	test_target_create_render(&t->ref, target, &ref);
	clear(&t->ref, &ref);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x = rand() % (2*out.width) - out.width;
			int y = rand() % (2*out.height) - out.height;
			int w = rand() % (2*out.width);
			int h = rand() % (2*out.height);
			uint8_t alu = rand() % (GXset + 1);
			uint32_t fg = rand();
			uint32_t lw = rand() % 4;

			draw_rect(&t->out, out.draw, alu,
				  x, y, w, h, fg, lw);
			draw_rect(&t->ref, ref.draw, alu,
				  x, y, w, h, fg, lw);
		}

		test_compare(t,
			     out.draw, out.format,
			     ref.draw, ref.format,
			     0, 0, out.width, out.height,
			     "");
	}

	printf("passed [%d iterations x %d]\n", reps, sets);

	test_target_destroy_render(&t->out, &out);
	test_target_destroy_render(&t->ref, &ref);
}

int main(int argc, char **argv)
{
	struct test test;
	int i;

	test_init(&test, argc, argv);

	for (i = 0; i <= DEFAULT_ITERATIONS; i++) {
		int reps = REPS(i), sets = SETS(i);
		enum target t;

		for (t = TARGET_FIRST; t <= TARGET_LAST; t++) {
			zrect_tests(&test, reps, sets, t);
			hrect_tests(&test, reps, sets, t);
			vrect_tests(&test, reps, sets, t);
			rect_tests(&test, reps, sets, t);
		}
	}

	return 0;
}
