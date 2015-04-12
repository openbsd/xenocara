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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <X11/X.h>
#include <X11/Xutil.h> /* for XDestroyImage */
#include <X11/Xlibint.h>
#include <X11/extensions/Xrender.h>
#if HAVE_MIT_SHM
#include <X11/extensions/XShm.h>
#if HAVE_X11_EXTENSIONS_SHMPROTO_H
#include <X11/extensions/shmproto.h>
#elif HAVE_X11_EXTENSIONS_SHMSTR_H
#include <X11/extensions/shmstr.h>
#else
#error Failed to find the right header for X11 MIT-SHM protocol definitions
#endif
#include <sys/ipc.h>
#include <sys/shm.h>
#endif
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
	int value;
	const char *name;
} ops[] = {
	{ PictOpClear, "Clear" },
	{ PictOpSrc, "Src" },
	{ PictOpDst, "Dst" },
	{ PictOpOver, "Over" },
	{ PictOpOverReverse, "OverReverse" },
	{ PictOpIn, "In" },
	{ PictOpInReverse, "InReverse" },
	{ PictOpOut, "Out" },
	{ PictOpOutReverse, "OutReverse" },
	{ PictOpAtop, "Atop" },
	{ PictOpAtopReverse, "AtopReverse" },
	{ PictOpXor, "Xor" },
	{ PictOpAdd, "Add" },
	{ PictOpSaturate, "Saturate" },
	{ PictOpMultiply, "Multiply" },
	{ PictOpScreen, "Screen" },
	{ PictOpOverlay, "Overlay" },
	{ PictOpDarken, "Darken" },
	{ PictOpLighten, "Lighten" },
	{ PictOpColorDodge, "Dodge" },
	{ PictOpColorBurn, "Burn" },
	{ PictOpHardLight, "HardLight" },
	{ PictOpSoftLight, "SoftLight" },
};

static Picture source_pixmap(struct test_display *t, struct test_target *target, int format)
{
	XRenderColor render_color[2] = {
		{ 0x8000, 0x8000, 0x8000, 0x8000 },
		{ 0xffff, 0xffff, 0xffff, 0xffff },
	};
	Pixmap pixmap;
	Picture picture;

	pixmap = XCreatePixmap(t->dpy, t->root,
			       target->width, target->height,
			       PIXMAN_FORMAT_DEPTH(formats[format].pixman_format));

	picture = XRenderCreatePicture(t->dpy, pixmap,
				       XRenderFindStandardFormat(t->dpy, format),
				       0, NULL);
	XFreePixmap(t->dpy, pixmap);

	XRenderFillRectangle(t->dpy, PictOpSrc, picture, &render_color[0],
			     0, 0, target->width, target->height/2);
	XRenderFillRectangle(t->dpy, PictOpSrc, picture, &render_color[1],
			     0, target->height/2, target->width, target->height/2);

	return picture;
}

static Picture source_a8r8g8b8(struct test_display *t, struct test_target *target)
{
	return source_pixmap(t, target, 0);
}

static Picture source_x8r8g8b8(struct test_display *t, struct test_target *target)
{
	return source_pixmap(t, target, 1);
}

static Picture source_a8(struct test_display *t, struct test_target *target)
{
	return source_pixmap(t, target, 2);
}

static Picture source_a4(struct test_display *t, struct test_target *target)
{
	return source_pixmap(t, target, 3);
}

static Picture source_a1(struct test_display *t, struct test_target *target)
{
	return source_pixmap(t, target, 3);
}

static Picture source_1x1r(struct test_display *t, struct test_target *target)
{
	XRenderColor render_color = { 0x8000, 0x8000, 0x8000, 0x8000 };
	XRenderPictureAttributes pa;
	Pixmap pixmap;
	Picture picture;

	pa.repeat = RepeatNormal;

	pixmap = XCreatePixmap(t->dpy, t->root, 1, 1, 32);
	picture = XRenderCreatePicture(t->dpy, pixmap,
				       XRenderFindStandardFormat(t->dpy, 0),
				       CPRepeat, &pa);
	XFreePixmap(t->dpy, pixmap);

	XRenderFillRectangle(t->dpy, PictOpSrc, picture, &render_color,
			     0, 0, 1, 1);

	return picture;
}

static Picture source_solid(struct test_display *t, struct test_target *target)
{
	XRenderColor render_color = { 0x8000, 0x8000, 0x8000, 0x8000 };
	return XRenderCreateSolidFill(t->dpy, &render_color);
}

