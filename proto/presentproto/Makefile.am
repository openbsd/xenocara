presentdir = $(includedir)/X11/extensions
present_HEADERS = presentproto.h presenttokens.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = presentproto.pc

dist_doc_DATA = presentproto.txt

MAINTAINERCLEANFILES = ChangeLog INSTALL

.PHONY: ChangeLog INSTALL

INSTALL:
	$(INSTALL_CMD)

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog INSTALL
