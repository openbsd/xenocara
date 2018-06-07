dri3dir = $(includedir)/X11/extensions
dri3_HEADERS = dri3proto.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = dri3proto.pc

dist_doc_DATA = dri3proto.txt

MAINTAINERCLEANFILES = ChangeLog INSTALL

.PHONY: ChangeLog INSTALL

INSTALL:
	$(INSTALL_CMD)

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog INSTALL
