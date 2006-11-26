
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
AC_INIT(xvidtune,[1.0.1], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],xvidtune)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_CPP
AC_PATH_PROGS(RAWCPP, [cpp], [$(CPP)], 
   [$PATH:/bin:/usr/bin:/usr/lib:/usr/libexec:/usr/ccs/lib:/usr/ccs/lbin:/lib])

m4_ifdef([AS_HELP_STRING], , [m4_define([AS_HELP_STRING], m4_defn([AC_HELP_STRING]))])

# Checks for pkg-config packages
XAW_CHECK_XPRINT_SUPPORT(XAW)
PKG_CHECK_MODULES(XVIDTUNE, xxf86vm)

XVIDTUNE_CFLAGS="$XVIDTUNE_CFLAGS $XAW_CFLAGS"
XVIDTUNE_LIBS="$XVIDTUNE_LIBS $XAW_LIBS"

echo $XVIDTUNE_LIBS

AC_SUBST(XVIDTUNE_CFLAGS)
AC_SUBST(XVIDTUNE_LIBS)

AC_ARG_WITH(vendor-name,
	    AS_HELP_STRING([--with-vendor-name=VENDOR],
			   [Vendor name for disclaimer statement]),
	    [ VENDOR_NAME="$withval" ],
	    [ VENDOR_NAME="The X.Org Foundation" ])
AC_SUBST([VENDOR_NAME])

PKG_CHECK_MODULES(APPDEFS, xt)
appdefaultdir=$(pkg-config --variable=appdefaultdir xt)
AC_SUBST(appdefaultdir)

XORG_MANPAGE_SECTIONS
XORG_RELEASE_VERSION

AC_OUTPUT([Makefile])
