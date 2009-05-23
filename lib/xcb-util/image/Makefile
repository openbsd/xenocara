#	$OpenBSD: Makefile,v 1.1.1.1 2009/05/23 14:43:12 matthieu Exp $

SUBLIB=		image

SRCS=		xcb_image.c

HEADERS=	xcb_image.h xcb_pixel.h

LDADD!=		pkg-config --libs xcb xcb-shm xcb-aux

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
