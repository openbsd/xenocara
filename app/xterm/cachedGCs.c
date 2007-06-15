/* $XTermId: cachedGCs.c,v 1.35 2007/03/21 23:21:50 tom Exp $ */

/************************************************************

Copyright 2007 by Thomas E. Dickey

                        All Rights Reserved

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name(s) of the above copyright
holders shall not be used in advertising or otherwise to promote the
sale, use or other dealings in this Software without prior written
authorization.

********************************************************/

#include <data.h>
#include <xstrings.h>

#include <X11/Xmu/Drawing.h>

#include <stdio.h>

/*
 * hide (or eliminate) calls to
 *	XCreateGC()
 *	XFreeGC()
 *	XGetGCValues()
 *	XSetBackground()
 *	XSetFont()
 *	XSetForeground()
 *	XtGetGC()
 *	XtReleaseGC()
 * by associating an integer with each GC, maintaining a cache which
 * reflects frequency of use rather than most recent usage.
 *
 * FIXME: XTermFonts should hold gc, font, fs.
 */
typedef struct {
    GC gc;
    unsigned used;
    unsigned cset;
    XFontStruct *font;
    Pixel tile;
    Pixel fg;
    Pixel bg;
} CgsCacheData;

#define DEPTH 8
#define ITEM()      (me->data - me->list)
#define LIST(item)  me->list[item]
#define LINK(item)  me->data = (me->list + (item))
#define THIS(field) me->data->field
#define NEXT(field) me->next.field

#define GC_CSet GCFunction

typedef struct {
    CgsCacheData list[DEPTH];
    CgsCacheData *data;		/* points to current list[] entry */
    XtGCMask mask;		/* changes since the last getCgsGC() */
    CgsCacheData next;		/* updated values, apply in getCgsGC() */
} CgsCache;

#if OPT_TRACE
#define CASE(name) case gc##name: result = #name; break
static String
traceCgsEnum(CgsEnum value)
{
    String result = "?";
    switch (value) {
	CASE(Norm);
	CASE(Bold);
	CASE(NormReverse);
	CASE(BoldReverse);
#if OPT_BOX_CHARS
	CASE(Line);
	CASE(Dots);
#endif
#if OPT_DEC_CHRSET
	CASE(CNorm);
	CASE(CBold);
#endif
#if OPT_WIDE_CHARS
	CASE(Wide);
	CASE(WBold);
	CASE(WideReverse);
	CASE(WBoldReverse);
#endif
	CASE(VTcursNormal);
	CASE(VTcursFilled);
	CASE(VTcursReverse);
	CASE(VTcursOutline);
#if OPT_TEK4014
	CASE(TKcurs);
#endif
	CASE(MAX);
    }
    return result;
}

#undef CASE

static String
traceVTwin(XtermWidget xw, VTwin * value)
{
    String result = "?";
    if (value == 0)
	result = "null";
    else if (value == &(xw->screen.fullVwin))
	result = "fullVwin";
#ifndef NO_ACTIVE_ICON
    else if (value == &(xw->screen.iconVwin))
	result = "iconVwin";
#endif
    return result;
}

#if OPT_TRACE > 1
static String
traceCSet(unsigned cset)
{
    static char result[80];
    switch (cset) {
    case CSET_SWL:
	strcpy(result, "SWL");
	break;
    case CSET_DHL_TOP:
	strcpy(result, "DHL_TOP");
	break;
    case CSET_DHL_BOT:
	strcpy(result, "DHL_BOT");
	break;
    case CSET_DWL:
	strcpy(result, "DWL");
	break;
    default:
	sprintf(result, "%#x", cset);
	break;
    }
    return result;
}

static String
traceFont(XFontStruct * font)
{
    static char result[80];
    if (font != 0) {
	sprintf(result, "%p(%dx%d %d %#lx)",
		font,
		font->max_bounds.width,
		font->max_bounds.ascent + font->max_bounds.descent,
		font->max_bounds.descent,
		(unsigned long) (font->fid));
    } else {
	strcpy(result, "null");
    }
    return result;
}

