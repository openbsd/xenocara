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
 * display manager
 */

#include	"dm.h"
#include	"dm_auth.h"
#include	"dm_error.h"

#include	<paths.h>
#include	<stdio.h>
#include	<signal.h>

#include	<sys/stat.h>
#include	<errno.h>
#include	<X11/Xfuncproto.h>
#include	<X11/Xatom.h>
#include	<stdarg.h>
#include	<stdint.h>

#include	<unistd.h>
#include	<syslog.h>

static void	StopAll (int n), RescanNotify (int n);
static void	RescanServers (void);
static void	RestartDisplay (struct display *d, bool forceReserver);
static void	ScanServers (void);
static void	SetConfigFileTime (void);
static void	StartDisplays (void);

static volatile int	Rescan = 0;
static long	ServersModTime, ConfigModTime;

int nofork_session = 0;

static void ChildNotify (int n);

static pid_t parent_pid = -1; 	/* PID of parent xenodm process */

int
main (int argc, char **argv)
{
    mode_t oldumask;
    char cmdbuf[1024];

    /* make sure at least world write access is disabled */
    if (((oldumask = umask(022)) & 002) == 002)
	(void) umask (oldumask);

    /*
     * Step 1 - load configuration parameters
     */
    InitResources (argc, argv);
    SetConfigFileTime ();
    LoadDMResources ();
    /*
     * Only allow root to run in non-debug mode to avoid problems
     */
    if (debugLevel == 0 && getuid() != 0)
    {
	fprintf (stderr, "Only root wants to run %s\n", argv[0]);
	exit (1);
    }
    if (debugLevel == 0 && daemonMode) {
      if (daemon (0, 0) == -1) {
	/* error */
	LogError("daemon() failed, %s\n", _SysErrorMsg (errno));
	exit(1);
      }
    }
    if (debugLevel == 0)
	InitErrorLog ();
    if (debugLevel >= 10)
	nofork_session = 1;
    LogInfo ("Starting\n");

    if (nofork_session == 0) {
	/* Clean up any old Authorization files */
	/* AUD: all good? */
	snprintf(cmdbuf, sizeof(cmdbuf), "/bin/rm -f %s/authdir/authfiles/A*", authDir);
	system(cmdbuf);
    }
    parent_pid = getpid ();
    (void) signal (SIGTERM, StopAll);
    (void) signal (SIGINT, StopAll);
    /*
     * Step 2 - Read Xservers and set up
     *	    the socket.
     *
     *	    Keep a sub-daemon running
     *	    for each entry
     */
    ScanServers ();
    StartDisplays ();
    (void) signal (SIGHUP, RescanNotify);
    (void) signal (SIGCHLD, ChildNotify);
    Debug ("startup successful; entering main loop\n");
    while (AnyDisplaysLeft ())
    {
	if (Rescan)
	{
	    RescanServers ();
	    Rescan = 0;
	}
	WaitForChild ();
    }
    Debug ("Nothing left to do, exiting\n");
    LogInfo ("Exiting\n");
    return 0;
}

/* ARGSUSED */
static void
RescanNotify (int n)
{
    int olderrno = errno;

    Debug ("Caught SIGHUP\n");
    Rescan = 1;
    errno = olderrno;
}

static void
ScanServers (void)
{
    char	lineBuf[10240];
    int		len;
    FILE	*serversFile;
    struct stat	statb;

    if (servers[0] == '/')
    {
	serversFile = fopen (servers, "r");
	if (serversFile == NULL)
	{
	    LogError ("cannot access servers file %s\n", servers);
	    return;
	}
	if (ServersModTime == 0)
	{
	    fstat (fileno (serversFile), &statb);
	    ServersModTime = statb.st_mtime;
	}
	while (fgets (lineBuf, sizeof (lineBuf)-1, serversFile))
	{
	    len = strlen (lineBuf);
	    if (lineBuf[len-1] == '\n')
		lineBuf[len-1] = '\0';
	    ParseDisplay (lineBuf);
	}
	fclose (serversFile);
    }
    else
    {
	ParseDisplay (servers);
    }
}

static void
MarkDisplay (struct display *d)
{
    d->state = MissingEntry;
}

static void
RescanServers (void)
{
    Debug ("rescanning servers\n");
    LogInfo ("Rescanning both config and servers files\n");
    ForEachDisplay (MarkDisplay);
    SetConfigFileTime ();
    ReinitResources ();
    LoadDMResources ();
    ScanServers ();
    StartDisplays ();
}

static void
SetConfigFileTime (void)
{
    struct stat	statb;

    if (stat (config, &statb) != -1)
	ConfigModTime = statb.st_mtime;
}


