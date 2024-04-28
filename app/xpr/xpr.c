/*

Copyright (c) 1985  X Consortium

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
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
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/

/*
 * XPR - process xwd(1) files for various printers
 *
 * Author: Michael R. Gretzinger, MIT Project Athena
 *
 * Modified by Marvin Solomon, University of Wisconsin, to handle Apple
 * Laserwriter (PostScript) devices (-device ps).
 * Also accepts the -compact flag that produces more compact output
 * by using run-length encoding on white (1) pixels.
 * This version does not (yet) support the following options
 *   -append -dump -noff -split
 *
 * Changes
 * Copyright 1986 by Marvin Solomon and the University of Wisconsin
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the names of Marvin Solomon and
 * the University of Wisconsin not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 * Neither Marvin Solomon nor the University of Wisconsin
 * makes any representations about the suitability of
 * this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * Modified by Bob Scheifler for 2x2 grayscale, then ...
 * Modified by Angela Bock and E. Mike Durbin, Rich Inc., to produce output
 * using 2x2, 3x3, or 4x4 grayscales. This version modifies the grayscale
 * conversion option of -gray to accept an input of  2, 3, or 4 to signify
 * the gray level desired.  The output is produced, using 5, 10, or 17-level
 * gray scales, respectively.
 *
 * Modifications by Larry Rupp, Hewlett-Packard Company, to support HP
 * LaserJet, PaintJet, and other PCL printers.  Added "ljet" and "pjet"
 * to devices recognized.  Also added -density, -cutoff, and -noposition
 * command line options.
 *
 * Modified by Jay Hobson, Sun Microsystems to support paper sizes other
 * than 8.5x11 inches.
 *
 * Copyright (c) 2002, Oracle and/or its affiliates.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <X11/Xos.h>
#include <X11/Xfuncs.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <string.h>
#ifndef WIN32
#include <pwd.h>
#endif
#include "xpr.h"
#include <X11/XWDFile.h>
#include <X11/Xmu/SysUtil.h>
#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifdef	NLS16
#ifndef NLS
#define	NLS
#endif
#endif

#ifndef NLS
#define catgets(i, sn,mn,s) (s)
#else /* NLS */
#define NL_SETN 1	/* set number */
#include <nl_types.h>

nl_catd nlmsg_fd;
#endif /* NLS */

int debug = 0;

#define W_MAX 2400
#define H_MAX 3150
#define W_MARGIN 75
#define H_MARGIN 37
#define W_PAGE 2550
#define H_PAGE 3225

#define PAPER_WIDTH 85*30 /* 8.5 inches */
#define PAPER_LENGTH 11*300 /* 11 inches */

#ifdef NOINLINE
#define min(x,y) (((x)<(y))?(x):(y))
#endif /* NOINLINE */

#define F_PORTRAIT 1
#define F_LANDSCAPE 2
#define F_APPEND 16
#define F_NOFF 32
#define F_REPORT 64
#define F_COMPACT 128
#define F_INVERT 256
#define F_GRAY 512
#define F_NPOSITION 1024
#define F_SLIDE 2048

#define DEFAULT_CUTOFF ((unsigned int) (0xFFFF * 0.50))

static const char *infilename = NULL;
const char *progname   = NULL;

struct _grayRec {
    int level;
    int sizeX, sizeY;		/* 2x2, 3x3, 4x4 */
    const unsigned long *grayscales;	/* pointer to the encoded pixels */
};
typedef const struct _grayRec GrayRec, *GrayPtr;

static const unsigned long grayscale2x2[] =
	{0, 1, 9, 11, 15};
static const unsigned long grayscale3x3[] =
	{0, 16, 68, 81, 325, 341, 349, 381, 383, 511};
static const unsigned long grayscale4x4[] =
	{0, 64, 4160, 4161, 20545, 21057, 23105,
	 23113, 23145, 24169, 24171, 56939, 55275, 55279,
	 57327, 65519, 65535};

static GrayRec gray2x2 = {sizeof(grayscale2x2)/sizeof(long), 2, 2, grayscale2x2};
static GrayRec gray3x3 = {sizeof(grayscale3x3)/sizeof(long), 3, 3, grayscale3x3};
static GrayRec gray4x4 = {sizeof(grayscale4x4)/sizeof(long), 4, 4, grayscale4x4};

typedef struct {
	char *name;
	double width;
	double length;
} PaperRec;

PaperRec paper_size[] = {
	{"Letter", 8.5,	 11.0 },
	{"Legal",  8.5,	 14.0 },
	{"A0",	  33.11, 46.81},
	{"A1",	  23.39, 33.11},
	{"A2",	  16.5,	 23.39},
	{"A3",	  11.7,	 16.5 },
	{"A4",	   8.27, 11.7 },
	{"A5",	   5.83,  8.27},
	{"A6",	   4.13,  5.83},
	{"A7",	   2.91,  4.13},
	{"A8",	   2.05,  2.91},
	{"A9",	   1.46,  2.05},
	{"A10",	   1.02,  1.46},
	{"B0",	  39.37, 55.67},
	{"B1",	  27.83, 39.37},
	{"B2",	  19.69, 27.83},
	{"B3",	  13.90, 19.69},
	{"B4",	   9.84, 13.90},
	{"B5",	   6.93,  9.84},
	{"B6",	   4.92,  6.93},
	{"B7",	   3.46,  4.92},
	{"B8",	   2.44,  3.46},
	{"B9",	   1.73,  2.44},
	{"B10",	   1.22,  1.73},
	{"C0",	  36.10, 51.06},
	{"C1",	  25.51, 36.10},
	{"C2",	  18.03, 25.51},
	{"C3",	  12.76, 18.03},
	{"C4",	   9.02, 12.76},
	{"C5",	   6.38,  9.02},
	{"C6",	   4.49,  6.38},
	{"C7",	   3.19,  4.49},
	{"C8",	   2.24,  3.19},
	{"C9",	   1.57,  2.24},
	{"C10",	   1.10,  1.57}
};


/* mapping tables to map a byte in to the hex representation of its
 * bit-reversal
 */
