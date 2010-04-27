xineramadir = $(includedir)/X11/extensions
xinerama_HEADERS = \
	panoramiXproto.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xineramaproto.pc

EXTRA_DIST = xineramaproto.pc.in

EXTRA_DIST += ChangeLog
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
