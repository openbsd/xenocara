/*	$OpenBSD: video.c,v 1.41 2021/10/25 09:30:33 matthieu Exp $	*/
/*
 * Copyright (c) 2010 Jacob Meuser <jakemsr@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/videoio.h>
#include <sys/time.h>
#include <sys/limits.h>
#include <sys/mman.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xvlib.h>
#include <X11/Xatom.h>

/*
 * XvListImageFormats(3) reports YUY2 for the format defined 
 * as YUYV in videoio.h. 
 */
#define V_V4L2_PIX_FMT_YUY2    v4l2_fourcc('Y', 'U', 'Y', '2')

/* Xv(3) adaptor properties */
struct xv_adap {
	char		 name[128];
	int		 nattr;
	unsigned int	 adap_index;
	XvPortID	 base_port;
	int		 nports;
#define MAX_FMTS	8
	int		 fmts[MAX_FMTS];
	int		 nfmts;
	int		 cur_fmt;
	int		 max_width;
	int		 max_height;
};

/* X(7) display properties */
struct xdsp {
	Display		*dpy;
	Window		 window;
	Window		 rwin;
	XEvent		 event;
	GC		 gc;
	Atom		 wmdelwin;
	XvPortID	 port;
	XvImage		*xv_image;
#define MAX_ADAPS	8
	struct xv_adap	 adaps[MAX_ADAPS];
	int		 nadaps;
	int		 cur_adap;
	int		 width;
	int		 height;
	int		 saved_x;
	int		 saved_y;
	int		 saved_w;
	int		 saved_h;
	int		 resized;
	int		 box_x;
	int		 box_y;
	int		 box_w;
	int		 box_h;
	int		 max_width;
	int		 max_height;
	int		 screen_id;
};

/* video(4) controls */
struct dev_ctrls {
	char		*name;
	int		 supported;
	int		 id;
	int		 id_auto;
	int		 def;
	int		 min;
	int		 max;
	int		 step;
	int		 cur;
} ctrls[] = {
#define CTRL_BRIGHTNESS	0
	{ "brightness",	0, V4L2_CID_BRIGHTNESS,	0, 0, 0, 0, 0, 0 },
#define CTRL_CONTRAST	1
	{ "contrast",	0, V4L2_CID_CONTRAST,	0, 0, 0, 0, 0, 0 },
#define CTRL_SATURATION	2
	{ "saturation",	0, V4L2_CID_SATURATION,	0, 0, 0, 0, 0, 0 },
#define CTRL_HUE	3
	{ "hue",	0, V4L2_CID_HUE,	0, 0, 0, 0, 0, 0 },
#define CTRL_GAIN	4
	{ "gain",	0, V4L2_CID_GAIN, 	0, 0, 0, 0, 0, 0 },
#define CTRL_GAMMA	5
	{ "gamma",	0, V4L2_CID_GAMMA, 	0, 0, 0, 0, 0, 0 },
#define CTRL_SHARPNESS	6
	{ "sharpness",	0, V4L2_CID_SHARPNESS, 	0, 0, 0, 0, 0, 0 },
#define CTRL_WHITE_BALANCE_TEMPERATURE 7
	{ "white_balance_temperature",
			0, V4L2_CID_WHITE_BALANCE_TEMPERATURE,
			   V4L2_CID_AUTO_WHITE_BALANCE, 0, 0, 0, 0, 0 },
	{ "backlight_compensation",
			0, V4L2_CID_BACKLIGHT_COMPENSATION,
			   0, 0, 0, 0, 0, 0 },
#define CTRL_LAST       9
	{ NULL, 0, 0, 0, 0, 0, 0, 0, 0 }
};

/* frame dimensions */
struct dim {
	int w;
	int h;
};

/* video(4) device properties */
struct dev {
	char		 path[FILENAME_MAX];
	int		 fd;
#define MAX_DSZS 32
	struct dim	 sizes[MAX_DSZS];
#define MAX_RATES 32
	int		 rates[MAX_DSZS][MAX_RATES];
	int		 nsizes;
	int		 buf_type;
};

/* video encodingss */
struct encodings {
	char	*name;
	int	 id;
	int	 bpp;
#define	SUP_DEV	0x1
#define	SUP_XV	0x2
	int	 flags;
} encs[] = {
#define ENC_YUYV	1
	{ "yuy2", V4L2_PIX_FMT_YUYV, 16, 0 },
#define ENC_YUY2	0
	{ "yuy2", V_V4L2_PIX_FMT_YUY2, 16, 0 },
#define ENC_UYVY	2
	{ "uyvy", V4L2_PIX_FMT_UYVY,  16, 0 },
#define ENC_YV12	3
	{ "yv12", V4L2_PIX_FMT_YVU420, 12, 0 },
#define ENC_LAST	4
	{ NULL,	0, 0, 0 }
};

struct video {
	struct xdsp 	 xdsp;
	struct dev	 dev;
#define MMAP_NUM_BUFS	4
	uint8_t		 mmap_on;
	void		*mmap_buffer[MMAP_NUM_BUFS];
	uint8_t		*frame_buffer;
	size_t		 frame_bufsz;
	uint8_t		*conv_buffer;
	size_t		 conv_bufsz;
	char		 iofile[FILENAME_MAX];
	int		 iofile_fd;
	char		*sz_str;
#define	CONV_NONE	0x1
#define	CONV_SWAP	0x2
#define	CONV_YV12	0x4
	int		 conv_type;
	int		 enc;
	int		 enc_xv;
	int		 full_screen;
	int		 net_wm;
	int		 width;
	int		 height;
	double		 aspect;
	int		 bpf;
	int		 fps;
	int		 nofps;
#define M_IN_DEV	0x1
#define M_OUT_XV	0x2
#define M_IN_FILE	0x4
#define M_OUT_FILE	0x8
#define M_QUERY		0x10
#define M_QUERY_CTRLS	0x20
#define M_RESET		0x40
	int		 mode;
	int		 verbose;
};

int xv_get_info(struct video *);
int xv_sel_adap(struct video *);
void xv_dump_info(struct video *);
int xv_init(struct video *);
void net_wm_supported(struct video *);
void resize_window(struct video *, int);
void display_event(struct video *);

int dev_check_caps(struct video *);
int dev_get_encs(struct video *);
int dev_get_sizes(struct video *);
int dev_get_rates(struct video *);
int dev_get_ctrls(struct video *);
void dev_dump_info(struct video *);
void dev_dump_query(struct video *);
void dev_dump_query_ctrls(struct video *);
int dev_init(struct video *);
int dev_set_ctrl_abs(struct video *vid, int, int);
void dev_set_ctrl_rel(struct video *, int, int);
int dev_get_ctrl_auto(struct video *, int);
void dev_set_ctrl_auto(struct video *, int, int, int);
void dev_reset_ctrls(struct video *);

int parse_ctrl(struct video *, int, char **);
int parse_size(struct video *);
int choose_size(struct video *);
int choose_enc(struct video *);
int mmap_init(struct video *);
int mmap_stop(struct video *);
int setup(struct video *);
void cleanup(struct video *, int);
int ioctl_input(struct video *);
int poll_input(struct video *);
int grab_frame(struct video *);
int stream(struct video *);

void got_frame(int);
void got_shutdown(int);
int find_enc(char *);
int find_enc_by_id(int);
void usage(void);

static volatile sig_atomic_t play, shutdown, hold, wout;
extern char *__progname;

void
usage(void)
{
	fprintf(stderr, "usage: %s [-cdgqRv] "
	    "[-a adaptor] [-e encoding] [-f file] [-i input] [-O output]\n"
	    "       %*s [-o output] [-r rate] [-s size] [control[=value]]\n",
	    __progname, (int)strlen(__progname), "");
}

int
find_enc(char *name)
{
	int i;

	for (i = 0; i < ENC_LAST; i++)
		if (!strcmp(encs[i].name, name))
			break;
	return i;
}

int
find_enc_by_id(int id)
{
	int i;

	for (i = 0; i < ENC_LAST; i++)
		if (encs[i].id == id)
			break;
	return i;
}

