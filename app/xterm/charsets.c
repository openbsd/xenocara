/* $XTermId: charsets.c,v 1.105 2019/02/11 00:41:13 tom Exp $ */

/*
 * Copyright 1998-2018,2019 by Thomas E. Dickey
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

#include <xterm.h>
#include <data.h>
#include <fontutils.h>

#include <X11/keysym.h>

#define BLANK ' '
#define UNDEF 0x2e2e		/* rendered as a backwards "?" (alt: 0x2426) */

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
 *
 * According to
 *  Digital ANSI-Compliant Printing Protocol
 *  Level 2 Programming Reference Manual
 *  EK–PPLV2–PM. B01
 *
 * the supplementary character sets Greek, Hebrew, Latin-5 and Latin/Cyrillic
 * are standardized by ISO:
 *  ISO Greek is 8859-7
 *  ISO Hebrew is 8859-8
 *  ISO Latin-5 is 8859-9
 *  ISO Latin/Cyrillic is 8859-5
 *
 * These are derived from the data at
 *  ftp://www.unicode.org/Public/MAPPINGS/ISO8859/
 *
 * Note: the "figure A-xx" comments refer to EK–PPLV2–PM. 
 */

/*
 * A "codepage" is treated different from the NRC mode:  it is always enabled.
 * Reuse the UNI() macros by temporarily setting its state.
 */
#if OPT_WIDE_CHARS
#define begin_CODEPAGE() \
	if (!(xw->flags & NATIONAL)) { \
	    screen->utf8_nrc_mode++; \
	}
#define end_CODEPAGE() \
	if (!(xw->flags & NATIONAL)) { \
	    screen->utf8_nrc_mode--; \
	}
#else
#define begin_CODEPAGE()	/* nothing */
#define end_CODEPAGE()		/* nothing */
#endif

/*
 * xterm's original implementation of NRCS in 1998 was before Unicode became
 * prevalent.  Most of the necessary mappings could be done using definitions
 * from X11/keysymdef.h
 */
#define map_NRCS_Dutch(code) \
	switch (code) { \
	    MAP(0x23, XK_sterling); \
	    MAP(0x40, XK_threequarters); \
	    UNI(0x5b, 0x0133); /* ij ligature */ \
	    MAP(0x5c, XK_onehalf); \
	    MAP(0x5d, XK_bar); \
	    MAP(0x7b, XK_diaeresis); \
	    UNI(0x7c, 0x0192); /* florin */ \
	    MAP(0x7d, XK_onequarter); \
	    MAP(0x7e, XK_acute); \
	}

#define map_NRCS_Finnish(code) \
	switch (code) { \
	    MAP(0x5b, XK_Adiaeresis); \
	    MAP(0x5c, XK_Odiaeresis); \
	    MAP(0x5d, XK_Aring); \
	    MAP(0x5e, XK_Udiaeresis); \
	    MAP(0x60, XK_eacute); \
	    MAP(0x7b, XK_adiaeresis); \
	    MAP(0x7c, XK_odiaeresis); \
	    MAP(0x7d, XK_aring); \
	    MAP(0x7e, XK_udiaeresis); \
	}

#define map_NRCS_French(code) \
	switch (code) { \
	    MAP(0x23, XK_sterling); \
	    MAP(0x40, XK_agrave); \
	    MAP(0x5b, XK_degree); \
	    MAP(0x5c, XK_ccedilla); \
	    MAP(0x5d, XK_section); \
	    MAP(0x7b, XK_eacute); \
	    MAP(0x7c, XK_ugrave); \
	    MAP(0x7d, XK_egrave); \
	    MAP(0x7e, XK_diaeresis); \
	}

#define map_NRCS_French_Canadian(code) \
	switch (code) { \
	    MAP(0x40, XK_agrave); \
	    MAP(0x5b, XK_acircumflex); \
	    MAP(0x5c, XK_ccedilla); \
	    MAP(0x5d, XK_ecircumflex); \
	    MAP(0x5e, XK_icircumflex); \
	    MAP(0x60, XK_ocircumflex); \
	    MAP(0x7b, XK_eacute); \
	    MAP(0x7c, XK_ugrave); \
	    MAP(0x7d, XK_egrave); \
	    MAP(0x7e, XK_ucircumflex); \
	}

#define map_NRCS_German(code) \
	switch (code) { \
	    MAP(0x40, XK_section); \
	    MAP(0x5b, XK_Adiaeresis); \
	    MAP(0x5c, XK_Odiaeresis); \
	    MAP(0x5d, XK_Udiaeresis); \
	    MAP(0x7b, XK_adiaeresis); \
	    MAP(0x7c, XK_odiaeresis); \
	    MAP(0x7d, XK_udiaeresis); \
	    MAP(0x7e, XK_ssharp); \
	}

#define map_NRCS_Italian(code) \
	switch (code) { \
	    MAP(0x23, XK_sterling); \
	    MAP(0x40, XK_section); \
	    MAP(0x5b, XK_degree); \
	    MAP(0x5c, XK_ccedilla); \
	    MAP(0x5d, XK_eacute); \
	    MAP(0x60, XK_ugrave); \
	    MAP(0x7b, XK_agrave); \
	    MAP(0x7c, XK_ograve); \
	    MAP(0x7d, XK_egrave); \
	    MAP(0x7e, XK_igrave); \
	}

#define map_NRCS_Norwegian_Danish(code) \
	switch (code) { \
	    MAP(0x40, XK_Adiaeresis); \
	    MAP(0x5b, XK_AE); \
	    MAP(0x5c, XK_Ooblique); \
	    MAP(0x5d, XK_Aring); \
	    MAP(0x5e, XK_Udiaeresis); \
	    MAP(0x60, XK_adiaeresis); \
	    MAP(0x7b, XK_ae); \
	    MAP(0x7c, XK_oslash); \
	    MAP(0x7d, XK_aring); \
	    MAP(0x7e, XK_udiaeresis); \
	}

#define map_NRCS_Portuguese(code) \
	switch (code) { \
	    MAP(0x5b, XK_Atilde); \
	    MAP(0x5c, XK_Ccedilla); \
	    MAP(0x5d, XK_Otilde); \
	    MAP(0x7b, XK_atilde); \
	    MAP(0x7c, XK_ccedilla); \
	    MAP(0x7d, XK_otilde); \
	}

#define map_NRCS_Spanish(code) \
	switch (code) { \
	    MAP(0x23, XK_sterling); \
	    MAP(0x40, XK_section); \
	    MAP(0x5b, XK_exclamdown); \
	    MAP(0x5c, XK_Ntilde); \
	    MAP(0x5d, XK_questiondown); \
	    MAP(0x7b, XK_degree); \
	    MAP(0x7c, XK_ntilde); \
	    MAP(0x7d, XK_ccedilla); \
	}

#define map_NRCS_Swedish(code) \
	switch (code) { \
	    MAP(0x40, XK_Eacute); \
	    MAP(0x5b, XK_Adiaeresis); \
	    MAP(0x5c, XK_Odiaeresis); \
	    MAP(0x5d, XK_Aring); \
	    MAP(0x5e, XK_Udiaeresis); \
	    MAP(0x60, XK_eacute); \
	    MAP(0x7b, XK_adiaeresis); \
	    MAP(0x7c, XK_odiaeresis); \
	    MAP(0x7d, XK_aring); \
	    MAP(0x7e, XK_udiaeresis); \
	}

#define map_NRCS_Swiss(code) \
	switch (code) { \
	    MAP(0x23, XK_ugrave); \
	    MAP(0x40, XK_agrave); \
	    MAP(0x5b, XK_eacute); \
	    MAP(0x5c, XK_ccedilla); \
	    MAP(0x5d, XK_ecircumflex); \
	    MAP(0x5e, XK_icircumflex); \
	    MAP(0x5f, XK_egrave); \
	    MAP(0x60, XK_ocircumflex); \
	    MAP(0x7b, XK_adiaeresis); \
	    MAP(0x7c, XK_odiaeresis); \
	    MAP(0x7d, XK_udiaeresis); \
	    MAP(0x7e, XK_ucircumflex); \
	}

/*
 * Unlike NRCS, which splices a few characters onto ASCII, the supplementary
 * character sets are complete, normally mapped to GR.  Most of these mappings
 * rely upon glyphs not found in ISO-8859-1.  We can display most of those
 * using Unicode, thereby supporting specialized applications that use SCS
 * with luit, subject to the limitation that select/paste will give meaningless
 * results in terms of the application which uses these mappings.
 *
 * Since the codepages introduced with VT320, etc, use 8-bit encodings, there
 * is no plausible argument to be made that these mappings "use" UTF-8, even
 * though there is a hidden step in the terminal emulator which relies upon
 * UTF-8.
 */
#define map_DEC_Supp_Graphic(code,dft) \
	begin_CODEPAGE(); \
	switch (code) { \
	    XXX(0x24, UNDEF); \
	    XXX(0x26, UNDEF); \
	    MAP(0x28, 0xa4); \
	    XXX(0x2c, UNDEF); \
	    XXX(0x2d, UNDEF); \
	    XXX(0x2e, UNDEF); \
	    XXX(0x2f, UNDEF); \
	    XXX(0x34, UNDEF); \
	    XXX(0x38, UNDEF); \
	    XXX(0x3e, UNDEF); \
	    XXX(0x50, UNDEF); \
	    UNI(0x57, 0x0152); \
	    MAP(0x5d, 0x0178); \
	    XXX(0x5e, UNDEF); \
	    XXX(0x70, UNDEF); \
	    UNI(0x77, 0x0153); \
	    MAP(0x7d, 0xff); \
	    XXX(0x7e, UNDEF); \
	    XXX(0x7f, UNDEF); \
	    default: dft; break; \
	} \
	end_CODEPAGE()

#if OPT_WIDE_CHARS
	/* derived from http://www.vt100.net/charsets/technical.html */
