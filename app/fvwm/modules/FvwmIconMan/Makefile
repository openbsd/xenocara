# $Xenocara: Makefile,v 1.1 2006/04/17 20:16:24 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmIconMan

PROG=	FvwmIconMan
SRCS=	FvwmIconMan.c debug.c functions.c fvwm.c globals.c readconfig.c \
	winlist.c x.c xmanager.c

LDADD+= -lXpm -lXext -lX11
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
