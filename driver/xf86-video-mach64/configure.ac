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
AC_INIT([xf86-video-mach64],
        [6.9.5],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        [xf86-video-mach64])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR(.)

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AH_TOP([#include "xorg-server.h"])

# Define a configure option for an alternate module directory
AC_ARG_WITH(xorg-module-dir,
            AS_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

AC_ARG_ENABLE(dri, AS_HELP_STRING([--disable-dri],
                                  [Disable DRI support [[default=auto]]]),
              [DRI="$enableval"],
              [DRI=auto])

AC_ARG_ENABLE(exa,
              AS_HELP_STRING([--disable-exa],
                             [Disable EXA support [[default=enabled]]]),
              [EXA="$enableval"],
              [EXA=yes])

# Store the list of server defined optional extensions in REQUIRED_MODULES
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XV, videoproto)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Obtain compiler/linker options for the driver dependencies
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.4 xproto fontsproto $REQUIRED_MODULES])
PKG_CHECK_MODULES(XEXT, [xextproto >= 7.0.99.1],
                  HAVE_XEXTPROTO_71="yes"; AC_DEFINE(HAVE_XEXTPROTO_71, 1, [xextproto 7.1 available]),
                  HAVE_XEXTPROTO_71="no")
AM_CONDITIONAL(HAVE_XEXTPROTO_71, [ test "$HAVE_XEXTPROTO_71" = "yes" ])
sdkdir=`$PKG_CONFIG --variable=sdkdir xorg-server`

# Checks for libraries.

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

# Note that this is sort of inverted from drivers/ati/Imakefile in
# the monolith.  We test for foo, not for !foo (i.e. ATMISC_CPIO, not
# ATIMISC_AVOID_CPIO), but the defines are negative.  So beware.  Oh yeah,
# TV_OUT is the special case where it's a positive define, not AVOID_TV_OUT.

# AVOID_CPIO: Only x86, amd64, and alpha are cool with CPIO.  It needs a
# little-endian, undirected PIO space of at least 64kB.
# AVOID_DGA: ???
# ATI_TV_OUT: This only works on x86.

ATIMISC_CPIO=no
ATIMISC_DGA=yes
ATIMISC_TV_OUT=no

case $host_cpu in
  i*86)
    ATIMISC_TV_OUT=yes
    ATIMISC_CPIO=yes
    ;;
  x86_64|amd64|alpha|ia64)
    ATIMISC_CPIO=yes
    ;;
  sparc)
    ATIMISC_DGA=no
    ;;
  *)
    ;;
esac

AC_MSG_CHECKING([whether to include PIO support])
AM_CONDITIONAL(ATIMISC_CPIO, test "x$ATIMISC_CPIO" = xyes)
if test "x$ATIMISC_CPIO" = xyes; then
	AC_MSG_RESULT([yes, PIO])
else
	AC_DEFINE(AVOID_CPIO, 1, [Avoid PIO and use MMIO for atimisc.])
	AC_MSG_RESULT([no, MMIO])
fi

AC_MSG_CHECKING([whether to include DGA support])
AC_MSG_RESULT([$ATIMISC_DGA])
AM_CONDITIONAL(ATIMISC_DGA, test "x$ATIMISC_DGA" = xyes)
if ! test "x$ATIMISC_DGA" = xyes; then
	AC_DEFINE(AVOID_DGA, 1, [Do not build DGA support.])
fi

AC_MSG_CHECKING([whether to include TV Out support])
AC_MSG_RESULT([$ATIMISC_TV_OUT])
AM_CONDITIONAL(ATIMISC_TV_OUT, test "x$ATIMISC_TV_OUT" = xyes)
if test "x$ATIMISC_TV_OUT" = xyes; then
	AC_DEFINE(TV_OUT, 1, [Build TV-Out support for atimisc.])
fi

# Properly handle EXA.
AC_MSG_CHECKING([whether to enable EXA support])
if test "x$EXA" = xyes; then
        AC_MSG_RESULT(yes)

        SAVE_CPPFLAGS="$CPPFLAGS"
        CPPFLAGS="$CPPFLAGS $XORG_CFLAGS"
        AC_CHECK_HEADER(exa.h,
                       [have_exa_h="yes"], [have_exa_h="no"])
        CPPFLAGS="$SAVE_CPPFLAGS"
else
        AC_MSG_RESULT(no)
fi 

SAVE_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $XORG_CFLAGS"
if test "x$have_exa_h" = xyes; then
        AC_MSG_CHECKING([whether EXA version is at least 2.0.0])
        AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[
#include "exa.h"
#if EXA_VERSION_MAJOR < 2
#error OLD EXA!
#endif
                          ]])],
                          [USE_EXA=yes],
                          [USE_EXA=no])
        AC_MSG_RESULT($USE_EXA)

        if test "x$USE_EXA" = xyes; then
                AC_DEFINE(USE_EXA, 1, [Build support for Exa])
        fi
fi

AC_ARG_ENABLE(xaa,
              AS_HELP_STRING([--enable-xaa],
                             [Enable legacy X Acceleration Architecture (XAA) [default=auto]]),
              [XAA="$enableval"],
              [XAA=auto])
if test "x$XAA" != xno; then
        save_CFLAGS=$CFLAGS
        save_CPPFLAGS=$CPPFLAGS
        CFLAGS=$XORG_CFLAGS
        CPPFLAGS="$XORG_CFLAGS"
        AC_CHECK_HEADERS([xaa.h], XAA=yes, XAA=no)
        CFLAGS=$save_CFLAGS
        CPPFLAGS=$save_CPPFLAGS
fi
AC_MSG_CHECKING([whether to include XAA support])
AM_CONDITIONAL(XAA, test "x$XAA" = xyes)
if test "x$XAA" = xyes; then
        AC_DEFINE(USE_XAA, test "x$XAA" = xyes, [Build support for XAA])
fi
AC_MSG_RESULT([$XAA])

AC_CHECK_DECL(XSERVER_LIBPCIACCESS,
	      [XSERVER_LIBPCIACCESS=yes],[XSERVER_LIBPCIACCESS=no],
	      [#include "xorg-server.h"])

CPPFLAGS="$SAVE_CPPFLAGS"

AM_CONDITIONAL(USE_EXA, test "x$USE_EXA" = xyes)

if test "x$XSERVER_LIBPCIACCESS" = xyes; then
    PKG_CHECK_MODULES([PCIACCESS], [pciaccess >= 0.12.901])
    XORG_CFLAGS="$XORG_CFLAGS $PCIACCESS_CFLAGS"
fi
AM_CONDITIONAL(XSERVER_LIBPCIACCESS, test "x$XSERVER_LIBPCIACCESS" = xyes)

AC_SUBST([moduledir])

DRIVER_NAME=mach64
AC_SUBST([DRIVER_NAME])

AC_MSG_NOTICE(
[Please change the Driver line in xorg.conf from "ati" to "mach64"]
[ or install the ati wrapper as well:]
[    git://anongit.freedesktop.org/git/xorg/driver/xf86-video-ati]
)

AC_CONFIG_FILES([
                Makefile
                src/Makefile
                man/Makefile
])
AC_OUTPUT
