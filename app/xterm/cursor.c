/* $XTermId: cursor.c,v 1.93 2023/09/21 08:17:56 tom Exp $ */

/*
 * Copyright 2002-2022,2023 by Thomas E. Dickey
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
 *
 * Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Digital Equipment
 * Corporation not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior permission.
 *
 *
 * DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/* cursor.c */

#include <xterm.h>
#include <data.h>
#include <menu.h>

#include <assert.h>

/*
 * Moves the cursor to the specified position, checking for bounds.
 * (this includes scrolling regions)
 * The origin is considered to be 0, 0 for this procedure.
 */
void
CursorSet(TScreen *screen, int row, int col, unsigned flags)
{
    int use_row = row;
    int use_col = col;
    int max_col = screen->max_col;
    int max_row = screen->max_row;

    if (flags & ORIGIN) {
	use_col += screen->lft_marg;
	max_col = screen->rgt_marg;
    }
    use_col = (use_col < 0 ? 0 : use_col);
    set_cur_col(screen, (use_col <= max_col ? use_col : max_col));

    if (flags & ORIGIN) {
	use_row += screen->top_marg;
	max_row = screen->bot_marg;
    }
    use_row = (use_row < 0 ? 0 : use_row);
    set_cur_row(screen, (use_row <= max_row ? use_row : max_row));

    ResetWrap(screen);

    TRACE(("CursorSet(%d,%d) margins V[%d..%d] H[%d..%d] -> %d,%d %s\n",
	   row, col,
	   screen->top_marg,
	   screen->bot_marg,
	   screen->lft_marg,
	   screen->rgt_marg,
	   screen->cur_row,
	   screen->cur_col,
	   ((flags & ORIGIN) ? "origin" : "normal")));
}

/*
 * Unlike VT100, xterm allows reverse wrapping of the cursor.  This feature was
 * introduced in X10R4 (December 1986), but did not modify the comment which
 * said "moves the cursor left n, no wrap around".  However, this reverse
 * wrapping allowed the cursor to wrap around to the end of the screen.
 *
 * xterm added VT420-compatible left/right margin support in 2012.  If the
 * cursor starts off within the margins, the reverse wrapping result will be
 * within the margins.
 *
 * Wrapping to the end of the screen did not appear to be the original intent.
 * That was revised in 2023, using private mode 45 for movement within the
 * current (wrapped) line, and 1045 for movement to "any" line.
 */
void
CursorBack(XtermWidget xw, int n)
{
#define WRAP_MASK (REVERSEWRAP | WRAPAROUND)
#define WRAP_MASK2 (REVERSEWRAP2 | WRAPAROUND)
    TScreen *screen = TScreenOf(xw);
    /* *INDENT-EQLS* */
    int rev    = (((xw->flags & WRAP_MASK) == WRAP_MASK) != 0);
    int rev2   = (((xw->flags & WRAP_MASK2) == WRAP_MASK2) != 0);
    int left   = ScrnLeftMargin(xw);
    int right  = ScrnRightMargin(xw);
    int before = screen->cur_col;
    int top    = ScrnTopMargin(xw);
    int bottom = ScrnBottomMargin(xw);
    int col    = screen->cur_col;
    int row    = screen->cur_row;

    int count;
    CLineData *ld;

    TRACE(("CursorBack(%d) current %d,%d rev=%d/%d margins H[%d..%d] V[%d..%d]\n",
	   n,
	   screen->cur_row, screen->cur_col,
	   rev, rev2,
	   left, right,
	   top, bottom));

    /* if the cursor is already before the left-margin, we have to let it go */
    if (before < left)
	left = 0;

    ld = NULL;
    if ((count = n) > 0) {
	if ((rev || rev2) && screen->do_wrap) {
	    --count;
	} else {
	    --col;
	}
    }

    for (;;) {
	if (col < left) {
	    if (rev2) {
		col = right;
		if (row == top)
		    row = bottom + 1;
	    } else {
		if (!rev) {
		    col = left;
		    break;
		}
		if (row <= top) {
		    col = left;
		    row = top;
		    break;
		}
	    }
	    ld = NULL;		/* try a reverse-wrap */
	    --row;
	}
	if (ld == NULL) {
	    ld = getLineData(screen, ROW2INX(screen, row));
	    if (ld == NULL)
		break;		/* should not happen */
	    if (row != screen->cur_row) {
		if (!rev2 && !LineTstWrapped(ld)) {
		    ++row;	/* reverse-wrap failed */
		    col = left;
		    break;
		}
		col = right;
	    }
	}

	if (--count <= 0)
	    break;
	--col;
    }
    set_cur_row(screen, row);
    set_cur_col(screen, col);
    do_xevents(xw);

    ResetWrap(screen);
}

