# Process this file with autoconf to produce a configure script

AC_PREREQ(2.57)
AC_INIT([xf86-video-radeonhd],
        1.2.3,
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg&component=Driver/radeonhd],
        xf86-video-radeonhd)

AC_CONFIG_SRCDIR([Makefile.am])
AM_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR(.)

AM_INIT_AUTOMAKE([dist-bzip2])

AM_MAINTAINER_MODE

# Program checks.
AC_DISABLE_STATIC
AC_PROG_CC
dnl Define SED var without requiring AC_PROG_SED (new in autoconf-2.59b)
m4_ifdef([AC_PROG_SED], [AC_PROG_SED], [AC_SUBST([SED], [${SED-sed}])])
CXX=no
F77=no
AC_PROG_LIBTOOL


AH_TOP([#ifndef _CONFIG_H])
AH_TOP([# define _CONFIG_H])
AH_TOP([# include "xorg-server.h"])
AH_BOTTOM([#endif])

AC_ARG_WITH(xorg-module-dir,
            AC_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

AC_ARG_ENABLE(exa,
              AC_HELP_STRING([--disable-exa],
	                     [Disable EXA support [[default enabled]]]),
              [EXA="$enableval"],
              [EXA=yes])

AC_ARG_ENABLE(dri,
              AC_HELP_STRING([--disable-dri],
	                     [Disable DRI support [[default enabled]]]),
              [DRI="$enableval"],
              [DRI=yes])

# Extensions checks.
m4_pattern_forbid([XORG_DRIVER_CHECK_EXT])dnl
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XV, videoproto)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)
XORG_DRIVER_CHECK_EXT(XSERVER_LIBPCIACCESS, pciaccess)
# @@@@ We don't do DGA any more

# pkg-config packages checks
# @@@@ need to check for the right server version once RandR 1.2 is in.
# @@@ do we really need fontsproto here?
PKG_CHECK_MODULES(XORG, [xorg-server xproto fontsproto $REQUIRED_MODULES])
sdkdir=$(pkg-config --variable=sdkdir xorg-server)

# Header files checks.
AC_HEADER_STDC

PKG_CHECK_MODULES(PCIUTILS, libpci,,
   [AC_MSG_WARN([libpci not registered in pkgconfig, will guess instead])
    test "x$PCIUTILS_CFLAGS$PCIUTILS_LIBS" = "x" && PCIUTILS_LIBS="-lpci -lz"
])

SAVED_CPPFLAGS="$CPPFLAGS"
SAVED_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS $PCIUTILS_CFLAGS"
CPPFLAGS="$CPPFLAGS $PCIUTILS_CFLAGS"
AC_CHECK_HEADER([pci/pci.h],
                [AC_DEFINE([HAVE_PCI_PCI_H], [1],
                           [Define to 1 if you have <pci/pci.h>.])
                 have_pci_pci_h=yes],
                [AC_MSG_WARN([Caution: Will not compile rhd_conntest without pciutils headers.])])
AM_CONDITIONAL([HAVE_PCI_PCI_H], [test "x$have_pci_pci_h" = "xyes"])

# Properly handle EXA.
CPPFLAGS="$SAVED_CPPFLAGS $XORG_CFLAGS"
CFLAGS="$SAVED_CFLAGS"

# basic exa check.
AC_MSG_CHECKING([whether to enable EXA support])
if test "x$EXA" = xyes; then
        AC_MSG_RESULT(yes)
        AC_CHECK_HEADER(exa.h,
                       [have_exa_h="yes"], [have_exa_h="no"])
else
        AC_MSG_RESULT(no)
fi

# exa version check.
if test "x$have_exa_h" = xyes; then
        AC_MSG_CHECKING([whether EXA version is at least 2.0.0])
        AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[
#include "exa.h"
#if EXA_VERSION_MAJOR < 2
#error OLD EXA!
#endif
                          ]])],
                          [USE_EXA=yes],
                          [USE_EXA=no])
        AC_MSG_RESULT($USE_EXA)

        if test "x$USE_EXA" = xyes; then
                AC_DEFINE(USE_EXA, 1, [Build support for Exa])
        fi
fi

AM_CONDITIONAL(USE_EXA, test "x$USE_EXA" = xyes)

# Handle DRI
CPPFLAGS="$SAVED_CPPFLAGS $XORG_CFLAGS"
CFLAGS="$SAVED_CFLAGS $XORG_CFLAGS"

# basic dri check.
USE_DRI=no
if test "x$DRI" = xyes; then
	USE_DRI="yes"
	AC_CHECK_DECL(XF86DRI,, [USE_DRI=no], [#include <xorg-server.h>])
	PKG_CHECK_MODULES(DRI, [libdrm >= 2.2 xf86driproto],, [USE_DRI=no])
fi
AC_MSG_CHECKING([whether to enable DRI support])
AC_MSG_RESULT($USE_DRI)
test "x$USE_DRI" = xyes && AC_DEFINE(USE_DRI, 1, [Build support for DRI])
AM_CONDITIONAL(USE_DRI, test "x$USE_DRI" = xyes)

AC_CHECK_FILE([${sdkdir}/damage.h],
	[AC_DEFINE(DAMAGE,1,[Use Damage extension])], [])

# check for xf86_ansic.h
CFLAGS="$SAVED_CFLAGS"
CPPFLAGS="$SAVED_CPPFLAGS"

AC_CHECK_HEADER([${sdkdir}/xf86_ansic.h],
		[AC_DEFINE([HAVE_XF86_ANSIC_H], [1],
			   [Define to 1 if you have <${sdkdir}/xf86_ansic.h>.])
		have_xf86_ansic_h=yes])
AM_CONDITIONAL([HAVE_XF86_ANSIC_H], [test "x$have_xf86_ansic_h" = "xyes"])

SAVED_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS $XORG_CFLAGS"

# Check whether MonRec has a reducedblanking member.
AC_CHECK_MEMBER([MonRec.reducedblanking],
         [MONREC_HAS_REDUCED=yes], [MONREC_HAS_REDUCED=no],
         [#include "${sdkdir}/xf86.h"])
if test "x$MONREC_HAS_REDUCED" = xyes; then
        AC_DEFINE(MONREC_HAS_REDUCED, 1, [MonRec has member reducedblanking])
fi

# Check whether MonRec has a maxPixClock member.
AC_CHECK_MEMBER([MonRec.maxPixClock],
         [MONREC_HAS_BANDWIDTH=yes], [MONREC_HAS_BANDWIDTH=no],
         [#include "${sdkdir}/xf86.h"])
if test "x$MONREC_HAS_BANDWIDTH" = xyes; then
        AC_DEFINE(MONREC_HAS_BANDWIDTH, 1, [MonRec has member maxPixClock])
fi

# Check whether xf86CrtcFuncsRec has a set_mode_major member.
AC_CHECK_MEMBER([xf86CrtcFuncsRec.set_mode_major],
         [XF86CRTCFUNCS_HAS_SETMODEMAJOR=yes], [XF86CRTCFUNCS_HAS_SETMODEMAJOR=no],
         [#include "${sdkdir}/xorg-server.h"
#include "${sdkdir}/xf86.h"
#include "${sdkdir}/xf86Crtc.h"])
if test "x$XF86CRTCFUNCS_HAS_SETMODEMAJOR" = xyes; then
        AC_DEFINE(XF86CRTCFUNCS_HAS_SETMODEMAJOR, 1, [xf86CrtcFuncsRec has member set_mode_major])
fi

CFLAGS="$SAVED_CFLAGS"

AC_ARG_ENABLE(atombios, AC_HELP_STRING([--disable-atombios],
                                  [Disable AtomBIOS support [[default=no]]]),
              [do_atombios="$enableval"],
              [do_atombios=yes])

AC_ARG_ENABLE(atombios-parser, AC_HELP_STRING([--disable-atombios-parser],
                                  [Disable AtomBIOS parser support [[default=no]]]),
              [do_atombios_parser="$enableval"],
              [do_atombios_parser=yes])

AC_CHECK_FILE([${srcdir}/src/AtomBios/includes/atombios.h],
              [have_atombios_h="yes"], [have_atombios_h="no"])

AC_CHECK_FILE([${srcdir}/src/AtomBios/includes/Decoder.h],
              [have_atombios_parser="yes"], [have_atombios_parser="no"])

AC_MSG_CHECKING([whether to include AtomBIOS])
if test "$have_atombios_h" = yes -a \
	"$do_atombios" = yes; then
	support_atombios="yes"
        AC_DEFINE(ATOM_BIOS, 1, [Add support for AtomBIOS tables])
else
	support_atombios="no"
fi

AC_MSG_RESULT([$support_atombios])

AC_MSG_CHECKING([whether to include AtomBIOS parser])
if test "$support_atombios" = yes -a \
	"$do_atombios_parser" = yes -a \
	"$have_atombios_parser" = yes; then
	support_atombios_parser="yes"
        AC_DEFINE(ATOM_BIOS_PARSER, 1, [Add support for AtomBIOS code])
else
	support_atombios_parser="no"
fi

AC_MSG_RESULT([$support_atombios_parser])

AM_CONDITIONAL(ATOM_BIOS, test x$support_atombios = xyes)

AM_CONDITIONAL(ATOM_BIOS_PARSER, test x$support_atombios_parser = xyes)

# libraries checks.
AC_CHECK_LIB([z], [gzopen], [have_zlib=yes], [have_zlib=no])
AM_CONDITIONAL([HAVE_ZLIB], [test "x$have_zlib" = "xyes"])

# compiler flags

dnl AC_PROG_CC_FLAG(flag,action-if-found,action-if-not-found)
AC_DEFUN([AC_PROG_CC_FLAG],[
  AC_LANG_CONFTEST([AC_LANG_PROGRAM([[]])])
  AC_MSG_CHECKING([whether $CC supports $1])
  if $CC $1 -c -o /dev/null conftest.c >/dev/null 2>&1 ; then
    AC_MSG_RESULT([yes])
    $2
  else
    AC_MSG_RESULT([no])
    $3
  fi
])

# Extra Warnings
# Due to xorg/os.h: -Wno-variadic-macros
# For testing: make CFLAGS="-O2 -Wshadow -Wunreachable-code -fno-inline-functions"
WARN_CFLAGS=""
for w in \
  -Wall \
  -Wextra -Wno-unused-parameter \
  -Werror-implicit-function-declaration \
  -Wstrict-aliasing -Wstrict-overflow \
  -Wpointer-arith -Woverlength-strings -Wvolatile-register-var \
  -Winit-self -Wbad-function-cast \
  -Wstrict-prototypes -Wold-style-definition \
  -Wnested-externs -Wdisabled-optimization ;
do
  AC_PROG_CC_FLAG($w,[WARN_CFLAGS="$WARN_CFLAGS $w"],)
done

# Due to AtomBIOS spitting out too much with -pedantic
PEDANTIC_CFLAGS=""
for w in -pedantic -Wno-long-long -Wno-variadic-macros ; do
  AC_PROG_CC_FLAG($w,[PEDANTIC_CFLAGS="$PEDANTIC_CFLAGS $w"],)
done

# AtomBios special flags
ATOMBIOS_CFLAGS=""
for w in -std=c99 -Wno-unknown-pragmas ; do
  AC_PROG_CC_FLAG($w,[ATOMBIOS_CFLAGS="$ATOMBIOS_CFLAGS $w"],)
done

# Optimizations
case "x$CFLAGS" in
dnl Automake needs extra escaping
  [*-O[2-9]*)]
    AC_MSG_NOTICE([Doing optimized build, checking for additional options])
    for w in -finline-functions ; do
      AC_PROG_CC_FLAG($w,[CFLAGS="$CFLAGS -finline-functions"],)
    done
    ;;
esac

# Substitute

AC_SUBST([PCIUTILS_CFLAGS])
AC_SUBST([PCIUTILS_LIBS])
AC_SUBST([XORG_CFLAGS])
AC_SUBST([DRI_CFLAGS])
AC_SUBST([WARN_CFLAGS])
AC_SUBST([PEDANTIC_CFLAGS])
AC_SUBST([ATOMBIOS_CFLAGS])
AC_SUBST([RANDR_VERSION],[`pkg-config --modversion randrproto`])
AC_SUBST([moduledir])

DRIVER_NAME=radeonhd
AC_SUBST([DRIVER_NAME])

m4_pattern_forbid([XORG_MANPAGE_SECTIONS])dnl
XORG_MANPAGE_SECTIONS
m4_pattern_forbid([XORG_RELEASE_VERSION])dnl
XORG_RELEASE_VERSION

dnl These are also defined in XORG_RELEASE_VERSION since 2006-11-30.
dnl So we check whether XORG_RELEASE_VERSION has already done this
dnl for us.
if test "x$PVM$PVP" = "x"; then
AC_DEFINE_UNQUOTED([PACKAGE_VERSION_MAJOR],
		[$(echo $PACKAGE_VERSION | cut -d . -f 1)],
		[Major Version])
AC_DEFINE_UNQUOTED([PACKAGE_VERSION_MINOR],
		[$(echo $PACKAGE_VERSION | cut -d . -f 2)],
		[Minor Version])
AC_DEFINE_UNQUOTED([PACKAGE_VERSION_PATCHLEVEL],
		[$(echo $PACKAGE_VERSION | cut -d . -f 3)],
		[Patchlevel Version])
fi

AC_OUTPUT([
	Makefile
	man/Makefile
	src/Makefile
	utils/conntest/Makefile
])
if test "x$USE_DRI" != xyes ; then
  echo ""
  echo "NOTE: DRI support is disabled"
  echo ""
fi
