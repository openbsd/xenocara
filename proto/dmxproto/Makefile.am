dmxdir = $(includedir)/X11/extensions
dmx_HEADERS = \
	dmx.h \
	dmxproto.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = dmxproto.pc

EXTRA_DIST = dmxproto.pc.in

EXTRA_DIST += ChangeLog
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