/*
 * moves the cursor forward n, no wraparound
 */
void
CursorForward(XtermWidget xw, int n)
{
    TScreen *screen = TScreenOf(xw);
#if OPT_DEC_CHRSET
    LineData *ld = getLineData(screen, screen->cur_row);
#endif
    int next = screen->cur_col + n;
    int max;

    if (IsLeftRightMode(xw)) {
	max = screen->rgt_marg;
	if (screen->cur_col > max)
	    max = screen->max_col;
    } else {
	max = LineMaxCol(screen, ld);
    }

    if (next > max)
	next = max;

    set_cur_col(screen, next);
    ResetWrap(screen);
}

/*
 * moves the cursor down n, no scrolling.
 * Won't pass bottom margin or bottom of screen.
 */
void
CursorDown(TScreen *screen, int n)
{
    int max;
    int next = screen->cur_row + n;

    max = (screen->cur_row > screen->bot_marg ?
	   screen->max_row : screen->bot_marg);
    if (next > max)
	next = max;
    if (next > screen->max_row)
	next = screen->max_row;

    set_cur_row(screen, next);
    ResetWrap(screen);
}

/*
 * moves the cursor up n, no linestarving.
 * Won't pass top margin or top of screen.
 */
void
CursorUp(TScreen *screen, int n)
{
    int min;
    int next = screen->cur_row - n;

    min = ((screen->cur_row < screen->top_marg)
	   ? 0
	   : screen->top_marg);
    if (next < min)
	next = min;
    if (next < 0)
	next = 0;

    set_cur_row(screen, next);
    ResetWrap(screen);
}

/*
 * Moves cursor down amount lines, scrolls if necessary.
 * Won't leave scrolling region. No carriage return.
 */
void
xtermIndex(XtermWidget xw, int amount)
{
    TScreen *screen = TScreenOf(xw);

    /*
     * indexing when below scrolling region is cursor down.
     * if cursor high enough, no scrolling necessary.
     */
    if (screen->cur_row > screen->bot_marg
	|| screen->cur_row + amount <= screen->bot_marg
	|| (IsLeftRightMode(xw)
	    && !ScrnIsColInMargins(screen, screen->cur_col))) {
	CursorDown(screen, amount);
    } else {
	int j;
	CursorDown(screen, j = screen->bot_marg - screen->cur_row);
	xtermScroll(xw, amount - j);
    }
}

/*
 * Moves cursor up amount lines, reverse scrolls if necessary.
 * Won't leave scrolling region. No carriage return.
 */
void
RevIndex(XtermWidget xw, int amount)
{
    TScreen *screen = TScreenOf(xw);

    /*
     * reverse indexing when above scrolling region is cursor up.
     * if cursor low enough, no reverse indexing needed
     */
    if (screen->cur_row < screen->top_marg
	|| screen->cur_row - amount >= screen->top_marg
	|| (IsLeftRightMode(xw)
	    && !ScrnIsColInMargins(screen, screen->cur_col))) {
	CursorUp(screen, amount);
    } else {
	RevScroll(xw, amount - (screen->cur_row - screen->top_marg));
	CursorUp(screen, screen->cur_row - screen->top_marg);
    }
}

