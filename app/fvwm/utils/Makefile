# $OpenBSD: Makefile,v 1.4 2011/03/08 20:48:59 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/utils

PROG=	xpmroot
SRCS=	xpmroot.c

LDADD+= -lXpm -lXt -lICE -lSM -lXext -lX11 -lxcb -lXdmcp -lXau

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
