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

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <X11/xshmfence.h>
#include <xcb/xcb.h>
#include <xcb/dri3.h>
#include <xcb/sync.h>
#include <unistd.h>

#include "dri3.h"

Pixmap dri3_create_pixmap(Display *dpy,
			  Drawable draw,
			  int width, int height, int depth,
			  int fd, int bpp, int stride, int size)
{
	xcb_connection_t *c = XGetXCBConnection(dpy);
	if (fd >= 0) {
		xcb_pixmap_t pixmap = xcb_generate_id(c);
		xcb_dri3_pixmap_from_buffer(c, pixmap, draw, size, width, height, stride, depth, bpp, fd);
		return pixmap;
	}
	return 0;
}

int dri3_create_fd(Display *dpy,
		   Pixmap pixmap,
		   int *stride)
{
	xcb_connection_t *c = XGetXCBConnection(dpy);
	xcb_dri3_buffer_from_pixmap_cookie_t cookie;
	xcb_dri3_buffer_from_pixmap_reply_t *reply;

	cookie = xcb_dri3_buffer_from_pixmap(c, pixmap);
	reply = xcb_dri3_buffer_from_pixmap_reply(c, cookie, NULL);
	if (!reply)
		return -1;

	if (reply->nfd != 1)
		return -1;

	*stride = reply->stride;
	return xcb_dri3_buffer_from_pixmap_reply_fds(c, reply)[0];
}

int dri3_create_fence(Display *dpy, Pixmap pixmap, struct dri3_fence *fence)
{
	xcb_connection_t *c = XGetXCBConnection(dpy);
	struct dri3_fence f;
	int fd;

	fd = xshmfence_alloc_shm();
	if (fd < 0)
		return -1;

	f.addr = xshmfence_map_shm(fd);
	if (f.addr == NULL) {
		close(fd);
		return -1;
	}

	f.xid = xcb_generate_id(c);
	xcb_dri3_fence_from_fd(c, pixmap, f.xid, 0, fd);

	*fence = f;
	return 0;
}

void dri3_fence_sync(Display *dpy, struct dri3_fence *fence)
{
	xcb_connection_t *c = XGetXCBConnection(dpy);

	xshmfence_reset(fence->addr);

	xcb_sync_trigger_fence(c, fence->xid);
	xcb_flush(c);

	xshmfence_await(fence->addr);
}

void dri3_fence_free(Display *dpy, struct dri3_fence *fence)
{
	xcb_connection_t *c = XGetXCBConnection(dpy);

	xshmfence_unmap_shm(fence->addr);
	xcb_sync_destroy_fence(c, fence->xid);
}

int dri3_open__full(Display *dpy, Window root, unsigned provider)
{
	xcb_connection_t *c = XGetXCBConnection(dpy);
	xcb_dri3_open_cookie_t cookie;
	xcb_dri3_open_reply_t *reply;

	cookie = xcb_dri3_open(c, root, provider);
	reply = xcb_dri3_open_reply(c, cookie, NULL);

	if (!reply)
		return -1;

	if (reply->nfd != 1)
		return -1;

	return xcb_dri3_open_reply_fds(c, reply)[0];
}

int dri3_open(Display *dpy)
{
	return dri3_open__full(dpy, RootWindow(dpy, DefaultScreen(dpy)), None);
}
