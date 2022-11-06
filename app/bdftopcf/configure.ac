dnl  Copyright 2005 Red Hat, Inc.
dnl 
dnl  Permission to use, copy, modify, distribute, and sell this software
dnl  and its documentation for any purpose is hereby granted without fee,
dnl  provided that the above copyright notice appear in all copies and
dnl  that both that copyright notice and this permission notice appear in
dnl  supporting documentation, and that the name of Red Hat not be used in
dnl  advertising or publicity pertaining to distribution of the software
dnl  without specific, written prior permission.  Red Hat makes no
dnl  representations about the suitability of this software for any
dnl  purpose.  It is provided "as is" without express or implied warranty.
dnl 
dnl  RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
dnl  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
dnl  NO EVENT SHALL RED HAT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
dnl  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
dnl  OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
dnl  OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE
dnl  USE OR PERFORMANCE OF THIS SOFTWARE.
dnl
dnl Process this file with autoconf to create configure.

# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([bdftopcf], [1.1.1],
        [https://gitlab.freedesktop.org/xorg/util/bdftopcf/-/issues], [bdftopcf])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-xz])

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

XORG_WITH_LINT

PKG_CHECK_MODULES(BDFTOPCF, [xproto >= 7.0.22 fontsproto >= 2.1.3])

PKG_CHECK_MODULES(BDFTOPCF, [xproto fontsproto])

AC_CONFIG_FILES([
	Makefile
	man/Makefile])
AC_OUTPUT
