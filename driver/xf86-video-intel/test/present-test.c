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
#include <X11/Xlib-xcb.h>
#include <X11/xshmfence.h>
#include <X11/Xutil.h>
#include <X11/Xlibint.h>
#include <X11/extensions/randr.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/XShm.h>
#if HAVE_X11_EXTENSIONS_SHMPROTO_H
#include <X11/extensions/shmproto.h>
#elif HAVE_X11_EXTENSIONS_SHMSTR_H
#include <X11/extensions/shmstr.h>
#else
#error Failed to find the right header for X11 MIT-SHM protocol definitions
#endif
#include <xcb/xcb.h>
#include <xcb/present.h>
#include <xf86drm.h>
#include <i915_drm.h>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>

#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pciaccess.h>

#include "dri3.h"

#define ALIGN(x, y) (((x) + (y) - 1) & -(y))
#define PAGE_ALIGN(x) ALIGN(x, 4096)

#define GTT I915_GEM_DOMAIN_GTT
#define CPU I915_GEM_DOMAIN_CPU

static int _x_error_occurred;
static uint32_t stamp;

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

static int is_i915_device(int fd)
{
	drm_version_t version;
	char name[5] = "";

	memset(&version, 0, sizeof(version));
	version.name_len = 4;
	version.name = name;

	if (drmIoctl(fd, DRM_IOCTL_VERSION, &version))
		return 0;

	return strcmp("i915", name) == 0;
}

static int is_intel(int fd)
{
	struct drm_i915_getparam gp;
	int ret;

	/* Confirm that this is a i915.ko device with GEM/KMS enabled */
	ret = is_i915_device(fd);
	if (ret) {
		gp.param = I915_PARAM_HAS_GEM;
		gp.value = &ret;
		if (drmIoctl(fd, DRM_IOCTL_I915_GETPARAM, &gp))
			ret = 0;
	}
	return ret;
}

static void *setup_msc(Display *dpy,  Window win)
{
	xcb_connection_t *c = XGetXCBConnection(dpy);
	xcb_void_cookie_t cookie;
	uint32_t id = xcb_generate_id(c);
	xcb_generic_error_t *error;
	void *q;

	cookie = xcb_present_select_input_checked(c, id, win, XCB_PRESENT_EVENT_MASK_COMPLETE_NOTIFY);
	q = xcb_register_for_special_xge(c, &xcb_present_id, id, &stamp);

	error = xcb_request_check(c, cookie);
	assert(error == NULL);

	return q;
}

static uint64_t check_msc(Display *dpy, Window win, void *q, uint64_t last_msc)
{
	xcb_connection_t *c = XGetXCBConnection(dpy);
	uint64_t msc = 0;

	xcb_present_notify_msc(c, win, 0, 0, 0, 0);
	xcb_flush(c);

	do {
		xcb_present_complete_notify_event_t *ce;
		xcb_generic_event_t *ev;

		ev = xcb_wait_for_special_event(c, q);
		if (ev == NULL)
			break;

		ce = (xcb_present_complete_notify_event_t *)ev;
		if (ce->kind != XCB_PRESENT_COMPLETE_KIND_PIXMAP)
			msc = ce->msc;
		free(ev);
	} while (msc == 0);

	if (msc < last_msc) {
		printf("Invalid MSC: was %llu, now %llu\n",
		       (long long)last_msc, (long long)msc);
	}

	return msc;
}

