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

SUBDIRS = xvmc

if DRI
OPENCHROME_DRI_SRCS = \
    via_dri.c \
    via_xvmc.c
endif

if XF86DRM_MODE
OPENCHROME_KMS_SRCS = \
    drmmode_display.c
endif

AM_CFLAGS = @XORG_CFLAGS@ $(CWARNFLAGS) @DRI_CFLAGS@ @LIBUDEV_CFLAGS@

openchrome_drv_la_LTLIBRARIES = openchrome_drv.la
openchrome_drv_la_LDFLAGS = -module -avoid-version
openchrome_drv_ladir = @moduledir@/drivers

openchrome_drv_la_SOURCES = \
    via_3d.c \
    via_analog.c \
    via_ch7xxx.c \
    via_display.c \
    via_driver.c \
    via_exa.c \
    via_exa_h2.c \
    via_exa_h6.c \
    via_fp.c \
    via_i2c.c \
    via_memcpy.c \
    via_memmgr.c \
    via_options.c \
    via_output.c \
    via_sii164.c \
    via_tmds.c \
    via_tv.c \
    via_ums.c \
    via_vgahw.c \
    via_vt162x.c \
    via_vt1632.c \
    via_xv.c \
    via_xv_overlay.c \
    $(OPENCHROME_DRI_SRCS) \
    $(OPENCHROME_KMS_SRCS)

EXTRA_DIST = \
    compat-api.h \
    drmmode_display.h \
    via_3d.h \
    via_3d_reg.h \
    via_ch7xxx.h \
    via_dmabuffer.h \
    via_dri.h \
    via_driver.h \
    via_drmclient.h \
    via_eng_regs.h \
    via_fp.h \
    via_memmgr.h \
    via_regs.h \
    via_rop.h \
    via_sii164.h \
    via_ums.h \
    via_vgahw.h \
    via_vt162x.h \
    via_vt1632.h \
    via_xv.h \
    via_xvpriv.h \
    via_xvmc.h \
    via_xv_overlay.h
