#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xfixes.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include <xf86drm.h>
#include <drm.h>

#include "dri2.h"

#define COUNT 60

static int dri2_open(Display *dpy)
{
	drm_auth_t auth;
	char *driver, *device;
	int fd;

	if (!DRI2Connect(dpy, DefaultRootWindow(dpy), &driver, &device))
		return -1;

	printf ("Connecting to %s driver on %s\n", driver, device);

	fd = open(device, O_RDWR);
	if (fd < 0)
		return -1;

	if (drmIoctl(fd, DRM_IOCTL_GET_MAGIC, &auth))
		return -1;

	if (!DRI2Authenticate(dpy, DefaultRootWindow(dpy), auth.magic))
		return -1;

	return fd;
}

static void dri2_copy_swap(Display *dpy, Drawable d,
			   int width, int height, int has_front)
{
	XRectangle rect;
	XserverRegion region;

	rect.x = 0;
	rect.y = 0;
	rect.width = width;
	rect.height = height;

	region = XFixesCreateRegion(dpy, &rect, 1);
	DRI2CopyRegion(dpy, d, region, DRI2BufferFrontLeft, DRI2BufferBackLeft);
	if (has_front)
		DRI2CopyRegion(dpy, d, region, DRI2BufferFakeFrontLeft, DRI2BufferFrontLeft);
	XFixesDestroyRegion(dpy, region);
}

static void xsync(Display *dpy, Window win)
{
	XImage *image;

	image = XGetImage(dpy, win, 0, 0, 1, 1, ~0, ZPixmap);
	if (image)
		XDestroyImage(image);
}

static double elapsed(const struct timespec *start,
		      const struct timespec *end)
{
	return (end->tv_sec - start->tv_sec) +
		1e-9*(end->tv_nsec - start->tv_nsec);
}

static void run(Display *dpy, int width, int height,
		unsigned int *attachments, int nattachments,
		const char *name)
{
	Window win;
	XSetWindowAttributes attr;
	int count;
	DRI2Buffer *buffers;
	struct timespec start, end;

	/* Be nasty and install a fullscreen window on top so that we
	 * can guarantee we do not get clipped by children.
	 */
	attr.override_redirect = 1;
	win = XCreateWindow(dpy, DefaultRootWindow(dpy),
			 0, 0, width, height, 0,
			 DefaultDepth(dpy, DefaultScreen(dpy)),
			 InputOutput,
			 DefaultVisual(dpy, DefaultScreen(dpy)),
			 CWOverrideRedirect, &attr);
	XMapWindow(dpy, win);
	xsync(dpy, win);

	DRI2CreateDrawable(dpy, win);

	buffers = DRI2GetBuffers(dpy, win, &width, &height,
				 attachments, nattachments, &count);
	if (count != nattachments)
		return;

	xsync(dpy, win);
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (count = 0; count < COUNT; count++)
		DRI2SwapBuffers(dpy, win, 0, 0, 0);
	xsync(dpy, win);
	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("%d %s (%dx%d) swaps in %fs.\n",
	       count, name, width, height, elapsed(&start, &end));

	xsync(dpy, win);
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (count = 0; count < COUNT; count++)
		dri2_copy_swap(dpy, win, width, height, nattachments == 2);
	xsync(dpy, win);
	clock_gettime(CLOCK_MONOTONIC, &end);

	printf("%d %s (%dx%d) blits in %fs.\n",
	       count, name, width, height, elapsed(&start, &end));

	DRI2SwapInterval(dpy, win, 0);

	xsync(dpy, win);
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (count = 0; count < COUNT; count++)
		DRI2SwapBuffers(dpy, win, 0, 0, 0);
	xsync(dpy, win);
	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("%d %s (%dx%d) vblank=0 swaps in %fs.\n",
	       count, name, width, height, elapsed(&start, &end));

	XDestroyWindow(dpy, win);
	free(buffers);

	XSync(dpy, 1);
}

int main(void)
{
	Display *dpy;
	int width, height, fd;
	unsigned int attachments[] = {
		DRI2BufferBackLeft,
		DRI2BufferFrontLeft,
	};

	dpy = XOpenDisplay (NULL);
	if (dpy == NULL)
		return 77;

	fd = dri2_open(dpy);
	if (fd < 0)
		return 1;

	width = WidthOfScreen(DefaultScreenOfDisplay(dpy));
	height = HeightOfScreen(DefaultScreenOfDisplay(dpy));
	run(dpy, width, height, attachments, 1, "fullscreen");
	run(dpy, width, height, attachments, 2, "fullscreen (with front)");

	width /= 2;
	height /= 2;
	run(dpy, width, height, attachments, 1, "windowed");
	run(dpy, width, height, attachments, 2, "windowed (with front)");

	return 0;
}
