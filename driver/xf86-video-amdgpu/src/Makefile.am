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

amdgpu_drv_la_LIBADD = $(LIBDRM_AMDGPU_LIBS) $(GBM_LIBS)

AMDGPU_KMS_SRCS=amdgpu_bo_helper.c amdgpu_dri2.c amdgpu_dri3.c amdgpu_drm_queue.c \
	amdgpu_kms.c amdgpu_present.c amdgpu_sync.c drmmode_display.c

AM_CFLAGS = \
            @GBM_CFLAGS@ \
            @LIBDRM_AMDGPU_CFLAGS@ \
            @XORG_CFLAGS@ \
            @LIBUDEV_CFLAGS@

if LIBUDEV
amdgpu_drv_la_LIBADD += $(LIBUDEV_LIBS)
endif

amdgpu_drv_la_LTLIBRARIES = amdgpu_drv.la
amdgpu_drv_la_LDFLAGS = -module -avoid-version
amdgpu_drv_ladir = @moduledir@/drivers
amdgpu_drv_la_SOURCES = \
	amdgpu_video.c \
	amdgpu_misc.c amdgpu_probe.c \
	$(AMDGPU_KMS_SRCS)

AM_CFLAGS += @LIBGLAMOR_CFLAGS@
amdgpu_drv_la_LIBADD += @LIBGLAMOR_LIBS@
amdgpu_drv_la_SOURCES += \
	amdgpu_glamor.c \
	amdgpu_glamor_wrappers.c \
	amdgpu_pixmap.c

EXTRA_DIST = \
	compat-api.h \
	amdgpu_bo_helper.h \
	amdgpu_drm_queue.h \
	amdgpu_glamor.h \
	amdgpu_drv.h \
	amdgpu_pixmap.h \
	amdgpu_probe.h \
	amdgpu_version.h \
	amdgpu_video.h \
	simple_list.h \
	amdgpu_dri2.h \
	drmmode_display.h
