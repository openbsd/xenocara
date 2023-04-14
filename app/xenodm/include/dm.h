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
 * dm.h
 *
 * public interfaces for greet/verify functionality
 */

#ifndef _DM_H_
# define _DM_H_ 1

# ifdef HAVE_CONFIG_H
#  include "config.h"
# endif

# include <X11/Xos.h>
# include <X11/Xfuncs.h>
# include <X11/Xfuncproto.h>
# include <X11/Xmd.h>
# include <X11/Xauth.h>
# include <X11/Intrinsic.h>

# include <limits.h>
# include <time.h>
# define Time_t time_t
# include <stdlib.h>
# include <stdbool.h>

# include <X11/Xdmcp.h>

# include <sys/wait.h>
# define waitCode(w)	(WIFEXITED(w) ? WEXITSTATUS(w) : 0)
# define waitSig(w)	(WIFSIGNALED(w) ? WTERMSIG(w) : 0)
# define waitCore(w)    0	/* not in POSIX.  so what? */
typedef int		waitType;

# include <sys/param.h>
# include <login_cap.h>
# include <pwd.h>
# include <bsd_auth.h>

# define waitCompose(sig,core,code) ((sig) * 256 + (core) * 128 + (code))
# define waitVal(w)	waitCompose(waitSig(w), waitCore(w), waitCode(w))

typedef enum displayStatus { running, notRunning, zombie, phoenix } DisplayStatus;

typedef enum fileState { NewEntry, OldEntry, MissingEntry } FileState;

struct display {
	struct display	*next;
	/* Xservers file / XDMCP information */
	char		*name;		/* DISPLAY name */
	char		*class;		/* display class (may be NULL) */
	char		**argv;		/* program name and arguments */

	/* display state */
	DisplayStatus	status;		/* current status */
	pid_t		pid;		/* process id of child */
	pid_t		serverPid;	/* process id of server (-1 if none) */
	FileState	state;		/* state during HUP processing */
	int		startTries;	/* current start try */
	Time_t		lastReserv;     /* time of last reserver crash */
	int		reservTries;	/* current reserver try */
	/* server management resources */
	int		serverAttempts;	/* number of attempts at running X */
	int		openDelay;	/* open delay time */
	int		openRepeat;	/* open attempts to make */
	int		openTimeout;	/* abort open attempt timeout */
	int		startAttempts;	/* number of attempts at starting */
	int		reservAttempts;	/* allowed start-IO error sequences */
	bool		terminateServer;/* restart for each session */
	bool		grabServer;	/* keep server grabbed for Login */
	int		grabTimeout;	/* time to wait for grab */
	int		resetSignal;	/* signal to reset server */
	int		termSignal;	/* signal to terminate server */
	bool		resetForAuth;	/* server reads auth file at reset */
	char            *keymaps;       /* binary compat with DEC */

	/* session resources */
	char		*resources;	/* resource file */
	char		*xrdb;		/* xrdb program */
	char		*setup;		/* Xsetup program */
	char		*startup;	/* Xstartup program */
	char		*reset;		/* Xreset program */
	char		*session;	/* Xsession program */
	char		*userPath;	/* path set for session */
	char		*systemPath;	/* path set for startup/reset */
	char		*systemShell;	/* interpreter for startup/reset */
	char		*failsafeClient;/* a client to start when the session fails */

	/* authorization resources */
	bool		authorize;	/* enable authorization */
	char		**authNames;	/* authorization protocol names */
	unsigned short	*authNameLens;	/* authorization protocol name lens */
	char		*clientAuthFile;/* client specified auth file */
	bool		authComplain;	/* complain when no auth for XDMCP */
	bool		listenTcp;	/* assume server is listening on TCP */

	/* information potentially derived from resources */
	int		authNameNum;	/* number of protocol names */
	Xauth		**authorizations;/* authorization data */
	int		authNum;	/* number of authorizations */
	char		*authFile;	/* file to store authorization in */

	/* add new fields only after here.  And preferably at the end. */

	Display		*dpy;		/* Display */
	char		*windowPath;	/* path to server "window" */
        char		*consolePath;	/* path to the console device */

	/* autologin */
	char		*autoLogin;	/* user to auto-login */
};


#  define PROTO_TIMEOUT	(30 * 60)   /* 30 minutes should be long enough */
#  define XDM_BROKEN_INTERVAL (120)   /* server crashing more than once within */
                                    /* two minutes is assumed to be broken!  */

