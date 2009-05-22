# $OpenBSD: Makefile,v 1.2 2009/05/22 15:11:02 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS= 	composite.h compositeproto.h
PKGCONFIG=	compositeproto.pc

afterinstall: compositeproto.txt
	${INSTALL_DATA} ${.CURDIR}/compositeproto.txt \
		${DESTDIR}${X11BASE}/share/doc/compositeproto

.include <bsd.xorg.mk>
