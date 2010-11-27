# -*- makefile -*- 
# $OpenBSD: Makefile.inc,v 1.6 2010/11/27 14:25:47 matthieu Exp $

.ifndef FVWM_MAKEFILE_INC
FVWM_MAKEFILE_INC = done
.include <bsd.xconf.mk>

CFLAGS+=	-I${X11BASE}/include -I${.CURDIR} -I${.CURDIR}/.. \
		-I${.CURDIR}/../libs
LDADD+=		-L${X11BASE}/lib

.if ${XENOCARA_BUILD_XCB:L} != "no"
LIBXCB= -lxcb
.endif

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
