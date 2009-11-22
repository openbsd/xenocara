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
AC_INIT([xf86-video-r128],
        6.8.1,
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        xf86-video-r128)

AC_CONFIG_SRCDIR([Makefile.am])
AM_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR(.)

AM_INIT_AUTOMAKE([dist-bzip2])

# Require xorg-macros: XORG_CWARNFLAGS, XORG_CHANGELOG
m4_ifndef([XORG_MACROS_VERSION], [AC_FATAL([must install xorg-macros 1.2 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.2)

AM_MAINTAINER_MODE

# Checks for programs.
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_PROG_CC

if test "x$GCC" = "xyes"; then
	CPPFLAGS="$CPPFLAGS -Wall"
fi

AH_TOP([#include "xorg-server.h"])

AC_ARG_WITH(xorg-module-dir,
            AC_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

AC_ARG_ENABLE(dri, AC_HELP_STRING([--disable-dri],
                                  [Disable DRI support [[default=auto]]]),
              [DRI="$enableval"],
              [DRI=auto])

# Checks for extensions
XORG_DRIVER_CHECK_EXT(XINERAMA, xineramaproto)
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XV, videoproto)
XORG_DRIVER_CHECK_EXT(XF86MISC, xf86miscproto)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.2 xproto fontsproto $REQUIRED_MODULES])
PKG_CHECK_MODULES(XEXT, [xextproto >= 7.0.99.1],
                  HAVE_XEXTPROTO_71="yes"; AC_DEFINE(HAVE_XEXTPROTO_71, 1, [xextproto 7.1 available]),
                  HAVE_XEXTPROTO_71="no")
AM_CONDITIONAL(HAVE_XEXTPROTO_71, [ test "$HAVE_XEXTPROTO_71" = "yes" ])
sdkdir=$(pkg-config --variable=sdkdir xorg-server)

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC

if test "$DRI" != no; then
	AC_CHECK_FILE([${sdkdir}/dri.h],
                      [have_dri_h="yes"], [have_dri_h="no"])
	AC_CHECK_FILE([${sdkdir}/sarea.h],
                      [have_sarea_h="yes"], [have_sarea_h="no"])
	AC_CHECK_FILE([${sdkdir}/dristruct.h],
                      [have_dristruct_h="yes"], [have_dristruct_h="no"])
	AC_CHECK_FILE([${sdkdir}/damage.h],
                      [have_damage_h="yes"], [have_damage_h="no"])
fi

AC_MSG_CHECKING([whether to include DRI support])
if test x$DRI = xauto; then
	if test "$have_dri_h" = yes -a \
                "$have_sarea_h" = yes -a \
                "$have_dristruct_h" = yes; then
		DRI="yes"
	else
		DRI="no"
	fi
fi
AC_MSG_RESULT([$DRI])

AM_CONDITIONAL(DRI, test x$DRI = xyes)
if test "$DRI" = yes; then
	PKG_CHECK_MODULES(DRI, [libdrm >= 2.2 xf86driproto])
        AC_DEFINE(XF86DRI,1,[Enable DRI driver support])
        AC_DEFINE(XF86DRI_DEVEL,1,[Enable developmental DRI driver support])
fi

SAVE_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $XORG_CFLAGS"
AC_CHECK_DECL(XSERVER_LIBPCIACCESS,
	      [XSERVER_LIBPCIACCESS=yes],[XSERVER_LIBPCIACCESS=no],
	      [#include "xorg-server.h"])
CPPFLAGS="$SAVE_CPPFLAGS"

AM_CONDITIONAL(USE_EXA, test "x$USE_EXA" = xyes)

if test "x$XSERVER_LIBPCIACCESS" = xyes; then
    PKG_CHECK_MODULES([PCIACCESS], [pciaccess >= 0.8.0])
    XORG_CFLAGS="$XORG_CFLAGS $PCIACCESS_CFLAGS"
fi
AM_CONDITIONAL(XSERVER_LIBPCIACCESS, test "x$XSERVER_LIBPCIACCESS" = xyes)

AC_SUBST([XORG_CFLAGS])
AC_SUBST([DRI_CFLAGS])
AC_SUBST([moduledir])

DRIVER_NAME=r128
AC_SUBST([DRIVER_NAME])

XORG_MANPAGE_SECTIONS
XORG_RELEASE_VERSION
XORG_CHECK_LINUXDOC
XORG_CHANGELOG

AC_MSG_NOTICE(
[Please change the Driver line in xorg.conf from "ati" to "r128" or install]
[the ati wrapper as well:]
[    git://anongit.freedesktop.org/git/xorg/driver/xf86-video-ati]
)

AC_OUTPUT([
	Makefile
	src/Makefile
	man/Makefile
])
