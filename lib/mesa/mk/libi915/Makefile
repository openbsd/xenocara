#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:38 jsg Exp $

LIB=	i915

NOPROFILE=

SRCS=	i830_context.c \
	i830_state.c \
	i830_texblend.c \
	i830_texstate.c \
	i830_vtbl.c \
	i915_context.c \
	i915_debug_fp.c \
	i915_fragprog.c \
	i915_program.c \
	i915_state.c \
	i915_tex_layout.c \
	i915_texstate.c \
	i915_vtbl.c \
	intel_batchbuffer.c \
	intel_blit.c \
	intel_buffer_objects.c \
	intel_buffers.c \
	intel_clear.c \
	intel_context.c \
	intel_extensions.c \
	intel_fbo.c \
	intel_mipmap_tree.c \
	intel_pixel.c \
	intel_pixel_bitmap.c \
	intel_pixel_copy.c \
	intel_pixel_draw.c \
	intel_pixel_read.c \
	intel_regions.c \
	intel_render.c \
	intel_screen.c \
	intel_state.c \
	intel_syncobj.c \
	intel_tex.c \
	intel_tex_copy.c \
	intel_tex_image.c \
	intel_tex_layout.c \
	intel_tex_subimage.c \
	intel_tex_validate.c \
	intel_tris.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa/drivers/dri/i915 \
		-I${MESA_SRC}/src/mesa/drivers/dri/common \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/generated/src/util

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/mesa/drivers/dri/i915
