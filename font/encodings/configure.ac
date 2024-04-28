AC_PREREQ([2.60])
AC_INIT([encodings], [1.1.0],
        [https://gitlab.freedesktop.org/xorg/font/encodings/-/issues])
AM_INIT_AUTOMAKE([foreign dist-xz])

# Require xorg-macros 1.20 or later: XORG_DEFAULT_NOCODE_OPTIONS
m4_ifndef([XORG_MACROS_VERSION],
	  [m4_fatal([must install xorg-macros 1.20 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.20)
XORG_DEFAULT_NOCODE_OPTIONS

# Require X.Org's font util macros 1.2 or later
m4_ifndef([XORG_FONT_MACROS_VERSION],
	  [m4_fatal([must install X.Org font-util 1.2 or later before running autoconf/autogen])])
XORG_FONT_MACROS_VERSION(1.2)

# mkfontscale
XORG_FONT_REQUIRED_PROG(MKFONTSCALE, mkfontscale)

# Choose whether or not to compress encodings - default is to compress all

AC_MSG_CHECKING([if small encodings should be compressed])
AC_ARG_ENABLE(gzip-small-encodings,
	AS_HELP_STRING([--disable-gzip-small-encodings],
			[Disable compression of small encoding files]),
	[GZIP_SMALL="$enableval"], [GZIP_SMALL="yes"]) 
AM_CONDITIONAL(GZIP_SMALL, [test x$GZIP_SMALL = xyes])
AC_MSG_RESULT([${GZIP_SMALL}])

AC_MSG_CHECKING([if large encodings should be compressed])
AC_ARG_ENABLE(gzip-large-encodings,
	AS_HELP_STRING([--disable-gzip-large-encodings],
			[Disable compression of large encoding files]),
	[GZIP_LARGE="$enableval"], [GZIP_LARGE="yes"]) 
AM_CONDITIONAL(GZIP_LARGE, [test x$GZIP_LARGE = xyes])
AC_MSG_RESULT([${GZIP_LARGE}])

# gzip

if test x$GZIP_SMALL = xyes -o x$GZIP_LARGE = xyes ; then
    AC_PATH_PROG(GZIP, gzip)
    if [[ -z $GZIP ]]; then
	AC_MSG_ERROR([gzip is needed to compress font encodings])
    fi
    COMPRESS_FLAGS="${COMPRESS_FLAGS--n}"
    AC_SUBST([COMPRESS_FLAGS])
fi

XORG_FONTSUBDIR([ENCODINGSDIR], [encodingsdir], [encodings])

AC_CONFIG_FILES([Makefile large/Makefile])
AC_OUTPUT
