#	$OpenBSD: Makefile,v 1.2 2012/10/06 15:41:13 shadchin Exp $

XKC_TOP =		../../..
PACKAGE_VERSION =	2.7
PKGCONFIG =		xkeyboard-config.pc
EXTRA_PKGCONFIG_SUBST =	-e 's,@VERSION@,${PACKAGE_VERSION},g' \
			-e 's,@xkb_base@,${XKB_BASE},g'

install: install-pc

.include <bsd.xorg.mk>
