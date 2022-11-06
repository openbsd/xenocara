AC_PREREQ(2.59c)
AC_INIT([xcb-util-keysyms], [0.4.1],
        [https://gitlab.freedesktop.org/xorg/lib/libxcb-keysyms/-/issues])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign dist-xz])

XCB_UTIL_COMMON([1.4], [1.6])

PKG_CHECK_MODULES(XPROTO, xproto >= 7.0.8)

AC_CONFIG_FILES([Makefile
	keysyms/Makefile keysyms/xcb-keysyms.pc
	])
AC_OUTPUT