#define map_DEC_Technical(code) \
	begin_CODEPAGE(); \
	switch (code) { \
	    UNI(0x21, 0x23b7);	/* RADICAL SYMBOL BOTTOM Centred left to right, so that it joins up with 02/02 */ \
	    UNI(0x22, 0x250c);	/* BOX DRAWINGS LIGHT DOWN AND RIGHT */ \
	    UNI(0x23, 0x2500);	/* BOX DRAWINGS LIGHT HORIZONTAL */ \
	    UNI(0x24, 0x2320);	/* TOP HALF INTEGRAL with the proviso that the stem is vertical, to join with 02/06 */ \
	    UNI(0x25, 0x2321);	/* BOTTOM HALF INTEGRAL with the proviso above. */ \
	    UNI(0x26, 0x2502);	/* BOX DRAWINGS LIGHT VERTICAL */ \
	    UNI(0x27, 0x23a1);	/* LEFT SQUARE BRACKET UPPER CORNER Joins vertically to 02/06, 02/08. Doesn't join to its right. */ \
	    UNI(0x28, 0x23a3);	/* LEFT SQUARE BRACKET LOWER CORNER Joins vertically to 02/06, 02/07. Doesn't join to its right. */ \
	    UNI(0x29, 0x23a4);	/* RIGHT SQUARE BRACKET UPPER CORNER Joins vertically to 026, 02a. Doesn't join to its left. */ \
	    UNI(0x2a, 0x23a6);	/* RIGHT SQUARE BRACKET LOWER CORNER Joins vertically to 026, 029. Doesn't join to its left. */ \
	    UNI(0x2b, 0x23a7);	/* LEFT CURLY BRACKET UPPER HOOK Joins vertically to 026, 02c, 02/15. Doesn't join to its right. */ \
	    UNI(0x2c, 0x23a9);	/* LEFT CURLY BRACKET LOWER HOOK Joins vertically to 026, 02b, 02/15. Doesn't join to its right. */ \
	    UNI(0x2d, 0x23ab);	/* RIGHT CURLY BRACKET UPPER HOOK Joins vertically to 026, 02e, 03/00. Doesn't join to its left. */ \
	    UNI(0x2e, 0x23ad);	/* RIGHT CURLY BRACKET LOWER HOOK Joins vertically to 026, 02d, 03/00. Doesn't join to its left. */ \
	    UNI(0x2f, 0x23a8);	/* LEFT CURLY BRACKET MIDDLE PIECE Joins vertically to 026, 02b, 02c. */ \
	    UNI(0x30, 0x23ac);	/* RIGHT CURLY BRACKET MIDDLE PIECE Joins vertically to 02/06, 02d, 02e. */ \
	    XXX(0x31, UNDEF);	/* Top Left Sigma. Joins to right with 02/03, 03/05. Joins diagonally below right with 03/03, 03/07. */ \
	    XXX(0x32, UNDEF);	/* Bottom Left Sigma. Joins to right with 02/03, 03/06. Joins diagonally above right with 03/04, 03/07. */ \
	    XXX(0x33, UNDEF);	/* Top Diagonal Sigma. Line for joining 03/01 to 03/04 or 03/07. */ \
	    XXX(0x34, UNDEF);	/* Bottom Diagonal Sigma. Line for joining 03/02 to 03/03 or 03/07. */ \
	    XXX(0x35, UNDEF);	/* Top Right Sigma. Joins to left with 02/03, 03/01. */ \
	    XXX(0x36, UNDEF);	/* Bottom Right Sigma. Joins to left with 02/03, 03/02. */ \
	    XXX(0x37, UNDEF);	/* Middle Sigma. Joins diagonally with 03/01, 03/02, 03/03, 03/04. */ \
	    XXX(0x38, UNDEF);	/* undefined */ \
	    XXX(0x39, UNDEF);	/* undefined */ \
	    XXX(0x3a, UNDEF);	/* undefined */ \
	    XXX(0x3b, UNDEF);	/* undefined */ \
	    UNI(0x3c, 0x2264);	/* LESS-THAN OR EQUAL TO */ \
	    UNI(0x3d, 0x2260);	/* NOT EQUAL TO */ \
	    UNI(0x3e, 0x2265);	/* GREATER-THAN OR EQUAL TO */ \
	    UNI(0x3f, 0x222B);	/* INTEGRAL */ \
	    UNI(0x40, 0x2234);	/* THEREFORE */ \
	    UNI(0x41, 0x221d);	/* PROPORTIONAL TO */ \
	    UNI(0x42, 0x221e);	/* INFINITY */ \
	    UNI(0x43, 0x00f7);	/* DIVISION SIGN */ \
	    UNI(0x44, 0x0394);	/* GREEK CAPITAL DELTA */ \
	    UNI(0x45, 0x2207);	/* NABLA */ \
	    UNI(0x46, 0x03a6);	/* GREEK CAPITAL LETTER PHI */ \
	    UNI(0x47, 0x0393);	/* GREEK CAPITAL LETTER GAMMA */ \
	    UNI(0x48, 0x223c);	/* TILDE OPERATOR */ \
	    UNI(0x49, 0x2243);	/* ASYMPTOTICALLY EQUAL TO */ \
	    UNI(0x4a, 0x0398);	/* GREEK CAPITAL LETTER THETA */ \
	    UNI(0x4b, 0x00d7);	/* MULTIPLICATION SIGN */ \
	    UNI(0x4c, 0x039b);	/* GREEK CAPITAL LETTER LAMDA */ \
	    UNI(0x4d, 0x21d4);	/* LEFT RIGHT DOUBLE ARROW */ \
	    UNI(0x4e, 0x21d2);	/* RIGHTWARDS DOUBLE ARROW */ \
	    UNI(0x4f, 0x2261);	/* IDENTICAL TO */ \
	    UNI(0x50, 0x03a0);	/* GREEK CAPITAL LETTER PI */ \
	    UNI(0x51, 0x03a8);	/* GREEK CAPITAL LETTER PSI */ \
	    UNI(0x52, UNDEF);	/* undefined */ \
	    UNI(0x53, 0x03a3);	/* GREEK CAPITAL LETTER SIGMA */ \
	    XXX(0x54, UNDEF);	/* undefined */ \
	    XXX(0x55, UNDEF);	/* undefined */ \
	    UNI(0x56, 0x221a);	/* SQUARE ROOT */ \
	    UNI(0x57, 0x03a9);	/* GREEK CAPITAL LETTER OMEGA */ \
	    UNI(0x58, 0x039e);	/* GREEK CAPITAL LETTER XI */ \
	    UNI(0x59, 0x03a5);	/* GREEK CAPITAL LETTER UPSILON */ \
	    UNI(0x5a, 0x2282);	/* SUBSET OF */ \
	    UNI(0x5b, 0x2283);	/* SUPERSET OF */ \
	    UNI(0x5c, 0x2229);	/* INTERSECTION */ \
	    UNI(0x5d, 0x222a);	/* UNION */ \
	    UNI(0x5e, 0x2227);	/* LOGICAL AND */ \
	    UNI(0x5f, 0x2228);	/* LOGICAL OR */ \
	    UNI(0x60, 0x00ac);	/* NOT SIGN */ \
	    UNI(0x61, 0x03b1);	/* GREEK SMALL LETTER ALPHA */ \
	    UNI(0x62, 0x03b2);	/* GREEK SMALL LETTER BETA */ \
	    UNI(0x63, 0x03c7);	/* GREEK SMALL LETTER CHI */ \
	    UNI(0x64, 0x03b4);	/* GREEK SMALL LETTER DELTA */ \
	    UNI(0x65, 0x03b5);	/* GREEK SMALL LETTER EPSILON */ \
	    UNI(0x66, 0x03c6);	/* GREEK SMALL LETTER PHI */ \
	    UNI(0x67, 0x03b3);	/* GREEK SMALL LETTER GAMMA */ \
	    UNI(0x68, 0x03b7);	/* GREEK SMALL LETTER ETA */ \
	    UNI(0x69, 0x03b9);	/* GREEK SMALL LETTER IOTA */ \
	    UNI(0x6a, 0x03b8);	/* GREEK SMALL LETTER THETA */ \
	    UNI(0x6b, 0x03ba);	/* GREEK SMALL LETTER KAPPA */ \
	    UNI(0x6c, 0x03bb);	/* GREEK SMALL LETTER LAMDA */ \
	    XXX(0x6d, UNDEF);	/* undefined */ \
	    UNI(0x6e, 0x03bd);	/* GREEK SMALL LETTER NU */ \
	    UNI(0x6f, 0x2202);	/* PARTIAL DIFFERENTIAL */ \
	    UNI(0x70, 0x03c0);	/* GREEK SMALL LETTER PI */ \
	    UNI(0x71, 0x03c8);	/* GREEK SMALL LETTER PSI */ \
	    UNI(0x72, 0x03c1);	/* GREEK SMALL LETTER RHO */ \
	    UNI(0x73, 0x03c3);	/* GREEK SMALL LETTER SIGMA */ \
	    UNI(0x74, 0x03c4);	/* GREEK SMALL LETTER TAU */ \
	    XXX(0x75, UNDEF);	/* undefined */ \
	    UNI(0x76, 0x0192);	/* LATIN SMALL LETTER F WITH HOOK Probably chosen for its meaning of "function" */ \
	    UNI(0x77, 0x03c9);	/* GREEK SMALL LETTER OMEGA */ \
	    UNI(0x78, 0x03bE);	/* GREEK SMALL LETTER XI */ \
	    UNI(0x79, 0x03c5);	/* GREEK SMALL LETTER UPSILON */ \
	    UNI(0x7a, 0x03b6);	/* GREEK SMALL LETTER ZETA */ \
	    UNI(0x7b, 0x2190);	/* LEFTWARDS ARROW */ \
	    UNI(0x7c, 0x2191);	/* UPWARDS ARROW */ \
	    UNI(0x7d, 0x2192);	/* RIGHTWARDS ARROW */ \
	    UNI(0x7e, 0x2193);	/* DOWNWARDS ARROW */ \
	} \
	end_CODEPAGE()
	/* ISO Latin/Cyrillic is 8859-5 */
#define map_ISO_Latin_Cyrillic(code) \
	begin_CODEPAGE(); \
	switch (code) { \
	    UNI(0x20, 0x00a0);	/* NO-BREAK SPACE */ \
	    UNI(0x21, 0x0401);	/* CYRILLIC CAPITAL LETTER IO */ \
	    UNI(0x22, 0x0402);	/* CYRILLIC CAPITAL LETTER DJE */ \
	    UNI(0x23, 0x0403);	/* CYRILLIC CAPITAL LETTER GJE */ \
	    UNI(0x24, 0x0404);	/* CYRILLIC CAPITAL LETTER UKRAINIAN IE */ \
	    UNI(0x25, 0x0405);	/* CYRILLIC CAPITAL LETTER DZE */ \
	    UNI(0x26, 0x0406);	/* CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I */ \
	    UNI(0x27, 0x0407);	/* CYRILLIC CAPITAL LETTER YI */ \
	    UNI(0x28, 0x0408);	/* CYRILLIC CAPITAL LETTER JE */ \
	    UNI(0x29, 0x0409);	/* CYRILLIC CAPITAL LETTER LJE */ \
	    UNI(0x2a, 0x040a);	/* CYRILLIC CAPITAL LETTER NJE */ \
	    UNI(0x2b, 0x040b);	/* CYRILLIC CAPITAL LETTER TSHE */ \
	    UNI(0x2c, 0x040c);	/* CYRILLIC CAPITAL LETTER KJE */ \
	    UNI(0x2d, 0x00ad);	/* SOFT HYPHEN */ \
	    UNI(0x2e, 0x040e);	/* CYRILLIC CAPITAL LETTER SHORT U */ \
	    UNI(0x2f, 0x040f);	/* CYRILLIC CAPITAL LETTER DZHE */ \
	    UNI(0x30, 0x0410);	/* CYRILLIC CAPITAL LETTER A */ \
	    UNI(0x31, 0x0411);	/* CYRILLIC CAPITAL LETTER BE */ \
	    UNI(0x32, 0x0412);	/* CYRILLIC CAPITAL LETTER VE */ \
	    UNI(0x33, 0x0413);	/* CYRILLIC CAPITAL LETTER GHE */ \
	    UNI(0x34, 0x0414);	/* CYRILLIC CAPITAL LETTER DE */ \
	    UNI(0x35, 0x0415);	/* CYRILLIC CAPITAL LETTER IE */ \
	    UNI(0x36, 0x0416);	/* CYRILLIC CAPITAL LETTER ZHE */ \
	    UNI(0x37, 0x0417);	/* CYRILLIC CAPITAL LETTER ZE */ \
	    UNI(0x38, 0x0418);	/* CYRILLIC CAPITAL LETTER I */ \
	    UNI(0x39, 0x0419);	/* CYRILLIC CAPITAL LETTER SHORT I */ \
	    UNI(0x3a, 0x041a);	/* CYRILLIC CAPITAL LETTER KA */ \
	    UNI(0x3b, 0x041b);	/* CYRILLIC CAPITAL LETTER EL */ \
	    UNI(0x3c, 0x041c);	/* CYRILLIC CAPITAL LETTER EM */ \
	    UNI(0x3d, 0x041d);	/* CYRILLIC CAPITAL LETTER EN */ \
	    UNI(0x3e, 0x041e);	/* CYRILLIC CAPITAL LETTER O */ \
	    UNI(0x3f, 0x041f);	/* CYRILLIC CAPITAL LETTER PE */ \
	    UNI(0x40, 0x0420);	/* CYRILLIC CAPITAL LETTER ER */ \
	    UNI(0x41, 0x0421);	/* CYRILLIC CAPITAL LETTER ES */ \
	    UNI(0x42, 0x0422);	/* CYRILLIC CAPITAL LETTER TE */ \
	    UNI(0x43, 0x0423);	/* CYRILLIC CAPITAL LETTER U */ \
	    UNI(0x44, 0x0424);	/* CYRILLIC CAPITAL LETTER EF */ \
	    UNI(0x45, 0x0425);	/* CYRILLIC CAPITAL LETTER HA */ \
	    UNI(0x46, 0x0426);	/* CYRILLIC CAPITAL LETTER TSE */ \
	    UNI(0x47, 0x0427);	/* CYRILLIC CAPITAL LETTER CHE */ \
	    UNI(0x48, 0x0428);	/* CYRILLIC CAPITAL LETTER SHA */ \
	    UNI(0x49, 0x0429);	/* CYRILLIC CAPITAL LETTER SHCHA */ \
	    UNI(0x4a, 0x042a);	/* CYRILLIC CAPITAL LETTER HARD SIGN */ \
	    UNI(0x4b, 0x042b);	/* CYRILLIC CAPITAL LETTER YERU */ \
	    UNI(0x4c, 0x042c);	/* CYRILLIC CAPITAL LETTER SOFT SIGN */ \
	    UNI(0x4d, 0x042d);	/* CYRILLIC CAPITAL LETTER E */ \
	    UNI(0x4e, 0x042e);	/* CYRILLIC CAPITAL LETTER YU */ \
	    UNI(0x4f, 0x042f);	/* CYRILLIC CAPITAL LETTER YA */ \
	    UNI(0x50, 0x0430);	/* CYRILLIC SMALL LETTER A */ \
	    UNI(0x51, 0x0431);	/* CYRILLIC SMALL LETTER BE */ \
	    UNI(0x52, 0x0432);	/* CYRILLIC SMALL LETTER VE */ \
	    UNI(0x53, 0x0433);	/* CYRILLIC SMALL LETTER GHE */ \
	    UNI(0x54, 0x0434);	/* CYRILLIC SMALL LETTER DE */ \
	    UNI(0x55, 0x0435);	/* CYRILLIC SMALL LETTER IE */ \
	    UNI(0x56, 0x0436);	/* CYRILLIC SMALL LETTER ZHE */ \
	    UNI(0x57, 0x0437);	/* CYRILLIC SMALL LETTER ZE */ \
	    UNI(0x58, 0x0438);	/* CYRILLIC SMALL LETTER I */ \
	    UNI(0x59, 0x0439);	/* CYRILLIC SMALL LETTER SHORT I */ \
	    UNI(0x5a, 0x043a);	/* CYRILLIC SMALL LETTER KA */ \
	    UNI(0x5b, 0x043b);	/* CYRILLIC SMALL LETTER EL */ \
	    UNI(0x5c, 0x043c);	/* CYRILLIC SMALL LETTER EM */ \
	    UNI(0x5d, 0x043d);	/* CYRILLIC SMALL LETTER EN */ \
	    UNI(0x5e, 0x043e);	/* CYRILLIC SMALL LETTER O */ \
	    UNI(0x5f, 0x043f);	/* CYRILLIC SMALL LETTER PE */ \
	    UNI(0x60, 0x0440);	/* CYRILLIC SMALL LETTER ER */ \
	    UNI(0x61, 0x0441);	/* CYRILLIC SMALL LETTER ES */ \
	    UNI(0x62, 0x0442);	/* CYRILLIC SMALL LETTER TE */ \
	    UNI(0x63, 0x0443);	/* CYRILLIC SMALL LETTER U */ \
	    UNI(0x64, 0x0444);	/* CYRILLIC SMALL LETTER EF */ \
	    UNI(0x65, 0x0445);	/* CYRILLIC SMALL LETTER HA */ \
	    UNI(0x66, 0x0446);	/* CYRILLIC SMALL LETTER TSE */ \
	    UNI(0x67, 0x0447);	/* CYRILLIC SMALL LETTER CHE */ \
	    UNI(0x68, 0x0448);	/* CYRILLIC SMALL LETTER SHA */ \
	    UNI(0x69, 0x0449);	/* CYRILLIC SMALL LETTER SHCHA */ \
	    UNI(0x6a, 0x044a);	/* CYRILLIC SMALL LETTER HARD SIGN */ \
	    UNI(0x6b, 0x044b);	/* CYRILLIC SMALL LETTER YERU */ \
	    UNI(0x6c, 0x044c);	/* CYRILLIC SMALL LETTER SOFT SIGN */ \
	    UNI(0x6d, 0x044d);	/* CYRILLIC SMALL LETTER E */ \
	    UNI(0x6e, 0x044e);	/* CYRILLIC SMALL LETTER YU */ \
	    UNI(0x6f, 0x044f);	/* CYRILLIC SMALL LETTER YA */ \
	    UNI(0x70, 0x2116);	/* NUMERO SIGN */ \
	    UNI(0x71, 0x0451);	/* CYRILLIC SMALL LETTER IO */ \
	    UNI(0x72, 0x0452);	/* CYRILLIC SMALL LETTER DJE */ \
	    UNI(0x73, 0x0453);	/* CYRILLIC SMALL LETTER GJE */ \
	    UNI(0x74, 0x0454);	/* CYRILLIC SMALL LETTER UKRAINIAN IE */ \
	    UNI(0x75, 0x0455);	/* CYRILLIC SMALL LETTER DZE */ \
	    UNI(0x76, 0x0456);	/* CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I */ \
	    UNI(0x77, 0x0457);	/* CYRILLIC SMALL LETTER YI */ \
	    UNI(0x78, 0x0458);	/* CYRILLIC SMALL LETTER JE */ \
	    UNI(0x79, 0x0459);	/* CYRILLIC SMALL LETTER LJE */ \
	    UNI(0x7a, 0x045a);	/* CYRILLIC SMALL LETTER NJE */ \
	    UNI(0x7b, 0x045b);	/* CYRILLIC SMALL LETTER TSHE */ \
	    UNI(0x7c, 0x045c);	/* CYRILLIC SMALL LETTER KJE */ \
	    UNI(0x7d, 0x00a7);	/* SECTION SIGN */ \
	    UNI(0x7e, 0x045e);	/* CYRILLIC SMALL LETTER SHORT U */ \
	    UNI(0x7f, 0x045f);	/* CYRILLIC SMALL LETTER DZHE */ \
	} \
	end_CODEPAGE()
	/* ISO Greek is 8859-7 */
