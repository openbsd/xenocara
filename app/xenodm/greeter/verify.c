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
 * verify.c
 *
 * typical unix verification routine.
 */

#include	"dm.h"
#include	"dm_error.h"
#include	"Login.h"

#include	<pwd.h>

#include	<login_cap.h>
#include	<stdarg.h>
#include	<bsd_auth.h>
#include	<vis.h>

#include	"greet.h"

static const char *envvars[] = {
    "TZ",			/* SYSV and SVR4, but never hurts */
    NULL
};

static char **
userEnv (struct display *d, int useSystemPath, char *user, char *home, char *shell)
{
    char	**env;
    const char	**envvar;
    const char	*str;

    env = defaultEnv (NULL);
    env = setEnv (env, "DISPLAY", d->name);
    env = setEnv (env, "HOME", home);
    env = setEnv (env, "LOGNAME", user); /* POSIX, System V */
    env = setEnv (env, "USER", user);    /* BSD */
    env = setEnv (env, "PATH", useSystemPath ? d->systemPath : d->userPath);
    env = setEnv (env, "SHELL", shell);
    for (envvar = envvars; *envvar; envvar++)
    {
	str = getenv(*envvar);
	if (str)
	    env = setEnv (env, *envvar, str);
    }
    return env;
}

_X_INTERNAL
int
Verify (struct display *d, struct greet_info *greet, struct verify_info *verify)
{
	struct passwd	*p;
	login_cap_t	*lc;
	auth_session_t	*as;
	char		*style, *shell, *home, *s, **argv;
	char		path[MAXPATHLEN];
	int		authok;
	size_t		passwd_len;
	char		visname[NAME_LEN*4];

	/* User may have specified an authentication style. */
	if ((style = strchr(greet->name, ':')) != NULL)
		*style++ = '\0';

	strvis(visname, greet->name, 0);
	Debug ("Verify %s, style %s ...\n", visname,
	    style ? style : "default");
	explicit_bzero(visname, NAME_LEN*4);

	p = getpwnam (greet->name);
	if (!p || strlen (greet->name) == 0) {
		Debug("getpwnam() failed.\n");
		explicit_bzero(greet->password, strlen(greet->password));
		return 0;
	}

	if ((lc = login_getclass(p->pw_class)) == NULL) {
		Debug("login_getclass() failed.\n");
		explicit_bzero(greet->password, strlen(greet->password));
		return 0;
	}
	if ((style = login_getstyle(lc, style, "xdm")) == NULL) {
		Debug("login_getstyle() failed.\n");
		explicit_bzero(greet->password, strlen(greet->password));
		return 0;
	}
	if ((as = auth_open()) == NULL) {
		Debug("auth_open() failed.\n");
		login_close(lc);
		explicit_bzero(greet->password, strlen(greet->password));
		return 0;
	}
	if (auth_setoption(as, "login", "yes") == -1) {
		Debug("auth_setoption() failed.\n");
		login_close(lc);
		explicit_bzero(greet->password, strlen(greet->password));
		return 0;
	}
	passwd_len = strlen(greet->password);
	/* Set up state for no challenge, just check a response. */
	auth_setstate(as, 0);
	auth_setdata(as, "", 1);
	auth_setdata(as, greet->password, passwd_len + 1);
	/* wipe password now, otherwise it'll be copied fork() in auth_call */
	explicit_bzero(greet->password, passwd_len);
	/* Build path of the auth script and call it */
	snprintf(path, sizeof(path), _PATH_AUTHPROG "%s", style);
	auth_call(as, path, style, "-s", "response", "--", greet->name,
		  lc->lc_class, (void *)NULL);
	authok = auth_getstate(as);

	if ((authok & AUTH_ALLOW) == 0) {
		Debug("password verify failed\n");
		auth_close(as);
		login_close(lc);
		return 0;
	}
	/* Run the approval script */
	if (!auth_approval(as, lc, greet->name, "auth-xdm")) {
		Debug("login not approved\n");
		auth_close(as);
		login_close(lc);
		return 0;
	}
	auth_close(as);
	login_close(lc);
	/* Check empty passwords against allowNullPasswd */
	if (!greet->allow_null_passwd && passwd_len == 0) {
		Debug("empty password not allowed\n");
		return 0;
	}
	/* Only accept root logins if allowRootLogin resource is set */
	if (p->pw_uid == 0 && !greet->allow_root_login) {
		Debug("root logins not allowed\n");
		return 0;
	}

	/*
	 * Shell must be in /etc/shells
	 */
	for (;;) {
		s = getusershell();
		if (s == NULL) {
			/* did not found the shell in /etc/shells
			   -> failure */
			Debug("shell not in /etc/shells\n");
			endusershell();
			return 0;
		}
		if (strcmp(s, p->pw_shell) == 0) {
			/* found the shell in /etc/shells */
			endusershell();
			break;
		}
	}

	Debug ("verify succeeded\n");
	verify->uid = p->pw_uid;
	verify->gid = p->pw_gid;
	home = p->pw_dir;
	shell = p->pw_shell;
	argv = NULL;
	if (d->session)
		argv = parseArgs (argv, d->session);
	if (greet->string)
		argv = parseArgs (argv, greet->string);
	if (!argv)
		argv = parseArgs (argv, "xsession");
	verify->argv = argv;
	verify->userEnviron = userEnv (d, p->pw_uid == 0,
				       greet->name, home, shell);
	Debug ("user environment:\n");
	printEnv (verify->userEnviron);
	verify->systemEnviron = systemEnv (d, greet->name, home);
	Debug ("system environment:\n");
	printEnv (verify->systemEnviron);
	Debug ("end of environments\n");
	return 1;
}

_X_INTERNAL
int
autoLoginEnv(struct display *d, struct verify_info *verify,
    struct greet_info *greet)
{
	struct passwd	*p;
	char *shell, *home, **argv;

	Debug("Autologin %s\n", d->autoLogin);
	p = getpwnam (d->autoLogin);
	if (p == NULL)
		return 0;

	greet->name = strdup(d->autoLogin);
	if (greet->name == NULL)
		return 0;
	verify->uid = p->pw_uid;
	verify->gid = p->pw_gid;
	home = p->pw_dir;
	shell = p->pw_shell;
	argv = NULL;
	if (d->session)
		argv = parseArgs (argv, d->session);
	if (greet->string)
		argv = parseArgs (argv, greet->string);
	if (!argv)
		argv = parseArgs (argv, "xsession");
	verify->argv = argv;
	verify->userEnviron = userEnv (d, p->pw_uid == 0,
	    greet->name, home, shell);
	Debug ("user environment:\n");
	printEnv (verify->userEnviron);
	verify->systemEnviron = systemEnv (d, greet->name, home);
	Debug ("system environment:\n");
	printEnv (verify->systemEnviron);
	Debug ("end of environments\n");
	return 1;
}
