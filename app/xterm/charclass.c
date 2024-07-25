/* $XTermId: charclass.c,v 1.50 2023/04/01 00:11:47 tom Exp $ */

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
 *----------------------------------------------------------------------------
 * Compact and efficient reimplementation of the
 * xterm character class mechanism for large character sets
 *
 * Markus Kuhn -- mkuhn@acm.org -- 2000-07-03
 *
 * xterm allows users to select entire words with a double-click on the left
 * mouse button.  Opinions might differ on what type of characters are part of
 * separate words, therefore xterm allows users to configure a class code for
 * each 8-bit character.  Words are maximum length sequences of neighboring
 * characters with identical class code.  Extending this mechanism to Unicode
 * naively would create an at least 2^16 entries (128 kB) long class code
 * table.
 *
 * Instead, we transform the character class table into a list of intervals,
 * that will be accessed via a linear search.  Changes made to the table by the
 * user will be appended.  A special class code IDENT (default) marks
 * characters who have their code number as the class code.
 *
 * We could alternatively use a sorted table of non-overlapping intervals that
 * can be accessed via binary search, but merging in new intervals is
 * significantly more hassle and not worth the effort here.
 */

#include <xterm.h>
#include <charclass.h>

#if OPT_WIDE_CHARS

#ifdef TEST_DRIVER

#include <ctype.h>
#include <wchar.h>
#include <wctype.h>

#if OPT_TRACE
#define Trace if (opt_v) printf
#endif

#undef OPT_REPORT_CCLASS
#define OPT_REPORT_CCLASS 1
#endif /* TEST_DRIVER */

static struct classentry {
    int cclass;
    int first;
    int last;
} *classtab;

#ifdef TEST_DRIVER
static int opt_all;
static int opt_check;
static int opt_quiet;
static int opt_v;
#endif

void
init_classtab(void)
{
    const int size = 50;

    TRACE(("init_classtab " TRACE_L "\n"));

    classtab = TypeMallocN(struct classentry, (unsigned) size);
    if (!classtab)
	abort();
    classtab[0].cclass = size;
    classtab[0].first = 1;
    classtab[0].last = 0;

    /* old xterm default classes */
    SetCharacterClassRange(0, 0, BLANK);
    SetCharacterClassRange(1, 31, CNTRL);
    SetCharacterClassRange('\t', '\t', BLANK);
    SetCharacterClassRange('0', '9', ALNUM);
    SetCharacterClassRange('A', 'Z', ALNUM);
    SetCharacterClassRange('_', '_', ALNUM);
    SetCharacterClassRange('a', 'z', ALNUM);
    SetCharacterClassRange(127, 159, CNTRL);
    SetCharacterClassRange(160, 191, IDENT);
    SetCharacterClassRange(192, 255, ALNUM);
    SetCharacterClassRange(215, 215, IDENT);
    SetCharacterClassRange(247, 247, IDENT);

    /* added Unicode classes */
    SetCharacterClassRange(0x0100, 0xffdf, ALNUM);	/* mostly characters */
    SetCharacterClassRange(0x037e, 0x037e, IDENT);	/* Greek question mark */
    SetCharacterClassRange(0x0387, 0x0387, IDENT);	/* Greek ano teleia */
    SetCharacterClassRange(0x055a, 0x055f, IDENT);	/* Armenian punctuation */
    SetCharacterClassRange(0x0589, 0x0589, IDENT);	/* Armenian full stop */
    SetCharacterClassRange(0x0700, 0x070d, IDENT);	/* Syriac punctuation */
    SetCharacterClassRange(0x104a, 0x104f, IDENT);	/* Myanmar punctuation */
    SetCharacterClassRange(0x10fb, 0x10fb, IDENT);	/* Georgian punctuation */
    SetCharacterClassRange(0x1361, 0x1368, IDENT);	/* Ethiopic punctuation */
    SetCharacterClassRange(0x166d, 0x166e, IDENT);	/* Canadian Syl. punctuation */
    SetCharacterClassRange(0x17d4, 0x17dc, IDENT);	/* Khmer punctuation */
    SetCharacterClassRange(0x1800, 0x180a, IDENT);	/* Mongolian punctuation */
    SetCharacterClassRange(0x2000, 0x200a, BLANK);	/* spaces */
    SetCharacterClassRange(0x200b, 0x200f, CNTRL);	/* formatting */
    SetCharacterClassRange(0x2010, 0x27ff, IDENT);	/* punctuation and symbols */
    SetCharacterClassRange(0x202a, 0x202e, CNTRL);	/* formatting */
    SetCharacterClassRange(0x2060, 0x206f, CNTRL);	/* formatting */
    SetCharacterClassRange(0x2070, 0x207f, U_SUP);	/* superscript */
    SetCharacterClassRange(0x2080, 0x208f, U_SUB);	/* subscript */
    SetCharacterClassRange(0x3000, 0x3000, BLANK);	/* ideographic space */
    SetCharacterClassRange(0x3001, 0x3020, IDENT);	/* ideographic punctuation */
    SetCharacterClassRange(0x3040, 0x309f, U_HIR);	/* Hiragana */
    SetCharacterClassRange(0x30a0, 0x30ff, U_KAT);	/* Katakana */
    SetCharacterClassRange(0x3300, 0x9fff, U_CJK);	/* CJK Ideographs */
    SetCharacterClassRange(0xac00, 0xd7a3, U_HAN);	/* Hangul Syllables */
    SetCharacterClassRange(0xf900, 0xfaff, U_CJK);	/* CJK Ideographs */
    SetCharacterClassRange(0xfe30, 0xfe6b, IDENT);	/* punctuation forms */
    SetCharacterClassRange(0xfeff, 0xfeff, CNTRL);	/* formatting */
    SetCharacterClassRange(0xff00, 0xff0f, IDENT);	/* half/fullwidth ASCII */
    SetCharacterClassRange(0xff1a, 0xff20, IDENT);	/* half/fullwidth ASCII */
    SetCharacterClassRange(0xff3b, 0xff40, IDENT);	/* half/fullwidth ASCII */
    SetCharacterClassRange(0xff5b, 0xff64, IDENT);	/* half/fullwidth ASCII */
    SetCharacterClassRange(0xfff9, 0xfffb, CNTRL);	/* formatting */

    TRACE((TRACE_R " init_classtab\n"));
    return;
}

