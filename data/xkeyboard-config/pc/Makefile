#	$OpenBSD: Makefile,v 1.4 2014/12/28 16:41:04 shadchin Exp $

XKC_TOP =		../../..
PACKAGE_VERSION =	2.13
PKGCONFIG =		xkeyboard-config.pc
EXTRA_PKGCONFIG_SUBST =	-e 's,@VERSION@,${PACKAGE_VERSION},g' \
			-e 's,@xkb_base@,${XKB_BASE},g'

install: install-pc

.include <bsd.xorg.mk>
