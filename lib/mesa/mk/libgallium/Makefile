#	$OpenBSD: Makefile,v 1.6 2023/01/28 08:56:34 jsg Exp $

.include "../Makefile.inc"

LIB=	gallium

NOPROFILE=

SRCS=	cso_cache.c \
	cso_context.c \
	cso_hash.c \
	draw_context.c \
	draw_fs.c \
	draw_gs.c \
	draw_pipe_aaline.c \
	draw_pipe_aapoint.c \
	draw_pipe.c \
	draw_pipe_clip.c \
	draw_pipe_cull.c \
	draw_pipe_flatshade.c \
	draw_pipe_offset.c \
	draw_pipe_pstipple.c \
	draw_pipe_stipple.c \
	draw_pipe_twoside.c \
	draw_pipe_unfilled.c \
	draw_pipe_user_cull.c \
	draw_pipe_util.c \
	draw_pipe_validate.c \
	draw_pipe_vbuf.c \
	draw_pipe_wide_line.c \
	draw_pipe_wide_point.c \
	draw_prim_assembler.c \
	draw_pt.c \
	draw_pt_emit.c \
	draw_pt_fetch.c \
	draw_pt_fetch_shade_emit.c \
	draw_pt_fetch_shade_pipeline.c \
	draw_pt_post_vs.c \
	draw_pt_so_emit.c \
	draw_pt_util.c \
	draw_pt_vsplit.c \
	draw_tess.c \
	draw_vertex.c \
	draw_vs.c \
	draw_vs_exec.c \
	draw_vs_variant.c \
	dd_context.c \
	dd_draw.c \
	dd_screen.c \
	noop_pipe.c \
	noop_state.c \
	tr_context.c \
	tr_dump.c \
	tr_dump_state.c \
	tr_screen.c \
	tr_texture.c \
	font.c \
	hud_context.c \
	hud_cpu.c \
	hud_nic.c \
	hud_cpufreq.c \
	hud_diskstat.c \
	hud_sensors_temp.c \
	hud_driver_query.c \
	hud_fps.c \
	u_primconvert.c \
	os_process.c \
	pb_buffer_fenced.c \
	pb_bufmgr_cache.c \
	pb_bufmgr_debug.c \
	pb_bufmgr_mm.c \
	pb_bufmgr_slab.c \
	pb_cache.c \
	pb_slab.c \
	pb_validate.c \
	pp_celshade.c \
	pp_colors.c \
	pp_init.c \
	pp_mlaa.c \
	pp_program.c \
	pp_run.c \
	rtasm_execmem.c \
	rtasm_x86sse.c \
	tgsi_aa_point.c \
	tgsi_build.c \
	tgsi_dump.c \
	tgsi_dynamic_indexing.c \
	tgsi_exec.c \
	tgsi_from_mesa.c \
	tgsi_info.c \
	tgsi_iterate.c \
	tgsi_lowering.c \
	tgsi_parse.c \
	tgsi_point_sprite.c \
	tgsi_sanity.c \
	tgsi_scan.c \
	tgsi_strings.c \
	tgsi_text.c \
	tgsi_transform.c \
	tgsi_two_side.c \
	tgsi_ureg.c \
	tgsi_util.c \
	tgsi_vpos.c \
	translate.c \
	translate_cache.c \
	translate_generic.c \
	translate_sse.c \
	u_async_debug.c \
	u_bitmask.c \
	u_blitter.c \
	u_cache.c \
	u_compute.c \
	u_debug_flush.c \
	u_debug_image.c \
	u_draw.c \
	u_draw_quad.c \
	u_driconf.c \
	u_dump_defines.c \
	u_dump_state.c \
	u_framebuffer.c \
	u_gen_mipmap.c \
	u_handle_table.c \
	u_helpers.c \
	u_index_modify.c \
	u_linear.c \
	u_live_shader_cache.c \
	u_log.c \
	u_prim.c \
	u_prim_restart.c \
	u_pstipple.c \
	u_resource.c \
	u_sampler.c \
	u_screen.c \
	u_simple_shaders.c \
	u_split_draw.c \
	u_suballoc.c \
	u_surface.c \
	u_tests.c \
	u_texture.c \
	u_tile.c \
	u_transfer.c \
	u_transfer_helper.c \
	u_threaded_context.c \
	u_trace_gallium.c \
	u_upload_mgr.c \
	u_vbuf.c \
	u_vertex_state_cache.c \
	tgsi_to_nir.c \
	nir_to_tgsi.c \
	nir_draw_helpers.c \
	nir_helpers.c

SRCS+=	renderonly.c \
	tr_util.c \
	u_tracepoints.c \
	u_indices_gen.c \
	u_unfilled_gen.c

