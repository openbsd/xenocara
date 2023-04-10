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
nv_drv_la_LTLIBRARIES = nv_drv.la
nv_drv_la_LDFLAGS = -module -avoid-version
nv_drv_ladir = @moduledir@/drivers

nv_drv_la_SOURCES = $(nv_sources) $(riva_sources) $(g80_sources)

nv_sources = \
         compat-api.h \
         pci_ids.h \
         nv_const.h \
         nv_cursor.c \
         nv_dac.c \
         nv_dga.c \
         nv_dma.h \
         nv_driver.c \
         nv_hw.c \
         nv_include.h \
         nv_local.h \
         nv_proto.h \
         nv_setup.c \
         nv_shadow.c \
         nv_type.h \
         nv_video.c \
         nv_xaa.c

riva_sources = \
         riva_const.h \
         riva_cursor.c \
         riva_dac.c \
         riva_dga.c \
         riva_driver.c \
         riva_hw.c \
         riva_hw.h \
         riva_include.h \
         riva_local.h \
         riva_proto.h \
         riva_setup.c \
         riva_shadow.c \
         riva_tbl.h \
         riva_type.h \
         riva_xaa.c

g80_sources = \
         g80_cursor.c \
         g80_cursor.h \
         g80_dac.c \
         g80_display.c \
         g80_display.h \
         g80_dma.c \
         g80_dma.h \
         g80_driver.c \
         g80_exa.c \
         g80_exa.h \
         g80_output.c \
         g80_output.h \
         g80_sor.c \
         g80_type.h \
         g80_xaa.c \
         g80_xaa.h

