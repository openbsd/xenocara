# $OpenBSD: Makefile,v 1.2 2009/05/22 15:11:02 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	randr.h randrproto.h
PKGCONFIG=	randrproto.pc

afterinstall: randrproto.txt
	${INSTALL_DATA} ${.CURDIR}/randrproto.txt \
		${DESTDIR}${X11BASE}/share/doc/randrproto

.include <bsd.xorg.mk>