int
xv_get_info(struct video *vid)
{
	struct xdsp *x = &vid->xdsp;
	XvImageFormatValues *xvformats;
	XvAdaptorInfo *ainfo;
	XvEncodingInfo *xv_encs;
	struct xv_adap *adap;
	unsigned int nenc, p;
	int num_xvformats, nadaps, i, j, ret, enc;

	if ((x->dpy = XOpenDisplay(NULL)) == NULL) {
		warnx("cannot open display %s", XDisplayName(NULL));
		return 0;
	}
	x->rwin = DefaultRootWindow(x->dpy);
	x->screen_id = DefaultScreen(x->dpy);
	x->max_width = XDisplayWidth(x->dpy, x->screen_id);
	x->max_height = XDisplayHeight(x->dpy, x->screen_id);

	ret = XvQueryExtension(x->dpy, &p, &p, &p, &p, &p);
	if (ret != Success) {
		warnx("Xv not available");
		return 0;
	}

	ret = XvQueryAdaptors(x->dpy, x->rwin, &nadaps, &ainfo);
	if (ret != Success) {
		warnx("no Xv adaptors present");
		return 0;
	}
	x->nadaps = 0;
	for (i = 0; i < nadaps; i++) {
		if (!(ainfo[i].type & XvInputMask) ||
		    !(ainfo[i].type & XvImageMask)) {
			continue;
		}
		ret = XvQueryEncodings(x->dpy, ainfo[i].base_id,
		    &nenc, &xv_encs);
		if (ret != Success) {
			if (vid->verbose > 2)
				warnx("adaptor %d XvQueryEncodings failed", i);
			continue;
		}
		adap = &x->adaps[x->nadaps];
		adap->adap_index = i;
		adap->base_port = ainfo[i].base_id;
		adap->nports = ainfo[i].num_ports;
		if (adap->nports == 0) {
			if (vid->verbose > 2)
				warnx("adaptor %d xv ports == 0", i);
			continue;
		}
		strlcpy(adap->name, ainfo[i].name, sizeof(adap->name));
		for (j = 0; j < nenc; j++) {
			if (!strcmp(xv_encs[j].name, "XV_IMAGE")) {
				if (xv_encs[j].width > adap->max_width &&
				    xv_encs[j].height > adap->max_height) {
					adap->max_width = xv_encs[j].width;
					adap->max_height = xv_encs[j].height;
				}
			}
		}
		if (xv_encs != NULL)
			XvFreeEncodingInfo(xv_encs);

		xvformats = XvListImageFormats(x->dpy, ainfo[i].base_id,
		    &num_xvformats);
		adap->nfmts = 0;
		for (j = 0; j < num_xvformats; j++) {
			enc = find_enc_by_id(xvformats[j].id);
			if (enc < ENC_LAST) {
				encs[enc].flags |= SUP_XV;
				adap->fmts[adap->nfmts++] = xvformats[j].id;
			}
			if (adap->nfmts >= MAX_FMTS)
				break;
		}
		if (xvformats != NULL)
			XFree(xvformats);
		if (adap->nfmts == 0) {
			if (vid->verbose > 2) {
				warnx("adaptor %d has no usable encodings",
				    adap->adap_index);
			}
			continue;
		}
		if (++x->nadaps >= MAX_ADAPS)
			break;
	}
	XvFreeAdaptorInfo(ainfo);

	return 1;
}

int
xv_sel_adap(struct video *vid)
{
	struct xdsp *x = &vid->xdsp;
	struct xv_adap *adap;
	XvAttribute *attr;
	Atom atom;
	int i, j, ret, nattr;

	/* At this point x->cur_adap is the index of the adaptor according
	 * to it's listing by the X server.  But after this point, x->cur_adap
	 * will be the index in x->adaps[].
	 */
	if (x->cur_adap != -1) {
		for (i = 0; i < x->nadaps; i++) {
			if (x->adaps[i].adap_index == x->cur_adap)
				break;
		}
		if (i >= x->nadaps) {
			warnx("Xv adaptor '%d' does not exist", x->cur_adap);
			return 0;
		}
		x->cur_adap = i;
		adap = &x->adaps[i];
		for (i = 0; i < adap->nfmts; i++) {
			if (adap->fmts[i] == encs[vid->enc_xv].id)
				break;
		}
		if (i >= adap->nfmts) {
			warnx("Xv adaptor '%d' doesn't support %s",
			    x->adaps[x->cur_adap].adap_index,
			    encs[vid->enc_xv].name);
			return 0;
		}
	}
	for (i = 0; i < x->nadaps && x->cur_adap == -1; i++) {
		adap = &x->adaps[i];
		for (j = 0; j < adap->nfmts; j++) {
			if (adap->fmts[j] == encs[vid->enc_xv].id) {
				x->cur_adap = i;
				break;
			}
		}
	}
	if (x->cur_adap == -1) {
		warnx("no usable Xv adaptor found");
		return 0;
	}

	adap = &x->adaps[x->cur_adap];
	x->port = adap->base_port;
	/* adap->nports will always be > 0, see xv_get_info() */
	while (x->port < adap->base_port + adap->nports &&
	    (ret = XvGrabPort(x->dpy, x->port, CurrentTime)) != Success)
		x->port++;
	if (ret != Success) {
		warnx("adaptor %d could not find usable Xv port",
		    x->adaps[x->cur_adap].adap_index);
		return 0;
	}

	atom = None;
	attr = XvQueryPortAttributes(x->dpy, x->port, &nattr);
	for (i = 0; attr && i < nattr; i++) {
		if (!strcmp(attr[i].name, "XV_AUTOPAINT_COLORKEY")) {
			atom = XInternAtom(x->dpy, "XV_AUTOPAINT_COLORKEY",
			    False);
			break;
		} else if (!strcmp(attr[i].name, "XV_AUTOPAINT_COLOURKEY")) {
			atom = XInternAtom(x->dpy, "XV_AUTOPAINT_COLOURKEY",
			    False);
			break;
		}
	}
	if (attr)
		XFree(attr);
	if (atom != None) {
		ret = XvSetPortAttribute(x->dpy, x->port, atom, 1);
		if (ret != Success && vid->verbose > 2)
			warnx("could not enable autopaint_colorkey");
	}

	if (x->max_width > adap->max_width)
		x->max_width = adap->max_width;
	if (x->max_height > adap->max_height)
		x->max_height = adap->max_height;

	return 1;
}

void
xv_dump_info(struct video *vid)
{
	struct xdsp *x = &vid->xdsp;
	int i, j;

	fprintf(stderr, "Xv adaptor %d, %s:\n",
	    x->adaps[x->cur_adap].adap_index, x->adaps[x->cur_adap].name);

	fprintf(stderr, "  encodings: ");
 	for (i = 0, j = 0; i < ENC_LAST; i++) {
		if (encs[i].id != -1 && (encs[i].flags & SUP_XV)) {
			if (j)
				fprintf(stderr, ", ");
			fprintf(stderr, "%s", encs[i].name);
			j++;
		}
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "  max size: %dx%d\n", x->max_width, x->max_height);
}

