/*
 * Copyright © 2024 Thomas E. Dickey
 * Copyright © 2002 Keith Packard
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Keith Packard not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Keith Packard makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include "xcursorint.h"
#include <stdlib.h>
#include <string.h>

#ifndef ICONDIR
#define ICONDIR "/usr/X11R6/lib/X11/icons"
#endif

#ifndef XCURSORPATH
#define XCURSORPATH "~/.local/share/icons:~/.icons:/usr/share/icons:/usr/share/pixmaps:"ICONDIR
#endif

typedef struct XcursorInherit {
    char	*line;
    const char	*theme;
} XcursorInherit;

const char *
XcursorLibraryPath (void)
{
    static const char	*path;

    if (!path)
    {
	path = getenv ("XCURSOR_PATH");
	if (!path)
	    path = XCURSORPATH;
	traceOpts((T_OPTION(XCURSOR_PATH) ": %s\n", NonNull(path)));
    }
    return path;
}

static  void
_XcursorAddPathElt (char *path, const char *elt, int len)
{
    size_t    pathlen = strlen (path);

    /* append / if the path doesn't currently have one */
    if (path[0] == '\0' || path[pathlen - 1] != '/')
    {
	strcat (path, "/");
	pathlen++;
    }
    if (len == -1)
	len = (int) strlen (elt);
    /* strip leading slashes */
    while (len && elt[0] == '/')
    {
	elt++;
	len--;
    }
    strncpy (path + pathlen, elt, (size_t) len);
    path[pathlen + (size_t) len] = '\0';
}

static char *
_XcursorBuildThemeDir (const char *dir, const char *theme)
{
    const char	    *colon;
    const char	    *tcolon;
    char	    *full;
    char	    *home;
    int		    dirlen;
    int		    homelen;
    int		    themelen;
    int		    len;

    if (!dir || !theme)
	return NULL;

    colon = strchr (dir, ':');
    if (!colon)
	colon = dir + strlen (dir);

    dirlen = (int) (colon - dir);

    tcolon = strchr (theme, ':');
    if (!tcolon)
	tcolon = theme + strlen (theme);

    themelen = (int) (tcolon - theme);

    home = NULL;
    homelen = 0;
    if (*dir == '~')
    {
	home = getenv ("HOME");
	if (!home)
	    return NULL;
	homelen = (int) strlen (home);
	dir++;
	dirlen--;
    }

    /*
     * add space for any needed directory separators, one per component,
     * and one for the trailing null
     */
    len = 1 + homelen + 1 + dirlen + 1 + themelen + 1;

    full = malloc ((size_t)len);
    if (!full)
	return NULL;
    full[0] = '\0';

    if (home)
	_XcursorAddPathElt (full, home, -1);
    _XcursorAddPathElt (full, dir, dirlen);
    _XcursorAddPathElt (full, theme, themelen);
    return full;
}

static char *
_XcursorBuildFullname (const char *dir, const char *subdir, const char *file)
{
    char    *full;

    if (!dir || !subdir || !file)
	return NULL;

    full = malloc (strlen (dir) + 1 + strlen (subdir) + 1 + strlen (file) + 1);
    if (!full)
	return NULL;
    full[0] = '\0';
    _XcursorAddPathElt (full, dir, -1);
    _XcursorAddPathElt (full, subdir, -1);
    _XcursorAddPathElt (full, file, -1);
    return full;
}

static const char *
_XcursorNextPath (const char *path)
{
    char    *colon = strchr (path, ':');

    if (!colon)
	return NULL;
    return colon + 1;
}

/*
 * _XcursorThemeInherits, XcursorWhite, & XcursorSep are copied in
 * libxcb-cursor/cursor/load_cursor.c.  Please update that copy to
 * include any changes made to the code for those here.
 */

#define XcursorWhite(c)	((c) == ' ' || (c) == '\t' || (c) == '\n')
#define XcursorSep(c) ((c) == ';' || (c) == ',')

static char *
_XcursorThemeInherits (const char *full)
{
    char    line[8192];
    char    *result = NULL;
    FILE    *f;

    if (!full)
	return NULL;

    f = fopen (full, "r" FOPEN_CLOEXEC);
    if (f)
    {
	while (fgets (line, sizeof (line), f))
	{
	    if (!strncmp (line, "Inherits", 8))
	    {
		char    *l = line + 8;
		while (*l == ' ') l++;
		if (*l != '=') continue;
		l++;
		while (*l == ' ') l++;
		result = malloc (strlen (l) + 1);
		if (result)
		{
		    char *r = result;
		    while (*l)
		    {
			while (XcursorSep(*l) || XcursorWhite (*l)) l++;
			if (!*l)
			    break;
			if (r != result)
			    *r++ = ':';
			while (*l && !XcursorWhite(*l) &&
			       !XcursorSep(*l))
			    *r++ = *l++;
		    }
		    *r++ = '\0';
		}
		break;
	    }
	}
	fclose (f);
    }
    return result;
}

