# $OpenBSD: Makefile,v 1.2 2009/05/22 15:11:02 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	damageproto.h damagewire.h
PKGCONFIG=	damageproto.pc

afterinstall: damageproto.txt
	${INSTALL_DATA} ${.CURDIR}/damageproto.txt \
		${DESTDIR}${X11BASE}/share/doc/damageproto

.include <bsd.xorg.mk>
