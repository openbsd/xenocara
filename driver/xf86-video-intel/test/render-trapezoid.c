#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xutil.h> /* for XDestroyImage */
#include <pixman.h> /* for pixman blt functions */

#include "test.h"

enum trapezoid {
	RECT_ALIGN,
	RECT_UNALIGN,
	GENERAL
};

static const uint8_t ops[] = {
	PictOpClear,
	PictOpSrc,
	PictOpDst,
};

static XRenderPictFormat *mask_format(Display *dpy, enum mask mask)
{
	switch (mask) {
	default:
	case MASK_NONE: return NULL;
	case MASK_A1: return  XRenderFindStandardFormat(dpy, PictStandardA1);
	case MASK_A8: return  XRenderFindStandardFormat(dpy, PictStandardA8);
	}
}

static const char *mask_name(enum mask mask)
{
	switch (mask) {
	default:
	case MASK_NONE: return "none";
	case MASK_A1: return "a1";
	case MASK_A8: return "a8";
	}
}

static const char *trapezoid_name(enum trapezoid trapezoid)
{
	switch (trapezoid) {
	default:
	case RECT_ALIGN: return "pixel-aligned";
	case RECT_UNALIGN: return "rectilinear";
	case GENERAL: return "general";
	}
}

static void fill_rect(struct test_display *dpy, Picture p, uint8_t op,
		      int x, int y, int w, int h,
		      int dx, int dy, enum mask mask,
		      uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	XRenderColor render_color;
	XTrapezoid trap;
	Picture src;

	render_color.red   = red * alpha;
	render_color.green = green * alpha;
	render_color.blue  = blue * alpha;
	render_color.alpha = alpha << 8;

	trap.left.p1.x = trap.left.p2.x = (x << 16) + dx;
	trap.top = trap.left.p1.y = trap.right.p1.y = (y << 16) + dy;
	trap.right.p1.x = trap.right.p2.x = ((x + w) << 16) + dx;
	trap.bottom = trap.left.p2.y = trap.right.p2.y = ((y + h) << 16) + dy;

	src = XRenderCreateSolidFill(dpy->dpy, &render_color);
	XRenderCompositeTrapezoids(dpy->dpy,
				   op, src, p, mask_format(dpy->dpy, mask),
				   0, 0, &trap, 1);
	XRenderFreePicture(dpy->dpy, src);
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

	printf("Testing setting of single pixels (%s): ", test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->out, target, &tt);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x = rand() % (tt.width - 1);
			int y = rand() % (tt.height - 1);
			int red = rand() % 0xff;
			int green = rand() % 0xff;
			int blue = rand() % 0xff;
			int alpha = rand() % 0xff;

			fill_rect(&t->out, tt.picture, PictOpSrc,
				  x, y, 1, 1,
				  0, 0, MASK_NONE,
				  red, green, blue, alpha);

			pixels[r].x = x;
			pixels[r].y = y;
			cells[y*t->out.width+x] = color(red, green, blue, alpha);
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
			int w = rand() % tt.width;
			int h = rand() % tt.height;
			int red = rand() % 0xff;
			int green = rand() % 0xff;
			int blue = rand() % 0xff;
			int alpha = rand() % 0xff;

			x = rand() % (2*tt.width) - tt.width;
			y = rand() % (2*tt.height) - tt.height;

			fill_rect(&t->out, tt.picture, PictOpSrc,
				  x, y, w, h,
				  0, 0, MASK_NONE,
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
				if (!pixel_equal(image.depth, result, cells[y*tt.width+x])) {
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
	}

	printf("passed [%d iterations x %d]\n", reps, sets);

	test_target_destroy_render(&t->out, &tt);
	free(cells);
}

static void rect_tests(struct test *t,
		       int dx, int dy,
		       enum mask mask,
		       int reps, int sets,
		       enum target target)
{
	struct test_target out, ref;
	int r, s;

	printf("Testing area fills (offset %dx%d, mask %s) (%s): ",
	       dx, dy, mask_name(mask), test_target_name(target));
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

			fill_rect(&t->out, out.picture, op,
				  x, y, w, h, dx, dy, mask,
				  red, green, blue, alpha);
			fill_rect(&t->ref, ref.picture, op,
				  x, y, w, h, dx, dy, mask,
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

static void random_trapezoid(XTrapezoid *trap, enum trapezoid trapezoid,
			     int x1, int y1, int x2, int y2)
{
	switch (trapezoid) {
	case RECT_ALIGN:
		x1 = x1 + rand() % (x2 - x1);
		x2 = x1 + rand() % (x2 - x1);
		y1 = y1 + rand() % (y2 - y1);
		y2 = y1 + rand() % (y2 - y1);

		trap->left.p1.x = trap->left.p2.x = x1 << 16;
		trap->top = trap->left.p1.y = trap->right.p1.y = y1 << 16;
		trap->right.p1.x = trap->right.p2.x = x2 << 16;
		trap->bottom = trap->left.p2.y = trap->right.p2.y = y2 << 16;
		break;

	case RECT_UNALIGN:
		x1 <<= 16; x2 <<= 16;
		y1 <<= 16; y2 <<= 16;

		x1 = x1 + rand() % (x2 - x1);
		x2 = x1 + rand() % (x2 - x1);
		y1 = y1 + rand() % (y2 - y1);
		y2 = y1 + rand() % (y2 - y1);

		trap->left.p1.x = trap->left.p2.x = x1;
		trap->top = trap->left.p1.y = trap->right.p1.y = y1;
		trap->right.p1.x = trap->right.p2.x = x2;
		trap->bottom = trap->left.p2.y = trap->right.p2.y = y2;
		break;

	case GENERAL:
		x1 <<= 16; x2 <<= 16;
		y1 <<= 16; y2 <<= 16;

		trap->top = y1 + rand() % (y2 - y1);
		trap->bottom = y1 + rand() % (y2 - y1);

		trap->left.p1.x = x1 + rand() % (x2 - x1);
		trap->left.p2.x = x1 + rand() % (x2 - x1);

		trap->right.p1.x = x1 + rand() % (x2 - x1);
		trap->right.p2.x = x1 + rand() % (x2 - x1);
		break;

	}
}

static void trap_tests(struct test *t,
		       enum mask mask,
		       enum trapezoid trapezoid,
		       int reps, int sets,
		       enum target target)
{
	struct test_target out, ref;
	XTrapezoid *traps;
	int max_traps = 65536;
	int r, s, n;

	traps = malloc(sizeof(*traps) * max_traps);
	if (traps == NULL)
		return;

	printf("Testing trapezoids (%s with mask %s) (%s): ",
	       trapezoid_name(trapezoid),
	       mask_name(mask),
	       test_target_name(target));
	fflush(stdout);

	test_target_create_render(&t->out, target, &out);
	clear(&t->out, &out);

	test_target_create_render(&t->ref, target, &ref);
	clear(&t->ref, &ref);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			XRenderColor render_color;
			int op = ops[rand() % sizeof(ops)];
			int red = rand() % 0xff;
			int green = rand() % 0xff;
			int blue = rand() % 0xff;
			int alpha = rand() % 0xff;
			int num_traps = rand() % max_traps;
			Picture src;

			for (n = 0; n < num_traps; n++)
				random_trapezoid(&traps[n], 0,
						 0, 0, out.width, out.height);

			render_color.red   = red * alpha;
			render_color.green = green * alpha;
			render_color.blue  = blue * alpha;
			render_color.alpha = alpha << 8;

			src = XRenderCreateSolidFill(t->out.dpy,
						     &render_color);
			XRenderCompositeTrapezoids(t->out.dpy,
						   op, src, out.picture,
						   mask_format(t->out.dpy, mask),
						   0, 0, traps, num_traps);
			XRenderFreePicture(t->out.dpy, src);

			src = XRenderCreateSolidFill(t->ref.dpy,
						     &render_color);
			XRenderCompositeTrapezoids(t->ref.dpy,
						   op, src, ref.picture,
						   mask_format(t->ref.dpy, mask),
						   0, 0, traps, num_traps);
			XRenderFreePicture(t->ref.dpy, src);
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
	free(traps);
}

int main(int argc, char **argv)
{
	struct test test;
	int i, dx, dy;
	enum target target;
	enum mask mask;
	enum trapezoid trapezoid;

	test_init(&test, argc, argv);

	for (i = 0; i <= DEFAULT_ITERATIONS; i++) {
		int reps = REPS(i), sets = SETS(i);

		for (target = TARGET_FIRST; target <= TARGET_LAST; target++) {
			pixel_tests(&test, reps, sets, target);
			area_tests(&test, reps, sets, target);
			for (dy = 0; dy < 1 << 16; dy += 1 << 14)
				for (dx = 0; dx < 1 << 16; dx += 1 << 14)
					for (mask = MASK_NONE; mask <= MASK_A8; mask++)
						rect_tests(&test, dx, dy, mask, reps, sets, target);
			for (trapezoid = RECT_ALIGN; trapezoid <= GENERAL; trapezoid++)
				trap_tests(&test, mask, trapezoid, reps, sets, target);
		}
	}

	return 0;
}
