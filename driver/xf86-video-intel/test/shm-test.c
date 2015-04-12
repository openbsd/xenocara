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

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xlibint.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/XShm.h>
#if HAVE_X11_EXTENSIONS_SHMPROTO_H
#include <X11/extensions/shmproto.h>
#elif HAVE_X11_EXTENSIONS_SHMSTR_H
#include <X11/extensions/shmstr.h>
#else
#error Failed to find the right header for X11 MIT-SHM protocol definitions
#endif

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pciaccess.h>

static int _x_error_occurred;

static int
can_use_shm(Display *dpy)
{
	int major, minor, has_pixmap;

	if (!XShmQueryExtension(dpy))
		return 0;

	XShmQueryVersion(dpy, &major, &minor, &has_pixmap);
	return has_pixmap;
}

static int test_subpage(Display *dpy)
{
	const int width = 10;
	const int height = 10;
	uint32_t pixel = 0xffffffff;
	char *expected;
	XShmSegmentInfo shm;
	Pixmap pixmap, source;
	XGCValues gcv;
	GC gc;

	printf("Creating %dx%d SHM pixmap\n", width, height);
	_x_error_occurred = 0;

	expected = malloc(4096);
	if (expected == NULL)
		return 0;

	shm.shmid = shmget(IPC_PRIVATE, 4096, IPC_CREAT | 0666);
	if (shm.shmid == -1)
		return 0;

	shm.shmaddr = shmat(shm.shmid, 0, 0);
	if (shm.shmaddr == (char *) -1) {
		shmctl(shm.shmid, IPC_RMID, NULL);
		return 0;
	}

	shm.readOnly = False;
	XShmAttach(dpy, &shm);

	memset(shm.shmaddr, 0xcc, 4096);
	memset(expected, 0xcc, 4096);
	memset(expected + 64, 0xff, 4*width * height);

	pixmap = XShmCreatePixmap(dpy, DefaultRootWindow(dpy),
				  shm.shmaddr + 64, &shm, width, height, 24);
	XSync(dpy, False);
	shmctl(shm.shmid, IPC_RMID, NULL);

	source = XCreatePixmap(dpy, DefaultRootWindow(dpy),
			       width, height, 24);

	gcv.graphics_exposures = False;
	gcv.subwindow_mode = IncludeInferiors;
	gcv.foreground = pixel;
	gcv.function = GXcopy;
	gcv.fill_style = FillSolid;
	gc = XCreateGC(dpy, pixmap, GCGraphicsExposures | GCSubwindowMode | GCFillStyle | GCForeground | GCFunction, &gcv);

	XCopyArea(dpy, pixmap, source, gc,
		  0, 0, width, height, 0, 0);

	XFillRectangle(dpy, source, gc, 0, 0, width, height);

	XCopyArea(dpy, source, pixmap, gc,
		  0, 0, width, height, 0, 0);
	XSync(dpy, True);

	if (_x_error_occurred == 0)
		_x_error_occurred = memcmp(shm.shmaddr, expected, 4096);

	printf("%s: %s\n", __func__, _x_error_occurred ? "failed" : "passed");

	XShmDetach(dpy, &shm);
	shmdt(shm.shmaddr);
	free(expected);


	return !_x_error_occurred;;
}

static int
_check_error_handler(Display     *display,
		     XErrorEvent *event)
{
	printf("X11 error from display %s, serial=%ld, error=%d, req=%d.%d\n",
	       DisplayString(display),
	       event->serial,
	       event->error_code,
	       event->request_code,
	       event->minor_code);
	_x_error_occurred++;
	return False; /* ignored */
}

int main(void)
{
	Display *dpy;
	int error = 0;

	dpy = XOpenDisplay(NULL);
	if (dpy == NULL)
		return 77;

	if (DefaultDepth(dpy, DefaultScreen(dpy)) != 24)
		return 77;

	if (!can_use_shm(dpy))
		return 0;

	XSetErrorHandler(_check_error_handler);

	error += test_subpage(dpy);

	return !!error;
}
