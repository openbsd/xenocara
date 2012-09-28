#	$OpenBSD: Makefile,v 1.1 2012/09/28 19:49:12 matthieu Exp $
 
XKC_TOP =		../../..
PACKAGE_VERSION =	2.6
PKGCONFIG =		xkeyboard-config.pc
EXTRA_PKGCONFIG_SUBST = -e 's,@VERSION@,${PACKAGE_VERSION},g' \
			-e 's,@xkb_base@,${XKB_BASE},g'

install: install-pc
 
.include <bsd.xorg.mk>
