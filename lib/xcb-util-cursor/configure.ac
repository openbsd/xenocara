dnl XCB_UTIL_M4_WITH_INCLUDE_PATH requires Autoconf >= 2.62
AC_PREREQ(2.62)
AC_INIT([xcb-util-cursor],[0.1.5],
        [https://gitlab.freedesktop.org/xorg/lib/libxcb-cursor/-/issues])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])

# Set common system defines for POSIX extensions, such as _GNU_SOURCE
# Must be called before any macros that run the compiler (like AC_PROG_LIBTOOL)
# to avoid autoconf errors.
AC_USE_SYSTEM_EXTENSIONS

AM_INIT_AUTOMAKE([foreign dist-xz])

AC_TYPE_SSIZE_T

XCB_UTIL_M4_WITH_INCLUDE_PATH
XCB_UTIL_COMMON([1.4], [1.6])

AM_MISSING_PROG([GPERF], [gperf])

AC_CHECK_HEADERS([endian.h sys/endian.h sys/byteorder.h libkern/OSByteOrder.h], [break])
AC_CHECK_FUNCS([le32toh])

PKG_CHECK_MODULES(XCB_RENDER, xcb-render)
PKG_CHECK_MODULES(XCB_RENDERUTIL, xcb-renderutil)
PKG_CHECK_MODULES(XCB_IMAGE, xcb-image)

DEF_CURSORPATH="~/.icons:/usr/share/icons:/usr/share/pixmaps:/usr/X11R6/lib/X11/icons"
AC_ARG_WITH(cursorpath,
	AS_HELP_STRING([--with-cursorpath=<paths>],
		[Set default search path for cursors]),
	[XCURSORPATH="$withval"],
	[XCURSORPATH="$DEF_CURSORPATH"])
AC_DEFINE_UNQUOTED([XCURSORPATH], ["$XCURSORPATH"])

AC_CONFIG_FILES([Makefile
	cursor/Makefile cursor/xcb-cursor.pc
	xcb_util_intro
	])
AC_OUTPUT