static const
char hex1[]="084c2a6e195d3b7f084c2a6e195d3b7f084c2a6e195d3b7f084c2a6e195d3b7f\
084c2a6e195d3b7f084c2a6e195d3b7f084c2a6e195d3b7f084c2a6e195d3b7f\
084c2a6e195d3b7f084c2a6e195d3b7f084c2a6e195d3b7f084c2a6e195d3b7f\
084c2a6e195d3b7f084c2a6e195d3b7f084c2a6e195d3b7f084c2a6e195d3b7f";

static const
char hex2[]="000000000000000088888888888888884444444444444444cccccccccccccccc\
2222222222222222aaaaaaaaaaaaaaaa6666666666666666eeeeeeeeeeeeeeee\
111111111111111199999999999999995555555555555555dddddddddddddddd\
3333333333333333bbbbbbbbbbbbbbbb7777777777777777ffffffffffffffff";


/* Local prototypes */
static void usage(void) _X_NORETURN;
static
void parse_args(
  int argc,
  char **argv,
  int *scale,
  int *width,
  int *height,
  int *left,
  int *top,
  enum device *device,
  int *flags,
  int *split,
  char **header,
  char **trailer,
  int *plane,
  GrayPtr *gray,
  int *density,
  unsigned int *cutoff,
  float *gamma,
  int *render,
  int *paper_width,
  int *paper_length);
static
void setup_layout(
  enum device device,
  int win_width,
  int win_height,
  int flags,
  int width,
  int height,
  char *header,
  char *trailer,
  int *scale,
  enum orientation *orientation,
  int wmax,
  int hmax);
static
char *convert_data(
    XWDFileHeader *win,
    char *data,
    int plane,
    GrayPtr gray,
    XColor *colors,
    int flags);
static void dump_prolog(int flags);
static int points(int n);
static char *escape(const char *s);
static
void ps_setup(
  int iw,
  int ih,
  enum orientation orientation,
  int scale,
  int left,
  int top,
  int flags,
  const char *header,
  const char *trailer,
  const char *name,
  int paper_width,
  int paper_length);
static void ps_finish(void);
static void ps_output_bits(
  int iw,
  int ih,
  int flags,
  enum orientation orientation,
  XWDFileHeader *win,
  const char *data);
static int ps_putbuf(
  register unsigned char *s,
  register int n,
  register int ocount,
  int compact);
static void ps_bitrot(
  unsigned char *s,
  register int n,
  int col,
  register int owidth,
  char *obuf);
static void fullread (
  int file,
  char *data,
  int nbytes);

int main(int argc, char **argv)
{
    unsigned long swaptest = 1;
    XWDFileHeader win;
    register int i;
    register int iw;
    register int ih;
    char *w_name;
    int scale, width, height, flags, split;
    int left, top;
    char *header, *trailer;
    int plane;
    int density, render;
    unsigned int cutoff;
    float gamma;
    GrayPtr gray;
    char *data;
    long size;
    enum orientation orientation;
    enum device device;
    XColor *colors = (XColor *)NULL;
    int paper_width  = PAPER_WIDTH;
    int paper_length = PAPER_LENGTH;
    int w_max        = W_MAX;
    int h_max        = W_MAX;

    if (!(progname = argv[0]))
      progname = "xpr";
#ifdef	NLS
    nlmsg_fd = catopen("xpr", 0);
#endif
    parse_args (argc, argv, &scale, &width, &height, &left, &top, &device,
		&flags, &split, &header, &trailer, &plane, &gray,
		&density, &cutoff, &gamma, &render, &paper_width, &paper_length);

    if (paper_width != PAPER_WIDTH)
       w_max = paper_width - 150;
    if (paper_length != PAPER_LENGTH)
       h_max = paper_length - 150;

    if (device == PP) {
	x2pmp(stdin, stdout, scale,
	      width >= 0? inch2pel((float)width/300.0): X_MAX_PELS,
	      height >= 0? inch2pel((float)height/300.0): Y_MAX_PELS,
	      left >= 0? inch2pel((float)left/300.0): inch2pel(0.60),
	      top >= 0? inch2pel((float)top/300.0): inch2pel(0.70),
	      header, trailer,
	      (flags & F_PORTRAIT)? PORTRAIT:
	        ((flags & F_LANDSCAPE)? LANDSCAPE: UNSPECIFIED),
	      (flags & F_INVERT));
	exit(0);
    } else if ((device == LJET) || (device == PJET) || (device == PJETXL)) {
        x2jet(stdin, stdout, scale, density, width, height, left, top,
	      header, trailer,
	      (flags & F_PORTRAIT)? PORTRAIT:
	      ((flags & F_LANDSCAPE)? LANDSCAPE: UNSPECIFIED),
	      (flags & F_INVERT),
	      ((flags & F_APPEND) && !(flags & F_NOFF)),
	      !(flags & F_NPOSITION),
	      (flags & F_SLIDE),
	      device, cutoff, gamma, render);
	exit(0);
    } else if (device != PS) {
	fprintf(stderr, "xpr: device not supported\n");
    }
    /* everything past here is for device == PS */

    /* read in window header */
    fullread(0, (char *)&win, sizeof win);
    if (*(char *) &swaptest)
	_swaplong((char *) &win, (long)sizeof(win));

    if (win.file_version != XWD_FILE_VERSION) {
	fprintf(stderr,"xpr: file format version mismatch.\n");
	exit(1);
    }
    if (win.header_size < sizeof(win)) {
	fprintf(stderr,"xpr: header size is too small.\n");
	exit(1);
    }

    w_name = malloc((unsigned)(win.header_size - sizeof win));
    fullread(0, w_name, (int) (win.header_size - sizeof win));

    if(win.ncolors) {
	XWDColor xwdcolor;
	colors = malloc((unsigned) (win.ncolors * sizeof(XColor)));
	for (i = 0; i < win.ncolors; i++) {
	    fullread(0, (char*)&xwdcolor, (int) sizeof xwdcolor);
	    colors[i].pixel = xwdcolor.pixel;
	    colors[i].red = xwdcolor.red;
	    colors[i].green = xwdcolor.green;
	    colors[i].blue = xwdcolor.blue;
	    colors[i].flags = xwdcolor.flags;
 	}
	if (*(char *) &swaptest) {
	    for (i = 0; i < win.ncolors; i++) {
		_swaplong((char *) &colors[i].pixel, (long)sizeof(long));
		_swapshort((char *) &colors[i].red, (long) (3 * sizeof(short)));
	    }
	}
	if ((win.ncolors == 2) &&
	    (INTENSITY(&colors[0]) > INTENSITY(&colors[1])))
	    flags ^= F_INVERT;
    }
    if (plane >= (long)win.pixmap_depth) {
	fprintf(stderr,"xpr: plane number exceeds image depth\n");
	exit(1);
    }
    size = win.bytes_per_line * win.pixmap_height;
    if (win.pixmap_format == XYPixmap)
	size *= win.pixmap_depth;
    data = malloc((unsigned)size);
    fullread(0, data, (int)size);
    if ((win.pixmap_depth > 1) || (win.byte_order != win.bitmap_bit_order)) {
	data = convert_data(&win, data, plane, gray, colors, flags);
	size = win.bytes_per_line * win.pixmap_height;
    }
    if (win.bitmap_bit_order == MSBFirst) {
	_swapbits((unsigned char *)data, size);
	win.bitmap_bit_order = LSBFirst;
    }
    if (flags & F_INVERT)
	_invbits((unsigned char *)data, size);

    /* calculate orientation and scale */
    setup_layout(device, (int) win.pixmap_width, (int) win.pixmap_height,
		 flags, width, height, header, trailer, &scale, &orientation,
		 w_max, h_max);

    iw = win.pixmap_width;
    ih = win.pixmap_height;

    ps_setup(iw, ih, orientation, scale, left, top,
             flags, header, trailer, w_name, paper_width, paper_length);
    ps_output_bits(iw, ih, flags, orientation, &win, data);
    ps_finish();

    /* print some statistics */
    if (flags & F_REPORT) {
	fprintf(stderr, "Name: %s\n", w_name);
	fprintf(stderr, "Width: %d, Height: %d\n", (int)win.pixmap_width,
		(int)win.pixmap_height);
	fprintf(stderr, "Orientation: %s, Scale: %d\n",
		(orientation==PORTRAIT) ? "Portrait" : "Landscape", scale);
    }
    exit(EXIT_SUCCESS);
}

