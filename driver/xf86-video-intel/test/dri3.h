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

#ifndef DRI3_H
#define DRI3_H

#include <X11/X.h>

int dri3_open(Display *dpy);
int dri3_open__full(Display *dpy, Window root, unsigned provider);

Pixmap dri3_create_pixmap(Display *dpy,
			  Drawable draw,
			  int width, int height, int depth,
			  int fd, int bpp, int stride, int size);
int dri3_create_fd(Display *dpy,
		   Pixmap pixmap,
		   int *stride);

struct dri3_fence {
	XID xid;
	void *addr;
};

int dri3_create_fence(Display *dpy, Pixmap pixmap, struct dri3_fence *out);
void dri3_fence_sync(Display *dpy, struct dri3_fence *fence);
void dri3_fence_free(Display *dpy, struct dri3_fence *fence);

#endif /* DRI3_H */
