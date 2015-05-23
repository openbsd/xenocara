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
AM_CFLAGS = @XORG_CFLAGS@ $(CWARNFLAGS)

cirrus_drv_la_LTLIBRARIES = cirrus_drv.la
cirrus_drv_la_LDFLAGS = -module -avoid-version
cirrus_drv_ladir = @moduledir@/drivers
cirrus_drv_la_SOURCES = \
         cir_dga.c \
         cir_driver.c \
         cir.h \
         CirrusClk.c \
         cir_shadow.c \
         cir_pcirename.h \
         compat-api.h

cirrus_alpine_la_LTLIBRARIES = cirrus_alpine.la
cirrus_alpine_la_LDFLAGS = -module -avoid-version
cirrus_alpine_ladir = @moduledir@/drivers
cirrus_alpine_la_SOURCES = \
         alp_driver.c \
         alp.h \
         alp_hwcurs.c \
         alp_i2c.c
if XAA
cirrus_alpine_la_SOURCES += \
         alp_xaa.c \
         alp_xaam.c
endif
if USE_EXA
cirrus_alpine_la_SOURCES += \
         alp_exa.c
endif

cirrus_laguna_la_LTLIBRARIES = cirrus_laguna.la
cirrus_laguna_la_LDFLAGS = -module -avoid-version
cirrus_laguna_ladir = @moduledir@/drivers
cirrus_laguna_la_SOURCES = \
         lg_driver.c \
         lg.h \
         lg_hwcurs.c \
         lg_i2c.c
if XAA
cirrus_laguna_la_SOURCES += \
         lg_xaa.c \
         lg_xaa.h
endif
