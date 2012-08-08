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
AC_INIT([xf86-video-ati],
        [6.14.6],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        [xf86-video-ati])

AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

AC_CONFIG_AUX_DIR(.)

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AC_SYS_LARGEFILE

AM_MAINTAINER_MODE

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# Checks for programs.
AM_PROG_CC_C_O

if test "x$GCC" = "xyes"; then
	CPPFLAGS="$CPPFLAGS -Wall"
fi

AH_TOP([#include "xorg-server.h"])

# Define a configure option for an alternate module directory
AC_ARG_WITH(xorg-module-dir,
            AS_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

AC_ARG_ENABLE(dri, AS_HELP_STRING([--disable-dri],
                                  [Disable DRI support [[default=auto]]]),
              [DRI="$enableval"],
              [DRI=auto])

AC_ARG_ENABLE(exa,
              AS_HELP_STRING([--disable-exa],
                             [Disable EXA support [[default=enabled]]]),
              [EXA="$enableval"],
              [EXA=yes])

AC_ARG_ENABLE(kms,
              AS_HELP_STRING([--disable-kms],
                             [Disable KMS support [[default=enabled]]]),
              [DRM_MODE="$enableval"],
              [DRM_MODE=yes])

# Store the list of server defined optional extensions in REQUIRED_MODULES
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XV, videoproto)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)

# Obtain compiler/linker options for the driver dependencies
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.3 xproto fontsproto $REQUIRED_MODULES])
PKG_CHECK_MODULES(XEXT, [xextproto >= 7.0.99.1],
                  HAVE_XEXTPROTO_71="yes"; AC_DEFINE(HAVE_XEXTPROTO_71, 1, [xextproto 7.1 available]),
                  HAVE_XEXTPROTO_71="no")
AM_CONDITIONAL(HAVE_XEXTPROTO_71, [ test "$HAVE_XEXTPROTO_71" = "yes" ])

# Checks for libraries.


if test "$DRI" != no; then
	PKG_CHECK_MODULES(DRI, [libdrm >= 2.2 xf86driproto])
	save_CPPFLAGS="$CPPFLAGS"
	CPPFLAGS="$XORG_CFLAGS $DRI_CFLAGS"
	AC_CHECK_HEADER([dri.h],
                      [have_dri_h="yes"], [have_dri_h="no"],[-])
	AC_CHECK_HEADER([sarea.h],
                      [have_sarea_h="yes"], [have_sarea_h="no"],[-])
	AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[
#include <xorg-server.h>
#include <dristruct.h>
                      ]])],
                      [have_dristruct_h="yes"], [have_dristruct_h="no"],[-])
	AC_CHECK_HEADER([damage.h],
                      [have_damage_h="yes"], [have_damage_h="no"],[-])
	CPPFLAGS="$save_CPPFLAGS"
fi

AC_MSG_CHECKING([whether to include DRI support])
if test x$DRI = xauto; then
	if test "$have_dri_h" = yes -a \
                "$have_sarea_h" = yes -a \
                "$have_dristruct_h" = yes; then
		DRI="yes"
	else
		DRI="no"
	fi
fi
AC_MSG_RESULT([$DRI])

