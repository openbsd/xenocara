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

AC_PREREQ(2.59)
AC_INIT([xf86-video-geode],
        2.11.8,
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg&component=Driver%2Fgeode],
        xf86-video-geode,
	[http://www.x.org/wiki/GeodeDriver])

AC_CONFIG_SRCDIR([Makefile.am])
AM_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR(.)

# Require xorg-macros: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION], 
          [m4_fatal([must install xorg-macros 1.4 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.4)
XORG_DEFAULT_OPTIONS

AM_INIT_AUTOMAKE([foreign dist-bzip2])

AM_MAINTAINER_MODE

# Checks for programs.
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_PROG_CC
AM_PROG_AS

AH_TOP([#include "xorg-server.h"])

AC_ARG_WITH(xorg-module-dir,
            AC_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

AC_ARG_ENABLE(geodegx-panel, 
	      AC_HELP_STRING([--disable-geodegx-panel],
	      	             [Disable support for flatpanels with the Geode GX]),
	      [ ], 
	      [ AMD_CFLAGS="-DPNL_SUP $AMD_CFLAGS" ])

AC_ARG_ENABLE(werror,        AC_HELP_STRING([--enable-werror],
				  [Treat warnings as errors (default: disabled)]),
			        [WERROR=$enableval], [WERROR=no])
AC_ARG_ENABLE(visibility,
	      AC_HELP_STRING([--enable-visibility],
			     [Enable GCC visibility optimizations]),
			     [ AMD_CFLAGS="$AMD_CFLAGS -fvisibility=hidden" ],
			     [ : ])

if test "x$GCC" = "xyes"; then
       GCC_WARNINGS1="-Wall -Wpointer-arith -Wstrict-prototypes"
       GCC_WARNINGS2="-Wmissing-prototypes -Wmissing-declarations"
       GCC_WARNINGS3="-Wnested-externs -fno-strict-aliasing"
       GCC_WARNINGS="$GCC_WARNINGS1 $GCC_WARNINGS2 $GCC_WARNINGS3"
       if test "x$WERROR" = "xyes"; then
               GCC_WARNINGS="${GCC_WARNINGS} -Werror"
       fi
       AMD_CFLAGS="$GCC_WARNINGS $AMD_CFLAGS"
fi

# Checks for extensions
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XV, videoproto)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, [xorg-server xproto fontsproto $REQUIRED_MODULES])
PKG_CHECK_MODULES(XEXT, [xextproto >= 7.0.99.1],
                  HAVE_XEXTPROTO_71="yes"; AC_DEFINE(HAVE_XEXTPROTO_71, 1, [xextproto 7.1 available]),
                  HAVE_XEXTPROTO_71="no")
AM_CONDITIONAL(HAVE_XEXTPROTO_71, [ test "$HAVE_XEXTPROTO_71" = "yes" ])

# Checks for libraries.
SAVE_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $XORG_CFLAGS"
AC_CHECK_DECL(XSERVER_LIBPCIACCESS,
              [XSERVER_LIBPCIACCESS=yes],[XSERVER_LIBPCIACCESS=no],
              [#include "xorg-server.h"])
CPPFLAGS="$SAVE_CPPFLAGS"

# Checks for header files.
AC_HEADER_STDC

if test "x$XSERVER_LIBPCIACCESS" = xyes; then
    PKG_CHECK_MODULES([PCIACCESS], [pciaccess >= 0.8.0])
    XORG_CFLAGS="$XORG_CFLAGS $PCIACCESS_CFLAGS"
fi

AC_SUBST([XORG_CFLAGS])
AC_SUBST([moduledir])
AC_SUBST([AMD_CFLAGS])

is386=false

case $host_cpu in
  i*86)
	is386=true
	;;
esac

# Unset is386 if compiling for AMD64/EM64T
if test $is386 = true ; then
	AC_CHECK_DECLS([_LP64, __amd64__, amd64], [is386=false])
fi

AM_CONDITIONAL([I386ARCH], [test $is386 = true])

DRIVER_NAME=geode
AC_SUBST([DRIVER_NAME])

AC_OUTPUT([
	Makefile
	src/Makefile
])
