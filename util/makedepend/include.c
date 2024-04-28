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

static boolean
isdot(const char *p)
{
    if (p && *p++ == '.' && *p++ == '\0')
        return (TRUE);
    return (FALSE);
}

static boolean
isdotdot(const char *p)
{
    if (p && *p++ == '.' && *p++ == '.' && *p++ == '\0')
        return (TRUE);
    return (FALSE);
}

static boolean
issymbolic(const char *dir, const char *component)
{
#ifdef S_IFLNK
    struct stat st;
    char buf[BUFSIZ], **pp;

    snprintf(buf, sizeof(buf), "%s%s%s", dir, *dir ? "/" : "", component);
    for (pp = notdotdot; *pp; pp++)
        if (strcmp(*pp, buf) == 0)
            return (TRUE);
    if (lstat(buf, &st) == 0 && (st.st_mode & S_IFMT) == S_IFLNK) {
        char *p = strdup(buf);
        if (p == NULL)
            fatalerr("strdup() failure in %s()\n", __func__);
        *pp++ = p;
        if (pp >= &notdotdot[MAXDIRS])
            fatalerr("out of .. dirs, increase MAXDIRS\n");
        return (TRUE);
    }
#endif
    return (FALSE);
}

/*
 * Occasionally, pathnames are created that look like .../x/../y
 * Any of the 'x/..' sequences within the name can be eliminated.
 * (but only if 'x' is not a symbolic link!!)
 */
static void
remove_dotdot(char *path)
{
    char        *end, *from, *to, **cp;
    char        *components[MAXFILES], newpath[BUFSIZ];
    boolean     component_copied;

    /*
     * slice path up into components.
     */
    to = newpath;
    if (*path == '/')
        *to++ = '/';
    *to = '\0';
    cp = components;
    for (from = end = path; *end; end++)
        if (*end == '/') {
            while (*end == '/')
                *end++ = '\0';
            if (*from)
                *cp++ = from;
            from = end;
        }
    *cp++ = from;
    *cp = NULL;

    /*
     * Recursively remove all 'x/..' component pairs.
     */
    cp = components;
    while (*cp) {
        if (!isdot(*cp) && !isdotdot(*cp) && isdotdot(*(cp + 1))
            && !issymbolic(newpath, *cp)) {
            char **fp = cp + 2;
            char **tp = cp;

            do
                *tp++ = *fp;    /* move all the pointers down */
            while (*fp++);
            if (cp != components)
                cp--;           /* go back and check for nested ".." */
        }
        else {
            cp++;
        }
    }
    /*
     * Concatenate the remaining path elements.
     */
    cp = components;
    component_copied = FALSE;
    while (*cp) {
        if (component_copied)
            *to++ = '/';
        component_copied = TRUE;
        for (from = *cp; *from;)
            *to++ = *from++;
        *to = '\0';
        cp++;
    }
    *to++ = '\0';

    /*
     * copy the reconstituted path back to our pointer.
     */
    strcpy(path, newpath);
}

/*
 * Add an include file to the list of those included by 'file'.
 */
struct inclist *
newinclude(const char *newfile, const char *incstring, const char *incpath)
{
    struct inclist *ip;

    /*
     * First, put this file on the global list of include files.
     */
    ip = inclistp++;
    if (inclistp == inclist + MAXFILES - 1)
        fatalerr("out of space: increase MAXFILES\n");
    ip->i_file = strdup(newfile);
    if (ip->i_file == NULL)
            fatalerr("strdup() failure in %s()\n", __func__);

    if (incstring == NULL)
        ip->i_incstring = ip->i_file;
    else {
        ip->i_incstring = strdup(incstring);
        if (ip->i_incstring == NULL)
            fatalerr("strdup() failure in %s()\n", __func__);
    }

    if (incpath == NULL) {
        char r_include[PATHMAX + 1];

        if (realpath(ip->i_file, r_include) == NULL)
            ip->i_realpath = ip->i_file;
        else
            ip->i_realpath = strdup(r_include);
    }
    else {
        ip->i_realpath = strdup(incpath);
    }
    if (ip->i_realpath == NULL)
        fatalerr("strdup() failure in %s()\n", __func__);

    inclistnext = inclistp;
    return (ip);
}

