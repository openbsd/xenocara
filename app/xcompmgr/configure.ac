#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
AC_INIT([xcompmgr], [1.1.5], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg])
AC_CONFIG_SRCDIR([xcompmgr.c])
AM_INIT_AUTOMAKE([dist-bzip2 foreign])
AM_MAINTAINER_MODE

# Require xorg-macros 1.3 or later: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION],
	  [m4_fatal([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)

AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
XORG_DEFAULT_OPTIONS

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_TIME
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gettimeofday localtime_r])

PKG_CHECK_MODULES(XCOMPMGR, xcomposite xfixes xdamage xrender)
AC_SUBST(XCOMPMGR_CFLAGS)
AC_SUBST(XCOMPMGR_LIBS)

AC_OUTPUT([Makefile])
