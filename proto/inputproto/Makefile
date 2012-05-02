# $OpenBSD: Makefile,v 1.4 2012/05/02 19:55:21 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	XI.h XIproto.h XI2.h XI2proto.h
PKGCONFIG=	inputproto.pc
SPECS=		XI2proto.txt XIproto.txt

afterinstall:
	@for i in ${SPECS}; do \
		echo installing $$i in ${X11BASE}/share/doc/inputproto;\
		${INSTALL_DATA} ${.CURDIR}/specs/$$i \
			${DESTDIR}${X11BASE}/share/doc/inputproto;\
	done

.include <bsd.xorg.mk>
