#	$OpenBSD: Makefile,v 1.4 2022/02/24 02:29:49 jsg Exp $

LIB=	llvmpipe

NOPROFILE=

SRCS=	lp_bld_alpha.c \
	lp_bld_blend.c \
	lp_bld_blend_aos.c \
	lp_bld_blend_logicop.c \
	lp_bld_depth.c \
	lp_bld_interp.c \
	lp_clear.c \
	lp_context.c \
	lp_cs_tpool.c \
	lp_draw_arrays.c \
	lp_fence.c \
	lp_flush.c \
	lp_jit.c \
	lp_linear.c \
	lp_linear_fastpath.c \
	lp_linear_interp.c \
	lp_linear_sampler.c \
	lp_memory.c \
	lp_perf.c \
	lp_query.c \
	lp_rast.c \
	lp_rast_debug.c \
	lp_rast_linear.c \
	lp_rast_linear_fallback.c \
	lp_rast_rect.c \
	lp_rast_tri.c \
	lp_scene.c \
	lp_scene_queue.c \
	lp_screen.c \
	lp_setup.c \
	lp_setup_analysis.c \
	lp_setup_line.c \
	lp_setup_point.c \
	lp_setup_rect.c \
	lp_setup_tri.c \
	lp_setup_vbuf.c \
	lp_state_blend.c \
	lp_state_clip.c \
	lp_state_cs.c \
	lp_state_derived.c \
	lp_state_fs.c \
	lp_state_fs_analysis.c \
	lp_state_fs_fastpath.c \
	lp_state_fs_linear.c \
	lp_state_fs_linear_llvm.c \
	lp_state_gs.c \
	lp_state_rasterizer.c \
	lp_state_sampler.c \
	lp_state_setup.c \
	lp_state_so.c \
	lp_state_surface.c \
	lp_state_tess.c \
	lp_state_vertex.c \
	lp_state_vs.c \
	lp_surface.c \
	lp_tex_sample.c \
	lp_texture.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers/llvmpipe \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/drivers/llvmpipe
