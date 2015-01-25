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
AC_INIT([xf86-video-vmware],
        [13.1.0],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        [xf86-video-vmware])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR(.)
AC_SYS_LARGEFILE

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_MAINTAINER_MODE

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# Checks for programs.
AM_PROG_CC_C_O

AH_TOP([#include "xorg-server.h"])

# Define a configure option for an alternate module directory
AC_ARG_WITH(xorg-module-dir,
            AS_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

# Define a configure option to build the vmwarectrl client tool
AC_ARG_ENABLE(vmwarectrl-client,
              AS_HELP_STRING([--enable-vmwarectrl-client],
                             [Enable vmwarectrl client (default: disabled)]),
                             [VMWARECTRL=$enableval], [VMWARECTRL=no])
AM_CONDITIONAL(BUILD_VMWARECTRL, [test "x$VMWARECTRL" = xyes])

# Store the list of server defined optional extensions in REQUIRED_MODULES
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(DPMSExtension, xextproto)
XORG_DRIVER_CHECK_EXT(XINERAMA, xineramaproto)
XORG_DRIVER_CHECK_EXT(XV, videoproto)

# Obtain compiler/linker options for the driver dependencies
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.0.1 xproto fontsproto $REQUIRED_MODULES])
PKG_CHECK_EXISTS([xorg-server >= 1.1.0],
                 [AC_DEFINE([HAVE_XORG_SERVER_1_1_0], 1,
                 [Has version 1.1.0 or greater of the Xserver])])

PKG_CHECK_EXISTS([xorg-server >= 1.2.0],
                 [AC_DEFINE([HAVE_XORG_SERVER_1_2_0], 1,
                 [Has version 1.2.0 or greater of the Xserver])])

PKG_CHECK_EXISTS([xorg-server >= 1.4.99],
                 [AC_DEFINE([HAVE_XORG_SERVER_1_5_0], 1,
                 [Has version 1.5.0 or greater of the Xserver])])

PKG_CHECK_EXISTS([xorg-server >= 1.7.0],
                 [AC_DEFINE([HAVE_XORG_SERVER_1_7_0], 1,
                 [Has version 1.7.0 or greater of the Xserver])
		 BUILD_VMWGFX=yes],[BUILD_VMWGFX=no])

PKG_CHECK_EXISTS([xorg-server >= 1.12.0],
                 [AC_DEFINE([HAVE_XORG_SERVER_1_12_0], 1,
                 [Has version 1.12.0 or greater of the Xserver])])

# Obtain compiler/linker options for the vmwarectrl client tool
PKG_CHECK_MODULES(X11, x11 xext)

# Checks for libraries.

save_CFLAGS="$CFLAGS"
CFLAGS="$XORG_CFLAGS"
AC_CHECK_DECL(XSERVER_LIBPCIACCESS,
             [XSERVER_LIBPCIACCESS=yes], [XSERVER_LIBPCIACCESS=no],
             [#include "xorg-server.h"])
CFLAGS="$save_CFLAGS"

if test x$XSERVER_LIBPCIACCESS = xyes; then
	PKG_CHECK_MODULES([PCIACCESS], [pciaccess >= 0.8.0])
fi

AM_CONDITIONAL(XSERVER_LIBPCIACCESS, test "x$XSERVER_LIBPCIACCESS" = xyes)

AC_SUBST([moduledir])

if test x$BUILD_VMWGFX = xyes; then
	PKG_CHECK_MODULES([LIBDRM], [libdrm],[],[BUILD_VMWGFX=no])
fi
if test x$BUILD_VMWGFX = xyes; then
#
# Early versions of mesa 10 forgot to bump the XA major version number in
# the xa_tracker.h header
#
	PKG_CHECK_MODULES([XATRACKER], [xatracker >= 0.4.0],
			  [PKG_CHECK_EXISTS([xatracker = 2.0.0],
	                  [AC_DEFINE([HAVE_XA_2], 1,
               		  [Has version 2 of XA])])],
			  [BUILD_VMWGFX=no])
#
# Check for prime.
#
	PKG_CHECK_EXISTS([libdrm >= 2.4.38],
			 [AC_DEFINE([HAVE_LIBDRM_2_4_38], 1,
			 [Has version 2.4.38 or greater of libdrm])])
fi

DRIVER_NAME=vmware
AC_SUBST([DRIVER_NAME])

AC_MSG_CHECKING([whether to build Kernel Mode Setting and 3D])
if test x$BUILD_VMWGFX = xyes; then
	AC_MSG_RESULT([yes])
        AC_DEFINE([BUILD_VMWGFX], 1, [Building the vmwgfx driver path])
else
	AC_MSG_RESULT([no])
fi

AM_CONDITIONAL(BUILD_VMWGFX, test "x$BUILD_VMWGFX" = xyes)

AC_CONFIG_FILES([
                Makefile
                man/Makefile
                saa/Makefile
                vmwgfx/Makefile
                src/Makefile
                vmwarectrl/Makefile
])

AC_OUTPUT
