/* $XTermId: fontutils.c,v 1.786 2024/09/30 07:35:30 tom Exp $ */

/*
 * Copyright 1998-2023,2024 by Thomas E. Dickey
 *
 *                         All Rights Reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization.
 */

/*
 * A portion of this module (for FontNameProperties) was adapted from EMU 1.3;
 * it constructs font names with specific properties changed, e.g., for bold
 * and double-size characters.
 */

#define RES_OFFSET(field)	XtOffsetOf(SubResourceRec, field)

#include <fontutils.h>
#include <X11/Xmu/Drawing.h>
#include <X11/Xmu/CharSet.h>

#include <main.h>
#include <data.h>
#include <error.h>
#include <menu.h>
#include <xstrings.h>
#include <xterm.h>

#include <stdio.h>
#include <ctype.h>

#define USE_FC_COLOR 0
#if OPT_RENDERFONT
#if XftVersion > 20304
#undef USE_FC_COLOR
#define USE_FC_COLOR 1
#endif
#endif

#define FcOK(func) (func == FcResultMatch)

#define NoFontWarning(data) (data)->warn = fwAlways

#define SetFontWidth(screen,dst,src)  (dst)->f_width = (src)
#define SetFontHeight(screen,dst,src) (dst)->f_height = dimRound((double)((screen)->scale_height * (float) (src)))

/* from X11/Xlibint.h - not all vendors install this file */
#define CI_NONEXISTCHAR(cs) (((cs)->width == 0) && \
			     (((cs)->rbearing|(cs)->lbearing| \
			       (cs)->ascent|(cs)->descent) == 0))

#define CI_GET_CHAR_INFO_1D(fs,col,cs) \
{ \
    cs = 0; \
    if (col >= fs->min_char_or_byte2 && col <= fs->max_char_or_byte2) { \
	if (fs->per_char == NULL) { \
	    cs = &fs->min_bounds; \
	} else { \
	    cs = &fs->per_char[(col - fs->min_char_or_byte2)]; \
	} \
	if (CI_NONEXISTCHAR(cs)) cs = 0; \
    } \
}

#define CI_GET_CHAR_INFO_2D(fs,row,col,cs) \
{ \
    cs = 0; \
    if (row >= fs->min_byte1 && row <= fs->max_byte1 && \
	col >= fs->min_char_or_byte2 && col <= fs->max_char_or_byte2) { \
	if (fs->per_char == NULL) { \
	    cs = &fs->min_bounds; \
	} else { \
	    cs = &fs->per_char[((row - fs->min_byte1) * \
				(fs->max_char_or_byte2 - \
				 fs->min_char_or_byte2 + 1)) + \
			       (col - fs->min_char_or_byte2)]; \
	} \
	if (CI_NONEXISTCHAR(cs)) cs = 0; \
    } \
}

#define FREE_FNAME(field) \
	    if (fonts == 0 || new_fnames.field != fonts->field) { \
		FREE_STRING(new_fnames.field); \
		new_fnames.field = 0; \
	    }

/*
 * A structure to hold the relevant properties from a font
 * we need to make a well formed font name for it.
 */
typedef struct {
    /* registry, foundry, family */
    const char *beginning;
    /* weight */
    const char *weight;
    /* slant */
    const char *slant;
    /* wideness */
    const char *wideness;
    /* add style */
    const char *add_style;
    int pixel_size;
    const char *point_size;
    int res_x;
    int res_y;
    const char *spacing;
    int average_width;
    /* charset registry, charset encoding */
    char *end;
} FontNameProperties;

#if OPT_WIDE_CHARS && (OPT_RENDERFONT || (OPT_TRACE > 1))
#define MY_UCS(code,high,wide,name) { code, high, wide, #name }
static const struct {
    unsigned code, high, wide;
    const char *name;
} unicode_boxes[] = {

    MY_UCS(0x2500, 0, 1, box drawings light horizontal),
	MY_UCS(0x2502, 1, 0, box drawings light vertical),
	MY_UCS(0x250c, 2, 2, box drawings light down and right),
	MY_UCS(0x2510, 2, 2, box drawings light down and left),
	MY_UCS(0x2514, 2, 2, box drawings light up and right),
	MY_UCS(0x2518, 2, 2, box drawings light up and left),
	MY_UCS(0x251c, 1, 2, box drawings light vertical and right),
	MY_UCS(0x2524, 1, 2, box drawings light vertical and left),
	MY_UCS(0x252c, 2, 1, box drawings light down and horizontal),
	MY_UCS(0x2534, 2, 1, box drawings light up and horizontal),
	MY_UCS(0x253c, 1, 1, box drawings light vertical and horizontal),
    {
	0, 0, 0, NULL
    }
};

#undef MY_UCS
#endif /* OPT_WIDE_CHARS */

#if OPT_LOAD_VTFONTS || OPT_WIDE_CHARS
static Boolean merge_sublist(char ***, char **);
#endif

static void save2FontList(XtermWidget, const char *, XtermFontNames *,
			  VTFontEnum, const char *, Bool, Bool);

#if OPT_RENDERFONT
static void fillInFaceSize(XtermWidget, int);
#endif

#if OPT_REPORT_FONTS && OPT_TRACE
static void
report_fonts(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
#if OPT_TRACE
    va_start(ap, fmt);
    TraceVA(fmt, ap);
    va_end(ap);
#endif
}

#define ReportFonts report_fonts
#else
#define ReportFonts printf
#endif

#if OPT_TRACE
static void
set_font_height(TScreen *screen, VTwin *win, int height)
{
    SetFontHeight(screen, win, height);
    TRACE(("SetFontHeight %d\n", win->f_height));
#undef SetFontHeight
#define SetFontHeight(screen, win, height) set_font_height(screen, win, height)
}

static void
set_font_width(TScreen *screen, VTwin *win, int width)
{
    (void) screen;
    SetFontWidth(screen, win, width);
    TRACE(("SetFontWidth  %d\n", win->f_width));
#undef  SetFontWidth
#define SetFontWidth(screen, win, width) set_font_width(screen, win, width)
}
#endif

#if OPT_REPORT_FONTS || OPT_WIDE_CHARS
static unsigned
countGlyphs(XFontStruct *fp)
{
    unsigned count = 0;

    if (fp != 0) {
	if (fp->min_byte1 == 0 && fp->max_byte1 == 0) {
	    count = fp->max_char_or_byte2 - fp->min_char_or_byte2 + 1;
	} else if (fp->min_char_or_byte2 < 256
		   && fp->max_char_or_byte2 < 256) {
	    unsigned first = (fp->min_byte1 << 8) + fp->min_char_or_byte2;
	    unsigned last = (fp->max_byte1 << 8) + fp->max_char_or_byte2;
	    count = last + 1 - first;
	}
    }
    return count;
}
#endif

#if OPT_WIDE_CHARS
/*
 * Verify that the wide-bold font is at least a bold font with roughly as many
 * glyphs as the wide font.  The counts should be the same, but settle for
 * filtering out the worst of the font mismatches.
 */
static Bool
compatibleWideCounts(XFontStruct *wfs, XFontStruct *wbfs)
{
    unsigned count_w = countGlyphs(wfs);
    unsigned count_wb = countGlyphs(wbfs);
    if (count_w <= 256 ||
	count_wb <= 256 ||
	((count_w / 4) * 3) > count_wb) {
	TRACE(("...font server lied (count wide %u vs wide-bold %u)\n",
	       count_w, count_wb));
	return False;
    }
    return True;
}
#endif /* OPT_WIDE_CHARS */

#if OPT_BOX_CHARS
static void
setupPackedFonts(XtermWidget xw)
{
    TScreen *screen = TScreenOf(xw);
    Bool value = False;

#if OPT_RENDERFONT
    if (xw->work.render_font == True) {
	int e;

	for (e = 0; e < fMAX; ++e) {
	    XTermXftFonts *data = getMyXftFont(xw, e, screen->menu_font_number);
	    if (data != 0) {
		if (data->font_info.mixed) {
		    screen->allow_packing = True;
		    break;
		}
	    }
	}
    }
#endif /* OPT_RENDERFONT */

    value = screen->allow_packing;

    SetItemSensitivity(fontMenuEntries[fontMenu_font_packedfont].widget, value);
}
#endif

typedef struct _nameList {
    struct _nameList *next;
    char *name;
} NameList;

static NameList *derived_fonts;

static Boolean
is_derived_font_name(const char *name)
{
    Boolean result = False;
    NameList *list;
    if (!IsEmpty(name)) {
	for (list = derived_fonts; list != 0; list = list->next) {
	    if (!x_strcasecmp(name, list->name)) {
		result = True;
		break;
	    }
	}
    }
    return result;
}

void
xtermDerivedFont(const char *name)
{
    if (!IsEmpty(name) && !is_derived_font_name(name)) {
	NameList *list = TypeCalloc(NameList);
	list->name = x_strdup(name);
	list->next = derived_fonts;
	derived_fonts = list;
    }
}

/*
 * Returns the fields from start to stop in a dash- separated string.  This
 * function will modify the source, putting '\0's in the appropriate place and
 * moving the beginning forward to after the '\0'
 *
 * This will NOT work for the last field (but we won't need it).
 */
static char *
n_fields(char **source, int start, int stop)
{
    int i;
    char *str, *str1;

    /*
     * find the start-1th dash
     */
    for (i = start - 1, str = *source; i; i--, str++) {
	if ((str = strchr(str, '-')) == 0)
	    return 0;
    }

    /*
     * find the stopth dash
     */
    for (i = stop - start + 1, str1 = str; i; i--, str1++) {
	if ((str1 = strchr(str1, '-')) == 0)
	    return 0;
    }

    /*
     * put a \0 at the end of the fields
     */
    *(str1 - 1) = '\0';

    /*
     * move source forward
     */
    *source = str1;

    return str;
}

static Boolean
check_fontname(const char *name)
{
    Boolean result = True;

    if (IsEmpty(name)) {
	TRACE(("fontname missing\n"));
	result = False;
    }
    return result;
}

/*
 * Gets the font properties from a given font structure.  We use the FONT name
 * to find them out, since that seems easier.
 *
 * Returns a pointer to a static FontNameProperties structure
 * or NULL on error.
 */
static FontNameProperties *
get_font_name_props(Display *dpy, XFontStruct *fs, char **result)
{
    static FontNameProperties props;
    static char *last_name;

    Atom fontatom;
    char *name;
    char *str;

    if (fs == NULL)
	return NULL;

    /*
     * first get the full font name
     */
    name = 0;
    fontatom = CachedInternAtom(dpy, "FONT");
    if (fontatom != 0) {
	XFontProp *fp;
	int i;

	for (i = 0, fp = fs->properties; i < fs->n_properties; i++, fp++) {
	    if (fp->name == fontatom) {
		name = XGetAtomName(dpy, fp->card32);
		break;
	    }
	}
    }

    if (name == 0)
	return 0;

    /*
     * XGetAtomName allocates memory - don't leak
     */
    XFree(last_name);
    last_name = name;

    if (result != 0) {
	if (!check_fontname(name))
	    return 0;
	free(*result);
	*result = x_strdup(name);
    }

    /*
     * Now split it up into parts and put them in
     * their places. Since we are using parts of
     * the original string, we must not free the Atom Name
     */

    /* registry, foundry, family */
    if ((props.beginning = n_fields(&name, 1, 3)) == 0)
	return 0;

    /* weight is the next */
    if ((props.weight = n_fields(&name, 1, 1)) == 0)
	return 0;

    /* slant */
    if ((props.slant = n_fields(&name, 1, 1)) == 0)
	return 0;

    /* width */
    if ((props.wideness = n_fields(&name, 1, 1)) == 0)
	return 0;

    /* add style */
    if ((props.add_style = n_fields(&name, 1, 1)) == 0)
	return 0;

    /* pixel size */
    if ((str = n_fields(&name, 1, 1)) == 0)
	return 0;
    if ((props.pixel_size = atoi(str)) == 0)
	return 0;

    /* point size */
    if ((props.point_size = n_fields(&name, 1, 1)) == 0)
	return 0;

    /* res_x */
    if ((str = n_fields(&name, 1, 1)) == 0)
	return 0;
    if ((props.res_x = atoi(str)) == 0)
	return 0;

    /* res_y */
    if ((str = n_fields(&name, 1, 1)) == 0)
	return 0;
    if ((props.res_y = atoi(str)) == 0)
	return 0;

    /* spacing */
    if ((props.spacing = n_fields(&name, 1, 1)) == 0)
	return 0;

    /* average width */
    if ((str = n_fields(&name, 1, 1)) == 0)
	return 0;
    if ((props.average_width = atoi(str)) == 0)
	return 0;

    /* the rest: charset registry and charset encoding */
    props.end = name;

    return &props;
}

#define ALLOCHUNK(n) ((n | 127) + 1)

static void
alloca_fontname(char **result, size_t next)
{
    size_t last = (*result != 0) ? strlen(*result) : 0;
    size_t have = (*result != 0) ? ALLOCHUNK(last) : 0;
    size_t want = last + next + 2;

    if (want >= have) {
	char *save = *result;
	want = ALLOCHUNK(want);
	if (last != 0) {
	    *result = TypeRealloc(char, want, *result);
	    if (*result == 0)
		free(save);
	} else {
	    if ((*result = TypeMallocN(char, want)) != 0) {
		free(save);
		**result = '\0';
	    }
	}
    }
}

static void
append_fontname_str(char **result, const char *value)
{
    if (value == 0)
	value = "*";
    alloca_fontname(result, strlen(value));
    if (*result != 0) {
	if (**result != '\0')
	    strcat(*result, "-");
	strcat(*result, value);
    }
}

static void
append_fontname_num(char **result, int value)
{
    if (value < 0) {
	append_fontname_str(result, "*");
    } else {
	char temp[100];
	sprintf(temp, "%d", value);
	append_fontname_str(result, temp);
    }
}

/*
 * Take the given font props and try to make a well formed font name specifying
 * the same base font and size and everything, but with different weight/width
 * according to the parameters.  The return value is allocated, should be freed
 * by the caller.
 */
static char *
derive_font_name(FontNameProperties *props,
		 const char *use_weight,
		 int use_average_width,
		 const char *use_encoding)
{
    char *result = 0;

    append_fontname_str(&result, props->beginning);
    append_fontname_str(&result, use_weight);
    append_fontname_str(&result, props->slant);
    append_fontname_str(&result, 0);
    append_fontname_str(&result, 0);
    append_fontname_num(&result, props->pixel_size);
    append_fontname_str(&result, props->point_size);
    append_fontname_num(&result, props->res_x);
    append_fontname_num(&result, props->res_y);
    append_fontname_str(&result, props->spacing);
    append_fontname_num(&result, use_average_width);
    append_fontname_str(&result, use_encoding);

    xtermDerivedFont(result);
    return result;
}

static char *
bold_font_name(FontNameProperties *props, int use_average_width)
{
    return derive_font_name(props, "bold", use_average_width, props->end);
}

#if OPT_WIDE_ATTRS
static char *
italic_font_name(FontNameProperties *props, const char *slant)
{
    FontNameProperties myprops = *props;
    myprops.slant = slant;
    return derive_font_name(&myprops, props->weight, myprops.average_width, props->end);
}

static Boolean
open_italic_font(XtermWidget xw, int n, FontNameProperties *fp, XTermFonts * data)
{
    static const char *const slant[] =
    {
	"o",
	"i"
    };
    Cardinal pass;
    Boolean result = False;

    NoFontWarning(data);
    for (pass = 0; pass < XtNumber(slant); ++pass) {
	char *name;
	if ((name = italic_font_name(fp, slant[pass])) != 0) {
	    TRACE(("open_italic_font %s %s\n",
		   whichFontEnum((VTFontEnum) n), name));
	    if (xtermOpenFont(xw, name, data, NULL, False)) {
		result = (data->fs != 0);
#if OPT_REPORT_FONTS
		if (resource.reportFonts) {
		    ReportFonts("opened italic version of %s:\n\t%s\n",
				whichFontEnum((VTFontEnum) n),
				name);
		}
#endif
	    }
	    free(name);
	    if (result)
		break;
	}
    }
#if OPT_TRACE
    if (result) {
	XFontStruct *fs = data->fs;
	if (fs != 0) {
	    TRACE(("...actual size %dx%d (ascent %d, descent %d)\n",
		   fs->ascent +
		   fs->descent,
		   fs->max_bounds.width,
		   fs->ascent,
		   fs->descent));
	}
    }
#endif
    return result;
}
#endif

#if OPT_WIDE_CHARS
#define derive_wide_font(props, weight) \
	derive_font_name(props, weight, props->average_width * 2, "ISO10646-1")

static char *
wide_font_name(FontNameProperties *props)
{
    return derive_wide_font(props, "medium");
}

static char *
widebold_font_name(FontNameProperties *props)
{
    return derive_wide_font(props, "bold");
}
#endif /* OPT_WIDE_CHARS */

#if OPT_DEC_CHRSET
/*
 * Take the given font props and try to make a well formed font name specifying
 * the same base font but changed depending on the given attributes and chrset.
 *
 * For double width fonts, we just double the X-resolution, for double height
 * fonts we double the pixel-size and Y-resolution
 */
char *
xtermSpecialFont(XTermDraw * params)
{
    TScreen *screen = TScreenOf(params->xw);
#if OPT_TRACE
    static char old_spacing[80];
    static FontNameProperties old_props;
#endif
    FontNameProperties *props;
    char *result = 0;
    const char *weight;
    int pixel_size;
    int res_x;
    int res_y;

    props = get_font_name_props(screen->display,
				GetNormalFont(screen, fNorm)->fs, 0);
    if (props == 0)
	return result;

    pixel_size = props->pixel_size;
    res_x = props->res_x;
    res_y = props->res_y;
    if (params->attr_flags & BOLD)
	weight = "bold";
    else
	weight = props->weight;

    if (CSET_DOUBLE(params->this_chrset))
	res_x *= 2;

    if (params->this_chrset == CSET_DHL_TOP
	|| params->this_chrset == CSET_DHL_BOT) {
	res_y *= 2;
	pixel_size *= 2;
    }
#if OPT_TRACE
    if (old_props.res_x != res_x
	|| old_props.res_x != res_y
	|| old_props.pixel_size != pixel_size
	|| strcmp(old_props.spacing, props->spacing)) {
	TRACE(("xtermSpecialFont(atts = %#x, draw = %#x, chrset = %#x)\n",
	       params->attr_flags, params->draw_flags, params->this_chrset));
	TRACE(("res_x      = %d\n", res_x));
	TRACE(("res_y      = %d\n", res_y));
	TRACE(("point_size = %s\n", props->point_size));
	TRACE(("pixel_size = %d\n", pixel_size));
	TRACE(("spacing    = %s\n", props->spacing));
	old_props.res_x = res_x;
	old_props.res_y = res_y;
	old_props.pixel_size = pixel_size;
	old_props.spacing = old_spacing;
	sprintf(old_spacing, "%.*s", (int) sizeof(old_spacing) - 2, props->spacing);
    }
#endif

    append_fontname_str(&result, props->beginning);
    append_fontname_str(&result, weight);
    append_fontname_str(&result, props->slant);
    append_fontname_str(&result, props->wideness);
    append_fontname_str(&result, props->add_style);
    append_fontname_num(&result, pixel_size);
    append_fontname_str(&result, props->point_size);
    append_fontname_num(&result, (params->draw_flags & NORESOLUTION) ? -1 : res_x);
    append_fontname_num(&result, (params->draw_flags & NORESOLUTION) ? -1 : res_y);
    append_fontname_str(&result, props->spacing);
    append_fontname_str(&result, 0);
    append_fontname_str(&result, props->end);

    xtermDerivedFont(result);
    return result;
}
#endif /* OPT_DEC_CHRSET */

/*
 * Case-independent comparison for font-names, including wildcards.
 * XLFD allows '?' as a wildcard, but we do not handle that (no one seems
 * to use it).
 */
static Bool
same_font_name(const char *pattern, const char *match)
{
    Bool result = False;

    if (pattern && match) {
	while (*pattern && *match) {
	    if (*pattern == *match) {
		pattern++;
		match++;
	    } else if (*pattern == '*' || *match == '*') {
		if (same_font_name(pattern + 1, match)) {
		    return True;
		} else if (same_font_name(pattern, match + 1)) {
		    return True;
		} else {
		    return False;
		}
	    } else {
		int p = x_toupper(*pattern++);
		int m = x_toupper(*match++);
		if (p != m)
		    return False;
	    }
	}
	result = (*pattern == *match);	/* both should be NUL */
    }
    return result;
}

/*
 * Double-check the fontname that we asked for versus what the font server
 * actually gave us.  The larger fixed fonts do not always have a matching bold
 * font, and the font server may try to scale another font or otherwise
 * substitute a mismatched font.
 *
 * If we cannot get what we requested, we will fallback to the original
 * behavior, which simulates bold by overstriking each character at one pixel
 * offset.
 */
static int
got_bold_font(Display *dpy, XFontStruct *fs, String requested)
{
    char *actual = 0;
    int got;

    if (get_font_name_props(dpy, fs, &actual) == 0)
	got = 0;
    else
	got = same_font_name(requested, actual);
    free(actual);
    return got;
}

/*
 * Check normal/bold (or wide/wide-bold) font pairs to see if we will be able
 * to check for missing glyphs in a comparable manner.
 */
static int
comparable_metrics(XFontStruct *normal, XFontStruct *bold)
{
#define DATA "comparable_metrics: "
    int result = 0;

    if (normal == 0 || bold == 0) {
	;
    } else if (normal->all_chars_exist) {
	if (bold->all_chars_exist) {
	    result = 1;
	} else {
	    TRACE((DATA "all chars exist in normal font, but not in bold\n"));
	}
    } else if (normal->per_char != 0) {
	if (bold->per_char != 0) {
	    result = 1;
	} else {
	    TRACE((DATA "normal font has per-char metrics, but not bold\n"));
	}
    } else {
	TRACE((DATA "normal font is not very good!\n"));
	result = 1;		/* give in (we're not going in reverse) */
    }
    return result;
#undef DATA
}

/*
 * If the font server tries to adjust another font, it may not adjust it
 * properly.  Check that the bounding boxes are compatible.  Otherwise we'll
 * leave trash on the display when we mix normal and bold fonts.
 */
static int
same_font_size(XtermWidget xw, XFontStruct *nfs, XFontStruct *bfs)
{
    TScreen *screen = TScreenOf(xw);
    int result = 0;

    if (nfs != 0 && bfs != 0) {
	TRACE(("same_font_size height %d/%d, min %d/%d max %d/%d\n",
	       nfs->ascent + nfs->descent,
	       bfs->ascent + bfs->descent,
	       nfs->min_bounds.width, bfs->min_bounds.width,
	       nfs->max_bounds.width, bfs->max_bounds.width));
	result = screen->free_bold_box
	    || ((nfs->ascent + nfs->descent) == (bfs->ascent + bfs->descent)
		&& (nfs->min_bounds.width == bfs->min_bounds.width
		    || nfs->min_bounds.width == bfs->min_bounds.width + 1)
		&& (nfs->max_bounds.width == bfs->max_bounds.width
		    || nfs->max_bounds.width == bfs->max_bounds.width + 1));
    }
    return result;
}

/*
 * Check if the font looks like it has fixed width
 */
static int
is_fixed_font(XFontStruct *fs)
{
    if (fs)
	return (fs->min_bounds.width == fs->max_bounds.width);
    return 1;
}

static int
differing_widths(XFontStruct *a, XFontStruct *b)
{
    int result = 0;
    if (a != NULL && b != NULL && a->max_bounds.width != b->max_bounds.width)
	result = 1;
    return result;
}

/*
 * Check if the font looks like a double width font (i.e. contains
 * characters of width X and 2X
 */
#if OPT_WIDE_CHARS
static int
is_double_width_font(XFontStruct *fs)
{
    return (fs != NULL && ((2 * fs->min_bounds.width) == fs->max_bounds.width));
}
#else
#define is_double_width_font(fs) 0
#endif

#if OPT_WIDE_CHARS && OPT_RENDERFONT && defined(HAVE_TYPE_FCCHAR32)
#define HALF_WIDTH_TEST_STRING "1234567890"

/* '1234567890' in Chinese characters in UTF-8 */
#define FULL_WIDTH_TEST_STRING "\xe4\xb8\x80\xe4\xba\x8c\xe4\xb8\x89" \
                               "\xe5\x9b\x9b\xe4\xba\x94" \
			       "\xef\xa7\x91\xe4\xb8\x83\xe5\x85\xab" \
			       "\xe4\xb9\x9d\xef\xa6\xb2"

/* '1234567890' in Korean script in UTF-8 */
#define FULL_WIDTH_TEST_STRING2 "\xec\x9d\xbc\xec\x9d\xb4\xec\x82\xbc" \
                                "\xec\x82\xac\xec\x98\xa4" \
			        "\xec\x9c\xa1\xec\xb9\xa0\xed\x8c\x94" \
			        "\xea\xb5\xac\xec\x98\x81"

#define HALF_WIDTH_CHAR1  0x0031	/* '1' */
#define HALF_WIDTH_CHAR2  0x0057	/* 'W' */
#define FULL_WIDTH_CHAR1  0x4E00	/* CJK Ideograph 'number one' */
#define FULL_WIDTH_CHAR2  0xAC00	/* Korean script syllable 'Ka' */

