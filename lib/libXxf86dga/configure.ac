AC_PREREQ([2.57])

AC_INIT(libXxf86dga, 1.1.1, [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg], libXxf86dga)
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Require xorg-macros: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION], [AC_FATAL([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)
AM_CONFIG_HEADER(src/config.h)

# Check for progs
AC_PROG_CC
AC_PROG_LIBTOOL
XORG_DEFAULT_OPTIONS

# Checks for pkg-config packages
PKG_CHECK_MODULES(XXF86DGA, xproto x11 xextproto xext [xf86dgaproto >= 2.0.99.2])
XXF86DGA_CFLAGS="$CWARNFLAGS $XXF86DGA_CFLAGS"

XORG_CHECK_MALLOC_ZERO

AC_OUTPUT([Makefile
	   src/Makefile
	   man/Makefile
	   xxf86dga.pc])