static void teardown_msc(Display *dpy, void *q)
{
	xcb_unregister_for_special_event(XGetXCBConnection(dpy), q);
}
static int test_whole(Display *dpy)
{
	Pixmap pixmap;
	struct dri3_fence fence;
	Window root;
	unsigned int width, height;
	unsigned border, depth;
	int x, y, ret = 1;

	XGetGeometry(dpy, DefaultRootWindow(dpy),
		     &root, &x, &y, &width, &height, &border, &depth);

	if (dri3_create_fence(dpy, root, &fence))
		return 0;

	printf("Testing whole screen flip: %dx%d\n", width, height);
	_x_error_occurred = 0;

	xshmfence_reset(fence.addr);

	pixmap = XCreatePixmap(dpy, root, width, height, depth);
	xcb_present_pixmap(XGetXCBConnection(dpy),
			   root, pixmap,
			   0, /* sbc */
			   0, /* valid */
			   0, /* update */
			   0, /* x_off */
			   0, /* y_off */
			   None,
			   None, /* wait fence */
			   fence.xid,
			   XCB_PRESENT_OPTION_NONE,
			   0, /* target msc */
			   0, /* divisor */
			   0, /* remainder */
			   0, NULL);
	XFreePixmap(dpy, pixmap);

	pixmap = XCreatePixmap(dpy, root, width, height, depth);
	xcb_present_pixmap(XGetXCBConnection(dpy),
			   root, pixmap,
			   0, /* sbc */
			   0, /* valid */
			   0, /* update */
			   0, /* x_off */
			   0, /* y_off */
			   None,
			   None, /* wait fence */
			   None, /* sync fence */
			   XCB_PRESENT_OPTION_NONE,
			   0, /* target msc */
			   0, /* divisor */
			   0, /* remainder */
			   0, NULL);
	XFreePixmap(dpy, pixmap);
	XFlush(dpy);

	ret = !!xshmfence_await(fence.addr);
	dri3_fence_free(dpy, &fence);

	XSync(dpy, True);
	ret += !!_x_error_occurred;

	return ret;
}

static inline XRRScreenResources *_XRRGetScreenResourcesCurrent(Display *dpy, Window window)
{
	XRRScreenResources *res;

	res = XRRGetScreenResourcesCurrent(dpy, window);
	if (res == NULL)
		res = XRRGetScreenResources(dpy, window);

	return res;
}

static XRRModeInfo *lookup_mode(XRRScreenResources *res, int id)
{
	int i;

	for (i = 0; i < res->nmode; i++) {
		if (res->modes[i].id == id)
			return &res->modes[i];
	}

	return NULL;
}

static int for_each_crtc(Display *dpy,
			  int (*func)(Display *dpy,
				      RRCrtc crtc,
				      int width, int height,
				      void *closure),
			  void *closure)
{
	XRRScreenResources *res;
	XRRCrtcInfo **original_crtc;
	int i, j, err = 0;

	if (!XRRQueryVersion(dpy, &i, &j))
		return -1;

	res = _XRRGetScreenResourcesCurrent(dpy, DefaultRootWindow(dpy));
	if (res == NULL)
		return -1;

	original_crtc = malloc(sizeof(XRRCrtcInfo *)*res->ncrtc);
	for (i = 0; i < res->ncrtc; i++)
		original_crtc[i] = XRRGetCrtcInfo(dpy, res, res->crtcs[i]);

	printf("noutput=%d, ncrtc=%d\n", res->noutput, res->ncrtc);

	for (i = 0; i < res->noutput; i++) {
		XRROutputInfo *output;
		XRRModeInfo *mode;

		output = XRRGetOutputInfo(dpy, res, res->outputs[i]);
		if (output == NULL)
			continue;

		mode = NULL;
		if (res->nmode)
			mode = lookup_mode(res, output->modes[0]);

		for (j = 0; mode && j < output->ncrtc; j++) {
			printf("[%d, %d] -- OUTPUT:%ld, CRTC:%ld\n",
			       i, j, (long)res->outputs[i], (long)output->crtcs[j]);
			XRRSetCrtcConfig(dpy, res, output->crtcs[j], CurrentTime,
					 0, 0, output->modes[0], RR_Rotate_0, &res->outputs[i], 1);
			XSync(dpy, True);

			err += func(dpy, output->crtcs[j], mode->width, mode->height, closure);

			XRRSetCrtcConfig(dpy, res, output->crtcs[j], CurrentTime,
					 0, 0, None, RR_Rotate_0, NULL, 0);
			XSync(dpy, True);
		}

		XRRFreeOutputInfo(output);
	}

	for (i = 0; i < res->ncrtc; i++)
		XRRSetCrtcConfig(dpy, res, res->crtcs[i], CurrentTime,
				 original_crtc[i]->x,
				 original_crtc[i]->y,
				 original_crtc[i]->mode,
				 original_crtc[i]->rotation,
				 original_crtc[i]->outputs,
				 original_crtc[i]->noutput);

	free(original_crtc);
	XRRFreeScreenResources(res);

	return j;
}

