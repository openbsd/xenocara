#	$OpenBSD: Makefile,v 1.5 2015/03/30 17:11:29 shadchin Exp $

XKC_TOP =		../../..
PACKAGE_VERSION =	2.14
PKGCONFIG =		xkeyboard-config.pc
EXTRA_PKGCONFIG_SUBST =	-e 's,@VERSION@,${PACKAGE_VERSION},g' \
			-e 's,@xkb_base@,${XKB_BASE},g'

install: install-pc

.include <bsd.xorg.mk>
