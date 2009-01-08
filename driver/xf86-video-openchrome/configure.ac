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
AC_INIT([xf86-video-openchrome],
        0.2.903,
        [http://www.openchrome.org/trac/report/1],
        xf86-video-openchrome)

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

AC_ARG_WITH(xorg-module-dir,
            AC_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

AC_ARG_ENABLE(dri, AC_HELP_STRING([--disable-dri],
                                  [Disable DRI support [[default=auto]]]),
              [DRI="$enableval"],
              [DRI=auto])

AC_ARG_ENABLE(debug, AC_HELP_STRING([--enable-debug],
				    [Enable debugging support [[default=no]]]),
              [DEBUG="$enableval"],
              [DEBUG=no])

AC_ARG_ENABLE(xv-debug, AC_HELP_STRING([--enable-xv-debug],
				    [Enable XVideo debugging support [[default=no]]]),
              [XV_DEBUG="$enableval"],
              [XV_DEBUG=no])

# Checks for extensions
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XV, videoproto)
XORG_DRIVER_CHECK_EXT(XF86DRI, xextproto x11)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, [xorg-server xproto xvmc fontsproto libdrm $REQUIRED_MODULES])
sdkdir=$(pkg-config --variable=sdkdir xorg-server)

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADER(xf86Modes.h,[XMODES=yes],[XMODES=no],[#include "xorg-server.h"])
AC_CHECK_DECL(XSERVER_LIBPCIACCESS,
             [XSERVER_LIBPCIACCESS=yes],[XSERVER_LIBPCIACCESS=no],
             [#include "xorg/xorg-server.h"])

if test x$XSERVER_LIBPCIACCESS = xyes; then
       PKG_CHECK_MODULES([PCIACCESS], [pciaccess >= 0.8.0])
fi

if test "$DRI" != no; then
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

AM_CONDITIONAL(DRI, test x$DRI = xyes)
if test "$DRI" = yes; then
        PKG_CHECK_MODULES(DRI, [libdrm xf86driproto])
        AC_DEFINE(CHROMEDRI,1,[Enable DRI driver support])
        AC_DEFINE(CHROMEDRI_DEVEL,1,[Enable developmental DRI driver support])
        case "$host_cpu" in
            i*86)
                XVMC=yes ;;
            amd64*|x86_64*)
                XVMC=yes ;;
            *)
                XVMC=no ;;
        esac
else
        XVMC="no"
fi

if test "x$XVMC" = xyes; then
	AC_CHECK_HEADERS(pthread.h sys/ioctl.h sys/time.h time.h,,[XVMC="no"; break],)
fi

AC_MSG_CHECKING([whether to build XvMC driver support])
AC_MSG_RESULT([$XVMC])

AM_CONDITIONAL(XVMC, test x$XVMC = xyes)

AC_CHECK_FILE([${sdkdir}/xf86Module.h],
              [have_xf86Module_h="yes"], [have_xf86Module_h="no"])

# Check the ABI_VIDEODRV_VERSION
SAVE_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS $XORG_CFLAGS"
		
if test "x$have_xf86Module_h" = xyes; then
AC_MSG_CHECKING([whether to use old Xv ABI])
        AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[
                           ]])],
                           [OLD_XVABI=no],
			   [OLD_XVABI=yes])
else
        OLD_XVABI=yes
        echo -n "xf86Module.h not found, "
fi

CFLAGS="$SAVE_CFLAGS"

if test "x$OLD_XVABI" = xyes; then
                echo "yes."
else
                echo "no."
                AC_DEFINE(USE_NEW_XVABI, 1, [Use new Xv ABI (7.1RC1+)])
fi

AM_CONDITIONAL(XSERVER_LIBPCIACCESS, test x$XSERVER_LIBPCIACCESS = xyes)
if test "$XSERVER_LIBPCIACCESS" = yes; then
        AC_DEFINE(XSERVER_LIBPCIACCESS,1,[Enable libpciaccess])
fi

AM_CONDITIONAL(DEBUG, test x$DEBUG = xyes)
if test "$DEBUG" = yes; then
        AC_DEFINE(HAVE_DEBUG,1,[Enable debug support])
fi

AM_CONDITIONAL(XV_DEBUG, test x$XV_DEBUG = xyes)
if test "$XV_DEBUG" = yes; then
        AC_DEFINE(XV_DEBUG,1,[Enable XVideo debug support])
fi

AC_SUBST([DRI_CFLAGS])
AC_SUBST([XORG_CFLAGS])
AC_SUBST([moduledir])

DRIVER_NAME=openchrome
AC_SUBST([DRIVER_NAME])

AC_DEFINE(X_USE_LINEARFB,1,[Compatibility define for older Xen])
AC_DEFINE(X_USE_REGION_NULL,1,[Compatibility define for older Xen])
AC_DEFINE(X_HAVE_XAAGETROP,1,[Compatibility define for older Xen])
AC_DEFINE(X_NEED_I2CSTART,1,[Compatibility define for older Xen])
AC_DEFINE(VIA_HAVE_EXA,1,[Build support for Exa])

DRIVER_MAN_SUFFIX="4"
AC_SUBST([DRIVER_MAN_SUFFIX])

XORG_RELEASE_VERSION

AC_OUTPUT([
	Makefile
	libxvmc/Makefile
	man/Makefile
	src/Makefile
])