struct test_crtc {
	Window win;
	int depth;
	unsigned flags;

	struct dri3_fence fence;
	void *queue;
	uint64_t msc;
};
#define SYNC 0x1

static int __test_crtc(Display *dpy, RRCrtc crtc,
		       int width, int height,
		       void *closure)
{
	struct test_crtc *test = closure;
	Pixmap pixmap;
	int err = 0;

	test->msc = check_msc(dpy, test->win, test->queue, test->msc);

	if (test->flags & SYNC)
		xshmfence_reset(test->fence.addr);

	pixmap = XCreatePixmap(dpy, test->win, width, height, test->depth);
	xcb_present_pixmap(XGetXCBConnection(dpy),
			   test->win, pixmap,
			   0, /* sbc */
			   0, /* valid */
			   0, /* update */
			   0, /* x_off */
			   0, /* y_off */
			   crtc,
			   None, /* wait fence */
			   test->flags & SYNC ? test->fence.xid : None,
			   XCB_PRESENT_OPTION_NONE,
			   0, /* target msc */
			   1, /* divisor */
			   0, /* remainder */
			   0, NULL);
	XFreePixmap(dpy, pixmap);

	if (test->flags & SYNC) {
		pixmap = XCreatePixmap(dpy, test->win, width, height, test->depth);
		xcb_present_pixmap(XGetXCBConnection(dpy),
				   test->win, pixmap,
				   1, /* sbc */
				   0, /* valid */
				   0, /* update */
				   0, /* x_off */
				   0, /* y_off */
				   crtc,
				   None, /* wait fence */
				   None, /* sync fence */
				   XCB_PRESENT_OPTION_NONE,
				   1, /* target msc */
				   1, /* divisor */
				   0, /* remainder */
				   0, NULL);
		XFreePixmap(dpy, pixmap);
		XFlush(dpy);
		err += !!xshmfence_await(test->fence.addr);
	}

	test->msc = check_msc(dpy, test->win, test->queue, test->msc);
	return err;
}

static int test_crtc(Display *dpy, void *queue, uint64_t last_msc)
{
	struct test_crtc test;
	int err = 0;

	XSync(dpy, True);
	_x_error_occurred = 0;

	test.win = DefaultRootWindow(dpy);
	test.depth = DefaultDepth(dpy, DefaultScreen(dpy));
	if (dri3_create_fence(dpy, test.win, &test.fence))
		return -1;
	test.queue = queue;
	test.msc = last_msc;

	printf("Testing each crtc, without waiting for each flip\n");
	test.flags = 0;
	err += for_each_crtc(dpy, __test_crtc, &test);

	printf("Testing each crtc, waiting for flips to complete\n");
	test.flags = SYNC;
	err += for_each_crtc(dpy, __test_crtc, &test);

	test.msc = check_msc(dpy, test.win, test.queue, test.msc);
	dri3_fence_free(dpy, &test.fence);

	XSync(dpy, True);
	err += !!_x_error_occurred;

	if (err)
		printf("%s: failures=%d\n", __func__, err);

	return err;
}

static int
can_use_shm(Display *dpy)
{
	int major, minor, has_pixmap;

	if (!XShmQueryExtension(dpy))
		return 0;

	XShmQueryVersion(dpy, &major, &minor, &has_pixmap);
	return has_pixmap;
}

