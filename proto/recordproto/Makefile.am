recorddir = $(includedir)/X11/extensions
record_HEADERS = \
	recordconst.h \
	recordproto.h \
	recordstr.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = recordproto.pc

EXTRA_DIST = recordproto.pc.in

EXTRA_DIST += ChangeLog
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