static void
RescanIfMod (void)
{
    struct stat	statb;

    if (config && stat (config, &statb) != -1)
    {
	if (statb.st_mtime != ConfigModTime)
	{
	    Debug ("Config file %s has changed, rereading\n", config);
	    LogInfo ("Rereading configuration file %s\n", config);
	    ConfigModTime = statb.st_mtime;
	    ReinitResources ();
	    LoadDMResources ();
	}
    }
    if (servers[0] == '/' && stat(servers, &statb) != -1)
    {
	if (statb.st_mtime != ServersModTime)
	{
	    Debug ("Servers file %s has changed, rescanning\n", servers);
	    LogInfo ("Rereading servers file %s\n", servers);
	    ServersModTime = statb.st_mtime;
	    ForEachDisplay (MarkDisplay);
	    ScanServers ();
	}
    }
}

/*
 * catch a SIGTERM, kill all displays and exit
 */

/* ARGSUSED */
static void
StopAll (int n)
{
    int olderrno = errno;

    if (parent_pid != getpid())
    {
	/*
	 * We are a child xdm process that was killed by the
	 * parent xdm before we were able to return from fork()
	 * and remove this signal handler.
	 *
	 * See defect XWSog08655 for more information.
	 */
	Debug ("Child xenodm caught SIGTERM before it removed that signal.\n");
	(void) signal (n, SIG_DFL);
	kill (getpid(), SIGTERM);
	errno = olderrno;
	return;
    }
    Debug ("Shutting down entire manager\n");
    LogInfo ("Shutting down\n");
    ForEachDisplay (StopDisplay);
    errno = olderrno;
}

/*
 * notice that a child has died and may need another
 * sub-daemon started
 */

static volatile int	ChildReady = 0;

/* ARGSUSED */
static void
ChildNotify (int n)
{
    int olderrno = errno;

    ChildReady++;
    errno = olderrno;
}

void
WaitForChild (void)
{
    pid_t		pid;
    struct display	*d;
    waitType	status;
    sigset_t mask, omask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigaddset(&mask, SIGHUP);
    sigprocmask(SIG_BLOCK, &mask, &omask);
    Debug ("signals blocked\n");
    if (!ChildReady && !Rescan)
	sigsuspend(&omask);
    sigprocmask(SIG_SETMASK, &omask, (sigset_t *)NULL);
    while ((pid = waitpid (-1, &status, WNOHANG)) > 0)
    {
	Debug ("Manager wait returns pid: %d sig %d core %d code %d\n",
	       pid, waitSig(status), waitCore(status), waitCode(status));
        ChildReady--;
	if (autoRescan)
	    RescanIfMod ();
	/* SUPPRESS 560 */
	if ((d = FindDisplayByPid (pid))) {
	    d->pid = -1;
	    switch (waitVal (status)) {
	    case UNMANAGE_DISPLAY:
		Debug ("Display exited with UNMANAGE_DISPLAY\n");
		StopDisplay (d);
		break;
	    case OBEYSESS_DISPLAY:
		d->startTries = 0;
		d->reservTries = 0;
		Debug ("Display exited with OBEYSESS_DISPLAY\n");
		if (d->status == zombie)
		    StopDisplay (d);
		else
		    RestartDisplay (d, false);
		break;
	    case OPENFAILED_DISPLAY:
		Debug ("Display exited with OPENFAILED_DISPLAY, try %d of %d\n",
		       d->startTries, d->startAttempts);
		LogError ("Display %s cannot be opened\n", d->name);
		/*
		 * no display connection was ever made, tell the
		 * terminal that the open attempt failed
		 */
		if (d->status == zombie ||
		    ++d->startTries >= d->startAttempts)
		{
		    LogError ("Display %s is being disabled\n", d->name);
		    StopDisplay (d);
		}
		else
		{
		    RestartDisplay (d, true);
		}
		break;
	    case RESERVER_DISPLAY:
		d->startTries = 0;
		Debug ("Display exited with RESERVER_DISPLAY\n");
		if (d->status == zombie)
		    StopDisplay(d);
		else {
		  Time_t now;
		  int crash;

		  time(&now);
		  crash = d->lastReserv &&
		    ((now - d->lastReserv) < XDM_BROKEN_INTERVAL);
		  Debug("time %lli %lli try %i of %i%s\n", 
			(long long)now, (long long)d->lastReserv,
			d->reservTries, d->reservAttempts,
			crash ? " crash" : "");

		  if (!crash)
		    d->reservTries = 0;

		  if (crash && ++d->reservTries >= d->reservAttempts) {
		    const char *msg =
			"Server crash frequency too high: stopping display";
		    Debug("%s %s\n", msg, d->name);
		    LogError("%s %s\n", msg, d->name);
		    /* For a local X server either:
		     * 1. The server exit was returned by waitpid().  So
		     *    serverPid==-1 => StopDisplay() calls RemoveDisplay()
		     *
		     * 2. The server is in zombie state or still running.  So
		     *    serverPid>1 => StopDisplay()
		     *                   a. sets status=zombie,
		     *                   b. kills the server.
		     *    The next waitpid() returns this zombie server pid
		     *    and the 'case zombie:' below then calls
		     *    RemoveDisplay().
		     */
		    StopDisplay(d);
		  } else {
		    RestartDisplay(d, true);
		  }
		  d->lastReserv = now;
		}
		break;
	    case waitCompose (SIGTERM,0,0):
		Debug ("Display exited on SIGTERM, try %d of %d\n",
			d->startTries, d->startAttempts);
		if (d->status == zombie ||
		    ++d->startTries >= d->startAttempts)
		{
		    /*
		     * During normal xdm shutdown, killed local X servers
		     * can be zombies; this is not an error.
		     */
		    if (d->status == zombie &&
			(d->startTries < d->startAttempts))
			LogInfo ("display %s is being disabled\n", d->name);
		    else
			LogError ("display %s is being disabled\n", d->name);
		    StopDisplay(d);
		} else
		    RestartDisplay (d, true);
		break;
	    case REMANAGE_DISPLAY:
		d->startTries = 0;
		Debug ("Display exited with REMANAGE_DISPLAY\n");
		/*
		 * XDMCP will restart the session if the display
		 * requests it
		 */
		if (d->status == zombie)
		    StopDisplay(d);
		else
		    RestartDisplay (d, false);
		break;
	    default:
		Debug ("Display exited with unknown status %d\n", waitVal(status));
		LogError ("Unknown session exit code %d from process %d\n",
			  waitVal (status), pid);
		StopDisplay (d);
		break;
	    }
	}
	/* SUPPRESS 560 */
	else if ((d = FindDisplayByServerPid (pid)))
	{
	    d->serverPid = -1;
	    switch (d->status)
	    {
	    case zombie:
		Debug ("Zombie server reaped, removing display %s\n", d->name);
		RemoveDisplay (d);
		break;
	    case phoenix:
		Debug ("Phoenix server arises, restarting display %s\n", d->name);
		d->status = notRunning;
		break;
	    case running:
		Debug ("Server for display %s terminated unexpectedly, status %d %d\n", d->name, waitVal (status), status);
		LogError ("Server for display %s terminated unexpectedly: %d\n", d->name, waitVal (status));
		if (d->pid != -1)
		{
		    Debug ("Terminating session pid %d\n", d->pid);
		    kill (d->pid, SIGTERM);
		}
		break;
	    case notRunning:
		Debug ("Server exited for notRunning session on display %s\n", d->name);
		break;
	    }
	}
	else
	{
	    Debug ("Unknown child termination, status %d\n", waitVal (status));
	}
    }
    StartDisplays ();
}

