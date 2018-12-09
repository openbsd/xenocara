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

SUBDIRS = xvmc

BUILT_SOURCES = version.h
EXTRA_DIST = version.h
CONFIG_CLEAN_FILES= version.h
AM_CFLAGS = @XORG_CFLAGS@ @LIBUDEV_CFLAGS@ @DRI_CFLAGS@
openchrome_drv_la_LTLIBRARIES = openchrome_drv.la
openchrome_drv_la_LDFLAGS = -module -avoid-version @LIBUDEV_LIBS@
openchrome_drv_ladir = @moduledir@/drivers

openchrome_drv_la_SOURCES = \
         compat-api.h \
         via_eng_regs.h \
         via_3d.c \
         via_3d.h \
         via_3d_reg.h \
         via_analog.c \
         via_rop.h \
         via_exa.c \
         via_exa_h2.c \
         via_exa_h6.c \
         via_ums.h \
         via_ch7xxx.c \
         via_ch7xxx.h \
         via_display.c \
         via_dmabuffer.h \
         via_driver.c \
         via_driver.h \
         via_i2c.c \
         via_fp.c \
         via_fp.h \
         via_memcpy.c \
         via_memmgr.h \
         via_memmgr.c \
         via_outputs.c \
         via_priv.h \
         via_regs.h \
         via_sii164.c \
         via_sii164.h \
         via_tmds.c \
         via_tv.c \
         via_xv_overlay.c \
         via_xv_overlay.h \
         via_ums.c \
         via_vgahw.c \
         via_vgahw.h \
         via_xv.c \
         via_xv.h \
         via_vt162x.c \
         via_vt162x.h \
         via_vt1632.c \
         via_vt1632.h \
         via_xvpriv.h

if DRI
openchrome_drv_la_SOURCES += \
         via_dri.c \
         via_dri.h \
         via_drm.h \
         via_drmclient.h \
         via_kms.h \
         via_kms.c \
         via_xvmc.c \
         via_xvmc.h \
         drm_fourcc.h
else
EXTRA_DIST += \
         via_dri.c \
         via_dri.h \
         via_drm.h \
         via_drmclient.h \
         via_kms.h \
         via_kms.c \
         via_xvmc.c \
         via_xvmc.h \
         drm_fourcc.h
endif

via_driver.lo: version.h
version.h: $(openchrome_drv_la_SOURCES)
	@if [ -d ../.git ]; then \
        echo '#define BUILDCOMMENT "(development build, branch' \
                     "`git branch -v | sed 's/no branch/no_branch/' | \
                      awk '/^\*/ { print $$2 }'`" \
                     'at revision' \
                     "`git log -1 --pretty=format:%h | head -1`"')"' \
                    > $@.tmp; \
    else \
        if [ -d .svn ]; then \
	        echo '#define BUILDCOMMENT "(development build, at revision '\
	                "`svnversion -nc .. | sed -e s/^[^:]*://`"')"' > $@.tmp; \
        elif [ "$$SOURCE_DATE_EPOCH" ]; then \
	        printf '#define BUILDCOMMENT "(compiled with SOURCE_DATE_EPOCH: %s)"' $$SOURCE_DATE_EPOCH \
        	> $@.tmp; \
        else \
	        date +'#define BUILDCOMMENT "(development build, compiled on %c)"' \
        	> $@.tmp; \
	    fi; \
    fi

	@chmod 666 $@.tmp
	@mv $@.tmp $@
	@echo "created $@"

dist-hook:
	@[ -e version.h ] && rm version.h; \
	echo '#define BUILDCOMMENT "(openchrome '$(VERSION)' release)"' \
	> version.h
