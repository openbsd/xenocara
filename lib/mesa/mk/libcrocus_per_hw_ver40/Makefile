#	$OpenBSD: Makefile,v 1.1 2022/09/02 05:46:37 jsg Exp $

GEN=	40
LIB=	crocus_per_hw_ver${GEN}

NOPROFILE=

SRCS=	crocus_blorp.c \
	crocus_query.c \
	crocus_state.c \
	crocus_blt.c

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

.PATH: ${MESA_SRC}/src/gallium/drivers/crocus