int
xv_init(struct video *vid)
{
	struct xdsp *x = &vid->xdsp;
	XTextProperty WinName;
	XSizeHints szhints;
	XWMHints wmhints;
	char *name;

	x->width = vid->width;
	x->height = vid->height;

	x->window = XCreateSimpleWindow(x->dpy, x->rwin, 0, 0, x->width,
	    x->height, 0, XWhitePixel(x->dpy, x->screen_id),
	    XBlackPixel(x->dpy, x->screen_id));

	szhints.flags = PSize | PMaxSize | PMinSize;
	szhints.width = x->width;
	szhints.height = x->height;
	szhints.max_width = x->max_width;
	szhints.max_height = x->max_height;
	szhints.min_width = 160;
	szhints.min_height = 120;

	wmhints.flags = InputHint | StateHint;
	wmhints.input = True;
	wmhints.initial_state = NormalState;

	name = __progname;
	XStringListToTextProperty(&name, 1, &WinName);
	XSetWMProperties(x->dpy, x->window, &WinName, &WinName, NULL, 0,
	    &szhints, &wmhints, NULL);

	XSelectInput(x->dpy, x->window,
	    KeyPressMask | ButtonPressMask | StructureNotifyMask);

	x->wmdelwin = XInternAtom(x->dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(x->dpy, x->window, &x->wmdelwin, 1);

	x->gc = XCreateGC(x->dpy, x->window, 0, NULL);

	XMapRaised(x->dpy, x->window);

	resize_window(vid, 0);

	x->xv_image = XvCreateImage(x->dpy, x->port, encs[vid->enc_xv].id,
	    vid->frame_buffer, vid->width, vid->height);

	return 1;
}

void
net_wm_supported(struct video *vid)
{
	struct xdsp *x = &vid->xdsp;
	Atom query, fullscreen;
	Atom type;
	Atom *data;
	long off = 0, len = 12;
	int fmt;
	unsigned long nitems, remain;
	int i;

	query = XInternAtom(x->dpy, "_NET_SUPPORTED", True);
	fullscreen = XInternAtom(x->dpy, "_NET_WM_STATE_FULLSCREEN", True);
	if (query == None || fullscreen == None)
		return;

	do {
		if (XGetWindowProperty(x->dpy, x->rwin, query, off, len,
		    False, XA_ATOM, &type, &fmt, &nitems, &remain,
		    (unsigned char **)&data) != Success)
			return;

		if (type == XA_ATOM && fmt == 32) {
			for (i = 0; i < nitems; i++) {
				if (data[i] == fullscreen) {
					vid->net_wm = 1;
					XFree(data);
					return;
				}
			}
		}
		XFree(data);

		off += len;
	} while (remain > 0);

	return;
}

void
resize_window(struct video *vid, int fullscreen)
{
	struct xdsp *x = &vid->xdsp;
	XWindowAttributes winatt;
	Window junk;
	int new_x;
	int new_y;
	XEvent ev;
	Atom property, value;

	if (fullscreen == 1) {
		if (vid->full_screen == 1) {
			x->width = x->saved_w;
			x->height = x->saved_h;
			new_x = x->saved_x;
			new_y = x->saved_y;
		} else {
			x->width = x->max_width;
			x->height = x->max_height;
			new_x = 0;
			new_y = 0;
		}

		vid->full_screen = !vid->full_screen;

		if (vid->net_wm) {
			property = XInternAtom(x->dpy, "_NET_WM_STATE", False);
			value = XInternAtom(x->dpy, "_NET_WM_STATE_FULLSCREEN",
			    False);

			memset(&ev, 0, sizeof(ev));
			ev.type = ClientMessage;
			ev.xclient.window = x->window;
			ev.xclient.message_type = property;
			ev.xclient.format = 32;
			ev.xclient.data.l[0] = vid->full_screen;
			ev.xclient.data.l[1] = value;

			XSendEvent(x->dpy, x->rwin, False,
			    SubstructureNotifyMask | SubstructureRedirectMask,
			    &ev);
		} else {
			XMoveResizeWindow(x->dpy, x->window, x->width,
			    x->height, new_x, new_y);
		}
	} else if (!vid->full_screen) {
		XGetWindowAttributes(x->dpy, x->window, &winatt);
		XTranslateCoordinates(x->dpy, x->window, x->rwin,
		   -winatt.x, -winatt.y, &new_x, &new_y, &junk);
		x->saved_w = x->width = winatt.width;
		x->saved_h = x->height = winatt.height;
		x->saved_x = new_x;
		x->saved_y = new_y;
	}

	x->resized = 1;

	if ((double)x->width > (double)x->height * vid->aspect) {
		x->box_h = x->height;
		x->box_y = 0;
		x->box_w = (int)(x->height * vid->aspect);
		x->box_x = (x->width - x->box_w) / 2;
	} else {
		x->box_w = x->width;
		x->box_x = 0;
		x->box_h = (int)(x->width / vid->aspect);
		x->box_y = (x->height - x->box_h) / 2;
	}

	XSync(x->dpy, False);
	XSync(x->dpy, True);
}

void
display_event(struct video *vid)
{
	struct xdsp *x = &vid->xdsp;
	char str;

	if (XPending(x->dpy)) {
		XNextEvent(x->dpy, &x->event);
		switch (x->event.type) {
		case KeyPress:
			if (vid->verbose > 2)
				warnx("got KeyPress event");
			XLookupString(&x->event.xkey, &str, 1, NULL, NULL);
			switch (str) {
			case 'A':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_SHARPNESS, 1);
				break;
			case 'a':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_SHARPNESS, -1);
				break;
			case 'B':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_BRIGHTNESS, 1);
				break;
			case 'b':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_BRIGHTNESS, -1);
				break;
			case 'C':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_CONTRAST, 1);
				break;
			case 'c':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_CONTRAST, -1);
				break;
			case 'f':
				resize_window(vid, 1);
				break;
			case 'G':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_GAIN, 1);
				break;
			case 'g':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_GAIN, -1);
				break;
			case 'H':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_HUE, 1);
				break;
			case 'h':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_HUE, -1);
				break;
			case 'O':
				if (!wout && vid->verbose > 0)
					fprintf(stderr, "starting output\n");
				wout = 1;
				break;
			case 'o':
				if (wout && vid->verbose > 0)
					fprintf(stderr, "stopping output\n");
				wout = 0;
				break;
			case 'M':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_GAMMA, 1);
				break;
			case 'm':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_GAMMA, -1);
				break;
			case 'p':
				hold = !hold;
				break;
			case 'q':
				shutdown = 1;
				break;
			case 'r':
				if (vid->mode & M_IN_DEV)
					dev_reset_ctrls(vid);
				break;
			case 'S':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_SATURATION, 1);
				break;
			case 's':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid, CTRL_SATURATION, -1);
				break;
			case 'W':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid,
					    CTRL_WHITE_BALANCE_TEMPERATURE, 10);
				break;
			case 'w':
				if (vid->mode & M_IN_DEV)
					dev_set_ctrl_rel(vid,
					    CTRL_WHITE_BALANCE_TEMPERATURE, -10);
				break;
			default:
				break;
			}
			break;
		case ClientMessage:
			if (vid->verbose > 2)
				warnx("got ClientMessage event");
			if (x->event.xclient.data.l[0] == x->wmdelwin) {
				shutdown = 1;
				break;
			}
			break;
		case ConfigureNotify:
			if (vid->verbose > 2)
				warnx("got ConfigureNotify event");
			resize_window(vid, 0);
			break;
		default:
			break;
		}
	}
}

int
dev_check_caps(struct video *vid)
{
	struct dev *d = &vid->dev;
	struct v4l2_capability cap;

	if ((d->fd = open(d->path, O_RDWR)) < 0) {
		warn("%s", d->path);
		return 0;
	}

	if (ioctl(d->fd, VIDIOC_QUERYCAP, &cap) < 0) {
		warn("VIDIOC_QUERYCAP");
		return 0;
	}
	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		warnx("%s is not a capture device", d->path);
		return 0;
	}
	if (!(cap.capabilities & V4L2_CAP_READWRITE) && !vid->mmap_on) {
		warnx("%s does not support read(2)", d->path);
		return 0;
	}
	if (!(cap.capabilities & V4L2_CAP_STREAMING) && vid->mmap_on) {
		warnx("%s does not support mmap(2)", d->path);
		return 0;
	}
	d->buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	return 1;
}

int
dev_get_encs(struct video *vid)
{
	struct dev *d = &vid->dev;
	struct v4l2_fmtdesc fmtdesc;
	int i;

	fmtdesc.index = 0;
	fmtdesc.type = d->buf_type;
	while (ioctl(d->fd, VIDIOC_ENUM_FMT, &fmtdesc) >= 0) {
		i = find_enc_by_id(fmtdesc.pixelformat);
		if (i < ENC_LAST)
			encs[i].flags |= SUP_DEV;
		fmtdesc.index++;
	}
	for (i = 0; encs[i].name; i++) {
		if (encs[i].flags & SUP_DEV)
			break;
	}
	if (i >= ENC_LAST) {
		warnx("%s has no usable YUV encodings", d->path);
		return 0;
	}

	return 1;
}

