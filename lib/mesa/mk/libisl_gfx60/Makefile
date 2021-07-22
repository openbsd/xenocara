#	$OpenBSD: Makefile,v 1.1 2021/07/22 11:10:11 jsg Exp $

GFX=	60
LIB=	isl_gfx${GFX}

NOPROFILE=

SRCS=	isl_emit_depth_stencil.c \
	isl_surface_state.c \
	isl_gfx6.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/generated/src/intel \
		-DGFX_VERx10=${GFX}

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/isl
