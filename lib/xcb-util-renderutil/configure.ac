AC_PREREQ(2.59c)
AC_INIT([xcb-util-renderutil],[0.3.10],
        [https://gitlab.freedesktop.org/xorg/lib/libxcb-render-util/-/issues],
        [xcb-util-renderutil])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign dist-xz])

XCB_UTIL_COMMON([1.4], [1.6])

PKG_CHECK_MODULES(XCB_RENDER, xcb-render)

AC_CONFIG_FILES([Makefile
	renderutil/Makefile
	renderutil/xcb-renderutil.pc])
AC_OUTPUT
