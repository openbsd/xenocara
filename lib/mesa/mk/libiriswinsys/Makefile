#	$OpenBSD: Makefile,v 1.3 2021/07/22 11:10:11 jsg Exp $

LIB=	iriswinsys

NOPROFILE=

SRCS=	iris_drm_winsys.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/winsys/iris/drm

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/winsys/iris/drm