void
included_by(struct inclist *ip, struct inclist *newfile)
{
    if (ip == NULL)
        return;
    /*
     * Put this include file (newfile) on the list of files included
     * by 'file'.  If 'file' is NULL, then it is not an include
     * file itself (i.e. was probably mentioned on the command line).
     * If it is already on the list, don't stick it on again.
     */
    if (ip->i_list == NULL) {
        ip->i_listlen++;
        ip->i_list = mallocarray(ip->i_listlen, sizeof(struct inclist *));
        ip->i_merged = mallocarray(ip->i_listlen, sizeof(boolean));
    }
    else {
        for (unsigned int i = 0; i < ip->i_listlen; i++) {
            if (ip->i_list[i] == newfile) {
                size_t l = strlen(newfile->i_file);
                if (!(ip->i_flags & INCLUDED_SYM) &&
                    !(l > 2 &&
                      newfile->i_file[l - 1] == 'c' &&
                      newfile->i_file[l - 2] == '.')) {
                    /* only bitch if ip has */
                    /* no #include SYMBOL lines  */
                    /* and is not a .c file */
                    if (warn_multiple) {
                        warning("%s includes %s more than once!\n",
                                ip->i_file, newfile->i_file);
                        warning1("Already have\n");
                        for (i = 0; i < ip->i_listlen; i++)
                            warning1("\t%s\n", ip->i_list[i]->i_file);
                    }
                }
                return;
            }
        }
        ip->i_listlen++;
        ip->i_list = reallocarray(ip->i_list, ip->i_listlen,
                                  sizeof(struct inclist *));
        ip->i_merged = reallocarray(ip->i_merged, ip->i_listlen,
                                    sizeof(boolean));
    }
    if ((ip->i_list == NULL) || (ip->i_merged == NULL))
        fatalerr("malloc()/realloc() failure in %s()\n", __func__);

    ip->i_list[ip->i_listlen - 1] = newfile;
    ip->i_merged[ip->i_listlen - 1] = FALSE;
}

void
inc_clean(void)
{
    struct inclist *ip;

    for (ip = inclist; ip < inclistp; ip++) {
        ip->i_flags &= ~MARKED;
    }
}

/*
 * Return full path for the "include" file of the given "type",
 * which may be found relative to the source file "file".
 */
static const char *
find_full_inc_path(const char *file, const char *include, int type)
{
    static char         path[BUFSIZ];
    struct stat         st;

    if (inclistnext == inclist) {
        /*
         * If the path was surrounded by "" or is an absolute path,
         * then check the exact path provided.
         */
        if ((type == INCLUDEDOT) ||
            (type == INCLUDENEXTDOT) ||
            (*include == '/')) {
            if (stat(include, &st) == 0 && !S_ISDIR(st.st_mode))
                return include;
            if (show_where_not)
                warning1("\tnot in %s\n", include);
        }

        /*
         * If the path was surrounded by "" see if this include file is
         * in the directory of the file being parsed.
         */
        if ((type == INCLUDEDOT) || (type == INCLUDENEXTDOT)) {
            const char *p = strrchr(file, '/');

            if ((p == NULL) || (p == file)) {
                strcpy(path, include);
            }
            else {
                strncpy(path, file, (p - file) + 1);
                path[(p - file) + 1] = '\0';
                strcpy(path + (p - file) + 1, include);
            }
            remove_dotdot(path);
            if (stat(path, &st) == 0 && !S_ISDIR(st.st_mode))
                return path;
            if (show_where_not)
                warning1("\tnot in %s\n", path);
        }
    }

    /*
     * Check the include directories specified.  Standard include dirs
     * should be at the end.
     */
    if ((type == INCLUDE) || (type == INCLUDEDOT))
        includedirsnext = includedirs;

    for (const char **pp = includedirsnext; *pp; pp++) {
        snprintf(path, sizeof(path), "%s/%s", *pp, include);
        remove_dotdot(path);
        if (stat(path, &st) == 0 && !S_ISDIR(st.st_mode)) {
            includedirsnext = pp + 1;
            return path;
        }
        if (show_where_not)
            warning1("\tnot in %s\n", path);
    }

    return NULL;
}

struct inclist *
inc_path(const char *file, const char *include, int type)
{
    const char      *fp;
    struct inclist  *ip;
    char r_include[PATHMAX + 1];

    /*
     * Check all previously found include files for a path that
     * has already been expanded.
     */
    if ((type == INCLUDE) || (type == INCLUDEDOT))
        inclistnext = inclist;
    ip = inclistnext;

    fp = find_full_inc_path(file, include, type);
    if (fp == NULL)
        return NULL;
    if (realpath(fp, r_include) == NULL)
        return NULL;

    for (; ip->i_file; ip++) {
        if ((strcmp(ip->i_incstring, include) == 0) &&
            !(ip->i_flags & INCLUDED_SYM)) {
            /*
             * Same filename but same file ?
             */
            if (!strcmp(r_include, ip->i_realpath)) {
                inclistnext = ip + 1;
                return ip;
            }
        }
    }

    return newinclude(fp, include, r_include);
}
