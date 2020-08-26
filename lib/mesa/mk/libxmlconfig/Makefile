#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:41 jsg Exp $

LIB=	xmlconfig

NOPROFILE=

SRCS=	xmlconfig.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/util

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/util
