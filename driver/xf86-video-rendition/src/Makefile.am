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
AM_CFLAGS = @XORG_CFLAGS@ -DMODULEDIR=\"@moduledir@\"
rendition_drv_la_LTLIBRARIES = rendition_drv.la
rendition_drv_la_LDFLAGS = -module -avoid-version
rendition_drv_ladir = @moduledir@/drivers

rendition_drv_la_SOURCES = \
         cmd2d.h \
         commonregs.h \
         compat-api.h \
         cscode.h \
         hwcursor.c \
         hwcursor.h \
         rendition.c \
         rendition.h \
         rendition_options.h \
         rendition_shadow.c \
         rendition_shadow.h \
         v1kregs.h \
         v1krisc.c \
         v1krisc.h \
         v2kregs.h \
         vboard.c \
         vboard.h \
         vloaduc.c \
         vloaduc.h \
         vmisc.c \
         vmisc.h \
         vmodes.c \
         vmodes.h \
         vos.h \
         vramdac.c \
         vramdac.h \
         vtypes.h

driverdir = @moduledir@
dist_driver_DATA = \
	v10002d.uc		\
	v20002d.uc

EXTRA_DIST = \
        accel.h \
        accelX.c \
	README.uc \
	vgafont-std.data \
	vgafont-vrx.data \
	vgapalette.data
                        
