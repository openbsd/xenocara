#	$OpenBSD: Makefile,v 1.6 2023/01/28 08:56:35 jsg Exp $

LIB=	r600

NOPROFILE=

SRCS=	compute_memory_pool.c \
	eg_asm.c \
	eg_debug.c \
	evergreen_compute.c \
	evergreen_hw_context.c \
	evergreen_state.c \
	r600_asm.c \
	r600_blit.c \
	r600_dump.c \
	r600_hw_context.c \
	r600_isa.c \
	r600_pipe.c \
	r600_shader.c \
	r600_state.c \
	r600_state_common.c \
	r600_uvd.c \
	r700_asm.c \
	cayman_msaa.c \
	r600_buffer_common.c \
	r600_gpu_load.c \
	r600_perfcounter.c \
	r600_pipe_common.c \
	r600_query.c \
	r600_streamout.c \
	r600_test_dma.c \
	r600_texture.c \
	r600_viewport.c \
	radeon_uvd.c \
	radeon_vce.c \
	radeon_video.c \
	sb_bc_builder.cpp \
	sb_bc_decoder.cpp \
	sb_bc_dump.cpp \
	sb_bc_finalize.cpp \
	sb_bc_parser.cpp \
	sb_context.cpp \
	sb_core.cpp \
	sb_dce_cleanup.cpp \
	sb_def_use.cpp \
	sb_dump.cpp \
	sb_expr.cpp \
	sb_gcm.cpp \
	sb_gvn.cpp \
	sb_if_conversion.cpp \
	sb_ir.cpp \
	sb_liveness.cpp \
	sb_pass.cpp \
	sb_peephole.cpp \
	sb_psi_ops.cpp \
	sb_ra_checker.cpp \
	sb_ra_coalesce.cpp \
	sb_ra_init.cpp \
	sb_sched.cpp \
	sb_shader.cpp \
	sb_ssa_builder.cpp \
	sb_valtable.cpp \
	sfn_alu_defines.cpp \
	sfn_alu_readport_validation.cpp \
	sfn_assembler.cpp \
	sfn_callstack.cpp \
	sfn_conditionaljumptracker.cpp \
	sfn_debug.cpp \
	sfn_instr.cpp \
	sfn_instr_alu.cpp \
	sfn_instr_alugroup.cpp \
	sfn_instr_controlflow.cpp \
	sfn_instr_export.cpp \
	sfn_instr_fetch.cpp \
	sfn_instr_mem.cpp \
	sfn_instr_lds.cpp \
	sfn_instr_tex.cpp \
	sfn_instrfactory.cpp \
	sfn_liverangeevaluator.cpp \
	sfn_liverangeevaluator_helpers.cpp \
	sfn_memorypool.cpp \
	sfn_nir.cpp \
	sfn_nir_legalize_image_load_store.cpp \
	sfn_nir_lower_64bit.cpp \
	sfn_nir_lower_alu.cpp \
	sfn_nir_lower_tex.cpp \
	sfn_nir_lower_fs_out_to_vector.cpp \
	sfn_nir_lower_tess_io.cpp \
	sfn_nir_vectorize_vs_inputs.c \
	sfn_optimizer.cpp \
	sfn_peephole.cpp \
	sfn_ra.cpp \
	sfn_scheduler.cpp \
	sfn_shader.cpp \
	sfn_shader_cs.cpp \
	sfn_shader_fs.cpp \
	sfn_shader_gs.cpp \
	sfn_shader_tess.cpp \
	sfn_shader_vs.cpp \
	sfn_valuefactory.cpp \
	sfn_virtualvalues.cpp

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-DGALLIUM_R600 \
		-I${MESA_SRC}/src/gallium/drivers/r600 \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/util \
		-I${MESA_SRC}/src/amd/common \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src/gallium/drivers/r600 \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/drivers/r600
.PATH: ${MESA_SRC}/src/gallium/drivers/r600/sb
.PATH: ${MESA_SRC}/src/gallium/drivers/r600/sfn
.PATH: ${MESA_SRC}/generated/src/gallium/drivers/r600
