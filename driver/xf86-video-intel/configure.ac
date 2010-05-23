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
AC_INIT([xf86-video-intel],
        2.9.1,
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        xf86-video-intel)

AC_CONFIG_SRCDIR([Makefile.am])
AM_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR(.)

AM_INIT_AUTOMAKE([foreign dist-bzip2])

AM_MAINTAINER_MODE

# Require xorg-macros: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION], 
          [m4_fatal([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)
XORG_DEFAULT_OPTIONS

# Checks for programs.
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_PROG_CC
AM_PROG_CC_C_O

PKG_CHECK_MODULES(GEN4ASM, [intel-gen4asm >= 1.0], [gen4asm=yes], [gen4asm=no])
AM_CONDITIONAL(HAVE_GEN4ASM, test x$gen4asm = xyes)

AC_HEADER_STDC
AC_CHECK_HEADERS(sys/mman.h)
AC_CHECK_FUNCS(mprotect)

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

AC_ARG_ENABLE(video-debug, AC_HELP_STRING([--enable-video-debug],
                                  [Enable video debugging support [[default=no]]]),
              [VIDEO_DEBUG="$enableval"],
              [VIDEO_DEBUG=no])

AC_ARG_ENABLE(xvmc, AC_HELP_STRING([--disable-xvmc],
                                  [Disable XvMC support [[default=yes]]]),
              [XVMC="$enableval"],
              [XVMC=yes])

AC_ARG_ENABLE(kms-only, AC_HELP_STRING([--enable-kms-only],
                                  [Assume KMS support [[default=no]]]),
              [KMS_ONLY="$enableval"],
              [KMS_ONLY=no])

# Checks for extensions
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XF86DRI, xextproto x11)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.6 xproto fontsproto $REQUIRED_MODULES])
PKG_CHECK_MODULES(DRM, [libdrm >= 2.4.11])
PKG_CHECK_MODULES(PCIACCESS, [pciaccess >= 0.10])
PKG_CHECK_MODULES(XEXT, [xextproto >= 7.0.99.1],
                  HAVE_XEXTPROTO_71="yes"; AC_DEFINE(HAVE_XEXTPROTO_71, 1, [xextproto 7.1 available]),
                  HAVE_XEXTPROTO_71="no")
AM_CONDITIONAL(HAVE_XEXTPROTO_71, [ test "$HAVE_XEXTPROTO_71" = "yes" ])

sdkdir=$(pkg-config --variable=sdkdir xorg-server)

save_CFLAGS="$CFLAGS"
CFLAGS="$XORG_CFLAGS $DRI_CFLAGS $DRM_CFLAGS"
CPPFLAGS="$XORG_CFLAGS $DRI_CFLAGS $DRM_CFLAGS"
AC_MSG_CHECKING([whether to include DRI support])
if test x$DRI != xno; then
        AC_CHECK_FILE([${sdkdir}/dri.h],
                      [have_dri_h="yes"], [have_dri_h="no"])
        AC_CHECK_FILE([${sdkdir}/sarea.h],
                      [have_sarea_h="yes"], [have_sarea_h="no"])
        AC_CHECK_FILE([${sdkdir}/dristruct.h],
                      [have_dristruct_h="yes"], [have_dristruct_h="no"])
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
CFLAGS="$save_CFLAGS $DEBUGFLAGS"

AM_CONDITIONAL(DRI, test x$DRI = xyes)
if test "$DRI" = yes; then
        PKG_CHECK_MODULES(DRI, [xf86driproto glproto])
        AC_DEFINE(XF86DRI,1,[Enable DRI driver support])
        AC_DEFINE(XF86DRI_DEVEL,1,[Enable developmental DRI driver support])
fi

AM_CONDITIONAL(VIDEO_DEBUG, test x$VIDEO_DEBUG = xyes)
if test "$VIDEO_DEBUG" = yes; then
	AC_DEFINE(VIDEO_DEBUG,1,[Enable debug support])
fi

if test "$XVMC" = yes; then
	PKG_CHECK_MODULES(XVMCLIB,
			  [xvmc xext xfixes dri2proto x11-xcb xcb-dri2 xcb-aux],
			  [XVMC=yes], [XVMC=no])
fi
AC_MSG_CHECKING([whether to include XvMC support])
AC_MSG_RESULT([$XVMC])
AM_CONDITIONAL(XVMC, test x$XVMC = xyes)
if test "$XVMC" = yes; then
	AC_DEFINE(ENABLE_XVMC,1,[Enable XvMC support])
fi

AM_CONDITIONAL(KMS_ONLY, test x$KMS_ONLY = xyes)
if test "$KMS_ONLY" = yes; then
	AC_DEFINE(KMS_ONLY,1,[Assume KMS support])
fi

AC_SUBST([DRI_CFLAGS])

DRIVER_NAME=intel
AC_SUBST([DRIVER_NAME])
AC_SUBST([moduledir])

XORG_CHECK_LINUXDOC

AC_OUTPUT([
	Makefile
	uxa/Makefile
	src/Makefile
	src/xvmc/Makefile
	src/xvmc/shader/Makefile
	src/xvmc/shader/mc/Makefile
	src/xvmc/shader/vld/Makefile
	src/bios_reader/Makefile
	src/ch7017/Makefile
	src/ch7xxx/Makefile
	src/ivch/Makefile
	src/reg_dumper/Makefile
	src/sil164/Makefile
	src/tfp410/Makefile
	man/Makefile
	src/render_program/Makefile
])
