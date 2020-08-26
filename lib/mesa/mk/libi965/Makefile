#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:38 jsg Exp $

LIB=	i965

NOPROFILE=

SRCS=	brw_binding_tables.c \
	brw_blorp.c \
	brw_bufmgr.c \
	brw_clear.c \
	brw_clip.c \
	brw_compute.c \
	brw_conditional_render.c \
	brw_context.c \
	brw_cs.c \
	brw_curbe.c \
	brw_disk_cache.c \
	brw_draw.c \
	brw_draw_upload.c \
	brw_ff_gs.c \
	brw_ff_gs_emit.c \
	brw_formatquery.c \
	brw_generate_mipmap.c \
	brw_gs.c \
	brw_gs_surface_state.c \
	brw_link.cpp \
	brw_meta_util.c \
	brw_misc_state.c \
	brw_nir_uniforms.cpp \
	brw_object_purgeable.c \
	brw_pipe_control.c \
	brw_performance_query.c \
	brw_program.c \
	brw_program_binary.c \
	brw_program_cache.c \
	brw_primitive_restart.c \
	brw_queryobj.c \
	brw_reset.c \
	brw_sf.c \
	brw_state_upload.c \
	brw_surface_formats.c \
	brw_sync.c \
	brw_tcs.c \
	brw_tcs_surface_state.c \
	brw_tes.c \
	brw_tes_surface_state.c \
	brw_urb.c \
	brw_util.c \
	brw_vs.c \
	brw_vs_surface_state.c \
	brw_wm.c \
	brw_wm_surface_state.c \
	gen6_clip_state.c \
	gen6_constant_state.c \
	gen6_multisample_state.c \
	gen6_queryobj.c \
	gen6_sampler_state.c \
	gen6_sol.c \
	gen6_urb.c \
	gen7_l3_state.c \
	gen7_sol_state.c \
	gen7_urb.c \
	gen8_depth_state.c \
	gen8_multisample_state.c \
	hsw_queryobj.c \
	hsw_sol.c \
	intel_batchbuffer.c \
	intel_blit.c \
	intel_buffer_objects.c \
	intel_buffers.c \
	intel_copy_image.c \
	intel_extensions.c \
	intel_fbo.c \
	intel_mipmap_tree.c \
	intel_pixel_bitmap.c \
	intel_pixel.c \
	intel_pixel_copy.c \
	intel_pixel_draw.c \
	intel_pixel_read.c \
	intel_screen.c \
	intel_state.c \
	intel_tex.c \
	intel_tex_copy.c \
	intel_tex_image.c \
	intel_tex_validate.c \
	intel_upload.c

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
