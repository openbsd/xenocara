#	$OpenBSD: Makefile,v 1.1 2022/09/02 05:46:38 jsg Exp $

LIB=	crocuswinsys

NOPROFILE=

SRCS=	crocus_drm_winsys.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/src/gallium/winsys/crocus/drm

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/winsys/crocus/drm
