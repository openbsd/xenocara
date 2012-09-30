# -*- makefile -*- 
# $OpenBSD: Makefile.inc,v 1.9 2012/09/30 10:12:51 espie Exp $

.ifndef FVWM_MAKEFILE_INC
FVWM_MAKEFILE_INC = done
.include <bsd.xconf.mk>

CFLAGS+=	-I${X11BASE}/include -I${.CURDIR} -I${.CURDIR}/.. \
		-I${.CURDIR}/../libs
LDADD+=		-L${X11BASE}/lib

FVWMLIBDIR=	${X11BASE}/lib/X11/fvwm
MANDIR=		${X11BASE}/man/man

obj: _xenocara_obj

LDADD+=	-L${.CURDIR}/../libs/${__objdir} -L${.CURDIR}/../libs -lfvwm
.endif
