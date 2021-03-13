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
 * xdm - display manager daemon
 * Author:  Keith Packard, MIT X Consortium
 *
 * server.c - manage the X server
 */

#include	"dm.h"
#include	"dm_error.h"
#include 	"dm_socket.h"

#include	<X11/Xlib.h>
#include	<X11/Xos.h>
#include	<stdio.h>
#include	<signal.h>
#include	<errno.h>
#include	<poll.h>
#include 	<sys/socket.h>

static int receivedUsr1;

static bool serverPause (unsigned t, pid_t serverPid);

/* ARGSUSED */
static void
CatchUsr1 (int n)
{
    int olderrno = errno;

    Debug ("display manager caught SIGUSR1\n");
    ++receivedUsr1;
    errno = olderrno;
}

const char *_SysErrorMsg (int n)
{
    const char *s = strerror(n);
    return (s ? s : "unknown error");
}

/* true if server successufully started */
static bool
StartServerOnce (struct display *d)
{
    char	**f;
    char	**argv;
    char	arg[1024];
    pid_t	pid;

    Debug ("StartServer for %s\n", d->name);
    receivedUsr1 = 0;
    (void) signal (SIGUSR1, CatchUsr1);
    argv = d->argv;
    switch (pid = fork ()) {
    case 0:
	CleanUpChild ();
	if (d->authFile) {
	    snprintf (arg, sizeof(arg), "-auth %s", d->authFile);
	    argv = parseArgs (argv, arg);
	}
	if (d->listenTcp)
	    argv = parseArgs(argv, "-listen tcp");
	if (!argv) {
	    LogError ("StartServer: no arguments\n");
	    sleep ((unsigned) d->openDelay);
	    exit (UNMANAGE_DISPLAY);
	}
	for (f = argv; *f; f++)
	    Debug ("'%s' ", *f);
	Debug ("\n");
	/*
	 * give the server SIGUSR1 ignored,
	 * it will notice that and send SIGUSR1
	 * when ready
	 */
	signal (SIGUSR1, SIG_IGN);
	execv (argv[0], argv);
	LogError ("server %s cannot be executed\n",
			argv[0]);
	sleep ((unsigned) d->openDelay);
	exit (REMANAGE_DISPLAY);
    case -1:
	LogError ("fork failed, sleeping\n");
	return false;
    default:
	break;
    }
    Debug ("Server Started %d\n", pid);
    d->serverPid = pid;
    if (serverPause ((unsigned) d->openDelay, pid))
	return false;
    return true;
}

bool
StartServer (struct display *d)
{
    int	i;
    bool ret = false;

    i = 0;
    while (d->serverAttempts == 0 || i < d->serverAttempts)
    {
        Debug("Starting X server attempt %d of %d\n", i, d->serverAttempts);
	if ((ret = StartServerOnce (d)) == true)
	    break;
	sleep (d->openDelay);
	i++;
    }
    return ret;
}

/*
 * sleep for t seconds, return 1 if the server is dead when
 * the sleep finishes, 0 else
 */

static void
chldHandler(int num)
{
    Debug("a child died\n");
    return;
}

static bool	serverPauseRet;

/* true if server died */
static bool
serverPause (unsigned t, pid_t serverPid)
{
    struct timespec timeout;
    sigset_t mask;
    int result;
    pid_t pid;
    void (*old)(int);

    Debug("serverPause\n");
    serverPauseRet = false;

    for (;;) {
        timeout.tv_sec = t;
	timeout.tv_nsec = 0;

        /* setup a SIGCHLD handler, to interrupt ppoll() below */
        old = signal(SIGCHLD, chldHandler);
        /* unblock all signals*/
	sigemptyset(&mask);
        if (!receivedUsr1) {
            result = ppoll(NULL, 0, &timeout, &mask);
            if (result != -1 || errno != EINTR)
                Debug("Timeout waiting for USR1\n");
        } else
	    Debug ("Already received USR1\n");

        pid = waitpid (serverPid, (int *) 0, WNOHANG);

        if (pid == serverPid ||
            (pid == -1 && errno == ECHILD)) {
            Debug ("Server dead\n");
            serverPauseRet = true;
            break;
        }
        if (pid == 0) {
            Debug ("Server alive and kicking\n");
            break;
        }
    }
    signal(SIGCHLD, old);
    if (serverPauseRet) {
	Debug ("Server died\n");
	LogError ("server unexpectedly died\n");
    }
    return serverPauseRet;
}

static __dead int
openErrorHandler (Display *dpy)
{
    LogError ("IO Error in XOpenDisplay\n");
    exit(OPENFAILED_DISPLAY);
}

int
WaitForServer (struct display *d)
{
    static int i;

    for (i = 0; i < (d->openRepeat > 0 ? d->openRepeat : 1); i++) {
	Debug ("Before XOpenDisplay(%s)\n", d->name);
	errno = 0;
	(void) XSetIOErrorHandler (openErrorHandler);
	d->dpy = XOpenDisplay (d->name);
	(void) XSetIOErrorHandler ((int (*)(Display *)) 0);
	Debug ("After XOpenDisplay(%s)\n", d->name);
	if (d->dpy) {
		RegisterCloseOnFork (ConnectionNumber (d->dpy));
		(void) fcntl (ConnectionNumber (d->dpy), F_SETFD, 0);
		return 1;
	} else {
		Debug ("OpenDisplay failed %d (%s) on \"%s\"\n",
		    errno, strerror (errno), d->name);
	}
	Debug ("waiting for server to start %d\n", i);
	sleep ((unsigned) d->openDelay);
    }
    Debug ("giving up on server\n");
    LogError ("server open failed for %s, giving up\n", d->name);
    return 0;
}

void
ResetServer (struct display *d)
{
    Debug("ResetServer");
    if (d->dpy)
	pseudoReset (d->dpy);
}

