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
 * auth.c
 *
 * maintain the authorization generation daemon
 */

#include <X11/X.h>
#include <X11/Xlibint.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "dm.h"
#include "dm_auth.h"
#include "dm_error.h"


#include <errno.h>
#include <pwd.h>
#include <string.h>

#include <sys/ioctl.h>

#if defined(TCPCONN)
# include "dm_socket.h"
# include <netinet/in.h>
#endif


struct AuthProtocol {
    unsigned short  name_length;
    const char	    *name;
    void	    (*InitAuth)(unsigned short len, char *name);
    Xauth	    *(*GetAuth)(unsigned short len, char *name);
    int		    inited;
};

static struct AuthProtocol AuthProtocols[] = {
{ (unsigned short) 18,	"MIT-MAGIC-COOKIE-1",
    MitInitAuth, MitGetAuth,
},
};

#define NUM_AUTHORIZATION (sizeof (AuthProtocols) / sizeof (AuthProtocols[0]))

static struct AuthProtocol *
findProtocol (unsigned short name_length, char *name)
{
    int	i;

    for (i = 0; i < NUM_AUTHORIZATION; i++)
	if (AuthProtocols[i].name_length == name_length &&
	    memcmp(AuthProtocols[i].name, name, name_length) == 0)
	{
	    return &AuthProtocols[i];
	}
    return (struct AuthProtocol *) 0;
}

bool
ValidAuthorization (unsigned short name_length, char *name)
{
    return findProtocol (name_length, name) != NULL;
}

static Xauth *
GenerateAuthorization (unsigned short name_length, char *name)
{
    struct AuthProtocol	*a;
    Xauth   *auth = NULL;
    int	    i;

    Debug ("GenerateAuthorization %*.*s\n",
	    name_length, name_length, name);
    a = findProtocol (name_length, name);
    if (a)
    {
	if (!a->inited)
	{
	    (*a->InitAuth) (name_length, name);
	    a->inited = true;
	}
	auth = (*a->GetAuth) (name_length, name);
	if (auth)
	{
	    Debug ("Got %p (%d %*.*s) ", auth,
		auth->name_length, auth->name_length,
		auth->name_length, auth->name);
	    for (i = 0; i < (int)auth->data_length; i++)
		Debug (" %02x", auth->data[i] & 0xff);
	    Debug ("\n");
	}
	else
	    Debug ("Got (null)\n");
    }
    else
    {
	Debug ("Unknown authorization %*.*s\n", name_length, name_length, name);
    }
    return auth;
}

void
CleanUpFileName (char *src, char *dst, int len)
{
    while (*src) {
	if (--len <= 0)
		break;
	switch (*src & 0x7f)
	{
	case '/':
	    *dst++ = '_';
	    break;
	case '-':
	    *dst++ = '.';
	    break;
	default:
	    *dst++ = (*src & 0x7f);
	}
	++src;
    }
    *dst = '\0';
}

/* Checks to see if specified directory exists, makes it if not
 * Returns: 0 if already exists, 1 if created, < 0 if error occurred
 */
static int
CheckServerAuthDir (const char *path, struct stat *statb, int mode)
{
    int r = stat(path, statb);

    if (r == -1) {
	if (errno == ENOENT) {
	    r = mkdir(path, mode);
	    if (r == -1) {
		LogError ("cannot make authentication directory %s: %s\n",
			  path, _SysErrorMsg (errno));
	    } else {
		r = 1;
	    }
	} else {
	    LogError ("cannot access authentication directory %s: %s\n",
		      path, _SysErrorMsg (errno));
	}
    } else { /* Directory already exists */
	if (!S_ISDIR(statb->st_mode)) {
	    LogError ("cannot make authentication directory %s: %s\n",
		      path, "file with that name already exists");
	    return -1;
	}
    }
    return r;
}

static char authdir1[] = "authdir";
static char authdir2[] = "authfiles";

