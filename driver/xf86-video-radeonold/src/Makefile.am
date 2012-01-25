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

radeonold_drv_la_LIBADD =

if DRI
RADEON_DRI_SRCS = radeon_dri.c
radeonold_drv_la_LIBADD += $(DRI_LIBS)
endif

RADEON_ATOMBIOS_SOURCES = \
	AtomBios/CD_Operations.c \
	AtomBios/Decoder.c \
	AtomBios/hwserv_drv.c \
	AtomBios/includes/atombios.h \
        AtomBios/includes/CD_binding.h \
        AtomBios/includes/CD_Common_Types.h \
        AtomBios/includes/CD_Definitions.h \
        AtomBios/includes/CD_hw_services.h \
        AtomBios/includes/CD_Opcodes.h \
        AtomBios/includes/CD_Structs.h \
        AtomBios/includes/Decoder.h \
        AtomBios/includes/ObjectID.h \
        AtomBios/includes/regsdef.h


if USE_EXA
RADEON_EXA_SOURCES = radeon_exa.c r600_exa.c r6xx_accel.c r600_textured_videofuncs.c r600_shader.c
endif

AM_CFLAGS = @XORG_CFLAGS@ @DRI_CFLAGS@ @XMODES_CFLAGS@ -DDISABLE_EASF -DENABLE_ALL_SERVICE_FUNCTIONS -DATOM_BIOS -DATOM_BIOS_PARSER -DDRIVER_PARSER

INCLUDES = -I$(srcdir)/AtomBios/includes

if XSERVER_LIBPCIACCESS
radeonold_drv_la_LIBADD += $(PCIACCESS_LIBS)
endif

radeonold_drv_la_LTLIBRARIES = radeonold_drv.la
radeonold_drv_la_LDFLAGS = -module -avoid-version
radeonold_drv_ladir = @moduledir@/drivers
radeonold_drv_la_SOURCES = \
	radeon_accel.c radeon_cursor.c radeon_dga.c radeon_legacy_memory.c \
	radeon_driver.c radeon_video.c radeon_bios.c radeon_mm_i2c.c \
	radeon_vip.c radeon_misc.c radeon_probe.c \
	legacy_crtc.c legacy_output.c \
	radeon_textured_video.c \
	radeon_crtc.c radeon_output.c radeon_modes.c radeon_tv.c \
	$(RADEON_ATOMBIOS_SOURCES) radeon_atombios.c radeon_atomwrapper.c \
	$(RADEON_DRI_SRCS) $(RADEON_EXA_SOURCES) atombios_output.c atombios_crtc.c


#theatre_detect_drv_la_LTLIBRARIES = theatre_detect_drv.la
#theatre_detect_drv_la_LDFLAGS = -module -avoid-version
#theatre_detect_drv_ladir = @moduledir@/multimedia
#theatre_detect_drv_la_SOURCES = \
#	theatre_detect.c theatre_detect_module.c

#theatre_drv_la_LTLIBRARIES = theatre_drv.la
#theatre_drv_la_LDFLAGS = -module -avoid-version
#theatre_drv_ladir = @moduledir@/multimedia

#theatre_drv_la_SOURCES = \
#	theatre.c theatre_module.c

#theatre200_drv_la_LTLIBRARIES = theatre200_drv.la
#theatre200_drv_la_LDFLAGS = -module -avoid-version
#theatre200_drv_ladir = @moduledir@/multimedia
#theatre200_drv_la_CFLAGS = \
#	$(AM_CFLAGS) -DMICROC_DIR=\"$(theatre200_drv_ladir)\"
#theatre200_drv_la_SOURCES = \
#	theatre200.c theatre200_module.c

EXTRA_DIST = \
	radeon_render.c \
	radeon_accelfuncs.c \
	radeon_textured_videofuncs.c \
	r600_reg.h \
	r600_reg_auto_r6xx.h \
	r600_reg_r6xx.h \
	r600_reg_r7xx.h \
	r600_shader.h \
	r600_state.h \
	bicubic_table.h \
	generic_bus.h \
	radeon_commonfuncs.c \
	radeon_dri.h \
	radeon_drm.h \
	radeon_exa_render.c \
	radeon_exa_funcs.c \
	radeon.h \
	radeon_macros.h \
	radeon_probe.h \
	radeon_reg.h \
	radeon_version.h \
	radeon_video.h \
	radeon_tv.h \
        radeon_atomwrapper.h \
	theatre200.h \
	theatre_detect.h \
	theatre.h \
	theatre_reg.h \
	atipciids.h \
	atipcirename.h \
	ati_pciids_gen.h \
	radeon_chipinfo_gen.h \
	radeon_chipset_gen.h \
	radeon_pci_chipset_gen.h \
	radeon_pci_device_match_gen.h \
	pcidb/ati_pciids.csv \
	pcidb/parse_pci_ids.pl \
	radeon_atombios.h
