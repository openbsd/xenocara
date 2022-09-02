#	$OpenBSD: Makefile,v 1.3 2022/09/02 05:46:41 jsg Exp $

LIB=	softpipe

NOPROFILE=

SRCS=	sp_buffer.c \
	sp_clear.c \
	sp_compute.c \
	sp_context.c \
	sp_draw_arrays.c \
	sp_fence.c \
	sp_flush.c \
	sp_fs_exec.c \
	sp_image.c \
	sp_prim_vbuf.c \
	sp_quad_blend.c \
	sp_quad_depth_test.c \
	sp_quad_fs.c \
	sp_quad_pipe.c \
	sp_query.c \
	sp_screen.c \
	sp_setup.c \
	sp_state_blend.c \
	sp_state_clip.c \
	sp_state_derived.c \
	sp_state_image.c \
	sp_state_rasterizer.c \
	sp_state_sampler.c \
	sp_state_shader.c \
	sp_state_so.c \
	sp_state_surface.c \
	sp_state_vertex.c \
	sp_surface.c \
	sp_tex_sample.c \
	sp_tex_tile_cache.c \
	sp_texture.c \
	sp_tile_cache.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers/softpipe \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/drivers/softpipe
