/*
 
Copyright 1990, 1998  The Open Group

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

 * Copyright 1990 Network Computing Devices;
 * Portions Copyright 1987 by Digital Equipment Corporation 
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of Network Computing Devices, or Digital
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 *
 * NETWORK COMPUTING DEVICES, AND DIGITAL DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL NETWORK COMPUTING DEVICES,
 * OR DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 * THIS SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <X11/fonts/FSlib.h>
#include <stdio.h>
#include <X11/Xos.h>
#include <stdlib.h>

#ifndef N_START
#define N_START 1000		/* Maximum # of fonts to start with */
#endif

static unsigned int max_output_line_width = 79;
static unsigned int output_line_padding = 3;
static unsigned int columns = 0;

#define L_SHORT 0
#define L_MEDIUM 1
#define L_LONG 2
#define L_VERYLONG 3

static Bool sort_output = True;
static int  long_list = L_SHORT;
static int  nnames = N_START;
static unsigned int  font_cnt;
static int  min_max;
typedef struct {
    char       *name;
    FSXFontInfoHeader *info;
    FSPropInfo *pi;
    FSPropOffset *po;
    unsigned char *pd;
}           FontList;
static FontList *font_list;

static FSServer *svr;

static char *program_name;

static void usage (const char *msg) _X_NORETURN _X_COLD;
static void get_list ( const char *pattern );
static int compare ( const void *f1, const void *f2 );
static void show_fonts ( void );
static void print_font_header ( void );
static void show_font_header ( FontList *list );
static void copy_number ( char **pp1, char **pp2, int n1, int n2 );
static void show_font_props ( FontList *list );

static void _X_NORETURN _X_COLD
missing_arg (const char *option)
{
    char msg[32];

    snprintf(msg, sizeof(msg), "%s requires an argument", option);
    usage(msg);
}

static void
usage(const char *msg)
{
    if (msg)
	fprintf(stderr, "%s: %s\n", program_name, msg);
    fprintf(stderr, "usage:  %s [-options] [-fn pattern]\n", program_name);
    fprintf(stderr, "%s", "where options include:\n"
	    "    -l[l[l]]                 give long info about each font\n"
	    "    -m                       give character min and max bounds\n"
	    "    -C                       force columns\n"
	    "    -1                       force single column\n"
	    "    -u                       keep output unsorted\n"
	    "    -w width                 maximum width for multiple columns\n"
	    "    -n columns               number of columns if multi column\n"
	    "    -server servername       font server to contact\n"
	    "    -version                 print command version and exit\n"
	    "\n");
    exit(1);
}

int
main(int argc, char *argv[])
{
    int         argcnt = 0,
                i;
    char       *servername = NULL;

    program_name = argv[0];

    for (i = 1; i < argc; i++) {
	if (strncmp(argv[i], "-s", 2) == 0) {
	    if (++i >= argc)
		missing_arg("-server");
	    servername = argv[i];
	}
	else if (strcmp(argv[i], "-version") == 0) {
	    printf("%s\n", PACKAGE_STRING);
	    exit(0);
	}
    }

    if ((svr = FSOpenServer(servername)) == NULL) {
	if (FSServerName(servername) == NULL) {
	    usage("no font server defined");
	}
	fprintf(stderr, "%s:  unable to open server \"%s\"\n",
		program_name, FSServerName(servername));
	exit(0);
    }
    /* Handle command line arguments, open display */
    for (argv++, argc--; argc; argv++, argc--) {
	if (argv[0][0] == '-') {
	    if (argcnt > 0)
		usage(NULL);
	    for (i = 1; argv[0][i]; i++)
		switch (argv[0][i]) {
		case 'l':
		    long_list++;
		    break;
		case 'm':
		    min_max++;
		    break;
		case 'C':
		    columns = 0;
		    break;
		case '1':
		    columns = 1;
		    break;
		case 'f':
		    if (--argc <= 0)
			missing_arg("-fn");
		    argcnt++;
		    argv++;
		    get_list(argv[0]);
		    goto next;
		case 'w':
		    if (--argc <= 0)
			missing_arg("-w");
		    argv++;
		    max_output_line_width = (unsigned int) atoi(argv[0]);
		    goto next;
		case 'n':
		    if (--argc <= 0)
			missing_arg("-n");
		    argv++;
		    columns = (unsigned int) atoi(argv[0]);
		    goto next;
		case 'u':
		    sort_output = False;
		    break;
		case 's':	/* eat -s */
		    if (--argc <= 0)
			missing_arg("-server");
		    argv++;
		    goto next;
		default:
		    fprintf(stderr, "%s: unrecognized option '%s'\n",
			    program_name, argv[0]);
		    usage(NULL);
		}
	    if (i == 1)
		usage(NULL);
	} else {
	    argcnt++;
	    get_list(argv[0]);
	}
next:	;
    }
    if (argcnt == 0)
	get_list("*");
    FSCloseServer(svr);
    show_fonts();
    exit(0);
}

