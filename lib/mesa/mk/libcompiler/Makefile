#	$OpenBSD: Makefile,v 1.3 2025/06/05 14:18:40 jsg Exp $

LIB=	compiler

NOPROFILE=

SRCS=	glsl_types.c \
	shader_enums.c

SRCS+=	builtin_types.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/compiler \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/compiler

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/compiler
.PATH:	${MESA_SRC}/generated/src/compiler
