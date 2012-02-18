#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Initialize Autoconf
AC_PREREQ(2.60)
AC_INIT([xcompmgr], [1.1.6],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg])
AC_CONFIG_SRCDIR([xcompmgr.c])
AC_CONFIG_HEADERS([config.h])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Require xorg-macros 1.3 or later: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION],
	  [m4_fatal([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)
XORG_DEFAULT_OPTIONS

PKG_CHECK_MODULES(XCOMPMGR, xcomposite xfixes xdamage xrender xext)

AC_OUTPUT([Makefile])