static void
CheckDisplayStatus (struct display *d)
{
    switch (d->state) {
      case MissingEntry:
        StopDisplay (d);
        break;
      case NewEntry:
        d->state = OldEntry;
      case OldEntry:
        if (d->status == notRunning)
            StartDisplay (d);
        break;
    }
}

static void
StartDisplays (void)
{
    ForEachDisplay (CheckDisplayStatus);
}

static void
SetConsolePath(struct display *d)
{
	Atom prop;
	Atom actualtype;
	int actualformat;
	unsigned long nitems;
	unsigned long bytes_after;
	unsigned char *buf;

	prop = XInternAtom(d->dpy, "Xorg_Console", True);
	if (prop == None) {
		LogError("no Xorg_Console atom\n");
		return;
	}
	if (XGetWindowProperty(d->dpy, DefaultRootWindow(d->dpy), prop, 0,
		TTY_NAME_MAX << 2,
		False, AnyPropertyType, &actualtype, &actualformat,
		&nitems, &bytes_after, &buf)) {
		LogError("no Xorg_Console property\n");
		return;
	}
	if (actualtype != XA_STRING) {
		LogError("type %lx in Xorg_Console property!\n", actualtype);
		XFree(buf);
		return;
	}
	if (actualformat != 8) {
		LogError("format %d in Xorg_Console property!\n", actualformat);
		XFree(buf);
		return;
	}
	d->consolePath = strdup(buf + strlen(_PATH_DEV));
	LogInfo("consolePath: %s\n", d->consolePath);
	XFree(buf);
}


