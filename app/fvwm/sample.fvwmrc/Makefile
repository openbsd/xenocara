# $OpenBSD: Makefile,v 1.4 2007/03/25 19:35:56 aanriot Exp $

.include "../Makefile.inc"

NOOBJ=Yes
OSRELEASE=`/sbin/sysctl -n kern.osrelease 2>&1`
RM?=rm

depend:

all:
	@sed -e "s,__osrelease__,${OSRELEASE}," \
		${.CURDIR}/system.fvwmrc > .fvwmrc

install:
	${INSTALL} ${INSTALL_COPY} .fvwmrc \
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
	${RM} -f .fvwmrc

cleandir:

.include <bsd.own.mk>
.include <bsd.xorg.mk>
