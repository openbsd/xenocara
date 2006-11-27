# $OpenBSD: Makefile,v 1.2 2006/11/27 19:56:35 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/utils

PROG=	xpmroot
SRCS=	xpmroot.c

LDADD+= -lXpm -lXt -lICE -lSM -lXext -lX11 -lXdmcp -lXau

.include <bsd.prog.mk>
.include <bsd.xorg.mk>