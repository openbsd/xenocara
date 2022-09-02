#	$OpenBSD: Makefile,v 1.3 2022/09/02 05:46:39 jsg Exp $

GEN=	120
LIB=	iris_gen${GEN}

NOPROFILE=

SRCS=	iris_blorp.c \
	iris_query.c \
	iris_state.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS} ${C_SSE2_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/intel \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-DGFX_VERx10=${GEN}

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/drivers/iris
