dnl XCB_UTIL_M4_WITH_INCLUDE_PATH requires Autoconf >= 2.62
AC_PREREQ(2.62)
AC_INIT([xcb-util-xrm],1.0,[xcb@lists.freedesktop.org])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])

# Set common system defines for POSIX extensions, such as _GNU_SOURCE
# Must be called before any macros that run the compiler (like AC_PROG_LIBTOOL)
# to avoid autoconf errors.
AC_USE_SYSTEM_EXTENSIONS

AM_INIT_AUTOMAKE([foreign dist-bzip2 subdir-objects])
AM_MAINTAINER_MODE

XCB_UTIL_M4_WITH_INCLUDE_PATH
XCB_UTIL_COMMON([1.4], [1.6])

PKG_CHECK_MODULES(XCB_AUX, xcb-aux)

AC_OUTPUT([Makefile
	xcb-xrm.pc
	xcb_xrm_intro
	])
