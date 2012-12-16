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

AM_CPPFLAGS = 				\
	-I$(top_srcdir)/src/cim 	\
	-I$(top_srcdir)/src/gfx 	\
	-I$(top_srcdir)/src/panel	\
	$(PANEL_CPPFLAGS)		\
	-DHAVE_GX			\
	-DHAVE_LX			\
	-DOPT_ACCEL

AM_CFLAGS = $(XORG_CFLAGS) $(CWARNFLAGS) $(M32_CFLAGS)

geode_drv_la_LTLIBRARIES = geode_drv.la
geode_drv_la_LDFLAGS = -module -avoid-version
geode_drv_ladir = $(moduledir)/drivers

geode_drv_la_SOURCES =		\
	cimarron.c		\
	cim_dev.h		\
	compat-api.h		\
	durango.c		\
	geode_blend.h		\
	geode_common.c		\
	geode_dcon.c		\
	geode_ddc.c		\
	geode_driver.c		\
	geode_fourcc.h		\
	geode.h			\
	geode_msr.c		\
	geode_pcirename.h	\
	gx_accel.c		\
	gx_cursor.c		\
	gx_driver.c		\
	gx_randr.c		\
	gx_regacc.c		\
	gx_rotate.c		\
	gx_video.c		\
	lx_cursor.c		\
	lx_display.c		\
	lx_driver.c		\
	lx_exa.c		\
	lx_memory.c		\
	lx_output.c		\
	lx_panel.c		\
	lx_video.c		\
	panel.c

EXTRA_DIST =			\
        cim/cim_defs.h		\
        cim/cim_df.c		\
        cim/cim_filter.c	\
        cim/cim_gp.c		\
        cim/cim_init.c		\
        cim/cim_modes.c		\
        cim/cim_msr.c		\
        cim/cim_parm.h		\
        cim/cim_regs.h		\
        cim/cim_rtns.h		\
        cim/cim_version.h	\
        cim/cim_vg.c		\
        cim/cim_vip.c		\
        cim/cim_vop.c		\
        cim/doc/release.txt	\
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
        gfx/gfx_priv.h		\
        gfx/gfx_regs.h		\
        gfx/gfx_rndr.c		\
        gfx/gfx_rtns.h		\
        gfx/gfx_tv.c		\
        gfx/gfx_type.h		\
        gfx/gfx_vga.c		\
        gfx/gfx_vid.c		\
        gfx/gfx_vip.c		\
        gfx/history.h		\
        gfx/i2c_acc.c		\
        gfx/i2c_gpio.c		\
        gfx/init_gu1.c		\
        gfx/init_gu2.c		\
        gfx/msr_rdcl.c		\
        gfx/release.txt		\
        gfx/rndr_gu1.c		\
        gfx/rndr_gu2.c		\
        gfx/saa7114.c		\
        gfx/tv_1200.c		\
        gfx/tv_fs450.c		\
        gfx/tv_fs450.h		\
        gfx/vga_gu1.c		\
        gfx/vid_1200.c		\
        gfx/vid_5530.c		\
        gfx/vid_rdcl.c		\
        gfx/vip_1200.c		\
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
        gx_vga.c		\
        lx_vga.c