/*
 * Moves Cursor To First Column In Line
 * (Note: xterm doesn't implement SLH, SLL which would affect use of this)
 */
void
CarriageReturn(XtermWidget xw)
{
    TScreen *screen = TScreenOf(xw);
    int left = ScrnLeftMargin(xw);
    int col;

    if (xw->flags & ORIGIN) {
	col = left;
    } else if (screen->cur_col >= left) {
	col = left;
    } else {
	/*
	 * If origin-mode is not active, it is possible to use cursor
	 * addressing outside the margins.  In that case we will go to the
	 * first column rather than following the margin.
	 */
	col = 0;
    }

    set_cur_col(screen, col);
    ResetWrap(screen);
    if (screen->jumpscroll && !screen->fastscroll)
	do_xevents(xw);
}

/*
 * When resizing the window, if we're showing the alternate screen, we still
 * have to adjust the saved cursor from the normal screen to account for
 * shifting of the saved-line region in/out of the viewable window.
 */
void
AdjustSavedCursor(XtermWidget xw, int adjust)
{
    TScreen *screen = TScreenOf(xw);

    if (screen->whichBuf) {
	SavedCursor *sc = &screen->sc[0];

	if (adjust > 0) {
	    TRACE(("AdjustSavedCursor %d -> %d\n", sc->row, sc->row - adjust));
	    sc->row += adjust;
	}
    }
}

/*
 * Save Cursor and Attributes
 */
void
CursorSave2(XtermWidget xw, SavedCursor * sc)
{
    TScreen *screen = TScreenOf(xw);

    sc->saved = True;
    sc->row = screen->cur_row;
    sc->col = screen->cur_col;
    sc->flags = xw->flags;
    sc->curgl = screen->curgl;
    sc->curgr = screen->curgr;
    sc->wrap_flag = screen->do_wrap;
#if OPT_ISO_COLORS
    sc->cur_foreground = xw->cur_foreground;
    sc->cur_background = xw->cur_background;
    sc->sgr_foreground = xw->sgr_foreground;
    sc->sgr_38_xcolors = xw->sgr_38_xcolors;
#endif
    saveCharsets(screen, sc->gsets);
}

void
CursorSave(XtermWidget xw)
{
    TScreen *screen = TScreenOf(xw);
    CursorSave2(xw, &screen->sc[screen->whichBuf]);
}

/*
 * We save/restore all visible attributes, plus wrapping, origin mode, and the
 * selective erase attribute.
 *
 * This is documented, but some of the documentation is incorrect.
 *
 * Page 270 of the VT420 manual (2nd edition) says that DECSC saves these
 * items:
 *
 * Cursor position
 * * Character attributes set by the SGR command
 * * Character sets (G0, G1, G2, or G3) currently in GL and GR
 * * Wrap flag (autowrap or no autowrap)
 * * State of origin mode (DECOM)
 * * Selective erase attribute
 * * Any single shift 2 (SS2) or single shift 3 (SS3) functions sent
 *
 * The VT520 manual has the same information (page 5-120).
 *
 * However, DEC 070 (29-June-1990), pages 5-186 to 5-191, describes
 * save/restore operations, but makes no mention of "wrap".
 *
 * Mattias Engdegård, who has investigated wrapping behavior of different
 * terminals,
 *
 *	https://github.com/mattiase/wraptest
 *
 * states
 *	The LCF is saved/restored by the Save/Restore Cursor (DECSC/DECRC)      
 *	control sequences.  The DECAWM flag is not included in the state        
 *	managed by these operations.
 *
 * DEC 070 does mention the ANSI color text extension saying that it, too, is
 * saved/restored.
 */
#define ALL_FLAGS (IFlags)(~0)
#define DECSC_FLAGS (ATTRIBUTES|ORIGIN|PROTECTED)

/*
 * Restore Cursor and Attributes
 */
