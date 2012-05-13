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
AC_INIT([xf86-input-vmmouse],
        [12.8.0],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        xf86-input-vmmouse)

AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_AUX_DIR(.)
AM_INIT_AUTOMAKE([foreign dist-bzip2])

AM_MAINTAINER_MODE

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

DRIVER_NAME=vmmouse
AC_SUBST([DRIVER_NAME])

AM_CONFIG_HEADER([config.h])

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
inputdir=${moduledir}/input
AC_SUBST(inputdir)

AC_ARG_WITH(xorg-conf-dir,
            AC_HELP_STRING([--with-xorg-conf-dir=DIR],
                           [Default xorg.conf.d directory [[default=from $PKG_CONFIG xorg-server]]]),
            [XORG_CONF_DIR="$withval"],
            [XORG_CONF_DIR="`$PKG_CONFIG --variable=sysconfigdir xorg-server`"])
AC_SUBST(XORG_CONF_DIR)
AM_CONDITIONAL(HAS_XORG_CONF_DIR, [test "x$XORG_CONF_DIR" != "x" && test "x$XORG_CONF_DIR" != "xno"])

AC_ARG_WITH(hal-bin-dir,
	    AC_HELP_STRING([--with-hal-bin-dir=DIR],
			   [Directory where HAL binaries where installed [[default=$bindir]]]),
	    [halbindir="$withval"],
	    [halbindir="$bindir"])
HAL_BIN_DIR=${halbindir}
AC_SUBST(HAL_BIN_DIR)

AC_ARG_WITH(hal-callouts-dir,
	    AC_HELP_STRING([--with-hal-callouts-dir=DIR],
			   [Directory where HAL expects its callout scripts to be located
			    [[default=$libdir/hal]]]),
	    [halcalloutsdir="$withval"],
	    [halcalloutsdir="$libdir/hal"])
HAL_CALLOUTS_DIR=${halcalloutsdir}
AC_SUBST(HAL_CALLOUTS_DIR)

AC_ARG_WITH(hal-fdi-dir,
	    AC_HELP_STRING([--with-hal-fdi-dir=DIR],
			   [Directory where HAL expects its fdi files to be located
			   [[default=$datadir/hal/fdi/policy/20thirdparty]]]),
	    [halfdidir="$withval"],
	    [halfdidir="$datadir/hal/fdi/policy/20thirdparty"])
HAL_FDI_DIR=${halfdidir}
AC_SUBST(HAL_FDI_DIR)

case $host_os in
  linux*)
    if test "x$prefix" = "xNONE" -o "x$prefix" = "x/usr" -o "x$prefix" = "x/usr/local" ; then
      slashlibdir=/lib
    else
      slashlibdir=$prefix/lib
    fi
    UDEV_RULES_DIR=$slashlibdir/udev/rules.d
    ;;
  *)
    UDEV_RULES_DIR=no
    ;;
esac

AC_ARG_WITH(udev-rules-dir,
	    AC_HELP_STRING([--with-udev-rules-dir=DIR],
			   [Default udev rules.d directory
			    [[default=($prefix)/lib/udev/rules.d on Linux, none otherwise]]]),
	    [UDEV_RULES_DIR="$withval"],
	    [])
AC_SUBST(UDEV_RULES_DIR)
AM_CONDITIONAL(HAS_UDEV_RULES_DIR, [test "x$UDEV_RULES_DIR" != "xno"])

# Checks for extensions
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(XINPUT, inputproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.0.1] xproto $REQUIRED_MODULES)

PKG_CHECK_EXISTS([xorg-server >= 1.1.0],
                 [AC_DEFINE([HAVE_XORG_SERVER_1_1_0], 1,
                            [Has version 1.1.0 or greater of the Xserver])])

PKG_CHECK_EXISTS([xorg-server > 1.4.99],
		[AC_DEFINE([HAVE_XORG_SERVER_1_5_0], 1,
		[Has version 1.5.0 or greater of the Xserver])])

PKG_CHECK_EXISTS([xorg-server >= 1.3.99.0 xorg-server <= 1.4.0.90],
                 [AC_DEFINE([CALL_CONVERSION_PROC], 1,
                            [Call conversion_proc from within the driver, as
                             Xserver doesn't])])
PKG_CHECK_EXISTS([xorg-server > 1.4.0.90],
		 [AC_DEFINE([ABS_VALUATOR_AXES], 1,
			    [Define absolute valuator axes])])
PKG_CHECK_EXISTS([xorg-server >= 1.6.0],
		 [AC_DEFINE([NO_MOUSE_MODULE], 1,
			    [Do not include built-in mouse module fallback])])

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC

AC_OUTPUT([Makefile shared/Makefile src/Makefile tools/Makefile fdi/Makefile man/Makefile])