static String
tracePixel(XtermWidget xw, Pixel value)
{
#define CASE(name) { name, #name }
    static struct {
	TermColors code;
	String name;
    } t_colors[] = {
	CASE(TEXT_FG),
	    CASE(TEXT_BG),
	    CASE(TEXT_CURSOR),
	    CASE(MOUSE_FG),
	    CASE(MOUSE_BG),
#if OPT_TEK4014
	    CASE(TEK_FG),
	    CASE(TEK_BG),
#endif
#if OPT_HIGHLIGHT_COLOR
	    CASE(HIGHLIGHT_BG),
	    CASE(HIGHLIGHT_FG),
#endif
#if OPT_TEK4014
	    CASE(TEK_CURSOR),
#endif
    };
    TScreen *screen = &(xw->screen);
    String result = 0;
    int n;

    for (n = 0; n < NCOLORS; ++n) {
	if (value == T_COLOR(screen, t_colors[n].code)) {
	    result = t_colors[n].name;
	    break;
	}
    }

    if (result == 0) {
	for (n = 0; n < MAXCOLORS; ++n) {
#if OPT_COLOR_RES
	    if (screen->Acolors[n].mode > 0
		&& value == screen->Acolors[n].value) {
		result = screen->Acolors[n].resource;
		break;
	    }
#else
	    if (value == screen->Acolors[n]) {
		char temp[80];
		sprintf(temp, "Acolors[%d]", n);
		result = x_strdup(temp);
		break;
	    }
#endif
	}
    }

    if (result == 0) {
	char temp[80];
	sprintf(temp, "%#lx", value);
	result = x_strdup(temp);
    }

    return result;
}

#undef CASE

#endif /* OPT_TRACE > 1 */
#endif /* OPT_TRACE */

/*
 * FIXME: move the cache into XtermWidget
 */
static CgsCache *
myCache(XtermWidget xw GCC_UNUSED, VTwin * cgsWin GCC_UNUSED, CgsEnum cgsId)
{
    static CgsCache *main_cache;
    CgsCache *my_cache;
    CgsCache *result = 0;

    if (main_cache == 0)
	main_cache = (CgsCache *) calloc(gcMAX, sizeof(CgsCache));
    my_cache = main_cache;
    if ((int) cgsId >= 0 && cgsId < gcMAX) {
#ifndef NO_ACTIVE_ICON
	static CgsCache icon_cache[gcMAX];
	if (cgsWin == &(xw->screen.iconVwin))
	    my_cache = icon_cache;
#endif
	result = my_cache + cgsId;
	if (result->data == 0) {
	    result->data = result->list;
	}
    }

    return result;
}

static Display *
myDisplay(XtermWidget xw)
{
    return xw->screen.display;
}

static Drawable
myDrawable(XtermWidget xw, VTwin * cgsWin)
{
    Drawable drawable = 0;

    if (cgsWin != 0 && cgsWin->window != 0)
	drawable = cgsWin->window;
    if (drawable == 0)
	drawable = RootWindowOfScreen(XtScreen(xw));
    return drawable;
}