static void
SetWindowPath(struct display *d)
{
	/* setting WINDOWPATH for clients */
	Atom prop;
	Atom actualtype;
	int actualformat;
	unsigned long nitems;
	unsigned long bytes_after;
	unsigned char *buf;
	const char *windowpath;
	char *newwindowpath = NULL;
	unsigned long num;

	prop = XInternAtom(d->dpy, "XFree86_VT", True);
	if (prop == None) {
		fprintf(stderr, "no XFree86_VT atom\n");
		return;
	}
	if (XGetWindowProperty(d->dpy, DefaultRootWindow(d->dpy), prop, 0, 1,
		False, AnyPropertyType, &actualtype, &actualformat,
		&nitems, &bytes_after, &buf)) {
		fprintf(stderr, "no XFree86_VT property\n");
		return;
	}
	if (nitems != 1) {
		fprintf(stderr, "%lu items in XFree86_VT property!\n", nitems);
		XFree(buf);
		return;
	}
	switch (actualtype) {
	case XA_CARDINAL:
	case XA_INTEGER:
	case XA_WINDOW:
		switch (actualformat) {
		case  8:
			num = (*(uint8_t  *)(void *)buf);
			break;
		case 16:
			num = (*(uint16_t *)(void *)buf);
			break;
		case 32:
			num = (*(uint32_t *)(void *)buf);
			break;
		default:
			fprintf(stderr, "format %d in XFree86_VT property!\n", actualformat);
			XFree(buf);
			return;
		}
		break;
	default:
		fprintf(stderr, "type %lx in XFree86_VT property!\n", actualtype);
		XFree(buf);
		return;
	}
	XFree(buf);
	windowpath = getenv("WINDOWPATH");
	if (!windowpath) {
		if (asprintf(&newwindowpath, "%lu", num) == -1)
			return;
	} else {
		if (asprintf(&newwindowpath, "%s:%lu", windowpath, num) == -1)
			return;
	}
	free(d->windowPath);
	d->windowPath = newwindowpath;
}

void
StartDisplay (struct display *d)
{
    pid_t	pid;

    Debug ("StartDisplay %s\n", d->name);
    LogInfo ("Starting X server on %s\n", d->name);
    LoadServerResources (d);
    if (d->authorize)
    {
        Debug ("SetLocalAuthorization %s, auth %s\n",
               d->name, d->authNames[0]);
        SetLocalAuthorization (d);
    }
    if (d->serverPid == -1 && !StartServer (d))
    {
        LogError ("Server for display %s can't be started, session disabled\n", d->name);
        RemoveDisplay (d);
        return;
    }
    if (!nofork_session)
	pid = fork ();
    else
	pid = 0;
    switch (pid)
    {
    case 0:
	if (!nofork_session) {
	    CleanUpChild ();
	    (void) signal (SIGPIPE, SIG_IGN);
	}
	openlog("xenodm", LOG_PID, LOG_AUTHPRIV);
	LoadSessionResources (d);
	SetAuthorization (d);
	if (!WaitForServer (d))
	    exit (OPENFAILED_DISPLAY);
	SetConsolePath(d);
	SetWindowPath(d);
	if (pledge("stdio rpath cpath wpath chown fattr flock proc dns inet unix exec prot_exec getpw id", NULL) != 0)
	    exit(OPENFAILED_DISPLAY);
	ManageSession (d);
	exit (REMANAGE_DISPLAY);
    case -1:
	break;
    default:
	Debug ("pid: %d\n", pid);
	d->pid = pid;
	d->status = running;
	break;
    }
}


/*
 * transition from running to zombie or deleted
 */

void
StopDisplay (struct display *d)
{
    if (d->serverPid != -1)
	d->status = zombie; /* be careful about race conditions */
    if (d->pid != -1)
	kill (d->pid, SIGTERM);
    if (d->serverPid != -1)
	kill (d->serverPid, d->termSignal);
    else
	RemoveDisplay (d);
}

/*
 * transition from running to phoenix or notRunning
 */

static void
RestartDisplay (struct display *d, bool forceReserver)
{
    if (d->serverPid != -1 && (forceReserver || d->terminateServer))
    {
	kill (d->serverPid, d->termSignal);
	d->status = phoenix;
    }
    else
    {
	d->status = notRunning;
    }
}

static fd_set	CloseMask;
static int	max;

void
RegisterCloseOnFork (int fd)
{
    FD_SET (fd, &CloseMask);
    if (fd > max)
	max = fd;
}

void
ClearCloseOnFork (int fd)
{
    FD_CLR (fd, &CloseMask);
    if (fd == max) {
	while (--fd >= 0)
	    if (FD_ISSET (fd, &CloseMask))
		break;
	max = fd;
    }
}

void
CloseOnFork (void)
{
    int	fd;

    for (fd = 0; fd <= max; fd++)
	if (FD_ISSET (fd, &CloseMask))
	{
	    close (fd);
        }
    FD_ZERO (&CloseMask);
    max = 0;
}
