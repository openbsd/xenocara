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
AC_INIT([xf86-video-apm],
        1.2.2,
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        xf86-video-apm)

AC_CONFIG_SRCDIR([Makefile.am])
AM_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR(.)

AM_INIT_AUTOMAKE([dist-bzip2])

AM_MAINTAINER_MODE

# Checks for programs.
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_PROG_CC

AH_TOP([#include "xorg-server.h"])

AC_ARG_WITH(xorg-module-dir, [  --with-xorg-module-dir=DIR ],
                             [ moduledir="$withval" ],
                             [ moduledir="$libdir/xorg/modules" ])
AC_SUBST(moduledir)


# Checks for extensions
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XV, videoproto)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.0.99.901] xproto fontsproto $REQUIRED_MODULES)
PKG_CHECK_MODULES(XEXT, [xextproto >= 7.0.99.1],
                  HAVE_XEXTPROTO_71="yes"; AC_DEFINE(HAVE_XEXTPROTO_71, 1, [xextproto 7.1 available]),
                  HAVE_XEXTPROTO_71="no")
AM_CONDITIONAL(HAVE_XEXTPROTO_71, [ test "$HAVE_XEXTPROTO_71" = "yes" ])
sdkdir=$(pkg-config --variable=sdkdir xorg-server)

PKG_CHECK_MODULES(XF86RUSH, xxf86rush, have_xf86rush=yes, have_xf86rush=no)
AM_CONDITIONAL(XF86RUSH, test "x$have_xf86rush" = xyes)
AC_SUBST([XF86RUSH_LIBS])
AC_SUBST([XF86RUSH_CFLAGS])

CFLAGS="$CFLAGS $XORG_CFLAGS $XF86RUSH_CFLAGS"' -I$(top_srcdir)/src'
INCLUDES="$XORG_INCS -I${sdkdir} $XF86RUSH_INCS "'-I$(top_srcdir)/src -I$(prefix)/include'
AC_SUBST([CFLAGS])
AC_SUBST([INCLUDES])

save_CFLAGS="$CFLAGS"
CFLAGS="$XORG_CFLAGS"
AC_CHECK_DECL(xf86ConfigIsaEntity,
	      [AC_DEFINE(HAVE_ISA, 1, [Have ISA support])],
	      [],
	      [#include "xf86.h"])
CFLAGS="$save_CFLAGS"

# Checks for libraries.
SAVE_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $XORG_CFLAGS"
AC_CHECK_DECL(XSERVER_LIBPCIACCESS,
              [XSERVER_LIBPCIACCESS=yes],[XSERVER_LIBPCIACCESS=no],
              [#include "xorg-server.h"])
AC_CHECK_HEADER(xf1bpp.h,[AC_DEFINE(HAVE_XF1BPP, 1, [Have 1bpp support])],[])
AC_CHECK_HEADER(xf4bpp.h,[AC_DEFINE(HAVE_XF4BPP, 1, [Have 4bpp support])],[])
CPPFLAGS="$SAVE_CPPFLAGS"

# Checks for header files.
AC_HEADER_STDC

if test "x$XSERVER_LIBPCIACCESS" = xyes; then
    PKG_CHECK_MODULES([PCIACCESS], [pciaccess >= 0.8.0])
    XORG_CFLAGS="$XORG_CFLAGS $PCIACCESS_CFLAGS"
fi

DRIVER_NAME=apm
AC_SUBST([DRIVER_NAME])

XORG_MANPAGE_SECTIONS
XORG_RELEASE_VERSION

XORG_CHECK_LINUXDOC

AC_OUTPUT([
	Makefile
	src/Makefile
	man/Makefile
])
