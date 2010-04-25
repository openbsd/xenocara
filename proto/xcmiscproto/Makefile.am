xcmiscdir = $(includedir)/X11/extensions
xcmisc_HEADERS = \
	xcmiscproto.h \
	xcmiscstr.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xcmiscproto.pc

EXTRA_DIST = xcmiscproto.pc.in

EXTRA_DIST += ChangeLog
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
