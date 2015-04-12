/*
 * Copyright (c) 2014 Intel Corporation
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <cairo-xlib-xrender.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>

#include <string.h>

static void core_cursor(Display *dpy, int width, int height)
{
	char text[256];
	cairo_surface_t *surface;
	cairo_text_extents_t extents;
	cairo_t *cr;
	Pixmap bitmap;
	int scr = DefaultScreen(dpy);
	Window root = RootWindow(dpy, scr);
	XColor bg, fg;
	int pitch;
	char *data;
	GC gc;

	sprintf(text, "%dx%d", width, height);

	pitch = (width + 31) >> 5 << 2;
	data = calloc(pitch, height);

	surface = cairo_image_surface_create_for_data((unsigned char *)data, CAIRO_FORMAT_A1, width, height, pitch);
	cr = cairo_create(surface);
	cairo_text_extents(cr, text, &extents);
	cairo_move_to(cr, 0, extents.height);
	cairo_show_text(cr, text);
	cairo_destroy(cr);
	cairo_surface_destroy(surface);

	bitmap = XCreatePixmap(dpy, root, width, height, 1);
	gc = XCreateGC(dpy, bitmap, 0, NULL);
	if (gc != NULL) {
		XImage ximage = {
			.height = height,
			.width = width,
			.depth = 1,
			.bits_per_pixel = 1,
			.xoffset = 0,
			.format = XYPixmap,
			.data = data,
			.byte_order = LSBFirst,
			.bitmap_unit = 32,
			.bitmap_bit_order = LSBFirst,
			.bitmap_pad = 32,
			.bytes_per_line = pitch,
		};
		XPutImage(dpy, bitmap, gc, &ximage, 0, 0, 0, 0, width, height);
		XFreeGC(dpy, gc);
	}
	free(data);

	XParseColor(dpy, DefaultColormap(dpy, scr), "green", &fg);
	XParseColor(dpy, DefaultColormap(dpy, scr), "black", &bg);

	XDefineCursor(dpy, root, XCreatePixmapCursor(dpy, bitmap, bitmap, &fg, &bg, 0, 0));
	XFreePixmap(dpy, bitmap);
}

static void render_cursor(Display *dpy, int width, int height)
{
	char text[256];
	cairo_surface_t *surface;
	cairo_text_extents_t extents;
	cairo_t *cr;
	Pixmap pixmap;
	Picture picture;

	sprintf(text, "%dx%d", width, height);

	pixmap = XCreatePixmap(dpy, DefaultRootWindow(dpy), width, height, 32);
	surface = cairo_xlib_surface_create_with_xrender_format(dpy, pixmap,
								DefaultScreenOfDisplay(dpy),
								XRenderFindStandardFormat(dpy, PictStandardARGB32),
								width, height);
	cr = cairo_create(surface);
	cairo_surface_destroy(surface);

	cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
	cairo_paint(cr);
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

	cairo_text_extents(cr, text, &extents);
	cairo_move_to(cr, 0, extents.height);
	cairo_set_source_rgb(cr, 1, 0, 1);
	cairo_show_text(cr, text);
	cairo_destroy(cr);

	picture = XRenderCreatePicture(dpy, pixmap, XRenderFindStandardFormat(dpy, PictStandardARGB32), 0, NULL);
	XFreePixmap(dpy, pixmap);

	XDefineCursor(dpy, DefaultRootWindow(dpy), XRenderCreateCursor(dpy, picture, 0, 0));
	XRenderFreePicture(dpy, picture);
}

int main(void)
{
	Display *dpy;
	int sizes[] = { 24, 32, 48, 64, 72, 128, 160, 256 };
	int x, y;

	dpy = XOpenDisplay(NULL);
	if (dpy == NULL)
		dpy = XOpenDisplay(":0"); /* lazy */
	if (dpy == NULL)
		return 77;

	for (x = 0; x < sizeof(sizes)/sizeof(sizes[0]); x++) {
		for (y = 0; y < sizeof(sizes)/sizeof(sizes[0]); y++) {
			printf("Testing %dx%d (core)\n", sizes[x], sizes[y]);
			core_cursor(dpy, sizes[x], sizes[y]);
			XSync(dpy, True);
			sleep(2);

			printf("Testing %dx%d (render)\n", sizes[x], sizes[y]);
			render_cursor(dpy, sizes[x], sizes[y]);
			XSync(dpy, True);
			sleep(2);
		}
	}

	return 0;
}
