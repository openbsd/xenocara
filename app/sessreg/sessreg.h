/*
 * Copyright 1990, 1998  The Open Group
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of The Open Group shall
 * not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization
 * from The Open Group.
 *
 */

/*
 * Copyright (c) 2005, Oracle and/or its affiliates. All rights reserved.
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

#include "config.h"

#include <sys/types.h>
#include <time.h>

/* Prefer POSIX standard utmpx interfaces if present, otherwise use utmp */
#ifdef HAVE_UTMP_H
# include <utmp.h>
# ifndef HAVE_UTMPX_H
#  define USE_UTMP
# endif
#endif

#ifdef HAVE_UTMPX_H
# include <utmpx.h>
# define USE_UTMPX
#endif

#ifdef HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

#if defined(HAVE_STRUCT_LASTLOG) && defined(HAVE_PWD_H)
# ifdef HAVE_LASTLOG_H
#  include <lastlog.h>
# endif
# include <pwd.h>
# define USE_LASTLOG
#endif

#ifndef WTMP_FILE
# ifdef _PATH_WTMP
#  define WTMP_FILE	_PATH_WTMP
# else
#  define WTMP_FILE	"/usr/adm/wtmp"
# endif
#endif
#ifndef UTMP_FILE
# ifdef _PATH_UTMP
#  define UTMP_FILE	_PATH_UTMP
# else
#  define UTMP_FILE	"/etc/utmp"
# endif
#endif
#ifndef LLOG_FILE
# ifdef _PATH_LASTLOG
#  define LLOG_FILE	_PATH_LASTLOG
# else
#  define LLOG_FILE	"/usr/adm/lastlog"
# endif
#endif
#ifndef TTYS_FILE
# define TTYS_FILE	"/etc/ttys"
#endif

#ifdef USE_UTMPX
#ifndef WTMPX_FILE
# define WTMPX_FILE	_PATH_WTMPX
#endif
#ifndef UTMPX_FILE
# define UTMPX_FILE	_PATH_UTMPX
#endif
#endif
