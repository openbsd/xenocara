AC_PREREQ(2.59c)
AC_INIT([xcb-util],0.3.9,[xcb@lists.freedesktop.org])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

XCB_UTIL_COMMON([1.4], [1.6])

AC_CHECK_FUNCS_ONCE(vasprintf)
AC_TYPE_SSIZE_T

PKG_CHECK_MODULES(XPROTO, xproto >= 7.0.8)

AC_CONFIG_FILES([Makefile
		src/Makefile
		xcb-atom.pc
		xcb-aux.pc
		xcb-event.pc
		xcb-util.pc
		xcb_util_intro])
AC_OUTPUT