static void _X_NORETURN _X_COLD
invalid_arg_value(const char *arg, const char *value)
{
    fprintf (stderr, "%s: %s is not a valid value for %s\n\n",
	     progname, value, arg);
    usage();
}

static void _X_NORETURN _X_COLD
missing_arg(const char *arg)
{
    fprintf (stderr, "%s: %s requires an argument\n\n", progname, arg);
    usage();
}

static void _X_NORETURN _X_COLD
unknown_arg(const char *arg)
{
    fprintf (stderr, "%s: unrecognized argument %s\n\n", progname, arg);
    usage();
}

static void _X_NORETURN _X_COLD
usage(void)
{
    fprintf(stderr, "usage: %s [options] [file]\n%s", progname,
	    "    -append <file>  -noff  -output <file>\n"
	    "    -compact\n"
	    "    -device {ps | lw | pp | ljet | pjet | pjetxl}\n"
	    "    -dump\n"
	    "    -gamma <correction>\n"
	    "    -gray {2 | 3 | 4}\n"
	    "    -height <inches>  -width <inches>\n"
	    "    -header <string>  -trailer <string>\n"
	    "    -landscape  -portrait\n"
	    "    -left <inches>  -top <inches>\n"
	    "    -noposition\n"
	    "    -papertype {letter | legal | A0-A10 | B0-B10 | C0-C10}\n"
	    "	 -pheight <inches>[in | cm | mm] -pwidth <inches>[in | cm | mm]\n"
	    "    -plane <n>\n"
	    "    -psfig\n"
	    "    -render <type>\n"
	    "    -report\n"
	    "    -rv\n"
	    "    -scale <scale>\n"
	    "    -slide\n"
	    "    -split <n-pages>\n"
	    "    -version\n"
	);
    exit(EXIT_FAILURE);
}

