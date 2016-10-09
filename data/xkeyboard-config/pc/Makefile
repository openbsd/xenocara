#	$OpenBSD: Makefile,v 1.9 2016/10/09 11:49:40 shadchin Exp $

XKC_TOP =		../../..
PACKAGE_VERSION =	2.19
PKGCONFIG =		xkeyboard-config.pc
EXTRA_PKGCONFIG_SUBST =	-e 's,@VERSION@,${PACKAGE_VERSION},g' \
			-e 's,@xkb_base@,${XKB_BASE},g'

install: install-pc

.include <bsd.xorg.mk>
