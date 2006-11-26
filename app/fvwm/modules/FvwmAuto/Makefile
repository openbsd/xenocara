# $Xenocara: Makefile,v 1.1 2006/04/17 17:51:34 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmAuto

PROG=	FvwmAuto
SRCS=	FvwmAuto.c

LDADD+= -lXpm -lX11
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>