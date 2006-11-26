# $Xenocara: Makefile,v 1.1 2006/04/17 20:16:24 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmButtons

PROG=	FvwmButtons
SRCS=	FvwmButtons.c button.c draw.c icons.c misc.c output.c parse.c

LDADD+= -lXpm -lXext -lX11
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