AM_CONDITIONAL(DRI, test x$DRI = xyes)
if test "$DRI" = yes; then
        AC_DEFINE(XF86DRI,1,[Enable DRI driver support])
        AC_DEFINE(XF86DRI_DEVEL,1,[Enable developmental DRI driver support])
	if test "$have_damage_h" = yes; then
		AC_DEFINE(DAMAGE,1,[Use Damage extension])
	fi

        save_CFLAGS="$CFLAGS"
        CFLAGS="$XORG_CFLAGS $DRI_CFLAGS $CFLAGS"
	if test "$DRM_MODE" = yes; then
	   	AC_CHECK_HEADER(xf86drmMode.h,[DRM_MODE=yes],[DRM_MODE=no],[#include <stdint.h>
#include <stdlib.h>])
		if test "x$DRM_MODE" = xyes; then
			PKG_CHECK_MODULES(LIBDRM_RADEON, [xorg-server >= 1.6.2 libdrm >= 2.4.36 libdrm_radeon],
			[LIBDRM_RADEON=yes], [LIBDRM_RADEON=no])

			if test "x$LIBDRM_RADEON" = xyes; then
			   	AC_DEFINE(XF86DRM_MODE,1,[DRM kernel modesetting])
				AC_DEFINE(RADEON_DRI2, 1,[Enable DRI2 code])
                        else
                                DRM_MODE=no
			fi
			PKG_CHECK_MODULES(LIBUDEV, [libudev], [LIBUDEV=yes], [LIBUDEV=no])
			if test "x$LIBUDEV" = xyes; then
			       AC_DEFINE(HAVE_LIBUDEV, 1,[libudev support])
			fi
        	fi
	fi
	CFLAGS="$save_CFLAGS"
else
	DRM_MODE=no
fi
AM_CONDITIONAL(DRM_MODE, test x$DRM_MODE = xyes)
AM_CONDITIONAL(LIBUDEV, test x$LIBUDEV = xyes)

AC_DEFINE(USE_XAA, 1, [Build support for XAA])

# Properly handle EXA.
AC_MSG_CHECKING([whether to enable EXA support])
if test "x$EXA" = xyes; then
        AC_MSG_RESULT(yes)

        SAVE_CPPFLAGS="$CPPFLAGS"
        CPPFLAGS="$CPPFLAGS $XORG_CFLAGS"
        AC_CHECK_HEADER(exa.h,
                       [have_exa_h="yes"], [have_exa_h="no"])
        CPPFLAGS="$SAVE_CPPFLAGS"
else
        AC_MSG_RESULT(no)
fi 

SAVE_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $XORG_CFLAGS"
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

AC_CHECK_DECL(xf86XVFillKeyHelperDrawable,
	      [AC_DEFINE(HAVE_XV_DRAWABLE_HELPER, 1, [Have xf86XVFillKeyHelperDrawable prototype])],
	      [],
	      [#include <xf86xv.h>])

AC_CHECK_DECL(xf86ModeBandwidth,
	      [AC_DEFINE(HAVE_XF86MODEBANDWIDTH, 1, [Have xf86ModeBandwidth prototype])],
	      [],
	      [#include "xorg-server.h"
	       #include "xf86Modes.h"])

AC_CHECK_DECL(xf86_crtc_clip_video_helper,
	      [AC_DEFINE(HAVE_XF86CRTCCLIPVIDEOHELPER, 1, [Have xf86_crtc_clip_video_helper prototype])],
	      [],
	      [#include <X11/Xmd.h>
	       #include "xorg-server.h"
	       #include "xf86i2c.h"
	       #include "xf86Crtc.h"])

AC_CHECK_DECL(xf86RotateFreeShadow,
	      [AC_DEFINE(HAVE_FREE_SHADOW, 1, [have new FreeShadow API])],
	      [],
	      [#include <xorg-server.h>
               #include <windowstr.h>
	       #include <xf86Crtc.h>])

AC_CHECK_DECL(pci_device_enable,
	      [AC_DEFINE(HAVE_PCI_DEVICE_ENABLE, 1, [Have pci_device_enable prototype])],
	      [],
	      [#include <pciaccess.h>])

AC_CHECK_DECL(XSERVER_LIBPCIACCESS,
	      [XSERVER_LIBPCIACCESS=yes],[XSERVER_LIBPCIACCESS=no],
	      [#include "xorg-server.h"])

AC_CHECK_HEADERS([list.h],
		 [have_list_h="yes"], [have_list_h="no"],
		 [#include <X11/Xdefs.h>
		  #include "xorg-server.h"])

if test "x$have_list_h" = xyes; then
    AC_CHECK_DECL(xorg_list_init,
		  [AC_DEFINE(HAVE_XORG_LIST, 1, [Have xorg_list API])], [],
		  [#include <X11/Xdefs.h>
		   #include "xorg-server.h"
		   #include "list.h"])
fi

CPPFLAGS="$SAVE_CPPFLAGS"

AM_CONDITIONAL(USE_EXA, test "x$USE_EXA" = xyes)

AM_CONDITIONAL(XF86DRM_MODE, test "x$LIBDRM_RADEON" = xyes)

if test "x$XSERVER_LIBPCIACCESS" = xyes; then
    PKG_CHECK_MODULES([PCIACCESS], [pciaccess >= 0.8.0])
    XORG_CFLAGS="$XORG_CFLAGS $PCIACCESS_CFLAGS"
fi
AM_CONDITIONAL(XSERVER_LIBPCIACCESS, test "x$XSERVER_LIBPCIACCESS" = xyes)

# Checks for headers/macros for byte swapping
# Known variants:
#	<byteswap.h> bswap_16, bswap_32, bswap_64  (glibc)
#	<sys/endian.h> __swap16, __swap32, __swap64 (OpenBSD)
#	<sys/endian.h> bswap16, bswap32, bswap64 (other BSD's)
#	and a fallback to local macros if none of the above are found

# if <byteswap.h> is found, assume it's the correct version
AC_CHECK_HEADERS([byteswap.h])

# if <sys/endian.h> is found, have to check which version
AC_CHECK_HEADER([sys/endian.h], [HAVE_SYS_ENDIAN_H="yes"], [HAVE_SYS_ENDIAN_H="no"])

if test "x$HAVE_SYS_ENDIAN_H" = "xyes" ; then
	AC_MSG_CHECKING([for __swap16 variant of <sys/endian.h> byteswapping macros])
	AC_LINK_IFELSE([AC_LANG_PROGRAM([
#include <sys/types.h>
#include <sys/endian.h>
 ], [
int a = 1, b;
b = __swap16(a);
 ])
], [SYS_ENDIAN__SWAP='yes'], [SYS_ENDIAN__SWAP='no'])
	AC_MSG_RESULT([$SYS_ENDIAN__SWAP])

	AC_MSG_CHECKING([for bswap16 variant of <sys/endian.h> byteswapping macros])
	AC_LINK_IFELSE([AC_LANG_PROGRAM([
#include <sys/types.h>
#include <sys/endian.h>
 ], [
int a = 1, b;
b = bswap16(a);
 ])
], [SYS_ENDIAN_BSWAP='yes'], [SYS_ENDIAN_BSWAP='no'])
	AC_MSG_RESULT([$SYS_ENDIAN_BSWAP])

    	if test "$SYS_ENDIAN_BSWAP" = "yes" ; then
		USE_SYS_ENDIAN_H=yes
		BSWAP=bswap
	else	
	    	if test "$SYS_ENDIAN__SWAP" = "yes" ; then
			USE_SYS_ENDIAN_H=yes
			BSWAP=__swap
		else
			USE_SYS_ENDIAN_H=no
		fi
	fi

	if test "$USE_SYS_ENDIAN_H" = "yes" ; then
	    AC_DEFINE([USE_SYS_ENDIAN_H], 1, 
		[Define to use byteswap macros from <sys/endian.h>])
	    AC_DEFINE_UNQUOTED([bswap_16], ${BSWAP}16, 
			[Define to 16-bit byteswap macro])
	    AC_DEFINE_UNQUOTED([bswap_32], ${BSWAP}32, 
			[Define to 32-bit byteswap macro])
	    AC_DEFINE_UNQUOTED([bswap_64], ${BSWAP}64, 
			[Define to 64-bit byteswap macro])
	fi
fi

case $host_os in
  *linux*)
  AC_DEFINE(FGL_LINUX, 1, [Use linux pragma pack]) ;;
esac

AC_SUBST([moduledir])

DRIVER_NAME=ati
AC_SUBST([DRIVER_NAME])

AC_MSG_NOTICE(
[The atimisc sub-driver has been split out to xf86-video-mach64:]
[    git://anongit.freedesktop.org/git/xorg/driver/xf86-video-mach64]
[Please install that driver as well for mach64-based cards.]
)

AC_MSG_NOTICE(
[The r128 sub-driver has been split out to xf86-video-r128:]
[    git://anongit.freedesktop.org/git/xorg/driver/xf86-video-r128]
[Please install that driver as well for rage128-based cards.]
)

AC_CONFIG_FILES([
                Makefile
                src/Makefile
                man/Makefile
])
AC_OUTPUT

dnl
dnl Output some configuration info for the user
dnl
echo ""
echo "        prefix:              $prefix"
echo "        exec_prefix:         $exec_prefix"
echo "        libdir:              $libdir"
echo "        includedir:          $includedir"


echo ""
echo "        Kernel modesetting:  $DRM_MODE"

echo ""
echo "        CFLAGS:              $CFLAGS"
echo "        CXXFLAGS:            $CXXFLAGS"
echo "        Macros:              $DEFINES"

echo ""
echo "        Run '${MAKE-make}' to build xf86-video-ati"
echo ""
