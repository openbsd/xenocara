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
AC_INIT(x11perf,[1.5.1], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],x11perf)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

# Require xorg-macros 1.3 or later: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION],
	  [m4_fatal([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)

AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_PROG_INSTALL

XORG_DEFAULT_OPTIONS

AC_PATH_PROG(SED,sed)
if [[ -z $SED ]] ; then
   AC_MSG_ERROR([sed is needed to build x11perf])
fi

AC_TYPE_SIGNAL

# Checks for pkg-config packages
PKG_CHECK_MODULES(X11PERF, x11 xmuu)
AC_SUBST(X11PERF_CFLAGS)
AC_SUBST(X11PERF_LIBS)

# Check for Xrender library
PKG_CHECK_MODULES(XRENDER, xrender, [xrender_found=yes], [xrender_found=no])
AC_SUBST(XRENDER_CFLAGS)
AC_SUBST(XRENDER_LIBS)
case "$xrender_found" in
yes)
	AC_DEFINE(XRENDER,1,[Xrender library available])
	;;
esac

# Check for Xft library
PKG_CHECK_MODULES(XFT, xft, [xft_found=yes], [xft_found=no])
AC_SUBST(XFT_CFLAGS)
AC_SUBST(XFT_LIBS)
case "$xft_found" in
yes)
	AC_DEFINE(XFT,1,[Xft library available])
	;;
esac

# Check for MIT-SHM extension
PKG_CHECK_MODULES(XEXT, xext, [xext_found=yes], [xext_found=no])
AC_SUBST(XEXT_CFLAGS)
AC_SUBST(XEXT_LIBS)
case "$xext_found" in
yes)
	save_LIBS="$LIBS"
	LIBS="$XEXT_LIBS $LIBS"
	AC_CHECK_FUNCS([XShmQueryExtension], 
		       AC_DEFINE(MITSHM, 1, [MIT-SHM extension available]))
	LIBS="$save_LIBS"
	;;
esac

AC_OUTPUT([Makefile])
