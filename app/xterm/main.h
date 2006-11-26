/* $XTermId: main.h,v 1.31 2006/02/13 01:14:59 tom Exp $ */

/* $XFree86: xc/programs/xterm/main.h,v 3.15 2006/02/13 01:14:59 dickey Exp $ */

/*
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
#ifndef included_main_h
#define included_main_h

#include <xterm.h>

#define DEFCLASS		"XTerm"
#define DEFFONT			"fixed"
#define DEFWIDEFONT		NULL	/* grab one which is 2x as wide */
#define DEFWIDEBOLDFONT		NULL
#define DEFXIMFONT		"*"
#define DEFBOLDFONT		NULL	/* no bold font uses overstriking */
#define DEFBORDER		2
#define DEFFACENAME		NULL
#define DEFFACESIZE		"14.0"

#if OPT_BLINK_TEXT
#define DEFBLINKASBOLD		FALSE
#else
#define DEFBLINKASBOLD		TRUE
#endif

#ifndef DEFDELETE_DEL
#define DEFDELETE_DEL		2
#endif

#ifndef DEF_BACKARO_ERASE
#define DEF_BACKARO_ERASE	FALSE
#endif

#ifndef DEF_COLOR4
#define DEF_COLOR4		"blue2"		/* see XTerm-col.ad */
#endif

#ifndef DEF_COLOR12
#define DEF_COLOR12		"rgb:5c/5c/ff"	/* see XTerm-col.ad */
#endif

#ifndef DEF_INITIAL_ERASE
#define DEF_INITIAL_ERASE	FALSE
#endif

#ifndef PROJECTROOT
#define PROJECTROOT		"/usr/X11R6"
#endif

/*
 * The configure script quotes PROJECTROOT's value.
 * imake does not quote PROJECTROOT's value.
 */
#ifdef HAVE_CONFIG_H
#define DEFLOCALEFILTER2(x)	x
#else
#define DEFLOCALEFILTER2(x)	#x
#endif

/*
 * If the configure script finds luit, we have the path directly.
 */
#ifdef LUIT_PATH
#define DEFLOCALEFILTER		LUIT_PATH
#else
#define DEFLOCALEFILTER1(x)	DEFLOCALEFILTER2(x)
#define DEFLOCALEFILTER		DEFLOCALEFILTER1(PROJECTROOT) "/bin/luit"
#endif

#endif	/* included_main_h */