static bool
MakeServerAuthFile (struct display *d, FILE ** file, uid_t uid, gid_t gid)
{
    int len;
#ifdef MAXNAMELEN
# define NAMELEN	MAXNAMELEN
#else
# define NAMELEN	255
#endif
    char    cleanname[NAMELEN];
    int r;
    int fd;
    struct stat	statb;

    *file = NULL;

    if (!d->authFile) {
	if (d->clientAuthFile && *d->clientAuthFile) {
	    d->authFile = strdup(d->clientAuthFile);
	    if (!d->authFile)
		return false;
	} else {
	    CleanUpFileName (d->name, cleanname, NAMELEN - 8);

	    /* Make authDir if it doesn't already exist */
	    r = CheckServerAuthDir(authDir, &statb, 0755);
	    if (r < 0) {
		return false;
	    }

	    len = strlen (authDir) + strlen (authdir1) + strlen (authdir2)
		+ strlen (cleanname) + 14;
	    d->authFile = malloc (len);
	    if (!d->authFile)
		return false;

	    snprintf (d->authFile, len, "%s/%s", authDir, authdir1);
	    r = CheckServerAuthDir(d->authFile, &statb, 0700);
	    if (r == 0) {
		if (statb.st_uid != uid)
		    (void) chown(d->authFile, uid, statb.st_gid);
		if ((statb.st_mode & 0077) != 0)
		    (void) chmod(d->authFile, statb.st_mode & 0700);
	    } else if (r < 0) {
		free (d->authFile);
		d->authFile = NULL;
		return false;
	    } else {
		(void) chown(d->authFile, uid, gid);
	    }

	    snprintf (d->authFile, len, "%s/%s/%s",
		      authDir, authdir1, authdir2);
	    r = CheckServerAuthDir(d->authFile, &statb, 0700);
	    if (r < 0) {
		free (d->authFile);
		d->authFile = NULL;
		return false;
	    } else {
		(void) chown(d->authFile, uid, gid);
	    }
	    snprintf (d->authFile, len, "%s/%s/%s/A%s-XXXXXX",
		      authDir, authdir1, authdir2, cleanname);
	    fd = mkstemp (d->authFile);
	    if (fd == -1) {
		LogError ("cannot make authentication file %s: %s\n",
			  d->authFile, _SysErrorMsg (errno));
		free (d->authFile);
		d->authFile = NULL;
		return false;
	    }

	    *file = fdopen(fd, "w");
	    if (!*file)
		(void) close (fd);
	    return true;
	}
    }

    (void) unlink (d->authFile);
    *file = fopen (d->authFile, "w");
    return true;
}

bool
SaveServerAuthorizations (
    struct display  *d,
    Xauth	    **auths,
    int		    count)
{
    FILE	*auth_file;
    mode_t	mask;
    int		ret;
    int		i;
    const char	dummy_auth[] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
		               "XXXXXXXXXXXXXXXXX"; /* 64 "X"s */
    int		err = 0;
    uid_t	uid;
    gid_t	gid;
    struct passwd *x11;

    /* Give read capability to group _x11 */
    x11 = getpwnam("_x11");
    if (x11 == NULL) {
	LogError("Can't find _x11 user\n");
	uid = getuid();
	gid = getgid();
    } else {
	uid = x11->pw_uid;
	gid = x11->pw_gid;
    }

    mask = umask (0077);
    ret = MakeServerAuthFile(d, &auth_file, uid, gid);
    umask (mask);
    if (!ret)
	return false;
    if (!auth_file) {
	LogError ("cannot open server authorization file %s: %s\n",
		  d->authFile, _SysErrorMsg (errno));
	ret = false;
    }
    else
    {
	fchown(fileno(auth_file), uid, gid);
	Debug ("File: %s auth: %p\n", d->authFile, auths);
	ret = true;
	if (count == 0)
	{
		/*
		 * This is a crude hack to determine whether we really can
		 * write to the auth file even if we don't have real data
		 * to write right now.
		 */

		/*
		 * Write garbage data to file to provoke ENOSPC and other
		 * errors.
		 */
		(void) fprintf (auth_file, "%s", dummy_auth);
		(void) fflush (auth_file);
		if (ferror (auth_file))
		{
		    err = errno;
		    ret = false;
		}
		/*
		 * Rewind so that the garbage data is overwritten later.
		 */
		rewind(auth_file);
	}
	for (i = 0; i < count; i++)
	{
	    /*
	     * User-based auths may not have data until
	     * a user logs in.  In which case don't write
	     * to the auth file so xrdb and setup programs don't fail.
	     */
	    if (auths[i]->data_length > 0)
	    {
		if (!XauWriteAuth (auth_file, auths[i]))
		{
		    Debug ("XauWriteAuth() failed\n");
		}
		(void) fflush (auth_file);
		if (ferror (auth_file))
		{
		    err = errno;
		    ret = false;
		}
	    }
	}
	/*
	 * XXX: This is not elegant, but stdio has no truncation function.
	 */
	if (ftruncate(fileno(auth_file), ftell(auth_file)))
	{
		Debug ("ftruncate() failed\n");
	}
	fclose (auth_file);

    }
    if (ret == false)
    {
	LogError ("Cannot write to server authorization file %s%s%s\n",
		  d->authFile,
		  err ? ": " : "",
		  err ? _SysErrorMsg (errno) : "");
	free (d->authFile);
	d->authFile = NULL;
    }
    return ret;
}

