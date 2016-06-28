#	$OpenBSD: Makefile,v 1.8 2016/06/28 16:49:22 shadchin Exp $

XKC_TOP =		../../..
PACKAGE_VERSION =	2.18
PKGCONFIG =		xkeyboard-config.pc
EXTRA_PKGCONFIG_SUBST =	-e 's,@VERSION@,${PACKAGE_VERSION},g' \
			-e 's,@xkb_base@,${XKB_BASE},g'

install: install-pc

.include <bsd.xorg.mk>