int
dev_get_sizes(struct video *vid)
{
	struct dev *d = &vid->dev;
	struct v4l2_frmsizeenum fsize;
	struct dim sizes[MAX_DSZS];
	int i, j, k, nsizes, tmp_w, tmp_h, step_w, step_h;

	nsizes = 0;
	fsize.index = 0;
	fsize.pixel_format = encs[vid->enc].id;
	while (ioctl(d->fd, VIDIOC_ENUM_FRAMESIZES, &fsize) == 0) {
		switch (fsize.type) {
		case V4L2_FRMSIZE_TYPE_DISCRETE:
			sizes[nsizes].w = fsize.discrete.width;
			sizes[nsizes].h = fsize.discrete.height;
			nsizes++;
			break;
		case V4L2_FRMSIZE_TYPE_CONTINUOUS:
			step_w = (((fsize.stepwise.max_width -
			    fsize.stepwise.min_width) / MAX_DSZS) + 15) & ~15;
			step_h = (((fsize.stepwise.max_height -
			    fsize.stepwise.min_height) / MAX_DSZS) + 15) & ~15;
			for (tmp_w = fsize.stepwise.min_width,
			    tmp_h = fsize.stepwise.min_height;
			    tmp_w <= fsize.stepwise.max_width &&
			    tmp_h <= fsize.stepwise.max_height;
			    tmp_w += step_w, tmp_h += step_h) {
				sizes[nsizes].w = tmp_w;
				sizes[nsizes].h = tmp_h;
				if (++nsizes >= MAX_DSZS)
					break;
			}
			break;
		case V4L2_FRMSIZE_TYPE_STEPWISE:
			step_w = (((fsize.stepwise.max_width -
			    fsize.stepwise.min_width) / MAX_DSZS) +
			    fsize.stepwise.step_width - 1) &
			    ~(fsize.stepwise.step_width - 1);
			step_h = (((fsize.stepwise.max_height -
			    fsize.stepwise.min_height) / MAX_DSZS) +
			    fsize.stepwise.step_height - 1) &
			    ~(fsize.stepwise.step_height - 1);
			for (tmp_w = fsize.stepwise.min_width,
			    tmp_h = fsize.stepwise.min_height;
			    tmp_w <= fsize.stepwise.max_width &&
			    tmp_h <= fsize.stepwise.max_height;
			    tmp_w += step_w, tmp_h += step_h) {
				sizes[nsizes].w = tmp_w;
				sizes[nsizes].h = tmp_h;
				if (++nsizes >= MAX_DSZS)
					break;
			}
			break;
		}
		if (nsizes >= MAX_DSZS)
			break;
		fsize.index++;
	}
	if (nsizes == 0) {
		warnx("%s doesn't have any video frame sizes", d->path);
		return 0;
	}

	/* insert sort increasing based first on width then height */
	d->sizes[0].w = sizes[0].w;
	d->sizes[0].h = sizes[0].h;
	d->nsizes = 1;
	for (i = 1; i < nsizes; i++) {
		for (j = 0; j < d->nsizes; j++) {
			if (sizes[i].w < d->sizes[j].w)
				break;
			if (sizes[i].w == d->sizes[j].w) {
				if (sizes[i].h < d->sizes[j].h)
					break;
			}
		}
		if (j < d->nsizes) {
			for (k = d->nsizes; k > j; k--) {
				d->sizes[k].w = d->sizes[k - 1].w;
				d->sizes[k].h = d->sizes[k - 1].h;
			}
		}
		d->sizes[j].w = sizes[i].w;
		d->sizes[j].h = sizes[i].h;
		d->nsizes++;
	}

	return 1;
}

int
dev_get_rates(struct video *vid)
{
	struct dev *d = &vid->dev;
	struct v4l2_frmivalenum ival;
	struct v4l2_frmival_stepwise *s;
	int i, j, num;

	for (i = 0; i < d->nsizes; i++) {
		bzero(&ival, sizeof(ival));
		ival.pixel_format = encs[vid->enc].id;
		ival.width = d->sizes[i].w;
		ival.height = d->sizes[i].h;
		ival.index = 0;
		while (ioctl(d->fd, VIDIOC_ENUM_FRAMEINTERVALS, &ival) != -1) {
			switch(ival.type) {
			case V4L2_FRMIVAL_TYPE_DISCRETE:
				if (ival.index < MAX_RATES) {
					d->rates[i][ival.index] =
					    ival.discrete.denominator /
					    ival.discrete.numerator;
				}
				break;
			case V4L2_FRMIVAL_TYPE_CONTINUOUS:
			case V4L2_FRMIVAL_TYPE_STEPWISE:
				if (ival.index != 0) {
					printf("invalid frame type!\n");
					return 0;
				}
				s = &ival.stepwise;
				if (s->step.denominator != s->min.denominator ||
				    s->step.denominator != s->max.denominator) {
					printf("can't parse frame rate!\n");
					break;
				}
				for (num = s->min.numerator, j = 0;
				    num <= s->max.numerator && j < MAX_RATES;
				    num += s->step.numerator, j++) {
					d->rates[i][j] =
					    s->step.denominator / num;
				}
				break;
			default:
				printf("invalid frame type!\n");
				return 0;
			}
			ival.index++;
		}
	}

	return 1;
}

int
dev_get_ctrls(struct video *vid)
{
	struct dev *d = &vid->dev;
	struct v4l2_queryctrl qctl;
	struct v4l2_control control;
	int i;

	for (i = 0; i < CTRL_LAST; i++) {
		bzero(&qctl, sizeof(struct v4l2_queryctrl));
		qctl.id = ctrls[i].id;
		if (ioctl(d->fd, VIDIOC_QUERYCTRL, &qctl) == -1) {
			if (errno == EINVAL)
				continue;
			warn("VIDIOC_QUERYCTRL");
			return 0;
		}
		if (qctl.flags & V4L2_CTRL_FLAG_DISABLED)
			continue;
		if (qctl.type == V4L2_CTRL_TYPE_MENU)
			continue;
		ctrls[i].def = qctl.default_value;
		ctrls[i].min = qctl.minimum;
		ctrls[i].max = qctl.maximum;
		ctrls[i].step = qctl.step;

		bzero(&qctl, sizeof(struct v4l2_queryctrl));
		control.id = ctrls[i].id;
		if (ioctl(d->fd, VIDIOC_G_CTRL, &control) != 0) {
			warn("VIDIOC_G_CTRL");
			continue;
		}
		ctrls[i].cur = control.value;
		ctrls[i].supported = 1;
	}

	return 1;
}

int
dev_set_ctrl_abs(struct video *vid, int ctrl, int val)
{
	struct dev *d = &vid->dev;
	struct v4l2_control control;

	if (ctrl < 0 || ctrl >= CTRL_LAST) {
		warnx("invalid control");
		return -1;
	}
	if (!ctrls[ctrl].supported) {
		warnx("control %s not supported by %s",
		    ctrls[ctrl].name, d->path);
		return -1;
	}
	if (ctrl == CTRL_WHITE_BALANCE_TEMPERATURE) {
		/*
		 * The spec requires auto-white balance to be off before
		 * we can set the white balance temperature.
		 */
		dev_set_ctrl_auto(vid, ctrl, 0, 0);
	}
	if (val > ctrls[ctrl].max)
		val = ctrls[ctrl].max;
	else if (val < ctrls[ctrl].min)
		val = ctrls[ctrl].min;
	control.id = ctrls[ctrl].id;
	control.value = val;
	if (ioctl(d->fd, VIDIOC_S_CTRL, &control) != 0) {
		warn("VIDIOC_S_CTRL");
		return -1;
	}
	control.id = ctrls[ctrl].id;
	if (ioctl(d->fd, VIDIOC_G_CTRL, &control) != 0) {
		warn("VIDIOC_G_CTRL");
		return -1;
	}
	ctrls[ctrl].cur = control.value;
	if (vid->verbose > 0)
		fprintf(stderr, "%s now %d\n", ctrls[ctrl].name,
		    ctrls[ctrl].cur);

	return 0;
}

void
dev_set_ctrl_rel(struct video *vid, int ctrl, int change)
{
	struct dev *d = &vid->dev;
	int val;

	if (ctrl < 0 || ctrl >= CTRL_LAST) {
		warnx("invalid control");
		return;
	}
	if (!ctrls[ctrl].supported) {
		warnx("control %s not supported by %s",
		    ctrls[ctrl].name, d->path);
		return;
	}
	val = ctrls[ctrl].cur + ctrls[ctrl].step * change;
	dev_set_ctrl_abs(vid, ctrl, val);
}

void
dev_set_ctrl_auto(struct video *vid, int ctrl, int value, int reset)
{
	struct dev *d = &vid->dev;
	struct v4l2_control control;

	if (!ctrls[ctrl].id_auto)
		return;

	control.id = ctrls[ctrl].id_auto;
	if (ioctl(d->fd, VIDIOC_G_CTRL, &control) != 0) {
		warn("VIDIOC_G_CTRL");
		return;
	}

	if (reset) {
		if (ioctl(d->fd, VIDIOC_S_CTRL, &control) != 0)
			warn("VIDIOC_S_CTRL");
	} else {
		if (control.value == value)
			return;
		control.value = value;
		if (ioctl(d->fd, VIDIOC_S_CTRL, &control) != 0)
			warn("VIDIOC_S_CTRL");
	}
}