void
SetLocalAuthorization (struct display *d)
{
    Xauth	*auth, **auths;
    int		i, j;

    if (d->authorizations)
    {
	for (i = 0; i < d->authNum; i++)
	    XauDisposeAuth (d->authorizations[i]);
	free (d->authorizations);
	d->authorizations = (Xauth **) NULL;
	d->authNum = 0;
    }
    if (!d->authNames)
	return;
    for (i = 0; d->authNames[i]; i++)
	;
    d->authNameNum = i;
    free (d->authNameLens);
    d->authNameLens = malloc (d->authNameNum * sizeof (unsigned short));
    if (!d->authNameLens)
	return;
    for (i = 0; i < d->authNameNum; i++)
	d->authNameLens[i] = strlen (d->authNames[i]);
    auths = malloc (d->authNameNum * sizeof (Xauth *));
    if (!auths)
	return;
    j = 0;
    for (i = 0; i < d->authNameNum; i++)
    {
	auth = GenerateAuthorization (d->authNameLens[i], d->authNames[i]);
	if (auth)
	    auths[j++] = auth;
    }
    if (SaveServerAuthorizations (d, auths, j))
    {
	d->authorizations = auths;
	d->authNum = j;
    }
    else
    {
	for (i = 0; i < j; i++)
	    XauDisposeAuth (auths[i]);
	free (auths);
    }
}

/*
 * Set the authorization to use for xdm's initial connection
 * to the X server.  Cannot use user-based authorizations
 * because no one has logged in yet, so we don't have any
 * user credentials.
 */
void
SetAuthorization (struct display *d)
{
    register Xauth **auth = d->authorizations;
    int i;

    for (i = 0; i < d->authNum; i++)
    {
	XSetAuthorization (auth[i]->name, (int) auth[i]->name_length,
			   auth[i]->data, (int) auth[i]->data_length);
    }
}

static int
openFiles (char *name, char *new_name, size_t len, FILE **oldp, FILE **newp)
{
	mode_t	mask;
	int newfd;

        snprintf(new_name, len, "%s-n", name);
	/*
	 * Set safe umask for file creation operations.
	 */
	mask = umask (0077);
	/*
	 * Unlink the authorization file we intend to create, and then open
	 * it with O_CREAT | O_EXCL to avoid race-based symlink attacks.
	 */
	(void) unlink (new_name);
	newfd = open (new_name, O_WRONLY | O_CREAT | O_EXCL, 0600);
	if (newfd >= 0)
	    *newp = fdopen (newfd, "w");
	else
	{
	    LogError ("Cannot create file %s: %s\n", new_name,
		      _SysErrorMsg (errno));
	    *newp = NULL;
	}
	/*
	 * There are no more attempts to create files after this point;
	 * restore the original umask.
	 */
	(void) umask (mask);
	if (!*newp) {
		Debug ("can't open new file %s\n", new_name);
		return 0;
	}
	if (!*oldp)
	    *oldp = fopen (name, "r");
	Debug ("opens succeeded %s %s\n", name, new_name);
	return 1;
}

static void
dumpBytes (unsigned short len, char *data)
{
	unsigned short	i;

	Debug ("%d: ", len);
	for (i = 0; i < len; i++)
		Debug ("%02x ", data[i] & 0377);
	Debug ("\n");
}

static void
dumpAuth (Xauth *auth)
{
	Debug ("family: %d\n", auth->family);
	Debug ("addr:   ");
	dumpBytes (auth->address_length, auth->address);
	Debug ("number: ");
	dumpBytes (auth->number_length, auth->number);
	Debug ("name:   ");
	dumpBytes (auth->name_length, auth->name);
	Debug ("data:   ");
	dumpBytes (auth->data_length, auth->data);
}

