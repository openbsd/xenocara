#	$OpenBSD: Makefile,v 1.2 2009/05/31 18:01:28 matthieu Exp $

SUBLIB=		image

SRCS=		xcb_image.c

HEADERS=	xcb_image.h xcb_pixel.h

LDADD=		`pkg-config --libs xcb xcb-shm xcb-aux`

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
