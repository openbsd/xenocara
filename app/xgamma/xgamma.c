/*
 * Copyright 1999  The XFree86 Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL 
 * THE XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF 
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 * 
 * Written by David Bateman
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <errno.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/xf86vmode.h>
#include <ctype.h>
#include <stdlib.h>

static char *ProgramName;
static int MajorVersion, MinorVersion;
static int EventBase, ErrorBase;

/* Minimum extension version required */
#define MINMAJOR 2
#define MINMINOR 0

/* Maximum and Minimum gamma values */
#define GAMMA_MIN 0.1
#define GAMMA_MAX 10.0

static void _X_NORETURN
Syntax(const char *errmsg)
{
    if (errmsg != NULL)
        fprintf (stderr, "%s: %s\n\n", ProgramName, errmsg);

    fprintf (stderr, "usage:  %s [-options]\n\n%s", ProgramName,
             "where the available options are:\n"
             "    -display host:dpy       or -d\n"
             "    -quiet                  or -q\n"
             "    -screen                 or -s\n"
             "    -version                or -v\n"
             "    -gamma f.f              Gamma Value\n"
             "    -rgamma f.f             Red Gamma Value\n"
             "    -ggamma f.f             Green Gamma Value\n"
             "    -bgamma f.f             Blue Gamma Value\n\n"
             "If no gamma is specified, returns the current setting\n");
    exit (1);
}


/*
 * The following is a hack until XrmParseCommand is ready.  It determines
 * whether or not the given string is an abbreviation of the arg.
 */

static Bool 
isabbreviation(const char *arg, const char *s, size_t minslen)
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

