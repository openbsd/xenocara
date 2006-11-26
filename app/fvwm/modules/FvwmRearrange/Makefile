# $Xenocara: Makefile,v 1.1 2006/04/17 20:16:24 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmRearrange

PROG=	FvwmRearrange
SRCS=	FvwmRearrange.c

LDADD+= -lXpm -lX11
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