static
void parse_args(
  int argc,
  char **argv,
  int *scale,
  int *width,
  int *height,
  int *left,
  int *top,
  enum device *device,
  int *flags,
  int *split,
  char **header,
  char **trailer,
  int *plane,
  GrayPtr *gray,
  int *density,
  unsigned int *cutoff,
  float *gamma,
  int *render,
  int *paper_width,
  int *paper_length)
{
    register char *output_filename;
    register int f;

    output_filename = NULL;
    *device = PS;	/* default */
    *flags = 0;
    *scale = 0;
    *split = 1;
    *width = -1;
    *height = -1;
    *top = -1;
    *left = -1;
    *header = NULL;
    *trailer = NULL;
    *plane = -1;
    *gray = (GrayPtr)NULL;
    *density = 0;
    *cutoff = DEFAULT_CUTOFF;
    *gamma = -1.0;
    *render = 0;

    for (argc--, argv++; argc > 0; argc--, argv++) {
	const char *arg = argv[0];
	if (argv[0][0] != '-') {
	    infilename = *argv;
	    continue;
	}
	if (!strcmp(*argv, "-append")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    output_filename = *argv;
	    *flags |= F_APPEND;
	} else if (!strcmp(*argv, "-compact")) {
	    *flags |= F_COMPACT;
	} else if (!strcmp(*argv, "-cutoff")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    *cutoff = min((atof(*argv) / 100.0 * 0xFFFF), 0xFFFF);
	} else if (!strcmp(*argv, "-density")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    *density = atoi(*argv);
	} else if (!strcmp(*argv, "-device")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    if (!strcmp(*argv, "ps")) {
		*device = PS;
	    } else if (!strcmp(*argv, "lw")) {
		*device = PS;
	    } else if (!strcmp(*argv, "pp")) {
		*device = PP;
	    } else if (!strcmp(*argv, "ljet")) {
		*device = LJET;
	    } else if (!strcmp(*argv, "pjet")) {
		*device = PJET;
	    } else if (!strcmp(*argv, "pjetxl")) {
		*device = PJETXL;
	    } else
		invalid_arg_value(arg, argv[0]);
	} else if (!strcmp(*argv, "-gamma")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    *gamma = atof(*argv);
	} else if (!strcmp(*argv, "-gray") ||
		   !strcmp(*argv, "-grey")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    switch (atoi(*argv)) {
	      case 2:
		*gray = &gray2x2;
		break;
	      case 3:
		*gray = &gray3x3;
		break;
	      case 4:
		*gray = &gray4x4;
		break;
	      default:
		invalid_arg_value(arg, argv[0]);
	    }
	    *flags |= F_GRAY;
	} else if (!strcmp(*argv, "-height")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    *height = (int)(300.0 * atof(*argv));
	} else if (!strcmp(*argv, "-header")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    *header = *argv;
	} else if (!strcmp(*argv, "-landscape")) {
	    *flags |= F_LANDSCAPE;
	} else if (!strcmp(*argv, "-left")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    *left = (int)(300.0 * atof(*argv));
	} else if (!strcmp(*argv, "-noff")) {
	    *flags |= F_NOFF;
	} else if (!strcmp(*argv, "-noposition")) {
	    *flags |= F_NPOSITION;
	} else if (!strcmp(*argv, "-output")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    output_filename = *argv;
	} else if (!strcmp(*argv, "-portrait")) {
	    *flags |= F_PORTRAIT;
	} else if (!strcmp(*argv, "-papertype")) {
	    int found = 0;
	    int i;

	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    for (i = 0; i < sizeof (paper_size) / sizeof (PaperRec); i++)
	    {
		if (!strcasecmp(paper_size[i].name, *argv))
		{
		    found = 1;
		    *paper_width  = (int)(300.0 * paper_size[i].width);
		    *paper_length = (int)(300.0 * paper_size[i].length);
		}
	    }
	    if (!found) usage();
	} else if (!strcmp(*argv, "-plane")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    *plane = atoi(*argv);
	} else if (!strcmp(*argv, "-psfig")) {
	    *flags |= F_NPOSITION;
	} else if (!strcmp(*argv, "-pwidth")) {
	    double mult = 1.0;

	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    if (strstr(*argv, "cm"))
		mult = 2.54;
	    else if ( strstr ( *argv, "mm" ))
		mult = 25.4;
	    *paper_width = (int)(300.0 * atof(*argv) / mult);
	} else if (!strcmp(*argv, "-pheight")) {
	    double mult = 1.0;

	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    if ( strstr ( *argv, "cm" ))
		mult = 2.54;
	    else if ( strstr ( *argv, "mm" ))
		mult = 25.4;
	    *paper_length = (int)(300.0 * atof(*argv) / mult);
	} else if (!strcmp(*argv, "-rv")) {
	    *flags |= F_INVERT;
	} else if (!strcmp(*argv, "-render")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    *render = atoi(*argv);
	} else if (!strcmp(*argv, "-report")) {
	    *flags |= F_REPORT;
	} else if (!strcmp(*argv, "-scale")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    *scale = atoi(*argv);
	} else if (!strcmp(*argv, "-slide")) {
	    *flags |= F_SLIDE;
	} else if (!strcmp(*argv, "-split")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    *split = atoi(*argv);
	} else if (!strcmp(*argv, "-top")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    *top = (int)(300.0 * atof(*argv));
	} else if (!strcmp(*argv, "-trailer")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    *trailer = *argv;
	} else if (strcmp(*argv, "-version") == 0) {
	    puts(PACKAGE_STRING);
	    exit(0);
	} else if (!strcmp(*argv, "-width")) {
	    argc--; argv++;
	    if (argc == 0) missing_arg(arg);
	    *width = (int)(300.0 * atof(*argv));
	} else
	    unknown_arg(arg);
    }

    if (infilename) {
	f = open(infilename, O_RDONLY|O_BINARY);
	if (f < 0) {
	    fprintf(stderr, "xpr: error opening \"%s\" for input\n",
		    infilename);
	    perror("");
	    exit(1);
	}
	dup2(f, 0);
	close(f);
    } else
	infilename = "stdin";

    if (output_filename != NULL) {
	if (!(*flags & F_APPEND)) {
	    f = open(output_filename, O_CREAT|O_WRONLY|O_TRUNC, 0664);
	} else {
	    f = open(output_filename, O_WRONLY);
	}
	if (f < 0) {
	    fprintf(stderr, "xpr: error opening \"%s\" for output\n",
		    output_filename);
	    perror("xpr");
	    exit(1);
	}
	if (*flags & F_APPEND) {
	    off_t pos = lseek(f, 0, SEEK_END);	/* get eof position */
	    if ((*flags & F_NOFF) &&
		!(*device == LJET || *device == PJET || *device == PJETXL))
		pos -= 3; /* set position before trailing */
	    		  /*     formfeed and reset */
	    lseek(f, pos, SEEK_SET);		/* set pointer */
	}
	dup2(f, 1);
	close(f);
    }
}

static
void setup_layout(
  enum device device,
  int win_width,
  int win_height,
  int flags,
  int width,
  int height,
  char *header,
  char *trailer,
  int *scale,
  enum orientation *orientation,
  int wmax,
  int hmax)
{
    register int w_scale;
    register int h_scale;
    register int iscale = *scale;
    register int w_max;
    register int h_max;

    if (header != NULL) win_height += 75;
    if (trailer != NULL) win_height += 75;

    /* check maximum width and height; set orientation and scale*/
    if (device == PS) {
	if ((win_width < win_height || (flags & F_PORTRAIT)) &&
	    !(flags & F_LANDSCAPE)) {
	    *orientation = PORTRAIT;
	    w_max = (width > 0)? width : wmax;
	    h_max = (height > 0)? height : hmax;
	    w_scale = w_max / win_width;
	    h_scale = h_max / win_height;
	    *scale = min(w_scale, h_scale);
	} else {
	    *orientation = LANDSCAPE;
	    w_max = (width > 0)? width : hmax;
	    h_max = (height > 0)? height : wmax;
	    w_scale = w_max / win_width;
	    h_scale = h_max / win_height;
	    *scale = min(w_scale, h_scale);
	}
    } else {
	*orientation = PORTRAIT;
	*scale = wmax / win_width;
    }
    if (*scale == 0) *scale = 1;
    if (*scale > 6) *scale = 6;
    if (iscale > 0 && iscale < *scale) *scale = iscale;
}

