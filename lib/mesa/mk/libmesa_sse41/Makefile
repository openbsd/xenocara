#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:40 jsg Exp $

LIB=	mesa_sse41

NOPROFILE=

.if ${MACHINE_ARCH} == "i386" || ${MACHINE_ARCH} == "amd64"
SRCS=		streaming-load-memcpy.c \
		sse_minmax.c
CFLAGS+=	${SSE41_ARGS}
.else
SRCS=
.endif

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/mesa/main
