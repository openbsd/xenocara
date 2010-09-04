SUBDIRS = src man

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xi.pc

EXTRA_DIST = xi.pc.in ChangeLog

CLEANFILES = ChangeLog

.PHONY: ChangeLog INSTALL

INSTALL:
	$(INSTALL_CMD)

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog INSTALL
