SUBDIRS = src man

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xxf86vm.pc

EXTRA_DIST = xxf86vm.pc.in ChangeLog
MAINTAINERCLEANFILES = ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog
