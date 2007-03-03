SUBDIRS = man src

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xext.pc

EXTRA_DIST=xext.pc.in ChangeLog autogen.sh
MAINTAINERCLEANFILES=ChangeLog

ChangeLog:
	(GIT_DIR=$(top_srcdir)/.git git-log > .changelog.tmp && mv .changelog.tmp ChangeLog; rm -f .changelog.tmp) || (touch ChangeLog; echo 'git directory not found: installing possibly empty changelog.' >&2)

dist-hook: ChangeLog

