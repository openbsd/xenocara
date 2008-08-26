#  Copyright 2005 Adam Jackson.
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  on the rights to use, copy, modify, merge, publish, distribute, sub
#  license, and/or sell copies of the Software, and to permit persons to whom
#  the Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice (including the next
#  paragraph) shall be included in all copies or substantial portions of the
#  Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
#  ADAM JACKSON BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
#  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
#  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# Process this file with autoconf to produce a configure script

AC_PREREQ(2.57)
AC_INIT([xf86-video-intel],
        2.3.2,
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        xf86-video-intel)

AC_CONFIG_SRCDIR([Makefile.am])
AM_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR(.)

AM_INIT_AUTOMAKE([dist-bzip2])

AM_MAINTAINER_MODE

# Checks for programs.
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_PROG_CC
AM_PROG_CC_C_O

AC_CHECK_PROG(gen4asm, [intel-gen4asm], yes, no)
AM_CONDITIONAL(HAVE_GEN4ASM, test x$gen4asm = xyes)

AC_CHECK_HEADERS(sys/mman.h)
AC_CHECK_FUNCS(mprotect)

AH_TOP([#include "xorg-server.h"])

AC_ARG_WITH(xorg-module-dir,
            AC_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

AC_ARG_ENABLE(dri, AC_HELP_STRING([--disable-dri],
                                  [Disable DRI support [[default=auto]]]),
              [DRI="$enableval"],
              [DRI=auto])

AC_ARG_ENABLE(video-debug, AC_HELP_STRING([--enable-video-debug],
                                  [Enable video debugging support [[default=no]]]),
              [VIDEO_DEBUG="$enableval"],
              [VIDEO_DEBUG=no])

AC_ARG_WITH(xserver-source,AC_HELP_STRING([--with-xserver-source=XSERVER_SOURCE],
					  [Path to X server source tree]),
			   [ XSERVER_SOURCE="$withval" ],
			   [ XSERVER_SOURCE="" ])

AC_ARG_ENABLE(xvmc, AC_HELP_STRING([--disable-xvmc],
                                  [Disable XvMC support [[default=auto]]]),
              [XVMC="$enableval"],
              [XVMC=auto])

# Checks for extensions
XORG_DRIVER_CHECK_EXT(XINERAMA, xineramaproto)
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XF86DRI, xextproto x11)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, [xorg-server xproto fontsproto $REQUIRED_MODULES])
sdkdir=$(pkg-config --variable=sdkdir xorg-server)

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC

AC_MSG_CHECKING([whether to include DRI support])
if test x$DRI = xauto; then
        AC_CHECK_FILE([${sdkdir}/dri.h],
                      [have_dri_h="yes"], [have_dri_h="no"])
        AC_CHECK_FILE([${sdkdir}/sarea.h],
                      [have_sarea_h="yes"], [have_sarea_h="no"])
        AC_CHECK_FILE([${sdkdir}/dristruct.h],
                      [have_dristruct_h="yes"], [have_dristruct_h="no"])
	AC_CHECK_FILE([${sdkdir}/damage.h],
                      [have_damage_h="yes"], [have_damage_h="no"])

        if test "$have_dri_h" = yes -a \
                "$have_sarea_h" = yes -a \
                "$have_dristruct_h" = yes; then
                DRI="yes"
        else
                DRI="no"
        fi
fi
AC_MSG_RESULT([$DRI])

save_CFLAGS="$CFLAGS"
CFLAGS="$XORG_CFLAGS"
AC_CHECK_HEADER(xf86Modes.h,[XMODES=yes],[XMODES=no],[#include "xorg-server.h"])
AC_CHECK_DECL(XSERVER_LIBPCIACCESS,
	      [XSERVER_LIBPCIACCESS=yes],[XSERVER_LIBPCIACCESS=no],
	      [#include "xorg-server.h"])
CFLAGS="$save_CFLAGS"

if test x$XSERVER_LIBPCIACCESS = xyes; then
	PKG_CHECK_MODULES([PCIACCESS], [pciaccess >= 0.10])
else
	PKG_CHECK_MODULES([PCIACCESS], [pciaccess >= 0.10],
				       have_libpciaccess=yes,
				       have_libpciaccess=no)
fi
AM_CONDITIONAL(XSERVER_LIBPCIACCESS, test "x$XSERVER_LIBPCIACCESS" = xyes)
AM_CONDITIONAL(LIBPCIACCESS,
	test "x$XSERVER_LIBPCIACCESS" = xyes -o "x$have_libpciaccess" = xyes)
AM_CONDITIONAL(XMODES, test "x$XMODES" = xno)

if test "x$XSERVER_SOURCE" = x; then
	if test -d ../../xserver; then
		XSERVER_SOURCE="`cd ../../xserver && pwd`"
	fi
fi
	
if test -d "$XSERVER_SOURCE"; then
	case "$XSERVER_SOURCE" in
	/*)
		;;
	*)
		XSERVER_SOURCE="`cd $XSERVER_SOURCE && pwd`"
		;;
	esac
	if test -f $srcdir/src/modes/xf86Modes.h; then
		:
	else
		ln -sf $XSERVER_SOURCE/hw/xfree86/modes $srcdir/src/modes
	fi
	
	if test -f $srcdir/src/parser/xf86Parser.h; then
		:
	else
		ln -sf $XSERVER_SOURCE/hw/xfree86/parser $srcdir/src/parser
	fi
fi

if test "x$XMODES" = xyes; then
	AC_MSG_NOTICE([X server has new mode code])
	AC_DEFINE(XMODES, 1,[X server has built-in mode code])
	XMODES_CFLAGS=
else
	if test -f $srcdir/src/modes/xf86Modes.h -a -f $srcdir/src/parser/xf86Parser.h; then
		AC_MSG_NOTICE([X server is missing new mode code, using local copy])
	else
		AC_MSG_ERROR([Must have X server >= 1.3 source tree for mode setting code. Please specify --with-xserver-source])
	fi
	XMODES_CFLAGS='-DXF86_MODES_RENAME -I$(top_srcdir)/src -I$(top_srcdir)/src/modes -I$(top_srcdir)/src/parser'
fi

AC_SUBST([XMODES_CFLAGS])

SAVE_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $XORG_CFLAGS"

AC_CHECK_DECL(xf86RotateFreeShadow,
	      [AC_DEFINE(HAVE_FREE_SHADOW, 1, [have new FreeShadow API])],
	      [],
	      [#include <xorg-server.h>
	       #include <windowstr.h>
	       #include <xf86Crtc.h>])
	     
CPPFLAGS="$SAVE_CPPFLAGS"

dnl Use lots of warning flags with GCC

WARN_CFLAGS=""

if test "x$GCC" = "xyes"; then
	WARN_CFLAGS="-Wall -Wpointer-arith -Wstrict-prototypes \
	-Wmissing-prototypes -Wmissing-declarations \
	-Wnested-externs -fno-strict-aliasing"
fi

AM_CONDITIONAL(DRI, test x$DRI = xyes)
if test "$DRI" = yes; then
        PKG_CHECK_MODULES(DRI, [libdrm xf86driproto glproto])
        AC_DEFINE(XF86DRI,1,[Enable DRI driver support])
        AC_DEFINE(XF86DRI_DEVEL,1,[Enable developmental DRI driver support])
	PKG_CHECK_MODULES(DRI_MM, [libdrm >= 2.4.0],[DRI_MM=yes], [DRI_MM=no])
	if test "x$DRI_MM" = xyes; then
		AC_DEFINE(XF86DRI_MM,1,[Extended DRI memory management])
	fi
	if test "$have_damage_h" = yes; then
		AC_DEFINE(DAMAGE,1,[Use Damage extension])
	fi

	save_CFLAGS="$CFLAGS"
	CFLAGS="$XORG_CFLAGS $DRI_CFLAGS"
	AC_CHECK_TYPE(drm_i915_flip_t,
		      [AC_DEFINE(HAVE_I915_FLIP, 1,
			         [Have drm_i915_flip_t and related definitions])],
		      [], [
#include <inttypes.h>
#include <i915_drm.h>
])
	CFLAGS="$save_CFLAGS"
fi

AM_CONDITIONAL(VIDEO_DEBUG, test x$VIDEO_DEBUG = xyes)
if test "$VIDEO_DEBUG" = yes; then
	AC_DEFINE(VIDEO_DEBUG,1,[Enable debug support])
fi

if test "$XVMC" = auto; then
	XVMC="$DRI"
fi
if test "$XVMC" = yes && test "$DRI" = no; then
	AC_MSG_ERROR([XvMC can't be enabled without DRI])
fi
if test "$XVMC" = yes; then
	PKG_CHECK_MODULES(XVMCLIB, [xvmc], [XVMC=yes], [XVMC=no])
fi
AC_MSG_CHECKING([whether to include XvMC support])
AC_MSG_RESULT([$XVMC])
AM_CONDITIONAL(XVMC, test x$XVMC = xyes)
if test "$XVMC" = yes; then
	AC_DEFINE(ENABLE_XVMC,1,[Enable XvMC support])
    	AC_SUBST([XVMCLIB_CFLAGS])
fi


AC_SUBST([DRI_CFLAGS])
AC_SUBST([XORG_CFLAGS])
AC_SUBST([WARN_CFLAGS])
AC_SUBST([moduledir])

DRIVER_NAME=intel
AC_SUBST([DRIVER_NAME])

m4_ifndef([XORG_MACROS_VERSION], [AC_FATAL([must install xorg-macros 1.1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.1.3)
XORG_MANPAGE_SECTIONS
XORG_RELEASE_VERSION

XORG_CHECK_LINUXDOC

AC_OUTPUT([
	Makefile
	src/Makefile
	src/xvmc/Makefile
	src/bios_reader/Makefile
	src/ch7017/Makefile
	src/ch7xxx/Makefile
	src/ivch/Makefile
	src/reg_dumper/Makefile
	src/sil164/Makefile
	src/tfp410/Makefile
	man/Makefile
])
