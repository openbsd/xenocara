# $OpenBSD: Makefile,v 1.5 2007/04/07 15:37:50 matthieu Exp $

DIST=	${.CURDIR}/../../dist/Mesa
GLW=	${DIST}/src/glw

DEBUG?=

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

obj: _xenocara_obj

includes:
	cd ${GLW}; for i in ${INCS}; do \
	    j="cmp -s $$i ${DESTDIR}${INCSDIR}/GL/$$i || \
		${INSTALL} ${INSTALL_COPY} -o ${BINOWN} -g ${BINGRP} -m 444 \
		$$i ${DESTDIR}${INCSDIR}/GL"; \
		echo "\tinstalling $$i"; \
		eval "$$j"; \
	done

beforeinstall: includes

NOPROFILE=

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${GLW}
