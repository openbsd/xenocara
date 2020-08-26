#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:40 jsg Exp $

LIB=	r100

NOPROFILE=

SRCS=	radeon_blit.c \
	radeon_buffer_objects.c \
	radeon_common.c \
	radeon_common_context.c \
	radeon_context.c \
	radeon_debug.c \
	radeon_dma.c \
	radeon_fbo.c \
	radeon_fog.c \
	radeon_ioctl.c \
	radeon_maos.c \
	radeon_mipmap_tree.c \
	radeon_pixel_read.c \
	radeon_queryobj.c \
	radeon_sanity.c \
	radeon_screen.c \
	radeon_span.c \
	radeon_state.c \
	radeon_state_init.c \
	radeon_swtcl.c \
	radeon_tcl.c \
	radeon_tex.c \
	radeon_tex_copy.c \
	radeon_texstate.c \
	radeon_texture.c \
	radeon_tile.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DRADEON_R100 \
		-I${MESA_SRC}/src/mesa/drivers/dri/radeon \
		-I${MESA_SRC}/src/mesa/drivers/dri/radeon/server \
		-I${MESA_SRC}/src/mesa/drivers/dri/common \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/mapi \
		-I${MESA_SRC}/generated/src/util

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/mesa/drivers/dri/radeon
