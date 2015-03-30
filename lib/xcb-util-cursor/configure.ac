dnl XCB_UTIL_M4_WITH_INCLUDE_PATH requires Autoconf >= 2.62
AC_PREREQ(2.62)
AC_INIT([xcb-util-cursor],0.1.2,[xcb@lists.freedesktop.org])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])

# Set common system defines for POSIX extensions, such as _GNU_SOURCE
# Must be called before any macros that run the compiler (like AC_PROG_LIBTOOL)
# to avoid autoconf errors.
AC_USE_SYSTEM_EXTENSIONS

AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

AC_TYPE_SSIZE_T

XCB_UTIL_M4_WITH_INCLUDE_PATH
XCB_UTIL_COMMON([1.4], [1.6])

AM_MISSING_PROG([GPERF], [gperf])

AC_CHECK_HEADERS([endian.h sys/endian.h sys/byteorder.h libkern/OSByteOrder.h], [break])
AC_CHECK_FUNCS([le32toh])

PKG_CHECK_MODULES(XCB_RENDER, xcb-render)
PKG_CHECK_MODULES(XCB_RENDERUTIL, xcb-renderutil)
PKG_CHECK_MODULES(XCB_IMAGE, xcb-image)

AC_OUTPUT([Makefile
	cursor/Makefile cursor/xcb-cursor.pc
	xcb_util_intro
	])
