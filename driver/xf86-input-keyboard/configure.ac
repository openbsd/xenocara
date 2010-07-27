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
AC_INIT([xf86-input-keyboard],
        1.4.0,
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        xf86-input-keyboard)

# Require xorg-macros version 1.3 or newer for XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION], [AC_FATAL([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)

AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_AUX_DIR(.)
AM_INIT_AUTOMAKE([dist-bzip2])

AM_MAINTAINER_MODE

DRIVER_NAME=kbd
AC_SUBST([DRIVER_NAME])

AM_CONFIG_HEADER([config.h])

# Checks for programs.
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_PROG_CC

AH_TOP([#include "xorg-server.h"])

case $host_os in
  linux*)
    IS_LINUX="yes"
    # check whether struct kbd_repeat has the 'period' field.
    # on kernels < 2.5.42 it's called 'rate' instead.
    AC_TRY_COMPILE([
#include <linux/kd.h>
#ifdef __sparc__
#include <asm/param.h>
#include <asm/kbio.h>
#endif
],[
int main () {
        struct kbd_repeat k;
        k.period = 0;
        return 0;
}],
    [period_field="period"],
    [period_field="rate"])
    AC_DEFINE_UNQUOTED(LNX_KBD_PERIOD_NAME, [$period_field],
                       [Name of the period field in struct kbd_repeat])
    ;;

  freebsd* | kfreebsd-gnu* | netbsd* | openbsd*)
    IS_BSD="yes"
    ;;

  solaris*)
    IS_SOLARIS="yes"
    ;;

  gnu*)
    IS_HURD="yes"
    ;;

  *)
    AC_MSG_ERROR([Your operating system is not supported by the kbd driver.
                  Contact xorg@lists.freedesktop.org if you are interested in
                  porting it.])
    ;;
esac
AM_CONDITIONAL(LINUX, [test "x$IS_LINUX" = xyes])
AM_CONDITIONAL(BSD, [test "x$IS_BSD" = xyes])
AM_CONDITIONAL(SOLARIS, [test "x$IS_SOLARIS" = xyes])
AM_CONDITIONAL(HURD, [test "x$IS_HURD" = xyes])
AM_CONDITIONAL(SCO, [test "x$IS_SCO" = xyes])

AC_ARG_WITH(xorg-module-dir,
            AC_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])
AC_SUBST(moduledir)

# Checks for extensions
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(XINPUT, inputproto)
XORG_DRIVER_CHECK_EXT(XKB, kbproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.2.99.0] xproto $REQUIRED_MODULES)
sdkdir=$(pkg-config --variable=sdkdir xorg-server)

XORG_DEFAULT_OPTIONS
XORG_CFLAGS="$CWARNFLAGS $XORG_CFLAGS"
AC_SUBST([XORG_CFLAGS])

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC

AC_OUTPUT([Makefile src/Makefile man/Makefile])
