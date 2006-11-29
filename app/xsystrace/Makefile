# $OpenBSD: Makefile,v 1.5 2006/11/29 11:33:49 matthieu Exp $

PROG=	xsystrace
MAN=	xsystrace.1

.ifmake install
SUBDIR=		pixmaps
.endif

CPPFLAGS+=	-I${X11BASE}/include
LDADD+=		-L${X11BASE}/lib -lXaw -lXt -lXmu -lX11 -lXext

SRCS=		main.c interface.c callbacks.c policy.c

afterinstall:
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} \
		${.CURDIR}/Xsystrace.ad ${DESTDIR}/etc/X11/app-defaults/Xsystrace

X11BASE?= /usr/X11R6
MANDIR=	${X11BASE}/man/cat

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
.include <bsd.subdir.mk>
