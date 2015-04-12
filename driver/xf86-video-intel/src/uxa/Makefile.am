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

AM_CFLAGS = @CWARNFLAGS@ $(XORG_CFLAGS) $(DRM_CFLAGS) $(PCIACCESS_CFLAGS)
AM_CFLAGS += $(UDEV_CFLAGS) $(DRM_CFLAGS) $(DRMINTEL_CFLAGS)
AM_CFLAGS += -I$(top_srcdir)/xvmc -I$(top_srcdir)/src -I$(top_srcdir)/src/render_program

noinst_LTLIBRARIES = libuxa.la
libuxa_la_LIBADD = $(UDEV_LIBS) $(DRMINTEL_LIBS) $(DRM_LIBS)
libuxa_la_SOURCES = \
	brw_defines.h \
	brw_structs.h \
	common.h \
	intel.h \
	intel_batchbuffer.c \
	intel_batchbuffer.h \
	intel_display.c \
	intel_driver.c \
	intel_glamor.h \
	intel_memory.c \
	intel_uxa.c \
	intel_video.c \
	intel_video.h \
	i830_3d.c \
	i830_render.c \
	i830_reg.h \
	i915_3d.h \
	i915_reg.h \
	i915_3d.c \
	i915_render.c \
	i915_video.c \
	i965_reg.h \
	i965_3d.c \
	i965_video.c \
	i965_render.c \
	uxa_module.h \
	uxa.c \
	uxa.h \
	uxa-accel.c \
	uxa-glamor.h \
	uxa-glyphs.c \
	uxa-render.c \
	uxa-priv.h \
	uxa-unaccel.c
	$(NULL)

if GLAMOR
AM_CFLAGS += $(LIBGLAMOR_CFLAGS)
libuxa_la_LIBADD += $(LIBGLAMOR_LIBS)
libuxa_la_SOURCES += \
	 intel_glamor.c \
	 $(NULL)
endif

if DRI2
AM_CFLAGS += $(DRI2_CFLAGS)
libuxa_la_SOURCES += \
	intel_dri.c \
	$(NULL)
libuxa_la_LIBADD += \
	$(DRI2_LIBS) \
	$(CLOCK_GETTIME_LIBS) \
	$(NULL)
endif

if DRI3
libuxa_la_SOURCES += \
	intel_dri3.c \
	intel_sync.c \
	$(NULL)
endif

if PRESENT
libuxa_la_SOURCES += \
	intel_present.c \
	$(NULL)
endif

if XVMC
AM_CFLAGS += -I$(top_srcdir)/xvmc
libuxa_la_SOURCES += \
	intel_hwmc.c \
	$(NULL)
endif
