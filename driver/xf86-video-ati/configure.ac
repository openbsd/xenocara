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
AC_INIT([xf86-video-ati],
        [7.5.0],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        [xf86-video-ati])

AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_AUX_DIR(.)

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AC_SYS_LARGEFILE

AM_MAINTAINER_MODE

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# Checks for programs.
AM_PROG_CC_C_O

if test "x$GCC" = "xyes"; then
	CPPFLAGS="$CPPFLAGS -Wall"
fi

AH_TOP([#include "xorg-server.h"])

# Define a configure option for an alternate module directory
AC_ARG_WITH(xorg-module-dir,
            AS_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

# Store the list of server defined optional extensions in REQUIRED_MODULES
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XV, videoproto)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Checks for libraries.
PKG_CHECK_MODULES(LIBDRM, [libdrm >= 2.4.58])
PKG_CHECK_MODULES(LIBDRM_RADEON, [libdrm_radeon])

# Obtain compiler/linker options for the driver dependencies
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.7 xproto fontsproto xf86driproto $REQUIRED_MODULES])
PKG_CHECK_MODULES(XEXT, [xextproto >= 7.0.99.1],
                  HAVE_XEXTPROTO_71="yes"; AC_DEFINE(HAVE_XEXTPROTO_71, 1, [xextproto 7.1 available]),
                  HAVE_XEXTPROTO_71="no")
AM_CONDITIONAL(HAVE_XEXTPROTO_71, [ test "$HAVE_XEXTPROTO_71" = "yes" ])

AC_ARG_ENABLE([udev],
		AS_HELP_STRING([--disable-udev], [Disable libudev support [default=auto]]),
		[enable_udev="$enableval"],
		[enable_udev=auto])
if test "x$enable_udev" != "xno"; then
	PKG_CHECK_MODULES(LIBUDEV, [libudev], [LIBUDEV=yes], [LIBUDEV=no])
	if test "x$LIBUDEV" = xyes; then
		AC_DEFINE(HAVE_LIBUDEV, 1,[libudev support])
	elif test "x$enable_udev" != "xauto"; then
		AC_MSG_ERROR([Building with udev requested but libudev not found])
	fi
fi
AM_CONDITIONAL(LIBUDEV, test x$LIBUDEV = xyes)

SAVE_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $XORG_CFLAGS"

AC_MSG_CHECKING([whether to include GLAMOR support])
AC_ARG_ENABLE(glamor,
	      AS_HELP_STRING([--disable-glamor],
			     [Disable glamor, a new GL-based acceleration [default=enabled]]),
	      [GLAMOR="$enableval"],
	      [GLAMOR=yes])

