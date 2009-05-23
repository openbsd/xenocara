#	$OpenBSD: Makefile,v 1.1.1.1 2009/05/23 14:43:12 matthieu Exp $

LIB=		xcb-render-util

SUBLIB=		renderutil

SRCS=		cache.c glyph.c util.c

HEADERS=	xcb_renderutil.h

LDADD!=		pkg-config --libs xcb xcb-render

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
