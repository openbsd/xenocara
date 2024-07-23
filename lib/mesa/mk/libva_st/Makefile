#	$OpenBSD: Makefile,v 1.2 2024/07/23 08:22:36 jsg Exp $

.include <bsd.xconf.mk>

LIB=	va_st

NOPROFILE=

SRCS=	buffer.c \
	config.c \
	context.c \
	display.c \
	image.c \
	picture.c \
	picture_av1.c \
	picture_av1_enc.c \
	picture_h264.c \
	picture_h264_enc.c \
	picture_hevc.c \
	picture_hevc_enc.c \
	picture_mjpeg.c \
	picture_mpeg12.c \
	picture_mpeg4.c \
	picture_vc1.c \
	picture_vp9.c \
	postproc.c \
	subpicture.c \
	surface.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}

VA_INIT!=	pkg-config --modversion libva \
		    | awk -F. '{printf "__vaDriverInit_%d_%d\n", $$1, $$2}'
CPPFLAGS+=	-DVA_DRIVER_INIT_FUNC=${VA_INIT}

CPPFLAGS+=	-I${MESA_SRC}/src/gallium/frontends/va \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/frontends/va
