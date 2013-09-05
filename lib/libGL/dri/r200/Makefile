# $OpenBSD: Makefile,v 1.6 2013/09/05 15:11:57 jsg Exp $

LIB=	r200_dri.so

RADEON_COMMON_SOURCES = \
	radeon_buffer_objects.c \
	radeon_common_context.c \
	radeon_common.c \
	radeon_dma.c \
	radeon_debug.c \
	radeon_fbo.c \
	radeon_fog.c \
	radeon_mipmap_tree.c \
	radeon_pixel_read.c \
	radeon_queryobj.c \
	radeon_span.c \
	radeon_texture.c \
	radeon_tex_copy.c \
	radeon_tile.c

DRIVER_SOURCES = r200_context.c \
		 r200_ioctl.c \
		 r200_state.c \
		 r200_state_init.c \
		 r200_cmdbuf.c \
		 r200_tex.c \
		 r200_texstate.c \
		 r200_tcl.c \
		 r200_swtcl.c \
		 r200_maos.c \
		 r200_sanity.c \
		 r200_fragshader.c \
		 r200_vertprog.c \
		 r200_blit.c \
		 radeon_screen.c \
		 $(EGL_SOURCES) \
		 $(RADEON_COMMON_SOURCES)

SRCS = $(COMMON_SOURCES) $(DRIVER_SOURCES)

DRIVER_DEFINES = -DRADEON_R200 \
	-DHAVE_LIBDRM_RADEON \
	-DRADEON_COMMON=1 -DRADEON_COMMON_FOR_R200 \
	-I$(MESA)/drivers/dri/r200 \
	-I$(MESA)/drivers/dri/r200/server \
	-I$(MESA)/drivers/dri/radeon \
	-I$(MESA)/drivers/dri/radeon/server

.include <bsd.xorg.mk>

LDADD+=	-ldrm_radeon

.PATH: $(MESA)/drivers/dri/r200
.PATH: $(MESA)/drivers/dri/radeon