struct addrList {
	unsigned short	family;
	unsigned short	address_length;
	char	*address;
	unsigned short	number_length;
	char	*number;
	unsigned short	name_length;
	char	*name;
	struct addrList	*next;
};

static struct addrList	*addrs;

static void
initAddrs (void)
{
	addrs = NULL;
}

static void
doneAddrs (void)
{
	struct addrList	*a, *n;
	for (a = addrs; a; a = n) {
		n = a->next;
		free (a->address);
		free (a->number);
		free (a);
	}
	addrs = NULL;
}

static int checkEntry (Xauth *auth);

static void
saveEntry (Xauth *auth)
{
	struct addrList	*new;

	new = malloc (sizeof (struct addrList));
	if (!new) {
		LogOutOfMem ("saveEntry");
		return;
	}
	if ((new->address_length = auth->address_length) > 0) {
		new->address = malloc (auth->address_length);
		if (!new->address) {
			LogOutOfMem ("saveEntry");
			free (new);
			return;
		}
		memmove( new->address, auth->address, (int) auth->address_length);
	} else
		new->address = NULL;
	if ((new->number_length = auth->number_length) > 0) {
		new->number = malloc (auth->number_length);
		if (!new->number) {
			LogOutOfMem ("saveEntry");
			free (new->address);
			free (new);
			return;
		}
		memmove( new->number, auth->number, (int) auth->number_length);
	} else
		new->number = NULL;
	if ((new->name_length = auth->name_length) > 0) {
		new->name = malloc (auth->name_length);
		if (!new->name) {
			LogOutOfMem ("saveEntry");
			free (new->number);
			free (new->address);
			free (new);
			return;
		}
		memmove( new->name, auth->name, (int) auth->name_length);
	} else
		new->name = NULL;
	new->family = auth->family;
	new->next = addrs;
	addrs = new;
}

static int
checkEntry (Xauth *auth)
{
	struct addrList	*a;

	for (a = addrs; a; a = a->next) {
		if (a->family == auth->family &&
		    a->address_length == auth->address_length &&
		    !memcmp (a->address, auth->address, auth->address_length) &&
		    a->number_length == auth->number_length &&
		    !memcmp (a->number, auth->number, auth->number_length) &&
		    a->name_length == auth->name_length &&
		    !memcmp (a->name, auth->name, auth->name_length))
		{
			return 1;
		}
	}
	return 0;
}

static int  doWrite;

static void
writeAuth (FILE *file, Xauth *auth)
{
    if (debugLevel >= 15) {	/* normally too verbose */
        Debug ("writeAuth: doWrite = %d\n", doWrite);
	dumpAuth (auth);	/* does Debug only */
    }
    if (doWrite)
        XauWriteAuth (file, auth);
}

static void
writeAddr (
    int		family,
    int		addr_length,
    char	*addr,
    FILE	*file,
    Xauth	*auth)
{
	auth->family = (unsigned short) family;
	auth->address_length = addr_length;
	auth->address = addr;
	Debug ("writeAddr: writing and saving an entry\n");
	writeAuth (file, auth);
	saveEntry (auth);
}

static void
DefineLocal (FILE *file, Xauth *auth)
{
	char	displayname[100];
	int	len = _XGetHostname (displayname, sizeof(displayname));

/* Make sure this produces the same string as _XGetHostname in lib/X/XlibInt.c.
 * Otherwise, Xau will not be able to find your cookies in the Xauthority file.
 *
 * Note: POSIX says that the ``nodename'' member of utsname does _not_ have
 *       to have sufficient information for interfacing to the network,
 *       and so, you may be better off using gethostname (if it exists).
 */

	writeAddr (FamilyLocal, len, displayname, file, auth);
}

# include <ifaddrs.h>