static Bool
is_double_width_font_xft(Display *dpy, XftFont *font)
{
    XGlyphInfo gi1, gi2;
    FcChar32 c1 = HALF_WIDTH_CHAR1, c2 = HALF_WIDTH_CHAR2;
    String fwstr = FULL_WIDTH_TEST_STRING;
    String hwstr = HALF_WIDTH_TEST_STRING;

    /* Some Korean fonts don't have Chinese characters at all. */
    if (!XftCharExists(dpy, font, FULL_WIDTH_CHAR1)) {
	if (!XftCharExists(dpy, font, FULL_WIDTH_CHAR2))
	    return False;	/* Not a CJK font */
	else			/* a Korean font without CJK Ideographs */
	    fwstr = FULL_WIDTH_TEST_STRING2;
    }

    XftTextExtents32(dpy, font, &c1, 1, &gi1);
    XftTextExtents32(dpy, font, &c2, 1, &gi2);
    if (gi1.xOff != gi2.xOff)	/* Not a fixed-width font */
	return False;

    XftTextExtentsUtf8(dpy,
		       font,
		       (_Xconst FcChar8 *) hwstr,
		       (int) strlen(hwstr),
		       &gi1);
    XftTextExtentsUtf8(dpy,
		       font,
		       (_Xconst FcChar8 *) fwstr,
		       (int) strlen(fwstr),
		       &gi2);

    /*
     * fontconfig and Xft prior to 2.2(?) set the width of half-width
     * characters identical to that of full-width character in CJK double-width
     * (bi-width / monospace) font even though the former is half as wide as
     * the latter.  This was fixed sometime before the release of fontconfig
     * 2.2 in early 2003.  See
     *  http://bugzilla.mozilla.org/show_bug.cgi?id=196312
     * In the meantime, we have to check both possibilities.
     */
    return ((2 * gi1.xOff == gi2.xOff) || (gi1.xOff == gi2.xOff));
}
#else
#define is_double_width_font_xft(dpy, xftfont) 0
#endif

#define EmptyFont(fs) (fs != 0 \
		   && ((fs)->ascent + (fs)->descent == 0 \
		    || (fs)->max_bounds.width == 0))

#define FontSize(fs) (((fs)->ascent + (fs)->descent) \
		    *  (fs)->max_bounds.width)

const VTFontNames *
xtermFontName(const char *normal)
{
    static VTFontNames data;
    FREE_STRING(data.f_n);
    memset(&data, 0, sizeof(data));
    if (normal)
	data.f_n = x_strdup(normal);
    return &data;
}

const VTFontNames *
defaultVTFontNames(XtermWidget xw)
{
    static VTFontNames data;
    memset(&data, 0, sizeof(data));
    data.f_n = DefaultFontN(xw);
    data.f_b = DefaultFontB(xw);
#if OPT_WIDE_CHARS
    data.f_w = DefaultFontW(xw);
    data.f_wb = DefaultFontWB(xw);
#endif
    return &data;
}

static void
cache_menu_font_name(TScreen *screen, int fontnum, int which, const char *name)
{
    if (name != 0) {
	String last = screen->menu_font_names[fontnum][which];
	if (last != 0) {
	    if (strcmp(last, name)) {
		FREE_STRING(last);
		TRACE(("caching menu fontname %d.%d %s\n", fontnum, which, name));
		screen->menu_font_names[fontnum][which] = x_strdup(name);
	    }
	} else {
	    TRACE(("caching menu fontname %d.%d %s\n", fontnum, which, name));
	    screen->menu_font_names[fontnum][which] = x_strdup(name);
	}
    }
}

static void
cannotFont(XtermWidget xw, const char *who, const char *tag, const char *name)
{
    static NameList *reported;
    NameList *list;

    switch (xw->misc.fontWarnings) {
    case fwNever:
	return;
    case fwResource:
	if (is_derived_font_name(name))
	    return;
	break;
    case fwAlways:
	break;
    }
    for (list = reported; list != 0; list = list->next) {
	if (!x_strcasecmp(name, list->name)) {
	    return;
	}
    }
    if ((list = TypeMalloc(NameList)) != 0) {
	list->name = x_strdup(name);
	list->next = reported;
	reported = list;
    }
    xtermWarning("cannot %s%s%s %sfont \"%s\"\n",
		 who, *tag ? " " : "", tag,
		 is_derived_font_name(name) ? "derived " : "",
		 name);
}

#if OPT_RENDERFONT
static void
noUsableXft(XtermWidget xw, const char *name)
{
    switch (xw->misc.fontWarnings) {
    case fwNever:
	return;
    case fwResource:
	/* these combinations of wide/bold/italic are all "derived" */
	return;
    case fwAlways:
	break;
    }
    xtermWarning("did not find a usable %s TrueType font\n", name);
}
#endif

XFontStruct *
xtermLoadQueryFont(XtermWidget xw, const char *name)
{
    XFontStruct *result = NULL;
    size_t have = strlen(name);
    if (have == 0 || have > MAX_U_STRING) {
	;			/* just ignore it */
    } else {
	TScreen *screen = TScreenOf(xw);
	result = XLoadQueryFont(screen->display, name);
    }
    return result;
}

/*
 * Open the given font and verify that it is non-empty.  Return false on
 * failure.
 */
Bool
xtermOpenFont(XtermWidget xw,
	      const char *name,
	      XTermFonts * result,
	      XTermFonts * current,
	      Bool force)
{
    Bool code = False;

    TRACE(("xtermOpenFont %d:%d '%s'\n",
	   result->warn, xw->misc.fontWarnings, NonNull(name)));

    if (!IsEmpty(name)) {
	Bool existing = (current != NULL
			 && current->fs != NULL
			 && current->fn != NULL);

	if ((result->fs = xtermLoadQueryFont(xw, name)) != 0) {
	    code = True;
	    if (EmptyFont(result->fs)) {
		xtermCloseFont(xw, result);
		code = False;
	    } else {
		result->fn = x_strdup(name);
	    }
	} else if (XmuCompareISOLatin1(name, DEFFONT) != 0) {
	    if (result->warn <= xw->misc.fontWarnings
#if OPT_RENDERFONT
		&& !UsingRenderFont(xw)
#endif
		) {
		cannotFont(xw, "load", "", name);
	    } else {
		TRACE(("xtermOpenFont: cannot load font '%s'\n", name));
	    }
	    if (existing) {
		TRACE(("...continue using font '%s'\n", current->fn));
		result->fn = x_strdup(current->fn);
		result->fs = current->fs;
	    } else if (force) {
		NoFontWarning(result);
		code = xtermOpenFont(xw, DEFFONT, result, NULL, True);
	    }
	}
    }
    NoFontWarning(result);
    return code;
}

/*
 * Close the font and free the font info.
 */
void
xtermCloseFont(XtermWidget xw, XTermFonts * fnt)
{
    if (fnt != 0 && fnt->fs != 0) {
	TScreen *screen = TScreenOf(xw);

	clrCgsFonts(xw, WhichVWin(screen), fnt);
	XFreeFont(screen->display, fnt->fs);
	xtermFreeFontInfo(fnt);
    }
}

/*
 * Close and free the font (as well as any aliases).
 */
static void
xtermCloseFont2(XtermWidget xw, XTermFonts * fnts, int which)
{
    XFontStruct *thisFont = fnts[which].fs;

    if (thisFont != 0) {
	int k;

	xtermCloseFont(xw, &fnts[which]);
	for (k = 0; k < fMAX; ++k) {
	    if (k != which) {
		if (thisFont == fnts[k].fs) {
		    xtermFreeFontInfo(&fnts[k]);
		}
	    }
	}
    }
}

/*
 * Close the listed fonts, noting that some may use copies of the pointer.
 */
void
xtermCloseFonts(XtermWidget xw, XTermFonts * fnts)
{
    int j;

    for (j = 0; j < fMAX; ++j) {
	xtermCloseFont2(xw, fnts, j);
    }
}

/*
 * Make a copy of the source, assuming the XFontStruct's to be unique, but
 * ensuring that the names are reallocated to simplify freeing.
 */
void
xtermCopyFontInfo(XTermFonts * target, XTermFonts * source)
{
    xtermFreeFontInfo(target);
    target->chrset = source->chrset;
    target->flags = source->flags;
    target->fn = x_strdup(source->fn);
    target->fs = source->fs;
    target->warn = source->warn;
}

void
xtermFreeFontInfo(XTermFonts * target)
{
    target->chrset = 0;
    target->flags = 0;
    FreeAndNull(target->fn);
    target->fs = 0;
}

#if OPT_REPORT_FONTS
static void
reportXCharStruct(const char *tag, XCharStruct * cs)
{
    ReportFonts("\t\t%s:\n", tag);
    ReportFonts("\t\t\tlbearing: %d\n", cs->lbearing);
    ReportFonts("\t\t\trbearing: %d\n", cs->rbearing);
    ReportFonts("\t\t\twidth:    %d\n", cs->width);
    ReportFonts("\t\t\tascent:   %d\n", cs->ascent);
    ReportFonts("\t\t\tdescent:  %d\n", cs->descent);
}

static void
fillXCharStruct(XCharStruct * cs, short value)
{
    cs->lbearing = value;
    cs->rbearing = value;
    cs->width = value;
    cs->ascent = value;
    cs->descent = value;
}

