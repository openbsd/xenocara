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
 * session.c
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "dm.h"
#include "dm_auth.h"
#include "dm_error.h"
#include "greet.h"

#include <sys/types.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Error.h>
#include <ctype.h>
#include <errno.h>
#include <grp.h>	/* for initgroups */
#include <signal.h>
#include <stdio.h>
#include <util.h>

static	int	runAndWait (char **args, char **environ);

#include <pwd.h>
#include <unistd.h>

static Bool StartClient(
    struct verify_info	*verify,
    struct display	*d,
    pid_t		*pidp,
    char		*name);

static pid_t			clientPid;
static struct greet_info	greet;
static struct verify_info	verify;

static int abortSession = 0;

/* ARGSUSED */
static void
catchTerm (int n)
{
    abortSession++;
}

static int childDied = 0;
static void catchChld(int n);

static void
catchChld(int n)
{
    childDied++;
}

static void
AbortClient (pid_t pid)
{
    int	sig = SIGTERM;
    volatile int	i;
    void (*old)(int);
    pid_t	retId;

    LogInfo("AbortClient\n");
    old = signal(SIGCHLD, catchChld);
    for (i = 0; i < 4; i++) {
	if (killpg (pid, sig) == -1) {
	    switch (errno) {
	    case EPERM:
		LogError ("xenodm can't kill client\n");
	    case EINVAL:
	    case ESRCH:
		goto done;
	    }
	}
	sleep(10);
	if (childDied) {
	  retId = waitpid(pid, NULL, WNOHANG);
	  LogInfo("retId = %d\n", retId);
	  if (retId == pid)
	    break;
	}
	sig = SIGKILL;
    }
done:
    signal(SIGCHLD, old);
}


/*
 * We need our own error handlers because we can't be sure what exit code Xlib
 * will use, and our Xlib does exit(1) which matches REMANAGE_DISPLAY, which
 * can cause a race condition leaving the display wedged.  We need to use
 * RESERVER_DISPLAY for IO errors, to ensure that the manager waits for the
 * server to terminate.  For other X errors, we should give up.
 */

/*ARGSUSED*/
static __dead int
IOErrorHandler (Display *dpy)
{
    LogError ("fatal IO error %d (%s)\n", errno, _SysErrorMsg(errno));
    exit(RESERVER_DISPLAY);
}

static int
ErrorHandler(Display *dpy, XErrorEvent *event)
{
    LogError ("X error\n");
    if (XmuPrintDefaultErrorMessage (dpy, event, stderr) == 0) return 0;
    exit(UNMANAGE_DISPLAY);
}

__dead void
ManageSession (struct display *d)
{
    pid_t	pid = 0;
    greet_user_rtn	greet_stat;

    Debug ("ManageSession %s\n", d->name);
    (void)XSetIOErrorHandler(IOErrorHandler);
    (void)XSetErrorHandler(ErrorHandler);
    setproctitle("%s", d->name);


    if (d->autoLogin == NULL || d->autoLogin[0] == '\0') {
        /* Load system default Resources */
        LoadXloginResources (d);
        greet_stat = GreetUser(d, &verify, &greet);
    } else
        greet_stat = AutoLogin(d, &verify, &greet);

    if (greet_stat == Greet_Success) {
	clientPid = 0;
	(void) signal (SIGTERM, catchTerm);
	/*
	 * Start the clients, changing uid/groups
	 *	   setting up environment and running the session
	 */
	if (StartClient (&verify, d, &clientPid, greet.name)) {
	  Debug ("Client Started\n");
	  /* Wait for session to end, */
	  pid = waitpid(clientPid, NULL, 0);
	  if (pid <= 0 && abortSession) {
	    /*
	     * when terminating the session, nuke
	     * the child and then run the reset script
	     */
	    AbortClient (clientPid);
	  }
	} else {
	  LogError ("session start failed\n");
	}
    }
    /*
     * run system-wide reset file
     */
    if (d->consolePath != NULL) {
        login_fbtab(d->consolePath, 0, 0);
    }
    Debug ("Source reset program %s\n", d->reset);
    source (verify.systemEnviron, d->reset);
    SessionExit (d, OBEYSESS_DISPLAY, true);
}

void
LoadXloginResources (struct display *d)
{
    char	**args;
    char	**env = NULL;

    if (d->resources[0] && access (d->resources, 4) == 0) {
	env = systemEnv (d, (char *) 0, (char *) 0);
	args = parseArgs ((char **) 0, d->xrdb);
	args = parseArgs (args, d->resources);
	Debug ("Loading resource file: %s\n", d->resources);
	(void) runAndWait (args, env);
	freeArgs (args);
	freeEnv (env);
    }
}