int
CharacterClass(int c)
{
    int i, cclass = IDENT;

    for (i = classtab[0].first; i <= classtab[0].last; i++)
	if (classtab[i].first <= c && classtab[i].last >= c)
	    cclass = classtab[i].cclass;

    if (cclass < 0)
	cclass = c;

    return cclass;
}

#if OPT_REPORT_CCLASS
#define charFormat(code) ((code) > 255 ? "0x%04X" : "%d")
static const char *
class_name(Classes code)
{
    static char buffer[80];
    const char *result = "?";
    switch (code) {
    case ALNUM:
	result = "ALNUM";
	break;
    case BLANK:
	result = "BLANK";
	break;
    case CNTRL:
	result = "CNTRL";
	break;
    case OTHER:
	result = "OTHER";
	break;
    case IDENT:
	result = "IDENT";
	break;
    case U_SUP:
	result = "superscript";
	break;
    case U_SUB:
	result = "subscript";
	break;
    case U_CJK:
	result = "CJK Ideographs";
	break;
    case U_HIR:
	result = "Hiragana";
	break;
    case U_KAT:
	result = "Katakana";
	break;
    case U_HAN:
	result = "Hangul Syllables";
	break;
    default:
	sprintf(buffer, charFormat(code), code);
	result = buffer;
	break;
    }
    return result;
}

/*
 * Special convention for classtab[0]:
 * - classtab[0].cclass is the allocated number of entries in classtab
 * - classtab[0].first = 1 (first used entry in classtab)
 * - classtab[0].last is the last used entry in classtab
 */

int
SetCharacterClassRange(int low, int high, int value)
{
    TRACE(("...SetCharacterClassRange (U+%04X .. U+%04X) = %s\n",
	   low, high, class_name(value)));

    if (high < low)
	return -1;		/* nothing to do */

    /* make sure we have at least one free entry left at table end */
    if (classtab[0].last > classtab[0].cclass - 2) {
	classtab[0].cclass += 5 + classtab[0].cclass / 4;
	classtab = TypeRealloc(struct classentry,
			         (unsigned) classtab[0].cclass, classtab);
	if (!classtab)
	    abort();
    }

    /* simply append new interval to end of interval array */
    classtab[0].last++;
    classtab[classtab[0].last].first = low;
    classtab[classtab[0].last].last = high;
    classtab[classtab[0].last].cclass = value;

    return 0;
}

void
report_wide_char_class(void)
{
    static const Classes known_classes[] =
    {IDENT, ALNUM, CNTRL, BLANK, U_SUP, U_SUB, U_HIR, U_KAT, U_CJK, U_HAN};
    int i;

    printf("\n");
    printf("Unicode charClass data uses the last match\n");
    printf("from these overlapping intervals of character codes:\n");
    for (i = classtab[0].first; i <= classtab[0].last; i++) {
	printf("\tU+%04X .. U+%04X %s\n",
	       (unsigned) classtab[i].first,
	       (unsigned) classtab[i].last,
	       class_name((Classes) classtab[i].cclass));
    }
    printf("\n");
    printf("These class-names are used internally (the first character code in a class):\n");
    for (i = 0; i < (int) XtNumber(known_classes); ++i) {
	printf("\t");
	printf(charFormat(known_classes[i]), known_classes[i]);
	printf(" = %s\n", class_name(known_classes[i]));
    }
}
#endif /* OPT_REPORT_CCLASS */

#ifdef NO_LEAKS
void
noleaks_CharacterClass(void)
{
    FreeAndNull(classtab);
}
#endif
#endif /* OPT_WIDE_CHARS */

