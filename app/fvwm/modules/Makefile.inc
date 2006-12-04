# -*- makefile -*- 
# $Xenocara: Makefile.inc,v 1.2 2006/04/17 20:16:24 matthieu Exp $

.ifndef FVWM_MAKEFILE_INC
FVWM_MAKEFILE_INC = done
.include <bsd.own.mk>

X11BASE?=	/usr/X11R6

CFLAGS+=	-I${X11BASE}/include -I${.CURDIR}/../.. -I${.CURDIR}/../../libs
LDADD+=		-L${X11BASE}/lib

FVWMLIBDIR=	${X11BASE}/lib/X11/fvwm
MANDIR=		${X11BASE}/man/cat

obj: _xenocara_obj

.if exists(${.CURDIR}/../../libs/${__objdir})
LDADD+=		-L${.CURDIR}/../../libs/${__objdir} -lfvwm
DPADD+=		${.CURDIR}/../../libs/${__objdir}/libfvwm.a
.else
LDADD+=		-L${.CURDIR}/../../libs -lfvwm
DPADD+= 	${.CURDIR}/../../libs/libfvwm.a
.endif
.endif
