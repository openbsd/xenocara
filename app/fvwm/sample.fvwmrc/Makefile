# $OpenBSD: Makefile,v 1.2 2006/11/27 19:56:35 matthieu Exp $

.include "../Makefile.inc"

depend:

all:

install:
	${INSTALL} ${INSTALL_COPY} ${.CURDIR}/system.fvwmrc \
		${DESTDIR}${FVWMLIBDIR}/.fvwmrc
	${INSTALL} ${INSTALL_COPY} ${.CURDIR}/system.fvwm2rc \
		${DESTDIR}${FVWMLIBDIR}
	${INSTALL} ${INSTALL_COPY} ${.CURDIR}/decor_examples \
		${DESTDIR}${FVWMLIBDIR}
	${INSTALL} ${INSTALL_COPY} ${.CURDIR}/new-features \
		${DESTDIR}${FVWMLIBDIR}
	${INSTALL} ${INSTALL_COPY} ${.CURDIR}/system.fvwm2rc-sample-1 \
		${DESTDIR}${FVWMLIBDIR}
	${INSTALL} ${INSTALL_COPY} ${.CURDIR}/system.fvwm2rc-sample-2 \
		${DESTDIR}${FVWMLIBDIR}

clean:

cleandir:

.include <bsd.own.mk>
.include <bsd.xorg.mk>
