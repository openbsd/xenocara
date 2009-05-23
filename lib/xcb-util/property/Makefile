#	$OpenBSD: Makefile,v 1.1.1.1 2009/05/23 14:43:12 matthieu Exp $

SUBLIB=		property

SRCS=		property.c

CPPFLAGS=	-I${XCBUTIL}/event

HEADERS=	xcb_property.h

LDADD!=		pkg-config --libs xcb xcb-event

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
