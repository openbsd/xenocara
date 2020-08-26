#	$OpenBSD: Makefile,v 1.2 2020/08/26 06:36:08 jsg Exp $

LIB=	r200

NOPROFILE=

SRCS=	radeon_buffer_objects.c \
	radeon_common.c \
	radeon_common_context.c \
	radeon_debug.c \
	radeon_dma.c \
	radeon_fbo.c \
	radeon_fog.c \
	radeon_mipmap_tree.c \
	radeon_pixel_read.c \
	radeon_queryobj.c \
	radeon_span.c \
	radeon_tex_copy.c \
	radeon_texture.c \
	radeon_tile.c \
	r200_blit.c \
	r200_cmdbuf.c \
	r200_context.c \
	r200_fragshader.c \
	r200_ioctl.c \
	r200_maos.c \
	r200_sanity.c \
	r200_state.c \
	r200_state_init.c \
	r200_swtcl.c \
	r200_tcl.c \
	r200_tex.c \
	r200_texstate.c \
	r200_vertprog.c \
	radeon_screen.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DRADEON_R200 \
		-I${MESA_SRC}/src/mesa/drivers/dri/r200 \
		-I${MESA_SRC}/src/mesa/drivers/dri/r200/server \
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

.PATH: ${MESA_SRC}/src/mesa/drivers/dri/r200
.PATH: ${MESA_SRC}/src/mesa/drivers/dri/radeon
