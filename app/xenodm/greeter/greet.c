/*

Copyright 1988, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from The Open Group.

*/
/*
 * Copyright (c) 2006, Oracle and/or its affiliates. All rights reserved.
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


/*
 * xdm - display manager daemon
 * Author:  Keith Packard, X Consortium
 *
 * widget to get username/password
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/XKBlib.h>

# include <X11/extensions/Xinerama.h>

#include "dm.h"
#include "dm_error.h"
#include "greet.h"
#include "LoginP.h"

#include <syslog.h>
#ifndef LOG_AUTHPRIV
# define LOG_AUTHPRIV LOG_AUTH
#endif
#ifndef LOG_PID
# define LOG_PID 0
#endif

#include <string.h>

static int	done, code;
static char	name[NAME_LEN], password[PASSWORD_LEN];
static Widget		toplevel;
static Widget		login;
static XtAppContext	context;


/*ARGSUSED*/
static void
GreetDone (
    Widget	w,
    LoginData	*data,
    int		status)
{
    switch (status) {
    case NOTIFY_OK:
	strlcpy (name, data->name, sizeof(name));
	strlcpy (password, data->passwd, sizeof(password));
	code = 0;
	done = 1;
	break;
    case NOTIFY_ABORT:
	Debug ("RESERVER_DISPLAY\n");
	code = RESERVER_DISPLAY;
	done = 1;
	break;
    case NOTIFY_RESTART:
	Debug ("REMANAGE_DISPLAY\n");
	code = REMANAGE_DISPLAY;
	done = 1;
	break;
    case NOTIFY_ABORT_DISPLAY:
	Debug ("UNMANAGE_DISPLAY\n");
	code = UNMANAGE_DISPLAY;
	done = 1;
	break;
    }
    if (done) {
	explicit_bzero (data->name, NAME_LEN);
	explicit_bzero (data->passwd, PASSWORD_LEN);
    }
}

static Display *
InitGreet (struct display *d)
{
    Arg		arglist[10];
    int		i;
    static int	argc;
    Screen		*scrn;
    static char	*argv[] = { "xlogin", NULL };
    Display		*dpy;
    XineramaScreenInfo *screens;
    int                 s_num;

    Debug ("greet %s\n", d->name);
    argc = 1;
    XtToolkitInitialize ();
    context = XtCreateApplicationContext();
    dpy = XtOpenDisplay (context, d->name, "xlogin", "Xlogin", NULL, 0,
			 &argc, argv);

    if (!dpy)
	return NULL;

    {
    int opcode, evbase, errbase, majret, minret;
    unsigned int value = XkbPCF_GrabsUseXKBStateMask;
    if (XkbQueryExtension (dpy, &opcode, &evbase, &errbase, &majret, &minret)) {
	if (!XkbSetPerClientControls (dpy, XkbPCF_GrabsUseXKBStateMask, &value))
	    LogError ("%s\n", "SetPerClientControls failed");
    }
    }
    RegisterCloseOnFork (ConnectionNumber (dpy));

    SecureDisplay (d, dpy);

    i = 0;
    scrn = XDefaultScreenOfDisplay(dpy);
    XtSetArg(arglist[i], XtNscreen, scrn);	i++;
    XtSetArg(arglist[i], XtNargc, argc);	i++;
    XtSetArg(arglist[i], XtNargv, argv);	i++;

    toplevel = XtAppCreateShell ((String) NULL, "Xlogin",
		    applicationShellWidgetClass, dpy, arglist, i);

    i = 0;
    XtSetArg (arglist[i], XtNnotifyDone, (XtPointer)GreetDone); i++;
    if (!d->authorize || d->authorizations || !d->authComplain)
    {
	XtSetArg (arglist[i], XtNsecureSession, True); i++;
    }
    login = XtCreateManagedWidget ("login", loginWidgetClass, toplevel,
				    arglist, i);
    XtRealizeWidget (toplevel);

    if (XineramaIsActive(dpy) &&
	(screens = XineramaQueryScreens(dpy, &s_num)) != NULL)
    {
	XWarpPointer(dpy, None, XRootWindowOfScreen (scrn),
			0, 0, 0, 0,
			screens[0].x_org + screens[0].width / 2,
			screens[0].y_org + screens[0].height / 2);

	XFree(screens);
    }
    else
        XWarpPointer(dpy, None, XRootWindowOfScreen (scrn),
                     0, 0, 0, 0,
                     XWidthOfScreen(scrn) / 2,
                     XHeightOfScreen(scrn) / 2);

    return dpy;
}

