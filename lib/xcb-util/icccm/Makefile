#	$OpenBSD: Makefile,v 1.2 2009/05/31 18:01:28 matthieu Exp $

SUBLIB=		icccm

SRCS=		icccm.c

CPPFLAGS=	-I${XCBUTIL}/property -I${XCBUTIL}/event -I${XCBUTIL}/atom

HEADERS=	xcb_icccm.h

LDADD=		`pkg-config --libs xcb-atom xcb-event xcb-property`

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

