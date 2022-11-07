
# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([libXmu], [1.1.4],
	[https://gitlab.freedesktop.org/xorg/lib/libxmu/-/issues], [libXmu])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

# Set common system defines for POSIX extensions, such as _GNU_SOURCE
# Must be called before any macros that run the compiler (like AC_PROG_LIBTOOL)
# to avoid autoconf errors.
AC_USE_SYSTEM_EXTENSIONS

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-xz])

# Initialize libtool
AC_PROG_LIBTOOL

# Require xorg-macros minimum of 1.12 for DocBook external references
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.12 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.12)
XORG_DEFAULT_OPTIONS
XORG_ENABLE_DOCS
XORG_WITH_XMLTO(0.0.22)
XORG_WITH_FOP
XORG_WITH_XSLTPROC
XORG_CHECK_SGML_DOCTOOLS(1.8)

# Obtain compiler/linker options for dependencies
PKG_CHECK_MODULES(XMU, xt xext x11 xextproto)
PKG_CHECK_MODULES(XMUU, x11)

# Checks for library functions.
AC_CHECK_FUNCS([uname])
AC_REPLACE_FUNCS([reallocarray])

# Allow checking code with lint, sparse, etc.
XORG_WITH_LINT
XORG_LINT_LIBRARY([Xmu])
LINTLIBUU=`echo $LINTLIB | sed s/Xmu/Xmuu/`
AC_SUBST(LINTLIBUU)

AC_CONFIG_FILES([Makefile
		doc/Makefile
		include/Makefile
		src/Makefile
		test/Makefile
		xmu.pc
		xmuu.pc])
AC_OUTPUT