#define XCURSOR_SCAN_CORE   ((FILE *) 1)
#define MAX_INHERITS_DEPTH  32

static FILE *
XcursorScanTheme (const char *theme, const char *name)
{
    FILE		*f = NULL;
    char		*full;
    char		*dir;
    const char		*path;
    XcursorInherit	 inherits[MAX_INHERITS_DEPTH + 1];
    int			 d;

    if (!theme || !name)
	return NULL;

    /*
     * XCURSOR_CORE_THEME is a magic name; cursors from the core set
     * are never found in any directory.  Instead, a magic value is
     * returned which truncates any search so that overlying functions
     * can switch to equivalent core cursors
     */
    if (!strcmp (theme, XCURSOR_CORE_THEME) && XcursorLibraryShape (name) >= 0)
	return XCURSOR_SCAN_CORE;

    memset (inherits, 0, sizeof (inherits));

    d = 0;
    inherits[d].theme = theme;

    while (f == NULL && d >= 0 && inherits[d].theme != NULL)
    {
	/*
	 * Scan this theme
	 */
	for (path = XcursorLibraryPath ();
	     path && f == NULL;
	     path = _XcursorNextPath (path))
	{
	    dir = _XcursorBuildThemeDir (path, inherits[d].theme);
	    if (dir)
	    {
		full = _XcursorBuildFullname (dir, "cursors", name);
		if (full)
		{
		    f = fopen (full, "r" FOPEN_CLOEXEC);
		    free (full);
		}
		if (!f && inherits[d + 1].line == NULL)
		{
		    if (d + 1 >= MAX_INHERITS_DEPTH)
		    {
			free (dir);
			goto finish;
		    }
		    full = _XcursorBuildFullname (dir, "", "index.theme");
		    if (full)
		    {
			inherits[d + 1].line = _XcursorThemeInherits (full);
			inherits[d + 1].theme = inherits[d + 1].line;
			free (full);
		    }
		}
		free (dir);
	    }
	}

	d++;
	while (d > 0 && inherits[d].theme == NULL)
	{
	    free (inherits[d].line);
	    inherits[d].line = NULL;

	    if (--d == 0)
		inherits[d].theme = NULL;
	    else
		inherits[d].theme = _XcursorNextPath (inherits[d].theme);
	}

	/*
	 * Detect and break self reference loop early on.
	 */
	if (inherits[d].theme != NULL && strcmp (inherits[d].theme, theme) == 0)
	    break;
    }

finish:
    for (d = 1; d <= MAX_INHERITS_DEPTH; d++)
	free (inherits[d].line);

    return f;
}

XcursorImage *
XcursorLibraryLoadImage (const char *file, const char *theme, int size)
{
    FILE	    *f = NULL;
    XcursorImage    *image = NULL;

    enterFunc((T_CALLED(XcursorLibraryLoadImage) "(\"%s\",\"%s\", %d)\n",
	      NonNull(file), NonNull(theme), size));

    if (!file)
	returnAddr(NULL);

    if (theme)
	f = XcursorScanTheme (theme, file);
    if (!f)
	f = XcursorScanTheme ("default", file);
    if (f != NULL && f != XCURSOR_SCAN_CORE)
    {
	image = XcursorFileLoadImage (f, size);
	fclose (f);
    }
    returnAddr(image);
}

static XcursorImages *
_XcursorLibraryLoadImages (Display *dpy, const char *file)
{
    int		    size = XcursorGetDefaultSize (dpy);
    char	    *theme = XcursorGetTheme (dpy);
    XcursorBool     resized = XcursorGetResizable (dpy);
    FILE	    *f = NULL;
    XcursorImages   *images = NULL;

    if (!file)
	return NULL;

    if (theme)
	f = XcursorScanTheme (theme, file);
    if (!f)
	f = XcursorScanTheme ("default", file);
    if (f != NULL && f != XCURSOR_SCAN_CORE)
    {
	images = _XcursorFileLoadImages (f, size, resized);
	if (images)
	    XcursorImagesSetName (images, file);
	fclose (f);
    }
    return images;
}

