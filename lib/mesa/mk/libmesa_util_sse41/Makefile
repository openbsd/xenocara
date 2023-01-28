#	$OpenBSD: Makefile,v 1.1 2023/01/28 08:56:35 jsg Exp $

LIB=	mesa_util_sse41

NOPROFILE=

.if ${MACHINE_ARCH} == "i386" || ${MACHINE_ARCH} == "amd64"
SRCS=		streaming-load-memcpy.c
CFLAGS+=	${SSE41_ARGS}
.else
SRCS=
.endif

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/util

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/util
