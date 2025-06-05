#	$OpenBSD: Makefile,v 1.3 2025/06/05 14:18:42 jsg Exp $

LIB=	glcpp

NOPROFILE=

SRCS=	glcpp-lex.c \
	glcpp-parse.c \
	pp.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CXXFLAGS+=	${CXX_VIS_FLAGS}
CPPFLAGS+=	-I${MESA_SRC}/src/compiler/glsl/glcpp \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/compiler/glsl/glcpp

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/compiler/glsl/glcpp
.PATH:	${MESA_SRC}/generated/src/compiler/glsl/glcpp

.SUFFIXES:
.SUFFIXES: .o .c
