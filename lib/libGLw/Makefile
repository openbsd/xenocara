# $OpenBSD: Makefile,v 1.2 2006/11/29 14:45:19 matthieu Exp $

DIST=	${.CURDIR}/../../dist/Mesa
GLW=	${DIST}/src/glw

LIB= GLw

CPPFLAGS += -I${DIST}/include \
	-I${X11BASE}/include

SRCS= \
	GLwDrawA.c

INCS= \
	GLwDrawA.h \
	GLwDrawAP.h \
	GLwMDrawA.h \
	GLwMDrawAP.h

includes:
	cd ${GLW}; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/GL/$$i || \
		${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 \
		$$i ${DESTDIR}${INCSDIR}/GL"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${GLW}
