#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xutil.h> /* for XDestroyImage */
#include <pixman.h> /* for pixman blt functions */

#include "test.h"

static void
show_cells(char *buf,
	   const uint32_t *real, const uint32_t *ref,
	   int x, int y, int w, int h)
{
	int i, j, len = 0;

	for (j = y - 2; j <= y + 2; j++) {
		if (j < 0 || j >= h)
			continue;

		for (i = x - 2; i <= x + 2; i++) {
			if (i < 0 || i >= w)
				continue;

			len += sprintf(buf+len, "%08x ", real[j*w+i]);
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

static void fill_rect(struct test_display *t, Picture p,
		      int x, int y, int w, int h,
		      uint8_t red, uint8_t green, uint8_t blue)
{
	Drawable tmp;
	XRenderColor c;
	Picture src;
	XRenderPictFormat *format;

	format = XRenderFindStandardFormat(t->dpy, PictStandardRGB24);

	tmp = XCreatePixmap(t->dpy, DefaultRootWindow(t->dpy),
			    w, h, format->depth);

	src = XRenderCreatePicture(t->dpy, tmp, format, 0, NULL);
	c.red = (int)red << 8 | red;
	c.green = (int)green << 8 | green;
	c.blue = (int)blue << 8 | blue;
	c.alpha = 0xffff;
	XRenderFillRectangle(t->dpy, PictOpSrc, src, &c, 0, 0, w, h);
	XRenderComposite(t->dpy, PictOpOver, src, 0, p, 0, 0, 0, 0, x, y, w, h);

	XRenderFreePicture(t->dpy, src);
	XFreePixmap(t->dpy, tmp);
}

static void pixel_tests(struct test *t, int reps, int sets, enum target target)
{
	struct test_target tt;
	XImage image;
	uint32_t *cells = malloc(t->real.width*t->real.height*4);
	struct {
		uint16_t x, y;
	} *pixels = malloc(reps*sizeof(*pixels));
	int r, s;

	test_target_create_render(&t->real, target, &tt);

	printf("Testing setting of single pixels (%s): ",
	       test_target_name(target));
	fflush(stdout);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x = rand() % (tt.width - 1);
			int y = rand() % (tt.height - 1);
			uint8_t red = rand();
			uint8_t green = rand();
			uint8_t blue = rand();

			fill_rect(&t->real, tt.picture,
				  x, y, 1, 1,
				  red, green, blue);

			pixels[r].x = x;
			pixels[r].y = y;
			cells[y*tt.width+x] = color(red, green, blue, 0xff);
		}

		test_init_image(&image, &t->real.shm, tt.format, 1, 1);

		for (r = 0; r < reps; r++) {
			uint32_t x = pixels[r].x;
			uint32_t y = pixels[r].y;
			uint32_t result;

			XShmGetImage(t->real.dpy, tt.draw, &image,
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

	test_target_destroy_render(&t->real, &tt);
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
	uint32_t *cells = calloc(sizeof(uint32_t), t->real.width*t->real.height);
	int r, s, x, y;

	printf("Testing area sets (%s): ", test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->real, target, &tt);
	clear(&t->real, &tt);

	test_init_image(&image, &t->real.shm, tt.format, tt.width, tt.height);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int w = 1 + rand() % (tt.width - 1);
			int h = 1 + rand() % (tt.height - 1);
			uint8_t red = rand();
			uint8_t green = rand();
			uint8_t blue = rand();

			x = rand() % (2*tt.width) - tt.width;
			y = rand() % (2*tt.height) - tt.height;

			fill_rect(&t->real, tt.picture,
				  x, y, w, h,
				  red, green, blue);

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
				    color(red, green, blue, 0xff));
		}

		XShmGetImage(t->real.dpy, tt.draw, &image, 0, 0, AllPlanes);

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

					die("failed to set pixel (%d,%d) to %08x [%08x], found %08x [%08x] instead (set %d, reps %d)\n%s",
					    x, y,
					    cells[y*tt.width+x] & mask,
					    cells[y*tt.width+x],
					    result & mask,
					    result, s, reps, buf);
				}
			}
		}
	}

	printf("passed [%d iterations x %d]\n", reps, sets);

	test_target_destroy_render(&t->real, &tt);
	free(cells);
}

static void rect_tests(struct test *t, int reps, int sets, enum target target, int use_window)
{
	struct test_target real, ref;
	int r, s;
	printf("Testing area fills (%s, using %s source): ",
	       test_target_name(target), use_window ? "window" : "pixmap");
	fflush(stdout);

	test_target_create_render(&t->real, target, &real);
	clear(&t->real, &real);

	test_target_create_render(&t->ref, target, &ref);
	clear(&t->ref, &ref);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x, y, w, h;
			uint8_t red = rand();
			uint8_t green = rand();
			uint8_t blue = rand();

			x = rand() % (real.width - 1);
			y = rand() % (real.height - 1);
			w = 1 + rand() % (real.width - x - 1);
			h = 1 + rand() % (real.height - y - 1);

			fill_rect(&t->real, real.picture,
				  x, y, w, h,
				  red, green, blue);
			fill_rect(&t->ref, ref.picture,
				  x, y, w, h,
				  red, green, blue);
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

		if (sets < 2)
			sets = 2;

		pixel_tests(&test, reps, sets, PIXMAP);
		area_tests(&test, reps, sets, PIXMAP);
		rect_tests(&test, reps, sets, PIXMAP, 0);
	}

	return 0;
}
