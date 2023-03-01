dnl Process this file with autoconf to create configure.

# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([xbitmaps], [1.1.3],
        [https://gitlab.freedesktop.org/xorg/data/bitmaps/-/issues], [xbitmaps])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-xz])

# Require xorg-macros 1.20 or later: XORG_DEFAULT_NOCODE_OPTIONS
m4_ifndef([XORG_MACROS_VERSION],
	  [m4_fatal([must install xorg-macros 1.20 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.20)
XORG_DEFAULT_NOCODE_OPTIONS

AC_CONFIG_FILES([Makefile
		xbitmaps.pc
])
AC_OUTPUT
