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
endif

if ATIMISC_CPIO
ATIMISC_CPIO_SOURCES = ativga.c ativgaio.c atibank.c atiwonder.c atiwonderio.c
endif

if ATIMISC_DGA
ATIMISC_DGA_SOURCES = atidga.c
endif

if USE_EXA
ATIMISC_EXA_SOURCES = atimach64exa.c
endif

AM_CFLAGS = $(CWARNFLAGS) @XORG_CFLAGS@ @DRI_CFLAGS@

mach64_drv_la_LTLIBRARIES = mach64_drv.la
mach64_drv_la_LDFLAGS = -module -avoid-version
mach64_drv_ladir = @moduledir@/drivers
mach64_drv_la_SOURCES = \
	atibus.c atichip.c atiprobe.c atividmem.c \
	atiadjust.c aticlock.c aticonfig.c aticonsole.c \
	atidac.c atidsp.c atii2c.c \
	atilock.c atimach64.c atimach64accel.c atimach64cursor.c \
	atimach64i2c.c atimach64io.c atimach64xv.c atimode.c atipreinit.c \
	atiprint.c atirgb514.c atiscreen.c atituner.c atiutil.c ativalid.c \
	atiload.c atimisc.c atimach64probe.c $(ATIMISC_CPIO_SOURCES) \
	$(ATIMISC_DGA_SOURCES) $(ATIMISC_DRI_SRCS) $(ATIMISC_EXA_SOURCES)

EXTRA_DIST = \
	atimach64render.c \
	\
	atiadjust.h \
	atiaudio.h \
	atibank.h \
	atibus.h \
	atichip.h \
	aticlock.h \
	aticonsole.h \
	aticrtc.h \
	aticursor.h \
	atidac.h \
	atidga.h \
	atidri.h \
	atidripriv.h \
	atidsp.h \
	ati.h \
	atii2c.h \
	atiload.h \
	atilock.h \
	atimach64accel.h \
	atimach64.h \
	atimach64i2c.h \
	atimach64io.h \
	atimach64probe.h \
	atimach64version.h \
	atimode.h \
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
	ativga.h \
	ativgaio.h \
	atividmem.h \
	atiwonder.h \
	atiwonderio.h \
	atixv.h \
	mach64_common.h \
	mach64_dri.h \
	mach64_sarea.h \
	atipcirename.h \
	compat-api.h
