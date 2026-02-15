/*
 * Copyright (C) 1989-95 GROUPE BULL
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * GROUPE BULL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of GROUPE BULL shall not be
 * used in advertising or otherwise to promote the sale, use or other dealings
 * in this Software without prior written authorization from GROUPE BULL.
 */

/*****************************************************************************\
* XpmI.h:                                                                     *
*                                                                             *
*  XPM library                                                                *
*  Internal Include file                                                      *
*                                                                             *
*  ** Everything defined here is subject to changes any time. **              *
*                                                                             *
*  Developed by Arnaud Le Hors                                                *
\*****************************************************************************/

#ifndef XPMI_h
#define XPMI_h

#include "xpm.h"

/*
 * lets try to solve include files
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#if !defined(_MSC_VER)
#include <strings.h>
#endif

#include <X11/Xos.h>
#include <X11/Xfuncs.h>
#include <X11/Xmd.h>

#ifdef VMS
#include <unixio.h>
#include <file.h>
#endif

/* The following should help people wanting to use their own memory allocation
 * functions. To avoid the overhead of a function call when the standard
 * functions are used these are all macros, even the XpmFree function which
 * needs to be a real function for the outside world though.
 * So if change these be sure to change the XpmFree function in misc.c
 * accordingly.
 */
#define XpmFree(ptr) free(ptr)

#define XpmMalloc(size) malloc((size))
#define XpmRealloc(ptr, size) realloc((ptr), (size))
#define XpmCalloc(nelem, elsize) calloc((nelem), (elsize))

#include <stdint.h>	/* For SIZE_MAX */
#include <limits.h>
#ifndef SIZE_MAX
# ifdef ULONG_MAX
#  define SIZE_MAX ULONG_MAX
# else
#  define SIZE_MAX UINT_MAX
# endif
#endif

#ifdef O_CLOEXEC
# define FOPEN_CLOEXEC "e"
#else
# define FOPEN_CLOEXEC ""
# define O_CLOEXEC 0
#endif

#define XPMMAXCMTLEN BUFSIZ
typedef struct {
    unsigned int type;
    union {
	FILE *file;
	char **data;
    }     stream;
    char *cptr;
    unsigned int line;
    int CommentLength;
    char Comment[XPMMAXCMTLEN];
    const char *Bcmt, *Ecmt;
    char Bos, Eos;
    int format;			/* 1 if XPM1, 0 otherwise */
#ifdef CXPMPROG
    int lineNum;
    int charNum;
#endif
}      xpmData;

#define XPMARRAY 0
#define XPMFILE  1
#define XPMPIPE  2
#define XPMBUFFER 3

#define EOL '\n'
#define TAB '\t'
#define SPC ' '

typedef struct {
    const char *type;		/* key word */
    const char *Bcmt;		/* string beginning comments */
    const char *Ecmt;		/* string ending comments */
    char Bos;			/* character beginning strings */
    char Eos;			/* character ending strings */
    const char *Strs;		/* strings separator */
    const char *Dec;		/* data declaration string */
    const char *Boa;		/* string beginning assignment */
    const char *Eoa;		/* string ending assignment */
}      xpmDataType;

extern _X_HIDDEN xpmDataType xpmDataTypes[];

/*
 * rgb values and ascii names (from rgb text file) rgb values,
 * range of 0 -> 65535 color mnemonic of rgb value
 */
typedef struct {
    int r, g, b;
    char *name;
}      xpmRgbName;

/* Maximum number of rgb mnemonics allowed in rgb text file. */
#define MAX_RGBNAMES 1024

extern _X_HIDDEN const char *xpmColorKeys[];

#define TRANSPARENT_COLOR "None"	/* this must be a string! */

/* number of xpmColorKeys */
#define NKEYS 5

/* XPM internal routines */

HFUNC(xpmParseData, int, (xpmData *data, XpmImage *image, XpmInfo *info));
HFUNC(xpmParseDataAndCreate, int, (Display *display, xpmData *data,
				  XImage **image_return,
				  XImage **shapeimage_return,
				  XpmImage *image, XpmInfo *info,
				  XpmAttributes *attributes));

HFUNC(xpmFreeColorTable, void, (XpmColor *colorTable, int ncolors));

HFUNC(xpmInitAttributes, void, (XpmAttributes *attributes));

HFUNC(xpmInitXpmImage, void, (XpmImage *image));

HFUNC(xpmInitXpmInfo, void, (XpmInfo *info));

HFUNC(xpmSetInfoMask, void, (XpmInfo *info, XpmAttributes *attributes));
HFUNC(xpmSetInfo, void, (XpmInfo *info, XpmAttributes *attributes));
HFUNC(xpmSetAttributes, void, (XpmAttributes *attributes, XpmImage *image,
			      XpmInfo *info));

HFUNC(xpmCreatePixmapFromImage, int, (Display *display, Drawable d,
				      XImage *ximage, Pixmap *pixmap_return));

HFUNC(xpmCreateImageFromPixmap, void, (Display *display, Pixmap pixmap,
				      XImage **ximage_return,
				      unsigned int *width,
				      unsigned int *height));

