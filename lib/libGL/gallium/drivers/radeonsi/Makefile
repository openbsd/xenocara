# $OpenBSD: Makefile,v 1.1 2013/09/05 15:12:00 jsg Exp $
.include <bsd.xconf.mk>

LIB = radeonsi

C_SOURCES = \
	r600_blit.c \
	r600_buffer.c \
	r600_hw_context.c \
	radeonsi_pipe.c \
	r600_query.c \
	r600_resource.c \
	radeonsi_shader.c \
	r600_texture.c \
	r600_translate.c \
	radeonsi_pm4.c \
	radeonsi_compute.c \
	si_state.c \
	si_state_streamout.c \
	si_state_draw.c \
	si_commands.c \
	radeonsi_uvd.c \
	radeon_uvd.c

LLVM_C_SOURCES = \
	radeon_llvm_emit.c \
	radeon_llvm_util.c \
	radeon_setup_tgsi_llvm.c

SRCS = \
	$(C_SOURCES)

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