#define map_ISO_Greek_Supp(code) \
	begin_CODEPAGE(); \
	switch (code) { \
	    UNI(0x20, 0x00a0);	/* NO-BREAK SPACE */ \
	    UNI(0x21, 0x2018);	/* LEFT SINGLE QUOTATION MARK */ \
	    UNI(0x22, 0x2019);	/* RIGHT SINGLE QUOTATION MARK */ \
	    UNI(0x23, 0x00a3);	/* POUND SIGN */ \
	    UNI(0x24, 0x20ac);	/* EURO SIGN */ \
	    UNI(0x25, 0x20af);	/* DRACHMA SIGN */ \
	    UNI(0x26, 0x00a6);	/* BROKEN BAR */ \
	    UNI(0x27, 0x00a7);	/* SECTION SIGN */ \
	    UNI(0x28, 0x00a8);	/* DIAERESIS */ \
	    UNI(0x29, 0x00a9);	/* COPYRIGHT SIGN */ \
	    UNI(0x2a, 0x037a);	/* GREEK YPOGEGRAMMENI */ \
	    UNI(0x2b, 0x00ab);	/* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
	    UNI(0x2c, 0x00ac);	/* NOT SIGN */ \
	    UNI(0x2d, 0x00ad);	/* SOFT HYPHEN */ \
	    XXX(0x2e, UNDEF);	/* undefined */ \
	    UNI(0x2f, 0x2015);	/* HORIZONTAL BAR */ \
	    UNI(0x30, 0x00b0);	/* DEGREE SIGN */ \
	    UNI(0x31, 0x00b1);	/* PLUS-MINUS SIGN */ \
	    UNI(0x32, 0x00b2);	/* SUPERSCRIPT TWO */ \
	    UNI(0x33, 0x00b3);	/* SUPERSCRIPT THREE */ \
	    UNI(0x34, 0x0384);	/* GREEK TONOS */ \
	    UNI(0x35, 0x0385);	/* GREEK DIALYTIKA TONOS */ \
	    UNI(0x36, 0x0386);	/* GREEK CAPITAL LETTER ALPHA WITH TONOS */ \
	    UNI(0x37, 0x00b7);	/* MIDDLE DOT */ \
	    UNI(0x38, 0x0388);	/* GREEK CAPITAL LETTER EPSILON WITH TONOS */ \
	    UNI(0x39, 0x0389);	/* GREEK CAPITAL LETTER ETA WITH TONOS */ \
	    UNI(0x3a, 0x038a);	/* GREEK CAPITAL LETTER IOTA WITH TONOS */ \
	    UNI(0x3b, 0x00bb);	/* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
	    UNI(0x3c, 0x038c);	/* GREEK CAPITAL LETTER OMICRON WITH TONOS */ \
	    UNI(0x3d, 0x00bd);	/* VULGAR FRACTION ONE HALF */ \
	    UNI(0x3e, 0x038e);	/* GREEK CAPITAL LETTER UPSILON WITH TONOS */ \
	    UNI(0x3f, 0x038f);	/* GREEK CAPITAL LETTER OMEGA WITH TONOS */ \
	    UNI(0x40, 0x0390);	/* GREEK SMALL LETTER IOTA WITH DIALYTIKA AND TONOS */ \
	    UNI(0x41, 0x0391);	/* GREEK CAPITAL LETTER ALPHA */ \
	    UNI(0x42, 0x0392);	/* GREEK CAPITAL LETTER BETA */ \
	    UNI(0x43, 0x0393);	/* GREEK CAPITAL LETTER GAMMA */ \
	    UNI(0x44, 0x0394);	/* GREEK CAPITAL LETTER DELTA */ \
	    UNI(0x45, 0x0395);	/* GREEK CAPITAL LETTER EPSILON */ \
	    UNI(0x46, 0x0396);	/* GREEK CAPITAL LETTER ZETA */ \
	    UNI(0x47, 0x0397);	/* GREEK CAPITAL LETTER ETA */ \
	    UNI(0x48, 0x0398);	/* GREEK CAPITAL LETTER THETA */ \
	    UNI(0x49, 0x0399);	/* GREEK CAPITAL LETTER IOTA */ \
	    UNI(0x4a, 0x039a);	/* GREEK CAPITAL LETTER KAPPA */ \
	    UNI(0x4b, 0x039b);	/* GREEK CAPITAL LETTER LAMDA */ \
	    UNI(0x4c, 0x039c);	/* GREEK CAPITAL LETTER MU */ \
	    UNI(0x4d, 0x039d);	/* GREEK CAPITAL LETTER NU */ \
	    UNI(0x4e, 0x039e);	/* GREEK CAPITAL LETTER XI */ \
	    UNI(0x4f, 0x039f);	/* GREEK CAPITAL LETTER OMICRON */ \
	    UNI(0x50, 0x03a0);	/* GREEK CAPITAL LETTER PI */ \
	    UNI(0x51, 0x03a1);	/* GREEK CAPITAL LETTER RHO */ \
	    XXX(0x52, UNDEF);	/* undefined */ \
	    UNI(0x53, 0x03a3);	/* GREEK CAPITAL LETTER SIGMA */ \
	    UNI(0x54, 0x03a4);	/* GREEK CAPITAL LETTER TAU */ \
	    UNI(0x55, 0x03a5);	/* GREEK CAPITAL LETTER UPSILON */ \
	    UNI(0x56, 0x03a6);	/* GREEK CAPITAL LETTER PHI */ \
	    UNI(0x57, 0x03a7);	/* GREEK CAPITAL LETTER CHI */ \
	    UNI(0x58, 0x03a8);	/* GREEK CAPITAL LETTER PSI */ \
	    UNI(0x59, 0x03a9);	/* GREEK CAPITAL LETTER OMEGA */ \
	    UNI(0x5a, 0x03aa);	/* GREEK CAPITAL LETTER IOTA WITH DIALYTIKA */ \
	    UNI(0x5b, 0x03ab);	/* GREEK CAPITAL LETTER UPSILON WITH DIALYTIKA */ \
	    UNI(0x5c, 0x03ac);	/* GREEK SMALL LETTER ALPHA WITH TONOS */ \
	    UNI(0x5d, 0x03ad);	/* GREEK SMALL LETTER EPSILON WITH TONOS */ \
	    UNI(0x5e, 0x03ae);	/* GREEK SMALL LETTER ETA WITH TONOS */ \
	    UNI(0x5f, 0x03af);	/* GREEK SMALL LETTER IOTA WITH TONOS */ \
	    UNI(0x60, 0x03b0);	/* GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND TONOS */ \
	    UNI(0x61, 0x03b1);	/* GREEK SMALL LETTER ALPHA */ \
	    UNI(0x62, 0x03b2);	/* GREEK SMALL LETTER BETA */ \
	    UNI(0x63, 0x03b3);	/* GREEK SMALL LETTER GAMMA */ \
	    UNI(0x64, 0x03b4);	/* GREEK SMALL LETTER DELTA */ \
	    UNI(0x65, 0x03b5);	/* GREEK SMALL LETTER EPSILON */ \
	    UNI(0x66, 0x03b6);	/* GREEK SMALL LETTER ZETA */ \
	    UNI(0x67, 0x03b7);	/* GREEK SMALL LETTER ETA */ \
	    UNI(0x68, 0x03b8);	/* GREEK SMALL LETTER THETA */ \
	    UNI(0x69, 0x03b9);	/* GREEK SMALL LETTER IOTA */ \
	    UNI(0x6a, 0x03ba);	/* GREEK SMALL LETTER KAPPA */ \
	    UNI(0x6b, 0x03bb);	/* GREEK SMALL LETTER LAMDA */ \
	    UNI(0x6c, 0x03bc);	/* GREEK SMALL LETTER MU */ \
	    UNI(0x6d, 0x03bd);	/* GREEK SMALL LETTER NU */ \
	    UNI(0x6e, 0x03be);	/* GREEK SMALL LETTER XI */ \
	    UNI(0x6f, 0x03bf);	/* GREEK SMALL LETTER OMICRON */ \
	    UNI(0x70, 0x03c0);	/* GREEK SMALL LETTER PI */ \
	    UNI(0x71, 0x03c1);	/* GREEK SMALL LETTER RHO */ \
	    UNI(0x72, 0x03c2);	/* GREEK SMALL LETTER FINAL SIGMA */ \
	    UNI(0x73, 0x03c3);	/* GREEK SMALL LETTER SIGMA */ \
	    UNI(0x74, 0x03c4);	/* GREEK SMALL LETTER TAU */ \
	    UNI(0x75, 0x03c5);	/* GREEK SMALL LETTER UPSILON */ \
	    UNI(0x76, 0x03c6);	/* GREEK SMALL LETTER PHI */ \
	    UNI(0x77, 0x03c7);	/* GREEK SMALL LETTER CHI */ \
	    UNI(0x78, 0x03c8);	/* GREEK SMALL LETTER PSI */ \
	    UNI(0x79, 0x03c9);	/* GREEK SMALL LETTER OMEGA */ \
	    UNI(0x7a, 0x03ca);	/* GREEK SMALL LETTER IOTA WITH DIALYTIKA */ \
	    UNI(0x7b, 0x03cb);	/* GREEK SMALL LETTER UPSILON WITH DIALYTIKA */ \
	    UNI(0x7c, 0x03cc);	/* GREEK SMALL LETTER OMICRON WITH TONOS */ \
	    UNI(0x7d, 0x03cd);	/* GREEK SMALL LETTER UPSILON WITH TONOS */ \
	    UNI(0x7e, 0x03ce);	/* GREEK SMALL LETTER OMEGA WITH TONOS */ \
	} \
	end_CODEPAGE()
	/* figure A-23 "ISO Latin-Hebrew Supplemental Character Set" */
