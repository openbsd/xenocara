#	$OpenBSD: Makefile,v 1.7 2024/04/02 10:42:12 jsg Exp $

LIB=	amd_common

NOPROFILE=

SRCS=	ac_binary.c \
	ac_shader_args.c \
	ac_shader_util.c \
	ac_gpu_info.c \
	ac_surface.c \
	ac_debug.c \
	ac_shadowed_regs.c \
	ac_spm.c \
	ac_sqtt.c \
	ac_rgp.c \
	ac_msgpack.c \
	ac_nir.c \
	ac_nir_opt_outputs.c \
	ac_nir_cull.c \
	ac_nir_lower_esgs_io_to_mem.c \
	ac_nir_lower_global_access.c \
	ac_nir_lower_image_opcodes_cdna.c \
	ac_nir_lower_resinfo.c \
	ac_nir_lower_subdword_loads.c \
	ac_nir_lower_taskmesh_io_to_mem.c \
	ac_nir_lower_tess_io_to_mem.c \
	ac_nir_lower_tex.c \
	ac_nir_lower_ngg.c \
	ac_nir_lower_ps.c \
	amd_family.c \
	ac_perfcounter.c

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
		-I${MESA_SRC}/generated/src/amd/common \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/nir

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/amd/common
.PATH: ${MESA_SRC}/generated/src/amd/common
