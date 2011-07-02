resourcedir = $(includedir)/X11/extensions
resource_HEADERS = \
	XResproto.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = resourceproto.pc

dist_doc_DATA = resproto.txt

MAINTAINERCLEANFILES = ChangeLog INSTALL

.PHONY: ChangeLog INSTALL

INSTALL:
	$(INSTALL_CMD)

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog INSTALL
