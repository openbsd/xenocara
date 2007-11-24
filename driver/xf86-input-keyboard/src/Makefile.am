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

driverdir = @inputdir@
driver_LTLIBRARIES = kbd_drv.la
kbd_drv_la_LDFLAGS = -avoid-version -module
kbd_drv_la_SOURCES = kbd.c xf86OSKbd.h xf86Keymap.h

BSD_SRCS = bsd_KbdMap.c bsd_kbd.c bsd_kbd.h at_scancode.c
HURD_SRCS = hurd_kbd.c at_scancode.c
LINUX_SRCS = lnx_KbdMap.c lnx_kbd.c lnx_kbd.h at_scancode.c
SCO_SRCS = sco_KbdMap.c sco_kbd.c sco_kbd.h
SOLARIS_SRCS = sun_kbd.c sun_kbd.h sun_kbdMap.c

if BSD
kbd_drv_la_SOURCES += $(BSD_SRCS)
endif

if SOLARIS
kbd_drv_la_SOURCES += $(SOLARIS_SRCS)
endif

if LINUX
kbd_drv_la_SOURCES += $(LINUX_SRCS)
endif

if SCO
kbd_drv_la_SOURCES += $(SCO_SRCS)
endif

if HURD
kbd_drv_la_SOURCES += $(HURD_SRCS)
endif

EXTRA_DIST = $(BSD_SRCS) $(HURD_SRCS) $(LINUX_SRCS) $(SCO_SRCS) $(SOLARIS_SRCS)