int
dev_get_ctrl_auto(struct video *vid, int ctrl)
{
	struct dev *d = &vid->dev;
	struct v4l2_control control;

	if (!ctrls[ctrl].id_auto)
		return 0;

	control.id = ctrls[ctrl].id_auto;
	if (ioctl(d->fd, VIDIOC_G_CTRL, &control) != 0) {
		warn("VIDIOC_G_CTRL");
		return 0;
	}

	return (control.value);
}

void
dev_reset_ctrls(struct video *vid)
{
	struct dev *d = &vid->dev;
	struct v4l2_control control;
	int i;

	for (i = 0; i < CTRL_LAST; i++) {
		if (!ctrls[i].supported)
			continue;
		dev_set_ctrl_abs(vid, i, ctrls[i].def);
		dev_set_ctrl_auto(vid, i, 1, 0);
	}
}

void
dev_dump_info(struct video *vid)
{
	struct dev *d = &vid->dev;
	int i, j;

	if (!vid->mmap_on)
		fprintf(stderr, "Using read instead of mmap to grab frames\n");

	fprintf(stderr, "video device %s:\n", d->path);

	fprintf(stderr, "  encodings: ");
 	for (i = 0, j = 0; i < ENC_LAST; i++) {
		if (encs[i].flags & SUP_DEV) {
			if (j)
				fprintf(stderr, ", ");
			fprintf(stderr, "%s", encs[i].name);
			j++;
		}
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "  frame sizes (width x height, in pixels) and rates (in frames per second):\n");
	for (i = 0; i < d->nsizes; i++) {
		fprintf(stderr, "\t%dx%d: ", d->sizes[i].w, d->sizes[i].h);
		for (j = 0; j < MAX_RATES; j++) {
			if (d->rates[i][j] == 0)
				break;
			if (j)
				fprintf(stderr, ", ");
			fprintf(stderr, "%d", d->rates[i][j]);
		}
		fprintf(stderr, "\n");
	}

	fprintf(stderr, "  controls: ");
	for (i = 0, j = 0; i < CTRL_LAST; i++) {
		if (ctrls[i].supported) {
			if (j)
				fprintf(stderr, ", ");
			fprintf(stderr, "%s", ctrls[i].name);
			j++;
		}
	}
	fprintf(stderr, "\n");
}

void
dev_dump_query(struct video *vid)
{
	if (!dev_check_caps(vid))
		return;
	if (!dev_get_encs(vid))
		return;
	if (!choose_enc(vid))
		return;
	if (!dev_get_sizes(vid))
		return;
	if (!dev_get_rates(vid))
		return;
	if (!dev_get_ctrls(vid))
		return;
	dev_dump_info(vid);
}

void
dev_dump_query_ctrls(struct video *vid)
{
	int i;

	for (i = 0; i < CTRL_LAST; i++) {
		if (!ctrls[i].supported)
			continue;

		if (dev_get_ctrl_auto(vid, i))
			fprintf(stderr, "%s=auto\n", ctrls[i].name);
		else
			fprintf(stderr, "%s=%d\n", ctrls[i].name, ctrls[i].cur);
	}
}

int
dev_init(struct video *vid)
{
	struct dev *d = &vid->dev;
	struct v4l2_format fmt;
	struct v4l2_streamparm parm;
	int fps;

	bzero(&fmt, sizeof(struct v4l2_format));
	fmt.type = d->buf_type;
	fmt.fmt.pix.width = vid->width;
	fmt.fmt.pix.height = vid->height;
	fmt.fmt.pix.pixelformat = encs[vid->enc].id;
	fmt.fmt.pix.field = V4L2_FIELD_ANY;
	if (ioctl(d->fd, VIDIOC_S_FMT, &fmt) < 0) {
		warn("VIDIOC_S_FMT");
		return 0;
	}
	if (fmt.fmt.pix.width != vid->width ||
	    fmt.fmt.pix.height != vid->height) {
		warnx("%s: returned size not as requested", d->path);
		return 0;
	}

	bzero(&parm, sizeof(parm));
	parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (vid->fps) {
		parm.parm.capture.timeperframe.numerator = 1;
		parm.parm.capture.timeperframe.denominator = vid->fps;
	}
	if (ioctl(d->fd, VIDIOC_S_PARM, &parm) < 0) {
		warn("VIDIOC_S_PARM");
		return 0;
	}

	bzero(&parm, sizeof(parm));
	parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(d->fd, VIDIOC_G_PARM, &parm) < 0) {
		warn("VIDIOC_G_PARM");
		return 0;
	}
	fps = parm.parm.capture.timeperframe.denominator /
	    parm.parm.capture.timeperframe.numerator;
	if (vid->fps && fps != vid->fps)
		warnx("device returned %d fps, claimed support for %d fps",
		    fps, vid->fps);

	return 1;
}

int
parse_ctrl(struct video *vid, int argc, char **argv)
{
	int i, val_old, auto_old, val_new;
	char *p;
	const char *errstr;

	if (*argv == NULL)
		return 1;	/* No control arguments found. */

	if (!dev_check_caps(vid))
		return 0;
	if (!dev_get_ctrls(vid))
		return 0;

	for (; argc > 0; argc--, argv++) {
		p = strchr(*argv, '=');

		/* Display control value. */
		if (p == NULL) {
			for (i = 0; i < CTRL_LAST; i++) {
				if (!strcmp(*argv, ctrls[i].name)) {
					fprintf(stderr, "%s=%d\n",
					    ctrls[i].name, ctrls[i].cur);
					break;
				}
			}
			if (i == CTRL_LAST)
				warnx("%s: unknown control", *argv);	
			continue;
		}

		/* Set control value. */
		for (i = 0, *p++ = '\0'; i < CTRL_LAST; i++) {
			if (strcmp(*argv, ctrls[i].name) != 0)
				continue;
			if (*p == '\0') {
				warnx("%s: no value", *argv);
				break;
			}
			auto_old = dev_get_ctrl_auto(vid, i);
			val_old = ctrls[i].cur;
			if (strcmp(p, "auto") == 0) {
				if (ctrls[i].id_auto == 0) {
					fprintf(stderr,
					    "%s: no automatic control found\n",
					    ctrls[i].name);
				} else if (!auto_old) {
					fprintf(stderr, "%s: %d -> auto\n",
					    ctrls[i].name, val_old);
					dev_set_ctrl_auto(vid, i, 1, 0);
				} else {
					fprintf(stderr,
					    "%s: auto -> auto\n",
					    ctrls[i].name);
				}
			} else {
				val_new = strtonum(p, -32768, 32768, &errstr);
				if (errstr != NULL) {
					warnx("%s: %s", *argv, errstr);
					return 0;
				}
				if (dev_set_ctrl_abs(vid, i, val_new) == 0) {
					if (auto_old) {
						fprintf(stderr,
						    "%s: auto -> %d\n",
						    ctrls[i].name,
						    ctrls[i].cur);
					} else {
						fprintf(stderr,
						    "%s: %d -> %d\n",
						    ctrls[i].name, val_old,
						    ctrls[i].cur);
					}
				}
			}
			break;
		}
		if (i == CTRL_LAST)
			warnx("%s: unknown control", *argv);
	}

	return 0;
}

int
parse_size(struct video *vid)
{
	struct xdsp *x = &vid->xdsp;
	char **dimp, *dims[2];
	const char *errstr;
	size_t dimx;

	if (!vid->sz_str) {
		vid->width = 640;
		vid->height = 480;
		return 1;
	}

	if (!strcmp(vid->sz_str, "full") ||
	    !strcmp(vid->sz_str, "half")) {
		if (!(vid->mode & M_IN_DEV) || !(vid->mode & M_OUT_XV)) {
			warnx("size '%s' not valid for this mode", vid->sz_str);
			return 0;
		}
		vid->width = x->max_width * 2 / 3;
		vid->height = x->max_height * 2 / 3;
		return 1;
	}

	dimx = strcspn(vid->sz_str, "x");
	for (dimp = dims; dimp < &dims[2] &&
	    (*dimp = strsep(&vid->sz_str, "x")) != NULL; ) {
		if (**dimp != '\0')
			dimp++;
	}
	if (dimx > 0) {
		if (dims[0] != NULL) {
			vid->width = strtonum(dims[0], 0, 4096, &errstr);
			if (errstr != NULL) {
				warnx("width '%s' is %s", dims[0], errstr);
				return 0;
			}
		}
		if (dims[1] != NULL) {
			vid->height = strtonum(dims[1], 0, 4096, &errstr);
			if (errstr != NULL) {
				warnx("height '%s' is %s", dims[1], errstr);
				return 0;
			}
		}
	} else if (dims[0] != NULL) {
		vid->height = strtonum(dims[0], 0, 4096, &errstr);
		if (errstr != NULL) {
			warnx("height '%s' is %s", dims[0], errstr);
			return 0;
		}
	}
	return 1;
}

