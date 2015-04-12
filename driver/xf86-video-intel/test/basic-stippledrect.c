#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "test.h"

static unsigned char bitmap4x4[] = {
	   0x03, 0x06, 0x0c, 0x09
};

static unsigned char bitmap8x8[3][8] = {
	{ 0xcc, 0x66, 0x33, 0x99, 0xcc, 0x66, 0x33, 0x99 },
	{ 0x00, 0xfe, 0x92, 0x92, 0xfe, 0x92, 0x92, 0xfe },
	{ 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa },
};

static void fill_rect(struct test_target *t, uint8_t alu,
		      XRectangle *clip, int nclip,
		      uint8_t stipple, uint8_t opaque, int tx, int ty,
		      int x, int y, int w, int h,
		      uint32_t fg, uint32_t bg)
{
	Display *dpy = t->dpy->dpy;
	XGCValues val;
	GC gc;

	val.function = alu;
	val.foreground = fg;
	val.background = bg;
	val.fill_style = opaque ? FillOpaqueStippled : FillStippled;
	val.ts_x_origin = tx;
	val.ts_y_origin = ty;
	if (stipple == 0) {
		val.stipple = XCreateBitmapFromData(dpy, t->draw, (char *)bitmap4x4, 4, 4);
	} else {
		char *b = (char *)bitmap8x8[stipple-1];
		val.stipple = XCreateBitmapFromData(dpy, t->draw, b, 8, 8);
	}

	gc = XCreateGC(dpy, t->draw, GCFillStyle | GCTileStipXOrigin | GCTileStipYOrigin | GCStipple | GCForeground | GCBackground | GCFunction, &val);
	if (nclip)
		XSetClipRectangles(dpy, gc, 0, 0, clip, nclip, Unsorted);
	XFillRectangle(dpy, t->draw, gc, x, y, w, h);
	XFreeGC(dpy, gc);
	XFreePixmap(dpy, val.stipple);
}

static void clear(struct test_target *tt)
{
	XRenderColor render_color = {0};
	XRenderFillRectangle(tt->dpy->dpy, PictOpClear, tt->picture, &render_color,
			     0, 0, tt->width, tt->height);
}

static void unclipped_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target out, ref;
	int r, s;

	printf("Testing unclipped stippled fills (%s): ", test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->out, target, &out);
	clear(&out);

	test_target_create_render(&t->ref, target, &ref);
	clear(&ref);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x = rand() % out.width;
			int y = rand() % out.height;
			int w = rand() % (out.width - x);
			int h = rand() % (out.height - y);
			int tx = rand() % (2*out.width) - out.width;
			int ty = rand() % (2*out.height) - out.height;
			uint8_t stipple = rand() % 4;
			uint8_t opaque = rand() % 1;
			uint8_t alu = rand() % (GXset + 1);
			uint32_t fg = rand();
			uint32_t bg = rand();

			fill_rect(&out, alu, NULL, 0,
				  stipple, opaque, tx, ty,
				  x, y, w, h,
				  fg, bg);
			fill_rect(&ref, alu, NULL, 0,
				  stipple, opaque, tx, ty,
				  x, y, w, h,
				  fg, bg);
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

static void simple_clip_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target out, ref;
	int r, s;

	printf("Testing simple clipped stippled fills (%s): ", test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->out, target, &out);
	clear(&out);

	test_target_create_render(&t->ref, target, &ref);
	clear(&ref);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x = rand() % (2*out.width) - out.width;
			int y = rand() % (2*out.height) - out.height;
			int w = rand() % (2*out.width);
			int h = rand() % (2*out.height);
			int tx = rand() % (2*out.width) - out.width;
			int ty = rand() % (2*out.height) - out.height;
			uint8_t stipple = rand() % 4;
			uint8_t opaque = rand() % 1;
			uint8_t alu = rand() % (GXset + 1);
			uint32_t fg = rand();
			uint32_t bg = rand();

			fill_rect(&out, alu, NULL, 0,
				  stipple, opaque, tx, ty,
				  x, y, w, h,
				  fg, bg);
			fill_rect(&ref, alu, NULL, 0,
				  stipple, opaque, tx, ty,
				  x, y, w, h,
				  fg, bg);
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

static void complex_clip_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target out, ref;
	XRectangle *clip;
	int nclip, r, s;

	printf("Testing complex clipped stippled fills (%s): ", test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->out, target, &out);
	clear(&out);

	test_target_create_render(&t->ref, target, &ref);
	clear(&ref);

	for (s = 0; s < sets; s++) {
		nclip = (rand() % 16) + 2;
		clip = malloc(sizeof(XRectangle)*nclip);
		for (r = 0; r < nclip; r++) {
			clip[r].x = rand() % out.width;
			clip[r].y = rand() % out.height;
			clip[r].width = rand() % (out.width - clip[r].x);
			clip[r].height = rand() % (out.height - clip[r].y);
		}

		for (r = 0; r < reps; r++) {
			int x = rand() % (2*out.width) - out.width;
			int y = rand() % (2*out.height) - out.height;
			int w = rand() % (2*out.width);
			int h = rand() % (2*out.height);
			int tx = rand() % (2*out.width) - out.width;
			int ty = rand() % (2*out.height) - out.height;
			uint8_t stipple = rand() % 4;
			uint8_t opaque = rand() % 1;
			uint8_t alu = rand() % (GXset + 1);
			uint32_t fg = rand();
			uint32_t bg = rand();

			fill_rect(&out, alu, clip, nclip,
				  stipple, opaque, tx, ty,
				  x, y, w, h,
				  fg, bg);
			fill_rect(&ref, alu, clip, nclip,
				  stipple, opaque, tx, ty,
				  x, y, w, h,
				  fg, bg);
		}

		test_compare(t,
			     out.draw, out.format,
			     ref.draw, ref.format,
			     0, 0, out.width, out.height,
			     "");

		free(clip);
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
			unclipped_tests(&test, reps, sets, t);
			simple_clip_tests(&test, reps, sets, t);
			complex_clip_tests(&test, reps, sets, t);
		}
	}

	return 0;
}
