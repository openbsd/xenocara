
# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([libXxf86dga], [1.1.6],
        [https://gitlab.freedesktop.org/xorg/lib/libxxf86dga/-/issues],
        [libXxf86dga])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS(src/config.h)

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-xz])

# Initialize libtool
AC_PROG_LIBTOOL

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS
XORG_CHECK_MALLOC_ZERO

# Obtain compiler/linker options for dependencies
PKG_CHECK_MODULES(XXF86DGA, xproto [x11 >= 1.6] xextproto xext [xf86dgaproto >= 2.0.99.2])

AC_CONFIG_FILES([Makefile
		src/Makefile
		man/Makefile
		xxf86dga.pc])
AC_OUTPUT
