#	$OpenBSD: Makefile,v 1.2 2025/06/05 14:18:42 jsg Exp $

LIB=	i915drm

NOPROFILE=

SRCS=	i915_drm_batchbuffer.c \
	i915_drm_buffer.c \
	i915_drm_fence.c \
	i915_drm_winsys.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/src/gallium/winsys/i915/drm \
		-I${MESA_SRC}/generated/src

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/winsys/i915/drm
