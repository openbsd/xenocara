#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:36 jsg Exp $

LIB=	amd_common

NOPROFILE=

SRCS=	ac_binary.c \
	ac_debug.c \
	ac_gpu_info.c \
	ac_rtld.c \
	ac_shader_args.c \
	ac_shader_util.c \
	ac_surface.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/amd \
		-I${MESA_SRC}/src/amd/common \
		-I${MESA_SRC}/src/compiler \
		-I${MESA_SRC}/generated/src/amd/common \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/amd/common