XcursorImages *
XcursorLibraryLoadImages (const char *file, const char *theme, int size)
{
    FILE	    *f = NULL;
    XcursorImages   *images = NULL;

    enterFunc((T_CALLED(XcursorLibraryLoadImages) "(\"%s\", \"%s\", %d)\n",
	      NonNull(file), NonNull(theme), size));

    if (!file)
	returnAddr(NULL);

    if (theme)
	f = XcursorScanTheme (theme, file);
    if (!f)
	f = XcursorScanTheme ("default", file);
    if (f != NULL && f != XCURSOR_SCAN_CORE)
    {
	images = XcursorFileLoadImages (f, size);
	if (images)
	    XcursorImagesSetName (images, file);
	fclose (f);
    }
    returnAddr(images);
}

Cursor
XcursorLibraryLoadCursor (Display *dpy, const char *file)
{
    XcursorImages   *images;
    Cursor	    cursor = 0;

    enterFunc((T_CALLED(XcursorLibraryLoadCursor) "(%p, \"%s\")\n",
	      (void*)dpy, NonNull(file)));

    if (!file)
	returnLong(cursor);

    images = _XcursorLibraryLoadImages (dpy, file);
    if (!images)
    {
	int id = XcursorLibraryShape (file);

	if (id >= 0)
	    cursor = _XcursorCreateFontCursor (dpy, (unsigned) id);
    }
    else
    {
	cursor = XcursorImagesLoadCursor (dpy, images);
	XcursorImagesDestroy (images);
#if defined HAVE_XFIXES && XFIXES_MAJOR >= 2
	XFixesSetCursorName (dpy, cursor, file);
#endif
    }
    returnLong(cursor);
}

XcursorCursors *
XcursorLibraryLoadCursors (Display *dpy, const char *file)
{
    XcursorImages   *images;
    XcursorCursors  *cursors;

    enterFunc((T_CALLED(XcursorLibraryLoadCursors) "(%p, \"%s\")\n",
	      (void*)dpy, NonNull(file)));

    if (!file)
	returnAddr(NULL);

    images = _XcursorLibraryLoadImages (dpy, file);
    if (!images)
    {
	int id = XcursorLibraryShape (file);

	if (id >= 0)
	{
	    cursors = XcursorCursorsCreate (dpy, 1);
	    if (cursors)
	    {
		cursors->cursors[0] = _XcursorCreateFontCursor (dpy, (unsigned) id);
		if (cursors->cursors[0] == None)
		{
		    XcursorCursorsDestroy (cursors);
		    cursors = NULL;
		}
		else
		    cursors->ncursor = 1;
	    }
	}
	else
	    cursors = NULL;
    }
    else
    {
	cursors = XcursorImagesLoadCursors (dpy, images);
	XcursorImagesDestroy (images);
    }
    returnAddr(cursors);
}

static const char _XcursorStandardNames[] =
	"X_cursor\0"
	"arrow\0"
	"based_arrow_down\0"
	"based_arrow_up\0"
	"boat\0"
	"bogosity\0"
	"bottom_left_corner\0"
	"bottom_right_corner\0"
	"bottom_side\0"
	"bottom_tee\0"
	"box_spiral\0"
	"center_ptr\0"
	"circle\0"
	"clock\0"
	"coffee_mug\0"
	"cross\0"
	"cross_reverse\0"
	"crosshair\0"
	"diamond_cross\0"
	"dot\0"
	"dotbox\0"
	"double_arrow\0"
	"draft_large\0"
	"draft_small\0"
	"draped_box\0"
	"exchange\0"
	"fleur\0"
	"gobbler\0"
	"gumby\0"
	"hand1\0"
	"hand2\0"
	"heart\0"
	"icon\0"
	"iron_cross\0"
	"left_ptr\0"
	"left_side\0"
	"left_tee\0"
	"leftbutton\0"
	"ll_angle\0"
	"lr_angle\0"
	"man\0"
	"middlebutton\0"
	"mouse\0"
	"pencil\0"
	"pirate\0"
	"plus\0"
	"question_arrow\0"
	"right_ptr\0"
	"right_side\0"
	"right_tee\0"
	"rightbutton\0"
	"rtl_logo\0"
	"sailboat\0"
	"sb_down_arrow\0"
	"sb_h_double_arrow\0"
	"sb_left_arrow\0"
	"sb_right_arrow\0"
	"sb_up_arrow\0"
	"sb_v_double_arrow\0"
	"shuttle\0"
	"sizing\0"
	"spider\0"
	"spraycan\0"
	"star\0"
	"target\0"
	"tcross\0"
	"top_left_arrow\0"
	"top_left_corner\0"
	"top_right_corner\0"
	"top_side\0"
	"top_tee\0"
	"trek\0"
	"ul_angle\0"
	"umbrella\0"
	"ur_angle\0"
	"watch\0"
	"xterm";

