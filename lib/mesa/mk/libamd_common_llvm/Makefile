#	$OpenBSD: Makefile,v 1.3 2023/11/02 04:53:30 jsg Exp $

LIB=	amd_common_llvm

NOPROFILE=

SRCS=	ac_llvm_build.c \
	ac_llvm_helper.cpp \
	ac_llvm_util.c \
	ac_nir_to_llvm.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/amd \
		-I${MESA_SRC}/src/amd/llvm \
		-I${MESA_SRC}/src/amd/common \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/compiler \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-I${MESA_SRC}/generated/src/amd/common

LLVM_CXXFLAGS!= llvm-config --cxxflags | sed 's/-std=[^ ]*//'
CXXFLAGS+= ${LLVM_CXXFLAGS}

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/amd/llvm
