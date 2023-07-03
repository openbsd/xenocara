ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src

pkgconfig_DATA =	\
	xcb-atom.pc	\
	xcb-aux.pc	\
	xcb-event.pc	\
	xcb-util.pc

MAINTAINERCLEANFILES = ChangeLog INSTALL

.PHONY: ChangeLog INSTALL

INSTALL:
	$(INSTALL_CMD)

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog INSTALL

EXTRA_DIST = autogen.sh README.md
