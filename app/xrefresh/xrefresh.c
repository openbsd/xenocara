/***********************************************************

Copyright 1987, 1988, 1998  The Open Group

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


Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/*
 * Kitchen sink version, useful for clearing small areas and flashing the 
 * screen.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <errno.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>

#ifndef HAVE_STRCASECMP
# include <ctype.h>
#endif

static Window win;

static char *ProgramName;

static void _X_NORETURN _X_COLD
Syntax(int exitval)
{
    FILE *out = (exitval == EXIT_SUCCESS) ? stdout : stderr;

    fprintf (out, "usage:  %s [-options] [geometry] [display]\n\n%s",
	     ProgramName,
	     "where the available options are:\n"
	     "    -display host:dpy       or -d\n"
	     "    -geometry WxH+X+Y       or -g spec\n"
	     "    -black                  use BlackPixel\n"
	     "    -white                  use WhitePixel\n"
	     "    -solid colorname        use the color indicated\n"
	     "    -root                   use the root background\n"
	     "    -none                   no background in window\n"
	     "    -delay ms               time to hold refresh\n"
	     "    -version                print program version\n"
	);
    fprintf(out, "\nThe default is:  %s -none -delay 0\n\n", ProgramName);
    exit (exitval);
}

static void _X_NORETURN _X_COLD
missing_arg(const char *arg)
{
    fprintf (stderr, "%s: %s requires an argument\n\n", ProgramName, arg);
    Syntax (EXIT_FAILURE);
}

static void _X_NORETURN _X_COLD
unknown_arg(const char *arg)
{
    fprintf (stderr, "%s: unrecognized argument %s\n\n", ProgramName, arg);
    Syntax (EXIT_FAILURE);
}

/*
 * The following parses options that should be yes or no; it returns -1, 0, 1
 * for error, no, yes.
 */

static int 
parse_boolean_option(char *option)
{
    static const struct _booltable {
        const char *name;
        int value;
    } booltable[] = {
        { "off", 0 }, { "n", 0 }, { "no", 0 }, { "false", 0 },
        { "on", 1 }, { "y", 1 }, { "yes", 1 }, { "true", 1 },
        { NULL, -1 }};
    register const struct _booltable *t;

#ifndef HAVE_STRCASECMP
    register char *cp;

    for (cp = option; *cp; cp++) {
        if (isascii (*cp) && isupper (*cp)) *cp = tolower (*cp);
    }
#endif

    for (t = booltable; t->name; t++) {
#ifdef HAVE_STRCASECMP
        if (strcasecmp (option, t->name) == 0) return (t->value);
#else
        if (strcmp (option, t->name) == 0) return (t->value);
#endif
    }
    return (-1);
}


/*
 * The following is a hack until XrmParseCommand is ready.  It determines
 * whether or not the given string is an abbreviation of the arg.
 */

static Bool 
isabbreviation(const char *arg, char *s, size_t minslen)
{
    size_t arglen;
    size_t slen;

    /* exact match */
    if (strcmp (arg, s) == 0) return (True);

    arglen = strlen (arg);
    slen = strlen (s);

    /* too long or too short */
    if (slen >= arglen || slen < minslen) return (False);

    /* abbreviation */
    if (strncmp (arg, s, slen) == 0) return (True);

    /* bad */
    return (False);
}


enum e_action {doDefault, doBlack, doWhite, doSolid, doNone, doRoot};

static const struct s_pair {
	const char *resource_name;
	enum e_action action;
} pair_table[] = {
	{ "Black", doBlack },
	{ "White", doWhite },
	{ "None", doNone },
	{ "Root", doRoot },
	{ NULL, doDefault }};

