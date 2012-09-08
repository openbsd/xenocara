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
savage_drv_la_LTLIBRARIES = savage_drv.la
savage_drv_la_LDFLAGS = -module -avoid-version
savage_drv_ladir = @moduledir@/drivers

savage_drv_la_SOURCES = \
         compat-api.h \
         savage_accel.c \
	 savage_xaa.c \
	 savage_exa.c \
         savage_bci.h \
         savage_common.h \
         savage_cursor.c \
         savage_dga.c \
         savage_driver.c \
         savage_driver.h \
         savage_drm.h \
         savage_i2c.c \
         savage_pciids.h \
         savage_regs.h \
         savage_sarea.h \
         savage_shadow.c \
         savage_streams.c \
         savage_streams.h \
         savage_vbe.c \
         savage_vbe.h \
         savage_video.c

if DRI
savage_drv_la_SOURCES += \
         savage_hwmc.c \
         savage_dri.c \
         savage_dri.h \
         savage_dripriv.h 
endif
