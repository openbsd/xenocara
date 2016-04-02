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

ati_drv_la_LIBADD = $(PCIACCESS_LIBS)
radeon_drv_la_LIBADD = $(LIBDRM_RADEON_LIBS)

RADEON_KMS_SRCS=radeon_dri2.c radeon_dri3.c radeon_drm_queue.c radeon_kms.c \
	radeon_present.c radeon_sync.c radeon_vbo.c radeon_bo_helper.c \
	drmmode_display.c

RADEON_EXA_SOURCES = radeon_exa.c r600_exa.c r6xx_accel.c r600_textured_videofuncs.c r600_shader.c radeon_exa_shared.c \
	evergreen_exa.c evergreen_accel.c evergreen_shader.c evergreen_textured_videofuncs.c cayman_accel.c cayman_shader.c

AM_CFLAGS = \
            @LIBDRM_RADEON_CFLAGS@ \
            @XORG_CFLAGS@ \
            @LIBUDEV_CFLAGS@

if LIBUDEV
radeon_drv_la_LIBADD += $(LIBUDEV_LIBS)
endif

ati_drv_la_LTLIBRARIES = ati_drv.la
ati_drv_la_LDFLAGS = -module -avoid-version
ati_drv_ladir = @moduledir@/drivers
ati_drv_la_SOURCES = \
	ati.c atimodule.c

radeon_drv_la_LTLIBRARIES = radeon_drv.la
radeon_drv_la_LDFLAGS = -module -avoid-version
radeon_drv_ladir = @moduledir@/drivers
radeon_drv_la_SOURCES = \
	radeon_accel.c radeon_video.c \
	radeon_misc.c radeon_probe.c \
	radeon_textured_video.c radeon_xvmc.c \
	$(RADEON_EXA_SOURCES) \
	$(RADEON_KMS_SRCS)

if GLAMOR
AM_CFLAGS += @LIBGLAMOR_CFLAGS@
radeon_drv_la_LIBADD += @LIBGLAMOR_LIBS@
radeon_drv_la_SOURCES += \
	 radeon_glamor_wrappers.c \
	 radeon_glamor.c
endif

EXTRA_DIST = \
	radeon_textured_videofuncs.c \
	r600_reg.h \
	r600_reg_auto_r6xx.h \
	r600_reg_r6xx.h \
	r600_reg_r7xx.h \
	r600_shader.h \
	r600_state.h \
	evergreen_reg.h \
	evergreen_reg_auto.h \
	evergreen_shader.h \
	evergreen_state.h \
	cayman_reg.h \
	cayman_reg_auto.h \
	cayman_shader.h \
	compat-api.h \
	ati.h \
	ativersion.h \
	bicubic_table.h \
	bicubic_table.py \
	radeon_bo_helper.h \
	radeon_drm_queue.h \
	radeon_exa_render.c \
	radeon_exa_funcs.c \
	radeon_exa_shared.h \
	radeon_glamor.h \
	radeon.h \
	radeon_list.h \
	radeon_probe.h \
	radeon_reg.h \
	radeon_version.h \
	radeon_vbo.h \
	radeon_video.h \
	simple_list.h \
	atipciids.h \
	atipcirename.h \
	ati_pciids_gen.h \
	radeon_chipinfo_gen.h \
	radeon_chipset_gen.h \
	radeon_pci_chipset_gen.h \
	radeon_pci_device_match_gen.h \
	pcidb/ati_pciids.csv \
	pcidb/parse_pci_ids.pl \
	radeon_dri2.h \
	drmmode_display.h
