/*

Copyright (c) 1993, 1994, 1998 The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/

#include "def.h"

void
add_include(struct filepointer *filep, struct inclist *file,
	    struct inclist *file_red, const char *include, int type,
	    boolean failOK)
{
	register struct inclist	*newfile;
	register struct filepointer	*content;

	/*
	 * First decide what the pathname of this include file really is.
	 */
	newfile = inc_path(file->i_file, include, type);
	if (newfile == NULL) {
		if (failOK)
		    return;
		if (file != file_red)
			warning("%s (reading %s, line %ld): ",
				file_red->i_file, file->i_file, filep->f_line);
		else
			warning("%s, line %ld: ", file->i_file, filep->f_line);
		warning1("cannot find include file \"%s\"\n", include);
		show_where_not = TRUE;
		newfile = inc_path(file->i_file, include, type);
		show_where_not = FALSE;
	}

	if (newfile) {
		included_by(file, newfile);
		if (!(newfile->i_flags & SEARCHED)) {
			newfile->i_flags |= SEARCHED;
			content = getfile(newfile->i_file);
			find_includes(content, newfile, file_red, 0, failOK);
			freefile(content);
		}
	}
}

/**
 * Replaces all ":" occurrences in @p input with "\:" using @p outputbuffer (of size @p bufsize)
 * possibly to hold the result. @p returns the string with quoted colons
 */
static const char *
quoteColons(const char *input, char *outputbuffer, size_t bufsize)
{
        const char *tmp=input;
        const char *loc;
        char *output=outputbuffer;

        loc = strchr(input, ':');
        if (loc == NULL) {
                return input;
        }

        tmp=input;
        while (loc != NULL && bufsize > loc-tmp+2 ) {
                memcpy(output, tmp, loc-tmp);
                output+=loc-tmp;
                bufsize-=loc-tmp+2;
                tmp=loc+1;
                *output='\\';
                output++;
                *output=':';
                output++;
                loc = strchr(tmp, ':');
        }

        if (strlen(tmp) <= bufsize)
           strcpy(output, tmp);
        else {
           strncpy(output, tmp, bufsize-1);
           output[bufsize]=0;
        }
        return outputbuffer;
}

static void
pr(struct inclist *ip, const char *file, const char *base)
{
	static const char *lastfile;
	static int	current_len;
	register int	len, i;
	const char *	quoted;
	char	quotebuf[ BUFSIZ ];

	printed = TRUE;
	quoted = quoteColons(ip->i_file, quotebuf, sizeof(quotebuf));
	len = strlen(quoted)+1;
	if (current_len + len > width || file != lastfile) {
		lastfile = file;
		current_len = fprintf(stdout, "\n%s%s%s: %s",
			 objprefix, base, objsuffix, quoted);
	}
	else {
		fprintf(stdout, " %s", quoted);
		current_len += len;
	}

	/*
	 * If verbose is set, then print out what this file includes.
	 */
	if (! verbose || ip->i_list == NULL || ip->i_flags & NOTIFIED)
		return;
	ip->i_flags |= NOTIFIED;
	lastfile = NULL;
	printf("\n# %s includes:", ip->i_file);
	for (i=0; i<ip->i_listlen; i++)
		printf("\n#\t%s", ip->i_list[ i ]->i_incstring);
}

void
recursive_pr_include(struct inclist *head, const char *file, const char *base)
{
	int	i;

	if (head->i_flags & MARKED)
		return;
	head->i_flags |= MARKED;
	if (head->i_file != file)
		pr(head, file, base);
	for (i=0; i<head->i_listlen; i++)
		recursive_pr_include(head->i_list[ i ], file, base);
}
