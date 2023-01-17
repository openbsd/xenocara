
# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([libXpm], [3.5.14],
        [https://gitlab.freedesktop.org/xorg/lib/libxpm/-/issues], [libXpm])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-xz])

# Initialize libtool
AC_PROG_LIBTOOL

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Checks for library functions
AC_CHECK_FUNCS([strlcat])
AC_CHECK_FUNC([fork],[], AC_DEFINE(NO_ZPIPE))

# Obtain compiler/linker options for dependencies
PKG_CHECK_MODULES(XPM, xproto x11)
PKG_CHECK_MODULES(SXPM, [x11 xt xext xextproto xproto >= 7.0.17],
                  [build_sxpm=true], [build_sxpm=false])
AM_CONDITIONAL(BUILD_SXPM, test x$build_sxpm = xtrue)

# Internationalization & localization support
AC_SEARCH_LIBS([gettext], [intl], [USE_GETTEXT="yes"], [USE_GETTEXT="no"])
AC_MSG_CHECKING([where to install localized messages])
AC_ARG_WITH([localedir], AS_HELP_STRING([--with-localedir=<path>],
	[Path to install message files in (default: datadir/locale)]),
	[LOCALEDIR=${withval}], [LOCALEDIR=${datadir}/locale])
AX_DEFINE_DIR([LOCALEDIR], [LOCALEDIR], [Location of translated messages])
if test "x$LOCALEDIR" = "xno" -o "x$USE_GETTEXT" = "xno" ; then
	AC_MSG_RESULT([nowhere])
	USE_GETTEXT="no"
else
	AC_MSG_RESULT([$LOCALEDIR])
fi

if test "x$USE_GETTEXT" = "xyes" ; then
	AC_DEFINE([USE_GETTEXT], 1,
		  [Define to 1 if you want to use the gettext() function.])
fi
AM_CONDITIONAL(USE_GETTEXT, test "x$USE_GETTEXT" = "xyes")

dnl Helper macro to find absolute path to program and add a #define for it
AC_DEFUN([XPM_PATH_PROG],[
AC_PATH_PROG([$1], [$2], [])
AS_IF([test "x$$1" = "x"],
      [AC_MSG_ERROR([$2 not found, set $1 or use --disable-stat-zfile])])
AC_DEFINE_UNQUOTED([$1], ["$$1"], [Path to $2])
]) dnl End of AC_DEFUN([XPM_PATH_PROG]...

case $host_os in
        *mingw*)
                AC_DEFINE(NO_ZPIPE, 1, [Define to 1 to disable decompression via pipes])
        ;;
        *)
                XPM_PATH_PROG([XPM_PATH_COMPRESS], [compress])
                XPM_PATH_PROG([XPM_PATH_UNCOMPRESS], [uncompress])
                XPM_PATH_PROG([XPM_PATH_GZIP], [gzip])
                XPM_PATH_PROG([XPM_PATH_GUNZIP], [gunzip])
                AC_CHECK_FUNCS([closefrom close_range], [break])
        ;;
esac

AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 include/Makefile
                 man/Makefile
                 src/Makefile
                 sxpm/Makefile
                 cxpm/Makefile
                 xpm.pc])
AC_OUTPUT
