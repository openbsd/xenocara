# $OpenBSD: Makefile,v 1.2 2006/11/27 19:56:35 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmIconMan

PROG=	FvwmIconMan
SRCS=	FvwmIconMan.c debug.c functions.c fvwm.c globals.c readconfig.c \
	winlist.c x.c xmanager.c

LDADD+= -lXpm -lXext -lX11
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
