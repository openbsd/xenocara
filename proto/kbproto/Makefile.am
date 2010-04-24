kbdir = $(includedir)/X11/extensions
kb_HEADERS = \
	XKBgeom.h \
	XKB.h \
	XKBproto.h \
	XKBsrv.h \
	XKBstr.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = kbproto.pc

EXTRA_DIST = kbproto.pc.in

EXTRA_DIST += ChangeLog
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
