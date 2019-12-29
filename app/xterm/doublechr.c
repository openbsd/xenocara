/* $XTermId: doublechr.c,v 1.101 2019/09/12 00:24:07 tom Exp $ */

/*
 * Copyright 1997-2018,2019 by Thomas E. Dickey
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

#include <xterm.h>
#include <data.h>
#include <fontutils.h>

#include <assert.h>

#define WhichCgsId(flag) (((flag) & BOLD) ? gcCBold : gcCNorm)

/*
 * The first column is all that matters for double-size characters (since the
 * controls apply to a whole line).  However, it's easier to maintain the
 * information for special fonts by writing to all cells.
 */
#if OPT_DEC_CHRSET

static void
repaint_line(XtermWidget xw, unsigned newChrSet)
{
    TScreen *screen = TScreenOf(xw);
    LineData *ld;
    int curcol = screen->cur_col;
    int currow = screen->cur_row;
    int width = MaxCols(screen);
    unsigned len = (unsigned) width;

    assert(width > 0);

    /*
     * Ignore repetition.
     */
    if (!IsLeftRightMode(xw)
	&& (ld = getLineData(screen, currow)) != 0) {
	unsigned oldChrSet = GetLineDblCS(ld);

	if (oldChrSet != newChrSet) {
	    TRACE(("repaint_line(%2d,%2d) (%s -> %s)\n", currow, screen->cur_col,
		   visibleDblChrset(oldChrSet),
		   visibleDblChrset(newChrSet)));
	    HideCursor();

	    /* If switching from single-width, keep the cursor in the visible part
	     * of the line.
	     */
	    if (CSET_DOUBLE(newChrSet)) {
		width /= 2;
		if (curcol > width)
		    curcol = width;
	    }

	    /*
	     * ScrnRefresh won't paint blanks for us if we're switching between a
	     * single-size and double-size font.  So we paint our own.
	     */
	    ClearCurBackground(xw,
			       currow,
			       0,
			       1,
			       len,
			       (unsigned) LineFontWidth(screen, ld));

	    SetLineDblCS(ld, newChrSet);

	    set_cur_col(screen, 0);
	    ScrnUpdate(xw, currow, 0, 1, (int) len, True);
	    set_cur_col(screen, curcol);
	}
    }
}
#endif

/*
 * Set the line to double-height characters.  The 'top' flag denotes whether
 * we'll be using it for the top (true) or bottom (false) of the line.
 */
void
xterm_DECDHL(XtermWidget xw GCC_UNUSED, Bool top)
{
#if OPT_DEC_CHRSET
    repaint_line(xw, (unsigned) (top ? CSET_DHL_TOP : CSET_DHL_BOT));
#else
    (void) top;
#endif
}

/*
 * Set the line to single-width characters (the normal state).
 */
void
xterm_DECSWL(XtermWidget xw GCC_UNUSED)
{
#if OPT_DEC_CHRSET
    repaint_line(xw, CSET_SWL);
#endif
}

/*
 * Set the line to double-width characters
 */
void
xterm_DECDWL(XtermWidget xw GCC_UNUSED)
{
#if OPT_DEC_CHRSET
    repaint_line(xw, CSET_DWL);
#endif
}

/*
 * Reset all lines on the screen to single-width/single-height.
 */
void
xterm_ResetDouble(XtermWidget xw)
{
#if OPT_DEC_CHRSET
    TScreen *screen = TScreenOf(xw);
    Boolean changed = False;
    unsigned code;
    int row;

    for (row = 0; row < screen->max_row; ++row) {
	LineData *ld;

	if ((ld = getLineData(screen, ROW2INX(screen, row))) != 0) {
	    code = GetLineDblCS(ld);
	    if (code != CSET_SWL) {
		SetLineDblCS(ld, CSET_SWL);
		changed = True;
	    }
	}
    }
    if (changed) {
	xtermRepaint(xw);
    }
#else
    (void) xw;
#endif
}

#if OPT_DEC_CHRSET
static void
discard_font(XtermWidget xw, int n)
{
    TScreen *screen = TScreenOf(xw);
    XTermFonts *data = getDoubleFont(screen, n);

    TRACE(("discard_font chrset=%d %s\n", data->chrset,
	   (data->fn != 0) ? data->fn : "<no-name>"));

    data->chrset = 0;
    data->flags = 0;
    if (data->fn != 0) {
	free(data->fn);
	data->fn = 0;
    }
    xtermCloseFont(xw, data);

    screen->fonts_used -= 1;
    while (n < screen->fonts_used) {
	screen->double_fonts[n] = screen->double_fonts[n + 1];
	++n;
    }
}

/* push back existing fonts and create a new entry */
static XTermFonts *
pushback_font(XtermWidget xw, XTermFonts * source)
{
    TScreen *screen = TScreenOf(xw);
    XTermFonts *data = getDoubleFont(screen, 0);
    int n;

    if (screen->fonts_used >= screen->cache_doublesize) {
	TRACE(("pushback_font: discard oldest\n"));
	discard_font(xw, screen->fonts_used - 1);
    } else {
	screen->fonts_used += 1;
    }

    for (n = screen->fonts_used; n > 0; n--)
	data[n] = data[n - 1];
    data[0] = *source;

    TRACE(("pushback_font -> (NEW:%d)\n", screen->fonts_used));

    return data;
}