static
char *convert_data(
    XWDFileHeader *win,
    char *data,
    int plane,
    GrayPtr gray,
    XColor *colors,
    int flags)
{
    XImage in_image_struct, out_image_struct;
    register XImage *in_image, *out_image;
    register int x, y;

    if ((win->pixmap_format == XYPixmap) && (plane >= 0)) {
	data += win->bytes_per_line * win->pixmap_height *
		(win->pixmap_depth - (plane + 1));
	win->pixmap_format = XYBitmap;
	win->pixmap_depth = 1;
	return data;
    }

    /* initialize the input image */

    in_image = &in_image_struct;
    in_image->byte_order = win->byte_order;
    in_image->bitmap_unit = win->bitmap_unit;
    in_image->bitmap_bit_order = win->bitmap_bit_order;
    in_image->depth = win->pixmap_depth;
    in_image->bits_per_pixel = win->bits_per_pixel;
    in_image->format = win->pixmap_format,
    in_image->xoffset = win->xoffset,
    in_image->data = data;
    in_image->width = win->pixmap_width;
    in_image->height = win->pixmap_height;
    in_image->bitmap_pad = win->bitmap_pad;
    in_image->bytes_per_line = win->bytes_per_line;
    in_image->red_mask = win->red_mask;
    in_image->green_mask = win->green_mask;
    in_image->blue_mask = win->blue_mask;
    in_image->obdata = NULL;
    if (!XInitImage(in_image)) {
	fprintf(stderr,"xpr: bad input image header data.\n");
	exit(1);
    }
    if ((flags & F_GRAY) && (in_image->depth > 1) && (plane < 0)) {
	win->pixmap_width *= gray->sizeX;
	win->pixmap_height *= gray->sizeY;
    }
    win->xoffset = 0;
    win->pixmap_format = XYBitmap;
    win->byte_order = LSBFirst;
    win->bitmap_unit = 8;
    win->bitmap_bit_order = LSBFirst;
    win->bitmap_pad = 8;
    win->pixmap_depth = 1;
    win->bits_per_pixel = 1;
    win->bytes_per_line = (win->pixmap_width + 7) >> 3;

    out_image = &out_image_struct;
    out_image->byte_order = win->byte_order;
    out_image->bitmap_unit = win->bitmap_unit;
    out_image->bitmap_bit_order = win->bitmap_bit_order;
    out_image->depth = win->pixmap_depth;
    out_image->bits_per_pixel = win->bits_per_pixel;
    out_image->format = win->pixmap_format;
    out_image->xoffset = win->xoffset,
    out_image->width = win->pixmap_width;
    out_image->height = win->pixmap_height;
    out_image->bitmap_pad = win->bitmap_pad;
    out_image->bytes_per_line = win->bytes_per_line;
    out_image->red_mask = 0;
    out_image->green_mask = 0;
    out_image->blue_mask = 0;
    out_image->obdata = NULL;
    out_image->data = malloc((unsigned)out_image->bytes_per_line *
				      out_image->height);
    if (!XInitImage(out_image)) {
	fprintf(stderr,"xpr: bad output image header data.\n");
	exit(1);
    }

    if ((in_image->depth > 1) && (plane > 0)) {
	for (y = 0; y < in_image->height; y++)
	    for (x = 0; x < in_image->width; x++)
		XPutPixel(out_image, x, y,
			  (XGetPixel(in_image, x, y) >> plane) & 1);
    } else if (plane == 0) {
	for (y = 0; y < in_image->height; y++)
	    for (x = 0; x < in_image->width; x++)
		XPutPixel(out_image, x, y, XGetPixel(in_image, x, y));
    } else if ((in_image->depth > 1) &&
	       ((win->visual_class == TrueColor) ||
		(win->visual_class == DirectColor))) {
	XColor color;
	int direct = 0;
	unsigned long rmask, gmask, bmask;
	int rshift = 0, gshift = 0, bshift = 0;

	rmask = win->red_mask;
	while (!(rmask & 1)) {
	    rmask >>= 1;
	    rshift++;
	}
	gmask = win->green_mask;
	while (!(gmask & 1)) {
	    gmask >>= 1;
	    gshift++;
	}
	bmask = win->blue_mask;
	while (!(bmask & 1)) {
	    bmask >>= 1;
	    bshift++;
	}
	if ((win->ncolors == 0) || (win->visual_class == DirectColor))
	    direct = 1;
	if (flags & F_GRAY) {
	    register int ox, oy;
	    int ix, iy;
	    unsigned long bits;
	    for (y = 0, oy = 0; y < in_image->height; y++, oy += gray->sizeY)
		for (x = 0, ox = 0; x < in_image->width; x++, ox += gray->sizeX)
		{
		    color.pixel = XGetPixel(in_image, x, y);
		    color.red = (color.pixel >> rshift) & rmask;
		    color.green = (color.pixel >> gshift) & gmask;
		    color.blue = (color.pixel >> bshift) & bmask;
		    if (!direct) {
			color.red = colors[color.red].red;
			color.green = colors[color.green].green;
			color.blue = colors[color.blue].blue;
		    }
		    bits = gray->grayscales[(int)(gray->level *
						  INTENSITY(&color)) /
					    (INTENSITYPER(100) + 1)];
		    for (iy = 0; iy < gray->sizeY; iy++)
			for (ix = 0; ix < gray->sizeX; ix++, bits >>= 1)
			    XPutPixel(out_image, ox + ix, oy + iy, bits);
		}
	} else {
	    for (y = 0; y < in_image->height; y++)
		for (x = 0; x < in_image->width; x++) {
		    color.pixel = XGetPixel(in_image, x, y);
		    color.red = (color.pixel >> rshift) & rmask;
		    color.green = (color.pixel >> gshift) & gmask;
		    color.blue = (color.pixel >> bshift) & bmask;
		    if (!direct) {
			color.red = colors[color.red].red;
			color.green = colors[color.green].green;
			color.blue = colors[color.blue].blue;
		    }
		    XPutPixel(out_image, x, y,
			      INTENSITY(&color) > HALFINTENSITY);
		}
	}
    } else if (flags & F_GRAY) {
	register int ox, oy;
	int ix, iy;
	unsigned long bits;

	if (win->ncolors == 0) {
	    fprintf(stderr, "no colors in data, can't remap\n");
	    exit(1);
	}
	for (x = 0; x < win->ncolors; x++) {
	    register XColor *color = &colors[x];

	    color->pixel = gray->grayscales[(gray->level * INTENSITY(color)) /
					    (INTENSITYPER(100) + 1)];
	}
	for (y = 0, oy = 0; y < in_image->height; y++, oy += gray->sizeY)
	    for (x = 0, ox = 0; x < in_image->width; x++, ox += gray->sizeX) {
		bits = colors[XGetPixel(in_image, x, y)].pixel;
	        for (iy = 0; iy < gray->sizeY; iy++)
		    for (ix = 0; ix < gray->sizeX; ix++, bits >>= 1)
			XPutPixel(out_image, ox + ix, oy + iy, bits);
	    }
    } else {
	if (win->ncolors == 0) {
	    fprintf(stderr, "no colors in data, can't remap\n");
	    exit(1);
	}
	for (x = 0; x < win->ncolors; x++) {
	    register XColor *color = &colors[x];
	    color->pixel = (INTENSITY(color) > HALFINTENSITY);
	}
	for (y = 0; y < in_image->height; y++)
	    for (x = 0; x < in_image->width; x++)
		XPutPixel(out_image, x, y,
			  colors[XGetPixel(in_image, x, y)].pixel);
    }
    free(data);
    return (out_image->data);
}

