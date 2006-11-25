#	$OpenBSD: Makefile,v 1.1.1.1 2006/11/25 14:56:32 matthieu Exp $

.include <bsd.own.mk>

FILES=	bsd.xorg.mk

all:

install::
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 ${FILES} \
		${DESTDIR}${X11BASE}/share/mk

.include <bsd.xorg.mk>
