#	$OpenBSD: Makefile,v 1.5 2023/01/28 08:56:35 jsg Exp $

LIB=	radeonsi

NOPROFILE=

SRCS=	gfx10_query.c \
	gfx10_shader_ngg.c \
	si_blit.c \
	si_buffer.c \
	si_clear.c \
	si_compute.c \
	si_compute_blit.c \
	si_cp_dma.c \
	si_cp_reg_shadowing.c \
	si_debug.c \
	si_descriptors.c \
	si_fence.c \
	si_get.c \
	si_gfx_cs.c \
	si_gpu_load.c \
	si_perfcounter.c \
	si_pipe.c \
	si_pm4.c \
	si_query.c \
	si_nir_optim.c \
	si_sdma_copy_image.c \
	si_shader.c \
	si_shader_info.c \
	si_shader_llvm.c \
	si_shader_llvm_gs.c \
	si_shader_llvm_ps.c \
	si_shader_llvm_resources.c \
	si_shader_llvm_tess.c \
	si_shader_llvm_vs.c \
	si_shader_nir.c \
	si_shaderlib_nir.c \
	si_shaderlib_tgsi.c \
	si_sqtt.c \
	si_state.c \
	si_state_binning.c \
	si_state_msaa.c \
	si_state_shaders.cpp \
	si_state_streamout.c \
	si_state_viewport.c \
	si_test_image_copy_region.c \
	si_test_dma_perf.c \
	si_texture.c \
	si_uvd.c \
	radeon_uvd.c \
	radeon_uvd_enc.c \
	radeon_uvd_enc_1_1.c \
	radeon_vce.c \
	radeon_vce_40_2_2.c \
	radeon_vce_50.c \
	radeon_vce_52.c \
	radeon_vcn.c \
	radeon_vcn_dec.c \
	radeon_vcn_dec_jpeg.c \
	radeon_vcn_enc.c \
	radeon_vcn_enc_1_2.c \
	radeon_vcn_enc_2_0.c \
	radeon_vcn_enc_3_0.c \
	radeon_vcn_enc_4_0.c \
	radeon_video.c \

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-DGALLIUM_RADEONSI \
		-I${MESA_SRC}/src/gallium/drivers/radeonsi \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/util \
		-I${MESA_SRC}/src/amd/common \
		-I${MESA_SRC}/src/amd/llvm \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-I${MESA_SRC}/generated/src/amd/common \
		-I${MESA_SRC}/generated/src/gallium/drivers/radeonsi

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/drivers/radeonsi
.PATH: ${MESA_SRC}/src/gallium/drivers/radeon
