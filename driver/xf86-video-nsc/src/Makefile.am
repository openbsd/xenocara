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
AM_CFLAGS = @XORG_CFLAGS@ -I$(top_srcdir)/src/gfx -I$(top_srcdir)/src/panel
AM_CCASFLAGS = @XORG_CFLAGS@
nsc_drv_la_LTLIBRARIES = nsc_drv.la
nsc_drv_la_LDFLAGS = -module -avoid-version
nsc_drv_ladir = @moduledir@/drivers

nsc_drv_la_SOURCES = \
         durango.c \
         nsc_driver.c \
         nsc_fourcc.h \
         nsc_galstub.c \
         nsc_gx1_accel.c \
         nsc_gx1_cursor.c \
         nsc_gx1_dga.c \
         nsc_gx1_driver.c \
         nsc_gx1_shadow.c \
         nsc_gx1_video.c \
         nsc_gx2_accel.c \
         nsc_gx2_cursor.c \
         nsc_gx2_dga.c \
         nsc_gx2_driver.c \
         nsc_gx2_shadow.c \
         nsc_gx2_video.c \
         nsc.h \
         panel.c

if I386ARCH
nsc_drv_la_SOURCES += \
	nsc_msr_asm.S
endif

EXTRA_DIST =			\
        nsc_galfns.c		\
        nsc_gx2_vga.c		\
        nsc_regacc.c		\
				\
	panel/92xx.h		\
	panel/cen9211.c		\
	panel/cen9211.h		\
	panel/dora9211.c	\
	panel/dora9211.h	\
	panel/drac9210.c	\
	panel/drac9210.h	\
	panel/gx2_9211.c	\
	panel/gx2_9211.h	\
	panel/panel.c		\
	panel/panel.h		\
	panel/platform.c	\
	panel/pnl_bios.c	\
	panel/pnl_defs.h	\
	panel/pnl_init.c	\
	panel/readme.txt	\
				\
	gfx/disp_gu1.c		\
	gfx/disp_gu2.c		\
	gfx/gfx_dcdr.c		\
	gfx/gfx_defs.h		\
	gfx/gfx_disp.c		\
	gfx/gfx_i2c.c		\
	gfx/gfx_init.c		\
	gfx/gfx_mode.h		\
	gfx/gfx_msr.c		\
	gfx/gfx_regs.h		\
	gfx/gfx_rndr.c		\
	gfx/gfx_rtns.h		\
	gfx/gfx_tv.c		\
	gfx/gfx_type.h		\
	gfx/gfx_vga.c		\
	gfx/gfx_vid.c		\
	gfx/gfx_vip.c		\
	gfx/i2c_acc.c		\
	gfx/i2c_gpio.c		\
	gfx/init_gu1.c		\
	gfx/init_gu2.c		\
	gfx/msr_rdcl.c		\
	gfx/rndr_gu1.c		\
	gfx/rndr_gu2.c		\
	gfx/saa7114.c		\
	gfx/tv_1200.c		\
	gfx/vga_gu1.c		\
	gfx/vid_1200.c		\
	gfx/vid_5530.c		\
	gfx/vid_rdcl.c		\
	gfx/vip_1200.c		\
				\
	gfx/durango.c		\
	gfx/gfx_tv.h		\
	gfx/history.h		\
	gfx/release.txt		\
	gfx/tv_fs450.c		\
	gfx/tv_fs450.h		\
	gfx/tv_fs451.c		\
	gfx/tv_geode.c		\
	gfx/vid_1400.c		\
	gfx/vip_1400.c


