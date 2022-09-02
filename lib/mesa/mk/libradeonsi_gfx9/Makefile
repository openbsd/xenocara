#	$OpenBSD: Makefile,v 1.2 2022/09/02 05:46:41 jsg Exp $

GEN=	9
LIB=	radeonsi_gfx${GEN}

NOPROFILE=

SRCS=	si_state_draw.cpp

.include "../Makefile.inc"

CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/amd/common \
		-I${MESA_SRC}/src/amd/llvm \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src/amd/common \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-DGFX_VER=${GEN}

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/drivers/radeonsi
