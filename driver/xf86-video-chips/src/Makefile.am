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

# sick hack borrowed from mga.  please, people, don't write code this ugly.
noinst_LTLIBRARIES = libct_accelmm.la libct_accelhi.la

libct_accelmm_la_SOURCES = ct_accel.c
libct_accelmm_la_CFLAGS = @XORG_CFLAGS@ -DCHIPS_MMIO

libct_accelhi_la_SOURCES = ct_accel.c
libct_accelhi_la_CFLAGS = @XORG_CFLAGS@ -DCHIPS_MMIO -DCHIPS_HIQV

# this is obnoxious:
# -module lets us name the module exactly how we want
# -avoid-version prevents gratuitous .0.0.0 version numbers on the end
# _ladir passes a dummy rpath to libtool so the thing will actually link
# TODO: -nostdlib/-Bstatic/-lgcc platform magic, not installing the .a, etc.
AM_CFLAGS = @XORG_CFLAGS@
chips_drv_la_LTLIBRARIES = chips_drv.la
chips_drv_la_LDFLAGS = -module -avoid-version
chips_drv_ladir = @moduledir@/drivers
chips_drv_la_LIBADD = $(noinst_LTLIBRARIES)

chips_drv_la_SOURCES = \
         ct_accel.c \
         ct_bank.c \
         ct_BlitMM.h \
         ct_Blitter.h \
         ct_BltHiQV.h \
         ct_cursor.c \
         ct_ddc.c \
         ct_dga.c \
         ct_driver.c \
         ct_driver.h \
	 ct_pcirename.h \
         ct_regs.c \
         ct_shadow.c \
         ct_video.c \
         compat-api.h
