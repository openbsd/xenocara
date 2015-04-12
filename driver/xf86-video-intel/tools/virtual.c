/*
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <X11/Xlibint.h>
#include <X11/extensions/record.h>
#include <X11/extensions/XShm.h>
#if HAVE_X11_EXTENSIONS_SHMPROTO_H
#include <X11/extensions/shmproto.h>
#elif HAVE_X11_EXTENSIONS_SHMSTR_H
#include <X11/extensions/shmstr.h>
#else
#error Failed to find the right header for X11 MIT-SHM protocol definitions
#endif
#include <X11/extensions/Xdamage.h>
#if HAVE_X11_EXTENSIONS_XINERAMA_H
#include <X11/extensions/Xinerama.h>
#define USE_XINERAMA
#endif
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xrender.h>
#include <X11/Xcursor/Xcursor.h>
#include <pixman.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/timerfd.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <getopt.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#define FORCE_FULL_REDRAW 0
#define FORCE_16BIT_XFER 0

#define DBG(v, x) if (verbose & v) printf x
static int verbose;
#define X11 0x1
#define XRR 0x1
#define TIMER 0x4
#define DRAW 0x8
#define DAMAGE 0x10
#define CURSOR 0x20
#define POLL 0x40

struct display {
	Display *dpy;
	struct clone *clone;
	struct context *ctx;

	int damage_event, damage_error;
	int xfixes_event, xfixes_error;
	int rr_event, rr_error, rr_active;
	int xinerama_event, xinerama_error, xinerama_active;
	int dri3_active;
	Window root;
	Visual *visual;
	Damage damage;

	int width;
	int height;
	int depth;

	XRenderPictFormat *root_format;
	XRenderPictFormat *rgb16_format;
	XRenderPictFormat *rgb24_format;

	int has_shm;
	int has_shm_pixmap;
	int shm_opcode;
	int shm_event;

	Cursor invisible_cursor;
	Cursor visible_cursor;

	XcursorImage cursor_image;
	int cursor_serial;
	int cursor_x;
	int cursor_y;
	int cursor_moved;
	int cursor_visible;
	int cursor;

	int flush;
	int send;
	int skip_clone;
	int skip_frame;
};

struct output {
	struct display *display;
	Display *dpy;
	char *name;
	RROutput rr_output;
	RRCrtc rr_crtc;
	Window window;
	Picture win_picture;
	Picture pix_picture;
	Pixmap pixmap;
	GC gc;

	long serial;
	int use_shm;
	int use_shm_pixmap;
	XShmSegmentInfo shm;

	XRenderPictFormat *use_render;

	int x, y;
	XRRModeInfo mode;
	Rotation rotation;
};

struct clone {
	struct clone *next;
	struct clone *active;

	struct output src, dst;
	long timestamp;

	XShmSegmentInfo shm;
	XImage image;

	int width, height, depth;
	struct { int x1, x2, y1, y2; } damaged;
	int rr_update;

	struct dri3_fence {
		XID xid;
		void *addr;
	} dri3;
};

struct context {
	struct display *display;
	struct clone *clones;
	struct clone *active;
	struct pollfd *pfd;
#define timer pfd[0].fd
	Display *record;
	int nclone;
	int ndisplay;
	int nfd;

	int timer_active;

	long timestamp;
	long configTimestamp;

	Atom singleton;
	char command[1024];
	int command_continuation;
};

static inline int is_power_of_2(unsigned long n)
{
	return n && ((n & (n - 1)) == 0);
}

static int xlib_vendor_is_xorg(Display *dpy)
{
	const char *const vendor = ServerVendor(dpy);
	return strstr(vendor, "X.Org") || strstr(vendor, "Xorg");
}

static inline XRRScreenResources *_XRRGetScreenResourcesCurrent(Display *dpy, Window window)
{
	XRRScreenResources *res;

	res = XRRGetScreenResourcesCurrent(dpy, window);
	if (res == NULL)
		res = XRRGetScreenResources(dpy, window);

	return res;
}

#define XORG_VERSION_ENCODE(major,minor,patch,snap) \
    (((major) * 10000000) + ((minor) * 100000) + ((patch) * 1000) + snap)

static int _x_error_occurred;

static int
_check_error_handler(Display     *display,
		     XErrorEvent *event)
{
	DBG(X11, ("X11 error from display %s, serial=%ld, error=%d, req=%d.%d\n",
	     DisplayString(display),
	     event->serial,
	     event->error_code,
	     event->request_code,
	     event->minor_code));
	_x_error_occurred = 1;
	return False; /* ignored */
}

static int
can_use_shm(Display *dpy,
	    Window window,
	    int *shm_event,
	    int *shm_opcode,
	    int *shm_pixmap)
{
	XShmSegmentInfo shm;
	Status success;
	XExtCodes *codes;
	int major, minor, has_shm, has_pixmap;

	if (!XShmQueryExtension(dpy))
		return 0;

	XShmQueryVersion(dpy, &major, &minor, &has_pixmap);

	shm.shmid = shmget(IPC_PRIVATE, 0x1000, IPC_CREAT | 0600);
	if (shm.shmid == -1)
		return 0;

	shm.readOnly = 0;
	shm.shmaddr = shmat(shm.shmid, NULL, 0);
	if (shm.shmaddr == (char *) -1) {
		shmctl(shm.shmid, IPC_RMID, NULL);
		return 0;
	}

	XSync(dpy, False);
	_x_error_occurred = 0;

	success = XShmAttach(dpy, &shm);

	XSync(dpy, False);
	has_shm = success && _x_error_occurred == 0;

	/* As libXext sets the SEND_EVENT bit in the ShmCompletionEvent,
	 * the Xserver may crash if it does not take care when processing
	 * the event type. For instance versions of Xorg prior to 1.11.1
	 * exhibited this bug, and was fixed by:
	 *
	 * commit 2d2dce558d24eeea0eb011ec9ebaa6c5c2273c39
	 * Author: Sam Spilsbury <sam.spilsbury@canonical.com>
	 * Date:   Wed Sep 14 09:58:34 2011 +0800
	 *
	 * Remove the SendEvent bit (0x80) before doing range checks on event type.
	 */
	codes = 0;
	if (has_shm)
		codes = XInitExtension(dpy, SHMNAME);
	if (xlib_vendor_is_xorg(dpy) &&
	    VendorRelease(dpy) < XORG_VERSION_ENCODE(1,11,0,1))
		codes = 0;
	if (codes) {
		XShmCompletionEvent e;

		memset(&e, 0, sizeof(e));

		e.type = codes->first_event;
		e.send_event = 1;
		e.serial = 1;
		e.drawable = window;
		e.major_code = codes->major_opcode;
		e.minor_code = X_ShmPutImage;

		e.shmseg = shm.shmid;
		e.offset = 0;

		XSendEvent(dpy, e.drawable, False, 0, (XEvent *)&e);
		XSync(dpy, False);

		if (_x_error_occurred == 0) {
			*shm_opcode = codes->major_opcode;
			*shm_event = codes->first_event;
			*shm_pixmap = has_pixmap;
		}
	}

	XShmDetach(dpy, &shm);
	shmctl(shm.shmid, IPC_RMID, NULL);
	shmdt(shm.shmaddr);

	return has_shm;
}

#ifdef DRI3
#include <X11/Xlib-xcb.h>
#include <X11/xshmfence.h>
#include <xcb/xcb.h>
#include <xcb/dri3.h>
#include <xcb/sync.h>
static Pixmap dri3_create_pixmap(Display *dpy,
				 Drawable draw,
				 int width, int height, int depth,
				 int fd, int bpp, int stride, int size)
{
	xcb_connection_t *c = XGetXCBConnection(dpy);
	xcb_pixmap_t pixmap = xcb_generate_id(c);
	xcb_dri3_pixmap_from_buffer(c, pixmap, draw, size, width, height, stride, depth, bpp, fd);
	return pixmap;
}

