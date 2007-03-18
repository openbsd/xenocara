#	$OpenBSD: Makefile,v 1.2 2007/03/18 10:26:58 matthieu Exp $

.include <bsd.own.mk>

FILES=	automake.dep bsd.xorg.mk

all:

install::
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 ${FILES} \
		${DESTDIR}${X11BASE}/share/mk

.include <bsd.xorg.mk>
