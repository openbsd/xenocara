#	$OpenBSD: Makefile,v 1.2 2009/05/31 18:01:28 matthieu Exp $

LIB=		xcb-render-util

SUBLIB=		renderutil

SRCS=		cache.c glyph.c util.c

HEADERS=	xcb_renderutil.h

LDADD=		`pkg-config --libs xcb xcb-render`

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