static void
CursorRestoreFlags(XtermWidget xw, SavedCursor * sc, IFlags our_flags)
{
    TScreen *screen = TScreenOf(xw);

    /* Restore the character sets, unless we never did a save-cursor op.
     * In that case, we'll reset the character sets.
     */
    if (sc->saved) {
	restoreCharsets(screen, sc->gsets);
	screen->curgl = sc->curgl;
	screen->curgr = sc->curgr;
    } else {
	resetCharsets(screen);
    }

    UIntClr(xw->flags, our_flags);
    UIntSet(xw->flags, sc->flags & our_flags);
    if ((xw->flags & ORIGIN)) {
	CursorSet(screen,
		  sc->row - screen->top_marg,
		  ((xw->flags & LEFT_RIGHT)
		   ? sc->col - screen->lft_marg
		   : sc->col),
		  xw->flags);
    } else {
	CursorSet(screen, sc->row, sc->col, xw->flags);
    }
    screen->do_wrap = sc->wrap_flag;	/* after CursorSet/ResetWrap */

#if OPT_ISO_COLORS
    xw->sgr_foreground = sc->sgr_foreground;
    xw->sgr_38_xcolors = sc->sgr_38_xcolors;
    SGR_Foreground(xw, (xw->flags & FG_COLOR) ? sc->cur_foreground : -1);
    SGR_Background(xw, (xw->flags & BG_COLOR) ? sc->cur_background : -1);
#endif
}

/*
 * Use this entrypoint for the status-line.
 */
void
CursorRestore2(XtermWidget xw, SavedCursor * sc)
{
    CursorRestoreFlags(xw, sc, ALL_FLAGS);
}

/*
 * Use this entrypoint for the VT100 window.
 */
void
CursorRestore(XtermWidget xw)
{
    TScreen *screen = TScreenOf(xw);
    CursorRestoreFlags(xw, &screen->sc[screen->whichBuf], DECSC_FLAGS);
}

/*
 * Move the cursor to the first column of the n-th next line.
 */
void
CursorNextLine(XtermWidget xw, int count)
{
    TScreen *screen = TScreenOf(xw);

    CursorDown(screen, count < 1 ? 1 : count);
    CarriageReturn(xw);
}

/*
 * Move the cursor to the first column of the n-th previous line.
 */
void
CursorPrevLine(XtermWidget xw, int count)
{
    TScreen *screen = TScreenOf(xw);

    CursorUp(screen, count < 1 ? 1 : count);
    CarriageReturn(xw);
}

/*
 * Return col/row values which can be passed to CursorSet() preserving the
 * current col/row, e.g., accounting for DECOM.
 */
int
CursorCol(XtermWidget xw)
{
    TScreen *screen = TScreenOf(xw);
    int result = screen->cur_col;
    if (xw->flags & ORIGIN) {
	result -= ScrnLeftMargin(xw);
	if (result < 0)
	    result = 0;
    }
    return result;
}

int
CursorRow(XtermWidget xw)
{
    TScreen *screen = TScreenOf(xw);
    int result = screen->cur_row;
    if (xw->flags & ORIGIN) {
	result -= screen->top_marg;
	if (result < 0)
	    result = 0;
    }
    return result;
}

#if OPT_TRACE
int
set_cur_row(TScreen *screen, int value)
{
    TRACE(("set_cur_row %d vs %d\n", value, screen ? LastRowNumber(screen) : -1));

    assert(screen != 0);
    assert(value >= 0);
    assert(value <= LastRowNumber(screen));
    if_STATUS_LINE(screen, {
	value = LastRowNumber(screen);
    });
    screen->cur_row = value;
    return value;
}

int
set_cur_col(TScreen *screen, int value)
{
    TRACE(("set_cur_col %d vs %d\n", value, screen ? screen->max_col : -1));

    assert(screen != 0);
    assert(value >= 0);
    assert(value <= screen->max_col);
    screen->cur_col = value;
    return value;
}
#endif /* OPT_TRACE */
/*
 * vile:cmode fk=utf-8
 */
