#	$OpenBSD: Makefile,v 1.2 2009/05/31 18:01:28 matthieu Exp $

SUBLIB=		property

SRCS=		property.c

CPPFLAGS=	-I${XCBUTIL}/event

HEADERS=	xcb_property.h

LDADD=		`pkg-config --libs xcb xcb-event`

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