static void
get_list(const char *pattern)
{
    int         available = nnames + 1,
                i;
    char      **fonts;
    FSXFontInfoHeader **info;
    FSPropInfo **props;
    FSPropOffset **offsets;
    unsigned char **pdata;

    /* Get list of fonts matching pattern */
    for (;;) {

	if (long_list >= L_MEDIUM)
	    fonts = FSListFontsWithXInfo(svr,
	       pattern, nnames, &available, &info, &props, &offsets, &pdata);
	else
	    fonts = FSListFonts(svr, pattern, nnames, &available);
	if (fonts == NULL || available < nnames)
	    break;

	if (long_list >= L_MEDIUM) {
	    for (i = 0; i < available; i++) {
		FSFree((char *) fonts[i]);
		FSFree((char *) info[i]);
		FSFree((char *) props[i]);
		FSFree((char *) offsets[i]);
		FSFree((char *) pdata[i]);
	    }
	    FSFree((char *) fonts);
	    FSFree((char *) info);
	    FSFree((char *) props);
	    FSFree((char *) offsets);
	    FSFree((char *) pdata);
	} else {
	    FSFreeFontNames(fonts);
	}
	nnames = available * 2;
    }

    if (fonts == NULL) {
	fprintf(stderr, "%s: pattern \"%s\" unmatched\n",
		program_name, pattern);
	return;
    }
    if (font_list)
	font_list = realloc(font_list, (font_cnt + (unsigned) available)
                            * sizeof(FontList));
    else
	font_list = malloc((font_cnt + (unsigned) available)
                           * sizeof(FontList));
    if (font_list == NULL) {
        fprintf(stderr, "%s: unable to allocate %zu bytes for font list\n",
                program_name,
                (font_cnt + (unsigned) available) * sizeof(FontList));
        exit(-1);
    }
    for (i = 0; i < available; i++) {
	font_list[font_cnt].name = fonts[i];

	if (long_list >= L_MEDIUM) {
	    font_list[font_cnt].info = info[i];
	    font_list[font_cnt].pi = props[i];
	    font_list[font_cnt].po = offsets[i];
	    font_list[font_cnt].pd = pdata[i];
	} else
	    font_list[font_cnt].info = NULL;
	font_cnt++;
    }
}

static int 
compare(const void *f1, const void *f2)
{
    const char *p1 = ((const FontList *)f1)->name,
               *p2 = ((const FontList *)f2)->name;

    while (*p1 && *p2 && *p1 == *p2)
	p1++, p2++;
    return (*p1 - *p2);
}

static void
show_fonts(void)
{
    unsigned int i;

    if (font_cnt == 0)
	return;

    /* first sort the output */
    if (sort_output)
	qsort(font_list, font_cnt, sizeof(FontList), compare);

    if (long_list > L_MEDIUM) {
	print_font_header();
	for (i = 0; i < font_cnt; i++) {
	    show_font_header(&font_list[i]);
	    show_font_props(&font_list[i]);
	}
	return;
    }
    if (long_list == L_MEDIUM) {
	print_font_header();

	for (i = 0; i < font_cnt; i++) {
	    show_font_header(&font_list[i]);
	}

	return;
    }
    if ((columns == 0 && isatty(1)) || columns > 1) {
	unsigned int width,
	            max_width = 0,
	            lines_per_column,
	            j,
	            index;

	for (i = 0; i < font_cnt; i++) {
	    width = (unsigned int) strlen(font_list[i].name);
	    if (width > max_width)
		max_width = width;
	}
	if (max_width == 0) {
	    fprintf(stderr, "all %d fontnames listed are zero length",
		    font_cnt);
	    exit(-1);
	}
	if (columns == 0) {
	    if ((max_width * 2) + output_line_padding >
		    max_output_line_width) {
		columns = 1;
	    } else {
		max_width += output_line_padding;
		columns = ((max_output_line_width +
			    output_line_padding) / max_width);
	    }
	} else {
	    max_width += output_line_padding;
	}
	if (columns <= 1)
	    goto single_column;

	if (font_cnt < columns)
	    columns = font_cnt;
	lines_per_column = (font_cnt + columns - 1) / columns;

	for (i = 0; i < lines_per_column; i++) {
	    for (j = 0; j < columns; j++) {
		index = j * lines_per_column + i;
		if (index >= font_cnt)
		    break;
		if (j + 1 == columns)
		    printf("%s", font_list[index].name);
		else
		    printf("%-*s",
			   max_width,
			   font_list[index].name);
	    }
	    printf("\n");
	}
	return;
    }
single_column:
    for (i = 0; i < font_cnt; i++)
	printf("%s\n", font_list[i].name);
}

