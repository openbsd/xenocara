# -*- makefile -*- 
# $OpenBSD: Makefile.inc,v 1.7 2011/03/08 20:48:59 matthieu Exp $

.ifndef FVWM_MAKEFILE_INC
FVWM_MAKEFILE_INC = done
.include <bsd.xconf.mk>

X11BASE?=	/usr/X11R6

CFLAGS+=	-I${X11BASE}/include -I${.CURDIR}/../.. -I${.CURDIR}/../../libs
LDADD+=		-L${X11BASE}/lib

XLIB=		-lX11 -lxcb -lXau -lXdmcp

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