void
SetupDisplay (struct display *d)
{
    char	**env = NULL;

    if (d->setup && d->setup[0]) {
	env = systemEnv (d, (char *) 0, (char *) 0);
	(void) source (env, d->setup);
	freeEnv (env);
    }
}

/*ARGSUSED*/
void
DeleteXloginResources (struct display *d, Display *dpy)
{
    int i;
    Atom prop = XInternAtom(dpy, "SCREEN_RESOURCES", True);

    XDeleteProperty(dpy, RootWindow (dpy, 0), XA_RESOURCE_MANAGER);
    if (prop) {
	for (i = ScreenCount(dpy); --i >= 0; )
	    XDeleteProperty(dpy, RootWindow (dpy, i), prop);
    }
}

void
SecureDisplay (struct display *d, Display *dpy)
{
    Debug ("SecureDisplay %s\n", d->name);
    Debug ("Before XGrabServer %s\n", d->name);
    XGrabServer (dpy);
    if (XGrabKeyboard (dpy, DefaultRootWindow (dpy), True, GrabModeAsync,
		       GrabModeAsync, CurrentTime) != GrabSuccess) {
	LogError ("WARNING: keyboard on display %s could not be secured\n",
		  d->name);
	SessionExit (d, RESERVER_DISPLAY, false);
    }
    Debug ("XGrabKeyboard succeeded %s\n", d->name);
    pseudoReset (dpy);
    if (!d->grabServer) {
	XUngrabServer (dpy);
	XSync (dpy, 0);
    }
    Debug ("done secure %s\n", d->name);
}

void
UnsecureDisplay (struct display *d, Display *dpy)
{
    Debug ("Unsecure display %s\n", d->name);
    if (d->grabServer) {
	XUngrabServer (dpy);
	XSync (dpy, 0);
    }
}

__dead void
SessionExit (struct display *d, int status, bool removeAuth)
{

    /* make sure the server gets reset after the session is over */
    if (d->serverPid >= 2 && d->resetSignal)
	kill (d->serverPid, d->resetSignal);
    else
	ResetServer (d);
    if (removeAuth) {
	if (setgid (verify.gid) == -1) {
	    LogError( "SessionExit: setgid: %s\n", strerror(errno));
	    exit(status);
	}
	if (setuid (verify.uid) == -1) {
	    LogError( "SessionExit: setuid: %s\n", strerror(errno));
	    exit(status);
	}
	RemoveUserAuthorization (d, &verify);
    }
    Debug ("Display %s exiting with status %d\n", d->name, status);
    exit (status);
}