int
choose_size(struct video *vid)
{
	struct xdsp *x = &vid->xdsp;
	struct dev *d = &vid->dev;
	int i, j, diff, best, cur;

	if (vid->height && !vid->width)
		vid->width = vid->height * 4 / 3;
	else if (vid->width && !vid->height)
		vid->height = vid->width * 3 / 4;

	if (vid->mode & M_OUT_XV) {
		if (vid->width > x->max_width)
			vid->width = x->max_width;
		if (vid->height > x->max_height)
			vid->height = x->max_height;
	}

	if (vid->mode & M_IN_DEV) {
		i = 0;
		while (i < d->nsizes &&
		    d->sizes[i].h <= vid->height &&
		    d->sizes[i].w <= vid->width)
			i++;
		if (i >= d->nsizes)
			i = d->nsizes - 1;
		if (i > 0 && (d->sizes[i].h > vid->height ||
		    d->sizes[i].w > vid->width))
			i--;
		vid->width = d->sizes[i].w;
		vid->height = d->sizes[i].h;

		/* interval is a property of the frame */
		if (!vid->nofps) {
			cur = j = 0;
			best = INT_MAX;
			while (j < MAX_RATES && d->rates[i][j]) {
				diff = abs(vid->fps - d->rates[i][j]);
				if (diff < best) {
					best = diff;
					cur = j;
					if (diff == 0)
						break;
				}
				j++;
			}
			vid->fps = d->rates[i][cur];
		}
	}
	vid->aspect = (double)vid->width / (double)vid->height;

	return 1;
}

int
choose_enc(struct video *vid)
{
	int i;
	int enc, enc_xv;

	if (vid->enc < 0) {
		if (vid->mode & M_IN_DEV) {
			for (i = 0; vid->enc < 0 && i < ENC_LAST; i++)
				if (encs[i].flags & SUP_DEV)
					vid->enc = i;
		} else {
			vid->enc = ENC_YUYV;
		}
		if ((vid->mode & M_IN_DEV) && (vid->mode & M_OUT_XV)) {
			for (i = 0; vid->enc < 0 && i < ENC_LAST; i++)
				if (encs[i].flags == (SUP_DEV | SUP_XV))
					vid->enc = vid->enc_xv = i;
		}
	}
	if (vid->enc < 0) {
		warnx("could not find a usable encoding");
		return 0;
	}
	if ((vid->mode & M_IN_DEV) && !(encs[vid->enc].flags & SUP_DEV)) {
		warnx("device %s can't supply %s", vid->dev.path,
		    encs[vid->enc].name);
		return 0;
	}
	if (vid->enc_xv < 0 && (vid->mode & M_OUT_XV)) {
		if (encs[vid->enc].flags & SUP_XV)
			vid->enc_xv = vid->enc;
		for (i = 0; vid->enc_xv < 0 && i < ENC_LAST; i++)
			if (encs[i].flags & SUP_XV)
				vid->enc_xv = i;
	}

	if (vid->mode & M_OUT_XV && vid->enc != vid->enc_xv) {
		/* check if conversion is possible */
		enc = (vid->enc == ENC_YUYV) ? ENC_YUY2 : vid->enc;
		enc_xv = (vid->enc_xv == ENC_YUYV) ? ENC_YUY2 : vid->enc_xv;
		if (enc == enc_xv)
			vid->conv_type = CONV_NONE;
		else {
			if (enc == ENC_UYVY || enc_xv == ENC_UYVY)
				vid->conv_type |= CONV_SWAP;
			if (enc_xv == ENC_YV12)
				vid->conv_type |= CONV_YV12;
		}
		if (!vid->conv_type)
			vid->enc_xv = vid->enc;
	}
	if ((vid->mode & M_OUT_XV) && !(encs[vid->enc_xv].flags & SUP_XV)) {
		warnx("Xv adaptor %d can't display %s",
		    vid->xdsp.adaps[vid->xdsp.cur_adap].adap_index,
		    encs[vid->enc_xv].name);
		return 0;
	}

	return 1;
}

int
mmap_init(struct video *vid)
{
	struct v4l2_requestbuffers rb;
	struct v4l2_buffer buf;
	int i, r, type;

	/* request buffers */
	rb.count = MMAP_NUM_BUFS;
	rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	rb.memory = V4L2_MEMORY_MMAP;
	r = ioctl(vid->dev.fd, VIDIOC_REQBUFS, &rb);
	if (r == -1) {
		warn("ioctl VIDIOC_REQBUFS");
		return 0;
	}

	/* mmap the buffers */
	for (i = 0; i < MMAP_NUM_BUFS; i++) {
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		r = ioctl(vid->dev.fd, VIDIOC_QUERYBUF, &buf);
		if (r == -1) {
			warn("ioctl VIDIOC_QUERYBUF");
			return 0;
		}
		vid->mmap_buffer[i] = mmap(0, buf.length, PROT_READ,
		    MAP_SHARED, vid->dev.fd, buf.m.offset);
		if (vid->mmap_buffer[i] == MAP_FAILED) {
			warn("mmap");
			return 0;
		}
	}

	/* initial buffer queueing */
	for (i = 0; i < MMAP_NUM_BUFS; i++) {
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		r = ioctl(vid->dev.fd, VIDIOC_QBUF, &buf);
		if (r == -1) {
			warn("ioctl VIDIOC_QBUF");
			return 0;
		}
	}

	/* start video stream */
	r = ioctl(vid->dev.fd, VIDIOC_STREAMON, &type);
	if (r == -1) {
		warn("ioctl VIDIOC_STREAMON");
		return 0;
	}

	return 1;
}

int
mmap_stop(struct video *vid)
{
	int i, r, type;

	/* stop video stream */
	r = ioctl(vid->dev.fd, VIDIOC_STREAMOFF, &type);
	if (r == -1) {
		warn("ioctl STREAMOFF");
		return 0;
	}

	/* unmap the buffers */
	for (i = 0; i < MMAP_NUM_BUFS; i++) {
		if (vid->mmap_buffer[i] != NULL &&
		    vid->mmap_buffer[i] != MAP_FAILED) {
			r = munmap(vid->mmap_buffer[i], vid->bpf);
			if (r == -1) {
				warn("munmap");
				return 0;
			}
		}
	}

	return 1;
}

