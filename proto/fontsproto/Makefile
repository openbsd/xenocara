# $OpenBSD: Makefile,v 1.2 2012/04/08 09:46:58 matthieu Exp $

HEADERS_SUBDIR=	X11/fonts/
HEADERS=	font.h fontproto.h fontstruct.h FS.h fsmasks.h FSproto.h
PKGCONFIG=	fontsproto.pc

afterinstall:
	${INSTALL_DATA} ${.CURDIR}/specs/fsproto.xml \
		${DESTDIR}${X11BASE}/share/doc/fontsproto

.include <bsd.xorg.mk>
