/*	$OpenBSD: xvctl.c,v 1.3 2013/08/26 19:25:10 espie Exp $	*/
/*
 * Copyright (c) 2001 Marc Espie.
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
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xvlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <ctype.h>

void
usage()
{
	fprintf(stderr,  "usage:\t%s\n\t%s\n\t%s\n",
	    "xvctl [-n] [-d display] [-s screen] [-A adaptor] variable ...",
	    "xvctl [-n] [-d display] [-s screen] [-A adaptor] variable=value ...",
	    "xvctl [-n] [-d display] [-s screen] [-A adaptor] -a");
	exit(1);
}

char *
reformat(char *name)
{
	static char result[BUFSIZ];
	char *s;
	char *t;

	if (strlcpy(result, name, sizeof result) >= sizeof result)
		warnx("Name %s was truncated", name);
	s = result;
	if (strncmp(result, "XV_", 3) == 0)
		s+=3;
	for (t = s; *t; t++)
		*t = tolower(*t);
    	return s;
}

XvAttribute *
findname(XvAttribute table[], int nattr, const char *name)
{
	int i;

	if (strncasecmp(name, "XV_", 3) == 0)
		name +=3; 
	for (i = 0; i < nattr; i++) {
		if (strcasecmp(table[i].name+3, name) == 0)
			return table+i;
	}
	warnx("No such attribute %s", name);
	return NULL;
}

void
printattribute(Display *dpy, XvPortID id, int nflag, XvAttribute *a)
{
	Atom atom;
	int v;

	atom = XInternAtom(dpy, a->name, True);
	if (atom != None)
		if (Success == XvGetPortAttribute(dpy, id, atom, &v)) {
			if (!nflag)
				printf("%s=", reformat(a->name));
			printf("%d\n", v);
		}
}

void
showall(Display *dpy, XvPortID id, int nflag, XvAttribute table[], int nattr)
{
	int i;

	for (i = 0; i < nattr; i++)
		if (table[i].flags & XvGettable)
			printattribute(dpy, id, nflag, table+i);
}

void
setvar(Display *dpy, XvPortID id, int nflag, XvAttribute table[], int nattr,
    const char *name, int v)
{
	XvAttribute *at;
	Atom atom;

	at = findname(table, nattr, name);
	if (!at) 
		return;
	if (!(at->flags & XvSettable)) {
		warnx("Can't set %s", name);
		return;
	}
	if ((at->max_value != -1 && v > at->max_value)  || v < at->min_value) {
		warnx("Value %d for %s out of range (%d-%d)", v, name,
		    at->min_value, at->max_value);
		return;
	}
	atom = XInternAtom(dpy, at->name, True);
	if (atom != None)
		if (Success == XvSetPortAttribute(dpy, id, atom, v)) {
			if (!nflag)
				printattribute(dpy, id, nflag, at);
		}
}

void
showvar(Display *dpy, XvPortID id, int nflag, XvAttribute table[], int nattr,
    const char *name)
{
	XvAttribute *at;

	at = findname(table, nattr, name);
	if (!at)
		return;
    	if (!(at->flags & XvGettable)) {
		warnx("Can't get %s", name);
		return;
	}
	printattribute(dpy, id, nflag, at);
}

void
parse(Display *dpy, XvPortID id, int nflag, XvAttribute table[],
    int nattr, const char *s)
{
	char buf[BUFSIZ];
	char *cp;

	if (strlcpy(buf, s, sizeof buf) >= sizeof(buf))
		warnx("Buffer too small for %s", s);
	cp = strchr(buf, '=');
	if (cp != NULL) {
		long v;
		char *end;
		char *tail;

		tail = cp;
		while (tail > buf && isspace(tail[-1]))
			tail--;
		*tail = 0;
		do {
			cp++;
		} while (isspace(*cp));
		v = strtol(cp, &end, 0);
		if (*end != 0) {
			warnx("Bogus value: %s", cp);
			return;
		}
		setvar(dpy, id, nflag, table, nattr, buf, v);
	} else
		showvar(dpy, id, nflag, table, nattr, buf);
}

char *
displayname(char *display)
{
	return display ? display : XDisplayName(display);
}

int
main(int argc, char *argv[])
{
	int ch;
	int aflag = 0;
	int nflag = 0;
	char *display = NULL;
	int screen = -1;
	int nscreens;
	int nattr;
	unsigned int nadaptors;
	XvAdaptorInfo *ainfo;
	XvAttribute *table;
	int adaptor = 0;
	Display *dpy;
	unsigned int ver, rev, eventB, reqB, errorB;

	while ((ch = getopt(argc, argv, "ad:ns:A:w")) != -1) {
		switch(ch) {
		case 'a':
			aflag = 1;
			break;
		case 'd':
			display = optarg;
			break;
		case 's':
			screen = atoi(optarg);
			break;
		case 'A':
			adaptor = atoi(optarg);
			break;
		case 'n':
			nflag = 1;
			break;
		case 'w':
			/* flag no longer needed */
			break;
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	dpy = XOpenDisplay(display);
	if (!dpy)
		errx(1, "Unable to open display %s", displayname(display));
	if (Success != XvQueryExtension(dpy, &ver, &rev, &reqB, &eventB,
	    &errorB)) 
	    	errx(1, "No X-Video extension on %s", displayname(display));

	if (!nflag)
		printf("# X-Video extension version %i.%i on %s\n", ver, rev,
		    displayname(display));

	nscreens = ScreenCount(dpy);
	if (screen == -1) {
		for (screen = 0; screen < nscreens; screen++) {
			XvQueryAdaptors(dpy, RootWindow(dpy, screen),
			    &nadaptors, &ainfo);
			if (nadaptors)
				break;
		}
	}
	if (screen >= nscreens)
		errx(1, "Not a screen with adaptors");

	if (adaptor > nadaptors)
		errx(1, "No such adaptor");

	table = XvQueryPortAttributes(dpy, ainfo[adaptor].base_id, &nattr);


	if (argc == 0) {
		showall(dpy, ainfo[adaptor].base_id, nflag, table, nattr);
	}
	for (; *argv != NULL; ++argv)
		parse(dpy, ainfo[adaptor].base_id, nflag, table,
		    nattr, *argv);
	XFree(table);
	exit(0);
}
