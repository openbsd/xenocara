#
#  Copyright 2005  Red Hat, Inc.
#
#  Permission to use, copy, modify, distribute, and sell this software and its
#  documentation for any purpose is hereby granted without fee, provided that
#  the above copyright notice appear in all copies and that both that
#  copyright notice and this permission notice appear in supporting
#  documentation, and that the name of Red Hat not be used in
#  advertising or publicity pertaining to distribution of the software without
#  specific, written prior permission.  Red Hat makes no
#  representations about the suitability of this software for any purpose.  It
#  is provided "as is" without express or implied warranty.
#
#  RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
#  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
#  EVENT SHALL RED HAT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
#  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
#  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
#  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
#  PERFORMANCE OF THIS SOFTWARE.

SUBDIRS = app-defaults chooser config greeter include man xdm xdmshell

ACLOCAL_AMFLAGS = -I m4

# This ensures distcheck is performed in a location where user has write access
DISTCHECK_CONFIGURE_FLAGS = \
	--with-appdefaultdir=\$${datadir}/X11/app-defaults \
	--with-systemdsystemunitdir=\$${libdir}/systemd/system

if LINT
ALL_LINT_FLAGS=$(LINT_FLAGS) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
		$(AM_CPPFLAGS) $(CPPFLAGS)

lint:
	$(LINT) $(ALL_LINT_FLAGS) $(xdm_CFLAGS) $(xdm_SOURCES) $(xdm_LIBS)
	(cd xdm && $(MAKE) $(AM_MAKEFLAGS) lint)
	(cd xdmshell && $(MAKE) $(AM_MAKEFLAGS) lint)
	(cd greeter && $(MAKE) $(AM_MAKEFLAGS) lint)
	(cd chooser && $(MAKE) $(AM_MAKEFLAGS) lint)
endif LINT

if HAVE_SYSTEMD
systemdsystemunit_DATA = xdm.service

xdm.service: xdm.service.in Makefile
if USE_SYSTEMD_DAEMON
	$(AM_V_GEN)$(SED) -e 's|BINDIR|$(bindir)|g' < xdm.service.in > $@
else !USE_SYSTEMD_DAEMON
	$(AM_V_GEN)$(SED) -e 's|BINDIR|$(bindir)|g' -e '/[Nn]otify/d' < xdm.service.in > $@
endif !USE_SYSTEMD_DAEMON

endif HAVE_SYSTEMD
CLEANFILES = xdm.service
EXTRA_DIST = xdm.service.in

MAINTAINERCLEANFILES = ChangeLog INSTALL

.PHONY: ChangeLog INSTALL

INSTALL:
	$(INSTALL_CMD)

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog INSTALL
