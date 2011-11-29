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

SUBDIRS = xvmc ch7017 ch7xxx ivch sil164 tfp410 render_program legacy

# this is obnoxious:
# -module lets us name the module exactly how we want
# -avoid-version prevents gratuitous .0.0.0 version numbers on the end
# _ladir passes a dummy rpath to libtool so the thing will actually link
# TODO: -nostdlib/-Bstatic/-lgcc platform magic, not installing the .a, etc.

AM_CFLAGS = @CWARNFLAGS@ @XORG_CFLAGS@ @DRM_CFLAGS@ @DRI_CFLAGS@ \
	@PCIACCESS_CFLAGS@ -I$(top_srcdir)/uxa -I$(top_srcdir)/src/render_program

intel_drv_la_LTLIBRARIES = intel_drv.la
intel_drv_la_LDFLAGS = -module -avoid-version
intel_drv_ladir = @moduledir@/drivers
intel_drv_la_LIBADD = -lm @DRM_LIBS@ -ldrm_intel ../uxa/libuxa.la legacy/liblegacy.la
intel_drv_la_LIBADD += @PCIACCESS_LIBS@

NULL:=#

intel_drv_la_SOURCES = \
         brw_defines.h \
         brw_structs.h \
         common.h \
	 i2c_vid.h \
         intel.h \
	 intel_module.c \
         intel_driver.c \
         intel_batchbuffer.c \
         intel_batchbuffer.h \
         intel_memory.c \
	 intel_shadow.c \
	 intel_uxa.c \
         intel_video.c \
         intel_video.h \
	 i830_3d.c \
         i830_bios.c \
         i830_bios.h \
         i830_common.h \
	 i830_crt.c \
         i830_cursor.c \
	 i830_display.c \
	 i830_display.h \
         i830_quirks.c \
	 i830_dvo.c \
         i830.h \
	 i830_hdmi.c \
         i830_i2c.c \
         i830_io.c \
	 i830_lvds.c \
         i830_modes.c \
	 i830_render.c \
	 i830_reg.h \
         i830_ring.h \
	 i830_sdvo.c \
	 i830_sdvo.h \
	 i830_sdvo_regs.h \
	 i830_tv.c \
	 i915_3d.c \
	 i915_3d.h \
	 i915_reg.h \
	 i915_render.c \
	 i915_video.c \
	 i965_reg.h \
	 i965_3d.c \
	 i965_video.c \
	 i965_render.c \
	 drmmode_display.c \
	 $(NULL)

if DRI
intel_drv_la_SOURCES += \
	intel_dri.c \
	$(NULL)
intel_drv_la_LIBADD += \
	$(DRI_LIBS) \
	$(NULL)
endif

if XVMC
intel_drv_la_SOURCES += \
	intel_hwmc.h \
	intel_hwmc.c \
	$(NULL)
endif
