/* $XTermId: charsets.c,v 1.129 2024/10/03 22:21:32 tom Exp $ */

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
 *
 */

#include <assert.h>
#include <X11/keysym.h>

#include <xterm.h>
#include <data.h>
#include <charsets.h>
#include <fontutils.h>

/*
 * This module performs translation as needed to support the DEC VT220 national
 * replacement character sets as well as supplementary character sets (aka
 * code-pages) introduced in VT320, etc.
 *
 * We assume that xterm's font is based on the ISO 8859-1 (Latin 1) character
 * set, which is almost the same as the DEC multinational character set.  Glyph
 * positions 0-31 have to be the DEC graphic characters, though.
 *
 * References:
 *	"VT220 Programmer Pocket Guide" EK-VT220-HR-002 (2nd ed., 1984), which
 *		contains character charts for the national character sets.
 *	"VT330/VT340 Programmer Reference Manual Volume 1: Text Programming"
 *		EK-VT3XX-TP-001 (1st ed, 1987), which contains a table (2-1)
 *		listing the glyphs which are mapped from the multinational
 *		character set to the national character set.
 *
 * The latter reference, though easier to read, has a few errors and omissions.
 */

#define HandleUPSS(charset) \
    if (charset == nrc_DEC_UPSS) { \
	charset = screen->gsets_upss; \
	if (screen->vtXX_level >= 5) { \
	    /* EMPTY */ ; \
	} else if (screen->vtXX_level >= 3) { \
	    if (charset != nrc_DEC_Supp) \
		charset = nrc_ISO_Latin_1_Supp; \
	} else if (screen->vtXX_level < 2) { \
	    charset = nrc_ASCII; \
	} \
    }

static Boolean
isSevenBit(DECNRCM_codes cs)
{
    Boolean result = False;

    switch (cs) {
    case nrc_ISO_Greek_Supp:
    case nrc_ISO_Hebrew_Supp:
    case nrc_ISO_Latin_1_Supp:
    case nrc_ISO_Latin_2_Supp:
    case nrc_ISO_Latin_5_Supp:
    case nrc_ISO_Latin_Cyrillic:
    case nrc_DEC_UPSS:
	break;
	/* VT100 character sets */
    case nrc_ASCII:
    case nrc_British:
    case nrc_DEC_Alt_Chars:
    case nrc_DEC_Spec_Graphic:
	/* VT220 character sets */
    case nrc_DEC_Alt_Graphics:
    case nrc_DEC_Supp:
	/* VT320 character sets */
    case nrc_DEC_Supp_Graphic:
    case nrc_DEC_Technical:
	/* NRCS character sets (VT320 to VT520) */
    case nrc_British_Latin_1:
    case nrc_Dutch:
    case nrc_Finnish2:
    case nrc_Finnish:
    case nrc_French2:
    case nrc_French:
    case nrc_French_Canadian2:
    case nrc_French_Canadian:
    case nrc_German:
    case nrc_Greek:
    case nrc_Hebrew:
    case nrc_Italian:
    case nrc_JIS_Katakana:
    case nrc_JIS_Roman:
    case nrc_Norwegian_Danish2:
    case nrc_Norwegian_Danish3:
    case nrc_Norwegian_Danish:
    case nrc_Portugese:
    case nrc_Russian:
    case nrc_SCS_NRCS:
    case nrc_Spanish:
    case nrc_Swedish2:
    case nrc_Swedish:
    case nrc_Swiss:
    case nrc_Turkish:
	/* other DEC character sets */
    case nrc_DEC_Cyrillic:
    case nrc_DEC_Greek_Supp:
    case nrc_DEC_Hebrew_Supp:
    case nrc_DEC_Turkish_Supp:
	result = True;
	break;
    case nrc_Unknown:
	break;
    }
    return result;
}

/*
 * Translate an input keysym to the corresponding NRC keysym.
 */