/* structures and functions related to hastable code */

typedef struct _xpmHashAtom {
    char *name;
    void *data;
}      *xpmHashAtom;

typedef struct {
    unsigned int size;
    unsigned int limit;
    unsigned int used;
    xpmHashAtom *atomTable;
}      xpmHashTable;

HFUNC(xpmHashTableInit, int, (xpmHashTable *table));
HFUNC(xpmHashTableFree, void, (xpmHashTable *table));
HFUNC(xpmHashSlot, xpmHashAtom *, (xpmHashTable *table, char *s));
HFUNC(xpmHashIntern, int, (xpmHashTable *table, char *tag, void *data));

#if defined(_MSC_VER) && defined(_M_X64)
#define HashAtomData(i) ((void *)(long long)i)
#define HashColorIndex(slot) ((unsigned long long)((*slot)->data))
#else
#define HashAtomData(i) ((void *)(long)i)
#define HashColorIndex(slot) ((unsigned long)((*slot)->data))
#endif
#define USE_HASHTABLE (cpp > 2 && ncolors > 4)

/* I/O utility */

HFUNC(xpmNextString, int, (xpmData *mdata));
HFUNC(xpmNextUI, int, (xpmData *mdata, unsigned int *ui_return));
HFUNC(xpmGetString, int, (xpmData *mdata, char **sptr, unsigned int *l));

#define xpmGetC(mdata) \
	((!mdata->type || mdata->type == XPMBUFFER) ? \
	 (*mdata->cptr++) : (getc(mdata->stream.file)))

HFUNC(xpmNextWord, unsigned int,
     (xpmData *mdata, char *buf, unsigned int buflen));
HFUNC(xpmGetCmt, int, (xpmData *mdata, char **cmt));
HFUNC(xpmParseHeader, int, (xpmData *mdata));
HFUNC(xpmParseValues, int, (xpmData *data, unsigned int *width,
			   unsigned int *height, unsigned int *ncolors,
			   unsigned int *cpp, unsigned int *x_hotspot,
			   unsigned int *y_hotspot, unsigned int *hotspot,
			   unsigned int *extensions));

HFUNC(xpmParseColors, int, (xpmData *data, unsigned int ncolors,
			   unsigned int cpp, XpmColor **colorTablePtr,
			   xpmHashTable *hashtable));

HFUNC(xpmParseExtensions, int, (xpmData *data, XpmExtension **extensions,
			       unsigned int *nextensions));

/* RGB utility */

HFUNC(xpmReadRgbNames, int, (const char *rgb_fname, xpmRgbName *rgbn));
HFUNC(xpmGetRgbName, char *, (xpmRgbName *rgbn, int rgbn_max,
			     int red, int green, int blue));
HFUNC(xpmFreeRgbNames, void, (xpmRgbName *rgbn, int rgbn_max));

HFUNC(xpm_xynormalizeimagebits, void, (register unsigned char *bp,
				      register XImage *img));
HFUNC(xpm_znormalizeimagebits, void, (register unsigned char *bp,
				     register XImage *img));

/*
 * Macros
 *
 * The XYNORMALIZE macro determines whether XY format data requires
 * normalization and calls a routine to do so if needed. The logic in
 * this module is designed for LSBFirst byte and bit order, so
 * normalization is done as required to present the data in this order.
 *
 * The ZNORMALIZE macro performs byte and nibble order normalization if
 * required for Z format data.
 *
 * The XYINDEX macro computes the index to the starting byte (char) boundary
 * for a bitmap_unit containing a pixel with coordinates x and y for image
 * data in XY format.
 *
 * The ZINDEX* macros compute the index to the starting byte (char) boundary
 * for a pixel with coordinates x and y for image data in ZPixmap format.
 *
 */

#define XYNORMALIZE(bp, img) \
    if ((img->byte_order == MSBFirst) || (img->bitmap_bit_order == MSBFirst)) \
	xpm_xynormalizeimagebits((unsigned char *)(bp), img)

#define ZNORMALIZE(bp, img) \
    if (img->byte_order == MSBFirst) \
	xpm_znormalizeimagebits((unsigned char *)(bp), img)

#define XYINDEX(x, y, img) \
    ((y) * img->bytes_per_line) + \
    (((x) + img->xoffset) / img->bitmap_unit) * (img->bitmap_unit >> 3)

#define ZINDEX(x, y, img) ((y) * img->bytes_per_line) + \
    (((x) * img->bits_per_pixel) >> 3)

#define ZINDEX32(x, y, img) ((y) * img->bytes_per_line) + ((x) << 2)

#define ZINDEX16(x, y, img) ((y) * img->bytes_per_line) + ((x) << 1)

#define ZINDEX8(x, y, img) ((y) * img->bytes_per_line) + (x)

#define ZINDEX1(x, y, img) ((y) * img->bytes_per_line) + ((x) >> 3)

HFUNC(xpmatoui, unsigned int,
     (char *p, unsigned int l, unsigned int *ui_return));

#endif
