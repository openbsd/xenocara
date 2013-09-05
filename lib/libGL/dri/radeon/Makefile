#	$OpenBSD: Makefile,v 1.6 2013/09/05 15:11:58 jsg Exp $

LIB=	radeon_dri.so

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

DRIVER_SOURCES = \
	radeon_context.c \
	radeon_ioctl.c \
	radeon_screen.c \
	radeon_state.c \
	radeon_state_init.c \
	radeon_tex.c \
	radeon_texstate.c \
	radeon_tcl.c \
	radeon_swtcl.c \
	radeon_maos.c \
	radeon_sanity.c \
	radeon_blit.c \
	$(RADEON_COMMON_SOURCES)

SRCS = \
	$(COMMON_SOURCES) \
	$(DRIVER_SOURCES)

DRIVER_DEFINES = -DRADEON_R100 -DRADEON_COMMON=0 \
	-DHAVE_LIBDRM_RADEON \
	-I$(MESA)/drivers/dri/radeon/server \
	-I$(MESA)/drivers/dri/radeon

.include <bsd.xorg.mk>

LDADD+=	-ldrm_radeon

.PATH: $(MESA)/drivers/dri/radeon
