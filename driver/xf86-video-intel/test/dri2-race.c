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

	fd = open("/dev/dri/card0", O_RDWR);
	if (fd < 0)
		return -1;

	if (drmIoctl(fd, DRM_IOCTL_GET_MAGIC, &auth))
		return -1;

	if (!DRI2Authenticate(dpy, DefaultRootWindow(dpy), auth.magic))
		return -1;

	return fd;
}

static void run(Display *dpy, int width, int height,
		unsigned int *attachments, int nattachments,
		const char *name)
{
	Window win;
	XSetWindowAttributes attr;
	int count, loop;
	DRI2Buffer *buffers;

	/* Be nasty and install a fullscreen window on top so that we
	 * can guarantee we do not get clipped by children.
	 */
	attr.override_redirect = 1;
	loop = 100;
	do {
		win = XCreateWindow(dpy, DefaultRootWindow(dpy),
				    0, 0, width, height, 0,
				    DefaultDepth(dpy, DefaultScreen(dpy)),
				    InputOutput,
				    DefaultVisual(dpy, DefaultScreen(dpy)),
				    CWOverrideRedirect, &attr);
		XMapWindow(dpy, win);

		DRI2CreateDrawable(dpy, win);

		buffers = DRI2GetBuffers(dpy, win, &width, &height,
					 attachments, nattachments, &count);
		if (count != nattachments)
			return;

		free(buffers);
		for (count = 0; count < loop; count++)
			DRI2SwapBuffers(dpy, win, 0, 0, 0);
		XDestroyWindow(dpy, win);
	} while (--loop);

	XSync(dpy, 1);
	sleep(2);
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
