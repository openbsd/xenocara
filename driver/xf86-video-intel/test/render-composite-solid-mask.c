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

static void fill_rect(struct test_display *dpy, Picture p, uint8_t op,
		      int x, int y, int w, int h,
		      uint8_t s_red, uint8_t s_green, uint8_t s_blue, uint8_t s_alpha,
		      uint8_t m_red, uint8_t m_green, uint8_t m_blue, uint8_t m_alpha)
{
	XRenderColor render_color;
	Picture source, mask;

	render_color.red   = s_red * s_alpha;
	render_color.green = s_green * s_alpha;
	render_color.blue  = s_blue * s_alpha;
	render_color.alpha = s_alpha << 8 | s_alpha;
	source = XRenderCreateSolidFill(dpy->dpy, &render_color);

	render_color.red   = m_red * m_alpha;
	render_color.green = m_green * m_alpha;
	render_color.blue  = m_blue * m_alpha;
	render_color.alpha = m_alpha << 8 | m_alpha;
	mask = XRenderCreateSolidFill(dpy->dpy, &render_color);

	XRenderComposite(dpy->dpy, op, source, mask, p, 0, 0, 0, 0, x, y, w,h);

	XRenderFreePicture(dpy->dpy, mask);
	XRenderFreePicture(dpy->dpy, source);
}

static void clear(struct test_display *dpy, struct test_target *tt)
{
	XRenderColor render_color = {0};
	XRenderFillRectangle(dpy->dpy, PictOpClear, tt->picture, &render_color,
			     0, 0, tt->width, tt->height);
}

static void ref_tests(struct test *t, int reps, int sets, enum target target)
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
			int s_red = rand() % 0xff;
			int s_green = rand() % 0xff;
			int s_blue = rand() % 0xff;
			int s_alpha = rand() % 0xff;
			int m_red = rand() % 0xff;
			int m_green = rand() % 0xff;
			int m_blue = rand() % 0xff;
			int m_alpha = rand() % 0xff;

			fill_rect(&t->out, out.picture,
				  op, x, y, w, h,
				  s_red, s_green, s_blue, s_alpha,
				  m_red, m_green, m_blue, m_alpha);
			fill_rect(&t->ref, ref.picture,
				  op, x, y, w, h,
				  s_red, s_green, s_blue, s_alpha,
				  m_red, m_green, m_blue, m_alpha);
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

		for (t = TARGET_FIRST; t <= TARGET_LAST; t++)
			ref_tests(&test, reps, sets, t);
	}

	return 0;
}
