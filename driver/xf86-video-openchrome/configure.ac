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
AC_INIT([xf86-video-openchrome],
        [0.6.409],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg&component=Driver/openchrome],
        [xf86-video-openchrome])

AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR(.)

AM_INIT_AUTOMAKE([foreign dist-bzip2])

# Require xorg-macros: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION], 
          [m4_fatal([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)
XORG_DEFAULT_OPTIONS

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AC_SYS_LARGEFILE

AH_TOP([#include "xorg-server.h"])

AC_ARG_WITH(xorg-module-dir,
            AS_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

AC_ARG_ENABLE(dri, AS_HELP_STRING([--disable-dri],
                                  [Disable DRI support [[default=auto]]]),
              [DRI="$enableval"],
              [DRI=no])

AC_ARG_ENABLE(kms, AS_HELP_STRING([--disable-kms],
                                  [Disable KMS support [[default=enabled]]]),
              [DRM_MODE="$enableval"],
              [DRM_MODE=yes])

AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug],
                                    [Enable debugging support [[default=no]]]),
              [DEBUG="$enableval"],
              [DEBUG=no])

AC_ARG_ENABLE(xv-debug, AS_HELP_STRING([--enable-xv-debug],
                                       [Enable XVideo debugging support [[default=no]]]),
              [XV_DEBUG="$enableval"],
              [XV_DEBUG=no])

AC_ARG_ENABLE(viaregtool, AS_HELP_STRING([--enable-viaregtool],
                                         [Enable build of registers dumper tool [[default=no]]]),
              [TOOLS="$enableval"],
              [TOOLS=no])

# Checks for extensions
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XV, videoproto)
XORG_DRIVER_CHECK_EXT(XF86DRI, xextproto x11)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, [xorg-server xproto fontsproto glproto $REQUIRED_MODULES])
PKG_CHECK_MODULES(XEXT, [xextproto >= 7.0.99.1],
                  HAVE_XEXTPROTO_71="yes"; AC_DEFINE(HAVE_XEXTPROTO_71, 1, [xextproto 7.1 available]),
                  HAVE_XEXTPROTO_71="no")
AM_CONDITIONAL(HAVE_XEXTPROTO_71, [ test "$HAVE_XEXTPROTO_71" = "yes" ])
sdkdir=$(pkg-config --variable=sdkdir xorg-server)

# Checks for libraries.
SAVE_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $XORG_CFLAGS"
AC_CHECK_DECL(XSERVER_LIBPCIACCESS,
             [XSERVER_LIBPCIACCESS=yes], [XSERVER_LIBPCIACCESS=no],
             [#include "xorg-server.h"])
CPPFLAGS="$SAVE_CPPFLAGS"

if test x$XSERVER_LIBPCIACCESS = xyes; then
    PKG_CHECK_MODULES([PCIACCESS], [pciaccess >= 0.8.0])
fi

if test "$DRI" != no; then
    PKG_CHECK_MODULES(DRI, [libdrm >= 2.2 xf86driproto])
    if test -f "${sdkdir}/dri.h"; then
        have_dri_h="yes"
    else
        have_dri_h="no"
    fi
    if test -f "${sdkdir}/sarea.h"; then
        have_sarea_h="yes"
    else
        have_sarea_h="no"
    fi
    if test -f "${sdkdir}/dristruct.h"; then
        have_dristruct_h="yes"
    else
        have_dristruct_h="no"
    fi
    if test -f "${sdkdir}/damage.h"; then
        have_damage_h="yes"
    else
        have_damage_h="no"
    fi
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
    AC_DEFINE(OPENCHROMEDRI, 1, [Enable DRI driver support])
    AC_DEFINE(OPENCHROMEDRI_DEVEL, 1, [Enable developmental DRI driver support])
    save_CFLAGS="$CFLAGS"
    CFLAGS="$XORG_CFLAGS $DRI_CFLAGS $CFLAGS"
    if test "x$DRM_MODE" = xyes; then
        AC_CHECK_HEADER(xf86drmMode.h,
                        [DRM_KMS=yes], [DRM_KMS=no],
                        [#include <stdint.h>
                         #include <stdlib.h>])
        if test "x$DRM_KMS" = xyes; then
            PKG_CHECK_MODULES(LIBUDEV, [libudev],
                              [LIBUDEV=yes], [LIBUDEV=no])
            if test "x$LIBUDEV" = xyes; then
                AC_DEFINE(HAVE_LIBUDEV, 1, [libudev support])
            fi
        fi
    else
        DRM_KMS=no
    fi
    CFLAGS="$save_CFLAGS"

    case "$host_cpu" in
        i*86)
            XVMC=yes ;;
        amd64*|x86_64*)
            XVMC=yes ;;
        *)
            XVMC=no ;;
    esac
else
    DRM_KMS=no
    XVMC="no"
fi

AC_MSG_CHECKING([whether to include KMS support])
AC_MSG_RESULT([$DRM_KMS])

if test "x$XVMC" = xyes; then
    AC_CHECK_HEADERS(pthread.h sys/ioctl.h sys/time.h time.h,,[XVMC="no"; break],)
    PKG_CHECK_MODULES(XVMC, [x11 xext xvmc])
fi

AC_MSG_CHECKING([whether to build XvMC driver support])
AC_MSG_RESULT([$XVMC])

AM_CONDITIONAL(XF86DRM_MODE, test "x$DRM_KMS" = xyes)

AM_CONDITIONAL(XVMC, test x$XVMC = xyes)

AM_CONDITIONAL(XSERVER_LIBPCIACCESS, test x$XSERVER_LIBPCIACCESS = xyes)
if test "$XSERVER_LIBPCIACCESS" = yes; then
    AC_DEFINE(XSERVER_LIBPCIACCESS, 1, [Enable libpciaccess])
fi

AM_CONDITIONAL(DEBUG, test x$DEBUG = xyes)
if test "$DEBUG" = yes; then
    AC_DEFINE(HAVE_DEBUG, 1, [Enable debug support])
fi

AM_CONDITIONAL(XV_DEBUG, test x$XV_DEBUG = xyes)
if test "$XV_DEBUG" = yes; then
    AC_DEFINE(XV_DEBUG, 1, [Enable XVideo debug support])
fi

AM_CONDITIONAL(TOOLS, test x$TOOLS = xyes)
if test "$TOOLS" = yes; then
    AC_DEFINE(TOOLS, 1, [Enable build of registers dumper tool])
fi

AC_DEFINE(X_USE_REGION_NULL, 1, [Compatibility define for older Xen])
AC_DEFINE(X_NEED_I2CSTART, 1, [Compatibility define for older Xen])

AC_SUBST([DRI_CFLAGS])
AC_SUBST([moduledir])

DRIVER_NAME=openchrome
AC_SUBST([DRIVER_NAME])

AC_CONFIG_FILES([
                Makefile
                src/xvmc/Makefile
                src/Makefile
                man/Makefile
                tools/Makefile
])
AC_OUTPUT
