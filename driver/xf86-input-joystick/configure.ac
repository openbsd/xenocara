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
AC_INIT([xf86-input-joystick],
        [1.6.4],
        [https://gitlab.freedesktop.org/xorg/driver/xf86-input-joystick/-/issues],
        [xf86-input-joystick])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR(.)

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-xz])

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

# Initialize X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Checks for extensions
XORG_DRIVER_CHECK_EXT(XINPUT, inputproto)
XORG_DRIVER_CHECK_EXT(XKB, kbproto)

# Checks for pkg-config packages
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.9.99.2] xproto $REQUIRED_MODULES)

DRIVER_NAME=joystick
AC_SUBST([DRIVER_NAME])

# -----------------------------------------------------------------------------
#			Configuration options
# -----------------------------------------------------------------------------
# Define a configure option for an alternate input module directory
AC_ARG_WITH(xorg-module-dir,
            AS_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])
inputdir=${moduledir}/input
AC_SUBST(inputdir)

# Define a configure option for an alternate X Server SDK Headers
# X Server SDK location is required to install joystick header files
AC_ARG_WITH(sdkdir,
            AS_HELP_STRING([--with-sdkdir=<path>],
                           [Xorg X Server sdk headers (default is autodetected)]),
            [sdkdir="$withval"],
            [sdkdir=`$PKG_CONFIG --variable=sdkdir xorg-server`])
AC_SUBST(sdkdir)

# Define a configure option to enable code debugging
AC_ARG_ENABLE(debug, AS_HELP_STRING([--disable-debug],
                                    [Disable debugging code (default: enabled)]),
                                    [DEBUGGING=$enableval], [DEBUGGING=yes])
if test "x$DEBUGGING" = xyes; then
   AC_DEFINE(DEBUG, 1, [Enable debugging code])
fi
AM_CONDITIONAL(DEBUG, [test "x$DEBUGGING" = xyes])

# -----------------------------------------------------------------------------
#		Determine which joystick backend to build
# -----------------------------------------------------------------------------
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
AC_CHECK_HEADERS([dev/hid/hid.h])
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
# -----------------------------------------------------------------------------

AC_CONFIG_FILES([Makefile
                src/Makefile
                man/Makefile
                config/Makefile
                include/Makefile
                xorg-joystick.pc])
AC_OUTPUT

# -----------------------------------------------------------------------------

AS_ECHO() 
AS_ECHO("Building Linux joystick backend: $linux_backend")
AS_ECHO("Building Linux evdev backend: $evdev_backend")
AS_ECHO("Building BSD usbhid backend: $bsd_backend")

if test "x$linux_backend" != "xyes" -a \
	"x$bsd_backend" != "xyes" -a \
	"x$evdev_backend" != "xyes"; then
    AC_MSG_ERROR([No backends were found. Your operating system is not
                  supported by the joystick driver. Contact
                  xorg@lists.freedesktop.org if you are interested in
                  porting it.])
fi
