/*	$OpenBSD: xidle.c,v 1.10 2021/10/25 09:30:33 matthieu Exp $	*/
/*
 * Copyright (c) 2005 Federico G. Schwindt
 * Copyright (c) 2005 Claudio Castiglia
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE OPENBSD PROJECT AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OPENBSD
 * PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/extensions/scrnsaver.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <paths.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef CLASS_NAME
#define CLASS_NAME	"XIdle"
#endif

#ifndef PATH_PROG
#define PATH_PROG	"/usr/X11R6/bin/xlock"
#endif


enum {
	north = 0x01,
	south = 0x02,
	east  = 0x04,
	west  = 0x08,
	none  = 0x10,
};

enum { XIDLE_LOCK = 1, XIDLE_DIE = 2 };

struct xinfo {
	Display		*dpy;
	Window		 win;
	int		 coord_x;
	int		 coord_y;

	int		 saver_event;	/* Only if Xss ext is available */

	int		 saved_timeout;
	int		 saved_interval;
	int		 saved_pref_blank;
	int		 saved_allow_exp;
};

struct	xinfo x;

static XrmOptionDescRec fopts[] = {
	{ "-area",	".area",	XrmoptionSepArg,	(caddr_t)NULL },
	{ "-delay",	".delay",	XrmoptionSepArg,	(caddr_t)NULL },
	{ "-display",	".display",	XrmoptionSepArg,	(caddr_t)NULL },
	{ "-program",	".program",	XrmoptionSepArg,	(caddr_t)NULL },
	{ "-timeout",	".timeout",	XrmoptionSepArg,	(caddr_t)NULL },

	{ "-no",	".position",	XrmoptionNoArg,		(caddr_t)"no" },
	{ "-ne",	".position",	XrmoptionNoArg,		(caddr_t)"ne" },
	{ "-nw",	".position", 	XrmoptionNoArg,		(caddr_t)"nw" },
	{ "-se",	".position",	XrmoptionNoArg,		(caddr_t)"se" },
	{ "-sw",	".position",	XrmoptionNoArg,		(caddr_t)"sw" }
};

extern char *__progname;

void	action(struct xinfo *, char **);
void	close_x(struct xinfo *);
Bool	getres(XrmValue *, const XrmDatabase, const char *, const char *);
void    init_x(struct xinfo *, int, int, int);
void	handler(int);
void	parse_opts(int, char **, Display **, int *, int *, int *, int *,
	    char **);
int	str2pos(const char *);
__dead void	usage(void);


__dead void
usage()
{
	fprintf(stderr, "Usage:\n%s %s\n", __progname,
	    "[-area pixels] [-delay secs] [-display host:dpy] "
	    "[-no | -ne | -nw | -se | -sw]\n      [-program path] [-timeout secs]");
	exit(1);
}


void
init_x(struct xinfo *xi, int position, int area, int timeout)
{
	XSetWindowAttributes attr;
	Display *dpy = xi->dpy;
	int error, event;
	int screen;

	screen = DefaultScreen(dpy);

	if (position & south)
		xi->coord_y = DisplayHeight(dpy, screen) - area;
	if (position & east)
		xi->coord_x = DisplayWidth(dpy, screen) - area;

	attr.override_redirect = True;
	xi->win = XCreateWindow(dpy, DefaultRootWindow(dpy),
	    xi->coord_x, xi->coord_y, area, area, 0, 0, InputOnly,
	    CopyFromParent, CWOverrideRedirect, &attr);

	if (position != none) {
		XSelectInput(dpy, xi->win, EnterWindowMask|StructureNotifyMask
#if 0
			       |VisibilityChangeMask
#endif
		);
		XMapWindow(dpy, xi->win);
	}

	/*
	 * AFAICT, we need the event number for ScreenSaverNotify
	 * _always_ since it's the only way to distinguish whether
	 * we've been obscured by an external locking program or
	 * by another window and react according.
	 */
	if (XScreenSaverQueryExtension(dpy, &event, &error) == True) {
		xi->saver_event = event;

		XScreenSaverSelectInput(dpy, DefaultRootWindow(dpy),
		    ScreenSaverNotifyMask);
	} else
		warnx("XScreenSaver extension not available.%s",
		    timeout > 0 ? " Timeout disabled." : "");

	if (timeout > 0 && xi->saver_event) {
		XGetScreenSaver(dpy, &xi->saved_timeout, &xi->saved_interval,
		    &xi->saved_pref_blank, &xi->saved_allow_exp);

		XSetScreenSaver(dpy, timeout, 0, DontPreferBlanking,
		    DontAllowExposures);
	}
}


