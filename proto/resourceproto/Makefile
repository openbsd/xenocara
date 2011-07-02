# $OpenBSD: Makefile,v 1.2 2011/07/02 20:31:11 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	XResproto.h
PKGCONFIG=	resourceproto.pc

afterinstall:
	${INSTALL_DATA} ${.CURDIR}/resproto.txt \
		${DESTDIR}${X11BASE}/share/doc/resproto

.include <bsd.xorg.mk>
