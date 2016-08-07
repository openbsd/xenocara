/* $XTermId: fontutils.c,v 1.453 2016/06/03 08:56:53 tom Exp $ */

/*
 * Copyright 1998-2015,2016 by Thomas E. Dickey
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
#include <menu.h>
#include <xstrings.h>
#include <xterm.h>

#include <stdio.h>
#include <ctype.h>

#define SetFontWidth(screen,dst,src)  (dst)->f_width = (src)
#define SetFontHeight(screen,dst,src) (dst)->f_height = dimRound((screen)->scale_height * (float) (src))

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
	    if (fonts == 0 || myfonts.field != fonts->field) { \
		FREE_STRING(myfonts.field); \
		myfonts.field = 0; \
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

#if OPT_RENDERFONT
static void fillInFaceSize(XtermWidget, int);
#endif

#if OPT_SHIFT_FONTS
static int lookupOneFontSize(XtermWidget, int);
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
#define MIXED(name) screen->name[fontnum].map.mixed
    if (xw->work.render_font == True) {
	int fontnum = screen->menu_font_number;

	screen->allow_packing = (Boolean) (MIXED(renderFontNorm)
					   || MIXED(renderFontBold)
					   || MIXED(renderFontItal)
#if OPT_RENDERWIDE
					   || MIXED(renderWideNorm)
					   || MIXED(renderWideBold)
					   || MIXED(renderWideItal)
#endif
	    );
#undef MIXED
    }
#endif /* OPT_RENDERFONT */

    value = screen->allow_packing;

    SetItemSensitivity(fontMenuEntries[fontMenu_font_packedfont].widget, value);
}
#endif

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

    Atom fontatom = XInternAtom(dpy, "FONT", False);
    char *name = 0;
    char *str;

    /*
     * first get the full font name
     */
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
    if (last_name != 0)
	XFree(last_name);
    last_name = name;

    if (result != 0) {
	if (!check_fontname(name))
	    return 0;
	if (*result != 0)
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
	want = ALLOCHUNK(want);
	if (last != 0) {
	    char *save = *result;
	    *result = TypeRealloc(char, want, *result);
	    if (*result == 0)
		free(save);
	} else {
	    if ((*result = TypeMallocN(char, want)) != 0)
		**result = '\0';
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

    return result;
}

static char *
bold_font_name(FontNameProperties *props, int use_average_width)
{
    return derive_font_name(props, "bold", use_average_width, props->end);
}

#if OPT_WIDE_ATTRS
static char *
italic_font_name(FontNameProperties *props, int use_average_width)
{
    FontNameProperties myprops = *props;
    myprops.slant = "o";
    return derive_font_name(&myprops, props->weight, use_average_width, props->end);
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
xtermSpecialFont(TScreen *screen, unsigned attr_flags, unsigned draw_flags, unsigned chrset)
{
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

    props = get_font_name_props(screen->display, screen->fnts[fNorm].fs, 0);
    if (props == 0)
	return result;

    pixel_size = props->pixel_size;
    res_x = props->res_x;
    res_y = props->res_y;
    if (attr_flags & BOLD)
	weight = "bold";
    else
	weight = props->weight;

    if (CSET_DOUBLE(chrset))
	res_x *= 2;

    if (chrset == CSET_DHL_TOP
	|| chrset == CSET_DHL_BOT) {
	res_y *= 2;
	pixel_size *= 2;
    }
#if OPT_TRACE
    if (old_props.res_x != res_x
	|| old_props.res_x != res_y
	|| old_props.pixel_size != pixel_size
	|| strcmp(old_props.spacing, props->spacing)) {
	TRACE(("xtermSpecialFont(atts = %#x, draw = %#x, chrset = %#x)\n",
	       attr_flags, draw_flags, chrset));
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
    append_fontname_num(&result, (draw_flags & NORESOLUTION) ? -1 : res_x);
    append_fontname_num(&result, (draw_flags & NORESOLUTION) ? -1 : res_y);
    append_fontname_str(&result, props->spacing);
    append_fontname_str(&result, 0);
    append_fontname_str(&result, props->end);

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

    if (normal->all_chars_exist) {
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
    TRACE(("same_font_size height %d/%d, min %d/%d max %d/%d\n",
	   nfs->ascent + nfs->descent,
	   bfs->ascent + bfs->descent,
	   nfs->min_bounds.width, bfs->min_bounds.width,
	   nfs->max_bounds.width, bfs->max_bounds.width));
    return screen->free_bold_box
	|| ((nfs->ascent + nfs->descent) == (bfs->ascent + bfs->descent)
	    && (nfs->min_bounds.width == bfs->min_bounds.width
		|| nfs->min_bounds.width == bfs->min_bounds.width + 1)
	    && (nfs->max_bounds.width == bfs->max_bounds.width
		|| nfs->max_bounds.width == bfs->max_bounds.width + 1));
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

/*
 * Check if the font looks like a double width font (i.e. contains
 * characters of width X and 2X
 */
#if OPT_WIDE_CHARS
static int
is_double_width_font(XFontStruct *fs)
{
    return ((2 * fs->min_bounds.width) == fs->max_bounds.width);
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
    data.f_n = x_strdup(normal);
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

/*
 * Open the given font and verify that it is non-empty.  Return a null on
 * failure.
 */
Bool
xtermOpenFont(XtermWidget xw,
	      const char *name,
	      XTermFonts * result,
	      fontWarningTypes warn,
	      Bool force)
{
    Bool code = False;
    TScreen *screen = TScreenOf(xw);

    if (!IsEmpty(name)) {
	if ((result->fs = XLoadQueryFont(screen->display, name)) != 0) {
	    code = True;
	    if (EmptyFont(result->fs)) {
		(void) xtermCloseFont(xw, result);
		code = False;
	    } else {
		result->fn = x_strdup(name);
	    }
	} else if (XmuCompareISOLatin1(name, DEFFONT) != 0) {
	    if (warn <= xw->misc.fontWarnings
#if OPT_RENDERFONT
		&& !UsingRenderFont(xw)
#endif
		) {
		TRACE(("OOPS: cannot load font %s\n", name));
		xtermWarning("cannot load font '%s'\n", name);
#if OPT_RENDERFONT
		/*
		 * Do a sanity check in case someone's mixed up xterm with
		 * one of those programs that read their resource data from
		 * xterm's namespace.
		 */
		if (strchr(name, ':') != 0 || strchr(name, '=') != 0) {
		    xtermWarning("Use the \"-fa\" option for the Xft fonts\n");
		}
#endif
	    } else {
		TRACE(("xtermOpenFont: cannot load font '%s'\n", name));
	    }
	    if (force) {
		code = xtermOpenFont(xw, DEFFONT, result, fwAlways, True);
	    }
	}
    }
    return code;
}

/*
 * Close the font and free the font info.
 */
XTermFonts *
xtermCloseFont(XtermWidget xw, XTermFonts * fnt)
{
    if (fnt != 0 && fnt->fs != 0) {
	TScreen *screen = TScreenOf(xw);

	clrCgsFonts(xw, WhichVWin(screen), fnt);
	XFreeFont(screen->display, fnt->fs);
	xtermFreeFontInfo(fnt);
    }
    return 0;
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
}

void
xtermFreeFontInfo(XTermFonts * target)
{
    target->chrset = 0;
    target->flags = 0;
    if (target->fn != 0) {
	free(target->fn);
	target->fn = 0;
    }
    target->fs = 0;
}

#if OPT_REPORT_FONTS
static void
reportXCharStruct(const char *tag, XCharStruct * cs)
{
    printf("\t\t%s:\n", tag);
    printf("\t\t\tlbearing: %d\n", cs->lbearing);
    printf("\t\t\trbearing: %d\n", cs->rbearing);
    printf("\t\t\twidth:    %d\n", cs->width);
    printf("\t\t\tascent:   %d\n", cs->ascent);
    printf("\t\t\tdescent:  %d\n", cs->descent);
}

static void
reportOneVTFont(const char *tag,
		XTermFonts * fnt)
{
    if (!IsEmpty(fnt->fn)) {
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

	printf("\t%s: %s\n", tag, NonNull(fnt->fn));
	printf("\t\tall chars:     %s\n", fs->all_chars_exist ? "yes" : "no");
	printf("\t\tdefault char:  %d\n", fs->default_char);
	printf("\t\tdirection:     %d\n", fs->direction);
	printf("\t\tascent:        %d\n", fs->ascent);
	printf("\t\tdescent:       %d\n", fs->descent);
	printf("\t\tfirst char:    %u\n", first_char);
	printf("\t\tlast char:     %u\n", last_char);
	printf("\t\tmaximum-chars: %u\n", countGlyphs(fs));
	if (FontLacksMetrics(fnt)) {
	    printf("\t\tmissing-chars: ?\n");
	    printf("\t\tpresent-chars: ?\n");
	} else {
	    unsigned missing = 0;
	    unsigned ch;
	    for (ch = first_char; ch <= last_char; ++ch) {
		if (xtermMissingChar(ch, fnt)) {
		    ++missing;
		}
	    }
	    printf("\t\tmissing-chars: %u\n", missing);
	    printf("\t\tpresent-chars: %u\n", countGlyphs(fs) - missing);
	}
	printf("\t\tmin_byte1:     %d\n", fs->min_byte1);
	printf("\t\tmax_byte1:     %d\n", fs->max_byte1);
	printf("\t\tproperties:    %d\n", fs->n_properties);
	reportXCharStruct("min_bounds", &(fs->min_bounds));
	reportXCharStruct("max_bounds", &(fs->max_bounds));
	/* TODO: report fs->properties and fs->per_char */
    }
}

static void
reportVTFontInfo(XtermWidget xw, int fontnum)
{
    if (resource.reportFonts) {
	TScreen *screen = TScreenOf(xw);

	if (fontnum) {
	    printf("Loaded VTFonts(font%d)\n", fontnum);
	} else {
	    printf("Loaded VTFonts(default)\n");
	}
	reportOneVTFont("fNorm", &screen->fnts[fNorm]);
	reportOneVTFont("fBold", &screen->fnts[fBold]);
#if OPT_WIDE_CHARS
	reportOneVTFont("fWide", &screen->fnts[fWide]);
	reportOneVTFont("fWBold", &screen->fnts[fWBold]);
#endif
    }
}
#endif

void
xtermUpdateFontGCs(XtermWidget xw, XTermFonts * fnts)
{
    TScreen *screen = TScreenOf(xw);
    VTwin *win = WhichVWin(screen);
    Pixel new_normal = getXtermForeground(xw, xw->flags, xw->cur_foreground);
    Pixel new_revers = getXtermBackground(xw, xw->flags, xw->cur_background);

    setCgsFore(xw, win, gcNorm, new_normal);
    setCgsBack(xw, win, gcNorm, new_revers);
    setCgsFont(xw, win, gcNorm, &(fnts[fNorm]));

    copyCgs(xw, win, gcBold, gcNorm);
    setCgsFont(xw, win, gcBold, &(fnts[fBold]));

    setCgsFore(xw, win, gcNormReverse, new_revers);
    setCgsBack(xw, win, gcNormReverse, new_normal);
    setCgsFont(xw, win, gcNormReverse, &(fnts[fNorm]));

    copyCgs(xw, win, gcBoldReverse, gcNormReverse);
    setCgsFont(xw, win, gcBoldReverse, &(fnts[fBold]));

    if_OPT_WIDE_CHARS(screen, {
	if (fnts[fWide].fs != 0
	    && fnts[fWBold].fs != 0) {
	    setCgsFore(xw, win, gcWide, new_normal);
	    setCgsBack(xw, win, gcWide, new_revers);
	    setCgsFont(xw, win, gcWide, &(fnts[fWide]));

	    copyCgs(xw, win, gcWBold, gcWide);
	    setCgsFont(xw, win, gcWBold, &(fnts[fWBold]));

	    setCgsFore(xw, win, gcWideReverse, new_revers);
	    setCgsBack(xw, win, gcWideReverse, new_normal);
	    setCgsFont(xw, win, gcWideReverse, &(fnts[fWide]));

	    copyCgs(xw, win, gcWBoldReverse, gcWideReverse);
	    setCgsFont(xw, win, gcWBoldReverse, &(fnts[fWBold]));
	}
    });
}

#if OPT_TRACE
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

int
xtermLoadFont(XtermWidget xw,
	      const VTFontNames * fonts,
	      Bool doresize,
	      int fontnum)
{
    TScreen *screen = TScreenOf(xw);
    VTwin *win = WhichVWin(screen);

    VTFontNames myfonts;
    FontNameProperties *fp;
    XTermFonts fnts[fMAX];
    char *tmpname = NULL;
    char *normal = NULL;
    Boolean proportional = False;
    fontWarningTypes warn[fMAX];
    int j;

    memset(&myfonts, 0, sizeof(myfonts));
    memset(fnts, 0, sizeof(fnts));

    if (fonts != 0)
	myfonts = *fonts;
    if (!check_fontname(myfonts.f_n))
	return 0;

    /*
     * Check the font names against the resource values, to see which were
     * derived in a previous call.  If so, we'll only warn about those if
     * the warning level is set to "always".
     */
    for (j = 0; j < fMAX; ++j) {
	warn[j] = fwAlways;
    }
#define CmpResource(field, index) \
    if (same_font_name(screen->menu_font_names[fontnum][index], myfonts.field)) \
	warn[index] = fwResource

    CmpResource(f_n, fNorm);
    if (fontnum == fontMenu_default) {
	CmpResource(f_b, fBold);
#if OPT_WIDE_CHARS
	CmpResource(f_b, fWide);
	CmpResource(f_b, fWBold);
#endif
    }

    if (fontnum == fontMenu_fontescape
	&& myfonts.f_n != screen->MenuFontName(fontnum)) {
	if ((tmpname = x_strdup(myfonts.f_n)) == 0)
	    return 0;
    }

    TRACE(("Begin Cgs - xtermLoadFont(%s)\n", myfonts.f_n));
    releaseWindowGCs(xw, win);

#define DbgResource(name, field, index) \
    TRACE(("xtermLoadFont #%d "name" %s%s\n", \
    	   fontnum, \
	   (warn[index] == fwResource) ? "*" : " ", \
	   NonNull(myfonts.field)))
    DbgResource("normal", f_n, fNorm);
    DbgResource("bold  ", f_b, fBold);
#if OPT_WIDE_CHARS
    DbgResource("wide  ", f_w, fWide);
    DbgResource("w/bold", f_wb, fWBold);
#endif

    /*
     * If we are opening the default font, and it happens to be missing, force
     * that to the compiled-in default font, e.g., "fixed".  If we cannot open
     * the font, disable it from the menu.
     */
    if (!xtermOpenFont(xw,
		       myfonts.f_n,
		       &fnts[fNorm],
		       warn[fNorm],
		       (fontnum == fontMenu_default))) {
	if (fontnum != fontMenu_fontsel) {
	    SetItemSensitivity(fontMenuEntries[fontnum].widget, False);
	}
	goto bad;
    }

    normal = x_strdup(myfonts.f_n);
    if (!check_fontname(myfonts.f_b)) {
	warn[fBold] = fwAlways;
	fp = get_font_name_props(screen->display, fnts[fNorm].fs, &normal);
	if (fp != 0) {
	    FREE_FNAME(f_b);
	    myfonts.f_b = bold_font_name(fp, fp->average_width);
	    if (!xtermOpenFont(xw, myfonts.f_b, &fnts[fBold], fwAlways, False)) {
		FREE_FNAME(f_b);
		myfonts.f_b = bold_font_name(fp, -1);
		xtermOpenFont(xw, myfonts.f_b, &fnts[fBold], fwAlways, False);
	    }
	    TRACE(("...derived bold '%s'\n", NonNull(myfonts.f_b)));
	}
	if (fp == 0 || fnts[fBold].fs == 0) {
	    xtermCopyFontInfo(&fnts[fBold], &fnts[fNorm]);
	    TRACE(("...cannot load a matching bold font\n"));
	} else if (comparable_metrics(fnts[fNorm].fs, fnts[fBold].fs)
		   && same_font_size(xw, fnts[fNorm].fs, fnts[fBold].fs)
		   && got_bold_font(screen->display, fnts[fBold].fs, myfonts.f_b)) {
	    TRACE(("...got a matching bold font\n"));
	    cache_menu_font_name(screen, fontnum, fBold, myfonts.f_b);
	} else {
	    xtermCloseFont2(xw, fnts, fBold);
	    fnts[fBold] = fnts[fNorm];
	    TRACE(("...did not get a matching bold font\n"));
	}
    } else if (!xtermOpenFont(xw, myfonts.f_b, &fnts[fBold], warn[fBold], False)) {
	xtermCopyFontInfo(&fnts[fBold], &fnts[fNorm]);
	warn[fBold] = fwAlways;
	TRACE(("...cannot load bold font '%s'\n", NonNull(myfonts.f_b)));
    } else {
	cache_menu_font_name(screen, fontnum, fBold, myfonts.f_b);
    }

    /*
     * If there is no widefont specified, fake it by doubling AVERAGE_WIDTH
     * of normal fonts XLFD, and asking for it.  This plucks out 18x18ja
     * and 12x13ja as the corresponding fonts for 9x18 and 6x13.
     */
    if_OPT_WIDE_CHARS(screen, {
	Boolean derived;
	char *bold = NULL;

	if (check_fontname(myfonts.f_w)) {
	    cache_menu_font_name(screen, fontnum, fWide, myfonts.f_w);
	} else if (screen->utf8_fonts && !is_double_width_font(fnts[fNorm].fs)) {
	    FREE_FNAME(f_w);
	    fp = get_font_name_props(screen->display, fnts[fNorm].fs, &normal);
	    if (fp != 0) {
		myfonts.f_w = wide_font_name(fp);
		warn[fWide] = fwAlways;
		TRACE(("...derived wide %s\n", NonNull(myfonts.f_w)));
		cache_menu_font_name(screen, fontnum, fWide, myfonts.f_w);
	    }
	}

	if (check_fontname(myfonts.f_w)) {
	    (void) xtermOpenFont(xw, myfonts.f_w, &fnts[fWide], warn[fWide], False);
	} else {
	    xtermCopyFontInfo(&fnts[fWide], &fnts[fNorm]);
	    warn[fWide] = fwAlways;
	}

	derived = False;
	if (!check_fontname(myfonts.f_wb)) {
	    fp = get_font_name_props(screen->display, fnts[fBold].fs, &bold);
	    if (fp != 0) {
		myfonts.f_wb = widebold_font_name(fp);
		warn[fWBold] = fwAlways;
		derived = True;
	    }
	}

	if (check_fontname(myfonts.f_wb)) {

	    xtermOpenFont(xw,
			  myfonts.f_wb,
			  &fnts[fWBold],
			  (screen->utf8_fonts
			   ? warn[fWBold]
			   : (fontWarningTypes) (xw->misc.fontWarnings + 1)),
			  False);

	    if (derived
		&& !compatibleWideCounts(fnts[fWide].fs, fnts[fWBold].fs)) {
		xtermCloseFont2(xw, fnts, fWBold);
	    }
	    if (fnts[fWBold].fs == 0) {
		FREE_FNAME(f_wb);
		if (IsEmpty(myfonts.f_w)) {
		    myfonts.f_wb = x_strdup(myfonts.f_b);
		    warn[fWBold] = fwAlways;
		    xtermCopyFontInfo(&fnts[fWBold], &fnts[fBold]);
		    TRACE(("...cannot load wide-bold, use bold %s\n",
			   NonNull(myfonts.f_b)));
		} else {
		    myfonts.f_wb = x_strdup(myfonts.f_w);
		    warn[fWBold] = fwAlways;
		    xtermCopyFontInfo(&fnts[fWBold], &fnts[fWide]);
		    TRACE(("...cannot load wide-bold, use wide %s\n",
			   NonNull(myfonts.f_w)));
		}
	    } else {
		TRACE(("...%s wide/bold %s\n",
		       derived ? "derived" : "given",
		       NonNull(myfonts.f_wb)));
		cache_menu_font_name(screen, fontnum, fWBold, myfonts.f_wb);
	    }
	} else if (is_double_width_font(fnts[fBold].fs)) {
	    xtermCopyFontInfo(&fnts[fWBold], &fnts[fBold]);
	    warn[fWBold] = fwAlways;
	    TRACE(("...bold font is double-width, use it %s\n", NonNull(myfonts.f_b)));
	} else {
	    xtermCopyFontInfo(&fnts[fWBold], &fnts[fWide]);
	    warn[fWBold] = fwAlways;
	    TRACE(("...cannot load wide bold font, use wide %s\n", NonNull(myfonts.f_w)));
	}

	free(bold);

	if (EmptyFont(fnts[fWBold].fs))
	    goto bad;		/* can't use a 0-sized font */
    });

    /*
     * Most of the time this call to load the font will succeed, even if
     * there is no wide font :  the X server doubles the width of the
     * normal font, or similar.
     *
     * But if it did fail for some reason, then nevermind.
     */
    if (EmptyFont(fnts[fBold].fs))
	goto bad;		/* can't use a 0-sized font */

    if (!same_font_size(xw, fnts[fNorm].fs, fnts[fBold].fs)
	&& (is_fixed_font(fnts[fNorm].fs) && is_fixed_font(fnts[fBold].fs))) {
	TRACE(("...ignoring mismatched normal/bold fonts\n"));
	xtermCloseFont2(xw, fnts, fBold);
	xtermCopyFontInfo(&fnts[fBold], &fnts[fNorm]);
    }

    if_OPT_WIDE_CHARS(screen, {
	if (fnts[fWide].fs != 0
	    && fnts[fWBold].fs != 0
	    && (!comparable_metrics(fnts[fWide].fs, fnts[fWBold].fs)
		|| (!same_font_size(xw, fnts[fWide].fs, fnts[fWBold].fs)
		    && is_fixed_font(fnts[fWide].fs)
		    && is_fixed_font(fnts[fWBold].fs)))) {
	    TRACE(("...ignoring mismatched normal/bold wide fonts\n"));
	    xtermCloseFont2(xw, fnts, fWBold);
	    xtermCopyFontInfo(&fnts[fWBold], &fnts[fWide]);
	}
    });

    /*
     * Normal/bold fonts should be the same width.  Also, the min/max
     * values should be the same.
     */
    if (!is_fixed_font(fnts[fNorm].fs)
	|| !is_fixed_font(fnts[fBold].fs)
	|| fnts[fNorm].fs->max_bounds.width != fnts[fBold].fs->max_bounds.width) {
	TRACE(("Proportional font! normal %d/%d, bold %d/%d\n",
	       fnts[fNorm].fs->min_bounds.width,
	       fnts[fNorm].fs->max_bounds.width,
	       fnts[fBold].fs->min_bounds.width,
	       fnts[fBold].fs->max_bounds.width));
	proportional = True;
    }

    if_OPT_WIDE_CHARS(screen, {
	if (fnts[fWide].fs != 0
	    && fnts[fWBold].fs != 0
	    && (!is_fixed_font(fnts[fWide].fs)
		|| !is_fixed_font(fnts[fWBold].fs)
		|| fnts[fWide].fs->max_bounds.width != fnts[fWBold].fs->max_bounds.width)) {
	    TRACE(("Proportional font! wide %d/%d, wide bold %d/%d\n",
		   fnts[fWide].fs->min_bounds.width,
		   fnts[fWide].fs->max_bounds.width,
		   fnts[fWBold].fs->min_bounds.width,
		   fnts[fWBold].fs->max_bounds.width));
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

    xtermCopyFontInfo(&(screen->fnts[fNorm]), &fnts[fNorm]);
    xtermCopyFontInfo(&(screen->fnts[fBold]), &fnts[fBold]);
#if OPT_WIDE_CHARS
    xtermCopyFontInfo(&(screen->fnts[fWide]), &fnts[fWide]);
    if (fnts[fWBold].fs == NULL)
	xtermCopyFontInfo(&fnts[fWBold], &fnts[fWide]);
    xtermCopyFontInfo(&(screen->fnts[fWBold]), &fnts[fWBold]);
#endif

    xtermUpdateFontGCs(xw, screen->fnts);

#if OPT_BOX_CHARS
    screen->allow_packing = proportional;
    setupPackedFonts(xw);
#endif
    screen->fnt_prop = (Boolean) (proportional && !(screen->force_packed));
    screen->fnt_boxes = True;

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
	screen->fnt_boxes = False;
	TRACE(("assume Xft missing line-drawing chars\n"));
    } else
#endif
    {
	unsigned ch;

#if OPT_TRACE
#define TRACE_MISS(index) show_font_misses(#index, &fnts[index])
	TRACE_MISS(fNorm);
	TRACE_MISS(fBold);
#if OPT_WIDE_CHARS
	TRACE_MISS(fWide);
	TRACE_MISS(fWBold);
#endif
#endif

	for (ch = 1; ch < 32; ch++) {
	    unsigned n = ch;
#if OPT_WIDE_CHARS
	    if (screen->utf8_mode || screen->unicode_font) {
		n = dec2ucs(ch);
		if (n == UCS_REPL)
		    continue;
	    }
#endif
	    if (IsXtermMissingChar(screen, n, &fnts[fNorm])) {
		TRACE(("missing normal char #%d\n", n));
		screen->fnt_boxes = False;
		break;
	    }
	    if (IsXtermMissingChar(screen, n, &fnts[fBold])) {
		TRACE(("missing bold char #%d\n", n));
		screen->fnt_boxes = False;
		break;
	    }
	}
    }
    TRACE(("Will %suse internal line-drawing characters\n",
	   screen->fnt_boxes ? "not " : ""));
#endif

    if (screen->always_bold_mode) {
	screen->enbolden = screen->bold_mode;
    } else {
	screen->enbolden = screen->bold_mode
	    && ((fnts[fNorm].fs == fnts[fBold].fs)
		|| same_font_name(normal, myfonts.f_b));
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
	screen->menu_font_sizes[fontnum] = FontSize(fnts[fNorm].fs);
#endif
    }
    if (normal)
	free(normal);
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
    if (fnts[fNorm].fn == fnts[fBold].fn) {
	free(fnts[fNorm].fn);
    } else {
	free(fnts[fNorm].fn);
	free(fnts[fBold].fn);
    }
#if OPT_WIDE_CHARS
    free(fnts[fWide].fn);
    free(fnts[fWBold].fn);
#endif
    return 1;

  bad:
    if (normal)
	free(normal);
    if (tmpname)
	free(tmpname);

#if OPT_RENDERFONT
    if ((fontnum == fontMenu_fontsel) && (fontnum != screen->menu_font_number)) {
	int old_fontnum = screen->menu_font_number;
#if OPT_TOOLBAR
	SetItemSensitivity(fontMenuEntries[fontnum].widget, True);
#endif
	Bell(xw, XkbBI_MinorError, 0);
	myfonts.f_n = screen->MenuFontName(old_fontnum);
	return xtermLoadFont(xw, &myfonts, doresize, old_fontnum);
    } else if (x_strcasecmp(myfonts.f_n, DEFFONT)) {
	int code;

	myfonts.f_n = DEFFONT;
	TRACE(("...recovering for TrueType fonts\n"));
	code = xtermLoadFont(xw, &myfonts, doresize, fontnum);
	if (code) {
	    if (fontnum != fontMenu_fontsel) {
		SetItemSensitivity(fontMenuEntries[fontnum].widget,
				   UsingRenderFont(xw));
	    }
	    TRACE(("...recovered size %dx%d\n",
		   FontHeight(screen),
		   FontWidth(screen)));
	}
	return code;
    }
#endif

    releaseWindowGCs(xw, win);

    xtermCloseFonts(xw, fnts);
    TRACE(("Fail Cgs - xtermLoadFont\n"));
    return 0;
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

    if (!screen->ifnts_ok) {
	int n;

	screen->ifnts_ok = True;
	for (n = 0; n < fMAX; ++n) {
	    FontNameProperties *fp;

	    /*
	     * FIXME - need to handle font-leaks
	     */
	    screen->ifnts[n].fs = 0;
	    if (screen->fnts[n].fs != 0 &&
		(fp = get_font_name_props(screen->display,
					  screen->fnts[n].fs,
					  0)) != 0) {
		char *name;

		if ((name = italic_font_name(fp, fp->average_width)) != 0) {
		    TRACE(("xtermLoadItalics #%d %s\n", n, name));
		    (void) xtermOpenFont(xw,
					 name,
					 &(screen->ifnts[n]),
					 fwResource,
					 False);
#if OPT_TRACE
		    {
			XFontStruct *fs =
			screen->ifnts[n].fs;
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
		    free(name);
		}
	    }
	}
    }
}
#endif

#if OPT_LOAD_VTFONTS || OPT_WIDE_CHARS
/*
 * Collect font-names that we can modify with the load-vt-fonts() action.
 */
#define MERGE_SUBFONT(src,dst,name) \
	if (IsEmpty(dst.name)) { \
	    TRACE(("MERGE_SUBFONT " #dst "." #name " merge %s\n", NonNull(src.name))); \
	    dst.name = x_strdup(src.name); \
	} else { \
	    TRACE(("MERGE_SUBFONT " #dst "." #name " found %s\n", NonNull(dst.name))); \
	}

#define INFER_SUBFONT(src,dst,name) \
	if (IsEmpty(dst.name)) { \
	    TRACE(("INFER_SUBFONT " #dst "." #name " will infer\n")); \
	    dst.name = x_strdup(""); \
	} else { \
	    TRACE(("INFER_SUBFONT " #dst "." #name " found %s\n", NonNull(dst.name))); \
	}

#define FREE_MENU_FONTS(dst) \
	TRACE(("FREE_MENU_FONTS " #dst "\n")); \
	for (n = fontMenu_default; n <= fontMenu_lastBuiltin; ++n) { \
	    for (m = 0; m < fMAX; ++m) { \
		FREE_STRING(dst.menu_font_names[n][m]); \
		dst.menu_font_names[n][m] = 0; \
	    } \
	}

#define COPY_MENU_FONTS(src,dst) \
	TRACE(("COPY_MENU_FONTS " #src " to " #dst "\n")); \
	for (n = fontMenu_default; n <= fontMenu_lastBuiltin; ++n) { \
	    for (m = 0; m < fMAX; ++m) { \
		FREE_STRING(dst.menu_font_names[n][m]); \
		dst.menu_font_names[n][m] = x_strdup(src.menu_font_names[n][m]); \
	    } \
	    TRACE((".. " #dst ".menu_fonts_names[%d] = %s\n", n, NonNull(dst.menu_font_names[n][fNorm]))); \
	}

#define COPY_DEFAULT_FONTS(target, source) \
	xtermCopyVTFontNames(&target.default_font, &source.default_font)

static void
xtermCopyVTFontNames(VTFontNames * target, VTFontNames * source)
{
    target->f_n = x_strdup(source->f_n);
    target->f_b = x_strdup(source->f_b);
#if OPT_WIDE_CHARS
    target->f_w = x_strdup(source->f_w);
    target->f_wb = x_strdup(source->f_wb);
#endif
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
	COPY_MENU_FONTS(xw->screen, screen->cacheVTFonts);
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
	FREE_MENU_FONTS(xw->screen);
	COPY_MENU_FONTS(screen->cacheVTFonts, xw->screen);
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
	     * If a particular resource value was not found, use the original.
	     */
	    MERGE_SUBFONT(xw->misc, subresourceRec, default_font.f_n);
	    INFER_SUBFONT(xw->misc, subresourceRec, default_font.f_b);
#if OPT_WIDE_CHARS
	    INFER_SUBFONT(xw->misc, subresourceRec, default_font.f_w);
	    INFER_SUBFONT(xw->misc, subresourceRec, default_font.f_wb);
#endif
	    for (n = fontMenu_font1; n <= fontMenu_lastBuiltin; ++n) {
		MERGE_SUBFONT(xw->screen, subresourceRec, MenuFontName(n));
	    }

	    /*
	     * Finally, copy the subresource data to the widget.
	     */
	    COPY_DEFAULT_FONTS(xw->misc, subresourceRec);
	    FREE_MENU_FONTS(xw->screen);
	    COPY_MENU_FONTS(subresourceRec, xw->screen);

	    FREE_STRING(screen->MenuFontName(fontMenu_default));
	    FREE_STRING(screen->menu_font_names[0][fBold]);
	    screen->MenuFontName(fontMenu_default) = x_strdup(xw->misc.default_font.f_n);
	    screen->menu_font_names[0][fBold] = x_strdup(xw->misc.default_font.f_b);
#if OPT_WIDE_CHARS
	    FREE_STRING(screen->menu_font_names[0][fWide]);
	    FREE_STRING(screen->menu_font_names[0][fWBold]);
	    screen->menu_font_names[0][fWide] = x_strdup(xw->misc.default_font.f_w);
	    screen->menu_font_names[0][fWBold] = x_strdup(xw->misc.default_font.f_wb);
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

    if (EmptyFont(screen->fnts[fWide].fs)) {
	result = (isWideFont(screen->fnts[fNorm].fs, "normal", nullOk)
		  && isWideFont(screen->fnts[fBold].fs, "bold", nullOk));
    } else {
	result = (isWideFont(screen->fnts[fWide].fs, "wide", nullOk)
		  && isWideFont(screen->fnts[fWBold].fs, "wide-bold", nullOk));
	if (result && !screen->utf8_latin1) {
	    result = (isWideFont(screen->fnts[fNorm].fs, "normal", nullOk)
		      && isWideFont(screen->fnts[fBold].fs, "bold", nullOk));
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
		  String *params GCC_UNUSED,
		  Cardinal *param_count GCC_UNUSED)
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
		    SetVTFont(xw, font_number, True,
			      ((font_number == fontMenu_default)
			       ? &(xw->misc.default_font)
			       : NULL));
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
    int ww = isCursorBar(screen) ? 1 : fw;
    int hh = isCursorUnderline(screen) ? 1 : fh;

    vp = &VTbox[1];
    (vp++)->x = (short) ww;
    (vp++)->y = (short) hh;
    (vp++)->x = (short) -ww;
    vp->y = (short) -hh;

    screen->box = VTbox;
}

#define CACHE_XFT(dst,src) if (src != 0) {\
	    checkXft(xw, &(dst[fontnum]), src);\
	    TRACE(("Xft metrics %s[%d] = %d (%d,%d)%s advance %d, actual %d%s\n",\
		#dst,\
	    	fontnum,\
		src->height,\
		src->ascent,\
		src->descent,\
		((src->ascent + src->descent) > src->height ? "*" : ""),\
		src->max_advance_width,\
		dst[fontnum].map.min_width,\
		dst[fontnum].map.mixed ? " mixed" : ""));\
	}

#if OPT_RENDERFONT

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
    FcChar32 this, last, next;
    FcChar32 map[FC_CHARSET_MAP_SIZE];
    int i;
    last = FcCharSetFirstPage(xft->charset, map, &next);
    while ((this = FcCharSetNextPage(xft->charset, map, &next)) != FC_CHARSET_DONE)
	last = this;
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
    return (long) last;
}

#if OPT_TRACE > 1
static void
dumpXft(XtermWidget xw, XTermXftFonts *data)
{
    XftFont *xft = data->font;
    TScreen *screen = TScreenOf(xw);
    VTwin *win = WhichVWin(screen);

    FcChar32 c;
    FcChar32 first = xtermXftFirstChar(xft);
    FcChar32 last = xtermXftLastChar(xft);
    unsigned count = 0;
    unsigned outside = 0;

    TRACE(("dumpXft {{\n"));
    TRACE(("   data range %#6x..%#6x\n", first, last));
    for (c = first; c <= last; ++c) {
	if (FcCharSetHasChar(xft->charset, c)) {
	    int width = my_wcwidth((int) c);
	    XGlyphInfo extents;

	    XftTextExtents32(XtDisplay(xw), xft, &c, 1, &extents);
	    TRACE(("%#6x  %2d  %.1f\n", c, width,
		   ((double) extents.width) / win->f_width));
	    if (extents.width > win->f_width)
		++outside;
	    ++count;
	}
    }
    TRACE(("}} %u total, %u outside\n", count, outside));
}
#define DUMP_XFT(xw, data) dumpXft(xw, data)
#else
#define DUMP_XFT(xw, data)	/* nothing */
#endif

static void
checkXft(XtermWidget xw, XTermXftFonts *data, XftFont *xft)
{
    FcChar32 c;
    Dimension width = 0;

    data->font = xft;
    data->map.min_width = 0;
    data->map.max_width = (Dimension) xft->max_advance_width;

    /*
     * For each ASCII or ISO-8859-1 printable code, ask what its width is.
     * Given the maximum width for those, we have a reasonable estimate of
     * the single-column width.
     *
     * Ignore control characters - their extent information is misleading.
     */
    for (c = 32; c < 256; ++c) {
	if (c >= 127 && c <= 159)
	    continue;
	if (FcCharSetHasChar(xft->charset, c)) {
	    XGlyphInfo extents;

	    XftTextExtents32(XtDisplay(xw), xft, &c, 1, &extents);
	    if (width < extents.width && extents.width <= data->map.max_width) {
		width = extents.width;
	    }
	}
    }
    data->map.min_width = width;
    data->map.mixed = (data->map.max_width >= (data->map.min_width + 1));
}

static void
reportXftFonts(XtermWidget xw,
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

	printf("Loaded XftFonts(%s[%s])\n", name, tag);

	for (ch = first_char; ch <= last_char; ++ch) {
	    if (xtermXftMissing(xw, fp, ch)) {
		++missing;
	    }
	}
	printf("\t\tfirst char:    %u\n", first_char);
	printf("\t\tlast char:     %u\n", last_char);
	printf("\t\tmissing-chars: %u\n", missing);
	printf("\t\tpresent-chars: %u\n", (last_char - first_char) + 1 - missing);

	if (XftNameUnparse(match, buffer, (int) sizeof(buffer))) {
	    char *target;
	    char *source = buffer;
	    while ((target = strtok(source, ":")) != 0) {
		printf("\t%s\n", target);
		source = 0;
	    }
	}
    }
}

static XftFont *
xtermOpenXft(XtermWidget xw, const char *name, XftPattern *pat, const char *tag)
{
    TScreen *screen = TScreenOf(xw);
    Display *dpy = screen->display;
    XftResult status;
    XftFont *result = 0;

    if (pat != 0) {
	XftPattern *match = XftFontMatch(dpy, DefaultScreen(dpy), pat, &status);
	if (match != 0) {
	    result = XftFontOpenPattern(dpy, match);
	    if (result != 0) {
		TRACE(("...matched %s font\n", tag));
		reportXftFonts(xw, result, name, tag, match);
	    } else {
		TRACE(("...could did not open %s font\n", tag));
		XftPatternDestroy(match);
		if (xw->misc.fontWarnings >= fwAlways) {
		    TRACE(("OOPS cannot open %s font \"%s\"\n", tag, name));
		    xtermWarning("cannot open %s font \"%s\"\n", tag, name);
		}
	    }
	} else {
	    TRACE(("...did not match %s font\n", tag));
	    if (xw->misc.fontWarnings >= fwResource) {
		TRACE(("OOPS: cannot match %s font \"%s\"\n", tag, name));
		xtermWarning("cannot match %s font \"%s\"\n", tag, name);
	    }
	}
    }
    return result;
}
#endif

#if OPT_RENDERFONT
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

/*
 * Given the Xft font metrics, determine the actual font size.  This is used
 * for each font to ensure that normal, bold and italic fonts follow the same
 * rule.
 */
static void
setRenderFontsize(TScreen *screen, VTwin *win, XftFont *font, const char *tag)
{
    if (font != 0) {
	int width, height, ascent, descent;

	(void) screen;

	width = font->max_advance_width;
	height = font->height;
	ascent = font->ascent;
	descent = font->descent;
	if (height < ascent + descent) {
	    TRACE(("...increase height from %d\n", height));
	    height = ascent + descent;
	}
	if (is_double_width_font_xft(screen->display, font)) {
	    TRACE(("...reduced width from %d\n", width));
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
    }
}
#endif

static void
checkFontInfo(int value, const char *tag)
{
    if (value == 0) {
	xtermWarning("Selected font has no non-zero %s for ISO-8859-1 encoding\n", tag);
	exit(1);
    }
}

#if OPT_RENDERFONT
void
xtermCloseXft(TScreen *screen, XTermXftFonts *pub)
{
    if (pub->font != 0) {
	XftFontClose(screen->display, pub->font);
	pub->font = 0;
    }
}

/*
 * Get the faceName/faceDoublesize resource setting.  Strip off "xft:", which
 * is not recognized by XftNameParse().
 */
String
getFaceName(XtermWidget xw, Bool wideName GCC_UNUSED)
{
#if OPT_RENDERWIDE
    String result = (wideName
		     ? xw->misc.face_wide_name
		     : xw->misc.face_name);
#else
    String result = xw->misc.face_name;
#endif
    if (!IsEmpty(result) && !strncmp(result, "xft:", (size_t) 4))
	result += 4;
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
    Boolean changed = (Boolean) ((xw->misc.face_name == 0)
				 || strcmp(xw->misc.face_name, value));

    if (changed) {
	int n;

	xw->misc.face_name = x_strdup(value);
	for (n = 0; n < NMENUFONTS; ++n) {
	    xw->misc.face_size[n] = -1.0;
	    xtermCloseXft(screen, &(screen->renderFontNorm[n]));
	    xtermCloseXft(screen, &(screen->renderFontBold[n]));
	    xtermCloseXft(screen, &(screen->renderFontBold[n]));
#if OPT_RENDERWIDE
	    xtermCloseXft(screen, &(screen->renderWideNorm[n]));
	    xtermCloseXft(screen, &(screen->renderWideBold[n]));
	    xtermCloseXft(screen, &(screen->renderWideItal[n]));
#endif
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
	XftFont *norm = screen->renderFontNorm[fontnum].font;
	XftFont *bold = screen->renderFontBold[fontnum].font;
	XftFont *ital = screen->renderFontItal[fontnum].font;
#if OPT_RENDERWIDE
	XftFont *wnorm = screen->renderWideNorm[fontnum].font;
	XftFont *wbold = screen->renderWideBold[fontnum].font;
	XftFont *wital = screen->renderWideItal[fontnum].font;
#endif

	if (norm == 0 && face_name) {
	    XftPattern *pat;
	    double face_size;

	    TRACE(("xtermComputeFontInfo font %d: norm(face %s, size %.1f)\n",
		   fontnum, face_name,
		   xw->misc.face_size[fontnum]));

	    fillInFaceSize(xw, fontnum);
	    face_size = xw->misc.face_size[fontnum];

	    /*
	     * By observation (there is no documentation), XftPatternBuild is
	     * cumulative.  Build the bold- and italic-patterns on top of the
	     * normal pattern.
	     */
#define NormXftPattern \
	    XFT_FAMILY, XftTypeString, "mono", \
	    XFT_SIZE, XftTypeDouble, face_size, \
	    XFT_SPACING, XftTypeInteger, XFT_MONO

#define BoldXftPattern(norm) \
	    XFT_WEIGHT, XftTypeInteger, XFT_WEIGHT_BOLD, \
	    XFT_CHAR_WIDTH, XftTypeInteger, norm->max_advance_width

#define ItalXftPattern(norm) \
	    XFT_SLANT, XftTypeInteger, XFT_SLANT_ITALIC, \
	    XFT_CHAR_WIDTH, XftTypeInteger, norm->max_advance_width

#if OPT_WIDE_ATTRS
#define HAVE_ITALICS 1
#define FIND_ITALICS ((pat = XftNameParse(face_name)) != 0)
#elif OPT_ISO_COLORS
#define HAVE_ITALICS 1
#define FIND_ITALICS (screen->italicULMode && (pat = XftNameParse(face_name)) != 0)
#else
#define HAVE_ITALICS 0
#endif

	    if ((pat = XftNameParse(face_name)) != 0) {
#define OPEN_XFT(tag) xtermOpenXft(xw, face_name, pat, tag)
		XftPatternBuild(pat,
				NormXftPattern,
				(void *) 0);
		norm = OPEN_XFT("normal");

		if (norm != 0) {
		    XftPatternBuild(pat,
				    BoldXftPattern(norm),
				    (void *) 0);
		    bold = OPEN_XFT("bold");

#if HAVE_ITALICS
		    if (FIND_ITALICS) {
			XftPatternBuild(pat,
					NormXftPattern,
					ItalXftPattern(norm),
					(void *) 0);
			ital = OPEN_XFT("italic");
		    }
#endif
#undef OPEN_XFT

		    /*
		     * FIXME:  just assume that the corresponding font has no
		     * graphics characters.
		     */
		    if (screen->fnt_boxes) {
			screen->fnt_boxes = False;
			TRACE(("Xft opened - will %suse internal line-drawing characters\n",
			       screen->fnt_boxes ? "not " : ""));
		    }
		}

		XftPatternDestroy(pat);
	    }

	    CACHE_XFT(screen->renderFontNorm, norm);
	    CACHE_XFT(screen->renderFontBold, bold);
	    CACHE_XFT(screen->renderFontItal, ital);

	    /*
	     * See xtermXftDrawString().
	     */
#if OPT_RENDERWIDE
	    if (norm != 0 && screen->wide_chars) {
		int char_width = norm->max_advance_width * 2;
#ifdef FC_ASPECT
		double aspect = ((xw->misc.face_wide_name
				  || screen->renderFontNorm[fontnum].map.mixed)
				 ? 1.0
				 : 2.0);
#endif

		face_name = getFaceName(xw, True);
		TRACE(("xtermComputeFontInfo wide(face %s, char_width %d)\n",
		       NonNull(face_name),
		       char_width));

#define WideXftPattern \
		XFT_FAMILY, XftTypeString, "mono", \
		XFT_SIZE, XftTypeDouble, face_size, \
		XFT_SPACING, XftTypeInteger, XFT_MONO

		if (face_name && (pat = XftNameParse(face_name)) != 0) {
#define OPEN_XFT(tag) xtermOpenXft(xw, face_name, pat, tag)
		    XftPatternBuild(pat,
				    WideXftPattern,
				    XFT_CHAR_WIDTH, XftTypeInteger, char_width,
#ifdef FC_ASPECT
				    FC_ASPECT, XftTypeDouble, aspect,
#endif
				    (void *) 0);
		    wnorm = OPEN_XFT("wide");

		    if (wnorm != 0) {
			XftPatternBuild(pat,
					WideXftPattern,
					BoldXftPattern(wnorm),
					(void *) 0);
			wbold = OPEN_XFT("wide-bold");

#if HAVE_ITALICS
			if (FIND_ITALICS) {
			    XftPatternBuild(pat,
					    WideXftPattern,
					    ItalXftPattern(wnorm),
					    (void *) 0);
			    wital = OPEN_XFT("wide-italic");
			}
#endif
#undef OPEN_XFT
		    }
		    XftPatternDestroy(pat);
		}

		CACHE_XFT(screen->renderWideNorm, wnorm);
		CACHE_XFT(screen->renderWideBold, wbold);
		CACHE_XFT(screen->renderWideItal, wital);
	    }
#endif /* OPT_RENDERWIDE */
	}
	if (norm == 0) {
	    TRACE(("...no TrueType font found for number %d, disable menu entry\n", fontnum));
	    xw->work.render_font = False;
	    update_font_renderfont();
	    /* now we will fall through into the bitmap fonts */
	} else {
	    setRenderFontsize(screen, win, norm, NULL);
	    setRenderFontsize(screen, win, bold, "bold");
	    setRenderFontsize(screen, win, ital, "ital");
#if OPT_BOX_CHARS
	    setupPackedFonts(xw);

	    if (screen->force_packed) {
		XTermXftFonts *use = &(screen->renderFontNorm[fontnum]);
		SetFontHeight(screen, win, use->font->ascent + use->font->descent);
		SetFontWidth(screen, win, use->map.min_width);
		TRACE(("...packed TrueType font %dx%d vs %d\n",
		       win->f_height,
		       win->f_width,
		       use->map.max_width));
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

    checkFontInfo(win->f_height, "height");
    checkFontInfo(win->f_width, "width");
}

/* save this information as a side-effect for double-sized characters */
void
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

    scrollbar_width = (xw->misc.scrollbar
		       ? (screen->scrollWidget->core.width +
			  BorderWidth(screen->scrollWidget))
		       : 0);
    xtermComputeFontInfo(xw, win, screen->fnts[fNorm].fs, scrollbar_width);
    xtermSaveFontInfo(screen, screen->fnts[fNorm].fs);

    if (doresize) {
	if (VWindow(screen)) {
	    xtermClear(xw);
	}
	TRACE(("xtermUpdateFontInfo {{\n"));
	DoResizeScreen(xw);	/* set to the new natural size */
	ResizeScrollBar(xw);
	Redraw();
	TRACE(("... }} xtermUpdateFontInfo\n"));
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

    if (fs->max_byte1 == 0) {
#if OPT_WIDE_CHARS
	if (ch < 256)
#endif
	{
	    CI_GET_CHAR_INFO_1D(fs, E2A(ch), pc);
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
	TRACE(("xtermMissingChar %#04x (!exists), %d cells\n",
	       ch, my_wcwidth(ch)));
	result = True;
    }
    if (ch < KNOWN_MISSING) {
	font->known_missing[ch] = (Char) (result ? 2 : 1);
    }
    return result;
}
#endif

#if OPT_BOX_CHARS
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
#define SCALED_X(n) ((int)(n) * (((int) font_width) - 1)) / (BOX_WIDE-1)
#define SCALED_Y(n) ((int)(n) * (((int) font_height) - 1)) / (BOX_HIGH-1)
#define SCALE_X(n) n = SCALED_X(n)
#define SCALE_Y(n) n = SCALED_Y(n)

#define SEG(x0,y0,x1,y1) x0,y0, x1,y1

/*
 * Draw the given graphic character, if it is simple enough (i.e., a
 * line-drawing character).
 */
void
xtermDrawBoxChar(XtermWidget xw,
		 unsigned ch,
		 unsigned attr_flags,
		 unsigned draw_flags,
		 GC gc,
		 int x,
		 int y,
		 int cells)
{
    TScreen *screen = TScreenOf(xw);
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
    };
    /* *INDENT-ON* */

    static const short *lines[] =
    {
	0,			/* 00 (unused) */
	0,			/* 01 diamond */
	0,			/* 02 box */
	glyph_ht,		/* 03 HT */
	glyph_ff,		/* 04 FF */
	0,			/* 05 CR */
	glyph_lf,		/* 06 LF */
	0,			/* 07 degrees (small circle) */
	plus_or_minus,		/* 08 */
	glyph_nl,		/* 09 */
	glyph_vt,		/* 0A */
	lower_right_corner,	/* 0B */
	upper_right_corner,	/* 0C */
	upper_left_corner,	/* 0D */
	lower_left_corner,	/* 0E */
	cross,			/* 0F */
	scan_line_1,		/* 10 */
	scan_line_3,		/* 11 */
	scan_line_7,		/* 12 */
	scan_line_9,		/* 13 */
	horizontal_line,	/* 14 */
	left_tee,		/* 15 */
	right_tee,		/* 16 */
	bottom_tee,		/* 17 */
	top_tee,		/* 18 */
	vertical_line,		/* 19 */
	less_than_or_equal,	/* 1A */
	greater_than_or_equal,	/* 1B */
	greek_pi,		/* 1C */
	not_equal_to,		/* 1D */
	0,			/* 1E LB */
	0,			/* 1F bullet */
    };

    GC gc2;
    CgsEnum cgsId = (ch == 2) ? gcDots : gcLine;
    VTwin *cgsWin = WhichVWin(screen);
    const short *p;
    unsigned font_width = (unsigned) (((draw_flags & DOUBLEWFONT) ? 2 : 1)
				      * screen->fnt_wide);
    unsigned font_height = (unsigned) (((draw_flags & DOUBLEHFONT) ? 2 : 1)
				       * screen->fnt_high);

    if (cells > 1)
	font_width *= (unsigned) cells;

#if OPT_WIDE_CHARS
    /*
     * Try to show line-drawing characters if we happen to be in UTF-8
     * mode, but have gotten an old-style font.
     */
    if (screen->utf8_mode
#if OPT_RENDERFONT
	&& !UsingRenderFont(xw)
#endif
	&& (ch > 127)
	&& (ch != UCS_REPL)) {
	unsigned n;
	for (n = 1; n < 32; n++) {
	    if (dec2ucs(n) == ch
		&& !((attr_flags & BOLD)
		     ? IsXtermMissingChar(screen, n, &screen->fnts[fBold])
		     : IsXtermMissingChar(screen, n, &screen->fnts[fNorm]))) {
		TRACE(("...use xterm-style linedrawing\n"));
		ch = n;
		break;
	    }
	}
    }
#endif

    TRACE(("DRAW_BOX(%d) cell %dx%d at %d,%d%s\n",
	   ch, font_height, font_width, y, x,
	   (ch >= (sizeof(lines) / sizeof(lines[0]))
	    ? "-BAD"
	    : "")));

    if (cgsId == gcDots) {
	setCgsFont(xw, cgsWin, cgsId, getCgsFont(xw, cgsWin, gc));
	setCgsFore(xw, cgsWin, cgsId, getCgsFore(xw, cgsWin, gc));
	setCgsBack(xw, cgsWin, cgsId, getCgsBack(xw, cgsWin, gc));
    } else {
	setCgsFont(xw, cgsWin, cgsId, getCgsFont(xw, cgsWin, gc));
	setCgsFore(xw, cgsWin, cgsId, getCgsBack(xw, cgsWin, gc));
	setCgsBack(xw, cgsWin, cgsId, getCgsBack(xw, cgsWin, gc));
    }
    gc2 = getCgsGC(xw, cgsWin, cgsId);

    if (!(draw_flags & NOBACKGROUND)) {
	XFillRectangle(screen->display, VDrawable(screen), gc2, x, y,
		       font_width,
		       font_height);
    }

    setCgsFont(xw, cgsWin, cgsId, getCgsFont(xw, cgsWin, gc));
    setCgsFore(xw, cgsWin, cgsId, getCgsFore(xw, cgsWin, gc));
    setCgsBack(xw, cgsWin, cgsId, getCgsBack(xw, cgsWin, gc));
    gc2 = getCgsGC(xw, cgsWin, cgsId);

    XSetLineAttributes(screen->display, gc2,
		       (attr_flags & BOLD)
		       ? ((font_height > 12)
			  ? font_height / 12
			  : 1)
		       : ((font_height > 16)
			  ? font_height / 16
			  : 1),
		       LineSolid,
		       CapProjecting,
		       JoinMiter);

    if (ch == 1) {		/* diamond */
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
	    points[n].x = (short) SCALED_X(points[n].x);
	    points[n].y = (short) SCALED_Y(points[n].y);
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
    } else if (ch < (sizeof(lines) / sizeof(lines[0]))
	       && (p = lines[ch]) != 0) {
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
	XDrawRectangle(screen->display, VDrawable(screen), gc2, x, y,
		       font_width - 1,
		       font_height - 1);
    }
}
#endif /* OPT_BOX_CHARS */

#if OPT_RENDERFONT

/*
 * Check if the given character has a glyph known to Xft.
 *
 * see xc/lib/Xft/xftglyphs.c
 */
Bool
xtermXftMissing(XtermWidget xw, XftFont *font, unsigned wc)
{
    Bool result = False;

    if (font != 0) {
	TScreen *screen = TScreenOf(xw);
	if (!XftGlyphExists(screen->display, font, wc)) {
#if OPT_WIDE_CHARS
	    TRACE(("xtermXftMissing %d (dec=%#x, ucs=%#x)\n",
		   wc, ucs2dec(wc), dec2ucs(wc)));
#else
	    TRACE(("xtermXftMissing %d\n", wc));
#endif
	    result = True;
	}
    }
    return result;
}
#endif /* OPT_RENDERFONT */

#if OPT_WIDE_CHARS
#define MY_UCS(ucs,dec) case ucs: result = dec; break
unsigned
ucs2dec(unsigned ch)
{
    unsigned result = ch;
    if ((ch > 127)
	&& (ch != UCS_REPL)) {
	switch (ch) {
	    MY_UCS(0x25ae, 0);	/* black vertical rectangle                   */
	    MY_UCS(0x25c6, 1);	/* black diamond                              */
	    MY_UCS(0x2592, 2);	/* medium shade                               */
	    MY_UCS(0x2409, 3);	/* symbol for horizontal tabulation           */
	    MY_UCS(0x240c, 4);	/* symbol for form feed                       */
	    MY_UCS(0x240d, 5);	/* symbol for carriage return                 */
	    MY_UCS(0x240a, 6);	/* symbol for line feed                       */
	    MY_UCS(0x00b0, 7);	/* degree sign                                */
	    MY_UCS(0x00b1, 8);	/* plus-minus sign                            */
	    MY_UCS(0x2424, 9);	/* symbol for newline                         */
	    MY_UCS(0x240b, 10);	/* symbol for vertical tabulation             */
	    MY_UCS(0x2518, 11);	/* box drawings light up and left             */
	    MY_UCS(0x2510, 12);	/* box drawings light down and left           */
	    MY_UCS(0x250c, 13);	/* box drawings light down and right          */
	    MY_UCS(0x2514, 14);	/* box drawings light up and right            */
	    MY_UCS(0x253c, 15);	/* box drawings light vertical and horizontal */
	    MY_UCS(0x23ba, 16);	/* box drawings scan 1                        */
	    MY_UCS(0x23bb, 17);	/* box drawings scan 3                        */
	    MY_UCS(0x2500, 18);	/* box drawings light horizontal              */
	    MY_UCS(0x23bc, 19);	/* box drawings scan 7                        */
	    MY_UCS(0x23bd, 20);	/* box drawings scan 9                        */
	    MY_UCS(0x251c, 21);	/* box drawings light vertical and right      */
	    MY_UCS(0x2524, 22);	/* box drawings light vertical and left       */
	    MY_UCS(0x2534, 23);	/* box drawings light up and horizontal       */
	    MY_UCS(0x252c, 24);	/* box drawings light down and horizontal     */
	    MY_UCS(0x2502, 25);	/* box drawings light vertical                */
	    MY_UCS(0x2264, 26);	/* less-than or equal to                      */
	    MY_UCS(0x2265, 27);	/* greater-than or equal to                   */
	    MY_UCS(0x03c0, 28);	/* greek small letter pi                      */
	    MY_UCS(0x2260, 29);	/* not equal to                               */
	    MY_UCS(0x00a3, 30);	/* pound sign                                 */
	    MY_UCS(0x00b7, 31);	/* middle dot                                 */
	}
    }
    return result;
}

#undef  MY_UCS
#define MY_UCS(ucs,dec) case dec: result = ucs; break

unsigned
dec2ucs(unsigned ch)
{
    unsigned result = ch;
    if (xtermIsDecGraphic(ch)) {
	switch (ch) {
	    MY_UCS(0x25ae, 0);	/* black vertical rectangle                   */
	    MY_UCS(0x25c6, 1);	/* black diamond                              */
	    MY_UCS(0x2592, 2);	/* medium shade                               */
	    MY_UCS(0x2409, 3);	/* symbol for horizontal tabulation           */
	    MY_UCS(0x240c, 4);	/* symbol for form feed                       */
	    MY_UCS(0x240d, 5);	/* symbol for carriage return                 */
	    MY_UCS(0x240a, 6);	/* symbol for line feed                       */
	    MY_UCS(0x00b0, 7);	/* degree sign                                */
	    MY_UCS(0x00b1, 8);	/* plus-minus sign                            */
	    MY_UCS(0x2424, 9);	/* symbol for newline                         */
	    MY_UCS(0x240b, 10);	/* symbol for vertical tabulation             */
	    MY_UCS(0x2518, 11);	/* box drawings light up and left             */
	    MY_UCS(0x2510, 12);	/* box drawings light down and left           */
	    MY_UCS(0x250c, 13);	/* box drawings light down and right          */
	    MY_UCS(0x2514, 14);	/* box drawings light up and right            */
	    MY_UCS(0x253c, 15);	/* box drawings light vertical and horizontal */
	    MY_UCS(0x23ba, 16);	/* box drawings scan 1                        */
	    MY_UCS(0x23bb, 17);	/* box drawings scan 3                        */
	    MY_UCS(0x2500, 18);	/* box drawings light horizontal              */
	    MY_UCS(0x23bc, 19);	/* box drawings scan 7                        */
	    MY_UCS(0x23bd, 20);	/* box drawings scan 9                        */
	    MY_UCS(0x251c, 21);	/* box drawings light vertical and right      */
	    MY_UCS(0x2524, 22);	/* box drawings light vertical and left       */
	    MY_UCS(0x2534, 23);	/* box drawings light up and horizontal       */
	    MY_UCS(0x252c, 24);	/* box drawings light down and horizontal     */
	    MY_UCS(0x2502, 25);	/* box drawings light vertical                */
	    MY_UCS(0x2264, 26);	/* less-than or equal to                      */
	    MY_UCS(0x2265, 27);	/* greater-than or equal to                   */
	    MY_UCS(0x03c0, 28);	/* greek small letter pi                      */
	    MY_UCS(0x2260, 29);	/* not equal to                               */
	    MY_UCS(0x00a3, 30);	/* pound sign                                 */
	    MY_UCS(0x00b7, 31);	/* middle dot                                 */
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
	if (xtermOpenFont(xw,
			  screen->MenuFontName(fontnum),
			  &fnt,
			  ((fontnum <= fontMenu_lastBuiltin)
			   ? fwAlways
			   : fwResource),
			  True)) {
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
	result = value;
    else
	result = 14.0;
    return result;
}

static void
fillInFaceSize(XtermWidget xw, int fontnum)
{
    TScreen *screen = TScreenOf(xw);
    double face_size = xw->misc.face_size[fontnum];

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

	    face_size = (ratio * xw->misc.face_size[0]);
	    TRACE(("scaled[%d] using %3ld/%ld = %.2f -> %f\n",
		   fontnum, num, den, ratio, face_size));
	} else
#endif
	{
#define LikeBitmap(s) (((s) / 78.0) * xw->misc.face_size[fontMenu_default])
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

    TRACE(("useFaceSizes {{\n"));
    if (UsingRenderFont(xw)) {
	Boolean nonzero = True;
	int n;

	for (n = 0; n < NMENU_RENDERFONTS; ++n) {
	    if (xw->misc.face_size[n] <= 0.0) {
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
    TRACE(("...}}useFaceSizes %d\n", result));
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
HandleLargerFont(Widget w GCC_UNUSED,
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
HandleSmallerFont(Widget w GCC_UNUSED,
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
    return fontnum;
}

/* ARGSUSED */
void
HandleSetFont(Widget w GCC_UNUSED,
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
		fonts.f_wb = params[4];
		/* FALLTHRU */
	    case 4:
		fonts.f_w = params[3];
		/* FALLTHRU */
#endif
	    case 3:
		fonts.f_b = params[2];
		/* FALLTHRU */
	    case 2:
		fonts.f_n = params[1];
		break;
	    }
	}

	SetVTFont(xw, fontnum, True, &fonts);
    }
}

void
SetVTFont(XtermWidget xw,
	  int which,
	  Bool doresize,
	  const VTFontNames * fonts)
{
    TScreen *screen = TScreenOf(xw);

    TRACE(("SetVTFont(which=%d, f_n=%s, f_b=%s)\n", which,
	   (fonts && fonts->f_n) ? fonts->f_n : "<null>",
	   (fonts && fonts->f_b) ? fonts->f_b : "<null>"));

    if (IsIcon(screen)) {
	Bell(xw, XkbBI_MinorError, 0);
    } else if (which >= 0 && which < NMENUFONTS) {
	VTFontNames myfonts;

	memset(&myfonts, 0, sizeof(myfonts));
	if (fonts != 0)
	    myfonts = *fonts;

	if (which == fontMenu_fontsel) {	/* go get the selection */
	    FindFontSelection(xw, myfonts.f_n, False);
	} else {
	    int oldFont = screen->menu_font_number;

#define USE_CACHED(field, name) \
	    if (myfonts.field == 0) { \
		myfonts.field = x_strdup(screen->menu_font_names[which][name]); \
		TRACE(("set myfonts." #field " from menu_font_names[%d][" #name "] %s\n", \
		       which, NonNull(myfonts.field))); \
	    } else { \
		TRACE(("set myfonts." #field " reused\n")); \
	    }
#define SAVE_FNAME(field, name) \
	    if (myfonts.field != 0) { \
		if (screen->menu_font_names[which][name] == 0 \
		 || strcmp(screen->menu_font_names[which][name], myfonts.field)) { \
		    TRACE(("updating menu_font_names[%d][" #name "] to %s\n", \
			   which, myfonts.field)); \
		    FREE_STRING(screen->menu_font_names[which][name]); \
		    screen->menu_font_names[which][name] = x_strdup(myfonts.field); \
		} \
	    }

	    USE_CACHED(f_n, fNorm);
	    USE_CACHED(f_b, fBold);
#if OPT_WIDE_CHARS
	    USE_CACHED(f_w, fWide);
	    USE_CACHED(f_wb, fWBold);
#endif
	    if (xtermLoadFont(xw,
			      &myfonts,
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
	    } else {
		(void) xtermLoadFont(xw,
				     xtermFontName(screen->MenuFontName(oldFont)),
				     doresize, oldFont);
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
    return;
}