static GC
newCache(XtermWidget xw, VTwin * cgsWin, CgsEnum cgsId, CgsCache * me)
{
    XGCValues xgcv;
    XtGCMask mask;

    THIS(font) = NEXT(font);
    THIS(cset) = NEXT(cset);
    THIS(fg) = NEXT(fg);
    THIS(bg) = NEXT(bg);

    memset(&xgcv, 0, sizeof(xgcv));
    xgcv.font = NEXT(font)->fid;
    mask = (GCForeground | GCBackground | GCFont);

    switch (cgsId) {
    case gcNorm:
    case gcBold:
    case gcNormReverse:
    case gcBoldReverse:
#if OPT_WIDE_CHARS
    case gcWide:
    case gcWBold:
    case gcWideReverse:
    case gcWBoldReverse:
#endif
	mask |= (GCGraphicsExposures | GCFunction);
	xgcv.graphics_exposures = True;		/* default */
	xgcv.function = GXcopy;
	break;
#if OPT_BOX_CHARS
    case gcLine:
	mask |= (GCGraphicsExposures | GCFunction);
	xgcv.graphics_exposures = True;		/* default */
	xgcv.function = GXcopy;
	break;
    case gcDots:
	xgcv.fill_style = FillTiled;
	xgcv.tile =
	    XmuCreateStippledPixmap(XtScreen((Widget) xw),
				    THIS(fg),
				    THIS(bg),
				    xw->core.depth);
	THIS(tile) = xgcv.tile;
	mask = (GCForeground | GCBackground);
	mask |= (GCGraphicsExposures | GCFunction | GCTile | GCFillStyle);
	xgcv.graphics_exposures = True;		/* default */
	xgcv.function = GXcopy;
	break;
#endif
#if OPT_DEC_CHRSET
    case gcCNorm:
    case gcCBold:
	break;
#endif
    case gcVTcursNormal:	/* FALLTHRU */
    case gcVTcursFilled:	/* FALLTHRU */
    case gcVTcursReverse:	/* FALLTHRU */
    case gcVTcursOutline:	/* FALLTHRU */
	break;
#if OPT_TEK4014
    case gcTKcurs:		/* FALLTHRU */
	/* FIXME */
#endif
    case gcMAX:		/* should not happen */
	return 0;
    }
    xgcv.foreground = NEXT(fg);
    xgcv.background = NEXT(bg);

    THIS(gc) = XCreateGC(myDisplay(xw), myDrawable(xw, cgsWin), mask, &xgcv);
    TRACE(("getCgsGC(%s) created gc %p(%d)\n",
	   traceCgsEnum(cgsId), THIS(gc), ITEM()));

    THIS(used) = 0;
    return THIS(gc);
}

static GC
chgCache(XtermWidget xw, CgsEnum cgsId GCC_UNUSED, CgsCache * me)
{
    XGCValues xgcv;
    XtGCMask mask = (GCForeground | GCBackground | GCFont);

    memset(&xgcv, 0, sizeof(xgcv));

    TRACE2(("...Cgs old data fg=%s, bg=%s, font=%s cset %s\n",
	    tracePixel(xw, THIS(fg)),
	    tracePixel(xw, THIS(bg)),
	    traceFont(THIS(font)),
	    traceCSet(THIS(cset))));

    THIS(font) = NEXT(font);
    THIS(cset) = NEXT(cset);
    THIS(fg) = NEXT(fg);
    THIS(bg) = NEXT(bg);

    xgcv.font = THIS(font)->fid;
    xgcv.foreground = THIS(fg);
    xgcv.background = THIS(bg);

    XChangeGC(myDisplay(xw), THIS(gc), mask, &xgcv);
    TRACE(("getCgsGC(%s) updated gc %p(%d)\n",
	   traceCgsEnum(cgsId), THIS(gc), ITEM()));

    THIS(used) = 0;
    return THIS(gc);
}

static Boolean
SameFont(XFontStruct * a, XFontStruct * b)
{
    return ((a != 0) && (b != 0) && (a == b) && !memcmp(a, b, sizeof(*a)));
}

#define SameColor(a,b) ((a) == (b))
#define SameCSet(a,b)  ((a) == (b))

/*
 * Use the "setCgsXXXX()" calls to initialize parameters for a new GC.
 */
void
setCgsFore(XtermWidget xw, VTwin * cgsWin, CgsEnum cgsId, Pixel fg)
{
    CgsCache *me;

    if ((me = myCache(xw, cgsWin, cgsId)) != 0) {
	if (!SameColor(NEXT(fg), fg)) {
	    NEXT(fg) = fg;
	    me->mask |= GCForeground;
	}
    }
}

