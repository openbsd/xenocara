#	$OpenBSD: Makefile,v 1.2 2021/07/22 11:10:09 jsg Exp $

LIB=	i965

NOPROFILE=

SRCS=	brw_batch.c \
	brw_binding_tables.c \
	brw_blit.c \
	brw_blorp.c \
	brw_buffer_objects.c \
	brw_buffers.c \
	brw_bufmgr.c \
	brw_clear.c \
	brw_clip.c \
	brw_compute.c \
	brw_conditional_render.c \
	brw_context.c \
	brw_copy_image.c \
	brw_cs.c \
	brw_curbe.c \
	brw_disk_cache.c \
	brw_draw.c \
	brw_draw_upload.c \
	brw_extensions.c \
	brw_fbo.c \
	brw_ff_gs.c \
	brw_ff_gs_emit.c \
	brw_formatquery.c \
	brw_generate_mipmap.c \
	brw_gs.c \
	brw_gs_surface_state.c \
	brw_link.cpp \
	brw_meta_util.c \
	brw_mipmap_tree.c \
	brw_misc_state.c \
	brw_nir_uniforms.cpp \
	brw_object_purgeable.c \
	brw_performance_query.c \
	brw_pipe_control.c \
	brw_pixel.c \
	brw_pixel_bitmap.c \
	brw_pixel_copy.c \
	brw_pixel_draw.c \
	brw_pixel_read.c \
	brw_primitive_restart.c \
	brw_program.c \
	brw_program_binary.c \
	brw_program_cache.c \
	brw_queryobj.c \
	brw_reset.c \
	brw_screen.c \
	brw_sf.c \
	brw_state.c \
	brw_state_upload.c \
	brw_surface_formats.c \
	brw_sync.c \
	brw_tcs.c \
	brw_tcs_surface_state.c \
	brw_tes.c \
	brw_tes_surface_state.c \
	brw_tex.c \
	brw_tex_copy.c \
	brw_tex_image.c \
	brw_tex_validate.c \
	brw_upload.c \
	brw_urb.c \
	brw_util.c \
	brw_vs.c \
	brw_vs_surface_state.c \
	brw_wm.c \
	brw_wm_surface_state.c \
	gfx6_clip_state.c \
	gfx6_constant_state.c \
	gfx6_multisample_state.c \
	gfx6_queryobj.c \
	gfx6_sampler_state.c \
	gfx6_sol.c \
	gfx6_urb.c \
	gfx7_l3_state.c \
	gfx7_sol_state.c \
	gfx7_urb.c \
	gfx8_depth_state.c \
	gfx8_multisample_state.c \
	hsw_queryobj.c \
	hsw_sol.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS} ${C_SSE2_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS} ${C_SSE2_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa/drivers/dri/i965 \
		-I${MESA_SRC}/src/mesa/drivers/dri/common \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src/util \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-I${MESA_SRC}/generated/src/intel

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/mesa/drivers/dri/i965
