# $OpenBSD: Makefile,v 1.3 2007/04/09 18:59:57 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmButtons

PROG=	FvwmButtons
SRCS=	FvwmButtons.c button.c draw.c icons.c misc.c output.c parse.c

LDADD+= -lXpm -lXext ${XLIB}
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
