#	$OpenBSD: Makefile,v 1.3 2008/03/25 23:41:50 matthieu Exp $

.include <bsd.own.mk>

FILES=	automake.dep bsd.xconf.mk bsd.xorg.mk

all:

install::
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 ${FILES} \
		${DESTDIR}${X11BASE}/share/mk

.include <bsd.xorg.mk>
