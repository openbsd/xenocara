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
AC_PREREQ([2.70])
AC_INIT([xauth],
	[1.1.4],
	[https://gitlab.freedesktop.org/xorg/app/xauth/-/issues],
	[xauth])
AC_CONFIG_SRCDIR([xauth.c])
AC_CONFIG_HEADERS([config.h])

# Set common system defines for POSIX extensions, such as _GNU_SOURCE
# Must be called before any macros that run the compiler (like
# AC_PROG_LIBTOOL) to avoid autoconf errors.
AC_USE_SYSTEM_EXTENSIONS

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-xz])

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Checks for header files.
AC_CHECK_HEADERS([net/errno.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_SYS_LARGEFILE

# Checks for pkg-config packages
PKG_CHECK_MODULES(XAUTH, x11 xau xext xmuu xproto >= 7.0.17)

# Issue an error if xtrans.m4 was not found and XTRANS_CONNECTION_FLAGS macro
# was not expanded, since xauth with no address support is rather broken.
#
# If you're seeing an error here, be sure you installed the lib/xtrans module
# first and if it's not in the default location, that you set the ACLOCAL
# environment variable to find it, such as:
#       ACLOCAL="aclocal -I ${PREFIX}/share/aclocal"
m4_pattern_forbid([^XTRANS_CONNECTION_FLAGS$])

# Transport selection macro from xtrans.m4
# Needed to set *CONN defines for gethost.c & parsedpy.c
XTRANS_CONNECTION_FLAGS

# Checks for library functions.
AC_CHECK_FUNCS([getaddrinfo inet_ntop reallocarray strlcpy])

XORG_WITH_LINT

AC_CONFIG_FILES([
	Makefile
	tests/Makefile
	man/Makefile])
AC_OUTPUT