static Picture source_linear_horizontal(struct test_display *t, struct test_target *target)
{
	XRenderColor colors[2] = {{0}, {0xffff, 0xffff, 0xffff, 0xffff}};
	XFixed stops[2] = {0, 0xffff};
	XLinearGradient gradient = { {0, 0}, {target->width << 16, 0}};

	return XRenderCreateLinearGradient(t->dpy, &gradient, stops, colors, 2);
}

static Picture source_linear_vertical(struct test_display *t, struct test_target *target)
{
	XRenderColor colors[2] = {{0}, {0xffff, 0xffff, 0xffff, 0xffff}};
	XFixed stops[2] = {0, 0xffff};
	XLinearGradient gradient = { {0, 0}, {0, target->height << 16}};

	return XRenderCreateLinearGradient(t->dpy, &gradient, stops, colors, 2);
}

static Picture source_linear_diagonal(struct test_display *t, struct test_target *target)
{
	XRenderColor colors[2] = {{0}, {0xffff, 0xffff, 0xffff, 0xffff}};
	XFixed stops[2] = {0, 0xffff};
	XLinearGradient gradient = { {0, 0}, {target->width << 16, target->height << 16}};

	return XRenderCreateLinearGradient(t->dpy, &gradient, stops, colors, 2);
}

static Picture source_radial_concentric(struct test_display *t, struct test_target *target)
{
	XRenderColor colors[2] = {{0}, {0xffff, 0xffff, 0xffff, 0xffff}};
	XFixed stops[2] = {0, 0xffff};
	XRadialGradient gradient = {
		{
			((target->width << 16) + 1) / 2,
			((target->height << 16) + 1) / 2,
			0,
		},
		{
			((target->width << 16) + 1) / 2,
			((target->height << 16) + 1) / 2,
			target->width << 15,
		}
	};

	return XRenderCreateRadialGradient(t->dpy, &gradient, stops, colors, 2);
}

static Picture source_radial_generic(struct test_display *t, struct test_target *target)
{
	XRenderColor colors[2] = {{0}, {0xffff, 0xffff, 0xffff, 0xffff}};
	XFixed stops[2] = {0, 0xffff};
	XRadialGradient gradient = {
		{ 0, 0, target->width << 14, },
		{ target->width << 16, target->height << 16, target->width << 14, }
	};

	return XRenderCreateRadialGradient(t->dpy, &gradient, stops, colors, 2);
}

#if HAVE_MIT_SHM
static XShmSegmentInfo shmref, shmout;

static void setup_shm(struct test *t)
{
	XShmSegmentInfo shm;
	int size;

	shm.shmid = -1;

	if (!(t->ref.has_shm_pixmaps && t->out.has_shm_pixmaps))
		return;

	size = t->ref.width * t->ref.height * 4;
	size = (size + 4095) & -4096;

	shm.shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
	if (shm.shmid == -1)
		return;

	shm.shmaddr = shmat(shm.shmid, 0, 0);
	if (shm.shmaddr == (char *) -1) {
		shmctl(shm.shmid, IPC_RMID, NULL);
		shm.shmid = -1;
		return;
	}

	shm.readOnly = False;

	shmref = shm;
	XShmAttach(t->ref.dpy, &shmref);
	XSync(t->ref.dpy, True);

	shmout = shm;
	XShmAttach(t->out.dpy, &shmout);
	XSync(t->out.dpy, True);
}

static Picture source_shm(struct test_display *t, struct test_target *target)
{
	XShmSegmentInfo *shm = t->target == REF ? &shmref : &shmout;
	Pixmap pixmap;
	Picture picture;
	int size;

	if (shm->shmid == -1)
		return 0;

	pixmap = XShmCreatePixmap(t->dpy, t->root,
				  shm->shmaddr, shm,
				  target->width, target->height, 32);

	picture = XRenderCreatePicture(t->dpy, pixmap,
				       XRenderFindStandardFormat(t->dpy, 0),
				       0, NULL);
	XFreePixmap(t->dpy, pixmap);

	size = target->width * target->height * 4;
	memset(shm->shmaddr, 0x80, size/2);
	memset(shm->shmaddr+size/2, 0xff, size/2);

	return picture;
}
#else
static void setup_shm(struct test *t) { }
static Picture source_shm(struct test_display *t, struct test_target *target) { return 0; }
#endif

