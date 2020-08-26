#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:39 jsg Exp $

LIB=	isl_tiled_memcpy_sse41

NOPROFILE=

SRCS=	isl_tiled_memcpy_sse41.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS} -msse2 ${SSE41_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/generated/src/intel

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/isl
