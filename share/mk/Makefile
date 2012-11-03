#	$OpenBSD: Makefile,v 1.6 2012/11/03 15:19:02 espie Exp $

.include <bsd.own.mk>

FILES=	automake.dep bsd.xconf.mk bsd.xorg.mk package_version.m4

all:

install::
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 ${FILES} \
		${DESTDIR}${X11BASE}/share/mk

.include <bsd.xorg.mk>
