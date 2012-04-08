# $OpenBSD: Makefile,v 1.3 2012/04/08 09:46:58 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS= 	bigreqsproto.h bigreqstr.h
PKGCONFIG=	bigreqsproto.pc

afterinstall:
	${INSTALL_DATA} ${.CURDIR}/specs/bigreq.xml \
		${DESTDIR}${X11BASE}/share/doc/bigreqsproto

.include <bsd.xorg.mk>	