int
setup(struct video *vid)
{
	if (vid->mode & M_IN_FILE) {
		if (!strcmp(vid->iofile, "-"))
			vid->iofile_fd = STDIN_FILENO;
		else
			vid->iofile_fd = open(vid->iofile, O_RDONLY);
	} else if (vid->mode & M_OUT_FILE) {
		if (!strcmp(vid->iofile, "-"))
			vid->iofile_fd = STDOUT_FILENO;
		else
			vid->iofile_fd = open(vid->iofile,
			    O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	if (vid->mode & (M_IN_FILE | M_OUT_FILE)) {
		if (vid->iofile_fd < 0) {
			warn("%s", vid->iofile);
			return 0;
		}
	}

	if ((vid->mode & M_OUT_XV) && !xv_get_info(vid))
		return 0;

	if ((vid->mode & M_IN_DEV) &&
	    (!dev_check_caps(vid) || !dev_get_encs(vid)))
		return 0;

	if (!choose_enc(vid))
		return 0;

	if ((vid->mode & M_OUT_XV) && !xv_sel_adap(vid))
		return 0;

	if ((vid->mode & M_IN_DEV) &&
	    (!dev_get_sizes(vid) || !dev_get_rates(vid) || !dev_get_ctrls(vid)))
		return 0;

	if (!parse_size(vid) || !choose_size(vid))
		return 0;

	vid->bpf = vid->width * vid->height * encs[vid->enc].bpp / NBBY;

	if (vid->verbose > 0) {
		if (vid->mode & M_IN_DEV)
			dev_dump_info(vid);
		if (vid->mode & M_OUT_XV)
			xv_dump_info(vid);
		fprintf(stderr, "using %s encoding\n", encs[vid->enc].name);
		fprintf(stderr, "using frame size %dx%d (%d bytes)\n",
		    vid->width, vid->height, vid->bpf);
		if (vid->fps)
			fprintf(stderr, "using frame rate %d fps\n", vid->fps);
		else
			fprintf(stderr, "using default frame rate\n");
	}

	if ((vid->frame_buffer = calloc(1, vid->bpf)) == NULL) {
		warn("frame_buffer");
		return 0;
	}

	if (vid->conv_type > CONV_NONE) {
		vid->conv_bufsz =
		    (vid->width * vid->height * encs[vid->enc_xv].bpp) / NBBY;
		if ((vid->conv_buffer = calloc(1, vid->conv_bufsz)) == NULL) {
			warn("conv_buffer");
			return 0;
		}
	}

	if ((vid->mode & M_IN_DEV) && !dev_init(vid))
		return 0;

	if ((vid->mode & M_OUT_XV) && !xv_init(vid))
		return 0;

	if (vid->sz_str && !strcmp(vid->sz_str, "full"))
		resize_window(vid, 1);

	if (vid->mmap_on) {
		if (!mmap_init(vid))
			return 0;
	}

	/*
	 * Reset the current White Balance Temperature Auto Control value
	 * after the video stream has been started since some cams only
	 * process this control while the video stream is on.
	 */
	dev_set_ctrl_auto(vid, CTRL_WHITE_BALANCE_TEMPERATURE, 0, 1);

	if (vid->mode & M_OUT_XV)
		net_wm_supported(vid);

	return 1;
}

int
ioctl_input(struct video *vid)
{
	struct v4l2_buffer buf;
	int r;

	/* dequeue buffer */
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	r = ioctl(vid->dev.fd, VIDIOC_DQBUF, &buf);
	if (r == -1) {
		warn("ioctl VIDIOC_DQBUF");
		return 0;
	}

	/* copy frame buffer */
	if (buf.bytesused > vid->bpf)
		return 0;
	memcpy(vid->frame_buffer, vid->mmap_buffer[buf.index], buf.bytesused);

	/* requeue buffer */
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	r = ioctl(vid->dev.fd, VIDIOC_QBUF, &buf);
	if (r == -1) {
		warn("ioctl VIDIOC_QBUF");
		return 0;
	}

	return 1;
}

int
poll_input(struct video *vid)
{
	struct pollfd pfds[1];
	int ret;

	pfds[0].fd = (vid->mode & M_IN_FILE) ? vid->iofile_fd : vid->dev.fd;
	pfds[0].events = POLLIN;

	ret = poll(pfds, 1, INFTIM);
	if (ret != 1)
		return ret;

	if (pfds[0].revents & (POLLERR | POLLHUP | POLLNVAL))
		return pfds[0].revents;

	if (!(pfds[0].revents & POLLIN))
		return 0;

	return 1;
}

int
grab_frame(struct video *vid)
{
	int fd, todo, done, ret;

	if (vid->mode & M_IN_FILE)
		fd = vid->iofile_fd;
	else
		fd = vid->dev.fd;

	done = 0;
	todo = vid->bpf;
	while (todo > 0) {
		ret = read(fd, vid->frame_buffer + done, todo);
		if (ret <= 0) {
			done = ret;
			break;
		}
		if (ret < todo) {
			/* video(4) doesn't support partial read() */
			if (vid->mode & M_IN_DEV) {
				done = 0;
				break;
			}
		}
		todo -= ret;
		done += ret;
	}

	if (done != vid->bpf) {
		if (done == 0 && (vid->mode & M_IN_FILE)) {
			if (vid->verbose > 1)
				warnx("%s: EOF", vid->iofile);
			return 255;
		}
		warn("%s read %d/%d", (vid->mode & M_IN_DEV) ?
		    vid->dev.path : vid->iofile, done, vid->bpf);
		return 0;
	}

	return 1;
}

void
got_frame(int s)
{
	play = 1;
}

void
got_shutdown(int s)
{
	shutdown = 1;
}

int
yuy2_to_yv12(uint8_t *src, uint8_t *dst, int width, int height, int swap)
{
	int row, col;
	uint8_t *s = src, *p;
	uint8_t *dy = dst;
	uint8_t *du = dy + width * height;
	uint8_t *dv = du + width * height / 4;

	if ((width | height) & 1)
		errx(1, "frame size %dx%d is not supported", width, height);

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col += 2) {
			p = (swap) ? s + 1 : s;
			*(dy++) = *p;
			*(dy++) = *(p + 2);
			if (!(row & 0x01)) {
				p = (swap) ? s - 1 : s;
				*(du++) =
				    (*(p + 3) + *(p + 3 + width * 2) + 1) / 2;
				*(dv++) =
				    (*(p + 1) + *(p + 1 + width * 2) + 1) / 2;
			}
			s += 4;
		}
	}

	return 0;
}

int
stream(struct video *vid)
{
	struct xdsp *x = &vid->xdsp;
	struct timespec tp_start, tp_now, tp_run;
	struct itimerval frit;
	double run_time;
	uint8_t *src;
	long frames_played = -1, frames_grabbed = 0, fus = 50000;
	int sequence = 20, ret, err, todo, done;

	/* Guard against uninitialized variable in case no frame is grabbed. */
	clock_gettime(CLOCK_UPTIME, &tp_start);

	if (vid->fps && !vid->nofps) {
		fus = 1000000 / vid->fps;
		timerclear(&frit.it_value);
		timerclear(&frit.it_interval);
		if (vid->fps == 1) {
			frit.it_value.tv_sec = 1;
			frit.it_interval.tv_sec = 1;
		} else {
			frit.it_value.tv_usec = fus;
			frit.it_interval.tv_usec = fus;
		}
		signal(SIGALRM, got_frame);
		if (setitimer(ITIMER_REAL, &frit, NULL) == -1) {
			warn("setitimer");
			return 0;
		}
	}
	signal(SIGHUP, got_shutdown);
	signal(SIGINT, got_shutdown);
	signal(SIGKILL, got_shutdown);
	signal(SIGTERM, got_shutdown);
	signal(SIGPIPE, got_shutdown);

	while (!shutdown) {
		err = 0;
		if (vid->mmap_on) {
			if (!(ret = ioctl_input(vid)))
				return 0;
		} else
			ret = poll_input(vid);
		if (ret == 1) {
			if ((vid->mode & M_IN_DEV) ||
			    frames_grabbed - 1 == frames_played) {
				if (!vid->mmap_on)
					ret = grab_frame(vid);
				if (ret == 1) {
					frames_grabbed++;
					if (vid->nofps)
						play = 1;
				} else if (ret == 255)
					break;
				else if (ret < 0)
					err++;
			}
		} else if (ret < 0) {
			/* continue if interrupted by play or shutdown signal */
			if (!((errno == EINTR || errno == EAGAIN) &&
			    (play || shutdown))) {
				err++;
				warn("poll");
			}
		} else if (ret & (POLLERR | POLLHUP | POLLNVAL)) {
			if (!strcmp(vid->iofile, "-") && (ret & POLLHUP))
				break;
			err++;
			warnx("poll error, revents=0x%x", ret);
		}
		if (err)
			return 0;

		if (vid->mode & M_OUT_XV)
			display_event(vid);
		if (shutdown)
			break;
		if (hold || ((vid->mode & M_IN_FILE) && !play))
			usleep(fus);
		if (frames_grabbed < 1)
			play = 0;
		if (hold || !play)
			continue;
		play = 0;

		src = vid->frame_buffer;
		if (vid->conv_type & CONV_YV12)
			yuy2_to_yv12(src, vid->conv_buffer,
                            vid->width, vid->height,
			    (vid->conv_type & CONV_SWAP));
		else if (vid->conv_type & CONV_SWAP)
			swab(src, vid->conv_buffer, vid->bpf);

		if ((vid->mode & M_OUT_FILE) && wout) {
			done = 0;
			todo = vid->bpf;
			while (todo > 0) {
				ret = write(vid->iofile_fd, src + done, todo);
				if (ret == -1) {
					if (!strcmp(vid->iofile, "-") &&
					    (errno == EPIPE))
						break;
					if (errno != EAGAIN && errno != EINTR) {
						warn("write %s", vid->iofile);
						return 0;
					}
				}
				done += ret;
				todo -= ret;
			}
		}
		if (vid->mode & M_OUT_XV) {
			src = (vid->conv_type > CONV_NONE) ?
			    vid->conv_buffer : vid->frame_buffer;
			x->xv_image->data = src;
			if (x->resized) {
				x->resized = 0;
				XClearWindow(x->dpy, x->window);
			}
			ret = XvPutImage(x->dpy, x->port, x->window, x->gc,
			    x->xv_image, 0, 0, vid->width, vid->height,
			    x->box_x, x->box_y, x->box_w, x->box_h);
			if (ret != Success) {
				warn("XvPutImage");
				return 0;
			}
		}
		if (frames_grabbed > 0)
			frames_played++;

		if (frames_played == 0)
			clock_gettime(CLOCK_UPTIME, &tp_start);

		if (vid->verbose > 1 && frames_played > 0 &&
		    (frames_played) % sequence == 0) {
			clock_gettime(CLOCK_UPTIME, &tp_now);
			timespecsub(&tp_now, &tp_start, &tp_run);
			run_time = tp_run.tv_sec +
			    tp_run.tv_nsec / 1000000000.0;
			fprintf(stderr, "frames: %08ld, seconds: "
			    "%09.2f, fps: %08.5f\r", frames_played,
			    run_time, ((double)frames_played) / run_time);
			fflush(stderr);
		}
	}
	clock_gettime(CLOCK_UPTIME, &tp_now);

	if (vid->fps) {
		timerclear(&frit.it_value);
		timerclear(&frit.it_interval);
		if (setitimer(ITIMER_REAL, &frit, NULL) == -1 &&
		    vid->verbose > 0)
			warn("setitimer");
		signal(SIGALRM, SIG_DFL);
	}
	signal(SIGHUP, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGKILL, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);

	if (vid->verbose > 1)
		fprintf(stderr, "\n");

	if (vid->verbose > 0) {
		timespecsub(&tp_now, &tp_start, &tp_run);
		run_time = tp_run.tv_sec + tp_run.tv_nsec / 1000000000.0;
		fprintf(stderr, "run time: %f seconds\n", run_time);
		fprintf(stderr, "frames grabbed: %ld\n", frames_grabbed);
		fprintf(stderr, "frames played: %ld\n", frames_played + 1);
		fprintf(stderr, "played fps: %f\n", ((double)frames_played) / run_time);
	}

	return 1;
}

