# $OpenBSD: Makefile,v 1.5 2011/03/08 07:59:23 matthieu Exp $

# bitmaps needed for building apps, so has to be done separately first

.include <bsd.xconf.mk>
.if ${XENOCARA_USE_XKEYBOARD_CONFIG:L} == "yes"
SUBDIR= xkeyboard-config
.else
SUBDIR= xkbdata
.endif

build: _SUBDIRUSE

.include <bsd.subdir.mk>
