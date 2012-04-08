# $OpenBSD: Makefile,v 1.3 2012/04/08 09:46:58 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	saver.h saverproto.h
PKGCONFIG=	scrnsaverproto.pc

afterinstall:
	${INSTALL_DATA} ${.CURDIR}/specs/saver.xml \
		${DESTDIR}${X11BASE}/share/doc/scrnsaverproto

.include <bsd.xorg.mk>