static const unsigned short _XcursorStandardNameOffsets[] = {
	0, 9, 15, 32, 47, 52, 61, 80, 100, 112, 123, 134, 145, 152, 158,
	169, 175, 189, 199, 213, 217, 224, 237, 249, 261, 272, 281, 287,
	295, 301, 307, 313, 319, 324, 335, 344, 354, 363, 374, 383, 392,
	396, 409, 415, 422, 429, 434, 449, 459, 470, 480, 492, 501, 510,
	524, 542, 556, 571, 583, 601, 609, 616, 623, 632, 637, 644, 651,
	666, 682, 699, 708, 716, 721, 730, 739, 748, 754
};

#define NUM_STANDARD_NAMES  (sizeof _XcursorStandardNameOffsets / sizeof _XcursorStandardNameOffsets[0])

#define STANDARD_NAME(id) \
    _XcursorStandardNames + _XcursorStandardNameOffsets[id]

XcursorImage *
XcursorShapeLoadImage (unsigned int shape, const char *theme, int size)
{
    unsigned int    id = shape >> 1;
    XcursorImage   *result = NULL;

    enterFunc((T_CALLED(XcursorShapeLoadImage) "(%u, \"%s\", %d)\n",
	      shape, NonNull(theme), size));

    if (id < NUM_STANDARD_NAMES)
	result = XcursorLibraryLoadImage (STANDARD_NAME (id), theme, size);

    returnAddr(result);
}

XcursorImages *
_XcursorShapeLoadImages (Display *dpy, unsigned int shape)
{
    unsigned int    id = shape >> 1;
    XcursorImages  *result = NULL;

    enterFunc((T_CALLED(_XcursorShapeLoadImages) "(%p, %u)\n",
	      (void*)dpy, shape));

    if (id < NUM_STANDARD_NAMES)
	result = _XcursorLibraryLoadImages (dpy, STANDARD_NAME (id));

    returnAddr(result);
}

XcursorImages *
XcursorShapeLoadImages (unsigned int shape, const char *theme, int size)
{
    unsigned int    id = shape >> 1;
    XcursorImages  *result = NULL;

    enterFunc((T_CALLED(XcursorShapeLoadImages) "(%u, \"%s\", %d)\n",
	      shape, NonNull(theme), size));

    if (id < NUM_STANDARD_NAMES)
	result = XcursorLibraryLoadImages (STANDARD_NAME (id), theme, size);

    returnAddr(result);
}

Cursor
XcursorShapeLoadCursor (Display *dpy, unsigned int shape)
{
    unsigned int    id = shape >> 1;
    Cursor          result = None;

    enterFunc((T_CALLED(XcursorShapeLoadCursor) "(%p, %u)\n",
	      (void*)dpy, shape));

    if (id < NUM_STANDARD_NAMES)
	result = XcursorLibraryLoadCursor (dpy, STANDARD_NAME (id));

    returnLong(result);
}

XcursorCursors *
XcursorShapeLoadCursors (Display *dpy, unsigned int shape)
{
    unsigned int    id = shape >> 1;
    XcursorCursors *result = NULL;

    enterFunc((T_CALLED(XcursorShapeLoadCursors) "(%p, %u)\n",
	      (void*)dpy, shape));

    if (id < NUM_STANDARD_NAMES)
	result = XcursorLibraryLoadCursors (dpy, STANDARD_NAME (id));

    returnAddr(result);
}

int
XcursorLibraryShape (const char *library)
{
    int	low, high;

    enterFunc((T_CALLED(XcursorLibraryShape) "(%s)\n", NonNull(library)));

    low = 0;
    high = NUM_STANDARD_NAMES - 1;
    while (low < high - 1)
    {
	int mid = (low + high) >> 1;
	int c = strcmp (library, STANDARD_NAME (mid));
	if (c == 0)
	    returnCode(mid << 1);
	if (c > 0)
	    low = mid;
	else
	    high = mid;
    }
    while (low <= high)
    {
	if (!strcmp (library, STANDARD_NAME (low)))
	    return (low << 1);
	low++;
    }
    returnCode(-1);
}
