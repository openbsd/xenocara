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
 * file.c
 */

#include	"dm.h"
#include	"dm_error.h"

#include	<ctype.h>


static void
freeFileArgs (char **args)
{
    char    **a;

    for (a = args; *a; a++)
	free (*a);
    free (args);
}

static char **
splitIntoWords (char *s)
{
    char    **args, **newargs;
    char    *wordStart;
    int	    nargs;

    args = NULL;
    nargs = 0;
    while (*s)
    {
	while (*s && isspace (*s))
	    ++s;
	if (!*s || *s == '#')
	    break;
	wordStart = s;
	while (*s && *s != '#' && !isspace (*s))
	    ++s;
	if (!args)
	{
	    args = malloc (2 * sizeof (char *));
	    if (!args)
		return NULL;
	}
	else
	{
	    newargs = realloc ((char *) args, (nargs+2)*sizeof (char *));
	    if (!newargs)
	    {
		freeFileArgs (args);
		return NULL;
	    }
	    args = newargs;
	}
	args[nargs] = malloc (s - wordStart + 1);
	if (!args[nargs])
	{
	    freeFileArgs (args);
	    return NULL;
	}
	strncpy (args[nargs], wordStart, s - wordStart);
	args[nargs][s-wordStart] = '\0';
	++nargs;
	args[nargs] = NULL;
    }
    return args;
}

static char **
copyArgs (char **args)
{
    char    **a, **new, **n;

    for (a = args; *a; a++)
	/* SUPPRESS 530 */
	;
    new = malloc ((a - args + 1) * sizeof (char *));
    if (!new)
	return NULL;
    n = new;
    a = args;
    /* SUPPRESS 560 */
    while ((*n++ = *a++))
	/* SUPPRESS 530 */
	;
    return new;
}

static void
freeSomeArgs (char **args, int n)
{
    char    **a;

    a = args;
    while (n--)
	free (*a++);
    free (args);
}


static int
isLocal (char *string)
{
	return (strcmp(string, "local") == 0);
}

void
ParseDisplay (char *source)
{
    char		**args, **argv, **a;
    char		*name, *class, *type;
    struct display	*d;
    Boolean		local;

    args = splitIntoWords (source);
    if (!args)
	return;
    if (!args[0])
    {
	LogError ("Missing display name in servers file\n");
	freeFileArgs (args);
	return;
    }
    name = args[0];
    if (!args[1])
    {
	LogError ("Missing display type for %s\n", args[0]);
	freeFileArgs (args);
	return;
    }
    local = isLocal (args[1]);
    class = NULL;
    type = args[1];
    argv = args + 2;
    /*
     * extended syntax; if the second argument doesn't
     * exactly match a legal display type and the third
     * argument does, use the second argument as the
     * display class string
     */
    if (!local && args[2])
    {
	local = isLocal (args[2]);
	class = args[1];
	type = args[2];
	argv = args + 3;
    }
    if (!local)
    {
	LogError ("Unacceptable display type %s for display %s\n",
		  type, name);
    }
    d = FindDisplayByName (name);
    if (d)
    {
	d->state = OldEntry;
	if (class && strcmp (d->class, class))
	{
	    char    *newclass;

	    newclass = strdup (class);
	    if (newclass)
	    {
		free (d->class);
		d->class = newclass;
	    }
	}
	Debug ("Found existing display:  %s %s %s", d->name, d->class , type);
	freeFileArgs (d->argv);
    }
    else
    {
	d = NewDisplay (name, class);
	Debug ("Found new display:  %s %s %s",
		d->name, d->class ? d->class : "", type);
    }
    d->argv = copyArgs (argv);
    for (a = d->argv; a && *a; a++)
	Debug (" %s", *a);
    Debug ("\n");
    freeSomeArgs (args, argv - args);
}
