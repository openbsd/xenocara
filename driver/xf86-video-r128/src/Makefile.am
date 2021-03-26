#  Copyright 2005 Adam Jackson.
#  Copyright 2005 Red Hat, Inc.
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

if DRI
R128_DRI_SRCS = r128_dri.c
endif

if USE_EXA
R128_EXA_SRCS = \
	r128_exa.c \
	r128_exa_render.c
endif

AM_CFLAGS = @XORG_CFLAGS@ @DRI_CFLAGS@

r128_drv_la_LTLIBRARIES = r128_drv.la
r128_drv_la_LDFLAGS = -module -avoid-version
r128_drv_ladir = @moduledir@/drivers
r128_drv_la_SOURCES = \
	r128_accel.c \
	r128_crtc.c \
	r128_cursor.c \
	r128_driver.c \
	r128_misc.c \
	r128_output.c \
	r128_probe.c \
	r128_video.c \
	$(R128_EXA_SRCS) \
	$(R128_DRI_SRCS)

EXTRA_DIST = \
	compat-api.h \
	r128.h \
	r128_common.h \
	r128_dri.h \
	r128_dripriv.h \
	r128_probe.h \
	r128_reg.h \
	r128_rop.h \
	r128_sarea.h \
	r128_version.h \
	atipcirename.h