static int
xterm_Double_index(XTermDraw * params)
{
    XTermDraw local = *params;
    int n;
    int result = -1;
    TScreen *screen = TScreenOf(local.xw);
    XTermFonts *data = getDoubleFont(screen, 0);

    local.attr_flags &= BOLD;
    TRACE(("xterm_Double_index chrset=%#x, flags=%#x\n", local.this_chrset, local.attr_flags));

    for (n = 0; n < screen->fonts_used; n++) {
	if (data[n].chrset == (unsigned) local.this_chrset
	    && data[n].flags == local.attr_flags) {
	    if (n != 0) {
		XTermFonts save;
		TRACE(("...xterm_Double_index -> %d (OLD:%d)\n", n, screen->fonts_used));
		save = data[n];
		while (n > 0) {
		    data[n] = data[n - 1];
		    n--;
		}
		data[n] = save;
	    }
	    result = n;
	    break;
	}
    }

    return result;
}

/*
 * Lookup/cache a GC for the double-size character display.  We save up to
 * NUM_CHRSET values.
 */
GC
xterm_DoubleGC(XTermDraw * params, GC old_gc, int *inxp)
{
    TScreen *screen = TScreenOf(params->xw);
    VTwin *cgsWin = WhichVWin(screen);
    char *name;
    GC result = 0;

    if ((name = xtermSpecialFont(params))
	!= 0) {
	CgsEnum cgsId = WhichCgsId(params->attr_flags);
	Boolean found = False;
	XTermFonts *data = 0;
	int n;

	if ((n = xterm_Double_index(params)) >= 0) {
	    data = getDoubleFont(screen, n);
	    if (data->fn != 0) {
		if (!strcmp(data->fn, name)
		    && data->fs != 0) {
		    found = True;
		    free(name);
		    name = NULL;
		} else {
		    discard_font(params->xw, n);
		}
	    }
	}

	if (!found && name != NULL) {
	    XTermFonts temp;

	    TRACE(("xterm_DoubleGC %s %d: %s\n",
		   (params->attr_flags & BOLD) ? "BOLD" : "NORM", n, name));

	    memset(&temp, 0, sizeof(temp));
	    temp.fn = name;
	    temp.chrset = params->this_chrset;
	    temp.flags = (params->attr_flags & BOLD);
	    temp.warn = fwResource;

	    if (!xtermOpenFont(params->xw, name, &temp, False)) {
		XTermDraw local = *params;
		char *nname;

		/* Retry with * in resolutions */
		local.draw_flags |= NORESOLUTION;
		nname = xtermSpecialFont(&local);
		if (nname != 0) {
		    found = (Boolean) xtermOpenFont(params->xw, nname, &temp,
						    False);
		    free(nname);
		}
	    } else {
		found = True;
	    }
	    free(name);

	    if (found) {
		n = 0;
		data = pushback_font(params->xw, &temp);
	    }

	    TRACE(("-> %s\n", found ? "OK" : "FAIL"));
	}

	if (found) {
	    setCgsCSet(params->xw, cgsWin, cgsId, params->this_chrset);
	    setCgsFont(params->xw, cgsWin, cgsId, data);
	    setCgsFore(params->xw, cgsWin, cgsId, getCgsFore(params->xw,
							     cgsWin, old_gc));
	    setCgsBack(params->xw, cgsWin, cgsId, getCgsBack(params->xw,
							     cgsWin, old_gc));
	    result = getCgsGC(params->xw, cgsWin, cgsId);
	    *inxp = n;
	} else if (params->attr_flags & BOLD) {
	    UIntClr(params->attr_flags, BOLD);
	    result = xterm_DoubleGC(params, old_gc, inxp);
	}
    }

    return result;
}

#if OPT_RENDERFONT
/*
 * Like xterm_DoubleGC(), but returning an Xft font.
 */
XftFont *
xterm_DoubleFT(XTermDraw * params, unsigned chrset, unsigned attr_flags)
{
    XftFont *result;
    TScreen *screen = TScreenOf(params->xw);
    unsigned num = (chrset & CSET_DWL);

    if ((attr_flags &= BOLD) != 0)
	num |= 4;

    if ((result = screen->double_xft_fonts[num]) == 0) {
	result = getDoubleXftFont(params, chrset, attr_flags);
	screen->double_xft_fonts[num] = result;
    }
    return result;
}

void
freeall_DoubleFT(XtermWidget xw)
{
    TScreen *screen = TScreenOf(xw);
    unsigned num;

    for (num = 0; num < XtNumber(screen->double_xft_fonts); ++num) {
	closeCachedXft(screen, screen->double_xft_fonts[num]);
	screen->double_xft_fonts[num] = 0;
    }
}
#endif /* OPT_RENDERFONT */

#endif /* OPT_DEC_CHRSET */
