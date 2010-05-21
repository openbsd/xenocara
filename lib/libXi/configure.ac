dnl Process this file with autoconf to create configure.

AC_PREREQ([2.57])

AC_INIT(libXi, 1.3, [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg], libXi)
AM_INIT_AUTOMAKE([dist-bzip2 foreign])
AM_MAINTAINER_MODE

# Require xorg-macros: XORG_CWARNFLAGS, XORG_CHANGELOG
m4_ifndef([XORG_MACROS_VERSION], [AC_FATAL([must install xorg-macros 1.5 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.5)
XORG_DEFAULT_OPTIONS
XORG_WITH_XMLTO
XORG_WITH_ASCIIDOC
AM_CONFIG_HEADER(src/config.h)

# Check for progs
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_SED

# Checks for pkg-config packages
PKG_CHECK_MODULES(XI, [xproto >= 7.0.13] [x11 >= 1.2.99.1] [xextproto >= 7.0.3] [xext >= 1.0.99.1] [inputproto >= 1.9.99.902])
XI_CFLAGS="$CWARNFLAGS $XI_CFLAGS"
AC_SUBST(XI_CFLAGS)
AC_SUBST(XI_LIBS)

# Check for xmlto and asciidoc for man page conversion
# (only needed by people building tarballs)
AM_CONDITIONAL([HAVE_DOCTOOLS], [test "x$XMLTO" != "x" && test "x$ASCIIDOC" != "x"])
if test "x$XMLTO" = "x" || test "x$ASCIIDOC" = "x"; then
	AC_MSG_WARN([xmlto or asciidoc not found - cannot create man pages without it])
fi

XORG_CHECK_MALLOC_ZERO

AC_OUTPUT([Makefile
	   src/Makefile
	   man/Makefile
           xi.pc])