int
main(int argc, char *argv[])
{
    int i, ret;
    char *displayname = NULL;
    Display *dpy;
    float gam = -1., rgam = -1., ggam = -1., bgam = -1.;
    XF86VidModeGamma gamma;
    Bool quiet = False;
    int screen = -1;

    ProgramName = argv[0];
    for (i = 1; i < argc; i++) {
	char *arg = argv[i];

	if (arg[0] == '-') {
	    if (isabbreviation ("-display", arg, 1)) {
		if (++i >= argc) Syntax ("-display requires an argument");
		displayname = argv[i];
		continue;
	    } else if (isabbreviation ("-quiet", arg, 1)) {
		quiet = True;
		continue;
	    } else if (isabbreviation ("-version", arg, 1)) {
		puts(PACKAGE_STRING);
		exit(0);
	    } else if (isabbreviation ("-screen", arg, 1)) {
		if (++i >= argc) Syntax ("-screen requires an argument");
		screen = atoi(argv[i]);
		continue;
	    } else if (isabbreviation ("-gamma", arg, 2)) {
		if (++i >= argc) Syntax ("-gamma requires an argument");
		if ((rgam >= 0.) || (ggam >= 0.) || (bgam >= 0.))
		    Syntax ("-gamma cannot be used with -rgamma, -ggamma, or -bgamma");
		gam = (float)atof(argv[i]);
		if ((gam < GAMMA_MIN) || (gam > GAMMA_MAX)) {
		    fprintf(stderr,
			    "Gamma values must be between %6.3f and %6.3f\n",
			    GAMMA_MIN, GAMMA_MAX);
		    exit(1);
		}
		continue;
	    } else if (isabbreviation ("-rgamma", arg, 2)) {
		if (++i >= argc) Syntax ("-rgamma requires an argument");
		if (gam >= 0.) Syntax ("cannot set both -gamma and -rgamma");
		rgam = (float)atof(argv[i]);
		if ((rgam < GAMMA_MIN) || (rgam > GAMMA_MAX)) {
		    fprintf(stderr,
			    "Gamma values must be between %6.3f and %6.3f\n",
			    GAMMA_MIN, GAMMA_MAX);
		    exit(1);
		}
		continue;
	    } else if (isabbreviation ("-ggamma", arg, 2)) {
		if (++i >= argc) Syntax ("-ggamma requires an argument");
		if (gam >= 0.) Syntax ("cannot set both -gamma and -ggamma");
		ggam = (float)atof(argv[i]);
		if ((ggam < GAMMA_MIN) || (ggam > GAMMA_MAX)) {
		    fprintf(stderr,
			    "Gamma values must be between %6.3f and %6.3f\n",
			    GAMMA_MIN, GAMMA_MAX);
		    exit(1);
		}
		continue;
	    } else if (isabbreviation ("-bgamma", arg, 2)) {
		if (++i >= argc) Syntax ("-bgamma requires an argument");
		if (gam >= 0.) Syntax ("cannot set both -gamma and -bgamma");
		bgam = (float)atof(argv[i]);
		if ((bgam < GAMMA_MIN) || (bgam > GAMMA_MAX)) {
		    fprintf(stderr,
			    "Gamma values must be between %6.3f and %6.3f\n",
			    GAMMA_MIN, GAMMA_MAX);
		    exit(1);
		}
		continue;
	    } else {
		if (!isabbreviation ("-help", arg, 1))
		    fprintf (stderr, "%s: unrecognized argument %s\n\n",
			     ProgramName, arg);
		Syntax (NULL);
	    }
	} else {
	    fprintf (stderr, "%s: unrecognized argument %s\n\n",
		     ProgramName, arg);
	    Syntax (NULL);
	}
    }

    if ((dpy = XOpenDisplay(displayname)) == NULL) {
	fprintf (stderr, "%s:  unable to open display '%s'\n",
		 ProgramName, XDisplayName (displayname));
	exit(1);
    } else if (screen == -1)
	screen = DefaultScreen(dpy);

    if (!XF86VidModeQueryVersion(dpy, &MajorVersion, &MinorVersion)) {
	fprintf(stderr, "Unable to query video extension version\n");
	exit(2);
    }

    if (!XF86VidModeQueryExtension(dpy, &EventBase, &ErrorBase)) {
	fprintf(stderr, "Unable to query video extension information\n");
	exit(2);
    }

    /* Fail if the extension version in the server is too old */
    if (MajorVersion < MINMAJOR || 
	(MajorVersion == MINMAJOR && MinorVersion < MINMINOR)) {
	fprintf(stderr,
		"Xserver is running an old XFree86-VidModeExtension version"
		" (%d.%d)\n", MajorVersion, MinorVersion);
	fprintf(stderr, "Minimum required version is %d.%d\n",
		MINMAJOR, MINMINOR);
	exit(2);
    }

    if (!XF86VidModeGetGamma(dpy, screen, &gamma)) {
	fprintf(stderr, "Unable to query gamma correction\n");
	XCloseDisplay (dpy);
	exit (2);
    } else if (!quiet)
	fprintf(stderr, "-> Red %6.3f, Green %6.3f, Blue %6.3f\n", gamma.red,
		gamma.green, gamma.blue);

    ret = 0;
    if (gam >= 0.) {
	gamma.red = gam;
	gamma.green = gam;
	gamma.blue = gam;
	if (!XF86VidModeSetGamma(dpy, screen, &gamma)) {
	    fprintf(stderr, "Unable to set gamma correction\n");
	    ret = 2;
	} else {
	    if (!XF86VidModeGetGamma(dpy, screen, &gamma)) {
		fprintf(stderr, "Unable to query gamma correction\n");
		ret = 2;
	    } else if (!quiet)
		fprintf(stderr, "<- Red %6.3f, Green %6.3f, Blue %6.3f\n",
		        gamma.red, gamma.green, gamma.blue);
	}
    } else if ((rgam >= 0.) || (ggam >= 0.) || (bgam >= 0.)) {
	if (rgam >= 0.) gamma.red = rgam;
	if (ggam >= 0.) gamma.green = ggam;
	if (bgam >= 0.) gamma.blue = bgam;
	if (!XF86VidModeSetGamma(dpy, screen, &gamma)) {
	    fprintf(stderr, "Unable to set gamma correction\n");
	    ret = 2;
	} else {
	    if (!XF86VidModeGetGamma(dpy, screen, &gamma)) {
		fprintf(stderr, "Unable to query gamma correction\n");
		ret = 2;
	    } else if (!quiet)
		fprintf(stderr, "<- Red %6.3f, Green %6.3f, Blue %6.3f\n",
		        gamma.red, gamma.green, gamma.blue);
	}
    }

    XCloseDisplay (dpy);
    exit (ret);
}