static const struct {
	Picture (*create)(struct test_display *, struct test_target *);
	const char *name;
} source[] = {
	{ source_a8r8g8b8, "a8r8g8b8 pixmap" },
	{ source_x8r8g8b8, "x8r8g8b8 pixmap" },
	{ source_a8, "a8 pixmap" },
	{ source_a4, "a4 pixmap" },
	{ source_a1, "a1 pixmap" },
	{ source_1x1r, "a8r8g8b8 1x1R pixmap" },
	{ source_solid, "solid" },
	{ source_shm, "a8r8g8b8 shm" },
	{ source_linear_horizontal, "linear (horizontal gradient)" },
	{ source_linear_vertical, "linear (vertical gradient)" },
	{ source_linear_diagonal, "linear (diagonal gradient)" },
	{ source_radial_concentric, "radial (concentric)" },
	{ source_radial_generic, "radial (generic)" },
};

static double _bench_source(struct test_display *t, enum target target_type,
			    int op, int src, int loops)
{
	XRenderColor render_color = { 0x8000, 0x8000, 0x8000, 0x8000 };
	struct test_target target;
	Picture picture;
	struct timespec tv;
	double elapsed;

	test_target_create_render(t, target_type, &target);
	XRenderFillRectangle(t->dpy, PictOpClear, target.picture, &render_color,
			     0, 0, target.width, target.height);

	picture = source[src].create(t, &target);
	if (picture) {
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
	} else
		elapsed = -1;

	test_target_destroy_render(t, &target);

	return elapsed;
}

static void bench_source(struct test *t, enum target target, int op, int src)
{
	double out, ref;

	fprintf(stdout, "%28s with %s: ", source[src].name, ops[op].name);
	fflush(stdout);

	op = ops[op].value;

	ref = _bench_source(&t->ref, target, op, src, 1000);
	if (ref < 0) {
		fprintf(stdout, "SKIP\n");
		return;
	}
	fprintf(stdout, "ref=%f, ", ref);
	fflush(stdout);

	out = _bench_source(&t->out, target, op, src, 1000);
	if (out < 0) {
		fprintf(stdout, "SKIP\n");
		return;
	}

	fprintf(stdout, "out=%f\n", out);
}

static double _bench_mask(struct test_display *t, enum target target_type,
			    int op, int src, int mask, int loops)
{
	XRenderColor render_color = { 0x8000, 0x8000, 0x8000, 0x8000 };
	struct test_target target;
	Picture ps, pm;
	struct timespec tv;
	double elapsed;

	test_target_create_render(t, target_type, &target);
	XRenderFillRectangle(t->dpy, PictOpClear, target.picture, &render_color,
			     0, 0, target.width, target.height);

	ps = source[src].create(t, &target);
	pm = source[mask].create(t, &target);
	if (ps && pm) {
		test_timer_start(t, &tv);
		while (loops--)
			XRenderComposite(t->dpy, op,
					 ps, pm, target.picture,
					 0, 0,
					 0, 0,
					 0, 0,
					 target.width, target.height);
		elapsed = test_timer_stop(t, &tv);
	} else
		elapsed = -1;

	if (ps)
		XRenderFreePicture(t->dpy, ps);
	if (pm)
		XRenderFreePicture(t->dpy, pm);

	test_target_destroy_render(t, &target);

	return elapsed;
}

static void bench_mask(struct test *t, enum target target, int op, int src, int mask)
{
	double out, ref;

	fprintf(stdout, "%28s In %28s with %s: ",
		source[src].name, source[mask].name, ops[op].name);
	fflush(stdout);

	op = ops[op].value;

	ref = _bench_mask(&t->ref, target, op, src, mask, 1000);
	if (ref < 0) {
		fprintf(stdout, "SKIP\n");
		return;
	}
	fprintf(stdout, "ref=%f, ", ref);
	fflush(stdout);

	out = _bench_mask(&t->out, target, op, src, mask, 1000);
	if (out < 0) {
		fprintf(stdout, "SKIP\n");
		return;
	}

	fprintf(stdout, "out=%f\n", out);
}

int main(int argc, char **argv)
{
	struct test test;
	unsigned op, src, mask;

	test_init(&test, argc, argv);

	setup_shm(&test);

	for (op = 0; op < sizeof(ops)/sizeof(ops[0]); op++) {
		for (src = 0; src < sizeof(source)/sizeof(source[0]); src++)
			bench_source(&test, ROOT, op, src);
		fprintf (stdout, "\n");

		for (src = 0; src < sizeof(source)/sizeof(source[0]); src++)
			for (mask = 0; mask < sizeof(source)/sizeof(source[0]); mask++)
				bench_mask(&test, ROOT, op, src, mask);
		fprintf (stdout, "\n");
	}

	return 0;
}
