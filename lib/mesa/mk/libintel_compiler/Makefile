#	$OpenBSD: Makefile,v 1.9 2025/06/05 14:18:42 jsg Exp $

LIB=	intel_compiler

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

SRCS+=	brw_builder.cpp \
	brw_cfg.cpp \
	brw_compile_bs.cpp \
	brw_compile_cs.cpp \
	brw_compile_fs.cpp \
	brw_compile_gs.cpp \
	brw_compile_mesh.cpp \
	brw_compile_tcs.cpp \
	brw_compile_tes.cpp \
	brw_compile_vs.cpp \
	brw_compiler.c \
	brw_debug_recompile.c \
	brw_def_analysis.cpp \
	brw_disasm.c \
	brw_disasm_info.cpp \
	brw_eu.c \
	brw_eu_compact.c \
	brw_eu_emit.c \
	brw_eu_validate.c \
	brw_fs.cpp \
	brw_fs_live_variables.cpp \
	brw_fs_nir.cpp \
	brw_fs_thread_payload.cpp \
	brw_fs_visitor.cpp \
	brw_generator.cpp \
	brw_ir_performance.cpp \
	brw_lower.cpp \
	brw_lower_dpas.cpp \
	brw_lower_integer_multiplication.cpp \
	brw_lower_logical_sends.cpp \
	brw_lower_pack.cpp \
	brw_lower_regioning.cpp \
	brw_lower_scoreboard.cpp \
	brw_lower_simd_width.cpp \
	brw_lower_subgroup_ops.cpp \
	brw_nir.c \
	brw_nir_analyze_ubo_ranges.c \
	brw_nir_lower_cooperative_matrix.c \
	brw_nir_lower_cs_intrinsics.c \
	brw_nir_lower_alpha_to_coverage.c \
	brw_nir_lower_intersection_shader.c \
	brw_nir_lower_ray_queries.c \
	brw_nir_lower_rt_intrinsics.c \
	brw_nir_lower_shader_calls.c \
	brw_nir_lower_storage_image.c \
	brw_nir_opt_fsat.c \
	brw_nir_rt.c \
	brw_opt.cpp \
	brw_opt_address_reg_load.cpp \
	brw_opt_algebraic.cpp \
	brw_opt_bank_conflicts.cpp \
	brw_opt_cmod_propagation.cpp \
	brw_opt_combine_constants.cpp \
	brw_opt_copy_propagation.cpp \
	brw_opt_cse.cpp \
	brw_opt_dead_code_eliminate.cpp \
	brw_opt_register_coalesce.cpp \
	brw_opt_saturate_propagation.cpp \
	brw_opt_txf_combiner.cpp \
	brw_opt_virtual_grfs.cpp \
	brw_packed_float.c \
	brw_print.cpp \
	brw_reg.cpp \
	brw_reg_allocate.cpp \
	brw_reg_type.c \
	brw_schedule_instructions.cpp \
	brw_shader.cpp \
	brw_simd_selection.cpp \
	brw_spirv.c \
	brw_validate.cpp \
	brw_vue_map.c \
	brw_workaround.cpp

SRCS+=	brw_nir_lower_fsign.c \
	brw_nir_trig_workarounds.c \
	brw_device_sha1_gen.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/intel \
		-I${MESA_SRC}/src/intel/compiler \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/compiler \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/intel \
		-I${MESA_SRC}/generated/src/intel/dev \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/intel/compiler
.PATH: ${MESA_SRC}/generated/src/intel/compiler
