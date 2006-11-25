SUBDIRS = src

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ice.pc

EXTRA_DIST=ice.pc.in ChangeLog
CLEANFILES = ChangeLog

ChangeLog: FORCE
	git-log > ChangeLog
FORCE: ChangeLog
