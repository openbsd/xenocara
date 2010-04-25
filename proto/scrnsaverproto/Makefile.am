scrnsaverdir = $(includedir)/X11/extensions
scrnsaver_HEADERS = \
	saver.h \
	saverproto.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = scrnsaverproto.pc

EXTRA_DIST = scrnsaverproto.pc.in

EXTRA_DIST += ChangeLog
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
