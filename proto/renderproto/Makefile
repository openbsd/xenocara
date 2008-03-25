# $OpenBSD: Makefile,v 1.1 2008/03/25 23:28:20 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	render.h renderproto.h
PKGCONFIG=	renderproto.pc

install:: renderproto.txt
	${INSTALL_DATA} ${.CURDIR}/renderproto.txt \
		${DESTDIR}${X11BASE}/share/doc/renderproto

.include <bsd.xorg.mk>
