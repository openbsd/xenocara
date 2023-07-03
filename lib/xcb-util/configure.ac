AC_PREREQ(2.59c)
AC_INIT([xcb-util],[0.4.1],
        [https://gitlab.freedesktop.org/xorg/lib/libxcb-util/-/issues],
        [xcb-util])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign dist-xz])

XCB_UTIL_COMMON([1.4], [1.6])

AC_CHECK_FUNCS_ONCE(vasprintf)

AC_CONFIG_FILES([Makefile
		src/Makefile
		xcb-atom.pc
		xcb-aux.pc
		xcb-event.pc
		xcb-util.pc])
AC_OUTPUT
