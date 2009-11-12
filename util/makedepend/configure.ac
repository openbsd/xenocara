AC_PREREQ([2.57])
AC_INIT([makedepend], [1.0.2], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg])
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_CONFIG_HEADER(makedepend-config.h)

# Require xorg-macros 1.3 or later: XORG_DEFAULT_OPTIONS, XORG_WITH_LINT
m4_ifndef([XORG_MACROS_VERSION],
	[m4_fatal([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)

dnl Checks for programs.
AC_PROG_CC
XORG_DEFAULT_OPTIONS
XORG_WITH_LINT

dnl Replacement for Imake SIGNAL_DEFINES
AC_TYPE_SIGNAL

dnl Checks for functions
AC_CHECK_FUNCS([rename fchmod])

dnl Use 64-bit file operations on 32-bit systems that support them
AC_SYS_LARGEFILE

dnl Check for pkg-config packages
PKG_CHECK_MODULES(X, [xproto])
AC_SUBST(X_CFLAGS)

AC_OUTPUT([Makefile])
