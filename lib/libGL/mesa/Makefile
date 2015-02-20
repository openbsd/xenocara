# $OpenBSD: Makefile,v 1.4 2015/02/20 23:31:06 jsg Exp $
.include <bsd.xconf.mk>

SUBDIR= libmesa

.if ${XENOCARA_BUILD_GALLIUM:L} == "yes" || ${XENOCARA_BUILD_GALLIUM} == "llvm"
SUBDIR+= libmesagallium
.endif

build depend all install clean cleandir: _SUBDIRUSE

.include <bsd.xorg.mk>
