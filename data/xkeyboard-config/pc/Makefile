#	$OpenBSD: Makefile,v 1.7 2016/02/06 17:50:06 shadchin Exp $

XKC_TOP =		../../..
PACKAGE_VERSION =	2.17
PKGCONFIG =		xkeyboard-config.pc
EXTRA_PKGCONFIG_SUBST =	-e 's,@VERSION@,${PACKAGE_VERSION},g' \
			-e 's,@xkb_base@,${XKB_BASE},g'

install: install-pc

.include <bsd.xorg.mk>
