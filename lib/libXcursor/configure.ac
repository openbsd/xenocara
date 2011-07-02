#
#  Copyright © 2003 Keith Packard
#
#  Permission to use, copy, modify, distribute, and sell this software and its
#  documentation for any purpose is hereby granted without fee, provided that
#  the above copyright notice appear in all copies and that both that
#  copyright notice and this permission notice appear in supporting
#  documentation, and that the name of Keith Packard not be used in
#  advertising or publicity pertaining to distribution of the software without
#  specific, written prior permission.  Keith Packard makes no
#  representations about the suitability of this software for any purpose.  It
#  is provided "as is" without express or implied warranty.
#
#  KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
#  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
#  EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
#  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
#  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
#  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
#  PERFORMANCE OF THIS SOFTWARE.
#

# Initialize Autoconf
AC_PREREQ([2.60])
#
# This is the package version number, not the shared library
# version.  This same version number must appear in Xcursor.h
# Yes, it is a pain to synchronize version numbers.  Unfortunately, it's
# not possible to extract the version number here from Xcursor.h
#
AC_INIT([libXcursor], [1.1.12],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],[libXcursor])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Initialize libtool
AC_PROG_LIBTOOL

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

AC_ARG_WITH(icondir,
        AS_HELP_STRING([--with-icondir=<path>],
                       [Set default icon directory (default: ${datadir}/icons)]),
        [ICONDIR="$withval"],
        [ICONDIR=${datadir}/icons])
AC_SUBST([ICONDIR])

DEF_CURSORPATH="~/.icons:${datadir}/icons:${datadir}/pixmaps"
if test "x${ICONDIR}" != "x${datadir}/icons"; then
	DEF_CURSORPATH="${DEF_CURSORPATH}:${ICONDIR}"
fi
AC_ARG_WITH(cursorpath,
        AS_HELP_STRING([--with-cursorpath=<paths>],
                       [Set default search path for cursors]),
        [XCURSORPATH="$withval"],
        [XCURSORPATH=$DEF_CURSORPATH])
AC_SUBST([XCURSORPATH])

# Obtain compiler/linker options for depedencies
PKG_CHECK_MODULES(XCURSOR, xrender >= 0.8.2 xfixes x11 fixesproto)
AC_DEFINE(HAVE_XFIXES, 1, [Define to 1 if you have Xfixes])

# Allow checking code with lint, sparse, etc.
XORG_WITH_LINT

AC_CONFIG_FILES([Makefile
		src/Makefile
		man/Makefile
		xcursor.pc])
AC_OUTPUT
