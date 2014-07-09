# $OpenBSD: Makefile,v 1.3 2014/07/09 21:32:40 jsg Exp $
.include <bsd.xconf.mk>

LIB = r600

C_SOURCES = \
	r600_asm.c \
	r600_blit.c \
	r600_hw_context.c \
	r600_isa.c \
	r600_pipe.c \
	r600_shader.c \
	r600_state.c \
	r700_asm.c \
	evergreen_hw_context.c \
	evergreen_state.c \
	eg_asm.c \
	r600_state_common.c \
	evergreen_compute.c \
	compute_memory_pool.c \
	r600_uvd.c \
	radeon_uvd.c

CXX_SOURCES = \
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

LLVM_C_SOURCES = \
	r600_llvm.c \
	radeon_elf_util.c \
	radeon_llvm_emit.c \
	radeon_llvm_util.c \
	radeon_setup_tgsi_llvm.c

RADEON_C_SOURCES = \
	cayman_msaa.c \
	r600_buffer_common.c \
	r600_pipe_common.c \
	r600_query.c \
	r600_streamout.c \
	r600_texture.c \
	radeon_video.c \
	radeon_uvd.c \
	radeon_vce.c \
	radeon_vce_40_2_2.c

SRCS = \
	$(C_SOURCES) \
	$(RADEON_C_SOURCES) \
	$(CXX_SOURCES)

CPPFLAGS+= -I${GALLIUM}/drivers/r600 -I${GALLIUM}/drivers/r600/sb \
	   -I${GALLIUM}/drivers -I${GALLIUM}/drivers/radeon \
	   -I${MESA}/../../include \
	   -I${MESA} -I${X11BASE}/include -I/usr/include/dev/pci/drm

.if ${XENOCARA_BUILD_GALLIUM} == "llvm"
SRCS+=		${LLVM_C_SOURCES}
CPPFLAGS+=	-DR600_USE_LLVM

PKG_CONFIG?=	usr/bin/pkg-config
ELF_CFLAGS!=	pkg-config --cflags libelf
#ELF_LDFLAGS!=	pkg-config --libs libelf
CPPFLAGS+= ${ELF_CFLAGS}
.endif

.include <bsd.xorg.mk>

.PATH: ${MESA}/program
.PATH: ${GALLIUM}/drivers/r600
.PATH: ${GALLIUM}/drivers/r600/sb
.PATH: ${GALLIUM}/drivers/radeon
