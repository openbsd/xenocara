#  Copyright 2008 VMware, Inc.
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
#  VMWARE, INC. BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
#  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
#  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

bin_PROGRAMS = @DRIVER_NAME@_detect

AM_CPPFLAGS = -I$(top_srcdir)/shared $(XORG_CFLAGS)

@DRIVER_NAME@_detect_SOURCES = vmmouse_detect.c vmmouse_udev.c vmmouse_iopl.c
@DRIVER_NAME@_detect_LDADD = $(top_builddir)/shared/lib@DRIVER_NAME@.la \
				@LIBUDEV_LIBS@
@DRIVER_NAME@_detect_CFLAGS = @LIBUDEV_CFLAGS@


calloutsdir=$(HAL_CALLOUTS_DIR)
callouts_SCRIPTS = hal-probe-vmmouse

hal-probe-vmmouse: hal-probe-vmmouse.in
	$(AM_V_GEN)$(SED) -e 's|__BIN_PREFIX__|$(bindir)|g' \
	    -e 's|__HAL_BIN_PREFIX__|$(HAL_BIN_DIR)|g' \
	    < $< > $@

CLEANFILES = hal-probe-vmmouse

if HAS_XORG_CONF_DIR

confdir=$(XORG_CONF_DIR)
dist_conf_DATA = 50-vmmouse.conf

endif

if HAS_UDEV_RULES_DIR
udevdir=$(UDEV_RULES_DIR)
udev_DATA = 69-xorg-vmmouse.rules

$(udev_DATA): $(udev_DATA).in
	$(AM_V_GEN)$(SED) -e 's|__BIN_PREFIX__|$(bindir)|g' < $< > $@

CLEANFILES += $(udev_DATA)

endif # HAS_UDEV_RULES_DIR

EXTRA_DIST = 69-xorg-vmmouse.rules.in hal-probe-vmmouse.in
