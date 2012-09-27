#	$OpenBSD: Makefile,v 1.4 2012/09/27 16:23:51 matthieu Exp $

SUBDIR =		compat geometry keycodes man rules symbols types
XKC_TOP =		../..
PACKAGE_VERSION =	2.6
PKGCONFIG =		xkeyboard-config.pc
EXTRA_PKGCONFIG_SUBST = -e 's,@VERSION@,${PACKAGE_VERSION},g' \
			-e 's,@xkb_base@,${XKB_BASE},g'

clean: _SUBDIRUSE

install: install-pc _SUBDIRUSE

.include "${.CURDIR}/Makefile.inc"
.include <bsd.xorg.mk>
.include <bsd.subdir.mk>
