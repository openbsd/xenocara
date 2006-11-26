# $Xenocara: Makefile,v 1.1 2006/04/17 20:17:13 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/modules/FvwmWinList

PROG=	FvwmWinList
SRCS=	ButtonArray.c Colors.c FvwmWinList.c List.c Mallocs.c 

LDADD+= -lXpm -lX11
BINDIR= ${FVWMLIBDIR}

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
