dnl
dnl Process this file with autoconf to create configure.

# Initialize Autoconf
AC_PREREQ([2.70])
AC_INIT([makedepend], [1.0.9],
        [https://gitlab.freedesktop.org/xorg/util/makedepend/-/issues])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([makedepend-config.h])
# Set common system defines for POSIX extensions, such as _GNU_SOURCE
# Must be called before any macros that run the compiler (like AC_PROG_LIBTOOL)
# to avoid autoconf errors.
AC_USE_SYSTEM_EXTENSIONS

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
AC_CHECK_FUNCS([rename fchmod reallocarray])

dnl Use 64-bit file operations on 32-bit systems that support them
AC_SYS_LARGEFILE

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
