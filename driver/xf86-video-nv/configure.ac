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
AC_INIT([xf86-video-nv],
        2.1.17,
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        xf86-video-nv)

AC_CONFIG_SRCDIR([Makefile.am])
AM_CONFIG_HEADER([config.h])

# Require xorg-macros: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION], 
          [m4_fatal([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)
XORG_DEFAULT_OPTIONS

AC_CONFIG_AUX_DIR(.)

AM_INIT_AUTOMAKE([foreign dist-bzip2])

AM_MAINTAINER_MODE

# Checks for programs.
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_PROG_CC

AH_TOP([#include "xorg-server.h"])

# Options
AC_ARG_WITH(xorg-module-dir,
            AC_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

AC_ARG_WITH(xserver-source,AC_HELP_STRING([--with-xserver-source=XSERVER_SOURCE],
                                          [Path to X server source tree]),
                           [ XSERVER_SOURCE="$withval" ],
                           [ XSERVER_SOURCE="" ])

# Checks for extensions
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XV, videoproto)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.2 xproto fontsproto $REQUIRED_MODULES])
PKG_CHECK_MODULES(XEXT, [xextproto >= 7.0.99.1],
                  HAVE_XEXTPROTO_71="yes"; AC_DEFINE(HAVE_XEXTPROTO_71, 1, [xextproto 7.1 available]),
                  HAVE_XEXTPROTO_71="no")
AM_CONDITIONAL(HAVE_XEXTPROTO_71, [ test "$HAVE_XEXTPROTO_71" = "yes" ])

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC

save_CFLAGS="$CFLAGS"
CFLAGS="$XORG_CFLAGS"

# RandR 1.2
AC_CHECK_HEADER(xf86Modes.h,[BUILD_XMODES=no],[BUILD_XMODES=yes],[#include "xorg-server.h"])

# PCI rework
AC_CHECK_DECL(XSERVER_LIBPCIACCESS,
              [XSERVER_LIBPCIACCESS=yes],[XSERVER_LIBPCIACCESS=no],
              [#include "xorg-server.h"])
if test "x$XSERVER_LIBPCIACCESS" = "xyes"; then
    PKG_CHECK_MODULES(PCIACCESS, [pciaccess >= 0.8.0])
fi

CFLAGS="$save_CFLAGS"

AM_CONDITIONAL(BUILD_XMODES, test "x$BUILD_XMODES" = xyes)
if test "x$XSERVER_SOURCE" != x; then
    if test -d "$XSERVER_SOURCE"; then
        XSERVER_SOURCE=`cd "$XSERVER_SOURCE" && pwd`
        if test -f "$XSERVER_SOURCE/hw/xfree86/modes/xf86Modes.h" -a -f "$XSERVER_SOURCE/hw/xfree86/parser/xf86Parser.h"; then
            :
        else
            AC_ERROR([Server source at $XSERVER_SOURCE doesn't have the new mode code])
        fi
    else
        AC_ERROR([Can't find xserver-source path $XSERVER_SOURCE])
    fi
fi
if test "x$BUILD_XMODES" = xno; then
    AC_MSG_NOTICE([X server has new mode code])
    XMODES_CFLAGS=
    parser_dir=
    modes_dir=
else
    if test "x$XSERVER_SOURCE" = x; then
            AC_MSG_NOTICE([X server is missing new mode code and --with-xserver-source not specified, using local copy])
            AC_MSG_NOTICE([If you're building from git, please read compat/README])
            parser_dir='$(top_srcdir)/compat/parser'
            modes_dir='$(top_srcdir)/compat/modes'
    else
            AC_MSG_NOTICE([Using mode code from $XSERVER_SOURCE])
            parser_dir="$XSERVER_SOURCE/hw/xfree86/parser"
            modes_dir="$XSERVER_SOURCE/hw/xfree86/modes"
    fi
    XMODES_CFLAGS="-DXF86_MODES_RENAME -I\"$modes_dir\" -I\"$parser_dir\""
    AC_DEFINE(BUILD_XMODES, 1,[X server doesn't have built-in mode code, so we need to build it])
fi
AC_SUBST([XMODES_CFLAGS])
AC_SUBST([parser_dir])
AC_SUBST([modes_dir])

# Substitutions
AC_SUBST([XORG_CFLAGS])
AC_SUBST([moduledir])

DRIVER_NAME=nv
AC_SUBST([DRIVER_NAME])

AC_OUTPUT([
	Makefile
	src/Makefile
	man/Makefile
	compat/Makefile
])