unsigned
xtermCharSetIn(XtermWidget xw, unsigned code, DECNRCM_codes charset)
{
    TScreen *screen = TScreenOf(xw);
#define MAP(to, from) case from: code = to; break;

#if OPT_WIDE_CHARS
#define UNI(to, from) case from: if (screen->utf8_nrc_mode) code = to; break;
#else
#define UNI(to, from) case from: break;
#endif

#define XXX(to, from)		/* no defined mapping to 0..255 */

    TRACE(("CHARSET-IN GL=%s(G%d) GR=%s(G%d) SS%d\n\t%s\n",
	   visibleScsCode(screen->gsets[screen->curgl]), screen->curgl,
	   visibleScsCode(screen->gsets[screen->curgr]), screen->curgr,
	   screen->curss,
	   visibleUChar(code)));

    HandleUPSS(charset);

    switch (charset) {
    case nrc_British:		/* United Kingdom set (or Latin 1)      */
	if (code == XK_sterling)
	    code = 0x23;
	code &= 0x7f;
	break;

    case nrc_DEC_Alt_Chars:
    case nrc_DEC_Alt_Graphics:
    case nrc_ASCII:
	break;

    case nrc_DEC_Spec_Graphic:
	break;

    case nrc_DEC_Supp:
	map_DEC_Supp_Graphic(code, code &= 0x7f);
	break;

    case nrc_DEC_Supp_Graphic:
	map_DEC_Supp_Graphic(code, code |= 0x80);
	break;

    case nrc_DEC_Technical:
	map_DEC_Technical(code);
	break;

    case nrc_Dutch:
	map_NRCS_Dutch(code);
	break;

    case nrc_Finnish:
    case nrc_Finnish2:
	map_NRCS_Finnish(code);
	break;

    case nrc_French:
    case nrc_French2:
	map_NRCS_French(code);
	break;

    case nrc_French_Canadian:
	map_NRCS_French_Canadian(code);
	break;

    case nrc_German:
	map_NRCS_German(code);
	break;

    case nrc_Greek:
	map_NRCS_Greek(code);	/* FIXME - ELOT? */
	break;

    case nrc_DEC_Greek_Supp:
	map_DEC_Greek_Supp(code);
	break;

    case nrc_ISO_Greek_Supp:
	map_ISO_Greek_Supp(code);
	break;

    case nrc_DEC_Hebrew_Supp:
	map_DEC_Hebrew_Supp(code);
	break;

    case nrc_Hebrew:
	map_NRCS_Hebrew(code);
	break;

    case nrc_ISO_Hebrew_Supp:
	map_ISO_Hebrew(code);
	break;

    case nrc_Italian:
	map_NRCS_Italian(code);
	break;

    case nrc_ISO_Latin_2_Supp:
	map_ISO_Latin_2(code);
	break;

    case nrc_ISO_Latin_5_Supp:
	map_ISO_Latin_5(code);
	break;

    case nrc_ISO_Latin_Cyrillic:
	map_ISO_Latin_Cyrillic(code);
	break;

    case nrc_JIS_Katakana:
	map_JIS_Katakana(code);
	break;

    case nrc_JIS_Roman:
	map_JIS_Roman(code);
	break;

    case nrc_Norwegian_Danish:
    case nrc_Norwegian_Danish2:
    case nrc_Norwegian_Danish3:
	map_NRCS_Norwegian_Danish(code);
	break;

    case nrc_Portugese:
	map_NRCS_Portuguese(code);
	break;

    case nrc_Russian:
	map_NRCS_Russian(code);
	break;

    case nrc_SCS_NRCS:		/* vt5xx - Serbo/Croatian */
	map_NRCS_Serbo_Croatian(code);
	break;

    case nrc_Spanish:
	map_NRCS_Spanish(code);
	break;

    case nrc_Swedish2:
    case nrc_Swedish:
	map_NRCS_Swedish(code);
	break;

    case nrc_Swiss:
	map_NRCS_Swiss(code);
	break;

    case nrc_Turkish:
	map_NRCS_Turkish(code);
	break;

    case nrc_DEC_Turkish_Supp:
	map_DEC_Turkish_Supp(code);
	break;

    case nrc_DEC_Cyrillic:
	map_DEC_Cyrillic(code);
	break;

    case nrc_ISO_Latin_1_Supp:
    case nrc_British_Latin_1:
    case nrc_French_Canadian2:
    case nrc_Unknown:
    case nrc_DEC_UPSS:
    default:			/* any character sets we don't recognize */
	break;
    }
    code &= 0x7f;		/* NRC in any case is 7-bit */
    TRACE(("->\t%s\n",
	   visibleUChar(code)));
    return code;
#undef MAP
#undef UNI
#undef XXX
}

