#	$OpenBSD: Makefile,v 1.2 2021/07/22 11:10:12 jsg Exp $

LIB=	xmlconfig

NOPROFILE=

SRCS=	xmlconfig.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/util \
		-I${MESA_SRC}/generated/src/util

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/util