#ifdef TEST_DRIVER
#if OPT_WIDE_CHARS
static void
usage(void)
{
    static const char *msg[] =
    {
	"Usage: test_charclass [options] [c1[-c1b] [c2-[c2b] [...]]]",
	"",
	"Options:",
	" -a  show all data",
	" -s  show only summary",
	" -v  verbose"
    };
    size_t n;
    for (n = 0; n < sizeof(msg) / sizeof(msg[0]); ++n) {
	fprintf(stderr, "%s\n", msg[n]);
    }
    exit(EXIT_FAILURE);
}

static int
expected_class(int wch)
{
    int result = wch;
    wint_t ch = (wint_t) wch;
    if (wch < 0 || ch == '\0' || ch == '\t') {
	result = BLANK;
    } else if (iswcntrl(ch)) {
	result = CNTRL;
    } else if (iswspace(ch)) {
	result = BLANK;
    } else if (ch < 127) {
	if (isalnum(ch) || ch == '_') {
	    result = ALNUM;
	}
    } else if (ch == 170 || ch == 181 || ch == 186) {
	;
    } else if (iswalnum(ch)) {
	result = ALNUM;
    }
    return result;
}

static int
show_cclass_range(int lo, int hi)
{
    int cclass = CharacterClass(lo);
    int ident = (cclass == lo);
    int more = 0;
    if (ident) {
	int ch;
	for (ch = lo + 1; ch <= hi; ch++) {
	    if (CharacterClass(ch) != ch) {
		ident = 0;
		break;
	    }
	}
	if (ident && (hi < 255)) {
	    ch = hi + 1;
	    if (CharacterClass(ch) == ch) {
		if (ch >= 255 || CharacterClass(ch + 1) != ch) {
		    more = 1;
		}
	    }
	}
    }
    if (!more) {
	if (lo == hi) {
	    printf("\t%d", lo);
	} else {
	    printf("\t%d-%d", lo, hi);
	}
	if (!ident)
	    printf(":%d", cclass);
	if (hi < 255)
	    printf(", \\");
	printf("\n");
    }
    return !more;
}

static void
report_resource(int first, int last)
{
    int class_p;
    int ch;
    int dh;

    class_p = CharacterClass(dh = first);
    for (ch = first; ch < last; ++ch) {
	int class_c = CharacterClass(ch);
	if (class_c != class_p) {
	    if (show_cclass_range(dh, ch - 1)) {
		dh = ch;
		class_p = class_c;
	    }
	}
    }
    if (dh < last - 1) {
	show_cclass_range(dh, last - 1);
    }
}

static int
decode_one(const char *source, char **target)
{
    int result = -1;
    long check;
    int radix = 0;
    if ((source[0] == 'u' || source[0] == 'U') && source[1] == '+') {
	source += 2;
	radix = 16;
    }
    check = strtol(source, target, radix);
    if (*target != NULL && *target != source)
	result = (int) check;
    return result;
}

static int
decode_range(const char *source, int *lo, int *hi)
{
    int result = 0;
    char *after1;
    char *after2;
    if ((*lo = decode_one(source, &after1)) >= 0) {
	after1 += strspn(after1, ":-.\t ");
	if ((*hi = decode_one(after1, &after2)) < 0) {
	    *hi = *lo;
	}
	result = 1;
    }
    return result;
}

static void
do_range(const char *source)
{
    int lo, hi;
    if (decode_range(source, &lo, &hi)) {
	if (opt_all) {
	    while (lo <= hi) {
		int other_rc = CharacterClass(lo);
		if (!opt_quiet)
		    printf("U+%04X\t%s\n", lo, class_name(other_rc));
		++lo;
	    }
	} else if (opt_check) {
	    while (lo <= hi) {
		int expect = expected_class(lo);
		int actual = CharacterClass(lo);
		if (actual != expect)
		    printf("U+%04X\t%s ->%s\n", lo,
			   class_name(expect),
			   class_name(actual));
		++lo;
	    }
	} else {
	    printf("\"charClass\" resource for [%d..%d]:\n", lo, hi);
	    report_resource(lo, hi + 1);
	}
    }
}
#endif /* OPT_WIDE_CHARS */

/*
 * TODO: add option to show do_range in hex
 */
int
main(int argc, char **argv ENVP_ARG)
{
#if OPT_WIDE_CHARS
    int ch;
#endif

    (void) argc;
    (void) argv;

#if OPT_WIDE_CHARS
    setlocale(LC_ALL, "");
    while ((ch = getopt(argc, argv, "acsv")) != -1) {
	switch (ch) {
	case 'a':
	    opt_all = 1;
	    break;
	case 'c':
	    opt_check = 1;
	    break;
	case 's':
	    opt_quiet = 1;
	    break;
	case 'v':
	    opt_v = 1;
	    break;
	default:
	    usage();
	}
    }
    init_classtab();

    if (optind >= argc) {
	do_range("0-255");
    } else {
	while (optind < argc) {
	    do_range(argv[optind++]);
	}
    }
    report_wide_char_class();
#else
    printf("wide-character support is not configured\n");
#endif /* OPT_WIDE_CHARS */
    return 0;
}
#endif /* TEST_DRIVER */