#define COMMENTVERSION "PS-Adobe-1.0"

#ifdef XPROLOG
/* for debugging, get the prolog from a file */
static
void dump_prolog(int flags)
{
    char *fname=(flags & F_COMPACT) ? "prolog.compact" : "prolog";
    FILE *fi = fopen(fname,"r");
    char buf[1024];

    if (fi==NULL) {
	perror(fname);
	exit(1);
    }
    while (fgets(buf,1024,fi)) fputs(buf,stdout);
    fclose(fi);
}

#else /* XPROLOG */
/* postscript "programs" to unpack and print the bitmaps being sent */

static const
char * const ps_prolog_compact[] = {
    "%%Pages: 1",
    "%%EndProlog",
    "%%Page: 1 1",
    "",
    "/bitgen",
    "	{",
    "		/nextpos 0 def",
    "		currentfile bufspace readhexstring pop % get a chunk of input",
    "		% interpret each byte of the input",
    "		{",
    "			flag { % if the previous byte was FF",
    "				/len exch def % this byte is a count",
    "				result",
    "				nextpos",
    "				FFstring 0 len getinterval % grap a chunk of FF's",
    "					putinterval % and stuff them into the result",
    "				/nextpos nextpos len add def",
    "				/flag false def",
    "			}{ % otherwise",
    "				dup 255 eq { % if this byte is FF",
    "					/flag true def % just set the flag",
    "					pop % and toss the FF",
    "				}{ % otherwise",
    "					% move this byte to the result",
    "					result nextpos",
    "						3 -1 roll % roll the current byte back to the top",
    "						put",
    "					/nextpos nextpos 1 add def",
    "				} ifelse",
    "			} ifelse",
    "		} forall",
    "		% trim unused space from end of result",
    "		result 0 nextpos getinterval",
    "	} def",
    "",
    "",
    "/bitdump % stk: width, height, iscale",
    "	% dump a bit image with lower left corner at current origin,",
    "	% scaling by iscale (iscale=1 means 1/300 inch per pixel)",
    "	{",
    "		% read arguments",
    "		/iscale exch def",
    "		/height exch def",
    "		/width exch def",
    "",
    "		% scale appropriately",
    "		width iscale mul height iscale mul scale",
    "",
    "		% data structures:",
    "",
    "		% allocate space for one line of input",
    "		/bufspace 36 string def",
    "",
    "		% string of FF's",
    "		/FFstring 256 string def",
    "		% for all i FFstring[i]=255",
    "		0 1 255 { FFstring exch 255 put } for",
    "",
    "		% 'escape' flag",
    "		/flag false def",
    "",
    "		% space for a chunk of generated bits",
    "		/result 4590 string def",
    "",
    "		% read and dump the image",
    "		width height 1 [width 0 0 height neg 0 height]",
    "			{ bitgen }",
    "			image",
    "	} def",
    NULL
};

static const
char * const ps_prolog[] = {
    "%%Pages: 1",
    "%%EndProlog",
    "%%Page: 1 1",
    "",
    "/bitdump % stk: width, height, iscale",
    "% dump a bit image with lower left corner at current origin,",
    "% scaling by iscale (iscale=1 means 1/300 inch per pixel)",
    "{",
    "	% read arguments",
    "	/iscale exch def",
    "	/height exch def",
    "	/width exch def",
    "",
    "	% scale appropriately",
    "	width iscale mul height iscale mul scale",
    "",
    "	% allocate space for one scanline of input",
    "	/picstr % picstr holds one scan line",
    "		width 7 add 8 idiv % width of image in bytes = ceiling(width/8)",
    "		string",
    "		def",
    "",
    "	% read and dump the image",
    "	width height 1 [width 0 0 height neg 0 height]",
    "	{ currentfile picstr readhexstring pop }",
    "	image",
    "} def",
    NULL
};

static
void dump_prolog(int flags) {
    const char * const *p =
        (flags & F_COMPACT) ? ps_prolog_compact : ps_prolog;
    while (*p)
        printf("%s\n", *p++);
}
#endif /* XPROLOG */

#define PAPER_WIDTH 85*30 /* 8.5 inches */
#define PAPER_LENGTH 11*300 /* 11 inches */

static
int points(int n)
{
    /* scale n from pixels (1/300 inch) to points (1/72 inch) */
    n *= 72;
    return n/300;
}

static
char *escape(const char *s)
{
    /* make a version of s in which control characters are deleted and
     * special characters are escaped.
     */
    static char buf[200];
    char *p = buf;

    for (;*s;s++) {
	if (*s < ' ' || *s > 0176) continue;
	if (*s==')' || *s=='(' || *s == '\\') {
	    sprintf(p,"\\%03o",*s);
	    p += 4;
	}
	else *p++ = *s;
    }
    *p = 0;
    return buf;
}

