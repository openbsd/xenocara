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
AC_INIT([xf86-input-mouse],
        [1.9.1],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        [xf86-input-mouse])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR(.)
AC_USE_SYSTEM_EXTENSIONS

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# Initialize X.Org macros
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS
XORG_WITH_LINT

# Checks for library functions
AC_CHECK_FUNCS([asprintf])

# Obtain compiler/linker options from server and required extensions
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.7] xproto inputproto)

# Define a configure option for an alternate input module directory
AC_ARG_WITH(xorg-module-dir,
            AS_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])
inputdir=${moduledir}/input
AC_SUBST(inputdir)

# X Server SDK location is required to install xf86-mouse-properties.h
# This location is also relayed in the xorg-mouse.pc file
sdkdir=`$PKG_CONFIG --variable=sdkdir xorg-server`

# Workaround overriding sdkdir to be able to create a tarball when user has no
# write permission in sdkdir. See DISTCHECK_CONFIGURE_FLAGS in Makefile.am
AC_ARG_WITH([sdkdir], [], [sdkdir="$withval"])
AC_SUBST([sdkdir])


# Work out which OS mouse driver we need to build
case $host_os in
  linux*)
    OS_MOUSE_NAME=lnx
    ;;
  freebsd*|kfreebsd*-gnu|dragonfly*|netbsd*|openbsd*)
    OS_MOUSE_NAME=bsd
    ;;
  solaris*)
    OS_MOUSE_NAME=sun
    ;;
  gnu*)
    OS_MOUSE_NAME=hurd
    ;;
esac
AC_SUBST([OS_MOUSE_NAME])

DRIVER_NAME=mouse
AC_SUBST([DRIVER_NAME])

AC_CONFIG_FILES([Makefile
                 xorg-mouse.pc
                 include/Makefile
                 src/Makefile
                 man/Makefile])
AC_OUTPUT
