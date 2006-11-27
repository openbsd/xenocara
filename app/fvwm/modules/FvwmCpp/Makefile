# $OpenBSD: Makefile,v 1.2 2006/11/27 19:56:35 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmCpp

PROG=	FvwmCpp
SRCS=	FvwmCpp.c

CPPFLAGS+= -DFVWM_MODULEDIR=\"$(FVWMLIBDIR)\" \
	-DFVWM_CONFIGDIR=\"$(FVWMLIBDIR)\" \
	-DFVWM_CPP=\"$(FVWMLIBDIR)/FvwmCpp\"
LDADD+=	-lX11
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