static int test_shm(Display *dpy)
{
	Window win = DefaultRootWindow(dpy);
	XShmSegmentInfo shm;
	Pixmap pixmap;
	Window root;
	unsigned int width, height;
	unsigned border, depth;
	int x, y, ret = 1;

	if (!can_use_shm(dpy))
		return 0;

	_x_error_occurred = 0;

	XGetGeometry(dpy, win, &root, &x, &y,
		     &width, &height, &border, &depth);

	printf("Using %dx%d SHM\n", width, height);

	shm.shmid = shmget(IPC_PRIVATE, height * 4*width, IPC_CREAT | 0666);
	if (shm.shmid == -1)
		return 0;

	shm.shmaddr = shmat(shm.shmid, 0, 0);
	if (shm.shmaddr == (char *) -1)
		goto rmid;

	shm.readOnly = False;
	XShmAttach(dpy, &shm);

	pixmap = XShmCreatePixmap(dpy, DefaultRootWindow(dpy),
				  shm.shmaddr, &shm, width, height, 24);
	if (_x_error_occurred)
		goto detach;

	xcb_present_pixmap(XGetXCBConnection(dpy),
			   win, pixmap,
			   0, /* sbc */
			   0, /* valid */
			   0, /* update */
			   0, /* x_off */
			   0, /* y_off */
			   None,
			   None, /* wait fence */
			   None,
			   XCB_PRESENT_OPTION_NONE,
			   0, /* target msc */
			   0, /* divisor */
			   0, /* remainder */
			   0, NULL);
	XFreePixmap(dpy, pixmap);

	XSync(dpy, True);
	if (_x_error_occurred)
		goto detach;

	ret = 0;
detach:
	XShmDetach(dpy, &shm);
	shmdt(shm.shmaddr);
	XSync(dpy, False);
rmid:
	shmctl(shm.shmid, IPC_RMID, NULL);
	return ret;
}

static uint32_t gem_create(int fd, int size)
{
	struct drm_i915_gem_create create;

	create.handle = 0;
	create.size = size;
	(void)drmIoctl(fd, DRM_IOCTL_I915_GEM_CREATE, &create);

	return create.handle;
}

struct local_i915_gem_caching {
	uint32_t handle;
	uint32_t caching;
};

#define LOCAL_I915_GEM_SET_CACHING	0x2f
#define LOCAL_IOCTL_I915_GEM_SET_CACHING DRM_IOW(DRM_COMMAND_BASE + LOCAL_I915_GEM_SET_CACHING, struct local_i915_gem_caching)

static int gem_set_caching(int fd, uint32_t handle, int caching)
{
	struct local_i915_gem_caching arg;

	arg.handle = handle;
	arg.caching = caching;

	return drmIoctl(fd, LOCAL_IOCTL_I915_GEM_SET_CACHING, &arg) == 0;
}

static int gem_export(int fd, uint32_t handle)
{
	struct drm_prime_handle args;

	args.handle = handle;
	args.flags = O_CLOEXEC;

	if (drmIoctl(fd, DRM_IOCTL_PRIME_HANDLE_TO_FD, &args))
		return -1;

	return args.fd;
}

static void gem_close(int fd, uint32_t handle)
{
	struct drm_gem_close close;

	close.handle = handle;
	(void)drmIoctl(fd, DRM_IOCTL_GEM_CLOSE, &close);
}

