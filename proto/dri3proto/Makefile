# $OpenBSD: Makefile,v 1.1 2018/06/07 21:59:18 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	dri3proto.h
PKGCONFIG=	dri3proto.pc

afterinstall:
	${INSTALL_DATA} ${.CURDIR}/dri3proto.txt \
		${DESTDIR}${X11BASE}/share/doc/dri3proto

.include <bsd.xorg.mk>
