# $OpenBSD: Makefile,v 1.2 2014/07/09 21:32:40 jsg Exp $
.include <bsd.xconf.mk>

LIB = radeonsi

C_SOURCES = \
	si_blit.c \
	si_commands.c \
	si_compute.c \
	si_descriptors.c \
	si_dma.c \
	si_hw_context.c \
	si_pipe.c \
	si_pm4.c \
	si_shader.c \
	si_state.c \
	si_state_draw.c \
	si_uvd.c

LLVM_C_SOURCES = \
	radeon_elf_util.c \
	radeon_setup_tgsi_llvm.c \
	radeon_llvm_emit.c \
	radeon_llvm_util.c

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
	$(RADEON_C_SOURCES)

CPPFLAGS+= -I${GALLIUM}/drivers/radeonsi \
	   -I${GALLIUM}/drivers -I${GALLIUM}/drivers/radeon \
	   -I${MESA}/../../include \
	   -I${MESA} -I${X11BASE}/include -I/usr/include/dev/pci/drm

SRCS+=		${LLVM_C_SOURCES}
CPPFLAGS+=	-DR600_USE_LLVM

PKG_CONFIG?=	usr/bin/pkg-config
ELF_CFLAGS!=	pkg-config --cflags libelf
#ELF_LDFLAGS!=	pkg-config --libs libelf
CPPFLAGS+= ${ELF_CFLAGS}

.include <bsd.xorg.mk>

.PATH: ${MESA}/program
.PATH: ${GALLIUM}/drivers/radeonsi
.PATH: ${GALLIUM}/drivers/radeon
