#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xutil.h> /* for XDestroyImage */

#include "test.h"

static void fill_rect(struct test_display *t,
		      Picture p,
		      XRenderPictFormat *format,
		      int use_window, int tx, int ty,
		      uint8_t op, int x, int y, int w, int h,
		      int mask_x, int mask_y, int mask_w, int mask_h,
		      uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	Drawable tmp;
	Pixmap pixmask;
	XRenderColor c;
	Picture src, mask;

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
		tmp = XCreatePixmap(t->dpy, DefaultRootWindow(t->dpy),
				    w, h, format->depth);

	pixmask = XCreatePixmap(t->dpy, DefaultRootWindow(t->dpy), w, h, 8);
	mask = XRenderCreatePicture(t->dpy, pixmask,
				    XRenderFindStandardFormat(t->dpy, PictStandardA8),
				    0, NULL);
	c.red = c.green = c.blue = c.alpha = 0;
	XRenderFillRectangle(t->dpy, PictOpSrc, mask, &c,
			     0, 0, w, h);
	c.red = c.green = c.blue = c.alpha = 0xffff;
	XRenderFillRectangle(t->dpy, PictOpSrc, mask, &c,
			     mask_x, mask_y, mask_w, mask_h);

	src = XRenderCreatePicture(t->dpy, tmp, format, 0, NULL);
	c.red = red * alpha;
	c.green = green * alpha;
	c.blue = blue * alpha;
	c.alpha = alpha << 8 | alpha;
	XRenderFillRectangle(t->dpy, PictOpSrc, src, &c, 0, 0, w, h);

	XRenderComposite(t->dpy, op, src, mask, p, 0, 0, 0, 0, x, y, w, h);

	XRenderFreePicture(t->dpy, src);
	if (use_window)
		XDestroyWindow(t->dpy, tmp);
	else
		XFreePixmap(t->dpy, tmp);

	XRenderFreePicture(t->dpy, mask);
	XFreePixmap(t->dpy, pixmask);
}

static void clear(struct test_display *dpy, struct test_target *tt)
{
	XRenderColor render_color = {0};
	XRenderFillRectangle(dpy->dpy, PictOpClear, tt->picture, &render_color,
			     0, 0, tt->width, tt->height);
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
			int x, y, w, h;
			int mask_x, mask_y, mask_w, mask_h;
			int tmpx, tmpy;
			uint8_t red = rand();
			uint8_t green = rand();
			uint8_t blue = rand();
			uint8_t alpha = rand();
			int try = 50;

			do {
				x = rand() % (out.width - 1);
				y = rand() % (out.height - 1);
				w = 1 + rand() % (out.width - x - 1);
				h = 1 + rand() % (out.height - y - 1);
				tmpx = w == out.width ? 0 : rand() % (out.width - w);
				tmpy = h == out.height ? 0 : rand() % (out.height - h);
			} while (((tmpx+w > x && tmpx < x+w) ||
				  (tmpy+h > y && tmpy < y+h)) &&
				 --try);

			mask_x = (rand() % (2*w)) - w;
			mask_y = (rand() % (2*h)) - h;
			mask_w = rand() % w;
			mask_h = rand() % h;

			if (try) {
				fill_rect(&t->out, out.picture, out.format,
					  use_window, tmpx, tmpy,
					  PictOpSrc, x, y, w, h,
					  mask_x, mask_y, mask_w, mask_h,
					  red, green, blue, alpha);
				fill_rect(&t->ref, ref.picture, ref.format,
					  use_window, tmpx, tmpy,
					  PictOpSrc, x, y, w, h,
					  mask_x, mask_y, mask_w, mask_h,
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

int main(int argc, char **argv)
{
	struct test test;
	int i;

	test_init(&test, argc, argv);

	for (i = 0; i <= DEFAULT_ITERATIONS; i++) {
		int reps = REPS(i), sets = SETS(i);
		enum target t;

		for (t = TARGET_FIRST; t <= TARGET_LAST; t++) {
			rect_tests(&test, reps, sets, t, 0);
			if (t != PIXMAP)
			    rect_tests(&test, reps, sets, t, 1);
		}
	}

	return 0;
}
