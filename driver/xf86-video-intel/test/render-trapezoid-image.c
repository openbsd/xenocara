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
	case MASK_NONE:
	case MASK_NONE_AA:
	       	return NULL;
	case MASK_A1:
	       	return  XRenderFindStandardFormat(dpy, PictStandardA1);
	case MASK_A8:
	       	return  XRenderFindStandardFormat(dpy, PictStandardA8);
	}
}

static const char *mask_name(enum mask mask)
{
	switch (mask) {
	default:
	case MASK_NONE: return "none";
	case MASK_NONE_AA: return "none/aa";
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


static void fill_rect(struct test_display *t, Picture p, XRenderPictFormat *format,
		      uint8_t op, int x, int y, int w, int h,
		      int dx, int dy, enum mask mask,
		      int use_window, int tx, int ty,
		      uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	XRenderColor color;
	XTrapezoid trap;
	Drawable tmp;
	Picture src;
	int w1 = w + (dx!=0);
	int h1 = h + (dy!=0);

	if (use_window) {
		XSetWindowAttributes attr;

		attr.override_redirect = 1;
		tmp = XCreateWindow(t->dpy, DefaultRootWindow(t->dpy),
				    tx, ty,
				    w1, h1,
				    0, format->depth,
				    InputOutput,
				    DefaultVisual(t->dpy,
						  DefaultScreen(t->dpy)),
				    CWOverrideRedirect, &attr);
		XMapWindow(t->dpy, tmp);
	} else
		tmp = XCreatePixmap(t->dpy, DefaultRootWindow(t->dpy),
				    w1, h1, format->depth);

	src = XRenderCreatePicture(t->dpy, tmp, format, 0, NULL);
	color.red = red * alpha;
	color.green = green * alpha;
	color.blue = blue * alpha;
	color.alpha = alpha << 8 | alpha;
	XRenderFillRectangle(t->dpy, PictOpSrc, src, &color, 0, 0, w1, h1);

	trap.left.p1.x = trap.left.p2.x = (x << 16) + dx;
	trap.top = trap.left.p1.y = trap.right.p1.y = (y << 16) + dy;
	trap.right.p1.x = trap.right.p2.x = ((x + w) << 16) + dx;
	trap.bottom = trap.left.p2.y = trap.right.p2.y = ((y + h) << 16) + dy;

	XRenderCompositeTrapezoids(t->dpy,
				   op, src, p, mask_format(t->dpy, mask),
				   0, 0, &trap, 1);

	XRenderFreePicture(t->dpy, src);
	if (use_window)
		XDestroyWindow(t->dpy, tmp);
	else
		XFreePixmap(t->dpy, tmp);
}

static void pixel_tests(struct test *t, int reps, int sets, enum target target, int use_window)
{
	struct test_target tt;
	XImage image;
	uint32_t *cells = malloc(t->out.width*t->out.height*4);
	struct {
		uint16_t x, y;
	} *pixels = malloc(reps*sizeof(*pixels));
	int r, s;

	printf("Testing setting of single pixels (%s using %s): ",
	       test_target_name(target),
	       use_window ? "window" : "pixmap");
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

			int tx, ty;

			do {
				tx = rand() % (tt.width - 1);
				ty = rand() % (tt.height - 1);
			} while (tx == x && ty == y);

			fill_rect(&t->out, tt.picture,
				  use_window ? t->out.format : tt.format,
				  PictOpSrc, x, y, 1, 1,
				  0, 0, MASK_NONE,
				  use_window, tx, ty,
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

static void set_mask(struct test_display *t, struct test_target *tt, enum mask mask)
{
	XRenderPictureAttributes pa;

	switch (mask) {
	case MASK_NONE:
		pa.poly_edge = PolyEdgeSharp;
		break;
	default:
		pa.poly_edge = PolyEdgeSmooth;
		break;
	}

	XRenderChangePicture(t->dpy, tt->picture, CPPolyEdge, &pa);
}

static void fill(uint32_t *cells,
		 int x, int y,
		 int w, int h,
		 int max_width, int max_height,
		 uint32_t pixel)
{
	if (x < 0)
		w += x, x = 0;
	if (y < 0)
		h += y, y = 0;
	if (x >= max_width || y >= max_height)
		return;

	if (x + w > max_width)
		w = max_width - x;
	if (y + h > max_height)
		h = max_height - y;
	if (w <= 0 || h <= 0)
		return;

	pixman_fill(cells, max_width, 32, x, y, w, h, pixel);
}

static void area_tests(struct test *t, int reps, int sets, enum target target, int use_window)
{
	struct test_target tt;
	XImage image;
	uint32_t *cells = calloc(sizeof(uint32_t), t->out.width*t->out.height);
	int r, s, x, y;

	printf("Testing area sets (%s using %s source): ",
	       test_target_name(target),
	       use_window ? "window" : "pixmap");
	fflush(stdout);

	test_target_create_render(&t->out, target, &tt);
	clear(&t->out, &tt);

	test_init_image(&image, &t->out.shm, tt.format, tt.width, tt.height);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int red = rand() % 0xff;
			int green = rand() % 0xff;
			int blue = rand() % 0xff;
			int alpha = rand() % 0xff;
			int tx, ty, try = 50;
			int w, h;

			x = rand() % (2*tt.width) - tt.width;
			y = rand() % (2*tt.height) - tt.height;
			if (use_window) {
				do {
					w = 1 + rand() % (tt.width - 1);
					h = 1 + rand() % (tt.height - 1);

					tx = w == tt.width ? 0 : rand() % (tt.width - w);
					ty = h == tt.height ? 0 : rand() % (tt.height - h);
				} while (((tx+w > x && tx < x+w) &&
					  (ty+h > y && ty < y+h)) &&
					 --try);

				if (!try)
					continue;
			} else {
				w = 1 + rand() % (2*tt.width);
				h = 1 + rand() % (2*tt.height);
				tx = ty = 0;
			}

			fill_rect(&t->out, tt.picture,
				  use_window ? t->out.format : tt.format,
				  PictOpSrc, x, y, w, h,
				  0, 0, MASK_NONE,
				  use_window, tx, ty,
				  red, green, blue, alpha);

			if (use_window)
				fill(cells, tx, ty, w, h, tt.width, tt.height,
				     color(red, green, blue, alpha));
			fill(cells, x, y, w, h, tt.width, tt.height,
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

static void rect_tests(struct test *t,
		       int dx, int dy,
		       enum mask mask,
		       int reps, int sets,
		       enum target target,
		       int use_window)
{
	struct test_target out, ref;
	int r, s;

	printf("Testing area fills (offset %dx%d, mask %s) (%s using %s source): ",
	       dx, dy, mask_name(mask), test_target_name(target),
	       use_window ? "window" : "pixmap");
	fflush(stdout);

	test_target_create_render(&t->out, target, &out);
	clear(&t->out, &out);
	set_mask(&t->out, &out, mask);

	test_target_create_render(&t->ref, target, &ref);
	clear(&t->ref, &ref);
	set_mask(&t->ref, &ref, mask);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int x, y, w, h;
			int op = ops[rand() % sizeof(ops)];
			int red = rand() % 0xff;
			int green = rand() % 0xff;
			int blue = rand() % 0xff;
			int alpha = rand() % 0xff;
			int tx, ty, try = 50;

			do {
				x = rand() % (out.width - 1);
				y = rand() % (out.height - 1);
				w = 1 + rand() % (out.width - x - 1);
				h = 1 + rand() % (out.height - y - 1);
				tx = w == out.width ? 0 : rand() % (out.width - w);
				ty = h == out.height ? 0 : rand() % (out.height - h);
			} while (((tx+w > x && tx < x+w) &&
				  (ty+h > y && ty < y+h)) &&
				 --try);

			if (try) {
				fill_rect(&t->out, out.picture,
					  use_window ? t->out.format : out.format,
					  op, x, y, w, h,
					  dx, dy, mask,
					  use_window, tx, ty,
					  red, green, blue, alpha);
				fill_rect(&t->ref, ref.picture,
					  use_window ? t->ref.format : ref.format,
					  op, x, y, w, h,
					  dx, dy, mask,
					  use_window, tx, ty,
					  red, green, blue, alpha);
			}
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

static void fill_traps(struct test_display *t, Picture p, XRenderPictFormat *format,
		       uint8_t op, XTrapezoid *traps, int ntraps, enum mask mask,
		       int srcx, int srcy, int srcw, int srch,
		       uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	XRenderColor color;
	Drawable tmp;
	Picture src;

	tmp = XCreatePixmap(t->dpy, DefaultRootWindow(t->dpy),
			    srcw, srch, format->depth);

	src = XRenderCreatePicture(t->dpy, tmp, format, 0, NULL);
	color.red = red * alpha;
	color.green = green * alpha;
	color.blue = blue * alpha;
	color.alpha = alpha << 8 | alpha;
	XRenderFillRectangle(t->dpy, PictOpSrc, src, &color, 0, 0, srcw, srch);

	XRenderCompositeTrapezoids(t->dpy,
				   op, src, p, mask_format(t->dpy, mask),
				   srcx, srcy, traps, ntraps);

	XRenderFreePicture(t->dpy, src);
	XFreePixmap(t->dpy, tmp);
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
	set_mask(&t->out, &out, mask);

	test_target_create_render(&t->ref, target, &ref);
	clear(&t->ref, &ref);
	set_mask(&t->ref, &ref, mask);

	for (s = 0; s < sets; s++) {
		for (r = 0; r < reps; r++) {
			int op = ops[rand() % sizeof(ops)];
			int red = rand() % 0xff;
			int green = rand() % 0xff;
			int blue = rand() % 0xff;
			int alpha = rand() % 0xff;
			int num_traps = rand() % max_traps;
			int srcx = rand() % 2*out.width - out.width;
			int srcy = rand() % 2*out.height - out.height;
			int srcw = rand() % out.width;
			int srch = rand() % out.height;

			for (n = 0; n < num_traps; n++)
				random_trapezoid(&traps[n], 0,
						 0, 0, out.width, out.height);


			fill_traps(&t->out, out.picture, out.format,
				   op, traps, num_traps, mask,
				   srcx, srcy, srcw, srch,
				   red, green, blue, alpha);

			fill_traps(&t->ref, ref.picture, ref.format,
				   op, traps, num_traps, mask,
				   srcx, srcy, srcw, srch,
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
			pixel_tests(&test, reps, sets, target, 0);
			area_tests(&test, reps, sets, target, 0);
			for (dy = 0; dy < 1 << 16; dy += 1 << 14)
				for (dx = 0; dx < 1 << 16; dx += 1 << 14)
					for (mask = MASK_NONE; mask <= MASK_A8; mask++)
						rect_tests(&test, dx, dy, mask, reps, sets, target, 0);
			if (target != CHILD) {
				pixel_tests(&test, reps, sets, target, 1);
				area_tests(&test, reps, sets, target, 1);
				for (dy = 0; dy < 1 << 16; dy += 1 << 14)
					for (dx = 0; dx < 1 << 16; dx += 1 << 14)
						for (mask = MASK_NONE; mask <= MASK_A8; mask++)
							rect_tests(&test, dx, dy, mask, reps, sets, target, 1);
			}
		}

		for (target = TARGET_FIRST; target <= TARGET_LAST; target++)
			for (trapezoid = RECT_ALIGN; trapezoid <= GENERAL; trapezoid++)
				trap_tests(&test, mask, trapezoid, reps, sets, target);
	}

	return 0;
}
