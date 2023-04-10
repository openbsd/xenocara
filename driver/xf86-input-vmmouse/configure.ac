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

AC_PREREQ(2.60)
AC_INIT([xf86-input-vmmouse],
        [13.2.0],
        [https://gitlab.freedesktop.org/xorg/driver/xf86-input-vmmouse/issues],
        xf86-input-vmmouse)

AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_AUX_DIR(.)
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign dist-xz])

# XORG_DEFAULT_OPTIONS below forces C99. Solaris GCC doesn't like that.
AC_PROG_CC
AC_PROG_CC_C89
save_solaris_cc=$CC

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

DRIVER_NAME=vmmouse
AC_SUBST([DRIVER_NAME])

# Checks for programs.
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AH_TOP([#include "xorg-server.h"])

AC_ARG_WITH(xorg-module-dir,
            AS_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])
inputdir=${moduledir}/input
AC_SUBST(inputdir)

AC_ARG_WITH(xorg-conf-dir,
            AS_HELP_STRING([--with-xorg-conf-dir=DIR],
                           [Default xorg.conf.d directory [[default=from $PKG_CONFIG xorg-server]]]),
            [XORG_CONF_DIR="$withval"],
            [XORG_CONF_DIR="`$PKG_CONFIG --variable=sysconfigdir xorg-server`"])
AC_SUBST(XORG_CONF_DIR)
AM_CONDITIONAL(HAS_XORG_CONF_DIR, [test "x$XORG_CONF_DIR" != "x" && test "x$XORG_CONF_DIR" != "xno"])

AC_ARG_WITH(hal-bin-dir,
	    AS_HELP_STRING([--with-hal-bin-dir=DIR],
			   [Directory where HAL binaries where installed [[default=$bindir]]]),
	    [halbindir="$withval"],
	    [halbindir="$bindir"])
HAL_BIN_DIR=${halbindir}
AC_SUBST(HAL_BIN_DIR)

AC_ARG_WITH(hal-callouts-dir,
	    AS_HELP_STRING([--with-hal-callouts-dir=DIR],
			   [Directory where HAL expects its callout scripts to be located
			    [[default=$libdir/hal]]]),
	    [halcalloutsdir="$withval"],
	    [halcalloutsdir="$libdir/hal"])
HAL_CALLOUTS_DIR=${halcalloutsdir}
AC_SUBST(HAL_CALLOUTS_DIR)

AC_ARG_WITH(hal-fdi-dir,
	    AS_HELP_STRING([--with-hal-fdi-dir=DIR],
			   [Directory where HAL expects its fdi files to be located
			   [[default=$datadir/hal/fdi/policy/20thirdparty]]]),
	    [halfdidir="$withval"],
	    [halfdidir="$datadir/hal/fdi/policy/20thirdparty"])
HAL_FDI_DIR=${halfdidir}
AC_SUBST(HAL_FDI_DIR)

# Udev location for rules directory
AC_ARG_WITH(udev-rules-dir,
	    AS_HELP_STRING([--with-udev-rules-dir=DIR],
			   [Directory where udev expects its rules files
			   [[default=$libdir/udev/rules.d]]]),
	    [udevdir="$withval"],
	    [udevdir="$libdir/udev/rules.d"])
UDEV_RULES_DIR=${udevdir}
AC_SUBST(UDEV_RULES_DIR)
AM_CONDITIONAL(HAS_UDEV_RULES_DIR, [test "x$UDEV_RULES_DIR" != "xno"])

# Checks for extensions
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(XINPUT, inputproto)

# Checks for pkg-config packages
libudev_check=yes
AC_ARG_WITH([libudev],
	[AS_HELP_STRING([--without-libudev],
		[Use to build without libudev on linux])],
	[if test x$withval = xno; then libudev_check=no; fi]
	[])

case $host_os in
     linux*)
	 if test $libudev_check != no; then
             PKG_CHECK_MODULES(LIBUDEV, [libudev],
					[AC_DEFINE([HAVE_LIBUDEV], 1,
					[Has libudev installed])],
					[]);
         fi
	 AC_CHECK_FUNCS(ioperm iopl,[],
			[AC_MSG_ERROR
			([cannot determine how to elevate io permissions)]],[1])
	 AC_DEFINE(VMMOUSE_OS_GENERIC, 1,
	           [Building for iopl / ioperm capable OS])
     ;;
     *bsd*)
         AC_DEFINE(VMMOUSE_OS_BSD, 1, [Building for BSD flavour])
     ;;
     solaris*)
	 if test "x$GCC" = "xyes"; then
            CC="$save_solaris_cc -fms-extensions"
	 fi
         AC_DEFINE(VMMOUSE_OS_SOLARIS, 1, [Building for Solaris flavour])
     ;;
     *)
	 AC_CHECK_FUNCS(ioperm iopl,[],
			[AC_MSG_ERROR
			([cannot determine how to elevate io permissions)]],[1])
	 AC_DEFINE(VMMOUSE_OS_GENERIC, 1)
     ;;
esac

case $host_cpu in
     i*86)
	case $host_os in
		*freebsd*)	AC_DEFINE(USE_DEV_IO) ;;
		*netbsd*)	AC_DEFINE([USE_I386_IOPL], [], [BSD i386 iopl])
				use_i386_iopl=yes ;;
	esac
	;;
     x86_64*|amd64*)
	case $host_os in
		*freebsd*)	AC_DEFINE(USE_DEV_IO, 1, [BSD /dev/io]) ;;
		*netbsd*)	AC_DEFINE(USE_X86_64_IOPL, 1, [BSD X86_64 iopl])
				use_x86_64_iopl=yes ;;
	esac
	;;
esac

if test x$use_i386_iopl = xyes; then
   AC_CHECK_LIB(i386, i386_iopl,[],
		[AC_MSG_ERROR([cannot find library for i386_iopl])])
fi

if test x$use_amd64_iopl = xyes; then
   AC_CHECK_LIB(amd64, amd64_iopl,[],
		[AC_MSG_ERROR([cannot find library for amd64_iopl])])
fi

if test x$use_x86_64_iopl = xyes; then
   AC_CHECK_LIB(x86_64, x86_64_iopl,[],
                [AC_MSG_ERROR([cannot find library for x86_64_iopl])])
fi

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

AC_CONFIG_FILES([Makefile
	shared/Makefile
	src/Makefile
	tools/Makefile
	fdi/Makefile
	man/Makefile])

AC_OUTPUT
