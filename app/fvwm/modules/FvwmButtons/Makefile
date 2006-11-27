# $OpenBSD: Makefile,v 1.2 2006/11/27 19:56:35 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmButtons

PROG=	FvwmButtons
SRCS=	FvwmButtons.c button.c draw.c icons.c misc.c output.c parse.c

LDADD+= -lXpm -lXext -lX11
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