#define map_ISO_Hebrew(code) \
	begin_CODEPAGE(); \
	switch (code) { \
	    UNI(0x20, 0x00a0);	/* NO-BREAK SPACE */ \
	    XXX(0x21, UNDEF);	/* undefined */ \
	    UNI(0x22, 0x00a2);	/* CENT SIGN */ \
	    UNI(0x23, 0x00a3);	/* POUND SIGN */ \
	    UNI(0x24, 0x00a4);	/* CURRENCY SIGN */ \
	    UNI(0x25, 0x00a5);	/* YEN SIGN */ \
	    UNI(0x26, 0x00a6);	/* BROKEN BAR */ \
	    UNI(0x27, 0x00a7);	/* SECTION SIGN */ \
	    UNI(0x28, 0x00a8);	/* DIAERESIS */ \
	    UNI(0x29, 0x00a9);	/* COPYRIGHT SIGN */ \
	    UNI(0x2a, 0x00d7);	/* MULTIPLICATION SIGN */ \
	    UNI(0x2b, 0x00ab);	/* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
	    UNI(0x2c, 0x00ac);	/* NOT SIGN */ \
	    UNI(0x2d, 0x00ad);	/* SOFT HYPHEN */ \
	    UNI(0x2e, 0x00ae);	/* REGISTERED SIGN */ \
	    UNI(0x2f, 0x00af);	/* MACRON */ \
	    UNI(0x30, 0x00b0);	/* DEGREE SIGN */ \
	    UNI(0x31, 0x00b1);	/* PLUS-MINUS SIGN */ \
	    UNI(0x32, 0x00b2);	/* SUPERSCRIPT TWO */ \
	    UNI(0x33, 0x00b3);	/* SUPERSCRIPT THREE */ \
	    UNI(0x34, 0x00b4);	/* ACUTE ACCENT */ \
	    UNI(0x35, 0x00b5);	/* MICRO SIGN */ \
	    UNI(0x36, 0x00b6);	/* PILCROW SIGN */ \
	    UNI(0x37, 0x00b7);	/* MIDDLE DOT */ \
	    UNI(0x38, 0x00b8);	/* CEDILLA */ \
	    UNI(0x39, 0x00b9);	/* SUPERSCRIPT ONE */ \
	    UNI(0x3a, 0x00f7);	/* DIVISION SIGN */ \
	    UNI(0x3b, 0x00bb);	/* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
	    UNI(0x3c, 0x00bc);	/* VULGAR FRACTION ONE QUARTER */ \
	    UNI(0x3d, 0x00bd);	/* VULGAR FRACTION ONE HALF */ \
	    UNI(0x3e, 0x00be);	/* VULGAR FRACTION THREE QUARTERS */ \
	    XXX(0x3f, UNDEF);	/* undefined */ \
	    XXX(0x40, UNDEF);	/* undefined */ \
	    XXX(0x41, UNDEF);	/* undefined */ \
	    XXX(0x42, UNDEF);	/* undefined */ \
	    XXX(0x43, UNDEF);	/* undefined */ \
	    XXX(0x44, UNDEF);	/* undefined */ \
	    XXX(0x45, UNDEF);	/* undefined */ \
	    XXX(0x46, UNDEF);	/* undefined */ \
	    XXX(0x47, UNDEF);	/* undefined */ \
	    XXX(0x48, UNDEF);	/* undefined */ \
	    XXX(0x49, UNDEF);	/* undefined */ \
	    XXX(0x4a, UNDEF);	/* undefined */ \
	    XXX(0x4b, UNDEF);	/* undefined */ \
	    XXX(0x4c, UNDEF);	/* undefined */ \
	    XXX(0x4d, UNDEF);	/* undefined */ \
	    XXX(0x4e, UNDEF);	/* undefined */ \
	    XXX(0x4f, UNDEF);	/* undefined */ \
	    XXX(0x50, UNDEF);	/* undefined */ \
	    XXX(0x51, UNDEF);	/* undefined */ \
	    XXX(0x52, UNDEF);	/* undefined */ \
	    XXX(0x53, UNDEF);	/* undefined */ \
	    XXX(0x54, UNDEF);	/* undefined */ \
	    XXX(0x55, UNDEF);	/* undefined */ \
	    XXX(0x56, UNDEF);	/* undefined */ \
	    XXX(0x57, UNDEF);	/* undefined */ \
	    XXX(0x58, UNDEF);	/* undefined */ \
	    XXX(0x59, UNDEF);	/* undefined */ \
	    XXX(0x5a, UNDEF);	/* undefined */ \
	    XXX(0x5b, UNDEF);	/* undefined */ \
	    XXX(0x5c, UNDEF);	/* undefined */ \
	    XXX(0x5d, UNDEF);	/* undefined */ \
	    XXX(0x5e, UNDEF);	/* undefined */ \
	    UNI(0x5f, 0x2017);	/* DOUBLE LOW LINE */ \
	    UNI(0x60, 0x05d0);	/* HEBREW LETTER ALEF */ \
	    UNI(0x61, 0x05d1);	/* HEBREW LETTER BET */ \
	    UNI(0x62, 0x05d2);	/* HEBREW LETTER GIMEL */ \
	    UNI(0x63, 0x05d3);	/* HEBREW LETTER DALET */ \
	    UNI(0x64, 0x05d4);	/* HEBREW LETTER HE */ \
	    UNI(0x65, 0x05d5);	/* HEBREW LETTER VAV */ \
	    UNI(0x66, 0x05d6);	/* HEBREW LETTER ZAYIN */ \
	    UNI(0x67, 0x05d7);	/* HEBREW LETTER HET */ \
	    UNI(0x68, 0x05d8);	/* HEBREW LETTER TET */ \
	    UNI(0x69, 0x05d9);	/* HEBREW LETTER YOD */ \
	    UNI(0x6a, 0x05da);	/* HEBREW LETTER FINAL KAF */ \
	    UNI(0x6b, 0x05db);	/* HEBREW LETTER KAF */ \
	    UNI(0x6c, 0x05dc);	/* HEBREW LETTER LAMED */ \
	    UNI(0x6d, 0x05dd);	/* HEBREW LETTER FINAL MEM */ \
	    UNI(0x6e, 0x05de);	/* HEBREW LETTER MEM */ \
	    UNI(0x6f, 0x05df);	/* HEBREW LETTER FINAL NUN */ \
	    UNI(0x70, 0x05e0);	/* HEBREW LETTER NUN */ \
	    UNI(0x71, 0x05e1);	/* HEBREW LETTER SAMEKH */ \
	    UNI(0x72, 0x05e2);	/* HEBREW LETTER AYIN */ \
	    UNI(0x73, 0x05e3);	/* HEBREW LETTER FINAL PE */ \
	    UNI(0x74, 0x05e4);	/* HEBREW LETTER PE */ \
	    UNI(0x75, 0x05e5);	/* HEBREW LETTER FINAL TSADI */ \
	    UNI(0x76, 0x05e6);	/* HEBREW LETTER TSADI */ \
	    UNI(0x77, 0x05e7);	/* HEBREW LETTER QOF */ \
	    UNI(0x78, 0x05e8);	/* HEBREW LETTER RESH */ \
	    UNI(0x79, 0x05e9);	/* HEBREW LETTER SHIN */ \
	    UNI(0x7a, 0x05ea);	/* HEBREW LETTER TAV */ \
	    XXX(0x7b, UNDEF);	/* undefined */ \
	    XXX(0x7c, UNDEF);	/* undefined */ \
	    UNI(0x7d, 0x200e);	/* LEFT-TO-RIGHT MARK */ \
	    UNI(0x7e, 0x200f);	/* RIGHT-TO-LEFT MARK */ \
	} \
	end_CODEPAGE()
	/* ISO Latin-5 is 8859-9 */
#define map_ISO_Latin_5(code) \
	begin_CODEPAGE(); \
	switch (code) { \
	    UNI(0x20, 0x00a0);	/* NO-BREAK SPACE */ \
	    UNI(0x21, 0x00a1);	/* INVERTED EXCLAMATION MARK */ \
	    UNI(0x22, 0x00a2);	/* CENT SIGN */ \
	    UNI(0x23, 0x00a3);	/* POUND SIGN */ \
	    UNI(0x24, 0x00a4);	/* CURRENCY SIGN */ \
	    UNI(0x25, 0x00a5);	/* YEN SIGN */ \
	    UNI(0x26, 0x00a6);	/* BROKEN BAR */ \
	    UNI(0x27, 0x00a7);	/* SECTION SIGN */ \
	    UNI(0x28, 0x00a8);	/* DIAERESIS */ \
	    UNI(0x29, 0x00a9);	/* COPYRIGHT SIGN */ \
	    UNI(0x2a, 0x00aa);	/* FEMININE ORDINAL INDICATOR */ \
	    UNI(0x2b, 0x00ab);	/* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
	    UNI(0x2c, 0x00ac);	/* NOT SIGN */ \
	    UNI(0x2d, 0x00ad);	/* SOFT HYPHEN */ \
	    UNI(0x2e, 0x00ae);	/* REGISTERED SIGN */ \
	    UNI(0x2f, 0x00af);	/* MACRON */ \
	    UNI(0x30, 0x00b0);	/* DEGREE SIGN */ \
	    UNI(0x31, 0x00b1);	/* PLUS-MINUS SIGN */ \
	    UNI(0x32, 0x00b2);	/* SUPERSCRIPT TWO */ \
	    UNI(0x33, 0x00b3);	/* SUPERSCRIPT THREE */ \
	    UNI(0x34, 0x00b4);	/* ACUTE ACCENT */ \
	    UNI(0x35, 0x00b5);	/* MICRO SIGN */ \
	    UNI(0x36, 0x00b6);	/* PILCROW SIGN */ \
	    UNI(0x37, 0x00b7);	/* MIDDLE DOT */ \
	    UNI(0x38, 0x00b8);	/* CEDILLA */ \
	    UNI(0x39, 0x00b9);	/* SUPERSCRIPT ONE */ \
	    UNI(0x3a, 0x00ba);	/* MASCULINE ORDINAL INDICATOR */ \
	    UNI(0x3b, 0x00bb);	/* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
	    UNI(0x3c, 0x00bc);	/* VULGAR FRACTION ONE QUARTER */ \
	    UNI(0x3d, 0x00bd);	/* VULGAR FRACTION ONE HALF */ \
	    UNI(0x3e, 0x00be);	/* VULGAR FRACTION THREE QUARTERS */ \
	    UNI(0x3f, 0x00bf);	/* INVERTED QUESTION MARK */ \
	    UNI(0x40, 0x00c0);	/* LATIN CAPITAL LETTER A WITH GRAVE */ \
	    UNI(0x41, 0x00c1);	/* LATIN CAPITAL LETTER A WITH ACUTE */ \
	    UNI(0x42, 0x00c2);	/* LATIN CAPITAL LETTER A WITH CIRCUMFLEX */ \
	    UNI(0x43, 0x00c3);	/* LATIN CAPITAL LETTER A WITH TILDE */ \
	    UNI(0x44, 0x00c4);	/* LATIN CAPITAL LETTER A WITH DIAERESIS */ \
	    UNI(0x45, 0x00c5);	/* LATIN CAPITAL LETTER A WITH RING ABOVE */ \
	    UNI(0x46, 0x00c6);	/* LATIN CAPITAL LETTER AE */ \
	    UNI(0x47, 0x00c7);	/* LATIN CAPITAL LETTER C WITH CEDILLA */ \
	    UNI(0x48, 0x00c8);	/* LATIN CAPITAL LETTER E WITH GRAVE */ \
	    UNI(0x49, 0x00c9);	/* LATIN CAPITAL LETTER E WITH ACUTE */ \
	    UNI(0x4a, 0x00ca);	/* LATIN CAPITAL LETTER E WITH CIRCUMFLEX */ \
	    UNI(0x4b, 0x00cb);	/* LATIN CAPITAL LETTER E WITH DIAERESIS */ \
	    UNI(0x4c, 0x00cc);	/* LATIN CAPITAL LETTER I WITH GRAVE */ \
	    UNI(0x4d, 0x00cd);	/* LATIN CAPITAL LETTER I WITH ACUTE */ \
	    UNI(0x4e, 0x00ce);	/* LATIN CAPITAL LETTER I WITH CIRCUMFLEX */ \
	    UNI(0x4f, 0x00cf);	/* LATIN CAPITAL LETTER I WITH DIAERESIS */ \
	    UNI(0x50, 0x011e);	/* LATIN CAPITAL LETTER G WITH BREVE */ \
	    UNI(0x51, 0x00d1);	/* LATIN CAPITAL LETTER N WITH TILDE */ \
	    UNI(0x52, 0x00d2);	/* LATIN CAPITAL LETTER O WITH GRAVE */ \
	    UNI(0x53, 0x00d3);	/* LATIN CAPITAL LETTER O WITH ACUTE */ \
	    UNI(0x54, 0x00d4);	/* LATIN CAPITAL LETTER O WITH CIRCUMFLEX */ \
	    UNI(0x55, 0x00d5);	/* LATIN CAPITAL LETTER O WITH TILDE */ \
	    UNI(0x56, 0x00d6);	/* LATIN CAPITAL LETTER O WITH DIAERESIS */ \
	    UNI(0x57, 0x00d7);	/* MULTIPLICATION SIGN */ \
	    UNI(0x58, 0x00d8);	/* LATIN CAPITAL LETTER O WITH STROKE */ \
	    UNI(0x59, 0x00d9);	/* LATIN CAPITAL LETTER U WITH GRAVE */ \
	    UNI(0x5a, 0x00da);	/* LATIN CAPITAL LETTER U WITH ACUTE */ \
	    UNI(0x5b, 0x00db);	/* LATIN CAPITAL LETTER U WITH CIRCUMFLEX */ \
	    UNI(0x5c, 0x00dc);	/* LATIN CAPITAL LETTER U WITH DIAERESIS */ \
	    UNI(0x5d, 0x0130);	/* LATIN CAPITAL LETTER I WITH DOT ABOVE */ \
	    UNI(0x5e, 0x015e);	/* LATIN CAPITAL LETTER S WITH CEDILLA */ \
	    UNI(0x5f, 0x00df);	/* LATIN SMALL LETTER SHARP S */ \
	    UNI(0x60, 0x00e0);	/* LATIN SMALL LETTER A WITH GRAVE */ \
	    UNI(0x61, 0x00e1);	/* LATIN SMALL LETTER A WITH ACUTE */ \
	    UNI(0x62, 0x00e2);	/* LATIN SMALL LETTER A WITH CIRCUMFLEX */ \
	    UNI(0x63, 0x00e3);	/* LATIN SMALL LETTER A WITH TILDE */ \
	    UNI(0x64, 0x00e4);	/* LATIN SMALL LETTER A WITH DIAERESIS */ \
	    UNI(0x65, 0x00e5);	/* LATIN SMALL LETTER A WITH RING ABOVE */ \
	    UNI(0x66, 0x00e6);	/* LATIN SMALL LETTER AE */ \
	    UNI(0x67, 0x00e7);	/* LATIN SMALL LETTER C WITH CEDILLA */ \
	    UNI(0x68, 0x00e8);	/* LATIN SMALL LETTER E WITH GRAVE */ \
	    UNI(0x69, 0x00e9);	/* LATIN SMALL LETTER E WITH ACUTE */ \
	    UNI(0x6a, 0x00ea);	/* LATIN SMALL LETTER E WITH CIRCUMFLEX */ \
	    UNI(0x6b, 0x00eb);	/* LATIN SMALL LETTER E WITH DIAERESIS */ \
	    UNI(0x6c, 0x00ec);	/* LATIN SMALL LETTER I WITH GRAVE */ \
	    UNI(0x6d, 0x00ed);	/* LATIN SMALL LETTER I WITH ACUTE */ \
	    UNI(0x6e, 0x00ee);	/* LATIN SMALL LETTER I WITH CIRCUMFLEX */ \
	    UNI(0x6f, 0x00ef);	/* LATIN SMALL LETTER I WITH DIAERESIS */ \
	    UNI(0x70, 0x011f);	/* LATIN SMALL LETTER G WITH BREVE */ \
	    UNI(0x71, 0x00f1);	/* LATIN SMALL LETTER N WITH TILDE */ \
	    UNI(0x72, 0x00f2);	/* LATIN SMALL LETTER O WITH GRAVE */ \
	    UNI(0x73, 0x00f3);	/* LATIN SMALL LETTER O WITH ACUTE */ \
	    UNI(0x74, 0x00f4);	/* LATIN SMALL LETTER O WITH CIRCUMFLEX */ \
	    UNI(0x75, 0x00f5);	/* LATIN SMALL LETTER O WITH TILDE */ \
	    UNI(0x76, 0x00f6);	/* LATIN SMALL LETTER O WITH DIAERESIS */ \
	    UNI(0x77, 0x00f7);	/* DIVISION SIGN */ \
	    UNI(0x78, 0x00f8);	/* LATIN SMALL LETTER O WITH STROKE */ \
	    UNI(0x79, 0x00f9);	/* LATIN SMALL LETTER U WITH GRAVE */ \
	    UNI(0x7a, 0x00fa);	/* LATIN SMALL LETTER U WITH ACUTE */ \
	    UNI(0x7b, 0x00fb);	/* LATIN SMALL LETTER U WITH CIRCUMFLEX */ \
	    UNI(0x7c, 0x00fc);	/* LATIN SMALL LETTER U WITH DIAERESIS */ \
	    UNI(0x7d, 0x0131);	/* LATIN SMALL LETTER DOTLESS I */ \
	    UNI(0x7e, 0x015f);	/* LATIN SMALL LETTER S WITH CEDILLA */ \
	    UNI(0x7f, 0x00ff);	/* LATIN SMALL LETTER Y WITH DIAERESIS */ \
	} \
	end_CODEPAGE()
	/* DEC Cyrillic from screenshot */