void
setCgsBack(XtermWidget xw, VTwin * cgsWin, CgsEnum cgsId, Pixel bg)
{
    CgsCache *me;

    if ((me = myCache(xw, cgsWin, cgsId)) != 0) {
	if (!SameColor(NEXT(bg), bg)) {
	    NEXT(bg) = bg;
	    me->mask |= GCBackground;
	}
    }
}

#if OPT_DEC_CHRSET
void
setCgsCSet(XtermWidget xw, VTwin * cgsWin, CgsEnum cgsId, unsigned cset)
{
    CgsCache *me;

    if ((me = myCache(xw, cgsWin, cgsId)) != 0) {
	if (!SameCSet(NEXT(cset), cset)) {
	    NEXT(cset) = cset;
	    me->mask |= GC_CSet;
	}
    }
}
#else
#define setCgsCSet(xw, cgsWin, dstCgsId, cset)	/* nothing */
#endif

void
setCgsFont(XtermWidget xw, VTwin * cgsWin, CgsEnum cgsId, XFontStruct * font)
{
    CgsCache *me;

    if ((me = myCache(xw, cgsWin, cgsId)) != 0) {
	if (font == 0) {
	    if (cgsId != gcNorm)
		(void) getCgsGC(xw, cgsWin, gcNorm);
#ifndef NO_ACTIVE_ICON
	    if (cgsWin == &(xw->screen.iconVwin))
		font = xw->screen.fnt_icon;
	    else
#endif
		font = xw->screen.fnts[fNorm];
	}
	if (okFont(font) && !SameFont(NEXT(font), font)) {
	    TRACE2(("...updated next font for %s to %s\n",
		    traceCgsEnum(cgsId), traceFont(font)));
	    TRACE2(("...next font was %s\n", traceFont(NEXT(font))));
	    NEXT(font) = font;
	    me->mask |= GCFont;
	} else {
	    TRACE2(("...NOT updated font for %s\n",
		    traceCgsEnum(cgsId)));
	}
    }
}

/*
 * Discard all of the font information, e.g., we are resizing the font.
 * Keep the GC's so we can simply change them rather than creating new ones.
 */
void
clrCgsFonts(XtermWidget xw, VTwin * cgsWin, XFontStruct * font)
{
    CgsCache *me;
    int j, k;

    for_each_gc(j) {
	if ((me = myCache(xw, cgsWin, (CgsEnum) j)) != 0) {
	    for (k = 0; k < DEPTH; ++k) {
		if (SameFont(LIST(k).font, font)) {
		    TRACE2(("clrCgsFonts %s gc %p(%d) %s\n",
			    traceCgsEnum((CgsEnum) j),
			    LIST(k).gc,
			    k,
			    traceFont(font)));
		    LIST(k).font = 0;
		    LIST(k).cset = 0;
		}
	    }
	    if (SameFont(NEXT(font), font)) {
		TRACE2(("clrCgsFonts %s next %s\n",
			traceCgsEnum((CgsEnum) j),
			traceFont(font)));
		NEXT(font) = 0;
		NEXT(cset) = 0;
		me->mask &= ~(GCFont | GC_CSet);
	    }
	}
    }
}

/*
 * Return a GC associated with the given id, allocating if needed.
 */
