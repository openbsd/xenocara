#	$OpenBSD: Makefile,v 1.1 2022/09/02 05:46:38 jsg Exp $

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
		-I${MESA_SRC}/src/gallium/winsys/i915/drm

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/winsys/i915/drm
