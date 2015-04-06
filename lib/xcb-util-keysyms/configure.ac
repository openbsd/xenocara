AC_PREREQ(2.59c)
AC_INIT([xcb-util-keysyms],0.4.0,[xcb@lists.freedesktop.org])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

XCB_UTIL_COMMON([1.4], [1.6])

PKG_CHECK_MODULES(XPROTO, xproto >= 7.0.8)

AC_OUTPUT([Makefile
	keysyms/Makefile keysyms/xcb-keysyms.pc
	])