struct greet_info {
	char		*name;		/* user name */
	char		*password;	/* user password */
	char		*string;	/* random string */
	/* add new fields below this line, and preferably at the end */
	Boolean		allow_null_passwd; /* allow null password on login */
	Boolean		allow_root_login; /* allow direct root login */
};

typedef void (*ListenFunc)(ARRAY8Ptr addr, void **closure);

struct verify_info {
	int		uid;		/* user id */
	int		gid;		/* group id */
	char		**argv;		/* arguments to session */
	char		**userEnviron;	/* environment for session */
	char		**systemEnviron;/* environment for startup/reset */
	/* add new fields below this line, and preferably at the end */
};

/* display manager exit status definitions */

# define OBEYSESS_DISPLAY	0	/* obey multipleSessions resource */
# define REMANAGE_DISPLAY	1	/* force remanage */
# define UNMANAGE_DISPLAY	2	/* force deletion */
# define RESERVER_DISPLAY	3	/* force server termination */
# define OPENFAILED_DISPLAY	4	/* XOpenDisplay failed, retry */

extern char	*config;
extern char	*servers;
extern int	request_port;
extern int	debugLevel;
extern char	*errorLogFile;
extern bool	daemonMode;
extern char	*authDir;
extern bool	autoRescan;
extern bool	removeDomainname;
extern char	**exportList;

extern struct display	*FindDisplayByName (char *name),
			*FindDisplayByPid (pid_t pid),
			*FindDisplayByServerPid (pid_t serverPid),
			*NewDisplay (char *name, char *class);

/* in Login.c */
extern void DrawFail (Widget ctx);

/* in dm.c */
extern void CloseOnFork (void);
extern void RegisterCloseOnFork (int fd);
extern void StartDisplay (struct display *d);

/* in dpylist.c */
extern int AnyDisplaysLeft (void);
extern void ForEachDisplay (void (*f)(struct display *));
extern void RemoveDisplay (struct display *old);

/* in file.c */
extern void ParseDisplay (char *source);

/* in netaddr.c */
extern char *NetaddrAddress(XdmcpNetaddr netaddrp, int *lenp);
extern int ConvertAddr (XdmcpNetaddr saddr, int *len, char **addr);
extern int NetaddrFamily (XdmcpNetaddr netaddrp);

/* in reset.c */
extern void pseudoReset (Display *dpy);

/* in resource.c */
extern void InitResources (int argc, char **argv);
extern void LoadDMResources (void);
extern void LoadServerResources (struct display *d);
extern void LoadSessionResources (struct display *d);
extern void ReinitResources (void);

/* in session.c */
extern char **defaultEnv (char **e);
extern char **systemEnv (struct display *d, char *user, char *home);
extern int source (char **environ, char *file);
extern void ClearCloseOnFork (int fd);
extern void DeleteXloginResources (struct display *d, Display *dpy);
extern void LoadXloginResources (struct display *d);
extern __dead void ManageSession (struct display *d);
extern void SecureDisplay (struct display *d, Display *dpy);
extern __dead void SessionExit (struct display *d, int status, bool removeAuth);
extern void SetupDisplay (struct display *d);
extern void UnsecureDisplay (struct display *d, Display *dpy);
extern void execute(char **argv, char **environ);

/* server.c */
extern const char *_SysErrorMsg (int n);
extern bool StartServer (struct display *d);
extern int WaitForServer (struct display *d);
extern void ResetServer (struct display *d);

/* in util.c */
extern char **parseArgs (char **argv, const char *string);
extern char **setEnv (char **e, const char *name, const char *value);
extern char **putEnv(const char *string, char **env);
extern char *getEnv (char **e, const char *name);
extern void CleanUpChild (void);
extern void freeArgs (char **argv);
extern void freeEnv (char **env);
extern void printEnv (char **e);

/* in verify.c */
extern int Verify (struct display *d, struct greet_info *greet, struct verify_info *verify);
extern int autoLoginEnv(struct display *d, struct verify_info *verify, struct greet_info *greet);

/* in dm.c */
extern void StopDisplay (struct display *d);
extern void WaitForChild (void);

/*
 * CloseOnFork flags
 */

# define CLOSE_ALWAYS	    0
# define LEAVE_FOR_DISPLAY  1

#endif /* _DM_H_ */
