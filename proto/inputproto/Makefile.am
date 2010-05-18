inputdir = $(includedir)/X11/extensions
input_HEADERS = \
	XI.h \
	XIproto.h \
	XI2.h \
	XI2proto.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = inputproto.pc

EXTRA_DIST = inputproto.pc.in

EXTRA_DIST += ChangeLog XI2proto.txt XIproto.txt
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
