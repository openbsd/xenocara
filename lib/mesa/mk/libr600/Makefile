#	$OpenBSD: Makefile,v 1.2 2020/09/22 02:09:02 jsg Exp $

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
	sb_valtable.cpp

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-DGALLIUM_R600 \
		-I${MESA_SRC}/src/gallium/drivers/r600 \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/src/mesa \
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
