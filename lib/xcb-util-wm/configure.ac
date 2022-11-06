dnl XCB_UTIL_M4_WITH_INCLUDE_PATH requires Autoconf >= 2.62
AC_PREREQ(2.62)
AC_INIT([xcb-util-wm], [0.4.2],
        [https://gitlab.freedesktop.org/xorg/lib/libxcb-wm/-/issues],
        [xcb-util-wm])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign dist-xz])

XCB_UTIL_M4_WITH_INCLUDE_PATH
XCB_UTIL_COMMON([1.4], [1.6])

AC_CONFIG_FILES([Makefile
	icccm/Makefile
	icccm/xcb-icccm.pc
	ewmh/Makefile
	ewmh/xcb-ewmh.pc])
AC_OUTPUT
