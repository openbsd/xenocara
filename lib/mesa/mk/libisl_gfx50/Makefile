#	$OpenBSD: Makefile,v 1.3 2023/11/02 04:53:31 jsg Exp $

GFX=	50
LIB=	isl_gfx${GFX}

NOPROFILE=

SRCS=	isl_emit_cpb.c \
	isl_emit_depth_stencil.c \
	isl_surface_state.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/intel \
		-DGFX_VERx10=${GFX}

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/isl
