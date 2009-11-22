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
AM_CFLAGS = @XORG_CFLAGS@
s3_drv_la_LTLIBRARIES = s3_drv.la
s3_drv_la_LDFLAGS = -module -avoid-version
s3_drv_ladir = @moduledir@/drivers

s3_drv_la_SOURCES = \
         newmmio.h \
         s3_bios.c \
         s3_cursor.c \
         s3_dga.c \
         s3_driver.c \
         s3.h \
         s3_IBMRGB.c \
	 s3_pcirename.h \
         s3_reg.h \
         s3_Ti.c \
         s3_Trio64DAC.c \
	 s3_shadow.c

noinst_LTLIBRARIES = libs3_accel_newmmio.la libs3_accel_pio.la
s3_drv_la_LIBADD = libs3_accel_newmmio.la libs3_accel_pio.la

libs3_accel_newmmio_la_SOURCES = s3_accel.c s3_video.c
libs3_accel_newmmio_la_CFLAGS = $(AM_CFLAGS) -DS3_NEWMMIO=1

libs3_accel_pio_la_SOURCES = s3_accel.c
libs3_accel_pio_la_CFLAGS = $(AM_CFLAGS)