__dead void
cleanup(struct video *vid, int excode)
{
	int type;

	if (vid->xdsp.xv_image != NULL)
		XFree(vid->xdsp.xv_image);

	if (vid->xdsp.gc != NULL)
		XFreeGC(vid->xdsp.dpy, vid->xdsp.gc);

	if (vid->xdsp.window != 0)
		XDestroyWindow(vid->xdsp.dpy, vid->xdsp.window);

	if (vid->xdsp.port != 0)
		XvUngrabPort(vid->xdsp.dpy, vid->xdsp.port, CurrentTime);

	if (vid->xdsp.dpy != NULL)
		XCloseDisplay(vid->xdsp.dpy);

	if (vid->dev.fd >= 0) {
		if (vid->mmap_on)
			mmap_stop(vid);
		else
			(void)ioctl(vid->dev.fd, VIDIOC_STREAMOFF, &type);
		close(vid->dev.fd);
	}

	if (vid->iofile_fd >= 0)
		close(vid->iofile_fd);

	if (vid->frame_buffer != NULL)
		free(vid->frame_buffer);

	if (vid->conv_buffer != NULL)
		free(vid->conv_buffer);

	if (vid->sz_str != NULL)
		free(vid->sz_str);

	exit(excode);
}

int
main(int argc, char *argv[])
{
	struct video vid;
	struct dev *d = &vid.dev;
	struct xdsp *x = &vid.xdsp;
	const char *errstr;
	size_t len;
	int ch, errs = 0;

	bzero(&vid, sizeof(struct video));

	snprintf(d->path, sizeof(d->path), "/dev/video");
	x->cur_adap = -1;
	vid.dev.fd = vid.iofile_fd = -1;
	vid.mode = M_IN_DEV | M_OUT_XV;
	vid.enc = -1;
	vid.enc_xv = -1;
	vid.mmap_on = 1; /* mmap method is default */
	wout = 1;

	while ((ch = getopt(argc, argv, "cdgqRva:e:f:i:O:o:r:s:")) != -1) {
		switch (ch) {
		case 'a':
			x->cur_adap = strtonum(optarg, 0, 4, &errstr);
			if (errstr != NULL) {
				warnx("Xv adaptor '%s' is %s", optarg, errstr);
				errs++;
			}
			break;
		case 'c':
			vid.mode |= M_QUERY_CTRLS;
			vid.mode &= ~M_OUT_XV;
			break;
		case 'd':
			vid.mode |= M_RESET;
			vid.mode &= ~M_OUT_XV;
			break;
		case 'e':
			vid.enc = find_enc(optarg);
			if (vid.enc >= ENC_LAST) {
				warnx("encoding '%s' is invalid", optarg);
				errs++;
			}
			break;
		case 'f':
			len = strlcpy(d->path, optarg, sizeof(d->path));
			if (len >= sizeof(d->path)) {
				warnx("file path is too long: %s", optarg);
				errs++;
			}
			break;
		case 'g':
			vid.mmap_on = 0;
			break;
		case 'i':
			if (vid.mode & (M_IN_FILE | M_OUT_FILE)) {
				warnx("only one input or output file allowed");
				errs++;
			} else {
				vid.mode = (vid.mode & ~M_IN_DEV) | M_IN_FILE;
				vid.mmap_on = 0; /* mmap mode does not work for files */
				len = strlcpy(vid.iofile, optarg,
				    sizeof(vid.iofile));
				if (len >= sizeof(vid.iofile)) {
					warnx("input path is too long: %s",
					    optarg);
					errs++;
				}
			}
			break;
		case 'o':
		case 'O':
			if (vid.mode & (M_IN_FILE | M_OUT_FILE)) {
				warnx("only one input or output file allowed");
				errs++;
			} else {
				vid.mode |= M_OUT_FILE;
				if (ch != 'O')
					vid.mode &= ~M_OUT_XV;
				len = strlcpy(vid.iofile, optarg,
				    sizeof(vid.iofile));
				if (len >= sizeof(vid.iofile)) {
					warnx("output path is too long: %s",
					    optarg);
					errs++;
				}
			}
			break;
		case 'q':
			vid.mode |= M_QUERY;
			vid.mode &= ~M_OUT_XV;
			break;
		case 'R':
			vid.nofps = 1;
			break;
		case 'r':
			vid.fps = strtonum(optarg, 1, 100, &errstr);
			if (errstr != NULL) {
				warnx("frame rate '%s' is %s", optarg, errstr);
				errs++;
			}
			break;
		case 's':
			vid.sz_str = strdup(optarg);
			break;
		case 'v':
			vid.verbose++;
			break;
		default:
			errs++;
			break;
		}
		if (errs > 0)
			break;
	}
	if (errs > 0) {
		usage();
		cleanup(&vid, 1);
	}
	argc -= optind;
	argv += optind;

	if (!parse_ctrl(&vid, argc, argv))
		cleanup(&vid, 0);

	if (vid.mode & M_QUERY) {
		if (pledge("stdio rpath wpath video", NULL) == -1)
			err(1, "pledge");
		dev_dump_query(&vid);
		cleanup(&vid, 0);
	}

	if (vid.fps == 0)
		vid.nofps = 1;

	if (!setup(&vid))
		cleanup(&vid, 1);

	if (vid.mode & M_RESET) {
		dev_reset_ctrls(&vid);
		if (!(vid.mode & M_QUERY_CTRLS))
			cleanup(&vid, 0);
	}

	if (vid.mode & M_QUERY_CTRLS) {
		dev_dump_query_ctrls(&vid);
		cleanup(&vid, 0);
	}

	if (vid.mode & M_IN_FILE) {
		if (pledge("stdio rpath", NULL) == -1)
			err(1, "pledge");
	} else {
		if (pledge("stdio rpath video", NULL) == -1)
			err(1, "pledge");
	}

	if (!stream(&vid))
		cleanup(&vid, 1);

	cleanup(&vid, 0);
}
