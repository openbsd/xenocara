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

# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([xf86-video-intel],
        [2.20.19],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        [xf86-video-intel])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR(.)

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen.
  Hint: either install from source, git://anongit.freedesktop.org/xorg/util/macros or,
  depending on you distribution, try package 'xutils-dev' or 'xorg-x11-util-macros'])])

XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Require X.Org server macros (i.e. XORG_DRIVER_CHECK_EXT) to check for required modules 
m4_ifndef([XORG_DRIVER_CHECK_EXT],
          [m4_fatal([must install xorg-server macros before running autoconf/autogen.
  Hint: either install from source, git://anongit.freedesktop.org/xorg/xserver or,
  depending on your distribution, try package 'xserver-xorg-dev' or 'xorg-x11-server-devel'])])

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_SYS_LARGEFILE

# Are we in a git checkout?
dot_git=no
if test -e .git; then
	AC_DEFINE(HAVE_DOT_GIT, 1, [Are we in a git checkout?])
	dot_git=yes
fi
AM_CONDITIONAL(HAVE_DOT_GIT, test x$dot_git = xyes)

# If so, we include the git description in our logs for sanity checking.
#
# However, for people not running their own drivers this is just noise.
# So we copy the xserver's builderstring idiom to allow for this to be
# overridden and perhaps replaced with something more useful.
AC_ARG_WITH(builderstring,
	    AS_HELP_STRING([--with-builderstring=BUILDERSTRING],
			   [Additional builder string (default: use git describe)]),
	    [BUILDERSTRING="$withval"],
	    [BUILDERSTRING="x-magic-git-describe"])

if test "x$BUILDERSTRING" = xx-magic-git-describe -a x$dot_git = xyes; then
	AC_DEFINE(USE_GIT_DESCRIBE, 1, [Use automagic builder description])
else
  if test "x$BUILDERSTRING" != x -a "x$BUILDERSTRING" != xno -a "x$BUILDERSTRING" != xx-magic-git-describe; then
	  AC_DEFINE_UNQUOTED(BUILDER_DESCRIPTION, ["$BUILDERSTRING"], [Builder description])
  fi
fi

AC_ARG_ENABLE(gen4asm,
              AS_HELP_STRING([--enable-gen4asm],
			     [Enable rebuilding the gen4 assembly files [default=no]]),
              [ASM="$enableval"],
              [ASM=no])

gen4asm=no
if test x$ASM != "xno"; then
	PKG_CHECK_MODULES(GEN4ASM, [intel-gen4asm >= 1.2], [gen4asm=yes], [gen4asm=no])
	if test x$ASM = "xyes" -a x$gen4asm != "xyes"; then
		AC_MSG_ERROR([intel-gen4asm support requested but not found])
	fi
	if test x$gen4asm = "xyes"; then
		AC_MSG_CHECKING([path to use for intel-gen4asm])
		INTEL_GEN4ASM="`pkg-config intel-gen4asm --variable=exec_prefix`/bin/intel-gen4asm"
		if ! test -e "$INTEL_GEN4ASM"; then
			AC_MSG_ERROR([intel-gen4asm enabled, but not found. Tried '$INTEL_GEN4ASM'.])
		fi
		AC_MSG_RESULT([$INTEL_GEN4ASM])
		AC_SUBST([INTEL_GEN4ASM])
	fi
fi
AM_CONDITIONAL(HAVE_GEN4ASM, test x$gen4asm = xyes)

AC_ARG_ENABLE(udev,
              AS_HELP_STRING([--disable-udev],
                             [Disable udev-based monitor hotplug detection [default=auto]]),
              [UDEV="$enableval"],
              [UDEV=auto])

if test x$UDEV != "xno"; then
	PKG_CHECK_MODULES(UDEV, [libudev], [udev=yes], [udev=no])
	if test x$UDEV = xyes -a x$udev != xyes; then
		AC_MSG_ERROR([udev support requested but not found (libudev)])
	fi
	if test x$udev = xyes; then
		AC_DEFINE(HAVE_UDEV,1,[Enable udev-based monitor hotplug detection])
	fi
fi

PKG_CHECK_MODULES(X11, [x11 xrender xext pixman-1], [x11=yes], [x11=no])
AM_CONDITIONAL(HAVE_X11, test x$x11 = xyes)