static
void ps_setup(
  int iw,
  int ih,
  enum orientation orientation,
  int scale,
  int left,
  int top,
  int flags,
  const char *header,
  const char *trailer,
  const char *name,
  int paper_width,
  int paper_length)
{
    char    hostname[256];
#ifdef WIN32
    char *username;
#else
    struct passwd  *pswd;
#endif
    time_t    clock;
    int lm, bm; /* left (bottom) margin */

    /* calculate margins */
    if (orientation==PORTRAIT) {
	lm = (left > 0)? left : ((PAPER_WIDTH - scale * iw) / 2);
	bm = (top > 0)? (PAPER_LENGTH - top - scale * ih)
		: ((PAPER_LENGTH - scale * ih) / 2);
    } else { /* orientation == LANDSCAPE */
	lm = (top > 0)? (PAPER_WIDTH - top - scale * ih)
		: ((PAPER_WIDTH - scale * ih) / 2);
	bm = (left > 0)? (PAPER_LENGTH - left - scale * iw)
		: ((PAPER_LENGTH - scale * iw) / 2);
    }
    printf ("%%!%s\n", COMMENTVERSION);
    printf ("%%%%BoundingBox: %d %d %d %d\n",
	    (flags & F_NPOSITION) ? points(lm) : 0,
	    (flags & F_NPOSITION) ? points(bm) : 0,
	    points(iw * scale), points(ih * scale));
    (void) XmuGetHostname (hostname, sizeof hostname);
#ifdef WIN32
    username = getenv("USERNAME");
    printf ("%%%%Creator: %s:%s\n", hostname,
	    username ? username : "unknown");
#else
    pswd = getpwuid (getuid ());
    printf ("%%%%Creator: %s:%s (%s)\n", hostname,
	    pswd->pw_name, pswd->pw_gecos);
#endif
    printf ("%%%%Title: %s (%s)\n", infilename,name);
    printf ("%%%%CreationDate: %s",
		(time (&clock), ctime (&clock)));
    printf ("%%%%EndComments\n");

    dump_prolog(flags);

    if (orientation==PORTRAIT) {
	if (header || trailer) {
	    printf("gsave\n");
	    printf("/Times-Roman findfont 15 scalefont setfont\n");
	    /* origin at bottom left corner of image */
	    printf("%d %d translate\n",points(lm),points(bm));
	    if (header) {
		char *label = escape(header);
		printf("%d (%s) stringwidth pop sub 2 div %d moveto\n",
		    points(iw*scale), label, points(ih*scale) + 10);
		printf("(%s) show\n",label);
	    }
	    if (trailer) {
		char *label = escape(trailer);
		printf("%d (%s) stringwidth pop sub 2 div -20 moveto\n",
		    points(iw*scale), label);
		printf("(%s) show\n",label);
	    }
	    printf("grestore\n");
	}
	/* set resolution to device units (300/inch) */
	printf("72 300 div dup scale\n");
	/* move to lower left corner of image */
	if (!(flags & F_NPOSITION))
	    printf("%d %d translate\n",lm,bm);
	/* dump the bitmap */
	printf("%d %d %d bitdump\n",iw,ih,scale);
    } else { /* orientation == LANDSCAPE */
	if (header || trailer) {
	    printf("gsave\n");
	    printf("/Times-Roman findfont 15 scalefont setfont\n");
	    /* origin at top left corner of image */
	    printf("%d %d translate\n",points(lm),points(bm + scale * iw));
	    /* rotate to print the titles */
	    printf("-90 rotate\n");
	    if (header) {
		char *label = escape(header);
		printf("%d (%s) stringwidth pop sub 2 div %d moveto\n",
		    points(iw*scale), label, points(ih*scale) + 10);
		printf("(%s) show\n",label);
	    }
	    if (trailer) {
		char *label = escape(trailer);
		printf("%d (%s) stringwidth pop sub 2 div -20 moveto\n",
		    points(iw*scale), label);
		printf("(%s) show\n",label);
	    }
	    printf("grestore\n");
	}
	/* set resolution to device units (300/inch) */
	printf("72 300 div dup scale\n");
	/* move to lower left corner of image */
	if (!(flags & F_NPOSITION))
	    printf("%d %d translate\n",lm,bm);
	/* dump the bitmap */
	printf("%d %d %d bitdump\n",ih,iw,scale);
    }
}

static const
char * const ps_epilog[] = {
	"",
	"showpage",
	"%%Trailer",
    NULL
};

static
void ps_finish(void)
{
	const char * const *p = ps_epilog;

	while (*p) printf("%s\n",*p++);
}

#define LINELEN 72 /* number of CHARS (bytes*2) per line of bitmap output */

static
void ps_output_bits(
  int iw,
  int ih,
  int flags,
  enum orientation orientation,
  XWDFileHeader *win,
  const char *data)
{
    unsigned long swaptest = 1;
    int iwb = win->bytes_per_line;
    register int i;
    int bytes;
    unsigned char *buffer = (unsigned char *)data;
    register int ocount=0;
    static char hex[] = "0123456789abcdef";

    if (orientation == LANDSCAPE) {
	/* read in and rotate the entire image */
	/* The Postscript language has a rotate operator, but using it
	 * seem to make printing (at least on the Apple Laserwriter
	 * take about 10 times as long (40 minutes for a 1024x864 full-screen
	 * dump)!  Therefore, we rotate the image here.
	 */
	int ocol = ih;
	int owidth = (ih+31)/32; /* width of rotated image, in bytes */
	int oheight = (iw+31)/32; /* height of rotated image, in scanlines */
	register char *p, *q;
	char *obuf;
	unsigned char *ibuf;
	owidth *= 4;
	oheight *= 32;

	/* Allocate buffer for the entire rotated image (output).
	 * Owidth and Oheight are rounded up to a multiple of 32 bits,
	 * to avoid special cases at the boundaries
	 */
	obuf = calloc(owidth, oheight);
	if (obuf==NULL) {
	    fprintf(stderr,"xpr: cannot allocate %d bytes\n",owidth*oheight);
	    exit(1);
	}

	ibuf = malloc((unsigned)(iwb + 3));
	for (i=0;i<ih;i++) {
	    memcpy(ibuf, buffer, iwb);
	    buffer += iwb;
	    if (!(*(char *) &swaptest))
		_swaplong((char *)ibuf,(long)iwb);
	    ps_bitrot(ibuf,iw,--ocol,owidth,obuf);
	}
	if (!(*(char *) &swaptest))
	    _swaplong(obuf,(long)(iw*owidth));
	q = &obuf[iw*owidth];
	bytes = (ih+7)/8;
	for (p=obuf;p<q;p+=owidth)
	    ocount = ps_putbuf((unsigned char *)p,bytes,ocount,flags&F_COMPACT);
    }
    else {
	for (i=0;i<ih;i++) {
	    ocount = ps_putbuf(buffer,(iw+7)/8,ocount,flags&F_COMPACT);
	    buffer += iwb;
	}
    }
    if (flags & F_COMPACT) {
	if (ocount) {
	    /* pad to an integral number of lines */
	    while (ocount++ < LINELEN)
		/* for debugging, pad with a "random" value */
		putchar(hex[ocount&15]);
	    putchar('\n');
	}
    }
}

