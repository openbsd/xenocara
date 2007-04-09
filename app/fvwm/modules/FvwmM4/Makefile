# $OpenBSD: Makefile,v 1.3 2007/04/09 18:59:57 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmM4

PROG=	FvwmM4
SRCS=	FvwmM4.c

CPPFLAGS+= -DFVWM_MODULEDIR=\"$(FVWMLIBDIR)\" \
	-DFVWM_CONFIGDIR=\"$(FVWMLIBDIR)\" \

LDADD+= ${XLIB}
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
