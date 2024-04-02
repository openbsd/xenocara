#	$OpenBSD: Makefile,v 1.1 2024/04/02 10:42:14 jsg Exp $

LIB=	vtn

NOPROFILE=

SRCS=	gl_spirv.c \
	spirv_to_nir.c \
	vtn_alu.c \
	vtn_amd.c \
	vtn_cfg.c \
	vtn_cmat.c \
	vtn_glsl450.c \
	vtn_opencl.c \
	vtn_structured_cfg.c \
	vtn_subgroup.c \
	vtn_variables.c

SRCS+= 	spirv_info.c \
	vtn_gather_types.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/compiler \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/src/compiler/spirv \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-I${MESA_SRC}/generated/src/compiler/spirv

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/compiler/spirv
.PATH: ${MESA_SRC}/generated/src/compiler/spirv
