# $OpenBSD: Makefile,v 1.8 2015/08/22 00:50:54 jsg Exp $
.include <bsd.xconf.mk>

LIB=	gallium

GALLIUM=	${.CURDIR}/../../../../dist/Mesa/src/gallium

CPPFLAGS+= \
	-I${GALLIUM}/include \
	-I${GALLIUM}/auxiliary \
	-I${GALLIUM}/auxiliary/util \
	-I${GALLIUM}/drivers \
	-I${.CURDIR}/../../../../dist/Mesa/include \
	-I${.CURDIR}/../../../../dist/Mesa/src \
	-I${X11BASE}/include \
	-I${X11BASE}/include/libdrm 

C_SOURCES = \
	cso_cache.c \
	cso_context.c \
	cso_hash.c \
	draw_context.c \
	draw_fs.c \
	draw_gs.c \
	draw_pipe.c \
	draw_pipe_aaline.c \
	draw_pipe_aapoint.c \
	draw_pipe_clip.c \
	draw_pipe_cull.c \
	draw_pipe_flatshade.c \
	draw_pipe_offset.c \
	draw_pipe_pstipple.c \
	draw_pipe_stipple.c \
	draw_pipe_twoside.c \
	draw_pipe_unfilled.c \
	draw_pipe_util.c \
	draw_pipe_validate.c \
	draw_pipe_vbuf.c \
	draw_pipe_wide_line.c \
	draw_pipe_wide_point.c \
	draw_prim_assembler.c \
	draw_pt.c \
	draw_pt_emit.c \
	draw_pt_fetch.c \
	draw_pt_fetch_emit.c \
	draw_pt_fetch_shade_emit.c \
	draw_pt_fetch_shade_pipeline.c \
	draw_pt_post_vs.c \
	draw_pt_so_emit.c \
	draw_pt_util.c \
	draw_pt_vsplit.c \
	draw_vertex.c \
	draw_vs.c \
	draw_vs_exec.c \
	draw_vs_variant.c \
	font.c \
	hud_context.c \
	hud_cpu.c \
	hud_fps.c \
	hud_driver_query.c \
	u_indices_gen.c \
	u_unfilled_gen.c \
	u_primconvert.c \
	os_misc.c \
	os_process.c \
	os_time.c \
	pb_buffer_fenced.c \
	pb_buffer_malloc.c \
	pb_bufmgr_alt.c \
	pb_bufmgr_cache.c \
	pb_bufmgr_debug.c \
	pb_bufmgr_mm.c \
	pb_bufmgr_ondemand.c \
	pb_bufmgr_pool.c \
	pb_bufmgr_slab.c \
	pb_validate.c \
	pp_celshade.c \
	pp_colors.c \
	pp_init.c \
	pp_mlaa.c \
	pp_program.c \
	pp_run.c \
	rbug_connection.c \
	rbug_context.c \
	rbug_core.c \
	rbug_demarshal.c \
	rbug_texture.c \
	rbug_shader.c \
	rtasm_cpu.c \
	rtasm_execmem.c \
	rtasm_x86sse.c \
	tgsi_build.c \
	tgsi_dump.c \
	tgsi_exec.c \
	tgsi_info.c \
	tgsi_iterate.c \
	tgsi_parse.c \
	tgsi_sanity.c \
	tgsi_scan.c \
	tgsi_strings.c \
	tgsi_text.c \
	tgsi_transform.c \
	tgsi_ureg.c \
	tgsi_util.c \
	translate.c \
	translate_cache.c \
	translate_generic.c \
	translate_sse.c \
	u_debug.c \
	u_debug_describe.c \
	u_debug_flush.c \
	u_debug_memory.c \
	u_debug_refcnt.c \
	u_debug_stack.c \
	u_debug_symbol.c \
	u_dump_defines.c \
	u_dump_state.c \
	u_bitmask.c \
	u_blit.c \
	u_blitter.c \
	u_cache.c \
	u_caps.c \
	u_cpu_detect.c \
	u_dl.c \
	u_draw.c \
	u_draw_quad.c \
	u_format.c \
	u_format_etc.c \
	u_format_other.c \
	u_format_latc.c \
	u_format_s3tc.c \
	u_format_rgtc.c \
	u_format_srgb.c \
	u_format_table.c \
	u_format_tests.c \
	u_format_yuv.c \
	u_format_zs.c \
	u_framebuffer.c \
	u_gen_mipmap.c \
	u_handle_table.c \
	u_hash.c \
	u_hash_table.c \
	u_helpers.c \
	u_index_modify.c \
	u_keymap.c \
	u_linear.c \
	u_linkage.c \
	u_network.c \
	u_math.c \
	u_mm.c \
	u_pstipple.c \
	u_ringbuffer.c \
	u_sampler.c \
	u_simple_shaders.c \
	u_slab.c \
	u_snprintf.c \
	u_staging.c \
	u_suballoc.c \
	u_surface.c \
	u_surfaces.c \
	u_texture.c \
	u_tile.c \
	u_transfer.c \
	u_resource.c \
	u_upload_mgr.c \
	u_vbuf.c \
	vl_compositor.c \
	vl_csc.c \
	vl_decoder.c \
	vl_idct.c \
	vl_matrix_filter.c \
	vl_mc.c \
	vl_median_filter.c \
	vl_mpeg12_bitstream.c \
	vl_mpeg12_decoder.c \
	vl_vertex_buffers.c \
	vl_video_buffer.c \
	vl_deint_filter.c \
	vl_winsys_dri.c \
	vl_zscan.c

