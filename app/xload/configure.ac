
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
AC_INIT([xload], [1.2.0],
	[https://gitlab.freedesktop.org/xorg/app/xload/-/issues], [xload])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_USE_SYSTEM_EXTENSIONS

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-xz])

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Checks for typedefs, structures, and compiler characteristics.
AC_SYS_LARGEFILE

# Checks for library functions.
AC_CHECK_FUNCS([asprintf])

# Internationalization & localization support
AC_SEARCH_LIBS([gettext], [intl], [USE_GETTEXT="yes"], [USE_GETTEXT="no"])
AC_MSG_CHECKING([where to install localized messages])
AC_ARG_WITH([localedir], AS_HELP_STRING([--with-localedir=<path>],
        [Path to install message files in (default: datadir/locale)]),
        [LOCALEDIR=${withval}], [LOCALEDIR=${datadir}/locale])
AX_DEFINE_DIR([LOCALEDIR], [LOCALEDIR], [Location of translated messages])
if test "x$LOCALEDIR" = "xno" -o "x$USE_GETTEXT" = "xno" ; then
        AC_MSG_RESULT([nowhere])
        USE_GETTEXT="no"
else
        AC_MSG_RESULT([$LOCALEDIR])
fi

if test "x$USE_GETTEXT" = "xyes" ; then
        AC_DEFINE([USE_GETTEXT], 1,
                  [Define to 1 if you want to use the gettext() function.])
fi
AM_CONDITIONAL(USE_GETTEXT, test "x$USE_GETTEXT" = "xyes")


### How to check load average on various OS'es:

# getloadavg: 4.3BSD-Reno & later, glibc 2.2 & later, Solaris 7 & later
# BSD & GNU libc use <stdlib.h>, Solaris requires <sys/loadavg.h>
AC_CHECK_FUNCS([getloadavg], [AC_CHECK_HEADERS([sys/loadavg.h])])

AC_CHECK_HEADERS([paths.h protocols/rwhod.h])

# Checks for pkg-config packages
PKG_CHECK_MODULES(XLOAD, xaw7 xmu xt x11 [xproto >= 7.0.17])

PKG_CHECK_MODULES(APPDEFS, xt)
xt_appdefaultdir=`$PKG_CONFIG --variable=appdefaultdir xt`
AC_ARG_WITH(appdefaultdir,
	AS_HELP_STRING([--with-appdefaultdir=<pathname>],
	  [specify directory for app-defaults files (default is autodetected)]),
	[appdefaultdir="$withval"], [appdefaultdir="${xt_appdefaultdir}"])
AC_SUBST(appdefaultdir)


AC_CONFIG_FILES([
	Makefile
	man/Makefile])
AC_OUTPUT
