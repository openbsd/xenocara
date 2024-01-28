/* $XConsortium: xbiff.c,v 1.19 94/04/17 20:43:28 rws Exp $ */
/*

Copyright (c) 1988  X Consortium

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/
/* $XFree86: xc/programs/xbiff/xbiff.c,v 1.3tsi Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include "Mailbox.h"
#include <X11/Xaw/Cardinals.h>

static const char *ProgramName;

static XrmOptionDescRec options[] = {
{ "-update", "*mailbox.update", XrmoptionSepArg, (caddr_t) NULL },
{ "-file",   "*mailbox.file", XrmoptionSepArg, (caddr_t) NULL },
{ "-volume", "*mailbox.volume", XrmoptionSepArg, (caddr_t) NULL },
{ "-shape",  "*mailbox.shapeWindow", XrmoptionNoArg, (caddr_t) "on" },
};

static Atom wm_delete_window;

static void quit (Widget w, XEvent *event, String *params, Cardinal *num_params)
{
    if (event->type == ClientMessage &&
        ((Atom) event->xclient.data.l[0]) != wm_delete_window) {
        XBell (XtDisplay(w), 0);
        return;
    }
    XCloseDisplay (XtDisplay(w));
    exit (0);
}

static XtActionsRec xbiff_actions[] = {
    { "quit", quit },
};

static void _X_NORETURN _X_COLD
Usage (int exitval)
{
    const char *help_message =
"where options include:\n"
"    -display host:dpy              X server to contact\n"
"    -geometry geom                 size of mailbox\n"
"    -file file                     file to watch\n"
"    -update seconds                how often to check for mail\n"
"    -volume percentage             how loud to ring the bell\n"
"    -bg color                      background color\n"
"    -fg color                      foreground color\n"
"    -rv                            reverse video\n"
"    -shape                         shape the window\n"
"    -help                          print usage info and exit\n"
"    -version                       print version info and exit\n";

    fprintf (stderr, "usage:  %s [-options ...]\n", ProgramName);
    fprintf (stderr, "%s\n", help_message);
    exit (exitval);
}


int
main (int argc, char **argv)
{
    XtAppContext xtcontext;
    Widget toplevel;

    ProgramName = argv[0];

    XtSetLanguageProc(NULL, (XtLanguageProc) NULL, NULL);

    /* Handle args that don't require opening a display */
    for (int n = 1; n < argc; n++) {
	const char *argn = argv[n];
	/* accept single or double dash for -help & -version */
	if (argn[0] == '-' && argn[1] == '-') {
	    argn++;
	}
	if (strcmp(argn, "-help") == 0) {
	    Usage(0);
	}
	if (strcmp(argn, "-version") == 0) {
	    puts(PACKAGE_STRING);
	    exit(0);
	}
    }

    toplevel = XtAppInitialize(&xtcontext, "XBiff", options, XtNumber (options),
			       &argc, argv, NULL, NULL, 0);
    if (argc != 1) {
	fputs("Unknown argument(s):", stderr);
	for (int n = 1; n < argc; n++) {
	    fprintf(stderr, " %s", argv[n]);
	}
	fputs("\n\n", stderr);
	Usage(1);
    }

    /*
     * This is a hack so that f.delete will do something useful in this
     * single-window application.
     */
    wm_delete_window = XInternAtom (XtDisplay(toplevel), "WM_DELETE_WINDOW",
                                    False);
    XtAppAddActions (xtcontext, xbiff_actions, XtNumber(xbiff_actions));
    XtOverrideTranslations(toplevel,
		   XtParseTranslationTable ("<Message>WM_PROTOCOLS: quit()"));

    (void) XtCreateManagedWidget ("mailbox", mailboxWidgetClass, toplevel,
				  NULL, 0);
    XtRealizeWidget (toplevel);
    (void) XSetWMProtocols (XtDisplay(toplevel), XtWindow(toplevel),
                            &wm_delete_window, 1);
    XtAppMainLoop (xtcontext);

    return 0;
}