GALLIVM_SOURCES = \
        lp_bld_arit.c \
        lp_bld_arit_overflow.c \
        lp_bld_assert.c \
        lp_bld_bitarit.c \
        lp_bld_const.c \
        lp_bld_conv.c \
        lp_bld_flow.c \
        lp_bld_format_aos.c \
        lp_bld_format_aos_array.c \
	lp_bld_format_float.c \
        lp_bld_format_srgb.c \
        lp_bld_format_soa.c \
        lp_bld_format_yuv.c \
        lp_bld_gather.c \
        lp_bld_init.c \
        lp_bld_intr.c \
        lp_bld_logic.c \
        lp_bld_pack.c \
        lp_bld_printf.c \
        lp_bld_quad.c \
        lp_bld_sample.c \
        lp_bld_sample_aos.c \
        lp_bld_sample_soa.c \
        lp_bld_struct.c \
        lp_bld_swizzle.c \
        lp_bld_tgsi.c \
        lp_bld_tgsi_action.c \
        lp_bld_tgsi_aos.c \
        lp_bld_tgsi_info.c \
        lp_bld_tgsi_soa.c \
        lp_bld_type.c \
        draw_llvm.c \
        draw_llvm_sample.c \
        draw_vs_llvm.c \
        draw_pt_fetch_shade_pipeline_llvm.c

GALLIVM_CPP_SOURCES = \
        lp_bld_debug.cpp \
	lp_bld_misc.cpp

SRCS=	${C_SOURCES}

.if ${XENOCARA_BUILD_GALLIUM} == "llvm"
SRCS+=	${GALLIVM_SOURCES} \
	${GALLIVM_CPP_SOURCES}
.endif

all: lib${LIB}_pic.a

obj: _xenocara_obj

install:
	@echo "Not installing lib${LIB}"

clean: 
	rm -f lib${LIB}_pic.a ${OBJS}

cleandir:	clean

.include <bsd.xorg.mk>

.PATH: ${GALLIUM}/auxiliary/cso_cache
.PATH: ${GALLIUM}/auxiliary/draw
.PATH: ${GALLIUM}/auxiliary/hud
.PATH: ${GALLIUM}/auxiliary/indices
.PATH: ${GALLIUM}/auxiliary/os
.PATH: ${GALLIUM}/auxiliary/pipebuffer
.PATH: ${GALLIUM}/auxiliary/postprocess
.PATH: ${GALLIUM}/auxiliary/rbug
.PATH: ${GALLIUM}/auxiliary/rtasm
.PATH: ${GALLIUM}/auxiliary/tgsi
.PATH: ${GALLIUM}/auxiliary/translate
.PATH: ${GALLIUM}/auxiliary/util
.PATH: ${GALLIUM}/auxiliary/vl
.PATH: ${GALLIUM}/auxiliary/gallivm
.PATH: ${.CURDIR}/../../generated/util
