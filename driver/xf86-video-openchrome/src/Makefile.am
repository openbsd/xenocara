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

BUILT_SOURCES = version.h
EXTRA_DIST = version.h
CONFIG_CLEAN_FILES= version.h
AM_CFLAGS = @XORG_CFLAGS@ @DRI_CFLAGS@
openchrome_drv_la_LTLIBRARIES = openchrome_drv.la
openchrome_drv_la_LDFLAGS = -module -avoid-version
openchrome_drv_ladir = @moduledir@/drivers

openchrome_drv_la_SOURCES = \
         compat-api.h \
         via.h \
         via_3d.c \
         via_3d.h \
         via_3d_reg.h \
         via_rop.h \
         via_accel.c \
         via_bandwidth.c \
         via_bios.h \
         via_ch7xxx.c \
         via_ch7xxx.h \
         via_cursor.c \
         via_crtc.c \
         via_dga.c \
         via_display.c \
         via_dmabuffer.h \
         via_driver.c \
         via_driver.h \
         via_i2c.c \
         via_id.c \
         via_id.h \
         via_lvds.c \
         via_memcpy.c \
         via_memcpy.h \
         via_memory.c \
         via_mode.c \
         via_mode.h \
         via_panel.c \
         via_priv.h \
         via_regs.h \
         via_shadow.c \
         via_swov.c \
         via_swov.h \
         via_timing.c \
         via_timing.h \
         via_vbe.c \
         via_vgahw.c \
         via_vgahw.h \
         via_video.c \
         via_video.h \
         via_vt162x.c \
         via_vt162x.h \
         via_xvpriv.h

if DRI
openchrome_drv_la_SOURCES += \
         via_dri.c \
         via_dri.h \
         via_drmclient.h \
         via_xvmc.c \
         via_xvmc.h 
else
EXTRA_DIST += \
         via_dri.c \
         via_dri.h \
         via_drmclient.h \
         via_xvmc.c \
         via_xvmc.h 
endif


via_driver.lo: version.h
version.h: $(openchrome_drv_la_SOURCES)
	@if [ -d ../.git ]; then \
	    echo '#define BUILDCOMMENT "(developement build, at revision '\
        "`git log -1 --pretty=format:%h | head -1`"')"' > $@.tmp; \
    else \
        if [ -d .svn ]; then \
	        echo '#define BUILDCOMMENT "(development build, at revision '\
	        "`svnversion -nc .. | sed -e s/^[^:]*://`"')"' > $@.tmp; \
        else \
	        date +'#define BUILDCOMMENT "(development build, compiled on %c)"' \
        	> $@.tmp; \
	    fi; \
    fi

	@chmod 666 $@.tmp
	@mv $@.tmp $@
	@echo "created $@"

dist-hook:
	rm version.h;
	echo '#define BUILDCOMMENT "(openchrome '$(VERSION)' release)"' \
	> version.h
