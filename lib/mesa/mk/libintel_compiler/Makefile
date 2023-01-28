#	$OpenBSD: Makefile,v 1.6 2023/01/28 08:56:34 jsg Exp $

LIB=	intel_compiler

NOPROFILE=

SRCS=	brw_cfg.cpp \
	brw_clip_line.c \
	brw_clip_point.c \
	brw_clip_tri.c \
	brw_clip_unfilled.c \
	brw_clip_util.c \
	brw_compile_clip.c \
	brw_compile_ff_gs.c \
	brw_compile_sf.c \
	brw_compiler.c \
	brw_dead_control_flow.cpp \
	brw_debug_recompile.c \
	brw_disasm.c \
	brw_disasm_info.c \
	brw_eu.c \
	brw_eu_compact.c \
	brw_eu_emit.c \
	brw_eu_util.c \
	brw_eu_validate.c \
	brw_fs_bank_conflicts.cpp \
	brw_fs_cmod_propagation.cpp \
	brw_fs_combine_constants.cpp \
	brw_fs_copy_propagation.cpp \
	brw_fs.cpp \
	brw_fs_cse.cpp \
	brw_fs_dead_code_eliminate.cpp \
	brw_fs_generator.cpp \
	brw_fs_live_variables.cpp \
	brw_fs_lower_pack.cpp \
	brw_fs_lower_regioning.cpp \
	brw_fs_nir.cpp \
	brw_fs_reg_allocate.cpp \
	brw_fs_register_coalesce.cpp \
	brw_fs_saturate_propagation.cpp \
	brw_fs_scoreboard.cpp \
	brw_fs_sel_peephole.cpp \
	brw_fs_thread_payload.cpp \
	brw_fs_validate.cpp \
	brw_fs_visitor.cpp \
	brw_interpolation_map.c \
	brw_ir_performance.cpp \
	brw_kernel.c \
	brw_lower_logical_sends.cpp \
	brw_mesh.cpp \
	brw_nir.c \
	brw_nir_analyze_boolean_resolves.c \
	brw_nir_analyze_ubo_ranges.c \
	brw_nir_attribute_workarounds.c \
	brw_nir_lower_conversions.c \
	brw_nir_lower_cs_intrinsics.c \
	brw_nir_lower_alpha_to_coverage.c \
	brw_nir_lower_intersection_shader.c \
	brw_nir_lower_mem_access_bit_sizes.c \
	brw_nir_lower_ray_queries.c \
	brw_nir_lower_rt_intrinsics.c \
	brw_nir_lower_scoped_barriers.c \
	brw_nir_lower_shader_calls.c \
	brw_nir_lower_shading_rate_output.c \
	brw_nir_lower_storage_image.c \
	brw_nir_opt_peephole_ffma.c \
	brw_nir_rt.c \
	brw_nir_tcs_workarounds.c \
	brw_nir_clamp_image_1d_2d_array_sizes.c \
	brw_packed_float.c \
	brw_predicated_break.cpp \
	brw_reg_type.c \
	brw_schedule_instructions.cpp \
	brw_shader.cpp \
	brw_simd_selection.c \
	brw_vec4_cmod_propagation.cpp \
	brw_vec4_copy_propagation.cpp \
	brw_vec4.cpp \
	brw_vec4_cse.cpp \
	brw_vec4_dead_code_eliminate.cpp \
	brw_vec4_generator.cpp \
	brw_vec4_gs_visitor.cpp \
	brw_vec4_live_variables.cpp \
	brw_vec4_nir.cpp \
	brw_vec4_gs_nir.cpp \
	brw_vec4_reg_allocate.cpp \
	brw_vec4_surface_builder.cpp \
	brw_vec4_tcs.cpp \
	brw_vec4_tes.cpp \
	brw_vec4_visitor.cpp \
	brw_vec4_vs_visitor.cpp \
	brw_vue_map.c \
	gfx6_gs_visitor.cpp

SRCS+=	brw_nir_trig_workarounds.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/intel/compiler \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src/intel \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/compiler
.PATH: ${MESA_SRC}/generated/src/intel/compiler
