# $OpenBSD: Makefile,v 1.4 2011/11/23 21:07:12 oga Exp $

LIB=	r200_dri.so

CS_SOURCES = \
	radeon_cs_space_drm.c \
	radeon_bo.c \
	radeon_cs.c

RADEON_COMMON_SOURCES = \
	radeon_bo_legacy.c \
	radeon_buffer_objects.c \
	radeon_common_context.c \
	radeon_common.c \
	radeon_cs_legacy.c \
	radeon_dma.c \
	radeon_debug.c \
	radeon_fbo.c \
	radeon_lock.c \
	radeon_mipmap_tree.c \
	radeon_pixel_read.c \
	radeon_queryobj.c \
	radeon_span.c \
	radeon_texture.c \
	radeon_tex_copy.c \
	radeon_tex_getimage.c \
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
		 $(RADEON_COMMON_SOURCES) \
		 $(CS_SOURCES)

SRCS = $(COMMON_SOURCES) $(DRIVER_SOURCES)

DRIVER_DEFINES = -DRADEON_R200 \
	-DRADEON_COMMON=1 -DRADEON_COMMON_FOR_R200 \
	-I$(MESA)/drivers/dri/r200 \
	-I$(MESA)/drivers/dri/r200/server \
	-I$(MESA)/drivers/dri/radeon \
	-I$(MESA)/drivers/dri/radeon/server

.include <bsd.xorg.mk>

LDADD+=	-ldrm_radeon

.PATH: $(MESA)/drivers/dri/r200
.PATH: $(MESA)/drivers/dri/radeon



