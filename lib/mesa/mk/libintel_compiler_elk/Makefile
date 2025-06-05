#	$OpenBSD: Makefile,v 1.1 2025/06/05 14:18:43 jsg Exp $

LIB=	intel_compiler_elk

NOPROFILE=

SRCS=	intel_nir.c \
	intel_nir_blockify_uniform_loads.c \
	intel_nir_clamp_image_1d_2d_array_sizes.c \
	intel_nir_clamp_per_vertex_loads.c \
	intel_nir_lower_conversions.c \
	intel_nir_lower_non_uniform_barycentric_at_sample.c \
	intel_nir_lower_non_uniform_resource_intel.c \
	intel_nir_lower_printf.c \
	intel_nir_lower_shading_rate_output.c \
	intel_nir_lower_sparse.c \
	intel_nir_lower_texture.c \
	intel_nir_opt_peephole_ffma.c \
	intel_nir_opt_peephole_imul32x16.c \
	intel_nir_tcs_workarounds.c

SRCS+=	elk_cfg.cpp \
	elk_clip_line.c \
	elk_clip_point.c \
	elk_clip_tri.c \
	elk_clip_unfilled.c \
	elk_clip_util.c \
	elk_compile_clip.c \
	elk_compile_ff_gs.c \
	elk_compile_sf.c \
	elk_compiler.c \
	elk_dead_control_flow.cpp \
	elk_debug_recompile.c \
	elk_disasm.c \
	elk_disasm_info.c \
	elk_eu.c \
	elk_eu_compact.c \
	elk_eu_emit.c \
	elk_eu_util.c \
	elk_eu_validate.c \
	elk_fs_bank_conflicts.cpp \
	elk_fs_cmod_propagation.cpp \
	elk_fs_combine_constants.cpp \
	elk_fs_copy_propagation.cpp \
	elk_fs.cpp \
	elk_fs_cse.cpp \
	elk_fs_dead_code_eliminate.cpp \
	elk_fs_generator.cpp \
	elk_fs_live_variables.cpp \
	elk_fs_lower_pack.cpp \
	elk_fs_lower_regioning.cpp \
	elk_fs_nir.cpp \
	elk_fs_reg_allocate.cpp \
	elk_fs_register_coalesce.cpp \
	elk_fs_saturate_propagation.cpp \
	elk_fs_sel_peephole.cpp \
	elk_fs_thread_payload.cpp \
	elk_fs_validate.cpp \
	elk_fs_visitor.cpp \
	elk_interpolation_map.c \
	elk_ir_performance.cpp \
	elk_lower_logical_sends.cpp \
	elk_nir.c \
	elk_nir_analyze_boolean_resolves.c \
	elk_nir_analyze_ubo_ranges.c \
	elk_nir_attribute_workarounds.c \
	elk_nir_lower_alpha_to_coverage.c \
	elk_nir_lower_cs_intrinsics.c \
	elk_nir_lower_storage_image.c \
	elk_nir_options.c \
	elk_packed_float.c \
	elk_predicated_break.cpp \
	elk_reg_type.c \
	elk_schedule_instructions.cpp \
	elk_shader.cpp \
	elk_simd_selection.cpp \
	elk_spirv.c \
	elk_vec4_cmod_propagation.cpp \
	elk_vec4_copy_propagation.cpp \
	elk_vec4.cpp \
	elk_vec4_cse.cpp \
	elk_vec4_dead_code_eliminate.cpp \
	elk_vec4_generator.cpp \
	elk_vec4_gs_visitor.cpp \
	elk_vec4_live_variables.cpp \
	elk_vec4_nir.cpp \
	elk_vec4_gs_nir.cpp \
	elk_vec4_reg_allocate.cpp \
	elk_vec4_surface_builder.cpp \
	elk_vec4_tcs.cpp \
	elk_vec4_tes.cpp \
	elk_vec4_visitor.cpp \
	elk_vec4_vs_visitor.cpp \
	elk_vue_map.c \
	elk_gfx6_gs_visitor.cpp

SRCS+=	elk_nir_trig_workarounds.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/intel/compiler/elk \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/compiler \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/intel \
		-I${MESA_SRC}/generated/src/intel/dev \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/elk \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/compiler
.PATH: ${MESA_SRC}/src/intel/compiler/elk
.PATH: ${MESA_SRC}/generated/src/intel/compiler/elk
