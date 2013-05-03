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
AC_INIT([xf86-input-keyboard],
        [1.7.0],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        [xf86-input-keyboard])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR(.)

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# Initialize X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Obtain compiler/linker options from server and required extensions
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.4] xproto inputproto)

# Define a configure option for an alternate input module directory
AC_ARG_WITH(xorg-module-dir,
            AC_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])
inputdir=${moduledir}/input
AC_SUBST(inputdir)

# The keyboard driver code is O/S specific
case $host_os in
  linux*)
    IS_LINUX="yes"
    ;;

  freebsd* | kfreebsd-gnu* | netbsd* | openbsd* | dragonfly*)
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

DRIVER_NAME=kbd
AC_SUBST([DRIVER_NAME])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 man/Makefile])
AC_OUTPUT
