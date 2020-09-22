#	$OpenBSD: Makefile,v 1.2 2020/09/22 02:09:02 jsg Exp $

LIB=	iriswinsys

NOPROFILE=

SRCS=	iris_drm_winsys.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/src/gallium/winsys/iris/drm

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/winsys/iris/drm
