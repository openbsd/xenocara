# $Xenocara: Makefile,v 1.1 2006/04/17 17:51:34 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/utils

PROG=	xpmroot
SRCS=	xpmroot.c

LDADD+= -lXpm -lXt -lICE -lSM -lXext -lX11 -lXdmcp -lXau

.include <bsd.prog.mk>
.include <bsd.xorg.mk>