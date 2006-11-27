# $OpenBSD: Makefile,v 1.2 2006/11/27 20:19:57 matthieu Exp $

PROG=	xsystrace
MAN=	xsystrace.1

.ifmake install
SUBDIR=		pixmaps
.endif

CPPFLAGS+=	-I${X11BASE}/include
LDADD+=		-L${X11BASE}/lib -lXaw -lXt -lXmu -lX11 -lXext

SRCS=		main.c interface.c callbacks.c policy.c

beforeinstall:
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} \
		${.CURDIR}/Xsystrace.ad ${PREFIX}/${LIBDIR}/X11/app-defaults/Xsystrace

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
.include <bsd.subdir.mk>
