#	$OpenBSD: Makefile,v 1.2 2023/03/03 06:32:18 jsg Exp $

LIB=	mesa_util_sse41

NOPROFILE=

SRCS=		streaming-load-memcpy.c

.if ${MACHINE_ARCH} == "i386" || ${MACHINE_ARCH} == "amd64"
CFLAGS+=	${SSE41_ARGS}
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
