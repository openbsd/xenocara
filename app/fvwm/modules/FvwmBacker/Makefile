# $Xenocara: Makefile,v 1.1 2006/04/17 17:51:34 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmBacker

PROG=	FvwmBacker
SRCS=	FvwmBacker.c Mallocs.c root_bits.c

LDADD+= -lXpm -lX11
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>