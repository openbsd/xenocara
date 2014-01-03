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


# this is obnoxious:
# -module lets us name the module exactly how we want
# -avoid-version prevents gratuitous .0.0.0 version numbers on the end
# _ladir passes a dummy rpath to libtool so the thing will actually link
# TODO: -nostdlib/-Bstatic/-lgcc platform magic, not installing the .a, etc.

AM_CFLAGS = $(XORG_CFLAGS) $(CWARNFLAGS)

@DRIVER_NAME@_drv_la_LTLIBRARIES = @DRIVER_NAME@_drv.la
@DRIVER_NAME@_drv_la_LDFLAGS = -module -avoid-version
@DRIVER_NAME@_drv_ladir = @inputdir@

AM_CPPFLAGS = -I$(top_srcdir)/include

@DRIVER_NAME@_drv_la_SOURCES =  jstk.c jstk.h \
				jstk_axis.c jstk_axis.h \
				jstk_key.c jstk_key.h \
				jstk_options.c jstk_options.h \
				jstk_properties.c jstk_properties.h

BSD_SRCS   = backend_bsd.c backend_bsd.h
LINUX_SRCS = backend_joystick.c backend_joystick.h
EVDEV_SRCS = backend_evdev.c backend_evdev.h

if BSD_BACKEND
@DRIVER_NAME@_drv_la_SOURCES += $(BSD_SRCS)
@DRIVER_NAME@_drv_la_LDFLAGS += -lusbhid
endif

if LINUX_BACKEND
@DRIVER_NAME@_drv_la_SOURCES += $(LINUX_SRCS)
endif

if EVDEV_BACKEND
@DRIVER_NAME@_drv_la_SOURCES += $(EVDEV_SRCS)
endif

EXTRA_DIST = $(BSD_SRCS) $(LINUX_SRCS)
