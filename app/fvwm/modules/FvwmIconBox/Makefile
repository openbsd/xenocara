# $Xenocara: Makefile,v 1.1 2006/04/17 20:16:24 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmIconBox

PROG=	FvwmIconBox
SRCS=	FvwmIconBox.c icons.c

LDADD+= -lXpm -lXext -lX11
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