static void
DefineSelf(FILE *file, Xauth *auth)
{
    struct ifaddrs *ifap, *ifr;
    char *addr;
    int family, len;

    Debug("DefineSelf\n");
    if (getifaddrs(&ifap) == -1)
	return;
    for (ifr = ifap; ifr != NULL; ifr = ifr->ifa_next) {
	len = sizeof(*(ifr->ifa_addr));
	family = ConvertAddr((XdmcpNetaddr)(ifr->ifa_addr), &len, &addr);
	if (family == -1 || family == FamilyLocal)
	    continue;
	/*
	 * don't write out 'localhost' entries, as
	 * they may conflict with other local entries.
	 * DefineLocal will always be called to add
	 * the local entry anyway, so this one can
	 * be tossed.
	 */
	if (family == FamilyInternet && len == 4 && addr[0] == 127)
	{
	    Debug ("Skipping localhost address\n");
	    continue;
	}
	if(family == FamilyInternet6) {
	    if (IN6_IS_ADDR_LOOPBACK(((struct in6_addr *)addr))) {
		Debug ("Skipping IPv6 localhost address\n");
		continue;
	    }
	    if (IN6_IS_ADDR_LINKLOCAL(((struct in6_addr *)addr))) {
                Debug("Skipping IPv6 link local address\n");
                continue;
            }
	    /* Also skip XDM-AUTHORIZATION-1 */
	    if (auth->name_length == 19 &&
		strcmp(auth->name, "XDM-AUTHORIZATION-1") == 0) {
		Debug ("Skipping IPv6 XDM-AUTHORIZATION-1\n");
		continue;
	    }
	}
	writeAddr(family, len, addr, file, auth);
    }
    freeifaddrs(ifap);
    Debug("DefineSelf done\n");
}

static void
setAuthNumber (Xauth *auth, char *name)
{
    char	*colon;
    char	*dot, *number;

    Debug ("setAuthNumber %s\n", name);
    colon = strrchr(name, ':');
    if (colon) {
	++colon;
	dot = strchr(colon, '.');
	if (dot)
	    auth->number_length = dot - colon;
	else
	    auth->number_length = strlen (colon);
	number = malloc (auth->number_length + 1);
	if (number) {
	    strncpy (number, colon, auth->number_length);
	    number[auth->number_length] = '\0';
	} else {
	    LogOutOfMem ("setAuthNumber");
	    auth->number_length = 0;
	}
	auth->number = number;
	Debug ("setAuthNumber: %s\n", number);
    }
}

static void
writeLocalAuth (FILE *file, Xauth *auth, char *name, int listenTcp)
{

    Debug ("writeLocalAuth: %s %.*s\n", name, auth->name_length, auth->name);
    setAuthNumber (auth, name);
#ifdef TCPCONN
    if (listenTcp)
        DefineSelf (file, auth);
#endif
    DefineLocal (file, auth);
}


void
SetUserAuthorization (struct display *d, struct verify_info *verify)
{
    FILE	*old = NULL, *new;
    char	home_name[1024], new_name[1024];
    char	*name = NULL;
    char	*home;
    char	*envname = NULL;
    int	lockStatus;
    Xauth	*entry, **auths;
    int		setenv = 0;
    struct stat	statb;
    int		i;
    int		magicCookie;
    char	**arg;
    int		foundListen = 0;

    Debug ("SetUserAuthorization\n");
    for (arg = d->argv; *arg!= NULL; arg++) {
        if (strcmp(*arg, "tcp") == 0 && foundListen) {
            Debug("setUserAuthorization: found listenTcp \n");
            d->listenTcp = 1;
            break;
        }
        if (strcmp(*arg, "-listen") == 0)
            foundListen = 1;
        else
            foundListen = 0;
    }
    auths = d->authorizations;
    if (auths) {
	home = getEnv (verify->userEnviron, "HOME");
	lockStatus = LOCK_ERROR;
	if (home) {
	    snprintf (home_name, sizeof(home_name), "%s/.Xauthority", home);
	    Debug ("XauLockAuth %s\n", home_name);
	    lockStatus = XauLockAuth (home_name, 1, 2, 10);
	    Debug ("Lock is %d\n", lockStatus);
	    if (lockStatus == LOCK_SUCCESS) {
                if (openFiles (home_name, new_name, sizeof(new_name), &old, &new)
		    && (new != NULL)) {
		    name = home_name;
		    setenv = 0;
		} else {
		    Debug ("openFiles failed\n");
		    XauUnlockAuth (home_name);
		    lockStatus = LOCK_ERROR;
		    if (old != NULL) {
			(void) fclose (old);
			old = NULL;
		    }
		    if (new != NULL)
			(void) fclose (new);
		}
	    }
	}
	if (lockStatus != LOCK_SUCCESS) {
	    Debug ("can't lock auth file %s\n",
			    home_name);
	    LogError ("can't lock authorization file %s\n",
			    home_name);
	    return;
	}
	initAddrs ();
	doWrite = 1;
	Debug ("%d authorization protocols for %s\n", d->authNum, d->name);
	/*
	 * Write MIT-MAGIC-COOKIE-1 authorization first, so that
	 * R4 clients which only knew that, and used the first
	 * matching entry will continue to function
	 */
	magicCookie = -1;
	for (i = 0; i < d->authNum; i++)
	{
	    if (auths[i]->name_length == 18 &&
		!strncmp (auths[i]->name, "MIT-MAGIC-COOKIE-1", 18))
	    {
		magicCookie = i;
		writeLocalAuth (new, auths[i], d->name, d->listenTcp);
		break;
	    }
	}
	if (old) {
	    if (fstat (fileno (old), &statb) != -1)
		chmod (new_name, (int) (statb.st_mode & 0777));
	    /*SUPPRESS 560*/
	    while ((entry = XauReadAuth (old))) {
		if (!checkEntry (entry))
		{
		    Debug ("Writing an entry\n");
		    writeAuth (new, entry);
		}
		XauDisposeAuth (entry);
	    }
	    fclose (old);
	}
	doneAddrs ();
	fclose (new);
	if (unlink (name) == -1)
	    if (errno != ENOENT)
		LogError ("cannot remove old authorization file %s: %s\n",
			  name, _SysErrorMsg (errno));
	envname = name;
	if (link (new_name, name) == -1) {
	    LogError ("cannot link temporary authorization file %s to old "
		      "location %s: %s\n", new_name, name,
		      _SysErrorMsg (errno));
	    setenv = 1;
	    envname = new_name;
	} else {
	    Debug ("authorization file %s successfully updated\n", name);
	    if (unlink (new_name))
		if (errno != ENOENT)
		    LogError ("cannot remove new authorization file %s:"
			      " %s\n", new_name, _SysErrorMsg (errno));
	}
	if (setenv) {
	    verify->userEnviron = setEnv (verify->userEnviron,
				    "XAUTHORITY", envname);
	    verify->systemEnviron = setEnv (verify->systemEnviron,
				    "XAUTHORITY", envname);
	}
	XauUnlockAuth (name);
	if (envname)
	    chown (envname, verify->uid, verify->gid);
    }
    Debug ("done SetUserAuthorization\n");
}