AH_TOP([#include "xorg-server.h"])

# Define a configure option for an alternate module directory
AC_ARG_WITH(xorg-module-dir,
            AS_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

AC_ARG_ENABLE(dri, AS_HELP_STRING([--disable-dri],
                                  [Disable DRI support [[default=auto]]]))

AC_ARG_ENABLE(xvmc, AS_HELP_STRING([--disable-xvmc],
                                  [Disable XvMC support [[default=yes]]]),
              [XVMC="$enableval"],
              [XVMC=yes])

AC_ARG_ENABLE(kms-only, AS_HELP_STRING([--enable-kms-only],
                                  [Assume KMS support [[default=no]]]),
              [KMS_ONLY="$enableval"],
              [KMS_ONLY=no])
AC_ARG_ENABLE(ums-only, AS_HELP_STRING([--enable-ums-only],
                                  [Assume only UMS (no KMS) support [[default=no]]]),
              [UMS_ONLY="$enableval"],
              [UMS_ONLY=no])

required_xorg_xserver_version=1.6
required_pixman_version=0.24

if pkg-config --exists 'pixman-1 >= 0.27.1'; then
	AC_DEFINE([HAS_PIXMAN_GLYPHS], 1, [Enable pixman glyph cache])
fi

AC_ARG_ENABLE(sna,
	      AS_HELP_STRING([--enable-sna],
			     [Enable SandyBridge's New Acceleration (SNA) [default=auto]]),
	      [SNA="$enableval"],
	      [SNA=auto])

AC_CHECK_HEADERS([sys/sysinfo.h], , SNA=no)
if test "x$SNA" = "xauto" && pkg-config --exists "xorg-server >= 1.10"; then
	SNA=yes
fi
if test "x$SNA" != "xno"; then
	required_xorg_xserver_version=1.10
	AC_DEFINE(USE_SNA, 1, [Enable SNA support])
fi
AC_MSG_CHECKING([whether to include SNA support])
AM_CONDITIONAL(SNA, test x$SNA != xno)
AC_MSG_RESULT([$SNA])

AC_ARG_ENABLE(uxa,
	      AS_HELP_STRING([--enable-uxa],
			     [Enable Unified Acceleration Architecture (UXA) [default=yes]]),
	      [UXA="$enableval"],
	      [UXA=yes])
AC_MSG_CHECKING([whether to include UXA support])
AM_CONDITIONAL(UXA, test x$UXA != xno)
AC_MSG_RESULT([$UXA])
if test "x$UXA" != "xno"; then
	AC_DEFINE(USE_UXA, 1, [Enable UXA support])
	PKG_CHECK_MODULES(DRMINTEL, [libdrm_intel >= 2.4.29])
fi

AC_MSG_CHECKING([whether to include GLAMOR support])
AC_ARG_ENABLE(glamor,
	      AS_HELP_STRING([--enable-glamor],
			     [Enable glamor, a new GL-based acceleration [default=no]]),
	      [GLAMOR="$enableval"],
	      [GLAMOR=no])
AC_MSG_RESULT([$GLAMOR])
AM_CONDITIONAL(GLAMOR, test x$GLAMOR != xno)
if test "x$GLAMOR" != "xno"; then
	PKG_CHECK_MODULES(LIBGLAMOR, [glamor >= 0.3.1])
	PKG_CHECK_MODULES(LIBGLAMOR_EGL, [glamor-egl])
	AC_DEFINE(USE_GLAMOR, 1, [Enable glamor acceleration])
fi


PKG_CHECK_MODULES(XORG, [xorg-server >= $required_xorg_xserver_version xproto fontsproto pixman-1 >= $required_pixman_version $REQUIRED_MODULES])

AC_ARG_ENABLE(xaa,
	      AS_HELP_STRING([--enable-xaa],
			     [Enable legacy X Acceleration Architecture (XAA) [default=auto]]),
	      [XAA="$enableval"],
	      [XAA=auto])
if test "x$XAA" != xno; then
        save_CFLAGS=$CFLAGS
        CFLAGS=$XORG_CFLAGS
	AC_CHECK_HEADERS([xaa.h], XAA=yes, XAA=no)
        CFLAGS=$save_CFLAGS
fi
AC_MSG_CHECKING([whether to include XAA support])
AM_CONDITIONAL(XAA, test "x$XAA" = xyes)
AC_MSG_RESULT([$XAA])

AC_ARG_ENABLE(dga,
	      AS_HELP_STRING([--enable-dga],
			     [Enable legacy Direct Graphics Access (DGA) [default=auto]]),
	      [DGA="$enableval"],
	      [DGA=auto])
if test "x$DGA" != xno; then
        save_CFLAGS=$CFLAGS
        CFLAGS=$XORG_CFLAGS
	AC_CHECK_HEADERS([dgaproc.h], DGA=yes, DGA=no, [#include <dixstruct.h>])
        CFLAGS=$save_CFLAGS
fi
AC_MSG_CHECKING([whether to include DGA support])
AM_CONDITIONAL(DGA, test "x$DGA" = xyes)
AC_MSG_RESULT([$DGA])

AC_ARG_WITH(default-accel,
	    AS_HELP_STRING([--with-default-accel],
			   [Select the default acceleration method [default=uxa if enabled, otherwise sna]]),
			   [accel="$withval"],
			   [accel=auto])
if test "x$accel" = xyes; then
	AC_MSG_WARN([No default acceleration specified, choosing automatic selection])
	accel="auto"
fi

AC_MSG_CHECKING([which acceleration method to use by default])
if test "x$accel" = xauto; then
	if test "x$UXA" != "xno"; then
		accel=uxa
	else
		if test "x$SNA" != "xno"; then
			accel=sna
		fi
	fi
	if test "x$accel" = xauto -a "x$UMS_ONLY" != "xyes"; then
		AC_MSG_ERROR([No default acceleration option])
	fi
fi

have_accel=none
if test "x$accel" = xsna; then
	if test "x$SNA" != "xno"; then
		AC_DEFINE(DEFAULT_ACCEL_METHOD, SNA, [Default acceleration method])
		have_accel=yes
	else
		AC_MSG_ERROR([SNA requested as default, but is not enabled])
	fi
fi

if test "x$accel" = xuxa; then
	if test "x$UXA" != "xno"; then
		AC_DEFINE(DEFAULT_ACCEL_METHOD, UXA, [Default acceleration method])
		have_accel=yes
	else
		AC_MSG_ERROR([UXA requested as default, but is not enabled])
	fi
fi
AC_MSG_RESULT($accel)
if test "x$accel" = xnone -a "x$UMS_ONLY" != "xyes"; then
	AC_MSG_ERROR([No default acceleration option])
fi

AC_ARG_ENABLE(userptr,
	      AS_HELP_STRING([--enable-userptr],
			     [Enable use of userptr (experimental) [default=no]]),
	      [USERPTR="$enableval"],
	      [USERPTR=no])
AM_CONDITIONAL(USE_USERPTR, test x$USERPTR = xyes)
if test "x$USERPTR" = xyes; then
	AC_DEFINE(USE_USERPTR,1,[Assume USERPTR support])
fi

AC_ARG_ENABLE(fast-reloc,
	      AS_HELP_STRING([--enable-fast-reloc],
			     [Enable use of "fast reloc" (experimental) [default=no]]),
	      [FASTRELOC="$enableval"],
	      [FASTRELOC=no])
if test "x$FASTRELOC" = xyes; then
	AC_DEFINE(USE_FASTRELOC,1,[Assume "fast reloc" support])
fi

AC_ARG_ENABLE(handle-lut,
	      AS_HELP_STRING([--enable-handle-lut],
			     [Enable use of "handle LUT" (experimental) [default=no]]),
	      [HANDLE_LUT="$enableval"],
	      [HANDLE_LUT=no])
if test "x$HANDLE_LUT" = xyes; then
	AC_DEFINE(USE_HANDLE_LUT,1,[Assume "handle LUT" support])
fi

AC_ARG_ENABLE(async-swap,
	      AS_HELP_STRING([--enable-async-swap],
			     [Enable use of asynchronous swaps (experimental) [default=no]]),
	      [ASYNC_SWAP="$enableval"],
	      [ASYNC_SWAP=no])
AM_CONDITIONAL(USE_ASYNC_SWAP, test x$ASYNC_SWAP = xyes)
if test "x$ASYNC_SWAP" = xyes; then
	AC_DEFINE(USE_ASYNC_SWAP,1,[Assume asynchronous swap support])
fi

AC_ARG_ENABLE(debug,
	      AS_HELP_STRING([--enable-debug],
			     [Enables internal debugging [default=no]]),
              [DEBUG="$enableval"],
              [DEBUG=no])
# Store the list of server defined optional extensions in REQUIRED_MODULES
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XF86DRI, xextproto x11)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Obtain compiler/linker options for the driver dependencies
PKG_CHECK_MODULES(DRM, [libdrm >= 2.4.24]) # libdrm_intel is checked separately
PKG_CHECK_MODULES(DRI, [xf86driproto], , DRI=no)
PKG_CHECK_MODULES(DRI2, [dri2proto >= 2.6],, DRI2=no)
PKG_CHECK_MODULES(PCIACCESS, [pciaccess >= 0.10])

sdkdir=`$PKG_CONFIG --variable=sdkdir xorg-server`

if test "x$enable_dri" != "xno"; then
        save_CFLAGS="$CFLAGS"
        save_CPPFLAGS="$CPPFLAGS"
        CFLAGS="$CFLAGS $XORG_CFLAGS $DRI_CFLAGS $DRM_CFLAGS"
        CPPFLAGS="$CPPFLAGS $XORG_CFLAGS $DRI_CFLAGS $DRM_CFLAGS"
        AC_CHECK_HEADERS([dri.h sarea.h dristruct.h],, [DRI=no],
                [/* for dri.h */
                 #include <xf86str.h>
                 /* for dristruct.h */
                 #include <xorg-server.h>
                 #ifdef HAVE_DRI_H
                 # include <dri.h>
                 #endif
                 #ifdef HAVE_SAREA_H
                 # include <sarea.h>
                 #endif
                ])
        CFLAGS="$save_CFLAGS $DEBUGFLAGS"
        CPPFLAGS="$save_CPPFLAGS"
else
        DRI=no
fi

AC_MSG_CHECKING([whether to include DRI support])
AC_MSG_RESULT([${DRI-yes}])

AM_CONDITIONAL(DRI, test x$DRI != xno)
if test "x$DRI" != "xno"; then
        AC_DEFINE(HAVE_DRI1,1,[Enable DRI driver support])
else
        DRI_CFLAGS=""
        DRI_LIBS=""

        if test "x$enable_dri" = "xyes"; then
                AC_MSG_ERROR([DRI requested but prerequisites not found])
        fi
fi

if test "x$DRI2" != "xno"; then
	save_CFLAGS=$CFLAGS
	CFLAGS="$XORG_CFLAGS $DRM_CFLAGS $DRI_CFLAGS $DRI2_CFLAGS"
	AC_CHECK_HEADERS([dri2.h], DRI2=yes, DRI2=no, [#include <dixstruct.h>])
	CFLAGS=$save_CFLAGS
fi
AC_MSG_CHECKING([whether to include DRI2 support])
AM_CONDITIONAL(DRI2, test "x$DRI2" = xyes)
AC_MSG_RESULT([$DRI2])

if test "$XVMC" = yes; then
	PKG_CHECK_MODULES(XVMCLIB,
			  [xvmc xext xfixes dri2proto x11-xcb xcb-dri2 xcb-aux],
			  [XVMC=yes], [XVMC=no])
fi
AC_MSG_CHECKING([whether to include XvMC support])
AC_MSG_RESULT([$XVMC])
AM_CONDITIONAL(XVMC, test x$XVMC = xyes)
if test "x$XVMC" = xyes; then
	AC_DEFINE(ENABLE_XVMC,1,[Enable XvMC support])
fi

AM_CONDITIONAL(KMS_ONLY, test x$KMS_ONLY = xyes)
if test "x$KMS_ONLY" = xyes; then
	AC_DEFINE(KMS_ONLY,1,[Assume KMS support])
fi
AM_CONDITIONAL(UMS_ONLY, test x$UMS_ONLY = xyes)
if test "x$UMS_ONLY" = xyes; then
	AC_DEFINE(UMS_ONLY,1,[Assume only UMS (no KMS) support])
fi

AM_CONDITIONAL(DEBUG, test x$DEBUG != xno)
AM_CONDITIONAL(FULL_DEBUG, test x$DEBUG = xfull)
if test "x$DEBUG" = xno; then
	AC_DEFINE(NDEBUG,1,[Disable internal debugging])
fi
if test "x$DEBUG" != xno; then
	PKG_CHECK_MODULES(VALGRIND, [valgrind], have_valgrind=yes, have_valgrind=no)
	if test x$have_valgrind = xyes; then
		AC_DEFINE([HAVE_VALGRIND], 1, [Use valgrind intrinsics to suppress false warnings])
	fi
fi
if test "x$DEBUG" = xsync; then
	AC_DEFINE(DEBUG_SYNC,1,[Enable synchronous rendering for debugging])
fi
if test "x$DEBUG" = xmemory; then
	AC_DEFINE(DEBUG_MEMORY,1,[Enable memory debugging])
fi
if test "x$DEBUG" = xpixmap; then
	AC_DEFINE(DEBUG_PIXMAP,1,[Enable pixmap debugging])
fi
if test "x$DEBUG" = xfull; then
	AC_DEFINE(DEBUG_MEMORY,1,[Enable memory debugging])
	AC_DEFINE(DEBUG_PIXMAP,1,[Enable pixmap debugging])
	AC_DEFINE(HAS_DEBUG_FULL,1,[Enable all debugging])
        CFLAGS="$CFLAGS -O0 -ggdb3"
fi

DRIVER_NAME=intel
AC_SUBST([DRIVER_NAME])
AC_SUBST([moduledir])

AC_CONFIG_FILES([
                Makefile
                uxa/Makefile
                src/Makefile
                src/xvmc/Makefile
                src/xvmc/shader/Makefile
                src/xvmc/shader/mc/Makefile
                src/xvmc/shader/vld/Makefile
                src/legacy/Makefile
                src/legacy/i810/Makefile
                src/legacy/i810/xvmc/Makefile
                src/sna/Makefile
                src/sna/brw/Makefile
                src/sna/fb/Makefile
                man/Makefile
                src/render_program/Makefile
		test/Makefile
])
AC_OUTPUT
