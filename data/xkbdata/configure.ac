# $XdotOrg: $

AC_PREREQ([2.57])
AC_INIT(xkbdata, [1.0.1], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg], xkbdata)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

AM_CONFIG_HEADER(config.h)

AC_PATH_PROG([XKBCOMP], [xkbcomp], [not_found])
if test x$XKBCOMP = xnot_found ; then
	AC_ERROR([xkbcomp is required to install the xkb data files])
fi

XORG_RELEASE_VERSION

AC_OUTPUT([
	Makefile
	semantics/Makefile
	keycodes/Makefile
	keycodes/sgi/Makefile
	keycodes/digital/Makefile
	keymap/Makefile
	keymap/sun/Makefile
	keymap/digital/Makefile
	keymap/sgi/Makefile
	geometry/Makefile
	geometry/digital/Makefile
	geometry/sgi/Makefile
	geometry/ibm/Makefile
	compat/Makefile
	rules/Makefile
	symbols/Makefile
	symbols/pc/Makefile
	symbols/nec/Makefile
	symbols/xfree68/Makefile
	symbols/sony/Makefile
	symbols/sgi/Makefile
	symbols/macintosh/Makefile
	symbols/sun/Makefile
	symbols/fujitsu/Makefile
	symbols/digital/Makefile
	symbols/hp/Makefile
	torture/Makefile
	types/Makefile
])