int
main(int argc, char *argv[])
{
    Visual visual;
    XSetWindowAttributes xswa;
    int i;
    char *displayname = NULL;
    Display *dpy;
    Colormap cmap;
    enum e_action action = doDefault;
    unsigned long mask;
    int screen;
    int x, y, width, height;
    unsigned long delay = 0;
    char *geom = NULL;
    int geom_result;
    int display_width, display_height;
    char *solidcolor = NULL;
    XColor cdef;
    struct timespec tim;

    ProgramName = argv[0];

    for (i = 1; i < argc; i++) {
	char *arg = argv[i];

	if (arg[0] == '-') {
	    if (isabbreviation ("-display", arg, 2)) {
		if (++i >= argc) missing_arg (arg);
		displayname = argv[i];
		continue;
	    } else if (isabbreviation ("-geometry", arg, 2)) {
		if (++i >= argc) missing_arg (arg);
		geom = argv[i];
		continue;
	    } else if (isabbreviation ("-black", arg, 2)) {
		action = doBlack;
		continue;
	    } else if (isabbreviation ("-white", arg, 2)) {
		action = doWhite;
		continue;
	    } else if (isabbreviation ("-solid", arg, 2)) {
		if (++i >= argc) missing_arg (arg);
		solidcolor = argv[i];
		action = doSolid;
		continue;
	    } else if (isabbreviation ("-none", arg, 2)) {
		action = doNone;
		continue;
	    } else if (isabbreviation ("-root", arg, 2)) {
		action = doRoot;
		continue;
	    } else if (isabbreviation("-delay", arg, 2)) {
		if (++i >= argc) missing_arg(arg);
		delay = ((unsigned long)atol(argv[i])) * 1000000L;
		continue;
	    }
	    else if (isabbreviation ("-version", arg, 1)) {
		puts(PACKAGE_STRING);
		exit(EXIT_SUCCESS);
            }
	    else if (isabbreviation ("-help", arg, 1)) {
		Syntax(EXIT_SUCCESS);
	    } else 
		unknown_arg (arg);
	} else if (arg[0] == '=')			/* obsolete */
	    geom = arg;
	else 
	    unknown_arg (arg);
    }

    if ((dpy = XOpenDisplay(displayname)) == NULL) {
	fprintf (stderr, "%s:  unable to open display '%s'\n",
		 ProgramName, XDisplayName (displayname));
	exit (1);
    }

    if (action == doDefault) {
	char *def;

	if ((def = XGetDefault (dpy, ProgramName, "Solid")) != NULL) {
	    solidcolor = strdup (def);
	    if (solidcolor == NULL) {
		fprintf (stderr,
			 "%s:  unable to allocate memory for string.\n",
			 ProgramName);
		exit (1);
	    }
	    action = doSolid;
	} else {
	    const struct s_pair *pp;

	    for (pp = pair_table; pp->resource_name != NULL; pp++) {
		def = XGetDefault (dpy, ProgramName, pp->resource_name);
		if (def && parse_boolean_option (def) == 1) {
		    action = pp->action;
		}
	    }
	}
    }

    if (geom == NULL) geom = XGetDefault (dpy, ProgramName, "Geometry");

    screen = DefaultScreen (dpy);
    display_width = DisplayWidth (dpy, screen);
    display_height = DisplayHeight (dpy, screen);
    x = y = 0; 
    width = display_width;
    height = display_height;

    if (DisplayCells (dpy, screen) <= 2 && action == doSolid) {
	if (strcmp (solidcolor, "black") == 0)
	    action = doBlack;
	else if (strcmp (solidcolor, "white") == 0) 
	    action = doWhite;
	else {
	    fprintf (stderr, 
	    	     "%s:  can't use colors on a monochrome display.\n",
		     ProgramName);
	    action = doNone;
	}
    }

    if (geom) 
        geom_result = XParseGeometry (geom, &x, &y,
				      (unsigned int *)&width,
				      (unsigned int *)&height);
    else
	geom_result = NoValue;

    /*
     * For parsing geometry, we want to have the following
     *     
     *     =                (0,0) for (display_width,display_height)
     *     =WxH+X+Y         (X,Y) for (W,H)
     *     =WxH-X-Y         (display_width-W-X,display_height-H-Y) for (W,H)
     *     =+X+Y            (X,Y) for (display_width-X,display_height-Y)
     *     =WxH             (0,0) for (W,H)
     *     =-X-Y            (0,0) for (display_width-X,display_height-Y)
     *
     * If we let any missing values be taken from (0,0) for 
     * (display_width,display_height) we just have to deal with the
     * negative offsets.
     */

    if (geom_result & XNegative) {
	if (geom_result & WidthValue) {
	    x = display_width - width + x;
	} else {
	    width = display_width + x;
	    x = 0;
	}
    } 
    if (geom_result & YNegative) {
	if (geom_result & HeightValue) {
	    y = display_height - height + y;
	} else {
	    height = display_height + y;
	    y = 0;
	}
    }

    mask = 0;
    switch (action) {
	case doBlack:
	    xswa.background_pixel = BlackPixel (dpy, screen);
	    mask |= CWBackPixel;
	    break;
	case doWhite:
	    xswa.background_pixel = WhitePixel (dpy, screen);
	    mask |= CWBackPixel;
	    break;
	case doSolid:
	    cmap = DefaultColormap (dpy, screen);
	    if (XParseColor (dpy, cmap, solidcolor, &cdef) &&
		XAllocColor (dpy, cmap, &cdef)) {
		xswa.background_pixel = cdef.pixel;
		mask |= CWBackPixel;
	    } else {
		fprintf (stderr,"%s:  unable to allocate color '%s'.\n",
			 ProgramName, solidcolor);
		xswa.background_pixmap = None;
		mask |= CWBackPixmap;
	    }
	    break;
	case doDefault:
	case doNone:
	    xswa.background_pixmap = None;
	    mask |= CWBackPixmap;
	    break;
	case doRoot:
	    xswa.background_pixmap = ParentRelative;
	    mask |= CWBackPixmap;
	    break;
    }
    xswa.override_redirect = True;
    xswa.backing_store = NotUseful;
    xswa.save_under = False;
    mask |= (CWOverrideRedirect | CWBackingStore | CWSaveUnder);
    visual.visualid = CopyFromParent;
    win = XCreateWindow(dpy, DefaultRootWindow(dpy), x, y, width, height,
	    0, DefaultDepth(dpy, screen), InputOutput, &visual, mask, &xswa);

    /*
     * at some point, we really ought to go walk the tree and turn off 
     * backing store;  or do a ClearArea generating exposures on all windows
     */
    XMapWindow (dpy, win);
    /* flushing, because sometimes window will never show (especially for
	 * exceptionally short delays) */
    XFlush(dpy);

    /* pause before returning screen */
    tim.tv_sec = delay / 1000000000L;
    tim.tv_nsec = delay % 1000000000L;
    nanosleep(&tim , NULL);

    /* the following will free the color that we might have allocated */
    XCloseDisplay (dpy);
    exit (0);
}

