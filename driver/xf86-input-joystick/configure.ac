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
AC_INIT([xf86-input-joystick],
        1.4.1,
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],
        xf86-input-joystick)

AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_AUX_DIR(.)
AM_INIT_AUTOMAKE([dist-bzip2])

AM_MAINTAINER_MODE

# Require xorg-macros: XORG_CWARNFLAGS, XORG_CHANGELOG
m4_ifndef([XORG_MACROS_VERSION], [AC_FATAL([must install xorg-macros 1.2 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.2)
AM_CONFIG_HEADER([config.h])

# Checks for programs.
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_PROG_CC
XORG_CWARNFLAGS

AH_TOP([#include "xorg-server.h"])

AC_ARG_WITH(xorg-module-dir,
            AC_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory 
                           [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

AC_ARG_ENABLE(debug,
            AS_HELP_STRING([--enable-debug],
                           [Enable debugging (default: enabled)]),
			   [DEBUGGING=$enableval], [DEBUGGING=yes])
if test "x$DEBUGGING" = xyes; then
    AC_DEFINE(DEBUG, 1, [Enable debugging code])
else
    AC_DEFINE(NDEBUG, 1, [Disable some debugging code])
fi
AM_CONDITIONAL(DEBUG, [test "x$DEBUGGING" = xyes])

inputdir=${moduledir}/input
AC_SUBST(inputdir)

# Checks for extensions
XORG_DRIVER_CHECK_EXT(XINPUT, inputproto)
XORG_DRIVER_CHECK_EXT(XKB, kbproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, 
               [xorg-server >= 1.3.99.901 xorg-server <= 1.6.99] xproto $REQUIRED_MODULES)
XORG_CFLAGS="$CWARNFLAGS $XORG_CFLAGS"
xorg_sdkdir=$(pkg-config --variable=sdkdir xorg-server)
AC_ARG_WITH(sdkdir,
	AC_HELP_STRING([--with-sdkdir=<path>],
		[Xorg X Server sdk headers (default is autodetected)]),
	[sdkdir="$withval"], [sdkdir="${xorg_sdkdir}"])
AC_SUBST(sdkdir)

# Checks for libraries.

# Checks for header files.
linux_backend=yes
AC_CHECK_HEADERS([linux/joystick.h],, [linux_backend=no])
AM_CONDITIONAL(LINUX_BACKEND, [test "x$linux_backend" = xyes])
if test "x$linux_backend" = xyes; then
    AC_DEFINE(LINUX_BACKEND, 1, [Compile Linux joystick backend])
fi

bsd_backend=yes
AC_CHECK_HEADERS([dev/usb/usb.h])
AC_CHECK_HEADERS([usbhid.h dev/usb/usbhid.h],, [bsd_backend=no],
[[#if HAVE_DEV_USB_USB_H
#include <dev/usb/usb.h>
#endif
]])
AC_CHECK_LIB([usbhid],[hid_get_item],, [bsd_backend=no])
AM_CONDITIONAL(BSD_BACKEND, [test "x$bsd_backend" = xyes])
if test "x$bsd_backend" = xyes; then
    AC_CHECK_HEADERS([dev/usb/usb_ioctl.h])
    AC_DEFINE(BSD_BACKEND, 1, [Compile BSD usbhid backend])
fi

evdev_backend=yes
AC_CHECK_HEADERS([linux/input.h],, [evdev_backend=no])
AM_CONDITIONAL(EVDEV_BACKEND, [test "x$evdev_backend" = xyes])
if test "x$evdev_backend" = xyes; then
    AC_DEFINE(EVDEV_BACKEND, 1, [Compile Linux evdev backend])
fi

AC_HEADER_STDC

DRIVER_NAME=joystick
AC_SUBST([DRIVER_NAME])

XORG_MANPAGE_SECTIONS
XORG_RELEASE_VERSION
XORG_CHANGELOG

AC_OUTPUT([Makefile
           src/Makefile
           man/Makefile
           config/Makefile
           include/Makefile
           xorg-joystick.pc])

echo 
echo Building Linux joystick backend: $linux_backend
echo Building Linux evdev backend: $evdev_backend
echo Building BSD usbhid backend: $bsd_backend

if test "x$linux_backend" != "xyes" -a \
	"x$bsd_backend" != "xyes" -a \
	"x$evdev_backend" != "xyes"; then
    AC_MSG_ERROR([No backends were found. Your operating is not supported by the
                  joystick driver. Contact xorg@lists.freedesktop.org if you are 
                  interested in porting it.])
fi
