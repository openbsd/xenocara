dnl
dnl Process this file with autoconf to create configure.

# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([makedepend], [1.0.8],
        [https://gitlab.freedesktop.org/xorg/util/makedepend/-/issues])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([makedepend-config.h])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-xz])

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
	  [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)

dnl Checks for programs.
XORG_DEFAULT_OPTIONS
XORG_WITH_LINT

dnl Checks for functions
AC_CHECK_FUNCS([rename fchmod])

dnl Use 64-bit file operations on 32-bit systems that support them
AC_SYS_LARGEFILE

dnl Check for pkg-config packages
PKG_CHECK_MODULES(X, [xproto >= 7.0.17])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
