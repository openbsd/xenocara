# $OpenBSD: Makefile,v 1.2 2012/04/08 10:08:32 matthieu Exp $

HEADERS_SUBDIR=	X11/extensions/
HEADERS=	XKBgeom.h XKB.h XKBproto.h XKBsrv.h XKBstr.h
PKGCONFIG=	kbproto.pc
SPECS=		XKBproto-1.svg XKBproto-2.svg XKBproto-3.svg XKBproto-4.svg \
		XKBproto-5.svg XKBproto-6.svg XKBproto-7.svg XKBproto-8.svg \
		XKBproto-9.svg XKBproto-10.svg XKBproto-11.svg \
		appA.xml appB.xml appC.xml appD.xml \
		ch01.xml ch02.xml ch03.xml ch04.xml ch05.xml ch06.xml \
		ch07.xml ch08.xml ch09.xml ch10.xml ch11.xml ch12.xml \
		ch13.xml ch15.xml ch16.xml xkbproto.xml

afterinstall:
	@for i in ${SPECS}; do \
		echo installing $$i in ${X11BASE}/share/doc/kbproto;\
		${INSTALL_DATA} ${.CURDIR}/specs/$$i \
			${DESTDIR}${X11BASE}/share/doc/kbproto;\
	done


.include <bsd.xorg.mk>
