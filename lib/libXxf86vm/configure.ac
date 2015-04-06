
# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([libXxf86vm], [1.1.4],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg], [libXxf86vm])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS(src/config.h)

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])

# Initialize libtool
AC_PROG_LIBTOOL

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS
XORG_CHECK_MALLOC_ZERO

# Obtain compiler/linker options for depedencies
PKG_CHECK_MODULES(XXF86VM, xproto [x11 >= 1.6] xextproto xext [xf86vidmodeproto >= 2.2.99.1])

AC_CONFIG_FILES([Makefile
		src/Makefile
		man/Makefile
		xxf86vm.pc])
AC_OUTPUT
