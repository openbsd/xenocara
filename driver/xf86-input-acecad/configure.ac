#  Copyright 2005 Adam Jackson.
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  on the rights to use, copy, modify, merge, publish, distribute, sub
#  license, and/or sell copies of the Software, and to permit persons to whom
#  the Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice (including the next
#  paragraph) shall be included in all copies or substantial portions of the
#  Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
#  ADAM JACKSON BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
#  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
#  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# Process this file with autoconf to produce a configure script

AC_PREREQ(2.57)
AC_INIT([xf86-input-acecad],
        1.4.0,
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        xf86-input-acecad)

AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_AUX_DIR(.)
AM_INIT_AUTOMAKE([dist-bzip2])

AM_MAINTAINER_MODE

# Require xorg-macros: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION], [AC_FATAL([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)
AM_CONFIG_HEADER([config.h])

# Checks for programs.
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_PROG_CC
XORG_DEFAULT_OPTIONS

AH_TOP([#include "xorg-server.h"])

#AC_DEFINE(XFree86LOADER,1,[Stub define for loadable drivers])
#
#AC_ARG_ENABLE(XINPUT, AS_HELP_STRING([--enable-xinput],
#              [Build XInput support (default: yes)]),
#              [XINPUT=$enableval],[XINPUT=yes])
#AM_CONDITIONAL(XINPUT, test "x$XINPUT" = "xyes")
#if test "x$XINPUT" = "xyes" ; then
#    AC_DEFINE(XINPUT,1,[Enable XInput support])
#fi
#
#AC_ARG_ENABLE(XKB, AS_HELP_STRING([--enable-xkb],
#              [Build XKB support (default: yes)]),
#              [XKB=$enableval],[XKB=yes])
#AM_CONDITIONAL(XKB, test "x$XKB" = "xyes")
#if test "x$XKB" = "xyes" ; then
#    AC_DEFINE(XKB,1,[Enable XKB support])
#fi

AC_ARG_WITH(xorg-module-dir,
            AC_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])
inputdir=${moduledir}/input
AC_SUBST(inputdir)

# Checks for extensions
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(XINPUT, inputproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.0.99.901] xproto $REQUIRED_MODULES)
sdkdir=$(pkg-config --variable=sdkdir xorg-server)
XORG_CFLAGS="$CWARNFLAGS $XORG_CFLAGS"

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_CHECK([linux/input.h],
		[AC_DEFINE([LINUX_INPUT], [1], [Using the Linux input subsystem])]
		)
AC_CHECK_HEADER(sysfs/libsysfs.h, HAVE_LIBSYSFS=yes, HAVE_LIBSYSFS=no)
if test x$HAVE_LIBSYSFS = xyes; then
    AC_DEFINE(LINUX_SYSFS, 1, [Using sysfs to probe devices])
fi
AM_CONDITIONAL(HAVE_LIBSYSFS, test x$HAVE_LIBSYSFS = xyes)

DRIVER_NAME=acecad
AC_SUBST([DRIVER_NAME])

AC_OUTPUT([Makefile src/Makefile man/Makefile])
