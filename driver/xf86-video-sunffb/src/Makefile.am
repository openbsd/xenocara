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
sunffb_drv_la_LTLIBRARIES = sunffb_drv.la
sunffb_drv_la_LDFLAGS = -module -avoid-version
sunffb_drv_ladir = @moduledir@/drivers

if SPARC
AM_CFLAGS += -DUSE_VIS
SPARC_ASM_SRC = ffb_asm.s VISmoveImage.s
endif

sunffb_drv_la_SOURCES = \
         compat-api.h \
         ffb_accel.c \
         ffb_attr.c \
         ffb_cursor.c \
         ffb_dac.c \
         ffb_dac.h \
         ffb_ddc.c \
         ffb_dga.c \
         ffb_driver.c \
         ffb_fifo.h \
         ffb.h \
         ffb_loops.h \
         ffb_rcache.h \
         ffb_regs.h \
         ffb_wid.c \
         $(SPARC_ASM_SRC)
