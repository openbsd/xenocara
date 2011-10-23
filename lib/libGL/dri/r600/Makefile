#	$OpenBSD: Makefile,v 1.3 2011/10/23 13:57:36 matthieu Exp $

LIB=	r600_dri.so

COMMON_SOURCES = \
	driverfuncs.c \
	mm.c \
	utils.c \
	texmem.c \
	vblank.c \
	xmlconfig.c \
	dri_util.c

RADEON_COMMON_SOURCES = \
	radeon_bo_legacy.c \
	radeon_common_context.c \
	radeon_buffer_objects.c \
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

DRIVER_SOURCES = \
		 radeon_screen.c \
		 r600_context.c \
		 r600_cmdbuf.c \
		 r600_emit.c       \
		 r700_assembler.c  \
		 r700_fragprog.c \
		 r700_vertprog.c \
		 r700_shader.c \
		 r700_shaderinst.c \
		 r700_ioctl.c \
		 r700_oglprog.c \
		 r700_chip.c     \
		 r700_state.c    \
		 r700_clear.c    \
		 r700_render.c   \
		 r600_tex.c      \
		 r600_texstate.c      \
		 r600_blit.c     \
		 r700_debug.c    \
		 evergreen_context.c \
		 evergreen_state.c \
		 evergreen_tex.c \
		 evergreen_ioctl.c \
		 evergreen_render.c \
		 evergreen_chip.c \
		 evergreen_vertprog.c \
		 evergreen_fragprog.c \
		 evergreen_oglprog.c \
                 evergreen_blit.c \
		 $(RADEON_COMMON_SOURCES) \
		 $(EGL_SOURCES) \
		 $(CS_SOURCES)

SRCS = $(COMMON_SOURCES) $(DRIVER_SOURCES)

DRIVER_DEFINES = -DRADEON_R600 \
	-I$(MESA)/drivers/dri/r600 \
	-I$(MESA)/drivers/dri/r600/server \
	-I$(MESA)/drivers/dri/radeon \
	-I$(MESA)/drivers/dri/radeon/server


.include <bsd.xorg.mk>

LDADD+= -ldrm_radeon

.PATH: $(MESA)/drivers/dri/r600
.PATH: $(MESA)/drivers/dri/radeon