void
close_x(struct xinfo *xi)
{
	XSetScreenSaver(xi->dpy, xi->saved_timeout, xi->saved_interval,
	    xi->saved_pref_blank, xi->saved_allow_exp);
	XDestroyWindow(xi->dpy, xi->win);
	XCloseDisplay(xi->dpy);
}


void
action(struct xinfo *xi, char **args)
{
	int dumb;

	switch (fork()) {
	case -1:
		err(1, "fork");
	case 0:
		setsid();
		execv(*args, args);
		exit(1);
	default:
		wait(&dumb);
		XSync(xi->dpy, True);
		break;
	}
}


void
handler(int sig)
{
	XClientMessageEvent ev;

	ev.type = ClientMessage;
	ev.display = x.dpy;
	ev.window = x.win;
	if (sig == SIGUSR1)
		ev.message_type = XIDLE_LOCK;
	else 
		ev.message_type = XIDLE_DIE;
	ev.format = 8;
	XSendEvent(x.dpy, x.win, False, 0L, (XEvent *)&ev);
	XFlush(x.dpy);
}


int
str2pos(const char *src)
{
	static struct {
		char	*str;
		int	 pos;
	} s2p[] = {
		{ "ne", north|east },
		{ "nw", north|west },
		{ "se", south|east },
		{ "sw", south|west },
		{ "no", none       },
		{ NULL, 0	   }
	}, *s;

	for (s = s2p; s->str != NULL; s++)
		if (!strcmp(src, s->str))
			break;
	return (s->pos);
}


Bool
getres(XrmValue *value, const XrmDatabase rdb, const char *rname,
    const char *cname)
{
	char fullres[PATH_MAX], fullclass[PATH_MAX], *type;

	snprintf(fullres, sizeof(fullres), "%s.%s", __progname, rname);
	snprintf(fullclass, sizeof(fullclass), "%s.%s", CLASS_NAME, cname);
	return (XrmGetResource(rdb, fullres, fullclass, &type, value));
}


void
parse_opts(int argc, char **argv, Display **dpy, int *area, int *delay,
    int *timeout, int *position, char **args)
{
	const char *errstr;
	char **ap, *program = PATH_PROG;
	char *display, *p;
	XrmDatabase tdb, rdb = NULL;
	XrmValue value;

	XrmInitialize();

	/* Get command line values. */
	XrmParseCommand(&rdb, fopts, sizeof(fopts) / sizeof(fopts[0]),
	    __progname, &argc, argv);
	if (argc > 1)
		usage();

	display = (getres(&value, rdb, "display", "Display") == True) ?
	    (char *)value.addr : NULL;

	*dpy = XOpenDisplay(display);
	if (!*dpy) {
		errx(1, "Unable to open display %s", XDisplayName(display));
	}

	/* Get server resources database. */
	p = XResourceManagerString(*dpy);
	if (!p) {
		/* Get screen resources database. */
		p = XScreenResourceString(ScreenOfDisplay(*dpy,
		    DefaultScreen(*dpy)));
	}

	if (p) {
		tdb = XrmGetStringDatabase(p);
		XrmMergeDatabases(tdb, &rdb);
	}

	if (getres(&value, rdb, "area", "Area")) {
		*area = strtonum(value.addr, 1, INT_MAX, &errstr);
		if (errstr)
			errx(1, "area is %s: %s", errstr, value.addr);
	}
	if (getres(&value, rdb, "delay", "Delay")) {
		*delay = strtonum(value.addr, 0, INT_MAX, &errstr);
		if (errstr)
			errx(1, "delay is %s: %s", errstr, value.addr);
	}
	if (getres(&value, rdb, "position", "Position")) {
		*position = str2pos(value.addr);
		if (!*position)
			errx(1, "position is invalid: %s", value.addr);
	}
	if (getres(&value, rdb, "timeout", "Timeout")) {
		*timeout = strtonum(value.addr, 0, INT_MAX, &errstr);
		if (errstr)
			errx(1, "timeout is %s: %s", errstr, value.addr);
	}
	if (getres(&value, rdb, "program", "Program")) {
		/* Should be the last :) */
		program = (char *)value.addr;
	}

	for (ap = args; ap < &args[9] &&
	    (*ap = strsep(&program, " ")) != NULL;) {
		if (**ap != '\0')
			ap++;
	}
	*ap = NULL;
}


