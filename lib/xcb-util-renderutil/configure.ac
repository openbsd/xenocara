AC_PREREQ(2.59c)
AC_INIT([xcb-util-renderutil],0.3.8,[xcb@lists.freedesktop.org])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

XCB_UTIL_COMMON([1.4], [1.6])

PKG_CHECK_MODULES(XCB_RENDER, xcb-render)

AC_OUTPUT([Makefile
	renderutil/Makefile renderutil/xcb-renderutil.pc
	xcb_util_intro
	])
