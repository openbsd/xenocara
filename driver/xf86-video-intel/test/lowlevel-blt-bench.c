/*
 * Copyright © 2009 Nokia Corporation
 * Copyright © 2010 Movial Creative Technologies Oy
 * Copyright © 2013 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <X11/X.h>
#include <X11/Xutil.h> /* for XDestroyImage */
#include <pixman.h> /* for pixman blt functions */

#include "test.h"

static const struct format {
	const char *name;
	pixman_format_code_t pixman_format;
} formats[] = {
	{ "a8r8g8b8", PIXMAN_a8r8g8b8 },
	{ "x8r8g8b8", PIXMAN_x8r8g8b8 },
	{ "a8", PIXMAN_a8 },
	{ "a4", PIXMAN_a4 },
	{ "a1", PIXMAN_a1 },
};

static const struct op {
	const char *name;
} ops[] = {
	[PictOpClear] = { "Clear" },
	[PictOpSrc] = { "Src" },
	[PictOpDst] = { "Dst" },
	[PictOpOver] = { "Over" },
	[PictOpOverReverse] = { "OverReverse" },
	[PictOpIn] = { "In" },
	[PictOpInReverse] = { "InReverse" },
	[PictOpOut] = { "Out" },
	[PictOpOutReverse] = { "OutReverse" },
	[PictOpAtop] = { "Atop" },
	[PictOpAtopReverse] = { "AtopReverse" },
	[PictOpXor] = { "Xor" },
	[PictOpAdd] = { "Add" },
	[PictOpSaturate] = { "Saturate" },
};

static double _bench(struct test_display *t, enum target target_type,
		     int op, int src_format,
		     int loops)
{
	XRenderColor render_color = { 0x8000, 0x8000, 0x8000, 0x8000 };
	struct test_target target;
	Pixmap pixmap;
	Picture picture;
	struct timespec tv;
	double elapsed;

	test_target_create_render(t, target_type, &target);
	XRenderFillRectangle(t->dpy, PictOpClear, target.picture, &render_color,
			     0, 0, target.width, target.height);

	pixmap = XCreatePixmap(t->dpy, t->root,
			       target.width, target.height,
			       PIXMAN_FORMAT_DEPTH(formats[src_format].pixman_format));

	picture = XRenderCreatePicture(t->dpy, pixmap,
				       XRenderFindStandardFormat(t->dpy, src_format),
				       0, NULL);
	XRenderFillRectangle(t->dpy, PictOpSrc, picture, &render_color,
			     0, 0, target.width, target.height);

	test_timer_start(t, &tv);
	while (loops--)
		XRenderComposite(t->dpy, op,
				 picture, 0, target.picture,
				 0, 0,
				 0, 0,
				 0, 0,
				 target.width, target.height);
	elapsed = test_timer_stop(t, &tv);

	XRenderFreePicture(t->dpy, picture);
	XFreePixmap(t->dpy, pixmap);
	test_target_destroy_render(t, &target);

	return elapsed;
}

static void bench(struct test *t, enum target target, int op, int sf)
{
	double real, ref;

	ref = _bench(&t->ref, target, op, sf, 1000);
	real = _bench(&t->real, target, op, sf, 1000);

	fprintf (stdout, "Testing %s with %s: ref=%f, real=%f\n",
		 formats[sf].name, ops[op].name, ref, real);
}

int main(int argc, char **argv)
{
	struct test test;
	unsigned op, sf;

	test_init(&test, argc, argv);

	for (op = 0; op < sizeof(ops)/sizeof(ops[0]); op++) {
		for (sf = 0; sf < sizeof(formats)/sizeof(formats[0]); sf++)
			bench(&test, ROOT, op, sf);
		fprintf (stdout, "\n");
	}

	return 0;
}
