# $OpenBSD: Makefile,v 1.2 2010/06/01 20:00:22 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	xfixesproto.h xfixeswire.h
PKGCONFIG=	fixesproto.pc

afterinstall: fixesproto.txt
	${INSTALL_DATA} ${.CURDIR}/fixesproto.txt \
		${DESTDIR}${X11BASE}/share/doc/fixesproto

.include <bsd.xorg.mk>
