
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

# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([xeyes], [1.2.0],
        [https://gitlab.freedesktop.org/xorg/app/xeyes/issues], [xeyes])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Checks for pkg-config packages
PKG_CHECK_MODULES(XEYES, [xi >= 1.7 x11 xt xext xmu xproto >= 7.0.17])

dnl Optional dependencies
AC_ARG_WITH(xrender, AS_HELP_STRING([--with-xrender],[Use Xrender for rendering (Default is YES)]),use_xrender="$withval",use_xrender="try")
if test x$use_xrender != xno ; then
	PKG_CHECK_MODULES(XRENDER, [xrender >= 0.4])
	AC_DEFINE([XRENDER],1,[Define to use X Render Extension])
fi

AC_ARG_WITH(present, AS_HELP_STRING([--with-present],[Use Present for updates (Default is AUTO)]),use_present="$withval",use_present="try")
if test x$use_present != xno ; then
	PKG_CHECK_MODULES(PRESENT, [x11-xcb xcb-present >= 1.9 xcb-xfixes xcb-damage])
	AC_DEFINE([PRESENT],1,[Define to use X Present Extension])
fi

AC_CONFIG_FILES([
	Makefile
	man/Makefile])
AC_OUTPUT
