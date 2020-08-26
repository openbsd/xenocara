#	$OpenBSD: Makefile,v 1.1 2020/08/26 06:02:40 jsg Exp $

.include <bsd.xconf.mk>

LIB=	pipe_loader_static

NOPROFILE=

SRCS=	pipe_loader.c \
	pipe_loader_drm.c \
	pipe_loader_sw.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
.if ${XENOCARA_BUILD_DRI:L} == "yes"
CPPFLAGS+=	-DHAVE_PIPE_LOADER_DRI \
		-DHAVE_PIPE_LOADER_KMS
.endif
CPPFLAGS+=	-DDROP_PIPE_LOADER_MISC \
		-DGALLIUM_STATIC_TARGETS=1 \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/auxiliary/pipe-loader \
		-I${MESA_SRC}/src/gallium/winsys \
		-I${MESA_SRC}/src/loader \
		-I${MESA_SRC}/generated/src/util

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/gallium/auxiliary/pipe-loader
