# $OpenBSD: Makefile,v 1.3 2009/05/22 15:15:52 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	dri2proto.h dri2tokens.h
PKGCONFIG=	dri2proto.pc

afterinstall:
	${INSTALL_DATA} ${.CURDIR}/dri2proto.txt \
		${DESTDIR}${X11BASE}/share/doc/dri2proto

.include <bsd.xorg.mk>
