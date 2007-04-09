# $OpenBSD: Makefile,v 1.3 2007/04/09 18:59:57 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmIconMan

PROG=	FvwmIconMan
SRCS=	FvwmIconMan.c debug.c functions.c fvwm.c globals.c readconfig.c \
	winlist.c x.c xmanager.c

LDADD+= -lXpm -lXext ${XLIB}
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
