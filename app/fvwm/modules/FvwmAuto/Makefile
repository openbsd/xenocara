# $OpenBSD: Makefile,v 1.4 2007/04/09 18:59:57 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmAuto

PROG=	FvwmAuto
SRCS=	FvwmAuto.c

LDADD+= -lXpm ${XLIB}
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