#define map_DEC_Cyrillic(code) \
	begin_CODEPAGE(); \
	switch (code) { \
	    UNI(0x20, 0x00a0);	/* NO-BREAK SPACE */ \
	    XXX(0x21, UNDEF);	/* undefined */ \
	    XXX(0x22, UNDEF);	/* undefined */ \
	    XXX(0x23, UNDEF);	/* undefined */ \
	    XXX(0x24, UNDEF);	/* undefined */ \
	    XXX(0x25, UNDEF);	/* undefined */ \
	    XXX(0x26, UNDEF);	/* undefined */ \
	    XXX(0x27, UNDEF);	/* undefined */ \
	    XXX(0x28, UNDEF);	/* undefined */ \
	    XXX(0x29, UNDEF);	/* undefined */ \
	    XXX(0x2a, UNDEF);	/* undefined */ \
	    XXX(0x2b, UNDEF);	/* undefined */ \
	    XXX(0x2c, UNDEF);	/* undefined */ \
	    XXX(0x2d, UNDEF);	/* undefined */ \
	    XXX(0x2e, UNDEF);	/* undefined */ \
	    XXX(0x2f, UNDEF);	/* undefined */ \
	    XXX(0x30, UNDEF);	/* undefined */ \
	    XXX(0x31, UNDEF);	/* undefined */ \
	    XXX(0x32, UNDEF);	/* undefined */ \
	    XXX(0x33, UNDEF);	/* undefined */ \
	    XXX(0x34, UNDEF);	/* undefined */ \
	    XXX(0x35, UNDEF);	/* undefined */ \
	    XXX(0x36, UNDEF);	/* undefined */ \
	    XXX(0x37, UNDEF);	/* undefined */ \
	    XXX(0x38, UNDEF);	/* undefined */ \
	    XXX(0x39, UNDEF);	/* undefined */ \
	    XXX(0x3a, UNDEF);	/* undefined */ \
	    XXX(0x3b, UNDEF);	/* undefined */ \
	    XXX(0x3c, UNDEF);	/* undefined */ \
	    XXX(0x3d, UNDEF);	/* undefined */ \
	    XXX(0x3e, UNDEF);	/* undefined */ \
	    XXX(0x3f, UNDEF);	/* undefined */ \
	    \
	    UNI(0x40, 0x044e);	/* CYRILLIC SMALL LETTER YU */ \
	    UNI(0x41, 0x0430);	/* CYRILLIC SMALL LETTER A */ \
	    UNI(0x42, 0x0431);	/* CYRILLIC SMALL LETTER BE */ \
	    UNI(0x43, 0x0446);	/* CYRILLIC SMALL LETTER TSE */ \
	    UNI(0x44, 0x0434);	/* CYRILLIC SMALL LETTER DE */ \
	    UNI(0x45, 0x0435);	/* CYRILLIC SMALL LETTER IE */ \
	    UNI(0x46, 0x0444);	/* CYRILLIC SMALL LETTER EF */ \
	    UNI(0x47, 0x0433);	/* CYRILLIC SMALL LETTER GHE */ \
	    UNI(0x48, 0x0445);	/* CYRILLIC SMALL LETTER HA */ \
	    UNI(0x49, 0x0438);	/* CYRILLIC SMALL LETTER I */ \
	    UNI(0x4a, 0x0439);	/* CYRILLIC SMALL LETTER SHORT I */ \
	    UNI(0x4b, 0x043a);	/* CYRILLIC SMALL LETTER KA */ \
	    UNI(0x4c, 0x043b);	/* CYRILLIC SMALL LETTER EL */ \
	    UNI(0x4d, 0x043c);	/* CYRILLIC SMALL LETTER EM */ \
	    UNI(0x4e, 0x043d);	/* CYRILLIC SMALL LETTER EN */ \
	    UNI(0x4f, 0x043e);	/* CYRILLIC SMALL LETTER O */ \
	    UNI(0x50, 0x043f);	/* CYRILLIC SMALL LETTER PE */ \
	    UNI(0x51, 0x044f);	/* CYRILLIC SMALL LETTER YA */ \
	    UNI(0x52, 0x0440);	/* CYRILLIC SMALL LETTER ER */ \
	    UNI(0x53, 0x0441);	/* CYRILLIC SMALL LETTER ES */ \
	    UNI(0x54, 0x0442);	/* CYRILLIC SMALL LETTER TE */ \
	    UNI(0x55, 0x0443);	/* CYRILLIC SMALL LETTER U */ \
	    UNI(0x56, 0x0436);	/* CYRILLIC SMALL LETTER ZHE */ \
	    UNI(0x57, 0x0432);	/* CYRILLIC SMALL LETTER VE */ \
	    UNI(0x58, 0x044c);	/* CYRILLIC SMALL LETTER SOFT SIGN */ \
	    UNI(0x59, 0x044b);	/* CYRILLIC SMALL LETTER YERU */ \
	    UNI(0x5a, 0x0437);	/* CYRILLIC SMALL LETTER ZE */ \
	    UNI(0x5b, 0x0448);	/* CYRILLIC SMALL LETTER SHA */ \
	    UNI(0x5c, 0x044d);	/* CYRILLIC SMALL LETTER E */ \
	    UNI(0x5d, 0x0449);	/* CYRILLIC SMALL LETTER SHCHA */ \
	    UNI(0x5e, 0x0447);	/* CYRILLIC SMALL LETTER CHE */ \
	    UNI(0x5f, 0x044a);	/* CYRILLIC SMALL LETTER HARD SIGN */ \
	    \
	    UNI(0x60, 0x042e);	/* CYRILLIC CAPITAL LETTER YU */ \
	    UNI(0x61, 0x0410);	/* CYRILLIC CAPITAL LETTER A */ \
	    UNI(0x62, 0x0411);	/* CYRILLIC CAPITAL LETTER BE */ \
	    UNI(0x63, 0x0426);	/* CYRILLIC CAPITAL LETTER TSE */ \
	    UNI(0x64, 0x0414);	/* CYRILLIC CAPITAL LETTER DE */ \
	    UNI(0x65, 0x0415);	/* CYRILLIC CAPITAL LETTER IE */ \
	    UNI(0x66, 0x0424);	/* CYRILLIC CAPITAL LETTER EF */ \
	    UNI(0x67, 0x0413);	/* CYRILLIC CAPITAL LETTER GHE */ \
	    UNI(0x68, 0x0425);	/* CYRILLIC CAPITAL LETTER HA */ \
	    UNI(0x69, 0x0418);	/* CYRILLIC CAPITAL LETTER I */ \
	    UNI(0x6a, 0x0419);	/* CYRILLIC CAPITAL LETTER SHORT I */ \
	    UNI(0x6b, 0x041a);	/* CYRILLIC CAPITAL LETTER KA */ \
	    UNI(0x6c, 0x041b);	/* CYRILLIC CAPITAL LETTER EL */ \
	    UNI(0x6d, 0x041c);	/* CYRILLIC CAPITAL LETTER EM */ \
	    UNI(0x6e, 0x041d);	/* CYRILLIC CAPITAL LETTER EN */ \
	    UNI(0x6f, 0x041e);	/* CYRILLIC CAPITAL LETTER O */ \
	    UNI(0x70, 0x041f);	/* CYRILLIC CAPITAL LETTER PE */ \
	    UNI(0x71, 0x042f);	/* CYRILLIC CAPITAL LETTER YA */ \
	    UNI(0x72, 0x0420);	/* CYRILLIC CAPITAL LETTER ER */ \
	    UNI(0x73, 0x0421);	/* CYRILLIC CAPITAL LETTER ES */ \
	    UNI(0x74, 0x0422);	/* CYRILLIC CAPITAL LETTER TE */ \
	    UNI(0x75, 0x0423);	/* CYRILLIC CAPITAL LETTER U */ \
	    UNI(0x76, 0x0416);	/* CYRILLIC CAPITAL LETTER ZHE */ \
	    UNI(0x77, 0x0412);	/* CYRILLIC CAPITAL LETTER VE */ \
	    UNI(0x78, 0x042c);	/* CYRILLIC CAPITAL LETTER SOFT SIGN */ \
	    UNI(0x79, 0x042b);	/* CYRILLIC CAPITAL LETTER YERU */ \
	    UNI(0x7a, 0x0417);	/* CYRILLIC CAPITAL LETTER ZE */ \
	    UNI(0x7b, 0x0428);	/* CYRILLIC CAPITAL LETTER SHA */ \
	    UNI(0x7c, 0x042d);	/* CYRILLIC CAPITAL LETTER E */ \
	    UNI(0x7d, 0x0429);	/* CYRILLIC CAPITAL LETTER SHCHA */ \
	    UNI(0x7e, 0x0427);	/* CYRILLIC CAPITAL LETTER CHE */ \
	    UNI(0x7f, 0x042a);	/* CYRILLIC CAPITAL LETTER HARD SIGN */ \
	    \
	} \
	end_CODEPAGE()
	/* figure A-24 "DEC Greek Supplemental Character Set" */
