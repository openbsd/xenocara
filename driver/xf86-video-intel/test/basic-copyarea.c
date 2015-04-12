#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xutil.h> /* for XDestroyImage */
#include <pixman.h> /* for pixman blt functions */

#include "test.h"

static void
show_cells(char *buf,
	   const uint32_t *out, const uint32_t *ref,
	   int x, int y, int w, int h)
{
	int i, j, len = 0;

	for (j = y - 2; j <= y + 2; j++) {
		if (j < 0 || j >= h)
			continue;

		for (i = x - 2; i <= x + 2; i++) {
			if (i < 0 || i >= w)
				continue;

			len += sprintf(buf+len, "%08x ", out[j*w+i]);
		}

		len += sprintf(buf+len, "\t");

		for (i = x - 2; i <= x + 2; i++) {
			if (i < 0 || i >= w)
				continue;

			len += sprintf(buf+len, "%08x ", ref[j*w+i]);
		}

		len += sprintf(buf+len, "\n");
	}
}

static void fill_rect(struct test_display *t,
		      Drawable d,
		      XRenderPictFormat *format,
		      int use_window, int tx, int ty,
		      uint8_t alu, int x, int y, int w, int h, uint32_t fg)
{
	XGCValues val;
	Drawable tmp;
	GC gc;

	val.graphics_exposures = 0;
	val.function = alu;
	val.foreground = fg;

	if (use_window) {
		XSetWindowAttributes attr;

		attr.override_redirect = 1;
		tmp = XCreateWindow(t->dpy, DefaultRootWindow(t->dpy),
				    tx, ty,
				    w, h,
				    0, format->depth,
				    InputOutput,
				    DefaultVisual(t->dpy,
						  DefaultScreen(t->dpy)),
				    CWOverrideRedirect, &attr);
		XMapWindow(t->dpy, tmp);
	} else
		tmp = XCreatePixmap(t->dpy, d, w, h, format->depth);

	gc = XCreateGC(t->dpy, d, GCGraphicsExposures | GCForeground, &val);
	XFillRectangle(t->dpy, tmp, gc, 0, 0, w, h);

	XChangeGC(t->dpy, gc, GCFunction, &val);
	XCopyArea(t->dpy, tmp, d, gc, 0, 0, w, h, x, y);

	XFreeGC(t->dpy, gc);
	if (use_window)
		XDestroyWindow(t->dpy, tmp);
	else
		XFreePixmap(t->dpy, tmp);
}

static void pixel_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target tt;
	XImage image;
	uint32_t *cells = malloc(t->out.width*t->out.height*4);
	struct {
		uint16_t x, y;
	} *pixels = malloc(reps*sizeof(*pixels));
	int r, s;

	test_target_create_render(&t->out, target, &tt);

	printf("Testing setting of single pixels (%s): ",
	       test_target_name(target));
	fflush(stdout);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x = rand() % (tt.width - 1);
			int y = rand() % (tt.height - 1);
			uint32_t fg = rand();

			fill_rect(&t->out, tt.draw, tt.format,
				  0, 0, 0,
				  GXcopy, x, y, 1, 1, fg);

			pixels[r].x = x;
			pixels[r].y = y;
			cells[y*tt.width+x] = fg;
		}

		test_init_image(&image, &t->out.shm, tt.format, 1, 1);

		for (r = 0; r < reps; r++) {
			uint32_t x = pixels[r].x;
			uint32_t y = pixels[r].y;
			uint32_t result;

			XShmGetImage(t->out.dpy, tt.draw, &image,
				     x, y, AllPlanes);

			result = *(uint32_t *)image.data;
			if (!pixel_equal(image.depth, result,
					 cells[y*tt.width+x])) {
				uint32_t mask = depth_mask(image.depth);

				die("failed to set pixel (%d,%d) to %08x [%08x], found %08x [%08x] instead\n",
				    x, y,
				    cells[y*tt.width+x] & mask,
				    cells[y*tt.width+x],
				    result & mask,
				    result);
			}
		}
	}
	printf("passed [%d iterations x %d]\n", reps, sets);

	test_target_destroy_render(&t->out, &tt);
	free(pixels);
	free(cells);
}