static void
print_font_header(void)
{
    printf("DIR  ");
    printf("MIN  ");
    printf("MAX ");
    printf("EXIST ");
    printf("DFLT ");
    printf("ASC ");
    printf("DESC ");
    printf("NAME");
    printf("\n");
}

static void
show_font_header(FontList *list)
{
    const char        *string;
    FSXFontInfoHeader *pfh;

    pfh = list->info;
    if (!pfh) {
	fprintf(stderr,
		"%s:  no font information for font \"%s\".\n",
		program_name, list->name ? list->name : "");
	return;
    }
    if (pfh->draw_direction == LeftToRightDrawDirection)
	string = "-->";
    else
	string = "<--";
    printf("%-4s", string);
    if (pfh->char_range.min_char.high == 0
	    && pfh->char_range.max_char.high == 0) {
	printf(" %3d ", pfh->char_range.min_char.low);
	printf(" %3d ", pfh->char_range.max_char.low);
    } else {
	printf("*%3d ", pfh->char_range.min_char.high);
	printf("*%3d ", pfh->char_range.max_char.high);
    }
    printf("%5s ", (pfh->flags & FontInfoAllCharsExist) ? "all" : "some");
    printf("%4d ", (pfh->default_char.high << 8) + pfh->default_char.low);
    printf("%3d ", pfh->font_ascent);
    printf("%4d ", pfh->font_descent);
    printf("%s\n", list->name);
    if (min_max) {
	char        min[BUFSIZ],
	            max[BUFSIZ];
	char       *pmax = max,
	           *pmin = min;

	strcpy(pmin, "     min(l,r,w,a,d) = (");
	strcpy(pmax, "     max(l,r,w,a,d) = (");
	pmin += strlen(pmin);
	pmax += strlen(pmax);

	copy_number(&pmin, &pmax,
		    pfh->min_bounds.left,
		    pfh->max_bounds.left);
	*pmin++ = *pmax++ = ',';
	copy_number(&pmin, &pmax,
		    pfh->min_bounds.right,
		    pfh->max_bounds.right);
	*pmin++ = *pmax++ = ',';
	copy_number(&pmin, &pmax,
		    pfh->min_bounds.width,
		    pfh->max_bounds.width);
	*pmin++ = *pmax++ = ',';
	copy_number(&pmin, &pmax,
		    pfh->min_bounds.ascent,
		    pfh->max_bounds.ascent);
	*pmin++ = *pmax++ = ',';
	copy_number(&pmin, &pmax,
		    pfh->min_bounds.descent,
		    pfh->max_bounds.descent);
	*pmin++ = *pmax++ = ')';
	*pmin = *pmax = '\0';
	printf("%s\n", min);
	printf("%s\n", max);
    }
}

#ifndef max
#define	max(a, b)	((a) > (b) ? (a) : (b))
#endif

static void
copy_number(char **pp1, char **pp2, int n1, int n2)
{
    char       *p1 = *pp1;
    char       *p2 = *pp2;
    int         w;

    sprintf(p1, "%d", n1);
    sprintf(p2, "%d", n2);
    w = (int) max(strlen(p1), strlen(p2));
    sprintf(p1, "%*d", w, n1);
    sprintf(p2, "%*d", w, n2);
    p1 += strlen(p1);
    p2 += strlen(p2);
    *pp1 = p1;
    *pp2 = p2;
}

static void
show_font_props(FontList *list)
{
    unsigned int  i;
    char        buf[1000];
    FSPropInfo *pi = list->pi;
    FSPropOffset *po = list->po;
    unsigned char *pd = list->pd;
    unsigned int  num_props;

    num_props = pi->num_offsets;
    for (i = 0; i < num_props; i++, po++) {
	strncpy(buf, (char *) (pd + po->name.position), po->name.length);
	buf[po->name.length] = '\0';
	printf("%s\t", buf);
	switch (po->type) {
	case PropTypeString:
	    strncpy(buf, (char *)pd + po->value.position, po->value.length);
	    buf[po->value.length] = '\0';
	    printf("%s\n", buf);
	    break;
	case PropTypeUnsigned:
	    printf("%lu\n", (unsigned long) po->value.position);
	    break;
	case PropTypeSigned:
	    printf("%lu\n", (long) po->value.position);
	    break;
	default:
	    fprintf(stderr, "bogus property\n");
	    break;
	}

    }
}