#define map_DEC_Greek_Supp(code)	\
	begin_CODEPAGE(); \
	switch (code) { \
	    MAP(0x20, 0x00a0);	/* NO-BREAK SPACE */ \
	    UNI(0x21, 0x00a1);	/* LEFT SINGLE QUOTATION MARK */ \
	    UNI(0x22, 0x00a2);	/* RIGHT SINGLE QUOTATION MARK */ \
	    UNI(0x23, 0x00a3);	/* POUND SIGN */ \
	    XXX(0x24, UNDEF);	/* EURO SIGN */ \
	    UNI(0x25, 0x00a5);	/* YEN SIGN */ \
	    XXX(0x26, UNDEF);	/* BROKEN BAR */ \
	    UNI(0x27, 0x00a7);	/* SECTION SIGN */ \
	    UNI(0x28, 0x00a4);	/* CURRENCY SIGN */ \
	    UNI(0x29, 0x00a9);	/* COPYRIGHT SIGN */ \
	    UNI(0x2a, 0x00aa);	/* FEMININE ORDINAL INDICATOR */ \
	    UNI(0x2b, 0x00ab);	/* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
	    XXX(0x2c, UNDEF);	/* reserved */ \
	    XXX(0x2d, UNDEF);	/* reserved */ \
	    XXX(0x2e, UNDEF);	/* reserved */ \
	    XXX(0x2f, UNDEF);	/* reserved */ \
	    UNI(0x30, 0x00b0);	/* DEGREE SIGN */ \
	    UNI(0x31, 0x00b1);	/* PLUS-MINUS SIGN */ \
	    UNI(0x32, 0x00b2);	/* SUPERSCRIPT TWO */ \
	    UNI(0x33, 0x00b3);	/* SUPERSCRIPT THREE */ \
	    XXX(0x34, UNDEF);	/* reserved */ \
	    UNI(0x35, 0x00b5);	/* MICRO SIGN */ \
	    UNI(0x36, 0x00b6);	/* PILCROW SIGN */ \
	    UNI(0x37, 0x00b7);	/* MIDDLE DOT */ \
	    XXX(0x38, UNDEF);	/* reserved */ \
	    UNI(0x39, 0x00b9);	/* SUPERSCRIPT ONE */ \
	    UNI(0x3a, 0x00ba);	/* MASCULINE ORDINAL INDICATOR */ \
	    UNI(0x3b, 0x00bb);	/* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
	    UNI(0x3c, 0x00bc);	/* VULGAR FRACTION ONE QUARTER */ \
	    UNI(0x3d, 0x00bd);	/* VULGAR FRACTION ONE HALF */ \
	    XXX(0x3e, UNDEF);	/* reserved */ \
	    UNI(0x3f, 0x00BF);  /* INVERTED QUESTION MARK */ \
	    UNI(0x40, 0x03ca);	/* GREEK SMALL LETTER IOTA WITH DIALYTIKA */ \
	    UNI(0x41, 0x0391);	/* GREEK CAPITAL LETTER ALPHA */ \
	    UNI(0x42, 0x0392);	/* GREEK CAPITAL LETTER BETA */ \
	    UNI(0x43, 0x0393);	/* GREEK CAPITAL LETTER GAMMA */ \
	    UNI(0x44, 0x0394);	/* GREEK CAPITAL LETTER DELTA */ \
	    UNI(0x45, 0x0395);	/* GREEK CAPITAL LETTER EPSILON */ \
	    UNI(0x46, 0x0396);	/* GREEK CAPITAL LETTER ZETA */ \
	    UNI(0x47, 0x0397);	/* GREEK CAPITAL LETTER ETA */ \
	    UNI(0x48, 0x0398);	/* GREEK CAPITAL LETTER THETA */ \
	    UNI(0x49, 0x0399);	/* GREEK CAPITAL LETTER IOTA */ \
	    UNI(0x4a, 0x039a);	/* GREEK CAPITAL LETTER KAPPA */ \
	    UNI(0x4b, 0x039b);	/* GREEK CAPITAL LETTER LAMDA */ \
	    UNI(0x4c, 0x039c);	/* GREEK CAPITAL LETTER MU */ \
	    UNI(0x4d, 0x039d);	/* GREEK CAPITAL LETTER NU */ \
	    UNI(0x4e, 0x039e);	/* GREEK CAPITAL LETTER XI */ \
	    UNI(0x4f, 0x039f);	/* GREEK CAPITAL LETTER OMICRON */ \
	    XXX(0x50, UNDEF);	/* reserved */ \
	    UNI(0x51, 0x03a0);	/* GREEK CAPITAL LETTER PI */ \
	    UNI(0x52, 0x03a1);	/* GREEK CAPITAL LETTER RHO */ \
	    UNI(0x53, 0x03a3);	/* GREEK CAPITAL LETTER SIGMA */ \
	    UNI(0x54, 0x03a4);	/* GREEK CAPITAL LETTER TAU */ \
	    UNI(0x55, 0x03a5);	/* GREEK CAPITAL LETTER UPSILON */ \
	    UNI(0x56, 0x03a6);	/* GREEK CAPITAL LETTER PHI */ \
	    UNI(0x57, 0x03a7);	/* GREEK CAPITAL LETTER CHI */ \
	    UNI(0x58, 0x03a8);	/* GREEK CAPITAL LETTER PSI */ \
	    UNI(0x59, 0x03a9);	/* GREEK CAPITAL LETTER OMEGA */ \
	    UNI(0x5a, 0x03ac);	/* GREEK SMALL LETTER ALPHA WITH TONOS */ \
	    UNI(0x5b, 0x03ad);	/* GREEK SMALL LETTER EPSILON WITH TONOS */ \
	    UNI(0x5c, 0x03ae);	/* GREEK SMALL LETTER ETA WITH TONOS */ \
	    UNI(0x5d, 0x03af);	/* GREEK SMALL LETTER IOTA WITH TONOS */ \
	    XXX(0x5e, UNDEF);	/* reserved */ \
	    UNI(0x5f, 0x03cc);	/* GREEK SMALL LETTER OMICRON WITH TONOS */ \
	    UNI(0x60, 0x03cb);	/* GREEK SMALL LETTER UPSILON WITH DIALYTIKA */ \
	    UNI(0x61, 0x03b1);	/* GREEK SMALL LETTER ALPHA */ \
	    UNI(0x62, 0x03b2);	/* GREEK SMALL LETTER BETA */ \
	    UNI(0x63, 0x03b3);	/* GREEK SMALL LETTER GAMMA */ \
	    UNI(0x64, 0x03b4);	/* GREEK SMALL LETTER DELTA */ \
	    UNI(0x65, 0x03b5);	/* GREEK SMALL LETTER EPSILON */ \
	    UNI(0x66, 0x03b6);	/* GREEK SMALL LETTER ZETA */ \
	    UNI(0x67, 0x03b7);	/* GREEK SMALL LETTER ETA */ \
	    UNI(0x68, 0x03b8);	/* GREEK SMALL LETTER THETA */ \
	    UNI(0x69, 0x03b9);	/* GREEK SMALL LETTER IOTA */ \
	    UNI(0x6a, 0x03ba);	/* GREEK SMALL LETTER KAPPA */ \
	    UNI(0x6b, 0x03bb);	/* GREEK SMALL LETTER LAMDA */ \
	    UNI(0x6c, 0x03bc);	/* GREEK SMALL LETTER MU */ \
	    UNI(0x6d, 0x03bd);	/* GREEK SMALL LETTER NU */ \
	    UNI(0x6e, 0x03be);	/* GREEK SMALL LETTER XI */ \
	    UNI(0x6f, 0x03bf);	/* GREEK SMALL LETTER OMICRON */ \
	    XXX(0x70, UNDEF);	/* reserved */ \
	    UNI(0x71, 0x03c0);	/* GREEK SMALL LETTER PI */ \
	    UNI(0x72, 0x03c1);	/* GREEK SMALL LETTER RHO */ \
	    UNI(0x73, 0x03c3);	/* GREEK SMALL LETTER SIGMA */ \
	    UNI(0x74, 0x03c4);	/* GREEK SMALL LETTER TAU */ \
	    UNI(0x75, 0x03c5);	/* GREEK SMALL LETTER UPSILON */ \
	    UNI(0x76, 0x03c6);	/* GREEK SMALL LETTER PHI */ \
	    UNI(0x77, 0x03c7);	/* GREEK SMALL LETTER CHI */ \
	    UNI(0x78, 0x03c8);	/* GREEK SMALL LETTER PSI */ \
	    UNI(0x79, 0x03c9);	/* GREEK SMALL LETTER OMEGA */ \
	    UNI(0x7a, 0x03c2);	/* GREEK SMALL LETTER FINAL SIGMA */ \
	    UNI(0x7b, 0x03cd);	/* GREEK SMALL LETTER UPSILON WITH TONOS */ \
	    UNI(0x7c, 0x03ce);	/* GREEK SMALL LETTER OMEGA WITH TONOS */ \
	    UNI(0x7d, 0x0384);	/* GREEK TONOS */ \
	    XXX(0x7e, UNDEF);	/* reserved */ \
	} \
	end_CODEPAGE()
	/* figure A-22 "DEC Hebrew Supplemental Character Set" */
