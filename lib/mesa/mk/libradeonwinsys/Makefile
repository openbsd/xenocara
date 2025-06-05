#	$OpenBSD: Makefile,v 1.2 2025/06/05 14:18:49 jsg Exp $

LIB=	radeonwinsys

NOPROFILE=

SRCS=	radeon_drm_bo.c \
	radeon_drm_cs.c \
	radeon_drm_surface.c \
	radeon_drm_winsys.c \
	radeon_surface.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/winsys/radeon/drm \
		-I${MESA_SRC}/generated/src

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/winsys/radeon/drm
