#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xutil.h> /* for XDestroyImage */

#include "test.h"

static void draw_string(struct test_display *t, Drawable d, uint8_t alu,
			int x, int y, uint32_t fg, uint32_t bg, int s, int fill)
{
	const char *strings[] = {
		"Hello",
		"World",
		"Cairo's twin is Giza",
	};
	XGCValues val;
	GC gc;

	val.function = alu;
	val.foreground = fg;
	val.background = bg;

	gc = XCreateGC(t->dpy, d, GCForeground | GCBackground | GCFunction, &val);
	if (fill)
		XDrawImageString(t->dpy, d, gc, x, y, strings[s%3], strlen(strings[s%3]));
	else
		XDrawString(t->dpy, d, gc, x, y, strings[s%3], strlen(strings[s%3]));
	XFreeGC(t->dpy, gc);
}

static void clear(struct test_display *dpy, struct test_target *tt)
{
	XRenderColor render_color = {0};
	XRenderFillRectangle(dpy->dpy, PictOpClear, tt->picture, &render_color,
			     0, 0, tt->width, tt->height);
}

static void string_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target real, ref;
	int r, s;

	printf("Testing general (%s): ", test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->real, target, &real);
	clear(&t->real, &real);

	test_target_create_render(&t->ref, target, &ref);
	clear(&t->ref, &ref);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x = rand() % (2*real.width) - real.width;
			int y = rand() % (2*real.height) - real.height;
			uint8_t alu = rand() % (GXset + 1);
			uint32_t fg = rand();
			uint32_t bg = rand();
			int str = rand();
			int fill = rand() & 1;

			draw_string(&t->real, real.draw, alu, x, y, fg, bg, str, fill);
			draw_string(&t->ref, ref.draw, alu, x, y, fg, bg, str, fill);
		}

		test_compare(t,
			     real.draw, real.format,
			     ref.draw, ref.format,
			     0, 0, real.width, real.height,
			     "");
	}

	printf("passed [%d iterations x %d]\n", reps, sets);

	test_target_destroy_render(&t->real, &real);
	test_target_destroy_render(&t->ref, &ref);
}

int main(int argc, char **argv)
{
	struct test test;
	int i;

	test_init(&test, argc, argv);

	for (i = 0; i <= DEFAULT_ITERATIONS; i++) {
		int reps = 1 << i;
		int sets = 1 << (12 - i);
		enum target t;

		if (sets < 2)
			sets = 2;

		for (t = TARGET_FIRST; t <= TARGET_LAST; t++) {
			string_tests(&test, reps, sets, t);
		}
	}

	return 0;
}
