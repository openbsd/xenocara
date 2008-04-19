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
AM_CFLAGS = @XORG_CFLAGS@ @DRI_CFLAGS@
sis_drv_la_LTLIBRARIES = sis_drv.la
sis_drv_la_LDFLAGS = -module -avoid-version
sis_drv_ladir = @moduledir@/drivers

sis_drv_la_SOURCES = \
         300vtbl.h \
         310vtbl.h \
         init301.c \
         init301.h \
         init.c \
         initdef.h \
         initextx.c \
         initextx.h \
         init.h \
         oem300.h \
         oem310.h \
         osdef.h \
         sis300_accel.c \
         sis300_accel.h \
         sis310_accel.c \
         sis310_accel.h \
         sis6326_video.c \
	 sispcirename.h \
         sis_accel.c \
         sis_accel.h \
         sis_common.h \
         sis_cursor.c \
         sis_cursor.h \
         sis_dac.c \
         sis_dac.h \
         sis_dga.c \
         sis_driver.c \
         sis_driver.h \
         sis.h \
         sis_memcpy.c \
         sis_opt.c \
         sis_regs.h \
         sis_setup.c \
         sis_shadow.c \
         sis_utility.c \
         sis_vb.c \
         sis_vga.c \
         sis_video.c \
         sis_video.h \
         sis_videostr.h \
         vgatypes.h \
         vstruct.h

if DRI
sis_drv_la_SOURCES += \
         sis_dri.c \
         sis_dri.h 
endif
