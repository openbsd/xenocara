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
AC_INIT(xinit,[1.0.6], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],xinit)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

AM_CONFIG_HEADER(config.h)

AC_PROG_CC
XORG_PROG_RAWCPP
AC_PROG_INSTALL
AC_PATH_PROG(SED,sed)
AC_CANONICAL_HOST

AC_TYPE_SIGNAL
AC_FUNC_FORK

AC_CHECK_FUNCS([killpg])

m4_ifdef([AS_HELP_STRING], , [m4_define([AS_HELP_STRING], m4_defn([AC_HELP_STRING]))])

# Build options
DEFAULT_XRDB=xrdb
DEFAULT_XMODMAP=xmodmap
DEFAULT_TWM=twm
DEFAULT_XCLOCK=xclock
DEFAULT_XTERM=xterm
# You always want to specify the full path to the X server
DEFAULT_XSERVER=${bindir}/X
DEFAULT_XAUTH=xauth
DEFAULT_XINIT=xinit
DEFAULT_WM=fvwm
DEFAULT_XCONSOLE=xconsole

AC_ARG_WITH(xrdb,
	 AS_HELP_STRING([--with-xrdb=XRDB], [Path to xrdb]),
	[XRDB="$withval"],
	[XRDB="$DEFAULT_XRDB"])

AC_ARG_WITH(xmodmap,
	 AS_HELP_STRING([--with-xmodmap=XMODMAP], [Path to xmodmap]),
	[XMODMAP="$withval"],
	[XMODMAP="$DEFAULT_XMODMAP"])

AC_ARG_WITH(twm,
	 AS_HELP_STRING([--with-twm=TWM], [Path to twm]),
	[TWM="$withval"],
	[TWM="$DEFAULT_TWM"])

AC_ARG_WITH(xclock,
	 AS_HELP_STRING([--with-xclock=XCLOCK], [Path to xclock]),
	[XCLOCK="$withval"],
	[XCLOCK="$DEFAULT_XCLOCK"])

AC_ARG_WITH(xterm,
	 AS_HELP_STRING([--with-xterm=XTERM], [Path to xterm]),
	[XTERM="$withval"],
	[XTERM="$DEFAULT_XTERM"])

AC_ARG_WITH(xserver,
	 AS_HELP_STRING([--with-xserver=XSERVER], [Path to default X server]),
	[XSERVER="$withval"],
	[XSERVER="$DEFAULT_XSERVER"])

AC_ARG_WITH(xauth,
	 AS_HELP_STRING([--with-xauth=XAUTH], [Path to xauth]),
	[XAUTH="$withval"],
	[XAUTH="$DEFAULT_XAUTH"])

AC_ARG_WITH(xinit,
	 AS_HELP_STRING([--with-xinit=XINIT], [Path to xinit]),
	[XINIT="$withval"],
	[XINIT="$DEFAULT_XINIT"])

AC_ARG_WITH(wm,
	AS_HELP_STRING([--with-wm=WM], [Path to default window manager]),
	[WM="$withval"],
	[WM="$DEFAULT_WM"])

AC_ARG_WITH(xconsole,
	 AS_HELP_STRING([--with-xconsole=XCONSOLE], [Path to xconsole]),
	[XCONSOLE="$withval"],
	[XCONSOLE="$DEFAULT_XCONSOLE"])

# Checks for pkg-config packages
PKG_CHECK_MODULES(XINIT, x11)

case $host_os in
    *bsd*)
        XINIT_CFLAGS="$XINIT_CFLAGS -DCSRG_BASED"
        ;;
esac

AC_SUBST(XINIT_CFLAGS)
AC_SUBST(XINIT_LIBS)

AC_PATH_PROGS(MCOOKIE, [mcookie], [$MCOOKIE],
  [$PATH:/bin:/usr/bin:/usr/lib:/usr/libexec:/usr/local/bin])

case $host_os in
	*openbsd*)
	MCOOKIE='dd if=/dev/urandom bs=16 count=1 2>/dev/null | hexdump -e \\\"%08x\\\"'
	;;
esac

if test "x$MCOOKIE" != x ; then
	STARTX_COOKIE_FLAGS='-DHAS_COOKIE_MAKER -DMK_COOKIE="$(MCOOKIE)"'
fi
AC_SUBST(STARTX_COOKIE_FLAGS)

# Additional substitutions in xinitrc & man pages
case $host_os in
    *sco*)	
	SHELL_CMD="/bin/ksh"
	ARCHMANDEFS="-D__SCOMAN__"
	;;
    *)
	SHELL_CMD="/bin/sh"
	ARCHMANDEFS=""
	;;
esac
AC_SUBST(SHELL_CMD)
AC_SUBST(ARCHMANDEFS)

AC_SUBST(XRDB)
AC_SUBST(XMODMAP)
AC_SUBST(TWM)
AC_SUBST(XCLOCK)
AC_SUBST(XTERM)
AC_SUBST(XSERVER)
AC_SUBST(XAUTH)
AC_SUBST(XINIT)
AC_SUBST(WM)
AC_SUBST(XCONSOLE)

# Defines which X servers are listed in help output in xinit.c
# Cheat, lie, and assume all the world is Xorg for now
AC_DEFINE(XORG, 1, [Define if you have Xorg X server])

XORG_MANPAGE_SECTIONS
XORG_RELEASE_VERSION

AC_OUTPUT([Makefile])
