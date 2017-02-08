#	$OpenBSD: Makefile,v 1.10 2017/02/08 21:33:42 danj Exp $

XKC_TOP =		../../..
PACKAGE_VERSION =	2.20
PKGCONFIG =		xkeyboard-config.pc
EXTRA_PKGCONFIG_SUBST =	-e 's,@VERSION@,${PACKAGE_VERSION},g' \
			-e 's,@xkb_base@,${XKB_BASE},g'

install: install-pc

.include <bsd.xorg.mk>
