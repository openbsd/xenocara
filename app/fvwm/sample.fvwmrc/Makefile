# $OpenBSD: Makefile,v 1.8 2020/09/30 20:42:12 kn Exp $

.include "../Makefile.inc"

RM?=rm

depend:

all:
	@cat ${.CURDIR}/system.fvwmrc > .fvwmrc

install:
	${INSTALL_DATA} .fvwmrc \
		${DESTDIR}${FVWMLIBDIR}/.fvwmrc
	${INSTALL_DATA} ${.CURDIR}/system.fvwm2rc \
		${DESTDIR}${FVWMLIBDIR}
	${INSTALL_DATA} ${.CURDIR}/decor_examples \
		${DESTDIR}${FVWMLIBDIR}
	${INSTALL_DATA} ${.CURDIR}/new-features \
		${DESTDIR}${FVWMLIBDIR}
	${INSTALL_DATA} ${.CURDIR}/system.fvwm2rc-sample-1 \
		${DESTDIR}${FVWMLIBDIR}
	${INSTALL_DATA} ${.CURDIR}/system.fvwm2rc-sample-2 \
		${DESTDIR}${FVWMLIBDIR}

clean:
	${RM} -f .fvwmrc

cleandir: clean

.include <bsd.own.mk>
.include <bsd.xorg.mk>
