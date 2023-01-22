/*
 *
Copyright 1989, 1998  The Open Group
Copyright 2009 Open Text Corporation

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.
 *
 * Author:  Jim Fulton, MIT X Consortium
 * Author:  Peter Harris, Open Text Corporation
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

#define ATOMS_PER_BATCH 100 /* This number can be tuned 
				higher for fewer round-trips
				lower for less bandwidth wasted */

static const char *ProgramName;
static const char *DisplayString;

static void do_name ( xcb_connection_t *c, const char *format, char *name );
static int parse_range ( char *range, xcb_atom_t *lowp, xcb_atom_t *highp );
static void do_range ( xcb_connection_t *c, const char *format, char *range );
static void list_atoms ( xcb_connection_t *c, const char *format, int mask,
			 xcb_atom_t low, xcb_atom_t high );

#ifndef __has_attribute
# define __has_attribute(x) 0  /* Compatibility with older compilers. */
#endif

static void
#if __has_attribute(__cold__)
__attribute__((__cold__))
#endif
#if __has_attribute(noreturn)
__attribute__((noreturn))
#endif
usage(const char *errmsg)
{
    if (errmsg != NULL)
	fprintf (stderr, "%s: %s\n\n", ProgramName, errmsg);

    fprintf (stderr, "usage:  %s [-options...]\n\n%s\n", ProgramName,
	     "where options include:\n"
	     "    -display dpy            X server to which to connect\n"
	     "    -format string          printf-style format to use\n"
	     "    -range [num]-[num]      atom values to list\n"
	     "    -name string            name of single atom to print\n"
	     "    -version                print program version\n"
	);
    exit (1);
}

int
main(int argc, char *argv[])
{
    char *displayname = NULL;
    const char *format = "%lu\t%s";
    int didit = 0;
    xcb_connection_t *c = NULL;

    ProgramName = argv[0];

    for (int doit = 0; doit < 2; doit++) {	/* pre-parse to get display */
	for (int i = 1; i < argc; i++) {
	    char *arg = argv[i];

	    if (arg[0] == '-') {
		switch (arg[1]) {
		  case 'd':			/* -display dpy */
		    if (++i >= argc) usage ("-display requires an argument");
		    if (!doit) displayname = argv[i];
		    continue;
		  case 'f':			/* -format string */
		    if (++i >= argc) usage ("-format requires an argument");
		    if (doit) format = argv[i];
		    continue;
		  case 'r':			/* -range num-[num] */
		    if (++i >= argc) usage ("-range requires an argument");
		    if (doit) {
			do_range (c, format, argv[i]);
			didit = 1;
		    }
		    continue;
		  case 'n':			/* -name string */
		    if (++i >= argc) usage ("-name requires an argument");
		    if (doit) {
			do_name (c, format, argv[i]);
			didit = 1;
		    }
		    continue;
		  case 'v':
		    if (strcmp(arg, "-version") == 0) {
			puts(PACKAGE_STRING);
			exit(0);
		    }
		    /* else FALLTHROUGH to unrecognized arg case below */
		}
	    }
	    fprintf (stderr, "%s: unrecognized argument %s\n\n",
		     ProgramName, arg);
	    usage (NULL);
	}
	if (!doit) {
	    DisplayString = displayname;
	    if (!DisplayString)
		DisplayString = getenv("DISPLAY");
	    if (!DisplayString)
		DisplayString = "";
	    c = xcb_connect(displayname, NULL);
	    if (!c || xcb_connection_has_error(c)) {
		fprintf (stderr, "%s:  unable to open display \"%s\"\n",
			 ProgramName, DisplayString);
		exit (1);
	    }
	} else
	    if (!didit)		/* no options, default is list all */
		list_atoms(c, format, 0, 0, 0);
    }

    xcb_disconnect(c);
    exit (0);
}

