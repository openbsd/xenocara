# $OpenBSD: Makefile,v 1.1.1.1 2006/11/26 10:58:43 matthieu Exp $

PROG=	xsystrace
MAN=	xsystrace.1

CPPFLAGS+=	-I${X11BASE}/include
LDADD+=		-L${X11BASE}/lib -lXaw -lXt -lXmu -lX11 -lXext

SRCS=		main.c interface.c callbacks.c policy.c

beforeinstall:
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} \
		${.CURDIR}/Xsystrace.ad ${PREFIX}/${LIBDIR}/X11/app-defaults

.include <bsd.prog.mk>
.include <bsd.xorg.mk>


