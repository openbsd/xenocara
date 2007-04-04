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

SUBDIRS = xvmc
# this is obnoxious:
# -module lets us name the module exactly how we want
# -avoid-version prevents gratuitous .0.0.0 version numbers on the end
# _ladir passes a dummy rpath to libtool so the thing will actually link
# TODO: -nostdlib/-Bstatic/-lgcc platform magic, not installing the .a, etc.
AM_CFLAGS = @XORG_CFLAGS@ @DRI_CFLAGS@ -DI830_XV

i810_drv_la_LTLIBRARIES = i810_drv.la
i810_drv_la_LDFLAGS = -module -avoid-version
i810_drv_ladir = @moduledir@/drivers

i810_drv_la_SOURCES = \
         common.h \
         i810_accel.c \
         i810_common.h \
         i810_cursor.c \
         i810_dga.c \
         i810_driver.c \
         i810.h \
         i810_io.c \
         i810_memory.c \
         i810_reg.h \
         i810_video.c \
         i810_wmark.c \
         i830_accel.c \
         i830_common.h \
         i830_cursor.c \
         i830_dga.c \
         i830_driver.c \
         i830.h \
         i830_io.c \
         i830_memory.c \
         i830_modes.c \
         i830_video.c \
         i830_shadow.c

if DRI
i810_drv_la_SOURCES += \
         i810_dri.c \
         i810_dri.h \
         i830_dri.c \
         i810_hwmc.c \
         i830_dri.h 
endif
