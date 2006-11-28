# $OpenBSD: Makefile,v 1.3 2006/11/28 19:05:40 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmAuto

PROG=	FvwmAuto
SRCS=	FvwmAuto.c

LDADD+= -lXpm -lX11
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
