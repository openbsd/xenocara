dnl  Copyright 2005 Red Hat, Inc.
dnl 
dnl  Permission to use, copy, modify, distribute, and sell this software and its
dnl  documentation for any purpose is hereby granted without fee, provided that
dnl  the above copyright notice appear in all copies and that both that
dnl  copyright notice and this permission notice appear in supporting
dnl  documentation, and that the name of Red Hat not be used in
dnl  advertising or publicity pertaining to distribution of the software without
dnl  specific, written prior permission.  Red Hat makes no
dnl  representations about the suitability of this software for any purpose.  It
dnl  is provided "as is" without express or implied warranty.
dnl 
dnl  RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
dnl  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
dnl  EVENT SHALL RED HAT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
dnl  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
dnl  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
dnl  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
dnl  PERFORMANCE OF THIS SOFTWARE.
dnl
dnl Process this file with autoconf to create configure.

AC_PREREQ([2.57])
AC_INIT(xlogo, [1.0.2], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],xlogo)
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Require xorg-macros: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)
XORG_DEFAULT_OPTIONS


AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_PROG_INSTALL

# Checks for pkg-config packages
PKG_CHECK_MODULES(XLOGO, xaw7 xt)

# Use render & xft2 to draw logo?

AC_ARG_WITH(render, 
	AC_HELP_STRING([--with-xrender], [Use Xrender & Xft2 to draw logo]), 
	USE_RENDER="$withval", USE_RENDER="try")

if test "x$USE_RENDER" = "xtry" ; then
	PKG_CHECK_EXISTS([xrender xft], USE_RENDER="yes", USE_RENDER="no")
fi

if test "x$USE_RENDER" = "xyes" ; then
	PKG_CHECK_MODULES(XRENDER, xrender)
	PKG_CHECK_MODULES(XFT, xft)

	AC_DEFINE(XRENDER, 1, [Define to 1 to use Xrender & Xft2 to draw logo])

	XLOGO_CFLAGS="$XLOGO_CFLAGS $XRENDER_CFLAGS $XFT_CFLAGS"
	XLOGO_LIBS="$XLOGO_LIBS $XRENDER_LIBS $XFT_LIBS"
fi

PKG_CHECK_MODULES(XEXT, xext)
XLOGO_CFLAGS="$XLOGO_CFLAGS $XEXT_CFLAGS"
XLOGO_LIBS="$XLOGO_LIBS $XEXT_LIBS"

XLOGO_CFLAGS="$CWARNFLAGS $XLOGO_CFLAGS"
AC_SUBST(XLOGO_CFLAGS)
AC_SUBST(XLOGO_LIBS)

PKG_CHECK_MODULES(APPDEFS, xt)
xt_appdefaultdir=$(pkg-config --variable=appdefaultdir xt)
AC_ARG_WITH(appdefaultdir,
	AC_HELP_STRING([--with-appdefaultdir=<pathname>],
	  [specify directory for app-defaults files (default is autodetected)]),
	[appdefaultdir="$withval"], [appdefaultdir="${xt_appdefaultdir}"])
AC_SUBST(appdefaultdir)

AC_OUTPUT([Makefile])
