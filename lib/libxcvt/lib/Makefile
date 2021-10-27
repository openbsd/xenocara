# $OpenBSD: Makefile,v 1.1 2021/10/27 16:13:42 matthieu Exp $

.include <bsd.xconf.mk>

LIB= xcvt

SRCS= libxcvt.c

CFLAGS+= -I${.CURDIR}/../include

LDADD= -lm

NOPROFILE=

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>
