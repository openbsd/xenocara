#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "test.h"

static const unsigned char data[] = {
	0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
	0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
	0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
	0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55,
};

static struct bitmap {
	int width, height;
	struct cache {
		Display *dpy;
		Pixmap pixmap;
	} cached[2];
} bitmaps[] = {
	{ 1, 1, },
	{ 1, 2, },
	{ 2, 3, },
	{ 3, 2, },
	{ 4, 4, },
	{ 6, 6, },
	{ 8, 8, },
	{ 8, 4, },
	{ 8, 2, },
	{ 8, 1, },
	{ 4, 8, },
	{ 2, 8, },
	{ 1, 8, },
	{ 16, 16, },
	{ 15, 17, },
	{ 24, 24, },
	{ 32, 32, },
	{ 16, 8, },
	{ 16, 4, },
	{ 16, 2, },
	{ 16, 1, },
	{ 8, 16, },
	{ 4, 16, },
	{ 2, 16, },
	{ 1, 16, },
};

static void reset_cache(void)
{
	int n, m;

	for (n = 0; n < sizeof(bitmaps)/sizeof(bitmaps[0]); n++) {
		for (m = 0; m < 2; m++) {
			if (bitmaps[n].cached[m].dpy) {
				XFreePixmap(bitmaps[n].cached[m].dpy, bitmaps[n].cached[m].pixmap);
				bitmaps[n].cached[m].dpy = NULL;
			}
		}
	}
}

static void fill_rect(struct test_target *t, uint8_t alu,
		      XRectangle *clip, int nclip,
		      uint8_t tile, int tx, int ty,
		      int x, int y, int w, int h,
		      uint32_t fg, uint32_t bg)
{
	Display *dpy = t->dpy->dpy;
	struct bitmap *b = &bitmaps[(tile >> 1) % (sizeof(bitmaps)/sizeof(bitmaps[0]))];
	XGCValues val;
	GC gc;
	int n;

	val.function = alu;
	val.function = GXcopy;
	val.fill_style = FillTiled;
	val.ts_x_origin = tx;
	val.ts_y_origin = ty;
	if (tile & 1) {
		val.tile = 0;
		for (n = 0; n < 2; n++) {
			if (b->cached[n].dpy == dpy) {
				val.tile = b->cached[n].pixmap;
				break;
			}
		}
		if (val.tile == 0) {
			val.tile = XCreatePixmapFromBitmapData(dpy, t->draw,
							       (char *)data, b->width, b->height,
							       fg, bg, t->depth);
			for (n = 0; n < 2; n++) {
				if (b->cached[n].dpy == NULL) {
					b->cached[n].dpy = dpy;
					b->cached[n].pixmap = val.tile;
					break;
				}
			}
		}
	} else
		val.tile = XCreatePixmapFromBitmapData(dpy, t->draw,
						       (char *)data, b->width, b->height,
						       fg, bg, t->depth);

	gc = XCreateGC(dpy, t->draw, GCFillStyle | GCTileStipXOrigin | GCTileStipYOrigin | GCTile | GCFunction, &val);
	if (nclip)
		XSetClipRectangles(dpy, gc, 0, 0, clip, nclip, Unsorted);
	XFillRectangle(dpy, t->draw, gc, x, y, w, h);
	XFreeGC(dpy, gc);
	if ((tile & 1) == 0)
		XFreePixmap(dpy, val.tile);
}

static void clear(struct test_target *tt)
{
	XRenderColor render_color = {0};
	XRenderFillRectangle(tt->dpy->dpy, PictOpClear, tt->picture, &render_color,
			     0, 0, tt->width, tt->height);
}

static void small_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target out, ref;
	int r, s;

	printf("Testing small tiled fills (%s): ", test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->out, target, &out);
	clear(&out);

	test_target_create_render(&t->ref, target, &ref);
	clear(&ref);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x = rand() % out.width;
			int y = rand() % out.height;
			int w = rand() % out.width;
			int h = rand() % 8;
			int tx = rand() % (2*out.width) - out.width;
			int ty = rand() % (2*out.height) - out.height;
			uint8_t tile = rand();
			uint8_t alu = rand() % (GXset + 1);
			uint32_t fg = rand();
			uint32_t bg = rand();

			fill_rect(&out, alu, NULL, 0,
				  tile, tx, ty,
				  x, y, w, h,
				  fg, bg);
			fill_rect(&ref, alu, NULL, 0,
				  tile, tx, ty,
				  x, y, w, h,
				  fg, bg);

			fill_rect(&out, alu, NULL, 0,
				  tile, tx, ty,
				  x, y, h, w,
				  fg, bg);
			fill_rect(&ref, alu, NULL, 0,
				  tile, tx, ty,
				  x, y, h, w,
				  fg, bg);
		}

		test_compare(t,
			     out.draw, out.format,
			     ref.draw, ref.format,
			     0, 0, out.width, out.height,
			     "");

		if (target == CHILD) {
			int x = rand() % (t->out.width-out.width);
			int y = rand() % (t->out.height-out.height);

			clear(&out);
			clear(&ref);

			XMoveWindow(out.dpy->dpy, out.draw, x, y);
			XMoveWindow(ref.dpy->dpy, ref.draw, x, y);

			clear(&out);
			clear(&ref);
		}
	}

	printf("passed [%d iterations x %d]\n", reps, sets);

	test_target_destroy_render(&t->out, &out);
	test_target_destroy_render(&t->ref, &ref);
}