static const
unsigned char _reverse_byte[0x100] = {
	0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
	0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
	0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
	0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
	0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
	0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
	0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
	0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
	0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
	0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
	0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
	0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
	0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
	0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
	0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
	0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
	0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
	0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
	0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
	0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
	0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
	0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
	0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
	0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
	0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
	0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
	0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
	0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
	0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
	0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
	0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
	0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};

void _invbits (
  register unsigned char *b,
  register long n)
{
	do {
		*b = ~*b;
		b++;
	    } while (--n > 0);

}

/* copied from lib/X/XPutImage.c */

void _swapbits (
  register unsigned char *b,
  register long n)
{
	do {
		*b = _reverse_byte[*b];
		b++;
	    } while (--n > 0);

}

void _swapshort (
  register char *bp,
  register long n)
{
	register char c;
	register char *ep = bp + n;
	do {
		c = *bp;
		*bp = *(bp + 1);
		bp++;
		*bp = c;
		bp++;
	}
	while (bp < ep);
}

void _swaplong (
     register char *bp,
     register long n)
{
	register char c;
	register char *ep = bp + n;
	register char *sp;
	do {
	  	sp = bp + 3;
		c = *sp;
		*sp = *bp;
		*bp++ = c;
		sp = bp + 1;
		c = *sp;
		*sp = *bp;
		*bp++ = c;
		bp += 2;
	}
	while (bp < ep);
}

/* Dump some bytes in hex, with bits in each byte reversed
 * Ocount is number of chacters that have been written to the current
 * output line.  It's new value is returned as the result of the function.
 * Ocount is ignored (and the return value is meaningless) if compact==0.
 */
static
int ps_putbuf(
  register unsigned char *s,	/* buffer to dump */
  register int n,		/* number of BITS to dump */
  register int ocount,		/* position on output line for next char */
  int compact)			/* if non-zero, do compaction (see below) */
{
    register int ffcount = 0;
    static char hex[] = "0123456789abcdef";
#define PUT(c) { putchar(c); if (++ocount>=LINELEN) \
	{ putchar('\n'); ocount=0; }}

    if (compact) {
	/* The following loop puts out the bits of the image in hex,
	 * compressing runs of white space (represented by one bits)
	 * according the the following simple algorithm:  A run of n
	 * 'ff' bytes (i.e., bytes with value 255--all ones), where
	 * 1<=n<=255, is represented by a single 'ff' byte followed by a
	 * byte containing n.
	 * On a typical dump of a full screen pretty much covered by
	 * black-on-white text windows, this compression decreased the
	 * size of the file from 223 Kbytes to 63 Kbytes.
	 * Of course, another factor of two could be saved by sending
	 * the bytes 'as is' rather than in hex, using some sort of
	 * escape convention to avoid problems with control characters.
	 * Another popular encoding is to pack three bytes into 4 'sixels'
	 * as in the LN03, etc, but I'm too lazy to write the necessary
	 * PostScript code to unpack fancier representations.
	 */
	while (n--) {
	    if (*s == 0xff) {
		if (++ffcount == 255) {
		    PUT('f'); PUT('f');
		    PUT('f'); PUT('f');
		    ffcount = 0;
		}
	    }
	    else {
		if (ffcount) {
		    PUT('f'); PUT('f');
		    PUT(hex[ffcount >> 4]);
		    PUT(hex[ffcount & 0xf]);
		    ffcount = 0;
		}
		PUT(hex1[*s]);
		PUT(hex2[*s]);
	    }
	    s++;
	}
	if (ffcount) {
	    PUT('f'); PUT('f');
		PUT(hex[ffcount >> 4]);
	    PUT(hex[ffcount & 0xf]);
	    ffcount = 0;
	}
    }
    else { /* no compaction: just dump the image in hex (bits reversed) */
	while (n--) {
	    putchar(hex1[*s]);
	    putchar(hex2[*s++]);
	}
	putchar('\n');
    }
    return ocount;
}

static
void ps_bitrot(
  unsigned char *s,
  register int n,
  int col,
  register int owidth,
  char *obuf)
/* s points to a chunk of memory and n is its width in bits.
 * The algorithm is, roughly,
 *    for (i=0;i<n;i++) {
 *        OR the ith bit of s into the ith row of the
 *        (col)th column of obuf
 *    }
 * Assume VAX bit and byte ordering for s:
 *	The ith bit of s is s[j]&(1<<k) where i=8*j+k.
 *	It can also be retrieved as t[j]&(1<<k), where t=(int*)s and i=32*j+k.
 * Also assume VAX bit and byte ordering for each row of obuf.
 * Ps_putbuf() takes care of converting to Motorola 68000 byte and bit
 * ordering.  The following code is very carefully tuned to yield a very
 * tight loop on the VAX, since it easily dominates the entire running
 * time of this program.  In particular, iwordp is declared last, since
 * there aren't enough registers, and iwordp is referenced only once
 * every 32 times through the loop.
 */
{
    register int mask = 1<<(col%32);
    register int iword; /* current input word (*iwordp) */
    register int b = 0; /* number of bits in iword left to examine */
    register char *opos = obuf + (col/32)*4;
	/* pointer to word of obuf to receive next output bit */
    register int *iwordp = (int *) s; /* pointer to next word of s */

    while (--n>=0) {
	if (--b < 0) {
	    iword = *iwordp++;
	    b = 31;
	}
	if (iword & 1) {
	    *(int *)opos |= mask;
	}
	opos += owidth;
	iword >>= 1;
    }
}

/* fullread() is the same as read(), except that it guarantees to
   read all the bytes requested. */

static
void fullread (
  int file,
  char *data,
  int nbytes)
{
    int bytes_read;
    while ((bytes_read = read(file, data, nbytes)) != nbytes) {
	if (bytes_read < 0) {
	    perror ("error while reading standard input");
	    return;
	    }
	else if (bytes_read == 0) {
	    fprintf (stderr, "xpr: premature end of file\n");
	    return;
	    }
	nbytes -= bytes_read;
	data += bytes_read;
	}
}
