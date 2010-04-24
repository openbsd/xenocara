xf86dridir = $(includedir)/X11/dri
xf86dri_HEADERS = xf86dri.h xf86dristr.h xf86driproto.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xf86driproto.pc

EXTRA_DIST = xf86driproto.pc.in

EXTRA_DIST += ChangeLog
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