/*
 * Translate a string to the display form.  This assumes the font has the
 * DEC graphic characters in cells 0-31, and otherwise is ISO-8859-1.
 */
Cardinal
xtermCharSetOut(XtermWidget xw, Cardinal length, DECNRCM_codes leftset)
{
    IChar *buf = xw->work.write_text;
    IChar *ptr = buf + length;
    IChar *s;
    TScreen *screen = TScreenOf(xw);
    Cardinal count = 0;
    DECNRCM_codes rightset = screen->gsets[(int) (screen->curgr)];
#if OPT_DEC_RECTOPS
    int sums = 0;
#endif

#define MAP(from, to) case from: chr = to; break;

#if OPT_WIDE_CHARS
#define UNI(from, to) case from: if (screen->utf8_nrc_mode) chr = to; break;
#define XXX(from, to) UNI(from, to)
#else
#define UNI(old, new) case new: chr = old; break;
#define XXX(from, to)		/* nothing */
#endif

    TRACE(("CHARSET-OUT GL=%s(G%d) GR=%s(G%d) SS%d\n\t%s\n",
	   visibleScsCode(leftset), screen->curgl,
	   visibleScsCode(rightset), screen->curgr,
	   screen->curss,
	   visibleIChars(buf, (size_t) length)));

    assert(length != 0);
#if OPT_DEC_RECTOPS
    if (length != 0 && length > xw->work.sizeof_sums) {
	xw->work.sizeof_sums += length + 80;
	xw->work.buffer_sums = realloc(xw->work.buffer_sums,
				       xw->work.sizeof_sums);
	xw->work.buffer_sets = realloc(xw->work.buffer_sets,
				       xw->work.sizeof_sums);
    }
    xw->work.write_sums = xw->work.buffer_sums;
#endif

    for (s = buf; s < ptr; ++s) {
	int eight = CharOf(*s);
	int seven = eight & 0x7f;
	DECNRCM_codes cs = (eight >= 128) ? rightset : leftset;
	int chr = eight;

	HandleUPSS(cs);

#if OPT_DEC_RECTOPS
	if (xw->work.buffer_sums != NULL && xw->work.buffer_sets != NULL) {
	    xw->work.buffer_sums[sums] = (Char) ((eight < 32 || eight > 255)
						 ? ANSI_ESC
						 : eight);
	    xw->work.buffer_sets[sums] = cs;
	    ++sums;
	}
#endif
	count++;
#if OPT_WIDE_CHARS
	/*
	 * This is only partly right - prevent inadvertent remapping of
	 * the replacement character and other non-8bit codes into bogus
	 * 8bit codes.
	 */
	if (screen->utf8_mode || screen->utf8_nrc_mode) {
	    if (*s > 255)
		continue;
	}
#endif
	if (*s < 32)
	    continue;

	switch (cs) {
	case nrc_DEC_UPSS:
	    break;

	case nrc_ISO_Latin_1_Supp:
	case nrc_British_Latin_1:
	case nrc_British:	/* United Kingdom set (or Latin 1)      */
	    if ((xw->flags & NATIONAL)
		|| (screen->vtXX_level <= 1)) {
		if ((xw->flags & NATIONAL)) {
		    chr = seven;
		}
		if (chr == 0x23) {
		    chr = XTERM_POUND;
#if OPT_WIDE_CHARS
		    if (screen->utf8_nrc_mode) {
			chr = 0xa3;
		    }
#endif
		}
	    }
	    break;

	case nrc_DEC_Alt_Chars:
	case nrc_DEC_Alt_Graphics:
	case nrc_ASCII:
	    break;

	case nrc_DEC_Spec_Graphic:
	    if (seven > 0x5f && seven <= 0x7e) {
#if OPT_WIDE_CHARS
		if (screen->utf8_mode || screen->utf8_nrc_mode)
		    chr = (int) dec2ucs(screen, (unsigned) (seven - 0x5f));
		else
#endif
		    chr = seven - 0x5f;
	    } else if (chr == 0x5f) {
		chr = 0;
	    } else {
		chr = seven;
	    }
	    break;

	case nrc_DEC_Supp:
	case nrc_DEC_Supp_Graphic:
	    map_DEC_Supp_Graphic(chr = seven, chr = eight);
	    break;

	case nrc_DEC_Technical:
	    map_DEC_Technical(chr = seven);
	    break;

	case nrc_Dutch:
	    map_NRCS_Dutch(chr = seven);
	    break;

	case nrc_Finnish:
	case nrc_Finnish2:
	    map_NRCS_Finnish(chr = seven);
	    break;

	case nrc_French:
	case nrc_French2:
	    map_NRCS_French(chr = seven);
	    break;

	case nrc_French_Canadian:
	case nrc_French_Canadian2:
	    map_NRCS_French_Canadian(chr = seven);
	    break;

	case nrc_German:
	    map_NRCS_German(chr = seven);
	    break;

	case nrc_Greek:
	    map_NRCS_Greek(chr = seven);	/* FIXME - ELOT? */
	    break;

	case nrc_DEC_Greek_Supp:
	    map_DEC_Greek_Supp(chr = seven);
	    break;

	case nrc_ISO_Greek_Supp:
	    map_ISO_Greek_Supp(chr = seven);
	    break;

	case nrc_DEC_Hebrew_Supp:
	    map_DEC_Hebrew_Supp(chr = seven);
	    break;

	case nrc_Hebrew:
	    map_NRCS_Hebrew(chr = seven);
	    break;

	case nrc_ISO_Hebrew_Supp:
	    map_ISO_Hebrew(chr = seven);
	    break;

	case nrc_Italian:
	    map_NRCS_Italian(chr = seven);
	    break;

	case nrc_ISO_Latin_2_Supp:
	    map_ISO_Latin_2(chr = seven);
	    break;

	case nrc_ISO_Latin_5_Supp:
	    map_ISO_Latin_5(chr = seven);
	    break;

	case nrc_ISO_Latin_Cyrillic:
	    map_ISO_Latin_Cyrillic(chr = seven);
	    break;

	case nrc_JIS_Katakana:
	    map_JIS_Katakana(chr = seven);
	    break;

	case nrc_JIS_Roman:
	    map_JIS_Roman(chr = seven);
	    break;

	case nrc_Norwegian_Danish:
	case nrc_Norwegian_Danish2:
	case nrc_Norwegian_Danish3:
	    map_NRCS_Norwegian_Danish(chr = seven);
	    break;

	case nrc_Portugese:
	    map_NRCS_Portuguese(chr = seven);
	    break;

	case nrc_Russian:
	    map_NRCS_Russian(chr = seven);
	    break;

	case nrc_SCS_NRCS:	/* vt5xx - Serbo/Croatian */
	    map_NRCS_Serbo_Croatian(chr = seven);
	    break;

	case nrc_Spanish:
	    map_NRCS_Spanish(chr = seven);
	    break;

	case nrc_Swedish2:
	case nrc_Swedish:
	    map_NRCS_Swedish(chr = seven);
	    break;

	case nrc_Swiss:
	    map_NRCS_Swiss(chr = seven);
	    break;

	case nrc_Turkish:
	    map_NRCS_Turkish(chr = seven);
	    break;

	case nrc_DEC_Turkish_Supp:
	    map_DEC_Turkish_Supp(chr = seven);
	    break;

	case nrc_DEC_Cyrillic:
	    map_DEC_Cyrillic(chr = seven);
	    break;

	case nrc_Unknown:
	default:		/* any character sets we don't recognize */
	    break;
	}
	/*
	 * The state machine already treated DEL as a nonprinting and
	 * nonspacing character.  If we have DEL now, remove it.
	 */
	if (chr == ANSI_DEL && isSevenBit(cs)) {
	    IChar *s1;
	    --ptr;
	    for (s1 = s; s1 < ptr; ++s1) {
		s1[0] = s1[1];
	    }
	    --count;
#if OPT_DEC_RECTOPS
	    --sums;
#endif
	} else {
	    if (eight >= 128 && chr < 128 && chr > 32)
		chr |= 128;
	    *s = (IChar) chr;
	}
    }
    TRACE(("%d\t%s\n",
	   count,
	   visibleIChars(buf, (size_t) length)));
    return count;
#undef MAP
#undef UNI
#undef XXX
}