static void clear(struct test_display *dpy, struct test_target *tt)
{
	XRenderColor render_color = {0};
	XRenderFillRectangle(dpy->dpy, PictOpClear, tt->picture, &render_color,
			     0, 0, tt->width, tt->height);
}

static void area_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target tt;
	XImage image;
	uint32_t *cells = calloc(sizeof(uint32_t), t->out.width*t->out.height);
	int r, s, x, y;

	printf("Testing area sets (%s): ", test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->out, target, &tt);
	clear(&t->out, &tt);

	test_init_image(&image, &t->out.shm, tt.format, tt.width, tt.height);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int w = 1 + rand() % (tt.width - 1);
			int h = 1 + rand() % (tt.height - 1);
			uint32_t fg = rand();

			x = rand() % (2*tt.width) - tt.width;
			y = rand() % (2*tt.height) - tt.height;

			fill_rect(&t->out, tt.draw, tt.format,
				  0, 0, 0,
				  GXcopy, x, y, w, h, fg);

			if (x < 0)
				w += x, x = 0;
			if (y < 0)
				h += y, y = 0;
			if (x >= tt.width || y >= tt.height)
				continue;

			if (x + w > tt.width)
				w = tt.width - x;
			if (y + h > tt.height)
				h = tt.height - y;
			if (w <= 0 || h <= 0)
				continue;

			pixman_fill(cells, tt.width, 32, x, y, w, h, fg);
		}

		XShmGetImage(t->out.dpy, tt.draw, &image, 0, 0, AllPlanes);

		for (y = 0; y < tt.height; y++) {
			for (x = 0; x < tt.width; x++) {
				uint32_t result = *(uint32_t *)
					(image.data +
					 y*image.bytes_per_line +
					 x*image.bits_per_pixel/8);
				if (!pixel_equal(image.depth, result, cells[y*tt.width+x])) {
					char buf[600];
					uint32_t mask = depth_mask(image.depth);
					show_cells(buf,
						   (uint32_t*)image.data, cells,
						   x, y, tt.width, tt.height);

					die("failed to set pixel (%d,%d) to %08x [%08x], found %08x [%08x] instead\n%s",
					    x, y,
					    cells[y*tt.width+x] & mask,
					    cells[y*tt.width+x],
					    result & mask,
					    result, buf);
				}
			}
		}
	}

	printf("passed [%d iterations x %d]\n", reps, sets);

	test_target_destroy_render(&t->out, &tt);
	free(cells);
}

static void rect_tests(struct test *t, int reps, int sets, enum target target, int use_window)
{
	struct test_target out, ref;
	int r, s;

	printf("Testing area fills (%s, using %s source): ",
	       test_target_name(target), use_window ? "window" : "pixmap");
	fflush(stdout);

	test_target_create_render(&t->out, target, &out);
	clear(&t->out, &out);

	test_target_create_render(&t->ref, target, &ref);
	clear(&t->ref, &ref);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x = rand() % (out.width - 1);
			int y = rand() % (out.height - 1);
			int w = 1 + rand() % (out.width - x - 1);
			int h = 1 + rand() % (out.height - y - 1);
			int tmpx = w == out.width ? 0 : rand() % (out.width - w);
			int tmpy = h == out.height ? 0 : rand() % (out.height - h);
			uint8_t alu = rand() % (GXset + 1);
			uint32_t fg = rand();

			fill_rect(&t->out, out.draw, out.format,
				  use_window, tmpx, tmpy,
				  alu, x, y, w, h, fg);
			fill_rect(&t->ref, ref.draw, ref.format,
				  use_window, tmpx, tmpy,
				  alu, x, y, w, h, fg);
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
			pixel_tests(&test, reps, sets, t);
			area_tests(&test, reps, sets, t);
			rect_tests(&test, reps, sets, t, 0);
			if (t != PIXMAP)
			    rect_tests(&test, reps, sets, t, 1);
		}
	}

	return 0;
}
