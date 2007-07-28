# $OpenBSD: Makefile,v 1.6 2007/07/28 13:50:48 matthieu Exp $

DEBUG?=

LIB= GLw

CPPFLAGS += -I${X11BASE}/include \
	-I${.CURDIR} -DUSE_XM_STUBS

SRCS= \
	GLwDrawA.c \
	GLwDAUtil.c \
	GLwM1DrawA.c \
	GLwM2DrawA.c

INCS= \
	GLwDrawA.h \
	GLwDrawAP.h \
	GLwMDrawA.h \
	GLwMDrawAP.h

obj: _xenocara_obj

includes:
	cd ${.CURDIR}; for i in ${INCS}; do \
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

