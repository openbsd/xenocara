# $OpenBSD: Makefile,v 1.3 2015/01/25 15:09:14 jsg Exp $
.include <bsd.xconf.mk>

SUBDIR= libmesa libmesautil

.if ${XENOCARA_BUILD_GALLIUM:L} == "yes" || ${XENOCARA_BUILD_GALLIUM} == "llvm"
SUBDIR+= libmesagallium
.endif

build depend all install clean cleandir: _SUBDIRUSE

.include <bsd.xorg.mk>
