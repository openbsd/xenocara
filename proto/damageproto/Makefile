# $OpenBSD: Makefile,v 1.1 2008/03/25 23:28:19 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	damageproto.h damagewire.h
PKGCONFIG=	damageproto.pc

install:: damageproto.txt
	${INSTALL_DATA} ${.CURDIR}/damageproto.txt \
		${DESTDIR}${X11BASE}/share/doc/damageproto

.include <bsd.xorg.mk>
