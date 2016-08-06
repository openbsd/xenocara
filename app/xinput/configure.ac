# Copyright © 2007 Peter Hutterer
# Copyright © 2009 Red Hat, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([xinput], [1.6.2],
	[https://bugs.freedesktop.org/enter_bug.cgi?product=xorg], [xinput])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Obtain compiler/linker options for dependencies
PKG_CHECK_MODULES(XINPUT, x11 xext [xi >= 1.2] [inputproto >= 1.5] xrandr xinerama)

# XI2 support
PKG_CHECK_MODULES(XI2, [xi >= 1.2.99.2] [inputproto >= 1.9.99.15],
                  HAVE_XI2="yes"; AC_DEFINE(HAVE_XI2, 1, [XI2 available]),
                  HAVE_XI2="no");
AM_CONDITIONAL(HAVE_XI2, [ test "$HAVE_XI2" = "yes" ])
AC_SUBST(HAVE_XI2)

# XI2.1 support
PKG_CHECK_MODULES(XI21, [xi >= 1.4.99.1] [inputproto >= 2.0.99.1],
                  AC_DEFINE(HAVE_XI21, 1, [XI2.1 available]));
# XI2.2 support
PKG_CHECK_MODULES(XI22, [xi >= 1.5.99.1] [inputproto >= 2.1.99.1],
                  AC_DEFINE(HAVE_XI22, 1, [XI2.2 available]));

AC_CONFIG_FILES([Makefile
           src/Makefile
           man/Makefile])
AC_OUTPUT