static void
do_name(xcb_connection_t *c, const char *format, char *name)
{
    xcb_intern_atom_reply_t *a = xcb_intern_atom_reply(c, 
	xcb_intern_atom_unchecked(c, 1, strlen(name), name), NULL);

    if (a && a->atom != XCB_NONE) {
	printf (format, (unsigned long) a->atom, name);
	putchar ('\n');
    } else {
	fprintf (stderr, "%s:  no atom named \"%s\" on server \"%s\"\n",
		 ProgramName, name, DisplayString);
    }

    if (a)
	free(a);
}


#define RangeLow (1 << 0)
#define RangeHigh (1 << 1)

static int
strtoatom(char *s, xcb_atom_t *atom)
{
    long long value;
    char *end;

    value = strtoll(s, &end, 10);
    if (s == end || *end != '\0' || value < 0 || value > UINT32_MAX) {
	return 1;
    }

    *atom = (xcb_atom_t) value;
    return 0;
}

static int 
parse_range(char *range, xcb_atom_t *lowp, xcb_atom_t *highp)
{
    char *dash;
    int mask = 0;

    if (!range) {			/* NULL means default */
	*lowp = 1;
	return RangeLow;
    }

    dash = strchr(range, '-');
    if (!dash) dash = strchr(range, ':');
    if (dash) {
	if (dash == range) {		/* -high */
	    *lowp = 1;
	} else {			/* low-[high] */
	    *dash = '\0';
	    if (strtoatom(range, lowp)) {
		*dash = '-';
		goto invalid;
	    }
	    *dash = '-';
	}
	mask |= RangeLow;
	dash++;
	if (*dash) {			/* [low]-high */
	    if (strtoatom(dash, highp) || *highp < *lowp) {
		goto invalid;
	    }
	    mask |= RangeHigh;
	}
    } else {				/* number (low == high) */
	if (strtoatom(range, lowp)) {
		goto invalid;
	}
	*highp = *lowp;
	mask |= (RangeLow | RangeHigh);
    }

    return mask;
invalid:
    fprintf(stderr, "%s:  invalid range: %s\n", ProgramName, range);
    exit(1);
}

static void
do_range(xcb_connection_t *c, const char *format, char *range)
{
    int mask;
    xcb_atom_t low, high;

    mask = parse_range (range, &low, &high);
    list_atoms (c, format, mask, low, high);
}

static int
say_batch(xcb_connection_t *c, const char *format, xcb_get_atom_name_cookie_t *cookie, xcb_atom_t low, long count, int stop_error)
{
    xcb_generic_error_t *e;
    char atom_name[1024];
    long i;
    int done = 0;

    for (i = 0; i < count; i++)
	cookie[i] = xcb_get_atom_name(c, (xcb_atom_t)i + low);

    for (i = 0; i < count; i++) {
	xcb_get_atom_name_reply_t *r;
	r = xcb_get_atom_name_reply(c, cookie[i], &e);
	if (r) {
	    if (!done || !stop_error) {
		/* We could just use %.*s in 'format', but we want to be compatible
		   with legacy command line usage */
		snprintf(atom_name, sizeof(atom_name), "%.*s",
		    r->name_len, xcb_get_atom_name_name(r));

		printf (format, i + low, atom_name);
		putchar ('\n');
	    }
	    free(r);
	}
	if (e) {
	    done = 1;
	    free(e);
	}
    }

    return done && stop_error;
}

static void
list_atoms(xcb_connection_t *c, const char *format, int mask, xcb_atom_t low, xcb_atom_t high)
{
    xcb_get_atom_name_cookie_t cookie_jar[ATOMS_PER_BATCH];
    int done = 0;

    if ((mask & RangeLow) == 0)
	low = 1;
    if ((mask & RangeHigh) == 0)
	high = UINT32_MAX;

    while (!done) {
	long count = (high - low < ATOMS_PER_BATCH - 1) ?
	    (high - low + 1) : ATOMS_PER_BATCH;
	done = say_batch(c, format, cookie_jar, low, count, (mask & RangeHigh) == 0);
	if (high - low < UINT32_MAX && low == high - count + 1) {
	    done = 1;
	}
	low += count;
    }
}
