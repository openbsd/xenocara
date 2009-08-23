SUBDIRS=src

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = sm.pc

EXTRA_DIST = sm.pc.in ChangeLog
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
