dnl  Copyright 2005 Red Hat, Inc.
dnl
dnl  Permission to use, copy, modify, distribute, and sell this software
dnl  and its documentation for any purpose is hereby granted without
dnl  fee, provided that the above copyright notice appear in all copies
dnl  and that both that copyright notice and this permission notice
dnl  appear in supporting documentation, and that the name of Red Hat
dnl  not be used in advertising or publicity pertaining to distribution
dnl  of the software without specific, written prior permission.  Red
dnl  Hat makes no representations about the suitability of this software
dnl  for any purpose.  It is provided "as is" without express or implied
dnl  warranty.
dnl
dnl  RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
dnl  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
dnl  NO EVENT SHALL RED HAT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
dnl  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
dnl  OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
dnl  NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
dnl  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
dnl
dnl Process this file with autoconf to create configure.

AC_PREREQ([2.60])
AC_INIT([font-arabic-misc], [1.0.4],
        [https://gitlab.freedesktop.org/xorg/font/arabic-misc/-/issues],
        [font-arabic-misc])
AM_INIT_AUTOMAKE([foreign dist-xz])

# Require xorg-macros 1.20 or later: XORG_DEFAULT_NOCODE_OPTIONS
m4_ifndef([XORG_MACROS_VERSION],
	  [m4_fatal([must install xorg-macros 1.20 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.20)
XORG_DEFAULT_NOCODE_OPTIONS

# Require X.Org's font util macros 1.4 or later for COMPRESS_FLAGS
m4_ifndef([XORG_FONT_MACROS_VERSION],
	  [m4_fatal([must install X.Org font-util 1.4 or later before running autoconf/autogen])])
XORG_FONT_MACROS_VERSION(1.4)

XORG_FONTDIR([misc])
XORG_FONT_BDF_UTILS

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