int
main(int argc, char **argv)
{
	char *args[10];
	int area = 2, delay = 2, timeout = 0;
	int position = north|west;
	int fd;
	u_long last_serial = 0;

	parse_opts(argc, argv, &x.dpy, &area, &delay, &timeout,
	    &position, args);

#ifdef DEBUG
	printf("Area: %d\nDelay: %d\nPosition: %d\nTimeout: %d\n"
	    "Program: \"%s\"\n",
	    area, delay, position, timeout, args[0]);
#endif

	init_x(&x, position, area, timeout);

	signal(SIGINT, handler);
	signal(SIGTERM, handler);
	signal(SIGUSR1, handler);

	fd = open(_PATH_DEVNULL, O_RDWR);
	if (fd < 0)
		err(1, _PATH_DEVNULL);
	dup2(fd, STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	if (fd > 2)
		close(fd);

	if (pledge("stdio proc exec", NULL) == -1)
		err(1, "pledge");

	for (;;) {
		XEvent ev;
		u_long mask;

		XNextEvent(x.dpy, &ev);

#ifdef DEBUG
		printf("got event %d\n", ev.type);
#endif

		switch (ev.type) {
		case VisibilityNotify:
			/*
			 * If we got here and the current serial matches
			 * the one from saver_event, we're being obscured
			 * by a locking program. Disable further
			 * screen saver events and raises.
			 */
			if (ev.xvisibility.serial == last_serial)
				mask = 0;
			else
				mask = ScreenSaverNotifyMask;

			XScreenSaverSelectInput(x.dpy,
			    DefaultRootWindow(x.dpy), mask);

			if (mask)
				XMapRaised(x.dpy, x.win);

			XSync(x.dpy, True);
			break;

		case MapNotify:
			XMapRaised(x.dpy, x.win);
			break;

		case ClientMessage:
			if (ev.xclient.message_type == XIDLE_DIE) {
				close_x(&x);
				exit(0);
			} else if (ev.xclient.message_type == XIDLE_LOCK)
				action(&x, args);
			break;

		case EnterNotify:
			sleep(delay);

			XQueryPointer(x.dpy, x.win, &ev.xcrossing.root,
			    &ev.xcrossing.window,
			    &ev.xcrossing.x_root, &ev.xcrossing.y_root,
			    &ev.xcrossing.x, &ev.xcrossing.y,
			    &ev.xcrossing.state);

			/* Check it was for real. */
			if (ev.xcrossing.x_root < x.coord_x ||
			    ev.xcrossing.y_root < x.coord_y ||
			    ev.xcrossing.x_root > x.coord_x + area ||
			    ev.xcrossing.y_root > x.coord_y + area)
				break;
			action(&x, args);
			break;

		default:
			if (ev.type == x.saver_event) {
				XScreenSaverNotifyEvent *se =
				    (XScreenSaverNotifyEvent *)&ev;

				/* Take note of the serial for this event. */
				last_serial = se->serial;

				/*
				 * Was for real or due to terminal
				 * switching or a locking program?
				 */
				if (timeout > 0 && se->forced == False)
					action(&x, args);
			}
			break;
		}
	}
}
