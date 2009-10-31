dnl 
dnl  $Id: configure.ac,v 1.2 2009/10/31 17:40:00 matthieu Exp $
dnl 
dnl  Copyright © 2003 Keith Packard, Noah Levitt
dnl 
dnl  Permission to use, copy, modify, distribute, and sell this software and its
dnl  documentation for any purpose is hereby granted without fee, provided that
dnl  the above copyright notice appear in all copies and that both that
dnl  copyright notice and this permission notice appear in supporting
dnl  documentation, and that the name of Keith Packard not be used in
dnl  advertising or publicity pertaining to distribution of the software without
dnl  specific, written prior permission.  Keith Packard makes no
dnl  representations about the suitability of this software for any purpose.  It
dnl  is provided "as is" without express or implied warranty.
dnl 
dnl  KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
dnl  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
dnl  EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
dnl  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
dnl  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
dnl  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
dnl  PERFORMANCE OF THIS SOFTWARE.
dnl
dnl Process this file with autoconf to create configure.

AC_PREREQ([2.57])

dnl
dnl Version should match the current XRes version. XResQueryVersion
dnl returns the version from XResproto.h, NOT the version we set here. But we
dnl try to keep these the same.  Note that the library has an extra
dnl digit in the version number to track changes which don't affect the
dnl protocol, so XRes version l.n.m corresponds to protocol version l.n
dnl
AC_INIT(libXres, 1.0.4, [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg], libXres)
AC_CONFIG_AUX_DIR(.)
AM_INIT_AUTOMAKE([dist-bzip2])

# Require xorg-macros 1.3 or later: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION],
	  [m4_fatal([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)

AM_CONFIG_HEADER(config.h)

# Check for progs
AC_PROG_CC
AC_PROG_LIBTOOL

XORG_DEFAULT_OPTIONS

# get modules

# Check xres configuration, strip extra digits from package version to
# find the required protocol version
RES_VERSION=[`echo $VERSION | sed 's/^\([0-9][0-9]*\.[0-9][0-9]*\).*$/\1/'`]
PKG_CHECK_MODULES(XRES, x11 xext xextproto [resourceproto >= $RES_VERSION])
XRES_CFLAGS="$CWARNFLAGS $XRES_CFLAGS"
AC_SUBST(XRES_CFLAGS)
AC_SUBST(XRES_LIBS)

XORG_CHECK_MALLOC_ZERO

AC_OUTPUT([Makefile
	   src/Makefile
	   man/Makefile
           xres.pc])

