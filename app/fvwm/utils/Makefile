# $OpenBSD: Makefile,v 1.3 2010/11/27 14:25:47 matthieu Exp $

.include "../Makefile.inc"

.PATH: ${DIST}/utils

PROG=	xpmroot
SRCS=	xpmroot.c

LDADD+= -lXpm -lXt -lICE -lSM -lXext -lX11 ${LIBXCB} -lXdmcp -lXau

.include <bsd.prog.mk>
.include <bsd.xorg.mk>
