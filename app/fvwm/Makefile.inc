# -*- makefile -*- 
# $OpenBSD: Makefile.inc,v 1.5 2008/03/25 23:41:50 matthieu Exp $

.ifndef FVWM_MAKEFILE_INC
FVWM_MAKEFILE_INC = done
.include <bsd.xconf.mk>

CFLAGS+=	-I${X11BASE}/include -I${.CURDIR} -I${.CURDIR}/.. \
		-I${.CURDIR}/../libs
LDADD+=		-L${X11BASE}/lib

FVWMLIBDIR=	${X11BASE}/lib/X11/fvwm
MANDIR=		${X11BASE}/man/cat

obj: _xenocara_obj

.if exists(${.CURDIR}/../libs/${__objdir})
LDADD+=		-L${.CURDIR}/../libs/${__objdir} -lfvwm
DPADD+=		${.CURDIR}/../libs/${__objdir}/libfvwm.a
.else
LDADD+=		-L${.CURDIR}/../libs -lfvwm
DPADD+= 	${.CURDIR}/../libs/libfvwm.a
.endif
.endif
