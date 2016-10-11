#	$OpenBSD: Makefile,v 1.7 2016/10/11 22:36:53 matthieu Exp $

.include <bsd.own.mk>

FILES=	bsd.xconf.mk bsd.xorg.mk package_version.m4

all:

install::
	${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 ${FILES} \
		${DESTDIR}${X11BASE}/share/mk

.include <bsd.xorg.mk>
