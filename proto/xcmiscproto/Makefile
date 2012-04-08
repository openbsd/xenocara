# $OpenBSD: Makefile,v 1.3 2012/04/08 09:46:58 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	xcmiscproto.h xcmiscstr.h
PKGCONFIG=	xcmiscproto.pc

afterinstall:
	${INSTALL_DATA} ${.CURDIR}/specs/xc-misc.xml \
		${DESTDIR}${X11BASE}/share/doc/xcmiscproto

.include <bsd.xorg.mk>