/* if the per-character data differs from the summary, that is a problem */
static void
compareXCharStruct(const char *tag, XCharStruct * actual, XCharStruct * expect)
{
#define CompareXCharStruct(field) \
    if (actual->field != expect->field) \
    	ReportFonts("\t\t%s %s differs: %d\n", tag, #field, actual->field)
    CompareXCharStruct(lbearing);
    CompareXCharStruct(rbearing);
    CompareXCharStruct(width);
    CompareXCharStruct(ascent);
    CompareXCharStruct(descent);
}

static void
reportXPerChar(XFontStruct *fs)
{
    XCharStruct *cs = fs->per_char;

    if (cs != NULL) {
	XCharStruct min_bounds;
	XCharStruct max_bounds;
	int valid = 0;
	int total = 0;
	unsigned first_char = 0;
	unsigned last_char = 0;
	unsigned ch;

	if (fs->max_byte1 == 0) {
	    first_char = fs->min_char_or_byte2;
	    last_char = fs->max_char_or_byte2;
	} else {
	    first_char = (fs->min_byte1 * 256) + fs->min_char_or_byte2;
	    last_char = (fs->max_byte1 * 256) + fs->max_char_or_byte2;
	}

	fillXCharStruct(&max_bounds, -32768);
	fillXCharStruct(&min_bounds, 32767);
	TRACE2(("\t\tCells: %d..%d\n", first_char, last_char));
	for (ch = first_char; ch < last_char; ++ch) {
	    XCharStruct *item = cs + ch - first_char;
	    ++total;
	    if (!CI_NONEXISTCHAR(item)) {
		++valid;
#define MIN_BOUNDS(field) min_bounds.field = Min(min_bounds.field, item->field)
		MIN_BOUNDS(lbearing);
		MIN_BOUNDS(rbearing);
		MIN_BOUNDS(width);
		MIN_BOUNDS(ascent);
		MIN_BOUNDS(descent);
#define MAX_BOUNDS(field) max_bounds.field = Max(max_bounds.field, item->field)
		MAX_BOUNDS(lbearing);
		MAX_BOUNDS(rbearing);
		MAX_BOUNDS(width);
		MAX_BOUNDS(ascent);
		MAX_BOUNDS(descent);
		TRACE2(("\t\t\t%d: cell [%d .. %d] wide %d high %d / %d\n",
			ch,
			item->lbearing,
			item->rbearing,
			item->width,
			item->ascent,
			item->descent));
	    } else {
		TRACE(("\t\t\t%d: cell missing\n", ch));
	    }
	}
	ReportFonts("\t\tPer-character: %d/%d\n", valid, total);
	compareXCharStruct("Max", &max_bounds, &(fs->max_bounds));
	compareXCharStruct("Min", &min_bounds, &(fs->min_bounds));
    } else {
	ReportFonts("\t\tPer-character: none\n");
    }
}

static void
reportOneVTFont(const char *tag,
		XTermFonts * fnt)
{
    if (!IsEmpty(fnt->fn) && fnt->fs != 0) {
	XFontStruct *fs = fnt->fs;
	unsigned first_char = 0;
	unsigned last_char = 0;

	if (fs->max_byte1 == 0) {
	    first_char = fs->min_char_or_byte2;
	    last_char = fs->max_char_or_byte2;
	} else {
	    first_char = (fs->min_byte1 * 256) + fs->min_char_or_byte2;
	    last_char = (fs->max_byte1 * 256) + fs->max_char_or_byte2;
	}

	ReportFonts("\t%s: %s\n", tag, NonNull(fnt->fn));
	ReportFonts("\t\tall chars:     %s\n", (fs->all_chars_exist
						? "yes"
						: "no"));
	ReportFonts("\t\tdefault char:  %u\n", fs->default_char);
	ReportFonts("\t\tdirection:     %u\n", fs->direction);
	ReportFonts("\t\tascent:        %d\n", fs->ascent);
	ReportFonts("\t\tdescent:       %d\n", fs->descent);
	ReportFonts("\t\tfirst char:    %u\n", first_char);
	ReportFonts("\t\tlast char:     %u\n", last_char);
	ReportFonts("\t\tmaximum-chars: %u\n", countGlyphs(fs));
	if (FontLacksMetrics(fnt)) {
	    ReportFonts("\t\tmissing-chars: ?\n");
	    ReportFonts("\t\tpresent-chars: ?\n");
	} else {
	    unsigned missing = 0;
	    unsigned ch;
	    for (ch = first_char; ch <= last_char; ++ch) {
		if (xtermMissingChar(ch, fnt)) {
		    ++missing;
		}
	    }
	    ReportFonts("\t\tmissing-chars: %u\n", missing);
	    ReportFonts("\t\tpresent-chars: %u\n", countGlyphs(fs) - missing);
	}
	ReportFonts("\t\tmin_byte1:     %u\n", fs->min_byte1);
	ReportFonts("\t\tmax_byte1:     %u\n", fs->max_byte1);
	ReportFonts("\t\tproperties:    %d\n", fs->n_properties);
	reportXCharStruct("min_bounds", &(fs->min_bounds));
	reportXCharStruct("max_bounds", &(fs->max_bounds));
	reportXPerChar(fs);
	/* TODO: report fs->properties */
    }
}

static void
reportVTFontInfo(XtermWidget xw, int fontnum)
{
    if (resource.reportFonts) {
	TScreen *screen = TScreenOf(xw);

	if (fontnum) {
	    ReportFonts("Loaded VTFonts(font%d)\n", fontnum);
	} else {
	    ReportFonts("Loaded VTFonts(default)\n");
	}

#define ReportOneVTFont(name) reportOneVTFont(#name, screen->fnts + name)
	ReportOneVTFont(fNorm);
	ReportOneVTFont(fBold);
#if OPT_WIDE_CHARS
	ReportOneVTFont(fWide);
	ReportOneVTFont(fWBold);
#endif
    }
}
#endif

void
xtermUpdateFontGCs(XtermWidget xw, MyGetFont myfunc)
{
    TScreen *screen = TScreenOf(xw);
    VTwin *win = WhichVWin(screen);
    Pixel new_normal = getXtermFG(xw, xw->flags, xw->cur_foreground);
    Pixel new_revers = getXtermBG(xw, xw->flags, xw->cur_background);

    setCgsFore(xw, win, gcNorm, new_normal);
    setCgsBack(xw, win, gcNorm, new_revers);
    setCgsFont(xw, win, gcNorm, myfunc(screen, fNorm));

    copyCgs(xw, win, gcBold, gcNorm);
    setCgsFont2(xw, win, gcBold, myfunc(screen, fBold), fBold);

    setCgsFore(xw, win, gcNormReverse, new_revers);
    setCgsBack(xw, win, gcNormReverse, new_normal);
    setCgsFont(xw, win, gcNormReverse, myfunc(screen, fNorm));

    copyCgs(xw, win, gcBoldReverse, gcNormReverse);
    setCgsFont2(xw, win, gcBoldReverse, myfunc(screen, fBold), fBold);

    if_OPT_WIDE_CHARS(screen, {
	XTermFonts *wide_xx = myfunc(screen, fWide);
	XTermFonts *bold_xx = myfunc(screen, fWBold);
	if (wide_xx->fs != 0
	    && bold_xx->fs != 0) {
	    setCgsFore(xw, win, gcWide, new_normal);
	    setCgsBack(xw, win, gcWide, new_revers);
	    setCgsFont(xw, win, gcWide, wide_xx);

	    copyCgs(xw, win, gcWBold, gcWide);
	    setCgsFont(xw, win, gcWBold, bold_xx);

	    setCgsFore(xw, win, gcWideReverse, new_revers);
	    setCgsBack(xw, win, gcWideReverse, new_normal);
	    setCgsFont(xw, win, gcWideReverse, wide_xx);

	    copyCgs(xw, win, gcWBoldReverse, gcWideReverse);
	    setCgsFont(xw, win, gcWBoldReverse, bold_xx);
	}
    });
}

#if OPT_WIDE_ATTRS
unsigned
xtermUpdateItalics(XtermWidget xw, unsigned new_attrs, unsigned old_attrs)
{
    TScreen *screen = TScreenOf(xw);

    (void) screen;
    if (UseItalicFont(screen)) {
	if ((new_attrs & ATR_ITALIC) && !(old_attrs & ATR_ITALIC)) {
	    xtermLoadItalics(xw);
	    xtermUpdateFontGCs(xw, getItalicFont);
	} else if (!(new_attrs & ATR_ITALIC) && (old_attrs & ATR_ITALIC)) {
	    xtermUpdateFontGCs(xw, getNormalFont);
	}
    }
    return new_attrs;
}
#endif

#if OPT_TRACE && OPT_BOX_CHARS
static void
show_font_misses(const char *name, XTermFonts * fp)
{
    if (fp->fs != 0) {
	if (FontLacksMetrics(fp)) {
	    TRACE(("%s font lacks metrics\n", name));
	} else if (FontIsIncomplete(fp)) {
	    TRACE(("%s font is incomplete\n", name));
	} else {
	    TRACE(("%s font is complete\n", name));
	}
    } else {
	TRACE(("%s font is missing\n", name));
    }
}
#endif

static Bool
loadNormFP(XtermWidget xw,
	   char **nameOutP,
	   XTermFonts * infoOut,
	   XTermFonts * current,
	   int fontnum)
{
    Bool status = True;

    TRACE(("loadNormFP (%s)\n", NonNull(*nameOutP)));

    if (!xtermOpenFont(xw,
		       *nameOutP,
		       infoOut,
		       current, (fontnum == fontMenu_default))) {
	/*
	 * If we are opening the default font, and it happens to be missing,
	 * force that to the compiled-in default font, e.g., "fixed".  If we
	 * cannot open the font, disable it from the menu.
	 */
	if (fontnum != fontMenu_fontsel) {
	    SetItemSensitivity(fontMenuEntries[fontnum].widget, False);
	}
	status = False;
    }
    return status;
}

static Bool
loadBoldFP(XtermWidget xw,
	   char **nameOutP,
	   XTermFonts * infoOut,
	   const char *nameRef,
	   XTermFonts * infoRef,
	   int fontnum)
{
    TScreen *screen = TScreenOf(xw);
    Bool status = True;

    TRACE(("loadBoldFP (%s)\n", NonNull(*nameOutP)));

    if (!check_fontname(*nameOutP)) {
	FontNameProperties *fp;
	char *normal = x_strdup(nameRef);

	fp = get_font_name_props(screen->display, infoRef->fs, &normal);
	if (fp != 0) {
	    NoFontWarning(infoOut);
	    *nameOutP = bold_font_name(fp, fp->average_width);
	    if (!xtermOpenFont(xw, *nameOutP, infoOut, NULL, False)) {
		free(*nameOutP);
		*nameOutP = bold_font_name(fp, -1);
		xtermOpenFont(xw, *nameOutP, infoOut, NULL, False);
	    }
	    TRACE(("...derived bold '%s'\n", NonNull(*nameOutP)));
	}
	if (fp == 0 || infoOut->fs == 0) {
	    xtermCopyFontInfo(infoOut, infoRef);
	    TRACE(("...cannot load a matching bold font\n"));
	} else if (comparable_metrics(infoRef->fs, infoOut->fs)
		   && same_font_size(xw, infoRef->fs, infoOut->fs)
		   && got_bold_font(screen->display, infoOut->fs, *nameOutP)) {
	    TRACE(("...got a matching bold font\n"));
	    cache_menu_font_name(screen, fontnum, fBold, *nameOutP);
	} else {
	    xtermCloseFont2(xw, infoOut - fBold, fBold);
	    *infoOut = *infoRef;
	    TRACE(("...did not get a matching bold font\n"));
	}
	free(normal);
    } else if (!xtermOpenFont(xw, *nameOutP, infoOut, NULL, False)) {
	xtermCopyFontInfo(infoOut, infoRef);
	TRACE(("...cannot load bold font '%s'\n", NonNull(*nameOutP)));
    } else {
	cache_menu_font_name(screen, fontnum, fBold, *nameOutP);
    }

    /*
     * Most of the time this call to load the font will succeed, even if
     * there is no wide font :  the X server doubles the width of the
     * normal font, or similar.
     *
     * But if it did fail for some reason, then nevermind.
     */
    if (EmptyFont(infoOut->fs))
	status = False;		/* can't use a 0-sized font */

    if (!same_font_size(xw, infoRef->fs, infoOut->fs)
	&& (is_fixed_font(infoRef->fs) && is_fixed_font(infoOut->fs))) {
	TRACE(("...ignoring mismatched normal/bold fonts\n"));
	xtermCloseFont2(xw, infoOut - fBold, fBold);
	xtermCopyFontInfo(infoOut, infoRef);
    }

    return status;
}

#if OPT_WIDE_CHARS
static Bool
loadWideFP(XtermWidget xw,
	   char **nameOutP,
	   XTermFonts * infoOut,
	   const char *nameRef,
	   XTermFonts * infoRef,
	   int fontnum)
{
    TScreen *screen = TScreenOf(xw);
    Bool status = True;

    TRACE(("loadWideFP (%s)\n", NonNull(*nameOutP)));

    if (!check_fontname(*nameOutP)
	&& (screen->utf8_fonts && !is_double_width_font(infoRef->fs))) {
	char *normal = x_strdup(nameRef);
	FontNameProperties *fp = get_font_name_props(screen->display,
						     infoRef->fs, &normal);
	if (fp != 0) {
	    *nameOutP = wide_font_name(fp);
	    NoFontWarning(infoOut);
	}
	free(normal);
    }

    if (check_fontname(*nameOutP)) {
	if (xtermOpenFont(xw, *nameOutP, infoOut, NULL, False)
	    && is_derived_font_name(*nameOutP)
	    && EmptyFont(infoOut->fs)) {
	    xtermCloseFont2(xw, infoOut - fWide, fWide);
	}
	if (infoOut->fs == 0) {
	    xtermCopyFontInfo(infoOut, infoRef);
	} else {
	    TRACE(("...%s wide %s\n",
		   is_derived_font_name(*nameOutP) ? "derived" : "given",
		   NonNull(*nameOutP)));
	    cache_menu_font_name(screen, fontnum, fWide, *nameOutP);
	}
    } else {
	xtermCopyFontInfo(infoOut, infoRef);
    }
#define MinWidthOf(fs) fs->min_bounds.width
#define MaxWidthOf(fs) fs->max_bounds.width
    xw->work.force_wideFont = False;
    if (MaxWidthOf(infoOut->fs) != (2 * MaxWidthOf(infoRef->fs))) {
	TRACE(("...reference width %d\n", MaxWidthOf(infoRef->fs)));
	TRACE(("...?? double-width %d\n", 2 * MaxWidthOf(infoRef->fs)));
	TRACE(("...actual width    %d\n", MaxWidthOf(infoOut->fs)));
	xw->work.force_wideFont = True;
    }
    return status;
}

static Bool
loadWBoldFP(XtermWidget xw,
	    char **nameOutP,
	    XTermFonts * infoOut,
	    const char *wideNameRef, XTermFonts * wideInfoRef,
	    const char *boldNameRef, XTermFonts * boldInfoRef,
	    int fontnum)
{
    TScreen *screen = TScreenOf(xw);
    Bool status = True;
    char *bold = NULL;

    TRACE(("loadWBoldFP (%s)\n", NonNull(*nameOutP)));

    if (!check_fontname(*nameOutP)) {
	FontNameProperties *fp;
	fp = get_font_name_props(screen->display, boldInfoRef->fs, &bold);
	if (fp != 0) {
	    *nameOutP = widebold_font_name(fp);
	    NoFontWarning(infoOut);
	}
    }

    if (check_fontname(*nameOutP)) {

	if (xtermOpenFont(xw, *nameOutP, infoOut, NULL, False)
	    && is_derived_font_name(*nameOutP)
	    && !compatibleWideCounts(wideInfoRef->fs, infoOut->fs)) {
	    xtermCloseFont2(xw, infoOut - fWBold, fWBold);
	}

	if (infoOut->fs == 0) {
	    if (is_derived_font_name(*nameOutP))
		free(*nameOutP);
	    if (IsEmpty(wideNameRef)) {
		*nameOutP = x_strdup(boldNameRef);
		xtermCopyFontInfo(infoOut, boldInfoRef);
		TRACE(("...cannot load wide-bold, use bold %s\n",
		       NonNull(boldNameRef)));
	    } else {
		*nameOutP = x_strdup(wideNameRef);
		xtermCopyFontInfo(infoOut, wideInfoRef);
		TRACE(("...cannot load wide-bold, use wide %s\n",
		       NonNull(wideNameRef)));
	    }
	} else {
	    TRACE(("...%s wide/bold %s\n",
		   is_derived_font_name(*nameOutP) ? "derived" : "given",
		   NonNull(*nameOutP)));
	    cache_menu_font_name(screen, fontnum, fWBold, *nameOutP);
	}
    } else if (is_double_width_font(boldInfoRef->fs)) {
	xtermCopyFontInfo(infoOut, boldInfoRef);
	TRACE(("...bold font is double-width, use it %s\n", NonNull(boldNameRef)));
    } else {
	xtermCopyFontInfo(infoOut, wideInfoRef);
	TRACE(("...cannot load wide bold font, use wide %s\n", NonNull(wideNameRef)));
    }

    free(bold);

    if (EmptyFont(infoOut->fs)) {
	status = False;		/* can't use a 0-sized font */
    } else {
	if ((!comparable_metrics(wideInfoRef->fs, infoOut->fs)
	     || (!same_font_size(xw, wideInfoRef->fs, infoOut->fs)
		 && is_fixed_font(wideInfoRef->fs)
		 && is_fixed_font(infoOut->fs)))) {
	    TRACE(("...ignoring mismatched normal/bold wide fonts\n"));
	    xtermCloseFont2(xw, infoOut - fWBold, fWBold);
	    xtermCopyFontInfo(infoOut, wideInfoRef);
	}
    }

    return status;
}
#endif

/*
 * Load a given bitmap font, along with the bold/wide variants.
 * Returns nonzero on success.
 */
int
xtermLoadFont(XtermWidget xw,
	      const VTFontNames * fonts,
	      Bool doresize,
	      int fontnum)
{
    TScreen *screen = TScreenOf(xw);
    VTwin *win = WhichVWin(screen);

    VTFontNames new_fnames;
    XTermFonts new_fonts[fMAX];
    XTermFonts old_fonts[fMAX];
    char *tmpname = NULL;
    Boolean proportional = False;
    Boolean recovered;
    int code = 0;

    memset(&new_fnames, 0, sizeof(new_fnames));
    memset(new_fonts, 0, sizeof(new_fonts));
    memcpy(&old_fonts, screen->fnts, sizeof(old_fonts));

    if (fonts != 0)
	new_fnames = *fonts;
    if (!check_fontname(new_fnames.f_n))
	return code;

    if (fontnum == fontMenu_fontescape
	&& new_fnames.f_n != screen->MenuFontName(fontnum)) {
	if ((tmpname = x_strdup(new_fnames.f_n)) == 0)
	    return code;
    }

    TRACE(("Begin Cgs - xtermLoadFont(%s)\n", new_fnames.f_n));
    releaseWindowGCs(xw, win);

#define DbgResource(name, field, index) \
    TRACE(("xtermLoadFont #%d "name" %s%s\n", \
    	   fontnum, \
	   (new_fonts[index].warn == fwResource) ? "*" : " ", \
	   NonNull(new_fnames.field)))
    DbgResource("normal", f_n, fNorm);
    DbgResource("bold  ", f_b, fBold);
#if OPT_WIDE_CHARS
    DbgResource("wide  ", f_w, fWide);
    DbgResource("w/bold", f_wb, fWBold);
#endif

    if (!loadNormFP(xw,
		    &new_fnames.f_n,
		    &new_fonts[fNorm],
		    &old_fonts[fNorm],
		    fontnum))
	goto bad;

    if (!loadBoldFP(xw,
		    &new_fnames.f_b,
		    &new_fonts[fBold],
		    new_fnames.f_n,
		    &new_fonts[fNorm],
		    fontnum))
	goto bad;

    /*
     * If there is no widefont specified, fake it by doubling AVERAGE_WIDTH
     * of normal fonts XLFD, and asking for it.  This plucks out 18x18ja
     * and 12x13ja as the corresponding fonts for 9x18 and 6x13.
     */
    if_OPT_WIDE_CHARS(screen, {

	if (!loadWideFP(xw,
			&new_fnames.f_w,
			&new_fonts[fWide],
			new_fnames.f_n,
			&new_fonts[fNorm],
			fontnum))
	    goto bad;

	if (!loadWBoldFP(xw,
			 &new_fnames.f_wb,
			 &new_fonts[fWBold],
			 new_fnames.f_w,
			 &new_fonts[fWide],
			 new_fnames.f_b,
			 &new_fonts[fBold],
			 fontnum))
	    goto bad;

    });

    /*
     * Normal/bold fonts should be the same width.  Also, the min/max
     * values should be the same.
     */
    if (new_fonts[fNorm].fs != 0
	&& new_fonts[fBold].fs != 0
	&& (!is_fixed_font(new_fonts[fNorm].fs)
	    || !is_fixed_font(new_fonts[fBold].fs)
	    || differing_widths(new_fonts[fNorm].fs, new_fonts[fBold].fs))) {
	TRACE(("Proportional font! normal %d/%d, bold %d/%d\n",
	       new_fonts[fNorm].fs->min_bounds.width,
	       new_fonts[fNorm].fs->max_bounds.width,
	       new_fonts[fBold].fs->min_bounds.width,
	       new_fonts[fBold].fs->max_bounds.width));
	proportional = True;
    }

    if_OPT_WIDE_CHARS(screen, {
	if (new_fonts[fWide].fs != 0
	    && new_fonts[fWBold].fs != 0
	    && (!is_fixed_font(new_fonts[fWide].fs)
		|| !is_fixed_font(new_fonts[fWBold].fs)
		|| differing_widths(new_fonts[fWide].fs, new_fonts[fWBold].fs))) {
	    TRACE(("Proportional font! wide %d/%d, wide bold %d/%d\n",
		   new_fonts[fWide].fs->min_bounds.width,
		   new_fonts[fWide].fs->max_bounds.width,
		   new_fonts[fWBold].fs->min_bounds.width,
		   new_fonts[fWBold].fs->max_bounds.width));
	    proportional = True;
	}
    });

    /* TODO : enforce that the width of the wide font is 2* the width
       of the narrow font */

    /*
     * If we're switching fonts, free the old ones.  Otherwise we'll leak
     * the memory that is associated with the old fonts.  The
     * XLoadQueryFont call allocates a new XFontStruct.
     */
    xtermCloseFonts(xw, screen->fnts);
#if OPT_WIDE_ATTRS
    xtermCloseFonts(xw, screen->ifnts);
    screen->ifnts_ok = False;
#endif

    xtermCopyFontInfo(GetNormalFont(screen, fNorm), &new_fonts[fNorm]);
    xtermCopyFontInfo(GetNormalFont(screen, fBold), &new_fonts[fBold]);
#if OPT_WIDE_CHARS
    xtermCopyFontInfo(GetNormalFont(screen, fWide), &new_fonts[fWide]);
    if (new_fonts[fWBold].fs == NULL)
	xtermCopyFontInfo(GetNormalFont(screen, fWide), &new_fonts[fWide]);
    xtermCopyFontInfo(GetNormalFont(screen, fWBold), &new_fonts[fWBold]);
#endif

    xtermUpdateFontGCs(xw, getNormalFont);

#if OPT_BOX_CHARS
    screen->allow_packing = proportional;
    setupPackedFonts(xw);
#endif
    screen->fnt_prop = (Boolean) (proportional && !(screen->force_packed));
    screen->fnt_boxes = 1;

#if OPT_BOX_CHARS
    /*
     * xterm uses character positions 1-31 of a font for the line-drawing
     * characters.  Check that they are all present.  The null character
     * (0) is special, and is not used.
     */
#if OPT_RENDERFONT
    if (UsingRenderFont(xw)) {
	/*
	 * FIXME: we shouldn't even be here if we're using Xft.
	 */
	screen->fnt_boxes = 0;
	TRACE(("assume Xft missing line-drawing chars\n"));
    } else
#endif
    {
	unsigned ch;

#if OPT_TRACE
#define TRACE_MISS(index) show_font_misses(#index, &new_fonts[index])
	TRACE_MISS(fNorm);
	TRACE_MISS(fBold);
#if OPT_WIDE_CHARS
	TRACE_MISS(fWide);
	TRACE_MISS(fWBold);
#endif
#endif

#if OPT_WIDE_CHARS
	if (screen->utf8_mode || screen->unicode_font) {
	    UIntSet(screen->fnt_boxes, 2);
	    for (ch = 1; ch < 32; ch++) {
		unsigned n = dec2ucs(screen, ch);
		if (!is_UCS_SPECIAL(n)
		    && (n != ch)
		    && (screen->fnt_boxes & 2)) {
		    if (xtermMissingChar(n, &new_fonts[fNorm]) ||
			xtermMissingChar(n, &new_fonts[fBold])) {
			UIntClr(screen->fnt_boxes, 2);
			TRACE(("missing graphics character #%d, U+%04X\n",
			       ch, n));
			break;
		    }
		}
	    }
	}
#endif

	for (ch = 1; ch < 32; ch++) {
	    if (xtermMissingChar(ch, &new_fonts[fNorm])) {
		TRACE(("missing normal char #%d\n", ch));
		UIntClr(screen->fnt_boxes, 1);
		break;
	    }
	    if (xtermMissingChar(ch, &new_fonts[fBold])) {
		TRACE(("missing bold   char #%d\n", ch));
		UIntClr(screen->fnt_boxes, 1);
		break;
	    }
	}

	TRACE(("Will %suse internal line-drawing characters (mode %d)\n",
	       screen->fnt_boxes ? "not " : "",
	       screen->fnt_boxes));
    }
#endif

    if (screen->always_bold_mode) {
	screen->enbolden = screen->bold_mode;
    } else {
	screen->enbolden = screen->bold_mode
	    && ((new_fonts[fNorm].fs == new_fonts[fBold].fs)
		|| same_font_name(new_fnames.f_n, new_fnames.f_b));
    }
    TRACE(("Will %suse 1-pixel offset/overstrike to simulate bold\n",
	   screen->enbolden ? "" : "not "));

    set_menu_font(False);
    screen->menu_font_number = fontnum;
    set_menu_font(True);
    if (tmpname) {		/* if setting escape or sel */
	if (screen->MenuFontName(fontnum))
	    FREE_STRING(screen->MenuFontName(fontnum));
	screen->MenuFontName(fontnum) = tmpname;
	if (fontnum == fontMenu_fontescape) {
	    update_font_escape();
	}
#if OPT_SHIFT_FONTS
	screen->menu_font_sizes[fontnum] = FontSize(new_fonts[fNorm].fs);
#endif
    }
    set_cursor_gcs(xw);
    xtermUpdateFontInfo(xw, doresize);
    TRACE(("Success Cgs - xtermLoadFont\n"));
#if OPT_REPORT_FONTS
    reportVTFontInfo(xw, fontnum);
#endif
    FREE_FNAME(f_n);
    FREE_FNAME(f_b);
#if OPT_WIDE_CHARS
    FREE_FNAME(f_w);
    FREE_FNAME(f_wb);
#endif
    if (new_fonts[fNorm].fn == new_fonts[fBold].fn) {
	free(new_fonts[fNorm].fn);
    } else {
	free(new_fonts[fNorm].fn);
	free(new_fonts[fBold].fn);
    }
#if OPT_WIDE_CHARS
    free(new_fonts[fWide].fn);
    free(new_fonts[fWBold].fn);
#endif
    xtermSetWinSize(xw);
    return 1;

  bad:
    recovered = False;
    free(tmpname);

#if OPT_RENDERFONT
    if ((fontnum == fontMenu_fontsel) && (fontnum != screen->menu_font_number)) {
	int old_fontnum = screen->menu_font_number;
#if OPT_TOOLBAR
	SetItemSensitivity(fontMenuEntries[fontnum].widget, True);
#endif
	Bell(xw, XkbBI_MinorError, 0);
	new_fnames.f_n = screen->MenuFontName(old_fontnum);
	if (xtermLoadFont(xw, &new_fnames, doresize, old_fontnum))
	    recovered = True;
    } else if (x_strcasecmp(new_fnames.f_n, DEFFONT)
	       && x_strcasecmp(new_fnames.f_n, old_fonts[fNorm].fn)) {
	new_fnames.f_n = x_strdup(old_fonts[fNorm].fn);
	TRACE(("...recovering from failed font-load\n"));
	if (xtermLoadFont(xw, &new_fnames, doresize, fontnum)) {
	    recovered = True;
	    if (fontnum != fontMenu_fontsel) {
		SetItemSensitivity(fontMenuEntries[fontnum].widget,
				   UsingRenderFont(xw));
	    }
	    TRACE(("...recovered size %dx%d\n",
		   FontHeight(screen),
		   FontWidth(screen)));
	}
    }
#endif
    if (!recovered) {
	releaseWindowGCs(xw, win);
	xtermCloseFonts(xw, new_fonts);
	TRACE(("Fail Cgs - xtermLoadFont\n"));
	code = 0;
    }
    return code;
}

#if OPT_WIDE_ATTRS
/*
 * (Attempt to) load matching italics for the current normal/bold/etc fonts.
 * If the attempt fails for a given style, use the non-italic font.
 */
void
xtermLoadItalics(XtermWidget xw)
{
    TScreen *screen = TScreenOf(xw);

    if (UseItalicFont(screen) && !screen->ifnts_ok) {
	int n;
	FontNameProperties *fp;
	XTermFonts *data;

	screen->ifnts_ok = True;
	for (n = 0; n < fMAX; ++n) {
	    switch (n) {
	    case fNorm:
		/* FALLTHRU */
	    case fBold:
		/* FALLTHRU */
#if OPT_WIDE_CHARS
	    case fWide:
		/* FALLTHRU */
	    case fWBold:
#endif
		/* FALLTHRU */
		data = getItalicFont(screen, n);

		/*
		 * FIXME - need to handle font-leaks
		 */
		data->fs = 0;
		if (getNormalFont(screen, n)->fs != 0 &&
		    (fp = get_font_name_props(screen->display,
					      getNormalFont(screen, n)->fs,
					      0)) != 0) {
		    if (!open_italic_font(xw, n, fp, data)) {
			if (n > 0) {
			    xtermCopyFontInfo(data,
					      getItalicFont(screen, n - 1));
			} else {
			    xtermOpenFont(xw,
					  getNormalFont(screen, n)->fn,
					  data, NULL, False);
			}
		    }
		}
		break;
	    }
	}
    }
}
#endif

#if OPT_LOAD_VTFONTS || OPT_WIDE_CHARS
/*
 * Collect font-names that we can modify with the load-vt-fonts() action.
 */
#define MERGE_SUBFONT(dst,src,name) \
	if (IsEmpty(dst.name)) { \
	    TRACE(("MERGE_SUBFONT " #dst "." #name " merge \"%s\"\n", NonNull(src.name))); \
	    dst.name = x_strdup(src.name); \
	} else { \
	    TRACE(("MERGE_SUBFONT " #dst "." #name " found \"%s\"\n", NonNull(dst.name))); \
	}
#define MERGE_SUBLIST(dst,src,name) \
	if (dst.fonts.x11.name == NULL) \
	    dst.fonts.x11.name = TypeCalloc(char *); \
	if (merge_sublist(&(dst.fonts.x11.name), src.fonts.x11.name)) { \
	    TRACE(("MERGE_SUBLIST " #dst "." #name " merge \"%s\"\n", src.fonts.x11.name[0])); \
	} else { \
	    TRACE(("MERGE_SUBLIST " #dst "." #name " found \"%s\"\n", dst.fonts.x11.name[0])); \
	}

#define INFER_SUBFONT(dst,src,name) \
	if (IsEmpty(dst.name)) { \
	    TRACE(("INFER_SUBFONT " #dst "." #name " will infer\n")); \
	    dst.name = x_strdup(""); \
	} else { \
	    TRACE(("INFER_SUBFONT " #dst "." #name " found \"%s\"\n", NonNull(dst.name))); \
	}

#define FREE_MENU_FONTS(dst) \
	TRACE(("FREE_MENU_FONTS " #dst "\n")); \
	for (n = fontMenu_default; n <= fontMenu_lastBuiltin; ++n) { \
	    for (m = 0; m < fMAX; ++m) { \
		FREE_STRING(dst.menu_font_names[n][m]); \
		dst.menu_font_names[n][m] = 0; \
	    } \
	}

#define COPY_MENU_FONTS(dst,src) \
	TRACE(("COPY_MENU_FONTS " #src " to " #dst "\n")); \
	for (n = fontMenu_default; n <= fontMenu_lastBuiltin; ++n) { \
	    for (m = 0; m < fMAX; ++m) { \
		FREE_STRING(dst.menu_font_names[n][m]); \
		dst.menu_font_names[n][m] = x_strdup(src.menu_font_names[n][m]); \
	    } \
	    TRACE((".. " #dst ".menu_fonts_names[%d] = %s\n", n, NonNull(dst.menu_font_names[n][fNorm]))); \
	}

#define COPY_DEFAULT_FONTS(target, source) \
	TRACE(("COPY_DEFAULT_FONTS " #source " to " #target "\n")); \
	xtermCopyVTFontNames(&target.default_font, &source.default_font)

#define COPY_X11_FONTLISTS(target, source) \
	TRACE(("COPY_X11_FONTLISTS " #source " to " #target "\n")); \
	xtermCopyFontLists(xw, &target.fonts.x11, &source.fonts.x11)

static void
xtermCopyVTFontNames(VTFontNames * target, VTFontNames * source)
{
#define COPY_IT(name,field) \
    TRACE((".. "#name" = %s\n", NonNull(source->field))); \
    free(target->field); \
    target->field = x_strdup(source->field)

    TRACE(("xtermCopyVTFontNames\n"));

    COPY_IT(font, f_n);
    COPY_IT(boldFont, f_b);

#if OPT_WIDE_CHARS
    COPY_IT(wideFont, f_w);
    COPY_IT(wideBoldFont, f_wb);
#endif
#undef COPY_IT
}

static void
xtermCopyFontLists(XtermWidget xw, VTFontList * target, VTFontList * source)
{
#define COPY_IT(name,field) \
    copyFontList(&(target->field), source->field); \
    TRACE_ARGV(".. " #name, source->field)

    (void) xw;
    TRACE(("xtermCopyFontLists %s ->%s\n",
	   whichFontList(xw, source),
	   whichFontList(xw, target)));

    COPY_IT(font, list_n);
    COPY_IT(fontBold, list_b);
#if OPT_WIDE_ATTRS || OPT_RENDERWIDE
    COPY_IT(fontItal, list_i);
    COPY_IT(fontBtal, list_bi);
#endif
#if OPT_WIDE_CHARS
    COPY_IT(wideFont, list_w);
    COPY_IT(wideBoldFont, list_wb);
    COPY_IT(wideItalFont, list_wi);
    COPY_IT(wideBtalFont, list_wbi);
#endif
#undef COPY_IT
}

void
xtermSaveVTFonts(XtermWidget xw)
{
    TScreen *screen = TScreenOf(xw);
    Cardinal n, m;

    if (!screen->savedVTFonts) {

	screen->savedVTFonts = True;
	TRACE(("xtermSaveVTFonts saving original\n"));
	COPY_DEFAULT_FONTS(screen->cacheVTFonts, xw->misc);
	COPY_X11_FONTLISTS(screen->cacheVTFonts, xw->work);
	COPY_MENU_FONTS(screen->cacheVTFonts, xw->screen);
    }
}

#define SAME_STRING(x,y) ((x) == (y) || ((x) && (y) && !strcmp(x, y)))
#define SAME_MEMBER(n)   SAME_STRING(a->n, b->n)

static Boolean
sameSubResources(SubResourceRec * a, SubResourceRec * b)
{
    Boolean result = True;

    if (!SAME_MEMBER(default_font.f_n)
	|| !SAME_MEMBER(default_font.f_b)
#if OPT_WIDE_CHARS
	|| !SAME_MEMBER(default_font.f_w)
	|| !SAME_MEMBER(default_font.f_wb)
#endif
	) {
	TRACE(("sameSubResources: default_font differs\n"));
	result = False;
    } else {
	int n;

	for (n = 0; n < NMENUFONTS; ++n) {
	    if (!SAME_MEMBER(menu_font_names[n][fNorm])) {
		TRACE(("sameSubResources: menu_font_names[%d] differs\n", n));
		result = False;
		break;
	    }
	}
    }

    return result;
}

/*
 * Load the "VT" font names from the given subresource name/class.  These
 * correspond to the VT100 resources.
 */
static Bool
xtermLoadVTFonts(XtermWidget xw, String myName, String myClass)
{
    SubResourceRec subresourceRec;
    SubResourceRec referenceRec;

    /*
     * These are duplicates of the VT100 font resources, but with a special
     * application/classname passed in to distinguish them.
     */
    static XtResource font_resources[] =
    {
	Sres(XtNfont, XtCFont, default_font.f_n, DEFFONT),
	Sres(XtNboldFont, XtCBoldFont, default_font.f_b, DEFBOLDFONT),
#if OPT_WIDE_CHARS
	Sres(XtNwideFont, XtCWideFont, default_font.f_w, DEFWIDEFONT),
	Sres(XtNwideBoldFont, XtCWideBoldFont, default_font.f_wb, DEFWIDEBOLDFONT),
#endif
	Sres(XtNfont1, XtCFont1, MenuFontName(fontMenu_font1), NULL),
	Sres(XtNfont2, XtCFont2, MenuFontName(fontMenu_font2), NULL),
	Sres(XtNfont3, XtCFont3, MenuFontName(fontMenu_font3), NULL),
	Sres(XtNfont4, XtCFont4, MenuFontName(fontMenu_font4), NULL),
	Sres(XtNfont5, XtCFont5, MenuFontName(fontMenu_font5), NULL),
	Sres(XtNfont6, XtCFont6, MenuFontName(fontMenu_font6), NULL),
	Sres(XtNfont7, XtCFont7, MenuFontName(fontMenu_font7), NULL),
    };
    Cardinal n, m;
    Bool status = True;
    TScreen *screen = TScreenOf(xw);

    TRACE(("called xtermLoadVTFonts(name=%s, class=%s)\n",
	   NonNull(myName), NonNull(myClass)));

    xtermSaveVTFonts(xw);

    if (IsEmpty(myName)) {
	TRACE(("xtermLoadVTFonts restoring original\n"));
	COPY_DEFAULT_FONTS(xw->misc, screen->cacheVTFonts);
	COPY_X11_FONTLISTS(xw->work, screen->cacheVTFonts);
	FREE_MENU_FONTS(xw->screen);
	COPY_MENU_FONTS(xw->screen, screen->cacheVTFonts);
    } else {
	TRACE(("xtermLoadVTFonts(%s, %s)\n", myName, myClass));

	memset(&referenceRec, 0, sizeof(referenceRec));
	memset(&subresourceRec, 0, sizeof(subresourceRec));
	XtGetSubresources((Widget) xw, (XtPointer) &subresourceRec,
			  myName, myClass,
			  font_resources,
			  (Cardinal) XtNumber(font_resources),
			  NULL, (Cardinal) 0);

	/*
	 * XtGetSubresources returns no status, so we compare the returned
	 * data against a zero'd struct to see if any data is returned.
	 */
	if (memcmp(&referenceRec, &subresourceRec, sizeof(referenceRec))
	    && !sameSubResources(&(screen->cacheVTFonts), &subresourceRec)) {

	    screen->mergedVTFonts = True;

	    /*
	     * To make it simple, reallocate the strings returned by
	     * XtGetSubresources.  We can free our own strings, but not theirs.
	     */
	    ALLOC_STRING(subresourceRec.default_font.f_n);
	    ALLOC_STRING(subresourceRec.default_font.f_b);
#if OPT_WIDE_CHARS
	    ALLOC_STRING(subresourceRec.default_font.f_w);
	    ALLOC_STRING(subresourceRec.default_font.f_wb);
#endif
	    for (n = fontMenu_font1; n <= fontMenu_lastBuiltin; ++n) {
		ALLOC_STRING(subresourceRec.MenuFontName(n));
	    }

	    /*
	     * Now, save the string to a font-list for consistency
	     */
#define ALLOC_SUBLIST(which,field) \
	    if (subresourceRec.default_font.field != NULL) { \
		char *blob = x_strdup(subresourceRec.default_font.field); \
		char *base; \
		for (base = blob; ; base = NULL) { \
		    char *item = strtok(base, ","); \
		    if (item == NULL) \
			break; \
		    save2FontList(xw, "cached", \
				  &(subresourceRec.fonts), \
				  which, \
				  item, False, False); \
		} \
		free(blob); \
	    }

	    ALLOC_SUBLIST(fNorm, f_n);
	    ALLOC_SUBLIST(fBold, f_b);
#if OPT_WIDE_CHARS
	    ALLOC_SUBLIST(fWide, f_w);
	    ALLOC_SUBLIST(fWBold, f_wb);
#endif

	    /*
	     * If a particular resource value was not found, use the original.
	     */
	    MERGE_SUBFONT(subresourceRec, xw->misc, default_font.f_n);
	    INFER_SUBFONT(subresourceRec, xw->misc, default_font.f_b);
	    MERGE_SUBLIST(subresourceRec, xw->work, list_n);
	    MERGE_SUBLIST(subresourceRec, xw->work, list_b);
#if OPT_WIDE_CHARS
	    INFER_SUBFONT(subresourceRec, xw->misc, default_font.f_w);
	    INFER_SUBFONT(subresourceRec, xw->misc, default_font.f_wb);
	    MERGE_SUBLIST(subresourceRec, xw->work, list_w);
	    MERGE_SUBLIST(subresourceRec, xw->work, list_wb);
#endif
	    for (n = fontMenu_font1; n <= fontMenu_lastBuiltin; ++n) {
		MERGE_SUBFONT(subresourceRec, xw->screen, MenuFontName(n));
	    }

	    /*
	     * Finally, copy the subresource data to the widget.
	     */
	    COPY_DEFAULT_FONTS(xw->misc, subresourceRec);
	    COPY_X11_FONTLISTS(xw->work, subresourceRec);
	    FREE_MENU_FONTS(xw->screen);
	    COPY_MENU_FONTS(xw->screen, subresourceRec);

	    FREE_STRING(screen->MenuFontName(fontMenu_default));
	    FREE_STRING(screen->menu_font_names[0][fBold]);
	    screen->MenuFontName(fontMenu_default) = x_strdup(DefaultFontN(xw));
	    screen->menu_font_names[0][fBold] = x_strdup(DefaultFontB(xw));
#if OPT_WIDE_CHARS
	    FREE_STRING(screen->menu_font_names[0][fWide]);
	    FREE_STRING(screen->menu_font_names[0][fWBold]);
	    screen->menu_font_names[0][fWide] = x_strdup(DefaultFontW(xw));
	    screen->menu_font_names[0][fWBold] = x_strdup(DefaultFontWB(xw));
#endif
	    /*
	     * And remove our copies of strings.
	     */
	    FREE_STRING(subresourceRec.default_font.f_n);
	    FREE_STRING(subresourceRec.default_font.f_b);
#if OPT_WIDE_CHARS
	    FREE_STRING(subresourceRec.default_font.f_w);
	    FREE_STRING(subresourceRec.default_font.f_wb);
#endif
	    for (n = fontMenu_font1; n <= fontMenu_lastBuiltin; ++n) {
		FREE_STRING(subresourceRec.MenuFontName(n));
	    }
	} else {
	    TRACE(("...no resources found\n"));
	    status = False;
	}
    }
    TRACE((".. xtermLoadVTFonts: %d\n", status));
    return status;
}

#if OPT_WIDE_CHARS
static Bool
isWideFont(XFontStruct *fp, const char *tag, Bool nullOk)
{
    Bool result = False;

    (void) tag;
    if (okFont(fp)) {
	unsigned count = countGlyphs(fp);
	TRACE(("isWideFont(%s) found %d cells\n", tag, count));
	result = (count > 256) ? True : False;
    } else {
	result = nullOk;
    }
    return result;
}

/*
 * If the current fonts are not wide, load the UTF8 fonts.
 *
 * Called during initialization (for wide-character mode), the fonts have not
 * been setup, so we pass nullOk=True to isWideFont().
 *
 * Called after initialization, e.g., in response to the UTF-8 menu entry
 * (starting from narrow character mode), it checks if the fonts are not wide.
 */
Bool
xtermLoadWideFonts(XtermWidget xw, Bool nullOk)
{
    TScreen *screen = TScreenOf(xw);
    Bool result;

    if (EmptyFont(GetNormalFont(screen, fWide)->fs)) {
	result = (isWideFont(GetNormalFont(screen, fNorm)->fs, "normal", nullOk)
		  && isWideFont(GetNormalFont(screen, fBold)->fs, "bold", nullOk));
    } else {
	result = (isWideFont(GetNormalFont(screen, fWide)->fs, "wide", nullOk)
		  && isWideFont(GetNormalFont(screen, fWBold)->fs,
				"wide-bold", nullOk));
	if (result && !screen->utf8_latin1) {
	    result = (isWideFont(GetNormalFont(screen, fNorm)->fs, "normal", nullOk)
		      && isWideFont(GetNormalFont(screen, fBold)->fs,
				    "bold", nullOk));
	}
    }
    if (!result) {
	TRACE(("current fonts are not all wide%s\n", nullOk ? " nullOk" : ""));
	result = xtermLoadVTFonts(xw, XtNutf8Fonts, XtCUtf8Fonts);
    }
    TRACE(("xtermLoadWideFonts:%d\n", result));
    return result;
}
#endif /* OPT_WIDE_CHARS */

/*
 * Restore the default fonts, i.e., if we had switched to wide-fonts.
 */
Bool
xtermLoadDefaultFonts(XtermWidget xw)
{
    Bool result;
    result = xtermLoadVTFonts(xw, NULL, NULL);
    TRACE(("xtermLoadDefaultFonts:%d\n", result));
    return result;
}
#endif /* OPT_LOAD_VTFONTS || OPT_WIDE_CHARS */

#if OPT_LOAD_VTFONTS
void
HandleLoadVTFonts(Widget w,
		  XEvent *event GCC_UNUSED,
		  String *params,
		  Cardinal *param_count)
{
    XtermWidget xw;

    if ((xw = getXtermWidget(w)) != 0) {
	static char empty[] = "";	/* appease strict compilers */

	TScreen *screen = TScreenOf(xw);
	char name_buf[80];
	String name = (String) ((*param_count > 0) ? params[0] : empty);
	char *myName = MyStackAlloc(strlen(name) + 1, name_buf);

	TRACE(("HandleLoadVTFonts(%d)\n", *param_count));
	if (myName != 0) {
	    char class_buf[80];
	    String convert = (String) ((*param_count > 1) ? params[1] : myName);
	    char *myClass = MyStackAlloc(strlen(convert) + 1, class_buf);

	    strcpy(myName, name);
	    if (myClass != 0) {
		strcpy(myClass, convert);
		if (*param_count == 1)
		    myClass[0] = x_toupper(myClass[0]);

		if (xtermLoadVTFonts(xw, myName, myClass)) {
		    int n;
		    /*
		     * When switching fonts, try to preserve the font-menu
		     * selection, since it is less surprising to do that (if
		     * the font-switching can be undone) than to switch to
		     * "Default".
		     */
		    int font_number = screen->menu_font_number;
		    if (font_number > fontMenu_lastBuiltin)
			font_number = fontMenu_lastBuiltin;
		    for (n = 0; n < NMENUFONTS; ++n) {
			screen->menu_font_sizes[n] = 0;
		    }
		    if (font_number == fontMenu_default) {
			SetVTFont(xw, font_number, True, defaultVTFontNames(xw));
		    } else {
			SetVTFont(xw, font_number, True, NULL);
		    }
		}
		MyStackFree(myClass, class_buf);
	    }
	    MyStackFree(myName, name_buf);
	}
    }
}
#endif /* OPT_LOAD_VTFONTS */

/*
 * Set the limits for the box that outlines the cursor.
 */
void
xtermSetCursorBox(TScreen *screen)
{
    static XPoint VTbox[NBOX];
    XPoint *vp;
    int fw = FontWidth(screen) - 1;
    int fh = FontHeight(screen) - 1;
    int ww = isCursorBar(screen) ? fw / 8 : fw;
    int hh = isCursorUnderline(screen) ? fh / 8 : fh;
    if (ww < 2)
	ww = 2;
    if (hh < 2)
	hh = 2;

    vp = &VTbox[1];
    (vp++)->x = (short) ww;
    (vp++)->y = (short) hh;
    (vp++)->x = (short) -ww;
    vp->y = (short) -hh;

    screen->box = VTbox;
}

#if OPT_RENDERFONT

#define CACHE_XFT(data) if (XftFp(data) != NULL) {\
	    int err = checkXftWidth(xw, data);\
	    TRACE(("Xft metrics %s[%d] = %d (%d,%d)%s advance %d, actual %d%s%s\n",\
		#data,\
		fontnum,\
		XftFp(data)->height,\
		XftFp(data)->ascent,\
		XftFp(data)->descent,\
		((XftFp(data)->ascent + XftFp(data)->descent) > XftFp(data)->height ? "*" : ""),\
		XftFp(data)->max_advance_width,\
		data->font_info.min_width,\
		data->font_info.mixed ? " mixed" : "",\
		err ? " ERROR" : ""));\
	    if (err) {\
		xtermCloseXft(screen, data);\
		memset((data), 0, sizeof(*data));\
		failed += err;\
	    }\
	}

#if OPT_REPORT_FONTS
static FcChar32
xtermXftFirstChar(XftFont *xft)
{
    FcChar32 map[FC_CHARSET_MAP_SIZE];
    FcChar32 next;
    FcChar32 first;
    int i;

    first = FcCharSetFirstPage(xft->charset, map, &next);
    for (i = 0; i < FC_CHARSET_MAP_SIZE; i++) {
	if (map[i]) {
	    FcChar32 bits = map[i];
	    first += (FcChar32) i *32;
	    while (!(bits & 0x1)) {
		bits >>= 1;
		first++;
	    }
	    break;
	}
    }
    return first;
}

static FcChar32
xtermXftLastChar(XftFont *xft)
{
    FcChar32 temp, last, next;
    FcChar32 map[FC_CHARSET_MAP_SIZE];
    int i;
    last = FcCharSetFirstPage(xft->charset, map, &next);
    while ((temp = FcCharSetNextPage(xft->charset, map, &next)) != FC_CHARSET_DONE)
	last = temp;
    last &= (FcChar32) ~ 0xff;
    for (i = FC_CHARSET_MAP_SIZE - 1; i >= 0; i--) {
	if (map[i]) {
	    FcChar32 bits = map[i];
	    last += (FcChar32) i *32 + 31;
	    while (!(bits & 0x80000000)) {
		last--;
		bits <<= 1;
	    }
	    break;
	}
    }
    return (FcChar32) last;
}
#endif /* OPT_REPORT_FONTS */

#if OPT_TRACE

#if !OPT_WIDE_CHARS
static Char *
convertToUTF8(Char *buffer, int c)
{
    buffer[0] = (Char) c;
    buffer[1] = 0;
    return buffer;
}
#endif

static void
dumpXft(XtermWidget xw, XTermXftFonts *data)
{
    XftFont *xft = XftFp(data);
    TScreen *screen = TScreenOf(xw);
    VTwin *win = WhichVWin(screen);

    FcChar32 c;
    FcChar32 first = xtermXftFirstChar(xft);
    FcChar32 last = xtermXftLastChar(xft);
    FcChar32 dump;
    unsigned count = 0;
    unsigned too_high = 0;
    unsigned too_wide = 0;
    Boolean skip = False;

    TRACE(("dumpXft " TRACE_L "\n"));
    TRACE(("\tdata range U+%04X..U+%04X\n", first, last));
    TRACE(("\tcode\tcells\tdimensions\n"));
#if OPT_TRACE < 2
    dump = 255;
#else
    dump = last;
#endif
    for (c = first; c <= last; ++c) {
	if (FcCharSetHasChar(xft->charset, c)) {
	    int width = CharWidth(screen, c);
	    XGlyphInfo extents;
	    Boolean big_x;
	    Boolean big_y;

	    XftTextExtents32(XtDisplay(xw), xft, &c, 1, &extents);
	    big_x = (extents.width > win->f_width);
	    big_y = (extents.height > win->f_height);

	    if (c <= dump) {
		Char buffer[80];

		*convertToUTF8(buffer, c) = '\0';
		TRACE(("%s%s\tU+%04X\t%d\t%.1f x %.1f\t%s\n",
		       (big_y ? "y" : ""),
		       (big_x ? "x" : ""),
		       c, width,
		       ((double) extents.height) / win->f_height,
		       ((double) extents.width) / win->f_width,
		       buffer));
	    } else if (!skip) {
		skip = True;
		TRACE(("\t...skipping\n"));
	    }
	    if (big_y)
		++too_high;
	    if (big_x)
		++too_wide;
	    ++count;
	}
    }
    TRACE((TRACE_R " %u total, %u too-high, %u too-wide\n", count, too_high, too_wide));
}
#define DUMP_XFT(xw, data) dumpXft(xw, data)
#else
#define DUMP_XFT(xw, data)	/* nothing */
#endif

/*
 * Check if this is a FC_COLOR font, which fontconfig misrepresents to "fix" a
 * problem with web browsers.  As of 2018/12 (4 years later), Xft does not work
 * with that.  Even with this workaround, fontconfig has at least one bug which
 * causes it to crash (Debian #917034).
 */
#ifdef FC_COLOR
#define GetFcBool(pattern, what) \
    FcOK(FcPatternGetBool(pattern, what, 0, &fcbogus))

static Boolean
isBogusXft(XftFont *font)
{
    Boolean result = False;
    if (font != 0) {
	FcBool fcbogus;
	if (GetFcBool(font->pattern, FC_COLOR) && fcbogus) {
	    TRACE(("...matched color-bitmap font\n"));
#if !USE_FC_COLOR
	    result = True;
#endif
	} else if (GetFcBool(font->pattern, FC_OUTLINE) && !fcbogus) {
	    TRACE(("...matched non-outline font\n"));
	    /* This is legal for regular bitmap fonts - fontconfig attempts to
	     * find a match - but problematic for misencoded color-bitmap fonts.
	     */
	}
    }
    return result;
}
#endif

#if OPT_BOX_CHARS
static void
setBrokenBoxChars(XtermWidget xw, Bool state)
{
    TRACE(("setBrokenBoxChars %s\n", BtoS(state)));
    term->work.broken_box_chars = (Boolean) state;
    TScreenOf(xw)->broken_box_chars = (Boolean) state;
    update_font_boxchars();
}

#else
#define setBrokenBoxChars(xw, state)	/* nothing */
#endif

static Boolean
checkedXftWidth(Display *dpy,
		XTermXftFonts *source,
		unsigned limit,
		Dimension *width,
		FcChar32 c)
{
    Boolean result = False;

    if (FcCharSetHasChar(XftFp(source)->charset, c)) {
	XGlyphInfo extents;

	result = True;
	XftTextExtents32(dpy, XftFp(source), &c, 1, &extents);
	if (*width < extents.width && extents.width <= limit) {
	    *width = extents.width;
	}
    }
    return result;
}

/*
 * Check if the given character has a glyph known to Xft.  This is likely to be
 * slower than checking our cache.
 *
 * see xc/lib/Xft/xftglyphs.c
 */
static Bool
slowXftMissing(XtermWidget xw, XftFont *font, unsigned wc)
{
    TScreen *screen = TScreenOf(xw);
    Bool result = False;

    if (font != NULL) {
	if (XftCharIndex(screen->display, font, wc) == 0)
	    result = True;
    }
    return result;
}

static int
checkXftWidth(XtermWidget xw, XTermXftFonts *data)
{
    FcChar32 c;
    FcChar32 last = xtermXftLastChar(XftFp(data));
    Dimension limit = (Dimension) XftFp(data)->max_advance_width;
    Dimension width = 0;
    Dimension width2 = 0;
    int failed = 0;
#if OPT_WIDE_CHARS
    Cardinal n;
#endif

    data->font_info.min_width = 0;
    data->font_info.max_width = limit;

#if OPT_WIDE_CHARS
    /*
     * Check if the line-drawing characters are all provided in the font.
     * If so, take that into account for the cell-widths.
     */
    for (n = 0; n < XtNumber(unicode_boxes) - 1; ++n) {
	if (!checkedXftWidth(XtDisplay(xw),
			     data,
			     limit,
			     &width2, unicode_boxes[n].code)) {
	    width2 = 0;
	    TRACE(("font omits U+%04X line-drawing symbol\n",
		   unicode_boxes[n].code));
	    break;
	}
    }
#else
    (void) width2;
#endif

    if (width2 > 0) {
	Dimension check = (Dimension) (limit + 1) / 2;
	TRACE(("font provides VT100-style line-drawing\n"));
	/*
	 * The "VT100 line-drawing" characters happen to be all "ambiguous
	 * width" in Unicode's scheme.  That means that they could be twice as
	 * wide as the Latin-1 characters.
	 */
#define FC_ERR(n) (1.2 * (n))
	if (width2 > FC_ERR(check)) {
	    TRACE(("line-drawing characters appear to be double-width (ignore)\n"));
	    setBrokenBoxChars(xw, True);
	} else if (width2 > width) {
	    width = width2;
	}
    } else {
	TRACE(("font does NOT provide VT100-style line-drawing\n"));
	setBrokenBoxChars(xw, True);
    }

    /*
     * For each printable code, ask what its width is.  Given the maximum width
     * for those, we have a reasonable estimate of the single-column width.
     *
     * Ignore control characters - their extent information is misleading.
     */
    for (c = 32; c < 256; ++c) {
	if (CharWidth(TScreenOf(xw), c) <= 0)
	    continue;
	if (FcCharSetHasChar(XftFp(data)->charset, c)) {
	    (void) checkedXftWidth(XtDisplay(xw),
				   data,
				   data->font_info.max_width,
				   &width, c);
	}
    }

    /*
     * Sometimes someone uses a symbol font which has no useful ASCII or
     * Latin-1 characters.  Allow that, in case they did it intentionally.
     */
    if (width == 0) {
	failed = 1;
	if (last >= 256) {
	    width = data->font_info.max_width;
	}
    }
    data->font_info.min_width = width;
    data->font_info.mixed = (data->font_info.max_width >=
			     (data->font_info.min_width + 1));
    return failed;
}

#if OPT_TRACE
static const char *
nameOfXftFont(XftFont *fp)
{
    static char *result;
    char buffer[1024];
    FreeAndNull(result);
    if (XftNameUnparse(fp->pattern, buffer, (int) sizeof(buffer))) {
	char *target;
	char *source = buffer;
	if ((target = strtok(source, ":")) != 0) {
	    result = x_strdup(target);
	}
    }
    return NonNull(result);
}
#endif

#if OPT_REPORT_FONTS
static void
reportXftFonts(XtermWidget xw,
	       XTermXftFonts *fontData,
	       int fontNum,
	       XftFont *fp,
	       const char *name,
	       const char *tag,
	       XftPattern *match)
{
    if (resource.reportFonts) {
	char buffer[1024];
	FcChar32 first_char = xtermXftFirstChar(fp);
	FcChar32 last_char = xtermXftLastChar(fp);
	FcChar32 ch;
	unsigned missing = 0;

	ReportFonts("Loaded XftFonts(%s[%s])\n", name, tag);

	for (ch = first_char; ch <= last_char; ++ch) {
	    if (xtermXftMissing(xw, fontData, fontNum, fp, ch)) {
		++missing;
	    }
	}
	ReportFonts("\t\tfirst char:    %u\n", first_char);
	ReportFonts("\t\tlast char:     %u\n", last_char);
	ReportFonts("\t\tmissing-chars: %u\n", missing);
	ReportFonts("\t\tpresent-chars: %u\n", ((last_char - first_char)
						+ 1 - missing));

	if (XftNameUnparse(match, buffer, (int) sizeof(buffer))) {
	    char *target;
	    char *source = buffer;
	    while ((target = strtok(source, ":")) != 0) {
		ReportFonts("\t%s\n", target);
		source = 0;
	    }
	}
	fflush(stdout);
    }
}

static void
reportXftFallbackFont(XtermWidget xw,
		      XTermXftFonts *fontData,
		      int fontNum,
		      XftFont *font,
		      XftPattern *match)
{
    if (resource.reportFonts) {
	char tag[80];
	sprintf(tag, "%s#%d",
		whichXftFonts(xw, fontData),
		fontNum + 1);
	reportXftFonts(xw, fontData, fontNum, font, "fallback", tag, match);
    }
}

#else
#define reportXftFonts(xw, fontData, fontNum, result, name, tag, match)		/* empty */
#define reportXftFallbackFont(xw, fontData, fontNum, font, match)	/* empty */
#endif /* OPT_REPORT_FONTS */

/*
 * Xft discards the pattern-match during open-pattern if the result happens to
 * match a currently-open file, but provides no clue to the caller when it does
 * this.  That is, closing a font-file may leave the data in Xft's cache, while
 * opening a file may free the data used for the match.
 *
 * Because of this problem, we cannot reliably refer to the pattern-match data
 * if it may have been seen before.
 */
Boolean
maybeXftCache(XtermWidget xw, XftFont *font)
{
    Boolean result = False;
    if (font != NULL) {
	TScreen *screen = TScreenOf(xw);
	ListXftFonts *p;
	for (p = screen->list_xft_fonts; p != NULL; p = p->next) {
	    if (p->font == font) {
		result = True;
		break;
	    }
	}
	if (!result) {
	    p = TypeXtMalloc(ListXftFonts);
	    if (p != NULL) {
		p->font = font;
		p->next = screen->list_xft_fonts;
		screen->list_xft_fonts = p;
	    }
	}
    }
    return result;
}

/*
 * Drop an entry from the cache, and close the font.
 */
void
closeCachedXft(TScreen *screen, XftFont *font)
{
    if (font != 0) {
	ListXftFonts *p, *q;

	for (p = screen->list_xft_fonts, q = 0; p != 0; q = p, p = p->next) {
	    if (p->font == font) {
		XftFontClose(screen->display, font);
		if (q != 0) {
		    q->next = p->next;
		} else {
		    screen->list_xft_fonts = p->next;
		}
		free(p);
		break;
	    }
	}
    }
}

static void
xtermOpenXft(XtermWidget xw,
	     XTermXftFonts *fontData,
	     int fontNum,
	     const char *name,
	     XftPattern *pat,
	     const char *tag)
{
    TScreen *screen = TScreenOf(xw);
    Display *dpy = screen->display;
    XftResult status;
    XftFont *result = 0;

    TRACE(("xtermOpenXft(name=%s, tag=%s)\n", name, tag));
    if (pat != 0 && (fontNum <= MaxXftCache)) {
	XftPattern *match;

	FcConfigSubstitute(NULL, pat, FcMatchPattern);
	XftDefaultSubstitute(dpy, DefaultScreen(dpy), pat);

	match = FcFontMatch(NULL, pat, &status);
	if (match != 0) {
	    Boolean maybeReopened = False;
	    result = XftFontOpenPattern(dpy, match);
#ifdef FC_COLOR
	    if (result != NULL) {
		if (isBogusXft(result)) {
		    XftFontClose(dpy, result);
		    result = NULL;
		    maybeReopened = True;
		}
	    }
#endif
	    if (result != NULL) {
		TRACE(("...matched %s font\n", tag));
		if (fontData->fs_size < fontNum)
		    fontData->fs_size = fontNum;
		XftFpN(fontData, fontNum) = result;
		XftIsN(fontData, fontNum) = xcOpened;
		if (!maybeXftCache(xw, result)) {
		    reportXftFonts(xw, fontData, fontNum, result, name, tag, match);
		}
	    } else {
		TRACE(("...could not open %s font\n", tag));
		if (!maybeReopened)
		    XftPatternDestroy(match);
		if (xw->misc.fontWarnings >= fwAlways) {
		    cannotFont(xw, "open", tag, name);
		}
	    }
	} else {
	    TRACE(("...did not match %s font\n", tag));
	    if (xw->misc.fontWarnings >= fwResource) {
		cannotFont(xw, "match", tag, name);
	    }
	}
    }
    if (result == NULL && (fontNum <= MaxXftCache)) {
	XftFpN(fontData, fontNum) = NULL;
	XftIsN(fontData, fontNum) = xcEmpty;
    }
}

#if OPT_SHIFT_FONTS
/*
 * Don't make a dependency on the math library for a single function.
 * (Newton Raphson).
 */
static double
dimSquareRoot(double value)
{
    double result = 0.0;
    if (value > 0.0) {
	int n;
	double older = value;
	for (n = 0; n < 10; ++n) {
	    double delta = (older * older - value) / (2.0 * older);
	    double newer = older - delta;
	    older = newer;
	    result = newer;
	    if (delta > -0.001 && delta < 0.001)
		break;
	}
    }
    return result;
}
#endif

#ifdef DEBUG_XFT
static void
trace_xft_glyph(XtermWidget xw, XTermXftFonts *data, FT_Face face, int code, const char *name)
{
    if (xtermXftMissing(xw, data, 0, XftFp(data), code)) {
	TRACE(("Xft glyph U+%04X missing :%s\n", code, name));
    } else if (FT_Load_Char(face, code, FT_LOAD_RENDER) == 0) {
	FT_GlyphSlot g = face->glyph;
	TRACE(("Xft glyph U+%04X size(%3d,%3d) at(%3d,%3d) :%s\n",
	       code,
	       g->bitmap.rows, g->bitmap.width,
	       g->bitmap_top, g->bitmap_left,
	       name));
    }
}

#if OPT_WIDE_CHARS
static void
trace_xft_line_drawing(XtermWidget xw, XTermXftFonts *data, FT_Face face)
{
    int n;
    for (n = 0; unicode_boxes[n].code != 0; ++n) {
	trace_xft_glyph(xw, data, face, unicode_boxes[n].code,
			unicode_boxes[n].name);
    }
}
#else
#define trace_xft_line_drawing(xw, data, face)	/* nothing */
#endif
#endif /* DEBUG_XFT */

/*
 * Check if the line-drawing characters do not fill the bounding box.  If so,
 * they're not useful.
 */
#if OPT_BOX_CHARS
static void
linedrawing_gaps(XtermWidget xw, XTermXftFonts *data)
{
    Boolean broken;

#if OPT_WIDE_CHARS
    TScreen *screen = TScreenOf(xw);
    int n;
    FT_Face face;
    face = XftLockFace(XftFp(data));
    broken = False;
    for (n = 0; unicode_boxes[n].code; ++n) {
	unsigned code = unicode_boxes[n].code;

	if (xtermXftMissing(xw, data, 0, XftFp(data), code)) {
	    TRACE(("Xft glyph U+%04X is missing\n", code));
	    broken = True;
	    break;
	}

	if (FT_Load_Char(face, code, FT_LOAD_RENDER) == 0) {
	    FT_GlyphSlot g = face->glyph;
	    TRACE(("Xft glyph U+%04X size(%3d,%3d) at(%3d,%3d) :%s\n",
		   code,
		   g->bitmap.rows, g->bitmap.width,
		   g->bitmap_top, g->bitmap_left,
		   unicode_boxes[n].name));
	    /*
	     * While it is possible for badly-designed fonts to have line
	     * drawing characters which do not meet, FreeType aggravates the
	     * situation with its rounding.  Check for an obvious case where
	     * the weights at the ends of a vertical line do not add up.  That
	     * shows up as two under-weight rows at the beginning/end of the
	     * bitmap.
	     */
	    if (code == 0x2502) {
		unsigned r, c;
		unsigned mids = 0, ends = 0;
		unsigned char *buffer = g->bitmap.buffer;

		switch (g->bitmap.pixel_mode) {
		case FT_PIXEL_MODE_MONO:
		    /* FALLTHRU */
		case FT_PIXEL_MODE_GRAY:
		    for (r = 0; r < (unsigned) g->bitmap.rows; ++r) {
			unsigned k = r * (unsigned) g->bitmap.pitch;
			unsigned sum = 0;
			for (c = 0; c < (unsigned) g->bitmap.width; ++c) {
			    unsigned xx = 0;
			    switch (g->bitmap.pixel_mode) {
			    case FT_PIXEL_MODE_MONO:
				xx = (unsigned) ((buffer[k + (c / 8)]
						  >> (c % 8)) & 1);
				break;
			    case FT_PIXEL_MODE_GRAY:
				xx = buffer[k + c];
				break;
			    }
			    sum += xx;
			    TRACE2((" %2x", xx));
			}
			TRACE2((" = %u\n", sum));
			if (r > 0 && (r + 1) < (unsigned) g->bitmap.rows) {
			    mids = sum;
			} else {
			    ends += sum;
			}
		    }
		    TRACE(("...compare middle %u vs ends %u\n", mids, ends));
		    if ((mids > ends) && (g->bitmap.rows < 16))
			broken = True;
		    break;
		default:
		    TRACE(("FIXME pixel_mode %d not handled\n",
			   g->bitmap.pixel_mode));
		    break;
		}
		if (broken)
		    break;
	    }
	    /*
	     * The factor of two accounts for line-drawing that goes through
	     * the middle of a cell, possibly leaving half of the cell unused.
	     * A horizontal line has to extend the full width of the cell.
	     */
	    switch (unicode_boxes[n].high) {
	    case 1:
		if ((unsigned) g->bitmap.rows < (unsigned) FontHeight(screen)) {
		    TRACE(("...bitmap is shorter than full-cell (%u vs %u)\n",
			   (unsigned) g->bitmap.rows,
			   (unsigned) FontHeight(screen)));
		    broken = True;
		}
		break;
	    case 2:
		if ((unsigned) (g->bitmap.rows * 2) < (unsigned) FontHeight(screen)) {
		    TRACE(("...bitmap is too short for half-cell (%u vs %u)\n",
			   (unsigned) (g->bitmap.rows * 2),
			   (unsigned) FontHeight(screen)));
		    broken = True;
		}
		break;
	    }
	    switch (unicode_boxes[n].wide) {
	    case 1:
		if ((unsigned) g->bitmap.width < (unsigned) FontWidth(screen)) {
		    TRACE(("...bitmap is narrower than full-cell (%u vs %u)\n",
			   (unsigned) g->bitmap.width,
			   (unsigned) FontWidth(screen)));
		    broken = True;
		}
		break;
	    case 2:
		if ((unsigned) (g->bitmap.width * 2) < (unsigned) FontWidth(screen)) {
		    TRACE(("...bitmap is too narrow for half-cell (%u vs %u)\n",
			   (unsigned) (g->bitmap.width * 2),
			   (unsigned) FontWidth(screen)));
		    broken = True;
		}
		break;
	    }
	    if (broken)
		break;
	}
    }
    XftUnlockFace(XftFp(data));
#else
    (void) data;
    broken = True;
#endif

    if (broken) {
	TRACE(("Xft line-drawing would not work\n"));
	setBrokenBoxChars(xw, True);
    }
}
#endif /* OPT_BOX_CHARS */

/*
 * Given the Xft font metrics, determine the actual font size.  This is used
 * for each font to ensure that normal, bold and italic fonts follow the same
 * rule.
 */
static void
setRenderFontsize(XtermWidget xw, VTwin *win, XTermXftFonts *data, const char *tag)
{
    XftFont *font = XftFp(data);
    if (font != NULL) {
	TScreen *screen = TScreenOf(xw);
	int width, height, ascent, descent;
#ifdef DEBUG_XFT
	int n;
	FT_Face face;
	FT_Size size;
	FT_Size_Metrics metrics;
	Boolean scalable;
	Boolean is_fixed;
	Boolean debug_xft = False;

	face = XftLockFace(font);
	size = face->size;
	metrics = size->metrics;
	is_fixed = FT_IS_FIXED_WIDTH(face);
	scalable = FT_IS_SCALABLE(face);
	trace_xft_line_drawing(xw, data, face);
	for (n = 32; n < 127; ++n) {
	    char name[80];
	    sprintf(name, "letter \"%c\"", n);
	    trace_xft_glyph(xw, data, face, n, name);
	}
	XftUnlockFace(font);

	/* freetype's inconsistent for this sign */
	metrics.descender = -metrics.descender;

#define TR_XFT	   "Xft metrics: "
#define D_64(name) ((double)(metrics.name)/64.0)
#define M_64(a,b)  ((font->a * 64) != metrics.b)
#define BOTH(a,b)  D_64(b), M_64(a,b) ? "*" : ""

	debug_xft = (M_64(ascent, ascender)
		     || M_64(descent, descender)
		     || M_64(height, height)
		     || M_64(max_advance_width, max_advance));

	TRACE(("Xft font is %sscalable, %sfixed-width\n",
	       is_fixed ? "" : "not ",
	       scalable ? "" : "not "));

	if (debug_xft) {
	    TRACE(("Xft font size %d+%d vs %d by %d\n",
		   font->ascent,
		   font->descent,
		   font->height,
		   font->max_advance_width));
	    TRACE((TR_XFT "ascender    %6.2f%s\n", BOTH(ascent, ascender)));
	    TRACE((TR_XFT "descender   %6.2f%s\n", BOTH(descent, descender)));
	    TRACE((TR_XFT "height      %6.2f%s\n", BOTH(height, height)));
	    TRACE((TR_XFT "max_advance %6.2f%s\n", BOTH(max_advance_width, max_advance)));
	} else {
	    TRACE((TR_XFT "matches font\n"));
	}
#endif

	width = font->max_advance_width;
	height = font->height;
	ascent = font->ascent;
	descent = font->descent;
	if (screen->force_xft_height && height < ascent + descent) {
	    TRACE(("...height is less than ascent + descent (%u vs %u)\n",
		   height, ascent + descent));
	    if ((ascent + descent) > (height + 1)) {
		/* this happens less than 10% of the time */
		--ascent;
		--descent;
		TRACE(("...decrement both ascent and descent before retry\n"));
	    } else if (ascent > descent) {
		/* this is the usual case */
		--ascent;
		TRACE(("...decrement ascent before retry\n"));
	    } else {
		/* this could happen, though rare... */
		--descent;
		TRACE(("...decrement descent before retry\n"));
	    }
	    height = ascent + descent;
	    font->ascent = ascent;
	    font->descent = descent;
	    TRACE(("...updated height %d vs %d (ascent %d, descent %d)\n",
		   height, ascent + descent, ascent, descent));
	}
	if (is_double_width_font_xft(screen->display, font)) {
	    TRACE(("...reduce width from %d to %d\n", width, width >> 1));
	    width >>= 1;
	}
	if (tag == 0) {
	    SetFontWidth(screen, win, width);
	    SetFontHeight(screen, win, height);
	    win->f_ascent = ascent;
	    win->f_descent = descent;
	    TRACE(("setRenderFontsize result %dx%d (%d+%d)\n",
		   width, height, ascent, descent));
	} else if (win->f_width < width ||
		   win->f_height < height ||
		   win->f_ascent < ascent ||
		   win->f_descent < descent) {
	    TRACE(("setRenderFontsize %s changed %dx%d (%d+%d) to %dx%d (%d+%d)\n",
		   tag,
		   win->f_width, win->f_height, win->f_ascent, win->f_descent,
		   width, height, ascent, descent));

	    SetFontWidth(screen, win, width);
	    SetFontHeight(screen, win, height);
	    win->f_ascent = ascent;
	    win->f_descent = descent;
	} else {
	    TRACE(("setRenderFontsize %s unchanged\n", tag));
	}
#if OPT_BOX_CHARS
	if (!screen->broken_box_chars && (tag == 0)) {
	    linedrawing_gaps(xw, data);
	}
#endif
    }
}
#endif

static void
checkFontInfo(int value, const char *tag, int failed)
{
    if (value == 0 || failed) {
	if (value == 0) {
	    xtermWarning("Selected font has no non-zero %s for ISO-8859-1 encoding\n", tag);
	    exit(ERROR_MISC);
	} else {
	    xtermWarning("Selected font has no valid %s for ISO-8859-1 encoding\n", tag);
	}
    }
}

#if OPT_RENDERFONT
void
xtermCloseXft(TScreen *screen, XTermXftFonts *pub)
{
    if (XftFp(pub) != NULL) {
	int n;

	if (pub->pattern) {
	    XftPatternDestroy(pub->pattern);
	    pub->pattern = NULL;
	}
	if (pub->fontset) {
	    XftFontSetDestroy(pub->fontset);
	    pub->fontset = NULL;
	}

	for (n = 0; n <= pub->fs_size; ++n) {
	    if (XftFpN(pub, n) != NULL) {
		closeCachedXft(screen, XftFpN(pub, n));
		XftFpN(pub, n) = NULL;
		XftIsN(pub, n) = xcEmpty;
	    }
	}
	FreeAndNull(pub->font_map.per_font);
	memset(pub, 0, sizeof(*pub));
    }
}

/*
 * Get the faceName/faceNameDoublesize resource setting.
 */
String
getFaceName(XtermWidget xw, Bool wideName)
{
#if OPT_RENDERWIDE
    String result = (wideName
		     ? FirstItemOf(xw->work.fonts.xft.list_w)
		     : CurrentXftFont(xw));
#else
    String result = CurrentXftFont(xw);
    (void) wideName;
#endif
    return x_nonempty(result);
}

/*
 * If we change the faceName, we'll have to re-acquire all of the fonts that
 * are derived from it.
 */
void
setFaceName(XtermWidget xw, const char *value)
{
    TScreen *screen = TScreenOf(xw);
    Boolean changed = (Boolean) ((CurrentXftFont(xw) == 0)
				 || strcmp(CurrentXftFont(xw), value));

    if (changed) {
	int n;

	CurrentXftFont(xw) = x_strdup(value);
	for (n = 0; n < NMENUFONTS; ++n) {
	    int e;
	    xw->misc.face_size[n] = -1.0;
	    for (e = 0; e < fMAX; ++e) {
		xtermCloseXft(screen, getMyXftFont(xw, e, n));
	    }
	}
    }
}
#endif

/*
 * Compute useful values for the font/window sizes
 */
void
xtermComputeFontInfo(XtermWidget xw,
		     VTwin *win,
		     XFontStruct *font,
		     int sbwidth)
{
    TScreen *screen = TScreenOf(xw);

    int i, j, width, height;
#if OPT_RENDERFONT
    int fontnum = screen->menu_font_number;
#endif
    int failed = 0;

#if OPT_RENDERFONT
    /*
     * xterm contains a lot of references to fonts, assuming they are fixed
     * size.  This chunk of code overrides the actual font-selection (see
     * drawXtermText()), if the user has selected render-font.  All of the
     * font-loading for fixed-fonts still goes on whether or not this chunk
     * overrides it.
     */
    if (UsingRenderFont(xw) && fontnum >= 0) {
	String face_name = getFaceName(xw, False);
	XTermXftFonts *norm = &(screen->renderFontNorm[fontnum]);
	XTermXftFonts *bold = &(screen->renderFontBold[fontnum]);
	XTermXftFonts *ital = &(screen->renderFontItal[fontnum]);
	XTermXftFonts *btal = &(screen->renderFontBtal[fontnum]);
#if OPT_RENDERWIDE
	XTermXftFonts *wnorm = &(screen->renderWideNorm[fontnum]);
	XTermXftFonts *wbold = &(screen->renderWideBold[fontnum]);
	XTermXftFonts *wital = &(screen->renderWideItal[fontnum]);
	XTermXftFonts *wbtal = &(screen->renderWideBtal[fontnum]);
#endif

	if (XftFp(norm) == 0 && !IsEmpty(face_name)) {
	    Work *work = &(xw->work);
	    XftPattern *pat;
	    double face_size;

	    TRACE(("xtermComputeFontInfo font %d: norm(face %s, size %.1f)\n",
		   fontnum, face_name,
		   xw->misc.face_size[fontnum]));

	    TRACE(("Using Xft %d\n", XftGetVersion()));
	    TRACE(("Using FontConfig %d\n", FC_VERSION));

	    if (work->xft_defaults == NULL) {
		FcInit();
		work->xft_defaults = FcPatternCreate();
		XftDefaultSubstitute(screen->display,
				     XScreenNumberOfScreen(XtScreen(xw)),
				     work->xft_defaults);
		if (screen->xft_max_glyph_memory > 0) {
		    FcPatternAddInteger(work->xft_defaults,
					XFT_MAX_GLYPH_MEMORY,
					screen->xft_max_glyph_memory);
		}
		if (screen->xft_max_unref_fonts > 0) {
		    FcPatternAddInteger(work->xft_defaults,
					XFT_MAX_UNREF_FONTS,
					screen->xft_max_unref_fonts);
		}
#ifdef XFT_TRACK_MEM_USAGE
		FcPatternAddBool(work->xft_defaults,
				 XFT_TRACK_MEM_USAGE,
				 screen->xft_track_mem_usage);
#endif
		XftDefaultSet(screen->display, work->xft_defaults);
	    }

	    fillInFaceSize(xw, fontnum);
	    face_size = (double) xw->misc.face_size[fontnum];

	    /*
	     * By observation (there is no documentation), XftPatternBuild is
	     * cumulative.  Build the bold- and italic-patterns on top of the
	     * normal pattern.
	     */
#ifdef FC_COLOR
#if USE_FC_COLOR
#define NormXftPattern \
	    XFT_FAMILY,     XftTypeString, "mono", \
	    FC_OUTLINE,     XftTypeBool,   FcTrue, \
	    XFT_SIZE,       XftTypeDouble, face_size
#else
#define NormXftPattern \
	    XFT_FAMILY,     XftTypeString, "mono", \
	    FC_COLOR,       XftTypeBool,   FcFalse, \
	    FC_OUTLINE,     XftTypeBool,   FcTrue, \
	    XFT_SIZE,       XftTypeDouble, face_size
#endif
#else
#define NormXftPattern \
	    XFT_FAMILY,     XftTypeString, "mono", \
	    XFT_SIZE,       XftTypeDouble, face_size
#endif

#define BoldXftPattern(norm) \
	    XFT_WEIGHT,     XftTypeInteger, XFT_WEIGHT_BOLD, \
	    XFT_CHAR_WIDTH, XftTypeInteger, XftFp(norm)->max_advance_width

#define ItalXftPattern(norm) \
	    XFT_SLANT,      XftTypeInteger, XFT_SLANT_ITALIC, \
	    XFT_CHAR_WIDTH, XftTypeInteger, XftFp(norm)->max_advance_width

#define BtalXftPattern(norm) \
	    XFT_WEIGHT,     XftTypeInteger, XFT_WEIGHT_BOLD, \
	    XFT_SLANT,      XftTypeInteger, XFT_SLANT_ITALIC, \
	    XFT_CHAR_WIDTH, XftTypeInteger, XftFp(norm)->max_advance_width

#if OPT_WIDE_ATTRS
#define HAVE_ITALICS 1
#define FIND_ITALICS ((pat = XftNameParse(face_name)) != 0)
#elif OPT_ISO_COLORS
#define HAVE_ITALICS 1
#define FIND_ITALICS (screen->italicULMode && (pat = XftNameParse(face_name)) != 0)
#else
#define HAVE_ITALICS 0
#endif

#if OPT_DEC_CHRSET
	    freeall_DoubleFT(xw);
#endif
	    if ((pat = XftNameParse(face_name)) != 0) {
#define OPEN_XFT(data, tag) xtermOpenXft(xw, data, 0, face_name, data->pattern, tag)
		norm->pattern = XftPatternDuplicate(pat);
		XftPatternBuild(norm->pattern,
				NormXftPattern,
				(void *) 0);
		OPEN_XFT(norm, "normal");

		if (XftFp(norm) != 0) {
		    bold->pattern = XftPatternDuplicate(pat);
		    XftPatternBuild(bold->pattern,
				    NormXftPattern,
				    BoldXftPattern(norm),
				    (void *) 0);
		    OPEN_XFT(bold, "bold");

#if HAVE_ITALICS
		    if (FIND_ITALICS) {
			ital->pattern = XftPatternDuplicate(pat);
			XftPatternBuild(ital->pattern,
					NormXftPattern,
					ItalXftPattern(norm),
					(void *) 0);
			OPEN_XFT(ital, "italic");
			btal->pattern = XftPatternDuplicate(pat);
			XftPatternBuild(btal->pattern,
					NormXftPattern,
					BtalXftPattern(norm),
					(void *) 0);
			OPEN_XFT(btal, "bold-italic");
		    }
#endif

		    /*
		     * FIXME:  just assume that the corresponding font has no
		     * graphics characters.
		     */
		    if (screen->fnt_boxes) {
			screen->fnt_boxes = 0;
			TRACE(("Xft opened - will not use internal line-drawing characters\n"));
		    }
		}

		CACHE_XFT(norm);

		CACHE_XFT(bold);
		if (XftFp(norm) != 0 && !XftFp(bold)) {
		    noUsableXft(xw, "bold");
		    XftPatternDestroy(bold->pattern);
		    bold->pattern = XftPatternDuplicate(pat);
		    XftPatternBuild(bold->pattern,
				    NormXftPattern,
				    (void *) 0);
		    OPEN_XFT(bold, "bold");
		    failed = 0;
		    CACHE_XFT(bold);
		}
#if HAVE_ITALICS
		CACHE_XFT(ital);
		if (XftFp(norm) != 0 && !XftFp(ital)) {
		    noUsableXft(xw, "italic");
		    XftPatternDestroy(ital->pattern);
		    ital->pattern = XftPatternDuplicate(pat);
		    XftPatternBuild(ital->pattern,
				    NormXftPattern,
				    (void *) 0);
		    OPEN_XFT(ital, "italics");
		    failed = 0;
		    CACHE_XFT(ital);
		}
		CACHE_XFT(btal);
		if (XftFp(norm) != 0 && !XftFp(btal)) {
		    noUsableXft(xw, "bold italic");
		    XftPatternDestroy(btal->pattern);
		    btal->pattern = XftPatternDuplicate(pat);
		    XftPatternBuild(btal->pattern,
				    NormXftPattern,
				    (void *) 0);
		    OPEN_XFT(btal, "bold-italics");
		    failed = 0;
		    CACHE_XFT(btal);
		}
#endif
		XftPatternDestroy(pat);
	    } else {
		failed = 1;
	    }

	    /*
	     * See xtermXftDrawString().  A separate double-width font is nice
	     * to have, but not essential.
	     */
#if OPT_RENDERWIDE
	    if (XftFp(norm) != 0 && screen->wide_chars) {
		int char_width = XftFp(norm)->max_advance_width * 2;
		double aspect = ((FirstItemOf(xw->work.fonts.xft.list_w)
				  || screen->renderFontNorm[fontnum].font_info.mixed)
				 ? 1.0
				 : 2.0);

		face_name = getFaceName(xw, True);
		TRACE(("xtermComputeFontInfo wide(face %s, char_width %d)\n",
		       NonNull(face_name),
		       char_width));

#define WideXftPattern \
		XFT_FAMILY,     XftTypeString,   "mono", \
		XFT_SIZE,       XftTypeDouble,   face_size, \
		XFT_SPACING,    XftTypeInteger,  XFT_MONO, \
		XFT_CHAR_WIDTH, XftTypeInteger,  char_width, \
		FC_ASPECT,      XftTypeDouble,   aspect

		if (!IsEmpty(face_name) && (pat = XftNameParse(face_name))
		    != 0) {
		    wnorm->pattern = XftPatternDuplicate(pat);
		    XftPatternBuild(wnorm->pattern,
				    WideXftPattern,
				    (void *) 0);
		    OPEN_XFT(wnorm, "wide");

		    if (XftFp(wnorm) != 0) {
			wbold->pattern = XftPatternDuplicate(pat);
			XftPatternBuild(wbold->pattern,
					WideXftPattern,
					BoldXftPattern(wnorm),
					(void *) 0);
			OPEN_XFT(wbold, "wide-bold");

#if HAVE_ITALICS
			if (FIND_ITALICS) {
			    wital->pattern = XftPatternDuplicate(pat);
			    XftPatternBuild(wital->pattern,
					    WideXftPattern,
					    ItalXftPattern(wnorm),
					    (void *) 0);
			    OPEN_XFT(wital, "wide-italic");
			}
			CACHE_XFT(wbtal);
			if (!XftFp(wbtal)) {
			    noUsableXft(xw, "wide bold");
			    XftPatternDestroy(wbtal->pattern);
			    wbtal->pattern = XftPatternDuplicate(pat);
			    XftPatternBuild(wbtal->pattern,
					    WideXftPattern,
					    (void *) 0);
			    OPEN_XFT(wbtal, "wide-bold-italics");
			    failed = 0;
			    CACHE_XFT(wbtal);
			}
#endif
		    }

		    CACHE_XFT(wnorm);

		    CACHE_XFT(wbold);
		    if (XftFp(wnorm) != 0 && !XftFp(wbold)) {
			noUsableXft(xw, "wide-bold");
			XftPatternDestroy(wbold->pattern);
			wbold->pattern = XftPatternDuplicate(pat);
			XftPatternBuild(bold->pattern,
					WideXftPattern,
					(void *) 0);
			OPEN_XFT(wbold, "wide-bold");
			failed = 0;
			CACHE_XFT(bold);
		    }

		    CACHE_XFT(wital);
		    if (XftFp(wnorm) != 0 && !XftFp(wital)) {
			noUsableXft(xw, "wide-italic");
			XftPatternDestroy(wital->pattern);
			wital->pattern = XftPatternDuplicate(pat);
			XftPatternBuild(wital->pattern,
					WideXftPattern,
					(void *) 0);
			OPEN_XFT(wital, "wide-italic");
			failed = 0;
			CACHE_XFT(wital);
		    }

		    XftPatternDestroy(pat);
		}
#undef OPEN_XFT
	    }
#endif /* OPT_RENDERWIDE */
	}
	if (XftFp(norm) == 0) {
	    TRACE(("...no TrueType font found for number %d, disable menu entry\n", fontnum));
	    xw->work.render_font = False;
	    update_font_renderfont();
	    /* now we will fall through into the bitmap fonts */
	} else {
	    setBrokenBoxChars(xw, False);
	    setRenderFontsize(xw, win, norm, NULL);
	    setRenderFontsize(xw, win, bold, "bold");
	    setRenderFontsize(xw, win, ital, "ital");
	    setRenderFontsize(xw, win, btal, "btal");
#if OPT_BOX_CHARS
	    setupPackedFonts(xw);

	    if (screen->force_packed) {
		XTermXftFonts *use = &(screen->renderFontNorm[fontnum]);
		SetFontHeight(screen, win, XftFp(use)->ascent + XftFp(use)->descent);
		SetFontWidth(screen, win, use->font_info.min_width);
		TRACE(("...packed TrueType font %dx%d vs %d\n",
		       win->f_height,
		       win->f_width,
		       use->font_info.max_width));
	    }
#endif
	    DUMP_XFT(xw, &(screen->renderFontNorm[fontnum]));
	}
    }
    /*
     * Are we handling a bitmap font?
     */
    else
#endif /* OPT_RENDERFONT */
    {
	if (is_double_width_font(font) && !(screen->fnt_prop)) {
	    SetFontWidth(screen, win, font->min_bounds.width);
	} else {
	    SetFontWidth(screen, win, font->max_bounds.width);
	}
	SetFontHeight(screen, win, font->ascent + font->descent);
	win->f_ascent = font->ascent;
	win->f_descent = font->descent;
    }
    i = 2 * screen->border + sbwidth;
    j = 2 * screen->border;
    width = MaxCols(screen) * win->f_width + i;
    height = MaxRows(screen) * win->f_height + j;
    win->fullwidth = (Dimension) width;
    win->fullheight = (Dimension) height;
    win->width = width - i;
    win->height = height - j;

    TRACE(("xtermComputeFontInfo window %dx%d (full %dx%d), fontsize %dx%d (asc %d, dsc %d)\n",
	   win->height,
	   win->width,
	   win->fullheight,
	   win->fullwidth,
	   win->f_height,
	   win->f_width,
	   win->f_ascent,
	   win->f_descent));

    checkFontInfo(win->f_height, "height", failed);
    checkFontInfo(win->f_width, "width", failed);
}

/* save this information as a side-effect for double-sized characters */
static void
xtermSaveFontInfo(TScreen *screen, XFontStruct *font)
{
    screen->fnt_wide = (Dimension) (font->max_bounds.width);
    screen->fnt_high = (Dimension) (font->ascent + font->descent);
    TRACE(("xtermSaveFontInfo %dx%d\n", screen->fnt_high, screen->fnt_wide));
}

/*
 * After loading a new font, update the structures that use its size.
 */
void
xtermUpdateFontInfo(XtermWidget xw, Bool doresize)
{
    TScreen *screen = TScreenOf(xw);

    int scrollbar_width;
    VTwin *win = &(screen->fullVwin);

#if USE_DOUBLE_BUFFER
    discardRenderDraw(TScreenOf(xw));
#endif /* USE_DOUBLE_BUFFER */

    scrollbar_width = (xw->misc.scrollbar
		       ? (screen->scrollWidget->core.width +
			  BorderWidth(screen->scrollWidget))
		       : 0);
    xtermComputeFontInfo(xw, win, GetNormalFont(screen, fNorm)->fs, scrollbar_width);
    xtermSaveFontInfo(screen, GetNormalFont(screen, fNorm)->fs);

    if (doresize) {
	if (VWindow(screen)) {
	    xtermClear(xw);
	}
	TRACE(("xtermUpdateFontInfo " TRACE_L "\n"));
	DoResizeScreen(xw);	/* set to the new natural size */
	ResizeScrollBar(xw);
	Redraw();
	TRACE((TRACE_R " xtermUpdateFontInfo\n"));
#ifdef SCROLLBAR_RIGHT
	updateRightScrollbar(xw);
#endif
    }
    xtermSetCursorBox(screen);
}

#if OPT_BOX_CHARS || OPT_REPORT_FONTS

/*
 * Returns true if the given character is missing from the specified font.
 */
Bool
xtermMissingChar(unsigned ch, XTermFonts * font)
{
    Bool result = False;
    XFontStruct *fs = font->fs;
    XCharStruct *pc = 0;

    if (fs == NULL) {
	result = True;
    } else if (fs->max_byte1 == 0) {
#if OPT_WIDE_CHARS
	if (ch < 256)
#endif
	{
	    CI_GET_CHAR_INFO_1D(fs, ch, pc);
	}
    }
#if OPT_WIDE_CHARS
    else {
	unsigned row = (ch >> 8);
	unsigned col = (ch & 0xff);
	CI_GET_CHAR_INFO_2D(fs, row, col, pc);
    }
#endif

    if (pc == 0 || CI_NONEXISTCHAR(pc)) {
	TRACE2(("xtermMissingChar %#04x (!exists)\n", ch));
	result = True;
    }
    if (ch < MaxUChar) {
	font->known_missing[ch] = (Char) (result ? 2 : 1);
    }
    return result;
}
#endif

#if OPT_BOX_CHARS || OPT_WIDE_CHARS
/*
 * The grid is arbitrary, enough resolution that nothing's lost in
 * initialization.
 */
#define BOX_HIGH 60
#define BOX_WIDE 60

#define MID_HIGH (BOX_HIGH/2)
#define MID_WIDE (BOX_WIDE/2)

#define CHR_WIDE ((9*BOX_WIDE)/10)
#define CHR_HIGH ((9*BOX_HIGH)/10)

/*
 * ...since we'll scale the values anyway.
 */
#define Scale_XY(n,d,f) ((int)(n) * ((int)(f))) / (d)
#define SCALED_X(n) Scale_XY(n, BOX_WIDE, font_width)
#define SCALED_Y(n) Scale_XY(n, BOX_HIGH, font_height)
#define SCALE_X(n) n = SCALED_X(n)
#define SCALE_Y(n) n = SCALED_Y(n)

#define SEG(x0,y0,x1,y1) x0,y0, x1,y1

/*
 * Draw the given graphic character, if it is simple enough (i.e., a
 * line-drawing character).
 */
void
xtermDrawBoxChar(XTermDraw * params,
		 unsigned ch,
		 GC gc,
		 int x,
		 int y,
		 int cells,
		 Bool xftords)
{
    TScreen *screen = TScreenOf(params->xw);
    /* *INDENT-OFF* */
    static const short glyph_ht[] = {
	SEG(1*BOX_WIDE/10,  0,		1*BOX_WIDE/10,5*MID_HIGH/6),	/* H */
	SEG(6*BOX_WIDE/10,  0,		6*BOX_WIDE/10,5*MID_HIGH/6),
	SEG(1*BOX_WIDE/10,5*MID_HIGH/12,6*BOX_WIDE/10,5*MID_HIGH/12),
	SEG(2*BOX_WIDE/10,  MID_HIGH,	  CHR_WIDE,	MID_HIGH),	/* T */
	SEG(6*BOX_WIDE/10,  MID_HIGH,	6*BOX_WIDE/10,	CHR_HIGH),
	-1
    }, glyph_ff[] = {
	SEG(1*BOX_WIDE/10,  0,		6*BOX_WIDE/10,	0),		/* F */
	SEG(1*BOX_WIDE/10,5*MID_HIGH/12,6*CHR_WIDE/12,5*MID_HIGH/12),
	SEG(1*BOX_WIDE/10,  0,		0*BOX_WIDE/3, 5*MID_HIGH/6),
	SEG(1*BOX_WIDE/3,   MID_HIGH,	  CHR_WIDE,	MID_HIGH),	/* F */
	SEG(1*BOX_WIDE/3, 8*MID_HIGH/6,10*CHR_WIDE/12,8*MID_HIGH/6),
	SEG(1*BOX_WIDE/3,   MID_HIGH,	1*BOX_WIDE/3,	CHR_HIGH),
	-1
    }, glyph_lf[] = {
	SEG(1*BOX_WIDE/10,  0,		1*BOX_WIDE/10,9*MID_HIGH/12),	/* L */
	SEG(1*BOX_WIDE/10,9*MID_HIGH/12,6*BOX_WIDE/10,9*MID_HIGH/12),
	SEG(1*BOX_WIDE/3,   MID_HIGH,	  CHR_WIDE,	MID_HIGH),	/* F */
	SEG(1*BOX_WIDE/3, 8*MID_HIGH/6,10*CHR_WIDE/12,8*MID_HIGH/6),
	SEG(1*BOX_WIDE/3,   MID_HIGH,	1*BOX_WIDE/3,	CHR_HIGH),
	-1
    }, glyph_nl[] = {
	SEG(1*BOX_WIDE/10,5*MID_HIGH/6, 1*BOX_WIDE/10,	0),		/* N */
	SEG(1*BOX_WIDE/10,  0,		5*BOX_WIDE/6, 5*MID_HIGH/6),
	SEG(5*BOX_WIDE/6, 5*MID_HIGH/6, 5*BOX_WIDE/6,	0),
	SEG(1*BOX_WIDE/3,   MID_HIGH,	1*BOX_WIDE/3,	CHR_HIGH),	/* L */
	SEG(1*BOX_WIDE/3,   CHR_HIGH,	  CHR_WIDE,	CHR_HIGH),
	-1
    }, glyph_vt[] = {
	SEG(1*BOX_WIDE/10,   0,		5*BOX_WIDE/12,5*MID_HIGH/6),	/* V */
	SEG(5*BOX_WIDE/12,5*MID_HIGH/6, 5*BOX_WIDE/6,	0),
	SEG(2*BOX_WIDE/10,  MID_HIGH,	  CHR_WIDE,	MID_HIGH),	/* T */
	SEG(6*BOX_WIDE/10,  MID_HIGH,	6*BOX_WIDE/10,	CHR_HIGH),
	-1
    }, plus_or_minus[] =
    {
	SEG(  0,	  5*BOX_HIGH/6,	  CHR_WIDE,   5*BOX_HIGH/6),
	SEG(  MID_WIDE,	  2*BOX_HIGH/6,	  MID_WIDE,   4*BOX_HIGH/6),
	SEG(  0,	  3*BOX_HIGH/6,	  CHR_WIDE,   3*BOX_HIGH/6),
	-1
    }, lower_right_corner[] =
    {
	SEG(  0,	    MID_HIGH,	  MID_WIDE,	MID_HIGH),
	SEG(  MID_WIDE,	    MID_HIGH,	  MID_WIDE,	0),
	-1
    }, upper_right_corner[] =
    {
	SEG(  0,	    MID_HIGH,	  MID_WIDE,	MID_HIGH),
	SEG( MID_WIDE,	    MID_HIGH,	  MID_WIDE,	BOX_HIGH),
	-1
    }, upper_left_corner[] =
    {
	SEG(  MID_WIDE,	    MID_HIGH,	  BOX_WIDE,	MID_HIGH),
	SEG(  MID_WIDE,	    MID_HIGH,	  MID_WIDE,	BOX_HIGH),
	-1
    }, lower_left_corner[] =
    {
	SEG(  MID_WIDE,	    0,		  MID_WIDE,	MID_HIGH),
	SEG(  MID_WIDE,	    MID_WIDE,	  BOX_WIDE,	MID_HIGH),
	-1
    }, cross[] =
    {
	SEG(  0,	    MID_HIGH,	  BOX_WIDE,	MID_HIGH),
	SEG(  MID_WIDE,	    0,		  MID_WIDE,	BOX_HIGH),
	-1
    }, scan_line_1[] =
    {
	SEG(  0,	    0,		  BOX_WIDE,	0),
	-1
    }, scan_line_3[] =
    {
	SEG(  0,	    BOX_HIGH/4,	  BOX_WIDE,	BOX_HIGH/4),
	-1
    }, scan_line_7[] =
    {
	SEG( 0,		    MID_HIGH,	  BOX_WIDE,	MID_HIGH),
	-1
    }, scan_line_9[] =
    {
	SEG(  0,	  3*BOX_HIGH/4,	  BOX_WIDE,   3*BOX_HIGH/4),
	-1
    }, horizontal_line[] =
    {
	SEG(  0,	    BOX_HIGH,	  BOX_WIDE,	BOX_HIGH),
	-1
    }, left_tee[] =
    {
	SEG(  MID_WIDE,	    0,		  MID_WIDE,	BOX_HIGH),
	SEG(  MID_WIDE,	    MID_HIGH,	  BOX_WIDE,	MID_HIGH),
	-1
    }, right_tee[] =
    {
	SEG(  MID_WIDE,	    0,		  MID_WIDE,	BOX_HIGH),
	SEG(  MID_WIDE,	    MID_HIGH,	  0,		MID_HIGH),
	-1
    }, bottom_tee[] =
    {
	SEG(  0,	    MID_HIGH,	  BOX_WIDE,	MID_HIGH),
	SEG(  MID_WIDE,	    0,		  MID_WIDE,	MID_HIGH),
	-1
    }, top_tee[] =
    {
	SEG(  0,	    MID_HIGH,	  BOX_WIDE,	MID_HIGH),
	SEG(  MID_WIDE,	    MID_HIGH,	  MID_WIDE,	BOX_HIGH),
	-1
    }, vertical_line[] =
    {
	SEG(  MID_WIDE,	    0,		  MID_WIDE,	BOX_HIGH),
	-1
    }, less_than_or_equal[] =
    {
	SEG(  CHR_WIDE,	    BOX_HIGH/3,	  0,		MID_HIGH),
	SEG(  CHR_WIDE,	  2*BOX_HIGH/3,	  0,		MID_HIGH),
	SEG(  0,	  3*BOX_HIGH/4,	  CHR_WIDE,   3*BOX_HIGH/4),
	-1
    }, greater_than_or_equal[] =
    {
	SEG(  0,	    BOX_HIGH/3,	  CHR_WIDE,	MID_HIGH),
	SEG(  0,	  2*BOX_HIGH/3,	  CHR_WIDE,	MID_HIGH),
	SEG(  0,	  3*BOX_HIGH/4,	  CHR_WIDE,   3*BOX_HIGH/4),
	-1
    }, greek_pi[] =
    {
	SEG(  0,	    MID_HIGH,	  CHR_WIDE,	MID_HIGH),
	SEG(5*CHR_WIDE/6,   MID_HIGH,	5*CHR_WIDE/6,	CHR_HIGH),
	SEG(2*CHR_WIDE/6,   MID_HIGH,	2*CHR_WIDE/6,	CHR_HIGH),
	-1
    }, not_equal_to[] =
    {
	SEG(2*BOX_WIDE/3, 1*BOX_HIGH/3, 1*BOX_WIDE/3,	CHR_HIGH),
	SEG(  0,	  2*BOX_HIGH/3,	  CHR_WIDE,   2*BOX_HIGH/3),
	SEG(  0,	    MID_HIGH,	  CHR_WIDE,	MID_HIGH),
	-1
    }, sigma_1[] =
    {
	SEG(BOX_WIDE,	    MID_HIGH,	  BOX_WIDE/2,	MID_HIGH),
	SEG(BOX_WIDE/2,	    MID_HIGH,	  BOX_WIDE,	BOX_HIGH),
	-1
    }, sigma_2[] =
    {
	SEG(BOX_WIDE,	    MID_HIGH,	  BOX_WIDE/2,	MID_HIGH),
	SEG(BOX_WIDE/2,	    MID_HIGH,	  BOX_WIDE,	0),
	-1
    }, sigma_3[] =
    {
	SEG(  0,	    0,	  	  BOX_WIDE,	BOX_HIGH),
	-1
    }, sigma_4[] =
    {
	SEG(  0,	    BOX_HIGH,	  BOX_WIDE,	0),
	-1
    }, sigma_5[] =
    {
	SEG(  0,	    MID_HIGH,	  MID_WIDE,	MID_HIGH),
	SEG(MID_WIDE,	    MID_HIGH,	  MID_WIDE,	BOX_HIGH),
	-1
    }, sigma_6[] =
    {
	SEG(  0,	    MID_HIGH,	  MID_WIDE,	MID_HIGH),
	SEG(MID_WIDE,	    MID_HIGH,	  MID_WIDE,	0),
	-1
    }, sigma_7[] =
    {
	SEG(  0,	    0,		  MID_WIDE,	MID_HIGH),
	SEG(  0,	    BOX_HIGH,	  MID_WIDE,	MID_HIGH),
	-1
    };

    static const struct {
	const int mode;			/* 1=y, 2=x, 3=both */
	const short *const data;
    } lines[] =
    {
	{ 0, 0 },			/* 00 (unused) */
	{ 0, 0 },			/* 01 diamond */
	{ 0, 0 },			/* 02 box */
	{ 0, glyph_ht },		/* 03 HT */
	{ 0, glyph_ff },		/* 04 FF */
	{ 0, 0 },			/* 05 CR */
	{ 0, glyph_lf },		/* 06 LF */
	{ 0, 0 },			/* 07 degrees (small circle) */
	{ 3, plus_or_minus },		/* 08 */
	{ 0, glyph_nl },		/* 09 */
	{ 0, glyph_vt },		/* 0A */
	{ 3, lower_right_corner },	/* 0B */
	{ 3, upper_right_corner },	/* 0C */
	{ 3, upper_left_corner },	/* 0D */
	{ 3, lower_left_corner },	/* 0E */
	{ 3, cross },			/* 0F */
	{ 2, scan_line_1 },		/* 10 */
	{ 2, scan_line_3 },		/* 11 */
	{ 2, scan_line_7 },		/* 12 */
	{ 2, scan_line_9 },		/* 13 */
	{ 2, horizontal_line },		/* 14 */
	{ 3, left_tee },		/* 15 */
	{ 3, right_tee },		/* 16 */
	{ 3, bottom_tee },		/* 17 */
	{ 3, top_tee },			/* 18 */
	{ 1, vertical_line },		/* 19 */
	{ 0, less_than_or_equal },	/* 1A */
	{ 0, greater_than_or_equal },	/* 1B */
	{ 0, greek_pi },		/* 1C */
	{ 0, not_equal_to },		/* 1D */
	{ 0, 0 },			/* 1E LB */
	{ 0, 0 },			/* 1F bullet */
	{ 0, 0 },			/* 20 space */
	{ 3, sigma_1 },			/* PUA(0) */
	{ 3, sigma_2 },			/* PUA(1) */
	{ 3, sigma_3 },			/* PUA(2) */
	{ 3, sigma_4 },			/* PUA(3) */
	{ 3, sigma_5 },			/* PUA(4) */
	{ 3, sigma_6 },			/* PUA(5) */
	{ 3, sigma_7 },			/* PUA(6) */
    };
    /* *INDENT-ON* */

    GC gc2;
    CgsEnum cgsId = (ch == 2) ? gcDots : gcLine;
    VTwin *cgsWin = WhichVWin(screen);
    const short *p;
    unsigned font_width = (((params->draw_flags & DOUBLEWFONT) ? 2U : 1U)
			   * screen->fnt_wide);
    unsigned font_height = (((params->draw_flags & DOUBLEHFONT) ? 2U : 1U)
			    * screen->fnt_high);
    unsigned thick;

    if (cells > 1)
	font_width *= (unsigned) cells;

#if OPT_WIDE_CHARS
    /*
     * Try to show line-drawing characters if we happen to be in UTF-8
     * mode, but have gotten an old-style font.
     */
    if (screen->utf8_mode
#if OPT_RENDERFONT
	&& !UsingRenderFont(params->xw)
#endif
	&& (ch > 127)
	&& !is_UCS_SPECIAL(ch)) {
	int which = (params->attr_flags & BOLD) ? fBold : fNorm;
	unsigned n;
	for (n = 1; n < 32; n++) {
	    if (xtermMissingChar(n, XTermFontsRef(screen->fnts, which)))
		continue;
	    if (dec2ucs(screen, n) != ch)
		continue;
	    TRACE(("...use xterm-style linedrawing U+%04X ->%d\n", ch, n));
	    ch = n;
	    break;
	}
    }
#endif

#if OPT_VT52_MODE
    if (!(screen->vtXX_level)) {
	switch (ch) {
	case 6:
	    ch = 7;
	    break;
	default:
	    ch = 256;
	    break;
	}
    }
#endif

    /*
     * Line-drawing characters display using the full (scaled) cellsize, while
     * other characters should be shifted to center them vertically.
     */
    if (!xftords) {
	if ((ch < XtNumber(lines)) && (lines[ch].mode & 3) != 0) {
	    font_height = (unsigned) ((float) font_height * screen->scale_height);
	} else {
	    y += ScaleShift(screen);
	}
    }

    if (xtermIsDecTechnical(ch)) {
	ch -= XTERM_PUA;
	ch += 33;
    }

    TRACE(("DRAW_BOX(%02X) cell %dx%d at %d,%d%s\n",
	   ch, font_height, font_width, y, x,
	   ((ch >= XtNumber(lines))
	    ? "-BAD"
	    : "")));

    if (cgsId == gcDots) {
	setCgsFont(params->xw, cgsWin, cgsId, getCgsFont(params->xw, cgsWin, gc));
	setCgsFore(params->xw, cgsWin, cgsId, getCgsFore(params->xw, cgsWin, gc));
	setCgsBack(params->xw, cgsWin, cgsId, getCgsBack(params->xw, cgsWin, gc));
    } else {
	setCgsFont(params->xw, cgsWin, cgsId, getCgsFont(params->xw, cgsWin, gc));
	setCgsFore(params->xw, cgsWin, cgsId, getCgsBack(params->xw, cgsWin, gc));
	setCgsBack(params->xw, cgsWin, cgsId, getCgsBack(params->xw, cgsWin, gc));
    }
    gc2 = getCgsGC(params->xw, cgsWin, cgsId);

    if (!(params->draw_flags & NOBACKGROUND)) {
	XFillRectangle(screen->display, VDrawable(screen), gc2, x, y,
		       font_width,
		       font_height);
    }

    setCgsFont(params->xw, cgsWin, cgsId, getCgsFont(params->xw, cgsWin, gc));
    setCgsFore(params->xw, cgsWin, cgsId, getCgsFore(params->xw, cgsWin, gc));
    setCgsBack(params->xw, cgsWin, cgsId, getCgsBack(params->xw, cgsWin, gc));
    gc2 = getCgsGC(params->xw, cgsWin, cgsId);

    thick = ((params->attr_flags & BOLD)
	     ? (Max((unsigned) screen->fnt_high / 12, 1))
	     : (Max((unsigned) screen->fnt_high / 16, 1)));
    setXtermLineAttributes(screen->display, gc2,
			   thick,
			   ((ch < XtNumber(lines))
			    ? LineSolid
			    : LineOnOffDash));

    if (ch == 32) {		/* space! */
	;			/* boxing a missing space is pointless */
    } else if (ch == 1) {	/* diamond */
	XPoint points[5];
	int npoints = 5, n;

	points[0].x = MID_WIDE;
	points[0].y = BOX_HIGH / 4;

	points[1].x = 8 * BOX_WIDE / 8;
	points[1].y = MID_HIGH;

	points[2].x = points[0].x;
	points[2].y = 3 * BOX_HIGH / 4;

	points[3].x = 0 * BOX_WIDE / 8;
	points[3].y = points[1].y;

	points[4].x = points[0].x;
	points[4].y = points[0].y;

	for (n = 0; n < npoints; ++n) {
	    points[n].x = (short) (SCALED_X(points[n].x));
	    points[n].y = (short) (SCALED_Y(points[n].y));
	    points[n].x = (short) (points[n].x + x);
	    points[n].y = (short) (points[n].y + y);
	}

	XFillPolygon(screen->display,
		     VDrawable(screen), gc2,
		     points, npoints,
		     Convex, CoordModeOrigin);
    } else if (ch == 7) {	/* degrees */
	unsigned width = (BOX_WIDE / 3);
	int x_coord = MID_WIDE - (int) (width / 2);
	int y_coord = MID_HIGH - (int) width;

	SCALE_X(x_coord);
	SCALE_Y(y_coord);
	width = (unsigned) SCALED_X(width);

	XDrawArc(screen->display,
		 VDrawable(screen), gc2,
		 x + x_coord, y + y_coord, width, width,
		 0,
		 360 * 64);
    } else if (ch == 0x1f) {	/* bullet */
	unsigned width = 7 * BOX_WIDE / 10;
	int x_coord = MID_WIDE - (int) (width / 3);
	int y_coord = MID_HIGH - (int) (width / 3);

	SCALE_X(x_coord);
	SCALE_Y(y_coord);
	width = (unsigned) SCALED_X(width);

	XDrawArc(screen->display,
		 VDrawable(screen), gc2,
		 x + x_coord, y + y_coord, width, width,
		 0,
		 360 * 64);
    } else if (ch < XtNumber(lines)
	       && (p = lines[ch].data) != 0) {
	int coord[4];
	int n = 0;
	while (*p >= 0) {
	    coord[n++] = *p++;
	    if (n == 4) {
		SCALE_X(coord[0]);
		SCALE_Y(coord[1]);
		SCALE_X(coord[2]);
		SCALE_Y(coord[3]);
		XDrawLine(screen->display,
			  VDrawable(screen), gc2,
			  x + coord[0], y + coord[1],
			  x + coord[2], y + coord[3]);
		n = 0;
	    }
	}
    } else if (screen->force_all_chars) {
	/* bounding rectangle, for debugging */
	if ((params->draw_flags & DOUBLEHFONT)) {
	    XRectangle clip;

	    clip.x = 0;
	    clip.y = 0;
	    clip.width = (unsigned short) ((font_width - 1) + (unsigned) thick);
	    clip.height = (unsigned short) ((unsigned) FontHeight(screen) + thick);

	    if ((params->draw_flags & DOUBLEFIRST)) {
		y -= (2 * FontDescent(screen));
		clip.height =
		    (unsigned short) (clip.height
				      - ((unsigned short) FontDescent(screen)));
	    } else {
		y -= FontHeight(screen);
		y += FontDescent(screen);
		clip.y = (short) FontHeight(screen);
	    }
	    XSetClipRectangles(screen->display, gc2, x, y, &clip, 1, Unsorted);
	}
	XDrawRectangle(screen->display, VDrawable(screen), gc2,
		       x + (int) thick, y + (int) thick,
		       font_width - (2 * thick),
		       font_height - (2 * thick));
	if ((params->draw_flags & DOUBLEHFONT)) {
	    XSetClipMask(screen->display, gc2, None);
	}
    }
    resetXtermLineAttributes(screen->display, gc2);
}
#endif /* OPT_BOX_CHARS || OPT_WIDE_CHARS */

#if OPT_RENDERFONT
static int
checkXftGlyph(XtermWidget xw, XftFont *font, unsigned wc)
{
    TScreen *screen = TScreenOf(xw);
    int result = 0;
    int expect;

    if ((expect = CharWidth(screen, wc)) > 0) {
	XGlyphInfo gi;
	int actual;
	int limit = (100 + xw->misc.limit_fontwidth);

	XftTextExtents32(screen->display, font, &wc, 1, &gi);
	/*
	 * Some (more than a few) fonts are sloppy; allow 10% outside
	 * the bounding box to accommodate them.
	 */
	actual = ((gi.xOff * 100) >= (limit * FontWidth(screen))) ? 2 : 1;
	if (actual <= expect) {
	    /* allow double-cell if wcwidth agrees */
	    result = 1;
	} else {
	    /*
	     * Do not use this font for this specific character, but
	     * possibly other characters can be used.
	     */
	    result = -1;
	    TRACE(("SKIP U+%04X %d vs %d (%d vs %d) %s\n",
		   wc, gi.xOff, FontWidth(screen), actual, expect,
		   nameOfXftFont(font)));
	}
    } else {
	result = 1;
    }
    return result;
}

/*
 * Check if the glyph is defined in the given font, and (try to) filter out
 * cases where double-width glyphs are stuffed into a single-width outline.
 */
static int
foundXftGlyph(XtermWidget xw, XTermXftFonts *data, int fontNum, unsigned wc)
{
    XftFont *font = XftFpN(data, fontNum);
    int result = 0;

    if (font != 0) {
	if (!xtermXftMissing(xw, data, fontNum, font, wc)) {

	    if (XftIsN(data, fontNum) == xcBogus) {
		;
	    } else if (XftIsN(data, fontNum) == xcOpened) {
		result = 1;
	    } else {
		result = checkXftGlyph(xw, font, wc);
	    }
	}
    }
    return result;
}

static void
markXftOpened(XtermWidget xw, XTermXftFonts *which, int n, unsigned wc)
{
    if (XftIsN(which, n) != xcOpened) {
	which->opened++;
	XftIsN(which, n) = xcOpened;
	/* XFT_DEBUG=3 will show useful context for this */
	if (getenv("XFT_DEBUG") != 0) {
	    printf("%s: matched U+%04X in fontset #%d [%u:%u]\n",
		   ProgramName,
		   wc, n + 1,
		   which->opened,
		   xw->work.max_fontsets);
	}
    }
}

static char **
xftData2List(XtermWidget xw, const XTermXftFonts *fontData)
{
    TScreen *screen = TScreenOf(xw);
    VTFontList *lists = &xw->work.fonts.xft;
    char **result = NULL;
    int n = screen->menu_font_number;

    if (fontData == &screen->renderFontNorm[n])
	result = lists->list_n;
    else if (fontData == &screen->renderFontBold[n])
	result = lists->list_b;
    else if (fontData == &screen->renderFontItal[n])
	result = lists->list_i;
    else if (fontData == &screen->renderFontBtal[n])
	result = lists->list_bi;
#if OPT_RENDERWIDE
    if (fontData == &screen->renderWideNorm[n])
	result = lists->list_w;
    else if (fontData == &screen->renderWideBold[n])
	result = lists->list_wb;
    else if (fontData == &screen->renderWideItal[n])
	result = lists->list_wi;
    else if (fontData == &screen->renderWideBtal[n])
	result = lists->list_wbi;
#endif
    return result;
}

static FcPattern *
mergeXftStyle(XtermWidget xw, FcPattern * myPattern, XTermXftFonts *fontData)
{
    TScreen *screen = TScreenOf(xw);
    Display *dpy = screen->display;
    XftFont *given = XftFp(fontData);
    XftResult mStatus;
    int iValue;
    double dValue;

    if (FcOK(FcPatternGetInteger(fontData->pattern, XFT_WEIGHT, 0, &iValue))) {
	FcPatternAddInteger(myPattern, XFT_WEIGHT, iValue);
    }
    if (FcOK(FcPatternGetInteger(fontData->pattern, XFT_SLANT, 0, &iValue))) {
	FcPatternAddInteger(myPattern, XFT_SLANT, iValue);
    }
    if (FcOK(FcPatternGetDouble(fontData->pattern, FC_ASPECT, 0, &dValue))) {
	FcPatternAddDouble(myPattern, FC_ASPECT, dValue);
    }
    if (FcOK(FcPatternGetDouble(fontData->pattern, XFT_SIZE, 0, &dValue))) {
	FcPatternAddDouble(myPattern, XFT_SIZE, dValue);
    }
    FcPatternAddBool(myPattern, FC_SCALABLE, FcTrue);
    FcPatternAddInteger(myPattern, XFT_SPACING, XFT_MONO);
    FcPatternAddInteger(myPattern, FC_CHAR_WIDTH, given->max_advance_width);
#ifdef FC_COLOR
#if !USE_FC_COLOR
    FcPatternAddBool(myPattern, FC_COLOR, FcFalse);
#endif
    FcPatternAddBool(myPattern, FC_OUTLINE, FcTrue);
#endif

    FcConfigSubstitute(NULL, myPattern, FcMatchPattern);
    XftDefaultSubstitute(dpy, DefaultScreen(dpy), myPattern);

    return FcFontMatch(NULL, myPattern, &mStatus);
}

/*
 * Check if the given character has a glyph known to Xft.  If it is missing,
 * try first to replace the font with a fallback that provides the glyph.
 *
 * Return -1 if nothing is found.  Otherwise, return the index in the cache.
 */
int
findXftGlyph(XtermWidget xw, XTermXftFonts *fontData, unsigned wc)
{
    TScreen *screen = TScreenOf(xw);
    XftFont *given;
    XftFont *actual = NULL;
    FcResult status;
    int n;
    int result = -1;

    /* sanity-check */
    if (fontData == NULL)
	return result;
    given = XftFp(fontData);

    /* if fontsets are not wanted, just leave */
    if (xw->work.max_fontsets == 0) {
	return result;
    }

    /* ignore codes in private use areas */
    if ((wc >= 0xe000 && wc <= 0xf8ff)
	|| (wc >= 0xf0000 && wc <= 0xffffd)
	|| (wc >= 0x100000 && wc <= 0x10fffd)) {
	return result;
    }
    /* the end of the BMP is reserved for non-characters */
    if (wc >= 0xfff0 && wc <= 0xffff) {
	return result;
    }

    /* initialize on the first call */
    if (fontData->fontset == NULL && fontData->pattern != NULL) {
	FcFontSet *sortedFonts;
	FcPattern *myPattern;
	int j;
	char **my_list;

	myPattern = FcPatternDuplicate(fontData->pattern);

	FcPatternAddBool(myPattern, FC_SCALABLE, FcTrue);
	FcPatternAddInteger(myPattern, FC_CHAR_WIDTH, given->max_advance_width);

	FcConfigSubstitute(FcConfigGetCurrent(),
			   myPattern,
			   FcMatchPattern);
	FcDefaultSubstitute(myPattern);

	sortedFonts = FcFontSort(NULL, myPattern, FcTrue, NULL, &status);

	fontData->fontset = FcFontSetCreate();

	if (fontData->fontset == 0 || !sortedFonts || sortedFonts->nfont <= 0) {
	    xtermWarning("did not find any usable TrueType font\n");
	    return 0;
	}

	/*
	 * Check if there are additional fonts in the XtermFontNames.xft for
	 * this font-data.
	 */
	if ((my_list = xftData2List(xw, fontData)) != NULL
	    && *++my_list != NULL) {
	    for (j = 0; my_list[j] != NULL; ++j) {
		FcPattern *extraPattern;
		if ((extraPattern = XftNameParse(my_list[j])) != 0) {
		    FcPattern *match;

		    match = mergeXftStyle(xw, extraPattern, fontData);

		    if (match != NULL) {
			FcFontSetAdd(fontData->fontset, match);
		    }
		    FcPatternDestroy(extraPattern);
		}
	    }
	}

	for (j = 0; j < sortedFonts->nfont; j++) {
	    FcPattern *font_pattern;

	    font_pattern = FcFontRenderPrepare(FcConfigGetCurrent(),
					       myPattern,
					       sortedFonts->fonts[j]);
	    if (font_pattern) {
		FcFontSetAdd(fontData->fontset, font_pattern);
	    }
	}

	FcFontSetSortDestroy(sortedFonts);
	FcPatternDestroy(myPattern);

	fontData->fs_size = Min(MaxXftCache, fontData->fontset->nfont);
    }
    if (fontData->fontset != NULL && fontData->fs_size > 0) {
	XftFont *check;
	int empty = fontData->fs_size;

	for (n = 1; n <= fontData->fs_size; ++n) {
	    XTermXftState usage = XftIsN(fontData, n);
	    if (usage == xcEmpty) {
		if (empty > n)
		    empty = n;
	    } else if (usage == xcOpened
		       || (usage == xcUnused
			   && (fontData->opened < xw->work.max_fontsets))) {
		check = XftFpN(fontData, n);
		if (foundXftGlyph(xw, fontData, (int) n, wc)) {
		    markXftOpened(xw, fontData, n, wc);
		    actual = check;
		    result = (int) n;
		    TRACE_FALLBACK(xw, "old", wc, result, actual);
		    break;
		}
	    }
	}

	if ((actual == NULL)
	    && (empty <= fontData->fs_size)
	    && (fontData->opened < xw->work.max_fontsets)) {
	    FcPattern *myPattern = NULL;
	    FcPattern *myReport = NULL;
	    int defer = -1;

	    if (empty == 0)	/* should not happen */
		empty++;

	    for (n = empty; n <= fontData->fs_size; ++n) {
		int found;
		int nn = n - 1;

		if (XftIsN(fontData, n) != xcEmpty) {
		    continue;
		}
		if (resource.reportFonts) {
		    if (myReport != NULL)
			FcPatternDestroy(myReport);
		    myReport = FcPatternDuplicate(fontData->fontset->fonts[nn]);
		}
		myPattern = FcPatternDuplicate(fontData->fontset->fonts[nn]);
		check = XftFontOpenPattern(screen->display, myPattern);
		(void) maybeXftCache(xw, check);
		XftFpN(fontData, n) = check;
		if (check == NULL) {
		    ;		/* shouldn't happen... */
		} else
#ifdef FC_COLOR
		if (isBogusXft(check)) {
		    XftIsN(fontData, n) = xcBogus;
		} else
#endif
		    if ((found = foundXftGlyph(xw, fontData, (int) n, wc))
			!= 0) {
		    markXftOpened(xw, fontData, n, wc);
		    reportXftFallbackFont(xw, fontData, (int) n, check, myReport);
		    if (found < 0) {
			if (defer < 0) {
			    defer = (int) n;
			    TRACE(("Deferring font choice #%d\n", n + 1));
			    continue;
			} else if (slowXftMissing(xw, check, wc)) {
			    TRACE(("Deferred, continuing  #%d\n", n + 1));
			    continue;
			}
		    } else if (defer >= 0) {
			defer = -1;
			TRACE(("Deferred, replacing %d with %d\n",
			       defer + 1, n + 1));
		    }
		    actual = check;
		    result = (int) n;
		    TRACE_FALLBACK(xw, "new", wc, result, actual);
		    break;
		} else {
		    if (defer >= 0
			&& !slowXftMissing(xw, check, wc)
			&& checkXftGlyph(xw, check, wc)) {
			XTermFontMap *font_map = &(fontData->font_map);
			TRACE(("checkrecover2 %d\n", n));
			markXftOpened(xw, fontData, n, wc);
			reportXftFallbackFont(xw, fontData, (int) n, check, myReport);
			actual = check;
			result = (int) n;
			TRACE_FALLBACK(xw, "fix", wc, result, actual);
			font_map->per_font[wc] = (XTfontNum) (result + 1);
			break;
		    } else {
			/*
			 * The slot is opened, but we are not using it yet.
			 */
			XftIsN(fontData, n) = xcUnused;
		    }
		}
	    }
	    if (myReport != NULL)
		FcPatternDestroy(myReport);
	}
    }
    return result;
}

/*
 * Check if the given character has a glyph known to Xft.  If it is missing,
 * return true.
 *
 * see xc/lib/Xft/xftglyphs.c
 */
Bool
xtermXftMissing(XtermWidget xw,
		XTermXftFonts *data,
		int fontNum,	/* 0=primary, 1+ is fallback */
		XftFont *font,	/* actual font if no data */
		unsigned wc)
{
    Bool result = True;

    (void) xw;
    if (data != NULL && font != NULL) {
	XTermFontMap *font_map = &(data->font_map);
	/*
	 * Each fallback font has one chance to be scanned/cached.
	 * We record in per_font[] the index of the first font containing a
	 * given glyph.
	 */
	if (font_map->depth <= fontNum) {
	    FcChar32 last = (xtermXftLastChar(font) | 255) + 1;
	    FcChar32 base;
	    FcChar32 nextPage;
	    FcChar32 map[FC_CHARSET_MAP_SIZE];
	    unsigned added = 0;
	    unsigned actual = 0;

	    font_map->depth = (fontNum + 1);
	    /* allocate space */
	    if (last > font_map->last_char) {
		size_t need = (last * sizeof(XTfontNum));
		size_t c1st = (font_map->last_char * sizeof(XTfontNum));
		font_map->per_font = realloc(font_map->per_font, need);
		memset(font_map->per_font + font_map->last_char, 0, (need - c1st));
		font_map->last_char = last;
	    }

	    /* scan new font */
	    base = FcCharSetFirstPage(font->charset, map, &nextPage);
	    do {
		unsigned row;
		unsigned col;
		FcChar32 bits;
		for (row = 0; row < FC_CHARSET_MAP_SIZE; ++row) {
		    bits = map[row];
		    for (col = 0; col < 32; ++col) {
			if ((bits & 1) != 0) {
			    actual++;
			    if (!font_map->per_font[base]) {
				font_map->per_font[base] = (Char) font_map->depth;
				++added;
			    }
			}
			bits >>= 1;
			++base;
		    }
		}
	    } while ((base = FcCharSetNextPage(font->charset, map,
					       &nextPage)) != FC_CHARSET_DONE);
	    (void) added;
	    (void) actual;
	    TRACE(("xtermXftMissing U+%04X #%-3d %6u added vs %6u of %6ld %s: %s\n",
		   wc,
		   font_map->depth,
		   added, actual,
		   font_map->last_char + 1,
		   whichXftFonts(xw, data),
		   nameOfXftFont(font)));
	}
	if (wc < font_map->last_char) {
	    result = (font_map->per_font[wc] != (fontNum + 1));
	}
    }
    return result;
}
#endif /* OPT_RENDERFONT */

#if OPT_WIDE_CHARS
#define MY_UCS(ucs,dec) case ucs: result = dec; break
unsigned
ucs2dec(TScreen *screen, unsigned ch)
{
    unsigned result = ch;

    (void) screen;
    if ((ch > 127)
	&& !is_UCS_SPECIAL(ch)) {
#if OPT_VT52_MODE
	if (screen != 0 && !(screen->vtXX_level)) {
	    /*
	     * Intentionally empty: it would be possible to use the built-in
	     * line-drawing fallback in xtermDrawBoxChar(), but for testing
	     * ncurses, this is good enough.
	     */
	    ;
	} else
#endif
	    switch (ch) {
		MY_UCS(0x25ae, 0);	/* black vertical rectangle           */
		MY_UCS(0x25c6, 1);	/* black diamond                      */
		MY_UCS(0x2592, 2);	/* medium shade                       */
		MY_UCS(0x2409, 3);	/* symbol for horizontal tabulation   */
		MY_UCS(0x240c, 4);	/* symbol for form feed               */
		MY_UCS(0x240d, 5);	/* symbol for carriage return         */
		MY_UCS(0x240a, 6);	/* symbol for line feed               */
		MY_UCS(0x00b0, 7);	/* degree sign                        */
		MY_UCS(0x00b1, 8);	/* plus-minus sign                    */
		MY_UCS(0x2424, 9);	/* symbol for newline                 */
		MY_UCS(0x240b, 10);	/* symbol for vertical tabulation     */
		MY_UCS(0x2518, 11);	/* box drawings light up and left     */
		MY_UCS(0x2510, 12);	/* box drawings light down and left   */
		MY_UCS(0x250c, 13);	/* box drawings light down and right  */
		MY_UCS(0x2514, 14);	/* box drawings light up and right    */
		MY_UCS(0x253c, 15);	/* box drawings light vertical and horizontal */
		MY_UCS(0x23ba, 16);	/* box drawings scan 1                */
		MY_UCS(0x23bb, 17);	/* box drawings scan 3                */
		MY_UCS(0x2500, 18);	/* box drawings light horizontal      */
		MY_UCS(0x23bc, 19);	/* box drawings scan 7                */
		MY_UCS(0x23bd, 20);	/* box drawings scan 9                */
		MY_UCS(0x251c, 21);	/* box drawings light vertical and right      */
		MY_UCS(0x2524, 22);	/* box drawings light vertical and left       */
		MY_UCS(0x2534, 23);	/* box drawings light up and horizontal       */
		MY_UCS(0x252c, 24);	/* box drawings light down and horizontal     */
		MY_UCS(0x2502, 25);	/* box drawings light vertical        */
		MY_UCS(0x2264, 26);	/* less-than or equal to              */
		MY_UCS(0x2265, 27);	/* greater-than or equal to           */
		MY_UCS(0x03c0, 28);	/* greek small letter pi              */
		MY_UCS(0x2260, 29);	/* not equal to                       */
		MY_UCS(0x00a3, 30);	/* pound sign                         */
		MY_UCS(0x00b7, 31);	/* middle dot                         */
	    }
    }
    return result;
}

#undef  MY_UCS
#define MY_UCS(ucs,dec) case dec: result = ucs; break

unsigned
dec2ucs(TScreen *screen, unsigned ch)
{
    unsigned result = ch;

    (void) screen;
    if (xtermIsDecGraphic(ch)) {
#if OPT_VT52_MODE
	if (screen != 0 && !(screen->vtXX_level)) {
	    switch (ch) {
		MY_UCS(0x0020, 0);	/* nbsp, treat as blank           */
		MY_UCS(0x0020, 1);	/* reserved, treat as blank       */
		MY_UCS(0x25ae, 2);	/* black vertical rectangle       */
		MY_UCS(0x215f, 3);	/* "1/"                           */
		MY_UCS(0x0020, 4);	/* "3/", not in Unicode, ignore   */
		MY_UCS(0x0020, 5);	/* "5/", not in Unicode, ignore   */
		MY_UCS(0x0020, 6);	/* "7/", not in Unicode, ignore   */
		MY_UCS(0x00b0, 7);	/* degree sign                    */
		MY_UCS(0x00b1, 8);	/* plus-minus sign                */
		MY_UCS(0x2192, 9);	/* right-arrow                    */
		MY_UCS(0x2026, 10);	/* ellipsis                       */
		MY_UCS(0x00f7, 11);	/* divide by                      */
		MY_UCS(0x2193, 12);	/* down arrow                     */
		MY_UCS(0x23ba, 13);	/* bar at scan 0                  */
		MY_UCS(0x23ba, 14);	/* bar at scan 1                  */
		MY_UCS(0x23bb, 15);	/* bar at scan 2                  */
		MY_UCS(0x23bb, 16);	/* bar at scan 3                  */
		MY_UCS(0x23bc, 17);	/* bar at scan 4                  */
		MY_UCS(0x23bc, 18);	/* bar at scan 5                  */
		MY_UCS(0x23bd, 19);	/* bar at scan 6                  */
		MY_UCS(0x23bd, 20);	/* bar at scan 7                  */
		MY_UCS(0x2080, 21);	/* subscript 0                    */
		MY_UCS(0x2081, 22);	/* subscript 1                    */
		MY_UCS(0x2082, 23);	/* subscript 2                    */
		MY_UCS(0x2083, 24);	/* subscript 3                    */
		MY_UCS(0x2084, 25);	/* subscript 4                    */
		MY_UCS(0x2085, 26);	/* subscript 5                    */
		MY_UCS(0x2086, 27);	/* subscript 6                    */
		MY_UCS(0x2087, 28);	/* subscript 7                    */
		MY_UCS(0x2088, 29);	/* subscript 8                    */
		MY_UCS(0x2089, 30);	/* subscript 9                    */
		MY_UCS(0x00b6, 31);	/* paragraph                      */
	    }
	} else
#endif
	    switch (ch) {
		MY_UCS(0x25ae, 0);	/* black vertical rectangle           */
		MY_UCS(0x25c6, 1);	/* black diamond                      */
		MY_UCS(0x2592, 2);	/* medium shade                       */
		MY_UCS(0x2409, 3);	/* symbol for horizontal tabulation   */
		MY_UCS(0x240c, 4);	/* symbol for form feed               */
		MY_UCS(0x240d, 5);	/* symbol for carriage return         */
		MY_UCS(0x240a, 6);	/* symbol for line feed               */
		MY_UCS(0x00b0, 7);	/* degree sign                        */
		MY_UCS(0x00b1, 8);	/* plus-minus sign                    */
		MY_UCS(0x2424, 9);	/* symbol for newline                 */
		MY_UCS(0x240b, 10);	/* symbol for vertical tabulation     */
		MY_UCS(0x2518, 11);	/* box drawings light up and left     */
		MY_UCS(0x2510, 12);	/* box drawings light down and left   */
		MY_UCS(0x250c, 13);	/* box drawings light down and right  */
		MY_UCS(0x2514, 14);	/* box drawings light up and right    */
		MY_UCS(0x253c, 15);	/* box drawings light vertical and horizontal */
		MY_UCS(0x23ba, 16);	/* box drawings scan 1                */
		MY_UCS(0x23bb, 17);	/* box drawings scan 3                */
		MY_UCS(0x2500, 18);	/* box drawings light horizontal      */
		MY_UCS(0x23bc, 19);	/* box drawings scan 7                */
		MY_UCS(0x23bd, 20);	/* box drawings scan 9                */
		MY_UCS(0x251c, 21);	/* box drawings light vertical and right      */
		MY_UCS(0x2524, 22);	/* box drawings light vertical and left       */
		MY_UCS(0x2534, 23);	/* box drawings light up and horizontal       */
		MY_UCS(0x252c, 24);	/* box drawings light down and horizontal     */
		MY_UCS(0x2502, 25);	/* box drawings light vertical        */
		MY_UCS(0x2264, 26);	/* less-than or equal to              */
		MY_UCS(0x2265, 27);	/* greater-than or equal to           */
		MY_UCS(0x03c0, 28);	/* greek small letter pi              */
		MY_UCS(0x2260, 29);	/* not equal to                       */
		MY_UCS(0x00a3, 30);	/* pound sign                         */
		MY_UCS(0x00b7, 31);	/* middle dot                         */
	    }
    }
    return result;
}

#endif /* OPT_WIDE_CHARS */

#if OPT_RENDERFONT || OPT_SHIFT_FONTS
static int
lookupOneFontSize(XtermWidget xw, int fontnum)
{
    TScreen *screen = TScreenOf(xw);

    if (screen->menu_font_sizes[fontnum] == 0) {
	XTermFonts fnt;

	memset(&fnt, 0, sizeof(fnt));
	screen->menu_font_sizes[fontnum] = -1;
	if (xtermOpenFont(xw, screen->MenuFontName(fontnum), &fnt, NULL, True)) {
	    if (fontnum <= fontMenu_lastBuiltin
		|| strcmp(fnt.fn, DEFFONT)) {
		screen->menu_font_sizes[fontnum] = FontSize(fnt.fs);
		if (screen->menu_font_sizes[fontnum] <= 0)
		    screen->menu_font_sizes[fontnum] = -1;
	    }
	    xtermCloseFont(xw, &fnt);
	}
    }
    return (screen->menu_font_sizes[fontnum] > 0);
}

/*
 * Cache the font-sizes so subsequent larger/smaller font actions will go fast.
 */
static void
lookupFontSizes(XtermWidget xw)
{
    int n;

    for (n = 0; n < NMENUFONTS; n++) {
	(void) lookupOneFontSize(xw, n);
    }
}
#endif /* OPT_RENDERFONT || OPT_SHIFT_FONTS */

#if OPT_RENDERFONT
static double
defaultFaceSize(void)
{
    double result;
    float value;

    if (sscanf(DEFFACESIZE, "%f", &value) == 1)
	result = (double) value;
    else
	result = 14.0;
    return result;
}

static void
fillInFaceSize(XtermWidget xw, int fontnum)
{
    TScreen *screen = TScreenOf(xw);
    double face_size = (double) xw->misc.face_size[fontnum];

    if (face_size <= 0.0) {
#if OPT_SHIFT_FONTS
	/*
	 * If the user is switching font-sizes, make it follow by
	 * default the same ratios to the default as the fixed fonts
	 * would, for easy comparison.  There will be some differences
	 * since the fixed fonts have a variety of height/width ratios,
	 * but this is simpler than adding another resource value - and
	 * as noted above, the data for the fixed fonts are available.
	 */
	(void) lookupOneFontSize(xw, 0);
	if (fontnum == fontMenu_default) {
	    face_size = defaultFaceSize();
	} else if (lookupOneFontSize(xw, fontnum)
		   && (screen->menu_font_sizes[0]
		       != screen->menu_font_sizes[fontnum])) {
	    double ratio;
	    long num = screen->menu_font_sizes[fontnum];
	    long den = screen->menu_font_sizes[0];

	    if (den <= 0)
		den = 1;
	    ratio = dimSquareRoot((double) num / (double) den);

	    face_size = (ratio * (double) xw->misc.face_size[0]);
	    TRACE(("scaled[%d] using %3ld/%ld = %.2f -> %f\n",
		   fontnum, num, den, ratio, face_size));
	} else
#endif
	{
#define LikeBitmap(s) (((s) / 78.0) * (double) xw->misc.face_size[fontMenu_default])
	    switch (fontnum) {
	    case fontMenu_font1:
		face_size = LikeBitmap(2.0);
		break;
	    case fontMenu_font2:
		face_size = LikeBitmap(35.0);
		break;
	    case fontMenu_font3:
		face_size = LikeBitmap(60.0);
		break;
	    default:
		face_size = defaultFaceSize();
		break;
	    case fontMenu_font4:
		face_size = LikeBitmap(90.0);
		break;
	    case fontMenu_font5:
		face_size = LikeBitmap(135.0);
		break;
	    case fontMenu_font6:
		face_size = LikeBitmap(200.0);
		break;
	    case fontMenu_font7:
		face_size = LikeBitmap(240.0);
		break;
	    }
	    TRACE(("builtin[%d] -> %f\n", fontnum, face_size));
	}
	xw->misc.face_size[fontnum] = (float) face_size;
    }
}

/* no selection or escape */
#define NMENU_RENDERFONTS (fontMenu_lastBuiltin + 1)

/*
 * Workaround for breakage in font-packages - check if all of the bitmap font
 * sizes are the same, and if we're using TrueType fonts.
 */
static Boolean
useFaceSizes(XtermWidget xw)
{
    Boolean result = False;

    TRACE(("useFaceSizes " TRACE_L "\n"));
    if (UsingRenderFont(xw)) {
	Boolean nonzero = True;
	int n;

	for (n = 0; n < NMENU_RENDERFONTS; ++n) {
	    if (xw->misc.face_size[n] <= 0.0f) {
		nonzero = False;
		break;
	    }
	}
	if (!nonzero) {
	    Boolean broken_fonts = True;
	    TScreen *screen = TScreenOf(xw);
	    long first;

	    lookupFontSizes(xw);
	    first = screen->menu_font_sizes[0];
	    for (n = 0; n < NMENUFONTS; n++) {
		if (screen->menu_font_sizes[n] > 0
		    && screen->menu_font_sizes[n] != first) {
		    broken_fonts = False;
		    break;
		}
	    }

	    if (broken_fonts) {

		TRACE(("bitmap fonts are broken - set faceSize resources\n"));
		for (n = 0; n < NMENUFONTS; n++) {
		    fillInFaceSize(xw, n);
		}

	    }
	}
	result = True;
    }
    TRACE((TRACE_R " useFaceSizes %d\n", result));
    return result;
}
#endif /* OPT_RENDERFONT */

#if OPT_SHIFT_FONTS
/*
 * Find the index of a larger/smaller font (according to the sign of 'relative'
 * and its magnitude), starting from the 'old' index.
 */
int
lookupRelativeFontSize(XtermWidget xw, int old, int relative)
{
    TScreen *screen = TScreenOf(xw);
    int m = -1;

    TRACE(("lookupRelativeFontSize(old=%d, relative=%d)\n", old, relative));
    if (!IsIcon(screen)) {
#if OPT_RENDERFONT
	if (useFaceSizes(xw)) {
	    TRACE(("...using FaceSize\n"));
	    if (relative != 0) {
		int n;
		for (n = 0; n < NMENU_RENDERFONTS; ++n) {
		    fillInFaceSize(xw, n);
		    if (xw->misc.face_size[n] > 0 &&
			xw->misc.face_size[n] != xw->misc.face_size[old]) {
			int cmp_0 = ((xw->misc.face_size[n] >
				      xw->misc.face_size[old])
				     ? relative
				     : -relative);
			int cmp_m = ((m < 0)
				     ? 1
				     : ((xw->misc.face_size[n] <
					 xw->misc.face_size[m])
					? relative
					: -relative));
			if (cmp_0 > 0 && cmp_m > 0) {
			    m = n;
			}
		    }
		}
	    }
	} else
#endif
	{
	    TRACE(("...using bitmap areas\n"));
	    lookupFontSizes(xw);
	    if (relative != 0) {
		int n;
		for (n = 0; n < NMENUFONTS; ++n) {
		    if (screen->menu_font_sizes[n] > 0 &&
			screen->menu_font_sizes[n] !=
			screen->menu_font_sizes[old]) {
			int cmp_0 = ((screen->menu_font_sizes[n] >
				      screen->menu_font_sizes[old])
				     ? relative
				     : -relative);
			int cmp_m = ((m < 0)
				     ? 1
				     : ((screen->menu_font_sizes[n] <
					 screen->menu_font_sizes[m])
					? relative
					: -relative));
			if (cmp_0 > 0 && cmp_m > 0) {
			    m = n;
			}
		    }
		}
	    }
	}
	TRACE(("...new index %d\n", m));
	if (m >= 0) {
	    if (relative > 1)
		m = lookupRelativeFontSize(xw, m, relative - 1);
	    else if (relative < -1)
		m = lookupRelativeFontSize(xw, m, relative + 1);
	}
    }
    return m;
}

/* ARGSUSED */
void
HandleLargerFont(Widget w,
		 XEvent *event GCC_UNUSED,
		 String *params GCC_UNUSED,
		 Cardinal *param_count GCC_UNUSED)
{
    XtermWidget xw;

    TRACE(("Handle larger-vt-font for %p\n", (void *) w));
    if ((xw = getXtermWidget(w)) != 0) {
	if (xw->misc.shift_fonts) {
	    TScreen *screen = TScreenOf(xw);
	    int m;

	    m = lookupRelativeFontSize(xw, screen->menu_font_number, 1);
	    if (m >= 0) {
		SetVTFont(xw, m, True, NULL);
	    } else {
		Bell(xw, XkbBI_MinorError, 0);
	    }
	}
    }
}

/* ARGSUSED */
void
HandleSmallerFont(Widget w,
		  XEvent *event GCC_UNUSED,
		  String *params GCC_UNUSED,
		  Cardinal *param_count GCC_UNUSED)
{
    XtermWidget xw;

    TRACE(("Handle smaller-vt-font for %p\n", (void *) w));
    if ((xw = getXtermWidget(w)) != 0) {
	if (xw->misc.shift_fonts) {
	    TScreen *screen = TScreenOf(xw);
	    int m;

	    m = lookupRelativeFontSize(xw, screen->menu_font_number, -1);
	    if (m >= 0) {
		SetVTFont(xw, m, True, NULL);
	    } else {
		Bell(xw, XkbBI_MinorError, 0);
	    }
	}
    }
}
#endif /* OPT_SHIFT_FONTS */

int
xtermGetFont(const char *param)
{
    int fontnum;

    if (param == NULL)
	param = "";

    switch (param[0]) {
    case 'd':
    case 'D':
    case '0':
	fontnum = fontMenu_default;
	break;
    case '1':
	fontnum = fontMenu_font1;
	break;
    case '2':
	fontnum = fontMenu_font2;
	break;
    case '3':
	fontnum = fontMenu_font3;
	break;
    case '4':
	fontnum = fontMenu_font4;
	break;
    case '5':
	fontnum = fontMenu_font5;
	break;
    case '6':
	fontnum = fontMenu_font6;
	break;
    case '7':
	fontnum = fontMenu_font7;
	break;
    case 'e':
    case 'E':
	fontnum = fontMenu_fontescape;
	break;
    case 's':
    case 'S':
	fontnum = fontMenu_fontsel;
	break;
    default:
	fontnum = -1;
	break;
    }
    TRACE(("xtermGetFont(%s) ->%d\n", param, fontnum));
    return fontnum;
}

/* ARGSUSED */
void
HandleSetFont(Widget w,
	      XEvent *event GCC_UNUSED,
	      String *params,
	      Cardinal *param_count)
{
    XtermWidget xw;

    if ((xw = getXtermWidget(w)) != 0) {
	int fontnum;
	VTFontNames fonts;

	memset(&fonts, 0, sizeof(fonts));

	if (*param_count == 0) {
	    fontnum = fontMenu_default;
	} else {
	    Cardinal maxparams = 1;	/* total number of params allowed */
	    int result = xtermGetFont(params[0]);

	    switch (result) {
	    case fontMenu_default:	/* FALLTHRU */
	    case fontMenu_font1:	/* FALLTHRU */
	    case fontMenu_font2:	/* FALLTHRU */
	    case fontMenu_font3:	/* FALLTHRU */
	    case fontMenu_font4:	/* FALLTHRU */
	    case fontMenu_font5:	/* FALLTHRU */
	    case fontMenu_font6:	/* FALLTHRU */
	    case fontMenu_font7:	/* FALLTHRU */
		break;
	    case fontMenu_fontescape:
#if OPT_WIDE_CHARS
		maxparams = 5;
#else
		maxparams = 3;
#endif
		break;
	    case fontMenu_fontsel:
		maxparams = 2;
		break;
	    default:
		Bell(xw, XkbBI_MinorError, 0);
		return;
	    }
	    fontnum = result;

	    if (*param_count > maxparams) {	/* see if extra args given */
		Bell(xw, XkbBI_MinorError, 0);
		return;
	    }
	    switch (*param_count) {	/* assign 'em */
#if OPT_WIDE_CHARS
	    case 5:
		fonts.f_wb = x_strdup(params[4]);
		/* FALLTHRU */
	    case 4:
		fonts.f_w = x_strdup(params[3]);
#endif
		/* FALLTHRU */
	    case 3:
		fonts.f_b = x_strdup(params[2]);
		/* FALLTHRU */
	    case 2:
		fonts.f_n = x_strdup(params[1]);
		break;
	    }
	}

	SetVTFont(xw, fontnum, True, &fonts);
    }
}

Bool
SetVTFont(XtermWidget xw,
	  int which,
	  Bool doresize,
	  const VTFontNames * fonts)
{
    TScreen *screen = TScreenOf(xw);
    Bool result = False;

    TRACE(("SetVTFont(which=%d, f_n=%s, f_b=%s)\n", which,
	   (fonts && fonts->f_n) ? fonts->f_n : "<null>",
	   (fonts && fonts->f_b) ? fonts->f_b : "<null>"));

    if (IsIcon(screen)) {
	Bell(xw, XkbBI_MinorError, 0);
    } else if (which >= 0 && which < NMENUFONTS) {
	VTFontNames new_fnames;

	memset(&new_fnames, 0, sizeof(new_fnames));
	if (fonts != 0)
	    new_fnames = *fonts;

	if (which == fontMenu_fontsel) {	/* go get the selection */
	    result = FindFontSelection(xw, new_fnames.f_n, False);
	} else {
#define USE_CACHED(field, name) \
	    if (new_fnames.field == NULL) { \
		new_fnames.field = x_strdup(screen->menu_font_names[which][name]); \
		TRACE(("set new_fnames." #field " from menu_font_names[%d][" #name "] %s\n", \
		       which, NonNull(new_fnames.field))); \
	    } else { \
		TRACE(("set new_fnames." #field " reused\n")); \
	    }
#define SAVE_FNAME(field, name) \
	    if (new_fnames.field != NULL \
		&& (screen->menu_font_names[which][name] == NULL \
		 || strcmp(screen->menu_font_names[which][name], new_fnames.field))) { \
		TRACE(("updating menu_font_names[%d][" #name "] to \"%s\"\n", \
		       which, new_fnames.field)); \
		FREE_STRING(screen->menu_font_names[which][name]); \
		screen->menu_font_names[which][name] = x_strdup(new_fnames.field); \
	    }

	    USE_CACHED(f_n, fNorm);
	    USE_CACHED(f_b, fBold);
#if OPT_WIDE_CHARS
	    USE_CACHED(f_w, fWide);
	    USE_CACHED(f_wb, fWBold);
#endif
	    if (xtermLoadFont(xw,
			      &new_fnames,
			      doresize, which)) {
		/*
		 * If successful, save the data so that a subsequent query via
		 * OSC-50 will return the expected values.
		 */
		SAVE_FNAME(f_n, fNorm);
		SAVE_FNAME(f_b, fBold);
#if OPT_WIDE_CHARS
		SAVE_FNAME(f_w, fWide);
		SAVE_FNAME(f_wb, fWBold);
#endif
		result = True;
	    } else {
		Bell(xw, XkbBI_MinorError, 0);
	    }
	    FREE_FNAME(f_n);
	    FREE_FNAME(f_b);
#if OPT_WIDE_CHARS
	    FREE_FNAME(f_w);
	    FREE_FNAME(f_wb);
#endif
	}
    } else {
	Bell(xw, XkbBI_MinorError, 0);
    }
    TRACE(("...SetVTFont: %d\n", result));
    return result;
}

#if OPT_RENDERFONT
static void
trimSizeFromFace(char *face_name, float *face_size)
{
    char *first = strstr(face_name, ":size=");
    if (first == 0) {
	first = face_name;
    } else {
	first++;
    }
    if (!strncmp(first, "size=", (size_t) 5)) {
	char *last = strchr(first, ':');
	char mark;
	float value;
	char extra;
	TRACE(("...before trimming, font = \"%s\"\n", face_name));
	if (last == 0)
	    last = first + strlen(first);
	mark = *last;
	*last = '\0';
	if (sscanf(first, "size=%g%c", &value, &extra) == 1) {
	    TRACE(("...trimmed size from font: %g\n", value));
	    if (face_size != 0)
		*face_size = value;
	}
	if (mark) {
	    while ((*first++ = *++last) != '\0') {
		;
	    }
	} else {
	    if (first != face_name)
		--first;
	    *first = '\0';
	}
	TRACE(("...after trimming, font = \"%s\"\n", face_name));
    }
}
#endif

/*
 * Save a font specification to the proper list.
 */
static void
save2FontList(XtermWidget xw,
	      const char *name,
	      XtermFontNames * fontnames,
	      VTFontEnum which,
	      const char *source,
	      Bool check,
	      Bool ttf)
{
    char *value;
    size_t plen;
    Bool marked = False;
    Bool use_ttf = ttf;

    (void) xw;

    if (source == 0)
	source = "";
    while (isspace(CharOf(*source)))
	++source;

    /* fontconfig patterns can contain ':' separators, but we'll treat
     * a leading prefix specially to denote whether the pattern might be
     * XLFD ("x" or "xlfd") versus Xft ("xft").
     */
    for (plen = 0; source[plen] != '\0'; ++plen) {
	if (source[plen] == ':') {
	    marked = True;
	    switch (plen) {
	    case 0:
		++plen;		/* trim leading ':' */
		break;
	    case 1:
		if (!strncmp(source, "x", plen)) {
		    ++plen;
		    use_ttf = False;
		} else {
		    marked = False;
		}
		break;
	    case 3:
		if (!strncmp(source, "xft", plen)) {
		    ++plen;
		    use_ttf = True;
		} else {
		    marked = False;
		}
		break;
	    case 4:
		if (!strncmp(source, "xlfd", plen)) {
		    ++plen;
		    use_ttf = False;
		} else {
		    marked = False;
		}
		break;
	    default:
		marked = False;
		plen = 0;
		break;
	    }
	    break;
	}
    }
    if (!marked)
	plen = 0;
    value = x_strtrim(source + plen);
    if (value != 0) {
	Bool success = False;
#if OPT_RENDERFONT
	VTFontList *target = (use_ttf
			      ? &(fontnames->xft)
			      : &(fontnames->x11));
#else
	VTFontList *target = &(fontnames->x11);
#endif
	char ***list = 0;
	char **next = 0;
	size_t count = 0;

	(void) use_ttf;
	switch (which) {
	case fNorm:
	    list = &(target->list_n);
	    break;
	case fBold:
	    list = &(target->list_b);
	    break;
#if OPT_WIDE_ATTRS || OPT_RENDERWIDE
	case fItal:
	    list = &(target->list_i);
	    break;
	case fBtal:
	    list = &(target->list_bi);
	    break;
#endif
#if OPT_WIDE_CHARS
	case fWide:
	    list = &(target->list_w);
	    break;
	case fWBold:
	    list = &(target->list_wb);
	    break;
	case fWItal:
	    list = &(target->list_wi);
	    break;
	case fWBtal:
	    list = &(target->list_wbi);
	    break;
#endif
	case fMAX:
	    list = 0;
	    break;
	}

	if (list != 0) {
	    success = True;
	    if (*list != 0) {
		while ((*list)[count] != 0) {
		    if (IsEmpty((*list)[count])) {
			TRACE(("... initial %s\n", value));
			free((*list)[count]);
			break;
		    } else if (!strcmp(value, (*list)[count])) {
			TRACE(("... duplicate %s\n", value));
			success = False;
			break;
		    }
		    ++count;
		}
	    }
	    if (success) {
		next = (char **) realloc(*list, sizeof(char *) * (count + 2));
		if (next != 0) {
#if OPT_RENDERFONT
		    if (use_ttf) {
			trimSizeFromFace(value,
					 (count == 0 && which == fNorm)
					 ? &(xw->misc.face_size[0])
					 : (float *) 0);
		    }
#endif
		    next[count++] = value;
		    next[count] = 0;
		    *list = next;
		    TRACE(("... saved \"%s\" \"%s\" %lu:\"%s\"\n",
			   whichFontList(xw, target),
			   whichFontList2(xw, *list),
			   (unsigned long) count,
			   value));
		} else {
		    xtermWarning("realloc failure in save2FontList(%s)\n", name);
		    freeFontList(list);
		    success = False;
		}
	    }
	}
	if (success) {
#if (MAX_XFT_FONTS == MAX_XLFD_FONTS)
	    size_t limit = MAX_XFT_FONTS;
#else
	    size_t limit = use_ttf ? MAX_XFT_FONTS : MAX_XLFD_FONTS;
#endif
	    if (count > limit && *x_skip_blanks(value)) {
		if (check) {
		    xtermWarning("too many fonts for %s, ignoring %s\n",
				 whichFontEnum(which),
				 value);
		}
		if (list && *list) {
		    free((*list)[limit]);
		    (*list)[limit] = 0;
		}
	    }
	} else {
	    free(value);
	}
    }
}

/*
 * In principle, any of the font-name resources could be extended to be a list
 * of font-names.  That would be bad for performance, but as a basis for an
 * extension, parse the font-name as a comma-separated list, creating/updating
 * an array of font-names.
 */
void
allocFontList(XtermWidget xw,
	      const char *name,
	      XtermFontNames * target,
	      VTFontEnum which,
	      const char *source,
	      Bool ttf)
{
    char *blob;

    blob = x_strdup(source);
    if (blob != 0) {
	int n;
	int pass;
	char **list = 0;

	TRACE(("allocFontList %s name=\"%s\" source=\"%s\"\n",
	       whichFontEnum(which), name, blob));

	for (pass = 0; pass < 2; ++pass) {
	    unsigned count = 0;
	    if (pass)
		list[0] = blob;
	    for (n = 0; blob[n] != '\0'; ++n) {
		if (blob[n] == ',') {
		    ++count;
		    if (pass != 0) {
			blob[n] = '\0';
			list[count] = blob + n + 1;
		    }
		}
	    }
	    if (!pass) {
		if (count == 0 && *blob == '\0')
		    break;
		list = TypeCallocN(char *, count + 2);
		if (list == 0)
		    break;
	    }
	}
	if (list) {
	    for (n = 0; list[n] != 0; ++n) {
		if (*list[n]) {
		    save2FontList(xw, name, target, which, list[n], True, ttf);
		}
	    }
	    free(list);
	}
    }
    free(blob);
}

static void
initFontList(XtermWidget xw,
	     const char *name,
	     XtermFontNames * target,
	     Bool ttf)
{
    int which;

    TRACE(("initFontList(%s)\n", name));
    for (which = 0; which < fMAX; ++which) {
	save2FontList(xw, name, target, (VTFontEnum) which, "", False, ttf);
    }
}

void
initFontLists(XtermWidget xw)
{
    TRACE(("initFontLists\n"));
    initFontList(xw, "x11 font", &(xw->work.fonts), False);
#if OPT_RENDERFONT
    initFontList(xw, "xft font", &(xw->work.fonts), True);
#endif
#if OPT_LOAD_VTFONTS || OPT_WIDE_CHARS
    initFontList(xw, "cached font",
		 &(xw->screen.cacheVTFonts.fonts), False);
#endif
}

void
copyFontList(char ***targetp, char **source)
{
    freeFontList(targetp);

    if (source != 0) {
	int pass;
	size_t count;

	for (pass = 0; pass < 2; ++pass) {
	    for (count = 0; source[count] != 0; ++count) {
		if (pass)
		    (*targetp)[count] = x_strdup(source[count]);
	    }
	    if (!pass) {
		++count;
		*targetp = TypeCallocN(char *, count);
	    }
	}
    } else {
	*targetp = TypeCallocN(char *, 2);
	(*targetp)[0] = x_strdup("");
    }
}

#if OPT_LOAD_VTFONTS || OPT_WIDE_CHARS
static Boolean
merge_sublist(char ***targetp, char **source)
{
    Boolean result = False;
    if ((*targetp == 0 || IsEmpty(**targetp)) && !IsEmpty(*source)) {
	copyFontList(targetp, source);
	result = True;
    }
    return result;
}
#endif

void
freeFontList(char ***targetp)
{
    if (targetp != 0) {
	char **target = *targetp;
	if (target != 0) {
	    int n;
	    for (n = 0; target[n] != 0; ++n) {
		free(target[n]);
	    }
	    free(target);
	    *targetp = 0;
	}
    }
}

void
freeFontLists(VTFontList * lists)
{
    int which;

    TRACE(("freeFontLists\n"));
    for (which = 0; which < fMAX; ++which) {
	char ***target = 0;
	switch (which) {
	case fNorm:
	    target = &(lists->list_n);
	    break;
	case fBold:
	    target = &(lists->list_b);
	    break;
#if OPT_WIDE_ATTRS || OPT_RENDERWIDE
	case fItal:
	    target = &(lists->list_i);
	    break;
	case fBtal:
	    target = &(lists->list_bi);
	    break;
#endif
#if OPT_WIDE_CHARS
	case fWide:
	    target = &(lists->list_w);
	    break;
	case fWBold:
	    target = &(lists->list_wb);
	    break;
	case fWItal:
	    target = &(lists->list_wi);
	    break;
	case fWBtal:
	    target = &(lists->list_wbi);
	    break;
#endif
	default:
	    target = 0;
	    break;
	}
	freeFontList(target);
    }
}

/*
 * Return a pointer to the XLFD font information for a given font class.
 * XXX make this allocate the font on demand.
 */
XTermFonts *
getNormalFont(TScreen *screen, int which)
{
    XTermFonts *result = 0;
    if (which >= 0 && which < fMAX)
	result = GetNormalFont(screen, which);
    return result;
}

#if OPT_DEC_CHRSET
XTermFonts *
getDoubleFont(TScreen *screen, int which)
{
    XTermFonts *result = 0;
    if ((int) which >= 0 && which < NUM_CHRSET)
	result = GetDoubleFont(screen, which);
    return result;
}

#if OPT_RENDERFONT
void
getDoubleXftFont(XTermDraw * params, XTermXftFonts *data, unsigned chrset, unsigned attr_flags)
{
    XtermWidget xw = params->xw;
    TScreen *screen = TScreenOf(xw);
    XftPattern *top_pattern;
    int fontnum = screen->menu_font_number;
    const char *face_name = getFaceName(xw, False);

    if (chrset != CSET_SWL
	&& (top_pattern = XftNameParse(face_name)) != 0) {
	double face_size = (double) xw->misc.face_size[fontnum];
	XftPattern *sub_pattern = XftPatternDuplicate(top_pattern);
	const char *category = "doublesize";

	switch (chrset) {
	case CSET_DHL_TOP:
	    category = "DHL_TOP";
	    goto double_high;
	case CSET_DHL_BOT:
	    category = "DHL_BOT";
	  double_high:
	    face_size *= 2;
	    XftPatternBuild(sub_pattern,
			    NormXftPattern,
			    (void *) 0);
	    break;
	case CSET_DWL:
	    category = "DWL";
	    XftPatternBuild(sub_pattern,
			    NormXftPattern,
			    FC_ASPECT, XftTypeDouble, 2.0,
			    (void *) 0);
	    break;
	}
	if (attr_flags & BOLD) {
	    XftPatternBuild(sub_pattern,
			    XFT_WEIGHT, XftTypeInteger, XFT_WEIGHT_BOLD,
			    (void *) 0);
	}
	xtermOpenXft(xw, data, 0, face_name, sub_pattern, category);
	data->pattern = sub_pattern;
    }
}
#endif
#endif /* OPT_DEC_CHRSET */

#if OPT_WIDE_ATTRS || OPT_RENDERWIDE
XTermFonts *
getItalicFont(TScreen *screen, int which)
{
    XTermFonts *result = 0;
#if OPT_WIDE_ATTRS
    if (which >= 0 && which < fMAX)
	result = GetItalicFont(screen, which);
#else
    (void) screen;
    (void) which;
#endif
    return result;
}
#endif

#if OPT_RENDERFONT
/*
 * This returns a pointer to everything known about a given Xft font.
 * XXX make this allocate the font on demand.
 */
XTermXftFonts *
getMyXftFont(XtermWidget xw, int which, int fontnum)
{
    TScreen *screen = TScreenOf(xw);
    XTermXftFonts *result = 0;
    if (fontnum >= 0 && fontnum < NMENUFONTS) {
	switch ((VTFontEnum) which) {
	case fNorm:
	    result = &(screen->renderFontNorm[fontnum]);
	    break;
	case fBold:
	    result = &(screen->renderFontBold[fontnum]);
	    break;
#if OPT_WIDE_ATTRS || OPT_RENDERWIDE
	case fItal:
	    result = &(screen->renderFontItal[fontnum]);
	    break;
	case fBtal:
	    result = &(screen->renderFontBtal[fontnum]);
	    break;
#endif
#if OPT_WIDE_CHARS
	case fWide:
	    result = &(screen->renderWideNorm[fontnum]);
	    break;
	case fWBold:
	    result = &(screen->renderWideBold[fontnum]);
	    break;
	case fWItal:
	    result = &(screen->renderWideItal[fontnum]);
	    break;
	case fWBtal:
	    result = &(screen->renderWideBtal[fontnum]);
	    break;
#endif
	case fMAX:
	    break;
	}
    }
    return result;
}

const char *
whichXftFonts(XtermWidget xw, const XTermXftFonts *data)
{
    TScreen *screen = TScreenOf(xw);
    const char *result = "?";
#define CHECK(name) ((data >= &(screen->name[0])) && (data < &(screen->name[NMENUFONTS]))) result = #name
    if CHECK
	(renderFontNorm);
    else if CHECK
	(renderFontBold);
#if OPT_WIDE_ATTRS || OPT_RENDERWIDE
    else if CHECK
	(renderFontItal);
    else if CHECK
	(renderFontBtal);
#endif
#if OPT_WIDE_CHARS
    else if CHECK
	(renderWideNorm);
    else if CHECK
	(renderWideBold);
    else if CHECK
	(renderWideItal);
    else if CHECK
	(renderWideBtal);
#endif
#if OPT_DEC_CHRSET
#if OPT_RENDERFONT
    else {
	int n;
	for (n = 0; n < NUM_CHRSET; ++n) {
	    if (data == &screen->double_xft_fonts[n]) {
		result = "double_xft_fonts";
		break;
	    }
	}
    }
#endif
#endif /* OPT_DEC_CHRSET */
    return result;
}

XftFont *
getXftFont(XtermWidget xw, VTFontEnum which, int fontnum)
{
    XTermXftFonts *data = getMyXftFont(xw, (int) which, fontnum);
    XftFont *result = 0;
    if (data != 0)
	result = XftFp(data);
    return result;
}
#endif

const char *
whichFontEnum(VTFontEnum value)
{
    const char *result = "?";
#define DATA(name) case name: result = #name; break
    switch (value) {
	DATA(fNorm);
	DATA(fBold);
#if OPT_WIDE_ATTRS || OPT_RENDERWIDE
	DATA(fItal);
	DATA(fBtal);
#endif
#if OPT_WIDE_CHARS
	DATA(fWide);
	DATA(fWBold);
	DATA(fWItal);
	DATA(fWBtal);
#endif
	DATA(fMAX);
    }
#undef DATA
    return result;
}

const char *
whichFontList(XtermWidget xw, const VTFontList * value)
{
    const char *result = "?";
    if (value == &(xw->work.fonts.x11))
	result = "x11_fontnames";
#if OPT_RENDERFONT
    else if (value == &(xw->work.fonts.xft))
	result = "xft_fontnames";
#endif
#if OPT_LOAD_VTFONTS || OPT_WIDE_CHARS
    else if (value == &(xw->screen.cacheVTFonts.fonts.x11))
	result = "cached_fontnames";
#endif
    return result;
}

static const char *
whichFontList2s(VTFontList * list, char **value)
{
    const char *result = 0;
#define DATA(name) if (value == (list->name)) result = #name
    DATA(list_n);
    DATA(list_b);
#if OPT_WIDE_ATTRS || OPT_RENDERWIDE
    DATA(list_i);
    DATA(list_bi);
#endif
#if OPT_WIDE_CHARS
    DATA(list_w);
    DATA(list_wb);
    DATA(list_wi);
    DATA(list_wbi);
#endif
#undef DATA
    return result;
}

const char *
whichFontList2(XtermWidget xw, char **value)
{
    const char *result = 0;
#define DATA(name) (result = whichFontList2s(&(xw->name), value))
    if (DATA(work.fonts.x11) == 0) {
#if OPT_RENDERFONT
	if (DATA(work.fonts.xft) == 0)
#endif
#if OPT_LOAD_VTFONTS || OPT_WIDE_CHARS
	    if (DATA(screen.cacheVTFonts.fonts.x11) == 0)
#endif
		result = "?";
    }
#undef DATA
    return result;
}