void
RemoveUserAuthorization (struct display *d, struct verify_info *verify)
{
    char    *home;
    Xauth   **auths, *entry;
    char    name[1024], new_name[1024];
    int	    lockStatus;
    FILE    *old, *new;
    struct stat	statb;
    int	    i;

    if (!(auths = d->authorizations))
	return;
    home = getEnv (verify->userEnviron, "HOME");
    if (!home)
	return;
    Debug ("RemoveUserAuthorization\n");
    snprintf(name, sizeof(name), "%s/.Xauthority", home);
    Debug ("XauLockAuth %s\n", name);
    lockStatus = XauLockAuth (name, 1, 2, 10);
    Debug ("Lock is %d\n", lockStatus);
    if (lockStatus != LOCK_SUCCESS)
	return;
    old = NULL;
    if (openFiles (name, new_name, sizeof(new_name), &old, &new))
    {
	initAddrs ();
	doWrite = 0;
	for (i = 0; i < d->authNum; i++)
            writeLocalAuth (new, auths[i], d->name, d->listenTcp);
	doWrite = 1;
	if (old) {
	    if (fstat (fileno (old), &statb) != -1)
		chmod (new_name, (int) (statb.st_mode & 0777));
	    /*SUPPRESS 560*/
	    while ((entry = XauReadAuth (old))) {
		if (!checkEntry (entry))
		{
		    Debug ("Writing an entry\n");
		    writeAuth (new, entry);
		}
		XauDisposeAuth (entry);
	    }
	    fclose (old);
	}
	doneAddrs ();
	fclose (new);
	if (unlink (name) == -1)
	    if (errno != ENOENT)
		LogError ("cannot remove new authorization file %s: %s\n",
			  name, _SysErrorMsg (errno));
	if (link (new_name, name) == -1) {
	    LogError ("cannot link temporary authorization file %s to old "
		      "location %s: %s\n", new_name, name,
		      _SysErrorMsg (errno));
	} else {
	    Debug ("authorization file %s successfully updated\n", name);
	    if (unlink (new_name))
		if (errno != ENOENT)
		    LogError ("cannot remove new authorization file %s:"
			      " %s\n", new_name, _SysErrorMsg (errno));
	}
    }
    XauUnlockAuth (name);
}
