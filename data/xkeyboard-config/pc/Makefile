#	$OpenBSD: Makefile,v 1.3 2014/09/17 16:11:50 shadchin Exp $

XKC_TOP =		../../..
PACKAGE_VERSION =	2.12
PKGCONFIG =		xkeyboard-config.pc
EXTRA_PKGCONFIG_SUBST =	-e 's,@VERSION@,${PACKAGE_VERSION},g' \
			-e 's,@xkb_base@,${XKB_BASE},g'

install: install-pc

.include <bsd.xorg.mk>
