# $OpenBSD: Makefile,v 1.3 2012/04/08 09:46:58 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	recordconst.h recordproto.h recordstr.h
PKGCONFIG=	recordproto.pc

afterinstall:
	${INSTALL_DATA} ${.CURDIR}/specs/record.xml \
		${DESTDIR}${X11BASE}/share/doc/recordproto

.include <bsd.xorg.mk>
