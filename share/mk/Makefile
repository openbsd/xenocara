#	$OpenBSD: Makefile,v 1.5 2010/05/08 21:09:51 matthieu Exp $

.include <bsd.own.mk>

FILES=	automake.dep bsd.xconf.mk bsd.xorg.mk

all:

install::
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 ${FILES} \
		${DESTDIR}${X11BASE}/share/mk

.include <bsd.xorg.mk>
