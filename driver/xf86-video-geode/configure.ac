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

# Initialize Autoconf
AC_PREREQ(2.60)
AC_INIT([xf86-video-geode],
        [2.11.17],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg&component=Driver%2Fgeode],
        [xf86-video-geode],
        [http://www.x.org/wiki/GeodeDriver])

AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# Require xorg-macros: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION], 
          [m4_fatal([must install xorg-macros 1.4 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.4)
XORG_DEFAULT_OPTIONS

# Checks for programs.
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AH_TOP([#include "xorg-server.h"])

# -----------------------------------------------------------------------------
#			Configuration options
# -----------------------------------------------------------------------------
# Define a configure option for an alternate input module directory
AC_ARG_WITH(xorg-module-dir,
            AS_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])
AC_SUBST([moduledir])

# Define a configure option to enable/disable lcd panel support
AC_ARG_ENABLE(geodegx-panel, 
	      AS_HELP_STRING([--disable-geodegx-panel],
	      	             [Disable support for flatpanels with the Geode GX]),
	      [ ], 
	      [ PANEL_CPPFLAGS=-DPNL_SUP ])
AC_SUBST(PANEL_CPPFLAGS)

# Define a configure option to enable/disable ztv
AC_ARG_ENABLE(ztv,
	AS_HELP_STRING([--enable-ztv],
	    [Enable Video For Linux based ZTV driver (default: auto-detected)]),
	    [ztv=$enableval],
	    [ztv=auto])

# Check for Video4Linux Version 2 (V4L2) availability
AC_CHECK_HEADERS([linux/videodev2.h],[v4l2=yes],[v4l2=no])
if test "x$ztv" != "xno" ; then
    if test "x$v4l2" = "xno" ; then
	if test "x$ztv" = "xyes" ; then
	    # User really wants ztv but V4L2 is not found
	    AC_MSG_ERROR([ZTV driver requested, but videodev2.h not found.])
	fi
    else
	BUILD_ZTV=yes
    fi
fi
AM_CONDITIONAL(BUILD_ZTV, [test "x$BUILD_ZTV" = xyes])

# Check if GCC supports compiling in 32 bit mode for 64 bit computers
case $host_cpu in
    x86_64*|amd64*)
	if  test "x$GCC" = xyes ; then
	    SAVE_CFLAGS="$CFLAGS"
	    CFLAGS="$CFLAGS -m32"
	    AC_MSG_CHECKING([if $CC supports the -m32 Intel/AMD option])
	    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include <unistd.h>
int
main ()
{
  ;
  return 0;
}]])],
		[m32_support=yes; M32_CFLAGS=-m32],
		[m32_support=no])
	    AC_MSG_RESULT([$m32_support])
	    CFLAGS="$SAVE_CFLAGS"
	fi
	;;
esac
AC_SUBST([M32_CFLAGS])

# Store the list of server defined optional extensions in REQUIRED_MODULES
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XV, videoproto)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Obtain compiler/linker options for the Geode driver dependencies
PKG_CHECK_MODULES(XORG, [xorg-server xproto fontsproto $REQUIRED_MODULES])
PKG_CHECK_MODULES(XEXT, [xextproto >= 7.0.99.1],
                  HAVE_XEXTPROTO_71="yes"; AC_DEFINE(HAVE_XEXTPROTO_71, 1, [xextproto 7.1 available]),
                  HAVE_XEXTPROTO_71="no")

# Define a configure option to enable/disable XAA support;
# handles unavailability of XAA since xserver-1.13
AC_ARG_ENABLE(xaa,
    AS_HELP_STRING([--enable-xaa],
                   [Enable legacy X Acceleration Architecture (XAA) [default=auto]]))

AS_IF([test "x$enable_xaa" != "xno"],
    [
        save_CFLAGS=$CFLAGS
        save_CPPFLAGS=$CPPFLAGS
        CFLAGS=$XORG_CFLAGS
        CPPFLAGS="$XORG_CFLAGS"
        AC_CHECK_HEADERS([xaa.h], have_XAA=yes, have_XAA=no)
        CFLAGS=$save_CFLAGS
        CPPFLAGS=$save_CPPFLAGS
])

AC_MSG_CHECKING([whether to include XAA support])

AS_IF([test "x$have_XAA" = "xyes"],
    [AC_MSG_RESULT([yes])],
    [AS_IF([test "x$enable_xaa" = "xyes"],
        [AC_MSG_ERROR([XAA requested but not found (perhaps building against xserver-1.13 or newer?)])],
        [AC_MSG_RESULT([no])
    ])
])

# Checks for libpciaccess support.
SAVE_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $XORG_CFLAGS"
AC_CHECK_DECL(XSERVER_LIBPCIACCESS,
              [XSERVER_LIBPCIACCESS=yes],[XSERVER_LIBPCIACCESS=no],
              [#include "xorg-server.h"])
CPPFLAGS="$SAVE_CPPFLAGS"

if test "x$XSERVER_LIBPCIACCESS" = xyes; then
    PKG_CHECK_MODULES([PCIACCESS], [pciaccess >= 0.8.0])
    XORG_CFLAGS="$XORG_CFLAGS $PCIACCESS_CFLAGS"
fi

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])
AC_OUTPUT