static void unclipped_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target out, ref;
	int r, s;

	printf("Testing unclipped tiled fills (%s): ", test_target_name(target));
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
			uint8_t tile = rand();
			uint8_t alu = rand() % (GXset + 1);
			uint32_t fg = rand();
			uint32_t bg = rand();

			fill_rect(&out, alu, NULL, 0,
				  tile, tx, ty,
				  x, y, w, h,
				  fg, bg);
			fill_rect(&ref, alu, NULL, 0,
				  tile, tx, ty,
				  x, y, w, h,
				  fg, bg);
		}

		test_compare(t,
			     out.draw, out.format,
			     ref.draw, ref.format,
			     0, 0, out.width, out.height,
			     "");

		if (target == CHILD) {
			int x = rand() % (t->out.width-out.width);
			int y = rand() % (t->out.height-out.height);

			clear(&out);
			clear(&ref);

			XMoveWindow(out.dpy->dpy, out.draw, x, y);
			XMoveWindow(ref.dpy->dpy, ref.draw, x, y);

			clear(&out);
			clear(&ref);
		}
	}

	printf("passed [%d iterations x %d]\n", reps, sets);

	test_target_destroy_render(&t->out, &out);
	test_target_destroy_render(&t->ref, &ref);
}

static void simple_clip_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target out, ref;
	int r, s;

	printf("Testing simple clipped tiled fills (%s): ", test_target_name(target));
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
			uint8_t tile = rand();
			uint8_t alu = rand() % (GXset + 1);
			uint32_t fg = rand();
			uint32_t bg = rand();

			fill_rect(&out, alu, NULL, 0,
				  tile, tx, ty,
				  x, y, w, h,
				  fg, bg);
			fill_rect(&ref, alu, NULL, 0,
				  tile, tx, ty,
				  x, y, w, h,
				  fg, bg);
		}

		test_compare(t,
			     out.draw, out.format,
			     ref.draw, ref.format,
			     0, 0, out.width, out.height,
			     "");

		if (target == CHILD) {
			int x = rand() % (t->out.width-out.width);
			int y = rand() % (t->out.height-out.height);

			clear(&out);
			clear(&ref);

			XMoveWindow(out.dpy->dpy, out.draw, x, y);
			XMoveWindow(ref.dpy->dpy, ref.draw, x, y);

			clear(&out);
			clear(&ref);
		}
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

	printf("Testing complex clipped tiled fills (%s): ", test_target_name(target));
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
			uint8_t tile = rand();
			uint8_t alu = rand() % (GXset + 1);
			uint32_t fg = rand();
			uint32_t bg = rand();

			fill_rect(&out, alu, clip, nclip,
				  tile, tx, ty,
				  x, y, w, h,
				  fg, bg);
			fill_rect(&ref, alu, clip, nclip,
				  tile, tx, ty,
				  x, y, w, h,
				  fg, bg);
		}

		test_compare(t,
			     out.draw, out.format,
			     ref.draw, ref.format,
			     0, 0, out.width, out.height,
			     "");

		free(clip);

		if (target == CHILD) {
			int x = rand() % (t->out.width-out.width);
			int y = rand() % (t->out.height-out.height);

			clear(&out);
			clear(&ref);

			XMoveWindow(out.dpy->dpy, out.draw, x, y);
			XMoveWindow(ref.dpy->dpy, ref.draw, x, y);

			clear(&out);
			clear(&ref);
		}
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
			small_tests(&test, reps, sets, t);
			unclipped_tests(&test, reps, sets, t);
			simple_clip_tests(&test, reps, sets, t);
			complex_clip_tests(&test, reps, sets, t);
			reset_cache();
		}
	}

	return 0;
}
