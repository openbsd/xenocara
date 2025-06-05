#	$OpenBSD: Makefile,v 1.1 2025/06/05 14:18:46 jsg Exp $

LIB=	loader_x11

NOPROFILE=

SRCS=	loader_x11.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DUSE_DRICONF \
		-DDEFAULT_DRIVER_DIR=\"${X11BASE}/lib/modules/dri\" \
		-I${MESA_SRC}/src/loader \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/util

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/x11