#define map_DEC_Hebrew_Supp(code) \
	begin_CODEPAGE(); \
	switch (code) { \
	    UNI(0x21, 0x00a1);	/* INVERTED EXCLAMATION MARK */ \
	    UNI(0x22, 0x00a2);	/* CENT SIGN */ \
	    UNI(0x23, 0x00a3);	/* POUND SIGN */ \
	    XXX(0x24, UNDEF);	/* CURRENCY SIGN */ \
	    UNI(0x25, 0x00a5);	/* YEN SIGN */ \
	    XXX(0x26, UNDEF);	/* BROKEN BAR */ \
	    UNI(0x27, 0x00a7);	/* SECTION SIGN */ \
	    UNI(0x28, 0x00a8);	/* DIAERESIS */ \
	    UNI(0x29, 0x00a9);	/* COPYRIGHT SIGN */ \
	    UNI(0x2a, 0x00d7);	/* MULTIPLICATION SIGN */ \
	    UNI(0x2b, 0x00ab);	/* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
	    XXX(0x2c, UNDEF);	/* NOT SIGN */ \
	    XXX(0x2d, UNDEF);	/* SOFT HYPHEN */ \
	    XXX(0x2e, UNDEF);	/* REGISTERED SIGN */ \
	    XXX(0x2f, UNDEF);	/* MACRON */ \
	    UNI(0x30, 0x00b0);	/* DEGREE SIGN */ \
	    UNI(0x31, 0x00b1);	/* PLUS-MINUS SIGN */ \
	    UNI(0x32, 0x00b2);	/* SUPERSCRIPT TWO */ \
	    UNI(0x33, 0x00b3);	/* SUPERSCRIPT THREE */ \
	    XXX(0x34, UNDEF);	/* ACUTE ACCENT */ \
	    UNI(0x35, 0x00b5);	/* MICRO SIGN */ \
	    UNI(0x36, 0x00b6);	/* PILCROW SIGN */ \
	    UNI(0x37, 0x00b7);	/* MIDDLE DOT */ \
	    XXX(0x38, UNDEF);	/* CEDILLA */ \
	    UNI(0x39, 0x00b9);	/* SUPERSCRIPT ONE */ \
	    UNI(0x3a, 0x00f7);	/* DIVISION SIGN */ \
	    UNI(0x3b, 0x00bb);	/* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
	    UNI(0x3c, 0x00bc);	/* VULGAR FRACTION ONE QUARTER */ \
	    UNI(0x3d, 0x00bd);	/* VULGAR FRACTION ONE HALF */ \
	    XXX(0x3e, UNDEF);	/* VULGAR FRACTION THREE QUARTERS */ \
	    UNI(0x3f, 0x00bf);	/* INVERTED QUESTION MARK */ \
	    XXX(0x40, UNDEF);	/* reserved */ \
	    XXX(0x41, UNDEF);	/* reserved */ \
	    XXX(0x42, UNDEF);	/* reserved */ \
	    XXX(0x43, UNDEF);	/* reserved */ \
	    XXX(0x44, UNDEF);	/* reserved */ \
	    XXX(0x45, UNDEF);	/* reserved */ \
	    XXX(0x46, UNDEF);	/* reserved */ \
	    XXX(0x47, UNDEF);	/* reserved */ \
	    XXX(0x48, UNDEF);	/* reserved */ \
	    XXX(0x49, UNDEF);	/* reserved */ \
	    XXX(0x4a, UNDEF);	/* reserved */ \
	    XXX(0x4b, UNDEF);	/* reserved */ \
	    XXX(0x4c, UNDEF);	/* reserved */ \
	    XXX(0x4d, UNDEF);	/* reserved */ \
	    XXX(0x4e, UNDEF);	/* reserved */ \
	    XXX(0x4f, UNDEF);	/* reserved */ \
	    XXX(0x50, UNDEF);	/* reserved */ \
	    XXX(0x51, UNDEF);	/* reserved */ \
	    XXX(0x52, UNDEF);	/* reserved */ \
	    XXX(0x53, UNDEF);	/* reserved */ \
	    XXX(0x54, UNDEF);	/* reserved */ \
	    XXX(0x55, UNDEF);	/* reserved */ \
	    XXX(0x56, UNDEF);	/* reserved */ \
	    XXX(0x57, UNDEF);	/* reserved */ \
	    XXX(0x58, UNDEF);	/* reserved */ \
	    XXX(0x59, UNDEF);	/* reserved */ \
	    XXX(0x5a, UNDEF);	/* reserved */ \
	    XXX(0x5b, UNDEF);	/* reserved */ \
	    XXX(0x5c, UNDEF);	/* reserved */ \
	    XXX(0x5d, UNDEF);	/* reserved */ \
	    XXX(0x5e, UNDEF);	/* reserved */ \
	    XXX(0x5f, UNDEF);	/* reserved */ \
	    UNI(0x60, 0x05d0);	/* HEBREW LETTER ALEF */ \
	    UNI(0x61, 0x05d1);	/* HEBREW LETTER BET */ \
	    UNI(0x62, 0x05d2);	/* HEBREW LETTER GIMEL */ \
	    UNI(0x63, 0x05d3);	/* HEBREW LETTER DALET */ \
	    UNI(0x64, 0x05d4);	/* HEBREW LETTER HE */ \
	    UNI(0x65, 0x05d5);	/* HEBREW LETTER VAV */ \
	    UNI(0x66, 0x05d6);	/* HEBREW LETTER ZAYIN */ \
	    UNI(0x67, 0x05d7);	/* HEBREW LETTER HET */ \
	    UNI(0x68, 0x05d8);	/* HEBREW LETTER TET */ \
	    UNI(0x69, 0x05d9);	/* HEBREW LETTER YOD */ \
	    UNI(0x6a, 0x05da);	/* HEBREW LETTER FINAL KAF */ \
	    UNI(0x6b, 0x05db);	/* HEBREW LETTER KAF */ \
	    UNI(0x6c, 0x05dc);	/* HEBREW LETTER LAMED */ \
	    UNI(0x6d, 0x05dd);	/* HEBREW LETTER FINAL MEM */ \
	    UNI(0x6e, 0x05de);	/* HEBREW LETTER MEM */ \
	    UNI(0x6f, 0x05df);	/* HEBREW LETTER FINAL NUN */ \
	    UNI(0x70, 0x05e0);	/* HEBREW LETTER NUN */ \
	    UNI(0x71, 0x05e1);	/* HEBREW LETTER SAMEKH */ \
	    UNI(0x72, 0x05e2);	/* HEBREW LETTER AYIN */ \
	    UNI(0x73, 0x05e3);	/* HEBREW LETTER FINAL PE */ \
	    UNI(0x74, 0x05e4);	/* HEBREW LETTER PE */ \
	    UNI(0x75, 0x05e5);	/* HEBREW LETTER FINAL TSADI */ \
	    UNI(0x76, 0x05e6);	/* HEBREW LETTER TSADI */ \
	    UNI(0x77, 0x05e7);	/* HEBREW LETTER QOF */ \
	    UNI(0x78, 0x05e8);	/* HEBREW LETTER RESH */ \
	    UNI(0x79, 0x05e9);	/* HEBREW LETTER SHIN */ \
	    UNI(0x7a, 0x05ea);	/* HEBREW LETTER TAV */ \
	    XXX(0x7b, UNDEF);	/* reserved */ \
	    XXX(0x7c, UNDEF);	/* reserved */ \
	    XXX(0x7d, UNDEF);	/* reserved */ \
	    XXX(0x7e, UNDEF);	/* reserved */ \
	} \
	end_CODEPAGE()
	/* figure A-27 "DEC 8-Bit Turkish Supplemental Character Set" */
#define map_DEC_Turkish_Supp(code) \
	begin_CODEPAGE(); \
	switch (code) { \
	    UNI(0x21, 0x00A1);	/* INVERTED EXCLAMATION MARK */ \
	    UNI(0x22, 0x00A2);	/* CENT SIGN */ \
	    UNI(0x23, 0x00A3);	/* POUND SIGN */ \
	    XXX(0x24, UNDEF);	/* reserved */ \
	    UNI(0x25, 0x00A5);	/* YEN SIGN */ \
	    XXX(0x26, UNDEF);	/* reserved */ \
	    UNI(0x27, 0x00A7);	/* SECTION SIGN */ \
	    UNI(0x28, 0x00A8);	/* DIAERESIS */ \
	    UNI(0x29, 0x00A9);	/* COPYRIGHT SIGN */ \
	    UNI(0x2A, 0x00AA);	/* FEMININE ORDINAL INDICATOR */ \
	    UNI(0x2B, 0x00AB);	/* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
	    XXX(0x2C, UNDEF);	/* reserved */ \
	    XXX(0x2D, UNDEF);	/* reserved */ \
	    UNI(0x2E, 0x0130);	/* LATIN CAPITAL LETTER I WITH DOT ABOVE */ \
	    XXX(0x2F, UNDEF);	/* reserved */ \
	    UNI(0x30, 0x00B0);	/* DEGREE SIGN */ \
	    UNI(0x31, 0x00B1);	/* PLUS-MINUS SIGN */ \
	    UNI(0x32, 0x00B2);	/* SUPERSCRIPT TWO */ \
	    UNI(0x33, 0x00B3);	/* SUPERSCRIPT THREE */ \
	    XXX(0x34, UNDEF);	/* reserved */ \
	    UNI(0x35, 0x00B5);	/* MICRO SIGN */ \
	    UNI(0x36, 0x00B6);	/* PILCROW SIGN */ \
	    UNI(0x37, 0x00B7);	/* MIDDLE DOT */ \
	    XXX(0x38, UNDEF);	/* reserved */ \
	    UNI(0x39, 0x00B9);	/* SUPERSCRIPT ONE */ \
	    UNI(0x3A, 0x00BA);	/* MASCULINE ORDINAL INDICATOR */ \
	    UNI(0x3B, 0x00BB);	/* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */ \
	    UNI(0x3C, 0x00BC);	/* VULGAR FRACTION ONE QUARTER */ \
	    UNI(0x3D, 0x00BD);	/* VULGAR FRACTION ONE HALF */ \
	    UNI(0x3E, 0x0131);	/* LATIN SMALL LETTER DOTLESS I */ \
	    UNI(0x3F, 0x00BF);	/* INVERTED QUESTION MARK */ \
	    UNI(0x40, 0x00C0);	/* LATIN CAPITAL LETTER A WITH GRAVE */ \
	    UNI(0x41, 0x00C1);	/* LATIN CAPITAL LETTER A WITH ACUTE */ \
	    UNI(0x42, 0x00C2);	/* LATIN CAPITAL LETTER A WITH CIRCUMFLEX */ \
	    UNI(0x43, 0x00C3);	/* LATIN CAPITAL LETTER A WITH TILDE */ \
	    UNI(0x44, 0x00C4);	/* LATIN CAPITAL LETTER A WITH DIAERESIS */ \
	    UNI(0x45, 0x00C5);	/* LATIN CAPITAL LETTER A WITH RING ABOVE */ \
	    UNI(0x46, 0x00C6);	/* LATIN CAPITAL LETTER AE */ \
	    UNI(0x47, 0x00C7);	/* LATIN CAPITAL LETTER C WITH CEDILLA */ \
	    UNI(0x48, 0x00C8);	/* LATIN CAPITAL LETTER E WITH GRAVE */ \
	    UNI(0x49, 0x00C9);	/* LATIN CAPITAL LETTER E WITH ACUTE */ \
	    UNI(0x4A, 0x00CA);	/* LATIN CAPITAL LETTER E WITH CIRCUMFLEX */ \
	    UNI(0x4B, 0x00CB);	/* LATIN CAPITAL LETTER E WITH DIAERESIS */ \
	    UNI(0x4C, 0x00CC);	/* LATIN CAPITAL LETTER I WITH GRAVE */ \
	    UNI(0x4D, 0x00CD);	/* LATIN CAPITAL LETTER I WITH ACUTE */ \
	    UNI(0x4E, 0x00CE);	/* LATIN CAPITAL LETTER I WITH CIRCUMFLEX */ \
	    UNI(0x4F, 0x00CF);	/* LATIN CAPITAL LETTER I WITH DIAERESIS */ \
	    UNI(0x50, 0x011E);	/* LATIN CAPITAL LETTER G WITH BREVE */ \
	    UNI(0x51, 0x00D1);	/* LATIN CAPITAL LETTER N WITH TILDE */ \
	    UNI(0x52, 0x00D2);	/* LATIN CAPITAL LETTER O WITH GRAVE */ \
	    UNI(0x53, 0x00D3);	/* LATIN CAPITAL LETTER O WITH ACUTE */ \
	    UNI(0x54, 0x00D4);	/* LATIN CAPITAL LETTER O WITH CIRCUMFLEX */ \
	    UNI(0x55, 0x00D5);	/* LATIN CAPITAL LETTER O WITH TILDE */ \
	    UNI(0x56, 0x00D6);	/* LATIN CAPITAL LETTER O WITH DIAERESIS */ \
	    UNI(0x57, 0x0152);	/* LATIN CAPITAL LIGATURE OE */ \
	    UNI(0x58, 0x00D8);	/* LATIN CAPITAL LETTER O WITH STROKE */ \
	    UNI(0x59, 0x00D9);	/* LATIN CAPITAL LETTER U WITH GRAVE */ \
	    UNI(0x5A, 0x00DA);	/* LATIN CAPITAL LETTER U WITH ACUTE */ \
	    UNI(0x5B, 0x00DB);	/* LATIN CAPITAL LETTER U WITH CIRCUMFLEX */ \
	    UNI(0x5C, 0x00DC);	/* LATIN CAPITAL LETTER U WITH DIAERESIS */ \
	    UNI(0x5D, 0x0178);	/* LATIN CAPITAL LETTER Y WITH DIAERESIS */ \
	    UNI(0x5E, 0x015E);	/* LATIN CAPITAL LETTER S WITH CEDILLA */ \
	    UNI(0x5F, 0x00DF);	/* LATIN SMALL LETTER SHARP S */ \
	    UNI(0x60, 0x00E0);	/* LATIN SMALL LETTER A WITH GRAVE */ \
	    UNI(0x61, 0x00E1);	/* LATIN SMALL LETTER A WITH ACUTE */ \
	    UNI(0x62, 0x00E2);	/* LATIN SMALL LETTER A WITH CIRCUMFLEX */ \
	    UNI(0x63, 0x00E3);	/* LATIN SMALL LETTER A WITH TILDE */ \
	    UNI(0x64, 0x00E4);	/* LATIN SMALL LETTER A WITH DIAERESIS */ \
	    UNI(0x65, 0x00E5);	/* LATIN SMALL LETTER A WITH RING ABOVE */ \
	    UNI(0x66, 0x00E6);	/* LATIN SMALL LETTER AE */ \
	    UNI(0x67, 0x00E7);	/* LATIN SMALL LETTER C WITH CEDILLA */ \
	    UNI(0x68, 0x00E8);	/* LATIN SMALL LETTER E WITH GRAVE */ \
	    UNI(0x69, 0x00E9);	/* LATIN SMALL LETTER E WITH ACUTE */ \
	    UNI(0x6A, 0x00EA);	/* LATIN SMALL LETTER E WITH CIRCUMFLEX */ \
	    UNI(0x6B, 0x00EB);	/* LATIN SMALL LETTER E WITH DIAERESIS */ \
	    UNI(0x6C, 0x00EC);	/* LATIN SMALL LETTER I WITH GRAVE */ \
	    UNI(0x6D, 0x00ED);	/* LATIN SMALL LETTER I WITH ACUTE */ \
	    UNI(0x6E, 0x00EE);	/* LATIN SMALL LETTER I WITH CIRCUMFLEX */ \
	    UNI(0x6F, 0x00EF);	/* LATIN SMALL LETTER I WITH DIAERESIS */ \
	    UNI(0x70, 0x011F);	/* LATIN SMALL LETTER G WITH BREVE */ \
	    UNI(0x71, 0x00F1);	/* LATIN SMALL LETTER N WITH TILDE */ \
	    UNI(0x72, 0x00F2);	/* LATIN SMALL LETTER O WITH GRAVE */ \
	    UNI(0x73, 0x00F3);	/* LATIN SMALL LETTER O WITH ACUTE */ \
	    UNI(0x74, 0x00F4);	/* LATIN SMALL LETTER O WITH CIRCUMFLEX */ \
	    UNI(0x75, 0x00F5);	/* LATIN SMALL LETTER O WITH TILDE */ \
	    UNI(0x76, 0x00F6);	/* LATIN SMALL LETTER O WITH DIAERESIS */ \
	    UNI(0x77, 0x0153);	/* LATIN SMALL LIGATURE OE */ \
	    UNI(0x78, 0x00F8);	/* LATIN SMALL LETTER O WITH STROKE */ \
	    UNI(0x79, 0x00F9);	/* LATIN SMALL LETTER U WITH GRAVE */ \
	    UNI(0x7A, 0x00FA);	/* LATIN SMALL LETTER U WITH ACUTE */ \
	    UNI(0x7B, 0x00FB);	/* LATIN SMALL LETTER U WITH CIRCUMFLEX */ \
	    UNI(0x7C, 0x00FC);	/* LATIN SMALL LETTER U WITH DIAERESIS */ \
	    UNI(0x7D, 0x00FF);	/* LATIN SMALL LETTER Y WITH DIAERESIS */ \
	    UNI(0x7E, 0x015F);	/* LATIN SMALL LETTER S WITH CEDILLA */ \
	} \
	end_CODEPAGE()
	/*
	 * mentioned, but not documented in VT510 manual, etc., this uses
	 * the ELOT927 table from Kermit 95:
	 */