GC
getCgsGC(XtermWidget xw, VTwin * cgsWin, CgsEnum cgsId)
{
    CgsCache *me;
    GC result = 0;
    int j, k;
    unsigned used = 0;

    if ((me = myCache(xw, cgsWin, cgsId)) != 0) {
	TRACE2(("getCgsGC(%s, %s)\n",
		traceVTwin(xw, cgsWin), traceCgsEnum(cgsId)));
	if (me->mask != 0) {

	    /* fill in the unchanged fields */
	    if (!(me->mask & GC_CSet))
		NEXT(cset) = 0;	/* OPT_DEC_CHRSET */
	    if (!(me->mask & GCFont))
		NEXT(font) = THIS(font);
	    if (!(me->mask & GCForeground))
		NEXT(fg) = THIS(fg);
	    if (!(me->mask & GCBackground))
		NEXT(bg) = THIS(bg);

	    if (NEXT(font) == 0) {
		setCgsFont(xw, cgsWin, cgsId, 0);
	    }

	    TRACE2(("...Cgs new data fg=%s, bg=%s, font=%s cset %s\n",
		    tracePixel(xw, NEXT(fg)),
		    tracePixel(xw, NEXT(bg)),
		    traceFont(NEXT(font)),
		    traceCSet(NEXT(cset))));

	    /* try to find the given data in an already-created GC */
	    for (j = 0; j < DEPTH; ++j) {
		if (LIST(j).gc != 0
		    && SameFont(LIST(j).font, NEXT(font))
		    && SameCSet(LIST(j).cset, NEXT(cset))
		    && SameColor(LIST(j).fg, NEXT(fg))
		    && SameColor(LIST(j).bg, NEXT(bg))) {
		    LINK(j);
		    result = THIS(gc);
		    TRACE2(("getCgsGC existing %p(%d)\n", result, ITEM()));
		    break;
		}
	    }

	    if (result == 0) {
		/* try to find an empty slot, to create a new GC */
		used = 0;
		for (j = 0; j < DEPTH; ++j) {
		    if (LIST(j).gc == 0) {
			LINK(j);
			result = newCache(xw, cgsWin, cgsId, me);
			break;
		    }
		    if (used < LIST(j).used)
			used = LIST(j).used;
		}
	    }

	    if (result == 0) {
		/* if none were empty, pick the least-used slot, to modify */
		for (j = 0, k = -1; j < DEPTH; ++j) {
		    if (used >= LIST(j).used) {
			used = LIST(j).used;
			k = j;
		    }
		}
		LINK(k);
		TRACE(("...getCgsGC least-used(%d) was %d\n", k, THIS(used)));
		result = chgCache(xw, cgsId, me);
	    }
	    me->next = *(me->data);
	} else {
	    result = THIS(gc);
	}
	me->mask = 0;
	THIS(used) += 1;
	TRACE2(("...getCgsGC(%s, %s) gc %p(%d), used %d\n",
		traceVTwin(xw, cgsWin),
		traceCgsEnum(cgsId), result, ITEM(), THIS(used)));
    }
    return result;
}

/*
 * Return the font for the given GC.
 */
CgsEnum
getCgsId(XtermWidget xw, VTwin * cgsWin, GC gc)
{
    int n;
    CgsEnum result = gcNorm;

    for_each_gc(n) {
	CgsCache *me;

	if ((me = myCache(xw, cgsWin, (CgsEnum) n)) != 0) {
	    if (THIS(gc) == gc) {
		result = (CgsEnum) n;
		break;
	    }
	}
    }
    return result;
}

/*
 * Return the font for the given GC.
 */
XFontStruct *
getCgsFont(XtermWidget xw, VTwin * cgsWin, GC gc)
{
    int n;
    XFontStruct *result = 0;

    for_each_gc(n) {
	CgsCache *me;

	if ((me = myCache(xw, cgsWin, (CgsEnum) n)) != 0) {
	    if (THIS(gc) == gc) {
		result = THIS(font);
		break;
	    }
	}
    }
    return result;
}

/*
 * Return the foreground color for the given GC.
 */
Pixel
getCgsFore(XtermWidget xw, VTwin * cgsWin, GC gc)
{
    int n;
    Pixel result = 0;

    for_each_gc(n) {
	CgsCache *me;

	if ((me = myCache(xw, cgsWin, (CgsEnum) n)) != 0) {
	    if (THIS(gc) == gc) {
		result = THIS(fg);
		break;
	    }
	}
    }
    return result;
}

/*
 * Return the background color for the given GC.
 */
