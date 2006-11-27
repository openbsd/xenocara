# $OpenBSD: Makefile,v 1.2 2006/11/27 19:56:35 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmBacker

PROG=	FvwmBacker
SRCS=	FvwmBacker.c Mallocs.c root_bits.c

LDADD+= -lXpm -lX11
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>