static int test_dri3(Display *dpy)
{
	Window win = DefaultRootWindow(dpy);
	Pixmap pixmap;
	Window root;
	unsigned int width, height;
	unsigned border, depth;
	unsigned stride, size;
	int x, y, ret = 1;
	int device, handle;
	int bpp;

	device = dri3_open(dpy);
	if (device < 0)
		return 0;

	if (!is_intel(device))
		return 0;

	printf("Opened Intel DRI3 device\n");

	XGetGeometry(dpy, win, &root, &x, &y,
		     &width, &height, &border, &depth);

	switch (depth) {
	case 8: bpp = 8; break;
	case 15: case 16: bpp = 16; break;
	case 24: case 32: bpp = 32; break;
	default: return 0;
	}

	stride = width * bpp/8;
	size = PAGE_ALIGN(stride * height);
	printf("Creating DRI3 %dx%d (source stride=%d, size=%d) for GTT\n",
	       width, height, stride, size);

	pixmap = 0;
	handle = gem_create(device, size);
	if (handle) {
		pixmap = dri3_create_pixmap(dpy, root,
					     width, height, depth,
					     gem_export(device, handle), bpp, stride, size);
		gem_close(device, handle);
	}
	if (pixmap == 0)
		goto fail;

	xcb_present_pixmap(XGetXCBConnection(dpy),
			   win, pixmap,
			   0, /* sbc */
			   0, /* valid */
			   0, /* update */
			   0, /* x_off */
			   0, /* y_off */
			   None,
			   None, /* wait fence */
			   None,
			   XCB_PRESENT_OPTION_NONE,
			   0, /* target msc */
			   0, /* divisor */
			   0, /* remainder */
			   0, NULL);
	XFreePixmap(dpy, pixmap);

	XSync(dpy, True);
	if (_x_error_occurred)
		goto fail;

	printf("Creating DRI3 %dx%d (source stride=%d, size=%d) for CPU\n",
	       width, height, stride, size);

	pixmap = 0;
	handle = gem_create(device, size);
	if (handle) {
		gem_set_caching(device, handle, CPU);
		handle = dri3_create_pixmap(dpy, root,
					     width, height, depth,
					     gem_export(device, handle), bpp, stride, size);
		gem_close(device, handle);
	}
	if (pixmap == 0)
		goto fail;

	xcb_present_pixmap(XGetXCBConnection(dpy),
			   win, pixmap,
			   0, /* sbc */
			   0, /* valid */
			   0, /* update */
			   0, /* x_off */
			   0, /* y_off */
			   None,
			   None, /* wait fence */
			   None,
			   XCB_PRESENT_OPTION_NONE,
			   0, /* target msc */
			   0, /* divisor */
			   0, /* remainder */
			   0, NULL);
	XFreePixmap(dpy, pixmap);

	XSync(dpy, True);
	if (_x_error_occurred)
		goto fail;

	ret = 0;
fail:
	close(device);
	return ret;
}

static int has_present(Display *dpy)
{
	xcb_connection_t *c = XGetXCBConnection(dpy);
	xcb_present_query_version_reply_t *reply;
	xcb_generic_error_t *error = NULL;

	reply = xcb_present_query_version_reply(c,
						xcb_present_query_version(c,
									  XCB_PRESENT_MAJOR_VERSION,
									  XCB_PRESENT_MINOR_VERSION),
						&error);

	free(reply);
	free(error);

	return reply != NULL;
}

int main(void)
{
	Display *dpy;
	Window root;
	int error = 0;
	uint64_t last_msc;
	void *queue;

	dpy = XOpenDisplay(NULL);
	if (dpy == NULL)
		return 77;

	if (!has_present(dpy))
		return 77;

	root = DefaultRootWindow(dpy);

	signal(SIGALRM, SIG_IGN);
	XSetErrorHandler(_check_error_handler);

	queue = setup_msc(dpy, root);
	last_msc = check_msc(dpy, root, queue, 0);

	error += test_whole(dpy);
	last_msc = check_msc(dpy, root, queue, last_msc);

	error += test_crtc(dpy, queue, last_msc);
	last_msc = check_msc(dpy, root, queue, last_msc);

	error += test_shm(dpy);
	last_msc = check_msc(dpy, root, queue, last_msc);

	error += test_dri3(dpy);
	last_msc = check_msc(dpy, root, queue, last_msc);

	teardown_msc(dpy, queue);

	return !!error;
}
