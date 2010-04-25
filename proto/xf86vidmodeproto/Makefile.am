xf86vidmodedir = $(includedir)/X11/extensions
xf86vidmode_HEADERS = \
	xf86vm.h \
	xf86vmproto.h \
	xf86vmstr.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xf86vidmodeproto.pc

EXTRA_DIST = xf86vidmodeproto.pc.in

EXTRA_DIST += ChangeLog
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
