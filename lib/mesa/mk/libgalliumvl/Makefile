#	$OpenBSD: Makefile,v 1.4 2024/07/19 14:40:43 rsadowski Exp $

LIB=	galliumvl

NOPROFILE=

SRCS=	vl_bicubic_filter.c \
	vl_codec.c \
	vl_compositor.c \
	vl_compositor_cs.c \
	vl_compositor_gfx.c \
	vl_csc.c \
	vl_decoder.c \
	vl_deint_filter.c \
	vl_idct.c \
	vl_matrix_filter.c \
	vl_mc.c \
	vl_median_filter.c \
	vl_mpeg12_bitstream.c \
	vl_mpeg12_decoder.c \
	vl_vertex_buffers.c \
	vl_video_buffer.c \
	vl_zscan.c

SRCS+=	vl_winsys_dri_vgem.c \
	vl_winsys_dri.c \
	vl_winsys_dri3.c \
	vl_winsys_drm.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/auxiliary/vl \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/loader \
		-I${MESA_SRC}/src/util

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/gallium/auxiliary/vl
