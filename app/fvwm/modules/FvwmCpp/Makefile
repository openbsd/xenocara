# $OpenBSD: Makefile,v 1.3 2007/04/09 18:59:57 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmCpp

PROG=	FvwmCpp
SRCS=	FvwmCpp.c

CPPFLAGS+= -DFVWM_MODULEDIR=\"$(FVWMLIBDIR)\" \
	-DFVWM_CONFIGDIR=\"$(FVWMLIBDIR)\" \
	-DFVWM_CPP=\"$(FVWMLIBDIR)/FvwmCpp\"
LDADD+=	${XLIB}
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
