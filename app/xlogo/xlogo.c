/*

Copyright 1989, 1998  The Open Group

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

 */

/*
 * Copyright (c) 2023, 2024, Oracle and/or its affiliates.
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xfuncproto.h>
#include "xlogo.h"
#include "Logo.h"
#include <X11/Xaw/Cardinals.h>
#ifdef XKB
#include <X11/extensions/XKBbells.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef HAVE_STRLCAT
#define strlcat(dst, src, len) do {              \
    strncat(dst, src, len - strlen(dst) - 1);    \
    dst[len] = '\0';                             \
} while(0)
#endif
#ifndef HAVE_STRLCPY
#define strlcpy(dst, src, len) do {              \
    strncpy(dst, src, len - 1);                  \
    dst[len] = '\0';                             \
} while(0)
#endif

/* Global vars*/
const char *ProgramName;    /* program name (from argv[0]) */

static void quit(Widget w,  XEvent *event, String *params, Cardinal *num_params);

static XrmOptionDescRec options[] = {
{ "-shape", "*shapeWindow", XrmoptionNoArg, (XPointer) "on" },
#ifdef XRENDER
{"-render", "*render",XrmoptionNoArg, "TRUE"},
{"-sharp", "*sharp", XrmoptionNoArg, "TRUE"},
#endif
{"-v",         "Verbose",     XrmoptionNoArg,  "TRUE"},
{"-q",         "Quiet",       XrmoptionNoArg,  "TRUE"},
};

static XtActionsRec actions[] = {
    {"quit",	quit },
};

static Atom wm_delete_window;

/* See xlogo.h */
XLogoResourceData userOptions;

#define Offset(field) XtOffsetOf(XLogoResourceData, field)

static XtResource resources[] = {
  {"verbose",   "Verbose",   XtRBoolean, sizeof(Boolean), Offset(verbose),      XtRImmediate, (XtPointer)False},
  {"quiet",     "Quiet",     XtRBoolean, sizeof(Boolean), Offset(quiet),        XtRImmediate, (XtPointer)False},
};


static String fallback_resources[] = {
    "*iconPixmap:    xlogo32",
    "*iconMask:      xlogo32",
    "*baseTranslations: #override \\"
                        "\t<Key>q: quit()",
    NULL,
};

static void
die(Widget w, _X_UNUSED XtPointer client_data, _X_UNUSED XtPointer call_data)
{
    XtAppSetExitFlag(XtWidgetToApplicationContext(w));
}

static void
save(_X_UNUSED Widget w, _X_UNUSED XtPointer client_data,
     _X_UNUSED XtPointer call_data)
{
    return;
}

/*
 * Report the syntax for calling xlogo.
 */
_X_NORETURN
static void
Syntax(Widget toplevel, String message, int exitval)
{
    SmcConn connection;
    String reasons[10];
    unsigned int n = 0;

    if (message)
        reasons[n++] = message;

    reasons[n++] = "Usage: ";
    reasons[n++] = (String)ProgramName;
    reasons[n++] = " [-fg <color>] [-bg <color>] [-rv] [-bw <pixels>] [-bd <color>]\n";
    reasons[n++] = "             [-v] [-q]\n";
    reasons[n++] = "             [-d [<host>]:[<vs>]]\n";
    reasons[n++] = "             [-g [<width>][x<height>][<+-><xoff>[<+-><yoff>]]]\n";
#ifdef XRENDER
    reasons[n++] = "             [-render] [-sharp]\n";
#endif /* XRENDER */
    reasons[n++] = "             [-shape]\n";
    reasons[n++] = "             [-version] [-help]\n";
    assert(n <= XtNumber(reasons));

    if (toplevel != NULL) {
	Arg arg;

	XtSetArg(arg, XtNconnection, &connection);
	XtGetValues(toplevel, &arg, (Cardinal)1);
    }
    else
	connection = NULL;

    if (connection)
	SmcCloseConnection(connection, (int) n, (char **) reasons);
    else {
	for (unsigned int i = 0; i < n; i++)
	    printf("%s", reasons[i]);
    }
    exit(exitval);
}

int
main(int argc, char *argv[])
{
    Widget toplevel;
    XtAppContext app_con;

    ProgramName = argv[0];

    /* Handle args that don't require opening a display */
    for (int n = 1; n < argc; n++) {
	const char *argn = argv[n];
	/* accept single or double dash for -help & -version */
	if (argn[0] == '-' && argn[1] == '-') {
	    argn++;
	}
	if (strcmp(argn, "-help") == 0) {
	    Syntax(NULL, NULL, EXIT_SUCCESS);
	}
	if (strcmp(argn, "-version") == 0) {
	    puts(PACKAGE_STRING);
	    exit(EXIT_SUCCESS);
	}
    }

    toplevel = XtOpenApplication(&app_con, "XLogo",
				 options, XtNumber(options),
				 &argc, argv, fallback_resources,
				 sessionShellWidgetClass, NULL, ZERO);
    if (argc != 1) {
        const char *header = "Unknown argument(s):";
        char *message;
        size_t len = strlen(header) + 3; /* 3 for "\n\n\0" */
        for (int n = 1; n < argc; n++) {
            len += strlen(argv[n]) + 1;
	}
        message = malloc(len);
        if (message != NULL) {
            strlcpy(message, header, len);
            for (int n = 1; n < argc; n++) {
                strlcat(message, " ", len);
                strlcat(message, argv[n], len);
            }
            strlcat(message, "\n\n", len);
        }
	Syntax(toplevel, message, EXIT_FAILURE);
    }

    XtGetApplicationResources(toplevel, (XtPointer)&userOptions, resources,
                              XtNumber(resources), NULL, 0);

    XtAppAddActions(app_con, actions, XtNumber(actions));

    XtAddCallback(toplevel, XtNsaveCallback, save, NULL);
    XtAddCallback(toplevel, XtNdieCallback,  die,  NULL);
    XtOverrideTranslations
	(toplevel, XtParseTranslationTable ("<Message>WM_PROTOCOLS: quit()"));
    XtCreateManagedWidget("xlogo", logoWidgetClass, toplevel, NULL, ZERO);
    XtRealizeWidget(toplevel);
    wm_delete_window = XInternAtom(XtDisplay(toplevel), "WM_DELETE_WINDOW",
				   False);
    (void) XSetWMProtocols (XtDisplay(toplevel), XtWindow(toplevel),
			    &wm_delete_window, 1);

    XtAppMainLoop(app_con);

    return EXIT_SUCCESS;
}

/*ARGSUSED*/
static void
quit(Widget w, XEvent *event,
     _X_UNUSED String *params, _X_UNUSED Cardinal *num_params)
{
    Arg arg;

    if (event->type == ClientMessage &&
	(Atom)event->xclient.data.l[0] != wm_delete_window) {
#ifdef XKB
	XkbStdBell(XtDisplay(w), XtWindow(w), 0, XkbBI_BadValue);
#else
	XBell(XtDisplay(w), 0);
#endif
    } else {
	/* resign from the session */
	XtSetArg(arg, XtNjoinSession, False);
	XtSetValues(w, &arg, ONE);
	die(w, NULL, NULL);
    }
}