#if OPT_DEC_RECTOPS
/*
 * Given a mapped character, e.g., a Unicode value returned by xtermCharSetIn,
 * match it against the current GL/GR selection and return the corresponding
 * DEC internal character-set code for DECRQCRA.
 *
 * A hardware terminal presumably stores the original and mapped characters,
 * as well as the character set which was selected at that time  Doing that
 * in xterm adds a couple of bytes to every cell.
 */
int
xtermCharSetDec(XtermWidget xw, IChar chr, DECNRCM_codes cs)
{
#define MAP(from, to) case from: result = to; break;

#define DFTMAP()      result = (actual | 0x80)
#define DFT_94(chr)   result = ((actual) & 0x7f)
#define DFT_96(chr)   result = ((actual) | 0x80)

#if OPT_WIDE_CHARS
#define UNI(from, to) case from: if (screen->utf8_nrc_mode) result = to; break;
#define XXX(from, to) UNI(from, to)
#else
#define UNI(old, new) case new: result = old; break;
#define XXX(from, to)		/* nothing */
#endif

    int result;

    if (chr < 0x20
#if OPT_WIDE_CHARS
	|| chr > 0xff
#endif
	) {
	result = ANSI_ESC;
    } else {
	Boolean isSeven = isSevenBit(cs);
	TScreen *screen = TScreenOf(xw);

	result = -1;

	HandleUPSS(cs);

	if (chr == 0xa0 && isSeven) {
	    result = ANSI_ESC;
	} else if (chr == ANSI_SPA && isSeven) {
	    result = ANSI_SPA;
	} else if ((chr == ANSI_DEL || chr == 0xff) && isSeven) {
	    result = 0;
	} else {
	    int actual = (int) chr;
	    chr &= 0x7f;

	    switch (cs) {
	    case nrc_DEC_Alt_Chars:
	    case nrc_DEC_Alt_Graphics:
	    case nrc_ASCII:
		result = (int) chr;
		break;

	    case nrc_British:
		if (chr >= 0xa0 && chr < 0xff) {
		    if (chr == 0x23)
			chr = 0xA3;
		    result = (int) chr;
		}
		break;

	    case nrc_DEC_Cyrillic:
		unmap_DEC_Cyrillic(chr, DFT_94(chr));
		break;

	    case nrc_DEC_Spec_Graphic:
		unmap_DEC_Spec_Graphic(chr, DFT_94(chr));
		break;

	    case nrc_DEC_Supp:
		/* FALLTHRU */
	    case nrc_DEC_Supp_Graphic:
		unmap_DEC_Supp_Graphic(chr, DFTMAP());
		break;

	    case nrc_DEC_Technical:
		unmap_DEC_Technical(chr, DFTMAP());
		break;

	    case nrc_Dutch:
		unmap_NRCS_Dutch(chr, DFT_94(chr));
		break;

	    case nrc_Finnish:
	    case nrc_Finnish2:
		unmap_NRCS_Finnish(chr, DFT_94(chr));
		break;

	    case nrc_French:
	    case nrc_French2:
		unmap_NRCS_French(chr, DFT_94(chr));
		break;

	    case nrc_French_Canadian:
	    case nrc_French_Canadian2:
		unmap_NRCS_French_Canadian(chr, DFT_94(chr));
		break;

	    case nrc_German:
		unmap_NRCS_German(chr, DFT_94(chr));
		break;

	    case nrc_Greek:
		unmap_NRCS_Greek(chr, DFT_94(chr));
		break;

	    case nrc_DEC_Greek_Supp:
		unmap_DEC_Greek_Supp(chr, DFTMAP());
		break;

	    case nrc_ISO_Greek_Supp:
		unmap_ISO_Greek_Supp(chr, DFTMAP());
		break;

	    case nrc_DEC_Hebrew_Supp:
		unmap_DEC_Hebrew_Supp(chr, DFTMAP());
		break;

	    case nrc_Hebrew:
		unmap_NRCS_Hebrew(chr, DFT_94(chr));
		break;

	    case nrc_ISO_Hebrew_Supp:
		unmap_ISO_Hebrew(chr, DFTMAP());
		break;

	    case nrc_Italian:
		unmap_NRCS_Italian(chr, DFT_94(chr));
		break;

	    case nrc_JIS_Katakana:
		unmap_JIS_Katakana(chr, DFT_94(chr));
		break;

	    case nrc_JIS_Roman:
		unmap_JIS_Roman(chr, DFT_94(chr));
		break;

	    case nrc_ISO_Latin_1_Supp:
		unmap_ISO_Latin_1(chr, DFTMAP());
		break;

	    case nrc_ISO_Latin_2_Supp:
		unmap_ISO_Latin_2(chr, DFTMAP());
		break;

	    case nrc_ISO_Latin_5_Supp:
		unmap_ISO_Latin_5(chr, DFTMAP());
		break;

	    case nrc_ISO_Latin_Cyrillic:
		unmap_ISO_Latin_Cyrillic(chr, DFTMAP());
		break;

	    case nrc_Norwegian_Danish:
	    case nrc_Norwegian_Danish2:
	    case nrc_Norwegian_Danish3:
		unmap_NRCS_Norwegian_Danish(chr, DFT_94(chr));
		break;

	    case nrc_Portugese:
		unmap_NRCS_Portuguese(chr, DFT_94(chr));
		break;

	    case nrc_Russian:
		unmap_NRCS_Russian(chr, DFT_94(chr));
		break;

	    case nrc_SCS_NRCS:
		unmap_NRCS_Serbo_Croatian(chr, DFT_94(chr));
		break;

	    case nrc_Spanish:
		unmap_NRCS_Spanish(chr, DFT_94(chr));
		break;

	    case nrc_Swedish:
	    case nrc_Swedish2:
		unmap_NRCS_Swedish(chr, DFT_94(chr));
		break;

	    case nrc_Swiss:
		unmap_NRCS_Swiss(chr, DFT_94(chr));
		break;

	    case nrc_DEC_Turkish_Supp:
		unmap_DEC_Turkish_Supp(chr, DFTMAP());
		break;

	    case nrc_Turkish:
		unmap_NRCS_Turkish(chr, DFT_94(chr));
		break;

	    case nrc_British_Latin_1:
	    case nrc_Unknown:
	    case nrc_DEC_UPSS:
	    default:		/* anything we cannot unmap */
		break;
	    }
	    if (result < 0) {
		if (isSeven) {
		    DFT_94(chr);
		} else {
		    DFT_96(chr);
		}
	    }
	}
    }
    return result;
#undef MAP
#undef UNI
#undef XXX
}
#endif /* OPT_DEC_RECTOPS */
