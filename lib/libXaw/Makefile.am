SUBDIRS = include src man

aclocaldir = $(datadir)/aclocal
aclocal_DATA = xaw.m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = 

if BUILD_XAW6

pkgconfig_DATA += xaw6.pc
DEFAULT_LIB = libXaw6.so

endif

if BUILD_XAW7

pkgconfig_DATA += xaw7.pc
DEFAULT_LIB = libXaw7.so

endif

if BUILD_XAW8

pkgconfig_DATA += xaw8.pc
DEFAULT_LIB = libXaw8.so

endif

EXTRA_DIST = xaw6.pc.in \
	xaw7.pc.in \
	xaw8.pc.in \
	autogen.sh \
	xaw.m4 \
	old-doc/Changelog \
	old-doc/CHANGES \
	ChangeLog

MAINTAINERCLEANFILES=ChangeLog

.PHONY: ChangeLog

ChangeLog:
	(GIT_DIR=$(top_srcdir)/.git git-log > .changelog.tmp && mv .changelog.tmp ChangeLog; rm -f .changelog.tmp) || (touch ChangeLog; echo 'git directory not found: installing possibly empty changelog.' >&2)

dist-hook: ChangeLog