if test "x$GLAMOR" != "xno"; then
	AC_CHECK_HEADERS([glamor.h], [GLAMOR_H="yes"], [GLAMOR_H="no"], [#include "xorg-server.h"])

	if test "x$GLAMOR_H" = xyes; then
		AC_CHECK_DECL(GLAMOR_NO_DRI3,
			      [GLAMOR_XSERVER="yes"], [GLAMOR_XSERVER="no"],
			      [#include "xorg-server.h"
			       #include "glamor.h"])
	fi

	if test "x$GLAMOR_XSERVER" != xyes; then
		PKG_CHECK_MODULES(LIBGLAMOR, [glamor >= 0.6.0])
		PKG_CHECK_MODULES(LIBGLAMOR_EGL, [glamor-egl])
	fi
	AC_DEFINE(USE_GLAMOR, 1, [Enable glamor acceleration])
else
	AC_MSG_RESULT([$GLAMOR])
fi
AM_CONDITIONAL(GLAMOR, test x$GLAMOR != xno)

AC_CHECK_HEADERS([list.h],
		 [have_list_h="yes"], [have_list_h="no"],
		 [#include <X11/Xdefs.h>
		  #include "xorg-server.h"])

if test "x$have_list_h" = xyes; then
    AC_CHECK_DECL(xorg_list_init,
		  [AC_DEFINE(HAVE_XORG_LIST, 1, [Have xorg_list API])], [],
		  [#include <X11/Xdefs.h>
		   #include "xorg-server.h"
		   #include "list.h"])
fi

CPPFLAGS="$SAVE_CPPFLAGS"

PKG_CHECK_MODULES([PCIACCESS], [pciaccess >= 0.8.0])
XORG_CFLAGS="$XORG_CFLAGS $PCIACCESS_CFLAGS"

# Checks for headers/macros for byte swapping
# Known variants:
#	<byteswap.h> bswap_16, bswap_32, bswap_64  (glibc)
#	<sys/endian.h> __swap16, __swap32, __swap64 (OpenBSD)
#	<sys/endian.h> bswap16, bswap32, bswap64 (other BSD's)
#	and a fallback to local macros if none of the above are found

# if <byteswap.h> is found, assume it's the correct version
AC_CHECK_HEADERS([byteswap.h])

# if <sys/endian.h> is found, have to check which version
AC_CHECK_HEADER([sys/endian.h], [HAVE_SYS_ENDIAN_H="yes"], [HAVE_SYS_ENDIAN_H="no"])

if test "x$HAVE_SYS_ENDIAN_H" = "xyes" ; then
	AC_MSG_CHECKING([for __swap16 variant of <sys/endian.h> byteswapping macros])
	AC_LINK_IFELSE([AC_LANG_PROGRAM([
#include <sys/types.h>
#include <sys/endian.h>
 ], [
int a = 1, b;
b = __swap16(a);
 ])
], [SYS_ENDIAN__SWAP='yes'], [SYS_ENDIAN__SWAP='no'])
	AC_MSG_RESULT([$SYS_ENDIAN__SWAP])

	AC_MSG_CHECKING([for bswap16 variant of <sys/endian.h> byteswapping macros])
	AC_LINK_IFELSE([AC_LANG_PROGRAM([
#include <sys/types.h>
#include <sys/endian.h>
 ], [
int a = 1, b;
b = bswap16(a);
 ])
], [SYS_ENDIAN_BSWAP='yes'], [SYS_ENDIAN_BSWAP='no'])
	AC_MSG_RESULT([$SYS_ENDIAN_BSWAP])

    	if test "$SYS_ENDIAN_BSWAP" = "yes" ; then
		USE_SYS_ENDIAN_H=yes
		BSWAP=bswap
	else	
	    	if test "$SYS_ENDIAN__SWAP" = "yes" ; then
			USE_SYS_ENDIAN_H=yes
			BSWAP=__swap
		else
			USE_SYS_ENDIAN_H=no
		fi
	fi

	if test "$USE_SYS_ENDIAN_H" = "yes" ; then
	    AC_DEFINE([USE_SYS_ENDIAN_H], 1, 
		[Define to use byteswap macros from <sys/endian.h>])
	    AC_DEFINE_UNQUOTED([bswap_16], ${BSWAP}16, 
			[Define to 16-bit byteswap macro])
	    AC_DEFINE_UNQUOTED([bswap_32], ${BSWAP}32, 
			[Define to 32-bit byteswap macro])
	    AC_DEFINE_UNQUOTED([bswap_64], ${BSWAP}64, 
			[Define to 64-bit byteswap macro])
	fi
fi

AC_SUBST([moduledir])

DRIVER_NAME=ati
AC_SUBST([DRIVER_NAME])

AC_MSG_NOTICE(
[The atimisc sub-driver has been split out to xf86-video-mach64:]
[    git://anongit.freedesktop.org/git/xorg/driver/xf86-video-mach64]
[Please install that driver as well for mach64-based cards.]
)

AC_MSG_NOTICE(
[The r128 sub-driver has been split out to xf86-video-r128:]
[    git://anongit.freedesktop.org/git/xorg/driver/xf86-video-r128]
[Please install that driver as well for rage128-based cards.]
)

AC_CONFIG_FILES([
                Makefile
                src/Makefile
                man/Makefile
])
AC_OUTPUT

dnl
dnl Output some configuration info for the user
dnl
echo ""
echo "        prefix:              $prefix"
echo "        exec_prefix:         $exec_prefix"
echo "        libdir:              $libdir"
echo "        includedir:          $includedir"

echo ""
echo "        CFLAGS:              $CFLAGS"
echo "        CXXFLAGS:            $CXXFLAGS"
echo "        Macros:              $DEFINES"

echo ""
echo "        Run '${MAKE-make}' to build xf86-video-ati"
echo ""