static int dri3_create_fd(Display *dpy,
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

static int dri3_query_version(Display *dpy, int *major, int *minor)
{
	xcb_connection_t *c = XGetXCBConnection(dpy);
	xcb_dri3_query_version_reply_t *reply;

	*major = *minor = -1;

	reply = xcb_dri3_query_version_reply(c,
					     xcb_dri3_query_version(c,
								    XCB_DRI3_MAJOR_VERSION,
								    XCB_DRI3_MINOR_VERSION),
					     NULL);
	if (reply == NULL)
		return -1;

	*major = reply->major_version;
	*minor = reply->minor_version;
	free(reply);

	return 0;
}

static int dri3_exists(Display *dpy)
{
	int major, minor;

	if (dri3_query_version(dpy, &major, &minor) < 0)
		return 0;

	return major >= 0;
}

static void dri3_create_fence(Display *dpy, Drawable d, struct dri3_fence *fence)
{
	xcb_connection_t *c = XGetXCBConnection(dpy);
	struct dri3_fence f;
	int fd;

	fd = xshmfence_alloc_shm();
	if (fd < 0)
		return;

	f.addr = xshmfence_map_shm(fd);
	if (f.addr == NULL) {
		close(fd);
		return;
	}

	f.xid = xcb_generate_id(c);
	xcb_dri3_fence_from_fd(c, d, f.xid, 0, fd);

	*fence = f;
}

static void dri3_fence_flush(Display *dpy, struct dri3_fence *fence)
{
	xcb_sync_trigger_fence(XGetXCBConnection(dpy), fence->xid);
}

static void dri3_fence_free(Display *dpy, struct dri3_fence *fence)
{
	xshmfence_unmap_shm(fence->addr);
	xcb_sync_destroy_fence(XGetXCBConnection(dpy), fence->xid);
}

#else

static int dri3_exists(Display *dpy)
{
	return 0;
}

static void dri3_create_fence(Display *dpy, Drawable d, struct dri3_fence *fence)
{
}

static void dri3_fence_flush(Display *dpy, struct dri3_fence *fence)
{
}

static void dri3_fence_free(Display *dpy, struct dri3_fence *fence)
{
}

static Pixmap dri3_create_pixmap(Display *dpy,
				 Drawable draw,
				 int width, int height, int depth,
				 int fd, int bpp, int stride, int size)
{
	return None;
}

static int dri3_create_fd(Display *dpy,
			  Pixmap pixmap,
			  int *stride)
{
	return -1;
}
#endif

static int timerfd(int hz)
{
	struct itimerspec it;
	int fd;

	fd = -1;
#ifdef CLOCK_MONOTONIC_COARSE
	fd = timerfd_create(CLOCK_MONOTONIC_COARSE, TFD_NONBLOCK);
#endif
	if (fd < 0)
		fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
	if (fd < 0)
		return -ETIME;

	it.it_interval.tv_sec = 0;
	it.it_interval.tv_nsec = 1000000000 / hz;
	it.it_value = it.it_interval;
	if (timerfd_settime(fd, 0, &it, NULL) < 0) {
		close(fd);
		return -ETIME;
	}

	return fd;
}

static int context_init(struct context *ctx)
{
	struct pollfd *pfd;

	memset(ctx, 0, sizeof(*ctx));

	ctx->pfd = malloc(2*sizeof(struct pollfd));
	if (ctx->pfd == NULL)
		return -ENOMEM;

	ctx->clones = malloc(sizeof(struct clone));
	if (ctx->clones == NULL)
		return -ENOMEM;

	ctx->display = malloc(sizeof(struct display));
	if (ctx->display == NULL)
		return -ENOMEM;

	pfd = memset(&ctx->pfd[ctx->nfd++], 0, sizeof(struct pollfd));
	pfd->fd = timerfd(60);
	if (pfd->fd < 0)
		return pfd->fd;
	pfd->events = POLLIN;

	return 0;
}

static void context_enable_timer(struct context *ctx)
{
	uint64_t count;

	DBG(TIMER, ("%s timer active? %d\n", __func__, ctx->timer_active));

	if (ctx->timer_active)
		return;

	/* reset timer */
	count = read(ctx->timer, &count, sizeof(count));

	ctx->timer_active = 1;
}

static int add_fd(struct context *ctx, int fd)
{
	struct pollfd *pfd;

	if (fd < 0)
		return fd;

	if (is_power_of_2(ctx->nfd)) {
		ctx->pfd = realloc(ctx->pfd, 2*ctx->nfd*sizeof(struct pollfd));
		if (ctx->pfd == NULL)
			return -ENOMEM;
	}

	pfd = memset(&ctx->pfd[ctx->nfd++], 0, sizeof(struct pollfd));
	pfd->fd = fd;
	pfd->events = POLLIN;
	return 0;
}

static void display_mark_flush(struct display *display)
{
	DBG(DRAW, ("%s mark flush (flush=%d)\n",
	     DisplayString(display->dpy), display->flush));

	if (display->flush)
		return;

	context_enable_timer(display->ctx);
	display->flush = 1;
}

static int mode_equal(const XRRModeInfo *a, const XRRModeInfo *b)
{
	return (a->width == b->width &&
		a->height == b->height &&
		a->dotClock == b->dotClock &&
		a->hSyncStart == b->hSyncStart &&
		a->hSyncEnd == b->hSyncEnd &&
		a->hTotal == b->hTotal &&
		a->hSkew == b->hSkew &&
		a->vSyncStart == b->vSyncStart &&
		a->vSyncEnd == b->vSyncEnd &&
		a->vTotal == b->vTotal &&
		a->modeFlags == b->modeFlags);
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

static void clone_update_edid(struct clone *clone)
{
	unsigned long nitems, after;
	unsigned char *data;
	int format;
	Atom type;

	if (XRRGetOutputProperty(clone->dst.dpy, clone->dst.rr_output,
				 XInternAtom(clone->dst.dpy, "EDID", False),
				 0, 100, False, False, AnyPropertyType,
				 &type, &format, &nitems, &after, &data) == Success) {
		XRRChangeOutputProperty(clone->src.dpy, clone->src.rr_output,
					XInternAtom(clone->src.dpy, "EDID", False),
					type, format, PropModeReplace, data, nitems);
	}
}

static int disable_crtc(Display *dpy, XRRScreenResources *res, RRCrtc crtc)
{
	XRRPanning panning;

	if (crtc) {
		XRRSetPanning(dpy, res, crtc, memset(&panning, 0, sizeof(panning)));

		if (XRRSetCrtcConfig(dpy, res, crtc, CurrentTime, 0, 0, None, RR_Rotate_0, NULL, 0) != Success)
			return 0;

		if (XRRSetPanning(dpy, res, crtc, memset(&panning, 0, sizeof(panning))) != Success) {
			DBG(XRR, ("%s failed to clear panning on CRTC:%ld\n", DisplayString(dpy), (long)crtc));
			if (verbose) {
				XRRCrtcInfo *c;
				XRRPanning *p;

				c = XRRGetCrtcInfo(dpy, res, crtc);
				if (c) {
					DBG(XRR, ("%s CRTC:%ld x=%d, y=%d, rotation=%d, mode=%ld\n",
					     DisplayString(dpy), (long)crtc,
					     c->x, c->y, c->rotation, c->mode));
					XRRFreeCrtcInfo(c);
				}

				p = XRRGetPanning(dpy, res, crtc);
				if (p) {
					DBG(XRR, ("%s CRTC:%ld panning (%d, %d)x(%d, %d), tracking (%d, %d)x(%d, %d), border (%d, %d),(%d, %d)\n",
					     DisplayString(dpy), (long)crtc,
					     p->left, p->top, p->width, p->height,
					     p->track_left, p->track_top, p->track_width, p->track_height,
					     p->border_left, p->border_top, p->border_right, p->border_bottom));
					XRRFreePanning(p);
				}
			}
		}
	}

	return 1;
}

static int clone_update_modes__randr(struct clone *clone)
{
	XRRScreenResources *from_res = NULL, *to_res = NULL;
	XRROutputInfo *from_info = NULL, *to_info = NULL;
	int i, j, ret = ENOENT;

	assert(clone->src.rr_output);
	assert(clone->dst.rr_output);
	assert(clone->dst.display->rr_event);

	from_res = _XRRGetScreenResourcesCurrent(clone->dst.dpy, clone->dst.window);
	if (from_res == NULL)
		goto err;

	from_info = XRRGetOutputInfo(clone->dst.dpy, from_res, clone->dst.rr_output);
	if (from_info == NULL)
		goto err;

	DBG(XRR, ("%s(%s-%s <- %s-%s): timestamp %ld (last %ld)\n", __func__,
	     DisplayString(clone->src.dpy), clone->src.name,
	     DisplayString(clone->dst.dpy), clone->dst.name,
	     from_info->timestamp, clone->timestamp));

	to_res = _XRRGetScreenResourcesCurrent(clone->src.dpy, clone->src.window);
	if (to_res == NULL)
		goto err;

	to_info = XRRGetOutputInfo(clone->src.dpy, to_res, clone->src.rr_output);
	if (to_info == NULL)
		goto err;

	DBG(XRR, ("%s: dst.rr_crtc=%ld, now %ld\n",
	     __func__, (long)clone->dst.rr_crtc, (long)from_info->crtc));
	if (clone->dst.rr_crtc == from_info->crtc) {
		for (i = 0; i < to_info->nmode; i++) {
			XRRModeInfo *mode, *old;

			mode = lookup_mode(to_res, to_info->modes[i]);
			if (mode == NULL)
				break;

			DBG(XRR, ("%s(%s-%s): lookup mode %s\n", __func__,
			     DisplayString(clone->src.dpy), clone->src.name,
			     mode->name));

			for (j = 0; j < from_info->nmode; j++) {
				old = lookup_mode(from_res, from_info->modes[j]);
				if (old && mode_equal(mode, old)) {
					mode = NULL;
					break;
				}
			}
			if (mode) {
				DBG(XRR, ("%s(%s-%s): unknown mode %s\n", __func__,
				     DisplayString(clone->src.dpy), clone->src.name,
				     mode->name));
				break;
			}
		}
		if (i == from_info->nmode && i == to_info->nmode) {
			DBG(XRR, ("%s(%s-%s): no change in output\n", __func__,
			     DisplayString(clone->src.dpy), clone->src.name));
			goto done;
		}
	}

	/* Disable the remote output */
	if (from_info->crtc != clone->dst.rr_crtc) {
		DBG(XRR, ("%s(%s-%s): disabling active CRTC\n", __func__,
		     DisplayString(clone->dst.dpy), clone->dst.name));
		if (disable_crtc(clone->dst.dpy, from_res, from_info->crtc)) {
			clone->dst.rr_crtc = 0;
			clone->dst.mode.id = 0;
		} else {
			XRRCrtcInfo *c = XRRGetCrtcInfo(clone->dst.dpy, from_res, from_info->crtc);
			if (c) {
				clone->dst.x = c->x;
				clone->dst.y = c->y;
				clone->dst.rotation = c->rotation;
				clone->dst.mode.id = c->mode;
				XRRFreeCrtcInfo(c);
			}
		}
	}

	/* Create matching modes for the real output on the virtual */
	XGrabServer(clone->src.dpy);

	/* Clear all current UserModes on the output, including any active ones */
	if (to_info->crtc) {
		DBG(XRR, ("%s(%s-%s): disabling active CRTC\n", __func__,
		     DisplayString(clone->src.dpy), clone->src.name));
		disable_crtc(clone->src.dpy, to_res, to_info->crtc);
	}
	for (i = 0; i < to_info->nmode; i++) {
		DBG(XRR, ("%s(%s-%s): deleting mode %ld\n", __func__,
		     DisplayString(clone->src.dpy), clone->src.name, (long)to_info->modes[i]));
		XRRDeleteOutputMode(clone->src.dpy, clone->src.rr_output, to_info->modes[i]);
	}

	clone->src.rr_crtc = 0;

	for (i = 0; i < from_info->nmode; i++) {
		XRRModeInfo *mode, *old;
		RRMode id;

		mode = lookup_mode(from_res, from_info->modes[i]);
		if (mode == NULL)
			continue;
		for (j = 0; j < i; j++) {
			old = lookup_mode(from_res, from_info->modes[j]);
			if (old && mode_equal(mode, old)) {
				mode = NULL;
				break;
			}
		}
		if (mode == NULL)
			continue;

		id = 0;
		for (j = 0; j < to_res->nmode; j++) {
			old = &to_res->modes[j];
			if (mode_equal(mode, old)) {
				id = old->id;
				DBG(XRR, ("%s(%s-%s): reusing mode %ld: %s\n", __func__,
				     DisplayString(clone->src.dpy), clone->src.name, id, mode->name));
				break;
			}
		}
		if (id == 0) {
			XRRModeInfo m;
			char buf[256];

			/* XXX User names must be unique! */
			m = *mode;
			m.nameLength = snprintf(buf, sizeof(buf),
						"%s.%ld-%s", clone->src.name, (long)from_info->modes[i], mode->name);
			m.name = buf;

			id = XRRCreateMode(clone->src.dpy, clone->src.window, &m);
			DBG(XRR, ("%s(%s-%s): adding mode %ld: %s\n", __func__,
			     DisplayString(clone->src.dpy), clone->src.name, id, mode->name));
		}

		XRRAddOutputMode(clone->src.dpy, clone->src.rr_output, id);
	}
	clone_update_edid(clone);
	XUngrabServer(clone->src.dpy);
done:
	ret = 0;
	clone->timestamp = from_info->timestamp;

err:
	if (to_info)
		XRRFreeOutputInfo(to_info);
	if (to_res)
		XRRFreeScreenResources(to_res);
	if (from_info)
		XRRFreeOutputInfo(from_info);
	if (from_res)
		XRRFreeScreenResources(from_res);

	return ret;
}

static int clone_update_modes__fixed(struct clone *clone)
{
	char mode_name[80];
	XRRScreenResources *res = NULL;
	XRROutputInfo *info = NULL;
	XRRModeInfo mode;
	RRMode id;
	int i, j, ret = ENOENT;

	assert(clone->src.rr_output);

	res = _XRRGetScreenResourcesCurrent(clone->src.dpy, clone->src.window);
	if (res == NULL)
		goto err;

	info = XRRGetOutputInfo(clone->src.dpy, res, clone->src.rr_output);
	if (info == NULL)
		goto err;

	XGrabServer(clone->src.dpy);

	/* Clear all current UserModes on the output, including any active ones */
	if (info->crtc) {
		DBG(XRR, ("%s(%s-%s): disabling active CRTC\n", __func__,
		     DisplayString(clone->src.dpy), clone->src.name));
		disable_crtc(clone->src.dpy, res, info->crtc);
	}
	for (i = 0; i < info->nmode; i++) {
		DBG(XRR, ("%s(%s-%s): deleting mode %ld\n", __func__,
		     DisplayString(clone->src.dpy), clone->src.name, (long)info->modes[i]));
		XRRDeleteOutputMode(clone->src.dpy, clone->src.rr_output, info->modes[i]);
	}

	clone->src.rr_crtc = 0;

	/* Create matching mode for the real output on the virtual */
	memset(&mode, 0, sizeof(mode));
	mode.width = clone->width;
	mode.height = clone->height;
	mode.nameLength = sprintf(mode_name, "FAKE-%dx%d", mode.width, mode.height);
	mode.name = mode_name;

	id = 0;
	for (j = 0; j < res->nmode; j++) {
		if (mode_equal(&mode, &res->modes[j])) {
			id = res->modes[j].id;
			break;
		}
	}
	if (id == 0)
		id = XRRCreateMode(clone->src.dpy, clone->src.window, &mode);

	XRRAddOutputMode(clone->src.dpy, clone->src.rr_output, id);

	XUngrabServer(clone->src.dpy);
	ret = 0;
err:
	if (info)
		XRRFreeOutputInfo(info);
	if (res)
		XRRFreeScreenResources(res);

	return ret;
}

static RROutput claim_virtual(struct display *display, char *output_name, int nclone)
{
	char mode_name[] = "ClaimVirtualHead";
	Display *dpy = display->dpy;
	XRRScreenResources *res;
	XRROutputInfo *output;
	XRRModeInfo mode;
	RRMode id;
	RROutput rr_output = 0;
	int i;

	DBG(X11, ("%s(%d)\n", __func__, nclone));
	XGrabServer(dpy);

	res = _XRRGetScreenResourcesCurrent(dpy, display->root);
	if (res == NULL)
		goto out;

	sprintf(output_name, "VIRTUAL%d", nclone);

	for (i = rr_output = 0; rr_output == 0 && i < res->noutput; i++) {
		output = XRRGetOutputInfo(dpy, res, res->outputs[i]);
		if (output == NULL)
			continue;

		if (strcmp(output->name, output_name) == 0)
			rr_output = res->outputs[i];

		XRRFreeOutputInfo(output);
	}
	for (i = id = 0; id == 0 && i < res->nmode; i++) {
		if (strcmp(res->modes[i].name, mode_name) == 0)
			id = res->modes[i].id;
	}
	XRRFreeScreenResources(res);

	DBG(XRR, ("%s(%s): rr_output=%ld\n", __func__, output_name, (long)rr_output));
	if (rr_output == 0)
		goto out;

	/* Set any mode on the VirtualHead to make the Xserver allocate another */
	memset(&mode, 0, sizeof(mode));
	mode.width = 1024;
	mode.height = 768;
	mode.name = mode_name;
	mode.nameLength = sizeof(mode_name) - 1;

	if (id == 0)
		id = XRRCreateMode(dpy, display->root, &mode);
	XRRAddOutputMode(dpy, rr_output, id);

	/* Force a redetection for the ddx to spot the new outputs */
	res = XRRGetScreenResources(dpy, display->root);
	if (res == NULL)
		goto out;

	/* Some else may have interrupted us and installed that new mode! */
	output = XRRGetOutputInfo(dpy, res, rr_output);
	if (output) {
		disable_crtc(dpy, res, output->crtc);
		XRRFreeOutputInfo(output);
	}
	XRRFreeScreenResources(res);

	XRRDeleteOutputMode(dpy, rr_output, id);
	XRRDestroyMode(dpy, id);

	/* And hide it again */
	res = XRRGetScreenResources(dpy, display->root);
	if (res != NULL)
		XRRFreeScreenResources(res);
out:
	XUngrabServer(dpy);

	return rr_output;
}

static int stride_for_depth(int width, int depth)
{
	if (depth == 24)
		depth = 32;
	return ((width * depth + 7) / 8 + 3) & ~3;
}

static void init_image(struct clone *clone)
{
	XImage *image = &clone->image;
	int ret;

	image->width = clone->width;
	image->height = clone->height;
	image->format = ZPixmap;
	image->xoffset = 0;
	image->byte_order = LSBFirst;
	image->bitmap_unit = 32;
	image->bitmap_bit_order = LSBFirst;
	image->bitmap_pad = 32;
	image->data = clone->shm.shmaddr;
	image->bytes_per_line = stride_for_depth(clone->width, clone->depth);
	switch (clone->depth) {
	case 24:
		image->red_mask = 0xff << 16;
		image->green_mask = 0xff << 8;
		image->blue_mask = 0xff << 0;;
		image->depth = 24;
		image->bits_per_pixel = 32;
		break;
	case 16:
		image->red_mask = 0x1f << 11;
		image->green_mask = 0x3f << 5;
		image->blue_mask = 0x1f << 0;;
		image->depth = 16;
		image->bits_per_pixel = 16;
		break;
	}

	ret = XInitImage(image);
	assert(ret);
	(void)ret;
}

static int mode_height(const XRRModeInfo *mode, Rotation rotation)
{
	switch (rotation & 0xf) {
	case RR_Rotate_0:
	case RR_Rotate_180:
		return mode->height;
	case RR_Rotate_90:
	case RR_Rotate_270:
		return mode->width;
	default:
		return 0;
	}
}

static int mode_width(const XRRModeInfo *mode, Rotation rotation)
{
	switch (rotation & 0xf) {
	case RR_Rotate_0:
	case RR_Rotate_180:
		return mode->width;
	case RR_Rotate_90:
	case RR_Rotate_270:
		return mode->height;
	default:
		return 0;
	}
}

static void output_init_xfer(struct clone *clone, struct output *output)
{
	if (output->pixmap == None && output->use_shm_pixmap) {
		DBG(DRAW, ("%s-%s: creating shm pixmap\n", DisplayString(output->dpy), output->name));
		XSync(output->dpy, False);
		_x_error_occurred = 0;

		output->pixmap = XShmCreatePixmap(output->dpy, output->window,
						  clone->shm.shmaddr, &output->shm,
						  clone->width, clone->height, clone->depth);
		if (output->pix_picture) {
			XRenderFreePicture(output->dpy, output->pix_picture);
			output->pix_picture = None;
		}

		XSync(output->dpy, False);
		if (_x_error_occurred) {
			XFreePixmap(output->dpy, output->pixmap);
			output->pixmap = None;
			output->use_shm_pixmap = 0;
		}
	}
	if (output->use_render) {
		DBG(DRAW, ("%s-%s: creating picture\n", DisplayString(output->dpy), output->name));
		if (output->win_picture == None)
			output->win_picture = XRenderCreatePicture(output->dpy, output->window,
								   output->display->root_format, 0, NULL);
		if (output->pixmap == None)
			output->pixmap = XCreatePixmap(output->dpy, output->window,
						       clone->width, clone->height, clone->depth);
		if (output->pix_picture == None)
			output->pix_picture = XRenderCreatePicture(output->dpy, output->pixmap,
								   output->use_render, 0, NULL);
	}

	if (output->gc == None) {
		XGCValues gcv;

		DBG(DRAW, ("%s-%s: creating gc\n", DisplayString(output->dpy), output->name));

		gcv.graphics_exposures = False;
		gcv.subwindow_mode = IncludeInferiors;

		output->gc = XCreateGC(output->dpy, output->pixmap ?: output->window, GCGraphicsExposures | GCSubwindowMode, &gcv);
	}
}

static int bpp_for_depth(int depth)
{
	switch (depth) {
	case 1: return 1;
	case 8: return 8;
	case 15: return 16;
	case 16: return 16;
	case 24: return 24;
	case 32: return 32;
	default: return 0;
	}
}

static int clone_init_xfer(struct clone *clone)
{
	int width, height;

	if (clone->dst.mode.id == 0) {
		clone->width = 0;
		clone->height = 0;
	} else if (clone->dri3.xid) {
		width = clone->dst.display->width;
		height = clone->dst.display->height;
	} else {
		width = mode_width(&clone->src.mode, clone->src.rotation);
		height = mode_height(&clone->src.mode, clone->src.rotation);
	}

	if (width == clone->width && height == clone->height)
		return 0;

	DBG(DRAW, ("%s-%s create xfer, %dx%d\n",
	     DisplayString(clone->dst.dpy), clone->dst.name,
	     width, height));

	if (clone->shm.shmaddr) {
		if (clone->src.use_shm)
			XShmDetach(clone->src.dpy, &clone->src.shm);
		if (clone->dst.use_shm)
			XShmDetach(clone->dst.dpy, &clone->dst.shm);

		shmdt(clone->shm.shmaddr);
		clone->shm.shmaddr = NULL;
	}

	if (clone->src.pixmap) {
		XFreePixmap(clone->src.dpy, clone->src.pixmap);
		clone->src.pixmap = 0;
	}

	if (clone->dst.pixmap) {
		XFreePixmap(clone->dst.dpy, clone->dst.pixmap);
		clone->dst.pixmap = 0;
	}

	if ((width | height) == 0) {
		clone->damaged.x2 = clone->damaged.y2 = INT_MIN;
		clone->damaged.x1 = clone->damaged.y1 = INT_MAX;
		return 0;
	}

	if (clone->dri3.xid) {
		int fd, stride;
		Pixmap src;

		_x_error_occurred = 0;

		DBG(DRAW, ("%s-%s create xfer, trying DRI3\n",
		     DisplayString(clone->dst.dpy), clone->dst.name));

		fd = dri3_create_fd(clone->dst.dpy, clone->dst.window, &stride);
		if (fd < 0)
			goto disable_dri3;

		DBG(DRAW, ("%s-%s create xfer, DRI3 fd=%d, stride=%d\n",
		     DisplayString(clone->dst.dpy), clone->dst.name,
		     fd, stride));

		src = dri3_create_pixmap(clone->src.dpy, clone->src.window,
					 width, height, clone->depth,
					 fd, bpp_for_depth(clone->depth),
					 stride, lseek(fd, 0, SEEK_END));

		XSync(clone->src.dpy, False);
		if (!_x_error_occurred) {
			clone->src.pixmap = src;
			clone->width = width;
			clone->height = height;
		} else {
			XFreePixmap(clone->src.dpy, src);
			close(fd);
disable_dri3:
			dri3_fence_free(clone->src.dpy, &clone->dri3);
			clone->dri3.xid = 0;

			DBG(DRAW, ("%s-%s create xfer, DRI3 failed\n",
			     DisplayString(clone->dst.dpy), clone->dst.name));
		}
	}

	width = mode_width(&clone->src.mode, clone->src.rotation);
	height = mode_height(&clone->src.mode, clone->src.rotation);

	if (!clone->dri3.xid) {
		DBG(DRAW, ("%s-%s create xfer, trying SHM\n",
		     DisplayString(clone->dst.dpy), clone->dst.name));

		clone->shm.shmid = shmget(IPC_PRIVATE,
					  height * stride_for_depth(width, clone->depth),
					  IPC_CREAT | 0666);
		if (clone->shm.shmid == -1)
			return errno;

		clone->shm.shmaddr = shmat(clone->shm.shmid, 0, 0);
		if (clone->shm.shmaddr == (char *) -1) {
			shmctl(clone->shm.shmid, IPC_RMID, NULL);
			return ENOMEM;
		}

		if (clone->src.use_shm) {
			clone->src.shm = clone->shm;
			clone->src.shm.readOnly = False;
			XShmAttach(clone->src.dpy, &clone->src.shm);
			XSync(clone->src.dpy, False);
		}
		if (clone->dst.use_shm) {
			clone->dst.shm = clone->shm;
			clone->dst.shm.readOnly = !clone->dst.use_shm_pixmap;
			XShmAttach(clone->dst.dpy, &clone->dst.shm);
			XSync(clone->dst.dpy, False);
		}

		shmctl(clone->shm.shmid, IPC_RMID, NULL);

		clone->width = width;
		clone->height = height;

		init_image(clone);
	}

	output_init_xfer(clone, &clone->src);
	output_init_xfer(clone, &clone->dst);

	clone->damaged.x1 = clone->src.x;
	clone->damaged.x2 = clone->src.x + width;
	clone->damaged.y1 = clone->src.y;
	clone->damaged.y2 = clone->src.y + height;

	display_mark_flush(clone->dst.display);
	return 0;
}

static void clone_update(struct clone *clone)
{
	if (!clone->rr_update)
		return;

	DBG(X11, ("%s-%s cloning modes\n",
	     DisplayString(clone->dst.dpy), clone->dst.name));

	clone_update_modes__randr(clone);
	clone->rr_update = 0;
}

static int context_update(struct context *ctx)
{
	Display *dpy = ctx->display->dpy;
	XRRScreenResources *res;
	int context_changed = 0;
	int i, n;

	DBG(X11, ("%s\n", __func__));

	res = _XRRGetScreenResourcesCurrent(dpy, ctx->display->root);
	if (res == NULL)
		return 0;

	DBG(XRR, ("%s timestamp %ld (last %ld), config %ld (last %ld)\n",
	     DisplayString(dpy),
	     res->timestamp, ctx->timestamp,
	     res->configTimestamp, ctx->configTimestamp));
	if (res->timestamp == ctx->timestamp &&
	    res->configTimestamp == ctx->configTimestamp &&
	    res->timestamp != res->configTimestamp) { /* mutter be damned */
		XRRFreeScreenResources(res);
		return 0;
	}

	ctx->timestamp = res->timestamp;
	ctx->configTimestamp = res->configTimestamp;

	for (n = 0; n < ctx->nclone; n++) {
		struct output *output = &ctx->clones[n].src;
		XRROutputInfo *o;
		XRRCrtcInfo *c;
		RRMode mode = 0;
		int changed = 0;

		o = XRRGetOutputInfo(dpy, res, output->rr_output);
		if (o == NULL)
			continue;

		c = NULL;
		if (o->crtc)
			c = XRRGetCrtcInfo(dpy, res, o->crtc);
		if (c) {
			DBG(XRR, ("%s-%s: (x=%d, y=%d, rotation=%d, mode=%ld) -> (x=%d, y=%d, rotation=%d, mode=%ld)\n",
			     DisplayString(dpy), output->name,
			     output->x, output->y, output->rotation, output->mode.id,
			     c->x, c->y, c->rotation, c->mode));

			changed |= output->rotation != c->rotation;
			output->rotation = c->rotation;

			changed |= output->x != c->x;
			output->x = c->x;

			changed |= output->y != c->y;
			output->y = c->y;

			changed |= output->mode.id != c->mode;
			mode = c->mode;
			XRRFreeCrtcInfo(c);
		} else {
			DBG(XRR, ("%s-%s: (x=%d, y=%d, rotation=%d, mode=%ld) -> off\n",
			     DisplayString(dpy), output->name,
			     output->x, output->y, output->rotation, output->mode.id));
		}
		output->rr_crtc = o->crtc;
		XRRFreeOutputInfo(o);

		DBG(XRR, ("%s-%s crtc changed? %d\n",
		     DisplayString(ctx->clones[n].dst.display->dpy), ctx->clones[n].dst.name, changed));

		if (mode) {
			if (output->mode.id != mode) {
				for (i = 0; i < res->nmode; i++) {
					if (res->modes[i].id == mode) {
						output->mode = res->modes[i];
						break;
					}
				}
			}
		} else {
			changed = output->mode.id != 0;
			output->mode.id = 0;
		}

		DBG(XRR, ("%s-%s output changed? %d\n",
		     DisplayString(ctx->clones[n].dst.display->dpy), ctx->clones[n].dst.name, changed));

		context_changed |= changed;
	}
	XRRFreeScreenResources(res);

	DBG(XRR, ("%s changed? %d\n", DisplayString(dpy), context_changed));
	if (!context_changed)
		return 0;

	for (n = 1; n < ctx->ndisplay; n++) {
		struct display *display = &ctx->display[n];
		struct clone *clone;
		int x1, x2, y1, y2;

		if (display->rr_active == 0)
			continue;

		x1 = y1 = INT_MAX;
		x2 = y2 = INT_MIN;

		for (clone = display->clone; clone; clone = clone->next) {
			struct output *output = &clone->src;
			int v;

			assert(clone->dst.display == display);

			if (output->mode.id == 0)
				continue;

			DBG(XRR, ("%s: source %s enabled (%d, %d)x(%d, %d)\n",
			     DisplayString(clone->dst.dpy), output->name,
			     output->x, output->y,
			     mode_width(&output->mode, output->rotation),
			     mode_height(&output->mode, output->rotation)));

			if (output->x < x1)
				x1 = output->x;
			if (output->y < y1)
				y1 = output->y;

			v = (int)output->x + mode_width(&output->mode, output->rotation);
			if (v > x2)
				x2 = v;
			v = (int)output->y + mode_height(&output->mode, output->rotation);
			if (v > y2)
				y2 = v;
		}

		DBG(XRR, ("%s fb bounds (%d, %d)x(%d, %d)\n", DisplayString(display->dpy),
		     x1, y1, x2, y2));

		XGrabServer(display->dpy);
		res = _XRRGetScreenResourcesCurrent(display->dpy, display->root);
		if (res == NULL)
			goto ungrab;

		if (x2 <= x1 || y2 <= y1) {
			/* Nothing enabled, preserve the current fb, and turn everything off */
			for (clone = display->clone; clone; clone = clone->next) {
				struct output *dst = &clone->dst;

				if (!dst->rr_crtc)
					continue;

				DBG(XRR, ("%s: disabling output '%s'\n",
				     DisplayString(display->dpy), dst->name));
				assert(clone->dst.display == display);
				if (disable_crtc(display->dpy, res, dst->rr_crtc)) {
					dst->rr_crtc = 0;
					dst->mode.id = 0;
				}
			}
			goto free_res;
		}

		x2 -= x1;
		y2 -= y1;
		DBG(XRR, ("%s: current size %dx%d, need %dx%d\n",
		     DisplayString(display->dpy),
		     display->width, display->height,
		     x2, y2));

		if (display->width != x2 || display->height != y2) {
			/* When shrinking we have to manually resize the fb */
			for (clone = display->clone; clone; clone = clone->next) {
				struct output *dst = &clone->dst;

				if (!dst->rr_crtc)
					continue;

				DBG(XRR, ("%s: disabling output '%s'\n",
				     DisplayString(display->dpy), dst->name));
				assert(clone->dst.display == display);
				if (disable_crtc(display->dpy, res, dst->rr_crtc)) {
					dst->rr_crtc = 0;
					dst->mode.id = 0;
				}
			}

			DBG(XRR, ("%s: XRRSetScreenSize %dx%d\n", DisplayString(display->dpy), x2, y2));
			XRRSetScreenSize(display->dpy, display->root, x2, y2, x2 * 96 / 25.4, y2 * 96 / 25.4);
			display->width = x2;
			display->height = y2;
		}

		for (clone = display->clone; clone; clone = clone->next) {
			struct output *src = &clone->src;
			struct output *dst = &clone->dst;
			XRROutputInfo *o;
			XRRPanning panning;
			struct clone *set;
			RRCrtc rr_crtc;
			Status ret;

			DBG(XRR, ("%s: copying configuration from %s (mode=%ld: %dx%d) to %s\n",
			     DisplayString(display->dpy),
			     src->name, (long)src->mode.id, src->mode.width, src->mode.height,
			     dst->name));

			if (src->mode.id == 0) {
err:
				if (dst->rr_crtc) {
					DBG(XRR, ("%s: disabling unused output '%s'\n",
					     DisplayString(display->dpy), dst->name));
					assert(clone->dst.display == display);
					if (disable_crtc(display->dpy, res, dst->rr_crtc)) {
						dst->rr_crtc = 0;
						dst->mode.id = 0;
					}
				}
				continue;
			}

			dst->x = src->x - x1;
			dst->y = src->y - y1;
			dst->rotation = src->rotation;
			dst->mode = src->mode;

			dst->mode.id = 0;
			for (i = 0; i < res->nmode; i++) {
				if (mode_equal(&src->mode, &res->modes[i])) {
					dst->mode.id = res->modes[i].id;
					break;
				}
			}
			if (dst->mode.id == 0) {
				XRRModeInfo m;
				char buf[256];
				RRMode id;

				/* XXX User names must be unique! */
				m = src->mode;
				m.nameLength = snprintf(buf, sizeof(buf),
							"%s.%ld-%dx%d", src->name,
							(long)src->mode.id,
							src->mode.width,
							src->mode.height);
				m.name = buf;

				id = XRRCreateMode(dst->dpy, dst->window, &m);
				if (id) {
					DBG(XRR, ("%s: adding mode %ld: %dx%d to %s, new mode %ld\n",
					     DisplayString(dst->dpy),
					     (long)src->mode.id,
					     src->mode.width,
					     src->mode.height,
					     dst->name, (long)id));
					XRRAddOutputMode(dst->dpy, dst->rr_output, id);
					dst->mode.id = id;
				} else {
					DBG(XRR, ("%s: failed to find suitable mode for %s\n",
					     DisplayString(dst->dpy), dst->name));
					goto err;
				}
			}

			rr_crtc = dst->rr_crtc;
			if (rr_crtc) {
				for (set = display->clone; set != clone; set = set->next) {
					if (set->dst.rr_crtc == rr_crtc) {
						DBG(XRR, ("%s: CRTC reassigned from %s\n",
						     DisplayString(dst->dpy), dst->name));
						rr_crtc = 0;
						break;
					}
				}
			}
			if (rr_crtc == 0) {
				o = XRRGetOutputInfo(dst->dpy, res, dst->rr_output);
				for (i = 0; i < o->ncrtc; i++) {
					DBG(XRR, ("%s: checking whether CRTC:%ld is available\n",
					     DisplayString(dst->dpy), (long)o->crtcs[i]));
					for (set = display->clone; set != clone; set = set->next) {
						if (set->dst.rr_crtc == o->crtcs[i]) {
							DBG(XRR, ("%s: CRTC:%ld already assigned to %s\n",
							     DisplayString(dst->dpy), (long)o->crtcs[i], set->dst.name));
							break;
						}
					}
					if (set == clone) {
						rr_crtc = o->crtcs[i];
						break;
					}
				}
				XRRFreeOutputInfo(o);
			}
			if (rr_crtc == 0) {
				DBG(XRR, ("%s: failed to find available CRTC for %s\n",
				     DisplayString(dst->dpy), dst->name));
				goto err;
			}

			DBG(XRR, ("%s: enabling output '%s' (%d,%d)x(%d,%d), rotation %d, on CRTC:%ld, using mode %ld\n",
			     DisplayString(dst->dpy), dst->name,
			     dst->x, dst->y, dst->mode.width, dst->mode.height,
			     dst->rotation, (long)rr_crtc, dst->mode.id));

			ret = XRRSetPanning(dst->dpy, res, rr_crtc, memset(&panning, 0, sizeof(panning)));
			DBG(XRR, ("%s-%s: XRRSetPanning %s\n", DisplayString(dst->dpy), dst->name, ret ? "failed" : "success"));
			(void)ret;

			ret = XRRSetCrtcConfig(dst->dpy, res, rr_crtc, CurrentTime,
					       dst->x, dst->y, dst->mode.id, dst->rotation,
					       &dst->rr_output, 1);
			DBG(XRR, ("%s-%s: XRRSetCrtcConfig %s\n", DisplayString(dst->dpy), dst->name, ret ? "failed" : "success"));
			if (ret)
				goto err;

			if (verbose & XRR) {
				XRRCrtcInfo *c;
				XRRPanning *p;

				c = XRRGetCrtcInfo(dst->dpy, res, rr_crtc);
				if (c) {
					DBG(XRR, ("%s-%s: x=%d, y=%d, rotation=%d, mode=%ld\n",
					     DisplayString(dst->dpy), dst->name,
					     c->x, c->y, c->rotation, c->mode));
					XRRFreeCrtcInfo(c);
				}

				p = XRRGetPanning(dst->dpy, res, rr_crtc);
				if (p) {
					DBG(XRR, ("%s-%s: panning (%d, %d)x(%d, %d), tracking (%d, %d)x(%d, %d), border (%d, %d),(%d, %d)\n",
					     DisplayString(dst->dpy), dst->name,
					     p->left, p->top, p->width, p->height,
					     p->track_left, p->track_top, p->track_width, p->track_height,
					     p->border_left, p->border_top, p->border_right, p->border_bottom));
					XRRFreePanning(p);
				}
			}

			dst->rr_crtc = rr_crtc;
		}
free_res:
		XRRFreeScreenResources(res);
ungrab:
		XUngrabServer(display->dpy);
	}

	ctx->active = NULL;
	for (n = 0; n < ctx->nclone; n++) {
		struct clone *clone = &ctx->clones[n];

		clone_init_xfer(clone);

		if (clone->dst.rr_crtc == 0)
			continue;

		DBG(XRR, ("%s-%s: added to active list\n",
		     DisplayString(clone->dst.display->dpy), clone->dst.name));

		clone->active = ctx->active;
		ctx->active = clone;
	}

	return 1;
}

static Cursor display_load_invisible_cursor(struct display *display)
{
	char zero[8] = {};
	XColor black = {};
	Pixmap bitmap = XCreateBitmapFromData(display->dpy, display->root, zero, 8, 8);
	return XCreatePixmapCursor(display->dpy, bitmap, bitmap, &black, &black, 0, 0);
}

static Cursor display_get_visible_cursor(struct display *display)
{
	if (display->cursor_serial != display->cursor_image.size) {
		DBG(CURSOR, ("%s updating cursor\n", DisplayString(display->dpy)));

		if (display->visible_cursor)
			XFreeCursor(display->dpy, display->visible_cursor);

		display->visible_cursor = XcursorImageLoadCursor(display->dpy, &display->cursor_image);
		display->cursor_serial = display->cursor_image.size;
	}

	return display->visible_cursor;
}

static void display_load_visible_cursor(struct display *display, XFixesCursorImage *cur)
{
	unsigned long *src; /* XXX deep sigh */
	XcursorPixel *dst;
	unsigned n;

	if (cur->width != display->cursor_image.width ||
	    cur->height != display->cursor_image.height)
		display->cursor_image.pixels = realloc(display->cursor_image.pixels,
						       4 * cur->width * cur->height);
	if (display->cursor_image.pixels == NULL)
		return;

	display->cursor_image.width  = cur->width;
	display->cursor_image.height = cur->height;
	display->cursor_image.xhot = cur->xhot;
	display->cursor_image.yhot = cur->yhot;
	display->cursor_image.size++;

	n = cur->width*cur->height;
	src = cur->pixels;
	dst = display->cursor_image.pixels;
	while (n--)
		*dst++ = *src++;

	DBG(CURSOR, ("%s marking cursor changed\n", DisplayString(display->dpy)));
	display->cursor_moved++;
	if (display->cursor != display->invisible_cursor) {
		display->cursor_visible++;
		context_enable_timer(display->ctx);
	}
}

static void display_cursor_move(struct display *display, int x, int y, int visible)
{
	DBG(CURSOR, ("%s cursor moved (visible=%d, (%d, %d))\n",
	     DisplayString(display->dpy), visible, x, y));
	display->cursor_moved++;
	display->cursor_visible += visible;
	if (visible) {
		display->cursor_x = x;
		display->cursor_y = y;
	}

	context_enable_timer(display->ctx);
}

static void display_flush_cursor(struct display *display)
{
	Cursor cursor;
	int x, y;

	if (!display->cursor_moved)
		return;

	if (display->cursor_visible) {
		x = display->cursor_x;
		y = display->cursor_y;
	} else {
		x = display->cursor_x++ & 31;
		y = display->cursor_y++ & 31;
	}

	DBG(CURSOR, ("%s setting cursor position (%d, %d), visible? %d\n",
	     DisplayString(display->dpy), x, y, display->cursor_visible));
	XWarpPointer(display->dpy, None, display->root, 0, 0, 0, 0, x, y);

	cursor = None;
	if (display->cursor_visible)
		cursor = display_get_visible_cursor(display);
	if (cursor == None)
		cursor = display->invisible_cursor;
	if (cursor != display->cursor) {
		XDefineCursor(display->dpy, display->root, cursor);
		display->cursor = cursor;
	}

	display_mark_flush(display);

	display->cursor_moved = 0;
	display->cursor_visible = 0;
}

static void clone_move_cursor(struct clone *c, int x, int y)
{
	int visible;

	DBG(CURSOR, ("%s-%s moving cursor (%d, %d) [(%d, %d), (%d, %d)]\n",
	     DisplayString(c->dst.dpy), c->dst.name,
	     x, y,
	     c->src.x, c->src.y,
	     c->src.x + c->width, c->src.y + c->height));

	visible = (x >= c->src.x && x < c->src.x + c->width &&
		   y >= c->src.y && y < c->src.y + c->height);

	x += c->dst.x - c->src.x;
	y += c->dst.y - c->src.y;

	display_cursor_move(c->dst.display, x, y, visible);
}

static int clone_output_init(struct clone *clone, struct output *output,
			     struct display *display, const char *name,
			     RROutput rr_output)
{
	Display *dpy = display->dpy;
	int depth;

	DBG(X11, ("%s(%s, %s)\n", __func__, DisplayString(dpy), name));

	output->name = strdup(name);
	if (output->name == NULL)
		return -ENOMEM;

	output->display = display;
	output->dpy = dpy;

	output->rr_output = rr_output;
	output->rotation = RR_Rotate_0;

	output->window = display->root;
	output->use_shm = display->has_shm;
	output->use_shm_pixmap = display->has_shm_pixmap;

	DBG(X11, ("%s-%s use shm? %d (use shm pixmap? %d)\n",
	     DisplayString(dpy), name, display->has_shm, display->has_shm_pixmap));

	depth = output->use_shm && !FORCE_16BIT_XFER ? display->depth : 16;
	if (depth < clone->depth)
		clone->depth = depth;

	return 0;
}

static void ximage_prepare(XImage *image, int width, int height)
{
	image->width = width;
	image->height = height;
	image->bytes_per_line = stride_for_depth(width, image->depth);
}

static void get_src(struct clone *c, const XRectangle *clip)
{
	DBG(DRAW,("%s-%s get_src(%d,%d)x(%d,%d)\n", DisplayString(c->dst.dpy), c->dst.name,
	     clip->x, clip->y, clip->width, clip->height));

	c->image.obdata = (char *)&c->src.shm;

	if (c->src.use_render) {
		XRenderComposite(c->src.dpy, PictOpSrc,
				 c->src.win_picture, 0, c->src.pix_picture,
				 clip->x, clip->y,
				 0, 0,
				 0, 0,
				 clip->width, clip->height);
		if (c->src.use_shm_pixmap) {
			XSync(c->src.dpy, False);
		} else if (c->src.use_shm) {
			ximage_prepare(&c->image, clip->width, clip->height);
			XShmGetImage(c->src.dpy, c->src.pixmap, &c->image,
				     clip->x, clip->y, AllPlanes);
		} else {
			ximage_prepare(&c->image, c->width, c->height);
			XGetSubImage(c->src.dpy, c->src.pixmap,
				     clip->x, clip->y, clip->width, clip->height,
				     AllPlanes, ZPixmap,
				     &c->image, 0, 0);
		}
	} else if (c->src.pixmap) {
		XCopyArea(c->src.dpy, c->src.window, c->src.pixmap, c->src.gc,
			  clip->x, clip->y,
			  clip->width, clip->height,
			  0, 0);
		XSync(c->src.dpy, False);
	} else if (c->src.use_shm) {
		ximage_prepare(&c->image, clip->width, clip->height);
		XShmGetImage(c->src.dpy, c->src.window, &c->image,
			     clip->x, clip->y, AllPlanes);
	} else {
		ximage_prepare(&c->image, c->width, c->height);
		XGetSubImage(c->src.dpy, c->src.window,
			     clip->x, clip->y, clip->width, clip->height,
			     AllPlanes, ZPixmap,
			     &c->image, 0, 0);
	}
	c->src.display->flush = 0;
}

static void put_dst(struct clone *c, const XRectangle *clip)
{
	DBG(DRAW, ("%s-%s put_dst(%d,%d)x(%d,%d)\n", DisplayString(c->dst.dpy), c->dst.name,
	     clip->x, clip->y, clip->width, clip->height));

	c->image.obdata = (char *)&c->dst.shm;

	if (c->dst.use_render) {
		if (c->dst.use_shm_pixmap) {
			DBG(DRAW, ("%s-%s using SHM pixmap composite\n",
			     DisplayString(c->dst.dpy), c->dst.name));
		} else if (c->dst.use_shm) {
			DBG(DRAW, ("%s-%s using SHM image composite\n",
			     DisplayString(c->dst.dpy), c->dst.name));
			XShmPutImage(c->dst.dpy, c->dst.pixmap, c->dst.gc, &c->image,
				     0, 0,
				     0, 0,
				     clip->width, clip->height,
				     False);
		} else {
			DBG(DRAW, ("%s-%s using composite\n",
			     DisplayString(c->dst.dpy), c->dst.name));
			XPutImage(c->dst.dpy, c->dst.pixmap, c->dst.gc, &c->image,
				  0, 0,
				  0, 0,
				  clip->width, clip->height);
		}
		if (c->dst.use_shm)
			c->dst.serial = NextRequest(c->dst.dpy);
		XRenderComposite(c->dst.dpy, PictOpSrc,
				 c->dst.pix_picture, 0, c->dst.win_picture,
				 0, 0,
				 0, 0,
				 clip->x, clip->y,
				 clip->width, clip->height);
		c->dst.display->send |= c->dst.use_shm;
	} else if (c->dst.pixmap) {
		DBG(DRAW, ("%s-%s using SHM pixmap\n",
		     DisplayString(c->dst.dpy), c->dst.name));
		c->dst.serial = NextRequest(c->dst.dpy);
		XCopyArea(c->dst.dpy, c->dst.pixmap, c->dst.window, c->dst.gc,
			  0, 0,
			  clip->width, clip->height,
			  clip->x, clip->y);
		c->dst.display->send = 1;
	} else if (c->dst.use_shm) {
		DBG(DRAW, ("%s-%s using SHM image\n",
		     DisplayString(c->dst.dpy), c->dst.name));
		c->dst.serial = NextRequest(c->dst.dpy);
		XShmPutImage(c->dst.dpy, c->dst.window, c->dst.gc, &c->image,
			     0, 0,
			     clip->x, clip->y,
			     clip->width, clip->height,
			     True);
	} else {
		DBG(DRAW, ("%s-%s using image\n",
		     DisplayString(c->dst.dpy), c->dst.name));
		XPutImage(c->dst.dpy, c->dst.window, c->dst.gc, &c->image,
			  0, 0,
			  clip->x, clip->y,
			  clip->width, clip->height);
		c->dst.serial = 0;
	}
}

static int clone_paint(struct clone *c)
{
	XRectangle clip;

	DBG(DRAW, ("%s-%s paint clone, damaged (%d, %d), (%d, %d) [(%d, %d), (%d,  %d)]\n",
	     DisplayString(c->dst.dpy), c->dst.name,
	     c->damaged.x1, c->damaged.y1,
	     c->damaged.x2, c->damaged.y2,
	     c->src.x, c->src.y,
	     c->src.x + c->width, c->src.y + c->height));

	if (c->damaged.x1 < c->src.x)
		c->damaged.x1 = c->src.x;
	if (c->damaged.x2 > c->src.x + c->width)
		c->damaged.x2 = c->src.x + c->width;
	if (c->damaged.x2 <= c->damaged.x1)
		goto done;

	if (c->damaged.y1 < c->src.y)
		c->damaged.y1 = c->src.y;
	if (c->damaged.y2 > c->src.y + c->height)
		c->damaged.y2 = c->src.y + c->height;
	if (c->damaged.y2 <= c->damaged.y1)
		goto done;

	DBG(DRAW, ("%s-%s is damaged, last SHM serial: %ld, now %ld\n",
	     DisplayString(c->dst.dpy), c->dst.name,
	     (long)c->dst.serial, (long)LastKnownRequestProcessed(c->dst.dpy)));
	if (c->dst.serial > LastKnownRequestProcessed(c->dst.dpy)) {
		struct pollfd pfd;

		pfd.fd = ConnectionNumber(c->dst.dpy);
		pfd.events = POLLIN;
		XEventsQueued(c->dst.dpy,
			      poll(&pfd, 1, 0) ? QueuedAfterReading : QueuedAfterFlush);

		if (c->dst.serial > LastKnownRequestProcessed(c->dst.dpy)) {
			c->dst.display->skip_clone++;
			return EAGAIN;
		}
	}

	c->dst.display->skip_clone = 0;
	c->dst.display->skip_frame = 0;

	if (FORCE_FULL_REDRAW) {
		c->damaged.x1 = c->src.x;
		c->damaged.y1 = c->src.y;
		c->damaged.x2 = c->src.x + c->width;
		c->damaged.y2 = c->src.y + c->height;
	}

	if (c->dri3.xid) {
		if (c->src.use_render) {
			XRenderComposite(c->src.dpy, PictOpSrc,
					 c->src.win_picture, 0, c->src.pix_picture,
					 c->damaged.x1, c->damaged.y1,
					 0, 0,
					 c->damaged.x1 + c->dst.x - c->src.x,
					 c->damaged.y1 + c->dst.y - c->src.y,
					 c->damaged.x2 - c->damaged.x1,
					 c->damaged.y2 - c->damaged.y1);
		} else {
			XCopyArea(c->src.dpy, c->src.window, c->src.pixmap, c->src.gc,
				  c->damaged.x1, c->damaged.y1,
				  c->damaged.x2 - c->damaged.x1,
				  c->damaged.y2 - c->damaged.y1,
				  c->damaged.x1 + c->dst.x - c->src.x,
				  c->damaged.y1 + c->dst.y - c->src.y);
		}
		dri3_fence_flush(c->src.dpy, &c->dri3);
	} else {
		clip.x = c->damaged.x1;
		clip.y = c->damaged.y1;
		clip.width  = c->damaged.x2 - c->damaged.x1;
		clip.height = c->damaged.y2 - c->damaged.y1;
		get_src(c, &clip);

		clip.x += c->dst.x - c->src.x;
		clip.y += c->dst.y - c->src.y;
		put_dst(c, &clip);
	}
	display_mark_flush(c->dst.display);

done:
	c->damaged.x2 = c->damaged.y2 = INT_MIN;
	c->damaged.x1 = c->damaged.y1 = INT_MAX;
	return 0;
}

static void clone_damage(struct clone *c, const XRectangle *rec)
{
	int v;

	if ((v = rec->x) < c->damaged.x1)
		c->damaged.x1 = v;
	if ((v = (int)rec->x + rec->width) > c->damaged.x2)
		c->damaged.x2 = v;
	if ((v = rec->y) < c->damaged.y1)
		c->damaged.y1 = v;
	if ((v = (int)rec->y + rec->height) > c->damaged.y2)
		c->damaged.y2 = v;

	DBG(DAMAGE, ("%s-%s damaged: (%d, %d), (%d, %d)\n",
	     DisplayString(c->dst.display->dpy), c->dst.name,
	     c->damaged.x1, c->damaged.y1,
	     c->damaged.x2, c->damaged.y2));
}

static void usage(const char *arg0)
{
	printf("Usage: %s [OPTION]... [TARGET_DISPLAY]...\n", arg0);
	printf("  -d <source display>  source display\n");
	printf("  -f                   keep in foreground (do not detach from console and daemonize)\n");
	printf("  -b                   start bumblebee\n");
	printf("  -a                   connect to all local displays (e.g. :1, :2, etc)\n");
	printf("  -S                   disable use of a singleton and launch a fresh intel-virtual-output process\n");
	printf("  -v                   all verbose output, implies -f\n");
	printf("  -V <category>        specific verbose output, implies -f\n");
	printf("  -h                   this help\n");
	printf("If no target displays are parsed on the commandline, \n");
	printf("intel-virtual-output will attempt to connect to any local display\n");
	printf("and then start bumblebee.\n");
}

static void record_callback(XPointer closure, XRecordInterceptData *data)
{
	struct context *ctx = (struct context *)closure;

	DBG(X11, ("%s\n", __func__));

	if (data->category == XRecordFromServer) {
		const xEvent *e = (const xEvent *)data->data;

		DBG(X11, ("%s -- from server, event type %d, root %ld (ours? %d)\n",
		     __func__, e->u.u.type, (long)e->u.keyButtonPointer.root,
		     ctx->display->root == e->u.keyButtonPointer.root));

		if (e->u.u.type == MotionNotify &&
		    e->u.keyButtonPointer.root == ctx->display->root) {
			struct clone *clone;

			for (clone = ctx->active; clone; clone = clone->active)
				clone_move_cursor(clone,
						  e->u.keyButtonPointer.rootX,
						  e->u.keyButtonPointer.rootY);
		}
	}

	XRecordFreeData(data);
}

static int record_mouse(struct context *ctx)
{
	Display *dpy;
	XRecordRange *rr;
	XRecordClientSpec rcs;
	XRecordContext rc;

	DBG(X11, ("%s(%s)\n", __func__, DisplayString(ctx->display->dpy)));

	dpy = XOpenDisplay(DisplayString(ctx->display->dpy));
	if (dpy == NULL)
		return -ECONNREFUSED;

	rr = XRecordAllocRange();
	if (rr == NULL)
		return -ENOMEM;

	rr->device_events.first = rr->device_events.last = MotionNotify;

	rcs = XRecordAllClients;
	rc = XRecordCreateContext(dpy, 0, &rcs, 1, &rr, 1);

	XSync(dpy, False);

	if (!XRecordEnableContextAsync(dpy, rc, record_callback, (XPointer)ctx))
		return -EINVAL;

	ctx->record = dpy;
	return ConnectionNumber(dpy);
}

static int bad_visual(Visual *visual, int depth)
{
	DBG(X11, ("%s? depth=%d, visual: class=%d, bits_per_rgb=%d, red_mask=%08lx, green_mask=%08lx, blue_mask=%08lx\n",
	     __func__, depth,
	     visual->class,
	     visual->bits_per_rgb,
	     visual->red_mask,
	     visual->green_mask,
	     visual->blue_mask));

	if (!(visual->class == TrueColor || visual->class == DirectColor))
		return 1;

	switch (depth) {
	case 16: return (/* visual->bits_per_rgb != 6          || */
			 visual->red_mask     != 0x1f << 11 ||
			 visual->green_mask   != 0x3f << 5  ||
			 visual->blue_mask    != 0x1f << 0);

	case 24: return (/* visual->bits_per_rgb != 8          || */
			 visual->red_mask     != 0xff << 16 ||
			 visual->green_mask   != 0xff << 8  ||
			 visual->blue_mask    != 0xff << 0);

	default: return 1;
	}
}

static XRenderPictFormat *
find_xrender_format(Display *dpy, pixman_format_code_t format)
{
    XRenderPictFormat tmpl;
    int mask;

#define MASK(x) ((1<<(x))-1)

    memset(&tmpl, 0, sizeof(tmpl));

    tmpl.depth = PIXMAN_FORMAT_DEPTH(format);
    mask = PictFormatType | PictFormatDepth;

    DBG(X11, ("%s(0x%08lx)\n", __func__, (long)format));

    switch (PIXMAN_FORMAT_TYPE(format)) {
    case PIXMAN_TYPE_ARGB:
	tmpl.type = PictTypeDirect;

	if (PIXMAN_FORMAT_A(format)) {
		tmpl.direct.alphaMask = MASK(PIXMAN_FORMAT_A(format));
		tmpl.direct.alpha = (PIXMAN_FORMAT_R(format) +
				     PIXMAN_FORMAT_G(format) +
				     PIXMAN_FORMAT_B(format));
	}

	tmpl.direct.redMask = MASK(PIXMAN_FORMAT_R(format));
	tmpl.direct.red = (PIXMAN_FORMAT_G(format) +
			   PIXMAN_FORMAT_B(format));

	tmpl.direct.greenMask = MASK(PIXMAN_FORMAT_G(format));
	tmpl.direct.green = PIXMAN_FORMAT_B(format);

	tmpl.direct.blueMask = MASK(PIXMAN_FORMAT_B(format));
	tmpl.direct.blue = 0;

	mask |= PictFormatRed | PictFormatRedMask;
	mask |= PictFormatGreen | PictFormatGreenMask;
	mask |= PictFormatBlue | PictFormatBlueMask;
	mask |= PictFormatAlpha | PictFormatAlphaMask;
	break;

    case PIXMAN_TYPE_ABGR:
	tmpl.type = PictTypeDirect;

	if (tmpl.direct.alphaMask) {
		tmpl.direct.alphaMask = MASK(PIXMAN_FORMAT_A(format));
		tmpl.direct.alpha = (PIXMAN_FORMAT_B(format) +
				     PIXMAN_FORMAT_G(format) +
				     PIXMAN_FORMAT_R(format));
	}

	tmpl.direct.blueMask = MASK(PIXMAN_FORMAT_B(format));
	tmpl.direct.blue = (PIXMAN_FORMAT_G(format) +
			    PIXMAN_FORMAT_R(format));

	tmpl.direct.greenMask = MASK(PIXMAN_FORMAT_G(format));
	tmpl.direct.green = PIXMAN_FORMAT_R(format);

	tmpl.direct.redMask = MASK(PIXMAN_FORMAT_R(format));
	tmpl.direct.red = 0;

	mask |= PictFormatRed | PictFormatRedMask;
	mask |= PictFormatGreen | PictFormatGreenMask;
	mask |= PictFormatBlue | PictFormatBlueMask;
	mask |= PictFormatAlpha | PictFormatAlphaMask;
	break;

    case PIXMAN_TYPE_BGRA:
	tmpl.type = PictTypeDirect;

	tmpl.direct.blueMask = MASK(PIXMAN_FORMAT_B(format));
	tmpl.direct.blue = (PIXMAN_FORMAT_BPP(format) - PIXMAN_FORMAT_B(format));

	tmpl.direct.greenMask = MASK(PIXMAN_FORMAT_G(format));
	tmpl.direct.green = (PIXMAN_FORMAT_BPP(format) - PIXMAN_FORMAT_B(format) -
			     PIXMAN_FORMAT_G(format));

	tmpl.direct.redMask = MASK(PIXMAN_FORMAT_R(format));
	tmpl.direct.red = (PIXMAN_FORMAT_BPP(format) - PIXMAN_FORMAT_B(format) -
			   PIXMAN_FORMAT_G(format) - PIXMAN_FORMAT_R(format));

	if (tmpl.direct.alphaMask) {
		tmpl.direct.alphaMask = MASK(PIXMAN_FORMAT_A(format));
		tmpl.direct.alpha = 0;
	}

	mask |= PictFormatRed | PictFormatRedMask;
	mask |= PictFormatGreen | PictFormatGreenMask;
	mask |= PictFormatBlue | PictFormatBlueMask;
	mask |= PictFormatAlpha | PictFormatAlphaMask;
	break;

    case PIXMAN_TYPE_A:
	tmpl.type = PictTypeDirect;

	tmpl.direct.alpha = 0;
	tmpl.direct.alphaMask = MASK(PIXMAN_FORMAT_A(format));

	mask |= PictFormatAlpha | PictFormatAlphaMask;
	break;

    case PIXMAN_TYPE_COLOR:
    case PIXMAN_TYPE_GRAY:
	/* XXX Find matching visual/colormap */
	tmpl.type = PictTypeIndexed;
	//tmpl.colormap = screen->visuals[PIXMAN_FORMAT_VIS(format)].vid;
	//mask |= PictFormatColormap;
	return NULL;
    }
#undef MASK

    return XRenderFindFormat(dpy, mask, &tmpl, 0);
}

static int display_init_render(struct display *display, int depth, XRenderPictFormat **use_render)
{
	Display *dpy = display->dpy;
	int major, minor;

	DBG(X11, ("%s is depth %d, want %d\n", DisplayString(dpy), display->depth, depth));

	*use_render = 0;
	if (depth == display->depth && !bad_visual(display->visual, depth))
		return 0;

	if (display->root_format == 0) {
		if (!XRenderQueryVersion(dpy, &major, &minor)) {
			fprintf(stderr, "Render extension not supported by %s\n", DisplayString(dpy));
			return -EINVAL;
		}

		display->root_format = XRenderFindVisualFormat(dpy, display->visual);
		display->rgb16_format = find_xrender_format(dpy, PIXMAN_r5g6b5);
		display->rgb24_format = XRenderFindStandardFormat(dpy, PictStandardRGB24);

		DBG(X11, ("%s: root format=%lx, rgb16 format=%lx, rgb24 format=%lx\n",
		     DisplayString(dpy),
		     (long)display->root_format,
		     (long)display->rgb16_format,
		     (long)display->rgb24_format));
	}

	switch (depth) {
	case 16: *use_render = display->rgb16_format; break;
	case 24: *use_render = display->rgb24_format; break;
	}
	if (*use_render == 0)
		return -ENOENT;

	return 0;
}

static int clone_init_depth(struct clone *clone)
{
	int ret, depth;

	DBG(X11,("%s-%s wants depth %d\n",
	     DisplayString(clone->dst.dpy), clone->dst.name, clone->depth));

	ret = -1;
	for (depth = clone->depth; depth <= 24; depth += 8) {
		ret = display_init_render(clone->src.display, depth, &clone->src.use_render);
		if (ret)
			continue;

		ret = display_init_render(clone->dst.display, depth, &clone->dst.use_render);
		if (ret)
			continue;

		break;
	}
	if (ret)
		return ret;

	DBG(X11, ("%s-%s using depth %d, requires xrender for src? %d, for dst? %d\n",
	     DisplayString(clone->dst.dpy), clone->dst.name,
	     clone->depth,
	     clone->src.use_render != NULL,
	     clone->dst.use_render != NULL));

	if (!clone->dst.use_render &&
	    clone->src.display->dri3_active &&
	    clone->dst.display->dri3_active)
		dri3_create_fence(clone->src.dpy, clone->src.window, &clone->dri3);

	return 0;
}

#if defined(USE_XINERAMA)
static int xinerama_active(struct display *display)
{
	int active = 0;
	if (XineramaQueryExtension(display->dpy, &display->xinerama_event, &display->xinerama_error))
		active = XineramaIsActive(display->dpy);
	return active;
}
#else
#define xinerama_active(d) 0
#endif

static int add_display(struct context *ctx, Display *dpy)
{
	struct display *display;
	int first_display = ctx->ndisplay == 0;

	if (is_power_of_2(ctx->ndisplay)) {
		struct display *new_display;

		new_display = realloc(ctx->display, 2*ctx->ndisplay*sizeof(struct display));
		if (new_display == NULL)
			return -ENOMEM;

		if (new_display != ctx->display) {
			int n;

			for (n = 0; n < ctx->nclone; n++) {
				struct clone *clone = &ctx->clones[n];
				clone->src.display = new_display + (clone->src.display - ctx->display);
				clone->dst.display = new_display + (clone->dst.display - ctx->display);
			}
		}

		ctx->display = new_display;
	}

	display = memset(&ctx->display[ctx->ndisplay++], 0, sizeof(struct display));

	display->dpy = dpy;
	display->ctx = ctx;

	display->root = DefaultRootWindow(dpy);
	display->depth = DefaultDepth(dpy, DefaultScreen(dpy));
	display->visual = DefaultVisual(dpy, DefaultScreen(dpy));

	display->has_shm = can_use_shm(dpy, display->root,
				       &display->shm_event,
				       &display->shm_opcode,
				       &display->has_shm_pixmap);
	DBG(X11, ("%s: has_shm?=%d, event=%d, opcode=%d, has_pixmap?=%d\n",
	     DisplayString(dpy),
	     display->has_shm,
	     display->shm_event,
	     display->shm_opcode,
	     display->has_shm_pixmap));

	display->rr_active = XRRQueryExtension(dpy, &display->rr_event, &display->rr_error);
	DBG(X11, ("%s: randr_active?=%d, event=%d, error=%d\n",
	     DisplayString(dpy),
	     display->rr_active,
	     display->rr_event,
	     display->rr_error));

	display->xinerama_active = xinerama_active(display);
	DBG(X11, ("%s: xinerama_active?=%d, event=%d, error=%d\n",
	     DisplayString(dpy),
	     display->xinerama_active,
	     display->xinerama_event,
	     display->xinerama_error));

	display->dri3_active = dri3_exists(dpy);
	DBG(X11, ("%s: dri3_active?=%d\n",
	     DisplayString(dpy),
	     display->dri3_active));

	/* first display (source) is slightly special */
	if (!first_display) {
		display->invisible_cursor = display_load_invisible_cursor(display);
		display_cursor_move(display, 0, 0, 0);
	}

	return ConnectionNumber(dpy);
}

static int display_open(struct context *ctx, const char *name)
{
	Display *dpy;
	int n;

	DBG(X11, ("%s(%s)\n", __func__, name));

	dpy = XOpenDisplay(name);
	if (dpy == NULL)
		return -ECONNREFUSED;

	/* Prevent cloning the same display twice */
	for (n = 0; n < ctx->ndisplay; n++) {
		if (strcmp(DisplayString(dpy), DisplayString(ctx->display[n].dpy)) == 0) {
			DBG(X11, ("%s %s is already connected\n", __func__, name));
			XCloseDisplay(dpy);
			return -EBUSY;
		}
	}

	return add_display(ctx, dpy);
}

static int bumblebee_open(struct context *ctx)
{
	char buf[256];
	struct sockaddr_un addr;
	int fd, len;

	fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if (fd < 0) {
		DBG(X11, ("%s unable to create a socket: %d\n", __func__, errno));
		return -ECONNREFUSED;
	}

	addr.sun_family = AF_UNIX;
	snprintf(addr.sun_path, sizeof(addr.sun_path), "%s",
		 optarg && *optarg ? optarg : "/var/run/bumblebee.socket");
	if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		DBG(X11, ("%s unable to create a socket: %d\n", __func__, errno));
		goto err;
	}

	/* Ask bumblebee to start the second server */
	buf[0] = 'C';
	if (send(fd, &buf, 1, 0) != 1 || (len = recv(fd, &buf, 255, 0)) <= 0) {
		DBG(X11, ("%s startup send/recv failed: %d\n", __func__, errno));
		goto err;
	}
	buf[len] = '\0';

	/* Query the display name */
	strcpy(buf, "Q VirtualDisplay");
	if (send(fd, buf, 17, 0) != 17 || (len = recv(fd, buf, 255, 0)) <= 0) {
		DBG(X11, ("%s query send/recv failed: %d\n", __func__, errno));
		goto err;
	}
	buf[len] = '\0';

	DBG(X11, ("%s query result '%s'\n", __func__, buf));

	if (strncmp(buf, "Value: ", 7))
		goto err;

	len = 7;
	while (buf[len] != '\n' && buf[len] != '\0')
		len++;
	buf[len] = '\0';

	/* XXX We must keep the control socket open whilst we want to keep
	 * the display around.
	 *
	 * So what we need to do is listen for new bumblee Xservers and
	 * bind only for their duration.
	 */

	return display_open(ctx, buf+7);

err:
	close(fd);
	return -ECONNREFUSED;
}

static int display_init_damage(struct display *display)
{
	DBG(X11, ("%s(%s)\n", __func__, DisplayString(display->dpy)));

	if (!XDamageQueryExtension(display->dpy, &display->damage_event, &display->damage_error) ||
	    !XFixesQueryExtension(display->dpy, &display->xfixes_event, &display->xfixes_error)) {
		fprintf(stderr, "Damage/Fixes extension not supported by %s\n", DisplayString(display->dpy));
		return EINVAL;
	}

	display->damage = XDamageCreate(display->dpy, display->root, XDamageReportBoundingBox);
	if (display->damage == 0)
		return EACCES;

	return 0;
}

static void display_reset_damage(struct display *display)
{
	Damage damage;

	damage = XDamageCreate(display->dpy, display->root, XDamageReportBoundingBox);
	if (damage) {
		XDamageDestroy(display->dpy, display->damage);
		display->damage = damage;
		XFlush(display->dpy);
		display->flush = 0;
	}
}

static void display_init_randr_hpd(struct display *display)
{
	int major, minor;

	DBG(X11,("%s(%s)\n", __func__, DisplayString(display->dpy)));

	if (!XRRQueryVersion(display->dpy, &major, &minor))
		return;

	DBG(X11, ("%s - randr version %d.%d\n", DisplayString(display->dpy), major, minor));
	if (major > 1 || (major == 1 && minor >= 2))
		XRRSelectInput(display->dpy, display->root, RROutputChangeNotifyMask);
}

static void rebuild_clones(struct context *ctx, struct clone *new_clones)
{
	int n, m;

	for (n = 1; n < ctx->ndisplay; n++) {
		struct display *d = &ctx->display[n];

		d->clone = NULL;
		for (m = 0; m < ctx->nclone; m++) {
			struct clone *c = &new_clones[m];

			if (c->dst.display != d)
				continue;

			c->next = d->clone;
			d->clone = c;
		}
	}

	ctx->clones = new_clones;
}

static struct clone *add_clone(struct context *ctx)
{
	if (is_power_of_2(ctx->nclone)) {
		struct clone *new_clones;

		new_clones = realloc(ctx->clones, 2*ctx->nclone*sizeof(struct clone));
		if (new_clones == NULL)
			return NULL;

		if (new_clones != ctx->clones)
			rebuild_clones(ctx, new_clones);
	}

	return memset(&ctx->clones[ctx->nclone++], 0, sizeof(struct clone));
}

static struct display *last_display(struct context *ctx)
{
	return &ctx->display[ctx->ndisplay-1];
}

static void reverse_clone_list(struct display *display)
{
	struct clone *list = NULL;

	while (display->clone) {
		struct clone *clone = display->clone;
		display->clone = clone->next;
		clone->next = list;
		list = clone;
	}

	display->clone = list;
}

static int last_display_add_clones__randr(struct context *ctx)
{
	struct display *display = last_display(ctx);
	XRRScreenResources *res;
	char buf[80];
	int i, ret;

	DBG(X11, ("%s(%s)\n", __func__, DisplayString(display->dpy)));

	display_init_randr_hpd(display);

	/* Force a probe of outputs on initial connection */
	res = XRRGetScreenResources(display->dpy, display->root);
	if (res == NULL)
		return -ENOMEM;

	DBG(X11, ("%s - noutputs=%d\n", DisplayString(display->dpy), res->noutput));
	for (i = 0; i < res->noutput; i++) {
		XRROutputInfo *o = XRRGetOutputInfo(display->dpy, res, res->outputs[i]);
		struct clone *clone = add_clone(ctx);
		RROutput id;

		if (clone == NULL)
			return -ENOMEM;

		clone->depth = 24;
		clone->next = display->clone;
		display->clone = clone;

		id = claim_virtual(ctx->display, buf, ctx->nclone);
		if (id == 0) {
			fprintf(stderr, "Failed to find available VirtualHead \"%s\" for \"%s\" on display \"%s\"\n",
				buf, o->name, DisplayString(display->dpy));
			return -ENOSPC;
		}

		ret = clone_output_init(clone, &clone->src, ctx->display, buf, id);
		if (ret) {
			fprintf(stderr, "Failed to add output \"%s\" on display \"%s\"\n",
				buf, DisplayString(ctx->display->dpy));
			return ret;
		}

		ret = clone_output_init(clone, &clone->dst, display, o->name, res->outputs[i]);
		if (ret) {
			fprintf(stderr, "Failed to add output \"%s\" on display \"%s\"\n",
				o->name, DisplayString(display->dpy));
			return ret;
		}

		ret = clone_init_depth(clone);
		if (ret) {
			fprintf(stderr, "Failed to negotiate image format for display \"%s\"\n",
				DisplayString(display->dpy));
			return ret;
		}

		ret = clone_update_modes__randr(clone);
		if (ret) {
			fprintf(stderr, "Failed to clone output \"%s\" from display \"%s\"\n",
				o->name, DisplayString(display->dpy));
			return ret;
		}


		if (o->crtc) {
			DBG(X11, ("%s - disabling active output\n", DisplayString(display->dpy)));
			disable_crtc(display->dpy, res, o->crtc);
		}

		XRRFreeOutputInfo(o);
	}
	XRRFreeScreenResources(res);

	reverse_clone_list(display);
	return 0;
}

#if defined(USE_XINERAMA)
static int last_display_add_clones__xinerama(struct context *ctx)
{
	struct display *display = last_display(ctx);
	Display *dpy = display->dpy;
	XineramaScreenInfo *xi;
	char buf[80];
	int n, count, ret;

	DBG(X11, ("%s(%s)\n", __func__, DisplayString(display->dpy)));

	count = 0;
	xi = XineramaQueryScreens(dpy, &count);
	for (n = 0; n < count; n++) {
		struct clone *clone = add_clone(ctx);
		RROutput id;

		if (clone == NULL)
			return -ENOMEM;

		if (xi[n].width == 0 || xi[n].height == 0)
			continue;

		clone->depth = 24;
		clone->next = display->clone;
		display->clone = clone;

		id = claim_virtual(ctx->display, buf, ctx->nclone);
		if (id == 0) {
			fprintf(stderr, "Failed to find available VirtualHead \"%s\" for Xinerama screen %d on display \"%s\"\n",
				buf, n, DisplayString(dpy));
		}
		ret = clone_output_init(clone, &clone->src, ctx->display, buf, id);
		if (ret) {
			fprintf(stderr, "Failed to add Xinerama screen %d on display \"%s\"\n",
				n, DisplayString(ctx->display->dpy));
			return ret;
		}

		sprintf(buf, "XINERAMA%d", n);
		ret = clone_output_init(clone, &clone->dst, display, buf, 0);
		if (ret) {
			fprintf(stderr, "Failed to add Xinerama screen %d on display \"%s\"\n",
				n, DisplayString(dpy));
			return ret;
		}

		ret = clone_init_depth(clone);
		if (ret) {
			fprintf(stderr, "Failed to negotiate image format for display \"%s\"\n",
				DisplayString(display->dpy));
			return ret;
		}

		/* Replace the modes on the local VIRTUAL output with the remote Screen */
		clone->width = xi[n].width;
		clone->height = xi[n].height;
		clone->dst.x = xi[n].x_org;
		clone->dst.y = xi[n].y_org;
		clone->dst.rr_crtc = -1;
		ret = clone_update_modes__fixed(clone);
		if (ret) {
			fprintf(stderr, "Failed to clone Xinerama screen %d from display \"%s\"\n",
				n, DisplayString(display->dpy));
			return ret;
		}

		clone->active = ctx->active;
		ctx->active = clone;
	}
	XFree(xi);

	reverse_clone_list(display);
	return 0;
}
#else
#define last_display_add_clones__xinerama(ctx) -1
#endif

static int last_display_add_clones__display(struct context *ctx)
{
	struct display *display = last_display(ctx);
	Display *dpy = display->dpy;
	struct clone *clone;
	Screen *scr;
	char buf[80];
	int ret;
	RROutput id;


	DBG(X11, ("%s(%s)\n", __func__, DisplayString(dpy)));
	clone = add_clone(ctx);
	if (clone == NULL)
		return -ENOMEM;

	clone->depth = 24;
	clone->next = display->clone;
	display->clone = clone;

	id = claim_virtual(ctx->display, buf, ctx->nclone);
	if (id == 0) {
		fprintf(stderr, "Failed to find available VirtualHead \"%s\" for on display \"%s\"\n",
			buf, DisplayString(dpy));
	}
	ret = clone_output_init(clone, &clone->src, ctx->display, buf, id);
	if (ret) {
		fprintf(stderr, "Failed to add display \"%s\"\n",
			DisplayString(ctx->display->dpy));
		return ret;
	}

	sprintf(buf, "WHOLE");
	ret = clone_output_init(clone, &clone->dst, display, buf, 0);
	if (ret) {
		fprintf(stderr, "Failed to add display \"%s\"\n",
			DisplayString(dpy));
		return ret;
	}

	ret = clone_init_depth(clone);
	if (ret) {
		fprintf(stderr, "Failed to negotiate image format for display \"%s\"\n",
			DisplayString(dpy));
		return ret;
	}

	/* Replace the modes on the local VIRTUAL output with the remote Screen */
	scr = ScreenOfDisplay(dpy, DefaultScreen(dpy));
	clone->width = scr->width;
	clone->height = scr->height;
	clone->dst.x = 0;
	clone->dst.y = 0;
	clone->dst.rr_crtc = -1;
	ret = clone_update_modes__fixed(clone);
	if (ret) {
		fprintf(stderr, "Failed to clone display \"%s\"\n",
			DisplayString(dpy));
		return ret;
	}

	clone->active = ctx->active;
	ctx->active = clone;

	return 0;
}

static int last_display_add_clones(struct context *ctx)
{
	struct display *display = last_display(ctx);

	display->width = DisplayWidth(display->dpy, DefaultScreen(display->dpy));
	display->height = DisplayHeight(display->dpy, DefaultScreen(display->dpy));
	DBG(X11, ("%s - initial size %dx%d\n", DisplayString(display->dpy), display->width, display->height));

	if (display->rr_active)
		return last_display_add_clones__randr(ctx);

	if (display->xinerama_active)
		return last_display_add_clones__xinerama(ctx);

	return last_display_add_clones__display(ctx);
}

static int last_display_clone(struct context *ctx, int fd)
{
	fd = add_fd(ctx, fd);
	if (fd < 0)
		return fd;

	fd = last_display_add_clones(ctx);
	if (fd)
		return fd;

	return 0;
}

static int first_display_has_singleton(struct context *ctx)
{
	struct display *display = ctx->display;
	unsigned long nitems, bytes;
	unsigned char *prop;
	int format;
	Atom type;

	ctx->singleton = XInternAtom(display->dpy, "intel-virtual-output-singleton", False);

	XGetWindowProperty(display->dpy, display->root, ctx->singleton,
			   0, 0, 0, AnyPropertyType, &type, &format, &nitems, &bytes, &prop);
	DBG(X11, ("%s: singleton registered? %d\n", DisplayString(display->dpy), type != None));
	return type != None;
}

static int first_display_wait_for_ack(struct context *ctx, int timeout, int id)
{
	struct display *display = ctx->display;
	struct pollfd pfd;
	char expect[6]; /* "1234R\0" */

	sprintf(expect, "%04xR", id);
	DBG(X11, ("%s: wait for act '%c%c%c%c%c'\n",
	     DisplayString(display->dpy),
	     expect[0], expect[1], expect[2], expect[3], expect[4]));

	XFlush(display->dpy);

	pfd.fd = ConnectionNumber(display->dpy);
	pfd.events = POLLIN;
	do {
		if (poll(&pfd, 1, timeout) <= 0)
			return -ETIME;

		while (XPending(display->dpy)) {
			XEvent e;
			XClientMessageEvent *cme;

			XNextEvent(display->dpy, &e);
			DBG(X11, ("%s: reading event type %d\n", DisplayString(display->dpy), e.type));

			if (e.type != ClientMessage)
				continue;

			cme = (XClientMessageEvent *)&e;
			if (cme->message_type != ctx->singleton)
				continue;
			if (cme->format != 8)
				continue;

			DBG(X11, ("%s: client message '%c%c%c%c%c'\n",
			     DisplayString(display->dpy),
			     cme->data.b[0],
			     cme->data.b[1],
			     cme->data.b[2],
			     cme->data.b[3],
			     cme->data.b[4]));
			if (memcmp(cme->data.b, expect, 5))
				continue;

			return -atoi(cme->data.b + 5);
		}
	} while (1);
}

#if defined(__GNUC__) && (__GNUC__ > 3)
__attribute__((format(gnu_printf, 3, 4)))
#endif
static int first_display_send_command(struct context *ctx, int timeout,
				      const char *format,
				      ...)
{
	struct display *display = ctx->display;
	char buf[1024], *b;
	int len, id;
	va_list va;

	id = rand() & 0xffff;
	sprintf(buf, "%04x", id);
	va_start(va, format);
	len = vsnprintf(buf+4, sizeof(buf)-4, format, va)+5;
	va_end(va);
	assert(len < sizeof(buf));

	DBG(X11, ("%s: send command '%s'\n", DisplayString(display->dpy), buf));

	b = buf;
	while (len) {
		XClientMessageEvent msg;
		int n = len;
		if (n > sizeof(msg.data.b))
			n = sizeof(msg.data.b);
		len -= n;

		msg.type = ClientMessage;
		msg.serial = 0;
		msg.message_type = ctx->singleton;
		msg.format = 8;
		memcpy(msg.data.b, b, n);
		b += n;

		XSendEvent(display->dpy, display->root, False, PropertyChangeMask, (XEvent *)&msg);
	}

	return first_display_wait_for_ack(ctx, timeout, id);
}

static void first_display_reply(struct context *ctx, int result)
{
	struct display *display = ctx->display;
	XClientMessageEvent msg;

	sprintf(msg.data.b, "%c%c%c%cR%d",
	     ctx->command[0],
	     ctx->command[1],
	     ctx->command[2],
	     ctx->command[3],
	     -result);

	DBG(X11, ("%s: send reply '%s'\n", DisplayString(display->dpy), msg.data.b));

	msg.type = ClientMessage;
	msg.serial = 0;
	msg.message_type = ctx->singleton;
	msg.format = 8;

	XSendEvent(display->dpy, display->root, False, PropertyChangeMask, (XEvent *)&msg);
	XFlush(display->dpy);
}

static void first_display_handle_command(struct context *ctx,
					 const char *msg)
{
	int len;

	DBG(X11, ("client message!\n"));

	for (len = 0; len < 20 && msg[len]; len++)
		;

	if (ctx->command_continuation + len > sizeof(ctx->command)) {
		ctx->command_continuation = 0;
		return;
	}

	memcpy(ctx->command + ctx->command_continuation, msg, len);
	ctx->command_continuation += len;

	if (len < 20) {
		ctx->command[ctx->command_continuation] = 0;
		DBG(X11, ("client command complete! '%s'\n", ctx->command));
		switch (ctx->command[4]) {
		case 'B':
			first_display_reply(ctx, last_display_clone(ctx, bumblebee_open(ctx)));
			break;
		case 'C':
			first_display_reply(ctx, last_display_clone(ctx, display_open(ctx, ctx->command + 5)));
			break;
		case 'P':
			first_display_reply(ctx, 0);
			break;
		case 'R':
			break;
		}
		ctx->command_continuation = 0;
		return;
	}
}

static int first_display_register_as_singleton(struct context *ctx)
{
	struct display *display = ctx->display;
	struct pollfd pfd;

	XChangeProperty(display->dpy, display->root, ctx->singleton,
			XA_STRING, 8, PropModeReplace, (unsigned char *)".", 1);
	XFlush(display->dpy);

	/* And eat the notify (presuming that it is ours!) */

	pfd.fd = ConnectionNumber(display->dpy);
	pfd.events = POLLIN;
	do {
		if (poll(&pfd, 1, 1000) <= 0) {
			fprintf(stderr, "Failed to register as singleton\n");
			return EBUSY;
		}

		while (XPending(display->dpy)) {
			XEvent e;

			XNextEvent(display->dpy, &e);
			DBG(X11, ("%s: reading event type %d\n", DisplayString(display->dpy), e.type));

			if (e.type == PropertyNotify &&
			    ((XPropertyEvent *)&e)->atom == ctx->singleton)
				return 0;
		}
	} while (1);
}

static void display_flush_send(struct display *display)
{
	XShmCompletionEvent e;

	if (!display->send)
		return;

	DBG(X11, ("%s flushing send (serial now %ld) (has shm send? %d)\n",
	     DisplayString(display->dpy),
	     (long)NextRequest(display->dpy),
	     display->shm_event));

	display->send = 0;

	if (display->shm_event == 0) {
		XSync(display->dpy, False);
		display->flush = 0;
		return;
	}

	memset(&e, 0, sizeof(e));
	e.type = display->shm_event;
	e.send_event = 1;
	e.drawable = display->root;
	e.major_code = display->shm_opcode;
	e.minor_code = X_ShmPutImage;

	XSendEvent(display->dpy, display->root, False, 0, (XEvent *)&e);
	display_mark_flush(display);
}

static void display_sync(struct display *display)
{
	if (display->skip_clone == 0)
		return;

	if (display->skip_frame++ < 2)
		return;

	DBG(X11, ("%s forcing sync\n", DisplayString(display->dpy)));
	XSync(display->dpy, False);

	display->flush = 0;
	display->send = 0;

	/* Event tracking proven unreliable, disable */
	display->shm_event = 0;
}

static void display_flush(struct display *display)
{
	display_flush_cursor(display);
	display_flush_send(display);

	display_sync(display);

	if (!display->flush)
		return;

	DBG(X11, ("%s(%s)\n", __func__, DisplayString(display->dpy)));

	XFlush(display->dpy);
	display->flush = 0;
}

static int first_display_first_sibling(struct context *ctx)
{
	const char *str, *colon;
	int dpy, scr, len;

	str = DisplayString(ctx->display->dpy);
	colon = strrchr(str, ':');
	if (colon == NULL)
		return -1;

	if (sscanf(colon + 1, "%d.%d", &dpy, &scr) == 1)
		scr = 0;

	len = (colon - str) + 1;
	memcpy(ctx->command, str, len);
	len += sprintf(ctx->command + len, "%d.", dpy);
	ctx->command_continuation = len;

	return scr + 1;
}

static int first_display_sibling(struct context *ctx, int i)
{
	if (i < 0)
		return 0;

	sprintf(ctx->command + ctx->command_continuation, "%d", i);
	return 1;
}

#define first_display_for_each_sibling(CTX, i) \
	for (i = first_display_first_sibling(CTX); first_display_sibling(CTX, i); i++)

static void display_cleanup(struct display *display)
{
	Display *dpy = display->dpy;
	XRRScreenResources *res;
	int n;

	XGrabServer(dpy);

	res = _XRRGetScreenResourcesCurrent(dpy, display->root);
	if (res != NULL) {
		for (n = 0; n < res->ncrtc; n++)
			disable_crtc(display->dpy, res, res->crtcs[n]);

		XRRFreeScreenResources(res);
	}

	XUngrabServer(dpy);
}

static void context_cleanup(struct context *ctx)
{
	Display *dpy = ctx->display->dpy;
	XRRScreenResources *res;
	int i, j;

	for (i = 1; i < ctx->ndisplay; i++)
		display_cleanup(&ctx->display[i]);

	res = _XRRGetScreenResourcesCurrent(dpy, ctx->display->root);
	if (res == NULL)
		return;

	XGrabServer(dpy);

	for (i = 0; i < ctx->nclone; i++) {
		struct clone *clone = &ctx->clones[i];
		XRROutputInfo *output;

		assert(clone->src.display == ctx->display);

		output = XRRGetOutputInfo(dpy, res, clone->src.rr_output);
		if (output == NULL)
			continue;

		disable_crtc(dpy, res, output->crtc);
		for (j = 0; j < output->nmode; j++)
			XRRDeleteOutputMode(dpy, clone->src.rr_output, output->modes[j]);

		XRRFreeOutputInfo(output);
	}

	for (i = 0; i < res->nmode; i++) {
		if (strncmp(res->modes[i].name, "VIRTUAL", 7) == 0) {
			XRRDestroyMode(dpy, res->modes[i].id);
			continue;
		}

		if (strcmp(res->modes[i].name, "ClaimVirtualHead") == 0) {
			XRRDestroyMode(dpy, res->modes[i].id);
			continue;
		}
	}
	XRRFreeScreenResources(res);

	/* And hide them again */
	res = XRRGetScreenResources(dpy, ctx->display->root);
	if (res != NULL)
		XRRFreeScreenResources(res);

	XUngrabServer(dpy);

	if (ctx->singleton)
		XDeleteProperty(dpy, ctx->display->root, ctx->singleton);
	XCloseDisplay(dpy);
}

static int done;

static void signal_handler(int sig)
{
	done = sig;
}

int main(int argc, char **argv)
{
	struct context ctx;
	const char *src_name = NULL;
	uint64_t count;
	int daemonize = 1, bumblebee = 0, siblings = 0, singleton = 1;
	int i, ret, open, fail;

	signal(SIGPIPE, SIG_IGN);

	while ((i = getopt(argc, argv, "abd:fhSvV:")) != -1) {
		switch (i) {
		case 'd':
			src_name = optarg;
			break;
		case 'f':
			daemonize = 0;
			break;
		case 'b':
			bumblebee = 1;
			break;
		case 's':
			siblings = 1;
			break;
		case 'S':
			singleton = 0;
			break;
		case 'v':
			verbose = ~0;
			daemonize = 0;
			break;
		case 'V':
			verbose = strtol(optarg, NULL, 0);
			daemonize = 0;
			break;
		case 'h':
		default:
			usage(argv[0]);
			exit(0);
		}
	}

	ret = context_init(&ctx);
	if (ret)
		return -ret;

	XSetErrorHandler(_check_error_handler);

	ret = add_fd(&ctx, display_open(&ctx, src_name));
	if (ret) {
		fprintf(stderr, "Unable to connect to \"%s\".\n", src_name ?: getenv("DISPLAY") ?:
			"<unspecified>, set either the DISPLAY environment variable or pass -d <display name> on the commandline");
		ret = -ret;
		goto out;
	}

	if (singleton) {
		XSelectInput(ctx.display->dpy, ctx.display->root, PropertyChangeMask);
		if (first_display_has_singleton(&ctx)) {
			DBG(X11, ("%s: pinging singleton\n", DisplayString(ctx.display->dpy)));
			ret = first_display_send_command(&ctx, 2000, "P");
			if (ret) {
				if (ret != -ETIME) {
					ret = -ret;
					goto out;
				}
				DBG(X11, ("No reply from singleton; assuming control\n"));
			} else {
				DBG(X11, ("%s: singleton active, sending open commands\n", DisplayString(ctx.display->dpy)));

				open = fail = 0;
				for (i = optind; i < argc; i++) {
					ret = first_display_send_command(&ctx, 5000, "C%s", argv[i]);
					if (ret && ret != -EBUSY) {
						fprintf(stderr, "Unable to connect to \"%s\".\n", argv[i]);
						fail++;
					} else
						open++;
				}
				if (siblings || (optind == argc && !bumblebee)) {
					first_display_for_each_sibling(&ctx, i) {
						ret = first_display_send_command(&ctx, 5000, "C%s", ctx.command);
						if (ret && ret != -EBUSY)
							break;
						else
							open++;
					}
				}
				if (bumblebee || (optind == argc && !siblings)) {
					ret = first_display_send_command(&ctx, 5000, "B");
					if (ret && ret != -EBUSY) {
						if (bumblebee)
							fprintf(stderr, "Unable to connect to bumblebee.\n");
						fail++;
					} else
						open++;
				}
				ret = open || !fail ? 0 : ECONNREFUSED;
				goto out;
			}
		}
		ret = first_display_register_as_singleton(&ctx);
		if (ret)
			goto out;
	}

	ret = display_init_damage(ctx.display);
	if (ret)
		goto out;

	if ((ctx.display->rr_event | ctx.display->rr_error) == 0) {
		fprintf(stderr, "RandR extension not supported by %s\n", DisplayString(ctx.display->dpy));
		ret = EINVAL;
		goto out;
	}
	XRRSelectInput(ctx.display->dpy, ctx.display->root, RRScreenChangeNotifyMask);
	XFixesSelectCursorInput(ctx.display->dpy, ctx.display->root, XFixesDisplayCursorNotifyMask);

	ret = add_fd(&ctx, record_mouse(&ctx));
	if (ret) {
		fprintf(stderr, "XTEST extension not supported by display \"%s\"\n", DisplayString(ctx.display->dpy));
		ret = -ret;
		goto out;
	}

	open = fail = 0;
	for (i = optind; i < argc; i++) {
		ret = last_display_clone(&ctx, display_open(&ctx, argv[i]));
		if (ret && ret != -EBUSY) {
			fprintf(stderr, "Unable to connect to \"%s\".\n", argv[i]);
			fail++;
		} else
			open++;
	}
	if (siblings || (optind == argc && !bumblebee)) {
		first_display_for_each_sibling(&ctx, i) {
			ret = last_display_clone(&ctx, display_open(&ctx, ctx.command));
			if (ret && ret != -EBUSY)
				break;
			else
				open++;
		}
	}
	if (bumblebee || (optind == argc && !siblings)) {
		ret = last_display_clone(&ctx, bumblebee_open(&ctx));
		if (ret && ret != -EBUSY) {
			if (bumblebee)
				fprintf(stderr, "Unable to connect to bumblebee.\n");
			fail++;
		} else
			open++;
	}
	if (open == 0) {
		ret = fail ? ECONNREFUSED : 0;
		goto out;
	}

	if (daemonize && daemon(0, 0)) {
		ret = EINVAL;
		goto out;
	}

	signal(SIGHUP, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	ctx.command_continuation = 0;
	while (!done) {
		XEvent e;
		int reconfigure = 0;
		int rr_update = 0;

		DBG(POLL, ("polling - enable timer? %d, nfd=%d, ndisplay=%d\n", ctx.timer_active, ctx.nfd, ctx.ndisplay));
		ret = poll(ctx.pfd + !ctx.timer_active, ctx.nfd - !ctx.timer_active, -1);
		if (ret <= 0)
			break;

		/* pfd[0] is the timer, pfd[1] is the local display, pfd[2] is the mouse, pfd[3+] are the remotes */

		DBG(POLL, ("poll reports %d fd awake\n", ret));
		if (ctx.pfd[1].revents || XPending(ctx.display[0].dpy)) {
			DBG(POLL,("%s woken up\n", DisplayString(ctx.display[0].dpy)));
			do {
				XNextEvent(ctx.display->dpy, &e);

				if (e.type == ctx.display->damage_event + XDamageNotify ) {
					const XDamageNotifyEvent *de = (const XDamageNotifyEvent *)&e;
					struct clone *clone;

					DBG(DAMAGE, ("%s damaged: (%d, %d)x(%d, %d)\n",
					     DisplayString(ctx.display->dpy),
					     de->area.x, de->area.y, de->area.width, de->area.height));

					for (clone = ctx.active; clone; clone = clone->active)
						clone_damage(clone, &de->area);

					if (ctx.active)
						context_enable_timer(&ctx);
				} else if (e.type == ctx.display->xfixes_event + XFixesCursorNotify) {
					XFixesCursorImage *cur;

					DBG(CURSOR, ("%s cursor changed\n",
					     DisplayString(ctx.display->dpy)));

					cur = XFixesGetCursorImage(ctx.display->dpy);
					if (cur == NULL)
						continue;

					for (i = 1; i < ctx.ndisplay; i++)
						display_load_visible_cursor(&ctx.display[i], cur);

					XFree(cur);
				} else if (e.type == ctx.display->rr_event + RRScreenChangeNotify) {
					DBG(XRR, ("%s screen changed (reconfigure pending? %d)\n",
					     DisplayString(ctx.display->dpy), reconfigure));
					reconfigure = 1;
				} else if (e.type == PropertyNotify) {
					XPropertyEvent *pe = (XPropertyEvent *)&e;
					if (pe->atom == ctx.singleton) {
						DBG(X11, ("lost control of singleton\n"));
						return 0;
					}
				} else if (e.type == ClientMessage) {
					XClientMessageEvent *cme;

					DBG(X11, ("%s client message\n",
					     DisplayString(ctx.display->dpy)));

					cme = (XClientMessageEvent *)&e;
					if (cme->message_type != ctx.singleton)
						continue;
					if (cme->format != 8)
						continue;

					first_display_handle_command(&ctx, cme->data.b);
				} else {
					DBG(X11, ("unknown event %d\n", e.type));
				}
			} while (XEventsQueued(ctx.display->dpy, QueuedAfterReading));
		}

		for (i = 1; i < ctx.ndisplay; i++) {
			if (ctx.pfd[i+2].revents == 0 && !XPending(ctx.display[i].dpy))
				continue;

			DBG(POLL, ("%s woken up\n", DisplayString(ctx.display[i].dpy)));
			do {
				XNextEvent(ctx.display[i].dpy, &e);

				DBG(POLL, ("%s received event %d\n", DisplayString(ctx.display[i].dpy), e.type));
				if (ctx.display[i].rr_active && e.type == ctx.display[i].rr_event + RRNotify) {
					XRRNotifyEvent *re = (XRRNotifyEvent *)&e;

					DBG(XRR, ("%s received RRNotify, type %d\n", DisplayString(ctx.display[i].dpy), re->subtype));
					if (re->subtype == RRNotify_OutputChange) {
						XRROutputPropertyNotifyEvent *ro = (XRROutputPropertyNotifyEvent *)re;
						struct clone *clone;

						DBG(XRR, ("%s RRNotify_OutputChange, timestamp %ld\n", DisplayString(ctx.display[i].dpy), ro->timestamp));
						for (clone = ctx.display[i].clone; clone; clone = clone->next) {
							if (clone->dst.rr_output == ro->output)
								rr_update = clone->rr_update = 1;
						}
					}
				}
			} while (XEventsQueued(ctx.display[i].dpy, QueuedAfterReading));
		}

		if (rr_update) {
			for (i = 0; i < ctx.nclone; i++)
				clone_update(&ctx.clones[i]);
		}

		if (reconfigure && context_update(&ctx))
			display_reset_damage(ctx.display);

		while (XPending(ctx.record)) /* discard all implicit events */
			XNextEvent(ctx.record, &e);

		if (ctx.timer_active && read(ctx.timer, &count, sizeof(count)) > 0) {
			struct clone *clone;

			DBG(TIMER, ("%s timer expired (count=%ld)\n", DisplayString(ctx.display->dpy), (long)count));
			ret = 0;

			if (ctx.active) {
				DBG(DAMAGE, ("%s clearing damage\n", DisplayString(ctx.display->dpy)));
				XDamageSubtract(ctx.display->dpy, ctx.display->damage, None, None);
				ctx.display->flush = 1;
			}

			for (clone = ctx.active; clone; clone = clone->active)
				ret |= clone_paint(clone);

			for (i = 0; i < ctx.ndisplay; i++)
				display_flush(&ctx.display[i]);

			DBG(TIMER, ("%s timer still active? %d\n", DisplayString(ctx.display->dpy), ret != 0));
			ctx.timer_active = ret != 0;
		}
	}

	ret = 0;
out:
	context_cleanup(&ctx);
	return ret;
}
