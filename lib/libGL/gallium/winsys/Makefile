# $OpenBSD: Makefile,v 1.2 2015/02/23 23:39:27 jsg Exp $

.if ${MACHINE} == i386 || ${MACHINE} == amd64
SUBDIR= radeon
.endif

build depend all install clean cleandir: _SUBDIRUSE

.include <bsd.xorg.mk>
