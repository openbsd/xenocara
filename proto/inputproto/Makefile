# $OpenBSD: Makefile,v 1.3 2010/06/01 20:00:22 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	XI.h XIproto.h XI2.h XI2proto.h
PKGCONFIG=	inputproto.pc

afterinstall: XI2proto.txt XIproto.txt
	${INSTALL_DATA} ${.CURDIR}/XI2proto.txt ${.CURDIR}/XIproto.txt \
		${DESTDIR}${X11BASE}/share/doc/inputproto

.include <bsd.xorg.mk>
