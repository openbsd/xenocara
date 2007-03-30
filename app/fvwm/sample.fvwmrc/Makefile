# $OpenBSD: Makefile,v 1.6 2007/03/30 17:24:12 aanriot Exp $

.include "../Makefile.inc"

OSRELEASE=`/sbin/sysctl -n kern.osrelease 2>&1`
RM?=rm

depend:

all:
	@sed -e "s,__osrelease__,${OSRELEASE}," \
		${.CURDIR}/system.fvwmrc > .fvwmrc

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

cleandir:

.include <bsd.own.mk>
.include <bsd.xorg.mk>