#define map_NRCS_Greek(code) \
	switch (code) { \
	    MAP(0x61, 0x0391); /* CAPITAL GREEK LETTER ALPHA */ \
	    MAP(0x62, 0x0392); /* CAPITAL GREEK LETTER BETA */ \
	    MAP(0x63, 0x0393); /* CAPITAL GREEK LETTER GAMMA */ \
	    MAP(0x64, 0x0394); /* CAPITAL GREEK LETTER DELTA */ \
	    MAP(0x65, 0x0395); /* CAPITAL GREEK LETTER EPSILON */ \
	    MAP(0x66, 0x0396); /* CAPITAL GREEK LETTER ZETA */ \
	    MAP(0x67, 0x0397); /* CAPITAL GREEK LETTER ETA */ \
	    MAP(0x68, 0x0398); /* CAPITAL GREEK LETTER THETA */ \
	    MAP(0x69, 0x0399); /* CAPITAL GREEK LETTER IOTA */ \
	    MAP(0x6a, 0x039a); /* CAPITAL GREEK LETTER KAPPA */ \
	    MAP(0x6b, 0x039b); /* CAPITAL GREEK LETTER LAMDA */ \
	    MAP(0x6c, 0x039c); /* CAPITAL GREEK LETTER MU */ \
	    MAP(0x6d, 0x039d); /* CAPITAL GREEK LETTER NU */ \
	    MAP(0x6e, 0x03a7); /* CAPITAL GREEK LETTER KSI (CHI) */ \
	    MAP(0x6f, 0x039f); /* CAPITAL GREEK LETTER OMICRON */ \
	    MAP(0x70, 0x03a0); /* CAPITAL GREEK LETTER PI */ \
	    MAP(0x71, 0x03a1); /* CAPITAL GREEK LETTER RHO */ \
	    MAP(0x72, 0x03a3); /* CAPITAL GREEK LETTER SIGMA */ \
	    MAP(0x73, 0x03a4); /* CAPITAL GREEK LETTER TAU */ \
	    MAP(0x74, 0x03a5); /* CAPITAL GREEK LETTER UPSILON */ \
	    MAP(0x75, 0x03a6); /* CAPITAL GREEK LETTER FI (PHI) */ \
	    MAP(0x76, 0x039e); /* CAPITAL GREEK LETTER XI */ \
	    MAP(0x77, 0x03a8); /* CAPITAL GREEK LETTER PSI */ \
	    MAP(0x78, 0x03a9); /* CAPITAL GREEK LETTER OMEGA */ \
	    XXX(0x79, BLANK);  /* unused */ \
	    XXX(0x7a, BLANK);  /* unused */ \
	}
	/* figure A-21 "DEC 7-Bit Hebrew Character Set" */
#define map_NRCS_Hebrew(code) \
	switch (code) { \
	    UNI(0x60, 0x05d0);	/* HEBREW LETTER ALEF */ \
	    UNI(0x61, 0x05d1);	/* HEBREW LETTER BET */ \
	    UNI(0x62, 0x05d2);	/* HEBREW LETTER GIMEL */ \
	    UNI(0x63, 0x05d3);	/* HEBREW LETTER DALET */ \
	    UNI(0x64, 0x05d4);	/* HEBREW LETTER HE */ \
	    UNI(0x65, 0x05d5);	/* HEBREW LETTER VAV */ \
	    UNI(0x66, 0x05d6);	/* HEBREW LETTER ZAYIN */ \
	    UNI(0x67, 0x05d7);	/* HEBREW LETTER HET */ \
	    UNI(0x68, 0x05d8);	/* HEBREW LETTER TET */ \
	    UNI(0x69, 0x05d9);	/* HEBREW LETTER YOD */ \
	    UNI(0x6a, 0x05da);	/* HEBREW LETTER FINAL KAF */ \
	    UNI(0x6b, 0x05db);	/* HEBREW LETTER KAF */ \
	    UNI(0x6c, 0x05dc);	/* HEBREW LETTER LAMED */ \
	    UNI(0x6d, 0x05dd);	/* HEBREW LETTER FINAL MEM */ \
	    UNI(0x6e, 0x05de);	/* HEBREW LETTER MEM */ \
	    UNI(0x6f, 0x05df);	/* HEBREW LETTER FINAL NUN */ \
	    UNI(0x70, 0x05e0);	/* HEBREW LETTER NUN */ \
	    UNI(0x71, 0x05e1);	/* HEBREW LETTER SAMEKH */ \
	    UNI(0x72, 0x05e2);	/* HEBREW LETTER AYIN */ \
	    UNI(0x73, 0x05e3);	/* HEBREW LETTER FINAL PE */ \
	    UNI(0x74, 0x05e4);	/* HEBREW LETTER PE */ \
	    UNI(0x75, 0x05e5);	/* HEBREW LETTER FINAL TSADI */ \
	    UNI(0x76, 0x05e6);	/* HEBREW LETTER TSADI */ \
	    UNI(0x77, 0x05e7);	/* HEBREW LETTER QOF */ \
	    UNI(0x78, 0x05e8);	/* HEBREW LETTER RESH */ \
	    UNI(0x79, 0x05e9);	/* HEBREW LETTER SHIN */ \
	    UNI(0x7a, 0x05ea);	/* HEBREW LETTER TAV */ \
	}
	/* figure A-26 "DEC 7-Bit Turkish Character Set" */
#define map_NRCS_Turkish(code) \
	switch (code) { \
	    UNI(0x26, 0x011f);	/* LATIN SMALL LETTER G WITH BREVE */ \
	    UNI(0x40, 0x0130);	/* LATIN CAPITAL LETTER I WITH DOT ABOVE */ \
	    UNI(0x5b, 0x015e);	/* LATIN CAPITAL LETTER S WITH CEDILLA */ \
	    UNI(0x5c, 0x00d6);	/* LATIN CAPITAL LETTER O WITH DIAERESIS */ \
	    UNI(0x5d, 0x00c7);	/* LATIN CAPITAL LETTER C WITH CEDILLA */ \
	    UNI(0x5e, 0x00dC);	/* LATIN CAPITAL LETTER U WITH DIAERESIS */ \
	    UNI(0x60, 0x011e);	/* LATIN CAPITAL LETTER G WITH BREVE */ \
	    UNI(0x7b, 0x015f);	/* LATIN SMALL LETTER S WITH CEDILLA */ \
	    UNI(0x7c, 0x00f6);	/* LATIN SMALL LETTER O WITH DIAERESIS */ \
	    UNI(0x7d, 0x00e7);	/* LATIN SMALL LETTER C WITH CEDILLA */ \
	    UNI(0x7e, 0x00fc);	/* LATIN SMALL LETTER U WITH DIAERESIS */ \
	}
#else
#define map_DEC_Cyrillic(code)	/* nothing */
#define map_DEC_Greek_Supp(code)	/* nothing */
#define map_DEC_Hebrew_Supp(code)	/* nothing */
#define map_DEC_Technical(code)	/* nothing */
#define map_DEC_Turkish_Supp(code)	/* nothing */
#define map_ISO_Greek_Supp(code)	/* nothing */
#define map_ISO_Hebrew(code)	/* nothing */
#define map_ISO_Latin_5(code)	/* nothing */
#define map_ISO_Latin_Cyrillic(code)	/* nothing */
#define map_NRCS_Greek(code)	/* nothing */
#define map_NRCS_Hebrew(code)	/* nothing */
#define map_NRCS_Turkish(code)	/* nothing */
#endif /* OPT_WIDE_CHARS */

/*
 * Translate an input keysym to the corresponding NRC keysym.
 */
unsigned
xtermCharSetIn(XtermWidget xw, unsigned code, DECNRCM_codes charset)
{
    TScreen *screen = TScreenOf(xw);
#define MAP(to, from) case from: code = to; break

#if OPT_WIDE_CHARS
#define UNI(to, from) case from: if (screen->utf8_nrc_mode) code = to; break
#else
#define UNI(to, from) case from: break
#endif

#define XXX(to, from)		/* no defined mapping to 0..255 */

    TRACE(("CHARSET-IN GL=%s(G%d) GR=%s(G%d) SS%d\n\t%s\n",
	   visibleScsCode(screen->gsets[screen->curgl]), screen->curgl,
	   visibleScsCode(screen->gsets[screen->curgr]), screen->curgr,
	   screen->curss,
	   visibleUChar(code)));

    (void) screen;
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

    case nrc_ISO_Latin_5_Supp:
	map_ISO_Latin_5(code);
	break;

    case nrc_ISO_Latin_Cyrillic:
	map_ISO_Latin_Cyrillic(code);
	break;

    case nrc_Norwegian_Danish:
    case nrc_Norwegian_Danish2:
    case nrc_Norwegian_Danish3:
	map_NRCS_Norwegian_Danish(code);
	break;

    case nrc_Portugese:
	map_NRCS_Portuguese(code);
	break;

    case nrc_SCS_NRCS:		/* vt5xx - probably Serbo/Croatian */
	/* FIXME */
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

    case nrc_Cyrillic:
	map_DEC_Cyrillic(code);
	break;

    case nrc_British_Latin_1:
    case nrc_Russian:
    case nrc_French_Canadian2:
    case nrc_Unknown:
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
int
xtermCharSetOut(XtermWidget xw, IChar *buf, IChar *ptr, DECNRCM_codes leftset)
{
    IChar *s;
    TScreen *screen = TScreenOf(xw);
    int count = 0;
    DECNRCM_codes rightset = screen->gsets[(int) (screen->curgr)];

#define MAP(from, to) case from: chr = to; break

#if OPT_WIDE_CHARS
#define UNI(from, to) case from: if (screen->utf8_nrc_mode) chr = to; break
#define XXX(from, to) UNI(from, to)
#else
#define UNI(old, new) chr = old; break
#define XXX(from, to)		/* nothing */
#endif

    TRACE(("CHARSET-OUT GL=%s(G%d) GR=%s(G%d) SS%d\n\t%s\n",
	   visibleScsCode(leftset), screen->curgl,
	   visibleScsCode(rightset), screen->curgr,
	   screen->curss,
	   visibleIChars(buf, (unsigned) (ptr - buf))));

    for (s = buf; s < ptr; ++s) {
	int eight = CharOf(E2A(*s));
	int seven = eight & 0x7f;
	DECNRCM_codes cs = (eight >= 128) ? rightset : leftset;
	int chr = eight;

	count++;
#if OPT_WIDE_CHARS
	/*
	 * This is only partly right - prevent inadvertant remapping of
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
	case nrc_British_Latin_1:
	    /* FALLTHRU */
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
	    } else {
		chr = (seven | 0x80);
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
	    map_DEC_Supp_Graphic(chr = seven, chr |= 0x80);
	    break;

	case nrc_DEC_Supp_Graphic:
	    map_DEC_Supp_Graphic(chr = seven, chr |= 0x80);
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

	case nrc_ISO_Latin_5_Supp:
	    map_ISO_Latin_5(chr = seven);
	    break;

	case nrc_ISO_Latin_Cyrillic:
	    map_ISO_Latin_Cyrillic(chr = seven);
	    break;

	case nrc_Norwegian_Danish:
	case nrc_Norwegian_Danish2:
	case nrc_Norwegian_Danish3:
	    map_NRCS_Norwegian_Danish(chr = seven);
	    break;

	case nrc_Portugese:
	    map_NRCS_Portuguese(chr = seven);
	    break;

	case nrc_SCS_NRCS:	/* vt5xx - probably Serbo/Croatian */
	    /* FIXME */
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

	case nrc_Cyrillic:
	    map_DEC_Cyrillic(chr = seven);
	    break;

	case nrc_Russian:
	case nrc_Unknown:
	default:		/* any character sets we don't recognize */
	    count--;
	    break;
	}
	/*
	 * The state machine already treated DEL as a nonprinting and
	 * nonspacing character.  If we have DEL now, simply render
	 * it as a blank.
	 */
	if (chr == ANSI_DEL)
	    chr = ' ';
	*s = (IChar) A2E(chr);
    }
    TRACE(("%d\t%s\n",
	   count,
	   visibleIChars(buf, (unsigned) (ptr - buf))));
    return count;
#undef MAP
#undef UNI
#undef XXX
}
