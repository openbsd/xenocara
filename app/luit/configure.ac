
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

AC_PREREQ([2.57])
AC_INIT(luit,[1.0.4], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],luit)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

# Require xorg-macros: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION], [AC_FATAL([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)

AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_PROG_INSTALL
AC_CANONICAL_HOST

XORG_DEFAULT_OPTIONS

AC_CHECK_HEADERS([poll.h pty.h stropts.h sys/param.h sys/select.h])
AC_CHECK_FUNCS([poll select grantpt])

AC_CHECK_LIB(z, gzopen,, AC_MSG_ERROR([zlib not found]))

AC_ARG_WITH(localealiasfile,
        AC_HELP_STRING([--with-localealiasfile=<path>],
                       [The locale alias file (default: ${datadir}/X11/locale/locale.alias)]),
        [LOCALEALIASFILE="$withval"],
        [LOCALEALIASFILE=${datadir}/X11/locale/locale.alias])
AC_SUBST([LOCALEALIASFILE])

# Checks for pkg-config packages
PKG_CHECK_MODULES(LUIT, x11 fontenc)

case $host_os in
    linux*)
        OS_CFLAGS="-D_XOPEN_SOURCE=500"
        ;;
    *)
        OS_CFLAGS=
        ;;
esac

LUIT_CFLAGS="$LUIT_CFLAGS $OS_CFLAGS"
AC_SUBST(LUIT_CFLAGS)
AC_SUBST(LUIT_LIBS)

AC_OUTPUT([Makefile])
