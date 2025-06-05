#	$OpenBSD: Makefile,v 1.8 2025/06/05 14:18:38 jsg Exp $

LIB=	amd_common

NOPROFILE=

SRCS=	ac_binary.c \
	ac_cmdbuf.c \
	ac_shader_args.c \
	ac_shader_util.c \
	ac_gather_context_rolls.c \
	ac_gpu_info.c \
	ac_surface.c \
	ac_debug.c \
	ac_descriptors.c \
	ac_formats.c \
	ac_shadowed_regs.c \
	ac_spm.c \
	ac_sqtt.c \
	ac_rgp.c \
	ac_msgpack.c \
	amd_family.c \
	ac_parse_ib.c \
	ac_perfcounter.c \
	ac_pm4.c \
	ac_vcn_dec.c \
	ac_vcn_enc.c \
	ac_uvd_dec.c \
	ac_nir.c \
	ac_nir_opt_outputs.c \
	ac_nir_cull.c \
	ac_nir_create_gs_copy_shader.c \
	ac_nir_lower_esgs_io_to_mem.c \
	ac_nir_lower_global_access.c \
	ac_nir_lower_image_opcodes_cdna.c \
	ac_nir_lower_intrinsics_to_args.c \
	ac_nir_lower_legacy_gs.c \
	ac_nir_lower_legacy_vs.c \
	ac_nir_lower_mem_access_bit_sizes.c \
	ac_nir_lower_resinfo.c \
	ac_nir_lower_taskmesh_io_to_mem.c \
	ac_nir_lower_tess_io_to_mem.c \
	ac_nir_lower_tex.c \
	ac_nir_lower_ngg.c \
	ac_nir_lower_ngg_gs.c \
	ac_nir_lower_ngg_mesh.c \
	ac_nir_lower_ps_early.c \
	ac_nir_lower_ps_late.c \
	ac_nir_lower_sin_cos.c \
	ac_nir_meta_cs_blit.c \
	ac_nir_meta_cs_clear_copy_buffer.c \
	ac_nir_meta_ps_resolve.c \
	ac_nir_opt_pack_half.c \
	ac_nir_opt_shared_append.c \
	ac_nir_prerast_utils.c

SRCS+=	ac_linux_drm.c

SRCS+=	ac_rtld.c \
	ac_rgp_elf_object_pack.c

SRCS+=	gfx10_format_table.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} -DADDR_FASTCALL=
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/amd \
		-I${MESA_SRC}/src/amd/common \
		-I${MESA_SRC}/src/compiler \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/src/util \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/amd/common \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/amd/common
.PATH: ${MESA_SRC}/src/amd/common/nir
.PATH: ${MESA_SRC}/generated/src/amd/common