.if ${WITH_LLVM} == "yes"
SRCS+=	lp_bld_arit.c \
	lp_bld_arit_overflow.c \
	lp_bld_assert.c \
	lp_bld_bitarit.c \
	lp_bld_const.c \
	lp_bld_conv.c \
	lp_bld_coro.c \
	lp_bld_debug.cpp \
	lp_bld_flow.c \
	lp_bld_format_aos_array.c \
	lp_bld_format_aos.c \
	lp_bld_format_float.c \
	lp_bld_format_s3tc.c \
	lp_bld_format.c \
	lp_bld_format_soa.c \
	lp_bld_format_srgb.c \
	lp_bld_format_yuv.c \
	lp_bld_gather.c \
	lp_bld_init.c \
	lp_bld_intr.c \
	lp_bld_ir_common.c \
	lp_bld_jit_types.c \
	lp_bld_logic.c \
	lp_bld_misc.cpp \
	lp_bld_nir.c \
	lp_bld_nir_aos.c \
	lp_bld_nir_soa.c \
	lp_bld_pack.c \
	lp_bld_printf.c \
	lp_bld_quad.c \
	lp_bld_sample_aos.c \
	lp_bld_sample.c \
	lp_bld_sample_soa.c \
	lp_bld_struct.c \
	lp_bld_swizzle.c \
	lp_bld_tgsi_action.c \
	lp_bld_tgsi_aos.c \
	lp_bld_tgsi.c \
	lp_bld_tgsi_info.c \
	lp_bld_tgsi_soa.c \
	lp_bld_type.c \
	draw_llvm.c \
	draw_llvm_sample.c \
	draw_pt_fetch_shade_pipeline_llvm.c \
	draw_vs_llvm.c \
	tessellator.cpp \
	p_tessellator.cpp \
	nir_to_tgsi_info.c

LLVM_CXXFLAGS!=	llvm-config --cxxflags
CXXFLAGS+= ${LLVM_CXXFLAGS}

.endif

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/auxiliary/cso_cache \
		-I${MESA_SRC}/src/gallium/auxiliary/draw \
		-I${MESA_SRC}/src/gallium/auxiliary/driver_ddebug \
		-I${MESA_SRC}/src/gallium/auxiliary/driver_noop \
		-I${MESA_SRC}/src/gallium/auxiliary/driver_trace \
		-I${MESA_SRC}/src/gallium/auxiliary/hud \
		-I${MESA_SRC}/src/gallium/auxiliary/indices \
		-I${MESA_SRC}/src/gallium/auxiliary/os \
		-I${MESA_SRC}/src/gallium/auxiliary/pipebuffer \
		-I${MESA_SRC}/src/gallium/auxiliary/postprocess \
		-I${MESA_SRC}/src/gallium/auxiliary/renderonly \
		-I${MESA_SRC}/src/gallium/auxiliary/rtasm \
		-I${MESA_SRC}/src/gallium/auxiliary/tgsi \
		-I${MESA_SRC}/src/gallium/auxiliary/translate \
		-I${MESA_SRC}/src/gallium/auxiliary/util \
		-I${MESA_SRC}/src/gallium/auxiliary/nir \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/src/util \
		-I${MESA_SRC}/generated/src/gallium/auxiliary \
		-I${MESA_SRC}/generated/src/compiler/nir
#llvm
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/auxiliary/gallivm \
		-I${MESA_SRC}/src/gallium/auxiliary/tessellator

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/gallium/auxiliary/cso_cache
.PATH:	${MESA_SRC}/src/gallium/auxiliary/draw
.PATH:	${MESA_SRC}/src/gallium/auxiliary/driver_ddebug
.PATH:	${MESA_SRC}/src/gallium/auxiliary/driver_noop
.PATH:	${MESA_SRC}/src/gallium/auxiliary/driver_trace
.PATH:	${MESA_SRC}/src/gallium/auxiliary/hud
.PATH:	${MESA_SRC}/src/gallium/auxiliary/indices
.PATH:	${MESA_SRC}/src/gallium/auxiliary/os
.PATH:	${MESA_SRC}/src/gallium/auxiliary/pipebuffer
.PATH:	${MESA_SRC}/src/gallium/auxiliary/postprocess
.PATH:	${MESA_SRC}/src/gallium/auxiliary/renderonly
.PATH:	${MESA_SRC}/src/gallium/auxiliary/rtasm
.PATH:	${MESA_SRC}/src/gallium/auxiliary/tgsi
.PATH:	${MESA_SRC}/src/gallium/auxiliary/translate
.PATH:	${MESA_SRC}/src/gallium/auxiliary/util
.PATH:	${MESA_SRC}/src/gallium/auxiliary/nir
.PATH:	${MESA_SRC}/generated/src/gallium/auxiliary
# llvm
.PATH:	${MESA_SRC}/src/gallium/auxiliary/gallivm
.PATH:	${MESA_SRC}/src/gallium/auxiliary/tessellator
