#	$OpenBSD: Makefile,v 1.1.1.1 2009/05/23 14:43:12 matthieu Exp $

SUBLIB=		icccm

SRCS=		icccm.c

CPPFLAGS=	-I${XCBUTIL}/property -I${XCBUTIL}/event -I${XCBUTIL}/atom

HEADERS=	xcb_icccm.h

LDADD!=		pkg-config --libs xcb-atom xcb-event xcb-property

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

