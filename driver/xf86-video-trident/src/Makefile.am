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
trident_drv_la_LTLIBRARIES = trident_drv.la
trident_drv_la_LDFLAGS = -module -avoid-version
trident_drv_ladir = @moduledir@/drivers

trident_drv_la_SOURCES = \
         blade_accel.c \
         blade_accel_exa.c \
         image_accel.c \
         trident_accel.c \
         trident_bank.c \
         trident_dac.c \
         trident_dga.c \
         trident_driver.c \
         trident.h \
         tridenthelper.c \
         trident_i2c.c \
         tridentramdac.c \
         trident_regs.h \
         trident_shadow.c \
         trident_tv.c \
         trident_video.c \
         tvga_dac.c \
         xp_accel.c \
	 xp4_accel.c \
	 xp4_accel_exa.c
