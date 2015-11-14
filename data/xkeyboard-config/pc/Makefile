#	$OpenBSD: Makefile,v 1.6 2015/11/14 15:30:01 shadchin Exp $

XKC_TOP =		../../..
PACKAGE_VERSION =	2.16
PKGCONFIG =		xkeyboard-config.pc
EXTRA_PKGCONFIG_SUBST =	-e 's,@VERSION@,${PACKAGE_VERSION},g' \
			-e 's,@xkb_base@,${XKB_BASE},g'

install: install-pc

.include <bsd.xorg.mk>
