dnl Process this file with autoconf to create configure.

AC_PREREQ([2.60])

AC_INIT(libXi, 1.4.0, [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg], libXi)
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Require xorg-macros minimum of 1.10 for DocBook XML documentation
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.10 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.10)
XORG_DEFAULT_OPTIONS
XORG_ENABLE_SPECS
XORG_WITH_XMLTO(0.0.20)
XORG_WITH_FOP
XORG_CHECK_SGML_DOCTOOLS(1.5)
XORG_WITH_ASCIIDOC(8.4.5)
AM_CONFIG_HEADER(src/config.h)

# Check for progs
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_SED

# Checks for pkg-config packages
PKG_CHECK_MODULES(XI, [xproto >= 7.0.13] [x11 >= 1.2.99.1] [xextproto >= 7.0.3] [xext >= 1.0.99.1] [inputproto >= 1.9.99.902])

# Check for xmlto and asciidoc for man page conversion
# (only needed by people building tarballs)
if test "$have_xmlto" = yes && test "$have_asciidoc" = yes; then
	have_doctools=yes
else
	have_doctools=no
fi
AM_CONDITIONAL([HAVE_DOCTOOLS], [test $have_doctools = yes])
if test $have_doctools = no; then
	AC_MSG_WARN([xmlto or asciidoc not found - cannot create man pages without it])
fi

# Determine if the source for man pages is available
# It may already be present (tarball) or can be generated using doctools
AM_CONDITIONAL([INSTALL_MANPAGES],
	[test -f "$srcdir/man/XAllowDeviceEvents.man" || \
	  test $have_doctools = yes])

XORG_CHECK_MALLOC_ZERO

AC_OUTPUT([Makefile
           src/Makefile
           man/Makefile
           doc/Makefile
           specs/Makefile
           xi.pc])

