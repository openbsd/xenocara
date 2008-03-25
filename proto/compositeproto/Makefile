# $OpenBSD: Makefile,v 1.1 2008/03/25 23:28:19 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS= 	composite.h compositeproto.h
PKGCONFIG=	compositeproto.pc

install:: compositeproto.txt
	${INSTALL_DATA} ${.CURDIR}/compositeproto.txt \
		${DESTDIR}${X11BASE}/share/doc/compositeproto

.include <bsd.xorg.mk>
