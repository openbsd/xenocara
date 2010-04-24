bigreqsdir = $(includedir)/X11/extensions
bigreqs_HEADERS = \
	bigreqsproto.h \
	bigreqstr.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = bigreqsproto.pc

EXTRA_DIST = bigreqsproto.pc.in

EXTRA_DIST += ChangeLog
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