static void
CloseGreet (struct display *d)
{
    Boolean	    allow;
    Arg	    arglist[1];
    Display *dpy = XtDisplay(toplevel);

    UnsecureDisplay (d, dpy);
    XtSetArg (arglist[0], XtNallowAccess, (char *) &allow);
    XtGetValues (login, arglist, 1);
    if (allow)
    {
	Debug ("Disabling access control\n");
	XSetAccessControl (dpy, DisableAccess);
    }
    XtDestroyWidget (toplevel);
    toplevel = NULL;
    login = NULL; /* child of toplevel, which we just destroyed */
    ClearCloseOnFork (XConnectionNumber (dpy));
    XCloseDisplay (dpy);
    Debug ("Greet connection closed\n");
}

#define WHITESPACE 0
#define ARGUMENT 1

static int
Greet (struct display *d, struct greet_info *greet)
{
    XEvent		event;
    Arg		arglist[1];

    XtSetArg (arglist[0], XtNallowAccess, False);
    XtSetValues (login, arglist, 1);

    Debug ("dispatching %s\n", d->name);
    done = 0;
    while (!done) {
	XtAppNextEvent (context, &event);
	switch (event.type) {
	case MappingNotify:
	    XRefreshKeyboardMapping(&event.xmapping);
	    break;
	default:
	    XtDispatchEvent (&event);
	    break;
	}
    }
    XFlush (XtDisplay (toplevel));
    Debug ("Done dispatch %s\n", d->name);
    if (code == 0)
    {
	char *ptr;
	unsigned int c,state = WHITESPACE;

	/*
	 * Process the name string to get rid of white spaces.
	 */
	for (ptr = name; state == WHITESPACE; ptr++)
	{
	    c = (unsigned int)(*ptr);
	    if (c == ' ')
		continue;

	    state = ARGUMENT;
	    break;
	}

	greet->name = ptr;
	greet->password = password;
	XtSetArg (arglist[0], XtNsessionArgument, (char *) &(greet->string));
	XtGetValues (login, arglist, 1);
	Debug ("sessionArgument: %s\n", greet->string ? greet->string : "<NULL>");
    }
    return code;
}


static void
FailedLogin (struct display *d, const char *username)
{
    if (username == NULL)
	username = "username unavailable";

    syslog(LOG_AUTHPRIV|LOG_NOTICE,
	   "LOGIN FAILURE ON %s, %s",
	   d->name, username);
    DrawFail (login);
}

_X_EXPORT
greet_user_rtn GreetUser(
    struct display          *d,
    struct verify_info      *verify,
    struct greet_info       *greet)
{
    Arg		arglist[2];
    Display    *dpy;

    /*
     * Run the setup script - note this usually will not work when
     * the server is grabbed, so we don't even bother trying.
     */
    if (!d->grabServer)
	SetupDisplay (d);
    dpy = InitGreet (d);
    if (!dpy) {
	LogError ("Cannot reopen display %s for greet window\n", d->name);
	exit (RESERVER_DISPLAY);
    }

    XtSetArg (arglist[0], XtNallowNullPasswd,
	      (char *) &(greet->allow_null_passwd));
    XtSetArg (arglist[1], XtNallowRootLogin,
	      (char *) &(greet->allow_root_login));
    XtGetValues (login, arglist, 2);

    for (;;) {
	/*
	 * Greet user, requesting name/password
	 */
	code = Greet (d, greet);
	if (code != 0)
	{
	    CloseGreet (d);
	    SessionExit (d, code, false);
	}
	/*
	 * Verify user
	 */
	if (Verify (d, greet, verify))
	    break;
	else
	{
	    FailedLogin (d, greet->name);
	    explicit_bzero (greet->name, strlen(greet->name));
	    explicit_bzero (greet->password, strlen(greet->password));
	}
    }
    DeleteXloginResources (d, dpy);
    CloseGreet (d);
    Debug ("Greet loop finished %d\n", getpid());
    /*
     * Run system-wide initialization file
     */
    if (source (verify->systemEnviron, d->startup) != 0)
    {
	Debug ("Startup program %s exited with non-zero status\n",
		d->startup);
	SessionExit (d, OBEYSESS_DISPLAY, false);
    }
    return Greet_Success;
}

_X_EXPORT
greet_user_rtn AutoLogin(
    struct display          *d,
    struct verify_info      *verify,
    struct greet_info       *greet)
{

    if (!autoLoginEnv(d, verify, greet)) {
        LogError("Autologin %s failed\n", d->autoLogin);
        SessionExit(d, UNMANAGE_DISPLAY, true);
    }
    
    /*
     * Run system-wide initialization file
     */
    if (source (verify->systemEnviron, d->startup) != 0)
    {
	Debug ("Startup program %s exited with non-zero status\n",
		d->startup);
	SessionExit (d, OBEYSESS_DISPLAY, false);
    }
    return Greet_Success;
}
