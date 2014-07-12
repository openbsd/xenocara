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
AC_PREREQ([2.60])
AC_INIT([xf86-video-modesetting],
        [0.9.0],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        [xf86-video-modesetting])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR(.)
AC_SYS_LARGEFILE

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# Initialize X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
	[m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

AH_TOP([#include "xorg-server.h"])

AC_ARG_WITH(xorg-module-dir,
            AS_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

# Checks for extensions
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(XF86DRI, xextproto x11)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, [xorg-server xproto fontsproto $REQUIRED_MODULES])
sdkdir=$(pkg-config --variable=sdkdir xorg-server)

PKG_CHECK_MODULES(XEXT, [xextproto >= 7.0.99.1],
                  HAVE_XEXTPROTO_71="yes"; AC_DEFINE(HAVE_XEXTPROTO_71, 1, [xextproto 7.1 available]),
                  HAVE_XEXTPROTO_71="no")
AM_CONDITIONAL(HAVE_XEXTPROTO_71, [ test "$HAVE_XEXTPROTO_71" = "yes" ])


# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC

PKG_CHECK_MODULES(DRM, [libdrm >= 2.4.46])
PKG_CHECK_MODULES([PCIACCESS], [pciaccess >= 0.10])
AM_CONDITIONAL(DRM, test "x$DRM" = xyes)

PKG_CHECK_MODULES(UDEV, [libudev], [udev=yes], [udev=no])
if test x"$udev" = xyes; then
        AC_DEFINE(HAVE_UDEV,1,[Enable udev-based monitor hotplug detection])
fi

SAVE_CFLAGS=$CFLAGS
SAVE_LIBS=$LIBS
CFLAGS=$DRM_CFLAGS
LIBS=$DRM_LIBS
AC_CHECK_FUNCS([drmPrimeFDToHandle])
CFLAGS=$SAVE_CFLAGS
LIBS=$SAVE_LIBS

DRIVER_NAME=modesetting
AC_SUBST([DRIVER_NAME])
AC_SUBST([moduledir])

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	man/Makefile
])

AC_OUTPUT
