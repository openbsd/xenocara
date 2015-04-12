#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xutil.h> /* for XDestroyImage */
#include <pixman.h> /* for pixman blt functions */

#include "test.h"

static const uint8_t ops[] = {
	PictOpClear,
	PictOpSrc,
	PictOpDst,
};

static void fill_rect(struct test_display *dpy,
		      Picture p,
		      XRenderPictFormat *format,
		      uint8_t op, int x, int y, int w, int h,
		      uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	Display *d = dpy->dpy;
	XRenderColor render_color;
	Pixmap pixmap1, pixmap2;
	Picture tmp1, tmp2;
	XRenderPictureAttributes pa;
	GC gc;

	render_color.red   = red * alpha;
	render_color.green = green * alpha;
	render_color.blue  = blue * alpha;
	render_color.alpha = alpha << 8;

	pixmap1 = XCreatePixmap(d, dpy->root, 1, 1, format->depth);
	tmp1 = XRenderCreatePicture(d, pixmap1, format, 0, NULL);

	pixmap2 = XCreatePixmap(d, dpy->root, 1, 1, format->depth);
	pa.repeat = 1;
	tmp2 = XRenderCreatePicture(d, pixmap2, format, CPRepeat, &pa);

	gc = XCreateGC(d, pixmap1, 0, NULL);

	XRenderFillRectangle(d, PictOpSrc, tmp1, &render_color, 0, 0, 1,1);
	XCopyArea(d, pixmap1, pixmap2, gc, 0, 0, 1, 1, 0, 0);
	XRenderComposite(d, PictOpSrc, tmp2, 0, p, 0, 0, 0, 0, x, y, w,h);

	XFreeGC(d, gc);

	XRenderFreePicture(d, tmp2);
	XFreePixmap(d, pixmap2);

	XRenderFreePicture(d, tmp1);
	XFreePixmap(d, pixmap1);
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
			int red = rand() % 0xff;
			int green = rand() % 0xff;
			int blue = rand() % 0xff;
			int alpha = rand() % 0xff;

			fill_rect(&t->out, tt.picture, tt.format,
				  PictOpSrc, x, y, 1, 1,
				  red, green, blue, alpha);

			pixels[r].x = x;
			pixels[r].y = y;
			cells[y*tt.width+x] = color(red, green, blue, alpha);
		}

		test_init_image(&image, &t->out.shm, tt.format, 1, 1);

		for (r = 0; r < reps; r++) {
			uint32_t result;
			uint32_t x = pixels[r].x;
			uint32_t y = pixels[r].y;

			XShmGetImage(t->out.dpy, tt.draw, &image,
				     x, y, AllPlanes);

			result = *(uint32_t *)image.data;
			if (!pixel_equal(image.depth, result,
					 cells[y*tt.width+x])) {
				uint32_t mask;

				if (image.depth == 32)
					mask = 0xffffffff;
				else
					mask = (1 << image.depth) - 1;
				die("failed to set pixel (%d,%d) to %08x, found %08x instead\n",
				    x, y,
				    cells[y*tt.width+x] & mask,
				    result & mask);
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
			int w = rand() % tt.width;
			int h = rand() % tt.height;
			int red = rand() % 0xff;
			int green = rand() % 0xff;
			int blue = rand() % 0xff;
			int alpha = rand() % 0xff;

			x = rand() % (2*tt.width) - tt.width;
			y = rand() % (2*tt.height) - tt.height;

			fill_rect(&t->out, tt.picture, tt.format,
				  PictOpSrc, x, y, w, h,
				  red, green, blue, alpha);

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

			pixman_fill(cells, tt.width, 32, x, y, w, h,
				    color(red, green, blue, alpha));
		}

		XShmGetImage(t->out.dpy, tt.draw, &image, 0, 0, AllPlanes);

		for (y = 0; y < tt.height; y++) {
			for (x = 0; x < tt.width; x++) {
				uint32_t result =
					*(uint32_t *)(image.data +
						      y*image.bytes_per_line +
						      image.bits_per_pixel*x/8);
				if (!pixel_equal(image.depth, result,
						 cells[y*tt.width+x])) {
					uint32_t mask;

					if (image.depth == 32)
						mask = 0xffffffff;
					else
						mask = (1 << image.depth) - 1;
					die("failed to set pixel (%d,%d) to %08x, found %08x instead\n",
					    x, y,
					    cells[y*tt.width+x] & mask,
					    result & mask);
				}
			}
		}
	}

	printf("passed [%d iterations x %d]\n", reps, sets);

	test_target_destroy_render(&t->out, &tt);
	free(cells);
}

static void rect_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target out, ref;
	int r, s;

	printf("Testing area fills (%s): ", test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->out, target, &out);
	clear(&t->out, &out);

	test_target_create_render(&t->ref, target, &ref);
	clear(&t->ref, &ref);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x = rand() % (2*out.width) - out.width;
			int y = rand() % (2*out.height) - out.height;
			int w = rand() % out.width;
			int h = rand() % out.height;
			int op = ops[rand() % sizeof(ops)];
			int red = rand() % 0xff;
			int green = rand() % 0xff;
			int blue = rand() % 0xff;
			int alpha = rand() % 0xff;

			fill_rect(&t->out, out.picture, out.format,
				  op, x, y, w, h,
				  red, green, blue, alpha);
			fill_rect(&t->ref, ref.picture, ref.format,
				  op, x, y, w, h,
				  red, green, blue, alpha);
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
			rect_tests(&test, reps, sets, t);
		}
	}

	return 0;
}
