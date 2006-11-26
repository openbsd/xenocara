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
ATIMISC_DRI_SRCS = atidri.c
R128_DRI_SRCS = r128_dri.c
RADEON_DRI_SRCS = radeon_dri.c
endif

if ATIMISC_CPIO
ATI_CPIO_SOURCES = ativgaio.c
ATIMISC_CPIO_SOURCES = ativga.c atibank.c atiwonder.c atiwonderio.c
endif

if ATIMISC_DGA
ATIMISC_DGA_SOURCES = atidga.c
endif

if USE_EXA
ATIMISC_EXA_SOURCES = atimach64exa.c
RADEON_EXA_SOURCES = radeon_exa.c
endif

AM_CFLAGS = @XORG_CFLAGS@ @DRI_CFLAGS@

ati_drv_la_LTLIBRARIES = ati_drv.la
ati_drv_la_LDFLAGS = -module -avoid-version
ati_drv_ladir = @moduledir@/drivers
ati_drv_la_SOURCES = \
	ati.c atiadapter.c atibus.c atichip.c atiident.c atioption.c \
	atiprobe.c atividmem.c atimodule.c $(ATI_CPIO_SOURCES) \
	radeon_probe.c r128_probe.c

atimisc_drv_la_LTLIBRARIES = atimisc_drv.la
atimisc_drv_la_LDFLAGS = -module -avoid-version
atimisc_drv_ladir = @moduledir@/drivers
atimisc_drv_la_SOURCES = \
	atiaccel.c atiadjust.c atiaudio.c aticlock.c aticonfig.c aticonsole.c \
	aticursor.c atidac.c atidecoder.c atidsp.c atifillin.c atii2c.c \
	atilock.c atimach64.c atimach64accel.c atimach64cursor.c \
	atimach64i2c.c atimach64io.c atimach64xv.c atimode.c atipreinit.c \
	atiprint.c atirgb514.c atiscreen.c atituner.c atiutil.c ativalid.c \
	atixv.c atiload.c atimisc.c $(ATIMISC_DRI_SRCS) $(ATIMISC_DGA_SOURCES) \
	$(ATIMISC_CPIO_SOURCES) $(ATIMISC_EXA_SOURCES)

r128_drv_la_LTLIBRARIES = r128_drv.la
r128_drv_la_LDFLAGS = -module -avoid-version
r128_drv_ladir = @moduledir@/drivers
r128_drv_la_SOURCES = \
	r128_accel.c r128_cursor.c r128_dga.c r128_driver.c \
	r128_video.c r128_misc.c $(R128_DRI_SRCS)

radeon_drv_la_LTLIBRARIES = radeon_drv.la
radeon_drv_la_LDFLAGS = -module -avoid-version
radeon_drv_ladir = @moduledir@/drivers
radeon_drv_la_SOURCES = \
	radeon_accel.c radeon_mergedfb.c radeon_cursor.c radeon_dga.c \
	radeon_driver.c radeon_video.c radeon_bios.c radeon_mm_i2c.c \
	radeon_vip.c radeon_misc.c $(RADEON_DRI_SRCS) $(RADEON_EXA_SOURCES)

theatre_detect_drv_la_LTLIBRARIES = theatre_detect_drv.la
theatre_detect_drv_la_LDFLAGS = -module -avoid-version
theatre_detect_drv_ladir = @moduledir@/multimedia
theatre_detect_drv_la_SOURCES = \
	theatre_detect.c theatre_detect_module.c

theatre_drv_la_LTLIBRARIES = theatre_drv.la
theatre_drv_la_LDFLAGS = -module -avoid-version
theatre_drv_ladir = @moduledir@/multimedia

theatre_drv_la_SOURCES = \
	theatre.c theatre_module.c

theatre200_drv_la_LTLIBRARIES = theatre200_drv.la
theatre200_drv_la_LDFLAGS = -module -avoid-version
theatre200_drv_ladir = @moduledir@/multimedia
theatre200_drv_la_CFLAGS = \
	$(AM_CFLAGS) -DMICROC_DIR=\"$(theatre200_drv_ladir)\"
theatre200_drv_la_SOURCES = \
	theatre200.c theatre200_module.c

EXTRA_DIST = \
	radeon_render.c \
	radeon_accelfuncs.c \
	\
	atiaccel.h \
	atiadapter.h \
	atiadjust.h \
	atiaudio.h \
	atibank.h \
	atibus.h \
	atichip.h \
	aticlock.h \
	aticonfig.h \
	aticonsole.h \
	aticrtc.h \
	aticursor.h \
	atidac.h \
	atidecoder.h \
	atidga.h \
	atidri.h \
	atidripriv.h \
	atidsp.h \
	atifillin.h \
	ati.h \
	atii2c.h \
	atiident.h \
	atiio.h \
	atiload.h \
	atilock.h \
	atimach64accel.h \
	atimach64cursor.h \
	atimach64.h \
	atimach64i2c.h \
	atimach64io.h \
	atimach64render.c \
	atimach64xv.h \
	atimode.h \
	atimodule.h \
	atimono.h \
	atioption.h \
	atipreinit.h \
	atiprint.h \
	atipriv.h \
	atiprobe.h \
	atiregs.h \
	atirgb514.h \
	atiscreen.h \
	atistruct.h \
	atituner.h \
	atiutil.h \
	ativalid.h \
	ativersion.h \
	ativga.h \
	ativgaio.h \
	atividmem.h \
	atiwonder.h \
	atiwonderio.h \
	atixv.h \
	generic_bus.h \
	mach64_common.h \
	mach64_dri.h \
	mach64_sarea.h \
	r128_chipset.h \
	r128_common.h \
	r128_dri.h \
	r128_dripriv.h \
	r128.h \
	r128_probe.h \
	r128_reg.h \
	r128_sarea.h \
	r128_version.h \
	radeon_chipset.h \
	radeon_common.h \
	radeon_commonfuncs.c \
	radeon_dri.h \
	radeon_dripriv.h \
	radeon_exa_render.c \
	radeon_exa_funcs.c \
	radeon.h \
	radeon_macros.h \
	radeon_mergedfb.h \
	radeon_probe.h \
	radeon_reg.h \
	radeon_sarea.h \
	radeon_version.h \
	radeon_video.h \
	theatre200.h \
	theatre_detect.h \
	theatre.h \
	theatre_reg.h \
	atipciids.h