Pixel
getCgsBack(XtermWidget xw, VTwin * cgsWin, GC gc)
{
    int n;
    Pixel result = 0;

    for_each_gc(n) {
	CgsCache *me;

	if ((me = myCache(xw, cgsWin, (CgsEnum) n)) != 0) {
	    if (THIS(gc) == gc) {
		result = THIS(bg);
		break;
	    }
	}
    }
    return result;
}

/*
 * Copy the parameters (except GC of course) from one cache record to another.
 */
void
copyCgs(XtermWidget xw, VTwin * cgsWin, CgsEnum dstCgsId, CgsEnum srcCgsId)
{
    if (dstCgsId != srcCgsId) {
	CgsCache *me;

	if ((me = myCache(xw, cgsWin, srcCgsId)) != 0) {
	    TRACE(("copyCgs from %s to %s\n",
		   traceCgsEnum(srcCgsId),
		   traceCgsEnum(dstCgsId)));
	    setCgsFont(xw, cgsWin, dstCgsId, THIS(font));
	    setCgsCSet(xw, cgsWin, dstCgsId, THIS(cset));
	    setCgsFore(xw, cgsWin, dstCgsId, THIS(fg));
	    setCgsBack(xw, cgsWin, dstCgsId, THIS(bg));
	}
    }
}

/*
 * Interchange colors in the cache, e.g., for reverse-video.
 */
void
redoCgs(XtermWidget xw, Pixel fg, Pixel bg, CgsEnum cgsId)
{
    int n;
    VTwin *cgsWin = WhichVWin(&(xw->screen));
    CgsCache *me = myCache(xw, cgsWin, cgsId);

    if (me != 0) {
	CgsCacheData *save_data = me->data;

	for (n = 0; n < DEPTH; ++n) {
	    if (LIST(n).gc != 0) {
		LINK(n);

		if (LIST(n).fg == fg
		    && LIST(n).bg == bg) {
		    setCgsFore(xw, cgsWin, cgsId, bg);
		    setCgsBack(xw, cgsWin, cgsId, fg);
		} else if (LIST(n).fg == bg
			   && LIST(n).bg == fg) {
		    setCgsFore(xw, cgsWin, cgsId, fg);
		    setCgsBack(xw, cgsWin, cgsId, bg);
		} else {
		    continue;
		}

		(void) chgCache(xw, cgsId, me);
	    }
	}
	me->data = save_data;
    }
}

/*
 * Swap the cache records, e.g., when doing reverse-video.
 */
void
swapCgs(XtermWidget xw, VTwin * cgsWin, CgsEnum dstCgsId, CgsEnum srcCgsId)
{
    if (dstCgsId != srcCgsId) {
	CgsCache *dst;
	CgsCache *src;

	if ((src = myCache(xw, cgsWin, srcCgsId)) != 0) {
	    if ((dst = myCache(xw, cgsWin, dstCgsId)) != 0) {
		CgsCache tmp;
		tmp = *dst;
		*dst = *src;
		*src = tmp;
	    }
	}
    }
}

/*
 * Free any GC associated with the given id.
 */
GC
freeCgs(XtermWidget xw, VTwin * cgsWin, CgsEnum cgsId)
{
    CgsCache *me;
    int j;

    if ((me = myCache(xw, cgsWin, cgsId)) != 0) {
	for (j = 0; j < DEPTH; ++j) {
	    if (LIST(j).gc != 0) {
		TRACE(("freeCgs(%s, %s) gc %p(%d)\n",
		       traceVTwin(xw, cgsWin),
		       traceCgsEnum(cgsId), LIST(j).gc, j));
		clrCgsFonts(xw, cgsWin, LIST(j).font);
#if OPT_BOX_CHARS
		if (cgsId == gcDots) {
		    XmuReleaseStippledPixmap(XtScreen((Widget) xw), LIST(j).tile);
		}
#endif
		XFreeGC(xw->screen.display, LIST(j).gc);
		memset(&LIST(j), 0, sizeof(LIST(j)));
	    }
	    LINK(0);
	}
    }
    return 0;
}
