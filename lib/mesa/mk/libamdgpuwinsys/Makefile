#	$OpenBSD: Makefile,v 1.3 2021/07/22 11:10:09 jsg Exp $

LIB=	amdgpuwinsys

NOPROFILE=

SRCS=	amdgpu_bo.c \
	amdgpu_cs.c \
	amdgpu_surface.c \
	amdgpu_winsys.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/winsys/amdgpu/drm \
		-I${MESA_SRC}/src/amd/common \
		-I${MESA_SRC}/src/amd/llvm \
		-I${MESA_SRC}/generated/src/amd/common

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/winsys/amdgpu/drm
