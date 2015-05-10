#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Initialize Autoconf
AC_PREREQ(2.60)
AC_INIT([xcompmgr], [1.1.7],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg])
AC_CONFIG_SRCDIR([xcompmgr.c])
AC_CONFIG_HEADERS([config.h])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
	  [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

PKG_CHECK_MODULES(XCOMPMGR, xcomposite xfixes xdamage xrender xext)

AC_CONFIG_FILES([
	Makefile
	man/Makefile])
AC_OUTPUT
