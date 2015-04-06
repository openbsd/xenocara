AC_PREREQ(2.59c)
AC_INIT([xcb-util-image],[0.4.0],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xcb&component=Utils],
        [xcb-util-image])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

XCB_UTIL_COMMON([1.4], [1.6])

AC_CHECK_HEADERS([sys/shm.h])
AM_CONDITIONAL(HAVE_SHM, test x$ac_cv_header_sys_shm_h = xyes)
PKG_CHECK_MODULES(XCB_SHM, xcb-shm)
PKG_CHECK_MODULES(XPROTO, xproto >= 7.0.8)
PKG_CHECK_MODULES(XCB_UTIL, xcb-util)

AC_CONFIG_FILES([
	Makefile
	image/Makefile
	test/Makefile
	image/xcb-image.pc
])
AC_OUTPUT
