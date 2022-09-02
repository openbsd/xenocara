#	$OpenBSD: Makefile,v 1.2 2022/09/02 05:46:38 jsg Exp $

LIB=	i915

NOPROFILE=

SRCS=	i915_blit.c \
	i915_clear.c \
	i915_context.c \
	i915_debug.c \
	i915_debug_fp.c \
	i915_flush.c \
	i915_fpc_emit.c \
	i915_fpc_optimize.c \
	i915_fpc_translate.c \
	i915_prim_emit.c \
	i915_prim_vbuf.c \
	i915_query.c \
	i915_resource_buffer.c \
	i915_resource.c \
	i915_resource_texture.c \
	i915_screen.c \
	i915_state.c \
	i915_state_derived.c \
	i915_state_dynamic.c \
	i915_state_emit.c \
	i915_state_fpc.c \
	i915_state_immediate.c \
	i915_state_sampler.c \
	i915_state_static.c \
	i915_surface.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DGALLIUM_I915 \
		-I${MESA_SRC}/src/gallium/drivers/i915 \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/drivers/i915
