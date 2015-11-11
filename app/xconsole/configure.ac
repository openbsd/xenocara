
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
AC_INIT([xconsole],
	[1.0.6],
	[https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
	[xconsole])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Checks for pkg-config packages
PKG_CHECK_MODULES(XCONSOLE, xaw7 xmu xt >= 1.0 x11 xproto >= 7.0.17)

PKG_CHECK_MODULES(APPDEFS, xt)
xt_appdefaultdir=`$PKG_CONFIG --variable=appdefaultdir xt`
AC_ARG_WITH(appdefaultdir,
	AS_HELP_STRING([--with-appdefaultdir=<pathname>],
	  [specify directory for app-defaults files (default is autodetected)]),
	[appdefaultdir="$withval"], [appdefaultdir="${xt_appdefaultdir}"])
AC_SUBST(appdefaultdir)

AC_SEARCH_LIBS(openpty, [util])
AC_CHECK_FUNC(openpty, [HAVE_OPENPTY="yes"])
if test "x$HAVE_OPENPTY" = "xyes" ; then
	AC_DEFINE([HAVE_OPENPTY], 1,
		[Define to 1 if you have the 'openpty' function.])
	AC_CHECK_HEADERS([util.h libutil.h pty.h], [break])
fi

# Privilege separation when running as root
AC_ARG_ENABLE(privsep,
	AC_HELP_STRING([--enable-privsep],
		[enable privilege separation]),
	[USE_PRIVSEP="$enableval"],
	USE_PRIVSEP="no")
# Privilege separation requires openpty()
if test "x$HAVE_OPENPTY" = "xno" ; then
	AC_MSG_ERROR([privilege separation code requires openpty()])
	USE_PRIVSEP="no"
fi
if test "x$USE_PRIVSEP" = "xyes" ; then
	AC_DEFINE([USE_PRIVSEP], 1, [Define to 1 to use privilege separation])
	AC_SEARCH_LIBS(setproctitle, [util])
	AC_CHECK_FUNC(setproctitle, [HAVE_SETPROCTITLE="yes"])
	if  test "x$HAVE_SETPROCTILE" = "xyes" ; then
		AC_DEFINE([HAVE_SETPROCTITLE], 1,
			[Define to 1 if you have the 'setproctitle function.])
	fi
	AC_CHECK_FUNCS(setresuid, [
		dnl Some platorms have setresuid that isn't implemented, 
		dnl test for this
		AC_MSG_CHECKING(if setresuid seems to work)
		AC_RUN_IFELSE(
			[AC_LANG_SOURCE([[
#include <stdlib.h>
#include <errno.h>
int main(){errno=0; setresuid(0,0,0); if (errno==ENOSYS) exit(1); else exit(0);}
			]])],
			[AC_MSG_RESULT(yes)],
			[AC_DEFINE(BROKEN_SETRESUID, 1, 
				[ENOSYS: Not implemented])
		 	AC_MSG_RESULT(not implemented)]
		)
	])

	AC_CHECK_FUNCS(setresgid, [
		dnl Some platorms have setresgid that isn't implemented, 
		dnl test for this
		AC_MSG_CHECKING(if setresgid seems to work)
		AC_RUN_IFELSE(
			[AC_LANG_SOURCE([[
#include <stdlib.h>
#include <errno.h>
int main(){errno=0; setresgid(0,0,0); if (errno==ENOSYS) exit(1); else exit(0);}
			]])],
			[AC_MSG_RESULT(yes)],
			[AC_DEFINE(BROKEN_SETRESGID, 1, 
				[ENOSYS: Not implemented])
		 	AC_MSG_RESULT(not implemented)]
		)
	])
fi
AM_CONDITIONAL(USE_PRIVSEP, test "x$USE_PRIVSEP" = "xyes")
AC_ARG_WITH(privsep-user,
	AC_HELP_STRING([--with-privsep-user=user],
		[Specify privilege separation user]),
	privsep_user="$withval",
	privsep_user="_x11")
if test "x$USE_PRIVSEP" = "xyes" ; then
	AC_DEFINE_UNQUOTED(XCONSOLE_USER, "$privsep_user", 
		[Unprivileged userid])
fi

AC_SUBST(privsep_user)

AC_CONFIG_FILES([
	Makefile
	man/Makefile])
AC_OUTPUT
