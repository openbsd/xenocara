
dnl  Copyright 2005 Red Hat, Inc.
dnl
dnl  Permission to use, copy, modify, distribute, and sell this software and its
dnl  documentation for any purpose is hereby granted without fee, provided that
dnl  the above copyright notice appear in all copies and that both that
dnl  copyright notice and this permission notice appear in supporting
dnl  documentation, and that the name of Red Hat not be used in
dnl  advertising or publicity pertaining to distribution of the software without
dnl  specific, written prior permission.  Red Hat makes no
dnl  representations about the suitability of this software for any purpose.  It
dnl  is provided "as is" without express or implied warranty.
dnl
dnl  RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
dnl  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
dnl  EVENT SHALL RED HAT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
dnl  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
dnl  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
dnl  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
dnl  PERFORMANCE OF THIS SOFTWARE.
dnl
dnl Process this file with autoconf to create configure.

# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([bitmap], [1.1.1],
	[https://gitlab.freedesktop.org/xorg/app/bitmap/issues], [bitmap])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-xz])

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

AM_PROG_CC_C_O

# Checks for typedefs, structures, and compiler characteristics.
AC_SYS_LARGEFILE

# Checks for library functions.
AC_CHECK_FUNCS([mkstemp])

# Math libraries & functions
# - some compilers use builtin inlines for floor when optimizing
# - lrint() is a C99 addition not found on some older systems
# - must do the libm check first so that the lrint check will have it in $LIBS
save_LIBS="$LIBS"
AC_SEARCH_LIBS([floor], [m])
AC_SEARCH_LIBS([lrint], [m])
AC_CHECK_FUNCS([lrint])
MATH_LIBS="$LIBS"
LIBS="$save_LIBS"
AC_SUBST([MATH_LIBS])

# Obtain compiler/linker options from dependencies
PKG_CHECK_MODULES(BMTOA, [x11 xmu xproto >= 7.0.17])
PKG_CHECK_MODULES(ATOBM, [xproto >= 7.0.25])
PKG_CHECK_MODULES(BITMAP, xbitmaps xaw7 xmu)

PKG_CHECK_MODULES(APPDEFS, xt)
xt_appdefaultdir=`$PKG_CONFIG --variable=appdefaultdir xt`
AC_ARG_WITH(appdefaultdir,
	AS_HELP_STRING([--with-appdefaultdir=<pathname>],
	  [specify directory for app-defaults files (default is autodetected)]),
	[appdefaultdir="$withval"], [appdefaultdir="${xt_appdefaultdir}"])
AC_SUBST(appdefaultdir)

AC_CONFIG_FILES([Makefile
           man/Makefile])
AC_CONFIG_FILES([test/round-trip-test], [chmod +x test/round-trip-test])
AC_OUTPUT
