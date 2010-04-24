videodir = $(includedir)/X11/extensions
video_HEADERS = \
	vldXvMC.h \
	Xv.h \
	XvMC.h \
	XvMCproto.h \
	Xvproto.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = videoproto.pc

EXTRA_DIST = videoproto.pc.in

EXTRA_DIST += ChangeLog
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
