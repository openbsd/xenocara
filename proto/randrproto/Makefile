# $OpenBSD: Makefile,v 1.1 2008/03/25 23:28:20 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	randr.h randrproto.h
PKGCONFIG=	randrproto.pc

install:: randrproto.txt
	${INSTALL_DATA} ${.CURDIR}/randrproto.txt \
		${DESTDIR}${X11BASE}/share/doc/randrproto

.include <bsd.xorg.mk>