static Bool
StartClient (
    struct verify_info	*vinfo,
    struct display	*d,
    pid_t		*pidp,
    char		*name)
{
    char	**f, *home;
    char	*failsafeArgv[2];
    pid_t	pid;
    struct passwd* pwd;

    if (pledge("stdio rpath wpath cpath chown fattr proc getpw id exec dns", NULL) == -1)
    	    exit(25);

    if (vinfo->argv) {
	Debug("StartClient %s: ", vinfo->argv[0]);
	for (f = vinfo->argv; *f; f++)
		Debug ("%s ", *f);
	Debug ("; ");
    }
    if (vinfo->userEnviron) {
	for (f = vinfo->userEnviron; *f; f++)
		Debug ("%s ", *f);
	Debug ("\n");
    }
    switch (pid = fork ()) {
    case 0:
	CleanUpChild ();

	/*
	 * Set the user's credentials: uid, gid, groups,
	 * environment variables, resource limits, and umask.
	 */
	pwd = getpwnam(name);
	if (pwd) {
	    if (d->consolePath != NULL)  {
	        Debug("login_fbtab %s %d\n", d->consolePath, geteuid());
	        login_fbtab(d->consolePath, pwd->pw_uid, pwd->pw_gid);
	    }
	    if (setusercontext(NULL, pwd, pwd->pw_uid, LOGIN_SETALL) == -1) {
		LogError ("setusercontext for \"%s\" failed: %s\n",
			  name, _SysErrorMsg (errno));
		return (0);
	    }
	    /*
	     * setusercontext(3) will update the environment based on
	     * the login class so update our idea about the environment
	     * as well
	     */
	    vinfo->userEnviron = defaultEnv (vinfo->userEnviron);
	} else {
	    LogError ("getpwnam for \"%s\" failed: %s\n",
		      name, _SysErrorMsg (errno));
	    return (0);
	}

	if (d->windowPath)
		vinfo->userEnviron = setEnv(vinfo->userEnviron, "WINDOWPATH", d->windowPath);
	else
		Debug("No WINDOWPATH found\n");

	SetUserAuthorization (d, vinfo);
	home = getEnv (vinfo->userEnviron, "HOME");
	if (home)
	    if (chdir (home) == -1) {
		LogError ("user \"%s\": cannot chdir to home \"%s\" (err %d), using \"/\"\n",
			  getEnv (vinfo->userEnviron, "USER"), home, errno);
		chdir ("/");
		vinfo->userEnviron = setEnv(vinfo->userEnviron, "HOME", "/");
	    }
	if (vinfo->argv) {
		LogInfo ("executing session %s\n", vinfo->argv[0]);
		execute (vinfo->argv, vinfo->userEnviron);
		LogError ("Session \"%s\" execution failed (err %d)\n", vinfo->argv[0], errno);
	} else {
		LogError ("Session has no command/arguments\n");
	}
	failsafeArgv[0] = d->failsafeClient;
	failsafeArgv[1] = NULL;
	execute (failsafeArgv, vinfo->userEnviron);
	exit (1);
    case -1:
	Debug("StartClient, fork failed\n");
	LogError ("can't start session on \"%s\", fork failed: %s\n",
		  d->name, _SysErrorMsg (errno));
	return 0;
    default:
	Debug("StartClient, fork succeeded %d\n", pid);
        if (pledge("stdio rpath wpath cpath fattr proc exec id dns", NULL) == -1)
            exit(25);

	*pidp = pid;
	return 1;
    }
}

int
source (char **environ, char *file)
{
    char	**args, *args_safe[2];
    int		ret = 0;
    FILE	*f;

    if (file && file[0]) {
	f = fopen (file, "r");
	if (!f)
	    LogInfo ("not sourcing %s (%s)\n", file, _SysErrorMsg (errno));
	else {
	    fclose (f);
	    LogInfo ("sourcing %s\n", file);
	    args = parseArgs ((char **) 0, file);
	    if (!args) {
		args = args_safe;
		args[0] = file;
		args[1] = NULL;
	    }
	    ret = runAndWait (args, environ);
	    freeArgs (args);
	}
    } else
	Debug ("source() given null pointer in file argument\n");
    return ret;
}

static int
runAndWait (char **args, char **environ)
{
    pid_t	pid;
    waitType	result;

    switch (pid = fork ()) {
    case 0:
	CleanUpChild ();
	execute (args, environ);
	LogError ("can't execute \"%s\" (err %d)\n", args[0], errno);
	exit (1);
    case -1:
	Debug ("fork failed\n");
	LogError ("can't fork to execute \"%s\" (err %d)\n", args[0], errno);
	return 1;
    default:
	while (wait (&result) != pid)
		/* SUPPRESS 530 */
		;
	break;
    }
    return waitVal (result);
}

void
execute (char **argv, char **environ)
{
    /* give /dev/null as stdin */
    (void) close (0);
    open ("/dev/null", O_RDONLY);
    /* make stdout follow stderr to the log file */
    dup2 (2,1);
    Debug ("attempting to execve() %s\n", argv[0]);
    execve (argv[0], argv, environ);
    Debug ("execve() of %s failed: %s\n", argv[0], _SysErrorMsg (errno));
}

char **
defaultEnv (char **e)
{
    char    **env, **exp, *value;

    env = e;
    for (exp = exportList; exp && *exp; ++exp) {
	value = getenv (*exp);
	if (value)
	    env = setEnv (env, *exp, value);
    }
    return env;
}

char **
systemEnv (struct display *d, char *user, char *home)
{
    char	**env;

    env = defaultEnv (NULL);
    env = setEnv (env, "DISPLAY", d->name);
    if (home)
	env = setEnv (env, "HOME", home);
    if (user) {
	env = setEnv (env, "USER", user);
	env = setEnv (env, "LOGNAME", user);
    }
    env = setEnv (env, "PATH", d->systemPath);
    env = setEnv (env, "SHELL", d->systemShell);
    if (d->authFile)
	    env = setEnv (env, "XAUTHORITY", d->authFile);
    if (d->windowPath)
	    env = setEnv (env, "WINDOWPATH", d->windowPath);
    return env;
}
