resourcedir = $(includedir)/X11/extensions
resource_HEADERS = \
	XResproto.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = resourceproto.pc

EXTRA_DIST = resourceproto.pc.in

EXTRA_DIST += ChangeLog
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
