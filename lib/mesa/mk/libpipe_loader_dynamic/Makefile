#	$OpenBSD: Makefile,v 1.1 2025/06/05 14:18:47 jsg Exp $

.include <bsd.xconf.mk>

LIB=	pipe_loader_dynamic

NOPROFILE=

SRCS=	pipe_loader.c \
	pipe_loader_drm.c \
	pipe_loader_sw.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/auxiliary/pipe-loader \
		-I${MESA_SRC}/src/gallium/winsys \
		-I${MESA_SRC}/src/loader \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/util \
		-DPIPE_SEARCH_DIR=\"${X11BASE}/lib/gallium-pipe\"

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH:	${MESA_SRC}/src/gallium/auxiliary/pipe-loader
