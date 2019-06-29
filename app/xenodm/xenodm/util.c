/*

Copyright 1989, 1998  The Open Group

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
 * util.c
 *
 * various utility routines
 */

#include   "dm.h"
#include   "dm_error.h"

#include <X11/Xmu/SysUtil.h>	/* for XmuGetHostname */

#include <signal.h>

void
printEnv (char **e)
{
	while (*e)
		Debug ("%s\n", *e++);
}

static char *
makeEnv (const char *name, const char *value)
{
	char	*result;

	if (asprintf(&result, "%s=%s", name, value) == -1) {
		LogOutOfMem ("makeEnv");
		return NULL;
	}

	return result;
}

char *
getEnv (char **e, const char *name)
{
	int	l = strlen (name);

	if (!e) return NULL;

	while (*e) {
		if ((int)strlen (*e) > l && !strncmp (*e, name, l) &&
			(*e)[l] == '=')
			return (*e) + l + 1;
		++e;
	}
	return NULL;
}

char **
setEnv (char **e, const char *name, const char *value)
{
	char	**new, **old;
	char	*newe;
	int	envsize;
	int	l;

	l = strlen (name);
	newe = makeEnv (name, value);
	if (!newe) {
		LogOutOfMem ("setEnv");
		return e;
	}
	if (e) {
		for (old = e; *old; old++)
			if ((int)strlen (*old) > l && !strncmp (*old, name, l) && (*old)[l] == '=')
				break;
		if (*old) {
			free (*old);
			*old = newe;
			return e;
		}
		envsize = old - e;
		new = realloc ((char *) e,
				(unsigned) ((envsize + 2) * sizeof (char *)));
	} else {
		envsize = 0;
		new = malloc (2 * sizeof (char *));
	}
	if (!new) {
		LogOutOfMem ("setEnv");
		free (newe);
		return e;
	}
	new[envsize] = newe;
	new[envsize+1] = NULL;
	return new;
}

char **
putEnv(const char *string, char **env)
{
    char *v, *b, *n;
    int nl;

    if ((b = strchr(string, '=')) == NULL)
	return NULL;
    v = b + 1;

    nl = b - string;
    if ((n = malloc(nl + 1)) == NULL)
    {
	LogOutOfMem ("putAllEnv");
	return NULL;
    }

    strlcpy(n, string, nl + 1);
    n[nl] = 0;

    env = setEnv(env,n,v);
    free(n);
    return env;
}

void
freeEnv (char **env)
{
    char    **e;

    if (env)
    {
	for (e = env; *e; e++)
	    free (*e);
	free (env);
    }
}

#define isblank(c)	((c) == ' ' || c == '\t')

char **
parseArgs (char **argv, const char *string)
{
	const char *word;
	char	*save;
	char    **newargv;
	int	i;

	i = 0;
	while (argv && argv[i])
		++i;
	if (!argv) {
		argv = malloc (sizeof (char *));
		if (!argv) {
			LogOutOfMem ("parseArgs");
			return NULL;
		}
	}
	word = string;
	for (;;) {
		if (!*string || isblank (*string)) {
			if (word != string) {
				newargv = realloc ((char *) argv,
					(unsigned) ((i + 2) * sizeof (char *)));
				save = malloc ((unsigned) (string - word + 1));
				if (!newargv || !save) {
					LogOutOfMem ("parseArgs");
					free (argv);
					free (newargv);
					free (save);
					return NULL;
				} else {
				    argv = newargv;
				}
				argv[i] = strncpy (save, word, string-word);
				argv[i][string-word] = '\0';
				i++;
			}
			if (!*string)
				break;
			word = string + 1;
		}
		++string;
	}
	argv[i] = NULL;
	return argv;
}

void
freeArgs (char **argv)
{
    char    **a;

    if (!argv)
	return;

    for (a = argv; *a; a++)
	free (*a);
    free (argv);
}

void
CleanUpChild (void)
{
	setsid();
	signal (SIGCHLD, SIG_DFL);
	signal (SIGTERM, SIG_DFL);
	signal (SIGPIPE, SIG_DFL);
	signal (SIGALRM, SIG_DFL);
	signal (SIGHUP, SIG_DFL);
	CloseOnFork ();
}

